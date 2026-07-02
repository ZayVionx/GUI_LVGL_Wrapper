
/**
 * @file lv_port_indev_templ.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 0

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "gui_lv_utils.h"

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#	include "lvgl.h"
#else
#	include "lvgl/lvgl.h"
#endif

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_conf.h>
#else
#   include "gui_lv_conf.h"
#endif

/*********************
 *      DEFINES
 *********************/
/* Number of Key */
#define KEY_NUM               2

/* Custom key-value pairs */
#define LV_KEY_NONE           0 	/* None Operation 	*/
#define LV_KEY_OK           131		/* OK		        */
#define LV_KEY_LONG_OK      132		/* Long Press OK	*/
#define LV_KEY_LONG_HOME    133		/* Long Press HOME	*/
#define LV_KEY_LONG_UP      134		/* Long Press UP	*/
#define LV_KEY_LONG_DOWN    135		/* Long Press DOWN  */
#define LV_KEY_LONG_LEFT    136		/* Long Press LEFT  */
#define LV_KEY_LONG_RIGH    137		/* Long Press RIGHT */
	
/**********************
 *      TYPEDEFS
 **********************/

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

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
