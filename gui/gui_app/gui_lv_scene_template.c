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
#include <gui_lv_scene_<name>.h>
#include <gui_lv_scene_task_<name>.h>

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

/*--------------------------*
 * Don't Fix Include Order! *
 *--------------------------*/
#define __GUI_LV_IMPL__
#include "gui_lv_utils.h"

#if !defined(_WIN64)
/* Application layer header includes */

#endif

/*============================ MACROS ========================================*/
#define GUI_LV_SCENE_GROUP_NUM      1
#define GUI_LV_SCENE_TIMER_NUM      0
#define GUI_LV_SCENE_USED_DATA      0

/*============================ TYPES =========================================*/
typedef struct {
    lv_obj_t   *ptRoot;

    /* User-defined scene members begin --------------------------------------*/


    /*------------------------------------------------------------------------*/

    lv_group_t *ptGroup[GUI_LV_SCENE_GROUP_NUM];
    lv_timer_t *ptTimer[GUI_LV_SCENE_TIMER_NUM];

#if GUI_LV_SCENE_USED_DATA
    GUI_LV_PRIVATE(
        struct {    
            /* USER: cached app data mirrored for GUI refresh ----------------*/

        } Data;
    )
#endif
} gui_scene_t;

/*============================ LOCAL VARIABLES ===============================*/
static gui_scene_t s_tGUI;

/*============================ PROTOTYPES ====================================*/
static void __on_scene_<name>_draw  (lv_obj_t *ptRoot);
static void __on_scene_<name>_load  (lv_obj_t *ptRoot);
static void __on_scene_<name>_bind  (void);
static void __on_scene_<name>_depose(void);

#if GUI_LV_SCENE_TIMER_NUM
static void __on_scene_<name>_timer0_cb(lv_timer_t *ptTimer);
#endif

static gui_lv_fsm_rt_t __scene_sync_app_data(void);

/*============================ SCENE REGISTRATION ============================*/
/** 
 * @note The scene ID should be defined in your enum as GUI_LV_SCENE_<NAME>.
 *       Header include is in gui_lv_scene_id.h
 */
void gui_lv_scene_<name>_init(void)
{
    static gui_lv_extend_t s_tGUIEX; 
    s_tGUIEX.chGroupNum = GUI_LV_SCENE_GROUP_NUM;
    s_tGUIEX.chTimerNum = GUI_LV_SCENE_TIMER_NUM;
    s_tGUIEX.ptGroup    = GUI_LV_SCENE_GROUP_NUM ? s_tGUI.ptGroup 
                                                 : NULL;
    s_tGUIEX.ptTimer    = GUI_LV_SCENE_TIMER_NUM ? s_tGUI.ptTimer 
                                                 : NULL;

    /* Scene registration config begin ---------------------------------------*/
    static gui_lv_scene_cfg_t s_tCFG = {
        .eSceneId  = GUI_LV_SCENE_<NAME>,
        .ptExtend  = &s_tGUIEX,
        .pfnDraw   = __on_scene_<name>_draw,
        .pfnLoad   = __on_scene_<name>_load,
        .pfnBind   = __on_scene_<name>_bind, 
        .pfnDepose = __on_scene_<name>_depose,
    };
    /* Scene registration config end -----------------------------------------*/

    gui_lv_scene_register(&s_tCFG);
}


/*============================= SCENE CALLBACKS ==============================*/
GUI_LV_NONNULL(1)
static void __on_scene_<name>_draw(lv_obj_t *ptRoot)
{
    s_tGUI.ptRoot           = ptRoot;
    gui_lv_language_t eLang = gui_lv_get_current_lang();

    /* User draw code begin --------------------------------------------------*/


    /* User draw code end   --------------------------------------------------*/

#if GUI_LV_SCENE_TIMER_NUM
    GUI_LV_TIMER_SET(s_tGUI.ptTimer[0], __on_scene_<name>_timer0_cb, 10, NULL);
    GUI_LV_TIMER_ALL_START(s_tGUI.ptTimer, GUI_LV_SCENE_TIMER_NUM);
#endif
}

GUI_LV_NONNULL(1)
static void __on_scene_<name>_load(lv_obj_t *ptRoot)
{
    GUI_LV_UNUSED(ptRoot);

    /* User load code begin --------------------------------------------------*/


    /* User load code end ----------------------------------------------------*/
    
}

static void __on_scene_<name>_bind(void)
{
    GUI_LV_UNUSED(0);

    /* User bind code begin --------------------------------------------------*/


    /* User bind code end ----------------------------------------------------*/
    
}

static void __on_scene_<name>_depose(void)
{
    s_tGUI.ptRoot = NULL;

    /* User cleanup code begin -----------------------------------------------*/


    /* User cleanup code end -------------------------------------------------*/
}

/*============================ TIMER TASKS ===================================*/
#if GUI_LV_SCENE_TIMER_NUM
GUI_LV_NONNULL(1)
static void __on_scene_<name>_timer0_cb(lv_timer_t *ptTimer)
{
    GUI_LV_UNUSED(ptTimer);

    (void)__scene_sync_app_data();

    /* USER: periodic visual refresh -----------------------------------------*/

}
#endif

/*============================ DATA SYNC =====================================*/
#if GUI_LV_SCENE_USED_DATA

#   if (GUI_LV_SCENE_TIMER_NUM == 0)
#   error "__scene_sync_app_data() requires a timer to trigger periodic updates."
#   error "Please set GUI_LV_SCENE_TIMER_NUM > 0."
#   endif

IMPL_GUI_LV_PERIODIC_PT(__scene_sync_app_data, 100)
    PLATFORM_ON_WIN64(
        /* USER: simulator/mock/host-side data update ------------------------*/
    )

    PLATFORM_ON_MCU(
        /* USER: target AppData mapping and hardware-dependent refresh -------*/
    )
END_IMPL_GUI_LV_PERIODIC_PT(__scene_sync_app_data)

#else
static gui_lv_fsm_rt_t __scene_sync_app_data(void)
{
    return gui_lv_fsm_rt_cpl;
}
#endif

/*============================ END OF FILE ===================================*/
