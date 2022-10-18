#include "StdAfx.h"
#include "BaseParty.h"


BaseParty::BaseParty()
{
    Init(0, 0, ePARTY_MAX);
}

BaseParty::~BaseParty()
{
}

void 
BaseParty::Init(WORD party_key, DWORD master_key, ePARTY_TYPE party_type)
{
    party_key_ = party_key;
    master_key_ = master_key;
    target_key_ = 0;
    party_type_ = party_type;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    distribution_type_ = eITEM_DISTRIBUTION_DICE_AND_DEFAULT;
#else // changed by _NA_002229_20110302_PARTY_DEFUALT_LOOTING_CHANGE
    distribution_type_ = eITEM_DISTRIBUTION_FIFO_PRIORITY;
#endif
    party_member_map_.clear();
}

void 
BaseParty::Release()
{
    Init(0, 0, ePARTY_MAX);
}

RC::ePARTY_RESULT 
BaseParty::Join(const _PARTY_MEMBER_INFO& member_info)
{
    if (GetMemberInfo(member_info.m_dwMemberKey)) 
    {
        return RC::RC_PARTY_ALREADY_IS_MEMBEROFPARTY;
    }
    party_member_map_.insert(MEMBER_MAP::value_type(member_info.m_dwMemberKey, member_info));
    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT 
BaseParty::Leave(DWORD member_key)
{
    if (!GetMemberInfo(member_key)) {
        return RC::RC_PARTY_ISNOT_MEMBEROFPARTY;
    };
    party_member_map_.erase(member_key);

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT 
BaseParty::ChangeMaster(DWORD next_master_key)
{
    if (master_key_ == next_master_key) {
        return RC::RC_PARTY_CANNOT_CHANGE_MASTER;
    };
    if (!GetMemberInfo(next_master_key)) {
        return RC::RC_PARTY_ISNOT_MEMBEROFPARTY;
    };
    master_key_ = next_master_key;

    return RC::RC_PARTY_SUCCESS;
}

const _PARTY_MEMBER_INFO* 
BaseParty::GetHeadUser()
{
    if (!GetMemberNum()) {
        return NULL;
    };
    _PARTY_MEMBER_INFO* member_info = &(*party_member_map_.begin()).second;

    return member_info;
}

const _PARTY_MEMBER_INFO* 
BaseParty::GetMemberInfo(DWORD member_key)const
{
    MEMBER_MAP::const_iterator it = party_member_map_.find(member_key);
    if (it == party_member_map_.end()) {
        return NULL;
    };
    return &it->second;
}

void 
BaseParty::GetMemberInfoAll(PARTY_MEMBER_TOTAL_INFO& member_info_all)
{
    member_info_all.m_Count = 0;

    MEMBER_MAP::iterator it = party_member_map_.begin(),
                         end = party_member_map_.end();
    for ( ; it != end; ++it)
    {
        if (member_info_all.m_Count >= MAX_PARTYMEMBER_NUM) {
            break;
        };
        _PARTY_MEMBER_INFO& memberInfo = it->second;

        BOOST_STATIC_ASSERT(sizeof(member_info_all.m_Slot[member_info_all.m_Count]) ==
                            sizeof(memberInfo));
        member_info_all.m_Slot[member_info_all.m_Count] = memberInfo;
        ++member_info_all.m_Count;
    }
}

DWORD 
BaseParty::GetMemberInfoAll(_GUIDANCE_MEMBER_INFO* member_info_all)
{
    DWORD count = 0;
    MEMBER_MAP::iterator it = party_member_map_.begin(),
                         end = party_member_map_.end();
    for ( ; it != end; ++it, ++count)
    {
        if (count >= MAX_PARTYMEMBER_NUM) {
            break;
        };
        _PARTY_MEMBER_INFO& memberInfo = it->second;
        _GUIDANCE_MEMBER_INFO& guidanceInfo = member_info_all[count];
        guidanceInfo.m_PartyKey = (BYTE)party_key_;
        guidanceInfo.m_bMaster = master_key_ == memberInfo.m_dwMemberKey;
        BOOST_STATIC_ASSERT(sizeof(guidanceInfo.m_MemberInfo) == sizeof(memberInfo));
        guidanceInfo.m_MemberInfo = memberInfo;
    }

    return count;
}

BOOL 
BaseParty::SetMemberInfo(DWORD member_key, _PARTY_MEMBER_INFO& member_info)
{
    _PARTY_MEMBER_INFO* current_info = const_cast<_PARTY_MEMBER_INFO*>(GetMemberInfo(member_key));
    if (current_info == NULL) {
        return false;
    };
    *current_info = member_info;

    return true;
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
VOID 
BaseParty::SetMemberMapCode(DWORD member_key, MAPCODE map_code)
{
    MEMBER_MAP::iterator find_iterator = party_member_map_.find(member_key);
    if (find_iterator == party_member_map_.end()) 
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party member : %d", member_key);
        return;
    }

    _PARTY_MEMBER_INFO& member_info = find_iterator->second;
    member_info.map_code_ = map_code;
}

VOID 
BaseParty::ResetReadyStateAllMember()
{
    MEMBER_MAP::iterator begin_iterator = party_member_map_.begin(),
                         end_iterator = party_member_map_.end();
    for ( ; begin_iterator != end_iterator; ++begin_iterator)
    {
        _PARTY_MEMBER_INFO& member_info = begin_iterator->second;
        member_info.m_isReady = false;
        member_info.m_selecteTeam = 0;
    }
}

VOID 
BaseParty::SetMemeberReadyState(DWORD member_key, BOOL ready_state)
{
    _PARTY_MEMBER_INFO* current_info = const_cast<_PARTY_MEMBER_INFO*>(GetMemberInfo(member_key));
    if (current_info == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found member info : %d", member_key);
        return;
    }

    if (current_info->m_isReady == (ready_state != false))
    {
        SUNLOG(eFULL_LOG, __FUNCTION__" : Already ready state is same : %d", ready_state);
        return;
    }

    current_info->m_isReady = (ready_state != false);
}

BOOL
BaseParty::GetMemeberReadyState(DWORD member_key)const
{
    const _PARTY_MEMBER_INFO* current_info = GetMemberInfo(member_key);
    if (current_info == NULL) 
    {
        return false;
    }

    return current_info->m_isReady;
} 

VOID 
BaseParty::SetMemeberTeam(DWORD member_key, BYTE selected_team)
{
    _PARTY_MEMBER_INFO* current_info = const_cast<_PARTY_MEMBER_INFO*>(GetMemberInfo(member_key));
    if (current_info == NULL) 
    {
        return;
    }

    if (current_info->m_selecteTeam == selected_team)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Already seleted team is same : %d", selected_team);
        return;
    }

    current_info->m_selecteTeam = selected_team;
}

BYTE
BaseParty::GetMemeberTeam(DWORD member_key)const 
    {
    const _PARTY_MEMBER_INFO* current_info = GetMemberInfo(member_key);
    if (current_info == NULL) 
    {
        return false;
    }

    return current_info->m_selecteTeam;
}



BOOL 
BaseParty::IsReadyToAllMember()
{
    MEMBER_MAP::iterator begin_iterator = party_member_map_.begin(),
                         end_iterator = party_member_map_.end();
    for ( ; begin_iterator != end_iterator; ++begin_iterator)
    {
        _PARTY_MEMBER_INFO& member_info = begin_iterator->second;

        if (member_info.m_isReady == false) {
            return false;
        }
    }
    return true;
}
