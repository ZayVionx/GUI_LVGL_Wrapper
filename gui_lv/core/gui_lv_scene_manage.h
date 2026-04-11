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

#ifndef __GUI_LV_SCENE_MANAGE_H__
#define __GUI_LV_SCENE_MANAGE_H__

/*================================= INCLUDES =================================*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_scene_id.h>
#else
#   include "gui_lv_scene_id.h"
#endif

#ifdef __EMB_UTILS_LIB_SUPPORTED__
#   include "emb_list.h"
#else
#   include "core/_internal/emb_list.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif

/* OOC header, please DO NOT modify  */
#ifdef __GUI_LV_SCENE_IMPLEMENT__
#   define __GUI_LV_IMPL__
#   undef __GUI_LV_SCENE_IMPLEMENT__
#elif defined(__GUI_LV_SCENE_INHERIT__)
#   undef __GUI_LV_SCENE_INHERIT__
#   define __GUI_LV_INHERIT__
#endif
#include "core/gui_lv_common.h"

/*================================== MACROS ==================================*/
/*!
 * \note do NOT use this macro directly
 */
#define __GUI_LV_SCENE_LIST_INIT()      __gui_lv_scend_list_init()

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/

/*!
 * \brief Switch screen with animation
 */
typedef const struct {
    uint32_t           u32AnimTime;         //!< time of the animation   
    uint32_t           u32AnimDelay;        //!< delay before the transition
    lv_scr_load_anim_t eLoadAnim;           //!< type of the animation (see lv_scr_load_anim_t)
} gui_lv_switch_anim_mode_t;


/*!
 * \brief Extended scene data structure
 */
typedef struct {
    uint8_t      u8GroupNum;                //!< Number of groups in the scene  
    uint8_t      u8TimerNum;                //!< Number of timers in the scene  
    lv_group_t **ptGroup;                   //!< Pointer to scene's group array 
    lv_timer_t **ptTimer;                   //!< Pointer to scene's timer array 
} gui_lv_extend_t;


/*!
 * \brief Scene configuration structure (internal use)
 */
typedef struct {

    gui_lv_scene_id_t  eId ;                //!< Scene identifier            
    gui_lv_extend_t   *ptEx;                //!< Extended scene data (opt.)  

    void (*pfnDraw  )(lv_obj_t *ptRoot);    //!< Scene init callback         
    void (*pfnLoad  )(lv_obj_t *ptRoot);    //!< Scene load callback         
    void (*pfnBind  )(void);                //!< Scene bind callback         
    void (*pfnDepose)(void);                //!< Scene destroy callback   
    
GUI_LV_PRIVATE(
    emb_list_t  tSceneNode;                 //!< Scene list node    
    uint8_t    *pchFocusIndex;              //!< Focus index array pointer
)

} gui_lv_scene_cfg_t;


/*!
 * \brief Page configuration structure
 */
typedef struct {

    gui_lv_page_id_t  eId ;                 //!< Page identifier            
    gui_lv_extend_t  *ptEx;                 //!< Extended scene data (opt.)  

    void (*pfnDraw  )(lv_obj_t *ptRoot);    //!< Page init callback         
    void (*pfnLoad  )(lv_obj_t *ptRoot);    //!< Page load callback         
    void (*pfnBind  )(void);                //!< Page bind callback         
    void (*pfnDepose)(void);                //!< Page destroy callback    

GUI_LV_PRIVATE(
    emb_list_t  tPageNode;                  //!< Page list node  
    uint8_t    *pchFocusIndex;              //!< Focus index array pointer
)

} gui_lv_page_cfg_t;

/*============================= GLOBAL VARIABLES =============================*/

extern 
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_LEFT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_RIGHT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_TOP;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OVER_BOTTOM;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_LEFT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_RIGHT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_TOP;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_MOVE_BOTTOM;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_FADE_IN;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_FADE_OUT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_LEFT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_RIGHT;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_TOP;

extern
gui_lv_switch_anim_mode_t GUI_LV_SWITCH_MODE_OUT_BOTTOM;

/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/

/*----------------------------------------------------------------------------*
 * Scene Management                                                           *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Register a set of scenes to a scene player
 * \param[in] ptThis the target scene player
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_scene_register(gui_lv_scene_cfg_t *ptThis);


/*!
 * \brief Switch to a different scene
 * \param[in] eId the target scene id
 */
extern
void gui_lv_scene_switch(gui_lv_scene_id_t eId);


/*!
 * \brief Switch to a different scene with animation
 *
 * \param[in] eId the target scene id
 * \param[in] eAnimMode the animation mode
 */
extern
void gui_lv_scene_switch_with_anim(gui_lv_scene_id_t eId, 
                                   gui_lv_switch_anim_mode_t eAnimMode);


/*!
 * \brief Go back to the previous scene with animation
 */
extern
void gui_lv_scene_back(void);


/*!
 * \brief Go back to the previous scene with animation
 * \param[in] eAnimMode the animation mode
 */
extern
void gui_lv_scene_back_with_anim(gui_lv_switch_anim_mode_t eAnimMode);



/*----------------------------------------------------------------------------*
 * Page Management                                                            *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Append a set of pages to a scene player
 *
 * \param[in] ptThis the target scene player
 * \param[in] eSceneId the scene id to which the page belongs
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_page_append_to_scene(gui_lv_page_cfg_t *ptThis, 
                                 gui_lv_scene_id_t eSceneId);


/*!
 * \brief Switch to a different page
 * \param[in] eId the target page id
 */
extern
void gui_lv_page_switch(gui_lv_page_id_t eId);


/*!
 * \brief Switch to a different page with animation
 *
 * \param[in] eId the target page id
 * \param[in] eAnimMode the animation mode
 */
extern
void gui_lv_page_switch_with_anim(gui_lv_page_id_t eId, 
                                  gui_lv_switch_anim_mode_t eAnimMode);


/*!
 * \brief Go back to the previous page
 */
extern
void gui_lv_page_back(void);


/*!
 * \brief Go back to the previous page with animation
 * \param[in] eAnimMode the animation mode
 */
extern
void gui_lv_page_back_with_anim(gui_lv_switch_anim_mode_t eAnimMode);



/*----------------------------------------------------------------------------*
 * Utility Functions                                                          *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Get the current scene ID
 * \return The current scene ID
 */
extern
gui_lv_scene_id_t gui_lv_get_scene_id(void);


/*!
 * \brief Get the current page ID
 * \return The current page ID
 */
extern
gui_lv_page_id_t gui_lv_get_page_id(void);

/*============================== IMPLEMENTATION ==============================*/
/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_MANAGE_H__ */
