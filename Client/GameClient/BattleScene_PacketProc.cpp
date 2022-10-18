//------------------------------------------------------------------------------
//  BattleScene_PacketProc.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "GameFramework.h"
#include <ItemAttrCalculator.h>
#include "Player.h"
#include "Monster.h"
#include "BattleScene.h"
#include "varpacket.h"
#include "Camera.h"
#include "ItemManager.h"
#include "HotKeyManager.h"
#include "HeroActionInput.h"
#include "Battle.h"
#include "SoundBGM.h"
#include "SolarDialog.h"
#include "LoadScene.h"
#include "Map.h"
#include "GameFunc.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "HeroGaugeDialog.h"
#include <SkillInfoParser.h>
#include "ChatDialog.h"
#include "PlayerAttributes.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "char_skilldialog.h"
#include "MapInfoParser.h"
#include "SCSlotContainer.h"
#include "SCItemSlot.h"
#include "EquipmentContainer.h"
#include "CloneManager.h"
#include <NPCInfoParser.h>
#include <StyleInfoParser.h>
#include <ItemInfoParser.h>
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "ResultCode.h"
#include "RewardDialog.h"
#include "ProjectileManagerEx.h"
#include "CommonCharacterFormula.h"
#include "MouseCursor.h"
#include "State/SkillStateFactory.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "uiGuildMan/uiGuildMan.h"
#include "radarplusdialog.h"
#include "radardialog.h"
#include "AppearanceManager.h"
#include "uiHelpMan/uiHelpMan.h"
#include "HeadMeshInfoParser.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "TiredSystemDialog.h"
#include "QuestManager_Concrete.h"
#include "CursorChangeTransact.h"
#include "Clone.h"
#include "uiPartyMan/uiPartyMan.h"
#include "EnduranceDialog.h"
#include "NumericValues.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "ResourceManager.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "SkillAttrCalculator.h"
#include "BattleLobby.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorList/uiVendorList.h"
#include "EventInfoParser.h"
#include "MissionScene.h"
#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "MissionRelay.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "TriggerManEx.h"
#include "Trigger.h"
#include "uiChangeRoomName/uiChangeRoomName.h"
#include "OptionGameDialog.h"
#include "InstanceDungeonScene.h"
#include "ItemShareSystem.h"
#include "NPC.h"
#include "SunEventInfoParser.h"
#include "uiPartyMan/uiPartyMan.h"
#include "ClientVer.h"
#include "NpcImageInfo.h"
#include "uiTradeMan/uiTradeMan_def.h"
#include "PlayerMonster.h"
#include "uiSSQMan/uiSSQManager.h"
#include "StateInfoParser.h"
#include <PacketControl/WarMessages_Sync.h>
#include "uiChatMan/uiChatMan_def.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "SolarDateTime.h"

#include "uiQuestListDlg/uiQuestListDlg.h"

#include "RadarFullDialog.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationStamp/uiDominationStamp.h"

#include "MissionManager_Concrete.h"

#ifdef _LTJ_GUILDMASTER_DOMINATION_EFFECT
#include "uiDominationZoneMarkDlg/uiDominationZoneMarkDlg.h"
#endif

#include "uiBattleScoreMan\uiBattleScoreMan.h"

#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "uiMissionObject/uiMissionObject.h"

#include "uiSkillMan/uiNewSkillMan.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#include "Rider.h"

#include "uiTargetMan/uiTargetMan.h"
#include "uiTargetAggro/uiTargetAggro.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#ifdef _NA_002935_20110704_ITEM_SCORE
#include "HeroEquipmentContainer.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#include "XignCode/XignCodeApi.h"
#include "uiLoginMan/uiLoginMan.h"
#include "KBMoveCheckManagerInClient.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif//#ifdef _DH_BATTLEZONE2_

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

#include "uiBankMan/uiBankDef.h"
#include "SkillFacade.h"
#include "HeroData.h"
#include "CooltimeManager.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemPacketParser.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "SkillEffectFunc.h"

#include "State/SkillState.h"
#include "State/SkillStateHelroidSpin.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
  #include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
  #include "uiSmartNpcDialogMan/uiSmartNpcDialog.h"
  #include "uiSmartNpcDialogMan/uiSmartNpcDialogReply.h"

  #include "PacketStruct_CG_SmartNPC.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "TiredSystemDialog.h"
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "uiDominationTimer/uiDominationTimer.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

using namespace RC;

class Attributes;

//------------------------------------------------------------------------------
/**
	파티장 소환 
*/
void _CallBack_Member_Summon_Req(bool bYes)
{
	if (bYes)
	{	
		MSG_CG_PARTY_MEMBER_SUMMON_RES_SYN sync;
		sync.m_byCategory = CG_PARTY;
		sync.m_byProtocol = CG_PARTY_MEMBER_SUMMON_RES_SYN;
		sync.m_byResultCode =RC_PARTY_SUCCESS;
		if(g_pHero)
			g_pHero->SendPacket(&sync,sizeof(sync));
	}
	else
	{
		MSG_CG_PARTY_MEMBER_SUMMON_RES_SYN sync;
		sync.m_byCategory = CG_PARTY;
		sync.m_byProtocol = CG_PARTY_MEMBER_SUMMON_RES_SYN;
		sync.m_byResultCode =RC_PARTY_REJECT_SUMMON;
		if(g_pHero)
			g_pHero->SendPacket(&sync,sizeof(sync));
	}
}

//------------------------------------------------------------------------------
/**
*/
void _callback_FollowPartyMaster(bool bYes)
{
	if (bYes)
	{
		MSG_CG_ZONE_LOBBY_JOIN_SYN	SendPacket;
		SendPacket.m_LobbyKey		= (KEYTYPE)BattleScene::GetPendingBattleRoomEntry()->GetSerial();
		SendPacket.m_byRoomPublic	= BattleScene::GetPendingBattleRoomEntry()->GetPublic();		//< eZONEPUBLIC

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SHOP , false );


		if (eZONEPUBLIC_PRIVATE == SendPacket.m_byRoomPublic)
		{
			StrnCopy(SendPacket.m_pszRoomPWD, BattleScene::GetPendingBattleRoomEntry()->GetPassword(), MAX_ROOMPASSWORD_LENGTH);
		}


		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());
	}
	else
	{
		uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
		if (partyMan)
		{
			partyMan->LeaveParty();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ParsePacketInChat( MSG_BASE * pMsg )
{
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    __UNUSED(pMsg);
    assert(!"this "__FUNCTION__" is not called anymore");
    return;
#else
	SceneBase::ParsePacketInChat( pMsg );
#endif
}

void BattleScene::OnRecvCG_Vendor(MSG_BASE* pMsg)
{
	switch( pMsg->m_byProtocol )
	{

		case CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_CMD:
			{

				ISecureWorkNode::OnMsgRecvSignal::MOLA_Query( pMsg, 0 );
			}
			break;

	}

}

void BattleScene::OnRecvCG_WareHouse(MSG_BASE* pMsg)
{
	switch( pMsg->m_byProtocol )
	{
		case CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_CMD:
			{
				ISecureWorkNode::OnMsgRecvSignal::MOLA_Configure( pMsg, 0 );
			}
			break;

	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ParsePacket( MSG_BASE * pMsg )
{
	switch( pMsg->m_byCategory )
	{	
    case CG_VENDOR:
        {
			OnRecvCG_Vendor(pMsg);
			
			GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL )->NetworkProc(pMsg);
			GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_BUY )->NetworkProc(pMsg);
			GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_LIST )->NetworkProc(pMsg);

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
			GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SEARCH)->NetworkProc(pMsg);
#endif
			
        }
        break;

    case CG_TRADE:
        {
        }
        break;


    case CG_CONNECTION:
		{
			OnRecvCG_CONNECTION( pMsg );			
		}
		return;

    case CG_GM:
        {
            OnRecvCG_GM( pMsg );
        }
        break;

	case CG_CHARINFO:
		{	     
			OnRecvCG_CHARINFO( pMsg );					
		}
		return;	

	case CG_MAP:
		{
			OnRecvCG_MAP( pMsg );
					
		}
		return;	

	case CG_SYNC:
		{
			if (IsLobby())
			{
				if (IsLobbyInPending())
				{
					OnRecvCG_SYNC_In_Lobby_Pending( pMsg );
				}
				else
				{
					OnRecvCG_SYNC_In_Lobby( pMsg );
				}
			}
			else
			{
				if (IsLobbyOutPending())
				{
					OnRecvCG_SYNC_In_Lobby_Out_Pending( pMsg );
				} 
				else
				{
					OnRecvCG_SYNC( pMsg );
				}
			}
		}
		return;

	case CG_BATTLE:
		{			
			OnRecvCG_BATTLE( pMsg );					
		}
        return;	

	case CG_STYLE:
		{		
			OnRecvCG_STYLE( pMsg );			
		}
		return;

	case CG_SKILL:
		{
			OnRecvCG_SKILL( pMsg );					
		}
		return;	

	case CG_ZONE:
		{
			OnRecvCG_ZONE(pMsg );			
		}
		return;

	case CG_CONVERSATION:
		{
			OnRecvCG_CONVERSATION( pMsg );			
		}
		return;

	case CG_ITEM:
		{
			ItemManager::Instance()->ParsePacket( pMsg );
		}
		return;

	case CG_EVENT:
		{
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
			this->OnRecvCG_EVENT( pMsg );
#else
			this->OnRecvCG_EVENT_INVENTORY( pMsg );
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
		}
		return;

	case CG_WAREHOUSE:
		{
			OnRecvCG_WareHouse(pMsg);
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
			g_InterfaceManager.NetworkProc(IM_BANK_MANAGER::BANK_DIALOG, pMsg );			
#endif
		}
		return;

	case CG_STATUS:
        {			
			OnRecvCG_STATUS( pMsg );			
		}
		return;;

	case CG_TRIGGER:
		{
			OnRecvCG_TRIGER( pMsg );
		}
		return;

    case CG_PARTY:
        {
            OnRecvCG_Party( pMsg );            
        }
        return;

	case CG_SUMMON:
		{
			OnRecvCG_Summon( pMsg );
		}
		return;


	case CG_CHAO:
		{
			OnRecvCG_Chao( pMsg );
		}
		return;


	case CG_AREACONQUEST:
		{
			OnRecvCG_AREACONQUEST( pMsg );
		}
		return;
	case CG_MISSION:
		{
			OnRecvCG_MISSION( pMsg );
		}
		return;

	case CG_ZONE_EX:
		{
			OnRecvCG_ZONE_EX(pMsg);
		}
		return;

	case CG_COLLECT:
		{
			OnRecvCG_COLLECT(pMsg);
		}
		break;
	default:
		{
			SceneBase::ParsePacket(pMsg);
		}
		break;
    }
}


//------------------------------------------------------------------------------
/**
*/
void 
BattleScene::OnRecvCG_CONNECTION( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch(pMsg->m_byProtocol)
	{
	case CG_CONNECTION_BACKTOCHARSELECT_ACK:
		{
			m_bPendingBackToSomeWhere = false;
			
			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );
			g_LoadScene.ChangeImage(TRUE);

			g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SYSTEM, FALSE );

			g_InterfaceManager.ShowGameInterface(FALSE);

			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg );

			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
		}
		break;

	case CG_CONNECTION_BACKTOCHARSELECT_NAK:
		{
			m_bPendingBackToSomeWhere = false;
			
			//	캐릭터 선택창으로 돌아가기가 실패했습니다.
			const int	c_iCharSelectReturnFailed = 70246;
			g_InterfaceManager.GetInterfaceString( c_iCharSelectReturnFailed, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case CG_CONNECTION_SERVER_SHUTDOWN_BRD:	// 운영 명령어(종료메시지) 처리
		{
			MSG_CG_CONNECTION_SERVER_SHUTDOWN_BRD * pRecvMsg = (MSG_CG_CONNECTION_SERVER_SHUTDOWN_BRD *)pMsg;
			
			uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

			if(helpMan)
			{
				if(pRecvMsg->m_dwStringCode == 0)
				{
					// 운영 명령어 처리용 헬프메시지 출력
					helpMan->ShowGMMsg(60001, pRecvMsg->m_byShutdownTime, true);
				}
				else
				{
					helpMan->ShowGMMsg(pRecvMsg->m_dwStringCode, pRecvMsg->m_byShutdownTime, true);
				}
			}
		}
		break;

	default:
		{
			SceneBase::OnRecvCG_CONNECTION(pMsg);
		}
		break;
	}
}


//------------------------------------------------------------------------------
/**
*/
void
BattleScene::OnRecvCG_Party( MSG_BASE * pMsg )
{
	switch(pMsg->m_byProtocol)
	{

	case CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::MOLA_Configure( pMsg, 0 );
		}
		break;


	case CG_PARTY_MINIMAP_POINTING_BRD:
		{
			MSG_CG_PARTY_MINIMAP_POINTING_BRD *pRecvPacket = (MSG_CG_PARTY_MINIMAP_POINTING_BRD *)pMsg;
            
            uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
            if (ui_radar_manager != NULL)
            {
                ui_radar_manager->SetClickPosition(pRecvPacket->m_fx, pRecvPacket->m_fy, 5000);
            }
		}
		break;

	case CG_PARTY_MEMBER_WARP_REQ_NAK:
		{
			//Todo:파티장 근처로 가기 실패 처리 
			MSG_CG_PARTY_MEMBER_WARP_REQ_NAK *pRecvPacket = (MSG_CG_PARTY_MEMBER_WARP_REQ_NAK *)pMsg;
			
			uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			if (partyMan)
			{
				partyMan->Reason(pRecvPacket->m_byErrorCode);
			}
		}
		break;

	case CG_PARTY_MEMBER_SUMMON_REQ_NAK:
		{
			//Todo:파티원 소환 요청 전달 실패 
			MSG_CG_PARTY_MEMBER_SUMMON_REQ_NAK *pRecvPacket = (MSG_CG_PARTY_MEMBER_SUMMON_REQ_NAK *)pMsg;
		
			uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			if (partyMan)
			{
				partyMan->Reason(pRecvPacket->m_byErrorCode);
			}
		}
		break;

	case CG_PARTY_MEMBER_SUMMON_REQ_CMD:
		{
			// 5180	파티장이 당신을 자신이 있는 곳으로 소환하려고 합니다. 확인버튼을 누르면 이동됩니다.
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString( 5180, szMessage, INTERFACE_STRING_LENGTH );
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_Member_Summon_Req);
		}
		break;

	case CG_PARTY_MEMBER_SUMMON_RES_NAK:
		{
			//파티원 소환 응답 실패 처리 
			MSG_CG_PARTY_MEMBER_SUMMON_RES_NAK *pRecvPacket = (MSG_CG_PARTY_MEMBER_SUMMON_RES_NAK *)pMsg;
		
			uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			if (partyMan)
			{
				partyMan->Reason(pRecvPacket->m_byErrorCode);
			}

		}
		break;

	case CG_PARTY_ITEM_DISTRIBUTION_CHANGE_NAK:
		{
			MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_NAK * pRecvPacket = (MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_NAK *) pMsg;
			uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			if (partyMan)
			{
				partyMan->Reason(pRecvPacket->m_byErrorCode);
			}
		}
		break;

	case CG_PARTY_ITEM_DISTRIBUTION_CHANGE_BRD:
		{
			MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_BRD * pRecvPacket = (MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_BRD *) pMsg;

			ItemShareSystem * pItemShareSystem = GET_CAST_DIALOG( ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM );
			
			if( pItemShareSystem) 
			{
				pItemShareSystem->SetToggleName(pRecvPacket->m_byItemDistributionType);
			}
		}
		break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case CG_CHAOS_ZONE_PACKET:
        {
            ChaosSystem::ParserPacket::ParsePacketChaosSystem(pMsg);
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	default:
		{
			SceneBase::OnRecvCG_Party(pMsg);
		}
		break;

	}
}

//------------------------------------------------------------------------------
/**
*/
void
BattleScene::OnRecvCG_Summon(MSG_BASE* pMsg)
{
	switch (pMsg->m_byProtocol)
	{
		case CG_SUMMON_CURRENT_MP_CMD:
			{
				MSG_CG_SUMMON_CURRENT_MP_CMD* pRecvPacket = (MSG_CG_SUMMON_CURRENT_MP_CMD*)pMsg;
				Character* pChar=(Character*)g_ObjectManager.GetObject(pRecvPacket->m_dwSummonedObjKey);
				if (!pChar)
                {
                    break;
                }

				pChar->SetMaxMP(pRecvPacket->m_dwMaxMP);
				pChar->SetMP(pRecvPacket->m_dwMP);
			}
			break;

		case CG_SUMMON_CURRENT_HP_CMD:
			{
				MSG_CG_SUMMON_CURRENT_HP_CMD* pRecvPacket = (MSG_CG_SUMMON_CURRENT_HP_CMD*)pMsg;
				Monster* pSummon = (Monster*)g_ObjectManager.GetObject(pRecvPacket->m_dwSummonedObjKey);
				assert(pSummon);
				if (pSummon)
				{
					pSummon->SetMaxHP(pRecvPacket->m_dwMaxHP);
					pSummon->SetHP(pRecvPacket->m_dwHP);
					pSummon->SetMoveSpeedRatio(pRecvPacket->m_dwSpeedRatio);

				}
			}
			break;

		case CG_SUMMON_CURRENT_HP_BRD:
			{
				MSG_CG_SUMMON_CURRENT_HP_BRD* pRecvPacket = (MSG_CG_SUMMON_CURRENT_HP_BRD*)pMsg;
				Monster* pSummon = (Monster*)g_ObjectManager.GetObject(pRecvPacket->m_dwSummonedObjKey);
				assert(pSummon);
				if (pSummon)
				{
					pSummon->SetMaxHP(pRecvPacket->m_dwMaxHP);
					pSummon->SetHP(pRecvPacket->m_dwHP);
					pSummon->SetMoveSpeedRatio(pRecvPacket->m_dwSpeedRatio);
				}
			}
			break;


		case CG_SUMMON_COMMAND_ACK:
			{
				MSG_CG_SUMMON_COMMAND_ACK *pRecvPacket = (MSG_CG_SUMMON_COMMAND_ACK *)pMsg;
		
				SummonTargetDialog * pSummonTargetDlg = GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);

				if (pSummonTargetDlg)
				{
					pSummonTargetDlg->ResetOhterCheckButton(pRecvPacket->m_byCommand);
				}
			}
			break;

		case CG_SUMMON_COMMAND_NAK:
			{
				MSG_CG_SUMMON_COMMAND_NAK *pRecvPacket = (MSG_CG_SUMMON_COMMAND_NAK *)pMsg;
				
				SummonTargetDialog * pSummonTargetDlg = GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);

				if (pSummonTargetDlg)
				{
					pSummonTargetDlg->ResetCheckButton(pRecvPacket->m_byCommand);
				}

			}
			break;

		case CG_CRYSTALWARP_COMMAND_BRD:
			{
				MSG_CG_CRYSTALWARP_COMMAND_BRD* pRecvPacket = (MSG_CG_CRYSTALWARP_COMMAND_BRD*)pMsg;
				if(eCRYSTALWARP_COMMAND_EXECUTE == pRecvPacket->m_byCommand)
				{
					Object* pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);
					if(pObject && pObject == g_pHero)
						SetBrightDelay(300);
				}
				else if(eCRYSTALWARP_COMMAND_DESTROY == pRecvPacket->m_byCommand)
				{
					Object* pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);
					if(NULL == pObject || FALSE == pObject->IsKindOfObject(PLAYER_OBJECT))
					{
#ifdef _DEV_VER
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "MSG_CG_CRYSTALWARP_COMMAND_ACK::Object가 NULL");
#endif //_DEV_VER
						break;
					}
				}

			}
			break;

		case CG_CRYSTALWARP_COMMAND_NAK:
			{
				MSG_CG_CRYSTALWARP_COMMAND_NAK* pRecvPacket = (MSG_CG_CRYSTALWARP_COMMAND_NAK*)pMsg;
				if(g_pHero)
					g_pHero->SendStopMovePacket();

				OutputSummonResultMsg(pRecvPacket->m_byErrorCode);
			}
			break;
	}

}


//------------------------------------------------------------------------------
/**
*/
void		
BattleScene::OnRecvCG_EVENT_INVENTORY( MSG_BASE * pMsg )
{
	g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_EVENT_INVENTORY, pMsg);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
void BattleScene::OnRecvCG_EVENT_PEAKTIME_NOTIFY( MSG_BASE* message )
{
	g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_CHAT, message);
}

void BattleScene::OnRecvCG_GMEVENT_PEAKTIME_EVENTLIST_CMD( MSG_BASE* message )
{
	MSG_CG_GMEVENT_PEAKTIME_EVENTLIST_CMD* recv_packet = static_cast<MSG_CG_GMEVENT_PEAKTIME_EVENTLIST_CMD*>(message);

	if ( recv_packet->data_count < 1 )
	{
		TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
		// 70681 : 진행중인 경험치 이벤트가 존재하지 않습니다.
		g_InterfaceManager.GetInterfaceString(70681, result_message);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message, CHAT_TYPE_SYSTEM);

		return;
	}

	for ( int i = 0; i < recv_packet->data_count; ++i )
	{
		TCHAR event_list_message[INTERFACE_STRING_LENGTH] = {0, };

		time_t start_t = (time_t)recv_packet->event_data[i].start_date;
		time_t end_t = (time_t)recv_packet->event_data[i].end_date;
		tm start_time;
		tm end_time;
		tm* t = ::localtime(&start_t);
		CopyMemory(&start_time, t, sizeof(tm));
		t = ::localtime(&end_t);
		CopyMemory(&end_time, t, sizeof(tm));

		Snprintf(event_list_message, INTERFACE_STRING_LENGTH-1, _T("%d%02d%02d %02d%02d %d%02d%02d %02d%02d %d"), 
			start_time.tm_year+1900, 
			start_time.tm_mon+1, 
			start_time.tm_mday, 
			start_time.tm_hour, 
			start_time.tm_min,
			end_time.tm_year+1900, 
			end_time.tm_mon+1, 
			end_time.tm_mday, 
			end_time.tm_hour, 
			end_time.tm_min, 
			(int)(recv_packet->event_data[i].exp_ratio * 100));

		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,event_list_message, CHAT_TYPE_SYSTEM);
	}
}

void BattleScene::OnRecvCG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD( MSG_BASE* message )
{
	MSG_CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD* recv_packet = static_cast<MSG_CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD*>(message);

	TiredSystemDialog* tired_system_dialog = static_cast<TiredSystemDialog*>(GET_DIALOG( IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ));
	if ( recv_packet->exp_ratio > 0.0001f )
	{		
		if ( tired_system_dialog )
		{
			tired_system_dialog->set_gm_event_exp_ratio_( recv_packet->exp_ratio );
		}
	}
	else
	{
		if ( tired_system_dialog )
		{
			tired_system_dialog->set_gm_event_exp_ratio_( 0.0f );
		}
	}
}
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT

void BattleScene::OnRecvCG_COLLECT( MSG_BASE * pMsg  )
{
	TCHAR		szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch(pMsg->m_byProtocol)
	{
	case CG_COLLECT_ALLCOLLECTIVE_CMD:
		{
			MSG_CG_COLLECT_ALLCOLLECTIVE_CMD* pRecvMsg = (MSG_CG_COLLECT_ALLCOLLECTIVE_CMD*)pMsg;
			if(1 > pRecvMsg->m_byCount)
			{
				assert(!"MSG_CG_COLLECT_ALLCOLLECTIVE_CMD::(1 > pRecvMsg->m_byCount)");
				break;
			}

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("CG_COLLECT_ALLCOLLECTIVE_CMD::cnt %d"), pRecvMsg->m_byCount);
#endif 

			for(int i = 0; i < pRecvMsg->m_byCount; i++)
			{
				COLLECT_RENDER_INFO* pCRI = &(pRecvMsg->m_CollectiveInfo[i]);
				Object* pObj = g_ObjectManager.GetObject(pCRI->m_dwObjectKey.Get());
				MapObject* pMapObj = NULL;
				Player*	   pPlayer = NULL;
				if(pObj && pObj->IsKindOfObject(MAP_OBJECT))
				{
					pMapObj = (MapObject*)pObj;
					pMapObj->SetCollectionID(pCRI->m_CollectionID);
				}
				else
				{
					assert(!"MSG_CG_COLLECT_ALLCOLLECTIVE_CMD::(!pObj || !pObj->IsKindOfObject(MAP_OBJECT))");
					break;
				}
			}
		}
		break;

	case CG_COLLECT_LEAVE_SECTOR_CMD:
		{
			MSG_CG_COLLECT_LEAVE_SECTOR_CMD* pRecvMsg = (MSG_CG_COLLECT_LEAVE_SECTOR_CMD*)pMsg;

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_LEAVE_SECTOR_CMD"));
#endif 

			for(int i = 0; i < pRecvMsg->m_byCount; i++)
			{
				Object* pObj = g_ObjectManager.GetObject(pRecvMsg->m_ObjKeyCollection[i].Get());
				if(NULL ==pObj || FALSE == pObj->IsKindOfObject(MAP_OBJECT))
				{
					assert(!"CG_COLLECT_LEAVE_SECTOR_CMD::if(NULL ==pObj && FALSE == pObj->IsKindOfObject(MAP_OBJECT))");
					break;
				}

				MapObject* pMapObj = (MapObject*)pObj;
				pMapObj->SetCollectionID(0);
			}
			
		}
		break;

	case CG_COLLECT_ACTION_BRD:
		{
            MSG_CG_COLLECT_ACTION_BRD* pRecvMsg = (MSG_CG_COLLECT_ACTION_BRD*)pMsg;

            MSGSUB_COLLECT_ACTION_BRD brd;
            brd.object_key = pRecvMsg->m_dwObjectKey;
            brd.collection_id = pRecvMsg->m_CollectionID;
            brd.collector = pRecvMsg->m_Collector;

            OnRecv_MSGSUB_COLLECT_ACTION_BRD(&brd);
            break;
		}
		break;

	case CG_COLLECT_ACTION_NAK:
		{
            MSG_CG_COLLECT_ACTION_NAK* pRecvMsg = (MSG_CG_COLLECT_ACTION_NAK*)pMsg;

#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_ACTION_NAK::%d"), pRecvMsg->m_byErrorCode);
#endif 
            OutputCollectionResultMsg(pRecvMsg->m_byErrorCode);

			if(g_pHero)
			{
#ifdef _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT
                // 채집 실패 시 해당 맵오브젝트의 채집키 제거 (시범적으로 미션에서만 적용)
                if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
                {
                    if ((pRecvMsg->m_byErrorCode == RC_COLLECT_FAIL_COUNTCONDISION) ||
                        (pRecvMsg->m_byErrorCode == RC_COLLECT_NOTEXIST))
                    {
                        DWORD object_key = g_pHero->GetCollectObjKey();
                        if (object_key != 0)
                        {
                            MapObject* map_object = (MapObject*)g_ObjectManager.GetObject(object_key);
                            if (map_object)
                            {
                                map_object->SetCollectionID(0);
                            }
                        }
                    }
                }
#endif //_NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT

				g_pHero->SetCollectObjKey(0);
				g_pHero->SetCollectID(0);
				g_pHero->SetCollectStartTime(0);
			}

			std::vector<Player*> vecPlayers;
			g_ObjectManager.GetPlayers(vecPlayers);
			int iCnt = vecPlayers.size();
			for(int i = 0; i <iCnt; ++i)
			{
				Player* pPlayer = vecPlayers[i];
				if( NULL != g_pEffectManager->GetEffectPtrHandle( pPlayer->GetCollectStartEftHndl() ) )
				{
					g_pEffectManager->RemoveEffectPtr(pPlayer->GetCollectStartEftHndl());
					pPlayer->SetCollectStartEftHndl(INVALID_HANDLE_VALUE);
				}
			}
			vecPlayers.clear();
		}
		break;

	case CG_COLLECT_CANCEL_BRD:
		{
			MSG_CG_COLLECT_CANCEL_BRD* pRecvMsg = (MSG_CG_COLLECT_CANCEL_BRD*)pMsg;

            MSGSUB_COLLECT_CANCEL_BRD brd;
            brd.object_key = pRecvMsg->m_dwObjectKey;
            brd.collection_id = pRecvMsg->m_CollectionID;
            brd.collector = pRecvMsg->m_Collector;

            OnRecv_MSGSUB_COLLECT_CANCEL_BRD(&brd);
		}
		break;

    //(채집시스템+채집획득CMD)
	case CG_COLLECT_OBTAIN_CMD:
		{
			MSG_CG_COLLECT_OBTAIN_CMD* pRecvMsg = (MSG_CG_COLLECT_OBTAIN_CMD*)pMsg;
			ItemManager::Instance()->InsertInventoryTotalInfo(PICK, pRecvMsg->m_tagInventory);

#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
            // 채집한 아이템이 모두 모였는지
            QCODE quest_code = pRecvMsg->quest_code_;
            SLOTCODE item_code = 0;
            uiQuestMan *quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
            if (quest_manager != NULL)
            {
                if (pRecvMsg->m_tagInventory.m_InvenCount != 1)
                {
                    // 채집해서 들어온게 1개가 아니다??
                    assert(!"pRecvMsg->m_tagInventory.m_InvenCount != 1");
                }
                if (pRecvMsg->m_tagInventory.m_InvenCount > 0)
                {
                    ITEMSLOTEX *slots = pRecvMsg->m_tagInventory.m_Slot;

                    SCItemSlot item_slot;
                    item_slot.Copy(slots[0].m_Stream);

                    item_code = item_slot.GetItemCode();
                }
            }
            if ((item_code != 0) && (quest_code != 0))
            {
                if (quest_manager->ConditionItemCompleteCheck(quest_code, item_code))
                {
                    quest_manager->SEND_CG_COLLECT_ALLCOLLECTIVE_SYN();
                }
            }
#endif //_NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
		}
		break;

	case CG_COLLECT_ACTIVE_BRD:
		{
			MSG_CG_COLLECT_ACTIVE_BRD* pRecvMsg = (MSG_CG_COLLECT_ACTIVE_BRD*)pMsg;

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_ACTIVE_BRD"));
#endif 

			Object* pObj = g_ObjectManager.GetObject(pRecvMsg->m_dwObjectKey.Get());
			if(pObj && pObj->IsKindOfObject(MAP_OBJECT))
			{
				MapObject* pMapObj	= (MapObject*)pObj;
                pMapObj->SetCollectionID(pRecvMsg->m_CollectionID);
            }
        }
        break;
#ifdef _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    case CG_COLLECT_DEACTIVE_BRD:
        {
            MSG_CG_COLLECT_LEAVE_SECTOR_CMD* pRecvMsg = (MSG_CG_COLLECT_LEAVE_SECTOR_CMD*)pMsg;

#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_LEAVE_SECTOR_CMD"));
#endif 

            for(int i = 0; i < pRecvMsg->m_byCount; i++)
            {
                Object* pObj = g_ObjectManager.GetObject(pRecvMsg->m_ObjKeyCollection[i].Get());
                if(NULL ==pObj || FALSE == pObj->IsKindOfObject(MAP_OBJECT))
                {
                    assert(!"MSG_CG_COLLECT_LEAVE_SECTOR_CMD::if(NULL ==pObj && FALSE == pObj->IsKindOfObject(MAP_OBJECT))");
                    break;
                }

                MapObject* pMapObj = (MapObject*)pObj;
                pMapObj->SetCollectionID(0);
            }
        }
        break;
#endif //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void BattleScene::OutputCollectionResultMsg(DWORD ResultCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	switch(ResultCode)
	{
	case RC_COLLECT_SUCCESS:
		g_InterfaceManager.GetInterfaceString(6832, szMessage);
		break;
	case RC_COLLECT_FAIL:								// 채집할 수 없습니다.
		g_InterfaceManager.GetInterfaceString(6833, szMessage);
		break;
	case RC_COLLECT_ALREADY_COLLECTED:					// 해당 채집물은 이미 채집되었습니다.
		g_InterfaceManager.GetInterfaceString(6834, szMessage);
		break;
	case RC_COLLECT_NOTEXIST:							// 채집물이 존재 하지 않는다.
		g_InterfaceManager.GetInterfaceString(6835, szMessage);
		break;
	case RC_COLLECT_NOTCOLLECTING:						// 채집물의 상태가 채집중이 아니다. 취소할수 없다.
		g_InterfaceManager.GetInterfaceString(6836, szMessage);
		break;
	case RC_COLLECT_FAIL_CANCEL:							// 취소에 실패했다.
		g_InterfaceManager.GetInterfaceString(6837, szMessage);
		break;
	case RC_COLLECT_FAIL_QCONDISION:						// 퀘스트 조건이 맞지 않는다.
		g_InterfaceManager.GetInterfaceString(6838, szMessage);
		break;
	case RC_COLLECT_FAIL_ACCONDISION:					// AC조건이 맞지 않는다.
		g_InterfaceManager.GetInterfaceString(6839, szMessage);
		break;
	case RC_COLLECT_FAIL_COUNTCONDISION:					// 같은 종류의 채집물을 더이상 채집할수 없다.
		g_InterfaceManager.GetInterfaceString(6840, szMessage);
		break;
	case RC_COLLECT_POSITION_INVALID:					// 플레이어가 존재할수 없는 위치에 있다.	
		g_InterfaceManager.GetInterfaceString(6841, szMessage);
		break;
	case RC_COLLECT_DISTANCE_INVALID:				// 최대 채집 가능 거리를 벗어났다.
		g_InterfaceManager.GetInterfaceString(6842, szMessage);
		break;
	case RC_COLLECT_ALREADY_COLLECTING:					// 플레이어는 채집중에 있다.
		g_InterfaceManager.GetInterfaceString(6843, szMessage);
		break;
	case RC_COLLECT_FAIL_DB_ADD:							// DB에 채집물품 저장 실패
		g_InterfaceManager.GetInterfaceString(6844, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_EXIT_CHAR:              // 70644 캐릭터를 찾을 수 없습니다.
		g_InterfaceManager.GetInterfaceString(70644, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_DEAD_CHAR:              // 70645 캐릭터가 사망하였습니다.
		g_InterfaceManager.GetInterfaceString(70645, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY:    // 70646 채집물이 잘못 되었습니다.
		g_InterfaceManager.GetInterfaceString(70646, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_POS:                    // 70647 보상을 받을 수 있는 거리가 아닙니다. 조금 더 다가가세요.
		g_InterfaceManager.GetInterfaceString(70647, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_DISTANCE:               // 70648 채집물과의 거리가 너무 멉니다.
		g_InterfaceManager.GetInterfaceString(70648, szMessage);
		break;
	case RC_COLLECT_FAIL_REWORD_ITEM:                   // 70649 보상 받기에 실패하였습니다.
		g_InterfaceManager.GetInterfaceString(70649, szMessage);
		break;
	case RC_COLLECT_FAIL_STAMP_DEAD_CHAR:
		g_InterfaceManager.GetInterfaceString(3342, szMessage); //각인에 실패하였습니다.
		break;
	case RC_COLLECT_CHAR_NOTCOLLECTING:
    case RC_COLLECT_FAIL_DB_DEL:
        // 70640 채집물이 존재하지 않습니다.
        g_InterfaceManager.GetInterfaceString(70640, szMessage);
        break;
    case RC_COLLECT_FAIL_OBJECTID_CONDISION:
        // 70641 해당 지역은 존재하지 않는 필드이거나 채집물의 ID가 잘못되었습니다.
        g_InterfaceManager.GetInterfaceString(70641, szMessage);
        break;
    case RC_COLLECT_FAIL_STAMP_CONDISION:
        // 70642 길드마스터가 아니면 각인할 수 없습니다.
        g_InterfaceManager.GetInterfaceString(70642, szMessage);
        break;
    case RC_COLLECT_FAIL_HAVENOTSPACE:
        // 70643 인벤토리가 부족하여 채집할 수 없습니다.
        g_InterfaceManager.GetInterfaceString(70643, szMessage);
        break;
	default:
		break;
	}

	if(0 != strlen(szMessage))
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
}


void BattleScene::OutputSummonResultMsg(DWORD ResultCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	switch(ResultCode)
	{
	case RC_SUMMON_SUCCESS:
		break;
	case RC_SUMMON_INVALID_COMMAND:				//< 존재하지않은 타입의 명령이다.
		break;
	case RC_SUMMON_SUMMONED_NPC_NOTEXIST:		//< 소환체가 존재하지 않는다.
		break;
	case RC_SUMMON_SAME_COMMAND:				//< 이미 내려진 명령이다.
		break;
	case RC_SUMMON_CANNOT_ATTACK_PLAYER:		//< 해당 플레이어를 공격할 수 없다.
		break;
	case RC_SUMMON_INVALID_PLAYER:			    //< 유저를 찾을수 없다.(시스템에러)
		break;
	case RC_SUMMON_INVALID_FIELD:			    //< 유저가 필드에 존재하지 않는다.(시스템에러)
		break;
	case RC_SUMMON_INVALID_DOMINATION_FIELD:     //3341 워프수정을 소환할 수 없는 지역입니다.
		g_InterfaceManager.GetInterfaceString(3341, szMessage);
		break;
	case RC_SUMMON_INVALID_CRYSTALWARP:          //< 워프 소환체가 존재하지 않는다.
		break;
	case RC_SUMMON_CANNOT_WARP_CMD:              //3321	이동할 수 없습니다. 수정 워프를 소환하시기 바랍니다.
		g_InterfaceManager.GetInterfaceString(3321, szMessage);
		break;
	case RC_SUMMON_FAIL_WARP_CMD:                //< 워프 명령 수행중 실패했다.
		break;
	case RC_SUMMON_INVALID_GUILD:                //< 크리스탈워프 소환길드가 존재 하지 않습니다.
		break;
	case RC_SUMMON_FAIL_CREATE_SKILL:
		g_InterfaceManager.GetInterfaceString(3353, szMessage);		//3353 워프수정 소환에 실패하였습니다.
		break;
	case RC_SUMMON_CRYSTALWARP_COUNTFULL:		 //3333	이미 모든 수정이 설치되어 있습니다.
		g_InterfaceManager.GetInterfaceString(3333, szMessage);
		break;
	}

	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
#ifdef _DEV_VER
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "SummonNak: %d", ResultCode);
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void BattleScene::OnRecvCG_ZONE_EX(MSG_BASE* pMsg)
{
	TCHAR		szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch(pMsg->m_byProtocol)
	{
		case CG_ZONE_EX_LOBBY_NAME_CHANGE_ACK:
			{
				MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_ACK* pRecvPacket = (MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_ACK*)pMsg;
				BattleLobby * pBAttleLobby = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
				pBAttleLobby->ChangeRoomTitle(pRecvPacket->m_pszRoomTitle);
			}
			break;

		case CG_ZONE_EX_LOBBY_NAME_CHANGE_NAK:
			{
				MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_NAK* pRecvPacket = (MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_NAK*)pMsg;
				DWORD dwErrorCode = pRecvPacket->m_dwErrorCode;
				g_InterfaceManager.GetStringZoneError(dwErrorCode, szMessage, INTERFACE_STRING_LENGTH-1);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
			}
			break;

		case CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD:
			{
				MSG_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD * pRecvMsg = (MSG_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD *)pMsg;

				const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(pRecvMsg->m_MapCode);
				if ( !pMapInfo )
				{				
					break;
				}

				if(g_pHero)
					g_pHero->GetCharInfo()->m_iRegion = pRecvMsg->m_MapCode;

			}
			break;

		case CG_ZONE_EX_ROOM_JOIN_ACK:
			{
				MSG_CG_ZONE_EX_ROOM_JOIN_ACK * pRecvMsg = (MSG_CG_ZONE_EX_ROOM_JOIN_ACK *) pMsg;
				
				const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(pRecvMsg->m_MapCode);
				if(NULL == pMapInfo)
				{
					assert(!"CG_ZONE_EX_ROOM_JOIN_ACK::(NULL == pMapInfo)");
					break;
				}

				SCENE_TYPE eSceneType;
				if (eZONETYPE_INSTANCE == pMapInfo->byMKind)
                {
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)pRecvMsg->m_byZONETYPE,
                        (eINSTANCE_DUNGEON_KIND)pMapInfo->byMType, pRecvMsg->m_MapCode);
#else
                    eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)pRecvMsg->m_byZONETYPE, (eINSTANCE_DUNGEON_KIND)pMapInfo->byMType);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                }
                else 
                {
                    eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)pRecvMsg->m_byZONETYPE);
                }


				if (eSceneType != SCENE_TYPE_NONE)
				{
					g_LoadScene.SetNextSceneAfterLoaded(eSceneType, pRecvMsg->m_MapCode, pRecvMsg->m_fcCODE );

                    EventInfoParser::Instance()->InitPlayedEvent();

					if( eSceneType == SCENE_TYPE_INSTANCE_DUNGEON )
					{
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
						g_InstanceDungeonScene.initInstanceDungeon( pRecvMsg->m_MapCode );
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
					}
                    else if (eSceneType == SCENE_TYPE_MISSION)
                    {
                        EventInfoParser::Instance()->InitPlayedEvent();
                        g_MissionScene.InitMissionTime();
                    }

					GameFramework::ChangeScene( SCENE_TYPE_LOAD );
				}
			}
			break;


		case CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK:
			{
				MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK* pRecvMsg = (MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK*)pMsg;

				TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

				if (pRecvMsg->m_Result == MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::SUCCESS)
				{
					assert(g_pHero);

					g_LoadScene.SetNextSceneAfterLoaded( SCENE_TYPE_INSTANCE_DUNGEON, pRecvMsg->RETINFO_SUCCESS.m_MapCode, 0 );
					BattleScene::SetLobbyFlag(FALSE);
					BattleScene::SetLobbyInPending(FALSE);

					EventInfoParser::Instance()->InitPlayedEvent();

					g_MissionScene.InitMissionTime();

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
					g_InstanceDungeonScene.initInstanceDungeon(pRecvMsg->RETINFO_SUCCESS.m_MapCode);
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

					GameFramework::ChangeScene( SCENE_TYPE_LOAD );
				}
				else if ( pRecvMsg->m_Result == MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::FAIL)
				{
					if( pRecvMsg->RETINFO_FAIL.m_byKind == MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::RET_ITEM )
					{
						ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->RETINFO_FAIL.m_dwErrorCode);
					}
					else if ( pRecvMsg->RETINFO_FAIL.m_byKind == MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::RET_ROOM )
					{
						if (g_InterfaceManager.GetStringZoneError(pRecvMsg->RETINFO_FAIL.m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
						{
							uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_VerifyLock(szMessage);
						}
					}
				}
			}
			break;

	case CG_ZONE_EX_SSQ_JOIN_INFO_ACK:
		{
			MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK* pRecvMsg = (MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK*)pMsg;

			if (pRecvMsg->IsSuccess())
			{
#ifdef _DH_BATTLEZONE2_
                //패킷 날릴 것 
                if (GENERALPARAM->IsNet() == FALSE)
                {
                    xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
                    g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SSQ, 12001, 61001);
                    GameFramework::ChangeScene(SCENE_TYPE_LOAD);
                }
                else
                {
                    //타락한 사원
                    if (g_pHero)
                    {
                        g_pHero->SendWingsControlByRider(false, false);
                        g_pHero->SendWingsControlByAccessory(false, false);
                    }
                    MSG_CG_ZONE_EX_SSQ_JOIN_SYN	send_packet;
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet)); 
                }
#else
                cProjectileBase* pProjectile = g_ProjectileManagerEx.CreatePortalForHero();
				g_dwSSQPortalKey = pProjectile->GetKey();

#endif//_DH_BATTLEZONE2_
			}
			else
			{
				switch (pRecvMsg->RETINFO_FAIL.m_Kind)
				{
				case MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK::RET_ITEM:
					{
						ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->RETINFO_FAIL.m_ErrorCode);
					}
					break;

				case MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK::RET_ROOM:
					{
						if (g_InterfaceManager.GetStringZoneError(pRecvMsg->RETINFO_FAIL.m_ErrorCode, szMessage, INTERFACE_STRING_LENGTH))
						{
							GlobalFunc::SysMessageBox(szMessage);
						}
					}
					break;
				}

			}
		}
		break;

	case CG_ZONE_EX_SSQ_JOIN_ACK:
		{
			MSG_CG_ZONE_EX_SSQ_JOIN_ACK* pRecvMsg = (MSG_CG_ZONE_EX_SSQ_JOIN_ACK*)pMsg;

			if (pRecvMsg->IsSuccess())
			{
				MAPCODE	Code = pRecvMsg->RETINFO_SUCCESS.m_CodeOfSSQ;

				BattleScene::SetLobbyFlag(FALSE);
				BattleScene::SetLobbyInPending(FALSE);

				g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SSQ, Code, 0);
				GameFramework::ChangeScene(SCENE_TYPE_LOAD);
			}
			
		}
		break;

	case CG_ZONE_EX_SSQ_ROOM_CTRL_CMD:
		{
			const MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD * pRecvPacket = static_cast<MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD*>(pMsg);

			WAR_MSG_HDR header = *(pRecvPacket->m_Header);
			WAR_TAG_P pIt = (WAR_TAG_P)(pRecvPacket->m_Header+1);

			OnRecvCG_SSQ_CTRL_CMD( pIt, header ); 
		}
		break;
	case CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD:
		{
			const MSG_CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD * pRecvPacket = static_cast<MSG_CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD*>(pMsg);
			
			SetSSQRewardInfo( pRecvPacket );

		}
		break;

	case CG_ZONE_EX_SSQ_WINNER_REWARD_CMD:
		{
			const MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD * pRecvPacket = static_cast<MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD*>( pMsg );

			if( g_pHero == NULL )
				break;

			INVENTORY_TOTAL_INFO Info = pRecvPacket->m_TotalInfo;
						
			ItemManager::Instance()->InsertInventoryTotalInfo( PICK, Info );
			
		}
		break;

		//인스턴스 포탈생성명령(미션포탈이나 퀘스트포탈)
	case CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD:
		{
			MSG_CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD* pRecvPacket = static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_CREATE_CMD*>( pMsg );

            g_ProjectileManagerEx.CreateQuestPortal( 
                pRecvPacket->questPortalInfo.portalPosition,
                pRecvPacket->questPortalInfo.requesterKey,
                pRecvPacket->questPortalInfo.portalLifetime,
                pRecvPacket->questPortalInfo.portalKey,
                pRecvPacket->questPortalInfo.enterableMapCode );



		}
		break;

		//인스턴스 포탈제거
	case CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD:
		{
			MSG_CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD * pRecvPacket = static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_CLOSE_CMD*>( pMsg );

			g_ProjectileManagerEx.DeleteProjectile( pRecvPacket->portalKey );
		}
		break;
		//포탈입장SYNC에대한 NAK처리..		
	case CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK:
		{
			MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK * pRecvPacket = static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK*>( pMsg );

			//TODO:포탈입장실패에대한 에러처리
			TCHAR   szMessage[INTERFACE_STRING_LENGTH];
			if (g_InterfaceManager.GetStringZoneError(pRecvPacket->errorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Verify(szMessage);

			}
		}
		break;
    case CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD:
        {
            MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD* cmd = static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD*>(pMsg);
            if (g_pHero != NULL)
            { 
                //CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD 
                g_pHero->FixedPortalState.portal_key = cmd->portal_key;
                g_pHero->FixedPortalState.open_state = cmd->open_state;
                g_pHero->FixedPortalState.portal_lifetime = cmd->portal_lifetime;
            }
        }
        break;
    case CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_NAK:
        {
            MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_NAK* nak = static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_NAK*>(pMsg);

            //TODO:포탈입장실패에대한 에러처리
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {NULL, };
            if (g_InterfaceManager.GetStringZoneError(nak->error_code, szMessage, INTERFACE_STRING_LENGTH ))
            {
                uiSystemMan* pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                if(pSystemMan)
                {
                    pSystemMan->Sys_Verify(szMessage);
                }
            }
        }
        break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case CG_ZONE_EX_SPA_ENTER_ACK:
        {
            MSG_CG_ZONE_EX_SPA_ENTER_ACK* packet = static_cast<MSG_CG_ZONE_EX_SPA_ENTER_ACK*>(pMsg);
            
            if (GENERALPARAM->IsNet())
            {
                if (packet->spa_mapcode != 0)
                {
                    // eZONETYPE_SPA_FIELD
                    g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SPA, packet->spa_mapcode, 0);
                    GameFramework::ChangeScene(SCENE_TYPE_LOAD);
                }
            }
        }
        break;
    case CG_ZONE_EX_SPA_ENTER_NAK:
        {
            MSG_CG_ZONE_EX_SPA_ENTER_NAK* packet = static_cast<MSG_CG_ZONE_EX_SPA_ENTER_NAK*>(pMsg);


            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            switch (packet->error_code)
            {
            case RC_SPA_USER_ALREADY_DOING_TRANSACTION:
                {
                    // 70531 이동할 수 없는 상태입니다.
                    g_InterfaceManager.GetInterfaceString(70531, string, INTERFACE_STRING_LENGTH-1);
                }
                break;
            case RC_SPA_INVALID_MAPCODE:
            case RC_SPA_INVALID_ZONE:
                {
                    // 70532 잘못된 맵 정보입니다.
                    g_InterfaceManager.GetInterfaceString(70532, string, INTERFACE_STRING_LENGTH-1);
                }
                break;
            default:
                {
                    Sprintf(string, _T("Spa Enter Fail - Not Found ErrorCode : %d"), packet->error_code);
                }
            }
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, string);

        }
        break;
    case CG_ZONE_EX_SPA_LEAVE_ACK:
        {
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_EX_SPA_LEAVE_ACK* resv_message = static_cast<MSG_CG_ZONE_EX_SPA_LEAVE_ACK*>(pMsg);

            if(g_pHero)
                g_pHero->GetCharInfo()->m_iRegion = resv_message->return_map_code;

            BackToTheVillage(resv_message->return_map_code);

            g_InterfaceManager.HideAllWindows();
            g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
        }
        break;
    case CG_ZONE_EX_SPA_LEAVE_NAK:
        {
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_EX_SPA_LEAVE_NAK* resv_message = static_cast<MSG_CG_ZONE_EX_SPA_LEAVE_NAK*>(pMsg);
            

            g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);


            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            switch (resv_message->error_code)
            {
            case RC_SPA_USER_ALREADY_DOING_TRANSACTION:
                {
                    // 85034 잘못된 플레이어 상태입니다.
                    g_InterfaceManager.GetInterfaceString(85034, string, INTERFACE_STRING_LENGTH-1);
                }
                break;
            case RC_SPA_INVALID_MAPCODE:
            case RC_SPA_INVALID_ZONE:
                {
                    // 70532 잘못된 맵 정보입니다.
                    g_InterfaceManager.GetInterfaceString(70532, string, INTERFACE_STRING_LENGTH-1);
                }
                break;
            default:
                {
                    Sprintf(string, _T("Spa Leave Fail - Not Found ErrorCode : %d"), resv_message->error_code);
                }
            }
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, string);
        }
        break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD:
        {
            MSG_CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD* resv_message =
                static_cast<MSG_CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD*>(pMsg);

            uiDominationTimer* domination_timer = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
            if (domination_timer)
            {
                if(SONNENSCHEIN_EVENT_STATE_END == resv_message->event_state_)
                {
                    if (TRUE == domination_timer->IsVisible())
                    {
                        domination_timer->ShowInterface(FALSE);
                    }
                    break;

#ifdef _NA_008334_20150608_SONNENSCHEIN
                    domination_timer->set_event_state(resv_message->event_state_);
#endif //_NA_008334_20150608_SONNENSCHEIN
                }

                if (FALSE == domination_timer->IsVisible())
                    domination_timer->ShowInterface(TRUE);

                domination_timer->SetTimer(resv_message->remain_time_, true);
                if(SONNENSCHEIN_EVENT_STATE_WAIT == resv_message->event_state_)
                {
                    domination_timer->SetStartTimer(false);
                }
                else if(SONNENSCHEIN_EVENT_STATE_START == resv_message->event_state_)
                {
                    domination_timer->SetStartTimer(true);
                }

#ifdef _NA_008334_20150608_SONNENSCHEIN
                domination_timer->set_event_state(resv_message->event_state_);
#endif //_NA_008334_20150608_SONNENSCHEIN
            }
        }
        break;
#endif //_NA_008334_20150608_SONNENSCHEIN

	}
}

#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
void BattleScene::OnRecvCG_EVENT( MSG_BASE* message )
{
	switch ( message->m_byProtocol )
	{
	case CG_EVENT_SELECT_ACK:
	case CG_EVENT_SELECT_NAK:
	case CG_EVENT_MOVE_ITEM_TO_INVEN_ACK:
	case CG_EVENT_MOVE_ITEM_TO_INVEN_NAK:
	case CG_EVENT_QUERY_EXIST_ACK:
		{
			OnRecvCG_EVENT_INVENTORY( message );
		}
		break;
	case CG_EVENT_PEAKTIME_NOTIFY_BRD:
		{
			OnRecvCG_EVENT_PEAKTIME_NOTIFY( message );
		}
		break;
	case CG_GMEVENT_PEAKTIME_EVENTLIST_CMD:
		{
			OnRecvCG_GMEVENT_PEAKTIME_EVENTLIST_CMD( message );
		}
		break;
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	case CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD:
		{
			OnRecvCG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD( message );
		}
		break;
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	}
}
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT

//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetSSQRewardInfo(const MSG_CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD* pRecvPacket)
{
	switch (pRecvPacket->m_RewardInfo.m_SharedInfo.EventID)
	{
	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
		{
			SSQ_REWARD_INFO Info(pRecvPacket->m_RewardInfo.m_WinnerReward.Point,
									pRecvPacket->m_RewardInfo.m_WinnerReward.Exp,
									pRecvPacket->m_RewardInfo.m_WinnerReward.MonsterKillNum,
									pRecvPacket->m_RewardInfo.m_WinnerReward.PlayerKillNum);

			uiSSQRewardWindow* pDlg = GET_CAST_DIALOG(uiSSQRewardWindow, IM_SSQ_MANAGER::SSQ_REWARD_WINDOW);

			if (pDlg)
			{
				pDlg->SetType(REWARD_WIN);
				pDlg->SetRewardInfo(Info);
				pDlg->ShowInterface(TRUE);
			}

			
		}
		break;

	case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
		{
			SSQ_REWARD_INFO Info(pRecvPacket->m_RewardInfo.m_QuestFail.Point,
									pRecvPacket->m_RewardInfo.m_QuestFail.Exp,
									pRecvPacket->m_RewardInfo.m_QuestFail.MonsterKillNum,
									pRecvPacket->m_RewardInfo.m_QuestFail.PlayerKillNum);

			uiSSQRewardWindow* pDlg = GET_CAST_DIALOG(uiSSQRewardWindow, IM_SSQ_MANAGER::SSQ_REWARD_WINDOW);

			if (pDlg)
			{
				pDlg->SetType(REWARD_FAIL);
				pDlg->SetRewardInfo(Info);
				pDlg->ShowInterface(TRUE);
			}
		}
		break;

	case eEVENT_MULTIUSE_SSQ_REFUND:
		{
			if (g_pHero == NULL)
            {
				break;
            }

			MONEY money = pRecvPacket->m_Refund.ChangedTotalHeim;

			g_pHero->SetMoney(money);

			INVENTORY_TOTAL_INFO Info = pRecvPacket->m_Refund.TotalInfo;

			ItemManager::Instance()->InsertInventoryTotalInfo(PICK, Info);
		}
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_ZONE( MSG_BASE * pMsg )
{
	TCHAR		szMessage[INTERFACE_STRING_LENGTH] = {0,};
	uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
	if (!pZoneMan)
	{
		assert(pZoneMan);
		return;
	}

	switch(pMsg->m_byProtocol)
	{
	case CG_ZONE_CREATE_TOP_OF_TRIAL_NAK:
			{
				MSG_CG_ZONE_CREATE_TOP_OF_TRIAL_NAK* pRecvMsg = (MSG_CG_ZONE_CREATE_TOP_OF_TRIAL_NAK *)pMsg;

				if(pRecvMsg->m_byKind)
				{
					if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
					{
						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);
					}
				}
				else
					ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			}
			break;

	case CG_ZONE_JOIN_TOP_OF_TRIAL_ACK:
		{
			assert(g_pHero);

#ifdef _DH_BATTLEZONE2_
            //배틀존 로비 캐릭터 오브젝트 저장
            uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
            if (ui_battlezone2_manager_)
            {  
                ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);
            }
#endif//_DH_BATTLEZONE2_

			MSG_CG_ZONE_JOIN_TOP_OF_TRIAL_ACK* pRecvMsg = (MSG_CG_ZONE_JOIN_TOP_OF_TRIAL_ACK*)pMsg;

			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_INSTANCE_DUNGEON, pRecvMsg->m_MapCode, 0 );
            g_LoadScene.ChangeImage(TRUE, IMG_LOADING_KALBUS);

			BattleScene::SetLobbyFlag(FALSE);
			BattleScene::SetLobbyInPending(FALSE);

			
			EventInfoParser::Instance()->InitPlayedEvent();

			g_MissionScene.InitMissionTime();

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
			g_InstanceDungeonScene.initInstanceDungeon(pRecvMsg->m_MapCode);
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
		}
		break;

	case CG_ZONE_JOIN_TOP_OF_TRIAL_NAK:
		{
			//MSG_CG_ZONE_JOIN_TOP_OF_TRIAL_NAK* pRecvMsg = (MSG_CG_ZONE_JOIN_TOP_OF_TRIAL_NAK* )pMsg;

		}
		break;


		// 로비에 참가
	case CG_ZONE_LOBBY_JOIN_ACK:
		{
			assert(g_pHero);
			if(!g_pHero)
				return;

			// 개인 상점 열렸을 경우 닫아준다.
			if( g_pHero->GetBeHaveState() == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
			{
				// 강제로 서게 해준다. pVendorSell->Exit()에서 서기 패킷 날림.
				if( g_pHero->GetCurState() == STATE::SIT )
					g_pHero->SetCondition(eCHAR_CONDITION_STANDUP);
				
				uiVendorSell * pVendorSell = GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );
				if( pVendorSell )
				{
					pVendorSell->Exit();
				}
			}

			uiVendorBuy * pVendorBuy = GET_CAST_DIALOG( uiVendorBuy, IM_VENDOR_MANAGER::VENDOR_BUY );
			if( pVendorBuy && pVendorBuy->IsVisible())
			{
				pVendorBuy->ShowWindow(FALSE);
			}

			uiVendorList * pVendorList = GET_CAST_DIALOG( uiVendorList, IM_VENDOR_MANAGER::VENDOR_LIST );
			if( pVendorList && pVendorList->IsVisible() )
			{
				pVendorList->NET_SEND_CG_VENDOR_LIST_END_SYN();
			}

			uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
			if(pCommMan)
			{
				if(pCommMan->IsRqstJoinGame())
					g_InterfaceManager.HideAllWindows();

				pCommMan->SetRqstJoinGame(false);
			}


			MSG_CG_ZONE_LOBBY_JOIN_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_JOIN_ACK *)pMsg;

			// TODO : 여기서 저장해놓은 방정보를 받아온다. 
			if(pZoneMan)
			{
				pZoneMan->SetBattleRoomEntry( BattleScene::GetPendingBattleRoomEntry() );
				pZoneMan->SetLobbyMasterPlayerKey(pRecvMsg->m_dwMasterKey);

				UserSlot EnterUser;
				EnterUser.SetPlayerKey( pRecvMsg->m_dwMasterKey );
				pZoneMan->AddPlayer( EnterUser );
			}

			// 몬스터와 자신을 제외한 모든 유저를 지운다.
			g_ObjectManager.DeleteObjectsByType(MONSTER_OBJECT);
			assert(g_pHero);
			g_ObjectManager.DeletePlayersWithoutHero( g_pHero->GetObjectKey() );


			// 로비 플레그 세팅

			BattleScene::SetLobbyFlag(TRUE);

			BattleScene::SetLobbyInPending(TRUE);
			BattleScene::SendPlayerEnterSyn(FALSE);
		}
		break;

	case CG_ZONE_LOBBY_JOIN_NAK:
		{
			uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
			if(pCommMan)
			{
				pCommMan->SetRqstJoinGame(false);
			}

			MSG_CG_ZONE_LOBBY_JOIN_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_JOIN_NAK *)pMsg;
			g_pSunRenderer->ClearRender();

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);
			}

			// 로비 플레그 세팅
			SetLobbyInPending(FALSE);

			// ckTODO: 존 클리어 함수 만들기 (uiBattleZoneMan)
		}
		break;


	case CG_ZONE_MISSIONLOBBY_JOIN_CMD:
		{
			MSG_CG_ZONE_MISSIONLOBBY_JOIN_CMD * pRecvMsg = (MSG_CG_ZONE_MISSIONLOBBY_JOIN_CMD *)pMsg;

			BattleRoomEntry BattleRoom;
			BattleRoom.Init();

			BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
			BattleRoom.SetType(eZONETYPE_MISSION);
			BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);		//< eZONEPUBLIC
			BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
			BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
			if (eZONEPUBLIC_PRIVATE == BattleRoom.GetPublic())
			{
				BattleRoom.SetPassword(pRecvMsg->m_pszRoomPWD);
			}

			BattleRoom.SetMaxPlayer(pRecvMsg->m_RoomInfo.m_MaxLimitUserNum);
			BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);

			BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

			// 5216	파티장이 배틀존을 개설하였습니다.\n참여하시겠습니까?\n참여하지 않을 경우 파티에서 탈퇴됩니다.
			g_InterfaceManager.GetInterfaceString(5216, szMessage, INTERFACE_STRING_LENGTH);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_Confirm(szMessage, _callback_FollowPartyMaster);
		}
		break;


	case CG_ZONE_PVPLOBBY_JOIN_CMD:
		{
			MSG_CG_ZONE_PVPLOBBY_JOIN_CMD * pRecvMsg = (MSG_CG_ZONE_PVPLOBBY_JOIN_CMD *)pMsg;

			BattleRoomEntry BattleRoom;
			BattleRoom.Init();

			BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
			BattleRoom.SetType(eZONETYPE_PVP);
			BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);		//< eZONEPUBLIC
			BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
			BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
			if (eZONEPUBLIC_PRIVATE == BattleRoom.GetPublic())
			{
				BattleRoom.SetPassword(pRecvMsg->m_pszRoomPWD);
			}
			BattleRoom.SetMaxPlayer(pRecvMsg->m_RoomInfo.m_MaxLimitUserNum);
			BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);
			BattleRoom.SetAdditionalPVPInfo(&pRecvMsg->m_RoomInfo.m_AdditionalPVPInfo);

			BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

			// 5216	파티장이 배틀존을 개설하였습니다.\n참여하시겠습니까?\n참여하지 않을 경우 파티에서 탈퇴됩니다.
			g_InterfaceManager.GetInterfaceString(5216, szMessage, INTERFACE_STRING_LENGTH);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_Confirm(szMessage, _callback_FollowPartyMaster);

		}
		break;

	case CG_ZONE_CHUNTINGLOBBY_JOIN_CMD:
		{
			MSG_CG_ZONE_CHUNTINGLOBBY_JOIN_CMD * pRecvMsg =
				(MSG_CG_ZONE_CHUNTINGLOBBY_JOIN_CMD *)pMsg;

			BattleRoomEntry BattleRoom;
			BattleRoom.Init();
			BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
			BattleRoom.SetType(eZONETYPE_CHUNTING);
			BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);
			BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
			BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
			if (eZONEPUBLIC_PRIVATE == BattleRoom.GetPublic())
			{
				BattleRoom.SetPassword(pRecvMsg->m_pszRoomPWD);
			}

			BattleRoom.SetMaxPlayer(pRecvMsg->m_RoomInfo.m_MaxLimitUserNum);		
			BattleRoom.SetMinLevel((BYTE)BattleRoom.GetCurMapInfo()->minLV);
			BattleRoom.SetMaxLevel((BYTE)BattleRoom.GetCurMapInfo()->maxLV);
			BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);
			BattleRoom.SetAdditionalCHuntingInfo(&pRecvMsg->m_RoomInfo.m_AdditionalInfo);

			BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

			// 5216	파티장이 배틀존을 개설하였습니다.\n참여하시겠습니까?\n참여하지 않을 경우 파티에서 탈퇴됩니다.
			g_InterfaceManager.GetInterfaceString(5216, szMessage, INTERFACE_STRING_LENGTH);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_Confirm(szMessage, _callback_FollowPartyMaster);

		}
		break;

	case CG_ZONE_MISSION_LEAVE_ACK:
		{
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            ItemShareSystem* item_share_system = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
            if(item_share_system) 
            {
                item_share_system->SetPartyObtainRoom(false);
            }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_MISSION_LEAVE_ACK * pRecvMsg = (MSG_CG_ZONE_MISSION_LEAVE_ACK *)pMsg;
			// 여기서 미션 나가는 처리
			BackToTheVillage();
			g_InterfaceManager.HideAllWindows();
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
#ifdef _DH_BATTLEZONE2_
            uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
            if (ui_battlezone2_manager_)
            {
                if (g_pHero->IsParty())
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
                }
                else
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LIST);
                }
            }
#endif//_DH_BATTLEZONE2_
		}
		break;

	case CG_ZONE_MISSION_LEAVE_NAK:
		{
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_MISSION_LEAVE_NAK * pRecvMsg = (MSG_CG_ZONE_MISSION_LEAVE_NAK *)pMsg;
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);

			}
		}
		break;


	case CG_ZONE_INSTANCE_DUNGEON_LEAVE_ACK:
		{
#ifdef _DH_BATTLEZONE2_
            uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
            if (ui_battlezone2_manager_)
            {
                if (g_pHero->IsParty())
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
                }
                else
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LIST);
                }
            }
#endif//_DH_BATTLEZONE2_
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            ItemShareSystem* item_share_system = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
            if(item_share_system) 
            {
                item_share_system->SetPartyObtainRoom(false);
            }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

			this->m_bPendingBackToSomeWhere = false;

			BackToTheVillage();
			g_InterfaceManager.HideAllWindows();
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
		}
		break;

	case CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK:
		{
			this->m_bPendingBackToSomeWhere = false;

			MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK * pRecvMsg = (MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK *)pMsg;
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);
			}
		}
		break;

	case CG_ZONE_RETURN_VILLAGE_ACK:
		{
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_RETURN_VILLAGE_ACK * pRecvMsg = (MSG_CG_ZONE_RETURN_VILLAGE_ACK *)pMsg;

			if(g_pHero)
				g_pHero->GetCharInfo()->m_iRegion = pRecvMsg->m_wMapCode;

			BackToTheVillage(pRecvMsg->m_wMapCode);
			g_InterfaceManager.HideAllWindows();
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
		}
		break;

    case CG_ZONE_RETURN_VILLAGE_NAK:
		{
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_RETURN_VILLAGE_NAK * pRecvMsg = (MSG_CG_ZONE_RETURN_VILLAGE_NAK *)pMsg;

			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_byErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);

			}
		}
		break;

	case CG_ZONE_PVP_LEAVE_ACK:
		{
#ifdef _DH_BATTLEZONE2_
            uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
            if (ui_battlezone2_manager_)
            {
                if (g_pHero->IsParty())
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
                }
                else
                {
                    ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LIST);
                }
            }
#endif//_DH_BATTLEZONE2_
            this->m_bPendingBackToSomeWhere = false;

			//MSG_CG_ZONE_PVP_LEAVE_ACK * pRecvMsg = (MSG_CG_ZONE_PVP_LEAVE_ACK *)pMsg;
			
			// 여기서 PVP 나가는 처리

			BackToTheVillage();
			g_InterfaceManager.HideAllWindows();
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);	
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_PVP_SCORE, pMsg);

		}
		break;

	case CG_ZONE_PVP_LEAVE_NAK:
		{
            this->m_bPendingBackToSomeWhere = false;

			MSG_CG_ZONE_PVP_LEAVE_NAK * pRecvMsg = (MSG_CG_ZONE_PVP_LEAVE_NAK *)pMsg;
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_PVP_SCORE, pMsg);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);

			}
		}
		break;

    case CG_ZONE_CHUNTING_LEAVE_ACK:
        {
            this->m_bPendingBackToSomeWhere = false;

            BackToTheVillage();
            g_InterfaceManager.HideAllWindows();
            g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);	
        }
        break;

    case CG_ZONE_CHUNTING_LEAVE_NAK:
        {
            this->m_bPendingBackToSomeWhere = false;

            MSG_CG_ZONE_CHUNTING_LEAVE_NAK * pRecvMsg = (MSG_CG_ZONE_CHUNTING_LEAVE_NAK *)pMsg;
            g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);

			}
        }
        break;

	case CG_ZONE_GMPORTAL_FIELD_MOVE_CMD:
		{
			MSG_CG_ZONE_GMPORTAL_FIELD_MOVE_CMD * pRecvMsg = (MSG_CG_ZONE_GMPORTAL_FIELD_MOVE_CMD *)pMsg;

            if(g_pHero)
            {
                g_pHero->GetCharInfo()->m_iRegion = pRecvMsg->m_MapCode;
            }

            g_LoadScene.SetNextSceneAfterLoaded( (MAPCODE)pRecvMsg->m_MapCode, 0 );
			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
		}
		break;

	case CG_ZONE_MISSION_RELAY_CREATE_BRD:
		{
			uiPartyMan* partyMan =
				static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

			m_CurLobbyKey = 0;
			MSG_CG_ZONE_MISSION_RELAY_CREATE_BRD* pRecvMsg = (MSG_CG_ZONE_MISSION_RELAY_CREATE_BRD*) pMsg;
			m_CurLobbyKey = pRecvMsg->m_LobbyKey;


            MissionRelay* pMissionRelay = (MissionRelay*) g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_MISSION_RELAY );
			if (pMissionRelay && m_CurLobbyKey)
			{
				pMissionRelay->ShowInterface(TRUE);
				//파티리더와관계없다
		
				pMissionRelay->SetForceMember(TRUE);
			}
		}
		break;

	case CG_ZONE_MISSION_RELAY_CREATE_NAK:
		{
			MSG_CG_ZONE_MISSION_RELAY_CREATE_NAK* pRecvMsg = (MSG_CG_ZONE_MISSION_RELAY_CREATE_NAK*) pMsg;
			g_InterfaceManager.GetStringZoneError(pRecvMsg->m_ErrorCode, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	//CG_ZONE_MISSION_RELAY_CREATE_SYNC(파티장),ZONE_MISSION_RELAY_JOIN_SYNC(파티원)
	case CG_ZONE_MISSION_RELAY_JOIN_ACK:
		{
			MSG_CG_ZONE_MISSION_RELAY_JOIN_ACK* pRecvMsg = (MSG_CG_ZONE_MISSION_RELAY_JOIN_ACK*) pMsg;
			KEYTYPE LobbyKey  = pRecvMsg->m_LobbyKey;
			DWORD   MasterKey = pRecvMsg->m_dwMasterKey;

			assert(g_pHero);
			if (!g_pHero)
            {
				return;
            }

			uiBattleScoreMan* battle_score_manager_ptr 
				= static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			if (battle_score_manager_ptr)
			{
				battle_score_manager_ptr->Reset();
			}

			uiBattleZoneMan* pZoneMan = 
				static_cast<uiBattleZoneMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));

			uiPartyMan* partyMan =
				static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			
			BattleRoomEntry* pRoomEntry = BattleScene::GetPendingBattleRoomEntry();
			pZoneMan->SetNonBlockSlot(pRoomEntry->GetMaxPlayer());

            uiMissionObject* dialog = GET_CAST_DIALOG(uiMissionObject, IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT);
            if (dialog)
            {
                dialog->SetMissionObjectMapCode(0, 0);// 없는 맵으로 초기화
            }
			if (partyMan && (TRUE == partyMan->IsLeader()))
			{
				pZoneMan->UnsetPKBool(PACKET_STATUS_SEND_CREATE);

				
				assert(pRoomEntry);

				// roomkey toss to dialog (battle lobby)
				pRoomEntry->SetSerial(LobbyKey);

				pZoneMan->SetBattleRoomEntry(pRoomEntry);
				pZoneMan->SetLobbyMasterPlayerKey(g_pHero->GetObjectKey());

				UserSlot EnterUser;
				EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
				pZoneMan->AddPlayer(EnterUser);

				g_ObjectManager.DeleteObjectsByType(MONSTER_OBJECT);
				g_ObjectManager.DeletePlayersWithoutHero(g_pHero->GetObjectKey());

				BattleScene::SetLobbyFlag(TRUE);
				BattleScene::SetLobbyInPending(TRUE);
				BattleScene::SendPlayerEnterSyn();
			}
			else
			{
				uiCommunityMan* pCommMan = 
					static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

				if (pCommMan)
				{
					if (pCommMan->IsRqstJoinGame())
                    {
						g_InterfaceManager.HideAllWindows();
                    }

					pCommMan->SetRqstJoinGame(false);
				}

				if (pZoneMan)
				{
					pZoneMan->SetBattleRoomEntry(BattleScene::GetPendingBattleRoomEntry());
					pZoneMan->SetLobbyMasterPlayerKey(MasterKey);

					UserSlot EnterUser;
					EnterUser.SetPlayerKey(MasterKey);
					pZoneMan->AddPlayer(EnterUser);
				}

				// 몬스터와 자신을 제외한 모든 유저를 지운다.
				g_ObjectManager.DeleteObjectsByType(MONSTER_OBJECT);
				assert(g_pHero);
				g_ObjectManager.DeletePlayersWithoutHero(g_pHero->GetObjectKey());


				// 로비 플레그 세팅
				BattleScene::SetLobbyFlag(TRUE);
				BattleScene::SetLobbyInPending(TRUE);
				BattleScene::SendPlayerEnterSyn(FALSE);
			}
		}
		break;
    }
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_CHARINFO( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	//	2007.12.24 / lammy / 스킬 포인트 초기화시 quick slot에 있는 스킬 업데이트를 위해...
	case CG_CHARINFO_QUICK_CMD:
		{
			MSG_CG_CHARINFO_QUICK_CMD * pRecvMsg = (MSG_CG_CHARINFO_QUICK_CMD *)pMsg;
            xGetHeroData()->SetQuickTotalInfo( &pRecvMsg->m_QuickInfo );
			if( g_pHero != NULL )
				g_pHero->SetQuickTotalInfo( pRecvMsg->m_QuickInfo );
		}
		break;

	case CG_CHARINFO_APPREARANCE_BRD:
		{
			MSG_CG_CHARINFO_APPREARANCE_BRD * pRecvMsg = (MSG_CG_CHARINFO_APPREARANCE_BRD *)pMsg;
			
			Player * pPlayer = (Player *) g_ObjectManager.GetObject( pRecvMsg->m_PlayerKey );
			if (pPlayer && pPlayer != g_pHero)
			{
				EquipmentContainer * pContainer = (EquipmentContainer *)pPlayer->GetSlotContainer(SI_EQUIPMENT);
				if (pContainer)
				{
					if ( pRecvMsg->m_EquipPos < pContainer->GetMaxSlotNum())
					{
						
						SCItemSlot ItemSlot( pRecvMsg->m_OptItemStream );
						pContainer->InsertSlot(pRecvMsg->m_EquipPos,ItemSlot);	
						
					}
					else
					{
						// 예외 처리!!
					}
				}


#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
                pPlayer->SetItemScore( pRecvMsg->m_TotalItemScore );
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE

			}
			else
			{
				// 예외 처리!!
			}
		}
		break;

	case CG_CHARINFO_DISAPPREARANCE_BRD:
		{
			MSG_CG_CHARINFO_DISAPPREARANCE_BRD * pRecvMsg = (MSG_CG_CHARINFO_DISAPPREARANCE_BRD *)pMsg;			

			Player * pPlayer = (Player *) g_ObjectManager.GetObject( pRecvMsg->m_PlayerKey );
			if (pPlayer && pPlayer != g_pHero)
			{
				SCSlotContainer * pContainer = pPlayer->GetSlotContainer(SI_EQUIPMENT);
				if (pContainer)
				{
					if ( pRecvMsg->m_EquipPos < pContainer->GetMaxSlotNum())
					{
						pContainer->DeleteSlot(pRecvMsg->m_EquipPos, NULL);		
					}
					else
					{
						// 예외 처리!!
					}
				}

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
                pPlayer->SetItemScore( pRecvMsg->m_TotalItemScore );
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE

			}
			else
			{
				// 예외 처리!!
			}
		}
		break;

    case CG_CHARINFO_APPREARANCE_LIST_BRD:
        {
            MSG_CG_CHARINFO_APPREARANCE_LIST_BRD* pRecvMsg = static_cast<MSG_CG_CHARINFO_APPREARANCE_LIST_BRD*>(pMsg);

            Player* pPlayer = static_cast<Player*>(g_ObjectManager.GetObject(pRecvMsg->player_key));
            if (pPlayer && pPlayer != g_pHero)
            {
                EquipmentContainer* pContainer = static_cast<EquipmentContainer*>(pPlayer->GetSlotContainer(SI_EQUIPMENT));
                if (pContainer)
                {
                    for (int i = 0; i < pRecvMsg->number_of_items; ++i)
                    {
                        if ( pRecvMsg->nodes[i].pos < pContainer->GetMaxSlotNum())
                        {
                            SCItemSlot ItemSlot(pRecvMsg->nodes[i].render_item);
                            pContainer->InsertSlot(pRecvMsg->nodes[i].pos ,ItemSlot);	
                        }
                        else
                        {
                            // 예외 처리!!
                        }
                    }                    
                }


#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
                pPlayer->SetItemScore( pRecvMsg->total_item_score_ );
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE

            }
            else
            {
                // 예외 처리!!
            }
        }
        break;

    case CG_CHARINFO_DISAPPREARANCE_LIST_BRD:
        {
            MSG_CG_CHARINFO_DISAPPREARANCE_LIST_BRD* pRecvMsg = static_cast<MSG_CG_CHARINFO_DISAPPREARANCE_LIST_BRD*>(pMsg);			

            Player* pPlayer = static_cast<Player*>(g_ObjectManager.GetObject(pRecvMsg->player_key));
            if (pPlayer && pPlayer != g_pHero)
            {
                SCSlotContainer * pContainer = pPlayer->GetSlotContainer(SI_EQUIPMENT);
                if (pContainer)
                {
                    for (int i = 0; i < pRecvMsg->number_of_items; ++i)
                    {
                        if (pRecvMsg->nodes[i].pos < pContainer->GetMaxSlotNum())
                        {
                            pContainer->DeleteSlot(pRecvMsg->nodes[i].pos, NULL);
                        }
                    }
                }


#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
                pPlayer->SetItemScore( pRecvMsg->total_item_score_ );
#endif //_NA_000000_20130206_BROADCAST_ITEM_SCORE

            }
            else
            {
                // 예외 처리!!
            }
        }
        break;

	case CG_CHARINFO_SELECT_INFO_ACK:
		{
			MSG_CG_CHARINFO_SELECT_INFO_ACK *pRecvMsg = (MSG_CG_CHARINFO_SELECT_INFO_ACK *)pMsg;

			Character *pChr = (Character *)g_ObjectManager.GetObject(pRecvMsg->m_dwObjectKey);

			if (pChr) 
			{
				HPINFO info;

                info.m_HP       = pRecvMsg->m_HP;
                info.m_MaxHP    = pRecvMsg->m_MaxHP;
				info.m_LV       = pRecvMsg->m_LV;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                info.m_SD       = pRecvMsg->shield;
                info.m_MaxSD    = pRecvMsg->max_shield;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO


				pChr->SetHPInfo(&info);                
			}		
		}
		break;

	case CG_CHARINFO_USERPOINT_CMD:
		{
			MSG_CG_CHARINFO_USERPOINT_CMD * pRecvMsg = (MSG_CG_CHARINFO_USERPOINT_CMD *) pMsg;

			if( g_pHero)
			{
				g_pHero->GetCharInfo()->m_UserPoint = pRecvMsg->m_TotalUP;
				TCHAR szMessage[INTERFACE_STRING_LENGTH];
				TCHAR szTemp[INTERFACE_STRING_LENGTH];
				// 총 %dUP를 가지고 있습니다. : 5328
				g_InterfaceManager.GetInterfaceString(eST_TOTAL_HAVE_UP , szTemp , INTERFACE_STRING_LENGTH);
				Sprintf( szMessage , szTemp , pRecvMsg->m_TotalUP );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );

			}
		}
		break;
    case CG_CHARINFO_CHANGE_APPEARANCE_BRD:
        {
            ItemManager::Instance()->SerialCharCustomize(pMsg);
        }
        break;
    case CG_CHARNAME_CHANGE_REQUEST_ACK:
        {
            MSG_CG_CHARNAME_CHANGE_REQUEST_ACK * pRecvMsg = (MSG_CG_CHARNAME_CHANGE_REQUEST_ACK *) pMsg;
            if (pRecvMsg->result == RC_CHANGE_CHARACTER_NAME_SUCCESS)
            {
                uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                if (ui_system_manager)
                {
                    TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                    // 158554 정상적으로 %s 캐릭터에게 캐릭터명 변경권이 사용되었습니다. 게임 종료 후, 재접속하시면 캐릭터 선택창에서 캐릭터명 변경을 진행하실 수 있습니다.
                    g_InterfaceManager.GetInterfaceStringFormat(message,
                        INTERFACE_STRING_LENGTH,
                        158554,
                        g_pHero->GetName());
                    DWORD dialog_key = ui_system_manager->Sys_SystemMessage(message,
                        NULL,
                        GlobalFunc::_fn_CharacterNameChangeAck, 
                        uiSystemMessage::kVerify_And_Dont_Close_Type,
                        true,
                        0,
                        49); // 49 : 게임 종료
                    g_KeyQueueManager.SetDefaultFun(NULL);
                }
            }
            else
            {
                int string_code = 158641;
                switch (pRecvMsg->result)
                {
                case RC_CHANGE_CHARACTER_NAME_ALREADY_USED:
                    {
                        // 158640
                        string_code = 158640;
                    }
                    break;
                case RC_CHANGE_CHARACTER_NAME_DELAY_WAIT:
                    {
                        // 158642
                        string_code = 158642;
                    }
                    break;
                default:
                    {
                        // 158641
                        string_code = 158641;
                    }
                    break;
                }
                TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                
                g_InterfaceManager.GetInterfaceString(string_code, message);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            }
        }
        break;
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    case CG_CHARINFO_SMARTNPC_PACKET:
        {
            MSG_CG_CHARINFO_SMARTNPC_PACKET* pRecvMsg = (MSG_CG_CHARINFO_SMARTNPC_PACKET*) pMsg;
            switch (pRecvMsg->packet_type)
            {
            case NPCDIALOG_SHOWMENU_CMD:
                {
                    MSG_CG_NPCDIALOG_SHOWMENU_CMD* pRecvMsg = (MSG_CG_NPCDIALOG_SHOWMENU_CMD*)pMsg;
                    uiSmartNpcDialog* smart_npc_dialog = 
                        uiSmartNpcDialogMan::Instance()->ParserSmartDialogPacket(SmartDialogType_Menu, pRecvMsg->data_);
                    if ((pRecvMsg->NID != 0) && smart_npc_dialog)
                    {
                        smart_npc_dialog->SetOwnerKey(pRecvMsg->NID);
                    }
                }
                break;
            case NPCDIALOG_SHOWMESSAGEBOX_CMD:
                {
                    MSG_CG_NPCDIALOG_SHOWMESSAGEBOX_CMD* pRecvMsg = (MSG_CG_NPCDIALOG_SHOWMESSAGEBOX_CMD*)pMsg;
                    uiSmartNpcDialog* smart_npc_dialog = 
                        uiSmartNpcDialogMan::Instance()->ParserSmartDialogPacket(SmartDialogType_Reply, pRecvMsg->data_);
                    if ((pRecvMsg->NID != 0) && smart_npc_dialog)
                    {
                        smart_npc_dialog->SetOwnerKey(pRecvMsg->NID);
                        for (int i = 0; i < 3; ++i)
                        {
                            smart_npc_dialog->SetControlEventID(uiSmartNpcDialogReply::kControl_Button_B001+i, pRecvMsg->EID, pRecvMsg->STEP);
                        }
                    }
                }
                break;
            }
        }
        break;
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
    default:
        {
            SceneBase::OnRecvCG_CHARINFO(pMsg);
        }
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_MAP( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	case CG_MAP_TELEPORT_BRD:
		{
			MSG_CG_MAP_TELEPORT_BRD* pRecvPacket = (MSG_CG_MAP_TELEPORT_BRD*)pMsg;
			Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);

			if (!pPlayer)
            {
				return;
            }

			WzVector vPos = pRecvPacket->vPos;

			if (pRecvPacket->m_bForcedCommand)//서버로 부터 초기화 될때 이다.
			{
				pPlayer->SetPosition(vPos);
				pPlayer->SetDirection(vPos, TRUE);
				pPlayer->StopMove();

                if( pPlayer->GetHP() > 0 )
                {
				    pPlayer->SetNextState(STATE::IDLE, g_CurTime);
                }

				WzVector vVisiblePos = vPos;
				vVisiblePos.z += 2.0;

				int iTileFocussing = -1;
				float fT = 0.0f;
				iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(vVisiblePos, &fT, -1.0f, 10.0f);

				if (iTileFocussing >= 0)
				{
					vVisiblePos.z -= fT;
				}

				if (!g_bObserverMode)
                {
					GlobalFunc::CreateEffectBeUpToOption(StrToWzID("k470"),vVisiblePos,pPlayer->GetObjectKey());
                }

				pPlayer->CommitChangeState(g_CurTime);	

			}
			else
			{
				if( pPlayer->GetClass() == eCHAR_ELEMENTALIST && pPlayer->IsMyHero() == FALSE /*&& pPlayer->IsGM() == FALSE*/ )
				{
					// 다른 엘리멘탈 리스트의 텔레포트는 액션으로 수행
					// 엘리멘탈의 텔레포트 (GM이 아닌경우)
					// 텔레포트 스킬을 사용하도록 액션을 추가해준다

					PLAYER_ACTION action;

					ZeroMemory(&action, sizeof(action));
					action.ActionID = ACTION_TELEPORT;
					action.TELEPORT.vDest = vPos;

                    pPlayer->DoAction(&action);	
                    pPlayer->CommitChangeState(g_CurTime);
				}
				else
				{
					// 나머지는 그냥 바로 워프시켜준다 (GM인 엘리멘탈이 텔레포트 스킬을 쓴 경우 식별이 안되므로 그냥 워프가 됨...)
					
					if( pPlayer->GetClass() == eCHAR_MYSTIC )
					{
						GlobalFunc::CreateEffectBeUpToOption(StrToWzID("KB17"),pPlayer->GetVisiblePos(),pPlayer->GetObjectKey());
					}
					else
					{
						GlobalFunc::CreateEffectBeUpToOption(StrToWzID("k470"),pPlayer->GetVisiblePos(),pPlayer->GetObjectKey());
					}

					pPlayer->SetDestWarp(vPos);//워프할 좌표를 세팅 한뒤 텔레포트 스테이트 종료시 빼자 
					pPlayer->SetPosition(vPos);
                    pPlayer->SetVisiblePos(vPos);

					pPlayer->SetDirection(vPos, TRUE);
					pPlayer->StopMove();

					//점프 중 워프 처리 
					if (pPlayer->GetCurState() == STATE::KEYBOARDJUMP)
					{
						pPlayer->CommitChangeState(g_CurTime);	
						if (!g_bObserverMode)
                        {
							if( pPlayer->GetClass() == eCHAR_MYSTIC )
							{
								GlobalFunc::CreateEffectBeUpToOption(StrToWzID("KB17"),pPlayer->GetVisiblePos(),pPlayer->GetObjectKey());
							}
							else
							{
								GlobalFunc::CreateEffectBeUpToOption(StrToWzID("k470"),pPlayer->GetVisiblePos(),pPlayer->GetObjectKey());
							}
                        }
					}
				}
			}
		}
		break;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    case CG_MAP_RECALL_NAK:
        {
            MSG_CG_MAP_RECALL_NAK* pRecvPacket = (MSG_CG_MAP_RECALL_NAK*)pMsg;
            if (pRecvPacket->error_code_ == MSG_CG_MAP_RECALL_NAK::RECALL_CANNOT_BETAKEY)
            {
                TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                // 70542 조건이 맞지 않아 해당 장비를 착용할 수 없습니다.
                g_InterfaceManager.GetInterfaceString(70542, message, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            }
        }
        break;
#endif


    case CG_MAP_KBMOVE_SERIAL_CHECK_CMD:
        {
            MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* cmd = static_cast<MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD*>(pMsg);
            MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK ack;
            if (KBMoveCheckManagerInClient::Instance()->ReceiveOrders(*cmd, &ack) == true)
            {
                GlobalFunc::SendPacketToGameServer(&ack, sizeof(ack));
            }
            else
            {
                WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "KBMoveCheckManagerInClient::Instance()->ReceiveOrders(*cmd, &ack) == false");
            }
        }
        break;
	}
}

//------------------------------------------------------------------------------
/**
*/
WORD GetPlayerSize(BYTE *pPacket,int nRenderInfoSize)
{     
	ETC_STATE_TOTAL_INFO *pShieldfo = (ETC_STATE_TOTAL_INFO *)( (BYTE*)(pPacket) + nRenderInfoSize );
	VENDOR_RENDER_INFO * pVendorInfo = (VENDOR_RENDER_INFO *)( (BYTE*)(pShieldfo) + pShieldfo->GetSize() );
	return nRenderInfoSize + pShieldfo->GetSize() + pVendorInfo->GetSize();
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_SYNC( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0};

	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T1( pMsg, 0 );
		}
		break;



	case CG_SYNC_PLAYER_ENTER_ACK:
		{
#ifdef _INFINITE_MAP_LOADING_TEST
			m_bReadyMapLoading = TRUE;
			m_dwStartTick = clock_function::GetTickCount();
#endif

			uiPersonalWayPointWindow* pDlg = GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
			if (pDlg)
			{
				pDlg->SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();
			}

#if defined(_GLOBAL) || defined(_RUSSIA)
            ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
            if (chat_dialog)
            {
                chat_dialog->BaseChannelCreate();
            }
#endif//defined(_GLOBAL) || defined(_RUSSIA)
			MSG_CG_SYNC_PLAYER_ENTER_ACK* pRecvPacket = (MSG_CG_SYNC_PLAYER_ENTER_ACK*)pMsg;

			pRecvPacket->Decode();

			WzVector wzVector;
			SetVector(&wzVector, pRecvPacket->m_fPos[0], pRecvPacket->m_fPos[1],pRecvPacket->m_fPos[2]);

			//Todo:히어로가 다른곳에서 파괴 되지 말고 새로 할당 된뒤 파괴 되게 하자. 곳곳에 널포인터 참조 위험이 있다?

			RESOURCECODE code = 0;
			eTRANSFORM_TYPE type = eTRANSFORM_TYPE_NONE;

			m_bWaitEnterSync = FALSE;

			m_iPrevSectorIndex = -1;

			if (TRUE == m_bComebackFromLobby)
			{
				// 로비에서 온놈이면 리스트를 띄워준다. 
				GET_DIALOG(IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST)->ShowInterface(TRUE);
				
				// 로비에서 돌아왔다.
				m_bComebackFromLobby = FALSE;
			}
			else
			{
				GET_DIALOG(IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE)->ShowInterface(FALSE);
                GET_DIALOG(IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2)->ShowInterface(FALSE);
			}

			if (pRecvPacket->m_TransSkillCode)
			{

				SkillScriptInfo* pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pRecvPacket->m_TransSkillCode);
				if (pInfo)
				{
					BASE_ABILITYINFO* pAbility = pInfo->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION1);

					if (pAbility)
					{
						code = pAbility->m_iOption1;
						type = (eTRANSFORM_TYPE)pAbility->m_iOption2;
					}
				}
			}


#ifdef _MOD_CHAGNE_DELETE_HERO
			g_ObjectManager.DeleteHero();
#endif

			if ( NULL == g_pHero)
			{
                DWORD class_code = xGetHeroData()->GetClassCode();
                ClassCodeEx class_code_ex;
                class_code_ex.class_type = xGetHeroData()->GetClassCode();
                class_code_ex.gender = xGetHeroData()->GetGender();
                class_code = class_code_ex.class_code;

                g_pHero = (Hero *)g_ObjectManager.Add(
                    xGetHeroData()->GetObjectKey(),
                    PLAYER_OBJECT,
                    class_code,
                    TRUE);

				g_InterfaceManager.InitSkillInfos();
				g_pHero->SetHeroInfo( xGetHeroData()->GetHeroInfo());
			}

			g_HeroInput.SetCameraBaseMove(GAMEOPTION->GetMoveToCamera());

            g_pHero->SetPosition( wzVector );

			if (!g_pMap)
			{
				return;
			}

			WzVector vecDummyTarget;
			if(g_pMap->GetSpawnHeroDirVector(g_pHero->GetPosition(), vecDummyTarget,200.0f))
			{
				//보간 하지 않고 바로 방향을 튼다.
				g_pHero->SetDirection(vecDummyTarget, TRUE);	
				g_Camera.SetCameraPosition(&g_pHero->GetVisiblePos());
				g_Camera.SetYaw(g_pHero->GetAngle());
				g_Camera.InitSpringCamera();
			}
			else
			{
				if ( g_pMap->GetPlayerDirection( &wzVector ) )
				{
					g_pHero->SetDirection(
						g_pMap->GetExtraCameraInfo()->WzTargetPos, 
						g_pMap->GetExtraCameraInfo()->WzPos);

					g_Camera.SetCameraPosition(&g_pHero->GetVisiblePos());
					g_Camera.SetYaw( Math_GetAngleFromVector( &wzVector) ); 
					g_Camera.InitSpringCamera();

				}

			}		
            m_bCameraDetech = FALSE;
			
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
			CHANGE_PCROOM_STATUS::eStatus PCBangStatus = (CHANGE_PCROOM_STATUS::eStatus)pRecvPacket->m_PcRoomStatus.status_;
			g_pHero->SetPCBangMode(CHANGE_PCROOM_STATUS::eStatus_PcRoom == PCBangStatus);

			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "CHANGE_PCROOM_STATUS : %d (CUR : %d)", PCBangStatus, g_pHero->GetPCRoomStatus());
	
			if (CHANGE_PCROOM_STATUS::eStatus_None != g_pHero->GetPCRoomStatus() && g_pHero->GetPCRoomStatus() != PCBangStatus)
			{
				int StringCode = 0;
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				if (PCBangStatus == CHANGE_PCROOM_STATUS::eStatus_PcRoom)
				{
					StringCode = 6071;
				}
				else
				{
					StringCode = 6072;
				}				
				g_InterfaceManager.GetInterfaceString(StringCode, szMessage);
				GlobalFunc::OutputMsg_SysMsg(szMessage);
			}
			g_pHero->SetPCRoomStatus(PCBangStatus);
#endif
			m_bWaitEnterSync = FALSE;
			g_pMap->SetUseAutoCamera(FALSE);
			g_pMap->SetAutoCameraTargetObjectKey( g_pHero->GetObjectKey() );

			if ( !m_bComebackFromLobby )
			{
				// 로비에서 돌아오지 않은 상태에서만 보여라.
				g_InterfaceManager.ShowGameInterface(TRUE);
			}

			// 글로우 켜라.
			// g_pMap->EnableBlur(TRUE); // 이거 뭐에요 왜있어요 ? --;
			m_bPlayerAck = TRUE;

			CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );

			// NPC 위치 시켜라.
			SettingNPC(g_pMap->GetMapID(),g_pMap->GetCurrentFieldID());	

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            // no operation
#else
            uiGuildMan* guildMan =
                static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
            if (guildMan)
            {
                guildMan->SetSilentGuildInfo();				
            }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

			uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
			
			if(pCommMan)
			{
				pCommMan->SEND_CW_FRIEND_NEWLIST_SYN();

				pCommMan->SEND_CW_GROUP_NEWLIST_SYN();	// 그룹목록요청
				pCommMan->SEND_CW_GROUP_RESTORELIST_SYN();	// 그룹복구요청

			}

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            if( ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance() )
            {
                pInstance->CheckNewPresent();
            }
#else
			uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
			if(pMan)
			{
				pMan->Send_CG_SHOP_IS_RECV_GIFT_SYN();
			}
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


			// 헬프 체크후 보여줄꺼 있으면 보여주자
			uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

			if (helpMan)
			{
				helpMan->CheckHelpMessage(eHK_CREATE_CHAR, (int)g_pMap->GetMapID());
				helpMan->CheckHelpMessage(eHK_JOIN_BATTLE_ZONE, (int)g_pMap->GetMapID());
				helpMan->CheckHelpMessage(eHK_LEVEL_UP);
				helpMan->ShowHelpMenu(TRUE);
			}

            uiGuideSystemMan* guide_manager =
                static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

            if (guide_manager)
            {
                guide_manager->ResetInfomationDialog();
                guide_manager->CheckHelpMessage(TotalGuideNotice::kCreate);
                guide_manager->CheckHelpMessage(TotalGuideNotice::kEnterToMap, (int)g_pMap->GetMapID());
                guide_manager->CheckHelpMessage(TotalGuideNotice::kLevelUp);
                guide_manager->CheckHelpMessage(TotalGuideNotice::kRemainGuide);
            }

			InitQuestInfo();

			EnduranceDIalog * pEndurance = static_cast<EnduranceDIalog *> 
				(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
			if(pEndurance)
			{
				pEndurance->UpdateEquipEnduranceState();
			}
			m_bMissionReward = FALSE;


			if(pRecvPacket->m_TransSkillCode)
			{
				g_pHero->GetTransformData().SetTransFormType(type);
				g_pHero->GetTransformData().SetTransFormResourceCode(code);
				g_pHero->SetTransSkillCode(pRecvPacket->m_TransSkillCode);
				g_pHero->ForceTransFrom();
			}

            //ASSERT(g_pHero->IsTransForm() && !(pRecvPacket->continued_wings_spreading_));
            if (g_pHero->IsTransForm() == FALSE)
            {
                if (g_pHero->rider())
                {
                    g_pHero->rider()->set_spread(pRecvPacket->continued_wings_spreading_);
                    WzID ani_id = GlobalFunc::GetRiderAnimationByPlayerAni(eANI_STAND, g_pHero->rider()->is_spread());
                    if (g_pHero->rider()->current_animation() != ani_id)
                    {
                        g_pHero->rider()->SetAnimation(ani_id, TRUE, TRUE);
                    }
                }
                else
                {
                    g_pHero->set_spread_wings(pRecvPacket->continued_wings_spreading_);
                }
            }

			uiQuestListDlg* pQuestListDlg = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
			if(pQuestListDlg)
			{
				pQuestListDlg->LoadNaviQCode();
				pQuestListDlg->RefreshQuestList();
			}

			uiQuestMan* pQM = (uiQuestMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
			if(pQM)
			{
				pQM->ClearKillMonQuest4Navi();
			}

			RadarFullDialog*  pfullRadar = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
			if (pfullRadar)
			{
				if( eFULL_RADAR_STATE_ALPHA == pfullRadar->GetRadarState() && SCENE_TYPE_SSQ != GameFramework::GetCurrentScene()->GetType() )
                {
                    uiBattleZoneMan* battlezone_manager = 
                        static_cast<uiBattleZoneMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
			        if (battlezone_manager && !battlezone_manager->BattleZoneListIsVisible())
                    {
					    pfullRadar->ShowInterface(TRUE);
                    }
                }
				else
                {
					pfullRadar->ResetRadarState();
                }
			}


			//////////////////////////////////////////////////////////////////////////
			//버프 이벤트
			EXTRA_NPCINFO_SET * pInfoSet =  
				NPCInfoParser::Instance()->GetExtraInfoSetFromFieldID(g_pMap->GetMapID(), (FIELDID)g_pMap->GetCurrentFieldID());

			vector<EXTRA_NPCINFO*>::iterator itr;
			EXTRA_NPCINFO * pInfo = NULL;

			for ( itr = pInfoSet->GetBegin(); itr != pInfoSet->GetEnd(); itr ++)
			{
				pInfo = *itr;

				if(!pInfo)
					continue;

				if ( pInfo->m_eNPCTYPE != eNPC_TYPE_EVENT_BUFF ) //NPC가 buff이벤트 타입 이라면
					continue;

                Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
                Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
                for ( ; found_itr != end_itr; ++found_itr)
                {

                    Object* pObject = found_itr->second;
                    if (pObject == NULL)
                    {
                        continue;
                    }

                    eOBJECT_TYPE eObjectType = pObject->GetSCObject().GetObjectType();
                    if (eObjectType != NPC_OBJECT)
                    {
                        continue;
                    }

                    NPC* pNPC = static_cast<NPC*>(pObject);
					if(NULL == pNPC)
						continue;

					if(pInfo->m_NPCCODE == pNPC->GetExtraInfo()->m_NPCCODE)
					{
						pNPC->SetSubTypeEffectShow(FALSE);

						SYSTEMTIME sTime;
						util::TimeSync::GetLocalTime(&sTime);

						DWORD dwDate = (sTime.wYear*10000) + (sTime.wMonth*100) + (sTime.wDay);
						WORD  dwTime = (sTime.wHour*100) + (sTime.wMinute);
						__time64_t CurDate;
						struct tm tmDate = { };
						tmDate.tm_year = dwDate / 10000 - 1900;
						tmDate.tm_mon = ( dwDate % 10000 ) / 100 - 1;
						tmDate.tm_mday = dwDate % 100;
						tmDate.tm_hour = dwTime / 100;
						tmDate.tm_min = dwTime % 100;
						if( tmDate.tm_hour == 24 )	
						{
							tmDate.tm_hour = 23;
							tmDate.tm_min = 59;
							tmDate.tm_sec = 59;
						}

						// struct tm를 time_t로 변환한다.
						CurDate = _mktime64( &tmDate ); 

						EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_Buff_Event_NPC_Type);
						if(pEventInfoVec)
						{
							EVENT_INFO_VEC::iterator begin_iter = pEventInfoVec->begin();
							EVENT_INFO_VEC::iterator end_iter = pEventInfoVec->end();
							int j = 0;
							for(; begin_iter < end_iter; ++begin_iter )
							{
								EVENT_INFO* pEventInfo = (*begin_iter);

								if(!pEventInfo)
									continue;

								if(CurDate >= pEventInfo->m_start_time    &&
								   CurDate <= pEventInfo->m_end_time	  &&
								   dwTime >= pEventInfo->m_wPeakTimeStart &&
								   dwTime <= pEventInfo->m_wPeakTimeEnd)
								{
										//이벤트 타임이라면 EffectShow
									pNPC->SetSubTypeEffectShow(TRUE);
									pNPC->SetNextState(STATE::NPC_SPECIAL_ACT_LOOP_STATE, g_CurTime);
									pNPC->CommitChangeState(g_CurTime);
									break;
								}
							}
						}
						break;
					}
				}
			}
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
            //피크타임 이벤트 타임 초기화
            TiredSystemDialog* tired_dialog = GET_CAST_DIALOG(TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM); 
            if (tired_dialog)
            {
               tired_dialog->InitPeakTimeEffect();
            }
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
            // 필드 소유길드정보 저장
			uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
			if (pUIMan)
			{
				pUIMan->SetDominationGuildInfo(&(pRecvPacket->m_GuildInfo));
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                pUIMan->set_mondschien_guild_name(pRecvPacket->m_ContinentLordInfo.guild_name);
                if (FALSE == pUIMan->get_is_character_enter_())
                {
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
                    pUIMan->set_domination_area_owner_guild_name_tutorial(pRecvPacket->m_ContinentLordInfo.guild_name, 
                                                                          pRecvPacket->m_ContinentLordInfo.allience_guild_infos, 
                                                                          pRecvPacket->m_ContinentLordInfo.allience_guild_count);
#else
                    pUIMan->set_domination_area_owner_guild_name_tutorial(pRecvPacket->m_ContinentLordInfo.guild_name);
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
                }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
			}
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
            // 출석체크
            uiAttendanceCheckMan* attendance_man = GET_DIALOG_MANAGER(uiAttendanceCheckMan, UIMAN_ATTENDANCE_CHECK);
            if (attendance_man)
            {
                attendance_man->first_open_window();    // 첫 접속 시 창열림(보상받을게 있다면)
            }
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#if USING_XIGNCODE
            uiLoginMan* login_man = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
            if (login_man != NULL && g_pHero != NULL && g_pMap != NULL)
            {
                XignCodeApi::SetInfo(login_man->GetCurrentServerData()->m_GroupName.c_str(),
                                    login_man->GetID(), 
                                    g_pHero->GetName(), 
                                    g_pMap->GetMapID(), 
                                    g_pMap->GetCurrentFieldID(), 
                                    g_pMap->GetFieldName());
            }
#endif //USING_XIGNCODE

            //
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
            KBMoveCheckManagerInClient::Instance()->StartLocalTickTest();
            ServerTimerSynchronier::Instance()->EnterField();
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT


#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 

            AutoTestManagerEx::Instance()->SetCompliteLogin();


            if( AutoTestManagerEx::Instance()->IsAutoCommand() == true )
            {
                AutoTestManagerEx::Instance()->RunAutoCommand();



                AutoTestManagerEx::Instance()->SetCompliteCommand();
            }

	//#ifdef _KIKI_AUTOTEST
	//		AutoTestManager::Instance()->CompleteCurTestCase();
	//			
	//#endif
#endif

            // 입장후 (또는 맵이동후)에 캐쉬중인 리소스를 모두 해제한다 
            ResourceManager::Instance()->FreeAllCachedData();

#if WZENGINEVER >= 720
            g_pEffectManager->ReleaseEffectTexture();
#endif 

			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CG_SYNC_PLAYER_ENTER_NAK:
		{
			//	Check The Map Version - CG_SYNC_PLAYER_ENTER_NAK - LoginScene
			m_bWaitEnterSync = FALSE;

            MSG_CG_SYNC_PLAYER_ENTER_NAK* recv =
                (MSG_CG_SYNC_PLAYER_ENTER_NAK *)pMsg;

			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("Recv : CG_SYNC_PLAYER_ENTER_NAK(%d)"), recv->m_dwErrorCode);

			switch(recv->m_dwErrorCode)
			{
			case RC_ZONE_CANT_FIND_ZONE:
				{
					//5751	해당 맵이 존재하지 않습니다.
					g_InterfaceManager.GetInterfaceString( 5751, szMessage, INTERFACE_STRING_LENGTH );
				}
				break;

			case RC_ZONE_NOT_MATCHED_MAP_VERSION:
				{
					//70247	맵 버전이 틀립니다.
					g_InterfaceManager.GetInterfaceString( 70247, szMessage, INTERFACE_STRING_LENGTH );
				}
				break;

			case RC_ZONE_MAP_PROBLEM:
				{
					//5752	해당 맵에 문제가 있어 입장할 수 없습니다.	
					g_InterfaceManager.GetInterfaceString( 5752, szMessage, INTERFACE_STRING_LENGTH );
				}
				break;

            case RC_ZONE_NOT_MATCHED_WMO_VERSION:
                {
#ifdef _DEV_VER
                    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("서버/클라간의 WMO파일이 틀립니다."));
#else //_DEV_VER
                    //70247	맵 버전이 틀립니다.
                    g_InterfaceManager.GetInterfaceString( 70247, szMessage, INTERFACE_STRING_LENGTH );
#endif //_DEV_VER
                }
                break;

			default:
				{
					//70530	[%d] 원인을 알 수 없는 에러가 발생했습니다. GM에게 문의해주십시오.
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(70530), recv->m_dwErrorCode);
				}
				break;
			}
			
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(szMessage, TRUE);

			
		}
		break;

	case CG_SYNC_ALLPLAYERS_CMD:
		{
			MSG_CG_SYNC_ALLPLAYERS_CMD *pRecvPacket = (MSG_CG_SYNC_ALLPLAYERS_CMD *)pMsg;

			BYTE *pPacket = ( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLPLAYERS_CMD) );

			PLAYER_RENDER_INFO_EX *	pRenderInfo = NULL;

			ETC_STATE_TOTAL_INFO *pShield = NULL;

			VENDOR_RENDER_INFO *	pVendorInfo = NULL;

			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{
				pRenderInfo = (PLAYER_RENDER_INFO_EX *)pPacket; 
				PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
				int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

				RESOURCECODE            rcCode=0;    
				DWORD                   dwSkillCode=0;
				eTRANSFORM_TYPE type=eTRANSFORM_TYPE_NONE;

				for( int j = 0; j < output.state_total_info.m_Count; ++j )
				{
					STATE_INFO & StateInfo = output.state_total_info.m_Slot[j];

					SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)StateInfo.m_SkillCode);
					if(pInfo)
					{
						BASE_ABILITYINFO * pAbility =pInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );

						if(pAbility)
						{
							rcCode= pAbility->m_iOption1;
							type= (eTRANSFORM_TYPE)pAbility->m_iOption2;
							dwSkillCode=StateInfo.m_SkillCode;
							break;
						}
					}
				}

				pShield = (ETC_STATE_TOTAL_INFO *)( (BYTE*)(pPacket) + nSize );
				pVendorInfo = (VENDOR_RENDER_INFO*)( (BYTE*)(pShield) + pShield->GetSize() );

				if (g_pHero)
				{
					if (g_pHero->GetObjectKey() == output.player_key)
					{
						assert(!"Hero가 ALLPLAYER_CMD에 왔다!");
						pPacket += GetPlayerSize(pPacket,nSize);
						continue;
					}
				}

                DWORD class_code = output.char_class;
                ClassCodeEx class_code_ex;
                class_code_ex.class_type = output.char_class;
                class_code_ex.gender = output.gender;
                class_code = class_code_ex.class_code;

				Player* pPlayer = (Player*)g_ObjectManager.Add( output.player_key, PLAYER_OBJECT, class_code, FALSE);
				if( !pPlayer )	
				{
					pPacket += GetPlayerSize(pPacket,nSize);
					continue;
				}

#ifdef _DEV_VER
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Add Player: %s(%d)", output.char_name, output.player_key);
#endif //_DEV_VER

				pPlayer->SetPosition( output.position );
				float fT=0.0f;
				WzVector vVisiblePos=output.position;
				int iMesh = -1;
				if(g_pMap)
					iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 5.0f,TMGO_NO_COLLISION);

				//타일로 바꾸자.
				if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
				{
					vVisiblePos.z -= fT;
				}

				pPlayer->SetVisiblePos(vVisiblePos);

                pPlayer->SetPlayerInfo(&output);

				pPlayer->SetCurrentAttackStyle(output.selected_style_code);
				

				if (output.helmet)
				{
					pPlayer->HideHelmet(TRUE,FALSE);

				}
				else
				{
					pPlayer->HideHelmet(FALSE,FALSE);
				}
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
                if (output.wing)
                {
                    pPlayer->HideWing(TRUE,FALSE);
                }
                else
                {
                    pPlayer->HideWing(FALSE,FALSE);
                }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
                if (output.costume)
                {
                    pPlayer->HideCostume(TRUE,FALSE);
                }
                else
                {
                    pPlayer->HideCostume(FALSE,FALSE);
                }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
				pPlayer->SetMarketTitle(pVendorInfo->m_tszVendorTitle, pVendorInfo->m_byTitleLength);


				for( int a = 0; a < pShield->m_Count; ++a )
				{
					ETC_STATE_INFO & rInfo = pShield->m_Slot[a];
                    if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( rInfo.m_wStateCode, rInfo.m_iLeavedTime ) )
                    {
                        pPlayer->AddSkillEffect(pEffect);
                    }
				}

				if(dwSkillCode)
				{
					pPlayer->GetTransformData().SetTransFormType(type);
					pPlayer->GetTransformData().SetTransFormResourceCode(rcCode);
					pPlayer->SetTransSkillCode(dwSkillCode);
					pPlayer->ForceTransFrom();
				}

				pPacket += GetPlayerSize(pPacket,nSize);
			}
		}
		break;

	case CG_SYNC_VILLAGE_ALLPLAYERS_CMD:
		{
			//no operation(패킷안옴)
		}
		break;

	case CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD:
		{
			MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD *pRecvPacket = (MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD *)pMsg;

			// 첫번째 장착정보의 위치를 셋팅한다.
			EQUIP_ITEM_BASEINFO *pEquipInfo = (EQUIP_ITEM_BASEINFO*)( (BYTE*)pRecvPacket + sizeof(MSG_CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD) );

			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{
				// 아이템 슬롯의 위치를 셋팅한다.
				EQUIP_ITEM_BASEINFO::data_type* pItemSlot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)pEquipInfo + sizeof(EQUIP_ITEM_BASEINFO));

				Player* pPlayer = (Player*)g_ObjectManager.GetObject(pEquipInfo->m_dwObjectKey);
				if(pPlayer)
				{
					pPlayer->SetEquipItemBaseInfo(pEquipInfo->m_Count, pItemSlot);
				}

				// 다음 장착정보의 위치를 셋팅한다.
				pEquipInfo = (EQUIP_ITEM_BASEINFO*)((BYTE*)pItemSlot + pEquipInfo->m_Count * sizeof(EQUIP_ITEM_BASEINFO::data_type));
			}
		}
		break;

	case CG_SYNC_ALLMONSTERS_CMD:
		{
			MSG_CG_SYNC_ALLMONSTERS_CMD *pRecvPacket = (MSG_CG_SYNC_ALLMONSTERS_CMD *)pMsg;

			MONSTER_RENDER_INFO *pRenderInfo = (MONSTER_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLMONSTERS_CMD) );

			ETC_STATE_TOTAL_INFO *pShield = NULL;

			float fMultiplier = GetMonsterHPIncreaseRatio();

			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{	
				AddMonsterWithPacket(pRenderInfo,fMultiplier);			
				
				Monster *pMonster = (Monster*)g_ObjectManager.GetObject(pRenderInfo->m_dwObjectKey);
				
				//쉴드정보만큼
				pShield = (ETC_STATE_TOTAL_INFO*)( (BYTE*)(pRenderInfo) + pRenderInfo->GetSize() );
				for( int a = 0; a < pShield->m_Count; ++a )
				{
					ETC_STATE_INFO & rInfo = pShield->m_Slot[a];
					if(pMonster)	
					{
                        if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( rInfo.m_wStateCode, rInfo.m_iLeavedTime ) )
                        {
                            pMonster->AddSkillEffect(pEffect);
                        }
					}
				}
			
				//몬스터정보만큼
				pRenderInfo = (MONSTER_RENDER_INFO*)( (BYTE*)(pShield) + pShield->GetSize() );

			}
		}
		break;
	case CG_SYNC_ALLMAPNPC_CMD:
		{
			MSG_CG_SYNC_ALLMAPNPC_CMD *pRecvPacket = (MSG_CG_SYNC_ALLMAPNPC_CMD *)pMsg;
		
			MAPNPC_RENDER_INFO *pRenderInfo = (MAPNPC_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLMAPNPC_CMD) );

			float fMultiplier = GetMonsterHPIncreaseRatio();
			
			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{	
				AddMonsterWithPacket(pRenderInfo->m_sMonsterRenderInfo,fMultiplier,FALSE,pRenderInfo->m_dwMapNpcID);				
				pRenderInfo = (MAPNPC_RENDER_INFO*)( (BYTE*)(pRenderInfo) + pRenderInfo->GetSize() );
			}
		}
		break;


	case CG_SYNC_ALLTOTEMNPC_CMD:
		{
			MSG_CG_SYNC_ALLTOTEMNPC_CMD *pRecvPacket = (MSG_CG_SYNC_ALLTOTEMNPC_CMD *)pMsg;

			TOTEMNPC_RENDER_INFO *pRenderInfo = (TOTEMNPC_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLTOTEMNPC_CMD) );

			// Monster 와 MapNpc 만이 HP 증가율이 적용된다 - 09.07.08 이태준
			//float fMultiplier = GetMonsterHPIncreaseRatio();			
			
			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{	
				AddTotemMonsterWithPacket(pRenderInfo, 1.0f/*fMultiplier*/);		
				pRenderInfo = (TOTEMNPC_RENDER_INFO*)( (BYTE*)(pRenderInfo) + pRenderInfo->GetSize() );
			}
		}
		break;


	//소환체 관련 
	case CG_SYNC_ALLSUMMONEDS_CMD:
		{
			MSG_CG_SYNC_ALLSUMMONEDS_CMD *pRecvPacket = (MSG_CG_SYNC_ALLSUMMONEDS_CMD *)pMsg;

			SUMMONED_RENDER_INFO *pRenderInfo = (SUMMONED_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLSUMMONEDS_CMD) );

			float fMultiplier = 1.0f;
			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{	
				Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRenderInfo->m_dwSummonerObjKey);

				Monster * pMonster = (Monster*)g_ObjectManager.Add( 
					pRenderInfo->m_dwObjectKey, 
					MONSTER_OBJECT, 
					pRenderInfo->m_dwMonsterCode, 
					FALSE);

				if( !pMonster )
				{
					WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER | c_wOUTPUT_LOG_CONSOLE, _T("몬스터 생성실패 Protocol = %d ObjKey = %d MonsterCode = %d\n"), pMsg->m_byProtocol, pRenderInfo->m_dwObjectKey, pRenderInfo->m_dwObjectKey  );
					
					pRenderInfo = (SUMMONED_RENDER_INFO*)( (BYTE*)(pRenderInfo) + pRenderInfo->GetSize() );
					continue;
				}

				pMonster->SetPosition( pRenderInfo->m_wvPos );
				
				float fT=0.0f;
				WzVector vVisiblePos=pRenderInfo->m_wvPos;
				int iMesh = -1;
				if(g_pMap)
					iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 5.0f,TMGO_NO_COLLISION);

				//타일로 바꾸자.
				if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
				{
					vVisiblePos.z -= fT;
				}

				pMonster->SetVisiblePos(vVisiblePos);
				pMonster->ChangeHPRatio(fMultiplier);
				pMonster->SetHP( pRenderInfo->m_dwHP );	
				pMonster->SetMaxHP( pRenderInfo->m_dwMaxHP );	
				pMonster->SetAttackSpeedModifier( pRenderInfo->m_wAttSpeedRatio/100.f );
				pMonster->SetMoveSpeedModifier( pRenderInfo->m_wMoveSpeedRatio/100.f );

				BOOL bShadowSummon = (pRenderInfo->m_bySummonType==eSUMMON_DEAD_MONSTER) ? (TRUE):(FALSE);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                BOOL bWitchBladeSummon = (pRenderInfo->m_bySummonType==eSUMMON_IMMOVABLE) ? (TRUE) : (FALSE);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

				if (g_pHero && (eSUMMON_CRYSTALWARP == pRenderInfo->m_bySummonType))
				{
					pMonster->SetMasterGuildID(pRenderInfo->m_dwSummonerObjKey);
					if ((g_pHero->GetGuildRenderInfo().m_GuildGuid!=INVALID_GUILDGUID) && 
                        g_pHero->GetPlayerGuildID() == pMonster->GetMasterGuildID())
                    {
						g_pHero->SetCrystalWarpKey(pMonster->GetObjectKey());
                    }

					if (RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter((Character*)pMonster, g_HeroInput.lsForceAttack()))
					{
						//빨강
						pMonster->SetParentId(0, 0, TRUE);
						pMonster->CreateCrystalWarpEffect(StrToWzID("O078"));
					}
					else
					{
						//파랑
						pMonster->SetParentId(0, 0, FALSE);
						pMonster->CreateCrystalWarpEffect(StrToWzID("O081"));
					}
				}
				else
				{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    BOOL is_target_window_hide = FALSE;
                    if (bShadowSummon == TRUE || bWitchBladeSummon == TRUE)
                    {
                        is_target_window_hide = TRUE;
                    }
                    pMonster->SetParentId(pRenderInfo->m_dwSummonerObjKey,pRenderInfo->m_byCurCommand,is_target_window_hide);
#else
					pMonster->SetParentId(pRenderInfo->m_dwSummonerObjKey, pRenderInfo->m_byCurCommand, bShadowSummon);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
				}

				//소울컨트롤된 몬스터
				if (bShadowSummon)
				{
					pMonster->OnShadowSummon();
				}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                //위치블레이드 고정형 소환수
                if(bWitchBladeSummon)
                {
                    pMonster->OnWitchBladeImmoveSummon();
                }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

                pMonster->SetStateTotalInfo(pRenderInfo->m_StateTotalInfo);
			
				if (pPlayer)
				{
					assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
					pPlayer->AddSummonId(pRenderInfo->m_dwObjectKey);
				}
				else
				{
					pMonster->SetMissedSummoner();
				}

				pRenderInfo = (SUMMONED_RENDER_INFO*)((BYTE*)(pRenderInfo) + pRenderInfo->GetSize());
			}
		}
		break;

		// 관리적인 문제로 이렇게 처리했습니다.
		// 아이템 처리부분은 Item Manager에서 하도록 하자.
	case CG_SYNC_ALLFIELDITEMS_CMD:
	case CG_SYNC_FIELDITEM_ENTER_BRD:
	case CG_SYNC_FIELDITEM_LEAVE_BRD:
		ItemManager::Instance()->OnRecvItemSync(pMsg);
		break;

	case CG_SYNC_PLAYER_ENTER_BRD:
		{
			MSG_CG_SYNC_PLAYER_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_PLAYER_ENTER_BRD *)pMsg;
            PLAYER_RENDER_INFO_EX* pRenderInfo = (PLAYER_RENDER_INFO_EX*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_PLAYER_ENTER_BRD) );
			PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
			int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Add Player: %s(%d)", output.char_name, output.player_key);
#endif //_DEV_VER

			if (g_pHero && (g_pHero->GetObjectKey() == output.player_key))
			{
    			break;
			}

			RESOURCECODE    rcCode=0;    
			DWORD           dwSkillCode=0;
			eTRANSFORM_TYPE type=eTRANSFORM_TYPE_NONE;

			for( int j = 0; j <  output.state_total_info.m_Count; ++j )
			{
				STATE_INFO & StateInfo = output.state_total_info.m_Slot[j];

				SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)StateInfo.m_SkillCode);
				if(pInfo)
				{
					BASE_ABILITYINFO * pAbility =pInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );
					//assert(pAbility);

					if(pAbility)
					{
						rcCode= pAbility->m_iOption1;
						type= (eTRANSFORM_TYPE)pAbility->m_iOption2;
						dwSkillCode=StateInfo.m_SkillCode;
						break;
					}
				}
			}

            DWORD class_code = output.char_class;
            ClassCodeEx class_code_ex;
            class_code_ex.class_type = output.char_class;
            class_code_ex.gender = output.gender;
            class_code = class_code_ex.class_code;
			Player* pPlayer = (Player*)g_ObjectManager.Add( output.player_key, PLAYER_OBJECT, class_code, FALSE);
			if( !pPlayer )	break;

			if (output.helmet)
			{
				pPlayer->HideHelmet(TRUE,FALSE);
			}
			else
			{
				pPlayer->HideHelmet(FALSE,FALSE);
			}
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
            if (output.wing)
            {
                pPlayer->HideWing(TRUE,FALSE);
            }
            else
            {
                pPlayer->HideWing(FALSE,FALSE);
            }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
            if (output.costume)
            {
                pPlayer->HideCostume(TRUE,FALSE);
            }
            else
            {
                pPlayer->HideCostume(FALSE,FALSE);
            }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
			pPlayer->SetPosition( output.position );

			float fT=0.0f;
			WzVector vVisiblePos=output.position;

			int iMesh = -1;
			if(g_pMap)
				iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 5.0f,TMGO_NO_COLLISION);

			//타일로 바꾸자.
			if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			{
				vVisiblePos.z -= fT;
			}

			pPlayer->SetVisiblePos(vVisiblePos);

			pPlayer->SetRandomAngle();
			pPlayer->SetPlayerInfo( &output);

			pPlayer->SetCurrentAttackStyle(output.selected_style_code);

			//쉴드정보가 이중간에 들어온다
			ETC_STATE_TOTAL_INFO *pShield = (ETC_STATE_TOTAL_INFO*)( (BYTE*)(pRenderInfo) + nSize);

			for( int a = 0; a < pShield->m_Count; ++a )
			{
				ETC_STATE_INFO & rInfo = pShield->m_Slot[a];
				if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( rInfo.m_wStateCode, rInfo.m_iLeavedTime ) )
                {
                    pPlayer->AddSkillEffect(pEffect);
                }
			}

			//이큅정보
			EQUIP_ITEM_BASEINFO* pEquipInfo = (EQUIP_ITEM_BASEINFO*)((BYTE*)(pShield) + pShield->GetSize());

			EQUIP_ITEM_BASEINFO::data_type* pItemSlot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)(pEquipInfo) + sizeof(EQUIP_ITEM_BASEINFO));
			pPlayer->SetEquipItemBaseInfo( pEquipInfo->m_Count, pItemSlot );

			if(dwSkillCode)
			{
				pPlayer->GetTransformData().SetTransFormType(type);
				pPlayer->GetTransformData().SetTransFormResourceCode(rcCode);
				pPlayer->SetTransSkillCode(dwSkillCode);
				pPlayer->ForceTransFrom();
			}

			//이펙트 정보
			sMSG_RENDER_EFFECT_ITEM_LIST* effect_item_info = (sMSG_RENDER_EFFECT_ITEM_LIST*)( ((BYTE*)pEquipInfo) + pEquipInfo->GetSize() );

			if( Player* player_ptr = (Player*)g_ObjectManager.GetObject(effect_item_info->m_PlayerKey) )
			{
				player_ptr->RefreshRenderEffectItem( player_ptr, &effect_item_info->m_ItemCodes[0], effect_item_info->m_ItemCount);
			}
		}
		break;

    case CG_SYNC_ALLMERCHANTNPC_CMD:
        {
            MSG_CG_SYNC_ALLMERCHANTNPC_CMD* recv_packet_ptr 
                = static_cast<MSG_CG_SYNC_ALLMERCHANTNPC_CMD*>(pMsg);

            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "MSG_CG_SYNC_ALLMERCHANTNPC_CMD");

            MERCHANT_RENDER_INFO* render_info_ptr = 
                (MERCHANT_RENDER_INFO*)((BYTE*)(recv_packet_ptr) + sizeof(MSG_CG_SYNC_ALLMERCHANTNPC_CMD));	
            if (render_info_ptr == NULL)
            {
                break;
            }

            int render_info_size = 0;
            for (int i = 0; i < recv_packet_ptr->m_byCount; i++)
            {
                if (render_info_ptr == NULL)
                {
                    break;
                }

                AddNPCWithPacket(render_info_ptr);

                render_info_size = render_info_ptr->GetSize();
                render_info_ptr = 
                    (MERCHANT_RENDER_INFO*)((BYTE*)(render_info_ptr) + render_info_size);
            }
        }
        break;

	case CG_SYNC_VILLAGE_PLAYER_ENTER_BRD:
		{
			//no operation (이패킷은 안옴)
		}
		break;


	case CG_SYNC_PLAYER_LEAVE_BRD:
		{
			MSG_CG_SYNC_PLAYER_LEAVE_BRD *pRecvPacket = (MSG_CG_SYNC_PLAYER_LEAVE_BRD *)pMsg;

			for( int i = 0; i < pRecvPacket->m_byCount; ++i )
			{
				DWORD dwObjectKey = pRecvPacket->m_PlayerKey[i];

				if(g_pHero && (dwObjectKey == g_pHero->GetObjectKey()))		continue;

				Object * pObject = g_ObjectManager.GetObject( dwObjectKey );
				Player * pPlayer = static_cast<Player *>(pObject);
				if( !pPlayer )	continue;

				pPlayer->ProcessAllRemainActionResult();
				g_ObjectManager.Delete( dwObjectKey );

			}
		}
		break;

	case CG_SYNC_MONSTER_ENTER_BRD:		// 필드에 등장하는 몬스터의 정보 브로드캐스팅
		{
			MSG_CG_SYNC_MONSTER_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_MONSTER_ENTER_BRD *)pMsg;

			MONSTER_RENDER_INFO *pRenderInfo = (MONSTER_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_MONSTER_ENTER_BRD) );

			float fMultiplier = GetMonsterHPIncreaseRatio();

			AddMonsterWithPacket(pRenderInfo,fMultiplier,TRUE);
		}
		break;

	case CG_SYNC_MAPNPC_ENTER_BRD:
		{
			MSG_CG_SYNC_MAPNPC_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_MAPNPC_ENTER_BRD *)pMsg;

			MONSTER_RENDER_INFO *pRenderInfo = (MONSTER_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_MAPNPC_ENTER_BRD) );

			float fMultiplier = GetMonsterHPIncreaseRatio();

			AddMonsterWithPacket(pRenderInfo,fMultiplier,TRUE,pRecvPacket->m_dwMapNpcID);	
		}
		break;


	case CG_SYNC_TOTEMNPC_ENTER_BRD:
		{
			MSG_CG_SYNC_TOTEMNPC_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_TOTEMNPC_ENTER_BRD *)pMsg;

			TOTEMNPC_RENDER_INFO *pRenderInfo = (TOTEMNPC_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_TOTEMNPC_ENTER_BRD) );

			// Monster 와 MapNpc 만이 HP 증가율이 적용된다 - 09.07.08 이태준
			//float fMultiplier = GetMonsterHPIncreaseRatio();			

			AddTotemMonsterWithPacket(pRenderInfo, 1.0f/*fMultiplier*/);	
		}
		break;


	case CG_SYNC_SUMMONED_ENTER_BRD:		// 필드에 등장하는 소환 몬스터의 정보 브로드캐스팅
		{
			MSG_CG_SYNC_SUMMONED_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_SUMMONED_ENTER_BRD *)pMsg;

			SUMMONED_RENDER_INFO *pRenderInfo = (SUMMONED_RENDER_INFO*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_SUMMONED_ENTER_BRD) );

			Player *pPlayer=(Player *)g_ObjectManager.GetObject(pRenderInfo->m_dwSummonerObjKey);
			Monster	*pMonster = (Monster*)g_ObjectManager.Add(pRenderInfo->m_dwObjectKey, MONSTER_OBJECT, pRenderInfo->m_dwMonsterCode, FALSE);

			if (!pMonster)	
			{
				WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER | c_wOUTPUT_LOG_CONSOLE, _T("몬스터 생성실패 Protocol = %d ObjKey = %d MonsterCode = %d\n"), pMsg->m_byProtocol, pRenderInfo->m_dwObjectKey, pRenderInfo->m_dwObjectKey  );
				break;
			}		
			
			if (pPlayer)
			{
				pPlayer->AddSummonId(pRenderInfo->m_dwObjectKey);
				assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
			}
			else
			{
				//GS_BUG_FIX: 주인(Player)정보가 오지않았다.
				pMonster->SetMissedSummoner();

			}
			
			float fMultiplier = 1.0f;

			pMonster->ChangeHPRatio(fMultiplier);
			pMonster->SetHP(pRenderInfo->m_dwHP);
			pMonster->SetMaxHP(pRenderInfo->m_dwMaxHP);
			pMonster->SetPosition( pRenderInfo->m_wvPos );

			WzVector vVisiblePos=pRenderInfo->m_wvPos;
			vVisiblePos.z+=2.0;
			float fT=0.0f;
			int iMesh = -1;
			if(g_pMap)
				iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 5.0f,TMGO_NO_COLLISION);

			//타일로 바꾸자.
			if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			{
				vVisiblePos.z -= fT;
			}

			pMonster->SetVisiblePos(vVisiblePos);
			pMonster->SetAttackSpeedModifier( pRenderInfo->m_wAttSpeedRatio/100.f );
			pMonster->SetMoveSpeedModifier( pRenderInfo->m_wMoveSpeedRatio/100.f );

			BOOL bShadowSummon = (pRenderInfo->m_bySummonType==eSUMMON_DEAD_MONSTER) ? (TRUE) : (FALSE);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            BOOL bWitchBladeSummon = (pRenderInfo->m_bySummonType==eSUMMON_IMMOVABLE) ? (TRUE) : (FALSE);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

			if (g_pHero && (eSUMMON_CRYSTALWARP == pRenderInfo->m_bySummonType))
			{
				pMonster->SetMasterGuildID(pRenderInfo->m_dwSummonerObjKey);
				if((g_pHero->GetGuildRenderInfo().m_GuildGuid!=INVALID_GUILDGUID) && 
                    g_pHero->GetPlayerGuildID() == pMonster->GetMasterGuildID())
					g_pHero->SetCrystalWarpKey(pMonster->GetObjectKey());

				if(RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter((Character*)pMonster, g_HeroInput.lsForceAttack()))
				{
					//빨강
					pMonster->SetParentId(0,0,TRUE);
					pMonster->CreateCrystalWarpEffect(StrToWzID("O078"));
				}
				else
				{
					//파랑
					pMonster->SetParentId(0,0,FALSE);
					pMonster->CreateCrystalWarpEffect(StrToWzID("O081"));
				}
			}
			else
			{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                BOOL is_target_window_hide = FALSE;
                if (bShadowSummon == TRUE || bWitchBladeSummon == TRUE)
                {
                    is_target_window_hide = TRUE;
                }
                pMonster->SetParentId(pRenderInfo->m_dwSummonerObjKey,pRenderInfo->m_byCurCommand,is_target_window_hide);
#else
				pMonster->SetParentId(pRenderInfo->m_dwSummonerObjKey,pRenderInfo->m_byCurCommand,bShadowSummon);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			}

			//소울컨트롤된 몬스터
			if(bShadowSummon)
			{
				pMonster->OnShadowSummon();
			}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            //위치블레이드 고정형 소환수
            if(bWitchBladeSummon)
            {
                pMonster->OnWitchBladeImmoveSummon();
            }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			
			pMonster->SetFreeMove(FALSE);
			pMonster->SetHeight(0.0f);

            pMonster->SetStateTotalInfo(pRenderInfo->m_StateTotalInfo);

			//에니메이션과시간 스크립트에서 읽을것 
			BASE_NPCINFO *pInfo=NPCInfoParser::Instance()->GetNPCInfo( pRenderInfo->m_dwMonsterCode );
			assert(pInfo);
			if(pInfo&&pInfo->m_dwSpawnTime!=0)//스폰 되는 놈만 
			{
				PLAYER_ACTION idleaction;
				idleaction.SPAWN.dwEndTime=pInfo->m_dwSpawnTime+g_CurTime;
				pMonster->SetCurrentAction(&idleaction);
				pMonster->SetNextState(STATE::SPAWN,g_CurTime);		
			}
			// 스폰애니없는놈은 스르륵 나타나지요
			else
			{
				pMonster->SetFadeIn(1000);
			}
		}
		break;

	case CG_SYNC_MONSTER_DIR_ENTER_BRD:
		{
            // [6/4/2010 iforall]
            // 해당 패킷은 현재 위미가 없어 보임
            // 패킥의 멤버변수로 있는 angle 값은 내부 MONSTER_RENDER_INFO 의 멤버로 있음
            // 서버확인결과 해당 값은 동일함.
            // 따라서 MSG_CG_SYNC_MONSTER_ENTER_BRD 패킷과 별도로 있을 필요가 없어보임.
            MSG_CG_SYNC_MONSTER_DIR_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_MONSTER_DIR_ENTER_BRD *)pMsg;

            MONSTER_RENDER_INFO *pRenderInfo = (MONSTER_RENDER_INFO*)((BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_MONSTER_DIR_ENTER_BRD));

            float fMultiplier = GetMonsterHPIncreaseRatio();

            pRenderInfo->m_wAngle = pRecvPacket->wAngle;

            AddMonsterWithPacket(pRenderInfo,fMultiplier,TRUE);
		}
		break;


    case CG_SYNC_MERCHANT_ENTER_BRD:
        {
            MSG_CG_SYNC_MERCHANT_ENTER_BRD* recv_packet_ptr 
                = static_cast<MSG_CG_SYNC_MERCHANT_ENTER_BRD*>(pMsg);

            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "MSG_CG_SYNC_MERCHANT_ENTER_BRD");

            MERCHANT_RENDER_INFO *render_info_ptr = 
                (MERCHANT_RENDER_INFO*)((BYTE*)(recv_packet_ptr) + sizeof(MSG_CG_SYNC_MERCHANT_ENTER_BRD));	

            if (render_info_ptr != NULL)
            {
                AddNPCWithPacket(render_info_ptr);
            }
        }
        break;

	case CG_SYNC_MONSTER_LEAVE_BRD:		// 필드에서 떠나는 몬스터의 정보 브로드캐스팅
		{
			MSG_CG_SYNC_MONSTER_LEAVE_BRD *pRecvPacket = (MSG_CG_SYNC_MONSTER_LEAVE_BRD *)pMsg;

			for( int i = 0; i < pRecvPacket->m_byCount; ++i )
			{
				Object *pObject = g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey[i] );
				Character *character_ptr = static_cast<Character *>(pObject);
				if( character_ptr == NULL )	continue;

				// 시체 스르륵 사라지기
				DWORD dwCloneKey = g_CloneManager.CreateClone(pObject);
				Clone *pClone = g_CloneManager.GetClone(dwCloneKey);
				if (pClone) 
				{
					WzColor color = WzColor_RGBA(255,255,255,255);
					pClone->SetAlphaBlend(AB_ALPHA);
					pClone->SetColor(color);
					pClone->SetLifeTime(900);
					pClone->SetDisappear(800);
					pClone->SetPlayAni(FALSE);
				}

				character_ptr->ProcessAllRemainActionResult();
				character_ptr->SetTargetID( 0 );
				g_ObjectManager.Delete(character_ptr->GetObjectKey());
			}
		}
		break;

	case CG_SYNC_OBJECT_DISAPPEAR_DEAD_BRD:
		{
			MSG_CG_SYNC_OBJECT_DISAPPEAR_DEAD_BRD* pRecvPacket = (MSG_CG_SYNC_OBJECT_DISAPPEAR_DEAD_BRD*) pMsg;
			Object *pObject = g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey );
			Monster *pMonster = static_cast<Monster *>(pObject);
			if( pMonster == NULL )	
				break;

            if ((pMonster->GetMonsterInfo() != NULL) && 
                (pMonster->GetMonsterInfo()->m_MonsterCode == 65535)
                /*(pMonster->GetMonsterType() == MONSTER_TYPE_TOTEM)*/)
            {
                //폭탄이면 바로 사라지게 한다.
            }
            else
            {
			    // 시체 스르륵 사라지기
			    DWORD dwCloneKey = g_CloneManager.CreateClone(pMonster);
			    Clone *pClone = g_CloneManager.GetClone(dwCloneKey);
			    if (pClone)
			    {
				    WzColor color = WzColor_RGBA(255,255,255,255);
				    pClone->SetAlphaBlend(AB_ALPHA);
				    pClone->SetColor(color);
				    pClone->SetLifeTime(900);
				    pClone->SetDisappear(800);
				    pClone->SetPlayAni(FALSE);
			    }
			    g_pEffectManager->CreateEffect(StrToWzID("A023"),pMonster->GetVisiblePos());
            }

			pMonster->ProcessAllRemainActionResult();
			pMonster->SetTargetID( 0 );
			g_ObjectManager.Delete(pMonster->GetObjectKey());
		}
		break;


    case CG_SYNC_MOVE_NAK:
        {
            MSG_CG_SYNC_MOVE_NAK * pRecvPacket = (MSG_CG_SYNC_MOVE_NAK *)pMsg;

	        if ( GENERALPARAM->GetMoveNakProcess() )
			{
 
                // 현재이동 중인 캐릭터를 Stop 시키고 현재 위치를 바꿈.
                if ( g_pHero )
				{
					WzVector vOffset = pRecvPacket->m_wvCurPos - g_pHero->GetPosition();
					float fDistance = VectorLength( &vOffset );

					if(fDistance>=10.0f)
					{
						g_InterfaceManager.SetMovePointer(FALSE, NULL);

						g_pHero->StopMove();
						g_pHero->SetPosition(pRecvPacket->m_wvCurPos);
						g_pHero->SetNextState(STATE::IDLE,g_CurTime);
						//점프 중 워프 처리 
						if(g_pHero->GetCurState()==STATE::KEYBOARDJUMP)
						{
							WzVector vVisiblePos=pRecvPacket->m_wvCurPos;
							vVisiblePos.z+=2.0;

							int iTileFocussing = -1;
							float fT=0.0f;
							iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vVisiblePos, &fT, -1.0f, 10.0f );

							if(iTileFocussing >=0)
                            {
#ifdef _NA_20120220_JUMP_RENEWAL
								g_pHero->m_JumpData.m_vJumpTargetPosition = g_pHero->m_JumpData.m_vJumpStartPosition = vVisiblePos;
#else //_NA_20120220_JUMP_RENEWAL
                                g_pHero->m_JumpData.m_JumpTargetPosition = g_pHero->m_JumpData.m_JumpStartPosition = vVisiblePos;
#endif //_NA_20120220_JUMP_RENEWAL
							}
						}
					}
                }
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
                if (g_pHero != NULL)
                {
                    g_pHero->PacketNak.is_recv_nak = true;
                    g_pHero->PacketNak.delay_tick = clock_function::GetTickCount();
                }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

                // error result 처리
                if ( GENERALPARAM->GetSpecialMode() )
                {
                    TCHAR szMessage[INTERFACE_STRING_LENGTH];
                    Sprintf( 
                        szMessage, 
                        _T("Move Nak Result Code(%d)"), 
                        pRecvPacket->m_byErrorCode);
                    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
                }                
            }
        }
        break;

    case CG_SYNC_NPC_JUMPRESULT_BRD:
        {
            MSG_CG_SYNC_NPC_JUMPRESULT_BRD * pRecvPacket = (MSG_CG_SYNC_NPC_JUMPRESULT_BRD *)pMsg;

            MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD pk; 
            pk.m_dwObjectKey =          pRecvPacket->m_dwObjectKey;
            pk.m_dwTargetObjectKey =    pRecvPacket->m_dwTargetObjectKey;
            pk.m_byAttackType =         pRecvPacket->m_byAttackType;
            pk.m_wvCurPos =             pRecvPacket->m_wvCurPos;
            pk.m_wDamage =              pRecvPacket->m_wDamage;
            pk.m_wTargetHP =            pRecvPacket->m_wTargetHP;
            pk.m_byEffect =             pRecvPacket->m_byEffect;

            OnRecv_MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD(&pk);
        }
        break;


	case CG_SYNC_ACTION_EXPIRED_CMD:
		{
			MSG_CG_SYNC_ACTION_EXPIRED_CMD *pRecvPacket = (MSG_CG_SYNC_ACTION_EXPIRED_CMD *)pMsg;

			if (g_pHero)
			{
				g_pHero->SetCannotMoveFlag(FALSE);
				// 테스트
				g_TempVariable[1]++;
			}
		}
		break;


	case CG_SYNC_JUMP_ACK:
		{
			//MSG_CG_SYNC_JUMP_ACK *pRecvPacket = (MSG_CG_SYNC_JUMP_ACK *)pMsg;
		}
		break;

	case CG_SYNC_JUMP_NAK:
		{
			//MSG_CG_SYNC_JUMP_NAK *pRecvPacket = (MSG_CG_SYNC_JUMP_NAK *)pMsg;	
		}
		break;

	case CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD:
		{
			MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD * pRecvPacket = (MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD *)pMsg;		

			Character * pChr = (Character *)g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey );    
			if ( pChr )
			{
				pChr->ReleaseMoveTarget();
				pChr->SetMoveTarget(0);					

				PLAYER_ACTION action;
				ZeroMemory(&action, sizeof(action));
				action.ActionID = ACTION_RUNAWAY;
				action.MOVE.byState = CMS_RUN;
				action.MOVE.bThrust = FALSE;
				action.MOVE.vCurPos.wvPos   = pChr->GetPosition();
				action.MOVE.vCurPos.wTile   = -1;
				// 특정 거리에 따른 렌덤한 포지션을 받아온다.
				if ( !GameFunc::GetValidPosForRange( 
					action.MOVE.vCurPos.wvPos,
					(float)pRecvPacket->m_iRotateAngle*WZ_PI/180.f,
					action.MOVE.vDestPos.wvPos, 
					40.f ) )
				{
					action.MOVE.vDestPos.wvPos = action.MOVE.vCurPos.wvPos;
				}
				action.MOVE.vDestPos.wTile  = -1;

				if (GENERALPARAM->IsServerDebug()) 
				{
					pChr->DoAction(&action);	
					pChr->CommitChangeState(g_CurTime);	
				}
				else
				{
					pChr->PutAction(&action);
				}
			}
		}
		break;


	case CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD:
		{
            MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD* recv_packet = static_cast<MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD*>(pMsg);
            BYTE* info_offset = reinterpret_cast<BYTE*>(recv_packet) + sizeof(MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD);
            GUILD_RENDER_INFO* guild_render_info = NULL;
            for (int i = 0; i < recv_packet->m_byCount; ++i)
            {
                guild_render_info = reinterpret_cast<GUILD_RENDER_INFO*>(info_offset);
                DWORD player_key = guild_render_info->m_dwObjectKey;
                Object* object = g_ObjectManager.GetObject(player_key);
                if (object != NULL &&
                    object->GetSCObject().GetObjectType() == PLAYER_OBJECT)
                {
                    Player* player = static_cast<Player*>(object);
                    if(player)
                    {
                        player->SetGuildRenderInfo(guild_render_info);
                        player->SetDominationStateInfo(&guild_render_info->m_stateTotalInfo);

                        // 자신의 인장UI(zonemark) 업데이트
                        if (g_pHero && (g_pHero->GetObjectKey() == player_key))
                        {                        
                            uiDominationTowerMan::RefreshZoneMarkInfo();
                        }
                    }
                }                
                else
                {
    #ifdef _DEV_VER
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_SYNC_ALLPLAYERS_GUILDRENDERINFO_CMD::Invalid Key(%d)"), player_key);
    #endif //_DEV_VER
                }
                info_offset += (guild_render_info->GetSize());
            }
		}
		break;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	case CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD:
		{
			MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD* recv_packet = static_cast<MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD*>(pMsg);
			BYTE* info_offset = reinterpret_cast<BYTE*>(recv_packet) + sizeof(MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD);
			SUN_RANKING_GRADE_INFO* sun_ranking_grade_info = NULL;
			for (int i = 0; i < recv_packet->m_byCount; ++i)
			{
				sun_ranking_grade_info = reinterpret_cast<SUN_RANKING_GRADE_INFO*>(info_offset);
				DWORD player_key = sun_ranking_grade_info->m_dwObjectKey;
				Object* object = g_ObjectManager.GetObject(player_key);
				if (object != NULL &&
					object->GetSCObject().GetObjectType() == PLAYER_OBJECT)
				{
					Player* player = static_cast<Player*>(object);
					if(player)
					{
						CLIENT_PLAYER_RANKING_GRADE_INFO client_player_ranking_grade_info;
						client_player_ranking_grade_info.ranking_type = sun_ranking_grade_info->ranking_type;
						client_player_ranking_grade_info.grade = sun_ranking_grade_info->grade;
						player->SetClientPlayerRankingGradeInfo(&client_player_ranking_grade_info);
					}
				}
				else
				{
#ifdef _DEV_VER
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD::Invalid Key(%d)"), player_key);
#endif //_DEV_VER
				}
				info_offset += (sun_ranking_grade_info->GetSize());
			}
		}
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE


	case CG_SYNC_COMPOSED_STREAM_CMD:
		{
			const MSG_CG_SYNC_COMPOSED_STREAM_CMD* pRecvPacket = static_cast<MSG_CG_SYNC_COMPOSED_STREAM_CMD*>(pMsg);

			WAR_MSG_HDR header = *(pRecvPacket->m_Header);
			WAR_TAG_P pIt = (WAR_TAG_P)(pRecvPacket->m_Header+1);

			OnRecvCG_SYNC_COMPOSE(pIt, header);
		}
		break;

    case CG_SYNC_WINGS_CONTROL_CMD:
        {
            const MSG_CG_SYNC_WINGS_CONTROL_CMD* pk = static_cast<MSG_CG_SYNC_WINGS_CONTROL_CMD*>(pMsg);

            if (pk->header.request == WingControl::eReq_SpreadWings)
            {
                OnRecv_SpreadWings(pk);
            }
            else if (pk->header.request == WingControl::eReq_FurlWings)
            {
                OnRecv_FurlWings(pk);
            }
        }
        break;
	}
}
//------------------------------------------------------------------------------ 
void BattleScene::OnRecv_SpreadWings(const MSG_CG_SYNC_WINGS_CONTROL_CMD* pk)
{
    if (pk->data.data_type != WingControlData::eDataType_Actor)
    {
        return;
    }
    Player* player = g_ObjectManager.GetPlayer(pk->data.actor.requester);
    if (player == NULL)
    {
        return;
    }
 
    //! 날개 펴기에 대한 응답
    if (pk->header.result == WingControl::eRet_Success)
    {
        // 성공처리
        if (pk->header.args.spread.wings_container == SI_INVENTORY)
        {
            // 현재 인벤토리는 칼버스 탈것 뿐임
            if (player->rider() != NULL)
            {
                player->rider()->set_spread(true);
            }

            if (player != g_pHero)
            {
                PLAYER_ACTION action;
                ZeroMemory(&action,sizeof(action));
                action.ActionID = ACTION_RIDER_SPREAD;
                player->PutAction(&action);
            }
        }
        else if (pk->header.args.spread.wings_container == SI_EQUIPMENT)
        {
            player->set_spread_wings(true);

            if (player != g_pHero)
            {
                PLAYER_ACTION action;
                ZeroMemory(&action,sizeof(action));
                action.ActionID = ACTION_WINGS_SPREAD;
                player->PutAction(&action);
            }
        }
    }
    else
    {
        // 실패
        if (pk->header.args.spread.wings_container == SI_INVENTORY)
        {
            // 현재 인벤토리는 칼버스 탈것 뿐임
            if (player->rider() != NULL)
            {
                player->rider()->set_spread(false);
            }

            if(player->GetCurState() == STATE::RIDER_SPREAD)    
            {
                if(player->GetNextState() == STATE::NONE)
                {
                    player->SetNextState(STATE::IDLE, g_CurTime);
                }
                player->CommitChangeState(g_CurTime);
            }
        }
        else if (pk->header.args.spread.wings_container == SI_EQUIPMENT)
        {
            player->set_spread_wings(false);

            if(player->GetCurState() == STATE::WINGS_SPREAD)    
            {
                if(player->GetNextState() == STATE::NONE)
                {
                    player->SetNextState(STATE::IDLE, g_CurTime);
                }
                player->CommitChangeState(g_CurTime);
            }
        }
        if (player == g_pHero)
        {
            GlobalFunc::OutputMsg_WingControl_ResultCode(pk->header.result, pk->header.result_code);
        }
    }
}
//------------------------------------------------------------------------------ 
void BattleScene::OnRecv_FurlWings(const MSG_CG_SYNC_WINGS_CONTROL_CMD* pk)
{
    if (pk->data.data_type != WingControlData::eDataType_Actor)
    {
        return;
    }

    Player* player = g_ObjectManager.GetPlayer(pk->data.actor.requester);
    if (player == NULL)
    {
        return;
    }

    //! 날개 접기에 대한 응답
    if (pk->header.result == WingControl::eRet_Success)
    {
        // 성공처리
        if (pk->header.args.spread.wings_container == SI_INVENTORY)
        {
            if (player->rider() != NULL)
            {
                player->rider()->set_spread(false);
            }
        }
        else if (pk->header.args.spread.wings_container == SI_EQUIPMENT)
        {
            player->set_spread_wings(false);
        }
    }
    else
    {
        if (player == g_pHero)
        {
            GlobalFunc::OutputMsg_WingControl_ResultCode(pk->header.result, pk->header.result_code);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_SYNC_COMPOSE(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header)
{
	using namespace nsSync;

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {'\0',};
	int	  MessageType = eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT;
	WAR_TAG_P pIt	= const_cast<WAR_TAG_P>(pMsg);
	WAR_TAG_P pEnd	= pIt + header.size;

	for (WORD size = 0; pIt < pEnd; pIt += size)
	{
		size	= 10000;	// 미 설정에 대한 스킵 설정 (buffer-overrun protector)
		switch (pIt->_TAG)
		{
		case SYNC_MOVE_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR(SYNC_MOVE_BRD);

				if (rMsg.m_ForcedMove)
				{
					if (g_pHero && (rMsg.m_dwObjectKey == g_pHero->GetObjectKey()))
					{
						if (g_pHero->SetNextState(STATE::MOVE, g_CurTime))
						{
							g_pHero->CommitChangeState(g_CurTime);	
							g_pHero->Move(g_pHero->GetPosition(), rMsg.m_wvDestPos, CMS_RUN, FALSE);
						}	
						continue;
					}
				}

				if (g_pHero && (rMsg.m_dwObjectKey == g_pHero->GetObjectKey()))
                {
					continue;
                }

				Character* pChr = (Character*)g_ObjectManager.GetObject(rMsg.m_dwObjectKey);
				
				if (pChr != NULL)
				{
					pChr->ReleaseMoveTarget();

					PLAYER_ACTION action;
					ZeroMemory(&action, sizeof(action));
					action.ActionID = ACTION_MOVE;
					action.MOVE.byState = rMsg.m_byState;
					action.MOVE.bThrust = FALSE;
					action.MOVE.vCurPos.wvPos = rMsg.m_wvCurPos;
					action.MOVE.vCurPos.wTile = -1;
					action.MOVE.vDestPos.wvPos = rMsg.m_wvDestPos;
					action.MOVE.vDestPos.wTile = -1;
					
					if (GENERALPARAM->IsServerDebug()) 
					{
						pChr->DoAction(&action);	
						pChr->CommitChangeState(g_CurTime);	
					}
					else
					{
						pChr->PutAction(&action);
					}

					if (pChr->IsKindOfObject(MONSTER_OBJECT)) 
					{
						if (GENERALPARAM->GetSpecialMode()) 
						{
							if (m_pTestMoveInfoExplorer != NULL)
							{
								m_pTestMoveInfoExplorer->Stop();

								g_pSunTerrain->PathExp_Reset(m_pTestMoveInfoExplorer, const_cast<WzVector*>(&rMsg.m_wvDestPos), -1);		
								if (m_pTestMoveInfoExplorer->GetTile() == -1)
								{
	#ifdef _DEBUG
									char szTemp[256];
									Sprintf(szTemp, "서버에서 보내준 목적지좌표 이상");
									GlobalFunc::OutputMsg_SysMsg(szTemp);
	#endif //_DEBUG
								}
							}

							RegisterMoveInfo(action.MOVE.vCurPos, action.MOVE.vDestPos, pChr->GetObjectKey(), FALSE);
		
						}
					}
				}
				
				break;
			}
			break;

		case SYNC_MOVE_THRUST_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_MOVE_THRUST_BRD );

				if (g_pHero) 
				{
					if (rMsg.m_dwObjectKey == g_pHero->GetObjectKey())
					{
						assert(rMsg.m_dwObjectKey != g_pHero->GetObjectKey());
						//GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("MSG_CG_SYNC_MOVE_THRUST_BRD 자기 자신의 이동패킷을 받았습니다."));
						continue;
					}
				}
			
				Character * pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_dwObjectKey);

				if (pChr != NULL)
				{
					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_MOVE;
					action.MOVE.byState = rMsg.m_byState;
					action.MOVE.bThrust = TRUE;
					action.MOVE.vCurPos.wvPos = pChr->GetPosition();						
					action.MOVE.vCurPos.wTile = rMsg.m_wTileIndex;
					action.MOVE.vDestPos.wvPos = rMsg.m_wvDestPos;
					action.MOVE.vDestPos.wTile = -1;


					if (GENERALPARAM->GetSpecialMode()) 
					{
						if (pChr->IsKindOfObject(MONSTER_OBJECT)) 
						{
							RegisterMoveInfo(action.MOVE.vCurPos,action.MOVE.vDestPos,pChr->GetObjectKey(),TRUE);
						}
					}
					
					if (GENERALPARAM->IsServerDebug()) 
					{
						pChr->DoAction(&action);	
						pChr->CommitChangeState(g_CurTime);	
					}
					else
					{
						pChr->PutAction(&action);
					}
				}
			}
			break;
		case SYNC_TARGET_MOVE_BRD:				
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_TARGET_MOVE_BRD );

				if( g_pHero && g_pHero->GetObjectKey() == rMsg.m_wPlayerKey )
					continue;

				assert(g_pHero && rMsg.m_wPlayerKey != g_pHero->GetObjectKey());
				Character * pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_wPlayerKey);
				
				if (pChr != NULL)
				{
					pChr->ReleaseMoveTarget();

					Character * pTarget = (Character *)g_ObjectManager.GetObject(rMsg.m_dwTargetKey);
					if (pTarget)
					{
						pChr->SetMoveTarget(rMsg.m_dwTargetKey);					
						pTarget->AddTargetingCount(pChr->GetObjectKey());
					}

					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_MOVE;
					action.MOVE.byState = CMS_RUN;
					action.MOVE.bThrust = FALSE;
					action.MOVE.vCurPos.wvPos = rMsg.m_wvCurPos;
					action.MOVE.vCurPos.wTile = -1;
					action.MOVE.vDestPos.wvPos = rMsg.m_wvDestPos;
					action.MOVE.vDestPos.wTile = -1;
					
					if (GENERALPARAM->IsServerDebug()) 
					{
						pChr->DoAction(&action);	
						pChr->CommitChangeState(g_CurTime);	
					}
					else
					{
						pChr->PutAction(&action);
					}

					if (pChr->IsKindOfObject(MONSTER_OBJECT)) 
					{
						if (GENERALPARAM->GetSpecialMode()) 
						{
							RegisterMoveInfo(action.MOVE.vCurPos,action.MOVE.vDestPos,pChr->GetObjectKey(),FALSE);
						}
					}
				}
				break;
			}
			break;
		case SYNC_KBMOVE_BRD:						
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_KBMOVE_BRD );

				if (g_pHero) 
				{
					if (rMsg.m_wPlayerKey == g_pHero->GetObjectKey())
					{
						continue;
					}
				}

				Player * pPlayer = (Player *)g_ObjectManager.GetObject(rMsg.m_wPlayerKey);
				
				if (pPlayer != NULL)
				{
					pPlayer->SetKeyboardMoveTime(0);
					pPlayer->ReleaseMoveTarget();

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                    if( pPlayer->GetCurState() == STATE::SKILL )
                    {
                        if( pPlayer->GetCurSkillState()->GetSkillClassCode() == eSKILL_HELLROID_1015 )
                        {
                            SkillStateHellroidSpin* pSpinState = (SkillStateHellroidSpin*)pPlayer->GetCurSkillState();
                            pSpinState->RecvKeyboardMovePacket( pPlayer, rMsg.m_byMoveState, rMsg.m_wvCurPos, (rMsg.m_wAngle * WZ_PI / 180.0f) );

                            break;
                        }
                    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID


					PLAYER_ACTION action;

					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_KEYBOARDMOVE;
					action.KEYBOARDMOVE.byKeyboardState = rMsg.m_byMoveState;
					action.KEYBOARDMOVE.vKeyboardCurPos.wvPos = rMsg.m_wvCurPos;
					action.KEYBOARDMOVE.vKeyboardCurPos.wTile = -1;

					action.KEYBOARDMOVE.fKeyboardAngle = (rMsg.m_wAngle * WZ_PI / 180.0f);
					
					if (GENERALPARAM->IsServerDebug()) 
					{
						pPlayer->DoAction(&action);	
						pPlayer->CommitChangeState(g_CurTime);	
					}
					else
					{
						pPlayer->PutAction(&action);
					}
				}

			}
			break;
		case SYNC_NPC_JUMP_BRD:					
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_NPC_JUMP_BRD );

				if( g_pHero && g_pHero->GetObjectKey() == rMsg.m_dwObjectKey )
					continue;

				Character * pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_dwObjectKey);

				if (pChr != NULL)
				{
					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));

					if ( rMsg.m_bJumpAttack )
					{
						action.ActionID = ACTION_ATTACKJUMP;
						action.ATTACKJUMP.dwAttackSerial = pChr->GetNextAttackSerial();
						action.ATTACKJUMP.vCurPos.wvPos  = pChr->GetVisiblePos();
						action.ATTACKJUMP.vCurPos.wTile  = -1;

						action.ATTACKJUMP.vDestPos.wvPos = rMsg.m_wvEndPos;                                 
						action.ATTACKJUMP.vDestPos.wvPos.z += 2.5f;
						if ( !GameFunc::GetVisiblePosition( &action.ATTACKJUMP.vDestPos.wvPos ) )
						{
							action.ATTACKJUMP.vDestPos.wvPos ;
						}
						action.ATTACKJUMP.vDestPos.wTile = -1;

						float fDistance;
						WzVector vDiff = 
							action.ATTACKJUMP.vDestPos.wvPos - action.ATTACKJUMP.vCurPos.wvPos;
						fDistance = VectorLength(&vDiff);

						action.ATTACKJUMP.dwStartTime = g_CurTime;
	                    
						pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_MONSTER_ATTACKJUMP) );
						float fMoveSpeed =  pChr->GetMoveSpeed() * pChr->GetMoveSpeedModifier();
						DWORD dwDeltaTime = DWORD( fDistance / fMoveSpeed );
						action.ATTACKJUMP.dwEndTime = g_CurTime + dwDeltaTime;

						if (GENERALPARAM->IsServerDebug()) 
						{
							pChr->DoAction(&action);	
							pChr->CommitChangeState(g_CurTime);	
						}
						else
						{
							pChr->PutAction(&action);
						}
					}
					else
					{
						action.ActionID = ACTION_EVENTJUMP;
						action.EVENT_JUMP.vCurPos.wvPos  = pChr->GetVisiblePos();
						action.EVENT_JUMP.vCurPos.wTile  = -1;
						WzVector wvEndPos = rMsg.m_wvEndPos;
						wvEndPos.z += 1.5f;
						GameFunc::GetVisiblePosition( &wvEndPos );
						action.EVENT_JUMP.vDestPos.wvPos  = wvEndPos;			                    
						action.EVENT_JUMP.vDestPos.wTile = -1;

						float fDistance;
						WzVector vDiff = 
							action.EVENT_JUMP.vDestPos.wvPos - action.EVENT_JUMP.vCurPos.wvPos;
						fDistance = VectorLength(&vDiff);
						action.EVENT_JUMP.dwStartTime = g_CurTime;
						action.EVENT_JUMP.dwEndTime = g_CurTime + (DWORD)(60.0f * fDistance);

						if (GENERALPARAM->IsServerDebug()) 
						{
							pChr->DoAction(&action);	
							pChr->CommitChangeState(g_CurTime);	
						}
						else
						{
							pChr->PutAction(&action);
						}
					}                            
				}
				break;

			}
			break;

		case SYNC_STOP_BRD:						
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_STOP_BRD );

				Character * pChr = (Character *)g_ObjectManager.GetObject( rMsg.m_dwObjectKey );

				if( pChr != NULL )
				{
					DWORD dwPlayerAtt = pChr->GetCurrentTileAtt( TRUE );

					if( BIT_CHECK( dwPlayerAtt, PTA_PLAYER_FALLDOWN_TILE ) )
					{
						if( pChr->GetCurState() != STATE::JUMP && pChr->GetCurState() != STATE::KEYBOARDJUMP )
						{
							PLAYER_ACTION action;
							ZeroMemory( &action, sizeof(action) );
							action.ActionID = ACTION_FALLDOWN;
							pChr->DoAction( &action );
							pChr->CommitChangeState( g_CurTime );
						}
					}
					else
                    {
                        if( pChr->GetCurState() == STATE::SUMMON_RIDER )
                        {
                            //no op
                        }
                        else
                        {
                            if (g_pHero && rMsg.m_dwObjectKey == g_pHero->GetObjectKey())
                            {
                                pChr->StopMove();
                                pChr->SetPosition( rMsg.m_wvCurPos );
                                pChr->SetNextState(STATE::IDLE,g_CurTime);
                                pChr->CommitChangeState(g_CurTime);
                            }
                            else
                            {
                                PLAYER_ACTION action;
                                ZeroMemory(&action,sizeof(action));

                                action.ActionID = ACTION_STOP;
                                action.STOP.vCurPos.wvPos = rMsg.m_wvCurPos;
                                action.STOP.vCurPos.wTile = -1;

                                pChr->DoAction(&action);
                                pChr->CommitChangeState(g_CurTime);
                            }
                        }
					}
				}

			}
			break;
		case SYNC_FAST_KBMOVE_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_FAST_KBMOVE_BRD );

				if( g_pHero && g_pHero->GetObjectKey() == rMsg.m_wPlayerKey )
					continue;

				Character * pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_wPlayerKey);

				if (pChr != NULL)
				{
					pChr->ReleaseMoveTarget();
					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_SPECIAL_MOVE;

					action.SPECIALMOVE.bType = rMsg.m_byCmsMoveState;
					action.SPECIALMOVE.vCurPos.wvPos = rMsg.m_wvCurPos;
					action.SPECIALMOVE.vCurPos.wTile = -1;
					action.SPECIALMOVE.fAngle = (rMsg.m_wAngle * WZ_PI / 180.0f);

					if (GENERALPARAM->IsServerDebug()) 
					{
						pChr->DoAction(&action);	
						pChr->CommitChangeState(g_CurTime);	
					}
					else
					{
						pChr->PutAction(&action);
					}
				}

			}
			break;
		case SYNC_JUMP_BRD:						
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_JUMP_BRD );

				if( g_pHero && g_pHero->GetObjectKey() == rMsg.m_PlayerKey )
					continue;

				Player * pPlayer = (Player *)g_ObjectManager.GetObject(rMsg.m_PlayerKey);

				if(pPlayer)
				{
					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_KEYBOARDJUMP;
					action.KEYBAORDJUMP.vDest=rMsg.m_wvDestPos;
					action.KEYBAORDJUMP.iJumpFlag=rMsg.m_iDir;

#ifdef _NA_20120220_JUMP_RENEWAL
#else
                    pPlayer->m_JumpData.m_JumpFlag.SetMoveFlag( rMsg.m_iDir );
#endif
					pPlayer->PutAction(&action);
				}

			}
			break;
		case SYNC_STATUS_ADD_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_STATUS_ADD_BRD );

				Character *pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_dwTargetKey);
				if (pChr)
				{
					if( SKILL_EFFECT *pEffect = SKILL_EFFECT::CreateStatusEffect( rMsg.m_wStatusCode, 0 ) )
                    {
                        pEffect->CreateEffect( pChr, pChr->GetObjectKey() );
                        pChr->AddSkillEffect(pEffect);
                    }
				}
			}
			break;
		case SYNC_STATUS_REMOVE_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_STATUS_REMOVE_BRD );

				Character *pChr = (Character *)g_ObjectManager.GetObject(rMsg.m_dwTargetKey);
				if (pChr)
				{
                    // 디펜스 모션 스킬을 사용중에, 버프를 제거하는 경우,, 모션을 종료시킨다
                    if( rMsg.m_wStatusCode == 121 )
                    {
                        if( pChr->GetCurState() == STATE::SKILL )
                        {
                            if( StateSkill* pState = pChr->GetCurSkillState() )
                            {
                                if( pState->GetSkillClassCode() == eSKILL_DEFENCE_MOTION )
                                {
                                    pState->ProcessExtraCommand();
                                }
                            }
                        }
                    }

					pChr->RemoveSkillEffectWithStatus((eCHAR_STATE_TYPE)rMsg.m_wStatusCode);
				}
			}
			break;

		case SYNC_CHARACTER_DEAD_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR(SYNC_CHARACTER_DEAD_BRD);
				
				Character* pChr = (Character*)g_ObjectManager.GetObject(rMsg.m_ObjectKey);
				if (pChr)
				{	
					pChr->SetLatestAttack(GetBattleTimeSerial());
					pChr->SetHP(0);
				}
			}
			break;

		case SYNC_CHARACTER_RESURRECTION_BRD:
			{
				RECV_PATTERNCODE_FOR_WAR( SYNC_CHARACTER_RESURRECTION_BRD );
				DWORD dwKey = rMsg.m_dwObjectKey;
				Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);

				if (pChr)
				{
					pChr->SetLatestAttack(GetBattleTimeSerial());
					pChr->SetPosition(rMsg.m_wvCurPos);
					pChr->SetHP(rMsg.m_dwHP);		
					pChr->SetMP(rMsg.m_dwMP);
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                    pChr->SetSD(rMsg.m_dwSD);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
					pChr->SetReserveHP( rMsg.m_dwReserveHP );


                    cTargetPosition Position;
                    Position.SetTargetVisiblePosition( pChr->GetObjectKey(), TRUE );

                    g_ProjectileManagerEx.CreateEffect( pChr->GetObjectKey(), StrToWzID("A022"), 5000, Position );
				}

				if (pChr == g_pHero)
				{
                    DoSomethingPacket(SYNC_CHARACTER_RESURRECTION_BRD);
					g_InterfaceManager.GetInterfaceString(5086, szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					SetBrightDelay(750);
					this->m_bDeadFlag = FALSE;
				}
			}
			break;
        case SYNC_REGENID_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR( SYNC_REGENID_BRD );

                Object* object = g_ObjectManager.GetObject(rMsg.m_dwObjectKey);
                if (object != NULL)
                {
                    if (object->IsKindOfObject(MONSTER_OBJECT))
                    {
                        Monster* monster_object = static_cast<Monster*>(object);
                        monster_object->set_unitid_for_trigger(rMsg.m_dwRegenID);
                        DWORD animation_code = rMsg.m_dwAnimationCode;
                        if (animation_code != 0)
                        {
                            // 해당 애니메이션을 플레이하도록 액션을 삽입한다.
                            PLAYER_ACTION action;
                            action.ActionID = ACTION_TRIGGER_ANIMATION_CONTROL;
                            action.TRIGGERANIMATIONCONTROL.start_animation_id = animation_code;
                            action.TRIGGERANIMATIONCONTROL.start_animation_loop = 0;

                            monster_object->PutAction(&action);
                        }
                    }
                    else if (object->IsKindOfObject(NPC_OBJECT))
                    {
                        NPC* npc_object = static_cast<NPC*>(object);
                        npc_object->set_unitid_for_trigger(rMsg.m_dwRegenID);
                        DWORD animation_code = rMsg.m_dwAnimationCode;
                        if (animation_code != 0)
                        {
                            // 해당 애니메이션을 플레이하도록 액션을 삽입한다.
                            PLAYER_ACTION action;
                            action.ActionID = ACTION_TRIGGER_ANIMATION_CONTROL;
                            action.TRIGGERANIMATIONCONTROL.start_animation_id = animation_code;
                            action.TRIGGERANIMATIONCONTROL.start_animation_loop = 0;

                            npc_object->PutAction(&action);
                        }
                    }
                }                
            }
            break;

        //------------------------------------------------------------------------------ 
        //! 2011.1.10 / i4u4me / 속성 리뉴얼 되면서 추가/변경된 패킷 
        case BATTLE_MONSTER_ATTACK_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(BATTLE_MONSTER_ATTACK_BRD);
                OnRecv_MSGSUB_BATTLE_MONSTER_ATTACK_BRD(&rMsg);
            }
            break;
        case BATTLE_MONSTER_JUMP_ATTACK_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(BATTLE_MONSTER_JUMP_ATTACK_BRD);
                OnRecv_MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD(&rMsg);
            }
            break;
        case BATTLE_PLAYER_ATTACK_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(BATTLE_PLAYER_ATTACK_BRD);
                OnRecv_MSGSUB_BATTLE_PLAYER_ATTACK_BRD((MSGSUB_BATTLE_PLAYER_ATTACK_BRD*)(&rMsg));
            }
            break;
        case BATTLE_PLAYER_AREA_ATTACK_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(BATTLE_PLAYER_AREA_ATTACK_BRD);
                OnRecv_MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD(&rMsg);
            }
            break;
        case STYLE_PLAYER_ATTACK_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(STYLE_PLAYER_ATTACK_BRD);
                OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_BRD(&rMsg);
            }
            break;
        case STYLE_PLAYER_ATTACK_RESULT_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(STYLE_PLAYER_ATTACK_RESULT_BRD);
                OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD((MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD*)(&rMsg));
            }
            break;
        case SKILL_ACTION_INSTANT_RESULT_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_ACTION_INSTANT_RESULT_BRD);

                BYTE *pPacket = (BYTE*)(&rMsg);
                pPacket += (rMsg.GetSize() - rMsg.sizeof_additions);

                OnRecv_MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD((MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD*)(&rMsg), pPacket);
            }
            break;
        case SKILL_ACTION_DELAY_START_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_ACTION_DELAY_START_BRD);
                OnRecv_MSGSUB_SKILL_ACTION_DELAY_START_BRD((MSGSUB_SKILL_ACTION_DELAY_START_BRD*)(&rMsg));
            }
            break;
        case SKILL_ACTION_DELAY_RESULT_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_ACTION_DELAY_RESULT_BRD);

                BYTE* pPacket = (BYTE*)(&rMsg);
                pPacket += (rMsg.GetSize() - rMsg.sizeof_additions);

                OnRecv_MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD((MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD*)(&rMsg), pPacket);
            }
            break;
        case SKILL_CASTING_CANCEL_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_CASTING_CANCEL_BRD);
                OnRecv_MSGSUB_SKILL_CASTING_CANCEL_BRD(&rMsg);
            }
            break;
        case SKILL_PERIODIC_DAMAGE_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_PERIODIC_DAMAGE_BRD);
                OnRecv_MSGSUB_SKILL_PERIODIC_DAMAGE_BRD(&rMsg);
            }
            break;
        case SKILL_REFLECT_DAMAGE_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_REFLECT_DAMAGE_BRD);
                OnRecv_MSGSUB_SKILL_REFLECT_DAMAGE_BRD(&rMsg);
            }
            break;
        case SKILL_EFFECT_BOMB_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_EFFECT_BOMB_BRD);
                OnRecv_MSGSUB_SKILL_EFFECT_BOMB_BRD(&rMsg);
            }
            break;
        case SKILL_SELF_DESTRUCTION_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_SELF_DESTRUCTION_BRD);
                OnRecv_MSGSUB_SKILL_SELF_DESTRUCTION_BRD(&rMsg);
            }
            break;
        case SKILL_TRANSPARENT_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_TRANSPARENT_BRD);
                OnRecv_MSGSUB_SKILL_TRANSPARENT_BRD(&rMsg);
            }
            break;
        case SKILL_SUCTION_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_SUCTION_BRD);
                OnRecv_MSGSUB_SKILL_SUCTION_BRD(&rMsg);
            }
            break;
        case SKILL_VITAL_SUCTION_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SKILL_VITAL_SUCTION_BRD);
                OnRecv_MSGSUB_SKILL_VITAL_SUCTION_BRD(&rMsg);
            }
            break;

        case COLLECT_ACTION_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(COLLECT_ACTION_BRD);
                OnRecv_MSGSUB_COLLECT_ACTION_BRD(&rMsg);
            }
            break;
        case COLLECT_CANCEL_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(COLLECT_CANCEL_BRD);
                OnRecv_MSGSUB_COLLECT_CANCEL_BRD(&rMsg);
            }
            break;
        case SYNC_SERVER_TIMER_SYNCHRONIZE_BRD:
            {
                RECV_PATTERNCODE_FOR_WAR(SYNC_SERVER_TIMER_SYNCHRONIZE_BRD);
                ServerTimerSynchronier::Instance()->CheckServerTimerSynchronizationTest(rMsg);
            }
            break;
		default:
			{
				char buff[512];
				sprintf( buff, "Can't Readable SSQ Msg Format (Tag = %d)\n ", pIt->_TAG );
				ASSERT( !"Can't Readable SSQ Msg Format" );
			}
			break;
		}

	}

	ASSERT( pIt==pEnd );
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvVkrAttack(DWORD	dwAttackerKey,DWORD	dwClientSerial,DWORD dwTargetKey,DAMAGETYPE	wDamage,DWORD dwTargetHP,BYTE byEffect)
{
	Character *pChr = (Character *)g_ObjectManager.GetObject(dwAttackerKey);
	
	// 자신이 한 공격이 아니면
	if (g_pHero && (g_pHero->GetObjectKey() != dwAttackerKey))
	{
		// 빠방~
		if (pChr)
		{
			Object *pTarget = g_ObjectManager.GetObject(dwTargetKey);
			// 타겟이 존재한다면
			if (pTarget)
			{
				// 키보드 이동중이 아니면 그 방향으로 허리 꺽는다
				if (pChr->GetCurState() != STATE::KEYBOARDMOVE)
				{
					WzVector vPos = pTarget->GetPosition();
					pChr->SetDirection(vPos,TRUE);
				}

				pChr->LockOn(dwTargetKey);
			}

			pChr->ValkyrieShot(dwClientSerial);
		}
	}

	// 공격 결과 처리
	ATTACK_RESULT attackresult;
	attackresult.SetAttackSerial(dwClientSerial);
	attackresult.SetTimeSerial(GetBattleTimeSerial());
	attackresult.SetWeaponSound(GameFunc::GetWeaponSoundKind((eWEAPONTYPE)pChr->GetWeaponKind()));

	attackresult.GetTargetData().SetTargetObjectKey( dwTargetKey );
	attackresult.GetTargetData().SetDamage( wDamage );
	attackresult.GetTargetData().SetTargetHP( dwTargetHP );

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    if (pChr && pChr->IsKindOfObject(PLAYER_OBJECT))
    {
        attackresult.GetTargetData().SetTargetSD(pChr->GetSD());
    }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	if (attackresult.GetTargetData().GetDamage() == 0)
	{
		attackresult.GetTargetData().SetMiss(TRUE);
	}

	int targethp = GetCharacterHP(dwTargetKey);


	if (byEffect & SKILL_EFFECT_CRITICAL)
	{
		attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL );
	}	

	if(byEffect & SKILL_EFFECT_DOUBLEDAMAGE)
	{
		attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOUBLE );
	}	


	if(byEffect & SKILL_EFFECT_ETHERDEVICE_METAL)
	{
		attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL );
	}
	if(byEffect & SKILL_EFFECT_ETHERDEVICE_ETHER)
	{
		attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER );
	}


	DAMAGETYPE firstDamage = (attackresult.GetTargetData().GetDamage() / 2);
	DAMAGETYPE lastDamage = attackresult.GetTargetData().GetDamage() - firstDamage;
	// 2연발이니 2개로 나누어넣는다.
	if (pChr) 
	{
		attackresult.GetTargetData().SetDamage( firstDamage );
		attackresult.GetTargetData().SetTargetHP( dwTargetHP + lastDamage );
		attackresult.SetGroupID(1);
		pChr->AddAttackResult(&attackresult);


		attackresult.GetTargetData().SetDamage( lastDamage );
		attackresult.GetTargetData().SetTargetHP( dwTargetHP );
		attackresult.SetGroupID(2);
		pChr->AddAttackResult(&attackresult);
	

		// 플레이어가 쏜 화살에대한 결과가 늦게온것이면 바로처리.			
		if (pChr->GetValkyrieShotSerial() != dwClientSerial)
		{
			pChr->ProcessAttackResult(dwClientSerial,TRUE);
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_BATTLE( MSG_BASE * pMsg )
{
	AddBattleTimeSerial();

	TCHAR	szMessage[MAX_MESSAGE_LENGTH];

	Object * pObject = NULL;
	Object * pTargetObject = NULL;
	Player	* pPlayer = NULL;
	Monster * pMonster = NULL;

	switch( pMsg->m_byProtocol )
	{
    case CG_ARRGO_DISPLAY_CMD:
        {
            uiTargetAggro* target_aggro_dialog = GET_CAST_DIALOG(uiTargetAggro, IM_TARGET_MANAGER::TARGET_AGGRO);
            if (target_aggro_dialog)
            {
                target_aggro_dialog->NetworkProc(pMsg);
            }
        }
        break;

	case CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT3( pMsg, 0 );
		}
		break;

	case CG_BATTLE_MONSTER_STATUS_RESIST_BRD:
		{
			MSG_CG_BATTLE_MONSTER_STATUS_RESIST_BRD	*pRecvPacket = (MSG_CG_BATTLE_MONSTER_STATUS_RESIST_BRD *)pMsg;

			Object *pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwMonsterObjKey);
			if( pObject &&
				pObject->IsKindOfObject(MONSTER_OBJECT)&&
				pRecvPacket->m_ScriptIndex<BASE_NPCINFO::MAX_RESISTANCE_NUM)
			{
				Monster *pMon=(Monster *)pObject;

				BASE_NPCINFO *pInfo=pMon->GetMonsterInfo();

				if(pInfo)
				{
					GlobalFunc::CreateEffectBeUpToOption(pInfo->m_ResistanceCondition[pRecvPacket->m_ScriptIndex].m_wzidEffectCode.id,pMon->GetVisiblePos(),pMon->GetObjectKey());

				}
			}

		}
		break;


	case CG_BATTLE_PLAYER_AREA_ATTACK_BRD:
		{
			MSG_CG_BATTLE_PLAYER_AREA_ATTACK_BRD	*pRecvPacket = (MSG_CG_BATTLE_PLAYER_AREA_ATTACK_BRD *)pMsg;

            MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD pk;
            pk.m_dwClientSerial = pRecvPacket->m_dwClientSerial;
            pk.m_dwAttackerKey = pRecvPacket->m_dwAttackerKey;
            pk.m_dwPrimaryTargetKey = pRecvPacket->m_dwPrimaryTargetKey;
            pk.m_wvCurPos = pRecvPacket->m_wvCurPos;
            pk.m_StyleCode = pRecvPacket->m_StyleCode;
            pk.m_byAttackType = pRecvPacket->m_byAttackType;
            pk.m_byNumberOfTargets = pRecvPacket->m_byNumberOfTargets;
            memcpy(pk.AttackInfo, pRecvPacket->AttackInfo, sizeof(pk.AttackInfo));

            OnRecv_MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD(&pk);

		}
		break;

	case CG_BATTLE_PLAYER_ATTACK_NAK:
		{
			MSG_CG_BATTLE_PLAYER_ATTACK_NAK	*pRecvPacket = (MSG_CG_BATTLE_PLAYER_ATTACK_NAK *)pMsg;

			if (g_pHero)
			{
				g_pHero->SetCannotMoveFlag(FALSE);
				// 테스트
				g_TempVariable[1]++;
			}

			if(RC_BATTLE_ALLIENCE_GUILD_CHARACTER == pRecvPacket->m_byErrorCode)
			{
				//6180 동맹을 맺으면 공격이 가능하지 않습니다.
				g_InterfaceManager.GetInterfaceString(6180, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
			else
			{
                if (pRecvPacket->m_byErrorCode == RC_BATTLE_OUT_OF_RANGE)
                {
                    if (g_pHero != NULL)
                    {
                        g_pHero->SendStopMovePacket();
                    }
                }
                else
                {
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
                    if (g_pHero != NULL)
                    {
                        g_pHero->PacketNak.is_recv_nak = true;
                        g_pHero->PacketNak.delay_tick = clock_function::GetTickCount();
                    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
                }

				//	플레이어의 공격이 실패했습니다
				if (GENERALPARAM->GetSpecialMode()) 
				{
					g_InterfaceManager.GetInterfaceString( eST_PLAYER_ATTACK_MISS, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
			}
		}
		break;

    case CG_BATTLE_PLAYER_ATTACK_BRD:
        {
            MSG_CG_BATTLE_PLAYER_ATTACK_BRD *pRecvPacket = (MSG_CG_BATTLE_PLAYER_ATTACK_BRD*)pMsg;

            MSGSUB_BATTLE_PLAYER_ATTACK_BRD pk;
            pk.dwAttackerKey = pRecvPacket->dwAttackerKey;
            pk.byAttackType = pRecvPacket->byAttackType; 
            pk.StyleCode = pRecvPacket->StyleCode;
            pk.dwClientSerial = pRecvPacket->dwClientSerial; 
            pk.wvCurPos = pRecvPacket->wvCurPos; 
            pk.dwTargetKey= pRecvPacket->dwTargetKey ; 
            pk.wDamage = pRecvPacket->wDamage;
            pk.dwTargetHP = pRecvPacket->dwTargetHP; 
            pk.byEffect = pRecvPacket->byEffect; 
            pk.byEtherComboCount = pRecvPacket->byEtherComboCount;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            pk.target_sd = 0;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

            OnRecv_MSGSUB_BATTLE_PLAYER_ATTACK_BRD(&pk);

        }
        break;

	case CG_BATTLE_MONSTER_ATTACK_CMD:	// 몬스터가 플레이어 공격
		{
			MSG_CG_BATTLE_MONSTER_ATTACK_CMD*pRecvPacket = (MSG_CG_BATTLE_MONSTER_ATTACK_CMD*)pMsg;

            MSGSUB_BATTLE_MONSTER_ATTACK_BRD pk;
            pk.m_dwObjectKey =          pRecvPacket->m_dwObjectKey;
            pk.m_dwTargetObjectKey =    pRecvPacket->m_dwTargetObjectKey;
            pk.m_byAttackType =         pRecvPacket->m_byAttackType;
            pk.m_wvCurPos =             pRecvPacket->m_wvCurPos;
            pk.m_wDamage =              pRecvPacket->m_wDamage;
            pk.m_wTargetHP =            pRecvPacket->m_wTargetHP;
            pk.m_wTargetReserveHP =     pRecvPacket->m_wTargetReserveHP;
            pk.m_byEffect =             pRecvPacket->m_byEffect;

            OnRecv_MSGSUB_BATTLE_MONSTER_ATTACK_BRD(&pk);

		}
		break;

	case CG_BATTLE_VKR_ATTACK_BRD:
		{
			MSG_CG_BATTLE_VKR_ATTACK_BRD	*pRecvPacket = (MSG_CG_BATTLE_VKR_ATTACK_BRD *)pMsg;

			OnRecvVkrAttack(pRecvPacket->dwAttackerKey,pRecvPacket->dwClientSerial,pRecvPacket->dwTargetKey,pRecvPacket->wDamage,pRecvPacket->dwTargetHP,pRecvPacket->byEffect);
		}
		break;

	case CG_BATTLE_VKR_RELOAD_BRD:
		{
			MSG_CG_BATTLE_VKR_RELOAD_BRD	*pRecvPacket = (MSG_CG_BATTLE_VKR_RELOAD_BRD *)pMsg;

			if (g_pHero) 
            {
				if (pRecvPacket->m_dwObjKey == g_pHero->GetObjectKey())
				{
					g_pHero->SetBulletCount(20);
					break;
				}
			}

			Character *pChr = (Character *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);
			if (pChr)
			{
				assert(pChr->IsKindOfObject(PLAYER_OBJECT));

				Player *pPlayer = (Player *)pChr;

				pPlayer->ValkyrieReload();
				pPlayer->SetBulletCount(20);
			}			
		}
		break;

    case CG_AGGRO_VALUES_CMD:
        {
            MSG_CG_AGGRO_VALUES_CMD* recieve_message = (MSG_CG_AGGRO_VALUES_CMD*) pMsg;

            //aggro_player_count = recieve_message->aggro_player_count_;
            //if (aggro_player_count > 0)
            //{
            //    for (int i = 0; i < aggro_player_count; ++i)
            //    {
            //        if (aggro_value_info->character_name_ != '\0')
            //        {
            //            sprintf(aggro_value_info[i].character_name_, recieve_message->aggor_info_[i].character_name_);
            //        }
            //        else
            //        {
            //            sprintf(aggro_value_info[i].character_name_, "null name"); //do not recieve character name
            //        }
            //        aggro_value_info[i].aggro_value_ = recieve_message->aggor_info_[i].aggro_value_;
            //    }
            //}
        }
        break;


	case CG_BATTLE_CHARGE_ETHER_BULLET_BRD:
		{
			MSG_CG_BATTLE_CHARGE_ETHER_BULLET_BRD * pRecvMsg = (MSG_CG_BATTLE_CHARGE_ETHER_BULLET_BRD *) pMsg;

			Player * pPlayer = (Player * ) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
			if( !pPlayer)
				return;

			assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
			if( pPlayer->IsKindOfObject(PLAYER_OBJECT) == FALSE )
				return;
			
			SCSlotContainer * pEquipContainer = pPlayer->GetSlotContainer(SI_EQUIPMENT);
			if( pEquipContainer )
			{
				if( pEquipContainer->IsEmpty(EquipmentContainer::EQUIP_WEAPON1) == FALSE)
				{
					SCItemSlot & rItemSlot = (SCItemSlot &)pEquipContainer->GetSlot(EquipmentContainer::EQUIP_WEAPON1);
					rItemSlot.SetEtherBulletIndex( pRecvMsg->m_BulletIndex);
					pPlayer->CreateEtherWeaponEffect(false,TRUE);
				}
				else
				{
					assert(!"총알을충전했는데이큅창에 무기가 없어서 충전을 못했다");
				}
			}
		}
		break;

	case CG_BATTLE_REMOVE_ETHER_BULLET_BRD:
		{
			MSG_CG_BATTLE_REMOVE_ETHER_BULLET_BRD * pRecvMsg = (MSG_CG_BATTLE_REMOVE_ETHER_BULLET_BRD *) pMsg;
			Player * pPlayer = (Player * ) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);

			if( !pPlayer )
				return;

			assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
			if( pPlayer->IsKindOfObject(PLAYER_OBJECT) == FALSE )
				return;

			SCSlotContainer * pEquipContainer = pPlayer->GetSlotContainer(SI_EQUIPMENT);
			if( pEquipContainer )
			{
				if( pEquipContainer->IsEmpty(EquipmentContainer::EQUIP_WEAPON1) == FALSE)
				{
					SCItemSlot & rItemSlot = (SCItemSlot &)pEquipContainer->GetSlot(EquipmentContainer::EQUIP_WEAPON1);
					rItemSlot.SetEtherBulletIndex(0);
					pPlayer->CreateEtherWeaponEffect(TRUE);//<- 샷이펙트 
				}
				else
				{
					assert(!"총알을쐈는데이큅창에 무기가 없어서 지우지 못했다");
				}
			}
		}
		break;
    case CG_BATTLE_DETAIL_INFORMATION_CMD:
        {
#ifdef _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO
  #ifdef _DEV_VER
            MSG_CG_BATTLE_DETAIL_INFORMATION_CMD *pRecvMsg = (MSG_CG_BATTLE_DETAIL_INFORMATION_CMD *) pMsg;

            if (TestModule::Instance()->GetTestMode(TestModule::TestMode_Show_Damage_info))
            {
                TCHAR skill_name[MAX_SKILLNAME_LENGTH+1] = {0,};
                SkillScriptInfo* base_skill_info = SkillInfoParser::Instance()->GetSkillInfo(pRecvMsg->skill_code);
                if (base_skill_info)
                {
                    StrnCopy(skill_name, base_skill_info->m_szSkillName, MAX_SKILLNAME_LENGTH);
                }
                
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                Snprintf(string, INTERFACE_STRING_LENGTH, "[%s -> %s] 데미지:%d (%s)",
                    pRecvMsg->attacker_name, pRecvMsg->target_name, pRecvMsg->damage, skill_name);
                GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, string);

                TCHAR path[MAX_PATH];
                GetCurrentDirectory( MAX_PATH, path);
                strcat(path, "\\damage_info_log.txt");
                if( FILE* fp = fopen(path, "a") )
                {
                    struct tm * today;
                    time_t		ltime;
                    time( &ltime );
                    today = localtime( &ltime );

                    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
                    sprintf(buffer,"[%02d:%02d:%02d] \t%s\t%s\t%d\t%s",
                        today->tm_hour, today->tm_min, today->tm_sec, pRecvMsg->attacker_name, pRecvMsg->target_name, pRecvMsg->damage, skill_name);

                    fprintf(fp, "%s\n", buffer);
                    fclose(fp);
                }
            }
  #endif //_DEV_VER
#endif //_NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO
        }
        break;

	}
}

//------------------------------------------------------------------------------
/**
    GM 카테고리 처리 
    Todo:화면에 정보 표시 기획 받으면 수정 일단은 채팅창으로~ 지완 
*/
void BattleScene::OnRecvCG_GM( MSG_BASE * pMsg )
{
    ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_CHAT );

	TCHAR	szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];
    switch( pMsg->m_byProtocol )
    {


	case CG_GM_VERSION_ACK: 
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			MSG_CG_GM_VERSION_ACK	*pRecvPacket = (MSG_CG_GM_VERSION_ACK *)pMsg;

		
			if(pRecvPacket->m_byErrorCode==RC_GM_CMD_SUCCESS)
			{
				switch(pRecvPacket->m_byVersionType)
				{
				case VERSION_SCRIPT:
					{
                        if(g_pClientScriptParserManager)
                        {
							SCRIPT_DATA *pScript=g_pClientScriptParserManager->FindScript( (eSCRIPT_CODE)pRecvPacket->m_byScriptCode );
							if(pScript)
							{						
								Sprintf(szMessage , _T("%s Sever_Ver:%s") ,pScript->m_szFileName,pRecvPacket->m_szVersion);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

								Sprintf(szMessage , _T("%s Client_Ver:%s") ,pScript->m_szFileName,pScript->m_szVersion);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);


							}
							else 
							{	
								if (GENERALPARAM->GetSpecialMode())
								{
									// 해당 스크립트 파일이 존재하지 않습니다.
									g_InterfaceManager.GetInterfaceString(eST_RC_GM_ISNOT_SCRIPT_FILE, szMessage, INTERFACE_STRING_LENGTH);	
									GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
								}

							}
                        }
					}
					break;

				case VERSION_BUILD:
					{
						Sprintf(szMessage , _T("Build_Sever_Ver: %s") ,pRecvPacket->m_szVersion);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

						Sprintf(szMessage , _T("Build_Client_Ver: %d"),NATION_BUILD_VERSION);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
                        
#if defined(__APPLY_HACKSHIELD)
                        Sprintf(szMessage , _T("HackShield_Ver: %d"),HACKSHIELD_VERSION);
#elif defined(__APPLY_XIGNCODE)
                        Sprintf(szMessage , _T("XIGNCODE_Ver: %d"),XIGNCODE_VERSION);
#else
                        Sprintf(szMessage , _T("USING_FROST"));
#endif
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

					}
					break;

				case VERSION_PROTOCOL:
					{
						Sprintf(szMessage , _T("Protocol_Server_Ver: %s") ,pRecvPacket->m_szVersion);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

						Sprintf(szMessage , _T("Protocol_Client_Ver: %d.%d.%d") , C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO,C2S_LOW_VERSION_NO);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
					break;

				case VERSION_COMPILE:
					{
						Sprintf(szMessage , _T("Compile_Server_Ver: %s") ,pRecvPacket->m_szVersion);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

						Sprintf(szMessage , _T("Compile_Client_Ver: %s") ,c_tcBUILD_VERSION);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

					}
					break;
				
				case VERSION_ENGINE:
					{
						Sprintf(szMessage , _T("Engine_Server_Ver: %s") ,pRecvPacket->m_szVersion);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

						Sprintf(szMessage , _T("Engine_Client_Ver: %d"),WZENGINEVER);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
					break;

				default:
					{
						Sprintf(szMessage , _T("Error versionType: %d"),pRecvPacket->m_byVersionType);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

					}
					break;

				}
				
			}
			else
			{
				RenderGMResultCode(pRecvPacket->m_byErrorCode);
			}
		}
		break;


	//	무적
	case CG_GM_UNDEAD_MODE_ACK:
		{
			MSG_CG_GM_UNDEAD_MODE_ACK	*pRecvPacket = (MSG_CG_GM_UNDEAD_MODE_ACK *)pMsg;

			if( g_pHero )
			{
				g_pHero->SetInvisible( pRecvPacket->m_bUndeadOn );
				const int	c_iInvisible = 70272;
				const int	c_iInvisibleFalse = 70273;
				if( pRecvPacket->m_bUndeadOn )
				{
					g_InterfaceManager.GetInterfaceString( c_iInvisible, szMessage, INTERFACE_STRING_LENGTH );
				}
				else
				{
					g_InterfaceManager.GetInterfaceString( c_iInvisibleFalse, szMessage, INTERFACE_STRING_LENGTH );
				}
				pDlg->AddString( szMessage );
			}
		}
		break;

	case CG_GM_UNDEAD_MODE_NAK:
		{
			//MSG_CG_GM_UNDEAD_MODE_NAK	*pRecvPacket = (MSG_CG_GM_UNDEAD_MODE_NAK *)pMsg;
		}
		break;


    case CG_GM_CHARINFO_ACK:
        {
            MSG_CG_GM_CHARINFO_ACK	*pRecvPacket = (MSG_CG_GM_CHARINFO_ACK *)pMsg;

			const int c_iClass = 10000;			
			g_InterfaceManager.GetInterfaceString( c_iClass + (pRecvPacket->m_eCharType * 2 - 1), szText, INTERFACE_STRING_LENGTH );

			Sprintf(szMessage,_T("Class : %s"), szText);
			pDlg->AddString(szMessage);
            Sprintf(szMessage,_T("Level : %d"),pRecvPacket->m_wLevel);
            pDlg->AddString(szMessage);
			Sprintf(szMessage,_T("Money : %d"),pRecvPacket->m_Money);
			pDlg->AddString(szMessage);        

            Sprintf(szMessage,_T("Room Key : %d"),pRecvPacket->m_RoomKey);
            pDlg->AddString(szMessage);

            Sprintf(szMessage,_T("Map Code : %d"),pRecvPacket->m_MapCode);
            pDlg->AddString(szMessage);

            Sprintf(szMessage,_T("Position : x=%4.4f y=%4.4f z=%4.4f"),pRecvPacket->m_vCurPos.x,pRecvPacket->m_vCurPos.y, pRecvPacket->m_vCurPos.z);
            pDlg->AddString(szMessage);
		}
        break;

		//	옵저버
	case CG_GM_OBSERVER_BRD:
		{
			MSG_CG_GM_OBSERVER_BRD	*pRecvPacket = (MSG_CG_GM_OBSERVER_BRD *)pMsg;

			Player	*pPlayer = (Player *)g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey );
			if( pPlayer )
			{
				//	On(1) or Off(0)
				BOOL	bIsHero = FALSE;
				if( g_pHero )
				{
					bIsHero = ( g_pHero->GetObjectKey() == pPlayer->GetObjectKey() );
				}
				pPlayer->SetObserverMode( (BOOL)pRecvPacket->m_byObserverOn, bIsHero );

				const int	c_iObserverModeOn = 70397;
				const int	c_iObserverModeOff = 70398;
				if( bIsHero )
				{
					if( pRecvPacket->m_byObserverOn )
					{
						//	GM 캐릭터 옵저버 모드 On
						g_InterfaceManager.GetInterfaceString( c_iObserverModeOn, szMessage, INTERFACE_STRING_LENGTH );
					}
					else
					{
						//	GM 캐릭터 옵저버 모드 Off
						g_InterfaceManager.GetInterfaceString( c_iObserverModeOff, szMessage, INTERFACE_STRING_LENGTH );
					}
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
			}
		}
		break;

    case CG_GM_OBSERVER_NAK:
        {
			//	투명 모드 설정 실패
			g_InterfaceManager.GetInterfaceString( eST_GM_OBSERVER_MODE_FAIL, szMessage, INTERFACE_STRING_LENGTH );
            pDlg->AddString(szMessage);
        }
        break;

	case CG_GM_CHARINFO_NAK:
		{
			//	캐릭터 정보 요청 실패
			g_InterfaceManager.GetInterfaceString( eST_GM_CHARINFO_REQUEST_FAIL, szMessage, INTERFACE_STRING_LENGTH );
			pDlg->AddString(szMessage);
		}
		break;

	case CG_GM_ROOMINFO_ACK:
		{
			MSG_CG_GM_ROOMINFO_ACK	*pRecvPacket = (MSG_CG_GM_ROOMINFO_ACK *)pMsg;

			TCHAR	szName[INTERFACE_STRING_LENGTH];

			//	파티장 이름
			const int	c_iPartyMaster = 644;
			g_InterfaceManager.GetInterfaceString( c_iPartyMaster, szText, INTERFACE_STRING_LENGTH );
			if( _tcsclen(pRecvPacket->m_szMasterName) == 0 )
			{
				g_InterfaceManager.GetInterfaceString( eST_NONE_TEXT, szName, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s : %s"), szText, szName );
			}
			else
			{
				Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s : %s"), szText, pRecvPacket->m_szMasterName );
			}
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

			for( int i = 0 ; i < pRecvPacket->m_byCount ; ++i )
			{
				//	플레이어 레벨, 클래스, 이름
				g_InterfaceManager.GetInterfaceString( GlobalFunc::GetClassNameCode(pRecvPacket->m_pPlayerInfo[i].m_byClass), szText, INTERFACE_STRING_LENGTH );
				Sprintf( szMessage, _T("%d %s [%s]"), pRecvPacket->m_pPlayerInfo[i].m_LV, pRecvPacket->m_pPlayerInfo[i].m_szCharName, szText );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
		}
		break;

	case CG_GM_ROOMINFO_NAK:
		{
			//	방 정보 요청 실패
		}
		break;


    case CG_GM_STAT_UP_ACK:
        {
            MSG_CG_GM_STAT_UP_ACK	*pRecvPacket = (MSG_CG_GM_STAT_UP_ACK *)pMsg;
            
            if(g_pHero)
            {
                BASE_PLAYERINFO*  pPlayInfo=g_pHero->GetCharInfo();
				if(pPlayInfo)
				{
					pPlayInfo->m_iRemainStat=pRecvPacket->m_dwRemainStat;	

                    uiCharacterStatusMan::ResetSimulationStatCount();
                    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();
				}
            }
        }
        break;

    case CG_GM_SKILLPOINT_UP_ACK:
        {
            MSG_CG_GM_SKILLPOINT_UP_ACK	*pRecvPacket = (MSG_CG_GM_SKILLPOINT_UP_ACK *)pMsg;
            
            if(g_pHero)
            {
                BASE_PLAYERINFO*  pPlayInfo=g_pHero->GetCharInfo();
				if(pPlayInfo)
				{
					pPlayInfo->m_iRemainSkill=pRecvPacket->m_dwRemainSkill;
				}
            }
        }
        break;


	case CG_GM_RESURRECTION_BRD:
		{
			MSG_CG_GM_RESURRECTION_BRD	*pRecvPacket = (MSG_CG_GM_RESURRECTION_BRD *)pMsg;

			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			if( pChr )
			{
				pChr->SetPosition(pRecvPacket->m_wvCurPos);
				pChr->SetHP( pRecvPacket->m_dwHP );
				pChr->SetMP( pRecvPacket->m_dwMP );

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pChr->GetObjectKey(), TRUE );

                g_ProjectileManagerEx.CreateEffect( pChr->GetObjectKey(), StrToWzID("A022"), 5000, Position );
			}

			if( g_pHero && pChr == g_pHero )
			{
				SetBrightDelay(750);
				this->m_bDeadFlag = FALSE;
			}
		}
		break;

	case CG_GM_RESURRECTION_NAK:
		{
			//MSG_CG_GM_RESURRECTION_NAK	*pRecvPacket = (MSG_CG_GM_RESURRECTION_NAK *)pMsg;
		}
		break;

	case CG_GM_RECOVERY_BRD:
		{
			MSG_CG_GM_RECOVERY_BRD	*pRecvPacket = (MSG_CG_GM_RECOVERY_BRD *)pMsg;

			DWORD	dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			if( pChr )
			{
				pChr->SetHP( pRecvPacket->m_dwHP );
				pChr->SetMP( pRecvPacket->m_dwMP );

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pChr->GetObjectKey(), TRUE );

                g_ProjectileManagerEx.CreateEffect( pChr->GetObjectKey(), StrToWzID("A021"), 5000, Position );
			}
		}
		break;

	case CG_GM_SET_SPEED_BRD:
		{
			MSG_CG_GM_SET_SPEED_BRD	*pRecvPacket = (MSG_CG_GM_SET_SPEED_BRD *)pMsg;

			Player *pPlayer = (Player *)g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey );

			if( pPlayer && pPlayer->GetObjectKey() == pRecvPacket->m_dwObjectKey )
			{
				//	내 캐릭터 속도( GM )
				if( g_pHero && pPlayer->GetObjectKey() == g_pHero->GetObjectKey() )
				{
					if( pRecvPacket->m_byMoveLevel == 0 )
					{
						//	원래 속도로 설정( 커스텀 스피드 플래그 끔 )
						g_pHero->SetCustomMoveSpeed( FALSE );
					}
					else
					{
						//	변화된 속도로 설정( 커스텀 스피드 플래그 설정 )
						g_pHero->SetCustomMoveSpeed( TRUE, pRecvPacket->m_fSpeedRatio );
					}

					//	GM 캐릭터 이동 속도 레벨 : %d
					const int	c_iSetMoveSpeed = 70383;
					g_InterfaceManager.GetInterfaceString( c_iSetMoveSpeed, szText, INTERFACE_STRING_LENGTH );
					Sprintf( szMessage, szText, pRecvPacket->m_byMoveLevel );
					pDlg->AddString(szMessage);
				}
				else
				{
					if( pRecvPacket->m_byMoveLevel == 0 )
					{
						//	원래 속도로 설정( 100% )
						pPlayer->SetMoveSpeedRatio( 100 );
					}
					else
					{
						//	변화된 속도로 설정
						pPlayer->SetMoveSpeedRatio( (int)(100.0f * pRecvPacket->m_fSpeedRatio) );
					}
				}
			}
		}
        break;

	case CG_GM_ENTRANCE_NAK:
		{
			//MSG_CG_GM_ENTRANCE_NAK	*pRecvPacket = (MSG_CG_GM_ENTRANCE_NAK *)pMsg;
		}
		break;


	case CG_GM_FORCE_DISCONNECT_ACK:
		{
			MSG_CG_GM_FORCE_DISCONNECT_ACK	*pRecvPacket = (MSG_CG_GM_FORCE_DISCONNECT_ACK *)pMsg;

			if( g_pHero && g_pHero->GetObjectKey() == pRecvPacket->m_dwUserKey )
			{
				const int	c_iGM_Shutdown = 70382;
				g_InterfaceManager.GetInterfaceString( c_iGM_Shutdown, szMessage, INTERFACE_STRING_LENGTH );

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage, TRUE);

			}
		}
		break;

	case CG_GM_ACCOUNT_MODE_ACK:
		{
			MSG_CG_GM_ACCOUNT_MODE_ACK	* pRecvPacket = (MSG_CG_GM_ACCOUNT_MODE_ACK *)pMsg;
			if(g_pHero)
				g_pHero->SetAccountState(pRecvPacket->m_bTurnOn);	
		}
		break;
	case CG_GM_CHANGE_ROOMTITLE_ACK:
		{
			//방제목이 변경되었습니다
			g_InterfaceManager.GetInterfaceString(eST_GM_CHANGE_ROOMTITLE_ACK,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;
	case CG_GM_CHANGE_ROOMTITLE_NAK:
		{
			//방제목 변경이 실패하였습니다
			g_InterfaceManager.GetInterfaceString(eST_GM_CHANGE_ROOMTITLE_NAK,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;
	case CG_GM_CHANNELINFO_ACK:
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			MSG_CG_GM_CHANNELINFO_ACK * pRecvPacket = (MSG_CG_GM_CHANNELINFO_ACK *)pMsg;
			
			// 현재채널접속자수: %d,헌팅룸: %d , 미션룸: %d
			g_InterfaceManager.GetInterfaceString(eST_GM_CHANNELINFO_ACK,szText,INTERFACE_STRING_LENGTH);
			Snprintf(szMessage
					,INTERFACE_STRING_LENGTH-1
					,szText
					,pRecvPacket->m_dwChannelUserNum
					,pRecvPacket->m_wHuntingRoomCount
					,pRecvPacket->m_wMissionRoomCount);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		}
		break;
	case CG_GM_CHANNELINFO_NAK:
		{
			//"채널 정보 전송 실패"
			g_InterfaceManager.GetInterfaceString(eST_GM_CHANNELINFO_NAK,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		}	
		break;

	case CG_GM_STRING_CMD_ACK:
		{
			MSG_CG_GM_STRING_CMD_ACK * pRecvPacket = ( MSG_CG_GM_STRING_CMD_ACK *) pMsg;
			g_InterfaceManager.GetInterfaceString(eST_GM_COMMAND_SUCCESS,szText,INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szText, pRecvPacket->m_szCommand);

            //////////////////////////////////////////////////////////////////////////
            //CMD가 ReloadMapObject일 경우

            if(strstr(pRecvPacket->m_szCommand, "reloadmapobject"))
            {
                Object_Map_Itr itr = g_ObjectManager.GetBegin();
                while(itr != g_ObjectManager.GetEnd() )
                {
                    Object * pObject = static_cast<Object *>(itr->second);

                    if (pObject == NULL)
                    {
                        ++itr;
                        continue;
                    }

                    if (pObject->IsKindOfObject( MAP_OBJECT ))
                    {
                        MapObject * pMapObj = (MapObject *)pObject;
                        pMapObj->InitState(STATE::IDLE,g_CurTime);
                        if (pMapObj->IsDominationObject() == FALSE)
                            pMapObj->SetShow(TRUE);
                    }

                    ++itr;
                }
 
                if (m_pTriggerMan)
                {
                    m_bTriggerInit = FALSE;
                    m_pTriggerMan->Release();
                }
                else
                {
                    assert(m_pTriggerMan);
                }
            }
            //////////////////////////////////////////////////////////////////////////

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		}
		break;

	case CG_GM_STRING_CMD_NAK:
		{
			MSG_CG_GM_STRING_CMD_NAK * pRecvPacket = ( MSG_CG_GM_STRING_CMD_NAK * ) pMsg;
			RenderGMResultCode(pRecvPacket->m_byErrorCode);
		}
		break;
	case CG_GM_PICKUP_ACK:
		{	
			MSG_CG_GM_PICKUP_ACK * pRecvPacket = ( MSG_CG_GM_PICKUP_ACK *) pMsg;
            if (pRecvPacket->m_byErrorCode != RC_GM_CMD_SUCCESS)
            {
            	RenderGMResultCode(pRecvPacket->m_byErrorCode);
            }
		}
		break;

	case CG_GM_SERVER_TIME_CMD:
		{
			MSG_CG_GM_SERVER_TIME_CMD* pRecvPacket = (MSG_CG_GM_SERVER_TIME_CMD *) pMsg;
			
			SYSTEMTIME time;
			util::YYYYMMDDHHMMSSToSYSTEMTIME(pRecvPacket->m_CurTime, time);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, "server time: %d-%d-%d  %d:%d:%d", time.wYear, time.wMonth , time.wDay, time.wHour, time.wMinute, time.wSecond);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		}
		break;

    }
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_STYLE( MSG_BASE * pMsg )
{
	AddBattleTimeSerial();

	Object * pObject = NULL;
	Object * pTargetObject = NULL;
	Player	* pPlayer = NULL;
	Monster * pMonster = NULL;


	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_QUERY_RESOURCE_CHK_T4_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT4( pMsg, 0 );
		}
		break;


	//스타일 변경 
	case CG_STYLE_SELECT_STYLE_BRD:
		{
			MSG_CG_STYLE_SELECT_STYLE_BRD	*pRecvPacket = (MSG_CG_STYLE_SELECT_STYLE_BRD *)pMsg;
			 		
			BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo(pRecvPacket->m_NewStyleCode);
			assert(pInfo);

			Object *pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);
			if( !pObject|| !pInfo)
			{
				return;
			}

			Player* pPlayer = static_cast<Player *>(pObject);

            // 용변신 일때 변신 전의 스타일을 저장해 두어야되는데, 용변신 보다 스타일이 먼저 바뀌기 때문에,
            // 여기서 이전 스타일을 저장해둠
            if ((pPlayer->GetObjectKey() == g_pHero->GetObjectKey()) &&
                (g_pHero->GetClass() == eCHAR_DRAGON))
            {
                if ((pRecvPacket->m_NewStyleCode == eSTYLE_DRAGON_TRANSFORM1) ||
                    (pRecvPacket->m_NewStyleCode == eSTYLE_DRAGON_TRANSFORM2))
                {
                    //현재 스타일을 저장
                    xGetHeroData()->SetOldAttackStyle( xGetHeroData()->GetAttackStyle() );
                }
            }

			pPlayer->SetCurrentAttackStyle(pRecvPacket->m_NewStyleCode);//조건 검사 안하고 직접 세팅한다.
			pPlayer->SetAttackSequence(eATTACK_SEQUENCE_FIRST);

			pPlayer->UpdateSkillAttr();
		}
		break;

	//스타일 변경 실패
	case CG_STYLE_SELECT_STYLE_NAK:
		{
			MSG_CG_STYLE_SELECT_STYLE_NAK	*pRecvPacket = (MSG_CG_STYLE_SELECT_STYLE_NAK *)pMsg;
			if (g_pHero) 
            {
				//예전스타일로 변경 
                if( g_pHero->CheckChangeAttackStyle(pRecvPacket->m_CurStyleCode) == TRUE )
                    g_pHero->SetCurrentAttackStyle(pRecvPacket->m_CurStyleCode);
				g_pHero->SetAttackSequence(eATTACK_SEQUENCE_FIRST);
				g_pHero->UpdateSkillAttr();

				//	스타일 변경 실패
				TCHAR szMessage[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString( eST_CHANGE_STYLE_FAIL, szMessage, INTERFACE_STRING_LENGTH );
				ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
				pChatDlg->AddString(szMessage);
                BattleScene::ShowErrorCodeSkill(pRecvPacket->m_byErrorCode);
			}
		}
		break;

	case CG_STYLE_PLAYER_ATTACK_BRD:
		{
			MSG_CG_STYLE_PLAYER_ATTACK_BRD*pRecvPacket = (MSG_CG_STYLE_PLAYER_ATTACK_BRD*)pMsg;

            MSGSUB_STYLE_PLAYER_ATTACK_BRD pk;
            pk.dwAttackerKey = pRecvPacket->dwAttackerKey;
            pk.byAttackType = pRecvPacket->byAttackType;
            pk.StyleCode = pRecvPacket->StyleCode;
            pk.dwClientSerial = pRecvPacket->dwClientSerial;
            pk.dwPrimaryTargetKey = pRecvPacket->dwPrimaryTargetKey;
            pk.wvCurPos = pRecvPacket->wvCurPos;

            OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_BRD(&pk);

		}
		break;

	case CG_STYLE_PLAYER_ATTACK_RESULT_BRD:
		{
			MSG_CG_STYLE_PLAYER_ATTACK_RESULT_BRD*pRecvPacket = (MSG_CG_STYLE_PLAYER_ATTACK_RESULT_BRD*)pMsg;

            MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD pk;
            pk.dwClientSerial = pRecvPacket->dwClientSerial;
            pk.dwAttackerKey = pRecvPacket->dwAttackerKey;
            pk.byNumberOfTargets = pRecvPacket->byNumberOfTargets;

            OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD(&pk);

		}
		break;


	//------------------------------------------------------------------------------------------
	// 스타일 링크 관련
	case CG_STYLE_LINK_ACK:
	case CG_STYLE_LINK_NAK:
	case CG_STYLE_UNLINK_ACK:
	case CG_STYLE_UNLINK_NAK:
	case CG_STYLE_LINKMOVE_ACK:
	case CG_STYLE_LINKMOVE_NAK:
		{
			ItemManager::Instance()->OnRecvStyleLink(pMsg);
		}
		break;
	}
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_SKILL( MSG_BASE * pMsg )
{
	AddBattleTimeSerial();

	Object * pObject = NULL;
	Object * pTargetObject = NULL;
	Player	* pPlayer = NULL;
	Monster * pMonster = NULL;

	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T2( pMsg, 0 );
		}
		break;


	case CG_SKILL_CASTING_CANCEL_BRD:
		{
			MSG_CG_SKILL_CASTING_CANCEL_BRD *pRecvPacket = (MSG_CG_SKILL_CASTING_CANCEL_BRD *)pMsg;

            MSGSUB_SKILL_CASTING_CANCEL_BRD pk;
            pk.m_dwObjectKey = pRecvPacket->m_dwObjectKey; 
            pk.m_SkillCode = pRecvPacket->m_SkillCode;  

            OnRecv_MSGSUB_SKILL_CASTING_CANCEL_BRD(&pk);

		}
		break;

	case CG_SKILL_CASTING_CANCEL_NAK:
		{
			//취소실패
		}
		break;

		//즉시 시킬 사용 응답 
	case CG_SKILL_ACTION_INSTANT_RESULT_BRD:
		{

			MSG_CG_SKILL_ACTION_INSTANT_RESULT_BRD *pRecvPacket = (MSG_CG_SKILL_ACTION_INSTANT_RESULT_BRD *)pMsg;

			//Todo:pPacket의 주소가 넘어 감으로 필요 없을거 같은데..확인해보자.
			MSG_CG_SKILL_ACTION_INSTANT_RESULT_BRD *pFirstPacket=pRecvPacket;

            MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD pk;
            pk.result.m_SkillCode = pRecvPacket->m_SkillCode;
            pk.result.m_dwClientSerial = pRecvPacket->m_dwClientSerial;
            pk.result.m_dwAttackerKey = pRecvPacket->m_dwAttackerKey;
            pk.result.m_dwPrimaryTargetKey = pRecvPacket->m_dwPrimaryTargetKey;
            pk.result.m_wvMainTargetPos = pRecvPacket->m_wvMainTargetPos;
            pk.result.m_wvCurPos = pRecvPacket->m_wvCurPos;
            pk.result.m_wvDestPos = pRecvPacket->m_wvDestPos;
            pk.result.m_dwAttackerHP = pRecvPacket->m_dwAttackerHP;
            pk.result.m_dwAttackerMP = pRecvPacket->m_dwAttackerMP;
            pk.result.m_byNumberOfTargets = pRecvPacket->m_byNumberOfTargets;
            pk.result.m_byNumberOfFieldEffect = pRecvPacket->m_byNumberOfFieldEffect;
            //pk.sizeof_additions = pRecvPacket->GetSize() >sizeof_additions;

            BYTE* action_list = (BYTE*)pRecvPacket;
            action_list += pRecvPacket->GetSize();

            OnRecv_MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD(&pk, action_list);
		}
		break;

	case CG_SKILL_ACTION_NAK:
		{
			MSG_CG_SKILL_ACTION_NAK *pRecvPacket = (MSG_CG_SKILL_ACTION_NAK *)pMsg;
			BattleScene::ShowErrorCodeSkill(pRecvPacket->m_byErrorCode);

#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
            // 토글 스킬 패킷펜딩
            // 스킬 사용 실패 경우에 스킬코드는 알려주지 않아서 그냥 펜딩해제
            if (g_pHero)
            {
                g_pHero->ToggleSkill::UnsetPacketPending();
            }
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING
		}
		break;

		//딜레이 스킬 사용 시작
	case CG_SKILL_ACTION_DELAY_START_BRD:
		{
			MSG_CG_SKILL_ACTION_DELAY_START_BRD *pRecvPacket = (MSG_CG_SKILL_ACTION_DELAY_START_BRD *)pMsg;

            MSGSUB_SKILL_ACTION_DELAY_START_BRD pk;
            pk.result.m_SkillCode = pRecvPacket->m_SkillCode;
            pk.result.m_dwClientSerial = pRecvPacket->m_dwClientSerial;
            pk.result.m_dwAttackerKey = pRecvPacket->m_dwAttackerKey;
            pk.result.m_dwPrimaryTargetKey = pRecvPacket->m_dwPrimaryTargetKey;
            pk.result.m_wvMainTargetPos = pRecvPacket->m_wvMainTargetPos;
            pk.result.m_wvCurPos = pRecvPacket->m_wvCurPos;
            pk.result.m_wvDestPos = pRecvPacket->m_wvDestPos;

            OnRecv_MSGSUB_SKILL_ACTION_DELAY_START_BRD(&pk);

		}
		break;

		//딜레이 스킬 사용 응답 
	case CG_SKILL_ACTION_DELAY_RESULT_BRD:
		{
			MSG_CG_SKILL_ACTION_DELAY_RESULT_BRD *pRecvPacket = (MSG_CG_SKILL_ACTION_DELAY_RESULT_BRD *)pMsg;

            MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD pk;
            pk.result.m_SkillCode = pRecvPacket->m_SkillCode;
            pk.result.m_dwClientSerial = pRecvPacket->m_dwClientSerial;
            pk.result.m_dwAttackerKey = pRecvPacket->m_dwAttackerKey;
            pk.result.m_dwPrimaryTargetKey = pRecvPacket->m_dwPrimaryTargetKey;
            pk.result.m_wvMainTargetPos = pRecvPacket->m_wvMainTargetPos;
            pk.result.m_dwAttackerHP = pRecvPacket->m_dwAttackerHP;
            pk.result.m_dwAttackerMP = pRecvPacket->m_dwAttackerMP;
            pk.result.m_byNumberOfTargets = pRecvPacket->m_byNumberOfTargets;
            pk.result.m_byNumberOfFieldEffect = pRecvPacket->m_byNumberOfFieldEffect;
            //pk.sizeof_additions = pk.sizeof_additions;

            BYTE* action_list = (BYTE*)pMsg;
            action_list += pRecvPacket->GetSize();

            OnRecv_MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD(&pk, action_list);

		}
		break;

	case CG_SKILL_PERIODIC_DAMAGE_BRD:
		{
			MSG_CG_SKILL_PERIODIC_DAMAGE_BRD *pRecvPacket = (MSG_CG_SKILL_PERIODIC_DAMAGE_BRD *)pMsg;			

            MSGSUB_SKILL_PERIODIC_DAMAGE_BRD pk;
            pk.m_dwAttackerKey = pRecvPacket->m_dwAttackerKey;
            pk.m_SkillCode = pRecvPacket->m_SkillCode;
            pk.m_byNumberOfTargets = pRecvPacket->m_byNumberOfTargets;
            memcpy(pk.m_DamageInfo, pRecvPacket->m_DamageInfo, sizeof(pk.m_DamageInfo));

            OnRecv_MSGSUB_SKILL_PERIODIC_DAMAGE_BRD(&pk);

		}
		break;

	case CG_SKILL_REFLECT_DAMAGE_BRD:
		{
			MSG_CG_SKILL_REFLECT_DAMAGE_BRD *pRecvPacket = (MSG_CG_SKILL_REFLECT_DAMAGE_BRD *)pMsg;

            MSGSUB_SKILL_REFLECT_DAMAGE_BRD pk;
            pk.m_dwAttackerKey=         pRecvPacket->m_dwAttackerKey; 
            pk.m_dwTargetKey=           pRecvPacket->m_dwTargetKey;   
            pk.m_SkillCode=             pRecvPacket->m_SkillCode;     
            pk.m_wAbilityCode=          pRecvPacket->m_wAbilityCode;  
            pk.m_wDamage=               pRecvPacket->m_wDamage;       
            pk.m_dwTargetHP=            pRecvPacket->m_dwTargetHP;    
            pk.m_dwTargetResrveHP=      pRecvPacket->m_dwTargetResrveHP;

            OnRecv_MSGSUB_SKILL_REFLECT_DAMAGE_BRD(&pk);

		}
		break;

	case CG_SKILL_SELF_DESTRUCTION_BRD:
		{
			const MSG_CG_SKILL_SELF_DESTRUCTION_BRD* pRecvPacket = (MSG_CG_SKILL_SELF_DESTRUCTION_BRD*)pMsg;

            MSGSUB_SKILL_SELF_DESTRUCTION_BRD pk;
            pk.m_dwAttackerKey=             pRecvPacket->m_dwAttackerKey;
            pk.m_SkillCode=                 pRecvPacket->m_SkillCode;
            pk.m_byNumberOfTargets=         pRecvPacket->m_byNumberOfTargets;
            memcpy(pk.m_DamageInfo, pRecvPacket->m_DamageInfo, sizeof(pk.m_DamageInfo));

            OnRecv_MSGSUB_SKILL_SELF_DESTRUCTION_BRD(&pk);
		}
		break;


	case CG_SKILL_VITAL_SUCTION_BRD:
		{
			MSG_CG_SKILL_VITAL_SUCTION_BRD* pRecvPacket = (MSG_CG_SKILL_VITAL_SUCTION_BRD*)pMsg;

            MSGSUB_SKILL_VITAL_SUCTION_BRD pk;
            pk.m_dwAttackerKey =  pRecvPacket->m_dwAttackerKey; 
            pk.m_dwTargetKey =    pRecvPacket->m_dwTargetKey;   
            pk.m_dwHP =           pRecvPacket->m_dwHP; 

            OnRecv_MSGSUB_SKILL_VITAL_SUCTION_BRD(&pk);
		}
		break;

	case CG_SKILL_SUCTION_BRD:
		{
			MSG_CG_SKILL_SUCTION_BRD* pRecvPacket = (MSG_CG_SKILL_SUCTION_BRD*)pMsg;

            MSGSUB_SKILL_SUCTION_BRD pk;
            pk.m_dwAttackerKey=     pRecvPacket->m_dwAttackerKey; 
            pk.m_SkillCode=         pRecvPacket->m_SkillCode;     
            pk.m_wTargetDamage=     pRecvPacket->m_wTargetDamage; 
            pk.m_dwTargetHP=        pRecvPacket->m_dwTargetHP;   
            pk.m_dwTargetMP=        pRecvPacket->m_dwTargetMP;   
            pk.m_dwAttackerHP=      pRecvPacket->m_dwAttackerHP; 
            pk.m_dwAttackerMP=      pRecvPacket->m_dwAttackerMP; 

            OnRecv_MSGSUB_SKILL_SUCTION_BRD(&pk);

		}
		break;

	case CG_SKILL_TRANSPARENT_BRD:
		{
			MSG_CG_SKILL_TRANSPARENT_BRD* pRecvPacket = (MSG_CG_SKILL_TRANSPARENT_BRD*)pMsg;

            MSGSUB_SKILL_TRANSPARENT_BRD pk;
            pk.m_dwObjectKey = pRecvPacket->m_dwObjectKey;

            OnRecv_MSGSUB_SKILL_TRANSPARENT_BRD(&pk);

		}
		break;


	case CG_SKILL_EXECUTE_STATUS_CMD:
		{
			MSG_CG_SKILL_EXECUTE_STATUS_CMD* pRecvPacket = (MSG_CG_SKILL_EXECUTE_STATUS_CMD*)pMsg;
			Character* pChr= (Character*)g_ObjectManager.GetObject(pRecvPacket->m_dwTargetKey);
	
			if (pChr)
			{
				if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( pRecvPacket->m_wStatusCode, pRecvPacket->m_iStatusTime ) )
                {
                    pChr->AddSkillEffect(pEffect);
                }
			}
		}
		break;
    case CG_SKILL_PERK_CONTROL_CMD:
    case CG_SKILL_PERK_STATUS_CMD:
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    case CG_SKILL_PERK_GROUP_CONTROL_CMD:
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
        {
            uiBase* perk_dialog = GET_DIALOG(IM_SKILL_MANAGER::SKILL_PERK_DLG);
            if (perk_dialog != NULL)
            {
                perk_dialog->NetworkProc(pMsg);
            }
        }
        break;

	default:
		{	
			SceneBase::OnRecvCG_SKILL(pMsg);
		}
		break;

	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ShowErrorCodeSkill(DWORD code)
{
	TCHAR	szMessage[MAX_MESSAGE_LENGTH];
	int		iMessage = 0;

	switch(code)
	{
	case RC_SKILL_SUCCESS:
		{
			//	CG_SKILL_ACTION_NAK: BaseSkillInfo가 없다
			iMessage = eST_SKILL_ACTION_NAK_SUCCESS;
		}
		break;

	case RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY:
		{
			//	CG_SKILL_ACTION_NAK: 적을 공격할 수 없는 상태이다
			iMessage = eST_SKILL_ACTION_NAK_STATE_WHERE_CANNOT_ATTACK_ENEMY;
		}
		break;

	case RC_SKILL_COOLTIME_ERROR:
		{
			//	CG_SKILL_ACTION_NAK: 아직 쿨타임이 끝나지 않았다
			iMessage = eST_SKILL_ACTION_NAK_COOLTIME_ERROR;
		}
		break;

	case RC_SKILL_HPMP_RUN_SHORT:
		{
			//	CG_SKILL_ACTION_NAK: //70036 : 해당 스킬을 사용할 HP나 MP가 부족합니다.
			iMessage = eST_SKILL_ACTION_NAK_HPMP_RUN_SHORT;
			g_HeroInput.PlayNotEnoughManaSound();
		}
		break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case RC_SKILL_FP_RUN_SHORT:
        {
            iMessage = eST_SKILL_ACTION_NAK_FP_RUN_SHORT;
            g_HeroInput.PlayNotEnoughFPSound();
        }
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	case RC_SKILL_DOES_NOT_HAVE:
		{
			//	CG_SKILL_ACTION_NAK: 스킬을 가지고 있지 않습니다.
			iMessage = eST_SKILL_ACTION_NAK_DOES_NOT_HAVE;
		}
		break;


	case RC_SKILL_CHAR_CLASS_LIMIT:
		{
			//	CG_SKILL_ACTION_NAK: 클래스 제한(그 스킬을 사용할 수 없는 캐릭터 타입이다)
			iMessage = eST_SKILL_ACTION_NAK_CHAR_CLASS_LIMIT;
		}
		break;

	case RC_SKILL_WEAPON_LIMIT:
		{
			//	CG_SKILL_ACTION_NAK: 무기 제한
			iMessage = eST_SKILL_ACTION_NAK_WEAPON_LIMIT;
		}
		break;

	case RC_SKILL_REQUIRE_LEVEL_LIMIT:
		{
			//	CG_SKILL_ACTION_NAK: 요구 레벨 제한
			iMessage = eST_SKILL_ACTION_NAK_REQUIRE_LEVEL_LIMIT;
		}
		break;

	case RC_SKILL_REQUIRE_SKILLSTAT_LIMIT:
		{
			//	CG_SKILL_ACTION_NAK: 요구 숙련도 제한
			iMessage = eST_SKILL_ACTION_NAK_REQUIRE_SKILLSTAT_LIMIT;
		}
		break;

	case RC_SKILL_CAN_NOT_STATE_HIDE:
		{
			// 하이드 스킬 실패(현재 전투상태 5초지속상태에서 스킬사용시 실패함)
			iMessage = 5074;
		}
		break;
    case RC_SKILL_CANNOT_SUMMON:
        {
            //5631:해당몬스터는 소환할수없습니다.
            iMessage = 5631;
        }
        break;
    case RC_SKILL_CANNOT_HEAL:
        {
            //5633 소환수에게 힐을 할 수 없습니다.
            iMessage = 5633;
        }
        break;
	}

	if(iMessage!=0)
	{
		g_InterfaceManager.GetInterfaceString( iMessage, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_CONVERSATION( MSG_BASE * pMsg )
{
	ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
	pChatDlg->NetworkProc( pMsg );
}

//------------------------------------------------------------------------------
/**
	캐릭터 뿐만 아니라 몬스터도 해당!
	앉기 등의 이모티콘 상태 변화를 수행 

    해제(서기) 동작은 서버로 부터 응답이 오거나 스킬,데미지시 자동 해제 된다.-지완 

*/
BOOL BattleScene::ChangeStatus(DWORD dwObjKey,BYTE byConditionType)
{
	Character *pChar=(Character *)g_ObjectManager.GetObject(dwObjKey);
	if(pChar)
	{
		pChar->SetCondition(byConditionType);
		pChar->SetEmotion( 0 );
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCG_STATUS( MSG_BASE * pMsg )
{

	AddBattleTimeSerial();

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_CMD:
	case CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_KEY:
		{
			ISecureWorkNode::OnMsgRecvSignal::DuplexSNK_T1( pMsg, 0 );
		}
		break;

		//스탯 변경 처리 -2005.04.30 지완추가 
	case CG_STATUS_STAT_SELECT_ACK :
		{
			MSG_CG_STATUS_STAT_SELECT_ACK	*pRecvPacket = (MSG_CG_STATUS_STAT_SELECT_ACK*)pMsg;
			eATTR_TYPE attrType = (eATTR_TYPE)pRecvPacket->m_AttrType;

            if( g_pHero )
			{
				(*g_pHero->GetPlayerAttribute())[attrType][eATTR_KIND_BASE] = pRecvPacket->m_dwValue;

				g_pHero->GetPlayerAttribute()->Update();

				BASE_PLAYERINFO*  pPlayInfo = g_pHero->GetCharInfo();
				if (g_pHero && pPlayInfo && (g_pHero->GetObjectKey() == g_pHero->GetObjectKey()))
				{
                    //! 2011.1.28 / i4u4me / gm 명령어로 올때는 attrType == eATTR_TYPE_INVALID 으로 온다.
                    //! gm 명령어로 스텟을 올릴 경우에는 보너스 스텟을 차감하지 않는다.
                    if ((attrType == eATTR_STR) || 
                        (attrType == eATTR_DEX) || 
                        (attrType == eATTR_VIT) || 
                        (attrType == eATTR_INT) || 
                        (attrType == eATTR_SPR) || 
                        (attrType == eATTR_EXPERTY1) || 
                        (attrType == eATTR_EXPERTY2))
                    {
                        pPlayInfo->m_iRemainStat--;
                    }

				}

				// update한 스텟을 얻어온다. 
				g_pHero->GetPlayerAttribute()->GetCharInfo( pPlayInfo );

				g_pHero->UpdateItemAttrCalculator();
				g_pHero->UpdateSkillAttr();

				//인벤토리내에 있어도 옵션 적용되는 아이템 처리 
				if (g_pHero && (g_pHero->GetObjectKey() == g_pHero->GetObjectKey()))
				{
                    uiCharacterStatusMan::ResetSimulationStatCount();
                    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

                    //용변신 & 외형과 상관없는슬롯위치에서는 갱신안함
                    if ((g_pHero->IsTransForm() == false) &&
                        (g_pHero->GetTransformData().GetTransFormProcessType() == TRANSFORM_PROCESS_NONE))
                    {
                        g_pHero->RefreshAppearRender();
                    }
				}
			}

            g_SoundEffect.PlayUI(UI_SOUND_STATE_UP_CLICK);
		}
		break;
		 
	//	//스탯 변경 실패 처리 -2005.04.30 지완추가 
	//case CG_STATUS_STAT_SELECT_NAK :
	//	{
	//		MSG_CG_STATUS_STAT_SELECT_NAK	*pRecvPacket = (MSG_CG_STATUS_STAT_SELECT_NAK*)pMsg;
 //           //TODO:메시지 출력 실패시 재시도 할수 있게 스위치 꺼줌 
	//	}
	//	break;

    case CG_STATUS_STAT_APPLY_ACK:
        {
            MSG_CG_STATUS_STAT_APPLY_ACK* ack = static_cast<MSG_CG_STATUS_STAT_APPLY_ACK*>(pMsg);
            if (g_pHero != NULL)
            {
                g_pHero->GetPlayerAttribute()->SetAttrValue(eATTR_STR, eATTR_KIND_BASE, ack->stat_point[eATTR_STR]);
                g_pHero->GetPlayerAttribute()->SetAttrValue(eATTR_DEX, eATTR_KIND_BASE, ack->stat_point[eATTR_DEX]);
                g_pHero->GetPlayerAttribute()->SetAttrValue(eATTR_VIT, eATTR_KIND_BASE, ack->stat_point[eATTR_VIT]);
                g_pHero->GetPlayerAttribute()->SetAttrValue(eATTR_INT, eATTR_KIND_BASE, ack->stat_point[eATTR_INT]);
                g_pHero->GetPlayerAttribute()->SetAttrValue(eATTR_SPR, eATTR_KIND_BASE, ack->stat_point[eATTR_SPR]);
                g_pHero->GetPlayerAttribute()->Update();
                BASE_PLAYERINFO* player_info = g_pHero->GetCharInfo();
                g_pHero->GetPlayerAttribute()->GetCharInfo(player_info);

                g_pHero->UpdateItemAttrCalculator();
                g_pHero->UpdateSkillAttr();

                if (g_pHero && (g_pHero->GetObjectKey() == g_pHero->GetObjectKey()))
                {
                    if (g_pHero->GetCharInfo() != NULL)
                    {
                        g_pHero->GetCharInfo()->m_iRemainStat = ack->remain_stat_point;
                    }

                    //! 스텟 정보도 빼고 , 시뮬레이션 정보도 갱신한다.
                    uiCharacterStatusMan::ResetSimulationStatCount();
                    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();
                }

                g_pHero->RefreshAppearRender();
                GlobalFunc::EquipmentPlayerRefreshAppearRender();

                g_SoundEffect.PlayUI(UI_SOUND_STATE_UP_CLICK);
            }
        }
        break;

    case CG_STATUS_STAT_APPLY_NAK:
        {
            MSG_CG_STATUS_STAT_APPLY_NAK* nak = static_cast<MSG_CG_STATUS_STAT_APPLY_NAK*>(pMsg);

            uiCharacterStatusMan::ResetSimulationStatCount();
            uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

            // 5228 잘못된 캐릭터 정보 입니다. 다시 시도해 주시기 바랍니다.
            g_InterfaceManager.GetInterfaceString(5228, szMessage , INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
        }
        break;


	//캐릭터 상태 변경 이모티콘용
	case CG_STATUS_CONDITION_CHANGE_BRD:
		{
			MSG_CG_STATUS_CONDITION_CHANGE_BRD	*pRecvPacket = (MSG_CG_STATUS_CONDITION_CHANGE_BRD*)pMsg;
			ChangeStatus(pRecvPacket->m_dwObjectKey,pRecvPacket->m_byConditionType);
		}
		break;

	case CG_STATUS_CONDITION_CHANGE_NAK:
		{
			//MSG_CG_STATUS_CONDITION_CHANGE_NAK	*pRecvPacket = (MSG_CG_STATUS_CONDITION_CHANGE_NAK*)pMsg;
#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
            g_pHero->set_sitting_packet_delay(FALSE);
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
		}
		break;

	case CG_STATUS_EMOTION_BRD:
		{
			MSG_CG_STATUS_EMOTION_BRD	*pRecvPacket = (MSG_CG_STATUS_EMOTION_BRD *)pMsg;
			Character	*pChar = (Character *)g_ObjectManager.GetObject( pRecvPacket->m_dwObjectKey );
			if( pChar )
			{
				SLOTCODE	dwSkillid = pRecvPacket->m_byEmotionType + eEMOTION_START_CODE;
				BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
				assert(pInfo);
				
				if(pInfo)
				{
					pChar->SetEmotion( pInfo->m_wzidAnimationCode[0].id );
				}

				pChar->SetNextState( STATE::EMOTICON, g_CurTime );
			}
		}
		break;

	case CG_STATUS_EMOTION_NAK:
		{
			MSG_CG_STATUS_EMOTION_NAK	*pRecvPacket = (MSG_CG_STATUS_EMOTION_NAK *)pMsg;
			
			switch(pRecvPacket->m_byErrorCode)
			{
			case RC_STATUS_REQUIRE_LEVEL_LIMIT:
				{
					// 현재 레벨에서 이용할 수 없는 기능입니다. : 5065
					g_InterfaceManager.GetInterfaceString(eST_NOT_USE_CURRENT_LEVEL, szMessage , INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				break;

			default:
				{
					// 이모션 적용에 실패 하였습니다. : 5453
					g_InterfaceManager.GetInterfaceString(eST_FAILED_EMOTION , szMessage , INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				break;
			}

		}
		break;

	case CG_STATUS_NPC_STATE_CHANGE_CMD:
		{
			MSG_CG_STATUS_NPC_STATE_CHANGE_CMD	*pRecvPacket = (MSG_CG_STATUS_NPC_STATE_CHANGE_CMD*)pMsg;

			Character *pChr = (Character *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);

			if (pChr)
			{
				// 느낌표
				pChr->ShowEmoticonIcon(HEAD_MESH_EXCLAMATION_MARK,4000);
			}


		}
		break;

		// 자신의 경험치가 변경되었다.
	case CG_STATUS_EXP_CMD :
		{
			assert(g_pHero);
			if(!g_pHero)
				break;

			MSG_CG_STATUS_EXP_CMD	*pRecvPacket = (MSG_CG_STATUS_EXP_CMD*)pMsg;
			EXPTYPE exp = pRecvPacket->m_i64Exp;			// 최종 경험치
			EXPTYPE prevExp = g_pHero->GetCharInfo()->m_i64Exp;

			g_pHero->SetExp(exp);
			EXPTYPE expoffset = exp - prevExp;

			if ( expoffset > 0 )
			{

			uiBattleScoreMan* battle_score_manager_ptr = 
                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			if (battle_score_manager_ptr)
			{
                uiBattleScoreMan::eBTSCROE_MODE bts_mode = battle_score_manager_ptr->GetMode();
                if (bts_mode == uiBattleScoreMan::eBTS_MISSION || bts_mode == uiBattleScoreMan::eBTS_SSQ)
                {
			       battle_score_manager_ptr->RefreshInfo(expoffset, 0);     
                }
			}
				if (pRecvPacket->m_dwTargetObjKey)
				{
					EXP_POPUP_INFO info;
					info.m_dwCharacterKey = pRecvPacket->m_dwTargetObjKey;
					info.m_dwExpOffset = expoffset;
					info.m_nBonusExpRatio = pRecvPacket->m_BonusRatio;
					m_ExpList.push_back(info);

				}
				else
				{
					PopupExp(expoffset , pRecvPacket->m_BonusRatio );
				}				
				
				uiHelpMan* helpMan =
							static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

				if (helpMan && (g_pHero->GetLevel() == 1))
				{
					helpMan->CheckHelpMessage(eHK_EXP);
				}
                uiGuideSystemMan* guide_manager =
                    static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                if (guide_manager)
                {
                    guide_manager->CheckHelpMessage(TotalGuideNotice::kGetExp);
                }
			}

		}
		break;

		// 자신의 그룹 경험치가 변경되었다.
	case CG_STATUS_GROUPEXP_CMD :
		{
			assert(g_pHero);
			if(!g_pHero)
				break;

			MSG_CG_STATUS_EXP_CMD	*pRecvPacket = (MSG_CG_STATUS_EXP_CMD*)pMsg;
			EXPTYPE exp = pRecvPacket->m_i64Exp;			// 최종 경험치
			EXPTYPE prevExp = g_pHero->GetCharInfo()->m_i64Exp;

			g_pHero->SetExp(exp);
			EXPTYPE expoffset = exp - prevExp;

			if( expoffset > 0 )
			{

				if (pRecvPacket->m_dwTargetObjKey)
				{
					EXP_POPUP_INFO info;
					info.m_dwCharacterKey = pRecvPacket->m_dwTargetObjKey;
					info.m_dwExpOffset = expoffset;
					info.m_nBonusExpRatio = pRecvPacket->m_BonusRatio;
					m_ExpList.push_back(info);

				}
				else
				{
					PopupExp(expoffset , pRecvPacket->m_BonusRatio, eGroupExp );
				}				
				
				uiHelpMan* helpMan =
							static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

				if (helpMan && (g_pHero->GetLevel() == 1))
				{
					helpMan->CheckHelpMessage(eHK_EXP);
				}
			}

		}
		break;

	case CG_STATUS_DEAD_BRD :
		{
			MSG_CG_STATUS_DEAD_BRD	*pRecvPacket = (MSG_CG_STATUS_DEAD_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwObjectKey;	// 누구
			WzVector deadPos = pRecvPacket->m_wvCurPos;
			EXPTYPE exp = pRecvPacket->m_i64Exp;

			Character* pChr = (Character*)g_ObjectManager.GetObject(dwKey);

			if (pChr)
			{	
				pChr->SetLatestAttack(GetBattleTimeSerial());
				pChr->SetHP(0);

                if( pChr->IsKindOfObject(PLAYER_OBJECT) == TRUE )
                {
                    Player* pPlayer = (Player*)pChr;
                    pPlayer->SetExp(exp);
                }
			}
		}
		break;
		
		// 누가 레벨업했다.
	case CG_STATUS_LEVEL_UP_BRD :
		{
			MSG_CG_STATUS_LEVEL_UP_BRD	*pRecvPacket = (MSG_CG_STATUS_LEVEL_UP_BRD *)pMsg;

			DWORD dwKey = pRecvPacket->m_dwObjectKey;		// 누구

            if( Player* pPlayer = (Player*)g_ObjectManager.GetPlayer(dwKey) )
            {
                pPlayer->SetLevel( pRecvPacket->m_Level );
                pPlayer->SetHP( pRecvPacket->m_dwCurHP );
                pPlayer->SetMP( pRecvPacket->m_dwCurMP );

                if( pPlayer->IsMyHero() )
                {
                    g_pHero->SetRemainStatPoint( pRecvPacket->m_wRemainStat );
                    g_pHero->SetRemainSkillPoint( pRecvPacket->m_wRemainSkill );
                }

                pPlayer->OnLevelUp();
            }
		}
		break;

	case CG_STATUS_LEVEL_UP_CMD:
		{
			MSG_CG_STATUS_LEVEL_UP_CMD *pRecvPacket = (MSG_CG_STATUS_LEVEL_UP_CMD *)pMsg;

			if (!g_pHero)
			{
				return;
			}

			g_pHero->SetSD(pRecvPacket->m_dwCurSD);
            g_pHero->SetLevel( pRecvPacket->m_Level );
            g_pHero->SetHP( pRecvPacket->m_dwCurHP );
            g_pHero->SetMP( pRecvPacket->m_dwCurMP );
            g_pHero->SetRemainStatPoint( pRecvPacket->m_wRemainStat );
            g_pHero->SetRemainSkillPoint( pRecvPacket->m_wRemainSkill );

			g_pHero->OnLevelUp();
            g_pHero->UpdateSkillAttr();

			if( uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP)) )
			{
				helpMan->CheckHelpMessage(eHK_LEVEL_UP);
			}

            if( uiGuideSystemMan* guide_manager = static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM)) )
            {
                guide_manager->CheckHelpMessage(TotalGuideNotice::kLevelUp);
            }

			if( uiCommunityMan* pCommunityMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY)) )
			{
				// 내가 레벨업한 경우 하위멤버랑 비교
				pCommunityMan->CheckGroupLevelfromHero();
			}

			RefreshNPCStateForQuest();
		}
		break;

	// 피회복~
	case CG_STATUS_RECOVER_ATTR_BRD :
		{
			MSG_CG_STATUS_RECOVER_ATTR_BRD	*pRecvPacket = (MSG_CG_STATUS_RECOVER_ATTR_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			

			if (pChr)
			{
				pChr->SetLatestAttack(GetBattleTimeSerial());
				pChr->SetHP(pRecvPacket->m_dwTargetHP);	

				if (pChr->GetMaxHP() < pChr->GetHP())
				{
				    _asm nop;
				}
				pChr->SetMP(pRecvPacket->m_dwTargetMP);
				
			}
		}
		break;

	case CG_STATUS_CHANGE_HP_BRD:
		{
            MSG_CG_STATUS_CHANGE_HP_BRD	*pRecvPacket = (MSG_CG_STATUS_CHANGE_HP_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			
			if (pChr)
			{
				pChr->SetLatestAttack(GetBattleTimeSerial());
				pChr->SetHP(pRecvPacket->m_dwTargetHP);
				pChr->SetReserveHP( pRecvPacket->m_dwReserveHP );
			}
		}
		break;

	case CG_STATUS_CHANGE_MP_BRD:
		{
			MSG_CG_STATUS_CHANGE_MP_BRD	*pRecvPacket = (MSG_CG_STATUS_CHANGE_MP_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			if (pChr)
			{
				pChr->SetMP(pRecvPacket->m_dwTargetMP);
#if defined( _DEBUG )
				if(pChr->IsKindOfObject(MONSTER_OBJECT))
				{
					TCHAR temp[1024] = {0,};
					Snprintf(temp, 1024-1, _T("MaxMp=%d CurMp=%d"),pChr->GetMaxMP(),pChr->GetMP());
					MessageBox(0,temp,temp,MB_OK);
				}
#endif
			}
		}
		break;

	case CG_STATUS_CHANGE_SD_CMD:
		{
			MSG_CG_STATUS_CHANGE_SD_CMD *pRecvPacket = (MSG_CG_STATUS_CHANGE_SD_CMD *)pMsg;
			//주인공의 SP
			if(g_pHero)
			{
				g_pHero->SetSD(pRecvPacket->m_dwSD);
			}
		}
		break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case CG_STATUS_CHANGE_FP_CMD:
        {
            MSG_CG_STATUS_CHANGE_FP_CMD* recv_packet = static_cast<MSG_CG_STATUS_CHANGE_FP_CMD*>(pMsg);
            // 주인공의 FP
            if (g_pHero)
            {
                g_pHero->SetFP(recv_packet->m_dwTargetFP);
            }
        }
        break;
    case CG_STATUS_CHANGE_FP_EFFECT_BRD:
        {
            MSG_CG_STATUS_CHANGE_FP_EFFECT_BRD* recv_packet = static_cast<MSG_CG_STATUS_CHANGE_FP_EFFECT_BRD*>(pMsg);
            DWORD object_key = recv_packet->object_key;
            Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
            if (player_ptr)
            {
                player_ptr->SetFP(recv_packet->current_fp);
            }
        }
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	// 부활~
	case CG_STATUS_RESURRECTION_BRD:
		{
			MSG_CG_STATUS_RESURRECTION_BRD	*pRecvPacket = (MSG_CG_STATUS_RESURRECTION_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);

			if (pChr)
			{
				pChr->SetLatestAttack(GetBattleTimeSerial());
				pChr->SetPosition(pRecvPacket->m_wvCurPos);
				pChr->SetHP(pRecvPacket->m_dwHP);		
				pChr->SetMP(pRecvPacket->m_dwMP);
                pChr->SetReserveHP( pRecvPacket->m_dwReserveHP );

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pChr->GetObjectKey(), TRUE );
                g_ProjectileManagerEx.CreateEffect( pChr->GetObjectKey(), StrToWzID("A022"), 5000, Position );
			}

			if (pChr == g_pHero)
			{
				if (GENERALPARAM->GetSpecialMode())
				{
					GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("플레이어 부활~"));
				}
				SetBrightDelay(750);
				this->m_bDeadFlag = FALSE;
			}
		}
		break;

	case CG_STATUS_REBIRTH_ON_VILLAGE_BRD:
		{
			MSG_CG_STATUS_REBIRTH_ON_VILLAGE_BRD *pRecvPacket = (MSG_CG_STATUS_REBIRTH_ON_VILLAGE_BRD *)pMsg;

			DWORD dwKey = pRecvPacket->m_dwObjectKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);

			if (pChr)
			{
				pChr->SetLatestAttack(GetBattleTimeSerial());
				pChr->SetPosition(pRecvPacket->m_wvCurPos);
				pChr->SetHP(pRecvPacket->m_dwHP);		
				pChr->SetMP(pRecvPacket->m_dwMP);

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pChr->GetObjectKey(), TRUE );
                g_ProjectileManagerEx.CreateEffect( pChr->GetObjectKey(), StrToWzID("A022"), 5000, Position );
			}

			if (pChr == g_pHero)
			{
				SetBrightDelay(750);
				this->m_bDeadFlag = FALSE;
			}
		}
		break;

	case CG_STATUS_REBIRTH_ON_VILLAGE_NAK:
		{
			//Todo:Nak처리 
		}
		break;


	case CG_STATUS_RESURRECTION_NAK:
		{
			//	부활실패!
			const int	c_iResurrectionFailed = 70257;
			g_InterfaceManager.GetInterfaceString( c_iResurrectionFailed, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

    case CG_STATUS_ATTR_CHANGE_BRD:
        {
            MSG_CG_STATUS_ATTR_CHANGE_BRD *pRecvPacket = (MSG_CG_STATUS_ATTR_CHANGE_BRD *)pMsg;

			Character *pChr = (Character *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);

			if (pChr) 
            {
				switch (pRecvPacket->m_AttrType)
				{
				case eATTR_MAX_HP:							// 최대 HP
					{
						pChr->SetMaxHP(pRecvPacket->m_dwValue);					
						if (pChr->GetMaxHP() < pChr->GetHP())
						{
							_asm nop;
						}
					}
					break;
				case eATTR_MAX_MP:							// 최대 MP
					pChr->SetMaxMP(pRecvPacket->m_dwValue);					
					break;
				case eATTR_ATTACK_SPEED:           // 공격속도
					pChr->SetAttackSpeedRatio(pRecvPacket->m_dwValue);					
					break;
				case eATTR_MOVE_SPEED:                // 이동속도
					pChr->SetMoveSpeedRatio(pRecvPacket->m_dwValue);										
					break;

				}
			}
        }
        break;

	case CG_STATUS_ETHER_ATTACK_SPEED_CMD:
		{
			if (!g_pHero)
            {
                return;
            }

			MSG_CG_STATUS_ETHER_ATTACK_SPEED_CMD* pRecvPacket = (MSG_CG_STATUS_ETHER_ATTACK_SPEED_CMD*)pMsg;
			
			g_pHero->SetEtherOptionValue(pRecvPacket->m_AttackSpeedValue);
			g_pHero->UpdateSkillAttr();

		}
		break;

	case CG_STATUS_ADD_BRD:
		{
			MSG_CG_STATUS_ADD_BRD* pRecvPacket = (MSG_CG_STATUS_ADD_BRD*)pMsg;

			DWORD dwKey = pRecvPacket->m_dwTargetKey;
			Character* pChr = (Character*)g_ObjectManager.GetObject(dwKey);
			if (pChr)
			{
				if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( pRecvPacket->m_wStatusCode, 0 ) )
                {
                    pChr->AddSkillEffect(pEffect);
                }
			}
		}
		break;

	case CG_STATUS_REMOVE_BRD:
		{
			MSG_CG_STATUS_REMOVE_BRD *pRecvPacket = (MSG_CG_STATUS_REMOVE_BRD *)pMsg;
			DWORD dwKey = pRecvPacket->m_dwTargetKey;
			Character *pChr = (Character *)g_ObjectManager.GetObject(dwKey);
			if (pChr)
			{
				//Todo:리턴 값 검사 
				pChr->RemoveSkillEffectWithStatus((eCHAR_STATE_TYPE)pRecvPacket->m_dwStatusCode);
			}
		}
		break;

	case CG_STATUS_RENDER_OPTION_BRD:
		{
			MSG_CG_STATUS_RENDER_OPTION_BRD	*pRecvPacket = (MSG_CG_STATUS_RENDER_OPTION_BRD *)pMsg;

			Object *pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);

			if (pObject)
			{
				if (pObject->IsKindOfObject(PLAYER_OBJECT))
				{
					Player *pPlayer = (Player *)pObject;

					switch(pRecvPacket->m_byRenderOptionType)
					{
					case MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION:
						{
							BOOL bHide = pRecvPacket->m_byOptionLevel;
							
							pPlayer->HideHelmet( bHide );
						}
						break;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
                    case MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_WING_OPTION:
                        {
                            BOOL bHide = pRecvPacket->m_byOptionLevel;

                            pPlayer->HideWing( bHide );
                        }
                        break;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
                    case MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION:
                        {
                            BOOL bHide = pRecvPacket->m_byOptionLevel;

                            pPlayer->HideCostume( bHide );
                        }
                        break;
#endif
					}					
				}
			}
		}
		break;
	case CG_STATUS_CHANGE_MONSTER_ABILITY:
		{
			MSG_CG_STATUS_CHANGE_MONSTER_ABILITY	*pRecvPacket = (MSG_CG_STATUS_CHANGE_MONSTER_ABILITY *)pMsg;

			SetMonsterHPMultiplier(pRecvPacket->m_fHPRatio);
		}
		break;

	case CG_STATUS_FATIGUE_CHANGE_CMD:
		{
			MSG_CG_STATUS_FATIGUE_CHANGE_CMD	*pRecvPacket =(MSG_CG_STATUS_FATIGUE_CHANGE_CMD *)pMsg;
			TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG(TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM); 
			pTiredDlg->SetTime(pRecvPacket->m_wPlayingTime);
#ifdef _CHINA
			pTiredDlg->RenderTiredsystemMessage(pRecvPacket->m_wPlayingTime);
#endif // _CHINA
		}
		break;

    case CG_STATUS_ALL_STATEINFO_CMD:
        {
            MSG_CG_STATUS_ALL_STATEINFO_CMD * pRecvPacket = (MSG_CG_STATUS_ALL_STATEINFO_CMD *)pMsg;
            if ( g_pHero )
            {
                g_pHero->SetStateTotalInfo(pRecvPacket->m_StateTotalInfo);
            }            
        }
        break;

	case CG_STATUS_ETC_STATEINFO_CMD:
		{
			MSG_CG_STATUS_ETC_STATEINFO_CMD * pRecvPacket = (MSG_CG_STATUS_ETC_STATEINFO_CMD *)pMsg;

			if(g_pHero)
			{
				for( int a = 0; a < pRecvPacket->m_stEtcStateTotalInfo.m_Count; ++a )
				{
					ETC_STATE_INFO & rInfo = pRecvPacket->m_stEtcStateTotalInfo.m_Slot[a];
					if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( rInfo.m_wStateCode, rInfo.m_iLeavedTime ) )
                    {
                        g_pHero->AddSkillEffect(pEffect);
                    }
				}
			}

		}
		break;


	// 변신 시작 Todo:바로 와야 한다.
	case CG_STATUS_START_DRAGON_FORMATION_BRD:
		{
			MSG_CG_STATUS_START_DRAGON_FORMATION_BRD * pRecvPacket = (MSG_CG_STATUS_START_DRAGON_FORMATION_BRD *)pMsg;
			RESOURCECODE code=0;
			eTRANSFORM_TYPE type=eTRANSFORM_TYPE_NONE;

			SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pRecvPacket->m_SkillCode);
			if(pInfo)
			{
				BASE_ABILITYINFO * pAbility =pInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );

				if(pAbility)
				{
					code= pAbility->m_iOption1;
					type= (eTRANSFORM_TYPE)pAbility->m_iOption2;
				}
				else
				{
					ASSERT("변신 어빌리티 인포가 없다");
					break;
				}
			}
			else 
			{
				ASSERT("변신 스킬 인포가 없다");
				break;
			}

			Object *pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);

			if (pObject)
			{
				if (pObject->IsKindOfObject(PLAYER_OBJECT))
				{
					Player *pPlayer = (Player *)pObject;


					if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( pRecvPacket->m_wStatusCode, pRecvPacket->m_iStatusTime ) )
                    {
                        pPlayer->AddSkillEffect(pEffect);
                    }

					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_TRANSFORM;	
					action.TRANSFORM.appearKind=APPEARANCEINFO::PLAYERTRANSFORM;
					action.TRANSFORM.transFormProcess=TRANSFORM_PROCESS_TRANSFORM;
					//Todo:스크립트에서 얻어 온다.
					action.TRANSFORM.transFormType=type;
					action.TRANSFORM.rcCode=code;

					pPlayer->SetTransSkillCode(pRecvPacket->m_SkillCode);
					
					if(g_pHero&&g_pHero==pPlayer)
					{
						g_HeroInput.SetTransFormAction(&action);
					}
					else
					{
						pPlayer->PutAction(&action);
					}
				}
			}
		}
		break;


	case CG_STATUS_END_DRAGON_FORMATION_NAK:
		{
			MSG_CG_STATUS_END_DRAGON_FORMATION_NAK * pRecvPacket = (MSG_CG_STATUS_END_DRAGON_FORMATION_NAK *)pMsg;
		
			if(g_pHero)
				BIT_REMOVE(g_pHero->m_dwPacketStatus, PACKET_STATUS__END_FORMATION );

		}
		break;

	case CG_STATUS_END_DRAGON_FORMATION_BRD :
		{
			MSG_CG_STATUS_END_DRAGON_FORMATION_BRD * pRecvPacket = (MSG_CG_STATUS_END_DRAGON_FORMATION_BRD *)pMsg;
			
			Object *pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);

			if (pObject)
			{
				if (pObject->IsKindOfObject(PLAYER_OBJECT))
				{
					Player *pPlayer = (Player *)pObject;
					pPlayer->RemoveSkillEffectWithStatus((eCHAR_STATE_TYPE)pRecvPacket->m_wStatusCode);
					pPlayer->SetTransSkillCode(0);

					PLAYER_ACTION action;
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_TRANSFORM;	
					action.TRANSFORM.appearKind=APPEARANCEINFO::PLAYER;
					action.TRANSFORM.rcCode=0;
					action.TRANSFORM.transFormProcess=TRANSFORM_PROCESS_UNTRANSFORM;
					action.TRANSFORM.transFormType=eTRANSFORM_TYPE_NONE;

					if(g_pHero&&g_pHero==pPlayer)
					{
						//Todo:pubic속성 제거 
						BIT_REMOVE(g_pHero->m_dwPacketStatus, PACKET_STATUS__END_FORMATION );
						g_HeroInput.SetTransFormAction(&action);
					}
					else
					{
						pPlayer->PutAction(&action);
					}
					
				}
				
			
			}

			
		}
		break;


	case CG_STATUS_INTERCEPTION_ACK:
		{
			MSG_CG_STATUS_INTERCEPTION_ACK * pRecvPacket = (MSG_CG_STATUS_INTERCEPTION_ACK *)pMsg;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			OptionGameDialog * pOptionDialog = GET_CAST_DIALOG( OptionGameDialog, IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG );

			if( pOptionDialog )
				pOptionDialog->SetBlockCheck( pRecvPacket->m_byInterceptionCode, pRecvPacket->m_bRequestFlag );
#endif
		}

		break;

	case CG_STATUS_INTERCEPTION_NAK:
		{
			MSG_CG_STATUS_INTERCEPTION_NAK * pRecvPacket = (MSG_CG_STATUS_INTERCEPTION_NAK *)pMsg;

			g_InterfaceManager.GetStringStatusError(
				pRecvPacket->m_byErrorCode, szMessage, INTERFACE_STRING_LENGTH);

			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, INTERFACE_STRING_LENGTH);

		}
		break;

	case CG_STATUS_FREE_INIT_STAT_UNDER_20_ACK:
		{
			MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_ACK * pRecvMsg = (MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_ACK*) pMsg;

			g_pHero->SetFreeInitState( pRecvMsg->m_byFreeInitCount );
		}
		break;

	case CG_STATUS_FREE_INIT_STAT_UNDER_20_NAK:
		{
			MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_NAK* pRecvMsg = (MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_NAK*)pMsg;
			TCHAR szMessage[INTERFACE_STRING_LENGTH];

			switch (pRecvMsg->m_ResultCode)
			{
            case RC_STATUS_PLAYER_LEVEL_TOO_HIGH_FOR_FREE_STAT_INIT:
			case RC_STATUS_PLAYER_LEVEL_NOT_UNDER_40:
			case RC_STATUS_PLAYER_LEVEL_NOT_UNDER_20:	
				g_InterfaceManager.GetInterfaceString(6292, szMessage);	
				break;

			case RC_STATUS_NOT_EXIST_INIT_POINT:		
				g_InterfaceManager.GetInterfaceString(5071, szMessage);
				break;

			case RC_STATUS_NOT_UNEQUIP_ALL:			
				g_InterfaceManager.GetInterfaceString(5884, szMessage);
				break;
			}

			GlobalFunc::OutputMsg_Notice((eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW), szMessage);
		}
		break;



	case CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD: // Syn 날렸을때
    case CG_STATUS_RENDER_EFFECT_ITEM_LIST_CMD: // 서버에서 알아서 보내준다(CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD와 같은 액션을 취하지만 오는 조건이 틀리다.)
		{
			MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD* pRecvPacket = (MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD*)pMsg;
			int iPlayerCnt = pRecvPacket->m_Count;
			sMSG_RENDER_EFFECT_ITEM_LIST* pOffsetOfList = (sMSG_RENDER_EFFECT_ITEM_LIST*)( ((BYTE*)pRecvPacket) + pRecvPacket->GetSize() );
			for(int i = 0; i < iPlayerCnt; i++)
			{
				sMSG_RENDER_EFFECT_ITEM_LIST* pRender_effect_List = pOffsetOfList;

				if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRender_effect_List->m_PlayerKey) )
				{
                    pPlayer->RefreshRenderEffectItem(pPlayer, &pRender_effect_List->m_ItemCodes[0], pRender_effect_List->m_ItemCount);
				}
						
				pOffsetOfList = (sMSG_RENDER_EFFECT_ITEM_LIST*)( ((BYTE*)pOffsetOfList) + pOffsetOfList->GetSize() );
			}
		}
		break;


	case CG_STATUS_DELETE_CHAR_STATE_ACK:
		{
			TCHAR szStateName[128 + 1] = {0,};
			TCHAR szMessageFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
			MSG_CG_STATUS_DELETE_CHAR_STATE_ACK* pRecvPacket = (MSG_CG_STATUS_DELETE_CHAR_STATE_ACK*)pMsg;
			if(TRUE == pRecvPacket->m_bResult)//삭제 성공
			{
				//6331	%s 상태 아이콘이 삭제 되었습니다.
				g_InterfaceManager.GetInterfaceString( 6331, szMessage );
				BASE_STATEINFO* pInfo = StateInfoParser::Instance()->GetStateInfo(pRecvPacket->m_eCharStateType);
				if(pInfo)
				{
					g_InterfaceManager.GetInterfaceString(pInfo->m_dwNamecode, szStateName);
				}
				Sprintf(szMessageFinal, szMessage, szStateName);
			}
			else //삭제 실패
			{
				//6332	%s 상태 아이콘은 삭제 할 수 없습니다.
				g_InterfaceManager.GetInterfaceString( 6332, szMessage );
				BASE_STATEINFO* pInfo = StateInfoParser::Instance()->GetStateInfo(pRecvPacket->m_eCharStateType);
				if(pInfo)
				{
					g_InterfaceManager.GetInterfaceString(pInfo->m_dwNamecode, szStateName);
				}
				Sprintf(szMessageFinal, szMessage, szStateName);
			}

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessageFinal);
		}
		break;

	case CG_STATUS_SSQ_INFO_BRD:
		{
			const MSG_CG_STATUS_SSQ_INFO_BRD* pRecvPacket = (MSG_CG_STATUS_SSQ_INFO_BRD*)pMsg;

			WAR_MSG_HDR header = *(pRecvPacket->m_Header);
			WAR_TAG_P pIt = (WAR_TAG_P)(pRecvPacket->m_Header+1);

			OnRecvCG_SSQ_CTRL_CMD(pIt, header); 

		}
		break;

	}
}

void BattleScene::OnRecvCG_TRIGER( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{
	case CG_TRIGGER_DO_ACTION_BRD:
		{
			MSG_CG_TRIGGER_DO_ACTION_BRD * pRecvPacket = (MSG_CG_TRIGGER_DO_ACTION_BRD *)pMsg;

			DWORD dwGroupID = 0;
			DWORD dwCategoryID = 0;
			DWORD dwTriggerID = pRecvPacket->m_dwTriggerID;

//////////////////////////////////////////////////////////////////////////
			if(NULL != m_pTriggerMan)
			{
				Trigger* pTrigger = static_cast<Trigger*>(m_pTriggerMan->FindTrigger(dwTriggerID));
				if(pTrigger)
				{
					pTrigger->_Action();

					if(FALSE != pTrigger->IsNextActive())
					{
						Trigger* pNextTrigger = static_cast<Trigger*>(m_pTriggerMan->FindNextTrigger(dwTriggerID));
						if(NULL == pNextTrigger)
							break;

						int iCnt = m_pTriggerMan->m_vecTrgObj.size();
						for(int i = 0; i < iCnt; i++)
						{
							sTRIGGER_OBJECT* pTO = m_pTriggerMan->m_vecTrgObj[i];
							if(pTO)
							{
								if(pTO->m_dwTriggerID == pNextTrigger->TriggerID())
								{
									pTO->m_bActive = TRUE;
								}
							}
						}
					}

    uiMissionObject* dialog = GET_CAST_DIALOG(uiMissionObject, IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT);
    if (dialog)
    {
        dialog->CheckMissionObjectShow(dwTriggerID);
    }

#ifdef _DEV_VER
                    if (CGeneralGameParam::Instance()->IsShowTrigger())
                    {
                        TCHAR szMessage[2048] = {0,};
                        pTrigger->_DisplayTriggerInfo(szMessage);
                        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
                    }
#endif
				}
			}
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
			TCHAR szDebug[INTERFACE_STRING_LENGTH];
			Sprintf(szDebug, _T("MSG_CG_TRIGGER_DO_ACTION_BRD : Action Brd TriggerID( %u )\n"), dwTriggerID);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_OUPUT_DEBUG, szDebug);
#endif
		}
		break;

	case CG_TRIGGER_ACTION_NAK:
		{
			MSG_CG_TRIGGER_ACTION_NAK	*pRecvPacket = (MSG_CG_TRIGGER_ACTION_NAK*)pMsg;

			//	TRIGGER_ACTION_NAK - BattleScene
			if (GENERALPARAM->GetSpecialMode())
			{
				WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("TRIGGER_ACTION_NAK - BattleScene"));
			}
		
		}
		break;

	case CG_TRIGGER_PORTAL_ACK:
		{
			MSG_CG_TRIGGER_PORTAL_ACK* pRecvPacket = (MSG_CG_TRIGGER_PORTAL_ACK*)pMsg;

			pRecvPacket->Decode();

			//마을이나,특정필드로 이동한다.
			if (pRecvPacket->m_LeaveType == MSG_CG_TRIGGER_PORTAL_ACK::eLeaveType_Target)
			{
				//돌어갈 마을값을 서버값에 따라 바꾸어준다.
				if (g_pHero)
				{
					g_pHero->GetCharInfo()->m_iRegion = pRecvPacket->m_MapCode;
				}

				break;
			}

			//	이동할 멥정보를 받았으니 로드 Scene 으로 넘어가자

			g_LoadScene.SetNextSceneAfterLoaded( GameFramework::GetCurrentScene()->GetType(), (MAPCODE)pRecvPacket->m_MapCode, pRecvPacket->m_FieldCode);
			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
		}
		break;


	case CG_TRIGGER_SHOW_THE_FLOOR_CMD:
		{
			MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD	*pRecvPacket = (MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD*)pMsg;

			if( Object* pObject = g_ObjectManager.GetObject(pRecvPacket->m_dwPlayerKey) )
			{
				if( pObject->IsKindOfObject( PLAYER_OBJECT ) == TRUE )
				{
					Player* pPlayer = static_cast<Player*>(pObject);
					pPlayer->SetFloor(pRecvPacket->m_byFloor);
				}
			}

			if(g_pHero && g_pHero->GetObjectKey() == pRecvPacket->m_dwPlayerKey)
			{
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
                SetCurrentRadarLayer((int)pRecvPacket->m_byFloor);
#else
				g_InstanceDungeonScene.SetCurLayer((int)pRecvPacket->m_byFloor);
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
			}
		}
		break;

	// 보상창 여셈
	case CG_TRIGGER_REWARD_WINDOW_CMD:
		{
			MSG_CG_TRIGGER_REWARD_WINDOW_CMD *pRecvPacket = (MSG_CG_TRIGGER_REWARD_WINDOW_CMD*)pMsg;
			if (g_pHero) 
            {				
				RewardDialog *pDlg = (RewardDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_REWARD );
				if (pDlg)
                {
					WORD wRewardCode = pRecvPacket->m_RewardCode;
					pDlg->SetMissionReward(wRewardCode);

				}

				g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_REWARD, TRUE);

				m_bMissionReward = TRUE;

				uiBattleScoreMan* battle_score_manager_ptr = 
					static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
				if (battle_score_manager_ptr && battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_MISSION)
				{
                    battle_score_manager_ptr->set_toggle_ui_open(true);
					battle_score_manager_ptr->SendPacketBattleResultSYN();
				}
			}
		}
		break;

	case CG_TRIGGER_REWARD_ACK:
		{
			MSG_CG_TRIGGER_REWARD_ACK * pRecvPacket = (MSG_CG_TRIGGER_REWARD_ACK*)pMsg;

			if( pRecvPacket->m_byFatigueType >= RC_FATIGUE_REWARD_OVER_THREE_HOURS )
			{
				TCHAR szText[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString( 6045, szText, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( static_cast<eOUTPUT_MSG_TYPE>( eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW ), szText );
			}


			g_InterfaceManager.ShowGetMoneyMessage(pRecvPacket->m_Money);

			if (g_pHero) 
			{
				g_pHero->SetMoney(pRecvPacket->m_Money);


				EXPTYPE	exp = pRecvPacket->m_i64Exp;			// 최종 경험치				
				EXPTYPE prevExp = g_pHero->GetCharInfo()->m_i64Exp;

				g_pHero->SetExp(exp);


				EXPTYPE expoffset = exp - prevExp;
				if( expoffset > 0 )
				{
					PopupExp(expoffset);
				}
			    uiBattleScoreMan* battle_score_manager_ptr = 
                    static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			    if (battle_score_manager_ptr)
			    {
                    uiBattleScoreMan::eBTSCROE_MODE bts_mode = battle_score_manager_ptr->GetMode();
                    if (bts_mode == uiBattleScoreMan::eBTS_MISSION || bts_mode == uiBattleScoreMan::eBTS_SSQ)
                    {
                        uiBattleScoreMan::eBTSCROE_MODE bts_mode = battle_score_manager_ptr->GetMode();
                        if (bts_mode == uiBattleScoreMan::eBTS_MISSION || bts_mode == uiBattleScoreMan::eBTS_SSQ)
                        {
				            battle_score_manager_ptr->RefreshInfo(expoffset, 0);
                        }
                    }
			    }

				ItemManager::Instance()->InsertInventoryTotalInfo(REWARD, pRecvPacket->m_ItemInfo);
			}

			BOOL bRet = g_InterfaceManager.IsShowDialog( InterfaceManager::DIALOG_REWARD);
			if ( bRet )	
			{

#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
				//길드원추가 보너스 Exp를 받았다
				if(pRecvPacket->m_bGuildBounsExp)
				{
					SunEventInfoParser* const pSunEventInfoParser = SunEventInfoParser::Instance();

					if(pSunEventInfoParser)
					{
						EVENT_INFO* const pEventInfo = pSunEventInfoParser->FindEventInfoFromMode( eEVENT_GUILD_MISSION_EVENT );

						//길드원추가 보너스 이벤트가 존재할때
						if(pEventInfo)
						{
							TCHAR szTemp[INTERFACE_STRING_LENGTH];
							TCHAR szInterfaceString[INTERFACE_STRING_LENGTH];
							g_InterfaceManager.GetInterfaceString( 7249, szTemp, INTERFACE_STRING_LENGTH);
							int nRewardRatio = (int)pEventInfo->m_fRewardExpRate*100;
							Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, szTemp, nRewardRatio);
							GlobalFunc::default_OutputMsg( eOUTPUT_MSG_CENTER_SHOW | eOUTPUT_MSG_CHAT, szInterfaceString );
						}
					}
				}
#endif//_NA_20081121_ABUMAN_NIJOT_EVENT
				g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_REWARD, FALSE);
			}

			BattleScene::m_bMissionReward = FALSE;

			//포탈을 통해 미션입장했을경우에는 재도전창을 뛰우지않는다.
			if (pRecvPacket->m_bPortalMission)
			{
				break;
			}

#ifdef _DH_BATTLEZONE2_
#else
			//보상창 닫히면 재도전창 연다.
			uiPartyMan* partyMan =
				static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
			if (partyMan && (TRUE == partyMan->IsLeader()))
			{
				if (SCENE_TYPE_MISSION == GameFramework::GetCurScene())
				{
					MissionRelay* pMissionRelay = (MissionRelay*)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_MISSION_RELAY );
					if (pMissionRelay)
					{
						pMissionRelay->ShowInterface(TRUE);
					}
				}
			}
#endif//_DH_BATTLEZONE2_
		}
		break;

	case CG_TRIGGER_REWARD_NAK:
		{
			TCHAR szText[256];

			const int NOT_ENOUGH_INVENTORY_FOR_REWARD = 5369;

			g_InterfaceManager.GetInterfaceString(NOT_ENOUGH_INVENTORY_FOR_REWARD,szText,512);		
			GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, szText );

			 BOOL bRet = g_InterfaceManager.IsShowDialog( InterfaceManager::DIALOG_REWARD);
			if ( bRet )	
			{

				// 마을에서는 창을 닫지 않고 계속 뻐팅긴다.
				if ( SCENE_TYPE_VILLAGE != GameFramework::GetCurScene() )
				{
					g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_REWARD, FALSE);
				}
				else
				{
					RewardDialog *pDlg = (RewardDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_REWARD );
					if (pDlg) 
                    {
						pDlg->EnableSendPacket();
					}
				}
			}
		}
		break;

	case CG_TRIGGER_DONE_ACTION_INFO_CMD:
		{
			MSG_CG_TRIGGER_DONE_ACTION_INFO_CMD	*pRecvPacket = (MSG_CG_TRIGGER_DONE_ACTION_INFO_CMD *)pMsg;

//////////////////////////////////////////////////////////////////////////
			if(m_pTriggerMan && m_bTriggerInit)
			{
				int iCount = pRecvPacket->m_byCount;
				for (int a = 0; a < iCount; a++)
				{
					Trigger* pTrigger = 
						static_cast<Trigger*>(m_pTriggerMan->FindTrigger(pRecvPacket->m_dwDoneTriggerID[a]));
					if(pTrigger)
                    {
						pTrigger->_Action(TRUE);
                        uiMissionObject* dialog = GET_CAST_DIALOG(uiMissionObject, IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT);
                        if (dialog)
                        {
                            dialog->CheckMissionObjectShow(pTrigger->TriggerID());
                        }
                    }

				}
			}
//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CG_TRIGGER_AREADAMAGE_BRD:
		{
			MSG_CG_TRIGGER_AREADAMAGE_BRD	*pRecvPacket = (MSG_CG_TRIGGER_AREADAMAGE_BRD *)pMsg;

			// 트리거 데미지는 자해로 처리
			Character *pChr = (Character *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);
			if (!pChr)
			{
				break;
			}

			pChr->DoPeriodicDamage(
                pRecvPacket->m_wDamage,
                pRecvPacket->m_dwObjectKey,
                pRecvPacket->m_dwHP,
                GetBattleTimeSerial(),
                0);
		}		
		break;	

	case CG_TRIGGER_PLAY_MUSIC_CMD:
		{
			MSG_CG_TRIGGER_PLAY_MUSIC_CMD*	pRecvPacket = (MSG_CG_TRIGGER_PLAY_MUSIC_CMD*)pMsg;
			g_SoundBGM.Stop();
			g_SoundBGM.Play(pRecvPacket->m_nMusCode, TRUE, FALSE);
		}
		break;


	case CG_TRIGGER_DISPLAY_MSGBOX_CMD:
		{
			MSG_CG_TRIGGER_DISPLAY_MSGBOX_CMD* pRecvPacket = (MSG_CG_TRIGGER_DISPLAY_MSGBOX_CMD*)pMsg;
			GlobalFunc::DisplayMessageForTrigger(FALSE, pRecvPacket->m_nTextID, 0, 0, 0, 0);
		}
		break;
    case CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD:
        {
            MSG_CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD* pRecvPacket = (MSG_CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD*)pMsg;
            GlobalFunc::DisplayMessageForTrigger(FALSE, pRecvPacket->m_dwMessageCode, 0, 0, 0, pRecvPacket->m_dwObjectKey);
        }
        break;

    case CG_TRIGGER_CONTROL_TIMER_CMD:
        {
            MSG_CG_TRIGGER_CONTROL_TIMER_CMD* recv_packet = 
                static_cast<MSG_CG_TRIGGER_CONTROL_TIMER_CMD*>(pMsg);
            uiHelpMan* ui_manager = GET_DIALOG_MANAGER(uiHelpMan, UIMAN_HELP);
            if (ui_manager)
            {
                ui_manager->SetMissionTimer(recv_packet->m_dwTime, recv_packet->m_byControlType);
            }
        }
        break;
#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    case CG_TRIGGER_CHANGE_RADAR_MAP_CMD:
        {
            MSG_CG_TRIGGER_CHANGE_RADAR_MAP_CMD* recv_packet = 
                static_cast<MSG_CG_TRIGGER_CHANGE_RADAR_MAP_CMD*>(pMsg);

            WzID sub_map_id = recv_packet->sub_map_id;

            if (BattleScene::last_loaded_submapid_ == sub_map_id)
            {
                return;
            }

            BattleScene::last_loaded_submapid_ = sub_map_id;

            DWORD field_code = g_pMap->GetCurMMCode();
            if (field_code == 0)
            {
                return;
            }

            TCHAR file_name[MAX_PATH]= {0,};
            _snprintf( file_name, MAX_PATH-1, "map_%05d_%s", field_code, WzIDToString(sub_map_id));

            uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
            if (ui_radar_manager != NULL)
            {
                ui_radar_manager->ChangeRadarMap(file_name);
            }
        }
        break;
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
	}
}

void BattleScene::OnRecvCG_AREACONQUEST(MSG_BASE *pMsg)
{
}

void BattleScene::OnRecvCG_MISSION( MSG_BASE * pMsg )
{
	switch (pMsg->m_byProtocol)
	{
  
	case CG_MISSION_SET_CLEAR_COUNT_CMD:
		{
			// 미션 클리어 카운트 갱신
			MSG_CG_MISSION_SET_CLEAR_COUNT_CMD * pRecvPacket = (MSG_CG_MISSION_SET_CLEAR_COUNT_CMD *) pMsg;
			g_pMission_Concrete->SetMissionPoint((BYTE)pRecvPacket->m_MapCode, (INT)pRecvPacket->m_ClearCount);

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_MISSION_SET_CLEAR_COUNT_CMD : MAPCODE : %2d, CLEARCOUNT : %2d"), (INT)pRecvPacket->m_MapCode, (INT)pRecvPacket->m_ClearCount);
#endif
		}
		break;
  
	default:
		break;
	}


}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::OnRecvCW_CHAT( MSG_BASE * pMsg )
{
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    SceneBase::OnRecvCW_CHAT(pMsg);
    return;
#else
	switch( pMsg->m_byProtocol )
	{
	case CW_CHAT_NORMAL_NAK:
		{

			MSG_CW_CHAT_NORMAL_NAK * pRecvPacket = (MSG_CW_CHAT_NORMAL_NAK *)pMsg;
			if(pRecvPacket->m_wErrorCode == RC_CHAT_SYNC)
			{
				m_iPrevSectorIndex = -1;
				return;
			}

		}
		break;
	}

	SceneBase::OnRecvCW_CHAT(pMsg);
#endif
}

//-------------------------------------------------------------------------------------------
/**
*/
void  BattleScene::AddMonsterWithPacket(MONSTER_RENDER_INFO *pRenderInfo,float fHPMultiplier,BOOL bEnter,DWORD dwMapObjectID)
{
	BASE_NPCINFO* pNpcInfo = 
		NPCInfoParser::Instance()->GetNPCInfo(pRenderInfo->m_dwMonsterCode);
	
	if (!pNpcInfo)
	{
		assert(pNpcInfo);
		return;
	}
		
	Monster* pMonster = NULL;

    // pNpcInfo->m_wNpcImageIndex : 플레이어형몬스터를 만들기 위하여 NPCImageInfo.txt를 참조하기 위한 인덱스
    // NPCImageInfo.txt 를 통해 각 클래스와 부위별 파츠를 세팅함
    // * 타락한 사원의 플레이어형 몬스터와 저탑 보스 등으로 등장하는 플레이어형몬스터    
    // PlayerMonster::SetMonsterInfo() 를 통하여 저탑 보스 등으로 등장하는 플레이어형몬스터의 경우 MONSTER_OBJECT 세팅
	if (pNpcInfo->m_wNpcImageIndex)
	{
		BASE_NPCImageInfo* pNpcImageInfo = NPCImageInfoParser::Instance()->GetNpcImageInfo(pNpcInfo->m_wNpcImageIndex);

		if (pNpcImageInfo)
		{
			PlayerMonster* pPlayerMonster = (PlayerMonster*)g_ObjectManager.Add(pRenderInfo->m_dwObjectKey, SSQMONSTER_OBJECT, pNpcImageInfo->bClassCode, FALSE);

			pMonster = pPlayerMonster;

			if (pPlayerMonster)
			{
				//PLAYER_RENDER_INFO_EX  info;
				//ZeroMemory(&info,sizeof(PLAYER_RENDER_INFO_EX));
				//info.char_class=pNpcImageInfo->bClassCode;
				//info.level=10;
				//info.hp=(PLAYERHP)pNpcInfo->m_dwMaxHP;
				//info.max_hp=(PLAYERHP)pNpcInfo->m_dwMaxHP;
				//info.move_speed_ratio=100;
				//info.attack_speed_ratio=100;

				//pPlayerMonster->SetPlayerInfo(&info);
				//srand(pRenderInfo->m_dwMonsterCode);

                //pPlayerMonster->SetClass( pNpcImageInfo->bClassCode );
                //pPlayerMonster->SetMoveSpeedRatio(100);
                //pPlayerMonster->SetAttackSpeedModifier(100);

                // 외형세팅
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_HAIR, pNpcImageInfo->hairImageID);
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_FACE, pNpcImageInfo->faceImageID);
                BOOL is_hide_helmet = (pNpcImageInfo->showhelmet == 1) ? false : true;
                pPlayerMonster->HideHelmet(is_hide_helmet, FALSE);
                NPC::InitNpcImageInfo(pPlayerMonster, pNpcInfo);

                pPlayerMonster->RefreshEquipData();
                GameFunc::SetPlayerMonsterAttackStyle(pPlayerMonster);

                // 저탑 보스 등으로 등장하는 플레이어형몬스터의 경우 MONSTER_OBJECT 세팅
                pPlayerMonster->SetMonsterInfo(pNpcInfo);

			}
		}
	}
	else 
	{
		pMonster = (Monster*)g_ObjectManager.Add( pRenderInfo->m_dwObjectKey, MONSTER_OBJECT, pRenderInfo->m_dwMonsterCode, FALSE);

	}


	if (!pMonster)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER | c_wOUTPUT_LOG_CONSOLE, _T("몬스터 생성실패 ObjKey = %d MonsterCode = %d\n"), pRenderInfo->m_dwObjectKey, pRenderInfo->m_dwObjectKey);
		return;
	}

	pMonster->SetPosition(pRenderInfo->m_wvPos);

	WzVector vVisiblePos = pRenderInfo->m_wvPos;
	vVisiblePos.z += 2.0;
	float fT = 0.0f;

	int iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand(vVisiblePos, &fT, 10.0f,TMGO_NO_COLLISION);

	//타일로 바꾸자.
	if (iMesh >= 0 && fT)//딛을수 있는 곳인가
	{
		vVisiblePos.z -= fT;
	}

	pMonster->SetVisiblePos(vVisiblePos);
	pMonster->ChangeHPRatio(fHPMultiplier);
	pMonster->SetHP(pRenderInfo->m_dwHP);	
	pMonster->SetMaxHP(pRenderInfo->m_dwMaxHP);
	pMonster->SetMP(pNpcInfo->m_dwMaxMP);	//초기 mp값은 없음으로 서버에서 세팅하라고함-지완
	pMonster->SetAttackSpeedModifier(pRenderInfo->m_wAttSpeedRatio/100.f);
	pMonster->SetMoveSpeedModifier(pRenderInfo->m_wMoveSpeedRatio/100.f);
	pMonster->SetFreeMove(FALSE);
	pMonster->SetHeight(0.0f);

	if (0 == pMonster->GetHP())
    {
		pMonster->SetbCreateDeadBody(true);
    }

	if (pNpcInfo->m_byMoveAttitude == 2)
	{
		WzVector vDummyTarget;
		if (g_pMap->GetSpawnMonsterDirVector(pMonster->GetPosition(), vDummyTarget, 50.0f))
		{	
			pMonster->SetDirection(vDummyTarget);
		}
		else
		{
			pMonster->SetAngle(((pRenderInfo->m_wAngle) * WZ_PI / 180.0f), TRUE);
		}
	}
	else
	{
		pMonster->SetAngle(((pRenderInfo->m_wAngle) * WZ_PI / 180.0f), TRUE);
	}


    pMonster->SetStateTotalInfo(pRenderInfo->m_StateTotalInfo);

	if (bEnter) 
    {
		if (pNpcInfo->m_dwSpawnTime != 0)//스폰 되는 놈만 
		{
			PLAYER_ACTION idleaction;
			idleaction.SPAWN.dwEndTime = pNpcInfo->m_dwSpawnTime + g_CurTime;
			pMonster->SetCurrentAction(&idleaction);
			pMonster->SetNextState(STATE::SPAWN, g_CurTime);		
		}
		// 스폰애니없는놈은 스르륵 나타나지요
		else
		{
			pMonster->SetFadeIn(1000);
		}

		if (0 != pNpcInfo->m_dwSpawnEffectCode)
		{
			HANDLE hEffect = GlobalFunc::CreateEffectBeUpToOption(pNpcInfo->m_dwSpawnEffectCode, pMonster->GetVisiblePos());
		}
	}

    // 맵오브젝트 계열
    if ((pNpcInfo->m_byGrade == eNPC_MAPOBJECT_NPC) || 
        (pNpcInfo->m_byGrade == eNPC_DOMINATION_MAPOBJECT_NPC) ||
        (pNpcInfo->m_byGrade == eNPC_FRIEND_MAPOBJECT_NPC))
	{
		if (g_pMap)
		{
			pMonster->SetMapObjectKey(dwMapObjectID);

			APPEARANCEINFO info;
			if (g_pMap->GetMapObjectAppearinfo(dwMapObjectID, &info))
			{
				pMonster->SetAngle(info.wzRot.x, info.wzRot.y, info.wzRot.z);
			}

			MapObject* pMapObject = 
				(MapObject*)g_ObjectManager.GetObject(MAP_OBJECT_KEY + dwMapObjectID);

			if (!pMapObject)
			{
				assert(pMapObject);
				return;
			}

			if (pMapObject->IsSettingNPCObject())
			{
				pMapObject->SetShow(FALSE);

				WzVector wzRot = pMapObject->GetRotation();

				pMonster->SetAngle(wzRot.x, wzRot.y, wzRot.z);
				pMonster->SetMonsterType(MONSTER_TYPE_SETTING_MAPOBJECT);
			
				if (eNPC_DOMINATION_MAPOBJECT_NPC == pNpcInfo->m_byGrade)
				{
					uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
					if(NULL == pUIMan)
					{
						assert(pUIMan);
						return;
					}

					if(NULL == pUIMan->GetDominationGuildInfo())
					{
						assert(pUIMan->GetDominationGuildInfo());
						return;
					}

					pMonster->SetMasterGuildID(pUIMan->GetDominationGuildInfo()->m_GuildGuid);
				}
			}
		}
	}


#ifdef _DEV_VER
    // 자동킬
    if( CGeneralGameParam::Instance()->IsEnableAutoKill() == true )
    {
        char temp[1024];
        wsprintf(temp, "kill %d", pRenderInfo->m_dwObjectKey );
        GlobalFunc::SendGMCommand(temp);
    }
#endif
}

void BattleScene::AddNPCWithPacket(MERCHANT_RENDER_INFO* render_info)
{
    if (render_info == NULL)
    {
        return;
    }

    DWORD monster_code = render_info->m_sMonsterRenderInfo->m_dwMonsterCode;
    DWORD object_key = render_info->m_sMonsterRenderInfo->m_dwObjectKey;

    EXTRA_NPCINFO * ex_npc_info = NPCInfoParser::Instance()->GetExtraInfo(monster_code);
    if (ex_npc_info == NULL)
    {
        return;
    }

    NPC * npc_ptr = NULL;
    if (ex_npc_info->m_eNPCTYPE==eNPC_TYPE_LOTTO)
    {
        npc_ptr = (NPC *)g_ObjectManager.Add(object_key, 
                                             LOTTO_NPC_OBJECT, 
                                             ex_npc_info->m_NPCCODE);
    }
    else
    {
        npc_ptr = (NPC *)g_ObjectManager.Add(object_key, 
                                             NPC_OBJECT, 
                                             ex_npc_info->m_NPCCODE );	
    }

    if (npc_ptr == NULL) 
    {
        return;
    }

    NPC::InitNpcImageInfo(npc_ptr, npc_ptr->GetMonsterInfo());
    npc_ptr->SetPosition(render_info->m_sMonsterRenderInfo->m_wvPos);
    npc_ptr->SetAngle(D3DXToRadian(render_info->m_sMonsterRenderInfo->m_wAngle));
    npc_ptr->SetExtraInfo(ex_npc_info);
    npc_ptr->SetMaxHP(render_info->m_sMonsterRenderInfo->m_dwMaxHP);
    npc_ptr->SetHP(render_info->m_sMonsterRenderInfo->m_dwHP);
    CWzUnitDraw* unit_draw_ptr = npc_ptr->GetBasePart();
    if (unit_draw_ptr == NULL)
    {
        return;
    }

    WzVector eff_pos, eff_rot, eff_scale;
    SetVector(&eff_rot, 0.0f, 0.0f, 0.0f);
    SetVector(&eff_scale, 1.0f, 1.0f, 1.0f);

    int nNumEffect = unit_draw_ptr->GetNumUnitEffectInfo();
    for (int i=0; i<nNumEffect; ++i)
    {
        WzUnitEffectInfo* pei = unit_draw_ptr->GetUnitEffectInfo(i);
        if (pei == NULL)
        {
            continue;
        }

        CEffectBody* pEffectBody = 
            g_pEffectManager->GetEffectPtrHandle(pei->m_hEffect);
        if (pEffectBody != NULL)
        {
            eff_pos = npc_ptr->GetPosition() + pei->m_wvPosition;
            pEffectBody->SetResultMatrix(eff_pos, eff_rot, eff_scale);
        }
    }

    RefreshNPCStateForQuest();
}

//-------------------------------------------------------------------------------------------
/**
*/
void  BattleScene::AddTotemMonsterWithPacket(TOTEMNPC_RENDER_INFO *pRenderInfo,float fHPMultiplier)
{
	BASE_NPCINFO * pNpcInfo = 
		NPCInfoParser::Instance()->GetNPCInfo(pRenderInfo->m_dwMonsterCode);

	if (!pNpcInfo)
	{
		assert(pNpcInfo);
		return;
	}

	Monster * pMonster = (Monster*)g_ObjectManager.Add( 
		pRenderInfo->m_dwObjectKey, 
		MONSTER_OBJECT, 
		pRenderInfo->m_dwMonsterCode, 
		FALSE);

	if(!pMonster)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER | c_wOUTPUT_LOG_CONSOLE, _T("토템몬스터 생성실패 Protocol = %d ObjKey = %d MonsterCode = %d\n"), pRenderInfo->m_dwObjectKey, pRenderInfo->m_dwObjectKey);
		return;
	}


	pMonster->SetPosition( pRenderInfo->m_wvPos );

	WzVector vVisiblePos=pRenderInfo->m_wvPos;
	vVisiblePos.z+=2.0;
	float fT=0.0f;

	int iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 5.0f,TMGO_NO_COLLISION);

	//타일로 바꾸자.
	if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
	{
		vVisiblePos.z -= fT;
	}

	pMonster->SetVisiblePos(vVisiblePos);
	pMonster->ChangeHPRatio(fHPMultiplier);
	pMonster->SetHP( pNpcInfo->m_dwMaxHP );
	pMonster->SetMP(pNpcInfo->m_dwMaxMP);
	
	pMonster->SetFreeMove(FALSE);
	pMonster->SetHeight(0.0f);

	BASE_ITEMINFO * pItemInfo = 
		(BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(pRenderInfo->m_ItemCode);

	if(pItemInfo&&pItemInfo->m_dwEffectCode!=Null_Value)
	{
		HANDLE hHandle=GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode,pMonster->GetVisiblePos(),pRenderInfo->m_dwObjectKey);

		pMonster->SetRangeEffectCode(hHandle);
	}

	pMonster->SetMonsterType(MONSTER_TYPE_TOTEM);
}


void BattleScene::RenderGMResultCode(DWORD dwCode)
{		
	TCHAR szMessage[INTERFACE_STRING_LENGTH];			
	switch( (eGM_RESULT) dwCode)						
	{													
	case RC_GM_ROOM_NOT_EXIST:							
		{					
			// 방이 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_ROOM_NOT_EXIST, szMessage, INTERFACE_STRING_LENGTH);		
		}												
		break;											
														
	case RC_GM_INVALID_ARGUMENT:
		{
			// 잘못된 인자값 이다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_INVALID_ARGUMENT, szMessage, INTERFACE_STRING_LENGTH);
		}
		break;
	case RC_GM_ISNOT_GM:
		{
			// GM이 아니다
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_ISNOT_GM, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;

	case RC_GM_NOTEXIST_USER:
		{
			// USER가 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_NOTEXIST_USER, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;

	case RC_GM_CANNOT_RESURRECTION:
		{
			// 부활할수 없다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_CANNOT_RESURRECTION, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;

	case RC_GM_CANNOT_RECOVERY:
		{
			// 회복할수 없다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_CANNOT_RECOVERY, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;
	case RC_GM_PLAYER_NOTEXIST:
		{
			// 그런 플레이어가 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_PLAYER_NOTEXIST, szMessage, INTERFACE_STRING_LENGTH);	

		}
		break;
	case RC_GM_NOT_KILL_MYSELF:
		{
			// 자기 자신을 죽일수 없다.
            g_InterfaceManager.GetInterfaceString(eST_RC_GM_NOT_KILL_MYSELF, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;
	case RC_GM_CURROOM_ISNOT_CHUNTING:
		{
			// 현재 방타입이 경쟁헌팅이 아니다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_CURROOM_ISNOT_CHUNTING, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;

	case RC_GM_NOTEXIST_SCRIPT:
		{
			// 해당 스크립트 파일이 존재하지 않습니다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_ISNOT_SCRIPT_FILE, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;

	case RC_GM_INVALID_VERSION_TYPE:
		{
			// 해당 타입의 버전이 존재하지 않습니다.
			g_InterfaceManager.GetInterfaceString(eST_RC_GM_ISNOT_SCRIPT_VERSION, szMessage, INTERFACE_STRING_LENGTH);	
		}
		break;


    default:
        //항상 default 에 대한 처리를 해두시는 편이 좋습니다. - j0 
        Sprintf(szMessage, _T("GM STRING Invlaid Error Code(%d)"), dwCode);
        break;
	}

    if (GENERALPARAM->GetSpecialMode())
	    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
}

//------------------------------------------------------------------------------
/** 로비상에서 이루어지는 플레이어 Sync 관련 처리
*/
void 
BattleScene::OnRecvCG_SYNC_In_Lobby( MSG_BASE * pMsg )
{
	assert(IsLobby());

	if ( !IsLobby() )
		return;

	uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
	if (!pZoneMan)
	{
		return;
	}

	switch (pMsg->m_byProtocol)
	{
	case CG_SYNC_PLAYER_ENTER_BRD:
		{
			// 여기 들어온놈은 마을에 있던녀석이거나 그렇지 않을것이다.
			// 마을에 있던 녀석이면 찾아서 레디창에 정보를 넘겨주고
			// 마을에 없었던 녀석이면 Add 이후 로비에게 정보를 넘겨준다.
			// 방장 정보는 등록되어 있는상태
			// 아니면 일반 유저로 넣는다.

			MSG_CG_SYNC_PLAYER_ENTER_BRD *pRecvPacket = (MSG_CG_SYNC_PLAYER_ENTER_BRD *)pMsg;

			RESOURCECODE   rcCode=0;    
			DWORD     dwSkillCode=0;
			eTRANSFORM_TYPE type=eTRANSFORM_TYPE_NONE;

			PLAYER_RENDER_INFO_EX *pRenderInfo = (PLAYER_RENDER_INFO_EX*)( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_PLAYER_ENTER_BRD) );
			PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
			int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

			if (g_pHero) 
			{
				if (g_pHero->GetObjectKey() == output.player_key)
				{
					//스타일설정만
					g_pHero->SetCurrentAttackStyle(output.selected_style_code);
					break;
				}
			}

			for( int j = 0; j < output.state_total_info.m_Count; ++j )
			{
				STATE_INFO & StateInfo = output.state_total_info.m_Slot[j];

				SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)StateInfo.m_SkillCode);
				if(pInfo)
				{
					BASE_ABILITYINFO * pAbility = pInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );
					//assert(pAbility);

					if(pAbility)
					{
						rcCode= pAbility->m_iOption1;
						type= (eTRANSFORM_TYPE)pAbility->m_iOption2;
						dwSkillCode=StateInfo.m_SkillCode;
						break;
					}
				}
			}

			Player* pPlayer = (Player*)g_ObjectManager.GetObject(output.player_key);

			if (!pPlayer)
			{
                DWORD class_code = output.char_class;
                ClassCodeEx class_code_ex;
                class_code_ex.class_type = output.char_class;
                class_code_ex.gender = output.gender;
                class_code = class_code_ex.class_code;
				pPlayer = (Player*)g_ObjectManager.Add( output.player_key, PLAYER_OBJECT, class_code, FALSE);
			}
			assert(pPlayer);
			
			if(pPlayer)
			{
				pPlayer->SetPosition( output.position );
				pPlayer->SetPlayerInfo(&output);

				//쉴드정보가 이중간에 들어온다
				ETC_STATE_TOTAL_INFO *pShield = (ETC_STATE_TOTAL_INFO*)( (BYTE*)(pRenderInfo) + nSize );
				for( int a = 0; a < pShield->m_Count; ++a )
				{
					ETC_STATE_INFO & rInfo = pShield->m_Slot[a];
					if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( rInfo.m_wStateCode, rInfo.m_iLeavedTime ) )
                    {
                        pPlayer->AddSkillEffect(pEffect);
                    }
				}
				//이큅정보
				EQUIP_ITEM_BASEINFO *pEquipInfo = (EQUIP_ITEM_BASEINFO*)( (BYTE*)(pShield) + pShield->GetSize() );
                
                EQUIP_ITEM_BASEINFO::data_type *pItemSlot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)(pEquipInfo) + sizeof(EQUIP_ITEM_BASEINFO));
				pPlayer->SetEquipItemBaseInfo( pEquipInfo->m_Count, pItemSlot );



				if(dwSkillCode)
				{
					pPlayer->GetTransformData().SetTransFormType(type);
					pPlayer->GetTransformData().SetTransFormResourceCode(rcCode);
					pPlayer->SetTransSkillCode(dwSkillCode);
					pPlayer->ForceTransFrom();
				}

				if (pZoneMan->GetLobbyMasterPlayerKey() != output.player_key)
				{
					UserSlot EnterUser;
					EnterUser.SetPlayerKey( pPlayer->GetObjectKey() );
					pZoneMan->AddPlayer( EnterUser );
				}
			}	
		}
		break;

	case CG_SYNC_PLAYER_LEAVE_BRD:
		{
			// 로비에서 해당 유저의 정보를 지워준다.
			// 방장이 나가면 모두들 돌아가야할 것이다.
			MSG_CG_SYNC_PLAYER_LEAVE_BRD *pRecvPacket = (MSG_CG_SYNC_PLAYER_LEAVE_BRD *)pMsg;

			for( int i = 0; i < pRecvPacket->m_byCount; ++i )
			{
				DWORD dwObjectKey = pRecvPacket->m_PlayerKey[i];

				if(g_pHero && (dwObjectKey == g_pHero->GetObjectKey()))	
				{
					assert(0);
					continue;
				}

				Player * pPlayer = static_cast<Player *>( g_ObjectManager.GetObject(dwObjectKey) );
				if( !pPlayer )
				{
					continue;
				}			

				pPlayer->ProcessAllRemainActionResult();
				g_ObjectManager.Delete( dwObjectKey );

				pZoneMan->LeavePlayer( dwObjectKey );
			}
		}
		break;


	case CG_SYNC_ALLPLAYERS_CMD:
		{
			BattleScene::OnRecvCG_SYNC(pMsg);

			MSG_CG_SYNC_ALLPLAYERS_CMD *pRecvPacket = (MSG_CG_SYNC_ALLPLAYERS_CMD *)pMsg;

			BYTE * pPacket = ( (BYTE*)(pRecvPacket) + sizeof(MSG_CG_SYNC_ALLPLAYERS_CMD) );

			PLAYER_RENDER_INFO_EX * pRenderInfo = NULL;

			ETC_STATE_TOTAL_INFO *pShield = NULL;

			VENDOR_RENDER_INFO * pVendorInfo = NULL;

			for (int i = 0; i < pRecvPacket->m_byCount; i++)
			{
				pRenderInfo = (PLAYER_RENDER_INFO_EX *)pPacket; 
				PLAYER_RENDER_INFO_EX output; // 초기화 필요 없음.
				int nSize = PLAYER_RENDER_INFO_EX::ConvertForRecv(pRenderInfo, &output);

				pShield = (ETC_STATE_TOTAL_INFO *)( (BYTE*)(pPacket) + nSize );
				pVendorInfo = (VENDOR_RENDER_INFO*)( (BYTE*)(pShield) + pShield->GetSize() );

				if (pZoneMan->GetLobbyMasterPlayerKey() == output.player_key)
				{
					// 방장은 미리등록해놓은 상태이다.

				}
				else
				{
					UserSlot EnterUser;
					EnterUser.SetPlayerKey( output.player_key);

					pZoneMan->AddPlayer( EnterUser );
				}

				pPacket += GetPlayerSize(pPacket,nSize);
			}

			MSG_CG_ZONE_LOBBY_REQUEST_INFO_SYN Sendpacket;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &Sendpacket, sizeof(Sendpacket));

		}
		break;
	case CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD:
		{
			BattleScene::OnRecvCG_SYNC(pMsg);
		}
		break;

	}
}

//----------------------------------------------------------------------------
/** 로비로 들어가기전 단계( 현재는 로비가 아니다. )
*/
void 
BattleScene::OnRecvCG_SYNC_In_Lobby_Pending( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	BattleLobby * pReadyDlg = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
	if (!pReadyDlg )
	{
		assert(0);
		return;
	}

	switch (pMsg->m_byProtocol)
	{

	case CG_SYNC_PLAYER_ENTER_ACK:
		{

			// 방장이면 방장 정보를 로비 창에 보여주고
			// 아니면 일반 유저로 넣는다.
			// 마을에서 로비로 갈때 플레이어 정보를 안지우기 때문에 바로 찾아서 셋한다.
			MSG_CG_SYNC_PLAYER_ENTER_ACK	*pRecvPacket = (MSG_CG_SYNC_PLAYER_ENTER_ACK *)pMsg;

			pRecvPacket->Decode();

			assert( g_pHero );

			if(!g_pHero)
				return;

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
			CHANGE_PCROOM_STATUS::eStatus PCBangStatus = (CHANGE_PCROOM_STATUS::eStatus)pRecvPacket->m_PcRoomStatus.status_;
			g_pHero->SetPCBangMode(CHANGE_PCROOM_STATUS::eStatus_PcRoom == PCBangStatus);

			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "CHANGE_PCROOM_STATUS : %d (CUR : %d)", PCBangStatus, g_pHero->GetPCRoomStatus());

			if (CHANGE_PCROOM_STATUS::eStatus_None != g_pHero->GetPCRoomStatus() && g_pHero->GetPCRoomStatus() != PCBangStatus)
			{
				int StringCode = 0;
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				if (PCBangStatus == CHANGE_PCROOM_STATUS::eStatus_PcRoom)
				{
					StringCode = 6071;
				}
				else
				{
					StringCode = 6072;
				}				
				g_InterfaceManager.GetInterfaceString(StringCode, szMessage);
				GlobalFunc::OutputMsg_SysMsg(szMessage);
			}
			g_pHero->SetPCRoomStatus(PCBangStatus);
#endif

			Player * pPlayer = static_cast<Player *>( g_ObjectManager.GetObject(g_pHero->GetObjectKey()) );
			if ( NULL == pPlayer || pPlayer != g_pHero)
			{
				assert(!"머야 약속과 다르다!!마을에서 받았던 키를 그대로 사용한다 했는데!!");
				return;
			}

			g_InterfaceManager.ShowGameInterface(FALSE);

			uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
			if (pZoneMan && (pZoneMan->GetLobbyMasterPlayerKey() != g_pHero->GetObjectKey()))
			{
				UserSlot EnterUser;
				EnterUser.SetPlayerKey( g_pHero->GetObjectKey() );
				BOOL bRet = pZoneMan->AddPlayer( EnterUser );	
				assert(bRet);
			}

			// 글로우 켜라.
			m_bPlayerAck = TRUE;
			m_bDrawWait = FALSE;

			// 로비에 들어오면 셋해준다. 
			m_bComebackFromLobby = TRUE;		

			pReadyDlg->ShowWindow(TRUE);
			
			BattleScene::SetLobbyFlag(TRUE);		// --> 로비 셋
			BattleScene::SetLobbyInPending(FALSE);

			// TODO : 여기서 몬스터와 NPC를 지우는것이 낫겠다. 
			// 왜냐? Nak 받으면 마을로 가야하기때문에!!

			// 히어로는 마을에서 삭제하지 않아서 달리고 있는 스테이터스가 
			// 유지될 수 있어서 플래그를 초기화 해준다.
			g_pHero->StopMove();
			g_pHero->ResetMoveFlag();
			g_HeroInput.SetAutoRun(FALSE);
			g_HeroInput.SetFollowState( FALSE );
			g_pHero->StopMove();	

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
			//RankTimer 닫기.
			if(g_pMap && g_pMap->IsMission())
			{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
                uiMissionRankMan2* rank_manager_ptr2 =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
                if ( rank_manager_ptr2 )
                {
                    rank_manager_ptr2->CloseMissionRunningDlg();
                }
#else
				uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );
				if(pRankMan)
					pRankMan->CloseMissionRunningDlg();
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
			}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
		}
		break;


	case CG_SYNC_PLAYER_ENTER_NAK:
		{
			MSG_CG_SYNC_PLAYER_ENTER_NAK * pRecvMsg = (MSG_CG_SYNC_PLAYER_ENTER_NAK *)pMsg;

			BattleScene::SetLobbyInPending(FALSE);

			if (g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH ))
			{
		
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock(szMessage);
			}
			g_InterfaceManager.HideAllWindows();
			g_InterfaceManager.ShowGameInterface(TRUE);
		}
		break;

	default:
		{
			
		}
		break;
	}
}

//----------------------------------------------------------------------------
/** 현재는 로비이나 로비를 떠나기 전.
*/
void 
BattleScene::OnRecvCG_SYNC_In_Lobby_Out_Pending(MSG_BASE * pMsg)
{
	switch (pMsg->m_byProtocol)
	{
	case CG_SYNC_PLAYER_ENTER_ACK:
		{
			BattleScene::SetLobbyOutPending(FALSE);
			BattleScene::SetLobbyFlag(FALSE);

			OnRecvCG_SYNC(pMsg);

			//미션 재시도후 로비에서 마을로 이동시에는 ShowGameInterface(TRUE)
			if( GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->IsVisible() )
			{
				g_InterfaceManager.ShowGameInterface( FALSE );
			}
		}
		break;

	case CG_SYNC_PLAYER_ENTER_NAK:
		{
			OnRecvCG_SYNC(pMsg);
			g_InterfaceManager.ShowGameInterface(FALSE);
		}
		break;

	default:
		{
			
		}
		break;
	}	

}

//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_BATTLE_MONSTER_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_MONSTER_ATTACK_BRD* recv_packet)
{
    Character *pChr = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwObjectKey);
    Character* target = (Character*)g_ObjectManager.GetObject(recv_packet->m_dwTargetObjectKey);

    if (target)
    {
        target->SetReserveHP(recv_packet->m_wTargetReserveHP);
    }

    if (pChr && pChr->IsKindOfObject(SSQMONSTER_OBJECT))
    {
        PlayerMonster *pMon = (PlayerMonster *)pChr;

        if (pMon->GetClass() == eCHAR_VALKYRIE && pMon->GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW)
        {
            OnRecvVkrAttack(recv_packet->m_dwObjectKey,
                pChr->GetNextAttackSerial(),
                recv_packet->m_dwTargetObjectKey,
                recv_packet->m_wDamage,
                recv_packet->m_wTargetHP,
                recv_packet->m_byEffect);
            return;
        }
    }

    if (g_pHero && (pChr != NULL) && (!pChr->IsDead()))
    {
        assert(pChr->IsKindOfObject(MONSTER_OBJECT));

        if (recv_packet->m_dwTargetObjectKey == g_pHero->GetObjectKey())
        {
            if (g_HeroInput.GetCurrentTarget() == 0)
            {
                //	주인공이 타겟을 잡고 있는 놈이 없다
                //	따라서 날 공격한 이 놈을 자동으로 타겟으로 설정

                if (g_HeroInput.GetAutoTarget())
                {
                    g_HeroInput.SetCurrentTarget(pChr->GetObjectKey());
                }

            }
        }

        // 몬스터도 큐큐큐!
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;
        action.ATTACK.byAttackType = recv_packet->m_byAttackType;		// 몬스터는 특별히 attack type이 없다.			 
        action.ATTACK.dwTargetID = recv_packet->m_dwTargetObjectKey;			
        action.ATTACK.vCurPos.wvPos = recv_packet->m_wvCurPos; 
        action.ATTACK.vCurPos.wTile = -1;
        action.ATTACK.vDestPos.wvPos = recv_packet->m_wvCurPos;
        action.ATTACK.vDestPos.wTile = -1;
        action.ATTACK.dwAttackSerial = pChr->GetNextAttackSerial();
        action.ATTACK.bAttackMove = FALSE;
        action.ATTACK.dwAttackStyle = pChr->GetCurrentAttackStyle();

        if (GENERALPARAM->IsServerDebug())
        {
            pChr->DoAction(&action);
            pChr->CommitChangeState(g_CurTime);	
        }
        else 
        {					
            pChr->PutAction(&action);
        }

        ATTACK_RESULT attackresult;
        attackresult.SetAttackSerial(pChr->GetNextAttackSerial());
        attackresult.SetTimeSerial(GetBattleTimeSerial());
        attackresult.SetGroupID(1);

        if (pChr->IsKindOfObject(MONSTER_OBJECT)) 
        {
            Monster *pMonster = (Monster *)pChr;
            // 2번 이상부터 마법공격이다.
            if (pMonster->GetMonsterInfo()->m_byAttType >= 2)
            {
                attackresult.SetWeaponSound(eWEAPONSOUNDKIND_MONSTERMAGICAL);
            }
            else
            {
                attackresult.SetWeaponSound(eWEAPONSOUNDKIND_MONSTERPHYSICAL);
            }
        }

        attackresult.GetTargetData().SetTargetObjectKey( recv_packet->m_dwTargetObjectKey );
        attackresult.GetTargetData().SetDamage( recv_packet->m_wDamage );
        attackresult.GetTargetData().SetTargetHP( recv_packet->m_wTargetHP );
		attackresult.GetTargetData().SetNoDamage( FALSE );
		attackresult.SetSkillCode(0);

		if (attackresult.GetTargetData().GetDamage() == 0)
		{
			attackresult.GetTargetData().SetMiss( TRUE );
		}

        if (recv_packet->m_byEffect & SKILL_EFFECT_CRITICAL)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL );
        }

        if (recv_packet->m_byEffect & SKILL_EFFECT_DOUBLEDAMAGE)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOUBLE );
        }

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        // 2013.04.25 몬스터에게 맞은 대상이 플레이어라면 대상의 쉴드 수치를 유지한다.
        if (target && target->IsKindOfObject(PLAYER_OBJECT))
        {
            attackresult.GetTargetData().SetTargetSD(target->GetSD());
        }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        pChr->AddAttackResult(&attackresult);

        // 서버 디버그모드일경우는 바로 데미지 처리.
        if (GENERALPARAM->IsServerDebug())
        {
            pChr->ProcessAttackResult(attackresult.GetAttackSerial(), TRUE);
        }

        if (GENERALPARAM->GetSpecialMode()) 
        {
            if (g_pHero && (g_pHero->GetObjectKey() == recv_packet->m_dwTargetObjectKey))
            {
                if (recv_packet->m_wTargetHP == 0)
                {
                    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("플레이어 사망!!!!!!!!!"));							
                }
            }
        }

        pChr->AddNextAttackSerial();
    }
    else
    {
        Character* pAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwObjectKey);

        // 없으면 자해라도한다.
        if (!pAttacker)
        {
            pAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwTargetObjectKey);
        }
        else if (pAttacker&&pAttacker->IsDead())
        {
            pAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwTargetObjectKey);

        }

        // 그래도 없으면 어쩔수없구
        if (pAttacker) 
        {
            pAttacker->DoPeriodicDamage(recv_packet->m_wDamage, recv_packet->m_dwTargetObjectKey, recv_packet->m_wTargetHP, GetBattleTimeSerial(), 0);
        }				
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD* recv_packet)
{
    if( Character* pAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwObjectKey) )
    {
        Character* pTarget = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwTargetObjectKey);

        ATTACK_RESULT attackresult;
        attackresult.SetAttackSerial(pAttacker->GetNextAttackSerial());
        attackresult.SetTimeSerial(GetBattleTimeSerial());

        attackresult.GetTargetData().SetTargetObjectKey( recv_packet->m_dwTargetObjectKey );
        attackresult.GetTargetData().SetDamage( recv_packet->m_wDamage );
        attackresult.GetTargetData().SetTargetHP( recv_packet->m_wTargetHP );

		attackresult.GetTargetData().SetPosition( recv_packet->m_wvCurPos, pTarget ? pTarget->GetPosition() : pAttacker->GetPosition() );
        attackresult.GetTargetData().SetNoDamage( FALSE );
		if (attackresult.GetTargetData().GetDamage() == 0)
		{
			attackresult.GetTargetData().SetMiss( TRUE );
		}

        attackresult.GetTargetData().SetAdditionalEffect( ConvertServerToClientEffect(recv_packet->m_byEffect) );

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        // 2013.04.25 몬스터에게 맞은 대상이 플레이어라면 대상의 쉴드 수치를 유지한다.
        if (pTarget && pTarget->IsKindOfObject(PLAYER_OBJECT))
        {
            attackresult.GetTargetData().SetTargetSD(pTarget->GetSD());
        }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        pAttacker->AddAttackResult(&attackresult);
        AddAttackResultQueueAction(attackresult.GetTargetData(), GetBattleTimeSerial(), recv_packet->m_dwObjectKey);

        pAttacker->AddNextAttackSerial();
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_BATTLE_PLAYER_ATTACK_BRD(nsSync::MSGSUB_BATTLE_PLAYER_ATTACK_BRD* recv_packet)
{
    Object * pObject = NULL;
    Object * pTargetObject = NULL;
    Player	* pPlayer = NULL;
    Monster * pMonster = NULL;

    pObject = g_ObjectManager.GetObject(recv_packet->dwAttackerKey);
    if (!pObject)
    {
        return;
    }

    pTargetObject = g_ObjectManager.GetObject(recv_packet->dwTargetKey);
    if (!pTargetObject)
    {
        return;
    }

    // TODO : 
    // 오브젝트 안에서 이 처리를 해야할것임
    // 인자로 오브젝트를 넘겨서 처리함!!
    // 일단 임시로 처리해놓음.
    // 플레이어가
    pPlayer = static_cast<Player *>(pObject);

    // 타 플레이어에 대해서만 체크
    if (g_pHero && (recv_packet->dwAttackerKey != g_pHero->GetObjectKey()))
    {
        if (recv_packet->dwTargetKey == g_pHero->GetObjectKey())
        {
            if (g_HeroInput.GetCurrentTarget() == 0)
            {
                //	주인공이 타겟을 잡고 있는 놈이 없다
                //	따라서 날 공격한 이 놈을 자동으로 타겟으로 설정

                if (g_HeroInput.GetAutoTarget())
                {
                    g_HeroInput.SetCurrentTarget(recv_packet->dwAttackerKey);
                }

            }
        }

        // 무조건 큐에 넣는거다!
        // 상태전이는 무조건 Process 안에서만 일어난다!
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;
        action.ATTACK.bAttackMove = TRUE;
        action.ATTACK.byAttackType = recv_packet->byAttackType;					 
        action.ATTACK.dwTargetID = recv_packet->dwTargetKey;
        action.ATTACK.vCurPos.wvPos = recv_packet->wvCurPos;					
        action.ATTACK.vCurPos.wTile = -1;

#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        action.ATTACK.style_presentation = recv_packet->style_presentation;
#endif //_NA003109_STYLE_PRESENTATION_IMPROVEMENT

        action.ATTACK.vDestPos.wvPos = recv_packet->wvCurPos;
        action.ATTACK.vDestPos.wTile = -1;
        action.ATTACK.dwAttackSerial = recv_packet->dwClientSerial;
        action.ATTACK.dwAttackStyle = recv_packet->StyleCode;

        if (GENERALPARAM->IsServerDebug()) 
        {
            pPlayer->DoAction(&action);
            pPlayer->CommitChangeState(g_CurTime);
        }
        else
        {
            pPlayer->PutAction(&action);
        }		
    }

    // ATTACK_RESULT 구조체크기가 큰데 복사가 많이일어나 별로 안좋다
    // 후에 좀더 효율적인 사용법을 고려
    ATTACK_RESULT attackresult;
    attackresult.SetAttackSerial(recv_packet->dwClientSerial);
    attackresult.SetTimeSerial(GetBattleTimeSerial());
    attackresult.SetGroupID(1);
    attackresult.SetWeaponSound(GameFunc::GetWeaponSoundKind((eWEAPONTYPE)pPlayer->GetWeaponKind()));

	attackresult.SetSkillCode(0);
    attackresult.GetTargetData().SetTargetObjectKey( recv_packet->dwTargetKey );
    attackresult.GetTargetData().SetDamage( recv_packet->wDamage );
    attackresult.GetTargetData().SetTargetHP( recv_packet->dwTargetHP );
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    attackresult.GetTargetData().SetTargetSD(recv_packet->target_sd);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

    BOOL bMonsterFly = FALSE;

    if (recv_packet->byEffect & SKILL_EFFECT_CRITICAL)
    {
        pPlayer->SetbtCriAttkAni(1);
        attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL );

        if (attackresult.GetTargetData().GetTargetHP() == 0 )
        {
            if (pPlayer->IsFlyFinishWeapon())
            {
                bMonsterFly = TRUE;
            }
        }

        Character* pTarget = (Character*)g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey() );
        if (pTarget && pTarget->IsKindOfObject(MONSTER_OBJECT))
        {
            Monster* pMonster = (Monster*)pTarget;
            if (MONSTER_TYPE_SETTING_MAPOBJECT == pMonster->GetMonsterType())
            {
                // 설치형 몬스터면 날리지 말아라
                bMonsterFly = FALSE;
            }
        }
    }
    else
    {
        pPlayer->SetbtCriAttkAni(0);
    }

    if (recv_packet->byEffect & SKILL_EFFECT_DOUBLEDAMAGE)
    {
        attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOUBLE );
    }


    if (recv_packet->byEffect & SKILL_EFFECT_ETHERDEVICE_METAL)
    {
        attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL );
    }

    if (recv_packet->byEffect & SKILL_EFFECT_ETHERDEVICE_ETHER)
    {
        attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER );
    }

    //safe_code:5콤보가 맥스이다.
    if (recv_packet->byEtherComboCount > 5)
    {
        recv_packet->byEtherComboCount = 5;
    }

    attackresult.GetTargetData().SetComboCount( recv_packet->byEtherComboCount );


    if (attackresult.GetTargetData().GetDamage() == 0 )
    {
        pPlayer->SetbtCriAttkAni(2);
        attackresult.GetTargetData().SetMiss( TRUE );
    }

    if (pPlayer->GetShotCount() == 1) 
    {
        // 날려 버리는 무기여야한다.
        if (bMonsterFly)
        {
            if (!(rand() % MONSTER_FINISH_FLY_CHANCE))
            {							
                pTargetObject = g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey());

                if (pTargetObject)
                {

					WzVector vSrc = pTargetObject->GetPosition();
                    WzVector vDest = pPlayer->GetPosition();

                    WzVector vDirection = vDest - vSrc;
                    vDirection.z = 0.0f;
                    float fAngleZ = 0.0f;
                    if (0.0f != VectorLength2(&vDirection))
                    {
                        fAngleZ = Math_GetAngleFromVector(&vDirection);
                    }

                    ClampAngle(&fAngleZ);
                    vDirection.x = sin(fAngleZ);
                    vDirection.y = -cos(fAngleZ);
                    vDirection.z = 0.0f;

                    vDest = pTargetObject->GetPosition() + vDirection * -5.0f;

					attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK );
					attackresult.GetTargetData().SetPosition( vSrc, vDest );
                }

            }
        }

        pPlayer->AddAttackResult(&attackresult);

        AddAttackResultQueueAction( attackresult.GetTargetData(), GetBattleTimeSerial(), recv_packet->dwAttackerKey);				
    }
    else if( pPlayer->GetShotCount() > 1 )
    {
        AddSplitDamageResult( pPlayer, &attackresult, pPlayer->GetShotCount() );
    }

    // 서버 디버그모드일경우는 바로 데미지 처리.
    if (GENERALPARAM->IsServerDebug())
    {
        pPlayer->ProcessAttackResult(attackresult.GetAttackSerial(), FALSE);
    }

    // 주인공의 경우.
    // 현재 AttackSerial보다 늦게 온 놈이면 바로 처리해버린다.
    if (pPlayer == g_pHero)
    {
        if (attackresult.GetAttackSerial() < pPlayer->GetNextAttackSerial())
        {
            pPlayer->ProcessAttackResult(attackresult.GetAttackSerial(), TRUE);
        }
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD* recv_packet)
{
    Object* pObject = g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);
    if ( pObject == NULL )
    {
        return;
    }

    //공격 모션 
    Player *pPlayer = static_cast<Player *>(pObject);
    // 메인목표는 무조건 첫번째로~
    DWORD dwTargetKey = recv_packet->m_dwPrimaryTargetKey;
    // 타 플레이어에 대해서만 체크
    if (g_pHero && (recv_packet->m_dwAttackerKey != g_pHero->GetObjectKey()))
    {
        if (dwTargetKey == g_pHero->GetObjectKey())
        {
            if (g_HeroInput.GetCurrentTarget() == 0)
            {
                //	주인공이 타겟을 잡고 있는 놈이 없다
                //	따라서 날 공격한 이 놈을 자동으로 타겟으로 설정
                g_HeroInput.SetCurrentTarget(recv_packet->m_dwAttackerKey);
            }
        }

        // 무조건 큐큐큐!
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;
        action.ATTACK.bAttackMove = TRUE;				
        action.ATTACK.byAttackType = recv_packet->m_byAttackType;					 
        action.ATTACK.dwTargetID = dwTargetKey;
        action.ATTACK.vCurPos.wvPos = recv_packet->m_wvCurPos;					
        action.ATTACK.vCurPos.wTile = -1;


        action.ATTACK.vDestPos.wvPos = recv_packet->m_wvCurPos;
        action.ATTACK.vDestPos.wTile = -1;

        action.ATTACK.dwAttackSerial = recv_packet->m_dwClientSerial;
        action.ATTACK.dwAttackStyle = recv_packet->m_StyleCode;

        if (GENERALPARAM->IsServerDebug()) 
        {
            pPlayer->DoAction(&action);
            pPlayer->CommitChangeState(g_CurTime);
        }
        else
        {
            pPlayer->PutAction(&action);
        }								
    }

    //공격 결과 처리 

    assert(recv_packet->m_byNumberOfTargets < MAX_TARGET_NUM);
	cAttackResultData TargetInfo[MAX_TARGET_NUM];

    for (DWORD a = 0; a < recv_packet->m_byNumberOfTargets; a++) 
    {

        ATTACK_RESULT attackresult;
        attackresult.SetAttackSerial( recv_packet->m_dwClientSerial );
        attackresult.SetTimeSerial( GetBattleTimeSerial() );
        attackresult.SetGroupID(1);
        attackresult.SetWeaponSound( GameFunc::GetWeaponSoundKind((eWEAPONTYPE)pPlayer->GetWeaponKind()) );

        attackresult.GetTargetData().SetTargetObjectKey( recv_packet->AttackInfo[a].m_dwTargetKey );
        attackresult.GetTargetData().SetDamage( recv_packet->AttackInfo[a].m_wDamage );
        attackresult.GetTargetData().SetTargetHP( recv_packet->AttackInfo[a].m_dwTargetHP );
		attackresult.GetTargetData().SetNoDamage( FALSE );
		if (attackresult.GetTargetData().GetDamage() == 0 )
		{
			attackresult.GetTargetData().SetMiss( TRUE );
		}
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        attackresult.GetTargetData().SetTargetSD(recv_packet->AttackInfo[a].target_sd);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        // 날려보내기 피니시

        BOOL bFlyFinish = FALSE;

        if (recv_packet->AttackInfo[a].m_byEffect & SKILL_EFFECT_CRITICAL)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL );


            if (attackresult.GetTargetData().GetTargetHP() == 0 )
            {
                if (pPlayer->IsFlyFinishWeapon())
                {
                    bFlyFinish = TRUE;
                }
            }

        }

        if (recv_packet->AttackInfo[a].m_byEffect & SKILL_EFFECT_DOUBLEDAMAGE)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOUBLE );
        }


        if (recv_packet->AttackInfo[a].m_byEffect & SKILL_EFFECT_ETHERDEVICE_METAL)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL );
        }

        if (recv_packet->AttackInfo[a].m_byEffect & SKILL_EFFECT_ETHERDEVICE_ETHER)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER );
        }



        if( Character* pTarget = (Character*)g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey() ) )
        {
			attackresult.GetTargetData().SetPosition( pTarget->GetPosition(), pTarget->GetPosition() );

            if (pTarget->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMonster = (Monster *)pTarget;
                if (MONSTER_TYPE_SETTING_MAPOBJECT == pMonster->GetMonsterType())
                {
                    // 설치형 몬스터면 날리지 말아라
                    bFlyFinish = FALSE;
                }
            }
        }

        // 날려 버리기 판정이 떳으면
        if (bFlyFinish)
        {
            if (!(rand() % MONSTER_FINISH_FLY_CHANCE))
            {
                if( Object* pTargetObject = g_ObjectManager.GetObject( attackresult.GetTargetData().GetTargetObjectKey() ) )
                {
					WzVector vSrc = pTargetObject->GetPosition();
                    WzVector vDest = pPlayer->GetPosition();

                    WzVector vDirection = vDest - vSrc;
                    vDirection.z = 0.0f;
                    float fAngleZ = 0.0f;
                    if (0.0f != VectorLength2(&vDirection))
                    {
                        fAngleZ = Math_GetAngleFromVector(&vDirection);
                    }

                    ClampAngle(&fAngleZ);
                    vDirection.x = sin(fAngleZ);
                    vDirection.y = -cos(fAngleZ);
                    vDirection.z = 0.0f;

                    vDest = pTargetObject->GetPosition()+vDirection * -5.0f;

					attackresult.GetTargetData().SetPosition( vSrc, vDest );
					attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK );
                }
            }
        }

        TargetInfo[a] = attackresult.GetTargetData();
        pPlayer->AddAttackResult(&attackresult);				
    }

    //공격을 받는 대상의 상태를 변경 시킨다.공중 부양,밀리기등
	for( int i=0;i<recv_packet->m_byNumberOfTargets;++i)
	{
		AddAttackResultQueueAction( TargetInfo[i], GetBattleTimeSerial(), recv_packet->m_dwAttackerKey );
	}


    // 현재 AttackSerial보다 늦게 온 놈이면 바로 처리해버린다.
    if (recv_packet->m_dwClientSerial < pPlayer->GetNextAttackSerial() || GENERALPARAM->IsServerDebug())
    {
        pPlayer->ProcessAttackResult(recv_packet->m_dwClientSerial,TRUE);
    }

}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_BRD(const nsSync::MSGSUB_STYLE_PLAYER_ATTACK_BRD* recv_packet)
{
    Object *pObject = g_ObjectManager.GetObject(recv_packet->dwAttackerKey);
    if (!pObject)
    {
        return;
    }

    Player *pPlayer = static_cast<Player *>(pObject);
    // 메인목표는 무조건 첫번째로~
    DWORD dwTargetKey = recv_packet->dwPrimaryTargetKey;
    // 타 플레이어에 대해서만 체크
    if (g_pHero && (recv_packet->dwAttackerKey != g_pHero->GetObjectKey()))
    {
        if (dwTargetKey == g_pHero->GetObjectKey())
        {
            if (g_HeroInput.GetCurrentTarget() == 0)
            {
                //	주인공이 타겟을 잡고 있는 놈이 없다
                //	따라서 날 공격한 이 놈을 자동으로 타겟으로 설정
                g_HeroInput.SetCurrentTarget(recv_packet->dwAttackerKey);
            }
        }

        // 무조건 큐큐큐!
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;
        action.ATTACK.bAttackMove = TRUE;				
        action.ATTACK.byAttackType = recv_packet->byAttackType;					 
        action.ATTACK.dwTargetID = dwTargetKey;
        action.ATTACK.vCurPos.wvPos = recv_packet->wvCurPos;					
        action.ATTACK.vCurPos.wTile = -1;

#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        action.ATTACK.style_presentation = recv_packet->style_presentation;
#endif //_NA003109_STYLE_PRESENTATION_IMPROVEMENT

        action.ATTACK.vDestPos.wvPos = recv_packet->wvCurPos;
        action.ATTACK.vDestPos.wTile = -1;
        action.ATTACK.dwAttackSerial = recv_packet->dwClientSerial;
        action.ATTACK.dwAttackStyle = recv_packet->StyleCode;

        if (GENERALPARAM->IsServerDebug()) 
        {
            pPlayer->DoAction(&action);
            pPlayer->CommitChangeState(g_CurTime);
        }
        else
        {
            pPlayer->PutAction(&action);
        }								
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD(nsSync::MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD* recv_packet)
{
    Object* pObject = g_ObjectManager.GetObject(recv_packet->dwAttackerKey);
    if( pObject == NULL )
	{
        return;
    }

    Player* pPlayer = static_cast<Player*>(pObject);
    assert(recv_packet->byNumberOfTargets < MAX_TARGET_NUM);

    cAttackResultData TargetInfo[MAX_TARGET_NUM];

    for (DWORD i = 0; i < recv_packet->byNumberOfTargets; i++) 
    {
        ATTACK_RESULT attackresult;
        ZeroMemory(&attackresult,sizeof(attackresult));
        attackresult.SetAttackSerial( recv_packet->dwClientSerial );
        attackresult.SetTimeSerial( GetBattleTimeSerial() );
        attackresult.SetGroupID(1);
        attackresult.SetWeaponSound( GameFunc::GetWeaponSoundKind((eWEAPONTYPE)pPlayer->GetWeaponKind()) );

        attackresult.GetTargetData().SetTargetObjectKey( recv_packet->AttackInfo[i].dwTargetKey );
        attackresult.GetTargetData().SetDamage( recv_packet->AttackInfo[i].wDamage );
        attackresult.GetTargetData().SetTargetHP( recv_packet->AttackInfo[i].dwTargetHP );
		attackresult.GetTargetData().SetPosition( recv_packet->AttackInfo[i].wvCurPos, recv_packet->AttackInfo[i].wvDestPos );
        attackresult.GetTargetData().SetNoDamage( FALSE );
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        attackresult.GetTargetData().SetTargetSD(recv_packet->AttackInfo[i].target_sd);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        // 날려보내기 피니시
        BOOL bFlyFinish = FALSE;

        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_CRITICAL)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL );

            if (attackresult.GetTargetData().GetTargetHP() == 0 )
            {
                if (pPlayer->IsFlyFinishWeapon())
                {
                    bFlyFinish = TRUE;
                }
            }
        }

        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_DOUBLEDAMAGE)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOUBLE );
        }


        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_ETHERDEVICE_METAL)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL );
        }

        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_ETHERDEVICE_ETHER)
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER );
        }

        //safe_code:5콤보가 맥스이다.
        if (recv_packet->AttackInfo[i].byEtherComboCount > 5) 
        {
            recv_packet->AttackInfo[i].byEtherComboCount = 5;
        }

        attackresult.GetTargetData().SetComboCount( recv_packet->AttackInfo[i].byEtherComboCount );


        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_KNOCKBACK) 
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK );

            bFlyFinish = FALSE;
        }

        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_KNOCKDOWN) 
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOWN );

            bFlyFinish = FALSE;
        }

        if (recv_packet->AttackInfo[i].byEffect & SKILL_EFFECT_STUN) 
        {
            attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN );

            if( Character* pTarget = (Character*)g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey() ) )
            {
                pTarget->OnParalyzed();
                pTarget->SetPosition(attackresult.GetTargetData().GetCurrentPosition().wvPos);

				// 스턴상태를 수동으로 추가
                if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( eCHAR_STATE_STUN, 0 ) )
                {
                    pEffect->dwAbilityID = eABILITY_ABNORMAL_STATUS;
                    for (int a = 0; a < 4; a++)
                    {
                        pEffect->AbilityInfo.m_iParam[0] = 0;	
                        pEffect->AbilityInfo.m_iParam[1] = 0;	
                        pEffect->AbilityInfo.m_iParam[2] = 0;	
                        pEffect->AbilityInfo.m_iParam[3] = 0;						
                    }
                    pTarget->AddSkillEffect(pEffect);
                }
            }

            bFlyFinish = FALSE;
        }

        if (attackresult.GetTargetData().GetDamage() == 0 )
        {
            attackresult.GetTargetData().SetMiss( TRUE );
        }

		// 설치형 몬스터면 날리지 말아라
        if( Character* pTarget =(Character*)g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey() ) )
		{
			if( pTarget->IsKindOfObject(MONSTER_OBJECT) )
			{
				Monster* pMonster = (Monster*)pTarget;
				if (MONSTER_TYPE_SETTING_MAPOBJECT == pMonster->GetMonsterType())
				{
					bFlyFinish = FALSE;
				}
			}
        }

        // 날려 버리기 판정이 떳으면
        if (bFlyFinish)
        {					
            if (!(rand() % MONSTER_FINISH_FLY_CHANCE))
            {
                if( Object* pTargetObject = g_ObjectManager.GetObject(attackresult.GetTargetData().GetTargetObjectKey()) )
                {
					WzVector vSrc = pTargetObject->GetPosition();
                    WzVector vDest = pPlayer->GetPosition();

                    WzVector vDirection = vDest - vSrc;
                    vDirection.z = 0.0f;
                    float fAngleZ = 0.0f;
                    if (0.0f != VectorLength2(&vDirection))
                    {
                        fAngleZ = Math_GetAngleFromVector(&vDirection);
                    }

                    ClampAngle(&fAngleZ);
                    vDirection.x = sin(fAngleZ);
                    vDirection.y = -cos(fAngleZ);
                    vDirection.z = 0.0f;

                    vDest = pTargetObject->GetPosition()+vDirection* -5.0f;

					attackresult.GetTargetData().SetPosition( vSrc, vDest );
					attackresult.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK );
                }
            }
        }

        TargetInfo[i] = attackresult.GetTargetData();

         if (pPlayer->GetShotCount() == 1) 
         {
             pPlayer->AddAttackResult(&attackresult);
         }
         else if (pPlayer->GetShotCount() > 1) 
         {
             //pPlayer->AddAttackResult(&attackresult);
             AddSplitDamageResult(pPlayer, &attackresult, pPlayer->GetShotCount());
         }
    }

    //공격을 받는 대상의 상태를 변경 시킨다.공중 부양,밀리기등
    if (pPlayer->GetShotCount() == 1)
    {
        for( int i=0;i<recv_packet->byNumberOfTargets;++i)
        {
            AddAttackResultQueueAction( TargetInfo[i], GetBattleTimeSerial(), recv_packet->dwAttackerKey );
        }
    }

    // 현재 AttackSerial보다 늦게 온 놈이면 바로 처리해버린다.
    if (recv_packet->dwClientSerial < pPlayer->GetNextAttackSerial() || GENERALPARAM->IsServerDebug())
    {
        pPlayer->ProcessAttackResult(recv_packet->dwClientSerial,TRUE);
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD(nsSync::MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD* recv_packet, BYTE* addition_data)
{
    Character *pChr = (Character *)g_ObjectManager.GetObject( recv_packet->result.m_dwAttackerKey );
    if (pChr == NULL)
    {
        return;
    }

    SkillFacade::InstantResultData packet_data;
    packet_data.ParserInstantResult( recv_packet, addition_data );

    // HP MP 대입
    pChr->SetLatestAttack(GetBattleTimeSerial());
    pChr->SetHP( packet_data.GetAttackerHP() );
    pChr->SetMP( packet_data.GetAttackerMP() );

    // 스킬에 걸려든 놈들 처리 
    // Todo:스킬 상태에 projectileId세팅 할수 있는 방법을 찾아야 한다.

    ////Todo:몬스터는 어택시리얼이 없음으로 덮어 쓴다.
    //if (pChr->GetSCObject().GetObjectType() == MONSTER_OBJECT)
    //{
    //    packet_data.SetClientSerial( pChr->GetNextAttackSerial() );
    //}

    AddSkillResult( pChr, &packet_data );
    AddSkillFieldEffect( pChr, &packet_data );



    SkillScriptInfo* pInfo = SkillInfoParser::Instance()->GetSkillInfo(recv_packet->result.m_SkillCode);
    assert(pInfo);

    if (!pInfo)
    {
        //GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, _T("Invalid %d Skill Code!!!"), recv_packet->result.m_SkillCode);
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, _T("Invalid %d Skill Code!!!"), recv_packet->result.m_SkillCode);
        return;
    }

    // 바닥에 생기는 이펙트들에대해서 처리


    // 사용 캐릭터에 해당 동작(skillState)을 적용

    // 다른 캐릭터만 모션 바꾼다.
    if (g_pHero)
    {
        if (recv_packet->result.m_dwAttackerKey != g_pHero->GetObjectKey())
        {
            bool play_action = true;
            // NonStop 계열이 아닌 스킬일때만 액션을 넣어준다!
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            // 다른플레이어가 사용한 토글 스킬인 경우에는 액션처리를 하지않는다
            if( pInfo->IsToggleEffectSkill() == true )
            {
                play_action = false;
            }

            if( pInfo->IsToggleSkill() == true )
            {
                play_action = false;
            }

            if ( pInfo->IsAuraSkill() == true )
            {
                play_action = false;
            }
#else
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            // 다른플레이어가 사용한 오오라 스킬인 경우에는 액션처리를 하지않는다
            if( pInfo->IsAuraEffectSkill() == true )
            {
                play_action = false;
            }

            if( pInfo->IsAuraSkill() == true )
            {
                play_action = false;
            }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


            if( pInfo->IsNonStopSkill() == true )
            {
                play_action = false;
            }

            if( pChr->IsDead() == TRUE )
            {
                play_action = false;
            }

            if( play_action == true )
            {
                PLAYER_ACTION action;
                ZeroMemory(&action,sizeof(action));
                action.ActionID = ACTION_SKILL;				
                action.SKILL.dwTargetID = recv_packet->result.m_dwPrimaryTargetKey;
                action.SKILL.dwAttackSerial = recv_packet->result.m_dwClientSerial;
                action.SKILL.dwSkillID = recv_packet->result.m_SkillCode;
                action.SKILL.vCurPos.wvPos =recv_packet->result.m_wvCurPos;					
                action.SKILL.vCurPos.wTile = -1;
                action.SKILL.vDestPos.wvPos =recv_packet->result.m_wvDestPos;
                action.SKILL.vDestPos.wTile = -1;
                action.SKILL.vTargePos = recv_packet->result.m_wvMainTargetPos;
                QueueSkillAction(&action,pChr);
            }

            // 소환수가 스킬썼을때
            if (g_pHero->GetSummonId()==recv_packet->result.m_dwAttackerKey)
            {
                xGetCooltimeManager()->StartSkillCooltime( recv_packet->result.m_SkillCode, g_pHero->GetBonusSkillCooltime(), g_pHero->GetRatioBonusSkillCooltime() );
            }
        }
        else
        {
            xGetCooltimeManager()->StartSkillCooltime( recv_packet->result.m_SkillCode, g_pHero->GetBonusSkillCooltime(), g_pHero->GetRatioBonusSkillCooltime() );
        }
    }

    //if (pChr->IsKindOfObject(MONSTER_OBJECT))
    //{
    //    pChr->AddNextAttackSerial();
    //}


    if (pChr->IsDead())
    {
        pChr->ProcessAttackResult(recv_packet->result.m_dwClientSerial,TRUE);//true일때 시리얼에 맞는 결과를 전부 제거 
    }

}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_ACTION_DELAY_START_BRD(nsSync::MSGSUB_SKILL_ACTION_DELAY_START_BRD* recv_packet)
{
	SkillActionDelayStart* pBase = &(recv_packet->result);

    Character* pCharacter = (Character*)g_ObjectManager.GetObject(pBase->m_dwAttackerKey);
    if( pCharacter == NULL )
    {
        return;
    }

	SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pBase->m_SkillCode);

	// 이펙트 처리
	SkillEffectFunc::CreateSkillEventEffectByFieldSkill( pSkillInfo->m_SkillClassCode, pBase->m_wvMainTargetPos );


	// 캐릭터가 직접 사용한 스킬이 아닌경우
	if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_DARKOFLIGHT_ZONE ||
		pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_GRAVITY_ZONE )
	{
		// 이펙트만 표시하고 패스
		return;
	}

	// 이하는 캐릭터가 직접 사용한 스킬

	// 사용자가 사망하면 패스
    if (pCharacter->IsDead())
    {
        return;
    }

    // Todo:몬스터는 어택시리얼이 없음으로 덮어 쓴다.
    if (pCharacter->IsKindOfObject(MONSTER_OBJECT))
    {
        Monster *pMonster =(Monster *)pCharacter;

        pBase->m_dwClientSerial=pCharacter->GetNextAttackSerial();
        pMonster->AddDelaySkillSerial(pBase->m_dwClientSerial);
    }

    if (!g_pHero)
    {
        return;
    }

    // 다른 캐릭터가 스킬은 사용한경우 액션으로 처리해준다
	// 자신은 직접 스킬을 사용하며 모션과 상태를 바꾸지만, 다른 캐릭터는 패킷을 받아야 스킬을 사용했다는걸 알 수 있으므로
    if (pBase->m_dwAttackerKey != g_pHero->GetObjectKey())
    {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // 다른플레이어가 사용한 토글 스킬인 경우에는 액션처리를 하지않는다
        if( pSkillInfo->IsToggleSkill() == true )
        {
            return;
        }

        if ( pSkillInfo->IsAuraSkill() == true )
        {
            return;
        }
#else
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        // 다른플레이어가 사용한 오오라 스킬인 경우에는 액션처리를 하지않는다
        if( pSkillInfo->IsAuraSkill() == true )
        {
            return;
        }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_SKILL;				
        action.SKILL.dwTargetID = pBase->m_dwPrimaryTargetKey;
        action.SKILL.dwAttackSerial = pBase->m_dwClientSerial;
        action.SKILL.dwSkillID = pBase->m_SkillCode;

        action.SKILL.vCurPos.wvPos =recv_packet->result.m_wvCurPos;					
        action.SKILL.vCurPos.wTile = -1;
        action.SKILL.vDestPos.wvPos =recv_packet->result.m_wvDestPos;
        action.SKILL.vDestPos.wTile = -1;
        action.SKILL.vTargePos=recv_packet->result.m_wvMainTargetPos;//패킷읽어서 세팅

        QueueSkillAction(&action,pCharacter);

		// 만약 그 캐릭터가 자신의 소환수라면 관련 스킬의 쿨타임을 리셋한다
        if (g_pHero->GetSummonId() == pBase->m_dwAttackerKey)
        {
            xGetCooltimeManager()->StartSkillCooltime( pBase->m_SkillCode, g_pHero->GetBonusSkillCooltime(), g_pHero->GetRatioBonusSkillCooltime() );
        }

    }			
    else
    { 
        xGetCooltimeManager()->StartSkillCooltime( pBase->m_SkillCode, g_pHero->GetBonusSkillCooltime(), g_pHero->GetRatioBonusSkillCooltime() );
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD(nsSync::MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD* recv_packet, BYTE* addition_packet)
{
    Character *pChr = (Character *)g_ObjectManager.GetObject(recv_packet->result.m_dwAttackerKey);
    if (!pChr)
    {
        return;
    }

    SkillFacade::DelayResultData packet_data;
    packet_data.ParserDelayResult( recv_packet, addition_packet );


    pChr->SetLatestAttack(GetBattleTimeSerial());
    pChr->SetHP(packet_data.GetAttackerHP() );
    pChr->SetMP(packet_data.GetAttackerMP() );


    //// TODO : 몬스터는 어택시리얼이 없음으로 덮어 쓴다.
    //if (pChr->GetSCObject().GetObjectType() == MONSTER_OBJECT)
    //{
    //    Monster * pMonster = (Monster *)pChr;
    //    packet_data.SetClientSerial( pMonster->GetDelaySkillSerial() );
    //}

    AddSkillResult( pChr, &packet_data );
    AddSkillFieldEffect( pChr, &packet_data );


    //if (pChr->IsKindOfObject(MONSTER_OBJECT))
    //{
    //    pChr->AddNextAttackSerial();
    //}

    if (g_pHero && pChr->GetObjectKey()==g_pHero->GetObjectKey())
    {
        //사용결과처리

    }

    if (pChr->IsDead())
    {
        pChr->ProcessAttackResult(packet_data.GetClientSerial(), TRUE);//true일때 시리얼에 맞는 결과를 전부 제거 
    }

}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_CASTING_CANCEL_BRD(const nsSync::MSGSUB_SKILL_CASTING_CANCEL_BRD* recv_packet)
{
    Player *pPlayer = (Player *)g_ObjectManager.GetObject(recv_packet->m_dwObjectKey);

    if (pPlayer && pPlayer->GetSCObject().GetObjectType() ==PLAYER_OBJECT)
    {
        if (g_pHero&&pPlayer->GetObjectKey()==g_pHero->GetObjectKey())
        {
            if( BASE_SKILLINFO* pInfo = SkillInfoParser::Instance()->GetSkillInfo(recv_packet->m_SkillCode) )
            {
                xGetCooltimeManager()->ResetSkillCooltime( recv_packet->m_SkillCode );
            }
        }

        if (pPlayer->GetCurState()==STATE::SKILL)
        {
            //다음 동작이 없을때만 
            if (pPlayer->GetNextState() == STATE::NONE)
                pPlayer->SetNextState(STATE::IDLE,g_CurTime);

            pPlayer->CommitChangeState(g_CurTime);
        }
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_PERIODIC_DAMAGE_BRD(const nsSync::MSGSUB_SKILL_PERIODIC_DAMAGE_BRD* recv_packet)
{
    Character *pRealAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);

    for (int a= 0; a < recv_packet->m_byNumberOfTargets; a++)
    {

        Character *pTarget = (Character *)g_ObjectManager.GetObject(recv_packet->m_DamageInfo[a].m_dwTargetKey);

        if (pRealAttacker) 
        {
            pRealAttacker->DoPeriodicDamage(
                recv_packet->m_DamageInfo[a].m_wDamage,
                recv_packet->m_DamageInfo[a].m_dwTargetKey,
                recv_packet->m_DamageInfo[a].m_dwTargetHP,
                GetBattleTimeSerial(),
                recv_packet->m_SkillCode);
        }
        else if (pTarget)
        {
            // 공격자가 없어도 타겟이 있으면 데미지라도 처리하여 정상적으로 적용되도록한다.
            pTarget->DoPeriodicDamage(
                recv_packet->m_DamageInfo[a].m_wDamage,
                recv_packet->m_DamageInfo[a].m_dwTargetKey,
                recv_packet->m_DamageInfo[a].m_dwTargetHP,
                GetBattleTimeSerial(),
                recv_packet->m_SkillCode);
        }
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_REFLECT_DAMAGE_BRD(const nsSync::MSGSUB_SKILL_REFLECT_DAMAGE_BRD* recv_packet)
{
    Character *pRealAttacker = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);
    Character *pTarget = (Character *)g_ObjectManager.GetObject(recv_packet->m_dwTargetKey);
    if (pTarget)
    {
        pTarget->SetReserveHP(recv_packet->m_dwTargetResrveHP);

        if (recv_packet->m_wDamage) 
        {
            if ((pTarget != NULL) && (recv_packet->m_dwTargetHP > 0))
            {
                //! 2011.3.29 / i4u4me / 반사계열 데미지를 list 에 담아 놓고
                //! ProcessAttackResult() 처리시 같이 처리한다.
                //! 주기전 데미지에 의한 반사 데미지는 ProcessAttackResult() 함수가 호출되지 않을수도 있을것 같고 다음 결과 처리시 같이 처리됨
                pTarget->AddReflectDamage(recv_packet->m_wDamage,
                                            recv_packet->m_dwTargetKey,
                                            recv_packet->m_dwTargetHP,
                                            GetBattleTimeSerial(),
                                            recv_packet->m_SkillCode);
            }
            else
            {
                if (pRealAttacker) 
                {
                    pRealAttacker->DoPeriodicDamage(recv_packet->m_wDamage,recv_packet->m_dwTargetKey,recv_packet->m_dwTargetHP,GetBattleTimeSerial(),recv_packet->m_SkillCode);
                }
                // 공격자가 없어도 타겟이 있으면 데미지라도 처리하여 정상적으로 적용되도록한다.
                else if (pTarget)
                {
                    pTarget->DoPeriodicDamage(recv_packet->m_wDamage,recv_packet->m_dwTargetKey,recv_packet->m_dwTargetHP,GetBattleTimeSerial(),recv_packet->m_SkillCode);
                }
            }
        }

        AddReflectDamageResult((char *)recv_packet,pTarget);
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_EFFECT_BOMB_BRD(const nsSync::MSGSUB_SKILL_EFFECT_BOMB_BRD* recv_packet)
{
    //assert(false && "BattleScene::OnRecv_MSGSUB_SKILL_EFFECT_BOMB_BRD(const nsSync::MSGSUB_SKILL_EFFECT_BOMB_BRD* recv_packet)");
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_SELF_DESTRUCTION_BRD(const nsSync::MSGSUB_SKILL_SELF_DESTRUCTION_BRD* recv_packet)
{
    Character* pRealAttacker = (Character*)g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);

    for (int i = 0; i < recv_packet->m_byNumberOfTargets; i++)
    {
        Character* pTarget = (Character*)g_ObjectManager.GetObject(recv_packet->m_DamageInfo[i].m_dwTargetKey);

        if (pRealAttacker) 
        {
            pRealAttacker->DoPeriodicDamage(
                recv_packet->m_DamageInfo[i].m_wDamage,
                recv_packet->m_DamageInfo[i].m_dwTargetKey,
                recv_packet->m_DamageInfo[i].m_dwTargetHP,
                GetBattleTimeSerial(),
                recv_packet->m_SkillCode);
        }
        else if (pTarget)
        {
            // 공격자가 없어도 타겟이 있으면 데미지라도 처리하여 정상적으로 적용되도록한다.
            pTarget->DoPeriodicDamage(
                recv_packet->m_DamageInfo[i].m_wDamage,
                recv_packet->m_DamageInfo[i].m_dwTargetKey,
                recv_packet->m_DamageInfo[i].m_dwTargetHP,
                GetBattleTimeSerial(),
                recv_packet->m_SkillCode);
        }
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_TRANSPARENT_BRD(const nsSync::MSGSUB_SKILL_TRANSPARENT_BRD* recv_packet)
{
    Player* pPlayer = (Player*)g_ObjectManager.GetObject(recv_packet->m_dwObjectKey); 
    if (pPlayer)
    {
        if (!pPlayer->IsKindOfObject(PLAYER_OBJECT))
        {
            return; 
        }

#ifdef	_DEV_VER
  #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        if (GlobalFunc::IsBattleGroundScene() == FALSE
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            && GlobalFunc::IsGoldRushScene() == FALSE
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            )
  #endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        {
            //safecode: 쉐도우가 아닌캐릭에게 하이드가 왔다
            if (pPlayer->GetClass() != eCHAR_SHADOW)
            {
                assert(!"비정상 하이드정보:CG_SKILL_TRANSPARENT_BRD");
                WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("비정상 하이드정보:CG_SKILL_TRANSPARENT_BRD [pPlayer->GetClass() != eCHAR_SHADOW]"));
            }
        }

#endif//_DEV_VER

        pPlayer->SetHideReserve(TRUE);
    }

}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_SUCTION_BRD(const nsSync::MSGSUB_SKILL_SUCTION_BRD* recv_packet)
{
    Object* pObj = g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);
    if (!pObj)
    {
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER , _T("CG_SKILL_SUCTION_BRD::if (pObj == NULL))"));
        return;
    }

    if (FALSE == pObj->IsKindOfObject(PLAYER_OBJECT))
    {
        assert(!"CG_SKILL_SUCTION_BRD::if (FALSE == pObj->IsKindOfObject(PLAYER_OBJECT))");
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER ,_T("CG_SKILL_SUCTION_BRD::if (FALSE == pObj->IsKindOfObject(PLAYER_OBJECT))"));
        return;
    }

    Player* pPlayer = (Player*)pObj;
    if (pPlayer)
    {
        pPlayer->SetHP(recv_packet->m_dwAttackerHP);
        pPlayer->SetMP(recv_packet->m_dwAttackerMP);
        Character* pTarget = (Character*)pPlayer->GetTargetObject();
        if (pTarget)
        {
            pTarget->SetHP(recv_packet->m_dwTargetHP);
            pTarget->SetMP(recv_packet->m_dwTargetMP);
        }
    }
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_SKILL_VITAL_SUCTION_BRD(const nsSync::MSGSUB_SKILL_VITAL_SUCTION_BRD* recv_packet)
{
    Player* pPlayer = (Player*)g_ObjectManager.GetObject(recv_packet->m_dwAttackerKey);
    if (pPlayer)
        pPlayer->SetHP(recv_packet->m_dwHP);
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_COLLECT_ACTION_BRD(const nsSync::MSGSUB_COLLECT_ACTION_BRD* recv_packet)
{
    Player* player = g_ObjectManager.GetPlayer(recv_packet->collector);
    if (player == NULL)
    {
        assert(!"MSGSUB_COLLECT_ACTION_BRD:");
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER ,_T("Player is Null :: MSGSUB_COLLECT_ACTION_BRD"));
        return;
    }

    PLAYER_ACTION action;
    ZeroMemory(&action, sizeof(action));
    action.ActionID = ACTION_COLLECTION_OBJECT;
    action.COLLECTOBJECT.dwTargetID = ((C_COLLECTKEY&)(recv_packet->object_key)).Get();

    Object* map_object = g_ObjectManager.GetObject(((C_COLLECTKEY&)(recv_packet->object_key)).Get());
    if (map_object != NULL)
    {
        player->SetDirection(map_object->GetVisiblePos());
    }

    player->SetCollectObjKey(((C_COLLECTKEY&)(recv_packet->object_key)).Get());
    player->SetCollectID(recv_packet->collection_id);
    player->SetCollectStartTime(0);
    player->DoAction(&action);

    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_ACTION_BRD: %s"), player->GetName());
}
//------------------------------------------------------------------------------ 
void	BattleScene::OnRecv_MSGSUB_COLLECT_CANCEL_BRD(const nsSync::MSGSUB_COLLECT_CANCEL_BRD* recv_packet)
{
    Player* player = g_ObjectManager.GetPlayer(recv_packet->collector);
    if (player == NULL)
    {
        assert(!"MSGSUB_COLLECT_CANCEL_BRD");
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER , _T("MSGSUB_COLLECT_CANCEL_BRD"));
        return;
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (GlobalFunc::IsBattleGroundScene() == TRUE)
    {
        // 거점 점령중 이펙트 제거
        // 아래코드에서 채집오브젝트키를 없에기 때문에 없에기 전에 호출해주자
        MapObject* map_object = (MapObject*)g_ObjectManager.GetObject(((C_COLLECTKEY&)recv_packet->object_key).Get());
        if (map_object)
        {
            ChaosSystem::Function::CreateBattleGroundConqeredEffect(player, map_object);
        }
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    player->SetCollectObjKey(0);
    player->SetCollectID(0);
    player->SetCollectStartTime(0);

    if (g_pEffectManager->GetEffectPtrHandle(player->GetCollectStartEftHndl()) != NULL)
    {
        g_pEffectManager->RemoveEffectPtr(player->GetCollectStartEftHndl());
        player->SetCollectStartEftHndl(INVALID_HANDLE_VALUE);
    }

    player->StopMove();
    player->SetNextState(STATE::IDLE, g_CurTime);

    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Recv Packet : MSGSUB_COLLECT_CANCEL_BRD"));
}
