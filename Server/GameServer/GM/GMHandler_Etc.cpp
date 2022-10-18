#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <ServerBuildVersion.h>
//
#include "GameScriptParserManager.h"

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
#include "PeakTimeEvent.h"
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#include "MonsterInvasionEvent.h"
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

#ifdef _NA_008078_20150211_GM_COMMAND_TIME_DATE
#include <ShellAPI.h>
#endif // _NA_000000_20150211_SERVER_TIME_CHANGE

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingGradeManager.h"
#include "ServerOptionParserEx.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

namespace nsGM {
;

// GM 귓말 켬/끔
static bool ProcessGMWhisperSet(DWORD player_key, const GmCmdTokens& vecCmd)
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

    BOOL bWantWhisperOn = false;
    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    ;    if (type == GMSTR_TURNON)  bWantWhisperOn = true;
    else if (type == GMSTR_TURNOFF) bWantWhisperOn = false;
    else                            return false;

    MSG_AG_GM_WHISPER_SET_CMD CmdMsg;
    CmdMsg.m_dwKey = pPlayer->GetCharGuid();
    CmdMsg.m_bOn = bWantWhisperOn;
    g_pGameServer->SendToServer(AGENT_SERVER, &CmdMsg, sizeof(CmdMsg));

    return true;
}

// 채팅금지
static bool ProcessUserChatBlock(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
    if (vecCmd.size() != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 누굴 블록할껴?
    TCHAR ptszCharName[MAX_CHARNAME_LENGTH+1]={0,};
    _tcsncpy(ptszCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);

    Player* pBlockPlayer = PlayerManager::Instance()->FindPlayerByName(ptszCharName);
    if (!pBlockPlayer)   return false;

    MSG_AG_GM_USERCHAT_BLOCK_CMD cmdMsg;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK    
    cmdMsg.m_dwKey = pPlayer->GetCharGuid();
    cmdMsg.m_bBlock = true;
    cmdMsg.m_CharGuid = pBlockPlayer->GetCharGuid();
    cmdMsg.m_dwBlockTime = atoi(vecCmd[2].c_str()); 

    g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));

    return true;
}

// 채팅허용
static bool ProcessUserChatAllow(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 누굴 허용할껴?
    TCHAR       ptszCharName[MAX_CHARNAME_LENGTH+1]={0,};

    _tcsncpy(ptszCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);

    Player* pAllowPlayer = PlayerManager::Instance()->FindPlayerByName(ptszCharName);

    if (!pAllowPlayer)
        return false;

    MSG_AG_GM_USERCHAT_BLOCK_CMD CmdMsg;
    CmdMsg.m_dwKey = pPlayer->GetCharGuid();
    CmdMsg.m_bBlock = false;
    CmdMsg.m_CharGuid = pAllowPlayer->GetCharGuid();
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
    g_pGameServer->SendToServer(AGENT_SERVER, &CmdMsg, sizeof(CmdMsg));

    return true;
}

//디버그정보
static bool ProcessDebugInfo(DWORD player_key, const GmCmdTokens& vecCmd)
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

    int nType = atoi(vecCmd[1].c_str());

    if (nType >= eDEBUGINFO_TYPE_MAX || nType < eDEBUGINFO_TYPE_INVALID)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    pPlayer->SetDebugInfoFlag((BYTE)nType);

    return true;
}

static bool ProcessServerShutdown(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    int iStringCode = atoi(vecCmd[1].c_str());
    int iShutdownDelayTime = atoi(vecCmd[2].c_str());
    if (iStringCode < 0 || (iShutdownDelayTime < 5 || iShutdownDelayTime > 30))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    MSG_AG_GM_SERVER_SHUTDOWN_CMD cmsg;
    cmsg.m_dwStringCode     = (DWORD)iStringCode;
    cmsg.m_byShutdownTime   = (BYTE)iShutdownDelayTime;
    pPlayer->SendPacket(&cmsg, sizeof(cmsg));

    GAMELOG->LogGM(GM_SERVERSHUTDOWN, pPlayer);

    return true;
}

//채널정보
static bool ProcessChannelInfo(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    MSG_AG_GM_CHANNEL_INFO_CMD cmd;
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;

}


static bool ProcessVersion(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)        return false;

    MSG_CG_GM_VERSION_ACK AckMsg;
    AckMsg.m_byScriptCode = SCRIPT_EMPTY;

    // 요구하는 버전 정보에 따라 인자의 개수가 달라진다.
    if (vecCmd.size() < 2)       
    {
        AckMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        AckMsg.m_byVersionType = VERSION_NONE;
        ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
        pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
        return false;
    }

    eVERSION_TYPE Type = (eVERSION_TYPE)atoi(vecCmd[1].c_str()); 

    //타입비교
    if (Type <= VERSION_NONE || Type >= VERSION_MAX)
    {
        AckMsg.m_byErrorCode = RC::RC_GM_INVALID_VERSION_TYPE;
        AckMsg.m_byVersionType = Type;
        ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
        pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
        return false;
    }

    // 버전타입.
    AckMsg.m_byVersionType = Type;

    if (Type == VERSION_SCRIPT)
    {
        //인자가 2개 인지 체크
        if (vecCmd.size() != 3)
        {
            AckMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
            ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
            pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
            return false;
        }

        STLX_STRING strFile = "data\\";
        strFile += vecCmd[2].c_str();

        SCRIPT_DATA* pScript = GameScriptParserManager::Instance()->FindScript(strFile.c_str());
        if (!pScript)
        {
            AckMsg.m_byErrorCode = RC::RC_GM_NOTEXIST_SCRIPT;
            ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
        }
        else
        {
            AckMsg.m_byScriptCode = pScript->m_ScriptCode;
            AckMsg.m_byErrorCode = RC::RC_GM_CMD_SUCCESS;
            strncpy(AckMsg.m_szVersion, pScript->m_szVersion, MAX_VERSION_SIZE);
        }

        pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
        return true;
    }
    else
    {
        TCHAR szVersion[MAX_VERSION_SIZE+1];

        //인자가 1개인지 체크
        if (vecCmd.size() != 2)
        {
            AckMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
            ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
            pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
            return false;
        }       

        switch(Type)
        {
        case VERSION_BUILD:
            AckMsg.m_byErrorCode = RC::RC_GM_CMD_SUCCESS;
            _snprintf(szVersion, MAX_VERSION_SIZE, "%u", NATION_BUILD_VERSION);
            break;
        case VERSION_PROTOCOL:
            AckMsg.m_byErrorCode = RC::RC_GM_CMD_SUCCESS;
            _snprintf(szVersion, MAX_VERSION_SIZE, "%d.%d.%d", C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO);
            break;
        case VERSION_COMPILE:
            AckMsg.m_byErrorCode = RC::RC_GM_CMD_SUCCESS;
            _snprintf(szVersion, MAX_VERSION_SIZE, "%d", SERVER_RELEASE_BUILD_NO);
            break;
        case VERSION_ENGINE:
            AckMsg.m_byErrorCode = RC::RC_GM_CMD_SUCCESS;
            _snprintf(szVersion, MAX_VERSION_SIZE, "%u", WZENGINEVER);
            break;
        default:
            AckMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
            ZeroMemory(AckMsg.m_szVersion, MAX_VERSION_SIZE);
            SUNLOG(eCRITICAL_LOG, "[ProcessVersion] 잘못된 버전타입 Type = %d", Type);
            break;
        }

        szVersion[MAX_VERSION_SIZE] = '\0';

        strncpy(AckMsg.m_szVersion, szVersion, MAX_VERSION_SIZE);
        pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
        return true;
    }

    return false;
}

struct GmNoticeMatchNode {
    ulong hash;
    eNOTICE_TYPE type_value;
};

static bool ProcessGMNotice(DWORD player_key, const GmCmdTokens& vecCmd)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(player_key);
    if (player == NULL) {
        return false;
    }

    static const GmNoticeMatchNode notice_static[] =
    {
        { util::StreamHashKey::GetStringHashKey("월드_공지"), eNOTICE_WORLD },
        { util::StreamHashKey::GetStringHashKey("notice_world"), eNOTICE_WORLD },
        { util::StreamHashKey::GetStringHashKey("채널_공지"), eNOTICE_CHANNEL },
        { util::StreamHashKey::GetStringHashKey("notice_channel"), eNOTICE_CHANNEL },
        { util::StreamHashKey::GetStringHashKey("지역_공지"), eNOTICE_ZONE },
        { util::StreamHashKey::GetStringHashKey("notice_area"), eNOTICE_ZONE },
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    eNOTICE_TYPE notice_type = eNOTICE_ZONE;
    const ulong gm_cmd_hash = util::StreamHashKey::GetStringHashKey(vecCmd[0].c_str());
    const GmNoticeMatchNode* node_it = notice_static,
                           * node_end = &notice_static[_countof(notice_static)];
    for ( ; node_it != node_end; ++node_it)
    {
        if (node_it->hash != gm_cmd_hash) {
            continue;
        }
        notice_type = node_it->type_value;
        break;
    }
    if (node_it == node_end) {
        return false;
    }

    // CHANGES: f101210.2L, check boundary and consider length with null-terminated position
    MSG_AG_GM_NOTICE_CMD msg_cmd;
    int written = 0;
    const int number_of_tokens = vecCmd.size(); // consider start offset
    // number of messages ranges = [1, n)
    for (int msg_index = 1; msg_index < number_of_tokens; ++msg_index)
    {
        const char* output_format = (msg_index + 1 == number_of_tokens) ? "%s" : "%s ";
        int token_written = _sntprintf(msg_cmd.m_tszNoticeMsg + written,
                                       _countof(msg_cmd.m_tszNoticeMsg) - written,
                                       output_format, vecCmd[msg_index].c_str());
        if (token_written < 0) {
            written = _countof(msg_cmd.m_tszNoticeMsg) - 1; // to fit 'written + 1'
            break;
        }
        written += token_written;
    };
    msg_cmd.m_tszNoticeMsg[_countof(msg_cmd.m_tszNoticeMsg) - 1] = _T('\0');
    // current state
    // origin  = hello, world
    // message = [h][e][l][l][o][,][ ][w][o][r][l][d]
    // buffer  = [0] ~                            [B][C]
    // m_wLen = 0xC (with null-terminated position)
    ;{
        msg_cmd.m_dwKey = player->GetUserGuid();
        msg_cmd.m_eType = notice_type;
        assert(written < _countof(msg_cmd.m_tszNoticeMsg));
        msg_cmd.m_wLen = written + 1;
    };

    player->SendPacket(&msg_cmd, msg_cmd.GetSize());

    return true;
}


static bool ProcessServerTime(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    MSG_CG_GM_SERVER_TIME_CMD cmd;

    util::GetDateTime_YYYYMMDDHHMMSS(cmd.m_CurTime);

    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}



//__NA_001359_20090619_HEIM_LOTTO_SYSTEM
static bool ProcessLottoNum(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer) { return false; }

    // Agent로 중계
    MSG_AG_GM_LOTTO_NUM_CMD cmd;
    cmd.dwUserKey = player_key;
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}

static bool ProcessSettleLotto(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer) { return false; }

    // Agent로 중계
    MSG_AG_GM_LOTTO_SETTLE_CMD  cmd;
    cmd.dwUserKey = player_key;
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}

static bool ProcessLottoFund(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer) { return false; }

    if (vecCmd.size() < 2) {
        return false;
    }

    // Agent로 중계
    MSG_AG_GM_LOTTO_FUND_CMD  cmd;
    cmd.user_key = player_key;
    cmd.amount_delta = _tstoi64(vecCmd[1].c_str());
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
static bool ProcessEventControl(DWORD player_key, const GmCmdTokens& cmd_args)
{
    enum CMD_EXPEVENT_INDEX
    {
        eEXPCMD_STARTDATE = 1,	// 시작날짜
        eEXPCMD_STARTTIME ,     // 시작시간
        eEXPCMD_ENDDATE	  ,  // 종료날짜
        eEXPCMD_ENDTIME   ,  // 종료시간
        eEXPCMD_RATIO	  ,  // 경험치증가비율
        eEXPCMD_MAX		  
    };

    PeakTimeEvent* peaktime_event = PeakTimeEvent::Instance();

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    
    if (cmd_args.size() == 2)
    {
        if (strcmp(cmd_args[1].c_str(), "종료") == 0 || strcmp(cmd_args[1].c_str(), "end") == 0)
        {
            // 현재 진행중인 [GM이 추가한 이벤트]를 제거한다
            {
                MSG_DG_GMEVENT_PEAKTIME_TERMINATE_SYN syn_msg;
                
                g_pGameServer->SendToServer(GAME_DBPROXY, &syn_msg, sizeof(syn_msg));

                SendGMCmdAck(gm_player, cmd_args[0].c_str());
            };

            return true;
        }
        else if (strcmp(cmd_args[1].c_str(), "확인") == 0 || strcmp(cmd_args[1].c_str(), "check") == 0)
        {
            // [GM이 추가한 이벤트] 목록을 보여준다
            peaktime_event->SendGMEventList(gm_player);

            SendGMCmdAck(gm_player, cmd_args[0].c_str());
        }
        else
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
            gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
            return false; 
        }
    }
    else if (cmd_args.size() == 6)
    {
        int event_start_date = 0;
        INT event_start_time = 0;
        int event_end_date = 0;
        int event_end_time = 0;
        int event_ratio = 0;
        util::internal::scoped_string_to_number(cmd_args[eEXPCMD_STARTDATE], event_start_date);
        util::internal::scoped_string_to_number(cmd_args[eEXPCMD_STARTTIME], event_start_time);
        util::internal::scoped_string_to_number(cmd_args[eEXPCMD_ENDDATE], event_end_date);
        util::internal::scoped_string_to_number(cmd_args[eEXPCMD_ENDTIME], event_end_time);
        util::internal::scoped_string_to_number(cmd_args[eEXPCMD_RATIO], event_ratio);

        if (event_start_date == 0 || event_end_date == 0 || event_ratio == 0)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
            gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
            return false; 
        }

        EVENT_INFO event_info;
        ZeroMemory(&event_info, sizeof(event_info));
        event_info.SetGMEventWithoutID(eEVENT_PEAKTIME, event_start_date, event_start_time, 
                                       event_end_date, event_end_time, event_ratio);

        {
            MSG_DG_GMEVENT_PEAKTIME_REGISTER_SYN dg_msg;
            dg_msg.register_event = event_info;

            g_pGameServer->SendToServer(GAME_DBPROXY, &dg_msg, sizeof(dg_msg));

            SendGMCmdAck(gm_player, cmd_args[0].c_str());
        };
        
        return true;
    }
    else
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    return false;
}
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
static bool ProcessMonsterInvasionEvent(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    if (cmd_args.size() == 2)
    {
        int village_code = 0;
        util::internal::scoped_string_to_number(cmd_args[1], village_code);

        MonsterInvasionEvent::Instance()->StartMonsterInvasion((WORD)village_code, TRUE);
    }
    else
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }
    return true;
}
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

#ifdef _NA_008078_20150211_GM_COMMAND_TIME_DATE
static bool ProcessServerTimeChange(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    if (g_pGameServer->GetMode() != eSERVER_DEV)
    {
        return false;
    }

    bool result = false;
    if (cmd_args.size() == 3)
    {
        int hours, minutes;
        util::internal::scoped_string_to_number(cmd_args[1], hours);
        util::internal::scoped_string_to_number(cmd_args[2], minutes);

        char temp_buffer[MAX_CHATMSG_SIZE] = {0, };
        if (0 <= hours && hours <= 24 && 0 <= minutes && minutes <= 60)
        {
            _sntprintf(temp_buffer, MAX_CHATMSG_SIZE, "/c time %d:%d:00", hours, minutes);
            ShellExecute(NULL, "open", "cmd.exe", temp_buffer, NULL, SW_HIDE);
            result = true;
        }
    }
    
    if (result == true)
    {
        SendGMCmdAck(gm_player, cmd_args[0].c_str());
    }
    else
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }
    return true;
}

static bool ProcessServerDateChange(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    if (g_pGameServer->GetMode() != eSERVER_DEV)
    {
        return false;
    }

    bool result = false;
    if (cmd_args.size() == 4)
    {
        int year, month, day;
        util::internal::scoped_string_to_number(cmd_args[1], year);
        util::internal::scoped_string_to_number(cmd_args[2], month);
        util::internal::scoped_string_to_number(cmd_args[3], day);

        char temp_buffer[MAX_CHATMSG_SIZE] = {0, };
        if (2000 <= year && year <= 3000 && 1 <= month && month <= 12 && 1 <= day && day <= 31)
        {
            _sntprintf(temp_buffer, MAX_CHATMSG_SIZE, "/c date %d-%d-%d", year, month, day);
            ShellExecute(NULL, "open", "cmd.exe", temp_buffer, NULL, SW_HIDE);
            result = true;
        }
    }

    if (result == true)
    {
        SendGMCmdAck(gm_player, cmd_args[0].c_str());
    }
    else
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        gm_player->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }
    return true;
}
#endif // _NA_000000_20150211_SERVER_TIME_CHANGE

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
static bool ProcessSunRankingSettle(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    if (g_pGameServer->GetMode() != eSERVER_DEV)
    {
        return false;
    }

    SunRankingGradeManager::Instance()->Settle_StartForGM();

    SendGMCmdAck(gm_player, cmd_args[0].c_str());

    return true;
}
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
static bool ProcessNotificationClear(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    MSG_DG_NOTIFICATION_CLEAR_SYN syn_msg;
    g_pGameServer->SendToSessionType(GAME_DBPROXY, &syn_msg,sizeof(syn_msg));

    SendGMCmdAck(gm_player, cmd_args[0].c_str());

    return true;
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterEtc(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "귓말", "whisper", &ProcessGMWhisperSet), // 귓말 켬, & 끔
        GMHANDLER_MAPPING(true, "채팅금지", "chatblock", &ProcessUserChatBlock), // 채팅 금지
        GMHANDLER_MAPPING(true, "채팅허용", "chatallow", &ProcessUserChatAllow), // 채팅 허용
        GMHANDLER_MAPPING(true, "채널정보", "channelinfo", &ProcessChannelInfo), // 채널 정보
        GMHANDLER_MAPPING(true, "아싸가오리", "assagaori", &ProcessDebugInfo),
        GMHANDLER_MAPPING(true, "종료메세지", "servershutdown", &ProcessServerShutdown),
        GMHANDLER_MAPPING(true, "버전", "version", &ProcessVersion),
        GMHANDLER_MAPPING(true, "월드_공지", "notice_world", &ProcessGMNotice),
        GMHANDLER_MAPPING(true, "채널_공지", "notice_channel", &ProcessGMNotice),
        GMHANDLER_MAPPING(true, "지역_공지", "notice_area", &ProcessGMNotice),
        GMHANDLER_MAPPING(true, "서버시간", "stime", &ProcessServerTime),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        GMHANDLER_MAPPING(true, "로또번호", "lottonum", &ProcessLottoNum), // 하임 로또 번호 보이기
        GMHANDLER_MAPPING(true, "로또결산", "settlelotto", &ProcessSettleLotto), // 하임 로또 강제 추첨
        GMHANDLER_MAPPING(true, "로또기금", "lottofund", &ProcessLottoFund), // 하임 로또 기금 적립
        //}
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
        GMHANDLER_MAPPING(true, "경험치이벤트", "expevent", &ProcessEventControl),
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
        GMHANDLER_MAPPING(true, "몬스터침공", "invasion", &ProcessMonsterInvasionEvent),
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008078_20150211_GM_COMMAND_TIME_DATE
        GMHANDLER_MAPPING(true, "서버시간변경", "server_time_change", &ProcessServerTimeChange),
        GMHANDLER_MAPPING(true, "서버날짜변경", "server_date_change", &ProcessServerDateChange),
#endif // _NA_000000_20150211_SERVER_TIME_CHANGE
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        GMHANDLER_MAPPING(true, "썬랭킹정산", "sun_ranking_settle", &ProcessSunRankingSettle),
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        GMHANDLER_MAPPING(true, "알림삭제", "notification_clear", &ProcessNotificationClear),
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
