#include "StdAfx.h"
#include ".\warehousedata.h"

WareHouseData::WareHouseData(void)
{
	SetCategory( LOG_SNAPSHOT );
}

WareHouseData::~WareHouseData(void)
{
}

VOID	WareHouseData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
}

VOID	WareHouseData::Init()
{
	BaseLogData::Init();

	SetCategory( LOG_SNAPSHOT );

	ZeroMemory( m_byWareHouse, MAX_WAREHOUSESTREAM_SIZE * sizeof(TCHAR) );
}

VOID	WareHouseData::SetTempInventory( BYTE* pbyWareHouse, DWORD MaxSize )
{
	if( !pbyWareHouse )
		return;

	ASSERT( MAX_WAREHOUSESTREAM_SIZE == MaxSize);

	memcpy( m_byWareHouse, pbyWareHouse, min( MAX_WAREHOUSESTREAM_SIZE, MaxSize ) );
}

VOID WareHouseData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;
	WORD wSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)WAREHOUSEDATA_LAST_VERSION;
	//바이너리
	if(WAREHOUSEDATA_LAST_VERSION >= 1)
	{
		rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;
		_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byWareHouse, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_Money, rArchive);
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText( m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID WareHouseData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;
	WORD wSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		rArchive >> m_ItemBinaryVersion;
		LOG_READ( wSize, m_byWareHouse, rArchive );
		LOG_READ( cSize, m_Money, rArchive );
	}
}

