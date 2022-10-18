#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <fstream>
#include <io.h>

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

typedef STLX_VECTOR<STLX_TSTRING> GmCmdHistory;
static GmCmdHistory gm_history_cmd_static;

static bool ProcessSaveCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    int nArgSize = vecCmd.size();
    CHKSendErr(nArgSize < 2 || nArgSize > 16 , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//
    const char* saving_gm_command = vecCmd[1].c_str();
    const GMCmdManager::HandlerNode* saving_gm_cmd_info = \
        gm_manager->GetGMCmdInfo(saving_gm_command);
    CHKSendErr(saving_gm_cmd_info == NULL,
               MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    const GMCmdManager::HandlerNode* run_gm_cmd_info = gm_manager->GetGMCmdInfo("run");
    assert(run_gm_cmd_info);
    if (saving_gm_cmd_info == run_gm_cmd_info) {
        return false;
    }
    //
    char string_buffer[GMCmdParser::kMaxGmStringLength + 1];
    int written = 0;
    for (uint8_t argn = 1; argn < vecCmd.number_of_arguments; ++argn) {
        written += _snprintf(&string_buffer[written], _countof(string_buffer) - written,
                             "%s ", vecCmd[argn].c_str());
    }
    string_buffer[_countof(string_buffer) - 1] = '\0';

    gm_history_cmd_static.push_back(string_buffer);

    return true;
}

static bool ProcessRunCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    CHKSendErr(1 != vecCmd.size() , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//
    GmCmdHistory::const_iterator it = gm_history_cmd_static.begin(),
                                 end = gm_history_cmd_static.end();
    for ( ; it != end; ++it) {
        const STLX_TSTRING& gm_cmd_line = *it;
        gm_manager->ParseCommand(player_key, gm_cmd_line.c_str());
    }

    return true;
}

static bool ProcessDelCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    CHKSendErr(1 != vecCmd.size() , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//

    gm_history_cmd_static.clear();

    return true;
}

static bool ProcessRunSCCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    CHKSendErr(2 != vecCmd.size() , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//   
    char string_buffer[GMCmdParser::kMaxGmStringLength + 1];
    _snprintf(string_buffer, _countof(string_buffer),
        "data\\GmCmd\\_dev_cmd_%s.txt", vecCmd[1].c_str());
    /*_snprintf(string_buffer, _countof(string_buffer),
              "%s data\\GmCmd\\_dev_cmd_%s.txt",
              vecCmd[0].c_str(), vecCmd[1].c_str());*/
    string_buffer[_countof(string_buffer) - 1] = '\0';

    std::ifstream fs(string_buffer);
    STLX_TSTRING strLine;
    while (getline(fs, strLine)) {
        gm_manager->ParseCommand(player_key, strLine.c_str());
    };
    return true;
}

static bool ProcessSaveSCCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    CHKSendErr(2 != vecCmd.size() , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//   

    // 폴더 생성 필요.

    if (_access("data\\GmCmd",0) == -1)
    {
        CreateDirectory("data\\GmCmd",NULL);
    }

    char string_buffer[GMCmdParser::kMaxGmStringLength + 1];
    _snprintf(string_buffer, _countof(string_buffer),
              "data\\GmCmd\\_dev_cmd_%s.txt",
              vecCmd[1].c_str());
    string_buffer[_countof(string_buffer) - 1] = '\0';

    std::ofstream fs(string_buffer);

    GmCmdHistory::const_iterator it = gm_history_cmd_static.begin(),
                                 end = gm_history_cmd_static.end();
    for ( ; it != end; ++it)
    {
        const STLX_TSTRING& gm_cmd_line = *it;
        fs << gm_cmd_line << std::endl;
    }

    return true;
}

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterBatchCmd(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "저장", "save",  &ProcessSaveCmd),
        GMHANDLER_MAPPING(true, "실행", "run",  &ProcessRunCmd),
        GMHANDLER_MAPPING(true, "삭제", "del",  &ProcessDelCmd),
        GMHANDLER_MAPPING(true, "파일실행", "runsc",  &ProcessRunSCCmd),
        GMHANDLER_MAPPING(true, "파일저장", "savesc",  &ProcessSaveSCCmd),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return true;
}

