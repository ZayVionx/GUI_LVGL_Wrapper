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

#ifndef __GUI_LV_CUSTOM_COMPONENTS_H__
#define __GUI_LV_CUSTOM_COMPONENTS_H__

/*================================= INCLUDES =================================*/
#include "gui_lv_utils.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/

/*----------------------------------------------------------------------------*
 * Battery Component                                                          *
 *----------------------------------------------------------------------------*/
typedef struct {
    lv_obj_t   *ptParent;               //!< 父容器对象
    lv_obj_t   *ptFrameImg;             //!< 电池外框图对象 
    void       *pImgScr;                //!< 电池外框图 
    void       *pLowImgScr;             //!< 低电量图（空闲且电量为0时显示，可为空）
    lv_align_t  chAlign;                //!< 电池对齐方式   
    lv_coord_t  i16X;                   //!< 电池组件X坐标
    lv_coord_t  i16Y;                   //!< 电池组件Y坐标

    /* 电量格属性 */
    struct {
        uint8_t    chBattGuageW;        //!< 电池格宽
        uint8_t    chBattGuageH;        //!< 电池格高
        uint8_t    chBattMax;           //!< 电池最大电量
        uint8_t    chBattLevel;         //!< 电池当前电量（用户设置的目标值)
        uint8_t    chStartX;            //!< 第一个电量格的起始X偏移
        uint8_t    chGap;               //!< 电量格之间的间距
    } BattGuage;

    /* 充电动画内部状态（用户无需关心） */
    struct {
        lv_timer_t *ptTimer;            //!< 闪烁定时器
        const void *pCurrentImgScr;     //!< 当前显示的电池图源
        uint8_t     chDisplayLevel;     //!< 当前显示的固态电量格数
        bool        bBlinkVisible;      //!< 闪烁格当前是否可见
        bool        bIsCharging;        //!< 是否处于充电模式
        bool        bPendingUpdate;     //!< 是否有待同步的电量更新
    } tAnim;
    
} gui_lv_custom_battery_t;



/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/

/*----------------------------------------------------------------------------*
 * Battery Component                                                          *
 *----------------------------------------------------------------------------*/
/*!
 * \brief Create a custom battery component.
 * \param[in] ptBattery: Pointer to battery component structure
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_create(gui_lv_custom_battery_t *ptBattery);

/*!
 * \brief Destroy a custom battery component: release timer and all UI objects.
 * \param[in] ptBattery: Pointer to battery component structure
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_destroy(gui_lv_custom_battery_t *ptBattery);

/*!
 * \brief Enter idle mode: stop blinking, show current level in gray.
 * \param[in] ptBattery: Pointer to battery component structure
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_enter_idle_mode(gui_lv_custom_battery_t *ptBattery);

/*!
 * \brief Enter charging mode: start blinking animation.
 * \param[in] ptBattery: Pointer to battery component structure
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_enter_charging_mode(gui_lv_custom_battery_t *ptBattery);

/*!
 * \brief Set battery level.
 * \param[in] ptBattery: Pointer to battery component structure
 * \param[in] chBattLevel: Battery level to set (0 to chBattMax)
 * \note  - In charging mode: mark pending update, will sync after current blink cycle
 *        - In idle mode: immediately refresh display
 */
extern
GUI_LV_NONNULL(1)
void gui_lv_custom_battery_set_level(gui_lv_custom_battery_t *ptBattery, uint8_t chBattLevel);


/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_CUSTOM_COMPONENTS_H__ */
