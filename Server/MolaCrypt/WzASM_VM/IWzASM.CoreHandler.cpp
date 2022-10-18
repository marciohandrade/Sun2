#include "stdafx.h"
#include "IWzASM.h"
#include "VMOp/VMOpCPU.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <MODULE VARIABLEs>
WzASM_VMCore*			WzASM_VMCore::sp_VMCore = NULL;
WzASM_VMData*			WzASM_VMCore::sp_VMData = NULL;
#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)
ISecureQueryProcessor*	WzASM_VMCore::sp_VMSQP = NULL;
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM_VMCore>
//
WzASM_VMCore::WzASM_VMCore() : m_GResHASH(m_GResHandler)
{
	sp_VMCore = this;
	sp_VMData = new WzASM_VMData;

#if SERVER_CODE
	sp_VMSQP = MolaTAllocNew(ISecureQueryProcessor)();
#endif

	ZeroMemory( m_JATARRAY, sizeof(m_JATARRAY) );
}

WzASM_VMCore::~WzASM_VMCore()
{
	Release();
}

WzRET_T	WzASM_VMCore::Find_WzASM( OPW_T cWzASM, sWzASM_BLK& OUT rINFO )
{
	IWzASM_HASH_IT itFIND = CODE_TABLE().find(cWzASM);
	if( itFIND == CODE_TABLE().end() )
		return sRET::FAIL;

	sWzASM_BLK rINFO_SRC = itFIND->second;
	rINFO = rINFO_SRC;

	return sRET::SUCCESS;
}

BOOL	WzASM_VMCore::CreateEIP( WzASM_EIP*& OUT rpINSTANCE, BYTE* pCmdStream, WORD wSize )
{
	// 일단 임시로 new로 생성 다른 방안을 생각해보자.
	// 그냥 Pool을 사용하는 것은 아무래도 분석되기 쉽다.
	if( !pCmdStream )
		return FALSE;

	WzASM_EIP* pEIP = MolaTAllocNew(WzASM_EIP)();	// new WzASM_EIP();
	if( !pEIP->Init( pCmdStream, wSize ) )
	{
		MolaTAllocDelete(WzASM_EIP, pEIP);
		return FALSE;
	}

	//__asm { int 3 };
	//pEIP->Register_REF( GRES_TABLE() );
	rpINSTANCE = pEIP;

	pEIP->ClearOP();

	return TRUE;
}

VOID	WzASM_VMCore::DestroyEIP( WzASM_EIP*& IN OUT rpINSTANCE )
{
	__debugbreak();
	SAFE_DELETE(rpINSTANCE);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// (WARNING)
//	아래와 같이 선언하더라도 메모리 스트림에 정상적으로 남지 않는다
//	그 아래 선언된 형태로 남겨야지 별도 오퍼레이션이 붙지 않고, 그대로 메모리 스트림에 남는다.
//	그렇담... 인젝션은 도대체 어캐할 수 있단 말인가???

// <PROTOTYPEs><INTERNAL IBF HANDLERs>
static uREG_PV		RegisterASMHandler_BASE( uREG_NODE* pNODE );
static uREG_PV		RegisterASMHandler( uREG_NODE* pNODE );
uREG_PV		RegisterJAT( uREG_NODE* pNODE );

#	include "IWzASM.EncryptedData.h"
//static sMETHOD_REGISTER_HANDLER	sENCODED_DATA_sMETHOD_REGISTER_HANDLER;
//static BYTE sENCODED_DATA_sMETHOD_REGISTER_HANDLER_PTR[sizeof(sMETHOD_REGISTER_HANDLER)] =
//{
//	'W', 0xF1, 'A', 0x74, 'm', '4',
//};



VOID	WzASM_VMCore::Load( WzGResTable* pREF_Handler )
{
	m_GResHandler = IWzGResTable(pREF_Handler);
	sREGISTER_BLK& rREGISTER = VM_REGISTER;

	// <WORK TO DO>
	//		pSTREAM 에 대한 Convert 수행
	// 
	//pHANDLER_CMD_STREAM->RegisterHandlers();

#if CODE_BACKUP

	sMETHOD_REGISTER_HANDLER* pHANDLER_CMD_STREAM = MolaTAllocNew(sMETHOD_REGISTER_HANDLER)();//  new sMETHOD_REGISTER_HANDLER;

	WzASM_EIP rEIP;
	rEIP.Init( (BYTE*)pHANDLER_CMD_STREAM, sizeof(*pHANDLER_CMD_STREAM) );
	rEIP.ClearOP();

	ZeroMemory( pHANDLER_CMD_STREAM, sizeof(*pHANDLER_CMD_STREAM) );
	MolaTAllocFree(pHANDLER_CMD_STREAM);
	//SAFE_DELETE( pHANDLER_CMD_STREAM );

#elif CODE_RENEWAL
	WzASM_EIP rEIP;
	sMETHOD_REGISTER_HANDLER* pHANDLER_CMD_STREAM = rEIP.InitInstance<sMETHOD_REGISTER_HANDLER>();
#endif

	uREG_NODE pNODE[sARG::MAXARG] = { NULL, };
    ////CALL_IWzASM_HANDLER(PUSH_EABCDX,		0);
	//rREGISTER.EBX.PTR = (WzPTR)RegisterASMHandler;
	//rREGISTER.EAX.EXA[sARG::ARG0].EX = rREGISTER.EBX.EXA[sARG::ARG0].EX;
	pNODE->EXA[sARG::EIP].PTR = (WzPTR)&rEIP;
	RegisterASMHandler_BASE( pNODE );


#if CODE_TEST(__MOLA_RESOURCE_TEST__)
	MODULE_HANDLER_CALL(UNIT_TEST, NULL);
#endif 

#if SERVER_CODE
	MODULE_HANDLER_CALL(REGISTER_QUERY_MOLA_CMD, NULL);
#endif //
}




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
VOID	WzASM_VMCore::Release()
{
	VMOpCPU::DestroyInstance();

	__DEBUGGING_VM_OUTPUT1( "Stack Leak", VM_STACK.m_STACK.size() );

#if SERVER_CODE
	MolaTAllocDelete(ISecureQueryProcessor, sp_VMSQP);
	sp_VMSQP = NULL;
#endif

	SAFE_DELETE(sp_VMData);
}


//
//
//
//
//


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <sOFFSET_CTRL>
BOOL			sOFFSET_CTRL::JMP( OPW_T* ptr )
{
	INT iDIFF = (INT)(__int64)ptr - (INT)(__int64)PTR;
	BOOL bMINUS = iDIFF < 0;

	DWORD dwDIFF = bMINUS ? (DWORD)-iDIFF : (DWORD)iDIFF;
	if( MAX_CMD_STREAM_SIZE > dwDIFF )		
	{
		if( bMINUS )
			DEC( (WORD)dwDIFF );
		else
			INC( (WORD)dwDIFF );
		return TRUE;
	}

	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM_STACK>
VOID			WzASM_STACK::StoreGPR( sREGISTER_BLK& IN pvBlk )
{
	const DWORD dwSIZE = sizeof(pvBlk)/sizeof(uREG_NODE);
	uREG_NODE* pOff = ((uREG_NODE*)((&pvBlk)+1))-1;
	for( DWORD dwIdx=0 ; dwIdx<dwSIZE ; ++dwIdx, --pOff )
		Push( *pOff );
}

VOID			WzASM_STACK::LoadGPR( sREGISTER_BLK& OUT pvBlk )
{
	const DWORD dwSIZE = sizeof(pvBlk)/sizeof(uREG_NODE);
	uREG_NODE* pOff = (uREG_NODE*)&pvBlk;
	for( DWORD dwIdx=0 ; dwIdx<dwSIZE ; ++dwIdx, ++pOff )
		*pOff = Pop();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM_JAT>
WORD			WzASM_JAT::FindAddIDX( OPW_T asmCode )
{
	JUMP_ADDR_IT itend = m_JumpAddr.end();
	for( JUMP_ADDR_IT it=m_JumpAddr.begin() ; it!=itend ; ++it )
	{
		sJMP_INFO& rINFO = *it;
		if( asmCode == rINFO.MARK )
			return rINFO.ADDR_IDX;
	}

	return 0;	//따라서 MARKn은 0번지에 넣어서는 안된다. 좀더 생각해 보자.
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <GResHASH>
IWzASM_PROC				GResHASH::Find( WzASMH wHASH )
{
	HASHED_HANDLER_IT itFIND = m_HASHED_HANDLER.find( wHASH );

	IWzASM_PROC uRET;

	if( itFIND != m_HASHED_HANDLER.end() )
		uRET = itFIND->second;

	return uRET;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <WzASM_EIP>
//
BOOL		WzASM_EIP::Init( BYTE* IN pCmdStream, WORD wSize )
{
	JAT.Init();
	ZeroMemory( &OP, sizeof(OP) );

	CMD.clear();
	if( MAX_CMD_STREAM_SIZE < wSize )
		return FALSE;

	CMD.assign( &(pCmdStream[0]), &(pCmdStream[wSize]) );

	ClearOP();

	return TRUE;
}

VOID		WzASM_EIP::ClearOP()
{
	ZeroMemory( &OP, sizeof(OP) );
	OP.OFFSET.PTR = &(CMD[0]);
}

WzRET_T		WzASM_EIP::ExecuteCode()
{
	IWzASM_Handler& rHANDLER = OP.HANDLER();
	WzRET_T bRET = sRET::FAIL;
	if( rHANDLER )
		bRET = rHANDLER( *this, OP.CODE_DATA );

	switch( bRET )
	{
	case sRET::SUCCESS:
		return bRET;
	case sRET::RETURN:
		return sRET::FAIL;
	case sRET::FAIL:
		__DEBUGGING_VM_OUTPUT1( "CODE EXECUTE FAILED", rHANDLER );
	case sRET::PENDING:
		break;
	}

	__DEBUGGING_VM_OUTPUT1( "CODE EXECUTE UNEXPECTED STATUS", bRET );
#ifdef _DEBUG
	__debugbreak();
#endif //

	return sRET::FAIL;
}


OPW_T*		WzASM_EIP::FindAddress( OPW_T asmCode )
{
	WORD wOFF = JAT.FindAddIDX( asmCode );
	if( wOFF )
		return (OPW_T*)&(CMD[wOFF]);

	return NULL;
}

BOOL	WzASM_EIP::Prefetch()
{
	// CONTROL VARIABLES
	WzRET_T bCONTINUE	= sRET::SUCCESS;
	ClearOP();
	JAT.Init();

	// 같아질 때까지...
	while( bCONTINUE && ( OP.OFFSET.IDX<CmdStreamSize() ) )
	{
		// ASM Code 읽기
		OPW_T* pASM = (OPW_T*)OP.OFFSET.PTR;
		OP.OFFSET.INC( sizeof(OPW_T) );

		if( !( bCONTINUE = VMCORE_INTERFACE.Find_WzASM( *pASM, OP.CODE_INFO ) ) )
		{
			__DEBUGGING_VM_OUTPUT2( "PREFETCH FAILED CODE", *pASM, OP.DATASIZE() );
			continue;
		}

		// ASM 에 첨부된 데이터 읽기
		if( OP.DATASIZE() )
		{
			OP.CODE_DATA = (uREG_NODE*)OP.OFFSET.PTR;
			OP.OFFSET.INC( OP.DATASIZE() );
		}

		// Control JAT
		switch( *pASM )
		{
		case IWzASM::MARK01:
		case IWzASM::MARK02:
		case IWzASM::MARK03:
		case IWzASM::MARK04:
		case IWzASM::MARK05:
		case IWzASM::MARK06:
		case IWzASM::MARK07:
		case IWzASM::MARK08:
		case IWzASM::MARK09:
		case IWzASM::MARK10:
		case IWzASM::MARK11:
		case IWzASM::MARK12:
		case IWzASM::MARK13:
		case IWzASM::MARK14:
		case IWzASM::MARK15:
		case IWzASM::MARK16:
		case IWzASM::MARK17:
		case IWzASM::MARK18:
		case IWzASM::MARK19:
			__DEBUGGING_VM_OUTPUT1( "ADDMARK asmcode", *pASM );
			JAT.Add( *pASM, OP.OFFSET.IDX /*pASM*/ );
			break;
		};
	}

	ClearOP();

	return sRET::SUCCESS;
}

uREG_NODE	WzASM_EIP::Execute()
{
#ifdef _DEBUG
	DWORD dwSTKLVL = (DWORD)VM_STACK.m_STACK.size();
#endif
	__DEBUGGING_VM_OUTPUT1( "EXEC - START : STK_LVL", dwSTKLVL );

	uREG_NODE uNODE = _Execute();

	__DEBUGGING_VM_OUTPUT2( "EXEC - END : STK_LVL", VM_STACK.m_STACK.size(), dwSTKLVL );

	return uNODE;
}

uREG_NODE	WzASM_EIP::_Execute()
{
	// CONTROL VARIABLES
	WzRET_T bCONTINUE	= sRET::SUCCESS;

	// 같아질 때까지...
	while( bCONTINUE && ( OP.OFFSET.IDX<CmdStreamSize() ) )
	{
		// ASM Code 읽기
		OPW_T* pASM = (OPW_T*)OP.OFFSET.PTR;
		OP.OFFSET.INC( sizeof(OPW_T) );

		if( !( bCONTINUE = VMCORE_INTERFACE.Find_WzASM( *pASM, OP.CODE_INFO ) ) )
		{
			__DEBUGGING_VM_OUTPUT2( "EXECUTE FAILED CODE", *pASM, OP.DATASIZE() );
			continue;
		}

		// ASM 에 첨부된 데이터 읽기
		if( OP.DATASIZE() )
		{
			OP.CODE_DATA = (uREG_NODE*)OP.OFFSET.PTR;
			OP.OFFSET.INC( OP.DATASIZE() );
		}

		bCONTINUE = ExecuteCode();
	}

	return VM_REGISTER.ERP;
}












#if CODE_BACKUP

#undef ADD_IWzASM_HANDLER

sWzASM_BLK arrays[] =
{
#undef ADD_IWzASM_HANDLER
#define ADD_IWzASM_HANDLER( codeASM, dataSIZE )	\
	{	IWzASM::codeASM,	dataSIZE,	HANDLER_WzASM_##codeASM	},
#		include "IWzASM.DefList.h"
#undef ADD_IWzASM_HANDLER
};

VOID	WzASM_VMCore::Load()
{
	sp_VMCore = this;
	sp_VMData = new WzASM_VMData;

	CODE_TABLE().clear();
//	sWzASM_BLK arrays[] =
//	{
//#undef ADD_IWzASM_HANDLER
//#define ADD_IWzASM_HANDLER( codeASM, dataSIZE )	\
//		{	IWzASM::codeASM,	dataSIZE,	HANDLER_WzASM_##codeASM	},
//#		include "IWzASM.DefList.h"
//#undef ADD_IWzASM_HANDLER
//	};

	sWzASM_BLK* pOffset = arrays;
	while( IWzASM::NOP != (pOffset->OPCODE) )
	{
#if CODE_TEST(__MOLA_RESOURCE_TEST__)
		IWzASM_HASH_IT itFIND = CODE_TABLE().find( pOffset->OPCODE );
		if( itFIND != CODE_TABLE().end() )
		{
			sWzASM_BLK& rDATA_EXIST = itFIND->second;
			printf( "----------ALREADY REGISTERED (%04X)----------\n", pOffset->OPCODE );
			DEBUGGING_INTERRUPT_INMOLA();
		}
#endif
		sWzASM_BLK& rDATA = CODE_TABLE()[pOffset->OPCODE];
		rDATA = *pOffset;

#if CODE_TEST(FALSE)
		if( IWzASM::MOV_EAX_B1 == pOffset->OPCODE )
		{
			printf( "Break\n" );
			printf( "%u %u %u\n", rDATA->OPCODE, rDATA->DATASIZE, rDATA->HANDLER );
		}
#endif

		++pOffset;
	}

	do
	{
		sWzASM_BLK& rDATA = CODE_TABLE()[IWzASM::NOP];
		rDATA = *pOffset;
	} while(0);

#if CODE_TEST(FALSE)
	sWzASM_BLK rINFO;
	Find_WzASM( IWzASM::MOV_EAX_B1, rINFO );
	printf( "%u %u %u\n", rINFO.OPCODE, rINFO.DATASIZE, rINFO.HANDLER );
#endif

	VMOpCPU::CreateInstance();
}

#endif //CODE_BACKUP

//////////////////////////////////////////////////////////////////////////

static uREG_PV		RegisterASMHandler_4Register( uREG_NODE* pNODE )
{
	uREG_PV bRET = { NULL, };
	// [0] : WzASM_EIP*
	WzASM_EIP& rEIP = *(WzASM_EIP*)pNODE->EXA[sARG::EIP].PTR;

#if CODE_SAMPLE
	sMETHOD_REGISTER_HANDLER* pCMD = CONTAINING_RECORD( &rEIP.CMD[0], sMETHOD_REGISTER_HANDLER, M02 );
#endif

	sMETHOD_REGISTER_HANDLER* pCMD = rEIP.CmdStream<sMETHOD_REGISTER_HANDLER>();


	pCMD->RegisterHandlers();
	bRET.PTR = (WzPTR)RegisterASMHandler;

	return bRET;
}

static uREG_PV		RegisterASMHandler_BASE( uREG_NODE* pNODE )
{
	uREG_PV bRET = { NULL, };

	VMOpCPU::CreateInstance();

	// [0] : WzASM_EIP*
	WzASM_EIP& rEIP = *(WzASM_EIP*)pNODE->EXA[sARG::EIP].PTR;


	// CONTROL VARIABLES
	DWORD dwASMCNT = 0;
	BOOL bCONTINUE	= TRUE;

	sREGISTER_BLK& rREGISTER = VM_REGISTER;

#if CODE_SAMPLE
	sMETHOD_REGISTER_HANDLER* pCMD = CONTAINING_RECORD( &rEIP.CMD[0], sMETHOD_REGISTER_HANDLER, M02 );
#endif

	sMETHOD_REGISTER_HANDLER* pCMD = rEIP.CmdStream<sMETHOD_REGISTER_HANDLER>();

	//sMETHOD_REGISTER_HANDLER* pCMD = (sMETHOD_REGISTER_HANDLER*)&rEIP.CMD[0];
	pCMD->RegisterHandler4Handlers( &rEIP );
	rREGISTER.EFP.PTR = (WzPTR)RegisterASMHandler_4Register;

	// 같아질 때까지...
	while( bCONTINUE && ( rEIP.OP.OFFSET.IDX<rEIP.CmdStreamSize() ) )
	{
		// ASM Code 읽기
		OPW_T* pASM = (OPW_T*)rEIP.OP.OFFSET.PTR;
		rEIP.OP.OFFSET.INC( sizeof(OPW_T) );

		//sWzASM_BLK rCODE_INFO;
		switch( *pASM )
		{
#define CURPTR2NODE()	(*(uREG_NODE*)rEIP.OP.OFFSET.PTR)

		case CONV_HASH_CODEVALUE(IWzASM::MOV_EAX0_PTR):
			rREGISTER.EAX.EXA[sARG::EIP].PTR = CURPTR2NODE().PTR;
			rEIP.OP.OFFSET.INC( sizeof(WzPTR) );
			break;
		case CONV_HASH_CODEVALUE(IWzASM::MOV_EAX0_ERP0):
			rREGISTER.EAX.EXA[sARG::ARG0] = rREGISTER.ERP.EXA[sARG::ARG0];
			break;
		case CONV_HASH_CODEVALUE(IWzASM::MOV_EAX1_B2):
			rREGISTER.EAX.EXA[sARG::ARG1].EX =  CURPTR2NODE().X;
			rEIP.OP.OFFSET.INC( 2 );
			break;
		case CONV_HASH_CODEVALUE(IWzASM::MOV_EAX2_B1):
			rREGISTER.EAX.EXA[sARG::ARG2].EX =  CURPTR2NODE().L;
			rEIP.OP.OFFSET.INC( 1 );
			break;
		case CONV_HASH_CODEVALUE(IWzASM::MOV_EAX3_PTR):
			rREGISTER.EAX.EXA[sARG::ARG3].PTR =  CURPTR2NODE().PTR;
			rEIP.OP.OFFSET.INC( sizeof(WzPTR) );
			break;
		case CONV_HASH_CODEVALUE(IWzASM::PUSH_EAX):
			VM_STACK.Push( rREGISTER.EAX );
			break;
		case CONV_HASH_CODEVALUE(IWzASM::MOV_EFP_EAX):
			rREGISTER.EFP = rREGISTER.EAX;
			break;
		case CONV_HASH_CODEVALUE(IWzASM::CALL_IBF_EFP):
			{
				uREG_NODE uPOPNODE = VM_STACK.Pop();
				rREGISTER.ERP.CONV = rREGISTER.EFP.FN_IB( &uPOPNODE );
				if( !rREGISTER.ERP.EX )
					goto CLOSURE_MARK;
			}
			break;
		case CONV_HASH_CODEVALUE(IWzASM::NOP):
		default:
			continue;
		};
	}

CLOSURE_MARK:

	uREG_NODE uNODE = VM_REGISTER.ERP;

	if( uNODE.EX )
	{
		CALL_IBF_HANDLER( StartIBFHandler, pNODE );
		CALL_IBF_HANDLER( StartGResHandler, pNODE );
	}

	return uNODE.CONV;
};


static uREG_PV		RegisterASMHandler( uREG_NODE* pNODE )
{
	uREG_PV bRET = { NULL, };
	// [0] : WzASM_EIP*
	// [1] : ASM CODE
	// [2] : DATA SIZE
	// [3] : FN HANDLER
	sWzASM_BLK sBLOCK;
	sBLOCK.OPCODE = pNODE->EXA[sARG::ARG1].X;
	sBLOCK.DATASIZE = pNODE->EXA[sARG::ARG2].L;
	sBLOCK.HANDLER = (IWzASM_Handler)pNODE->EXA[sARG::ARG3].PTR;

	WzASM_VMCore& VMCORE = VMCORE_INTERFACE;

#if CODE_TEST(__MOLA_RESOURCE_TEST__)
	WzASM_VMCore::IWzASM_HASH_IT itFIND = VMCORE.CODE_TABLE().find( sBLOCK.OPCODE );
	if( itFIND != VMCORE.CODE_TABLE().end() )
	{
		sWzASM_BLK& rDATA_EXIST = itFIND->second;
		printf( "----------ALREADY REGISTERED (%04X)----------\n", sBLOCK.OPCODE );
		DEBUGGING_INTERRUPT_INMOLA();
	}
#endif

	sWzASM_BLK& rDATA = VMCORE.CODE_TABLE()[sBLOCK.OPCODE];
	rDATA = sBLOCK;
	bRET.VAL = TRUE;

	return bRET;
};

uREG_PV		RegisterJAT( uREG_NODE* pNODE )
{
	uREG_PV bRET = { NULL, };
	// [0] : WzASM_EIP*
	// [1] : OPW_T : WORD
	// [2] : OPB_T : BYTE
	// [3] : ~~
	WzASM_VMCore& VMCORE = VMCORE_INTERFACE;

	WzASM_VMCore::sJAT_MATCH* pMATCH = VMCORE.JAT_MATCH_ARRAY();
	WORD* rpCOUNT = (WORD*)pMATCH;
	++(*rpCOUNT);
	pMATCH += (*rpCOUNT);

#if CODE_BACKUP
	WzASM_VMCore::sJAT_MATCH matchTB;
	matchTB.JMPMRK = pNODE->EXA[sARG::ARG2].L;
	matchTB.ASMOP = pNODE->EXA[sARG::ARG1].X; 
#endif

	pMATCH->JMPMRK = pNODE->EXA[sARG::ARG2].L;
	pMATCH->ASMOP = pNODE->EXA[sARG::ARG1].X; 
	

#if CODE_TEST(__MOLA_RESOURCE_TEST__)
	WzASM_VMCore::JAT_MATCH_IT itFIND = VMCORE.JAT_MATCH_TABLE().find( pMATCH->JMPMRK );
	if( itFIND != VMCORE.JAT_MATCH_TABLE().end() )
	{
		WzASM_VMCore::sJAT_MATCH*& rDATA_EXIST = itFIND->second;
		printf( "----------ALREADY REGISTERED (%04X %04X)----------\n", rDATA_EXIST->JMPMRK, rDATA_EXIST->ASMOP );
		DEBUGGING_INTERRUPT_INMOLA();
	}
#endif

	do {
		WzASM_VMCore::sJAT_MATCH*& rpDATA = VMCORE.JAT_MATCH_TABLE()[pMATCH->JMPMRK];
		rpDATA = pMATCH;
	} while(0);
	do {
		WzASM_VMCore::sJAT_MATCH*& rpDATA = VMCORE.JAT_MATCHW_TABLE()[pMATCH->ASMOP];
		rpDATA = pMATCH;
	} while(0);

	bRET.VAL = TRUE;

	return bRET;
};


