#include "stdafx.h"
#include "GMCmdManager.h"
//
// @history
//  CHANGES: f110103.4L, added file. this file is used to modify various formula constant values.
#include "CharacterFormula.h"
// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool ProcessFormulaList(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    return false;
#endif
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
    int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
        "List = { damage_formula, critical_formula }");
    msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
    msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
    player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    return true;
#endif
};
// query command
// '//damage_formula'
// '//데미지공식'
// change command
// '//damage_formula [C1] [C2] [C3.1] [C3.2]'
// '//데미지공식 [C1] [C2] [C3.1] [C3.2]'
// C1 = 공격자 레벨 가중치
// C2 = 공격/방어자 간의 레벨 차이
// C3.1 = PvP 데미지 차감률
// C3.2 = PvE 데미지 차감률
static bool ProcessDamageFormula(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    return false;
#endif

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    bool processed = false;
    const int number_of_arguments = cmd_tokens.size();
    if (number_of_arguments == 5)
    {
        processed = true;
        ep2::DamageBalanceConstant constants = { 0, };
        constants.attacker_level_weight = atof(cmd_tokens[1].c_str());
        constants.diff_level_weight = atof(cmd_tokens[2].c_str());
        constants.damage_deduction_pvp_weight = atof(cmd_tokens[3].c_str());
        constants.damage_deduction_pve_weight = atof(cmd_tokens[4].c_str());
        ep2::ModifyDamageBalanceConstant(constants);
    }
    // query command
    if (number_of_arguments == 1 || number_of_arguments == 5)
    {
        processed = true;
        ep2::DamageBalanceConstant constants;
        ep2::GetCurrentDamageBalanceConstant(&constants);
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
                  "AttackerWeight = %.4f, DiffLevelWeight = %.4f, "
                  "PvPdeduction = %.4f, PvEdeduction = %.4f",
                  constants.attacker_level_weight,
                  constants.diff_level_weight,
                  constants.damage_deduction_pvp_weight,
                  constants.damage_deduction_pve_weight);
        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        return true;
    }
    //
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    if (processed == false)
    {
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
            "Usage = "
            "\"//damage_formula [AttackerWeight] [DiffLevelWeight] "
                               "[PvPdeduction] [PvEdeduction]\"");
        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    }
    return false;
#endif
};

// query command
// '//critical_formula'
// '//크리티컬공식'
// change command
// '//critical_formula [C1] [C2] [C3] [C4] [C5]'
// '//크리티컬공식 [C1] [C2] [C3] [C4] [C5]'
// C1 = 버서커
// C2 = 드래곤나이트
// C3 = 셰도우
// C4 = 발키리
// C5 = 엘리멘탈리스트
static bool ProcessCriticalFormula(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    return false;
#endif

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    bool processed = false;
    const int number_of_arguments = cmd_tokens.size();
    if (number_of_arguments == eCHAR_TYPE_MAX)
    {
        processed = true;
        ns_formula::CriticalBalanceConstants constants = { 0, };
        constants.kNumberatorWeight = -1.0f;
        constants.kDenominatorWeight = -1.0f;
        constants.base_probabilies[eCHAR_BERSERKER] = atof(cmd_tokens[1].c_str());
        constants.base_probabilies[eCHAR_DRAGON] = atof(cmd_tokens[2].c_str());
        constants.base_probabilies[eCHAR_SHADOW] = atof(cmd_tokens[3].c_str());
        constants.base_probabilies[eCHAR_VALKYRIE] = atof(cmd_tokens[4].c_str());
        constants.base_probabilies[eCHAR_ELEMENTALIST] = atof(cmd_tokens[5].c_str());
        // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        constants.base_probabilies[eCHAR_MYSTIC] = atof(cmd_tokens[6].c_str());
        //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        constants.base_probabilies[eCHAR_HELLROID] = atof(cmd_tokens[7].c_str());
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        constants.base_probabilies[eCHAR_WITCHBLADE] = atof(cmd_tokens[8].c_str());
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        ns_formula::BalanceConstantsChangerInProgramCommon::ModifyCriticalChance(constants);
    }
    // query command
    if (number_of_arguments == 1 || number_of_arguments == eCHAR_TYPE_MAX)
    {
        processed = true;
        ns_formula::CriticalBalanceConstants constants;
        ns_formula::BalanceConstantsChangerInProgramCommon::GetCriticalChance(&constants);
        //
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
            "Berserker=(%.2f/100), DragonKnight=(%.2f/100), Shadow=(%.2f/100),"
            "Valkyrie=(%.2f/100), Elementalist=(%.2f/100), Mystic=(%.2f/100)",
            constants.base_probabilies[eCHAR_BERSERKER],
            constants.base_probabilies[eCHAR_DRAGON],
            constants.base_probabilies[eCHAR_SHADOW],
            constants.base_probabilies[eCHAR_VALKYRIE],
            constants.base_probabilies[eCHAR_ELEMENTALIST], 
            constants.base_probabilies[eCHAR_MYSTIC]);
                  
        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        return true;
    }
    //
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    if (processed == false)
    {
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
            "Usage = "
            "\"//critical_formula [Berserker:C1] [DragonKnight:C2] "
            " [Shadow:C3] [Valkyrie:C4] [Elementalist:C5] [Mystic:C6]\"");
        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    }
    return false;
#endif
};

// query command
// '//element'
// '//원소'
// change command
// '//element [C1] [C2]'
// '//원소 [C1] [C2]'
// [C1] : '(불/물/바람/대지/암흑)공격' or '(불/물/바람/대지/암흑)저항'
// [C2] : 현 기준 변경할 수치
struct ElementMatchNode
{
    uint8_t is_defense;
    ulong key;
    eATTR_TYPE attr_type;
};

static bool ProcessChangeElements(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    return false;
#endif

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    static const ElementMatchNode elements_matches[10 * 2] =
    {
        { 0, util::StreamHashKey::GetStringHashKey("불공격"), eATTR_MAGICAL_FIRE_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("불저항"), eATTR_DEL_FIRE_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("물공격"), eATTR_MAGICAL_WATER_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("물저항"), eATTR_DEL_WATER_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("바람공격"), eATTR_MAGICAL_WIND_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("바람저항"), eATTR_DEL_WIND_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("대지공격"), eATTR_MAGICAL_EARTH_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("대지저항"), eATTR_DEL_EARTH_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("암흑공격"), eATTR_MAGICAL_DARKNESS_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("암흑저항"), eATTR_DEL_DARKNESS_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("fireattack"), eATTR_MAGICAL_FIRE_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("fireresist"), eATTR_DEL_FIRE_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("waterattack"), eATTR_MAGICAL_WATER_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("waterresist"), eATTR_DEL_WATER_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("windattack"), eATTR_MAGICAL_WIND_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("windresist"), eATTR_DEL_WIND_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("earthattack"), eATTR_MAGICAL_EARTH_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("earthresist"), eATTR_DEL_EARTH_DAMAGE },

        { 0, util::StreamHashKey::GetStringHashKey("darkattack"), eATTR_MAGICAL_DARKNESS_ATTACK_POWER },
        { 1, util::StreamHashKey::GetStringHashKey("darkresist"), eATTR_DEL_DARKNESS_DAMAGE },
    };

    bool processed = false;
    const int number_of_arguments = cmd_tokens.size();

    const ElementMatchNode* match_node = NULL;
    if (number_of_arguments > 1)
    {
        char token[20];
        strncpy(token, cmd_tokens[1].c_str(), _countof(token));
        token[_countof(token) - 1] = '\0';
        _strlwr(token);
        const ulong hash_key = util::StreamHashKey::GetStringHashKey(token);

        match_node = elements_matches;
        for ( ; match_node != &elements_matches[_countof(elements_matches)]; ++match_node)
        {
            if (match_node->key == hash_key) {
                break;
            }
        }
        if (match_node == &elements_matches[_countof(elements_matches)]) {
            match_node = NULL; // not found
        }
    };

    const Attributes& attributes = player->GetAttr();
    if (match_node != NULL && number_of_arguments == 3)
    {
        processed = true;
        const Attributes::Operation oper = { 0, };
        Attributes::OperationRecord record = { 0, };
        record.operate = oper.eOper_Add;
        record.attr_kind = eATTR_KIND_BASE;
        record.attr_type = static_cast<uint16_t>(match_node->attr_type);
        record.attr_value = atoi(cmd_tokens[2].c_str());
        const_cast<Attributes&>(attributes).GroupBatchOperate(&record, 1);
        const_cast<Attributes&>(attributes).UpdateEx();
        ;{
            int value = attributes[match_node->attr_type][eATTR_KIND_BASE];
            MSG_CG_STATUS_STAT_SELECT_ACK msg_ack;
            msg_ack.m_dwObjKey = player->GetObjectKey();
            msg_ack.m_AttrType = match_node->attr_type;
            msg_ack.m_dwValue = value;
            player->SendPacket(&msg_ack, sizeof(msg_ack));
        };
        return true;
    }
    // query command
    if (number_of_arguments == 1 || number_of_arguments == 3)
    {
        processed = true;
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
            "Element:(Attack/Resist)  "
            "Fire(%d,%d), Water(%d,%d), Wind(%d,%d), Earth(%d,%d), Dark(%d,%d)",
            attributes[elements_matches[0].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[1].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[2].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[3].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[4].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[5].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[6].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[7].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[8].attr_type][eATTR_KIND_CALC],
            attributes[elements_matches[9].attr_type][eATTR_KIND_CALC]);

        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        return true;
    }
    //
    ;{
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_CMD_FAILED;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    if (processed == false)
    {
        MSG_CG_ETC_DEBUGINFO_CMD msg_cmd;
        int written = _snprintf(msg_cmd.m_pszChatMsg, _countof(msg_cmd.m_pszChatMsg),
            "Usage = \"//element ["
            "fireattack|fireresist|"
            "waterattack|waterresist|"
            "windattack|windresist|"
            "earthattack|earthresist|"
            "darkattack|darkresist] "
            "[(+/-)value]\"");
        msg_cmd.m_pszChatMsg[_countof(msg_cmd.m_pszChatMsg) - 1] = '\0';
        msg_cmd.m_byMsgLength = static_cast<uint8_t>(min(written, _countof(msg_cmd.m_pszChatMsg)));
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    }
    return false;
#endif
}

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterFomulas(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "공식목록", "formula_list", &ProcessFormulaList),
        GMHANDLER_MAPPING(true, "데미지공식", "damage_formula", &ProcessDamageFormula),
        GMHANDLER_MAPPING(true, "크리티컬공식", "critical_formula", &ProcessCriticalFormula),
        GMHANDLER_MAPPING(true, "원소", "element",  &ProcessChangeElements),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
