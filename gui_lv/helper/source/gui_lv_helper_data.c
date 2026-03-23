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
#include "helper/include/gui_lv_helper_data.h"

/*================================== MACROS ==================================*/
/* 校验值 */
#define UI_DATA_CHECK_U16_VALUE(_data)                                         \
    do {                                                                       \
        if(_data->u16Data_t.u16Value < _data->u16Data_t.u16MinValue)           \
            _data->u16Data_t.u16Value = _data->u16Data_t.u16MinValue;          \
        else if(_data->u16Data_t.u16Value > _data->u16Data_t.u16MaxValue)      \
            _data->u16Data_t.u16Value = _data->u16Data_t.u16MaxValue;          \
    } while(0)

#define UI_DATA_CHECK_F_VALUE(_data)                                           \
    do {                                                                       \
        if(_data->fData_t.fValue < _data->fData_t.fMinValue)                   \
            _data->fData_t.fValue = _data->fData_t.fMinValue;                  \
        else if(_data->fData_t.fValue > _data->fData_t.fMaxValue)              \
            _data->fData_t.fValue = _data->fData_t.fMaxValue;                  \
    } while(0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
static ui_data_t s_tUiDataOps = {NULL};

/*============================== LOCAL FUNCTION ==============================*/


/*============================== IMPLEMENTATION ==============================*/
/*******************************************************************************
 * @brief   Initialize UI data module.
 ******************************************************************************/
bool ui_data_init(ui_data_cfg_t *ptCfg)
{
   if(ptCfg == NULL) return false;

    s_tUiDataOps.ptCfg = ptCfg;
    
    /* Load system data */
    // s_tUiDataOps.ptCfg->pfDataLoad ();
    return true;
}

/*******************************************************************************
 * @brief   Set UI data value.
 * @param   ptRange: Pointer to range data.
 * @param   eType:   Data type (INT/FLOAT).
 * @param   eDit:    Edit command (add or reduce).
 * @param   bIsWrap: Whether to wrap around on overflow/underflow.
 ******************************************************************************/
void ui_data_set(ui_data_range_t *ptRange, ui_data_type_t eType, ui_edit_t eDit, bool bIsWrap)
{
    if(ptRange == NULL) return;
    UI_DATA_CHECK_U16_VALUE(ptRange);
    UI_DATA_CHECK_F_VALUE(ptRange);
    
    if (eType == UI_DATA_TYPE_INT)
    {
        if(eDit == UI_EDIT_ADD)
        {
            // value:19  min:0 max:20 step:1
            if(ptRange->u16Data_t.u16Value > (ptRange->u16Data_t.u16MaxValue - ptRange->u16Data_t.u16Step))
            {
                uint16_t u16Difference = ptRange->u16Data_t.u16Step - 1 -
                                        (ptRange->u16Data_t.u16MaxValue - ptRange->u16Data_t.u16Value);
                ptRange->u16Data_t.u16Value = bIsWrap ? (ptRange->u16Data_t.u16MinValue + u16Difference) 
                                            : ptRange->u16Data_t.u16MaxValue;
            }
            else ptRange->u16Data_t.u16Value += ptRange->u16Data_t.u16Step;
        }
        else if(eDit == UI_EDIT_REDUCE)
        {
            // value:5  min:0 max:20 step:10
            if(ptRange->u16Data_t.u16Value < (ptRange->u16Data_t.u16MinValue + ptRange->u16Data_t.u16Step))
            {
                uint16_t u16Difference = ptRange->u16Data_t.u16Step - 1 -
                                        (ptRange->u16Data_t.u16Value - ptRange->u16Data_t.u16MinValue);
                ptRange->u16Data_t.u16Value = bIsWrap ? (ptRange->u16Data_t.u16MaxValue - u16Difference)
                                            : ptRange->u16Data_t.u16MinValue;
            }
            else ptRange->u16Data_t.u16Value -= ptRange->u16Data_t.u16Step;
        }
    }
    else if (eType == UI_DATA_TYPE_FLOAT)
    {
        if(eDit == UI_EDIT_ADD)
        {
            if((ptRange->fData_t.fValue + ptRange->fData_t.fStep) > ptRange->fData_t.fMaxValue)
            {
               ptRange->fData_t.fValue = bIsWrap ? ptRange->fData_t.fMinValue : ptRange->fData_t.fMaxValue;
            }
            else ptRange->fData_t.fValue += ptRange->fData_t.fStep;
        }
        else if(eDit == UI_EDIT_REDUCE)
        {
            if((ptRange->fData_t.fValue - ptRange->fData_t.fStep) < ptRange->fData_t.fMinValue)
            {
               ptRange->fData_t.fValue = bIsWrap ? ptRange->fData_t.fMaxValue : ptRange->fData_t.fMinValue;
            }
            else ptRange->fData_t.fValue -= ptRange->fData_t.fStep;
        }
    }
}

/*******************************************************************************
 * @brief   Save parameters to flash.
 ******************************************************************************/
void ui_data_save_to_flash(void)
{
    if(    s_tUiDataOps.ptCfg == NULL 
        || s_tUiDataOps.ptCfg->pfDataSave == NULL)  return;

    s_tUiDataOps.ptCfg->pfDataSave();
}

/*******************************************************************************
 * @brief   Reset parameters to default values.
 ******************************************************************************/
void ui_data_reset_to_default(void)
{
    if(    s_tUiDataOps.ptCfg == NULL 
        || s_tUiDataOps.ptCfg->pfDataReset == NULL)  return;
        
    s_tUiDataOps.ptCfg->pfDataReset();
}



/*======================= LOCAL FUNCTION IMPLEMENTATION ======================*/


/*=================================== END ====================================*/
