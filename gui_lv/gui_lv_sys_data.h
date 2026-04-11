/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __GUI_LV_SYS_DATA_H__
#define __GUI_LV_SYS_DATA_H__

/*================================= INCLUDES =================================*/
#include <stdbool.h>
#include <stdint.h>

#if __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_scene_id.h>
#else
#   include "gui_lv_scene_id.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*! \brief UI display language. */
typedef enum {
    GUI_LV_LANGUAGE_CN  = 0,    //!< Simplified Chinese
    GUI_LV_LANGUAGE_TC  = 1,    //!< Traditional Chinese
    GUI_LV_LANGUAGE_EN  = 2,    //!< English
    GUI_LV_LANGUAGE_JP  = 3,    //!< Japanese
    GUI_LV_LANGUAGE_RUS = 4,    //!< Russian

    GUI_LV_LANGUAGE_MAX
} gui_lv_language_t;

/*! \brief Buzzer volume level. */
typedef enum {
    GUI_LV_BEEP_LEVEL_1   = 0,  //!< Low
    GUI_LV_BEEP_LEVEL_2   = 1,  //!< Medium
    GUI_LV_BEEP_LEVEL_3   = 2,  //!< High
    GUI_LV_BEEP_LEVEL_OFF = 3,  //!< Off

    GUI_LV_BEEP_LEVEL_MAX
} gui_lv_beep_level_t;

/*! \brief Device information. */
typedef struct {
    const char *pchName;        //!< Device name
    uint32_t    u32SN;          //!< Device serial number
    uint8_t     chHardWare_Ver; //!< Device hardware version
    uint8_t     chSoftWare_Ver; //!< Device software version
} gui_lv_device_info_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================= SETTER FUNCTIONS =============================*/
/*!
 * \brief   Set UI language.
 * \param[in] eLang Language to set.
 * \retval  true  Success.
 * \retval  false Fail.
 */
extern
bool gui_lv_set_lang(gui_lv_language_t eLang);

/*!
 * \brief   Set boot scene ID.
 * \param[in] eSceneId Scene ID to set.
 * \retval  true  Success.
 * \retval  false Fail.
 */
extern
bool gui_lv_set_boot_scene_id(gui_lv_scene_id_t eSceneId);

/*!
 * \brief   Set the level of the buzzer.
 * \param[in] eLevel The desired beep level (0-3).
 * \retval  true  Success.
 * \retval  false Fail.
 */
extern
bool gui_lv_set_beep_level(gui_lv_beep_level_t eLevel);

/*!
 * \brief   Set device information.
 * \param[in] pchName Device name string (null-terminated).
 * \param[in] u32SN Device serial number.
 * \param[in] chHardWare_Ver Hardware version number.
 * \param[in] chSoftWare_Ver Software version number.
 * \retval  true  Success.
 * \retval  false Fail.
 */
extern
bool gui_lv_set_device_info( const char *pchName, 
                             uint32_t u32SN, 
                             uint8_t chHardWare_Ver, 
                             uint8_t chSoftWare_Ver );

/*============================= GETTER FUNCTIONS =============================*/
/*!
 * \brief   Get current UI language.
 * \return  Current UI language.
 */
extern
gui_lv_language_t gui_lv_get_current_lang(void);

/*!
 * \brief   Get current boot scene ID.
 * \return  Current boot scene ID.
 */
extern
gui_lv_scene_id_t gui_lv_get_boot_scene_id(void);

/*!
 * \brief   Get the level of the buzzer.
 * \return  Current beep level (0-3).
 */
extern
gui_lv_beep_level_t gui_lv_get_beep_level(void);

/*!
 * \brief   Get device information.
 * \retval  Device information.
 */
extern
gui_lv_device_info_t gui_lv_get_device_info(void);

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SYS_DATA_H__ */
