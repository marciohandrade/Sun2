#pragma once
#pragma pack(push,1)

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_WZ.h>


#define PACKET_WZ_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_WZ_CLOSE		};


PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_DESTROY_GUILD_CMD )
	GUILDGUID		m_GuildGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_JOIN_GUILD_CMD )
	GUILDGUID				m_GuildGuid;
	BASE_GUILD_MEMBER_INFO	m_Info;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_WITHDRAW_GUILD_CMD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_WZ_CLOSE

// 길드 공지
PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_NOTICE_CMD )
	GUILDGUID		m_GuildGuid;
	WORD			m_wNoticeLen;
	TCHAR			m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1];
	WORD			GetSize()
	{
		return ( sizeof( MSG_WZ_GUILD_NOTICE_CMD ) - ( sizeof(TCHAR) * ( MAX_GUILDNOTICE_LENGTH - m_wNoticeLen ) ) );
	}
PACKET_WZ_CLOSE


#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHUNTING_PVP_RESULT_CMD )
	GUILDGUID			m_GuildGuid;
	CHUNTING_PVP_INFO	m_PVPInfo;
PACKET_WZ_CLOSE
#endif

//길드 직책 설정
PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHANGE_DUTY_CMD )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_TargetCharGuid;
	eGUILD_DUTY		m_eGuildDuty;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHANGE_MASTER_CMD )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_OldMasterGuid;
	CHARGUID		m_NewMasterGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_KICK_PLAYER_CMD )
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_KickCharGuid;
PACKET_WZ_CLOSE

//길드 군단 생성
PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHANGE_COMMANDER_CMD )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID		m_TargetCharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHANGE_CORPS_CMD )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
	CHARGUID		m_TargetCharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CREATE_REAR_CAMP_CMD )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_DESTROY_REAR_CAMP_CMD )
	GUILDGUID		m_GuildGuid;
	BYTE			m_byCorpsNum;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CREATE_CMD )
	WORLD_GUILD_INFO		m_WorldGuildInfo;
	BASE_GUILD_MEMBER_INFO	m_BaseMemberInfo;
PACKET_WZ_CLOSE


PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_RESTORE_SYN )
	GUILDGUID				m_GuildGuid;
	CHARGUID				m_CharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_RESTORE_ACK )
	WORLD_GUILD_INFO				m_WorldGuildInfo;
	BASE_GUILD_MEMBER_INFO			m_BaseMemberInfo;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_RESTORE_NAK )
	BYTE					m_byErrorCode;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_MEMBER_INFO_REQ_SYN )
	GUILDGUID				m_GuildGuid;
	CHARGUID				m_CharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_MEMBER_INFO_REQ_ACK )
	GUILDGUID				m_GuildGuid;
	BASE_GUILD_MEMBER_INFO	m_BaseMemberInfo;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_LOGIN_CHAR_CMD )
	WORLD_GUILD_INFO		m_WorldGuildInfo;
	BASE_GUILD_MEMBER_INFO	m_BaseMemberInfo;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_LOGOUT_CHAR_CMD )
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_CharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_DESTROY_WAIT_CMD)
	GUILDGUID		m_GuildGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_RECOVERY_GUILD_CMD )
	GUILDGUID		m_GuildGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_GUILDMARK_REGISTER_CMD)
	GUILDGUID m_GuildGuid;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_GUILDMARK_DELETE_CMD)
	GUILDGUID m_GuildGuid;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_WAREHOUSE_OPEN_CMD)
	GUILDGUID m_GuildGuid;
	CHARGUID m_OpenUserKey;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_WAREHOUSE_CLOSE_CMD)
	GUILDGUID m_GuildGuid;
	CHARGUID m_CloseUserKey;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_SYN)
	GUILDGUID	m_GuildGuid;
	CHARGUID	m_OperCharGuid;
	WORD		m_wNoticeLen;
	TCHAR		m_szGuildNotice[MAX_GUILDNOTICE_LENGTH+1];
	WORD		GetSize()
	{
		// MAX_GUILDNOTICE_LENGTH 에 +1를 안해주는 이유는 공지 문자열 맨끝에 null도 포함시키기 위해서다.
		// 원래 사이즈는 이렇다.( (MAX_GUILDNOTICE_LENGTH + 1) - (m_wNoticeLen + 1) ) 이걸 약식으로 처리하면
		// 아래와 같다.
		return sizeof(MSG_WZ_GUILD_CHANGE_NOTICE_SYN) - (MAX_GUILDNOTICE_LENGTH - m_wNoticeLen )*sizeof(TCHAR);
 	}
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_CMD)
	eGUILD_NOTICE	m_eNoticeType;	
	GUILDGUID m_GuildGuid;
	WORD m_wNoticeLen;
	TCHAR m_szGuildNotice[MAX_GUILDNOTICE_LENGTH+1];
	WORD GetSize()
	{
		return sizeof(MSG_WZ_GUILD_CHANGE_NOTICE_SYN) - (MAX_GUILDNOTICE_LENGTH - m_wNoticeLen )*sizeof(TCHAR);
	}
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_NAK)
	BYTE m_byErrorCode;
	CHARGUID m_OperCharGuid;
PACKET_WZ_CLOSE

// __NA000000_20081106_GUILD_NAME_CHANGE
// 길드명 변경
PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_NAME_CHANGE_CMD)
GUILDGUID	m_GuildGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
WORD GetSize()
{
	return sizeof(MSG_WZ_GUILD_NAME_CHANGE_CMD) - (MAX_GUILDNAME_LENGTH+1)*sizeof(TCHAR);
}
PACKET_WZ_CLOSE

PACKET_WZ_OPEN(WZ_GUILD, WZ_GUILD_RENAME_CHAR_CMD)
    GUILDGUID	guildguid;
    CHARGUID	charguid;
    TCHAR       character_name_[MAX_CHARNAME_LENGTH + 1];
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_DOMINATION_RETURNHIME_CMD )
    MAPCODE 				m_MapCode;
    TCHAR		            m_tszGuild[MAX_GUILDNAME_LENGTH+1];
    BYTE                    m_DataCount;
    DOMINATION_RETURNMONEY_INFO  m_ReturnInfo[MAX_DOMINATION_AUCTION];
    uint16_t GetSize() const
    {
        assert(m_DataCount <= _countof(m_ReturnInfo));
        const size_t kMinSize = sizeof(*this) - sizeof(m_ReturnInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_ReturnInfo[0]) * m_DataCount);
    }
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_DOMINATION_RETURNHIME_ACK )
RC::eDOMINATION_RESULT  m_RsultCode;                        //결과값 코드
MAPCODE 				m_MapCode;
PACKET_WZ_CLOSE

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN )
    GUILDGUID       m_guildGuid;
    CHARGUID		m_dwToCharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK )
    CHARGUID		m_dwToCharGuid;
PACKET_WZ_CLOSE

PACKET_WZ_OPEN( WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK )
	BYTE            m_byErrorCode;
PACKET_WZ_CLOSE

#pragma pack(pop)
