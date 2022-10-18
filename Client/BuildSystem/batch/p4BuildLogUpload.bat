@echo off

rem 매개변수-  실행파일output폴더 빌드로그업로드용폴더 빌드된실행파일명

if '%1'=='' goto errlog

	set bindir=%1
	set logupload=%2
	set exefile=%3
    
	set dmpinfo=%bindir%\DumpInfos
	
	mkdir %logupload%
	

	rem 일단 sync를 해두자
	p4 sync %logupload%\...
	
    if '%exefile%'=='GameClient.exe' (
		echo ▷ GameClient.exe 배포

        
        rem 빌드로그폴더에 업로드
        xcopy %bindir%\GameClient.exe %logupload%\client\ /y /r /f
		xcopy %dmpinfo%\GameClient_Release.map %logupload%\dumpinfos\ /y /r /f
		xcopy %dmpinfo%\GameClient_Release.pdb %logupload%\dumpinfos\ /y /r /f
		
		
		if exist %logupload%\client\GameClient.exe (
		        p4 add %logupload%\client\GameClient.exe
		        p4 edit %logupload%\client\GameClient.exe
		)
		if exist %logupload%\dumpinfos\GameClient_Release.map (
		    p4 add %logupload%\dumpinfos\GameClient_Release.map
		    p4 edit %logupload%\dumpinfos\GameClient_Release.map
        )
        if exist %logupload%\dumpinfos\GameClient_Release.pdb (
            p4 add %logupload%\dumpinfos\GameClient_Release.pdb
            p4 edit %logupload%\dumpinfos\GameClient_Release.pdb
        )            
		
	)

	if '%exefile%'=='Sungame_Dump.exe' (
		echo ▷ Sungame_Dump.exe 배포

		rem 빌드로그폴더에 업로드
		xcopy %bindir%\Sungame_Dump.exe %logupload%\client\ /y /r /f
		xcopy %dmpinfo%\GameClient_User_Dump.map %logupload%\dumpinfos\ /y /r /f
		xcopy %dmpinfo%\GameClient_User_Dump.pdb %logupload%\dumpinfos\ /y /r /f
		
		if exist %logupload%\client\Sungame_Dump.exe (
		    p4 add %logupload%\client\Sungame_Dump.exe
		    p4 edit %logupload%\client\Sungame_Dump.exe
        )
		
		if exist %logupload%\dumpinfos\GameClient_User_Dump.map (
		    p4 add %logupload%\dumpinfos\GameClient_User_Dump.map
		    p4 edit %logupload%\dumpinfos\GameClient_User_Dump.map
        )		    
		
		if exist %logupload%\dumpinfos\GameClient_User_Dump.pdb (
		    p4 add %logupload%\dumpinfos\GameClient_User_Dump.pdb
		    p4 edit %logupload%\dumpinfos\GameClient_User_Dump.pdb
        )		    

	)

	if '%exefile%'=='Sungame.exe' (
		echo ▷ Sungame.exe 배포
		
		rem 빌드로그폴더에 업로드
		xcopy %bindir%\Sungame.exe %logupload%\client\unpack\ /y /r /f
		xcopy %dmpinfo%\GameClient_User.map %logupload%\dumpinfos\ /y /r /f
		xcopy %dmpinfo%\GameClient_User.pdb %logupload%\dumpinfos\ /y /r /f

        if exist %logupload%\client\unpack\Sungame.exe (
		    p4 add %logupload%\client\unpack\Sungame.exe
		    p4 edit %logupload%\client\unpack\Sungame.exe
        )		    
		
		if exist %logupload%\dumpinfos\GameClient_User.map (
		    p4 add %logupload%\dumpinfos\GameClient_User.map
		    p4 edit %logupload%\dumpinfos\GameClient_User.map
        )		    
		
		if exist %logupload%\dumpinfos\GameClient_User.pdb (
		    p4 add %logupload%\dumpinfos\GameClient_User.pdb
		    p4 edit %logupload%\dumpinfos\GameClient_User.pdb
        )		    

	)
    

exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause