#pragma once

//��������������������������������������������������������������������������������������������������������������������������������������������������
/// RandomNumberGenerator class
/**
	@author	Waverix
	@since	2008/08/06
	@note
		- Randomizing Number Generator
		- util::Randomizer�� ��Ÿ random number ���� Ư�� ����� ���� ���� ������ �κп� ���� ���� �۾�
	@todo
	- Interface #1)	RandomNumberGenerator::GetRandomNumber() <- DWORD ���� random���� ��´�.
	- Interface #2)	RandomNumberGenerator::GetRandomNumberRange( _min, _max ) <- DWORD�̳� _min <= X < max ������ �������� ��´�.
	- Interface #3)	RandomNumberGenerator::AddRandomNumberEntropy()
		<- �ݵ�� ����� �ʿ�� ����. ����� �Է��� ����� �ܺ� ��ġ(Entropy)�� �̿��� Next Generation Random Base Value�� ��´�.
	@history
	- 

*/
//��������������������������������������������������������������������������������������������������������������������������������������������������


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



