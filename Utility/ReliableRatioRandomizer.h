#pragma once

//=============================================================================================================================
/// 신뢰할 수 있는 확률 추출기 Class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 7. 13
	@remarks
		- 정확한(!) 확률 정규화(normalization)가 가능한 클래스
	@note
		- 아이템,돈 드랍 확률 계산에 사용할 수 있다.
	@todo
		- 통계를 위한 추가 메서드 작성할 필요 있음
		- 최적화 작업 : RandomizedKey()시에 확률이 높은 순서로 내림차순으로 미리 정렬할 필요가 있음
	@history
		- n/a
	@usage
		-
		enum DROP_TYPE
		{
			DROP_NOTHING = 0,
			DROP_NORMALITEM,
			DROP_MONEY,
			DROP_RAREITEM,
			DROP_WASTEITEM,
		}

		ReliableRatioRandomizer<10> m_ratioSelect;						//< 최대 입력할 비율 개수 = 10
		or
		ReliableRatioRandomizer<10, DWORD, DWORD> m_ratioSelect;


		// 비율 입력
		// param1 : 키
		// param2 : 분자
		// param3 : 분모

		assert( FALSE == m_ratioSelect.AddRatio( 0, 30, 100 ) );		//< 키로 0을 사용할 수 없다.
		assert( TRUE == m_ratioSelect.AddRatio( DROP_NORMALITEM, 13, 100 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_MONEY, 15, 1000 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_RAREITEM, 2, 1000000 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_WASTEITEM, 20, 1000 ) );

		// 실제 사용
		for(int i = 0 ;i < 1000 ; ++i )
		{
			DWORD key = m_ratioSelect.RandomizedKey();
			switch( key )
			{
			case 0: //< N/A
				break;
			default:
				{
					// 무언가가 떨어진다.
				}
			}
		}

*/
//=============================================================================================================================

#include <Methods.h>
#include <Misc.h>


// 신뢰할 수 있는 비율 추출기
template< DWORD nSeperator, class Type = DWORD, typename KeyType = DWORD >
class ReliableRatioRandomizer
{
public:
    // (CHANGES) (f100612.3L) add types
    static const DWORD kNumberOfSeperators = nSeperator;
    typedef KeyType key_type;
    typedef key_type KeyTypeArray[kNumberOfSeperators];

	ReliableRatioRandomizer(void):
	  m_nReload(0)
	{
		Clear();
	}
	~ReliableRatioRandomizer(void){}

	VOID Clear()
	{
		m_CurTotalRatio = 0;
		m_MaxTotalRatio = 0;
		m_freeIndex = 0;
		m_nReload = 0;
		memset( m_Ratio, 0, sizeof(Type)*nSeperator );
		memset( m_MaxRatio, 0, sizeof(Type)*nSeperator );
		memset( m_Key, 0, sizeof(KeyType)*nSeperator );
	}

	BOOL AddRatio( KeyType Key, Type numerator, Type denominator );


	// 랜덤한 수의 키를 리턴
	KeyType RandomizedKey(BOOL bDebug= FALSE);
    // (CHANGES) (f100612.3L) add an interface for an added stream array
    const KeyTypeArray& GetKeyTypeArray() const { return m_Key; }

	// 총 발급한 확률
	inline DWORD GetTotalRatio()
	{
		return (m_nReload*m_MaxTotalRatio+(m_MaxTotalRatio-m_CurTotalRatio));
	}
	// The Key에 대한 총 발급 비율
	inline DWORD GetRatioForTheKey( KeyType Key )
	{
		DWORD index = m_freeIndex;  
		for( DWORD i = 0 ; i < m_freeIndex ; ++i )
		{
			if( m_Key[i] ==  Key )
			{
				index = i;
				break;
			}
		}
		if( index == m_freeIndex ) return 0;
		return (m_nReload*m_MaxRatio[index]+(m_MaxRatio[index]-m_Ratio[index]));
	}

private:
	VOID Reload();
	inline DWORD GetIndexForTheKey( KeyType Key )
	{
		DWORD index = m_freeIndex;  
		for( DWORD i = 0 ; i < m_freeIndex ; ++i )
		{
			if( m_Key[i] ==  Key )
			{
				index = i;
				break;
			}
		}

	}	
	BOOL UpdateRatio( Type prelcm, Type lcm, Type numerator, Type denominator );

private:
	// == 0이 되면 Reload()
	Type m_CurTotalRatio;
	Type m_MaxTotalRatio;
	Type m_Ratio[nSeperator];		//< 특정Index에 대한 전체 확률에 대한 부분 비율
	Type m_MaxRatio[nSeperator];	//< 특정Index에 대한 전체 확률에 대한 부분 비율
	KeyTypeArray m_Key;		//< 특정Index에 대해 지정된 키
	DWORD m_freeIndex;

	// 통계를 위한 맴버
	DWORD m_nReload;
};

template< DWORD nSeperator, class Type, typename KeyType >
BOOL ReliableRatioRandomizer<nSeperator, Type, KeyType>::AddRatio(KeyType Key, Type numerator, Type denominator)
{
	ASSERT( m_freeIndex < nSeperator  );
	ASSERT( Key != 0 );

	ASSERT( m_Ratio[m_freeIndex] == 0 );
	ASSERT( m_CurTotalRatio == m_MaxTotalRatio ); //< 사용하기전에 AddRatio가 이루어져야 한다.
	for( DWORD i = 0 ; i < m_freeIndex ; ++i )
	{
		ASSERT( m_Key[i] != Key );
	}

	Type lcm = 0; 
	if( 0 == m_MaxTotalRatio )
		lcm = denominator;
	else
		lcm = LCM( denominator, m_MaxTotalRatio );

	m_CurTotalRatio = m_MaxTotalRatio = lcm;
	ASSERT( m_CurTotalRatio != 0 );

	m_Key[m_freeIndex] = Key;

	++m_freeIndex;

	return UpdateRatio( m_MaxTotalRatio, lcm, numerator, denominator );
}

template< DWORD nSeperator, class Type, typename KeyType >
BOOL ReliableRatioRandomizer<nSeperator, Type, KeyType>::UpdateRatio( Type prelcm, Type lcm, Type numerator, Type denominator )
{
	--m_freeIndex;
	ASSERT( m_CurTotalRatio == m_MaxTotalRatio );
	ASSERT( m_Ratio[m_freeIndex] == 0 && m_MaxRatio[m_freeIndex] == 0 );

	Type CalcRatio = 0;
	DWORD i = 0;
	for( i = 0 ; i < m_freeIndex ; ++i )
	{
		//ASSERT( m_MaxRatio[i] == m_Ratio[i] );
		m_MaxRatio[i] = m_Ratio[i] *= (lcm/prelcm);
		CalcRatio += m_MaxRatio[i];
	}
	m_MaxRatio[i] = m_Ratio[i] = numerator*(lcm/denominator);
	CalcRatio += m_MaxRatio[i];
	
	++m_freeIndex;

	if( CalcRatio > lcm ) return FALSE;

	return TRUE;
}

template< DWORD nSeperator, class Type, typename KeyType >
KeyType ReliableRatioRandomizer<nSeperator, Type, KeyType>::RandomizedKey(BOOL bDebug)
{
    __UNUSED(bDebug);
	// 초기화 해놓고 값이 없을때
	if( 0 == m_MaxTotalRatio ) return 0;

	if( 0 == m_CurTotalRatio ) Reload();

	ASSERT( m_CurTotalRatio != 0 );

	KeyType luckyNumber = 0;
	Type FromPercent	= 0;
	Type ToPercent		= 0;

	//Type seed = __random( 0, m_CurTotalRatio - 1 );

	//Type seed = rand() % ( m_CurTotalRatio );
	Type seed = (Type)dRandom( 0, m_CurTotalRatio-1 );
	--m_CurTotalRatio;

	for( DWORD i = 0 ; i < m_freeIndex ; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += m_Ratio[i];
		if(FromPercent <= seed && seed < ToPercent)
		{
			ASSERT(m_Ratio[i]!=0);
			--m_Ratio[i];
			luckyNumber = m_Key[i];
			break;
		}
	}

	return luckyNumber;
}

template< DWORD nSeperator, class Type, typename KeyType >
VOID ReliableRatioRandomizer<nSeperator, Type, KeyType>::Reload()
{
	ASSERT( m_CurTotalRatio == 0 );
	ASSERT( m_MaxTotalRatio != 0 );
	++m_nReload;
	m_CurTotalRatio = m_MaxTotalRatio;
	memcpy( m_Ratio, m_MaxRatio, sizeof(Type)*nSeperator );
}
