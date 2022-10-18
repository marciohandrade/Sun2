#include "stdafx.h"
#include "RandomSNKey.h"
//////////////////////////////////////////////////////////////////////////
#include "Macros/GenerateRandomBlock.h"
//#include "RandomSNKey.Gender.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// <MODULE VARIABLEs>
static int _cKeyCheck = 0xFF000000;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
RandomSNKey::RandomSNKey() {}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<RandomSNKey::Initialize>
//
VOID			RandomSNKey::Initialize( DWORD dwRndKey, BYTE majorVer, BYTE minorVer )
{
	majorVer += HEX_KEY_1_GENERATOR_008+1;
	minorVer += (HEX_KEY_1_GENERATOR_002*HEX_KEY_1_GENERATOR_004) + majorVer;

	DWORD dwTMP1 = SHA32_T0(dwRndKey)+SHA32_T3(majorVer)+DWORD_KEY_1_GENERATOR_001;
	DWORD dwTMP2 = SHA32_T3(dwTMP1)+SHA32_T0(minorVer)+DWORD_KEY_1_GENERATOR_002;
	EDW(m_dwRLK_A) = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_003, dwTMP1, dwTMP2, dwRndKey );
	EDW(m_dwRLK_B) = MAKE_RANDOM32_KEY_EXPOSED( dwTMP1, MOLA_RCG_T1_004, dwTMP2, EDW(m_dwRLK_A) );

	EDW(m_dwRLK_SOLAR1) = MAKE_RANDOM32_KEY_EXPOSED( EDW(m_dwRLK_A), EDW(m_dwRLK_B), dwTMP2, MOLA_RCG_T1_005 );
	EDW(m_dwRLK_LUNAR1) = MAKE_RANDOM32_KEY_EXPOSED( EDW(m_dwRLK_B), MOLA_RCG_T1_006, EDW(m_dwRLK_A), dwTMP1 );
	EDW(m_dwRLK_SOLAR2) = MAKE_RANDOM32_KEY_EXPOSED( EDW(m_dwRLK_B), (~EDW(m_dwRLK_SOLAR1)), EDW(m_dwRLK_SOLAR1), EDW(m_dwRLK_A) );
	EDW(m_dwRLK_LUNAR2) = MAKE_RANDOM32_KEY_EXPOSED( EDW(m_dwRLK_A), EDW(m_dwRLK_LUNAR1), (~EDW(m_dwRLK_LUNAR1)), EDW(m_dwRLK_B) );

	m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_BASE] = m_dwRLK_SOLAR2.pKEYS[eSNKEY_IDX::SOLAR_BASE];
	m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_PAIR] = m_dwRLK_SOLAR2.pKEYS[eSNKEY_IDX::SOLAR_PAIR];
	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_BASE];
	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_PAIR] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_PAIR];

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_RNDSNK__)
	printf( "%08X %08X %08X %08X\n", EDW(m_dwRLK_SOLAR1), EDW(m_dwRLK_SOLAR2), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );

	printf( "%02X %02X %02X %02X\n", m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_BASE], m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_PAIR],
		m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE], m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_PAIR] );
#endif 

#if CLIENT_CODE
	__DEBUGGING_OUTPUT2( "RandomSNKey::InitializeInfo Values", m_dwRLK_SOLAR1, m_dwRLK_LUNAR1 );
	__DEBUGGING_OUTPUT2( "RandomSNKey::InitializeInfo Incs", m_dwRLK_SOLAR2, m_dwRLK_LUNAR2 );
	__DEBUGGING_OUTPUT1( "RandomSNKey::InitializeInfo Keys", m_CURKEY );
#else //SERVER_CODE
#	if CODE_TEST(__MOLA_CLIENT_DEBUGGING_RNDSNK__)
	printf( "RandomSNKey::InitializeInfo Values %08X\t%08X\n", m_dwRLK_SOLAR1, m_dwRLK_LUNAR1 );
	printf( "RandomSNKey::InitializeInfo Incs %08X\t%08X\n", m_dwRLK_SOLAR2, m_dwRLK_LUNAR2 );
	printf( "RandomSNKey::InitializeInfo Keys %08X\n", m_bySolarBaseKey<<24 | m_bySolarPairKey<<16 | m_byLunarBaseKey<<8 | m_byLunarPairKey );
#	endif
#endif //
}

//////////////////////////////////////////////////////////////////////////
// <NOT USED>
VOID			RandomSNKey::KeyExchange( DWORD dwRndKey )
{
	m_dwRLK_A.dwKEY = dwRndKey;
	//(proto_Major<<24) | (proto_Minor<<16) | (proto_Devel<<8) | 0;
	DWORD tmp;
	tmp = m_dwRLK_SOLAR2.dwKEY;
	m_dwRLK_SOLAR2 = m_dwRLK_LUNAR2;
	m_dwRLK_LUNAR2.dwKEY = tmp;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
//






//////////////////////////////////////////////////////////////////////////
//	<ROUNDUP SolarKey>
//		- In Server, SendKey
//		- In Client, RecvKey
VOID RandomSNKey::NextRoundSNKey4Solar()
{
	sRoundUpChecker<ROUND_CHK_1st_NO, ROUND_CHK_2nd_NO, ROUND_CHK_3rd_NO, ROUND_CHK_4th_NO> Checker;

	uSNKEY uTMP = Checker.RoundUp00( MOLA_RCG_T1_002, MOLA_RCG_T1_003, EDW(m_dwRLK_B), EDW(m_dwRLK_SOLAR1) );

	if( Checker.CheckUp1st( EDW(m_dwRLK_SOLAR1) ) )
	{
		m_dwRLK_SOLAR2 = Checker.RoundUp01( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_SOLAR1), EDW(m_dwRLK_SOLAR2) );
		m_dwRLK_SOLAR1 = uTMP;

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_RNDSNK__)
		printf( "[S1] uTMP %08X %08X %08X\n", uTMP, m_dwRLK_SOLAR1, m_dwRLK_SOLAR2 );
#endif
	}

#if	(CATEGORY_ODD_NUMBER&SWV(7))
	else if( Checker.CheckUp2nd( EDW(m_dwRLK_SOLAR2) ) )
	{
		m_dwRLK_SOLAR1 = uTMP;
		m_dwRLK_SOLAR2 = Checker.RoundUp02( EDW(uTMP), EDW(m_dwRLK_SOLAR2), EDW(m_dwRLK_SOLAR1), EDW(m_dwRLK_B) );
#	if CODE_TEST(__MOLA_CLIENT_DEBUGGING_RNDSNK__)
		printf( "[S2] uTMP %08X %08X %08X\n", uTMP, m_dwRLK_SOLAR1, m_dwRLK_SOLAR2 );
#	endif
	}
#endif //

	else
	{
		m_dwRLK_SOLAR2 = Checker.RoundUp01( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_SOLAR1), EDW(m_dwRLK_SOLAR2) );
		m_dwRLK_SOLAR1 = uTMP;
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_RNDSNK__)
		printf( "[S3] uTMP %08X %08X %08X\n", uTMP, m_dwRLK_SOLAR1, m_dwRLK_SOLAR2 );
#endif
	}

	m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_BASE] = m_dwRLK_SOLAR2.pKEYS[eSNKEY_IDX::SOLAR_BASE];
	m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_PAIR] = m_dwRLK_SOLAR2.pKEYS[eSNKEY_IDX::SOLAR_PAIR];

	__DEBUGGING_OUTPUT1( "RandomSNKey::NextRoundSNKey4Solar", m_CURKEY.pKEYS[eSNKEY_IDX::SOLAR_BASE] );
}

//////////////////////////////////////////////////////////////////////////
//	<ROUNDUP LunarKey>
//		- In Server, RecvKey
//		- In Client, SendKey
VOID RandomSNKey::NextRoundSNKey4Lunar()
{
	sRoundUpChecker<ROUND_CHK_5th_NO, ROUND_CHK_6th_NO, ROUND_CHK_7th_NO, ROUND_CHK_8th_NO> Checker;

	uSNKEY uTMP = Checker.RoundUp04( MOLA_RCG_T1_002, MOLA_RCG_T1_003, EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1) );

	if( Checker.CheckUp4th( EDW(m_dwRLK_LUNAR1) ) )
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp05( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}
#if		(CATEGORY_ODD_NUMBER&SWV(7))
	else if( Checker.CheckUp3rd( EDW(m_dwRLK_LUNAR2) ) )
	{
		m_dwRLK_LUNAR1 = uTMP;
		m_dwRLK_LUNAR2 = Checker.RoundUp06( EDW(uTMP), EDW(m_dwRLK_LUNAR2), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_B) );
	}
#elif	(CATEGORY_ODD_NUMBER&SWV(4))
	else if( Checker.CheckUp4th( EDW(m_dwRLK_LUNAR1) ) )
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp06( EDW(m_dwRLK_B), EDW(uTMP), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}
#elif	(CATEGORY_ODD_NUMBER&SWV(5))
	else if( Checker.CheckUp3rd( EDW(uTMP) ) )
	{
		m_dwRLK_LUNAR1 = uTMP;
		m_dwRLK_LUNAR2 = Checker.RoundUp05( EDW(m_dwRLK_B), EDW(uTMP), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
	}
#endif
	else
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp03( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}

	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_BASE];
	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_PAIR] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_PAIR];

	__DEBUGGING_OUTPUT1( "RandomSNKey::NextRoundSNKey4Lunar", m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE] );
}

//////////////////////////////////////////////////////////////////////////
//
//
//  (__MOLA_VERSION__ >= 1010)
//
//////////////////////////////////////////////////////////////////////////

// 패킷 종류에 따라 처리

// Simplex-Communication Random Key In Client
VOID	RandomSNKey::KeyExchangerSimplex( DWORD dwRndKey )
{
	//	(WARNING)
	//		서버(Recv), 클라(Send) ... 즉 (LUNAR) 키만 건들어야 한다.
	//	(THINKING)
	//		- Patter

	uSNKEY uTMP;
	EDW(uTMP) = MAKE_RANDOM16_KEY_EXPOSED( EDW(m_dwRLK_B), MOLA_RCG_T1_007, EDW(m_CURKEY), EDW(m_dwRLK_A) );
}

//	Normal Convert - Simplex-Communication Random Key In Client/Server
//		Pattern01 : CMD->(CONV)->ACK->CONV
//		(CONV) : SRC KEY BLOCK만 변경한다.
VOID	RandomSNKey::KeyExchangerSimplexPattern01( DWORD dwRndKey )
{
	//	(WARNING)
	//		서버(Recv), 클라(Send) ... 즉 (LUNAR) 키만 건들어야 한다.

	uSNKEY uTMP, uTMP2;
	EDW(uTMP) = MAKE_RANDOM16_KEY_EXPOSED( EDW(m_dwRLK_LUNAR1), MOLA_RCG_T1_007, dwRndKey, EDW(m_dwRLK_LUNAR2) );
	EDW(uTMP2) = MAKE_RANDOM16_KEY_EXPOSED( EDW(uTMP), EDW(m_dwRLK_LUNAR2), EDW(m_dwRLK_LUNAR1), dwRndKey );
	EDW(uTMP) = MAKEDWORD( EDW(uTMP), EDW(uTMP2) );

	EDW(m_dwRLK_LUNAR1) = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_007, EDW(uTMP), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
	EDW(m_dwRLK_LUNAR2) = MAKE_RANDOM32_KEY_EXPOSED( EDW(m_dwRLK_LUNAR2), EDW(m_dwRLK_LUNAR1), dwRndKey, MOLA_RCG_T1_007 );
}

//	PreConvert - Simplex-Communication Random Key In Client/Server
//		Pattern02 : CMD->CONV->ACK
//		CONV : NextRoundKey하는 효과. Server에서는 길이 검사를 통해 해결한다.
VOID	RandomSNKey::KeyExchangerSimplexPattern02( DWORD dwRndKey )
{
	//	(WARNING)
	//		서버(Recv), 클라(Send) ... 즉 (LUNAR) 키만 건들어야 한다.

	sRoundUpChecker<ROUND_CHK_7th_NO, ROUND_CHK_2nd_NO, ROUND_CHK_4th_NO, ROUND_CHK_1st_NO> Checker;

	uSNKEY uTMP = Checker.RoundUp01( MOLA_RCG_T1_002, MOLA_RCG_T1_003, EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1) );

	if( Checker.CheckUp4th( EDW(m_dwRLK_LUNAR1) ) )
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp02( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}
#if		(CATEGORY_ODD_NUMBER&SWV(2))
	else if( Checker.CheckUp3rd( EDW(m_dwRLK_LUNAR2) ) )
	{
		m_dwRLK_LUNAR1 = uTMP;
		m_dwRLK_LUNAR2 = Checker.RoundUp05( EDW(uTMP), EDW(m_dwRLK_LUNAR2), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_B) );
	}
#elif	(CATEGORY_ODD_NUMBER&SWV(4))
	else if( Checker.CheckUp4th( EDW(m_dwRLK_LUNAR1) ) )
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp06( EDW(m_dwRLK_B), EDW(uTMP), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}
#elif	(CATEGORY_ODD_NUMBER&SWV(7))
	else if( Checker.CheckUp3rd( EDW(uTMP) ) )
	{
		m_dwRLK_LUNAR1 = uTMP;
		m_dwRLK_LUNAR2 = Checker.RoundUp05( EDW(m_dwRLK_B), EDW(uTMP), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
	}
#endif
	else
	{
		m_dwRLK_LUNAR2 = Checker.RoundUp03( EDW(uTMP), EDW(m_dwRLK_B), EDW(m_dwRLK_LUNAR1), EDW(m_dwRLK_LUNAR2) );
		m_dwRLK_LUNAR1 = uTMP;
	}

	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_BASE];
	m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_PAIR] = m_dwRLK_LUNAR2.pKEYS[eSNKEY_IDX::LUNAR_PAIR];

	__DEBUGGING_OUTPUT1( "RandomSNKey::NextRoundSNKey4Lunar", m_CURKEY.pKEYS[eSNKEY_IDX::LUNAR_BASE] );
}

//	PreConvert - Simplex-Communication Random Key In Client/Server
//		Pattern03 : CMD->CONV
//
VOID	RandomSNKey::KeyExchangerSimplexPattern03( DWORD dwRndKey )
{
	//	(WARNING)
	//		서버(Recv), 클라(Send) ... 즉 (LUNAR) 키만 건들어야 한다.
}

// similar to Initialize
VOID	RandomSNKey::KeyExchangerDuplex( DWORD dwKey )
{
}

//  (__MOLA_VERSION__ >= 1010)
//
//////////////////////////////////////////////////////////////////////////


