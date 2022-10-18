#ifndef __PACKETSTRUCT_MP_H__
#define __PACKETSTRUCT_MP_H__

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_MP.h>

#pragma pack(push,1)


#define PACKET_MP_OPEN( c, p )	struct MSG_##p : public MSG_BASE { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_MP_CLOSE		};

PACKET_MP_OPEN( MP_CONNECTION, MP_CONNECTION_SERVER_INFO_CMD )
	SERVER_KEY		m_ServerKey;
	DWORD			m_dwServerGUID;
	char			m_szPortalServerInnerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wPortalServerInnerPort;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wDBProxyServerPort;
PACKET_MP_CLOSE


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_MP_OPEN( MP_CONNECTION, MP_CONNECTION_DISCONNECT_CMD )
PACKET_MP_CLOSE

PACKET_MP_OPEN( MP_CONNECTION, MP_CONNECTION_PHASE_SHIFT_CMD )
	DWORD	m_dwPHASE_SHIFT;
PACKET_MP_CLOSE

PACKET_MP_OPEN( MP_CONNECTION, MP_CONNECTION_BLOCK_CMD )
PACKET_MP_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#pragma pack(pop)

#endif // __PACKETSTRUCT_MP_H__