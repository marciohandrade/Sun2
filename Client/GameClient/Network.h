
#pragma once

//=======================================================================================================================
/// 클라이언트 소켓모듈을 랩핑한 Network class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 12. 22
	@remarks
			- WindowProc()를 메인 윈도 메시지 프로시저에서 호출되어야 한다.
			- 메시지가 없을때까지 GetNextMsg()루프를 돌면서 버퍼에 있는 패킷을 pop한다.
	@note
			- 여러 연결을 편하게 하기 위해 eCONNECTION_INDEX를 둠
*/
//=======================================================================================================================

enum eCONNECTION_INDEX
{
	CI_GAMESERVERIDX = 1,
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    CI_CHATSERVERIDX = CI_GAMESERVERIDX,
#else
	CI_CHATSERVERIDX,
#endif
	CI_AUTHSERVERIDX,

	CI_MAX,
};

#include <SolarClientNetwork.h>
class SolarClientNetwork;

#include "ClientCryptography.h"

typedef VOID (*fn_Logic_Recv)( DWORD dwIndex, BYTE * pData, DWORD dwSize );

class Network
{
	friend VOID fn_Callback_Recv( DWORD dwIndex, BYTE * pData, DWORD dwSize );

public:
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
	enum	{ MAX_CRYPTOGRAM_STREAM_SIZE = 16000, MAX_CONN_BUFFER_SIZE = 480*1000 };	// 기존 MAX_CONN_BUFFER_SIZE 보다 4배 크게
#else
	enum	{ MAX_CRYPTOGRAM_STREAM_SIZE = 16000, MAX_CONN_BUFFER_SIZE = 120*1000 };
#endif

	Network();
	~Network();

	VOID					Init(fn_Logic_Recv _fn_Recv, fn_Disconnect _fn_Disconnect, fn_Error _fn_Error );
	VOID					Release();
	VOID					Update();
	BOOL					Connect(  eCONNECTION_INDEX idx, char * pszIPAddress, unsigned short usPort, BYTE HeaderSize = 2, BOOL bExcludeHeaderSize = TRUE);
	BOOL					Disconnect( eCONNECTION_INDEX idx, BOOL isCallfn=false);
	BOOL					Send( eCONNECTION_INDEX idx, void * pBuf, int iLength );
	//BOOL					WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	BOOL					IsConnected( eCONNECTION_INDEX idx );
	BOOL					IsCommunicatable( eCONNECTION_INDEX idx );
	const char *            GetLocalAddress(eCONNECTION_INDEX idx);

	BOOL					IsRqstDisconnectAuth() { return m_bRqstDisconnectAuth; }

	inline static BYTE*		GetPacketIOStream() { return ms_PacketIOStream; };
	inline sCLIENT_CRYPT_INFO* GetSendProtocolCryptography() { return m_ProtocolCryptograpyMgr[CI_GAMESERVERIDX].GetSendCryptData(); }
	inline sCLIENT_CRYPT_INFO* GetRecvProtocolCryptography() { return m_ProtocolCryptograpyMgr[CI_GAMESERVERIDX].GetRecvCryptData(); }
	// <HEARTBEAT><PROC>
	VOID					SetEnableSendHeartBeat(bool bEnable);	
	BOOL					ISEnableSendHeartBeat() const			{ return m_bEnableSendHeartBeat; }

	VOID					SetEnableSendChatHeartBeat(bool bEnable);	
	BOOL					ISEnableSendChatHeartBeat() const			{ return m_bEnableSendChatHeartBeat; }

	//int					GetBackDataCount();

	// <CRYPTOGRAPHY><CONTOROL>
	VOID					InitCryptographyInfo( INT nIndex );
	VOID					InitCryptographyInfo( VOID );
	BOOL					SetCryptographyInfo( INT nIndex, INT nCryptoType, MSG_BASE * pMsg );
	VOID					CopyRecvCryptographyInfoFromSend( INT nIndex );

	BOOL					SetCryptographyInfo_SNK(INT nIndex, INT nCryptoType, MSG_BASE * pMsg);
	VOID					CopyRecvCryptographyInfoFromSend_SNK( INT nIndex );
    bool                    GetMACAddress(OUT char* str_address, int buffer_size); 

    // NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
    void ChangeWorldMessageSendableState(bool enabled);
    bool EnabledWorldService() const;
    bool CheckSendableMessage(uint8_t message_category) const; // CI_GAMESERVERIDX
	//////////////////////////////////////////////////////////////////////////////////////////
	// <FIELDs><PUBLIC>
	fn_Logic_Recv				m_fn_Forward_Recv;
private:
	// <STATIC>
	static BYTE					ms_PacketIOStream[MAX_CRYPTOGRAM_STREAM_SIZE];

	// <FIELDs><PRIVATE>
	SolarClientNetwork*			m_pNetwork;
	CProtocolCryptographyMgr	m_ProtocolCryptograpyMgr[CI_MAX];

	struct
	{	// <FIELDs> HEART_BEAT 처리 관련
		bool					m_bEnableSendHeartBeat;
		bool					m_bEnableSendChatHeartBeat;
		DWORD					m_dwHeartBeatTick;
		DWORD					m_dwChatHeartBeatTick;
	};
	bool						m_bGameSvrDisconnectFlag;

	BOOL						m_bRqstDisconnectAuth;	// 내가 Auth에게 Disconnect을 요청하면 TRUE
    bool world_message_sendable_state_; // NOTE: f110916.1L, _NA002676_WORLD_SERVER_RENEWAL
#if USING_GAMEGUARD
    DWORD                       game_guard_tick_;
#endif // USING_GAMEGUARD
};


//==================================================================================================
// NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
inline void Network::ChangeWorldMessageSendableState(bool enabled)
{
    world_message_sendable_state_ = enabled;
}

// NOTE: f110916.1L, added by _NA002676_WORLD_SERVER_RENEWAL
inline bool Network::EnabledWorldService() const
{
    return world_message_sendable_state_;
}