#include "stdafx.h"
#include "VMOp.Units.h"
#include "VMOpCPU.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
IVMOpPPU::IVMOpPPU()
{
}

IVMOpPPU::~IVMOpPPU()
{
}

BOOL	IVMOpPPU::DoInit()
{
	return TRUE;
}

BOOL	IVMOpPPU::DoProcess()
{
	if( !m_bEnabled )
		return TRUE;

	m_bEnabled = FALSE;

	return TRUE;
}

BOOL	IVMOpPPU::IsComplete()
{
	return FALSE;
}

BOOL	IVMOpPPU::DoRelease()
{
	return TRUE;
}

// Control Interface
VOID	IVMOpPPU::OnMsg( eMOLASIG_MSG* pSIG_MSG )
{
	if( SIGMSG::NOTIFY_HEARTBEAT == pSIG_MSG->_TYPE )
	{
		m_bEnabled = TRUE;
	}
}

