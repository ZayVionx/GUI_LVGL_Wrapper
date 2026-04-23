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

#ifndef __GUI_LV_HELPER_H__
#define __GUI_LV_HELPER_H__

/*================================= INCLUDES =================================*/
#include "helper/include/gui_lv_helper_data.h"
#include "helper/include/gui_lv_helper_input.h"
#include "helper/include/gui_lv_helper_style.h"
#include "helper/include/gui_lv_helper_bind.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*!
 * \note do NOT use this macro directly
 */
extern void __gui_lv_helper_init(void);


/*================================ PROTOTYPES ================================*/
extern uint32_t gui_lv_helper_get_reference_clock_frequency(void);
extern int64_t  gui_lv_helper_get_system_timestamp(void);
extern int64_t  gui_lv_helper_convert_ticks_to_ms(int64_t lTick);
extern int64_t  gui_lv_helper_convert_ms_to_ticks(uint32_t wMS);

/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_HELPER_H__ */
