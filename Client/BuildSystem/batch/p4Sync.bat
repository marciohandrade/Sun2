@echo off

rem 매개변수-  E:\Perforce\test_release\3.업데이트버전\SUN_Client\...

if '%1'=='' goto errlog

	set depotpath=%1

	p4 sync %depotpath%
	exit /b 0



:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause