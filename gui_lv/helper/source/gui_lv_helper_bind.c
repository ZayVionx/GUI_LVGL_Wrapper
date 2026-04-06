/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*================================= INCLUDES =================================*/
#include "helper/include/gui_lv_helper_bind.h"
#include <string.h>

/*================================== MACROS ==================================*/
#define GUI_LV_BIND_SRC_MAX         64
#define GUI_LV_BIND_NODE_MAX        128
#define GUI_LV_BIND_META_MAX        64
#define GUI_LV_BIND_HASH_BUCKETS    32
#define GUI_LV_BIND_INVALID_IDX    (-1)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
typedef enum {
    GUI_LV_BIND_NODE_LABEL = 0,
    GUI_LV_BIND_NODE_ARC,
} gui_lv_bind_node_kind_t;

typedef struct {
    const void               *pvAddr;
    gui_lv_bind_data_type_t   eType;
    gui_lv_bind_edit_cfg_t    tEdit;
    int16_t                   i16NodeHead;
    int16_t                   i16HashNext;
    bool                      bUsed;
} gui_lv_bind_src_t;

typedef struct gui_lv_bind_node_t gui_lv_bind_node_t;

struct gui_lv_bind_node_t {
    lv_obj_t                 *ptObj;
    gui_lv_bind_node_kind_t   eKind;
    int16_t                   i16SrcIdx;
    int16_t                   i16NextSrc;
    union {
        struct {
            const char *pchFmt;
        } tLabel;
        struct {
            int32_t i32Min;
            int32_t i32Max;
        } tArc;
    } tCfg;
    void (*pfUpdate)(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc);
    bool bUsed;
};

typedef struct {
    lv_obj_t                 *ptObj;
    lv_obj_t                 *ptRoot;
    int16_t                   i16SrcIdx;
    uint16_t                 *pu16EditAddr;
    uint16_t                  u16Min;
    uint16_t                  u16Max;
    uint16_t                  u16Step;
    gui_lv_bind_setter_t      pfSet;
    gui_lv_bind_getter_t      pfGet;
    gui_lv_bind_action_cb_t   pfAction;
    void                     *pvActionParam;
    bool                      bHasEdit;
    bool                      bHasAction;
    bool                      bUsed;
} gui_lv_bind_meta_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
static gui_lv_bind_src_t  s_atSrcPool[GUI_LV_BIND_SRC_MAX];
static gui_lv_bind_node_t s_atNodePool[GUI_LV_BIND_NODE_MAX];
static gui_lv_bind_meta_t s_atMetaPool[GUI_LV_BIND_META_MAX];
static int16_t            s_ai16HashHead[GUI_LV_BIND_HASH_BUCKETS];

/*================================ PROTOTYPES ================================*/
static uint16_t gui_lv_bind_hash_ptr(const void *pvAddr);
static void gui_lv_bind_init_once(void);
static int16_t gui_lv_bind_src_find(const void *pvAddr);
static int16_t gui_lv_bind_src_ensure(const void *pvAddr, gui_lv_bind_data_type_t eType);
static int16_t gui_lv_bind_node_alloc(void);
static int16_t gui_lv_bind_meta_ensure(lv_obj_t *ptObj);
static gui_lv_bind_meta_t *gui_lv_bind_meta_find(lv_obj_t *ptObj);
static lv_obj_t *gui_lv_bind_find_root(lv_obj_t *ptObj);
static bool gui_lv_bind_update_u16_value(gui_lv_bind_meta_t *ptMeta, int32_t i32Delta);
static void gui_lv_bind_update_label_u8(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc);
static void gui_lv_bind_update_label_u16(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc);
static void gui_lv_bind_update_arc_u8(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc);

/*============================== IMPLEMENTATION ==============================*/
static uint16_t gui_lv_bind_hash_ptr(const void *pvAddr)
{
    uintptr_t uAddr = (uintptr_t)pvAddr;
    return (uint16_t)((uAddr >> 2U) & (GUI_LV_BIND_HASH_BUCKETS - 1U));
}

static void gui_lv_bind_init_once(void)
{
    static bool s_bInited = false;

    if(s_bInited) return;

    for(uint16_t i = 0; i < GUI_LV_BIND_HASH_BUCKETS; i++)
        s_ai16HashHead[i] = GUI_LV_BIND_INVALID_IDX;

    s_bInited = true;
}

