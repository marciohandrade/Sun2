@echo off

rem 매개변수-  작업폴더 실행파일폴더 패킹output폴더 arm파일path+파일명

if '%1'=='' goto errlog

	set workdir=%1
	set bindir=%2
	set packdir=%3
	set armpath=%4

	rmdir %packdir%\ /s /q
	mkdir %packdir%\

	xcopy %bindir%\Sungame.exe %packdir%\ /y /r /f

	"C:\Program Files\SoftwarePassport\ArmCLine.exe" %armpath% /P

	if errorlevel 1 goto armadillo_error
	goto end




:end

@echo.
@echo ▷ 완료
@echo.
	
exit /b 0


:armadillo_error
@echo.
@echo ▷ 바이너리패킹 실패
@echo.

pause	
exit /b 1001

:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause