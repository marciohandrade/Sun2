#include "StdAfx.h"
#include "BombEffectAbility.h"
#include "BaseEffect.h"
#include "EffectManager.h"
#include "SkillInfoParser.h"
#include "PublicMath.h"
#include "Skill.h"


void BombEffectAbility::InitDetailed(Character* attacker,
                                     SLOTCODE skill_code, SkillInfo* skill_info,
                                     BYTE skill_stat_type,
                                     const BASE_ABILITYINFO* ability_info)
{
    Ability::InitDetailed(attacker, skill_code, skill_info, skill_stat_type, ability_info);

    if (skill_info)
    {
        main_target_key_ = skill_info->main_target_key_;
        main_target_pos_ = skill_info->main_target_pos_;
    }
    else
    {
        main_target_key_ = 0;
        memset(&main_target_pos_, 0, sizeof(main_target_pos_));
    }
}

bool BombEffectAbility::ExecuteEffect(BYTE* pMsg, WORD& wMsgSize)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }
    
    GameField *pField = attacker->GetField();
    if (!pField)
        return false;

    SKILL_RESULT_EFFECT *pEffectMsg = (SKILL_RESULT_EFFECT*)pMsg;
    if (!pEffectMsg)
        return false;

    pEffectMsg->m_byCount = 0;

    static WzVector vDestPos[MAX_EFFECT_COUNT];

    // �Ķ���� ����
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    float fEffectRadius = ability_info->m_iOption1 / 10.f;
    int iApplicationTime = max(1000, ability_info->m_iOption2);
    BYTE byAreaCount = min(MAX_EFFECT_COUNT, (BYTE)ability_info->m_iParam[0]);

    for (int i = 0; i < byAreaCount; ++i)
    {
        if (i >= MAX_EFFECT_COUNT)
        {
            SUNLOG(eCRITICAL_LOG, "[SusunLove] ExecuteEffect byAreaCount[%d]", byAreaCount);
            continue;
        }

        // ���� ������ ���Ѵ�.
        if (!GetRandomArea(vDestPos[i]))
            continue;

        // ����Ʈ�� �߰��Ѵ�.
        BaseEffect *pEffect = pField->GetEffectManager()->AllocEffect(EFFECT_TYPE_BOMB);
        if (!pEffect)
            continue;

        pEffect->Init(GetSkillCode(), iApplicationTime, 0, attacker, vDestPos[i], fEffectRadius);
        pEffect->SetSkillPercentDamage(ability_info->m_iParam[1] / 1000.0f);
        pEffect->Start();

        // ��Ŷ����ü�� EffectPos�� �����Ѵ�.
        pEffectMsg->m_EffectInfo[i].m_Time = iApplicationTime;
        pEffectMsg->m_EffectInfo[i].m_wvEffectPos = vDestPos[i];
        pEffectMsg->m_byCount++;
    }

    // ��Ŷ����ü�� ä���.
    pEffectMsg->m_wAbilityIndex = GetIndex();
    wMsgSize = pEffectMsg->GetSize();

    return true;
}

bool BombEffectAbility::GetRandomArea(WzVector& OUT result_dest_pos)
{
    const Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(GetSkillCode());
    if (!pBaseSkillInfo)
    {
        SUNLOG(eCRITICAL_LOG, "[Ability::GetRandomArea] SkillCode[%d] pBaseSkillInfo is NULL", GetSkillCode());
        return false;
    }

    // ���� ������ ���ϴµ� �ʿ��� �Ķ����
    int iAngle = 360;
    float fAreaRadius = max(1, pBaseSkillInfo->m_wSkillArea/10.0f);
    switch (pBaseSkillInfo->m_byAttRangeform)
    {
    case SRF_FOWARD_120:    iAngle = 120;   break;
    case SRF_FOWARD_160:    iAngle = 160;   break;
    case SRF_FOWARD_360:    iAngle = 360;   break;
    }

    WzVector vCurPos, vMainNormal;
    attacker->GetPos(&vCurPos);

    // MainTarget ���⺤�ͺ��� ���Ѵ�.
    if (main_target_key_ == attacker->GetObjectKey())
        VectorNormalize(&vMainNormal, &vCurPos);
    else
        VectorNormalize(&vMainNormal, &(main_target_pos_ - vCurPos));

    // MainTarget ������ iAngle �������� ������ ��ġ�� ���´�.
    int iRotateAngle = random(0, iAngle);
    if (iRotateAngle > (iAngle/2))      iRotateAngle = 360 - (iRotateAngle - iAngle/2);
    Math_ZRotateVector(&vMainNormal, result_dest_pos, iRotateAngle);

    // ���������� �Ÿ��� ���Ѵ�.
    float MOVE_DISTANCE = (float)dRandom(1, fAreaRadius);

    result_dest_pos = vCurPos + result_dest_pos * MOVE_DISTANCE;

    return true;
}













