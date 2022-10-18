#include "stdafx.h"
#include "./GMCmdManager.h"
//==================================================================================================
#include <StreamHashKey.h>

#include "StatusManager.h"
#include "FunctionalRestrictManager.h"

#include "./GM/GMHandler_InterUtils.hxx"
//==================================================================================================

GMCmdManager* GMCmdManager::instance_static_ = NULL;

GMCmdManager* GMCmdManager::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    instance_static_ = new GMCmdManager;
    return instance_static_;
}

void GMCmdManager::DestroyInstance()
{
    SAFE_DELETE(instance_static_);
}

//==================================================================================================

GMCmdManager::GMCmdManager()
{
    //gm_cmd_string_table_.SetParser(this);
}

GMCmdManager::~GMCmdManager()
{

}

void GMCmdManager::Init()
{
}

BOOL GMCmdManager::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
{
    switch (script_code)
    {
#ifdef _NA000000_GM_COMMAND_CHANGE_CONCEPT_
    case SCRIPT_GMRIGHTS:
        if (bool registered = RegisterAll()) {
            return LoadRights(); // same as 'SetupAuthorization'
        };
        return false;
#else
    case SCRIPT_GMCOMMAND :
        // 2nd load
        //gm_cmd_string_table_.LoadGmData();
        break;
    case SCRIPT_DEVCOMMAND :
        // 1st load
        //gm_cmd_string_table_.LoadDevData();
        RegisterAll();
        // NOTE: this method is started on script loading step. change the gm command authorization
        SetupAuthorization();
        //
        break;
#endif
    default:
        return false;
    }

    return true;
}

void GMCmdManager::Release()
{
    //gm_cmd_string_table_.Release();
}

// CHANGES: f101209.1L, added the interface for total gm command information
const GMCmdManager::HandlerNode* GMCmdManager::GetGMCmdInfo(const char* gm_string_token) const
{
    if (gm_string_token == NULL) {
        return NULL;
    }
    const ulong hash_key = util::StreamHashKey::GetStringHashKey(gm_string_token);
    CmdRouter::const_iterator found = gm_cmd_router_.find(hash_key);
    if (found == gm_cmd_router_.end()) {
        return NULL;
    };
    const HandlerNode* gm_info = found->second;
    return gm_info;
}

//==================================================================================================
//==================================================================================================
// namespace nsGM
//
namespace nsGM {
;

struct GmStatMatchNode {
    ulong hash;
    eATTR_TYPE type_value;
};

struct GmOptionMatchNode {
    ulong hash;
    eGM_STRING type_value;
};

void SendGMCmdAck(Player* pPlayer, const char* source)
{
    MSG_CG_GM_STRING_CMD_ACK ackMsg; // pad out with zeroes    
    strncpy(ackMsg.m_szCommand, source, MSG_CG_GM_STRING_CMD_ACK::MAX_STRING_CMD_LENGTH); 
    pPlayer->SendPacket(&ackMsg, sizeof(ackMsg));
}

bool ModifyGmHandlerNode(GMCmdManager::HandlerNode* node_array, size_t number_of_nodes)
{
    char converted[GMCmdManager::kMaxGmCommandStringLength];
    GMCmdManager::HandlerNode* node_it = node_array,
                             * const node_end = node_array + number_of_nodes;
    for ( ; node_it != node_end; ++node_it)
    {
        if (node_it->__str_korean != NULL && node_it->__str_korean[0] != '\0')
        {
            strncpy(converted, node_it->__str_korean, _countof(converted));
            converted[_countof(converted) - 1] = '\0';
            _strlwr(converted);
            node_it->hash_korean = util::StreamHashKey::GetStringHashKey(converted);
        };
        if (node_it->__str_english != NULL && node_it->__str_english[0] != '\0')
        {
            strncpy(converted, node_it->__str_english, _countof(converted));
            converted[_countof(converted) - 1] = '\0';
            _strlwr(converted);
            node_it->hash_english = util::StreamHashKey::GetStringHashKey(converted);
        };
    }
    return true;
}

void SendGMCmdNak(Player* player, const BYTE error_code)
{
    MSG_CG_GM_STRING_CMD_NAK NakMsg;
    NakMsg.m_byErrorCode = error_code;
    player->SendPacket(&NakMsg, sizeof(NakMsg));
}
}; //end of namespace

//==================================================================================================
//==================================================================================================

bool GMCmdManager::Register_NodeList(const HandlerNode* node_array, size_t number_of_nodes)
{
    if (node_array == NULL) {
        return true;
    };
    const HandlerNode* node_it = node_array,
                     * node_end = node_array + number_of_nodes;
    for ( ; node_it != node_end; ++node_it)
    {
        if (node_it->activated == false) {
            continue;
        }
        if (node_it->hash_korean != 0xFFFFFFFF) {
            CmdRouter::const_iterator found = gm_cmd_router_.find(node_it->hash_korean);
            if (found != gm_cmd_router_.end()) {
                assert(!"try to regist gm command as same key");
                continue;
            }
            gm_cmd_router_.insert(std::make_pair(node_it->hash_korean, node_it));
        }
        if (node_it->hash_english != 0xFFFFFFFF) {
            CmdRouter::const_iterator found = gm_cmd_router_.find(node_it->hash_english);
            if (found != gm_cmd_router_.end()) {
                assert(!"try to regist gm command as same key");
                continue;
            }
            gm_cmd_router_.insert(std::make_pair(node_it->hash_english, node_it));
        }
    }
    return true;
}

bool GMCmdManager::RegisterAll()
{
    const HandlerNode* node_array = NULL;
    size_t number_of_nodes = 0;
    //
    if (RegisterBatchCmd(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterComplex(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterDomination(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterEtc(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterGuild(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterInfo(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterMob(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterPlayerControl(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterRecord(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterSlot(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterStat(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterZone(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterFomulas(&node_array, &number_of_nodes)) { // CHANGES: f110103.4L
        Register_NodeList(node_array, number_of_nodes);
    };
    //
    //
    if (RegisterTestUnit_iamsample(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_arycoat(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_gamelulu(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_hukim(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_jaykang(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_namka199(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_waverix(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    if (RegisterTestUnit_youngmoon(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    //
    //
    // NOTE: call 'SetupAuthorization' or 'LoadRights' after this logic is completed

    return true;
}

bool GMCmdManager::SetupAuthorization()
{
    const eSERVER_MODE server_mode = g_pGameServer->GetMode();
    if (server_mode == eSERVER_DEV || server_mode == eSERVER_INTRAOFFICE)
    {
        const uint8_t change_gm_grade = eGM_GRADE_FIRST;
        FOREACH_CONTAINER(const CmdRouter::value_type& node, gm_cmd_router_, CmdRouter)
        {
            const HandlerNode* handler_node = node.second;
            if (HandlerNode* handler_node_edit = const_cast<HandlerNode*>(handler_node)) {
                handler_node_edit->authorization = change_gm_grade;
            }
        }
        return true;
    };
#ifdef _NA000000_GM_COMMAND_CHANGE_CONCEPT_
    //----------------------------------------------------------------------------------------------
    // TODO: f101209.4L, read the 'GMRights.TXT' script (NEW!).
    //       reserved deletion scripts = { GMCommand.ini, DevCommand.ini }
    // description: exported gm command list on live mode
    // and set access rights on each command by GM grade value (authorization)
    //----------------------------------------------------------------------------------------------
    char lower_string[32];
    const int max_rows = GetRowSize();
    //
    for (int row = 0; row < max_rows; ++row)
    {
        const char* gm_command = GetDataString("Command", row);
        ;{
            strncpy(lower_string, gm_command, _countof(lower_string));
            _strlwr(lower_string);
        };
        const HandlerNode* gm_info = GetGMCmdInfo(lower_string);
        if (gm_info == NULL) {
            continue;
        };
        //
        int gm_grade = GetDataInt("GMGrade", row);
        ;   gm_grade = gm_grade < eGM_GRADE_FIRST   ? eGM_GRADE_FIRST
                     : gm_grade > eGM_GRADE_THIRD   ? eGM_GRADE_MAX
                     : gm_grade;
        //
        if (HandlerNode* gm_info_edit = const_cast<HandlerNode*>(gm_info)) {
            gm_info_edit->authorization = static_cast<uint8_t>(gm_grade);
        }
    }
    return true;
    //
#else //if !defined(_NA000000_GM_COMMAND_CHANGE_CONCEPT_)
        //------------------------------------------------------------------------------------------
        // NOTE: this logic is used only if branched version is foreign version using solarauth
        // because this is a temporary routine to support old custom logic.
        //------------------------------------------------------------------------------------------
        // eGM_GRADE_FIRST
        static const char* live_gm_commands_all[] =
        {
            "����",  "�ӵ�", "�Ӹ�",  "����", "����",
            "��ī��Ʈ",  "��������", "����", "ų",   "����",
            "����",  "������",   "��������",   "��Ȱ", "ȸ��",
            "���ͼ�ȯ", "����������",    "���ݾ�", "��ų����Ʈ��", "����",
            "������", "��þƮ�����", "��", "��", "����_����",
            "ä��_����", "����_����", "������", "������", "����",
            "��", "�Ӹ�", "ä�ñ���", "ä�����", "ä������",
            "�����񺯰�", "����޼���", "����Ŭ����", "���ÿ���", "����",
            "����", "��ȯ", "�̵�", "ī������",
            "��������", "����", "�ݱ�", "Ŭ����̼�", "����Ƽ�����",
            "ĸƾ����", "�̼�", "��Ű����", "üũ����", "����ġ",
            "�κ�����", "ü������", "����ð�", "��������", "�ڻ�",
            "��Ÿ��", "Ŭ���ų", "������", "ac�ʱ�ȭ", "ä���ʱ�ȭ",
            "����Ʈ����", "����Ʈ�Ϸ�", "����Ʈ�ʱ�ȭ", "���", "�ζǹ�ȣ",
            "�ζǰ��", "������", "����������", "�ƽ�������", "��������Ʈ",
            "���â��", "�ܷ�����", "��׷�ǥ��", "��ŸŰ"
            "in", "speed", "hide", "undead", "account",
            "removem", "remove", "kill", "make", "heim",
            "levelup", "kick", "rebirth", "recover", "summon",
            "removei", "statup", "skillup", "info", "roominfo",
            "createenchant", "on", "off", "notice_world", "notice_channel",
            "notice_area", "assagaori", "savemonster", "stopmonster", "regen",
            "mob", "whisper", "chatblock", "chatallow", "channelinfo",
            "changeroominfo", "servershutdown", "chuntingclear", "chuntingincwin", "go",
            "tracking", "call", "move", "settleranking", "increasechao",
            "removestatus", "version", "pickup", "clearmission", "createlimited",
            "killcaptain", "mission", "createlucky", "checkstat", "exp",
            "removeinven", "changeup", "chargetime", "removeh", "die", "ctime",
            "clearskill", "strength", "ssq", "acreset", "ac", "collectable",
            "acceptq", "finishq", "resetq", "drop", "lottonum",
            "settlelotto", "guilddel", "rider", "item", "domi",
            "maxdamage", "accumulatepoint", "guildwarehouse", "perklevelup","aggrodisplay",
            "betakey",
        };
        // eGM_GRADE_SECOND
        static const char* live_gm_commands_second[] = 
        {
            "��������", "removem",
            "����", "removal",
            "���ͼ�ȯ", "summon",
            "������", "levelup",
            "���ݾ�", "statup",
            "��ų����Ʈ��", "skillup",
            "ȸ��", "recover",
            "����������", "removei",
        };
        // eGM_GRADE_THIRD
        static const char* live_gm_commands_third[] = 
        {
            "ų", "kill",
            "����", "make",
            "����", "heim",
            "��þƮ�����", "createenchant",
            "����޼���", "servershutdown",
            "������", "savemonster",
            "������", "stopmonster",
            "����", "regen",
            "��", "mob",
        };
        //
        ;{
            const char** it = live_gm_commands_all,
                      ** end = &live_gm_commands_all[_countof(live_gm_commands_all)];
            for ( ; it != end; ++it)
            {
                const HandlerNode* gm_info = GetGMCmdInfo(*it);
                if (gm_info == NULL) {
                    continue;
                }
                if (HandlerNode* gm_info_edit = const_cast<HandlerNode*>(gm_info)) {
                    gm_info_edit->authorization = eGM_GRADE_FIRST;
                }
            }
        };
        ;{
            const char** it = live_gm_commands_second,
                      ** end = &live_gm_commands_second[_countof(live_gm_commands_second)];
            for ( ; it != end; ++it)
            {
                const HandlerNode* gm_info = GetGMCmdInfo(*it);
                if (gm_info == NULL) {
                    continue;
                }
                if (HandlerNode* gm_info_edit = const_cast<HandlerNode*>(gm_info)) {
                    gm_info_edit->authorization = eGM_GRADE_SECOND;
                }
            }
        };
        ;{
            const char** it = live_gm_commands_third,
                      ** end = &live_gm_commands_third[_countof(live_gm_commands_third)];
            for ( ; it != end; ++it)
            {
                const HandlerNode* gm_info = GetGMCmdInfo(*it);
                if (gm_info == NULL) {
                    continue;
                }
                if (HandlerNode* gm_info_edit = const_cast<HandlerNode*>(gm_info)) {
                    gm_info_edit->authorization = eGM_GRADE_THIRD;
                }
            }
        };
        return true;
#endif
}

Player* GMCmdManager::GetGMPlayer(DWORD player_key)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(player_key);
    if (player == NULL) {
        return NULL;
    }
    // GM���δ� �����̵� �系�̵� �����̵� ������ ó���� üũ �ʼ�!!
    eSERVER_MODE mode = g_pGameServer->GetMode();
    if (player->IsGMUser())     //sts ���� 100�� �Ѿ GM�̸�....
    {
        if (mode == eSERVER_LIVE || mode == eSERVER_TESTLIVE)    // ���񽺿� �̸�..
        {
            return player;
        }
        else if (mode == eSERVER_DEV || mode == eSERVER_INTRAOFFICE) //�����̳�, �系���̸�.. GMListApply�� ���� �޶�����.
        {
            return player;
        }
    }

    //������� ���� GM�� �ƴ϶�� ���̴�.
    MSG_CG_GM_STRING_CMD_NAK NakMsg;
    NakMsg.m_byErrorCode =  RC::RC_GM_ISNOT_GM; // GM�� �ƴϴ�.
    player->SendPacket(&NakMsg, sizeof(NakMsg));
    return NULL;
}

// �� ���� ��� ���� �Լ� 
eATTR_TYPE GMCmdManager::strStatToEnum(const char* stat_string)
{
    static const nsGM::GmStatMatchNode hashes_static[7 * 2] =
    {
        { util::StreamHashKey::GetStringHashKey("��"), eATTR_STR },
        { util::StreamHashKey::GetStringHashKey("str"), eATTR_STR },
        { util::StreamHashKey::GetStringHashKey("��ø"), eATTR_DEX },
        { util::StreamHashKey::GetStringHashKey("dex"), eATTR_DEX },
        { util::StreamHashKey::GetStringHashKey("ü��"), eATTR_VIT },
        { util::StreamHashKey::GetStringHashKey("vit"), eATTR_VIT },
        { util::StreamHashKey::GetStringHashKey("����"), eATTR_INT },
        { util::StreamHashKey::GetStringHashKey("int"), eATTR_INT },
        { util::StreamHashKey::GetStringHashKey("���ŷ�"), eATTR_SPR },
        { util::StreamHashKey::GetStringHashKey("spi"), eATTR_SPR },
        { util::StreamHashKey::GetStringHashKey("����1"), eATTR_EXPERTY1 },
        { util::StreamHashKey::GetStringHashKey("tra1"), eATTR_EXPERTY1 },
        { util::StreamHashKey::GetStringHashKey("����2"), eATTR_EXPERTY2 },
        { util::StreamHashKey::GetStringHashKey("tra2"), eATTR_EXPERTY2 },
    };

    if (stat_string == NULL) {
        return eATTR_TYPE_INVALID;
    }
    char stat_lower_string[7];
    ;{
        strncpy(stat_lower_string, stat_string, _countof(stat_lower_string));
        _strlwr(stat_lower_string);
    };
    const ulong hash_key = util::StreamHashKey::GetStringHashKey(stat_lower_string);
    const nsGM::GmStatMatchNode* node_it = hashes_static,
                               * node_end = &hashes_static[_countof(hashes_static)];
    for ( ; node_it != node_end; ++node_it)
    {
        if (node_it->hash == hash_key) {
            return node_it->type_value;
        }
    }

    return eATTR_TYPE_INVALID;
}

eGM_STRING GMCmdManager::GetGMOptionType(const char* gm_option_token) const
{
    static const nsGM::GmOptionMatchNode option_static[] =
    {
        { util::StreamHashKey::GetStringHashKey("��"), GMSTR_TURNON },
        { util::StreamHashKey::GetStringHashKey("on"), GMSTR_TURNON },
        { util::StreamHashKey::GetStringHashKey("��"), GMSTR_TURNOFF },
        { util::StreamHashKey::GetStringHashKey("off"), GMSTR_TURNOFF },
    };
    if (gm_option_token == NULL) {
        return GMSTR_NONE;
    }
    char toggle_option[4];
    ;{
        strncpy(toggle_option, gm_option_token, _countof(toggle_option));
        _strlwr(toggle_option);
    };
    const ulong hash_key = util::StreamHashKey::GetStringHashKey(toggle_option);
    const nsGM::GmOptionMatchNode* node_it = option_static,
                                 * node_end = &option_static[_countof(option_static)];
    for ( ; node_it != node_end; ++node_it)
    {
        if (node_it->hash == hash_key) {
            return node_it->type_value;
        }
    }

    return GMSTR_NONE;
}

bool GMCmdManager::CheckRight(const HandlerNode* gm_cmd_info, Player* player)
{
    // #1: is GM player
    if (gm_cmd_info == NULL || player->IsGMUser() == false) {
        return false;
    };

    const eSERVER_MODE server_mode = g_pGameServer->GetMode();
    const uint8_t gm_grade = player->GetSts();
    if (gm_grade >= gm_cmd_info->authorization) {
        return true;
    }
    return false;
}

bool GMCmdManager::ParseCommand(DWORD player_key, const char* gm_cmd_string)
{
    GmCmdTokens tokens;
    if (gm_cmd_tokenizer_.Tokenize(gm_cmd_string, &tokens) == false) {
        return false;
    };
    if (ProcessCommandEx(player_key, tokens) == false) {
        return false;
    };
    return true;
}


bool GMCmdManager::ProcessCommandEx(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    const int number_of_tokens = cmd_tokens.size();
    if (number_of_tokens == 0) {
        assert(!"can't enter");
        return false;
    };
    // gm command token specification
    // '//���� 100'
    //   [0]    [1]
    // '����', '100'
    //

    const char* gm_command = cmd_tokens[0].c_str();
    const HandlerNode* gm_node_info = GetGMCmdInfo(gm_command);
    if (gm_node_info == NULL) {
        return false;
    }
    // #2: is GM player
    Player* const player = GetGMPlayer(player_key);
    if (player == NULL)
    {
        Player* normal_player = PlayerManager::Instance()->FindPlayerByUserKey(player_key);
        if (normal_player)
        {
            MessageOut(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|")
                       _T("|Msg=normal player try to execute GM command|")
                       _T("CharGuid=%u, CharName='%s'|"),
                       normal_player->GetCharGuid(), normal_player->GetCharName());
        }
        //
        return false;
    }
    // #3: is valid authorize
    if (CheckRight(gm_node_info, player) == false)
    {
        MessageOut(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("|Msg=GM player is not qualified about '%s' command|")
                   _T("CharGuid=%u, CharName='%s'|"),
                   gm_command, player->GetCharGuid(), player->GetCharName());
        return false;
    }
    // #4: check server run-time configuration
    if (g_FunctionalRestrictMgr.IsRestrictFunction(eFUNCTIONALRESTRICT_GM_COMMAND))
    {
        MessageOut(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("|Msg=Current Server is not serviced GM command by 'Function Restrict'|"));
        return false;
    }

    // #5: check GM command handler
    if (gm_node_info->handler == NULL)
    {
        MessageOut(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("|Msg=can't find '%s' GM command handler|"),
                   gm_command);
    }
    // #5: exectue GM command
    bool success = gm_node_info->handler(player_key, cmd_tokens);
    return success;
}

//==================================================================================================

//���� ����
bool GMCmdManager::ApplyTransparent(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false;
    };
    if (STATE_GM_INFO* state_gm_info = &player->GetGMStateInfo()) {
        state_gm_info->m_TransOn = (turn_on != false);
        player->GetStatusManager()->Status.On_GMTransOn((turn_on != false));
    };

    ;{
        MSG_CG_GM_OBSERVER_BRD msg_brd;
        msg_brd.m_dwObjectKey = player->GetObjectKey();
        msg_brd.m_byObserverOn = turn_on;
        // Enter�ÿ��� ���� ������ Field�� �÷��̾� ����Ʈ�� ���ԵǾ����� �ʴ�.
        player->GetField() ? player->SendPacketAround(&msg_brd, sizeof(msg_brd), true)
                           : player->SendPacket(&msg_brd, sizeof(msg_brd));
    };

    return true;
}

//����
bool GMCmdManager::ApplyInvincibilty(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false;
    }
    if (STATE_GM_INFO* state_gm_info = &player->GetGMStateInfo()) {
        state_gm_info->m_UndeadOn = (turn_on != false);
    }

    StatusField& status_field = player->GetStatusManager()->Status;
    status_field.On_GMUndeadOn(turn_on != false);
    // ack
    MSG_CG_GM_UNDEAD_MODE_ACK msg_ack;
    msg_ack.m_bUndeadOn = (turn_on != false);
    player->SendPacket(&msg_ack, sizeof(msg_ack));

    return true;

}

bool GMCmdManager::ApplySpeed(Player* player, int speed_ratio_level)
{
    if (player == NULL) {
        return false; 
    }
    // ������ 0~8�����̴�.
    if (speed_ratio_level > 8 || speed_ratio_level < 0)
    {
        MSG_CG_GM_STRING_CMD_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        player->SendPacket(&msg_nak, sizeof(msg_nak));
        return false;
    }

    float speed_ratio = 1.0f + 0.5f * speed_ratio_level;

    //�߰��� �̵�
    player->GetGMStateInfo().m_MoveSpeedLevel = speed_ratio_level;
    player->SetAddMoveSpeedRatio(speed_ratio);

    //����������� �ӵ��� �ٲ���� ������.
    MSG_CG_GM_SET_SPEED_BRD msg_brd;
    msg_brd.m_dwObjectKey = player->GetObjectKey();
    msg_brd.m_byMoveLevel = speed_ratio_level;
    msg_brd.m_fSpeedRatio = speed_ratio;

    // Enter�ÿ��� ���� ������ Field�� �÷��̾� ����Ʈ�� ���ԵǾ����� �ʴ�.
    player->GetField() ? player->SendPacketAround(&msg_brd, sizeof(msg_brd), true)
                       : player->SendPacket(&msg_brd, sizeof(msg_brd));

    return true;
}

//����
bool GMCmdManager::ApplyEntrance(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false; 
    };

    if (ApplyTransparent(player, (turn_on ? true : false)) == false ||
        ApplyInvincibilty(player, (turn_on ? true : false)) == false ||
        ApplySpeed(player, (turn_on ? 3 : 0)) == false)
    {
        return false;
    }
    return true;
}

bool GMCmdManager::ApplyMaxDamage(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false;
    }
    StatusField& status_field = player->GetStatusManager()->Status;
    status_field.On_GMMaxDamageOn(turn_on != false);

    return true;
}

bool GMCmdManager::ApplyMaxExp(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false;
    }

    StatusField& status_field = player->GetStatusManager()->Status;
    status_field.On_GMMaxExpOn(turn_on != false);

    return true;
}

bool GMCmdManager::ApplyDropListAll(Player* player, BOOL turn_on)
{
    if (player == NULL) {
        return false;
    }

    StatusField& status_field = player->GetStatusManager()->Status;
    status_field.On_GMDropListAllOn(turn_on != false);

    return true;
}


//==================================================================================================
//==================================================================================================
//==================================================================================================
// util::internal

namespace util { namespace internal {
;

// �Է°��� �˻��Ѵ�.
bool scoped_check_input_in_gm(Player* const gm, const GmCmdTokens& gm_cmd_arguments,
    const size_t num_of_min_gm_cmd_argument, const size_t num_of_max_gm_cmd_argument,
    const bool is_check_field)
{
    const bool valid_number_of_arguments = (gm_cmd_arguments.size() >= num_of_min_gm_cmd_argument && 
                                            gm_cmd_arguments.size() <= num_of_max_gm_cmd_argument);
    if (gm == NULL) {
        return false;
    }
    if (is_check_field == true && gm->GetField() == NULL) {
        return false;
    }

    if (valid_number_of_arguments == false)
    {
        MSG_CG_GM_STRING_CMD_NAK error_msg;
        typedef mpl::if_c<sizeof(error_msg.m_byErrorCode) == sizeof(BYTE),
                          BYTE, void>::type  ConversionCheck;
        error_msg.m_byErrorCode = static_cast<ConversionCheck>(RC::RC_GM_INVALID_ARGUMENT);
        gm->SendPacket(&error_msg, sizeof(error_msg));
        return false;
    }
    return true;
}

}}; //end of namespace