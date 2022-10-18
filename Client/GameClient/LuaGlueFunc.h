//=======================================================================================================================
/** 루아 글루 함수 모음.
	@author
			유재영 < y2jinc@webzen.co.kr >
	@desc 
			루아에서 호출 할수 있는 함수를 모아놓는다.
	@since
			2006. 2. 2
	@remarks
		- !!반드시 정의부에 파라메터에 대한 정의 및 설명을 넣어주세요
		- 함수명은 LG_ 로 시작 해서 선언해 주시기 바랍니다.
*/

#ifdef LUAGLUEFUNC_H
#define LUAGLUEFUNC_H

#pragma once

#include <clua.h>

#ifdef _USE_LUA_SCRIPT

extern luaDef SunGlue[];

// SET FUNCTIONS
LuaGlue LG_SetObjectPosition(lua_State *L);
LuaGlue LG_SetWindowPosition(lua_State *L);

// GET FUNCTIONS
LuaGlue LG_GetPlayerCount(lua_State *L);
LuaGlue LG_GetMonsterCount(lua_State *L);

// ETC FUNCTIONS
LuaGlue LG_TEST_OpenComposte(lua_State *L);
LuaGlue LG_TEST_OpenSocketExtract(lua_State *L);
LuaGlue LG_TEST_OpenEnchant(lua_State *L);
LuaGlue LG_TEST_OpenRankup(lua_State *L);
LuaGlue LG_TEST_AttachTestCase(lua_State *L);
LuaGlue LG_TEST_DeAttachTestCase(lua_State *L);
LuaGlue LG_TEST_OpenHelp(lua_State *L);
LuaGlue LG_TEST_OpenHelpString(lua_State *L);
LuaGlue LG_TEST_OpenGMHelp(lua_State *L);
LuaGlue	LG_TEST_AddMissionHelp(lua_State *L);
LuaGlue	LG_TEST_DelMissionHelp(lua_State *L);
LuaGlue LG_TEST_AppendFormatText(lua_State *L);
LuaGlue LG_TEST_ClearFormatText(lua_State *L);
LuaGlue LG_TEST_SetFormatTextRect(lua_State *L);
LuaGlue LG_TEST_TooltipItem(lua_State *L);
LuaGlue LG_TEST_TooltipClear(lua_State *L);
LuaGlue LG_NPC_MEET(lua_State *L);
LuaGlue LG_CREATE_HERO(lua_State *L);
LuaGlue LG_Load_Map(lua_State *L);
LuaGlue LG_Load_Map_All(lua_State *L);
LuaGlue LG_SetLevel(lua_State *L);
LuaGlue LG_AddItemAtInventory(lua_State *L);
LuaGlue LG_SetMoney(lua_State *L);
LuaGlue LG_TEST_OpenQuestList(lua_State *L);
LuaGlue LG_TEST_SaveQuestList(lua_State *L);
LuaGlue LG_TEST_LoadQuestList(lua_State *L);
LuaGlue LG_TEST_MONSTER_KILL_CMD(lua_State *L);
LuaGlue LG_TEST_SettingNPC(lua_State *L);
LuaGlue LG_TEST_RefreshNPCState(lua_State *L);
LuaGlue LG_TEST_InitQuestAreaInfo(lua_State *L);
LuaGlue LG_TEST_Quest_Accept(lua_State * L);
LuaGlue LG_TEST_ShowGameInterface(lua_State *L);
LuaGlue LG_ToggleACDialog(lua_State *L);
LuaGlue LG_TEST_EventWindow(lua_State *L);
LuaGlue LG_TEST_Exit(lua_State *L);
LuaGlue LG_TEST_SetMissionPosint(lua_State *L)
LuaGlue LG_TEST_OpenKillPoint(lua_State *L);
LuaGlue LG_TEST_SetKillPoint(lua_State *L);
LuaGlue LG_TEST_CMD_AUTO_QUEST(lua_State *L);
LuaGlue LG_TEST_CMD_VENDOR_BUY(lua_State *L);
LuaGlue	LG_TEST_LOAD_ENDURANCE(lua_State *L);
LuaGlue LG_TEST_SETDURA(lua_State *L);
LuaGlue LG_TEST_WayPointWindow(lua_State *L);
LuaGlue LG_TEST_SaveWayPoint(lua_State *L);
LuaGlue LG_TEST_TestWayPoint(lua_State *L);
LuaGlue LG_TEST_AttackJump(lua_State *L);
LuaGlue LG_TEST_EventJump(lua_State *L);
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
LuaGlue LG_TEST_MissionRankList(lua_State *L);
LuaGlue LG_TEST_MissionRankGamble(lua_State *L);
LuaGlue LG_TEST_RqstMissionRank(lua_State *L);
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
LuaGlue LG_TEST_SendMoveNak(lua_State *L);
LuaGlue LG_TEST_jjumbo( lua_State *L);
LuaGlue LG_TEST_UpdateAutoQuestRequest( lua_State *L );
LuaGlue LG_TEST_SpawnMonster( lua_State *L );
LuaGlue LG_TEST_RunAway( lua_State * L );
LuaGlue LG_TEST_CurrentPos( lua_State * L );
LuaGlue LG_TEST_RqstMemoList( lua_State * L );
LuaGlue LG_TEST_ShowCashItemShopNpc( lua_State * L );
LuaGlue LG_TEST_ShowBillingAlarm( lua_State * L );
LuaGlue LG_TEST_ShowDisconnectAlarm( lua_State * L );
LuaGlue LG_TEST_PopupMesh( lua_State * L );
LuaGlue LG_TEST_DeleteItemCmd( lua_State * L );
LuaGlue LG_TEST_SetBrightDelay( lua_State * L );
LuaGlue LG_TEST_PlayEffectSound( lua_State * L );
LuaGlue LG_TEST_PlayEffectSounds( lua_State * L );
LuaGlue LG_TEST_SpawnFullsetPlayer( lua_State * L);
LuaGlue LG_TEST_StopAllVoice( lua_State * L );
LuaGlue LG_TEST_ItemDestroyDuraCmd( lua_State * L );
LuaGlue LG_TEST_InsertInvertoryItem(lua_State * L);
LuaGlue LG_TEST_SetProjection(lua_State * L);
LuaGlue LG_TEST_CreateEtherWeaponEffect(lua_State * L);
LuaGlue LG_TEST_DestoryEtherWeaponEffect(lua_State * L);

