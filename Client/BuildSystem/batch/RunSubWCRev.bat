@echo off

rem �Ű�����- localfolder 
	if '%1'=='' goto errlog

	set WZ_WORKFOLD=%1

del %WZ_WORKFOLD%\Client\BuildSystem\batch\TagToSVN.bat /f /q

echo Create %WZ_WORKFOLD%\Client\BuildSystem\batch\TagToSVN.bat
SubWCRev.exe %WZ_WORKFOLD% %WZ_WORKFOLD%\Client\BuildSystem\batch\TagToSVN.sample %WZ_WORKFOLD%\Client\BuildSystem\batch\TagToSVN.bat

exit /b 0


:errlog
echo.
echo �� �Ű����� ���� ������ �� �����ϴ�.
echo.

pause