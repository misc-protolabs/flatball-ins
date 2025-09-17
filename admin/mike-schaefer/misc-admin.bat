
python ..\py\sync-github-config.py

cd ..\..\dev\
::python .\py\propogate-lic.py --dry-run
::python .\py\propogate-lic.py
python ..\admin\py\propogate-lic.py --report
