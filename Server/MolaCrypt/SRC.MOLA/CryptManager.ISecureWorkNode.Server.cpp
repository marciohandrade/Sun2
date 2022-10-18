#include "stdafx.h"

#if SERVER_CODE || CODE_TEST(__MOLA_IwASM_CMD_SERVER_CODE_TEST__)


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<SECURE CMD CONTROL INTERFACEs>
DWORD	ISecureQueryProcessor::ms_dwWORK_KEY_INC = ISecureQueryProcessor::MOLA_WORK_KEY_LBND;

static DWORD	sINITIAL_KEYs[7] = { MOLA_RCG_012, MOLA_RCG_011, MOLA_RCG_052, MOLA_RCG_093, MOLA_RCG_024, MOLA_RCG_055, MOLA_RCG_076 };
static DWORD	sWORKING_KEYs[7] = { 0, };
DWORD*		ISecureQueryProcessor::ms_pGLOBAL_RND_KEY = sWORKING_KEYs;

ISecureQueryProcessor::ISecureQueryProcessor()
{
	RNGKEY_InsertKeys();
	RNDKEY_CALC();
}

ISecureQueryProcessor::~ISecureQueryProcessor()
{
}

QUERIED_LIST_IT		ISecureQueryProcessor::FindMolaQueriedMsg( QUERIED_LIST& IN rLIST, const DWORD dwWorkKey )
{
	QUERIED_LIST_IT end = rLIST.end();
	for( QUERIED_LIST_IT it=rLIST.begin() ; it!=rLIST.end() ; ++it )
	{
		MOLA_QUERY_MSG& rMSG = *it;
		if( dwWorkKey == rMSG.m_dwWorkKey )
			return it;
	}

	return end;
}

VOID				ISecureQueryProcessor::DeleteUser( DWORD dwUserKey )
{
	QUERIED_INFO_MAP_IT itFIND = m_QueriedTable.find( dwUserKey );
	if( itFIND != m_QueriedTable.end() )
	{
		QUERIED_LIST& rLIST = itFIND->second;
		QUERIED_LIST_IT end = rLIST.end();
		for( QUERIED_LIST_IT it=rLIST.begin() ; it!=rLIST.end() ; ++it )
		{
			MOLA_QUERY_MSG& rMSG = *it;
			if( rMSG.m_pBackupMsg )
				MolaTAllocFree(rMSG.m_pBackupMsg);
		}

		m_QueriedTable.erase( itFIND );
	}
}

DWORD				ISecureQueryProcessor::RegisterQueriedMsg( MOLA_QUERY_MSG* IN pARG )
{
	//	pARG != NULL
	DWORD dwWorkKey = GetNewWorkKey();

	QUERIED_LIST& IN rLIST = GetQueriedList( pARG->m_dwUserKey );
	pARG->m_dwWorkKey = dwWorkKey;
	rLIST.push_back( *pARG );

	return dwWorkKey;
}



static sSERVER_QUERY_LINKER s_sSERVER_QUERY_LINKER_EMPTY =
{
	eSECURE_QUERY::NON_OPERATION, 0, NULL
};

sSERVER_QUERY_LINKER&		ISecureQueryProcessor::GetRandomQueryInfo( eSECURE_QUERY::IDX_T queryT )
{
	if( (eSECURE_QUERY::NON_OPERATION == queryT) || !(eSECURE_QUERY::MAX_SECURE_PACKET > (DWORD)queryT) )
		return s_sSERVER_QUERY_LINKER_EMPTY;

	static DWORD dwRandKey = _RNDFN_F1( GetTickCount(), GetTickCount(), GetTickCount() );

	QUERY_VECTOR& rLIST = m_QueryTable[queryT];
	DWORD dwSZnIDX = (DWORD)rLIST.size();
	if( dwSZnIDX )
	{
		dwRandKey = _RNDFN_F0( SHA32_T3(dwRandKey), GetTickCount(), MOLA_RCG_T1_002 );
		dwSZnIDX = dwRandKey%dwSZnIDX;
		sSERVER_QUERY_LINKER& rINFO = rLIST[dwSZnIDX];

		return rINFO;
	}

	return s_sSERVER_QUERY_LINKER_EMPTY;
}

