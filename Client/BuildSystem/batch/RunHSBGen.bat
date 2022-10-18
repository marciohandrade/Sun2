@echo off

rem 매개변수- 실행파일폴더 pack파일폴더 HSBGen.exe의path HSB_Unpack파일path HSB_Pack파일path AntiCpx.hsb의outputpath LMP삽입여부

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
	rem 실행파일경로,pack파일사용여부(1 or 0),패킹파일경로,패킹파일실행여부(1 or 0),hsb파일경로,실행파일여부(1은 exe, 0은 dll),LMP기능적용여부(1 or 0)
	rem LMP기능은 dll인 경우에만 적용됨 
	HSBGen.exe %hsb_unpackpath%,1,%hsb_packpath%,0,%hsb_filepath%\AntiCpx.hsb,1,%lmp_option%

	chdir /d %cur_dir%
	
	rem 원본pack파일폴더로 sungame.exe 결과를 다시 copy (hsb 생성시 실행파일이 변경됨)
	xcopy %hsb_packpath% %binpackdir%\ /y /r /f
	
	goto end



:end

@echo.
@echo ▷ 완료
@echo.
exit /b 0




:errlog
echo.
echo ▷ 매개변수가 잘못 되었습니다.
echo.

pause