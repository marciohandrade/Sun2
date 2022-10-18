#include "StdAfx.h"
#include ".\moneydata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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

	ASSERT( MAX_ACCOUT_ID_SIZE_V1 == MaxSize );

	_tcsncpy( m_szToAccountID, pszID, MAX_ACCOUT_ID_SIZE_V1 );
	m_szToAccountID[MAX_ACCOUT_ID_SIZE_V1] = '\0';
	m_byIDLen = (BYTE)_tcslen( m_szToAccountID );
	if( m_byIDLen > MAX_ACCOUT_ID_SIZE_V1)
		m_byIDLen = MAX_ACCOUT_ID_SIZE_V1;
}

VOID MoneyData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize )
{
	if( !pszCharName )
		return;

	ASSERT( MAX_CHAR_NAME_SIZE_V1 == MaxSize );

	_tcsncpy( m_szToCharName, pszCharName, MAX_CHAR_NAME_SIZE_V1 );
	m_szToCharName[MAX_CHAR_NAME_SIZE_V1] = '\0';
	m_byNameLen = (BYTE)_tcslen( m_szToCharName );
	if( m_byNameLen > MAX_CHAR_NAME_SIZE_V1 )
		m_byNameLen = MAX_CHAR_NAME_SIZE_V1;

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
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)MONEYDATA_LAST_VERSION;
	//바이너리
	switch(MONEYDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen );			//< 파트너id
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen );			//< 파트너명

		rArchive << m_Money;
		rArchive << m_ToMoney;
		rArchive << m_RoomKey;
		break;
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
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;
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
		rArchive >> m_Money;
		rArchive >> m_ToMoney;
		rArchive >> m_RoomKey;
		break;
	}
}

#endif