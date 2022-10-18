#include "StdAfx.h"
#include ".\Handler_CG_STATUS.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "SkillInfoParser.h"
#include "Player.h"
#include "GameField.h"
#include "GameServerEx.h"
#include "StatusManager.h"
#include "GameZone.h"
#include "Map.h"
#include <PacketStruct_AG.h>
#include "ItemManager.h"
#include "StateInfoParser.h"
#include "EquipmentSlotContainer.h"

Handler_CG_STATUS::Handler_CG_STATUS()
{
}

Handler_CG_STATUS::~Handler_CG_STATUS()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_STATUS::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_STATUS::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_STAT_SELECT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_RESURRECTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_EMOTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_CONDITION_CHANGE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_RENDER_OPTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_END_DRAGON_FORMATION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_REBIRTH_ON_VILLAGE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_INTERCEPTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_FREE_INIT_STAT_UNDER_20_SYN, PHASE_SERVICE),	
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_DELETE_CHAR_STATE_SYN, PHASE_SERVICE),
    #ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION   
        HANDLER_NODE_INFO(CG_STATUS, CG_STATUS_STAT_APPLY_SYN, PHASE_SERVICE),
    #endif
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION   

VOID Handler_CG_STATUS::OnCG_STATUS_STAT_APPLY_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_STATUS_STAT_APPLY_SYN * pRecvMsg = (MSG_CG_STATUS_STAT_APPLY_SYN *)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if (pPlayer == NULL) {
        return;
    }
    // CHANGES: f110305.3L, check boundary, this logic need fixed size buffer array
    if (FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize) == false) {
        return;
    }
    if( !pPlayer->GetField() )
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_STAT_APPLY_SYN] Not Exist GameField" );
        return;
    }
    ushort stat_point_array[eATTR_BASE_STAT_COUNT];
    CopyMemory(stat_point_array, pRecvMsg->stat_point, sizeof(stat_point_array));
    // CHANGES: f110305.3L
    BOOST_STATIC_ASSERT(sizeof(stat_point_array) == sizeof(pRecvMsg->stat_point) &&
                        _countof(stat_point_array) == _countof(pRecvMsg->stat_point));
    // CHANGES: f110305.3L, solve invalid logging and network buffer io problem
    RC::eSTAT_POINT_RESULT result_code = pPlayer->ApplyStat(stat_point_array, eATTR_BASE_STAT_COUNT);
    if (RC::RC_STAT_POINT_SUCCESS == result_code)
    {
        uint objectkey = pPlayer->GetObjectKey();
        MSG_CG_STATUS_STAT_APPLY_ACK ack_message;
        // CHANGES: f110305.3L
        BOOST_STATIC_ASSERT(sizeof(ack_message.stat_point) == sizeof(stat_point_array) &&
            _countof(ack_message.stat_point) == _countof(stat_point_array));
        ack_message.remain_stat_point = pPlayer->GetCharInfo()->m_iRemainStat;
        ack_message.object_key = objectkey;
        CopyMemory(ack_message.stat_point, stat_point_array, sizeof(ushort)*eATTR_BASE_STAT_COUNT);
        pPlayer->SendPacket(&ack_message, sizeof(ack_message));

        // STAT 사용 로그 남기기
        uint attribute_type = eATTR_BASE_STAT_START_INDEX;
        for ( ; attribute_type < eATTR_BASE_STAT_COUNT ; ++attribute_type)
        {
            if (stat_point_array[attribute_type] != 0)
                GAMELOG->WriteUseStat(pPlayer, (BYTE)attribute_type);
        }
    }
    else
    {
        MSG_CG_STATUS_STAT_APPLY_NAK nak_message;
        nak_message.result_code = result_code;
        pPlayer->SendPacket(&nak_message, sizeof(nak_message));
    }
}

#endif //_NA_0_20110216_STATUS_STAT_POINT_SIMULATION   

VOID Handler_CG_STATUS::OnCG_STATUS_STAT_SELECT_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_STAT_SELECT_SYN * pRecvMsg = (MSG_CG_STATUS_STAT_SELECT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_STAT_SELECT_SYN] Not Exist GameField" );
		return;
	}

	eATTR_TYPE attrType = (eATTR_TYPE)pRecvMsg->m_AttrType;

	// need StatManager? --;
	if (pPlayer->CanSelectStat(attrType))
	{
		DWORD resultValue = pPlayer->SelectStat( attrType );

		MSG_CG_STATUS_STAT_SELECT_ACK sendMsg;
		sendMsg.m_byCategory		= CG_STATUS;
		sendMsg.m_byProtocol		= CG_STATUS_STAT_SELECT_ACK;
		sendMsg.m_dwObjKey			= pPlayer->GetObjectKey();
		sendMsg.m_AttrType			= (BYTE)attrType;
		sendMsg.m_dwValue			= resultValue;

		pPlayer->SendPacket( &sendMsg, sizeof(sendMsg) );

		// STAT 사용 로그 남기기
		GAMELOG->WriteUseStat( pPlayer, (BYTE)attrType );
		// STAT 사용 로그 남기기 끝.
	}
	else
	{
		MSG_CG_STATUS_STAT_SELECT_NAK sendMsg;
		sendMsg.m_byCategory		= CG_STATUS;
		sendMsg.m_byProtocol		= CG_STATUS_STAT_SELECT_NAK;
		sendMsg.m_AttrType			= attrType;
		sendMsg.m_dwErrorCode		= 0;		// Error Code 정의 필요

		pPlayer->SendPacket( &sendMsg, sizeof(sendMsg) );
	}
}

VOID Handler_CG_STATUS::OnCG_STATUS_RESURRECTION_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_RESURRECTION_SYN * pRecvMsg = (MSG_CG_STATUS_RESURRECTION_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_RESURRECTION_SYN] Not Exist GameField" );
		return;
	}

    // added rules to support _NA_0_20100722_AUTO_SYSTEM
    // CHUNTING, Auto아이템 착용시 가능
    GameZone* pGameZone = pPlayer->GetGameZonePtr();
    if (!pGameZone)
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_RESURRECTION_SYN] Not Exist GameZone" );
        return;
    }

    eZONETYPE ZoneType = pGameZone->GetZoneType();
    if (ZoneType != eZONETYPE_CHUNTING)
    {
        CEquipmentSlotContainer* pContainer = 
            static_cast<CEquipmentSlotContainer*>(
            pPlayer->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));

        POSTYPE pos = pContainer->GetPosEquipByItemType(eITEMTYPE_AUTO_HUNTING, 
                                                        eEQUIPCONTAINER_CHARGE1, eEQUIPCONTAINER_MAX);

        if (pos == eEQUIPCONTAINER_INVALID)
        {
            MSG_CG_STATUS_RESURRECTION_NAK NakMsg;
            NakMsg.m_dwObjectKey = pPlayer->GetObjectKey();
            NakMsg.m_byErrorCode = 0;
            pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
            return;
        }

        SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(pContainer->GetSlot(pos));

        if (rItemSlot.IsProgressTime() == false)
        {
            MSG_CG_STATUS_RESURRECTION_NAK NakMsg;
            NakMsg.m_dwObjectKey = pPlayer->GetObjectKey();
            NakMsg.m_byErrorCode = 0;
            pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
            return;
        }
    }

	pPlayer->OnResurrection( 0, 1.f, 1.f );
}


