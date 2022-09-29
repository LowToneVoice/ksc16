@echo off
echo converting
powershell -NoProfile -ExecutionPolicy Unrestricted .\mca2hist.ps1
echo Done!!
echo Click any-keys to close this window.
pause > nul
exit
