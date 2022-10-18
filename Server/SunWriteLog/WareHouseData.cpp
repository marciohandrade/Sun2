#include "StdAfx.h"
#include ".\warehousedata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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
	
	ZeroMemory( m_byWareHouse, MAX_WAREHOUSE_SIZE_NOW * sizeof(TCHAR) );
}


VOID	WareHouseData::SetTempInventory( BYTE* pbyWareHouse, DWORD MaxSize )
{
	if( !pbyWareHouse )
		return;

	ASSERT( MAX_WAREHOUSE_SIZE_NOW == MaxSize);

	memcpy( m_byWareHouse, pbyWareHouse, min( MAX_WAREHOUSE_SIZE_NOW, MaxSize ) );
}

VOID WareHouseData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)WAREHOUSEDATA_LAST_VERSION;
	//바이너리
	switch(WAREHOUSEDATA_LAST_VERSION)
	{
	case 1:
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_byWareHouse, MAX_WAREHOUSE_SIZE_V1 );
		rArchive << m_Money;
		break;
	case 2:
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_byWareHouse, MAX_WAREHOUSE_SIZE_V2 );
		rArchive << m_Money;
		break;
	case 3:
		rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
		rArchive.Write( m_byWareHouse, MAX_WAREHOUSE_SIZE_V3 );
		rArchive << m_Money;
		break;
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
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_ItemBinaryVersion;
		rArchive.Read( m_byWareHouse, MAX_WAREHOUSE_SIZE_V1 );
		rArchive >> m_Money;
		break;
	case 2:
		rArchive >> m_ItemBinaryVersion;
		rArchive.Read( m_byWareHouse, MAX_WAREHOUSE_SIZE_V2 );
		rArchive >> m_Money;
		break;
	case 3:
		rArchive >> m_ItemBinaryVersion;
		rArchive.Read( m_byWareHouse, MAX_WAREHOUSE_SIZE_V3 );
		rArchive >> m_Money;
		break;
	}
}

#endif