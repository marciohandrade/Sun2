#include "stdafx.h"
#include "Monster.h"
#include "ChangeMonsterAbility.h"
#include "Map.h"
#include "AIParamParser.h"
#include "StatusManager.h"
#include "SummonManager.h"

VOID Monster::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	SetRegenPos( rwzVec );

	// ��Ÿ�԰� ���ο��� ���� ���� �ɷ�ġ ��ȭ
	g_ChangeMonsterAbility.UpdateMonster( rField.GetGameZone(), this );

	// HP, MP �ڵ�ȸ��
	if( IsHPMPRegenNPC() )
	{
		AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
		m_pStatusManager->AllocStatus( eCHAR_STATE_ETC_AUTO_RECOVER_HPMP, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_HPMP_REGEN_PERIOD );
	}

	NPC::OnEnterField( rField, rwzVec, iAngle );
}

VOID Monster::OnLeaveField()
{
	NPC::OnLeaveField();
}