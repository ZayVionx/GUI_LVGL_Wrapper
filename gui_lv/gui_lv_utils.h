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

#ifndef __GUI_LV_UTILS_H__
#define __GUI_LV_UTILS_H__

/*================================= INCLUDES =================================*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _RTE_
#   include "RTE_Components.h"
#endif

#if __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lv_conf.h>
#else  
#   include "gui_lv_conf.h"
#endif

#ifdef _WIN64
#   include "lvgl/lvgl.h"
#else
#   include "lvgl.h"
#   include "lv_port_disp_template.h"
#   include "lv_port_indev_template.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*----------------------------------------------------------------------------*
 * LVGL-INDEV                                                                 *
 *----------------------------------------------------------------------------*/
#ifdef _WIN64
#   define LV_INDEV_KEYPAD       kb_indev
#   define LV_INDEV_ENCODER      enc_indev
#   define LV_INDEV_TOUCHPAD     mouse_indev
    extern lv_indev_t           *kb_indev;
    extern lv_indev_t           *enc_indev;
    extern lv_indev_t           *mouse_indev;
#else
#   if __LV_USE_KEYPAD_INDEV__
        extern lv_indev_t       *indev_keypad;
#       define LV_INDEV_KEYPAD   indev_keypad
#   endif
#   if __LV_USE_TOUCHPAD_INDEV__
        extern lv_indev_t       *indev_touchpad;
#       define LV_INDEV_TOUCHPAD indev_touchpad
#   endif
#   if __LV_USE_ENCODER_INDEV__
        extern lv_indev_t       *indev_encoder;
#       define LV_INDEV_ENCODER  indev_encoder
#   endif
#endif

/*----------------------------------------------------------------------------*
 * Fall-through                                                               *
 *----------------------------------------------------------------------------*/
#if   defined(__cplusplus) && (__cplusplus >= 201703L)
#   define FALL_THROUGH        [[fallthrough]]
#elif defined(__GNUC__)    && (__GNUC__    >= 7)
#   define FALL_THROUGH        __attribute__((fallthrough))
#elif defined(__clang__)   && defined(__has_attribute)
#   if __has_attribute(fallthrough)
#       define FALL_THROUGH    __attribute__((fallthrough))
#   else
#       define FALL_THROUGH    ((void)0)
#   endif
#else
#   define FALL_THROUGH        ((void)0)
#endif

#if !defined(GUI_LV_WEAK)
#   if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)            \
        || defined(__ARMCC_VERSION)
#       define GUI_LV_WEAK __attribute__((weak))
#   else
#       define GUI_LV_WEAK
#   endif
#endif

/*----------------------------------------------------------------------------*
 * Misc                                                                       *
 *----------------------------------------------------------------------------*/
#define __VA_NUM_ARGS( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,              \
								_13,_14,_15,_16,__N,...)    __N
#define __EMB_VA_NUM_ARGS(...)                                                 \
			__VA_NUM_ARGS( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,             \
									  8,7,6,5,4,3,2,1,0)
#define __EMB_CONNECT2(__A, __B)                __A##__B
#define __EMB_CONNECT3(__A, __B, __C)           __A##__B##__C
#define __EMB_CONNECT4(__A, __B, __C, __D)      __A##__B##__C##__D
#define ALT_EMB_CONNECT2(__A, __B)              __EMB_CONNECT2(__A, __B)
#define EMB_CONNECT2(__A, __B)                  __EMB_CONNECT2(__A, __B)
#define EMB_CONNECT3(__A, __B, __C)             __EMB_CONNECT3(__A, __B, __C)
#define EMB_CONNECT4(__A, __B, __C, __D)        __EMB_CONNECT4(__A, __B, __C, __D)
#define EMB_CONNECT(...)                                                       \
			ALT_EMB_CONNECT2( EMB_CONNECT,                                     \
							  __EMB_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

/*!
 * \brief A macro to generate a safe name, usually used in macro template as the 
 *        name of local variables
 * 
 */
#define GUI_LV_SAFE_NAME(...)       EMB_CONNECT(__,__LINE__,##__VA_ARGS__)

/*----------------------------------------------------------------------------*
 * Misc                                                                       *
 *----------------------------------------------------------------------------*/
/*!
 * \brief A macro to safely invode a function pointer
 * 
 * \param[in] __FUNC_PTR the target function pointer
 * \param[in] ... an optional parameter list
 */
#define GUI_LV_INVOKE(__FUNC_PTR, ...)                                         \
    ((NULL == (__FUNC_PTR)) ? 0 : ((*(__FUNC_PTR))(__VA_ARGS__)))

/*!
 * \brief A macro to safely call a function pointer that has no return value
 * 
 * \param[in] __FUNC_PTR the target function pointer
 * \param[in] ... an optional parameter list
 */
#define GUI_LV_INVOKE_RT_VOID(__FUNC_PTR, ...)                                 \
    if (NULL != (__FUNC_PTR)) (*(__FUNC_PTR))(__VA_ARGS__)
    
/*----------------------------------------------------------------------------*
 * Misc                                                                       *
 *----------------------------------------------------------------------------*/

#define GUI_LV_NULL                 ((void *)0)

#define GUI_LV_UNUSED(__VAR)        (void)(__VAR)

#define GUI_LV_ASSERT(_expr)        do { if(!(_expr)){while(1);} } while(0)

/*!
 * \brief a wrapper for __attribute__((nonnull))
 */
#ifndef GUI_LV_NONNULL
#   if  defined(__IS_COMPILER_ARM_COMPILER_5__) ||\
        defined(__IS_COMPILER_ARM_COMPILER_6__) ||\
        defined(__IS_COMPILER_GCC__)            ||\
        defined(__IS_COMPILER_LLVM__)
#       define GUI_LV_NONNULL(...)     __attribute__((nonnull(__VA_ARGS__)))
#   else
#       define GUI_LV_NONNULL(...)
#   endif
#endif

/*----------------------------------------------------------------------------*
 * Array                                                                      *
 *----------------------------------------------------------------------------*/
/**
 * @brief Get the number of elements in a real array.
 * @note This macro only works for arrays, not pointers.
 */
#define GUI_LV_ARRAY_SIZE(a)        (sizeof(a) / sizeof((a)[0]))

/**
 * @brief Get the column count of a two-dimensional array.
 * @note This macro only works for two-dimensional arrays.
 */
#define GUI_LV_ARRAY_COLS(a)        (sizeof((a)[0]) / sizeof((a)[0][0]))

/*----------------------------------------------------------------------------*
 * Color                                                                      *
 *----------------------------------------------------------------------------*/ 
#define COLOR_HEX(_hex)             lv_color_hex(0x##_hex)
#define rgb(_r, _g, _b)             lv_color_make(_r, _g, _b)

#if LV_COLOR_DEPTH == 1
	#define RGB(r, g, b) \
		{.full = ((uint8_t)(((uint16_t)(r) + (uint16_t)(g) + (uint16_t)(b)) / 3 > 128 ? 1 : 0))}
#elif LV_COLOR_DEPTH == 8
	#define RGB(r, g, b) \
		{.full = ((uint8_t)(((uint8_t)((r) >> 5) << 5) | \
				   ((uint8_t)((g) >> 5) << 2) | \
				   ((uint8_t)((b) >> 6))))}
#elif LV_COLOR_DEPTH == 16
	#if LV_COLOR_16_SWAP == 0
		#define RGB(r, g, b) \
			.full = ((uint16_t)(((uint16_t)((r) >> 3) << 11) | \
						((uint16_t)((g) >> 2) << 5) | \
						((uint16_t)((b) >> 3))))
	#else
		#define RGB(r, g, b) \
			{.full = ((uint16_t)(((uint16_t)((g) >> 5) << 13) | \
						((uint16_t)((r) >> 3) << 8) | \
						((uint16_t)((b) >> 3) << 3) | \
						((uint16_t)((g) >> 2) & 0x07)))}
	#endif
#elif LV_COLOR_DEPTH == 32
	#define RGB(r, g, b) \
		{.full = ((uint32_t)((uint32_t)(b) | \
					((uint32_t)(g) << 8) | \
					((uint32_t)(r) << 16) | \
					(0xFFUL << 24)))}
#else
	#error "Unsupported LV_COLOR_DEPTH"
#endif

#define RGB_TO_COLOR(r, g, b)  ((lv_color_t){.full = RGB(r, g, b)})

/*----------------------------------------------------------------------------*
 * Font                                                                       *
 *----------------------------------------------------------------------------*/
#define FONT(_font, _size)          EMB_CONNECT(&lv_font_, _font, _, _size)

/*----------------------------------------------------------------------------*
 * Image                                                                      *
 *----------------------------------------------------------------------------*/
#define IMAGE(_img)                 EMB_CONNECT(&lv_img_, _img)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*----------------------------------------------------------------------------*
 * Container macro func                                                       *
 *----------------------------------------------------------------------------*/
#define GUI_LV_SCREEN_WIDTH         lv_disp_get_hor_res(lv_disp_get_default())
#define GUI_LV_SCREEN_HEIGHT        lv_disp_get_ver_res(lv_disp_get_default())

#define GUI_LV_CONTAINER_SET_BG_COLOR(_container, _color)                      \
	do {                                                                       \
		if ((_container) != NULL) {                                            \
			lv_obj_set_style_bg_color((_container), (_color), 0);              \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Label macro func                                                           *
 *----------------------------------------------------------------------------*/
#define GUI_LV_LABEL_SET_TEXT(_label, _text)                                   \
	do {                                                                       \
		if ((_label) != NULL) {                                                \
			lv_label_set_text((_label), (_text));                              \
		}                                                                      \
	} while (0)

#define GUI_LV_LABEL_SET_TEXT_FMT(_label, _fmt, ...)                           \
	do {                                                                       \
		if ((_label) != NULL) {                                                \
			lv_label_set_text_fmt((_label), (_fmt), __VA_ARGS__);              \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Image macro func                                                           *
 *----------------------------------------------------------------------------*/
#define GUI_LV_IMG_SET_SRC(_img, _width, _height, _src)                        \
	do {                                                                       \
		if ((_img) != NULL) {                                                  \
			lv_img_set_src((_img), (_src));                                    \
			lv_obj_set_size((_img), (_width), (_height));                      \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Bar macro func                                                             *
 *----------------------------------------------------------------------------*/
#define GUI_LV_BAR_SET_VALUE(_bar, _value, _is_anim)                           \
	do {                                                                       \
		if ((_bar) != NULL) {                                                  \
			lv_bar_set_value((_bar), (_value), (lv_anim_enable_t)(_is_anim));  \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Timer macro func                                                           *
 *----------------------------------------------------------------------------*/
#define GUI_LV_TIMER_SET(_timer, _cb, _period, _user_data)                     \
	do {                                                                       \
		if ((_timer) != NULL) {                                                \
			(_timer)->timer_cb  = (_cb);                                       \
			(_timer)->period    = (_period);                                   \
			(_timer)->user_data = (_user_data);                                \
		}                                                                      \
	} while (0)

#define GUI_LV_TIMER_START(_timer)                                             \
	do {                                                                       \
		if ((_timer) != NULL) {                                                \
			(_timer)->paused = false;                                          \
		}                                                                      \
	} while (0)

#define GUI_LV_TIMER_STOP(_timer)                                              \
	do {                                                                       \
		if ((_timer) != NULL) {                                                \
			(_timer)->paused = true;                                           \
		}                                                                      \
	} while (0)

#define GUI_LV_TIMER_ALL_START(_timer, _number)                                \
	do {                                                                       \
		for (uint8_t i = 0; i < (_number); i++) {                              \
			if ((_timer)[i] != NULL) {                                         \
				(_timer)[i]->paused = false;                                   \
			}                                                                  \
		}                                                                      \
	} while (0)

#define GUI_LV_TIMER_ALL_STOP(_timer, _number)                                 \
	do {                                                                       \
		for (uint8_t i = 0; i < (_number); i++) {                              \
			if ((_timer)[i] != NULL) {                                         \
				(_timer)[i]->paused = true;                                    \
			}                                                                  \
		}                                                                      \
	} while (0)

#define GUI_LV_TIMER_DESTROY(_timer)                                           \
	do {                                                                       \
		if ((_timer) != NULL) {                                                \
			(_timer)->paused = true;                                           \
			lv_timer_del((_timer));                                            \
			(_timer) = NULL;                                                   \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Group macro func                                                           *
 *----------------------------------------------------------------------------*/
#define GUI_LV_GROUP_FOCUS_NEXT(void)                                          \
	do {                                                                       \
		lv_group_t *group = LV_INDEV_KEYPAD->group;                            \
		lv_group_focus_next(group);                                            \
	} while (0)
    
#define GUI_LV_GROUP_FOCUS_PREV(void)                                          \
	do {                                                                       \
		lv_group_t *group = LV_INDEV_KEYPAD->group;                            \
		lv_group_focus_prev(group);                                            \
	} while (0)

#define GUI_LV_INDEV_BIND_GROUP(_group)                                        \
	do {                                                                       \
		if (LV_INDEV_KEYPAD != NULL) {                                         \
			lv_indev_set_group(LV_INDEV_KEYPAD, (_group));                     \
		}                                                                      \
	} while (0)

#define GUI_LV_GROUP_GET_FOCUSED_OBJ()                                         \
	({                                                                         \
		lv_obj_t *_ptObj = NULL;                                               \
		if (LV_INDEV_KEYPAD != NULL) {                                         \
			_ptObj = lv_group_get_focused(LV_INDEV_KEYPAD->group);             \
		}                                                                      \
		_ptObj;                                                                \
	})

#define GUI_LV_GROUP_DESTROY(_group)                                           \
	do {                                                                       \
		if ((_group) != NULL) {                                                \
			lv_group_remove_all_objs((_group));                                \
			lv_group_del((_group));                                            \
			(_group) = NULL;                                                   \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * Common macro func                                                          *
 *----------------------------------------------------------------------------*/
#define GUI_LV_DISP_LOAD_SCR(_scr)                                             \
	do {                                                                       \
		if ((_scr) != NULL) {                                                  \
			if(lv_scr_act() != NULL) {                                         \
				lv_scr_load_anim((_scr), LV_SCR_LOAD_ANIM_NONE,0,0,true);      \
			} else {                                                           \
				lv_disp_load_scr((_scr));                                      \
			}                                                                  \
		}                                                                      \
	} while (0)

#define GUI_LV_SET_ALIGN(_obj, _align, _ofs_x, _ofs_y)                         \
	do {                                                                       \
		if ((_obj) != NULL) {                                                  \
			lv_obj_align((_obj), (_align),                                     \
						 (int16_t)(_ofs_x), (int16_t)(_ofs_y));                \
		}                                                                      \
	} while (0)

#define GUI_LV_SET_ALIGN_TO(_obj, _base, _align, _ofs_x, _ofs_y)               \
	do {                                                                       \
		if ((_obj) != NULL) {                                                  \
			lv_obj_align_to((_obj), (_base), (_align),                         \
							(int16_t)(_ofs_x), (int16_t)(_ofs_y));             \
		}                                                                      \
	} while (0)

/*----------------------------------------------------------------------------*
 * MSG macro func                                                             *
 *----------------------------------------------------------------------------*/
#if LV_USE_MSG
typedef enum {
    MSG_INT, MSG_FLOAT, MSG_DOUBLE, MSG_STR, 
    MSG_U8,  MSG_U16, MSG_U32, MSG_U64,
} gui_lv_msg_type_t;

typedef enum {
    GUI_LV_MSG_ID1 = 0x0001,
    GUI_LV_MSG_ID2,
    GUI_LV_MSG_ID3,
    GUI_LV_MSG_ID4,
    GUI_LV_MSG_ID5,
    GUI_LV_MSG_ID6,
    GUI_LV_MSG_ID7,
    GUI_LV_MSG_ID8,
    GUI_LV_MSG_ID9,
    GUI_LV_MSG_ID10,
    GUI_LV_MSG_ID11,
    GUI_LV_MSG_ID12,
    GUI_LV_MSG_ID13,
} gui_lv_msg_id_t;

static inline
void gui_lv_label_display_event_cb(lv_event_t *e)
{
    lv_msg_t *ptMsg       = lv_event_get_msg(e);
    lv_obj_t *ptLabel     = lv_event_get_target(e);
    const char *pchFmt    = lv_msg_get_user_data(ptMsg);
    const void *pvPayload = lv_msg_get_payload(ptMsg);
    uintptr_t type        = (uintptr_t)lv_event_get_user_data(e);

    switch((gui_lv_msg_type_t)type)
    {
        case MSG_INT:    lv_label_set_text_fmt(ptLabel,pchFmt,*(const int*)pvPayload );     break;
        case MSG_FLOAT:  lv_label_set_text_fmt(ptLabel,pchFmt,*(const float*)pvPayload );   break;
        case MSG_DOUBLE: lv_label_set_text_fmt(ptLabel,pchFmt,*(const double*)pvPayload );  break;
        case MSG_STR:    lv_label_set_text_fmt(ptLabel,pchFmt,*(const char**)pvPayload );   break;
        case MSG_U8:     lv_label_set_text_fmt(ptLabel,pchFmt,*(const uint8_t*)pvPayload ); break;
        case MSG_U16:    lv_label_set_text_fmt(ptLabel,pchFmt,*(const uint16_t*)pvPayload );break;
        case MSG_U32:    lv_label_set_text_fmt(ptLabel,pchFmt,*(const uint32_t*)pvPayload );break;
        case MSG_U64:    lv_label_set_text_fmt(ptLabel,pchFmt,*(const uint64_t*)pvPayload );break;
        default:         break;
    }
    
}

#define GUI_LV_MSG_LABEL_SUBSCRIBE(_msg_id, _obj, _format, _type)              \
    do {                                                                       \
        if ((_obj) != NULL) {                                                  \
            lv_obj_add_event_cb((_obj),                                        \
                                gui_lv_label_display_event_cb,                 \
                                LV_EVENT_MSG_RECEIVED,                         \
                                (void *)_type);                                \
            lv_msg_subscribe_obj(_msg_id, _obj, (_format));                    \
        }                                                                      \
    } while (0)

#define GUI_LV_MSG_LABEL_UNSUBSCRIBE(_msg_id, _obj)                            \
    do {                                                                       \
        if ((_obj) != NULL) {                                                  \
            lv_obj_remove_event_cb((_obj), gui_lv_label_display_event_cb);     \
            lv_msg_unsubscribe_obj(_msg_id, _obj);                             \
        }                                                                      \
    } while (0)

#define GUI_LV_MSG_SEND(_msg_id, _payload)                                     \
    do {                                                                       \
        lv_msg_send(_msg_id, _payload);                                        \
    } while (0)
#endif



/*----------------------------------------------------------------------------*
 * PT Operations                                                              *
 *----------------------------------------------------------------------------*/
/*
Protothreads open source BSD-style license
The protothreads library is released under an open source license that allows 
both commercial and non-commercial use without restrictions. The only 
requirement is that credits is given in the source code and in the documentation 
for your product.

The full license text follows.

Copyright (c) 2004-2005, Swedish Institute of Computer Science.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the Institute nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS `AS IS' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

Author: Adam Dunkels
*/

typedef enum {
    gui_lv_fsm_rt_err          = -1,    //!< fsm error
    gui_lv_fsm_rt_cpl          = 0,     //!< fsm complete
    gui_lv_fsm_rt_on_going     = 1,     //!< fsm on-going
    gui_lv_fsm_rt_wait_for_obj = 2,     //!< fsm wait for IPC object
    gui_lv_fsm_rt_async        = 3,     //!< fsm work asynchronously, please check it later.
    gui_lv_fsm_rt_wait_for_res = 4,     //!< wait for resource
    __gui_lv_fsm_rt_last,
} gui_lv_fsm_rt_t;

#define GUI_LV_PT_BEGIN(__STATE)                                               \
            enum {                                                             \
                count_offset = __COUNTER__ + 1,                                \
            };                                                                 \
            uint8_t *ptPTState = &(__STATE);                                   \
            switch (__STATE) {                                                 \
                case __COUNTER__ - count_offset: (void)(*ptPTState);

#define GUI_LV_PT_ENTRY(...)                                                   \
            (*ptPTState) = (__COUNTER__ - count_offset + 1) >> 1;              \
            __VA_ARGS__                                                        \
            case (__COUNTER__ - count_offset) >> 1: (void)(*ptPTState);
            
#define GUI_LV_PT_YIELD(...)                                                   \
            GUI_LV_PT_ENTRY(return __VA_ARGS__;)
            
#define GUI_LV_PT_END()                                                        \
            (*ptPTState) = 0;                                                  \
            break;}

#define GUI_LV_PT_GOTO_PREV_ENTRY(...)    return __VA_ARGS__;

#define GUI_LV_PT_WAIT_UNTIL(__CONDITION, ...)                                 \
            GUI_LV_PT_ENTRY()                                                  \
                __VA_ARGS__;                                                   \
                if (!(__CONDITION)) {                                          \
                    GUI_LV_PT_GOTO_PREV_ENTRY(gui_lv_fsm_rt_on_going);         \
                }

#define GUI_LV_PT_WAIT_OBJ_UNTIL(__CONDITION, ...)                             \
            GUI_LV_PT_ENTRY()                                                  \
                __VA_ARGS__;                                                   \
                if (!(__CONDITION)) {                                          \
                    GUI_LV_PT_GOTO_PREV_ENTRY(gui_lv_fsm_rt_wait_for_obj);     \
                }

#define GUI_LV_PT_WAIT_RESOURCE_UNTIL(__CONDITION, ...)                        \
            GUI_LV_PT_ENTRY()                                                  \
                __VA_ARGS__;                                                   \
                if (!(__CONDITION)) {                                          \
                    GUI_LV_PT_GOTO_PREV_ENTRY(gui_lv_fsm_rt_wait_for_res);     \
                }

#define GUI_LV_PT_DELAY_MS(__MS, ...)                                          \
            GUI_LV_PT_ENTRY(                                                   \
                static int64_t GUI_LV_SAFE_NAME(s_lTimestamp);                 \
                int64_t *GUI_LV_SAFE_NAME(plTimestamp)                         \
                    = (&GUI_LV_SAFE_NAME(s_lTimestamp), ##__VA_ARGS__);        \
                *GUI_LV_SAFE_NAME(plTimestamp) =                               \
                    gui_lv_helper_get_system_timestamp();                      \
            )                                                                  \
            do {                                                               \
                GUI_LV_SAFE_NAME(plTimestamp)                                  \
                    = (&GUI_LV_SAFE_NAME(s_lTimestamp), ##__VA_ARGS__);        \
                int64_t GUI_LV_SAFE_NAME(lElapsedMs) =                         \
                    gui_lv_helper_convert_ticks_to_ms(                         \
                        gui_lv_helper_get_system_timestamp()                   \
                    -   *GUI_LV_SAFE_NAME(plTimestamp));                       \
                if (GUI_LV_SAFE_NAME(lElapsedMs) < (__MS)) {                   \
                    GUI_LV_PT_GOTO_PREV_ENTRY(gui_lv_fsm_rt_on_going);         \
                }                                                              \
                *GUI_LV_SAFE_NAME(plTimestamp) = 0;                            \
            } while(0)

#define GUI_LV_PT_REPORT_STATUS(...)                                           \
            GUI_LV_PT_ENTRY(                                                   \
                return __VA_ARGS__;                                            \
            )
            
#define GUI_LV_PT_RETURN(...)                                                  \
            (*ptPTState) = 0;                                                  \
            return __VA_ARGS__;

/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
extern int64_t gui_lv_helper_get_system_timestamp(void);
extern int64_t gui_lv_helper_convert_ticks_to_ms(int64_t lTick);

/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_UTILS_H__ */
