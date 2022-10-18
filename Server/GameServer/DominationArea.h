#pragma once
/*
 ������ ���� ���� ������ ���� Ŭ����, DominationAuction �� ������ �ִ�.
*/

#ifndef DOMINATION_AREA_H
#define DOMINATION_AREA_H

#ifdef _NA001385_20090924_DOMINATION
#include "DominationAuction.h"
#include "DominationMember.h"

class DominationArea
{
public:
    DominationArea(MAPCODE map_code);
    ~DominationArea();

    //������ �⺻/��� ���� �ε� ���� Ȯ��
    bool IsLoaded()const;

    //Data Load ��û
    void DataLoad()const;

    //������ ���� SET
    void SetInfo(const DOMINATION_INFO& domination_info);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void SetInfo(const DOMINATION_INFO& domination_info, const eDOMINATION_EVENT_STATE domi_state);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    //������ ���� Update
    void UpdateInfo(const DOMINATION_INFO& domination_info);

    //������ �⺻ ���� ���
    const DOMINATION_INFO& GetInfo()const;

    //������ �⺻ ���� ����
    DOMINATION_INFO& GetInfo();

    //�ش� ���� ���ڵ� 
    MAPCODE GetMapCode()const;

    //�溮 ����Ʈ Transaction flag �� ����
    void SetTransactionToDeffenseWall(bool set_value);

    //�溮 ����Ʈ ���� ���� ����
    bool CanIncreaseDeffenseWall()const;

    //������ ��� Ŭ���� �ν��Ͻ� ���
    DominationAuction* GetAuction();
    const DominationAuction* GetAuction()const;

    const RC::eDOMINATION_RESULT CanJoinMember(const GUILDGUID guild_guid, const CHARGUID char_guid)const;

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //������ �ɹ� Ŭ���� �ν��Ͻ� ���
    DominationMember* GetMember();
    const DominationMember* GetMember()const;

    //GUILDGUID�� ������ ���� ���� 
    bool IsDeffenseGuild(GUILDGUID guild_guid)const;
    bool IsAttackGuild(GUILDGUID guild_guid)const;
    bool IsApplyGuild(GUILDGUID guild_guid)const;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //���� ���� ���� ���� ����ΰ�?
    bool IsPrevInGuild(GUILDGUID guild_guid)const;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    //GUILDGUID�� �ش��ϴ� ��尡 ����or���� ��忡 ���� �ϴ°�?
    bool ExistGuild(GUILDGUID guild_guid)const;

    //������ ���� ���� ����(��帶ũ, ����̸�, ��帶����)
    void ChangeGuildMark(GUILDGUID guild_guid, GUILDMARKIDX pattern_index, GUILDMARKIDX background_index);   
    void ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name);
    void ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE GetAreaProgressState() { return domination_info_.progress_state_; }
    void ChangeAreaProgressState(const eDOMINATION_PROGRESS_STATE area_state);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    
private:
    //is_load_data_ ���� �޼ҵ�
    bool _IsLoadData()const;
    void _IsLoadData(bool set_value);

    //������ ���� �ε� ��û
    void _RequestData()const;

    //GUILDGUID ��忡 �ش��ϴ� ���(����or����) ���� ������ ����
    DOMINATION_BASIC_GUILD_INFO* _WhatGuild(GUILDGUID guild_guid);

private:
    MAPCODE map_code_;
    DOMINATION_INFO domination_info_;//������ �⺻ ����
    bool transaction_increase_deffense_wall_; //�溮 ������Ʈ ���� transaction flag
    bool is_load_data_;//������ �⺻ ���� �ε� ����
    DominationAuction auction_;//���
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    DominationMember member_;//������ �ɹ�
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //DOMINATION_INFO ���� ������ �ֱ⿡�� �ʹ� ũ��(DBP���� ���� ����ȭ�� ����ؾ��Ѵ�)
    //�� ������ DB�� ����� �ʿ䵵 ���� ��Ʈ���� ������ �ÿ��� ����ϹǷ� �� Ŭ�������� ����� �д�
    DOMINATION_BASIC_GUILD_INFO m_PrevInGuildInfo;          //���� ���� ��� ���� 
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
};

inline void
DominationArea::UpdateInfo(const DOMINATION_INFO& domination_info)
{
    domination_info_ = domination_info;
}

inline const DOMINATION_INFO&
DominationArea::GetInfo() const
{
    return domination_info_;
}

inline DOMINATION_INFO&
DominationArea::GetInfo()
{
    return domination_info_;
}

inline void 
DominationArea::SetTransactionToDeffenseWall(bool set_value)
{
    transaction_increase_deffense_wall_ = set_value;
}

inline bool
DominationArea::_IsLoadData()const
{
    return is_load_data_;
}

inline void
DominationArea::_IsLoadData(bool set_value)
{
    is_load_data_ = set_value;
}

inline bool
DominationArea::IsLoaded()const
{
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    return _IsLoadData() && auction_.IsLoaded() && member_.IsLoaded();
}

inline MAPCODE
DominationArea::GetMapCode()const
{
    return map_code_;
}

inline DominationAuction* 
DominationArea::GetAuction()
{
    return &auction_;
}

inline const DominationAuction* 
DominationArea::GetAuction()const
{
    return &auction_;
}

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
inline DominationMember* 
DominationArea::GetMember()
{
    return &member_;
}

inline const DominationMember* 
DominationArea::GetMember()const
{
    return &member_;
}

#endif//_NA001385_20090924_DOMINATION
#endif//DOMINATION_AREA_H