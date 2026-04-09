/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*================================= INCLUDES =================================*/
#include "core/gui_lv_scene_manage_1.h"
#include <stddef.h>
#include <string.h>

#ifdef __EMB_UTILS_LIB_SUPPORTED__
#   include "emb_list.h"
#else
#   include "core/_internal/emb_list.h"
#endif

/*================================== MACROS ==================================*/
#define GUI_SCENE_HISTORY_DEPTH   8      /* Scene back-navigation stack depth  */
#define GUI_PAGE_NODE_POOL_SIZE   8      /* Max simultaneous page nodes        */

/*================================== TYPES ===================================*/

/** @brief  Page node (one per pushed sub-page, allocated from static pool). */
typedef struct {
    emb_list_t      tNode;              /* Intrusive list node             */
    lv_obj_t       *ptPageRoot;         /* Framework-created container     */
    lv_group_t     *ptGroup;            /* Page's group (resolved at push) */
    int16_t         ai16FocusIdx[GUI_SCENE_GROUP_MAX]; /* Saved ALL groups focus  */
    void          (*pfInit)(lv_obj_t *);/* Page init (for re-creation)     */
    void          (*pfDeinit)(void);    /* User destroy callback           */
    uint8_t         u8GroupIdx;         /* Scene group index (for re-resolve)*/
} gui_page_node_t;

/** @brief  Per-scene runtime object. */
typedef struct {
    gui_scene_cfg_t  tCfg;               /* Registered configuration        */
    lv_obj_t       *ptRoot;             /* Scene root screen object        */
    emb_list_t      tPageHead;          /* Page chain list head            */
    int16_t         ai16BaseFocusIdx[GUI_SCENE_GROUP_MAX]; /* ALL groups base focus */
    lv_group_t     *ptBaseGroup;        /* Scene base-view indev group     */
} gui_scene_obj_t;

/** @brief  Scene navigation history (static stack). */
typedef struct {
    gui_scene_id_t eCurrentId;                          /* Active scene       */
    gui_scene_id_t aeStack[GUI_SCENE_HISTORY_DEPTH];    /* Back-nav stack     */
    int8_t        i8Top;                               /* Stack top, -1=empty*/
} gui_scene_tracker_t;

/*============================= LOCAL VARIABLES ==============================*/

static gui_scene_tracker_t s_tTracker = {
    .eCurrentId = GUI_SCENE_MAX,
    .i8Top      = -1,
};

static gui_scene_obj_t s_atScenes[GUI_SCENE_MAX];

/* Page node static pool */
static gui_page_node_t s_atPagePool[GUI_PAGE_NODE_POOL_SIZE];
static bool            s_abPoolUsed[GUI_PAGE_NODE_POOL_SIZE];

/* Page config registration table (indexed by gui_scene_page_id_t) */
static gui_page_cfg_t s_atPageCfgs[GUI_SCENE_PAGE_MAX];

/*============================ FORWARD DECLARATIONS ==========================*/

/* Scene lifecycle */
static void      scene_teardown       (gui_scene_id_t eId);
static void      scene_setup          (gui_scene_id_t eId);
static void      scene_push_history   (gui_scene_id_t eId);
static lv_obj_t *scene_create_root    (void);
static void      scene_load_scr       (lv_obj_t *ptRoot);

/* Extended data (groups / timers) */
static void      scene_ex_init        (const gui_scene_cfg_t *ptCfg);
static void      scene_ex_destroy     (const gui_scene_cfg_t *ptCfg);
static void      scene_bind_indev     (const gui_scene_cfg_t *ptCfg);

/* Page chain helpers */
static gui_page_node_t *page_node_alloc (void);
static void             page_node_free  (gui_page_node_t *ptNode);
static void            page_chain_clear(gui_scene_id_t eId);

/* Focus save / restore */
static int16_t   focus_save_current   (lv_group_t *ptGroup);
static void      focus_restore_silent (lv_group_t *ptGroup, int16_t i16Idx);
static void      focus_save_all_groups  (const gui_scene_obj_t *ptScene,
                                         int16_t *pai16Out);
static void      focus_restore_all_groups(const gui_scene_obj_t *ptScene,
                                          const int16_t *pai16In);

/*============================== IMPLEMENTATION ==============================*/

