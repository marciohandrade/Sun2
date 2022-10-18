#pragma once

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

enum eCurrencyInfo
{
    // 유저의 화폐지갑
    CURRENCY_INFO_GUILD_COIN = 1,
    CURRENCY_INFO_GUILD_UP = 10,

    //
    CURRENCY_INFO_DONATION_GUILDCOIN = 10001,
    CURRENCY_INFO_GUILD_POINT,
};

class CurrencyInfoParser : public util::Singleton<CurrencyInfoParser>, public IParser
{
public:
    struct CurrencyInfo
    {
        DWORD currency_code; // 화폐코드
        DWORD currency_max_point; // 최대소지가능포인트
        STLX_STRING currency_name; // 화폐이름(디버그용)
        DWORD currency_name_code; // 화폐이름 스트링코드
        DWORD currency_icon_code; // 화폐아이콘 이미지코드
        DWORD currency_description_code; // 화폐설명 스트링코드
        DWORD currency_category_name_code; // 화폐카테고리 스트링코드
        BYTE currency_category; // 화폐카테고리
        BYTE currency_category_order; // 화폐카테고리 내 순서
    };

    typedef STLX_MAP<DWORD, CurrencyInfo*> CurrencyInfoMap;
    typedef STLX_MAP<DWORD, CurrencyInfo*> CurrencyCategoryMap;

public:
    CurrencyInfoParser(void);
    virtual	~CurrencyInfoParser(void);

    void Init();
    void Init(char* pack_filename);
    void Release();

    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    CurrencyInfo* GetCurrencyInfo(DWORD currency_code);

    CurrencyInfoMap& GetCurrencyInfoMap() { return currency_info_map_; }

    const CurrencyInfoMap& GetCurrencyInfoMap() const { return currency_info_map_; }

    CurrencyCategoryMap& GetCurrencyCategoryMap() { return currency_category_map_; }

    static DWORD CalcCategoryMapKey(BYTE currency_category, BYTE currency_category_order);

private:
    BOOL _Load(BOOL is_reload);

    CurrencyInfoMap currency_info_map_;
    CurrencyCategoryMap currency_category_map_;
};

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM