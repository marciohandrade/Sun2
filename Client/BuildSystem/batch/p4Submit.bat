@echo off

rem �Ű�����-  log����

if '%1'=='' goto errlog

	set logpath=%1


	p4 submit -i < %logpath%\p4_desc.txt
	exit /b 0


:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause