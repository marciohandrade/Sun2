#include "stdafx.h"

// =======================================================================================

//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureSimplexSNK01::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};




// [for CLIENT] Client::Application::Heartbeat에서 호출 (TRUE, PTR, SZ) 모두 참일 경우 패킷 전송
BOOL	ISecureWorkNode::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
#if CODE_BACKUP
	__DEBUGGING_OUTPUT0( "ISecureWorkNode::DoEarlyProcess" );
	if( m_pSendCrypt && m_pRecvCrypt )
	{
		if( memcmp( &m_pSendCrypt->m_SNKey, &m_pRecvCrypt->m_SNKey, sizeof(sCLIENT_CRYPT_INFO) ) != 0 )
		{
			__DEBUGGING_OUTPUT1( "MEMCMP SEND", *(DWORD*)&m_pSendCrypt->m_SNKey );
			__DEBUGGING_OUTPUT1( "MEMCMP RECV", *(DWORD*)&m_pRecvCrypt->m_SNKey );
		}
	}
#endif


#if SERVER_CODE

	pSendingMsg = NULL;
	wSendingSize = 0;

#elif CLIENT_CODE

	pSendingMsg = NULL;
	wSendingSize = 0;

#endif

	return FALSE;
};




