#include "stdafx.h"
#include "VMOp.Units.h"
#include "VMOpCPU.h"

#pragma pack(push, 1)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<CHECK BOOLEAN>
//
//	<EXPRESSION>
//		--PUSH GPR--
//		MEMBER* pMEMBER = &((OBJECT*)->(_OFFSET MEMBER));
//		BOOL bCHECK = CALL_HANDLER( 0, pMEMBER, 0, 0 );
//		if( bCHECK )
//		{
//			EAX = { ~, pMEMBER, _INTERVAL, ~ }
//			CALL_HANDLER( EAX );
//			--POP GPR--
//			return TRUE;
//		}
//		//if( !bCHECK ) goto _JMPFAILCASE;
//	_JMPFAILCASE:
//		--POP GPR--
//		return FALSE;
//
//	<USED REG>
//		EAX = { ~, pMEMBER, _INTERVAL, ~ }
//		EOP = { OBJECT*, sCALC(OBJECT, MEMBER) }
//		ERP = bCHECK
//		EFL
//

template<typename WORD _OFFSET, typename DWORD _BOOL>
struct sIL_SET_BOOL
{
	sIL<IWzASM::PUSH_EAX>								M01;
	sIL_GET_OFFSET_PTR_ON_ROOT<NULL, _OFFSET>			M02;
	struct sIL_SET_B4
	{
		sIL<IWzASM::MOVP_EAX_B4>					M01;
		sDT4<_BOOL>									M02;
	}													M03_SET_VALUE;
	sIL<IWzASM::POP_EAX>								M04;

	inline VOID		ChangeBaseObject( WzPTR pPTR )
	{	M02.ChangeBaseObject( pPTR );	}

	inline VOID		ChangeSettingBOOL( BOOL bBOOL )
	{	M03_SET_VALUE.M02.Change( (DWORD)bBOOL );	}
};

template<typename WzASM_IBF_HASH_T _ACT_IBF_HASH, typename _USRCODE, typename WORD _OFFSET, typename DWORD _INTERVAL>
struct sMETHOD_CLOCK_CONTROL_CODES
{
	sIL<IWzASM::PUSH_EABCDX>												M01;	// STK(n)
	struct sINTER_make_args_arg1isOffsetPtr_arg2isInterval
	{
		sIL_MOV_EAX1_B4<_INTERVAL>										M01;
		sIL_GET_OFFSET_PTR_ON_ROOT<NULL, _OFFSET>						M02;
		sIL<IWzASM::RORB_EAX>											M03;
	}																		M02_MAKE_ARGS;
	sIL_CALL_IBF_HASH_USRINPUT<sIL<IWzASM::PUSH_EAX>, _ACT_IBF_HASH>		M03;

	// User Code Block
	_USRCODE																M06;
	// 결과처리 (TRUE)
	// EBX = TRUE;
	// ERP = EBX
	struct sINTER_make_return_true_result {
		sIL<IWzASM::MOV_EBX_B1>										M01;
		sDT1<TRUE>													M02;
		sIL<IWzASM::MOV_ERP_EBX>									M03;
	}																		M09;
	sIL_MARK01																M10;
	sIL<IWzASM::POP_EABCDX>													M11;
	sIL_RET																	M12;

	inline VOID		ChangeBaseObject( WzPTR pPTR )
	{	M02_MAKE_ARGS.M02.ChangeBaseObject( pPTR );	}

	inline VOID		Change_call_IBF_handler( WzASM_IBF_HASH_T wHASH_IBF )
	{	M03.ChangeHASH( wHASH_IBF );	}
};
#pragma pack(pop)

typedef sIL_SET_BOOL<OFFSETOF(IVMOpGenerater, m_bEnabled), TRUE>	sIL_SET_BOOL_ENABLED_FALSE_T;
typedef sMETHOD_CLOCK_CONTROL_CODES<
		IBF_HASH::IBF_ITimeout_SetNTCTB,
		sIL_NOP,
		OFFSETOF(IVMOpGenerater, m_UpdateInterval),
		IVMOpGenerater::INTERVAL_MOLA_HEARTBEAT >		/*TYPE*/ sMETHOD_CLOCK_START_CODES_T;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
IVMOpGenerater::IVMOpGenerater()
{
}

IVMOpGenerater::~IVMOpGenerater()
{
}

BOOL			IVMOpGenerater::DoInit()
{
	m_bEnabled = FALSE;
	m_UpdateInterval.SetNextTimeoutByCurTickBased( INTERVAL_MOLA_HEARTBEAT );	// 매 1초의 Heartbeat
	m_dwUpdateLoopCount = 0;
	m_dwUpdateCount = 0;				// DoProcess 호출 수

	do {
		m_SetNextStartAction = MolaTAllocNew(WzASM_EIP)();
		sMETHOD_CLOCK_START_CODES_T* pSTREAM = m_SetNextStartAction->InitInstance<sMETHOD_CLOCK_START_CODES_T>();
		pSTREAM->ChangeBaseObject(this);
		m_SetNextStartAction->Prefetch();
	} while(0);


	do {
		m_SetEnable = MolaTAllocNew(WzASM_EIP)();
		sIL_SET_BOOL_ENABLED_FALSE_T* pSTREAM = m_SetEnable->InitInstance<sIL_SET_BOOL_ENABLED_FALSE_T>();
		pSTREAM->ChangeBaseObject(this);
	} while(0);

	return TRUE;
}

BOOL			IVMOpGenerater::DoProcess()
{
	if( !m_bEnabled )
		return TRUE;

	++m_dwUpdateLoopCount;
	if( m_UpdateInterval.IsExpired() )
	{
		m_UpdateInterval.SetNextTimeoutByCurTickBased( INTERVAL_MOLA_HEARTBEAT );
		++m_dwUpdateCount;

		sSIG_NOTIFY_HEARTBEAT msgSIG( m_dwUpdateCount );
		VMOpCPU::OnMsg( &msgSIG );
	}

	return TRUE;
}

BOOL			IVMOpGenerater::IsComplete()
{
	return FALSE;
}

BOOL			IVMOpGenerater::DoRelease()
{
	return TRUE;
}

VOID			IVMOpGenerater::OnMsg( eMOLASIG_MSG* pSIG_MSG )
{
	if( SIGMSG::CONTROL_START == pSIG_MSG->_TYPE )
	{
		sIL_SET_BOOL_ENABLED_FALSE_T* pSTREAM = m_SetEnable->CmdStream<sIL_SET_BOOL_ENABLED_FALSE_T>();
		pSTREAM->ChangeSettingBOOL( TRUE );
		m_SetEnable->Execute();
		//m_UpdateInterval.SetNextTimeoutByCurTickBased( INTERVAL_MOLA_HEARTBEAT );
		//m_bEnabled = TRUE;
	}
	else if( SIGMSG::CONTROL_STOP == pSIG_MSG->_TYPE )
	{
		sIL_SET_BOOL_ENABLED_FALSE_T* pSTREAM = m_SetEnable->CmdStream<sIL_SET_BOOL_ENABLED_FALSE_T>();
		pSTREAM->ChangeSettingBOOL( FALSE );
		m_SetEnable->Execute();
		//m_bEnabled = FALSE;
	}
}



