#pragma once

#include "BaseLogData.h"

class DLL_DECLARE WareHouseData : public BaseLogData
{
public:
	WareHouseData(void);
	~WareHouseData(void);

protected:
	//Ver. 1	
	__LOG_PROPERTY(WORD, ReadVersion );
	__LOG_PROPERTY(WORD, ItemBinaryVersion );
	BYTE 		m_byWareHouse[MAX_WAREHOUSESTREAM_SIZE];
	__LOG_PROPERTY( ULONGLONG, Money );

protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	inline  VOID	Init();
	inline  VOID	SetTempInventory( BYTE* pbyWareHouse, DWORD MaxSize );	
	inline  BYTE*	GetWareHousePtr(){ return m_byWareHouse; }
	inline  int		GetWareHouseSize(){ return sizeof(m_byWareHouse); }
};