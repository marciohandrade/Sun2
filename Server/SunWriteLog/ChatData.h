#pragma once

#include "BaseLogData.h"

class DLL_DECLARE ChatData : public BaseLogData
{
public:
	ChatData(VOID);
	~ChatData(VOID);

protected:	
	__LOG_PROPERTY( int, Channel );
	__LOG_PROPERTY( int, UserGuid );
	__LOG_PROPERTY( int, ListenUserGuid );
	__LOG_PROPERTY( BYTE, byNameLen );
	__LOG_PROPERTY(WORD, ReadVersion );

	TCHAR	m_szListenCharName[MAX_CHARNAME_LENGTH+1];
	TCHAR	m_szContext[MAX_CHATMSG_SIZE+1];
	__LOG_PROPERTY( BYTE, byTextLen );	
	__LOG_PROPERTY( WORD, MapCode );
	__LOG_PROPERTY( int, RoomNo );

protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

    VOID	Init();
	//setter Method
public:	
	inline VOID	SetListenCharName( TCHAR* pszName, DWORD MaxSize );
	inline VOID	SetContext( TCHAR* pszContext, DWORD MaxSize );
	inline TCHAR* GetListenCharName(){ return m_szListenCharName; }
	inline TCHAR* GetContext(){ return m_szContext; } 
};