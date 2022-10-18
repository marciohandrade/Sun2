#pragma once

#include "GameGuildCommon.h"

class GameGuild;

class GameGuildMember
{
	__DECL_GUILDPOOL(GameGuildMember)
	friend class GameGuild;
public:
	GameGuildMember(void);
	virtual ~GameGuildMember(void);

	
	inline	CHARGUID	GetCharGuid() const	{ return m_Info.m_CharGuid;	}

	inline	eGUILD_DUTY	GetDuty() const { return m_Info.m_eGuildDuty; }
	inline	VOID		SetDuty(eGUILD_DUTY eDuty){ m_Info.m_eGuildDuty = eDuty; }
	const	char*		GetCharName() const { return m_Info.m_tszCharName; }
    void SetCharName(const char* char_name);

	inline	BYTE		GetCorpsNum() const { return m_Info.m_byCorpsNum; }
	inline	VOID		SetCorpsNum( BYTE byCorpsNum ){ m_Info.m_byCorpsNum = byCorpsNum; }

			VOID		Init( const BASE_GUILD_MEMBER_INFO& rInfo );
            VOID		Release();

#ifdef _NA_008334_20150608_SONNENSCHEIN
            VOID SetFlag_OptionActive(BOOL flag) {option_applied_flag_ = flag;}
            BOOL GetFlag_RewardOption() {return option_applied_flag_;}
#endif //_NA_008334_20150608_SONNENSCHEIN
private:
	BASE_GUILD_MEMBER_INFO	m_Info;

#ifdef _NA_008334_20150608_SONNENSCHEIN
    BOOL option_applied_flag_;  //可记 利侩 敲贰弊 TRUE : 可记利侩等 惑怕, FALSE : 可记固利侩惑怕
#endif //_NA_008334_20150608_SONNENSCHEIN
};
