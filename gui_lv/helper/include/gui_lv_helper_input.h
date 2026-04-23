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

#ifndef __GUI_LV_HELPER_INPUT_H__
#define __GUI_LV_HELPER_INPUT_H__

/*================================= INCLUDES =================================*/
#include <stdint.h>
#include <stdbool.h>

#ifdef   __cplusplus
extern "C" {
#endif

/*================================== MACROS ==================================*/
#ifndef NULL
#   define NULL       ((void *)0)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/**
 * @brief   UI Event Types.
 */
typedef enum {
    GUI_LV_EVENT_TYPE_NAV  = 0,
    GUI_LV_EVENT_TYPE_EDIT = 1,
    GUI_LV_EVENT_TYPE_MAX
} gui_lv_event_type_t;

/**
 * @brief   UI Navigation Commands.
 */
typedef enum {
    GUI_LV_NAV_UP      = 0,
    GUI_LV_NAV_DOWN    = 1,
    GUI_LV_NAV_LEFT    = 2,
    GUI_LV_NAV_RIGHT   = 3,
    GUI_LV_NAV_CONFIRM = 4,
    GUI_LV_NAV_CANCEL  = 5,

	GUI_LV_NAV_MAX
} gui_lv_navigation_t;

/**
 * @brief   UI Edit Commands.
 */
typedef enum {
    GUI_LV_EDIT_ADD     = 0,
    GUI_LV_EDIT_REDUCE  = 1,
    GUI_LV_EDIT_CONFIRM = 2,
    GUI_LV_EDIT_CANCEL  = 3,

	GUI_LV_EDIT_MAX
} gui_lv_edit_t;

typedef struct {
    uint8_t u8FocusIndex;
    uint8_t u8MaxIndex;
    uint8_t u8MinIndex;
} gui_lv_index_t;

/**
 * @brief   UI Input Event Container.
 */
typedef struct {
    gui_lv_event_type_t  eType;  /* Type of the event */
    union {
        gui_lv_navigation_t eNav;   /* Payload for navigation events */
        gui_lv_edit_t       eEdit;  /* Payload for edit events */
    } uCmd;
} gui_lv_input_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/**
 * @brief   Handle UI navigation input.
 * @param   i8Index: Index of the data entry.
 * @param   eNav:    Navigation command.
 * @param   bIsWrap: Whether to wrap around on overflow/underflow.
 */
extern 
void gui_lv_input_nav(gui_lv_index_t *ptIndex, gui_lv_navigation_t eNav, uint8_t u8ColNum, bool bIsWrap);

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_HELPER_INPUT_H__ */
