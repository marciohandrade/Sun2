#pragma once


#define Handler_CG_PARTY_DECL( p )	static VOID On##p( UserSession * pUserSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_CG_PARTY_IMPL( p )	VOID Handler_CG_PARTY::On##p( UserSession * pUserSession, MSG_BASE * pMsg, WORD wSize )

class UserSession;

class Handler_CG_PARTY
{
public:
	Handler_CG_PARTY(){}
	~Handler_CG_PARTY(){}

	Handler_CG_PARTY_DECL( CG_PARTY_JOIN_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_JOIN_RESPONSE_ACK );
	Handler_CG_PARTY_DECL( CG_PARTY_JOIN_RESPONSE_NAK );
	Handler_CG_PARTY_DECL( CG_PARTY_LEAVE_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_FORCED_EXPULSION_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_CHANGE_MASTER_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_DESTROY_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_INVITE_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_INVITE_RESPONSE_ACK );
	Handler_CG_PARTY_DECL( CG_PARTY_INVITE_RESPONSE_NAK );
	Handler_CG_PARTY_DECL( CG_PARTY_SELECT_TARGET_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_MINIMAP_POINTING_SYN );
	Handler_CG_PARTY_DECL( CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN );
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    Handler_CG_PARTY_DECL( CG_PARTY_CREATE_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_CHECK_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_BATTLE_ZONE_INFO_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_LEAVE_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_ROOM_LIST_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_CHANGE_ROOM_OPTION_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_JOIN_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_STANDBY_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_READY_STATE_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_ROOM_FORCE_LEAVE_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_ROOM_START_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_SELECT_TEAM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_REQUEST_MEMBER_INFO_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_FIND_ROOM_BY_CHARID_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_STANDBY_PAUSE_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_REQUEST_RENDER_INFO_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_FIND_WAITING_ROOM_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN );
    Handler_CG_PARTY_DECL( CG_PARTY_INVITE_PARTY_ROOM_ANS );
  #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    Handler_CG_PARTY_DECL( CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN );
  #endif

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    Handler_CG_PARTY_DECL( CG_CHAOS_ZONE_PACKET );
#endif  // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};
