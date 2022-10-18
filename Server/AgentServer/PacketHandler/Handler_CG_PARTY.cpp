#include "stdafx.h"
#include "Handler_FromClient.h"
//
#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>
//
#include <ServerSessions/ServerSessionEx.h>
#include <Zone/ZoneManager.h>
#include <Zone/RoomInterface.h>

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include <AgentParty/PartyRoomFinder.h>
#include <AgentParty/PartyRoomList.h>
#include "..\Party\PartyRoom.h"
#include "BattlezoneMissionTreeParser.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "../ChaosZoneSystem/AgentChaosZoneManager.h"
#include <ChaosZoneSystem/BattleGroundManager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


//파티가입 요청
HANDLER_CG_IMPL( CG_PARTY_JOIN_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_JOIN_SYN* recv_msg = (MSG_CG_PARTY_JOIN_SYN*)pMsg;

	//파티요청자
	User* request_user = pUser;

	//파티요청 대상
	TCHAR target_user_name[MAX_CLIENT_CHARNAME_LENGTH]={0,};
	_tcsncpy(target_user_name, recv_msg->m_TargetCharName, _countof(target_user_name));
    target_user_name[_countof(target_user_name) - 1] = '\0';

	User* target_user = UserManager::Instance()->FindUserForCharName(target_user_name);

    DWORD party_member_count = 
        AgentPartyManager::Instance()->GetMemberNum(request_user->GetPartyState().GetPartyKey());

    //파티 유무 상관없이 최소치 0, 최대치 10 유지...할 것.
    BOOLEAN is_acceptable_count = party_member_count < MAX_PARTYMEMBER_NUM;

	if (target_user && is_acceptable_count)
	{
        //_NA_0_20110210_INVITE_PARTY_BUG
        RC::ePARTY_RESULT result = AgentPartyManager::Instance()->RequestInviteParty(request_user, target_user);
        if (result != RC::RC_PARTY_SUCCESS)
        {
            MSG_CG_PARTY_JOIN_NAK nak_msg;
            nak_msg.m_byErrorCode = result;
            request_user->SendPacket(&nak_msg, sizeof(nak_msg));
            return;
        }

		MSG_AG_PARTY_JOIN_SYN send_msg;
        {
            //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
            send_msg.m_dwFlag = MSG_AG_PARTY_JOIN_SYN::ENABLE_REQUEST;
            send_msg.m_dwReqMemberKey = request_user->GetPlayerKey();
            send_msg.m_dwRecvMemberKey = target_user->GetPlayerKey();
        }
		request_user->SendToLinkedServer(&send_msg, sizeof(send_msg));
	}
	else
	{
		MSG_CG_PARTY_JOIN_NAK nak_msg;
        {
            nak_msg.m_byErrorCode = is_acceptable_count ? RC::RC_PARTY_PLAYER_NOTEXIST
                                                        : RC::RC_PARTY_PARTYMEMBER_NUM_FULL;
        }
		request_user->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}
}

// 파티가입 수락
HANDLER_CG_IMPL( CG_PARTY_JOIN_RESPONSE_ACK )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_JOIN_RESPONSE_ACK* recv_msg = 
        static_cast<MSG_CG_PARTY_JOIN_RESPONSE_ACK*>(pMsg);

	// 파티요청 대상
	User* response_user = pUser;

    //_NA_0_20110210_INVITE_PARTY_BUG
    RC::ePARTY_RESULT result = AgentPartyManager::Instance()->ResponseInviteParty(response_user);
    if (result != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_JOIN_NAK nak_message;
        nak_message.m_byErrorCode = result;
        response_user->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

	// 파티요청자
    UserManager* user_manager = UserManager::Instance();
	User* request_user = 
        user_manager->GetUserByObjKey(response_user->GetPartyState().GetOtherMemberKey());
	if (request_user == NULL)
    {
        return;
    }

    //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
    MSG_AG_PARTY_JOIN_SYN msg;
    msg.m_dwFlag = MSG_AG_PARTY_JOIN_SYN::ENABLE_RESPONSE;
    msg.m_dwReqMemberKey = request_user->GetPlayerKey();
    msg.m_dwRecvMemberKey = response_user->GetPlayerKey();
    request_user->SendToLinkedServer(&msg, sizeof(msg));
}

// 파티가입 거절
HANDLER_CG_IMPL( CG_PARTY_JOIN_RESPONSE_NAK )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_JOIN_RESPONSE_NAK* recv_msg = (MSG_CG_PARTY_JOIN_RESPONSE_NAK*)pMsg;

	// 파티요청 대상
	User* pResponseUser = (User*)pUser;

    //_NA_0_20110210_INVITE_PARTY_BUG
    RC::ePARTY_RESULT result = AgentPartyManager::Instance()->ResponseInviteParty(pResponseUser);
    if (result != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_JOIN_NAK nak_message;
        nak_message.m_byErrorCode = result;
        pResponseUser->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

	// 파티요청자
	User* pRequestUser = UserManager::Instance()->GetUserByObjKey( pResponseUser->GetPartyState().GetOtherMemberKey() );
	if( !pRequestUser )
    {
        return;
    }


	// 상대방 UserKey를 초기화한다.
	AgentPartyManager::Instance()->ResetOtherMemberKey( pRequestUser, pResponseUser );

	// 상대방이 파티요청에 거절했음을 알린다.
	MSG_CG_PARTY_JOIN_NAK msgNAK;
	msgNAK.m_byErrorCode = RC::RC_PARTY_REJECT_INVITEMENT;
	pRequestUser->SendPacket( &msgNAK, sizeof(msgNAK) );
}

