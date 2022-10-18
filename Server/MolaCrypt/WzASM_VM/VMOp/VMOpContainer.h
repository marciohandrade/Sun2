#pragma once

#include "VMOp.h"
#include "VMOpCPU.h"

// �ֹ��� �˻��ϰ� �����ϱ� ���� ����� ������ �� ��.

template<typename _TYPE >
class VMOpContainer
{
	typedef std::pair<WORK_KEY, _TYPE>									_INTER_VALUE_PAIR;
	typedef std::pair< _TYPE, _INTER_VALUE_PAIR >						_INTER_PAIR;
	typedef STLX_MAP< _TYPE, _INTER_VALUE_PAIR >			_INTER_MAP;

public:
	VMOpContainer() { Initialize(); }
	~VMOpContainer() { Release( TRUE ); }
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
			ASSERT( !"�ߺ��� Work�� ����Ϸ��� �ߴ�." );
			return FALSE;
		}
		m_WorkMap.insert( m_WorkMap.end(), _INTER_PAIR( key, _INTER_VALUE_PAIR(value_workKey, key) ) );
		return TRUE;
	}

	WORK_KEY		FindWorkKey( _TYPE key )
	{
		_INTER_MAP::iterator it = m_WorkMap.find( key );
		if( it == m_WorkMap.end() )
			return VMOpCPU::c_NotExistKey;

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
			VMOpCPU::ReleaseTask( workKey, TRUE );
		}

		it = m_WorkMap.find( key );
		if( it == m_WorkMap.end() )
			return;
		m_WorkMap.erase( it );
	}

	VOID			OnMsg( eMOLASIG_MSG* pSIG_MSG )
	{
		_INTER_MAP::iterator it = m_WorkMap.begin();
		_INTER_MAP::iterator itend = m_WorkMap.end();

		for( ; it != itend ; ++it )
		{
			_TYPE key = it->first;
			IVMOp* pWorkNode = (IVMOp*)(__int64)key;
			pWorkNode->OnMsg( pSIG_MSG );
		}
	}

private:

private:
	//pair<WORK_KEY, _T>
	// _T�� �������� ���, �̰��� ������� �����ϸ�, �������� �ʴ´�. ������ ��
	_INTER_MAP		m_WorkMap;
};

typedef VMOpContainer<DWORD>		WORK_LOAD_CONTAINER_SESSION_WORK;

