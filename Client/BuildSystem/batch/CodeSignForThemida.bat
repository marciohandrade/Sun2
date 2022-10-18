@echo off

rem 매개변수- 작업폴더 코드사인할 실행파일 path (바이너리 패킹 및 HSB 추출이 완료된 파일 위치)

if '%1'=='' goto errlog

set strName="SUN Online"
set strInfoURL=http://sunonline.hangame.com/

set workdir=%1
set strFile=%2

::-----------------------------------------------------------------
rem 수동 싸인코드 서명 (사용안함. WZ_Cert_Cli_Build 으로 변경)
::set strSignCode=%workdir%\Client\BuildSystem\code_signing\signcode.exe
::set strSignCodePwd=%workdir%\Client\BuildSystem\code_signing\signcode-pwd.exe
::set strSpc=%workdir%\Client\BuildSystem\code_signing\www.webzen.com.spc
::set strPvk=%workdir%\Client\BuildSystem\code_signing\www.webzen.com.pvk

rem signcode-pwd.exe 는 서명 비밀번호를 자동으로 입력해주는 유틸리티임. (인증 기관과는 관련없는 파일로, 유저 편의를 위해 만들어진 유틸)
rem code sign은 개발망에서 진행해야하므로, timestamp는 정보는 세팅하지 않음. 
rem 타임스탬프가 없는 코드사인을 했을 경우 인증서 유효기간이 지나면, 코드사인 즉 서명자체가 무효화 된다고 하니, 주의 바람.
rem set strTimeStampUrl=http://timestamp.comodoca.com/authenticode

::echo Start signcode-pwd.exe
::%strSignCodePwd% -m @wjtmt1tm

::echo Execute signcode.exe
::%strSignCode% -spc %strSpc% -v %strPvk% -p "Microsoft Strong Cryptographic Provider" -a sha1 -n %strName% -i %strInfoURL% %strFile%

::rem %strSignCode% -n %strName% -i %strInfoURL% -a sha1 -spc %strSpc% -v %strPvk% -t %strTimeStampUrl% %strFile%

::-----------------------------------------------------------------
rem WZ_Cert_Cli_Build 사용

set strCert=%workdir%\Client\BuildSystem\WzAppCertBuilder\WZ_Cert_Cli_Build.exe

echo Execute WZ_Cert_Cli_Build.exe

rem 매개변수 - 실행파일.exe 대상파일 개발AD계정 패스워드
%strCert% %strFile% naklu3@webzendev.com thdckswhd34!@

::-----------------------------------------------------------------


if errorlevel 0 goto end_success

:end_error
echo Error occured while signing
::%strSignCodePwd% -t
pause
exit /b 1

:end_success
echo Singing was successfully
::%strSignCodePwd% -t
exit /b 0


:errlog
echo.-------------------------------------------------------------------------
echo ▷ 매개변수가 잘못 되었습니다.
echo.-------------------------------------------------------------------------

pause
exit /b 1