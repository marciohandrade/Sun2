@echo off

rem �Ű�����-  ������SUNEnginePath ����ƮSUNEnginePath

if '%1'=='' goto errlog

set custom_engine_path=%1
set default_engine_path=%2


echo.
echo �� ������ ���� path: %custom_engine_path%
echo �� ����Ʈ ���� path: %default_engine_path%
echo.

xcopy %custom_engine_path%\*.* %default_engine_path%\*.* /e /c /f /r /y

exit /b 0



:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause