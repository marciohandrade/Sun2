#include "StdAfx.h"
#include ".\inventorydata.h"

InventoryData::InventoryData(void)
{
	SetCategory( LOG_SNAPSHOT );
}

InventoryData::~InventoryData(void)
{
}


VOID	InventoryData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{

}

VOID	InventoryData::Init()
{
	BaseLogData::Init();

	SetCategory( LOG_SNAPSHOT );

	ZeroMemory( m_byInventory, MAX_INVENTORYITEMSTREAM_SIZE * sizeof(TCHAR) );
	ZeroMemory( m_byTempInventory, MAX_TEMPINVENTORYITEMSTREAM_SIZE * sizeof(TCHAR) );
}

VOID	InventoryData::SetInventory( BYTE* pbyInventory, DWORD MaxSize )
{
	if( !pbyInventory )
		return;

	ASSERT( MAX_INVENTORYITEMSTREAM_SIZE == MaxSize);

	memcpy( m_byInventory, pbyInventory, min(MAX_INVENTORYITEMSTREAM_SIZE, MaxSize) );
}

VOID	InventoryData::SetTempInventory( BYTE* pbyTempInventory, DWORD MaxSize )
{
	if( !pbyTempInventory )
		return;

	ASSERT( MAX_TEMPINVENTORYITEMSTREAM_SIZE == MaxSize);

	memcpy( m_byTempInventory, pbyTempInventory, min(MAX_TEMPINVENTORYITEMSTREAM_SIZE, MaxSize) );
}

VOID InventoryData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{	
	WORD wSize = 0;
	BYTE cSize = 0;
	
	BaseLogData::Serialize(rArchive, bAddText);
	
	rArchive << (WORD)INVENTORYDATA_LAST_VERSION;
	//바이너리
	if(INVENTORYDATA_LAST_VERSION >= 1)
	{
		rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;
		_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byInventory, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byTempInventory, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_InventoryMoney, rArchive);
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID InventoryData::Read(CLogArchive& rArchive)
{
	WORD wSize = 0;
	BYTE cSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		rArchive >> m_ItemBinaryVersion;
		LOG_READ( wSize, m_byInventory, rArchive );
		LOG_READ( wSize, m_byTempInventory, rArchive );
		LOG_READ( cSize, m_InventoryMoney, rArchive );
	}
}

