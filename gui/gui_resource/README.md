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
- `lz4` — 备用依赖，当前流程未使用

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

生成结果输出到 `generated/`，同时自动同步 `generated/ui_resource.h`。

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
│   └── ui_resource.h   # 自动同步的声明头文件
└── tools/
    ├── resource_gen.py  # 主生成脚本（生成 LVGL8 格式）
    └── requirements.txt
```

## Pack mode

The pack consumes pre-generated files under `generated/` and does not require Python.

## Direct source integration

You can use the pre-generated files directly or regenerate them locally.
