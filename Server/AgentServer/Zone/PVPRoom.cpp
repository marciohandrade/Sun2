#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>

//////////////////////////////////////////////////////////////////////////
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <UserSessions/User.h>

#include <Zone/PVPRoom.h>
#include <Zone/LobbyRoom.h>
//_NA_20110630_BATTLE_ZONE_RENEWAL
#include <MapInfoParser.h>
#include <MapPartialLoadingParser.h>
#include <AgentParty/AgentPartyManager.h>
#include <BaseParser.h>


_IMPL_POOL(PVPRoom)
PVPRoom::PVPRoom(void)
{
	m_Link2ndPartyKey = 0;
}

PVPRoom::~PVPRoom(void)
{
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
RC::eROOM_RESULT
PVPRoom::_SetBaseRoomInfo(User* user, 
                          BASE_ROOMINFO& base_room_info, 
                          MAPCODE map_code)
{
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL) 
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if(map_info->byMKind != eZONETYPE_PVP)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    base_room_info.m_ClassLimit = map_info->byClass;
    base_room_info.m_MinLV = static_cast<BYTE>(map_info->minLV);
    base_room_info.m_MaxLV = static_cast<BYTE>(map_info->maxLV);    

    if (user->CanApplyPCBangBenefit(TRUE))
    {
        base_room_info.m_SpecialType = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        base_room_info.m_SpecialType = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
PVPRoom::Create(User* user, 
                MAPCODE map_code, 
                KEYTYPE mission_key, 
                ServerSession* server_session, 
                const ADDITIONAL_PVPINFO* additional_pvp_info)
{
    BASE_ROOMINFO base_room_info;
    RC::eROOM_RESULT result_code = _SetBaseRoomInfo(user, base_room_info, map_code);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    SetMaster(user);
    LinkParty(user->GetPartyState().GetPartyKey());
    SetAdditionalPVPInfo(additional_pvp_info);

    result_code = _CanCreate(user, map_code, base_room_info, eZONETYPE_PVP);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;    
    }

    RoomInterface::Create(user, mission_key, map_code, eZONEPUBLIC_PUBLIC, 
                          "", "", base_room_info, server_session);
    On_create();

    if (_SendPVPInfo(user) == false)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : SendPVPInfo fail : user name : %s",
            user->GetSelectedCharName()
            );
        return RC::RC_ROOM_FAILED;
    }

    return result_code;
}

bool 
PVPRoom::_SendPVPInfo(User* user)
{
    WORD party_key = user->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const BaseParty* base_party = party_manager->FindParty(party_key);
    if (base_party == NULL)
    {
        return false;
    }

    MSG_AG_ZONE_PVP_INFO_CMD cmd_msg;
    cmd_msg.m_RoomKey = GetKey();
    cmd_msg.m_Count	= 0;

    UserManager* const user_manager = UserManager::Instance();

    typedef BaseParty::MEMBER_MAP MemberList;
    const MemberList member_list = base_party->GetMemberList();

    MemberList::const_iterator begin_iterator = member_list.begin(),
                               end_iterator = member_list.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const _PARTY_MEMBER_INFO& member_info = begin_iterator->second;
        User* current_user = user_manager->GetUserByObjKey(member_info.m_dwMemberKey);
        if (current_user == NULL)
        {
            continue;
        }

        cmd_msg.m_Info[cmd_msg.m_Count].m_dwPlayerKey = current_user->GetPlayerKey();
        cmd_msg.m_Info[cmd_msg.m_Count].m_Team = current_user->GetTeam();
        ++cmd_msg.m_Count;

        if (cmd_msg.m_Count >= _countof(cmd_msg.m_Info))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Member count is over : %d", 
                cmd_msg.m_Count
                );
            break;
        }
    }

    SendToLinkedServer(&cmd_msg, cmd_msg.GetSize());
    return true;
}

RC::eROOM_RESULT 
PVPRoom::_CanCreate(User* user, 
                    MAPCODE map_code, 
                    const BASE_ROOMINFO& base_room_info, 
                    eZONETYPE zone_type)
{
    if (GetRoomState() != eROOMSTATE_DESTROY)
    {
        return RC::RC_ROOM_INVALIDPREVLOBBY;
    }

    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (map_info->GetZoneType4SharedCode() != zone_type)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    MAP_PARTIAL_LOADING* loading_info = NULL;
    loading_info = MapPartialLoadingParser::Instance()->FindMapInfo(map_info->MapKind);
    if (loading_info && !(loading_info->m_bLoading))
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (base_room_info.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
    {
        if (user->CanApplyPCBangBenefit(TRUE) == FALSE)
        {
            return RC::RC_ROOM_ISNOT_PCBANG_USER;
        }
    }
    else if (base_room_info.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
    {
        if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER) == FALSE)
        {
            return RC::RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM;
        }
    }

    bool is_valid_limit_level = (base_room_info.m_MinLV <= base_room_info.m_MaxLV) &&
                                (map_info->minLV        <= base_room_info.m_MinLV) && 
                                (base_room_info.m_MaxLV <= map_info->maxLV);

    if (is_valid_limit_level == false)
    {
        return RC::RC_ROOM_INVALID_LIMITLEVEL;
    }

    LEVELTYPE user_level = user->GetSelectedCharLV();
    is_valid_limit_level = base_room_info.m_MinLV <= user_level &&
                           user_level <= base_room_info.m_MaxLV;
    if (is_valid_limit_level == false)
    {
        return RC::RC_ROOM_INVALID_LIMITLEVEL;
    }

    for(int i = 0; i < eCHAR_TYPE_MAX - 1; ++i)
    {
        if((map_info->byClass & (eCLASS_BERSERKER << i)) == 0)
        {
            if(base_room_info.m_ClassLimit & (eCLASS_BERSERKER << i))
            {
                return RC::RC_ROOM_INVALID_LIMITCLASS;
            }
        }
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT 
PVPRoom::Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession )
{
	if( pPreviousRoom->GetType() != eZONETYPE_PVP && pPreviousRoom->GetType() != eZONETYPE_LOBBY )
		return RC::RC_ROOM_INVALIDROOMTYPE;

	RoomInterface::Migrate( key, pPreviousRoom, pServerSession );

	if( pPreviousRoom->GetType() == eZONETYPE_PVP )
	{
		PVPRoom * pRoom = (PVPRoom *)pPreviousRoom;
		SetAdditionalPVPInfo( pRoom->GetAdditionalPVPInfo() );
	}
	else
	{
		LobbyRoom * pRoom = (LobbyRoom *)pPreviousRoom;
		SetAdditionalPVPInfo( pRoom->GetAdditionalPVPInfo() );
	}

	On_create();

	// 릴레이 유저 등록
	RelayUser(pPreviousRoom);

	//---------------------------------------------------------------
	// PVP 정보 전송
	MSG_AG_ZONE_PVP_INFO_CMD cmsg;
	cmsg.m_RoomKey	= GetKey();
	cmsg.m_Count	= 0;

	const ZONE_USER_LIST& rList = pPreviousRoom->GetUserList();
	FOREACH_CONTAINER( const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST )
	{
		User* const pUser = rNode.second;
		cmsg.m_Info[cmsg.m_Count].m_dwPlayerKey = pUser->GetPlayerKey();
		cmsg.m_Info[cmsg.m_Count].m_Team		= pUser->GetTeam();
		++cmsg.m_Count;

		if( MAX_PARTYMEMBER_NUM <= cmsg.m_Count )	// upper-bound check
			break;
	}

	ASSERT( MAX_PARTYMEMBER_NUM >= rList.size() );
	SendToLinkedServer( &cmsg, cmsg.GetSize() );
	//---------------------------------------------------------------

	//pPreviousRoom->SetReusedKey(TRUE);

	return RC::RC_ROOM_SUCCESS;
}

VOID
PVPRoom::ReadyUser(User* pUser, eSEND_TYPE type,
                   MAPCODE field_code, DWORD areaID, WzVector* pwvStartPos, bool bFirstJoin)
{
    // (CHANGES) (091207) (WAVERIX) PvP도 정해진 타일로 선택이 가능하도록 설정 하기 위한 선행 처리
    const BYTE teamNo = pUser->GetTeam();
    if(areaID == 0 && teamNo != 0)
    {
        const sMAPINFO* pMapInfo = GetMapInfo();
        assert(pMapInfo);
        // (NOTE) 값이 없으면 기존대로 처리가 될 것이다.
        areaID = teamNo == 1 ? pMapInfo->wStartAreaID : pMapInfo->wStartAreaID2;
    }
    return RoomInterface::ReadyUser(pUser, type, field_code, areaID, pwvStartPos, bFirstJoin);

}

VOID 
PVPRoom::On_create()
{
	RoomInterface::On_create();

	MSG_AG_ZONE_PVP_CREATE_CMD msg;
	msg.m_dwKey = GetMaster()->GetUserKey();
	msg.m_MapCode = GetMapCode();
	msg.m_RoomKey = GetKey();
	memcpy( msg.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
	memcpy( msg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH*sizeof(TCHAR) );
	msg.m_RoomInfo = *GetRoomInfo();									//< 생성할 방 정보
	msg.m_AdditionalPVPInfo = *GetAdditionalPVPInfo();
	SendToLinkedServer( &msg, sizeof(msg) );

}

VOID 
PVPRoom::On_destroy()
{
	if( !IsReusedServerSession() )
		ServerSessionManager::Instance()->Decr_LOF_BattleServer( GetLinkedServer()->GetSessionIndex() );

	RoomInterface::On_destroy();

	m_Link2ndPartyKey = 0;
}
VOID 
PVPRoom::On_readyuser( User * pUser )
{
	RoomInterface::On_readyuser(pUser);

	pUser->SetZoneState( ePRS_BEFORE_ENTER_PVP );
	pUser->SetZoneKey( GetKey() );
	pUser->SetZoneType( eZONETYPE_PVP );
}

VOID 
PVPRoom::On_enteruser( User * pUser )
{
	RoomInterface::On_enteruser(pUser);
	pUser->SetZoneState( ePRS_AT_PVP );
}

VOID 
PVPRoom::On_leaveuser( User * pUser )
{
	RoomInterface::On_leaveuser(pUser);
    pUser->SetTeam(0); // (CHANGES) (f100406.1L) clear pvp section
}
//
//VOID PVPRoom::SerializeLoad( PVP_ROOMINFO & OUT rRoomInfo )
//{
//	rRoomInfo.m_Key					= GetKey();
//	rRoomInfo.m_MapCode				= GetMapCode();
//	memcpy( rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
//	rRoomInfo.m_byRoomPublic		= GetZonePublic();
//	rRoomInfo.m_CurNumberOfPlayers	= (BYTE)GetCurUserNum();
//	rRoomInfo.m_RoomInfo			= *GetRoomInfo();
//	rRoomInfo.m_AdditionalPVPInfo	= *GetAdditionalPVPInfo();
//}

RC::eROOM_RESULT 
PVPRoom::_CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD )
{
	// 카오상태 체크
	if(!GetServerOptChaoStateUseZone(GetType()) && pUser->IsChaoState())
	{
		return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
	}

#if defined( __NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)	// 길드전은 pvp룸이 아님
	if( IsSpecialType( eROOM_SPECIAL_GUILD_PVP ) )
	{
		RC::eROOM_RESULT rcResult = CanJoinGuildPVP( pUser );
		if( rcResult != RC::RC_ROOM_SUCCESS )
			return rcResult;
	}
#endif

	return RC::RC_ROOM_SUCCESS;
}

#if defined( __NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)	//길드전은 pvp룸이 아님
RC::eROOM_RESULT PVPRoom::CanJoinGuildPVP( User * pUser )
{
	// 길드 소속 체크
	if( !pUser->GetSelectedCharGuildGuid() )
		return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD;

	// 방장과 같은 길드인지 체크
	if( GetMaster() )
	{
		if( GetMaster()->GetSelectedCharGuildGuid() != pUser->GetSelectedCharGuildGuid() )
			return RC::RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD;
	}

	// 입장권 수량 체크
	const sMAPINFO * pInfo = MapInfoParser::Instance()->FindMapInfo( GetMapCode() );
	if( pUser->GetItemOwnFlag().GetItemNum( eITEMTYPE_GUILD_PVP_ENTRANCE ) < pInfo->EntranceItemCount )
		return RC::RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP;

	return RC::RC_ROOM_SUCCESS;
}
#endif