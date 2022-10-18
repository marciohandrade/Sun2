#include "SunClientPrecompiledHeader.h"
#include "network.h"

#include <SolarClientNetwork.h>
#include "Application.h"
#include "InterfaceManager.h"
#include "uiLoginMan/uiLoginMan.h"
#include "version.h"
#include "globalfunc.h"
#include <PacketControl/PacketControl.h>
#include "XignCode/XignCodeApi.h"
#include <iptypes.h>
#include <iphlpapi.h>
#if USING_GAMEGUARD
#include "GameGuard/GameGuardApi.h"
#endif // USING_GAMEGUARD


#pragma comment( lib, "ws2_32.lib" )

#ifdef _DEBUG
    #pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/SolarClientNetwork_d.lib" )
#else
    #pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/SolarClientNetwork.lib" )
#endif

#define WM_SOCKET_ASSYNC_MSG		( WM_USER + 1000)


//==============================================================================================
VOID fn_Callback_Recv( DWORD dwIndex, BYTE * pData, DWORD dwSize )
{
    if (g_pNetwork)
    {
	    using namespace nsPacket;
	    // decode
	    if (g_pNetwork->m_ProtocolCryptograpyMgr[dwIndex].IsUseRecvCryptography())
	    {
		    static SPacketBuffer decodeBuffer;
		    CProtocolCryptographyMgr& rCRYPTMGR = g_pNetwork->m_ProtocolCryptograpyMgr[dwIndex];
		    if( rCRYPTMGR.DecodePacket( decodeBuffer, pData, (INT)dwSize ) )
		    {
			    if( CI_GAMESERVERIDX == dwIndex )
			    {
				    PacketInfo packetInfo = PacketControl::UnPack( (MSG_BASE*)decodeBuffer.PACKET_PTR, (WORD)dwSize );

				    g_pNetwork->m_fn_Forward_Recv( dwIndex, (BYTE*)packetInfo.pMsg, dwSize );
			    }
			    else
			    {
				    g_pNetwork->m_fn_Forward_Recv( dwIndex, decodeBuffer.PACKET_PTR, dwSize );
			    }
		    }
	    }
	    else
	    {
		    g_pNetwork->m_fn_Forward_Recv( dwIndex, pData, dwSize );
	    }
    }
}

//==============================================================================================
Network::Network()
{
	m_pNetwork = NULL;
	m_bEnableSendHeartBeat = FALSE;
	m_bEnableSendChatHeartBeat = FALSE;
    world_message_sendable_state_ = false;
#if USING_GAMEGUARD
    game_guard_tick_ = 0;
#endif // USING_GAMEGUARD
}

Network::~Network()
{
	Release();
}

VOID Network::Init(fn_Logic_Recv _fn_Recv, fn_Disconnect _fn_Disconnect, fn_Error _fn_Error )
{
	if( m_pNetwork == NULL )
	{
		m_fn_Forward_Recv = _fn_Recv;

		m_pNetwork = new SolarClientNetwork;
		SOLAR_CLIENT_DESC desc;
		desc.nMaxConnectionNum = CI_MAX-CI_GAMESERVERIDX;
		desc.nMaxBufferSizePerConn = MAX_CONN_BUFFER_SIZE;
		desc.fnCallBackDisconnect = _fn_Disconnect;
		desc.fnCallBackError = _fn_Error;
		desc.fnCallBackRecv	= fn_Callback_Recv;
		m_pNetwork->InitializeNetwork(desc);
	}

	m_bEnableSendHeartBeat	= FALSE;
	m_bRqstDisconnectAuth	= FALSE;
    world_message_sendable_state_ = false;
#if USING_GAMEGUARD
    game_guard_tick_ = 0;
#endif // USING_GAMEGUARD
}

BOOL Network::Connect(  eCONNECTION_INDEX idx, char * pszIPAddress, unsigned short usPort, BYTE HeaderSize, BOOL bExcludeHeaderSize )
{
	if( !m_pNetwork ) 
    {
        return FALSE;
    }

#if USING_XIGNCODE
    if (idx == CI_GAMESERVERIDX)
    {
        if (XignCodeApi::Connect() == false)
        {
            //! 보안모듈 초기화 실패 끊어야 하나?
            //! 일단 접속 안되게 함
            return FALSE;
        }
    }
#endif //USING_XIGNCODE


	BOOL bConnected = m_pNetwork->Connect( idx, pszIPAddress, usPort, HeaderSize, bExcludeHeaderSize );

	if (bConnected)
	{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
		if( idx != CI_CHATSERVERIDX )
#endif
			m_bEnableSendHeartBeat = FALSE;	
		if(idx == CI_AUTHSERVERIDX)
			m_bRqstDisconnectAuth = FALSE;
	}

	return bConnected;
}

VOID Network::Release()
{
	m_bEnableSendHeartBeat = FALSE;
	m_bEnableSendChatHeartBeat = FALSE;

	if( m_pNetwork )
	{
		m_pNetwork->ReleaseNetwork();
		SAFE_DELETE (m_pNetwork);
	}
}

VOID Network::Update()
{
	DWORD dwCurrentTick = clock_function::GetTickCount();

	if (ISEnableSendHeartBeat() &&IsConnected(CI_GAMESERVERIDX)) 
	{
		if ( (m_dwHeartBeatTick + HEARTBEAT_TICK) <= dwCurrentTick)
		{		
			MSG_CG_CONNECTION_HEARTBEAT_SYN	SendPacket;
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
			m_dwHeartBeatTick  = dwCurrentTick;
		}

	}
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	if (ISEnableSendChatHeartBeat() && IsConnected(CI_CHATSERVERIDX) )
	{
		if ( (m_dwChatHeartBeatTick + HEARTBEAT_TICK) <= dwCurrentTick)
		{
			MSG_CW_HEARTBEAT CW_HeartbeatMsg;
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &CW_HeartbeatMsg, sizeof(CW_HeartbeatMsg));

			m_dwChatHeartBeatTick = dwCurrentTick;

		}
	}
#endif
	if(m_pNetwork)
		m_pNetwork->UpdateNetwork();


	uiLoginMan* loginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));

	if (loginMan && loginMan->IsTryReconnect() && !m_bRqstDisconnectAuth) // 내가 원해서 끊은 경우가 아니면 재접시도
		g_pApplication->ConnectToAuthServer();

#if USING_GAMEGUARD
    if (game_guard_tick_ + GAME_GUARD_CHECK_TICK <= dwCurrentTick)
    {
        DWORD result = GameGuardApi::Instance()->Check();
        if (result != NPGAMEMON_SUCCESS)
        {
            GameGuardApi::Instance()->ErrorCodeResult(result, true);
        }
        game_guard_tick_ = dwCurrentTick;
    }
#endif // USING_GAMEGUARD
	/*
	WORD		wProtocolSize	= 0;
	BYTE *		pPacket			= NULL;
	BYTE *		pRecvProtocol	= NULL;

	for( int iChannel = 0 ; iChannel < CI_MAX ; ++iChannel )
	{
	while ( ( NULL != m_paNetwork2[iChannel] ) && (pPacket = m_paNetwork2[iChannel]->GetFirstPacketPtr()) )
	{
	// 일반 패킷은 size(word) / body 순이므로 헤더의 내용을 제거하고 body만을 넘겨주어야 한다.
	// encoding 된 패킷은 size(word) / encrypt info(word) / body 순이므로 헤더의 내용을 제거하고 body만을 넘겨주어야 한다.
	wProtocolSize	= *(WORD*)pPacket;				// 패킷의 앞 2바이트는 프로토콜의 사이즈임.
	pRecvProtocol	= &pPacket[ sizeof(WORD) ];		// 실제 프로토콜의 시작 지점

	// decode
	if( m_ProtocolCryptograpyMgr[iChannel].IsUseRecvCryptography() )
	{
	static BYTE pDecodeProtocol[MAX_CRYPTOGRAM_STREAM_SIZE];
	ZeroMemory( pDecodeProtocol, MAX_CRYPTOGRAM_STREAM_SIZE );

	if( m_ProtocolCryptograpyMgr[iChannel].DecodePacket( pDecodeProtocol, pRecvProtocol, wProtocolSize ) )
	m_fn_Recv( iChannel, pDecodeProtocol );
	}
	else
	{
	m_fn_Recv( iChannel, pRecvProtocol );
	}

	// 패킷의 제거는 디코딩 성공여부와 관계없이 진행.
	m_paNetwork2[iChannel]->RemoveFirstPacket( wProtocolSize + sizeof(WORD) ); // 패킷의 총 사이즈
	}
	}
	*/
}

BOOL Network::IsConnected( eCONNECTION_INDEX idx )
{
	if (m_pNetwork)
	{
		if(m_pNetwork->IsDisconnected(idx)) 
			return FALSE;
		else
			return TRUE;

	}
	
    return FALSE;
	
}

// 해당 채널과 통신이 가능한가?
BOOL Network::IsCommunicatable( eCONNECTION_INDEX idx )
{
	return ( IsConnected( idx ) && m_ProtocolCryptograpyMgr[idx].IsUseSendCryptography() );
}

BOOL Network::Disconnect( eCONNECTION_INDEX idx, BOOL isCallfn )
{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	if( idx != CI_CHATSERVERIDX )
#endif
		m_bEnableSendHeartBeat = FALSE;
    world_message_sendable_state_ = false;
	if( m_pNetwork )
	{
		BOOL bResult = m_pNetwork->Disconnect(idx);

#if USING_XIGNCODE
        if (idx == CI_GAMESERVERIDX)
        {
            XignCodeApi::Disconnect();
        }
#endif //USING_XIGNCODE

		if(idx == CI_AUTHSERVERIDX)
			m_bRqstDisconnectAuth = TRUE;

		return bResult;
	}
	return FALSE;
}

BOOL Network::Send( eCONNECTION_INDEX idx, void * pBuf, int iLength )
{
    if (( idx >= CI_MAX ) || !m_pNetwork ||
        (iLength < sizeof(MSG_BASE) || iLength > MAX_CRYPTOGRAM_STREAM_SIZE))
    {
		return FALSE;
    }

	if( m_ProtocolCryptograpyMgr[idx].IsUseSendCryptography() )
	{
		static BYTE pEncodingStream[MAX_CRYPTOGRAM_STREAM_SIZE];

		iLength = m_ProtocolCryptograpyMgr[idx].EncodePacket( pEncodingStream, pBuf, iLength );
		return m_pNetwork->SendPacket( idx, (char*)pEncodingStream, iLength );
	}
	else
	{
		return m_pNetwork->SendPacket( idx, (char*)pBuf, iLength );
	}
}

const char * Network::GetLocalAddress(eCONNECTION_INDEX idx )
{
	return m_pNetwork->GetConnectionIP(idx);
}

void   Network::SetEnableSendHeartBeat(bool bEnable)
{
	m_bEnableSendHeartBeat = bEnable; 
	if (bEnable)
	{
		m_dwHeartBeatTick = clock_function::GetTickCount();
	}
}

void   Network::SetEnableSendChatHeartBeat(bool bEnable)
{
	m_bEnableSendChatHeartBeat = bEnable; 
	if (bEnable)
	{
        
		m_dwChatHeartBeatTick = clock_function::GetTickCount();
	}
}


// 암호화 관련
void	Network::InitCryptographyInfo( int nIndex )
{
	if( nIndex < CI_MAX )
		m_ProtocolCryptograpyMgr[nIndex].InitData();
}

void	Network::InitCryptographyInfo( void )
{
	for( int i = 0; i < CI_MAX; ++i )
		m_ProtocolCryptograpyMgr[i].InitData();
}

BOOL	Network::SetCryptographyInfo( int nIndex, int nCryptoType, MSG_BASE * pMsg )
{
	if( nIndex < CI_MAX )
		return m_ProtocolCryptograpyMgr[nIndex].SetCryptographyInfo( nCryptoType, pMsg );
	else
		return FALSE;
}

void	Network::CopyRecvCryptographyInfoFromSend( int nIndex )
{
	if( nIndex < CI_MAX )
		m_ProtocolCryptograpyMgr[nIndex].CopyRecvCryptographyInfoFromSend();
}

BOOL	Network::SetCryptographyInfo_SNK(int nIndex, int nCryptoType, MSG_BASE * pMsg)
{
	if( nIndex < CI_MAX )
	{
		return m_ProtocolCryptograpyMgr[nIndex].SetCryptographyInfo_SNK( nCryptoType, pMsg );
	}
	else
	{
		return FALSE;
	}
}

void	Network::CopyRecvCryptographyInfoFromSend_SNK( int nIndex )
{
	if( nIndex < CI_MAX )
		m_ProtocolCryptograpyMgr[nIndex].CopyRecvCryptographyInfoFromSend_SNK();
}
//------------------------------------------------------------------------------ 
bool Network::GetMACAddress(OUT char* str_address, int buffer_size)
{
    IP_ADAPTER_INFO AdapterInfo[16];    // Allocate information for up to 16 NICs

    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

    // Call GetAdapterInfo
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo,   // [out] buffer to receive data
                                    &dwBufLen);     // [in] size of receive data buffer
    if (dwStatus != ERROR_SUCCESS)
    {
        assert(dwStatus == ERROR_SUCCESS);  // Verify return value is valid, no buffer overflow
        return false;
    }

    //! 첫번째 것만
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
    Snprintf(str_address, buffer_size, "%2hx-%2hx-%2hx-%2hx-%2hx-%2hx",
            pAdapterInfo->Address[0], 
            pAdapterInfo->Address[1], 
            pAdapterInfo->Address[2], 
            pAdapterInfo->Address[3], 
            pAdapterInfo->Address[4], 
            pAdapterInfo->Address[5]);


    //PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
    //do {
    //    pAdapterInfo->Address;                  // Print MAC address
    //    pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
    //}while(pAdapterInfo);                    // Terminate if last adapter

    return true;
}

bool Network::CheckSendableMessage(uint8_t message_category) const // CI_GAMESERVERIDX
{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    return true;
#endif
#ifdef _NA002676_WORLD_SERVER_RENEWAL
  #if !defined(_NA000000_CW_PROTOCOL_UNIFICATION)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
    // CHANGES: CI_CHATSERVERIDX same with CI_GAMESERVERIDX since _NA002676_WORLD_SERVER_RENEWAL.
    // To support the accurate world service synchronization added the CW protocol filters
    switch (message_category)
    {
    case CW_CONNECTION   :
    case CW_CHAT         :
    case CW_GM           :
    case CW_VIEWPORT     :
    case CW_ARMORKIT     :
    case CW_FRIEND       :
    case CW_GUILD        :
    case CW_CRYPTOGRAPHY :
    case CW_COMMUNITY    :
    case CW_REVENGE      :
    case CW_MEMO         :
    //case CW_SYNC       :
    case CW_GROUP        :
    case CW_LOTTO        :
    case CW_PARTY        :
        return this->EnabledWorldService();
    };
    return true;
#endif
}
