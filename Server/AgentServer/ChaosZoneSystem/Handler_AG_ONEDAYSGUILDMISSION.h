#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#define Handler_AG_ONEDAYSGUILDMISSION_DECL(p) \
	VOID On##p(GameServerSession* server_session, MSG_BASE* packet, WORD size)
#define Handler_AG_ONEDAYSGUILDMISSION_IMPL(p) \
	VOID Handler_AG_ONEDAYSGUILDMISSION::On##p(GameServerSession* server_session, MSG_BASE* packet, WORD size)

/*--------------------------------------------------------------------------------------------------
	-- Class Defined
--------------------------------------------------------------------------------------------------*/
class GameServerSession;

class Handler_AG_ONEDAYSGUILDMISSION
{
public:
	Handler_AG_ONEDAYSGUILDMISSION() {}
	~Handler_AG_ONEDAYSGUILDMISSION() {}

	void OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);

private:
	Handler_AG_ONEDAYSGUILDMISSION_DECL(AG_ONEDAYSGUILDMISSION_KICKUSER_CMD);
	Handler_AG_ONEDAYSGUILDMISSION_DECL(AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD);
	Handler_AG_ONEDAYSGUILDMISSION_DECL(AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD);

	Handler_AG_ONEDAYSGUILDMISSION_DECL(AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK);
	Handler_AG_ONEDAYSGUILDMISSION_DECL(AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD);
};

#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION