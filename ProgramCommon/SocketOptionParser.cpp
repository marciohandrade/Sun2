#include "StdAfx.h"
#include ".\socketoptionparser.h"
#include <ItemInfoParser.h>
#include <boost/foreach.hpp>


#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : ZardOptionInfo
////////////////////////////////////////////////////////////////////////////////////////////////////

ZardOptionInfo::~ZardOptionInfo()
{
    FOREACH_CONTAINER(
        const OptionValueMap::value_type& option_value_pair, option_value_map_, OptionValueMap)
    {
        OptionValueInfo* option_value = option_value_pair.second;
        SAFE_DELETE(option_value);
    }
}

bool ZardOptionInfo::Insert(OptionValueInfo*& INOUT new_option_value)
{
    if (new_option_value == NULL)
    {
        return false;
    }
    if (new_option_value->point == 0)
    {
        SAFE_DELETE(new_option_value);
        return false;
    }
    if (IsExist(new_option_value->point) == true)
    {
        SAFE_DELETE(new_option_value);
        return false;
    }
    option_value_map_[new_option_value->point] = new_option_value;
    return true;
}

const ZardOptionInfo::OptionValueInfo* ZardOptionInfo::Find(const OptionPoint option_point) const
{
    if (option_point > 0)
    {
        // �ԷµȰ��� ���� �ʴ� ���� ���� ū ����Ʈ���� �����Ѵ�.
        // ���� ������ �߿��ϱ� ������ �ɼǰ� ���� �ؽø����� �ٲٸ� �ȵȴ�.
        BOOST_REVERSE_FOREACH(const OptionValueMap::value_type& option_value_pair, option_value_map_)
        {
            const OptionValueInfo* const option_value = option_value_pair.second;
            if (option_value != NULL && option_value->point <= option_point)
            {
                if (option_value->point < 0)
                {
                    // 0���� �������� ã�Ҵٸ�, �Է� ����Ʈ�� �ʹ� �۾Ƽ� �����ų �ɼǰ��� ã�� ���ߴٴ� �ǹ��̴�.
                    return NULL;
                }
                return option_value;
            }
        }
    }
    else if (option_point < 0)
    {
        FOREACH_CONTAINER(
            const OptionValueMap::value_type& option_value_pair, option_value_map_, OptionValueMap)
        {
            const OptionValueInfo* const option_value = option_value_pair.second;
            if (option_value != NULL && option_value->point >= option_point)
            {
                if (option_value->point > 0)
                {
                    // 0���� ū���� ã�Ҵٸ�, �Է� ����Ʈ�� �ʹ� Ŀ�� �����ų �ɼǰ��� ã�� ���ߴٴ� �ǹ��̴�.
                    return NULL;
                }
                return option_value;
            }
        }
    }
    return NULL;
}

eVALUE_TYPE ZardOptionInfo::GetOptionType() const
{
    OptionValueMap::const_iterator it = option_value_map_.begin();
    if (it != option_value_map_.end())
    {
        const OptionValueInfo* const option_value = it->second;
        if (option_value != NULL)
        {
            return option_value->type;
        }
    }
    return VALUE_TYPE_VALUE;
}

bool sSOCKET_OPTION::SetMainOption()
{
    enum { kMainOption  = 0 };
    const sSOCKET_OPTION::OptionPointInfo* const main_option_point = GetPointInfo(kMainOption);
    if (main_option_point == NULL)
    {
        return false;
    }
    const ZardOptionInfo* const main_option = main_option_point->zard_option;
    if (main_option == NULL)
    {
        return false;
    }

    item_option_kind = main_option->item_option_code;
    attr_option_kind = g_ITEM_OPTION[item_option_kind];

    const ZardOptionInfo::OptionValueInfo* const main_option_value = 
        main_option->Find(main_option_point->option_point);
    if (main_option_value != NULL)
    {
        option_type = main_option_value->type;
        option_value = main_option_value->value;
    }
    else
    {
        option_type = VALUE_TYPE_VALUE;
        option_value = 0;
    }
    return true;
}
#endif // _NA_0_20100727_SOCKET_SYSTEM_PARSER


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : SocketOptionParser
////////////////////////////////////////////////////////////////////////////////////////////////////

