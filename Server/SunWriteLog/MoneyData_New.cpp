#include "StdAfx.h"
#include ".\moneydata.h"

MoneyData::MoneyData(VOID)
{
	SetCategory( LOG_MONEY );
}

MoneyData::~MoneyData(VOID)
{
}

VOID MoneyData::Init()
{
	BaseLogData::Init();

	m_MapCode			= 0;
	m_MapX				= 0;
	m_MapY				= 0;
	m_MapZ				= 0;
	m_szToAccountID[0]	= '\0';
	m_szToCharName[0]	= '\0';
	m_Money			= 0;
	m_ToMoney		= 0;
	m_RoomKey			= 0;

	m_byNameLen = 0;
	m_byIDLen = 0;
}

VOID MoneyData::SetToAccountID( const TCHAR* pszID , DWORD MaxSize)
{
	if( !pszID )
		return;

	ASSERT( ID_MAX_LEN == MaxSize );

	_tcsncpy( m_szToAccountID, pszID, ID_MAX_LEN );
	m_szToAccountID[ID_MAX_LEN] = '\0';
	m_byIDLen = (BYTE)_tcslen( m_szToAccountID );
	if( m_byIDLen > ID_MAX_LEN)
		m_byIDLen = ID_MAX_LEN;
}

VOID MoneyData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize )
{
	if( !pszCharName )
		return;

	ASSERT( MAX_CHARNAME_LENGTH == MaxSize );

	_tcsncpy( m_szToCharName, pszCharName, MAX_CHARNAME_LENGTH );
	m_szToCharName[MAX_CHARNAME_LENGTH] = '\0';
	m_byNameLen = (BYTE)_tcslen( m_szToCharName );
	if( m_byNameLen > MAX_CHARNAME_LENGTH )
		m_byNameLen = MAX_CHARNAME_LENGTH;

}

VOID MoneyData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%u,%d,%d,%d,%s,%s,%I64u,%I64u,%u"
		, m_MapCode
		, m_MapX
		, m_MapY
		, m_MapZ
		, m_szToAccountID
		, m_szToCharName
		, m_Money
		, m_ToMoney							
		, m_RoomKey
		);

	pszBodyData[nSize] = '\0';
}

VOID MoneyData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{		
	BYTE cSize = 0;
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)MONEYDATA_LAST_VERSION;
	//바이너리
	if(MONEYDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapX, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapY, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapZ, rArchive);

		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen );			//< 파트너id
		
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen );			//< 파트너명

		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_Money, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_ToMoney, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_RoomKey, rArchive);
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID MoneyData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_MapCode, rArchive );
		LOG_READ( cSize, m_MapX, rArchive );
		LOG_READ( cSize, m_MapY, rArchive );
		LOG_READ( cSize, m_MapZ, rArchive );

		rArchive >> m_byIDLen;
		rArchive.Read( m_szToAccountID, m_byIDLen );			//< 파트너id
		if( m_byIDLen < sizeof(m_szToAccountID) )
			m_szToAccountID[m_byIDLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byNameLen;
		rArchive.Read( m_szToCharName, m_byNameLen );			//< 파트너명
		if( m_byNameLen < sizeof(m_szToCharName) )
			m_szToCharName[m_byNameLen] = '\0';
		else
			ASSERT(FALSE);

		LOG_READ( cSize, m_Money, rArchive );
		LOG_READ( cSize, m_ToMoney, rArchive );
		LOG_READ( cSize, m_RoomKey, rArchive );
	}
}

