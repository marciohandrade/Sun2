#pragma once

#pragma warning(push)
#pragma warning(disable:4307)
#pragma warning(disable:4308)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <RANDOM HASH GENERATOR FUNCTION>

// <SHARED MACROs>
#define _RNDFN_F0(x,y,z)	((x & y) | (z & (x | y)))
#define _RNDFN_F1(x,y,z)	(z ^ (x & (y ^ z)))
#define _RNDFN_F2(x,y,z)	( (x) ^ (y) ^ (z) )

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<<<32BIT OPERATORs>>>
#define _RNDFN32_SHR(x,n)		((x & 0xFFFFFFFF) >> n)
#define _RNDFN32_SHL(x,n)		(x << (32 - n))
#define _RNDFN32_SWAP(x,n)		( _RNDFN32_SHR(x,n) | _RNDFN32_SHL(x,n) )

#define SHA32_T0(x) (_RNDFN32_SWAP(x, 7) ^ _RNDFN32_SWAP(x,18) ^ _RNDFN32_SHR(x, 3))
#define SHA32_T1(x) (_RNDFN32_SWAP(x,17) ^ _RNDFN32_SWAP(x,19) ^ _RNDFN32_SHR(x,10))
#define SHA32_T2(x) (_RNDFN32_SWAP(x, 2) ^ _RNDFN32_SWAP(x,13) ^ _RNDFN32_SWAP(x,22))
#define SHA32_T3(x) (_RNDFN32_SWAP(x, 6) ^ _RNDFN32_SWAP(x,11) ^ _RNDFN32_SWAP(x,25))

//////////////////////////////////////////////////////////////////////////
//	<32BIT INPUT ARG 7>
//		- Internal RCG Generator
#define _RNDFN32_OUT0(a,b,c,d,e,f,g)                  \
	( ( g + SHA32_T0(e) + _RNDFN_F1(e,f,g) + c + d ) + (SHA32_T1(_RNDFN_F0(a,g,g)) + _RNDFN_F1(a,b,c)) )

#define _RNDFN32_OUT1(a,b,c,d,e,f,g)                  \
	( ( g + SHA32_T1(e) + _RNDFN_F1(e,f,g) + c + d ) + (SHA32_T2(_RNDFN_F0(g,g,c)) + _RNDFN_F0(a,b,c)) )

#define _RNDFN32_OUT2(a,b,c,d,e,f,g)                  \
	( ( g + SHA32_T2(e) + _RNDFN_F0(e,f,g) + c + d ) + (SHA32_T3(_RNDFN_F1(g,b,g)) + _RNDFN_F0(a,b,c)) )

//		- External RLG Generator (exported code)
#define _RNDFN32_OUT3(a,b,c,d,e,f,g)                  \
	( ( g + SHA32_T3(e) + _RNDFN_F0(e,f,g) + c + d ) + (SHA32_T0(_RNDFN_F1(g,g,g)) + _RNDFN_F1(a,b,c)) )

//////////////////////////////////////////////////////////////////////////
//	<32BIT INPUT ARG 4>
//		- Internal RCG Generator
#define _RNDFN32_OUT_B11(a,b,c,d)		                \
	( ( d + SHA32_T3(c) + _RNDFN_F0(b,c,d) + a + d ) + (SHA32_T0(_RNDFN_F1(d,c,d)) + _RNDFN_F0(a,b,c)) )
#define _RNDFN32_OUT_B12(a,b,c,d)		                \
	( ( d + SHA32_T2(c) + _RNDFN_F0(b,c,d) + a + d ) + (SHA32_T2(_RNDFN_F1(d,d,d)) + _RNDFN_F1(a,b,c)) )
//	- External RLG Generator (exported code)
//	USED RESOURCE - SHA32_T0, SHA32_T3, _RNDFN_F0, _RNDFN_F1
#define _RNDFN32_OUT_B13(a,b,c,d)		                \
	( ( d + SHA32_T0(c) + _RNDFN_F0(b,c,d) + a + d ) + (SHA32_T3(_RNDFN_F0(c,c,d)) + _RNDFN_F1(a,b,c)) )

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<<<16BIT OPERATORs>>>
#define _RNDFN16_SHR(x,n)		( ((WORD)x & 0xFFFF) >> n )
#define _RNDFN16_SHL(x,n)		( (WORD)(x << (16 - n)) )
#define _RNDFN16_SWAP(x,n)		( _RNDFN16_SHR(x,n) | _RNDFN16_SHL(x,n) )

#define SHA16_T0(x) ( (WORD)(_RNDFN16_SWAP(x, 4) ^ _RNDFN16_SWAP(x,6) ^ _RNDFN16_SHR(x, 13)) )
#define SHA16_T1(x) ( (WORD)(_RNDFN16_SWAP(x, 7) ^ _RNDFN16_SWAP(x,9) ^ _RNDFN16_SHR(x, 10)) )
#define SHA16_T2(x) ( (WORD)(_RNDFN16_SWAP(x, 2) ^ _RNDFN16_SWAP(x,5) ^ _RNDFN16_SWAP(x, 12)) )
#define SHA16_T3(x) ( (WORD)(_RNDFN16_SWAP(x, 6) ^ _RNDFN16_SWAP(x,11) ^ _RNDFN16_SWAP(x, 15)) )

#define WRAP16_ADD2(a,b)		( (WORD)( (WORD)(a) + (WORD)(b) ) )
#define WRAP16_ADD3(a,b,c)		( (WORD)( (WORD)(WRAP16_ADD2(a,b)) + (WORD)(c) ) )
#define WRAP16_ADD4(a,b,c,d)	( (WORD)( (WORD)(WRAP16_ADD3(a,b,c)) + (WORD)(d) ) )
#define WRAP16_ADD5(a,b,c,d,e)	( (WORD)( (WORD)(WRAP16_ADD4(a,b,c,d)) + (WORD)(e) ) )

//////////////////////////////////////////////////////////////////////////
//	<16BIT INPUT ARG 7>
//		- Internal RCG Generator
#define _RNDFN16_OUT0(a,b,c,d,e,f,g)                  \
	( WRAP16_ADD2( WRAP16_ADD5(g, SHA16_T0(e), _RNDFN_F1(e,f,g), c, d),	\
		WRAP16_ADD2(SHA16_T1(_RNDFN_F0(a,g,g)), _RNDFN_F1(a,b,c)) ) )

#define _RNDFN16_OUT1(a,b,c,d,e,f,g)                  \
	( WRAP16_ADD2( WRAP16_ADD5( g, SHA16_T1(e), _RNDFN_F1(e,f,g), c, d ),	\
		WRAP16_ADD2(SHA16_T2(_RNDFN_F0(g,g,c)), _RNDFN_F0(a,b,c)) ) )

#define _RNDFN16_OUT2(a,b,c,d,e,f,g)                  \
	( WRAP16_ADD2( WRAP16_ADD5( g, SHA16_T2(e), _RNDFN_F0(e,f,g), c, d ),	\
		WRAP16_ADD2(SHA16_T3(_RNDFN_F1(g,b,g)), _RNDFN_F0(a,b,c)) ) )

//		- External RLG Generator (exported code)
#define _RNDFN16_OUT3(a,b,c,d,e,f,g)                  \
	( WRAP16_ADD2( WRAP16_ADD5( g, SHA16_T3(e), _RNDFN_F0(e,f,g), c, d ),	\
		WRAP16_ADD2(SHA16_T0(_RNDFN_F1(g,g,g)), _RNDFN_F1(a,b,c)) ) )

//////////////////////////////////////////////////////////////////////////
//	<16BIT INPUT ARG 4>
//		- Internal RCG Generator
#define _RNDFN16_OUT_B11(a,b,c,d)		                \
	( WRAP16_ADD2( WRAP16_ADD5( d, SHA16_T3(c), _RNDFN_F0(b,c,d), a, d ),	\
		WRAP16_ADD2(SHA16_T0(_RNDFN_F1(d,c,d)), _RNDFN_F0(a,b,c)) ) )

#define _RNDFN16_OUT_B12(a,b,c,d)		                \
	( WRAP16_ADD2( WRAP16_ADD5( d, SHA16_T2(c), _RNDFN_F0(b,c,d), a, d ),	\
		WRAP16_ADD2(SHA16_T2(_RNDFN_F1(d,d,d)), _RNDFN_F1(a,b,c)) ) )

//		- External RLG Generator (exported code)
#define _RNDFN16_OUT_B13(a,b,c,d)		                \
	( WRAP16_ADD2( WRAP16_ADD5( d, SHA16_T0(c), _RNDFN_F0(b,c,d), a, d ),	\
		WRAP16_ADD2(SHA16_T3(_RNDFN_F0(c,c,d)), _RNDFN_F1(a,b,c)) ) )

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<EXPOSED CODE RANDOM GENERATOR>
#define MAKE_RANDOM32_KEY_EXPOSED		_RNDFN32_OUT_B13
#define MAKE_RANDOM16_KEY_EXPOSED		_RNDFN16_OUT_B13

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
// <RANDOMKEY GENERATOR with CATEGORYnPROTOCOL>
//	<DWORD & WORD>
#define MAKEFORMAT_WORD_RNDKEY( a, b )			\
	((((PROTOCOL_GENERATOR(a))&0x03) << 0) |	\
	(((PROTOCOL_GENERATOR(a))&0x0C) << 2) |		\
	(((PROTOCOL_GENERATOR(b))&0x30) << 4) |		\
	(((PROTOCOL_GENERATOR(b))&0xC0) << 6) |		\
	(((CATEGORY_GENERATOR(a))&0x03) << 14) |	\
	(((CATEGORY_GENERATOR(a))&0x0C) << 8) |		\
	(((CATEGORY_GENERATOR(b))&0x30) << 2) |		\
	(((CATEGORY_GENERATOR(b))&0xC0) >> 4))


#define MAKEFORMAT_DWORD_RNDKEY( a, b, c, d )													\
	SHA32_T0( ( MAKEFORMAT_WORD_RNDKEY( PROTOCOL_GENERATOR(a), CATEGORY_GENERATOR(b) ) << 16 ) |	\
	( MAKEFORMAT_WORD_RNDKEY( PROTOCOL_GENERATOR(c), CATEGORY_GENERATOR(d) )  ) )

//(DWORD)(PROTOCOL_GENERATOR(a)<<24 | CATEGORY_GENERATOR(b)<<16	\
//| PROTOCOL_GENERATOR(c)<<8 | CATEGORY_GENERATOR(d) )



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<RCG - Random Const Generated-Number>
//
//////////////////////////////////////////////////////////////////////////
//	<MOLA_RCG_X - DEFAULT RANDOM NUMBER { A,B,C,D,E,F,G }
const DWORD MOLA_RCG_A		= MAKEFORMAT_DWORD_RNDKEY( 'M', 'O', 'L', 'A' );

const DWORD MOLA_RCG_B		= MAKEFORMAT_DWORD_RNDKEY(
	_RNDFN32_OUT_B11( 'M', 'O', ~CATEGORY_ODD_NUMBER, ~PROTOCOL_ODD_NUMBER ),
	_RNDFN32_OUT_B12( CATEGORY_ODD_NUMBER, PROTOCOL_ODD_NUMBER, 'L', 'A' ),
	_RNDFN32_OUT_B13( (CATEGORY_ODD_NUMBER>>1), 'O', (PROTOCOL_ODD_NUMBER>>1), 'A' ),
	MOLA_RCG_A
	);

const DWORD MOLA_RCG_C		= MAKEFORMAT_DWORD_RNDKEY(
	MOLA_RCG_A,
	_RNDFN32_OUT_B11( 'M', (CATEGORY_ODD_NUMBER>>3), 'L', (PROTOCOL_ODD_NUMBER>>7) ),
	_RNDFN32_OUT_B12( (CATEGORY_ODD_NUMBER>>5), 'O', 'L', (PROTOCOL_ODD_NUMBER>>4) ),
	MOLA_RCG_B
	);

#define _RNDFN_OUT0_MAKEHASH( postfix, a,b,c,d,e,f,g )	\
	const DWORD MOLA_RCG_##postfix = _RNDFN32_OUT0( MOLA_RCG_##a, MOLA_RCG_##b, MOLA_RCG_##c, MOLA_RCG_##d, MOLA_RCG_##e, MOLA_RCG_##f, MOLA_RCG_##g )

_RNDFN_OUT0_MAKEHASH(D,	A,	B,	C,	A,	B,	C,	C);
_RNDFN_OUT0_MAKEHASH(E,	B,	C,	D,	A,	B,	C,	D);
_RNDFN_OUT0_MAKEHASH(F,	C,	D,	E,	A,	B,	C,	E);
_RNDFN_OUT0_MAKEHASH(G,	D,	E,	F,	A,	B,	C,	F);

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<MOLA PROTOCOL VERSION CRC>
const DWORD	MOLA_PROTOCOL_CRC	= _RNDFN32_OUT0(
										MOLA_RCG_G,
										(VERHASH_CATEGORY()<<OCT_KEY_1_GENERATOR_003),
										(VERHASH_PROTOCOL()<<OCT_KEY_1_GENERATOR_008),
										(VERHASH_CRYPT_KEY_INITIAL()<<OCT_KEY_1_GENERATOR_005),
										(VERHASH_CRYPT_KEY_SERVICE()<<OCT_KEY_1_GENERATOR_002),
										(PROTOCOL_ODD_NUMBER<<OCT_KEY_1_GENERATOR_007),
										(CATEGORY_ODD_NUMBER) );

//MAKEDWORD(VERHASH_CATEGORY(), VERHASH_PROTOCOL()),
//MAKEDWORD(VERHASH_CATEGORY(), VERHASH_CRYPT_KEY_INITIAL()),
//MAKEDWORD(VERHASH_CRYPT_KEY_SERVICE(), VERHASH_CRYPT_KEY_INITIAL()),

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<MOLA_RCG_000 ~ MOLA_RCG_255>
#undef MAKE_RND_DWORD_BASE_KEY
#define MAKE_RND_DWORD_BASE_KEY( a, b, c, d, e, f, g )						\
	( _RNDFN32_OUT0(a, b, c, d, e, f, g) + _RNDFN32_OUT1(a, b, c, d, e, f, g)	\
	+ _RNDFN32_OUT2(a, b, c, d, e, f, g) + _RNDFN32_OUT3(a, b, c, d, e, f, g) )

#undef _RNDFN_OUT0_MAKEHASH
#define _RNDFN_OUT0_MAKEHASH( postfix, a,b,c,d,e,f,g )	\
	const DWORD MOLA_RCG_##postfix = MAKE_RND_DWORD_BASE_KEY( MOLA_RCG_##a, MOLA_RCG_##b, MOLA_RCG_##c, MOLA_RCG_##d, MOLA_RCG_##e, MOLA_RCG_##f, MOLA_RCG_##g );

#include	<Macros/ConstVariables.HashValue.h>
//
//////////////////////////////////////////////////////////////////////////


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<MOLA_RCG_PAIR_000 ~ MOLA_RCG_PAIR_255>
#undef MAKE_RND_DWORD_BASE_KEY
#define MAKE_RND_DWORD_BASE_KEY( a, b, c, d, e, f, g )						\
	( _RNDFN32_OUT0(b, c, d, e, f, a, g) + _RNDFN32_OUT1(c, d, e, f, a, b, g)	\
	+ _RNDFN32_OUT2(d, e, f, a, b, c, g) + _RNDFN32_OUT3(e, f, a, b, c, d, g) )

#undef _RNDFN_OUT0_MAKEHASH
#define _RNDFN_OUT0_MAKEHASH( postfix, a,b,c,d,e,f,g )	\
	const DWORD MOLA_RCG_PAIR_##postfix = MAKE_RND_DWORD_BASE_KEY( MOLA_RCG_##a, MOLA_RCG_##b, MOLA_RCG_##c, MOLA_RCG_##d, MOLA_RCG_##e, MOLA_RCG_##f, MOLA_RCG_##g );

#include	<Macros/ConstVariables.HashValue.h>
//
//////////////////////////////////////////////////////////////////////////


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<MOLA_RCG_T1_000 ~ MOLA_RCG_T1_255>
//	<EXPORTED VALUES>
#undef MAKE_RND_DWORD_BASE_KEY
#define MAKE_RND_DWORD_BASE_KEY( a, b, c, d, e, f, g )						\
	( _RNDFN32_OUT0(c, d, e, f, a, b, g) + _RNDFN32_OUT1(d, e, f, a, b, c, g)	\
	+ _RNDFN32_OUT2(e, f, a, b, c, d, g) + _RNDFN32_OUT3(f, a, b, c, d, e, g) )

