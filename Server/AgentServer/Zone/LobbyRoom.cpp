#include "stdafx.h"
#include "./LobbyRoom.h"

//==================================================================================================
#include <PacketStruct_AG.h>
#include <PacketStruct_CG.h>
#include <MapInfoParser.h>
#include <RequireFieldLevelInfoParser.h>
#include <ServerOptionParser.h>
#include <MatchInfoParser.h>
//==================================================================================================
#include <UserSessions/User.h>
#include "ZoneManager.h"
#include "CHuntingRoom.h"
#include "Village.h"
#include "ServerOptionParserEx.h"
#include <ProhibitionWordParser.h>

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

//==================================================================================================
//==================================================================================================
//==================================================================================================

_IMPL_POOL(LobbyRoom);

//==================================================================================================

LobbyRoom::LobbyRoom(void)
{
}

LobbyRoom::~LobbyRoom(void)
{
}

VOID
LobbyRoom::On_create()
{
    RoomInterface::On_create();

    MSG_AG_ZONE_LOBBY_CREATE_CMD msg;
    msg.m_dwKey     = GetMaster()->GetUserKey();
    msg.m_MapCode   = GetMapCode();
    msg.m_RoomKey   = GetKey();
    memcpy(msg.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH);
    memcpy(msg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH);
    msg.m_RoomInfo  = *GetRoomInfo();                                    //< 생성할 방 정보
    SendToLinkedServer(&msg, sizeof(msg));

    // 경쟁헌팅 링크 로비방이라면, 다시 링크를 걸지 못하도록 플래그를 셋팅한다.
    if(GetLobbyType() == eZONETYPE_CHUNTING)
    {
        CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(m_AdditionalCHuntingExInfo.m_LinkHuntingKey);
        if(pCHunting)
        {
            pCHunting->SetLinkedLobby(TRUE);
        }
    }

    m_WaitTimer.SetTimer(0);
}

VOID
LobbyRoom::On_destroy()
{
    // 경쟁헌팅 링크 로비방이라면, 다시 링크를 걸수 있도록 플래그를 셋팅한다.
    if(GetLobbyType() == eZONETYPE_CHUNTING)
    {
        CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(m_AdditionalCHuntingExInfo.m_LinkHuntingKey);
        if(pCHunting)
        {
            pCHunting->SetLinkedLobby(FALSE);
        }
    }

    RoomInterface::On_destroy();
}

VOID
LobbyRoom::On_readyuser(User* pUser)
{
    RoomInterface::On_readyuser(pUser);
    pUser->SetZoneState(ePRS_BEFORE_ENTER_LOBBY);
    pUser->SetZoneKey(GetKey());
    pUser->SetZoneType(eZONETYPE_LOBBY);

    // 팀배정
    AssignTeam(pUser);
}

VOID
LobbyRoom::On_enteruser(User* pUser)
{
    RoomInterface::On_enteruser(pUser);
    pUser->SetZoneState(ePRS_AT_LOBBY);
}

VOID
LobbyRoom::On_leaveuser(User* pUser)
{
    eZONETYPE eFromZoneType, eToZoneType;
    DWORD ToLinkServerIndex;
    pUser->GetMoveZoneType(eFromZoneType, eToZoneType, ToLinkServerIndex);

    if(0 == GetUserNum())
    {
        SetRoomState(eROOMSTATE_DESTROY);
    }
    else if(BOOLEAN bMaster = GetMaster() == pUser)
    {
        if(eToZoneType != eZONETYPE_INSTANCE &&
           eToZoneType != eZONETYPE_MISSION &&
           eToZoneType != eZONETYPE_PVP && // (CHANGES) (WAVERIX) prevent change master in pvp
           eToZoneType != eZONETYPE_CHUNTING)
        {
            // 방장이면 위임
            VestUserWithMaster(GetFirstUser());

            // 슈퍼방 아이템을 소지한 자가 없으면 슈퍼방 기능이 해제된다.
            if(bMaster)
            {
                User* pNextMaster = RoomInterface::ChangeSpecialType(false, pUser);
                if(pNextMaster)
                {
                    AgentPartyManager::Instance()->ChangeMaster(pNextMaster->GetPartyState().GetPartyKey(),
                                                                pNextMaster->GetPlayerKey());
                }
            }
        }
    }

    if (pUser->GetPartyState().IsPartyMember())
    {
        AgentPartyManager::Instance()->LeaveZone(pUser, this);
    }

    pUser->SetReady(FALSE); // 유저 초기화
    // (CHANGES) Leave Lobby -> Enter PvP시에 팀 설정 초기화 방지 처리
    if(eToZoneType != eZONETYPE_PVP)
        pUser->SetTeam(0);
}

RC::eROOM_RESULT
LobbyRoom::ChangeMap(MAPCODE mapCode)
{
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
    if(pInfo == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;

    MapGroup* pGroup = MapInfoParser::Instance()->GetMapGroup(pInfo->MapKind);
    sMAPINFO* pChangingMapInfo = pGroup->GetMapInfoCode(mapCode);
    if(pChangingMapInfo == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {
        if(!pInfo->EntranceItemCode)
            return RC::RC_ROOM_INVALID_MAPCODE;
    }
#endif

    SetMapCode(mapCode);

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
LobbyRoom::CanCreate(User* pUser, MAPCODE mapCode, eZONETYPE eZoneType,
                     eZONEPUBLIC ePublic, BASE_ROOMINFO& IN baseRoomInfo, KEYTYPE linkHuntingKey)
{
    // 길드 PVP 모드라면
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(baseRoomInfo.IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {
        // 길드 소속 체크
        if(!pUser->GetSelectedCharGuildGuid())
            return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD;

        // PVP가 가능한 맵인지 체크
        const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
        if(!pInfo)
            return RC::RC_ROOM_INVALID_MAPCODE;
        if(!pInfo->EntranceItemCode)
            return RC::RC_ROOM_INVALID_MAPCODE;

        // 입장권 수량 체크
        if(pUser->GetItemOwnFlag().GetItemNum(eITEMTYPE_GUILD_PVP_ENTRANCE) < pInfo->EntranceItemCount)
            return RC::RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP;
    }
#endif

    //.// 카오 체크

    if(!GetServerOptChaoStateUseZone(eZoneType) && pUser->IsChaoState())
    {
        return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    if(eZoneType == eZONETYPE_CHUNTING)
    {
        // 기존 경쟁헌팅방에 조인하는 로비 생성인 경우
        if(linkHuntingKey)
        {
            CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(linkHuntingKey);
            if(!pCHunting)
                return RC::RC_ROOM_INVALID_CHUNTING_LINK_KEY;

            // 이미 링크 로비방이 만들어 졌는지 체크
            if(pCHunting->GetLinkedLobby())
                return RC::RC_ROOM_ALREADY_EXIST_LINKED_LOBBY;

            // 경쟁중인 헌팅방은 들어갈 수 없다.
            if(pCHunting->IsCompetition())
                return RC::RC_ROOM_DOING_COMPETITION;

            // 들어오려는 파티원수가 경쟁헌팅방의 멤버수보다 적어야 한다.
            if(AgentPartyManager::Instance()->GetMemberNum(pUser->GetPartyState().GetPartyKey()) > pCHunting->GetCurUserNum())
                return RC::RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY;
        }
    }
    else if(eZoneType == eZONETYPE_MISSION)
    {
        // 미션방을 최고 수치보다 높은지 체크.
        if(baseRoomInfo.m_MaxLV > (pUser->GetSelectedCharLV() + MISSION_LEVEL_JOIN_OFFSET))
            return RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
    }

    return RoomInterface::CanCreate(pUser, mapCode, eZoneType, ePublic, baseRoomInfo);
}

RC::eROOM_RESULT
LobbyRoom::_CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD)
{
    // 카오상태 체크
    if(!GetServerOptChaoStateUseZone(GetType()) && pUser->IsChaoState())
    {
        return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    // (WAVERIX) (090522) (CHANGES)
    //const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(GetMapCode());
    const sMAPINFO* pInfo = GetMapInfo();
    if(!pInfo) return RC::RC_ROOM_INVALID_MAPCODE;

    return RC::RC_ROOM_SUCCESS;
}

BOOL
LobbyRoom::CanShowAtRoomList(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD)
{
    // 파티원들이 들어올 수 있도록 기다려준다.
    if(!m_WaitTimer.IsExpired(FALSE))
        return FALSE;

    return RoomInterface::CanShowAtRoomList(pUser, RoomPublic, pszRoomPWD);
}

RC::eROOM_RESULT
LobbyRoom::Create(User* pUser, KEYTYPE key, MAPCODE mapCode, 
                  eZONETYPE eLobbyType, eZONEPUBLIC ePublic, TCHAR* pszRoomTitle, TCHAR* pszRoomPWD, 
                  BASE_ROOMINFO & IN baseRoomInfo,
                  ServerSession* pServerSession, KEYTYPE linkHuntingKey)
{

    if(eLobbyType != eZONETYPE_HUNTING && eLobbyType != eZONETYPE_MISSION && eLobbyType != eZONETYPE_PVP && eLobbyType != eZONETYPE_CHUNTING) 
        return RC::RC_ROOM_INVALID_ROOM_TYPE;

    // 제대로 된 미션, 헌팅, pVP맵 코드가 맞는지 체크

    RC::eROOM_RESULT rt = CanCreate(pUser, mapCode, eLobbyType, ePublic, baseRoomInfo, linkHuntingKey);

    // 유저 상태 검사
    if(rt != RC::RC_ROOM_SUCCESS) return rt;
    // 인자 유효성 검사
    
    // PVP일경우 체크
    if(eLobbyType == eZONETYPE_PVP)
    {
        // 10레벨이상
        if(pUser->GetSelectedCharLV() < 10)            return RC::RC_ROOM_NEED_CHAR_LEVEL_10_FOR_PVP;
    }
    // 미션의 경우 방장보다 10LV 이상은 못들어오게 한다.
    else if(eLobbyType == eZONETYPE_MISSION)
    {
        if(baseRoomInfo.m_MaxLV > (pUser->GetSelectedCharLV()+MISSION_LEVEL_JOIN_OFFSET))
            baseRoomInfo.m_MaxLV = pUser->GetSelectedCharLV() + MISSION_LEVEL_JOIN_OFFSET;
    }

    if (pszRoomTitle[0] == '\0') {
        return RC::RC_ROOM_CANNOT_USE_ROOM_TITLE;
    };

    util::StrArrayForRoomTitle room_title;
    _tcsncpy(room_title, pszRoomTitle, _countof(room_title));
    room_title[_countof(room_title) - 1] = _T('\0');
    if (ProhibitionWordParser::Instance()->CheckFilter(room_title) == false) {
        return RC::RC_ROOM_CANNOT_USE_ROOM_TITLE;
    };

    // 정보 복사 -----------------------------------------------------
    RoomInterface::Create(pUser, key, mapCode,
                          ePublic, pszRoomTitle, pszRoomPWD, baseRoomInfo, pServerSession);

    SetLobbyType(eLobbyType);
    // 부가정보 기본 값으로 셋팅
    m_AdditionalRoomInfo.m_Difficulty   = eHUNTING_MONSTER_DIFFICULT_EASY;
    m_AdditionalRoomInfo.m_Bonus        = eHUNTING_BONUS_TYPE_ITEM;
    m_AdditionalPVPInfo.m_Rule          = ePVP_RULE_10;
    m_AdditionalPVPInfo.m_Mode          = ePVP_PERSONAL_MODE;

    m_AdditionalCHuntingExInfo.m_Difficulty = eHUNTING_MONSTER_DIFFICULT_EASY;
    m_AdditionalCHuntingExInfo.m_Bonus      = eHUNTING_BONUS_TYPE_ITEM;
    m_AdditionalCHuntingExInfo.m_Rule       = eCOMPETITION_RULE_DUAL_PARTY;
    m_AdditionalCHuntingExInfo.m_Mode       = eCOMPETITION_MODE_TIME_ATTACK;

    m_AdditionalCHuntingExInfo.m_LinkHuntingKey = linkHuntingKey;

    // 유저
    eROOMSTATE_TYPE type =  GetRoomState();
    SetRoomState(eROOMSTATE_GENERNAL);
    rt = CanJoin(pUser, GetZonePublic(), GetRoomPWD());
    SetRoomState(type);
    if(rt != RC::RC_ROOM_SUCCESS)
        return rt;

    // 최대인원수 제한을 기존 경쟁헌팅방과 같게한다.
    CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(linkHuntingKey);
    if(pCHunting)
    {
        SetLimitMaxUserNum((BYTE)pCHunting->GetCurUserNum());
        SetAdditionalCHuntingInfo(const_cast<ADDITIONAL_CHUNTINGINFO*>(pCHunting->GetAdditionalCHuntingInfo()));
    }

    On_create();

    return rt;
}

VOID
LobbyRoom::SerializeLoad(HUNTING_ROOMINFO & OUT rRoomInfo)
{
    rRoomInfo.m_Key      = GetKey();
    rRoomInfo.m_MapCode  = GetMapCode();
    memcpy(rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR));
    rRoomInfo.m_byRoomPublic    = GetZonePublic();
    rRoomInfo.m_CurUserNum      = (BYTE)GetCurUserNum();
    rRoomInfo.m_MaxLimitUserNum = GetLimitMaxUserNum();
    rRoomInfo.m_RoomInfo        = *GetRoomInfo();
    rRoomInfo.m_AdditionalInfo  = *GetAdditionalInfo();
}

VOID
LobbyRoom::SerializeLoad(MISSION_ROOMINFO & OUT rRoomInfo)
{
    ASSERT(GetLobbyType() == eZONETYPE_MISSION);
    rRoomInfo.m_Key          = GetKey();
    rRoomInfo.m_MapCode      = GetMapCode();
    rRoomInfo.m_byRoomPublic = GetZonePublic();
    memcpy(rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR));
    rRoomInfo.m_CurUserNum      = (BYTE)GetUserNum();
    rRoomInfo.m_MaxLimitUserNum = GetLimitMaxUserNum();
    rRoomInfo.m_RoomInfo        = *GetRoomInfo();
}

VOID
LobbyRoom::SerializeLoad(PVP_ROOMINFO & OUT rRoomInfo)
{
    rRoomInfo.m_Key     = GetKey();
    rRoomInfo.m_MapCode = GetMapCode();
    memcpy(rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR));
    rRoomInfo.m_byRoomPublic        = GetZonePublic();
    rRoomInfo.m_CurUserNum          = (BYTE)GetUserNum();
    rRoomInfo.m_MaxLimitUserNum     = GetLimitMaxUserNum();
    rRoomInfo.m_RoomInfo            = *GetRoomInfo();
    rRoomInfo.m_AdditionalPVPInfo   = *GetAdditionalPVPInfo();
}

VOID
LobbyRoom::SerializeLoad(CHUNTING_ROOMINFO & OUT rRoomInfo)
{
    rRoomInfo.m_Key      = GetKey();
    rRoomInfo.m_MapCode  = GetMapCode();
    memcpy(rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR));
    rRoomInfo.m_byRoomPublic    = GetZonePublic();
    rRoomInfo.m_CurUserNum      = (BYTE)GetUserNum();
    rRoomInfo.m_MaxLimitUserNum = GetLimitMaxUserNum();
    rRoomInfo.m_RoomInfo        = *GetRoomInfo();
    memcpy(&rRoomInfo.m_AdditionalInfo, GetAdditionalCHuntingInfo(), sizeof(ADDITIONAL_CHUNTINGINFO));
}

BOOL
LobbyRoom::IsAllReady()
{
    User* const pMaster = GetMaster();
    const ZONE_USER_LIST& rList = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        if(pMaster && (pMaster == pUser))
            continue;
        if(!pUser->IsReady())
            return FALSE;
    }
    return TRUE;
}

BOOL
LobbyRoom::IsPrepareUserNum()
{
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(GetMapCode());
    if(!pInfo)
        return FALSE;

    // 최소인원 제한 체크
    if(GetUserNum() < pInfo->byMinUserNum)
        return FALSE;

    // 최대인원 제한 체크
    if(GetUserNum() > GetLimitMaxUserNum())
        return FALSE;

    if(GetLobbyType() == eZONETYPE_PVP && IsPVPTeamMode())
    {    // 팀전
        //    (WAVERIX)(080726)(NOTE)    개인적인 의견일 지 모르겠지만, DEBUG|RELEASE WIN-WIN 정책을 사용하고자 하면
        //    const BYTE teamNo = pUser->GetTeam();와 같이 하는 편이 더 나을 것이다.
        //    뭐, 팀의 규약이 기존과 같은 형태로 정했다면 할 수 없지만, GetTeam이라는 interface는 const 함수임을 우리는 알지만,
        //    컴파일러는 알기 힘들다는 점과 더 나은 코드가 생성되기 힘들게 된다는 점을 알아두자.
        //    팀 번호 1,2는 사람이 인식하는 코드라는 점에서... 뭐라 따지기는 힘들다. 극악이 아닌 이상 용인하는 편이 나을 듯.
        const BYTE teamUpperbound    = 3;    // 0 is skip no
        BYTE teams[teamUpperbound]    = { 0, };
        const ZONE_USER_LIST& rList    = GetUserList();
        FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
        {
            User* const pUser = rNode.second;
            const BYTE teamNo = pUser->GetTeam();
            if(teamNo && teamNo<teamUpperbound)    // team1, team2
                ++teams[teamNo];
        }

        return (teams[1] && teams[2]);
    }

    return TRUE;
}

VOID
LobbyRoom::AssignTeam(User* pTargetUser)
{
    const BOOLEAN isPvPZone = GetLobbyType() == eZONETYPE_PVP;
    const BOOLEAN isPvPTeamMode = !!IsPVPTeamMode();

    const ZONE_USER_LIST& rList = GetUserList();

    if(pTargetUser)
    {
        if(!(isPvPZone && isPvPTeamMode))
        {
            pTargetUser->SetTeam(0);    // 0으로 초기화 한다... 문제 없는지 타전해 볼 것.
            return;
        }

        const BYTE teamUpperbound = 3;    // 0 is skip no
        BYTE teams[teamUpperbound] = { 0, };
        // 팀원이 이미 들어 있다.
        FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
        {
            User* const pUser = rNode.second;
            const BYTE teamNo = pUser->GetTeam();
            if(teamNo && teamNo<teamUpperbound)    // team1, team2
                ++teams[teamNo];
        }

        const BYTE selectedTeam = (teams[1] > teams[2])
            ?    2
            :    1
            ;
        pTargetUser->SetTeam(selectedTeam);
    }
    else
    {    // 팀 전...?
        if(!isPvPZone)
            return;

        BYTE toggle = 0;
        //    (WAVERIX)(080726)(NOTE) - 흐음... 효율성 문제는 떠나서 가독성 문제는 따져봐야 할 듯...
        //    내가 보기엔 그게 그거지만... 다른 사람의 눈에는 어캐 보이는 가... 모르겠다.
        FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
        {
            const BYTE selectedTeam = !isPvPTeamMode
                ?    0
                :    (!(toggle++ & 1)
                        ?    1
                        :    2
                    )
                ;

            User* const pUser = rNode.second;
            pUser->SetTeam(selectedTeam);
        }
    }
}

User*
LobbyRoom::Get2ndTeamMaster()
{
    User* pMaster = GetMaster();
    if(!pMaster)    return NULL;

    const ZONE_USER_LIST& rList = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        if(pUser->GetTeam() != pMaster->GetTeam())
            return pUser;
    }

    return NULL;
}

BOOL
LobbyRoom::RecommendLevel(LEVELTYPE LV)
{
    BYTE byDiff = 99;
    switch(GetLobbyType())
    {
    case eZONETYPE_PVP: return RoomInterface::ValidLevel(LV);
    case eZONETYPE_MISSION: break;
    case eZONETYPE_HUNTING: byDiff = GetAdditionalInfo()->m_Difficulty; break;
    default: return FALSE;
    }

    BASE_RequireFieldLevelInfo* pFieldLVInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(GetMapCode(), byDiff);
    if(!pFieldLVInfo) return FALSE;

    if(pFieldLVInfo->m_wMinlv <= LV && LV <= pFieldLVInfo->m_wMaxlv)
        return TRUE;

    return FALSE;
}

VOID
LobbyRoom::Invite(User* pUser)
{
    switch(GetLobbyType())
    {
    case eZONETYPE_HUNTING:
        {
            MSG_CG_ZONE_HUNTINGLOBBY_JOIN_CMD cmsg;
            SerializeLoad(cmsg.m_RoomInfo);
            if(GetZonePublic() == eZONEPUBLIC_PRIVATE)
                memcpy(cmsg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH);
            pUser->SendPacket(&cmsg, cmsg.GetSize());
        }
        break;
    case eZONETYPE_MISSION:
        {
            MSG_CG_ZONE_MISSIONLOBBY_JOIN_CMD cmsg;
            SerializeLoad(cmsg.m_RoomInfo);
            if(GetZonePublic() == eZONEPUBLIC_PRIVATE)
                memcpy(cmsg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH);
            pUser->SendPacket(&cmsg, cmsg.GetSize());
        }
        break;
    case eZONETYPE_PVP:
        {
            MSG_CG_ZONE_PVPLOBBY_JOIN_CMD cmsg;
            SerializeLoad(cmsg.m_RoomInfo);
            if(GetZonePublic() == eZONEPUBLIC_PRIVATE)
                memcpy(cmsg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH);
            pUser->SendPacket(&cmsg, cmsg.GetSize());
        }
        break;
    case eZONETYPE_CHUNTING:
        {
            MSG_CG_ZONE_CHUNTINGLOBBY_JOIN_CMD cmsg;
            SerializeLoad(cmsg.m_RoomInfo);
            if(GetZonePublic() == eZONEPUBLIC_PRIVATE)
                memcpy(cmsg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH);
            pUser->SendPacket(&cmsg, cmsg.GetSize());
        }
        break;
    }
}

VOID
LobbyRoom::ExpelAll()
{
    //  (WAVERIX)(080726)(NOTE)
    //  VAssistX or VStudio 쪽의 인식 패턴의 문제일까...? 아래와 같이 하는 게 brace 분석에 문제가 없다.
    const ZONE_USER_LIST& rList = GetUserList();
    ZONE_USER_LIST::const_iterator it = rList.begin(),
                                   itend = rList.end();
    while(it != itend)
    {
        User* const pUser = it->second;
        ++it;

        MAPCODE mapCode = pUser->GetSelectedCharMapCode();
        ZoneInterface* pZone = g_ZoneManager.FindStaticZone(mapCode);
        if(pZone == 0)
            continue;

        BOOLEAN isAcceptable = pZone->GetType() == eZONETYPE_VILLAGE ||
                               pZone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD;
        if(isAcceptable == 0)
            continue;
        //const sMAPINFO* pMAPINFO = MapInfoParser::Instance()->FindMapInfo(mapCode);
        //if(!((eZONETYPE_VILLAGE == pMAPINFO->byMKind) || (eZONETYPE_TUTORIAL_FIELD == pMAPINFO->byMKind)))
        //    continue;

        // 로비 나감 CMD
        LeaveUser(pUser, eSEND_CMD, eZONETYPE_VILLAGE, pZone->GetLinkedServerIndex());

        // 마을 들어감 CMD
        pZone->ReadyUser(pUser, eSEND_CMD);

        // 로비에서 나간다 패킷 보냄
        MSG_CG_ZONE_LOBBY_LEAVE_ACK msg;
        pUser->SendPacket(&msg, sizeof(msg));
    }
}

BOOL
LobbyRoom::CheckAbusing(CHuntingRoom* pCHuntingRoom)
{
    if(!pCHuntingRoom)
        return FALSE;

    BYTE byLimitCount = 5;
    BASE_MatchInfo* pMatchInfo = MatchInfoParser::Instance()->GetMatchInfo(MATCH_ABUSE_JOINTIMES);
    if(pMatchInfo)    byLimitCount = pMatchInfo->m_byMatchLimitCount;

    const ZONE_USER_LIST& rList = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        //    같은 방에 특정시간에 특정회수 이상의 조인을 막는다.
        //    (WAVERIX)(080726)(NOTE) 뭔지 모르지만 흥미가 생기게 만든다. 나중에 이 주석을 보면 어떻게 구현했는지 살펴보자.
        const WORD wJoinCount = pCHuntingRoom->GetPlayerHistoryNum(pUser->GetUserGUID());
        if(wJoinCount > byLimitCount)
            return TRUE;
    }

    return FALSE;
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
RC::eROOM_RESULT
LobbyRoom::CanMigrateToChunting()
{
    // 이 함수는.. 실제로는 GUILD PVP 체크 함수이다... 
    // 따라서 길드 PVP가 아니면 넘어 가는 것을 한다.
    if(!GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
        return RC::RC_ROOM_SUCCESS;

    // 길드 PVP에서는 서바이벌 모드를 지원하지 않는다.
    if(GetAdditionalCHuntingInfo()->m_Mode == eCOMPETITION_MODE_SURVIVAL)
        return RC::RC_ROOM_INVALID_MODE_VALUE;

    // 길드 PVP가 가능한 맵인지 체크
    const sMAPINFO* pInfo = GetMapInfo();
    if(pInfo == 0) // (WAVERIX) 이 시점에는 이미 선행 검사가 되었으리라 생각되지만...
        return RC::RC_ROOM_INVALID_MAPCODE;
    if(pInfo->EntranceItemCode == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;

    // DualParty Rule인지 체크
    if(m_AdditionalCHuntingExInfo.m_Rule != eCOMPETITION_RULE_DUAL_PARTY)
        return RC::RC_ROOM_INVALID_RULE_VALUE;

    if(ServerOptionParserEx::Instance()->GetServerOption().m_byGuildPVPUserCount != GetUserNum())
        return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT_USER;

    const ZONE_USER_LIST& rList = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        const RC::eROOM_RESULT ret = CanJoinGuildPVP(pUser);
        if(ret != RC::RC_ROOM_SUCCESS)
            return ret;
    }

    return RC::RC_ROOM_SUCCESS;
}
#endif

BOOL
LobbyRoom::ChangeRoomTitle(const TCHAR* pszRoomTitle)
{
    if (pszRoomTitle[0] == '\0') {
        return false;
    };

    util::StrArrayForRoomTitle room_title;
    _tcsncpy(room_title, pszRoomTitle, _countof(room_title));
    room_title[_countof(room_title) - 1] = _T('\0');

    if (ProhibitionWordParser::Instance()->CheckFilter(room_title) == false) {
        return false;
    };
    SetRoomTitle(room_title);

    return true;
}

// CHANGES: f110608.6L, this method added to reply against CG_ZONE_LOBBY_REQUEST_INFO_SYN
bool LobbyRoom::SendLobbyInfoForRequest(User* requester) const
{
    uint8_t shared_buffer[max(sizeof(MSG_CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK),
                          max(sizeof(MSG_CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK),
                          max(sizeof(MSG_CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK), 0)))];
    switch (m_LobbyType)
    {
    case eZONETYPE_MISSION:
        if (MSG_CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK* const msg_ack =
                new (shared_buffer) MSG_CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK)
        {
            msg_ack->m_MapCode = this->GetMapCode();
            msg_ack->m_NonBlockNum = this->GetLimitMaxUserNum();
            //
            const ns_functor::LobbyChangedStatusNotificationFilter filter;
            msg_ack->m_Count = this->GetListMemberInfo(
                filter.GetRedirectedArray(msg_ack->m_dwReadyPlayerKey), requester);
            //
            requester->SendPacket(msg_ack, msg_ack->GetSize());
        };
        break;
    //case eZONETYPE_HUNTING: <- don't support anymore since ... when is this type removed?
    case eZONETYPE_PVP:
        if (MSG_CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK* const msg_ack =
                new (shared_buffer) MSG_CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK)
        {
            msg_ack->m_MapCode = this->GetMapCode();
            msg_ack->m_AdditionalPVPInfo = this->m_AdditionalPVPInfo;
            msg_ack->m_NonBlockNum = this->GetLimitMaxUserNum();
            //
            const ns_functor::PvPLobbyChangedStatusNotificationFilter filter;
            msg_ack->m_Count = this->GetListMemberInfo(//  no checked requester
                filter.GetRedirectedArray(msg_ack->m_PlayerInfo), NULL);
            //
            requester->SendPacket(msg_ack, msg_ack->GetSize());
            if (this->IsPVPTeamMode())
            {
                MSG_CG_ZONE_LOBBY_PVP_INFO_BRD msg_brd;
                msg_brd.m_dwPlayerKey = requester->GetPlayerKey();
                msg_brd.m_Team = requester->GetTeam();
                this->SendToAll(&msg_brd, sizeof(msg_brd), requester->GetUserKey());
            };
        };
        break;
    case eZONETYPE_CHUNTING:
        if (MSG_CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK* const msg_ack =
                new (shared_buffer) MSG_CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK)
        {
            msg_ack->m_MapCode = this->GetMapCode();
            msg_ack->m_AdditionalCHuntingInfo = this->m_AdditionalCHuntingExInfo;
            msg_ack->m_NonBlockNum = this->GetLimitMaxUserNum();

            const ns_functor::LobbyChangedStatusNotificationFilter filter;
            msg_ack->m_Count = this->GetListMemberInfo(
                filter.GetRedirectedArray(msg_ack->m_dwReadyPlayerKey), requester);
            //
            requester->SendPacket(msg_ack, msg_ack->GetSize());
        };
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected lobby type %d|"),
               m_LobbyType);
        return false;
    };
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// CHANGES: f110609.3L, it added to make shared part data.
// returns: number of aligned information
uint8_t LobbyRoom::GetListMemberInfo(
    ns_functor::LobbyChangedStatusNotificationFilter::MemberInfoArray& result_array,
    User* requester) const
{
    const ns_functor::LobbyChangedStatusNotificationFilter filter;
    BOOST_STATIC_ASSERT(_countof(result_array) == MAX_PARTYMEMBER_NUM);
    uint16_t* dest_it = result_array;
    int number_of_aligned = 0;
    //
    const ZONE_USER_LIST& user_list = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& node, user_list, ZONE_USER_LIST)
    {
        User* const user = node.second;
        if (filter.Check(user, requester) == false) {
            continue;
        };
        //
        *dest_it = static_cast<uint16_t>(user->GetPlayerKey());
        ++dest_it;
        //
        number_of_aligned = static_cast<int>(dest_it - result_array);
        if (number_of_aligned >= _countof(result_array)) {
            break;
        }
    };
    return static_cast<uint8_t>(number_of_aligned);
}

uint8_t LobbyRoom::GetListMemberInfo(
    ns_functor::PvPLobbyChangedStatusNotificationFilter::MemberInfoArray& result_array,
    User* requester) const
{
    __TOUCHED(requester);
    const ns_functor::PvPLobbyChangedStatusNotificationFilter filter;
    BOOST_STATIC_ASSERT(_countof(result_array) == MAX_PARTYMEMBER_NUM);
    PVP_LOBBY_INFO* dest_it = result_array;
    int number_of_aligned = 0;
    //
    const ZONE_USER_LIST& user_list = GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& node, user_list, ZONE_USER_LIST)
    {
        User* const user = node.second;
        //             no checked requester
        if (filter.Check(user, NULL) == false) {
            continue;
        };
        //
        dest_it->m_Team = user->GetTeam();
        dest_it->m_Ready = (user->IsReady() != false);
        dest_it->m_dwPlayerKey = static_cast<uint16_t>(user->GetPlayerKey());
        ++dest_it;
        //
        number_of_aligned = static_cast<uint8_t>(dest_it - result_array);
        if (number_of_aligned >= _countof(result_array)) {
            break;
        }
    };

    return static_cast<uint8_t>(number_of_aligned);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
