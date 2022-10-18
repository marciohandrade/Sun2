
#include "stdafx.h"
#include <PacketStruct_ServerCommon.h>

#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoad.Common.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

BOOL	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::DoInit()
{
	m_bCompleted = FALSE;
	LOADING_INFO.m_bLoadMonitoring = FALSE;
	LOADING_INFO.m_dwPeer = 0;	
	LOADING_INFO.m_dwTail = 0;
	memset( LOADING_INFO.m_pLoadScriptTable, 0, sizeof(LOADING_INFO.m_pLoadScriptTable) );

	if( _ACTION_Script_Data_Load( TRUE ) )
		if( _ACTION_Always_Alive() )
			return TRUE;

	return FALSE;
}

// <INITIALIZER by CATEGORIES>
//

// SERVICE_FUNCTION_ALWAYS_ALIVE
BOOL	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::_ACTION_Always_Alive()
{
	if( ! (m_pIChecker && m_pIChecker->IsValidObject( m_dwCheckValue ) ) )
		return FALSE;

	if( !m_pIChecker->IsValidState( m_dwStatusValue ) )
		return TRUE;

	if( !LOADING_INFO.IsEmpty() )
	{
		eSCRIPT_CODE scriptCode = (eSCRIPT_CODE)LOADING_INFO.Peek();
		if( ScriptDataManager::IsLoadedData( scriptCode ) )
		{
			LOADING_INFO.DeQueue();
			if( !LOADING_INFO.IsEmpty() )
			{
				scriptCode = (eSCRIPT_CODE)LOADING_INFO.Peek();
				SendPacket_4ScriptData( scriptCode );
			}
		}
	}

	if( ScriptDataManager::GetNeedLoadDataCount() )
	{
		_ACTION_Script_Data_Load();
	}
	return TRUE;
}

// SERVICE_FUNCTION_SCRIPT_DATA_LOAD
BOOL	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::_ACTION_Script_Data_Load( BOOL bFirst )
{
	if( ! (m_pIChecker && m_pIChecker->IsValidObject( m_dwCheckValue ) ) )
		return FALSE;

	if( !m_pIChecker->IsValidState( m_dwStatusValue ) )
		return TRUE;

	if( ScriptDataManager::GetNeedLoadDataCount() )
	{
		ScriptDataManager::ForEach_NeedLoadData(
			&PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::SetLoadScriptTable,
			this);

		// Change status all, empty->loading
		ScriptDataManager::DataStatusCommand(
			SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ALL | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING
			);

		if( bFirst )
		{
			eSCRIPT_CODE scriptCode = (eSCRIPT_CODE)LOADING_INFO.Peek();
			SendPacket_4ScriptData( scriptCode );
		}
	}

	return TRUE;
}

//
VOID	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::SetLoadScriptTable(
	eSCRIPT_CODE scriptCode
	)
{
	if( SAFE_NUMERIC_TYPECAST( eSCRIPT_CODE, scriptCode, BYTE ) )
	{
		if( !LOADING_INFO.IsFull() )
		{
			LOADING_INFO.EnQueue( (BYTE)scriptCode );

			if( !LOADING_INFO.m_bLoadMonitoring )
				LOADING_INFO.m_bLoadMonitoring = TRUE;

			return;
		}
	}

	ASSERT( !"Queue ũ��� ��ũ��Ʈ ���� ����� �� Ȯ���� �ּ�!" );
}

//
VOID	PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE::SendPacket_4ScriptData(
	eSCRIPT_CODE scriptCode
	)
{
	MSG_SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN msgSYN;
	msgSYN.m_wDataType = (WORD)scriptCode;

	// Server Session�� ���� ������ _ACTION_Script_Data_Load ���� ���������Ƿ� ��������. 
	m_pIChecker->SendPacket4Interface( &msgSYN, sizeof(msgSYN) );
}


