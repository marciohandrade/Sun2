//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
#ifndef CONV_CODE
#	define CONV_CODE()					\
	( RGPG014 | RGPG044<<8 )
#	define CONV_HASH_CODEVALUE(val)		\
	( CONV_CODE() ^ (val) )
#endif //



#pragma pack(push, 1)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//
template<typename OPW_T _HASH, typename BYTE _SZ>
struct sIL_ASM_INFO_NODE
{
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX1_B2)>		M01;
	sDT2<_HASH>											M02;
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX2_B1)>		M03;
	sDT1<_SZ>											M04;
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX3_PTR)>		M05;
	sDTP<NULL>										M06;
	sIL<CONV_HASH_CODEVALUE(IWzASM::PUSH_EAX)>			M07;
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EFP_EAX)>		M08;
	sIL<CONV_HASH_CODEVALUE(IWzASM::CALL_IBF_EFP)>		M09;

	inline VOID			ChangePTR( IWzASM_Handler fnPTR )
	{	M06.Change( (WzPTR)fnPTR );	}
};

//
//////////////////////////////////////////////////////////////////////////
//

struct sMETHOD_REGISTER_HANDLER
{
	//sIL<IWzASM::PUSH_EABCDX>				M02;
	//sIL_MOV_EBX_PTR<NULL>					M03_RegisterASMHandler;
	//sIL<IWzASM::MOV_EAX0_EBX0>				M04;
	sIL<CONV_HASH_CODEVALUE(IWzASM::NOP)>				M02;
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX0_PTR)>		M03;
	sDTP<NULL>										M04;
	sIL<CONV_HASH_CODEVALUE(IWzASM::PUSH_EAX)>			M05;
	sIL<CONV_HASH_CODEVALUE(IWzASM::CALL_IBF_EFP)>		M06;	//RegisterHandler Return
	sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX0_ERP0)>		M07;

	//////////////////////////////////////////////////////////////////////////

#undef ADD_IWzASM_HANDLER
#	define ADD_IWzASM_HANDLER( codeASM, dataSIZE )		\
	sIL_ASM_INFO_NODE<IWzASM::codeASM,	dataSIZE>	M_##codeASM;

#		include "IWzASM.DefList.h"

	struct sINTER_register_JAT_matching_table
	{
		sIL<CONV_HASH_CODEVALUE(IWzASM::MOV_EAX0_PTR)>		M00_0_RegisterHandler;
		sDTP<NULL>										M00_1_HandlerPTR;
		sIL_ASM_INFO_NODE<IWzASM::MARK01, OPT::MARK01>	M01;
		sIL_ASM_INFO_NODE<IWzASM::MARK02, OPT::MARK02>	M02;
		sIL_ASM_INFO_NODE<IWzASM::MARK03, OPT::MARK03>	M03;
		sIL_ASM_INFO_NODE<IWzASM::MARK04, OPT::MARK04>	M04;
		sIL_ASM_INFO_NODE<IWzASM::MARK05, OPT::MARK05>	M05;
		sIL_ASM_INFO_NODE<IWzASM::MARK06, OPT::MARK06>	M06;
		sIL_ASM_INFO_NODE<IWzASM::MARK07, OPT::MARK07>	M07;
		sIL_ASM_INFO_NODE<IWzASM::MARK08, OPT::MARK08>	M08;
		sIL_ASM_INFO_NODE<IWzASM::MARK09, OPT::MARK09>	M09;
		sIL_ASM_INFO_NODE<IWzASM::MARK10, OPT::MARK10>	M10;
		sIL_ASM_INFO_NODE<IWzASM::MARK11, OPT::MARK11>	M11;
		sIL_ASM_INFO_NODE<IWzASM::MARK12, OPT::MARK12>	M12;
		sIL_ASM_INFO_NODE<IWzASM::MARK13, OPT::MARK13>	M13;
		sIL_ASM_INFO_NODE<IWzASM::MARK14, OPT::MARK14>	M14;
		sIL_ASM_INFO_NODE<IWzASM::MARK15, OPT::MARK15>	M15;
		sIL_ASM_INFO_NODE<IWzASM::MARK16, OPT::MARK16>	M16;
		sIL_ASM_INFO_NODE<IWzASM::MARK17, OPT::MARK17>	M17;
		sIL_ASM_INFO_NODE<IWzASM::MARK18, OPT::MARK18>	M18;
		sIL_ASM_INFO_NODE<IWzASM::MARK19, OPT::MARK19>	M19;
	}													M08_register_JAT_matching_table;
	sIL_RET												M09;
	//////////////////////////////////////////////////////////////////////////

	VOID	RegisterHandler4Handlers( WzASM_EIP* pEIP )
	{
		M04.Change( pEIP );
	}

	VOID	RegisterHandlers()
	{
		//M03_RegisterASMHandler.Change( (WzPTR)RegisterASMHandler );

#	undef ADD_IWzASM_HANDLER
#	define ADD_IWzASM_HANDLER( codeASM, dataSIZE )		\
	M_##codeASM . ChangePTR( (HANDLER_WzASM_##codeASM) );

#		include "IWzASM.DefList.h"

		M08_register_JAT_matching_table.M00_1_HandlerPTR.Change( RegisterJAT );

#ifndef _DEBUG
	}
#else //_DEBUG
		//Output();
	}

	VOID	Output()
	{
		BYTE* pOFF = (BYTE*)&M02;
		pOFF += sizeof(M02);
		for( DWORD i=0 ; i<sizeof(*this) ; ++i, ++pOFF )
		{
			if( i % 13 == 0 )
				puts("");
			printf( "[%02X]", *pOFF );
		}
		puts("");
	}
#endif //

#if CODE_BACKUP
	VOID	CommanStreamInfo( BYTE*& rpSTREAM, WORD& rwSIZE )
	{
#ifdef _DEBUG
		printf( "-ASM REGISTER BLOCK SZ %u\n", sizeof(*this) );
		printf( "-ASM REGISTER CMD BLOCK SZ %u\n", sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M10_ENDMARK)) );
#endif //
		rpSTREAM = (BYTE*)&M02;
		rwSIZE = sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M10_ENDMARK));
	}
#endif
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//
struct sMETHOD_IBF_HANDLERS_REGISTER_CMD
{
	sIL<IWzASM::PUSH_EABCDX>				M02;
	sIL_MOV_EBX_PTR<NULL>					M03_RegisterIBFHandler;
	sIL<IWzASM::MOV_EAX0_EBX0>				M04;

	//////////////////////////////////////////////////////////////////////////
#	pragma warning(push)
#	pragma warning(disable:4307)

#	undef DECL_IBF_HASH
#	define DECL_IBF_HASH(name, val, val2)								\
	sIL<IWzASM::MOV_EAX1_B2>						M##val##val2##_01;	\
	sDT2< IBF_HASH_VALUE(name, val, val2) >			M##val##val2##_02;	\
	sIL<IWzASM::MOV_EAX2_PTR>						M##val##val2##_03;	\
	sDTP<NULL>									M##val##val2##_04;	\
	sIL<IWzASM::PUSH_EAX>							M##val##val2##_05;	\
	sIL<IWzASM::MOV_EFP_EAX>						M##val##val2##_06;	\
	sIL<IWzASM::CALL_IBF_EFP>						M##val##val2##_07;

#	include "IWzASM.IBFList.h"

#	pragma warning(pop)

	sIL<IWzASM::POP_EABCDX>					M05;
	sIL_RET									M06;
	//////////////////////////////////////////////////////////////////////////

	VOID	ChangeRegisterHandler( WzPTR fnPTR )
	{	M03_RegisterIBFHandler.ChangeVAL( fnPTR );	}

	VOID	RegisterHandlers()
	{
		//		set<WORD>	findKEY;
		//		set<WORD>::iterator it;
		//#	undef DECL_IBF_HASH
		//#	define DECL_IBF_HASH(name, tag, val)					\
		//	M##val##_04 . Change( (HANDLER_WzASM_IBF_##name) );							\
		//	it = findKEY.find( M##val##_02.VALUE );										\
		//	if( it != findKEY.end() )		printf( "\nDUPLICATED KEY %02X\n", *it );	\
		//	else							findKEY.insert( M##val##_02.VALUE );

#	undef DECL_IBF_HASH
#	define DECL_IBF_HASH(name, val, val2)					\
	M##val##val2##_04 . Change( (HANDLER_WzASM_IBF_##name) );

#	include "IWzASM.IBFList.h"

#ifndef _DEBUG
	}
#else //_DEBUG
		//Output();
	}

	VOID	Output()
	{
		BYTE* pOFF = (BYTE*)this;
		for( DWORD i=0 ; i<sizeof(*this) ; ++i, ++pOFF )
		{
			printf( "[%02X]", *pOFF );
		}
		puts("");
	}
#endif //

#if CODE_BACKUP
	VOID	CommanStreamInfo( BYTE*& rpSTREAM, WORD& rwSIZE )
	{
#ifdef _DEBUG
		printf( "-IBF REGISTER BLOCK SZ %u\n", sizeof(*this) );
		printf( "-IBF REGISTER CMD BLOCK SZ %u\n", sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M07_ENDMARK)) );
