#!/usr/bin/env python3
"""Save and restore per-project GUI_LV configuration profiles."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import sys
from datetime import datetime
from pathlib import Path


CONFIG_FILES = [
    "gui_lvgl.c",
    "gui_lvgl.h",
    "gui_lv_font.h",
    "gui_lv_sys_data.c",
    "gui_lv_sys_data.h",
    "gui_lv_scene_id.h",
    "gui_lv_conf.h",
    "gui_lv_custom_components.c",
    "gui_lv_custom_components.h",
    "platform/gui_lv_user_arch_port.h",
]

PROFILE_META = "gui_lv_profile.json"
STATUS_FILE = ".gui_lv_current_project.json"
RTE_RELATIVE_CANDIDATES = [
    Path("RTE") / "GUI_LVGL_Wrapper",
    Path("MDK-ARM") / "RTE" / "GUI_LVGL_Wrapper",
]


class ProfileError(RuntimeError):
    pass


def timestamp() -> str:
    return datetime.now().strftime("%Y%m%d_%H%M%S")


def now_iso() -> str:
    return datetime.now().astimezone().isoformat(timespec="seconds")


def script_path() -> Path:
    return Path(__file__).resolve()


def profile_root() -> Path:
    return script_path().parents[1]


def repo_root() -> Path:
    return profile_root().parents[1]


def source_gui_lv_dir() -> Path:
    return repo_root() / "gui_lv"


def profile_dir(project: str) -> Path:
    return profile_root() / f"gui_lv_{project}"


def backup_root() -> Path:
    return profile_root() / "_backup"


def validate_project_name(project: str) -> None:
    if not project:
        raise ProfileError("project name is empty")
    allowed = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-")
    bad = sorted({ch for ch in project if ch not in allowed})
    if bad:
        raise ProfileError(
            "project name can only contain letters, numbers, '_' and '-'; "
            f"invalid: {''.join(bad)}"
        )


def ensure_dir(path: Path, label: str) -> None:
    if not path.is_dir():
        raise ProfileError(f"{label} does not exist: {path}")


def missing_files(base: Path) -> list[str]:
    return [rel for rel in CONFIG_FILES if not (base / rel).is_file()]


def require_complete_config(base: Path, label: str) -> None:
    missing = missing_files(base)
    if missing:
        joined = "\n  ".join(missing)
        raise ProfileError(f"{label} is missing required files:\n  {joined}")


def copy_config_files(src: Path, dst: Path) -> None:
    for rel in CONFIG_FILES:
        src_file = src / rel
        dst_file = dst / rel
        dst_file.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src_file, dst_file)


def write_json(path: Path, data: dict) -> None:
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


def read_json(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def use_color() -> bool:
    return sys.stdout.isatty() and os.environ.get("NO_COLOR") is None


def paint(text: object, code: str) -> str:
    value = str(text)
    if not use_color():
        return value
    return f"\033[{code}m{value}\033[0m"


def cyan(text: object) -> str:
    return paint(text, "36;1")


def green(text: object) -> str:
    return paint(text, "32;1")


def yellow(text: object) -> str:
    return paint(text, "33;1")


def dim(text: object) -> str:
    return paint(text, "2")


def shorten_path(path: object) -> str:
    raw = str(path)
    try:
        home = str(Path.home())
        if raw.lower().startswith(home.lower()):
            raw = "~" + raw[len(home):]
    except OSError:
        pass
    return raw


def code(text: object) -> str:
    return f"`{text}`"


def bold(text: object) -> str:
    return f"**{text}**"


def print_title(title: str, subtitle: str | None = None) -> None:
    print()
    print(cyan(f"## {title}"))
    print(dim("-" * 72))
    if subtitle:
        print(dim(subtitle))


def label_text(label: str, width: int = 12) -> str:
    return f"{label:<{width}}:"


def print_field(label: str, value: object) -> None:
    print(f"  {dim(label_text(label))} {value}")


def print_hint(text: str) -> None:
    print(f"\n{yellow(bold('Hint:'))} {text}")


def print_result(label: str, value: object, ok: bool = True) -> None:
    tag = green(bold("[OK]")) if ok else yellow(bold("[WARN]"))
    print(f"  {tag} {label:<10} {value}")


def print_summary(label: str, value: object, ok: bool = True) -> None:
    tag = green(bold("[OK]")) if ok else yellow(bold("[WARN]"))
    print(f"{tag} {label}: {value}")


def print_error(message: object) -> None:
    print_title("GUI_LV Profile Error")
    print_summary("Error", yellow(bold(message)), ok=False)


def print_rule() -> None:
    print(dim("-" * 72))


def print_profile_card(project: str, mode: str, saved_at: str, path: Path) -> None:
    profile_name = f"gui_lv_{project}"
    mode_badge = cyan(mode.upper()) if mode != "-" else "-"
    print()
    print(f"- {green(bold(project))} {dim('(' + profile_name + ')')}")
    print(f"  {label_text('Mode')} {mode_badge}")
    print(f"  {label_text('Saved')} {saved_at}")
    print(f"  {label_text('Path')} {code(shorten_path(path))}")


def make_backup(target: Path, project: str, action: str) -> Path | None:
    existing = [rel for rel in CONFIG_FILES if (target / rel).is_file()]
    if not existing:
        return None

    backup_dir = backup_root() / f"{timestamp()}_{action}_{project}"
    for rel in existing:
        src_file = target / rel
        dst_file = backup_dir / rel
        dst_file.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src_file, dst_file)

    status = target / STATUS_FILE
    if status.is_file():
        shutil.copy2(status, backup_dir / STATUS_FILE)

    return backup_dir


def backup_existing_profile(project: str, path: Path) -> Path | None:
    if not path.exists():
        return None
    backup_dir = backup_root() / f"{timestamp()}_profile_{project}"
    backup_dir.parent.mkdir(parents=True, exist_ok=True)
    shutil.copytree(path, backup_dir)
    shutil.rmtree(path)
    return backup_dir


def find_rte_dir(start: Path) -> Path:
    current = start.resolve()
    if current.is_file():
        current = current.parent

    hits: list[Path] = []
    for base in [current, *current.parents]:
        for rel in RTE_RELATIVE_CANDIDATES:
            candidate = base / rel
            if candidate.is_dir() and candidate not in hits:
                hits.append(candidate)

    if not hits:
        raise ProfileError(
            "RTE/GUI_LVGL_Wrapper was not found. "
            "Run this command from the Keil project directory or one of its subdirectories."
        )
    if len(hits) > 1:
        joined = "\n  ".join(str(path) for path in hits)
        raise ProfileError(f"multiple RTE GUI_LVGL_Wrapper directories found:\n  {joined}")
    return hits[0]


def resolve_target(use_rte: bool) -> Path:
    if use_rte:
        return find_rte_dir(Path.cwd())
    return source_gui_lv_dir()


def save_project(project: str, use_rte: bool) -> None:
    validate_project_name(project)
    src = resolve_target(use_rte)
    ensure_dir(src, "source GUI_LV directory")
    require_complete_config(src, "source GUI_LV directory")

    dst = profile_dir(project)
    backup = backup_existing_profile(project, dst)
    dst.mkdir(parents=True, exist_ok=True)
    copy_config_files(src, dst)

    write_json(
        dst / PROFILE_META,
        {
            "project": project,
            "profile": f"gui_lv_{project}",
            "saved_at": now_iso(),
            "mode": "RTE" if use_rte else "source",
            "source": str(src),
            "files": CONFIG_FILES,
        },
    )

    print_title("GUI_LV Profile Saved")
    print_summary("Saved", green(bold(f"gui_lv_{project}")))
    print_field("Project", green(project))
    print_field("Mode", "RTE" if use_rte else "source")
    print_field("Source", code(shorten_path(src)))
    print_field("Profile", code(shorten_path(dst)))
    print_field("Files", len(CONFIG_FILES))
    if backup:
        print_field("Backup", code(shorten_path(backup)))


def write_project(project: str, use_rte: bool) -> None:
    validate_project_name(project)
    src = profile_dir(project)
    ensure_dir(src, "profile")
    require_complete_config(src, "profile")

    dst = resolve_target(use_rte)
    ensure_dir(dst, "target GUI_LV directory")

    backup = make_backup(dst, project, "write")
    copy_config_files(src, dst)
    write_json(
        dst / STATUS_FILE,
        {
            "project": project,
            "profile": f"gui_lv_{project}",
            "written_at": now_iso(),
            "mode": "RTE" if use_rte else "source",
            "profile_path": str(src),
            "target": str(dst),
            "files": CONFIG_FILES,
        },
    )

    print_title("GUI_LV Profile Written")
    print_summary("Written", green(bold(project)))
    print_field("Mode", "RTE" if use_rte else "source")
    print_field("Target", code(shorten_path(dst)))
    print_field("Profile", code(shorten_path(src)))
    print_field("Files", len(CONFIG_FILES))
    if backup:
        print_field("Backup", code(shorten_path(backup)))


def get_list() -> None:
    root = profile_root()
    profiles = []
    for path in sorted(root.glob("gui_lv_*")):
        if path.is_dir():
            project = path.name.removeprefix("gui_lv_")
            meta_path = path / PROFILE_META
            saved_at = "-"
            mode = "-"
            if meta_path.is_file():
                try:
                    meta = read_json(meta_path)
                    saved_at = meta.get("saved_at", "-")
                    mode = meta.get("mode", "-")
                except (OSError, json.JSONDecodeError):
                    saved_at = "metadata unreadable"
            profiles.append((project, mode, saved_at, path))

    if not profiles:
        print_title("GUI_LV Profiles")
        print_summary("Profiles", "0 saved", ok=False)
        print("No profiles have been saved yet.")
        print_hint(f"Run {code('save <project>')} to capture the current gui_lv configuration.")
        return

    print_title("GUI_LV Profiles")
    print_summary("Profiles", f"{len(profiles)} saved")
    print_rule()
    for project, mode, saved_at, path in profiles:
        print_profile_card(project, mode, saved_at, path)


def get_status(use_rte: bool) -> None:
    target = resolve_target(use_rte)
    ensure_dir(target, "target GUI_LV directory")
    status_path = target / STATUS_FILE
    if not status_path.is_file():
        print_title("GUI_LV Status")
        print_summary("Project", yellow(bold("not written by profile tool")), ok=False)
        print_field("Mode", "RTE" if use_rte else "source")
        print_field("Target", code(shorten_path(target)))
        print_hint(f"Run {code('write <project>')} to write a profile and record status.")
        return

    try:
        status = read_json(status_path)
    except json.JSONDecodeError as exc:
        raise ProfileError(f"status file is not valid JSON: {status_path} ({exc})") from exc

    project = status.get("project", "-")
    mode = status.get("mode", "-")
    print_title("GUI_LV Status")
    print_summary("Active", green(bold(f"gui_lv_{project}")))
    print_field("Project", green(project))
    print_field("Mode", mode)
    print_field("Written At", status.get("written_at", "-"))
    print_field("Target", code(shorten_path(status.get("target", target))))
    print_field("Profile", code(shorten_path(status.get("profile_path", "-"))))


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="gui_lv_profile",
        description="Save and restore GUI_LV project configuration profiles.",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    save = subparsers.add_parser("save", help="save current GUI_LV config to a project profile")
    save.add_argument("project")
    save.add_argument("--RTE", action="store_true", help="read from the current Keil RTE GUI_LVGL_Wrapper directory")

    write = subparsers.add_parser("write", help="write a project profile to GUI_LV config")
    write.add_argument("project")
    write.add_argument("--RTE", action="store_true", help="write to the current Keil RTE GUI_LVGL_Wrapper directory")

    get = subparsers.add_parser("get", help="query profile information")
    get.add_argument("topic", choices=["status", "list"])
    get.add_argument("--RTE", action="store_true", help="query the current Keil RTE GUI_LVGL_Wrapper directory")

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)

    try:
        if args.command == "save":
            save_project(args.project, args.RTE)
        elif args.command == "write":
            write_project(args.project, args.RTE)
        elif args.command == "get" and args.topic == "list":
            get_list()
        elif args.command == "get" and args.topic == "status":
            get_status(args.RTE)
        else:
            parser.error("unknown command")
    except ProfileError as exc:
        print_error(exc)
        return 1
    except OSError as exc:
        print_error(exc)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
