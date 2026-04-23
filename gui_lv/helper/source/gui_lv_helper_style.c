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

/*============================ INCLUDES ======================================*/
#include "helper/include/gui_lv_helper_style.h"
#include <string.h>

/*============================ MACROS ========================================*/


/*============================ MACROFIED FUNCTIONS ===========================*/


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

/*================================ IMPLEMENTATION ============================*/
/**
 * @brief 初始化样式（从配置结构体创建样式）
 */
void gui_lv_style_init(gui_lv_style_t *ptStyle)
{
    if (ptStyle == NULL) return;
    
    lv_style_init(&ptStyle->style);
    
    gui_lv_style_config_t *ptCfg = &ptStyle->config;
    
    // 尺寸
    if (ptCfg->_mark_min_width) {
        lv_style_set_min_width(&ptStyle->style, ptCfg->min_width);
    }
    if (ptCfg->_mark_min_height) {
        lv_style_set_min_height(&ptStyle->style, ptCfg->min_height);
    }
    if (ptCfg->_mark_width) {
        lv_style_set_width(&ptStyle->style, ptCfg->width);
    }
    if (ptCfg->_mark_height) {
        lv_style_set_height(&ptStyle->style, ptCfg->height);
    }
    
    // 内边距
    if (ptCfg->_mark_pad_all) {
        lv_style_set_pad_all(&ptStyle->style, ptCfg->pad_all);
    } 
    if (ptCfg->_mark_pad_top) {
        lv_style_set_pad_top(&ptStyle->style, ptCfg->pad_top);
    }
    if (ptCfg->_mark_pad_bottom) {
        lv_style_set_pad_bottom(&ptStyle->style, ptCfg->pad_bottom);
    }
    if (ptCfg->_mark_pad_left) {
        lv_style_set_pad_left(&ptStyle->style, ptCfg->pad_left);
    }
    if (ptCfg->_mark_pad_right) {
        lv_style_set_pad_right(&ptStyle->style, ptCfg->pad_right);
    }
        
    // 背景
    if (ptCfg->_mark_bg_color) {
        lv_style_set_bg_color(&ptStyle->style, ptCfg->bg_color);
    }
    if (ptCfg->_mark_bg_opa) {
        lv_style_set_bg_opa(&ptStyle->style, ptCfg->bg_opa);
    }
    if (ptCfg->_mark_radius) {
        lv_style_set_radius(&ptStyle->style, ptCfg->radius);
    }
    
    // 边框
    if (ptCfg->_mark_border_color) {
        lv_style_set_border_color(&ptStyle->style, ptCfg->border_color);
    }
    if (ptCfg->_mark_border_width) {
        lv_style_set_border_width(&ptStyle->style, ptCfg->border_width);
    }
    if (ptCfg->_mark_border_opa) {
        lv_style_set_border_opa(&ptStyle->style, ptCfg->border_opa);
    }
    
    // 文本
    if (ptCfg->_mark_text_font) {
        lv_style_set_text_font(&ptStyle->style, ptCfg->text_font);
    }
    if (ptCfg->_mark_text_color) {
        lv_style_set_text_color(&ptStyle->style, ptCfg->text_color);
    }
    if (ptCfg->_mark_text_align) {
        lv_style_set_text_align(&ptStyle->style, ptCfg->text_align);
    }
    
    // 轮廓
    if (ptCfg->_mark_outline_color) {
        lv_style_set_outline_color(&ptStyle->style, ptCfg->outline_color);
    }
    if (ptCfg->_mark_outline_width) {
        lv_style_set_outline_width(&ptStyle->style, ptCfg->outline_width);
    }
    if (ptCfg->_mark_outline_pad) {
        lv_style_set_outline_pad(&ptStyle->style, ptCfg->outline_pad);
    }
    if (ptCfg->_mark_outline_opa) {
        lv_style_set_outline_opa(&ptStyle->style, ptCfg->outline_opa);
    }
    
    // 阴影
    if (ptCfg->_mark_shadow_width) {
        lv_style_set_shadow_width(&ptStyle->style, ptCfg->shadow_width);
    }
    if (ptCfg->_mark_shadow_opa) {
        lv_style_set_shadow_opa(&ptStyle->style, ptCfg->shadow_opa);
    }
    
    // 背景图片
    if (ptCfg->_mark_bg_img_src) {
        lv_style_set_bg_img_src(&ptStyle->style, ptCfg->bg_img_src);
    }

    // 线
    if (ptCfg->_mark_line_color) {
        lv_style_set_line_color(&ptStyle->style, ptCfg->line_color);
    }   
    if (ptCfg->_mark_line_width) {
        lv_style_set_line_width(&ptStyle->style, ptCfg->line_width);
    }
    if (ptCfg->_mark_line_dash_width) {
        lv_style_set_line_dash_width(&ptStyle->style, ptCfg->line_dash_width);
    }
    if (ptCfg->_mark_line_dash_gap) {
        lv_style_set_line_dash_gap(&ptStyle->style, ptCfg->line_dash_gap);
    }
    
}

/**
 * @brief 应用样式到对象
 */
void gui_lv_style_apply(lv_obj_t *ptObj, gui_lv_style_t *ptStyle, lv_style_selector_t selector)
{
    if (ptObj != NULL && ptStyle != NULL) {
        lv_obj_add_style(ptObj, &ptStyle->style, selector);
    }
}

/**
 * @brief 移除样式
 */
void gui_lv_style_remove(lv_obj_t *ptObj, gui_lv_style_t *ptStyle, lv_style_selector_t selector)
{
    if (ptObj != NULL && ptStyle != NULL) {
        lv_obj_remove_style(ptObj, &ptStyle->style, selector);
    }
}

/**
 * @brief 重置样式
 */
void gui_lv_style_reset(gui_lv_style_t *ptStyle)
{
    if (ptStyle != NULL) {
        lv_style_reset(&ptStyle->style);
    }
}

/*=========================== LOCAL FUNCTION HELPER ==========================*/

/*============================ END ===========================================*/
