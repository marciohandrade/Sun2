@echo off
cls

rem 매개변수- localfolder log폴더
	if '%1'=='' goto errlog
	

	set WZ_WORKFOLD=%1
	set logfile=%2\svn_update_log.txt

@echo.
@echo ▷ %WZ_WORKFOLD% 최신 소스 가져오는 중...
@echo.
	
	svn update %WZ_WORKFOLD% > %logfile%

@echo.
@echo ▷ %WZ_WORKFOLD% 최신 소스 Update 완료
@echo.
	
exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause

