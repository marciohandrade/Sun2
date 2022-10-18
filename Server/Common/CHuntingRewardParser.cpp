#include "StdAfx.h"
#include "CHuntingRewardParser.h"
#include <SolarFileReader.h>
#include <iostream>

CHuntingRewardParser::CHuntingRewardParser(void) : m_pRewardHash(NULL)
{
}

CHuntingRewardParser::~CHuntingRewardParser(void)
{
}

VOID CHuntingRewardParser::Release()
{
	Unload();
	SAFE_DELETE( m_pRewardHash );
}

VOID CHuntingRewardParser::Init( DWORD dwPoolSize )
{
	m_pRewardHash = new REWARDINFO_HASH;
}

void CHuntingRewardParser::Unload()
{
    if (m_pRewardHash)
    {
        REWARDINFO_HASH::const_iterator it = m_pRewardHash->begin(),
                                        end = m_pRewardHash->end();
        for (; it != end; ++it)
        {
            REWARD_INFO* reward = it->second;
            SAFE_DELETE(reward);
        };

        m_pRewardHash->clear();
    };
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)
VOID CHuntingRewardParser::Reload()
{
//	_Load( m_pszFileName, TRUE );
}

BOOL CHuntingRewardParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		REWARD_INFO * pInfo = new REWARD_INFO;

		pInfo->m_wIndex			= GetDataWORD( "wIndex", nRow );
		
		pInfo->m_MapCode		= GetDataWORD( "wMapCode", nRow );
		pInfo->m_FieldCode		= GetDataWORD( "wFieldCode", nRow );

		pInfo->m_bGuildPVP		= GetDataBYTE( "bGuildPvP", nRow );

		pInfo->m_byRule			= GetDataBYTE( "bCompete", nRow );
		pInfo->m_byMode			= GetDataBYTE( "bGameMode", nRow );
		pInfo->m_byWinRecord	= GetDataBYTE( "bWinRecord", nRow );
		pInfo->m_byRanking		= GetDataBYTE( "bRankCode", nRow );
		pInfo->m_byPotion		= GetDataBYTE( "bPotionNum", nRow );

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( int i = 0; i < MAX_RANDOM_ITEM_NUM; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "dwRandomRewarditem%d\0", i+1 );
			pInfo->m_dwRewardIndex[i]	= GetDataDWORD( szTemp, nRow );
		}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		DWORD64 RewardKey = MakeRewardKey( pInfo->m_MapCode, pInfo->m_FieldCode, pInfo->m_bGuildPVP, pInfo->m_byRule, pInfo->m_byMode, pInfo->m_byWinRecord );
#else
		DWORD64 RewardKey = MakeRewardKey( pInfo->m_MapCode, pInfo->m_FieldCode, pInfo->m_byRule, pInfo->m_byMode, pInfo->m_byWinRecord, pInfo->m_byRanking );
#endif
        REWARDINFO_HASH::const_iterator found_it = m_pRewardHash->find(RewardKey);
        if (found_it == m_pRewardHash->end())
        {
            m_pRewardHash->insert(REWARDINFO_HASH::value_type(RewardKey, pInfo));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "[°æ°í]VSHuintingRewardInfo.txt: '%d' Å° °ãÄ§", pInfo->m_wIndex);
            SAFE_DELETE(pInfo);
        };
    }

	return TRUE;
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
DWORD64 CHuntingRewardParser::MakeRewardKey( MAPCODE MapCode, FIELDCODE FieldCode, BYTE bGuildPVP, BYTE byRule, BYTE byMode, BYTE byWinRecord )
#else
DWORD64 CHuntingRewardParser::MakeRewardKey( MAPCODE MapCode, FIELDCODE FieldCode, BYTE byRule, BYTE byMode, BYTE byWinRecord, BYTE byRanking )
#endif
{
	DWORD64 dw64Key = 0;
	memcpy( &dw64Key, &MapCode, sizeof(MAPCODE) );		dw64Key <<= 16;
	memcpy( &dw64Key, &FieldCode, sizeof(FIELDCODE) );	dw64Key <<= 8;
	memcpy( &dw64Key, &byRule, sizeof(BYTE) );			dw64Key <<= 8;
	memcpy( &dw64Key, &byMode, sizeof(BYTE) );			dw64Key <<= 8;
	memcpy( &dw64Key, &byWinRecord, sizeof(BYTE) );		dw64Key <<= 8;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	memcpy( &dw64Key, &bGuildPVP, sizeof(BYTE) );
#else
	memcpy( &dw64Key, &byRanking, sizeof(BYTE) );
#endif

	return dw64Key;
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
REWARD_INFO* CHuntingRewardParser::GetRewardInfo( MAPCODE MapCode, FIELDCODE FieldCode, BYTE bGuildPVP, BYTE byRule, BYTE byMode, BYTE byWinRecord )
#else
REWARD_INFO* CHuntingRewardParser::GetRewardInfo( MAPCODE MapCode, FIELDCODE FieldCode, BYTE byRule, BYTE byMode, BYTE byWinRecord, BYTE byRanking )
#endif
{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	DWORD64 RewardKey = MakeRewardKey( MapCode, FieldCode, bGuildPVP, byRule, byMode, byWinRecord );
#else
	DWORD64 RewardKey = MakeRewardKey( MapCode, FieldCode, byRule, byMode, byWinRecord, byRanking );
#endif

    REWARD_INFO* pInfo = NULL;
    // NOTE: f110527.6L
    ;{
        REWARDINFO_HASH::const_iterator found_it = m_pRewardHash->find(RewardKey);
        pInfo = (found_it == m_pRewardHash->end()) ? NULL : found_it->second;
    };

	return pInfo;
}

DWORD CHuntingRewardParser::GetRandomRewardIndex( REWARD_INFO *pRewardInfo )
{
	BYTE byValidIndexNum = 0;
	for( int i = 0; i < MAX_RANDOM_ITEM_NUM; ++i )
	{
		if( pRewardInfo->m_dwRewardIndex[i] )
			++byValidIndexNum;
	}

	BYTE bySelect = rand() % byValidIndexNum;

	return pRewardInfo->m_dwRewardIndex[bySelect];
}

//#pragma warning ( pop )

BOOL	CHuntingRewardParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_VSHUNTINGREWARDINFO:
		return _Load( bReload );
	};

	return FALSE;
}