#endif //
		rpSTREAM = (BYTE*)&M02;
		rwSIZE = sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M07_ENDMARK));
	}
#endif
};



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
//
template<typename WzASMH _HASH>
struct sIL_WzEIP_GRES_HANDLERS_REGISTER_EACHNODE
{
	// EAX0 = _RegisterGResHandler
	sIL<IWzASM::MOV_EAX1_B2>				M01;
	sDT2<_HASH>								M02;
	sIL<IWzASM::MOV_EAX2_PTR>				M03;
	sDTP<NULL>							M04;
	sIL<IWzASM::PUSH_EAX>					M05;
	sIL_CALL_IBF_STK_EFP					M06;

	inline VOID		Change( WzPTR fnPTR )
	{	M04.Change( fnPTR );	}
};

struct sMETHOD_GRES_HANDLERS_REGISTER_CMD
{
	sIL<IWzASM::PUSH_EABCDX>				M02;
	sIL_MOV_EBX_PTR<NULL>					M03_RegisterGResHandler;
	sIL<IWzASM::MOV_EAX0_EBX0>				M04;

	//////////////////////////////////////////////////////////////////////////
#	pragma warning(push)
#	pragma warning(disable:4307)

#	undef DECL_GRES_HASH
#	define DECL_GRES_HASH(name, val, val2)						\
	sIL_WzEIP_GRES_HANDLERS_REGISTER_EACHNODE<GResHASH::name>	M##name;

#	include "IWzASM.ResHashList.h"

#undef DECL_GRES_HASH

//
//#	undef DECL_GRES_HASH
//#	define DECL_GRES_HASH(name, tag, val)							\
//	sIL<IWzASM::MOV_EAX1_B2>						M##val##_01;	\
//	sDT2< IBF_HASH_VALUE(name, tag, val) >			M##val##_02;	\
//	sIL<IWzASM::MOV_EAX2_PTR>						M##val##_03;	\
//	sDTP<NULL>									M##val##_04;	\
//	sIL<IWzASM::PUSH_EAX>							M##val##_05;	\
//	sIL<IWzASM::MOV_EFP_EAX>						M##val##_06;	\
//	sIL<IWzASM::CALL_IBF_EFP>						M##val##_07;
//
//#	include "IWzASM.ResHash.h"

#	pragma warning(pop)

	sIL<IWzASM::POP_EABCDX>					M05;
	sIL_RET									M06;
	//////////////////////////////////////////////////////////////////////////


	VOID	ChangeRegisterHandler( WzPTR fnPTR )
	{	M03_RegisterGResHandler.ChangeVAL( fnPTR );	}

	VOID	RegisterHandlers()
	{
		//		set<WORD>	findKEY;
		//		set<WORD>::iterator it;
		//#	undef DECL_IBF_HASH
		//#	define DECL_IBF_HASH(name, tag, val)					\
		//	M##val##_04 . Change( (HANDLER_WzASM_IBF_##name) );							\
		//	it = findKEY.find( M##val##_02.VALUE );										\
		//	if( it != findKEY.end() )		printf( "\nDUPLICATED KEY %02X\n", *it );	\
		//	else							findKEY.insert( M##val##_02.VALUE );

#	undef DECL_GRES_HASH
#	define DECL_GRES_HASH(name, val, val2)						\
	M##name.Change( CALL_IGRes##name );

#	include "IWzASM.ResHashList.h"

#	undef DECL_GRES_HASH

#ifndef _DEBUG
	}
#else //_DEBUG
		//Output();
	}

	VOID	Output()
	{
		BYTE* pOFF = (BYTE*)this;
		for( DWORD i=0 ; i<sizeof(*this) ; ++i, ++pOFF )
		{
			printf( "[%02X]", *pOFF );
		}
		puts("");
	}
#endif //

#if CODE_BACKUP
	VOID	CommanStreamInfo( BYTE*& rpSTREAM, WORD& rwSIZE )
	{
#ifdef _DEBUG
		printf( "-GRES REGISTER BLOCK SZ %u\n", sizeof(*this) );
		printf( "-GRES REGISTER CMD BLOCK SZ %u\n", sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M07_ENDMARK)) );
#endif //
		rpSTREAM = (BYTE*)&M02;
		rwSIZE = sizeof(*this) - (sizeof(M01_STRMARK)+sizeof(M07_ENDMARK));
	}
#endif
};

#pragma pack(pop)

