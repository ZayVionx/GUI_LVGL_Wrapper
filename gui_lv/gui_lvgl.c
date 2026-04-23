/****************************************************************************
*  Copyright 2025 ZJY <Email:zhujinyuan818@gemail.com>                      *
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
        GUI_LV_STYLE_BG_OPA(LV_OPA_COVER)
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
    gui_lv_set_lang(GUI_LV_LANGUAGE_TC);
    gui_lv_set_boot_scene_id( (gui_lv_scene_id_t)0);
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
    gui_lv_scene_manage_init();

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
    __GUI_LV_ALL_SCENE_INIT();
#else
    /*-------------------- user code begin: scene register --------------------*/
    /* Example:
     *   gui_lv_scene_0_init();
     */
    /*-------------------- user code end  : scene register --------------------*/
#endif

    /************************************
     *    Set the default home scene    *
     ************************************/
    gui_lv_scene_set_home((gui_lv_scene_id_t)0);
}


/*----------------------------------------------------------------------------*
 * Public API                                                                 *
 *----------------------------------------------------------------------------*/
/*!
 * \brief  GUI system total initialization entry
 * \note   LVGL init + display/indev port + helper + user config + scene load
 */
void gui_lv_init(void)
{
#if !defined(_WIN64)
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
#endif
    __gui_lv_helper_init();

    /* User configuration */
    gui_sys_data_init();
    gui_common_style_init();
    gui_all_scene_init();
    
    /* Load boot scene */
    gui_lv_scene_switch(gui_lv_get_boot_scene_id());
    lv_timer_handler();
}

/*==================================== END ===================================*/
