@echo off


	if '%1'=='' goto errlog
	
rem �Ű�����- log����
	
	set logpath=%1
	
	mkdir %logpath%
	del %logpath%\*.log /f /q

@echo.
@echo �� %logpath%\*.log ���� �Ϸ�
@echo.
	

exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause
