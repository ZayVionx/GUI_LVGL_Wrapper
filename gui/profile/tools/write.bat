@echo off
set "PYTHONDONTWRITEBYTECODE=1"
python "%~dp0gui_lv_profile.py" write %*
