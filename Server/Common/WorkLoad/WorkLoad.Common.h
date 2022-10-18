#pragma once

#include <ScriptCode.IParser.h>
#include <SolarAuth.Common.h>
#include <SolarAuth.Interfaces.h>

#include <list>
#include <map>
#include <algorithm>

struct MSG_BASE;

//==================================================================================

typedef DWORD		WORK_KEY;

typedef STLX_LIST<WORK_KEY>							WORK_UPDATE_QUEUE;
typedef WORK_UPDATE_QUEUE::iterator					WORK_UPDATE_QUEUE_IT;

typedef Delegator<IWorkNode>						WORK_NODE_DELEGATOR;
typedef std::pair<WORK_NODE_DELEGATOR, BOOL>				WORK_NODE_UPDATE_PAIR;
typedef std::pair<WORK_KEY, WORK_NODE_UPDATE_PAIR	>	WORK_NODE_PAIR;
typedef STLX_MAP<WORK_KEY, WORK_NODE_UPDATE_PAIR>	WORK_NODE_MAP;
typedef WORK_NODE_MAP::iterator						WORK_NODE_MAP_IT;


class WorkLoadManager;
typedef Delegator<WorkLoadManager>				WORK_LOAD_MANAGER;


// �ٸ� ��忡�� ���񽺷� ��ȯ�Ǿ��� ��, �۾��� ����
class ISession4StartServiceWorkNode
{
public:
	virtual BOOL	IsValidObject( DWORD dwCheckValue ) = 0;
	virtual BOOL	IsValidState( DWORD dwStatusValue ) = 0;
	virtual BOOL	SendPacket4Interface( MSG_BASE* pMsg, WORD wSize ) = 0;
};

class PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE : public IWorkNode
{
public:
	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE() {}
	~PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE() {}

public:
	VOID					SetInformation( ISession4StartServiceWorkNode* pObject, DWORD dwCheckKey, DWORD dwStatusValue )
	{
		m_pIChecker = pObject;
		m_dwCheckValue = dwCheckKey;
		m_dwStatusValue = dwStatusValue;
	}

public:
	// <INTERFACE><IMPLEMENTATION>
	virtual BOOL	DoInit();

	virtual BOOL	DoProcess()
	{
		return _ACTION_Always_Alive();
	}

	virtual BOOL	IsComplete()
	{
		return m_bCompleted;
	}

	virtual BOOL	DoRelease()
	{
		return TRUE;
	}

private:
	// SERVICE_FUNCTION_ALWAYS_ALIVE
	BOOL	_ACTION_Always_Alive();
	// SERVICE_FUNCTION_SCRIPT_DATA_LOAD
	BOOL	_ACTION_Script_Data_Load( BOOL bFirst = FALSE );

private:
	VOID	SendPacket_4ScriptData( eSCRIPT_CODE scriptCode );
	VOID	SetLoadScriptTable( eSCRIPT_CODE scriptCode );

private:
	ISession4StartServiceWorkNode*	m_pIChecker;
	DWORD							m_dwCheckValue;
	DWORD							m_dwStatusValue;
	BOOL							m_bCompleted;
	struct
	{
		BOOL						m_bLoadMonitoring;
		DWORD						m_dwPeer;
		DWORD						m_dwTail;
		BYTE						m_pLoadScriptTable[SCRIPT_LIST_SIZE];

		// METHODs
		inline DWORD			GetNextIndex( DWORD dwIndex )
		{  return ((dwIndex+1)%SCRIPT_LIST_SIZE); }
		inline BOOL				IsEmpty() { return (BOOL)( m_dwPeer == m_dwTail ); }
		inline BOOL				IsFull() { return (BOOL)( m_dwPeer == GetNextIndex(m_dwTail) ); }
		// no check "IsEmpty" & "IsFull"
		inline VOID				EnQueue( BYTE byScriptCode )
		{
			m_pLoadScriptTable[m_dwTail] = byScriptCode;
			m_dwTail = GetNextIndex( m_dwTail );
		}

		inline BYTE				DeQueue()
		{
			BYTE value = m_pLoadScriptTable[m_dwPeer];
			m_dwPeer = GetNextIndex(m_dwPeer);
			return value;
		}

		inline BYTE				Peek() { return m_pLoadScriptTable[m_dwPeer]; }
	} LOADING_INFO;
};

