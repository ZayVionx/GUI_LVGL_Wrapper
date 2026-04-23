#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Generate LVGL font/image resources from JSON configs."""

from __future__ import annotations

import argparse
import io
import json
import logging
import re
import shutil
import subprocess
import tempfile
import os
from collections import OrderedDict
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple


class ResourceError(RuntimeError):
    """Raised when resource generation fails."""


# Maps config format strings -> (bpp, LV_IMG_CF_* macro, is_indexed, is_alpha)  [LVGL8]
# is_indexed: palette + pixel-index data
# is_alpha:   alpha-only data (no palette)
IMG_FORMAT_MAP: Dict[str, Tuple[int, str, bool, bool]] = {
    "CF_ALPHA_1_BIT":             (1,  "LV_IMG_CF_ALPHA_1BIT",   False, True),
    "CF_ALPHA_2_BIT":             (2,  "LV_IMG_CF_ALPHA_2BIT",   False, True),
    "CF_ALPHA_4_BIT":             (4,  "LV_IMG_CF_ALPHA_4BIT",   False, True),
    "CF_ALPHA_8_BIT":             (8,  "LV_IMG_CF_ALPHA_8BIT",   False, True),
    "CF_INDEXED_1_BIT":           (1,  "LV_IMG_CF_INDEXED_1BIT", True,  False),
    "CF_INDEXED_2_BIT":           (2,  "LV_IMG_CF_INDEXED_2BIT", True,  False),
    "CF_INDEXED_4_BIT":           (4,  "LV_IMG_CF_INDEXED_4BIT", True,  False),
    "CF_INDEXED_8_BIT":           (8,  "LV_IMG_CF_INDEXED_8BIT", True,  False),
    "LV_IMG_CF_ALPHA_1BIT":       (1,  "LV_IMG_CF_ALPHA_1BIT",   False, True),
    "LV_IMG_CF_ALPHA_2BIT":       (2,  "LV_IMG_CF_ALPHA_2BIT",   False, True),
    "LV_IMG_CF_ALPHA_4BIT":       (4,  "LV_IMG_CF_ALPHA_4BIT",   False, True),
    "LV_IMG_CF_ALPHA_8BIT":       (8,  "LV_IMG_CF_ALPHA_8BIT",   False, True),
    "LV_IMG_CF_INDEXED_1BIT":     (1,  "LV_IMG_CF_INDEXED_1BIT", True,  False),
    "LV_IMG_CF_INDEXED_2BIT":     (2,  "LV_IMG_CF_INDEXED_2BIT", True,  False),
    "LV_IMG_CF_INDEXED_4BIT":     (4,  "LV_IMG_CF_INDEXED_4BIT", True,  False),
    "LV_IMG_CF_INDEXED_8BIT":     (8,  "LV_IMG_CF_INDEXED_8BIT", True,  False),
    "LV_IMG_CF_TRUE_COLOR":        (32, "LV_IMG_CF_TRUE_COLOR",        False, False),
    "LV_IMG_CF_TRUE_COLOR_ALPHA":  (32, "LV_IMG_CF_TRUE_COLOR_ALPHA",  False, False),
    "LV_IMG_CF_TRUE_COLOR_CHROMA": (32, "LV_IMG_CF_TRUE_COLOR_CHROMA", False, False),
    "LV_IMG_CF_RGB565A8":          (16, "LV_IMG_CF_RGB565A8",          False, False),
    "CF_TRUE_COLOR":               (32, "LV_IMG_CF_TRUE_COLOR",        False, False),
    "CF_TRUE_COLOR_ALPHA":         (32, "LV_IMG_CF_TRUE_COLOR_ALPHA",  False, False),
    "CF_TRUE_COLOR_CHROMA":        (32, "LV_IMG_CF_TRUE_COLOR_CHROMA", False, False),
    "CF_RGB565A8":                 (16, "LV_IMG_CF_RGB565A8",          False, False),
    "CF_RAW":                      (0,  "LV_IMG_CF_RAW",               False, False),
    "CF_RAW_CHROMA":               (0,  "LV_IMG_CF_RAW_CHROMA",        False, False),
    "CF_RAW_ALPHA":                (0,  "LV_IMG_CF_RAW_ALPHA",         False, False),
}


FONT_FILE_PATTERN = re.compile(r"^(lv_font_[A-Za-z0-9_]+)\.c$")
IMG_FILE_PATTERN = re.compile(r"^(lv_img_[A-Za-z0-9_]+)\.c$")


def _strip_json_comments(chRawJson: str) -> str:
    """Remove // comments while keeping content in strings."""
    lchCleaned: List[str] = []
    bInString: bool = False
    bEscaped: bool = False
    u32Index: int = 0
    u32Length: int = len(chRawJson)

    while u32Index < u32Length:
        chCurrent: str = chRawJson[u32Index]

        if bInString:
            lchCleaned.append(chCurrent)
            if bEscaped:
                bEscaped = False
            elif chCurrent == "\\":
                bEscaped = True
            elif chCurrent == '"':
                bInString = False
            u32Index += 1
            continue

        if chCurrent == '"':
            bInString = True
            lchCleaned.append(chCurrent)
            u32Index += 1
            continue

        if chCurrent == "/" and (u32Index + 1) < u32Length:
            if chRawJson[u32Index + 1] == "/":
                while u32Index < u32Length and chRawJson[u32Index] != "\n":
                    u32Index += 1
                continue

        lchCleaned.append(chCurrent)
        u32Index += 1

    return "".join(lchCleaned)


def _load_json_with_comments(ptJsonPath: Path) -> dict:
    if not ptJsonPath.exists():
        raise ResourceError(f"Config not found: {ptJsonPath}")

    chRawJson: str = ptJsonPath.read_text(encoding="utf-8")
    chCleanedJson: str = _strip_json_comments(chRawJson)

    try:
        return json.loads(chCleanedJson)
    except json.JSONDecodeError as tError:
        raise ResourceError(
            f"JSON parse failed: {ptJsonPath}: {tError}"
        ) from tError


def _resolve_input_path(ptConfigDir: Path,
                        chSubDirName: str,
                        chRawPath: str) -> Path:
    lptCandidates: List[Path] = [
        Path(chRawPath),
        ptConfigDir / chRawPath,
        ptConfigDir / chSubDirName / chRawPath,
    ]

    for ptCandidate in lptCandidates:
        if ptCandidate.exists():
            return ptCandidate.resolve()

    raise ResourceError(
        f"Input file not found: {chRawPath} "
        f"(config dir: {ptConfigDir})"
    )


def _prepare_command(lchCommand: Sequence[str]) -> List[str]:
    lchResolved: List[str] = list(lchCommand)
    chResolvedPath = shutil.which(lchResolved[0])

    if chResolvedPath is None:
        return lchResolved

    if os.name != "nt":
        lchResolved[0] = chResolvedPath
        return lchResolved

    ptResolvedPath = Path(chResolvedPath)
    chSuffix: str = ptResolvedPath.suffix.lower()

    if chSuffix in [".js", ".cjs", ".mjs"]:
        chNodePath = shutil.which("node")
        if chNodePath is None:
            raise ResourceError(
                "Required tool not found in PATH: node"
            )
        return [chNodePath, str(ptResolvedPath), *lchResolved[1:]]

    if chSuffix in [".exe", ".com", ".cmd", ".bat"]:
        lchResolved[0] = str(ptResolvedPath)
        return lchResolved

    for chExt in [".cmd", ".bat", ".exe", ".com"]:
        ptCandidate = ptResolvedPath.with_suffix(chExt)
        if ptCandidate.exists():
            lchResolved[0] = str(ptCandidate)
            return lchResolved

    lchResolved[0] = str(ptResolvedPath)
    return lchResolved


