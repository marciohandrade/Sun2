#pragma once

#include "WorkLoad.Common.h"
#include "WorkLoadManager.h"

// 쌍방을 검색하고 관리하기 쉬운 방법을 생각해 볼 것.

template<typename _TYPE >
class WorkLoadContainer
{
	typedef std::pair<WORK_KEY, _TYPE>					_INTER_VALUE_PAIR;
	typedef std::pair<_TYPE, _INTER_VALUE_PAIR>			_INTER_PAIR;
	typedef STLX_MAP<_TYPE, _INTER_VALUE_PAIR>		_INTER_MAP;

public:
	WorkLoadContainer() { Initialize(); }
	~WorkLoadContainer() { Release( TRUE ); }
public:
	VOID			Initialize() { Release(); }
	VOID			Release( BOOL bRelatedObjectRelease = FALSE )
	{
		if( bRelatedObjectRelease )
		{
			while( m_WorkMap.size() )
			{
				_INTER_MAP::iterator it = m_WorkMap.begin();
				_TYPE key = it->first;
				ReleaseWork( key, TRUE );
			}
		}

		m_WorkMap.clear();
	}

	BOOL			RegisterWork( WORK_KEY value_workKey, _TYPE key )
	{
		_INTER_MAP::iterator it = m_WorkMap.find( key );
		if( it != m_WorkMap.end() )
		{
			ASSERT( !"중복된 Work을 등록하려고 했다." );
			return FALSE;
		}
		m_WorkMap.insert( m_WorkMap.end(), _INTER_PAIR( key, _INTER_VALUE_PAIR(value_workKey, key) ) );
		return TRUE;
	}

	WORK_KEY		FindWorkKey( _TYPE key )
	{
		_INTER_MAP::iterator it = m_WorkMap.find( key );
		if( it == m_WorkMap.end() )
			return WorkLoadManager::c_NotExistKey;

		return it->second.first;
	}

	_TYPE			FindWorkKeyPair( WORK_KEY value_workKey )
	{
		_INTER_MAP::iterator it(m_WorkMap.begin()), end(m_WorkMap.end());
		for( ; it != end ; ++it )
		{
			if( it->second.first == value_workKey )
				return it->second.second;
		}
		return NULL;
	}

	VOID			ReleaseWork( _TYPE key, BOOL bRelatedObjectRelease = FALSE )
	{
		_INTER_MAP::iterator it = m_WorkMap.find( key );
		if( it == m_WorkMap.end() )
			return;

		if( bRelatedObjectRelease )
		{
			_INTER_VALUE_PAIR& rPair = it->second;
			WORK_KEY workKey = rPair.first;
			WorkLoadManager::ReleaseTask( workKey, TRUE );
		}

		it = m_WorkMap.find( key );
		if( it == m_WorkMap.end() )
			return;
		m_WorkMap.erase( it );
	}

	VOID			OnMsg( sSIG_MSG* pSIG_MSG )
	{
		_INTER_MAP::iterator it = m_WorkMap.begin();
		_INTER_MAP::iterator itend = m_WorkMap.end();

		for( ; it != itend ; ++it )
		{
			_TYPE key = it->first;
			IWorkNode* pWorkNode = (IWorkNode*)(__int64)key;
			pWorkNode->OnMsg( pSIG_MSG );
		}
	}

private:

private:
	//pair<WORK_KEY, _T>
	// _T가 포인터일 경우, 이것은 참조라고만 생각하며, 해제하지 않는다. 주의할 것
	_INTER_MAP		m_WorkMap;
};

typedef WorkLoadContainer<DWORD>		WORK_LOAD_CONTAINER_SESSION_WORK;