SocketOptionParser::SocketOptionParser()
{
#ifndef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    m_SocketOptionParamHash.Initialize(4*10);
#endif // _NA_000251_20100727_SOCKET_SYSTEM
}

SocketOptionParser::~SocketOptionParser()
{
#ifndef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    Unload();
#endif // _NA_000251_20100727_SOCKET_SYSTEM
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SocketOptionParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch(script_code)
    {
#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT
    case SCRIPT_ZARD_OPTION_INFO:
        return (LoadZardOption(!!is_reload) ? TRUE : FALSE);
    case SCRIPT_ZARD_POINT_INFO:
        return (LoadSocketOption(!!is_reload) ? TRUE : FALSE);
#else
    case SCRIPT_SOCKETOPTIONINFO:
        return (LoadSocketOption(!!is_reload) ? TRUE : FALSE);
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT
    }    
    return FALSE;
}

#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
void SocketOptionParser::Release()
{
    ReleaseSocketOption();
    ReleaseZardOption();
}

const sSOCKET_OPTION* const SocketOptionParser::FindSocketOption(
    const SOCKETINDEX socket_option_index) const
{
    SocketOptionMap::const_iterator socket_option_pos = socket_option_map_.find(socket_option_index);
    if (socket_option_pos != socket_option_map_.end())
    {
        return socket_option_pos->second;
    }
    return NULL;
}

SOCKETINDEX SocketOptionParser::GetRandomZardOptionByGrade(
    const ZardGrade zard_grade, const ZardOptionKind zard_item_option_kind) const
{
    if (zard_grade <= kZardGradeUnused || zard_grade >= kZardGradeMax)
    {
        return kInvalidSocketOptionIndex;
    }

    if (zard_item_option_kind <= eATTR_TYPE_INVALID || zard_item_option_kind >= _countof(g_ITEM_OPTION))
    {
        return kInvalidSocketOptionIndex;
    }

    ZardOptionRatio* const zard_option_ratio = FindZardOptionRatio(zard_grade, zard_item_option_kind);
    if (zard_option_ratio == NULL)
    {
        return kInvalidSocketOptionIndex;
    }

    const DWORD ratio = rand() % zard_option_ratio->GetTotalRatio();
    SOCKETINDEX zard_option_index = 0;
    if (!zard_option_ratio->Select(ratio, zard_option_index))
    {
        return kInvalidSocketOptionIndex;
    }
    
    return zard_option_index;
}

SOCKETINDEX SocketOptionParser::GetRandomZardOption(const CODETYPE item_code) const
{
    ZardItemRatioMap::const_iterator pos = zard_item_ratio_map_.find(item_code);
    if (pos == zard_item_ratio_map_.end())
    {
        return kInvalidSocketOptionIndex;
    }
    
    ZardOptionRatio* const item_ratio = pos->second;
    const DWORD ratio = rand() % item_ratio->GetTotalRatio();
    
    SOCKETINDEX option_index = kInvalidSocketOptionIndex;
    if (item_ratio->Select(ratio, option_index) == false)
    {
        return kInvalidSocketOptionIndex;
    }

    return option_index;
}

SOCKETINDEX SocketOptionParser::GetFixedZardOption(const CODETYPE item_code) const
{
    ZardItemRatioMap::const_iterator pos = zard_item_ratio_map_.find(item_code);
    if (pos == zard_item_ratio_map_.end())
    {
        return kInvalidSocketOptionIndex;
    }

    ZardOptionRatio* const item_ratio = pos->second;
    if (item_ratio->GetCount() != 1)
    {
        return kInvalidSocketOptionIndex;
    }
    
    const DWORD ratio = rand() % item_ratio->GetTotalRatio();
    SOCKETINDEX option_index = kInvalidSocketOptionIndex;
    if (item_ratio->Select(ratio, option_index) == false)
    {
        return kInvalidSocketOptionIndex;
    }

    return option_index;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
static SocketOptionParser::ZardOptionRatioKey MakeZardOptionRatioKey(
    SocketOptionParser::ZardGrade zard_grade, 
    SocketOptionParser::ZardOptionKind zard_item_option_kind)
{
    FASSERT(SAFE_NUMERIC_TYPECAST(SocketOptionParser::ZardGrade, zard_grade, BYTE));
    FASSERT(SAFE_NUMERIC_TYPECAST(SocketOptionParser::ZardOptionKind, zard_item_option_kind, BYTE));
#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
    return (SocketOptionParser::ZardOptionRatioKey)zard_grade;
#else
    return MAKEWORD(zard_grade, zard_item_option_kind);
#endif //_NA_007973_20150120_ADD_MULTIOPTION_ZARD
}
} // internal
} // util

bool SocketOptionParser::LoadSocketOption(const bool is_reload)
{
    if (is_reload == true)
    {
        ReleaseSocketOption();
    }

    bool is_valid = true;
    const ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        // ��� �ɼ� �ε����� �д´�.
        enum { kMaxSocketIndex = 1023 };
        const SOCKETINDEX option_index = GetDataWORD(_T("Index"), row);
        {
            if (option_index > kMaxSocketIndex)
            {
                SUNLOG(eCRITICAL_LOG, 
                    _T("|%s|��� �ɼ� �ε����� �ִ밪�� �ʰ� �߽��ϴ�.|row = %u, �ִ밪 = %u, ���簪 = %u|"), 
                    __FUNCTION__, 
                    row, 
                    kMaxSocketIndex, 
                    option_index);
                is_valid = false;
                continue;
            }
            if (socket_option_map_.find(option_index) != socket_option_map_.end())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�̹� ���� ��� �ɼ� �ε����� �����մϴ�.|row = &u, OptionIndex = %u|"), 
                       __FUNCTION__, 
                       row, 
                       option_index);
                is_valid = false;
                continue;
            }
        }
        
        // ��� �ɼ� �߻� Ȯ���� �д´�.
        enum { kMaxRandomRatio = 1000 };
        const int random_ratio = GetDataInt(_T("RandomRatio"), row);
        if (random_ratio <= 0 || random_ratio > kMaxRandomRatio)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼ� �߻� Ȯ���� �ǹٸ��� �ʽ��ϴ�.|row = %u, RandomRatio = %u|"), 
                   __FUNCTION__, 
                   row, 
                   random_ratio);
            is_valid = false;
            continue;
        }

        // ��� ������ �ڵ带 �д´�.
        const CODETYPE zard_item_code = GetDataDWORD(_T("ItemCode"), row);
        const BASE_ITEMINFO* const zard_item = item_parser->GetItemInfo(zard_item_code);
        if (zard_item == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� ������ ������ ã�� �� �����ϴ�.|row = %u, ItemCode = %u|"), 
                   __FUNCTION__, 
                   row, 
                   zard_item_code);
            is_valid = false;
            continue;
        }

        // ��� ����� �˻��Ѵ�.
        if ((zard_item->zard_grade == kZardGradeUnused) && (zard_item->zard_grade >= kZardGradeMax))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ �ʴ� ��� ����Դϴ�.|row = %u, ZardGrade = %u, OptionIndex = %d|"), 
                   __FUNCTION__, 
                   row, 
                   zard_item->zard_grade, 
                   option_index);
            is_valid = false;
            continue;
        }

#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT
#else // _NA_001699_20110104_SOCKET_OPTION_POINT
        // ��� �ɼ��� �˻��Ѵ�.
        const int option_value = GetDataInt(_T("OptionValue"), row);
        if (CheckOptionValidity(
            zard_item->zard_item_option_kind, VALUE_TYPE_VALUE, option_value) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼ��� ��ȿ���� �ʽ��ϴ�.|row = &u, ItemOptionKind = %u, OptionType = %d, OptionValue = %d, OptionIndex = %u|"), 
                   __FUNCTION__, 
                   row, 
                   zard_item->zard_item_option_kind, 
                   VALUE_TYPE_VALUE, 
                   option_value, 
                   option_index);
            is_valid = false;
            continue;
        }
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT

        sSOCKET_OPTION* new_socket_option(new sSOCKET_OPTION);
        {
            new_socket_option->index = option_index;
            new_socket_option->item_code = zard_item_code;
            new_socket_option->zard_grade = static_cast<eSOCKET_LEVEL>(zard_item->zard_grade);
            new_socket_option->option_image = zard_item->m_wVICode;
#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT
            if (LoadZardPoint(row, new_socket_option) == false)
            {
                SAFE_DELETE(new_socket_option);
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��� ����Ʈ ������ �дµ� �����߽��ϴ� .|row = %u, OptionIndex = %u|"), 
                       __FUNCTION__, 
                       row, 
                       option_index);
                is_valid = false;
                continue;
            }
            if (new_socket_option->SetMainOption() == false)
            {
                SAFE_DELETE(new_socket_option);
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��� �� �ɼǰ��� �����ϴµ� �����߽��ϴ�.|row = %u, OptionIndex = %u|"), 
                       __FUNCTION__, 
                       row, 
                       option_index);
                is_valid = false;
                continue;
            }
            new_socket_option->name_code = zard_item->m_NCode;
#else // _NA_001699_20110104_SOCKET_OPTION_POINT
            new_socket_option->name_code = GetDataDWORD(_T("NameCode"), row);
            new_socket_option->item_option_kind = zard_item->zard_item_option_kind;
            new_socket_option->option_type = VALUE_TYPE_VALUE;
            new_socket_option->option_value = option_value;
            new_socket_option->attr_option_kind = g_ITEM_OPTION[new_socket_option->item_option_kind];
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT            
        }
        socket_option_map_[new_socket_option->index] = new_socket_option;

        // ��� �ɼ� �߻� Ȯ���� �����Ѵ�.
        ZardOptionRatio* const zard_option_ratio = 
            FindZardOptionRatio(new_socket_option->zard_grade, new_socket_option->item_option_kind);
        if (zard_option_ratio == NULL)
        {
            ZardOptionRatio* new_option_ratio(new ZardOptionRatio);
            new_option_ratio->Add(new_socket_option->index, random_ratio);
            
            const ZardOptionRatioKey new_option_ratio_key = 
                util::internal::MakeZardOptionRatioKey(
                    new_socket_option->zard_grade, new_socket_option->item_option_kind);
            zard_option_ratio_map_[new_option_ratio_key] = new_option_ratio;
        }
        else
        {
            zard_option_ratio->Add(new_socket_option->index, random_ratio);
        }
        
        // ��� �������� �������� �� �ɼ� �߻� Ȯ���� �����Ѵ�.
        ZardItemRatioMap::const_iterator pos = zard_item_ratio_map_.find(new_socket_option->item_code);
        if (pos == zard_item_ratio_map_.end())
        {
            ZardOptionRatio* const new_item_ratio(new ZardOptionRatio);
            new_item_ratio->Add(new_socket_option->index, random_ratio);
            zard_item_ratio_map_[new_socket_option->item_code] = new_item_ratio;
        }
        else
        {
            ZardOptionRatio* const item_ratio = pos->second;
            item_ratio->Add(new_socket_option->index, random_ratio);                
        }
    }

    ASSERT(is_valid && _T("��ũ��Ʈ �����Ϳ� ������ �ֽ��ϴ�."));
    //return is_valid; ��ũ��Ʈ �����Ͱ� Ʋ�� ��찡 �� �־ �����ϰ� �ϴ� ���� ��Ų��.
    // �̷��� ó���ϴ��� �߸��� �� �־�ΰ� ���� �� ���׷� �ǵ��� ���� ���� ������� �ʴ´�.
    //return true;
    return is_valid;
}

void SocketOptionParser::ReleaseSocketOption()
{
    FOREACH_CONTAINER(
        const ZardItemRatioMap::value_type& item_ratio_pair, 
        zard_item_ratio_map_, 
        ZardItemRatioMap)
    {
        ZardOptionRatio* item_ratio = item_ratio_pair.second;
        SAFE_DELETE(item_ratio);
    }
    zard_item_ratio_map_.clear();

    FOREACH_CONTAINER(
        const ZardOptionRatioMap::value_type& option_ratio_pair, 
        zard_option_ratio_map_, 
        ZardOptionRatioMap)
    {
        ZardOptionRatio* option_ratio = option_ratio_pair.second;
        SAFE_DELETE(option_ratio);
    }
    zard_option_ratio_map_.clear();

    FOREACH_CONTAINER(
        const SocketOptionMap::value_type& socket_option_pair, socket_option_map_, SocketOptionMap)
    {
        sSOCKET_OPTION* socket_option = socket_option_pair.second;
        SAFE_DELETE(socket_option);
    }
    socket_option_map_.clear();
}

SocketOptionParser::ZardOptionRatio* SocketOptionParser::FindZardOptionRatio(
    const ZardGrade zard_grade, const ZardOptionKind zard_item_option_kind) const
{
    const ZardOptionRatioKey key = 
        util::internal::MakeZardOptionRatioKey(zard_grade, zard_item_option_kind);
    ZardOptionRatioMap::const_iterator pos = zard_option_ratio_map_.find(key);
    if (pos != zard_option_ratio_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

bool SocketOptionParser::LoadZardOption(const bool is_reload)
{
    if (is_reload == true)
    {
        ReleaseZardOption();
    }

    bool is_valid_script = true;

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; row++)
    {
        // �ɼ� ����Ʈ ���� �д´�.
        enum { kMinOptionPoint = -100, kMaxOptionPoint = 100 };
        const ZardOptionInfo::OptionPoint option_point = GetDataInt(_T("PointValue"), row);
        if (option_point < kMinOptionPoint || option_point > kMaxOptionPoint)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼ� ����Ʈ���� ������ �ǹٸ��� �ʽ��ϴ�.|row = %u, �ּҰ� = %d, �ִ밪 = %d, ���簪 = %d|"), 
                   __FUNCTION__, 
                   row, 
                   kMinOptionPoint, 
                   kMaxOptionPoint, 
                   option_point);
            is_valid_script = false;
            continue;
        }

        // ��� �ɼ��� �д´�.
        const ZardOptionInfo::ItemOptionCode item_option_code = GetDataBYTE(_T("OptionCode"), row);
        const BYTE option_type = GetDataBYTE(_T("OptionType"), row);
        const ZardOptionInfo::OptionValueInfo::ValueType option_value = GetDataInt(_T("OptionValue"), row);
        if (CheckOptionValidity(item_option_code, option_type, option_value) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼ��� ��ȿ���� �ʽ��ϴ�.|row = %u, ItemOptionCode = %u, OptionType = %u, OptionValue = %d|"), 
                   __FUNCTION__, 
                   row, 
                   item_option_code, 
                   option_type, 
                   option_value);
            is_valid_script = false;
            continue;
        }

        // ��� �ɼ����� ��� ������ �ɼ����� �˻��Ѵ�.
        // ���� ���� ���� �ɼ��� ����� �����Ѵ�.
        const eATTR_TYPE attr_option_code = g_ITEM_OPTION[item_option_code];
        switch (attr_option_code)
        {
        case eATTR_STR:
        case eATTR_DEX:
        case eATTR_VIT:
        case eATTR_INT:
        case eATTR_SPR:
        case eATTR_INCREASE_STAT_POINT:
        case eATTR_DECREASE_LIMIT_STAT:
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼ����� ����� �� ���� �ɼ��Դϴ�.|row = %u, ItemOptionCode = %u|"), 
                   __FUNCTION__, 
                   row, 
                   item_option_code);
            is_valid_script = false;
            continue;
        }

        // ��� �ɼ��� ��´�.
        ZardOptionInfo* zard_option = FindZardOption(item_option_code);
        if (zard_option == NULL)
        {
            ZardOptionInfo* new_zard_option(new ZardOptionInfo);
            new_zard_option->item_option_code = item_option_code;
            new_zard_option->attr_option_code = attr_option_code;
            zard_option_map_[item_option_code] = new_zard_option;
            zard_option = FindZardOption(item_option_code);
            ASSERT(zard_option != NULL);
        }

        // �����Ϸ��� �ɼǰ��� �̹� �����ϴ��� �˻��Ѵ�.
        if (zard_option->IsExist(option_point) == true)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�����Ϸ��� ��� �ɼǰ��� �̹� �����մϴ�.|row = %u, OptionPoint = %d|"), 
                   __FUNCTION__, 
                   row, 
                   option_point);
            is_valid_script = false;
            continue;
        }

        // �ɼǰ��� �����Ѵ�.
        ZardOptionInfo::OptionValueInfo* new_option_value(new ZardOptionInfo::OptionValueInfo);
        {
            new_option_value->point = option_point;
            new_option_value->type = static_cast<eVALUE_TYPE>(option_type);
            new_option_value->value = option_value;
        }
        if (zard_option->Insert(new_option_value) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��� �ɼǰ��� �����ϴµ� ���� �߽��ϴ�.|row = %u, OptionPoint = %d|"), 
                   __FUNCTION__, 
                   row, 
                   option_point);
            is_valid_script = false;
            continue;
        }
    }

    FASSERT(is_valid_script && _T("��ũ��Ʈ �����Ϳ� ������ �ֽ��ϴ�."));
    return is_valid_script;
}