/*******************************************************************************
 * @brief   Register a scene into the scene table.
 ******************************************************************************/
bool gui_lv_scene_register(const gui_scene_cfg_t *ptCfg)
{
    if(ptCfg == NULL)                                    return false;
    if(ptCfg->eId >= GUI_SCENE_MAX)                      return false;
    if(ptCfg->pfInit == NULL || ptCfg->pfDeinit == NULL) return false;

    gui_scene_obj_t *ptObj = &s_atScenes[ptCfg->eId];
    ptObj->tCfg           = *ptCfg;
    ptObj->ptRoot          = NULL;
    for(uint8_t i = 0; i < GUI_SCENE_GROUP_MAX; i++)
        ptObj->ai16BaseFocusIdx[i] = -1;
    ptObj->ptBaseGroup     = NULL;
    emb_list_init(&ptObj->tPageHead);
    return true;
}

/*******************************************************************************
 * @brief   Switch to a different scene (record history).
 ******************************************************************************/
void gui_lv_scene_switch(gui_scene_id_t eTargetId)
{
    if(eTargetId >= GUI_SCENE_MAX) return;
    if(s_atScenes[eTargetId].tCfg.pfInit == NULL) return;

    gui_scene_id_t ePrevId = s_tTracker.eCurrentId;

    /* Teardown current scene */
    if(ePrevId < GUI_SCENE_MAX)
    {
        scene_teardown(ePrevId);
        scene_push_history(ePrevId);
    }

    /* Setup new scene */
    s_tTracker.eCurrentId = eTargetId;
    scene_setup(eTargetId);
}

/*******************************************************************************
 * @brief   Go back to the previous scene.
 ******************************************************************************/
void gui_lv_scene_back(void)
{
    if(s_tTracker.i8Top < 0) return;

    gui_scene_id_t ePrevId = s_tTracker.aeStack[s_tTracker.i8Top--];
    if(ePrevId >= GUI_SCENE_MAX) return;
    if(s_atScenes[ePrevId].tCfg.pfInit == NULL) return;

    /* Teardown current scene (do NOT push history — this is back) */
    gui_scene_id_t eCurrentId = s_tTracker.eCurrentId;
    if(eCurrentId < GUI_SCENE_MAX)
    {
        scene_teardown(eCurrentId);
    }

    /* Setup previous scene from scratch */
    s_tTracker.eCurrentId = ePrevId;
    scene_setup(ePrevId);
}

/*******************************************************************************
 * @brief   Register a page config into the page table.
 ******************************************************************************/
bool gui_lv_scene_page_register(const gui_page_cfg_t *ptCfg)
{
    if(ptCfg == NULL)                           return false;
    if(ptCfg->eId >= GUI_SCENE_PAGE_MAX)        return false;
    if(ptCfg->pfInit == NULL)                   return false;

    s_atPageCfgs[ptCfg->eId] = *ptCfg;
    return true;
}

/*******************************************************************************
 * @brief   Push a sub-page onto the current scene's page chain.
 * @note    Destroys the current view's UI (lv_obj_clean) to free memory.
 *          On back, the previous view is re-created via its pfInit.
 ******************************************************************************/