#undef _RNDFN_OUT0_MAKEHASH
#define _RNDFN_OUT0_MAKEHASH( postfix, a,b,c,d,e,f,g )	\
	const DWORD MOLA_RCG_T1_##postfix = MAKE_RND_DWORD_BASE_KEY( MOLA_RCG_##a, MOLA_RCG_##b, MOLA_RCG_##c, MOLA_RCG_##d, MOLA_RCG_##e, MOLA_RCG_##f, MOLA_RCG_##g );

#include	<Macros/ConstVariables.HashValue.h>
//
//////////////////////////////////////////////////////////////////////////

#undef MAKE_RND_DWORD_BASE_KEY
#undef _RNDFN_OUT0_MAKEHASH









const DWORD MOLA_CONST_RND_GEN_BASE		= 0x19F38039;
const DWORD MOLA_CONST_RND_GEN_INC		= 0x3F278313;
#define MAKE_RND_DWORD_BASE_KEY( a, b, c, d )	\
	(DWORD)(CRYPT_KEY_SERVICE_GENERATOR(a)<<24 | CRYPT_KEY_INITIAL_GENERATOR(b)<<16	\
			| CRYPT_KEY_SERVICE_GENERATOR(c)<<8 | CRYPT_KEY_INITIAL_GENERATOR(d) )

const DWORD MOLA_INIT_CODE	= MAKE_RND_DWORD_BASE_KEY(
	CRYPT_KEY_INITIAL_GENERATOR(211), CRYPT_KEY_SERVICE_GENERATOR(36),
	CRYPT_KEY_INITIAL_GENERATOR(24), CRYPT_KEY_SERVICE_GENERATOR(37) );


//////////////////////////////////////////////////////////////////////////

const DWORD MOLA_BASE_CODE_TEMP		= (MOLA_CONST_RND_GEN_BASE*MOLA_INIT_CODE)
									+ (MOLA_CONST_RND_GEN_INC+(MOLA_INIT_CODE)) ;
const DWORD MOLA_BASE_ERR_CODE		= (MOLA_CONST_RND_GEN_BASE*MOLA_BASE_CODE_TEMP)
									+ (MOLA_CONST_RND_GEN_INC+(MOLA_BASE_CODE_TEMP>>16));
const DWORD MOLA_BASE_CODE			= (MOLA_CONST_RND_GEN_BASE*MOLA_BASE_ERR_CODE)
									+ (MOLA_CONST_RND_GEN_INC+(MOLA_BASE_ERR_CODE>>16));

//////////////////////////////////////////////////////////////////////////

const DWORD MOLA_GARBAGE_RND_GEN_BASE =
									(MOLA_CONST_RND_GEN_BASE*MOLA_BASE_CODE)
									+ (MOLA_CONST_RND_GEN_INC+(MOLA_BASE_CODE>>16));
const DWORD MOLA_GARBAGE_RND_GEN_INC =
									(MOLA_CONST_RND_GEN_BASE*MOLA_GARBAGE_RND_GEN_BASE)
									+ (MOLA_CONST_RND_GEN_INC+(MOLA_GARBAGE_RND_GEN_BASE>>16));

#define MAKE_GARBAGE_DWORD_NEXT_KEY( rndKey )	\
	(DWORD)( (MOLA_GARBAGE_RND_GEN_BASE*(rndKey)) + (MOLA_GARBAGE_RND_GEN_INC + ((rndKey)>>16) ) )

//////////////////////////////////////////////////////////////////////////

#define MAKE_DEF_RND_NEXT_KEY( rndKey )	\
	(DWORD)( (MOLA_CONST_RND_GEN_BASE*(rndKey)) + (MOLA_CONST_RND_GEN_INC + ((rndKey)>>16) ) )

//////////////////////////////////////////////////////////////////////////
// SNKey Generators

const DWORD MOLA_SNK_RND_GEN_BASE_INIT	= MAKE_DEF_RND_NEXT_KEY(MOLA_BASE_CODE);
const DWORD MOLA_SNK_RND_GEN_INC_INIT	= MAKE_DEF_RND_NEXT_KEY(MOLA_SNK_RND_GEN_BASE_INIT);

const DWORD MOLA_SNK_RND_GEN_BASE	= MAKE_DEF_RND_NEXT_KEY(MOLA_SNK_RND_GEN_BASE_INIT);
const DWORD MOLA_SNK_RND_GEN_INC	= MAKE_DEF_RND_NEXT_KEY(MOLA_SNK_RND_GEN_INC_INIT);

#define MAKE_SNK_RND_NEXT_KEY_INIT( rndKey )	\
	(DWORD)( (MOLA_SNK_RND_GEN_BASE_INIT*(rndKey)) + (MOLA_SNK_RND_GEN_INC_INIT + ((rndKey)>>16) ) )



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//
//
//
//
//


//////////////////////////////////////////////////////////////////////////
// ------- using (RandomSNKey)
#define MAKE_SNK_RND_NEXT_KEY_BY_BASE_INIT( baseKey, rndKey )	\
	(DWORD)( ((baseKey)*(rndKey)) + ((baseKey) + ((rndKey)>>16) ) )

// ------- using (RandomSNKey)
#define MAKE_SNK_RND_NEXT_KEY_BY_BASE_PAIR_INIT( baseKey, pairKey, rndKey )	\
	(DWORD)( ((baseKey)*(rndKey)) + ( (pairKey) + ((rndKey)>>16) ) )


#define MAKE_SNK_RND_NEXT_KEY( rndKey )	\
	(DWORD)( (MOLA_SNK_RND_GEN_BASE*(rndKey)) + (MOLA_SNK_RND_GEN_INC + ((rndKey)>>16) ) )

#define MAKE_SNK_RND_NEXT_KEY_BY_BASE( baseKey, rndKey )	\
	(DWORD)( ((baseKey)*(rndKey)) + ((baseKey) + ((rndKey)>>16) ) )

#define MAKE_SNK_RND_NEXT_KEY_BY_BASE_PAIR( baseKey, pairKey, rndKey )	\
	(DWORD)( ((baseKey)*(rndKey)) + ( (pairKey) + ((rndKey)>>16) ) )

//////////////////////////////////////////////////////////////////////////
// ISecureWorkNode
const DWORD MOLA_SECURE_WORKNODE_WALL1	= MAKE_SNK_RND_NEXT_KEY_BY_BASE(MOLA_BASE_CODE, MOLA_SNK_RND_GEN_BASE_INIT);
const DWORD MOLA_SECURE_WORKNODE_WALL2	= MAKE_SNK_RND_NEXT_KEY_BY_BASE(MOLA_BASE_CODE, MOLA_SECURE_WORKNODE_WALL1);

//#if		(HEX_KEY_1_GENERATOR_001 == 0)
//#elif	(HEX_KEY_1_GENERATOR_001 == 1)
//#elif	(HEX_KEY_1_GENERATOR_001 == 2)
//#elif	(HEX_KEY_1_GENERATOR_001 == 3)
//#elif	(HEX_KEY_1_GENERATOR_001 == 4)
//#elif	(HEX_KEY_1_GENERATOR_001 == 5)
//#elif	(HEX_KEY_1_GENERATOR_001 == 6)
//#elif	(HEX_KEY_1_GENERATOR_001 == 7)
//#elif	(HEX_KEY_1_GENERATOR_001 == 8)
//#elif	(HEX_KEY_1_GENERATOR_001 == 9)
//#elif	(HEX_KEY_1_GENERATOR_001 == 10)
//#elif	(HEX_KEY_1_GENERATOR_001 == 11)
//#elif	(HEX_KEY_1_GENERATOR_001 == 12)
//#elif	(HEX_KEY_1_GENERATOR_001 == 13)
//#elif	(HEX_KEY_1_GENERATOR_001 == 14)
//#elif	(HEX_KEY_1_GENERATOR_001 == 15)
//#endif	//(HEX_KEY_1_GENERATOR_001 == n)

#pragma warning(pop)

