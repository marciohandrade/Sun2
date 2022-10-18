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

    {   //  Init(args...) 루틴 내용
        //  이것은 일반적인 루틴을 타지 않고 있다. 따라서 Monster::Init(...)와 동일하게 설정한다.
        INpcLinkActionD linkAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction(NPCCode, MONSTER_OBJECT);
        pTargetMonster->ChangeNpcDependencyChain(linkAction);
        // BaseNPCInfo 설정
        pTargetMonster->SetBaseInfo(NPCCode, pTargetMonster->GetStateManager()->GetMoveType(), 
            pTargetMonster->GetStateManager()->GetMoveAreaID(), STATE_ID_KNOCKDOWN, 4000);
    }

    // 새로 타겟을 설정한다. 그래야 Down 상태에서 풀린 후 돌아가지 않는다.
    Character* pTargetChar = pTargetMonster->SearchTarget();
    pTargetMonster->SetTargetChar(pTargetChar);

    // 방타입과 방인원에 따른 몬스터 능력치 변화
    g_ChangeMonsterAbility.UpdateMonster(pZone, pTargetMonster);

    // HP, MP 자동회복
    if (pTargetMonster->IsHPMPRegenNPC())
    {
        AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pTargetMonster->GetBaseInfo()->m_wAICode);
        pTargetMonster->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_HPMP_REGEN_PERIOD);
    }

    // SD 자동회복
    if (pTargetMonster->IsSDRegenNPC())
    {
        AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pTargetMonster->GetBaseInfo()->m_wAICode);
        pTargetMonster->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_AUTO_RECOVER_SD, BASE_EXPIRE_TIME_INFINITY, pAITypeInfo->m_wNPC_SD_REGEN_PERIOD);
    }

    // 패킷구조체를 채운다.
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





