@echo off

rem �Ű����� - localfolder log����
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	set logfile=%2\svn_update_ClientVer_log.txt
	

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h �������� ��...
@echo.
	
	svn update %WZ_WORKFOLD%\Client\GameClient\ClientVer.h > %logfile%

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Update �Ϸ�
@echo.
	
exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause



