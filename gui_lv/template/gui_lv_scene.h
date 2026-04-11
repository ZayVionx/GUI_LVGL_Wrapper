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
 * Title:        gui_scene.h
 * Description:  GUI scene template (CMSIS-Pack/RTE instance-based)
 *
 * NOTE:
 * - `GUI_SCENE_<NAME>` need to be defined in the "gui_scene_id.h" file,
 * 
 * - Do NOT compile this template file directly. For non-RTE usage, replace
 *   `%Instance%` manually (and rename files/symbols as needed).
 * --------------------------------------------------------------------
 */

#ifndef __GUI_LV_SCENE_%Instance%_H__
#define __GUI_LV_SCENE_%Instance%_H__

/*================================= INCLUDES =================================*/

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
 * \brief Register/init GUI scene <name>.
 *
 * \note The scene ID should be defined in your enum as GUI_SCENE_<NAME>.
 */
extern
void gui_lv_scene_%Instance%_init(void);

/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_SCENE_%Instance%_H__ */
