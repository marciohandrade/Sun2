#include "stdafx.h"

// =======================================================================================


//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureQueryMola::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};


// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD
VOID		ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT1( PVOID pPacketMsg, WORD wSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
}


// [for CLIENT] CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD
VOID		ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T1( PVOID pPacketMsg, WORD wSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//

//sRETURN_PTR_SZ		ISecureQueryCmdStreamBuiltinCmd::ExecQueryNResult()
//{
//	sRETURN_PTR_SZ info;
//	info.RET_STREAM = NULL;
//	info.RET_SZ = 0;
//
//	return info;
//}



