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

#ifndef __GUI_LV_HELPER_STYLE_H__
#define __GUI_LV_HELPER_STYLE_H__

/*============================ INCLUDES ======================================*/
#include "gui_lv_utils.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
// 简化的设置宏
#define GUI_LV_STYLE_WIDTH(v)                   .width                   = (v), ._mark_width                   = 1
#define GUI_LV_STYLE_MIN_WIDTH(v)               .min_width               = (v), ._mark_min_width               = 1
#define GUI_LV_STYLE_MAX_WIDTH(v)               .max_width               = (v), ._mark_max_width               = 1
#define GUI_LV_STYLE_HEIGHT(v)                  .height                  = (v), ._mark_height                  = 1
#define GUI_LV_STYLE_MIN_HEIGHT(v)              .min_height              = (v), ._mark_min_height              = 1
#define GUI_LV_STYLE_MAX_HEIGHT(v)              .max_height              = (v), ._mark_max_height              = 1
#define GUI_LV_STYLE_X(v)                       .x                       = (v), ._mark_x                       = 1
#define GUI_LV_STYLE_Y(v)                       .y                       = (v), ._mark_y                       = 1
#define GUI_LV_STYLE_ALIGN(v)                   .align                   = (v), ._mark_align                   = 1
#define GUI_LV_STYLE_LAYOUT(v)                  .layout                  = (v), ._mark_layout                  = 1
#define GUI_LV_STYLE_RADIUS(v)                  .radius                  = (v), ._mark_radius                  = 1

#define GUI_LV_STYLE_TRANSFORM_WIDTH(v)         .transform_width         = (v), ._mark_transform_width         = 1
#define GUI_LV_STYLE_TRANSFORM_HEIGHT(v)        .transform_height        = (v), ._mark_transform_height        = 1
#define GUI_LV_STYLE_TRANSLATE_X(v)             .translate_x             = (v), ._mark_translate_x             = 1
#define GUI_LV_STYLE_TRANSLATE_Y(v)             .translate_y             = (v), ._mark_translate_y             = 1
#define GUI_LV_STYLE_TRANSFORM_ZOOM(v)          .transform_zoom          = (v), ._mark_transform_zoom          = 1
#define GUI_LV_STYLE_TRANSFORM_ANGLE(v)         .transform_angle         = (v), ._mark_transform_angle         = 1
#define GUI_LV_STYLE_TRANSFORM_PIVOT_X(v)       .transform_pivot_x       = (v), ._mark_transform_pivot_x       = 1
#define GUI_LV_STYLE_TRANSFORM_PIVOT_Y(v)       .transform_pivot_y       = (v), ._mark_transform_pivot_y       = 1

#define GUI_LV_STYLE_PAD_TOP(v)                 .pad_top                 = (v), ._mark_pad_top                 = 1
#define GUI_LV_STYLE_PAD_BOTTOM(v)              .pad_bottom              = (v), ._mark_pad_bottom              = 1
#define GUI_LV_STYLE_PAD_LEFT(v)                .pad_left                = (v), ._mark_pad_left                = 1
#define GUI_LV_STYLE_PAD_RIGHT(v)               .pad_right               = (v), ._mark_pad_right               = 1
#define GUI_LV_STYLE_PAD_ALL(v)                 .pad_all                 = (v), ._mark_pad_all                 = 1
#define GUI_LV_STYLE_PAD_ROW(v)                 .pad_row                 = (v), ._mark_pad_row                 = 1
#define GUI_LV_STYLE_PAD_COLUMN(v)              .pad_column              = (v), ._mark_pad_column              = 1
#define GUI_LV_STYLE_PAD_GAP(v)                 .pad_gap                 = (v), ._mark_pad_gap                 = 1
#define GUI_LV_STYLE_BASE_DIR(v)                .base_dir                = (v), ._mark_base_dir                = 1

#define GUI_LV_STYLE_BG_COLOR(v)                .bg_color                = {v}, ._mark_bg_color                = 1
#define GUI_LV_STYLE_BG_OPA(v)                  .bg_opa                  = (v), ._mark_bg_opa                  = 1
#define GUI_LV_STYLE_BG_GRAD_COLOR(v)           .bg_grad_color           = {v}, ._mark_bg_grad_color           = 1
#define GUI_LV_STYLE_BG_GRAD_DIR(v)             .bg_grad_dir             = (v), ._mark_bg_grad_dir             = 1
#define GUI_LV_STYLE_BG_MAIN_STOP(v)            .bg_main_stop            = (v), ._mark_bg_main_stop            = 1
#define GUI_LV_STYLE_BG_GRAD_STOP(v)            .bg_grad_stop            = (v), ._mark_bg_grad_stop            = 1
#define GUI_LV_STYLE_BG_GRAD(v)                 .bg_grad                 = (v), ._mark_bg_grad                 = 1
#define GUI_LV_STYLE_BG_DITHER_MODE(v)          .bg_dither_mode          = (v), ._mark_bg_dither_mode          = 1
#define GUI_LV_STYLE_BG_IMG_SRC(v)              .bg_img_src              = (v), ._mark_bg_img_src              = 1
#define GUI_LV_STYLE_BG_IMG_OPA(v)              .bg_img_opa              = (v), ._mark_bg_img_opa              = 1
#define GUI_LV_STYLE_BG_IMG_RECOLOR(v)          .bg_img_recolor          = {v}, ._mark_bg_img_recolor          = 1
#define GUI_LV_STYLE_BG_IMG_RECOLOR_OPA(v)      .bg_img_recolor_opa      = (v), ._mark_bg_img_recolor_opa      = 1
#define GUI_LV_STYLE_BG_IMG_TILED(v)            .bg_img_tiled            = (v), ._mark_bg_img_tiled            = 1

#define GUI_LV_STYLE_BORDER_COLOR(v)            .border_color            = {v}, ._mark_border_color            = 1
#define GUI_LV_STYLE_BORDER_OPA(v)              .border_opa              = (v), ._mark_border_opa              = 1
#define GUI_LV_STYLE_BORDER_WIDTH(v)            .border_width            = (v), ._mark_border_width            = 1
#define GUI_LV_STYLE_BORDER_SIDE(v)             .border_side             = (v), ._mark_border_side             = 1
#define GUI_LV_STYLE_BORDER_POST(v)             .border_post             = (v), ._mark_border_post             = 1

#define GUI_LV_STYLE_OUTLINE_WIDTH(v)           .outline_width           = (v), ._mark_outline_width           = 1
#define GUI_LV_STYLE_OUTLINE_COLOR(v)           .outline_color           = {v}, ._mark_outline_color           = 1
#define GUI_LV_STYLE_OUTLINE_OPA(v)             .outline_opa             = (v), ._mark_outline_opa             = 1
#define GUI_LV_STYLE_OUTLINE_PAD(v)             .outline_pad             = (v), ._mark_outline_pad             = 1

#define GUI_LV_STYLE_SHADOW_WIDTH(v)            .shadow_width            = (v), ._mark_shadow_width            = 1
#define GUI_LV_STYLE_SHADOW_OFS_X(v)            .shadow_ofs_x            = (v), ._mark_shadow_ofs_x            = 1
#define GUI_LV_STYLE_SHADOW_OFS_Y(v)            .shadow_ofs_y            = (v), ._mark_shadow_ofs_y            = 1
#define GUI_LV_STYLE_SHADOW_SPREAD(v)           .shadow_spread           = (v), ._mark_shadow_spread           = 1
#define GUI_LV_STYLE_SHADOW_COLOR(v)            .shadow_color            = {v}, ._mark_shadow_color            = 1
#define GUI_LV_STYLE_SHADOW_OPA(v)              .shadow_opa              = (v), ._mark_shadow_opa              = 1

#define GUI_LV_STYLE_IMG_OPA(v)                 .img_opa                 = (v), ._mark_img_opa                 = 1
#define GUI_LV_STYLE_IMG_RECOLOR(v)             .img_recolor             = {v}, ._mark_img_recolor             = 1
#define GUI_LV_STYLE_IMG_RECOLOR_OPA(v)         .img_recolor_opa         = (v), ._mark_img_recolor_opa         = 1

#define GUI_LV_STYLE_LINE_WIDTH(v)              .line_width              = (v), ._mark_line_width              = 1
#define GUI_LV_STYLE_LINE_DASH_WIDTH(v)         .line_dash_width         = (v), ._mark_line_dash_width         = 1
#define GUI_LV_STYLE_LINE_DASH_GAP(v)           .line_dash_gap           = (v), ._mark_line_dash_gap           = 1
#define GUI_LV_STYLE_LINE_ROUNDED(v)            .line_rounded            = (v), ._mark_line_rounded            = 1
#define GUI_LV_STYLE_LINE_COLOR(v)              .line_color              = {v}, ._mark_line_color              = 1
#define GUI_LV_STYLE_LINE_OPA(v)                .line_opa                = (v), ._mark_line_opa                = 1

#define GUI_LV_STYLE_ARC_WIDTH(v)               .arc_width               = (v), ._mark_arc_width               = 1
#define GUI_LV_STYLE_ARC_ROUNDED(v)             .arc_rounded             = (v), ._mark_arc_rounded             = 1
#define GUI_LV_STYLE_ARC_COLOR(v)               .arc_color               = {v}, ._mark_arc_color               = 1
#define GUI_LV_STYLE_ARC_OPA(v)                 .arc_opa                 = (v), ._mark_arc_opa                 = 1
#define GUI_LV_STYLE_ARC_IMG_SRC(v)             .arc_img_src             = (v), ._mark_arc_img_src             = 1

#define GUI_LV_STYLE_TEXT_COLOR(v)              .text_color              = {v}, ._mark_text_color              = 1
#define GUI_LV_STYLE_TEXT_OPA(v)                .text_opa                = (v), ._mark_text_opa                = 1
#define GUI_LV_STYLE_TEXT_FONT(v)               .text_font               = (v), ._mark_text_font               = 1
#define GUI_LV_STYLE_TEXT_LETTER_SPACE(v)       .text_letter_space       = (v), ._mark_text_letter_space       = 1
#define GUI_LV_STYLE_TEXT_LINE_SPACE(v)         .text_line_space         = (v), ._mark_text_line_space         = 1
#define GUI_LV_STYLE_TEXT_DECOR(v)              .text_decor              = (v), ._mark_text_decor              = 1
#define GUI_LV_STYLE_TEXT_ALIGN(v)              .text_align              = (v), ._mark_text_align              = 1

#define GUI_LV_STYLE_CLIP_CORNER(v)             .clip_corner             = (v), ._mark_clip_corner             = 1
#define GUI_LV_STYLE_OPA(v)                     .opa                     = (v), ._mark_opa                     = 1
#define GUI_LV_STYLE_OPA_LAYERED(v)             .opa_layered             = (v), ._mark_opa_layered             = 1
#define GUI_LV_STYLE_COLOR_FILTER_DSC(v)        .color_filter_dsc        = (v), ._mark_color_filter_dsc        = 1
#define GUI_LV_STYLE_COLOR_FILTER_OPA(v)        .color_filter_opa        = (v), ._mark_color_filter_opa        = 1
#define GUI_LV_STYLE_ANIM(v)                    .anim                    = (v), ._mark_anim                    = 1
#define GUI_LV_STYLE_ANIM_TIME(v)               .anim_time               = (v), ._mark_anim_time               = 1
#define GUI_LV_STYLE_ANIM_SPEED(v)              .anim_speed              = (v), ._mark_anim_speed              = 1
#define GUI_LV_STYLE_TRANSITION(v)              .transition              = (v), ._mark_transition              = 1
#define GUI_LV_STYLE_BLEND_MODE(v)              .blend_mode              = (v), ._mark_blend_mode              = 1


/*============================ MACROFIED FUNCTIONS ===========================*/
// 简化应用样式到主要部分
#define GUI_LV_STYLE_APPLY_MAIN(obj, style)                                    \
    gui_lv_style_apply(obj, style, LV_PART_MAIN)

// 简化应用样式到聚焦状态
#define GUI_LV_STYLE_APPLY_FOCUSED(obj, style)                                 \
    gui_lv_style_apply(obj, style, LV_PART_MAIN | LV_STATE_FOCUSED)

// 简化应用样式到按下状态
#define GUI_LV_STYLE_APPLY_PRESSED(obj, style)                                 \
    gui_lv_style_apply(obj, style, LV_PART_MAIN | LV_STATE_PRESSED)

// 简化应用样式到选中状态
#define GUI_LV_STYLE_APPLY_CHECKED(obj, style)                                 \
    gui_lv_style_apply(obj, style, LV_PART_INDICATOR | LV_STATE_CHECKED)

// 简化应用样式到指示器部分
#define GUI_LV_STYLE_APPLY_INDICATOR(obj, style)                               \
    gui_lv_style_apply(obj, style, LV_PART_INDICATOR)

