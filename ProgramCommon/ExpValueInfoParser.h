#ifndef _EXPVALUEINFO_PARSER_H_
#define _EXPVALUEINFO_PARSER_H_

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <Define.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>


struct BASE_ACCUM_EXP_INFO
{
	LEVELTYPE	m_Level;			// 레벨
	EXPTYPE		m_AccumExp;			// 누적 경험치
};


class ExpValueInfoParser : public util::Singleton<ExpValueInfoParser>, public IParser
{
public:
	ExpValueInfoParser();
	~ExpValueInfoParser();

	VOID					Init(DWORD dwPoolSize);
	VOID					Init(DWORD dwPoolSize , char *szPackfileName);

	VOID					Release();
	VOID					Reload();

	BASE_ACCUM_EXP_INFO*	GetAccumExpInfo( LEVELTYPE Level ) { return m_pAccumExpInfoHashTable->GetData( Level ); }
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	LEVELTYPE               CalculateAccumExpToLevel(const EXPTYPE& accum_exp); // 누적 경험치로 레벨을 얻어온다.
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM

	BOOL					_Load( BOOL bReload );
public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	VOID					Unload();

private:
	util::SolarHashTable<BASE_ACCUM_EXP_INFO *>*	m_pAccumExpInfoHashTable;
	char											m_pszFileName[MAX_PATH];
};

#endif // _EXPVALUEINFO_PARSER_H_