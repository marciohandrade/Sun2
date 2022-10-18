#include "StdAfx.h"
#include ".\inventorydata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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

	ZeroMemory( m_byInventory, MAX_INVENTORYITEM_SIZE_MAX * sizeof(TCHAR) );
	ZeroMemory( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_MAX * sizeof(TCHAR) );
}

VOID	InventoryData::SetInventory( BYTE* pbyInventory, DWORD MaxSize )
{
	if( !pbyInventory )
		return;

	ASSERT( MAX_INVENTORYITEM_SIZE_NOW == MaxSize);

	memcpy( m_byInventory, pbyInventory, min(MAX_INVENTORYITEM_SIZE_NOW, MaxSize) );
}

VOID	InventoryData::SetTempInventory( BYTE* pbyTempInventory, DWORD MaxSize )
{
	if( !pbyTempInventory )
		return;

	ASSERT( MAX_TEMPINVENTORYITEM_SIZE_NOW == MaxSize);

	memcpy( m_byTempInventory, pbyTempInventory, min(MAX_TEMPINVENTORYITEM_SIZE_NOW, MaxSize) );
}

VOID InventoryData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{	
			
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)INVENTORYDATA_LAST_VERSION;
	//바이너리
	switch(INVENTORYDATA_LAST_VERSION)
	{
	case 1:
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive << m_InventoryMoney;
		break;
	case 2:
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive << m_InventoryMoney;
		break;
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
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_ItemBinaryVersion;
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive >> m_InventoryMoney;
		break;
	case 2:
		rArchive >> m_ItemBinaryVersion;
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive >> m_InventoryMoney;
		break;
	}
}

#endif