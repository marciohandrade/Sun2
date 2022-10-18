#include "stdafx.h"

#include "BBTanStream.h"

#define BBTAN_TWINKLE_EXPORTER(name)	\
	BYTE* pSTK_FRM = (BYTE*)(name)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// <TYPE02>
//////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4307)

#define CALC_GENDER_NO1(convNo1, convNo2)	\
	(					\
	(convNo1)			\
	^(GETRNDNO_RNG08_T3(convNo2)<<GETRNDNO_RNG04_T5(convNo1))	\
	^((GETRNDNO_RNG16_T5(convNo1)<<1)|(GETRNDNO_RNG32_T1(convNo2)))		\
	^(GETRNDNO_RNG128_T3(convNo1)<<1)	\
	^(GETRNDNO_RNG128_T2(convNo2))		\
	)

#define CALC_GENDER_NO2(convNo1, convNo2)	\
	(					\
	(convNo2)			\
	^(GETRNDNO_RNG08_T5(convNo2)<<GETRNDNO_RNG04_T2(convNo1))	\
	^((GETRNDNO_RNG16_T2(convNo2)<<1)|(GETRNDNO_RNG32_T2(convNo1)))		\
	^(GETRNDNO_RNG128_T3(convNo2)<<1)	\
	^(GETRNDNO_RNG128_T5(convNo1))		\
	)

#include "FI_VAR_FUNC_COLLECTOR.def"

// <DEFINE>
#define BBTAN_RECURSIVE_HANDLER_NAME( no )					\
	BBTAN_RECURSIVE_HANDLER_##no

#define BBTAN_RECURSIVE_HANDLER_NAME2( no )					\
	BBTAN_RECURSIVE_HANDLER2_##no

// <DEFINE>
#define BBTAN_RECURSIVE_HANDLER_DECL( no )					\
	static BOOL	BBTAN_RECURSIVE_HANDLER_##no( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wRemain, BYTE& rbyLinkedNo )

#define BBTAN_RECURSIVE_HANDLER_DECL2( no )					\
	static BOOL	BBTAN_RECURSIVE_HANDLER2_##no( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wRemain, BYTE& rbyLinkedNo )


// <DEFINE>
#define BBTAN_CHECK_ACTION_HANDLER_CALL( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	INLINE_CHECK_ACTION_FUNCTION_IMPL_##No1st##_##No2nd( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )

#define BBTAN_CHECK_ACTION_HANDLER_CALL2( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	INLINE_CHECK_ACTION_FUNCTION_IMPL_##No3rd##_##No4th( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )

// <DEFINE>
#define BBTAN_RECURSIVE_HANDLER_CALL( no, wSize, ref )					\
	BBTAN_RECURSIVE_HANDLER_##no( rKEY, pBYTE, wSize, ref )

#define BBTAN_RECURSIVE_HANDLER_CALL2( no, wSize, ref )					\
	BBTAN_RECURSIVE_HANDLER2_##no( rKEY, pBYTE, wSize, ref )

// <DEFINE>
#define BBTAN_RECURSIVE_HANDLER( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )			\
	BBTAN_RECURSIVE_HANDLER_DECL(No1st)	{																	\
		BBTAN_TWINKLE_EXPORTER(&BBTAN_RECURSIVE_HANDLER_NAME(No1st));CALLSTACK_TRACING();										\
		if( PreCalculator( pBYTE, wRemain ) )	return TRUE;												\
		BBTAN_CHECK_ACTION_HANDLER_CALL(No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4);	\
		PostCalculator( pBYTE, wRemain, rbyLinkedNo );														\
		BYTE* mpBYTE = pBYTE; WORD mwRemain = wRemain; BYTE mrbyLinkedNo = rbyLinkedNo;						\
		BOOL bSUCCESS = TRUE;																				\
		if( rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] > rKEY.RNDBLOCK[ARRAY4_RNDIDX(No4th)] )	{					\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No2nd)] += convNo2; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo4;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL(No2nd, wRemain, rbyLinkedNo);						\
		} else {rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo3; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] += convNo1;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL2(No2nd, wRemain, rbyLinkedNo);	}					\
		if( bSUCCESS )	{																					\
			pBYTE = mpBYTE; wRemain = mwRemain>>2; rbyLinkedNo ^= mrbyLinkedNo;								\
			if( rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] > rKEY.RNDBLOCK[ARRAY4_RNDIDX(No2nd)] ) {				\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] += convNo1; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No4th)] += convNo2;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL2(No3rd, wRemain, rbyLinkedNo);						\
			} else {																						\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No2nd)] += convNo3; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo2;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL2(No4th, wRemain, rbyLinkedNo);						\
			}	}					\
		return bSUCCESS;	\
	}

#define BBTAN_RECURSIVE_HANDLER2( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )			\
	BBTAN_RECURSIVE_HANDLER_DECL2(No3rd)	{																\
		BBTAN_TWINKLE_EXPORTER(&BBTAN_RECURSIVE_HANDLER_NAME2(No3rd));CALLSTACK_TRACING();										\
		if( PreCalculator( pBYTE, wRemain ) )	return TRUE;												\
		BBTAN_CHECK_ACTION_HANDLER_CALL2(No3rd, No4th, No1st, No2nd, convNo1, convNo2, convNo3, convNo4);	\
		PostCalculator( pBYTE, wRemain, rbyLinkedNo );														\
		BYTE* mpBYTE = pBYTE; WORD mwRemain = wRemain; BYTE mrbyLinkedNo = rbyLinkedNo;						\
		BOOL bSUCCESS = TRUE;																				\
		if( rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] > rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] )	{					\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No2nd)] += convNo1; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] += convNo3;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL2(No4th, wRemain, rbyLinkedNo);						\
		} else {rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo4; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No4th)] += convNo2;	\
			bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL(No4th, wRemain, rbyLinkedNo);	}						\
		if( bSUCCESS )	{																					\
			pBYTE = mpBYTE; wRemain = mwRemain>>2; rbyLinkedNo ^= mrbyLinkedNo;								\
			if( rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] > rKEY.RNDBLOCK[ARRAY4_RNDIDX(No4th)] ) {				\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No1st)] += convNo4; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo1;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL(No4th, wRemain, rbyLinkedNo);						\
			} else {																						\
				rKEY.RNDBLOCK[ARRAY4_RNDIDX(No3rd)] += convNo2; rKEY.RNDBLOCK[ARRAY4_RNDIDX(No4th)] += convNo3;	\
				bSUCCESS = BBTAN_RECURSIVE_HANDLER_CALL(No3rd, wRemain, rbyLinkedNo);						\
			}	}				\
		return bSUCCESS;	\
	}

//		BBTAN_CHECK_ACTION_HANDLER_CALL(No2nd, No1st, convNo1, convNo2);							\
//////////////////////////////////////////////////////////////////////////

// <PROTOTYPEs>
#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )		\
	BBTAN_RECURSIVE_HANDLER_DECL(No1st);

#include BBTAN_STREAM_TYPE2NAME

#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )		\
	BBTAN_RECURSIVE_HANDLER_DECL2(No1st);

#include BBTAN_STREAM_TYPE2NAME

// <IMPLEMENTATION>
#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	BBTAN_RECURSIVE_HANDLER(No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4);

#include BBTAN_STREAM_TYPE2NAME

#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	BBTAN_RECURSIVE_HANDLER2(No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4);

#include BBTAN_STREAM_TYPE2NAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#undef BBTAN_RECURSIVE_HANDLER_NAME
#undef BBTAN_RECURSIVE_HANDLER_DECL
#undef BBTAN_RECURSIVE_HANDLER_NAME2
#undef BBTAN_RECURSIVE_HANDLER_DECL2

//#undef BBTAN_RECURSIVE_HANDLER_CALL
#undef BBTAN_RECURSIVE_HANDLER
#undef CALC_GENDER_NO1
#undef CALC_GENDER_NO2

#define CALC_GENDER_NO1(convNo1, convNo2)	\
	(					\
	(convNo1)			\
	^(GETRNDNO_RNG04_T2(convNo1)<<GETRNDNO_RNG04_T1(convNo2))	\
	^((GETRNDNO_RNG16_T3(convNo2)<<2)|(GETRNDNO_RNG32_T4(convNo2)))		\
	^(GETRNDNO_RNG128_T3(convNo1)<<1)	\
	^(GETRNDNO_RNG128_T1(convNo2))		\
	)

#define CALC_GENDER_NO2(convNo1, convNo2)	\
	(					\
	(convNo2)			\
	^(GETRNDNO_RNG04_T4(convNo2)<<GETRNDNO_RNG04_T1(convNo1))	\
	^((GETRNDNO_RNG16_T2(convNo2)<<1)|(GETRNDNO_RNG32_T1(convNo1)))		\
	^(GETRNDNO_RNG128_T2(convNo2)<<1)	\
	^(GETRNDNO_RNG128_T2(convNo1))		\
	)


#define BBTAN_RECURSIVE_HANDLER_NAME( no )							\
	BBTAN_RECURSIVE_HANDLER_ROOT_##no

// <DEFINE>
#define BBTAN_RECURSIVE_HANDLER_DECL( no )					\
	static BOOL	BBTAN_RECURSIVE_HANDLER_ROOT_##no( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wRemain, BYTE& rbyLinkedNo )

//#define BBTAN_RECURSIVE_HANDLER_CALL( no, ref )					\
//	BBTAN_RECURSIVE_HANDLER_ROOT_##no( pBYTE, wRemain, ref )
#define BBTAN_RECURSIVE_HANDLER( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )					\
	BBTAN_RECURSIVE_HANDLER_DECL(No1st)									{					\
		BBTAN_TWINKLE_EXPORTER(&BBTAN_RECURSIVE_HANDLER_NAME(No1st));						\
		WORD wMSG_BASE_RND = wRemain;														\
		if( wMSG_BASE_RND > 8 )																\
			wMSG_BASE_RND = (7&rKEY.RNDBLOCK[0]) + RANGE_RND_000_004(wRemain);				\
		if( PreCalculator( pBYTE, wMSG_BASE_RND ) )	return TRUE;							\
		BBTAN_CHECK_ACTION_HANDLER_CALL(No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4);				\
		PostCalculator( pBYTE, wMSG_BASE_RND, rbyLinkedNo );							\
		return BBTAN_RECURSIVE_HANDLER_CALL(No2nd, wMSG_BASE_RND, rbyLinkedNo);	}


#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )		\
	BBTAN_RECURSIVE_HANDLER_DECL(No1st);

#include BBTAN_STREAM_TYPE2NAME

#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	BBTAN_RECURSIVE_HANDLER(No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4);

#include BBTAN_STREAM_TYPE2NAME

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VOID	BBTanStream::Init_T2()
{
	ConvertBBTanHandler	stk_Handlers[256] =
	{
#undef BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE
#define BBTAN_RECURSIVE_HANDLER_DEFINE_REFINE( No1st, No2nd, No3rd, No4th, convNo1, convNo2, convNo3, convNo4 )	\
	BBTAN_RECURSIVE_HANDLER_NAME(No1st),

		#include BBTAN_STREAM_TYPE2NAME
	};

#if SERVER_CODE
	__DEBUGGING_OUTPUT0( "CryptManager::Init_T2-server" );
#else
	__DEBUGGING_OUTPUT0( "CryptManager::Init_T2-client" );
#endif

	ConvertBBTanHandler* pOff = stk_Handlers;
	for( DWORD dwIdx=0 ; dwIdx<256 ; ++dwIdx, ++pOff )
	{
#if SERVER_CODE
		m_Receiver.insert( m_Receiver.end(), make_pair( (BYTE)dwIdx, *pOff ) );
#else //CLIENT_CODE
		m_Sender.insert( m_Sender.end(), make_pair( (BYTE)dwIdx, *pOff ) );
#endif //
	}
}
