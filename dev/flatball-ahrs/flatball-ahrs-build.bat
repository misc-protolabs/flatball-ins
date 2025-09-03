@echo off
cls
cd /d C:\.local\desktop\flatball-aero-ssl\dev\flatball-ahrs

echo Running flatball-ahrs-build.py with quiet console output...
python utils/py/flatball-ahrs-build.py

echo Build complete.
pause
