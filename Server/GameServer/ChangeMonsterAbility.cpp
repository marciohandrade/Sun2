#include "StdAfx.h"
#include "ChangeMonsterAbility.h"
#include "GameZone.h"
#include "Monster.h"
#include "GameCHuntingRoom.h"
#include "CHuntingManager.h"
#include "CharacterFormula.h"
#include "Map.h"

ChangeMonsterAbility g_ChangeMonsterAbility;

VOID
ChangeMonsterAbility::UpdateMonster(GameZone* pZone, NPC* pNPC)
{
    if(pZone == 0)
        return;

    sPOLICY_ZONE& Policyment = pZone->Policyment;
    if(Policyment.FIELD & Policyment.ENABLE_CHANGE_MOB_ABILITY)
    {
        const BYTE player_count = GetPlayerNum(pZone);
        if(!player_count)
            return;

        pNPC->SetHPAttackPowerAsParty(player_count);
    }
}


VOID
ChangeMonsterAbility::UpdateAllMonster(GameZone* pZone)
{
    if(pZone == 0)
        return;

    sPOLICY_ZONE& Policyment = pZone->Policyment;
    if(Policyment.FIELD & Policyment.ENABLE_CHANGE_MOB_ABILITY)
    {
        const BYTE player_number = GetPlayerNum(pZone, TRUE);
        if(player_number == 0)
            return;

        // ����� HP ������ ���� �˸���.
        const FLOAT fIncreaseRatio = CalcHPIncreaseRatioAsParty(player_number);
        MSG_CG_STATUS_CHANGE_MONSTER_ABILITY msg;
        msg.m_fHPRatio = fIncreaseRatio;
        pZone->SendPacketToAll(&msg, sizeof(msg));

        // Zone�� ��� �ʵ忡 �����Ѵ�.
        const _GAMEFIELD_HASH& rFields = pZone->GetFieldHash();
        FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
        {
            GameField* const pField = rField.second;

            typedef GameField::_NPC_HASH    _NPC_HASH;
            const _NPC_HASH& rMobs = pField->GetRefNPCList();
            FOREACH_CONTAINER(const _NPC_HASH::value_type& rMob, rMobs, _NPC_HASH)
            {
                NPC* const pNPC = rMob.second;
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
                if (pNPC != NULL && pNPC->IsDeleted())
                {
                    const Object::Key key = pNPC->GetObjectKey();
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        key
                    );
                    continue;
                }
//#endif
                pNPC->SetHPAttackPowerAsParty(player_number);
            }
        }
    }
}

void
ChangeMonsterAbility::NotifyChangeAbility(GameZone* pZone, Player* pPlayer)
{
    BYTE number_of_players = GetPlayerNum(pZone);
    if(number_of_players == 0)
        return;

    // ����� HP ������ �˸���.
    MSG_CG_STATUS_CHANGE_MONSTER_ABILITY msg;
    msg.m_fHPRatio = CalcHPIncreaseRatioAsParty(number_of_players);
    pPlayer->SendPacket(&msg, sizeof(msg));
}

BYTE
ChangeMonsterAbility::GetPlayerNum(GameZone* pZone, BOOLEAN bRefresh)
{
    const DWORD AcceptMask = (1 << eZONETYPE_MISSION)
                           | (1 << eZONETYPE_HUNTING)
                           | (1 << eZONETYPE_CHUNTING)
                           | (1 << eZONETYPE_INSTANCE);

    DWORD zone_type_flag = pZone->GetZoneType();
          zone_type_flag = 1 << zone_type_flag;
    if((AcceptMask & zone_type_flag) == 0)
        return 0; // �� ���̸� Default�̹Ƿ�, 1�� ����� ������ ���̴�.

    WORD number_of_expected_players = 0;
    if(bRefresh == 0)
        number_of_expected_players = pZone->GetNumberOfExpectedPlayers();
    else if(pZone->UpdateNumberOfExpectedPlayers(&number_of_expected_players) == 0)
        return 0;

    //DWORD number_of_expected_players = pZone->GetNumberOfExpectedPlayers();
    //const BOOLEAN assert_cond =
    //    !!FlowControl::FCAssert(SAFE_NUMERIC_TYPECAST(DWORD, number_of_expected_players, BYTE));
    //if(assert_cond == 0)
    //    number_of_expected_players = pZone->GetPlayerNum();

    if((1 << eZONETYPE_CHUNTING) & zone_type_flag)
    {
        CHuntingManager* pHuntingManager = static_cast<GameCHuntingRoom*>(pZone)->GetCHuntingManager();
        if(pHuntingManager == 0)
            return 0; //null�� ��찡 ������... �� �Ǵ� ��...
        if(pHuntingManager->GetCompetitionState() != eCOMPETITION_NONE)
            number_of_expected_players = number_of_expected_players >> 1;
    }

    return BYTE(max(1, number_of_expected_players));
}



