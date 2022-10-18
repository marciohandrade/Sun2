#include "stdafx.h"
#include "IWzASM.h"
#include "VMOp/VMOpCPU.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//static WzASM_VMCore* spCORE = NULL;

WzGResTable::WzGResTable()
{
#if SUN_CODE_BACKUP
	MOLASharedMemoryPool::CreateInstance();
#endif
	//WzASM_VMCore*& rpCORE = WzASM_VMCore::sp_VMCore;
	WzASM_VMCore* pCORE = new WzASM_VMCore;
	pCORE->Load( this );
}

WzGResTable::~WzGResTable()
{
	SAFE_DELETE( WzASM_VMCore::sp_VMCore );
#if SUN_CODE_BACKUP
	MOLASharedMemoryPool::DestroyInstance();
#endif
}



#define IWZASM_REGISTER_DATA_HANDLER(name, size)		\
	do {												\
	HASHED_HANDLER& rTABLE = *TABLE_PTR();				\
	IWzASM_PROC& rHANDLER = rTABLE[GResHASH::name];	\
	rHANDLER.DATASIZE = size;						\
	rHANDLER.RESDATA = HANDLER_##name;				\
	} while(0)

#define IWZASM_REGISTER_FUNC_HANDLER(name, size)		\
	do {												\
	HASHED_HANDLER& rTABLE = *TABLE_PTR();				\
	IWzASM_PROC& rHANDLER = rTABLE[GResHASH::name];	\
	rHANDLER.DATASIZE = size;						\
	rHANDLER.RESFUNC = HANDLER_##name;				\
	} while(0)


#pragma pack(push, 1 )
struct sTEST_GRES_FUNCTION_CALL
{
	sIL_MOV_EAX1_B2<GResHASH::EMPTY_CODE>	M01;
	sIL<IWzASM::PUSH_EAX>					M02;
	sIL<IWzASM::LEA_REF_HASH>				M03;
	sDTH<GResHASH::EMPTY_CODE>			M04;
	sIL<IWzASM::CALL_RC>					M05;
	sIL_RET									M06;
} test_cmdstream;
#pragma pack(pop)

VOID	WzGResTable::Register()
{
	//IWzASM_PROC& rHANDLER = TABLE()[HASHED::GET_C2S_GAME_SENDCRYPT_INFO_DATA];
	//rHANDLER.DATASIZE = 0;
	//rHANDLER.RESDATA = HANDLER_GET_C2S_GAME_SENDCRYPT_INFO_DATA;
	//IWZASM_REGISTER_DATA_HANDLER( GET_C2S_GAME_SENDCRYPT_INFO_DATA, 0 );
	//IWZASM_REGISTER_DATA_HANDLER( GET_C2S_GAME_RECVCRYPT_INFO_DATA, 0 );
	//IWZASM_REGISTER_FUNC_HANDLER( GET_CHAR_MOVSPD_IN_DATA, 1 );
	//IWZASM_REGISTER_FUNC_HANDLER( GET_CHAR_MOVSPD_IN_GAME, 1 );

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_TESTLOG__)
	//uREG_NODE reg;
	//reg.EX = 10;
	//reg = GET_CHAR_MOVSPD_IN_GAME( &reg );
	//printf( "COOPERATING TEST in DLL %u\n", reg.EX );
	do
	{
		uREG_NODE sNODE;
		sNODE.EXA[0].PTR = NULL;
		sNODE.EXA[1].PTR = &test_cmdstream;
		sNODE.EXA[2].X = sizeof(test_cmdstream);
		sNODE.EXA[3].PTR = NULL;
		uREG_PV bRET = HANDLER_WzASM_IBF_PrepareEIP( &sNODE );
		WzASM_EIP* pEIP2 = (WzASM_EIP*)bRET.PTR;

		pEIP2->Prefetch();
		uREG_NODE uNODE2 = pEIP2->Execute();

		MolaTAllocDelete(WzASM_EIP, pEIP2);	// SAFE_DELETE(pEIP2);
	} while(0);

//	printf( "01:	%04X\n", GResHASH::TEST01 );
#endif //
};

VOID		WzGResTable::UnRegister()
{
}