VOID Handler_CG_STATUS::OnCG_STATUS_EMOTION_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_EMOTION_SYN * pRecvMsg = (MSG_CG_STATUS_EMOTION_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_EMOTION_SYN] Not Exist GameField" );
		return;
	}

	MSG_CG_STATUS_EMOTION_NAK NakMsg;

	// 해당 스킬이 존재하는지 체크
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( eEMOTION_START_CODE + pRecvMsg->m_byEmotionType );
	if( !pBaseSkillInfo )
	{
		NakMsg.m_byErrorCode = RC::RC_STATUS_SKILLINFO_IS_NULL;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// 요구레벨 체크
	if( pBaseSkillInfo->m_wRequireLv > pPlayer->GetLevel() )
	{
		NakMsg.m_byErrorCode = RC::RC_STATUS_REQUIRE_LEVEL_LIMIT;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// 변신중에는 이모션 불가
	StatusManager* const pStatusManager = pPlayer->GetStatusManager();
	if( pStatusManager->Status.IsDragonTransforming() )
	{
		NakMsg.m_byErrorCode = RC::RC_STATUS_DRAGON_TRANSFORMATION_LIMIT;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}
    if(pStatusManager->Status.IsRidingRider()) {
        NakMsg.m_byErrorCode = RC::RC_STATUS_CANT_EMOTION_ON_CURRENT_STATE;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return;
    }
	// Emotion 동작을 시작할 때 필요한 처리들을 한다.(앉기상태가 서기상태로 전환된다.)
	pPlayer->GetStatusManager()->ActEmotion();

	MSG_CG_STATUS_EMOTION_BRD BrdMsg;
	BrdMsg.m_dwObjectKey = pPlayer->GetObjectKey();
	BrdMsg.m_byEmotionType = pRecvMsg->m_byEmotionType;
	pPlayer->SendPacketAround( &BrdMsg, sizeof(BrdMsg) );
}

VOID Handler_CG_STATUS::OnCG_STATUS_CONDITION_CHANGE_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_CONDITION_CHANGE_SYN* pRecvMsg = (MSG_CG_STATUS_CONDITION_CHANGE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if (pPlayer == NULL || pPlayer->GetField() == NULL) {
        return;
    };

	// 실제로 상태를 바꿔준다.
	RC::eCONDITION_RESULT rcResult = pPlayer->GetStatusManager()->ChangeCondition( (eCHAR_CONDITION)pRecvMsg->m_byConditionType );
	if( rcResult != RC::RC_CONDITION_SUCCESS )
	{
		MSG_CG_STATUS_CONDITION_CHANGE_NAK NakMsg;
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
	}
}

VOID Handler_CG_STATUS::OnCG_STATUS_RENDER_OPTION_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_RENDER_OPTION_SYN* pRecvMsg = (MSG_CG_STATUS_RENDER_OPTION_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_RENDER_OPTION_SYN] Not Exist GameField" );
		return;
	}

	// 투구 옵션
	if(MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION == pRecvMsg->m_byRenderOptionType)
	{
		pPlayer->SetHelmetOption(pRecvMsg->m_byOptionLevel);
	}
    else if(MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_WING_OPTION == pRecvMsg->m_byRenderOptionType)
    {
        pPlayer->SetWingOption(pRecvMsg->m_byOptionLevel);
    }
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    else if(MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION == pRecvMsg->m_byRenderOptionType)
    {
        pPlayer->SetCostumeOption(pRecvMsg->m_byOptionLevel);
    }
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
	else
	{
		return;
	}

	MSG_CG_STATUS_RENDER_OPTION_BRD sendMsg;
	sendMsg.m_dwObjectKey = pPlayer->GetObjectKey();
	sendMsg.m_byRenderOptionType = pRecvMsg->m_byRenderOptionType;
	sendMsg.m_byOptionLevel = pRecvMsg->m_byOptionLevel;
	pPlayer->SendPacketAround(&sendMsg, sizeof(sendMsg));

}

VOID Handler_CG_STATUS::OnCG_STATUS_RESURRECTION_TO_SAFEZONE_SYN( ServerSession * pServerSession, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN *pRecvMsg = (MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_CG_STATUS( CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN )" );
#endif

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_RESURRECTION_TO_SAFEZONE_SYN] Not Exist GameField" );
		return;
	}

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( !pZone )	
	{
		MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK NakMsg;
		NakMsg.m_byErrorCode = RC::RC_STATUS_ZONE_IS_NULL;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	RC::eSTATUS_RESULT rcResult = pZone->ResurrectionAtSafezone( pPlayer );
	if( rcResult != RC::RC_STATUS_SUCCESS )
	{
		MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK NakMsg;
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
	}
}


VOID Handler_CG_STATUS::OnCG_STATUS_END_DRAGON_FORMATION_SYN( ServerSession * pServerSession, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_END_DRAGON_FORMATION_SYN *pRecvMsg = (MSG_CG_STATUS_END_DRAGON_FORMATION_SYN*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_END_DRAGON_FORMATION_SYN] Not Exist GameField" );
		return;
	}

	// 변신 상태를 해제한다.
	if( !pPlayer->GetStatusManager()->Remove( eCHAR_STATE_TRANSFORMATION ) )
	{
		MSG_CG_STATUS_END_DRAGON_FORMATION_NAK NakMsg;
		NakMsg.m_byErrorCode = RC::RC_STATUS_STATE_NOT_EXSITS;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
	}
}

VOID
Handler_CG_STATUS::OnCG_STATUS_REBIRTH_ON_VILLAGE_SYN(ServerSession* pServerSession,
                                                      MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_STATUS_REBIRTH_ON_VILLAGE_SYN* pRecvMsg = (MSG_CG_STATUS_REBIRTH_ON_VILLAGE_SYN*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(pPlayer == 0)
        return;

    GameField* pGameField = pPlayer->GetField();
    if(pGameField == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Not Exist GameField");
        return;
    }
    GameZone* pGameZone = pGameField->GetGameZone();
    eZONETYPE zoneType = pGameZone->GetZoneType();
    //__NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__
    BOOLEAN bAcceptableStatus = zoneType == eZONETYPE_VILLAGE ||
                                pGameZone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD;
    if(bAcceptableStatus == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Abusing Status %u", pPlayer->GetUserKey());
        return;
    }

    //부활
    RC::eSTATUS_RESULT result;
    if(pPlayer->m_bIsFallDead)
    {
        result = pPlayer->RebirthFromFallDead();
    }
    else
    {
        if(pPlayer->OnResurrection(0, 1.0, 1.0))
            result = RC::RC_STATUS_SUCCESS;
        else
            result = RC::RC_STATUS_ALIVE; //OnResurrection()메소드의 FALSE인 경우가 이거밖에 없어서 사용 함.
    }
    if(result != RC::RC_STATUS_SUCCESS)
    {
        MSG_CG_STATUS_REBIRTH_ON_VILLAGE_NAK nakmsg;
        nakmsg.m_dwErrorCode = result;
        pPlayer->SendPacket(&nakmsg, sizeof(nakmsg));
        return;
    }

    MSG_CG_STATUS_REBIRTH_ON_VILLAGE_BRD sendMsg;
    sendMsg.m_dwObjectKey   = pPlayer->GetObjectKey();
    sendMsg.m_dwHP  = pPlayer->GetHP();
    sendMsg.m_dwMP  = pPlayer->GetMP();
    sendMsg.m_wvCurPos = *pPlayer->GetPosPtr();
    pPlayer->SendPacketAround(&sendMsg, sizeof(sendMsg));
}

VOID Handler_CG_STATUS::OnCG_STATUS_INTERCEPTION_SYN( ServerSession * pServerSession, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_INTERCEPTION_SYN *pRecvMsg = (MSG_CG_STATUS_INTERCEPTION_SYN*)pMsg;
	MSG_CG_STATUS_INTERCEPTION_NAK nakMSG;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
	if(!pPlayer)	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_INTERCEPTION_SYN] Not Exist GameField" );
		return;
	}

	/*if(!pPlayer->SetInterceptionOption(pRecvMsg->m_byInterceptionCode, pRecvMsg->m_bRequestFlag))
	{
		MSG_CG_STATUS_INTERCEPTION_NAK nakMSG;
		nakMSG.m_byErrorCode = RC::RC_STATUS_INTERCEPTION_FAILED;
		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}*/

    // 월드 서버 플래그와 동기화 해야 될 경우 월드 서버에 다녀오삼!
	if(pRecvMsg->m_byInterceptionCode != eINTERCEPTION_WHISPER)
	{
		if(!pPlayer->SetInterceptionOption(pRecvMsg->m_byInterceptionCode, pRecvMsg->m_bRequestFlag))
		{			
			switch(pRecvMsg->m_byInterceptionCode)
			{
			case eINTERCEPTION_TRADE:
				nakMSG.m_byErrorCode = RC::RC_STATUS_TRADE_INTERCEPTION_FAILED;
				pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
				break;
			default:
				break;
			}
		}

		MSG_CG_STATUS_INTERCEPTION_ACK ackMSG;
		ackMSG.m_byInterceptionCode = pRecvMsg->m_byInterceptionCode;
		ackMSG.m_bRequestFlag		= pRecvMsg->m_bRequestFlag;
        pPlayer->SendPacket(&ackMSG, sizeof(ackMSG));
		return;
	}
	else if(pRecvMsg->m_byInterceptionCode == eINTERCEPTION_WHISPER)
	{
		MSG_AG_STATUS_INTERCEPTION_SYN synMSG;
		synMSG.m_dwKey				= pPlayer->GetUserGuid();
		synMSG.m_byInterceptionCode	= pRecvMsg->m_byInterceptionCode;
		synMSG.m_bRequestFlag		= pRecvMsg->m_bRequestFlag;

		g_pGameServer->SendToServer(AGENT_SERVER, &synMSG, sizeof(synMSG));
	}
}

VOID Handler_CG_STATUS::OnCG_STATUS_FREE_INIT_STAT_UNDER_20_SYN ( ServerSession * pServerSession, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_SYN * pRecvMsg = (MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STATUS::OnCG_STATUS_FREE_INIT_STAT_UNDER_20_SYN] Not Exist GameField" );
		return;
	}
	
	//if( !IsUnEquipAll() )
	//	return RC::RC_ITEM_NOT_UNEQUIP_ALL;
	RC::eSTATUS_RESULT rt = RC::RC_STATUS_SUCCESS;
