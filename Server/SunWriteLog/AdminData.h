#pragma once

#include "BaseLogData.h"

class DLL_DECLARE AdminData : public BaseLogData
{
public:
	AdminData(void);
	~AdminData(void);

protected:
	
	__LOG_PROPERTY( WORD, MapCode );
	__LOG_PROPERTY( int, MapX );
	__LOG_PROPERTY( int, MapY );
	__LOG_PROPERTY( int, MapZ );	
	__LOG_PROPERTY( BYTE, byNameLen );
	__LOG_PROPERTY(WORD, ReadVersion );
	TCHAR 	m_szToCharName[MAX_MONSTERNAME_LENGTH+1];					//상대방 캐릭터 명.

protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	inline  VOID	Init();
	inline VOID	SetToCharName( const TCHAR* pszCharName, DWORD MaxSize );
	inline TCHAR* GetToCharName(){ return m_szToCharName; }
};