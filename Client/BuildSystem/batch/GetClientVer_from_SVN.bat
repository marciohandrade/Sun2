@echo off

rem 매개변수 - localfolder log폴더
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	set logfile=%2\svn_update_ClientVer_log.txt
	

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h 가져오는 중...
@echo.
	
	svn update %WZ_WORKFOLD%\Client\GameClient\ClientVer.h > %logfile%

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Update 완료
@echo.
	
exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause



