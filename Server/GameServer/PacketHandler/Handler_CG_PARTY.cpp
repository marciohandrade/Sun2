#include "stdafx.h"
#include ".\Handler_CG_PARTY.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "Field.h"
#include "FieldServer.h"
#include "Map.h"
#include "Player.h"
#include <ServerSession.h>
#include "SkillInfoParser.h"
#include "GameZone.h"

#include "GameParty/NewGamePartyManager.h"

Handler_CG_PARTY::Handler_CG_PARTY()
{
}

Handler_CG_PARTY::~Handler_CG_PARTY()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_PARTY::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_PARTY::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_PARTY, CG_PARTY_MEMBER_WARP_REQ_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_PARTY, CG_PARTY_MEMBER_SUMMON_REQ_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_PARTY, CG_PARTY_MEMBER_SUMMON_RES_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


VOID Handler_CG_PARTY::OnCG_PARTY_MEMBER_WARP_REQ_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_PARTY_MEMBER_WARP_REQ_SYN * pRecvMsg = (MSG_CG_PARTY_MEMBER_WARP_REQ_SYN *)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_PARTY::OnCG_PARTY_MEMBER_WARP_REQ_SYN] Not Exist GameField" );
		return;
	}

	RC::ePARTY_RESULT rcResult = RC::RC_PARTY_SUCCESS;

	// �ش� ��ų�� �����ϴ��� üũ
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( eACTION_START_SKILL_CODE + eACTION_PARTY_MEMBER_WARP );
	if( !pBaseSkillInfo )
	{
		rcResult = RC::RC_PARTY_SKILLINFO_IS_NULL;
		goto RESULT_ERROR;
	}

	// ��Ÿ���� üũ�Ѵ�.
	util::Timer * pTimer = pPlayer->GetSkillCoolTimer( pBaseSkillInfo->m_SkillCode );
	if(!pTimer)
	{
		rcResult = RC::RC_PARTY_COOLTIME_ERROR;
		goto RESULT_ERROR;
	}

	if( !pTimer->IsExpired( FALSE ) )
	{
		rcResult = RC::RC_PARTY_COOLTIME_ERROR;
		goto RESULT_ERROR;
	}

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( !pZone )
	{
		rcResult = RC::RC_PARTY_INVALID_ZONESTATE;
		goto RESULT_ERROR;
	}

	// ������ ������ ������Ų��.
	rcResult = pZone->WarpAtPartyMaster( pPlayer );
	if( rcResult == RC::RC_PARTY_SUCCESS )
	{
		// ��Ÿ�� ����
		pTimer->Reset();
		return;
	}

RESULT_ERROR:
	MSG_CG_PARTY_MEMBER_WARP_REQ_NAK NakMsg;
	NakMsg.m_byErrorCode = rcResult;
	pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
}

VOID Handler_CG_PARTY::OnCG_PARTY_MEMBER_SUMMON_REQ_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_PARTY_MEMBER_SUMMON_REQ_SYN * pRecvMsg = (MSG_CG_PARTY_MEMBER_SUMMON_REQ_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_PARTY::OnCG_PARTY_MEMBER_SUMMON_REQ_SYN] Not Exist GameField" );
		return;
	}

	RC::ePARTY_RESULT rcResult = RC::RC_PARTY_SUCCESS;

	// �ش� ��ų�� �����ϴ��� üũ
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( eACTION_START_SKILL_CODE + eACTION_PARTY_MEMBER_SUMMON );
	if( !pBaseSkillInfo )
	{
		rcResult = RC::RC_PARTY_SKILLINFO_IS_NULL;
		goto RESULT_ERROR;
	}

	// ��Ÿ���� üũ�Ѵ�.
	util::Timer * pTimer = pPlayer->GetSkillCoolTimer( pBaseSkillInfo->m_SkillCode );
	if(!pTimer)
	{
		rcResult = RC::RC_PARTY_COOLTIME_ERROR;
		goto RESULT_ERROR;
	}

	if( !pTimer->IsExpired( FALSE ) )
	{
		rcResult = RC::RC_PARTY_COOLTIME_ERROR;
		goto RESULT_ERROR;
	}

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( !pZone )
	{
		rcResult = RC::RC_PARTY_INVALID_ZONESTATE;
		goto RESULT_ERROR;
	}

	// ��Ƽ�� ��ó�� ��ȯ ��û
	rcResult = pZone->SummonReqAtPartyMaster( pPlayer, pRecvMsg->m_dwObjKey );
	if( rcResult == RC::RC_PARTY_SUCCESS )
	{
		// ��Ÿ�� ����
		pTimer->Reset();
		return;
	}

RESULT_ERROR:
	MSG_CG_PARTY_MEMBER_SUMMON_REQ_NAK NakMsg;
	NakMsg.m_byErrorCode = rcResult;
	pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
}

VOID Handler_CG_PARTY::OnCG_PARTY_MEMBER_SUMMON_RES_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_PARTY_MEMBER_SUMMON_RES_SYN * pRecvMsg = (MSG_CG_PARTY_MEMBER_SUMMON_RES_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_PARTY::OnCG_PARTY_MEMBER_SUMMON_RES_SYN] Not Exist GameField" );
		return;
	}

	RC::ePARTY_RESULT rcResult = (RC::ePARTY_RESULT)pRecvMsg->m_byResultCode;

	// ������ ������ ���
	if( rcResult == RC::RC_PARTY_SUCCESS )
	{
		GameZone *pZone = pPlayer->GetGameZonePtr();
		if( pZone )
		{
			// ��Ƽ�� ��ó�� ������Ų��.
			rcResult = pZone->WarpAtPartyMaster( pPlayer );
		}
		else
			rcResult = RC::RC_PARTY_INVALID_ZONESTATE;
	}

	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		Player *pMasterPlayer = NewGamePartyManager::Instance()->FindMasterPlayer( pPlayer->GetPartyState().GetPartyKey() );

		if( !pMasterPlayer )	return ;

		MSG_CG_PARTY_MEMBER_SUMMON_RES_NAK NakMsg;
		NakMsg.m_byErrorCode = rcResult;
		pMasterPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
	}
}














