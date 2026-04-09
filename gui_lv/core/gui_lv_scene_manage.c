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
#define __GUI_LV_SCENE_IMPLEMENT__

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "core/gui_lv_scene_manage.h"

/*================================== MACROS ==================================*/
#undef this
#define this        (*ptThis)

// #define GUI_LV_LIST_APPEND_TAIL(__HEAD, __TAIL, __ITEM)
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/

typedef struct {
    lv_obj_t           *ptRoot;
    gui_lv_scene_cfg_t *ptCFG;
} gui_lv_scene_t;

typedef struct {
    lv_obj_t           *ptRoot;
    gui_lv_page_cfg_t  *ptCFG;
} gui_lv_page_t;


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
/*!
 * \brief Scene list head
 */
static emb_list_t s_tSceneHead;
static emb_list_t s_tPageHead ;

/*!
 * \brief Scene pools
 */
static gui_lv_scene_t s_tScenePools[GUI_SCENE_MAX];
static gui_lv_page_t  s_tPagePools[GUI_PAGE_MAX];


/*================================ PROTOTYPES ================================*/
static void __gui_lv_extend_create(gui_lv_extend_t *ptEx);
static void __gui_lv_extend_depose(gui_lv_extend_t *ptEx);

/*============================== IMPLEMENTATION ==============================*/
/*!
 * \brief Initialize the scene and page lists
 */
void __gui_lv_scend_list_init(void)
{
    emb_list_init(&s_tSceneHead);
    emb_list_init(&s_tPageHead) ;
}

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

    /* Initialize focus indices */
    uint8_t chGroupNum = ptThis->ptEx->u8GroupNum;
    if(chGroupNum > 0)
    {
        ptThis->pchFocusIndex = (uint8_t*)malloc(chGroupNum);
        GUI_LV_ASSERT(ptThis->pchFocusIndex != NULL);

        //*! Set all focus indices to 0
        gui_lv_foreach(uint8_t,
                   ptThis->pchFocusIndex,
                   chGroupNum,
                   pFocus)
        {
            *pFocus = 0;
        }
    }
    else
    {
        ptThis->pchFocusIndex = NULL;
    }
}


void gui_lv_scene_switch(gui_scene_id_t eId)
{
    if(eId >= GUI_SCENE_MAX)    return;

    emb_list_add_tail(&s_tScenePools[eId].ptSceneList, 
                      &s_tSceneHead);
    
    
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




static 
GUI_LV_NONNULL(1)
void __gui_lv_extend_create(gui_lv_extend_t *ptEx)
{
    GUI_LV_ASSERT(ptEx != NULL);
    GUI_LV_ASSERT(!(ptEx->u8GroupNum > 0 && ptEx->ptGroup == NULL));
    GUI_LV_ASSERT(!(ptEx->u8TimerNum > 0 && ptEx->ptTimer == NULL));

    /* Create groups */
    gui_lv_foreach(lv_group_t*, 
                   ptEx->ptGroup, 
                   ptEx->u8GroupNum, 
                   group) 
    {
        if(!*group) *group = lv_group_create();
    }

    /* Create timers */
    gui_lv_foreach(lv_timer_t*, 
                   ptEx->ptTimer, 
                   ptEx->u8TimerNum, 
                   timer) 
    {
        if(!*timer) *timer = lv_timer_create(NULL, 500, NULL);
        GUI_LV_TIMER_STOP(*timer);
    }
}


static 
GUI_LV_NONNULL(1)
void __gui_lv_extend_depose(gui_lv_extend_t *ptEx)
{
    GUI_LV_ASSERT(ptEx != NULL);
    GUI_LV_ASSERT(!(ptEx->u8GroupNum > 0 && ptEx->ptGroup == NULL));
    GUI_LV_ASSERT(!(ptEx->u8TimerNum > 0 && ptEx->ptTimer == NULL));

    /* Destroy groups */
    gui_lv_foreach(lv_group_t*, 
                   ptEx->ptGroup, 
                   ptEx->u8GroupNum, 
                   group) 
    {
        if(*group) GUI_LV_GROUP_DESTROY(*group);
    }

    /* Destroy timers */
    gui_lv_foreach(lv_timer_t*, 
                   ptEx->ptTimer, 
                   ptEx->u8TimerNum, 
                   timer) 
    {
        if(*timer) GUI_LV_TIMER_DESTROY(*timer);
    }
    
}
/*=================================== END ====================================*/
