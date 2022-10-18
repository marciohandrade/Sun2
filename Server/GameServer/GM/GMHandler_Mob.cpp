#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <NPCInfoParser.h>
#include <RewardInfoList.h>
#include <ItemTypeList.h>
//
#include <SCItemSlot.h>
//
#include <DropRatioInfoList.h>
//
#include "GameZone.h"
#include "GameField.h"
#include "MovingPlanner.h"
#include "NPC.h"
#include "TargetManager.h"

#include "DropManager.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

//���� ����
static bool ProcessRemovalMonster(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() < 1)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    int damegePercent = 0;

    if (vecCmd.size() == 2)
        damegePercent = atoi(vecCmd[1].c_str());

    pField->KillAllNPC(pPlayer, 30, damegePercent/100.f);

    GAMELOG->WriteRemovalMonster(pPlayer);

    return true;
}



// Code : SaemSong
// �� ����  
// ĳ���� ������ ��ġ�ص� ���͵��� ������Ų��. (������������ ����)
//static bool ProcessMonsterRegen(Player* pPlayer, GameField* pField)
//{
//  WzVector vPlayerPos;
//
//  if (!pPlayer || !pField)
//      return false;
//
//  pPlayer->GetPos(&vPlayerPos);
//
//  // �ݰ� 15m ���� ���͵��� �����Ѵ�.
//  DestroyMonsterAroundEx DestroyMonsterOpr(vPlayerPos, 15, pPlayer, 1);
//  pField->ForNeighborNPC(pPlayer->GetSectorIndex(), DestroyMonsterOpr);
//
//  //GAMELOG->WriteRemovalMonster(pPlayer);
//
//  return true;
//}
//

//����
//���� : //���� [������Ű]
static bool ProcessRemoval(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    DWORD dwMonsterKey = atoi(vecCmd[1].c_str());

    //2. User Check
    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    NPC* pMonster = (NPC*)pField->FindCharacter(dwMonsterKey);
    if (!pMonster)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_NOTEXIST_USER; // USER�� �������� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    //GameField �� npc �ϳ��� ����� �ɸ��� �����...
    pField->KillNPC(pPlayer, pMonster, false);

    GAMELOG->WriteRemoval(pPlayer, pMonster->GetCharName());

    return true;
}

//ų
static bool ProcessKill(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    int nArgSize = vecCmd.size();
    CHKSendErr(2 > nArgSize,MSG_CG_GM_STRING_CMD_NAK,RC::RC_GM_INVALID_ARGUMENT,pPlayer);

    //2. User Check
    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    DWORD dwMonsterKey = atoi(vecCmd[1].c_str());
    NPC* pMonster = (NPC*)pField->FindCharacter(dwMonsterKey);

    CHKSendErr(!pMonster,MSG_CG_GM_STRING_CMD_NAK,RC::RC_GM_NOTEXIST_USER,pPlayer);

    //GameField �� npc �ϳ��� ����� �ɸ��� �����...
    pField->KillNPC(pPlayer, pMonster, true);

    GAMELOG->WriteKill(pPlayer, pMonster->GetCharName());

    return true;
}


//���� ��ȯ
static bool ProcessCreateMonster(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 3)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GameField* pField = pPlayer->GetField();
    if (NULL == pField)
    {
        // GM�� ���ӳ� �ʵ�ȿ� ���ͼ� ����� �� ��츸 ���͸� �������� ����!!
        return false;
    }

    int nMonsterCode = atoi(vecCmd[1].c_str());
    int nMonsterCount = atoi(vecCmd[2].c_str());
    if (nMonsterCount <= 0)
        return false;

    WzVector vPlayerPos;
    pPlayer->GetPos(&vPlayerPos); 

    // �÷��̾ ��ġ�� Ÿ�������� ������ ��ġ�� ���� ����(20050624)
    int iTileIdx = pPlayer->GetPathExplorer()->GetTile();
    if (-1 == iTileIdx)
    {
        //DISPMSG("[OnCG_GM_MONSTER_CREATE_SYN] Player Position : TileIdx -1 !!!");
        return false;
    }

    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
    vPlayerPos = pMovingPlanner->GetRandomPosInTile(iTileIdx);

    BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo(nMonsterCode);
    if (!pBaseNPCInfo)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    for (int i = 0; i < nMonsterCount; ++i)
    {
        pField->CreateMonster(nMonsterCode, &vPlayerPos);
    }

    GAMELOG->WriteCreateMonster(pPlayer, nMonsterCode, nMonsterCount);

    return true;
}



namespace ns_functor {
;
// (CHANGES) (f100503.3L) change the interface usage
class ActionNPCAroundEx : public IObjectFinder
{
public:
    ActionNPCAroundEx(DWORD npc_object_key,
        const WzVector& origin_pos,
        int range,
        bool operation_gm_idle,
        SLOTCODE skill_code = 0,
        Player* player = NULL) 
        : npc_object_key_(npc_object_key),
        find_mode_(npc_object_key != 0),
        operation_gm_idle_(operation_gm_idle),
        skill_code_(skill_code),
        range_square_(static_cast<float>(range) * range),
        player_(player),
        origin_pos_(origin_pos)
    {
    }
    ~ActionNPCAroundEx(){}

    virtual bool operator()(Object* object) const
    {
        // the return values is false if all npc is targeted in the loop processing
        const bool kReturnValue = false;
        if (npc_object_key_ && npc_object_key_ != object->GetObjectKey()) {
            return false;
        }
        assert(object->IsEqualObjectKind(NPC_OBJECT));
        NPC* npc = static_cast<NPC*>(object);
        // if the find_mode_ is true, the below step is acquired the true return value
        GameField* game_field = npc->GetField();
        if (game_field == NULL) {
            return find_mode_;
        }

        WzVector diff_square = *npc->GetPosPtr();
        ; diff_square -= origin_pos_;
        float distance_square = VectorLength2(&diff_square);
        if (distance_square > range_square_) {
            return find_mode_;
        }
        // GMIdle ���·� ����
        npc->SetGMIdleOn(operation_gm_idle_);
        if (operation_gm_idle_) {
            npc->ChangeState(STATE_ID_IDLE);
        }
        // ���� ��ų���
        if (skill_code_)
        {
            RC::eSKILL_RESULT skill_result = npc->CanUseSkill(skill_code_, false);
            if (skill_result != RC::RC_SKILL_SUCCESS) {
                return find_mode_;
            }

            AI_MSG_USE_SKILL skill_msg_for_ai;
            skill_msg_for_ai.dwSkillCode = skill_code_;
            skill_msg_for_ai.dwTargetKey = player_->GetObjectKey();
            player_->GetPos(&skill_msg_for_ai.vTargetPos);
            // ��ų�� ����ϵ��� AI �޼����� ������.
            npc->SendAIMessage(&skill_msg_for_ai, sizeof(skill_msg_for_ai));
        }
        return find_mode_;
    }

private:
    const DWORD npc_object_key_;
    const bool find_mode_; // else then foreach
    const bool operation_gm_idle_;
    const SLOTCODE skill_code_;
    const float range_square_;
    Player* const player_;
    const WzVector origin_pos_;
};

}; //end of namespace

static bool ProcessMonsterIdle(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    MSG_CG_GM_STRING_CMD_NAK NakMsg;

    BOOL bGMIdle = true; DWORD dwNPCObjKey = 0;
    if (vecCmd.size() == 1)
    {
        // ���;��̵� : ��
        // ������ : �ֺ� NPC ���
    }
    else if (vecCmd.size() == 2)
    {
        // ���;��̵�
        eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
        if (type == GMSTR_TURNOFF)   bGMIdle = false;

        // ������ : �ֺ� NPC ���
    }
    else if (vecCmd.size() == 3)
    {
        // ���;��̵�
        eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
        if (type == GMSTR_TURNOFF)   bGMIdle = false;

        // ������
        dwNPCObjKey = atoi(vecCmd[1].c_str());
    }

    // �ݰ� 30m ���� ���͵��� GMIdle ���¸� �����Ѵ�.
    WzVector vPlayerPos; pPlayer->GetPos(&vPlayerPos);

    // (CHANGES) (f100503.3L) change the interface usage
    ns_functor::ActionNPCAroundEx npc_action_ctrl(dwNPCObjKey, vPlayerPos, 30, bGMIdle != false);
    pField->SearchNeighborNpc(pPlayer->GetSectorIndex(), &npc_action_ctrl);
#if SUN_CODE_BACKUP
    ActionNPCAroundEx ActionNPCOpr(dwNPCObjKey, vPlayerPos, 30, bGMIdle);
    pField->ForNeighborNPC(pPlayer->GetSectorIndex(), ActionNPCOpr);
#endif

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

static bool ProcessMonsterUseSkill(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    MSG_CG_GM_STRING_CMD_NAK NakMsg;

    if (vecCmd.size() != 2 && vecCmd.size() != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bGMIdle = true; DWORD dwNPCObjKey = 0; SLOTCODE SkillCode = 0;
    if (vecCmd.size() == 2)
    {
        // ��ų�ڵ�
        SkillCode = atoi(vecCmd[1].c_str());
    }
    else if (vecCmd.size() == 3)
    {
        // ��ų�ڵ�
        SkillCode = atoi(vecCmd[1].c_str());

        // ������
        dwNPCObjKey = atoi(vecCmd[2].c_str());
    }

    // �ݰ� 30m ���� ���͵��� ��ų���
    WzVector vPlayerPos; pPlayer->GetPos(&vPlayerPos);

    // (CHANGES) (f100503.3L) change the interface usage
    ns_functor::ActionNPCAroundEx npc_action_ctrl(dwNPCObjKey, vPlayerPos, 30, bGMIdle != false,
        SkillCode, pPlayer);
    pField->SearchNeighborNpc(pPlayer->GetSectorIndex(), &npc_action_ctrl);
#if SUN_CODE_BACKUP
    ActionNPCAroundEx ActionNPCOpr(dwNPCObjKey, vPlayerPos, 30, bGMIdle, SkillCode, pPlayer);
    pField->ForNeighborNPC(pPlayer->GetSectorIndex(), ActionNPCOpr);
#endif
    return true;
}

static bool ProcessDisplaySkillResult(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    MSG_CG_GM_STRING_CMD_NAK NakMsg;

    if (vecCmd.size() != 1 && vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    if (vecCmd.size() == 1)
    {
        pPlayer->SetDebugInfoFlag(eDEBUGINFO_TYPE_MONSTERSKILL);
    }
    else
    {
        eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
        if (type == GMSTR_TURNOFF)           
            pPlayer->SetDebugInfoFlag(eDEBUGINFO_TYPE_INVALID);
        else
            pPlayer->SetDebugInfoFlag(eDEBUGINFO_TYPE_MONSTERSKILL);
    }

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}


static bool ProcessMaxDamage(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bWantUndeadOn = false;
    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bWantUndeadOn = true;
    else if (type == GMSTR_TURNOFF)  bWantUndeadOn = false;
    else 
        return false;

    if (!gm_manager->ApplyMaxDamage(pPlayer, bWantUndeadOn))
        return false;

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}
static bool ProcessMaxExp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bWantUndeadOn = false;
    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bWantUndeadOn = true;
    else if (type == GMSTR_TURNOFF)  bWantUndeadOn = false;
    else 
        return false;

    if (!gm_manager->ApplyMaxExp(pPlayer, bWantUndeadOn))
        return false;

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}
static bool ProcessDropAll(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bWantUndeadOn = false;
    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bWantUndeadOn = true;
    else if (type == GMSTR_TURNOFF)  bWantUndeadOn = false;
    else 
        return false;

    if (!gm_manager->ApplyDropListAll(pPlayer, bWantUndeadOn))
        return false;

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//{__NA_1325_GM_CMD_ADD_ABOUT_DROP
static bool ProcessForceDrop(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    };
    int number_of_arguments = (INT)vecCmd.size();
    CHKSendErr(4 != number_of_arguments,
        MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    //0�̸� NPCInfo 1�̸� DropRatioInfo
    int gm_option_drop_type_selection = atoi(vecCmd[1].c_str());
    //Index
    const int npc_code_or_drop_index = atoi(vecCmd[2].c_str());
    //Ƚ��
    const int loop_count = atoi(vecCmd[3].c_str());

    GameField* const game_field = player->GetField();
    const WzVector* drop_base_pos = player->GetPosPtr();

    CHKSendErr(!game_field, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_USER_STATE_INVALID, player);
    CHKSendErr(!drop_base_pos, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_USER_STATE_INVALID, player);

    //���� �ε���
    if (gm_option_drop_type_selection == 0)
    {
        const BASE_NPCINFO* npc_info = NPCInfoParser::Instance()->GetNPCInfo(npc_code_or_drop_index);
        CHKSendErr(!npc_info , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
        for (int i = 0; i < loop_count; ++i)
        {
            g_DropManager.InstantDrop_(game_field, player->GetSectorIndex(),
                drop_base_pos, player,
                0, npc_info,
                (eNPC_GRADE)npc_info->m_byGrade, npc_info->m_DisplayLevel);
        };
        return true;
    }
    else if (gm_option_drop_type_selection == 1)
    {
        // will process in next context.
    }
    else
    {
        CHKSendErr(true , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
        return false; // can't enterable... -.-;
    };
    //
    SCItemSlot dropped_item_slot;
    //
    assert(gm_option_drop_type_selection == 1);
    //Drop�ε���
    for (int i = 0 ; i < loop_count; i++)
    {
        const DropObj* const drop_obj =
            DropRatioInfoList::Instance()->PopRandomItemTypeIndex(npc_code_or_drop_index);
        if (drop_obj == NULL) {
            continue;
        };
        if (drop_obj->GetType() != DROP_OBJ_ITEM)
        {   //���� ������� �ʴ´�.
            continue;
        };

        const DropItemObj* const drop_item_obj = static_cast<const DropItemObj*>(drop_obj);

        SLOTCODE drop_item_code = static_cast<SLOTCODE>(drop_item_obj->GetItemCode());
        if (0 == drop_item_code) {
            continue;
        };
        //
        DWORD drop_item_reserved = drop_item_obj->GetReserved();
        // CHANGES: f110628.5L, fixed the initialization problem, modified since sun:r54509 by jaykang
        dropped_item_slot.Clear();
        dropped_item_slot.SetCode(drop_item_code);
        //
        const nsSlot::ItemRuleInfo& drop_item_rule_info = dropped_item_slot.GetItemRuleInfo();
        if (drop_item_rule_info.is_valid_item == 0) 
        {
            SUNLOG(eCRITICAL_LOG, TEXT("drop_item_code|%u|Try to invalid item drop"), drop_item_code);
            continue;
        };
        const BYTE group_code = drop_item_obj->GetGroupCode();
        // NOTE: this logic don't control 'DROP_GROUP::LIMIT_DISTRIBUTE'
        if (dropped_item_slot.IsOverlap())
        {
            const DURATYPE drop_item_max_num = dropped_item_slot.GetMaxNum();
            //default : GetNum == 1
        //#ifdef _NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES
        //    if (group_code & DROP_GROUP::LIMIT_DISTRIBUTE)
        //    {
        //        // NOTE: f110401.3L, the 'drop_item_reserved' is external controlled.
        //        // TODO: changes generally drop processing.
        //        dropped_item_slot.SetNum(1);
        //        *result_number_of_drop_items = drop_item_reserved;
        //    }
        //    else
        //#endif
            {
                if (drop_item_max_num < drop_item_reserved)
                {
                    dropped_item_slot.SetNum(drop_item_max_num);
                }
                else if (drop_item_reserved != 0)
                {
                    assert(SAFE_NUMERIC_TYPECAST(DWORD, drop_item_reserved, DURATYPE));
                    dropped_item_slot.SetNum(static_cast<DURATYPE>(drop_item_reserved));
                }
            };

            WzVector dropped_pos;
            if (game_field->GetRandomPosInAround(drop_base_pos, 3, dropped_pos) == false) {
                dropped_pos = *drop_base_pos;
            };
            g_DropManager.DropItemToField(game_field, &dropped_pos,
                player->GetObjectKey(), 0, dropped_item_slot);

            continue;
        };
        //------------------------------------------------------------------------------------------
        // added rule to support _NA_0_20110318_ITEM_EXPIRATION_PERIOD
        const bool apply_time =
            ((group_code & (DROP_GROUP::LIMIT_DATE|DROP_GROUP::LIMIT_TIME)) != 0);
        if (apply_time)
        {
            if (dropped_item_slot.ChangeItemToUsingTimer() == false)
            {
                SUNLOG(eCRITICAL_LOG, _T("ItemCode|%u|failed apply timer to drop item"),
                       drop_item_code);
                return false;
            };

            switch (drop_item_rule_info.charge_sub_type)
            {
            case eCHARGE_SUB_FIXED_AMOUNT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX:
            case eCHARGE_SUB_FIXED_QUANTITY:
            case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
            case eCHARGE_SUB_FATE:
            case eCHARGE_SUB_COORDINATE_EX:
            case eCHARGE_SUB_RIDER_WITH_PART:
            case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
            case eCHARGE_SUB_FIXED_AMOUNT_EX2:
                if (group_code & DROP_GROUP::LIMIT_TIME)
                {
                    // �д��� ����
                    dropped_item_slot.SetDateTime(drop_item_reserved * 60);
                }
                else if (group_code & DROP_GROUP::LIMIT_DATE)
                {
                    //��¥ YYYYMMDDHHMMSS ���� ���߱� ���� 1000000�� ���Ѵ�.
                    DWORD64 d = (DWORD64)drop_item_reserved*1000000;
                    dropped_item_slot.SetFixedDateTime(d);
                };
                // CHANGES: f110603.4L, set default value about drop_item_reserved
                drop_item_reserved = 0;
                break;
            }
        }; // apply time
        //------------------------------------------------------------------------------------------
        if (drop_item_rule_info.is_weapon || drop_item_rule_info.is_armor) 
        {
        }
        else
        {
        };
        //------------------------------------------------------------------------------------------
        const ItemType* drop_item_type_ptr = (drop_item_reserved != 0) ?
            ItemTypeList::Instance()->FindItemType(drop_item_reserved) : NULL;
        if (drop_item_type_ptr != NULL)
        {
            if (drop_item_rule_info.IsEnableEnchant())
            {
                const BYTE enchant = drop_item_type_ptr->GetEnchant();
                dropped_item_slot.SetEnchant(enchant);
            }            
        #ifdef _NA_000251_20100727_SOCKET_SYSTEM
            // EP2������ ������ Ÿ�� ������ ��ũ �׸��� ���� ������ ����Ѵ�.
            const BYTE number_of_socket = drop_item_type_ptr->GetRank();
            if (number_of_socket != 0 && drop_item_rule_info.IsEnableSocket())
            {
                if (dropped_item_slot.CreateSocketItem(number_of_socket, false) == false)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|���� ���� ������ �����߽��ϴ�.|�������ڵ� = %u, ���ϰ��� = &u"), 
                           __FUNCTION__, 
                           drop_item_code, 
                           number_of_socket);
                }
            };
        #else
            // (CHANGES) (WAVERIX) itemslot���� ����Ʈ ���� ���ŵǾ���.
            const BASE_ITEMINFO* drop_item_info_ptr = dropped_item_slot.GetItemInfo();
            assert(drop_item_info_ptr != NULL);
            if (drop_item_info_ptr->IsElite() == 0 && drop_item_rule_info.IsEnableRank())
            {
                const eRANK item_rank = static_cast<eRANK>(drop_item_type_ptr->GetRank());
                dropped_item_slot.SetRank(item_rank);
            };
        #endif // _NA_000251_20100727_SOCKET_SYSTEM
            
            nsSlot::ItemTypeChanger::ChangeItemByType(&dropped_item_slot,
                                                      drop_item_type_ptr->GetItemType());
        }

        WzVector dropped_pos;
        if (!game_field->GetRandomPosInAround(drop_base_pos, 3, dropped_pos)) {
            dropped_pos = *drop_base_pos;
        };
        g_DropManager.DropItemToField(game_field, &dropped_pos,
            player->GetObjectKey(), 0, dropped_item_slot);
    }

    return true;
}
//}__NA_1325_GM_CMD_ADD_ABOUT_DROP

#ifdef _NA_0_20100819_AGGRO_RENEWAL
static bool ProcessAggroDisplay(const DWORD player_key, 
                                const GmCmdTokens& gm_cmd_arguments)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const pPlayer = gm_manager->GetGMPlayer(player_key);
    if (pPlayer == 0)
        return false;

    INT nArgSize = (INT)gm_cmd_arguments.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer); 

    DWORD object_key = (DWORD)atoi(gm_cmd_arguments[1].c_str());

    GameField* const field = pPlayer->GetField();
    if (!field)
        return false;

    Character* const character = field->FindCharacter(object_key);
    if (!character)
        return false;

    if (!character->IsEqualObjectKind(MONSTER_OBJECT))
        return false;

    NPC* const target_monster = dynamic_cast<NPC*>(character);
    if (!target_monster)
        return false;

    TargetManager* const target_manager = target_monster->GetTargetManager();

    if (!target_manager)
        return false;

    target_manager->AddAggroDisplayUser(pPlayer->GetObjectKey());


    return true;
}
#endif //_NA_0_20100819_AGGRO_RENEWAL

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterMob(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "��������", "removem",  &ProcessRemovalMonster), // ��������
        GMHANDLER_MAPPING(true, "����", "remove",  &ProcessRemoval), // ����
        GMHANDLER_MAPPING(true, "ų", "kill",  &ProcessKill), // ų
        GMHANDLER_MAPPING(true, "���ͼ�ȯ", "summon",  &ProcessCreateMonster), // ���� ��ȯ
        GMHANDLER_MAPPING(true, "���;��̵�", "monsteridle",  &ProcessMonsterIdle),
        GMHANDLER_MAPPING(true, "���ͽ�ų�ߵ�", "monsteruseskill",  &ProcessMonsterUseSkill),
        GMHANDLER_MAPPING(true, "���ͽ�ų���", "displayskillresult",  &ProcessDisplaySkillResult),
        GMHANDLER_MAPPING(true, "�ִ뵥����", "maxd",  &ProcessMaxDamage),
        GMHANDLER_MAPPING(true, "�ִ����ġ", "maxe",  &ProcessMaxExp),
        GMHANDLER_MAPPING(true, "�������Ʈ", "droplist",  &ProcessDropAll),
        GMHANDLER_MAPPING(true, "���", "drop",  &ProcessForceDrop),
#ifdef _NA_0_20100819_AGGRO_RENEWAL
        GMHANDLER_MAPPING(true, "��׷�ǥ��", "aggrodisplay",  &ProcessAggroDisplay),
#endif
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
