//-------------------------------------------------------------------------------------------------
// 2007. 01. 12
// 김창현 - shogen@webzen.co.kr
// 유료화 아이템 '몬스터 소환' 을 위한 스크립트 파서
//-------------------------------------------------------------------------------------------------

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ServerStruct.h>

#include <SolarFileReader.h>
#include <ReliableRatioRandomizer.h>
#define MAX_MONSTER_SUMMON_IN_GROUP	30

struct MONSTER_SUMMON_INFO
{
	CODETYPE	MonsterCode;
	double		dSeed;
};

//-------------------------------------------------------------------------------------------------
// 몬스터 소환 정보
//-------------------------------------------------------------------------------------------------
struct MonsterSummonScriptInfo
{
	WORD					m_wMonsterGroupCode;
	WORD					m_wMonsterNum;
	double					m_dMaxRatio;
	DWORD					m_LiveTime;
	BYTE					m_LimitedLV;
	MONSTER_SUMMON_INFO		m_pMonster[MAX_MONSTER_SUMMON_IN_GROUP];	
};
//-------------------------------------------------------------------------------------------------
// 몬스터 소환 Parser
//-------------------------------------------------------------------------------------------------
typedef util::SolarHashTable<MonsterSummonScriptInfo *>		MONSTER_SUMMON_HASH;
typedef MONSTER_SUMMON_HASH::iterator						MONSTER_SUMMON_ITER;

typedef util::SolarHashTable<ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP> *>		SUMMON_RATIO_HASH;
typedef SUMMON_RATIO_HASH::iterator							SUMMON_RATIO_ITER;

class MonsterSummonParser : public util::Singleton<MonsterSummonParser>, public IParser
{
public:
	MonsterSummonParser();
	~MonsterSummonParser();


	VOID					Init( DWORD dwPoolSize );
	VOID					Release();
	VOID					Unload();
	CODETYPE				GetRandomMonsterCode( WORD GroupCode );
	WORD					GetMonsterSummonNum( WORD GroupCode );	
	DWORD					GetMonsterLiveTime( WORD GroupCode ) const;	
	const MonsterSummonScriptInfo* GetMonsterSummonScriptInfo(WORD GroupCode) const;
private:
	BOOL					CheckValidity( MonsterSummonScriptInfo *pMonsterSummonInfo, double dAccumulation );

	virtual BOOL			_Load(BOOL bReload);
	virtual BOOL			LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

private:
	MONSTER_SUMMON_HASH*									m_pMonsterSummonHash;
	char													m_pszFileName[MAX_PATH];
	SUMMON_RATIO_HASH*    									m_pRatioSelectHash;			//< 최대 입력할 비율 개수 = MAX_MONSTER_SUMMON_IN_GROUP
};

