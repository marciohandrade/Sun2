#pragma once
/*
������ ���� �ɹ� ���� Ŭ����
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

    //�ش� ���� ���ڵ�
    MAPCODE GetMapCode()const;

    //��� ���� �ε� ���� �޼ҵ�
    bool IsLoaded()const;
    void RequestData()const;
    void SetInfo(const DominationMemberInfo* member_info, int data_count);

    //guild_type�� �ش��ϴ� ������ �ɹ��� ���� ���Ѵ�.
    BYTE MemberCount(const bool guild_type)const;
    BYTE MemberCount(const bool guild_type, const CHARGUID char_guid) const;

    //��� ��� ���� �޼ҵ�
    const DominationMemberInfo* FindMember(CHARGUID char_guid)const;

    //�ɹ� ����Ʈ �ۼ�
    BYTE GetMemberList(DominationMemberInfo* OUT member_list, int max_count)const;

    //��� �߰� ���� �޼ҵ�
    void MemberInsert(const DominationMemberInfo* member_info);
    void MemberDelete(CHARGUID char_guid);

    //�ɹ� �� Ŭ����
    void ClearMember();

private:
    //is_lad_data_ ���� �޼ҵ�
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