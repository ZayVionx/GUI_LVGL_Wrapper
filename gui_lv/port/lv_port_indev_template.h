
/**
 * @file lv_port_indev_templ.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

#ifdef __GUI_LVGL_WRAPPER__
#   include <gui_lv_conf.h>
#else
#   include "gui_lv_conf.h"
#endif

/*********************
 *      DEFINES
 *********************/
// 自定义键值
#define LV_KEY_NONE      	  0 	/* 无操作 	 */
#define LV_KEY_OK        	131		/* OK		 */
#define LV_KEY_LONG_OK   	132		/* 长按OK	 */
#define LV_KEY_LONG_HOME 	133		/* 长按HOME	 */
#define LV_KEY_LONG_UP   	134		/* 长按UP	 */
#define LV_KEY_LONG_DOWN 	135		/* 长按DOWN  */
#define LV_KEY_LONG_LEFT 	136		/* 长按LEFT  */
#define LV_KEY_LONG_RIGHT	137		/* 长按RIGHT */
	
/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    KEY_MODE_NAV,      // 导航模式
    KEY_MODE_EDIT,     // 编辑模式
    KEY_MODE_MAX       
} key_mode_e;

/**********************
 *  GLOBAL VARIABLES
 **********************/
#if __LV_USE_TOUCHPAD_INDEV__
    extern lv_indev_t *indev_touchpad;
#endif
#if __LV_USE_MOUSE_INDEV__
    extern lv_indev_t *indev_mouse;
#endif
#if __LV_USE_KEYPAD_INDEV__
    extern lv_indev_t *indev_keypad;
#endif
#if __LV_USE_ENCODER_INDEV__
    extern lv_indev_t *indev_encoder;
#endif
#if __LV_USE_BUTTON_INDEV__
    extern lv_indev_t *indev_button;
#endif

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_port_indev_init(void);

/**
 * 以下是键盘输入设备的模式设置和获取函数声明。
 *  - ui_key_mode_set()：设置当前键盘工作模式，参数为key_mode_e枚举值。
 *  - ui_key_mode_get()：获取当前键盘工作模式，返回值为key_mode_e枚举值。
 *  - 键盘工作模式定义在key_mode_e枚举中，目前包含导航模式和编辑模式两种。
 *  -       这些函数的实现需要在lv_port_indev_template.c中完成，
 *    以便在键盘输入处理逻辑中根据当前模式应用不同的按键映射规则。
 */
extern void ui_set_key_mode(key_mode_e mode);
extern key_mode_e ui_get_key_mode(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
