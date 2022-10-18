#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <ServerOptionParserEx.h>
//
#include "GameZoneManager.h"
#include "GameZone.h"
#include "World/GameWarControlManager.h"
#include "MovingPlanner.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <../Common/PacketStruct_AG_ChaosZoneSystem.h>
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include "GameInstanceDungeon.h"
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

namespace nsGM {
;

// 사용법 : 가기 맵코드 X Y Z // 방으로 이동은 아직 지원안함
static bool ProcessGo(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)        return false;

    if (vecCmd.size() != 2 && vecCmd.size() != 5)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    WzVector vecDestPos; memset(&vecDestPos, 0, sizeof(WzVector));
    if (vecCmd.size() == 5)
    {
        vecDestPos.x = (float)atof(vecCmd[2].c_str());
        vecDestPos.y = (float)atof(vecCmd[3].c_str());
        vecDestPos.z = (float)atof(vecCmd[4].c_str());
    }

    FIELDCODE ToFieldCode = atoi(vecCmd[1].c_str());

    if (pPlayer->Portal(ePORTAL_TYPE_GM, ToFieldCode, vecDestPos, 0) == RC::RC_PORTAL_FAIL)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    // GM 명령 성공
    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

//추적..
static bool ProcessTracking(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        SendGMCmdNak(pPlayer, RC::RC_GM_INVALID_ARGUMENT);
        return false; 
    }

    // Agent에 추적명령을 알린다.
    MSG_AG_GM_TRACKING_CMD AGMsg;
    AGMsg.m_PortalType = (BYTE)ePORTAL_TYPE_GM;
    _tcsncpy(AGMsg.m_szCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);
    AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
    pPlayer->GetPos(&AGMsg.m_vCurPos);
    pPlayer->SendPacket(&AGMsg, sizeof(AGMsg));

    // GM 명령 성공
    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}


//
//  ssq [open|close] [mapcode]
//
static bool ProcessSSQCtrlCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    const INT args = (INT)vecCmd.size();
    CHKSendErr(!(1<args && args<(5+1)), MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    GameSSQCtrlCmd::GMCmd gm_cmd;
    nsSSQ_Util::GM_Spec::StringArray& arguments = gm_cmd.Arguments;
    ZeroMemory(arguments, sizeof(arguments));

    const INT STRBUF_SIZE = _countof(arguments[0]);
    for (INT i=0 ; i<args ; ++i)
    {
        _tcsncpy(arguments[i], vecCmd[i].c_str(), STRBUF_SIZE);
        arguments[i][STRBUF_SIZE-1] = _T('\0');

        for (TCHAR* pOff = arguments[i] ; *pOff ; ++pOff)
            *pOff = tolower(*pOff);
    }

    const BOOLEAN bResult = GameWarControlManager::OnMsg(gm_cmd, pPlayer);
    CHKSendErr(!bResult, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    return true;
}


//입장
static bool ProcessEntrance(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; 
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bEntranceOn = false;

    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bEntranceOn = true;
    else if (type == GMSTR_TURNOFF)  bEntranceOn = false;
    else 
        return false;

    if (!gm_manager->ApplyEntrance(pPlayer, bEntranceOn))
        return false;
    else
    {
        GAMELOG->WriteEntrance(pPlayer); //입장 GM로그
        return true;
    }
}

//방정보
static bool ProcessRoomInfo(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //인자개수 체크
    if (vecCmd.size() != 2 && vecCmd.size() != 1)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    DWORD dwRoomKey = 0;

    // 방번호가 존재하는가 안하는가에 따라 틀리다.
    if (vecCmd.size() == 1)          // 방번호가 존재하지 않으면...
    {
        GameZone* pZone = pPlayer->GetGameZonePtr();
        if (!pZone)
        {
            MSG_CG_GM_STRING_CMD_NAK NakMsg;
            NakMsg.m_byErrorCode    =  RC::RC_GM_ROOM_NOT_EXIST; // 방이 존재하지 않습니다.
            pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
            return false; 
        }

        dwRoomKey = pZone->GetKey();
    }
    else if (vecCmd.size() > 1)      // 방번호가 존재하면..
        dwRoomKey = atoi(vecCmd[1].c_str());

    //Agent로..Send
    MSG_AG_GM_ROOM_INFO_CMD cmd;
    cmd.m_dwRoomKey = dwRoomKey;
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}

//방제목 변경
static bool ProcessChangeRoomTitle(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    KEYTYPE RoomKey = atoi(vecCmd[1].c_str());

    //1. AGENT로 보내서, AGENT의 방 타이틀를 변경시킨다.
    //2. 해당 방의 방장에게 방제목 변경 패킷을 보낸다.
    MSG_AG_GM_CHANGE_ROOMTITLE_CMD  cmd;
    cmd.m_dwRoomKey = RoomKey;
    ZeroMemory(cmd.m_szRoomTitle, MAX_ROOMTITLE_LENGTH);
    strncpy(cmd.m_szRoomTitle, vecCmd[2].c_str() , MAX_ROOMTITLE_LENGTH);
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}


// 유저 이동
static bool ProcessUserMove(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)        return false;

    if (vecCmd.size() != 5)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    // 필드코드
    FIELDCODE ToFieldCode = atoi(vecCmd[2].c_str());

    // 좌표
    WzVector vecDestPos; memset(&vecDestPos, 0, sizeof(WzVector));

    vecDestPos.x = (float)atof(vecCmd[3].c_str());
    vecDestPos.y = (float)atof(vecCmd[4].c_str());
    vecDestPos.z = 0;

    GameZone* pZone = g_GameZoneManager.GetFieldVillageZone(ToFieldCode);

    if (!pZone) 
    { 
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    float fZDiff = 0.0f;

    SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

    GameField* const pGameField = pZone->FindGameField(ToFieldCode);//...찾아지는 모양이지?
    nsAI::MovingPlanner* const pMovingPlanner = pGameField->GetMovingPlanner();

    const INT iTileIndex
        = pMovingPlanner->GetTileToStand(
        vecDestPos, &fZDiff, stServerOptionInfo.m_fFINDPATH_JUMPLIMIT, 100.0f
        );
    if (iTileIndex == -1)    
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    vecDestPos.z -= fZDiff;

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(pPlayer->ReCallPlayer(vecCmd[1].c_str(), 0, ToFieldCode, vecDestPos, 
                             ePORTAL_TYPE_GM, INVALID_POSTYPE_VALUE, 0))
    {
        SendGMCmdAck(pPlayer, vecCmd[0].c_str());
    }
#else
    // Agent에 이동명령을 내린다.
    MSG_AG_GM_GO_CMD AGMsg;
    _tcsncpy(AGMsg.m_szCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);
    AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
    AGMsg.m_RoomKey = 0;
    AGMsg.m_FieldCode = ToFieldCode;
    pPlayer->GetPos(&AGMsg.m_vCurPos);
    AGMsg.m_vDestPos = vecDestPos;
    AGMsg.m_PortalType = (BYTE)ePORTAL_TYPE_GM;
    AGMsg.m_atItemPos = INVALID_POSTYPE_VALUE;
    AGMsg.m_atItemCode = 0;

    pPlayer->SendPacket(&AGMsg, sizeof(AGMsg));

    // GM 명령 성공
    SendGMCmdAck(pPlayer, vecCmd[0].c_str());
#endif

    return true;
}

#ifdef _NA_0_20100416_RELOAD_MAPOBJECT
static bool ProcessReLoadMapObject(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (pPlayer == 0)
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(1 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return false;

    ZoneReloader ReLoader;
    ReLoader.Reload();

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}
#endif

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
// 자유전투실 제어 자유전투실 켬[끔]
static bool ProcessFreePVPControl(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL)
    {
        return false;
    }

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // 인자가 맞지 않습니다.
        player->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    bool is_free_pvp_active = false;
    eGM_STRING string_type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (string_type == GMSTR_TURNON)
    {
        is_free_pvp_active = true;
    }
    else if (string_type == GMSTR_TURNOFF)
    {
        is_free_pvp_active = false;
    }
    else
    {
        return false;
    }

    MSG_AG_FREE_PVP_ACTIVE_CMD request;
    {
        request.m_dwKey = player->GetObjectKey();
        request.is_free_pvp_active = is_free_pvp_active;
    }
    player->SendPacket(&request, sizeof(request));

}
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
static bool ProcessBattleGround(DWORD player_key, const GmCmdTokens& cmd_tokens)
{
    enum GMCmdIndex 
    { 
        kMinArgumentSize = 2,
        kMaxArgumentSize = 4,
    };

    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    using namespace util::internal;
    if (scoped_check_input_in_gm(player, cmd_tokens, kMinArgumentSize, kMaxArgumentSize, true) == false)
    {
        return false;
    }

    enum cmd_mode {
        reservation,
        cancel_reservation,
        team_reservation,
        cancel_team_reservation,
        start,
        enter,
        victory_point,
        clear,
        max_cmd_mode,
    };

    MSG_AG_GM_BATTLE_GROUND_CMD cmd_msg;

    cmd_mode mode = max_cmd_mode;
    const char* cmd_str = cmd_tokens[1].c_str();

    if (strcmp(cmd_str, "예약") == 0 || strcmp(cmd_str, "reservation") == 0)
        mode = reservation;
    else if (strcmp(cmd_str, "예약취소") == 0 || strcmp(cmd_str, "cancel_reservation") == 0)
        mode = cancel_reservation;
    else if (strcmp(cmd_str, "팀예약") == 0 || strcmp(cmd_str, "team_reservation") == 0)
        mode = team_reservation;
    else if (strcmp(cmd_str, "팀예약취소") == 0 || strcmp(cmd_str, "cancel_team_reservation") == 0)
        mode = cancel_team_reservation;
    else if (strcmp(cmd_str, "시작") == 0 || strcmp(cmd_str, "start") == 0)
        mode = start;
    else if (strcmp(cmd_str, "입장") == 0 || strcmp(cmd_str, "enter") == 0)
        mode = enter;
    else if (strcmp(cmd_str, "승점") == 0 || strcmp(cmd_str, "victory_point") == 0)
        mode = victory_point;
    else if (strcmp(cmd_str, "클리어") == 0 || strcmp(cmd_str, "clear") == 0)
        mode = clear;
    else
        mode = max_cmd_mode;

    switch(mode)
    {
    case reservation:
        {
            cmd_msg.cmd_mode = reservation;
            const char* team_str = cmd_tokens[2].c_str();
            if (strcmp(team_str, "레드") == 0 || strcmp(team_str, "red") == 0)
                cmd_msg.team_key = kRed;
            else if (strcmp(team_str, "블루") == 0 || strcmp(team_str, "blue") == 0)
                cmd_msg.team_key = kBlue;

            const TCHAR* char_name_str = cmd_tokens[3].c_str();
            Player* const player = PlayerManager::Instance()->FindPlayerByName(char_name_str);
            if (player == NULL)
            {
                return false;
            }
            cmd_msg.char_guid = player->GetCharGuid();

            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s][%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str(),
                cmd_tokens[2].c_str(), cmd_tokens[3].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');

#ifdef _NA_007191_20140428_MODIFY_GM_COMMAND
            cmd_msg.map_code = 0;
#endif //_NA_007191_20140428_MODIFY_GM_COMMAND
        }break;

    case cancel_reservation:
        {
            cmd_msg.cmd_mode = cancel_reservation;
            const TCHAR* char_name_str = cmd_tokens[2].c_str();
            Player* const player = PlayerManager::Instance()->FindPlayerByName(char_name_str);
            if (player == NULL)
            {
                return false;
            }

            cmd_msg.char_guid = player->GetCharGuid();
            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str(),
                cmd_tokens[2].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');

        }break;

    case team_reservation:
        {
            cmd_msg.cmd_mode = team_reservation;
            const char* team_str = cmd_tokens[2].c_str();
            if (strcmp(team_str, "레드") == 0 || strcmp(team_str, "red") == 0)
                cmd_msg.team_key = kRed;
            else if (strcmp(team_str, "블루") == 0 || strcmp(team_str, "blue") == 0)
                cmd_msg.team_key = kBlue;

            const TCHAR* char_name_str = cmd_tokens[3].c_str();
            Player* const player = PlayerManager::Instance()->FindPlayerByName(char_name_str);
            if (player == NULL)
            {
                return false;
            }
            cmd_msg.char_guid = player->GetCharGuid();

            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s][%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str(),
                cmd_tokens[2].c_str(), cmd_tokens[3].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');

#ifdef _NA_007191_20140428_MODIFY_GM_COMMAND
            const char* temp_str = cmd_tokens[4].c_str();
            if (temp_str == NULL) {
                return false;
            }
            MAPCODE map_code = static_cast<MAPCODE>(atoi(temp_str));

            cmd_msg.map_code = map_code;
#endif //_NA_007191_20140428_MODIFY_GM_COMMAND
        }break;

    case cancel_team_reservation:
        {
            cmd_msg.cmd_mode = cancel_team_reservation;
            const TCHAR* char_name_str = cmd_tokens[2].c_str();
            Player* const player = PlayerManager::Instance()->FindPlayerByName(char_name_str);
            if (player == NULL)
            {
                return false;
            }
            cmd_msg.char_guid = player->GetCharGuid();

            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str(),
                cmd_tokens[2].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');

        }break;

    case start:
        {
            cmd_msg.cmd_mode = start;
            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');

#ifdef _NA_007191_20140428_MODIFY_GM_COMMAND
            const char* temp_str = cmd_tokens[2].c_str();
            if (temp_str == NULL) {
                return false;
            }
            MAPCODE map_code = static_cast<MAPCODE>(atoi(temp_str));

            cmd_msg.map_code = map_code;
#endif //_NA_007191_20140428_MODIFY_GM_COMMAND
        }break;

    case enter:
        {
            cmd_msg.cmd_mode = enter;
            int zone_key = 0;
            scoped_string_to_number(cmd_tokens[2], zone_key);
            cmd_msg.zone_key = static_cast<KEYTYPE>(zone_key);

            cmd_msg.cmd_mode = start;
            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');
        }break;

    case victory_point:
        {
            if (player->IsThereBattleGroundZone() == false) {
                return false;
            }

            ChaosZone* chaos_zone = player->GetCurrentChaosZone();
            if (chaos_zone)
            {
                ChaosZoneTeam team_key = kChaosZoneTeamNone;
                const char* team_key_str = cmd_tokens[2].c_str();
                if (strcmp(team_key_str, "레드") == 0 || strcmp(team_key_str, "red") == 0)
                    team_key = kRed;
                if (strcmp(team_key_str, "블루") == 0 || strcmp(team_key_str, "blue") == 0)
                    team_key = kBlue;

                BattleGround* const battle_ground = \
                    static_cast<BattleGround*>(chaos_zone);

                BattleGroundTeam* const team = battle_ground ? battle_ground->GetTeam(team_key) : NULL;
                if (team == NULL) {
                    return false;
                }

                int strong_point = 0;
                scoped_string_to_number(cmd_tokens[3], strong_point);
                team->IncreaseVictroyPoint(strong_point);

                char source[100+1] = {0,};
                ::_sntprintf(source, 101, _T("[%s][%s][%s][%s]"),
                    cmd_tokens[0].c_str(), cmd_tokens[1].c_str(),
                    cmd_tokens[2].c_str(), cmd_tokens[3].c_str());
                source[100] = _T('\0');
                SendGMCmdAck(player, source);
                return true;
            }
        }break;

    case clear:
        {
            cmd_msg.cmd_mode = start;
            ::_sntprintf(cmd_msg.gm_cmd_str, 101, _T("[%s][%s]"),
                cmd_tokens[0].c_str(), cmd_tokens[1].c_str());
            cmd_msg.gm_cmd_str[100] = _T('\0');
        }break;

    }
    player->SendPacket(&cmd_msg, sizeof(cmd_msg));

    return true;
}

#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

#ifdef _NA_008334_20150608_SONNENSCHEIN
static bool ProcessCreateSonnenSchein(DWORD player_key, const GmCmdTokens& cmd_args)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const gm_player = gm_manager->GetGMPlayer(player_key);
    if (gm_player == NULL) {
        return false;
    }

    if (cmd_args.size() == 2)
    {
        const char* cmd_string = cmd_args[1].c_str();

        if ( (strcmp(cmd_string, "생성") == 0 || strcmp(cmd_string, "create") == 0) ||
             (strcmp(cmd_string, "입장") == 0 || strcmp(cmd_string, "enter") == 0)  ||
             (strcmp(cmd_string, "파괴") == 0 || strcmp(cmd_string, "destroy") == 0) )
        {
            MSG_AG_GM_SONNENSCHEIN_CMD cmd_msg;
            strcpy(cmd_msg.gm_cmd, cmd_string);
            
            gm_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        }
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
#endif //_NA_008334_20150608_SONNENSCHEIN

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterZone(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "입장", "in",  &ProcessEntrance), // 입장
        GMHANDLER_MAPPING(true, "방정보", "roominfo",  &ProcessRoomInfo), // 방정보 요청
        GMHANDLER_MAPPING(true, "방제목변경", "changeroominfo",  &ProcessChangeRoomTitle), // 방제목 변경
        GMHANDLER_MAPPING(true, "가기", "go",  &ProcessGo),
        GMHANDLER_MAPPING(true, "추적", "tracking",  &ProcessTracking),
        GMHANDLER_MAPPING(true, "이동", "move",  &ProcessUserMove),
        GMHANDLER_MAPPING(true, NULL, "ssq",  &ProcessSSQCtrlCmd),
    #ifdef _NA_0_20100416_RELOAD_MAPOBJECT
        // unused
        GMHANDLER_MAPPING(false, NULL, NULL,  &ProcessReLoadMapObject),
    #endif
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        GMHANDLER_MAPPING(true, "자유전투실", "freepvp",  &ProcessFreePVPControl),
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        GMHANDLER_MAPPING(true, "전장", "battle_ground", &ProcessBattleGround),
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#ifdef _NA_008334_20150608_SONNENSCHEIN
        GMHANDLER_MAPPING(true, "소넨샤인", "sonnenschein", &ProcessCreateSonnenSchein),
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
