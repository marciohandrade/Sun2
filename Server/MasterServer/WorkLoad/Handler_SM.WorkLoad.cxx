// no compile


// 다른 모드에서 서비스로 전환되었을 때, 작업할 내역
class PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE : public IWorkNode
{
public:
	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE() {}
	~PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE() {}

public:
	SolarAuthServerSession*	GetServerSession() { return m_pServerSession; }
	VOID					SetServerSession( SolarAuthServerSession* pSession )
	{
		m_pServerSession = pSession;
		m_GLOBAL_SERVER_KEY = m_pServerSession->GlobalServerInfo().m_GlobalServerKey;
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
	SolarAuthServerSession*		m_pServerSession;
	GLOBAL_SERVER_KEY			m_GLOBAL_SERVER_KEY;
	BOOL						m_bCompleted;
	struct
	{
		BOOL					m_bLoadMonitoring;
		DWORD					m_dwPeer;
		DWORD					m_dwTail;
		BYTE					m_pLoadScriptTable[SCRIPT_LIST_SIZE];

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



BOOL	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::DoInit()
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
BOOL	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::_ACTION_Always_Alive()
{
	if( !LOADING_INFO.IsEmpty() )
	{
		eSCRIPT_CODE scriptCode = (eSCRIPT_CODE)LOADING_INFO.Peek();
		// LOADING->LOADING의 상황을 고려한다면...
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
BOOL	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::_ACTION_Script_Data_Load( BOOL bFirst )
{
	SolarAuthServerSession* pSession = GetServerSession();
	// 아래와 같이 사용하는 것은 해당 서버타입을 해제시에 TEMP_SERVER로 전환하기 때문이다.
	if( ! (pSession
		&& SOLARAUTH_SERVER == pSession->GetServerType()
		&& m_GLOBAL_SERVER_KEY == pSession->GlobalServerInfo().m_GlobalServerKey) )
		return FALSE;

	if( ScriptDataManager::GetNeedLoadDataCount() )
	{
		ScriptDataManager::ForEach_NeedLoadData(
			&PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::SetLoadScriptTable,
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
VOID	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::SetLoadScriptTable(
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

	ASSERT( !"Queue 크기와 스크립트 종류 등등을 잘 확인해 주셈!" );
}

//
VOID	PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE::SendPacket_4ScriptData(
								eSCRIPT_CODE scriptCode
								)
{
	MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN msgSYN;
	msgSYN.m_wDataType = (WORD)scriptCode;

	if( SCRIPT_CONTROL_FLAG_UPPERBOUNDARY > scriptCode )
	{
		__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "Check \tscriptCode %d\n", scriptCode ); );
	}
	// Server Session에 대한 검증은 _ACTION_Script_Data_Load 에서 수행했으므로 문제없다. 
	GetServerSession()->SendPacket( &msgSYN, sizeof(msgSYN) );
}


//
//
//
//
//
//












