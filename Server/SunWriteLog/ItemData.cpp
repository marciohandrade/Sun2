#include "stdafx.h"
#include ".\itemdata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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

	ASSERT( MAX_ACCOUT_ID_SIZE_V1 == MaxSize );

	_tcsncpy( m_szToAccountID, pszID, MAX_ACCOUT_ID_SIZE_V1 );
	m_szToAccountID[MAX_ACCOUT_ID_SIZE_V1] = '\0';
	m_byIDLen = (BYTE)_tcslen( m_szToAccountID );
	if( m_byIDLen > MAX_ACCOUT_ID_SIZE_V1 )
		m_byIDLen = MAX_ACCOUT_ID_SIZE_V1;
}

VOID ItemData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize)
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
VOID ItemData::SetItemStreamEx1( const BYTE * pItemStreamEx, DWORD MaxSize )
{
	if( !pItemStreamEx ) return;
		
	ASSERT(MAX_ITEMSTREAMEX_SIZE_NOW == MaxSize);
	
	memcpy( m_pItemStreamEX1, pItemStreamEx, min(MAX_ITEMSTREAMEX_SIZE_NOW, MaxSize) );
}
VOID ItemData::SetItemStreamEx2( const BYTE * pItemStreamEx , DWORD MaxSize)
{
	if( !pItemStreamEx ) return;
	
	ASSERT(MAX_ITEMSTREAMEX_SIZE_NOW == MaxSize);
	
	memcpy( m_pItemStreamEX2, pItemStreamEx, min(MAX_ITEMSTREAMEX_SIZE_NOW, MaxSize) );
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
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << ITEMDATA_LAST_VERSION;
	//바이너리
	switch(ITEMDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen);						//< 파트너id
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen);			//< 파트너명
			// 아이템 정보
		rArchive << m_ItemSerial;
		rArchive << m_ItemCode;
		rArchive << m_ItemNum;
		rArchive.Write( &m_Money, ITEM_DATA_MONEY_SIZE_V1 );
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V1 );
		rArchive.Write( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V1 );
		break;
	case 2:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen);						//< 파트너id
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen);			//< 파트너명
		// 아이템 정보
		rArchive << m_ItemSerial;
		rArchive << m_ItemCode;
		rArchive << m_ItemNum;
		rArchive.Write( &m_Money, ITEM_DATA_MONEY_SIZE_V1 );
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V2 );
		rArchive.Write( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V2 );
		break;
	case 3:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen);						//< 파트너id
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen);			//< 파트너명
		// 아이템 정보
		rArchive << m_ItemSerial;
		rArchive << m_ItemCode;
		rArchive << m_ItemNum;
		rArchive.Write( &m_Money, ITEM_DATA_MONEY_SIZE_V2 );
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V2 );
		rArchive.Write( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V2 );
		break;
	case 4:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byIDLen;
		rArchive.Write( m_szToAccountID, m_byIDLen);						//< 파트너id
		rArchive << m_byNameLen;
		rArchive.Write( m_szToCharName, m_byNameLen);			//< 파트너명
		// 아이템 정보
		rArchive << m_ItemSerial;
		rArchive << m_ItemCode;
		rArchive << m_ItemNum;
		rArchive.Write( &m_Money, ITEM_DATA_MONEY_SIZE_V2 );
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V3 );
		rArchive.Write( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V3 );
		break;
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
		rArchive >> m_ItemSerial;
		rArchive >> m_ItemCode;
		rArchive >> m_ItemNum;
		rArchive.Read( &m_Money, ITEM_DATA_MONEY_SIZE_V1 );
		rArchive >> m_ItemStreamVersion;
		rArchive.Read( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V1 );
		rArchive.Read( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V1 );
		break;
	case 2:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;
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
		rArchive >> m_ItemSerial;
		rArchive >> m_ItemCode;
		rArchive >> m_ItemNum;
		rArchive.Read( &m_Money, ITEM_DATA_MONEY_SIZE_V1 );
		rArchive >> m_ItemStreamVersion;
		rArchive.Read( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V2 );
		rArchive.Read( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V2 );
		break;
	case 3:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;
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
		rArchive >> m_ItemSerial;
		rArchive >> m_ItemCode;
		rArchive >> m_ItemNum;
		rArchive.Read( &m_Money, ITEM_DATA_MONEY_SIZE_V2 );
		rArchive >> m_ItemStreamVersion;
		rArchive.Read( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V2 );
		rArchive.Read( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V2 );
		break;
	case 4:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;
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
		rArchive >> m_ItemSerial;
		rArchive >> m_ItemCode;
		rArchive >> m_ItemNum;
		rArchive.Read( &m_Money, ITEM_DATA_MONEY_SIZE_V2 );
		rArchive >> m_ItemStreamVersion;
		rArchive.Read( m_pItemStreamEX1, MAX_ITEMSTREAMEX_SIZE_V3 );
		rArchive.Read( m_pItemStreamEX2, MAX_ITEMSTREAMEX_SIZE_V3 );
		break;
	}
}

#endif