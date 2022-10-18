@echo off

rem 매개변수-  빌드모드(1:build, 2:rebuild) cfg명 작업폴더 log폴더

if '%1'=='' goto errlog


	set cfg_name=%2
	set sln_path=%3\Client\GameClient\GameClient.sln
	set logpath=%4
	set buildlog=%logpath%\build.log

	del %logpath%\success.log /q
	del %logpath%\fail.log /q
	del %logpath%\build.log /q

	if '%1'=='1' goto buildstep
	if '%1'=='2' goto rebuildstep


	goto errlog

echo.
echo ▷ 빌드 진행중...
echo.


	:buildstep
	
	BuildConsole.exe %sln_path% /cfg=%cfg_name% /log=%buildlog% /ShowTime /NoIncrediLink /OpenMonitor

	if errorlevel 1 goto build_err
	goto end


	:rebuildstep


	BuildConsole.exe %sln_path% /cfg=%cfg_name% /rebuild /log=%buildlog% /ShowTime /NoIncrediLink /OpenMonitor


	if errorlevel 1 goto build_err
	goto end






:end

@echo.
@echo ▷ 빌드 성공 (%cfg_name%) >  %logpath%\success.log
@echo.
	
exit /b 0


:build_err
@echo.
@echo ▷ 빌드 실패 (%cfg_name%) > %logpath%\fail.log
@echo.

@echo.  >> %logpath%\BuildSystemResult.txt
@echo ================== 빌드 실패 (%cfg_name%) ================== >> %logpath%\BuildSystemResult.txt
findstr /i /c:"fatal error" %buildlog% >> %logpath%\BuildSystemResult.txt
findstr /i /c:"error C" %buildlog% >> %logpath%\BuildSystemResult.txt
@echo =================================================== >> %logpath%\BuildSystemResult.txt
@echo.  >> %logpath%\BuildSystemResult.txt
	
exit /b 1001


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause