#pragma once
//----------------------------------------------------------------------------
// 신뢰할 수 있는 확률 추출기 확장
// 
// @author	Kim Chang Hyun < shogen@webzen.co.kr >
// @since	2007. 8. 24
// @remarks
// - 확률의 확률범의를 위한 클래스
//----------------------------------------------------------------------------
template< DWORD nSeperator, class Type = DWORD, class ListType = INT, typename KeyType = DWORD >
class ReliableRatioRandomizerEX
{
public:
	ReliableRatioRandomizerEX(void)
	{
		Clear();
	}
	~ReliableRatioRandomizerEX(void){}

	BOOL AddRatio( KeyType Key, Type numerator, Type denominator );
	VOID SetMin( ListType Value, DWORD Num );
	VOID SetMax( ListType Value, DWORD Num );
	ListType GetRandomizedValue();
	VOID Clear();

private:
	ListType m_Min[nSeperator];
	ListType m_Max[nSeperator];
	ReliableRatioRandomizer<nSeperator, Type, KeyType> m_Randomizer;
};

template< DWORD nSeperator, class Type, class ListType, typename KeyType >
BOOL ReliableRatioRandomizerEX<nSeperator, Type, ListType, KeyType>::AddRatio( KeyType Key, Type numerator, Type denominator )
{
	return m_Randomizer.AddRatio( Key, numerator, denominator );	
}


template< DWORD nSeperator, class Type, class ListType, typename KeyType >
VOID ReliableRatioRandomizerEX<nSeperator, Type, ListType, KeyType>::SetMin( ListType Value, DWORD Num )
{	
	if( Num == 0 || Num > nSeperator )
	{
		ASSERT(FALSE);
		return;
	}

	m_Min[Num - 1] = Value;	
}

template< DWORD nSeperator, class Type, class ListType, typename KeyType >
VOID ReliableRatioRandomizerEX<nSeperator, Type, ListType, KeyType>::SetMax( ListType Value, DWORD Num )
{
	if( Num == 0 || Num > nSeperator )
	{
		ASSERT(FALSE);
		return;
	}

	m_Max[Num - 1] = Value;
}

template< DWORD nSeperator, class Type, class ListType, typename KeyType >
ListType ReliableRatioRandomizerEX<nSeperator, Type, ListType, KeyType>::GetRandomizedValue()
{
	KeyType Key = m_Randomizer.RandomizedKey();
	ListType Min = m_Min[Key-1];
	ListType Max = m_Max[Key-1];

	return random(Min, Max);
} 

template< DWORD nSeperator, class Type, class ListType, typename KeyType >
VOID ReliableRatioRandomizerEX<nSeperator, Type, ListType, KeyType>::Clear()
{
	memset( m_Min, 0, sizeof(ListType)*nSeperator );
	memset( m_Max, 0, sizeof(ListType)*nSeperator );

	m_Randomizer.Clear();
}
