# 在 CMSIS-Pack（RTE）中使用 User Scene Template（`<name>` / `<NAME>`）

本文说明如何在 **CMSIS-Pack / RTE（例如 Keil MDK 的 Run-Time Environment）** 中使用 Arm-2D 提供的 **User Scene Template** 来创建“自定义命名”的 Scene（即把模板里的 `<name>` / `<NAME>` 替换成你的场景名）。

> 关联模板文件：
> - `Helper/template/arm_2d_scene_template.h`
> - `Helper/template/arm_2d_scene_template.c`
>
> 另外，RTE 里通过“Scene 组件实例数”自动生成 `scene0/scene1/...` 使用的是另一套模板：
> - `Helper/template/arm_2d_scene.h`
> - `Helper/template/arm_2d_scene.c`

---

## 1. 两种“在 Pack 里添加 Scene”的方式（先搞清楚差别）

在 Pack/RTE 场景下，你通常会遇到两种添加 Scene 的方法：

1) **RTE 组件实例法（sceneN）**
- 在 RTE 配置里把 `Acceleration::Arm-2D Helper::Scene` 的实例数调大。
- 工具会在 `RTE/Acceleration/` 下自动生成 `arm_2d_scene_0.c/.h`、`arm_2d_scene_1.c/.h` ...
- 这些 scene 会被聚合头文件 `Helper/Include/arm_2d_scenes.h` 自动按宏开关包含。
- 调用方式是 `arm_2d_scene0_init(&DISP0_ADAPTER);` 这种 **数字命名**。

2) **User Code Template 法（自定义命名 `<name>`）**（本文重点）
- 在 IDE 里通过“User Scene Template”生成两份文件（或在 VS Code 里手工复制）。
- 你需要把模板中的 `<name>`/`<NAME>` 替换为你的命名。
- 这些文件 **不会** 被 `arm_2d_scenes.h` 自动包含，你需要自己 `#include "arm_2d_scene_<name>.h"`。

如果你的目标是“我想要一个叫 `arm_2d_scene_my_scene.c/h` 的场景”，请选择 **方法 2**。

---

## 2. 前置条件（RTE 里要勾哪些组件）

User Scene Template 的代码主体被以下宏包住：

- `RTE_Acceleration_Arm_2D_Helper_PFB`

因此你至少需要在 RTE 中启用：

- `Acceleration::Arm-2D::Core`
- `Acceleration::Arm-2D Helper::PFB`
- `Acceleration::Arm-2D Helper::Display Adapter`（至少 1 个实例，例如 Adapter0）

模板自带“演示绘制代码”，还会用到：

- `Acceleration::Arm-2D Extras::Controls`（提供 `draw_round_corner_box()` 等示例控件）
- `Acceleration::Arm-2D Extras::LCD ASCII Printf`（提供 `arm_lcd_text_set_font()` / `arm_lcd_puts()` / `ARM_2D_FONT_6x8` 等）

如果你不想引入上述 Extras，也可以直接把模板里的演示绘制与文字输出段落删掉，仅保留场景骨架。

---

## 3. 生成/添加模板文件

### 3.1 在 Keil MDK（RTE 工程）里添加

- 在 Project 视图里选中一个 Group
- 右键 → **Add New Item to Group**
- 找到 **User Code Template**
- 在 Acceleration 分类下选择 **User Scene Template**
- 点击 Add

随后会把 `arm_2d_scene_template.c` / `arm_2d_scene_template.h` 加到你的工程目录（位置取决于你当前 Group 的路径/设置）。

### 3.2 在 VS Code / 非 MDK 环境手工添加

- 直接复制：
  - `Helper/template/arm_2d_scene_template.h`
  - `Helper/template/arm_2d_scene_template.c`
- 放到你的工程源码目录（确保编译系统会编译 `.c`、并能找到 `.h`）

---

## 4. `<name>` / `<NAME>` 替换与重命名规则（核心）

模板里有两类占位符：

- `<name>`：**小写/下划线**风格（建议 `snake_case`），用于：
  - 文件名：`arm_2d_scene_<name>.h/.c`
  - 符号名：`user_scene_<name>_t`、`arm_2d_scene_<name>_init()`、`__arm_2d_scene_<name>_init()` 等

- `<NAME>`：**大写/下划线**风格（建议全大写），用于：
  - include guard：`__ARM_2D_SCENE_<NAME>_H__`
  - OOC 控制宏：`__USER_SCENE_<NAME>_IMPLEMENT__` / `__USER_SCENE_<NAME>_INHERIT__`

推荐做法（避免多个 Scene 冲突）：

1) 先把文件重命名为你的场景名（可选但强烈建议）
- `arm_2d_scene_template.h` → `arm_2d_scene_my_scene.h`
- `arm_2d_scene_template.c` → `arm_2d_scene_my_scene.c`

2) 全局替换占位符
- `<name>` → `my_scene`
- `<NAME>` → `MY_SCENE`

3) 检查 `.c` 顶部 include 是否匹配

模板原始写法类似：

```c
#define __USER_SCENE_<NAME>_IMPLEMENT__
#include "arm_2d_scene_<name>.h"
```

替换后应为：

```c
#define __USER_SCENE_MY_SCENE_IMPLEMENT__
#include "arm_2d_scene_my_scene.h"
```

> 注意：`__USER_SCENE_<NAME>_IMPLEMENT__` 只应该在 **本 Scene 的 `.c` 文件**里定义一次。

---

## 5. 在应用中初始化并显示 Scene

### 5.1 最小调用方式

在你的 `main.c`（或系统初始化代码）里：

```c
#include "arm_2d_helper.h"
#include "arm_2d_disp_adapters.h"
#include "arm_2d_scene_my_scene.h"    /* 你的自定义 scene */

int main(void)
{
    arm_2d_init();

    disp_adapter0_init();

    /* 创建/初始化并追加到 Scene Player FIFO */
    arm_2d_scene_my_scene_init(&DISP0_ADAPTER);

    /* 让 Player 切换到 FIFO 的下一个场景（通常是你刚追加的那个） */
    arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);

    while (1) {
        disp_adapter0_task();
    }
}
```

### 5.2 关于“是否必须调用 switch_to_next_scene()”

Scene 是被“追加”到 `Scene Player FIFO` 里的：

- 如果你的 Display Adapter **启用了默认场景**，那么新追加的 scene 往往只是“下一个可用场景”，需要调用一次 `arm_2d_scene_player_switch_to_next_scene()` 才能显示。
- 如果你在 Display Adapter 配置里 **禁用了默认场景**，那么你追加的第一个 scene 可能会立刻成为当前 scene，此时不一定需要额外 switch。

由于不同工程模板对“默认 scene”的策略不同，最稳妥做法是：

- 初始化后显式调用一次 `arm_2d_scene_player_switch_to_next_scene()`。

---

## 6. Scene 实例生命周期（自动分配 vs 用户分配）

`arm_2d_scene_<name>_init()` 是一个宏，最终会调用：

```c
user_scene_<name>_t *__arm_2d_scene_<name>_init(arm_2d_scene_player_t *ptDispAdapter,
                                               user_scene_<name>_t *ptScene);
```

用法有两种：

1) **传 NULL（自动分配）**
- `arm_2d_scene_my_scene_init(&DISP0_ADAPTER);`
- 内部会从 scratch memory 分配 `user_scene_my_scene_t`
- 在 `fnDepose` 回调中自动释放

2) **传入你自己管理的对象（用户分配）**

```c
static user_scene_my_scene_t s_tMyScene;
arm_2d_scene_my_scene_init(&DISP0_ADAPTER, &s_tMyScene);
```

- 模板内部会设置 `bUserAllocated = true`
- `fnDepose` 不会释放这块内存
- 你必须保证 `s_tMyScene` 的生命周期覆盖整个 scene 使用期

---

## 7. 常见问题与排查

### 7.1 调用了 `arm_2d_scene_<name>_init()`，但链接报找不到符号

优先检查：

- 你的 `.c` 文件是否真的被加入工程并参与编译/链接
- RTE 是否启用了 `Acceleration::Arm-2D Helper::PFB`（否则模板代码块会被 `#if defined(RTE_Acceleration_Arm_2D_Helper_PFB)` 排除）

### 7.2 编译通过但链接报 `arm_lcd_*` / `ARM_2D_FONT_6x8` 未定义

这是因为模板演示代码使用了 LCD printf 能力。

- 方案 A：在 RTE 勾选 `Acceleration::Arm-2D Extras::LCD ASCII Printf`
- 方案 B：删除 scene 绘制函数里那段 `arm_lcd_text_*` 与 `arm_lcd_puts()` 的演示输出

### 7.3 编译报 `draw_round_corner_box()` 等未声明/未定义

- 在 RTE 勾选 `Acceleration::Arm-2D Extras::Controls`
- 或者删除模板演示绘制段落，只保留你自己的绘制逻辑

### 7.4 自定义命名的 scene 为什么没被 `arm_2d_scenes.h` 自动包含？

`Helper/Include/arm_2d_scenes.h` 只会按 `RTE_Acceleration_Arm_2D_SceneN` 这些宏去 `#include "arm_2d_scene_N.h"`。

User Scene Template（`<name>`）生成的文件名不是 `arm_2d_scene_N.*`，也不会自动受 RTE Scene 实例宏控制，所以需要你手动 include。

---

## 8. 附：何时用 RTE 的 `sceneN`，何时用 `<name>`

- 你想让 **RTE 自动生成** scene 文件、并用 `arm_2d_scenes.h` 自动聚合 → 用 `sceneN`
- 你想要 **可读性更强、按业务命名**（例如 `my_scene`, `boot_logo`, `home`）→ 用 `<name>`

两者本质都是“把 scene 追加到同一个 scene player FIFO”，可以混用。
