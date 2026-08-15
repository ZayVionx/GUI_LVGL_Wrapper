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

/*============================ INCLUDES ======================================*/
#include "gui_lv_page_cursor.h"

/*============================ MACROS ========================================*/
#undef  this
#define this    (*ptThis)
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
static bool __page_cursor_check_index(gui_lv_page_cursor_index_e eIndexBase);
static void __page_cursor_clear      (gui_lv_page_cursor_t *ptThis);
static void __page_cursor_refresh    (gui_lv_page_cursor_t *ptThis);

/*============================ IMPLEMENTATION ================================*/
/*******************************************************************************
 * @brief Initialize the page cursor
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] chPageSize Maximum item count per page
 * @param[in] eIndexBase Item index base, 0-based or 1-based
 * 
 * @return true on success, false on failure
 ******************************************************************************/
bool gui_lv_page_cursor_init(gui_lv_page_cursor_t       *ptThis,
                             uint8_t                    chPageSize,
                             gui_lv_page_cursor_index_e eIndexBase)
{
    if(    (ptThis == NULL) || (chPageSize == 0) 
        || (!__page_cursor_check_index(eIndexBase)) )   return false;
        

    this.chPageSize = chPageSize;
    this.eIndexBase = eIndexBase;

    __page_cursor_clear(ptThis);

    return true;
}

/*******************************************************************************
 * @brief Reset the page cursor to its initial state
 * @param[in] ptThis Pointer to the page cursor structure
 * @note This function can be used to clear the page cursor information
 *       and reset it to the initial state.
 ******************************************************************************/
void gui_lv_page_cursor_reset(gui_lv_page_cursor_t *ptThis)
{
    if(ptThis == NULL)  return;

    this.u16CurPage   = 1;
    this.chFocusIndex = 0;
    __page_cursor_refresh(ptThis);
}

/*******************************************************************************
 * @brief Update the page cursor based on the total item count and current page number
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] u16ItemNum Total item count
 ******************************************************************************/
void gui_lv_page_cursor_update(gui_lv_page_cursor_t *ptThis, uint16_t u16ItemNum)
{
    if(ptThis == NULL) return;

    this.u16ItemNum = u16ItemNum;
    __page_cursor_refresh(ptThis);
}

/*******************************************************************************
 * @brief Move the page cursor to the previous item
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] bIsLoop When the first item has been reached,
 *                    will it loop back to the last item of the previous page?
 * @return true if the page cursor successfully moved to the previous item,
 *         false if already at the first item
 ******************************************************************************/
extern
bool gui_lv_page_cursor_prev_item(gui_lv_page_cursor_t *ptThis, bool bIsLoop)
{
    if(ptThis == NULL || this.chVaildNum == 0) return false;

    if(this.chFocusIndex > 0)
    {
        this.chFocusIndex--;
        return true;
    }

    if(this.u16CurPage > 1)
    {
        this.u16CurPage--;
        __page_cursor_refresh(ptThis);
        this.chFocusIndex = this.chVaildNum - 1;
        return true;
    }

    if(!bIsLoop) return false;

    this.u16CurPage = this.u16PageNum;
    __page_cursor_refresh(ptThis);
    this.chFocusIndex = this.chVaildNum - 1;

    return true;
}

/*******************************************************************************
 * @brief Move the page cursor to the next item
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] bIsLoop When the last item has been reached,
 *                    will it loop back to the first item of the next page?
 * @return true if the page cursor successfully moved to the next item,
 *         false if already at the last item
 ******************************************************************************/
extern
bool gui_lv_page_cursor_next_item(gui_lv_page_cursor_t *ptThis, bool bIsLoop)
{
    if(ptThis == NULL || this.chVaildNum == 0) return false;

    if(this.chFocusIndex < (this.chVaildNum - 1))
    {
        this.chFocusIndex++;
        return true;
    }

    if(this.u16CurPage < this.u16PageNum)
    {
        this.u16CurPage++;
        __page_cursor_refresh(ptThis);
        this.chFocusIndex = 0;
        return true;
    }

    if(!bIsLoop) return false;

    this.u16CurPage = 1;
    __page_cursor_refresh(ptThis);
    this.chFocusIndex = 0;

    return true;
}

/*******************************************************************************
 * @brief Move the page cursor to the previous page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor successfully moved to the previous page,
 *         false if already at the first page
 ******************************************************************************/
bool gui_lv_page_cursor_prev_page(gui_lv_page_cursor_t *ptThis)
{
    if(ptThis == NULL || this.u16CurPage <= 1) 
        return false;

    this.u16CurPage--;
    __page_cursor_refresh(ptThis);

    return true;
}

/*******************************************************************************
 * @brief Move the page cursor to the next page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor successfully moved to the next page,
 *         false if already at the last page
 ******************************************************************************/
bool gui_lv_page_cursor_next_page(gui_lv_page_cursor_t *ptThis)
{
    if(    (ptThis == NULL) 
        || (this.u16CurPage >= this.u16PageNum))    return false;

    this.u16CurPage++;
    __page_cursor_refresh(ptThis);

    return true;
}

/*******************************************************************************
 * @brief Move the page cursor to a specific page number
 * @param[in] ptThis Pointer to the page cursor structure
 * @param[in] u16PageNum Target page number (1-based)
 * @return true if the page cursor successfully moved to the target page,
 *         false if the target page number is out of range
 ******************************************************************************/
bool gui_lv_page_cursor_goto(gui_lv_page_cursor_t *ptThis, uint16_t u16PageNum)
{
    if(ptThis == NULL || u16PageNum < 1 || u16PageNum > this.u16PageNum)
        return false;

    if(this.u16CurPage == u16PageNum)
        return false;

    this.u16CurPage = u16PageNum;
    __page_cursor_refresh(ptThis);

    return true;    
}

/*******************************************************************************
 * @brief Check if the page cursor is empty (no items)
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is empty, false otherwise
 ******************************************************************************/
bool gui_lv_page_cursor_is_empty(const gui_lv_page_cursor_t *ptThis)
{
    return (ptThis == NULL) ? true 
                            : (this.u16ItemNum == 0);
}

/*******************************************************************************
 * @brief Check if the page cursor is at the first page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is at the first page, false otherwise
 ******************************************************************************/
bool gui_lv_page_cursor_is_first(const gui_lv_page_cursor_t *ptThis)
{
    return (ptThis == NULL) ? false 
                            : (this.u16CurPage <= 1);
}

/*******************************************************************************
 * @brief Check if the page cursor is at the last page
 * @param[in] ptThis Pointer to the page cursor structure
 * @return true if the page cursor is at the last page, false otherwise
 ******************************************************************************/
bool gui_lv_page_cursor_is_last(const gui_lv_page_cursor_t *ptThis)
{
    return (ptThis == NULL) ? false 
                            : (this.u16CurPage >= this.u16PageNum);
}


/*============================ LOCAL FUNCTION IMPLEMENTATION =================*/
static bool __page_cursor_check_index(gui_lv_page_cursor_index_e eIndexBase)
{
    switch(eIndexBase)
    {
        case GUI_LV_PAGE_CURSOR_INDEX_0:
        case GUI_LV_PAGE_CURSOR_INDEX_1:
            return true;

        default:
            return false;
    }
}

static void __page_cursor_clear(gui_lv_page_cursor_t *ptThis)
{
    if(ptThis == NULL) return;

    this.u16ItemNum    = 0;
    this.u16PageNum    = 1;
    this.u16CurPage    = 1;
    this.chVaildNum    = 0;
    this.chStartIndex  = 0;
    this.chEndIndex    = 0;
    this.chFocusIndex  = 0;
}

static void __page_cursor_refresh(gui_lv_page_cursor_t *ptThis)
{
    uint16_t u16PageNum;
    uint16_t u16CurPage;
    uint32_t u32IndexBase;
    uint32_t u32StartOffset;
    uint32_t u32RemainNum;

    if(ptThis == NULL) return;

    if(    (this.chPageSize == 0)
        || (this.u16ItemNum == 0))
    {
        __page_cursor_clear(ptThis);
        return;
    }

    u16PageNum = (uint16_t)(((uint32_t)this.u16ItemNum + this.chPageSize - 1)
               / this.chPageSize);

    u16CurPage = this.u16CurPage;

    if(u16CurPage == 0)
        u16CurPage = 1;
    else if(u16CurPage > u16PageNum)
        u16CurPage = u16PageNum;

    this.u16PageNum = u16PageNum;
    this.u16CurPage = u16CurPage;

    u32IndexBase   = (uint32_t)this.eIndexBase;
    u32StartOffset = ((uint32_t)u16CurPage - 1) * this.chPageSize;
    u32RemainNum   = (uint32_t)this.u16ItemNum - u32StartOffset;

    if(u32StartOffset > (UINT8_MAX - u32IndexBase))
        this.chStartIndex = UINT8_MAX;
    else
        this.chStartIndex = (uint8_t)(u32StartOffset + u32IndexBase);

    this.chVaildNum = u32RemainNum > this.chPageSize
                    ? this.chPageSize
                    : (uint8_t)u32RemainNum;

    if(this.chVaildNum == 0)
    {
        this.chFocusIndex = 0;
    }
    else if(this.chFocusIndex >= this.chVaildNum)
    {
        this.chFocusIndex = this.chVaildNum - 1;
    }

    if(this.chVaildNum == 0)
    {
        this.chEndIndex = 0;
    }
    else
    {
        uint32_t u32EndOffset = u32StartOffset + this.chVaildNum - 1 + u32IndexBase;

        if(u32EndOffset > UINT8_MAX)
            this.chEndIndex = UINT8_MAX;
        else
            this.chEndIndex = (uint8_t)u32EndOffset;
    }
}
/*============================ END ===========================================*/
