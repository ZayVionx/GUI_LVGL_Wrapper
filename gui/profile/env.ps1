$profileRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$toolsPath = Join-Path $profileRoot "tools"

if (($env:Path -split ';') -notcontains $toolsPath) {
    $env:Path = "$toolsPath;$env:Path"
}

$env:GUI_LV_PROFILE_ROOT = $profileRoot
Write-Host "GUI_LV profile commands are ready: save, write, get"
