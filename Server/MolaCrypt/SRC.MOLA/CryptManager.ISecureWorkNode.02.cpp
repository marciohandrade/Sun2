#include "stdafx.h"

// =======================================================================================


//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

BOOL	ISecureSimplexSNK03::DoEarlyProcess( BYTE*& pSendingMsg, WORD& wSendingSize )
{
	pSendingMsg = NULL;
	wSendingSize = 0;
	return FALSE;
};


// [for CLIENT] Application::Heartbeat에서 호출
VOID		ISecureWorkNode::Update( DWORD dwDeltaTick )
{
	//__DEBUGGING_OUTPUT0( __FUNCTION__ );

#if SERVER_CODE

#elif CLIENT_CODE

#endif //

}



// [for CLIENT] BattleScene일 경우 경우 호출 - Init 계열에서 한번
VOID		ISecureWorkNode::OnChange::Scene( eZONETYPE ztZONE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );

#if SERVER_CODE

#elif CLIENT_CODE

#endif //

}



