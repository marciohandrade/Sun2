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


// [for CLIENT] Application::Heartbeat���� ȣ��
VOID		ISecureWorkNode::Update( DWORD dwDeltaTick )
{
	//__DEBUGGING_OUTPUT0( __FUNCTION__ );

#if SERVER_CODE

#elif CLIENT_CODE

#endif //

}



// [for CLIENT] BattleScene�� ��� ��� ȣ�� - Init �迭���� �ѹ�
VOID		ISecureWorkNode::OnChange::Scene( eZONETYPE ztZONE )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );

#if SERVER_CODE

#elif CLIENT_CODE

#endif //

}



