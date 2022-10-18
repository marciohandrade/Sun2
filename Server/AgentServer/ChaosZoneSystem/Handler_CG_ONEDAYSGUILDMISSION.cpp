#include <stdafx.h>

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include "Handler_CG_ONEDAYSGUILDMISSION.h"

#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/OneDaysGuildMissionManager.h>
#include <ChaosZoneSystem/OneDaysGuildMissionRoom.h>

void Handler_CG_ONEDAYSGUILDMISSION::OnCG_ProcessPacket( User* user, MSG_BASE* packet, WORD size )
{
	MSG_CG_ONEDAYSGUILDMISSION_PACKET* received_msg = static_cast<MSG_CG_ONEDAYSGUILDMISSION_PACKET*>(packet);

	switch(received_msg->packet_type)
	{
	case CG_ONEDAYSGUILDMISSION_CREATE_SYN:
		{
			OnCG_ONEDAYSGUILDMISSION_CREATE_SYN(user, packet, size);
		} break;
	case CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN:
		{
			OnCG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(user, packet, size);
		} break;
	case CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN:
		{
			OnCG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN(user, packet, size);
		} break;
	default:
		{
			user->SendToLinkedServer(static_cast<MSG_BASE_FORWARD*>(packet),size);
		}break;
	}
}

Handler_CG_ONEDAYSGUILDMISSION_IMPL(CG_ONEDAYSGUILDMISSION_CREATE_SYN)
{
	MSG_CG_ONEDAYSGUILDMISSION_CREATE_SYN* recved_msg = static_cast<MSG_CG_ONEDAYSGUILDMISSION_CREATE_SYN*>(packet);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();

	RC::eONEDAYSGUILDMISSION_RESULT create_result = onedaysguildmission_mgr->CreateOneDaysGuildMissionRoom(user, FALSE);

	;{
		MSG_CG_ONEDAYSGUILDMISSION_CREATE_ACK ack_msg;
		ack_msg.result_code = create_result;
		user->SendPacket(&ack_msg, sizeof(ack_msg));
	};
}

Handler_CG_ONEDAYSGUILDMISSION_IMPL(CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN)
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN* recv_msg = 
		static_cast<MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN*>(packet);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();
	
	// 입장가능검사
	RC::eONEDAYSGUILDMISSION_RESULT check_result = onedaysguildmission_mgr->CanAUserEntersTheRoom(user, FALSE);
	if (check_result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS) {
		MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK ack_msg;
		ack_msg.result_code = check_result;
		user->SendPacket(&ack_msg, sizeof(ack_msg));
		return;
	}

	// 방이 생성되어있나?
	OneDaysGuildMissionRoom* room =
		onedaysguildmission_mgr->FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
	if (room == NULL)
	{
		MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK ack_msg;
		ack_msg.result_code = RC::RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST;
		user->SendPacket(&ack_msg, sizeof(ack_msg));
		return;
	}

	MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN ag_syn_msg;
	if(room->IsStandByMember(user->GetSelectedCharGuid()))
	{
		ag_syn_msg.pass_require_item_check = true;
	}
	ag_syn_msg.entrance_item_code = recv_msg->entrance_item_code;
	ag_syn_msg.m_dwKey = user->GetUserKey();
	ag_syn_msg.map_code = onedaysguildmission_mgr->GetOneDaysGuildMissionMapCode();
	user->SendToLinkedServer(&ag_syn_msg, sizeof(ag_syn_msg));
}

Handler_CG_ONEDAYSGUILDMISSION_IMPL(CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN)
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN* recv_msg = \
		static_cast<MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN*>(packet);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();
	if(onedaysguildmission_mgr)
	{
		if(user)
		{
			GUILDGUID guild_guid = user->GetSelectedCharGuildGuid();

			MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK ack_msg;
			OneDaysGuildMissionRoom* room = onedaysguildmission_mgr->FindRoomUsingGuildGuid(guild_guid);
			if(room)
			{
				ack_msg.event_state_ = room->GetOneDaysGuildMissionState();
				ack_msg.remain_time_ = room->GetOneDaysGuildMissionRemindTime();
			}
			else
			{
				ack_msg.event_state_ = eGUILDMISSION_NONE;
				ack_msg.remain_time_ = 0;
			}

			BOOL result = onedaysguildmission_mgr->GetStandByUserList_CharGuid(guild_guid, \
				ack_msg.stand_by_user_list, &ack_msg.stand_by_user_list_cnt);

			if (result == FALSE) {
				//SUNLOG(eCRITICAL_LOG, "Requested OneDaysGuildMissionRoom is not Exist(StandByUserList)! [Guild:%d]", guild_guid);
			}

			result = onedaysguildmission_mgr->GetEnterPlayerList_CharGuid(guild_guid, \
				ack_msg.enter_player_list, &ack_msg.enter_player_count);

			if (result == FALSE) {
				//SUNLOG(eCRITICAL_LOG, "Requested OneDaysGuildMissionRoom is not Exist(EnterPlayerList)! [Guild:%d]", guild_guid);
			}

			user->SendPacket(&ack_msg, sizeof(ack_msg));
		}
	}
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)