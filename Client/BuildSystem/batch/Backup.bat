@echo off

rem �Ű�����-  �۾����� ������� BuildSystem����

if '%1'=='' goto errlog

	set workdir=%1
	set backupdir=%2
	set buildsysdir=%3
	set listfile=%buildsysdir%\batch\exclude_list.xcopy

	xcopy %workdir%\*.* %backupdir%\ /s /f /r /y /exclude:%listfile%
	exit /b 0


:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause