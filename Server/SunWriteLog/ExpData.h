#pragma once

#include "BaseLogData.h"

class DLL_DECLARE ExpData : public BaseLogData
{
public:
	ExpData(VOID);
	~ExpData(VOID);

protected:
	__LOG_PROPERTY(WORD, ReadVersion );
	__LOG_PROPERTY( int, NewLevel );
	__LOG_PROPERTY( int, OldLevel );
	__LOG_PROPERTY( INT64, NewExp );
	__LOG_PROPERTY( INT64, OldExp );
	__LOG_PROPERTY( int, DivideCount );

protected:
	virtual VOID GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	inline VOID	Init();
};