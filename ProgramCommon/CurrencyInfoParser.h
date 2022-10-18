#pragma once

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

enum eCurrencyInfo
{
    // ������ ȭ������
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
        DWORD currency_code; // ȭ���ڵ�
        DWORD currency_max_point; // �ִ������������Ʈ
        STLX_STRING currency_name; // ȭ���̸�(����׿�)
        DWORD currency_name_code; // ȭ���̸� ��Ʈ���ڵ�
        DWORD currency_icon_code; // ȭ������� �̹����ڵ�
        DWORD currency_description_code; // ȭ�󼳸� ��Ʈ���ڵ�
        DWORD currency_category_name_code; // ȭ��ī�װ� ��Ʈ���ڵ�
        BYTE currency_category; // ȭ��ī�װ�
        BYTE currency_category_order; // ȭ��ī�װ� �� ����
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