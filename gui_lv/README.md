# GUI_LVGL_Wrapper

GUI_LVGL_Wrapper 是一套面向 LVGL 8.x 的嵌入式 GUI 封装组件，用于
统一界面初始化、场景切换、页面栈管理、输入焦点恢复及常用控件创建
流程。该组件将应用层界面逻辑与底层样板代码解耦，使业务代码能够围绕
界面结构与交互行为展开，并保持一致的组织方式。

## 1. 概述

本组件提供以下核心能力：

- 单入口初始化接口：`gui_lv_init()`
- 场景注册、场景切换与场景回退
- 场景内页面压栈与页面回退
- 多 group 焦点索引保存与恢复
- 常用 LVGL 控件快速创建接口
- 结构化样式描述与样式应用接口
- 输入导航与参数调节辅助接口
- 同时支持直接源码集成与 CMSIS-Pack / RTE 集成

本组件适用于以下类型的工程：

- 基于 LVGL 8.x 的 MCU 图形界面工程
- 具有多场景、多页面层级的应用
- 需要统一界面模块边界与初始化流程的项目
- 需要同时兼容源码集成与 Pack 分发的工程

## 2. 目录结构

`gui_lv/` 目录下主要内容如下：

```text
gui_lv/
├─ core/                     # 场景管理与基础控件封装
│  ├─ gui_lv_common.h/.c
│  └─ gui_lv_scene_manage.h/.c
├─ helper/                   # 辅助模块：style / data / input
│  ├─ include/
│  └─ source/
├─ platform/                 # 编译器/架构相关基础宏移植层
│  └─ gui_lv_user_arch_port.h
├─ port/                     # 显示与输入设备移植模板
│  ├─ lv_port_disp_template.h/.c
│  └─ lv_port_indev_template.h/.c
├─ template/                 # 场景模板与 RTE 场景实例模板
├─ gui_lvgl.h/.c             # 组件主入口
├─ gui_lv_sys_data.h/.c      # 系统参数接口
├─ gui_scene_id.h            # 场景与页面 ID 定义
├─ gui_lv_conf.h             # 组件配置项
├─ gui_lv_utils.h            # 常用宏与快捷工具
└─ ZJY.GUI_LVGL_Wrapper.pdsc # CMSIS-Pack 描述文件
```

各目录职责如下：

- `core/`：提供场景管理与基础控件封装
- `helper/`：提供样式、输入与数据辅助功能
- `platform/`：提供编译器/架构相关基础宏移植层
- `port/`：提供显示与输入设备移植模板
- `template/`：提供场景模板与 RTE 实例模板

## 3. 软件结构

### 3.1 主入口模块

`gui_lvgl.h/.c` 负责提供组件初始化入口，并串联以下阶段：

- 系统参数初始化
- 公共样式初始化
- 全部场景注册
- 开机场景切换

### 3.2 场景管理模块

`core/gui_lv_scene_manage.h/.c` 负责管理场景与页面生命周期，主要包
括：

- 场景注册表管理
- 当前场景切换
- 历史场景回退
- 场景内页面压栈与页面回退
- group 焦点索引保存与恢复

### 3.3 基础控件模块

`core/gui_lv_common.h/.c` 对常用 LVGL 控件创建过程进行封装，用于减
少界面初始化代码中的重复样板。

### 3.4 Helper 模块

`helper/` 目录用于提供辅助逻辑：

- `helper/style`：统一样式描述与样式应用
- `helper/input`：导航控制、加减调节与回环处理
- `helper/data`：参数范围控制与持久化接口占位

## 4. 集成方式

本组件支持两种集成方式。

### 4.1 直接源码集成

适用于未使用 RTE 的普通工程。集成步骤如下：

1. 引入 LVGL 8.x，并完成显示与输入设备移植。
2. 将 `gui_lv/` 下所需源码加入工程编译。
3. 在 `gui_scene_id.h` 中定义业务场景与页面 ID。
4. 基于 `template/gui_scene_template.c/.h` 创建用户场景文件。
5. 在 `gui_lvgl.c` 的用户代码区完成系统参数、公共样式和场景注册。
6. 系统启动后调用 `gui_lv_init()`，运行阶段周期调用
   `lv_timer_handler()`。

建议至少纳入以下文件或目录：

- `core/`
- `helper/`
- `gui_lvgl.c/.h`
- `gui_lv_sys_data.c/.h`
- `gui_scene_id.h`

### 4.2 CMSIS-Pack / RTE 集成

适用于采用 Pack 方式管理软件组件的工程。在 RTE 中按以下顺序启用
组件：

1. `GUI_LVGL_Wrapper::Core`
2. `GUI_LVGL_Wrapper::Helper`
3. `GUI_LVGL_Wrapper::USER`
4. `GUI_LVGL_Wrapper::Scene`

相关说明如下：

- `Scene` 为可多实例组件，最大实例数为 20
- 启用 `Scene` 后会生成对应的 `gui_scene_%Instance%.c/.h` 文件
- `gui_lvgl.c` 在 `__RTE_GUI_LVGL_WRAPPER__` 条件下，通过
  `template/gui_scene_include.h` 自动初始化已启用的场景实例
- 如需基于 `<name>` 或 `<NAME>` 自定义模板命名规则，可参考
  `template/how_to_use_user_scene_template_in_pack_zh.md`

## 5. 初始化流程

`gui_lv_init()` 的执行顺序如下：

1. 非 `_WIN64` 环境下调用 `lv_init()`
2. 非 `_WIN64` 环境下调用 `lv_port_disp_init()`
3. 非 `_WIN64` 环境下调用 `lv_port_indev_init()`
4. 调用 `__gui_sys_data_init()` 初始化系统参数
5. 调用 `__gui_common_style_init()` 初始化公共样式
6. 调用 `__gui_all_scene_init()` 注册全部场景
7. 调用 `gui_lv_scene_switch(gui_lv_get_boot_scene_id())`
8. 调用 `lv_timer_handler()` 触发首次界面处理

运行要求如下：

- `gui_lv_init()` 仅调用一次
- `lv_timer_handler()` 由系统周期调度
- LVGL Tick 必须正常递增
- 开机场景 ID 必须已经注册

## 6. 场景与页面机制

### 6.1 场景接口

场景用于表示应用中的一级界面。主要接口如下：

| 接口 | 说明 |
| --- | --- |
| `bool gui_lv_scene_register(const gui_scene_cfg_t *ptCfg)` | 注册场景 |
| `void gui_lv_scene_switch(gui_scene_id_t eTargetId)` | 切换目标场景 |
| `void gui_lv_scene_back(void)` | 返回上一场景 |
| `gui_scene_id_t gui_lv_scene_get_current(void)` | 获取当前场景 ID |
| `lv_obj_t *gui_lv_scene_get_current_root(void)` | 获取当前场景根对象 |

场景注册时应至少提供以下成员：

- `eId`
- `pfInit`
- `pfDeinit`

场景切换过程中，组件会销毁当前场景并重新构建目标场景，以确保场景
根对象、页面状态与焦点记录保持一致。

### 6.2 页面接口

页面用于表示场景内部的子页面或局部流程页面。主要接口如下：

| 接口 | 说明 |
| --- | --- |
| `bool gui_lv_scene_page_register(const gui_page_cfg_t *ptCfg)` | 注册页面 |
| `void gui_lv_scene_page_push(gui_scene_page_id_t ePageId)` | 进入子页面 |
| `void gui_lv_scene_page_back(void)` | 返回上一页面 |

页面机制行为如下：

- `push` 时清空当前场景 root 下的界面对象
- 为目标页面重新创建容器与界面对象
- `back` 时依据保存的页面节点重新调用上一页初始化函数
- 自动保存并恢复焦点索引，支持多 group 场景

### 6.3 容量定义

当前实现使用以下默认容量：

| 宏 | 默认值 | 说明 |
| --- | --- | --- |
| `GUI_SCENE_HISTORY_DEPTH` | 8 | 场景历史深度 |
| `GUI_PAGE_NODE_POOL_SIZE` | 8 | 页面节点池大小 |
| `GUI_SCENE_GROUP_MAX` | 4 | 单场景最大 group 记录数 |

如业务场景需要更大容量，可在 `core/gui_lv_scene_manage.c` 中调整对应
宏定义。

## 7. 使用示例

### 7.1 定义场景 ID

在 `gui_scene_id.h` 中定义业务场景枚举：

```c
typedef enum {
    GUI_SCENE_HOME = 0,
    GUI_SCENE_SETTING,
    GUI_SCENE_MAX
} gui_scene_id_t;
```

### 7.2 注册场景

在用户场景文件中注册场景：

```c
static void __on_scene_home_draw(lv_obj_t *ptRoot)
{
    gui_lv_label_init(ptRoot,
                      10,
                      10,
                      "Home",
                      &lv_font_montserrat_20,
                      rgb(255, 255, 255));
}

static void __on_scene_home_depose(void)
{
}

void gui_lv_scene_home_init(void)
{
    const gui_scene_cfg_t c_tCFG = {
        .eId      = GUI_SCENE_HOME,
        .ptEx     = NULL,
        .pfInit   = __on_scene_home_draw,
        .pfDeinit = __on_scene_home_depose,
    };

    gui_lv_scene_register(&c_tCFG);
}
```

### 7.3 配置开机场景

在 `gui_lvgl.c` 的用户代码区完成系统参数与场景初始化：

```c
static void __gui_sys_data_init(void)
{
    gui_lv_set_lang(GUI_LV_LANGUAGE_EN);
    gui_lv_set_boot_scene_id(GUI_SCENE_HOME);
}

static void __gui_all_scene_init(void)
{
    gui_lv_scene_home_init();
}
```

## 8. Helper 模块说明

### 8.1 基础控件快捷接口

`core/gui_lv_common.*` 提供以下常用接口：

- `gui_lv_container_init`
- `gui_lv_img_init`
- `gui_lv_label_init`
- `gui_lv_btn_init`
- `gui_lv_bar_init`
- `gui_lv_spinbox_init`
- `gui_lv_timer_init`

同时提供 group 焦点相关接口：

- `gui_lv_group_focus_nav`
- `gui_lv_group_get_focus_index`

### 8.2 样式辅助接口

`helper/style` 使用 `gui_lv_style_t` 与 `gui_lv_style_config_t` 描述样式配置，支持
以下接口：

- `gui_lv_style_init()`
- `gui_lv_style_apply()`
- `gui_lv_style_remove()`
- `gui_lv_style_reset()`

支持的典型配置宏包括：

- `GUI_LV_STYLE_BG_COLOR(...)`
- `GUI_LV_STYLE_RADIUS(...)`
- `GUI_LV_STYLE_TEXT_FONT(...)`

### 8.3 输入与数据辅助接口

`helper/input` 与 `helper/data` 主要用于参数编辑类界面：

- `gui_lv_input_nav()`：网格导航控制
- `gui_lv_data_set()`：整型与浮点参数增减控制
- `gui_lv_data_save_to_flash()`：参数持久化接口占位
- `gui_lv_data_reset_to_default()`：参数恢复默认值接口占位

## 9. 资源生成说明

资源生成工具位于 `gui/gui_resource/`。相关文档如下：

- `../gui/gui_resource/README.md`
- `../gui/gui_resource/copilot.md`

典型使用流程如下：

1. 在 `assets/font_config.json` 与 `assets/img_config.json` 中配置资源
2. 运行 `tools/resource_gen.py` 或 `tools/generate_all.py` 生成资源文件
3. 在应用代码中使用 `generated/ui_resource.h` 中声明的字体与图片符号

在 Pack 集成场景下，通常直接使用 `generated/` 目录下的预生成文件，
不依赖 Python 运行环境。

## 10. 配置项说明

`gui_lv_conf.h` 中的关键配置如下：

- 显示分辨率
  - `MY_DISP_HOR_RES`
  - `MY_DISP_VER_RES`
- 显示缓冲区
  - `__BUF_PX_SIZE__`
  - `__LV_DISP_USE_BUFFER__`
- 输入设备使能
  - `__LV_USE_KEYPAD_INDEV__`
  - `__LV_USE_TOUCHPAD_INDEV__`
  - `__LV_USE_ENCODER_INDEV__`
  - `__LV_USE_MOUSE_INDEV__`
  - `__LV_USE_BUTTON_INDEV__`

配置输入设备时，应依据实际硬件能力与交互方式统一规划 group 绑定关
系和焦点移动策略。

## 11. 移植检查项

完成工程集成后，应至少确认以下事项：

- 已实现并验证 `lv_port_disp_init()`
- 已实现并验证 `lv_port_indev_init()`
- LVGL Tick 与 `lv_timer_handler()` 调度正常
- `gui_scene_id.h` 已定义有效场景枚举
- 开机场景 ID 已正确配置且已注册
- 若启用 keypad/group，已确认输入设备与 group 绑定正确
- 若启用页面栈，页面 ID 与节点池容量满足业务需求

## 12. 常见问题

### Q1：调用 `gui_lv_init()` 后没有显示内容

排查顺序如下：

- 确认开机场景已经注册
- 确认 `gui_lv_set_boot_scene_id()` 设置的 ID 有效
- 确认显示驱动刷新正常
- 确认 `lv_timer_handler()` 被周期调用

### Q2：调用 `gui_lv_scene_page_push()` 无页面变化

排查顺序如下：

- 确认已调用 `gui_lv_scene_page_register()`
- 确认 page ID 未越界
- 确认页面节点池未耗尽

### Q3：模板文件编译时报 `<name>` 或 `%Instance%` 未替换

说明如下：

- `template/` 目录中的相关文件为模板文件
- 模板文件不能原样直接参与编译
- 非 RTE 模式下应先复制模板并替换占位符

### Q4：RTE 模式下场景未自动初始化

排查顺序如下：

- 确认已启用对应 `Scene` 组件实例
- 确认已生成 `__RTE_GUI_LVGL_SceneN__` 宏

## 13. License

本项目采用 Apache-2.0 License，详见 `LICENSE`。

