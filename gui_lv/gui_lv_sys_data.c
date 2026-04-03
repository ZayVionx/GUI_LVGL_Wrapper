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

/*================================= INCLUDES =================================*/
#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_sys_data.h>
#else
#   include "gui_lv_sys_data.h"
#endif

/*================================== MACROS ==================================*/
#ifndef GUI_LV_NULL
#   define GUI_LV_NULL      ((void *)0)
#endif  

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/

/*! \brief System-level UI persistent data. */
typedef struct {
    gui_lv_language_t    eLanguage;     //!< Current display language.
    gui_scene_id_t       eBootSceneId;  //!< Scene shown at startup.
    gui_lv_beep_level_t  eBeepLevel;    //!< Buzzer volume level.
    gui_lv_device_info_t tDeviceInfo;   //!< Device information.
} gui_lv_sys_data_t;


/*============================= GLOBAL VARIABLES =============================*/
/*============================ LOCAL SYS VARIABLES ===========================*/
static gui_lv_sys_data_t s_tUiSysData;

/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
/*============================= SETTER FUNCTIONS =============================*/
/*******************************************************************************
 * @brief   Set UI language.
 * @param[in] eLang Language to set.
 * @return  true: Set language Success;
 *          false: Fail.
 ******************************************************************************/
bool gui_lv_set_lang(gui_lv_language_t eLang)
{
    if(eLang >= GUI_LV_LANGUAGE_MAX) return false;
    
    s_tUiSysData.eLanguage = eLang;
    return true;
}

/*******************************************************************************
 * @brief   Set boot scene ID.
 * @param[in] eSceneId Scene ID to set.
 * @return  true: Set scene ID Success;
 *          false: Fail.
 ******************************************************************************/
bool gui_lv_set_boot_scene_id(gui_scene_id_t eSceneId)
{
    if(eSceneId >= GUI_SCENE_MAX) return false;
    
    s_tUiSysData.eBootSceneId = eSceneId;
    return true;
}

/*******************************************************************************
 * @brief   Set the level of the buzzer.
 * @param[in] eLevel The desired beep level (0-3).
 * @return  true: Set beep level Success;
 *          false: Fail.
 ******************************************************************************/
bool gui_lv_set_beep_level(gui_lv_beep_level_t eLevel)
{
    if(eLevel >= GUI_LV_BEEP_LEVEL_MAX) return false;

    s_tUiSysData.eBeepLevel = eLevel;
    return true;
}

/*******************************************************************************
 * @brief   Set device information. 
 * @param[in] pchName Device name string (null-terminated). 
 * @param[in] u32SN Device serial number.   
 * @param[in] chHardWare_Ver Device hardware version.
 * @param[in] chSoftWare_Ver Device software version.
 * @return  true: Set device info Success;
 *          false: Fail.
 */
bool gui_lv_set_device_info( const char *pchName, 
                             uint32_t u32SN, 
                             uint8_t chHardWare_Ver, 
                             uint8_t chSoftWare_Ver )
{
    if(pchName == GUI_LV_NULL) return false;

    s_tUiSysData.tDeviceInfo.pchName        = pchName;
    s_tUiSysData.tDeviceInfo.u32SN          = u32SN;
    s_tUiSysData.tDeviceInfo.chHardWare_Ver = chHardWare_Ver;
    s_tUiSysData.tDeviceInfo.chSoftWare_Ver = chSoftWare_Ver;
    return true;
}   

/*============================= GETTER FUNCTIONS =============================*/
/*******************************************************************************
 * @brief   Get current UI language.
 * @return  Current UI language.
 ******************************************************************************/
gui_lv_language_t gui_lv_get_current_lang(void)
{
    return s_tUiSysData.eLanguage;
}

/*******************************************************************************
 * @brief   Get current boot scene ID.
 * @return  Current boot scene ID.
 ******************************************************************************/
gui_scene_id_t gui_lv_get_boot_scene_id(void)
{
    return s_tUiSysData.eBootSceneId;
}

/*******************************************************************************
 * @brief   Get the level of the buzzer.
 * @return  Current beep level (0-3).
 ******************************************************************************/
gui_lv_beep_level_t gui_lv_get_beep_level(void)
{
    return s_tUiSysData.eBeepLevel;
}

/*******************************************************************************
 * @brief   Get device information.
 * @return  Pointer to the current device information structure.
 ******************************************************************************/
gui_lv_device_info_t gui_lv_get_device_info(void)
{
    return s_tUiSysData.tDeviceInfo;
}

/*=================================== END ====================================*/
