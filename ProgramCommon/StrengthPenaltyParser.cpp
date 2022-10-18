#include "stdafx.h"
#include "./StrengthPenaltyParser.h"

#ifdef __NA_001062_20080623_STRENGTH_PENALTY

//==================================================================================================
//==================================================================================================
//==================================================================================================

StrengthPenaltyParser* StrengthPenaltyParser::instance_static_ = NULL;

StrengthPenaltyParser* StrengthPenaltyParser::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(StrengthPenaltyParser** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        StrengthPenaltyParser** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    StrengthPenaltyParser* provider = new StrengthPenaltyParser;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

// old util::Singleton support interface
void StrengthPenaltyParser::DestroyInstance()
{
    SAFE_DELETE(instance_static_);
}

//==================================================================================================

sSTRENGTH_PENALTY::sSTRENGTH_PENALTY()
    : m_fStrengthPro(0.0f)
    , m_nStrengthUIIndex(0)
    , m_StrengthColorCode(0)
{
}

struct StrengthPenaltyParser::StrengthPenaltyRecord
{
    int index_;
    sSTRENGTH_PENALTY sections_[kMaxNumberOfStrengthPenaltySections];
    //
    __DISABLE_COPY(StrengthPenaltyRecord);
};

//==================================================================================================

StrengthPenaltyParser::StrengthPenaltyParser()
    : number_of_records_(0)
    , table_(NULL)
{
}

StrengthPenaltyParser::~StrengthPenaltyParser()
{
    Unload();
}

//void StrengthPenaltyParser::Init(char * pszPackFileName)
//{
//    SetPackFileName(pszPackFileName);
//}

BOOL StrengthPenaltyParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_STRENGTH_PENALTY:
        return _Load(reloaded);
    }

    return false;
}

void StrengthPenaltyParser::Release()
{
    Unload();
};

void StrengthPenaltyParser::Unload()
{
    if (table_) {
        ::free(table_);
        table_ = NULL;
    };
    number_of_records_ = 0;
}

__forceinline
StrengthPenaltyParser::StrengthPenaltyRecord*
StrengthPenaltyParser::FindStrengthPenalty(int index) const
{
    if (table_ == NULL) {
        return NULL;
    };
    StrengthPenaltyRecord* it = table_;
    const StrengthPenaltyRecord* const end = table_ + number_of_records_;
    for (; it != end; ++it)
    {
        if (it->index_ == index) {
            return it;
        };
    }
    return NULL;
}

int StrengthPenaltyParser::CalcStrengthToIdx(int max_durability, int cur_durability) const
{
    // NOTE: f110505.3L, remove 'Err_Idx' because this value is not controlled.
    enum {
        MinMaxPairNum = 2,
    };
    static const short kRatioSections[kMaxNumberOfStrengthPenaltySections][MinMaxPairNum] =
    {
        //  min,  max
        {  8100, 10000 }, // 81.00% ~ 100.00%
        {  6100,  8099 }, // 61.00% ~  80.99%
        {  4100,  6099 }, // 41.00% ~  60.99%
        {  2100,  4099 }, // 21.00% ~  40.99%
        {  1100,  2099 }, // 11.00% ~  20.99%
        {   600,  1099 }, //  6.00% ~  10.99%
        {   100,   599 }, //  1.00% ~   5.99%
        {   000,    99 }  //  0.00% ~   0.99%
    };
    BOOST_STATIC_ASSERT(_countof(kRatioSections) == kMaxNumberOfStrengthPenaltySections);
    if (cur_durability == 0) {
        return _countof(kRatioSections) - 1;
    };
    if (max_durability <= 0 || cur_durability < 0) {
        return _countof(kRatioSections) - 1;
    };
    // NOTE: prevent error if missing data is inputed in scripts = { strengthpenalty, items... }
    if (max_durability <= cur_durability) {
        return 0;
    };
    //
    cur_durability *= 10000;
    const short ratio = static_cast<short>(cur_durability / max_durability);
    for (int section_num = 0; section_num < _countof(kRatioSections); ++section_num)
    {
        if (ratio >= kRatioSections[section_num][Percent_Min] &&
            ratio <= kRatioSections[section_num][Percent_Max])
        {
            return section_num;
        }
    };
    return _countof(kRatioSections) - 1;
}

sSTRENGTH_PENALTY* StrengthPenaltyParser::GetStrengthPenaltyInfo(int index,
    int max_durability, int cur_durability) const
{
    const StrengthPenaltyRecord* record = FindStrengthPenalty(index);
    if (record == NULL) {
        return NULL;
    };
    int offset = CalcStrengthToIdx(max_durability, cur_durability);
    const sSTRENGTH_PENALTY* penalty_info = &record->sections_[offset];
    return const_cast<sSTRENGTH_PENALTY*>(penalty_info);
}

bool StrengthPenaltyParser::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    //
    const int max_rows = GetRowSize();
    if (FlowControl::FCAssert(max_rows != 0 && table_ == NULL) == false) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*table_) * max_rows;
        table_ = reinterpret_cast<StrengthPenaltyRecord*>(::malloc(buffer_size));
        ZeroMemory(table_, buffer_size);
        number_of_records_ = 0;
    };

    char column_buffer[MAX_FIELD_NAME_SIZE];
    for (int row = 0; row < max_rows; ++row, ++number_of_records_)
    {
        StrengthPenaltyRecord* const record = &table_[row];
        record->index_ = GetDataInt("Index", row);
        if (StrengthPenaltyRecord* check_record = FindStrengthPenalty(record->index_))
        {
            SUNLOG(eCRITICAL_LOG, "StrengthPenalty 인덱스(%d)가 중복됩니다", record->index_);
            FASSERT(!"데이터에 오류가 있습니다.");
        };
        for (int i = 0; i < _countof(record->sections_); ++i)
        {
            sSTRENGTH_PENALTY* const node = &record->sections_[i];
            //
            _snprintf(column_buffer, _countof(column_buffer), "StrengthPro%d\0", i + 1);
            const_cast<float&>(node->m_fStrengthPro) = GetDataFloat(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "StrengthUIIndex%d\0", i + 1);
            const_cast<int&>(node->m_nStrengthUIIndex) = GetDataInt(column_buffer, row);
            _snprintf(column_buffer, _countof(column_buffer), "StrengthColorCode%d\0", i + 1);
            const_cast<COLORREF&>(node->m_StrengthColorCode) =
                strtol(GetDataString(column_buffer, row), NULL, 16);
        }
        FASSERT(CheckValidated(record));
    }

    return true;
}

bool StrengthPenaltyParser::CheckValidated(StrengthPenaltyRecord* const record)
{
    if (record->index_ == 0)
    {
        // 잘못된 인덱스 값.
        ASSERT(false);
    }

    for (int i = 0; i < kMaxNumberOfStrengthPenaltySections; ++i)
    {
        //스크립트 값 체크.
    }

    return true;
}


#endif //__NA_001062_20080623_STRENGTH_PENALTY