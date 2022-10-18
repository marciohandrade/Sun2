#include "stdafx.h"
#include "./GameShopManager.h"


#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


//==================================================================================================
#include <PacketStruct_GS.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_GM.h>
#include <SCItemSlotContainer.h>
#include <ServerOptionParserEx.h>

#include "./GameServerEx.h"
#include "./Player.h"
#include "./PlayerManager.h"
#include "./ItemManager.h"
#include "./FunctionalRestrictManager.h"
#include "./ShopUser.h"
#include "./PresentRecvBox.h"
#include "ItemInfoParser.h"

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
#include "GameShop.ShopItemBuilder.h"
#include <ShopCategory.h>
#endif
//==================================================================================================
// f100906.3L
struct GameShopManager::InterCache
{
    // ref: MAX_BUY_REQ_SIZE
    struct ItemInfoNode
    {
        const BASE_ITEMINFO* item_info;
        POSTYPE number;
        uint8_t enchant;
        KindOfItemType::value_type item_kind; // replace rank
        DWORD use_time;
    };
    struct PriceNode
    {
        PRICEGUID price_guid;
        //
        ulong is_valid_node : 1;
        ulong is_package : 1;
        ulong is_expired : 1;
        ulong is_pc_room_only : 1;
        ulong is_pp_cash_tab_item : 1;
        ulong is_buy_pp_cash_only : 1; // used by _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
        //
        ShopItemPrice* shop_item_price;
        ShopItem* shop_item;
        DWORD price;
        int number_of_items;
        ItemInfoNode shop_item_nodes[MAX_PACKAGE_ITEM_SIZE];
    };
    typedef STLX_HASH_MAP<PRICEGUID, PriceNode> PRICE_INFO_CACHE;
    //
    struct CustomMakingItemInfoBlock
    {
        POSTYPE number_of_kinds;
        SLOTCODE item_code_array_[MAX_INVENTORY_SLOT_NUM];
        POSTYPE item_number_array_[MAX_INVENTORY_SLOT_NUM];
        DWORD date_time_array_[MAX_INVENTORY_SLOT_NUM];
        uint8_t enchant_array_[MAX_INVENTORY_SLOT_NUM];
        KindOfItemType::value_type item_type_array_[MAX_INVENTORY_SLOT_NUM];
    };
    //
    void Clear();
    RC::eSHOP_RESULT QueryPriceNode(ShopItemPrice* shop_item_price, const PriceNode** price_node);
    CustomMakingItemInfoBlock* SetupMakingItemInfo(const PriceNode** price_node_array,
                                                   size_t number_of_price_nodes);
    //
    static InterCache* Instance();
    static void DestroyInstance();
private:
    //
    PRICE_INFO_CACHE price_info_cache_;
    CustomMakingItemInfoBlock making_item_info_block_;
    static InterCache** inter_cache_static_;
};

GameShopManager::InterCache** GameShopManager::InterCache::inter_cache_static_;

GameShopManager::InterCache* GameShopManager::InterCache::Instance()
{
    static InterCache* inter_cache_static = new InterCache;
    inter_cache_static_ = &inter_cache_static;
    return inter_cache_static;
}

void GameShopManager::InterCache::DestroyInstance()
{
    Instance();
    if (*inter_cache_static_ == NULL) {
        return;
    }
    (*inter_cache_static_)->Clear();
    delete (*inter_cache_static_);
    *inter_cache_static_ = NULL;
}

void GameShopManager::InterCache::Clear()
{
    price_info_cache_.clear();
}

// WARNING
RC::eSHOP_RESULT GameShopManager::InterCache::QueryPriceNode(ShopItemPrice* shop_item_price,
                                                             const PriceNode** result_price_node)
{
    assert(shop_item_price);
    struct ResultHandler {
        ResultHandler()
            : price_node_(NULL), result_(RC::RC_SHOP_SUCCESS)
        {}
        ~ResultHandler()
        {
            if (price_node_ && result_ != RC::RC_SHOP_SUCCESS) {
                price_node_->is_valid_node = false;
            }
        }
        //
        PriceNode* price_node_;
        RC::eSHOP_RESULT result_;
    } result_handler;
    //
    const PRICEGUID price_guid = shop_item_price->GetPriceGuid();
    bool new_insert = false;
    PRICE_INFO_CACHE::iterator found = price_info_cache_.find(price_guid);
    PriceNode* price_node = NULL;
    if (found != price_info_cache_.end())
    {
        price_node = &found->second;
        result_handler.price_node_ = price_node;
    }
    else
    {   // new insert
        price_node = &price_info_cache_[price_guid];
        ZeroMemory(price_node, sizeof(*price_node));
        result_handler.price_node_ = price_node;
        new_insert = true;
        // align node
        price_node->price_guid = price_guid;
        price_node->shop_item_price = shop_item_price;
        //
        ShopItem* shop_item = shop_item_price->GetShopItem();
        price_node->is_valid_node = (shop_item != NULL);
        if (price_node->is_valid_node == false) {
            return (result_handler.result_ = RC::RC_SHOP_CANNOTBUYITEMPRICE);
        };
        ;{
            price_node->is_package = (shop_item->GetType() != ShopItem::SINGLE);
            eSHOP_CATEGORY category = static_cast<eSHOP_CATEGORY>(shop_item->GetCategory());
            price_node->is_pc_room_only = (category == SHOP_CATEGORY_PCROOM);
            price_node->is_pp_cash_tab_item = (category == SHOP_CATEGORY_PPCASH);
            //price_node->is_buy_pp_cash_only
            price_node->shop_item = shop_item;
            price_node->price = shop_item_price->GetPrice();
        };
        //
        const ItemInfoParser* item_parser = ItemInfoParser::Instance();
        if (price_node->is_package == false)
        {
            SingleItem* const single_item = static_cast<SingleItem*>(shop_item);
            SLOTCODE item_code = single_item->GetItemCode();
            const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(item_code);
            if (item_info == NULL) {
                return (result_handler.result_ = RC::RC_SHOP_CANNOTBUYITEM);
            }
            //
        #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
            price_node->is_buy_pp_cash_only = (item_info->m_ChargeType == eCHARGE_CHARGE_PPCARD);
        #endif
            //
            ItemInfoNode* shop_item_node = &price_node->shop_item_nodes[price_node->number_of_items];
            ++price_node->number_of_items;
            shop_item_node->item_info = item_info;
            DURATYPE amount = shop_item_price->GetAmount();
            shop_item_node->number = amount == 0 ? ++amount : amount;
            shop_item_node->enchant = single_item->GetEnchant();
            shop_item_node->item_kind = single_item->GetRank();
            shop_item_node->use_time = shop_item_price->GetUseTime() * 60; // convert minutes to seconds
            //
        }
        else //price_node->is_package == true
        {
            PackageItem* package_item = static_cast<PackageItem*>(shop_item);
            const BYTE piece_count = package_item->GetPieceCount();
            for (BYTE piece_idx = 0;
                 piece_idx < piece_count;
                 ++piece_idx, ++price_node->number_of_items)
            {
                const PackageItem::ItemPiece* piece = package_item->GetItemPiece(piece_idx);
                const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(piece->ItemCode);
                if (item_info == NULL) {
                    return (result_handler.result_ = RC::RC_SHOP_CANNOTBUYITEM);
                }
                //
            #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
                if (price_node->is_buy_pp_cash_only == false) {
                    price_node->is_buy_pp_cash_only = \
                        (item_info->m_ChargeType == eCHARGE_CHARGE_PPCARD);
                }
            #endif
                //
                ItemInfoNode* const shop_item_node = \
                    &price_node->shop_item_nodes[price_node->number_of_items];
                //
                shop_item_node->item_info = item_info;
                DURATYPE amount = piece->Amount;
                shop_item_node->number = amount == 0 ? ++amount : amount;
                shop_item_node->enchant = piece->Enchant;
                shop_item_node->item_kind = piece->Rank;
                // convert minutes to seconds
                shop_item_node->use_time = piece->UseTime * 60;
            }
        }
        // end section 'new insert'
    };
    // check the expiration date
    price_node->is_expired = (price_node->shop_item->IsSellEndDate() != false);
    if (price_node->is_expired) {
        return (result_handler.result_ = RC::RC_SHOP_CANNOTBUYITEMPRICE);
    }
    // last field
    *result_price_node = price_node;
    return RC::RC_SHOP_SUCCESS;
}

GameShopManager::InterCache::CustomMakingItemInfoBlock*
GameShopManager::InterCache::SetupMakingItemInfo(const PriceNode** price_node_array,
                                                 size_t number_of_price_nodes)
{
    ZeroMemory(&making_item_info_block_, sizeof(making_item_info_block_));
    CustomMakingItemInfoBlock* const making_node = &making_item_info_block_;
    SLOTCODE* making_node_item_code_it = making_node->item_code_array_;
    POSTYPE* making_node_number_it = making_node->item_number_array_;
    DWORD* making_node_datetime_it = making_node->date_time_array_;
    uint8_t* making_node_enchant_it = making_node->enchant_array_;
    KindOfItemType::value_type* making_node_item_type_it = making_node->item_type_array_;
    //
    const PriceNode** price_node_it = price_node_array;
    for (size_t index = 0; index < number_of_price_nodes; ++index)
    {
        const PriceNode* price_node = *price_node_it;
        if (price_node->is_valid_node == false) {
            assert(price_node->is_valid_node);
            continue;
        };
        const ItemInfoNode* it = price_node->shop_item_nodes,
                          * end = price_node->shop_item_nodes + price_node->number_of_items;
        for ( ; it != end; ++it, ++making_node->number_of_kinds)
        {
            *making_node_item_code_it++ = it->item_info->m_Code;
            *making_node_number_it++ = it->number;
            *making_node_datetime_it++ = it->use_time;
            *making_node_enchant_it++ = it->enchant;
            *making_node_item_type_it++ = it->item_kind;
        }
    }; //end 'for'
    return making_node;
}

//==================================================================================================

GameShopManager g_GameShopManager;

//==================================================================================================

GameShopManager::GameShopManager()
    : inter_cache_(InterCache::Instance())
{
    m_ShopUserPool.Initialize(20, 20, "GameShopUser");

    m_dwWaitItemListTick = INFINITE;

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    bit_block_container_.Initialize(20, 20);
#endif
}

GameShopManager::~GameShopManager()
{
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    bit_block_container_.Release();
#endif

    InterCache::DestroyInstance();
}

// (CHANGE) (WAVERIX) move implementation to code file
ShopUser*
GameShopManager::CreateShopUser()
{
    return (ShopUser *)m_ShopUserPool.Alloc();
}

void
GameShopManager::DestroyShopUser(ShopUser* pUser)
{
    pUser->Release();
    m_ShopUserPool.Free((GameShopUser*)pUser);
}

//==================================================================================================

BOOL
GameShopManager::IsSuccessTransaction(DWORD dwResult, BYTE& result_code_out OUT)
{
    result_code_out = RC::RC_SHOP_FAILED;
    switch(dwResult)
    {
    case eRET_FAIL:
        break;
    case eRET_SUCCESS:
        return true;
    case eRET_NOT_ENOUGH_CASH:
        result_code_out = RC::RC_SHOP_NOTENOUGHCASH;
        break;
    case eRET_DB_ERROR:
        result_code_out = RC::RC_SHOP_DB_ERROR;
        break;
    case eRET_NOT_FOUND_ITEM:
        result_code_out = RC::RC_SHOP_CANNOTBUYITEM;
        break;
    case eRET_LIMIT_BUY_COUNT_ERROR:
        result_code_out = RC::RC_SHOP_CANNOTBUYBYLIMITCOUNT;
        break;
    case eRET_BUY_COOL_TIME_ERROR:
        result_code_out = RC::RC_SHOP_NOTEXPIREDCOOLTIME;
        break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    case eRET_NOT_ENOUGH_MILEAGE:
        result_code_out = RC::RC_SHOP_NOTENOUGHMILEAGE;
        break;
#endif
        //_US_SHOP_GIFT_ERRORCONTROL_ADD �Ϲ� �����ϱ� ���� ����� ó��
    case eRet_INVALID_BILLING_PARAM:
        result_code_out = RC::RC_SHOP_INVALID_BILLING_PARAM;
        break;
    case eRet_BILLING_NETWORK_ERROR:
        result_code_out = RC::RC_SHOP_BILLING_NETWORK_ERROR;
        break;
    case eRet_BILLING_SERVER_LOGIC_ERROR:   
        result_code_out = RC::RC_SHOP_BILLING_SERVER_LOGIC_ERROR;
        break;
    case eRet_NOT_EXIST_BILLING_ITEM_CODE:
        result_code_out = RC::RC_SHOP_NOT_EXIST_BILLING_ITEM_CODE;
        break;
    case eRet_NOT_EXIST_BILLING_TRADE_CODE:
        result_code_out = RC::RC_SHOP_NOT_EXIST_BILLING_TRADE_CODE;
        break;
    case eRet_EXCEED_LIMIT_CHARGE:
        result_code_out = RC::RC_SHOP_EXCEED_LIMIT_CHARGE;
        break;
    case eRet_ABNORMALITY_CHARGE_TRANSACTION:
        result_code_out = RC::RC_SHOP_ABNORMALITY_CHARGE_TRANSACTION;
        break;
    case eRet_GIFT_ITEM_BLACK_LIST:
        result_code_out = RC::RC_SHOP_GIFT_ITEM_BLACK_LIST;
        break;
    case eRet_GIFT_ITEM_BANNED_USER:
        result_code_out = RC::RC_SHOP_GIFT_ITEM_BANNED_USER;
        break;
    case eRet_GIFT_ITEM_NOT_EXIST_HISTORY:   
        result_code_out = RC::RC_SHOP_GIFT_ITEM_NOT_EXIST_HISTORY;
        break;
    case eRet_EXCEED_LIMIT_GIFT_ITEM:        
        result_code_out = RC::RC_SHOP_EXCEED_LIMIT_GIFT_ITEM;
        break;
    case eRet_BILLING_SYSTEM_ERROR:         
        result_code_out = RC::RC_SHOP_BILLING_SYSTEM_ERROR;
        break;
    case eRet_BILLING_ETC_ERROR:            
        result_code_out = RC::RC_SHOP_BILLING_ETC_ERROR;
        break;
        //_US_SHOP_GIFT_ERRORCONTROL_ADD �Ϲ� �����ϱ� ���� ����� ó�� END
    }
    return false;
}

//==================================================================================================

BOOL
GameShopManager::ServerTypeCheck()
{
    if (ServerOptionParserEx::Instance()->GetServerOption().m_byUseItemShopBattle == false &&
       g_pGameServer->GetServerType() != FIELD_SERVER)
    {
        return false;
    }

    return true;
}

//==================================================================================================

void
GameShopManager::OnStart()
{
    if (!ServerTypeCheck())
        return;

    if (IsTransaction())
    {
        SUNLOG(eCRITICAL_LOG, "Shop Info Request Transaction Already is going");
        return;
    }

    m_dwWaitItemListTick = INFINITE;

    // �������� ������ ����Ʈ ��� ��û
    MSG_GS_SHOP_ITEM_LIST_REQ_SYN msg;
    if (g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        SUNLOG(eCRITICAL_LOG, "MSG_GS_SHOP_ITEM_LIST_REQ_SYN send to SHOP_SERVER");

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
        GameShop::ShopItemBuilder shopitem_builder(&g_GameShopManager);
        shopitem_builder.ClearAll();
        search_cache_hashmap_.clear();
        bit_block_container_.Release();
        bit_block_container_.Initialize(20, 20);
#endif

        Release();
        Init();
        SetTransaction(true);
    }
}

//==================================================================================================

void
GameShopManager::OnEnd()
{
    if (!ServerTypeCheck())
        return;

    SetTransaction(false);

    Release();
}

// f100906.3L
void GameShopManager::Release()
{
    inter_cache_->Clear();
    ShopManager::Release();
}

//==================================================================================================
//==================================================================================================

void
GameShopManager::BuildTree(DWORD count, tblItemBranch* pBranch IN)
{
    ShopManager::BuildTree(count, pBranch);

    SetTransaction(false);
}

//==================================================================================================

void
GameShopManager::OnPlayerDisconnect(Player* pPlayer)
{
    OnRemoveShopUser(pPlayer->GetUserGuid());
}



#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

RC::eSHOP_RESULT GameShopManager::Serialize(GameShopManager::SearchInfo& search_info, uchar* INOUT stream, uint16_t& INOUT size)
{
    if (!ServerTypeCheck())
        return RC::RC_SHOP_CANNOT_USE_SHOP;

    if (!g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_SHOP_SERVICE))
        return RC::RC_SHOP_SERVICE_STOP;

    if (IsTransaction())
        return RC::RC_SHOP_STANDBY_SERVICE;


    ArchiveOfStream archive(stream, size);

    uint8_t count = search_info.output.shopitems_count;
    archive << (uint8_t)count;

    ShopItem* shopitem = NULL;
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        shopitem = search_info.output.shopitems[idx];
        shopitem->Serialize(*this, archive, eSHOP_SERIALIZE_ITEM_LIST_LOAD);
    }

    int archive_size = archive.GetSize();
    ASSERT(archive_size <= USHRT_MAX);
    size = static_cast<uint16_t>(archive_size);

    return RC::RC_SHOP_SUCCESS;
}

#else

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::Serialize(Player* pPlayer,
                           BYTE Category_Sep/*eSHOP_CATEGORY_SEPERATE*/, BYTE Category,
                           BYTE Page, BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se)
{
    if (!ServerTypeCheck())
        return RC::RC_SHOP_CANNOT_USE_SHOP;

    /*if (!pPlayer || pPlayer->GetGameZoneState() != ePRS_AT_VILLAGE)
    return RC::RC_SHOP_CANNOT_USE_SHOP;*/

    if (!g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_SHOP_SERVICE))
        return RC::RC_SHOP_SERVICE_STOP;

    if (IsTransaction())
        return RC::RC_SHOP_STANDBY_SERVICE;

    ShopManager::Serialize(Category_Sep, Category, Page, pStream, Size, se);

    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


RC::eSHOP_RESULT
GameShopManager::SerializeItemDetail(Player* pPlayer, ITEMGUID item_guid,
                                     BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se)
{
    if (!ServerTypeCheck())
        return RC::RC_SHOP_CANNOT_USE_SHOP;

    if (!g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_SHOP_SERVICE))
        return RC::RC_SHOP_SERVICE_STOP;

    if (IsTransaction())
        return RC::RC_SHOP_STANDBY_SERVICE;

    ShopManager::SerializeItemDetail(item_guid, pStream, Size, se);

    return RC::RC_SHOP_SUCCESS;
}


//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::DoUserCash(Player* pPlayer, USERGUID user_guid)
{
    if (!ServerTypeCheck())
        return RC::RC_SHOP_CANNOT_USE_SHOP;

    if (!g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_SHOP_SERVICE))
        return RC::RC_SHOP_SERVICE_STOP;

    if (IsTransaction())
        return RC::RC_SHOP_STANDBY_SERVICE;

    MSG_GS_SHOP_USER_CASH_REQ_SYN msg;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = pPlayer->GetGameChuUsn();
#endif
    msg.dwUserGuid = user_guid;
#if defined(__NA000000_SHOP_USERID_50_PASSING_PROC__) //&& 0
    strncpy(msg.user_id, pPlayer->GetUserID(), _countof(msg.user_id));
    msg.user_id[_countof(msg.user_id) - 1] = '\0';
#endif
    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;

    return RC::RC_SHOP_PENDING;
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::CanBuyNDoRequest(Player* pPlayer, BYTE count, int* pPriceGuid,
                                  SLOTIDX slot_index, POSTYPE extend_item_pos,
                                  BOOLEAN purchase_pp_cash)
{
    if (ServerTypeCheck() == false) {
        return RC::RC_SHOP_CANNOT_USE_SHOP;
    }
    if (g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_BUY_SHOPITEM) == false) {
        return RC::RC_SHOP_SERVICE_STOP;
    }
    if (g_FunctionalRestrictMgr.IsPossibleFunction(eFUNCTIONALRESTRICT_SHOP_SERVICE) == false) {
        return RC::RC_SHOP_SERVICE_STOP;
    }
    if (IsTransaction()) {
        return RC::RC_SHOP_STANDBY_SERVICE;
    }
    if (count > MAX_BUY_REQ_SIZE) {
        return RC::RC_SHOP_OVER_BUY_COUNT;
    }

    USERGUID user_guid = pPlayer->GetUserGuid();
    CHARGUID char_guid = pPlayer->GetCharGuid();
    const char* pCharName = pPlayer->GetCharName();

    GameShopUser* shop_user = (GameShopUser*)FindShopUser(user_guid);
    if (shop_user == NULL) {
        return RC::RC_SHOP_USERISNOTFOUND;
    }
    if (shop_user->IsTransaction()) {
        return RC::RC_SHOP_ALREADY_BUYING_TRANSACTION;
    }
    ;{  // NOTE: �� �ǹ� ���� ������ �ǰ� �ִ�. GetCharName�� ��Ȯ�� ������ �� �ֵ��� �����Ѵ�.
        size_t charnamelen = strlen(pCharName);
        if (charnamelen > (MAX_CHARACTER_NAME_LENGTH - 1)) {
            return RC::RC_SHOP_INVALID_CHAR_NAME_LEN;
        }
    };
    // CHANGES: f100907.3L
    //          f101012.2L, fixed an invalid check routine whether the pc room user.
    bool is_pc_room = (pPlayer->CanApplyPCBangBenefit(true) != false);
    size_t number_of_price_cache_nodes = 0;
    const InterCache::PriceNode* price_cache_nodes[MAX_BUY_REQ_SIZE];
    for ( ; number_of_price_cache_nodes < count; ++number_of_price_cache_nodes)
    {
        ShopItemPrice* const shop_item_price = \
            FindShopItemPrice(pPriceGuid[number_of_price_cache_nodes]);
        if (shop_item_price == NULL) {
            return RC::RC_SHOP_CANNOTBUYITEMPRICE;
        }
        //
        RC::eSHOP_RESULT shop_result = inter_cache_->QueryPriceNode(\
            shop_item_price, &price_cache_nodes[number_of_price_cache_nodes]);
        if (shop_result != RC::RC_SHOP_SUCCESS) {
            return shop_result;
        }

        const InterCache::PriceNode* price_node = price_cache_nodes[number_of_price_cache_nodes];
        if (is_pc_room == false && price_node->is_pc_room_only) {
            return RC::RC_SHOP_CANNOT_BUY_PC_ROOM_ITEM_GENERAL_USER;
        }
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
        if (purchase_pp_cash == false && price_node->is_buy_pp_cash_only) {
            return RC::RC_SHOP_CAN_BUY_PP_CASH_ONLY;
        }
    #endif
    }

//#ifdef __NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__
//    CInventorySlotContainer* const pSlotContainer = static_cast<CInventorySlotContainer*>( \
//        pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));
//#endif
    char __msg_block[sizeof(MSG_GS_SHOP_BUY_ITEM_SYN)] = { 0, };
    MSG_GS_SHOP_BUY_ITEM_SYN& msg = *new (__msg_block) MSG_GS_SHOP_BUY_ITEM_SYN;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = pPlayer->GetGameChuUsn();
    strncpy(msg.ip_address_, pPlayer->GetUserIP(), _countof(msg.ip_address_));
    msg.ip_address_[_countof(msg.ip_address_) - 1] = '\0';
#endif
    {
        msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey();
        msg.dwUserGuid = user_guid;
    #if defined(__NA000000_SHOP_USERID_50_PASSING_PROC__) //&& 0
        strncpy(msg.user_id, pPlayer->GetUserID(), _countof(msg.user_id));
        msg.user_id[_countof(msg.user_id) - 1] = '\0';
        msg.char_class = pPlayer->GetCharType();
        msg.char_level = pPlayer->GetLevel();
    #endif
        msg.dwCharGuid = char_guid;
        strncpy(msg.szCharName, pCharName, _countof(msg.szCharName));
        msg.szCharName[_countof(msg.szCharName) - 1] = '\0';
        msg.dwCount = count;
    };
    //
    CASH total_cash = 0;
    CASH total_pp_cash = 0; //
    MONEY total_money = 0;
    DWORD total_mileage = 0; //__NA_00387_20070605_SHOP_MILEAGE2
    //
    BOOST_STATIC_ASSERT(_countof(msg.dwPriceGuids) == _countof(price_cache_nodes));
    bool is_buy_pp_cash_only = false;
    for (size_t index = 0; index < number_of_price_cache_nodes; ++index)
    {
        const InterCache::PriceNode* price_node = price_cache_nodes[index];
        assert(price_node);
        msg.dwPriceGuids[index] = price_node->price_guid;
        //
        CASH* cash_selector = &total_cash;
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
        if (purchase_pp_cash) {
            cash_selector = &total_pp_cash;
        }
    #endif
        // f100910.3L, add an only P-Coin purchasable tab rule
        if (is_buy_pp_cash_only == false && price_node->is_buy_pp_cash_only) {
            is_buy_pp_cash_only = true;
        }
        if (purchase_pp_cash == false && is_buy_pp_cash_only) {
            return RC::RC_SHOP_CAN_BUY_PP_CASH_ONLY;
        }
        switch (price_node->shop_item->GetMoneyType())
        {
        case MONEY_CASH_TYPE:
            *cash_selector = *cash_selector + price_node->price;
            break;
        case MONEY_HIME_TYPE:
            total_money += price_node->price;
            break;
    #ifdef __NA_00387_20070605_SHOP_MILEAGE2
        case MONEY_MILEAGE_TYPE:
            total_mileage += price_node->price;
            break;
    #endif
        default:
            return RC::RC_SHOP_CANNOTBUYITEMPRICE;
        }
    }
    //=============================================================================================
    if (pPlayer->GetMoney() < total_money)
        return RC::RC_SHOP_NOTENOUGHMONEY;
    if (purchase_pp_cash == false) {
        if (HaveCash(user_guid, total_cash) == false)
            return RC::RC_SHOP_NOTENOUGHCASH;
    }
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    if (HaveMileage(user_guid, total_mileage) == false)
        return RC::RC_SHOP_NOTENOUGHMILEAGE;
#endif
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    if (purchase_pp_cash != false) {
        if (HavePPCash(user_guid, total_pp_cash) == false)
            return RC::RC_SHOP_NOTENOUGHCASH;
    }
    //
    if (purchase_pp_cash == false) {
        memset(msg.buy_cash_types, msg.PURCHASE_DEFAULT, number_of_price_cache_nodes);
    }
    else {
        memset(msg.buy_cash_types, msg.PURCHASE_PP_CASH, number_of_price_cache_nodes);
    }
#endif

    //DISPMSG("user_guid:%u,char_guid:%u,Num:%u,", user_guid, char_guid, count);
    //DISPMSG("total_money:%I64u,total_cash:%u\n", total_money, total_cash);

    //=============================================================================================
    ItemManager* item_manager = pPlayer->GetItemManager();
    InterCache::CustomMakingItemInfoBlock* const making_item_node = \
        inter_cache_->SetupMakingItemInfo(price_cache_nodes, number_of_price_cache_nodes);
    if (making_item_node->number_of_kinds == 0) {
        return RC::RC_SHOP_CANNOTBUYITEMPRICE;
    }
    //
    shop_user->ClearTransactionInfo(); // f100909.1L
    //
    const ItemInfoParser* item_parser = ItemInfoParser::Instance();
    // ������ִ��� üũ
    // �Ⱓ���� ������ ������ ��� ������ �������� ���Կ� Lock�� �Ǵ�.
    const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(making_item_node->item_code_array_[0]);
    assert(item_info);
    const bool is_extend_date = item_info && (item_info->m_wType == eITEMTYPE_EXTEND_DATE_WEAPON ||
                                              item_info->m_wType == eITEMTYPE_EXTEND_DATE_ARMOR ||
                                              item_info->m_wType == eITEMTYPE_EXTEND_DATE_RIDER);
    if (is_extend_date)
    {
        if (RC::RC_ITEM_SUCCESS != item_manager->ReservedExtendDateItem(\
                making_item_node->item_code_array_, slot_index, extend_item_pos,
                shop_user->locked_info_array_, &shop_user->number_of_used_locked_infos_))
        {
            return RC::RC_SHOP_NOT_EXIST_EXTEND_ITEM;
        }

        // �Ⱓ���� ������ ���� Pos ����
        if (GameShopUser::ExtendSlotInfo* const extend_slot_info = &shop_user->extend_slot_info_)
        {
            extend_slot_info->slot_index = slot_index;
            extend_slot_info->slot_pos = extend_item_pos;
        };
    }
    // (Ref100525.1L) a reserved replacement (rank info -> item type info)
    else if (RC::RC_ITEM_SUCCESS != item_manager->ReserveSpaceAllKindOfItem(\
                making_item_node->number_of_kinds,
                making_item_node->item_code_array_, making_item_node->item_number_array_,
                making_item_node->date_time_array_,
                making_item_node->enchant_array_,
                making_item_node->item_type_array_, // use KindOfItemType
                NULL, // unused rank
                GameShopUser::MAX_LOCKEDINFO_NUM,
                shop_user->locked_info_array_,
                &shop_user->number_of_used_locked_infos_))
    {
        return RC::RC_SHOP_NOTENOUGHINVENTORYSPACE;
    }
    //=============================================================================================
    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        // ���н� �ѹ�
        item_manager->RollbackReservedAllKindOfItem(shop_user->locked_info_array_,
                                                    shop_user->GetUsedLockedCount());
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;
    }
    //=============================================================================================
    // pre patch step
    shop_user->option_tag_.value = shop_user->option_tag_.Tag_None;
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    if (purchase_pp_cash) {
        shop_user->option_tag_.value = shop_user->option_tag_.Tag_PCoinPurchase;
    }
#endif
    // ����ȭ�� ���� ���� ����
    if (0 != total_money)
    {
        // �̸� ������ ����
        pPlayer->MinusMoney(total_money);
        GAMELOG->LogMoney(MONEY_CHARGE_BUY_BY_HIME, pPlayer, total_money);
    }
    if (0 != total_cash)
    {
        // [ĳ�����������α�]
        GAMELOG->LogMoney(MONEY_CHARGE_BUY_BY_CASH, pPlayer, total_cash);
    }
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    // (NOTE) ���� Ÿ������ �������� ���θ� �Ǵ��� �ʿ� ����.
    if (0 != total_pp_cash) {
        // [ĳ�����������α�]
        GAMELOG->LogMoney(MONEY_CHARGE_BUY_BY_CASH, pPlayer, total_pp_cash);
    }
#endif
    // [���ſ�û �α�]
    GAMELOG->LogMultiItem(ITEM_CHARGE_BUY_REQUEST, pPlayer,
                          making_item_node->number_of_kinds,
                          making_item_node->item_code_array_,
                          making_item_node->item_number_array_,
                          making_item_node->date_time_array_);
    //=============================================================================================

    // Ʈ����� ����
    shop_user->SetTransaction(true);

    return RC::RC_SHOP_PENDING;
}

