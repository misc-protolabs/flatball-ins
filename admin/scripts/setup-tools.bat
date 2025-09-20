@echo off
setlocal enabledelayedexpansion
echo This script will help you install the necessary development toolchain.
echo.

set packages=^
    TheDocumentFoundation.LibreOffice ^
    FreeCAD.FreeCAD ^
    Ultimaker.Cura ^
    KiCad.KiCad ^
    ArduinoSA.IDE.stable ^
    Python.Python.3.13 ^
    Microsoft.VisualStudioCode ^
    OpenJS.NodeJS.LTS ^
    Git.Git ^
    TortoiseGit.TortoiseGit

for %%p in (%packages%) do (
    echo --------------------------------------------------
    echo Checking for package: %%p
    winget list --id %%p -e | find "%%p" > nul
    if not errorlevel 1 (
        echo %%p is already installed.
    ) else (
        set "install="
        set /p "install=%%p is not installed. Do you want to install it? (y/n) "
        if /i "!install!"=="y" (
            echo Installing %%p...
            winget install --id %%p -e --accept-source-agreements --accept-package-agreements
        ) else (
            echo Skipping installation of %%p.
        )
    )
    echo.
)

echo --------------------------------------------------
echo Checking for npm package: three
npm list -g three | find "three@" > nul
if not errorlevel 1 (
    echo three is already installed.
) else (
    set "install="
    set /p "install=three is not installed. Do you want to install it? (y/n) "
    if /i "!install!"=="y" (
        echo Installing three...
        npm install -g three
    ) else (
        echo Skipping installation of three.
    )
)
echo.

echo Toolchain setup complete.
endlocal