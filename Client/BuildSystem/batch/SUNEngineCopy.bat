@echo off

rem 매개변수-  국가별SUNEnginePath 디폴트SUNEnginePath

if '%1'=='' goto errlog

set custom_engine_path=%1
set default_engine_path=%2


echo.
echo ▷ 국가별 엔진 path: %custom_engine_path%
echo ▷ 디폴트 엔진 path: %default_engine_path%
echo.

xcopy %custom_engine_path%\*.* %default_engine_path%\*.* /e /c /f /r /y

exit /b 0



:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause