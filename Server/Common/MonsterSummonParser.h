//-------------------------------------------------------------------------------------------------
// 2007. 01. 12
// ��â�� - shogen@webzen.co.kr
// ����ȭ ������ '���� ��ȯ' �� ���� ��ũ��Ʈ �ļ�
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
// ���� ��ȯ ����
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
// ���� ��ȯ Parser
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
	SUMMON_RATIO_HASH*    									m_pRatioSelectHash;			//< �ִ� �Է��� ���� ���� = MAX_MONSTER_SUMMON_IN_GROUP
};