#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
	if( !(pPlayer->GetItemManager()->IsUnEquipAll()) )
		rt = RC::RC_STATUS_NOT_UNEQUIP_ALL;
#endif
	if( pPlayer->GetCharInfo()->m_byFreeInitCount <= 0 )
		rt = RC::RC_STATUS_NOT_EXIST_INIT_POINT;

    LEVELTYPE max_level_for_free_stat_init = GameOptionParser::Instance()->GetFreeStatInitLevel();
    if (pPlayer->GetLevel() > max_level_for_free_stat_init) {
        rt = RC::RC_STATUS_PLAYER_LEVEL_TOO_HIGH_FOR_FREE_STAT_INIT;
    }

	if ( rt == RC::RC_STATUS_SUCCESS )
	{
		pPlayer->GetCharInfo()->m_byFreeInitCount--;
		WORD rDecrease = 1;
		pPlayer->InitStat( eItemWASTE_INIT_ALL, rDecrease );		
		pPlayer->InitStat( eItemWASTE_INIT_PASSIVE_ALL, rDecrease );
		
		MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_ACK MsgAck;
		MsgAck.m_byFreeInitCount = pPlayer->GetCharInfo()->m_byFreeInitCount;
		
		pPlayer->SendPacket( &MsgAck, sizeof(MsgAck) );		
	}
	else
	{	
		MSG_CG_STATUS_FREE_INIT_STAT_UNDER_20_NAK MsgNak;
		MsgNak.m_ResultCode = rt;
		pPlayer->SendPacket( &MsgNak, sizeof(MsgNak) );
	}
}

VOID	Handler_CG_STATUS::OnCG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	const MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN* pRecvMsg = (const MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN*)pMsg;
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if (pPlayer == NULL || pPlayer->GetField() == NULL) {
        return;
    }

	VarMsg2* const pVarMsgSRender = &(Player::m_VarMsg2[Player::SENDINFO_PLAYER_SPECIAL_RENDER]);
	MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD*	pSpecialRender = pVarMsgSRender->StartTypePtr(Param2Class<MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD>());
	pVarMsgSRender->SetSize( pSpecialRender->GetSize() );

	pSpecialRender->m_Count = 1;	// 1명 대상
	sMSG_RENDER_EFFECT_ITEM_LIST* pEffectNode = (sMSG_RENDER_EFFECT_ITEM_LIST*)pVarMsgSRender->NextPtr();
	pEffectNode->m_PlayerKey = pPlayer->GetObjectKey(); // _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET
	const DWORD alignedCount = pPlayer->RENDER_EFFECT.SerializeRenderInfo( pEffectNode->m_BitRender, pEffectNode->m_ItemCodes, sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS );
	pEffectNode->m_ItemCount = (BYTE)alignedCount;	// Grid와 달리 이것은 요청에 대한 정보 Broadcasting이며, 0인 경우 하나도 effect가 없는 상태가 될 것이다.
	pVarMsgSRender->SetSize( pEffectNode->GetSize() );

	pPlayer->SendExPacketAround( pVarMsgSRender->GetMsgCount(), pVarMsgSRender->GetMsgPPtr(), pVarMsgSRender->GetMsgSizePtr(), FALSE );
}

VOID	Handler_CG_STATUS::OnCG_STATUS_DELETE_CHAR_STATE_SYN( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	const MSG_CG_STATUS_DELETE_CHAR_STATE_SYN* pRecvMsg = (const MSG_CG_STATUS_DELETE_CHAR_STATE_SYN*)pMsg;
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	MSG_CG_STATUS_DELETE_CHAR_STATE_ACK msgACK;
	msgACK.m_bResult = FALSE;
	msgACK.m_eCharStateType = pRecvMsg->m_eCharStateType;

	StatusManager* pStatusManager = pPlayer->GetStatusManager();
	if( !pStatusManager )
	{
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
		return;
	}

	BaseStatus* pBaseStatus = pStatusManager->FindStatus( pRecvMsg->m_eCharStateType );
	if( !pBaseStatus )
	{
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
		return;
	}

	BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo( pBaseStatus->GetStateID() );
	if( !pStateInfo->m_bDelType )
	{
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );		
		return;
	}

	if( !pStatusManager->Remove( pRecvMsg->m_eCharStateType ) )
	{
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );		
		return;
	}
    
	msgACK.m_bResult = TRUE;
	pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
}
