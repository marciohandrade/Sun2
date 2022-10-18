#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <SkillAttrCalculator.h>
//
#include "CharacterFormula.h"
#include "ItemManager.h"
#include "Status/StatusManager.h"
#include "SkillSystem/PerkManager.h"

#include "ChaoState.h"

#include "PacketHandler/Handler_CG_ITEM.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;


//����
//���� : //���� ��[��]
static bool ProcessTransparent(DWORD player_key, const GmCmdTokens& vecCmd)
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

    BOOL bWantObserverOn = false;

    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bWantObserverOn = true;
    else if (type == GMSTR_TURNOFF)  bWantObserverOn = false;
    else 
        return false;

    if (!gm_manager->ApplyTransparent(pPlayer, bWantObserverOn))
        return false;
    else
    {
        GAMELOG->WriteTransparent(pPlayer);
        return true;
    }
}

//���� ��/��
static bool ProcessInvincibility(DWORD player_key, const GmCmdTokens& vecCmd)
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

    if (!gm_manager->ApplyInvincibilty(pPlayer, bWantUndeadOn))
        return false;
    else
    {
        GAMELOG->WriteInvincibility(pPlayer);
        return true;
    }
}

//�ӵ�
//���� : //�ӵ� 0-5
static bool ProcessSpeed(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // GM�� �ƴϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nLevel = atoi(vecCmd[1].c_str());

    if (!gm_manager->ApplySpeed(pPlayer, nLevel))
        return false;
    else
    {
        GAMELOG->WriteSpeed(pPlayer); //�ӵ�GM�α�
        return true;
    }
}

//������
static bool ProcessLevelUp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nPlusLevel = atoi(vecCmd[1].c_str());

    //(lst1024)(080901) ������ �ִ� �̻�, �ּ� ���� �Ͻ� �ƽ�ġ �� �������� ����
    // ���� ��ƾ -> nak ����
    int nTempLevel = pPlayer->GetLevel() + nPlusLevel;

    // nPlusLevel�� 0���� ũ�ٸ� CheckMaxLevel �Լ��� ����ϰ� �ƴϸ� ���� ������ üũ�Ͽ� 0���� ū���� üũ�Ѵ�.
    if (nPlusLevel >= 0)
    {
        if (!Generic::CheckMaxLevel(pPlayer, nPlusLevel) || !Generic::CheckMaxLevel(pPlayer, nTempLevel))
        {
            //���׼���:4954, ��¥:2010-01-26, �ۼ���:������, ���׹���:���� (���׹���:������ ��� �����̸� Ư�� ������ ��� ������)
            //���׳���:130 �������� �������� 115 �Ǵ� ���� ����
            //nPlusLevel = GameOptionParser::Instance()->GetMaxLevel() - pPlayer->GetLevel();
            nPlusLevel = Generic::GetMaxLevel(pPlayer) - pPlayer->GetLevel();
        }
    }
    else // ���̳ʽ� ������ �̸�...
    {
        if (nTempLevel <= 0)
            nPlusLevel = -pPlayer->GetLevel()+1;
    }


    LEVELTYPE OldLevel = pPlayer->GetLevel();

    if (nPlusLevel >= 0)
        pPlayer->LevelUp((DWORD)nPlusLevel);
    else
        pPlayer->LevelDown(nPlusLevel); 
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    if (ItemManager* item_manager = pPlayer->GetItemManager()) {
        item_manager->UpdateItemAttributes(true, false);
    };
#endif

    GAMELOG->WriteLevelUp(pPlayer, OldLevel);

    return true;
}

//////////////////////////////////////////////////////////////////////////
//  "//changeup 100"    100���� ü������
struct eCHANGEUP_EVENT_DESC
{
    enum {
        CHANGEUP_NUDE           = 0,        // �� ��ġ�� �ʱ� ĳ���� ���� (���� �� ü������ ���¸�)�� ��ȯ
        CHANGEUP_100LEVEL       = 100,      // �� ��ġ�� ��¡���� �ǹ̷� ���ȴ�.
    };
};

static bool ProcessChangeUpCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    const INT nArgSize = (INT)vecCmd.size();

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(nArgSize == 2, RC::RC_GM_INVALID_ARGUMENT);

        const INT ced_mark = atoi(vecCmd[1].c_str());

        LEVELTYPE changeupLevel = 0;
        BASE_PLAYERINFO& rPlayerInfo = *pPlayer->GetCharInfo();
        PROPERTY<BYTE>& rChangeOfClass = rPlayerInfo.m_ChangeOfClassStep;
        const eCHAR_TYPE charType = pPlayer->GetCharType();

        switch(ced_mark)
        {
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        case eCHANGEUP_EVENT_DESC::CHANGEUP_NUDE:
            // the Episode II version basically configured changeup status.
    #endif
        case eCHANGEUP_EVENT_DESC::CHANGEUP_100LEVEL:
            break;
        default:
            SetFailFc("invalid event value", RC::RC_GM_INVALID_ARGUMENT);
            break;
        }

        if (flow.IsErrorOccured())
            break;

        switch(ced_mark)
        {
        case eCHANGEUP_EVENT_DESC::CHANGEUP_NUDE:
            {
                changeupLevel = 1;
                rChangeOfClass.Set((BYTE)charType);
            }
            break;
        case eCHANGEUP_EVENT_DESC::CHANGEUP_100LEVEL:
            {
                // �ٷ� ���� �ܰ��� �ְ����� ��������.
                changeupLevel = (LEVELTYPE)GameOptionParser::Instance()->GetMaxLevel();
                //changeupLevel = GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st();
                switch(charType)
                {
                case eCHAR_BERSERKER:       rChangeOfClass.Set((BYTE)eCHAREX_BERSERKER_100LEVEL); break;
                case eCHAR_DRAGON:          rChangeOfClass.Set((BYTE)eCHAREX_DRAGON_KNIGHT_100LEVEL); break;
                case eCHAR_SHADOW:          rChangeOfClass.Set((BYTE)eCHAREX_SHADOW_100LEVEL); break;
                case eCHAR_VALKYRIE:        rChangeOfClass.Set((BYTE)eCHAREX_VALKYRIE_100LEVEL); break;
                case eCHAR_ELEMENTALIST:    rChangeOfClass.Set((BYTE)eCHAREX_ELEMENTALIST_100LEVEL); break;
                case eCHAR_MYSTIC:          rChangeOfClass.Set((BYTE)eCHAREX_MYSTIC_100LEVEL); break;
                case eCHAR_HELLROID:        rChangeOfClass.Set((BYTE)eCHAREX_HELLROID_100LEVEL); break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                case eCHAR_WITCHBLADE:      rChangeOfClass.Set((BYTE)eCHAREX_WITCHBLADE_100LEVEL); break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                };
#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
                if (pPlayer->GetItemManager()->IsUnEquipAll())
                    pPlayer->InitAllStatByChangeUp();
#endif
            }
            break;
        }

        {
            eSPECIAL_RENDER_EFFECT effect = eSPECIAL_RENDER_EFFECT_NONE;
            ns_object::SPECIAL_RENDER_EFFECT& rRENDER_EFFECT = pPlayer->RENDER_EFFECT;

            // ChangeUp Field Clear
            rRENDER_EFFECT.RemoveEffect(eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st);

            // Reconfiguration
            switch(rChangeOfClass)
            {
            case eCHAREX_BERSERKER_100LEVEL:
            case eCHAREX_DRAGON_KNIGHT_100LEVEL:
            case eCHAREX_SHADOW_100LEVEL:
            case eCHAREX_VALKYRIE_100LEVEL:
            case eCHAREX_ELEMENTALIST_100LEVEL:
            case eCHAREX_MYSTIC_100LEVEL:
            case eCHAREX_HELLROID_100LEVEL:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case eCHAREX_WITCHBLADE_100LEVEL:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

                effect = eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st;
                break;
            }
            rRENDER_EFFECT.AddEffect(effect);
        };

        const LEVELTYPE curLevel = pPlayer->GetLevel();
        const LEVELTYPE largerLevel = max(curLevel, changeupLevel);
        const LEVELTYPE lessLevel = min(curLevel, changeupLevel);

        const INT plus_minus_level =
            curLevel == lessLevel ? /*UpTo*/    (INT)largerLevel - lessLevel
            :                       /*DownTo*/  (INT)lessLevel - largerLevel;

        if (plus_minus_level)
        {
            char levelup_gm_string[20];
            _snprintf(levelup_gm_string, _countof(levelup_gm_string),
                      "levelup %d", plus_minus_level);
            levelup_gm_string[_countof(levelup_gm_string) - 1] = '\0';
            IfFailThenBreak(gm_manager->ParseCommand(pPlayer->GetUserGuid(), levelup_gm_string),
                            RC::RC_GM_INVALID_ARGUMENT);
        }

        {   // ChangeUp Notification Packet
            MSG_CG_QUEST_CHANGEofCLASS_CMD msgCMD;
            msgCMD.m_ChangeOfClassCode = rChangeOfClass;
            pPlayer->SendPacket(&msgCMD, sizeof(msgCMD));
        };

        return true;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    MSG_CG_GM_STRING_CMD_NAK NakMsg;
    NakMsg.m_byErrorCode = (BYTE)flow.GetErrorCode();
    pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
    return false;
}

