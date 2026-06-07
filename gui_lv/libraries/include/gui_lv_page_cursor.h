/******************************************************************************
 * @file     gui_lv_page_cursor.h
 * @brief    Public header file for GUI LV Page Cursor
 * @version  V1.0.0
 * @date     6 Jun 2026
 ******************************************************************************/
/****************************************************************************
*  Copyright 2025 ZJY <Email:zhujinyuan818@gmail.com>                       *
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

#ifndef __GUI_LV_PAGE_CURSOR_H__
#define __GUI_LV_PAGE_CURSOR_H__

/*================================= INCLUDES =================================*/
#include <stdint.h>
#include <stdbool.h>

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
#ifndef NULL
#   define NULL     ((void *)0)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
typedef enum {
    GUI_LV_PAGE_CURSOR_INDEX_0 = 0,
    GUI_LV_PAGE_CURSOR_INDEX_1 = 1,
} gui_lv_page_cursor_index_e;

typedef struct {
    uint16_t                   u16ItemNum  ;    //!< Total item count
    uint16_t                   u16PageNum  ;    //!< Total page count, minimum is 1
    uint16_t                   u16CurPage  ;    //!< Current page number, 1-based
    uint8_t                    chPageSize  ;    //!< Maximum item count per page
    uint8_t                    chVaildNum  ;    //!< Valid item count on the current page
    uint8_t                    chStartIndex;    //!< Start item index of the current page
    uint8_t                    chEndIndex  ;    //!< End item index of the current page
    uint8_t                    chFocusIndex;    //!< Focused item index on the current page(0-vaildNum-1), or 0 if no focus
    gui_lv_page_cursor_index_e eIndexBase  ;    //!< Item index base, 0-based or 1-based
} gui_lv_page_cursor_t;

/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/**
 * @brief Initialize the page cursor
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] chPageSize Maximum item count per page
 * @param[in] eIndexBase Item index base, 0-based or 1-based
 * 
 * @return true on success, false on failure
 */
extern
bool gui_lv_page_cursor_init(gui_lv_page_cursor_t       *ptThis,
                             uint8_t                    chPageSize,
                             gui_lv_page_cursor_index_e eIndexBase);

/**
 * @brief Reset the page cursor to its initial state
 * @param[in] ptThis Pointer to the page cursor structure
 * @note This function can be used to clear the page cursor information
 *       and reset it to the initial state.
 */
extern
void gui_lv_page_cursor_reset(gui_lv_page_cursor_t *ptThis);

/**
 * @brief Update the page cursor based on the total item count and current page number
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] u16ItemNum Total item count
 */
extern
void gui_lv_page_cursor_update(gui_lv_page_cursor_t *ptThis, uint16_t u16ItemNum);

/**
 * @brief Move the page cursor to the previous item
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] bIsLoop When the first item has been reached,
 *                    will it loop back to the last item of the previous page?
 * @return true if the page cursor successfully moved to the previous item,
 *         false if already at the first item
 */
extern
bool gui_lv_page_cursor_prev_item(gui_lv_page_cursor_t *ptThis, bool bIsLoop);

/**
 * @brief Move the page cursor to the next item
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] bIsLoop When the last item has been reached,
 *                    will it loop back to the first item of the next page?
 * @return true if the page cursor successfully moved to the next item,
 *         false if already at the last item
 */
extern
bool gui_lv_page_cursor_next_item(gui_lv_page_cursor_t *ptThis, bool bIsLoop);

/**
 * @brief Move the page cursor to the previous page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor successfully moved to the previous page,
 *         false if already at the first page
 */
extern
bool gui_lv_page_cursor_prev_page(gui_lv_page_cursor_t *ptThis);

/**
 * @brief Move the page cursor to the next page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor successfully moved to the next page,
 *         false if already at the last page
 */
extern
bool gui_lv_page_cursor_next_page(gui_lv_page_cursor_t *ptThis);

/**
 * @brief Move the page cursor to a specific page number
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] u16PageNum Target page number (1-based)
 * @return true if the page cursor successfully moved to the target page,
 *         false if the target page number is out of range
 */
extern
bool gui_lv_page_cursor_goto(gui_lv_page_cursor_t *ptThis, uint16_t u16PageNum);

/**
 * @brief Check if the page cursor is empty (no items)
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is empty, false otherwise
 */
extern
bool gui_lv_page_cursor_is_empty(const gui_lv_page_cursor_t *ptThis);

/**
 * @brief Check if the page cursor is at the first page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is at the first page, false otherwise
 */
extern
bool gui_lv_page_cursor_is_first(const gui_lv_page_cursor_t *ptThis);

/**
 * @brief Check if the page cursor is at the last page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is at the last page, false otherwise
 */
extern
bool gui_lv_page_cursor_is_last(const gui_lv_page_cursor_t *ptThis);
/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_PAGE_CURSOR_H__ */
