#include "StdAfx.h"
#include ".\rewardinfolist.h"
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include "ItemInfoParser.h"
#include <iostream>



//////////////////////////////////////////RewardInfo//////////////////////////////////////////////


//////////////////////////////////////////RewardInfoList//////////////////////////////////////////
RewardInfoList::RewardInfoList(void)
{
}

RewardInfoList::~RewardInfoList(void)
{
	Unload();
}

void RewardInfoList::Unload()
{
	RewardInfo * pInfo = NULL;
	REWARD_HASH::iterator it, end(m_RewardInfoHash.end());
	for( it = m_RewardInfoHash.begin(); it != end ; ++it )
	{
		pInfo = it->second;
		SAFE_DELETE(pInfo);
	}
	m_RewardInfoHash.clear();
}

void RewardInfoList::Init(char * pszPackFile)
{
	SetPackFileName( pszPackFile );	
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
#pragma warning ( disable : 4800)

BOOL RewardInfoList::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
	int nRowSize = GetRowSize();
	TCHAR szTemp[MAX_FIELD_NAME_SIZE] = {0,};
    DWORD info_index = 0;

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
        info_index = GetDataNumber("Index",nRow);

        if ((FindRewardInfo(info_index) != NULL))
        {
#ifndef _SERVER //#if'N'def
            assert(!"[RewardInfoList::_Load] 중복된 Index");
#endif //_SERVER            
            SUNLOG(eCRITICAL_LOG, "[RewardInfoList::_Load] 중복된 Index = %d", info_index);
            continue;
        }

		RewardInfo * pInfo = new RewardInfo;
        pInfo->SetIndex(info_index);
        m_RewardInfoHash[ pInfo->GetIndex() ] = pInfo;
		
		strncpy( pInfo->m_pszDesc, GetDataString("Description",nRow), RewardInfo::MAX_DESC_LENGTH );
		pInfo->SetDCode( GetDataNumber("DescriptionCode",nRow) );
		pInfo->SetExp( GetDataNumber("Exp",nRow) );
		pInfo->SetHime( GetDataNumber("Hime",nRow) );
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        pInfo->SetCoin( GetDataNumber("Coin",nRow) );
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        pInfo->SetTitle( GetDataWORD("Title",nRow) );
        pInfo->SetReputeCode( GetDataWORD("ReputeCode",nRow) );
        pInfo->SetReputePoint( GetDataWORD("ReputePoint",nRow) );
#endif //_NA_003027_20111013_HONOR_SYSTEM
		pInfo->SetChaoTime( GetDataNumber("ChaoTime",nRow) );
		pInfo->SetItemCode( GetDataNumber("Fixed_IC",nRow) );
		pInfo->SetItemTypeIndex( GetDataNumber("IGtype",nRow) );
		if( pInfo->GetItemCode() !=0 )
		{
			if( !IsExistItemInScript( pInfo->GetItemCode(), pInfo->GetItemTypeIndex() ) )
			{
				SUNLOG( eCRITICAL_LOG, "[Index : %u]\n", pInfo->GetIndex());
				pInfo->SetItemTypeIndex(1);
			}		
		}

		pInfo->SetSkillCode( GetDataNumber("Skill",nRow) );
		pInfo->SetRefType( GetDataNumber("ReferenceType",nRow) );

		if( 0 != pInfo->GetItemCode() && NULL == ItemInfoParser::Instance()->GetItemInfo(pInfo->GetItemCode()) )
		{
			SUNLOG( eCRITICAL_LOG, "[RewardInfoList] Fixed ItemCode(%u) doesn't exist\n", pInfo->GetItemCode());
		}

		pInfo->SetSelectNum( GetDataNumber("Select_Num",nRow) );		
		for(int i = 0 ; i < RewardInfo::MAX_DROP_INDEX_NUM ; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "Sel_IC%d\0", i+1 );
			pInfo->SetTypeCode( i, GetDataNumber(szTemp,nRow) );
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "IGtype%d\0", i+1 );
			pInfo->SetTypeIndex( i, GetDataNumber(szTemp,nRow) );
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
            _snprintf( szTemp, MAX_FIELD_NAME_SIZE, "TimeLimit%d\0", i+1);
            pInfo->SetTimeLimit( i, GetDataNumber(szTemp,nRow));
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

			// 타입 체크
			if( pInfo->GetRefType() == RewardInfo::REF_TYPE_SELECT && 
					SCItemSlot::IsOverlap( pInfo->GetTypeCode(i) ) )
			{
				if( pInfo->GetTypeIndex(i) <= 0 )
				{
					SUNLOG( eCRITICAL_LOG, "[경고:오류:%d]잘못된 스택아이템(%d)의 개수이다\n", pInfo->GetIndex(), pInfo->GetTypeCode(i) );
					pInfo->SetTypeIndex(i, 1);
				}
			}

			if( pInfo->GetRefType() == RewardInfo::REF_TYPE_SELECT || 
				pInfo->GetRefType() == RewardInfo::REF_TYPE_EVENT_ITEMCODE )
			{
				if( 0!= pInfo->GetTypeCode(i) && NULL == ItemInfoParser::Instance()->GetItemInfo(pInfo->GetTypeCode(i)) )
					SUNLOG( eCRITICAL_LOG, "[RewardInfoList] Select ItemCode(%u) doesn't exist\n", pInfo->GetTypeCode(i));
			}
		}

		if( pInfo->GetItemCode() != 0 &&
			SCItemSlot::IsOverlap(pInfo->GetItemCode()) )
		{
			if( pInfo->GetItemTypeIndex() <= 0 )
			{
				SUNLOG( eCRITICAL_LOG, "[경고:오류:%d]잘못된 스택아이템(%d)의 개수이다\n", pInfo->GetIndex(), pInfo->GetItemTypeIndex() );
				pInfo->SetItemTypeIndex(1);
			}
		}

		//
		int count = 0;
		for(int i = 0 ; i < RewardInfo::MAX_DROP_INDEX_NUM ; ++i )
		{
			if( pInfo->GetTypeCode(i) != 0 )
			{
				++count;
			}
		}

		if( pInfo->GetRefType() == RewardInfo::REF_TYPE_SELECT || 
			pInfo->GetRefType() == RewardInfo::REF_TYPE_RANDOM )
		{
			if( count < pInfo->GetSelectNum() ||
				( count == 0 && pInfo->GetSelectNum() > 0 ) ||
				( pInfo->GetSelectNum() == 0 && count > 0 ) )
			{
				SUNLOG( eCRITICAL_LOG, "[경고:오류:%d]잘못된 SelectNum(%d)의 개수이다\n", pInfo->GetIndex(), pInfo->GetSelectNum() );
			}
		}

#ifdef _NA_20100307_BETAKEY_SYSTEM
        pInfo->SetBetaKey( GetDataInt("BetaKeyCode", nRow) );        
#endif
	}
		
	return TRUE;
}

#pragma warning ( pop )

void RewardInfoList::Reload()
{
//	Load( m_pszFileName, TRUE );
}	

RewardInfo * RewardInfoList::FindRewardInfo( DWORD Index )
{
	REWARD_HASH::iterator itr = m_RewardInfoHash.find( Index );

	if ( itr != m_RewardInfoHash.end() )
	{
		return itr->second;
	}

	return NULL;
}

BOOL RewardInfoList::IsExistItemInScript(SLOTCODE IN ItemCode, DWORD dwItemTypeIndex)
{
	if( NULL == ItemInfoParser::Instance()->GetItemInfo(ItemCode) )
	{
		SUNLOG( eCRITICAL_LOG, "[RewardInfoList] 아이템(%u)존재하지않는다.", ItemCode);
		return FALSE;
	}

	if( dwItemTypeIndex <= 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[RewardInfoList] 잘못된 아이템(%d)의 개수이다.", dwItemTypeIndex );			
		return FALSE;
	}

	if( !SCItemSlot::IsOverlap(ItemCode) )
	{
		if( dwItemTypeIndex > 1 )
		{
			SUNLOG( eCRITICAL_LOG, "[RewardInfoList] 중복이 불가능한 아이템(%d)이다.", ItemCode );			
			return FALSE;
		}
	}

	return TRUE;
}


BOOL	RewardInfoList::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_REWARDINFOLIST:
		return _Load( bReload );
	}

	return FALSE;
}

VOID	RewardInfoList::Release()
{
	Unload();
}
