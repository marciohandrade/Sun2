@echo off

rem �Ű�����-  �������ϸ� ������������ �۾����� autotest.ini����path

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
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause