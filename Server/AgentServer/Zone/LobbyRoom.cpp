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
    msg.m_RoomInfo  = *GetRoomInfo();                                    //< ������ �� ����
    SendToLinkedServer(&msg, sizeof(msg));

    // �������� ��ũ �κ���̶��, �ٽ� ��ũ�� ���� ���ϵ��� �÷��׸� �����Ѵ�.
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
    // �������� ��ũ �κ���̶��, �ٽ� ��ũ�� �ɼ� �ֵ��� �÷��׸� �����Ѵ�.
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

    // ������
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
            // �����̸� ����
            VestUserWithMaster(GetFirstUser());

            // ���۹� �������� ������ �ڰ� ������ ���۹� ����� �����ȴ�.
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

    pUser->SetReady(FALSE); // ���� �ʱ�ȭ
    // (CHANGES) Leave Lobby -> Enter PvP�ÿ� �� ���� �ʱ�ȭ ���� ó��
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
    // ��� PVP �����
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(baseRoomInfo.IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {
        // ��� �Ҽ� üũ
        if(!pUser->GetSelectedCharGuildGuid())
            return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD;

        // PVP�� ������ ������ üũ
        const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
        if(!pInfo)
            return RC::RC_ROOM_INVALID_MAPCODE;
        if(!pInfo->EntranceItemCode)
            return RC::RC_ROOM_INVALID_MAPCODE;

        // ����� ���� üũ
        if(pUser->GetItemOwnFlag().GetItemNum(eITEMTYPE_GUILD_PVP_ENTRANCE) < pInfo->EntranceItemCount)
            return RC::RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP;
    }
#endif

    //.// ī�� üũ

    if(!GetServerOptChaoStateUseZone(eZoneType) && pUser->IsChaoState())
    {
        return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    if(eZoneType == eZONETYPE_CHUNTING)
    {
        // ���� �������ù濡 �����ϴ� �κ� ������ ���
        if(linkHuntingKey)
        {
            CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(linkHuntingKey);
            if(!pCHunting)
                return RC::RC_ROOM_INVALID_CHUNTING_LINK_KEY;

            // �̹� ��ũ �κ���� ����� ������ üũ
            if(pCHunting->GetLinkedLobby())
                return RC::RC_ROOM_ALREADY_EXIST_LINKED_LOBBY;

            // �������� ���ù��� �� �� ����.
            if(pCHunting->IsCompetition())
                return RC::RC_ROOM_DOING_COMPETITION;

            // �������� ��Ƽ������ �������ù��� ��������� ����� �Ѵ�.
            if(AgentPartyManager::Instance()->GetMemberNum(pUser->GetPartyState().GetPartyKey()) > pCHunting->GetCurUserNum())
                return RC::RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY;
        }
    }
    else if(eZoneType == eZONETYPE_MISSION)
    {
        // �̼ǹ��� �ְ� ��ġ���� ������ üũ.
        if(baseRoomInfo.m_MaxLV > (pUser->GetSelectedCharLV() + MISSION_LEVEL_JOIN_OFFSET))
            return RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
    }

    return RoomInterface::CanCreate(pUser, mapCode, eZoneType, ePublic, baseRoomInfo);
}

RC::eROOM_RESULT
LobbyRoom::_CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD)
{
    // ī������ üũ
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
    // ��Ƽ������ ���� �� �ֵ��� ��ٷ��ش�.
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

    // ����� �� �̼�, ����, pVP�� �ڵ尡 �´��� üũ

    RC::eROOM_RESULT rt = CanCreate(pUser, mapCode, eLobbyType, ePublic, baseRoomInfo, linkHuntingKey);

    // ���� ���� �˻�
    if(rt != RC::RC_ROOM_SUCCESS) return rt;
    // ���� ��ȿ�� �˻�
    
    // PVP�ϰ�� üũ
    if(eLobbyType == eZONETYPE_PVP)
    {
        // 10�����̻�
        if(pUser->GetSelectedCharLV() < 10)            return RC::RC_ROOM_NEED_CHAR_LEVEL_10_FOR_PVP;
    }
    // �̼��� ��� ���庸�� 10LV �̻��� �������� �Ѵ�.
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

    // ���� ���� -----------------------------------------------------
    RoomInterface::Create(pUser, key, mapCode,
                          ePublic, pszRoomTitle, pszRoomPWD, baseRoomInfo, pServerSession);

    SetLobbyType(eLobbyType);
    // �ΰ����� �⺻ ������ ����
    m_AdditionalRoomInfo.m_Difficulty   = eHUNTING_MONSTER_DIFFICULT_EASY;
    m_AdditionalRoomInfo.m_Bonus        = eHUNTING_BONUS_TYPE_ITEM;
    m_AdditionalPVPInfo.m_Rule          = ePVP_RULE_10;
    m_AdditionalPVPInfo.m_Mode          = ePVP_PERSONAL_MODE;

    m_AdditionalCHuntingExInfo.m_Difficulty = eHUNTING_MONSTER_DIFFICULT_EASY;
    m_AdditionalCHuntingExInfo.m_Bonus      = eHUNTING_BONUS_TYPE_ITEM;
    m_AdditionalCHuntingExInfo.m_Rule       = eCOMPETITION_RULE_DUAL_PARTY;
    m_AdditionalCHuntingExInfo.m_Mode       = eCOMPETITION_MODE_TIME_ATTACK;

    m_AdditionalCHuntingExInfo.m_LinkHuntingKey = linkHuntingKey;

    // ����
    eROOMSTATE_TYPE type =  GetRoomState();
    SetRoomState(eROOMSTATE_GENERNAL);
    rt = CanJoin(pUser, GetZonePublic(), GetRoomPWD());
    SetRoomState(type);
    if(rt != RC::RC_ROOM_SUCCESS)
        return rt;

    // �ִ��ο��� ������ ���� �������ù�� �����Ѵ�.
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

    // �ּ��ο� ���� üũ
    if(GetUserNum() < pInfo->byMinUserNum)
        return FALSE;

    // �ִ��ο� ���� üũ
    if(GetUserNum() > GetLimitMaxUserNum())
        return FALSE;

    if(GetLobbyType() == eZONETYPE_PVP && IsPVPTeamMode())
    {    // ����
        //    (WAVERIX)(080726)(NOTE)    �������� �ǰ��� �� �𸣰�����, DEBUG|RELEASE WIN-WIN ��å�� ����ϰ��� �ϸ�
        //    const BYTE teamNo = pUser->GetTeam();�� ���� �ϴ� ���� �� ���� ���̴�.
        //    ��, ���� �Ծ��� ������ ���� ���·� ���ߴٸ� �� �� ������, GetTeam�̶�� interface�� const �Լ����� �츮�� ������,
        //    �����Ϸ��� �˱� ����ٴ� ���� �� ���� �ڵ尡 �����Ǳ� ����� �ȴٴ� ���� �˾Ƶ���.
        //    �� ��ȣ 1,2�� ����� �ν��ϴ� �ڵ��� ������... ���� ������� �����. �ؾ��� �ƴ� �̻� �����ϴ� ���� ���� ��.
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
            pTargetUser->SetTeam(0);    // 0���� �ʱ�ȭ �Ѵ�... ���� ������ Ÿ���� �� ��.
            return;
        }

        const BYTE teamUpperbound = 3;    // 0 is skip no
        BYTE teams[teamUpperbound] = { 0, };
        // ������ �̹� ��� �ִ�.
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
    {    // �� ��...?
        if(!isPvPZone)
            return;

        BYTE toggle = 0;
        //    (WAVERIX)(080726)(NOTE) - ����... ȿ���� ������ ������ ������ ������ �������� �� ��...
        //    ���� ���⿣ �װ� �װ�����... �ٸ� ����� ������ ��ĳ ���̴� ��... �𸣰ڴ�.
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
    //  VAssistX or VStudio ���� �ν� ������ �����ϱ�...? �Ʒ��� ���� �ϴ� �� brace �м��� ������ ����.
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

        // �κ� ���� CMD
        LeaveUser(pUser, eSEND_CMD, eZONETYPE_VILLAGE, pZone->GetLinkedServerIndex());

        // ���� �� CMD
        pZone->ReadyUser(pUser, eSEND_CMD);

        // �κ񿡼� ������ ��Ŷ ����
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
        //    ���� �濡 Ư���ð��� Ư��ȸ�� �̻��� ������ ���´�.
        //    (WAVERIX)(080726)(NOTE) ���� ������ ��̰� ����� �����. ���߿� �� �ּ��� ���� ��� �����ߴ��� ���캸��.
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
    // �� �Լ���.. �����δ� GUILD PVP üũ �Լ��̴�... 
    // ���� ��� PVP�� �ƴϸ� �Ѿ� ���� ���� �Ѵ�.
    if(!GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
        return RC::RC_ROOM_SUCCESS;

    // ��� PVP������ �����̹� ��带 �������� �ʴ´�.
    if(GetAdditionalCHuntingInfo()->m_Mode == eCOMPETITION_MODE_SURVIVAL)
        return RC::RC_ROOM_INVALID_MODE_VALUE;

    // ��� PVP�� ������ ������ üũ
    const sMAPINFO* pInfo = GetMapInfo();
    if(pInfo == 0) // (WAVERIX) �� �������� �̹� ���� �˻簡 �Ǿ������� ����������...
        return RC::RC_ROOM_INVALID_MAPCODE;
    if(pInfo->EntranceItemCode == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;

    // DualParty Rule���� üũ
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
