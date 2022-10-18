#pragma once
#ifndef DBPROXY_GUILD_MEMBER_H
#define DBPROXY_GUILD_MEMBER_H

/*=============================================================================================================================
GuildMember class

	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 26
	@remark
		- 길드의 구성원을 나타내는 클래스
	@note
		- 길드와 마찬가지로 실시간 처리를 한다.
		- OnXXX 계열의 함수는 DBR로 결과값이 온 후 호출되는 콜벡스타일의 함수이다.
		- 
	@history 
		- 

=============================================================================================================================*/

#include "GuildCommon.h"

enum eGUILD_MEMBER_STATE
{
	eGUILD_MEMBER_STATE_READY,
	eGUILD_MEMBER_STATE_START,
	eGUILD_MEMBER_STATE_END,
};

struct BASE_GUILD_MEMBER_INFO;
struct GUILD_MEMBER_INFO;

class GuildMember
{
	__DECL_GUILDPOOL(GuildMember)
	friend class Guild;
	friend class GuildManager;
public:
	GuildMember(void);
	virtual ~GuildMember(void);

	// 풀에서 가져와서 초기화
	void InitializeMember();

	void ReleaseMember();

	void Join(
        CHARGUID char_guid, 
        const TCHAR* char_name, 
        BYTE char_class, 
        LEVELTYPE char_level
        );

public:
    const TCHAR* GetCharName() const;

	BYTE GetCorpsNumber() const;
	
    CHARGUID GetCharGuid() const;
	
    eGUILD_DUTY GetDuty() const;
	
    BYTE GetCharState() const;
	
    BYTE GetTotalInfoReqCnt()const;
	
    LEVELTYPE GetLevel() const;
	
    BYTE GetClass() const;
	
    BOOL IsLogIn() const;
	
	void SetCorpsNumber(
        BYTE corps_num
        );

	void SetChaoState(
        BYTE chao_state
        );

	void SetDuty(
        eGUILD_DUTY duty
        );

	void SetCharGuid(
        CHARGUID char_guid
        );

	void SetTotalInfoReqCnt(
        BYTE request_count
        );

	void GetDelegateRight(
        OUT GUILD_RIGHT_USE_INFO* delegate_right
        );

	void SetClass(
        BYTE char_class
        );

	void SetLevel(
        LEVELTYPE level
        );
	
	bool CheckDelegateRight(
        eGUILD_RIGHTS guild_right
        );

	void SerializeServerMemberInfo(
        INOUT SERVER_GUILD_MEMBER_INFO& server_member_info, 
        eSERIALIZE se
        );

	void SerializeBaseMemberInfo(
        INOUT BASE_GUILD_MEMBER_INFO& base_member_info, 
        eSERIALIZE se
        );

	void SerializeMemberInfo(
        INOUT GUILD_MEMBER_INFO& member_info, 
        eSERIALIZE se
        );

	void SetDelegateRight(
        GUILD_RIGHT_USE_INFO* delegate_right
        );

	void SetCharName(
        const TCHAR* char_name
        );

	void SetLogin(
        bool is_login
        );

	void InitDelegateRight();

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    void  SetGuildCoin(DWORD guild_coin);
    DWORD GetGuildCoin() const;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

private:
	SERVER_GUILD_MEMBER_INFO member_info_;
	BYTE total_info_request_count_;
};

inline const TCHAR* 
GuildMember::GetCharName() const 
{ 
    return member_info_.m_tszCharName; 
}

inline BYTE 
GuildMember::GetCorpsNumber() const 
{ 
    return member_info_.m_byCorpsNum; 
}

inline CHARGUID 
GuildMember::GetCharGuid() const 
{ 
    return member_info_.m_CharGuid; 
}

inline eGUILD_DUTY 
GuildMember::GetDuty() const 
{ 
    return member_info_.m_eGuildDuty; 
}

inline BYTE 
GuildMember::GetCharState() const 
{ 
    return member_info_.m_byChaoState; 
}

inline BYTE 
GuildMember::GetTotalInfoReqCnt() const 
{ 
    return total_info_request_count_; 
}

inline LEVELTYPE 
GuildMember::GetLevel() const 
{ 
    return member_info_.m_Level; 
}

inline BYTE 
GuildMember::GetClass() const 
{ 
    return member_info_.m_byClass; 
}

inline BOOL 
GuildMember::IsLogIn() const 
{ 
    return member_info_.m_bLogin; 
}

inline void 
GuildMember::SetCorpsNumber(BYTE corps_num) 
{ 
    member_info_.m_byCorpsNum = corps_num; 
}

inline void 
GuildMember::SetChaoState(BYTE chao_state) 
{ 
    member_info_.m_byChaoState = chao_state; 
}

inline void 
GuildMember::SetDuty(eGUILD_DUTY duty) 
{ 
    member_info_.m_eGuildDuty = duty; 
}

inline void 
GuildMember::SetCharGuid(CHARGUID char_guid) 
{ 
    member_info_.m_CharGuid = char_guid; 
}

inline void 
GuildMember::SetTotalInfoReqCnt(BYTE request_count) 
{ 
    total_info_request_count_ = request_count; 
}

inline void 
GuildMember::SetClass(BYTE char_class) 
{ 
    member_info_.m_byClass = char_class; 
}

inline void 
GuildMember::SetLevel(LEVELTYPE char_level) 
{ 
    member_info_.m_Level = char_level; 
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
inline void  GuildMember::SetGuildCoin(DWORD guild_coin)
{
    member_info_.guild_coin_ = guild_coin; 
}

inline DWORD GuildMember::GetGuildCoin() const
{
    return member_info_.guild_coin_; 
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#endif //DBPROXY_GUILD_MEMBER_H