@echo off
set choice_get_p4=n
set /p choice_get_p4=☞ 퍼포스 데이터를 새로 받으시겠습니까? (y,n)?
if not '%choice_get_p4%'=='y' goto skip_get_p4

p4 sync //test_release/3.업데이트버전/SUN_Client/Data/...

:skip_get_p4

set choice_get=n
set /p choice_get=☞ 퍼포스 데이터를 D:\Project\Client\_bin\Data 로 가져오시겠습니까? (y,n)?
if not '%choice_get%'=='y' goto skip_get
	
xcopy D:\Perforce\test_release\3.업데이트버전\SUN_Client\Data D:\Project\Client\_bin\Data /a /d /e /c /w /f /h /r /y

:skip_get