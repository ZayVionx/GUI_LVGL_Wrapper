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

#ifndef __GUI_LV_CONF_H__
#define __GUI_LV_CONF_H__

/*================================= INCLUDES =================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
#define LV_DISP_ONE_BUFFER          0
#define LV_DISP_TWO_BUFFER          1
#define LV_DISP_DOUBLE_BUFFER       2

/* NOTE: The __LV_* macro names are retained as the stable configuration
 * contract for existing projects and Pack-generated RTE files.
 */
// <<< Use Configuration Wizard in Context Menu >>>

// <q> Enable file system support
#ifndef __LV_USE_FILE_SYSTEM__
#   define __LV_USE_FILE_SYSTEM__   0
#endif


// <h> Display configuration
    // <o> Horizontal resolution 
	// <1-4096>
    #ifndef MY_DISP_HOR_RES
    #   define MY_DISP_HOR_RES              320
    #endif
    // <o> Vertical resolution 
	// <1-4096>
    #ifndef MY_DISP_VER_RES
    #   define MY_DISP_VER_RES              240
    #endif
    // <o> Display buffer size in pixels 
	// <1-65536>
    #ifndef __BUF_PX_SIZE__
    #   define __BUF_PX_SIZE__              19200
    #endif
    // <o> Buffering mode
    //     <0=>One buffer
    //     <1=>Two buffer
    //     <2=>Double buffer
    #ifndef __LV_DISP_USE_BUFFER__
    #   define __LV_DISP_USE_BUFFER__       1
    #endif
// </h>


// <h> Input device selection
    // <q> Enable keypad input device
    #ifndef __LV_USE_KEYPAD_INDEV__
    #   define __LV_USE_KEYPAD_INDEV__      1
    #endif
    // <q> Enable touch input device
    #ifndef __LV_USE_TOUCHPAD_INDEV__
    #   define __LV_USE_TOUCHPAD_INDEV__    0
    #endif
    // <q> Enable encoder input device
    #ifndef __LV_USE_ENCODER_INDEV__
    #   define __LV_USE_ENCODER_INDEV__     0
    #endif
    // <q> Enable mouse input device
    #ifndef __LV_USE_MOUSE_INDEV__
    #   define __LV_USE_MOUSE_INDEV__       0
    #endif
    // <q> Enable button input device
    #ifndef __LV_USE_BUTTON_INDEV__
    #   define __LV_USE_BUTTON_INDEV__      0
    #endif
    // <o> Default scene input device
    //     <0=>LV_INDEV_TOUCHPAD
    //     <1=>LV_INDEV_KEYPAD
    //     <2=>LV_INDEV_ENCODER
    //     <3=>LV_INDEV_MOUSE
    //     <4=>LV_INDEV_BUTTON
    #ifndef __LV_SCENE_DEFAULT_INDEV__
    #   define __LV_SCENE_DEFAULT_INDEV__   1
    #endif
// </h>

// <<< end of configuration section >>>

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_CONF_H__ */
