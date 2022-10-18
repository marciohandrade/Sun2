#include "stdafx.h"
#include ".\Handler_CG_TRIGGER.h"
#include "GameZone.h"
#include "GameField.h"
#include "Player.h"
#include "MissionManager.h"
#include "MissionRewardParser.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include <SCItemSlotContainer.h>
#include "MissionRewardManager.h"
#include "TriggerManagerEx.h"
#include "..\\..\\SolarTrigger\\TriggerMsg.h"
#include <FatigueOptionParser.h>


Handler_CG_TRIGGER::Handler_CG_TRIGGER()
{

}
Handler_CG_TRIGGER::~Handler_CG_TRIGGER()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_TRIGGER::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_TRIGGER::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_TRIGGER, CG_TRIGGER_CLICK_OBJECT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRIGGER, CG_TRIGGER_ENTER_AREA_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRIGGER, CG_TRIGGER_LEAVE_AREA_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRIGGER, CG_TRIGGER_REWARD_SYN, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


VOID Handler_CG_TRIGGER::OnCG_TRIGGER_CLICK_OBJECT_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRIGGER_CLICK_OBJECT_SYN * pRecvMsg = (MSG_CG_TRIGGER_CLICK_OBJECT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

    GameField* pField = pPlayer->GetField();

	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_TRIGGER::OnCG_TRIGGER_CLICK_OBJECT_SYN] Not Exist GameField" );
		return;
	}

	BOOL bIsValidSize = (BOOL) ( sizeof(MSG_CG_TRIGGER_CLICK_OBJECT_SYN) == wSize );
	if( !bIsValidSize )
	{
		SUNLOG( eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_TRIGGER_CLICK_OBJECT_SYN") );
		return;
	}

	//(lst1024)(0325)클라에서 받는 맵 오브젝트 키 값은 하위(1~) 값이다.	__NA001283_20090303_COLLECTION_MAPOBJECTID
	DWORD dwClickedObjectKey = pRecvMsg->m_dwClickedObjectKey;

	if( !pField->IsReady() ) 
        return;

	TRIGGER_CLICK_OBJECT msg;
	msg.m_eObjectType = (eOBJECT_TYPE)0;
	msg.m_dwObjectKey = dwClickedObjectKey;
	msg.m_PlayerKey   = pPlayer->GetObjectKey();
	pField->GetTriggerManager()->OnMsg(&msg);
	
}

VOID Handler_CG_TRIGGER::OnCG_TRIGGER_ENTER_AREA_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_TRIGGER_ENTER_AREA_SYN* recv_msg = (MSG_CG_TRIGGER_ENTER_AREA_SYN *)pMsg;

    if (sizeof(*recv_msg) != wSize) {
        return;
    };
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    GameField* const game_field = player ? player->GetField() : NULL;
    if (game_field == NULL || game_field->IsReady() == false) {
        return;
    };
    // NOTE: f110928.3L, prevent the client enter area request hacking loophole.
    TriggerManagerEx* const trigger_manager = game_field->GetTriggerManager();
    const int area_id = recv_msg->m_iAreaID;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if (trigger_manager->CheckAreaIdBasedOnPos(area_id, player->GetPosPtr(), NULL) == false) {
        return;
    };
#endif
    //
    ns_trigger::TriggerPlayerLinkInGameSlots* const trigger_links = player->GetTriggerLinks();
    //
    TRIGGER_ENTER_AREA trigger_msg;
    trigger_msg.m_PlayerKey = player->GetObjectKey();
    trigger_msg.m_AreaID = area_id;
    trigger_msg.m_pLink = trigger_links->AllocEmptySlot();

    if (trigger_manager->OnEnterArea(&trigger_msg) == false)
    {
        const int standing_tile_no = player->GetPathExplorer()->GetTile();
        const WzVector* cur_pos = player->GetPosPtr();
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=enter area operation failed.|")
            _T("AreaID=%d, CurrentTile=%d, Pos=[%.2f, %.2f, %.2f]|"),
            area_id, standing_tile_no, cur_pos->x, cur_pos->y, cur_pos->z);
        // CHANGES: f110311.4L, fixed a memory leak occurrance
        // when the request identification of area can't find in a field.
        trigger_links->FreeSlot(\
            static_cast<ns_trigger::TriggerPlayerLinkInGame*>(trigger_msg.m_pLink));
    };
}

