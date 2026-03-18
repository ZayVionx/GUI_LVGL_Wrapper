# GUI_LV 代码规范

## 1. 适用范围与基本原则

- 适用范围：`gui_lv` 相关的 `.h/.c` 文件（含 porting、helper、drivers 等）。
- 原则：
  - 对外接口清晰、稳定、可文档化。
  - 内部实现可替换（私有符号与宏明确隔离）。
  - 编译器/平台差异显式处理，不把 warning 传播给用户工程。

---

## 2. 文件结构（头文件/源文件模板）

### 2.1 头文件（.h）建议骨架

按固定分段组织，保持可扫描性（段名用全大写）：

1) 文件头注释（License + 简短描述 + 元信息）

2) include guard

3) `/*============================ INCLUDES ======================================*/`

4) `extern "C"`（若需要 C++ 兼容）

5) Doxygen 分组（如 `\addtogroup`）

6) `/*============================ MACROS ========================================*/`

7) `/*============================ MACROFIED FUNCTIONS ===========================*/`

8) `/*============================ TYPES =========================================*/`

9) `/*============================ GLOBAL VARIABLES ==============================*/`

10) `/*============================ PROTOTYPES ====================================*/`

11) 关闭 `extern "C"`

12) `#endif`

### 2.2 源文件（.c）建议骨架

1) 文件头注释

2) `/*============================ INCLUDES ======================================*/`

3) 必要的 `#define __GUI_LV_xxx_IMPLEMENT__`（用于控制头文件的实现/继承开关，见 5.3）

4) `/*============================ MACROS ========================================*/`

5) `/*============================ TYPES =========================================*/`

6) `/*============================ LOCAL VARIABLES ===============================*/`

7) `/*============================ GLOBAL VARIABLES ==============================*/`

8) `/*============================ PROTOTYPES ====================================*/`

9) `/*============================ IMPLEMENTATION ================================*/`

---

## 3. 注释规范（Doxygen 优先）

### 3.1 什么时候必须写注释

- 所有对外 API（在 public header 中暴露的函数、类型、宏、全局变量）。
- 复杂/易错的内部算法、位域、定点格式、内存布局、并发/IRQ 保护代码。
- 配置宏（尤其是“性能/安全取舍”的开关）必须写 `\note` 或块注释解释风险。

### 3.2 Doxygen 块注释格式

对外符号使用 `/*! ... */`。

- 函数：
  - `\brief` 一句话说明做什么。
  - `\param[in]` / `\param[out]` 清晰描述含义、单位、允许范围、NULL 语义。
  - `\return` 或 `\retval`（布尔/状态码）说明返回值语义。
  - `\note` 写“调用方必须知道但不适合塞进 brief 的约束”。
  - `\details` 写更长解释或算法背景（只在必要时）。

- 类型/结构体：
  - `\brief` 说明用途。
  - 成员注释建议用行尾 `//!<`（简短）或成员前块注释（较长）。

- 示例：用 `\code ... \endcode` 或 `\example`。

### 3.3 分组（Module/Group）

- 使用 `\addtogroup` 将 API 分到明确模块（例如 `gKernel`、`gHelper`、`gPorting`）。
- 每个模块用 `@{` / `@}` 成对闭合。

---

## 4. 代码风格（排版与表达方式）

### 4.1 缩进与括号

- 缩进：4 空格（不要 Tab）。
- 大括号：函数采用“左大括号换行”（Allman）：

```c
void gui_lv_init(void)
{
    /* ... */
}
```

- `if/for/while/switch` 的 `{}`：推荐始终保留（避免单行分支引入隐患），除非项目已有一致例外规则。

### 4.2 空格与换行

- 二元运算符两侧空格：`a + b`、`x == y`。
- 指针声明 `type *p` 风格（星号贴变量名）。
- 逻辑块之间空一行；连续声明/短语句不强行空行。

### 4.3 作用域与局部变量

- 尽量缩小变量作用域；短期临时变量可以用 `do { ... } while (0);` 限定。
- 宏模板需要局部变量时，必须使用安全命名（见 5.6），避免与调用点冲突。

### 4.4 断言与参数校验

- 对内部函数：可 `assert()` + `GUI_LV_UNUSED()` 抑制未用参数。
- 对外 API：根据策略选择“返回错误码/状态”或“断言”，但必须一致。

### 4.5 编译器差异与 warning 控制

- 对外公共头文件可对 clang/gcc/armcc 分支 `#pragma diagnostic push/ignored`，目标是“库的 warning 不污染用户工程”。
- 源文件可按需压制特定 warning，但不要全关。

---

## 5. 命名规范（核心）

### 5.1 符号分层（public / private）

