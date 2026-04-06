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

/* ----------------------------------------------------------------------
 * Project:      GUI LVGL Wrapper
 * Title:        gui_lvgl.h
 * Description:  Public façade header for the LVGL wrapper module
 *
 * NOTE:
 * - This is a framework public header; application code should include it.
 * 
 * - To customise boot language/scene and shared styles, edit the "user code"
 *   sections in gui_lvgl.c (__gui_sys_data_init/__gui_common_style_init/...).
 * --------------------------------------------------------------------
 */

#ifndef __GUI_LVGL_H__
#define __GUI_LVGL_H__

/*================================= INCLUDES =================================*/
#include "core/gui_lv_scene_manage.h"
#include "helper/include/gui_lv_helper.h"

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_scene_id.h>
#   include <gui_lv_font.h>
#   include <gui_lv_sys_data.h>
#   include <gui_lv_custom_components.h>   
#else
#   include "gui_scene_id.h"
#   include "gui_lv_font.h"
#   include "gui_lv_sys_data.h"
#   include "gui_lv_custom_components.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
extern gui_lv_style_t ptContDefStyle;

/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*!
 * \brief Initialise the GUI module (LVGL core/port + user hooks)
 *
 * \note This API defines a single, stable entry point for GUI bring-up.
 *       The implementation intentionally separates platform-specific LVGL
 *       initialisation from user extension hooks, so application code stays
 *       portable and testable.
 *
 * \note Application-specific configuration should be done in gui_lvgl.c within
 *       the marked "user code begin/end" regions.
 */
extern
void gui_lv_init(void);

/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LVGL_H__ */
