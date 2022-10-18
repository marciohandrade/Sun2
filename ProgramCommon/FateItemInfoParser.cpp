#include "stdafx.h"
#include "./FateItemInfoParser.h"

#pragma warning(push, 4)

//==================================================================================================

FateItemInfoParser::FateItemInfoParser()
{
}

FateItemInfoParser::~FateItemInfoParser()
{
    Unload();
}

void FateItemInfoParser::Release()
{
    Unload();
}

BOOL FateItemInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_FATE_ITEM_INFO:
        return _Load(reloaded);
    }

    return false;
}

//==================================================================================================

void FateItemInfoParser::Unload()
{
    LUCKY_ITEM_INFO_HASH::const_iterator it(lucky_item_table_.begin()),
                                         end(lucky_item_table_.end());
    for (; it != end; ++it)
    {
        LUCKY_ITEM_INFO* lucky_info = it->second;
        const DWORD lucky_index = lucky_info->m_dwIndex;

        SAFE_DELETE(fate_item_durability_selector_[lucky_index]);
        SAFE_DELETE(fate_item_atkdef_selector_[lucky_index]);
        SAFE_DELETE(fate_item_time_selector_[lucky_index]);
    #if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
        SAFE_DELETE(fate_item_socket_selector_[lucky_index]);
    #endif

        SAFE_DELETE(lucky_info);
    };
    //
    lucky_item_table_.clear();
    //
    fate_item_ref_item_table_.clear();
    fate_item_durability_selector_.clear();
    fate_item_atkdef_selector_.clear();
    fate_item_time_selector_.clear();
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    fate_item_socket_selector_.clear();
#endif
}

bool FateItemInfoParser::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    const int max_rows = GetRowSize();
    for (int row = 0; row < max_rows; ++row)
    {
        const DWORD lucky_index = GetDataWORD("wIndex", row);
        LUCKY_ITEM_INFO_HASH::iterator found_lucky_it = lucky_item_table_.find(lucky_index);
        if (found_lucky_it != lucky_item_table_.end())
        {
            SUNLOG(eCRITICAL_LOG, _T("Script File Data Error, File = %s, Code = %d"),
                   GetCurFileName(), lucky_index);
            FASSERT(!"데이터에 오류가 있습니다.");
            return false;
        };
        //
        DWORD ref_item_code = GetDataWORD("wBasicItemIndex", row);
        //
        LUCKY_ITEM_INFO* const lucky_info = new LUCKY_ITEM_INFO;
        ZeroMemory(lucky_info, sizeof(*lucky_info));

        ReliableRatioRandomizerEX<MAX_FATE_DURA_RATIO_COUNT>* durability_selector =
            new ReliableRatioRandomizerEX<MAX_FATE_DURA_RATIO_COUNT>;
        ReliableRatioRandomizerEX<MAX_FATE_ATK_DEF_RATIO_COUNT>* atkdef_selector =
            new ReliableRatioRandomizerEX<MAX_FATE_ATK_DEF_RATIO_COUNT>;
        ReliableRatioRandomizer<MAX_FATE_USING_TIME_RATIO_COUNT>* time_selector =
            new ReliableRatioRandomizer<MAX_FATE_USING_TIME_RATIO_COUNT>;
    #if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
        ReliableRatioRandomizer<MAX_FATE_SOCKET_COUNT>* socket_selector =
            new ReliableRatioRandomizer<MAX_FATE_SOCKET_COUNT>;
    #endif
        //
        lucky_item_table_[lucky_index] = lucky_info;
        fate_item_ref_item_table_[ref_item_code] = lucky_info;

        fate_item_durability_selector_[lucky_index] = durability_selector;
        fate_item_atkdef_selector_[lucky_index] = atkdef_selector;
        fate_item_time_selector_[lucky_index] = time_selector;
    #if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
        fate_item_socket_selector_[lucky_index] = socket_selector;
    #endif

        lucky_info->m_dwIndex = lucky_index;
        lucky_info->m_dwRefItemIndex = ref_item_code;

        //GetDataString("sPszName", row); //사용 안함. 버림

        char column_buffer[32 + 1] = {0,};
        // 내구도
        lucky_info->m_dwFateDuraRatioSum = 0;
        for (int i = 0; i < MAX_FATE_DURA_RATIO_COUNT; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "bSection_Rate_1_%d", i + 1);
            lucky_info->m_dwDuraRatio[i] = GetDataDWORD(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "wBasic_Rmin_1_%d", i + 1);
            lucky_info->m_wDura_Min[i] = GetDataWORD(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "wBasic_Rmax_1_%d", i + 1);
            lucky_info->m_wDura_Max[i] = GetDataWORD(column_buffer, row);

            lucky_info->m_dwFateDuraRatioSum += lucky_info->m_dwDuraRatio[i];
        }

        //방어력 / 데미지 (물리, 마법)
        lucky_info->m_dwAtkDefRatioSum = 0;
        for (int i = 0; i < MAX_FATE_ATK_DEF_RATIO_COUNT; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "bSection_Rate_2_%d", i + 1);
            lucky_info->m_dwAtkDefRatio[i] = GetDataDWORD(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "wBasic_Rmin_2_%d", i + 1);
            lucky_info->m_wAtkDef_Min[i] = GetDataWORD(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "wBasic_Rmax_2_%d", i + 1);
            lucky_info->m_wAtkDef_Max[i] = GetDataWORD(column_buffer, row);

            lucky_info->m_dwAtkDefRatioSum += lucky_info->m_dwAtkDefRatio[i];
        }

        //사용 기간
        lucky_info->m_dwUsingTimeRatioSum = 0;
        for (int i = 0; i < MAX_FATE_USING_TIME_RATIO_COUNT; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "bSection_Rate_3_%d", i + 1);
            lucky_info->m_dwUsingTimeRatio[i] = GetDataDWORD(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "wUseLimitTime_%d", i + 1);
            lucky_info->m_dwUsingTimeValue[i] = GetDataDWORD(column_buffer, row);

            lucky_info->m_dwUsingTimeRatioSum += lucky_info->m_dwUsingTimeRatio[i];
        }

    #if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
        //소켓
        lucky_info->m_dwSocketRatioSum = 0;
        for (int i = 0; i < MAX_FATE_SOCKET_COUNT; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "SocketRate_%d", i);
            lucky_info->m_dwSocketRatio[i] = GetDataDWORD(column_buffer, row);

            lucky_info->m_dwSocketRatioSum += lucky_info->m_dwSocketRatio[i];
        }
    #endif

        if (ValidCheck(lucky_info))
        {
            ASSERT(false);
        }

        //랜덤 생성기에 입력
        //내구도
        for (int i = 0; i < MAX_FATE_DURA_RATIO_COUNT; ++i)
        {
            durability_selector->AddRatio(i + 1,
                lucky_info->m_dwDuraRatio[i], lucky_info->m_dwFateDuraRatioSum);
            durability_selector->SetMin(lucky_info->m_wDura_Min[i], i + 1);
            durability_selector->SetMax(lucky_info->m_wDura_Max[i], i + 1);
        }
        //방어력/공격력
        for (int i = 0; i < MAX_FATE_ATK_DEF_RATIO_COUNT; ++i)
        {
            atkdef_selector->AddRatio(i + 1,
                lucky_info->m_dwAtkDefRatio[i], lucky_info->m_dwAtkDefRatioSum);
            atkdef_selector->SetMin(lucky_info->m_wAtkDef_Min[i], i + 1);
            atkdef_selector->SetMax(lucky_info->m_wAtkDef_Max[i], i + 1);
        }

        //사용기간
        for (int i = 0; i < MAX_FATE_USING_TIME_RATIO_COUNT; ++i)
        {
            time_selector->AddRatio(i + 1,
                lucky_info->m_dwUsingTimeRatio[i], lucky_info->m_dwUsingTimeRatioSum);
        }

    #if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
        //소켓
        for (int i = 0; i < MAX_FATE_SOCKET_COUNT; ++i)
        {
            socket_selector->AddRatio(i + 1,
                lucky_info->m_dwSocketRatio[i], lucky_info->m_dwSocketRatioSum);
        }
    #endif
    }; //end 'foreach row in rows'

    return true;
}