void SocketOptionParser::ReleaseZardOption()
{
    FOREACH_CONTAINER(
        const ZardOptionMap::value_type& zard_option_pair, zard_option_map_, ZardOptionMap)
    {
        ZardOptionInfo* zard_option = zard_option_pair.second;
        SAFE_DELETE(zard_option);
    }
    zard_option_map_.clear();
}

ZardOptionInfo* SocketOptionParser::FindZardOption( 
    const ZardOptionInfo::ItemOptionCode item_option_code) const
{
    const ZardOptionMap::const_iterator it = zard_option_map_.find(item_option_code);
    if (it != zard_option_map_.end())
    {
        return it->second;
    }
    return NULL;
}

bool SocketOptionParser::LoadZardPoint(const int row, sSOCKET_OPTION* OUT socket_option)
{
    enum { kMaxFieldNameSize = 32 }; 
    TCHAR temp_field_name[kMaxFieldNameSize];

    for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
        i < sSOCKET_OPTION::OptionPointInfo::kMaxListSize; 
        ++i)
    {
        // ��谪�� �˻��Ѵ�.
        sSOCKET_OPTION::OptionPointInfo* const option_point_info = 
            socket_option->GetPointInfoByMutable(socket_option->option_point_list_size);
        if (option_point_info == NULL)
        {
            break;
        }

        // ��� �ɼ� �ڵ带 �д´�.
        _sntprintf(temp_field_name, kMaxFieldNameSize, _T("OptionCode%u"), i+1);
        const ZardOptionInfo::ItemOptionCode item_option_code = GetDataBYTE(temp_field_name, row);
        if (item_option_code == eATTR_TYPE_INVALID)
        {
            continue;
        }
        const ZardOptionInfo* const zard_option = FindZardOption(item_option_code);
        if (zard_option == NULL)
        {
            continue;
        }

        // ��� �ɼ� ����Ʈ�� �д´�.
        _sntprintf(temp_field_name, kMaxFieldNameSize, _T("OptionPoint%u"), i+1);
        const ZardOptionInfo::OptionPoint option_point = GetDataInt(temp_field_name, row);

        // ����Ʈ ������ �����Ѵ�.
        {
            option_point_info->item_option_code = item_option_code;
            option_point_info->option_point = option_point;
            option_point_info->zard_option = zard_option;
        }
        ++(socket_option->option_point_list_size);
    }
    if (socket_option->option_point_list_size < 1)
    {
        return false;
    }
    return true;
}
#else // _NA_0_20100727_SOCKET_SYSTEM_PARSER
VOID SocketOptionParser::Unload()
{
    sSOCKETITEM_OPTION * pInfo = NULL ;
    m_SocketOptionParamHash.SetFirst();
    while( pInfo = m_SocketOptionParamHash.GetNext() )
    {
        SAFE_DELETE( pInfo );
    }
    m_SocketOptionParamHash.RemoveAll();

    memset( m_SocketOption, 0, sizeof(sSOCKET_OPTION)*MAX_ITEM_OPTION_KIND_NUM );
}

WORD	SocketOptionParser::ValidCheck(sSOCKETITEM_OPTION *pInfo)
{
    // ���⼭ ������ ���� __NA_0_SOCKET_OPTION_BUG_RECOVERY �۾� ����
    // ���� �ɼ� ���� ����� ���� �մϴ�.
    if( pInfo->m_AttrIndex >= 64 )
        return 1;

    // ����Ÿ�� �ɼ� üũ
    if( !CheckOptionValidity( pInfo->m_AttrIndex, pInfo->m_NumericType, pInfo->m_byValue ) )
    {
        SUNLOG( eCRITICAL_LOG, "[SocketOptionParser] Index(%d) Invalid m_AttrIndex(%d) m_NumericType(%d) m_byValue(%d)! \n", 
            pInfo->m_SocketItemCode, pInfo->m_AttrIndex, pInfo->m_NumericType, pInfo->m_byValue );
    }

    return 0;
}

