# GUI_LVGL_Wrapper

GUI_LVGL_Wrapper 是一套面向 LVGL 8.x 的嵌入式图形界面封装工程，
用于统一 GUI 初始化流程、场景管理、页面切换、输入焦点恢复以及资
源组织方式。仓库同时提供源码集成形态、CMSIS-Pack 分发形态以及资
源自动生成工具，适用于 MCU 图形界面项目的快速落地与后续维护。

## 1. 项目定位

本仓库的目标是为 LVGL 图形界面工程提供一套可复用的基础框架，主
要覆盖以下内容：

- GUI 框架主入口与系统初始化流程
- 场景注册、场景切换和页面栈管理
- 输入焦点记录与恢复机制
- 常用样式、输入和数据辅助模块
- 显示与输入设备移植模板
- 资源生成工具链与预生成资源文件
- CMSIS-Pack / RTE 模板与集成支持

适用场景如下：

- 基于 LVGL 8.x 的 MCU 图形界面项目
- 需要统一场景组织方式的多页面应用
- 同时支持源码集成与 Pack 集成的工程
- 需要规范化管理字体和图片资源的项目

## 2. 仓库结构

仓库主要由 GUI 框架、资源系统和模板文件三部分组成。

```text
GUI_LVGL_Wrapper/
├─ gui/                     # 用户层示例目录、资源目录与模板占位目录
│  ├─ gui_app/             # 应用层场景模板占位文件
│  ├─ gui_resource/        # GUI 资源配置、生成结果与工具脚本
│  └─ gui_task/            # GUI 任务层模板占位文件
├─ gui_lv/                 # GUI 封装核心代码与 Pack 模板
│  ├─ core/                # 场景管理与基础控件封装
│  ├─ helper/              # style / input / data 辅助模块
│  ├─ port/                # 显示与输入设备移植模板
│  └─ template/            # 场景模板与 Pack 场景模板
└─ README.md               # 仓库总说明文档
```

各目录职责说明如下：

| 目录 | 说明 |
| --- | --- |
| gui_lv/ | GUI 框架主体，包含初始化入口、场景管理、辅助模块和模板文件 |
| gui/gui_resource/ | 资源配置、生成脚本、预生成字体与图片文件 |
| gui/gui_app/ | 用户场景模板占位目录，便于应用层扩展 |
| gui/gui_task/ | 用户任务层模板占位目录，便于任务调度层扩展 |

## 3. 核心模块

### 3.1 gui_lv

gui_lv 是本仓库的核心模块，负责 LVGL 图形界面的统一封装，主要提
供以下能力：

- gui_lv_init() 统一初始化入口
- 场景注册、切换与返回
- 页面压栈与页面返回
- 焦点记录与恢复
- 常用控件创建辅助接口
- 样式、输入和数据辅助接口

详细说明见 [gui_lv/README.md](gui_lv/README.md)。

### 3.2 gui_resource

gui/gui_resource 用于管理项目中的字体与图片资源，支持通过 JSON 配
置和 Python 脚本自动生成 LVGL 可直接编译的 .c 资源文件。目录中同
时保留 generated/ 预生成结果，便于在不依赖 Python 环境的情况下直
接集成。

详细说明见 [gui/gui_resource/README.md](gui/gui_resource/README.md)。

### 3.3 template

gui_lv/template 和 gui 下的模板文件用于创建新的场景源码，适用于以
下两类使用方式：

- 直接复制模板文件后按业务命名修改
- 在 CMSIS-Pack / RTE 中按模板规则自动生成

关于 Pack 场景模板的使用方法，见
[gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md](gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md)。

## 4. 推荐使用方式

### 4.1 作为源码库集成

适用于普通嵌入式工程，典型步骤如下：

1. 集成 LVGL 8.x，并完成显示与输入设备底层移植。
2. 将 gui_lv/ 中所需源码加入工程编译。
3. 按需将 gui/gui_resource/generated/ 下的资源文件加入工程。
4. 在 gui_scene_id.h 中定义场景与页面 ID。
5. 基于模板创建业务场景文件并完成注册。
6. 系统启动时调用 gui_lv_init()，运行阶段周期调用
	 lv_timer_handler()。

### 4.2 作为 CMSIS-Pack 集成

适用于采用 RTE 管理组件的工程。仓库中已提供 PDSC 文件和模板文件，
可通过 Pack 方式启用以下模块：

- Core
- Helper
- USER
- Scene

在 Pack 方式下，可直接使用实例化场景模板，也可使用用户命名模板手
动注册场景。

## 5. 资源管理说明

资源系统位于 gui/gui_resource/，主要包含以下内容：

- assets/font_config.json：字体生成配置
- assets/img_config.json：图片生成配置
- generated/font/：预生成字体资源
- generated/img/：预生成图片资源
- generated/ui_resource.h：资源声明头文件
- tools/：资源生成脚本与依赖配置

资源使用方式如下：

- 如果仅使用现成资源，可直接将 generated/ 下文件加入工程。
- 如果需要更新资源，可在本地安装依赖后重新执行生成脚本。

## 6. 文档入口

为避免根文档过长，详细说明拆分在各子模块文档中：

| 文档 | 说明 |
| --- | --- |
| [gui_lv/README.md](gui_lv/README.md) | GUI 框架主文档 |
| [gui/gui_resource/README.md](gui/gui_resource/README.md) | 资源目录简要说明 |
| [gui/gui_resource/copilot.md](gui/gui_resource/copilot.md) | 资源生成系统详细设计与使用说明 |
| [gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md](gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md) | Pack 场景模板使用说明 |

## 7. 依赖说明

不同使用方式对应的依赖如下：

| 类型 | 依赖 |
| --- | --- |
| GUI 运行 | LVGL 8.x |
| 资源生成 | Python 3、Pillow |
| 字体生成 | lv_font_conv |
| Pack 集成 | 支持 CMSIS-Pack / RTE 的工程环境 |

## 8. 说明

- 根目录 README 用于说明仓库整体结构与使用入口。
- GUI 框架的详细接口、初始化流程和场景机制，以 gui_lv/README.md
	为准。
- 资源生成流程、配置格式和工具依赖，以 gui/gui_resource/copilot.md
	为准。

