#include "stdafx.h"

#include "VMOpCPU.h"

static VMOpCPU*	s_IVMClockManager = NULL;

VMOpCPU::VMOpCPU()
{
	s_IVMClockManager = this;
	m_dwAutoIncWorkKey = c_DefaultKey;
	m_WorkQueue.clear();
	m_WorkList.clear();
}

VMOpCPU::~VMOpCPU()
{
	ReleaseTaskAll();
}

VMOp_MANAGER	VMOpCPU::CreateInstance()
{
	if( s_IVMClockManager != NULL )
		return s_IVMClockManager;

	s_IVMClockManager = new VMOpCPU();
	return s_IVMClockManager;
}

VOID		VMOpCPU::DestroyInstance()
{
	SAFE_DELETE( s_IVMClockManager );
}

//
WORK_KEY	VMOpCPU::RegisterTask( IVMOp* pIWorkNode, BOOL bDefinedWork, WORK_KEY registeredKey )
{
	if( !( s_IVMClockManager && pIWorkNode ) )
		return c_NotExistKey;

	WORK_NODE_DELEGATOR delegatorNode = pIWorkNode;
	
	WORK_KEY workKey;

	if( bDefinedWork )
	{
		if( !(registeredKey < c_DefaultKey) )
			return c_NotExistKey;

		workKey = registeredKey;
	}
	else
	{
		workKey = s_IVMClockManager->m_dwAutoIncWorkKey;
		s_IVMClockManager->m_dwAutoIncWorkKey
			= (s_IVMClockManager->m_dwAutoIncWorkKey+1)%c_MaxWorkKey;
	}

	delegatorNode->DoInit();

	s_IVMClockManager->WORK_QUEUE().push_back( workKey );
	s_IVMClockManager->WORK_LIST().insert( s_IVMClockManager->WORK_LIST().end(), WORK_NODE_PAIR( workKey, WORK_NODE_UPDATE_PAIR(delegatorNode, FALSE) ) );

	return workKey;
}

DWORD		VMOpCPU::PendingTask( WORK_KEY workKey, BOOL bPending )
{
	WORK_NODE_MAP_IT it = s_IVMClockManager->WORK_LIST().find( workKey );
	if( it == s_IVMClockManager->WORK_LIST().end() )
		return 0;

	WORK_NODE_UPDATE_PAIR& rUpdatePair = it->second;
	WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
	rUpdatePair.second = bPending;

	return 0;
}

VOID		VMOpCPU::ReleaseTask( WORK_KEY key, BOOL bForced )
{
	WORK_NODE_MAP_IT it = s_IVMClockManager->WORK_LIST().find( key );
	if( it == s_IVMClockManager->WORK_LIST().end() )
		return;

	WORK_NODE_PAIR rPair = *it;

	s_IVMClockManager->_NodeRelease( rPair );
	s_IVMClockManager->WORK_LIST().erase( it );
}

VOID		VMOpCPU::ReleaseTaskAll()
{
	for_each(
		s_IVMClockManager->WORK_LIST().begin(),
		s_IVMClockManager->WORK_LIST().end(),
		bind1st(mem_fun(&VMOpCPU::_NodeRelease), s_IVMClockManager)
		);

	s_IVMClockManager->WORK_QUEUE().clear();
	s_IVMClockManager->WORK_LIST().clear();
}


VOID		VMOpCPU::_NodeUpdate( WORK_UPDATE_QUEUE_IT itKey )
{
	WORK_KEY workKey = *itKey;

	WORK_NODE_MAP_IT it = m_WorkList.find( workKey );
	if( it == m_WorkList.end() )
		return;

	WORK_NODE_UPDATE_PAIR& rUpdatePair = it->second;
	WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
	BOOL bPending = rUpdatePair.second;

	if( rWorkNode != NULL )
	{
		if( bPending )
			return;

		if( rWorkNode->DoProcess() && !rWorkNode->IsComplete() )
			return;

		rWorkNode->DoRelease();

		if( rWorkNode.REFERENCE() )
		{
			MolaTAllocDelete(IVMOp, rWorkNode.REFERENCE());
			rWorkNode.REFERENCE() = NULL;
		}
	}
	m_WorkQueue.erase( itKey );
	m_WorkList.erase( it );
}

//VOID		VMOpCPU::_NodeRelease( WORK_NODE_MAP_IT it )
VOID		VMOpCPU::_NodeRelease( WORK_NODE_PAIR node )
{
	WORK_NODE_UPDATE_PAIR& rUpdatePair = node.second;
	WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
	if( rWorkNode == NULL )
		return;

	rWorkNode->DoRelease();
	if( rWorkNode.REFERENCE() )
	{
		MolaTAllocDelete(IVMOp, rWorkNode.REFERENCE());
		rWorkNode.REFERENCE() = NULL;
	}

}

VOID		VMOpCPU::OnMsg( eMOLASIG_MSG* pMSG, WORK_KEY key )
{
	 WORK_NODE_MAP& rMAP = s_IVMClockManager->WORK_LIST();

	 if( c_MaxWorkKey == key )
	 {
		WORK_NODE_MAP_IT it(rMAP.begin()), end(rMAP.end());
		for( ; it != end ; ++it )
		{
			WORK_NODE_UPDATE_PAIR& rUpdatePair = it->second;
			WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
			rWorkNode->OnMsg( pMSG );
		}
	 }
	 else
	 {
		WORK_NODE_MAP_IT it = rMAP.find( key );
		if( it == rMAP.end() )
			return;

		WORK_NODE_UPDATE_PAIR& rUpdatePair = it->second;
		WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
		rWorkNode->OnMsg( pMSG );
	 }
}

VOID		VMOpCPU::Update()
{
	WORK_UPDATE_QUEUE_IT it(s_IVMClockManager->WORK_QUEUE().begin());

	for( ; it != s_IVMClockManager->WORK_QUEUE().end() ; )
	{
		s_IVMClockManager->_NodeUpdate( it++ );
	}
}


