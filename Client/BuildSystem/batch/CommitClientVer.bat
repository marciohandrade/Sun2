@echo off

rem 매개변수 - localfolder 국가디파인 새버전명
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	set strdef=%2
	set strver=%3
	set logmsg="%strdef%: Update Client Build Ver. %strver%"

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Commit...
@echo.
	
	svn commit -m %logmsg% %WZ_WORKFOLD%\Client\GameClient\ClientVer.h

@echo.
@echo ▷ %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Commit 완료
@echo.
	
exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause



