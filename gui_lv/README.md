# GUI_LVGL_Wrapper

## 使用说明

本教程以 Keil 和 LVGL 8.x 为例。选择 RTE 部署或源码部署，按步骤完成配置后调用 `gui_lv_init()`，屏幕上应显示 `GUI ready`。

## 接入前提

开始前请确认：

1. 工程已加入 LVGL 8.x，配置好 `lv_conf.h` 和 LVGL 头文件路径。
2. 已完成屏幕驱动移植，实现 `lv_port_disp_init()`，屏幕可以正常刷新。
3. 已完成所需输入设备移植，实现 `lv_port_indev_init()`。没有输入设备时，将此函数实现为空函数，并在后面的配置步骤中关闭全部输入选项。

移植文件可参考 `port/lv_port_disp_template.c/.h` 和 `port/lv_port_indev_template.c/.h`。使用这些文件时，将文件顶部的 `#if 0` 改为 `#if 1`，补齐硬件接口，并将完成的源文件和头文件加入工程。

将 `lv_port_disp_template.h` 和 `lv_port_indev_template.h` 所在目录加入 Keil 的 **Include Paths**。使用已有移植代码时，也需提供这两个名称的头文件，分别声明 `lv_port_disp_init()` 和 `lv_port_indev_init()`。

## Keil 环境使用

以下两种部署方式选择一种即可。

### 一、RTE 部署

#### 1. 安装并选择组件

安装 GUI_LVGL_Wrapper 的 `.pack` 文件，然后打开 Keil 工程，进入 **Manage Run-Time Environment**，展开 **GUI_LVGL_Wrapper**，按下表选择：

| 组件 | 本教程的选择 |
| --- | --- |
| Core | 勾选 |
| Helper | 勾选 |
| User | 勾选 |
| Scene | 添加 1 个实例 |
| Libraries | 使用分页游标时勾选，本教程可不选 |

点击 **OK** 后，在工程中找到以下文件：

| 文件 | 接下来要配置的内容 |
| --- | --- |
| `gui_lv_conf.h` | 屏幕和输入设备参数 |
| `gui_lv_scene_id.h` | 页面 ID |
| `gui_lvgl.c` | 首页和启动页面 |
| `gui_lv_scene_0.c/.h` | 第一个页面的内容 |

工程中还会出现 `gui_lv_scene_task_0.c/.h`，本教程暂时保持原样。

#### 2. 配置屏幕和输入设备

打开工程中的 `gui_lv_conf.h`，切换到 **Configuration Wizard**，按实际硬件填写：

| 配置项 | 操作 |
| --- | --- |
| Horizontal resolution | 填写屏幕宽度，例如 320 |
| Vertical resolution | 填写屏幕高度，例如 240 |
| Display buffer size in pixels | 填写绘图缓冲区的像素数 |
| Buffering mode | 选择显示移植所用的缓冲模式 |
| Is DMA used for display flushing? | 按显示驱动配置 DMA，没有使用则选择 Disable |
| Is GPU used for filling operations? | 按显示驱动配置 GPU，没有使用则选择 Disable |
| Input device selection | 仅勾选已完成移植的输入设备；没有输入设备则全部取消勾选 |
| Default scene input device | 选择本项目使用的默认输入设备；没有输入设备时可保持原值 |

保存文件。后续若更换屏幕或输入设备，再回到这里修改对应选项。

#### 3. 添加页面 ID

打开 `gui_lv_scene_id.h`，在 `Scene ID Begin` 和 `Scene ID End` 之间加入 `GUI_LV_SCENE_HOME`：

~~~c
typedef enum {
    PLACEHOLDER0,

    /* ---------- Scene ID Begin ---------- */
    GUI_LV_SCENE_HOME,
    /* ---------- Scene ID End ------------ */

    GUI_LV_SCENE_MAX
} gui_lv_scene_id_t;
~~~

#### 4. 填写第一个页面

打开 `gui_lv_scene_0.c`，保留模板中的注册配置区和注释，只将 `.eSceneId` 的值改为：

~~~c
        .eSceneId  = GUI_LV_SCENE_HOME,
~~~

在同一文件的 `__on_scene0_draw()` 中，找到 `User draw code begin` 和 `User draw code end`，在这两处注释之间加入：

~~~c
    lv_obj_t *label = lv_label_create(ptRoot);
    lv_label_set_text(label, "GUI ready");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_pos(label, 20, 20);
~~~

#### 5. 设置首页和启动页面

打开 `gui_lvgl.c`，在 `gui_all_scene_init()` 中，将原有的 `gui_lv_scene_set_home()` 调用改为：

~~~c
    gui_lv_scene_set_home(GUI_LV_SCENE_HOME);
~~~

在同一文件的 `gui_sys_data_init()` 中，找到 `startup` 的定义，改为：

~~~c
    gui_lv_scene_id_t startup = GUI_LV_SCENE_HOME;
~~~

保存文件，继续阅读[初始化](#初始化)。

### 二、源码部署

#### 1. 把 Wrapper 加入工程

将完整的 `gui_lv/` 目录复制到工程目录。在 Keil 的 **Options for Target** 中打开 **C/C++** 页面，将 `gui_lv/` 和 `gui_lv/port/` 加入 **Include Paths**。

在工程树中创建分组，通过 **Add Existing Files to Group** 加入以下源文件：

- `core/` 下的 `.c` 文件。
- `helper/source/` 下的 `.c` 文件。
- `gui_lvgl.c`、`gui_lv_sys_data.c`、`gui_lv_custom_components.c`。
- 已完成移植的显示和输入 `.c` 文件。

完成后，应能在工程树中看到这些源文件。

如果工程中没有 `RTE_Components.h`，在自己的头文件目录中创建此文件，填写以下内容，并将该目录加入 **Include Paths**。已有此文件时保留现有内容。

~~~c
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H
#endif
~~~

#### 2. 检查 Wrapper 配置

打开复制到工程中的 `gui_lv_conf.h`，按前面的[屏幕和输入设备配置表](#2-配置屏幕和输入设备)填写并保存。

#### 3. 添加一个真实场景 ID

打开 `gui_lv_scene_id.h`，在 `Scene ID Begin` 和 `Scene ID End` 之间加入 `GUI_LV_SCENE_HOME`：

~~~c
typedef enum {
    PLACEHOLDER0,

    /* ---------- Scene ID Begin ---------- */
    GUI_LV_SCENE_HOME,
    /* ---------- Scene ID End ------------ */

    GUI_LV_SCENE_MAX
} gui_lv_scene_id_t;
~~~

#### 4. 使用场景模板创建页面

将 `template/gui_lv_scene_template.h` 和 `template/gui_lv_scene_template.c` 复制到应用目录，分别命名为 `gui_lv_scene_home.h` 和 `gui_lv_scene_home.c`。复制后，将模板中的 `<NAME>` 替换为 `HOME`，将 `<name>` 替换为 `home`，并在 `__on_scene_home_draw()` 的 `User draw code begin` 和 `User draw code end` 之间加入首页内容。

下面是替换占位符并加入首页内容后的文件。分区标题和注释保留自场景模板。

`gui_lv_scene_home.h`：

~~~c
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

#ifndef __GUI_LV_SCENE_HOME_H__
#define __GUI_LV_SCENE_HOME_H__

/*============================ INCLUDES ======================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/**
 * @note The scene ID should be defined in your enum as GUI_LV_SCENE_HOME.
 *       Header include is in gui_lv_scene_id.h
 */
extern
void gui_lv_scene_home_init(void);

/*============================ IMPLEMENTATION ================================*/
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_SCENE_HOME_H__ */
~~~

`gui_lv_scene_home.c`：

~~~c
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
#include "gui_lv_scene_home.h"

#ifdef __GUI_LVGL_WRAPPER_CONF__
#   include <gui_lvgl.h>
#else
#   include "gui_lvgl.h"
#endif

#if (__LV_USE_KEYPAD_INDEV__ || __LV_USE_ENCODER_INDEV__)
#   include "gui_lv_scene_task_home.h"
#endif

/*--------------------------*
 * Don't Fix Include Order! *
 *--------------------------*/
#define __GUI_LV_IMPL__
#   include "gui_lv_utils.h"

/*---------------------------*
 * APP layer header includes *
 *---------------------------*/
#if !defined(_WIN64)

#endif

/*============================ MACROS ========================================*/
#define GUI_LV_SCENE_GROUP_NUM      0
#define GUI_LV_SCENE_TIMER_NUM      0

/*============================ TYPES =========================================*/
static struct {
    lv_obj_t   *ptRoot;

    /* User-defined scene members begin --------------------------------------*/


    /*------------------------------------------------------------------------*/

    lv_group_t *ptGroup[GUI_LV_SCENE_GROUP_NUM];
    lv_timer_t *ptTimer[GUI_LV_SCENE_TIMER_NUM];

GUI_LV_PRIVATE(
    struct {
        /* USER: cached app data mirrored for GUI refresh ----------------*/

    } Data;
)

} s_tGUI;

typedef enum {
    NONE_DATA_MASK = 0U,

    ALL_DATA_MASK,
} Data_Mask;


/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
static void __on_scene_home_draw  (lv_obj_t *ptRoot);
static void __on_scene_home_load  (lv_obj_t *ptRoot);
static void __on_scene_home_bind  (void);
static void __on_scene_home_depose(void);

#if GUI_LV_SCENE_TIMER_NUM
static void __on_scene_home_timer0_cb(lv_timer_t *ptTimer);
#endif

static void __scene_home_get_app_data(Data_Mask eData_Mask);
static void __scene_home_set_app_data(Data_Mask eData_Mask);


/*============================ SCENE REGISTRATION ============================*/
/******************************************************************************
 * @note The scene ID should be defined in your enum as GUI_LV_SCENE_HOME.
 *       Header include is in gui_lv_scene_id.h
 ******************************************************************************/
void gui_lv_scene_home_init(void)
{
    static gui_lv_extend_t s_tGUIEX;
    s_tGUIEX.chGroupNum = GUI_LV_SCENE_GROUP_NUM;
    s_tGUIEX.chTimerNum = GUI_LV_SCENE_TIMER_NUM;
    s_tGUIEX.ptGroup    = GUI_LV_SCENE_GROUP_NUM ? s_tGUI.ptGroup
                                                 : NULL;
    s_tGUIEX.ptTimer    = GUI_LV_SCENE_TIMER_NUM ? s_tGUI.ptTimer
                                                 : NULL;

    /* Scene registration config begin ---------------------------------------*/
    static gui_lv_scene_cfg_t s_tCFG = {
        .eSceneId  = GUI_LV_SCENE_HOME,
        .ptExtend  = &s_tGUIEX,
        .pfnDraw   = __on_scene_home_draw,
        .pfnLoad   = __on_scene_home_load,
        .pfnBind   = __on_scene_home_bind,
        .pfnDepose = __on_scene_home_depose,
    };
    /* Scene registration config end -----------------------------------------*/

    gui_lv_scene_register(&s_tCFG);
}


/*============================== SCENE CALLBACKS =============================*/
/******************************************************************************
 * @brief Scene callback responsibilities.                                    *
 *                                                                            *
 * @note  draw   - Create widgets under `ptRoot`.                             *
 * @note  load   - Use app-layer parameters once to fill runtime-only values. *
 * @note  bind   - Register events and input handlers.                        *
 * @note  depose - Clear scene-local state before exit.                       *
 ******************************************************************************/
GUI_LV_NONNULL(1)
static void __on_scene_home_draw(lv_obj_t *ptRoot)
{
    s_tGUI.ptRoot           = ptRoot;
    gui_lv_language_t eLang = gui_lv_get_current_lang();

    /* User draw code begin --------------------------------------------------*/
    lv_obj_t *label = lv_label_create(ptRoot);
    lv_label_set_text(label, "GUI ready");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_pos(label, 20, 20);
    /* User draw code end   --------------------------------------------------*/

#if GUI_LV_SCENE_TIMER_NUM
    GUI_LV_TIMER_SET(s_tGUI.ptTimer[0], __on_scene_home_timer0_cb, 10, NULL);
    GUI_LV_TIMER_ALL_START(s_tGUI.ptTimer, GUI_LV_SCENE_TIMER_NUM);
#endif
}

GUI_LV_NONNULL(1)
static void __on_scene_home_load(lv_obj_t *ptRoot)
{
    GUI_LV_UNUSED(ptRoot);

    /* User load code begin --------------------------------------------------*/


    /* User load code end ----------------------------------------------------*/

}

static void __on_scene_home_bind(void)
{
    GUI_LV_UNUSED(0);

    /* User bind code begin --------------------------------------------------*/


    /* User bind code end ----------------------------------------------------*/

}

static void __on_scene_home_depose(void)
{
    s_tGUI.ptRoot = NULL;

    /* User cleanup code begin -----------------------------------------------*/


    /* User cleanup code end -------------------------------------------------*/
}


/*============================ TIMER TASKS ===================================*/
#if GUI_LV_SCENE_TIMER_NUM
GUI_LV_NONNULL(1)
static void __on_scene_home_timer0_cb(lv_timer_t *ptTimer)
{
    GUI_LV_UNUSED(ptTimer);

    /* USER: periodic visual refresh -----------------------------------------*/

}
#endif


/*============================ DATA INTEGRATION ==============================*/
/**
 * \brief Fetch selected application data into the scene cache.
 *
 * Reads the application-layer data specified by \p eData_Mask and stores
 * it in the scene-local cache \c s_tGUI.Data for rendering or refresh.
 *
 * \param[in] eData_Mask Mask specifying which application data fields to fetch.
 *                       Use NONE_DATA_MASK for no data and ALL_DATA_MASK for
 *                       all supported data fields.
 */
static void __scene_home_get_app_data(Data_Mask eData_Mask)
{
    GUI_LV_UNUSED(eData_Mask);

}

/**
 * \brief Write selected scene-cache data back to application-layer variables.
 *
 * Copies the data fields selected by \p eData_Mask from
 * \c s_tGUI.Data to the corresponding application-layer variables.
 *
 * \param[in] eData_Mask Mask specifying which cached data fields to write back.
 *                       Use NONE_DATA_MASK for no data and ALL_DATA_MASK for
 *                       all supported data fields.
 */
static void __scene_home_set_app_data(Data_Mask eData_Mask)
{
    GUI_LV_UNUSED(eData_Mask);

}


/*============================ END OF FILE ===================================*/
~~~

如果启用了键盘或编码器输入，还需将 `template/gui_lv_scene_task_template.h/.c` 复制为 `gui_lv_scene_task_home.h/.c`，并按需要实现按键处理。将场景源文件（以及使用时的任务源文件）加入 Keil 工程，并将头文件所在目录加入 **Include Paths**。

#### 5. 注册场景并设置启动场景

打开 `gui_lvgl.c`，在 `User scene header includes` 注释下加入：

~~~c
#include "gui_lv_scene_home.h"
~~~

将同一文件中的 `gui_all_scene_init()` 函数改为：

~~~c
static void gui_all_scene_init(void)
{
    gui_lv_scene_manage_init();

    gui_lv_scene_home_init();

    gui_lv_scene_set_home(GUI_LV_SCENE_HOME);
}
~~~

在 `gui_sys_data_init()` 中找到 `startup` 的定义，改为：

~~~c
    gui_lv_scene_id_t startup = GUI_LV_SCENE_HOME;
~~~

保存文件，继续阅读[初始化](#初始化)。

## 初始化

完成所选部署方式的配置后，在应用启动文件中包含 `gui_lvgl.h`，初始化 GUI 时只需调用一次：

~~~c
gui_lv_init();
~~~

应用启动代码中无需另行调用 `lv_init()`。

在 Keil 中编译并下载程序。运行后，屏幕左上方应显示 `GUI ready`。

> **提示：** 无论是否使用本库，都需要提前自行完成 `lv_timer_handler()` 的周期调用和 `lv_hal_tick` 时基接入（按项目配置使用 `lv_tick_inc()` 等接口），并在 GUI 初始化后持续运行。

## 第一次运行检查

先确认以下事项，再开始添加更多界面功能：

- 工程能找到 LVGL、Wrapper 头文件、RTE_Components.h 和 lv_conf.h。
- gui_lv_conf.h 中的分辨率、缓冲和输入选项符合当前工程。
- lv_port_disp_init() 已实现，屏幕可以刷新。
- lv_port_indev_init() 已实现；无输入设备时配置已关闭且函数为空。
- gui_lv_scene_id.h 中有至少一个真实场景 ID。
- RTE 部署已添加 Scene 实例并填写页面内容；源码部署已加入页面源文件和初始化调用。
- 首页设置和 startup 都填写为本教程中的 GUI_LV_SCENE_HOME。
- gui_lv_init() 只调用一次。
- 屏幕上能看到 GUI ready 或场景创建的其他内容。

## 继续添加页面

1. 在 `gui_lv_scene_id.h` 中增加新的页面 ID，例如 `GUI_LV_SCENE_SETTING`。
2. RTE 部署时，在 **Manage Run-Time Environment** 中增加一个 Scene 实例，确认后打开新增的 `gui_lv_scene_1.c`，填写页面 ID 和显示内容。
3. 源码部署时，参考第一个页面创建 `gui_lv_scene_setting.c/.h`，将其中的 `home` 改为 `setting`、`GUI_LV_SCENE_HOME` 改为 `GUI_LV_SCENE_SETTING`，再修改显示内容。把源文件加入工程，在 `gui_lvgl.c` 中包含对应头文件，并在 `gui_all_scene_init()` 的首页设置之前加入 `gui_lv_scene_setting_init();`。
4. 在按钮事件等需要切换页面的位置调用：

~~~c
gui_lv_scene_switch(GUI_LV_SCENE_SETTING);
~~~

重新编译下载后，触发该事件，屏幕应切换到新页面。

## 资源生成

需要添加字体或图片时，再执行以下步骤。下列路径相对于本 README 所在目录。

1. 在 ../gui/gui_resource/assets/font_config.json 和 img_config.json 中配置资源。
2. 在 ../gui/gui_resource/ 目录执行 python tools/resource_gen.py。
3. 将 generated/ 下需要的 .c 文件和 generated/gui_lv_resource.h 加入应用工程。
4. 确认资源生成版本与实际编译使用的 LVGL 版本匹配。

完成后，应能在应用工程中找到所选资源的 `.c` 文件和 `gui_lv_resource.h`。

## 常见问题

### 调用 gui_lv_init() 后屏幕没有内容

对照教程确认 `startup` 和页面的 `.eSceneId` 都填写为 `GUI_LV_SCENE_HOME`，页面绘制位置已加入标签代码。然后检查显示移植是否刷新屏幕。

### gui_lv_scene_set_home() 触发断言

检查 `gui_lv_scene_set_home()` 的参数是否与页面中的 `.eSceneId` 一致。源码部署时，对照教程检查 `gui_all_scene_init()` 中的调用顺序；RTE 部署时，确认已添加对应的 Scene 实例。

### 找不到 lv_port_disp_init() 或 lv_port_indev_init()

应用工程尚未加入相应移植实现，或者缺少函数声明。请完成 port/ 模板并确保源文件、头文件加入构建。

### 输入没有反应

确认 gui_lv_conf.h 启用了正确的输入设备，移植代码注册了对应 LVGL 输入设备，并且键盘或编码器场景配置了需要的 group。

### 找不到页面文件或页面初始化函数

RTE 部署时，返回 **Manage Run-Time Environment**，检查 Scene 实例数量并确认。源码部署时，检查页面 `.c` 文件是否已加入工程、页面头文件目录是否已加入 **Include Paths**，以及初始化函数名是否与页面文件中的定义一致。

## 许可证

本项目采用 Apache-2.0 License，详见 [LICENSE](LICENSE)。
