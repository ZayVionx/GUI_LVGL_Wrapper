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

/* ----------------------------------------------------------------------
 * Project:      GUI LVGL Wrapper
 * Title:        gui_scene_id.h
 * Description:  Canonical scene and page identifier definitions.
 * -------------------------------------------------------------------- */

#ifndef __GUI_SCENE_ID_H__
#define __GUI_SCENE_ID_H__

/*================================= INCLUDES =================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== TYPES ===================================*/
typedef enum {
    /* scene id to begin */

    /* scene id to end   */

    GUI_SCENE_MAX
} gui_scene_id_t;


typedef enum {
    /* page id to begin */
    
    /* page id to end   */

    GUI_PAGE_MAX
} gui_page_id_t;

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_SCENE_ID_H__ */