//_NA002217_100728_EP2_PERK_SYSTEM_
//perklevelup [number]
static bool ProcessPerkLevelUp(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    return false;
#endif

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }

    const size_t number_of_args = gm_cmd_arguments.size();
    bool success = true;
    if (number_of_args != 2) {
        success = false;
    }
    //
    if (success)
    {
        int changed_level = atoi(gm_cmd_arguments[1].c_str());
        nsSkill::PerkManager* const perk_manager = player->GetPerkManager();
        perk_manager->ChangePerkLevel(changed_level);
    }

    if (success == false)
    {
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    return true;
#endif
}

//exp �� �Լ� �߰�, �κ� ���� �Լ� �߰�
static bool ProcessAddExp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    EXPTYPE Exp = _atoi64(vecCmd[1].c_str()); 

    if (0 == pPlayer->AddExp(Exp))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//��������Ʈ ��
static bool ProcessStatPointUp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2 && vecCmd.size() != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // ��������Ʈ ��
    if (vecCmd.size() == 2)
    {
        int nPlusPoint = atoi(vecCmd[1].c_str()); 
        pPlayer->StatPointUp(nPlusPoint);

        GAMELOG->WriteStatPointUp(pPlayer, nPlusPoint);
    }
    // ���ݾ�(��, ��ø, ü��, ����, ���ŷ�, ����1, ����2)
    else
    {
        eATTR_TYPE attrType = gm_manager->strStatToEnum(vecCmd[1].c_str());
        if (eATTR_TYPE_INVALID == attrType)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
            pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
            return false; 
        }

        // �ش� ������ �����Ѵ�.
        int nPlusPoint = atoi(vecCmd[2].c_str()); 
        if (pPlayer->CanSelectStat(attrType))
        {
            DWORD resultValue = pPlayer->SelectStat(attrType, nPlusPoint);

            // ���ݾ� ������ �˸���.
            MSG_CG_STATUS_STAT_SELECT_ACK sendMsg;
            sendMsg.m_dwObjKey = pPlayer->GetObjectKey();
            sendMsg.m_AttrType = (BYTE)attrType;
            sendMsg.m_dwValue = resultValue;
            pPlayer->SendPacket(&sendMsg, sizeof(sendMsg));

            // ���� ���������� �˸���.
            MSG_CG_GM_STAT_UP_ACK send2Msg;
            send2Msg.m_byCategory = CG_GM;
            send2Msg.m_byProtocol = CG_GM_STAT_UP_ACK;
            send2Msg.m_dwRemainStat = pPlayer->GetRemainStatPoint();
            pPlayer->SendPacket(&send2Msg, sizeof(send2Msg));

            GAMELOG->WriteUseStat(pPlayer, (BYTE)attrType);
        }
        else {
            return false;
        }
    }

    return true;

}

//��ų����Ʈ ��
static bool ProcessSkillPointUp(DWORD player_key, const GmCmdTokens& vecCmd)
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

    int nPlusPoint = atoi(vecCmd[1].c_str()); 

    pPlayer->SkillPointUp(nPlusPoint);

    GAMELOG->WriteSkillPointUp(pPlayer, nPlusPoint);

    return true;
}

static bool ProcessCheckValidStat(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (!pPlayer->GetCheckedStat())
    {
        if (!pPlayer->CheckValidStatPoint())
        {
            pPlayer->RecoveryStatPoint();
        }
    }
    return true;
}

// 
// // ��ų���� �ʱ�ȭ 
// // Ŭ���ų
static bool ProcessClearSkillCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    WORD rDecrease = 1;
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    pPlayer->InitStat(eItemWASTE_INIT_EXPERTY_ALL, rDecrease);
#else
    pPlayer->InitStat(eItemWASTE_INIT_EXPERTY_ALL, rDecrease);
    pPlayer->InitStat(eItemWASTE_INIT_PASSIVE_ALL, rDecrease);
#endif

    return true;
}

//ȸ��
static bool ProcessRecovery(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //(lst1024)(081001)HPȸ���� ���� �߰�
    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    INT nRecoverHP = pPlayer->GetMaxHP();
    INT nRecoverMP = pPlayer->GetMaxMP();
    INT nRecoverSD = pPlayer->GetMaxSD();

    if (nArgSize > 1)        
        nRecoverHP = atoi(vecCmd[1].c_str());

    if (nArgSize > 2)        
        nRecoverMP = atoi(vecCmd[2].c_str());

    if (nArgSize > 3)        
        nRecoverSD = atoi(vecCmd[3].c_str());

    //Recover�Լ� ���ο� BRD ��Ŷ�� �ִ�.. ���� ���� �ʿ� ����.
    pPlayer->OnRecover(nRecoverHP, nRecoverMP, nRecoverSD);
    GAMELOG->WriteRecovery(pPlayer);

    return true;

}

//��Ȱ
static bool ProcessRebirth(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    if (!pPlayer->OnResurrection(0, 1.0f, 1.0f))
    {
        return false;
    }

    GAMELOG->WriteRebirth(pPlayer);

    return true;
}

// �ڻ� ��ɾ� (Ÿ�쵵 �ʿ�?)
// �ڻ�
static bool ProcessDieCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    pPlayer->DecreaseHP(pPlayer->GetHP());

    return true;
}

// ��ų ��Ÿ�� �ʱ�ȭ ��ɾ�
// ��Ÿ��
static bool ProcessInitializeCooltimeCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    pPlayer->InitializeSkillCoolTime();
    pPlayer->InitializeItemCooltime();

    return true;
}

static bool ProcessRemoveStatus(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    int StatusCode = atoi(vecCmd[1].c_str());

    // ��������
    if (!pPlayer->GetStatusManager()->Remove((eCHAR_STATE_TYPE)StatusCode))
    {
        MSG_CG_STATUS_REMOVE_BRD BrdMsg;
        BrdMsg.m_dwTargetKey = pPlayer->GetObjectKey();
        BrdMsg.m_dwStatusCode = StatusCode;
        pPlayer->SendPacketAround(&BrdMsg, BrdMsg.GetSize());
    }

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}   

static bool ProcessIncreaseChaoState(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    // ī������ �ʱ�ȭ
    pPlayer->GetChaoState().UpChaoState();

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//_NA_001231_20081216_ADD_OPTION_KIND
// �ɼǺ��� 133 5 1000 -
static bool ProcessOptionCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(4 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//

    SkillAttrCalculator SkillCalc(pPlayer->GetAttr());

    eATTR_TYPE eAttrType = (eATTR_TYPE)atoi(vecCmd[1].c_str());
    int iValueType      = atoi(vecCmd[2].c_str());
    int iValue          = atoi(vecCmd[3].c_str());
    bool bOperationPlus = true;


    if (nArgSize == 5 && strcmp(vecCmd[4].c_str(),"-")==0)
        bOperationPlus = false;

    if (bOperationPlus)
        SkillCalc.AddAttr(eAttrType, iValueType, iValue);
    else
        SkillCalc.DelAttr(eAttrType, iValueType, iValue);

    return true;
}

#ifdef _NA_20100307_BETAKEY_SYSTEM

static bool ProcessBetaKey(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    
    BETAKEY betakey = atof(vecCmd[1].c_str());    

    pPlayer->SetBetaKeyInfo(betakey);

    return true;
}

#endif


//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
// ���� ���� ���� �Լ�
//namespace util { namespace internal {

// �Ű����� ����� ĳ���� �ܸ�� ��ȯ�Ѵ�.
inline void scoped_arguments_to_character_appearance(const GmCmdTokens& cmd_args, 
                                                     CharacterAppearanceInfo& OUT appearance)
{
    using namespace util::internal;
    typedef ChangeCharacterAppearanceTypeConst ChangeConst;
    scoped_string_to_number(cmd_args[1], appearance.appearance_type_);

    if (ChangeConst::IsChangeFace(appearance.appearance_type_))
    {
        scoped_string_to_number(cmd_args[2], appearance.face_);
    }
    else if (ChangeConst::IsChangeHair(appearance.appearance_type_))
    {
        scoped_string_to_number(cmd_args[2], appearance.hair_);
    }
    else if (ChangeConst::IsChangeHeight(appearance.appearance_type_))
    {
        scoped_string_to_number(cmd_args[2], appearance.height_);
    }
#ifdef _NA_002050_20110216_ADD_GENDER
    else if (ChangeConst::IsChangeGender(appearance.appearance_type_))
    {
        scoped_string_to_number(cmd_args[2], appearance.gender);
    }
#endif
}

//}}; // namespace util

// ĳ���� �ܸ� �����Ѵ�.
static bool ProcessChangeCharacterAppearance(DWORD player_key, 
                                             const GmCmdTokens& gm_cmd_arguments)
{
    using namespace util::internal;
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    // �Է°��� �˻��Ѵ�.
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (scoped_check_input_in_gm(gm, gm_cmd_arguments, 3, 4, false) == false)
    {
        return false;
    }

    // �������� ����� �ܸ� �������� �˻��Ѵ�.
    const bool is_use_item = (gm_cmd_arguments.size() == 4);
    if (is_use_item)
    {
        // GM ��ɾ ��û �޽����� ��ȯ�Ѵ�.
        MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN change_syn;
        change_syn.m_dwKey = gm->GetUserKey();
        scoped_string_to_number(gm_cmd_arguments[3], change_syn.item_position_);
        scoped_arguments_to_character_appearance(gm_cmd_arguments, change_syn.character_appearance_);

        // ó���⸦ ȣ���Ѵ�.
        Handler_CG_ITEM::OnCG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN(NULL, &change_syn, change_syn.GetSize());

        return true;
    }

    // ĳ���� �ܸ� ������ �� ����� �����Ѵ�.
    MSG_CG_CHARINFO_CHANGE_APPEARANCE_BRD change_brd;
    {
        change_brd.player_key_ = gm->GetObjectKey();
        scoped_arguments_to_character_appearance(gm_cmd_arguments, change_brd.character_appearance_);
        change_brd.result_ = RC::ChangeCharacterAppearanceResultUtil::EnumToValue(
            gm->ChangeCharacterAppearance(change_brd.character_appearance_));
        gm->SendPacketAround(&change_brd, change_brd.GetSize());
    }

    return true;
}

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterStat(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "�ӵ�", "speed",  &ProcessSpeed), // �ӵ�
        GMHANDLER_MAPPING(true, "����", "hide",  &ProcessTransparent), // ����
        GMHANDLER_MAPPING(true, "����", "undead",  &ProcessInvincibility), // ����
        GMHANDLER_MAPPING(true, "������", "levelup",  &ProcessLevelUp), // ������
        GMHANDLER_MAPPING(true, "��Ȱ", "rebirth",  &ProcessRebirth), // ��Ȱ
        GMHANDLER_MAPPING(true, "ȸ��", "recover",  &ProcessRecovery), // ȸ��
        GMHANDLER_MAPPING(true, "���ݾ�", "statup",  &ProcessStatPointUp), // ��������Ʈ ��
        GMHANDLER_MAPPING(true, "��ų����Ʈ��", "skillup",  &ProcessSkillPointUp), // ��ų����Ʈ ��
        GMHANDLER_MAPPING(true, "ī������", "increasechao",  &ProcessIncreaseChaoState),
        GMHANDLER_MAPPING(true, "��������", "removestatus",  &ProcessRemoveStatus),
        GMHANDLER_MAPPING(true, "üũ����", "checkstat",  &ProcessCheckValidStat),
        GMHANDLER_MAPPING(true, "����ġ", "exp",  &ProcessAddExp),
        GMHANDLER_MAPPING(true, "ü������", "changeup",  &ProcessChangeUpCmd),
        GMHANDLER_MAPPING(true, "��Ÿ��", "ctime",  &ProcessInitializeCooltimeCmd),
        GMHANDLER_MAPPING(true, "�ڻ�", "die",  &ProcessDieCmd),
        GMHANDLER_MAPPING(true, "Ŭ���ų", "clearskill",  &ProcessClearSkillCmd),
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessOptionCmd),
        //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessChangeCharacterAppearance), // ĳ���� �ܸ� ����
        GMHANDLER_MAPPING(true, "�ܷ�����", "perklevelup",  &ProcessPerkLevelUp),   
    #ifdef _NA_20100307_BETAKEY_SYSTEM
        GMHANDLER_MAPPING(true, "��ŸŰ", "betakey",  &ProcessBetaKey),
    #endif

    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}