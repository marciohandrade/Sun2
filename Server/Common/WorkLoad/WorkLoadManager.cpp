#include "stdafx.h"

#include "WorkLoadManager.h"

static WorkLoadManager*		s_pWorkLoadManager = NULL;
static WORK_LOAD_MANAGER	s_WorkLoadManagerDelegator = NULL;

WorkLoadManager::WorkLoadManager()
{
	s_pWorkLoadManager = this;
	s_WorkLoadManagerDelegator = s_pWorkLoadManager;
	m_dwAutoIncWorkKey = c_DefaultKey;
	m_WorkQueue.clear();
	m_WorkList.clear();
}

WorkLoadManager::~WorkLoadManager()
{
	ReleaseTaskAll();
}

WORK_LOAD_MANAGER&	WorkLoadManager::CreateInstance()
{
	if( s_WorkLoadManagerDelegator != NULL )
		return s_WorkLoadManagerDelegator;

	s_WorkLoadManagerDelegator = new WorkLoadManager();
	return s_WorkLoadManagerDelegator;
}

VOID		WorkLoadManager::DestroyInstance()
{
	SAFE_DELETE( s_pWorkLoadManager );
}

//
WORK_KEY	WorkLoadManager::RegisterTask( IWorkNode* pIWorkNode, BOOL bDefinedWork, WORK_KEY registeredKey )
{
	if( !s_pWorkLoadManager )
		return c_NotExistKey;

	ASSERT( pIWorkNode );
	if( !pIWorkNode )
		return c_NotExistKey;

	WorkLoadManager& rManager = *s_pWorkLoadManager;

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
		workKey = rManager.m_dwAutoIncWorkKey;
		rManager.m_dwAutoIncWorkKey = (rManager.m_dwAutoIncWorkKey+1)%c_MaxWorkKey;
	}

	delegatorNode->DoInit();

	rManager.m_WorkQueue.push_back( workKey );
	rManager.m_WorkList.insert( rManager.m_WorkList.end(), WORK_NODE_PAIR( workKey, WORK_NODE_UPDATE_PAIR(delegatorNode, FALSE) ) );

	return workKey;
}

DWORD		WorkLoadManager::PendingTask( WORK_KEY workKey, BOOL bPending )
{
	if( !s_pWorkLoadManager )
		return 0;

	WorkLoadManager& rManager = *s_pWorkLoadManager;

	WORK_NODE_MAP_IT it = rManager.m_WorkList.find( workKey );
	if( it == rManager.m_WorkList.end() )
		return 0;

	WORK_NODE_UPDATE_PAIR& rUpdatePair = it->second;
	WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
    __TOUCHED((rWorkNode));
	rUpdatePair.second = bPending;

	return 0;
}

VOID		WorkLoadManager::ReleaseTask( WORK_KEY key, BOOL bForced )
{
    __UNUSED(bForced);
	if( !s_pWorkLoadManager )
		return;

	WorkLoadManager& rManager = *s_pWorkLoadManager;

	WORK_NODE_MAP_IT it = rManager.m_WorkList.find( key );
	if( it == rManager.m_WorkList.end() )
		return;

	WORK_NODE_PAIR rPair = *it;

	rManager._NodeRelease( rPair );
	rManager.m_WorkList.erase( it );
}

VOID		WorkLoadManager::ReleaseTaskAll()
{
	if( !s_pWorkLoadManager )
		return;

	WorkLoadManager& rManager = *s_pWorkLoadManager;

	for_each(
		rManager.m_WorkList.begin(),
		rManager.m_WorkList.end(),
		bind1st(mem_fun(&WorkLoadManager::_NodeRelease), &rManager)
		);

	rManager.m_WorkQueue.clear();
	rManager.m_WorkList.clear();
}


VOID		WorkLoadManager::_NodeUpdate( WORK_UPDATE_QUEUE_IT itKey )
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
		SAFE_DELETE( rWorkNode.REFERENCE() );
	}
	m_WorkQueue.erase( itKey );
	m_WorkList.erase( it );
}

//VOID		WorkLoadManager::_NodeRelease( WORK_NODE_MAP_IT it )
VOID		WorkLoadManager::_NodeRelease( WORK_NODE_PAIR node )
{
	WORK_NODE_UPDATE_PAIR& rUpdatePair = node.second;
	WORK_NODE_DELEGATOR& rWorkNode = rUpdatePair.first;
	if( rWorkNode == NULL )
		return;

	rWorkNode->DoRelease();
	SAFE_DELETE( rWorkNode.REFERENCE() );
}

VOID		WorkLoadManager::OnMsg( sSIG_MSG* pMSG, WORK_KEY key )
{
	 if( !(s_pWorkLoadManager && pMSG) )
		 return;

	 WorkLoadManager* pManager = s_pWorkLoadManager;

	 WORK_NODE_MAP& rMAP = pManager->m_WorkList;

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

VOID		WorkLoadManager::Update()
{
	if( !s_pWorkLoadManager )
		return;

	WorkLoadManager* pManager = s_pWorkLoadManager;

	WORK_UPDATE_QUEUE_IT it(pManager->m_WorkQueue.begin());

	for( ; it != pManager->m_WorkQueue.end() ; )
	{
		pManager->_NodeUpdate( it++ );
	}
}


