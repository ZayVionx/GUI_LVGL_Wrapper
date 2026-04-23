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

/*================================= INCLUDES =================================*/
#ifdef __RTE_Acceleration_GUI_LVGL_Scene%Instance%__
#   include <gui_lv_scene_task_%Instance%.h>
#   include <gui_lv_scene_%Instance%.h>
#endif

/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
    
/*!
 * \brief GUI scene %Instance% task implementation.
 */
void gui_lv_scene_task_%Instance%(lv_event_t *e)
{
    if(lv_event_get_code(e) != LV_EVENT_KEY)    return;
    uint32_t u32Key = lv_indev_get_key(LV_INDEV_KEYPAD);

    switch(u32Key)
    {
        // case LV_KEY_UP:    break;
        // case LV_KEY_DOWN:  break;
        // case LV_KEY_LEFT:  break;
        // case LV_KEY_RIGHT: break;
        // case LV_KEY_ESC:   break;
        // case LV_KEY_ENTER: break;
    }

}

/*==================================== END ===================================*/
