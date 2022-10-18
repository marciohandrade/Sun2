@echo off

rem 매개변수 - localfolder
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Revert...
@echo.
	
	svn revert %WZ_WORKFOLD%\Client\GameClient\ClientVer.h

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Revert 완료
@echo.
	
exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause



