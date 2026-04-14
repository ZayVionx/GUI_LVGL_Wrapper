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

#ifndef __GUI_LV_SCENE_TASK_<NAME>_H__
#define __GUI_LV_SCENE_TASK_<NAME>_H__

/*================================= INCLUDES =================================*/
#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*!
 * \brief GUI scene <NAME> task implementation.
 */
extern
void gui_lv_scene_task_<name>(lv_event_t *e);

/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_TASK_<NAME>_H__ */
