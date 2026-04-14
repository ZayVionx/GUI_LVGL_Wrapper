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

/*================================= INCLUDES =================================*/
#define __GUI_LV_CUSTOM_IMPLEMENT__
#include "gui_lv_custom_components.h"


/*================================== MACROS ==================================*/
#define BATT_BLINK_PERIOD_MS    500                  //!< the blink period in millisecond
#define BATT_COLOR_IDLE         rgb(229, 229, 229) //!< the colour of the gauge in idle mode
#define BATT_COLOR_CHARGING     rgb(0, 255, 0)     //!< the colour of the gauge in charging mode

/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
static void __gui_lv_battery_set_frame_img(gui_lv_custom_battery_t *ptBattery,
                                           const void *pImgScr);
static void __gui_lv_battery_refresh_display(gui_lv_custom_battery_t *ptBattery);
static void __gui_lv_battery_timer_cb(lv_timer_t *ptTimer);

/*============================== IMPLEMENTATION ==============================*/

/*----------------------------------------------------------------------------*
 * Battery Component                                                          *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Create a custom battery component.
 * \param[in] ptBattery: Pointer to battery component structure
 */
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_create(gui_lv_custom_battery_t *ptBattery)
{
    gui_lv_custom_battery_t *ptThis = ptBattery;

    uint32_t u32ImgSize   = ((const lv_img_dsc_t *)ptThis->pImgScr)->header.w; 
    uint32_t u32ImgHeight = ((const lv_img_dsc_t *)ptThis->pImgScr)->header.h; 

    /* 创建电池外框图像对象 */
    ptThis->ptFrameImg = gui_lv_img_init(ptThis->ptParent,
                                           0, 0,
                                           u32ImgSize, u32ImgHeight,
                                           ptThis->pImgScr);
    lv_obj_align(ptThis->ptFrameImg, ptThis->chAlign, ptThis->i16X, ptThis->i16Y);
    ptThis->tAnim.pCurrentImgScr = ptThis->pImgScr;
    
    /* 创建电量格对象 */
    uint16_t u16GaugeW  = ptThis->BattGuage.chBattGuageW;
    uint8_t  chStartX   = ptThis->BattGuage.chStartX;
    uint8_t  chGap      = ptThis->BattGuage.chGap;
    uint8_t  chMax      = ptThis->BattGuage.chBattMax;

    for (uint8_t i = 0; i < chMax; i++)
    {
        lv_obj_t *ptGuage = lv_obj_create(ptThis->ptFrameImg);
        lv_obj_set_size(ptGuage, u16GaugeW, ptThis->BattGuage.chBattGuageH);

        lv_coord_t i16PosX = chStartX + i * (u16GaugeW + chGap);
        lv_obj_align(ptGuage, LV_ALIGN_LEFT_MID, i16PosX, 0);

        lv_obj_set_style_border_width(ptGuage, 0, 0);
        lv_obj_set_style_outline_width(ptGuage, 0, 0);
        lv_obj_set_style_outline_pad(ptGuage, 0, 0);
        lv_obj_set_style_bg_color(ptGuage, BATT_COLOR_IDLE, 0);
        
        lv_obj_clear_flag(ptGuage, LV_OBJ_FLAG_SCROLLABLE);
    }

    /* 初始化动画状态 */
    ptThis->tAnim.bBlinkVisible  = false;
    ptThis->tAnim.bIsCharging    = false;
    ptThis->tAnim.bPendingUpdate = false;
    ptThis->tAnim.chDisplayLevel = ptThis->BattGuage.chBattLevel;

    /* 创建闪烁定时器（初始为停止状态） */
    ptThis->tAnim.ptTimer = lv_timer_create(__gui_lv_battery_timer_cb, BATT_BLINK_PERIOD_MS, ptBattery);
    GUI_LV_TIMER_STOP(ptThis->tAnim.ptTimer);

    /* 初始显示 */
    __gui_lv_battery_refresh_display(ptThis);
}

/*!
 * \brief Destroy a custom battery component: release timer and all UI objects.
 * \param[in] ptBattery: Pointer to battery component structure
 */
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_destroy(gui_lv_custom_battery_t *ptBattery)
{
    GUI_LV_TIMER_DESTROY(ptBattery->tAnim.ptTimer);
	
	if (ptBattery->ptFrameImg) 
    {
        lv_obj_del(ptBattery->ptFrameImg);
        ptBattery->ptFrameImg = NULL;
    }
}

/*!
 * \brief Enter idle mode: stop blinking and display current battery level in gray.
 * \param[in] ptBattery: Pointer to battery component structure
 */
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_enter_idle_mode(gui_lv_custom_battery_t *ptBattery)
{
    ptBattery->tAnim.bIsCharging    = false;
    ptBattery->tAnim.bPendingUpdate = false;
    ptBattery->tAnim.chDisplayLevel = ptBattery->BattGuage.chBattLevel;

    GUI_LV_TIMER_STOP(ptBattery->tAnim.ptTimer);
    __gui_lv_battery_refresh_display(ptBattery);
}

/*!
 * \brief Enter charging mode: start blinking animation.
 * \param[in] ptBattery: Pointer to battery component structure
 */
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_enter_charging_mode(gui_lv_custom_battery_t *ptBattery)
{
    ptBattery->tAnim.bIsCharging    = true;
    ptBattery->tAnim.bPendingUpdate = false;
    ptBattery->tAnim.chDisplayLevel = ptBattery->BattGuage.chBattLevel;
    ptBattery->tAnim.bBlinkVisible  = false;

    GUI_LV_TIMER_START(ptBattery->tAnim.ptTimer);
    __gui_lv_battery_refresh_display(ptBattery);
}

/*!
 * \brief Set the battery level.
 * \param[in] ptBattery: Pointer to battery component structure
 * \param[in] chBattLevel: Battery level to set
 */
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_set_level(gui_lv_custom_battery_t *ptBattery, 
                                     uint8_t chBattLevel)
{
    if (chBattLevel > ptBattery->BattGuage.chBattMax) {
        chBattLevel = ptBattery->BattGuage.chBattMax;
    }

    ptBattery->BattGuage.chBattLevel = chBattLevel;

    if (ptBattery->tAnim.bIsCharging) {
        /* 充电中：标记待更新，等闪烁周期结束后同步 */
        ptBattery->tAnim.bPendingUpdate = true;
    } else {
        /* 空闲模式：直接更新显示 */
        ptBattery->tAnim.chDisplayLevel = chBattLevel;
        __gui_lv_battery_refresh_display(ptBattery);
    }
}

/*!
 * \brief an internal helper function to set the battery frame image
 * 
 * \param[in] ptThis the target battery component
 * \param[in] pImgScr the pointer of the image source
 */
static 
GUI_LV_NONNULL(1)
void __gui_lv_battery_set_frame_img(gui_lv_custom_battery_t *ptBattery,
                                           const void *pImgScr)
{
    const lv_img_dsc_t *ptImgDsc;

    if(ptBattery->ptFrameImg == NULL || pImgScr == NULL) return;

    if(ptBattery->tAnim.pCurrentImgScr == pImgScr)       return;

    ptImgDsc = (const lv_img_dsc_t *)pImgScr;
    GUI_LV_IMG_SET_SRC( ptBattery->ptFrameImg, 
                        ptImgDsc->header.w, 
                        ptImgDsc->header.h, 
                        pImgScr);
    lv_obj_align(ptBattery->ptFrameImg, ptBattery->chAlign, ptBattery->i16X, ptBattery->i16Y);

    ptBattery->tAnim.pCurrentImgScr = pImgScr;
}

/*!
 * \brief an internal helper function to refresh the battery gauge display
 * 
 * \param[in] ptThis the target battery component
 * \note 
 *       - Charging mode: gauges [0, chDisplayLevel) are shown in green, 
 *         gauge [chDisplayLevel] is blinking, and the rest are hidden.
 *       - Idle mode: gauges [0, chBattLevel) are shown in gray, and the 
 *         rest are hidden.
 *       - When idle, level is 0, and a low-battery image is configured: 
 *         switch to the low-battery image and hide all gauges.
 */
static 
GUI_LV_NONNULL(1)
void __gui_lv_battery_refresh_display(gui_lv_custom_battery_t *ptBattery)
{
    uint8_t chDisplayLevel = ptBattery->tAnim.chDisplayLevel;
    bool    bIsCharging    = ptBattery->tAnim.bIsCharging;
    bool    bBlinkVisible  = ptBattery->tAnim.bBlinkVisible;
    bool    bShowLowImg    = (!bIsCharging) &&
                             (ptBattery->BattGuage.chBattLevel == 0U) &&
                             (ptBattery->pLowImgScr != NULL);
    lv_color_t tColor      = bIsCharging ? BATT_COLOR_CHARGING : BATT_COLOR_IDLE;

    __gui_lv_battery_set_frame_img(ptBattery, bShowLowImg ? ptBattery->pLowImgScr : ptBattery->pImgScr);

    for (uint8_t i = 0; i < ptBattery->BattGuage.chBattMax; i++)
    {
        lv_obj_t *ptGuage = lv_obj_get_child(ptBattery->ptFrameImg, i);

        if (bShowLowImg) {
            lv_obj_add_flag(ptGuage, LV_OBJ_FLAG_HIDDEN);
            continue;
        }

        if (i < chDisplayLevel) {
            /* 已确认的电量格：显示 */
            lv_obj_set_style_bg_color(ptGuage, tColor, 0);
            lv_obj_clear_flag(ptGuage, LV_OBJ_FLAG_HIDDEN);
        }
        else if (bIsCharging && (i == chDisplayLevel) && (chDisplayLevel < ptBattery->BattGuage.chBattMax)) {
            /* 闪烁格：充电模式下，下一格做闪烁 */
            lv_obj_set_style_bg_color(ptGuage, BATT_COLOR_CHARGING, 0);
            if (bBlinkVisible)  lv_obj_clear_flag(ptGuage, LV_OBJ_FLAG_HIDDEN);
            else                lv_obj_add_flag(ptGuage, LV_OBJ_FLAG_HIDDEN);
        }
        else {
            /* 超出电量范围：隐藏 */
            lv_obj_add_flag(ptGuage, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/*!
 * \brief the blink timer callback
 * 
 * \param[in] ptTimer the attached LVGL timer pointer
 * \note It toggles the visibility of the blinking gauge and synchronizes 
 *       the battery level at the end of a complete cycle (visible -> invisible).
 */
static void __gui_lv_battery_timer_cb(lv_timer_t *ptTimer)
{
    gui_lv_custom_battery_t *ptBattery = (gui_lv_custom_battery_t *)ptTimer->user_data;

    /* 切换闪烁状态 */
    ptBattery->tAnim.bBlinkVisible = !ptBattery->tAnim.bBlinkVisible;

    /* 从 可见→不可见 时，是一个完整闪烁周期结束点，检查是否需要同步电量 */
    if (!ptBattery->tAnim.bBlinkVisible && ptBattery->tAnim.bPendingUpdate) {
        ptBattery->tAnim.bPendingUpdate = false;
        ptBattery->tAnim.chDisplayLevel = ptBattery->BattGuage.chBattLevel;
    }

    __gui_lv_battery_refresh_display(ptBattery);
}

#if 0 /* ======================== Usage instructions ======================== */

gui_lv_custom_battery_t tBattery;

void gui_lv_batt_draw(void)
{
    /* step1. 初始化电池组件 */
    tBattery.ptParent = s_tScene.ptRoot;
    tBattery.pImgScr  = &lv_img_battery_frame;
    tBattery.chAlign  = LV_ALIGN_CENTER;
    tBattery.i16X     = 0;
    tBattery.i16Y     = 0;
    tBattery.BattGuage.chBattGuageW = 19;
    tBattery.BattGuage.chBattGuageH = 48;
    tBattery.BattGuage.chBattMax    = 5;
    tBattery.BattGuage.chStartX     = 4;       /* 第一格起始X */
    tBattery.BattGuage.chGap        = 3;       /* 格间距 */
    tBattery.BattGuage.chBattLevel  = 3;       /* 初始电量 */
    gui_lv_custom_battery_create(&tBattery);
}

/* 在某个定时器/事件回调中调用 */
void gui_lv_batt_update(bool bIsCharging, uint8_t chLevel)
{
    static bool bPrevCharging = false;

    /* 充电状态变化时切换模式 */
    if (bPrevCharging != bIsCharging) {
        if (bIsCharging) {
            gui_lv_custom_battery_enter_charging_mode(&tBattery);
        } else {
            gui_lv_custom_battery_enter_idle_mode(&tBattery);
        }
        bPrevCharging = bIsCharging;
    }

    /* 随时可以设置电量，组件内部会处理异步同步 */
    gui_lv_custom_battery_set_level(&tBattery, chLevel);
}

#endif /* ==================== END 用户使用示例 ====================== */

/*=================================== END ====================================*/
