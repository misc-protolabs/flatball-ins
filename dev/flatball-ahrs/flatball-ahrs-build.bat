@echo off
cls
cd /d flatball-ahrs

echo Running flatball-ahrs-build.py with quiet console output...
python utils/py/flatball-ahrs-build.py

echo Build complete.
pause
