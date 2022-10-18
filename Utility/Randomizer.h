#pragma once

#include <vector>
#include <algorithm>

typedef std::vector<int>		RANDOM_VEC;
typedef RANDOM_VEC::iterator	RANDOM_VEC_ITER;

// 정규화된 난수 추출기
// 맨첨에 Create 한번 하고 Rand 함수 사용하면 됨!
class Randomizer
{
public:
	Randomizer() { m_iRandMax = 0; m_iGetCount = 0; }
	~Randomizer() { Destroy(); }

	void Create( int iRandMax )
	{
        if (m_vecRandom.size() != 0) {
            Destroy();
        }

		m_iRandMax = iRandMax;
		for( int i = 1; i <= iRandMax; ++i )
		{
			m_vecRandom.push_back( i );
		}
        m_iGetCount = m_iRandMax - 1;
		RandomShuffule();
	}

	void Destroy()
	{
		m_vecRandom.clear();
        m_iGetCount = 0;
        m_iRandMax = 0;
	}

	// 1 ~ RandMax 사이의 임의의 수
	inline int Rand()
	{
		int retVal = m_vecRandom[m_iGetCount--];
		if( m_iGetCount < 5 ) {	
            m_iGetCount = m_iRandMax - 1; 
            RandomShuffule(); 
        }

		return retVal;
	}

	// min ~ max 사이의 임의의 수
	//inline int Rand( int min, int max )
	//{
	//	return ( Rand() % ( max + 1 - min ) ) + min;
	//}

private:
	inline void RandomShuffule()
	{
		random_shuffle( m_vecRandom.begin(), m_vecRandom.end() );
	}

	RANDOM_VEC		m_vecRandom;
	int				m_iRandMax;
	int				m_iGetCount;
};