//==================================================================================================
//_NA00XXXX_090111_CASH_TYPE_SHARED_
void
GameShopManager::OnBuyResult(Player* player, USERGUID user_guid, CHARGUID char_guid,
                             const CashData& cash_data,
                             DWORD result, BYTE count, tblBuyItemTransaction* const buy_list)
{
    ASSERT(false == IsTransaction());
    //
    static MSG_CG_SHOP_ITEM_BUY_ACK msg_ack;
    ;      MSG_CG_SHOP_ITEM_BUY_NAK msg_nak;
    static MSG_GS_SHOP_BUY_ITEM_NTF msg_ntf;
    ;{
        msg_ack.m_TotalInfo.m_Count = 0;
        msg_ack.m_ExtendItemCode = 0;
        msg_ntf.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey();
        msg_ntf.dwUserGuid = user_guid;
        msg_ntf.dwCharGuid = char_guid;
        msg_ntf.dwCount = count;
    };
    //
#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
    // NOTE: if waverix has a chance, replace a solarauth routine
    static PRICEGUID    s_rPriceGuid[MAX_INVENTORY_SLOT_NUM];
    static SLOTCODE     s_rItemCode[MAX_INVENTORY_SLOT_NUM];
    static BYTE         s_rCategoryID[MAX_INVENTORY_SLOT_NUM];
    static DWORD        s_rPrice[MAX_INVENTORY_SLOT_NUM];
    int                 nReportTotalCount = 0;
#endif //__NA000000_070523_REPORT_TO_SOLARAUTH

    MONEY recover_money=0;
    MONEY recover_total_money=0;
    BOOL  is_extend_date = false; // �Ⱓ���� ������ ������ ��� �ٸ��� ó���Ѵ�.(������ ����ڰ� �Ⱓ���忡 ���ؼ��� ���� ó�� �����شܴ�! -_-+)

    this->OnResultBuy(user_guid, cash_data);

    GameShopUser* const shop_user = static_cast<GameShopUser*>(FindShopUser(user_guid));
    if (shop_user) {
        shop_user->SetTransaction(false);
    }
    //=============================================================================================
    if (player == NULL)
    {
        tblBuyItemTransaction* buy_list_it = buy_list,
                             * const buy_list_end = &buy_list[count];
        // ���� : �÷��̾ �̹� ���� ���� �ߴ�.
        for ( ; buy_list_it != buy_list_end; ++buy_list_it)
        {
            // [���зα�]
            //GAMELOG->LogShopItem(ITEM_CHARGE_BUY_FAILED, player,
            //  buy_list[idx].dwPriceGuid, buy_list[idx].dwTransactionGuid, buy_list[idx].dwResult);
            SUNLOG(eCRITICAL_LOG,
                   "[Shop]PlayerNotFound:price_guid[%u],Trans[%u],Result[%u],User[%d],Char[%u]", 
                   buy_list_it->dwPriceGuid, buy_list_it->dwTransactionGuid, buy_list_it->dwResult,
                   user_guid, char_guid);
            buy_list_it->dwResult = eRET_FAIL;
        }
        // [���зα�]
        goto buy_shop_ntf;
    }
    //=============================================================================================
    // �÷��̾� ���� => �ѹ�
    player->GetItemManager()->RollbackReservedAllKindOfItem(shop_user->locked_info_array_,
                                                            shop_user->GetUsedLockedCount());

    //=============================================================================================
    if (result != eRET_SUCCESS)
    {
        tblBuyItemTransaction* buy_list_it = buy_list,
                             * const buy_list_end = &buy_list[count];
        // ������ ���� ��ü ����
        for ( ; buy_list_it != buy_list_end; ++buy_list_it)
        {
            // [���зα�]
            GAMELOG->LogShopItem(ITEM_CHARGE_BUY_FAILED, player, \
                                 buy_list_it->dwPriceGuid,
                                 buy_list_it->dwTransactionGuid,
                                 buy_list_it->dwResult);
        #ifdef __NA_00387_20070605_SHOP_MILEAGE2
            IsSuccessTransaction(result, msg_nak.m_byErrorCode);
        #else
            msg_nak.m_byErrorCode = RC::RC_SHOP_FAILED;
        #endif
            msg_nak.m_PriceGuid = buy_list_it->dwPriceGuid;
            player->SendPacket(&msg_nak, sizeof(msg_nak));
            buy_list_it->dwResult = eRET_FAIL;
        }
        goto buy_shop_ntf;
    }

    //=============================================================================================
    size_t number_of_price_cache_nodes = 0;
    const InterCache::PriceNode* price_cache_nodes[MAX_BUY_REQ_SIZE];
    ;{
        tblBuyItemTransaction* buy_list_it = buy_list,
                             * const buy_list_end = &buy_list[count];
        //
        for ( ; buy_list_it != buy_list_end && number_of_price_cache_nodes < count; ++buy_list_it)
        {
            ShopItemPrice* const shop_item_price = FindShopItemPrice(buy_list_it->dwPriceGuid);
            bool success = (shop_item_price != NULL);
            if (success)
            {
                RC::eSHOP_RESULT shop_result = inter_cache_->QueryPriceNode(\
                    shop_item_price, &price_cache_nodes[number_of_price_cache_nodes]);
                if (shop_result != RC::RC_SHOP_SUCCESS) {
                    success = false;
                }
            };
            if (success == false)
            {
                // [���зα�]
                GAMELOG->LogShopItem(ITEM_CHARGE_BUY_FAILED, player,
                                     buy_list_it->dwPriceGuid, buy_list_it->dwTransactionGuid,
                                     buy_list_it->dwResult);
                SUNLOG(eCRITICAL_LOG,
                       _T("[Shop]ShopItemPriceIsNotFound:price_guid[%u],User[%d],Char[%u]"),
                       buy_list_it->dwPriceGuid, user_guid, char_guid);
                //
                buy_list_it->dwResult = eRET_NOT_FOUND_ITEM;
                continue;
            };
            //
            const InterCache::PriceNode* const price_node = \
                price_cache_nodes[number_of_price_cache_nodes];
            const bool is_money_type = (price_node->shop_item->GetMoneyType() == MONEY_HIME_TYPE);
            if (is_money_type) {
                recover_total_money += price_node->price;
            }
            //
            if (buy_list_it->dwResult != eRET_SUCCESS)
            {
                // [���зα�]
                GAMELOG->LogShopItem(ITEM_CHARGE_BUY_FAILED, player,
                                     buy_list_it->dwPriceGuid,
                                     buy_list_it->dwTransactionGuid,
                                     buy_list_it->dwResult);
                //
                if (is_money_type) {
                    recover_money += price_node->price;
                }
                //
                msg_nak.m_PriceGuid = buy_list_it->dwPriceGuid;
                BOOL rtt = IsSuccessTransaction(buy_list_it->dwResult, msg_nak.m_byErrorCode);
                ASSERT(false == rtt);
                SUNLOG(eCRITICAL_LOG,
                       _T("[Shop]user_guid[%d]char_guid[%d]MoneyRecover[%u]PriceGuid[%u],Error[%u]"),
                       user_guid, char_guid, price_node->price,
                       buy_list_it->dwPriceGuid, buy_list_it->dwResult);
                player->SendPacket(&msg_nak, sizeof(msg_nak));
                continue;
            }
            //
            assert(success);
            if (success) {
                ++number_of_price_cache_nodes;
            }
        } // end 'for'
    };
    ;{
        for (size_t index = 0; index < number_of_price_cache_nodes; ++index)
        {
            const InterCache::PriceNode* price_node = price_cache_nodes[index];
            if (price_node->is_package == false)
            {
                const InterCache::ItemInfoNode& item_info_node = price_node->shop_item_nodes[0];
                const eITEMTYPE item_type = \
                    static_cast<eITEMTYPE>(item_info_node.item_info->m_wType);
                if (item_type == eITEMTYPE_EXTEND_DATE_WEAPON ||
                    item_type == eITEMTYPE_EXTEND_DATE_ARMOR ||
                    item_type == eITEMTYPE_EXTEND_DATE_RIDER)
                {
                    is_extend_date = true;
                    assert(number_of_price_cache_nodes == 1);
                    break;
                }
            }
        } // end 'for'
    };
    //=============================================================================================
    RC::eITEM_RESULT item_result = RC::RC_ITEM_SUCCESS;
    ItemManager* item_manager = player->GetItemManager();
    InterCache::CustomMakingItemInfoBlock* const making_item_node = \
        inter_cache_->SetupMakingItemInfo(price_cache_nodes, number_of_price_cache_nodes);
    if (number_of_price_cache_nodes == 0)
    {
        item_result = RC::RC_ITEM_FAILED;
    }
    else if (is_extend_date)
    {
        const GameShopUser::ExtendSlotInfo& extend_slot_info = shop_user->extend_slot_info_;
        item_result = item_manager->ExtendDateOfItem(number_of_price_cache_nodes,
                                                     making_item_node->item_code_array_,
                                                     making_item_node->date_time_array_,
                                                     extend_slot_info.slot_index,
                                                     extend_slot_info.slot_pos);
    }
    else {
        // (Ref100525.1L) a reserved replacement (rank info -> item type info)
        item_result = item_manager->ObtainAllKindOfItemForShop(\
            making_item_node->number_of_kinds, shop_user->option_tag_,
            making_item_node->item_code_array_, making_item_node->item_number_array_,
            making_item_node->date_time_array_, making_item_node->enchant_array_,
            //      use KindOfItemType       unused rank
            making_item_node->item_type_array_, NULL,
            &msg_ack.m_TotalInfo);
    }

    if (item_result == RC::RC_ITEM_SUCCESS)
    {
        player->SerializeInfoToDBProxy();
        // [������ ȹ�� ���� �α�]
        GAMELOG->LogMultiItem(ITEM_CHARGE_BUY_SUCCESS, player,
                              making_item_node->number_of_kinds,
                              making_item_node->item_code_array_,
                              making_item_node->item_number_array_,
                              making_item_node->date_time_array_);

        msg_ack.m_UserMoney = player->GetMoney();
        msg_ack.m_CashData = cash_data;
        if (is_extend_date) {
            msg_ack.m_ExtendItemCode = making_item_node->item_code_array_[0];
        }
        player->SendPacket(&msg_ack, msg_ack.GetSize());

    #ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
        for (size_t index = 0; index < number_of_price_cache_nodes; ++index)
        {   // special content, send a purchase history to the solarauth server
            const InterCache::PriceNode* price_node = price_cache_nodes[index];
            if (price_node->shop_item->GetMoneyType() == MONEY_CASH_TYPE)
            {
                s_rPriceGuid[nReportTotalCount] = price_node->price_guid;
                s_rItemCode[nReportTotalCount] = 0;
                s_rCategoryID[nReportTotalCount] = price_node->shop_item->GetCategory();
                s_rPrice[nReportTotalCount] = price_node->price;
                ++nReportTotalCount;
            }
            ReportBuyMultiCashItem(player, nReportTotalCount,
                                   s_rPriceGuid, s_rItemCode, s_rCategoryID, s_rPrice);
        };
    #endif
    }
    else
    {
        // [���зα�]
        tblBuyItemTransaction* buy_list_it = buy_list,
                             * const buy_list_end = &buy_list[count];
        for ( ; buy_list_it != buy_list_end; ++buy_list_it) {
            buy_list_it->dwResult = eRET_FAIL;
        }
        if (recover_total_money != 0)
        {
            // ��������� �ϴ� ���� �ִٸ� ����
            GAMELOG->LogMoney(MONEY_CHARGE_RECOVER_FOR_BUY_BY_HIME, player, recover_total_money);
            player->PlusMoney(recover_total_money);
            recover_money = 0;
        }
    }
    //=============================================================================================
buy_shop_ntf:
    if (0 != recover_money && player)
    {
        // ��������� �ϴ� ���� �ִٸ� ����
        player->PlusMoney(recover_money);
        GAMELOG->LogMoney(MONEY_CHARGE_RECOVER_FOR_BUY_BY_HIME, player, recover_money);
    }
    ;{
        tblBuyItemTransaction* buy_list_it = buy_list,
                             * const buy_list_end = &buy_list[count];
        //// (CHANGES) (100113) (WAVERIX) changes to shortly validation logic
        //util::AssertCompileTimeVariableTypeEqual(msg_ntf.transactions[0], buy_list[0]);
        BOOST_STATIC_ASSERT(_countof(msg_ntf.transactions) == MAX_BUY_REQ_SIZE);
        tblBuyItemTransaction* dest_it = msg_ntf.transactions;
        for ( ; buy_list_it != buy_list_end; ++buy_list_it) {
            *dest_it++ = *buy_list_it;
        }
        g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg_ntf, sizeof(msg_ntf));
    };
}

//==================================================================================================
//_NA00XXXX_090111_CASH_TYPE_SHARED_
RC::eSHOP_RESULT
GameShopManager::OnResultUserCash(USERGUID user_guid, const CashData& cash_data)
{
    ASSERT(false == IsTransaction());

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid);
    if (player != NULL)
    {
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
        bit_block_container_.new_bit_block(user_guid);
#endif

        ShopManager::OnResultUserCash(user_guid, cash_data);

        MSG_CG_SHOP_CASH_REQ_ACK msg;
        msg.cash_info_ = cash_data;
        player->SendPacket(&msg, sizeof(msg));
    }
    else
    {
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
        bit_block_container_.delete_bit_block(user_guid);
#endif
        DISPMSG("Player(%u),Money(%u)IsDisconnected\n", user_guid, cash_data.cash_);
        return RC::RC_SHOP_FAILED;
    }
    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::ValidShopUser(USERGUID user_guid)
{
    GameShopUser* shop_user = (GameShopUser*)FindShopUser(user_guid);
    if (!shop_user)
    {
        return RC::RC_SHOP_USERISNOTFOUND;
    }
    if (shop_user->IsTransaction())
    {
        return RC::RC_SHOP_ALREADY_BUYING_TRANSACTION;
    }

    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__

RC::eSHOP_RESULT
GameShopManager::OnSendGiftNDoRequest(Player* pPlayer,
                                      BYTE item_type, int price_guid, BOOLEAN purchase_pp_cash,
                                      const QueryUserIdInfo& queried_info,
                                      const char* gift_message)
{
    BOOST_STATIC_ASSERT(sizeof(char) == sizeof(char));

    USERGUID user_guid = pPlayer->GetUserGuid();

#ifdef _US_SHOP_GIFT_ERRORCONTROL_ADD
    if ( pPlayer->GetLevel() < GIFT_REQ_USERLEVEL )
        return RC::RC_SHOP_USERLEVEL_LOW;
#endif//_US_SHOP_GIFT_ERRORCONTROL_ADD

    RC::eSHOP_RESULT rt = ValidShopUser(user_guid);
    if (rt != RC::RC_SHOP_SUCCESS)
        return rt;

    if (!FlowControl::FCAssert(queried_info.char_name_[0] != '\0'))
        return RC::RC_SHOP_CANNOT_FIND_CHARNAME;

    //ĳ���� ���ٴ� GUID�� ������
    //if (0 == strncmp(pPlayer->GetCharName(), queried_info.char_name_, MAX_CHARNAME_LENGTH))
    if (queried_info.char_guid_ == pPlayer->GetCharGuid())
        return RC::RC_SHOP_CANNOT_SEND_GIFT_MYSELF;

    GameShopUser* shop_user = static_cast<GameShopUser *>(FindShopUser(user_guid));
    if (shop_user->IsTransaction())
        return RC::RC_SHOP_ALREADY_BUYING_TRANSACTION;
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    if (purchase_pp_cash == false) {
        return RC::RC_SHOP_CAN_PRESENT_PP_CASH_ONLY;
    }
#endif

    MONEY total_money = 0;
    CASH total_cash = 0;
    ShopItemPrice* pShopItemPrice = FindShopItemPrice(price_guid);
    if (!(pShopItemPrice && pShopItemPrice->GetShopItem()))
        return RC::RC_SHOP_CANNOTBUYITEMPRICE;

    ShopItem* const pShopItem = pShopItemPrice->GetShopItem();
    if (false == CanEnablePresent(pShopItem))
        return RC::RC_SHOP_CANNOT_PRESENT_ITEM;

    if (pShopItem->GetMoneyType() == MONEY_HIME_TYPE)
        total_money += pShopItemPrice->GetPrice();
    else
        total_cash += pShopItemPrice->GetPrice();

    if (!pPlayer->CanMinusMoney(total_money))
        return RC::RC_SHOP_NOTENOUGHMONEY;
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    assert(purchase_pp_cash != false);
    if (!HavePPCash(user_guid, total_cash)) {
        return RC::RC_SHOP_NOTENOUGHCASH;
    }
#else
    assert(purchase_pp_cash == false);
    if (!HaveCash(user_guid, total_cash)) {
        return RC::RC_SHOP_NOTENOUGHCASH;
    }
#endif

    //OnSend
    char __msg_block[sizeof(MSG_GS_SHOP_SEND_GIFT_SYN)] = { 0, };
    MSG_GS_SHOP_SEND_GIFT_SYN& msgSYN = *new (__msg_block) MSG_GS_SHOP_SEND_GIFT_SYN;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msgSYN.gamechu_usn_ = pPlayer->GetGameChuUsn();
    strncpy(msgSYN.ip_address_, pPlayer->GetUserIP(), _countof(msgSYN.ip_address_));
    msgSYN.ip_address_[_countof(msgSYN.ip_address_) - 1] = '\0';
#endif
    msgSYN.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey();    //< ���� ��û ���� Guid
    msgSYN.dwUserGuid = pPlayer->GetUserGuid(); //< ����� Guid
    //
    strncpy(msgSYN.user_id, pPlayer->GetUserID(), _countof(msgSYN.user_id));
    msgSYN.user_id[_countof(msgSYN.user_id) - 1] = '\0';
    msgSYN.char_class = pPlayer->GetCharType();
    msgSYN.char_level = pPlayer->GetLevel();
    //
    msgSYN.dwCharGuid = pPlayer->GetCharGuid(); //< ĳ���� Guid
    strncpy(msgSYN.szCharName, pPlayer->GetCharName(), _countof(msgSYN.szCharName)); //< ĳ���� �̸�
    msgSYN.szCharName[_countof(msgSYN.szCharName) - 1] = '\0';

    // ���� ���� ����� ����
    msgSYN.dwToServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ������ ���� ���� Guid
    msgSYN.dwToUserGuid = queried_info.user_guid_;  //< ������ ���� ����� Guid
    // (BUGFIX) (WAVERIX) fix up send gift functionality because expect 'receiver user id', but send 'sender user id'
    strncpy(msgSYN.to_user_id, queried_info.user_id_, _countof(msgSYN.to_user_id));
    msgSYN.to_user_id[_countof(msgSYN.to_user_id) - 1] = '\0';
    msgSYN.to_char_class = queried_info.char_class_;
    msgSYN.to_char_level = queried_info.char_level_;
    //
    msgSYN.dwToCharGuid = queried_info.char_guid_;  //< ������ ���� ĳ���� Guid 

    strncpy(msgSYN.szToCharName, queried_info.char_name_, _countof(msgSYN.szToCharName));    //< ������ ���� ĳ���� �̸�
    msgSYN.szToCharName[_countof(msgSYN.szToCharName) - 1] = '\0';

    strncpy(msgSYN.szGiftMessage, gift_message, _countof(msgSYN.szGiftMessage));
    msgSYN.szGiftMessage[_countof(msgSYN.szGiftMessage) - 1] = '\0';

#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    msgSYN.buy_cash_type = purchase_pp_cash == false
                         ?  msgSYN.PURCHASE_DEFAULT
                         :  msgSYN.PURCHASE_PP_CASH;
#endif
    msgSYN.dwPriceGuid = price_guid;

    ShopUser::TransactionInfo* trsansaction_info = shop_user->GetTransactionInfo();
    // (BUGFIX) (WAVERIX) changes sender key to receiver key: need target user's guids
    trsansaction_info->SetToUserGuid(queried_info.user_guid_);
    trsansaction_info->SetToCharGuid(queried_info.char_guid_);

    g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msgSYN, sizeof(msgSYN));
    shop_user->SetTransaction(true); //Start Transaction - 'present'

    pPlayer->MinusMoney(total_money);
    GAMELOG->LogMoney(MONEY_CHARGE_GIFT_BY_HIME, pPlayer, total_money);

    return RC::RC_SHOP_SUCCESS;
}

#else //!__NA000000_SHOP_USERID_50_PASSING_PROC__

RC::eSHOP_RESULT
GameShopManager::CanSendGiftNDoRequest(Player* pPlayer, BYTE ItemType,
                                       int price_guid, const char* tszCharName,
                                       const char* pszGiftMessage)
{
    USERGUID user_guid = pPlayer->GetUserGuid();

    RC::eSHOP_RESULT rt = ValidShopUser(user_guid);
    if (rt != RC::RC_SHOP_SUCCESS)
        return rt;

    if (0 == _tcsncmp(pPlayer->GetCharName(), tszCharName, MAX_CHARNAME_LENGTH))
    {
        return RC::RC_SHOP_CANNOT_SEND_GIFT_MYSELF;
    }

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(user_guid);

    MONEY TotalMoney = 0;
    CASH TotalCash = 0;
    ShopItemPrice* pShopItemPrice = FindShopItemPrice(price_guid);
    if (!(pShopItemPrice && pShopItemPrice->GetShopItem()))
        return RC::RC_SHOP_CANNOTBUYITEMPRICE;

    ShopItem* const pShopItem = pShopItemPrice->GetShopItem();
    if (false == CanEnablePresent(pShopItem))
        return RC::RC_SHOP_CANNOT_PRESENT_ITEM;

    if (pShopItem->GetMoneyType() == MONEY_HIME_TYPE)
        TotalMoney += pShopItemPrice->GetPrice();
    else
        TotalCash += pShopItemPrice->GetPrice();
    if (!pPlayer->CanMinusMoney(TotalMoney))
        return RC::RC_SHOP_NOTENOUGHMONEY;
    if (!HaveCash(user_guid, TotalCash))
        return RC::RC_SHOP_NOTENOUGHCASH;

    MSG_DG_OTHER_EXIST_CHARNAME_SYN msg;
    msg.m_dwKey = pPlayer->GetUserKey();
    msg.m_Type = eCHARNAME_ACTION_SHOP_SEND_GIFT;
    strncpy(msg.m_tszCharName, tszCharName, MAX_CHARNAME_LENGTH+1);
    msg.m_tszCharName[MAX_CHARNAME_LENGTH]='\0';

    if (!g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg)))
    {
        return RC::RC_SHOP_DB_ERROR;
    }

    shop_user->GetTransactionInfo()->SetToGiftMessage(pszGiftMessage);
    shop_user->GetTransactionInfo()->SetPriceGuid(price_guid);
    shop_user->SetTransaction(true);

    return RC::RC_SHOP_SUCCESS;
}

RC::eSHOP_RESULT
GameShopManager::OnSendGiftNDoRequest(BOOL bSuccessToFoundCharName, Player* pPlayer,
                                      USERGUID ToUserGuid, CHARGUID ToCharGuid,
                                      const char* tszToCharName)
{
    if (!pPlayer) return RC::RC_SHOP_USERISNOTFOUND;

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(pPlayer->GetUserGuid());
    if (!shop_user)
        return RC::RC_SHOP_USERISNOTFOUND;

    shop_user->SetTransaction(false);

    if (!bSuccessToFoundCharName)
        return RC::RC_SHOP_CANNOT_FIND_CHARNAME;

    PRICEGUID price_guid = shop_user->GetTransactionInfo()->GetPriceGuid();
    USERGUID user_guid = pPlayer->GetUserGuid();

    MONEY TotalMoney = 0;
    CASH TotalCash = 0;
    ShopItemPrice* pShopItemPrice = FindShopItemPrice(price_guid);
    if (!(pShopItemPrice && pShopItemPrice->GetShopItem()))
        return RC::RC_SHOP_CANNOTBUYITEMPRICE;

    if (pShopItemPrice->GetShopItem()->GetMoneyType() == MONEY_HIME_TYPE)
        TotalMoney += pShopItemPrice->GetPrice();
    else
        TotalCash += pShopItemPrice->GetPrice();

    if (!pPlayer->CanMinusMoney(TotalMoney))
        return RC::RC_SHOP_NOTENOUGHMONEY;
    if (!HaveCash(user_guid, TotalCash))
        return RC::RC_SHOP_NOTENOUGHCASH;

    static MSG_GS_SHOP_SEND_GIFT_SYN msg;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = pPlayer->GetGameChuUsn();
    strncpy(msg.ip_address_, pPlayer->GetUserIP(), _countof(msg.ip_address_));
    msg.ip_address_[_countof(msg.ip_address_) - 1] = '\0';
#endif
    
    msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ���� ��û ���� Guid
    msg.dwUserGuid = pPlayer->GetUserGuid(); //< ����� Guid
    msg.dwCharGuid = pPlayer->GetCharGuid(); //< ĳ���� Guid
    strncpy(msg.szCharName, pPlayer->GetCharName(), MAX_CHARACTER_NAME_LENGTH); //< ĳ���� �̸�

    // ���� ���� ����� ����
    msg.dwToServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ������ ���� ���� Guid
    msg.dwToUserGuid = ToUserGuid; //< ������ ���� ����� Guid
    msg.dwToCharGuid = ToCharGuid; //< ������ ���� ĳ���� Guid 
    strncpy(msg.szToCharName, tszToCharName, MAX_CHARACTER_NAME_LENGTH); //< ������ ���� ĳ���� �̸�
    strncpy(msg.szGiftMessage, shop_user->GetTransactionInfo()->GetToGiftMessage(), MAX_GIFT_MESSAGE_LENGTH); //< ���� �޼���
    msg.dwPriceGuid = shop_user->GetTransactionInfo()->GetPriceGuid(); //< �Ǹ� ���� ���� ��ȣ
    shop_user->GetTransactionInfo()->SetToUserGuid(ToUserGuid);
    shop_user->GetTransactionInfo()->SetToCharGuid(ToCharGuid);

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;

    pPlayer->MinusMoney(TotalMoney);
    GAMELOG->LogMoney(MONEY_CHARGE_GIFT_BY_HIME, pPlayer, TotalMoney);
    shop_user->SetTransaction(true);

    return RC::RC_SHOP_SUCCESS;
}
#endif //!__NA000000_SHOP_USERID_50_PASSING_PROC__

//==================================================================================================

void
GameShopManager::OnSendGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                            const CashData& cash_data, PRICEGUID price_guid, DWORD dwResult)
{
    if (!pPlayer) 
    {
        // [��α�:����]
        SUNLOG(eCRITICAL_LOG, "[U:%d,C:%d]OnSendGift(%u) failed, pPlayer is NULL:%u",
               user_guid, char_guid, price_guid, dwResult);
        return;
    }

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(pPlayer->GetUserGuid());
    ASSERT(shop_user);
    if (!shop_user)
    {
        // [��α�:����]
        // [���ӷα�:����]
        GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_FAILED, pPlayer,
                             price_guid, 0, dwResult);

        SUNLOG(eCRITICAL_LOG, "[U:%d,C:%d]OnSendGift(%u) failed, shop_user is NULL:%u",
               user_guid, char_guid, price_guid, dwResult);     
        return;
    }

    shop_user->SetTransaction(false);

    MONEY TotalMoney = 0;
    ShopItemPrice* pShopItemPrice = FindShopItemPrice(price_guid);
    if (!pShopItemPrice || !pShopItemPrice->GetShopItem())
    {
        // [��α�:����]
        SUNLOG(eCRITICAL_LOG, "[U:%d,C:%d]ShopItemPrice[%u] is not find,Result:%u",
               user_guid, char_guid, price_guid, dwResult);
    }
    else
    {
        if (pShopItemPrice->GetShopItem()->GetMoneyType() == MONEY_HIME_TYPE)
            TotalMoney += pShopItemPrice->GetPrice();
    }

    MSG_CG_SHOP_SEND_GIFT_NAK nmsg;
    if (IsSuccessTransaction(dwResult, nmsg.m_byErrorCode))
    {
        // ���߿� ���������� ���յǸ� pp_cash�� �������� �ϰ� ó���ؾ� �Ѵ�.
        shop_user->SetCashData(cash_data);
        //// (100112) current setting, ref #100112
        //shop_user->SetUserCash(cash_data.cash_);
        //shop_user->SetUserMileage(cash_data.mileage_); //__CN_0_20070507_SHOP_MILEAGE

        MSG_CG_SHOP_SEND_GIFT_ACK msg;
        msg.m_CashData = cash_data;
        //msg.m_CashData.pp_cash_ = shop_user->GetUserPPCash(); // #100112
        msg.m_UserMoney = pPlayer->GetMoney();
        pPlayer->SendPacket(&msg, sizeof(msg));

        MSG_DG_OTHER_SHOP_RECV_GIFT_NTF ntf;
        ntf.m_UserGuid = shop_user->GetTransactionInfo()->GetToUserGuid();
        ntf.m_CharGuid = shop_user->GetTransactionInfo()->GetToCharGuid();
        g_pGameServer->SendToServer(GAME_DBPROXY, &ntf, sizeof(ntf));
        // [���ӷα�:����]
        GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_SUCCESS, pPlayer,
                             price_guid, 0, dwResult);

    #ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
        if (pShopItemPrice->GetShopItem()->GetMoneyType() == MONEY_CASH_TYPE)
        {
            WORD wItemCode = 0;
            if (pShopItemPrice->GetShopItem()->GetType() == ShopItem::SINGLE)
                wItemCode = ((SingleItem*)pShopItemPrice->GetShopItem())->GetItemCode();

            ReportBuyCashItem(pPlayer, pShopItemPrice->GetPriceGuid(), wItemCode, 
                              pShopItemPrice->GetShopItem()->GetCategory(),
                              pShopItemPrice->GetPrice());
        }
    #endif //__NA000000_070523_REPORT_TO_SOLARAUTH
    }
    else
    {
        if (0 != TotalMoney) 
        {
            pPlayer->PlusMoney(TotalMoney);
            GAMELOG->LogMoney(MONEY_CHARGE_RECOVER_FOR_GIFT_BY_HIME, pPlayer, TotalMoney);
        }
        pPlayer->SendPacket(&nmsg, sizeof(nmsg));

        // [���ӷα�:����]
        GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_FAILED, pPlayer,
                             price_guid, 0, dwResult);
    }
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::CanActionToPresent(PresentRecvBox::PresentInfo* pPresent, BYTE ActionType)
{
    ShopItemPrice* pPrice = FindShopItemPrice(pPresent->GetPriceGuid());
    if (!pPrice || !pPrice->GetShopItem())
    {
        return RC::RC_SHOP_CANNOTBUYITEM;
    }

    switch(ActionType)
    {
        //  case eGS_WAIT:          //< ���
    case eGS_ACCEPT:            //< ���� �ޱ�
        if (pPresent->GetState() != eGS_WAIT)
            return RC::RC_SHOP_CANNOT_OBTAIN_GIFT;
        break;
    case eGS_REJECT:            //< ���� ����
        if (pPresent->GetState() != eGS_WAIT)
            return RC::RC_SHOP_CANNOT_REJECT_GIFT;
        break;
    case eGS_DELETE:            //< ���� ��� ����
        if (pPresent->GetState() != eGS_ACCEPT)
            return RC::RC_SHOP_CANNOT_DISCARD_GIFT;
        break;
    default:
        return RC::RC_SHOP_INVALID_GIFT_GUID;
    }

    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================

void
GameShopManager::GetItemInfosForPriceUnit(ShopItemPrice* pPrice, int& number_of_kinds_out OUT,
                                          SLOTCODE* pItemCode OUT, POSTYPE* pItemNum OUT,
                                          DWORD* pDateTime OUT, BYTE* pEnchant OUT, BYTE* pRank OUT)
{
    number_of_kinds_out = 0;
    if (pPrice->GetShopItem()->GetType() == ShopItem::SINGLE)
    {
        // ��Ű���� �������̳� ������ �޶���
        pItemCode[number_of_kinds_out] = ((SingleItem*)pPrice->GetShopItem())->GetItemCode();
        pItemNum[number_of_kinds_out] = (pPrice->GetAmount()==0?1:pPrice->GetAmount());
        pDateTime[number_of_kinds_out] = (pPrice->GetUseTime()*60); //< ��->�ʴ����� ����
        pEnchant[number_of_kinds_out] = ((SingleItem*)pPrice->GetShopItem())->GetEnchant();
        pRank[number_of_kinds_out] = ((SingleItem*)pPrice->GetShopItem())->GetRank();
        ++number_of_kinds_out;
    }
    else
    {
        PackageItem* package_item = (PackageItem*)pPrice->GetShopItem();
        for (BYTE piece_idx = 0 ; piece_idx < package_item->GetPieceCount() ; ++piece_idx)
        {
            const PackageItem::ItemPiece* piece = package_item->GetItemPiece(piece_idx);
            pItemCode[number_of_kinds_out] = piece->ItemCode;
            pItemNum[number_of_kinds_out] = piece->Amount == 0 ? 1 : piece->Amount;
            pDateTime[number_of_kinds_out] = piece->UseTime * 60; //< ��->�ʴ����� ����
            pEnchant[number_of_kinds_out] = piece->Enchant;
            pRank[number_of_kinds_out] = piece->Rank;
            ++number_of_kinds_out;
        }
    }
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::CanActionRecvGiftNDoRequest(Player* pPlayer, BYTE ActionType,
                                             int gift_guid, const char* const tszThanksMessage)
{
    RC::eSHOP_RESULT rt = ValidShopUser(pPlayer->GetUserGuid());
    if (rt != RC::RC_SHOP_SUCCESS)
        return rt;

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(pPlayer->GetUserGuid());
    PresentRecvBox::PresentInfo* pPresent = shop_user->GetPresentRecvBox()->FindPresent(gift_guid);
    if (!pPresent)
        return RC::RC_SHOP_INVALID_GIFT_GUID;

    if (ActionType ==  eGS_REJECT)
    {
        if (strncmp(pPresent->GetSendCharName(), pPlayer->GetCharName(), MAX_CHARNAME_LENGTH) == 0) 
            return RC::RC_SHOP_CANNOT_REJECT_GIFT_MYSELF;
    }

    rt = CanActionToPresent(pPresent, ActionType);
    if (rt != RC::RC_SHOP_SUCCESS)
        return rt;

    int KindNum = 0;
    static SLOTCODE s_ItemCode[MAX_INVENTORY_SLOT_NUM];
    static POSTYPE  s_ItemNum[MAX_INVENTORY_SLOT_NUM];
    static DWORD    s_DateTime[MAX_INVENTORY_SLOT_NUM];
    static BYTE     s_Enchant[MAX_INVENTORY_SLOT_NUM];
    static BYTE     s_Rank[MAX_INVENTORY_SLOT_NUM];

    if (ActionType ==  eGS_ACCEPT)
    {
        ShopItemPrice* pPrice = FindShopItemPrice(pPresent->GetPriceGuid());

        GetItemInfosForPriceUnit(pPrice, KindNum, s_ItemCode, s_ItemNum, s_DateTime, s_Enchant, s_Rank);

        // =====================================================================================
        ItemManager* pManager = pPlayer->GetItemManager();
        // (Ref100525.1L) a reserved replacement (rank info -> item type info)
        if (RC::RC_ITEM_SUCCESS != pManager->ReserveSpaceAllKindOfItem(KindNum,
                s_ItemCode, s_ItemNum, s_DateTime, s_Enchant, s_Rank, NULL,
                GameShopUser::MAX_LOCKEDINFO_NUM,
                shop_user->locked_info_array_, &shop_user->number_of_used_locked_infos_))
        {
            return RC::RC_SHOP_NOTENOUGHINVENTORYSPACE;
        }
        // =========================================================================================================
    }

    static MSG_GS_SHOP_ACTION_RECV_GIFT_SYN msg;
    msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ���� GUID
    msg.dwUserGuid = pPlayer->GetUserGuid(); //< ����� GUID
    msg.dwCharGuid = pPlayer->GetCharGuid(); //< ĳ���� GUID
    msg.dwGiftItemID = gift_guid; //< ���� ID
    msg.dwAction = ActionType; //< ���� ó�� ��� (1:���� �ޱ�, 2:���� ������, 3:��Ͽ��� ���� ����) ���� : eGIFT_ACTION
    strncpy(msg.szMessage, tszThanksMessage, MAX_GIFT_MESSAGE_LENGTH);

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        if (ActionType ==  eGS_ACCEPT)
        {
            ItemManager* pManager = pPlayer->GetItemManager();
            // ���н� �ѹ�
            pManager->RollbackReservedAllKindOfItem(shop_user->locked_info_array_,
                                                    shop_user->GetUsedLockedCount());
        }
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;
    }

    shop_user->SetTransaction(true);

    return RC::RC_SHOP_SUCCESS;
}

void
GameShopManager::OnActionRecvGift(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                                  GIFTGUID gift_guid, BYTE ActionType, DWORD dwResult)
{
    BOOST_STATIC_ASSERT(eGS_ACCEPT == MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::OBTAIN);
    BOOST_STATIC_ASSERT(eGS_REJECT == MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::REJECT);
    BOOST_STATIC_ASSERT(eGS_DELETE == MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::DISCARD);

    static MSG_CG_SHOP_ACTION_RECV_GIFT_ACK amsg;
    ;      MSG_CG_SHOP_ACTION_RECV_GIFT_NAK nmsg;
    ;      MSG_GS_SHOP_ACTION_RECV_GIFT_NTF ntf_msg;
    amsg.m_ActionType = ActionType;
    ntf_msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey();
    ntf_msg.dwUserGuid = user_guid;
    ntf_msg.dwCharGuid = char_guid;
    ntf_msg.dwGiftItemID = gift_guid;
    ntf_msg.dwResult = 0;           //< ���� : 0, ���� : 1

    if (!pPlayer || pPlayer->GetUserGuid() != user_guid)
    {
        // ����
        goto send_nft_to_shopserver;
    }

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(user_guid);
    ASSERT(shop_user);
    if (!shop_user) 
    {
        // ����
        nmsg.m_byErrorCode = RC::RC_SHOP_USERISNOTFOUND;
        pPlayer->SendPacket(&nmsg, sizeof(nmsg));
        goto send_nft_to_shopserver;
    }

    shop_user->SetTransaction(false);

    if (!IsSuccessTransaction(dwResult, nmsg.m_byErrorCode))
    {
        // ����
        // [���а��ӷα�]
        pPlayer->SendPacket(&nmsg, sizeof(nmsg));
        goto send_nft_to_shopserver;
    }

    PresentRecvBox::PresentInfo* pPresent = shop_user->GetPresentRecvBox()->FindPresent(gift_guid);
    if (!pPresent) 
    {
        //����
        nmsg.m_byErrorCode = RC::RC_SHOP_CANNOTBUYITEM;
        pPlayer->SendPacket(&nmsg, sizeof(nmsg));
        goto send_nft_to_shopserver;
    }

    RC::eSHOP_RESULT rt = CanActionToPresent(pPresent, ActionType);
    if (rt != RC::RC_SHOP_SUCCESS)
    {
        nmsg.m_byErrorCode = rt;
        // ����
        goto send_nft_to_shopserver;
    }

    if (ActionType == eGS_ACCEPT)
    {
        pPresent->SetState(PresentRecvBox::PresentInfo::ePS_ACCEPT);

        ItemManager* pManager = pPlayer->GetItemManager();
#ifdef _GLOBAL
        // ��Ģ�� P���θ� �����ϱⰡ �����ϴ�.
        KindOfItemTag option_tag = shop_user->option_tag_;
        option_tag.value = KindOfItemTag::Tag_PCoinPurchase;
#else
        const KindOfItemTag& option_tag = shop_user->option_tag_;
#endif
        pManager->ObtainReservedAllKindOfItem(option_tag, \
            shop_user->locked_info_array_, shop_user->GetUsedLockedCount(), &amsg.m_TotalInfo);

        pPlayer->SerializeInfoToDBProxy();

        // [�������ӷα�]
        ntf_msg.dwResult = 1;

        GAMELOG->LogShop_RecvItem(ITEM_CHARGE_ACTION_RECV_GIFT_SUCCESS, pPlayer, gift_guid, dwResult);
    }
    else
    {
        pPresent->SetState(ActionType);
        // ��� ����
        shop_user->GetPresentRecvBox()->ErasePresent(gift_guid);
        // ����
        ntf_msg.dwResult = 1;
    }

    pPlayer->SendPacket(&amsg, amsg.GetSize());

send_nft_to_shopserver:
    if (ntf_msg.dwResult == 0)
    {
        if (ActionType == eGS_ACCEPT && pPlayer)
        {
            // ������ �������� ��, ���г� ���, ���ӷα� ����
            GAMELOG->LogShop_RecvItem(ITEM_CHARGE_ACTION_RECV_GIFT_FAILED,
                                      pPlayer, gift_guid, dwResult);
            SUNLOG(eCRITICAL_LOG, "OnActionRecvGift(%u):Cant Accept Item,U:%d,C:%d,G:%d",
                   nmsg.m_byErrorCode, user_guid, char_guid, gift_guid);
        }
    }
    g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&ntf_msg, sizeof(ntf_msg));
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::RequestRecvGiftList(Player* pPlayer)
{
    RC::eSHOP_RESULT rt = ValidShopUser(pPlayer->GetUserGuid());
    if (rt != RC::RC_SHOP_SUCCESS)
        return rt;

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(pPlayer->GetUserGuid());

    MSG_GS_SHOP_RECV_GIFT_LIST_SYN msg;
    msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ���� GUID
    msg.dwUserGuid = pPlayer->GetUserGuid(); //< ����� GUID
    msg.dwCharGuid = pPlayer->GetCharGuid(); //< ĳ���� GUID
    msg.dwMaxCount = 0; //< �ִ� ��� ����(0 : �⺻, N : N���� �ִ� ���)
    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;

    shop_user->SetTransaction(true);

    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================

void
GameShopManager::OnRecvGiftList(Player* pPlayer, USERGUID user_guid, CHARGUID char_guid,
                                DWORD count, tblRecvGiftItem* pGiftList)
{
    if (!pPlayer)
        return;

    GameShopUser* shop_user = (GameShopUser *)FindShopUser(user_guid);
    ASSERT(shop_user);
    if (!shop_user)
        return;

    // ���� ���� ���� ����� �ִ� ����(32)�� �ʰ��� ��� �������κ��� 
    // ������ 32���� �߶� ������ �´�.
    // Ŭ���̾�Ʈ�� ������ �ѹ� ��û�� ���� ������ �ϳ��� �����Ѵ�.
    // �� ������ ���� ���� ��� ���� ���� 32���� �����Ѵ�.
    if (shop_user->IsTransaction() == false)
    {
        return;
    }
    shop_user->SetTransaction(false);

    shop_user->GetPresentRecvBox()->SetList(*this, count, pGiftList);

    MSG_CG_SHOP_RECV_GIFT_LIST_ACK msg;
    ArchiveOfStream lArchive(msg.m_pBUFFER,msg.MAX_BUFFER_SIZE);
    shop_user->GetPresentRecvBox()->Serialize(lArchive, eSHOP_SERIALIZE_LOAD);
    msg.m_Size = lArchive.GetSize();
    pPlayer->SendPacket(&msg, msg.GetSize());
}

//==================================================================================================

RC::eSHOP_RESULT
GameShopManager::RequestRecvGiftCount(Player* pPlayer)
{
    static MSG_GS_SHOP_IS_RECV_GIFT_SYN msg;
    msg.dwServerGuid = g_pGameServer->GetKey().GetOldServerKey(); //< ���� GUID
    msg.dwUserGuid = pPlayer->GetUserGuid(); //< ����� GUID
    msg.dwCharGuid = pPlayer->GetCharGuid(); //< ĳ���� GUID
    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
        return RC::RC_SHOP_SHOPSERVERDISCONNECTED;

    return RC::RC_SHOP_SUCCESS;
}

//==================================================================================================

void
GameShopManager::OnRecvGiftListCount(Player* pPlayer, DWORD dwCount)
{
    if (!pPlayer)
        return;

    MSG_CG_SHOP_IS_RECV_GIFT_ACK msg;
    msg.m_GiftCount = dwCount;
    pPlayer->SendPacket(&msg, sizeof(msg));
}

//==================================================================================================

#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH

void
GameShopManager::ReportBuyMultiCashItem(Player* pPlayer, DWORD dwItemCount,
                                        PRICEGUID* pPriceGuid, SLOTCODE* pItemCode, BYTE* pCategory,
                                        DWORD* pPrice)
{
    if (dwItemCount == 0 || !pPlayer || !pPriceGuid || !pItemCode || !pCategory || !pPrice)
        return;

    MSG_GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF  ntf;
    ntf.m_INFO.dwCharGuid = pPlayer->GetCharGuid();
    ntf.m_INFO.dwUserGuid = pPlayer->GetUserGuid();
    _tcsncpy(ntf.m_INFO.szUserID, pPlayer->GetUserID(), ID_MAX_LEN);
    ntf.m_INFO.szUserID[ID_MAX_LEN] = '\0';

    for (DWORD dwIdx = 0; dwIdx < dwItemCount; dwIdx++)
    {
        ntf.m_INFO.dwPriceGuid = pPriceGuid[dwIdx];
        ntf.m_INFO.wItemCode = pItemCode[dwIdx];
        ntf.m_INFO.byCategoryID = pCategory[dwIdx];
        ntf.m_INFO.dwPriceGuid = pPrice[dwIdx];
        g_pGameServer->SendToServer(MASTER_SERVER, &ntf, sizeof(ntf));
    } 

}

void
GameShopManager::ReportBuyCashItem(Player* pPlayer, PRICEGUID nPriceGuid,
                                   SLOTCODE wItemCode, BYTE byCategory, DWORD dwPrice)
{
    if (!pPlayer)
        return;

    MSG_GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF  ntf;
    ntf.m_INFO.dwCharGuid = pPlayer->GetCharGuid();
    ntf.m_INFO.dwUserGuid = pPlayer->GetUserGuid();
    _tcsncpy(ntf.m_INFO.szUserID, pPlayer->GetUserID(), ID_MAX_LEN);
    ntf.m_INFO.szUserID[ID_MAX_LEN] = '\0';
    ntf.m_INFO.dwPriceGuid = nPriceGuid;
    ntf.m_INFO.wItemCode = wItemCode;
    ntf.m_INFO.byCategoryID = byCategory;
    ntf.m_INFO.dwPriceGuid = dwPrice;
    g_pGameServer->SendToServer(MASTER_SERVER, &ntf, sizeof(ntf));
}

#endif //__NA000000_070523_REPORT_TO_SOLARAUTH

//==================================================================================================

void
GameShopManager::ApplyExtendOption(DWORD count, tblItemExtendOption* pOption IN)
{
    if (count > MAX_ITEM_EXTEND_OPTION_SIZE)   
    {
        SUNLOG(eCRITICAL_LOG,
               "[ApplyExtendOption] ItemOptionCount[%d] is over! MaxCount[%d] ShopServerError!",
               count, MAX_ITEM_EXTEND_OPTION_SIZE);
        return;
    }

    for (DWORD i = 0 ; i < count ; ++i)
    {
        // NOTE: temporary setting. added rule by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        SUNLOG(eCRITICAL_LOG, _T("SkippedExtendOption|Code=%d|Option1=%d|Option2=%d|"),
               pOption[i].dwItemGuid, pOption[i].dwItemOptions[0], pOption[i].dwItemOptions[1]);
    }
}

//==================================================================================================

void
GameShopManager::ReceiveSignal(eSHOP_MANAGER_SIGNAL eSignal, void *pMessage)
{
    // declared by __NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION
    // ������ ����Ʈ�� �ް� �� �� 5�ʵ��� ����Ʈ�� �� ������ ���
    m_dwWaitItemListTick[eSignal] = GetTickCount() + 5000;
    //
    switch(eSignal)
    {
        ///////////////////////////////////////////////�Ϲ� ������ ����Ʈ///////////////////////////////////////////////////////
    case eSHOP_SIGNAL_RECEIVE_ITEM_LIST:
        {           
            MSG_GS_SHOP_ITEM_LIST_REQ_ACK* pMsg = (MSG_GS_SHOP_ITEM_LIST_REQ_ACK*)pMessage;
        
        #ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
            GameShop::ShopItemBuilder builder(&g_GameShopManager);
            builder.BuildShopItem(pMsg->sellItems, pMsg->dwItemCount);
        #else
            g_GameShopManager.BuildShopItem(pMsg->dwItemCount, pMsg->sellItems);
        #endif
        
            SUNLOG(eCRITICAL_LOG, "[MSG_GS_SHOP_ITEM_LIST_REQ_ACK]ItemNum:%u", pMsg->dwItemCount);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////��� ������ Ȯ��////////////////////////////////////////////////////////
    case eSHOP_SIGNAL_RECEIVE_EXTEND_OPTION_LIST:
        {           
            MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK* pMsg = (MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK*)pMessage;

            g_GameShopManager.ApplyExtendOption(pMsg->dwItemOptionCount, pMsg->itemExtendOptions);
            SUNLOG(eCRITICAL_LOG, "[GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK]ItemOptionCount:%u",
                   pMsg->dwItemOptionCount);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////// ���ϸ��� /////////////////////////////////////////////////////////
    case eSHOP_SIGNAL_RECEIVE_MILEAGE_LIST:
        {   
    #ifdef __NA_00387_20070605_SHOP_MILEAGE2
            MSG_GS_SHOP_MILEAGE_LIST_REQ_ACK* pMsg = (MSG_GS_SHOP_MILEAGE_LIST_REQ_ACK *)pMessage;

        #ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
            GameShop::ShopItemBuilder builder(&g_GameShopManager);
            builder.BuildShopItem(pMsg->sellItems, pMsg->dwItemCount);
        #else
            g_GameShopManager.BuildShopItem(pMsg->dwItemCount, pMsg->sellItems);
        #endif
        
            SUNLOG(eCRITICAL_LOG, "[MSG_GS_SHOP_MILEAGE_LIST_REQ_ACK]ItemNum:%u", pMsg->dwItemCount);
    #endif
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case eSHOP_SIGNAL_RECEIVE_PACKAGE_LIST:
        {           
            MSG_GS_SHOP_PACK_LIST_REQ_ACK* pMsg = (MSG_GS_SHOP_PACK_LIST_REQ_ACK*)pMessage;

        #ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
            GameShop::ShopItemBuilder builder(&g_GameShopManager);
            builder.BuildShopItem(pMsg->sellPackages, pMsg->dwPackageCount);
        #else
            g_GameShopManager.BuildPackageItem(pMsg->dwPackageCount, pMsg->sellPackages);
        #endif

            SUNLOG(eCRITICAL_LOG, "[MSG_GS_SHOP_PACK_LIST_REQ_ACK]PackageNum:%u", pMsg->dwPackageCount);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case eSHOP_SIGNAL_RECEIVE_BRANCH_LIST:
        {           
            MSG_GS_SHOP_BRANCH_LIST_REQ_ACK* pMsg = (MSG_GS_SHOP_BRANCH_LIST_REQ_ACK*)pMessage;

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
            GameShop::ShopItemBuilder builder(&g_GameShopManager);
            builder.BuildShopItem(pMsg->branchItems, pMsg->dwItemCount);
#else
            g_GameShopManager.BuildTree(pMsg->dwItemCount, pMsg->branchItems);
#endif
            SUNLOG(eCRITICAL_LOG, "[MSG_GS_SHOP_BRANCH_LIST_REQ_ACK]BranchNum:%u", pMsg->dwItemCount);
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }//End switch(eSignal)
}

//==================================================================================================
//{__NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION
void
GameShopManager::CancelWaitItemList(DWORD idx)
{   
    switch(idx)
    {
    case eSHOP_SIGNAL_RECEIVE_ITEM_LIST: 
        RequireExtendOptionList();
        break;

    case eSHOP_SIGNAL_RECEIVE_EXTEND_OPTION_LIST: 
    #ifdef __NA_00387_20070605_SHOP_MILEAGE2
        RequireMileageList(); 
    #else
        RequirePackageList();
    #endif
        break;

    case eSHOP_SIGNAL_RECEIVE_MILEAGE_LIST: 
    #ifdef __NA_00387_20070605_SHOP_MILEAGE2
        RequirePackageList(); 
    #endif
        break;

    case eSHOP_SIGNAL_RECEIVE_PACKAGE_LIST: 
        RequireBranchList(); 
        break;
    }

    m_dwWaitItemListTick[idx] = INFINITE;
}

//==================================================================================================
void
GameShopManager::Update()
{
    DWORD dwTick = GetTickCount();

    for (int i = 0; i < eSHOP_SIGNAL_RECEIVE_MAX; ++i)
    {
        if (m_dwWaitItemListTick[i] < dwTick) 
            CancelWaitItemList(i);
    }

    ShopManager::Update();
}
//}__NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION

//==================================================================================================
void
GameShopManager::RequireExtendOptionList()
{
    MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN msg;

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        SUNLOG(eCRITICAL_LOG, "[GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN]SHOP_SERVER is Disconnected1");
    }
}

//==================================================================================================
void
GameShopManager::RequireMileageList()
{
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    MSG_GS_SHOP_MILEAGE_LIST_REQ_SYN msg;

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        SUNLOG(eCRITICAL_LOG, "[GS_SHOP_MILEAGE_LIST_REQ_SYN]SHOP_SERVER is Disconnected1");
    }
#endif
}

//==================================================================================================
void
GameShopManager::RequirePackageList()
{
    MSG_GS_SHOP_PACK_LIST_REQ_SYN msg;

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        SUNLOG(eCRITICAL_LOG, "[GS_SHOP_PACK_LIST_REQ_SYN]SHOP_SERVER is Disconnected1");
    }
}

//==================================================================================================
void
GameShopManager::RequireBranchList()
{
    MSG_GS_SHOP_BRANCH_LIST_REQ_SYN msg;

    if (!g_pGameServer->SendToServer(SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg)))
    {
        SUNLOG(eCRITICAL_LOG, "[GS_SHOP_BRANCH_LIST_REQ_SYN]SHOP_SERVER is Disconnected");
    }
}

//==================================================================================================
BOOLEAN
GameShopManager::CanEnablePresent(ShopItem* const shop_item)
{
    // @note
    //  (NOTE) �۷ι� ���� ó��, eCHARGE_CHARGE_PPCARD� �����ϱ� �������� ó���Ѵ�.
    //  ��� �� ����(100112) �۷ι������� �����ϱ� ����� ���� ������, ���� �����ϴ��� �߰���
    //  Ÿ���� PPCard(���� ����)�� ��쿡 �ս��� ���� �� �ֱ� ������ ���� �ʿ䰡 �ִ�.
    //  (�ϴ� ��ũ��Ʈ ��� ������ ����������, �־��� ��Ȳ�� ���⸦ ����ϴ� ��ȹ�� �䱸 ���� �ݿ�)

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(shop_item != NULL, false);

        ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();
        const ShopItem::eTYPE item_type = shop_item->GetType();
        switch(item_type)
        {
        case shop_item->SINGLE:
            {
                SingleItem* const single_item = static_cast<SingleItem*>(shop_item);
                const SLOTCODE item_code = single_item->GetItemCode();
                const BASE_ITEMINFO* item_info = item_info_parser->GetItemInfo(item_code);
                // (JAYKANG) (090807) (CRASH-FIX)
                if (NULL == item_info) { 
                    SUNLOG(eCRITICAL_LOG, "[GameShopManager::CanEnablePresent] "
                                          "ERROR item [%d] not found for shop item [%d]",
                                          item_code, shop_item->GetGuid());
                    return false;
                }
                if (SCItemSlot::CanTradeSell(item_info, eITEM_TRADESELL_CASH_PRESENT) == false) {
                    return false;
                }
            //#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
            //    if (item_info->m_ChargeType == eCHARGE_CHARGE) {
            //        return false;
            //    }
            //#endif
            };
            break;
        case shop_item->PACKAGE:
            {
                PackageItem* const package_item = static_cast<PackageItem*>(shop_item);
                const BYTE upperbound = package_item->GetPieceCount();
                for (BYTE piece_idx = 0 ; piece_idx < upperbound ; ++piece_idx)
                {
                    const SLOTCODE item_code = package_item->GetItemCode(piece_idx);
                    const BASE_ITEMINFO* item_info = item_info_parser->GetItemInfo(item_code);
                    // (JAYKANG) (090807) (CRASH-FIX)
                    if (NULL == item_info) {
                        SUNLOG(eCRITICAL_LOG, "[GameShopManager::CanEnablePresent] "
                                              "ERROR item [%d] not found for shop item [%d]",
                                              item_code, shop_item->GetGuid());
                        return false;
                    }
                    if (SCItemSlot::CanTradeSell(item_info, eITEM_TRADESELL_CASH_PRESENT) == false) {
                        return false;
                    }
                //#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
                //    if (item_info->m_ChargeType == eCHARGE_CHARGE) {
                //        return false; // (NOTE) �۷ι� ���� ó��, eCHARGE_CHARGE_PPCARD� ����
                //    }
                //#endif
                }
            };
            break;
        }

        return true;
    }
    FLOWCONTROL_END;

    return false;
}

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
bool GameShopManager::Search(GameShopUser* shopuser, SearchInfo& search_info)
{
    uint search_cache_key = (search_info.input.partition_index << 24) | 
                            (search_info.input.category_index << 16) | 
                            (search_info.input.filter);

    ShopCategory* category = FindCategory(search_info.input.partition_index, search_info.input.category_index);
    ASSERT(category);
    if (category == NULL)
        return false;

    STLX_DEQUE<ShopItem*>* shopitems = NULL;
    Search_Cache_HashMap::iterator iter = search_cache_hashmap_.find(search_cache_key);
    if (iter != search_cache_hashmap_.end())
    {
        Search_Cache& search_cache = iter->second;
        if (search_cache.change_count != category->get_shopitem_changed_count())
        {
            search_cache.shopitems.clear();
            category->Search(search_cache.shopitems, search_info.input.filter);
        }
        Search(search_cache.shopitems, category, shopuser, search_info);
        search_cache.change_count = category->get_shopitem_changed_count();
    }
    else
    {
        Search_Cache search_cache;
        category->Search(search_cache.shopitems, search_info.input.filter);
        Search(search_cache.shopitems, category, shopuser, search_info);
        search_cache.change_count = category->get_shopitem_changed_count();
        search_cache_hashmap_[search_cache_key] = search_cache;
    }

    return true;
}

bool GameShopManager::Search(STLX_DEQUE<ShopItem*>& shopitems, ShopCategory* category, GameShopUser* shopuser, SearchInfo& search_info)
{
    GameShop::BitBlockContainer::BitBlock* bit_block = bit_block_container_.find_bit_block(shopuser->GetUserGuid());
    if (bit_block == NULL)
        return false;


    // ��ü ������ ����.
    uint total_count = shopitems.size();
    uint count_per_page = category->get_shopitem_count_per_page();
    uint total_page_count = (uint)ceil((double)total_count/(double)count_per_page);
    uint last_page_index = (total_page_count > 0) ? (total_page_count - 1) : 0;
    uint page = min(search_info.input.page, last_page_index);


    uint item_count = 0;
    uint guid_count = 0;

    // ������ �ɷ�����
    if (total_count)
    {
        uint start_idx = count_per_page * page;
        uint read_count = start_idx + min(count_per_page, (total_count - start_idx));
        for (uint idx = start_idx ; idx < read_count ; ++idx)
        {
            if (guid_count >= SearchInfo::Output::Max_Count)
                break;

            ShopItem* shopitem = shopitems[idx];

            search_info.output.exist_guid[guid_count] = shopitem->GetGuid();
            ++guid_count;

            // ������ ������ ���� �ʴ� �������ΰ��
            if (!bit_block->is_on(shopitem->GetCache_Index()))
            {
                search_info.output.shopitems[item_count] = shopitem;
                bit_block->flag_on(shopitem->GetCache_Index());
                ++item_count;
            }
        }
    }

    search_info.output.page = page;
    search_info.output.total_page = total_page_count;
    search_info.output.exist_guid_count = guid_count;
    search_info.output.shopitems_count = item_count;

    return true;
}

#endif


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
