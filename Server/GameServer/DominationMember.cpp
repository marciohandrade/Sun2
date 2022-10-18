#pragma once
#include "stdafx.h"
#include "DominationMember.h"
#include "PacketStruct_GZ.h"
//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN

DominationMember::DominationMember(MAPCODE map_code)
:map_code_(0)
,is_load_data_(false)
{
    map_code_ = map_code;
}

DominationMember::~DominationMember()
{

}

BYTE
DominationMember::GetMemberList(DominationMemberInfo* OUT member_list, int max_count)const
{
    if (max_count > MAX_DOMINATION_MEMBER*2)
    {
        assert(!"Request count of member is over");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Request count of member is over : %d", max_count);
        return 0;
    }
    
    ConstMemberMapIterator begin_iterator = member_map_.begin(),
                           end_iterator = member_map_.end();

    BYTE data_index = 0;
    for(; data_index < max_count && begin_iterator != end_iterator; ++begin_iterator, ++data_index)
    {
        const DominationMemberInfo& member_info = begin_iterator->second;
        member_list[data_index] = (member_info);
    }

    return data_index;
}

const DominationMemberInfo* 
DominationMember::FindMember(CHARGUID char_guid)const
{
    ConstMemberMapIterator find_iterator = member_map_.find(char_guid);
    if (find_iterator == member_map_.end())
    {
        return NULL;
    }

    return &find_iterator->second;
}

void 
DominationMember::RequestData()const
{
    MSG_GZ_DOMINATION_MEMBERLIST_REQ send_msg;
    send_msg.map_code_ = GetMapCode();
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), send_msg.map_code_)));
}

void
DominationMember::MemberInsert(const DominationMemberInfo* member_info)
{
    member_map_.insert(std::make_pair(member_info->member_guid_, *member_info));
}

void
DominationMember::MemberDelete(CHARGUID char_guid)
{
    MemberMapIterator find_iterator =  member_map_.find(char_guid);
    if (find_iterator == member_map_.end())
    {
        return;
    }
    member_map_.erase(find_iterator);
}

void 
DominationMember::ClearMember()
{
    member_map_.clear();
}

BYTE
DominationMember::MemberCount(const bool guild_type, const CHARGUID char_guid) const
{
    ConstMemberMapIterator begin_iterator = member_map_.begin(),
        end_iterator = member_map_.end();

    BYTE member_count = 0;
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationMemberInfo& member_info = begin_iterator->second;
        if (member_info.guild_type_ == guild_type && member_info.member_guid_ != char_guid)
        {
            ++member_count;
        }
    }
    return member_count;
}

BYTE
DominationMember::MemberCount(const bool guild_type) const
{
    ConstMemberMapIterator begin_iterator = member_map_.begin(),
                           end_iterator = member_map_.end();

    BYTE member_count = 0;
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationMemberInfo& member_info = begin_iterator->second;
        if (member_info.guild_type_ == guild_type)
        {
            ++member_count;
        }
    }
    return member_count;
}

void 
DominationMember::SetInfo(const DominationMemberInfo* member_info, int data_count)
{
    if (data_count < 0 || data_count > (MAX_DOMINATION_MEMBER*2))
    {
        assert(!"Member data count is incorrect");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Member count is incorrect : %d", data_count);
        return;
    }

    for (int i=0; i<data_count; ++i)
    {
        MemberInsert(&(member_info[i]));
    }
    SUNLOG(eCRITICAL_LOG,__FUNCTION__"Set Member Info : %d", GetMapCode());

    _IsLoadData(true);
}