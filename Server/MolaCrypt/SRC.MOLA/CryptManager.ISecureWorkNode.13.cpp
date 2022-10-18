#include "stdafx.h"

// =======================================================================================


//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

extern DWORD	dwBUILTIN_QUERY_RESULT;
static MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_ACK		sBUILTIN_MSG_ACK[1];
// [for CLIENT] CG_SECURE_QUERY_BUILTIN_CHK_CMD
BOOL		ISecureWorkNode::OnMsgSend::BuiltinCodeCheck( BYTE*& prSendingMsg, WORD& rwSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );

#if SERVER_CODE
	return FALSE;

#elif CLIENT_CODE

	MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_ACK* pMsgACK = new (sBUILTIN_MSG_ACK) MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_ACK;

#if CODE_TEST(0)
	MSG_CG_SECURE_QUERY_BUILTIN_CHK_ACK msgACK2;
	printf( "%04X\t%04X\n", *(WORD*)pMsgACK, *(WORD*)&msgACK2 );
#endif

#if 1 // code renewal
    const BYTE* src = reinterpret_cast<BYTE*>(&dwBUILTIN_QUERY_RESULT);
    C_ASSERT(sizeof(pMsgACK->m_dwBuiltinACK) == 8 && \
             sizeof(pMsgACK->m_dwBuiltinACK) / sizeof(WORD) == sizeof(dwBUILTIN_QUERY_RESULT));
    WORD* dest = reinterpret_cast<WORD*>(pMsgACK->m_dwBuiltinACK);
    for (int loop = sizeof(pMsgACK->m_dwBuiltinACK) / sizeof(WORD); loop; --loop) {
        *dest++ = *src++;
    }
#else
	copy<BYTE*, WORD*>(
		&((BYTE*)(&dwBUILTIN_QUERY_RESULT))[0], &((BYTE*)(&dwBUILTIN_QUERY_RESULT))[4],
		(WORD*)&(pMsgACK->m_dwBuiltinACK[0]) );
#endif
	prSendingMsg = (BYTE*)pMsgACK;
	rwSIZE = sizeof(*pMsgACK);

	return TRUE;

#endif
}



