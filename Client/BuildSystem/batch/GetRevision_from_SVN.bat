@echo off
cls

rem �Ű�����- localfolder revision log����
	if '%1'=='' goto errlog


	set WZ_WORKFOLD=%1
	set rev=%2
	set logfile=%3\svn_update_log.txt

@echo.
@echo �� Revision(%rev%) �ҽ� �������� ��... (to %WZ_WORKFOLD%)
@echo.

	svn update -r %rev% %WZ_WORKFOLD% > %logfile%

@echo.
@echo �� Revision(%rev%) Update �Ϸ� (to %WZ_WORKFOLD%)
@echo.

exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause

