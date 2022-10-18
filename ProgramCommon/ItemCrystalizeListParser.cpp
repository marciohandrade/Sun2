#include "StdAfx.h"
#include ".\itemcrystalizelistparser.h"


ItemCrystalizeListParser::ItemCrystalizeListParser(void):
m_pCrystalizeListHash(NULL)
{
	m_pszFileName[0] = '\0';
}

ItemCrystalizeListParser::~ItemCrystalizeListParser(void)
{
	FASSERT( m_pCrystalizeListHash == NULL );
	Release();
}

VOID ItemCrystalizeListParser::Init( DWORD dwComposeListSize )
{
	FASSERT( m_pCrystalizeListHash == NULL );
	m_pCrystalizeListHash = new _CRYSTALIZELIST_HASH;
	m_pCrystalizeListHash->Initialize( dwComposeListSize );
}

VOID ItemCrystalizeListParser::Init( DWORD dwComposeListSize, char * pszPackFileName )
{
	Init( dwComposeListSize );
	SetPackFileName( pszPackFileName );
}

VOID ItemCrystalizeListParser::Release()
{
	Unload();

	SAFE_DELETE( m_pCrystalizeListHash );
}

VOID ItemCrystalizeListParser::Reload()
{
//	Load( this->m_pszFileName, TRUE );
}


VOID ItemCrystalizeListParser::Unload()
{
	// 조합정보 내림
	if( m_pCrystalizeListHash )
	{
		m_pCrystalizeListHash->SetFirst();
		while(sITEM_CRYSTALIZE_LIST * pList = m_pCrystalizeListHash->GetNext())
		{
			SAFE_DELETE( pList );
		}
		m_pCrystalizeListHash->RemoveAll();
	}

    // [10/22/2009 namka199]
    RemoveCrystallizeSelector();
}

VOID ItemCrystalizeListParser::RemoveCrystallizeSelector()
{
    if( !m_CrystallizeSelector.empty() )
    {
        CRYSTALLIZE_SELECTOR::iterator it = m_CrystallizeSelector.begin();
        for( ; it != m_CrystallizeSelector.end() ; ++it )
        {
            SAFE_DELETE(it->second);
        }
        m_CrystallizeSelector.clear();
    }
}

BOOL	ItemCrystalizeListParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
    switch( scriptCode )
    {
    case SCRIPT_ITEMCRYSTALIZELIST:
        return _Load( bReload );
    }

    return FALSE;	
}

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE

VOID ItemCrystalizeListParser::AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo )
{
    FASSERT( pInfo );
    sCRYSTALLIZE_SELECTOR* pItem = NULL;
    CRYSTALLIZE_SELECTOR::iterator it = m_CrystallizeSelector.find(wItemType);
    if( it != m_CrystallizeSelector.end() )
    {
        pItem = it->second;
    }
    else
    {
        pItem = new sCRYSTALLIZE_SELECTOR;
        pItem->m_Count = 0;
    }

    pItem->values.push_back(sCRYSTALLIZE_SELECTOR::sVALUE(pInfo->m_Code, pInfo->m_ItemLevel, pInfo->m_EnchantLv, pInfo->item_subtype));
    pItem->m_Count++;
    m_CrystallizeSelector.insert( CRYSTALLIZE_SELECTOR::value_type(wItemType, pItem) );
}

BOOL ItemCrystalizeListParser::_Load( BOOL bReload )
{
    TCHAR szFieldName[64] = { 0, };
    const INT nRowSize = GetRowSize();
    for( INT nRow=0; nRow<nRowSize ; ++nRow )
    {
        const SLOTCODE code				= GetDataWORD( "Code", nRow );
        sITEM_CRYSTALIZE_LIST* pInfo	= m_pCrystalizeListHash->GetData( code );
        if( !pInfo )
        {
            // 없으면!
            pInfo = new sITEM_CRYSTALIZE_LIST;
            m_pCrystalizeListHash->Add( pInfo, code );
        }
        else
        {
            if( FALSE == bReload )
            {
                SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", code );
                FASSERT( !"데이터에 오류가 있습니다." );
            }
        }
        pInfo->m_Code = code;

        _tcsncpy( pInfo->m_pszDebugName, GetDataString( "Name", nRow ), MAX_ITEMNAME_LENGTH );
        
        pInfo->m_ItemType  = GetDataWORD( "ItemType", nRow );
        pInfo->item_subtype = GetDataWORD( "ItemSubType", nRow);
        pInfo->m_ItemLevel = GetDataWORD( "ItemLevel", nRow );
        pInfo->m_EnchantLv = GetDataWORD( "EnchantLV", nRow );

        // 분해물
        {
            for( INT i = 0 ; i < sITEM_CRYSTALIZE_LIST::MAX_MATERIALS ; ++i )
            {
                _sntprintf(szFieldName, _countof(szFieldName), "ResultRatio%d", i+1);
                pInfo->m_pResult[i].m_ResultRatio   = GetDataBYTE( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialCode%d", i+1);
                pInfo->m_pResult[i].m_MaterialCode  = GetDataWORD( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialCount%d", i+1);
                pInfo->m_pResult[i].m_MaterialCount	= GetDataBYTE( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialMoney%d", i+1);
                pInfo->m_pResult[i].m_MaterialMoney	= (MONEY)GetDataNumber( szFieldName, nRow );

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
#else
                if( 0 != pInfo->m_pResult[i].m_MaterialCode )
                {
                    FASSERT( 0 != pInfo->m_pResult[i].m_MaterialCount );
                }
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
            }
        };

        // [10/20/2009 namka199]
        AddCrystallizeSelector( pInfo->m_ItemType, pInfo );
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**	클라이언트에서 사용 - j0
*/

// re-implemented by _KR_001399_20091020_CRYSTALLIZE_EXTEND
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
SLOTCODE ItemCrystalizeListParser::GetCrystalizeCode( SCItemSlot & rItemSlot, BYTE enchant_level )
#else
SLOTCODE ItemCrystalizeListParser::GetCrystalizeCode( SCItemSlot & rItemSlot )
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
{
    SLOTCODE Code = 0;
    const BASE_ITEMINFO* const pItemInfo = rItemSlot.GetItemInfo();
    if( !pItemInfo )
        return Code;

    if( m_CrystallizeSelector.empty() )
        return Code;

    sCRYSTALLIZE_SELECTOR* pSelector = NULL;
    CRYSTALLIZE_SELECTOR::iterator it = m_CrystallizeSelector.find( pItemInfo->m_wType );
    if( it != m_CrystallizeSelector.end() )
        pSelector = it->second;
    else
        return Code;

    // 특정 조건 설정 : 악세사리인 경우만 레벨 검사. 기본값 0.
    LEVELTYPE Level = pItemInfo->m_LV;
    WORD item_subtype = 0;

    if (rItemSlot.IsDivine() == true){
        item_subtype = eITEMSUBTYPE_DIVINE;
    }
    else {
        item_subtype = pItemInfo->m_wSubType;
    }

    for( WORD i = 0 ; i < pSelector->m_Count ; ++i )
    {
        if( (pSelector->values[i].m_ItemLevel > 0 ? pSelector->values[i].m_ItemLevel == Level : true) && // 레벨이 0보다 클 경우에만 검사
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
            (pSelector->values[i].m_EnchantLevel == (LEVELTYPE)enchant_level) &&
#else
            (pSelector->values[i].m_EnchantLevel == (LEVELTYPE)rItemSlot.GetEnchant()) &&
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
            (pSelector->values[i].item_subtype == item_subtype) )
        {
            Code = pSelector->values[i].m_Code;
            break;
        }
    }

    return Code;
}
#else

VOID ItemCrystalizeListParser::AddCrystallizeSelector( WORD wItemType, sITEM_CRYSTALIZE_LIST* pInfo )
{
    FASSERT( pInfo );
    sCRYSTALLIZE_SELECTOR* pItem = NULL;
    CRYSTALLIZE_SELECTOR::iterator it = m_CrystallizeSelector.find(wItemType);
    if( it != m_CrystallizeSelector.end() )
    {
        pItem = it->second;
    }
    else
    {
        pItem = new sCRYSTALLIZE_SELECTOR;
        pItem->m_Count = 0;
    }

    FASSERT( pItem && (pItem->m_Count < sCRYSTALLIZE_SELECTOR::MAX_COUNT) );

    WORD Count = pItem->m_Count;
    pItem->m_Value[Count].m_Code = pInfo->m_Code;
    pItem->m_Value[Count].m_ItemLevel = pInfo->m_ItemLevel;
    pItem->m_Value[Count].m_EnchantLevel = pInfo->m_EnchantLv;
    pItem->m_Value[Count].m_LuckyItemType = pInfo->m_LuckyItemType;
    pItem->m_Count++;
    m_CrystallizeSelector.insert( CRYSTALLIZE_SELECTOR::value_type(wItemType, pItem) );
}

BOOL ItemCrystalizeListParser::_Load( BOOL bReload )
{
    TCHAR szFieldName[64] = { 0, };
    const INT nRowSize = GetRowSize();
    for( INT nRow=0; nRow<nRowSize ; ++nRow )
    {
        const SLOTCODE code				= GetDataWORD( "Code", nRow );
        sITEM_CRYSTALIZE_LIST* pInfo	= m_pCrystalizeListHash->GetData( code );
        if( !pInfo )
        {
            // 없으면!
            pInfo = new sITEM_CRYSTALIZE_LIST;
            m_pCrystalizeListHash->Add( pInfo, code );
        }
        else
        {
            if( FALSE == bReload )
            {
                SUNLOG( eCRITICAL_LOG, "Script File Data Error, Code = %d", code );
                FASSERT( !"데이터에 오류가 있습니다." );
            }
        }
        pInfo->m_Code = code;

        _tcsncpy( pInfo->m_pszDebugName, GetDataString( "Name", nRow ), MAX_ITEMNAME_LENGTH );
        pInfo->m_NCode	   = GetDataDWORD( "Ncode", nRow );
        pInfo->m_ItemType  = GetDataWORD( "ItemType", nRow );
        pInfo->m_ItemLevel = GetDataWORD( "ItemLevel", nRow );
        pInfo->m_EnchantLv = GetDataWORD( "EnchantLV", nRow );

        // 분해물
        {
            for( INT i = 0 ; i < sITEM_CRYSTALIZE_LIST::MAX_RESULT ; ++i )
            {
                _sntprintf(szFieldName, _countof(szFieldName), "ResultRatio%d", i+1);
                pInfo->m_pResult[i].m_ResultRatio   = GetDataBYTE( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialCode%d", i+1);
                pInfo->m_pResult[i].m_MaterialCode  = GetDataWORD( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialCount%d", i+1);
                pInfo->m_pResult[i].m_MaterialCount	= GetDataBYTE( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "MaterialMoney%d", i+1);
                pInfo->m_pResult[i].m_MaterialMoney	= (MONEY)GetDataNumber( szFieldName, nRow );

                if( 0 != pInfo->m_pResult[i].m_MaterialCode )
                {
                    FASSERT( 0 != pInfo->m_pResult[i].m_MaterialCount );
                }
            }
        };



        // 랜덤 분해물
        {
            for( INT i = 0 ; i < sITEM_CRYSTALIZE_LIST::MAX_RANDOMRESULT ; ++i )
            {
                _sntprintf(szFieldName, _countof(szFieldName), "RandomRatio%d", i+1);
                pInfo->m_pRandomResult[i].m_RandomRatio	        = GetDataDWORD( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "RandomStandardRatio%d", i+1);
                pInfo->m_pRandomResult[i].m_RandomStandardRatio = GetDataDWORD( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "RandomItemCode%d", i+1);
                pInfo->m_pRandomResult[i].m_MaterialCode        = GetDataWORD( szFieldName, nRow );
                _sntprintf(szFieldName, _countof(szFieldName), "RandomItemCount%d", i+1);
                pInfo->m_pRandomResult[i].m_MaterialCount       = GetDataBYTE( szFieldName, nRow );

                FASSERT( pInfo->m_pRandomResult[i].m_RandomRatio <= pInfo->m_pRandomResult[i].m_RandomStandardRatio );

                if( 0 != pInfo->m_pRandomResult[i].m_MaterialCode )
                {
                    FASSERT( 0 != pInfo->m_pRandomResult[i].m_MaterialCount );
                }
            }
        };

        pInfo->m_LuckyItemType = GetDataWORD( "LuckyItemType", nRow );

        // [10/20/2009 namka199]
        AddCrystallizeSelector( pInfo->m_ItemType, pInfo );
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**	클라이언트에서 사용 - j0
*/

// re-implemented by _KR_001399_20091020_CRYSTALLIZE_EXTEND
SLOTCODE ItemCrystalizeListParser::GetCrystalizeCode( SCItemSlot & rItemSlot )
{
    SLOTCODE Code = 0;
    const BASE_ITEMINFO* const pItemInfo = rItemSlot.GetItemInfo();
    if( !pItemInfo )
        return Code;

    if( m_CrystallizeSelector.empty() )
        return Code;

    sCRYSTALLIZE_SELECTOR* pSelector = NULL;
    CRYSTALLIZE_SELECTOR::iterator it = m_CrystallizeSelector.find( pItemInfo->m_wType );
    if( it != m_CrystallizeSelector.end() )
        pSelector = it->second;
    else
        return Code;

    // 특정 조건 설정 : 악세사리인 경우만 레벨 검사. 기본값 0.
    LEVELTYPE Level = 0;
    switch(pItemInfo->m_wType)
    {
    case eITEMTYPE_RING:			
    case eITEMTYPE_NECKLACE:		
    case eITEMTYPE_BERSERKER_SACCESSORY:
    case eITEMTYPE_DRAGON_SACCESSORY:	
    case eITEMTYPE_SHADOW_SACCESSORY:	
    case eITEMTYPE_VALKYRIE_SACCESSORY:	
    case eITEMTYPE_ELEMENTALIST_SACCESSORY:
    case eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        Level = pItemInfo->m_LV;
        break;
    }

#define _MOD_ELITE_ITEM_CRYSTALIZE  // 엘리트아이템 결정화 관련 수정
#ifdef _MOD_ELITE_ITEM_CRYSTALIZE
    // 특정 조건 설정 : 아이템의 2차타입에 해당하는 조건 검사
    // 0 : 일반       아이템. 기타등등 : NORMAL_ITEM_TYPE
    // 1 : 럭키       아이템           : LUCKY_ITEM_TYPE
    // 2 : 엘리트1차  아이템           : ELITE_1ST_ITEM_TYPE
    // 3 : 엘리트1.5차아이템           : ELITE_1ST_HALF_ITEM_TYPE
    // 4 : 엘리트2차  아이템           : ELITE_2ND_ITEM_TYPE
    // 5 : 엘리트2.5차아이템           : ELITE_2ND_HALF_ITEM_TYPE
    // 6 : 엘리트3차  아이템           : ELITE_3RD_ITEM_TYPE
    // 7 : 엘리트3.5차아이템           : ELITE_3RD_HALF_ITEM_TYPE
	// 8 : 엘리트4차  아이템           : ELITE_4RD_ITEM_TYPE
    WORD LuckyItemType = sITEM_CRYSTALIZE_LIST::NORMAL_ITEM_TYPE;
    switch(pItemInfo->m_wSubType)
    {
    case eITEMSUBTYPE_FATE: 
        LuckyItemType = sITEM_CRYSTALIZE_LIST::LUCKY_ITEM_TYPE; 
        break;
    case eITEMSUBTYPE_ELITE:
        {
            switch (pItemInfo->m_LV)
            {
            case sITEM_CRYSTALIZE_LIST::ELITE_1ST_ITEM_LEVEL:// 엘리트1차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_1ST_ITEM_TYPE;
                break;
            case sITEM_CRYSTALIZE_LIST::ELITE_1ST_HALF_ITEM_LEVEL:// 엘리트 1.5차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_1ST_HALF_ITEM_TYPE;
                break;
            case sITEM_CRYSTALIZE_LIST::ELITE_2ND_ITEM_LEVEL:// 엘리트 2차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_2ND_ITEM_TYPE;
                break;
            case sITEM_CRYSTALIZE_LIST::ELITE_2ND_HALF_ITEM_LEVEL: // 엘리트 2.5차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_2ND_HALF_ITEM_TYPE;
                break;
            case sITEM_CRYSTALIZE_LIST::ELITE_3RD_ITEM_LEVEL: // 엘리트 3차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_3RD_ITEM_TYPE;
                break;
            case sITEM_CRYSTALIZE_LIST::ELITE_3RD_HALF_ITEM_LEVEL: // 엘리트 3.5차
                LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_3RD_HALF_ITEM_TYPE;
                break;
			case sITEM_CRYSTALIZE_LIST::ELITE_4RD_ITEM_TYPE: // 엘리트 4차
				LuckyItemType = sITEM_CRYSTALIZE_LIST::ELITE_4RD_ITEM_TYPE;
				break;
            }
        }        
        break;
    }    
#else
    // 특정 조건 설정 : 아이템의 2차타입에 해당하는 조건 검사
    // 0 : 일반   아이템. 기타등등
    // 1 : 럭키   아이템
    // 2 : 엘리트 아이템
    WORD LuckyItemType = 0;
    switch(pItemInfo->m_wSubType)
    {
    case eITEMSUBTYPE_FATE: 
        LuckyItemType = 1; 
        break;
    case eITEMSUBTYPE_ELITE:
    case eITEMSUBTYPE_ELITE_SSQ: // (f100730.3L)
        LuckyItemType = 2; 
        break;
    }
#endif

    for( WORD i = 0 ; i < pSelector->m_Count ; ++i )
    {
        if( (pSelector->m_Value[i].m_ItemLevel == Level) &&
            (pSelector->m_Value[i].m_EnchantLevel == (LEVELTYPE)rItemSlot.GetEnchant()) &&
            (pSelector->m_Value[i].m_LuckyItemType == LuckyItemType) )
        {
            Code = pSelector->m_Value[i].m_Code;
            break;
        }
    }

    return Code;
}
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