/*============================ TYPES =========================================*/
/**
 * @brief 样式配置
 */
typedef struct {
    // 尺寸
    lv_coord_t min_width;
    lv_coord_t min_height;
    lv_coord_t width;
    lv_coord_t max_width;
    lv_coord_t height;
    lv_coord_t max_height;
    lv_coord_t x;
    lv_coord_t y;
    lv_align_t align;
    uint16_t layout;

    // 内边距
    lv_coord_t pad_top;
    lv_coord_t pad_bottom;
    lv_coord_t pad_left;
    lv_coord_t pad_right;
    lv_coord_t pad_all;
    lv_coord_t pad_row;
    lv_coord_t pad_column;
    lv_coord_t pad_gap;
    lv_base_dir_t base_dir;

    // 背景
    lv_color_t bg_color;
    lv_opa_t bg_opa;
    lv_coord_t radius;
    lv_color_t bg_grad_color;
    lv_grad_dir_t bg_grad_dir;
    lv_coord_t bg_main_stop;
    lv_coord_t bg_grad_stop;
    const lv_grad_dsc_t *bg_grad;
    lv_dither_mode_t bg_dither_mode;
    const void *bg_img_src;
    lv_opa_t bg_img_opa;
    lv_color_t bg_img_recolor;
    lv_opa_t bg_img_recolor_opa;
    bool bg_img_tiled;

    // 边框
    lv_color_t border_color;
    lv_coord_t border_width;
    lv_opa_t border_opa;
    lv_border_side_t border_side;
    bool border_post;

    // 文本
    const lv_font_t* text_font;
    lv_color_t text_color;
    lv_opa_t text_opa;
    lv_coord_t text_letter_space;
    lv_coord_t text_line_space;
    lv_text_decor_t text_decor;
    lv_text_align_t text_align;

    // 轮廓
    lv_color_t outline_color;
    lv_coord_t outline_width;
    lv_coord_t outline_pad;
    lv_opa_t outline_opa;

    // 阴影
    lv_coord_t shadow_width;
    lv_opa_t shadow_opa;
    lv_coord_t shadow_ofs_x;
    lv_coord_t shadow_ofs_y;
    lv_coord_t shadow_spread;
    lv_color_t shadow_color;

    // 背景图片
    lv_opa_t img_opa;
    lv_color_t img_recolor;
    lv_opa_t img_recolor_opa;

    // 线
    lv_color_t line_color;
    lv_coord_t line_width;
    lv_coord_t line_dash_width;
    lv_coord_t line_dash_gap;
    bool line_rounded;
    lv_opa_t line_opa;

    // Arc
    lv_coord_t arc_width;
    bool arc_rounded;
    lv_color_t arc_color;
    lv_opa_t arc_opa;
    const void *arc_img_src;

    // Transform
    lv_coord_t transform_width;
    lv_coord_t transform_height;
    lv_coord_t translate_x;
    lv_coord_t translate_y;
    lv_coord_t transform_zoom;
    lv_coord_t transform_angle;
    lv_coord_t transform_pivot_x;
    lv_coord_t transform_pivot_y;

    // Common effects
    bool clip_corner;
    lv_opa_t opa;
    lv_opa_t opa_layered;
    const lv_color_filter_dsc_t *color_filter_dsc;
    lv_opa_t color_filter_opa;
    const lv_anim_t *anim;
    uint32_t anim_time;
    uint32_t anim_speed;
    const lv_style_transition_dsc_t *transition;
    lv_blend_mode_t blend_mode;

    // 标记字节（每个字段1字节，0表示未设置，1表示已设置）
    uint8_t _mark_min_width;
    uint8_t _mark_min_height;
    uint8_t _mark_width;
    uint8_t _mark_max_width;
    uint8_t _mark_height;
    uint8_t _mark_max_height;
    uint8_t _mark_x;
    uint8_t _mark_y;
    uint8_t _mark_align;
    uint8_t _mark_layout;
    uint8_t _mark_transform_width;
    uint8_t _mark_transform_height;
    uint8_t _mark_translate_x;
    uint8_t _mark_translate_y;
    uint8_t _mark_transform_zoom;
    uint8_t _mark_transform_angle;
    uint8_t _mark_transform_pivot_x;
    uint8_t _mark_transform_pivot_y;
    uint8_t _mark_pad_top;
    uint8_t _mark_pad_bottom;
    uint8_t _mark_pad_left;
    uint8_t _mark_pad_right;
    uint8_t _mark_pad_all;
    uint8_t _mark_pad_row;
    uint8_t _mark_pad_column;
    uint8_t _mark_pad_gap;
    uint8_t _mark_base_dir;
    uint8_t _mark_bg_color;
    uint8_t _mark_bg_opa;
    uint8_t _mark_radius;
    uint8_t _mark_bg_grad_color;
    uint8_t _mark_bg_grad_dir;
    uint8_t _mark_bg_main_stop;
    uint8_t _mark_bg_grad_stop;
    uint8_t _mark_bg_grad;
    uint8_t _mark_bg_dither_mode;
    uint8_t _mark_bg_img_opa;
    uint8_t _mark_bg_img_recolor;
    uint8_t _mark_bg_img_recolor_opa;
    uint8_t _mark_bg_img_tiled;
    uint8_t _mark_border_color;
    uint8_t _mark_border_width;
    uint8_t _mark_border_opa;
    uint8_t _mark_border_side;
    uint8_t _mark_border_post;
    uint8_t _mark_text_font;
    uint8_t _mark_text_color;
    uint8_t _mark_text_opa;
    uint8_t _mark_text_letter_space;
    uint8_t _mark_text_line_space;
    uint8_t _mark_text_decor;
    uint8_t _mark_text_align;
    uint8_t _mark_outline_color;
    uint8_t _mark_outline_width;
    uint8_t _mark_outline_pad;
    uint8_t _mark_outline_opa;
    uint8_t _mark_shadow_width;
    uint8_t _mark_shadow_opa;
    uint8_t _mark_shadow_ofs_x;
    uint8_t _mark_shadow_ofs_y;
    uint8_t _mark_shadow_spread;
    uint8_t _mark_shadow_color;
    uint8_t _mark_bg_img_src;
    uint8_t _mark_img_opa;
    uint8_t _mark_img_recolor;
    uint8_t _mark_img_recolor_opa;
    uint8_t _mark_line_color;
    uint8_t _mark_line_width;
    uint8_t _mark_line_dash_width;
    uint8_t _mark_line_dash_gap;
    uint8_t _mark_line_rounded;
    uint8_t _mark_line_opa;
    uint8_t _mark_arc_width;
    uint8_t _mark_arc_rounded;
    uint8_t _mark_arc_color;
    uint8_t _mark_arc_opa;
    uint8_t _mark_arc_img_src;
    uint8_t _mark_clip_corner;
    uint8_t _mark_opa;
    uint8_t _mark_opa_layered;
    uint8_t _mark_color_filter_dsc;
    uint8_t _mark_color_filter_opa;
    uint8_t _mark_anim;
    uint8_t _mark_anim_time;
    uint8_t _mark_anim_speed;
    uint8_t _mark_transition;
    uint8_t _mark_blend_mode;

} gui_lv_style_config_t;

