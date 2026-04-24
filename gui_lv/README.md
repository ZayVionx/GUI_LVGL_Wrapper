# GUI_LVGL_Wrapper

GUI_LVGL_Wrapper 是一套面向 LVGL 8.x 的嵌入式界面封装层，用于把
"显示驱动初始化、场景切换、页面回退、样式复用、输入焦点管理" 这类
高重复度基础工作从业务界面代码中剥离出来。

它不是对 LVGL 的替代，而是在 LVGL 之上补齐一层更适合 MCU 工程协作的
界面组织方式。应用层只需要围绕场景、页面、控件和事件编写业务逻辑，
无需在每个页面反复手写相同的初始化、切换和回收样板。

## 1. 设计意义

在 MCU 图形界面项目中，随着页面数量增加，代码通常会快速出现以下问题：

- 界面创建逻辑散落在多个文件，初始化顺序不统一
- 页面切换、返回和历史记录依赖人工维护，容易遗漏
- 多输入设备或 group 焦点恢复逻辑分散，交互一致性差
- 控件样式重复定义，界面维护成本持续上升
- 同一套 GUI 需要同时适配源码集成和 Pack/RTE 集成时，工程边界不清晰

GUI_LVGL_Wrapper 的设计目标，就是把这些通用问题沉淀为统一机制，让业务
代码专注于两件事：

1. 这个场景要显示什么
2. 这个场景如何响应用户输入和业务状态变化

## 2. 核心价值

### 2.1 统一初始化入口

库通过 `gui_lv_init()` 提供单入口初始化流程，把 LVGL 核心、显示移植、
输入移植、系统参数、样式初始化和场景注册串联起来，降低集成时的顺序
错误。

### 2.2 统一场景生命周期

场景以配置结构体注册，切换时按统一流程执行 draw、load、bind、depose，
减少业务层直接操作 `lv_scr_load()` 或手动销毁对象带来的不一致问题。

### 2.3 统一页面层级管理

除了一级场景，库还支持将二级页面挂接到指定场景，形成更清晰的
"场景-页面" 结构，适合设置页、弹出子流程页、参数编辑页等典型业务。

### 2.4 统一输入焦点恢复

库内部支持 group 焦点索引保存与恢复，适合键盘、编码器等导航型输入设备。
这使得页面返回后能够保持用户上次停留位置，改善交互连续性。

### 2.5 统一样式与控件封装

库对常见 LVGL 控件创建流程进行了轻量封装，并提供结构化样式配置接口，
可以在保证灵活性的前提下减少重复样板代码。

### 2.6 兼容两类工程组织方式

同一套代码同时支持：

- 直接源码集成
- CMSIS-Pack / RTE 集成

这对于需要在示例工程、量产工程和 Pack 分发之间复用同一套 GUI 框架的项
目尤其有价值。

## 3. 适用场景

该库适合以下项目：

- 基于 LVGL 8.x 的 MCU 图形界面项目
- 存在多场景、多级页面或菜单流程的设备
- 需要键盘、编码器或 group 焦点管理的交互型 UI
- 希望把 GUI 基础设施与业务界面代码解耦的项目
- 同时需要源码交付和 Pack 交付能力的组件化项目

以下场景不建议把它当成必须依赖：

- 只有单页面、无切换逻辑的极小型 UI
- 需要完全自定义对象生命周期、不希望引入场景管理约束的工程

## 4. 目录结构与职责

`gui_lv/` 目录主要由以下部分组成：

```text
gui_lv/
├─ core/                     # 场景管理与基础控件封装
├─ helper/                   # style / data / input 等辅助模块
├─ platform/                 # 编译器与平台适配层
├─ port/                     # 显示和输入设备移植模板
├─ template/                 # Pack/RTE 侧场景模板
├─ gui_lvgl.h/.c             # 组件主入口
├─ gui_lv_sys_data.h/.c      # 系统参数接口
├─ gui_lv_scene_id.h         # 场景与页面 ID 定义
├─ gui_lv_conf.h             # 配置项
├─ gui_lv_utils.h            # 工具宏
└─ ZJY.GUI_LVGL_Wrapper.pdsc # CMSIS-Pack 描述文件
```

职责划分如下：

