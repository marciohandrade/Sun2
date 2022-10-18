// FI_RND_GENDER_CALCULATOR.h
#pragma once

#define CONSTVAL(val)   CONST_##val

#define MAKERND(val)    \
    ((CONST_VA01*(DWORD64)(val)) + ((CONST_VA02 >> 16) + (val) + CONST_VA01))

#define MAKERND2(val1, const_val2)  \
    ( (const_val2*(DWORD64)(val1) + ((const_val2 >> 16) + (val1)) ) )

#define MAKERNDEX(val1, const_val2, const_val3) \
    ( (const_val2*(DWORD64)(val1) + ((const_val3 >> 16) + (val1)) ) )

#define MAKERNDEX_FIX(val1, val2, val3) \
    ( ((DWORD64)(val2) * ((val1) + CONSTVAL(VA03))) + ( (val3 >> 16) + ((val1) + CONSTVAL(VA05)) ) )

#define NEXTRANDOMVALUE(val, val2)  \
    const DWORD CONST_##val = static_cast<DWORD>(MAKERND(val2))

#define RND_BASE_VALUE  (PROTOCOL_ODD_NUMBER % (16384 - 1))

//////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4307 4309)

const DWORD CONST_DEF   = static_cast<DWORD>(0x000488AF * (RND_BASE_VALUE + 1));
const DWORD CONST_VA01  = static_cast<DWORD>(MAKERNDEX(CONST_DEF, 0x1214FA03, 0x1214FA03));
const DWORD CONST_VA02  = static_cast<DWORD>(MAKERNDEX(CONST_DEF, 0x490E1433, 0x490E1433));

const DWORD CONST_VA03  = static_cast<DWORD>(MAKERNDEX(CONST_DEF, CONST_VA01, CONST_VA02));
const DWORD CONST_VA04  = static_cast<DWORD>(MAKERNDEX(CONST_VA01, CONST_VA02, CONST_VA03));
const DWORD CONST_VA05  = static_cast<DWORD>(MAKERNDEX(CONST_VA02, CONST_VA03, CONST_VA04));
const DWORD CONST_VA06  = static_cast<DWORD>(MAKERNDEX(CONST_VA03, CONST_VA04, CONST_VA05));
const DWORD CONST_VA07  = static_cast<DWORD>(MAKERNDEX(CONST_VA04, CONST_VA05, CONST_VA06));
const DWORD CONST_VA08  = static_cast<DWORD>(MAKERNDEX(CONST_VA05, CONST_VA06, CONST_VA07));
const DWORD CONST_VA09  = static_cast<DWORD>(MAKERNDEX(CONST_VA06, CONST_VA07, CONST_VA08));
const DWORD CONST_VA10  = static_cast<DWORD>(MAKERNDEX(CONST_VA07, CONST_VA08, CONST_VA09));

NEXTRANDOMVALUE(VA11, CONSTVAL(VA10));
NEXTRANDOMVALUE(VA12, CONSTVAL(VA11));
NEXTRANDOMVALUE(VA13, CONSTVAL(VA12));
NEXTRANDOMVALUE(VA14, CONSTVAL(VA13));
NEXTRANDOMVALUE(VA15, CONSTVAL(VA14));
NEXTRANDOMVALUE(VA16, CONSTVAL(VA15));
NEXTRANDOMVALUE(VA17, CONSTVAL(VA16));
NEXTRANDOMVALUE(VA18, CONSTVAL(VA17));
NEXTRANDOMVALUE(VA19, CONSTVAL(VA18));

#pragma warning(pop)

//////////////////////////////////////////////////////////////////////////

#define GETRNDBYTENO(idx)   \
    static_cast<DWORD>(MAKERNDEX_FIX(idx, CONSTVAL(VA11), CONSTVAL(VA10)))

#define BITSCOUNTinRND(type)    (sizeof(type) << 3)

#define GETRNDNO(bits, refname)	\
	(	\
		(	\
			CONSTVAL(refname)	\
			>>	\
			(BITSCOUNTinRND(DWORD)-(bits-1))	\
		)	\
		| 0x1	\
	)

#define GETRNDNO_BYTE(refname)	\
	(	\
	(	\
	CONSTVAL(refname)	\
	>>	\
	(BITSCOUNTinRND(DWORD)-(16-1))	\
	)	& 0xFF \
	| 0x1	\
	)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 1~4
#define GETRNDNO_RNG04_T1(idx)	GETRNDNO_RNGxx_T1(idx, 27, 4)
#define GETRNDNO_RNG04_T2(idx)	GETRNDNO_RNGxx_T2(idx, 27, 4)
#define GETRNDNO_RNG04_T3(idx)	GETRNDNO_RNGxx_T3(idx, 26, 4)
#define GETRNDNO_RNG04_T4(idx)	GETRNDNO_RNGxx_T4(idx, 25, 4)
#define GETRNDNO_RNG04_T5(idx)	GETRNDNO_RNGxx_T5(idx, 28, 4)
// 1~8
#define GETRNDNO_RNG08_T1(idx)	GETRNDNO_RNGxx_T1(idx, 26, 8)
#define GETRNDNO_RNG08_T2(idx)	GETRNDNO_RNGxx_T2(idx, 27, 8)
#define GETRNDNO_RNG08_T3(idx)	GETRNDNO_RNGxx_T3(idx, 17, 8)
#define GETRNDNO_RNG08_T4(idx)	GETRNDNO_RNGxx_T4(idx, 27, 8)
#define GETRNDNO_RNG08_T5(idx)	GETRNDNO_RNGxx_T5(idx, 21, 8)
// 1~16
#define GETRNDNO_RNG16_T1(idx)	GETRNDNO_RNGxx_T1(idx, 22, 16)
#define GETRNDNO_RNG16_T2(idx)	GETRNDNO_RNGxx_T2(idx, 25, 16)
#define GETRNDNO_RNG16_T3(idx)	GETRNDNO_RNGxx_T3(idx, 22, 16)
#define GETRNDNO_RNG16_T4(idx)	GETRNDNO_RNGxx_T4(idx, 25, 16)
#define GETRNDNO_RNG16_T5(idx)	GETRNDNO_RNGxx_T5(idx, 27, 16)
// 1~32
#define GETRNDNO_RNG32_T1(idx)	GETRNDNO_RNGxx_T1(idx, 23, 32)
#define GETRNDNO_RNG32_T2(idx)	GETRNDNO_RNGxx_T2(idx, 21, 32)
#define GETRNDNO_RNG32_T3(idx)	GETRNDNO_RNGxx_T3(idx, 19, 32)
#define GETRNDNO_RNG32_T4(idx)	GETRNDNO_RNGxx_T4(idx, 22, 32)
#define GETRNDNO_RNG32_T5(idx)	GETRNDNO_RNGxx_T5(idx, 24, 32)

// 1~128
#define GETRNDNO_RNG128_T1(idx)	GETRNDNO_RNGxx_T1(idx, 24, 128)
#define GETRNDNO_RNG128_T2(idx)	GETRNDNO_RNGxx_T2(idx, 24, 128)
#define GETRNDNO_RNG128_T3(idx)	GETRNDNO_RNGxx_T3(idx, 24, 128)
#define GETRNDNO_RNG128_T4(idx)	GETRNDNO_RNGxx_T4(idx, 24, 128)
#define GETRNDNO_RNG128_T5(idx)	GETRNDNO_RNGxx_T5(idx, 24, 128)


//////////////////////////////////////////////////////////////////////////

#define GETRNDNO_RNGxx_T1(idx, shift, bitmask)	\
	(BYTE)	\
	(	\
	(	\
	(	\
	MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA01, CONST_VA09), CONST_VA15 )	\
	>>	\
	(shift)	\
	)	\
	& ((bitmask)-1) \
	)	\
	+ 0x1	\
	)

//////////////////////////////////////////////////////////////////////////

#define GETRNDNO_RNGxx_T2(idx, shift, bitmask)	\
	(BYTE)	\
	(	\
	(	\
	(	\
	MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA02, CONST_VA06), CONST_VA19 )	\
	>>	\
	(shift)	\
	)	\
	& ((bitmask)-1) \
	)	\
	+ 0x1	\
	)

//////////////////////////////////////////////////////////////////////////

#define GETRNDNO_RNGxx_T3(idx, shift, bitmask)	\
	(BYTE)	\
	(	\
	(	\
	(	\
	( MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA02, CONST_VA04), CONST_VA07 )	\
	^ MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA03, CONST_VA05), CONST_VA06 )	\
	)	\
	>>	\
	(shift)	\
	)	\
	& ((bitmask)-1) \
	)	\
	+ 0x1	\
	)

//////////////////////////////////////////////////////////////////////////

#define GETRNDNO_RNGxx_T4(idx, shift, bitmask)	\
	(BYTE)	\
	(	\
	(	\
	(	\
	( MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA06, CONST_VA17), CONST_VA05 )	\
	^ MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA09, CONST_VA11), CONST_VA16 )	\
	)	\
	>>	\
	(shift)	\
	)	\
	& ((bitmask)-1) \
	)	\
	+ 0x1	\
	)

//////////////////////////////////////////////////////////////////////////

#define GETRNDNO_RNGxx_T5(idx, shift, bitmask)	\
	(BYTE)	\
	(	\
	(	\
	(	\
	( MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA01, CONST_VA15), CONST_VA11 )	\
	^ MAKERND2( MAKERNDEX(MAKERND(idx), CONST_VA09, CONST_VA14), CONST_VA13 )	\
	)	\
	>>	\
	(shift)	\
	)	\
	& ((bitmask)-1) \
	)	\
	+ 0x1	\
	)

#include "FI_RND_GENDER.h"

#define PACKET_RND_TOTAL_BLOCK_START(rndSize)		union { BYTE __byRND_PAD_BLOCK[rndSize]
#define PACKET_RND_TOTAL_BLOCK_END()				}

#define PACKET_RND_PAD_START(rndSize)				struct { BYTE __byRND_PAD[rndSize]
#define PACKET_RND_PAD_CONT(rndSize, seqno)			BYTE __byRND_PAD##seqno[rndSize]
#define PACKET_RND_PAD_END()						}

#define PACKET_RND_SHRBLK64_START(rndSize)				struct { __int64	__byRND_SIZE : rndSize
#define PACKET_RND_SHRBLK64_CONT(type, name)			__int64		name : BITSCOUNTinRND(type)
#define PACKET_RND_SHRBLK64_RNDCONT( rndSize, seqno )	__int64		__rnd_##seqno : rndSize
#define PACKET_RND_SHRBLK64_END()						}

