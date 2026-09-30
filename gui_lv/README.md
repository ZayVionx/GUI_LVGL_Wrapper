# GUI_LVGL_Wrapper

GUI_LVGL_Wrapper 是一套面向 LVGL 8.x 的嵌入式界面封装层，用于把
"显示驱动初始化、场景切换、页面回退、样式复用、输入焦点管理" 这类
高重复度基础工作从业务界面代码中剥离出来。

它不是对 LVGL 的替代，而是在 LVGL 之上补齐一层更适合 MCU 工程协作的
界面组织方式。应用层只需要围绕场景、页面、控件和事件编写业务逻辑，
无需在每个页面反复手写相同的初始化、切换和回收样板。

## 1. 快速了解

GUI_LVGL_Wrapper 运行在 LVGL 8.x 之上，提供场景、分页游标、焦点、样式和常用
控件的封装。接入时只需要准备 LVGL 和显示/输入移植，注册业务场景，然后
调用 `gui_lv_init()`。

适合需要多场景、多级页面、键盘或编码器输入的 MCU 图形界面项目。

## 2. 目录结构

`gui_lv/` 目录中，与接入最相关的内容如下：

```text
gui_lv/
├─ core/                     # 场景管理与基础控件封装
├─ helper/                   # style / data / input 等辅助模块
├─ platform/                 # 编译器与平台适配层
├─ port/                     # 显示和输入设备移植模板
├─ template/                 # Pack/RTE 侧场景模板
├─ gui_lvgl.h/.c             # 组件主入口
├─ gui_lv_sys_data.h/.c      # 系统参数接口
├─ gui_lv_scene_id.h         # 场景 ID 定义
├─ gui_lv_conf.h             # 配置项
├─ gui_lv_utils.h            # 工具宏
└─ ZJY.GUI_LVGL_Wrapper.pdsc # CMSIS-Pack 描述文件
```

## 3. 快速上手

如果你是第一次接入该库，建议按下面顺序完成。

### 3.1 准备依赖

需要先准备以下基础条件：

- LVGL 8.x 已集成到工程
- 显示驱动已完成移植
- 输入设备驱动已完成移植
- `lv_tick_inc()` 与 `lv_timer_handler()` 调度正常

如果尚未完成移植，可参考：

- `port/lv_port_disp_template.c/.h`
- `port/lv_port_indev_template.c/.h`

### 3.2 将源码加入工程

直接源码集成时，建议至少将以下内容纳入编译：

- `core/`
- `helper/`
- `gui_lvgl.c/.h`
- `gui_lv_sys_data.c/.h`
- `gui_lv_scene_id.h`

根据工程实际情况，再补充字体、资源、自定义控件和移植文件。

### 3.3 定义场景 ID

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
```

### 3.4 创建场景文件

非 RTE 模式下，建议从以下模板复制：

- `gui_lv/template/gui_lv_scene_template.c`
- `gui_lv/template/gui_lv_scene_template.h`

如果场景还需要任务处理逻辑，可一并参考：

- `gui_lv/template/gui_lv_scene_task_template.c`
- `gui_lv/template/gui_lv_scene_task_template.h`

典型场景实现步骤如下：

1. 定义场景私有结构体，保存 root、控件指针、group、timer
2. 实现 `__on_scene_xxx_draw()` 创建控件
3. 实现 `__on_scene_xxx_load()` 刷新页面展示数据
4. 实现 `__on_scene_xxx_bind()` 绑定事件与输入逻辑
5. 实现 `__on_scene_xxx_depose()` 释放资源
6. 在 `gui_lv_scene_xxx_init()` 中构造 `gui_lv_scene_cfg_t` 并注册

示例：

```c
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
```

### 3.5 配置系统参数与开机场景

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

#if defined(__RTE_Acceleration_GUI_LVGL_SCENE__)
    __RTE_Acceleration_GUI_LV_ALL_SCENE_INIT();
#else
    /* 使用 RTE 部署时，由 RTE 自动完成场景初始化，无需在此处手动添加。
     * 非 RTE 模式下，请按以下方式手动添加场景初始化。
     * 两种初始化方式只能二选一，不能同时使用。
     */
    gui_lv_scene_home_init();
    gui_lv_scene_setting_init();
    gui_lv_scene_about_init();
#endif

    gui_lv_scene_set_home(GUI_LV_SCENE_HOME);
}
```

### 3.6 在系统启动时调用初始化入口

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

## 4. 常用功能

### 4.1 场景注册与切换

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

### 4.2 分页游标

需要在多个条目之间分页时，启用 `GUI_LVGL_Wrapper::Libraries` 组件，并使用 `gui_lv_page_cursor_t`。

常用接口包括：

- `gui_lv_page_cursor_init()`：初始化游标
- `gui_lv_page_cursor_update()`：更新条目数量
- `gui_lv_page_cursor_prev_item()` / `gui_lv_page_cursor_next_item()`：切换条目
- `gui_lv_page_cursor_prev_page()` / `gui_lv_page_cursor_next_page()`：切换页面
- `gui_lv_page_cursor_goto()`：跳转到指定页面

### 4.3 Group 与焦点恢复

如果场景使用键盘或编码器导航，建议在场景扩展信息中维护 group 数组。

`gui_lv_extend_t` 主要包含：

- `chGroupNum`
- `chTimerNum`
- `ptGroup`
- `ptTimer`

