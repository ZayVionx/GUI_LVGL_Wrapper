# UI_Resource

## 环境配置

在使用 `tools/` 下的脚本重新生成资源前，需要完成以下环境配置。

### 1. Python 依赖

需要 Python 3.8+，安装以下依赖：

```bash
pip install -r tools/requirements.txt
```

依赖说明：
- `pillow` — PNG 图片读取（alpha 格式转换使用）
- `pypng` — 解析 pngquant 输出的调色板 PNG（indexed 格式转换使用）
- `lz4` — LVGL 9 官方图片转换脚本的启动依赖

### 2. pngquant（必须）

`pngquant` 是图片颜色量化工具，用于将 RGBA PNG 转换为索引色格式，与 LVGL 在线转换器使用相同的量化算法。

**Windows：**

从 [https://pngquant.org/](https://pngquant.org/) 下载 `pngquant-windows.zip`，解压后将 `pngquant.exe` 放入 PATH 中的任意目录，例如：

```
D:\Python\Scripts\pngquant.exe
```

验证安装：

```bash
pngquant --version
```

**Linux：**

```bash
sudo apt install pngquant
```

**macOS：**

```bash
brew install pngquant
```

### 3. lv_font_conv（生成字体时需要）

```bash
npm install -g lv_font_conv
```

---

## 使用方式

在 `gui/gui_resource/` 目录下执行：

```bash
# 同时生成字体和图片
python tools/resource_gen.py

# 仅生成图片
python tools/resource_gen.py --img

# 仅生成字体
python tools/resource_gen.py --font
```

在 `tools/` 目录下也可以执行 `make img`、`make image` 或 `make all`。

图片生成版本由 `assets/img_config.json` 顶层的 `lvgl_version` 控制：

```json
{
    "lvgl_version": 9
}
```

- `8`：使用原有 LVGL 8 生成流程；省略该配置时也按 `8` 处理。
- `9`：调用同目录的官方 `LVGLImage.py` 生成 LVGL 9 格式。
- 仅接受整数 `8`、`9`，不接受字符串或其他版本值。

`format` 的候选名称随版本变化：

- LVGL 8：使用 `LV_IMG_CF_INDEXED_*BIT`、`LV_IMG_CF_ALPHA_*BIT`，共 8 种。
- LVGL 9：使用 9.6.0 源码中的 `LV_COLOR_FORMAT_*`，如
  `LV_COLOR_FORMAT_I4`、`LV_COLOR_FORMAT_RGB565`、`LV_COLOR_FORMAT_ARGB8888`。
  共提供本地官方 Python 转换器能生成的 20 种格式，完整候选及说明见 Schema。
- 旧版索引色、透明度名称仍可被生成器读取；编辑器在版本 9 下只推荐新名称。

网页转换器的 5 种格式是 Python 转换器支持范围的子集。
源码中的 YUV 等其他枚举不代表此脚本能够生成，不会作为候选显示。
`RAW`、`RAW_ALPHA` 保留原始文件数据，需要运行端提供对应图片解码器；
`A1/A2/A4`、`ARGB8565` 在 9.6.0 源码中标注为软件渲染不支持，
选择前应确认所用渲染后端支持。

两个版本都使用相同的输出文件名和目录，切换版本后需重新生成图片。
这项配置只控制图片资源，不会自动迁移 GUI 封装代码的 LVGL API。

生成结果输出到 `generated/`，同时自动同步 `generated/gui_lv_resource.h`。

---

## 目录结构

```
gui_resource/
├── assets/
│   ├── img_scr/        # 源图片（PNG）
│   ├── font_scr/       # 源字体文件及字符集
│   ├── img_config.json # 图片生成配置
│   └── font_config.json# 字体生成配置
├── generated/
│   ├── img/            # 生成的图片 .c 文件
│   ├── font/           # 生成的字体 .c 文件
│   └── gui_lv_resource.h # 自动同步的声明头文件
└── tools/
    ├── resource_gen.py  # 主生成脚本（按配置生成 LVGL 8/9 图片格式）
    ├── LVGLImage.py     # LVGL 9 官方图片转换脚本
    └── requirements.txt
```

## Pack mode

The pack consumes pre-generated files under `generated/` and does not require Python.

## Direct source integration

You can use the pre-generated files directly or regenerate them locally.
