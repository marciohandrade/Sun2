@echo off

rem �Ű����� - localfolder ���������� ��������
	if '%1'=='' goto errlog
		
	set WZ_WORKFOLD=%1
	set strdef=%2
	set strver=%3
	set logmsg="%strdef%: Update Client Build Ver. %strver%"

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Commit...
@echo.
	
	svn commit -m %logmsg% %WZ_WORKFOLD%\Client\GameClient\ClientVer.h

@echo.
@echo �� %WZ_WORKFOLD%\Client\GameClient\ClientVer.h Commit �Ϸ�
@echo.
	
exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause



