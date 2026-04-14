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
 * Project:      GUI LVGL Wrapper
 * Title:        gui_lvgl.c
 * Description:  LVGL wrapper bring-up entry (LVGL core/port + user hooks)
 *
 * NOTE:
 * - This file provides the stable entry point: gui_lv_init().
 * - Application integration should normally only edit the regions explicitly
 *   marked as "user code begin/end".
 * - Typical user tasks:
 *   1) Set language / boot scene id in __gui_sys_data_init().
 *   2) Define shared styles (e.g. ptContDefStyle) and initialise them in
 *      __gui_common_style_init().
 *   3) Register scenes in __gui_all_scene_init() for non-RTE builds.
 * - In CMSIS-Pack/RTE builds (defined(__RTE_GUI_LVGL_WRAPPER__)), scenes are
 *   auto-initialised by __GUI_LV_ALL_SCENE_INIT().
 * --------------------------------------------------------------------
 */

/*================================= INCLUDES =================================*/
#if defined(__GUI_LVGL_WRAPPER__)
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
#   include "template/gui_scene_include.h"
#endif

/*================================== MACROS ==================================*/
#define BG_COLOR_CONT_NORMAL    RGB(  0,   0,   0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================== LOCAL VARIABLES =============================*/
/*============================== GLOBAL VARIABLES ============================*/
gui_lv_style_t ptContDefStyle = {
    .config = {
        GUI_LV_STYLE_RADIUS(0),
        GUI_LV_STYLE_BORDER_WIDTH(0),
        GUI_LV_STYLE_PAD_ALL(0),
        GUI_LV_STYLE_BG_COLOR(BG_COLOR_CONT_NORMAL),
    },
};

/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
/*!
 * \note Called by gui_lv_init() after the LVGL core/port initialisation stage.
 *       Modify only the "user code" block below.
 */
static void gui_sys_data_init(void)
{
    /*---------------------- user code begin: sys data ----------------------*/
    /* Override defaults (language/boot scene/etc.) for your application. */
    gui_lv_set_lang(GUI_LV_LANGUAGE_TC);
    gui_lv_set_boot_scene_id( (gui_lv_scene_id_t)(0) );
    /*---------------------- user code end  : sys data ----------------------*/
}

/*!
 * \note Called by gui_lv_init() after gui_sys_data_init().
 *       Modify only the "user code" block below.
 */
static void gui_common_style_init(void)
{
    /*------------------- user code begin: common style -------------------*/
    gui_lv_style_init(&ptContDefStyle);
    
    /*------------------- user code end  : common style -------------------*/
}

/*!
 * \note Called by gui_lv_init() after data/style initialisation.
 *       - RTE build: keep the default __GUI_LV_ALL_SCENE_INIT().
 *       - Non-RTE  : register scenes manually in the "user code" block.
 */
static void gui_all_scene_init(void)
{
    __GUI_LV_SCENE_LIST_INIT();

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
    __GUI_LV_ALL_SCENE_INIT();
#else
    /*-------------------- user code begin: scene register --------------------*/
    /* Example:
     *   gui_lv_scene_0_init();
     *   gui_lv_scene_1_init();
     */

    /*-------------------- user code end  : scene register --------------------*/

#endif
}


/*----------------------------------------------------------------------------*
 * Public API                                                                 *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Initialise the GUI module (LVGL core/port + user hooks)
 *
 * \note This API defines a single, stable entry point for GUI bring-up.
 *       The implementation intentionally separates platform-specific LVGL
 *       initialisation from user extension hooks, so application code stays
 *       portable and testable.
 */
void gui_lv_init(void)
{
#if !defined(_WIN64)
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
#endif
    __gui_lv_helper_init();

    /* initialize members of scene begin */
    gui_sys_data_init();
    gui_common_style_init();
    gui_all_scene_init();
    /* initialize members of scene end */
    
    gui_lv_scene_switch( 
        gui_lv_get_boot_scene_id());

    lv_timer_handler();
}

/*==================================== END ===================================*/
