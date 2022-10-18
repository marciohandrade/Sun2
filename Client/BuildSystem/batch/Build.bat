@echo off

rem �Ű�����-  ������(1:build, 2:rebuild) cfg�� �۾����� log����

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
echo �� ���� ������...
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
@echo �� ���� ���� (%cfg_name%) >  %logpath%\success.log
@echo.
	
exit /b 0


:build_err
@echo.
@echo �� ���� ���� (%cfg_name%) > %logpath%\fail.log
@echo.

@echo.  >> %logpath%\BuildSystemResult.txt
@echo ================== ���� ���� (%cfg_name%) ================== >> %logpath%\BuildSystemResult.txt
findstr /i /c:"fatal error" %buildlog% >> %logpath%\BuildSystemResult.txt
findstr /i /c:"error C" %buildlog% >> %logpath%\BuildSystemResult.txt
@echo =================================================== >> %logpath%\BuildSystemResult.txt
@echo.  >> %logpath%\BuildSystemResult.txt
	
exit /b 1001


:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause