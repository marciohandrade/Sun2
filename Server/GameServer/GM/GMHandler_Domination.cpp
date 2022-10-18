#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <DominationInfoParser.h>
//
#include "GameZoneManager.h"
#include "GameDominationField.h"

#include "GameGuildManager.h"
#include "GameGuild.h"

#include "GameDominationManager.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool ProcessDomination(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer) 
        return false;

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    enum cmd_mode {scriptmode = 0, time, offense, defense, observer, request_info, 
        exec_reward_event,jointime, max_cmd_mode};

    cmd_mode mode = max_cmd_mode;
    const char* cmd_str = vecCmd[1].c_str();

    if (strcmp(cmd_str, "스크립트모드") == 0 || strcmp(cmd_str, "scriptmode") == 0)
        mode = scriptmode;
    else if (strcmp(cmd_str, "시간") == 0 || strcmp(cmd_str, "time") == 0)
        mode = time;
    else if (strcmp(cmd_str, "공격") == 0 || strcmp(cmd_str, "off") == 0)
        mode = offense;
    else if (strcmp(cmd_str, "방어") == 0 || strcmp(cmd_str, "def") == 0)
        mode = defense;
    else if (strcmp(cmd_str, "옵저버") == 0 || strcmp(cmd_str, "observer") == 0)
        mode = observer;
    else if (strcmp(cmd_str, "정보") == 0 || strcmp(cmd_str, "info") == 0)
        mode = request_info;
    else if (strcmp(cmd_str, "보상이벤트") == 0 || strcmp(cmd_str, "rewardevent") == 0)
        mode = exec_reward_event;
    else if (strcmp(cmd_str, "참여") == 0 || strcmp(cmd_str, "jointime") == 0)
        mode = jointime;
    else
        mode = max_cmd_mode;

    switch(mode)
    {
    case exec_reward_event:
        {   // implemented by _NA_0_20100705_DOMINATION_EVENT
            CHKSendErr(5 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            MAPCODE mapCode = atoi(vecCmd[2].c_str());
            WORD wDayOfWeek = atoi(vecCmd[3].c_str());
            DWORD dwTime = atoi(vecCmd[4].c_str());

            mapCode = DominationInfoParser::Instance()->GetDominationFieldCode(mapCode);
            CHKSendErr(!mapCode, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_NOTEXIST_SCRIPT, pPlayer);

            GameDominationField* field = static_cast<GameDominationField *>(
                g_GameZoneManager.GetFieldVillageZone(mapCode));
            CHKSendErr(!field, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_NOTEXIST_SCRIPT, pPlayer);

            field->GMRewardEventReset(wDayOfWeek, dwTime);
        }
        break;

    case request_info:
        {
            MAPCODE mapCode = 0;

            if (2 < nArgSize)
            {
                mapCode = atoi(vecCmd[2].c_str());
            }
            else
            {
                GameField* pGameField = pPlayer->GetField();
                if (NULL != pGameField)
                    mapCode = pGameField->GetFieldCode();
            }

            mapCode = DominationInfoParser::Instance()->GetDominationFieldCode(mapCode);
            CHKSendErr(!mapCode, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_NOTEXIST_SCRIPT, pPlayer);

            GameDominationField* field = static_cast<GameDominationField *>(
                g_GameZoneManager.GetFieldVillageZone(mapCode));
            CHKSendErr(!field, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_NOTEXIST_SCRIPT, pPlayer);
            const eDOMINATION_EVENT_STATE curEventState = field->GetEventState();

            const char* event_string = 
                curEventState  == DOMINATION_EVENT_AUCTION_START ? "AUCTION_START" 
                : curEventState == DOMINATION_EVENT_AUCTION_START2? "AUCTION_START2"
                : curEventState == DOMINATION_EVENT_SYSTEM_RESET ? "SYSTEM_RESET"
                : curEventState == DOMINATION_EVENT_AUCTION_END  ? "AUCTION_END"
                : curEventState == DOMINATION_EVENT_SELECT_ATTACKGUILD ? "SELECT_ATTACKGUILD"
                : curEventState == DOMINATION_EVENT_SELECT_ATTACKGUILD_END ? "SELECT_ATTACKGUILD_END"
                : curEventState == DOMINATION_EVENT_WAIT  ? "DOMINATION_WAIT"
                : curEventState == DOMINATION_EVENT_START ? "DOMINATION_START"
                : curEventState == DOMINATION_EVENT_END   ? "DOMINATION_END"
                : curEventState == DOMINATION_EVENT_MAX   ? "DOMINATION_MAX_ERR" 
                : "EMPTY";

            const DOMINATION_INFO* pDominationInfo = 
                GameDominationManager::Instance()->GetDominationInfo(mapCode);
            if (pDominationInfo == NULL)
            {
                return false;
            }

            CHKSendErr(!pDominationInfo, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
            INT iLen = _snprintf(msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg),
                "mapcode[%u] event[%s] defense[%s] offense[%s] "
                "reward_event_state[%d] reward_event_day[%d] reward_event_time[%d]",
                pDominationInfo->m_FieldCode, 
                event_string,
                pDominationInfo->m_inGuildInfo.m_tszGuildName,
                pDominationInfo->m_disGuildInfo.m_tszGuildName,
                pDominationInfo->m_rewardEvent.m_byEventStats,
                pDominationInfo->m_rewardEvent.m_wEventDayOfWeek,
                pDominationInfo->m_rewardEvent.m_dwEventTime);
            msgCMD.m_byMsgLength = (BYTE)iLen;
            pPlayer->SendPacket(&msgCMD, msgCMD.GetSize());
            return true;
        }
        break;

    case scriptmode:
        {
            CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            DominationInfoParser::Instance()->GMClearEventInfo();

            MSG_AG_GM_DOMINATION_INIT_CMD cmdMsg;
            cmdMsg.m_flag  = MSG_AG_GM_DOMINATION_INIT_CMD::time;
            cmdMsg.m_bMode = true;
            g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));
        }
        break;

    case time:
        {
            CHKSendErr(5 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
            const WORD auction  = atoi(vecCmd[2].c_str());  //경매시간 더이상 사용되지 않음, 그러나 기존의 명령어와 혼용 가능하도록 수정하지않음
            const DWORD wait    = atoi(vecCmd[3].c_str());  //점령전 준비 시간
            const DWORD war     = atoi(vecCmd[4].c_str());  //점령전 전쟁 시간
            DWORD map_code      = 0;                        //점령전 맵 코드
            
            if (nArgSize >= 6)  //맵 코드가 입력되었을 경우, 특정 맵에서만 점령전 활성화
            {
                map_code = atoi(vecCmd[5].c_str()); 
            }

            DWORD dwTime = 0;
            const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);

            MSG_AG_GM_DOMINATION_INIT_CMD cmdMsg;
            cmdMsg.m_flag  = MSG_AG_GM_DOMINATION_INIT_CMD::time;
            cmdMsg.m_bMode = false;

            sDOMINATION_EVENT_INFO& eventInfo = cmdMsg.m_Info;
            DWORD* pTime = eventInfo.m_dwTime;

            eventInfo.m_wEventIndex = 0;
            eventInfo.m_wMapCode = map_code;
            eventInfo.m_bDominationType = 0;
            eventInfo.m_wDayOfWeek = wDayOfWeek;

            //GM 명령어에서 아래 시간의 사이 간격을 너무 짧게 잡을 경우
            //플레이어 이동중 점령전이 reset되는 경우가 생겨 플레이어가 이전 맵코드를 잃어버리는 현상이 있다.
            *pTime++ = DOMINATION_EVENT_AUCTION_START_TIME;
            *pTime++ = util::CarcHHMMSS(dwTime,DOMINATION_EVENT_AUCTION_END_TIME);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),20);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),5);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),2);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),13);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),wait);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),war);
            *pTime = DOMINATION_EVENT_AUCTION_START2_TIME;
#else
            const DWORD auction = atoi(vecCmd[2].c_str());
            const DWORD wait    = atoi(vecCmd[3].c_str());
            const DWORD war     = atoi(vecCmd[4].c_str());

            DWORD dwTime = 0;
            const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);

            MSG_AG_GM_DOMINATION_INIT_CMD cmdMsg;
            cmdMsg.m_flag  = MSG_AG_GM_DOMINATION_INIT_CMD::time;
            cmdMsg.m_bMode = false;

            sDOMINATION_EVENT_INFO& eventInfo = cmdMsg.m_Info;
            DWORD* pTime = eventInfo.m_dwTime;

            eventInfo.m_wDayOfWeek = wDayOfWeek;

            *pTime++ = 0;
            *pTime++ = util::CarcHHMMSS(dwTime,auction);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),20);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),5);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),2);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),13);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),wait);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),war);
            *pTime = 240000;
#endif

            SUNLOG(eCRITICAL_LOG,  "지역점령전 시간 : A_START[%d] A_END[%d] "
                "SYSTEM[%d] WAIT[%d] START[%d] END[%d] A_START2[%d]", 
                eventInfo.m_dwTime[DOMINATION_EVENT_AUCTION_START],
                eventInfo.m_dwTime[DOMINATION_EVENT_AUCTION_END],
                eventInfo.m_dwTime[DOMINATION_EVENT_SELECT_ATTACKGUILD],
                eventInfo.m_dwTime[DOMINATION_EVENT_SELECT_ATTACKGUILD_END],
                eventInfo.m_dwTime[DOMINATION_EVENT_SYSTEM_RESET],
                eventInfo.m_dwTime[DOMINATION_EVENT_WAIT],
                eventInfo.m_dwTime[DOMINATION_EVENT_START],
                eventInfo.m_dwTime[DOMINATION_EVENT_END],
                eventInfo.m_dwTime[DOMINATION_EVENT_AUCTION_START2]);

            CHKSendErr(!DominationInfoParser::Instance()->GMInsertEventInfo(eventInfo),
                MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

            g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));
        }
        break;
    case jointime:
        {
            CHKSendErr(6 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            const DWORD auction     = atoi(vecCmd[2].c_str());
            const DWORD selectend   = atoi(vecCmd[3].c_str());
            const DWORD wait        = atoi(vecCmd[4].c_str());
            const DWORD war         = atoi(vecCmd[5].c_str());

            DWORD dwTime = 0;
            const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);

            MSG_AG_GM_DOMINATION_INIT_CMD cmdMsg;
            cmdMsg.m_flag  = MSG_AG_GM_DOMINATION_INIT_CMD::time;
            cmdMsg.m_bMode = false;

            sDOMINATION_EVENT_INFO& eventInfo = cmdMsg.m_Info;
            DWORD* pTime = eventInfo.m_dwTime;

            eventInfo.m_wDayOfWeek = wDayOfWeek;

            *pTime++ = 0;
            *pTime++ = util::CarcHHMMSS(dwTime,auction);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),1);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),5);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),selectend);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),10);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),wait);
            *pTime++ = util::CarcHHMMSS(*(pTime-1),war);
            *pTime = 240000;

            CHKSendErr(!DominationInfoParser::Instance()->GMInsertEventInfo(eventInfo),
                       MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

            g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));
        }
        break;

    case offense:
    case defense:
        {
            MAPCODE mapCode = 0;
            TCHAR tszGuildName[MAX_GUILDNAME_LENGTH+1] = {0,};

            if (2 < nArgSize)
            {
                mapCode = atoi(vecCmd[2].c_str());
            }
            else
            {
                GameField* pGameField = pPlayer->GetField();
                if (NULL != pGameField)
                    mapCode = pGameField->GetFieldCode();
            }

            if (3 < nArgSize)
            {
                _tcsncpy(tszGuildName, vecCmd[3].c_str(), MAX_GUILDNAME_LENGTH);
            }
            else
            {
                GameGuild* pGameGuild = g_GameGuildManager.FindGuild(pPlayer->GetGuildGuid());
                CHKSendErr(!pGameGuild, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_USER_STATE_INVALID, pPlayer);

                _tcsncpy(tszGuildName, pGameGuild->GetGuildName(), MAX_GUILDNAME_LENGTH);
            }
            tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

            mapCode = DominationInfoParser::Instance()->GetDominationFieldCode(mapCode);
            CHKSendErr(!mapCode, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_NOTEXIST_SCRIPT, pPlayer);

            const DOMINATION_INFO* pDominationInfo = 
                GameDominationManager::Instance()->GetDominationInfo(mapCode);
            if (pDominationInfo == NULL)
            {
                return false;
            }

            CHKSendErr(!pDominationInfo, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            eDOMINATION_EVENT_TYPE eSetType = DOMINATION_INIT;

            DOMINATION_INFO newInfo;

            ZeroMemory(&newInfo, sizeof(newInfo));
            newInfo.m_FieldCode = mapCode;
            newInfo.m_TotalPoint = 5;

            if (mode == offense)
            {
                eSetType = DOMINATION_GM_OFFENCE_GUILD_SET;
                newInfo.m_inGuildInfo = pDominationInfo->m_inGuildInfo;
                _tcsncpy(newInfo.m_disGuildInfo.m_tszGuildName, tszGuildName, MAX_GUILDNAME_LENGTH);
            }
            else
            {
                eSetType = DOMINATION_GM_DEFENCE_GUILD_SET;
                newInfo.m_disGuildInfo = pDominationInfo->m_disGuildInfo;
                _tcsncpy(newInfo.m_inGuildInfo.m_tszGuildName, tszGuildName, MAX_GUILDNAME_LENGTH);
            }

            GameDominationManager::Instance()->SendUpdateDominationInfo(newInfo, eSetType);
        }
        break;

    case observer:
        {
            CHKSendErr(3 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

            const char* cmd_str = vecCmd[2].c_str();

            BOOL is_observer_mode;
            if (strcmp(cmd_str, "켬") == 0 || strcmp(cmd_str, "on") == 0)
            {
                is_observer_mode = true;
            }
            else if (strcmp(cmd_str, "끔") == 0 || strcmp(cmd_str, "off") == 0)
            {
                is_observer_mode = false;
            }
            else
            {
                _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);
                return false;
            }

            MSG_AG_GM_DOMINATION_INIT_CMD cmdMsg;
            cmdMsg.m_flag  = MSG_AG_GM_DOMINATION_INIT_CMD::observer;
            cmdMsg.m_bMode = is_observer_mode;
            g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));
        }
        break;

    default:
        _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);
        return false;
    }

    SendGMCmdAck(pPlayer, vecCmd[0].c_str());

    return true;
}

}; //end of namespace 'nsGM'

bool GMCmdManager::RegisterDomination(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "지역점령전", "domi", &ProcessDomination), // 지역 점령전 트리거 리셋
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