- 对外 API（函数/可见全局）：
  - 前缀：`gui_lv_`
  - 例：`gui_lv_init()`、`gui_lv_set_default_framebuffer()`

- 对外宏/常量/枚举值：
  - 前缀：`GUI_LV_`
  - 例：`GUI_LV_VERSION_MAJOR`、`GUI_LV_ERR_BUSY`

- 内部实现（不希望用户调用/依赖）：
  - 函数/变量：`__gui_lv_...`
  - 宏/配置：`__GUI_LV_...`

> 约定：双下划线前缀表示“内部/私有/可能变化”。只在模块内部 include 的私有头文件中暴露。

### 5.2 类型命名

- 所有公开类型以 `_t` 结尾：`gui_lv_xxx_t`。
- 结构体/联合体：
  - `typedef struct gui_lv_xxx_t { ... } gui_lv_xxx_t;`
  - 或先声明 `struct gui_lv_xxx_t;` 再 `typedef`。

### 5.3 “实现/继承”开关宏（可选但推荐）

如果需要在头文件里通过宏控制：实现展开、继承保护等（类似 OOC 风格），使用：

- `__GUI_LV_IMPL__` / `__GUI_LV_INHERIT__`
- 每个 helper/service 头可定义自己的开关：
  - `__GUI_LV_HELPER_IMPLEMENT__`
  - `__GUI_LV_HELPER_INHERIT__`

并在 `.c` 中先 `#define __GUI_LV_HELPER_IMPLEMENT__` 再 include 头文件。

### 5.4 配置与特性宏

- 特性检测（是否支持某能力）：`__GUI_LV_HAS_xxx__`
  - 例：`__GUI_LV_HAS_ASYNC__`、`__GUI_LV_HAS_FPU__`

- 配置项（可由用户工程覆写）：`__GUI_LV_CFG_xxx__`
  - 例：`__GUI_LV_CFG_ENABLE_LOG__`、`__GUI_LV_CFG_DEFAULT_TASK_POOL_SIZE__`

- 约定：
  - 配置项必须 `#ifndef` 包裹，给出默认值。
  - 性能/安全“危险开关”用 `__GUI_LV_CFG_UNSAFE_xxx__` 命名，并用注释明确风险。

### 5.5 变量前缀（匈牙利式轻量约定）

- `bXxx`：bool
- `chXxx`：8-bit（字符/uint8_t）
- `hwXxx`：16-bit（half word / uint16_t）
- `wXxx`：32-bit（word / uint32_t）
- `lXxx`：64-bit（long long / int64_t）
- `nXxx`：有符号 int（常见 int32_t）

指针类：

- `ptXxx`：指向结构体/对象的指针（pointer to）
- `pchXxx`：`char *` / `uint8_t *`（按项目习惯可用于 byte buffer）
- `pnXxx`：`int *`
- `plXxx`：`int64_t *`
- `pwXxx`：`uint32_t *`
- `phwXxx`：`uint16_t *`

数组/常量：

- `s_`：file-static 变量（例如 `s_tContext`）
- `c_`：常量/只读资源（例如 `c_bmpLogo`、`c_tileLogo`）

### 5.6 宏局部变量的“安全命名”

- 必须提供：`GUI_LV_SAFE_NAME(...)` / `gui_lv_safe_name(...)`
- 用途：宏展开时生成不会与调用点冲突的局部变量名（常用 `__LINE__` 拼接）。

示例：

```c
#define GUI_LV_SAFE_NAME(...)    GUI_LV_CONNECT(__, __LINE__, ##__VA_ARGS__)
```

---

## 6. 例子（建议写法）

### 6.1 对外 API 注释示例

```c
/*!
 * \brief 初始化 GUI_LV 内核
 *
 * \note 必须在任何 GUI_LV 绘制/任务接口之前调用。
 */
void gui_lv_init(void);
```

### 6.2 NULL 语义与返回值示例

```c
/*!
 * \brief 设置默认帧缓冲
 * \param[in] ptFrameBuffer 新的帧缓冲；传 NULL 表示清空默认帧缓冲
 * \return 返回旧的默认帧缓冲指针（可能为 NULL）
 */
const gui_lv_tile_t *gui_lv_set_default_framebuffer(const gui_lv_tile_t *ptFrameBuffer);
```

---

## 7. 文档与一致性检查（建议）

- 头文件对外符号必须有 Doxygen 注释，否则视为未完成接口设计。
- “公共 API/宏/类型”的命名必须严格遵循 `gui_lv_` / `GUI_LV_` 前缀。
- 内部符号必须 `__gui_lv_` / `__GUI_LV_`，并尽量放在私有头或源文件内。

---

