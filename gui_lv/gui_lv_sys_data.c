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
#include "gui_lv_sys_data.h"

/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================ LOCAL SYS VARIABLES ===========================*/
static gui_lv_sys_data_t s_tUiSysData = {
    .eLanguage    = GUI_LV_LANGUAGE_CN,
    .eBootSceneId = GUI_SCENE_OPTION,
    .eBeepLevel   = GUI_LV_BEEP_LEVEL_MAX,
};

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

/*=================================== END ====================================*/
