#include "StdAfx.h"
#include ".\itemcomposelistparser.h"
#include "ItemInfoParser.h"

ItemComposeListParser::ItemComposeListParser(void):
	m_pComposeListHash(NULL)
{
}

ItemComposeListParser::~ItemComposeListParser(void)
{
	FASSERT( m_pComposeListHash == NULL );
	Release();
}

VOID ItemComposeListParser::Init( DWORD dwComposeListSize )
{
	FASSERT( m_pComposeListHash == NULL );
	m_pComposeListHash = new _COMPOSELIST_HASH;
	m_pComposeListHash->Initialize( dwComposeListSize );
}

VOID ItemComposeListParser::Init( DWORD dwComposeListSize, char * pszPackFileName )
{
	Init( dwComposeListSize );
	SetPackFileName( pszPackFileName );
}

VOID ItemComposeListParser::Release()
{
	Unload();

	SAFE_DELETE( m_pComposeListHash );
}

BOOL ItemComposeListParser::_Load( BOOL bReload )
{
	const INT nRowSize = GetRowSize();
	for( INT nRow=0; nRow<nRowSize ; ++nRow )
	{
		const SLOTCODE code			= GetDataWORD( "Code", nRow );
		sITEM_COMPOSE_LIST* pInfo	= m_pComposeListHash->GetData( code );
		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new sITEM_COMPOSE_LIST;
			m_pComposeListHash->Add( pInfo, code );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "[ItemComposeListParser]Script File Data Error, Code = %d", code );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->m_Code = code;
		_tcsncpy( pInfo->m_pszDebugName, GetDataString( "Name", nRow ), MAX_ITEMNAME_LENGTH );
		pInfo->m_NCode				= GetDataDWORD( "Ncode", nRow );
		pInfo->m_LimitCharClass		= GetDataDWORD( "LimitCharClass", nRow );
		pInfo->m_LimitGuildClass	= GetDataDWORD( "LimitGuildClass", nRow );
		pInfo->m_ItemType			= GetDataDWORD( "ItemType", nRow );
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        pInfo->m_dCategoryCount     = GetDataDWORD( "CategorySubCount",nRow);

        TCHAR tempSring[64];
        for (int i = 0 ; i < pInfo->m_dCategoryCount ; ++i)
        {
            sprintf(tempSring,"bCategory%d",i+1);
            pInfo->m_pCategory[i] = GetDataDWORD(tempSring, nRow );
            sprintf(tempSring,"Category%d_Scode",i+1);
            pInfo->m_pCategoryCode[i] = GetDataDWORD(tempSring, nRow );
        }
#else
		pInfo->m_dwCategory1		= GetDataDWORD( "bCategory1", nRow );
		pInfo->m_Category1NCode		= GetDataDWORD( "Category1_Scode", nRow );
		pInfo->m_dwCategory2		= GetDataDWORD( "bCategory2", nRow );
		pInfo->m_Category2NCode		= GetDataDWORD( "Category2_Scode", nRow );
#endif
		pInfo->m_byMatCount = 0;
		{	// Materials 재료물...
			TCHAR materialCodeString[32] = { 0, };
			TCHAR materialInfoString[32] = { 0, };
			TCHAR materialCountString[32] = { 0, };

			const INT material_code_tail_idx = _sntprintf( materialCodeString, _countof(materialCodeString), "MaterialCode" );
			const INT material_Info_tail_idx = _sntprintf( materialInfoString, _countof(materialInfoString), "IGType" );
			const INT material_count_tail_idx = _sntprintf( materialCountString, _countof(materialCountString), "MaterialCount" );
			for( char i=0 ; i<sITEM_COMPOSE_LIST::MAX_COMPOSE_ITEM_NUM ; ++i )
			{
				materialCodeString[material_code_tail_idx] = '1' + i;
				materialInfoString[material_Info_tail_idx] = '1' + i;
				materialCountString[material_count_tail_idx] = '1' + i;

				pInfo->m_pMaterial[i].m_ItemCode		= GetDataWORD( materialCodeString, nRow );
				pInfo->m_pMaterial[i].m_ItemTypeIndex	= GetDataDWORD( materialInfoString, nRow );
				pInfo->m_pMaterial[i].m_byItemNum		= GetDataBYTE( materialCountString, nRow );

				if( 0 != pInfo->m_pMaterial[i].m_ItemCode )
				{
					FASSERT( 0 != pInfo->m_pMaterial[i].m_byItemNum );
					++pInfo->m_byMatCount;
				}
			}
		};

		pInfo->m_Money				= (MONEY)GetDataNumber( "Money", nRow );	// <--- 음... 이 타입은 어캐 읽는 지 확인해 볼 것.
		// 결과물
		pInfo->m_byCreateRatio		= GetDataBYTE( "SuccessRatio", nRow );
		pInfo->m_byResultType		= GetDataBYTE( "bResultType", nRow );

		FASSERT( pInfo->m_byResultType == sITEM_COMPOSE_LIST::RESULTTYPE_ITEMCODE || 
			pInfo->m_byResultType == sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE );

		pInfo->m_Result.m_ItemCode	= GetDataWORD( "ResultItemCode", nRow );
		pInfo->m_Result.m_ItemTypeIndex	= GetDataWORD( "ResultIGType", nRow );
		pInfo->m_Result.m_byItemNum	= GetDataBYTE( "ResultItemCount", nRow );

		{	// 조합 가능 맵 코드 목록
			TCHAR allowedMapCode[32] = { 0, };
			const INT allowedMapCode_tail_idx = _sntprintf( allowedMapCode, _countof(allowedMapCode), "AllowedMapCode" );
			for( INT i=0 ; i<sITEM_COMPOSE_LIST::MAX_COMPOSE_MAP_NUM ; ++i )
			{
				const BYTE index = (BYTE)i;
				const BYTE carry = index/10;
				const BYTE remain = index%10;
				allowedMapCode[allowedMapCode_tail_idx] = '0' + carry;
				allowedMapCode[allowedMapCode_tail_idx+1] = '0' + remain;
				pInfo->m_MapCode[i]	= GetDataWORD( allowedMapCode, nRow );
			}
		};

#ifndef _SERVER //Client Only
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        DWORD category_order = MAKE_CATEGORY_ORDER(pInfo->m_Code, pInfo->m_pCategory[0], pInfo->m_pCategory[1]);
        m_ComposeCategoryMap.insert(ComposeCategotyMap::value_type(category_order, pInfo));
#else
        DWORD category_order = MAKE_CATEGORY_ORDER(pInfo->m_Code, pInfo->m_dwCategory1, pInfo->m_dwCategory2);
        m_ComposeCategoryMap.insert(ComposeCategotyMap::value_type(category_order, pInfo));
#endif
#endif //_SERVER

		ValidCheck(pInfo);
	}

	return TRUE;
}


