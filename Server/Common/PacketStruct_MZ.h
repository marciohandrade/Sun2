#pragma once

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_MZ.h>

#pragma pack(push,1)

struct MSG_MZ_SERVER_INFO_CMD : public MSG_BASE
{
	MSG_MZ_SERVER_INFO_CMD() { m_byCategory = MZ_CONNECTION; m_byProtocol = MZ_SERVER_INFO_CMD; }

	SERVER_KEY		ServerKey;
	DWORD			m_dwServerGUID;

	char			m_szGuildServerInnerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wGuildServerServerInnerPort;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wDBProxyServerPort;
	char			m_szWorldServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wWorldServerPort;
};

#define PACKET_MZ_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_MZ_CLOSE		};

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_MZ_OPEN( MZ_CONNECTION, MZ_CONNECTION_DISCONNECT_CMD )
PACKET_MZ_CLOSE

PACKET_MZ_OPEN( MZ_CONNECTION, MZ_CONNECTION_PHASE_SHIFT_CMD )
DWORD	m_dwPHASE_SHIFT;
PACKET_MZ_CLOSE

PACKET_MZ_OPEN( MZ_CONNECTION, MZ_CONNECTION_BLOCK_CMD )
PACKET_MZ_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_MZ_OPEN( MZ_GUILD, MZ_GUILD_NAME_CHANGE_SYN )
GUILDGUID	m_GuildGuid;
TCHAR		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
PACKET_MZ_CLOSE

PACKET_MZ_OPEN( MZ_GUILD, MZ_GUILD_NAME_CHANGE_ACK )
PACKET_MZ_CLOSE

PACKET_MZ_OPEN( MZ_GUILD, MZ_GUILD_NAME_CHANGE_NAK )
BYTE		m_byErrorCode;
PACKET_MZ_CLOSE


#pragma pack(pop)
