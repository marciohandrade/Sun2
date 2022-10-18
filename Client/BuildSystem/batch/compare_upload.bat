@echo off

rem 매개변수- 비교할파일1 비교할파일2 n.txt가위치하는path

if not exist "%1" goto end
if not exist "%2" goto end

set inputfile=%3\n.txt

comp "%1" "%2" < %inputfile%

rem "%errorlevel%"=="0" 면 같다는 것. 같으면 end
if "%errorlevel%"=="0" goto end
	xcopy "%1" "%2" /y /r /f
	if  exist "%2" (
		p4 add "%2"
	        	p4 edit "%2"
	)

exit /b 1




:end
exit /b 0
