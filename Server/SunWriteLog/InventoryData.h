#pragma once

#include "BaseLogData.h"

class DLL_DECLARE InventoryData : public BaseLogData
{
public:
	InventoryData(void);
	~InventoryData(void);

protected:	
	__LOG_PROPERTY(WORD, ReadVersion );
	__LOG_PROPERTY(WORD, ItemBinaryVersion );
	BYTE 		m_byInventory[MAX_INVENTORYITEMSTREAM_SIZE];
	BYTE 		m_byTempInventory[MAX_TEMPINVENTORYITEMSTREAM_SIZE];
	__LOG_PROPERTY( ULONGLONG, InventoryMoney );

protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	inline VOID		Init();
	inline VOID		SetInventory( BYTE* pbyInventory , DWORD MaxSize );
	inline VOID		SetTempInventory( BYTE* pbyTempInventory, DWORD MaxSize );	
	inline BYTE*	GetInventoryPtr(){ return m_byInventory; }
	inline BYTE*	GetTempInventoryPtr(){ return m_byTempInventory; }
	inline int		GetInventorySize(){ return sizeof(m_byInventory); }
	inline int		GetTempInventorySize(){ return sizeof(m_byTempInventory); }
};