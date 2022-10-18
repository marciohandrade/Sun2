#include "StdAfx.h"

#include ".\CashCategoryParser.h"

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



CashCategoryParser::CashCategoryParser(void)
{
}

CashCategoryParser::~CashCategoryParser(void)
{
}

VOID CashCategoryParser::Init()
{
}

VOID CashCategoryParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID CashCategoryParser::Release()
{
}

BOOL CashCategoryParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
    const int row_count = GetRowSize();

    category_hashmap_.clear();
    uint itemcode = 0;
    uint category_index = 0;
    for (int row = 0 ; row < row_count ; ++row)
    {
        itemcode = GetDataDWORD("code", row);
        category_index = GetDataDWORD("category", row);
        if (!category_hashmap_.insert(CategoryHashMap::value_type(itemcode, category_index)).second)
            SUNLOG(eCRITICAL_LOG, "[CashCategoryParser] already exist itemcode[%d]", itemcode);
        category_hashmap_[itemcode] = category_index;
    }
    return TRUE;
}

BOOL CashCategoryParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch (scriptCode)
	{
	case SCRIPT_CASHCATEGORY:
		return _Load( bReload );
	}

    return FALSE;	
}

#endif //#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

