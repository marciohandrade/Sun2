@echo off

rem 매개변수-  작업폴더 dll파일명 tmd파일path+파일명
rem themida 실행파일 경로를 반드시 환경변수 세팅을 해주자!!!! (그래야 배치파일이 정상실행됨)


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
@echo ▷ 바이너리패킹 실패
@echo ERROR: Project file does not exist or invalid. 
@echo.

pause	
exit /b 1001

 
 :2  
@echo.  
@echo ▷ 바이너리패킹 실패
@echo ERROR: File to protect cannot be opened.    
@echo.

pause	
exit /b 1001

 
 :3  
@echo.  
@echo ▷ 바이너리패킹 실패
@echo ERROR: File already protected.  
@echo.

pause	
exit /b 1001


 :4  
@echo.  
@echo ▷ 바이너리패킹 실패
@echo ERROR: Error in inserted SecureEngine macros  
@echo.

pause	
exit /b 1001


 :5  
@echo.  
@echo ▷ 바이너리패킹 실패
@echo ERROR: Fatal error while protecting file.   
@echo.

pause	
exit /b 1001


 :6  
@echo.  
@echo ▷ 바이너리패킹 실패
@echo ERROR: Cannot write protected file to disk.   
@echo.

pause	
exit /b 1001



:done  
@echo.
@echo ▷ 바이너리 패킹 완료. 코드 사인 진행 시작.
@echo.
	
call %batchdir%\CodeSignForThemida.bat %workdir% %enginedir%\%dllfile%

exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause