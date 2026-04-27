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
    if (ptCfg->_mark_max_width) {
        lv_style_set_max_width(&ptStyle->style, ptCfg->max_width);
    }
    if (ptCfg->_mark_min_height) {
        lv_style_set_min_height(&ptStyle->style, ptCfg->min_height);
    }
    if (ptCfg->_mark_max_height) {
        lv_style_set_max_height(&ptStyle->style, ptCfg->max_height);
    }
    if (ptCfg->_mark_width) {
        lv_style_set_width(&ptStyle->style, ptCfg->width);
    }
    if (ptCfg->_mark_height) {
        lv_style_set_height(&ptStyle->style, ptCfg->height);
    }
    if (ptCfg->_mark_x) {
        lv_style_set_x(&ptStyle->style, ptCfg->x);
    }
    if (ptCfg->_mark_y) {
        lv_style_set_y(&ptStyle->style, ptCfg->y);
    }
    if (ptCfg->_mark_align) {
        lv_style_set_align(&ptStyle->style, ptCfg->align);
    }
    if (ptCfg->_mark_layout) {
        lv_style_set_layout(&ptStyle->style, ptCfg->layout);
    }

    // Transform
    if (ptCfg->_mark_transform_width) {
        lv_style_set_transform_width(&ptStyle->style, ptCfg->transform_width);
    }
    if (ptCfg->_mark_transform_height) {
        lv_style_set_transform_height(&ptStyle->style, ptCfg->transform_height);
    }
    if (ptCfg->_mark_translate_x) {
        lv_style_set_translate_x(&ptStyle->style, ptCfg->translate_x);
    }
    if (ptCfg->_mark_translate_y) {
        lv_style_set_translate_y(&ptStyle->style, ptCfg->translate_y);
    }
    if (ptCfg->_mark_transform_zoom) {
        lv_style_set_transform_zoom(&ptStyle->style, ptCfg->transform_zoom);
    }
    if (ptCfg->_mark_transform_angle) {
        lv_style_set_transform_angle(&ptStyle->style, ptCfg->transform_angle);
    }
    if (ptCfg->_mark_transform_pivot_x) {
        lv_style_set_transform_pivot_x(&ptStyle->style, ptCfg->transform_pivot_x);
    }
    if (ptCfg->_mark_transform_pivot_y) {
        lv_style_set_transform_pivot_y(&ptStyle->style, ptCfg->transform_pivot_y);
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
    if (ptCfg->_mark_pad_row) {
        lv_style_set_pad_row(&ptStyle->style, ptCfg->pad_row);
    }
    if (ptCfg->_mark_pad_column) {
        lv_style_set_pad_column(&ptStyle->style, ptCfg->pad_column);
    }
    if (ptCfg->_mark_pad_gap) {
        lv_style_set_pad_gap(&ptStyle->style, ptCfg->pad_gap);
    }
    if (ptCfg->_mark_base_dir) {
        lv_style_set_base_dir(&ptStyle->style, ptCfg->base_dir);
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
    if (ptCfg->_mark_bg_grad_color) {
        lv_style_set_bg_grad_color(&ptStyle->style, ptCfg->bg_grad_color);
    }
    if (ptCfg->_mark_bg_grad_dir) {
        lv_style_set_bg_grad_dir(&ptStyle->style, ptCfg->bg_grad_dir);
    }
    if (ptCfg->_mark_bg_main_stop) {
        lv_style_set_bg_main_stop(&ptStyle->style, ptCfg->bg_main_stop);
    }
    if (ptCfg->_mark_bg_grad_stop) {
        lv_style_set_bg_grad_stop(&ptStyle->style, ptCfg->bg_grad_stop);
    }
    if (ptCfg->_mark_bg_grad) {
        lv_style_set_bg_grad(&ptStyle->style, ptCfg->bg_grad);
    }
    if (ptCfg->_mark_bg_dither_mode) {
        lv_style_set_bg_dither_mode(&ptStyle->style, ptCfg->bg_dither_mode);
    }
    if (ptCfg->_mark_bg_img_src) {
        lv_style_set_bg_img_src(&ptStyle->style, ptCfg->bg_img_src);
    }
    if (ptCfg->_mark_bg_img_opa) {
        lv_style_set_bg_img_opa(&ptStyle->style, ptCfg->bg_img_opa);
    }
    if (ptCfg->_mark_bg_img_recolor) {
        lv_style_set_bg_img_recolor(&ptStyle->style, ptCfg->bg_img_recolor);
    }
    if (ptCfg->_mark_bg_img_recolor_opa) {
        lv_style_set_bg_img_recolor_opa(&ptStyle->style, ptCfg->bg_img_recolor_opa);
    }
    if (ptCfg->_mark_bg_img_tiled) {
        lv_style_set_bg_img_tiled(&ptStyle->style, ptCfg->bg_img_tiled);
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
    if (ptCfg->_mark_border_side) {
        lv_style_set_border_side(&ptStyle->style, ptCfg->border_side);
    }
    if (ptCfg->_mark_border_post) {
        lv_style_set_border_post(&ptStyle->style, ptCfg->border_post);
    }

    // 文本
    if (ptCfg->_mark_text_font) {
        lv_style_set_text_font(&ptStyle->style, ptCfg->text_font);
    }
    if (ptCfg->_mark_text_color) {
        lv_style_set_text_color(&ptStyle->style, ptCfg->text_color);
    }
    if (ptCfg->_mark_text_opa) {
        lv_style_set_text_opa(&ptStyle->style, ptCfg->text_opa);
    }
    if (ptCfg->_mark_text_letter_space) {
        lv_style_set_text_letter_space(&ptStyle->style, ptCfg->text_letter_space);
    }
    if (ptCfg->_mark_text_line_space) {
        lv_style_set_text_line_space(&ptStyle->style, ptCfg->text_line_space);
    }
    if (ptCfg->_mark_text_decor) {
        lv_style_set_text_decor(&ptStyle->style, ptCfg->text_decor);
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
    if (ptCfg->_mark_shadow_ofs_x) {
        lv_style_set_shadow_ofs_x(&ptStyle->style, ptCfg->shadow_ofs_x);
    }
    if (ptCfg->_mark_shadow_ofs_y) {
        lv_style_set_shadow_ofs_y(&ptStyle->style, ptCfg->shadow_ofs_y);
    }
    if (ptCfg->_mark_shadow_spread) {
        lv_style_set_shadow_spread(&ptStyle->style, ptCfg->shadow_spread);
    }
    if (ptCfg->_mark_shadow_color) {
        lv_style_set_shadow_color(&ptStyle->style, ptCfg->shadow_color);
    }
    if (ptCfg->_mark_shadow_opa) {
        lv_style_set_shadow_opa(&ptStyle->style, ptCfg->shadow_opa);
    }


    // Image
    if (ptCfg->_mark_img_opa) {
        lv_style_set_img_opa(&ptStyle->style, ptCfg->img_opa);
    }
    if (ptCfg->_mark_img_recolor) {
        lv_style_set_img_recolor(&ptStyle->style, ptCfg->img_recolor);
    }
    if (ptCfg->_mark_img_recolor_opa) {
        lv_style_set_img_recolor_opa(&ptStyle->style, ptCfg->img_recolor_opa);
    }

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
    if (ptCfg->_mark_line_rounded) {
        lv_style_set_line_rounded(&ptStyle->style, ptCfg->line_rounded);
    }
    if (ptCfg->_mark_line_opa) {
        lv_style_set_line_opa(&ptStyle->style, ptCfg->line_opa);
    }

    // Arc
    if (ptCfg->_mark_arc_width) {
        lv_style_set_arc_width(&ptStyle->style, ptCfg->arc_width);
    }
    if (ptCfg->_mark_arc_rounded) {
        lv_style_set_arc_rounded(&ptStyle->style, ptCfg->arc_rounded);
    }
    if (ptCfg->_mark_arc_color) {
        lv_style_set_arc_color(&ptStyle->style, ptCfg->arc_color);
    }
    if (ptCfg->_mark_arc_opa) {
        lv_style_set_arc_opa(&ptStyle->style, ptCfg->arc_opa);
    }
    if (ptCfg->_mark_arc_img_src) {
        lv_style_set_arc_img_src(&ptStyle->style, ptCfg->arc_img_src);
    }

    // Common effects
    if (ptCfg->_mark_clip_corner) {
        lv_style_set_clip_corner(&ptStyle->style, ptCfg->clip_corner);
    }
    if (ptCfg->_mark_opa) {
        lv_style_set_opa(&ptStyle->style, ptCfg->opa);
    }
    if (ptCfg->_mark_opa_layered) {
        lv_style_set_opa_layered(&ptStyle->style, ptCfg->opa_layered);
    }
    if (ptCfg->_mark_color_filter_dsc) {
        lv_style_set_color_filter_dsc(&ptStyle->style, ptCfg->color_filter_dsc);
    }
    if (ptCfg->_mark_color_filter_opa) {
        lv_style_set_color_filter_opa(&ptStyle->style, ptCfg->color_filter_opa);
    }
    if (ptCfg->_mark_anim) {
        lv_style_set_anim(&ptStyle->style, ptCfg->anim);
    }
    if (ptCfg->_mark_anim_time) {
        lv_style_set_anim_time(&ptStyle->style, ptCfg->anim_time);
    }
    if (ptCfg->_mark_anim_speed) {
        lv_style_set_anim_speed(&ptStyle->style, ptCfg->anim_speed);
    }
    if (ptCfg->_mark_transition) {
        lv_style_set_transition(&ptStyle->style, ptCfg->transition);
    }
    if (ptCfg->_mark_blend_mode) {
        lv_style_set_blend_mode(&ptStyle->style, ptCfg->blend_mode);
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
