# GUI_LVGL_Wrapper

`GUI_LVGL_Wrapper` 是一套面向 `LVGL 8.x` 的嵌入式 GUI 封装工程，用于统一 GUI 初始化、场景管理、页面切换、输入焦点恢复和资源组织方式。

仓库支持两种集成形态：

- 源码集成：直接将 `gui_lv/` 及所需资源文件加入现有嵌入式工程。
- `CMSIS-Pack` 集成：通过 Pack / RTE 组件方式启用框架模块和场景模板。

## 1. 适用场景

- 基于 `LVGL 8.x` 的 MCU 图形界面项目。
- 需要统一管理多页面、多场景切换逻辑的应用。
- 需要复用 GUI 初始化、页面栈、焦点恢复等基础能力的工程。
- 需要规范化管理字体、图片等 GUI 资源的项目。
- 需要同时维护源码集成和 `CMSIS-Pack` 集成方式的项目。

## 2. 仓库结构

```text
GUI_LVGL_Wrapper/
├─ gui/                     # 用户层目录、资源目录与模板占位目录
│  ├─ gui_app/              # 应用层场景模板占位目录
│  ├─ gui_resource/         # GUI 资源配置、生成结果与工具脚本
│  └─ gui_task/             # GUI 任务层模板占位目录
├─ gui_lv/                  # GUI 封装核心代码与 Pack 模板
│  ├─ core/                 # 场景管理与基础控件封装
│  ├─ helper/               # style / input / data 辅助模块
│  ├─ port/                 # 显示与输入设备移植模板
│  └─ template/             # 场景模板与 Pack 场景模板
└─ README.md                # 仓库总说明文档
```

核心目录说明：

| 目录 | 说明 |
| --- | --- |
| `gui_lv/` | GUI 框架主体，包含初始化入口、场景管理、辅助模块、移植模板和 Pack 模板。 |
| `gui/gui_resource/` | 字体、图片等 GUI 资源的配置、生成脚本和预生成结果。 |
| `gui/gui_app/` | 用户场景模板占位目录，便于应用层扩展。 |
| `gui/gui_task/` | 用户任务层模板占位目录，便于任务调度层扩展。 |

## 3. 快速接入

### 3.1 源码集成

适用于普通嵌入式工程，典型接入流程如下：

1. 集成 `LVGL 8.x`，并完成显示与输入设备底层移植。
2. 将 `gui_lv/` 中需要的源码加入工程编译。
3. 按需将 `gui/gui_resource/generated/` 下的资源文件加入工程。
4. 在 `gui_scene_id.h` 中定义场景与页面 ID。
5. 基于模板创建业务场景文件，并完成场景注册。
6. 系统启动时调用 `gui_lv_init()`，运行阶段周期调用 `lv_timer_handler()`。

框架接口、初始化流程和场景机制以 [gui_lv/README.md](gui_lv/README.md) 为准。

### 3.2 CMSIS-Pack 集成

适用于采用 RTE 管理组件的工程。仓库中已提供 PDSC 文件和模板文件，可通过 Pack 方式启用以下模块：

- `Core`
- `Helper`
- `USER`
- `Scene`

Pack 方式下可直接使用实例化场景模板，也可使用用户命名模板手动注册场景。

## 4. 资源管理

资源系统位于 `gui/gui_resource/`，用于通过 JSON 配置和 Python 脚本生成 LVGL 可直接编译的 `.c` 资源文件。

- 如果仅使用现有资源，可直接将 `generated/` 下的文件加入工程。
- 如果需要更新字体或图片资源，可安装工具依赖后重新执行生成脚本。

资源配置格式、生成流程和工具依赖以 [gui/gui_resource/copilot.md](gui/gui_resource/copilot.md) 为准。

## 5. 依赖说明

| 类型 | 依赖 |
| --- | --- |
| GUI 运行 | `LVGL 8.x` |
| 资源生成 | `Python 3`、`Pillow` |
| 字体生成 | `lv_font_conv` |
| Pack 集成 | 支持 `CMSIS-Pack` / `RTE` 的工程环境 |

## 6. 文档入口

| 文档 | 说明 |
| --- | --- |
| [gui_lv/README.md](gui_lv/README.md) | GUI 框架主文档，包含接口、初始化流程和场景机制。 |
| [gui/gui_resource/README.md](gui/gui_resource/README.md) | 资源目录简要说明。 |
| [gui/gui_resource/copilot.md](gui/gui_resource/copilot.md) | 资源生成系统详细设计与使用说明。 |
| [gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md](gui_lv/template/how_to_use_user_scene_template_in_pack_zh.md) | Pack 场景模板使用说明。 |
