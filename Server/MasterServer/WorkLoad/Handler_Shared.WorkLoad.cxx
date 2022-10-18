// no compile embedded file

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

class PROCESS_DATA_TRANSFER_SENDDATA_CMD : public IWorkNode
{
private:
	static const DWORD	c_dwMaxTokenSize =
		MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO::MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE;
public:
	PROCESS_DATA_TRANSFER_SENDDATA_CMD();
	~PROCESS_DATA_TRANSFER_SENDDATA_CMD();

public:
	ServerSessionEx*		GetServerSessionEx() { return m_pServerSessionEx; }
	VOID				SetServerSessionEx( ServerSessionEx* pSession );

	// pSTREAM은 내부에서 해제하지 않는다. 이것은 공유하는 버퍼일 가능성이 있다. 이후 Delegator처리로?
	BOOL	SetInformation( const BYTE* pSTREAM, const DWORD dwBufSize, const DWORD dwCRC, const WORD wScriptNo );
public:
	// <INTERFACE><IMPLEMENTATION>
	virtual BOOL	DoInit();
	virtual BOOL	DoProcess();
	virtual BOOL	IsComplete();
	virtual BOOL	DoRelease();

private:
	inline BOOL		CheckOverflow( const DWORD dwCurrRemainedSize, const DWORD dwSendingSize );

private:
	ServerSessionEx*				m_pServerSessionEx;
	eSERVER_TYPE				m_ServerType;
	struct	// 네트웍 버퍼 오버플로를 제어하기 위한 타이머 설정
	{
		BOOL						m_bNetStreamTimerEnabled;
		DWORD						m_NetStreamTimer;
	};
	struct	// 완료조건 설정 필드
	{
		BOOL						m_bHeadSended;
		DWORD						m_dwCompleteCount;
		DWORD						m_dwProcessed;
	};
	BYTE*						m_pSTREAM;
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER*
		m_pTOKEN_INFO_HEADER;
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_FILE_INFO
		m_FILE_INFO;
};


PROCESS_DATA_TRANSFER_SENDDATA_CMD::PROCESS_DATA_TRANSFER_SENDDATA_CMD()
{
	m_pServerSessionEx			= NULL;
	m_ServerType				= TEMP_SERVER;
	m_bNetStreamTimerEnabled	= FALSE;
	m_NetStreamTimer			= 0;
	m_bHeadSended				= FALSE;
	m_dwCompleteCount			= 0;
	m_dwProcessed				= 0;
	m_pSTREAM					= NULL;
	m_pTOKEN_INFO_HEADER		= NULL;
	memset( &m_FILE_INFO, 0, sizeof(MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_FILE_INFO) );
}

PROCESS_DATA_TRANSFER_SENDDATA_CMD::~PROCESS_DATA_TRANSFER_SENDDATA_CMD()
{
	//DoRelease()
}

VOID	PROCESS_DATA_TRANSFER_SENDDATA_CMD::SetServerSessionEx(
						ServerSessionEx* pSession
						)
{
	m_pServerSessionEx = pSession;
	m_ServerType = pSession->GetServerType();
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::SetInformation(
							const BYTE* pSTREAM,
							const DWORD dwBufSize,
							const DWORD dwCRC,
							const WORD wScriptNo
							)
{
	if( !pSTREAM || !dwBufSize )
		return FALSE;

	const DWORD tokens = (DWORD)(dwBufSize/c_dwMaxTokenSize);
	const DWORD remainSz = (DWORD)(dwBufSize%c_dwMaxTokenSize);
	const DWORD total_tokens = tokens + (DWORD)(!!remainSz);

	m_dwCompleteCount = total_tokens;
	m_pSTREAM = (BYTE*)pSTREAM;

	m_FILE_INFO.m_dwBufferSize = dwBufSize;
	m_FILE_INFO.m_wDataType = wScriptNo;
	m_FILE_INFO.m_dwCRC = dwCRC;

	return TRUE;
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::CheckOverflow(
														const DWORD dwCurrRemainedSize,
														const DWORD dwSendingSize
														)
{
	if( dwCurrRemainedSize < dwSendingSize+1000 )
	{
		__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "Remained Size dwCurrRemainedSize %u\tdwSendingSize+1000 %u\n", dwCurrRemainedSize, dwSendingSize+1000 ); );
		m_NetStreamTimer = GetTickCount();
		m_bNetStreamTimerEnabled = TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::DoInit()
{
	if( m_pServerSessionEx == NULL )
		return FALSE;

	m_pTOKEN_INFO_HEADER =
		new MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER[m_dwCompleteCount];

	memset(
		m_pTOKEN_INFO_HEADER,
		0,
		sizeof(MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER)*m_dwCompleteCount
		);

	DWORD dwLimit = m_dwCompleteCount-1;
	for( DWORD i=0 ; i<dwLimit ; ++i )
	{
		MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER& rHeader =
			m_pTOKEN_INFO_HEADER[i];
		rHeader.m_wDataType = (WORD)m_FILE_INFO.m_wDataType;
		rHeader.m_wTokenNo = (WORD)i;
		rHeader.m_wBufferSize = c_dwMaxTokenSize;
	}

	DWORD dwRemains = m_FILE_INFO.m_dwBufferSize % c_dwMaxTokenSize;
	dwRemains = dwRemains ? dwRemains : c_dwMaxTokenSize;
	m_pTOKEN_INFO_HEADER[dwLimit].m_wDataType = (WORD)m_FILE_INFO.m_wDataType;
	m_pTOKEN_INFO_HEADER[dwLimit].m_wBufferSize = (WORD)dwRemains;
	m_pTOKEN_INFO_HEADER[dwLimit].m_wTokenNo = (WORD)dwLimit;

	return TRUE;
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::DoProcess()
{
	ServerSessionEx* pSession = GetServerSessionEx();
	if( ! (pSession
			&& m_ServerType == pSession->GetServerType()
			/*&& m_GLOBAL_SERVER_KEY == pSession->GetGlobalServerKey()*/
			) )
		return FALSE;

	//---------------------------------------------------------
	// Delayed Control by protect network buffer overflow
	if( m_bNetStreamTimerEnabled )
	{
		DWORD dwCurTick = GetTickCount();
		if( m_NetStreamTimer + 10000 < dwCurTick )
		{
			m_bNetStreamTimerEnabled = FALSE;
		}
		else
			return TRUE;	//작업은 수행하지 않지만...
	}

	NetworkObject::BUFFER_INFO bufferINFO;
	pSession->GetSendBufferInfo( bufferINFO );

	// dwRemainBuffer은 이후 세부 제어에서 컨트롤을 수행한다.
	DWORD dwRemainBuffer = bufferINFO.m_dwMaxBufferSize - bufferINFO.m_dwCurrentUsedSize;
	//---------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD =
		new ( PACKET_CONTROL_SM::GetBasePacketPtr4Send() ) MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD();

	if( m_bHeadSended )
	{
		// Network/Session접근할 수 있으면 좋을 텐데... 해도 될까? 음...
		DWORD dwLimitCount = m_dwCompleteCount-1;
		pMsgCMD->m_wControlCmd = MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA;

		MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER& rHeader =
			m_pTOKEN_INFO_HEADER[m_dwProcessed];
		pMsgCMD->m_TOKEN_INFO.m_Header = rHeader;

		// BufferCheck
		if( !CheckOverflow( dwRemainBuffer, pMsgCMD->GetSize() ) )
			return TRUE;

		++m_dwProcessed;
		do
		{
			DWORD dwOffset = rHeader.m_wTokenNo*c_dwMaxTokenSize/*rHeader.m_wBufferSize*/;
			eSCRIPT_DATA_CMD cmd = SCRIPT_DATA_CMD::GET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;
			BYTE* pSTREAM = NULL;
			DWORD dwSize = 0;
			ScriptDataManager::DataStreamCommand( cmd, (eSCRIPT_CODE)m_FILE_INFO.m_wDataType, pSTREAM, dwSize );

			if( pSTREAM != NULL )
			{// Offset처리 신경쓸 것.
				memcpy( pMsgCMD->m_TOKEN_INFO.m_pSTREAM, pSTREAM + dwOffset, rHeader.m_wBufferSize );
				//if( SCRIPT_CODE_KEY(FIELD) == rHeader.m_wDataType )
				//{
				//	printf( (CHAR*)pMsgCMD->m_TOKEN_INFO.m_pSTREAM );
				//}
				pSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );
			}
		} while(0);

		// 종결자
		if( m_dwCompleteCount == m_dwProcessed )
		{
			pMsgCMD->m_wControlCmd = MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE;
			pMsgCMD->m_FILE_CLOSURE.m_wDataType = m_FILE_INFO.m_wDataType;
			pMsgCMD->m_FILE_CLOSURE.m_dwCRC = m_FILE_INFO.m_dwCRC;

			pSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );
		}
	}
	else
	{
		pMsgCMD->m_wControlCmd = MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER;

		// BufferCheck
		if( !CheckOverflow( dwRemainBuffer, pMsgCMD->GetSize() ) )
			return TRUE;

		pMsgCMD->m_FILE_INFO = m_FILE_INFO;
		pSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );
		m_bHeadSended = TRUE;
	}

	return TRUE;
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::IsComplete()
{
	if( m_dwCompleteCount == m_dwProcessed )
		return TRUE;

	return FALSE;
}

BOOL	PROCESS_DATA_TRANSFER_SENDDATA_CMD::DoRelease()
{
	ServerSessionEx* pSession = GetServerSessionEx();
	if( pSession
		&& (m_ServerType == pSession->GetServerType())
		&& (pSession->Services().GetSyncKey() == m_ServerType)
		/*&& m_GLOBAL_SERVER_KEY == pSession->GetGlobalServerKey()*/
		)
	{
		Services4Session& rSERVICE = pSession->Services();
		rSERVICE.GetWorkLoadContainer().ReleaseWork( (DWORD)(__int64)this );
	}

	SAFE_DELETE_ARRAY( m_pTOKEN_INFO_HEADER );
	return TRUE;
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__




