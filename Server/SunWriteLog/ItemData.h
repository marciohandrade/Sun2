#pragma once

#include "BaseLogData.h"

class DLL_DECLARE ItemData : public BaseLogData
{
public:
	ItemData(VOID);
	~ItemData(VOID);

protected:
	//Ver. 1	
	__LOG_PROPERTY( WORD, MapCode );
	__LOG_PROPERTY( SHORT, MapX );
	__LOG_PROPERTY( SHORT, MapY );
	__LOG_PROPERTY( SHORT, MapZ );

	__LOG_PROPERTY( BYTE, byIDLen );	
	__LOG_PROPERTY( BYTE, byNameLen );
	
		// 아이템 정보
	__LOG_PROPERTY( DWORD64, ItemSerial );
	__LOG_PROPERTY( WORD, ItemCode );
	__LOG_PROPERTY( BYTE, ItemNum );
	__LOG_PROPERTY( ULONGLONG, Money );	
	__LOG_PROPERTY(WORD, ReadVersion );

	TCHAR 				m_szToAccountID[ID_MAX_LEN+1];			//< 파트너id
	TCHAR 				m_szToCharName[MAX_CHARNAME_LENGTH+1];			//< 파트너명
	
	__LOG_PROPERTY(WORD, ItemStreamVersion );
	BYTE				m_pItemStreamEX1[MAX_ITEMPARTEX_SIZE];
	BYTE				m_pItemStreamEX2[MAX_ITEMPARTEX_SIZE];	
	
protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);
	
	VOID	Init();

	//setter Method
public:
	inline VOID		SetToAccountID( const TCHAR* pszID, DWORD MaxSize );
	inline TCHAR*	GetToAccountID(){ return m_szToAccountID; }
	inline VOID		SetToCharName( const TCHAR* pszCharName, DWORD MaxSize );
	inline TCHAR*	GetToCharName(){ return m_szToCharName; }
	inline VOID		SetItemStreamEx1( const BYTE * pItemStreamEx, DWORD MaxSize );
	inline BYTE*	GetItemStreamEx1(){ return m_pItemStreamEX1; }
	inline int		GetItemStreamEx1Size(){ return sizeof(m_pItemStreamEX1); }
	inline VOID		SetItemStreamEx2( const BYTE * pItemStreamEx, DWORD MaxSize );
	inline BYTE*	GetItemStreamEx2(){ return m_pItemStreamEX2; }
	inline int		GetItemStreamEx2Size(){ return sizeof(m_pItemStreamEX2); }
};