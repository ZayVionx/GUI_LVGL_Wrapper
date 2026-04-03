/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __GUI_LV_HELPER_BIND_H__
#define __GUI_LV_HELPER_BIND_H__

/*================================= INCLUDES =================================*/
#include "core/gui_lv_common.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
#define GUI_LV_BIND_LABEL_TO_U8(_OBJ, _ADDR, _FMT)                            \
    gui_lv_bind_label_to_u8((_OBJ), (_ADDR), (_FMT))

#define GUI_LV_BIND_LABEL_TO_U16(_OBJ, _ADDR, _FMT)                           \
    gui_lv_bind_label_to_u16((_OBJ), (_ADDR), (_FMT))

#define GUI_LV_BIND_ARC_TO_U8(_OBJ, _ADDR, _MIN, _MAX)                        \
    gui_lv_bind_arc_to_u8((_OBJ), (_ADDR), (_MIN), (_MAX))

#define GUI_LV_BIND_EDIT_TO_U16(_OBJ, _ADDR, _MIN, _MAX, _STEP)               \
    gui_lv_bind_edit_to_u16((_OBJ), (_ADDR), (_MIN), (_MAX), (_STEP))

#define GUI_LV_BIND_EDIT_TO_U16_EX(_OBJ, _ADDR, _MIN, _MAX, _STEP, _SETTER)   \
    gui_lv_bind_edit_to_u16_ex((_OBJ), (_ADDR), (_MIN), (_MAX), (_STEP), (_SETTER))

#define GUI_LV_BIND_ACTION(_OBJ, _CB, _PARAM)                                 \
    gui_lv_bind_action((_OBJ), (_CB), (_PARAM))

#define GUI_LV_BIND_NOTIFY(_ADDR)                                             \
    gui_lv_bind_notify((_ADDR))

#define GUI_LV_BIND_INC_BY_OBJ(_OBJ)                                          \
    gui_lv_bind_inc_by_obj((_OBJ))

#define GUI_LV_BIND_DEC_BY_OBJ(_OBJ)                                          \
    gui_lv_bind_dec_by_obj((_OBJ))

#define GUI_LV_BIND_EXEC_BY_OBJ(_OBJ)                                         \
    gui_lv_bind_exec_by_obj((_OBJ))

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
typedef enum {
    GUI_LV_BIND_DATA_U8 = 0,
    GUI_LV_BIND_DATA_U16,
} gui_lv_bind_data_type_t;

typedef bool (*gui_lv_bind_setter_t)(const void *pvValue, uint32_t u32Size);
typedef bool (*gui_lv_bind_getter_t)(void *pvValue, uint32_t u32Size);
typedef void (*gui_lv_bind_action_cb_t)(void *pvParam);
typedef void (*gui_lv_bind_style_u16_cb_t)(lv_obj_t *ptObj, uint16_t u16Value);

typedef struct {
    int32_t              i32Min;
    int32_t              i32Max;
    int32_t              i32Step;
    gui_lv_bind_setter_t pfSet;
    gui_lv_bind_getter_t pfGet;
} gui_lv_bind_edit_cfg_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
extern
bool gui_lv_bind_label_to_u8(lv_obj_t *ptObj,
                             const uint8_t *pu8Addr,
                             const char *pchFmt);

extern
bool gui_lv_bind_label_to_u16(lv_obj_t *ptObj,
                              const uint16_t *pu16Addr,
                              const char *pchFmt);

extern
bool gui_lv_bind_arc_to_u8(lv_obj_t *ptObj,
                           const uint8_t *pu8Addr,
                           int32_t i32Min,
                           int32_t i32Max);

extern
bool gui_lv_bind_edit_to_u16(lv_obj_t *ptObj,
                             uint16_t *pu16Addr,
                             uint16_t u16Min,
                             uint16_t u16Max,
                             uint16_t u16Step);

extern
bool gui_lv_bind_edit_to_u16_ex(lv_obj_t *ptObj,
                                uint16_t *pu16Addr,
                                uint16_t u16Min,
                                uint16_t u16Max,
                                uint16_t u16Step,
                                gui_lv_bind_setter_t pfSet);

extern
bool gui_lv_bind_action(lv_obj_t *ptObj,
                        gui_lv_bind_action_cb_t pfAction,
                        void *pvParam);

extern
bool gui_lv_bind_notify(const void *pvAddr);

extern
bool gui_lv_bind_inc_by_obj(lv_obj_t *ptObj);

extern
bool gui_lv_bind_dec_by_obj(lv_obj_t *ptObj);

extern
bool gui_lv_bind_exec_by_obj(lv_obj_t *ptObj);

extern
void gui_lv_bind_unbind_by_root(lv_obj_t *ptRoot);

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_HELPER_BIND_H__ */