void gui_lv_scene_page_push(gui_scene_page_id_t ePageId)
{
    if(ePageId >= GUI_SCENE_PAGE_MAX) return;

    const gui_page_cfg_t *ptCfg = &s_atPageCfgs[ePageId];
    if(ptCfg->pfInit == NULL) return;

    gui_scene_id_t eId = s_tTracker.eCurrentId;
    if(eId >= GUI_SCENE_MAX) return;

    gui_scene_obj_t *ptScene = &s_atScenes[eId];
    if(ptScene->ptRoot == NULL) return;

    /* ---- Resolve group from current scene's extended data ---- */
    lv_group_t *ptGroup = NULL;
    if(ptScene->tCfg.ptEx != NULL &&
       ptCfg->u8GroupIdx < ptScene->tCfg.ptEx->u8GroupNum &&
       ptScene->tCfg.ptEx->ptSceneGroup != NULL)
    {
        ptGroup = ptScene->tCfg.ptEx->ptSceneGroup[ptCfg->u8GroupIdx];
    }

    /* ---- Save current page's focus (ALL groups) ---- */
    if(emb_list_is_empty(&ptScene->tPageHead))
    {
        /* At scene base view — save ALL groups' focus */
        extern lv_indev_t *indev_keypad;
        lv_group_t *ptCurGroup = NULL;
        if(indev_keypad != NULL)
        {
            ptCurGroup = indev_keypad->group;
        }
        ptScene->ptBaseGroup = ptCurGroup;
        focus_save_all_groups(ptScene, ptScene->ai16BaseFocusIdx);
    }
    else
    {
        /* At a pushed page — save ALL groups' focus, then call pfDeinit */
        emb_list_t *ptTailNode = emb_list_get_tail(&ptScene->tPageHead);
        gui_page_node_t *ptTail = EMB_LIST_ENTRY(ptTailNode,
                             gui_page_node_t, tNode);
        focus_save_all_groups(ptScene, ptTail->ai16FocusIdx);
        if(ptTail->pfDeinit != NULL)
        {
            ptTail->pfDeinit();
        }
    }

    /* ---- Destroy current view UI to free memory ---- */
    lv_obj_clean(ptScene->ptRoot);

    /* ---- Allocate page node ---- */
    gui_page_node_t *ptNew = page_node_alloc();
    if(ptNew == NULL) return; /* pool exhausted */

    /* ---- Create full-screen container ---- */
    lv_obj_t *ptPageRoot = gui_lv_container_init(ptScene->ptRoot, 0, 0,
                                              GUI_LV_SCREEN_WIDTH,
                                              GUI_LV_SCREEN_HEIGHT,
                                              rgb(0, 0, 0), false);
    if(ptPageRoot == NULL)
    {
        page_node_free(ptNew);
        return;
    }

    /* ---- Fill node (store pfInit + u8GroupIdx for re-creation on back) ---- */
    ptNew->ptPageRoot  = ptPageRoot;
    ptNew->ptGroup     = ptGroup;
    ptNew->pfInit      = ptCfg->pfInit;
    ptNew->pfDeinit    = ptCfg->pfDeinit;
    ptNew->u8GroupIdx  = ptCfg->u8GroupIdx;

    /* ---- Call page init ---- */
    ptCfg->pfInit(ptPageRoot);

    /* ---- Bind page's group to indev ---- */
    if(ptGroup != NULL)
    {
        GUI_LV_INDEV_BIND_GROUP(ptGroup);
    }

    /* ---- Add to chain tail ---- */
    emb_list_add_tail(&ptNew->tNode, &ptScene->tPageHead);
}

/*******************************************************************************
 * @brief   Go back one page in the current scene's page chain.
 * @note    Destroys current page UI, then re-creates the previous view by
 *          calling its pfInit again, and silently restores saved focus.
 ******************************************************************************/