//
//	<SECURE CMD CONTROL INTERFACEs>
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫


//////////////////////////////////////////////////////////////////////////
//
#undef MAKE_RND_DWORD_BASE_KEY
#define MAKE_RND_DWORD_BASE_KEY( a, b, c, d, e, f, g )						\
	( _RNDFN32_OUT0(b, c, d, e, f, a, g) + _RNDFN32_OUT1(c, d, e, f, a, b, g)	\
	+ _RNDFN32_OUT2(d, e, f, a, b, c, g) + _RNDFN32_OUT3(e, f, a, b, c, d, g) )

VOID		ISecureQueryProcessor::RNGKEY_InsertKeys()
{
    BOOST_STATIC_ASSERT(_countof(sINITIAL_KEYs) == _countof(sWORKING_KEYs));
    CopyMemory(sWORKING_KEYs, sINITIAL_KEYs, sizeof(sWORKING_KEYs));
}

VOID		ISecureQueryProcessor::RNDKEY_ROLKeys()
{
    DWORD temp[_countof(sWORKING_KEYs)];
    CopyMemory(temp, &sWORKING_KEYs[1], sizeof(sWORKING_KEYs) - sizeof(sWORKING_KEYs[0]));
    temp[_countof(temp) - 1] = sWORKING_KEYs[0];
    CopyMemory(sWORKING_KEYs, temp, sizeof(sWORKING_KEYs));
}

DWORD		ISecureQueryProcessor::RNDKEY_CALC()
{
	sWORKING_KEYs[0] = MAKE_RND_DWORD_BASE_KEY(\
        sWORKING_KEYs[0], sWORKING_KEYs[1], sWORKING_KEYs[2], sWORKING_KEYs[3],
        sWORKING_KEYs[4], sWORKING_KEYs[5], sWORKING_KEYs[6]);
	RNDKEY_ROLKeys();

	return sWORKING_KEYs[0];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
DWORD		ISecureWorkNode::QueryProcess( MOLA_QUERY_MSG* pARG )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );
	if( pARG )
	{
		DWORD dwKEY = pARG->m_dwUserKey;
		eSECURE_QUERY::IDX_T eSQT = pARG->m_eSQT;

		if( eSECURE_QUERY::NON_OPERATION == eSQT )
			return 0;

		WzASM_VMCore& rIVMCORE = VMCORE_INTERFACE;
		ISecureQueryProcessor& rSQP = VM_SQP;
		QUERIED_LIST& rLIST = rSQP.GetQueriedList( dwKEY );

		switch( eSQT )
		{
		case eSECURE_QUERY::SIMPLEX_PATTERN_SNK:
			break;
		case eSECURE_QUERY::QUERY_MOLA:
			break;
		case eSECURE_QUERY::CONFIGURE_MOLA:
			break;
		case eSECURE_QUERY::DUPLEX_PATTERN_SNK:
			break;
		case eSECURE_QUERY::RESOURCE_CHECK:
			break;
		case eSECURE_QUERY::BUILTIN_QUERY:
			{
				ISecureQueryProcessor& rISQP = VM_SQP;

				IWzASM_PROC uRES = rIVMCORE.FindGResHandlerInfo( GResHASH::NETWORK_SENDPACKET_TO_GAMESERVER );
				MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_CMD* pMsgCMD = MolaTAllocNew(MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_CMD)();
				DWORD dwRndKey = rISQP.RNDKEY_CALC();
				DWORD dwIdx = (dwRndKey%1024);
				*(DWORD*)&(pMsgCMD->m_dwBuiltinCMD[0]) = (WORD)(dwIdx);
				*(DWORD*)&(pMsgCMD->m_dwBuiltinCMD[4]) = dwRndKey;

				uREG_NODE rARG;
				rARG.EXA[0].PTR = pMsgCMD;
				rARG.EXA[1].EX = sizeof(*pMsgCMD);
				uRES.RESFUNC( &rARG );

				// 결과값 저장
				if( GET_ARRAY_SIZE(BuiltinQueryFunc, BUILTIN_QUERY_FUNCs) > dwIdx )
				{
					BuiltinQueryFunc func = BUILTIN_QUERY_FUNCs[dwIdx];
					dwRndKey = func( dwRndKey );
				}
                BOOST_STATIC_ASSERT(_countof(pMsgCMD->m_dwBuiltinCMD) == 8);
                ;{
                    const BYTE* src_it = reinterpret_cast<BYTE*>(&dwRndKey);
                    const BYTE* const src_end = reinterpret_cast<BYTE*>(&dwRndKey) + sizeof(dwRndKey);
                    WORD* dest_it = reinterpret_cast<WORD*>(pMsgCMD->m_dwBuiltinCMD);
                    for (; src_it != src_end; ++src_it, ++dest_it) {
                        *dest_it = *src_it;
                    };
                };
				MOLA_QUERY_MSG molaMSG( dwKEY, eSECURE_QUERY::BUILTIN_QUERY, 0, 0, (BYTE*)pMsgCMD, sizeof(*pMsgCMD) );
				
				return rISQP.RegisterQueriedMsg( &molaMSG );
			}
			break;
		//case eSECURE_QUERY::MAX_SECURE_PACKET:
		case eSECURE_QUERY::FIND_INFO_BY_WORK_KEY:
		case eSECURE_QUERY::DEL_INFO_BY_WORK_KEY:
		case eSECURE_QUERY::POP_INFO_BY_WORK_KEY:
			{
				DWORD dwWorkKey = pARG->m_dwWorkKey;
				QUERIED_LIST_IT itFIND = rSQP.FindMolaQueriedMsg( rLIST, dwWorkKey );
				if( itFIND != rLIST.end() )
				{
					MOLA_QUERY_MSG& rMSG = *itFIND;
					if( eSECURE_QUERY::DEL_INFO_BY_WORK_KEY != eSQT )
						CopyMemory( pARG, &rMSG, sizeof(MOLA_QUERY_MSG) );
					if( eSECURE_QUERY::FIND_INFO_BY_WORK_KEY != eSQT )
						rLIST.erase( itFIND );
					if( eSECURE_QUERY::DEL_INFO_BY_WORK_KEY == eSQT )
						return TRUE;

					return dwWorkKey;
				}

				return FALSE;
			}
			break;
		case eSECURE_QUERY::DEL_USER:
			{
				rSQP.DeleteUser( dwKEY );
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}


DWORD		ISecureWorkNode::OnRecvPacket( MOLA_QUERY_MSG* IN OUT pARG )
{
	__DEBUGGING_OUTPUT0( __FUNCTION__ );

#	define ADD_HANDLER_INFO_MOLA( c, p, accptablePHASE )	const WORD c##p = MAKEWORD(c, p);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_MAP, CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK, PHASE_UNLIMITED)
	ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN, PHASE_UNLIMITED)

#	undef ADD_HANDLER_INFO_MOLA

	if( !(pARG && pARG->m_pBackupMsg) )
		return FALSE;

	MSG_BASE* pBASE = (MSG_BASE*)pARG->m_pBackupMsg;
	WORD wPROTOCOL = MAKEWORD(pBASE->m_byCategory, pBASE->m_byProtocol);
	switch( wPROTOCOL )
	{
#	define ADD_HANDLER_INFO_MOLA( c, p, accptablePHASE )	case c##p:

		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
		ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_ACK, PHASE_UNLIMITED)
		{
			__DEBUGGING_OUTPUT0( "CG_SECURE_QUERY_BUILTIN_CHK_ACK" );
			DWORD dwSuccess = FALSE;
			DWORD dwBuiltinWorkKey = pARG->m_dwWorkKeyB;
			if( dwBuiltinWorkKey )
			{
				MOLA_QUERY_MSG rARG( pARG->m_dwUserKey, eSECURE_QUERY::POP_INFO_BY_WORK_KEY, dwBuiltinWorkKey );
				DWORD dwRet = QueryProcess( &rARG );
				if( dwRet == dwBuiltinWorkKey )
				{
					MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_CMD* pSRC = (MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_CMD*)rARG.m_pBackupMsg;
					MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_ACK* pMSG = (MSGINTER_CG_SECURE_QUERY_BUILTIN_CHK_ACK*)pARG->m_pBackupMsg;
					if( rARG.m_pBackupMsg ) // chunked memory
					{
						if( 0 == memcmp( pSRC->m_dwBuiltinCMD, pMSG->m_dwBuiltinACK, sizeof(pSRC->m_dwBuiltinCMD) ) )
							dwSuccess = 2;
						MolaTAllocFree(rARG.m_pBackupMsg);
					}
				}
			}
			return dwSuccess;
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_MAP, CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK, PHASE_UNLIMITED)
		{
		}
		break;
		ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN, PHASE_UNLIMITED)
		{
		}
		break;
#	undef ADD_HANDLER_INFO_MOLA
	} // end switch

	return FALSE;
}



// SERVER
PVOID	ISecureWorkNode::MakePacketForClient( const BYTE byCate, const BYTE byProto, BYTE*& pSendingMsg, WORD& wSendingSize )
{
	__DEBUGGING_OUTPUT0( "ISecureWorkNode::MakePacketForClient" );
#if CODE_BACKUP
	static DWORD dwRndKey = MAKE_SNK_RND_NEXT_KEY( MAKEDWORD(byCate, byProto) );
	WORD wCmd = MAKEWORD(byCate, byProto);
	switch( wCmd )
	{
	case MAKEWORD(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD):
		{
			MSGINTER_CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD* pMsgCMD =
				new (s_ISecureWorkNodeStream->m_bySendPacketStream) MSGINTER_CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD;

			pMsgCMD->m_BASE.bySecureType = SECURE_QUERY_SIMPLEX_PATTERN02_SNK;
			dwRndKey = MAKE_SNK_RND_NEXT_KEY( dwRndKey );
			pMsgCMD->m_TOKEN.uDWORD = MAKE_SNK_RND_NEXT_KEY( dwRndKey );

			pSendingMsg = (BYTE*)pMsgCMD;
			wSendingSize = (WORD)sizeof(*pMsgCMD);
			return &pMsgCMD->m_TOKEN;
		}
		break;
	};
#endif //CODE_BACKUP

	return NULL;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<QUERY_MOLA_CMD DECLARATION>
//
//
#pragma pack(push, 1)

#define BUILTIN_CMD_VAR(name)	sISecureBC_##name
#define BUILTIN_CMD_TYPE(name)	name##_T
#define BUILTIN_CMD_DECL(name)														\
	typedef ISecureQueryCmdStreamBuiltinCmd<name, 0>	BUILTIN_CMD_TYPE(name);		\
	static BUILTIN_CMD_TYPE(name)	BUILTIN_CMD_VAR(name);

//	QUERY_MOLA_CMD LIST
//	<BLOCK START>
BUILTIN_CMD_DECL(sBUILTIN_001_A_PLUS_B_RET)
BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_RET)
BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_PLUS_D_RET)
//	<BLOCK END>

#pragma pack(pop)
//
//	<QUERY_MOLA_CMD DECLARATION>
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<QUERY_MOLA_CMD REGISTER>
//
MODULE_HANDLER_IMPL(REGISTER_QUERY_MOLA_CMD)
{
#	undef BUILTIN_CMD_DECL
#	define BUILTIN_CMD_DECL(name)			{ wTypeKey, wIndex++, &BUILTIN_CMD_VAR(name) },

#	define BUILTIN_CMD_REGISTER_HEADER(size)	\
	do {										\
		QUERY_VECTOR& rLIST = rMAP[wTypeKey];	\
		sSERVER_QUERY_LINKER rITEMS[size] = {

#	define BUILTIN_CMD_REGISTER_TAIL(size)		\
		};										\
		for( INT i=0 ; i<size ; ++i )			\
			rLIST.push_back( rITEMS[i] );		\
	} while(0)


	ISecureQueryCmdStreamBuiltinCmd<sBUILTIN_001_A_PLUS_B_RET, 0>	cmd;

	//WzASM_VMCore& rCORE = VMCORE_INTERFACE;
	ISecureQueryProcessor& rISQP = VM_SQP;
	QUERY_INFO_MAP& rMAP = rISQP.QueryTable();

	WORD wTypeKey = eSECURE_QUERY::QUERY_MOLA;
	WORD wIndex = 0;

#if CODE_BACKUP
	QUERY_VECTOR& rLIST = rMAP[wIndex];
	sSERVER_QUERY_LINKER rITEMS[3] =
	{
		//	BUILTIN CMD STREAM LIST
		//	<BLOCK START>
		BUILTIN_CMD_DECL(sBUILTIN_001_A_PLUS_B_RET)
		BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_RET)
		BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_PLUS_D_RET)
		//	<BLOCK END>
	};
	for( INT i=0 ; i<3 ; ++i )
		rLIST.push_back( rITEMS[i] );
#endif //CODE_BACKUP

	//////////////////////////////////////////////////////////////////////////
	//
	wTypeKey = eSECURE_QUERY::QUERY_MOLA;
	wIndex = 0;

	BUILTIN_CMD_REGISTER_HEADER(3)
		//	BUILTIN CMD STREAM LIST
		//	<BLOCK START>
		BUILTIN_CMD_DECL(sBUILTIN_001_A_PLUS_B_RET)
		BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_RET)
		BUILTIN_CMD_DECL(sBUILTIN_002_A_PLUS_B_PLUS_C_PLUS_D_RET)
		//	<BLOCK END>
	BUILTIN_CMD_REGISTER_TAIL(3);





	return TRUE;
}
//	<QUERY_MOLA_CMD DECLARATION>
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫



#endif //SERVER_CODE











#if CODE_TEST(__MOLA_IwASM_CMD_TEST__)


MODULE_HANDLER_IMPL(UNIT_TEST)
{
	uREG_NODE uARG;

	do { //sBUILTIN_001_A_PLUS_B_RET
		sBUILTIN_001_A_PLUS_B_RET sBBB;

		SET_PASSING_ARG( uARG, 10, 200, 0, 0 );
		sBBB.SetArgument( &uARG );
		sRETURN_PTR_SZ sRET = sBBB.GetResult();

		WzASM_EIP sEIP;
		sEIP.Init( (BYTE*)&sBBB, sizeof(sBBB) );
		uREG_NODE uRET = sEIP.Execute();

		if( uRET.EX != sRET.RET_EX )
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
			__asm { int 3 };
		}
		else
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
		}
	} while(0);

	do { //sBUILTIN_001_A_PLUS_B_RET
		sBUILTIN_002_A_PLUS_B_PLUS_C_RET sBBB;

		SET_PASSING_ARG( uARG, 10, 200, 3000, 0 );
		sBBB.SetArgument( &uARG );
		sRETURN_PTR_SZ sRET = sBBB.GetResult();

		WzASM_EIP sEIP;
#if CODE_BACKUP
		sBUILTIN_002_A_PLUS_B_PLUS_C_RET* pInstance = sEIP.InitInstance<sBUILTIN_002_A_PLUS_B_PLUS_C_RET>();
		pInstance->SetArgument( (PARGB_T)dwARG );
#endif
		sEIP.Init( (BYTE*)&sBBB, sizeof(sBBB) );
		uREG_NODE uRET = sEIP.Execute();

		if( uRET.EX != sRET.RET_EX )
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
			__asm { int 3 };
		}
		else
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
		}
	} while(0);



	do { //sBUILTIN_001_A_PLUS_B_RET
		sBUILTIN_002_A_PLUS_B_PLUS_C_PLUS_D_RET sBBB;

		SET_PASSING_ARG( uARG, 10, 200, 3000, 40000 );
		sBBB.SetArgument( &uARG );
		sRETURN_PTR_SZ sRET = sBBB.GetResult();

		WzASM_EIP sEIP;
		sEIP.Init( (BYTE*)&sBBB, sizeof(sBBB) );
		uREG_NODE uRET = sEIP.Execute();

		if( uRET.EX != sRET.RET_EX )
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
			__asm { int 3 };
		}
		else
		{
			printf( "UNIT TEST SUCESS {%u==%u}\n", sRET.RET_EX, uRET.EX );
		}
	} while(0);

	//printf( "call exit\n" );
	//exit(0);

	return TRUE;
}

#endif //CODE_TEST(__MOLA_IwASM_CMD_TEST__)



