@echo off
cls

rem �Ű�����- localfolder log����
	if '%1'=='' goto errlog
	

	set WZ_WORKFOLD=%1
	set logfile=%2\svn_update_log.txt

@echo.
@echo �� %WZ_WORKFOLD% �ֽ� �ҽ� �������� ��...
@echo.
	
	svn update %WZ_WORKFOLD% > %logfile%

@echo.
@echo �� %WZ_WORKFOLD% �ֽ� �ҽ� Update �Ϸ�
@echo.
	
exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause

