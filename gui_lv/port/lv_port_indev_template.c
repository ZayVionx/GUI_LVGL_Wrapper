/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include "../../lvgl.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if __LV_USE_TOUCHPAD_INDEV__
    static void touchpad_init(void);
    static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
    static bool touchpad_is_pressed(void);
    static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);
#endif

#if __LV_USE_MOUSE_INDEV__
    static void mouse_init(void);
    static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
    static bool mouse_is_pressed(void);
    static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);
#endif

#if __LV_USE_KEYPAD_INDEV__
static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static uint32_t keypad_get_key(void);
#endif

#if __LV_USE_ENCODER_INDEV__
static void encoder_init(void);
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static void encoder_handler(void);
#endif

#if __LV_USE_BUTTON_INDEV__
static void button_init(void);
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
#if __LV_USE_TOUCHPAD_INDEV__
    lv_indev_t *indev_touchpad;
#endif
#if __LV_USE_MOUSE_INDEV__
    lv_indev_t *indev_mouse;
#endif
#if __LV_USE_KEYPAD_INDEV__
    lv_indev_t *indev_keypad;
#endif
#if __LV_USE_ENCODER_INDEV__
    lv_indev_t *indev_encoder;
	static int32_t encoder_diff;
	static lv_indev_state_t encoder_state;
#endif
#if __LV_USE_BUTTON_INDEV__
    lv_indev_t *indev_button;
#endif

//static int32_t encoder_diff;
//static lv_indev_state_t encoder_state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

#if __LV_USE_TOUCHPAD_INDEV__
    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
#endif

#if __LV_USE_MOUSE_INDEV__
    /*------------------
     * Mouse
     * -----------------*/

    /*Initialize your mouse if you have*/
    mouse_init();

    /*Register a mouse input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv);

    /*Set cursor. For simplicity set a HOME symbol now.*/
    lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);
#endif

#if __LV_USE_KEYPAD_INDEV__
    /*------------------
     * Keypad
     * -----------------*/

    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/
#endif

#if __LV_USE_ENCODER_INDEV__
    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/
#endif

#if __LV_USE_BUTTON_INDEV__
    /*------------------
     * Button
     * -----------------*/

    /*Initialize your button if you have*/
    button_init();

    /*Register a button input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = button_read;
    indev_button = lv_indev_drv_register(&indev_drv);
#endif

    /*Assign buttons to points on the screen*/
    // static const lv_point_t btn_points[2] = {
    //     {10, 10},   /*Button 0 -> x:10; y:10*/
    //     {40, 100},  /*Button 1 -> x:40; y:100*/
    // };
    // lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#if __LV_USE_TOUCHPAD_INDEV__
/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = 0;
    (*y) = 0;
}
#endif

#if __LV_USE_MOUSE_INDEV__
/*------------------
 * Mouse
 * -----------------*/

/*Initialize your mouse*/
static void mouse_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /*Get the current x and y coordinates*/
    mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the mouse button is pressed or released*/
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/*Return true is the mouse button is pressed*/
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the mouse is pressed*/
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = 0;
    (*y) = 0;
}
#endif

#if __LV_USE_KEYPAD_INDEV__
#include "basic_gpio.h"
#include "key.h"

/*------------------
 * Keypad
 * -----------------*/
static key_mode_e g_key_mode = KEY_MODE_NAV;
static const lv_key_t g_key_mode_map[KEY_MODE_MAX][KEY_NUM][2] = {
    // ===================== 模式0：导航模式 =====================
    {
        // {LV_KEY_NONE,    LV_KEY_NONE},     
        {LV_KEY_UP,     LV_KEY_LONG_UP},       // 上键：短按=上选，长按=无
        {LV_KEY_DOWN,   LV_KEY_LONG_DOWN},       // 下键：短按=下选，长按=无
        {LV_KEY_LEFT,   LV_KEY_LONG_LEFT},       // 左键：短按=前翻页，长按=无
        {LV_KEY_RIGHT,   LV_KEY_LONG_RIGHT},      // 右键：短按=后翻页，长按=无
        {LV_KEY_OK,     LV_KEY_LONG_OK},    // OK键：短按=确认，长按
        {LV_KEY_ESC,    LV_KEY_NONE},       // 返回键：短按=返回，长按=无
        {LV_KEY_HOME,   LV_KEY_LONG_HOME}   // HOME键：短按=主页，长按=开关机
    },
    // ===================== 模式1：编辑模式 =====================
    {
        // {LV_KEY_NONE,    LV_KEY_NONE}, 
        {LV_KEY_UP,     LV_KEY_LONG_UP},    // 上键：短按=值+，长按=值连+
        {LV_KEY_DOWN,   LV_KEY_LONG_DOWN},  // 下键：短按=值-，长按=值连-
        {LV_KEY_LEFT,   LV_KEY_LONG_LEFT},  // 左键：短按=移位+，长按=移位连+
        {LV_KEY_RIGHT,  LV_KEY_LONG_RIGHT}, // 右键：短按=移位-，长按=移位连-
        {LV_KEY_OK,     LV_KEY_LONG_OK},    // OK键：短按=确认，长按
        {LV_KEY_ESC,    LV_KEY_NONE},       // 返回键：短按=取消设置，长按=无
        {LV_KEY_HOME,   LV_KEY_LONG_HOME}   // HOME键：短按=主页，长按=开关机
    }
};

/*Initialize your keypad*/
static void keypad_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{    
    uint8_t u8KeyInfo  = key_get_value();
    uint8_t u8KeyId    = u8KeyInfo & 0x0F;
    uint8_t u8KeyState = (u8KeyInfo >> 4) & 0x0F; // 0: 短按，1: 长按

    data->state = LV_INDEV_STATE_REL;
    data->key   = 0;  /* 默认无按键，防止残留值被误读为 LV_KEY_ENTER */

	if(u8KeyId < KEY_NUM)
    {
        data->key = g_key_mode_map[g_key_mode][u8KeyId][u8KeyState];
        data->state = LV_INDEV_STATE_PR;
    }
}

/* 设置键盘工作模式 */
void ui_set_key_mode(key_mode_e mode) 
{
    if(mode < KEY_MODE_MAX) {
        g_key_mode = mode;
    } else {
        g_key_mode = KEY_MODE_NAV; // 越界默认切回导航模式
    }
}

/* 获取键盘工作模式 */
key_mode_e ui_get_key_mode(void)
{
    return g_key_mode;
}	

#if __LV_USE_ENCODER_INDEV__
/*------------------
 * Encoder
 * -----------------*/

/*Initialize your keypad*/
static void encoder_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    data->enc_diff = encoder_diff;
    data->state = encoder_state;
}

/*Call this function in an interrupt to process encoder events (turn, press)*/
static void encoder_handler(void)
{
    /*Your code comes here*/

    encoder_diff += 0;
    encoder_state = LV_INDEV_STATE_REL;
}
#endif

#if __LV_USE_BUTTON_INDEV__
/*------------------
 * Button
 * -----------------*/

/*Initialize your buttons*/
static void button_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the button*/
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 0; i < 2; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{

    /*Your code comes here*/

    return false;
}
#endif

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif

#endif
