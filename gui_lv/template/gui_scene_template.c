/****************************************************************************
*  Copyright 2025 ZJY <3102196558@qq.com>                                   *
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

/* ----------------------------------------------------------------------
 * Project:      GUI (LVGL)
 * Title:        gui_scene_template.c
 * Description:  GUI scene template
 *
 * NOTE:
 * - `<name>`     ---> Scene name in lowercase (e.g. "main_menu")
 * - `<NAME>`     ---> Scene name in uppercase (e.g. "MAIN_MENU")
 * - `%Instance%` ---> Scene name in scene id  (e.g. "0/1/2/...")
 * 
 * - `GUI_SCENE_<NAME>` need to be defined in the "gui_scene_id.h" file,
 * 
 * - Do NOT compile this template file directly. For non-RTE usage, replace
 *   `_<name>` manually (and rename files/symbols as needed).
 * --------------------------------------------------------------------
 */

/*============================ INCLUDES ======================================*/
#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

/*============================ MACROS ========================================*/
/*!
 * \brief Scene group/timer config.
 * 
 * \note Set to 0 to disable the feature.
 */
#define GUI_LV_SCENE_GROUP_NUM     0
#define GUI_LV_SCENE_TIMER_NUM     0

/*============================ TYPES =========================================*/
typedef struct {
    lv_obj_t   *ptRoot;                                 /*!< Scene root container */
    
    /* -- insert your members begin -- */

    /* -- insert your members end -- */

    lv_group_t *ptGroup[GUI_LV_SCENE_GROUP_NUM];        /*!< Input device groups */
    lv_timer_t *ptTimer[GUI_LV_SCENE_TIMER_NUM];        /*!< Scene timers */
} gui_scene_t;

/*============================ LOCAL VARIABLES ===============================*/
static gui_scene_t s_tScene;

/*============================ PROTOTYPES ====================================*/
static void __on_scene_<name>_draw(lv_obj_t *ptRoot);
static void __on_scene_<name>_load(lv_obj_t *ptRoot);
static void __on_scene_<name>_depose(void);

#if GUI_LV_SCENE_TIMER_NUM
static void __on_scene_<name>_timer0_cb(lv_timer_t *ptTimer);
#endif

/*============================ IMPLEMENTATION ================================*/
#if GUI_LV_SCENE_TIMER_NUM
static void __on_scene_<name>_timer0_cb(lv_timer_t *ptTimer)
{
    GUI_LV_UNUSED(ptTimer);
}
#endif

/*! 
 * \brief Scene build callback (create widgets/layout).
 * \param[in] ptRoot The root container of this scene.
 */
static void __on_scene_<name>_draw(lv_obj_t *ptRoot)
{
    s_tScene.ptRoot = ptRoot;

    /*------------------------- draw the scene begin -------------------------*/


    /*------------------------- draw the scene end   -------------------------*/
    __on_scene_<name>_load(ptRoot);

#if GUI_LV_SCENE_TIMER_NUM
    GUI_LV_TIMER_SET(s_tScene.ptTimer[0], __on_scene_<name>_timer0_cb, 1000, NULL);
    GUI_LV_TIMER_ALL_STOP(s_tScene.ptTimer, GUI_LV_SCENE_TIMER_NUM);
#endif
}

/*! 
 * \brief Scene resource loading callback.
 * \param[in] ptRoot The root container of this scene.
 */
static void __on_scene_<name>_load(lv_obj_t *ptRoot)
{
    GUI_LV_UNUSED(ptRoot);

    /*------------------------- load the scene begin -------------------------*/


    /*------------------------- load the scene end   -------------------------*/
}

/*! 
 * \brief Scene resource dispose callback.
 */
static void __on_scene_<name>_depose(void)
{
    /*--------------------- insert your depose code begin --------------------*/


    /*--------------------- insert your depose code end  ---------------------*/
}

/*! 
 * \brief Register GUI scene <name>.
 */
void gui_lv_scene_%Instance%_init(void)
{
    static gui_scene_ex_t s_tSceneEX; 
    s_tSceneEX.u8GroupNum   = GUI_LV_SCENE_GROUP_NUM;
    s_tSceneEX.u8TimerNum   = GUI_LV_SCENE_TIMER_NUM;
    s_tSceneEX.ptSceneGroup = GUI_LV_SCENE_GROUP_NUM ? s_tScene.ptGroup 
                                                     : NULL;
    s_tSceneEX.ptSceneTimer = GUI_LV_SCENE_TIMER_NUM ? s_tScene.ptTimer 
                                                     : NULL;

    /* ------------ initialize members of scene begin ------------ */
    const gui_scene_cfg_t c_tCFG = {
        .eId      = GUI_SCENE_<NAME>,
        .ptEx     = &s_tSceneEX,
        .pfInit   = __on_scene_<name>_draw,
        .pfDeinit = __on_scene_<name>_depose,
    };
    /* ------------ initialize members of scene end -------------- */

    gui_lv_scene_register(&c_tCFG);
}

/*============================ END OF FILE ===================================*/
