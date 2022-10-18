#include "StdAfx.h"
#include "MonsterTransAbility.h"
#include "NPCStateManager.h"
#include "NPCInfoParser.h"
#include "Monster.h"
#include "ChangeMonsterAbility.h"
#include "AIParamParser.h"
#include "StatusManager.h"


bool MonsterTransAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    Ability::Execute(target, pMsg, wMsgSize);

    SKILL_RESULT_MONSTER_TRANSFORMATION* pCodeMsg = (SKILL_RESULT_MONSTER_TRANSFORMATION*)pMsg;

    GameZone* pZone = target->GetGameZonePtr();
    if (!pZone) return false;

    if (target->IsEqualObjectKind(MONSTER_OBJECT) == 0)
        return false;

    Monster* pTargetMonster = (Monster*)target;

    WORD NPCCode = GetBaseAbilityInfo()->m_iOption1;
    BASE_NPCINFO* pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo(NPCCode);
    if (!pBaseNPCInfo)
        return false;

    {   //  Init(args...) ��ƾ ����
        //  �̰��� �Ϲ����� ��ƾ�� Ÿ�� �ʰ� �ִ�. ���� Monster::Init(...)�� �����ϰ� �����Ѵ�.
        INpcLinkActionD linkAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction(NPCCode, MONSTER_OBJECT);
        pTargetMonster->ChangeNpcDependencyChain(linkAction);
        // BaseNPCInfo ����
        pTargetMonster->SetBaseInfo(NPCCode, pTargetMonster->GetStateManager()->GetMoveType(), 
            pTargetMonster->GetStateManager()->GetMoveAreaID(), STATE_ID_KNOCKDOWN, 4000);
    }

    // ���� Ÿ���� �����Ѵ�. �׷��� Down ���¿��� Ǯ�� �� ���ư��� �ʴ´�.
    Character* pTargetChar = pTargetMonster->SearchTarget();
    pTargetMonster->SetTargetChar(pTargetChar);

    // ��Ÿ�԰� ���ο��� ���� ���� �ɷ�ġ ��ȭ
    g_ChangeMonsterAbility.UpdateMonster(pZone, pTargetMonster);

    // HP, MP �ڵ�ȸ��
    if (pTargetMonster->IsHPMPRegenNPC())
    {
        AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pTargetMonster->GetBaseInfo()->m_wAICode);
        pTargetMonster->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_HPMP_REGEN_PERIOD);
    }

    // SD �ڵ�ȸ��
    if (pTargetMonster->IsSDRegenNPC())
    {
        AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pTargetMonster->GetBaseInfo()->m_wAICode);
        pTargetMonster->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_AUTO_RECOVER_SD, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_SD_REGEN_PERIOD);
    }

    // ��Ŷ����ü�� ä���.
    if (pCodeMsg)
    {
        wMsgSize = pCodeMsg->GetSize();
        pCodeMsg->m_dwHP = target->GetHP();
        pCodeMsg->m_dwMaxHP = target->GetMaxHP();
        pCodeMsg->m_wMoveSpeedRatio = target->GetMoveSpeedRatio();
        pCodeMsg->m_wAttSpeedRatio = target->GetAttSpeedRatio();
    }

    return true;
}





