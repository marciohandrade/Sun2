@echo off
cls

set bStart=n
set /p bStart=�� OBJ ������ �����Ͻðڽ��ϱ�? (y,n)
if not '%bStart%'=='y' goto skip


@echo.
@echo �� AreaConquest OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\AreaConquest\Debug" /s /q
rmdir "..\AreaConquest\Release" /s /q

@echo.
@echo �� PartyLib OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\PartyLib\Debug" /s /q
rmdir "..\PartyLib\Release" /s /q

@echo.
@echo �� PortalParty OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\PortalParty\Debug" /s /q
rmdir "..\PortalParty\Release" /s /q

@echo.
@echo �� ProgramCommon OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\ProgramCommon\Obj" /s /q
rmdir "..\ProgramCommon\AK Test Release - Ignore AK" /s /q
rmdir "..\ProgramCommon\AK Test Release using DLL" /s /q
rmdir "..\ProgramCommon\Debug" /s /q
rmdir "..\ProgramCommon\Release" /s /q
rmdir "..\ProgramCommon\Release - ���� ���� ����" /s /q
rmdir "..\ProgramCommon\Release - ���� ���� ���� ( ���� ���� )" /s /q
rmdir "..\ProgramCommon\Release-Ignore AK" /s /q
rmdir "..\ProgramCommon\Test Release - ���� ���� ����" /s /q
rmdir "..\ProgramCommon\Test Release - ���� ����(����)" /s /q

@echo.
@echo �� RankingLib OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\RankingLib\Debug" /s /q
rmdir "..\RankingLib\Release" /s /q

@echo.
@echo �� SolarQuest OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\SolarQuest\Debug" /s /q
rmdir "..\SolarQuest\Release" /s /q


@echo.
@echo �� SolarShop OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\SolarShop\Debug" /s /q
rmdir "..\SolarShop\Release" /s /q


@echo.
@echo �� SolarTrigger OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.


rmdir "..\SolarTrigger\Debug" /s /q
rmdir "..\SolarTrigger\Release" /s /q

@echo.
@echo �� Utility OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\Utility\Debug" /s /q
rmdir "..\Utility\Release" /s /q


@echo.
@echo �� GameClient OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir ".\GameClient\Obj" /s /q 
rmdir ".\GameClient\Debug" /s /q 
rmdir ".\GameClient\Release" /s /q 

@echo.
@echo �� Lua_library OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir ".\Lua_library\Debug" /s /q 
rmdir ".\Lua_library\Release" /s /q 

@echo.
@echo �� SolarClientNetwork OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir ".\SolarClientNetwork\Debug" /s /q 
rmdir ".\SolarClientNetwork\Release" /s /q 


@echo.
@echo �� SolarSlot OBJ ���� ������.. ��ø� ��ٷ� �ּ���
@echo.

rmdir "..\SolarSlot\Debug" /s /q 
rmdir "..\SolarSlot\Release" /s /q 


@echo.
@echo �� �Ϸ�
@echo.

pause

:skip