def _run_command(lchCommand: Sequence[str],
                 chDisplayName: str) -> None:
    lchResolved: List[str] = _prepare_command(lchCommand)

    tResult = subprocess.run(
        lchResolved,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8",
        errors="ignore",
        check=False,
    )

    if tResult.returncode != 0:
        chStdout: str = tResult.stdout.strip()
        chStderr: str = tResult.stderr.strip()
        raise ResourceError(
            f"Command failed ({chDisplayName})\n"
            f"cmd: {' '.join(lchResolved)}\n"
            f"stdout:\n{chStdout}\n"
            f"stderr:\n{chStderr}"
        )

def _cleanup_generated_files(ptOutputDir: Path,
                             chPrefix: str) -> None:
    ptOutputDir.mkdir(parents=True, exist_ok=True)
    for ptFile in sorted(ptOutputDir.glob(f"{chPrefix}*.c")):
        ptFile.unlink()
        logging.info("[clean] remove %s", ptFile.name)


def _strip_absolute_paths_in_file(ptFilePath: Path,
                                  lptPaths: Sequence[Path]) -> None:
    """Replace absolute paths with bare filenames in generated .c files."""
    chContent: str = ptFilePath.read_text(encoding="utf-8")
    for ptPath in lptPaths:
        chContent = chContent.replace(str(ptPath), ptPath.name)
    chContent = _reformat_opts_comment(chContent)
    ptFilePath.write_text(chContent, encoding="utf-8")


def _reformat_opts_comment(chContent: str) -> str:
    """Split Opts comment into 3 groups, wrap --symbols at 100 chars."""
    tMatch = re.search(
        r"( \* Opts: )(.*?)( --symbols )(.*?)( --format )(.*)",
        chContent,
    )
    if tMatch is None:
        return chContent

    chPrefix:  str = tMatch.group(1)
    chBefore:  str = tMatch.group(2)
    chSymFlag: str = tMatch.group(3).strip()
    chSymbols: str = tMatch.group(4)
    chFmtFlag: str = tMatch.group(5).strip()
    chAfter:   str = tMatch.group(6)

    chIndent: str = " *   "
    u32MaxWidth: int = 50

    lchSymLines: List[str] = []
    for u32Start in range(0, len(chSymbols), u32MaxWidth):
        lchSymLines.append(
            chSymbols[u32Start: u32Start + u32MaxWidth]
        )

    lchResult: List[str] = [
        f"{chPrefix}{chBefore}",
        f"{chIndent}{chSymFlag}",
    ]
    for chLine in lchSymLines:
        lchResult.append(f"{chIndent}  {chLine}")
    lchResult.append(f"{chIndent}{chFmtFlag} {chAfter}")

    chOldLine: str = tMatch.group(0)
    chNewBlock: str = "\n".join(lchResult)
    return chContent.replace(chOldLine, chNewBlock)


def _build_unique_charset(lptTextFiles: Iterable[Path]) -> Tuple[str, int]:
    setSeenChars = set()
    lchMerged: List[str] = []

    for ptTextFile in lptTextFiles:
        chContent: str = ptTextFile.read_text(encoding="utf-8")

        for chChar in chContent:
            if chChar in {"\r", "\n", "\ufeff"}:
                continue
            if chChar not in setSeenChars:
                setSeenChars.add(chChar)
                lchMerged.append(chChar)

    chMerged: str = "".join(lchMerged)
    return chMerged, len(chMerged)


def _ensure_tool(chToolName: str) -> None:
    chResolvedPath = shutil.which(chToolName)
    if chResolvedPath is None:
        raise ResourceError(
            f"Required tool not found in PATH: {chToolName}"
        )

    if os.name != "nt":
        return

    chSuffix: str = Path(chResolvedPath).suffix.lower()
    if chSuffix in [".js", ".cjs", ".mjs"]:
        if shutil.which("node") is None:
            raise ResourceError(
                "Required tool not found in PATH: node"
            )


def _validate_fields(tItem: dict,
                     lchRequiredKeys: Sequence[str],
                     chSectionName: str,
                     u32Index: int) -> None:
    for chKey in lchRequiredKeys:
        if chKey not in tItem:
            raise ResourceError(
                f"Missing key '{chKey}' in {chSectionName}[{u32Index}]"
            )


def _generate_fonts(ptFontConfigPath: Path,
                    ptFontOutputDir: Path) -> None:
    _ensure_tool("lv_font_conv")

    tFontConfig: dict = _load_json_with_comments(ptFontConfigPath)
    ltFontItems = tFontConfig.get("font", [])

    if not isinstance(ltFontItems, list):
        raise ResourceError("font config field 'font' must be a list")

    _cleanup_generated_files(ptFontOutputDir, "lv_font_")

    ptConfigDir: Path = ptFontConfigPath.parent
    dtGroupMap: "OrderedDict[Tuple[str, str, str, str], dict]" = OrderedDict()

    for u32Index, tFontItem in enumerate(ltFontItems):
        if not isinstance(tFontItem, dict):
            raise ResourceError(f"font[{u32Index}] must be an object")

        _validate_fields(
            tFontItem,
            ["name", "text", "source", "bpp", "size"],
            "font",
            u32Index,
        )

        chName: str = str(tFontItem["name"]).strip()
        chText: str = str(tFontItem["text"]).strip()
        chSource: str = str(tFontItem["source"]).strip()
        chBpp: str = str(tFontItem["bpp"]).strip()
        chSize: str = str(tFontItem["size"]).strip()

        ptTextPath: Path = _resolve_input_path(ptConfigDir, "font_scr", chText)
        ptSourcePath: Path = _resolve_input_path(
            ptConfigDir,
            "font_scr",
            chSource,
        )

        tGroupKey = (chName, str(ptSourcePath), chBpp, chSize)
        if tGroupKey not in dtGroupMap:
            dtGroupMap[tGroupKey] = {
                "name": chName,
                "source": ptSourcePath,
                "bpp": chBpp,
                "size": chSize,
                "texts": [],
            }

        dtGroupMap[tGroupKey]["texts"].append(ptTextPath)

    with tempfile.TemporaryDirectory(prefix="lv_font_merge_") as chTempDir:
        ptTempDir = Path(chTempDir)

        for tGroup in dtGroupMap.values():
            chName = tGroup["name"]
            chSize = tGroup["size"]
            chBpp = tGroup["bpp"]
            ptSourcePath = tGroup["source"]
            lptTextFiles = tGroup["texts"]

            chMergedCharset, u32UniqueCharCount = _build_unique_charset(
                lptTextFiles
            )
            chOutputBase: str = f"lv_font_{chName}_{chSize}"
            ptTempTextPath: Path = ptTempDir / f"{chOutputBase}.txt"
            ptOutputPath: Path = ptFontOutputDir / f"{chOutputBase}.c"

            ptTempTextPath.write_text(chMergedCharset, encoding="utf-8")

            logging.info(
                "[font] 合并%d个字体配置，去重后字符数：%d -> %s",
                len(lptTextFiles),
                u32UniqueCharCount,
                ptOutputPath.name,
            )

            lchCommand: List[str] = [
                "lv_font_conv",
                "--bpp",
                chBpp,
                "--size",
                chSize,
                "--no-compress",
                "--no-prefilter",
                "--font",
                str(ptSourcePath),
                "--symbols",
                chMergedCharset,
                "--format",
                "lvgl",
                "-o",
                str(ptOutputPath),
                "--force-fast-kern-format",
            ]

            _run_command(lchCommand, chOutputBase)

            _strip_absolute_paths_in_file(
                ptOutputPath,
                [ptSourcePath, ptOutputPath],
            )
            _fix_font_includes(ptOutputPath)

            u32FileSize: int = ptOutputPath.stat().st_size
            logging.info(
                "[font] generated %s (%d bytes)",
                ptOutputPath.name,
                u32FileSize,
            )


def _pngquant_quantize(ptInputPath: Path, u32Colors: int) -> bytes:
    """Run pngquant on ptInputPath, return quantized PNG bytes (stdout)."""
    chPngquant = shutil.which("pngquant")
    if chPngquant is None:
        raise ResourceError(
            "pngquant not found in PATH. "
            "Windows: download from https://pngquant.org/ and add to PATH. "
            "Linux: sudo apt install pngquant  "
            "macOS: brew install pngquant"
        )
    tResult = subprocess.run(
        [chPngquant, str(u32Colors), "--force", "--output", "-", "--", str(ptInputPath)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if tResult.returncode not in (0, 98, 99):  # 98/99 = minor quality warnings
        raise ResourceError(
            f"pngquant failed for {ptInputPath.name}: "
            f"{tResult.stderr.decode(errors='replace').strip()}"
        )
    return tResult.stdout


def _png_bytes_to_indexed(lu8PngBytes: bytes,
                          u32Bpp: int) -> Tuple[int, int, bytearray]:
    """Parse a palette-mode PNG and return (w, h, lvgl8_data).

    lvgl8_data layout (same as LVGL8 online converter):
      - palette: (1<<bpp) * 4 bytes, each entry = [B, G, R, A]
      - pixels:  packed indices, MSB-first, row-padded to byte boundary
    """
    try:
        import png  # pypng
    except ImportError as e:
        raise ResourceError(
            "Missing dependency 'pypng', install with: pip install pypng"
        ) from e

    reader = png.Reader(bytes=lu8PngBytes)
    u32Width, u32Height, tRows, tMeta = reader.read()

    if not tMeta.get("palette"):
        raise ResourceError("pngquant output is not palette mode")

    # palette from pypng: list of (R,G,B) or (R,G,B,A)
    ltPalette = tMeta["palette"]
    u32PaletteSize = 1 << u32Bpp

    lu8LvglPalette = bytearray()
    for u32I in range(u32PaletteSize):
        if u32I < len(ltPalette):
            tEntry = ltPalette[u32I]
            u8R, u8G, u8B = int(tEntry[0]), int(tEntry[1]), int(tEntry[2])
            u8A = int(tEntry[3]) if len(tEntry) >= 4 else 255
            # Normalize fully-transparent entries: force RGB to (0,0,0)
            # pngquant may output garbage RGB when A=0
            if u8A == 0:
                u8R, u8G, u8B = 0, 0, 0
        else:
            # Unused palette slots: transparent padding (matches LVGL8 online converter)
            u8R, u8G, u8B, u8A = 0, 0, 0, 0
        lu8LvglPalette.extend([u8B, u8G, u8R, u8A])  # LVGL8: B,G,R,A

    # pack pixel rows MSB-first
    u32PixPerByte = 8 // u32Bpp
    u32Mask = (1 << u32Bpp) - 1
    lu8Pixels = bytearray()
    for lu8Row in tRows:
        for u32Col in range(0, u32Width, u32PixPerByte):
            u8Byte = 0
            for u32Sub in range(u32PixPerByte):
                u32Pos = u32Col + u32Sub
                u8Idx = int(lu8Row[u32Pos]) if u32Pos < u32Width else 0
                u8Byte |= (u8Idx & u32Mask) << (8 - u32Bpp * (u32Sub + 1))
            lu8Pixels.append(u8Byte)

    return u32Width, u32Height, lu8LvglPalette + lu8Pixels


def _png_to_alpha_data(ptInputPath: Path,
                       u32Bpp: int) -> Tuple[int, int, bytearray]:
    """Extract alpha channel, quantize to u32Bpp bits, pack MSB-first."""
    try:
        from PIL import Image
    except ImportError as e:
        raise ResourceError(
            "Missing dependency 'Pillow', install with: pip install pillow"
        ) from e

    tImg = Image.open(ptInputPath).convert("RGBA")
    u32Width, u32Height = tImg.size
    u32Levels = 1 << u32Bpp
    u32PixPerByte = 8 // u32Bpp
    u32Mask = (1 << u32Bpp) - 1

    lu8Pixels = bytearray()
    lu8Alpha = [tImg.getpixel((x, y))[3] for y in range(u32Height) for x in range(u32Width)]

    for u32Row in range(u32Height):
        for u32Col in range(0, u32Width, u32PixPerByte):
            u8Byte = 0
            for u32Sub in range(u32PixPerByte):
                u32Pos = u32Row * u32Width + u32Col + u32Sub
                if u32Pos < len(lu8Alpha):
                    u8Idx = int(lu8Alpha[u32Pos]) * (u32Levels - 1) // 255
                else:
                    u8Idx = 0
                u8Byte |= (u8Idx & u32Mask) << (8 - u32Bpp * (u32Sub + 1))
            lu8Pixels.append(u8Byte)

    return u32Width, u32Height, lu8Pixels


def _write_lvgl8_c_file(ptOutputPath: Path,
                        chVarName: str,
                        chCfMacro: str,
                        u32Width: int,
                        u32Height: int,
                        lu8Data: bytearray) -> None:
    """Write a LVGL8-compatible .c image file."""
    chAttr = f"LV_ATTRIBUTE_IMG_{chVarName.upper()}"
    # format data: 15 bytes per line
    lchLines: List[str] = []
    for u32Start in range(0, len(lu8Data), 15):
        chLine = ", ".join(f"0x{b:02x}" for b in lu8Data[u32Start:u32Start + 15])
        lchLines.append(f"  {chLine}, ")

    lchContent = [
        "#ifdef __has_include",
        "    #if __has_include(\"lvgl.h\")",
        "        #ifndef LV_LVGL_H_INCLUDE_SIMPLE",
        "            #define LV_LVGL_H_INCLUDE_SIMPLE",
        "        #endif",
        "    #endif",
        "#endif",
        "",
        "#if defined(LV_LVGL_H_INCLUDE_SIMPLE)",
        "    #include \"lvgl.h\"",
        "#else",
        "    #include \"lvgl/lvgl.h\"",
        "#endif",
        "",
        "#ifndef LV_ATTRIBUTE_MEM_ALIGN",
        "#define LV_ATTRIBUTE_MEM_ALIGN",
        "#endif",
        "",
        f"#ifndef {chAttr}",
        f"#define {chAttr}",
        "#endif",
        "",
        f"const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST {chAttr} uint8_t {chVarName}_map[] = {{",
        *lchLines,
        "};",
        "",
        f"const lv_img_dsc_t {chVarName} = {{",
        f"  .header.cf = {chCfMacro},",
        "  .header.always_zero = 0,",
        "  .header.reserved = 0,",
        f"  .header.w = {u32Width},",
        f"  .header.h = {u32Height},",
        f"  .data_size = {len(lu8Data)},",
        f"  .data = {chVarName}_map,",
        "};",
        "",
    ]
    ptOutputPath.write_text("\n".join(lchContent), encoding="utf-8")


def _generate_images(ptImgConfigPath: Path,
                     ptImgOutputDir: Path) -> None:
    tImgConfig: dict = _load_json_with_comments(ptImgConfigPath)
    ltImgItems = tImgConfig.get("img", [])

    if not isinstance(ltImgItems, list):
        raise ResourceError("img config field 'img' must be a list")

    _cleanup_generated_files(ptImgOutputDir, "lv_img_")
    ptConfigDir: Path = ptImgConfigPath.parent

    for u32Index, tImgItem in enumerate(ltImgItems):
        if not isinstance(tImgItem, dict):
            raise ResourceError(f"img[{u32Index}] must be an object")

        _validate_fields(tImgItem, ["name", "file", "format"], "img", u32Index)

        chName: str = str(tImgItem["name"]).strip()
        chFile: str = str(tImgItem["file"]).strip()
        chFormat: str = str(tImgItem["format"]).strip().upper()
        chOutputName: str = f"lv_img_{chName}"

        if chFormat not in IMG_FORMAT_MAP:
            raise ResourceError(
                f"Unsupported image format '{chFormat}' in img[{u32Index}]"
            )

        u32Bpp, chCfMacro, bIndexed, bAlpha = IMG_FORMAT_MAP[chFormat]
        ptInputPath: Path = _resolve_input_path(ptConfigDir, "img_scr", chFile)
        ptOutputPath: Path = ptImgOutputDir / f"{chOutputName}.c"

        logging.info("[img] generate %s from %s (%s)",
                     ptOutputPath.name, ptInputPath.name, chFormat)

        if bIndexed:
            # quantize with pngquant (same as LVGL8 online converter)
            lu8PngBytes = _pngquant_quantize(ptInputPath, 1 << u32Bpp)
            u32W, u32H, lu8Data = _png_bytes_to_indexed(lu8PngBytes, u32Bpp)
        elif bAlpha:
            u32W, u32H, lu8Data = _png_to_alpha_data(ptInputPath, u32Bpp)
        else:
            raise ResourceError(
                f"Format '{chFormat}' (true-color) is not yet supported"
            )

        _write_lvgl8_c_file(ptOutputPath, chOutputName, chCfMacro,
                            u32W, u32H, lu8Data)

        logging.info("[img] generated %s (%d bytes)",
                     ptOutputPath.name, ptOutputPath.stat().st_size)


def _collect_declarations(ptDir: Path,
                          tPattern: re.Pattern[str]) -> List[str]:
    lchNames: List[str] = []

    if not ptDir.exists():
        return lchNames

    for ptFile in sorted(ptDir.glob("*.c")):
        tMatch = tPattern.match(ptFile.name)
        if tMatch is not None:
            lchNames.append(tMatch.group(1))

    return lchNames


def _write_ui_resource_header(ptHeaderPath: Path,
                              lchFontNames: Sequence[str],
                              lchImgNames: Sequence[str]) -> None:
    lchLines: List[str] = [
        "#ifndef __GUI_LV_RESOURCE_H__",
        "#define __GUI_LV_RESOURCE_H__",
        "",
        "/*================================= INCLUDES "
        "=================================*/",        
        "",
        "#ifdef   __cplusplus",
        "extern \"C\" {",
        "#endif",
        "/*=================================== FONT "
        "===================================*/",
    ]

    for chFontName in lchFontNames:
        lchLines.append(f"LV_FONT_DECLARE({chFontName})")

    if len(lchFontNames) == 0:
        lchLines.append("// LV_FONT_DECLARE(lv_font_Roboto_Bold_70)")

    lchLines.extend([
        "",
        "/*=================================== IMAGE "
        "==================================*/",
    ])

    for chImgName in lchImgNames:
        lchLines.append(f"LV_IMG_DECLARE({chImgName})")

    if len(lchImgNames) == 0:
        lchLines.append("// LV_IMG_DECLARE(lv_img_qr_code)")

    lchLines.extend([
        "",
        "/*=================================== END "
        "====================================*/",
        "#ifdef   __cplusplus",
        "}",
        "#endif",
        "#endif  /* __GUI_LV_RESOURCE_H__ */",
        "",
    ])

    ptHeaderPath.write_text("\n".join(lchLines), encoding="utf-8")


def _fix_font_includes(ptFilePath: Path) -> None:
    """Replace the legacy LV_LVGL_H_INCLUDE_SIMPLE guard with a portable
    __has_include-based guard so the file works in all three usage modes
    (bundled MCU, bundled PC, and pack/external LVGL)."""

    chOld = (
        "#ifdef LV_LVGL_H_INCLUDE_SIMPLE\n"
        "#include \"lvgl.h\"\n"
        "#else\n"
        "#include \"lvgl/lvgl.h\"\n"
        "#endif"
    )
    chNew = (
        "#ifdef __has_include\n"
        "    #if __has_include(\"lvgl.h\")\n"
        "        #ifndef LV_LVGL_H_INCLUDE_SIMPLE\n"
        "            #define LV_LVGL_H_INCLUDE_SIMPLE\n"
        "        #endif\n"
        "    #endif\n"
        "#endif\n"
        "\n"
        "#if defined(LV_LVGL_H_INCLUDE_SIMPLE)\n"
        "    #include \"lvgl.h\"\n"
        "#else\n"
        "    #include \"lvgl/lvgl.h\"\n"
        "#endif"
    )
    chContent: str = ptFilePath.read_text(encoding="utf-8")
    if chOld in chContent:
        chContent = chContent.replace(chOld, chNew, 1)
        ptFilePath.write_text(chContent, encoding="utf-8")



def _build_parser(ptDefaultResourceDir: Path) -> argparse.ArgumentParser:
    ptDefaultAssetsDir:    Path = ptDefaultResourceDir / "assets"
    ptDefaultGeneratedDir: Path = ptDefaultResourceDir / "generated"

    tParser = argparse.ArgumentParser(
        description="Generate LVGL fonts/images and sync gui_lv_resource.h",
    )
    tParser.add_argument("--font", action="store_true", help="Generate fonts")
    tParser.add_argument("--img", action="store_true", help="Generate images")
    tParser.add_argument(
        "--font-config",
        default=str(ptDefaultAssetsDir / "font_config.json"),
        help="font config json path",
    )
    tParser.add_argument(
        "--img-config",
        default=str(ptDefaultAssetsDir / "img_config.json"),
        help="image config json path",
    )
    tParser.add_argument(
        "--font-out",
        default=str(ptDefaultGeneratedDir / "font"),
        help="font output directory",
    )
    tParser.add_argument(
        "--img-out",
        default=str(ptDefaultGeneratedDir / "img"),
        help="image output directory",
    )
    tParser.add_argument(
        "--header",
        default=str(ptDefaultGeneratedDir / "gui_lv_resource.h"),
        help="gui_lv_resource.h path",
    )
    return tParser


def main() -> int:
    logging.basicConfig(level=logging.INFO, format="%(message)s")

    ptScriptPath: Path = Path(__file__).resolve()
    ptResourceDir: Path = ptScriptPath.parent.parent

    tParser = _build_parser(ptResourceDir)
    tArgs = tParser.parse_args()

    bDoFont: bool = bool(tArgs.font)
    bDoImg: bool = bool(tArgs.img)

    if (not bDoFont) and (not bDoImg):
        bDoFont = True
        bDoImg = True

    ptFontConfigPath = Path(tArgs.font_config).resolve()
    ptImgConfigPath = Path(tArgs.img_config).resolve()
    ptFontOutDir = Path(tArgs.font_out).resolve()
    ptImgOutDir = Path(tArgs.img_out).resolve()
    ptHeaderPath = Path(tArgs.header).resolve()
    try:
        if bDoFont:
            logging.info("[font] start")
            _generate_fonts(ptFontConfigPath, ptFontOutDir)

        if bDoImg:
            logging.info("[img] start")
            _generate_images(ptImgConfigPath, ptImgOutDir)

        lchFontNames = _collect_declarations(ptFontOutDir, FONT_FILE_PATTERN)
        lchImgNames = _collect_declarations(ptImgOutDir, IMG_FILE_PATTERN)

        _write_ui_resource_header(ptHeaderPath, lchFontNames, lchImgNames)
        logging.info(
            "[header] synced %s (font=%d, img=%d)",
            ptHeaderPath.name,
            len(lchFontNames),
            len(lchImgNames),
        )

    except ResourceError as tError:
        logging.error("[error] %s", tError)
        return 1

    logging.info("[done] resource generation completed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