// 파티탈퇴 요청
HANDLER_CG_IMPL( CG_PARTY_LEAVE_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_LEAVE_SYN* pRecvMsg = (MSG_CG_PARTY_LEAVE_SYN*)pMsg;

	User* pLeaveUser = (User*)pUser;

	// (WAVERIX) 임시 코드 - 중국에서 발생한 로비에서의 파티 탈퇴문제 방지 목적
	if( pLeaveUser->GetZoneType() == eZONETYPE_LOBBY
		|| (ePRS_AT_ZONE != (ePRS_AT_ZONE & pLeaveUser->GetZoneState()))
		)
	{
		MSG_CG_PARTY_LEAVE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_PARTY_INVALID_ZONESTATE;
		pLeaveUser->SendPacket( &msgNAK, sizeof(msgNAK) );
		return;
	}

	// 파티탈퇴 조건이 되는지 체크한 후 처리한다.
	RC::ePARTY_RESULT rcResult = AgentPartyManager::Instance()->LeaveParty( pLeaveUser, eREASON_LEAVE_PARTY_INTEND );
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_LEAVE_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pLeaveUser->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

// 강제추방 요청
HANDLER_CG_IMPL( CG_PARTY_FORCED_EXPULSION_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_FORCED_EXPULSION_SYN* pRecvMsg = (MSG_CG_PARTY_FORCED_EXPULSION_SYN*)pMsg;

	User* pMaster = (User*)pUser;

	User* pLeaveUser = UserManager::Instance()->GetUserByObjKey( pRecvMsg->m_dwObjKey );
	if( !pLeaveUser )
		return;

	// 파티탈퇴 조건이 되는지 체크한 후 처리한다.
	RC::ePARTY_RESULT rcResult = AgentPartyManager::Instance()->ForcedExpulsion( pMaster, pLeaveUser );
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_LEAVE_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pMaster->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

// 파티장 변경 요청
HANDLER_CG_IMPL( CG_PARTY_CHANGE_MASTER_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_CHANGE_MASTER_SYN* pRecvMsg = (MSG_CG_PARTY_CHANGE_MASTER_SYN*)pMsg;

	User* pMaster = (User*)pUser;

	RC::ePARTY_RESULT rcResult = RC::RC_PARTY_SUCCESS;

	User* pNextMaster = UserManager::Instance()->GetUserByObjKey( pRecvMsg->m_dwObjKey );
	if( !pNextMaster )
	{
		rcResult = RC::RC_PARTY_NEXTMASTER_NOTEXIST;
		goto ERROR_RESULT;
	}

	if( !AgentPartyManager::Instance()->IsMaster( pMaster ) )
	{
		rcResult = RC::RC_PARTY_ISNOT_MASTEROFPARTY;
		goto ERROR_RESULT;
	}

    KEYTYPE zoneKey = pMaster->GetZoneKey();
    ZoneInterface* pZone = g_ZoneManager.FindZone(zoneKey);
    if(pZone == 0)
    {
        rcResult = RC::RC_PARTY_CANNOT_CHANGE_MASTER;
        goto ERROR_RESULT;
    }

	rcResult = AgentPartyManager::Instance()->ChangeMaster( pMaster->GetPartyState().GetPartyKey(), pNextMaster->GetPlayerKey() );
    if(rcResult == RC::RC_PARTY_SUCCESS)
        static_cast<RoomInterface*>(pZone)->ChangeSpecialType(true, pNextMaster);

ERROR_RESULT:
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_CHANGE_MASTER_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pMaster->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

// 파티해체 요청
HANDLER_CG_IMPL( CG_PARTY_DESTROY_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_DESTROY_SYN* pRecvMsg = (MSG_CG_PARTY_DESTROY_SYN*)pMsg;

	User* pMaster = (User*)pUser;

	RC::ePARTY_RESULT rcResult = RC::RC_PARTY_SUCCESS;

	if( !AgentPartyManager::Instance()->IsMaster( pMaster ) )
	{
		rcResult = RC::RC_PARTY_ISNOT_MASTEROFPARTY;
		goto ERROR_RESULT;
	}

	rcResult = AgentPartyManager::Instance()->DestroyParty( pMaster->GetPartyState().GetPartyKey() );

ERROR_RESULT:
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_DESTROY_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pMaster->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

// 파티타겟지정 요청
HANDLER_CG_IMPL( CG_PARTY_SELECT_TARGET_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_SELECT_TARGET_SYN* pRecvMsg = (MSG_CG_PARTY_SELECT_TARGET_SYN*)pMsg;

	User* pMaster = (User*)pUser;

	// 파티타겟지정 조건이 되는지 체크한 후 처리한다.
	RC::ePARTY_RESULT rcResult = AgentPartyManager::Instance()->SelectTarget( pMaster, pRecvMsg->m_dwObjectKey );
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_SELECT_TARGET_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pMaster->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

// 미니맵포인팅 
HANDLER_CG_IMPL( CG_PARTY_MINIMAP_POINTING_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_MINIMAP_POINTING_SYN* pRecvMsg = (MSG_CG_PARTY_MINIMAP_POINTING_SYN*)pMsg;

	RC::ePARTY_RESULT rcResult = AgentPartyManager::Instance()->SelectMinimap( pUser, pRecvMsg->m_fx, pRecvMsg->m_fy );
	if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_MINIMAP_POINTING_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

HANDLER_CG_IMPL( CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN* pRecvMsg = (MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN*)pMsg;
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

	RC::ePARTY_RESULT rcResult = party_manager->ChangeDistributionType( pUser, (eITEM_DISTRIBUTION_TYPE)pRecvMsg->m_byItemDistributionType );

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    WORD party_key = pUser->GetPartyState().GetPartyKey();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room)
    {
        if (party_room->GetIsMissionStart() == false)
        {
            party_manager->ResetPartyRoomReadyState(pUser, party_key);
        }
    }

    if( rcResult != RC::RC_PARTY_SUCCESS )
	{
		MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
HANDLER_CG_IMPL(CG_PARTY_REQUEST_RENDER_INFO_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_REQUEST_RENDER_INFO_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_REQUEST_RENDER_INFO_SYN*>(pMsg);

    if (pUser->GetPartyState().IsPartyMember() == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Is not party member : %s", 
               pUser->GetSelectedCharName());
        return;
    }

    MSG_AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD cmd_msg;
    cmd_msg.request_party_key_ = recv_msg->request_party_key_;
    cmd_msg.request_user_key_ = pUser->GetPlayerKey();
    cmd_msg.target_user_key_ = recv_msg->request_member_key_;

    pUser->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
}

HANDLER_CG_IMPL(CG_PARTY_STANDBY_PAUSE_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_STANDBY_PAUSE_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_STANDBY_PAUSE_SYN*>(pMsg);

    pUser->SetIsRecvInvitePartyRoom(recv_msg->set_value_);

    MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK ack_msg;
    pUser->SendPacket(&ack_msg, sizeof(ack_msg));
}

HANDLER_CG_IMPL(CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* const party_room_list = party_manager->GetPartyRoomList();
    const PartyRoom* party_room = party_room_list->FindPartyRoomByRoomKey(recv_msg->find_room_key_);
    if (party_room == NULL)
    {
        MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_FOUND_PARTYROOM;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK ack_msg;
    ack_msg.result_code_ = RC::RC_PARTY_SUCCESS;
    ack_msg.party_room_info_ = *(party_room->GetPartyRoomInfo());
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}


HANDLER_CG_IMPL(CG_PARTY_FIND_ROOM_BY_CHARID_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_FIND_ROOM_BY_CHARID_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_FIND_ROOM_BY_CHARID_SYN*>(pMsg);

    TCHAR find_char_name[MAX_CHARNAME_LENGTH+1];
    _tcsncpy(find_char_name, recv_msg->find_char_name_, _countof(find_char_name));
    find_char_name[_countof(find_char_name)+1] = '\0';

    User* find_user = UserManager::Instance()->FindUserForCharName(find_char_name);
    if (find_user == NULL)
    {
        MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_FOUND_USER;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    WORD party_key = find_user->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_FOUND_PARTYROOM;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK ack_msg;
    ack_msg.result_code_ = RC::RC_PARTY_SUCCESS;
    ack_msg.party_room_info_ = *(party_room->GetPartyRoomInfo());
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}


HANDLER_CG_IMPL(CG_PARTY_REQUEST_MEMBER_INFO_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_REQUEST_MEMBER_INFO_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_REQUEST_MEMBER_INFO_SYN*>(pMsg);

    //AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    WORD party_key = pUser->GetPartyState().GetPartyKey();
    if (party_key == 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : User is not with a party");
        return;
    }

    MSG_AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD cmd_msg;
    cmd_msg.party_key_ = party_key;
    cmd_msg.member_key_ = pUser->GetPlayerKey();
    pUser->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
}


HANDLER_CG_IMPL(CG_PARTY_SELECT_TEAM_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_SELECT_TEAM_SYN* recv_msg = static_cast<MSG_CG_PARTY_SELECT_TEAM_SYN*>(pMsg);
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

    WORD party_key = pUser->GetPartyState().GetPartyKey();
    pUser->SetTeam(recv_msg->select_team_);
    party_manager->SetMemberTeam(pUser, recv_msg->select_team_);

    //gamelulu's note : 현재 상태에서는 실패할 수 있는 로직이 존재 하지 않는다.(2011.09.22)
    //하지만 앞으로 여러 제한 사항이 발생할 여지가 존재 하기 때문에 요청 응답 패킷을 구성한다.
    MSG_CG_PARTY_SELECT_TEAM_ACK ack_msg;
    ack_msg.result_code_ = RC::RC_PARTY_SUCCESS;
    pUser->SendPacket(&ack_msg, sizeof(ack_msg));

    MSG_CG_PARTY_SELECT_TEAM_BRD brd_msg;
    brd_msg.member_key_ = pUser->GetPlayerKey();
    brd_msg.select_team_ = recv_msg->select_team_;
    party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}


HANDLER_CG_IMPL(CG_PARTY_ROOM_START_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_ROOM_START_SYN* recv_msg = static_cast<MSG_CG_PARTY_ROOM_START_SYN*>(pMsg);

    WORD party_key = pUser->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               __FUNCTION__" : Not found party room : %d",
               party_key);
        return;
    }

    party_manager->SetMemberReadyState(pUser, true);

    RC::ePARTY_RESULT result_code = party_manager->CanStartRoom(pUser);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Fail start mission : %d",
               result_code);
        return;
    }

    MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN syn_msg;
    syn_msg.map_code_ = party_room->GetMapCode();
    syn_msg.party_key_ = party_key;
    syn_msg.party_room_type_ = party_room->GetPartyRoomType();
    syn_msg.require_item_ = recv_msg->require_item_;
    pUser->SendToLinkedServer(&syn_msg, sizeof(syn_msg));
}

HANDLER_CG_IMPL( CG_PARTY_ROOM_FORCE_LEAVE_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_ROOM_FORCE_LEAVE_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_ROOM_FORCE_LEAVE_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

    User* leave_member = UserManager::Instance()->GetUserByObjKey(recv_msg->member_key_);
    if (leave_member == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", recv_msg->member_key_);
        return;
    }

    RC::ePARTY_RESULT result_value = party_manager->FoceLeavePartyRoom(pUser, leave_member);
    if (result_value != RC::RC_PARTY_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Party force leave fail : %d", result_value);
        return;
    }
}

HANDLER_CG_IMPL( CG_PARTY_READY_STATE_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_READY_STATE_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_READY_STATE_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    party_manager->SetMemberReadyState(pUser, recv_msg->ready_state_);

}

HANDLER_CG_IMPL( CG_PARTY_STANDBY_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_STANDBY_ROOM_SYN* recv_msg =
        static_cast<MSG_CG_PARTY_STANDBY_ROOM_SYN*>(pMsg);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        // 전장을 예약하고 매칭 대기열에 있는 상태라면 배틀존 알림 예약 불가
        if (battle_ground_manager->IsValidStandbyGroupMember(pUser->GetUserKey()))
        {
            MSG_CG_PARTY_STANDBY_ROOM_ACK ack_msg;
            ack_msg.result_value_ = false;
            pUser->SendPacket(&ack_msg, sizeof(ack_msg));
            return;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomFinder* room_finder = party_manager->GetPartyRoomFinder();

    bool result_value = room_finder->InsertCharGuidToMissionList(pUser->GetSelectedCharGuid(),
                                                                 recv_msg->map_code_,
                                                                 recv_msg->number_of_mission_);
    if (result_value == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail user standby : %d", pUser->GetSelectedCharGuid());
    }

    pUser->SetIsRecvInvitePartyRoom(TRUE);

    MSG_CG_PARTY_STANDBY_ROOM_ACK ack_msg;
    ack_msg.result_value_ = result_value;
    pUser->SendPacket(&ack_msg, sizeof(ack_msg));
}

HANDLER_CG_IMPL( CG_PARTY_JOIN_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_JOIN_ROOM_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_JOIN_ROOM_SYN*>(pMsg);
    
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(recv_msg->join_party_key_);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", recv_msg->join_party_key_);
        return;
    }

    if (party_room->CheckPassword(recv_msg->password_) == false)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOTEQUAL_PASSWORD;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    if (party_room->GetMapCode() != 0 && party_room->CheckLevel(pUser->GetCharLV()) == false)
    { 
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    if (party_room->CanJoinPartyRoomByMemberCount() == false)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_MAX_MEMBER_COUNT;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    if ( party_room->_GetPartyRoomState() != PartyRoom::PartyRoomStateNormal)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_JOIN_PARTY_ROOM;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION

    MSG_AG_PARTY_CAN_JOIN_ROOM_SYN syn_msg;
    syn_msg.map_code_ = party_room->GetMapCode();
    syn_msg.party_room_type_ = party_room->GetZoneType();
    syn_msg.join_party_key_ = recv_msg->join_party_key_;
    pUser->SendToLinkedServer(&syn_msg, sizeof(syn_msg));
}

HANDLER_CG_IMPL( CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* loom_list = party_manager->GetPartyRoomList();    
    WORD party_key = pUser->GetPartyState().GetPartyKey();

    if (loom_list->SetMaxPlayerCount(party_key, recv_msg->max_count_) == false)
    {
        MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_UNKNOWN_REASON;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Fail change max count : %d", recv_msg->max_count_);
        return;
    }

    MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_ACK ack_msg;
    ack_msg.result_code_ = RC::RC_PARTY_SUCCESS;
    pUser->SendPacket((&ack_msg), sizeof(ack_msg));

    MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_BRD brd_msg;
    brd_msg.max_count_ = recv_msg->max_count_;
    party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}


HANDLER_CG_IMPL( CG_PARTY_CHANGE_ROOM_OPTION_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_CHANGE_ROOM_OPTION_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_CHANGE_ROOM_OPTION_SYN*>(pMsg);

    WORD party_key = pUser->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    if (party_room->GetIsMissionStart() == true)
    {
        MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_CAHNGE_OPTION_PLAYING;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));

        return;
    }

    //맵코드가 0 일 경우에는 조건 검사가 필요 하지 않다.
    if (recv_msg->change_party_room_info_.map_code_ == 0)
    {
        const BasePartyRoomInfo& room_info = recv_msg->change_party_room_info_;

        MAPCODE delete_map_code = party_room->GetMapCode();

        PartyRoomFinder* room_finder = party_manager->GetPartyRoomFinder();
        room_finder->DeletePartyKeyFromMissionList(delete_map_code, party_key);

        PartyRoomList* room_list = party_manager->GetPartyRoomList();
        room_list->SetMissionCode(party_key, room_info.map_code_);

        MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK ack_msg;
        ack_msg.result_code_ = RC::RC_PARTY_SUCCESS;
        pUser->SendPacket(&ack_msg, sizeof(ack_msg));

        MSG_CG_PARTY_CHANGE_ROOM_OPTION_BRD brd_msg;
        brd_msg.change_party_room_info_ = recv_msg->change_party_room_info_;
        party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));

        return;
    }

    //BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();
    
    MSG_AG_PARTY_CAN_CHANGE_OPTION_SYN send_msg;
    send_msg.party_room_info_ = recv_msg->change_party_room_info_;
    send_msg.party_room_info_.special_type_ = eROOM_SPECIAL_NORMAL;
    send_msg.party_key_ = party_key;
    send_msg.require_item_ = recv_msg->require_item_;
    if (pUser->CanApplyPCBangBenefit(TRUE))
    {
        send_msg.party_room_info_.special_type_ = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        send_msg.party_room_info_.special_type_ = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }

    AgentPartyManager::FixStringOfPartyRoomInfo(&send_msg.party_room_info_);
    send_msg.master_key_ = pUser->GetPlayerKey();

    pUser->SendToLinkedServer(&send_msg, sizeof(send_msg));
}


HANDLER_CG_IMPL( CG_PARTY_ROOM_LIST_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_ROOM_LIST_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_ROOM_LIST_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* room_list = party_manager->GetPartyRoomList();

    MSG_CG_PARTY_ROOM_LIST_ACK ack_msg;

    ack_msg.all_page_num_ = room_list->CalcAllPageOfRoomList();
    ack_msg.number_of_list_ = room_list->ArchiveRoomList(ack_msg.party_room_info_,
                                                         _countof(ack_msg.party_room_info_),
                                                         recv_msg->request_page_);
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}

HANDLER_CG_IMPL( CG_PARTY_FIND_WAITING_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_FIND_WAITING_ROOM_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_FIND_WAITING_ROOM_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* room_list = party_manager->GetPartyRoomList();

    MSG_CG_PARTY_FIND_WAITING_ROOM_ACK ack_msg;
    ack_msg.number_of_list_ = room_list->SearchRoomByWaiting(ack_msg.party_room_info_,
                                                             _countof(ack_msg.party_room_info_));
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}

HANDLER_CG_IMPL( CG_PARTY_INVITE_PARTY_ROOM_ANS )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_INVITE_PARTY_ROOM_ANS* recv_msg = 
        static_cast<MSG_CG_PARTY_INVITE_PARTY_ROOM_ANS*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    if (pUser->GetPartyState().IsPartyMember() == true)
    {
        party_manager->LeaveParty(pUser, eREASON_LEAVE_PARTY_INTEND);
    }
    
    WORD join_party_key = recv_msg->join_party_key_;
    const PartyRoom* party_room = party_manager->FindPartyRoom(join_party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", join_party_key);
        return;
    }

    if (party_room->GetMapCode() != 0 && 
        party_room->CheckLevel(pUser->GetCharLV()) == false)
    { 
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    if (party_room->CanJoinPartyRoomByMemberCount() == false)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_MAX_MEMBER_COUNT;
        pUser->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    MSG_AG_PARTY_CAN_JOIN_ROOM_SYN syn_msg;
    syn_msg.map_code_ = party_room->GetMapCode();
    syn_msg.party_room_type_ = party_room->GetZoneType();
    syn_msg.join_party_key_ = join_party_key;
    pUser->SendToLinkedServer(&syn_msg, sizeof(syn_msg));

    return;

}

HANDLER_CG_IMPL( CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* room_list = party_manager->GetPartyRoomList();

    MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK ack_msg;
    ack_msg.number_of_list_ = room_list->SearchRoomByMapCode(ack_msg.party_room_info_,
                                                             recv_msg->request_map_code_,
                                                             recv_msg->party_room_type_,
                                                             _countof(ack_msg.party_room_info_));
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}

HANDLER_CG_IMPL( CG_PARTY_LEAVE_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_LEAVE_ROOM_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_LEAVE_ROOM_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    RC::ePARTY_RESULT result_value = party_manager->LeaveParty(pUser, eREASON_LEAVE_PARTY_INTEND);
    if (result_value != RC::RC_PARTY_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Party leave fail : %d", result_value);
    }

    MSG_CG_PARTY_LEAVE_ROOM_ACK ack_msg;
    ack_msg.result_value_ = result_value;

    pUser->SendPacket(&ack_msg, sizeof(ack_msg));
}

HANDLER_CG_IMPL( CG_PARTY_CREATE_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_CREATE_ROOM_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_CREATE_ROOM_SYN*>(pMsg);

    if (pUser->IsBeginTransaction())
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    if (pUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = RC::RC_PARTY_INVALID_PLAYER_STATE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MAPCODE map_code = recv_msg->party_room_info_.map_code_;
    if (map_code == 0)
    {
        AgentPartyManager::Instance()->CreateNoneTypePartyRoom(pUser, recv_msg->party_room_info_);
        return;
    }

    MSG_AG_PARTY_CAN_CREATE_ROOM_SYN send_msg;
    send_msg.master_key_ = pUser->GetPlayerKey();
    send_msg.party_room_info_ = recv_msg->party_room_info_;
    send_msg.require_item_ = recv_msg->require_item_;
    AgentPartyManager::FixStringOfPartyRoomInfo(&send_msg.party_room_info_);

    pUser->SendToLinkedServer(&send_msg, sizeof(send_msg));
}

HANDLER_CG_IMPL( CG_PARTY_CHECK_ROOM_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_CHECK_ROOM_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_CHECK_ROOM_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoomFinder* party_room_finder = party_manager->GetPartyRoomFinder();
    WORD find_party_key = 
        party_room_finder->GetPartyKeyInMissionList(pUser,
                                                    recv_msg->map_code_,
                                                    recv_msg->select_mission_num_);
    MAPCODE map_code = 0;
    if (find_party_key)
    {
        const PartyRoom* party_room = party_manager->FindPartyRoom(find_party_key);
        if (party_room == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   __FUNCTION__" : Not found party room : Party key : %d", 
                   find_party_key);
        }
        else
        {
            map_code = party_room->GetMapCode();
        }
    }   

    MSG_CG_PARTY_CHECK_ROOM_ACK ack_msg;
    ack_msg.find_party_key_ = find_party_key;
    ack_msg.check_type_ = static_cast<MSG_CG_PARTY_CHECK_ROOM_ACK::CheckType>(recv_msg->check_type_);
    ack_msg.map_code_ = map_code;

    pUser->SendPacket(&ack_msg, sizeof(ack_msg));
}

HANDLER_CG_IMPL( CG_PARTY_BATTLE_ZONE_INFO_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_BATTLE_ZONE_INFO_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_BATTLE_ZONE_INFO_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomList* room_list = party_manager->GetPartyRoomList();

    MSG_CG_PARTY_BATTLE_ZONE_INFO_ACK ack_msg;

    ack_msg.all_page_num_ = room_list->CalcAllPageOfRoomList();
    ack_msg.number_of_list_ = room_list->ArchiveRoomList(ack_msg.party_room_info_,
                                                         _countof(ack_msg.party_room_info_), 1);
    pUser->SendPacket(&ack_msg, ack_msg.GetSize());
}

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
HANDLER_CG_IMPL( CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN )
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN* recv_msg = 
        static_cast<MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN*>(pMsg);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const AgentParty* party = 
        static_cast<const AgentParty*>(party_manager->FindParty(recv_msg->request_party_key_));
    if (party == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found party : user name : %s, party key : %d",
            pUser->GetSelectedCharName(), 
            recv_msg->request_party_key_
            );
        return;
    }
    
    party_manager->RemoveClosingUser(party->GetPartyKey(), recv_msg->leave_member_name_);

    MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD cmd_msg;
    cmd_msg.CopyFrom(recv_msg->leave_member_name_);
    party->SendPacketAll(&cmd_msg, cmd_msg.GetSize());
}
#endif//_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

