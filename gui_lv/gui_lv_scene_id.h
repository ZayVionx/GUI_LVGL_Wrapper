/****************************************************************************
*  Copyright 2025 ZJY <Email:zhujinyuan818@gemail.com>                      *
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

/* ----------------------------------------------------------------------
 * Project:      GUI LVGL Wrapper
 * Title:        gui_scene_id.h
 * Description:  Canonical scene and page identifier definitions.
 * -------------------------------------------------------------------- */

#ifndef __GUI_LV_SCENE_ID_H__
#define __GUI_LV_SCENE_ID_H__

/*================================= INCLUDES =================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== TYPES ===================================*/
typedef enum {
    /* 占位枚举（请勿使用） */
    PLACEHOLDER0,

    /* ---------- Scene ID Begin ---------- */

    /* ----------- Scene ID End ----------- */

    GUI_LV_SCENE_MAX
} gui_lv_scene_id_t;


typedef enum {
    /* 占位枚举（请勿使用） */
    PLACEHOLDER1,
    
    /* ----------- Page ID Begin ---------- */
    
    /* ----------- Page ID End ------------ */

    GUI_LV_PAGE_MAX
} gui_lv_page_id_t;

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_ID_H__ */
