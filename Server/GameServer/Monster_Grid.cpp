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

	// 방타입과 방인원에 따른 몬스터 능력치 변화
	g_ChangeMonsterAbility.UpdateMonster( rField.GetGameZone(), this );

	// HP, MP 자동회복
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