#pragma once

#include "VMOp.h"

class VMOpCPU
{
private:
	VMOpCPU();
	~VMOpCPU();
public:
	static const WORK_KEY		c_MaxWorkKey	= 0x0FFFFFFF;
	static const WORK_KEY		c_NotExistKey	= c_MaxWorkKey+1;
	static const WORK_KEY		c_DefaultKey	= 1000;

	static VMOp_MANAGER	CreateInstance();
	static VOID					DestroyInstance();
	//
	static WORK_KEY				RegisterTask( IVMOp* pIWorkNode, BOOL bDefinedWork = FALSE, WORK_KEY registeredKey = 0 );
	static DWORD				PendingTask( WORK_KEY workKey, BOOL bPending = TRUE /*reserved*/ );

	static VOID					ReleaseTask( DWORD key, BOOL bForced );
	static VOID					ReleaseTaskAll();

	static VOID					OnMsg( eMOLASIG_MSG* pMSG, WORK_KEY key = c_MaxWorkKey );

	static VOID					Update();

	// WorkNode Special
	template<typename _TYPE>
	inline static _TYPE*		CreateOPClock()
	{	return MolaTAllocNew(_TYPE)();	}

private:
	VOID						_NodeUpdate( WORK_UPDATE_QUEUE_IT it );
	//VOID						_NodeRelease( WORK_NODE_MAP_IT it );
	VOID		VMOpCPU::_NodeRelease( WORK_NODE_PAIR node );

private:
	inline WORK_UPDATE_QUEUE&	WORK_QUEUE() { return m_WorkQueue; }
	inline WORK_NODE_MAP&		WORK_LIST() { return m_WorkList; }
private:
	DWORD						m_dwAutoIncWorkKey;
	WORK_UPDATE_QUEUE			m_WorkQueue;
	WORK_NODE_MAP				m_WorkList;
};

