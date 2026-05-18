#!/usr/bin/env python3
"""Save and restore per-project GUI_LV configuration profiles."""

from __future__ import annotations

import argparse
import json
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

    print(f"saved {project}: {src} -> {dst}")
    if backup:
        print(f"previous profile backup: {backup}")


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

    print(f"wrote {project}: {src} -> {dst}")
    if backup:
        print(f"target backup: {backup}")


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
        print("no gui_lv profiles saved")
        return

    for project, mode, saved_at, path in profiles:
        print(f"{project}\t{mode}\t{saved_at}\t{path}")


def get_status(use_rte: bool) -> None:
    target = resolve_target(use_rte)
    ensure_dir(target, "target GUI_LV directory")
    status_path = target / STATUS_FILE
    if not status_path.is_file():
        print(f"no current project status in {target}")
        return

    try:
        status = read_json(status_path)
    except json.JSONDecodeError as exc:
        raise ProfileError(f"status file is not valid JSON: {status_path} ({exc})") from exc

    print(f"project: {status.get('project', '-')}")
    print(f"mode: {status.get('mode', '-')}")
    print(f"written_at: {status.get('written_at', '-')}")
    print(f"target: {status.get('target', target)}")
    print(f"profile_path: {status.get('profile_path', '-')}")


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
        print(f"error: {exc}", file=sys.stderr)
        return 1
    except OSError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
