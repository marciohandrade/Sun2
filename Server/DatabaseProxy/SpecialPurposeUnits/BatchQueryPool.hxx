#ifndef __DBPROXY_SPECIALPURPOSEUNITS_BATCHQUERYPOOL_H
#define __DBPROXY_SPECIALPURPOSEUNITS_BATCHQUERYPOOL_H

#pragma once

namespace batch {
;
class IBatchQueryPool
{
public:
    virtual void Init() = 0;
};

}; //end of namespace

//==================================================================================================

#include "ACAchievementUpdateQuery.h"

namespace batch {
;

class ACAchievementUpdateBatchQueryPool : public IBatchQueryPool
{
public:
    //util::SafeMemoryPoolFactory<Query_GetACStreamInfo>  get_ac_stream_info_pool_;
    //util::SafeMemoryPoolFactory<Query_SetACStreamInfo>  set_ac_stream_info_pool_;

    void Init()
    {
        //get_ac_stream_info_pool_.Initialize(6);
        //set_ac_stream_info_pool_.Initialize(MAX_GET_ACSTREAM_INFO_ROWS_PER_PAGE << 2);
        //Query_GetACStreamInfo::SetPool(&get_ac_stream_info_pool_);
        //Query_SetACStreamInfo::SetPool(&set_ac_stream_info_pool_);
    };
};

}; //end of namespace

//==================================================================================================

#include "ACConvertQuery.h"

namespace batch {

class ACConvertBatchQueryPool : public IBatchQueryPool
{
public:
    //util::SafeMemoryPoolFactory<Query_Select_Character_ACInfo>  select_character_acinfo_pool_;
    //util::SafeMemoryPoolFactory<Query_Update_Character_ACInfo>  update_character_acinfo_pool_;

    void Init()
    {
        //select_character_acinfo_pool_.Initialize(3);
        //update_character_acinfo_pool_.Initialize(6000);
        //Query_Select_Character_ACInfo::SetPool(&select_character_acinfo_pool_);
        //Query_Update_Character_ACInfo::SetPool(&update_character_acinfo_pool_);
    };
};

}; //end of namespace

//==================================================================================================

#include "RankConvertQuery.h"

namespace batch {

class RankReporterQueryPool : public IBatchQueryPool
{
public:
    //util::SafeMemoryPoolFactory<Query_GetItemList>  select_query_pool_;
    //util::SafeMemoryPoolFactory<Query_SetRankReport>  update_query_pool_;

    void Init()
    {
//        select_query_pool_.Initialize(100);
//#if !defined(_JAPAN)
//        update_query_pool_.Initialize(500);
//#else
//        update_query_pool_.Initialize(7000);
//#endif
//        Query_GetItemList::SetPool(&select_query_pool_);
//        Query_SetRankReport::SetPool(&update_query_pool_);
    };
};

}; //end of namespace



#endif //__DBPROXY_SPECIALPURPOSEUNITS_BATCHQUERYPOOL_H
