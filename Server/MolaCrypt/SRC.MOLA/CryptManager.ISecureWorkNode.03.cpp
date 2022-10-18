#include "stdafx.h"

// =======================================================================================


//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureSimplexSNK04::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
//
//
//
//
//
//

DWORD	dwBUILTIN_QUERY_RESULT = 0;
// [for CLIENT] CG_SECURE_QUERY_BUILTIN_CHK_CMD
VOID		ISecureWorkNode::OnMsgRecvSignal::BuiltinCodeCheck( PVOID pPacketMsg, WORD wSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
	#if CODE_TEST(FALSE)
		printf( "Received1\n" );
	#endif

#if SERVER_CODE

#elif CLIENT_CODE

	#if CODE_TEST(FALSE)
		printf( "Received\n" );
	#endif

	const WORD PROTOCOL_CMD = MAKEWORD( CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_CMD );
	MSG_CG_SECURE_QUERY_BUILTIN_CHK_CMD* pMSG = (MSG_CG_SECURE_QUERY_BUILTIN_CHK_CMD*)pPacketMsg;
	if( PROTOCOL_CMD == MAKEWORD(pMSG->m_byCategory, pMSG->m_byProtocol) )
	{
		DWORD dwIdx = *(DWORD*)&pMSG->m_dwBuiltinCMD[0];
		DWORD dwKey = *(DWORD*)&pMSG->m_dwBuiltinCMD[4];
		if( GET_ARRAY_SIZE(BuiltinQueryFunc, BUILTIN_QUERY_FUNCs) > dwIdx )
		{
			BuiltinQueryFunc func = BUILTIN_QUERY_FUNCs[dwIdx];
			dwBUILTIN_QUERY_RESULT = func( dwKey );
		}
	}
#endif

#if CODE_TEST(FALSE)
	printf( "CG_SECURE_QUERY_BUILTIN_CHK_CMD::VALUE %08X %08X\n",
		*(DWORD*)&pMSG->m_dwBuiltinCMD[0], *(DWORD*)&pMSG->m_dwBuiltinCMD[4] );
#endif //

}








//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	서버 클라이언트 공통 코드
//	BUILTIN QUERY FUNCTION

//// prototype
//DWORD	BuiltinQueryFunc001_0x010338F1( DWORD dwInputKey )
//{
//	return 0x010338F1+dwInputKey;
//}

#pragma warning(push)
#pragma warning(disable:4307)

#	include "BuiltinCmd/BuiltinQueryFuncImpl.def"

#define DECL_BUILTIN_QUERY_NODE(index, key, funcname)	&##funcname,


BuiltinQueryFunc	BUILTIN_QUERY_FUNCs[1024] =
{
#	include "BuiltinCmd/BuiltinQueryFuncDecl.def"
};

#pragma warning(pop)


