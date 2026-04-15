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
 * Title:        gui_scene.c
 * Description:  GUI scene template (CMSIS-Pack/RTE instance-based)
 *
 * NOTE:
 * - `<NAME>`     ---> Scene name in uppercase (e.g. "MAIN_MENU")
 * 
 * - `GUI_SCENE_<NAME>` need to be defined in the "gui_scene_id.h" file,
 * 
 * - Do NOT compile this template file directly. For non-RTE usage, replace
 *   `%Instance%` manually (and rename files/symbols as needed).
 * --------------------------------------------------------------------
 */

/*============================ INCLUDES ======================================*/
#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

#ifdef __RTE_Acceleration_GUI_LVGL_Scene%Instance%__
#   include <gui_lv_scene_%Instance%.h>
#   include <gui_lv_scene_task_%Instance%.h>
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

    lv_group_t *ptGroup[GUI_LV_SCENE_GROUP_NUM];           /*!< Input device groups (optional) */
    lv_timer_t *ptTimer[GUI_LV_SCENE_TIMER_NUM];           /*!< Scene timers (optional) */
} gui_scene_t;

/*============================ LOCAL VARIABLES ===============================*/
static gui_scene_t s_tGUI;

/*============================ PROTOTYPES ====================================*/
static void __on_scene%Instance%_draw(lv_obj_t *ptRoot);
static void __on_scene%Instance%_load(lv_obj_t *ptRoot);
static void __on_scene%Instance%_bind(void);
static void __on_scene%Instance%_depose(void);

/*============================ IMPLEMENTATION ================================*/
#if GUI_LV_SCENE_TIMER_NUM
static void __on_scene%Instance%_timer0_cb(lv_timer_t *ptTimer)
{
    GUI_LV_UNUSED(ptTimer);
}
#endif

/*! 
 * \brief Scene build callback (create widgets/layout).
 * \param[in] ptRoot The root container of this scene.
 */
static void __on_scene%Instance%_draw(lv_obj_t *ptRoot)
{
    s_tGUI.ptRoot           = ptRoot;
    gui_lv_style_apply(ptRoot, &ptContDefStyle, 0);
    gui_lv_language_t eLang = gui_lv_get_current_lang();

    /*------------------------- draw the scene begin -------------------------*/


    /*------------------------- draw the scene end   -------------------------*/
    // __on_scene%Instance%_load(ptRoot);
    // __on_scene%Instance%_bind();
    GUI_LV_INVOKE_RT_VOID(__on_scene%Instance%_load, ptRoot);
    GUI_LV_INVOKE_RT_VOID(__on_scene%Instance%_bind);

#if GUI_LV_SCENE_TIMER_NUM
    GUI_LV_TIMER_SET(s_tGUI.ptTimer[0], __on_scene%Instance%_timer0_cb, 1000, NULL);
    GUI_LV_TIMER_ALL_STOP(s_tGUI.ptTimer, GUI_LV_SCENE_TIMER_NUM);
#endif
}

/*! 
 * \brief Scene resource loading callback.
 * \param[in] ptRoot The root container of this scene.
 */
static void __on_scene%Instance%_load(lv_obj_t *ptRoot)
{
    GUI_LV_UNUSED(ptRoot);

    /*------------------------- load the scene begin -------------------------*/


    /*------------------------- load the scene end   -------------------------*/
}

/*! 
 * \brief Application layer data binding.
 */
static void __on_scene%Instance%_bind(void)
{
    GUI_LV_UNUSED(0);

    /*------------------------- bind the scene begin -------------------------*/

    /*------------------------- bind the scene end   -------------------------*/
}

/*! 
 * \brief Scene resource depose callback.
 */
static void __on_scene%Instance%_depose(void)
{
    GUI_LV_UNUSED(0);

    /*--------------------- insert your depose code begin --------------------*/


    /*--------------------- insert your depose code end  ---------------------*/
}

/*! 
 * \brief Register GUI scene <NAME>.
 */
void gui_lv_scene_%Instance%_init(void)
{
    static gui_lv_extend_t s_tGUIEX; 
    s_tGUIEX.u8GroupNum = GUI_LV_SCENE_GROUP_NUM;
    s_tGUIEX.u8TimerNum = GUI_LV_SCENE_TIMER_NUM;
    s_tGUIEX.ptGroup    = GUI_LV_SCENE_GROUP_NUM ? s_tGUI.ptGroup 
                                                 : NULL;
    s_tGUIEX.ptTimer    = GUI_LV_SCENE_TIMER_NUM ? s_tGUI.ptTimer 
                                                 : NULL;

    /* ------------ initialize members of scene begin ------------ */
    static gui_lv_scene_cfg_t c_tCFG = {
        .eId       = GUI_LV_SCENE_<NAME>,
        .ptEx      = &s_tGUIEX,
        .pfnDraw   = __on_scene%Instance%_draw,
        .pfnLoad   = __on_scene%Instance%_load,
        .pfnBind   = __on_scene%Instance%_bind, 
        .pfnDepose = __on_scene%Instance%_depose,
    };
    /* ------------ initialize members of scene end -------------- */

    gui_lv_scene_register(&c_tCFG);
}

/*============================ END OF FILE ===================================*/
