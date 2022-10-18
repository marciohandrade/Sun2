////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PartyFindingManager.h
/// @author hukim
/// @date   2010/04/19
/// @brief  [2010/04/19 by hukim] �۾� ���� ���� ����.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WORLDSERVER_PARTYFINDING_PARTYFINDINGMANAGER_H_
#define WORLDSERVER_PARTYFINDING_PARTYFINDINGMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM 

#include <MemoryPoolFactory.h>


// ��Ƽ ���� ������ �����ϴ� Ŭ����
class PartyFindingManager
{   
public:
    typedef RC::PartyFindingResultUtil::ValueType PartyFindingResultValue;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : ������ & �Ҹ���
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    explicit PartyFindingManager();
    ~PartyFindingManager();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : ���� �������̽�
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    static PartyFindingManager& Instance()
    {
        static PartyFindingManager instance;
        return instance;
    }

public:    
    // ��Ƽ ������ ����Ѵ�.
    PartyFindingResultValue Register(PartyFindingInfo& INOUT temp_party_finding);

    // ��Ƽ ������ �����Ѵ�.
    PartyFindingResultValue Update(const PartyFindingInfo& temp_party_finding);

    // ��Ƽ ���� ����� ��´�.
    PartyFindingResultValue GetList(
        PartyFindingInfo* INOUT party_finding_list, 
        PartyFindingListConst::ValueType& OUT party_finding_list_size);

    // ��Ƽ ������ ����Ѵ�.
    PartyFindingResultValue Cancel(User* const party_master, 
                                   const PartyFindingKeyType cancel_wanted_party_finding_key);

    // ��Ƽ ���� ������ ã�´�.
    const PartyFindingInfo* const FindPartyFinding(
        const PartyFindingKeyType party_finding_key) const;

    // ��Ƽ ������ ã�´�.
    const PartyInfo* const FindParty(const PartyKeyType party_key) const;

public:
    // ��Ƽ ������ �����Ѵ�.
    PartyFindingResultValue CreateParty(const PartyInfo& temp_party);

    // ��Ƽ ������ �����Ѵ�.
    PartyFindingResultValue UpdateParty(PartyInfo& INOUT temp_party);

    // ��Ƽ ������ �����Ѵ�.
    PartyFindingResultValue DestroyParty(const PartyInfo& temp_party);


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : ���� ����
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    // ���ο� ��Ƽ ���� ���� Ű�� �����.
    PartyFindingKeyType MakePartyFindingKey();
    
    // ���� ������ ��Ƽ ���� ������ ã�´�.
    PartyFindingInfo* FindPartyFindingByMutable(
        const PartyFindingKeyType party_finding_key) const;

    // ��Ƽ ���� ������ �޸� Ǯ���� �Ҵ��Ѵ�.
    PartyFindingInfo* AllocatePartyFinding();

    // ���� ������ ��Ƽ ������ ã�´�.
    PartyInfo* FindPartyByMutable(const PartyKeyType party_key) const;

    // ��Ƽ ������ �޸� Ǯ���� �Ҵ��Ѵ�.
    PartyInfo* AllocateParty();

private:
    PartyFindingKeyType lastest_party_finding_key_;
    
    typedef STLX_MAP<PartyFindingKeyType, PartyFindingInfo*> PartyFindingInfoMap;
    PartyFindingInfoMap party_finding_map_;
    util::CMemoryPoolFactory<PartyFindingInfo> party_finding_pool_;

    typedef STLX_MAP<PartyKeyType, PartyInfo*> PartyInfoMap;
    PartyInfoMap party_map_;
    util::CMemoryPoolFactory<PartyInfo> party_pool_;
    
private:
    __DISABLE_COPY(PartyFindingManager);

}; //}} PartyFindingManager

typedef PartyFindingManager::PartyFindingResultValue PartyFindingResultValueType;


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : inline ����
////////////////////////////////////////////////////////////////////////////////////////////////////

inline const PartyFindingInfo* const PartyFindingManager::FindPartyFinding( 
    const PartyFindingKeyType party_finding_key) const
{
    return FindPartyFindingByMutable(party_finding_key);
}

inline const PartyInfo* const PartyFindingManager::FindParty(const PartyKeyType party_key) const
{
    return FindPartyByMutable(party_key);
}

inline PartyFindingKeyType PartyFindingManager::MakePartyFindingKey()
{
    PartyFindingKeyType new_party_finding_key = ::GetTickCount();
    while (new_party_finding_key == lastest_party_finding_key_)
    {
        new_party_finding_key = ::GetTickCount();
    }
    lastest_party_finding_key_ = new_party_finding_key;
    return lastest_party_finding_key_;
}

inline PartyFindingInfo* PartyFindingManager::FindPartyFindingByMutable(
    const PartyFindingKeyType party_finding_key) const
{
    const PartyFindingInfoMap::const_iterator& pos = party_finding_map_.find(party_finding_key);
    if (pos != party_finding_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

inline PartyFindingInfo* PartyFindingManager::AllocatePartyFinding()
{
    return static_cast<PartyFindingInfo*>(party_finding_pool_.Alloc());
}

inline PartyInfo* PartyFindingManager::FindPartyByMutable(const PartyKeyType party_key) const
{
    const PartyInfoMap::const_iterator& pos = party_map_.find(party_key);
    if (pos != party_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

inline PartyInfo* PartyFindingManager::AllocateParty()
{
    return static_cast<PartyInfo*>(party_pool_.Alloc());
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

#endif //}} WORLDSERVER_PARTYFINDING_PARTYFINDINGMANAGER_H_
