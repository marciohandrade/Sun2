#include "Stdafx.h"
#include "AgentParty.h"
#include "AgentPartyOperator.h"

AgentParty::AgentParty()
{
}

AgentParty::~AgentParty()
{
}

void
AgentParty::Init(WORD party_key, DWORD master_key)
{
	BaseParty::Init(party_key, master_key, ePARTY_MAX);
	session_member_mgr_.Release();
	closing_user_list_.Init(party_key);
}

void
AgentParty::Release()
{
	BaseParty::Release();

	session_member_mgr_.Release();
}

void
AgentParty::Update()
{
	closing_user_list_.Update(this);
}

void
AgentParty::SendPacketAll(MSG_BASE* send_msg, 
                          WORD msg_size, 
                          DWORD except_key, 
                          KEYTYPE check_zone_key)const
{
    UserManager* user_manager = UserManager::Instance();
    FOREACH_CONTAINER(const MEMBER_MAP::value_type& ref_member, party_member_map_, MEMBER_MAP)
    {
        const _PARTY_MEMBER_INFO& member = ref_member.second;
        if (except_key != 0 && except_key == member.m_dwMemberKey)
        {
            continue;
        }

        User* user = user_manager->GetUserByObjKey(member.m_dwMemberKey);
        if (user == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Not found user : %d", 
                member.m_dwMemberKey
                );
            continue;
        }
        user->SendPacket(send_msg, msg_size);
    }
}

void
AgentParty::Display()
{
    DWORD number_of_members = DWORD(party_member_map_.size());

    int total_sessions = session_member_mgr_.GetTotSessionMemberNum();
    int total_members = session_member_mgr_.GetTotMemberNum();

    if (number_of_members == 0 || 
        total_sessions != number_of_members || 
        total_members != number_of_members)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__"Number of members : %2u, Invalid total sessions : %2u, Total Members : %2u",
            number_of_members, 
            total_sessions, 
            total_members
            );
    }
}

bool
AgentParty::IsClosingUser(User* user)
{
    const ClosingUserInfo* closing_user_info = 
        closing_user_list_.FindClosingUser(user->GetSelectedCharGuid());

    return closing_user_info ? true : false;
}

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
void 
AgentParty::GetMemberInfoAll(PARTY_MEMBER_TOTAL_INFO& member_info_all)
{
    BaseParty::GetMemberInfoAll(member_info_all);
    closing_user_list_.AddClosingUserInfoToMemberList(member_info_all);
}
#endif