/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
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

#ifndef __GUI_LV_SCENE_MANAGE_H__
#define __GUI_LV_SCENE_MANAGE_H__

/*================================= INCLUDES =================================*/
#include "gui_lv_common.h"

#ifdef __GUI_LVGL_WRAPPER__
#   include <gui_scene_id.h>
#else
#   include "gui_scene_id.h"
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
#define GUI_SCENE_GROUP_MAX   4      /* Max groups per scene for focus save  */

/*================================== TYPES ===================================*/
typedef struct {
#if (__UI_LIB__ == __LVGL_LIB)
    uint8_t      u8GroupNum;       /* Number of groups in the scene  */
    uint8_t      u8TimerNum;       /* Number of timers in the scene  */
    lv_group_t **ptSceneGroup;     /* Pointer to scene's group array */
    lv_timer_t **ptSceneTimer;     /* Pointer to scene's timer array */
#else
    /* Extend for other GUI libraries */
#endif  /* __UI_LIB__ */
} gui_scene_ex_t;

typedef struct {
    gui_scene_id_t  eId;                         /* Scene identifier            */
    gui_scene_ex_t *ptEx;                        /* Extended scene data (opt.)  */

    void (*pfInit  )(lv_obj_t *ptRoot);         /* Scene init callback         */
    void (*pfDeinit)(void);                     /* Scene destroy callback      */
} gui_scene_cfg_t;

typedef struct {
    gui_scene_page_id_t eId;                      /* Page identifier            */
    void       (*pfInit  )(lv_obj_t *ptRoot);    /* Page init callback         */
    void       (*pfDeinit)(void);                /* Page destroy callback      */
    uint8_t      u8GroupIdx;                     /* Scene group index to bind  */
} gui_page_cfg_t;

/*================================ PROTOTYPES ================================*/

/**
 * @brief   Register a scene into the scene table.
 * @param   ptCfg: Scene config pointer (eId, pfInit, pfDeinit required).
 * @return  true: Success; false: Fail.
 */
extern
bool gui_lv_scene_register(const gui_scene_cfg_t *ptCfg);

/**
 * @brief   Switch to a different scene.
 * @note    - Fully tears down current scene (pages, groups, timers, root).
 *          - Pushes current scene ID onto back-navigation history.
 *          - Target scene is always initialized from scratch via pfInit.
 * @param   eTargetId: Target scene ID.
 */
extern
void gui_lv_scene_switch(gui_scene_id_t eTargetId);

/**
 * @brief   Go back to the previous scene.
 * @note    - Pops one scene from the history stack.
 *          - Fully tears down current scene.
 *          - Previous scene is initialized from scratch (no state restore).
 *          - No-op if history stack is empty.
 */
extern
void gui_lv_scene_back(void);

/**
 * @brief   Register a page config into the page table.
 * @param   ptCfg: Page config pointer (eId, pfInit required).
 * @return  true: Success; false: Fail.
 */
extern
bool gui_lv_scene_page_register(const gui_page_cfg_t *ptCfg);

/**
 * @brief   Push a sub-page onto the current scene's page chain.
 * @note    - Saves the current page's focus position.
 *          - Destroys current view's UI (lv_obj_clean) to free memory.
 *          - Creates a full-screen container and passes it to pfInit.
 *          - Resolves group from current scene's ptEx->ptSceneGroup[u8GroupIdx].
 * @param   ePageId: Registered page identifier.
 */
extern
void gui_lv_scene_page_push(gui_scene_page_id_t ePageId);

/**
 * @brief   Go back one page in the current scene's page chain.
 * @note    - Tears down the top page (pfDeinit + lv_obj_clean).
 *          - Re-creates the previous page by calling its pfInit again.
 *          - Restores group binding and focus position (silent).
 *          - No-op if already at the scene's base view.
 */
extern
void gui_lv_scene_page_back(void);

/**
 * @brief   Get current scene ID.
 * @return  Current scene ID, or UI_SCENE_MAX if none active.
 */
extern
gui_scene_id_t gui_lv_scene_get_current(void);

/**
 * @brief   Get current scene root object.
 * @return  Current scene root object, or NULL if none active.
 */
extern
lv_obj_t *gui_lv_scene_get_current_root(void);

/**
 * @brief   Get current scene focus index for a specific group.
 * @param   eSceneId:   Scene identifier.
 * @param   u8GroupIdx: Group index (0-based).
 * @return  Saved focus index for that group, or -1 if none.
 */
extern
int16_t gui_lv_scene_get_focus_index(gui_scene_id_t eSceneId, uint8_t u8GroupIdx);

/**
 * @brief   Get current scene page focus index for a specific group.
 * @param   ePageId:    Page identifier.
 * @param   u8GroupIdx: Group index (0-based).
 * @return  Saved focus index for that group, or -1 if none.
 */
extern
int16_t gui_lv_scene_page_get_focus_index(gui_scene_page_id_t ePageId, uint8_t u8GroupIdx);

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_MANAGE_H__ */
