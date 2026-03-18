#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Generate LVGL font/image resources from JSON configs."""

from __future__ import annotations

import argparse
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


IMG_FORMAT_MAP: Dict[str, Tuple[int, str]] = {
    "CF_INDEXED_1_BIT": (1, "LV_IMG_CF_INDEXED_1BIT"),
    "CF_INDEXED_2_BIT": (2, "LV_IMG_CF_INDEXED_2BIT"),
    "CF_INDEXED_4_BIT": (4, "LV_IMG_CF_INDEXED_4BIT"),
    "CF_INDEXED_8_BIT": (8, "LV_IMG_CF_INDEXED_8BIT"),
    "LV_IMG_CF_INDEXED_1BIT": (1, "LV_IMG_CF_INDEXED_1BIT"),
    "LV_IMG_CF_INDEXED_2BIT": (2, "LV_IMG_CF_INDEXED_2BIT"),
    "LV_IMG_CF_INDEXED_4BIT": (4, "LV_IMG_CF_INDEXED_4BIT"),
    "LV_IMG_CF_INDEXED_8BIT": (8, "LV_IMG_CF_INDEXED_8BIT"),
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


def _resolve_alpha_for_palette(u32PaletteIndex: int,
                               vTransparency) -> int:
    if vTransparency is None:
        return 255

    if isinstance(vTransparency, bytes):
        if u32PaletteIndex < len(vTransparency):
            return int(vTransparency[u32PaletteIndex])
        return 255

    if isinstance(vTransparency, list):
        if u32PaletteIndex < len(vTransparency):
            return int(vTransparency[u32PaletteIndex])
        return 255

    if isinstance(vTransparency, int):
        if u32PaletteIndex == vTransparency:
            return 0
        return 255

    return 255


def _pack_index_rows(lu8Indices: Sequence[int],
                     u32Width: int,
                     u32Height: int,
                     u32Bpp: int) -> bytearray:
    u32PixelsPerByte: int = 8 // u32Bpp
    u32Mask: int = (1 << u32Bpp) - 1
    lu8Packed = bytearray()

    for u32Row in range(u32Height):
        u32RowStart: int = u32Row * u32Width
        lu8Row = lu8Indices[u32RowStart: u32RowStart + u32Width]

        if u32Bpp == 8:
            lu8Packed.extend(lu8Row)
            continue

        for u32Col in range(0, u32Width, u32PixelsPerByte):
            u8PackedByte: int = 0
            for u32Index in range(u32PixelsPerByte):
                u32PixelPos: int = u32Col + u32Index
                u8PaletteIndex: int = 0
                if u32PixelPos < u32Width:
                    u8PaletteIndex = int(lu8Row[u32PixelPos])

                u32Shift: int = 8 - (u32Bpp * (u32Index + 1))
                u8PackedByte |= (u8PaletteIndex & u32Mask) << u32Shift

            lu8Packed.append(u8PackedByte)

    return lu8Packed


def _convert_png_to_indexed_data(ptImagePath: Path,
                                 u32Bpp: int) -> Tuple[int, int, List[int]]:
    try:
        from PIL import Image  # type: ignore[import-not-found]
    except ImportError as tError:
        raise ResourceError(
            "Missing dependency 'Pillow', install with: "
            "python -m pip install pillow"
        ) from tError

    u32PaletteSize: int = 1 << u32Bpp
    tSourceImage = Image.open(ptImagePath).convert("RGBA")
    u32Width: int = tSourceImage.width
    u32Height: int = tSourceImage.height

    tIndexedImage = tSourceImage.quantize(
        colors=u32PaletteSize,
        method=Image.Quantize.FASTOCTREE,
        dither=Image.Dither.NONE,
    )

    if tIndexedImage.mode != "P":
        raise ResourceError(f"Indexed convert failed for: {ptImagePath}")

    lu8PaletteRgb = tIndexedImage.getpalette() or []
    vTransparency = tIndexedImage.info.get("transparency")

    lu8LvglPalette = bytearray()
    for u32PaletteIndex in range(u32PaletteSize):
        u32RgbBase: int = u32PaletteIndex * 3
        if (u32RgbBase + 2) < len(lu8PaletteRgb):
            u8Red: int = int(lu8PaletteRgb[u32RgbBase + 0])
            u8Green: int = int(lu8PaletteRgb[u32RgbBase + 1])
            u8Blue: int = int(lu8PaletteRgb[u32RgbBase + 2])
        else:
            u8Red = 255
            u8Green = 255
            u8Blue = 255

        u8Alpha: int = _resolve_alpha_for_palette(
            u32PaletteIndex,
            vTransparency,
        )

        # LVGL indexed palette uses B, G, R, A order.
        lu8LvglPalette.extend([u8Blue, u8Green, u8Red, u8Alpha])

    lu8Indices: List[int] = list(tIndexedImage.getdata())
    lu8Packed = _pack_index_rows(lu8Indices, u32Width, u32Height, u32Bpp)
    lu8Data: List[int] = list(lu8LvglPalette + lu8Packed)
    return u32Width, u32Height, lu8Data


def _format_hex_data_lines(lu8Data: Sequence[int],
                           u32LineWidth: int = 12) -> List[str]:
    lchLines: List[str] = []

    for u32Start in range(0, len(lu8Data), u32LineWidth):
        lu8Chunk = lu8Data[u32Start: u32Start + u32LineWidth]
        chLineBody: str = ", ".join(f"0x{u8Byte:02x}" for u8Byte in lu8Chunk)
        lchLines.append(f"  {chLineBody},")

    return lchLines


def _write_lvgl8_image_file(ptOutputPath: Path,
                            chVarName: str,
                            chImgCfMacro: str,
                            u32Width: int,
                            u32Height: int,
                            lu8Data: Sequence[int]) -> None:
    chAttrMacro: str = f"LV_ATTRIBUTE_IMG_{chVarName.upper()}"
    lchDataLines: List[str] = _format_hex_data_lines(lu8Data)

    lchContent: List[str] = [
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
        f"#ifndef {chAttrMacro}",
        f"#define {chAttrMacro}",
        "#endif",
        "",
        "const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST "
        f"{chAttrMacro} uint8_t {chVarName}_map[] = {{",
        *lchDataLines,
        "};",
        "",
        f"const lv_img_dsc_t {chVarName} = {{",
        f"  .header.cf = {chImgCfMacro},",
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

        _validate_fields(
            tImgItem,
            ["name", "file", "format"],
            "img",
            u32Index,
        )

        chName: str = str(tImgItem["name"]).strip()
        chFile: str = str(tImgItem["file"]).strip()
        chFormat: str = str(tImgItem["format"]).strip().upper()
        chOutputName: str = f"lv_img_{chName}"

        if chFormat not in IMG_FORMAT_MAP:
            raise ResourceError(
                f"Unsupported image format '{chFormat}' in img[{u32Index}]"
            )

        u32Bpp, chImgCfMacro = IMG_FORMAT_MAP[chFormat]
        ptInputPath: Path = _resolve_input_path(ptConfigDir, "img_scr", chFile)
        ptFinalOutputPath: Path = ptImgOutputDir / f"{chOutputName}.c"

        logging.info(
            "[img] generate %s from %s (%s)",
            ptFinalOutputPath.name,
            ptInputPath.name,
            chFormat,
        )

        u32Width, u32Height, lu8Data = _convert_png_to_indexed_data(
            ptInputPath,
            u32Bpp,
        )
        _write_lvgl8_image_file(
            ptFinalOutputPath,
            chOutputName,
            chImgCfMacro,
            u32Width,
            u32Height,
            lu8Data,
        )
        u32FileSize: int = ptFinalOutputPath.stat().st_size
        logging.info(
            "[img] generated %s (%d bytes)",
            ptFinalOutputPath.name,
            u32FileSize,
        )


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
        "#ifndef __UI_RESOURCE_H__",
        "#define __UI_RESOURCE_H__",
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
        "#endif  /* __UI_RESOURCE_H__ */",
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
        description="Generate LVGL fonts/images and sync ui_resource.h",
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
        default=str(ptDefaultGeneratedDir / "ui_resource.h"),
        help="ui_resource.h path",
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
