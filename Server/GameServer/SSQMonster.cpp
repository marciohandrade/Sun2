#include "stdafx.h"

//����������������������������������������������������������������������������������
//

//����������������������������������������������������������������������������������
//
#include "Monster.h"
#include "SSQMonster.h"

#include "GameZone.h"
#include "GameInstanceDungeon.h"
#include <World/GameWarControlManager.h>
#include "AIParamParser.h"
#include "StatusManager.h"
#include "CharacterFormula.h"

SSQMonster::SSQMonster()
{
	SetObjectType( SSQMONSTER_OBJECT );
}

SSQMonster::~SSQMonster()
{
}


VOID
SSQMonster::InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID )
{
	this->Init();
	m_LinkedAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction( NPCCode, SSQMONSTER_OBJECT );
	NPC::SetBaseInfo( NPCCode, byMoveType, dwMoveAreaID );


}


BOOL
SSQMonster::Init()
{
	Monster::Init();
	Policyment.SetField( Policyment.PCC_IN_SSQ_FIELD );
	ControlStatus.Clear();

	return TRUE;
}

VOID
SSQMonster::Release()
{
	ControlStatus.Clear();
	Monster::Release();
}


VOID
SSQMonster::OnEnterField( GameField& IN rField, const WzVector& IN rwzVec, int iAngle )
{
    // SD �ڵ�ȸ��
    if( IsSDRegenNPC() )
    {
        AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
        m_pStatusManager->AllocStatus( eCHAR_STATE_ETC_AUTO_RECOVER_SD, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_SD_REGEN_PERIOD );
    }

	Monster::OnEnterField( rField, rwzVec, iAngle );
}

VOID
SSQMonster::OnLeaveField()
{
	Monster::OnLeaveField();
}

//{__NA_001290_20090525_SHIELD_SYSTEM
VOID
SSQMonster::OnEnterSector()
{
    Monster::OnEnterSector();

    const DWORD curSD = GetSD();
    if( curSD )
    {
        ALLOC_STATUS_PAIR pairBaseStatus = m_pStatusManager->AllocStatus( eCHAR_STATE_ETC_EXIST_SHELD_POINT );        
        if( pairBaseStatus.second )
            pairBaseStatus.second->SendStatusAddBRD();
    }
}

VOID
SSQMonster::UpdateCalcRecover(BOOL hp_updated, BOOL mp_updated, BOOL sd_updated)
{
    __UNUSED((hp_updated, mp_updated));
    const int hp_recovery_quantity = 0;
    const int mp_recovery_quantity = 0;
    int sd_recovery_quantity = 0;
    if (sd_updated)
    {
        eCHAR_CONDITION char_condition = GetStatusManager()->GetCondition();
        LEVELTYPE level = this->GetLevel();

        const uint8_t moving_flag = IsMoving() ? CHAR_ACTION_CONDITION_MOVING
                                               : CHAR_ACTION_CONDITION_NONE;
        // TODO: f110413.1L, changes the method to get fighting_flag.
        const uint8_t fighting_flag = GetStatusManager()->FindStatus(eCHAR_STATE_ETC_FIGHTING)
            ?   CHAR_ACTION_CONDITION_FIGHTING
            :   CHAR_ACTION_CONDITION_NONE;

        sd_recovery_quantity = CalcSDRecover(
            eCHAR_NONE, char_condition, (moving_flag | fighting_flag), level);
    };
    //
    if (sd_updated)
    {
        // CHANGES: f110413.1L, change logic to reduce a busy big update process
        // because HP, MP and SD recovery update routine is often invoked by a changed value.
        m_Attr.UpdateChangedRecoveries(
            false,                 hp_recovery_quantity,
            false,                 mp_recovery_quantity,
            (sd_updated != false), sd_recovery_quantity);
    };
}

//}__NA_001290_20090525_SHIELD_SYSTEM

VOID
SSQMonster::OnDead()
{
	GameField* const pGameField = GetField();
	if( !pGameField )
		return;

	GameZone* const pGameZone = GetGameZonePtr();

	// ������ �̺�Ʈ ����
	if( pGameZone )
		pGameZone->OnKillMonster( this );

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(m_dwKillerObjectKey);
    if (pPlayer == NULL)
    {
        Character* const killer_character = pGameField->FindCharacter(m_dwKillerObjectKey);
        if ((killer_character != NULL) && killer_character->IsEqualObjectKind(SUMMON_OBJECT))
        {
            pPlayer = killer_character->GetPlayerOwner();
        }
    }
	if (pPlayer != NULL)
	{	// for Trigger & Quest & ItemDeleteEvent + Event for SSQ
		m_LinkedAction.OnDead( pGameField, this, pPlayer, m_dwKillerObjectKey );
	}
	else
	{
		const eZONETYPE zone_type = pGameZone->GetZoneType();		//pGameZone...NULL��Ȳ�� �ִ°�?
		if( zone_type == eZONETYPE_INSTANCE )
		{
			GameInstanceDungeon* const pDungeon = static_cast<GameInstanceDungeon*>( pGameZone );
			if( pDungeon->GetTypeOfIndun() == INDUN_KIND_SS_QUEST )
				GameWarControlManager::OnMsg( GameSSQCtrlCmd::CharacterEvent::OnDead( this, NULL ), pDungeon );
		}
	}

	//Monster::OnDead(); �� �ǳʶڴ�.
	//	Monster::OnDead�� ������ �Ŀ� ó���ϵ��� �Ѵ�.
	NPC::OnDead();
}

