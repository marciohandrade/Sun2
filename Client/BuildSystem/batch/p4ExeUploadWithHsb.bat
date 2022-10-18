@echo off

rem �Ű�����-  �������Ͼ��ε����� ��������output���� pack��������output���� hsb���� ����α׾��ε������ ���������Ķ���� log���� ����path

if '%1'=='' goto errlog

	set upload=%1
	set bindir=%2
	set binpackdir=%3
	set hsbdir=%4
	set logupload=%5
	set param=%6
	set logdir=%7
	set enginepath=%8
	
	set dmpinfo=%bindir%\DumpInfos
	set gameclient=%param:~0,1%
	set dump=%param:~1,1%
	set sungame=%param:~2,1%
	set engine=%param:~3,1%
	set shader=%param:~4,1%
	
	mkdir %upload%
	mkdir %logupload%
	

	rem �ϴ� sync�� �ص���
	p4 sync %upload%\...
	p4 sync %logupload%\...
	
    set curdir=%cd%
    
    if '%gameclient%'=='1' (
		echo �� GameClient.exe ����

		rem ���������� ���ε�
		xcopy %bindir%\GameClient.exe %upload%\client\ /y /r /f
		if exist %upload%\client\GameClient.exe (		
            p4 add %upload%\client\GameClient.exe
    		p4 edit %upload%\client\GameClient.exe
        )
        
        rem ����α������� ���ε�
        call %curdir%\batch\compare_upload.bat %bindir%\GameClient.exe %logupload%\client\GameClient.exe %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_Release.map %logupload%\dumpinfos\GameClient_Release.map %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_Release.pdb %logupload%\dumpinfos\GameClient_Release.pdb %curdir%\batch
	)

	if '%dump%'=='1' (
		echo �� Sungame_Dump.exe ����

		rem ���������� ���ε�
		xcopy %bindir%\Sungame_Dump.exe %upload%\client\ /y /r /f
		if exist %upload%\client\Sungame_Dump.exe (				
    		p4 add %upload%\client\Sungame_Dump.exe
    		p4 edit %upload%\client\Sungame_Dump.exe
        )
        
		rem ����α������� ���ε�
		call %curdir%\batch\compare_upload.bat %bindir%\Sungame_Dump.exe %logupload%\client\Sungame_Dump.exe %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_User_Dump.map %logupload%\dumpinfos\GameClient_User_Dump.map %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_User_Dump.pdb %logupload%\dumpinfos\GameClient_User_Dump.pdb %curdir%\batch
		
	)

	
    chdir /d %hsbdir%
    
	if '%sungame%'=='1' (
		echo �� Sungame.exe ����
		
		rem ���������� ���ε�
		xcopy %binpackdir%\Sungame.exe %upload%\client\ /y /r /f
		if exist %upload%\client\Sungame.exe (						
    		p4 add %upload%\client\Sungame.exe
    		p4 edit %upload%\client\Sungame.exe
        )
        
        xcopy AntiCpx.hsb %upload%\hsb\ /y /r /f
		if exist %upload%\hsb\AntiCpx.hsb (						        
    		p4 add %upload%\hsb\AntiCpx.hsb
    		p4 edit %upload%\hsb\AntiCpx.hsb
        )
		
		rem ����α������� ���ε�
		xcopy AntiCpx.hsb %logupload%\hsb\ /y /r /f
		xcopy %binpackdir%\Sungame.exe %logupload%\client\ /y /r /f

		if exist %logupload%\hsb\AntiCpx.hsb (
		    p4 add %logupload%\hsb\AntiCpx.hsb
		    p4 edit %logupload%\hsb\AntiCpx.hsb
        )
		
        if exist %logupload%\client\Sungame.exe (
		    p4 add %logupload%\client\Sungame.exe
		    p4 edit %logupload%\client\Sungame.exe
        )		    


		

	)
    
	chdir /d %curdir%
	
	if '%sungame%'=='1' (
		rem hsb ������ chdir ��� ����ϰ� �Ǿ� �̷��� ���� ����...
		call %curdir%\batch\compare_upload.bat %bindir%\Sungame.exe %logupload%\client\unpack\Sungame.exe %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_User.map %logupload%\dumpinfos\GameClient_User.map %curdir%\batch
		call %curdir%\batch\compare_upload.bat %dmpinfo%\GameClient_User.pdb %logupload%\dumpinfos\GameClient_User.pdb %curdir%\batch	
	)	


	if '%engine%'=='1' (
		echo �� Engine dll ����

		rem ������������ο� �ִ� dll �� ������bin��ο� copy
		xcopy %enginepath%\dll\3DFileAccess.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\3DFileAccess_p.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\3DRendererD3D.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\3DRendererD3D_p.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\3DTerrain.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\3DTerrain_p.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\Effect.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\Effect_p.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\ControlsDLL.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\ControlsDLL_p.dll %bindir%\ /c /f /r /y
		xcopy %enginepath%\dll\wzSound.dll %bindir%\ /c /f /r /y		
		
		rem ���������� ���ε�
		xcopy %bindir%\3DFileAccess.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\3DFileAccess_p.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\3DRendererD3D.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\3DRendererD3D_p.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\3DTerrain.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\3DTerrain_p.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\Effect.dll %upload%\client\ /c /f /r /y	
		xcopy %bindir%\Effect_p.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\ControlsDLL.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\ControlsDLL_p.dll %upload%\client\ /c /f /r /y
		xcopy %bindir%\wzSound.dll %upload%\client\ /c /f /r /y
												
		if exist %upload%\client\3DFileAccess.dll (
    		p4 add %upload%\client\3DFileAccess.dll
    		p4 edit %upload%\client\3DFileAccess.dll
		)
		
		if exist %upload%\client\3DFileAccess_p.dll (
		    p4 add %upload%\client\3DFileAccess_p.dll
		    p4 edit %upload%\client\3DFileAccess_p.dll
        )
		
        if exist %upload%\client\3DRendererD3D.dll (
    		p4 add %upload%\client\3DRendererD3D.dll
    		p4 edit %upload%\client\3DRendererD3D.dll
        )
        
        if exist %upload%\client\3DRendererD3D_p.dll (		
    		p4 add %upload%\client\3DRendererD3D_p.dll
    		p4 edit %upload%\client\3DRendererD3D_p.dll
        )						
				
        if exist %upload%\client\3DTerrain.dll (				
    		p4 add %upload%\client\3DTerrain.dll
    		p4 edit %upload%\client\3DTerrain.dll
        )
        						
        if exist %upload%\client\3DTerrain_p.dll (								
    		p4 add %upload%\client\3DTerrain_p.dll
    		p4 edit %upload%\client\3DTerrain_p.dll
        )						
				
        if exist %upload%\client\Effect.dll (										
    		p4 add %upload%\client\Effect.dll
    		p4 edit %upload%\client\Effect.dll
        )				
		
        if exist %upload%\client\Effect_p.dll (			
    		p4 add %upload%\client\Effect_p.dll
    		p4 edit %upload%\client\Effect_p.dll
        )		
		
        if exist %upload%\client\ControlsDLL.dll (				
    		p4 add %upload%\client\ControlsDLL.dll
    		p4 edit %upload%\client\ControlsDLL.dll
        )		

        if exist %upload%\client\ControlsDLL_p.dll (			
    		p4 add %upload%\client\ControlsDLL_p.dll
    		p4 edit %upload%\client\ControlsDLL_p.dll
        )		

        if exist %upload%\client\wzSound.dll (			
    		p4 add %upload%\client\wzSound.dll
    		p4 edit %upload%\client\wzSound.dll
        )
        
        rem ����α������� ���ε�
		xcopy %bindir%\3DFileAccess.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\3DFileAccess_p.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\3DRendererD3D.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\3DRendererD3D_p.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\3DTerrain.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\3DTerrain_p.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\Effect.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\Effect_p.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\ControlsDLL.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\ControlsDLL_p.dll %logupload%\client\ /c /f /r /y
		xcopy %bindir%\wzSound.dll %logupload%\client\ /c /f /r /y
		
        if exist %logupload%\client\3DFileAccess.dll (
		    p4 add %logupload%\client\3DFileAccess.dll
		    p4 edit %logupload%\client\3DFileAccess.dll
        )
		
        if exist %logupload%\client\3DFileAccess_p.dll (
		    p4 add %logupload%\client\3DFileAccess_p.dll
		    p4 edit %logupload%\client\3DFileAccess_p.dll
        )
        
        if exist %logupload%\client\3DRendererD3D.dll (
		    p4 add %logupload%\client\3DRendererD3D.dll
		    p4 edit %logupload%\client\3DRendererD3D.dll
        )
        
        if exist %logupload%\client\3DRendererD3D_p.dll (
		    p4 add %logupload%\client\3DRendererD3D_p.dll
		    p4 edit %logupload%\client\3DRendererD3D_p.dll
        )
        
        if exist %logupload%\client\3DTerrain.dll (
		    p4 add %logupload%\client\3DTerrain.dll
		    p4 edit %logupload%\client\3DTerrain.dll
        )
        
        if exist %logupload%\client\3DTerrain_p.dll (
		    p4 add %logupload%\client\3DTerrain_p.dll
		    p4 edit %logupload%\client\3DTerrain_p.dll
        )
        
        if exist %logupload%\client\Effect.dll (
		    p4 add %logupload%\client\Effect.dll
		    p4 edit %logupload%\client\Effect.dll
        )
        
        if exist %logupload%\client\Effect_p.dll (
		    p4 add %logupload%\client\Effect_p.dll
		    p4 edit %logupload%\client\Effect_p.dll
        )
        
        if exist %logupload%\client\ControlsDLL.dll (
		    p4 add %logupload%\client\ControlsDLL.dll
		    p4 edit %logupload%\client\ControlsDLL.dll
        )                                                                

        if exist %logupload%\client\ControlsDLL_p.dll (
		    p4 add %logupload%\client\ControlsDLL_p.dll
		    p4 edit %logupload%\client\ControlsDLL_p.dll
        )       
                
        if exist %logupload%\client\wzSound.dll (
		    p4 add %logupload%\client\wzSound.dll
		    p4 edit %logupload%\client\wzSound.dll
        )         
        
	)

	if '%shader%'=='1' (
		echo �� Shader ����
		
		rem ������������ο� �ִ� shader �� ������bin\system��ο� copy
		xcopy %enginepath%\shader\*.vso %bindir%\System\ /c /f /r /y
		xcopy %enginepath%\shader\*.pso %bindir%\System\ /c /f /r /y
				

		rem ���������� ���ε�
        xcopy %bindir%\System\*.vso %upload%\client\System\ /c /f /r /y
        xcopy %bindir%\System\*.pso %upload%\client\System\ /c /f /r /y

        p4 add %upload%\client\System/*.vso
        p4 edit %upload%\client\System/*.vso
        
        p4 add %upload%\client\System/*.pso
        p4 edit %upload%\client\System/*.pso

        
        rem ����α������� ���ε�
		xcopy %bindir%\System\*.vso %logupload%\client\System\ /c /f /r /y
		xcopy %bindir%\System\*.pso %logupload%\client\System\ /c /f /r /y

		p4 add %logupload%\client\System\*.*
		p4 edit %logupload%\client\System\*.*
        
	)

	
	xcopy %logdir%\BuildSystemResult.txt %logupload%\log\ /c /f /r /y

	if exist %logupload%\log\BuildSystemResult.txt (
		p4 add %logupload%\log\BuildSystemResult.txt        
		p4 edit %logupload%\log\BuildSystemResult.txt        
    	)
		
	 
	call %curdir%\batch\compare_upload.bat %logdir%\svn_tag_log.txt %logupload%\log\svn_tag_log.txt %curdir%\batch
	call %curdir%\batch\compare_upload.bat %logdir%\svn_update_log.txt %logupload%\log\svn_update_log.txt %curdir%\batch

echo curdir: %curdir%

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