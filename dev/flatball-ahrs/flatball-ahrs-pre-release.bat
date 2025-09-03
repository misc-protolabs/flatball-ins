@echo off
cls

cd /d C:\.local\desktop\flatball-aero-ssl\dev\flatball-ahrs\test
echo ...Running flatball-ahrs-tests.py...
python ./flatball-ahrs-tests.py
move C:\.local\desktop\flatball-aero-ssl\dev\flatball-ahrs\test\flatball-ahrs-tests.log ..
echo ...Tests complete.

cd /d C:\.local\desktop\flatball-aero-ssl\dev\flatball-ahrs
echo ...Running profiler test...
python utils/py/flatball-ahrs-profiler.py
echo ...Profiler test complete.

echo ...Running flatball-ahrs-build.py with quiet console output...
python utils/py/flatball-ahrs-build.py
echo ...Build complete.

echo ...Uploading release candidate...
pio run -e esp32dev -t upload
pause
