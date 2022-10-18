@echo off
cls

set bStart=n
set /p bStart=☞ OBJ 파일을 제거하시겠습니까? (y,n)
if not '%bStart%'=='y' goto skip


@echo.
@echo ▷ AreaConquest OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\AreaConquest\Debug" /s /q
rmdir "..\AreaConquest\Release" /s /q

@echo.
@echo ▷ PartyLib OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\PartyLib\Debug" /s /q
rmdir "..\PartyLib\Release" /s /q

@echo.
@echo ▷ PortalParty OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\PortalParty\Debug" /s /q
rmdir "..\PortalParty\Release" /s /q

@echo.
@echo ▷ ProgramCommon OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\ProgramCommon\Obj" /s /q
rmdir "..\ProgramCommon\AK Test Release - Ignore AK" /s /q
rmdir "..\ProgramCommon\AK Test Release using DLL" /s /q
rmdir "..\ProgramCommon\Debug" /s /q
rmdir "..\ProgramCommon\Release" /s /q
rmdir "..\ProgramCommon\Release - 유저 배포 버전" /s /q
rmdir "..\ProgramCommon\Release - 유저 배포 버전 ( 덤프 남김 )" /s /q
rmdir "..\ProgramCommon\Release-Ignore AK" /s /q
rmdir "..\ProgramCommon\Test Release - 유저 배포 버전" /s /q
rmdir "..\ProgramCommon\Test Release - 유저 배포(덤프)" /s /q

@echo.
@echo ▷ RankingLib OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\RankingLib\Debug" /s /q
rmdir "..\RankingLib\Release" /s /q

@echo.
@echo ▷ SolarQuest OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\SolarQuest\Debug" /s /q
rmdir "..\SolarQuest\Release" /s /q


@echo.
@echo ▷ SolarShop OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\SolarShop\Debug" /s /q
rmdir "..\SolarShop\Release" /s /q


@echo.
@echo ▷ SolarTrigger OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.


rmdir "..\SolarTrigger\Debug" /s /q
rmdir "..\SolarTrigger\Release" /s /q

@echo.
@echo ▷ Utility OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\Utility\Debug" /s /q
rmdir "..\Utility\Release" /s /q


@echo.
@echo ▷ GameClient OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir ".\GameClient\Obj" /s /q 
rmdir ".\GameClient\Debug" /s /q 
rmdir ".\GameClient\Release" /s /q 

@echo.
@echo ▷ Lua_library OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir ".\Lua_library\Debug" /s /q 
rmdir ".\Lua_library\Release" /s /q 

@echo.
@echo ▷ SolarClientNetwork OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir ".\SolarClientNetwork\Debug" /s /q 
rmdir ".\SolarClientNetwork\Release" /s /q 


@echo.
@echo ▷ SolarSlot OBJ 파일 제거중.. 잠시만 기다려 주세요
@echo.

rmdir "..\SolarSlot\Debug" /s /q 
rmdir "..\SolarSlot\Release" /s /q 


@echo.
@echo ▷ 완료
@echo.

pause

:skip