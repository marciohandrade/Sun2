#include "stdafx.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <FUNCTION PROTOTYPEs>
static BOOL		CreateISecureWorkNode();

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <MODULE VARIABLEs>
static DWORD	s_dwHeartbeatSize = sizeof(MSG_CG_CONNECTION_HEARTBEAT_SYN);

static sSECURE_WORKNODE_STRUCTURE	s_ISecureWorkNodeStream[1] = { 0, };

static BOOL		s_bISecureInit = CreateISecureWorkNode();

static DWORD	s_SecureLogicBase = NULL;
static DWORD	s_SecureLogics[SECURE_PATTERNS_MAX] = { NULL, };


static sSECURE_WORKNODE_CMDSTREAM	s_ISecureCmdStream[1];




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//
//


static BOOL		CreateISecureWorkNode()
{
	EXPORTER(CreateISecureWorkNode);
	new (s_ISecureWorkNodeStream) ISecureWorkNode();

	return TRUE;
}

ISecureWorkNode*		CryptManager::GetISecureWorkNode()
{
	EXPORTER(CryptManager::GetISecureWorkNode);
	return (ISecureWorkNode*)s_ISecureWorkNodeStream;
}

DWORD			CryptManager::GetHeartbeatSize()
{
	return s_dwHeartbeatSize;
}

ISecureQueryCmdStreamBase*	ISecureQueryCmdStreamBase::GetStaticInstance()
{
	EXPORTER(ISecureQueryCmdStreamBase::GetStaticInstance);
	return (ISecureQueryCmdStreamBase*)( new (s_ISecureCmdStream) ISecureQueryCmdStreamInit() );
}









//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽







//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽



BOOL	CryptManager::InitSecureInfoParser()
{
	//MOLA_SECURE_WORKNODE_WALL2
	__DEBUGGING_OUTPUT0( "ISecureWorkNode::InitSecureInfoParser" );

	BYTE stk[256];

	ISecureWorkNode* pSecureNode;

	pSecureNode = new (stk) ISecureWorkNode;
	s_SecureLogicBase = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureSimplexSNK01;
	s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN01_SNK] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureSimplexSNK02;
	s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN02_SNK] = OBJCONVERT(pSecureNode);

	__DEBUGGING_OUTPUT1( "s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN02_SNK]", s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN02_SNK] );
	__DEBUGGING_OUTPUT1( "pSecureNode", *(DWORD*)stk );

	pSecureNode = new (stk) ISecureSimplexSNK03;
	s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN03_SNK] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureSimplexSNK04;
	s_SecureLogics[SECURE_QUERY_SIMPLEX_PATTERN04_SNK] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureQueryMola;
	s_SecureLogics[SECURE_QUERY_MOLA] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureConfigureMola;
	s_SecureLogics[SECURE_QUERY_CONFIG_MOLA] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureConfigureHeartbeat;
	s_SecureLogics[SECURE_QUERY_CONFIG_MOLA_HB] = OBJCONVERT(pSecureNode);

	pSecureNode = new (stk) ISecureDuplexSNK;
	s_SecureLogics[SECURE_QUERY_DUPLEX_SNK] = OBJCONVERT(pSecureNode);

	memset( stk, 0, sizeof(stk) );

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
		printf( "InitSecureInfo %08X %08X\n", MOLA_SECURE_WORKNODE_WALL1, MOLA_SECURE_WORKNODE_WALL2 );
#endif

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef _USRDLL

namespace util
{

	VOID	AssertionLogger4Pool_Default( DWORD errTYPE, ePOOL_ERROR* IN pINFO )
	{
		TCHAR* pErrTypeMsg = ePOOL_ERROR::INVALID_REFERENCE == errTYPE ? _T("Freeing Address is Can't Accessable")
			: ePOOL_ERROR::CANT_ALLOCATABLE == errTYPE ? _T("Can't Allocatable")
			: ePOOL_ERROR::CANT_DEALLOCATABLE == errTYPE ? _T("Can't DeAllocatable, STAT(Already Freed | Invalid Reference | Overrun)")
			: ePOOL_ERROR::NOT_MATCHED_ALLOCnDEALLOC == errTYPE ? _T("Free Count is not matched AllocCount")
			: NULL;

		//MessageOut( 3, _T("[POOL ASSERTION][TYPE:%s] \"%s\""), pINFO->m_pszTYPENAME, pErrTypeMsg );
#	ifdef _DEBUG
		printf( _T("[POOL ASSERTION][TYPE:%s] \"%s\"\n"), pINFO->m_pszTYPENAME, pErrTypeMsg );
#	endif

#	ifdef _DEBUG
		__asm { int 3 }
#	endif //
	}

} //

#endif //_USRDLL

