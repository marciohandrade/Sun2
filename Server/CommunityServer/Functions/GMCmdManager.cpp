#include "WorldServerPch.hxx"
#include "./GMCmdManager.h"

//==================================================================================================
#include <StreamHashKey.h>

#include "WorldServer.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
//==================================================================================================

#if !defined(_NA000000_GM_COMMAND_CHANGE_CONCEPT_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

ns_world::GMCmdManager* ns_world::GMCmdManager::instance_static_ = NULL;

ns_world::GMCmdManager* ns_world::GMCmdManager::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ns_world::GMCmdManager** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            TAllocDelete(ns_world::GMCmdManager, *static_table_);
        }
        ns_world::GMCmdManager** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ns_world::GMCmdManager* provider = TAllocNew(ns_world::GMCmdManager);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

ns_world::GMCmdManager::GMCmdManager()
{
}

ns_world::GMCmdManager::~GMCmdManager()
{

}

void ns_world::GMCmdManager::Init()
{
}

BOOL ns_world::GMCmdManager::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
{
    __UNUSED(reload);
    switch (script_code)
    {
    case SCRIPT_GMRIGHTS:
        if (bool registered = RegisterAll()) {
            return LoadRights(); // same as 'SetupAuthorization'
        };
        return false;
    default:
        return false;
    };
}

void ns_world::GMCmdManager::Release()
{
    //gm_cmd_string_table_.Release();
}

// CHANGES: f101209.1L, added the interface for total gm command information
const ns_world::GMCmdManager::HandlerNode* ns_world::GMCmdManager::GetGMCmdInfo(const char* gm_string_token) const
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

bool ModifyGmHandlerNode(ns_world::GMCmdManager::HandlerNode* node_array, size_t number_of_nodes)
{
    char converted[ns_world::GMCmdManager::kMaxGmCommandStringLength];
    ns_world::GMCmdManager::HandlerNode* node_it = node_array,
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

}; //end of namespace

//==================================================================================================
//==================================================================================================

bool ns_world::GMCmdManager::Register_NodeList(const HandlerNode* node_array, size_t number_of_nodes)
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

bool ns_world::GMCmdManager::RegisterAll()
{
    const HandlerNode* node_array = NULL;
    size_t number_of_nodes = 0;
    //
    if (RegisterCommands(&node_array, &number_of_nodes)) {
        Register_NodeList(node_array, number_of_nodes);
    };
    //
    //
    // NOTE: call 'SetupAuthorization' or 'LoadRights' after this logic is completed

    return true;
}

bool ns_world::GMCmdManager::SetupAuthorization()
{
    const eSERVER_MODE server_mode = ns_world::WorldServer::Instance()->GetMode();
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
}

ns_world::WorldUser* ns_world::GMCmdManager::GetGMUser(DWORD user_key)
{
    ns_world::WorldUser* user = ns_world::WorldUserManager::Instance()->FindUser(user_key);
    if (user == NULL || user->GetUserAccountInfo().gm_grade < eGM_GRADE_FIRST) {
        return NULL;
    };

    return user;
}

