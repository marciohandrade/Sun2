#pragma once

/*
������ ��� Ŭ���� : ��� ������ ��� ���μ��� 
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

    //��� ���� Set
    void SetInfo(const DominationAuctionInfo* auction_info, int data_count);

    //��� ���� ���ڵ� GET
    MAPCODE GetMapCode()const;

    //��� ���� �ε� �Ϸ� ����
    bool IsLoaded()const;

    //��� ���� ��û
    void RequestData()const;

    //��� ������ �Է� : �ű� ������ �ߺ� ������ �����Ͽ� ���� �Ѵ�.
    eDOMINATION_TENDER_TYPE EntryAuction(const DominationAuctionInfo& auction_info);

    //��� ��� �ݾ�
    MONEY GetTenderCost(GUILDGUID guild_guid)const;

    //�� ���� �ְ� ��� �ݾ�
    MONEY GetBestTenderCost()const;

    //���� ��� ����
    const DominationAuctionInfo* SelectAttackGuild()const;

    //��� ����Ʈ ��û
    BYTE GetAuctionList(DOMINATION_ACTION_INFO* OUT auction_list, BYTE max_count)const;

    //��� ������ ����
    void DeleteAuctionInfo(GUILDGUID guild_guid);

    //��� ������ �ʱ�ȭ
    void ClearAuctionInfo();

    //��� ���� �޸� ������
    void SendRewardMemo(SLOTCODE item_code, eSYSTEM_MEMO_TYPE memo_type)const;
    
    //��� ����Ʈ ����
    BYTE CountAuctionList()const;

    //GUILDGUID�� ��尡 ���� �Ͽ��� : return true(���� ���� ����) / false(���� ���� ����)
    bool IsApplyToGuild(GUILDGUID guild_guid)const;

private:
    //��� ���� �Է�
    void _InsertData();

    //is_load_auction_info_ ���� �޼ҵ�
    bool _IsLoadData()const;
    void _IsLoadData(bool set_value);

    //��� ������ ��Ʈ��(�Է�/����)
    void _InsertData(const DominationAuctionInfo& auction_info);
    void _ChangeData(const DominationAuctionInfo& auction_info);

    //��� ���� ã��/�����ϱ�
    const DominationAuctionInfo* _GetInfo(GUILDGUID guild_guid)const;
    DominationAuctionInfo* _AccessInfo(GUILDGUID guild_guid);

    //��� ���� �ݾ� ��(true:first�� ũ�ų� ������, false:first�� ������)
    bool _Comp(const DominationAuctionInfo* first_data,
               const DominationAuctionInfo* second_data)const;

    //�ְ� ��� �ݾ� ������ ã��
    const DominationAuctionInfo* _FindBestCostGuild()const;

    //���� ������ ������ ������� Random ����
    const DominationAuctionInfo* _ProbabilityFindGuild()const;

private:
    MAPCODE map_code_;//��� ���� ���ڵ�
    AuctionInfoMap auction_info_map_;//��� MAP
    bool is_load_data_;//������ ��� ���� �ε� ����
    SystemMemo system_memo_;//�ý��� �޸� 
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