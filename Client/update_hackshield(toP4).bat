@echo off

set /p latest_hackshield=�� �ֽ� �ٽ��� ���� ��θ� �Է��ϼ��� ( ex. E:\Perforce\hackshield\R.5.2.5.1(build847)_TS_20090717\Korean(kr)-SDK ) :

@echo %latest_hackshield%

set /p p4path=�� Update�� Perforce HShield��θ� �Է��ϼ��� ( ex. E:\Perforce\test_release\3.������Ʈ����\SUN_Client\HShield ) :

@echo %p4path%


xcopy %latest_hackshield%\Bin\HShield\*.* %p4path% /y /r /f /h /k /s
xcopy %latest_hackshield%\Bin\Update\*.* %p4path% /y /r /f /h /k /s /exclude:exclude_hackshield.xcopy

p4 edit %p4path%\...

@echo Perforce�� ����, Submit�� �����ϼ���

pause 