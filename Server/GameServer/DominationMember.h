#pragma once
/*
점령전 참여 맴버 관리 클래스
*/

#ifndef DOMINATION_MEMBER_H
#define DOMINATION_MEMBER_H

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN

typedef STLX_MULTIMAP<CHARGUID, DominationMemberInfo> MemberMap;
typedef MemberMap::iterator MemberMapIterator;
typedef MemberMap::const_iterator ConstMemberMapIterator;

class DominationMember
{
public:
    DominationMember(MAPCODE map_code);
    ~DominationMember();

    //해당 지역 맵코드
    MAPCODE GetMapCode()const;

    //멤버 정보 로드 관련 메소드
    bool IsLoaded()const;
    void RequestData()const;
    void SetInfo(const DominationMemberInfo* member_info, int data_count);

    //guild_type에 해당하는 점령전 맴버의 수를 구한다.
    BYTE MemberCount(const bool guild_type)const;
    BYTE MemberCount(const bool guild_type, const CHARGUID char_guid) const;

    //멤버 노드 접근 메소드
    const DominationMemberInfo* FindMember(CHARGUID char_guid)const;

    //맴버 리스트 작성
    BYTE GetMemberList(DominationMemberInfo* OUT member_list, int max_count)const;

    //멤버 추가 삭제 메소드
    void MemberInsert(const DominationMemberInfo* member_info);
    void MemberDelete(CHARGUID char_guid);

    //맴버 맵 클리어
    void ClearMember();

private:
    //is_lad_data_ 접근 메소드
    bool _IsLoadData()const;
    void _IsLoadData(bool set_value);

private:
    MAPCODE map_code_;
    MemberMap member_map_;
    bool is_load_data_;
};

inline bool
DominationMember::_IsLoadData()const
{
    return is_load_data_;
}

inline void
DominationMember::_IsLoadData(bool set_value)
{
    is_load_data_ = set_value;
}

inline bool
DominationMember::IsLoaded()const
{
    return _IsLoadData();
}

inline MAPCODE
DominationMember::GetMapCode()const
{
    return map_code_;
}
#endif//DOMINATION_MEMBER_H