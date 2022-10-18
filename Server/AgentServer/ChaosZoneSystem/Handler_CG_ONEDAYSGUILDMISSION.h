#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#define Handler_CG_ONEDAYSGUILDMISSION_DECL(p) \
	VOID On##p(User* user, MSG_BASE* packet, WORD wSize)
#define Handler_CG_ONEDAYSGUILDMISSION_IMPL(p) \
	VOID Handler_CG_ONEDAYSGUILDMISSION::On##p(User* user, MSG_BASE* packet, WORD wSize)

/*--------------------------------------------------------------------------------------------------
	-- Class Defined
--------------------------------------------------------------------------------------------------*/
class Handler_CG_ONEDAYSGUILDMISSION
{
public:
	Handler_CG_ONEDAYSGUILDMISSION() {}
	~Handler_CG_ONEDAYSGUILDMISSION() {}

	void OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size);

private:
	Handler_CG_ONEDAYSGUILDMISSION_DECL(CG_ONEDAYSGUILDMISSION_CREATE_SYN);
	Handler_CG_ONEDAYSGUILDMISSION_DECL(CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN);
	Handler_CG_ONEDAYSGUILDMISSION_DECL(CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN);
};	

#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION