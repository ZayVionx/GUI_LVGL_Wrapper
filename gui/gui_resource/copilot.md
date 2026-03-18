# LVGL 资源自动化生成系统

## 概述

本系统通过 Python 脚本 `resource_gen.py` + Makefile，从 JSON 配置自动生成 LVGL 8.x
兼容的字体 `.c` 文件和图片 `.c` 文件，并自动维护 `ui_resource.h` 头文件声明。

---

## 1. 目录结构

```
UI_Resource/
├── copilot.md              # 本文档
├── ui_resource.h           # 自动生成的资源声明头文件
├── font/                   # 字体 .c 输出目录
│   ├── lv_font_Roboto_Bold_16.c
│   └── ...
├── img/                    # 图片 .c 输出目录
│   ├── lv_img_qr_code.c
│   └── ...
├── src/                    # 配置与源素材
│   ├── font_config.json    # 字体配置
│   ├── img_config.json     # 图片配置
│   ├── font_scr/           # 字体源文件 (.otf/.ttf) + 字符文本 (.txt)
│   └── img_scr/            # 图片源文件 (.png)
└── tools/                  # 构建工具
    ├── Makefile             # 构建入口
    └── resource_gen.py      # 核心生成脚本
```

---

## 2. 使用方法

在 `UI_Resource/tools/` 目录下执行：

| 命令                  | 说明                           |
| --------------------- | ------------------------------ |
| `make all resource`   | 全量生成所有字体和图片 `.c`    |
| `make font resource`  | 仅生成所有字体 `.c`            |
| `make img resource`   | 仅生成所有图片 `.c`            |

也可在任意目录直接调用 Python 脚本：

```bash
python tools/resource_gen.py          # 等同于 make all
python tools/resource_gen.py --font   # 仅字体
python tools/resource_gen.py --img    # 仅图片
```

每次执行会：
1. 清空对应输出目录中的旧文件
2. 按配置重新生成所有 `.c` 文件
3. 自动同步 `ui_resource.h` 声明（仅保留当前实际生成的资源）

---

## 3. 配置文件格式

### 3.1 字体配置 (`src/font_config.json`)

支持 `//` 行注释。每个条目对应一个字体生成任务：

```jsonc
{
    "font": [
        {
            "name"   : "Roboto_Bold",         // 用于生成文件名
            "text"   : "roboto_bold_16.txt",  // 字符集文本文件 (在 font_scr/ 下)
            "source" : "DROIDSANS-BOLD.TTF",  // 字体源文件 (在 font_scr/ 下)
            "bpp"    : "4",                   // 位深度: 1/2/3/4/8
            "size"   : "16"                   // 字号 (px)
        }
    ]
}
```

**生成文件名规则**: `lv_font_{name}_{size}.c`
- 示例: `name=Roboto_Bold`, `size=16` → `lv_font_Roboto_Bold_16.c`

### 3.2 图片配置 (`src/img_config.json`)

```jsonc
{
    "img": [
        {
            "name"   : "qr_code",           // 用于生成文件名
            "file"   : "qr_code.png",       // 图片源文件 (在 img_scr/ 下)
            "format" : "CF_INDEXED_4_BIT"   // 索引色格式
        }
    ]
}
```

**生成文件名规则**: `lv_img_{name}.c`
- 示例: `name=qr_code` → `lv_img_qr_code.c`

**支持的图片格式**:

| 配置值               | LVGL 宏                    | 色深 |
| -------------------- | -------------------------- | ---- |
| `CF_INDEXED_1_BIT`   | `LV_IMG_CF_INDEXED_1BIT`  | 1    |
| `CF_INDEXED_2_BIT`   | `LV_IMG_CF_INDEXED_2BIT`  | 2    |
| `CF_INDEXED_4_BIT`   | `LV_IMG_CF_INDEXED_4BIT`  | 4    |
| `CF_INDEXED_8_BIT`   | `LV_IMG_CF_INDEXED_8BIT`  | 8    |

---

## 4. 核心机制

### 4.1 字体合并与去重

当多个配置条目的 `name`、`source`、`bpp`、`size` 完全相同，仅 `text` 不同时，
自动合并为单个字体文件：

