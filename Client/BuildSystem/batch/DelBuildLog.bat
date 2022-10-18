@echo off


	if '%1'=='' goto errlog
	
rem 매개변수- log폴더
	
	set logpath=%1
	
	mkdir %logpath%
	del %logpath%\*.log /f /q

@echo.
@echo ▷ %logpath%\*.log 삭제 완료
@echo.
	

exit /b 0


:errlog
echo.
echo ▷ 매개변수 없이 실행할 수 없습니다.
echo.

pause
