@echo off

rem �Ű�����- ��������1 ��������2 n.txt����ġ�ϴ�path

if not exist "%1" goto end
if not exist "%2" goto end

set inputfile=%3\n.txt

comp "%1" "%2" < %inputfile%

rem "%errorlevel%"=="0" �� ���ٴ� ��. ������ end
if "%errorlevel%"=="0" goto end
	xcopy "%1" "%2" /y /r /f
	if  exist "%2" (
		p4 add "%2"
	        	p4 edit "%2"
	)

exit /b 1




:end
exit /b 0
