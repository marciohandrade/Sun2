#include "StdAfx.h"
#include "SummonCrystalWarpAbility.h"
#include "CrystalWarp.h"
#include "Skill.h"
#include "SkillInfoParser.h"
#include "GameDominationField.h"

bool SummonCrystalWarpAbility::ExecuteEffect(BYTE* /*msg*/, WORD& /*msg_size*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    if (false == attacker->IsEqualObjectKind(PLAYER_OBJECT))
        return false;

    Player*    pPlayer = (Player*)attacker;
    GameField* pField = attacker->GetField();
    GameZone*  pZone  = attacker->GetGameZonePtr();

    if (NULL == pField || NULL == pZone) 
        return false;

    if (eZONETYPE_DOMINATION_FIELD != pZone->GetZoneType())
        return false;

    const GUILDGUID guildGuid = pPlayer->GetGuildGuid();
    GameDominationField* pGameDominationField = (GameDominationField*)pZone;

    const eABILITY abilityID = GetAbilityID();

    switch (abilityID)
    {
    case eABILITY_SUMMON_CRYSTALWARP:
        {
        #ifdef _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD
            if (pPlayer->IsDead()) // ��ȯ������ �� �÷��̾ ������ ��ȯ false..
            {
                return false;
            }
        #endif // _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD
            const FIELDCODE fCode = pField->GetFieldCode();
            const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
            const WORD  wSummonID = ability_info->m_iOption2;		                 // ����ID
            const DWORD dwExpireTime = ability_info->m_iParam[2];                  // �Ҹ�ð�
            const WzVector wvCrystalWarp = GetSKill()->GetSkillInfo()->main_target_pos_; // ��ȯ��ġ

            CrystalWarp* pCrystalWarp = pField->CreateSummonCrystalWarp(wSummonID,
                                            &wvCrystalWarp, guildGuid, dwExpireTime);
            if (NULL == pCrystalWarp)
                return false;

            if (false == pGameDominationField->InsertCrystalWarp(guildGuid,pCrystalWarp))
            {
                pField->DestroyObject(pCrystalWarp);
                return false;
            }

            MSG_CG_SUMMON_CURRENT_HP_CMD summonMsg;
            summonMsg.m_dwSummonedObjKey = pCrystalWarp->GetObjectKey();
            summonMsg.m_dwMaxHP			 = pCrystalWarp->GetMaxHP();
            summonMsg.m_dwHP			 = pCrystalWarp->GetHP();
            summonMsg.m_dwSpeedRatio	 = pCrystalWarp->GetMoveSpeedRatio();
            attacker->SendPacket(&summonMsg, sizeof(MSG_CG_SUMMON_CURRENT_HP_CMD));

            GAMELOG->LogDomination(ACT_DOMINATION_CRYSTALWARP_CREATE, pPlayer, pCrystalWarp);
        }
        break;

    case eABILITY_SUMMON_CRYSTALWARP_DESTROY:
        {
            const DWORD dwCrystalWarpKey = GetSKill()->GetSkillInfo()->main_target_key_; // ��ȯ��
            if (false == pGameDominationField->DestroyCrystalWarp(pPlayer, dwCrystalWarpKey))
                return false;
        }
        break;
    }

    return false;   // �ǵ��� false : msg �� ��� ���� �ʴ´�.
}