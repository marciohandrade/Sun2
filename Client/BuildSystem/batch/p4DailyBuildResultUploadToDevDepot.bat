@echo off

rem 매개변수-  실행파일업로드폴더 작업폴더

if '%1'=='' goto errlog

        set upload=%1
        set workdir=%2
	set bindir=%workdir%\Client\_bin
	set logdir=%workdir%\Client\BuildSystem\log

        rem --------------------------------------------------------
	rem 일단 sync를 해두자
        rem --------------------------------------------------------
	rem p4 sync %upload%\...

	p4 sync %upload%\System\...
	p4 sync %upload%\GameClient.exe
	p4 sync %upload%\Sungame_Dump.exe
	p4 sync %upload%\Sungame.exe
	p4 sync %upload%\3DFileAccess.dll
	p4 sync %upload%\3DFileAccess_p.dll
	p4 sync %upload%\3DRendererD3D.dll
	p4 sync %upload%\3DRendererD3D_p.dll
	p4 sync %upload%\3DTerrain.dll
	p4 sync %upload%\3DTerrain_p.dll
	p4 sync %upload%\Effect.dll
	p4 sync %upload%\Effect_p.dll
	p4 sync %upload%\ControlsDLL.dll
	p4 sync %upload%\ControlsDLL_p.dll
	p4 sync %upload%\wzSound.dll



	

	echo ▷ GameClient.exe 배포
        rem --------------------------------------------------------
        rem 최신개발폴더에 업로드
        rem --------------------------------------------------------

	xcopy %bindir%\GameClient.exe %upload%\ /c /f /r /y
	if exist %upload%\GameClient.exe (		
            p4 add %upload%\GameClient.exe
            p4 edit %upload%\GameClient.exe
        )

	echo ▷ Engine dll 배포
        rem --------------------------------------------------------
        rem 최신개발폴더에 업로드
        rem --------------------------------------------------------

	xcopy %bindir%\3DFileAccess.dll %upload%\ /c /f /r /y
	xcopy %bindir%\3DFileAccess_p.dll %upload%\ /c /f /r /y
	xcopy %bindir%\3DRendererD3D.dll %upload%\ /c /f /r /y
	xcopy %bindir%\3DRendererD3D_p.dll %upload%\ /c /f /r /y
	xcopy %bindir%\3DTerrain.dll %upload%\ /c /f /r /y
	xcopy %bindir%\3DTerrain_p.dll %upload%\ /c /f /r /y
	xcopy %bindir%\Effect.dll %upload%\ /c /f /r /y	
	xcopy %bindir%\Effect_p.dll %upload%\ /c /f /r /y
	xcopy %bindir%\ControlsDLL.dll %upload%\ /c /f /r /y
	xcopy %bindir%\ControlsDLL_p.dll %upload%\ /c /f /r /y
	xcopy %bindir%\wzSound.dll %upload%\ /c /f /r /y
										
	if exist %upload%\3DFileAccess.dll (
   		p4 add %upload%\3DFileAccess.dll
    		p4 edit %upload%\3DFileAccess.dll
	)
		
	if exist %upload%\3DFileAccess_p.dll (
	    p4 add %upload%\3DFileAccess_p.dll
	    p4 edit %upload%\3DFileAccess_p.dll
        )
		
        if exist %upload%\3DRendererD3D.dll (
            p4 add %upload%\3DRendererD3D.dll
            p4 edit %upload%\3DRendererD3D.dll
        )
        
        if exist %upload%\3DRendererD3D_p.dll (		
            p4 add %upload%\3DRendererD3D_p.dll
            p4 edit %upload%\3DRendererD3D_p.dll
        )						
				
        if exist %upload%\3DTerrain.dll (				
            p4 add %upload%\3DTerrain.dll
            p4 edit %upload%\3DTerrain.dll
        )
        						
        if exist %upload%\3DTerrain_p.dll (								
            p4 add %upload%\3DTerrain_p.dll
            p4 edit %upload%\3DTerrain_p.dll
        )						
				
        if exist %upload%\Effect.dll (										
            p4 add %upload%\Effect.dll
            p4 edit %upload%\Effect.dll
        )				
		
        if exist %upload%\Effect_p.dll (			
            p4 add %upload%\Effect_p.dll
            p4 edit %upload%\Effect_p.dll
        )		
		
        if exist %upload%\ControlsDLL.dll (				
            p4 add %upload%\ControlsDLL.dll
            p4 edit %upload%\ControlsDLL.dll
        )		

        if exist %upload%\ControlsDLL_p.dll (			
            p4 add %upload%\ControlsDLL_p.dll
            p4 edit %upload%\ControlsDLL_p.dll
        )		
                
        if exist %upload%\wzSound.dll (			
            p4 add %upload%\wzSound.dll
            p4 edit %upload%\wzSound.dll
        )	        



	echo ▷ Shader 배포
        rem --------------------------------------------------------
        rem 최신개발폴더에 업로드
        rem --------------------------------------------------------

        xcopy %bindir%\System\*.vso %upload%\System\ /c /f /r /y
        xcopy %bindir%\System\*.pso %upload%\System\ /c /f /r /y

        p4 add %upload%\System/*.vso
        p4 edit %upload%\System/*.vso
        
        p4 add %upload%\System/*.pso
        p4 edit %upload%\System/*.pso
        


		
    
echo.-------------------------------------------------------------------------
echo ▷ %logdir%\p4_desc.txt
echo.-------------------------------------------------------------------------
	
    del %logdir%\p4_desc.txt /f /q
    p4 change -o > %logdir%\p4_desc.txt	
    %buildsystempath%\p4_desc_make %logdir%\p4_desc.txt DailyBuild-[%date%]

    p4 submit -i < %logdir%\p4_desc.txt    

    exit /b 0


:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause