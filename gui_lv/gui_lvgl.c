/****************************************************************************
*  Copyright 2025 ZJY <Email:zhujinyuan818@gmail.com>                       *
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

/*============================ INCLUDES ======================================*/
#if defined(__GUI_LVGL_WRAPPER__)
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
#   include "template/gui_lv_scene_include.h"
#else
/* User scene header includes */

#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
#if !defined(_WIN64)
static gui_lv_data_cfg_t tUiDataCfg = {
    .pfnDataLoad  = (bool (*)(void))NULL,           //!< Load params from Flash
    .pfnDataSave  = (bool (*)(void))NULL,           //!< Write params to Flash
    .pfnDataReset = (bool (*)(void))NULL,           //!< Restore factory data
    .pfnPowerOff  = (void (*)(void))NULL,           //!< System power off 
};
#endif

/*============================ GLOBAL VARIABLES ==============================*/
gui_lv_style_t g_tContDefStyle = {
    .config = {
        GUI_LV_STYLE_RADIUS         (0),
        GUI_LV_STYLE_PAD_ALL        (0),
        GUI_LV_STYLE_BORDER_WIDTH   (0),
        GUI_LV_STYLE_BG_OPA         (LV_OPA_COVER),  
        GUI_LV_STYLE_BG_COLOR       (RGB(0, 0, 0)),
    },
};

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*******************************************************************************
 * @brief Initialise product-specific GUI system data.
 * @note  Configure language, startup scene, beep level, device info, and optional
 *        persistent-data callbacks before styles and scenes are initialised.
 *        Replace the placeholder values in the user block during product porting.
 ******************************************************************************/
static void gui_sys_data_init(void)
{
    /*------------------- user code begin: sys data -------------------*/
#if !defined(_WIN64)
    /* Set language */
	gui_lv_language_t   language = GUI_LV_LANGUAGE_CN;      //!< Need to replace with actual data and remove this comment

    /* Set boot scene ID */
	gui_lv_scene_id_t    startup = 0;                       //!< Need to replace with actual data and remove this comment

    /* Set the buzzer level */
	gui_lv_beep_level_t   volume = GUI_LV_BEEP_LEVEL_1;     //!< Need to replace with actual data and remove this comment

    /* Set device information */
    uint8_t  u8HW_Ver  = 10;                                //!< Need to replace with actual data and remove this comment
    uint8_t  u8SW_Ver  = 10;                                //!< Need to replace with actual data and remove this comment
    uint32_t u32SN_Ver = 1234567890;                        //!< Need to replace with actual data and remove this comment
    
    gui_lv_set_lang            ((gui_lv_language_t)   language);
    gui_lv_set_startup_scene_id((gui_lv_scene_id_t)   startup );
    gui_lv_set_beep_level      ((gui_lv_beep_level_t) volume  );
    gui_lv_set_device_info     ("NAME", u32SN_Ver, u8SW_Ver, u8HW_Ver);

    gui_lv_data_init(&tUiDataCfg);
#else
    gui_lv_set_lang            (GUI_LV_LANGUAGE_CN        );
    gui_lv_set_startup_scene_id((gui_lv_scene_id_t)0      );
    gui_lv_set_beep_level      (GUI_LV_BEEP_LEVEL_1       );
    gui_lv_set_device_info     ("NAME", 1234567890, 11, 12);
#endif
    /*------------------- user code end  : sys data -------------------*/
}

/*******************************************************************************
 * @brief Initialise GUI-wide reusable styles.
 * @note  Put shared styles used by multiple widgets or scenes in the user block.
 *        This runs before scene registration/loading, so scene code can assume
 *        common styles such as g_tContDefStyle are ready.
 ******************************************************************************/
static void gui_common_style_init(void)
{
    /*------------------- user code begin: common style -------------------*/
    gui_lv_style_init(&g_tContDefStyle);
    /*------------------- user code end  : common style -------------------*/
}

/*******************************************************************************
 * @brief Initialise the scene manager and register all available scenes.
 * @note  RTE builds use generated scene registration. Non-RTE builds should add
 *        manual scene registration in the user block if auto-export is disabled
 *        or incomplete. The home scene is selected after registration.
 ******************************************************************************/
static void gui_all_scene_init(void)
{
    gui_lv_scene_manage_init();

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
    __RTE_Acceleration_GUI_LV_ALL_SCENE_INIT();
#else
    /* !!! If this auto initialization function is disabled,
     * manually append all scene initialization logic below */
    __GUI_LV_SCENE_AUTO_INIT_EXPORTED();
#endif

    gui_lv_scene_set_home((gui_lv_scene_id_t)0);
}


/*----------------------------------------------------------------------------*
 * Public API                                                                 *
 *----------------------------------------------------------------------------*/
/*******************************************************************************
 * @brief Initialise the GUI module.
 *
 * @note This API defines a single, stable entry point for GUI bring-up.
 *
 * @note Application-specific configuration should be done in gui_lvgl.c within
 *       the marked "user code begin/end" regions.
 ******************************************************************************/
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
    
    /* Load startup scene */
    gui_lv_scene_switch(
        gui_lv_get_startup_scene_id()
    );
    lv_timer_handler();
}

/*============================ END ===========================================*/
