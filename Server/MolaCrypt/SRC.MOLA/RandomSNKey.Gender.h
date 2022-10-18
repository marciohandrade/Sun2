// RandomSNKey.Gender.h
#pragma once

//==================================================================================================
#pragma warning(push)
#pragma warning(disable: 4293)
//==================================================================================================
//
#pragma pack(push, 1)
union Fcode {
	DWORD	dwVal;
	float	fVal;
	struct {
		WORD	wVal;
		BYTE	byDummy1;
		BYTE	byDummy2;
	};
};
#pragma pack(pop)

// 위장 코드 1
inline DWORD GenderDummyCodeKey1( DWORD& rKey1, DWORD& rKey2, DWORD& rKey3 )
{
	Fcode fcode;
	fcode.dwVal = rKey1 ^ rKey2;
	fcode.fVal = ((float)fcode.dwVal/7.f);

	rKey1 = rKey2 ^ fcode.dwVal;

	return rKey1;
}

// 위장 코드 2
inline DWORD GenderDummyCodeKey2( DWORD& rKey1, DWORD& rKey2, DWORD& rKey3 )
{
	rKey1 = GenderDummyCodeKey1( rKey1, rKey2, rKey3 );

	rKey2 = MAKE_SNK_RND_NEXT_KEY_BY_BASE_PAIR(rKey1, rKey2, rKey3);
	rKey3 = rKey1 ^ rKey2;

	rKey3 = (BYTE)((rKey3 ^ rKey2)>>28);
	rKey2 = (BYTE)((rKey2 ^ rKey1)>>24);

	return rKey2;
}

inline VOID GenderNextRound_T1( DWORD& rKey )
{
#if		CODE_SELECTOR_BY_VER == 0
	rKey += 1;
#elif	CODE_SELECTOR_BY_VER == 1
	rKey ^= 0x40200001;
#elif	CODE_SELECTOR_BY_VER == 2
	rKey ^= 0x09180010;
#elif	CODE_SELECTOR_BY_VER == 3
#endif
}

inline VOID GenderNextRound_T2( DWORD& rKey )
{
#if		CODE_SELECTOR_BY_VER == 0
	rKey += 7;
#elif	CODE_SELECTOR_BY_VER == 1
	rKey ^= 0x00010F00;
#elif	CODE_SELECTOR_BY_VER == 2
	rKey ^= 0x000010F0;
#elif	CODE_SELECTOR_BY_VER == 3
#endif
}


inline DWORD GenderKeyBase01( DWORD dwRndKey, DWORD dwAddKey )
{
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
//
#define WV(val)			(val)
#define EDW(val)		(WV(val).dwKEY)
#define ELW(val)		(WV(val).pKEYW[eSNKEY_IDX::LOWORD_IDX])
#define EHW(val)		(WV(val).pKEYW[eSNKEY_IDX::HIWORD_IDX])
#define SWV(val)		(1<<WV(val))


#if		(CATEGORY_ODD_NUMBER&SWV(4))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(5))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(6))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(7))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(8))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(9))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(10))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(11))
#endif


template<typename DWORD CHK1st, typename DWORD CHK2nd, typename DWORD CHK3rd, typename DWORD CHK4th>
struct sRoundUpChecker
{
	inline BOOL		CheckUp1st( DWORD dwCMPVAL )
	{
		if( SWV(CHK1st) & dwCMPVAL )
			return TRUE;
		return FALSE;
	}

	inline BOOL		CheckUp2nd( DWORD dwCMPVAL )
	{
		if( SWV(CHK2nd) & dwCMPVAL )
			return TRUE;
		return FALSE;
	}

	inline BOOL		CheckUp3rd( DWORD dwCMPVAL )
	{
#if		(CATEGORY_ODD_NUMBER&SWV(4))
		if( SWV(CHK3rd) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(5))
		if( (SWV(CHK2nd)|SWV(CHK3rd)) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(6))
		if( !(SWV(CHK3rd) & dwCMPVAL) )
#elif	(CATEGORY_ODD_NUMBER&SWV(7))
		if( SWV(CHK2nd) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(8))
		if( !( (SWV(CHK2nd)|SWV(CHK4th)) & dwCMPVAL ) )
#elif	(CATEGORY_ODD_NUMBER&SWV(9))
		if( !(SWV(CHK4th) & dwCMPVAL) )
#elif	(CATEGORY_ODD_NUMBER&SWV(10))
		if( SWV(CHK3rd) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(11))
		if( 1 )
#endif
			return TRUE;

		return FALSE;
	}

	inline BOOL		CheckUp4th( DWORD dwCMPVAL )
	{
#if		(CATEGORY_ODD_NUMBER&SWV(4))
		if( SWV(CHK4th) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(5))
		if( (SWV(CHK1st)|SWV(CHK4th)) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(6))
		if( !(SWV(CHK4th) & dwCMPVAL) )
#elif	(CATEGORY_ODD_NUMBER&SWV(7))
		if( SWV(CHK2nd) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(8))
		if( !( (SWV(CHK2nd)|SWV(CHK3rd)) & dwCMPVAL ) )
#elif	(CATEGORY_ODD_NUMBER&SWV(9))
		if( !(SWV(CHK3rd) & dwCMPVAL) )
#elif	(CATEGORY_ODD_NUMBER&SWV(10))
		if( SWV(CHK4th) & dwCMPVAL )
#elif	(CATEGORY_ODD_NUMBER&SWV(11))
		if( 1 )
#endif
			return TRUE;
		return FALSE;
	}

	inline uSNKEY	RoundUp00(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( a, c, d, b );
		return dwSUM;
	}

	inline uSNKEY	RoundUp01(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = SHA32_T3( a );
		EDW(dwSUM) = SHA32_T3( b ) + EDW(dwSUM);

		return dwSUM;
	}

	inline uSNKEY	RoundUp02(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = _RNDFN_F0( a, b, c );

#if		(CATEGORY_ODD_NUMBER&SWV(4))
		EDW(dwSUM) ^= 0x00000002;
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(5))
#elif	(CATEGORY_ODD_NUMBER&SWV(6))
#elif	(CATEGORY_ODD_NUMBER&SWV(7))
#elif	(CATEGORY_ODD_NUMBER&SWV(8))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(9))
		EDW(dwSUM) ^= 0x80000000;
#elif	(CATEGORY_ODD_NUMBER&SWV(10))
		EDW(dwSUM) += _RNDFN_F1( b, c, d );
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(11))
#endif

		return dwSUM;
	}

	inline uSNKEY	RoundUp03(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = _RNDFN_F1( b, c, a );

#if		(CATEGORY_ODD_NUMBER&SWV(4))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(5))
		EDW(dwSUM) += 0x00000001;
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(6))
		EDW(dwSUM) |= 0x30000000;
#elif	(CATEGORY_ODD_NUMBER&SWV(7))
#elif	(CATEGORY_ODD_NUMBER&SWV(8))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(9))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(10))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(11))
		EDW(dwSUM) += _RNDFN_F0( d, EDW(dwSUM), a );
#endif

		return dwSUM;
	}

	inline uSNKEY	RoundUp04(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( c, d, a, b );
		return dwSUM;

#if		(CATEGORY_ODD_NUMBER&SWV(7))
		EDW(dwSUM) += 0x00000001;
#elif	(CATEGORY_ODD_NUMBER&SWV(8))
		EDW(dwSUM) ^= 0x00050000 + HEX_KEY_1_GENERATOR_004;
#endif
	}

	inline uSNKEY	RoundUp05(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = SHA32_T0(d);
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( c, EDW(dwSUM), a, b );

#if		(CATEGORY_ODD_NUMBER&SWV(5))
		EDW(dwSUM) += SHA32_T0(EDW(dwSUM));
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(6))
		EDW(dwSUM) ^= SHA32_T0(EDW(dwSUM));
#endif

		return dwSUM;
	}

	inline uSNKEY	RoundUp06(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = SHA32_T3(a);

#if		(CATEGORY_ODD_NUMBER&SWV(8))
		EDW(dwSUM) += _RNDFN32_SWAP( a, HEX_KEY_1_GENERATOR_001 );
#elif	(CATEGORY_ODD_NUMBER&SWV(6))
		EDW(dwSUM) += _RNDFN32_SWAP( c, HEX_KEY_1_GENERATOR_002 );
#elif	(CATEGORY_ODD_NUMBER&SWV(10))
		EDW(dwSUM) += _RNDFN32_SWAP( d, HEX_KEY_1_GENERATOR_003 );
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(7))
#elif	(CATEGORY_ODD_NUMBER&SWV(5))
#elif	(CATEGORY_ODD_NUMBER&SWV(9))
#endif
#if		(CATEGORY_ODD_NUMBER&SWV(11))
#elif	(CATEGORY_ODD_NUMBER&SWV(4))
#endif
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( c, a, EDW(dwSUM), d );

		return dwSUM;
	}

	inline uSNKEY	RoundUp07(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( a, b, a, c );
		return dwSUM;
	}

	inline uSNKEY	RoundUp08(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( c, b, a, b );
		return dwSUM;
	}

	inline uSNKEY	RoundUp09(   DWORD   a,   DWORD   b,   DWORD   c,   DWORD   d )
	{
		uSNKEY dwSUM;
		EDW(dwSUM) = MAKE_RANDOM32_KEY_EXPOSED( d, c, d, a );
		return dwSUM;
	}
};

//==================================================================================================
#pragma warning(pop)
//==================================================================================================