当场景切换后希望恢复之前的焦点位置，可以使用：

- `gui_lv_scene_focus_restore_enable(scene_id)`
- `gui_lv_scene_focus_restore_disable(scene_id)`\r\n- `gui_lv_scene_is_focus_restore(scene_id)`

辅助接口还包括：

- `gui_lv_group_focus_nav()`
- `gui_lv_group_get_focus_index()`
- `gui_lv_group_get_index_obj()`

这组接口适合栅格菜单、列表选择和参数项焦点跳转。

### 4.4 常用控件快捷创建

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

### 4.5 样式系统使用方式

样式辅助接口位于 `helper/style`，常用流程如下：

1. 使用 `gui_lv_style_config_t` 描述样式
2. 调用 `gui_lv_style_init()` 初始化样式
3. 调用 `gui_lv_style_apply()` 应用到控件

常用辅助宏包括：

- `GUI_LV_STYLE_APPLY_MAIN()`
- `GUI_LV_STYLE_APPLY_FOCUSED()`
- `GUI_LV_STYLE_APPLY_PRESSED()`
- `GUI_LV_STYLE_APPLY_CHECKED()`
- `GUI_LV_STYLE_APPLY_INDICATOR()`

### 4.6 输入与数据辅助接口

`helper/input` 和 `helper/data` 主要面向设置类、调参类界面。

典型用途包括：

- 编码器或键盘在网格中的方向移动
- 数值增加、减少、限幅和回环
- 参数保存到 Flash 的占位接口
- 参数恢复默认值的占位接口

当界面包含多个参数项、需要统一的加减逻辑时，优先复用 helper 接口而不
是让每个页面各自维护一份参数处理代码。

### 4.7 定时器与场景扩展数据

如果一个场景需要周期刷新，例如：

- 实时状态显示
- 动画节拍更新
- 传感器值轮询展示

可以在场景私有结构中预留 `ptTimer[]`，并在 `ptExtend` 中传入 timer 数组。
模板文件已经给出了标准写法。这样可以让定时器和场景生命周期绑定，避免
定时器游离于场景外部。

## 5. 资源生成与使用

资源工具位于 `gui/gui_resource/`。典型流程如下：

1. 在 `assets/font_config.json` 中配置字体
2. 在 `assets/img_config.json` 中配置图片
3. 运行 `tools/resource_gen.py` 或 `tools/generate_all.py`
4. 在应用代码中引用 `generated/gui_lv_resource.h` 中声明的资源符号

如果工程采用 Pack 集成，通常可以直接使用 `generated/` 下的预生成资源，
无需在目标工程中再运行 Python 脚本。

## 6. 集成方式

### 6.1 直接源码集成

按“快速上手”完成源码接入即可。源码模式下，场景文件由工程自行维护，场景初始化在 `gui_all_scene_init()` 中手动调用。

### 6.2 CMSIS-Pack / RTE 集成

适用于通过 Pack 管理软件组件的工程。推荐启用顺序如下：

1. `GUI_LVGL_Wrapper::Core`
2. `GUI_LVGL_Wrapper::Helper`
3. `GUI_LVGL_Wrapper::User`
4. `GUI_LVGL_Wrapper::Scene`\r\n5. `GUI_LVGL_Wrapper::Libraries`（使用分页游标时启用）

需要注意：

- `Scene` 是可多实例组件
- 启用后会生成对应场景实例文件
- [gui_lvgl.c](gui_lvgl.c) 会在 RTE 条件编译下通过
  `template/gui_lv_scene_include.h` 自动纳入场景初始化
- `template/` 下文件属于模板或 Pack 使用场景，不应直接原样参与普通源码编译

## 7. 配置与移植检查项

接入完成后，至少检查以下项目：

- `lv_port_disp_init()` 已实现且显示正常刷新
- `lv_port_indev_init()` 已实现且输入事件可达
- `lv_tick_inc()` 和 `lv_timer_handler()` 调度正常
- [gui_lv_scene_id.h](gui_lv_scene_id.h) 已补充有效场景和页面 ID
- `gui_lv_set_startup_scene_id()` 设置的场景已注册
- 如果启用 group，输入设备与 group 绑定正确
- 如果启用页面机制，页面已追加到正确场景
- 定时器、资源和字体对象的生命周期与场景生命周期一致

## 8. 常见问题

### Q1：调用 `gui_lv_init()` 后没有显示任何内容

优先检查：

- 启动场景是否已经注册
- `gui_lv_set_startup_scene_id()` 设置是否正确
- 显示驱动是否真正刷新到屏幕
- `lv_timer_handler()` 是否被周期调用

### Q2：场景能切换，但焦点位置不对

优先检查：

- 场景是否正确维护了 `ptGroup`
- group 数量是否与 `chGroupNum` 一致
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

## 9. 设计约定

为了让这套封装长期可维护，建议在项目中遵循以下约定：

1. 一个场景文件只负责一个一级界面
2. draw 只做界面创建，bind 只做事件绑定，depose 只做资源回收
3. 样式尽量集中定义，不要在每个控件上散写同类属性
4. 页面只处理当前场景内部的二级流程，跨一级业务直接走场景切换
5. 所有启动逻辑统一回收进 [gui_lvgl.c](gui_lvgl.c)，避免初始化分散

## 10. License

本项目采用 Apache-2.0 License，详见 `LICENSE`。
