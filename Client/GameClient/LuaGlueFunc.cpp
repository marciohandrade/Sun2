#include "SunClientPrecompiledHeader.h"
#include "LuaGlueFunc.h"
#include "Object.h"
#include "hero.h"
#include "ObjectManager.h"
#include <cLua.h>
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "ItemETCCompositeDlg.h"
#include "uiHelpMan/uiHelpMan.h"
#include "SCItemSlot.h"
#include "slotkeygenerator.h"
#include "TestUIScene_Ext01.h"
#include "TestUIScene_Ext02.h"
#include "TestCaseMsg.h"
#include "GameFunc.h"
#include "Map.h"
#include "Camera.h"
#include "ItemManager.h"
#include "hero.h"
#include "uiQuestMan/uiQuestMan.h"
#include "QuestManager_Concrete.h"
#include "QuestInfoManagerex.h"
#include "RewardDialog.h"
#include "uiEventMan/uiEventMan.h"
#include "MissionManager_Concrete.h"
#include "uiBattleZoneDoing/uiBattleZoneDoing.h"
#include "uiWayPointMan/uiWayPointMan.h"
#include "Application.h"
#include "CastLeaveFieldDialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "HeroActionInput.h"
#include "MapInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ChatDialog.h"
#include "PcbangPlayTimeDialog.h"
#include "ObjectManager.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "MissionRelay.h"
#include "uiSystemMan/uiSystemMan.h"

#ifdef _JBH_ADD_GUILD_POINT_EXCHANGE
#include "uiGuildConvertGP.h"
#include "uiGuildWindow/uiGuildWindow.h"
#endif

#ifdef _JBH_ADD_PERSONAL_VENDOR_LIST
#include "uiVendorList/uiVendorList.h"
#endif	

#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "SoundEffect.h"
#include <ItemInfoParser.h>
#include <scslotHeader.h>
#include <SCSlot.h>
#include <SCSlotContainer.h>
#include "MouseHandler.h"
#include "EquipmentContainer.h"
#include "uiSSQMan/uiSSQManager.h"

#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoWindow/uiLottoWindow.h"
#include "uiLottoNumInput/uiLottoNumInput.h"

#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"

#include "Rider.h"

#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#ifdef _DEV_VER //_DH_GM_ITEM_WINDOW
#include "uiBaseMan.h"
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "interfacemanager.h"
#endif//_DEV_VER //_DH_GM_ITEM_WINDOW

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
#endif

#include "TextureAnimationManager.h"


#include "message.h"
#include "CommandManager.h"
#include "uiConvenienceStore/uiConvenienceStoreMan.h"
#include "HeroData.h"

#ifdef _USE_MSXML
#import <msxml4.dll>
using namespace MSXML2;

#endif

#ifdef _USE_LUA_SCRIPT

 
#pragma warning ( push )
#pragma warning ( disable : 4244)

luaDef SunGlue[];

