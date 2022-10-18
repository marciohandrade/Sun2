#pragma once

// BIT Operation
#define MAX_BITS(byCount)	(byCount<<3)

template<DWORD max_size>
class BitOperation
{
public:
	inline VOID ClearAll() { memset( m_Array, 0, sizeof(m_Array) ); }
	inline BOOL operator [] ( DWORD dwIndex ) { return IsSet( dwIndex ); }

	inline BOOL IsSet( DWORD dwIndex )
	{
		ASSERT( dwIndex < max_size );
		if( !(dwIndex<max_size) ) return FALSE;

		return (BOOL)(m_Array[dwIdx>>5] & (1UL<<(dwIdx&31)));
	}

	inline VOID Set( DWORD dwIndex )
	{
		ASSERT( dwIndex < max_size );
		if( !(dwIndex<max_size) ) return;

		(m_Array)[dwIndex >> 5] |= (1UL << (dwIndex & 31));
	}

	inline VOID UnSet( DWORD dwIndex )
	{
		ASSERT( dwIndex < max_size );
		if( !(dwIndex<max_size) ) return;

		(m_Array)[dwIndex >> 5] &= ~(1UL << (dwIndex & 31));
	}

private:
	DWORD	m_Array[ ( (max_size)/MAX_BITS(sizeof(DWORD)) ) + 1 ];
};