// 비정상 종료 유저가 재접속 시 CG_PARTY_RETURN_C2G_MEMBER_QUERY_CMD 가 들어온다. (NO ARG)
// (1) : CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::ACCEPT_JOIN (파티참가 희망)
// (2) : CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::REJECT_JOIN (파티참가 포기)
// (3) : CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::REJECT_JOIN (파티참가 2nd 포기) --> 완전 포기
HANDLER_CG_IMPL( CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK )
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK* recv_msg = 
        static_cast<MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK*>(pMsg);
    if (sizeof(*recv_msg) != wSize)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Invalid packet : user guid : %u", 
            pUser->GetUserGUID()
            );
        return;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    if (party_manager->CanJoinReturnParty(recv_msg->PARTY_KEY(), pUser) == false)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Can't join closing user : user name : %s, party key : %u", 
            pUser->GetSelectedCharName(), recv_msg->PARTY_KEY()
            );
        return;
    }

    RC::ePARTY_RESULT result_code = RC::RC_PARTY_SUCCESS;
    if (recv_msg->ANSWER() == MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::ACCEPT_JOIN )
    {
        result_code = party_manager->AcceptReturnParty(recv_msg->PARTY_KEY(), pUser);
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Fail accept return party :user name : %s, fail code : %d",
                pUser->GetSelectedCharName(), result_code
                );
            party_manager->RemoveClosingUser(recv_msg->PARTY_KEY(), pUser->GetSelectedCharGuid());
        }
    }
    else
    {
        result_code = party_manager->RejectReturnParty(recv_msg->PARTY_KEY(), pUser);
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Fail reject return party : user name : %s ,fail code : %d, ",
                pUser->GetSelectedCharName(), result_code
                );
        }
    }
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
HANDLER_CG_IMPL( CG_CHAOS_ZONE_PACKET )
{
    __TOUCHED((pMsg, wSize));
    AgentChaosZoneManager::Instance()->ParseCGPacket(pUser, pMsg, wSize);
}
#endif  // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
