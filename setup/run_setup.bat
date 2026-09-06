@echo off
title DSi-FC-Men Setup Tool
echo.
echo Starting DSi-FC-Men Setup...
echo.
python setup.py %1
if errorlevel 1 (
    echo.
    echo Python not found or error occurred.
    echo Please install Python 3 and try again.
    pause
)
pause
