@echo off
setlocal enabledelayedexpansion
REM ---------------------------------------------------------------
REM  Fix GitHub access: your DNS resolves github.com to a private
REM  IP (10.101.x.x), which makes every git clone/push time out.
REM  This script writes GitHub's real IPs into the hosts file.
REM  USAGE: right-click -> Run as administrator
REM ---------------------------------------------------------------

set HOSTS=%SystemRoot%\System32\drivers\etc\hosts
set TMPF=%TEMP%\hosts_github_new.txt

net session >nul 2>&1
if %errorlevel% neq 0 (
  echo.
  echo  [ERROR] Not running as administrator.
  echo  Right-click this file and choose "Run as administrator".
  echo.
  pause
  exit /b 1
)

echo.
echo  Backing up hosts file ...
copy /Y "%HOSTS%" "%HOSTS%.bak-github" >nul

echo  Removing old github entries ...
if exist "%TMPF%" del /Q "%TMPF%"
for /f "usebackq delims=" %%L in ("%HOSTS%") do (
  set "line=%%L"
  echo.!line! | findstr /I /R "github\.com" >nul
  if errorlevel 1 echo %%L>>"%TMPF%"
)

echo  Writing correct GitHub IPs ...
echo.>>"%TMPF%"
echo # GitHub direct - added by fix-github-hosts.bat>>"%TMPF%"
echo 20.205.243.166 github.com>>"%TMPF%"
echo 140.82.114.4 github.com>>"%TMPF%"
echo 20.205.243.168 api.github.com>>"%TMPF%"

copy /Y "%TMPF%" "%HOSTS%" >nul
del /Q "%TMPF%" >nul
ipconfig /flushdns >nul

echo.
echo  Done. Current github entries:
findstr /I "github.com" "%HOSTS%"
echo.
echo  Now run:  git push -u origin main
echo.
pause
