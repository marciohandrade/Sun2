#include "StdAfx.h"
#include "Handler_CG_SUMMON.h"
#include "Player.h"
#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "PublicMath.h"
#include "float.h"
#include "SkillInfoParser.h"
#include "Summoned.h"
#include "Skill.h"
#include "ChaoState.h"
#include "GameDominationField.h"

Handler_CG_SUMMON::Handler_CG_SUMMON()
{
}

Handler_CG_SUMMON::~Handler_CG_SUMMON()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_SUMMON::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_SUMMON::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_SUMMON, CG_SUMMON_COMMAND_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SUMMON, CG_SUMMON_SKILL_ACTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SUMMON, CG_CRYSTALWARP_COMMAND_SYN, PHASE_SERVICE),
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


VOID Handler_CG_SUMMON::OnCG_SUMMON_COMMAND_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_SUMMON_COMMAND_SYN* pRecv = (MSG_CG_SUMMON_COMMAND_SYN*)pMsg;
	MSG_CG_SUMMON_COMMAND_NAK NakMsg;
	NakMsg.m_byCommand = pRecv->m_byCommand;
	eSUMMON_COMMAND eCommand = (eSUMMON_COMMAND)pRecv->m_byCommand;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecv->m_dwKey );
	if (!pPlayer) return;

    GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_SUMMON::OnCG_SUMMON_COMMAND_SYN] Not Exist GameField" );
		return;
	}

	// �����ϴ� ������� üũ
	if( eCommand >= eSUMMON_COMMAND_MAX )
	{
		NakMsg.m_byErrorCode = RC::RC_SUMMON_INVALID_COMMAND;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	if( eCommand == eSUMMON_COMMAND_ATTACK )
	{
		Character* pTargetChar = pField->FindCharacter( pRecv->m_dwTargetKey );

		if( !pTargetChar )
		{
			NakMsg.m_byErrorCode = RC::RC_BATTLE_INVALID_MAINTARGET;
			pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
			return;
		}

		sPOLICY_CHARACTER& rPolicyment = pPlayer->Policyment;
		const BOOLEAN bApplyGeneralForcedAtkRule = !( rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD );
		if( bApplyGeneralForcedAtkRule )
		{
			bool befForcedAttack = pPlayer->IsForceAttack();
			pPlayer->SetForceAttack( pRecv->m_byAttackPropensity==ATTACK_PROPENSITY_FORCE?true:false );
			RC::eSKILL_RESULT rcResult = pPlayer->ChkSkillPvP(pTargetChar);
			if( rcResult != RC::RC_SKILL_SUCCESS )
			{
				pPlayer->SetForceAttack(befForcedAttack);
				NakMsg.m_byErrorCode = rcResult;
				pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
				return;
			}
		}
	}

    RC::eSUMMON_RESULT rcResult = SummonManager::Instance()->SetCommandState( pPlayer->GetObjectKey(), eCommand, 0, pRecv->m_dwTargetKey );
	if( rcResult != RC::RC_SUMMON_SUCCESS )
	{
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	MSG_CG_SUMMON_COMMAND_ACK AckMsg;
	AckMsg.m_byCommand = pRecv->m_byCommand;
	pPlayer->SendPacket( &AckMsg, sizeof(AckMsg) );
}

VOID Handler_CG_SUMMON::OnCG_SUMMON_SKILL_ACTION_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_SUMMON_SKILL_ACTION_SYN* pRecvMsg = (MSG_CG_SUMMON_SKILL_ACTION_SYN*)pMsg;
	MSG_CG_SKILL_ACTION_NAK NakMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if (!pPlayer) return;

    GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_SUMMON::OnCG_SUMMON_SKILL_ACTION_SYN] Not Exist GameField" );
		return;
	}

	// ���� ��ǥ�� ��ȿ���� üũ
	if( !_finite(pRecvMsg->m_wvCurPos.x) || !_finite(pRecvMsg->m_wvCurPos.y) || !_finite(pRecvMsg->m_wvCurPos.z) )
	{
		SUNLOG( eCRITICAL_LOG,  "[OnCG_SUMMON_SKILL_ACTION_SYN] CurPos is Invalid! x[%f] y[%f] z[%f]", pRecvMsg->m_wvCurPos.x, pRecvMsg->m_wvCurPos.y, pRecvMsg->m_wvCurPos.z );
		NakMsg.m_byErrorCode = RC::RC_SKILL_POSITION_INVALID;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// ������ ��ǥ�� ��ȿ���� üũ
	if( !_finite(pRecvMsg->m_wvDestPos.x) || !_finite(pRecvMsg->m_wvDestPos.y) || !_finite(pRecvMsg->m_wvDestPos.z) )
	{
		SUNLOG( eCRITICAL_LOG,  "[OnCG_SUMMON_SKILL_ACTION_SYN] DestPos is Invalid! x[%f] y[%f] z[%f]", pRecvMsg->m_wvDestPos.x, pRecvMsg->m_wvDestPos.y, pRecvMsg->m_wvDestPos.z );
		NakMsg.m_byErrorCode = RC::RC_SKILL_POSITION_INVALID;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// ��ȯü�� �����ϴ��� üũ
	Summoned *pSummoned = NULL;
	SummonedNPCs *pSummonNPCs = SummonManager::Instance()->FindSummonNPCs( pPlayer->GetObjectKey() );
	if( pSummonNPCs )	pSummoned = (Summoned*)pSummonNPCs->GetSummoned( pRecvMsg->m_dwSummonedObjKey );
	if( !pSummoned )
	{
		NakMsg.m_byErrorCode = RC::RC_SKILL_SUMMONED_NOTEXIST;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// Ŭ��� �������� ��ȯü ��ġ ����
	WzVector wvResultCurPos, wvResultDestPos;
	pSummoned->ModifyPosition( FALSE, pRecvMsg->m_wvCurPos, pRecvMsg->m_wvDestPos, wvResultCurPos, wvResultDestPos, pRecvMsg->m_SkillCode );

	// ��ȯ��(�÷��̾�)�� �� ��ų�� �����ϰ� �ִ��� üũ
	RC::eSKILL_RESULT rcResult = pPlayer->CanUseSummonSkill( pRecvMsg->m_SkillCode );
	if( rcResult != RC::RC_SKILL_SUCCESS )
	{
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// ��ȯü�� �� ��ų�� ����� �� �ִ��� üũ
	rcResult = pSummoned->CanUseSkill( pRecvMsg->m_SkillCode, pRecvMsg->m_dwMainTargetKey, FALSE );
	if( rcResult != RC::RC_SKILL_SUCCESS )
	{
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	AI_MSG_USE_SKILL AISkillMsg;
	AISkillMsg.dwSkillCode = pRecvMsg->m_SkillCode;
	AISkillMsg.dwTargetKey = pRecvMsg->m_dwMainTargetKey;
	AISkillMsg.vTargetPos = pRecvMsg->m_wvMainTargetPos;
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
    AISkillMsg.dwSelectedSkillDelay = pRecvMsg->m_dwClientSerial;
#endif //_NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY

	// MainTarget ��ǥ�� �����Ѵ�.
	if( !CheckMainTarget( pSummoned, AISkillMsg.dwSkillCode, AISkillMsg.dwTargetKey, AISkillMsg.vTargetPos ) )
	{
		NakMsg.m_byErrorCode = RC::RC_SKILL_TARGET_NOTEXIST;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
		return;
	}

	// PK ���� üũ
	if( pRecvMsg->m_byAttackPropensity )
	{
		Character* pTargetChar = pField->FindCharacter( pRecvMsg->m_dwMainTargetKey );

		if( !pTargetChar )
		{
			NakMsg.m_byErrorCode = RC::RC_BATTLE_INVALID_MAINTARGET;
			pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
			return;
		}

		sPOLICY_CHARACTER& rPolicyment = pPlayer->Policyment;
		const BOOLEAN bApplyGeneralForcedAtkRule = !( rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD );
		if( bApplyGeneralForcedAtkRule )
		{
			bool befForcedAttack = pPlayer->IsForceAttack();
			pPlayer->SetForceAttack( ATTACK_PROPENSITY_FORCE );
			rcResult = pPlayer->ChkSkillPvP( pTargetChar );
			if( rcResult != RC::RC_SKILL_SUCCESS )
			{
				pPlayer->SetForceAttack(befForcedAttack);
				NakMsg.m_byErrorCode = rcResult;
				pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
				return;
			}
		}
	}

	// ��ȯü�� ��ų�� ����ϵ��� AI �޼����� ������.
	pSummoned->SendAIMessage( &AISkillMsg, sizeof(AISkillMsg) );

	// ��ȯü�� ��ų��� ��尡 �ǵ��� �����Ѵ�.
	pSummonNPCs->SetCommandState( eSUMMON_COMMAND_SKILL_ATTACK, pRecvMsg->m_dwSummonedObjKey, 0 );
}

VOID Handler_CG_SUMMON::OnCG_CRYSTALWARP_COMMAND_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_CRYSTALWARP_COMMAND_SYN* pRecv = (MSG_CG_CRYSTALWARP_COMMAND_SYN*)pMsg;
    eCRYSTALWARP_COMMAND eCommand = (eCRYSTALWARP_COMMAND)pRecv->m_byCommand;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    return; // ���������� ������� ���� ũ����Ż���� �̻��..
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecv->m_dwKey );

    S_TRY
    {
        if( !pPlayer ) 
            S_THROW(RC::RC_SUMMON_INVALID_PLAYER);

        if( !pPlayer->GetField() )
            S_THROW(RC::RC_SUMMON_INVALID_FIELD);

        GameZone* pZone = pPlayer->GetGameZonePtr();
        if(NULL == pZone)
            S_THROW(RC::RC_SUMMON_INVALID_FIELD);

        if(eZONETYPE_DOMINATION_FIELD != pZone->GetZoneType())
            S_THROW(RC::RC_SUMMON_INVALID_DOMINATION_FIELD);

        if( eCommand >= eCRYSTALWARP_COMMAND_MAX )  // �����ϴ� ������� üũ
            S_THROW(RC::RC_SUMMON_INVALID_COMMAND);

                
        GameDominationField* pDomiZone = (GameDominationField*)pZone;
        DWORD     dwObjKey  = pRecv->m_dwCrystalWarpObjKey;
        GUILDGUID guildGuid = pPlayer->GetGuildGuid();

        RC::eSUMMON_RESULT rcResult = pDomiZone->SetCrystalWarpCommand(eCommand,guildGuid,pRecv->m_wvPos,dwObjKey,pPlayer);
        if( rcResult != RC::RC_SUMMON_SUCCESS )
            S_THROW(rcResult);
    }
    S_CATCH(DWORD, dwErr)
    {
        switch(dwErr)
        {
        case RC::RC_SUMMON_INVALID_FIELD:
            SUNLOG( eCRITICAL_LOG, "[Handler_CG_SUMMON::OnCG_CRYSTALWARP_COMMAND_SYN] Not Exist GameField" );
        case RC::RC_SUMMON_INVALID_PLAYER:
            return;
        }

        MSG_CG_CRYSTALWARP_COMMAND_NAK NakMsg;
        NakMsg.m_byCommand = eCommand;
        NakMsg.m_byErrorCode = (BYTE)dwErr;
        pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
        return;
    }

    MSG_CG_CRYSTALWARP_COMMAND_BRD brdMsg;
    brdMsg.m_byCommand = eCommand;
    brdMsg.m_dwObjKey = pPlayer->GetObjectKey();

    pPlayer->SendPacketAround(&brdMsg, sizeof(brdMsg));
}

BOOL Handler_CG_SUMMON::CheckMainTarget( Summoned *pSummoned, SLOTCODE SkillCode, DWORD &dwMainTragetKey, WzVector &vMainTragetPos )
{
	GameField *pField = pSummoned->GetField();
	if( !pField )	return FALSE;
	SkillScriptInfo *pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo( SkillCode );

	// ����Ÿ�� ��ǥ�� �����Ѵ�.
	if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_ME )
	{
		dwMainTragetKey = pSummoned->GetObjectKey();
		pSummoned->GetPos( &vMainTragetPos );
	}
	else if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_AREA )
	{
	}
	else if( pBaseSkillInfo->m_byAttRangeform != SRF_PIERCE )		// ���뽺ų�� ��� m_wvMainTargetPos�� �״�� ����.
	{
		Character *pMainTarget = pField->FindCharacter( dwMainTragetKey );
		if( !pMainTarget )		return FALSE;
		pMainTarget->GetPos( &vMainTragetPos );
	}
	return TRUE;
}



























