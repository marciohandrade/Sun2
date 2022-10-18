#include "stdafx.h"

// =======================================================================================


//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureConfigureHeartbeat::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};




// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD
VOID		ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT3( PVOID pPacketMsg, WORD wSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
}

// [for CLIENT] CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD
VOID		ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T3( PVOID pPacketMsg, WORD wSIZE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
}




