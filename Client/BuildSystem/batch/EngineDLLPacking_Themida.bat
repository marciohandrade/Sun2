@echo off

rem �Ű�����-  �۾����� dll���ϸ� tmd����path+���ϸ�
rem themida �������� ��θ� �ݵ�� ȯ�溯�� ������ ������!!!! (�׷��� ��ġ������ ��������)


if '%1'=='' goto errlog

	set workdir=%1
	set dllfile=%2
	set tmdpath=%3

	set enginedir=%workdir%\SUN3DEngine\dll
	set batchdir=%workdir%\Client\BuildSystem\batch
	start /w themida /protect %tmdpath% /inputfile %enginedir%\%dllfile% /outputfile %enginedir%\%dllfile%

	if errorlevel 6 goto 6  
	if errorlevel 5 goto 5  
	if errorlevel 4 goto 4   
	if errorlevel 3 goto 3  
	if errorlevel 2 goto 2  
	if errorlevel 1 goto 1  
	if errorlevel 0 goto 0



 :0  
 echo Application protected successfully  
 goto done  
 
 :1  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: Project file does not exist or invalid. 
@echo.

pause	
exit /b 1001

 
 :2  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: File to protect cannot be opened.    
@echo.

pause	
exit /b 1001

 
 :3  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: File already protected.  
@echo.

pause	
exit /b 1001


 :4  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: Error in inserted SecureEngine macros  
@echo.

pause	
exit /b 1001


 :5  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: Fatal error while protecting file.   
@echo.

pause	
exit /b 1001


 :6  
@echo.  
@echo �� ���̳ʸ���ŷ ����
@echo ERROR: Cannot write protected file to disk.   
@echo.

pause	
exit /b 1001



:done  
@echo.
@echo �� ���̳ʸ� ��ŷ �Ϸ�. �ڵ� ���� ���� ����.
@echo.
	
call %batchdir%\CodeSignForThemida.bat %workdir% %enginedir%\%dllfile%

exit /b 0


:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause