#include "stdafx.h"
#include "IWzASM.h"
#include "IWzASM.IBF.h"

#pragma pack(push, 1)





////{
////	DWORD dwSUM = 0;
////	for( DWORD dwIDX=0 ; dwIDX<100 ; ++dwIDX )
////		dwSUM += dwIDX;
////}
//// �� ��ƾ���� ���Խ� EDX �������� ��� �����ϴ�. ������ ��.
//template<typename _USRCODE, typename DWORD _UPRVAL, typename OPB_T _EXPRMARK, typename OPB_T _LOOPMARK>
//struct sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP1 :
//	sIL_FORLOOP_RAW<
//	sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_INIT_REG<sIL_MOV_EBX_B4<_UPRVAL>, 0>,
//	sIL_IF_THEN_ELSE_IS_BELOW<sIL_CMP_EAX_EBX, _LOOPMARK>,
//	sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP_ITERATOR,
//	_USRCODE,
//	_EXPRMARK, _LOOPMARK
//	>
//{
//};
//struct sTEST_COMMAND_STREAM_T2
//{
//	sIL_FORLOOP_ZEROtoUPPER_COUNTING_LOOP<
//		sIL_CALL_RC<
//		WzGResTable::HASHED::GET_CHAR_MOVSPD_IN_GAME,
//		/*sIL_PUSH_B1<100>,*/
//		sIL<IWzASM::PUSH_ERP>,
//		0>,
//		2,
//		OPT::MARK01, OPT::MARK02
//	>		M01;
//} s_cmdstream[1];
//
//VOID TTT()
//{
//	BYTE pbySTREAM[256];
//	BYTE* pOFF = pbySTREAM;
//
//	for( DWORD dwIdx=0 ; dwIdx<sizeof(pbySTREAM) ; ++dwIdx, ++pOFF )
//	{
//		// ��� ����...
//	}
//}
//
//struct sIL_PTR_CALCULATE
//{
//	sIL_FORLOOP_USR_DEFINE_COUNTING_LOOP<
//};
#pragma pack(pop)
//
//����������������������������������������������������������������������������


#undef ADD_HANDLER_IBF
#define ADD_HANDLER_IBF(name)	\
	uREG_PV		HANDLER_WzASM_IBF_##name( uREG_NODE* pNODE )

//����������������������������������������������������������������������������
//  <ARGUMENT FORMAT>
//	[0][1][2][3]
//


//����������������������������������������������������������������������������
//
//
uREG_PV		RegisterJAT( uREG_NODE* pNODE );
#include "IWzASM.EncryptedData.h"
//static sMETHOD_IBF_HANDLERS_REGISTER_CMD	sENCODED_DATA_sIL_WzEIP_IBF_HANDLERS_REGISTER_CMD;




//����������������������������������������������������������������������������
//	[0] :	NONE
//	[1] :	pALLOCED : BYTE*	(ALLOCnCLEAR_MEMORY �� ����� �����)
//	[2] :	SZ : WORD			(ALLOCnCLEAR_MEMORY �� ����� ��� ũ��)
ADD_HANDLER_IBF(RegisterIBFHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	if( pREGS[sARG::ARG1].PTR && pREGS[sARG::ARG2].X == sizeof(sMETHOD_IBF_HANDLERS_REGISTER_CMD) )
	{
		sMETHOD_IBF_HANDLERS_REGISTER_CMD* pCMD = new (pREGS[sARG::ARG1].PTR) sMETHOD_IBF_HANDLERS_REGISTER_CMD;

		// <WORK TO DO>
		//		Converting
		pCMD->RegisterHandlers();

#if CODE_SAMPLE
		BYTE* pCMDSTREAM;
		WORD wCMDSIZE;
		pCMD->CommanStreamInfo( pCMDSTREAM, wCMDSIZE );
		pCMD->ChangeRegisterHandler( (WzPTR)FNPTR_IBF_HANDLER(_RegisterIBFHandler) );

		sUREG_NODE sPrepareEIP_CMD( NULL, pCMDSTREAM, wCMDSIZE );
#endif

		pCMD->ChangeRegisterHandler( (WzPTR)FNPTR_IBF_HANDLER(_RegisterIBFHandler) );

		sUREG_NODE sPrepareEIP_CMD( NULL, pCMD, sizeof(*pCMD) );

		bRET = CALL_IBF_HANDLER( PrepareEIP, &sPrepareEIP_CMD.NODE );
		ZeroMemory( pCMD, sizeof(sMETHOD_IBF_HANDLERS_REGISTER_CMD) );
		MolaTAllocFree(pCMD);
		//SAFE_DELETE( pCMD );

		WzASM_EIP* pEIP = (WzASM_EIP*)bRET.PTR;
		if( pEIP )
		{
			sUREG_NODE sEIP_ExecuteCMD( pEIP );
			bRET = CALL_IBF_HANDLER( EIP_Execute, &sEIP_ExecuteCMD.NODE );
		}

		MolaTAllocDelete(WzASM_EIP, pEIP);	// SAFE_DELETE( pEIP );
	}

	return bRET;
}

