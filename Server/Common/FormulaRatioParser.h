#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#include <SolarFileReader.h>

enum RATIO_TYPE
{
	RATIO_TYPE_EXP_FIELD			= 1,		// 경험치 배율(필드)
	RATIO_TYPE_EXP_HUNTING			= 11,		// 경험치 배율(헌팅)
	RATIO_TYPE_EXP_MISSION			= 21,		// 경험치 배율(미션)

	RATIO_TYPE_DROP_FIELD			= 101,		// 드랍 배율(필드)
	RATIO_TYPE_DROP_HUNTING			= 111,		// 드랍 배율(헌팅)
	RATIO_TYPE_DROP_MISSION			= 121,		// 드랍 배율(미션)

	RATIO_TYPE_CHUNTING_EXP			= 1001,		// 경쟁헌팅 경험치 배율
	RATIO_TYPE_CHUNTING_MONEY		= 2001,		// 경쟁헌팅 하임 배율
	RATIO_TYPE_CHUNTING_PARTYNUM	= 10000,	// 경쟁헌팅 파티원증가에따른 %값
	RATIO_TYPE_TIMEATTACK			= 10001,	// 경쟁헌팅 모드 배율(타임어택)
	RATIO_TYPE_DEATHMATCH			= 10002,	// 경쟁헌팅 모드 배율(데쓰매치)
	RATIO_TYPE_SURVIVAL				= 10003,	// 경쟁헌팅 모드 배율(서바이벌)

	RATIO_TYPE_REGEN_TIME			= 20001,	// 리젠 속도 조절
};


struct BASE_FORMULA_RATIOINFO
{
	WORD		m_wNCode;		// 구분 코드(RATIO_TYPE)
	LEVELTYPE	m_Level;		// 레벨
	float		m_fRatio;		// 배율
};

#define MAKE_FOMULAKEY( CODE, LEVEL )	(DWORD)( ((CODE&0xFFFF)<<16)|(LEVEL&0xFFFF) )

class FormulaRatioParser : public util::Singleton<FormulaRatioParser>, public IParser
{
public:
	FormulaRatioParser();
	~FormulaRatioParser();

	VOID					Init( DWORD dwPoolSize );
	VOID					Release();

	VOID					Reload();

	BASE_FORMULA_RATIOINFO*	GetFormulaRatio( WORD wCode, LEVELTYPE Level=0 );

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

private:
	VOID					Unload();

private: // FILEDs
	util::SolarHashTable<BASE_FORMULA_RATIOINFO *>*		m_pRatioHashTable;
	char												m_pszFileName[MAX_PATH];
};





