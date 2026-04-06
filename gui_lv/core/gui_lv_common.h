#ifndef __GUI_LV_COMMON_H__
#define __GUI_LV_COMMON_H__

/*=============================== UI LIB MACROS ==============================*/
/*================================= INCLUDES =================================*/
#include "gui_lv_utils.h"
#include "helper/include/gui_lv_helper_input.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ PROTOTYPES ====================================*/
extern
lv_obj_t *gui_lv_container_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                        lv_color_t bg_color, bool bIsClearStyle);

extern
lv_obj_t *gui_lv_img_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                                            const void *img_scr);                       

extern
lv_obj_t *gui_lv_label_init(lv_obj_t *parent, int16_t x, int16_t y, const char *text, 
                                        const lv_font_t *font, lv_color_t color);

extern
lv_obj_t *gui_lv_btn_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height,
                                        lv_color_t bg_color, bool bIsClearStyle);

extern
lv_obj_t *gui_lv_bar_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                        int32_t min, int32_t max, int32_t value);


#if LV_USE_SPINBOX

extern
lv_obj_t *gui_lv_spinbox_init(lv_obj_t *parent, int16_t x, int16_t y, uint16_t width, uint16_t height, 
                                      const lv_font_t *font, bool bIsClearStyle);

extern
void gui_lv_spinbox_param_init(lv_obj_t *spinbox, int32_t count, int32_t point, uint32_t step,
                               int32_t value, int32_t min  , int32_t max  , uint8_t pos);

#endif

extern
lv_timer_t *gui_lv_timer_init(lv_timer_cb_t timer_cb, uint32_t period, void *user_data,
                                                                    bool BIs_On);

/*================================== UNTILS ==================================*/
extern
int32_t gui_lv_group_focus_nav(uint8_t cols, gui_lv_navigation_t eNav, bool wrap);

extern
int16_t gui_lv_group_get_focus_index(lv_group_t *ptGroup);

/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_COMMON_H__ */