//����������������������������������������������������������������������������
// <StartIBFHandler>
//		�ܺο��� �̰��� ȣ���ϸ�, IBF�ڵ鷯�� ��Ͻ�Ų��.
// <Logic Seq.>
//		1) ALLOCnCLEARMEMORY (size : sMETHOD_IBF_HANDLERS_REGISTER_CMD)
//		2) replacement new sMETHOD_IBF_HANDLERS_REGISTER_CMD
//		3) PROCESSING
//		4) StartSubHandler
//	[0]	:	NONE
ADD_HANDLER_IBF(StartIBFHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzASM_EIP sEIP;
	sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR<sizeof(sMETHOD_IBF_HANDLERS_REGISTER_CMD)> uCMD_STREAM;
	uCMD_STREAM.Register( NULL, FNPTR_IBF_HANDLER(RegisterIBFHandler) );
	sEIP.Init( (BYTE*)&uCMD_STREAM, sizeof(uCMD_STREAM) );

	sUREG_NODE sNODE( &sEIP );
	bRET = CALL_IBF_HANDLER( EIP_Execute, &sNODE.NODE );

	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	NONE
//	[1] :	HASH :	WORD
//	[2] :	fnPTR : IBF_Handler
ADD_HANDLER_IBF(_RegisterIBFHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzASM_VMCore& VMCORE = VMCORE_INTERFACE;
	if( pREGS[sARG::ARG2].PTR )
	{
		WORD wHASH = pREGS[sARG::ARG1].X;

#if CODE_TEST(__MOLA_RESOURCE_TEST__)
		WzASM_VMCore::IBF_HASH_MAP_IT itFIND = VMCORE.IBF_TABLE().find( wHASH );
		if( itFIND != VMCORE.IBF_TABLE().end() )
		{
			//HandlerIBF& rHANDLER = itFIND->second;
			printf( "----------ALREADY REGISTERED (%04X)----------\n", wHASH );
			DEBUGGING_INTERRUPT_INMOLA();
		}
#endif
		HandlerIBF& rHANDLER = VMCORE.IBF_TABLE()[wHASH];
		rHANDLER = (HandlerIBF)pREGS[sARG::ARG2].PTR;
	}

	return bRET;
}


//����������������������������������������������������������������������������
//	[0] :	NONE
//	[1] :	HASH :	WORD
//	[2] :	fnPTR : IBF_Handler
ADD_HANDLER_IBF(StartGResHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzASM_EIP sEIP;

	typedef sMETHOD_CALL_WITH_ALLOCEDMEMORY_PTR<sizeof(sMETHOD_GRES_HANDLERS_REGISTER_CMD)>
		sMETHOD_CALL_GRES_HANDLERS_REGISTER_CMD_T;

#if CODE_BACKUP
	sMETHOD_CALL_GRES_HANDLERS_REGISTER_CMD_T uCMD_STREAM;
	uCMD_STREAM.Register( NULL, FNPTR_IBF_HANDLER(RegisterGResHandler) );
	sEIP.Init( (BYTE*)&uCMD_STREAM, sizeof(uCMD_STREAM) );
#endif

	sMETHOD_CALL_GRES_HANDLERS_REGISTER_CMD_T* pCMD =
		sEIP.InitInstance< sMETHOD_CALL_GRES_HANDLERS_REGISTER_CMD_T >();
	pCMD->Register( NULL, FNPTR_IBF_HANDLER(RegisterGResHandler) );

	sUREG_NODE sNODE( &sEIP );
	bRET = CALL_IBF_HANDLER( EIP_Execute, &sNODE.NODE );

	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	NONE
//	[1] :	pALLOCED : BYTE*	(ALLOCnCLEAR_MEMORY �� ����� �����)
//	[2] :	SZ : WORD			(ALLOCnCLEAR_MEMORY �� ����� ��� ũ��)
ADD_HANDLER_IBF(RegisterGResHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	if( pREGS[sARG::ARG1].PTR && pREGS[sARG::ARG2].X == sizeof(sMETHOD_GRES_HANDLERS_REGISTER_CMD) )
	{
		sMETHOD_GRES_HANDLERS_REGISTER_CMD* pCMD = new (pREGS[sARG::ARG1].PTR) sMETHOD_GRES_HANDLERS_REGISTER_CMD;

		// <WORK TO DO>
		//		Converting
		pCMD->RegisterHandlers();

#if CODE_BACKUP
		BYTE* pCMDSTREAM;
		WORD wCMDSIZE;
		pCMD->CommanStreamInfo( pCMDSTREAM, wCMDSIZE );
		pCMD->ChangeRegisterHandler( (WzPTR)FNPTR_IBF_HANDLER(_RegisterGResHandler) );

		sUREG_NODE sPrepareEIP_CMD( NULL, pCMDSTREAM, wCMDSIZE );
#endif

		pCMD->ChangeRegisterHandler( (WzPTR)FNPTR_IBF_HANDLER(_RegisterGResHandler) );
		sUREG_NODE sPrepareEIP_CMD( NULL, pCMD, sizeof(*pCMD) );

		bRET = CALL_IBF_HANDLER( PrepareEIP, &sPrepareEIP_CMD.NODE );
		ZeroMemory( pCMD, sizeof(*pCMD) );

		// �Ҹ��ڰ� �ʿ���� ���
		MolaTAllocFree(pCMD);
		//SAFE_DELETE( pCMD );

		WzASM_EIP* pEIP = (WzASM_EIP*)bRET.PTR;
		if( pEIP )
		{
			sUREG_NODE sEIP_ExecuteCMD( pEIP );
			bRET = CALL_IBF_HANDLER( EIP_Execute, &sEIP_ExecuteCMD.NODE );
		}

		MolaTAllocDelete(WzASM_EIP, pEIP);	// SAFE_DELETE( pEIP );
	}

	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	NONE
//	[1] :	HASH :	WORD
//	[2] :	fnPTR : IBF_Handler
ADD_HANDLER_IBF(_RegisterGResHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	GResHASH& rHASH = VMCORE_INTERFACE.GRES_HASH();

	GResHASH::HASHED_HANDLER& rTABLE = rHASH.TABLE();

	WzASMH wHASH = pREGS[sARG::ARG1].X;
	IGResHandler pFNPTR = (IGResHandler)pREGS[sARG::ARG2].PTR;

	if( !pFNPTR )
	{
		__DEBUGGING_VM_OUTPUT0( "Can't Register GResHandler null ptr\n" );
		return bRET;
	}

	IWzASM_PROC& rHANDLER = rTABLE[wHASH];
	rHANDLER.DATASIZE = 0;
	rHANDLER.RESFUNC = pFNPTR;

#if CODE_BACKUP
	#undef DECL_GRES_HASH
	#define DECL_GRES_HASH(name, val, val2)				\
		do {											\
		IWzASM_PROC& rHANDLER = rTABLE[GResHASH::name];	\
		rHANDLER.DATASIZE = 0;							\
		rHANDLER.RESFUNC = CALL_IGRes##name;			\
		} while(0);

		////DECL_GRES_HASH(EMPTY_CODE								, 001, 001)
		//do {										
		//	IWzASM_PROC& rHANDLER = rTABLE[GResHASH::EMPTY_CODE];
		//	rHANDLER.DATASIZE = 0;							
		//	rHANDLER.RESFUNC = CALL_IGResEMPTY_CODE;			
		//	//uRET = rHANDLER.RESFUNC( &uRET );	
		//	printf( "GRES HASHCODE %08X listsize %u\n", &rTABLE,  rTABLE.size() );
		//} while(0);

	#	include "IWzASM.ResHashList.h"

	#undef DECL_GRES_HASH
#endif

	return bRET;
}










//////////////////////////////////////////////////////////////////////////
// <NOTE>
//	PrepareEIP�� 0��° ���ڰ� ��� �ȴ�.
//	ReturnValue : WzASM_EIP* (���� ������ EIP��ü)
//	 [0] : pEIP : WzASM_EIP*	(NULL)
//	 [1] : pCMD : BYTE*
//	 [2] : wSize : WORD
ADD_HANDLER_IBF(PrepareEIP)
{
	DEBUGGING_OUTPUT();

	//uCONV pREGS[sARG::MAXARG];
	//pREGS[sARG::EIP].PTR = NULL;
	//pREGS[sARG::ARG1].PTR = NULL;
	//pREGS[sARG::ARG2].PTR = NULL;
	//pREGS[sARG::ARG3].PTR = NULL;
	uCONV* pREGS = pNODE->EXA;

	WzASM_EIP sEIP;

	sMETHOD_CALL_IBF_PTR uCMD_STREAM;
	uCMD_STREAM.RegisterIBF(FNPTR_IBF_HANDLER(CreateEIP));
	uCMD_STREAM.ChangeARG( pREGS[sARG::ARG1].PTRB, pREGS[sARG::ARG2].X );

	sEIP.Init( (BYTE*)&uCMD_STREAM, sizeof(uCMD_STREAM) );

	sUREG_NODE sNODE( &sEIP );
	uREG_PV bRET = CALL_IBF_HANDLER( EIP_Execute, &sNODE.NODE );
	//sEIP.Execute();

	return bRET;
}

//////////////////////////////////////////////////////////////////////////
//	 [0] :	pEIP :	WzASM_EIP*
//	 [1] :	pCMD :	BYTE*
//	 [2] :	wSize :	WORD
ADD_HANDLER_IBF(CreateEIP)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };
	if( pREGS[sARG::ARG1].PTR && pREGS[sARG::ARG2].X )
	{
		// ��ȯ�� EIP ��ü
		WzASM_EIP* pEIP = MolaTAllocNew(WzASM_EIP)();// new WzASM_EIP;
		sMETHOD_CALL_IBF_PTR uCMD_STREAM;
		uCMD_STREAM.RegisterIBF(FNPTR_IBF_HANDLER(EIP_Init));
		uCMD_STREAM.ChangeARG( pREGS[sARG::ARG1].PTRB, pREGS[sARG::ARG2].X );
		uCMD_STREAM.RegisterEIP( pEIP );

		WzASM_EIP sEIP;
		sEIP.Init( (BYTE*)&uCMD_STREAM, sizeof(uCMD_STREAM) );

		sUREG_NODE sNODE( &sEIP );
		bRET = CALL_IBF_HANDLER( EIP_Execute, &sNODE.NODE );

		//sEIP.Execute();

		return VM_REGISTER.ERP.CONV;;
	}

	return bRET;
}

//////////////////////////////////////////////////////////////////////////
//	 [0] :	pEIP :	WzASM_EIP*
//	 [1] :	pCMD :	BYTE*
//	 [2] :	wSize :	WORD
ADD_HANDLER_IBF(EIP_Init)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;

	if( pREGS[sARG::EIP].PTR && pREGS[sARG::ARG1].PTR && pREGS[sARG::ARG2].X )
	{
		WzASM_EIP* pEIP = (WzASM_EIP*)pREGS[sARG::EIP].PTR;
		pEIP->Init( pREGS[sARG::ARG1].PTRB, pREGS[sARG::ARG2].X );

		return pREGS[sARG::EIP].CONV;
	}

	uREG_PV bRET = { NULL, };
	return bRET;
}

