#pragma once

#include <PacketStruct_CG.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class User;

//-------------------------------------------------------------------------------------------------

#define HANDLER_CG_DECL( fnName )		\
	static VOID On##fnName( User* pUser, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_CG_IMPL( fnName )		\
	VOID Handler_CG::On##fnName( User* pUser, MSG_BASE* pMsg, WORD wSize )

class Handler_CG
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_CHARINFO();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_SYNC();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_ZONE();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_PARTY();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_GM();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_SECURE();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_ETC();
    static sPACKET_HANDLER_INFO*	GetHandlerInfo_Event();
public:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	HANDLER_CG_DECL( CG_CONNECTION_HEARTBEAT_SYN );
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    HANDLER_CG_DECL( CG_CONNECTION_TICK_QUERY_ACK );
#endif
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	// 월드와 관련된 패킷
	HANDLER_CG_DECL( CG_PREPARE_WORLD_CONNECT_SYN );
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	// DBP와 관련된 패킷
	HANDLER_CG_DECL( CG_CONNECTION_REENTERSERVER_SYN );
	HANDLER_CG_DECL( CG_CHARINFO_CREATE_SYN );
	HANDLER_CG_DECL( CG_CHARINFO_DESTROY_SYN );
	HANDLER_CG_DECL( CG_CHARINFO_RECOVER_SYN );
#ifdef __NA_001044_ADD_CUSTOM_UI
	HANDLER_CG_DECL( CG_CHARINFO_CHARACTERID_CHECK_SYN );
#endif
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	HANDLER_CG_DECL( CG_SYNC_PLAYER_ENTER_SYN );

	// Game Server와 관련된 패킷
	HANDLER_CG_DECL( CG_CONNECTION_ENTERVILLAGEREQ_SYN );
	HANDLER_CG_DECL( CG_CONNECTION_BACKTOCHARSELECT_SYN );
	HANDLER_CG_DECL( CG_CONNECTION_NOTIFY_LOGOUT_SYN );

	// 미션 로비 방 리스트 요청
	HANDLER_CG_DECL( CG_ZONE_LIST_MISSIONLOBBY_SYN );
	// 헌팅 로비 방 리스트 요청
	HANDLER_CG_DECL( CG_ZONE_LIST_HUNTINGLOBBY_SYN );	
	// 진행된 헌팅 방 리스트 요청
	HANDLER_CG_DECL( CG_ZONE_LIST_HUNTING_SYN );
	// PVP방 리스트 요청
	HANDLER_CG_DECL( CG_ZONE_LIST_PVPLOBBY_SYN );

	// 경쟁헌팅방 리스트 요청
	HANDLER_CG_DECL( CG_ZONE_LIST_CHUNTINGLOBBY_SYN );
	HANDLER_CG_DECL( CG_ZONE_LIST_CHUNTING_SYN );

	// 특정 방 리스트 새로고침
	HANDLER_CG_DECL( CG_ZONE_LIST_REFRESH_SYN );

	// 바로가기
	HANDLER_CG_DECL( CG_ZONE_SHORTCUT_SYN );
	// 아이디로 방 찾기
	HANDLER_CG_DECL( CG_ZONE_FIND_ROOM_FROM_CHARID_SYN );
	HANDLER_CG_DECL( CG_ZONE_FIND_ROOM_FROM_NO_SYN );

	// 방장이 로비(헌팅or미션)방을 만든다
	HANDLER_CG_DECL( CG_ZONE_LOBBY_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_CHUNTINGLOBBY_CREATE_SYN );

	//로비 이름 변경
	HANDLER_CG_DECL( CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN );

	// 로비에 참가
	HANDLER_CG_DECL( CG_ZONE_LOBBY_JOIN_SYN );
	// [구성원의 권한]
	// 구성원이 레디 버튼을 누른다.
	HANDLER_CG_DECL( CG_ZONE_LOBBY_READY_SYN );
	HANDLER_CG_DECL( CG_ZONE_LOBBY_READY_CANCEL_SYN );

	// 기존의 방정보를 요청한다.
	HANDLER_CG_DECL( CG_ZONE_LOBBY_REQUEST_INFO_SYN );

	// [방장의 권한]
	// 인원수 변경
	// 플레이어 강퇴
	HANDLER_CG_DECL( CG_ZONE_LOBBY_KICKPLAYER_SYN );
	// 플레이어 팀 변경
	HANDLER_CG_DECL( CG_ZONE_LOBBY_CHANGE_TEAM_SYN );
	// 맵 변경
	HANDLER_CG_DECL( CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN );
	HANDLER_CG_DECL( CG_ZONE_LOBBY_CHANGE_MAP_SYN );

	// 로비의 헌팅 제한 설정 바꿈(필드, 난이도, 보너스)
	HANDLER_CG_DECL( CG_ZONE_LOBBY_HUNTING_CONFIG_SYN );
	// 로비의 PVP 제한 설정 바꿈(필드, 룰, 모드)
	HANDLER_CG_DECL( CG_ZONE_LOBBY_PVP_CONFIG_SYN );
	HANDLER_CG_DECL( CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN );

	HANDLER_CG_DECL( CG_ZONE_HUNTING_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_MISSION_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_PVP_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_CHUNTING_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_CHUNTING_MIGRATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_HUNTING_JOIN_SYN );
	HANDLER_CG_DECL( CG_ZONE_LOBBY_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_MISSION_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_HUNTING_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_PVP_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_CHUNTING_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_HUNTING_RELAY_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_HUNTING_RELAY_JOIN_SYN );
	HANDLER_CG_DECL( CG_ZONE_PORTAL_FIELD_MOVE_SYN );		// 필드/마을 이동
	HANDLER_CG_DECL( CG_ZONE_RETURN_VILLAGE_SYN );			// 죽었을때 필드에서 마을로 이동.
	HANDLER_CG_DECL( CG_ZONE_MISSION_RELAY_CREATE_SYN );
	HANDLER_CG_DECL( CG_ZONE_MISSION_RELAY_JOIN_SYN );


	HANDLER_CG_DECL( CG_PARTY_JOIN_SYN );
	HANDLER_CG_DECL( CG_PARTY_JOIN_RESPONSE_ACK );
	HANDLER_CG_DECL( CG_PARTY_JOIN_RESPONSE_NAK );
	HANDLER_CG_DECL( CG_PARTY_LEAVE_SYN );
	HANDLER_CG_DECL( CG_PARTY_FORCED_EXPULSION_SYN );
	HANDLER_CG_DECL( CG_PARTY_CHANGE_MASTER_SYN );
	HANDLER_CG_DECL( CG_PARTY_DESTROY_SYN );
	HANDLER_CG_DECL( CG_PARTY_INVITE_SYN );
	HANDLER_CG_DECL( CG_PARTY_INVITE_RESPONSE_ACK );
	HANDLER_CG_DECL( CG_PARTY_INVITE_RESPONSE_NAK );
	HANDLER_CG_DECL( CG_PARTY_SELECT_TARGET_SYN );
	HANDLER_CG_DECL( CG_PARTY_MINIMAP_POINTING_SYN );
	HANDLER_CG_DECL( CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN );

	//HANDLER_CG_DECL( CG_PARTY_RETURN_G2C_LEADER_QUERY_CMD );		// 파티장에게 파티원 초대 여부 메시지, 파티원의 한번 취소시
	//HANDLER_CG_DECL( CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD );		// 파티원에게 초대 메시지
	HANDLER_CG_DECL( CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK );		// 파티원의 거부 관련

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    HANDLER_CG_DECL( CG_PARTY_CREATE_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_CHECK_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_BATTLE_ZONE_INFO_SYN );
    HANDLER_CG_DECL( CG_PARTY_LEAVE_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_ROOM_LIST_SYN );
    HANDLER_CG_DECL( CG_PARTY_CHANGE_ROOM_OPTION_SYN );
    HANDLER_CG_DECL( CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN );
    HANDLER_CG_DECL( CG_PARTY_JOIN_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_STANDBY_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_READY_STATE_SYN );
    HANDLER_CG_DECL( CG_PARTY_ROOM_FORCE_LEAVE_SYN );
    HANDLER_CG_DECL( CG_PARTY_ROOM_START_SYN );
    HANDLER_CG_DECL( CG_PARTY_SELECT_TEAM_SYN );
    HANDLER_CG_DECL( CG_PARTY_REQUEST_MEMBER_INFO_SYN );
    HANDLER_CG_DECL( CG_PARTY_FIND_ROOM_BY_CHARID_SYN );
    HANDLER_CG_DECL( CG_PARTY_STANDBY_PAUSE_SYN );
    HANDLER_CG_DECL( CG_PARTY_REQUEST_RENDER_INFO_SYN );
    HANDLER_CG_DECL( CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN );
    HANDLER_CG_DECL( CG_PARTY_FIND_WAITING_ROOM_SYN );
    HANDLER_CG_DECL( CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN );
    HANDLER_CG_DECL( CG_PARTY_INVITE_PARTY_ROOM_ANS );
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    HANDLER_CG_DECL( CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN );
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    HANDLER_CG_DECL( CG_CHAOS_ZONE_PACKET );
#endif  // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	// GM 명령어
	HANDLER_CG_DECL( CG_GM_CHANNELINFO_SYN );
	HANDLER_CG_DECL( CG_GM_WARP_SYN );

	///////////////////////////////////////////////////////////////////////////
	// <Waverix><ArmorKit>
	HANDLER_CG_DECL( CG_ARMORKIT_FINDHACK );

	// <Cryptography>
	HANDLER_CG_DECL( CG_CRYPTOGRAPHY_ACK );
	HANDLER_CG_DECL( CG_CRYPTOGRAPHY_NAK );

	HANDLER_CG_DECL(CG_SECURE_MOLACONTROL);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK);
	//HANDLER_CG_DECL(CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_BUILTIN_CHK_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK);
	//HANDLER_CG_DECL(CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN);
    // USING_HACKSHIELD
	HANDLER_CG_DECL( CG_HACKSHIELD_SERVER_EX_QUERY_ACK );
    // USING_XIGNCODE
    HANDLER_CG_DECL(CG_XIGNCODE_SERVER_QUERY_ACK);
    // USING_GAMEGUARD
    HANDLER_CG_DECL(CG_GAMEGUARD_SERVER_QUERY_ACK);
    HANDLER_CG_DECL(CG_GAMEGUARD_HACKDATA_CMD);

	HANDLER_CG_DECL( CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN );
	HANDLER_CG_DECL( CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN );
	HANDLER_CG_DECL( CG_CONNECTION_CRASH_REPORT_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN );

    HANDLER_CG_DECL( CG_ZONE_EX_SSQ_JOIN_INFO_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_SSQ_JOIN_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_SSQ_ROOM_INFO_SYN );
	HANDLER_CG_DECL( CG_ZONE_EX_SSQ_OBSERVER_SYN );
    //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    HANDLER_CG_DECL(CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN);
    HANDLER_CG_DECL(CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN);

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    HANDLER_CG_DECL(CG_ZONE_EX_SPA_ENTER_SYN);

    HANDLER_CG_DECL(CG_ZONE_EX_SPA_LEAVE_SYN);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	//Agent와 관련된 패킷
	HANDLER_CG_DECL( CG_ETC_GET_BATTLE_OPEN_TIME_SYN );

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    HANDLER_CG_DECL( CG_ETC_GAMEOPTION_LOAD_SYN );
    HANDLER_CG_DECL( CG_ETC_GAMEOPTION_SAVE_SYN );
#endif //_NA_0_20110329_GAMEOPTION_SETTING
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    HANDLER_CG_DECL(CG_ETC_GUILD_RANKINGINFO_SYN);
    HANDLER_CG_DECL(CG_ETC_GUILDINFO_SYN);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

    //_NA_000000_20120410_SPEEDHACK
    HANDLER_CG_DECL(CG_ETC_MEMORY_HACK_CMD);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    HANDLER_CG_DECL(CG_CONNECTION_SERVER_SELECT_REQUEST_SYN);
#endif

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        HANDLER_CG_DECL(CG_SHOP_SEND_GIFT_SYN);
    #endif
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //프로토콜 핸들러 선언부
    HANDLER_CG_DECL( CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN ); //친구 초대 이벤트의 보상 아이템이 목록을 요청한다.    
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    HANDLER_CG_DECL(CG_CHARNAME_CHANGE_SYN);    
#endif

};

// DONE: f110609.3L, f110516.3L, need renewal functors = {
//  SendLobbyInfoForRequest, SendPVPLobbyInfoForRequest, SendBroadMessage }
#if SUN_CODE_BACKUP
template<class PACKET_TYPE>
class SendLobbyInfoForRequest
{
	User * m_pMe;
	PACKET_TYPE & m_msg;
public:
	SendLobbyInfoForRequest( PACKET_TYPE & msg, User * pMe):m_msg(msg),m_pMe(pMe){ m_msg.m_Count = 0; }
	~SendLobbyInfoForRequest(){}

	VOID SendPacket()
	{
		m_pMe->SendPacket( &m_msg, m_msg.GetSize() );
	}
	VOID operator()( User * pUser )
	{
		if( m_pMe && m_pMe->GetUserKey() == pUser->GetUserKey() ) return;

		if( !pUser->IsMaster() && pUser->IsReady() )
		{
			m_msg.m_dwReadyPlayerKey[m_msg.m_Count] = pUser->GetPlayerKey();
			++m_msg.m_Count;
		}
	}
};

template<class PACKET_TYPE>
class SendPVPLobbyInfoForRequest
{
	User * m_pMe;
	PACKET_TYPE & m_msg;
public:
	SendPVPLobbyInfoForRequest( PACKET_TYPE & msg, User * pMe = NULL ):m_msg(msg),m_pMe(pMe){ m_msg.m_Count = 0; }
	~SendPVPLobbyInfoForRequest(){}

	VOID SendPacket()
	{
		m_pMe->SendPacket( &m_msg, m_msg.GetSize() );
	}
	VOID operator()( User * pUser )
	{
		if( m_pMe && m_pMe->GetUserKey() == pUser->GetUserKey() ) return;

		m_msg.m_PlayerInfo[m_msg.m_Count].m_Team		= pUser->GetTeam();
		m_msg.m_PlayerInfo[m_msg.m_Count].m_Ready		= (BYTE)pUser->IsReady();
		m_msg.m_PlayerInfo[m_msg.m_Count].m_dwPlayerKey = pUser->GetPlayerKey();
		++m_msg.m_Count;
	}
};

template<class PACKET_TYPE>
class SendBroadMessage
{
	PACKET_TYPE & m_msg;
public:
	SendBroadMessage( PACKET_TYPE & msg ):m_msg(msg){}
	~SendBroadMessage(){}

	VOID operator()( User * pUser )
	{
		pUser->SendPacket( &m_msg, m_msg.GetSize() );
	}
};
#endif //SUN_CODE_BACKUP