LuaGlue LG_TEST_OpenSetLottoNum(lua_State * L);
LuaGlue LG_TEST_OpenLottoWindow(lua_State * L);
LuaGlue LG_TEST_OpenLottoNumInput(lua_State * L);

LuaGlue LG_TEST_CreateEffect(lua_State * L);
LuaGlue LG_TEST_SetGrayScale(lua_State * L);
LuaGlue LG_TEST_SetGrayScaleColor(lua_State * L);
LuaGlue LG_TEST_SetSepiaMode(lua_State * L);
LuaGlue LG_TEST_SSQRewardFail(lua_State * L);
LuaGlue LG_TEST_ForceMoveToHero(lua_State * L);
LuaGlue LG_TEST_MissionRelay(lua_State * L);
LuaGlue LG_TEST_OpenPVPScore(lua_State * L);
LuaGlue LG_TEST_LottoActive(lua_State * L);
LuaGlue LG_TEST_SendMSG(lua_State * L);
LuaGlue LG_TEST_CreateRider(lua_State * L);
LuaGlue LG_TEST_RiderPart(lua_State * L);
LuaGlue LG_TEST_SetSpeed(lua_State * L);
LuaGlue LG_TEST_CreateRiderPlayer(lua_State * L);
LuaGlue LG_TEST_DeleteEffect(lua_State * L);
#endif

LuaGlue LG_TEST_AssertTest(lua_State * L);
LuaGlue LG_TEST_JumpGravity(lua_State * L);
LuaGlue LG_TEST_JumpForce(lua_State * L);
LuaGlue LG_TEST_MotionBlur(lua_State * L);

LuaGlue LG_TEST_MemoryUsage(lua_State * L);

LuaGlue LG_TEST_OpenCharCustomize(lua_State * L);

//! hero 길찾기 seek node 갯수 설정
LuaGlue LG_TEST_SetSeekNodeCount(lua_State * L);
LuaGlue LG_TEST_SetShadowDepthOff(lua_State * L);

LuaGlue LG_TEST_OpenSocketComposite(lua_State* L);

LuaGlue LG_TEST_OpenItemCube(lua_State*);

#ifdef _DEV_VER //_DH_GM_ITEM_WINDOW
LuaGlue LG_TEST_OpenGMWin(lua_State*);
#endif//_DEV_VER//_DH_GM_ITEM_WINDOW
// _USE_LUA_SCRIPT
#endif
// LUAGLUEFUNC_H