WORD FateItemInfoParser::ValidCheck(LUCKY_ITEM_INFO* lucky_info)
{
    BOOST_STATIC_ASSERT(_countof(lucky_info->m_wDura_Min) == _countof(lucky_info->m_wDura_Max));
    for (int i = 0; i < _countof(lucky_info->m_wDura_Min); ++i)
    {
        if (lucky_info->m_wDura_Min[i] > 500) {
            return 1;
        };
        if (lucky_info->m_wDura_Max[i] > 500) {
            return 2;
        };
    }
    BOOST_STATIC_ASSERT(_countof(lucky_info->m_wAtkDef_Min) == _countof(lucky_info->m_wAtkDef_Max));
    for (int i = 0; i < _countof(lucky_info->m_wAtkDef_Min); ++i)
    {
        if (lucky_info->m_wAtkDef_Min[i] > 500) {
            return 3;
        };
        if (lucky_info->m_wAtkDef_Max[i] > 500) {
            return 4;
        };
    }

    return 0;
}

sFATE_ITEM_INFO* FateItemInfoParser::GetFateItemInfoByIndex(DWORD lucky_index) const
{
    LUCKY_ITEM_INFO_HASH::const_iterator found = lucky_item_table_.find(lucky_index);
    if (found != lucky_item_table_.end()) {
        LUCKY_ITEM_INFO* lucky_info = found->second;
        return lucky_info;
    };
    return NULL;
}

sFATE_ITEM_INFO* FateItemInfoParser::GetFateItemInfoByRefItemCode(DWORD ref_item_code) const
{
    LUCKY_ITEM_INFO_HASH::const_iterator found = fate_item_ref_item_table_.find(ref_item_code);
    if (found != fate_item_ref_item_table_.end()) {
        LUCKY_ITEM_INFO* lucky_info = found->second;
        return lucky_info;
    };
    return NULL;
}

WORD FateItemInfoParser::GetRandomkeyFromFateDuraRatio(DWORD lucky_index)
{
    int random_number = fate_item_durability_selector_[lucky_index]->GetRandomizedValue();

    if (random_number < 0) {
        random_number = 0;
    };
    return static_cast<WORD>(random_number);
}

WORD FateItemInfoParser::GetRandomkeyFromFateAtkDefRatio(DWORD lucky_index)
{
    int random_number = fate_item_atkdef_selector_[lucky_index]->GetRandomizedValue();

    if (random_number < 0) {
        random_number = 0;
    };
    return static_cast<WORD>(random_number);
}

DWORD FateItemInfoParser::GetRandomkeyFromFateUsingTimeRatio(DWORD lucky_index)
{
    DWORD random_number = fate_item_time_selector_[lucky_index]->RandomizedKey();
    --random_number;
    if (random_number >= MAX_FATE_USING_TIME_RATIO_COUNT) {
        random_number = 0;
    };
    sFATE_ITEM_INFO* lucky_info = GetFateItemInfoByIndex(lucky_index);

    return lucky_info->m_dwUsingTimeValue[random_number];
}

#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
WORD FateItemInfoParser::GetRandomkeyFromFateSocketRatio(DWORD lucky_index)
{
    DWORD random_number = fate_item_socket_selector_[lucky_index]->RandomizedKey();
    --random_number;
    if (random_number >= MAX_FATE_SOCKET_COUNT) {
        random_number = 0;
    };
    return static_cast<WORD>(random_number);
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

sFATE_ITEM_INFO* FateItemInfoParser::GetNext()
{
    if (fate_item_iterator_ != lucky_item_table_.end())
    {
        sFATE_ITEM_INFO* lucky_info = fate_item_iterator_->second;
        ++fate_item_iterator_;
        return lucky_info;
    }

    return NULL;
}
