#include "StdAfx.h"

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include ".\CurrencyInfoParser.h"

CurrencyInfoParser::CurrencyInfoParser(void)
{
}

CurrencyInfoParser::~CurrencyInfoParser(void)
{
}

VOID CurrencyInfoParser::Init()
{
}

VOID CurrencyInfoParser::Init(char* pack_filename)
{
    SetPackFileName(pack_filename);
    Init();
}

VOID CurrencyInfoParser::Release()
{
    CurrencyInfoMap::iterator currency_info_itr = GetCurrencyInfoMap().begin();
    CurrencyInfoMap::iterator currency_info_end = GetCurrencyInfoMap().end();
    for ( ; currency_info_itr != currency_info_end; ++currency_info_itr)
    {
        CurrencyInfo* repute_info = currency_info_itr->second;
        SAFE_DELETE(repute_info);
    }
    GetCurrencyInfoMap().clear();

    GetCurrencyCategoryMap().clear();
}

BOOL CurrencyInfoParser::_Load(BOOL is_reload)
{
    __UNUSED(is_reload);//

    CurrencyInfoMap::iterator currency_info_itr;
    CurrencyInfoMap::iterator currency_info_end = GetCurrencyInfoMap().end();

    int row_size = GetRowSize();
    for (int row_index = 0; row_index < row_size; ++row_index)
    {
        // 화폐 포인트
        DWORD currency_code = GetDataDWORD("Code", row_index);

        currency_info_itr = GetCurrencyInfoMap().find(currency_code);
        if (currency_info_itr != currency_info_end)
        {
            // 중복 코드 체크
            SUNLOG(eCRITICAL_LOG, "Duplicate CurrencyInfo CurrencyCode(%d)", currency_code);
            continue;
        }

        //
        CurrencyInfo* currency_info = new CurrencyInfo;
        GetCurrencyInfoMap().insert(CurrencyInfoMap::value_type(currency_code, currency_info));

        // 화폐 코드
        currency_info->currency_code = currency_code;

        // 화폐 최대 소지 제한
        currency_info->currency_max_point = GetDataDWORD("MaxCoin", row_index);

        // 화폐 이름(디버그용)
        currency_info->currency_name = GetDataString("Name", row_index);

        // 화폐 이름 스트링코드
        currency_info->currency_name_code = GetDataDWORD("NCode", row_index);

        // 화폐 아이콘 이미지코드
        currency_info->currency_icon_code = GetDataDWORD("IconCode", row_index);

        // 화폐 이름 스트링코드
        currency_info->currency_description_code = GetDataDWORD("DCode", row_index);

        // 화폐 카테고리 스트링코드
        currency_info->currency_category_name_code = GetDataDWORD("CategoryNCode", row_index);

        // 화폐 카테고리
        currency_info->currency_category = GetDataBYTE("Category", row_index);

        // 화폐 카테고리 내 순서
        currency_info->currency_category_order = GetDataBYTE("Order", row_index);

        // 카테고리 맵 삽입
        if (currency_info->currency_code < CURRENCY_INFO_DONATION_GUILDCOIN)
        {
            DWORD category_map_key = CalcCategoryMapKey(currency_info->currency_category, 
                currency_info->currency_category_order);
            GetCurrencyCategoryMap().insert(
                CurrencyCategoryMap::value_type(category_map_key, currency_info));
        }
    }

    return TRUE;
}

BOOL CurrencyInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_CURRENCY_INFO:
        return _Load(is_reload);
    }

    return FALSE;	
}

CurrencyInfoParser::CurrencyInfo* CurrencyInfoParser::GetCurrencyInfo(DWORD currency_code)
{
    CurrencyInfo* currency_info = NULL;
    CurrencyInfoMap::iterator found_itr = GetCurrencyInfoMap().find(currency_code);
    if (found_itr != GetCurrencyInfoMap().end())
    {
        currency_info = found_itr->second;
    }

    return currency_info;
}

DWORD CurrencyInfoParser::CalcCategoryMapKey(BYTE currency_category, BYTE currency_category_order)
{
    DWORD category_map_key = ((currency_category<<8) | currency_category_order);
    return category_map_key;
}

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM