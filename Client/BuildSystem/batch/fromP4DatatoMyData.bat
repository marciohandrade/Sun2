@echo off

rem 매개변수-  p4리소스폴더 실행파일ouput폴더

if '%1'=='' goto errlog

set depotpath=%1\Data
set workpath=%2\Data

set depot_sys_path=%1\System
set work_sys_path=%2\System

echo.
echo ▷ %depotpath%
echo ▷ %workpath%
echo.

xcopy %depotpath%\*.* %workpath%\*.* /d /s /c /f /r /y
xcopy %depot_sys_path%\*.ini %work_sys_path%\*.ini /d /s /c /f /r /y

exit /b 0



:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause