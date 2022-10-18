#include "StdAfx.h"
#include "SummonAbility.h"
#include "SummonedNPCs.h"
#include "SummonManager.h"
#include "GroupParser.h"
#include "Summoned.h"
#include "MovingPlanner.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Skill.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

bool SummonAbility::ExecuteEffect(BYTE* /*msg*/, WORD& /*msg_size*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    GameField *pField = attacker->GetField();
    if (!pField)
        return false;

    // �÷��̾ ����� ��ȯ��ų���� ���Ͱ� ����� ��ų������ ���� ��ȯ���� ������ �޶�����.
    BOOL bPlayerSkill = attacker->IsEqualObjectKind(PLAYER_OBJECT);

    // �Ķ���� ����
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    BYTE bySummonGroupType = ability_info->m_iOption1;        // ����(0), �׷�(0)
    DWORD dwSummonID = ability_info->m_iOption2;      // ����ID, �׷�ID
    BYTE bySummonedNum = ability_info->m_iParam[0];

    if (!bySummonedNum || bySummonedNum > MAX_SUMMONED_NUM) return false;

    // ��ȯ NPC ����Ʈ�� ã�ų� ������ �����Ѵ�.
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    SummonedNPCs *pOwnerSummonNPCs = NULL;
    Player* player = NULL;
    if (bPlayerSkill == true) {
        player = reinterpret_cast<Player*>(attacker);
    }
    if (player != NULL && player->GetCharType() == eCHAR_WITCHBLADE)
    {
        pOwnerSummonNPCs = SummonManager::Instance()->AllocSummonNPCs(attacker, bPlayerSkill, 
                                                                      ability_info->m_wAbilityID);
    }
    else
    {
         pOwnerSummonNPCs = SummonManager::Instance()->AllocSummonNPCs(attacker, bPlayerSkill);
    }
#else
    SummonedNPCs *pOwnerSummonNPCs = SummonManager::Instance()->AllocSummonNPCs(attacker, bPlayerSkill);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    if (!pOwnerSummonNPCs)
        return false;

    // ��ȯ����� �׷��� ���
    if (bySummonGroupType == 1)
    {
        BASE_GROUPINFO *pGroupInfo = GroupParser::Instance()->GetGroupInfo((WORD)dwSummonID);
        // �ϴ� ����...
    }

    // NPC�� �����Ѵ�.
    if (bPlayerSkill)
    {
        CreateSummoned(attacker, pOwnerSummonNPCs, dwSummonID, bySummonedNum);
    }
    else
    {
        CreateSummonNPC(attacker, pOwnerSummonNPCs, dwSummonID, bySummonedNum);
    }

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
    attacker->SetSummonerKey(pOwnerSummonNPCs->GetSummonerKey());
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

    return false; // �ǵ��� false : msg �� ��� ���� �ʴ´�. Effect �� Ŭ�� ���� �ʴ´�.
}

void SummonAbility::CreateSummoned(Character *pSummoner, SummonedNPCs *pOwnerSummonNPCs, DWORD dwCode, DWORD dwNumberOfNPCs)
{
    GameField *pField = pSummoner->GetField();
    if (!pField)
        return;

    int iCurTile = pSummoner->GetPathExplorer()->GetTile();
    if (iCurTile < 0)   
    {
        WzVector wvSummonerPos;     pSummoner->GetPos(&wvSummonerPos);
        SUNLOG(eCRITICAL_LOG, 
            "[CreateSummoned] Summoner "
            "Key[%d] Type[%d] CurTile[%d]"
            "FCode[%d] pos[%d,%d,%d]", 
            pSummoner->GetObjectKey(), pSummoner->GetObjectType(), iCurTile,
            pField->GetFieldCode(), wvSummonerPos.x, wvSummonerPos.y, wvSummonerPos.z);
        return;
    }

    DWORD dwExpireTime = GetBaseAbilityInfo()->m_iParam[2];
    WzVector wvSummonerPos;     pSummoner->GetPos(&wvSummonerPos);
    WzVector wvSummonPos;   float fMoveLength2 = 0;

    //  (WAVERIX)(080926)(NOTE)
    //  - thinking... ��� ��ʷ� ������ regen�� ��ġ ������ Ÿ�� ����� �ʿ��ϰ� �� ��. �ֱ� �� �� ��������.
    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();

    // ���� ����
    bool is_pos_input=false;
    for (DWORD i = 0; i < dwNumberOfNPCs; ++i)
    {
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        //������ ��ȯ���� ��� main_target_pos ��ġ�� ��ȯ��Ų��.
        //�� �ܿ� ����ó�� �ڱ������� ��ȯ
        is_pos_input=false;
        if (GetAbilityID() == eABILITY_SUMMON_IMMOVABLE)
        {
            Skill* skill = GetSKill();
            if (skill != NULL)
            {
                SkillInfo* skill_info = skill->GetSkillInfo();
                if(skill_info != NULL) 
                {
                    wvSummonPos = skill_info->main_target_pos_;
                    is_pos_input = true;
                }
            }
        }

        if (is_pos_input == false)
        {
            // ��ȯü�� ��ġ����(��ȯ�ڿ� �ʹ� �ָ� �������� �ʵ��� �Ѵ�.)
            for (int iPosTrial = 0; iPosTrial < 51; ++iPosTrial)
            {
                wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);
                fMoveLength2 = VectorLength2(&(wvSummonerPos - wvSummonPos));
                if (fMoveLength2 >= 4.0 && fMoveLength2 <= 16.0)
                    break;

                // 50���� ���� ���ȿ��� ��ȯü ��ġ�� �������� ���ߴ°�?
                if (iPosTrial >= 50)
                {
                    SUNLOG(eFULL_LOG, "[Ability::CreateSummoned] Can't Find wvSummonPos whithin 50 Times");
                    break;
                }
            }
        }
#else
        // ��ȯü�� ��ġ����(��ȯ�ڿ� �ʹ� �ָ� �������� �ʵ��� �Ѵ�.)
        for (int iPosTrial = 0; iPosTrial < 51; ++iPosTrial)
        {
            wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);
            fMoveLength2 = VectorLength2(&(wvSummonerPos - wvSummonPos));
            if (fMoveLength2 >= 4.0 && fMoveLength2 <= 16.0)
                break;

            // 50���� ���� ���ȿ��� ��ȯü ��ġ�� �������� ���ߴ°�?
            if (iPosTrial >= 50)
            {
                SUNLOG(eFULL_LOG, "[Ability::CreateSummoned] Can't Find wvSummonPos whithin 50 Times");
                break;
            }
        }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

        // ��ȯü ����
        Summoned* pSummoned = pField->CreateSummonByPlayer(dwCode, &wvSummonPos, pSummoner, pOwnerSummonNPCs, dwExpireTime, GetSkillCode(), NULL);
        if (pSummoned)
        {
            MSG_CG_SUMMON_CURRENT_HP_CMD summonMsg;
            summonMsg.m_dwSummonedObjKey    = pSummoned->GetObjectKey();
            summonMsg.m_dwMaxHP             = pSummoned->GetMaxHP();
            summonMsg.m_dwHP                = pSummoned->GetHP();
            summonMsg.m_dwSpeedRatio        = pSummoned->GetMoveSpeedRatio();
            pSummoner->SendPacket(&summonMsg, sizeof(MSG_CG_SUMMON_CURRENT_HP_CMD));
        }
    }
}

void SummonAbility::CreateSummonNPC(Character *pSummoner, SummonedNPCs *pOwnerSummonNPCs, DWORD dwCode, DWORD dwNumberOfNPCs)
{
    GameField *pField = pSummoner->GetField();
    if (!pField) return;

    int iCurTile = pSummoner->GetPathExplorer()->GetTile();
    if (iCurTile < 0)   
    {
        SUNLOG(eCRITICAL_LOG, "[CreateSummonNPC] pSummoner Key[%d] Type[%d] CurTile[%d]", pSummoner->GetObjectKey(), pSummoner->GetObjectType(), iCurTile);
        return;
    }

    WzVector wvSummonPos;

    //  (WAVERIX)(080926)(NOTE)
    //  - thinking... ��� ��ʷ� ������ regen�� ��ġ ������ Ÿ�� ����� �ʿ��ϰ� �� ��. �ֱ� �� �� ��������.
    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();

    // ���� ����
    for (DWORD i = 0; i < dwNumberOfNPCs; ++i)
    {
        // ��ġ ����
        wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);

        // ��ȯü ����
        pField->CreateSummonByNPC(dwCode, &wvSummonPos, pSummoner, pOwnerSummonNPCs);
    }
}













