#pragma once

#include <struct.h>
#include "WorldGuildCommon.h"

class WorldGuildMember
{
	__DECL_GUILDPOOL(WorldGuildMember)
public:
	WorldGuildMember(void);
	virtual ~WorldGuildMember(void);

	VOID						Release(){}
	inline	CHARGUID			GetCharGuid() { return m_Info.m_CharGuid;	}

	inline	VOID				SetCorpsNum( BYTE byCorpsNum ){ m_Info.m_byCorpsNum = byCorpsNum; }
	inline	BYTE				GetCorpsNum(){ return m_Info.m_byCorpsNum; }
			VOID				Serialize( INOUT BASE_GUILD_MEMBER_INFO& info, eSERIALIZE se );
	inline	VOID				SetDuty(eGUILD_DUTY eGuildDuty){ m_Info.m_eGuildDuty = eGuildDuty; }
			eGUILD_DUTY			GetDuty(){ return m_Info.m_eGuildDuty; }
	const	char*				GetCharName(){ return m_Info.m_tszCharName; }

    void SetCharName(const char* char_name);

private:
	BASE_GUILD_MEMBER_INFO	m_Info;
};
