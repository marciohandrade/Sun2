#include "stdafx.h"
#include "./ItemManager_Trading.h"
#include "ItemManager.h"

#include "ItemTypeList.h"
#include "ExchangeRewardInfo.h"
#include "RewardInfoList.h"

#include "InventorySlotContainer.h"
#include "SlotManager.h"

#include "RewardManager.h"
#include "ChaoState.h"

#include "AchievementManager.h"
#include "AccumulatePoint.h"
#include <CurrencyInfoParser.h>
#include <GameGuild.h>
#include <GameGuildManager.h>

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemTradingImpl) == sizeof(ItemManager*));

}; //end of namespace

//==================================================================================================
//


RC::eITEM_RESULT nsSlot::ItemTradingImpl::Exchange(
    POSTYPE atPos, INVENTORY_TOTAL_INFO* OUT pTotalInfo)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_EXCHANGE_ITEM))
        return RC::RC_ITEM_UNUSABLE_FUNCTION;

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, atPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCSlotContainer* pAtContainer  = root_->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& rItemSlot  = (SCItemSlot &)pAtContainer->GetSlot(atPos);
    //BASE_ITEMINFO* pInfo = rItemSlot.GetItemInfo();

    // 판매불가능인가?
    // Exchange아이템은 판매불가 아이템이다.
    /*
        if ((rItemSlot.GetItemInfo()->m_byTradeSellType & eITEM_TRADESELL_SELL))
        {
            return RC::RC_ITEM_CANNOT_SELL_ITEM;
        }
    */

    // 소켓 개수 다시 생성 종류 아이템은 규칙상 교환 불가능하다.
    if (rItemSlot.GetItemInfo()->m_wType == eITEMTYPE_STAT_CHANGE)
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    if (0 == rItemSlot.GetItemInfo()->m_ExchangedItem)
    {
        return RC::RC_ITEM_CANNOT_EXCHANGE_ITEM;
    }

    if (rItemSlot.GetItemInfo()->IsChargeItem())
    {
        return RC::RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM;
    }

    RC::eITEM_RESULT rt = root_->Obtain(rItemSlot.GetItemInfo()->m_ExchangedItem, 1, pTotalInfo);
    if (rt == RC::RC_ITEM_SUCCESS)
    {
        // 아이템 삭제 로그
        SCItemSlot ObtainItem;
        ObtainItem.SetCode(rItemSlot.GetItemInfo()->m_ExchangedItem);
        GAMELOG->LogItemEx(ITEM_EXCHANGE, root_->player_, rItemSlot, ObtainItem);

        root_->Lose(pAtContainer, atPos, 1);
    }

    return rt;
}

RC::eITEM_RESULT nsSlot::ItemTradingImpl::Sell(
    const SLOTIDX slot_type, const POSTYPE slot_position, const DURATYPE sell_quantity)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_SELL_ITEM))
    {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
        
    if (!root_->ValidState())
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    if (!root_->ValidPos(slot_type, slot_position))
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
        
    // 인벤토리가 아니면 팔 수 없다.
    if (slot_type != SI_INVENTORY)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    if (!root_->player_->GetChaoState().CanUseFuction(CHAO_USE_STORE))
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
        
    if (root_->player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING)
    {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }

    SCSlotContainer* const item_slot_container = root_->GetItemSlotContainer(slot_type);
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(item_slot_container->GetSlot(slot_position));
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    if (item_info == NULL)
    {
        return RC::RC_ITEM_NOINFO;
    }

    // 판매불가능인가?
    if (item_slot.CanTradeSell(eITEM_TRADESELL_SELL) == false) 
    {
        return RC::RC_ITEM_CANNOT_SELL_ITEM;
    }

    // 아이템을 분할하여 판매한다면
    const bool is_divide_sell = (sell_quantity > 0 && sell_quantity != item_slot.GetNum());
    if (is_divide_sell) 
    {
        // 판매 개수를 검사한다.
        if (sell_quantity > item_slot.GetNum())
        {
            return RC::RC_ITEM_INVALID_VALUE;
        }

        // 임시 아이템 슬롯을 만들어서
        SCItemSlot temp_item_slot; 
        temp_item_slot.Copy(item_slot); 
        temp_item_slot.SetNum(sell_quantity);
        temp_item_slot.SetSerial(TEMP_DBSERIAL_VALUE);
        
        // 금액을 획득한다.
        const MONEY price_of_goods = temp_item_slot.GetPriceForSell();
        if (!root_->player_->PlusMoney(price_of_goods))
        {
            return RC::RC_ITEM_FAILED;
        }

        // 현재 아이템 수량을 판매 개수만큼 감소하고
        item_slot_container->UpdateSlot(slot_position, -sell_quantity);

        // 판매 목록에 등록한다.
        RegisterSaleItem(temp_item_slot);

        // 로그를 기록한다.
        {
            GAMELOG->LogItem_Pos(ITEM_NPC_SELL,
                                 root_->player_,
                                 &temp_item_slot,
                                 price_of_goods,
                                 temp_item_slot.GetNum(),
                                 slot_type,
                                 slot_position,
                                 0,
                                 0);
            GAMELOG->LogMoney(MONEY_NPC_SELL, root_->player_, price_of_goods);
        }
    }
    else // 분할 판매가 아니라면 이전과 동일한 방식으로 판매한다.
    {
        const MONEY price_of_goods = item_slot.GetPriceForSell();
        if (!root_->player_->PlusMoney(price_of_goods))
        {
            return RC::RC_ITEM_FAILED;
        }
        
        DEBUG_CODE(
            if (price_of_goods == 0 && item_slot.GetItemInfo()->m_wType != eITEMTYPE_WASTE) 
            SUNLOG(eFULL_LOG, _T("[nsSlot::ItemTradingImpl::Sell] 아이템(%s)파는가격이%I64u=0이다."),
                   item_info->m_pszName, price_of_goods););
        
        GAMELOG->LogItem_Pos(ITEM_NPC_SELL,
                             root_->player_,
                             &item_slot,
                             price_of_goods,
                             item_slot.GetNum(),
                             slot_type,
                             slot_position,
                             0,
                             0);
        GAMELOG->LogMoney(MONEY_NPC_SELL, root_->player_, price_of_goods);

        RegisterSaleItem(item_slot);
        item_slot_container->DeleteSlot(slot_position, NULL);
    }

    return RC::RC_ITEM_SUCCESS;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// @praram 
//  byNumberOfItemToBuy : m_byDupNum을 넘을 수 없다.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
RC::eITEM_RESULT nsSlot::ItemTradingImpl::Buy(
    const DWORD shop_list_id, const BYTE shop_tab_index, const BYTE shop_item_index,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
    const DURATYPE buy_quantity)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_BUY_ITEM))
    {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
        
    if (!root_->ValidState())
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    if (!root_->ValidContainer(SI_INVENTORY))
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    if (!root_->player_->GetChaoState().CanUseFuction(CHAO_USE_STORE))
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
        
    if (root_->player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING)
    {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }

    SHOPINFO* const shop_info = ShopInfoParser::Instance()->GetShopList(shop_list_id);
    if (shop_info == NULL)
    {
        return RC::RC_ITEM_INVALIDSHOPLISTID;
    }

    if (shop_tab_index >= SHOPINFO::_MAX_TAB_NUM ||
        shop_item_index >= SHOPINFO::_MAX_SELLITEM_NUM)
    {
        return RC::RC_ITEM_OUTOFSHOPITEMINDEX;
    }

    SHOPINFO::SHOP_ITEM* const shop_item =
        shop_info->GetItemForIndex(shop_tab_index, shop_item_index);
    if (shop_item == NULL)
    {
        return RC::RC_ITEM_OUTOFSHOPITEMINDEX;
    }

    const SLOTCODE buy_wanted_item_code = shop_item->m_SellItemCode;
        
    //!임시가격!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!임시가격!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // 돈이 있는지 체크

    const DURATYPE number_of_buy_wanted_item = (buy_quantity > 0) ?
        buy_quantity : shop_item->m_SellItemNum;
    SCItemSlot buy_wanted_item_slot;
    // (f100728.1L) add a making item interface related to the item type information
    if (nsSlot::ItemTypeChanger::MakeItemSlotByType(&buy_wanted_item_slot,
                                                    buy_wanted_item_code,
                                                    number_of_buy_wanted_item,
                                                    shop_item->item_type_) == false)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    //
    const BASE_ITEMINFO* const buy_wanted_item_info = buy_wanted_item_slot.GetItemInfo();
    //    for client check routine
    if (buy_wanted_item_info == NULL || buy_wanted_item_info->m_Code == 0) {
        return RC::RC_ITEM_OUTOFSHOPITEMINDEX;
    }
#ifdef _NA_007031_20140116_CAN_SELL_CASH_ITEM_IN_STORE
#else
    if (buy_wanted_item_info->IsChargeItem()) {
        return RC::RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM;
    }
#endif //_NA_007031_20140116_CAN_SELL_CASH_ITEM_IN_STORE

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    if (shop_info->GetShopType() == SHOP_TYPE_OF_CURRENCY &&
        shop_info->GetCurrencyItemCode() == CURRENCY_INFO_GUILD_COIN)
    {
        GUILDGUID guid = root_->player_->GetGuildGuid();
        if (INVALID_GUILDGUID == guid) 
            return RC::RC_ITEM_INVALIDSTATE;

        GameGuild* game_guild = g_GameGuildManager.FindGuild(guid);
        if (NULL == game_guild)
            return RC::RC_ITEM_INVALIDSTATE;
        
        // fame_rating_를 길드레벨로 사용.
        if (game_guild->GetGuildGrade() < shop_item->fame_rating_)
            return RC::RC_ITEM_BUY_ERROR_GUILD_LEVEL;

        // repute_rating_을 길드공헌도로 사용
        PointWalletManager* const pointwallet_manager = root_->player_->GetPointWalletManager();
        int guild_point = pointwallet_manager->GetPointWallet(CURRENCY_INFO_DONATION_GUILDCOIN);
        if (guild_point < shop_item->repute_rating_)
            return RC::RC_ITEM_BUY_ERROR_GUILD_EXP;
    }
    else 
    {
        HonorManager* honor_manager = root_->player_->GetHonorManager();
        if (shop_item->repute_area_ > 0 && shop_item->repute_rating_ > 0) {
            int repute_rating= \
                honor_manager->GetFameReputeGrade(HONOR_POINT_INFO::REPUTE, shop_item->repute_area_);
            if(repute_rating < shop_item->repute_rating_) {
                return RC::RC_ITEM_BUY_ERROR_OF_REPUTE;
            }
        }

        if (shop_item->fame_area_ > 0 && shop_item->fame_rating_ > 0) {
            int fame_rating = \
                honor_manager->GetFameReputeGrade(HONOR_POINT_INFO::FAME, shop_item->fame_area_);
            if(fame_rating < shop_item->fame_rating_) {
                return RC::RC_ITEM_BUY_ERROR_OF_FAME;
            }
        }
    }
#else 
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HonorManager* honor_manager = root_->player_->GetHonorManager();
    if(shop_item->repute_area_ > 0 && shop_item->repute_rating_ > 0)
    {
        int repute_rating= \
            honor_manager->GetFameReputeGrade(HONOR_POINT_INFO::REPUTE, shop_item->repute_area_);
        if(repute_rating < shop_item->repute_rating_)
        {
            return RC::RC_ITEM_BUY_ERROR_OF_REPUTE;
        }
    }

    if(shop_item->fame_area_ > 0 && shop_item->fame_rating_ > 0)
    {
        int fame_rating = \
            honor_manager->GetFameReputeGrade(HONOR_POINT_INFO::FAME, shop_item->fame_area_);
        if(fame_rating < shop_item->fame_rating_)
        {
            return RC::RC_ITEM_BUY_ERROR_OF_FAME;
        }
    }
#endif //NA_003027_20111013_HONOR_SYSTEM
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // 상점에서 파는것들은 기본적으로 소켓을 생성하지 않지만 
    // 아이템 타입 정보가 설정되어 있으면 소켓 생성에 우선적으로 사용한다.
    const nsSlot::ItemRuleInfo& item_rule_info = buy_wanted_item_slot.GetItemRuleInfo();
    if (shop_item->item_type_ == NULL && item_rule_info.IsEnableSocket())
    {
        buy_wanted_item_slot.SetSocketIdentifyType(eSOCKET_FAIL_IDENTIFY_PROCESS);
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
    buy_wanted_item_slot.SetDateTime(shop_item->timelimit_ * 60);
#endif //_NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP

#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    const RC::eITEM_RESULT pay_result = _PayProcessing(shop_tab_index,
                                                       number_of_buy_wanted_item,
                                                       buy_wanted_item_slot,
                                                       buy_msg,
                                                       shop_info,
                                                       shop_item);
#else //_NA_20110303_ADD_TYPE_OF_NPCSHOP
    const RC::eITEM_RESULT pay_result = _PayProcessing(shop_tab_index,
                                                       number_of_buy_wanted_item,
                                                       buy_wanted_item_slot,
                                                       buy_msg,
                                                       shop_info);
#endif //_NA_20110303_ADD_TYPE_OF_NPCSHOP

    return pay_result;
}

