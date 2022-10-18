#include "StdAfx.h"
#include ".\dropitemratiocalc.h"

#if(0)

#include <SolarFileReader.h>
#include <ItemInfoParser.h>
#include <SCItemSlot.h>

DropItemRatioCalc::DropItemRatioCalc()
{
	m_WeightRatioInfoHash.Initialize(MAX_ITEM_DROP_TYPE_NUM*4);
}

DropItemRatioCalc::~DropItemRatioCalc() 
{
	Release();
}

VOID DropItemRatioCalc::Reload()
{
	Release();

	_Load( m_tszFileName, TRUE );

	_AnalysisDropItemInfo();
}

VOID DropItemRatioCalc::Init( TCHAR * tszFileName )
{
	Release();

	_Load( tszFileName );

	_AnalysisDropItemInfo();
}

VOID DropItemRatioCalc::Release()
{
	m_ItemGradeSelector.Clear();
	
	DropItemGradeRatioInfo * pInfo = NULL;
	m_WeightRatioInfoHash.SetFirst();
	while( (pInfo = m_WeightRatioInfoHash.GetNext()) != NULL )
	{
		pInfo->Clear();
		SAFE_DELETE(pInfo);
	}
	m_WeightRatioInfoHash.RemoveAll();
}

DropItemInfo * DropItemRatioCalc::PopRandomDropItemForLevel( LEVELTYPE LV )
{
	if( LV == 0 || LV > MAX_LEVEL )		return NULL;

	DWORD DropKey = m_ItemGradeSelector.RandomizedKey();
	DropItemGradeRatioInfo * pInfo = _GetDropItemGradeRatioInfo(DropKey);ASSERT(pInfo);
	if( pInfo )
	{
		int max_count = pInfo->m_DropItemVector[LV].size();
		if( max_count == 0 ) return NULL;
		return pInfo->m_DropItemVector[LV][random(0,max_count-1)];
	}
	return NULL;
}

VOID DropItemRatioCalc::_AddDropItem( DWORD key, LEVELTYPE DropLV, DropItemInfo * pItem )
{
	DropItemGradeRatioInfo * pInfo = _GetDropItemGradeRatioInfo(key);
	ASSERT(pInfo);
	if( pInfo )
	{
		DropItemInfo * pn = new DropItemInfo;
		*pn = *pItem;
		pInfo->m_DropItemVector[DropLV].push_back(pn);
	}
	
}

VOID DropItemRatioCalc::_AnalysisDropItemInfo()
{
	ItemInfoParser * pParser = ItemInfoParser::Instance();
	ASSERT(pParser->IsLoaded());

	BASE_ITEMINFO * pItemInfo = NULL;
	pParser->SetFirst();
	while( (pItemInfo = pParser->GetNext()) != NULL )
	{
		// 드랍되는거냐?
		if( !pItemInfo->m_byIsDrop ) continue;

		for( BYTE ec = 0 ; ec < MAX_PLUS_ENCANT+1 ; ++ec )
		{
			for( BYTE rank = 0 ; rank < MAX_PLUS_RANK+1 ; ++rank )
			{
				for( BYTE setcode = 0 ; setcode < MAX_PLUS_SET+1 ; ++setcode )
				{
					SCItemSlot slot;
					slot.SetCode(pItemInfo->m_Code);
					slot.SetEnchant(ec);
					slot.SetRank((eRANK)rank);
					slot.SetSet(setcode);
					if( 0 == setcode || pItemInfo->IsArmor() || pItemInfo->IsWeapon() )
					{
						DropItemGradeRatioInfo tmpInfo;
						tmpInfo.m_EnchantValue	= ec;
						tmpInfo.m_DivineValue	= setcode;
						DropItemInfo item( pItemInfo->m_Code, ec, (eRANK)rank, setcode );
						_AddDropItem( tmpInfo.m_Key, slot.GetDropLevel(), &item );
					}
				}
			}
		}
	}
}



#pragma warning ( push )
#pragma warning ( disable : 4244)

VOID DropItemRatioCalc::_Load( TCHAR * tszFileName, BOOL bReload )
{
	if( !bReload ) strncpy( m_tszFileName, tszFileName, MAX_PATH );

	m_ItemGradeSelector.Clear();

	util::SolarFileReader sr;
	if( !sr.OpenFile( m_tszFileName, SEPERATOR_WHITESPACE ) )
	{
		ASSERT( !"파일을 읽을 수 없습니다." );
		return;
	}

	INT index = 0;
	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
		{
			break;
		}
		index = sr.GetTokenNumber();
		ASSERT( index < MAX_ITEM_DROP_TYPE_NUM );
		DropItemGradeRatioInfo * pInfo = new DropItemGradeRatioInfo;
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_EnchantValue	= sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_DivineValue		= sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_Ratio			= sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_StandardRatio	= sr.GetTokenNumber();

		BOOL rt = m_ItemGradeSelector.AddRatio( pInfo->m_Key, pInfo->m_Ratio, pInfo->m_StandardRatio ); ASSERT(rt);
		ASSERT( NULL == m_WeightRatioInfoHash.GetData(pInfo->m_Key) );
		m_WeightRatioInfoHash.Add( pInfo, pInfo->m_Key );
	}

	sr.CloseFile();
}
#pragma warning ( pop )


#endif