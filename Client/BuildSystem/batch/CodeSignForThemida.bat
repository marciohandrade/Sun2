@echo off

rem �Ű�����- �۾����� �ڵ������ �������� path (���̳ʸ� ��ŷ �� HSB ������ �Ϸ�� ���� ��ġ)

if '%1'=='' goto errlog

set strName="SUN Online"
set strInfoURL=http://sunonline.hangame.com/

set workdir=%1
set strFile=%2

::-----------------------------------------------------------------
rem ���� �����ڵ� ���� (������. WZ_Cert_Cli_Build ���� ����)
::set strSignCode=%workdir%\Client\BuildSystem\code_signing\signcode.exe
::set strSignCodePwd=%workdir%\Client\BuildSystem\code_signing\signcode-pwd.exe
::set strSpc=%workdir%\Client\BuildSystem\code_signing\www.webzen.com.spc
::set strPvk=%workdir%\Client\BuildSystem\code_signing\www.webzen.com.pvk

rem signcode-pwd.exe �� ���� ��й�ȣ�� �ڵ����� �Է����ִ� ��ƿ��Ƽ��. (���� ������� ���þ��� ���Ϸ�, ���� ���Ǹ� ���� ������� ��ƿ)
rem code sign�� ���߸����� �����ؾ��ϹǷ�, timestamp�� ������ �������� ����. 
rem Ÿ�ӽ������� ���� �ڵ������ ���� ��� ������ ��ȿ�Ⱓ�� ������, �ڵ���� �� ������ü�� ��ȿȭ �ȴٰ� �ϴ�, ���� �ٶ�.
rem set strTimeStampUrl=http://timestamp.comodoca.com/authenticode

::echo Start signcode-pwd.exe
::%strSignCodePwd% -m @wjtmt1tm

::echo Execute signcode.exe
::%strSignCode% -spc %strSpc% -v %strPvk% -p "Microsoft Strong Cryptographic Provider" -a sha1 -n %strName% -i %strInfoURL% %strFile%

::rem %strSignCode% -n %strName% -i %strInfoURL% -a sha1 -spc %strSpc% -v %strPvk% -t %strTimeStampUrl% %strFile%

::-----------------------------------------------------------------
rem WZ_Cert_Cli_Build ���

set strCert=%workdir%\Client\BuildSystem\WzAppCertBuilder\WZ_Cert_Cli_Build.exe

echo Execute WZ_Cert_Cli_Build.exe

rem �Ű����� - ��������.exe ������� ����AD���� �н�����
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
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.-------------------------------------------------------------------------

pause
exit /b 1