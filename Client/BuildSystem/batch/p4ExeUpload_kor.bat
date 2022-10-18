@echo off

rem �Ű�����-  �������Ͼ��ε����� ��������output���� pack��������output���� ���������Ķ���� log����

if '%1'=='' goto errlog

	set upload=%1
	set bindir=%2
	set binpackdir=%3
	set param=%4
	set logdir=%5
    
	set gameclient=%param:~0,1%
	set dump=%param:~1,1%
	set sungame=%param:~2,1%
	set engine=%param:~3,1%
	set shader=%param:~4,1%
	
	
	rem �ϴ� sync�� �ص���
rem	p4 sync %upload%\...

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


    if '%gameclient%'=='1' (
		echo �� GameClient.exe ����

		rem ���������� ���ε�
		xcopy %bindir%\GameClient.exe %upload%\ /c /f /r /y
		if exist %upload%\GameClient.exe (		
            p4 add %upload%\GameClient.exe
    		p4 edit %upload%\GameClient.exe
        )
        
		
	)

	if '%dump%'=='1' (
		echo �� Sungame_Dump.exe ����

		rem ���������� ���ε�
		xcopy %bindir%\Sungame_Dump.exe %upload%\ /c /f /r /y
		if exist %upload%\Sungame_Dump.exe (				
    		p4 add %upload%\Sungame_Dump.exe
    		p4 edit %upload%\Sungame_Dump.exe
        )
        
	)

	if '%sungame%'=='1' (
		echo �� Sungame.exe ����
		
		rem ���������� ���ε�
		xcopy %binpackdir%\Sungame.exe %upload%\ /c /f /r /y
		if exist %upload%\Sungame.exe (						
    		p4 add %upload%\Sungame.exe
    		p4 edit %upload%\Sungame.exe
        )

	)
    

	if '%engine%'=='1' (
		echo �� Engine dll ����

		rem ���������� ���ε�
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
                        
        
	)

	if '%shader%'=='1' (
		echo �� Shader ����

		rem ���������� ���ε�
        xcopy %bindir%\System\*.vso %upload%\System\ /c /f /r /y
        xcopy %bindir%\System\*.pso %upload%\System\ /c /f /r /y

        p4 add %upload%\System/*.vso
        p4 edit %upload%\System/*.vso
        
        p4 add %upload%\System/*.pso
        p4 edit %upload%\System/*.pso
        
	)


    
echo.
echo �� %logdir%\p4_desc.txt
echo.
	
    del %logdir%\p4_desc.txt /f /q
    p4 change -o > %logdir%\p4_desc.txt	
exit /b 0


:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause