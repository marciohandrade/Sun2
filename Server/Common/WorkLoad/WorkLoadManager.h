#pragma once

#include "WorkLoad.Common.h"

class WorkLoadManager
{
private:
	WorkLoadManager();
	~WorkLoadManager();
public:
	static const WORK_KEY		c_MaxWorkKey	= 0x7FFFFFFF;
	static const WORK_KEY		c_NotExistKey	= c_MaxWorkKey+1;
	static const WORK_KEY		c_DefaultKey	= 0xFFFF;

	static WORK_LOAD_MANAGER&	CreateInstance();
	static VOID					DestroyInstance();
	//
	static WORK_KEY				RegisterTask( IWorkNode* pIWorkNode, BOOL bDefinedWork = FALSE, WORK_KEY registeredKey = 0 );
	static DWORD				PendingTask( WORK_KEY workKey, BOOL bPending = TRUE /*reserved*/ );

	static VOID					ReleaseTask( DWORD key, BOOL bForced );
	static VOID					ReleaseTaskAll();

	static VOID					OnMsg( sSIG_MSG* pMSG, WORK_KEY key = c_MaxWorkKey );

	static VOID					Update();

private:
	VOID						_NodeUpdate( WORK_UPDATE_QUEUE_IT it );
	//VOID						_NodeRelease( WORK_NODE_MAP_IT it );
	VOID		WorkLoadManager::_NodeRelease( WORK_NODE_PAIR node );

private:
	DWORD						m_dwAutoIncWorkKey;
	WORK_UPDATE_QUEUE			m_WorkQueue;
	WORK_NODE_MAP				m_WorkList;
};

