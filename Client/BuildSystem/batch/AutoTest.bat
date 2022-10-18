@echo off

rem 매개변수-  실행파일명 실행파일폴더 작업폴더 autotest.ini파일path

if '%1'=='' goto errlog

	set exe_name=%1
	set exe_path=%2
	set workdir=%3
    	set inidir=%4
	set curdir=%cd%

	chdir /d %exe_path%

	del %exe_path%\log\*.log /f /q
	
	xcopy %inidir%\autotest.ini %exe_path%\ /y /r /f

	call %exe_name% -autotest

	del %exe_path%\autotest.ini /f /q

	chdir /d %curdir%

	exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause