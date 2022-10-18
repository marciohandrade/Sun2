#pragma once

#include "BaseParty.h"
#include "SessionMemberMgr.h"
#include "ClosingUserList.h"

class AgentParty : public BaseParty
{
public:
    AgentParty();
    ~AgentParty();

    void Release();
    void Update();
    void Display();
    void Init(
        WORD party_key, 
        DWORD master_key
        );

    void SendPacketAll(
        MSG_BASE* msg,
        WORD size,
        DWORD except_key = 0,
        KEYTYPE check_zone_key = 0
        )const;

    bool IsClosingUser(
        User* user
        );

    inline SessionMemberMgr& GetSessionMemberMgr();
    inline ClosingUserList& GetClosingUserList();
	inline BYTE GetMemberNum()const;
    inline BYTE GetActiveMemberCount()const;
    inline BYTE GetClosingUserCount()const;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    void GetMemberInfoAll(
        PARTY_MEMBER_TOTAL_INFO& member_info_all
        );
#endif

private:
	SessionMemberMgr session_member_mgr_;
	ClosingUserList closing_user_list_;
};

inline SessionMemberMgr&
AgentParty::GetSessionMemberMgr()
{
    return session_member_mgr_;
}

inline ClosingUserList&
AgentParty::GetClosingUserList()
{
    return closing_user_list_;
}

inline BYTE 
AgentParty::GetMemberNum()const
{
    return GetActiveMemberCount() + GetClosingUserCount();
}

inline BYTE
AgentParty::GetActiveMemberCount()const
{
    BYTE number_of_active_members = BaseParty::GetMemberNum();

    return number_of_active_members;
}

inline BYTE
AgentParty::GetClosingUserCount()const
{
    BYTE number_of_closing_members = closing_user_list_.GetClosingUserCount();

    return number_of_closing_members;
}