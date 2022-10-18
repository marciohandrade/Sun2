#include "stdafx.h"
#include "VMOp.Units.h"
#include "VMOpCPU.h"


//����������������������������������������������������������������������������
//
IVMOpScheduler::IVMOpScheduler()
{
}

IVMOpScheduler::~IVMOpScheduler()
{
}

// <PATTERN>
//	�Ϲ����� ������ ������ ����.
//		- ���Ǵ� �������ʹ� �̸� PUSH �صΰ� ����� ������ ������Ų��.

#pragma pack(push, 1)
// ���� ������ ���, ������ ��ƾ,	1�ʸ��� �۾��ϴ� ��ƾ
struct sMETHOD_INTERVAL_CHECKER_WORK_SUBROUTINE
{
};

struct sMETHOD_INTERVAL_RESCHECKER_WORK_SUBROUTINE
{
};
#pragma pack(pop)

typedef sMETHOD_INTERVAL_WORKNODE_CODES<
			IBF_HASH::IBF_ITimeout_IsExpired,
			IBF_HASH::IBF_ITimeout_SetNTCTB,
			sIL_NOP,
			OFFSETOF(IVMOpScheduler, m_CheckInterval),
			IVMOpScheduler::INTERVAL_CHECKER >			/*TYPE*/ sMETHOD_INTERVAL_CHECKER_WORK;

typedef sMETHOD_INTERVAL_WORKNODE_CODES<
			IBF_HASH::IBF_ITimeout_IsExpired,
			IBF_HASH::IBF_ITimeout_SetNTCTB,
			sIL_NOP,
			OFFSETOF(IVMOpScheduler, m_SendIntervalBySendMsg),
			IVMOpScheduler::INTERVAL_SEND_DELAY >		/*TYPE*/ sMETHOD_INTERVAL_RESCHECKER_WORK;

BOOL	IVMOpScheduler::DoInit()
{
	m_bEnabled = FALSE;
	m_CheckInterval.SetNextTimeoutByCurTickBased( INTERVAL_CHECKER );
	m_SendIntervalBySendMsg.SetNextTimeoutByCurTickBased( INTERVAL_SEND_DELAY );

	do {
		m_EIPCheckInterval = MolaTAllocNew(WzASM_EIP)();
		sMETHOD_INTERVAL_CHECKER_WORK* pSTREAM = m_EIPCheckInterval->InitInstance<sMETHOD_INTERVAL_CHECKER_WORK>();
		pSTREAM->ChangeBaseObject( this );
		m_EIPCheckInterval->Prefetch();

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_VMLOG_T__)
		printf( "%04X\t%04X\n", IWzASM::MARK01, DECL_WzASM_HASH(OPT::MARK01, OPT::NOP) );
#endif
	} while(0);

	do {
		m_EIPSendIntervalBySendMsg = MolaTAllocNew(WzASM_EIP)();
		sMETHOD_INTERVAL_RESCHECKER_WORK* pSTREAM = m_EIPSendIntervalBySendMsg->InitInstance<sMETHOD_INTERVAL_RESCHECKER_WORK>();
		pSTREAM->ChangeBaseObject(this);
		m_EIPSendIntervalBySendMsg->Prefetch();
	} while(0);

	m_pCONTROL_INFO = NULL;

	m_dwHBLastedUpdateTick = GetTickCount();
	m_dwHBIncreaseCount = 0;
	m_dwHBRandomKey = MOLA_RCG_T1_016;	// <- �̰��� Ŭ���̾�Ʈ ���� ���� Ʋ�������� ��������.

	return TRUE;
}

BOOL	IVMOpScheduler::DoProcess()
{
	if( !m_bEnabled )
		return TRUE;

	uREG_NODE uNODE;
	uNODE = m_EIPCheckInterval->Execute();

	if( uNODE.EX )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_VMLOG_T__)
		printf( "m_EIPCheckInterval %u %u\n", m_CheckInterval.TimeoutTick(),  GetTickCount() );
#endif

		//struct {
		//	DWORD		m_dwHBLastedUpdateTick;		// ���� �ֱٿ� ���ŵ� ƽ
		//	DWORD		m_dwHBIncreaseCount;		// ���ŵ� ȸ��
		//	DWORD		m_dwHBRandomKey;			// ���ŵ� ����Ű
		//};

		m_dwHBLastedUpdateTick = GetTickCount();
		++m_dwHBIncreaseCount;

		__DEBUGGING_OUTPUT1( "HBRandomKey", m_dwHBRandomKey );
		m_dwHBRandomKey = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_017, m_dwHBIncreaseCount, MOLA_RCG_T1_018, m_dwHBRandomKey );
		__DEBUGGING_OUTPUT1( "HBRandomKey", m_dwHBRandomKey );
	}

#if CODE_BACKUP
	uNODE = m_EIPSendIntervalBySendMsg->Execute();

	if( uNODE.EX )
	{
		printf( "m_EIPSendIntervalBySendMsg %u\n", GetTickCount() );
	}
#endif

	m_bEnabled = FALSE;

	return TRUE;
}

BOOL	IVMOpScheduler::IsComplete()
{
	m_EIPCheckInterval->ClearOP();
	m_EIPSendIntervalBySendMsg->ClearOP();
	return FALSE;
}

BOOL	IVMOpScheduler::DoRelease()
{
	MolaTAllocDelete(WzASM_EIP, m_EIPCheckInterval);
	MolaTAllocDelete(WzASM_EIP, m_EIPSendIntervalBySendMsg);
	return TRUE;
}

// Control Interface
VOID	IVMOpScheduler::OnMsg( eMOLASIG_MSG* pSIG_MSG )
{
	if( SIGMSG::NOTIFY_HEARTBEAT == pSIG_MSG->_TYPE )
	{
		m_bEnabled = TRUE;
	}
}

