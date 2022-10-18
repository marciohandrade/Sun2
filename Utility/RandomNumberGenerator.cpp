#include "UtilityCommon.h"
#include "ITimeout.h"
#include "RandomNumberGenerator.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SHARED MACROs>

#define _RNDTMFN_F0(x,y,z)	( ( (x) & (y) ) | ( (z) & ( (x) | (y) ) ) )
#define _RNDTMFN_F1(x,y,z)	( (z) ^ ( (x) & ( (y) ^ (z) ) ) )
#define _RNDTMFN_F2(x,y,z)	( (x) ^ (y) ^ (z) )
//	<<<32BIT OPERATORs>>>
#define _RNDTMFN32_SHR(x,n)		(((x) & 0xFFFFFFFF) >> (n))
#define _RNDTMFN32_SHL(x,n)		((x) << (32 - (n)))
#define _RNDTMFN32_SWAP(x,n)	( _RNDTMFN32_SHR(x,n) | _RNDTMFN32_SHL(x,n) )

#define RNDTM_SHA32_T1(x) (_RNDTMFN32_SWAP(x, 7) ^ _RNDTMFN32_SWAP(x,18) ^ _RNDTMFN32_SHR(x, 3))
#define RNDTM_SHA32_T2(x) (_RNDTMFN32_SWAP(x,17) ^ _RNDTMFN32_SWAP(x,19) ^ _RNDTMFN32_SHR(x,10))

#define RNDTM_COMPLEX_CALC(a,b,c,d)	\
	( ( (a) + RNDTM_SHA32_T1(b) + RNDTM_SHA32_T2(c) ) + ( _RNDTMFN32_SWAP(d, 2) + b + c ) )

#define RNDTM_DEF_CALCULATOR( input, base, adder )	\
	( (input)*(base) + (adder) )

//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct RandomNumberGeneratorAutoClosure
{
    RandomNumberGeneratorAutoClosure()
    {}
    ~RandomNumberGeneratorAutoClosure()
    {
        if (RandomNumberGenerator::ms_pInstance) {
            TAllocDelete(RandomNumberGenerator, RandomNumberGenerator::ms_pInstance);
        };
        RandomNumberGenerator::ms_pInstance = NULL;
    }
};

static RandomNumberGeneratorAutoClosure		randomNumberGeneratorAutoClosure;

//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

RandomNumberGenerator*
RandomNumberGenerator::ms_pInstance = 0;


RandomNumberGenerator::RandomNumberGenerator()
{
	ms_pInstance = this;
    m_UpdateTimer.SetTimer(UPDATE_INTERVAL);

	const DWORD randTick= GetTickCount();
	DWORD _tempV		= (DWORD)rand();
	_tempV				= _tempV ? _tempV : randTick+BASE_ADDER;

	const DWORD randV	= RNDTM_COMPLEX_CALC( randTick, _tempV, BASE_MULTIPLIER, BASE_ADDER );
	m_Values[1]			= randV + (BASE_ADDER>>0);
	m_Values[2]			= randV + (BASE_ADDER>>1);
	m_Values[3]			= randV + (BASE_ADDER>>2);

	m_Values[0]			= RNDTM_COMPLEX_CALC( randV, m_Values[1], m_Values[2], m_Values[3] );
}

VOID
RandomNumberGenerator::_Update()
{
    if (ms_pInstance == NULL) {
        ms_pInstance = TAllocNew(RandomNumberGenerator);
    };
	RandomNumberGenerator* this_ = ms_pInstance;
	if( this_->m_UpdateTimer.IsExpired() )
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		const DWORD rndTime	= RNDTM_SHA32_T2(counter.LowPart);
		DWORD _tempV		= (DWORD)rand();	// 범위의 제한이 걸린다. (MS WinXP SP3 의 경우는 40000이하의 수치만 나온다.)
		_tempV				+= _RNDTMFN32_SWAP(rndTime, 5);

		const DWORD adder	= _tempV&1 ? _tempV : _tempV+BASE_ADDER;
		const DWORD randV	= RNDTM_COMPLEX_CALC( rndTime, _tempV, BASE_MULTIPLIER, adder );
		//randV				= RNDTM_SHA32_T1(randV);

		DWORD rArr0			= this_->m_Values[0];
		DWORD rArr1			= this_->m_Values[1];
		DWORD rArr2			= this_->m_Values[2];
		DWORD rArr3			= this_->m_Values[3];

		rArr1				= RNDTM_COMPLEX_CALC( randV, rArr2, rArr3, rArr1 );
		rArr2				= RNDTM_COMPLEX_CALC( rArr3, randV, rArr1, rArr2 );
		rArr3				= RNDTM_COMPLEX_CALC( rArr1, rArr2, randV, rArr3 );
		rArr0				= RNDTM_COMPLEX_CALC( rArr2, rArr3, rArr1, randV );

		rArr0				= RNDTM_DEF_CALCULATOR( rArr0, BASE_MULTIPLIER, adder );

		this_->m_Values[0]	= rArr0;
		this_->m_Values[1]	= rArr1;
		this_->m_Values[2]	= rArr2;
		this_->m_Values[3]	= rArr3;
	}
}

VOID
RandomNumberGenerator::AddRandomNumberEntropy( const DWORD entropy )
{
	if( !entropy )
		return;

	const DWORD selector			= GetRandomNumberRange( 0, BASE_RNDARRAY );
	const DWORD selector_n			= GetRandomNumberRange( 0, BASE_RNDARRAY );
	RandomNumberGenerator* this_	= ms_pInstance;

	DWORD src1						= this_->m_Values[selector];
	DWORD src2						= this_->m_Values[selector_n];
	src2							= src2&1 ? src2 : src2+BASE_ADDER;
	src1							+= RNDTM_DEF_CALCULATOR( entropy, BASE_MULTIPLIER, src2 );

	if( selector == selector_n )
	{
		const DWORD shifter			= src1&15;
		src1						= _RNDTMFN32_SWAP( src1, shifter );
		this_->m_Values[selector]	+= src1;
	}
	else
	{
		const DWORD shifter			= src2&15;
		src2						= _RNDTMFN32_SWAP( src2, shifter );
		this_->m_Values[selector]	+= src2;
		this_->m_Values[selector_n]	+= src1;
	}
}

DWORD
RandomNumberGenerator::GetRandomNumber()
{
	_Update();
	RandomNumberGenerator* this_	= ms_pInstance;
	DWORD rArr0						= this_->m_Values[0];
	const DWORD adder				= this_->m_Values[1]&1 ? this_->m_Values[1] : this_->m_Values[1]+BASE_ADDER;
	rArr0 = this_->m_Values[0]		= RNDTM_DEF_CALCULATOR( rArr0, BASE_MULTIPLIER, adder );

	const DWORD shifter				= rArr0&31;
	rArr0							= _RNDTMFN32_SWAP( rArr0, shifter );
	return rArr0;
}

DWORD
RandomNumberGenerator::GetRandomNumberRange( const DWORD _min, const DWORD _max )
{
    if (_min == _max) {
        return _max;
    }
	_Update();
	const DWORD minV				= min( _min, _max );
	const DWORD dist				= max( _min, _max ) - minV;

	if( !FlowControl::FCAssert( dist || (_min != _max) ) )
		return 0;

	RandomNumberGenerator* this_	= ms_pInstance;
	DWORD rArr0						= this_->m_Values[0];
	const DWORD adder				= this_->m_Values[2]&1 ? this_->m_Values[2] : this_->m_Values[2]+BASE_ADDER;
	rArr0 = this_->m_Values[0]		= RNDTM_DEF_CALCULATOR( rArr0, BASE_MULTIPLIER, adder );

	const DWORD shifter				= rArr0&31;
	rArr0							= _RNDTMFN32_SWAP( rArr0, shifter );
	const DWORD randV				// dist는 결국 사용자 범위 입력 수치로, 특정 패턴이 양상될 수 있으므로 다음과 같이 변경한다.
		= dist < 0x00000100
		?	((rArr0>>15) % dist) + minV
		: dist < 0x00010000
		?	((rArr0>> 6) % dist) + minV
		:	((rArr0>> 0) % dist) + minV
		;

	return randV;
}



