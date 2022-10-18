#pragma once
#ifndef DBPROXY_GUILD_CORPS_H
#define DBPROXY_GUILD_CORPS_H

/**********************************************/
/* 길드 군단
/* 길드 군단의 최대 인원은 20명 이다.
/* 길드 군단은 1명의 군단장, 1명의 훈련대장, 5명의 훈련병, 13명의 정예병으로 이루어 질 수 있다.
/**********************************************/

#include <map>
#include "Services/Guild/GuildMember.h"

typedef std::map< CHARGUID, GuildMember*> CorpsMemberMap;
typedef	std::map< std::string, GuildMember*> CorpsMemberStringMap;

class GuildCorps
{
public:
	GuildCorps(void);
	~GuildCorps(void);

public:
	void Destroy();
	
    void SerializeBaseMemberTotalInfo( 
        INOUT BASE_GUILD_MEMBER_TOTAL_INFO& base_member_total_info, 
        eSERIALIZE se 
        );

	void SerializeMemberTotalInfo( 
        INOUT GUILD_MEMBER_TOTAL_INFO& member_total_info,
        eSERIALIZE se 
        );

	void SerializeServerMemberTotalInfo( 
        INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& server_member_total_info, 
        eSERIALIZE se 
        );

	void SetGuid(DWORD corps_guid);
	DWORD GetGuid();
	void SetNumber(BYTE corps_number);
	BYTE GetNumber();
	BYTE GetMemberCount();

	//현재 새로 길드에 가입시 이 함수를 쓴다.( GuildMember 객체 자체를 소멸시킨다. )
	GuildMember* CreateMember( 
        CHARGUID char_guid, 
        const TCHAR* char_name, 
        eGUILD_DUTY duty, 
        BYTE char_class, 
        LEVELTYPE char_level, 
        BYTE chao_state, 
        bool is_login 
        );

	void DestroyMember( 
        CHARGUID char_guid
        );

	//같은 길드 내에서 군단 변경시 이 함수를 쓴다.( 컨테이너에서 GuildMember 원소만 삭제 한다. )
	bool InsertMember ( 
        GuildMember* insert_member 
        );

	void DeleteMember( 
        CHARGUID char_guid
        );

	GuildMember* FindMember( 
        CHARGUID char_guid
        );

	GuildMember* FindMember(
        const TCHAR* member_name 
        );

	GuildMember* FindCommander();

	GuildMember* FindMemberByDuty(
        const eGUILD_DUTY duty
        );

	bool IsExistMemberDuty( 
        eGUILD_DUTY duty 
        );		

	BYTE GetMemberCountByDuty( 
        eGUILD_DUTY duty 
        );

	BOOL IsExistRearCamp();

	void SetRearCamp( 
        BOOL is_camp 
        );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    const void FindMembersByDuty(eGUILD_DUTY guild_duty, CorpsMemberMap& member_map);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

private:
    DWORD corps_guid_;// 군단 Guid
    BYTE corps_number_;// 군단 번호( 0군단(직할), 1군단, 2군단.... )
    CorpsMemberMap member_map_;
    CorpsMemberStringMap member_string_map_;	
    BOOL is_create_rear_camp;// 길드 육성캠프 존재여부.
};

inline void 
GuildCorps::SetGuid(DWORD corps_guid)
{ 
    corps_guid_ = corps_guid; 
}

inline DWORD 
GuildCorps::GetGuid()
{ 
    return corps_guid_; 
}

inline void 
GuildCorps::SetNumber(BYTE corps_number)
{
    corps_number_ = corps_number; 
}

inline BYTE 
GuildCorps::GetNumber()
{ 
    return corps_number_; 
}

inline BYTE 
GuildCorps::GetMemberCount()
{ 
    return static_cast<BYTE>(member_map_.size()); 
}

inline BOOL 
GuildCorps::IsExistRearCamp()
{ 
    return is_create_rear_camp; 
}

inline void
GuildCorps::SetRearCamp(BOOL is_camp)
{ 
    is_create_rear_camp = is_camp; 
}

#endif //DBPROXY_GUILD_CORPS_H