static int16_t gui_lv_bind_src_find(const void *pvAddr)
{
    uint16_t u16Bucket;
    int16_t  i16Idx;

    if(pvAddr == NULL) return GUI_LV_BIND_INVALID_IDX;

    gui_lv_bind_init_once();
    u16Bucket = gui_lv_bind_hash_ptr(pvAddr);
    i16Idx = s_ai16HashHead[u16Bucket];

    while(i16Idx != GUI_LV_BIND_INVALID_IDX)
    {
        if(s_atSrcPool[i16Idx].bUsed && s_atSrcPool[i16Idx].pvAddr == pvAddr)
            return i16Idx;
        i16Idx = s_atSrcPool[i16Idx].i16HashNext;
    }

    return GUI_LV_BIND_INVALID_IDX;
}

static int16_t gui_lv_bind_src_ensure(const void *pvAddr, gui_lv_bind_data_type_t eType)
{
    int16_t  i16Idx;
    uint16_t u16Bucket;

    i16Idx = gui_lv_bind_src_find(pvAddr);
    if(i16Idx != GUI_LV_BIND_INVALID_IDX)
        return i16Idx;

    for(i16Idx = 0; i16Idx < GUI_LV_BIND_SRC_MAX; i16Idx++)
    {
        if(!s_atSrcPool[i16Idx].bUsed)
        {
            memset(&s_atSrcPool[i16Idx], 0, sizeof(s_atSrcPool[i16Idx]));
            s_atSrcPool[i16Idx].pvAddr = pvAddr;
            s_atSrcPool[i16Idx].eType = eType;
            s_atSrcPool[i16Idx].i16NodeHead = GUI_LV_BIND_INVALID_IDX;
            s_atSrcPool[i16Idx].bUsed = true;

            u16Bucket = gui_lv_bind_hash_ptr(pvAddr);
            s_atSrcPool[i16Idx].i16HashNext = s_ai16HashHead[u16Bucket];
            s_ai16HashHead[u16Bucket] = i16Idx;
            return i16Idx;
        }
    }

    return GUI_LV_BIND_INVALID_IDX;
}

static int16_t gui_lv_bind_node_alloc(void)
{
    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_NODE_MAX; i16Idx++)
    {
        if(!s_atNodePool[i16Idx].bUsed)
        {
            memset(&s_atNodePool[i16Idx], 0, sizeof(s_atNodePool[i16Idx]));
            s_atNodePool[i16Idx].i16SrcIdx = GUI_LV_BIND_INVALID_IDX;
            s_atNodePool[i16Idx].i16NextSrc = GUI_LV_BIND_INVALID_IDX;
            s_atNodePool[i16Idx].bUsed = true;
            return i16Idx;
        }
    }

    return GUI_LV_BIND_INVALID_IDX;
}

static int16_t gui_lv_bind_meta_ensure(lv_obj_t *ptObj)
{
    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_META_MAX; i16Idx++)
    {
        if(s_atMetaPool[i16Idx].bUsed && s_atMetaPool[i16Idx].ptObj == ptObj)
            return i16Idx;
    }

    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_META_MAX; i16Idx++)
    {
        if(!s_atMetaPool[i16Idx].bUsed)
        {
            memset(&s_atMetaPool[i16Idx], 0, sizeof(s_atMetaPool[i16Idx]));
            s_atMetaPool[i16Idx].ptObj = ptObj;
            s_atMetaPool[i16Idx].ptRoot = gui_lv_bind_find_root(ptObj);
            s_atMetaPool[i16Idx].i16SrcIdx = GUI_LV_BIND_INVALID_IDX;
            s_atMetaPool[i16Idx].bUsed = true;
            return i16Idx;
        }
    }

    return GUI_LV_BIND_INVALID_IDX;
}

static gui_lv_bind_meta_t *gui_lv_bind_meta_find(lv_obj_t *ptObj)
{
    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_META_MAX; i16Idx++)
    {
        if(s_atMetaPool[i16Idx].bUsed && s_atMetaPool[i16Idx].ptObj == ptObj)
            return &s_atMetaPool[i16Idx];
    }

    return NULL;
}

static lv_obj_t *gui_lv_bind_find_root(lv_obj_t *ptObj)
{
    lv_obj_t *ptIter = ptObj;

    while(ptIter != NULL)
    {
        lv_obj_t *ptParent = lv_obj_get_parent(ptIter);
        if(ptParent == NULL) return ptIter;
        ptIter = ptParent;
    }

    return ptObj;
}

static bool gui_lv_bind_update_u16_value(gui_lv_bind_meta_t *ptMeta, int32_t i32Delta)
{
    uint16_t u16Value;
    int32_t  i32NewValue;

    if(ptMeta == NULL || !ptMeta->bHasEdit || ptMeta->pu16EditAddr == NULL)
        return false;

    u16Value = *ptMeta->pu16EditAddr;
    i32NewValue = (int32_t)u16Value + i32Delta;

    if(i32NewValue < (int32_t)ptMeta->u16Min)
        i32NewValue = ptMeta->u16Min;
    if(i32NewValue > (int32_t)ptMeta->u16Max)
        i32NewValue = ptMeta->u16Max;

    u16Value = (uint16_t)i32NewValue;

    if(ptMeta->pfSet != NULL)
    {
        if(!ptMeta->pfSet(&u16Value, sizeof(u16Value)))
            return false;
    }
    else
    {
        *ptMeta->pu16EditAddr = u16Value;
    }

    return gui_lv_bind_notify(ptMeta->pu16EditAddr);
}

static void gui_lv_bind_update_label_u8(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc)
{
    const uint8_t *pu8Value;

    if(ptNode == NULL || ptSrc == NULL || ptNode->ptObj == NULL) return;
    if(!lv_obj_is_valid(ptNode->ptObj)) return;

    pu8Value = (const uint8_t *)ptSrc->pvAddr;
    lv_label_set_text_fmt(ptNode->ptObj, ptNode->tCfg.tLabel.pchFmt, *pu8Value);
}

static void gui_lv_bind_update_label_u16(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc)
{
    const uint16_t *pu16Value;

    if(ptNode == NULL || ptSrc == NULL || ptNode->ptObj == NULL) return;
    if(!lv_obj_is_valid(ptNode->ptObj)) return;

    pu16Value = (const uint16_t *)ptSrc->pvAddr;
    lv_label_set_text_fmt(ptNode->ptObj, ptNode->tCfg.tLabel.pchFmt, *pu16Value);
}

static void gui_lv_bind_update_arc_u8(gui_lv_bind_node_t *ptNode, const gui_lv_bind_src_t *ptSrc)
{
    const uint8_t *pu8Value;
    int32_t        i32Value;

    if(ptNode == NULL || ptSrc == NULL || ptNode->ptObj == NULL) return;
    if(!lv_obj_is_valid(ptNode->ptObj)) return;

    pu8Value = (const uint8_t *)ptSrc->pvAddr;
    i32Value = *pu8Value;
    if(i32Value < ptNode->tCfg.tArc.i32Min) i32Value = ptNode->tCfg.tArc.i32Min;
    if(i32Value > ptNode->tCfg.tArc.i32Max) i32Value = ptNode->tCfg.tArc.i32Max;
    lv_arc_set_range(ptNode->ptObj, ptNode->tCfg.tArc.i32Min, ptNode->tCfg.tArc.i32Max);
    lv_arc_set_value(ptNode->ptObj, i32Value);
}