void gui_lv_scene_page_back(void)
{
    gui_scene_id_t eId = s_tTracker.eCurrentId;
    if(eId >= GUI_SCENE_MAX) return;

    gui_scene_obj_t *ptScene = &s_atScenes[eId];

    /* Nothing to pop */
    if(emb_list_is_empty(&ptScene->tPageHead)) return;

    /* ---- Get tail node (current page) ---- */
    emb_list_t *ptTailNode = emb_list_get_tail(&ptScene->tPageHead);
    gui_page_node_t *ptTail = EMB_LIST_ENTRY(ptTailNode,
                                             gui_page_node_t, tNode);

    /* ---- Teardown current page ---- */
    if(ptTail->pfDeinit != NULL)
    {
        ptTail->pfDeinit();
    }

    /* ---- Destroy all UI on scene root ---- */
    lv_obj_clean(ptScene->ptRoot);

    /* ---- Remove from chain ---- */
    emb_list_del(&ptTail->tNode);
    page_node_free(ptTail);

    /* ---- Re-create previous view ---- */
    if(!emb_list_is_empty(&ptScene->tPageHead))
    {
        /* Previous is a pushed page — re-create it */
        emb_list_t *ptPrevNode = emb_list_get_tail(&ptScene->tPageHead);
        gui_page_node_t *ptPrev = EMB_LIST_ENTRY(ptPrevNode,
                             gui_page_node_t, tNode);

        /* Resolve group */
        lv_group_t *ptGroup = NULL;
        if(ptScene->tCfg.ptEx != NULL &&
           ptPrev->u8GroupIdx < ptScene->tCfg.ptEx->u8GroupNum &&
           ptScene->tCfg.ptEx->ptSceneGroup != NULL)
        {
            ptGroup = ptScene->tCfg.ptEx->ptSceneGroup[ptPrev->u8GroupIdx];
        }
        ptPrev->ptGroup = ptGroup;

        /* Create container + re-call pfInit */
        lv_obj_t *ptPageRoot = gui_lv_container_init(ptScene->ptRoot, 0, 0,
                                                  GUI_LV_SCREEN_WIDTH,
                                                  GUI_LV_SCREEN_HEIGHT,
                                                  rgb(0, 0, 0), false);
        ptPrev->ptPageRoot = ptPageRoot;

        if(ptPrev->pfInit != NULL)
        {
            ptPrev->pfInit(ptPageRoot);
        }

        /* Bind group + restore ALL groups' focus */
        if(ptGroup != NULL)
        {
            GUI_LV_INDEV_BIND_GROUP(ptGroup);
        }
        focus_restore_all_groups(ptScene, ptPrev->ai16FocusIdx);
    }
    else
    {
        /* Chain is now empty — re-create scene base view */
        ptScene->tCfg.pfInit(ptScene->ptRoot);

        /* Bind base group + restore ALL groups' focus */
        scene_bind_indev(&ptScene->tCfg);

        if(ptScene->ptBaseGroup != NULL)
        {
            GUI_LV_INDEV_BIND_GROUP(ptScene->ptBaseGroup);
        }
        focus_restore_all_groups(ptScene, ptScene->ai16BaseFocusIdx);
    }
}

/*******************************************************************************
 * @brief   Get current scene ID.
 ******************************************************************************/
gui_scene_id_t gui_lv_scene_get_current(void)
{
    return s_tTracker.eCurrentId;
}

/*******************************************************************************
 * @brief   Get current scene root object.
 ******************************************************************************/
lv_obj_t *gui_lv_scene_get_current_root(void)
{
    gui_scene_id_t eId = s_tTracker.eCurrentId;
    if(eId >= GUI_SCENE_MAX) return NULL;
    return s_atScenes[eId].ptRoot;
}

/*******************************************************************************
 * @brief   Get current scene focus index for a specific group.
 * @return  Saved focus index for that group, or -1 if none.
 ******************************************************************************/
int16_t gui_lv_scene_get_focus_index(gui_scene_id_t eSceneId, uint8_t u8GroupIdx)
{
    if(eSceneId >= GUI_SCENE_MAX || u8GroupIdx >= GUI_SCENE_GROUP_MAX)
        return -1;
    return s_atScenes[eSceneId].ai16BaseFocusIdx[u8GroupIdx];
}

/*******************************************************************************
 * @brief   Get current scene page focus index for a specific group.
 * @return  Saved focus index for that group, or -1 if none.
 ******************************************************************************/
int16_t gui_lv_scene_page_get_focus_index(gui_scene_page_id_t ePageId, uint8_t u8GroupIdx)
{
    if(ePageId >= GUI_SCENE_PAGE_MAX || u8GroupIdx >= GUI_SCENE_GROUP_MAX)
        return -1;
    return s_atPagePool[ePageId].ai16FocusIdx[u8GroupIdx];
}

/*=========================== STATIC IMPLEMENTATION ==========================*/

/*******************************************************************************
 * @brief   Full scene setup: create root, ex_init, pfInit, bind, load.
 ******************************************************************************/
static void scene_setup(gui_scene_id_t eId)
{
    gui_scene_obj_t *ptObj = &s_atScenes[eId];
    gui_scene_cfg_t *ptCfg = &ptObj->tCfg;

    /* Reset page chain state */
    emb_list_init(&ptObj->tPageHead);
    for(uint8_t i = 0; i < GUI_SCENE_GROUP_MAX; i++)
        ptObj->ai16BaseFocusIdx[i] = -1;
    ptObj->ptBaseGroup     = NULL;

    /* Create root screen */
    lv_obj_t *ptRoot = scene_create_root();
    ptObj->ptRoot = ptRoot;

    /* Init groups and timers */
    scene_ex_init(ptCfg);

    /* Call user init */
    ptCfg->pfInit(ptRoot);

    /* Load screen */
    scene_load_scr(ptRoot);
    
    /* Auto-bind group[0] to indev */
    scene_bind_indev(ptCfg);

    // /* Load screen */
    // scene_load_scr(ptRoot);
}

