#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <Const.h>

#include <SolarFileReader.h>

enum { MAX_RANDOM_ITEM_NUM = 5 };

struct REWARD_INFO
{
	WORD			m_wIndex;
	MAPCODE			m_MapCode;
	FIELDCODE		m_FieldCode;
	BYTE			m_bGuildPVP;	// __NA_000000_20070528_TAIWAN_GUILD_PVP
	BYTE			m_byRule;		// Free For All, Dual Party
	BYTE			m_byMode;		// 서바이벌, 타임어택
	BYTE			m_byWinRecord;	// 연승
	BYTE			m_byRanking;	// 랭킹(1등, 2등, 3등)
	BYTE			m_byPotion;
	DWORD			m_dwRewardIndex[MAX_RANDOM_ITEM_NUM];	// 보상 아이템인포 인덱스
};

class CHuntingRewardParser : public util::Singleton<CHuntingRewardParser>, public IParser
{
public:
    // NOTE: f110527.6L
    typedef STLX_HASH_MAP<DWORD64, REWARD_INFO*> REWARDINFO_HASH;

	CHuntingRewardParser();
	~CHuntingRewardParser();

	VOID					Init( DWORD dwPoolSize );
	VOID					Release();

	VOID					Reload();

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	REWARD_INFO	*			GetRewardInfo( MAPCODE MapCode, FIELDCODE FieldCode, BYTE bGuildPVP, BYTE byRule, BYTE byMode, BYTE byWinRecord );
#else
	REWARD_INFO	*			GetRewardInfo( MAPCODE MapCode, FIELDCODE FieldCode, BYTE byRule, BYTE byMode, BYTE byWinRecord, BYTE byRanking );
#endif

	DWORD					GetRandomRewardIndex( REWARD_INFO *pRewardInfo );
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
private:
	BOOL					_Load( BOOL bReload );
	VOID					Unload();

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	DWORD64					MakeRewardKey( MAPCODE MapCode, FIELDCODE FieldCode, BYTE bGuildPVP, BYTE byRule, BYTE byMode, BYTE byWinRecord );
#else
	DWORD64					MakeRewardKey( MAPCODE MapCode, FIELDCODE FieldCode, BYTE byRule, BYTE byMode, BYTE byWinRecord, BYTE byRanking );
#endif

private:
	REWARDINFO_HASH*		m_pRewardHash;
	TCHAR					m_pszFileName[MAX_PATH];
};