eGM_STRING ns_world::GMCmdManager::GetGMOptionType(const char* gm_option_token) const
{
    static const nsGM::GmOptionMatchNode option_static[] =
    {
        { util::StreamHashKey::GetStringHashKey("켬"), GMSTR_TURNON },
        { util::StreamHashKey::GetStringHashKey("on"), GMSTR_TURNON },
        { util::StreamHashKey::GetStringHashKey("끔"), GMSTR_TURNOFF },
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

bool ns_world::GMCmdManager::CheckRight(const HandlerNode* gm_cmd_info, ns_world::WorldUser* user)
{
    // #1: is GM player
    if (gm_cmd_info == NULL || user->IsGMUser() == false) {
        return false;
    };

    //const eSERVER_MODE server_mode = ns_world::WorldServer::Instance()->GetMode();
    if (user->GetUserAccountInfo().gm_grade >= gm_cmd_info->authorization) {
        return true;
    }
    return false;
}

bool ns_world::GMCmdManager::ParseCommand(DWORD user_key, const char* gm_cmd_string)
{
    GmCmdTokens tokens;
    if (gm_cmd_tokenizer_.Tokenize(gm_cmd_string, &tokens) == false) {
        return false;
    };
    if (ProcessCommandEx(user_key, tokens) == false) {
        return false;
    };
    return true;
}


bool ns_world::GMCmdManager::ProcessCommandEx(DWORD user_key, const GmCmdTokens& cmd_tokens)
{
    const int number_of_tokens = cmd_tokens.size();
    if (number_of_tokens == 0) {
        assert(!"can't enter");
        return false;
    };
    // gm command token specification
    // '//하임 100'
    //   [0]    [1]
    // '하임', '100'
    //

    const char* gm_command = cmd_tokens[0].c_str();
    const HandlerNode* gm_node_info = GetGMCmdInfo(gm_command);
    if (gm_node_info == NULL) {
        return false;
    }
    // #2: is GM player
    ns_world::WorldUser* user = this->GetGMUser(user_key);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return false;
    };

    // #3: is valid authorize
    if (CheckRight(gm_node_info, user) == false)
    {
        MessageOut(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("|Msg=GM player is not qualified about '%s' command|")
                   _T("CharGuid=%u, CharName='%s'|"),
                   gm_command, user->GetCharGuid(), user->GetCharName());
        return false;
    }
    // #4: check server run-time configuration
    // NOTE: no check in world server

    // #5: check GM command handler
    if (gm_node_info->handler == NULL)
    {
        MessageOut(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("|Msg=can't find '%s' GM command handler|"),
                   gm_command);
    }
    // #5: execute GM command
    bool success = gm_node_info->handler(user_key, cmd_tokens);
    return success;
}

bool ns_world::GMCmdManager::NoticeBroadcast(
    ns_world::WorldUser* user, eNOTICE_TYPE notice_type, const char* gm_notice_msg)
{
    const char* notice_type_str = NULL;
    ;;;; if (notice_type == eNOTICE_WORLD) {
        notice_type_str = "notice_world";
    }
    else if (notice_type == eNOTICE_CHANNEL) {
        notice_type_str = "notice_channel";
    }
    else if (notice_type == eNOTICE_ZONE) {
        notice_type_str = "notice_area";
    }
    else {
        assert(!"undefined notice type");
        return false;
    };

    // NOTE: check boundary problem...

    char cmd_buffer[GMCmdParser::kMaxGmStringLength + 1];
    _snprintf(cmd_buffer, _countof(cmd_buffer), "%s %s", notice_type_str, gm_notice_msg);

    return this->ParseCommand(user->GetUserGuid(), cmd_buffer);
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace nsGM {
;

static bool ProcessWorldNotice(DWORD user_key, const GmCmdTokens& vecCmd);
static bool ProcessChannelNotice(DWORD user_key, const GmCmdTokens& vecCmd);
static bool ProcessZoneNotice(DWORD user_key, const GmCmdTokens& vecCmd);

};
//==================================================================================================
//==================================================================================================
//==================================================================================================

bool ns_world::GMCmdManager::RegisterCommands(
    const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "월드_공지", "notice_world", &nsGM::ProcessWorldNotice),
        GMHANDLER_MAPPING(true, "채널_공지", "notice_channel", &nsGM::ProcessChannelNotice),
        GMHANDLER_MAPPING(true, "지역_공지", "notice_area", &nsGM::ProcessZoneNotice),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
//==================================================================================================
//==================================================================================================
//==================================================================================================


static bool nsGM::ProcessWorldNotice(DWORD user_key, const GmCmdTokens& vecCmd)
{
    ns_world::GMCmdManager* const gm_manager = ns_world::GMCmdManager::Instance();
    ns_world::WorldUser* user = gm_manager->GetGMUser(user_key);
    if (user == NULL) {
        return false;
    };

    MSG_CW_NOTICE_BRD msg_brd;
    msg_brd.m_byRed = 236;
    msg_brd.m_byGreen = 96;
    msg_brd.m_byBlue = 12;
    msg_brd.wLen = 0;
    //msg_brd.szMsg[MAX_NOTICE_LEN + 1];
    int written = 0;
    const int number_of_args = vecCmd.size();
    for (int index = 1; index < number_of_args; ++index)
    {
        const char* token = vecCmd[index].c_str();
        int result = _snprintf(&msg_brd.szMsg[written], _countof(msg_brd.szMsg) - written,
            "%s", token);
        if (result < 0) {
            break;
        };
        written += result;
    };
    msg_brd.szMsg[_countof(msg_brd.szMsg) - 1] = '\0';
    if (written >= _countof(msg_brd.szMsg)) {
        written = _countof(msg_brd.szMsg) - 1;
    };
    msg_brd.wLen = static_cast<uint16_t>(written);

    ns_world::WorldUserManager* user_manager = ns_world::WorldUserManager::Instance();
    user_manager->SendToAll(&msg_brd, msg_brd.GetSize());

    const TCHAR* notice_tag_msg = _T("World Notice");
    SUNLOG(eCRITICAL_LOG,
        _T("-----------------------------<< %s >>-----------------------------"), notice_tag_msg);
    SUNLOG(eCRITICAL_LOG,
        _T("|Len=%d,Message=[%s]\r\n"), msg_brd.wLen, msg_brd.szMsg);
    SUNLOG(eCRITICAL_LOG,
        _T("---------------------------------------------------------------------------"));
    //
    return true;
};

static bool nsGM::ProcessChannelNotice(DWORD user_key, const GmCmdTokens& vecCmd)
{
    ns_world::GMCmdManager* const gm_manager = ns_world::GMCmdManager::Instance();
    ns_world::WorldUser* user = gm_manager->GetGMUser(user_key);
    if (user == NULL) {
        return false;
    };

    MSG_CW_NOTICE_BRD msg_brd;
    msg_brd.m_byRed = 236;
    msg_brd.m_byGreen = 96;
    msg_brd.m_byBlue = 12;
    msg_brd.wLen = 0;
    //msg_brd.szMsg[MAX_NOTICE_LEN + 1];
    int written = 0;
    const int number_of_args = vecCmd.size();
    for (int index = 1; index < number_of_args; ++index)
    {
        const char* token = vecCmd[index].c_str();
        int result = _snprintf(&msg_brd.szMsg[written], _countof(msg_brd.szMsg) - written,
            "%s", token);
        if (result < 0) {
            break;
        };
        written += result;
    };
    msg_brd.szMsg[_countof(msg_brd.szMsg) - 1] = '\0';
    if (written >= _countof(msg_brd.szMsg)) {
        written = _countof(msg_brd.szMsg) - 1;
    };
    msg_brd.wLen = static_cast<uint16_t>(written);

    user->SendPacketBelongToChannel(&msg_brd, msg_brd.GetSize());

    const TCHAR* notice_tag_msg = _T("Channel Notice");
    SUNLOG(eCRITICAL_LOG,
        _T("-----------------------------<< %s >>-----------------------------"), notice_tag_msg);
    SUNLOG(eCRITICAL_LOG,
        _T("|Len=%d,Message=[%s]\r\n"), msg_brd.wLen, msg_brd.szMsg);
    SUNLOG(eCRITICAL_LOG,
        _T("---------------------------------------------------------------------------"));
    //
    return true;
};

static bool nsGM::ProcessZoneNotice(DWORD user_key, const GmCmdTokens& vecCmd)
{
    ns_world::GMCmdManager* const gm_manager = ns_world::GMCmdManager::Instance();
    ns_world::WorldUser* user = gm_manager->GetGMUser(user_key);
    if (user == NULL) {
        return false;
    };

    MSG_CW_NOTICE_BRD msg_brd;
    msg_brd.m_byRed = 236;
    msg_brd.m_byGreen = 96;
    msg_brd.m_byBlue = 12;
    msg_brd.wLen = 0;
    //msg_brd.szMsg[MAX_NOTICE_LEN + 1];
    int written = 0;
    const int number_of_args = vecCmd.size();
    for (int index = 1; index < number_of_args; ++index)
    {
        const char* token = vecCmd[index].c_str();
        int result = _snprintf(&msg_brd.szMsg[written], _countof(msg_brd.szMsg) - written,
            "%s", token);
        if (result < 0) {
            break;
        };
        written += result;
    };
    msg_brd.szMsg[_countof(msg_brd.szMsg) - 1] = '\0';
    if (written >= _countof(msg_brd.szMsg)) {
        written = _countof(msg_brd.szMsg) - 1;
    };
    msg_brd.wLen = static_cast<uint16_t>(written);

    user->SendPacketBelongToZone(&msg_brd, msg_brd.GetSize());

    const TCHAR* notice_tag_msg = _T("Zone Notice");
    SUNLOG(eCRITICAL_LOG,
        _T("-----------------------------<< %s >>-----------------------------"), notice_tag_msg);
    SUNLOG(eCRITICAL_LOG,
        _T("|Len=%d,Message=[%s]\r\n"), msg_brd.wLen, msg_brd.szMsg);
    SUNLOG(eCRITICAL_LOG,
        _T("---------------------------------------------------------------------------"));
    //
    return true;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================