BOOL SocketOptionParser::LoadSocketOption(BOOL bReload)
{
    memset( m_SocketOption, 0, sizeof(sSOCKET_OPTION)*MAX_ITEM_OPTION_KIND_NUM );

    int nRowSize = GetRowSize();
    for( int nRow = 0; nRow < nRowSize; ++nRow )
    {
        CODETYPE Code = GetDataDWORD( "ItemCode", nRow);
        sSOCKETITEM_OPTION * pInfo = m_SocketOptionParamHash.GetData(Code);
        if(!pInfo)
        {
            pInfo = new sSOCKETITEM_OPTION;
            m_SocketOptionParamHash.Add( pInfo, Code );
        }
        else
        {
            if( FALSE == bReload )
            {
                SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, Code = %d",
                    GetDataString( "Name", nRow ), Code );
                FASSERT( !"�����Ϳ� ������ �ֽ��ϴ�." );
            }
        }
        
        // sSOCKETITEM_OPTION�� ���� ä��
        pInfo->m_SocketItemCode = Code;
        pInfo->m_AttrIndex = GetDataInt( "Optcode", nRow); //(�Ӽ��ڵ�)
        strncpy(pInfo->m_pszAttrName, GetDataString( "Name", nRow ), MAX_ATTRIBUTENAME_LENGTH);
        pInfo->m_NCode		= GetDataDWORD( "Ncode", nRow);
        pInfo->m_SocketLevel = (eSOCKET_LEVEL)(int)GetDataNumber( "Socket_grade", nRow);
        pInfo->m_byValue = GetDataBYTE( "Value", nRow);
        pInfo->m_NumericType = (eVALUE_TYPE)(int)GetDataNumber( "Type", nRow);

        FASSERT( pInfo->m_SocketLevel == SOCKETLV_LOW || pInfo->m_SocketLevel == SOCKETLV_HIGH );
        if( ValidCheck(pInfo) > 0 )
        {
            SUNLOG( eCRITICAL_LOG, "�ɼ� �ε��� �ߺ�!! ���� �ɼ� ���� ��ƾ�� �����ؾ� �մϴ�." );
            ASSERT(FALSE);
            return FALSE;
        }

        // ���� : m_AttrIndex(Optcode)�� �ߺ��ȴ�. m_SocketOption���� �ҷ��°� �ֱ�?
        // ���� : MAX_ATTRIBUTENAME_LENGTH(16) ���� ��� ū���̴�. 

        // sSOCKET_OPTION�� ������ ä��
        m_SocketOption[pInfo->m_AttrIndex].m_AttrIndex = pInfo->m_AttrIndex;
        m_SocketOption[pInfo->m_AttrIndex].m_SocketItemCode = pInfo->m_SocketItemCode;
        strncpy( m_SocketOption[pInfo->m_AttrIndex].m_pszAttrName, pInfo->m_pszAttrName, MAX_ATTRIBUTENAME_LENGTH );
        m_SocketOption[pInfo->m_AttrIndex].m_NCode = pInfo->m_NCode;
        m_SocketOption[pInfo->m_AttrIndex].m_NumericType = pInfo->m_NumericType;
        m_SocketOption[pInfo->m_AttrIndex].m_iValue[pInfo->m_SocketLevel] = pInfo->m_byValue;

    //	SUNLOG(eCRITICAL_LOG, "%u\t%u\t%s\t%u\t%u\t%u\t%u\n", 
    //		pInfo->m_SocketItemCode,
    //		pInfo->m_AttrIndex,
    //		pInfo->m_pszAttrName,
    //		pInfo->m_NCode,
    //		pInfo->m_SocketLevel,
    //		pInfo->m_NumericType,
    //		pInfo->m_byValue);
    }

    return TRUE;
}
#endif // _NA_0_20100727_SOCKET_SYSTEM_PARSER