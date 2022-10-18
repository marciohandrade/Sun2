#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef __GAMESERVER_SHOP_MANAGER_H
#define __GAMESERVER_SHOP_MANAGER_H


#if _MSC_VER > 1000
#pragma once
#endif


#include <ShopManager.h>
#include "GameShopUser.h"

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
#include "GameShop.BitBlockContainer.h"
#endif


class Player;

//==================================================================================================

enum eSHOP_MANAGER_SIGNAL
{
    eSHOP_SIGNAL_NON_USE_TYPE,
    eSHOP_SIGNAL_RECEIVE_ITEM_LIST = 1,
    eSHOP_SIGNAL_RECEIVE_EXTEND_OPTION_LIST,
    eSHOP_SIGNAL_RECEIVE_MILEAGE_LIST,
    eSHOP_SIGNAL_RECEIVE_PACKAGE_LIST,
    eSHOP_SIGNAL_RECEIVE_BRANCH_LIST,
    eSHOP_SIGNAL_RECEIVE_MAX,
};

//__NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION
struct WaitItemListTick
{
    WaitItemListTick& operator = (DWORD dwWaitItemListTick) 
    { 
        for(int i = 0; i < eSHOP_SIGNAL_RECEIVE_MAX; ++i)
            m_Tick[i] = dwWaitItemListTick;

        return *this;
    }
    DWORD& operator[] (DWORD idx) 
    { 
        return idx < eSHOP_SIGNAL_RECEIVE_MAX ? m_Tick[idx] : m_Tick[eSHOP_SIGNAL_NON_USE_TYPE];
    }

private:
    DWORD m_Tick[eSHOP_SIGNAL_RECEIVE_MAX];
};

//==================================================================================================

class GameShopManager : public ShopManager
{
public:

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    struct SearchInfo
    {
        struct Input
        {
            eSHOP_PARTITION partition_index;
            eSHOP_CATEGORY category_index;
            uint page;
            uint filter;
        };
        struct Output
        {
            enum { Max_Count = 10};
            ShopItem* shopitems[Max_Count];
            uint shopitems_count;
            uint exist_guid[Max_Count];
            uint exist_guid_count;
            uint total_page;
            uint page;
        };

        Input input;
        Output output;
    };

    struct Search_Cache
    {        
        uint change_count;
        STLX_DEQUE<ShopItem*> shopitems;
    };
    
    typedef STLX_HASH_MAP<uint, Search_Cache> Search_Cache_HashMap;
    Search_Cache_HashMap search_cache_hashmap_;

    bool Search(GameShopUser* shopuser, SearchInfo& search_info);
    bool Search(STLX_DEQUE<ShopItem*>& shopitems, ShopCategory* category, GameShopUser* shopuser, SearchInfo& search_info);
    GameShop::BitBlockContainer bit_block_container_;
#endif

    GameShopManager();
    virtual ~GameShopManager();
    //
    virtual void Update();
    //
    void OnStart();
    void OnEnd();
    void OnPlayerDisconnect(Player* pPlayer);
    //
    virtual void BuildTree(DWORD count, tblItemBranch* pBranch IN);
    virtual RC::eSHOP_RESULT DoUserCash(Player* pPlayer, USERGUID user_guid);
    RC::eSHOP_RESULT CanBuyNDoRequest(Player* pPlayer,
                                      BYTE count, int* pPriceGuid,
                                      SLOTIDX slot_index, POSTYPE extend_item_pos,
                                      BOOLEAN purchase_pp_cash);
    virtual RC::eSHOP_RESULT OnResultUserCash(USERGUID user_guid, const CashData& cash_data);
    void OnBuyResult(Player* player, USERGUID user_guid, CHARGUID char_guid,
                     const CashData& cash_data,
                     DWORD result, BYTE count, tblBuyItemTransaction* const buy_list);

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    RC::eSHOP_RESULT OnSendGiftNDoRequest(Player* pPlayer,
                                          BYTE item_type, int price_guid, BOOLEAN purchase_pp_cash,
                                          const QueryUserIdInfo& query_user_id_info,
                                          const char* gift_message);
#else // pre-0905
    public:
        RC::eSHOP_RESULT CanSendGiftNDoRequest(Player* pPlayer, BYTE ItemType, int price_guid,
                                               const char* tszCharName, const char* pszGiftMessage);
        RC::eSHOP_RESULT OnSendGiftNDoRequest(BOOL bSuccessToFoundCharName, Player* pPlayer,
                                              USERGUID ToUserGuid, CHARGUID ToCharGuid,
                                              const char* tszToCharName);
#endif

#ifdef _NA00XXXX_090111_CASH_TYPE_SHARED_
    void OnSendGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                    const CashData& cash_data, PRICEGUID price_guid, DWORD dwResult);
#else //!_NA00XXXX_090111_CASH_TYPE_SHARED_
  #ifdef __CN_0_20070507_SHOP_MILEAGE
    public:
        void OnSendGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                        CASH user_cash, DWORD user_mileage, PRICEGUID price_guid, DWORD dwResult);
  #else
    public:
        void OnSendGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                        CASH user_cash, PRICEGUID price_guid, DWORD dwResult);
  #endif
#endif //!_NA00XXXX_090111_CASH_TYPE_SHARED_
public:
    RC::eSHOP_RESULT CanActionRecvGiftNDoRequest(Player* pPlayer,
                                                 BYTE ActionType, int gift_guid,
                                                 const char* const tszThanksMessage);
    void OnActionRecvGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                          GIFTGUID gift_guid, BYTE ActionType, DWORD dwResult);
    RC::eSHOP_RESULT RequestRecvGiftList(Player* pPlayer);
    RC::eSHOP_RESULT RequestRecvGiftCount(Player* pPlayer);
    void OnRecvGiftList(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                        DWORD count, tblRecvGiftItem* pGiftList);
    void OnRecvGiftListCount(Player* pPlayer, DWORD dwCount);

    // 眉农 风凭
    BOOL IsSuccessTransaction(DWORD dwResult, BYTE& ResultCode OUT);
    RC::eSHOP_RESULT CanActionToPresent(PresentRecvBox::PresentInfo* pPresent, BYTE ActionType);

    void GetItemInfosForPriceUnit(ShopItemPrice* pPrice,
                                  int& KindNum OUT, SLOTCODE* pItemCode OUT, POSTYPE* pItemNum OUT,
                                  DWORD* pDateTime OUT, BYTE* pEnchant OUT, BYTE* pRank OUT);

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING    
    RC::eSHOP_RESULT Serialize(SearchInfo& search_info, uchar* INOUT stream, uint16_t& INOUT size);
#else
    RC::eSHOP_RESULT Serialize(Player* pPlayer, BYTE Category_Sep/*eSHOP_CATEGORY_SEPERATE*/,
                               BYTE Category,
                               BYTE Page, BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se);
#endif
    RC::eSHOP_RESULT SerializeItemDetail(Player* pPlayer, ITEMGUID Guid,
                                         BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se);

#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
    void ReportBuyMultiCashItem(Player* pPlayer, DWORD dwItemCount,
                                PRICEGUID* pPriceGuid, SLOTCODE* pItemCode,
                                BYTE* pCategory, DWORD* pPrice);
    void ReportBuyCashItem(Player* pPlayer, PRICEGUID nPriceGuid,
                           SLOTCODE wItemCode, BYTE byCategory, DWORD dwPrice);
#endif //__NA000000_070523_REPORT_TO_SOLARAUTH

    void ApplyExtendOption(DWORD count, tblItemExtendOption* pOption IN);

    void ReceiveSignal(eSHOP_MANAGER_SIGNAL eSignal, void* pMessage);
    void RequireExtendOptionList();
    void RequireMileageList();
    void RequirePackageList();
    void RequireBranchList();
    void CancelWaitItemList(DWORD idx);


private:
    static const int _MAX_USER_POOL_SIZE = 1000;
    struct InterCache; // f100906.3L, to support local caching data about a ShopItemPrice
    //
    virtual void Release(); // f100906.3L
    // (CHANGE) (WAVERIX) move implementation to code file
    virtual ShopUser* CreateShopUser();
    virtual void DestroyShopUser(ShopUser* pUser);

    BOOLEAN CanEnablePresent(ShopItem* const pShopItem);
    BOOL ServerTypeCheck();
    RC::eSHOP_RESULT ValidShopUser(USERGUID user_guid);

    // data fields...
    InterCache* const inter_cache_;
    WaitItemListTick m_dwWaitItemListTick;   // 东户 内靛 (矫难辑 窃)
    util::CMemoryPoolFactory<GameShopUser>  m_ShopUserPool;
    //
    __DISABLE_COPY(GameShopManager);
};


extern GameShopManager g_GameShopManager;



#endif //__GAMESERVER_SHOP_MANAGER_H

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
