#include "stdafx.h"

// =======================================================================================

//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureSimplexSNK02::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};



BOOL	ISecureWorkNode::DoLatelyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
#if CODE_BACKUP
	__DEBUGGING_OUTPUT0( "ISecureWorkNode::DoLatelyProcess" );
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


