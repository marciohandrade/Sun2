#include "stdafx.h"
#include "UnexpectedClosingMembers.h"
#include <UserSessions/UserManager.h>
#include <AgentParty/AgentParty.h>
#include <Zone/ZoneManager.h>
#include <Zone/RoomInterface.h>

UnexpectedClosingMembers::UCLOSING_USER_LIST::iterator
UnexpectedClosingMembers::_FindChar(const DWORD char_guid)
{
    UCLOSING_USER_LIST::iterator it = m_List.begin(),
                                 end = m_List.end();
    for( ; it != end; ++it)
    {
        sUClosingUser& rUserInfo = *it;
        if(char_guid == rUserInfo.CHAR_GUID)
            return it;
    }

    return end;
}

sUClosingUser*
UnexpectedClosingMembers::FindChar(const DWORD char_guid)
{
    if(m_List.empty() == 0)
    {
        UCLOSING_USER_LIST::iterator itFind = _FindChar(char_guid);
        if(itFind != m_List.end())
        {
            sUClosingUser& rUserInfo = *itFind;
            return &rUserInfo;
        }
    }

    return NULL;
}

BOOL
UnexpectedClosingMembers::UpdateChar(sUClosingUser& rCUser)
{
    if(m_List.empty())
        return FALSE;

    UCLOSING_USER_LIST::iterator itFind = _FindChar(rCUser.CHAR_GUID);
    if(itFind != m_List.end())
    {
        *itFind = rCUser;
        return TRUE;
    }

    return FALSE;
}

void
UnexpectedClosingMembers::AddChar(const DWORD char_guid, const TCHAR* pCharName,
                                  PartyReturnInfo& rInfo)
{
    sUClosingUser uUser(char_guid);
    uUser.SetCharName(pCharName);
    uUser.m_ReturnPos       = rInfo.ReturnPos;
    uUser.m_ReturnFieldCode = rInfo.ReturnFieldCode;
    uUser.m_ReturnZoneType  = rInfo.ReturnZoneType;
    uUser.m_ReturnZoneKey   = rInfo.ReturnZoneKey;
    rInfo.Init();

    m_List.push_back(uUser);
}

void
UnexpectedClosingMembers::RemoveChar(const DWORD char_guid)
{
    UCLOSING_USER_LIST::iterator itFind = _FindChar(char_guid);
    if(itFind != m_List.end())
        m_List.erase(itFind);
}

BOOL
UnexpectedClosingMembers::ChangeStatus(const DWORD char_guid, const IUCM_T::eUSR_STATUS IN rStatus)
{
    UCLOSING_USER_LIST::iterator itFind = _FindChar(char_guid);
    if(itFind != m_List.end())
    {
        sUClosingUser& rUserInfo = *itFind;
        rUserInfo.STATUS = IUCM_T::eUSR_STATUS(rUserInfo.STATUS | rStatus);
        return TRUE;
    }

    return FALSE;
}

BOOL
UnexpectedClosingMembers::SetFirstStatus(const DWORD char_guid)
{
    UCLOSING_USER_LIST::iterator itFind = _FindChar(char_guid);
    if(itFind != m_List.end())
    {
        sUClosingUser& rUserInfo = *itFind;
        rUserInfo.STATUS = IUCM_T::NEW_INSERTED;
        return TRUE;
    }

    return FALSE;
}

void
UnexpectedClosingMembers::Update(AgentParty* const pAgentParty)
{
    UserManager* const pUserManager = UserManager::Instance();
    UCLOSING_USER_LIST::iterator it = m_List.begin(),
                                 end = m_List.end();
    while(it != end)
    {
        sUClosingUser& rUserInfo = *it;
        User* pUCUser = pUserManager->GetUserByCharGuid(rUserInfo.CHAR_GUID);

        if(rUserInfo.TIMEOUT.IsExpired())
        {   // TRUE: 제거해야 한다.
            //  (UPDATE FIX)(070907)(Waverix)
            //  - 비정상 종료된 유저 목록 (클라이언트에서) 보이지 않게 하기)
            //  - 파티 멤버원 전체에게 그 정보를 보내야 한다.
            //  - 하지만 플레이 중인 경우가 아니라면 보내지 않는 편이 나을 듯
            if(pUCUser != 0)
                pUCUser->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);

            MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD msgCMD;
            msgCMD.CopyFrom(rUserInfo.GetCharName());
            pAgentParty->SendPacketAll(&msgCMD, msgCMD.GetSize());

            const _PARTY_MEMBER_INFO* pMemberInfo = pAgentParty->GetHeadUser();
            User* pZonePlayUser = pUserManager->GetUserByObjKey(pMemberInfo->m_dwMemberKey);
            KEYTYPE zone_key = 0;
            if(pZonePlayUser &&
               (zone_key = pZonePlayUser->GetZoneKey()) &&
               zone_key == rUserInfo.m_ReturnZoneKey) // (BUGFIX) (091215) (WAVERIX) 복귀할 방 검증한다.
            {
                ZoneInterface* pZone = g_ZoneManager.FindZone(zone_key);
                if(pZone &&
                   (PolicymentForRoom::MonsterAbilityChangableRoomsBit & (1 << pZone->GetType())))
                {
                    RoomInterface* pRoom = static_cast<RoomInterface*>(pZone);
                    BYTE number_of_players = pRoom->GetNumberOfExpectedPlayers();
                    if(number_of_players)
                    {   // if number_of_players == 0 -> room destroying ~ ed
                        pRoom->SetNumberOfExpectedPlayers(--number_of_players);
                        g_ZoneManager.ChangeBattlePlayersCount(pRoom, number_of_players);
                    }
                }
            }

            // assert( last block on delete )
            it = m_List.erase(it);
            continue;
        }

        // else FALSE
        // 아직 제거되지 않은 경우,
        IUCM_T::eUSR_STATUS& rStatus = rUserInfo.STATUS;
        if(IUCM_T::NEW_INSERTED == rStatus)
        {
            // Unexpected Closing User (UCU) Zone state integrity check
            const DWORD FromZoneAcceptFlagOfUCUser = (1 << eZONETYPE_VILLAGE)
                                                   | (1 << eZONETYPE_DOMINATION_FIELD)
                                                   | (1 << eZONETYPE_FIELD);

            const DWORD FromZoneAcceptStateFlagOfUCUser = (ePRS_AT_VILLAGE & (~ePRS_AT_ZONE))
                                                        | (ePRS_AT_DOMINATION_FIELD & (~ePRS_AT_ZONE))
                                                        | (ePRS_AT_FIELD & (~ePRS_AT_ZONE));
            // Party leader zone state integrity check
            const DWORD ToZoneAcceptFlagOfMaster = (1 << eZONETYPE_MISSION)
                                                 | (1 << eZONETYPE_INSTANCE);

            User* pMasterUser = pUCUser
                              ?     pUserManager->GetUserByObjKey(pAgentParty->GetMasterKey())
                              :     NULL;

            // (WAVERIX) (090625) (REMOVE)
            //User* pUCUser = pUserManager->GetUserByCharGuid(rUserInfo.CHAR_GUID);
            DWORD player_key_of_ucuser = pUCUser ? pUCUser->GetPlayerKey() : 0;

            // (WAVERIX) (090625) (CHANGES)
            BOOLEAN is_accept = pMasterUser && pUCUser && player_key_of_ucuser;
            if(is_accept)
                is_accept = !!(ToZoneAcceptFlagOfMaster & (1 << pMasterUser->GetZoneType()));
            if(is_accept)
                is_accept = !!(FromZoneAcceptFlagOfUCUser & (1 << pUCUser->GetZoneType()));
            if(is_accept)
                is_accept = !!(FromZoneAcceptStateFlagOfUCUser &
                               ((~ePRS_AT_ZONE) & pUCUser->GetZoneState()));

            //if(pMasterUser && pUCUser && player_key_of_ucuser &&
            //   (ToZoneAcceptFlagOfMaster & (1 << pMasterUser->GetZoneType())) &&
            //   (FromZoneAcceptFlagOfUCUser & (1 << pUCUser->GetZoneType())) &&
            //   (FromZoneAcceptStateFlagOfUCUser & ((~ePRS_AT_ZONE) & pUCUser->GetZoneState())))
            if(is_accept)
            {
                if(pUCUser->GetPartyState().GetPartyKey() == 0) // 다른 파티 상태 가능성 제거
                {
                    pUCUser->SetBehaveState(PLAYER_BEHAVE_UNEXPECTED_WAITING);

                    MSG_CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD msgCMD;
                    msgCMD.PARTY_KEY() = pMasterUser->GetPartyState().GetPartyKey();
                    rStatus = IUCM_T::LEADER_REQUEST;
                    pUCUser->SendPacket(&msgCMD, sizeof(msgCMD));
                    return;
                }
                else
                {
                    rUserInfo.TIMEOUT.Instance();   // 목록에서 제거한다.
                }
            }
        }

        // assert( last block )
        ++it;
    }
}

