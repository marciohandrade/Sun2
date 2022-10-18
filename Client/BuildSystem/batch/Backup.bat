@echo off

rem 매개변수-  작업폴더 백업폴더 BuildSystem폴더

if '%1'=='' goto errlog

	set workdir=%1
	set backupdir=%2
	set buildsysdir=%3
	set listfile=%buildsysdir%\batch\exclude_list.xcopy

	xcopy %workdir%\*.* %backupdir%\ /s /f /r /y /exclude:%listfile%
	exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause