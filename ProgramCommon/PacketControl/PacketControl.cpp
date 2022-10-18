#include "stdafx.h"
#include "PacketControl.h"

#include <zlib.h>

#include <SkillInfoParser.h> // reserve deletion, used by 'PacketControl_Skill.hxx'

#ifdef _SERVER
    #define SUBSTRACT_SIZE  (0)
#else
    #define SUBSTRACT_SIZE  (sizeof(DWORD))
#endif


//==================================================================================================
//

//==================================================================================================
//
struct MSG_BASE;

//////////////////////////////////////////////////////////////////////////
#include "CategoryLinkPair.h"
#include "PacketControl_Compress.h"
#include "PacketControl_Types.hxx"

namespace nsPacket
{

//==================================================================================================
//
BOOLEAN
Environment::Init()
{
	static BOOLEAN	sbInit = FALSE;
	if(sbInit)
		return TRUE;
	sbInit = TRUE;

	__zlib_register_malloc(&CompressUtil::memalloc_handler);
	__zlib_register_free(&CompressUtil::memfree_handler);

	return TRUE;
}

//==================================================================================================
//	<PacketControl::Pack_Unpack>
PacketInfo
PacketControl::MakeStream(const BOOLEAN isPack, MSG_BASE* const pMsg, const WORD wSize)
{
	static BOOLEAN	sbInit = Environment::Init();

	static BYTE	s_pPackStream[PacketControl::SIZEofSTREAM];
	static BYTE	s_pUnPackStream[PacketControl::SIZEofSTREAM];

	BYTE* const pIOStream = isPack ? s_pPackStream : s_pUnPackStream;

	PacketInfo packetInfo = { pMsg, wSize };

	const WORD protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);
	switch(protocol)
	{
	//case blah~:
	//	break;
	case MAKEWORD(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_KEY):
	case MAKEWORD(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_CMD):
		packetInfo.pMsg = pMsg;
		packetInfo.Size = wSize;
		break;
	default:
		{
			packetInfo = CompressUtil::MakeStream(isPack, pIOStream, pMsg, wSize);
		}
		break;
	}

	return packetInfo;
};

//==================================================================================================
//	<PacketControl::Pack>
//	-	단일 패킷 Pack 방안, 정의된 패킷에 대해서 처리된 결과 network IO buffer size와 달라질 수 있다.
//	-	(In Server) GameServer, AgentServer의 ->C 계열 패킷들은 이것을 거치도록 한다.
//	-	(In Client) to GameServer 패킷들 모두 이것을 거치도록 한다.
PacketInfo
PacketControl::Pack(MSG_BASE* const pMsg, const WORD wSize)
{
	return MakeStream(TRUE, pMsg, wSize);
};

//==================================================================================================
//	<PacketControl::Pack>
//	-	단일 패킷 Pack 방안, 정의된 패킷에 대해서 처리된 결과 network IO buffer size와 달라질 수 있다.
//	-	(In Server) from Client 계열 패킷들은 이것을 거치도록 한다.
//	-	(In Client) from GameServer 패킷들 모두 이것을 거치도록 한다.
PacketInfo
PacketControl::UnPack(MSG_BASE* const pMsg, const WORD wSize)
{
	return MakeStream(FALSE, pMsg, wSize);
};

/*
//==================================================================================================
//	<Sample for Client>
//	-	gameserver->client 인 경우에만...
VOID fn_Callback_Recv(DWORD dwIndex, BYTE * pData, DWORD dwSize)
{
	// decode
	if(g_pNetwork->m_ProtocolCryptograpyMgr[dwIndex].IsUseRecvCryptography())
	{
		static SPacketBuffer decodeBuffer;
		CProtocolCryptographyMgr& rCRYPTMGR = g_pNetwork->m_ProtocolCryptograpyMgr[dwIndex];
		if(rCRYPTMGR.DecodePacket(decodeBuffer, pData, (INT)dwSize))
		{
			if(CI_GAMESERVERIDX == idx)
			{
				PacketInfo packetInfo = PacketControl::UnPack(decodeBuffer.PACKET_PTR, (WORD)dwSize);
				g_pNetwork->m_fn_Forward_Recv(dwIndex, packetInfo.pMsg);
			}
			else
			{
				g_pNetwork->m_fn_Forward_Recv(dwIndex, decodeBuffer.PACKET_PTR);
			}
		}
	}
	else
	{
		g_pNetwork->m_fn_Forward_Recv(dwIndex, pData);
	}
}

//	client->gameserver 인 경우에만...
BOOL GlobalFunc::SendPacket(eCONNECTION_INDEX idx, void * pBuf, int iLength)
{
	// sample packet
	//	MSG_CG_TEST_PACKET packet;
	//	packet.field0 = xxx;
	//	packet.field1 = xxx;
	//	packet.field2 = xxx;

	if(CI_GAMESERVERIDX == idx)
	{
		PacketInfo packetInfo = PacketControl::Pack(pMsg, wSize);
		return g_pNetwork->Send(idx, (VOID*)packetInfo.pMsg, (INT)packetInfo.Size);
	}
	return g_pNetwork->Send(idx, pBuf, iLength);
}

//
//	- (WAVERIX)(080404)	여러 종류의 패킷을 한꺼번에 압축할 수 있는 형태 지원
//	-	(예) ATTACK_BRD 이후 HP BRD가 연결되곤 한다. 이 경우 각각에 대한 PACK이 수행되는 것이 아니라
//	-		하나의 패킷에 이 두가지가 큰 비율로 압축되고, 풀릴 때, 두 가지로 분리되는 형태를 들 수 있다.
//	-		적어도 기존 패킷 핸들링 각각에 대해 변동이 없는 방향이 필요하다.
//	-	이건 정책성 알고리즘 범주에 속한다.
//
//	-	하지만 이것은 이번에 적용하지 않는다.
//	-	현 시점 설득도 힘들고 또한 현 구조에서 말끔한 구조가 나오지 않을 듯.
//	-	무엇보다 서버에 그런 매커니즘이 전무한 실정
//
VOID	OnRecv(const MSG_BASE* pMsg, const WORD wSize)
{
	switch(pMsg->m_byProtocol)
	{
	case CG_COMPOSE_STREAM:
		{
			// 재귀적인 처리를 위한 방안
		}
		break;
	}
}
*/

};


