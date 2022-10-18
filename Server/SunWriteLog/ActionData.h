#pragma once

#include "BaseLogData.h"

class DLL_DECLARE ActionData : public BaseLogData
{
public:
	ActionData(VOID);
	~ActionData(VOID);


protected:
	__LOG_PROPERTY(WORD, ReadVersion );
	//현재버전 사용하는 거 
	__LOG_PROPERTY(WORD, MapCode );
	__LOG_PROPERTY(SHORT, MapX );
	__LOG_PROPERTY(SHORT, MapY );
	__LOG_PROPERTY(SHORT, MapZ );
	//현재버전에서 사용하지 않는 거

protected:
	VOID	GetBodyDataText( TCHAR* pszData, int nSize );

public:
	VOID	Init();

	//setter Method
public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);
};