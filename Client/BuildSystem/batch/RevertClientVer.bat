@echo off

rem �Ű����� - localfolder
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Revert...
@echo.
	
	svn revert %WZ_WORKFOLD%\Client\GameClient\ClientVer.h

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Revert �Ϸ�
@echo.
	
exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause



