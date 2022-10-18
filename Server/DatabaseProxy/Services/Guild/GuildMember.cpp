#include "stdafx.h"
#include "./GuildMember.h"

#include <Struct.h>
#include <GuildRightInfoParser.h>

BOOST_STATIC_ASSERT(eGUILD_DUTY_MAX < UCHAR_MAX);
__IMPL_GUILDPOOL(GuildMember);

GuildMember::GuildMember(void)
{
}

GuildMember::~GuildMember(void)
{
}

void 
GuildMember::InitializeMember()
{
	ZeroMemory(&member_info_, sizeof(member_info_));
	InitDelegateRight();
	total_info_request_count_ = 0;
}

void 
GuildMember::ReleaseMember()
{

}

void 
GuildMember::Join(CHARGUID char_guid, 
                  const TCHAR* char_name, 
                  BYTE char_class, 
                  LEVELTYPE char_level)
{
	SetCharGuid(char_guid);
	SetCharName(char_name);
	SetClass(char_class);
	SetLevel(char_level);
	SetLogin(true);
	SetDuty(eGUILD_DUTY_REGULAR_SOLDIER);
	SetCorpsNumber(0);
	SetChaoState(eCHAO_STATE_NORMAL);
}


void 
GuildMember::SerializeBaseMemberInfo(INOUT BASE_GUILD_MEMBER_INFO& base_guild_info, eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		CopyMemory( &base_guild_info, &member_info_, sizeof(base_guild_info) );
	}
	else
	{
		// 조건체크..
		if (base_guild_info.m_eGuildDuty == eGUILD_DUTY_NONE || 
            base_guild_info.m_eGuildDuty >= eGUILD_DUTY_MAX)
        {
			SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Invalid value : name = %s, duty = %d", 
                base_guild_info.m_tszCharName, 
                base_guild_info.m_eGuildDuty 
                );
        }

		CopyMemory(&member_info_, &base_guild_info, sizeof(member_info_));
	}
}

void 
GuildMember::SerializeMemberInfo(INOUT GUILD_MEMBER_INFO&  member_info, eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		CopyMemory( &member_info, &member_info_, sizeof(member_info) );
	}
	else
	{
        // 조건체크..
        if (member_info.m_eGuildDuty == eGUILD_DUTY_NONE || 
            member_info.m_eGuildDuty >= eGUILD_DUTY_MAX)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Invalid value : name = %s, Duty = %d", 
                member_info.m_tszCharName, 
                member_info.m_eGuildDuty 
                );
        }

		CopyMemory(&member_info_, &member_info, sizeof(member_info_));
	}
}

void 
GuildMember::SerializeServerMemberInfo(INOUT SERVER_GUILD_MEMBER_INFO&  server_member_info, 
                                       eSERIALIZE se)
{
	if( se == SERIALIZE_LOAD )
	{
		CopyMemory( &server_member_info, &member_info_, sizeof(server_member_info) );
	}
	else
	{
        // 조건체크..
        if (server_member_info.m_eGuildDuty == eGUILD_DUTY_NONE || 
            server_member_info.m_eGuildDuty >= eGUILD_DUTY_MAX)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Invalid value : name : %s, duty : %d", 
                server_member_info.m_tszCharName, 
                server_member_info.m_eGuildDuty 
                );
        }

		CopyMemory(&member_info_, &server_member_info, sizeof(member_info_));
	}
}

bool	
GuildMember::CheckDelegateRight(eGUILD_RIGHTS Right)
{
	for (int i = 0 ; i < MAX_GUILD_DELEGATE_RIGHT; i++)
	{
		if (member_info_.m_DelegateRight[i].m_Right == Right)
        {
			return member_info_.m_DelegateRight[i].m_bRightUse;
        }
	}

	return false;
}

void	
GuildMember::GetDelegateRight(OUT GUILD_RIGHT_USE_INFO* delegate_right)
{
	CopyMemory( 
        delegate_right, 
        member_info_.m_DelegateRight, 
        sizeof(GUILD_RIGHT_USE_INFO)*MAX_GUILD_DELEGATE_RIGHT 
        );
}

void	
GuildMember::SetDelegateRight(GUILD_RIGHT_USE_INFO* delegate_right)
{
	if (!delegate_right)
    {
        return;
    }

	CopyMemory( 
        member_info_.m_DelegateRight, 
        delegate_right, 
        sizeof( GUILD_RIGHT_USE_INFO )*MAX_GUILD_DELEGATE_RIGHT 
        );
}

void	
GuildMember::InitDelegateRight()
{
	ZeroMemory(member_info_.m_DelegateRight, sizeof(member_info_.m_DelegateRight));
	member_info_.m_DelegateRight[0].m_Right = eGUILD_RIGHTS_RELATION;
	member_info_.m_DelegateRight[1].m_Right = eGUILD_RIGHTS_RANKUP;
	member_info_.m_DelegateRight[2].m_Right = eGUILD_RIGHTS_UP2GP;
	member_info_.m_DelegateRight[3].m_Right = eGUILD_RIGHTS_USE_UPGP;
	member_info_.m_DelegateRight[4].m_Right = eGUILD_RIGHTS_SEIGE_WARFARE;
	member_info_.m_DelegateRight[5].m_Right = eGUILD_RIGHTS_GUILD_MARK;
}

void
GuildMember::SetCharName(const TCHAR* pszCharName)
{
	strncpy(member_info_.m_tszCharName, pszCharName, MAX_CHARNAME_LENGTH);
}

void 
GuildMember::SetLogin(bool bLogin)
{ 
	member_info_.m_bLogin = bLogin;
}