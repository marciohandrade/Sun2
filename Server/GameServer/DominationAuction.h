#pragma once

/*
점령전 경매 클래스 : 경매 정보및 경매 프로세서 
*/


#ifndef DOMINATION_AUCTION_H
#define DOMINATION_AUCTION_H

#ifdef _NA001385_20090924_DOMINATION

#include "SystemMemo.h"

typedef STLX_MAP<GUILDGUID, DominationAuctionInfo> AuctionInfoMap;
typedef AuctionInfoMap::iterator AuctionInfoIterator;
typedef AuctionInfoMap::const_iterator ConstAuctionInfoIterator;

class DominationAuction
{
public:
    DominationAuction(MAPCODE map_code);
    ~DominationAuction();

    //경매 정보 Set
    void SetInfo(const DominationAuctionInfo* auction_info, int data_count);

    //경매 지역 맵코드 GET
    MAPCODE GetMapCode()const;

    //경매 정보 로드 완료 여부
    bool IsLoaded()const;

    //경매 정보 요청
    void RequestData()const;

    //경매 데이터 입력 : 신규 입찰과 중복 입찰을 구분하여 리턴 한다.
    eDOMINATION_TENDER_TYPE EntryAuction(const DominationAuctionInfo& auction_info);

    //길드 경매 금액
    MONEY GetTenderCost(GUILDGUID guild_guid)const;

    //이 지역 최고 경매 금액
    MONEY GetBestTenderCost()const;

    //선정 길드 선택
    const DominationAuctionInfo* SelectAttackGuild()const;

    //경매 리스트 요청
    BYTE GetAuctionList(DOMINATION_ACTION_INFO* OUT auction_list, BYTE max_count)const;

    //경매 데이터 삭제
    void DeleteAuctionInfo(GUILDGUID guild_guid);

    //경매 데이터 초기화
    void ClearAuctionInfo();

    //경매 보상 메모 보내기
    void SendRewardMemo(SLOTCODE item_code, eSYSTEM_MEMO_TYPE memo_type)const;
    
    //경매 리스트 갯수
    BYTE CountAuctionList()const;

    //GUILDGUID의 길드가 입찰 하였나 : return true(입찰 내역 존재) / false(입찰 내역 없음)
    bool IsApplyToGuild(GUILDGUID guild_guid)const;

private:
    //경매 정보 입력
    void _InsertData();

    //is_load_auction_info_ 접근 메소드
    bool _IsLoadData()const;
    void _IsLoadData(bool set_value);

    //경매 데이터 컨트롤(입력/변경)
    void _InsertData(const DominationAuctionInfo& auction_info);
    void _ChangeData(const DominationAuctionInfo& auction_info);

    //경매 정보 찾기/접근하기
    const DominationAuctionInfo* _GetInfo(GUILDGUID guild_guid)const;
    DominationAuctionInfo* _AccessInfo(GUILDGUID guild_guid);

    //경매 입찰 금액 비교(true:first가 크거나 같으면, false:first가 작으면)
    bool _Comp(const DominationAuctionInfo* first_data,
               const DominationAuctionInfo* second_data)const;

    //최고 경매 금액 데이터 찾기
    const DominationAuctionInfo* _FindBestCostGuild()const;

    //입찰 아이템 갯수를 기반으로 Random 선정
    const DominationAuctionInfo* _ProbabilityFindGuild()const;

private:
    MAPCODE map_code_;//경매 지역 맵코드
    AuctionInfoMap auction_info_map_;//경매 MAP
    bool is_load_data_;//점령전 경매 정보 로드 상태
    SystemMemo system_memo_;//시스템 메모 
};

inline bool
DominationAuction::_IsLoadData()const
{
    return is_load_data_;
}

inline void
DominationAuction::_IsLoadData(bool set_value)
{
    is_load_data_ = set_value;
}

inline MAPCODE
DominationAuction::GetMapCode()const
{
    return map_code_;
}

inline bool
DominationAuction::IsLoaded()const
{
    return _IsLoadData();
}
#endif//_NA001385_20090924_DOMINATION
#endif//DOMINATION_AUCTION_H