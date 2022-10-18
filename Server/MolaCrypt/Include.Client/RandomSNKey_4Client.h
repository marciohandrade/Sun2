#ifndef _RND_SNKEY_H	
#define _RND_SNKEY_H
#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
#ifdef _MOLA_EXPORTS
#	define EXPORTS_API		__declspec(dllexport)
#else //
#	define EXPORTS_API
#endif //


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
#ifndef __MOLA_VERSION__
#	define __MOLA_VERSION__	5000
#endif __MOLA_VERSION__

#define MAKE_MOLA_VERSION_( major, minor, devel )	\
	( (major<<24) | (minor<<16) | (devel&0xFFFF) )


#pragma pack(push,1)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//// <TYPE DEFINITIONS>
typedef DWORD	SNKey_T;
typedef BYTE	SNKB;
typedef WORD	SNKW;

struct eSNKEY_IDX
{
	enum {
		SOLAR_BASE		,
		SOLAR_PAIR		,
		LUNAR_BASE		,
		LUNAR_PAIR		,
		SIZE_KEY_MAX	,

		SIZE_KEYW_MAX	= LUNAR_BASE,	// 2
		LOWORD_IDX		= SOLAR_BASE,	// 0
		HIWORD_IDX		= SOLAR_PAIR,	// 1
	};
};

union uSNKEY
{
	DWORD		dwKEY;
	SNKB		pKEYS[eSNKEY_IDX::SIZE_KEY_MAX];
	SNKW		pKEYW[eSNKEY_IDX::SIZE_KEYW_MAX];
#pragma warning(push)
#pragma warning(disable:4201)
	struct
	{
		SNKB	bySolarBaseKey;
		SNKB	bySolarPairKey;
		SNKB	byLunarBaseKey;
		SNKB	byLunarPairKey;
	};
#pragma warning(pop)
};

//
#ifndef SECURE_VARIABLE_SIZE_BLOCK
#define SECURE_VARIABLE_SIZE_BLOCK

struct sSECURE_VARIABLE_SIZE_BLOCK
{
	BYTE	m_byLength;
	BYTE	m_pSTREAM[1];
};

#endif //SECURE_VARIABLE_SIZE_BLOCK

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
class EXPORTS_API RandomSNKey
{
public:		static const SNKey_T SNKEY_BASE		= 0;
			static const SNKey_T SNKEY_PAIR		= 1;
			static const SNKey_T c_dwBaseKey	= 0;
			static const SNKey_T c_dwPairKey	= 1;
public:
			RandomSNKey();
			// Init & Duplex-Communication Random Key
			VOID			Initialize( DWORD dwRndKey, BYTE majorVer, BYTE minorVer );
			VOID			KeyExchange( DWORD dwRndKey );

			inline SNKB		GetSolarSNKey( SNKey_T keyType )
			{
				SNKB bySolarKey = (SNKEY_BASE == keyType) ? m_CURKEY.bySolarBaseKey : m_CURKEY.bySolarPairKey;
				return bySolarKey;
			};
			inline SNKB		GetLunarSNKey( SNKey_T keyType )
			{
				SNKB byLunarKey = (SNKEY_BASE == keyType) ? m_CURKEY.byLunarBaseKey : m_CURKEY.byLunarPairKey;
				return byLunarKey;
			};

			VOID			NextRoundSNKey4Solar();
			VOID			NextRoundSNKey4Lunar();

private:
			uSNKEY			m_CURKEY;
			uSNKEY			m_KEYBLK[6];








public:
//////////////////////////////////////////////////////////////////////////
// 패킷 종류에 따라 처리
#if (__MOLA_VERSION__ >= 1010)
			//////////////////////////////////////////////////////////////////////////
			//	__MOLA_VERSION__ == 1010
			//	PreConvert - Simplex-Communication Random Key In Client/Server
			//		Pattern00 : (OLDver.) CMD->CONV->ACK
			VOID			KeyExchangerSimplex( DWORD dwRndKey );
			//	__MOLA_VERSION__ == 5000
			//	Normal Convert - Simplex-Communication Random Key In Client/Server
			//		Pattern01 : CMD->(CONV)->ACK->CONV
			//		(CONV) : SRC KEY BLOCK만 변경한다.
			VOID			KeyExchangerSimplexPattern01( DWORD dwRndKey );
			//	__MOLA_VERSION__ == 5000
			//	PreConvert - Simplex-Communication Random Key In Client/Server
			//		Pattern02 : CMD->CONV->ACK
			//		CONV : NextRoundKey하는 효과. Server에서는 길이 검사를 통해 해결한다.
			VOID			KeyExchangerSimplexPattern02( DWORD dwRndKey );
			//	__MOLA_VERSION__ == 5000
			//	PreConvert - Simplex-Communication Random Key In Client/Server
			//		Pattern03 : CMD->CONV
			//		<Not Implementation>
			VOID			KeyExchangerSimplexPattern03( DWORD dwRndKey );
			// similar to Initialize
			VOID			KeyExchangerDuplex( DWORD dwRndKey );
#endif //(__MOLA_VERSION__ >= 1010)
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
class ClientSNKey : public RandomSNKey
{
public:
			inline SNKB		GetSendSNKey( SNKey_T keyType ) { return GetLunarSNKey(keyType); }
			inline SNKB		GetRecvSNKey( SNKey_T keyType ) { return GetSolarSNKey(keyType); }

			inline VOID		NextRoundSNKey4Send() { NextRoundSNKey4Lunar(); }
			inline VOID		NextRoundSNKey4Recv() { NextRoundSNKey4Solar(); }
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//










#pragma pack(pop)



#endif //_RND_SNKEY_H
