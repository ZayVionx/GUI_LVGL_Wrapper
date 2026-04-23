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

#ifndef __GUI_LV_HELPER_STYLE_H__
#define __GUI_LV_HELPER_STYLE_H__

/*============================ INCLUDES ======================================*/
#include "gui_lv_utils.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
// 简化的设置宏
#define GUI_LV_STYLE_MIN_WIDTH(v)      .min_width      = (v), ._mark_min_width      = 1
#define GUI_LV_STYLE_MIN_HEIGHT(v)     .min_height     = (v), ._mark_min_height     = 1
#define GUI_LV_STYLE_WIDTH(v)          .width          = (v), ._mark_width          = 1
#define GUI_LV_STYLE_HEIGHT(v)         .height         = (v), ._mark_height         = 1
#define GUI_LV_STYLE_PAD_TOP(v)        .pad_top        = (v), ._mark_pad_top        = 1
#define GUI_LV_STYLE_PAD_BOTTOM(v)     .pad_bottom     = (v), ._mark_pad_bottom     = 1
#define GUI_LV_STYLE_PAD_LEFT(v)       .pad_left       = (v), ._mark_pad_left       = 1
#define GUI_LV_STYLE_PAD_RIGHT(v)      .pad_right      = (v), ._mark_pad_right      = 1
#define GUI_LV_STYLE_PAD_ALL(v)        .pad_all        = (v), ._mark_pad_all        = 1
#define GUI_LV_STYLE_BG_COLOR(v)       .bg_color       = {v}, ._mark_bg_color       = 1
#define GUI_LV_STYLE_BG_OPA(v)         .bg_opa         = (v), ._mark_bg_opa         = 1
#define GUI_LV_STYLE_RADIUS(v)         .radius         = (v), ._mark_radius         = 1
#define GUI_LV_STYLE_BORDER_COLOR(v)   .border_color   = {v}, ._mark_border_color   = 1
#define GUI_LV_STYLE_BORDER_WIDTH(v)   .border_width   = (v), ._mark_border_width   = 1
#define GUI_LV_STYLE_BORDER_OPA(v)     .border_opa     = (v), ._mark_border_opa     = 1
#define GUI_LV_STYLE_TEXT_FONT(v)      .text_font      = (v), ._mark_text_font      = 1
#define GUI_LV_STYLE_TEXT_COLOR(v)     .text_color     = {v}, ._mark_text_color     = 1
#define GUI_LV_STYLE_TEXT_ALIGN(v)     .text_align     = (v), ._mark_text_align     = 1
#define GUI_LV_STYLE_OUTLINE_COLOR(v)  .outline_color  = {v}, ._mark_outline_color  = 1
#define GUI_LV_STYLE_OUTLINE_WIDTH(v)  .outline_width  = (v), ._mark_outline_width  = 1
#define GUI_LV_STYLE_OUTLINE_PAD(v)    .outline_pad    = (v), ._mark_outline_pad    = 1
#define GUI_LV_STYLE_OUTLINE_OPA(v)    .outline_opa    = (v), ._mark_outline_opa    = 1
#define GUI_LV_STYLE_SHADOW_WIDTH(v)   .shadow_width   = (v), ._mark_shadow_width   = 1
#define GUI_LV_STYLE_SHADOW_OPA(v)     .shadow_opa     = (v), ._mark_shadow_opa     = 1
#define GUI_LV_STYLE_BG_IMG_SRC(v)     .bg_img_src     = (v), ._mark_bg_img_src     = 1
#define GUI_LV_STYLE_LINE_COLOR(v)     .line_color     = {v}, ._mark_line_color     = 1
#define GUI_LV_STYLE_LINE_WIDTH(v)     .line_width     = (v), ._mark_line_width     = 1
#define GUI_LV_STYLE_LINE_DASH_WIDTH(v) .line_dash_width = (v), ._mark_line_dash_width = 1
#define GUI_LV_STYLE_LINE_DASH_GAP(v)   .line_dash_gap   = (v), ._mark_line_dash_gap   = 1


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
    lv_coord_t height;
    
    // 内边距
    lv_coord_t pad_top;
    lv_coord_t pad_bottom;
    lv_coord_t pad_left;
    lv_coord_t pad_right;
    lv_coord_t pad_all;
    
    // 背景
    lv_color_t bg_color;
    lv_opa_t bg_opa;
    lv_coord_t radius;
    
    // 边框
    lv_color_t border_color;
    lv_coord_t border_width;
    lv_opa_t border_opa;
    
    // 文本
    const lv_font_t* text_font;
    lv_color_t text_color;
    lv_text_align_t text_align;
    
    // 轮廓
    lv_color_t outline_color;
    lv_coord_t outline_width;
    lv_coord_t outline_pad;
    lv_opa_t outline_opa;
    
    // 阴影
    lv_coord_t shadow_width;
    lv_opa_t shadow_opa;
    
    // 背景图片
    const void* bg_img_src;
    
    // 线
    lv_color_t line_color;
    lv_coord_t line_width;
    lv_coord_t line_dash_width;
    lv_coord_t line_dash_gap;

    // 标记字节（每个字段1字节，0表示未设置，1表示已设置）
    uint8_t _mark_min_width;
    uint8_t _mark_min_height;
    uint8_t _mark_width;
    uint8_t _mark_height;
    uint8_t _mark_pad_top;
    uint8_t _mark_pad_bottom;
    uint8_t _mark_pad_left;
    uint8_t _mark_pad_right;
    uint8_t _mark_pad_all;
    uint8_t _mark_bg_color;
    uint8_t _mark_bg_opa;
    uint8_t _mark_radius;
    uint8_t _mark_border_color;
    uint8_t _mark_border_width;
    uint8_t _mark_border_opa;
    uint8_t _mark_text_font;
    uint8_t _mark_text_color;
    uint8_t _mark_text_align;
    uint8_t _mark_outline_color;
    uint8_t _mark_outline_width;
    uint8_t _mark_outline_pad;
    uint8_t _mark_outline_opa;
    uint8_t _mark_shadow_width;
    uint8_t _mark_shadow_opa;
    uint8_t _mark_bg_img_src;
    uint8_t _mark_line_color;
    uint8_t _mark_line_width;
    uint8_t _mark_line_dash_width;
    uint8_t _mark_line_dash_gap;

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
