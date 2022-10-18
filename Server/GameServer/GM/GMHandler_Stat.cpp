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


//투명
//사용법 : //투명 켬[끔]
static bool ProcessTransparent(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
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

//무적 켬/끔
static bool ProcessInvincibility(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
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

//속도
//사용법 : //속도 0-5
static bool ProcessSpeed(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // GM이 아니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nLevel = atoi(vecCmd[1].c_str());

    if (!gm_manager->ApplySpeed(pPlayer, nLevel))
        return false;
    else
    {
        GAMELOG->WriteSpeed(pPlayer); //속도GM로그
        return true;
    }
}

//레벨업
static bool ProcessLevelUp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int nPlusLevel = atoi(vecCmd[1].c_str());

    //(lst1024)(080901) 레벨업 최대 이상, 최소 이하 일시 맥스치 값 셋팅으로 변경
    // 이전 루틴 -> nak 리턴
    int nTempLevel = pPlayer->GetLevel() + nPlusLevel;

    // nPlusLevel이 0보다 크다면 CheckMaxLevel 함수를 사용하고 아니면 현재 레벨값 체크하여 0보다 큰지만 체크한다.
    if (nPlusLevel >= 0)
    {
        if (!Generic::CheckMaxLevel(pPlayer, nPlusLevel) || !Generic::CheckMaxLevel(pPlayer, nTempLevel))
        {
            //버그수정:4954, 날짜:2010-01-26, 작성자:문상현, 버그범위:공통 (버그범위:공통은 모든 국가이며 특정 국가인 경우 국가명)
            //버그내용:130 레벨에서 레벨업시 115 되는 버그 수정
            //nPlusLevel = GameOptionParser::Instance()->GetMaxLevel() - pPlayer->GetLevel();
            nPlusLevel = Generic::GetMaxLevel(pPlayer) - pPlayer->GetLevel();
        }
    }
    else // 마이너스 레벨업 이면...
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
//  "//changeup 100"    100레벨 체인지업
struct eCHANGEUP_EVENT_DESC
{
    enum {
        CHANGEUP_NUDE           = 0,        // 이 수치는 초기 캐릭터 상태 (레벨 및 체인지업 상태만)로 전환
        CHANGEUP_100LEVEL       = 100,      // 이 수치는 상징적인 의미로 사용된다.
    };
};

static bool ProcessChangeUpCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
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
                // 바로 이전 단계의 최고랩으로 설정하자.
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

//exp 업 함수 추가, 인벤 삭제 함수 추가
static bool ProcessAddExp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    EXPTYPE Exp = _atoi64(vecCmd[1].c_str()); 

    if (0 == pPlayer->AddExp(Exp))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//스텟포인트 업
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

    // 스텟포인트 업
    if (vecCmd.size() == 2)
    {
        int nPlusPoint = atoi(vecCmd[1].c_str()); 
        pPlayer->StatPointUp(nPlusPoint);

        GAMELOG->WriteStatPointUp(pPlayer, nPlusPoint);
    }
    // 스텟업(힘, 민첩, 체력, 지력, 정신력, 숙련1, 숙련2)
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

        // 해당 스텟을 적용한다.
        int nPlusPoint = atoi(vecCmd[2].c_str()); 
        if (pPlayer->CanSelectStat(attrType))
        {
            DWORD resultValue = pPlayer->SelectStat(attrType, nPlusPoint);

            // 스텟업 정보를 알린다.
            MSG_CG_STATUS_STAT_SELECT_ACK sendMsg;
            sendMsg.m_dwObjKey = pPlayer->GetObjectKey();
            sendMsg.m_AttrType = (BYTE)attrType;
            sendMsg.m_dwValue = resultValue;
            pPlayer->SendPacket(&sendMsg, sizeof(sendMsg));

            // 남은 스텟정보를 알린다.
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

//스킬포인트 업
static bool ProcessSkillPointUp(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
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
// // 스킬레벨 초기화 
// // 클리어스킬
static bool ProcessClearSkillCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
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

//회복
static bool ProcessRecovery(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //(lst1024)(081001)HP회복값 인자 추가
    //인자개수 체크
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

    //Recover함수 내부에 BRD 패킷이 있다.. 따로 보낼 필요 없다.
    pPlayer->OnRecover(nRecoverHP, nRecoverMP, nRecoverSD);
    GAMELOG->WriteRecovery(pPlayer);

    return true;

}

//부활
static bool ProcessRebirth(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 1)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
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

// 자살 명령어 (타살도 필요?)
// 자살
static bool ProcessDieCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    pPlayer->DecreaseHP(pPlayer->GetHP());

    return true;
}

// 스킬 쿨타임 초기화 명령어
// 쿨타임
static bool ProcessInitializeCooltimeCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
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

    // 상태해제
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

    if (vecCmd.size() != 1)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    // 카오상태 초기화
    pPlayer->GetChaoState().UpChaoState();

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//_NA_001231_20081216_ADD_OPTION_KIND
// 옵션변경 133 5 1000 -
static bool ProcessOptionCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
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

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    
    BETAKEY betakey = atof(vecCmd[1].c_str());    

    pPlayer->SetBetaKeyInfo(betakey);

    return true;
}

#endif


//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
// 구현 전용 지역 함수
//namespace util { namespace internal {

// 매개변수 목록을 캐릭터 외모로 변환한다.
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

// 캐릭터 외모를 변경한다.
static bool ProcessChangeCharacterAppearance(DWORD player_key, 
                                             const GmCmdTokens& gm_cmd_arguments)
{
    using namespace util::internal;
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    // 입력값을 검사한다.
    Player* const gm = gm_manager->GetGMPlayer(player_key);
    if (scoped_check_input_in_gm(gm, gm_cmd_arguments, 3, 4, false) == false)
    {
        return false;
    }

    // 아이템을 사용한 외모 변경인지 검사한다.
    const bool is_use_item = (gm_cmd_arguments.size() == 4);
    if (is_use_item)
    {
        // GM 명령어를 요청 메시지로 변환한다.
        MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN change_syn;
        change_syn.m_dwKey = gm->GetUserKey();
        scoped_string_to_number(gm_cmd_arguments[3], change_syn.item_position_);
        scoped_arguments_to_character_appearance(gm_cmd_arguments, change_syn.character_appearance_);

        // 처리기를 호출한다.
        Handler_CG_ITEM::OnCG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN(NULL, &change_syn, change_syn.GetSize());

        return true;
    }

    // 캐릭터 외모를 변경한 후 결과를 응답한다.
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
        GMHANDLER_MAPPING(true, "속도", "speed",  &ProcessSpeed), // 속도
        GMHANDLER_MAPPING(true, "투명", "hide",  &ProcessTransparent), // 투명
        GMHANDLER_MAPPING(true, "무적", "undead",  &ProcessInvincibility), // 무적
        GMHANDLER_MAPPING(true, "레벨업", "levelup",  &ProcessLevelUp), // 레벨업
        GMHANDLER_MAPPING(true, "부활", "rebirth",  &ProcessRebirth), // 부활
        GMHANDLER_MAPPING(true, "회복", "recover",  &ProcessRecovery), // 회복
        GMHANDLER_MAPPING(true, "스텟업", "statup",  &ProcessStatPointUp), // 스텟포인트 업
        GMHANDLER_MAPPING(true, "스킬포인트업", "skillup",  &ProcessSkillPointUp), // 스킬포인트 업
        GMHANDLER_MAPPING(true, "카오증가", "increasechao",  &ProcessIncreaseChaoState),
        GMHANDLER_MAPPING(true, "상태제거", "removestatus",  &ProcessRemoveStatus),
        GMHANDLER_MAPPING(true, "체크스텟", "checkstat",  &ProcessCheckValidStat),
        GMHANDLER_MAPPING(true, "경험치", "exp",  &ProcessAddExp),
        GMHANDLER_MAPPING(true, "체인지업", "changeup",  &ProcessChangeUpCmd),
        GMHANDLER_MAPPING(true, "쿨타임", "ctime",  &ProcessInitializeCooltimeCmd),
        GMHANDLER_MAPPING(true, "자살", "die",  &ProcessDieCmd),
        GMHANDLER_MAPPING(true, "클리어스킬", "clearskill",  &ProcessClearSkillCmd),
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessOptionCmd),
        //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessChangeCharacterAppearance), // 캐릭터 외모 변경
        GMHANDLER_MAPPING(true, "퍽레벨업", "perklevelup",  &ProcessPerkLevelUp),   
    #ifdef _NA_20100307_BETAKEY_SYSTEM
        GMHANDLER_MAPPING(true, "베타키", "betakey",  &ProcessBetaKey),
    #endif

    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}