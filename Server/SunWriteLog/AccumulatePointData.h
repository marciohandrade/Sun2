#pragma once

#include "baselogdata.h"

class DLL_DECLARE AccumulatePointData : public BaseLogData
{
public:
	AccumulatePointData(void);
	virtual ~AccumulatePointData(void);

protected:	
    __LOG_PROPERTY(WORD, ReadVersion );
    __LOG_PROPERTY( WORD, AccumulateYear );
    __LOG_PROPERTY( DWORD, AccumulateTime );
    __LOG_PROPERTY( WORD, OldAccumulatePoint );
    __LOG_PROPERTY( WORD, NewAccumulatePoint );
    __LOG_PROPERTY( WORD, RewardItemCode );

protected:
	virtual	VOID GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID Read(CLogArchive& rArchive);
	VOID Init();
};