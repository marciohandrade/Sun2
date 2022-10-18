#ifndef     __UIDEF_MANAGER_GUILD_H__
#define     __UIDEF_MANAGER_GUILD_H__

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

struct IM_GUILD_MANAGER
{
	static const DWORD  GUILD_CREATE;
    static const DWORD  GUILD_WINDOW;
    static const DWORD  GUILD_MENU;
    static const DWORD  GUILD_MAIN;
	static const DWORD  GUILD_CONVERT_GP;
	static const DWORD  GUILD_DONATE_UP;
	static const DWORD  GUILD_NOTICE;
	static const DWORD  GUILD_RANKUP;
	static const DWORD  GUILD_DUTY_VEST;
	static const DWORD  GUILD_RIGHT_DELEGATION;
	static const DWORD  GUILD_MEMBER_RIGHT_VIEW;
	static const DWORD  GUILD_CORPS_CHANGE;
	static const DWORD	GUILD_CORPS_DESTORY;
	static const DWORD  GUILD_MARK_COMBINATION;
	static const DWORD  GUILD_MARK_REGISTE_CONFIRM;
	static const DWORD  GUILD_ALLIANCE;
	static const DWORD  GUILD_HOSTILITY;
	static const DWORD  GUILD_ONE_SIDE_HOSTILITY;
	static const DWORD  GUILD_RELATION_ASK;
	static const DWORD  GUILD_ALLIANCE_REQUEST;
	static const DWORD  GUILD_WAREHOUSE;
	static const DWORD  GUILD_WAREHOUSE_RIGHT;
	static const DWORD  GUILD_WAREHOUSE_LOG;
	static const DWORD	GUILD_CHANGE_GUILD_NAME;
};


//
//  Packet I/O Boolean
//
enum
{
    ePKBOOL_GUILD_BASE = 0,
    ePKBOOL_GUILD_CW_GUILD_MEMBER_SYN,
    ePKBOOL_GUILD_MAX,
};

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#endif //__UIDEF_MANAGER_GUILD_H__