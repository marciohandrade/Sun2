#ifndef __PACKETSTRUCT_DM_H__
#define __PACKETSTRUCT_DM_H__

#include <ServerStruct.h>
#include <PacketStruct.h>

#include "Protocol_DM.h"

#pragma pack(push,1)


struct MSG_DM_CONNECTION_SERVER_INFO_CMD : public MSG_BASE
{
	SERVER_KEY		m_ServerKey;
	DWORD			m_dwServerGUID;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wDBProxyServerPort;
};
/*
struct MSG_DM_OPERATION_SERVERSHUTDOWN_REQ : public MSG_BASE
{
	MSG_DM_OPERATION_SERVERSHUTDOWN_REQ() { m_byCategory = DM_OPERATION; m_byProtocol = DM_OPERATION_SERVERSHUTDOWN_REQ; }
};

struct MSG_DM_OPERATION_SERVERSHUTDOWN_ANS : public MSG_BASE
{
	MSG_DM_OPERATION_SERVERSHUTDOWN_ANS() { m_byCategory = DM_OPERATION; m_byProtocol = DM_OPERATION_SERVERSHUTDOWN_ANS; }
};
*/


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#define PACKET_DM_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_DM_CLOSE		};


PACKET_DM_OPEN( DM_CONNECTION, DM_CONNECTION_DISCONNECT_CMD )
PACKET_DM_CLOSE

PACKET_DM_OPEN( DM_CONNECTION, DM_CONNECTION_PHASE_SHIFT_CMD )
DWORD	m_dwPHASE_SHIFT;
PACKET_DM_CLOSE

PACKET_DM_OPEN( DM_CONNECTION, DM_CONNECTION_BLOCK_CMD )
PACKET_DM_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#pragma pack(pop)

#endif	// __PACKETSTRUCT_DM_H__