//------------------------------------------------------------------------------
/**
	@desc : 오브젝트의 위치를 바꾸는 함수.

	@pram info :
		[IN] DWORD KEY, FLOAT X, FLOAT Y, FLOAT Z
		[OUT] BOOL
	@return :
*/
LuaGlue LG_SetObjectPosition(lua_State *L)
{
	int argNum = 1;

	DWORD dwObjectKey	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);
	float x				= (float) GET_LUA()->GetNumberArgument(argNum++);
	float y				= (float) GET_LUA()->GetNumberArgument(argNum++);
	float z				= (float) GET_LUA()->GetNumberArgument(argNum++);

	Object * pObject = g_ObjectManager.GetObject(dwObjectKey);

	if ( pObject )
	{
		WzVector wzVec;
		wzVec.x = x;
		wzVec.y = y;
		wzVec.z = z;
		pObject->SetPosition(wzVec);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
	@desc : 오브젝트의 위치를 바꾸는 함수.

	@pram info :
		[IN] STRING DLG ID, FLOAT X, FLOAT Y
		[OUT] BOOL
	@return :
*/
LuaGlue LG_SetWindowPosition(lua_State *L)
{
	int argNum = 1;
	const char * pszGetDlgID = GET_LUA()->GetStringArgument(argNum++);
	float x			  = (float) GET_LUA()->GetNumberArgument(argNum++);
	float y			  = (float) GET_LUA()->GetNumberArgument(argNum++);
	
	char szDlgID[256];
	strncpy( szDlgID, pszGetDlgID, strlen(pszGetDlgID));
	WzID Id = StrToWzID(szDlgID);
	SolarDialog * pDlg = g_InterfaceManager.GetDialog( Id );

	if ( pDlg )
	{
        pDlg->GetDialogWZ()->MoveWindowWZ(x, y);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
	@desc : 유저 카운트를 알아내는 함수

	@pram info :
		[IN] DWORD KEY, FLOAT X, FLOAT Y, FLOAT Z
	@return :
		Player Counter
*/
LuaGlue LG_GetPlayerCount(lua_State *L)
{
	GET_LUA()->PushNumber( g_ObjectManager.GetObjectCount(PLAYER_OBJECT) );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
	desc : 몬스터 카운트를 알아내는 함수

	pram info:
		[IN] DWORD KEY, FLOAT X, FLOAT Y, FLOAT Z

	return :
		Player Counter
*/
LuaGlue LG_GetMonsterCount(lua_State *L)
{
	GET_LUA()->PushNumber( g_ObjectManager.GetObjectCount(MONSTER_OBJECT) );

	return TRUE;
}


//------------------------------------------------------------------------------
// ETC FUNCS

//-------------------------------------------------------------------------------------------
/** 
	desc : 조합 창을 연다.

	pram info:
		[IN] VOID
		[OUT] VOID
	return :		
*/
LuaGlue LG_TEST_OpenComposte(lua_State *L)
{
	// test
	// 조합
	//uiItemCompositeMan * pCompositeMan = (uiItemCompositeMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE);
	//if ( pCompositeMan )
	//{
	//	static int CompositeCode = eITEMCOMPOSITE_LOW_RING;
	//	CompositeCode = 
	//		(CompositeCode > eITEMCOPOSITE_EXTRASTONE_CREATE)? 
	//				eITEMCOMPOSITE_LOW_RING : min( CompositeCode, eITEMCOPOSITE_EXTRASTONE_CREATE);

	//	pCompositeMan->ShowItemComposite(ITEM_COMPOSITE_CATEGORY_COMPOSITE, (eITEMCOMPOSITECODE)CompositeCode );
	//	CompositeCode = CompositeCode + 1;
	//}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
	desc : 결정화 창을 연다.

	pram info:
		[IN] VOID
		[OUT] VOID
	return :		
*/

//-------------------------------------------------------------------------------------------
/** 
desc : 큐브 창을 연다.		
*/
LuaGlue LG_TEST_OpenItemCube(lua_State *)
{
    uiItemCube* dialog = GET_CAST_DIALOG(uiItemCube, IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE);
    if (dialog)
    {
        dialog->ShowInterface(TRUE);
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
	desc : 옵션 제거 창을 연다.

	pram info:
		[IN] VOID
		[OUT] VOID
	return :		
*/
LuaGlue LG_TEST_OpenSocketExtract(lua_State *L)
{
	ItemETCCompositeDlg * pDlg = (ItemETCCompositeDlg *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ITEM_ETC_COMPOSE);
	if ( pDlg )
	{
		pDlg->SetCurrentCompositeType( eITEMCOMPOSITE_EXTRACT_SOCKET );
		pDlg->ShowDialog(TRUE);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
	desc : 인첸트 창을 연다.

	pram info:
		[IN] VOID
		[OUT] VOID
	return :		
*/
LuaGlue LG_TEST_OpenEnchant(lua_State *L)
{
	g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_ENCHANT, TRUE);		

	return TRUE;
}


LuaGlue LG_TEST_AttachTestCase(lua_State *L)
{
	int argNum = 1;
	const char * pszTestName = GET_LUA()->GetStringArgument(argNum++);
	TCHAR szName[512];
	StrCopy(szName, pszTestName);
	SUN_STRING strTestName = szName;

	BOOL bRet = g_TestUIScene.AttachTestCase(strTestName );
	GET_LUA()->PushNumber(bRet);

	return TRUE;    
}


LuaGlue LG_TEST_DetachTestCase(lua_State *L)
{
	int argNum = 1;
	const char * pszTestName = GET_LUA()->GetStringArgument(argNum++);
	TCHAR szName[512];
	StrCopy(szName, pszTestName);
	SUN_STRING strTestName = szName;

	BOOL bRet = FALSE;
	if ( GameFramework::GetCurScene() == SCENE_TYPE_TESTUI )
	{
		bRet = g_TestUIScene.DeAttachTestCase(strTestName );
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
LuaGlue LG_TEST_OpenHelp(lua_State *L)
{
	int argNum = 1;

	DWORD dwHelpIndex	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowHelpTest(dwHelpIndex);
	}

	return TRUE;
}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

LuaGlue LG_TEST_OpenGMHelp(lua_State *L)
{
	int argNum = 1;

	DWORD dwHelpIndex	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);
	int iParam = (int) GET_LUA()->GetNumberArgument(argNum++);

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowGMMsg(dwHelpIndex, iParam);
	}

	return TRUE;
}

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
LuaGlue LG_TEST_OpenHelpString(lua_State *L)
{
	int argNum = 1;

	DWORD dwStringCode	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowHelpTestStringCode(dwStringCode);
	}

	return TRUE;

}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

LuaGlue	LG_TEST_AddMissionHelp(lua_State *L)
{
	int argNum = 1;

	int iTextID	= (int) GET_LUA()->GetNumberArgument(argNum++);
	//DWORD dwNCode	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);
	//DWORD dwDCode	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);
	//DWORD dwImgCode	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);
	MESSAGE_INFO	*pMessageInfo = MessageInfoParser::Instance()->GetMessage( iTextID );

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan && pMessageInfo)
	{
		helpMan->AddHelpMission(iTextID, pMessageInfo->dwNCode, pMessageInfo->dwDCode, pMessageInfo->dwImgCode);
	}

	return TRUE;
}

LuaGlue	LG_TEST_DelMissionHelp(lua_State *L)
{
	int argNum = 1;

	int iTextID	= (int) GET_LUA()->GetNumberArgument(argNum++);
	
	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->DelHelpMission(iTextID);
	}

	return TRUE;
}


LuaGlue LG_TEST_TooltipItem(lua_State *L)
{
	int argNum = 1;

	
	DWORD dwCode	= (DWORD) GET_LUA()->GetNumberArgument(argNum++);

	SCItemSlot ItemSlot;
	ItemSlot.SetCode(dwCode);

	uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
	if(pUIMan)
		pUIMan->InitTooltip();


	const BASE_ITEMINFO *pItemInfo = ItemSlot.GetItemInfo();
	if(pItemInfo->m_Code)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->SetTooltipTestMode(TRUE);
			pUIMan->RegisterTooltipItem(&ItemSlot, FALSE);
			pUIMan->SetTooltipDialogID(0);
		}
	}

	return TRUE;

}

LuaGlue LG_TEST_TooltipClear(lua_State *L)
{
	uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
	if(pUIMan)
	{
		pUIMan->SetTooltipTestMode(FALSE);
		pUIMan->InitTooltip();
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_EventWindow(lua_State *L)
{

	uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));

	if(eventMan)
	{
		eventMan->ShowEventWindow(TRUE);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_WayPointWindow(lua_State *L)
{
	uiWayPointMan* waypointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);

	if(waypointMan)
	{
		waypointMan->ShowWayPointWindow(true);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_SaveWayPoint(lua_State *L)
{
	int argNum = 1;

	MAPCODE code = (WORD) GET_LUA()->GetNumberArgument(argNum++);

	uiWayPointMan* waypointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
	if(waypointMan)
	{
		waypointMan->SetWayPointbyMapCode(code);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_TestWayPoint(lua_State *L)
{
	uiWayPointMan* waypointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
	if(waypointMan)
	{
		waypointMan->SEND_CG_WAYPOINT_INFO_SAVE_SYN(1);
	}

	return TRUE;
}




//-------------------------------------------------------------------------------------------
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
LuaGlue LG_TEST_MissionRankList(lua_State *L)
{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager2_ptr )
    {
        int arg_number = 1;

        int type = static_cast<int>(GET_LUA()->GetNumberArgument(arg_number++));
        rank_manager2_ptr->ShowRankList(type);
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if( pRankMan )
	{
		int argNum = 1;

		int type = (int) GET_LUA()->GetNumberArgument(argNum++);
		pRankMan->ShowRankList(type);
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
	return TRUE;
}

//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_MissionRankGamble(lua_State *L)
{
	int argNum = 1;

	int arg = (int) GET_LUA()->GetNumberArgument(argNum++);

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager2_ptr )
    {
        rank_manager2_ptr->SetRewardRank(arg);
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if(pRankMan)
	{
		pRankMan->SetRewardRank(arg);
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
	return TRUE;
}
//-------------------------------------------------------------------------------------------
LuaGlue LG_TEST_RqstMissionRank(lua_State *L)
{
	int argNum = 1;

	BYTE type = (BYTE) GET_LUA()->GetNumberArgument(argNum++);
	BYTE mission = (BYTE) GET_LUA()->GetNumberArgument(argNum++);

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
    BYTE system_ranking_type = (BYTE) GET_LUA()->GetNumberArgument(argNum++);

    if ( rank_manager2_ptr )
    {
        rank_manager2_ptr->Send_CG_MISSION_RANKING_INFO_SYN(type, mission, static_cast<eRANKING_SYSTEM_TYPE>(system_ranking_type), 0);
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

    if( pRankMan )
    {
        pRankMan->Send_CG_MISSION_RANKING_INFO_SYN(type, mission, 0);
    }
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
	
	return TRUE;
}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM



LuaGlue LG_TEST_SendMoveNak(lua_State *L)
{
    if ( g_pApplication && g_pHero)
    {
        MSG_CG_SYNC_MOVE_NAK SendPacketAck;

        SendPacketAck.m_wvCurPos = g_pHero->GetPosition() - (g_pHero->GetDirection() * 0.4f);

        g_pApplication->AddVirtualPacket( 
            CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------
/** 
	desc : FormatText TestCase에서 스트링을 추가한다.

	pram info:
		[IN] INTERFACE STRING ID 
	return :		
		TRUE, FALSE
*/
LuaGlue LG_TEST_AppendFormatText(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	DWORD dwStrID = GET_LUA()->GetNumberArgument(argNum++);

	if ( GameFramework::GetCurScene() == SCENE_TYPE_TESTUI )
	{
		g_TestUIScene.EntMessage(TESTCASE_FT_APPEND_STRING, dwStrID, 0);
	}
	else
	{		
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}


//-------------------------------------------------------------------------------------------
/** 
	desc : FormatText TestCase에서 스트링을 초기화 한다.

	pram info:
	return :
		TRUE, FALSE
*/
LuaGlue LG_TEST_ClearFormatText(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	if ( GameFramework::GetCurScene() == SCENE_TYPE_TESTUI )
	{
		g_TestUIScene.EntMessage(TESTCASE_FT_CLEAR_STRING, 0, 0);
	}
	else
	{		
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
	desc : FormatText TestCase에서 스트링을 초기화 한다.

	pram info:
	return :
		TRUE, FALSE
*/
LuaGlue LG_TEST_SetFormatTextRect(lua_State *L)
{
	BOOL bRet = FALSE;
	int argNum = 1;
	int left	= (int) GET_LUA()->GetNumberArgument(argNum++);
	int top		= (int) GET_LUA()->GetNumberArgument(argNum++);
	int right	= (int) GET_LUA()->GetNumberArgument(argNum++);	
	int bottom	= (int) GET_LUA()->GetNumberArgument(argNum++);

	if ( GameFramework::GetCurScene() == SCENE_TYPE_TESTUI )
	{
		g_TestUIScene.EntMessage(TESTCASE_FT_SET_RECT, left, top, right, bottom);
	}
	else
	{		
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;	
}

//-------------------------------------------------------------------------------------------
/** 
	desc : FormatText TestCase에서 스트링을 초기화 한다.

	pram info:
		NPCCODE
	return :
		TRUE, FALSE
*/
LuaGlue LG_NPC_MEET(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

//	MAPID MapCode = 0;
//	FIELDID FieldCode = 0;
	CODETYPE NPCCode = 0;

//	MapCode		= GET_LUA()->GetNumberArgument(argNum++);
//	FieldCode	= GET_LUA()->GetNumberArgument(argNum++);
	NPCCode		= GET_LUA()->GetNumberArgument(argNum++);

	bRet = (BOOL)GameFunc::NPCMeetWithForward(1, 1, NPCCode);

	GET_LUA()->PushNumber(bRet);

	return TRUE;	
}

//-------------------------------------------------------------------------------------------
/** 
	desc : TestCase에서 Hero 생성 스트링을 초기화 한다.

	pram info:
	CLASS X Y
	return :
	
*/
LuaGlue LG_CREATE_HERO(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	int iClass  = (int)GET_LUA()->GetNumberArgument(argNum++);
	


	BattleScene::SetHeroInfo( iClass );

	if ( g_pHero )
	{
		g_ObjectManager.Delete( g_pHero->GetObjectKey() );
		g_pHero = NULL;
	}

	g_pHero = (Hero *)g_ObjectManager.Add(
		g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT), 
		PLAYER_OBJECT,
        xGetHeroData()->GetClassCode(),
		TRUE);

	g_InterfaceManager.InitSkillInfos();

	g_pHero->SetHeroInfo(BattleScene::GetHeroInfo());
	
	WzVector vTestPos = g_pSunTerrain->GetRandomPosInTile(1);
	g_pHero->SetPosition(vTestPos,1);


	// 임시로 버서커에게 칼을 들려준다.
	if (g_pHero->GetClass() == eCHAR_BERSERKER) 
	{

		SCItemSlot ItemSlot;
		ItemSlot.SetCode(1);

		SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	
		g_pHero->SetCurrentAttackStyle(eSTYLE_TWOHANDAXE_NORMAL);
	}
	// 드래곤도 칼주자
	if (g_pHero->GetClass() == eCHAR_DRAGON) 
	{

		SCItemSlot ItemSlot;
		ItemSlot.SetCode(31);

		SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);			
	}
	// 발키리 석궁주기
	if (g_pHero->GetClass() == eCHAR_VALKYRIE) 
	{

		SCItemSlot ItemSlot;
		ItemSlot.SetCode(91); // 석궁
		//ItemSlot.SetCode(106); // 에테르

		SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

		g_pHero->SetCurrentAttackStyle(eSTYLE_ONEHANDCROSSBOW_NORMAL);
		//g_pHero->SetCurrentAttackStyle(eSTYLE_ETHER_NORMAL);
	}

	if (g_pHero->GetClass() == eCHAR_MAGICIAN) 
	{

		SCItemSlot ItemSlot;
		ItemSlot.SetCode(154);
		g_pHero->SetCurrentNomalAttackStyle(eWEAPONTYPE_ORB);

		SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);			
	}

	g_pHero->SetHP(100);
	g_pHero->SetMP(100);

	g_pHero->SetPartVariation(PLAYER_VARIATION_HAIR,1);
	g_pHero->SetPartVariation(PLAYER_VARIATION_FACE,1);
    g_pHero->RefreshAppearRender();

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 맵로드 

pram info:
	MapID, FieldID
return :

*/
LuaGlue LG_Load_Map(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	MAPID	MapCode		= 0;
	FIELDID FieldCode	= 0;

	MapCode		= GET_LUA()->GetNumberArgument(argNum++);
	FieldCode	= GET_LUA()->GetNumberArgument(argNum++);

	if ( !g_pMap )
	{
		g_pMap = new Map;
	}

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
	g_ObjectManager.Create();
#endif

	char szRootDir[0xff];
	::GetCurrentDirectoryA(0xff, szRootDir); 

    g_pMap->Destroy();
	g_pMap->Create( g_pSunRenderer, szRootDir );	
	bRet = g_pMap->Load(MapCode, FieldCode);		
	if ( bRet )
	{
		g_pMap->EnableBlur(FALSE);
		g_pMap->SetRenderFlag(TRUE);

		// 맵에서 얻어온 (맵툴에서 정한) 카메라 위치와 바라보는 방향과 목표를 설정한다.
		WzVector wzCamera, wzTarget;
		WzVector wzDir,wzBase;

		CAMERA_EXTRA_INFO * camExtra = g_pMap->GetExtraCameraInfo();
		SetVector(&wzTarget, camExtra->WzTargetPos.x, camExtra->WzTargetPos.y, camExtra->WzTargetPos.z);
		SetVector(&wzCamera, camExtra->WzPos.x, camExtra->WzPos.y, camExtra->WzPos.z);

		wzDir = wzTarget - wzCamera;
		wzBase = wzDir;
		wzBase.z = 0.0f;
		float fBase = VectorLength(&wzBase);
		g_Camera.SetCameraDistance(fBase);
		float fYaw = Math_GetAngleFromVector(&wzDir);
		g_Camera.SetYaw( fYaw );
		g_Camera.SetPitch(atan( wzDir.z / fBase));
		g_Camera.SetCameraFromTo(wzCamera, wzTarget);
		g_Camera.SetCameraPosition(&wzCamera);

		g_Camera.UpdateCamera();
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 맵로드 

pram info:
MapID, FieldID
return :

*/
LuaGlue LG_Load_Map_All(lua_State *L)
{
#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
	g_ObjectManager.Create();
#endif

    sMAPINFO_HASH::const_iterator itr = 
        MapInfoParser::Instance()->GetMapInfoHash().begin();

    SUN_STRING strNotice;

    if ( g_pMap )
    {
        g_pMap->Destroy();            
    }
#ifdef _USE_MSXML
    MSXML2::IXMLDOMDocument2Ptr             pDoc;   
    MSXML2::IXMLDOMProcessingInstructionPtr pPI;    // XML ProcessingInstruction 선언
    MSXML2::IXMLDOMElementPtr               pRootElement;  
    MSXML2::IXMLDOMTextPtr                  pXMLText;

    pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );
    pPI = pDoc->createProcessingInstruction( 
        "xml", 
        "version=\"1.0\" endcoding=\"euc-kr\"" );

    pRootElement  = pDoc->createElement("MAP_LOAD");

    pDoc->appendChild(pPI);
                
    char szRootDir[0xff];
    ::GetCurrentDirectoryA(0xff, szRootDir);     
    for( ; itr != MapInfoParser::Instance()->GetMapInfoHash().end() ; ++itr )
    {
        const sMAPINFO * pMapInfo = (*itr);      

        MSXML2::IXMLDOMElementPtr pMapElement;
        pMapElement = pDoc->createElement("MAP");               

        for ( int i = 0; i < 3; ++i )
        {
            if ( !pMapInfo->FCode[i] )
                continue;

            MSXML2::IXMLDOMElementPtr pFieldElement;
            pFieldElement = pDoc->createElement("FIELD");
                  
            g_pMap->Create( g_pSunRenderer, szRootDir );        

            strNotice.clear();

            BOOL bRet = g_pMap->Load( pMapInfo->MapCode, pMapInfo->FCode[i] );    
            assert(bRet);

            g_pMap->Destroy();  

            pMapElement->appendChild(pFieldElement);
        } 

        pRootElement->appendChild(pMapElement);
    }

    pDoc->appendChild(pRootElement);
    pDoc->save("..\\MAP_LOAD_TEST_LOG_MSXML.xml");

#else
    //CMarkupArchive xmlLog(
    //    CMarkupArchive::ArchiveModeNotZipped | 
    //    CMarkupArchive::ArchiveModeCreate | 
    //    CMarkupArchive::ArchiveModeStoring );
    //xmlLog.SetEncodingString("euc-kr");
    //xmlLog.Open("MapLoadTestLog.xml");


    //xmlLog.AddElem( "MAP_LOAD", "Map Load Test Log");
    //xmlLog.IntoElem();

    char szRootDir[0xff];
    ::GetCurrentDirectoryA(0xff, szRootDir);     
    for( ; itr != MapInfoParser::Instance()->GetMapInfoHash().end() ; ++itr )
    {
        const sMAPINFO * pMapInfo = static_cast<const sMAPINFO *> ( itr->second );      

        //xmlLog.AddElem( "MAP");
        //xmlLog.AddAttrib("ID", pMapInfo->MapCode );
        //xmlLog.IntoElem();

        for ( int i = 0; i < 3; ++i )
        {
            if ( !pMapInfo->FCode[i] )
                continue;

            //xmlLog.AddElem( "FIELD");
            //xmlLog.AddAttrib("ID", pMapInfo->FCode[i] );
            //xmlLog.AddAttrib("TRIGGER_ID", pMapInfo->FGCode[i] );

            g_pMap->Create( g_pSunRenderer, szRootDir );        

            strNotice.clear();

            BOOL bRet = g_pMap->Load( pMapInfo->MapCode, pMapInfo->FCode[i] );    
            assert(bRet);

            g_pMap->Destroy();

/*            if (bRet)
            {
                xmlLog.AddAttrib("CHECK", "OK" );
            }
            else
            {
                xmlLog.AddAttrib("CHECK", "FAILED" );
            }  */          
        } 

//        xmlLog.OutOfElem();
    }

    //xmlLog.Close();
#endif

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 레벨 설정 ( 클라이언트에서만 의미 있음 ) 

pram info:
	LEVEL
return :

*/
LuaGlue LG_SetLevel(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	LEVELTYPE	Level	= 0;

	Level = GET_LUA()->GetNumberArgument(argNum++);
	if (g_pHero)
	{
		bRet = TRUE;
		g_pHero->SetLevel(Level);
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("%d Level 로 설정 하였습니다."), g_pHero->GetLevel());
	}

	GET_LUA()->PushNumber(bRet);
	
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 아이템을 인벤토리에 추가한다.( 클라이언트 테스트에서만 의미 있음. )

pram info:
	ITEMCODE NUM
return :

*/
LuaGlue LG_AddItemAtInventory(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	CODETYPE ItemCode = 0;
	WORD	 ItemNum = 0;
	
	ItemCode = GET_LUA()->GetNumberArgument(argNum++);	
	ItemNum	 = GET_LUA()->GetNumberArgument(argNum++, 1);	

	ItemManager::Instance()->Test_InsertItemAtInventory( ItemCode, ItemNum );

	GET_LUA()->PushNumber(bRet);
				
	return TRUE;	
}

//-------------------------------------------------------------------------------------------
/** 
desc : 돈 ( 클라이언트 테스트에서만 의미 있음. )

pram info:
return :

*/
LuaGlue LG_SetMoney(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	DWORD	 money = 0;
	
	money = GET_LUA()->GetNumberArgument(argNum++);	

	if ( g_pHero )
	{
		g_pHero->SetMoney(money);
	}

				
	return TRUE;	
}

//-------------------------------------------------------------------------------------------
/** 
desc : 스크립트 실행 

pram info:
LEVEL
return :

*/
LuaGlue LG_RunScript(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	LEVELTYPE	Level	= 0;

	SUN_STRING strFile;
	strFile = (TCHAR *)GET_LUA()->GetStringArgument(argNum++);

	bRet = GET_LUA()->RunScript(strFile.c_str());

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}



//-------------------------------------------------------------------------------------------
/** 
desc : 스크립트 실행 

pram info:
condition type
condition value
return :


*/

//-------------------------------------------------------------------------------------------
/** 
desc : 퀘스트 리스트 창 오픈

pram info:
return :
*/
LuaGlue LG_ToggleACDialog(lua_State *L)
{
    uiBase* pDlg = GET_DIALOG(IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);
	if (pDlg) {	
		if (pDlg->IsVisibleWindow())
		{
			pDlg->ShowDialog(FALSE);				
		}
		else
		{
			pDlg->ShowDialog(TRUE);				

		}
	}

	return TRUE;
}



// 보상창 연다.
LuaGlue LG_OpenRewardDialog(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	int      iReward = 0;


	iReward = GET_LUA()->GetNumberArgument(argNum++);	

	if (g_pHero) {
		g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_REWARD, TRUE);

		RewardDialog *pDlg = (RewardDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_REWARD );
		if (pDlg) {
			pDlg->SetMissionReward(iReward);
		}

		
	}

	return TRUE;	
}



//-------------------------------------------------------------------------------------------
/** 
desc : 퀘스트 리스트 창 오픈

pram info:
return :
*/
LuaGlue LG_TEST_OpenQuestList(lua_State *L)
{
	uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( pQuestMan )
	{
		pQuestMan->OpenQuestList();
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 퀘스트 정보 저장

pram info:
return :
*/
LuaGlue LG_TEST_SaveQuestList(lua_State *L)
{
	BOOL bRet = FALSE;

	if ( g_pQMGR_Real )
	{
		g_pQuestStreamFile = fopen("TestQuestStream.dat", "w");
		if ( g_pQuestStreamFile )
		{

			g_pQMGR_Real->SerializeStream( g_ClientQuestProgressTotalInfo, QSERIALIZE_LOAD );
			g_pQMGR_Real->SerializeStream( g_ClientQuestCompleteTotalInfo, QSERIALIZE_LOAD );

			fwrite(
				&g_ClientQuestProgressTotalInfo, 
				sizeof(g_ClientQuestProgressTotalInfo), 
				1, 
				g_pQuestStreamFile);

			fwrite(
				&g_ClientQuestCompleteTotalInfo, 
				sizeof(g_ClientQuestCompleteTotalInfo), 
				1, 
				g_pQuestStreamFile);

			fclose( g_pQuestStreamFile );
			g_pQuestStreamFile = NULL;
			
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, 
				_T("Save Quest Info!"));

			bRet = TRUE;
		}
	}
	else
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, 
			_T("Can't Save Quest Info!"));
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 퀘스트 정보 로드

pram info:
return :
*/
LuaGlue LG_TEST_LoadQuestList(lua_State *L)
{
	BOOL bRet = FALSE;

	if ( g_pQMGR_Real )
	{
		g_pQuestStreamFile = fopen("TestQuestStream.dat", "r");
		if ( g_pQuestStreamFile )
		{
			fread(
				&g_ClientQuestProgressTotalInfo, 
				sizeof(g_ClientQuestProgressTotalInfo), 
				1, 
				g_pQuestStreamFile);

			fread(
				&g_ClientQuestCompleteTotalInfo, 
				sizeof(g_ClientQuestCompleteTotalInfo), 
				1, 
				g_pQuestStreamFile);


			g_pQMGR_Real->SerializeStream( g_ClientQuestProgressTotalInfo, QSERIALIZE_SAVE );
			g_pQMGR_Real->SerializeStream( g_ClientQuestCompleteTotalInfo, QSERIALIZE_SAVE );

			fclose( g_pQuestStreamFile );
			g_pQuestStreamFile = NULL;

			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, 
				_T("Quest Info Load!!"));

			bRet = TRUE;
		}
	}
	else
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, 
			_T("Can't Load Quest Info!!"));
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 특정 몬스터 죽였다고 서버에서 알려오는 Quest 정보를 임의적으로 생성

pram info:
	QUEST CODE 
	MONSTER CODE
return :
	TRUE / FALSE
*/
LuaGlue LG_TEST_MONSTER_KILL_CMD(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	CODETYPE QuestCode = 0, MonsterCode = 0;
	
	QuestCode	= GET_LUA()->GetNumberArgument(argNum++);	
	MonsterCode = GET_LUA()->GetNumberArgument(argNum++);	

	MSG_CG_QUEST_MONSTER_KILL_CMD SendPacketAck;

	SendPacketAck.m_QuestCode = QuestCode;
	SendPacketAck.m_MonsterCode = MonsterCode;

	g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
	bRet = TRUE;

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : NPC 위치로~

pram info:
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_SettingNPC(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	if (g_pMap)
	{
		g_TestUIScene.SettingNPC(g_pMap->GetMapID(),g_pMap->GetCurrentFieldID());
		bRet = TRUE;
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : NPC 위치로~
pram info:
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_RefreshNPCState(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	RefreshNPCStateForQuest();

	GET_LUA()->PushNumber(TRUE);

	return TRUE;			
}

//-------------------------------------------------------------------------------------------
/** 
desc : 퀘스트 관련 에어리어 정보를 받아온다.
pram info:
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_InitQuestAreaInfo(lua_State *L)
{
	BOOL bRet = FALSE;
	if ( g_pMap )
	{
		g_TestUIScene.InitQuestAreaInfo();
		bRet = TRUE;
	}

	GET_LUA()->PushNumber(bRet);	

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 싱글상에서 특정 퀘스트 수락 패킷을 가상으로 날림.
pram info:
return :
TRUE / FALSE
*/

LuaGlue LG_TEST_Quest_Accept(lua_State * L)
{
	if ( GENERALPARAM->IsNet() )
	{
		return FALSE;
	}

	int argNum = 1;
	QCODE QuestCode = GET_LUA()->GetNumberArgument(argNum++);

	QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo(QuestCode);
	if ( pQuestInfo && !pQuestInfo->IsChildQuest())
	{
		uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_QUEST );
		pQuestMan->NET_SEND_CG_QUEST_ACCEPT_SYN( QuestCode );
	}

	return TRUE;
}



//-------------------------------------------------------------------------------------------
/** 
desc : 게임 기본 인터페이스 창 열고 닫기
pram info:
BOOL (SHOW/HIDE)
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_ShowGameInterface(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = GET_LUA()->GetNumberArgument(argNum++);
	g_InterfaceManager.ShowGameInterface(bRet);

	return TRUE;	
}


//-------------------------------------------------------------------------------------------
/** 
desc : 게임 종료
pram info:
return :
*/
LuaGlue LG_TEST_Exit(lua_State *L)
{
	::SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 
desc : 미션포인트 설정
pram info:
	MAPCODE, POINT
return :
	TRUE / FALSE
*/
LuaGlue LG_TEST_SetMissionPosint(lua_State *L)
{
	int argNum = 1;
	BOOL bRet = FALSE;
	CODETYPE MapCode  = GET_LUA()->GetNumberArgument(argNum++);
	WORD MissionPoint = GET_LUA()->GetNumberArgument(argNum++);
	if (g_pMap && g_pMission_Concrete) 
	{
		g_pMission_Concrete->SetMissionPoint(
			MapCode,
			MissionPoint);
		bRet = TRUE;
	}

	GET_LUA()->PushNumber(bRet);

	return TRUE;		
}


LuaGlue LG_TEST_OpenKillPoint(lua_State *L)
{
	int argNum = 1;

	uiBase * pDialog = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );
	//uiBattleZoneDoing *pDialog = static_cast<uiBattleZoneDoing *>(g_InterfaceManager.GetUserInterface(UI_BATTLEZONE_CHUNTING_DOING));

	if( pDialog )
	{
		pDialog->ShowWindow(TRUE);
	}
	return TRUE;
}


LuaGlue LG_TEST_SetKillPoint(lua_State *L)
{
	int argNum = 1;
	int iBlue	= (int) GET_LUA()->GetNumberArgument(argNum++);
	int iRed	= (int) GET_LUA()->GetNumberArgument(argNum++);

	uiBattleZoneDoing * pDialog = GET_CAST_DIALOG( uiBattleZoneDoing, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );
	//uiBattleZoneDoing *pDialog = static_cast<uiBattleZoneDoing *>(g_InterfaceManager.GetUserInterface(UI_BATTLEZONE_CHUNTING_DOING));

	if( pDialog )
	{
		pDialog->SetKillPoint(iBlue, iRed);
	}
	return TRUE;
}



LuaGlue LG_TEST_CMD_AUTO_QUEST(lua_State *L)
{
	BOOL bRet = FALSE;
	int argNum = 1;
	CODETYPE QuestCode = (int) GET_LUA()->GetNumberArgument(argNum++);
    
	if ( !GENERALPARAM->IsNet() )
	{
        bRet = TRUE;

		MSG_CG_QUEST_ACCEPT_CMD SendPacketAck;
		SendPacketAck.m_QuestCode = QuestCode;
		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
	}
	
	GET_LUA()->PushNumber( bRet );

    return TRUE;
}



//-------------------------------------------------------------------------------------------
/**
desc : 미션포인트 설정
pram info:
	BUY POSITION(Inventory)
return :
	TRUE / FALSE
*/
LuaGlue LG_TEST_CMD_VENDOR_BUY(lua_State *L)
{
	MSG_CG_VENDOR_BUY_CMD SendPacketCmd;

	BOOL bRet = FALSE;
	int argNum = 1;
	SendPacketCmd.m_VendorPos = (int) GET_LUA()->GetNumberArgument(argNum++);

	if ( !GENERALPARAM->IsNet() )
	{
		if ( g_pApplication )
		{
			g_pApplication->AddVirtualPacket(
				CI_GAMESERVERIDX, (BYTE*)&SendPacketCmd, sizeof(SendPacketCmd));

			bRet = TRUE;
		}
	}

	GET_LUA()->PushNumber( bRet );

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
desc : 어텍 점프 테스트 함수. ( 싱글 테스트 용)
pram info:
BUY POSITION(Inventory)
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_AttackJump(lua_State *L)
{
    BOOL bRet = FALSE;
    int argNum = 1;
    
    //CODETYPE QuestCode = (int) GET_LUA()->GetNumberArgument(argNum++);

    if ( FALSE == GENERALPARAM->IsNet() )
    {
        DWORD dwTargetObjKey = g_HeroInput.GetCurrentTarget();
        Object * pTarget = g_ObjectManager.GetObject( dwTargetObjKey );
        if ( g_pHero && pTarget )
        {
            bRet = TRUE;

            MSG_CG_SYNC_NPC_JUMP_BRD        SendPacket;

            SendPacket.m_bJumpAttack = TRUE;
            SendPacket.m_dwObjectKey = dwTargetObjKey;
            SendPacket.m_wvEndPos = g_pHero->GetVisiblePos();

            g_pApplication->AddVirtualPacket( CI_GAMESERVERIDX, (BYTE*)&SendPacket, sizeof(SendPacket) );
        }        
    }

    GET_LUA()->PushNumber( bRet );

    return TRUE;
}

LuaGlue LG_TEST_EventJump(lua_State *L)
{
    BOOL bRet = FALSE;
    int argNum = 1;

    //CODETYPE QuestCode = (int) GET_LUA()->GetNumberArgument(argNum++);

    if ( FALSE == GENERALPARAM->IsNet() )
    {
        DWORD dwTargetObjKey = g_HeroInput.GetCurrentTarget();
        Object * pTarget = g_ObjectManager.GetObject( dwTargetObjKey );
        if ( g_pHero && pTarget )
        {
            bRet = TRUE;

            MSG_CG_SYNC_NPC_JUMP_BRD        SendPacket;

            SendPacket.m_bJumpAttack = FALSE;
            SendPacket.m_dwObjectKey = dwTargetObjKey;
            SendPacket.m_wvEndPos = g_pHero->GetVisiblePos();

            g_pApplication->AddVirtualPacket( CI_GAMESERVERIDX, (BYTE*)&SendPacket, sizeof(SendPacket) );
        }        
    }

    GET_LUA()->PushNumber( bRet );

    return TRUE;
}



LuaGlue LG_TEST_jjumbo( lua_State *L )
{
/*
	ChatDialog * pChatDialog = (ChatDialog *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
	if (pChatDialog)
	{
		pChatDialog->AutoFillWhisperNameWhenChatListClick();
	}


#ifdef _JBH_ADD_PC_BANG_EVENT_TIME
	int argNum = 1;
	int iTime  = GET_LUA()->GetNumberArgument(argNum++);
	PcbangPlayTimeDialog * pDlg = ( PcbangPlayTimeDialog *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_PCBANG_EVENT_TIME );
	if ( pDlg)
	{
		pDlg->SetTime(iTime);
	}
#endif


#ifdef _JBH_ADD_SPETIAL_OPTION
	int argNum = 1;
	int iTime  = GET_LUA()->GetNumberArgument(argNum++);
	g_InterfaceManager.HideAllWindows();
#endif

*/


	



	//RECT rt;
	//rt.left = 100;
	//rt.right = 300;
	//rt.top = 100;
	//rt.bottom = 300;
	//TCHAR szMessage[INTERFACE_STRING_LENGTH];
	//Sprintf( szMessage , "가나 다라마바 사아자차카타파하가나다라마바사아자카타파하하하하하하하하하하");
	//g_pSunRenderer->x_pManagerTextOut->DrawTextA(szMessage , &rt , WzColor_RGBA(255,0,0,255) );

	//GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage);
/*
#ifdef _JBH_ADD_GUILD_POINT_EXCHANGE
	uiGuildConvertGP * pDlg = (uiGuildConvertGP * ) g_InterfaceManager.GetUserInterface(UI_GUILD_CONVERT_GP);
	if( pDlg)
	{
		pDlg->ShowInterface(TRUE);
	}
#endif


#ifdef _JBH_ADD_PERSONAL_VENDOR_LIST
	uiVendorList * pVendorList = (uiVendorList * ) g_InterfaceManager.GetUserInterface(UI_VENDOR_LIST );
	if( pVendorList )
	{
		pVendorList->ShowInterface(TRUE);
	}
#endif
	
	int argNum = 1;
	int iCode  = GET_LUA()->GetNumberArgument(argNum++);
	BankDialog * pBank = ( BankDialog * ) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BANK );
	if( pBank )
	{
		pBank->SendWareHouseStart();
	}
	return TRUE;
	*/
	
	GlobalFunc::CreateEffectBeUpToOption(StrToWzID("G101"), g_pHero->GetBonePos(5));
	return TRUE;
}



LuaGlue LG_TEST_CreateEffect(lua_State * L)
{
	int argNum = 1;

	const char * pszEffectName  = GET_LUA()->GetStringArgument(argNum++);
    int iBoneIndex =1;
    iBoneIndex  = GET_LUA()->GetNumberArgument(argNum++);

	if(!pszEffectName)
		return FALSE;

	char szEffectName[100] = {0,};

	strcpy(szEffectName, pszEffectName);

    GlobalFunc::CreateEffectBeUpToOption(StrToWzID(szEffectName), g_pHero->GetBonePos(5));
	
	return TRUE;
}

LuaGlue LG_TEST_SetGrayScale(lua_State * L)
{
	int argNum = 1;

	int iSet  = GET_LUA()->GetNumberArgument(argNum++);

	if(iSet > 0)
	{
		g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GRAYSCALE );
	}
	else
	{
		g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect( PET_GRAYSCALE );
	}

	
	return TRUE;
}

LuaGlue LG_TEST_SetGrayScaleColor(lua_State * L)
{
	int argNum = 1;

	int iRed  = GET_LUA()->GetNumberArgument(argNum++);
	int iGreen  = GET_LUA()->GetNumberArgument(argNum++);
	int iBlue  = GET_LUA()->GetNumberArgument(argNum++);

	WzColor wzColor;
	wzColor = WzColor_RGB( iRed, iGreen, iBlue);

	g_pSunRenderer->x_pManagerPostEffect->SetGrayScaleColor(wzColor);
	g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GRAYSCALE );

	return TRUE;

}


LuaGlue LG_TEST_SetSepiaMode(lua_State * L)
{
	int argNum = 1;

	int iMode  = GET_LUA()->GetNumberArgument(argNum++);

	WzColor wzColor;

	switch(iMode)
	{
	case 1:
		{
			wzColor = WzColor_RGB(255,198,134);
			g_pSunRenderer->x_pManagerPostEffect->SetGrayScaleColor(wzColor);
			g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GRAYSCALE );
		}
		break;

	case 2:
		{
			wzColor = WzColor_RGB(245,188,124);
			g_pSunRenderer->x_pManagerPostEffect->SetGrayScaleColor(wzColor);
			g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GRAYSCALE );
		}
		break;

	case 3:
		{
			wzColor = WzColor_RGB(235,188,124);
			g_pSunRenderer->x_pManagerPostEffect->SetGrayScaleColor(wzColor);
			g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_GRAYSCALE );
		}
		break;

	}
	

	return TRUE;
}



//-------------------------------------------------------------------------------------------
/**
desc : 퀘스트 자동 수락 / 완료 요청 
pram info:
BUY POSITION(Inventory)
return :
TRUE / FALSE
*/
LuaGlue LG_TEST_UpdateAutoQuestRequest( lua_State *L )
{
    // 우선 순위가 보상이 우위 다.
    BOOL bReward = FALSE;
    bReward = UpdateAutoRewardRequest();

    if (!bReward)
    {
        UpdateAutoAcceptRequest();
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
desc : 몬스터 소환
pram info:
BUY POSITION(Inventory)
return :
*/
LuaGlue LG_TEST_SpawnMonster( lua_State *L )
{
    BOOL bRet = FALSE;
    int argNum = 1;

    CODETYPE MonsterCode = (int) GET_LUA()->GetNumberArgument(argNum++);
    size_t   MonsterNum = (int) GET_LUA()->GetNumberArgument(argNum++);

    GameFunc::SpawnMonster(MonsterCode, MonsterNum);

    return TRUE;
}


//-------------------------------------------------------------------------------------------
/**
    도망 치기
*/
LuaGlue LG_TEST_RunAway( lua_State * L )
{
    DWORD dwTargetObjKey = g_HeroInput.GetCurrentTarget();

	MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD SendRunawayBrd;
	SendRunawayBrd.m_dwObjectKey = dwTargetObjKey;	

	Object * pObj = g_ObjectManager.GetObject(dwTargetObjKey);
	if ( pObj )
	{
		SendRunawayBrd.m_iRotateAngle = (int)(pObj->GetAngle() * 180 / WZ_PI);
		if ( g_pApplication )
		{
			g_pApplication->AddVirtualPacket(
				CI_GAMESERVERIDX, (BYTE *)&SendRunawayBrd, sizeof(MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD));
		}
	}

    return TRUE;
}


LuaGlue LG_TEST_CurrentPos( lua_State * L )
{
	DWORD dwTargetObjKey = g_HeroInput.GetCurrentTarget();
	Character * pChr = (Character *)g_ObjectManager.GetObject( dwTargetObjKey );

	if ( !pChr )
	{
		return TRUE;
	}

	GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, _T("CurVisible Pos : x(%f), y(%f), z(%f)"), 
		pChr->GetVisiblePos().x, pChr->GetVisiblePos().y, pChr->GetVisiblePos().z );

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
desc : 히어로 플레이어 카오상태 변경.
pram info:
BUY POSITION(Inventory)
return :
*/
LuaGlue LG_TEST_SetChaoState( lua_State * L )
{
	if (!g_pHero)
	{
		return TRUE;
	}

	int argNum = 1;
	BYTE ChaoState = (BYTE) GET_LUA()->GetNumberArgument(argNum++);
	g_pHero->SetChaoState( ChaoState );

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
// 쪽지리스트 요청
*/
LuaGlue LG_TEST_RqstMemoList( lua_State * L )
{

	MSG_CW_MEMO_LIST_REQ sync;
	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
// 유료아이템 NPC창
*/

LuaGlue LG_TEST_ShowCashItemShopNpc( lua_State * L )
{

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //! 2011.7.22 / i4u4me / 캐시샵 작업 

#else
	uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
	if(pMan)
		pMan->ShowCashItemShopNpcPopup(TRUE);


#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL



	return TRUE;
}



//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_ShowBillingAlarm( lua_State * L )
{
	int argNum = 1;
	DWORD dwTime = (DWORD)GET_LUA()->GetNumberArgument(argNum++);

	if(GameFramework::GetCurrentScene())
		GameFramework::GetCurrentScene()->ShowBillingAlarm(dwTime);
		
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_ShowDisconnectAlarm( lua_State * L )
{
	int argNum = 1;
	DWORD dwErrCode = (DWORD)GET_LUA()->GetNumberArgument(argNum++);

	if(GameFramework::GetCurrentScene())
		GameFramework::GetCurrentScene()->ShowDisconnectAlarm(dwErrCode);
		
	return TRUE;
}


//-------------------------------------------------------------------------------------------
/** 데미지 메쉬 생성을 해본다.
*/
LuaGlue LG_TEST_PopupMesh( lua_State * L )
{
	DWORD dwColor = 0;
	int argNum = 1;
	BOOL bSkill = (BOOL)GET_LUA()->GetNumberArgument(argNum++);
	INT64 iNumber = (int)GET_LUA()->GetNumberArgument(argNum++);
	eDAMAGEPOPUP_KIND eKind= (eDAMAGEPOPUP_KIND)(int)GET_LUA()->GetNumberArgument(argNum++);		
	const char * pStr = GET_LUA()->GetStringArgument(argNum++, "0xFFFF00FF");
	sscanf( pStr, "%x", &dwColor);
	BOOL bCritical = (BOOL) GET_LUA()->GetNumberArgument(argNum++);
	float fMultiplier = (float) GET_LUA()->GetNumberArgument(argNum++, 1.0f);
	int iSecondary = (int) GET_LUA()->GetNumberArgument(argNum++, -1);
	DWORD secondarycol = WzColor_RGBA(0,0,0,0);
	float fHeight = 0.0f;

	DWORD dwTarget = g_HeroInput.GetCurrentTarget();
	Character * pTarget = (Character *)g_ObjectManager.GetObject( dwTarget );

	if ( !g_pHero )
	{
		return FALSE;
	}

	if ( NULL == pTarget )
	{
		pTarget = g_pHero;
	}

	pTarget->CreatePopupMesh(
		bSkill,
		iNumber, 
		eKind,
		bCritical, 
		FALSE,
		fMultiplier, 
		iSecondary, 
		secondarycol, 
		fHeight
		);

	return TRUE;
}



LuaGlue LG_TEST_ItemDestroyDuraCmd( lua_State * L )
{
	int argNum = 1;
	SLOTIDX AtIndex = (SLOTIDX)GET_LUA()->GetNumberArgument(argNum++);
	POSTYPE pos = (POSTYPE)GET_LUA()->GetNumberArgument(argNum++);

	SCSlotContainer * pFromContainer = (SCSlotContainer *)ItemManager::Instance()->GetContainer(AtIndex);
	
	if(pFromContainer)
	{
		SCSlot & rFromSlot = pFromContainer->GetSlot(pos);
		TCHAR szName1[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

		BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rFromSlot.GetCode());
		if (pItemInfo)
		{
			g_InterfaceManager.GetItemName( rFromSlot.GetCode(), szName1, INTERFACE_STRING_LENGTH );

			g_InterfaceManager.GetInterfaceString( eST_ITEM_DESTROY_DURA, szText, INTERFACE_STRING_LENGTH );

			_stprintf( szMessage, szText,szName1);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			BOOL bResult = pFromContainer->DeleteSlot(pos, &rFromSlot);

			MouseHandler::Instance()->RemoveItemAtHand();

		}
	}

	return TRUE;
}

LuaGlue LG_TEST_DeleteItemCmd( lua_State * L )
{
	int argNum = 1;
	SLOTIDX AtIndex = (SLOTIDX)GET_LUA()->GetNumberArgument(argNum++, SI_INVENTORY);
	POSTYPE pos = (POSTYPE)GET_LUA()->GetNumberArgument(argNum++, 0);
	int iNum = (int)GET_LUA()->GetNumberArgument(argNum++, 1);
	eITEM_DELETE_RESULT eDelType = 
		(eITEM_DELETE_RESULT)(int)GET_LUA()->GetNumberArgument(argNum++, RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM);

	if (!g_pHero )
	{
		return TRUE;
	}
	
	ItemManager::Instance()->DeleteItemCmd(AtIndex, pos, iNum, eDelType);

	return TRUE;
}
	
LuaGlue LG_TEST_SetBrightDelay( lua_State * L )
{
	int argNum = 1;
	DWORD dwDelay = (DWORD)GET_LUA()->GetNumberArgument(argNum++, 1500);

	SceneBase * pScene = GameFramework::GetCurrentScene();
	if ( !pScene )
	{
		return TRUE;
	}

	if ( SCENE_TYPE_VILLAGE == pScene->GetType() || 
		 SCENE_TYPE_FIELD == pScene->GetType() || 
		 SCENE_TYPE_MISSION == pScene->GetType() ||
		 SCENE_TYPE_CHUNTING == pScene->GetType() )
	{
		BattleScene * pBattleScene = (BattleScene *)pScene;
		pBattleScene->SetBrightDelay( dwDelay );
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_PlayEffectSound( lua_State * L )
{
	int argNum = 1;
	int index = (int)GET_LUA()->GetNumberArgument(argNum++);

	g_SoundEffect.Play(index, // sound ID
				&g_pHero->GetVisiblePos());
		
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_PlayEffectSounds( lua_State * L )
{
	int argNum = 1;
	int index = (int)GET_LUA()->GetNumberArgument(argNum++);
	int index2 = (int)GET_LUA()->GetNumberArgument(argNum++);

	for(int i=index; i<=index2; ++i)
	{
		g_SoundEffect.Play(i, // sound ID
					&g_pHero->GetVisiblePos());
	}
		
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_SpawnFullsetPlayer( lua_State * L)
{
	int argNum = 1;
	int iSet   = (int) GET_LUA()->GetNumberArgument(argNum++);

	GameFunc::SpawnFullsetPlayer(eCHAR_BERSERKER , iSet);
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
LuaGlue LG_TEST_StopAllVoice( lua_State * L)
{
	int argNum = 1;
	int iFade = (int) GET_LUA()->GetNumberArgument(argNum++);

	if(iFade <= 0)
	{
		wzsnd_stop_all_voice(FALSE);
	}
	else
	{
		wzsnd_stop_all_voice(TRUE);
	}

	return TRUE;
}

LuaGlue LG_TEST_InsertInvertoryItem(lua_State * L)
{
	int argNum = 1;

	int AtPos = ( int ) GET_LUA()->GetNumberArgument(argNum++);
	int iItemCode = (int ) GET_LUA()->GetNumberArgument(argNum++);

	if( g_pHero )
	{
		SCSlotContainer * pInventory =  g_pHero->GetSlotContainer(SI_INVENTORY);
		if( pInventory )
		{

			SCItemSlot ItemSlot;
			ItemSlot.SetCode(iItemCode);
			pInventory->InsertSlot(AtPos, ItemSlot);
		}
	}
	return TRUE;
}

LuaGlue LG_TEST_SetProjection(lua_State * L)
{
	int argNum = 1;

	float fMaxFar = (float ) GET_LUA()->GetNumberArgument(argNum++);

	g_Camera.SetProjection(1.0f,fMaxFar, WZ_PI / 4.0f);
	return TRUE;
}


LuaGlue LG_TEST_CreateEtherWeaponEffect(lua_State * L)
{
	g_pHero->CreateEtherWeaponEffect();
	return TRUE;
}

LuaGlue LG_TEST_DestoryEtherWeaponEffect(lua_State * L)
{
	g_pHero->DestoryEtherWeaponEffect();
	return TRUE;
}


LuaGlue LG_TEST_OpenSetLottoNum(lua_State * L)
{
	int argNum = 1;
	BOOL bRet = FALSE;

	INT64 iArgs=0;

	iArgs = GET_LUA()->GetNumberArgument(argNum++);

	//나의 로또응모정보를 요청한다
	uiLottoMan* pLoginMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	if(pLoginMan)
	{
		pLoginMan->SetLottoNumber(iArgs);
	}
	return TRUE;
}

LuaGlue LG_TEST_OpenLottoWindow(lua_State * L)
{
	static BOOL bOpen = TRUE;

	uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );

	if(pLottoWindow)
	{
        pLottoWindow->ShowInterface(TRUE);
		
		//아이템갯수
		pLottoWindow->SetItemNum(3);

		//로또기본정보
		LOTTO_BASE_INFO baseinfo;
		BYTE arNum[4] = {1,2,3,4};
		memcpy(baseinfo.arLuckyNum,arNum,sizeof(baseinfo.arLuckyNum));
		baseinfo.wDrawNum=28;
		baseinfo.LottoHeim=200102000;
		baseinfo.WinnerHeim = 102319023981;
		baseinfo.dwWinnerNum = 1432;
		pLottoWindow->SetLottoBaseInfo(baseinfo);

		//응모정보
		for(int i=0;i<30;++i)
		{
			LOTTO_APPLY_DATA ApplyData;
			ApplyData.wDrawNum = i;
			if(i>4 &&i<9)
			{
				ApplyData.byEntryCnt= 2;
			}
			else
			{
				ApplyData.byEntryCnt = 5;
			}

			for(int a=0;a<ApplyData.byEntryCnt;++a)
			{
				ApplyData.arNumInfo[a][0]=25;
				ApplyData.arNumInfo[a][1]=25;
				ApplyData.arNumInfo[a][2]=25;
				ApplyData.arNumInfo[a][3]=25;
			}
		
			pLottoWindow->InsertApplyInfo(ApplyData);
		}

		pLottoWindow->SetApplyInfo();

	}

	

	return TRUE;
}

LuaGlue LG_TEST_OpenLottoNumInput(lua_State * L)
{
	static BOOL bOpen = TRUE;
	//testcode
	uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG( uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG );

	if(pLottoNumInput)
	{
		pLottoNumInput->ShowInterface(bOpen);
	}

	return TRUE;
}

LuaGlue LG_TEST_SSQRewardFail(lua_State * L)
{
	SSQ_REWARD_INFO Info(10, //POINT
						10,//EXP
						10,//MON_KILL
						10);//Player_Num

	uiSSQRewardWindow* pDlg = GET_CAST_DIALOG(uiSSQRewardWindow, IM_SSQ_MANAGER::SSQ_REWARD_WINDOW);

	if (pDlg)
	{
		pDlg->SetType(REWARD_FAIL);
		pDlg->SetRewardInfo(Info);
		pDlg->ShowInterface(TRUE);
	}
	return TRUE;
}

LuaGlue LG_TEST_ForceMoveToHero(lua_State * L)
{
	DWORD dwObjKey = (DWORD)GET_LUA()->GetNumberArgument(1);	

	Object *pObj = g_ObjectManager.GetObject(dwObjKey);

	if(pObj)
	{
		pObj->SetPosition(g_pHero->GetPosition());
	}

	return TRUE;
}

LuaGlue LG_TEST_MissionRelay(lua_State * L)
{
	DWORD dwObjKey = (DWORD)GET_LUA()->GetNumberArgument(1);	

	MissionRelay * pMissionRelay = ( MissionRelay *) g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_MISSION_RELAY );
	if( pMissionRelay)
	{
		pMissionRelay->ShowInterface(TRUE);	
	}
	
	return TRUE;
}

LuaGlue LG_TEST_LottoActive(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs = GET_LUA()->GetNumberArgument(argNum++);
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	BOOL bOpen  = (iArgs==0)?(FALSE):(TRUE);
	
	if(pLottoMan)
	{
		pLottoMan->SetLottoActive(bOpen);
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------
/** 메시지출력
*/
LuaGlue LG_TEST_SendMSG(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs = GET_LUA()->GetNumberArgument(argNum++);

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	
	g_InterfaceManager.GetInterfaceString((int)iArgs,szMessage);

	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	
	g_InterfaceManager.ShowMessageBox(szMessage, 5000, NULL, 0 );
	
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 탈것생성 (사용안함)
*/
LuaGlue LG_TEST_CreateRider(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs = GET_LUA()->GetNumberArgument(argNum++);
	if(g_pHero->IsRiding())
	{
		g_pHero->DestroyRider();
		return TRUE;
	}
	
    g_pHero->CreateRider(iArgs);

	return TRUE;
}
//-------------------------------------------------------------------------------------------
/** 탈것 파츠생성
*/
LuaGlue LG_TEST_RiderPart(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs_idx = GET_LUA()->GetNumberArgument(argNum++);
	INT64 iArgs_ID  = GET_LUA()->GetNumberArgument(argNum++);

	g_pHero->AttachRiderPart(iArgs_idx,iArgs_ID);
	return TRUE;
}
//-------------------------------------------------------------------------------------------
/** 싱글모드 캐릭속도
*/
LuaGlue LG_TEST_SetSpeed(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs = GET_LUA()->GetNumberArgument(argNum++);

	g_pHero->SetCustomMoveSpeed( TRUE, iArgs/100.f );
	return TRUE;
}
//-------------------------------------------------------------------------------------------
/** 플레이어 라이더생성
*/
LuaGlue LG_TEST_CreateRiderPlayer(lua_State * L)
{
	int argNum = 1;
	INT64 iArgs = GET_LUA()->GetNumberArgument(argNum++);

	Object *pObject = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

	if(pObject&& pObject->IsKindOfObject(PLAYER_OBJECT))
	{
		Player *pPlayer = (Player *)pObject;
		if(pPlayer->IsRiding())
		{
			pPlayer->DestroyRider();	
			return TRUE;
		}
		pPlayer->CreateRider(iArgs);

	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------
/** 라이더 인첸트효과
*/
LuaGlue LG_TEST_DeleteEffect(lua_State * L)
{
	int argNum = 1;
   
    return TRUE;
}

LuaGlue LG_TEST_AssertTest(lua_State * L)
{
	int argNum = 1;

	float fTemp = (float ) GET_LUA()->GetNumberArgument(argNum++);

    if (fTemp <= 1000.f)
    {
        ASSERT(fTemp <= 100.f);
    }
    else
    {
        ASSERTMSG(fTemp <= 2000.f, "이러면 안된다");
    }
	//SUN_ASSERT(fTemp <= 100.0f);
	//SUN_ASSERT_MSG(fTemp == 1.0f, "assert! fTemp 값이 유효하지 않닷!");

	//if (fTemp == 0.0f)
	//{
	//	WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, _T("에러입니다."));
	//}

	//if (fTemp == 1000.0f)
	//{
	//	SUN_ASSERT(!"케케케");
	//	SUN_ASSERT(0);
	//}

    return TRUE;
}
LuaGlue LG_TEST_JumpGravity(lua_State * L)
{
    int argNum = 1;

    float fTemp = (float ) GET_LUA()->GetNumberArgument(argNum++);

    g_fGravity = fTemp;
    
    return TRUE;
}
LuaGlue LG_TEST_JumpForce(lua_State * L)
{
    int argNum = 1;

    float fTemp = (float ) GET_LUA()->GetNumberArgument(argNum++);
    
    g_fUpperForce = fTemp;

    return TRUE;
}
LuaGlue LG_TEST_MotionBlur(lua_State * L)
{
    int argNum = 1;

    float fTemp = (float ) GET_LUA()->GetNumberArgument(argNum++);

    static bool trun_on = false;

    trun_on  = !trun_on;

    if (trun_on)
    {
        g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_MOTION_BLUR );
    }
    else
    {
        g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect( PET_MOTION_BLUR );
    }
    

    return TRUE;
}

extern void TestUnit_UsedLogOutput();
LuaGlue LG_TEST_MemoryUsage(lua_State * L)
{
    TestUnit_UsedLogOutput(); // debug  

    return TRUE;
}

LuaGlue LG_TEST_OpenCharCustomize(lua_State * L)
{
    uiCharCustomizeDlg* char_customize_dlg = GET_CAST_DIALOG( uiCharCustomizeDlg,
                                                 IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG );

    if (char_customize_dlg)
    {
        static BOOL is_open = TRUE;
        static WORD type = eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL;
        char_customize_dlg->GenerateCustomInfo(type ,0);
        char_customize_dlg->ShowInterface(is_open);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
LuaGlue LG_TEST_SetSeekNodeCount(lua_State* L)
{
    int agr_num = 1;
    int seek_count = static_cast<int>(GET_LUA()->GetNumberArgument(agr_num++));
    if (g_pHero != NULL)
    {
        g_pHero->SetSeekNodeCount(seek_count);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
LuaGlue LG_TEST_AutobotAction(lua_State* L)
{
    int agr_num = 1;

    const char* parameter1 = GET_LUA()->GetStringArgument(agr_num++);
    const char* parameter2 = GET_LUA()->GetStringArgument(agr_num++);

#ifdef _YMS_AUTOBOT_GAME
    Autobot::Instance()->Lua_Action(parameter1, parameter2);
#endif //_YMS_AUTOBOT_GAME
    return TRUE;
}

//------------------------------------------------------------------------------
LuaGlue LG_TEST_SetShadowDepthOff(lua_State* L)
{
    static bool apply = false;
    g_pSunTerrain->SetDepthApplyOff(apply);
    apply = !apply;

    return TRUE;
}
//------------------------------------------------------------------------------
LuaGlue LG_TEST_OpenSocketComposite(lua_State* L)
{
    uiSocketComposite* ui_socket_composite_ptr = 
        GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);

    if(ui_socket_composite_ptr)
    {
        ui_socket_composite_ptr->ShowInterface(TRUE);	
    }
    return TRUE;    
}

#ifdef _DEV_VER
LuaGlue LG_TEST_OpenGMWin(lua_State* L)
{
    int agr_num = 1;
    const char* parameter = GET_LUA()->GetStringArgument(agr_num++);

    DWORD window_type = -1;

    if (parameter != NULL)
    {
        if (strcmp(parameter, "ITEM") == 0)
        {
            window_type = UI_GM_ITEM_WINDOW;
        }
        else if (strcmp(parameter, "ITEMOPTION") == 0)
        {
            window_type = UI_GM_ITEM_OPTION_WINDOW;
        }
    }
    else
    {   // 기본값 (현재는 아이템)
        window_type = UI_GM_ITEM_WINDOW;
    }

    uiGMWindowMan* ui_gm_window_manager_ptr =
        GET_DIALOG_MANAGER(uiGMWindowMan, UIMAN_GM_WINDOW);

    if (ui_gm_window_manager_ptr)
    {
        ui_gm_window_manager_ptr->ToggleGMWindow(window_type);
        return TRUE;
    }
    return FALSE;
}
#endif//_DEV_VER//_DH_GM_ITEM_WINDOW

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Show_Dialog(lua_State* L)
{
    bool is_res = true;

    is_res = g_pClientScriptParserManager->ReloadScript("Data\\DialogPosInfo.txt", 
                                                        "Data\\DialogPosInfo.txt", 
                                                        MESSAGE_TOTAL_MAX);

    int argNum = 1;
    const char * pszGetDlgID = GET_LUA()->GetStringArgument(argNum++);
    if (pszGetDlgID == NULL)
    {
        return TRUE;
    }

    char dialog_id[4] = {'0', '0', '0', '0'};
    size_t string_len = strlen(pszGetDlgID);
    string_len = min(string_len, 4);
    strncpy(dialog_id + (4 - string_len), pszGetDlgID, string_len);
    WzID Id = StrToWzID(dialog_id);

    SolarDialog * pDlg = g_InterfaceManager.GetDialog( Id );

    if ( pDlg )
    {
        if (pDlg->IsLoaded() == FALSE)
        {
            ((uiBase*)pDlg)->LoadInterface();
        }
        
        g_InterfaceManager.ShowWindow(Id, 0);
        g_InterfaceManager.ShowWindow(Id, 1);
    }

    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Graphic_Ini_Reload(lua_State* L)
{
    CGraphicParam::Instance()->Load();

    return TRUE;
}
//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Tex_Ani_Reload(lua_State* L)
{
    bool is_res = true;
    TextureAnimationManager::Instance()->Release();

    is_res = g_pClientScriptParserManager->ReloadScript(GlobalFunc::AddNationName("data\\TextureListInfo.txt"), 
                                                        GlobalFunc::AddNationName("data\\TextureListInfo.txt"), 
                                                        MESSAGE_TOTAL_MAX);
    is_res = g_pClientScriptParserManager->ReloadScript(GlobalFunc::AddNationName("data\\TextureAnimationInfo.txt"), 
                                                        GlobalFunc::AddNationName("data\\TextureAnimationInfo.txt"), 
                                                        MESSAGE_TOTAL_MAX);

    TextureAnimationManager::Instance()->Clear(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_EP2_Effect);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_EP2_Effect);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_EP2_Effect);

    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Custom_Camera_Reload(lua_State* L)
{
    bool is_res = true;
    is_res = g_pClientScriptParserManager->ReloadScript("data\\CreateCharCustomCamera.txt",  "data\\CreateCharCustomCamera.txt", MESSAGE_TOTAL_MAX);

    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Script_Reload(lua_State* L)
{
    int agr_num = 1;
    eSCRIPT_CODE script_code = static_cast<eSCRIPT_CODE>((int)GET_LUA()->GetNumberArgument(agr_num++));

    SCRIPT_DATA* script_data = g_pClientScriptParserManager->FindScript(script_code);
    if (script_data != NULL)
    {
        bool is_res = true;

        is_res = g_pClientScriptParserManager->ReloadScript(script_data->m_szFileName, script_data->m_szFileName, MESSAGE_TOTAL_MAX);

        int a = 0;
    }
    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_List(lua_State* L)
{
    if( cLuaMain* pInstance = cLuaMain::instance() )
    {
        pInstance->PrintLuaFuncList();
    }

    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_Check_Meshgroup(lua_State* L)
{
    TCHAR msg[MAX_PATH] = {NULL, };
    
    if (g_DebugCommandManager.Check_MashGroup() == false)
    {
        strcpy(msg, "mesh group check : success");
    }
    else
    {
        strcpy(msg, "mesh group check : fail");
    }

    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg);

    return TRUE;
}

//------------------------------------------------------------------------------ 
LuaGlue LG_TEST_SystemMessage_Test(lua_State* L)
{
    int string_code = (int)GET_LUA()->GetNumberArgument(1);

    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager != NULL)
    {
        uiSystemMessage::MessageType message_type = uiSystemMessage::kVerify_Type;
        bool is_modal = true;
        ui_system_manager->Sys_SystemMessage(
            g_InterfaceManager.GetInterfaceString(string_code), 
            NULL, 
            NULL, 
            message_type, 
            is_modal);
    }

    return TRUE;
}

//------   매크로 전용      ---------------------------------------------------- 
//------------------------------------------------------------------------------ 
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
LuaGlue LG_Macro_RunMacro(lua_State* L)
{
    int argNum = 1;
    BOOL bRet = FALSE;

    SUN_STRING strFile;
    strFile = (TCHAR *)GET_LUA()->GetStringArgument(argNum++);

    MacroCommand command;
    command.command_type_ = Macro_RunMacro;
    strncpy(command.value.file_name_, strFile.c_str(), LUA_FILE_NAME_LENGTH-1);
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}

LuaGlue LG_Macro_Move(lua_State* L)
{
    // 히어로 Move
    int argment_index = 1;
    float pos_x = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));
    float pos_y = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));
    float pos_z = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_Move;
    command.value.MovePosition.x_ = pos_x;
    command.value.MovePosition.y_ = pos_y;
    command.value.MovePosition.z_ = pos_z;
    xGetHeroData()->macro_system()->InsertCommand(command);
    
    return TRUE;
}
LuaGlue LG_Macro_MoveMap(lua_State* L)
{
    // 맵 이동
    int argment_index = 1;
    float map_code = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_MoveMap;
    command.value.map_code_ = map_code;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
//------------------------------------------------------------------------------ 
LuaGlue LG_Macro_NpcMeet(lua_State* L)
{
    // Npc창 오픈
    int argment_index = 1;
    DWORD npc_code = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_NpcMeet;
    command.value.npc_code_ = npc_code;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
LuaGlue LG_Macro_CreateRoom(lua_State* L)
{
    // 배틀존 방 생성
    int argment_index = 1;
    DWORD depth1 = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));
    DWORD depth2 = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));
    DWORD depth3 = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_CreateRoom;
    command.value.CreateRoomCommand.depth1_ = static_cast<BYTE>(depth1);
    command.value.CreateRoomCommand.depth2_ = static_cast<BYTE>(depth2);
    command.value.CreateRoomCommand.depth3_ = static_cast<BYTE>(depth3);
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};

LuaGlue LG_Macro_JoinRoom(lua_State* L)
{
    MacroCommand command;
    command.command_type_ = Macro_JoinRoom;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
//------------------------------------------------------------------------------ 
LuaGlue LG_Macro_ExitRoom(lua_State* L)
{
    MacroCommand command;
    command.command_type_ = Macro_ExitRoom;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
//------------------------------------------------------------------------------ 
LuaGlue LG_Macro_ResetMacrom(lua_State* L)
{
    xGetHeroData()->macro_system()->ResetMacro();

    return TRUE;
}
//------------------------------------------------------------------------------ 
LuaGlue LG_Macro_QuestAccept(lua_State* L)
{
    // 퀘스트 수락
    int argment_index = 1;
    DWORD quest_code = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_QuestAccept;
    command.value.quest_code_ = quest_code;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
LuaGlue LG_Macro_QuestReward(lua_State* L)
{
    // 퀘스트 완료
    int argment_index = 1;
    DWORD quest_code = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));
    DWORD reward_index = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_QuestReward;
    command.value.QuestReward.quest_code_ = quest_code;
    command.value.QuestReward.reward_index_ = reward_index;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
}
LuaGlue LG_Macro_Attack(lua_State* L)
{
    // 몬스터 사냥
    int argment_index = 1;
    DWORD monster = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));
    DWORD time = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_Attack;
    command.value.Attack.monster_code_ = monster;
    command.value.Attack.fight_time_ = time;
    xGetHeroData()->macro_system()->InsertCommand(command);


    return TRUE;
}
LuaGlue LG_Macro_Wait(lua_State* L)
{
    // 대기
    int argment_index = 1;
    DWORD wait_time = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_Wait;
    command.value.wait_time_ = wait_time;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_Jump(lua_State* L)
{
    // 점프 (전방으로)
    int argment_index = 1;

    MacroCommand command;
    command.command_type_ = Macro_Jump;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_MoveAttack(lua_State* L)
{
    // 이동 하며 공격
    int argment_index = 1;
    DWORD monster = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));
    float pos_x = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));
    float pos_y = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));
    float pos_z = static_cast<float>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_MoveAttack;
    command.value.MoveAttack.monster_code_ = monster;
    command.value.MoveAttack.x_ = pos_x;
    command.value.MoveAttack.y_ = pos_y;
    command.value.MoveAttack.z_ = pos_z;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_TriggerClick(lua_State* L)
{
    // 가장 가까운 트리거 클릭
    int argment_index = 1;

    MacroCommand command;
    command.command_type_ = Macro_TriggerClick;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_CollectClick(lua_State* L)
{
    // 가장 가까운 트리거 클릭
    int argment_index = 1;

    MacroCommand command;
    command.command_type_ = Macro_CollectClick;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_WriteLog(lua_State* L)
{
    // 로그 쓰기
    int argment_index = 1;
    BOOL is_write = static_cast<BOOL>(GET_LUA()->GetNumberArgument(argment_index++));

    xGetHeroData()->macro_system()->set_write_log(is_write);

    return TRUE;
};
LuaGlue LG_Macro_MissionReward(lua_State* L)
{
    // 미션 보상 받기
    int argment_index = 1;
    BYTE select_item = static_cast<BYTE>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_MissionReward;
    command.value.select_item_ = select_item;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_RuneStone(lua_State* L)
{
    // 룬스톤 사용
    int argment_index = 1;
    BYTE select_index = static_cast<BYTE>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_RuneStone;
    command.value.index_ = select_index;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_DummyMemory(lua_State* L)
{
    // 더미 메모리 할당
    int argment_index = 1;
    DWORD size = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    MacroCommand command;
    command.command_type_ = Macro_DummyMemory;
    command.value.size_ = size;
    xGetHeroData()->macro_system()->InsertCommand(command);

    return TRUE;
};
LuaGlue LG_Macro_ItemPickup(lua_State* L)
{
    // 주울 아이템 추가
    int argment_index = 1;
    DWORD item_code = static_cast<DWORD>(GET_LUA()->GetNumberArgument(argment_index++));

    xGetHeroData()->macro_system()->add_pickup_item(item_code);

    return TRUE;
};

#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

//-------------------------------------------------------------------------------------------
/**
*/
luaDef SunGlue[] = 
{
    {"RunScript",		LG_RunScript},

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    {"RunMacro",       LG_Macro_RunMacro},
    {"Move",           LG_Macro_Move},
    {"MoveMap",        LG_Macro_MoveMap},
    {"NpcMeet",        LG_Macro_NpcMeet},
    {"CreateRoom",     LG_Macro_CreateRoom},
    {"JoinRoom",       LG_Macro_JoinRoom},
    {"ExitRoom",       LG_Macro_ExitRoom},
    {"ResetMacro",     LG_Macro_ResetMacrom},

    {"QuestAccept",    LG_Macro_QuestAccept},
    {"QuestReward",    LG_Macro_QuestReward},
    {"Attack",         LG_Macro_Attack},
    {"Wait",           LG_Macro_Wait},
    {"Jump",           LG_Macro_Jump},
    {"MoveAttack",     LG_Macro_MoveAttack},
    {"TriggerClick",   LG_Macro_TriggerClick},
    {"CollectClick",   LG_Macro_CollectClick},
    {"WriteLog",       LG_Macro_WriteLog},
    {"MissionReward",  LG_Macro_MissionReward},

    {"RuneStone",      LG_Macro_RuneStone},
    {"DummyMemory",    LG_Macro_DummyMemory},
    {"ItemPickup",     LG_Macro_ItemPickup},
    
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

#ifdef _DEV_VER
	{"SetObjectPosition", LG_SetObjectPosition},
	{"GetPlayerCount", LG_GetPlayerCount},
	{"GetMonsterCount", LG_GetMonsterCount},
	{"OpenComposite", LG_TEST_OpenComposte},
	{"OpenSocketExtract", LG_TEST_OpenSocketExtract},
	{"OpenEnchant", LG_TEST_OpenEnchant},
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
	{"openhelp",		LG_TEST_OpenHelp},
	{"openhelpstring",	LG_TEST_OpenHelpString},
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

	{"opengmhelp",		LG_TEST_OpenGMHelp},

	{"addmhelp",			LG_TEST_AddMissionHelp},
	{"delmhelp",			LG_TEST_DelMissionHelp},

	{"SetDlgPos",		LG_SetWindowPosition},
	{"AttachTestCase",	LG_TEST_AttachTestCase},
	{"DetachTestCase",	LG_TEST_DetachTestCase},
	{"AppendFText",		LG_TEST_AppendFormatText},
	{"ClearFText",		LG_TEST_ClearFormatText},
	{"SetFTextRect",	LG_TEST_SetFormatTextRect},
	{"tooltipitem",		LG_TEST_TooltipItem},	
	{"tooltipclear",	LG_TEST_TooltipClear},	
	{"NPCMeet",			LG_NPC_MEET},
	{"CreateHero",		LG_CREATE_HERO},
	{"LoadMap",			LG_Load_Map},
    {"LoadMapTest",		LG_Load_Map_All},    
	{"SetLevel",		LG_SetLevel},
	{"InsertItem",		LG_AddItemAtInventory},
	{"SetMoney",		LG_SetMoney},
	{"ToggleACDialog", LG_ToggleACDialog},
	{"OpenRewardDialog",LG_OpenRewardDialog},
	{"OpenQuestList",  LG_TEST_OpenQuestList},
	{"SaveQuestInfo",  LG_TEST_SaveQuestList},
	{"LoadQuestInfo",  LG_TEST_LoadQuestList},
	{"MonsterKillCmd", LG_TEST_MONSTER_KILL_CMD},	
	{"SettingNPC",	   LG_TEST_SettingNPC},	
	{"RefreshNPCState",LG_TEST_RefreshNPCState},
	{"InitQuestArea",  LG_TEST_InitQuestAreaInfo},
	{"AcceptQuest",	   LG_TEST_Quest_Accept},	
	{"ShowGameInterface",  LG_TEST_ShowGameInterface},
	{"openevent",		LG_TEST_EventWindow},
	{"waypoint",		LG_TEST_WayPointWindow},
	{"savewp",			LG_TEST_SaveWayPoint},
	{"testwp",			LG_TEST_TestWayPoint},
	{"openkillpoint",		LG_TEST_OpenKillPoint},
	{"setkillpoint",		LG_TEST_SetKillPoint},
	{"SendAutoQuest", LG_TEST_CMD_AUTO_QUEST},
	{"exit",		LG_TEST_Exit},
	{"SetMissionPoint", LG_TEST_SetMissionPosint},
	{"CmdVenderBuy", LG_TEST_CMD_VENDOR_BUY},
    {"AttackJump", LG_TEST_AttackJump }, 
    {"EventJump", LG_TEST_EventJump },     
    { "SendMoveNak", LG_TEST_SendMoveNak },	
	{ "jjumbo", LG_TEST_jjumbo },
    { "UpdateAutoQuestRequest", LG_TEST_UpdateAutoQuestRequest },    
    { "SpawnMonster", LG_TEST_SpawnMonster },
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	{ "ranklist", LG_TEST_MissionRankList },
	{ "rankgamble", LG_TEST_MissionRankGamble },
	{ "rank", LG_TEST_RqstMissionRank },
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
    { "RunAway", LG_TEST_RunAway },
	{ "CurrentVisiblePos", LG_TEST_CurrentPos },
	{ "SetChaoState", LG_TEST_SetChaoState },
    { "rqstmemolist", LG_TEST_RqstMemoList },
	{ "ShowCashItemShopNpc", LG_TEST_ShowCashItemShopNpc },
	{ "ShowBillingAlarm", LG_TEST_ShowBillingAlarm },
	{ "ShowDisconnectAlarm", LG_TEST_ShowDisconnectAlarm },
	{ "PopupMesh", LG_TEST_PopupMesh },
	{ "DeleteItemCmd", LG_TEST_DeleteItemCmd },
	{ "SetBrightDelay", LG_TEST_SetBrightDelay },
	{ "PlayEffectSound", LG_TEST_PlayEffectSound },
	{ "PlayEffectSounds", LG_TEST_PlayEffectSounds },
	{ "SpawnBerserker", LG_TEST_SpawnFullsetPlayer},
	{ "StopAllVoice", LG_TEST_StopAllVoice },
	{ "ItemDestroyDuraCmd", LG_TEST_ItemDestroyDuraCmd },
	{ "InsertInventoryItem", LG_TEST_InsertInvertoryItem},
	{ "SetProjection" , LG_TEST_SetProjection},
	{"CreateEtherEffect" , LG_TEST_CreateEtherWeaponEffect},
	{"DestorEtherEffect", LG_TEST_DestoryEtherWeaponEffect},


	{"OpenSetLottoNum" , LG_TEST_OpenSetLottoNum},
	{"OpenLottoWindow" , LG_TEST_OpenLottoWindow},
	{"OpenLottoNumInput", LG_TEST_OpenLottoNumInput},

	{"CreateEffect", LG_TEST_CreateEffect},

	{"SetGrayScale", LG_TEST_SetGrayScale},
	{"SetGrayScaleColor", LG_TEST_SetGrayScaleColor},
	{"SetSepiaMode", LG_TEST_SetSepiaMode},
	{"SSQRewardFail", LG_TEST_SSQRewardFail},
	{"ForceMoveToHero", LG_TEST_ForceMoveToHero},
	{"MissionRelay", LG_TEST_MissionRelay},


	{"LottoActive", LG_TEST_LottoActive},
	{"SendMSG", LG_TEST_SendMSG},
	{"CreateRider",LG_TEST_CreateRider},
	{"RiderPart",LG_TEST_RiderPart},
	{"SetSpeed",LG_TEST_SetSpeed},
	{"CreateRiderPlayer",LG_TEST_CreateRiderPlayer},
	{"DeleteEffect",LG_TEST_DeleteEffect},
	{"AssertTest",LG_TEST_AssertTest},
    
    {"JumpGravity",LG_TEST_JumpGravity},
    {"JumpForce",LG_TEST_JumpForce},
    {"MotionBlur",LG_TEST_MotionBlur},

    {"MemUsage",LG_TEST_MemoryUsage},

    {"OpenCharCustomize",LG_TEST_OpenCharCustomize},

    {"SetSeekNodeCount",LG_TEST_SetSeekNodeCount},
    {"AutobotAction",LG_TEST_AutobotAction},
    {"SetShadowDepthOff",LG_TEST_SetShadowDepthOff},
    {"OpenItemCube",LG_TEST_OpenItemCube},
    {"SocketComposite",LG_TEST_OpenSocketComposite},
//#ifdef _DEV_VER
    {"GMWin", LG_TEST_OpenGMWin},
//#endif//_DEV_VER//_DH_GM_ITEM_WINDOW

    {"show_dialog",LG_TEST_Show_Dialog},
    {"graphic_ini_reload", LG_TEST_Graphic_Ini_Reload},

    {"tex_ani_reload", LG_TEST_Tex_Ani_Reload},

    {"custom_camera_reload", LG_TEST_Custom_Camera_Reload},

    {"script_reload", LG_TEST_Script_Reload},

    {"list", LG_TEST_List},

    {"check_meshgroup", LG_TEST_Check_Meshgroup},

    {"messagebox", LG_TEST_SystemMessage_Test},    
#endif //_DEV_VER

    {NULL, NULL},
};

#pragma warning ( pop )

#endif
//_USE_LUA_SCRIPT