//////////////////////////////////////////////////////////////////////////
//	[0] :	pEIP :	WzASM_EIP*
ADD_HANDLER_IBF(EIP_ExecuteCode)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;

	uREG_PV bRET = { NULL, };
	return bRET;
}

//////////////////////////////////////////////////////////////////////////
//	[0] :	pEIP :	WzASM_EIP*
ADD_HANDLER_IBF(EIP_Prefetch)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };
	if( !pREGS[sARG::EIP].PTR )
		return bRET;

	WzASM_EIP& rEIP = *(WzASM_EIP*)pREGS[sARG::EIP].PTR;

	// CONTROL VARIABLES
	WzRET_T bCONTINUE	= sRET::SUCCESS;
	rEIP.ClearOP();
	rEIP.JAT.Init();

	// ������ ������...
	while( bCONTINUE && ( rEIP.OP.OFFSET.IDX<rEIP.CmdStreamSize() ) )
	{
		// ASM Code �б�
		OPW_T* pASM = (OPW_T*)rEIP.OP.OFFSET.PTR;
		rEIP.OP.OFFSET.INC( sizeof(OPW_T) );

		if( !( bCONTINUE = VMCORE_INTERFACE.Find_WzASM( *pASM, rEIP.OP.CODE_INFO ) ) )
		{
			__DEBUGGING_VM_OUTPUT2( "PREFETCH FAILED CODE", *pASM, rEIP.OP.DATASIZE() );
			continue;
		}

		// ASM �� ÷�ε� ������ �б�
		if( rEIP.OP.DATASIZE() )
		{
			rEIP.OP.CODE_DATA = (uREG_NODE*)rEIP.OP.OFFSET.PTR;
			rEIP.OP.OFFSET.INC( rEIP.OP.DATASIZE() );
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
			rEIP.JAT.Add( *pASM, rEIP.OP.OFFSET.IDX /*pASM*/ );
			break;
		};
	}

	rEIP.ClearOP();

	bRET.VAL = TRUE;
	return bRET;
}

//////////////////////////////////////////////////////////////////////////
//	[0] :	pEIP :	WzASM_EIP*
ADD_HANDLER_IBF(EIP_Execute)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

#ifdef _DEBUG
	DWORD dwSTKLVL = (DWORD)VM_STACK.m_STACK.size();
#endif //
	__DEBUGGING_VM_OUTPUT1( "EXEC - START : STK_LVL", dwSTKLVL );

	if( !pREGS[sARG::EIP].PTR )
		return bRET;

	WzASM_EIP& rEIP = *(WzASM_EIP*)pREGS[sARG::EIP].PTR;

	// CONTROL VARIABLES
	WzRET_T bCONTINUE	= sRET::SUCCESS;

	// ������ ������...
	while( bCONTINUE && ( rEIP.OP.OFFSET.IDX<rEIP.CmdStreamSize() ) )
	{
		// ASM Code �б�
		OPW_T* pASM = (OPW_T*)rEIP.OP.OFFSET.PTR;
		rEIP.OP.OFFSET.INC( sizeof(OPW_T) );

		if( !( bCONTINUE = VMCORE_INTERFACE.Find_WzASM( *pASM, rEIP.OP.CODE_INFO ) ) )
		{
			__DEBUGGING_VM_OUTPUT2( "EXECUTE FAILED CODE", *pASM, rEIP.OP.DATASIZE() );
			continue;
		}

		// ASM �� ÷�ε� ������ �б�
		if( rEIP.OP.DATASIZE() )
		{
			rEIP.OP.CODE_DATA = (uREG_NODE*)rEIP.OP.OFFSET.PTR;
			rEIP.OP.OFFSET.INC( rEIP.OP.DATASIZE() );
		}

		bCONTINUE = rEIP.ExecuteCode();
	}

	uREG_NODE uNODE = VM_REGISTER.ERP;
	//eEIP = VM_EIP_STACK.Pop();
	//VM_STACK.LoadGPR( VM_REGISTER );

	__DEBUGGING_VM_OUTPUT2( "EXEC - END : STK_LVL", VM_STACK.m_STACK.size(), dwSTKLVL );

	return uNODE.CONV;
}

//////////////////////////////////////////////////////////////////////////
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	asmCode :	OPW_T
ADD_HANDLER_IBF(EIP_FindAddress)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;

	uREG_PV bRET = { NULL, };
	return bRET;
}

//����������������������������������������������������������������������������
// <C Runtime Library & WIN32 APIs Complex API>
//////////////////////////////////////////////////////////////////////////
//	[0] : pEIP : WzASM_EIP*	(NULL)
//	[1] : wSize : WORD
ADD_HANDLER_IBF(ALLOCnCLEAR_MEMORY)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	if( pREGS[sARG::ARG1].X )
	{
		sMETHOD_ALLOCnCLEAR_MEMORY_PTR<> sCMD;
		sCMD.ChangeSize( pREGS[sARG::ARG1].X );

		sUREG_NODE sPrepareEIP_CMD( NULL, &sCMD, sizeof(sCMD) );
		bRET = CALL_IBF_HANDLER( PrepareEIP, &sPrepareEIP_CMD.NODE );

		WzASM_EIP* pEIP = (WzASM_EIP*)bRET.PTR;
		if( pEIP )
		{
			sCMD.RegisterEIP( pEIP );

			sUREG_NODE sEIP_ExecuteCMD( pEIP );
			bRET = CALL_IBF_HANDLER( EIP_Execute, &sEIP_ExecuteCMD.NODE );

			MolaTAllocDelete(WzASM_EIP, pEIP);	// delete pEIP;
		}

		return bRET;
	}

	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	Sz :	SIZE_T
ADD_HANDLER_IBF(malloc)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzPTR& rPTR = VM_REGISTER.ERP.PTR;
	rPTR = MolaTAllocMalloc(pREGS[sARG::ARG1].EX);
		//malloc( pREGS[sARG::ARG1].EX );

	if( rPTR )
	{
		bRET.PTR = rPTR;
        return bRET;
	}

	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	Dest :	PVOID
//	[2] :	Sz :	SIZE_T
ADD_HANDLER_IBF(ZeroMemory)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	if( pREGS[sARG::ARG1].PTR )
	{
		ZeroMemory( pREGS[sARG::ARG1].PTR, pREGS[sARG::ARG2].EX );
		//VM_REGISTER.ERP.EX = TRUE;

		bRET.VAL = TRUE;
		return bRET;
	}
	//VM_REGISTER.ERP.EX = FALSE;

	bRET.VAL = FALSE;
	return bRET;
}

//����������������������������������������������������������������������������
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	Dest :	PVOID
//	[2] :	Src :	PVOID
//	[3] :	Sz :	SIZE_T
ADD_HANDLER_IBF(CopyMemory)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	if( pREGS[sARG::ARG1].PTR && pREGS[sARG::ARG2].PTR )
	{
		bRET.PTR = CopyMemory( pREGS[sARG::ARG1].PTR, pREGS[sARG::ARG2].PTR, pREGS[sARG::ARG3].EX );
		return bRET;
	}
	VM_REGISTER.ERP.PTR = NULL;
	
	bRET.PTR = NULL;
	return bRET;
}

