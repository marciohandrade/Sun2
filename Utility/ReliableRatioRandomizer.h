#pragma once

//=============================================================================================================================
/// �ŷ��� �� �ִ� Ȯ�� ����� Class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 7. 13
	@remarks
		- ��Ȯ��(!) Ȯ�� ����ȭ(normalization)�� ������ Ŭ����
	@note
		- ������,�� ��� Ȯ�� ��꿡 ����� �� �ִ�.
	@todo
		- ��踦 ���� �߰� �޼��� �ۼ��� �ʿ� ����
		- ����ȭ �۾� : RandomizedKey()�ÿ� Ȯ���� ���� ������ ������������ �̸� ������ �ʿ䰡 ����
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

		ReliableRatioRandomizer<10> m_ratioSelect;						//< �ִ� �Է��� ���� ���� = 10
		or
		ReliableRatioRandomizer<10, DWORD, DWORD> m_ratioSelect;


		// ���� �Է�
		// param1 : Ű
		// param2 : ����
		// param3 : �и�

		assert( FALSE == m_ratioSelect.AddRatio( 0, 30, 100 ) );		//< Ű�� 0�� ����� �� ����.
		assert( TRUE == m_ratioSelect.AddRatio( DROP_NORMALITEM, 13, 100 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_MONEY, 15, 1000 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_RAREITEM, 2, 1000000 ) );
		assert( TRUE == m_ratioSelect.AddRatio( DROP_WASTEITEM, 20, 1000 ) );

		// ���� ���
		for(int i = 0 ;i < 1000 ; ++i )
		{
			DWORD key = m_ratioSelect.RandomizedKey();
			switch( key )
			{
			case 0: //< N/A
				break;
			default:
				{
					// ���𰡰� ��������.
				}
			}
		}

*/
//=============================================================================================================================

#include <Methods.h>
#include <Misc.h>


// �ŷ��� �� �ִ� ���� �����
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


	// ������ ���� Ű�� ����
	KeyType RandomizedKey(BOOL bDebug= FALSE);
    // (CHANGES) (f100612.3L) add an interface for an added stream array
    const KeyTypeArray& GetKeyTypeArray() const { return m_Key; }

	// �� �߱��� Ȯ��
	inline DWORD GetTotalRatio()
	{
		return (m_nReload*m_MaxTotalRatio+(m_MaxTotalRatio-m_CurTotalRatio));
	}
	// The Key�� ���� �� �߱� ����
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
	// == 0�� �Ǹ� Reload()
	Type m_CurTotalRatio;
	Type m_MaxTotalRatio;
	Type m_Ratio[nSeperator];		//< Ư��Index�� ���� ��ü Ȯ���� ���� �κ� ����
	Type m_MaxRatio[nSeperator];	//< Ư��Index�� ���� ��ü Ȯ���� ���� �κ� ����
	KeyTypeArray m_Key;		//< Ư��Index�� ���� ������ Ű
	DWORD m_freeIndex;

	// ��踦 ���� �ɹ�
	DWORD m_nReload;
};

template< DWORD nSeperator, class Type, typename KeyType >
BOOL ReliableRatioRandomizer<nSeperator, Type, KeyType>::AddRatio(KeyType Key, Type numerator, Type denominator)
{
	ASSERT( m_freeIndex < nSeperator  );
	ASSERT( Key != 0 );

	ASSERT( m_Ratio[m_freeIndex] == 0 );
	ASSERT( m_CurTotalRatio == m_MaxTotalRatio ); //< ����ϱ����� AddRatio�� �̷������ �Ѵ�.
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
	// �ʱ�ȭ �س��� ���� ������
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
