#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/// RandomNumberGenerator class
/**
	@author	Waverix
	@since	2008/08/06
	@note
		- Randomizing Number Generator
		- util::Randomizer나 기타 random number 사용시 특수 편향된 랜덤 값이 나오는 부분에 대한 개선 작업
	@todo
	- Interface #1)	RandomNumberGenerator::GetRandomNumber() <- DWORD 값의 random값을 얻는다.
	- Interface #2)	RandomNumberGenerator::GetRandomNumberRange( _min, _max ) <- DWORD이내 _min <= X < max 범위의 랜덤값을 얻는다.
	- Interface #3)	RandomNumberGenerator::AddRandomNumberEntropy()
		<- 반드시 사용할 필요는 없다. 사용자 입력을 비롯한 외부 수치(Entropy)를 이용해 Next Generation Random Base Value를 얻는다.
	@history
	- 

*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


class RandomNumberGenerator
{
public:		RandomNumberGenerator();
public:
	static	DWORD		GetRandomNumber();
	static	DWORD		GetRandomNumberRange( const DWORD _min, const DWORD _max );
	static	VOID		AddRandomNumberEntropy( const DWORD entropy );	// runtime - user input, randomizing by entropy - (0, MAX]
private:
	static	VOID		_Update();
private:
    // f101227.1L, changes refresh interval about the base random seed update
	static const DWORD		UPDATE_INTERVAL		=	3 * 1000;
	static const DWORD		BASE_RNDARRAY		=	4;
	static const DWORD		BASE_MULTIPLIER		=	0x0012FF41;
	static const DWORD		BASE_ADDER			=	0x03712BA7;
	static	RandomNumberGenerator*		ms_pInstance;
private:	ITimerBase		m_UpdateTimer;	// next seed;
			DWORD			m_Values[BASE_RNDARRAY];
			friend struct RandomNumberGeneratorAutoClosure;
};



