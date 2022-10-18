#include "StdAfx.h"
#include ".\SocketItemRatioParser.h"

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
SocketItemRatioParser::SocketItemRatioParser()
{
}

SocketItemRatioParser::~SocketItemRatioParser()
{    
}

void SocketItemRatioParser::Release()
{
    FOREACH_CONTAINER(
        const SocketRatioMap::value_type& socket_ratio_pair, 
        socket_ratio_map_, 
        SocketRatioMap)
    {
        SocketItemRatioInfo* socket_ratio = socket_ratio_pair.second;
        SAFE_DELETE(socket_ratio);
    }
    socket_ratio_map_.clear();
}

BOOL SocketItemRatioParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_SOCKET_ITEM_RATIO_INFO:
        return (LoadSocketItemRatio(!!is_reload) ? TRUE : FALSE);
    }
    return FALSE;	
}

SocketItemRatioInfo::RatioType::Value SocketItemRatioParser::GetRatioType(
    const SocketItemRatioInfo::ItemType item_type, const bool is_recreate) const
{
    switch (item_type)
    {
    case eITEMTYPE_TWOHANDAXE:  
    case eITEMTYPE_TWOHANDSWORD:    
    case eITEMTYPE_ONEHANDSWORD:
    case eITEMTYPE_SPEAR:
    case eITEMTYPE_WHIP:        
    case eITEMTYPE_DAGGER:  
    case eITEMTYPE_ONEHANDCROSSBOW:
    case eITEMTYPE_ETHERWEAPON:
    case eITEMTYPE_STAFF:
    case eITEMTYPE_ORB:
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (is_recreate)
        {
            return SocketItemRatioInfo::RatioType::kRecreateWeapon;
        }
        return SocketItemRatioInfo::RatioType::kWeapon;
    case eITEMTYPE_ARMOR:
    case eITEMTYPE_PANTS:
    case eITEMTYPE_BOOTS:
    case eITEMTYPE_GLOVE:
    case eITEMTYPE_HELMET:
    case eITEMTYPE_PROTECTOR:
    case eITEMTYPE_BELT:
    case eITEMTYPE_SHIRTS:
        if (is_recreate)
        {
            return SocketItemRatioInfo::RatioType::kRecreateArmor;
        }
        return SocketItemRatioInfo::RatioType::kArmor;
    }
    return SocketItemRatioInfo::RatioType::kInvalid;
}

const SocketItemRatioInfo* SocketItemRatioParser::FindSocketRatio(
    const SocketItemRatioInfo::RatioType::Value ratio_type, 
    const SocketItemRatioInfo::ItemLevel item_level) const
{
    if (ratio_type == SocketItemRatioInfo::RatioType::kInvalid)
    {
        return NULL;
    }
    const SocketRatioKey socket_ratio_key = MAKEWORD(ratio_type, item_level);
    const SocketRatioMap::const_iterator iter = socket_ratio_map_.find(socket_ratio_key);
    if (iter != socket_ratio_map_.end())
    {
        return iter->second;
    }
    return NULL;
}

bool SocketItemRatioParser::IsValid(const SocketItemRatioInfo& socket_ratio) const
{
    if (FindSocketRatio(socket_ratio.ratio_type_, socket_ratio.item_level_))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|같은 확률 타입과 레벨값이 이미 존재 합니다.|RatioType = %d, ItemLevel = %d|"), 
               __FUNCTION__, 
               socket_ratio.ratio_type_, 
               socket_ratio.item_level_);
        return false;
    }

    if (socket_ratio.ratio_type_ < 0 || 
        socket_ratio.ratio_type_ > SocketItemRatioInfo::RatioType::kMax)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|옳바르지 않은 확률 타입 입니다.|RatioType = %d|"), 
            __FUNCTION__, 
            socket_ratio.ratio_type_);
        return false;
    }

    if (socket_ratio.socket_creation_ratio_ < 1 || 
        socket_ratio.socket_creation_ratio_ > SocketItemRatioInfo::kMaxRatio)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|소켓 아이템이 될 확률이 허용할 수 없는 값입니다.|RatioType = %d, ItemLevel = %d, SocketCreation = %d, 최대값 = %d|"), 
               __FUNCTION__, 
               socket_ratio.ratio_type_, 
               socket_ratio.item_level_, 
               socket_ratio.socket_creation_ratio_, 
               SocketItemRatioInfo::kMaxRatio);
        return false;
    }

    SocketItemRatioInfo::Ratio total_socket_number_ratio = 0;
    for(int i = 0; i < SOCKET_MAX; ++i)
    {
        total_socket_number_ratio += socket_ratio.socket_ratio_[i];
    }

    if (total_socket_number_ratio > SocketItemRatioInfo::kMaxRatio)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|소켓 확률의 합이 %d가 아닙니다.|RatioType = %d, ItemLevel = %d|"), 
               __FUNCTION__, 
               SocketItemRatioInfo::kMaxRatio, 
               socket_ratio.ratio_type_, 
               socket_ratio.item_level_);
        return false;
    }

    return true;
}

bool SocketItemRatioParser::LoadSocketItemRatio(const bool is_reload)
{
    __UNUSED(is_reload);

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        SocketItemRatioInfo* socket_ratio(new SocketItemRatioInfo);
        {
            GetData(row, socket_ratio->ratio_type_, _T("ItemType"));
            GetData(row, socket_ratio->item_level_, _T("ItemLevel"));
            GetData(row, socket_ratio->socket_creation_ratio_, _T("SocketCreation"));

            for (int i = 0; i < SOCKET_MAX; ++i)
            {
                GetData(row, socket_ratio->socket_ratio_[i], _T("Socket%d"), i+1);
            }
        }
        if (IsValid(*socket_ratio) == false)
        {
            SAFE_DELETE(socket_ratio);
            continue;
        }
        const SocketRatioKey socket_ratio_key = MAKEWORD(socket_ratio->ratio_type_, socket_ratio->item_level_);
        socket_ratio_map_.insert(std::make_pair(socket_ratio_key, socket_ratio));
    }
    return true;
}
#endif //_NA_000251_20100727_SOCKET_SYSTEM