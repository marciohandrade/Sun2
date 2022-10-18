#include "stdafx.h"
#include "./GuildCorps.h"


GuildCorps::GuildCorps(void)
{
	corps_guid_ = 0;
	corps_number_ = 0;
	is_create_rear_camp = false;
}

GuildCorps::~GuildCorps(void)
{
}

void		
GuildCorps::Destroy()
{
	if (member_map_.size() == 0)
    {
		return;
    }

	CorpsMemberMap::iterator iter;
	for (iter = member_map_.begin(); iter != member_map_.end(); ++iter)
	{
		GuildMember* guild_member = iter->second;
		if (guild_member)
		{
			guild_member->ReleaseMember();
			GuildMember::FREE(guild_member);
		}
	}

	member_map_.clear();
	member_string_map_.clear();
}

void		
GuildCorps::SerializeBaseMemberTotalInfo(INOUT BASE_GUILD_MEMBER_TOTAL_INFO& base_member_total_info, 
                                         eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		CorpsMemberMap::iterator iter;
		for (iter = member_map_.begin(); iter != member_map_.end(); ++iter)
		{
			GuildMember* guild_member = iter->second;
			if (guild_member)
			{
				guild_member->SerializeBaseMemberInfo( 
                    base_member_total_info.m_MemberInfo[base_member_total_info.m_byMemberCount], 
                    se 
                    );

				base_member_total_info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for (int i = 0 ; i < base_member_total_info.m_byMemberCount; i++)
		{
			if (base_member_total_info.m_MemberInfo[i].m_byCorpsNum != GetNumber())
            {
				 continue;
            }

			GuildMember* guild_member = 
                FindMember(base_member_total_info.m_MemberInfo[i].m_CharGuid);

			if (guild_member == NULL)
			{
				GuildMember* new_member = GuildMember::ALLOC();
				new_member->InitializeMember();
				new_member->SerializeBaseMemberInfo(base_member_total_info.m_MemberInfo[i], se);
				InsertMember(new_member);
			}
			else
            {
				guild_member->SerializeBaseMemberInfo(base_member_total_info.m_MemberInfo[i], se);
            }
		}
	}
}


void		
GuildCorps::SerializeMemberTotalInfo(INOUT GUILD_MEMBER_TOTAL_INFO& member_total_info, 
                                     eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		CorpsMemberMap::iterator iter;
		for (iter = member_map_.begin(); iter != member_map_.end(); ++iter)
		{
			GuildMember* guild_member = iter->second;
			if (guild_member)
			{
				guild_member->SerializeMemberInfo(
                    member_total_info.m_MemberInfo[member_total_info.m_byMemberCount], 
                    se
                    );

				member_total_info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for (int i = 0 ; i < member_total_info.m_byMemberCount; i++)
		{
			if (member_total_info.m_MemberInfo[i].m_byCorpsNum != GetNumber())
            {
				 continue;
            }

			GuildMember* guild_member = FindMember(member_total_info.m_MemberInfo[i].m_CharGuid);
			if (guild_member == NULL)
			{
				GuildMember* new_member = GuildMember::ALLOC();
				new_member->InitializeMember();
				new_member->SerializeMemberInfo(member_total_info.m_MemberInfo[i], se);
				InsertMember(new_member);
			}
			else
            {
				guild_member->SerializeMemberInfo( member_total_info.m_MemberInfo[i], se );
            }
		}
	}
}

void		
GuildCorps::SerializeServerMemberTotalInfo(INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& server_member_total_info, 
                                           eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		CorpsMemberMap::iterator iter;
		for (iter = member_map_.begin(); iter != member_map_.end(); ++iter)
		{
			GuildMember* guild_member = iter->second;
			if (guild_member)
			{
				guild_member->SerializeServerMemberInfo( 
                    server_member_total_info.m_MemberInfo[server_member_total_info.m_byMemberCount], 
                    se);
				server_member_total_info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for (int i = 0 ; i < server_member_total_info.m_byMemberCount; i++)
		{
			if (server_member_total_info.m_MemberInfo[i].m_byCorpsNum != GetNumber())
            {
				 continue;
            }

			GuildMember* guild_member = 
                FindMember(server_member_total_info.m_MemberInfo[i].m_CharGuid);
			if (guild_member == NULL)
			{
				GuildMember* new_member = GuildMember::ALLOC();
				new_member->InitializeMember();
				new_member->SerializeServerMemberInfo(server_member_total_info.m_MemberInfo[i], se);
				InsertMember(new_member);
			}
			else
            {
				guild_member->SerializeServerMemberInfo(server_member_total_info.m_MemberInfo[i], se);
            }
		}
	}

}

GuildMember*	
GuildCorps::CreateMember(CHARGUID char_guid, 
                         const TCHAR* char_name, 
                         eGUILD_DUTY duty, 
                         BYTE char_class, 
                         LEVELTYPE char_level, 
                         BYTE chao_state, 
                         bool is_login)
{
	GuildMember* exist_member = FindMember(char_guid);
	if (exist_member)
    {
        return NULL;
    }

	GuildMember* guild_member = GuildMember::ALLOC();
	guild_member->InitializeMember();
	guild_member->SetCharGuid(char_guid);
	guild_member->SetCharName(char_name);
	guild_member->SetDuty(duty);
	guild_member->SetCorpsNumber(GetNumber());
	guild_member->SetClass(char_class);
	guild_member->SetLevel(char_level);
	guild_member->SetChaoState(chao_state);
	guild_member->SetLogin(is_login);

	if (!InsertMember(guild_member))
	{
		guild_member->ReleaseMember();
		GuildMember::FREE(guild_member);
		return NULL;
	}
	
	return guild_member;
}

// 중요!!!!!!!!!!!!!!!!
// 이함수는 절대로 멤버컨테이너를 루프 돌면서 호출하지 말것.
// 내부적으로 DeleteMember함수가 멤버컨테이너에서 해당 멤버를 삭제 시킴.
void
GuildCorps::DestroyMember(CHARGUID char_guid)
{
	GuildMember* guild_member = FindMember(char_guid);
	if (guild_member == NULL)
    {
        return;
    }

	guild_member->ReleaseMember();
	DeleteMember(guild_member->GetCharGuid());
	GuildMember::FREE( guild_member );
}

//기존의 길드 멤버를 군단에 가입 시킨다.
bool		
GuildCorps::InsertMember(GuildMember* guild_member)
{
	if (guild_member == NULL)
    {
        return false;
    }

	GuildMember* exist_member = FindMember(guild_member->GetCharGuid());
	if (exist_member)
    {
        return false;
    }

	GuildMember* find_member = FindMember(guild_member->GetCharName());
    if (find_member)
    {
        return false;
    }

	member_map_.insert(std::make_pair(guild_member->GetCharGuid(), guild_member))  ;
	member_string_map_.insert(std::make_pair(guild_member->GetCharName(), guild_member));

	return true;
}

//단순히 군단에서 제외시킨다.
// 중요!!!!!!!!!!!!!!!!
// 이함수는 절대로 멤버컨테이너를 루프 돌면서 호출하지 말것.
// 내부적으로 DeleteMember함수가 멤버컨테이너에서 해당 멤버를 삭제 시킴.
void		
GuildCorps::DeleteMember(CHARGUID char_guid)
{
	GuildMember* guild_member = FindMember(char_guid);
	if (guild_member == NULL)
    {
        return;
    }

	CorpsMemberMap::iterator find_member_it;
	find_member_it = member_map_.find(guild_member->GetCharGuid());
	if (find_member_it != member_map_.end())
    {
		member_map_.erase(find_member_it);
    }

	CorpsMemberStringMap::iterator find_string_it;
	find_string_it = member_string_map_.find( guild_member->GetCharName() );
	if (find_string_it != member_string_map_.end())
    {
		member_string_map_.erase( find_string_it );
    }

}

GuildMember*	
GuildCorps::FindMember( CHARGUID char_guid )
{
	CorpsMemberMap::iterator find_it;
	find_it = member_map_.find(char_guid);
	if (find_it != member_map_.end())
	{
		GuildMember* guild_member = find_it->second;
		if (guild_member)
        {
			return guild_member;
        }
	}

	return NULL;
}

GuildMember*	
GuildCorps::FindMember(const CHAR* member_name)
{
	CorpsMemberStringMap::iterator find_it;
	find_it = member_string_map_.find(member_name);
	if (find_it != member_string_map_.end())
	{
		GuildMember* guild_member = find_it->second;
		if (guild_member)
        {
			return guild_member;
        }
	}

	return NULL;
}

GuildMember*	
GuildCorps::FindCommander()
{
	CorpsMemberMap::iterator begin_it;
	for (begin_it = member_map_.begin(); begin_it != member_map_.end(); ++begin_it )
	{
		GuildMember* guild_member = begin_it->second;
		if (guild_member == NULL)
        {
            continue;
        }

		//직할군단이면 길드장이 군단장 이다.
		if (GetNumber() == 0)
		{
			if (guild_member->GetDuty() == eGUILD_DUTY_MASTER)
            {
				return guild_member;
            }
		}
		else
		{
			if (guild_member->GetDuty() == eGUILD_DUTY_COMMANDER)
            {
				return guild_member;
            }
		}
	}

	return NULL;
}

GuildMember*	
GuildCorps::FindMemberByDuty(const eGUILD_DUTY duty)
{
	CorpsMemberMap::iterator begin_it;
	for (begin_it = member_map_.begin(); begin_it != member_map_.end(); ++begin_it)
	{
		GuildMember* guild_member = begin_it->second;
		if (guild_member && duty == guild_member->GetDuty())
        {
			return guild_member;
        }
	}

	return NULL;
}

bool			
GuildCorps::IsExistMemberDuty(eGUILD_DUTY duty)
{
	CorpsMemberMap::iterator begin_it;
	for (begin_it = member_map_.begin(); begin_it != member_map_.end(); ++begin_it)
	{
		GuildMember* guild_member = begin_it->second;
		if (guild_member == NULL)
        {
            continue;
        }

		if (guild_member->GetDuty() == duty)
        {
			return true;
        }
	}

	return false;
}

//직책별 멤버들의 수
BYTE			
GuildCorps::GetMemberCountByDuty(eGUILD_DUTY duty)
{
	BYTE byCount = 0;

	CorpsMemberMap::iterator begin_it;
	for( begin_it = member_map_.begin(); begin_it != member_map_.end(); ++begin_it )
	{
		GuildMember* guild_member = begin_it->second;
		if (guild_member == NULL)
        {
            continue;
        }

		if (guild_member->GetDuty() == duty)
        {
			byCount++;
        }
	}

	return byCount;
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
const void GuildCorps::FindMembersByDuty( eGUILD_DUTY guild_duty, CorpsMemberMap& member_map )
{
    CorpsMemberMap::iterator begin_it;
    for (begin_it = member_map_.begin(); begin_it != member_map_.end(); ++begin_it)
    {
        GuildMember* guild_member = begin_it->second;
        if (guild_member && guild_duty == guild_member->GetDuty())
        {
            member_map.insert(std::make_pair(guild_member->GetCharGuid(), guild_member));
        }
    }

}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION