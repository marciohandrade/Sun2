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

    // 플레이어가 사용한 소환스킬인지 몬스터가 사용한 스킬인지에 따라서 소환몬스터 종류가 달라진다.
    BOOL bPlayerSkill = attacker->IsEqualObjectKind(PLAYER_OBJECT);

    // 파라미터 셋팅
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    BYTE bySummonGroupType = ability_info->m_iOption1;        // 몬스터(0), 그룹(0)
    DWORD dwSummonID = ability_info->m_iOption2;      // 몬스터ID, 그룹ID
    BYTE bySummonedNum = ability_info->m_iParam[0];

    if (!bySummonedNum || bySummonedNum > MAX_SUMMONED_NUM) return false;

    // 소환 NPC 리스트를 찾거나 없으면 생성한다.
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

    // 소환대상이 그룹인 경우
    if (bySummonGroupType == 1)
    {
        BASE_GROUPINFO *pGroupInfo = GroupParser::Instance()->GetGroupInfo((WORD)dwSummonID);
        // 일단 보류...
    }

    // NPC를 생성한다.
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

    return false; // 의도된 false : msg 를 사용 하지 않는다. Effect 를 클라에 주지 않는다.
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
    //  - thinking... 사용 사례로 봐서는 regen될 위치 결정시 타일 고려가 필요하게 될 듯. 애구 할 일 많아지넹.
    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();

    // 몬스터 생성
    bool is_pos_input=false;
    for (DWORD i = 0; i < dwNumberOfNPCs; ++i)
    {
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        //고정형 소환수일 경우 main_target_pos 위치로 소환시킨다.
        //그 외엔 기존처럼 자기주위에 소환
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
            // 소환체의 위치결정(소환자와 너무 멀리 떨어지지 않도록 한다.)
            for (int iPosTrial = 0; iPosTrial < 51; ++iPosTrial)
            {
                wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);
                fMoveLength2 = VectorLength2(&(wvSummonerPos - wvSummonPos));
                if (fMoveLength2 >= 4.0 && fMoveLength2 <= 16.0)
                    break;

                // 50번을 도는 동안에도 소환체 위치를 결정하지 못했는가?
                if (iPosTrial >= 50)
                {
                    SUNLOG(eFULL_LOG, "[Ability::CreateSummoned] Can't Find wvSummonPos whithin 50 Times");
                    break;
                }
            }
        }
#else
        // 소환체의 위치결정(소환자와 너무 멀리 떨어지지 않도록 한다.)
        for (int iPosTrial = 0; iPosTrial < 51; ++iPosTrial)
        {
            wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);
            fMoveLength2 = VectorLength2(&(wvSummonerPos - wvSummonPos));
            if (fMoveLength2 >= 4.0 && fMoveLength2 <= 16.0)
                break;

            // 50번을 도는 동안에도 소환체 위치를 결정하지 못했는가?
            if (iPosTrial >= 50)
            {
                SUNLOG(eFULL_LOG, "[Ability::CreateSummoned] Can't Find wvSummonPos whithin 50 Times");
                break;
            }
        }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

        // 소환체 생성
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
    //  - thinking... 사용 사례로 봐서는 regen될 위치 결정시 타일 고려가 필요하게 될 듯. 애구 할 일 많아지넹.
    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();

    // 몬스터 생성
    for (DWORD i = 0; i < dwNumberOfNPCs; ++i)
    {
        // 위치 결정
        wvSummonPos = pMovingPlanner->GetRandomPosInTile(iCurTile);

        // 소환체 생성
        pField->CreateSummonByNPC(dwCode, &wvSummonPos, pSummoner, pOwnerSummonNPCs);
    }
}