/**
 * @brief 样式对象（包含LVGL样式和配置）
 */
typedef struct {
    lv_style_t style;
    gui_lv_style_config_t config;
    // const char* name;  // 用于调试
} gui_lv_style_t;



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/**
 * @brief 初始化样式（从配置结构体创建样式）
 * @param my_style 样式对象指针
 * @return 无
 */
extern
void gui_lv_style_init(gui_lv_style_t *ptStyle);

/**
 * @brief 应用样式到对象
 * @param obj LVGL对象
 * @param my_style 样式对象指针
 * @param selector 选择器（如 LV_PART_MAIN, LV_PART_MAIN | LV_STATE_FOCUSED）
 */
extern
void gui_lv_style_apply(lv_obj_t *ptObj, gui_lv_style_t *ptStyle, lv_style_selector_t selector);

/**
 * @brief 移除样式
 * @param obj LVGL对象
 * @param my_style 样式对象指针
 * @param selector 选择器
 */
extern
void gui_lv_style_remove(lv_obj_t *ptObj, gui_lv_style_t *ptStyle, lv_style_selector_t selector);

/**
 * @brief 重置样式（释放资源）
 * @param my_style 样式对象指针
 */
extern
void gui_lv_style_reset(gui_lv_style_t *ptStyle);

/*============================ IMPLEMENTATION ================================*/
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_HELPER_STYLE_H__ */
