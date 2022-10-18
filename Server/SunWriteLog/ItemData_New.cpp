#include "stdafx.h"
#include ".\itemdata.h"

ItemData::ItemData(VOID)
{
	SetCategory( LOG_ITEM );	
}

ItemData::~ItemData(VOID)
{
}

VOID ItemData::Init()
{
	BaseLogData::Init();

	m_MapCode			= 0;
	m_MapX				= 0;
	m_MapY				= 0;
	m_MapZ				= 0;
	m_szToAccountID[0]	= '\0';
	m_szToCharName[0]	= '\0';
	m_ItemSerial		= 0;
	m_ItemCode			= 0;
	m_ItemNum			= 0;
	m_Money				= 0;
	memset(m_pItemStreamEX1, 0, sizeof(m_pItemStreamEX1));
	memset(m_pItemStreamEX2, 0, sizeof(m_pItemStreamEX2));

	m_byIDLen = 0;
	m_byNameLen = 0;
}

VOID ItemData::SetToAccountID( const TCHAR* pszID, DWORD MaxSize )
{
	if( !pszID )
		return;

	ASSERT( ID_MAX_LEN == MaxSize );

	_tcsncpy( m_szToAccountID, pszID, ID_MAX_LEN );
	m_szToAccountID[ID_MAX_LEN] = '\0';
	m_byIDLen = (BYTE)_tcslen( m_szToAccountID );
	if( m_byIDLen > ID_MAX_LEN )
		m_byIDLen = ID_MAX_LEN;
}

VOID ItemData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize)
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
VOID ItemData::SetItemStreamEx1( const BYTE * pItemStreamEx, DWORD MaxSize )
{
	if( !pItemStreamEx ) return;

	ASSERT(MAX_ITEMPARTEX_SIZE == MaxSize);

	memcpy( m_pItemStreamEX1, pItemStreamEx, min(MAX_ITEMPARTEX_SIZE, MaxSize) );
}
VOID ItemData::SetItemStreamEx2( const BYTE * pItemStreamEx , DWORD MaxSize)
{
	if( !pItemStreamEx ) return;

	ASSERT(MAX_ITEMPARTEX_SIZE == MaxSize);

	memcpy( m_pItemStreamEX2, pItemStreamEx, min(MAX_ITEMPARTEX_SIZE, MaxSize) );
}
VOID ItemData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%u,%d,%d,%d,%s,%s,%u,%u,%u"
		, m_MapCode
		, m_MapX
		, m_MapY
		, m_MapZ
		, m_szToAccountID
		, m_szToCharName
		, m_ItemCode
		, m_ItemSerial							
		, m_ItemNum
		);

	pszBodyData[nSize] = '\0';
}
VOID ItemData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;
	WORD wSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)ITEMDATA_LAST_VERSION;
	//바이너리
	if(ITEMDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapX, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapY, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapZ, rArchive);
				
		rArchive << m_byIDLen;		
		rArchive.Write( m_szToAccountID, m_byIDLen);						//< 파트너id
		
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen);			//< 파트너명
		
		// 아이템 정보

		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_ItemSerial, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_ItemCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_ItemNum, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_Money, rArchive);

		rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;		

		_CHECK_LOG_SIZE_AND_WRITE( wSize, m_pItemStreamEX1, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( wSize, m_pItemStreamEX2, rArchive);
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}	
}

VOID ItemData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;
	WORD wSize = 0;

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
		rArchive.Read( m_szToAccountID, m_byIDLen);						//< 파트너id
		if( m_byIDLen < sizeof(m_szToAccountID) )
			m_szToAccountID[m_byIDLen] = '\0';
		else
			ASSERT(FALSE);
		rArchive >> m_byNameLen;
		rArchive.Read( m_szToCharName, m_byNameLen);			//< 파트너명
		if( m_byNameLen < sizeof(m_szToCharName) )
			m_szToCharName[m_byNameLen] = '\0';
		else
			ASSERT(FALSE);

		// 아이템 정보
		LOG_READ( cSize, m_ItemSerial, rArchive );
		LOG_READ( cSize, m_ItemCode, rArchive );
		LOG_READ( cSize, m_ItemNum, rArchive );
		LOG_READ( cSize, m_Money, rArchive );
		
		rArchive >> m_ItemStreamVersion;
		
		LOG_READ( wSize, m_pItemStreamEX1, rArchive );
		LOG_READ( wSize, m_pItemStreamEX2, rArchive );
	}
}

