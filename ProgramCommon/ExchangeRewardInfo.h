#pragma once
#ifndef PROGRAMCOMMON_EXCHANGE_REWARD_INFO_H
#define PROGRAMCOMMON_EXCHANGE_REWARD_INFO_H

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ScriptCode.IParser.h>

//==================================================================================================
// NOTE: this class is used by __NA_001027_20080423_RANDOM_REWARD_EVENT_, default exposed

struct EXCHANGE_REWARDINFO
{
	DWORD		m_Index;
	SLOTCODE	m_ItemCode;
	WORD		m_ItemCount;
	DWORD		m_RewardIndex;
};

// CHANGES: f110603.6L, changes the key specification
typedef util::SolarHashTable<EXCHANGE_REWARDINFO*, DWORD> EXCHANGE_REWARDINFO_HASH;

//==================================================================================================

class ExchangeRewardInfoParser : public util::Singleton<ExchangeRewardInfoParser>, public IParser
{
public:
	ExchangeRewardInfoParser();
	virtual ~ExchangeRewardInfoParser();

	VOID					Init( DWORD dwPoolSize );
	VOID					Init( DWORD dwPoolSize, char * pszPackFileName );
	VOID					Release();

	VOID					Reload();
	VOID					Unload();

	EXCHANGE_REWARDINFO *	GetRewardInfo( SLOTCODE ItemCode, WORD ItemCount );
	EXCHANGE_REWARDINFO_HASH * GetHashTable() {return m_pRewardInfoHash;}

	BOOL					_Load( BOOL bReload );
public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL					_Load( util::SolarFileReader& sr, BOOL bReload );
    DWORD MakeRewardKey(SLOTCODE item_code, WORD number_of_items);

private:
	EXCHANGE_REWARDINFO_HASH *	m_pRewardInfoHash;
	char						m_pszFileName[MAX_PATH];
};

//==================================================================================================

// CHANGES: f110603.6L, changes the key specification
inline DWORD ExchangeRewardInfoParser::MakeRewardKey(SLOTCODE item_code, WORD number_of_items)
{
    DWORD composed = number_of_items;
    ;     composed <<= 16;
    ;     composed |= item_code;
    return composed;
}

//==================================================================================================
#endif //PROGRAMCOMMON_EXCHANGE_REWARD_INFO_H