@echo off

rem �Ű�����-  �۾����� ������������ ��ŷoutput���� arm����path+���ϸ�

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
@echo �� �Ϸ�
@echo.
	
exit /b 0


:armadillo_error
@echo.
@echo �� ���̳ʸ���ŷ ����
@echo.

pause	
exit /b 1001

:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause