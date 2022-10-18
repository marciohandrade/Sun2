#include <stdafx.h>

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include "Handler_AG_ONEDAYSGUILDMISSION.h"

#include "OneDaysGuildMissionManager.h"
#include "OneDaysGuildMissionRoom.h"
#include "PacketStruct_AG_ChaosZoneSystem.h"
#include "ServerSessions/GameServerSession.h"

void Handler_AG_ONEDAYSGUILDMISSION::OnAG_ProcessPacket( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
	MSG_AG_ONEDAYSGUILDMISSION_PACKET* received_msg = static_cast<MSG_AG_ONEDAYSGUILDMISSION_PACKET*>(packet);

	switch(received_msg->packet_type)
	{
	case AG_ONEDAYSGUILDMISSION_KICKUSER_CMD:
		{
			OnAG_ONEDAYSGUILDMISSION_KICKUSER_CMD(server_session, packet, size);
		}break;
	case AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD:
		{
			OnAG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD(server_session, packet, size);
		} break;
	case AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD:
		{
			OnAG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD(server_session, packet, size);
		} break;
	case AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK:
		{
			OnAG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK(server_session, packet, size);
		} break;
	case AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD:
		{
			OnAG_ONEDAYSGUILDMISSION_TIME_STATE_CMD(server_session, packet, size);
		} break;
	}
}

Handler_AG_ONEDAYSGUILDMISSION_IMPL(AG_ONEDAYSGUILDMISSION_KICKUSER_CMD)
{
	const MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD* recv_msg = 
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD*>(packet);

	OneDaysGuildMissionManager* manager = OneDaysGuildMissionManager::Instance();
	if(manager)
	{
		manager->AllPlayerLeaveRoom(recv_msg->room_key);
	}
}

Handler_AG_ONEDAYSGUILDMISSION_IMPL(AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD)
{
	const MSG_AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD* recv_msg = 
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD*>(packet);

	OneDaysGuildMissionManager* manager = OneDaysGuildMissionManager::Instance();
	if(manager)
	{
		manager->DestroyRoom(recv_msg->room_key);
	}
}

Handler_AG_ONEDAYSGUILDMISSION_IMPL(AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD)
{
	const MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD* recv_msg = 
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD*>(packet);

	OneDaysGuildMissionManager* manager = OneDaysGuildMissionManager::Instance();
	User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);

	if(manager)
	{
		manager->LeaveRoom(recv_msg->zone_key, user);
	}
}

Handler_AG_ONEDAYSGUILDMISSION_IMPL(AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK)
{
	const MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK* recv_msg = \
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK*>(packet);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();
	User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);

	//실패
	if (recv_msg->result_code != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK nak_msg;
		nak_msg.result_code = recv_msg->result_code;
		user->SendPacket(&nak_msg, sizeof(nak_msg));
		return;
	}

	//MoveZone
	BOOL move_result = onedaysguildmission_mgr->MoveToOneDaysGuildMissionRoom(user);

	if (move_result == TRUE)
	{
		MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK ack_msg;
		ack_msg.result_code = RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
		ack_msg.onedaysguildmissin_mapcode = onedaysguildmission_mgr->GetOneDaysGuildMissionMapCode();
		user->SendPacket(&ack_msg, sizeof(ack_msg));

		// 방 입장 성공, 스타트업 리스트에 갱신하자!
		GUILDGUID user_guild_guid = user->GetSelectedCharGuildGuid();
		ZONEKEY zonekey = onedaysguildmission_mgr->GetZoneKeyUsingGuildGuid(user_guild_guid);
		OneDaysGuildMissionRoom* onedaysmissionroom = onedaysguildmission_mgr->FindRoomUsingRoomKey(zonekey);
		if(onedaysmissionroom)
		{
			if(onedaysmissionroom->GetStandByMemberInitCheck())
			{
				if(!onedaysmissionroom->IsStandByMember(user->GetSelectedCharGuid()))
				{
					onedaysmissionroom->InsertStandByMember(user->GetSelectedCharGuid());
				}
			}
		}
	}
	else
	{
		MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK nak_msg;
		nak_msg.result_code = RC::RC_ONEDAYSGUILDMISSION_FAILED;
		user->SendPacket(&nak_msg, sizeof(nak_msg));
	}
}

Handler_AG_ONEDAYSGUILDMISSION_IMPL(AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD)
{
	const MSG_AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD* recv_msg = \
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD*>(packet);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();
	if(onedaysguildmission_mgr)
	{
		OneDaysGuildMissionRoom* onedaysmissionroom = onedaysguildmission_mgr->FindRoomUsingRoomKey(recv_msg->room_key_);
		if(onedaysmissionroom)
		{
			onedaysmissionroom->SetOneDaysGuildMissionState(recv_msg->event_state_);
			onedaysmissionroom->SetOneDaysGuildMissionRemindTime(recv_msg->remain_time_);
		}
	}
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)