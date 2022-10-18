////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   SCRandomInfo.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/12/13 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "./SCRandomInfo.h" // header
#include "RandomItemParser.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

SCRandomInfo::SCRandomInfo() : item_info_(NULL)
{
}

SCRandomInfo::~SCRandomInfo()
{
    Clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

void SCRandomInfo::Clear()
{
    option_info_.Clear();
    if (item_info_ != NULL)
    {
        RandomItemParser::Instance()->DestroyItemInfo(item_info_);
        item_info_ = NULL;
    }
}

void SCRandomInfo::Copy(const SCRandomInfo& src)
{
    option_info_.Copy(src.option_info_);
    if (src.item_info_ != NULL)
    {
        if (item_info_ == NULL)
        {
            item_info_ = RandomItemParser::Instance()->ConstructItemInfo();
            if (item_info_ == NULL)
            {
                return;
            }
        }
        ::ZeroMemory(item_info_, sizeof(*item_info_));
        ::CopyMemory(item_info_, src.item_info_, sizeof(*item_info_));
    }
    else
    {
        if (item_info_ != NULL)
        {
            RandomItemParser::Instance()->DestroyItemInfo(item_info_);
            item_info_ = NULL;
        }
    }
}

bool SCRandomInfo::EnableRandomize(const BASE_ITEMINFO& org_item_info) const
{
    const RandomItemParser* const parser = RandomItemParser::Instance();
    return parser->EnableRandomize(org_item_info);
}

bool SCRandomInfo::Randomize( 
    const RandomValueTypeGrade::Value value_type_grade, const BASE_ITEMINFO& org_item_info)
{
    const RandomItemParser* const parser = RandomItemParser::Instance();
    const RandomItemParser::OptionParam& option_param = 
        parser->RandomizeOption(value_type_grade, org_item_info);
    if (option_param.is_valid == false)
    {
        return false;
    }
    if (option_param.size > _countof(option_info_.keys))
    {
        return false;
    }
    option_info_.Clear();
    for (uint8_t i = 0; i < option_param.size; ++i)
    {
        option_info_.keys[i] = option_param.keys[i];
        ++option_info_.size;
    }
    return true;
}

bool SCRandomInfo::CreateItemInfo(const BASE_ITEMINFO& org_item_info)
{
    if (option_info_.size < 1)
    {
        return false;
    }
    RandomItemParser* const parser = RandomItemParser::Instance();
    if (parser->EnableRandomize(org_item_info) == false)
    {
        return false;
    }
    RandomItemParser::OptionParam option_param;
    if (option_info_.size > _countof(option_param.keys))
    {
        return false;
    }
    for (uint8_t i = 0; i < option_info_.size; ++i)
    {
        option_param.keys[i] = option_info_.keys[i];
        ++option_param.size;
    }
    option_param.is_valid = true;
    
    if (item_info_ == NULL)
    {
        item_info_ = parser->ConstructItemInfo();
        if (item_info_ == NULL)
        {
            return false;
        }
    }    
    ::ZeroMemory(item_info_, sizeof(*item_info_));
    ::CopyMemory(item_info_, &org_item_info, sizeof(*item_info_));

    if (parser->CreateOption(option_param, *item_info_) == false)
    {
        return false;
    }
    BOOST_STATIC_ASSERT(_countof(option_info_.grades) == _countof(option_param.grades));
    for (uint8_t i = 0; i < _countof(option_info_.grades); ++i)
    {
        option_info_.grades[i] = option_param.grades[i];
    }
    return true;
}

RandomValueGrade::Value SCRandomInfo::GetRandomOptionGrade(const uint8_t index) const
{
    if (index >= _countof(option_info_.grades))
    {
        return RandomValueGrade::kNone;
    }
    return (option_info_.grades)[index];
}