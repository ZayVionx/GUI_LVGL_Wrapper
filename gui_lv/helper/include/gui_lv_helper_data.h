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

#ifndef __GUI_LV_HELPER_DATA_H__
#define __GUI_LV_HELPER_DATA_H__

/*================================= INCLUDES =================================*/
#include "helper/include/gui_lv_helper_input.h"
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
/********************************** DATA TYPE *********************************/
/**
 * @brief   Supported Data Types.
 */
typedef enum {
    GUI_LV_DATA_TYPE_INT,
    GUI_LV_DATA_TYPE_FLOAT,
    GUI_LV_DATA_TYPE_BOOL,
    GUI_LV_DATA_TYPE_STRING,
} gui_lv_data_type_t;

/**
 * @brief   Data Range Constraint.
 * @note    Used for integer values that need min/max limits.
 *          UNIT: uint16_t
 */
typedef union {
    struct u16Data{
        uint16_t u16MinValue;
        uint16_t u16MaxValue;
        uint16_t u16Step;
        uint16_t u16Value;
    } u16Data_t;

    struct fData{
        float  fMinValue;
        float  fMaxValue;
        float  fStep;
        float  fValue;
    } fData_t;
} gui_lv_data_range_t;

/**
 * @brief   Generic Data Entry.
 * @note    Container for any type of data managed by the UI.
 */
typedef struct {
    void                *pValue;       /* Pointer to the actual data */
    void                *pRange;       /* Pointer to range constraint (optional) */
    gui_lv_data_type_t   eType;        /* Data type */
} gui_lv_data_entry_t;

 typedef struct {
    bool (*pfDataLoad )(void);
    bool (*pfDataSave )(void);
    bool (*pfDataReset)(void);
 } gui_lv_data_cfg_t;

typedef struct {
    gui_lv_data_cfg_t *ptCfg;
} gui_lv_data_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/**
 * @brief   Initialize UI data module.
 */
extern
bool gui_lv_data_init(gui_lv_data_cfg_t *ptCfg);

/**
 * @brief   Set UI data value.
 * @param   ptRange: Pointer to range data.
 * @param   eDit:    Edit command (add or reduce).
 * @param   bIsWrap: Whether to wrap around on overflow/underflow.
 */
extern 
void gui_lv_data_set(gui_lv_data_range_t *ptRange,
                     gui_lv_data_type_t eType,
                     gui_lv_edit_t eDit,
                     bool bIsWrap);

/**
 * @brief   Save parameters to flash.
 */
extern
void gui_lv_data_save_to_flash(void);

/**
 * @brief   Reset parameters to default values.
 */
extern 
void gui_lv_data_reset_to_default(void);

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_HELPER_DATA_H__ */
