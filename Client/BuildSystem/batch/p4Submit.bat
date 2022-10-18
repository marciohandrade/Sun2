@echo off

rem 매개변수-  log폴더

if '%1'=='' goto errlog

	set logpath=%1


	p4 submit -i < %logpath%\p4_desc.txt
	exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause