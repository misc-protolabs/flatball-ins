@echo off
echo Starting Python HTTP server...
start "" python -m http.server

echo Starting telemetry WebSocket bridge...
start "" python .\public\py\ins-telemetry.py

echo All services launched.
