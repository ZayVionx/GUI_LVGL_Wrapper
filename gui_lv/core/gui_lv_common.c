/****************************************************************************
*  Copyright 2025 ZJY (Email:zhujinyuan818@gmail.com)                       *
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
#include "core/gui_lv_common.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

lv_obj_t *gui_lv_container_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                        lv_color_t bg_color, bool bIsClearStyle)
{
    lv_obj_t *container = lv_obj_create(parent);
    if(bIsClearStyle) lv_obj_remove_style_all(container);

    lv_obj_set_size          (container, width, height);
    lv_obj_set_pos           (container, x, y);
    lv_obj_set_style_bg_color(container, bg_color, 0);
    return container;
}

lv_obj_t *gui_lv_img_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                                            const void *img_scr)
{
    lv_obj_t *img_obj = lv_img_create(parent);
    lv_obj_set_size(img_obj, width, height);
    lv_obj_set_pos (img_obj, x, y);
    lv_img_set_src (img_obj, (const lv_img_dsc_t *)img_scr);
    return img_obj;
}                            

lv_obj_t *gui_lv_label_init(lv_obj_t *parent, int16_t x, int16_t y, const char *text, 
                                        const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *label_obj = lv_label_create(parent);
    
    if (x != 0 && y != 0) {
        lv_obj_set_pos(label_obj, x, y);
    }

    lv_label_set_text          (label_obj, text);
    lv_obj_set_style_text_font (label_obj, font, 0);
    lv_obj_set_style_text_color(label_obj, color, 0);
    return label_obj;
}                              

lv_obj_t *gui_lv_btn_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height,
                                        lv_color_t bg_color, bool bIsClearStyle)
{
    lv_obj_t *btn_obj = lv_btn_create(parent);
    if(bIsClearStyle) lv_obj_remove_style_all(btn_obj);

    lv_obj_set_size          (btn_obj, width, height);
    lv_obj_set_pos           (btn_obj, x, y);
    lv_obj_set_style_bg_color(btn_obj, bg_color, 0);
    return btn_obj;
}

lv_obj_t *gui_lv_bar_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                        int32_t min, int32_t max, int32_t value)
{
    lv_obj_t *bar_obj = lv_bar_create(parent);
    lv_obj_set_size (bar_obj, width, height);
    lv_obj_set_pos  (bar_obj, x, y);
    lv_bar_set_range(bar_obj, min, max);
    lv_bar_set_value(bar_obj, value, LV_ANIM_OFF);
    return bar_obj;
}                                            

lv_obj_t *gui_lv_slider_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                        int32_t min, int32_t max, int32_t value)
{
    lv_obj_t *slider_obj = lv_slider_create(parent);
    lv_obj_set_size    (slider_obj, width, height);
    lv_obj_set_pos     (slider_obj, x, y);
    lv_slider_set_range(slider_obj, min, max);
    lv_slider_set_value(slider_obj, value, LV_ANIM_OFF);
    return slider_obj;
}

#if LV_USE_SPINBOX

lv_obj_t *gui_lv_spinbox_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                      const lv_font_t *font, bool bIsClearStyle)
{
    lv_obj_t *spinbox = lv_spinbox_create(parent);
    if(bIsClearStyle) lv_obj_remove_style_all(spinbox);
    lv_obj_set_size(spinbox, width, height);
    lv_obj_set_pos(spinbox, x, y);
    lv_obj_set_style_text_font(spinbox, font, 0);
    return spinbox;
}

void gui_lv_spinbox_param_init(lv_obj_t *spinbox, int32_t count, int32_t point, uint32_t step,
                               int32_t value, int32_t min  , int32_t max  , uint8_t pos)
{
    lv_spinbox_set_digit_format(spinbox, count, point);
    lv_spinbox_set_step(spinbox, step);
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_value(spinbox, value);  
    lv_spinbox_set_cursor_pos(spinbox, pos);
}

#endif

lv_timer_t *gui_lv_timer_init(lv_timer_cb_t timer_cb, uint32_t period, void *user_data,
                                                                    bool BIs_On)
{
    lv_timer_t *timer_obj = lv_timer_create(timer_cb, period, user_data);

    if(!BIs_On) timer_obj->paused = true;
    else {
        timer_obj->paused = false;
        lv_timer_ready(timer_obj);
    }

    return timer_obj;
}



/*================================== UTILS ===================================*/

/**
 * @brief  Grid navigation on LVGL group — one call handles everything.
 *
 *   Direction keys:  calculate target index → focus it → return new index.
 *   Other keys:      no movement → just return current focus index.
 *
 * @param  group   LVGL group (contains the focusable objects)
 * @param  cols    Number of columns in the grid (1 = vertical list)
 * @param  eNav    GUI_LV_NAV_UP / DOWN / LEFT / RIGHT (others = no move)
 * @param  wrap    true = wrap around edges, false = clamp
 * @return 0-based focus index after navigation, or -1 if group is empty
 *
 * Usage:
 *   Direction:  gui_lv_group_focus_nav(group, 2, GUI_LV_NAV_UP,      true);
 *   Confirm:    int32_t idx = gui_lv_group_focus_nav(group, 2, GUI_LV_NAV_MAX, false);
 *   1D list:    gui_lv_group_focus_nav(group, 1, GUI_LV_NAV_DOWN,    true);
 */
int32_t gui_lv_group_focus_nav(uint8_t cols, gui_lv_navigation_t eNav, bool wrap)
{
    lv_group_t *group = LV_INDEV_KEYPAD->group;
    if(group == NULL || group->obj_focus == NULL) return -1;

    /* --- 1. find current index --- */
    int32_t cur = 0;
    lv_obj_t **node;
    _LV_LL_READ(&group->obj_ll, node) {
        if(node == group->obj_focus) break;
        cur++;
    }

    int32_t total = (int32_t)lv_group_get_obj_count(group);
    if(total <= 0) return -1;

    int32_t target = cur;

    /* --- 2. calculate target --- */
    switch(eNav) 
    {
        case GUI_LV_NAV_LEFT:   target = cur - 1;     break;
        case GUI_LV_NAV_RIGHT:  target = cur + 1;     break;
        case GUI_LV_NAV_UP:     target = cur - cols;  break;
        case GUI_LV_NAV_DOWN:   target = cur + cols;  break;
        default:            return cur;  /* no move, just return index */
    }

    /* --- 3. boundary handling --- */
    if(wrap) {
        target = ((target % total) + total) % total;  /* universal modulo wrap */
    } else {
        if(target < 0)     target = cur;  /* clamp: stay */
        if(target >= total) target = cur;
    }

    /* --- 4. focus target --- */
    if(target != cur) {
        int32_t i = 0;
        _LV_LL_READ(&group->obj_ll, node) {
            if(i == target) { lv_group_focus_obj(*node); break; }
            i++;
        }
    }

    return target;
}


/**
 * @brief Get current focus index of a group.   
 * @param ptGroup: LVGL group to query.
 * 
 * @return 0-based focus index, or -1 if no focus / NULL group.
 */
int16_t gui_lv_group_get_focus_index(lv_group_t *ptGroup)
{
    if(ptGroup == NULL) return -1;

    lv_obj_t *ptFocusObj = lv_group_get_focused(ptGroup);
    if(ptFocusObj == NULL) return -1;

    int16_t i16Idx = 0;
    lv_obj_t **pp;
    _LV_LL_READ(&ptGroup->obj_ll, pp) {
        if(*pp == ptFocusObj)
        {
            return i16Idx;
        }
        i16Idx++;
    }

    return -1;
}

/*!
 * \brief Get the object at a specific index in a group.
 *
 * \param ptGroup LVGL group to query.
 * \param chIdx 0-based index of the object.
 * 
 * \return Pointer to the object, or NULL if not found.
 */
lv_obj_t *gui_lv_group_get_index_obj(lv_group_t *ptGroup, uint8_t chIdx)
{
    if(ptGroup == NULL) return NULL;

    uint8_t chCur = 0;
    lv_obj_t **pp;
    _LV_LL_READ(&ptGroup->obj_ll, pp) {
        if(chCur == chIdx)
        {
            /* Clear previous focus visual state */
            if(ptGroup->obj_focus != NULL && *(ptGroup->obj_focus) != NULL)
            {
                lv_obj_clear_state(*(ptGroup->obj_focus),
                                      LV_STATE_FOCUSED 
                                    | LV_STATE_EDITED 
                                    | LV_STATE_FOCUS_KEY);
            }
            ptGroup->obj_focus = pp;
            return *pp;
        }
        chCur++;
    }

    return NULL;
}

/*=================================== END ====================================*/