VOID ItemComposeListParser::Reload()
{
}


VOID ItemComposeListParser::Unload()
{
	// 조합정보 내림
	if( m_pComposeListHash )
	{
		m_pComposeListHash->SetFirst();
		while(sITEM_COMPOSE_LIST* pComposeList = m_pComposeListHash->GetNext())
		{
			SAFE_DELETE( pComposeList );
		}
		m_pComposeListHash->RemoveAll();
	}
}

//BOOL	ItemComposeListParser::LoadEx( eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bReload )
//{
//	FASSERT( m_pComposeListHash != NULL );
//	return IParser::LoadEx( ScriptCode, pszFileName, bReload );
//}

BOOL
ItemComposeListParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch( scriptCode )
	{
	case SCRIPT_ITEMCOMPOSELIST:
		return _Load( bReload );
	}

	return FALSE;
}

VOID ItemComposeListParser::ValidCheck(sITEM_COMPOSE_LIST* pComposeInfo)
{
	CODETYPE ItemCode = 0;
	BASE_ITEMINFO *pItemInfo = NULL;

	//재로는 체크 하지말자.
	/*
	for( int i = 0 ; i <sITEM_COMPOSE_LIST::MAX_COMPOSE_ITEM_NUM; i++ )
	{
		ItemCode = pComposeInfo->m_pMaterial[i].m_ItemCode;
		if( ItemCode != 0 )
		{
			pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCode);
			if(pItemInfo)
			{
				if( (pItemInfo->m_byDupNum > 1) && pComposeInfo->m_pMaterial[i].m_ItemTypeIndex != 0)
					SUNLOG( eCRITICAL_LOG, "[ItemComposeListParser] Index(%u) Not Armor or Weapon ItemCode(%u)\n",pComposeInfo->m_Code, ItemCode );
			}
			else
			{
				SUNLOG( eCRITICAL_LOG, "[ItemComposeListParser] Index(%u) Not Exist Item Code(%u) \n",pComposeInfo->m_Code, ItemCode );
			}
		}
	}
	*/

	ItemCode = pComposeInfo->m_Result.m_ItemCode;
	if( ItemCode != 0 )
	{
		pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCode);
		if(pItemInfo)
		{
			if( (pItemInfo->m_byDupNum > 1) && pComposeInfo->m_Result.m_ItemTypeIndex != 0)
				SUNLOG( eCRITICAL_LOG, "[ItemComposeListParser] Index(%u) Not Armor or Weapon ItemCode(%u)\n",pComposeInfo->m_Code, ItemCode );
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "[ItemComposeListParser] Index(%u) Not Exist Item Code(%u) \n",pComposeInfo->m_Code, ItemCode );
		}
	}
}