VOID Handler_CG_TRIGGER::OnCG_TRIGGER_LEAVE_AREA_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRIGGER_LEAVE_AREA_SYN * pRecvMsg = (MSG_CG_TRIGGER_LEAVE_AREA_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    GameField* pField = pPlayer ? pPlayer->GetField() : NULL;
    if (pPlayer == NULL || pField == NULL) {
        return;
    };

	TRIGGER_LEAVE_AREA msg;
	msg.m_PlayerKey = pPlayer->GetObjectKey();
	msg.m_AreaID = pRecvMsg->m_iAreaID;
    // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
    ns_trigger::TriggerPlayerLinkInGameSlots* const trigger_links = pPlayer->GetTriggerLinks();
    msg.m_pLink = trigger_links->GetAreaLink(msg.m_AreaID);
    if (msg.m_pLink == NULL) {
        return;
    }

	if( !pField->IsReady() ) 
        return;

	pField->GetTriggerManager()->OnLeaveArea( &msg );
    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__ 0904
    trigger_links->FreeSlot(static_cast<ns_trigger::TriggerPlayerLinkInGame*>(msg.m_pLink));
}


Handler_CG_TRIGGER_IMPL( CG_TRIGGER_REWARD_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRIGGER_REWARD_SYN* pRecvMsg = (MSG_CG_TRIGGER_REWARD_SYN *)pMsg;

	MissionRewardManager* pManager = MissionRewardManager::Instance();
	pManager->Init();

	MissionRewardArgs& rMRA = pManager->GetMissionRewardArgs();
	MissionRewardInfo* pMRI = NULL;
	// MissionRewardArgs 정보 설정 <START>
	do {
		rMRA.SetPlayer( PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey ) );
		ASSERT( rMRA.GetPlayer() );
		if( !rMRA.GetPlayer() ) return;

		GameField *pField = rMRA.GetPlayer()->GetField();
		if( !pField || !pField->IsReady() ) return;

		rMRA.SetMissionManager(rMRA.GetPlayer()->GetMissionManager());
		rMRA.SetNum(rMRA.GetMissionManager()->ReservedMissionNum());

		if( rMRA.GetMissionManager()->GetMissionState(rMRA.GetNum()) != MISSION_REWARD )
		{
			MSG_CG_TRIGGER_REWARD_NAK nmsg;
			nmsg.m_dwErrorCode = 4116;
			rMRA.GetPlayer()->SendPacket( &nmsg, sizeof(nmsg) );

			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
                _T("[ID:%s][%08u][MissionCode:%d] 미션보상 받을 수 있는 상태가 아닌데 미션보상을 시도했다!!"),
				rMRA.GetPlayer()->GetUserID(), rMRA.GetPlayer()->GetUserKey(), rMRA.GetNum() );

			return;
		}

		if( rMRA.GetMissionManager()->GetMissionPoint(rMRA.GetNum()) == 0 )
		{
			MSG_CG_TRIGGER_REWARD_NAK nmsg;
			nmsg.m_dwErrorCode = 4120;	// 임시코드
			rMRA.GetPlayer()->SendPacket( &nmsg, sizeof(nmsg) );

			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
				_T("[ID:%s][%08u] 미션보상 받을 상태인데 클리어 회수가 0이다!!"),
				rMRA.GetPlayer()->GetUserID(), rMRA.GetPlayer()->GetUserKey() );

			return;
		}
		// 아래의 경우가 발생한다. Trigger의 미션보상 명령 수행중, 미션포인트의 증가없이 서버 종료되었을 때...
		//ASSERT( rMRA.GetMissionManager()->GetMissionPoint(pRecvMsg->m_Num) != 0 );

		pMRI = MissionRewardParser::Instance()->GetMissionReward( rMRA.GetNum() );
		if( !pMRI )
		{
			MSG_CG_TRIGGER_REWARD_NAK nmsg;
			nmsg.m_dwErrorCode = 4117;
			rMRA.GetPlayer()->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}

		//rMRA.SetType( pRecvMsg->m_Type );
		rMRA.SetSelectedItem( pRecvMsg->m_SelectedItem );

	} while(0);
	// MissionRewardArgs 정보 설정 <END>

	REWARD_MSG_CREATOR creator;
	MSG_CG_TRIGGER_REWARD_ACK* pMsgACK = creator.GetRewardMSG_HeaderPtr(MSG_CG_TRIGGER_REWARD_ACK)();
	pManager->GetMissionRewardInfoNClassCode();

	if( RC::RC_REWARD_SUCCESS != pManager->RewardItem( rMRA.GetMissionRewardInfoClassCode(), pMsgACK->m_ItemInfo ) )
	{
		// 아래 코드를 넣어야 하는가?
		//rMRA.GetMissionManager()->SetMissionState(rMRA.GetNum(), MISSION_NONE);
		MSG_CG_TRIGGER_REWARD_NAK nmsg;
		if( rMRA.GetMissionManager()->GetMissionPoint(rMRA.GetNum()) != 0 )
			nmsg.m_dwErrorCode = 4118;
		else
			nmsg.m_dwErrorCode = 4119;
		rMRA.GetPlayer()->SendPacket( &nmsg, sizeof(nmsg) );
		if( nmsg.m_dwErrorCode != 4118 )
		{
			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
				_T("[ID:%s][%08u] 미션보상 받을 상태인데 보상할 수 없는 상태다."),
				rMRA.GetPlayer()->GetUserID(), rMRA.GetPlayer()->GetUserKey() );
		}		

		return;
	}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // 이미 미션 보상 매니저에서 로그를 남겼지만 미션 클리어 로직에 의해 미션 로그로 남기에 
    // 전장 보상은 한번더 남긴다. 보상아이템 코드는 이미 미션타입으로 로그를 남겼기 때문에..
    // 전장 보상 아이템 로그는 아래 남기는 로그 바로 위에 남게 된다.
    // MissionRewardInfo.txt 스크립트를 참조하여
    // 미션보상 Index가 61부터 카오스존 대역폭이다
    if (rMRA.GetNum() > 60)
    {
        Player* const plyaer = rMRA.GetPlayer();
        GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_REWARD, plyaer);
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	MONEY PreMoney = rMRA.GetPlayer()->GetMoney();
	pManager->RewardHime( rMRA.GetMissionRewardInfoClassCode(), &(pMsgACK->m_Money) );
	EXPTYPE OldExp = rMRA.GetPlayer()->GetExp();
	pManager->RewardExp( rMRA.GetMissionRewardInfoClassCode(), &(pMsgACK->m_i64Exp) );
	pMsgACK->m_i64Exp = rMRA.GetPlayer()->GetExp();

	if( pMsgACK->m_Money - PreMoney != 0 )
		GAMELOG->LogMoney( MONEY_REWARD_MISSION, rMRA.GetPlayer(), (pMsgACK->m_Money - PreMoney) );

	if( pMsgACK->m_i64Exp - OldExp != 0 )
		GAMELOG->LogExp( EXP_MISSION, rMRA.GetPlayer(), pMsgACK->m_i64Exp, OldExp );

	//피로도 기능을 꺼버리면 현재 피로도 구간이 0일수 있다. 
	_FATIGUE_TYPE* pFatigue = rMRA.GetPlayer()->GetFatigueManager()->GetCurFatigueType();
	if( pFatigue )
		pMsgACK->m_byFatigueType = pFatigue->m_byType;
	else
		pMsgACK->m_byFatigueType = 0;

#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
	// 현재 이벤트 중이면
	if( rMRA.GetPlayer()->GetMissionManager()->IsGuildEvent() )
	{
		rMRA.GetPlayer()->GetMissionManager()->SetGuildEvent( false );
		pMsgACK->m_bGuildBounsExp = true;
	}
	else
		pMsgACK->m_bGuildBounsExp = false;
#endif
    GameZone* pGameZone = rMRA.GetPlayer()->GetField()->GetGameZone();
    pMsgACK->m_bPortalMission =
        !!(pGameZone->Ruleset.rules & pGameZone->Ruleset.ZONE_RULE_OPTION_ROOM_BY_PORTAL);

	rMRA.GetPlayer()->SendPacket( pMsgACK, pMsgACK->GetSize() );
	rMRA.GetMissionManager()->SetMissionState(rMRA.GetNum(), MISSION_NONE);

	return;
}