1. 读取所有涉及的 `.txt` 字符集文件
2. 合并并去重（保留每个字符的唯一实例）
3. 用合并后的字符集调用 `lv_font_conv` 生成一个 `.c` 文件

日志示例：`[font] 合并3个字体配置，去重后字符数：288 -> lv_font_SourceHanSansSC_Reg_18.c`

### 4.2 字体.c 文件头部注释格式

生成后自动将绝对路径替换为纯文件名，并以三段式格式化 `Opts:` 注释：

```c
/*******************************************************************************
 * Size: 18 px
 * Bpp: 4
 * Opts: --bpp 4 --size 18 --no-compress --no-prefilter --font SourceHanSansSC-Regular-2.otf
 *   --symbols
 *     0123456789:?ABEKNSTVZghilns不中主于亮从他
 *     件低体关其册出分动厂取号否商型基复子屏州
 *   --format lvgl -o lv_font_SourceHanSansSC_Reg_18.c --force-fast-kern-format
 ******************************************************************************/
```

- `--symbols` 前的参数为第一段
- `--symbols` 内容每行不超过 60 字符自动换行为第二段
- `--format` 及之后为第三段

### 4.3 图片转换

使用 Pillow 将 PNG 转换为 LVGL 索引色格式（无需外部工具 `pngquant`）：

1. 读取 PNG → 转 RGBA
2. 量化到指定色深的调色板
3. 按 LVGL 格式 (B, G, R, A) 排列调色板
4. 打包像素索引数据
5. 写入符合 LVGL 8.x 规范的 `.c` 文件

### 4.4 头文件自动维护

每次生成后自动扫描 `font/` 和 `img/` 目录，收集所有 `.c` 文件名，
重新生成 `ui_resource.h`：

```c
#ifndef __UI_RESOURCE_H__
#define __UI_RESOURCE_H__

/*================================= INCLUDES =================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*=================================== FONT ===================================*/
LV_FONT_DECLARE(lv_font_Roboto_Bold_16)
LV_FONT_DECLARE(lv_font_Roboto_Bold_70)

/*=================================== IMAGE ==================================*/
LV_IMG_DECLARE(lv_img_qr_code)

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif  /* __UI_RESOURCE_H__ */
```

---

## 5. 生成文件格式规范

### 5.1 字体 `.c` 文件

由 `lv_font_conv` 生成，兼容 LVGL 6/7/8/9，包含：
- 字形位图数据 (`glyph_bitmap`)
- 字形描述 (`glyph_dsc`)
- Unicode 字符映射 (`cmaps`)
- 字距数据 (`kern_classes`)
- 公开字体描述符 (`lv_font_t lv_font_xxx`)

### 5.2 图片 `.c` 文件

由脚本自身生成，包含：
- `#include "lvgl.h"` 检测
- `LV_ATTRIBUTE_MEM_ALIGN` / `LV_ATTRIBUTE_IMG_xxx` 宏
- 像素数据数组 (`uint8_t lv_img_xxx_map[]`)
- 图片描述符 (`lv_img_dsc_t lv_img_xxx`)

---

## 6. 外部依赖

| 依赖         | 用途           | 安装方式                          |
| ------------ | -------------- | --------------------------------- |
| `lv_font_conv` | 字体生成    | `npm install -g lv_font_conv`     |
| `Pillow`     | 图片转换       | `pip install pillow`              |

---

## 7. 日志输出示例

```
[font] start
[clean] remove lv_font_Roboto_Bold_16.c
[font] 合并1个字体配置，去重后字符数：9 -> lv_font_Roboto_Bold_16.c
[font] generated lv_font_Roboto_Bold_16.c (8450 bytes)
[img] start
[clean] remove lv_img_qr_code.c
[img] generate lv_img_qr_code.c from qr_code.png (CF_INDEXED_4_BIT)
[img] generated lv_img_qr_code.c (46211 bytes)
[header] synced ui_resource.h (font=9, img=29)
[done] resource generation completed
```

---

## 8. 设计约束

1. **幂等性**: 重复执行同一命令不会产生错误（先清空再重建）
2. **声明同步**: `ui_resource.h` 仅保留当前实际存在的资源声明
3. **路径安全**: 生成的 `.c` 文件中不包含本地绝对路径
4. **合并完整性**: 合并后的字符集包含所有参与合并配置的唯一字符，无遗漏无重复