@echo off

set /p latest_hackshield=☞ 최신 핵쉴드 버전 경로를 입력하세요 ( ex. E:\Perforce\hackshield\R.5.2.5.1(build847)_TS_20090717\Korean(kr)-SDK ) :

@echo %latest_hackshield%

set /p p4path=☞ Update할 Perforce HShield경로를 입력하세요 ( ex. E:\Perforce\test_release\3.업데이트버전\SUN_Client\HShield ) :

@echo %p4path%


xcopy %latest_hackshield%\Bin\HShield\*.* %p4path% /y /r /f /h /k /s
xcopy %latest_hackshield%\Bin\Update\*.* %p4path% /y /r /f /h /k /s /exclude:exclude_hackshield.xcopy

p4 edit %p4path%\...

@echo Perforce를 열고, Submit을 실행하세요

pause 