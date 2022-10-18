#pragma once
/*
 점령전 지역 정보 관리를 위한 클래스, DominationAuction 을 가지고 있다.
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

    //점령전 기본/경매 정보 로드 상태 확인
    bool IsLoaded()const;

    //Data Load 요청
    void DataLoad()const;

    //점령전 정보 SET
    void SetInfo(const DOMINATION_INFO& domination_info);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void SetInfo(const DOMINATION_INFO& domination_info, const eDOMINATION_EVENT_STATE domi_state);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    //점령전 정보 Update
    void UpdateInfo(const DOMINATION_INFO& domination_info);

    //점령전 기본 정보 얻기
    const DOMINATION_INFO& GetInfo()const;

    //점령전 기본 정보 접근
    DOMINATION_INFO& GetInfo();

    //해당 지역 맵코드 
    MAPCODE GetMapCode()const;

    //방벽 포인트 Transaction flag 값 셋팅
    void SetTransactionToDeffenseWall(bool set_value);

    //방벽 포인트 증가 가능 여부
    bool CanIncreaseDeffenseWall()const;

    //현지역 경매 클래스 인스턴스 얻기
    DominationAuction* GetAuction();
    const DominationAuction* GetAuction()const;

    const RC::eDOMINATION_RESULT CanJoinMember(const GUILDGUID guild_guid, const CHARGUID char_guid)const;

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //현지역 맴버 클래스 인스턴스 얻기
    DominationMember* GetMember();
    const DominationMember* GetMember()const;

    //GUILDGUID의 점령전 참여 형태 
    bool IsDeffenseGuild(GUILDGUID guild_guid)const;
    bool IsAttackGuild(GUILDGUID guild_guid)const;
    bool IsApplyGuild(GUILDGUID guild_guid)const;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //현재 맵의 이전 점령 길드인가?
    bool IsPrevInGuild(GUILDGUID guild_guid)const;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    //GUILDGUID에 해당하는 길드가 점령or공격 길드에 존재 하는가?
    bool ExistGuild(GUILDGUID guild_guid)const;

    //점령전 지역 정보 변경(길드마크, 길드이름, 길드마스터)
    void ChangeGuildMark(GUILDGUID guild_guid, GUILDMARKIDX pattern_index, GUILDMARKIDX background_index);   
    void ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name);
    void ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE GetAreaProgressState() { return domination_info_.progress_state_; }
    void ChangeAreaProgressState(const eDOMINATION_PROGRESS_STATE area_state);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    
private:
    //is_load_data_ 접근 메소드
    bool _IsLoadData()const;
    void _IsLoadData(bool set_value);

    //점령전 정보 로드 요청
    void _RequestData()const;

    //GUILDGUID 길드에 해당하는 길드(점령or공격) 정보 포인터 리턴
    DOMINATION_BASIC_GUILD_INFO* _WhatGuild(GUILDGUID guild_guid);

private:
    MAPCODE map_code_;
    DOMINATION_INFO domination_info_;//점령전 기본 정보
    bool transaction_increase_deffense_wall_; //방벽 업데이트 관련 transaction flag
    bool is_load_data_;//점령전 기본 정보 로드 상태
    DominationAuction auction_;//경매
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    DominationMember member_;//점령전 맴버
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //DOMINATION_INFO 에서 가지고 있기에는 너무 크다(DBP에서 부터 동기화를 고려해야한다)
    //이 정보는 DB에 저장될 필요도 없고 몬트샤인 점령전 시에만 사용하므로 이 클래스에서 멤버로 둔다
    DOMINATION_BASIC_GUILD_INFO m_PrevInGuildInfo;          //이전 점령 길드 정보 
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