@echo off
cls

rem 매개변수- localfolder revision log폴더
	if '%1'=='' goto errlog


	set WZ_WORKFOLD=%1
	set rev=%2
	set logfile=%3\svn_update_log.txt

@echo.
@echo ▷ Revision(%rev%) 소스 가져오는 중... (to %WZ_WORKFOLD%)
@echo.

	svn update -r %rev% %WZ_WORKFOLD% > %logfile%

@echo.
@echo ▷ Revision(%rev%) Update 완료 (to %WZ_WORKFOLD%)
@echo.

exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause

