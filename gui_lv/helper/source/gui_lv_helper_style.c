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
void style_init(ui_style_t* my_style)
{
    if (!my_style) return;
    
    lv_style_init(&my_style->style);
    
    style_config_t* cfg = &my_style->config;
    
    // 尺寸
    if (cfg->_mark_min_width) {
        lv_style_set_min_width(&my_style->style, cfg->min_width);
    }
    if (cfg->_mark_min_height) {
        lv_style_set_min_height(&my_style->style, cfg->min_height);
    }
    if (cfg->_mark_width) {
        lv_style_set_width(&my_style->style, cfg->width);
    }
    if (cfg->_mark_height) {
        lv_style_set_height(&my_style->style, cfg->height);
    }
    
    // 内边距
    if (cfg->_mark_pad_all) {
        lv_style_set_pad_all(&my_style->style, cfg->pad_all);
    } 
    if (cfg->_mark_pad_top) {
        lv_style_set_pad_top(&my_style->style, cfg->pad_top);
    }
    if (cfg->_mark_pad_bottom) {
        lv_style_set_pad_bottom(&my_style->style, cfg->pad_bottom);
    }
    if (cfg->_mark_pad_left) {
        lv_style_set_pad_left(&my_style->style, cfg->pad_left);
    }
    if (cfg->_mark_pad_right) {
        lv_style_set_pad_right(&my_style->style, cfg->pad_right);
    }
        
    // 背景
    if (cfg->_mark_bg_color) {
        lv_style_set_bg_color(&my_style->style, cfg->bg_color);
    }
    if (cfg->_mark_bg_opa) {
        lv_style_set_bg_opa(&my_style->style, cfg->bg_opa);
    }
    if (cfg->_mark_radius) {
        lv_style_set_radius(&my_style->style, cfg->radius);
    }
    
    // 边框
    if (cfg->_mark_border_color) {
        lv_style_set_border_color(&my_style->style, cfg->border_color);
    }
    if (cfg->_mark_border_width) {
        lv_style_set_border_width(&my_style->style, cfg->border_width);
    }
    if (cfg->_mark_border_opa) {
        lv_style_set_border_opa(&my_style->style, cfg->border_opa);
    }
    
    // 文本
    if (cfg->_mark_text_font) {
        lv_style_set_text_font(&my_style->style, cfg->text_font);
    }
    if (cfg->_mark_text_color) {
        lv_style_set_text_color(&my_style->style, cfg->text_color);
    }
    if (cfg->_mark_text_align) {
        lv_style_set_text_align(&my_style->style, cfg->text_align);
    }
    
    // 轮廓
    if (cfg->_mark_outline_color) {
        lv_style_set_outline_color(&my_style->style, cfg->outline_color);
    }
    if (cfg->_mark_outline_width) {
        lv_style_set_outline_width(&my_style->style, cfg->outline_width);
    }
    if (cfg->_mark_outline_pad) {
        lv_style_set_outline_pad(&my_style->style, cfg->outline_pad);
    }
    if (cfg->_mark_outline_opa) {
        lv_style_set_outline_opa(&my_style->style, cfg->outline_opa);
    }
    
    // 阴影
    if (cfg->_mark_shadow_width) {
        lv_style_set_shadow_width(&my_style->style, cfg->shadow_width);
    }
    if (cfg->_mark_shadow_opa) {
        lv_style_set_shadow_opa(&my_style->style, cfg->shadow_opa);
    }
    
    // 背景图片
    if (cfg->_mark_bg_img_src) {
        lv_style_set_bg_img_src(&my_style->style, cfg->bg_img_src);
    }

    // 线
    if (cfg->_mark_line_color) {
        lv_style_set_line_color(&my_style->style, cfg->line_color);
    }   
    if (cfg->_mark_line_width) {
        lv_style_set_line_width(&my_style->style, cfg->line_width);
    }
    if (cfg->_mark_line_dash_width) {
        lv_style_set_line_dash_width(&my_style->style, cfg->line_dash_width);
    }
    if (cfg->_mark_line_dash_gap) {
        lv_style_set_line_dash_gap(&my_style->style, cfg->line_dash_gap);
    }
    
}

/**
 * @brief 应用样式到对象
 */
void style_apply(lv_obj_t* obj, ui_style_t* my_style, lv_style_selector_t selector) 
{
    if (obj && my_style) {
        lv_obj_add_style(obj, &my_style->style, selector);
    }
}

/**
 * @brief 移除样式
 */
void style_remove(lv_obj_t* obj, ui_style_t* my_style, lv_style_selector_t selector) 
{
    if (obj && my_style) {
        lv_obj_remove_style(obj, &my_style->style, selector);
    }
}

/**
 * @brief 重置样式
 */
void style_reset(ui_style_t* my_style) 
{
    if (my_style) {
        lv_style_reset(&my_style->style);
    }
}

/*=========================== LOCAL FUNCTION HELPER ==========================*/

/*============================ END ===========================================*/