/*******************************************************************************
 * @brief   Full scene teardown: clear pages, destroy ex, pfDeinit, null root.
 ******************************************************************************/
static void scene_teardown(gui_scene_id_t eId)
{
    if(eId >= GUI_SCENE_MAX) return;

    gui_scene_obj_t *ptObj = &s_atScenes[eId];
    gui_scene_cfg_t *ptCfg = &ptObj->tCfg;

    /* Clear all pushed pages first */
    page_chain_clear(eId);

    /* Destroy groups and timers */
    scene_ex_destroy(ptCfg);

    /* User cleanup */
    if(ptCfg->pfDeinit != NULL)
    {
        ptCfg->pfDeinit();
    }

    /* Mark root invalid (LVGL auto-deletes old screen on switch) */
    ptObj->ptRoot = NULL;
}

/*******************************************************************************
 * @brief   Push scene ID onto history stack (FIFO drop on overflow).
 ******************************************************************************/
static void scene_push_history(gui_scene_id_t eId)
{
    if(eId >= GUI_SCENE_MAX) return;

    if(s_tTracker.i8Top >= (GUI_SCENE_HISTORY_DEPTH - 1))
    {
        /* Stack full: shift left, drop oldest */
        for(uint8_t i = 1; i < GUI_SCENE_HISTORY_DEPTH; i++)
        {
            s_tTracker.aeStack[i - 1] = s_tTracker.aeStack[i];
        }
        s_tTracker.i8Top = GUI_SCENE_HISTORY_DEPTH - 2;
    }
    s_tTracker.aeStack[++s_tTracker.i8Top] = eId;
}

/*******************************************************************************
 * @brief   Create a new root screen object.
 ******************************************************************************/
static lv_obj_t *scene_create_root(void)
{
    return gui_lv_container_init(NULL, 0, 0,
                             GUI_LV_SCREEN_WIDTH, 
                             GUI_LV_SCREEN_HEIGHT,
                             rgb(0, 0, 0), false);
}

/*******************************************************************************
 * @brief   Load screen (no animation).
 ******************************************************************************/
static void scene_load_scr(lv_obj_t *ptRoot)
{
    if(ptRoot == NULL) return;

    if(lv_scr_act() != NULL)
    {
        lv_scr_load_anim(ptRoot, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    }
    else
    {
        lv_disp_load_scr(ptRoot);
    }
}

/*******************************************************************************
 * @brief   Initialize extended scene data (groups, timers).
 ******************************************************************************/
static void scene_ex_init(const gui_scene_cfg_t *ptCfg)
{
    if(ptCfg == NULL || ptCfg->ptEx == NULL) return;

    /* Validate pointers */
    if(ptCfg->ptEx->u8GroupNum > 0 && ptCfg->ptEx->ptSceneGroup == NULL) return;
    if(ptCfg->ptEx->u8TimerNum > 0 && ptCfg->ptEx->ptSceneTimer == NULL) return;

    /* Create groups */
    for(uint8_t i = 0; i < ptCfg->ptEx->u8GroupNum; i++)
    {
        if(ptCfg->ptEx->ptSceneGroup[i] == NULL)
        {
            ptCfg->ptEx->ptSceneGroup[i] = lv_group_create();
            if(ptCfg->ptEx->ptSceneGroup[i] == NULL)
            {
                scene_ex_destroy(ptCfg);
                return;
            }
        }
    }

    /* Create timers (initially paused) */
    for(uint8_t j = 0; j < ptCfg->ptEx->u8TimerNum; j++)
    {
        if(ptCfg->ptEx->ptSceneTimer[j] == NULL)
        {
            ptCfg->ptEx->ptSceneTimer[j] = lv_timer_create(NULL, 1000, NULL);
            if(ptCfg->ptEx->ptSceneTimer[j] == NULL)
            {
                scene_ex_destroy(ptCfg);
                return;
            }
        }
        GUI_LV_TIMER_STOP(ptCfg->ptEx->ptSceneTimer[j]);
    }
}

/*******************************************************************************
 * @brief   Destroy extended scene data (groups, timers).
 ******************************************************************************/
static void scene_ex_destroy(const gui_scene_cfg_t *ptCfg)
{
    if(ptCfg == NULL || ptCfg->ptEx == NULL) return;

    if(ptCfg->ptEx->u8GroupNum > 0 && ptCfg->ptEx->ptSceneGroup == NULL) return;
    if(ptCfg->ptEx->u8TimerNum > 0 && ptCfg->ptEx->ptSceneTimer == NULL) return;

    /* Destroy timers first */
    for(uint8_t i = 0; i < ptCfg->ptEx->u8TimerNum; i++)
    {
        GUI_LV_TIMER_DESTROY(ptCfg->ptEx->ptSceneTimer[i]);
    }

    /* Destroy groups */
    for(uint8_t j = 0; j < ptCfg->ptEx->u8GroupNum; j++)
    {
        if(ptCfg->ptEx->ptSceneGroup[j] != NULL)
        {
            GUI_LV_GROUP_DESTROY(ptCfg->ptEx->ptSceneGroup[j]);
        }
    }
}

/*******************************************************************************
 * @brief   Auto-bind group[0] to indev_keypad.
 ******************************************************************************/
static void scene_bind_indev(const gui_scene_cfg_t *ptCfg)
{
    if(ptCfg == NULL || ptCfg->ptEx == NULL) return;
    if(ptCfg->ptEx->u8GroupNum == 0 || ptCfg->ptEx->ptSceneGroup == NULL) return;
    if(ptCfg->ptEx->ptSceneGroup[0] == NULL) return;

    GUI_LV_INDEV_BIND_GROUP(ptCfg->ptEx->ptSceneGroup[0]);
}

/*======================== PAGE NODE POOL MANAGEMENT =========================*/

/*******************************************************************************
 * @brief   Allocate a page node from the static pool.
 * @return  Pointer to node, or NULL if pool exhausted.
 ******************************************************************************/
static gui_page_node_t *page_node_alloc(void)
{
    for(uint8_t i = 0; i < GUI_PAGE_NODE_POOL_SIZE; i++)
    {
        if(!s_abPoolUsed[i])
        {
            s_abPoolUsed[i] = true;
            memset(&s_atPagePool[i], 0, sizeof(gui_page_node_t));
            emb_list_init(&s_atPagePool[i].tNode);
            for(uint8_t g = 0; g < GUI_SCENE_GROUP_MAX; g++)
                s_atPagePool[i].ai16FocusIdx[g] = -1;
            return &s_atPagePool[i];
        }
    }
    return NULL;
}

/*******************************************************************************
 * @brief   Return a page node back to the static pool.
 ******************************************************************************/
static void page_node_free(gui_page_node_t *ptNode)
{
    if(ptNode == NULL) return;

    for(uint8_t i = 0; i < GUI_PAGE_NODE_POOL_SIZE; i++)
    {
        if(&s_atPagePool[i] == ptNode)
        {
            s_abPoolUsed[i] = false;
            return;
        }
    }
}

/*******************************************************************************
 * @brief   Clear all pages from a scene's page chain (no focus restore).
 *          Used during scene teardown when groups are about to be destroyed.
 * @note    Only the topmost page has live UI on ptRoot (previous pages' UI
 *          was destroyed on push). Scene teardown deletes ptRoot entirely,
 *          so we only call pfDeinit callbacks and free nodes here.
 ******************************************************************************/
static void page_chain_clear(gui_scene_id_t eId)
{
    if(eId >= GUI_SCENE_MAX) return;

    gui_scene_obj_t *ptScene = &s_atScenes[eId];

    /* Pop from tail to head — only call pfDeinit + free nodes */
    while(!emb_list_is_empty(&ptScene->tPageHead))
    {
        emb_list_t *ptTailNode = emb_list_get_tail(&ptScene->tPageHead);
        gui_page_node_t *ptTail = EMB_LIST_ENTRY(ptTailNode,
                             gui_page_node_t, tNode);

        if(ptTail->pfDeinit != NULL) ptTail->pfDeinit();

        /* ptPageRoot lives on ptScene->ptRoot which will be deleted
           by scene teardown → no need to lv_obj_del individually */
        ptTail->ptPageRoot = NULL;

        emb_list_del(&ptTail->tNode);
        page_node_free(ptTail);
    }
}

/*========================== FOCUS SAVE / RESTORE ============================*/

/*******************************************************************************
 * @brief   Save the current focus index of a group.
 * @param   ptGroup: Group to inspect.
 * @return  Focus index (>=0), or -1 if no focus / NULL group.
 ******************************************************************************/
static int16_t focus_save_current(lv_group_t *ptGroup)
{
    if(ptGroup == NULL) return -1;

    lv_obj_t *ptFocused = lv_group_get_focused(ptGroup);
    if(ptFocused == NULL) return -1;

    int16_t i16Idx = 0;
    lv_obj_t **pp;
    _LV_LL_READ(&ptGroup->obj_ll, pp) {
        if(*pp == ptFocused)
        {
            return i16Idx;
        }
        i16Idx++;
    }
    return -1;
}

/*******************************************************************************
 * @brief   Silently restore focus to a saved index without triggering events.
 * @param   ptGroup: Group whose focus to change.
 * @param   i16Idx:  Saved focus index. -1 = do nothing.
 * @note    Directly manipulates obj_focus + LV_STATE_FOCUSED visual state.
 *          Does NOT fire LV_EVENT_FOCUSED / DEFOCUSED.
 ******************************************************************************/
static void focus_restore_silent(lv_group_t *ptGroup, int16_t i16Idx)
{
    if(ptGroup == NULL || i16Idx < 0) return;

    int16_t i16Cur = 0;
    lv_obj_t **pp;
    _LV_LL_READ(&ptGroup->obj_ll, pp) {
        if(i16Cur == i16Idx)
        {
            /* Already focused — nothing to do */
            if(ptGroup->obj_focus == pp) return;

            /* Clear previous focus visual state */
            if(ptGroup->obj_focus != NULL && *(ptGroup->obj_focus) != NULL)
            {
                lv_obj_clear_state(*(ptGroup->obj_focus),
                                   LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_KEY);
            }

            /* Set new focus pointer */
            ptGroup->obj_focus = pp;

            /* Apply visual state */
            if(*pp != NULL)
            {
                lv_obj_add_state(*pp, LV_STATE_FOCUSED);
            }
            return;
        }
        i16Cur++;
    }
}

/*******************************************************************************
 * @brief   Save the focus index of ALL groups in the scene.
 * @param   ptScene: Scene object whose groups to inspect.
 * @param   pai16Out: Output array of size GUI_SCENE_GROUP_MAX.
 ******************************************************************************/
static void focus_save_all_groups(const gui_scene_obj_t *ptScene,
                                  int16_t *pai16Out)
{
    for(uint8_t i = 0; i < GUI_SCENE_GROUP_MAX; i++)
        pai16Out[i] = -1;

    if(ptScene->tCfg.ptEx == NULL) return;
    if(ptScene->tCfg.ptEx->ptSceneGroup == NULL) return;

    uint8_t u8Num = ptScene->tCfg.ptEx->u8GroupNum;
    if(u8Num > GUI_SCENE_GROUP_MAX) u8Num = GUI_SCENE_GROUP_MAX;

    for(uint8_t i = 0; i < u8Num; i++)
    {
        pai16Out[i] = focus_save_current(ptScene->tCfg.ptEx->ptSceneGroup[i]);
    }
}

/*******************************************************************************
 * @brief   Restore focus for ALL groups in the scene (silent, no events).
 * @param   ptScene: Scene object whose groups to restore.
 * @param   pai16In: Input array of size GUI_SCENE_GROUP_MAX (saved indices).
 ******************************************************************************/
static void focus_restore_all_groups(const gui_scene_obj_t *ptScene,
                                     const int16_t *pai16In)
{
    if(ptScene->tCfg.ptEx == NULL) return;
    if(ptScene->tCfg.ptEx->ptSceneGroup == NULL) return;

    uint8_t u8Num = ptScene->tCfg.ptEx->u8GroupNum;
    if(u8Num > GUI_SCENE_GROUP_MAX) u8Num = GUI_SCENE_GROUP_MAX;

    for(uint8_t i = 0; i < u8Num; i++)
    {
        focus_restore_silent(ptScene->tCfg.ptEx->ptSceneGroup[i],
                             pai16In[i]);
    }
}

/*=================================== END ====================================*/
