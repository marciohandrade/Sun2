////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PartyFindingManager.h
/// @author hukim
/// @date   2010/04/19
/// @brief  [2010/04/19 by hukim] 작업 내역 최초 생성.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WORLDSERVER_PARTYFINDING_PARTYFINDINGMANAGER_H_
#define WORLDSERVER_PARTYFINDING_PARTYFINDINGMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM 

#include <MemoryPoolFactory.h>


// 파티 모집 정보를 관리하는 클래스
class PartyFindingManager
{   
public:
    typedef RC::PartyFindingResultUtil::ValueType PartyFindingResultValue;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : 생성자 & 소멸자
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    explicit PartyFindingManager();
    ~PartyFindingManager();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : 공개 인터페이스
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    static PartyFindingManager& Instance()
    {
        static PartyFindingManager instance;
        return instance;
    }

public:    
    // 파티 모집을 등록한다.
    PartyFindingResultValue Register(PartyFindingInfo& INOUT temp_party_finding);

    // 파티 모집을 갱신한다.
    PartyFindingResultValue Update(const PartyFindingInfo& temp_party_finding);

    // 파티 모집 목록을 얻는다.
    PartyFindingResultValue GetList(
        PartyFindingInfo* INOUT party_finding_list, 
        PartyFindingListConst::ValueType& OUT party_finding_list_size);

    // 파티 모집을 취소한다.
    PartyFindingResultValue Cancel(User* const party_master, 
                                   const PartyFindingKeyType cancel_wanted_party_finding_key);

    // 파티 모집 정보를 찾는다.
    const PartyFindingInfo* const FindPartyFinding(
        const PartyFindingKeyType party_finding_key) const;

    // 파티 정보를 찾는다.
    const PartyInfo* const FindParty(const PartyKeyType party_key) const;

public:
    // 파티 정보를 생성한다.
    PartyFindingResultValue CreateParty(const PartyInfo& temp_party);

    // 파티 정보를 갱신한다.
    PartyFindingResultValue UpdateParty(PartyInfo& INOUT temp_party);

    // 파티 정보를 삭제한다.
    PartyFindingResultValue DestroyParty(const PartyInfo& temp_party);


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : 내부 구현
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    // 새로운 파티 모집 정보 키를 만든다.
    PartyFindingKeyType MakePartyFindingKey();
    
    // 변경 가능한 파티 모집 정보를 찾는다.
    PartyFindingInfo* FindPartyFindingByMutable(
        const PartyFindingKeyType party_finding_key) const;

    // 파티 모집 정보를 메모리 풀에서 할당한다.
    PartyFindingInfo* AllocatePartyFinding();

    // 변경 가능한 파티 정보를 찾는다.
    PartyInfo* FindPartyByMutable(const PartyKeyType party_key) const;

    // 파티 정보를 메모리 풀에서 할당한다.
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
// REGION : inline 구현
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
