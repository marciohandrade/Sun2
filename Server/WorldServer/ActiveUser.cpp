#include "StdAfx.h"
#include ".\activeuser.h"
#include "PacketHandler.h"

//OnRecv
#include <PacketStruct.h>

// 암호화 때문에 include
#include <PacketStruct_CW.h>
#include "WorldServerInfoParser.h"

ActiveUser::ActiveUser(void)
{
}

ActiveUser::~ActiveUser(void)
{
}

VOID ActiveUser::Init()
{
	m_bUseCryptography = FALSE;

	User::Init();
}

VOID ActiveUser::Release()
{
	User::Release();
}

VOID ActiveUser::Update()
{
	User::Update();
}

VOID ActiveUser::OnAccept(DWORD dwNetworkIndex)
{
	ASSERT( !"들어올 수 없는 부분!" );
}

VOID ActiveUser::OnRecv( BYTE *pMsg, WORD wSize )
{
	union
	{
		MSG_BASE*			pBASE;
		MSG_BASE_INTERNAL*	pINTERNAL;
		BYTE*				pBYTE;
		MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED*	pENCODED;
	};

	BOOL bCrypted = WorldServerInfoParser::Instance()->IsCryptable();

	if( bCrypted )
		pENCODED = PACKET_CONTROL::GetPacketPtr_C2A_ENCODED();
	else
		pBYTE = PACKET_CONTROL::GetPacketPtr_C2A();

	if( pBYTE != pMsg )
		CopyMemory( pBYTE, pMsg, (SIZE_T)wSize );

	pMsg = pBYTE;


	if( bCrypted && GetCryptography() )
	{
		//MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED* pEncodedMsg = (MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED*)pMsg;
		if( m_sCrypt.GetRecvSNKey() == pENCODED->GetSNKey() )
		{
			DWORD size = (DWORD)wSize - pENCODED->GetHeaderSize();
			if( size && (size < 16000)
				&& Decrypt( pENCODED->GetPacketStream(), size, pENCODED->GetCRC() ) )
			{
				//ResetSendIdleCheck();
				//MSG_BASE * pMsgBase = (MSG_BASE *)pMsg;
				//g_AgentServer.GetPacketLogger()->LOG_Arg( "From[U:%s:C:%d:P:%d]Client", GetUserID(), pMsgBase->m_byCategory, pMsgBase->m_byProtocol );

				// protect memory scan - in client MIN=4?
				//*(WORD*)pMsg = 0;

				m_sCrypt.NextRoundSNKey4Recv();	// 다음에 사용할 SN키를 생성한다.

				pBASE = PACKET_CONTROL::GetPacketPtr_BASE_C2A();
				size += (DWORD)sizeof(DWORD);
				if(!PacketHandler::Instance()->ParsePacket_CW( this, pBASE, (WORD)size ))
				{
					MessageOut( eCRITICAL_LOG, "[%s][Guid:%u][ZT:%u] Client Packet Parse Error !(C:%u)(P:%u)", GetCharName().c_str(), 
																							GetGUID(), 
																							GetStatus(),
																							pBASE->m_byCategory, 
																							pBASE->m_byProtocol );
					Logout();
				}

				// protect memory scan
				//if( size<sizeof(DWORD)	memset(pEncodedMsg->GetPacketStream(), 0, wSize);
				//else *(DWORD*)pEncodedMsg->GetPacketStream() = 0;

				return;
			}
		}
		// 정상적인 패킷이 아니다.
		// 이후 작동할 로직은?
		Logout();;
	}
	else
	{
		MSG_BASE *pRecvMsg = (MSG_BASE*)pMsg;

		if( !PacketHandler::Instance()->ParsePacket_CW( this, (MSG_BASE*)pMsg, wSize ) )
		{
			MessageOut( eCRITICAL_LOG, "[%s][Guid:%u][ZT:%u] Client Packet Parse Error !(C:%u)(P:%u)", GetCharName().c_str(), 
																					GetGUID(), 
																					GetStatus(),
																					pRecvMsg->m_byCategory, 
																					pRecvMsg->m_byProtocol );
			Logout();
		}
	}
}