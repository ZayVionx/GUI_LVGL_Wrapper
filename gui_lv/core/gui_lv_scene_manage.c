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
#include "core/gui_lv_scene_manage.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __EMB_UTILS_LIB_SUPPORTED__
#   include "emb_list.h"
#else
#   include "core/_internal/emb_list.h"
#endif

/*================================== MACROS ==================================*/
#define GUI_LV_LIST_QUEUE_ENQUEUE(__HEAD, __TAIL, __ITEM)
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/

typedef struct {
    lv_obj_t           *ptRoot;
    gui_lv_scene_cfg_t *ptCFG;
    uint8_t            *pchFocusIndex;
} gui_lv_scene_t;

typedef struct {
    lv_obj_t           *ptRoot;
    gui_lv_page_cfg_t  *ptCFG;
    uint8_t            *pchFocusIndex;
} gui_lv_page_t;


typedef struct {
    emb_list_t         tSceneHead;         //!< List head for scenes
}

/*============================= GLOBAL VARIABLES =============================*/

gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_LEFT   = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OVER_LEFT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_RIGHT  = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OVER_RIGHT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_TOP    = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OVER_TOP,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_BOTTOM = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OVER_BOTTOM,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_LEFT   = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_MOVE_LEFT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_RIGHT  = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_MOVE_RIGHT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_TOP    = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_MOVE_TOP,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_BOTTOM = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_MOVE_BOTTOM,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_FADE_IN     = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_FADE_IN,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_FADE_OUT    = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_FADE_OUT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_LEFT    = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_LEFT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_RIGHT   = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_RIGHT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_TOP     = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_TOP,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_BOTTOM  = {
    .u32AnimTime  = 200,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_BOTTOM,
};

/*============================== LOCAL VARIABLES =============================*/

static gui_lv_scene_t s_tScenePools[GUI_SCENE_MAX];
static gui_lv_page_t  s_tPagePools[GUI_PAGE_MAX];


/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/

/*----------------------------------------------------------------------------*
 * Scene Management                                                           *
 *----------------------------------------------------------------------------*/
GUI_LV_NONNULL(1)
void gui_lv_scene_register(gui_lv_scene_cfg_t *ptThis)
{
    GUI_LV_ASSERT(ptThis != NULL);

    gui_lv_scene_id_t  eId    = ptThis->eId;
    s_tScenePools[eId].ptCFG  = ptThis;
    s_tScenePools[eId].ptRoot = NULL;

    for(uint8_t i = 0; i < ptThis->ptEx->u8GroupNum; i++)
    {
        s_tScenePools[eId].pchFocusIndex[i] = 0;
    }



}


void gui_lv_scene_switch(gui_scene_id_t eId)
{

}

void gui_lv_scene_switch_with_anim(gui_scene_id_t eId, 
                                   gui_lv_switch_anim_mode_t eAnimMode)
{

}

void gui_lv_scene_back(void)
{

}

void gui_lv_scene_back_with_anim(gui_lv_switch_anim_mode_t eAnimMode)
{

}


/*----------------------------------------------------------------------------*
 * Page Management                                                            *
 *----------------------------------------------------------------------------*/
GUI_LV_NONNULL(1)
void gui_lv_page_append_to_scene(gui_lv_page_cfg_t *ptThis, 
                                 gui_scene_id_t eSceneId)
{
    GUI_LV_ASSERT(ptThis != NULL);

}

void gui_lv_page_switch(gui_page_id_t eId)
{

}


void gui_lv_page_switch_with_anim(gui_page_id_t eId, 
                                  gui_lv_switch_anim_mode_t eAnimMode)
{

}


void gui_lv_page_back(void)
{

}


void gui_lv_page_back_with_anim(gui_lv_switch_anim_mode_t eAnimMode)
{
    

}


/*----------------------------------------------------------------------------*
 * Utility Functions                                                          *
 *----------------------------------------------------------------------------*/
gui_scene_id_t gui_lv_scene_get_id(void)
{

}


gui_page_id_t gui_lv_page_get_id(void)
{
    
}

/*=================================== END ====================================*/
