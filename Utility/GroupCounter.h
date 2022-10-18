#pragma once

//=======================================================================================================================
// Group Counter
/**
@author
	Cho Jae Sik < jscho@webzen.co.kr >	

@since
	2007. 8. 15( ������... T.T )

@remarks
	- �׷�(Ű) ������ �ڵ����� ī��Ʈ ���� �� ����

@ToDo
   - CountType���� �Ϲ� �ڷ����� �ü� �ִ�. ���������� ó������ �ʱ�ȭ ��ų�� 0���� �ʱ�ȭ ��Ų��.
   - GetCount() �Լ��� �ش� Ű���� �������� ������ 0�� �����Ѵ�.

@example
	- GroupCounter< DWORD, DWORD > Counter;
	  Counter.Increse( 1 );

@History
    - 2007�� 8�� 15�� ���� ������..

*/
//=======================================================================================================================


#include <map>

template< class KeyType, class CountType >
class GroupCounter
{
public:
	GroupCounter();
	~GroupCounter();

public:
	void			Increse( KeyType Key );
	void			Decrese( KeyType Key );
	CountType		GetCount( KeyType Key );

private:
	CountType*		Find( KeyType Key );
	void			Release();

private:
	std::map< KeyType, CountType* >		m_mapGroupCounter;
};


template< class KeyType, class CountType >
GroupCounter< KeyType, CountType >::GroupCounter()
{
	m_mapGroupCounter.clear();
}

template< class KeyType, class CountType >
GroupCounter< KeyType, CountType >::~GroupCounter()
{
	Release();
}

template< class KeyType, class CountType >
void	GroupCounter< KeyType, CountType >::Release()
{
	std::map< KeyType, CountType* >::iterator	iter;

	for( iter = m_mapGroupCounter.begin(); iter != m_mapGroupCounter.end(); ++iter) 
	{
		CountType* pCount = iter->second;
		if( pCount )
			delete pCount;
	}

	m_mapGroupCounter.clear();
}


template< class KeyType, class CountType >
CountType*	GroupCounter< KeyType, CountType >::Find( KeyType Key )
{
	std::map< KeyType, CountType* >::iterator	iter;
	iter = m_mapGroupCounter.find( Key );
	if( iter != m_mapGroupCounter.end() )
		return iter->second;

	return NULL;
}

template< class KeyType, class CountType >
void GroupCounter< KeyType, CountType >::Increse( KeyType Key )
{
	CountType* pCount = Find( Key );
	if( pCount )
	{
		(*pCount)++;
		return;
	}

	pCount = new CountType;
	(*pCount) = 0;
	(*pCount)++;
	m_mapGroupCounter.insert( std::make_pair( Key, pCount ) );
	return;
}

template< class KeyType, class CountType >
void GroupCounter< KeyType, CountType >::Decrese( KeyType Key )
{
	CountType* pCount = Find( Key );
	if( pCount )
	{
		if( (*pCount) == 0 )		//ī���Ͱ� 0�̸� ���ҽ�ų�� ����. �Ҹ���Ѿ� �ϴ°�?
			return;

		(*pCount)--;
		return;
	}

	return;
}

template< class KeyType, class CountType >
CountType GroupCounter< KeyType, CountType >::GetCount( KeyType Key )
{
	CountType* pCount = Find( Key );
	if( pCount )
		return (*pCount);

	return 0;
}