#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
//gamelulu_dev_note : 아이템 메니저에서 npc상점 부분을 분리 해야할 필요성이 있다.
RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayProcessing(
    const BYTE shop_tab_index,
    const DURATYPE number_of_buy_wanted_item,
    const SCItemSlot buy_wanted_item_slot,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
    SHOPINFO* shop_info,
    SHOPINFO::SHOP_ITEM* const shop_item)
{
    RC::eITEM_RESULT process_result_value = RC::RC_ITEM_SUCCESS;

    switch (shop_info->GetShopType())
    {
    case SHOP_TYPE_OF_NARMAL://일반
        process_result_value = _PayNormalShop(shop_info,
                                              shop_tab_index,
                                              buy_wanted_item_slot, 
                                              number_of_buy_wanted_item,
                                              buy_msg);
        break;

    case SHOP_TYPE_OF_ITEM://아이템
        process_result_value = _PayItemShop(shop_info,
                                            shop_item, 
                                            buy_wanted_item_slot,
                                            number_of_buy_wanted_item,
                                            buy_msg);
        break;

    case SHOP_TYPE_OF_ACCUMULATE://적립 포인트
        process_result_value = _PayAccumulatePointShop(shop_info,
                                                       shop_item,
                                                       buy_wanted_item_slot,
                                                       number_of_buy_wanted_item, 
                                                       buy_msg);
        break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        case SHOP_TYPE_OF_CURRENCY: 
        {
            process_result_value = _PayPointWalletShop( \
                shop_info, shop_item, buy_wanted_item_slot, number_of_buy_wanted_item, buy_msg);
        } break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
        default:
            return RC::RC_ITEM_FAILED;
    }

#ifdef _NA_001990_ACSYSTEM_ADD
    if (process_result_value == RC::RC_ITEM_SUCCESS)
    {
        const BASE_ITEMINFO* item_info = buy_wanted_item_slot.GetItemInfo();
        if (item_info->m_wACCode) {
            root_->player_->GetACManager()->ProcessItemObtain(
                item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, number_of_buy_wanted_item);
        }
    }
#endif //_NA_001990_ACSYSTEM_ADD

    return process_result_value;
}

RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayNormalShop(
    SHOPINFO* shop_info,
    const BYTE shop_tab_index,
    const SCItemSlot buy_wanted_item_slot, 
    const DURATYPE number_of_buy_wanted_item,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg)
{
    MSG_CG_ITEM_BUY_ACK* msg = static_cast<MSG_CG_ITEM_BUY_ACK*>(buy_msg);

    MONEY price_of_goods = buy_wanted_item_slot.GetPriceForBuy() * number_of_buy_wanted_item;
    ASSERT(price_of_goods > 0);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //상점 구매 하임 증감 어트리뷰트 적용
    price_of_goods = root_->player_->GetAttr().GetValueAppliedAttr(price_of_goods, \
        eATTR_SHOP_BUY_HEIM_RATIO);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //체크
    if (root_->player_->GetMoney() < price_of_goods)
    {
        return RC::RC_ITEM_HAVENOTMONEY;
    } 

    //지급
    const RC::eITEM_RESULT pay_result_value = 
        root_->Obtain(buy_wanted_item_slot, number_of_buy_wanted_item, &(msg->m_TotalInfo));
    if (pay_result_value != RC::RC_ITEM_SUCCESS)
    {
        return pay_result_value;
    }

    //삭제
    root_->player_->MinusMoney(price_of_goods);
    GAMELOG->LogMoney(MONEY_NPC_BUY, root_->player_, price_of_goods);

    msg->m_Money = root_->player_->GetMoney();
    msg->m_ShopType = shop_info->GetShopType();

    GAMELOG->LogItem(ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_, root_->player_, &buy_wanted_item_slot, 
                     number_of_buy_wanted_item, static_cast<WORD>(price_of_goods),shop_info);

    //gamelulu_dev_note : 기존 로그 지원을 위해 남긴다.
    GAMELOG->LogItem(ITEM_NPC_BUY, root_->player_, &buy_wanted_item_slot, 
                     price_of_goods, number_of_buy_wanted_item);
    g_pGameServer->NotifyHeimConsumption(root_->player_->GetCharGuid(), 
                                         eHEIM_CONSUMPTION_NPC_SHOP, 
                                         price_of_goods, 
                                         shop_info->GetLottoAccumRatio(shop_tab_index));

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayItemShop(
    SHOPINFO* shop_info,
    SHOPINFO::SHOP_ITEM* const shop_item, 
    const SCItemSlot buy_wanted_item_slot,
    const DURATYPE number_of_buy_wanted_item,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg)
{
    MSG_CG_ITEM_BUY_ACK* msg = static_cast<MSG_CG_ITEM_BUY_ACK*>(buy_msg);

    size_t check_price_of_goods = shop_item->item_cost_ * number_of_buy_wanted_item;
    if (check_price_of_goods > USHRT_MAX)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Item price value is over : prive value : %d",
            check_price_of_goods);
        check_price_of_goods = USHRT_MAX;
    }
    const WORD price_of_goods = static_cast<WORD>(check_price_of_goods);
    ASSERT(price_of_goods > 0);

    //체크
    ns_functor::EmptyItemCheck empty_checker(shop_info->GetCurrencyItemCode(), price_of_goods);
    if (root_->ForeachSlot(SI_INVENTORY, &empty_checker) == false)
    {
        return RC::RC_ITEM_IS_SHORT_HAVE_ITEM;
    }

    //지급
    const RC::eITEM_RESULT obtain_result_value = 
        root_->Obtain(buy_wanted_item_slot, number_of_buy_wanted_item, &(msg->m_TotalInfo));
    if (obtain_result_value != RC::RC_ITEM_SUCCESS)
    {
        return obtain_result_value;
    }

    //삭제
    MSG_CG_ITEM_LOSE_ACK pay_msg;
    SLOTCODE currency_item_code = shop_info->GetCurrencyItemCode();
    const RC::eITEM_RESULT pay_result_value = 
        root_->DeleteOneKindOfItem(currency_item_code, price_of_goods, &(pay_msg.m_ItemInfo));
    if (pay_result_value != RC::RC_ITEM_SUCCESS)
    {
        return pay_result_value;
    }
    root_->player_->SendPacket(&pay_msg, pay_msg.GetSize());
    
    msg->m_Money = root_->player_->GetMoney();
    msg->m_ShopType = shop_info->GetShopType();

    GAMELOG->LogItem(ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_, root_->player_, &buy_wanted_item_slot, 
                     number_of_buy_wanted_item, static_cast<WORD>(price_of_goods),shop_info);


    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayAccumulatePointShop(
    SHOPINFO* shop_info,
    SHOPINFO::SHOP_ITEM* const shop_item, 
    const SCItemSlot buy_wanted_item_slot,
    const DURATYPE number_of_buy_wanted_item,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg)
{
    MSG_CG_ITEM_BUY_ACK* msg = static_cast<MSG_CG_ITEM_BUY_ACK*>(buy_msg);

#ifdef _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE
	WORD price_of_goods =  static_cast<WORD>(shop_item->item_cost_);
	if(shop_item->m_SellItemNum > 1)
	{
		// 사려고 하는 수량이 기본 묶음보다 커야 한다.
		if(number_of_buy_wanted_item < shop_item->m_SellItemNum)
		{
			return RC::RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT;
		}
		// 묶음 아이템일 경우는 가격을 다시 설정해 줘야 한다.
		if((number_of_buy_wanted_item % shop_item->m_SellItemNum) != 0)
		{
			return RC::RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT;
		}
		else
		{
			price_of_goods *= (number_of_buy_wanted_item / shop_item->m_SellItemNum);
		}
	}
	else
	{
		price_of_goods = static_cast<WORD>(shop_item->item_cost_ * number_of_buy_wanted_item);
	}
#else
	WORD price_of_goods =  static_cast<WORD>(shop_item->item_cost_ * number_of_buy_wanted_item);
#endif //(_NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE)
    ASSERT(price_of_goods > 0);

    //체크
    if (root_->player_->GetAccumulatePoint()->get_accumulate_point() < price_of_goods)
    {
        return RC::RC_ITEM_IS_SHORT_HAVE_ACCUMULATEPOINT;
    }
    
    //지급
    const RC::eITEM_RESULT pay_result_value 
        = root_->Obtain(buy_wanted_item_slot, number_of_buy_wanted_item, &(msg->m_TotalInfo));
    if (pay_result_value != RC::RC_ITEM_SUCCESS)
    {
        return pay_result_value;
    }

    //삭제
    root_->player_->GetAccumulatePoint()->MinusAccumulatePoint(price_of_goods);
    GAMELOG->LogAccumulatePoint(ACCUM_CONSUMPTION_ROULETTE, root_->player_, price_of_goods, 
                                buy_wanted_item_slot.GetItemCode());

    msg->m_Money = static_cast<MONEY>(root_->player_->GetAccumulatePoint()->get_accumulate_point());
    msg->m_ShopType = shop_info->GetShopType();

    GAMELOG->LogItem(ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_, root_->player_, &buy_wanted_item_slot, 
                     number_of_buy_wanted_item, static_cast<WORD>(price_of_goods),shop_info);

    return RC::RC_ITEM_SUCCESS;
}
#else //_NA_20110303_ADD_TYPE_OF_NPCSHOP
RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayProcessing(
    const BYTE shop_tab_index,
    const DURATYPE number_of_buy_wanted_item,
    const SCItemSlot buy_wanted_item_slot,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
    SHOPINFO* shop_info)
{
    MSG_CG_ITEM_BUY_ACK* msg = static_cast<MSG_CG_ITEM_BUY_ACK*>(buy_msg);

    const MONEY price_of_goods = buy_wanted_item_slot.GetPriceForBuy() * number_of_buy_wanted_item;
    ASSERT(price_of_goods > 0);

    if (root_->player_->GetMoney() < price_of_goods)
    {
        return RC::RC_ITEM_HAVENOTMONEY;
    } 

    const RC::eITEM_RESULT obtain_result = 
        root_->Obtain(buy_wanted_item_slot, number_of_buy_wanted_item, &(msg->m_TotalInfo));

    if (obtain_result == RC::RC_ITEM_SUCCESS)
    {
        root_->player_->MinusMoney(price_of_goods);

        msg->m_Money = root_->player_->GetMoney();

        GAMELOG->LogItem(ITEM_NPC_BUY, root_->player_, &buy_wanted_item_slot, 
                         price_of_goods, number_of_buy_wanted_item);
        GAMELOG->LogMoney(MONEY_NPC_BUY, root_->player_, price_of_goods);
        g_pGameServer->NotifyHeimConsumption(root_->player_->GetCharGuid(), 
                                             eHEIM_CONSUMPTION_NPC_SHOP, 
                                             price_of_goods, 
                                             shop_info->GetLottoAccumRatio(shop_tab_index));
#ifdef _NA_001990_ACSYSTEM_ADD
        const BASE_ITEMINFO* item_info = buy_wanted_item_slot.GetItemInfo();
        if (item_info->m_wACCode) {
            root_->player_->GetACManager()->ProcessItemObtain(
                item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, number_of_buy_wanted_item);
        }
#endif //_NA_001990_ACSYSTEM_ADD

        return obtain_result;//success
    }

    return obtain_result;//fail
}
#endif //_NA_20110303_ADD_TYPE_OF_NPCSHOP

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
RC::eITEM_RESULT nsSlot::ItemTradingImpl::_PayPointWalletShop(
    SHOPINFO* shop_info,
    SHOPINFO::SHOP_ITEM* const shop_item, 
    const SCItemSlot buy_wanted_item_slot, 
    const DURATYPE number_of_buy_wanted_item,
    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg)
{
    MSG_CG_ITEM_BUY_ACK* msg = static_cast<MSG_CG_ITEM_BUY_ACK*>(buy_msg);
    
    DWORD pointwallet_code = shop_info->GetCurrencyItemCode();
    DWORD price_of_goods = static_cast<DWORD>(shop_item->item_cost_ * number_of_buy_wanted_item);
 
    //1. 체크
    PointWalletManager* const pointwallet_manager = root_->player_->GetPointWalletManager();
    if (pointwallet_manager->GetPointWallet(pointwallet_code) < price_of_goods) {
        return RC::RC_ITEM_FAILED_TO_BUY_BY_POINT;
    }

    //2. 지급
    const RC::eITEM_RESULT pay_result_value = \
        root_->Obtain(buy_wanted_item_slot, number_of_buy_wanted_item, &msg->m_TotalInfo);
    if (pay_result_value != RC::RC_ITEM_SUCCESS) {
        return pay_result_value;
    }

    //3. 차감
    DWORD previous_point = pointwallet_manager->GetPointWallet(pointwallet_code);
    DWORD result_point   = pointwallet_manager->ChangeWalletPoint(pointwallet_code, -price_of_goods);
        
    msg->m_Money = root_->player_->GetMoney();
    msg->m_ShopType = shop_info->GetShopType();

    //4. 로그
    GAMELOG->LogItem(ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_, root_->player_, &buy_wanted_item_slot, 
        number_of_buy_wanted_item, static_cast<WORD>(price_of_goods), shop_info);

    GAMELOG->LogPointWallet(root_->player_, POINTWALLET_WITHDRAW_BY_BUYING_ITEMS_ON_MARKET,
        pointwallet_code, previous_point, -price_of_goods, result_point);

    return RC::RC_ITEM_SUCCESS;
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

//==================================================================================================
//==================================================================================================
//==================================================================================================

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
RC::eITEM_RESULT nsSlot::ItemTradingImpl::ExchangeRewardItem(
    SLOTCODE ItemCode, DURATYPE num,
    INVENTORY_TOTAL_INFO* OUT pTotalInfo)
{
    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    // 교환 가능한 아이템인지 스크립트 체크
    EXCHANGE_REWARDINFO* pExchangeInfo = \
        ExchangeRewardInfoParser::Instance()->GetRewardInfo(ItemCode, num);
    if (!pExchangeInfo)
        return RC::RC_ITEM_CANNOT_EXCHANGE_ITEM;

    // 충분한 개수의 아이템이 있는지 체크
    int ExistCount = root_->ExistItem(SI_INVENTORY, ItemCode, num);
    if (ExistCount < num)
        return RC::RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT;

    // 선택 아이템 선택
    RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(
        pExchangeInfo->m_RewardIndex);
    if (!pRewardInfo) return RC::RC_ITEM_CANNOT_EXCHANGE_ITEM;
    int SelectableCount = 0;
    for (int i = 0 ; i < RewardInfo::MAX_DROP_INDEX_NUM ; ++i)
    {
        if (pRewardInfo->GetTypeCode(i))
            SelectableCount++;
    }
    SelectableCount = max(0, SelectableCount - 1);
    BYTE SelectItemBits = (1 << random(0,SelectableCount));

    // 보상 아이템 획득
    RC::eREWARD_RESULT rcResult = g_RewardManager.RewardItem(
        RewardManager::eREWARD_EXCHANGE_ITEM, 
        pExchangeInfo->m_RewardIndex, SelectItemBits, root_->player_, *pTotalInfo);
    if (rcResult != RC::RC_REWARD_SUCCESS)
        return RC::RC_ITEM_ISNOTEMPTYSLOT;

    // 교환할 아이템 삭제
    MSG_CG_ITEM_LOSE_ACK LoseMsg;
    RC::eITEM_RESULT rcResult2 = \
        root_->DeleteAllKindOfItem(1, &ItemCode, &num, &(LoseMsg.m_ItemInfo));
    if (rcResult2 != RC::RC_ITEM_SUCCESS)
        return rcResult2;

    root_->player_->SendPacket(&LoseMsg, LoseMsg.GetSize());

    return RC::RC_ITEM_SUCCESS;
}
#endif


//==================================================================================================
//==================================================================================================
//==================================================================================================

void
nsSlot::ItemTradingImpl::RegisterSaleItem(SCItemSlot& item)
{
    BYTE pos = 0;
    RepurchaseSlot* pRepurchase = NULL;
    for (;pos < MAX_REPURCHASE_ITEM_NUM; ++pos)
    {
        // 비어있는 슬롯을 찾는다.
        pRepurchase = &root_->saled_item_list_[pos];

#ifdef _DEBUG
        if (!pRepurchase) ASSERT(0);
#endif

        if (!pRepurchase->m_bIsUse)
        {
            pRepurchase->m_bIsUse = true;
            pRepurchase->m_itemSlot = new SCItemSlot;
            pRepurchase->m_itemSlot->Copy(item);
            break;
        }
    }

    // 비어있는 슬롯이 없을 경우
    if (MAX_REPURCHASE_ITEM_NUM <=pos)
    {
        // (CHANGES) (f100802.1L) fix up an unsafe pointer usage case.
        // a deque event after pop_front has free operation or reuse operation.
        SCItemSlot* reuse_slot = NULL;
        ;{
            RepurchaseSlot* delete_reserved_repurchase_slot = &root_->saled_item_list_[0];
            reuse_slot = delete_reserved_repurchase_slot->m_itemSlot;
            root_->saled_item_list_.pop_front();
        };
        ;{
            RepurchaseSlot realign_repurchase_slot;
            realign_repurchase_slot.m_bIsUse = true;
            reuse_slot = reuse_slot ? reuse_slot : new SCItemSlot;
            reuse_slot->Copy(item);
            realign_repurchase_slot.m_itemSlot = reuse_slot;
            root_->saled_item_list_.push_back(realign_repurchase_slot);
        };
    }
}


RC::eITEM_RESULT
nsSlot::ItemTradingImpl::Repurchase(POSTYPE RepurchasePos, INVENTORY_TOTAL_INFO* pTotalInfo)
{
    if (0 >= root_->saled_item_list_.size() || MAX_REPURCHASE_ITEM_NUM <= RepurchasePos)
        return RC::RC_ITEM_EMPTYSLOT;

    ItemManager::SALE_ITEM_DEQ::iterator it(root_->saled_item_list_.begin() + RepurchasePos);
    RepurchaseSlot pRepurchase = *it;
    if (pRepurchase.m_bIsUse == false)
        return RC::RC_ITEM_EMPTYSLOT;

    SCItemSlot* Slotitem = pRepurchase.m_itemSlot;
    if (!Slotitem || !Slotitem->GetItemInfo()) 
        return RC::RC_ITEM_INVALIDSTATE;

    MONEY PriceOfGoods = Slotitem->GetPriceForSell();
    if (0 > PriceOfGoods) return RC::RC_ITEM_INVALIDSTATE;

    if (root_->player_->GetMoney() < PriceOfGoods)
        return RC::RC_ITEM_HAVENOTMONEY;


    int itemNum = Slotitem->GetNum();
    Slotitem->SetNum(1);
    RC::eITEM_RESULT rt = root_->Obtain(*Slotitem, itemNum, pTotalInfo);
    if (rt == RC::RC_ITEM_SUCCESS)
    {
        root_->player_->MinusMoney(PriceOfGoods);

        GAMELOG->LogItem(ITEM_NPC_REPURCHASE, root_->player_, Slotitem, PriceOfGoods, itemNum);
        GAMELOG->LogMoney(MONEY_NPC_BUY, root_->player_, PriceOfGoods);

        pRepurchase.m_bIsUse = false;
        SAFE_DELETE(pRepurchase.m_itemSlot);
        root_->saled_item_list_.erase(it);
        root_->saled_item_list_.push_back(pRepurchase); 

#ifdef _DEBUG //5개 유지..
        if (MAX_REPURCHASE_ITEM_NUM != root_->saled_item_list_.size())
            ASSERT("판매리스트 5개 초과.. 젠장");
#endif

    }

    return rt;
}

#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
void nsSlot::ItemTradingImpl::SerializeRepurchaseItems(
    REPURCHASITEM_TOTAL_INFO& RepurchasItems, eSERIALIZE eType)
{
    if (eType == SERIALIZE_LOAD)
    {
        // f101014.1L, added clear routine in this function
        RepurchasItems.Clear();
        ItemManager::SALE_ITEM_DEQ::iterator iter;
        for (iter = root_->saled_item_list_.begin(); 
            iter != root_->saled_item_list_.end() && MAX_REPURCHASE_ITEM_NUM > RepurchasItems.m_Count;
            ++iter)
        {
            RepurchaseSlot& slot = *iter;
            if (slot.m_bIsUse)
            {
                slot.m_itemSlot->CopyOut(RepurchasItems.m_Slot[RepurchasItems.m_Count++]);
            }
        }
    }
    else if (eType == SERIALIZE_STORE)
    {
        // f101014.1L, fixed unclear remained data.
        // NOTE: if fails, it already occur buffer overrun.
        ASSERT(RepurchasItems.m_Count <= MAX_REPURCHASE_ITEM_NUM); // 
        const int number_of_items = min(RepurchasItems.m_Count, MAX_REPURCHASE_ITEM_NUM);
        for (int i = 0; i < number_of_items; ++i)
        {            
            BYTE pos = 0;
            RepurchaseSlot* pRepurchase = NULL;
            pRepurchase = &root_->saled_item_list_[i];

            if (pRepurchase)
            {
                pRepurchase->m_bIsUse = true;
                pRepurchase->m_itemSlot = new SCItemSlot;
                pRepurchase->m_itemSlot->Copy(RepurchasItems.m_Slot[i]);
            }
            else
            {
                ASSERT("포인터가 없을수가 없는데");
            } 
        }
    }
    else
    {
        ASSERT("로딩도 아니고 저장도 아니고~ 있을수 없는일...");
    }
}

#endif //_NA_20100106_REPURCHAS_BUGFIX_2496

void nsSlot::ItemTradingImpl::ClearRepurchasItemList()
{
    ItemManager::SALE_ITEM_DEQ::iterator iter;
    for (iter = root_->saled_item_list_.begin(); iter != root_->saled_item_list_.end(); ++iter)
    {
        RepurchaseSlot& slot = *iter;
        SAFE_DELETE(slot.m_itemSlot);
    }
    root_->saled_item_list_.clear();
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

