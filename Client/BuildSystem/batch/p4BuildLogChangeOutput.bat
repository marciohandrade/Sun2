@echo off

rem 매개변수-  빌드로그업로드용폴더 log폴더

if '%1'=='' goto errlog

	set logupload=%1
	set logdir=%2
	
	mkdir %logupload%
	

	rem 일단 sync를 해두자
	p4 sync %logupload%\...
	
	xcopy %logdir%\svn_tag_log.txt %logupload%\log\ /y /r
	xcopy %logdir%\svn_update_log.txt %logupload%\log\ /y /r

	if exist %logupload%\log\svn_tag_log.txt (
		p4 add %logupload%\log\svn_tag_log.txt        
    	)

	if exist %logupload%\log\svn_update_log.txt (
		p4 add %logupload%\log\svn_update_log.txt        
    	)


echo.
echo ▷ %logdir%\p4_desc.txt
echo.
	
    del %logdir%\p4_desc.txt /f /q
    p4 change -o > %logdir%\p4_desc.txt	
exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause