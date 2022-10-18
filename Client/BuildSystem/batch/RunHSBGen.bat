@echo off

rem �Ű�����- ������������ pack�������� HSBGen.exe��path HSB_Unpack����path HSB_Pack����path AntiCpx.hsb��outputpath LMP���Կ���

if '%1'=='' goto errlog

	set bindir=%1
	set binpackdir=%2
	set hsb_gen_path=%3
	set hsb_unpackpath=%4
	set hsb_packpath=%5
	set hsb_filepath=%6
	set lmp_option=%7
    
	set cur_dir=%cd%

	xcopy %bindir%\Sungame.exe %hsb_unpackpath% /y /r /f	
	xcopy %binpackdir%\Sungame.exe %hsb_packpath% /y /r /f


	chdir /d %hsb_gen_path%
              attrib -R *.*

	rem hsbgen command parameter usage
	rem �������ϰ��,pack���ϻ�뿩��(1 or 0),��ŷ���ϰ��,��ŷ���Ͻ��࿩��(1 or 0),hsb���ϰ��,�������Ͽ���(1�� exe, 0�� dll),LMP������뿩��(1 or 0)
	rem LMP����� dll�� ��쿡�� ����� 
	HSBGen.exe %hsb_unpackpath%,1,%hsb_packpath%,0,%hsb_filepath%\AntiCpx.hsb,1,%lmp_option%

	chdir /d %cur_dir%
	
	rem ����pack���������� sungame.exe ����� �ٽ� copy (hsb ������ ���������� �����)
	xcopy %hsb_packpath% %binpackdir%\ /y /r /f
	
	goto end



:end

@echo.
@echo �� �Ϸ�
@echo.
exit /b 0




:errlog
echo.
echo �� �Ű������� �߸� �Ǿ����ϴ�.
echo.

pause