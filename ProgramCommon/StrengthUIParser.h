#ifndef __PROGRAMCOMMON_STRENGTH_UI_PARSER_H
#define __PROGRAMCOMMON_STRENGTH_UI_PARSER_H
#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <Define.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

#ifdef __NA_001062_20080623_STRENGTH_PENALTY

#define MAX_STRENGTH_UI_XCOORD 9

struct sSTRENGTH_UI_RECODE
{
	sSTRENGTH_UI_RECODE() : m_Idx(0), m_YCoordinate(0) { memset(m_XCoordinate,0,MAX_STRENGTH_UI_XCOORD); }

	BOOL IsSetRecode() { return m_Idx; }

	INT	m_Idx;
	INT m_YCoordinate;
	INT m_XCoordinate[MAX_STRENGTH_UI_XCOORD];
};

typedef util::SolarHashTable<sSTRENGTH_UI_RECODE *>				STRENGTH_UI_HASH;
typedef util::SolarHashTable<sSTRENGTH_UI_RECODE *>::iterator	STRENGTH_UI_HASH_ITR;

class StrengthUIParser : public util::Singleton<StrengthUIParser>, public IParser
{

public:
	StrengthUIParser(void);
	virtual ~StrengthUIParser(void);

	sSTRENGTH_UI_RECODE* GetStrengthUIInfo( INT Idx ) { return FindStrengthUI(Idx); }

public:
	VOID Init( char * pszPackFileName );
	VOID Release(){ Unload(); };

	VOID Reload();
	VOID Unload();

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

	inline VOID RemoveStrengthUI( INT Idx )								{ m_StrengthUIHash.Remove(Idx); }
	inline VOID AddStrengthUI( sSTRENGTH_UI_RECODE* pInfo, INT Idx )	{ m_StrengthUIHash.Add(pInfo, Idx); }
	inline sSTRENGTH_UI_RECODE* FindStrengthUI( INT Idx )				{ return m_StrengthUIHash.GetData(Idx); }

	sSTRENGTH_UI_RECODE*  _NewStrengthUI( INT Idx );

	BOOL Validating(sSTRENGTH_UI_RECODE& Recode);

private:
	char m_tszFileName[MAX_PATH];
	STRENGTH_UI_HASH m_StrengthUIHash;
};

#endif //__NA_001062_20080623_STRENGTH_PENALTY

#endif //__PROGRAMCOMMON_STRENGTH_UI_PARSER_H