//����������������������������������������������������������������������������
ADD_HANDLER_IBF(GetTickCount)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	bRET.VAL = GetTickCount();

	return bRET;
}



//����������������������������������������������������������������������������
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	ptr :	PVOID (������ �޸� ���۹���)
ADD_HANDLER_IBF(free)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzPTR ptr = pREGS[sARG::ARG1].PTR;
	if( ptr )
	{
		//free( ptr );
		MolaTAllocFree(ptr);
	}

	return bRET;
}


//
//
//
//
//


//����������������������������������������������������������������������������
//
ADD_HANDLER_IBF(GetMOLAVersion)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	bRET.VAL = CryptManager::VERSION_CODE;

	return bRET;
}

//����������������������������������������������������������������������������
//	<NOTE>
//		EIP��ü�� REF�� �־��� �ؽ��� �ش��ϴ� GRES �Լ� ������ �Է��Ѵ�.
//	<RETURN>
//		TRUE, FALSE
//	<ARGUMENTs>
//	[0] :	pEIP :	WzASM_EIP*
//	[1] :	wHASH :	WORD
ADD_HANDLER_IBF(CallGRESHandler)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzASM_EIP* pEIP = (WzASM_EIP*)pREGS[sARG::EIP].PTR;
	WORD& rwSZ = pREGS[sARG::ARG1].X;
	if( pEIP && rwSZ )
	{
		IWzASM_PROC uRES = VMCORE_INTERFACE.FindGResHandlerInfo( rwSZ );
		pEIP->Register_REF( uRES );

		if( uRES.RESFUNC )
		{
			uREG_NODE uNODE = VM_STACK.Pop();
			uNODE = uRES.RESFUNC( &uNODE );
			return uNODE.CONV;
		}
	}

	return bRET;
}



//����������������������������������������������������������������������������
//	<NOTE>
//		ITimeout Controller : IsExpired
//	<RETURN>
//		TRUE, FALSE
//	<ARGUMENTs>
//	[0] :	pEIP :	NULL
//	[1] :	ITimeout*
ADD_HANDLER_IBF(ITimeout_IsExpired)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzPTR& rPTR = pREGS[sARG::ARG1].PTR;
	if( rPTR )
	{
		ITimeout* pTIMEOUT = (ITimeout*)rPTR;
		bRET.VAL = (DWORD)pTIMEOUT->IsExpired();

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_VMLOG_T__)
		if( bRET.VAL )
			printf( "Expired %u %u\n", pTIMEOUT->TimeoutTick(), GetTickCount() );
#endif
	}

	return bRET;
}

//����������������������������������������������������������������������������
//	<NOTE>
//		ITimeout Controller : ITimeout_SetNextTimeout
//	<RETURN>
//		EMPTY
//	<ARGUMENTs>
//	[0] :	pEIP :	NULL
//	[1] :	ITimeout*
//	[2] :	NextTick
ADD_HANDLER_IBF(ITimeout_SetNextTimeout)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzPTR& rPTR = pREGS[sARG::ARG1].PTR;
	if( rPTR )
	{
		ITimeout* pTIMEOUT = (ITimeout*)rPTR;
		pTIMEOUT->SetNextTimeout( pREGS[sARG::ARG2].EX );
	}

	return bRET;
}

//����������������������������������������������������������������������������
//	<NOTE>
//		ITimeout Controller : ITimeout_SetNTCTB
//	<RETURN>
//		EMPTY
//	<ARGUMENTs>
//	[0] :	pEIP :	NULL
//	[1] :	ITimeout*
ADD_HANDLER_IBF(ITimeout_SetNTCTB)
{
	DEBUGGING_OUTPUT();

	uCONV* pREGS = pNODE->EXA;
	uREG_PV bRET = { NULL, };

	WzPTR& rPTR = pREGS[sARG::ARG1].PTR;
	ITimeout* pTIMEOUT = NULL;
	if( rPTR )
	{
		pTIMEOUT = (ITimeout*)rPTR;
		pTIMEOUT->SetNextTimeoutByCurTickBased( pREGS[sARG::ARG2].EX );
	}

	return bRET;
}

//����������������������������������������������������������������������������
//
//
//

#undef DECL_IBF_HASH
#define DECL_IBF_HASH(name, val, val2)							\
	uREG_PV		HANDLER_WzASM_IBF_##name( uREG_NODE* pNODE ) {	\
		DEBUGGING_OUTPUT();										\
		uREG_PV bRET = { NULL, };								\
		return bRET;	}

DECL_IBF_HASH(	RESERVED_IBF014,							014,	014 )
DECL_IBF_HASH(	RESERVED_IBF015,							015,	015 )
DECL_IBF_HASH(	RESERVED_IBF018,							018,	018 )
DECL_IBF_HASH(	RESERVED_IBF019,							019,	019 )
DECL_IBF_HASH(	RESERVED_IBF020,							020,	020 )
DECL_IBF_HASH(	RESERVED_IBF021,							021,	021 )
DECL_IBF_HASH(	RESERVED_IBF022,							022,	022 )
DECL_IBF_HASH(	RESERVED_IBF023,							023,	023 )
//DECL_IBF_HASH(	RESERVED_IBF024,							024,	024 )
//DECL_IBF_HASH(	RESERVED_IBF025,							025,	025 )
//DECL_IBF_HASH(	RESERVED_IBF026,							026,	026 )
DECL_IBF_HASH(	RESERVED_IBF027,							027,	027 )
DECL_IBF_HASH(	RESERVED_IBF028,							028,	028 )
DECL_IBF_HASH(	RESERVED_IBF029,							029,	029 )
DECL_IBF_HASH(	RESERVED_IBF030,							030,	030 )
DECL_IBF_HASH(	RESERVED_IBF031,							031,	031 )
DECL_IBF_HASH(	RESERVED_IBF032,							032,	032 )
DECL_IBF_HASH(	RESERVED_IBF033,							033,	033 )
DECL_IBF_HASH(	RESERVED_IBF034,							034,	034 )
DECL_IBF_HASH(	RESERVED_IBF035,							035,	035 )
DECL_IBF_HASH(	RESERVED_IBF036,							036,	036 )
DECL_IBF_HASH(	RESERVED_IBF037,							037,	037 )
DECL_IBF_HASH(	RESERVED_IBF038,							038,	038 )
DECL_IBF_HASH(	RESERVED_IBF039,							039,	039 )
DECL_IBF_HASH(	RESERVED_IBF040,							040,	040 )
DECL_IBF_HASH(	RESERVED_IBF041,							041,	041 )
DECL_IBF_HASH(	RESERVED_IBF042,							042,	042 )
DECL_IBF_HASH(	RESERVED_IBF043,							043,	043 )
DECL_IBF_HASH(	RESERVED_IBF044,							044,	044 )
DECL_IBF_HASH(	RESERVED_IBF045,							045,	045 )
DECL_IBF_HASH(	RESERVED_IBF046,							046,	046 )
DECL_IBF_HASH(	RESERVED_IBF047,							047,	047 )
DECL_IBF_HASH(	RESERVED_IBF048,							048,	048 )
DECL_IBF_HASH(	RESERVED_IBF049,							049,	049 )
DECL_IBF_HASH(	RESERVED_IBF050,							050,	050 )
DECL_IBF_HASH(	RESERVED_IBF051,							051,	051 )
DECL_IBF_HASH(	RESERVED_IBF052,							052,	052 )
DECL_IBF_HASH(	RESERVED_IBF053,							053,	053 )
DECL_IBF_HASH(	RESERVED_IBF054,							054,	054 )
DECL_IBF_HASH(	RESERVED_IBF055,							055,	055 )
DECL_IBF_HASH(	RESERVED_IBF056,							056,	056 )
DECL_IBF_HASH(	RESERVED_IBF057,							057,	057 )
DECL_IBF_HASH(	RESERVED_IBF058,							058,	058 )
DECL_IBF_HASH(	RESERVED_IBF059,							059,	059 )
DECL_IBF_HASH(	RESERVED_IBF060,							060,	060 )
DECL_IBF_HASH(	RESERVED_IBF061,							061,	061 )
DECL_IBF_HASH(	RESERVED_IBF062,							062,	062 )
DECL_IBF_HASH(	RESERVED_IBF063,							063,	063 )
DECL_IBF_HASH(	RESERVED_IBF064,							064,	064 )
DECL_IBF_HASH(	RESERVED_IBF065,							065,	065 )
DECL_IBF_HASH(	RESERVED_IBF066,							066,	066 )
DECL_IBF_HASH(	RESERVED_IBF067,							067,	067 )
DECL_IBF_HASH(	RESERVED_IBF068,							068,	068 )
DECL_IBF_HASH(	RESERVED_IBF069,							069,	069 )
DECL_IBF_HASH(	RESERVED_IBF070,							070,	070 )
DECL_IBF_HASH(	RESERVED_IBF071,							071,	071 )
DECL_IBF_HASH(	RESERVED_IBF072,							072,	072 )
DECL_IBF_HASH(	RESERVED_IBF073,							073,	073 )
DECL_IBF_HASH(	RESERVED_IBF074,							074,	074 )
DECL_IBF_HASH(	RESERVED_IBF075,							075,	075 )
DECL_IBF_HASH(	RESERVED_IBF076,							076,	076 )
DECL_IBF_HASH(	RESERVED_IBF077,							077,	077 )
DECL_IBF_HASH(	RESERVED_IBF078,							078,	078 )
DECL_IBF_HASH(	RESERVED_IBF079,							079,	079 )
DECL_IBF_HASH(	RESERVED_IBF080,							080,	080 )
DECL_IBF_HASH(	RESERVED_IBF081,							081,	081 )
DECL_IBF_HASH(	RESERVED_IBF082,							082,	082 )
DECL_IBF_HASH(	RESERVED_IBF083,							083,	083 )
DECL_IBF_HASH(	RESERVED_IBF084,							084,	084 )
DECL_IBF_HASH(	RESERVED_IBF085,							085,	085 )
DECL_IBF_HASH(	RESERVED_IBF086,							086,	086 )
DECL_IBF_HASH(	RESERVED_IBF087,							087,	087 )
DECL_IBF_HASH(	RESERVED_IBF088,							088,	088 )
DECL_IBF_HASH(	RESERVED_IBF089,							089,	089 )
DECL_IBF_HASH(	RESERVED_IBF090,							090,	090 )
DECL_IBF_HASH(	RESERVED_IBF091,							091,	091 )
DECL_IBF_HASH(	RESERVED_IBF092,							092,	092 )
DECL_IBF_HASH(	RESERVED_IBF093,							093,	093 )
DECL_IBF_HASH(	RESERVED_IBF094,							094,	094 )
DECL_IBF_HASH(	RESERVED_IBF095,							095,	095 )
DECL_IBF_HASH(	RESERVED_IBF096,							096,	096 )
DECL_IBF_HASH(	RESERVED_IBF097,							097,	097 )
DECL_IBF_HASH(	RESERVED_IBF098,							098,	098 )
DECL_IBF_HASH(	RESERVED_IBF099,							099,	099 )
DECL_IBF_HASH(	RESERVED_IBF100,							100,	100 )
DECL_IBF_HASH(	RESERVED_IBF101,							101,	101 )
DECL_IBF_HASH(	RESERVED_IBF102,							102,	102 )
DECL_IBF_HASH(	RESERVED_IBF103,							103,	103 )
DECL_IBF_HASH(	RESERVED_IBF104,							104,	104 )
DECL_IBF_HASH(	RESERVED_IBF105,							105,	105 )
DECL_IBF_HASH(	RESERVED_IBF106,							106,	106 )
DECL_IBF_HASH(	RESERVED_IBF107,							107,	107 )
DECL_IBF_HASH(	RESERVED_IBF108,							108,	108 )
DECL_IBF_HASH(	RESERVED_IBF109,							109,	109 )
DECL_IBF_HASH(	RESERVED_IBF110,							110,	110 )
DECL_IBF_HASH(	RESERVED_IBF111,							111,	111 )
DECL_IBF_HASH(	RESERVED_IBF112,							112,	112 )
DECL_IBF_HASH(	RESERVED_IBF113,							113,	113 )
DECL_IBF_HASH(	RESERVED_IBF114,							114,	114 )
DECL_IBF_HASH(	RESERVED_IBF115,							115,	115 )
DECL_IBF_HASH(	RESERVED_IBF116,							116,	116 )
DECL_IBF_HASH(	RESERVED_IBF117,							117,	117 )
DECL_IBF_HASH(	RESERVED_IBF118,							118,	118 )
DECL_IBF_HASH(	RESERVED_IBF119,							119,	119 )
DECL_IBF_HASH(	RESERVED_IBF120,							120,	120 )
DECL_IBF_HASH(	RESERVED_IBF121,							121,	121 )
DECL_IBF_HASH(	RESERVED_IBF122,							122,	122 )
DECL_IBF_HASH(	RESERVED_IBF123,							123,	123 )
DECL_IBF_HASH(	RESERVED_IBF124,							124,	124 )
DECL_IBF_HASH(	RESERVED_IBF125,							125,	125 )
DECL_IBF_HASH(	RESERVED_IBF126,							126,	126 )
DECL_IBF_HASH(	RESERVED_IBF127,							127,	127 )
DECL_IBF_HASH(	RESERVED_IBF128,							128,	128 )
DECL_IBF_HASH(	RESERVED_IBF129,							129,	129 )
DECL_IBF_HASH(	RESERVED_IBF130,							130,	130 )
DECL_IBF_HASH(	RESERVED_IBF131,							131,	131 )
DECL_IBF_HASH(	RESERVED_IBF132,							132,	132 )
DECL_IBF_HASH(	RESERVED_IBF133,							133,	133 )
DECL_IBF_HASH(	RESERVED_IBF134,							134,	134 )
DECL_IBF_HASH(	RESERVED_IBF135,							135,	135 )
DECL_IBF_HASH(	RESERVED_IBF136,							136,	136 )
DECL_IBF_HASH(	RESERVED_IBF137,							137,	137 )
DECL_IBF_HASH(	RESERVED_IBF138,							138,	138 )
DECL_IBF_HASH(	RESERVED_IBF139,							139,	139 )
DECL_IBF_HASH(	RESERVED_IBF140,							140,	140 )
DECL_IBF_HASH(	RESERVED_IBF141,							141,	141 )
DECL_IBF_HASH(	RESERVED_IBF142,							142,	142 )
DECL_IBF_HASH(	RESERVED_IBF143,							143,	143 )
DECL_IBF_HASH(	RESERVED_IBF144,							144,	144 )
DECL_IBF_HASH(	RESERVED_IBF145,							145,	145 )
DECL_IBF_HASH(	RESERVED_IBF146,							146,	146 )
DECL_IBF_HASH(	RESERVED_IBF147,							147,	147 )
DECL_IBF_HASH(	RESERVED_IBF148,							148,	148 )
DECL_IBF_HASH(	RESERVED_IBF149,							149,	149 )
DECL_IBF_HASH(	RESERVED_IBF150,							150,	150 )
DECL_IBF_HASH(	RESERVED_IBF151,							151,	151 )
DECL_IBF_HASH(	RESERVED_IBF152,							152,	152 )
DECL_IBF_HASH(	RESERVED_IBF153,							153,	153 )
DECL_IBF_HASH(	RESERVED_IBF154,							154,	154 )
DECL_IBF_HASH(	RESERVED_IBF155,							155,	155 )
DECL_IBF_HASH(	RESERVED_IBF156,							156,	156 )
DECL_IBF_HASH(	RESERVED_IBF157,							157,	157 )
DECL_IBF_HASH(	RESERVED_IBF158,							158,	158 )
DECL_IBF_HASH(	RESERVED_IBF159,							159,	159 )
DECL_IBF_HASH(	RESERVED_IBF160,							160,	160 )
DECL_IBF_HASH(	RESERVED_IBF161,							161,	161 )
DECL_IBF_HASH(	RESERVED_IBF162,							162,	162 )
DECL_IBF_HASH(	RESERVED_IBF163,							163,	163 )
DECL_IBF_HASH(	RESERVED_IBF164,							164,	164 )
DECL_IBF_HASH(	RESERVED_IBF165,							165,	165 )
DECL_IBF_HASH(	RESERVED_IBF166,							166,	166 )
DECL_IBF_HASH(	RESERVED_IBF167,							167,	167 )
DECL_IBF_HASH(	RESERVED_IBF168,							168,	168 )
DECL_IBF_HASH(	RESERVED_IBF169,							169,	169 )
DECL_IBF_HASH(	RESERVED_IBF170,							170,	170 )
DECL_IBF_HASH(	RESERVED_IBF171,							171,	171 )
DECL_IBF_HASH(	RESERVED_IBF172,							172,	172 )
DECL_IBF_HASH(	RESERVED_IBF173,							173,	173 )
DECL_IBF_HASH(	RESERVED_IBF174,							174,	174 )
DECL_IBF_HASH(	RESERVED_IBF175,							175,	175 )
DECL_IBF_HASH(	RESERVED_IBF176,							176,	176 )
DECL_IBF_HASH(	RESERVED_IBF177,							177,	177 )
DECL_IBF_HASH(	RESERVED_IBF178,							178,	178 )
DECL_IBF_HASH(	RESERVED_IBF179,							179,	179 )
DECL_IBF_HASH(	RESERVED_IBF180,							180,	180 )
DECL_IBF_HASH(	RESERVED_IBF181,							181,	181 )
DECL_IBF_HASH(	RESERVED_IBF182,							182,	182 )
DECL_IBF_HASH(	RESERVED_IBF183,							183,	183 )
DECL_IBF_HASH(	RESERVED_IBF184,							184,	184 )
DECL_IBF_HASH(	RESERVED_IBF185,							185,	185 )
DECL_IBF_HASH(	RESERVED_IBF186,							186,	186 )
DECL_IBF_HASH(	RESERVED_IBF187,							187,	187 )
DECL_IBF_HASH(	RESERVED_IBF188,							188,	188 )
DECL_IBF_HASH(	RESERVED_IBF189,							189,	189 )
DECL_IBF_HASH(	RESERVED_IBF190,							190,	190 )
DECL_IBF_HASH(	RESERVED_IBF191,							191,	191 )
DECL_IBF_HASH(	RESERVED_IBF192,							192,	192 )
DECL_IBF_HASH(	RESERVED_IBF193,							193,	193 )
DECL_IBF_HASH(	RESERVED_IBF194,							194,	194 )
DECL_IBF_HASH(	RESERVED_IBF195,							195,	195 )
DECL_IBF_HASH(	RESERVED_IBF196,							196,	196 )
DECL_IBF_HASH(	RESERVED_IBF197,							197,	197 )
DECL_IBF_HASH(	RESERVED_IBF198,							198,	198 )
DECL_IBF_HASH(	RESERVED_IBF199,							199,	199 )
DECL_IBF_HASH(	RESERVED_IBF200,							200,	200 )
DECL_IBF_HASH(	RESERVED_IBF201,							201,	201 )
DECL_IBF_HASH(	RESERVED_IBF202,							202,	202 )
DECL_IBF_HASH(	RESERVED_IBF203,							203,	203 )
DECL_IBF_HASH(	RESERVED_IBF204,							204,	204 )
DECL_IBF_HASH(	RESERVED_IBF205,							205,	205 )
DECL_IBF_HASH(	RESERVED_IBF206,							206,	206 )
DECL_IBF_HASH(	RESERVED_IBF207,							207,	207 )
DECL_IBF_HASH(	RESERVED_IBF208,							208,	208 )
DECL_IBF_HASH(	RESERVED_IBF209,							209,	209 )
DECL_IBF_HASH(	RESERVED_IBF210,							210,	210 )
DECL_IBF_HASH(	RESERVED_IBF211,							211,	211 )
DECL_IBF_HASH(	RESERVED_IBF212,							212,	212 )
DECL_IBF_HASH(	RESERVED_IBF213,							213,	213 )
DECL_IBF_HASH(	RESERVED_IBF214,							214,	214 )
DECL_IBF_HASH(	RESERVED_IBF215,							215,	215 )
DECL_IBF_HASH(	RESERVED_IBF216,							216,	216 )
DECL_IBF_HASH(	RESERVED_IBF217,							217,	217 )
DECL_IBF_HASH(	RESERVED_IBF218,							218,	218 )
DECL_IBF_HASH(	RESERVED_IBF219,							219,	219 )
DECL_IBF_HASH(	RESERVED_IBF220,							220,	220 )
DECL_IBF_HASH(	RESERVED_IBF221,							221,	221 )
DECL_IBF_HASH(	RESERVED_IBF222,							222,	222 )
DECL_IBF_HASH(	RESERVED_IBF223,							223,	223 )
DECL_IBF_HASH(	RESERVED_IBF224,							224,	224 )
DECL_IBF_HASH(	RESERVED_IBF225,							225,	225 )
DECL_IBF_HASH(	RESERVED_IBF226,							226,	226 )
DECL_IBF_HASH(	RESERVED_IBF227,							227,	227 )
DECL_IBF_HASH(	RESERVED_IBF228,							228,	228 )
DECL_IBF_HASH(	RESERVED_IBF229,							229,	229 )
DECL_IBF_HASH(	RESERVED_IBF230,							230,	230 )
DECL_IBF_HASH(	RESERVED_IBF231,							231,	231 )
DECL_IBF_HASH(	RESERVED_IBF232,							232,	232 )
DECL_IBF_HASH(	RESERVED_IBF233,							233,	233 )
DECL_IBF_HASH(	RESERVED_IBF234,							234,	234 )
DECL_IBF_HASH(	RESERVED_IBF235,							235,	235 )
DECL_IBF_HASH(	RESERVED_IBF236,							236,	236 )
DECL_IBF_HASH(	RESERVED_IBF237,							237,	237 )
DECL_IBF_HASH(	RESERVED_IBF238,							238,	238 )
DECL_IBF_HASH(	RESERVED_IBF239,							239,	239 )
DECL_IBF_HASH(	RESERVED_IBF240,							240,	240 )
DECL_IBF_HASH(	RESERVED_IBF241,							241,	241 )
DECL_IBF_HASH(	RESERVED_IBF242,							242,	242 )
DECL_IBF_HASH(	RESERVED_IBF243,							243,	243 )
DECL_IBF_HASH(	RESERVED_IBF244,							244,	244 )
DECL_IBF_HASH(	RESERVED_IBF245,							245,	245 )
DECL_IBF_HASH(	RESERVED_IBF246,							246,	246 )
DECL_IBF_HASH(	RESERVED_IBF247,							247,	247 )
DECL_IBF_HASH(	RESERVED_IBF248,							248,	248 )
DECL_IBF_HASH(	RESERVED_IBF249,							249,	249 )
DECL_IBF_HASH(	RESERVED_IBF250,							250,	250 )
DECL_IBF_HASH(	RESERVED_IBF251,							251,	251 )
DECL_IBF_HASH(	RESERVED_IBF252,							252,	252 )
DECL_IBF_HASH(	RESERVED_IBF253,							253,	253 )
DECL_IBF_HASH(	RESERVED_IBF254,							254,	254 )
DECL_IBF_HASH(	RESERVED_IBF255,							255,	255 )
DECL_IBF_HASH(	RESERVED_IBF256,							256,	256 )


