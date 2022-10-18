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
//	-	���� ��Ŷ Pack ���, ���ǵ� ��Ŷ�� ���ؼ� ó���� ��� network IO buffer size�� �޶��� �� �ִ�.
//	-	(In Server) GameServer, AgentServer�� ->C �迭 ��Ŷ���� �̰��� ��ġ���� �Ѵ�.
//	-	(In Client) to GameServer ��Ŷ�� ��� �̰��� ��ġ���� �Ѵ�.
PacketInfo
PacketControl::Pack(MSG_BASE* const pMsg, const WORD wSize)
{
	return MakeStream(TRUE, pMsg, wSize);
};

//==================================================================================================
//	<PacketControl::Pack>
//	-	���� ��Ŷ Pack ���, ���ǵ� ��Ŷ�� ���ؼ� ó���� ��� network IO buffer size�� �޶��� �� �ִ�.
//	-	(In Server) from Client �迭 ��Ŷ���� �̰��� ��ġ���� �Ѵ�.
//	-	(In Client) from GameServer ��Ŷ�� ��� �̰��� ��ġ���� �Ѵ�.
PacketInfo
PacketControl::UnPack(MSG_BASE* const pMsg, const WORD wSize)
{
	return MakeStream(FALSE, pMsg, wSize);
};

/*
//==================================================================================================
//	<Sample for Client>
//	-	gameserver->client �� ��쿡��...
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

//	client->gameserver �� ��쿡��...
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
//	- (WAVERIX)(080404)	���� ������ ��Ŷ�� �Ѳ����� ������ �� �ִ� ���� ����
//	-	(��) ATTACK_BRD ���� HP BRD�� ����ǰ� �Ѵ�. �� ��� ������ ���� PACK�� ����Ǵ� ���� �ƴ϶�
//	-		�ϳ��� ��Ŷ�� �� �ΰ����� ū ������ ����ǰ�, Ǯ�� ��, �� ������ �и��Ǵ� ���¸� �� �� �ִ�.
//	-		��� ���� ��Ŷ �ڵ鸵 ������ ���� ������ ���� ������ �ʿ��ϴ�.
//	-	�̰� ��å�� �˰��� ���ֿ� ���Ѵ�.
//
//	-	������ �̰��� �̹��� �������� �ʴ´�.
//	-	�� ���� ���浵 ����� ���� �� �������� ������ ������ ������ ���� ��.
//	-	�������� ������ �׷� ��Ŀ������ ������ ����
//
VOID	OnRecv(const MSG_BASE* pMsg, const WORD wSize)
{
	switch(pMsg->m_byProtocol)
	{
	case CG_COMPOSE_STREAM:
		{
			// ������� ó���� ���� ���
		}
		break;
	}
}
*/

};