- `gui_lvgl.*`：总初始化入口和用户配置入口
- `core/gui_lv_scene_manage.*`：场景、页面、回退与焦点恢复管理
- `core/gui_lv_common.*`：常用控件快捷创建接口
- `helper/include` 与 `helper/source`：样式、输入、数据辅助
- `port/`：显示和输入驱动移植模板
- `template/`：RTE 场景模板

## 5. 软件结构

### 5.1 初始化主线

当前库的初始化主线如下：

1. 调用 `lv_init()`
2. 调用 `lv_port_disp_init()`
3. 调用 `lv_port_indev_init()`
4. 调用 `__gui_lv_helper_init()`
5. 调用 `gui_sys_data_init()`
6. 调用 `gui_common_style_init()`
7. 调用 `gui_all_scene_init()`
8. 调用 `gui_lv_scene_switch(gui_lv_get_startup_scene_id())`
9. 调用 `lv_timer_handler()` 触发首次界面处理

对于应用层来说，真正需要改动的入口主要集中在 [gui_lvgl.c](gui_lvgl.c)
中的用户代码区域。

### 5.2 场景模型

场景是一级界面。一个场景通常对应一块完整屏幕，例如：

- 主界面
- 设置界面
- 关于界面
- 历史记录界面

每个场景通过 `gui_lv_scene_cfg_t` 描述，典型回调包括：

- `pfnDraw`：创建对象、布局和静态控件
- `pfnLoad`：加载资源或刷新页面初始内容
- `pfnBind`：绑定事件或业务数据
- `pfnDepose`：释放场景持有资源

### 5.3 页面模型

页面是挂接在场景内部的二级流程单元，适合以下用途：

- 设置场景下的各个参数子页
- 列表详情页
- 向导型流程的中间步骤页

页面通过 `gui_lv_page_cfg_t` 定义，并使用
`gui_lv_page_append_to_scene()` 挂到指定场景下。

## 6. 快速上手

如果你是第一次接入该库，建议按下面顺序完成。

### 6.1 准备依赖

需要先准备以下基础条件：

- LVGL 8.x 已集成到工程
- 显示驱动已完成移植
- 输入设备驱动已完成移植
- `lv_tick_inc()` 与 `lv_timer_handler()` 调度正常

如果尚未完成移植，可参考：

- `port/lv_port_disp_template.c/.h`
- `port/lv_port_indev_template.c/.h`

### 6.2 将源码加入工程

直接源码集成时，建议至少将以下内容纳入编译：

- `core/`
- `helper/`
- `gui_lvgl.c/.h`
- `gui_lv_sys_data.c/.h`
- `gui_lv_scene_id.h`

根据工程实际情况，再补充字体、资源、自定义控件和移植文件。

### 6.3 定义场景与页面 ID

在 [gui_lv_scene_id.h](gui_lv_scene_id.h) 中补充业务枚举。建议保留文件中
已有占位和边界标记，只在注释区间内插入自己的 ID。

示例：

```c
typedef enum {
    PLACEHOLDER0,

    GUI_LV_SCENE_HOME,
    GUI_LV_SCENE_SETTING,
    GUI_LV_SCENE_ABOUT,

    GUI_LV_SCENE_MAX
} gui_lv_scene_id_t;

typedef enum {
    PLACEHOLDER1,

    GUI_LV_PAGE_SETTING_DISPLAY,
    GUI_LV_PAGE_SETTING_AUDIO,

    GUI_LV_PAGE_MAX
} gui_lv_page_id_t;
```

### 6.4 创建场景文件

非 RTE 模式下，建议从以下模板复制：

- `gui/gui_app/gui_lv_scene_template.c`
- `gui/gui_app/gui_lv_scene_template.h`

如果场景还需要任务处理逻辑，可一并参考：

- `gui/gui_task/gui_lv_scene_task_template.c`
- `gui/gui_task/gui_lv_scene_task_template.h`

典型场景实现步骤如下：

1. 定义场景私有结构体，保存 root、控件指针、group、timer
2. 实现 `__on_scene_xxx_draw()` 创建控件
3. 实现 `__on_scene_xxx_load()` 刷新页面展示数据
4. 实现 `__on_scene_xxx_bind()` 绑定事件与输入逻辑
5. 实现 `__on_scene_xxx_depose()` 释放资源
6. 在 `gui_lv_scene_xxx_init()` 中构造 `gui_lv_scene_cfg_t` 并注册

示例：

```c
static void __on_scene_home_draw(lv_obj_t *ptRoot)
{
    gui_lv_label_init(ptRoot,
                      20,
                      20,
                      "Home",
                      &lv_font_montserrat_20,
                      lv_color_white());
}

static void __on_scene_home_load(lv_obj_t *ptRoot)
{
    GUI_LV_UNUSED(ptRoot);
}

static void __on_scene_home_bind(void)
{
}

static void __on_scene_home_depose(void)
{
}

void gui_lv_scene_home_init(void)
{
    static gui_lv_scene_cfg_t s_tCFG = {
        .eSceneId  = GUI_LV_SCENE_HOME,
        .ptExtend  = NULL,
        .pfnDraw   = __on_scene_home_draw,
        .pfnLoad   = __on_scene_home_load,
        .pfnBind   = __on_scene_home_bind,
        .pfnDepose = __on_scene_home_depose,
    };

    gui_lv_scene_register(&s_tCFG);
}
```

### 6.5 配置系统参数与开机场景

在 [gui_lvgl.c](gui_lvgl.c) 中，用户通常需要修改两个静态函数：

- `gui_sys_data_init()`
- `gui_all_scene_init()`

其中 `gui_sys_data_init()` 用来配置语言、开机场景和设备参数；
`gui_all_scene_init()` 用来注册全部场景。

示例：

```c
static void gui_sys_data_init(void)
{
    gui_lv_set_lang(GUI_LV_LANGUAGE_EN);
    gui_lv_set_startup_scene_id(GUI_LV_SCENE_HOME);
}

static void gui_all_scene_init(void)
{
    gui_lv_scene_manage_init();

    gui_lv_scene_home_init();
    gui_lv_scene_setting_init();
    gui_lv_scene_about_init();

    gui_lv_scene_set_home(GUI_LV_SCENE_HOME);
}
```

### 6.6 在系统启动时调用初始化入口

系统启动完成后调用一次：

```c
gui_lv_init();
```

运行阶段持续周期调用：

```c
lv_timer_handler();
```

调用约束如下：

- `gui_lv_init()` 只能初始化一次
- 启动场景必须已经注册
- LVGL tick 必须正常递增
- 显示和输入移植必须可用

## 7. 详细使用方法

### 7.1 场景注册与切换

场景管理核心接口位于 `core/gui_lv_scene_manage.h`。最常用的接口包括：

| 接口 | 说明 |
| --- | --- |
| `gui_lv_scene_register()` | 注册场景 |
| `gui_lv_scene_switch()` | 切换到指定场景 |
| `gui_lv_scene_switch_with_anim()` | 带动画切换场景 |
| `gui_lv_scene_back()` | 返回上一个场景 |
| `gui_lv_scene_back_with_anim()` | 带动画返回 |
| `gui_lv_scene_switch_to_home()` | 回到 home 场景 |
| `gui_lv_get_scene_id()` | 获取当前场景 ID |

一个典型切换流程通常是：

1. 在按键或事件回调中获取用户意图
2. 调用 `gui_lv_scene_switch()` 或动画版本接口
3. 目标场景按照统一生命周期重新构建

示例：

```c
static void __event_goto_setting(lv_event_t *ptEvent)
{
    GUI_LV_UNUSED(ptEvent);
    gui_lv_scene_switch(GUI_LV_SCENE_SETTING);
}
```

### 7.2 页面挂接与回退

当一个场景内部需要多个二级界面时，可以为该场景追加页面。

常用接口包括：

| 接口 | 说明 |
| --- | --- |
| `gui_lv_page_append_to_scene()` | 将页面挂接到指定场景 |
| `gui_lv_page_switch()` | 切换到目标页面 |
| `gui_lv_page_switch_with_anim()` | 带动画切换页面 |
| `gui_lv_page_back()` | 返回上一页面 |
| `gui_lv_page_back_with_anim()` | 带动画返回页面 |
| `gui_lv_get_page_id()` | 获取当前页面 ID |

示例：

```c
void gui_lv_page_setting_display_init(void)
{
    static gui_lv_page_cfg_t s_tCFG = {
        .ePageId   = GUI_LV_PAGE_SETTING_DISPLAY,
        .ptExtend  = NULL,
        .pfnDraw   = __on_page_setting_display_draw,
        .pfnLoad   = __on_page_setting_display_load,
        .pfnBind   = __on_page_setting_display_bind,
        .pfnDepose = __on_page_setting_display_depose,
    };

    gui_lv_page_append_to_scene(&s_tCFG, GUI_LV_SCENE_SETTING);
}
```

进入页面：

```c
gui_lv_page_switch(GUI_LV_PAGE_SETTING_DISPLAY);
```

返回页面：

```c
gui_lv_page_back();
```

### 7.3 Group 与焦点恢复

如果场景使用键盘或编码器导航，建议在场景扩展信息中维护 group 数组。

`gui_lv_extend_t` 主要包含：

- `u8GroupNum`
- `u8TimerNum`
- `ptGroup`
- `ptTimer`

当场景切换后希望恢复之前的焦点位置，可以使用：

- `gui_lv_scene_focus_restore_enabled()`
- `gui_lv_scene_focus_restore_disable()`

辅助接口还包括：

- `gui_lv_group_focus_nav()`
- `gui_lv_group_get_focus_index()`
- `gui_lv_group_get_index_obj()`

这组接口适合栅格菜单、列表选择和参数项焦点跳转。

### 7.4 常用控件快捷创建

为了减少重复样板，库在 `core/gui_lv_common.*` 中提供了一组轻量接口：

- `gui_lv_container_init()`
- `gui_lv_img_init()`
- `gui_lv_label_init()`
- `gui_lv_btn_init()`
- `gui_lv_bar_init()`
- `gui_lv_spinbox_init()`
- `gui_lv_spinbox_param_init()`
- `gui_lv_timer_init()`

这些接口适合在 `pfnDraw` 阶段快速创建标准控件。对于复杂自定义对象，
仍然可以直接调用原生 LVGL API。

### 7.5 样式系统使用方式

样式辅助接口位于 `helper/style`。其核心思想是：

1. 用 `gui_lv_style_config_t` 描述样式
2. 用 `gui_lv_style_init()` 将配置转成 LVGL 样式对象
3. 用 `gui_lv_style_apply()` 应用到目标对象

示例：

```c
static gui_lv_style_t s_tTitleStyle = {
    .config = {
        GUI_LV_STYLE_TEXT_FONT(&lv_font_montserrat_20),
        GUI_LV_STYLE_TEXT_COLOR(lv_color_white()),
        GUI_LV_STYLE_BG_OPA(LV_OPA_TRANSP)
    },
};

static void __style_init(void)
{
    gui_lv_style_init(&s_tTitleStyle);
}

static void __draw_title(lv_obj_t *ptParent)
{
    lv_obj_t *ptLabel = gui_lv_label_init(ptParent,
                                          12,
                                          8,
                                          "Setting",
                                          &lv_font_montserrat_20,
                                          lv_color_white());

    gui_lv_style_apply(ptLabel,
                       &s_tTitleStyle,
                       LV_PART_MAIN);
}
```

为了减少样板，也可以使用辅助宏：

- `GUI_LV_STYLE_APPLY_MAIN()`
- `GUI_LV_STYLE_APPLY_FOCUSED()`
- `GUI_LV_STYLE_APPLY_PRESSED()`
- `GUI_LV_STYLE_APPLY_CHECKED()`
- `GUI_LV_STYLE_APPLY_INDICATOR()`

### 7.6 输入与数据辅助接口

`helper/input` 和 `helper/data` 主要面向设置类、调参类界面。

典型用途包括：

- 编码器或键盘在网格中的方向移动
- 数值增加、减少、限幅和回环
- 参数保存到 Flash 的占位接口
- 参数恢复默认值的占位接口

当界面包含多个参数项、需要统一的加减逻辑时，优先复用 helper 接口而不
是让每个页面各自维护一份参数处理代码。

### 7.7 定时器与场景扩展数据

如果一个场景需要周期刷新，例如：

- 实时状态显示
- 动画节拍更新
- 传感器值轮询展示

可以在场景私有结构中预留 `ptTimer[]`，并在 `ptExtend` 中传入 timer 数组。
模板文件已经给出了标准写法。这样可以让定时器和场景生命周期绑定，避免
定时器游离于场景外部。

## 8. 资源生成与使用

资源工具位于 `gui/gui_resource/`。典型流程如下：

1. 在 `assets/font_config.json` 中配置字体
2. 在 `assets/img_config.json` 中配置图片
3. 运行 `tools/resource_gen.py` 或 `tools/generate_all.py`
4. 在应用代码中引用 `generated/gui_lv_resource.h` 中声明的资源符号

如果工程采用 Pack 集成，通常可以直接使用 `generated/` 下的预生成资源，
无需在目标工程中再运行 Python 脚本。

## 9. 两种集成方式

### 9.1 直接源码集成

适用于普通裸机工程或自定义工程。推荐流程如下：

1. 引入 LVGL 8.x 并完成显示、输入移植
2. 将 `gui_lv/` 相关源码加入编译
3. 在 [gui_lv_scene_id.h](gui_lv_scene_id.h) 中定义场景和页面 ID
4. 从 `gui/gui_app/` 与 `gui/gui_task/` 模板创建场景文件
5. 在 [gui_lvgl.c](gui_lvgl.c) 中填写系统参数和场景注册逻辑
6. 启动时调用 `gui_lv_init()`
7. 周期调度 `lv_timer_handler()`

### 9.2 CMSIS-Pack / RTE 集成

适用于通过 Pack 管理软件组件的工程。推荐启用顺序如下：

1. `GUI_LVGL_Wrapper::Core`
2. `GUI_LVGL_Wrapper::Helper`
3. `GUI_LVGL_Wrapper::USER`
4. `GUI_LVGL_Wrapper::Scene`

需要注意：

- `Scene` 是可多实例组件
- 启用后会生成对应场景实例文件
- [gui_lvgl.c](gui_lvgl.c) 会在 RTE 条件编译下通过
  `template/gui_lv_scene_include.h` 自动纳入场景初始化
- `template/` 下文件属于模板或 Pack 使用场景，不应直接原样参与普通源码编译

## 10. 配置与移植检查项

接入完成后，至少检查以下项目：

- `lv_port_disp_init()` 已实现且显示正常刷新
- `lv_port_indev_init()` 已实现且输入事件可达
- `lv_tick_inc()` 和 `lv_timer_handler()` 调度正常
- [gui_lv_scene_id.h](gui_lv_scene_id.h) 已补充有效场景和页面 ID
- `gui_lv_set_startup_scene_id()` 设置的场景已注册
- 如果启用 group，输入设备与 group 绑定正确
- 如果启用页面机制，页面已追加到正确场景
- 定时器、资源和字体对象的生命周期与场景生命周期一致

## 11. 常见问题

### Q1：调用 `gui_lv_init()` 后没有显示任何内容

优先检查：

- 启动场景是否已经注册
- `gui_lv_set_startup_scene_id()` 设置是否正确
- 显示驱动是否真正刷新到屏幕
- `lv_timer_handler()` 是否被周期调用

### Q2：场景能切换，但焦点位置不对

优先检查：

- 场景是否正确维护了 `ptGroup`
- group 数量是否与 `u8GroupNum` 一致
- 是否在需要的场景上启用了焦点恢复

### Q3：页面切换没有效果

优先检查：

- 页面是否已通过 `gui_lv_page_append_to_scene()` 挂接到目标场景
- 页面 ID 是否有效
- 当前业务流程是否已经位于对应场景内

### Q4：模板文件出现 `<name>` 或 `<NAME>` 未替换

这说明你正在直接编译模板文件。模板文件只能：

- 作为普通工程的复制起点手动替换
- 作为 Pack/RTE 的实例模板参与生成

不能原样加入最终工程编译。

## 12. 设计建议

为了让这套封装长期可维护，建议在项目中遵循以下约定：

1. 一个场景文件只负责一个一级界面
2. draw 只做界面创建，bind 只做事件绑定，depose 只做资源回收
3. 样式尽量集中定义，不要在每个控件上散写同类属性
4. 页面只处理当前场景内部的二级流程，跨一级业务直接走场景切换
5. 所有启动逻辑统一回收进 [gui_lvgl.c](gui_lvgl.c)，避免初始化分散

## 13. License

本项目采用 Apache-2.0 License，详见 `LICENSE`。