bool gui_lv_bind_label_to_u8(lv_obj_t *ptObj,
                             const uint8_t *pu8Addr,
                             const char *pchFmt)
{
    int16_t i16SrcIdx;
    int16_t i16NodeIdx;

    if(ptObj == NULL || pu8Addr == NULL || pchFmt == NULL) return false;

    i16SrcIdx = gui_lv_bind_src_ensure(pu8Addr, GUI_LV_BIND_DATA_U8);
    i16NodeIdx = gui_lv_bind_node_alloc();
    if(i16SrcIdx == GUI_LV_BIND_INVALID_IDX || i16NodeIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    s_atNodePool[i16NodeIdx].ptObj = ptObj;
    s_atNodePool[i16NodeIdx].eKind = GUI_LV_BIND_NODE_LABEL;
    s_atNodePool[i16NodeIdx].i16SrcIdx = i16SrcIdx;
    s_atNodePool[i16NodeIdx].tCfg.tLabel.pchFmt = pchFmt;
    s_atNodePool[i16NodeIdx].pfUpdate = gui_lv_bind_update_label_u8;
    s_atNodePool[i16NodeIdx].i16NextSrc = s_atSrcPool[i16SrcIdx].i16NodeHead;
    s_atSrcPool[i16SrcIdx].i16NodeHead = i16NodeIdx;
    return true;
}

bool gui_lv_bind_label_to_u16(lv_obj_t *ptObj,
                              const uint16_t *pu16Addr,
                              const char *pchFmt)
{
    int16_t i16SrcIdx;
    int16_t i16NodeIdx;

    if(ptObj == NULL || pu16Addr == NULL || pchFmt == NULL) return false;

    i16SrcIdx = gui_lv_bind_src_ensure(pu16Addr, GUI_LV_BIND_DATA_U16);
    i16NodeIdx = gui_lv_bind_node_alloc();
    if(i16SrcIdx == GUI_LV_BIND_INVALID_IDX || i16NodeIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    s_atNodePool[i16NodeIdx].ptObj = ptObj;
    s_atNodePool[i16NodeIdx].eKind = GUI_LV_BIND_NODE_LABEL;
    s_atNodePool[i16NodeIdx].i16SrcIdx = i16SrcIdx;
    s_atNodePool[i16NodeIdx].tCfg.tLabel.pchFmt = pchFmt;
    s_atNodePool[i16NodeIdx].pfUpdate = gui_lv_bind_update_label_u16;
    s_atNodePool[i16NodeIdx].i16NextSrc = s_atSrcPool[i16SrcIdx].i16NodeHead;
    s_atSrcPool[i16SrcIdx].i16NodeHead = i16NodeIdx;
    return true;
}

bool gui_lv_bind_arc_to_u8(lv_obj_t *ptObj,
                           const uint8_t *pu8Addr,
                           int32_t i32Min,
                           int32_t i32Max)
{
    int16_t i16SrcIdx;
    int16_t i16NodeIdx;

    if(ptObj == NULL || pu8Addr == NULL) return false;

    i16SrcIdx = gui_lv_bind_src_ensure(pu8Addr, GUI_LV_BIND_DATA_U8);
    i16NodeIdx = gui_lv_bind_node_alloc();
    if(i16SrcIdx == GUI_LV_BIND_INVALID_IDX || i16NodeIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    s_atNodePool[i16NodeIdx].ptObj = ptObj;
    s_atNodePool[i16NodeIdx].eKind = GUI_LV_BIND_NODE_ARC;
    s_atNodePool[i16NodeIdx].i16SrcIdx = i16SrcIdx;
    s_atNodePool[i16NodeIdx].tCfg.tArc.i32Min = i32Min;
    s_atNodePool[i16NodeIdx].tCfg.tArc.i32Max = i32Max;
    s_atNodePool[i16NodeIdx].pfUpdate = gui_lv_bind_update_arc_u8;
    s_atNodePool[i16NodeIdx].i16NextSrc = s_atSrcPool[i16SrcIdx].i16NodeHead;
    s_atSrcPool[i16SrcIdx].i16NodeHead = i16NodeIdx;
    return true;
}

bool gui_lv_bind_edit_to_u16(lv_obj_t *ptObj,
                             uint16_t *pu16Addr,
                             uint16_t u16Min,
                             uint16_t u16Max,
                             uint16_t u16Step)
{
    return gui_lv_bind_edit_to_u16_ex(ptObj,
                                      pu16Addr,
                                      u16Min,
                                      u16Max,
                                      u16Step,
                                      NULL);
}

bool gui_lv_bind_edit_to_u16_ex(lv_obj_t *ptObj,
                                uint16_t *pu16Addr,
                                uint16_t u16Min,
                                uint16_t u16Max,
                                uint16_t u16Step,
                                gui_lv_bind_setter_t pfSet)
{
    int16_t            i16MetaIdx;
    int16_t            i16SrcIdx;
    gui_lv_bind_meta_t *ptMeta;

    if(ptObj == NULL || pu16Addr == NULL) return false;

    i16MetaIdx = gui_lv_bind_meta_ensure(ptObj);
    i16SrcIdx = gui_lv_bind_src_ensure(pu16Addr, GUI_LV_BIND_DATA_U16);
    if(i16MetaIdx == GUI_LV_BIND_INVALID_IDX || i16SrcIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    ptMeta = &s_atMetaPool[i16MetaIdx];
    ptMeta->i16SrcIdx = i16SrcIdx;
    ptMeta->pu16EditAddr = pu16Addr;
    ptMeta->u16Min = u16Min;
    ptMeta->u16Max = u16Max;
    ptMeta->u16Step = (u16Step == 0U) ? 1U : u16Step;
    ptMeta->pfSet = pfSet;
    ptMeta->bHasEdit = true;
    return true;
}

bool gui_lv_bind_action(lv_obj_t *ptObj,
                        gui_lv_bind_action_cb_t pfAction,
                        void *pvParam)
{
    int16_t            i16MetaIdx;
    gui_lv_bind_meta_t *ptMeta;

    if(ptObj == NULL || pfAction == NULL) return false;

    i16MetaIdx = gui_lv_bind_meta_ensure(ptObj);
    if(i16MetaIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    ptMeta = &s_atMetaPool[i16MetaIdx];
    ptMeta->pfAction = pfAction;
    ptMeta->pvActionParam = pvParam;
    ptMeta->bHasAction = true;
    return true;
}

bool gui_lv_bind_notify(const void *pvAddr)
{
    int16_t i16SrcIdx;
    int16_t i16NodeIdx;

    i16SrcIdx = gui_lv_bind_src_find(pvAddr);
    if(i16SrcIdx == GUI_LV_BIND_INVALID_IDX)
        return false;

    i16NodeIdx = s_atSrcPool[i16SrcIdx].i16NodeHead;
    while(i16NodeIdx != GUI_LV_BIND_INVALID_IDX)
    {
        gui_lv_bind_node_t *ptNode = &s_atNodePool[i16NodeIdx];
        if(ptNode->bUsed && ptNode->pfUpdate != NULL)
            ptNode->pfUpdate(ptNode, &s_atSrcPool[i16SrcIdx]);
        i16NodeIdx = ptNode->i16NextSrc;
    }

    return true;
}

bool gui_lv_bind_inc_by_obj(lv_obj_t *ptObj)
{
    gui_lv_bind_meta_t *ptMeta = gui_lv_bind_meta_find(ptObj);
    if(ptMeta == NULL) return false;
    return gui_lv_bind_update_u16_value(ptMeta, ptMeta->u16Step);
}

bool gui_lv_bind_dec_by_obj(lv_obj_t *ptObj)
{
    gui_lv_bind_meta_t *ptMeta = gui_lv_bind_meta_find(ptObj);
    if(ptMeta == NULL) return false;
    return gui_lv_bind_update_u16_value(ptMeta, -((int32_t)ptMeta->u16Step));
}

bool gui_lv_bind_exec_by_obj(lv_obj_t *ptObj)
{
    gui_lv_bind_meta_t *ptMeta = gui_lv_bind_meta_find(ptObj);
    if(ptMeta == NULL || !ptMeta->bHasAction || ptMeta->pfAction == NULL)
        return false;

    ptMeta->pfAction(ptMeta->pvActionParam);
    return true;
}

void gui_lv_bind_unbind_by_root(lv_obj_t *ptRoot)
{
    if(ptRoot == NULL) return;

    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_NODE_MAX; i16Idx++)
    {
        if(!s_atNodePool[i16Idx].bUsed) continue;
        if(s_atNodePool[i16Idx].ptObj == NULL) continue;

        if(!lv_obj_is_valid(s_atNodePool[i16Idx].ptObj) ||
              gui_lv_bind_find_root(s_atNodePool[i16Idx].ptObj) == ptRoot)
        {
            s_atNodePool[i16Idx].bUsed = false;
        }
    }

    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_SRC_MAX; i16Idx++)
    {
        int16_t i16Prev = GUI_LV_BIND_INVALID_IDX;
        int16_t i16Node = s_atSrcPool[i16Idx].i16NodeHead;

        if(!s_atSrcPool[i16Idx].bUsed) continue;

        while(i16Node != GUI_LV_BIND_INVALID_IDX)
        {
            int16_t i16Next = s_atNodePool[i16Node].i16NextSrc;
            if(!s_atNodePool[i16Node].bUsed)
            {
                if(i16Prev == GUI_LV_BIND_INVALID_IDX)
                    s_atSrcPool[i16Idx].i16NodeHead = i16Next;
                else
                    s_atNodePool[i16Prev].i16NextSrc = i16Next;
            }
            else
            {
                i16Prev = i16Node;
            }
            i16Node = i16Next;
        }

        if(s_atSrcPool[i16Idx].i16NodeHead == GUI_LV_BIND_INVALID_IDX)
            s_atSrcPool[i16Idx].bUsed = false;
    }

    for(int16_t i16Idx = 0; i16Idx < GUI_LV_BIND_META_MAX; i16Idx++)
    {
        if(!s_atMetaPool[i16Idx].bUsed) continue;
        if(s_atMetaPool[i16Idx].ptRoot == ptRoot ||
           !lv_obj_is_valid(s_atMetaPool[i16Idx].ptObj))
        {
            s_atMetaPool[i16Idx].bUsed = false;
        }
    }
}
