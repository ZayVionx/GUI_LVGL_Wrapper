/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
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
#include "helper/include/gui_lv_helper.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
static struct {
    bool     bInitialized;
    uint32_t wMSUnit;
    int64_t  lTimestampInit;
} s_tHelper = {
    .wMSUnit = 1,
};

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/

#if defined(GUI_LV_GET_SYSTEM_TIMESTAMP)                                      \
    && !defined(GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY)
#   error "GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY must be defined together with GUI_LV_GET_SYSTEM_TIMESTAMP"
#elif !defined(GUI_LV_GET_SYSTEM_TIMESTAMP)                                   \
    && defined(GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY)
#   error "GUI_LV_GET_SYSTEM_TIMESTAMP must be defined together with GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY"
#endif

#if defined(GUI_LV_GET_SYSTEM_TIMESTAMP)                                      \
    && defined(GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY)
int64_t gui_lv_helper_get_system_timestamp(void)
{
    return (int64_t)(GUI_LV_GET_SYSTEM_TIMESTAMP());
}

uint32_t gui_lv_helper_get_reference_clock_frequency(void)
{
    return (uint32_t)(GUI_LV_GET_REFERENCE_CLOCK_FREQUENCY());
}

#elif !defined(_WIN64)

GUI_LV_WEAK
int64_t gui_lv_helper_get_system_timestamp(void)
{
    return (int64_t)lv_tick_get();
}

GUI_LV_WEAK
uint32_t gui_lv_helper_get_reference_clock_frequency(void)
{
    return 1000ul;
}

#elif defined(_POSIX_VERSION) || defined(CLOCK_REALTIME) || defined(__APPLE__)

GUI_LV_WEAK
int64_t gui_lv_helper_get_system_timestamp(void)
{
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);

    return (1000000ll * timestamp.tv_sec) + (timestamp.tv_nsec / 1000ll);
}

GUI_LV_WEAK
uint32_t gui_lv_helper_get_reference_clock_frequency(void)
{
    return 1000000ul;
}

#else

GUI_LV_WEAK
int64_t gui_lv_helper_get_system_timestamp(void)
{
    return (int64_t)clock();
}

GUI_LV_WEAK
uint32_t gui_lv_helper_get_reference_clock_frequency(void)
{
    return CLOCKS_PER_SEC;
}
#endif


int64_t gui_lv_helper_convert_ticks_to_ms(int64_t lTick)
{
    return lTick / (int64_t)s_tHelper.wMSUnit;
}

int64_t gui_lv_helper_convert_ms_to_ticks(uint32_t wMS)
{
    int64_t lResult = (int64_t)s_tHelper.wMSUnit * (int64_t)wMS;
    return lResult ? lResult : 1;
}

void __gui_lv_helper_init(void)
{
    bool bInitialized = true;
    
    bInitialized = s_tHelper.bInitialized;
    s_tHelper.bInitialized = true;
    
    if (bInitialized) {
        return ;
    }

    s_tHelper.wMSUnit = gui_lv_helper_get_reference_clock_frequency() / 1000ul;
    if (s_tHelper.wMSUnit == 0) {
        s_tHelper.wMSUnit = 1;
    }
}

/*=================================== END ====================================*/
