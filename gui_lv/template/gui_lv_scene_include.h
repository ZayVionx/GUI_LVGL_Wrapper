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

#ifndef __GUI_LV_SCENE_INCLUDE_H__
#define __GUI_LV_SCENE_INCLUDE_H__

/*================================= INCLUDES =================================*/
#include "RTE_Components.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*
 * NOTE:
 * - This header is meant to be used in CMSIS-Pack / RTE projects.
 * - Do NOT rely on the generated scene header filenames here. RTE-managed
 *   config files are often renamed/copied by the IDE, and manual renaming is
 *   a common source of "file missing" issues.
 * - We only depend on the stable init symbol names:
 *     gui_lv_scene_%Instance%_init()
 * - If an instance macro is not enabled, we provide an empty init macro so
 *   __GUI_LV_ALL_SCENE_INIT() is always safe to call.
 */

/* Declare all possible instance init symbols (maxInstances = 20). */

/* Per-instance init macros (enabled by __RTE_Acceleration_GUI_LVGL_SceneN__). */
#ifdef __RTE_Acceleration_GUI_LVGL_Scene0__
#   include "gui_lv_scene_0.h"
#   define __GUI_LV_SCENE_0_INIT()     gui_lv_scene_0_init()
#else
#   define __GUI_LV_SCENE_0_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene1__
#   include "gui_lv_scene_1.h"
#   define __GUI_LV_SCENE_1_INIT()     gui_lv_scene_1_init()
#else
#   define __GUI_LV_SCENE_1_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene2__
#   include "gui_lv_scene_2.h"
#   define __GUI_LV_SCENE_2_INIT()     gui_lv_scene_2_init()
#else
#   define __GUI_LV_SCENE_2_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene3__
#   include "gui_lv_scene_3.h"
#   define __GUI_LV_SCENE_3_INIT()     gui_lv_scene_3_init()
#else
#   define __GUI_LV_SCENE_3_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene4__
#   include "gui_lv_scene_4.h"
#   define __GUI_LV_SCENE_4_INIT()     gui_lv_scene_4_init()
#else
#   define __GUI_LV_SCENE_4_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene5__
#   include "gui_lv_scene_5.h"
#   define __GUI_LV_SCENE_5_INIT()     gui_lv_scene_5_init()
#else
#   define __GUI_LV_SCENE_5_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene6__
#   include "gui_lv_scene_6.h"
#   define __GUI_LV_SCENE_6_INIT()     gui_lv_scene_6_init()
#else
#   define __GUI_LV_SCENE_6_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene7__
#   include "gui_lv_scene_7.h"
#   define __GUI_LV_SCENE_7_INIT()     gui_lv_scene_7_init()
#else
#   define __GUI_LV_SCENE_7_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene8__
#   include "gui_lv_scene_8.h"
#   define __GUI_LV_SCENE_8_INIT()     gui_lv_scene_8_init()
#else
#   define __GUI_LV_SCENE_8_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene9__
#   include "gui_lv_scene_9.h"
#   define __GUI_LV_SCENE_9_INIT()     gui_lv_scene_9_init()
#else
#   define __GUI_LV_SCENE_9_INIT()     ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene10__
#   include "gui_lv_scene_10.h"
#   define __GUI_LV_SCENE_10_INIT()    gui_lv_scene_10_init()
#else
#   define __GUI_LV_SCENE_10_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene11__
#   include "gui_lv_scene_11.h"
#   define __GUI_LV_SCENE_11_INIT()    gui_lv_scene_11_init()
#else
#   define __GUI_LV_SCENE_11_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene12__
#   include "gui_lv_scene_12.h"
#   define __GUI_LV_SCENE_12_INIT()    gui_lv_scene_12_init()
#else
#   define __GUI_LV_SCENE_12_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene13__
#   include "gui_lv_scene_13.h"
#   define __GUI_LV_SCENE_13_INIT()    gui_lv_scene_13_init()
#else
#   define __GUI_LV_SCENE_13_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene14__
#   include "gui_lv_scene_14.h"
#   define __GUI_LV_SCENE_14_INIT()    gui_lv_scene_14_init()
#else
#   define __GUI_LV_SCENE_14_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene15__
#   include "gui_lv_scene_15.h"
#   define __GUI_LV_SCENE_15_INIT()    gui_lv_scene_15_init()
#else
#   define __GUI_LV_SCENE_15_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene16__
#   include "gui_lv_scene_16.h"
#   define __GUI_LV_SCENE_16_INIT()    gui_lv_scene_16_init()
#else
#   define __GUI_LV_SCENE_16_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene17__
#   include "gui_lv_scene_17.h"
#   define __GUI_LV_SCENE_17_INIT()    gui_lv_scene_17_init()
#else
#   define __GUI_LV_SCENE_17_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene18__
#   include "gui_lv_scene_18.h"
#   define __GUI_LV_SCENE_18_INIT()    gui_lv_scene_18_init()
#else
#   define __GUI_LV_SCENE_18_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene19__
#   include "gui_lv_scene_19.h"
#   define __GUI_LV_SCENE_19_INIT()    gui_lv_scene_19_init()
#else
#   define __GUI_LV_SCENE_19_INIT()    ((void)0)
#endif
#ifdef __RTE_Acceleration_GUI_LVGL_Scene20__
#   include "gui_lv_scene_20.h"
#   define __GUI_LV_SCENE_20_INIT()    gui_lv_scene_20_init()
#else
#   define __GUI_LV_SCENE_20_INIT()    ((void)0)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GUI_LV_ALL_SCENE_INIT()                                              \
    do {                                                                       \
        __GUI_LV_SCENE_0_INIT();                                               \
        __GUI_LV_SCENE_1_INIT();                                               \
        __GUI_LV_SCENE_2_INIT();                                               \
        __GUI_LV_SCENE_3_INIT();                                               \
        __GUI_LV_SCENE_4_INIT();                                               \
        __GUI_LV_SCENE_5_INIT();                                               \
        __GUI_LV_SCENE_6_INIT();                                               \
        __GUI_LV_SCENE_7_INIT();                                               \
        __GUI_LV_SCENE_8_INIT();                                               \
        __GUI_LV_SCENE_9_INIT();                                               \
        __GUI_LV_SCENE_10_INIT();                                              \
        __GUI_LV_SCENE_11_INIT();                                              \
        __GUI_LV_SCENE_12_INIT();                                              \
        __GUI_LV_SCENE_13_INIT();                                              \
        __GUI_LV_SCENE_14_INIT();                                              \
        __GUI_LV_SCENE_15_INIT();                                              \
        __GUI_LV_SCENE_16_INIT();                                              \
        __GUI_LV_SCENE_17_INIT();                                              \
        __GUI_LV_SCENE_18_INIT();                                              \
        __GUI_LV_SCENE_19_INIT();                                              \
        __GUI_LV_SCENE_20_INIT();                                              \
    } while (0)
    
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_INCLUDE_H__ */
