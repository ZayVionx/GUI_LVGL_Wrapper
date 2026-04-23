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

/*================================= INCLUDES =================================*/
#include "helper/include/gui_lv_helper_input.h"

/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
/*******************************************************************************
 * @brief   Handle UI navigation input.
 * @param   i8Index: Index of the data entry.
 * @param   eNav:    Navigation command.
 * @param   bIsWrap: Whether to wrap around on overflow/underflow.
 ******************************************************************************/
void gui_lv_input_nav(gui_lv_index_t *ptIndex, gui_lv_navigation_t eNav, uint8_t u8ColNum, bool bIsWrap)
{
    if(ptIndex == NULL) return;
    if(u8ColNum == 0) u8ColNum = 1; // 防止除0错误，默认为1列

    uint8_t u8Cur = ptIndex->u8FocusIndex;
    uint8_t u8Min = ptIndex->u8MinIndex;
    uint8_t u8Max = ptIndex->u8MaxIndex;
    uint8_t u8Total = u8Max - u8Min + 1;
    
    // 转换为 0-based 索引
    uint8_t u8Norm = u8Cur - u8Min;

    switch(eNav)
    {
        case GUI_LV_NAV_LEFT:
            if(u8Norm > 0)
            {
                u8Norm--;
            }
            else if(bIsWrap)
            {
                u8Norm = u8Total - 1;
            }
            break;

        case GUI_LV_NAV_RIGHT:
            if(u8Norm < u8Total - 1)
            {
                u8Norm++;
            }
            else if(bIsWrap)
            {
                u8Norm = 0;
            }
            break;

        case GUI_LV_NAV_UP:
            if(u8Norm >= u8ColNum)
            {
                u8Norm -= u8ColNum;
            }
            else if(bIsWrap)
            {
                // 向上回环到当前列的底部
                uint8_t u8ColIdx = u8Norm % u8ColNum;
                uint8_t u8LastRowStart = (u8Total - 1) / u8ColNum * u8ColNum;
                uint8_t u8Target = u8LastRowStart + u8ColIdx;
                
                // 如果目标位置超出范围（不规则网格的最后一项），则上移一行
                if(u8Target >= u8Total)
                {
                    u8Target -= u8ColNum;
                }
                u8Norm = u8Target;
            }
            break;

        case GUI_LV_NAV_DOWN:
            if(u8Norm + u8ColNum < u8Total)
            {
                u8Norm += u8ColNum;
            }
            else if(bIsWrap)
            {
                // 向下回环到当前列的顶部
                u8Norm = u8Norm % u8ColNum;
            }
            break;
            
        default:
            return;
    }
    
    // 还原为原始索引
    ptIndex->u8FocusIndex = u8Norm + u8Min;
}


/*=================================== END ====================================*/
