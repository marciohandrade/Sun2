#include "StdAfx.h"
#include "ExchangeRewardInfo.h"
#include <Struct.h>
#include <iostream>

//==================================================================================================

ExchangeRewardInfoParser::ExchangeRewardInfoParser() : m_pRewardInfoHash( NULL )
{
}

ExchangeRewardInfoParser::~ExchangeRewardInfoParser()
{
	FASSERT( m_pRewardInfoHash == NULL );
}

VOID ExchangeRewardInfoParser::Init( DWORD dwPoolSize )
{
	FASSERT( m_pRewardInfoHash == NULL );
	m_pRewardInfoHash = new EXCHANGE_REWARDINFO_HASH;
	m_pRewardInfoHash->Initialize( dwPoolSize );
}


VOID ExchangeRewardInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID ExchangeRewardInfoParser::Release()
{
	Unload();

	SAFE_DELETE( m_pRewardInfoHash );
}

VOID ExchangeRewardInfoParser::Unload()
{
	if( m_pRewardInfoHash )
	{
		while (EXCHANGE_REWARDINFO* pInfo = m_pRewardInfoHash->GetNext())
		{
			delete pInfo;
		}
		m_pRewardInfoHash->RemoveAll();
	}
}

VOID ExchangeRewardInfoParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

BOOL ExchangeRewardInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; nRow++ )
	{
		EXCHANGE_REWARDINFO * pInfo = new EXCHANGE_REWARDINFO;

		pInfo->m_Index			= GetDataDWORD( "Index", nRow ); 
		pInfo->m_ItemCount		= GetDataWORD( "BNum", nRow );
		pInfo->m_ItemCode		= GetDataWORD( "ICode", nRow );
		pInfo->m_RewardIndex	= GetDataDWORD( "JCode", nRow );
        
		DWORD RewardKey = MakeRewardKey( pInfo->m_ItemCode, pInfo->m_ItemCount );
		EXCHANGE_REWARDINFO * pPrevInfo = m_pRewardInfoHash->GetData( RewardKey );
		if( !pPrevInfo ) 
		{
			m_pRewardInfoHash->Add( pInfo, RewardKey );
		}
		else
		{
			if( bReload )
				memcpy( pPrevInfo, pInfo, sizeof(EXCHANGE_REWARDINFO) );
			else
			{
				ASSERT( !"ExchangeRewardInfo.txt 데이터에 오류가 있습니다." );
				SUNLOG( eCRITICAL_LOG, "[경고]ExchangeRewardInfo.txt: %d = %d index의 키 겹침\n", pPrevInfo->m_Index, pInfo->m_Index );
			}
			SAFE_DELETE( pInfo );
		}
	}

	return TRUE;
}

BOOL ExchangeRewardInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_EXCHANGE_REWARD_INFO:
		return _Load( bReload );
	}

	return FALSE; 
}

EXCHANGE_REWARDINFO * ExchangeRewardInfoParser::GetRewardInfo( SLOTCODE ItemCode, WORD ItemCount )
{
	DWORD RewardKey = MakeRewardKey( ItemCode, ItemCount );

	EXCHANGE_REWARDINFO *pInfo = m_pRewardInfoHash->GetData( RewardKey );

	return pInfo;
}

