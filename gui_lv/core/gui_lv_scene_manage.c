/****************************************************************************
*  Copyright 2025 ZJY (Email:zhujinyuan818@gemail.com)                      *
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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "core/gui_lv_scene_manage.h"
#include "helper/include/gui_lv_helper_style.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wpedantic"
#   pragma clang diagnostic ignored "-Wtautological-pointer-compare"
#   pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#   pragma clang diagnostic ignored "-Wswitch-default"
#elif defined(__IS_COMPILER_GCC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wstrict-aliasing"
#   pragma GCC diagnostic ignored "-Wunused-value"

#endif

/*================================== MACROS ==================================*/
#undef this
#define this        (*ptThis)
     
gui_lv_style_t ptRootStyle = {
    .config = {
        GUI_LV_STYLE_RADIUS(0),
        GUI_LV_STYLE_BORDER_WIDTH(0),
        GUI_LV_STYLE_PAD_ALL(0),
        GUI_LV_STYLE_BG_OPA(LV_OPA_COVER)
    },
};

/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GUI_LV_CREATE_CONTAINER_ROOT()                        \
({                                                              \
    lv_obj_t *__ptRoot = gui_lv_container_init(NULL, 0, 0,      \
                            GUI_LV_SCREEN_WIDTH,                \
                            GUI_LV_SCREEN_HEIGHT,               \
                            rgb(0, 0, 0), false);             \
    gui_lv_style_apply(__ptRoot, &ptRootStyle, 0);              \
    __ptRoot;                                                   \
})

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
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_NONE        = {
    .u32AnimTime  = 0,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_NONE,
};
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
    .u32AnimTime  = 150,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_FADE_IN,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_FADE_OUT    = {
    .u32AnimTime  = 150,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_FADE_OUT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_LEFT    = {
    .u32AnimTime  = 180,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_LEFT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_RIGHT   = {
    .u32AnimTime  = 180,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_RIGHT,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_TOP     = {
    .u32AnimTime  = 180,
    .u32AnimDelay = 0,
    .eLoadAnim    = LV_SCR_LOAD_ANIM_OUT_TOP,
};
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_BOTTOM  = {
    .u32AnimTime  = 180,
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
static gui_lv_scene_t s_tScenePools[GUI_LV_SCENE_MAX];
static gui_lv_page_t  s_tPagePools[GUI_LV_PAGE_MAX];


/*================================ PROTOTYPES ================================*/
static void __gui_lv_page_list_pop_stack(void);

static void __gui_lv_scene_list_pop_stack (gui_lv_switch_anim_mode_t eAnimMode);
static void __gui_lv_scene_list_push_stack(gui_lv_scene_id_t         eTargetId, 
                                           gui_lv_switch_anim_mode_t eAnimMode);

static void __gui_lv_extend_create   (gui_lv_extend_t *ptExtend);
static void __gui_lv_extend_depose   (gui_lv_extend_t *ptExtend);
static void __gui_lv_indev_bind_group(gui_lv_extend_t *ptExtend);

/*============================== IMPLEMENTATION ==============================*/
/*!
 * \brief Initialize the scene and page lists
 */
void gui_lv_scene_manage_init(void)
{
    emb_list_init(&s_tSceneHead);
    emb_list_init(&s_tPageHead );
}

/*!
 * \brief Set the home scene
 * \param[in] eId the scene id to set as home
 */
void gui_lv_scene_set_home(gui_lv_scene_id_t eId)
{
    if(eId >= GUI_LV_SCENE_MAX) return;
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG != NULL);

    /* Move the home scene to the head of the list */
    if(&(s_tScenePools[eId].ptCFG->tSceneNode) != s_tSceneHead.next)
    {
        emb_list_add(
            &(s_tScenePools[eId].ptCFG->tSceneNode), 
            &s_tSceneHead);
    }
}

/*!
 * \brief Switch to the home scene
 */
void gui_lv_scene_switch_to_home(void)
{
    if(emb_list_is_empty(&s_tSceneHead)) return;

    /**************************
     *     Clear the page     *
     **************************/
    while(!emb_list_is_empty(&s_tPageHead))
    {
        __gui_lv_page_list_pop_stack();
    }

    /**************************
     * Pop scenes until home *
     **************************/
    while(s_tSceneHead.prev != s_tSceneHead.next)
    {
        gui_lv_scene_cfg_t *ptSceneCFG = EMB_LIST_ENTRY( s_tSceneHead.prev,
                                                         gui_lv_scene_cfg_t,
                                                         tSceneNode);
        emb_list_del(&ptSceneCFG->tSceneNode);
        GUI_LV_INVOKE_RT_VOID(ptSceneCFG->pfnDepose);
        __gui_lv_extend_depose(ptSceneCFG->ptExtend);
        ptSceneCFG->bIsInitExtend = false;
    }

    gui_lv_scene_cfg_t *ptHomeCFG = EMB_LIST_ENTRY( s_tSceneHead.next,
                                                    gui_lv_scene_cfg_t,
                                                    tSceneNode);
    gui_lv_scene_switch(ptHomeCFG->eSceneId);
}

/*----------------------------------------------------------------------------*
 * Scene Management                                                           *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Register a scene configuration
 * \param[in] ptThis the scene configuration to register
 */
GUI_LV_NONNULL(1)
void gui_lv_scene_register(gui_lv_scene_cfg_t *ptThis)
{
    GUI_LV_ASSERT(ptThis != NULL);

    gui_lv_scene_id_t  eId    = ptThis->eSceneId;
    s_tScenePools[eId].ptCFG  = ptThis;
    s_tScenePools[eId].ptRoot = NULL;
    emb_list_init(&ptThis->tSceneNode);

    /* Initialize focus indices */
    uint8_t chGroupNum = ptThis->ptExtend->u8GroupNum;
    if(chGroupNum > 0)
    {
        ptThis->pchFocusIndex = (uint8_t*)malloc(chGroupNum);
        GUI_LV_ASSERT(ptThis->pchFocusIndex != NULL);

        //*! Set all focus indices to 0
        memset( ptThis->pchFocusIndex,
                0,
                sizeof(ptThis->pchFocusIndex[0]) * chGroupNum);
    }
    else  
    {
        ptThis->pchFocusIndex = NULL;
    }
}

#include <stdio.h>
/*!
 * \brief Switch to a different scene
 * \param[in] eId the target scene id
 */
void gui_lv_scene_switch(gui_lv_scene_id_t eId)
{
    if(eId >= GUI_LV_SCENE_MAX)    return;
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG            != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnDraw   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnLoad   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnBind   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnDepose != NULL);

    /**************************
     *     Clear the page     *
     **************************/
    do {
        if(emb_list_is_empty(&s_tPageHead)) break;

        __gui_lv_page_list_pop_stack();
    } while(true);

     /****************************************
     * 1. Clear the prev scene              *
     * 2. Load the new scene with animation *
     ****************************************/
    __gui_lv_scene_list_push_stack(eId, GUI_LV_SWITCH_MODE_NONE);

    emb_list_t *ptNode;
    for(ptNode = s_tSceneHead.next; ptNode != &s_tSceneHead; ptNode = ptNode->next)
    {
        gui_lv_scene_cfg_t *ptSceneCFG = EMB_LIST_ENTRY( ptNode, 
                                                         gui_lv_scene_cfg_t, 
                                                         tSceneNode);
        printf("Scene %d is in the stack\n", ptSceneCFG->eSceneId);
    }
}

/*!
 * \brief Switch to a different scene with animation
 *
 * \param[in] eId the target scene id
 * \param[in] eAnimMode the animation mode
 */
void gui_lv_scene_switch_with_anim(gui_lv_scene_id_t eId, 
                                   gui_lv_switch_anim_mode_t eAnimMode)
{
    if(eId >= GUI_LV_SCENE_MAX)    return;
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG            != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnDraw   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnLoad   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnBind   != NULL);
    GUI_LV_ASSERT(s_tScenePools[eId].ptCFG->pfnDepose != NULL);

    /**************************
     *     Clear the page     *
     **************************/
    do {
        if(emb_list_is_empty(&s_tPageHead)) break;

        __gui_lv_page_list_pop_stack();
    } while(true);

    /****************************************
     * 1. Clear the prev scene              *
     * 2. Load the new scene with animation *
     ****************************************/
    __gui_lv_scene_list_push_stack(eId, eAnimMode);
}

/*!
 * \brief Go back to the previous scene 
 */
void gui_lv_scene_back(void)
{
    if(s_tSceneHead.next->next == &s_tSceneHead) return;

    /**************************
     *     Clear the page     *
     **************************/
    do {
        if(emb_list_is_empty(&s_tPageHead)) break;

        __gui_lv_page_list_pop_stack();
    } while(true);

    /************************************************
     * 1. Clear the current scene                   *
     * 2. Load the previous scene without animation *
     ************************************************/
    __gui_lv_scene_list_pop_stack(GUI_LV_SWITCH_MODE_NONE);
}

/*!
 * \brief Go back to the previous scene with animation
 * \param[in] eAnimMode the animation mode
 */
void gui_lv_scene_back_with_anim(gui_lv_switch_anim_mode_t eAnimMode)
{
    if(s_tSceneHead.next->next == &s_tSceneHead) return;

    /**************************
     *     Clear the page     *
     **************************/
    do {
        if(emb_list_is_empty(&s_tPageHead)) break;

        __gui_lv_page_list_pop_stack();

    } while(true);

    /************************************************
     * 1. Clear the current scene                   *
     * 2. Load the previous scene with animation   *
     ************************************************/
    __gui_lv_scene_list_pop_stack(eAnimMode);
}


/*----------------------------------------------------------------------------*
 * Page Management                                                            *
 *----------------------------------------------------------------------------*/
GUI_LV_NONNULL(1)
void gui_lv_page_append_to_scene(gui_lv_page_cfg_t *ptThis, 
                                 gui_lv_scene_id_t eSceneId)
{
    GUI_LV_ASSERT(ptThis != NULL);

}

void gui_lv_page_switch(gui_lv_page_id_t eId)
{

}


void gui_lv_page_switch_with_anim(gui_lv_page_id_t eId, 
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
/*!
 * \brief Get the current active scene id
 * \return the current active scene id
 */
gui_lv_scene_id_t gui_lv_get_scene_id(void)
{
    GUI_LV_ASSERT(!emb_list_is_empty(&s_tSceneHead));

    gui_lv_scene_cfg_t *ptThis = EMB_LIST_ENTRY( s_tSceneHead.prev, 
                                                 gui_lv_scene_cfg_t, 
                                                 tSceneNode);
    return this.eSceneId;
}

/*!
 * \brief Get the current active page id
 * \return the current active page id
 */
gui_lv_page_id_t gui_lv_get_page_id(void)
{
    GUI_LV_ASSERT(!emb_list_is_empty(&s_tPageHead));
    
    gui_lv_page_cfg_t *ptThis = EMB_LIST_ENTRY( s_tPageHead.prev, 
                                                gui_lv_page_cfg_t, 
                                                tPageNode);
    return this.ePageId;
}



/*=========================== LOCAL IMPLEMENTATION ===========================*/
/*!
 * \brief Pop the top page from the page stack and destroy it
 *
 * \note  This function assumes that the page stack is not empty 
 *        and does not perform any checks.
 */
static void __gui_lv_page_list_pop_stack(void)
{
    gui_lv_page_cfg_t *ptThis  = EMB_LIST_ENTRY( s_tPageHead.prev, 
                                                 gui_lv_page_cfg_t, 
                                                 tPageNode);
    emb_list_del          (&(ptThis->tPageNode));
    GUI_LV_INVOKE_RT_VOID (  ptThis->pfnDepose );
    __gui_lv_extend_depose(  ptThis->ptExtend  );
    ptThis->bIsInitExtend = false;
}

/*!
 * \brief Pop the top scene from the scene stack, destroy it, and load the new top scene
 * \param[in] eAnimMode the animation mode to loading the new scene
 *
 * \note  This function assumes that the scene stack is not empty 
 *        and does not perform any checks.
 */
static void __gui_lv_scene_list_pop_stack(gui_lv_switch_anim_mode_t eAnimMode)
{
    /**************************
     * Clear the current scene*
     **************************/
    do {
        if(s_tSceneHead.next->next == &s_tSceneHead) break;
        
        gui_lv_scene_cfg_t *ptCFG  = EMB_LIST_ENTRY( s_tSceneHead.prev, 
                                                     gui_lv_scene_cfg_t, 
                                                     tSceneNode);
        emb_list_del(&ptCFG->tSceneNode);
        GUI_LV_INVOKE_RT_VOID (ptCFG->pfnDepose);
        __gui_lv_extend_depose(ptCFG->ptExtend);
        ptCFG->bIsInitExtend = false;

    } while(0);

    /**************************
     *     Load the Scene     *
     **************************/
    gui_lv_scene_id_t eId  = EMB_LIST_ENTRY( s_tSceneHead.prev, 
                                             gui_lv_scene_cfg_t, 
                                             tSceneNode)->eSceneId;
    gui_lv_scene_cfg_t *ptThis = s_tScenePools[eId].ptCFG;

    lv_obj_t *ptRoot = __GUI_LV_CREATE_CONTAINER_ROOT();
    s_tScenePools[eId].ptRoot = ptRoot;
    __gui_lv_extend_create(ptThis->ptExtend);
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnDraw, ptRoot);
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnLoad, ptRoot);
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnBind);
    ptThis->bIsInitExtend = true;

    uint32_t           u32AnimTime  = eAnimMode.u32AnimTime ;         
    uint32_t           u32AnimDelay = eAnimMode.u32AnimDelay;
    lv_scr_load_anim_t eLoadAnim    = eAnimMode.eLoadAnim   ;
    lv_scr_load_anim(ptRoot, eLoadAnim, u32AnimTime, u32AnimDelay, true);

    __gui_lv_indev_bind_group(ptThis->ptExtend);
}

/*!
 * \brief Push a new scene onto the scene stack and load it
 * \param[in] eTargetId the target scene id to load
 * \param[in] eAnimMode the animation mode to loading the new scene
 *
 * \note  This function assumes that the scene id is valid 
 *        and does not perform any checks.
 */
static void __gui_lv_scene_list_push_stack(gui_lv_scene_id_t         eTargetId, 
                                           gui_lv_switch_anim_mode_t eAnimMode )
{
    /* -----------------------------------------------------------------------
     * [STEP 1] CLEAR THE PREVIOUS SCENE
     * -----------------------------------------------------------------------*/
    do {
        // if(s_tSceneHead.next->next == &s_tSceneHead) break;
        
        gui_lv_scene_cfg_t *ptPrevScene  = EMB_LIST_ENTRY( s_tSceneHead.prev, 
                                                           gui_lv_scene_cfg_t, 
                                                           tSceneNode);
        // if(ptPrevScene->eSceneId == eTargetId)     break;
        
        if(ptPrevScene->bIsInitExtend)
        {
            __gui_lv_extend_depose(ptPrevScene->ptExtend);
            GUI_LV_INVOKE_RT_VOID (ptPrevScene->pfnDepose);
            ptPrevScene->bIsInitExtend = false;
        }

        if(ptPrevScene->eSceneId == eTargetId)
            emb_list_del(&ptPrevScene->tSceneNode);
    } while(0);

    /* -----------------------------------------------------------------------
     * [STEP 2] NEW SCENE SETUP
     * -----------------------------------------------------------------------*/
    gui_lv_scene_cfg_t *ptThis = s_tScenePools[eTargetId].ptCFG;
    lv_obj_t           *ptRoot = NULL;

    /* [ 2.1 ]  Append List and Extend Create */
    // if(&(ptThis->tSceneNode) != s_tSceneHead.prev)
    {
        emb_list_add_tail( &(ptThis->tSceneNode), 
                           &s_tSceneHead);
                           
        ptRoot = __GUI_LV_CREATE_CONTAINER_ROOT();
        s_tScenePools[eTargetId].ptRoot = ptRoot;
        __gui_lv_extend_create(ptThis->ptExtend);
        ptThis->bIsInitExtend = true;
    }
    // else
    {
        // ptRoot = s_tScenePools[eTargetId].ptRoot;
    }

    /* [ 2.2 ]  Draw, Load and Bind */
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnDraw, ptRoot);
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnLoad, ptRoot);
    GUI_LV_INVOKE_RT_VOID(ptThis->pfnBind);

    /* -----------------------------------------------------------------------
     * [STEP 3] LOAD NEW SCENE WITH ANIMATION
     * -----------------------------------------------------------------------*/
    uint32_t           u32AnimTime  = eAnimMode.u32AnimTime ;         
    uint32_t           u32AnimDelay = eAnimMode.u32AnimDelay;
    lv_scr_load_anim_t eLoadAnim    = eAnimMode.eLoadAnim   ;
    lv_scr_load_anim(ptRoot, eLoadAnim, u32AnimTime, u32AnimDelay, true);

    __gui_lv_indev_bind_group(ptThis->ptExtend);
}


/*!
 * \brief Create groups and timers for a scene based on its extended data
 * \param[in] ptExtend the extended data of the scene
 */
GUI_LV_NONNULL(1)
static void __gui_lv_extend_create(gui_lv_extend_t *ptExtend)
{
    GUI_LV_ASSERT(ptExtend != NULL);
    GUI_LV_ASSERT(!(ptExtend->u8GroupNum > 0 && ptExtend->ptGroup == NULL));
    GUI_LV_ASSERT(!(ptExtend->u8TimerNum > 0 && ptExtend->ptTimer == NULL));

    /* Create groups */
    gui_lv_foreach(lv_group_t*, 
                   ptExtend->ptGroup, 
                   ptExtend->u8GroupNum, 
                   pptGroup) 
    {
        if(*pptGroup == NULL)   *pptGroup = lv_group_create();
    }

    /* Create timers */
    gui_lv_foreach(lv_timer_t*, 
                   ptExtend->ptTimer, 
                   ptExtend->u8TimerNum, 
                   pptTimer) 
    {
        if(*pptTimer == NULL)   *pptTimer = lv_timer_create(NULL, 500, NULL);
        GUI_LV_TIMER_STOP(*pptTimer);
    }
}


/*!
 * \brief Destroy groups and timers of a scene based on its extended data
 * \param[in] ptExtend the extended data of the scene
 */
GUI_LV_NONNULL(1)
static void __gui_lv_extend_depose(gui_lv_extend_t *ptExtend)
{
    GUI_LV_ASSERT(ptExtend != NULL);
    GUI_LV_ASSERT(!(ptExtend->u8GroupNum > 0 && ptExtend->ptGroup == NULL));
    GUI_LV_ASSERT(!(ptExtend->u8TimerNum > 0 && ptExtend->ptTimer == NULL));
    
    /* Destroy groups */
    gui_lv_foreach(lv_group_t*, 
                   ptExtend->ptGroup, 
                   ptExtend->u8GroupNum, 
                   pptGroup) 
    {
        if(*pptGroup != NULL)   GUI_LV_GROUP_DESTROY(*pptGroup);
    }

    /* Destroy timers */
    gui_lv_foreach(lv_timer_t*, 
                   ptExtend->ptTimer, 
                   ptExtend->u8TimerNum, 
                   pptTimer) 
    {
        if(*pptTimer != NULL)   GUI_LV_TIMER_DESTROY(*pptTimer);
    }
}


/*!
 * \brief Bind the first group in the scene's extended data to the input device
 * \param[in] ptExtend the extended data of the scene
 */
GUI_LV_NONNULL(1)
static void __gui_lv_indev_bind_group(gui_lv_extend_t *ptExtend)
{
    GUI_LV_ASSERT(ptExtend != NULL);
    GUI_LV_ASSERT(!(ptExtend->u8GroupNum > 0 && ptExtend->ptGroup == NULL));
    GUI_LV_ASSERT(!(ptExtend->u8TimerNum > 0 && ptExtend->ptTimer == NULL));
    if(ptExtend->u8GroupNum == 0)   return;

    GUI_LV_INDEV_BIND_GROUP(ptExtend->ptGroup[0]);
}

/*=================================== END ====================================*/
