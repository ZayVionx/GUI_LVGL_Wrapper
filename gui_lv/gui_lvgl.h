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

#ifndef __GUI_LVGL_H__
#define __GUI_LVGL_H__

/*================================= INCLUDES =================================*/
#include "core/gui_lv_scene_manage.h"
#include "helper/include/gui_lv_helper.h"

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_font.h>
#   include <gui_lv_sys_data.h>
#   include <gui_lv_custom_components.h>   
#else
#   include "gui_lv_font.h"
#   include "gui_lv_sys_data.h"
#   include "gui_lv_custom_components.h"
#endif

#if 0
#include "../gui/gui_resource/generated/gui_lv_resource.h"
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
 * \brief Initialise the GUI module.
 *
 * \note This API defines a single, stable entry point for GUI bring-up.
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
