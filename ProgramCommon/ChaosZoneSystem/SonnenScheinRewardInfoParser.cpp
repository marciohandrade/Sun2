#include "stdafx.h"
#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "SonnenScheinRewardInfoParser.h"

BOOL SonnenScheinRewardInfoParser::_Load( BOOL is_reload )
{
    if (is_reload == TRUE) {
        GetSScheinRewardInfoHash().clear();
    }

    int row_size = GetRowSize();
    for (int row= 0; row < row_size; ++row)
    {
        SScheinRewardInfo* reward_info = new SScheinRewardInfo();

        reward_info->index = GetDataBYTE("Index", row);
        reward_info->target_type = GetDataBYTE("TargetType", row);
        // 채집물(각인물)
        CHAR temp[8] = {0,};
        strncpy(temp, GetDataString("CollectID", row), sizeof(temp));
        reward_info->collect_id = StrToWzID(temp);
        
        // 타겟 오브젝트
        for (int target_count = 0; target_count < SScheinRewardInfo::TARGET_SIZE; ++target_count)
        {
            GetData(row, reward_info->target_array[target_count], "TargetID%d", target_count+1);
        }
        // 아이템 보상
        for (int item_count = 0; item_count < SScheinRewardInfo::REWARD_ITEM_SIZE; ++item_count)
        {
            GetData(row, reward_info->reward_item_array[item_count], "RewardItem%d", item_count+1);
        }
        // 버프 보상
        for (int buff_count = 0; buff_count < SScheinRewardInfo::REWARD_BUFF_SIZE; ++buff_count)
        {
            AttrOptionInfo& option_info = reward_info->reward_buff_option[buff_count];
            GetData(row, option_info.m_wOptionKind, "RewardOptionKind%d", buff_count+1);
            GetData(row, option_info.m_byOptionType, "RewardOptionType%d", buff_count+1);
            GetData(row, option_info.m_iOptionValue, "RewardOptionValue%d", buff_count+1);
        }

        //하임보상
        GetData(row, reward_info->reward_drop_heim_rate, "RewardDropHeimRate");

        GetSScheinRewardInfoHash().insert(SScheinRewardHash::value_type(reward_info->index, reward_info));
    }

    return TRUE;
}

void SonnenScheinRewardInfoParser::_Unload()
{
    SScheinRewardHash::iterator itr = sschein_rewardinfo_hash_.begin();
    for (itr; itr != sschein_rewardinfo_hash_.end(); ++itr)
    {
        SScheinRewardInfo* info = (itr->second);
        SAFE_DELETE(info);
    }
    sschein_rewardinfo_hash_.clear();
}

VOID SonnenScheinRewardInfoParser::Init( char* file_name )
{
    SetPackFileName(file_name);
}

VOID SonnenScheinRewardInfoParser::Release()
{
    _Unload();
}

BOOL SonnenScheinRewardInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    switch (script_code)
    {
    case SCRIPT_SONNENSCHEIN_REWARDINFO:
        return _Load(is_reload);
    default:
        break;
    }

    return FALSE;
}

const SScheinRewardInfo* SonnenScheinRewardInfoParser::FindData( const BYTE reward_phase ) const
{
    SScheinRewardHash::const_iterator itr = sschein_rewardinfo_hash_.find(reward_phase);

    if (itr != sschein_rewardinfo_hash_.end())
    {
        return itr->second;
    }
    
    return NULL;
}


#endif //_NA_008334_20150608_SONNENSCHEIN
