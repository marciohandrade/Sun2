@echo off

rem �Ű�����-  E:\Perforce\test_release\3.������Ʈ����\SUN_Client\...

if '%1'=='' goto errlog

	set depotpath=%1

	p4 sync %depotpath%
	exit /b 0



:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause