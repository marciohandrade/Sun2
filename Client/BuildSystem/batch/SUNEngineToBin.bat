@echo off

rem 매개변수-  SUNEnginePath 실행파일OutputPath

if '%1'=='' goto errlog

set engine_dll_path=%1\dll
set engine_shader_path=%1\shader

set bin_path=%2\
set bin_system=%2\system\

echo.
echo ▷ 엔진 dll path: %engine_dll_path%
echo ▷ 엔진 shader path: %engine_shader_path%
echo ▷ bin path: %bin_path%
echo.

xcopy %engine_dll_path%\*.dll %bin_path% /c /f /r /y
xcopy %engine_shader_path%\*.pso %bin_system% /c /f /r /y
xcopy %engine_shader_path%\*.vso %bin_system% /c /f /r /y

exit /b 0



:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause