#pragma once

//=======================================================================================================================
// Group Counter
/**
@author
	Cho Jae Sik < jscho@webzen.co.kr >	

@since
	2007. 8. 15( 광복절... T.T )

@remarks
	- 그룹(키) 값별로 자동으로 카운트 유지 및 관리

@ToDo
   - CountType에는 일반 자료형만 올수 있다. 내부적으로 처음사용시 초기화 시킬때 0으로 초기화 시킨다.
   - GetCount() 함수는 해당 키값이 존재하지 않으면 0를 리턴한다.

@example
	- GroupCounter< DWORD, DWORD > Counter;
	  Counter.Increse( 1 );

@History
    - 2007년 8월 15일 최초 릴리즈..

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
		if( (*pCount) == 0 )		//카운터가 0이면 감소시킬수 없다. 소멸시켜야 하는가?
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

