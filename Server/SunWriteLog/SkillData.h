#pragma once

#include "BaseLogData.h"

class DLL_DECLARE SkillData : public BaseLogData
{
public:
	SkillData(VOID);
	~SkillData(VOID);

protected:
	__LOG_PROPERTY(WORD, ReadVersion );
	//Ver. 1
	__LOG_PROPERTY( WORD, SkillLevel );
	__LOG_PROPERTY( int, SkillUseCnt );
protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	VOID	Init();
	//setter Method
public:
};