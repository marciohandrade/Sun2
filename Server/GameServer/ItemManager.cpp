#include "stdafx.h"
#include "./ItemManager.h"

#include <PublicMath.h>

#include "InventorySlotContainer.h"
#include "EquipmentSlotContainer.h"
#include "SlotManager.h"

#include "DropManager.h"
#include "FunctionalRestrictManager.h"
#include "AchievementManager.h"
#include "ItemExpDistributor.h"
#include "Item.h"
#include "GameParty/NewGamePartyManager.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

//
//==================================================================================================
//==================================================================================================
//==================================================================================================
//	ItemManager
//
ItemManager::ItemManager()
    : player_(NULL)
    , slot_manager_(NULL)
    , slot_containers_(NULL)
    , current_special_transaction_(ItemManager::TransactionClosure::eTransType_None)
    , item_switch_handle_(NULL)
{
    item_update_timer_.SetTimer(3 * 1000);
}

ItemManager::~ItemManager()
{
	Release();
}


VOID
ItemManager::Release()
{
//	m_SerialCodeCheck.RemoveAll();
	SALE_ITEM_DEQ::iterator iter;
	for(iter = saled_item_list_.begin(); iter != saled_item_list_.end(); ++iter)
	{
		RepurchaseSlot& slot = *iter;
		SAFE_DELETE(slot.m_itemSlot);
	}
	saled_item_list_.clear();
	SAFE_DELETE(item_switch_handle_);
}


void ItemManager::Init(SlotManager* slot_manager)
{
    player_ = slot_manager->player_;
    slot_containers_ = slot_manager->slot_container_list_;
    slot_manager_ = slot_manager;

    // CHANGES: f091202.x, WOPS:5334, 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    current_special_transaction_ = TransactionClosure::eTransType_None; // ref) eLOG_ITEM
    // CHANGES: f110404.2L, this object has not release routine.
    new (&durability_impl_) nsSlot::DurabilityImpl(this);
    new (&item_move_impl_) nsSlot::ItemMoveImpl(this);
    new (&trading_impl_) nsSlot::ItemTradingImpl(this);
    new (&updater_impl_) nsSlot::ItemManagerUpdateImpl(this);
    serial_check_table_.clear();

    expiration_item_table_.clear();

    saled_item_list_.clear();
    saled_item_list_.resize(MAX_REPURCHASE_ITEM_NUM, RepurchaseSlot());
}

//==================================================================================================
//== basic check routines
//==================================================================================================

bool ItemManager::IsEmpty(SLOTIDX slot_index, POSTYPE pos_in_slots)
{
    ASSERT(slot_index < SI_MAX && slot_index != SI_SKILL);
    return slot_manager_->IsEmpty(slot_index, pos_in_slots);
}

SCItemSlotContainer* ItemManager::GetItemSlotContainer(const SLOTIDX slot_index) const
{
    const ulong kAcceptablContainers = (1 << SI_INVENTORY)
                                     | (1 << SI_EQUIPMENT)
                                     | (1 << SI_WAREHOUSE)
                                     | (1 << SI_GUILDWAREHOUSE);
    if (bool in_range = (((1 << slot_index) & kAcceptablContainers) != 0)) {
        SCItemSlotContainer* selected_container =
            static_cast<SCItemSlotContainer*>(slot_manager_->GetSlotContainer(slot_index));
        // the guild warehouse container may have not in list
        return selected_container; 
    }
    return NULL;
}

bool ItemManager::ValidPos(
    const SLOTIDX slot_index, const POSTYPE pos_in_slots, const bool check_empty) const
{
    const ulong kPreventContainers = (1 << SI_SKILL)
                                   | (1 << SI_STYLE) // removed by _NA002217_100728_EP2_PERK_SYSTEM_
                                   | (1 << SI_QUICK);
    if (bool is_prevent = (((1 << slot_index) & kPreventContainers) != 0)) {
        return false;
    }

    return slot_manager_->ValidPos(slot_index, pos_in_slots, check_empty);
}

bool ItemManager::ValidContainer(SLOTIDX slot_index)
{
    return slot_manager_->ValidContainer(slot_index);
}

bool ItemManager::ValidState() const
{
    return slot_manager_->ValidState();
}

bool ItemManager::IsRestrictFunction(eFUNCTIONALRESTRICT_TYPE type) const
{
    switch (type)
    {
    case eFUNCTIONALRESTRICT_ENCHANT:
    case eFUNCTIONALRESTRICT_RANKUP:
    case eFUNCTIONALRESTRICT_ADDSOCKET:
    case eFUNCTIONALRESTRICT_FILLSOCKET:
    case eFUNCTIONALRESTRICT_EXTRACTSOCKET:
    case eFUNCTIONALRESTRICT_COMPOSE:
    case eFUNCTIONALRESTRICT_CRYSTALIZE:
    case eFUNCTIONALRESTRICT_USE_EXTRASTONE:
    case eFUNCTIONALRESTRICT_USE_WAREHOUSE:
    //case eFUNCTIONALRESTRICT_USE_TRADE:
    //case eFUNCTIONALRESTRICT_USE_VENDOR:
    case eFUNCTIONALRESTRICT_REPAIR_ITEM:
    case eFUNCTIONALRESTRICT_SELL_ITEM:
    case eFUNCTIONALRESTRICT_BUY_ITEM:
    case eFUNCTIONALRESTRICT_EXCHANGE_ITEM:
        if (g_pGameServer->GetServerType() == BATTLE_SERVER)
        {   // 해킹 유저임!!!! 제제 조치가 필요함!
            return false;
        }
        break;
    }
    return g_FunctionalRestrictMgr.IsRestrictFunction(type);
}

void ItemManager::BindGuildWarehouseContainer(
    CGuildWarehouseSlotContainer* guild_warehouse_container)
{
    // TODO: waverix, added acceptable slotcontainer filter
    slot_manager_->BindGuildWarehouseContainer(guild_warehouse_container);
}

void ItemManager::UnbindGuildWarehouseContainer()
{
    // TODO: waverix, added acceptable slotcontainer filter
    slot_manager_->UnbindGuildWarehouseContainer();
}

//==================================================================================================
//== the root of the item update routine. the real processing worked in ItemManagerUpdaterImpl
//==================================================================================================

void ItemManager::Update()
{
    updater_impl_._UpdateExpirationItem();
    updater_impl_._UpdateEtcItem();
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    updater_impl_.UpdateImmediatelyUseItem();
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT

void ItemManager::UpdateItemAttributes(bool recalced_equip_items,
                                       bool recalced_inven_option_items)
{
    if (recalced_equip_items == false && recalced_inven_option_items == false) {
        assert(!"check! unexpected command");
        return;
    };

    ns_object::ChangedValuesNotifier notifier(player_);

    if (recalced_equip_items)
    {
        CEquipmentSlotContainer* const equip_container =
            static_cast<CEquipmentSlotContainer*>(GetItemSlotContainer(SI_EQUIPMENT));
        equip_container->CheckupActiveEquipItemAll();
    };

    if (recalced_inven_option_items) {
        CInventorySlotContainer* const inventory =
            static_cast<CInventorySlotContainer*>(GetItemSlotContainer(SI_INVENTORY));
        inventory->UpdateAttr();
    };
}

// CHANGES: f110422.2L, refresh CEquipmentSlotContainer::equip_existences_
void ItemManager::RefreshEquipItemsCache()
{
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(GetItemSlotContainer(SI_EQUIPMENT));
    equip_container->RefreshEquipItemsCache();
}

//==================================================================================================
#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================

void ItemManager::UpdateAttrInventory() // Inventory에 대해서만 UpdateAttr수행
{
    CInventorySlotContainer* const inventory =
        static_cast<CInventorySlotContainer*>(GetItemSlotContainer(SI_INVENTORY));
    inventory->UpdateAttr();
}

void ItemManager::UpdateAttrForEquipItem()
{
    DWORD maxHP = player_->GetMaxHP();
    DWORD maxMP = player_->GetMaxMP();
    DWORD maxSD = player_->GetMaxSD();

    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(GetItemSlotContainer(SI_EQUIPMENT));
    equip_container->CheckupActiveEquipItemAll();

    if (maxHP != player_->GetMaxHP()) {
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    };
    if (maxMP != player_->GetMaxMP()) {
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
    };
    if (maxSD != player_->GetMaxSD()) {
        player_->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);   
    }
}
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
//==================================================================================================

RC::eITEM_RESULT
ItemManager::DropMoney( MONEY Money )
{
	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;


	if( Money > player_->GetMoney() )
	{
		return RC::RC_ITEM_HAVENOTMONEY;
	}

	WzVector wvCurPos = player_->GetRandomPosInAround(2);
	if( !g_DropManager.DropMoneyToField( player_->GetField(), &wvCurPos, 0, 0, Money ) )
	{
		return RC::RC_ITEM_ERRORDROPMONEY;
	}

//	player_->SetMoney( player_->GetMoney() - Money );
	player_->MinusMoney( Money );	// SetMoney로 돈 쓰는 부분 전부 MinusMoney()로 교체중(현창)

	return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT
ItemManager::Drop( SLOTIDX atIndex, POSTYPE atPos, DWORD dwOwnerKey )
{
	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;

	if( !ValidPos( atIndex, atPos ) )
		return RC::RC_ITEM_INVALIDPOS;

	SCSlotContainer * pAtContainer = GetItemSlotContainer( atIndex );
	SCItemSlot & rSlot = (SCItemSlot &)pAtContainer->GetSlot(atPos);

    // (CHANGES) protect invalid item operation
    // (NOTE) 우선 from만 검사하기로 한다.
    const nsSlot::ItemRuleInfo& from_item_rule_info = rSlot.GetItemRuleInfo();
    if(from_item_rule_info.is_valid_item == 0)
        return RC::RC_ITEM_INVALIDPOS;
	// 드랍 불가능인가?

    if (rSlot.IsPossessionItem() && rSlot.GetIdentifyPossession() && (!rSlot.IsDestroyItem())) {
        return RC::RC_ITEM_CANNOT_DROP_ITEM;
    }
    if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
        return RC::RC_ITEM_CANNOT_DROP_ITEM;
    }

	SCItemSlot DropItemSlotOut;
	if( !pAtContainer->DeleteSlot( atPos, &DropItemSlotOut ) )
		return RC::RC_ITEM_CANNNOT_DROPSTATE;

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    PlayerSpaInfo* spa_info = player_->GetPlayerSpaInfo();
    spa_info->CheckAndResetSpaTicket(atPos);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	WzVector wvCurPos = player_->GetRandomPosInAround(2);
	if( !g_DropManager.DropItemToField( player_->GetField(), &wvCurPos, dwOwnerKey, 0, DropItemSlotOut, 0 ) )
	{
		pAtContainer->InsertSlot( atPos, DropItemSlotOut );
		return RC::RC_ITEM_INVALIDPOSFORDROPITEM;
	}

	// 게임 로그
	GAMELOG->LogItem( ITEM_DROP, player_, &DropItemSlotOut, 0, DropItemSlotOut.GetNum() ); 

	return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT
ItemManager::Pick( POSTYPE NumberOfItems, SCItemSlot & rItemSlot, INVENTORY_TOTAL_INFO * OUT pTotalInfo )
{
	DURATYPE Num = rItemSlot.GetNum();
	rItemSlot.SetNum(1);
	RC::eITEM_RESULT rt = Obtain( rItemSlot, NumberOfItems*Num, pTotalInfo );

	if( rt == RC::RC_ITEM_SUCCESS )
	{
		// 아이템 줍기 로그
		GAMELOG->LogItem( ITEM_PICK, player_, &rItemSlot, 0, NumberOfItems*Num );
		// 아이템 줍기 로그 끝

#ifdef _NA_001990_ACSYSTEM_ADD
        const BASE_ITEMINFO* const pInfo = rItemSlot.GetItemInfo();
        if(pInfo->m_wACCode)
        {
            player_->GetACManager()->ProcessItemObtain(pInfo->m_Code, pInfo->m_wACCode, 
                pInfo->m_byACReferenceID, NumberOfItems*Num);
        }
#endif

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        if (rItemSlot.GetSerial() == 0 &&   // 시리얼이 0 이면 몬스터로부터 드랍된 아이템
            NotificationInfoParser::Instance()->IsNotificationItem(
            rItemSlot, eNotificationDrop) == true)
        {
            MSG_DG_NOTIFICATION_ITEM_SYN msg;
            msg.info.SetInfo(player_->GetCharName(),
                (player_->GetGameZonePtr() != NULL) ? player_->GetGameZonePtr()->GetMapCode() : 0,
                GetTickCount(), eNotificationDrop, rItemSlot.GetItemCode());
            g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
        }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	}

	return rt;
}


// GameField에 있는 아이템을 집었다.
// 아이템을 인벤토리의 빈공간 혹은 임시 인벤토리에 찾아서 넣어준다.
// 겹칠 수 있는 아이템은 겹치게 처리하고 ACK를 보낸다.
// 겹치지 않는 아이템은 비동기적으로 DBP로 부터 시리얼을 발급받고 ACK를 보낸다.
RC::eITEM_RESULT
ItemManager::Pick( DWORD dwItemObjectKey, INVENTORY_TOTAL_INFO * OUT pTotalInfo )
{	
	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;
	if( !ValidContainer(SI_INVENTORY) ) 
		return RC::RC_ITEM_INVALIDSTATE;

	GameField *pField = player_->GetField();
	if( !pField || !pField->IsReady() )
		return RC::RC_ITEM_INVALIDSTATE;

    // CHANGES: f100503.3L, change the interface usage
    // f110415.6L, changes using method to find simple. the internal process of SearchNeighborItem
    // brutely has the object searching algorithm for each items.
    Item* pItem = NULL;
    ;{
        Object* object = pField->FindObject(ITEM_OBJECT, dwItemObjectKey);
        if (object && object->IsEqualObjectKind(ITEM_OBJECT) &&
            object->IsDeleteReserved() == false)
        {
            pItem = static_cast<Item*>(object);
        }
    };
#if SUN_CODE_BACKUP
    Item* pItem = pField->SearchNeighborItem(player_->GetSectorIndex(), dwItemObjectKey);
#endif
	if( !pItem)
	{
		return RC::RC_ITEM_NOTEXISTITEMATFIELD;
	}

	// 이 아이템을 주을 수 있는지 체크
	RC::eITEM_RESULT rt = CanPick( pItem );
	if( rt != RC::RC_ITEM_SUCCESS )
	{
		return rt;
	}

	SCItemSlot ItemSlot = pItem->GetItemSlot();
	DURATYPE num = 1;

//#ifdef _DEBUG
//	테스트 용
//	switch( rItemSlot.GetItemInfo()->m_ChargeSubType )
//	{
//	case eCHARGE_SUB_FIXED_AMOUNT:
//	case eCHARGE_SUB_FIXED_AMOUNT_EX:
//	case eCHARGE_SUB_FIXED_QUANTITY:
//		{
//			// 30분적용
//			rItemSlot.SetDateTime( 30*60 );
//		}break;
//	}
//#endif

	rt = Pick( num, ItemSlot, pTotalInfo );
	if( rt == RC::RC_ITEM_SUCCESS )
	{
		// 몬스터로부터 얻은 아이템을 처음 습득시 주변 파티원들에게도 아이템을 분배한다.
		if( pItem->GetFromMonsterKey() != 0 )
		{	
			g_ItemExpDistributor.DistributeItem( pItem, player_ );
		}

		pField->DestroyObject( pItem );
	}
	return rt;
}


#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
//파티원의 아이템을 대신 집어 줄 수 있다
RC::eITEM_RESULT
ItemManager::PickParty( DWORD dwItemObjectKey, const SERVER_COMMON_OPTION& ServerOption )
{	
    if( !ValidState() ) 
        return RC::RC_ITEM_INVALIDSTATE;
    if( !ValidContainer(SI_INVENTORY) ) 
        return RC::RC_ITEM_INVALIDSTATE;

    GameField *pField = player_->GetField();
    if( !pField || !pField->IsReady() )
        return RC::RC_ITEM_INVALIDSTATE;

    // CHANGES: f100503.3L, change the interface usage
    // f110415.6L, changes using method to find simple. the internal process of SearchNeighborItem
    // brutely has the object searching algorithm for each items.
    Item* pItem = NULL;
    ;{
        Object* object = pField->FindObject(ITEM_OBJECT, dwItemObjectKey);
        if (object && object->IsEqualObjectKind(ITEM_OBJECT) &&
            object->IsDeleteReserved() == false)
        {
            pItem = static_cast<Item*>(object);
        }
    };

    if( !pItem)
    {
        return RC::RC_ITEM_NOTEXISTITEMATFIELD;
    }

    // 이 아이템을 주을 수 있는지 체크
    RC::eITEM_RESULT rt = CanPick( pItem );
    if( rt != RC::RC_ITEM_SUCCESS )
    {
        return rt;
    }

    //
    Player* item_owner = player_;

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    ;{
        NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
        DWORD item_owner_key = pItem->GetOwnerPlayerKey();

        if (pItem->IsEqualObjectKind(ITEM_OBJECT))
        {
            // 아이템의 소유권이 파티원에게 있을 경우
            WORD party_key = player_->GetPartyState().GetPartyKey();
            if(Player* party_member = party_manager->FindMemberPlayer(party_key, item_owner_key))
            {
                eITEM_DISTRIBUTION_TYPE party_type = \
                    static_cast<eITEM_DISTRIBUTION_TYPE>(party_manager->GetDistributionType(party_key));

                if( party_type != eITEM_DISTRIBUTION_DEFAULT &&
                    party_type != eITEM_DISTRIBUTION_DICE_AND_DEFAULT )
                {
                    item_owner = party_member; // 파티원의 아이템을 대신 루팅해 줄수 있다.
                }

                //
                if( pItem->GetItemSlot().GetDiceCastType() )
                {
                    MSG_CG_PARTY_DICECAST_RESULT_CMD msg;
                    {
                        msg.item_object_key = 0;
                        msg.winner_key = 0;
                        memset(msg.result, 0, sizeof(msg.result));
                    };

                    DWORD winner_key = party_manager->GetDiceCastDistributionMember(player_, party_key, msg.result);
                    Player* winner_player = party_manager->FindMemberPlayer(party_key, winner_key);
                    if(winner_player)
                    {
                        // 주사위 결과를 알려줌
                        msg.item_object_key = pItem->GetObjectKey();
                        msg.item_code = pItem->GetItemSlot().GetItemInfo()->m_Code;
                        msg.winner_key = winner_key;
                        party_manager->SendPacketAllMember(party_key, &msg, sizeof(msg));

                        // 한번 주사위 굴린 아이템은 일반 아이템으로 전환.
                        pItem->GetItemSlot().SetDiceCastType(FALSE);
                        // 아이템의 소유권은 주사위 결과 1등한 유저가 갖음.
                        pItem->SetOwnerPlayerKey(winner_key);

                        item_owner = winner_player;
                    }
                }
            }
        }
    };
#else
    if (pItem->IsEqualObjectKind(ITEM_OBJECT) && (player_->GetObjectKey() != pItem->GetOwnerPlayerKey()))
    {
        WORD PartyKey = player_->GetPartyState().GetPartyKey();
        if( PartyKey )
        {
            if(NewGamePartyManager::Instance()->GetDistributionType(PartyKey) != eITEM_DISTRIBUTION_DEFAULT)
            {
                Player *pMember = NewGamePartyManager::Instance()->FindMemberPlayer( PartyKey, pItem->GetOwnerPlayerKey() );
                if( pMember )
                {
                    item_owner = pMember; // 파티원의 아이템을 대신 루팅해 줄수 있다. arycoat 2011.6.28 #2946 아이템획득 방식 변경
                }
            }
        }
    }
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

    MSG_CG_ITEM_PICK_ACK msgACK;
    // NOTE: f110715.1L, Copied because the 'Pick' operation has SCItemSlot modification side-effect
    SCItemSlot ItemSlot = pItem->GetItemSlot();
    DURATYPE num = 1;

    if(item_owner == player_) {
        rt = Pick(num, ItemSlot, &msgACK.m_ItemInfo);
    }
    else {
        if( item_owner->IsDead() )
            return RC::RC_ITEM_DEAD_OWNER;

        GameField* pOwnerField = item_owner->GetField();
        GameField* pMemberField	= player_->GetField();
        if ((pOwnerField == NULL) || (pMemberField == NULL) || (pOwnerField != pMemberField)) {
            return RC::RC_ITEM_DEAD_OWNER;
        };
        //
        rt = item_owner->GetItemManager()->Pick(num, ItemSlot, &msgACK.m_ItemInfo);
    }

    if( rt == RC::RC_ITEM_SUCCESS )
    {
        // 몬스터로부터 얻은 아이템을 처음 습득시 주변 파티원들에게도 아이템을 분배한다.
        if( pItem->GetFromMonsterKey() != 0 )
        {	
            g_ItemExpDistributor.DistributeItem( pItem, item_owner );
        }

        pField->DestroyObject( pItem );

        //
        item_owner->SendPacket(&msgACK, msgACK.GetSize());

        const sPOLICY_CHARACTER& rPolicyment = item_owner->Policyment;
        const BOOLEAN isBrdApply = !(rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD_PLAYER);
        if(isBrdApply) {
            // 아이템 습득을 주변에 알린다.
            MSG_CG_ITEM_PICK_BRD msgBRD;
            msgBRD.m_dwObjKey = item_owner->GetObjectKey();
            msgBRD.m_ItemCode = msgACK.m_ItemInfo.m_Slot[0].m_Stream.Part.wCode;
            if(ServerOption.m_bItemPickUpMsgBrd != 0) {
                item_owner->SendPacketAround(&msgBRD, sizeof(msgBRD), FALSE);
            }

            if(ServerOption.m_bItemPickUpPartyMsgBrd != 0) {
                WORD party_key = item_owner->GetPartyState().GetPartyKey();
                if(party_key != 0) {
                    NewGamePartyManager* pPartyManager = NewGamePartyManager::Instance();
                    pPartyManager->SendPacketAllMember(party_key, &msgBRD, sizeof(msgBRD));
                }
            }
        }
    }

    return rt;
}
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE


RC::eITEM_RESULT
ItemManager::CanPick( Item *pItem )
{
    if(pItem->IsDeleteReserved())
        return RC::RC_ITEM_NOTEXISTITEMATFIELD;
	if( player_->GetGMStateInfo().m_TransOn )
		return RC::RC_ITEM_CANNOT_PICKUP_OBSERVER;

	//현재 클라이언트에서 7m로 되어있음
	// 플레이어와 Item의 거리가 멀다면 주울 수 없다.
	WzVector posOfItem;
	WzVector posOfPlayer;
	pItem->GetPos( &posOfItem );
	player_->GetPos( &posOfPlayer );

	float fItemLen = Math_Vector3DLength( posOfPlayer, posOfItem );
	if( DISTANCE_SPACE_GET_ITEM < fItemLen )
	{
		return RC::RC_ITEM_CANT_PICKABLE_BY_LENGTH;
	}

	// 소유권을 체크한다.
	if( pItem->GetOwnerPlayerKey() )
	{
		WORD PartyKey = player_->GetPartyState().GetPartyKey();
		if( PartyKey )
		{
			Player *pMember = NewGamePartyManager::Instance()->FindMemberPlayer( PartyKey, pItem->GetOwnerPlayerKey() );

			// 아이템의 소유권자가 같은 파티원인지 체크
			if( !pMember )
			{
				return RC::RC_ITEM_DONOT_HAVE_OWNERSHIP;
			}

#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
            BOOL bPartyMemberCanPickItem = FALSE;
            if(pItem->IsEqualObjectKind(ITEM_OBJECT))
            {
                // 파티원의 아이템을 대신 루팅해 줄수 있다. arycoat 2011.6.28 #2946 아이템획득 방식 변경
                bPartyMemberCanPickItem = TRUE; 
#if SUN_CODE_BACKUP
                const BASE_ITEMINFO* ItemInfo = pItem->GetItemSlot().GetItemInfo();
                if(ItemInfo)
                {
                    const eITEMTYPE type = static_cast<eITEMTYPE>(ItemInfo->m_wType);
                    if (type == eITEMTYPE_QUEST ||
                        type == eITEMTYPE_AC || 
                        type == eITEMTYPE_EVENT_FIELD_PARTY_ALL_MEMBER_PICK_UP)
                    {
                        bPartyMemberCanPickItem = TRUE;
                    }
                }
#endif //SUN_CODE_BACKUP
            }
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
            // changed rule by _NA_0_20101206_PARTY_ITEM_DISTRIBUTE_CHANGE
            //--------------------------------------------------------------------------------------
            // CHANGES: f110413.2L, solve the version control
            // because nested macros containing the below code block are shadowy.
            // 하임은 모든 파티원이 획득할 수 있다. - arycoat 2011.03.24
            const bool result_of_money_distribute_method = !pItem->IsEqualObjectKind(MONEY_OBJECT);
        #ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
            const bool result_of_item_routing_method = (bPartyMemberCanPickItem == false);
        #else
            const bool result_of_item_routing_method = true;
        #endif
            if ((result_of_money_distribute_method) &&
                (result_of_item_routing_method) &&
                (NewGamePartyManager::Instance()->GetDistributionType(PartyKey) !=
                    eITEM_DISTRIBUTION_DEFAULT))
            {
                if (player_->GetObjectKey() != pItem->GetOwnerPlayerKey()) {
                    return RC::RC_ITEM_DONOT_HAVE_OWNERSHIP;
                }
            }
            //--------------------------------------------------------------------------------------
		}
		else // if PartyKey is zero...
		{
			if( player_->GetObjectKey() != pItem->GetOwnerPlayerKey() )
			{
				return RC::RC_ITEM_DONOT_HAVE_OWNERSHIP;
			}
		}
	}

	//아이템은 줍는 확률이라는게 존재하지 않는다.
	//떨어져 있는 아이템이 있으면, 줍을수 있냐 없냐 뿐이다.
	if( pItem->GetFromMonsterKey() != 0 )
	{
		BOOL bItemPicUp = player_->GetFatigueManager()->GetItemPickUp();
		if( bItemPicUp == 0 )
			return RC::RC_ITEM_OVER_PLAYING_TIME;
	}

	return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT ItemManager::Combine(const SLOTIDX from_slot_type, 
                                      const POSTYPE from_slot_position, 
                                      const SLOTIDX to_slot_type, 
                                      const POSTYPE to_slot_position, 
                                      DURATYPE& INOUT combine_quantity)
{
    if (IsRestrictFunction(eFUNCTIONALRESTRICT_COMBINE_ITEM))
    {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
        
    if (!ValidState())
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    if (!ValidPos(from_slot_type, from_slot_position))
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
        
    if (!ValidPos(to_slot_type, to_slot_position))
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
        
    // 인벤토리가 아니면 combine할 수 없다.
    // 창고는 가능하게...
    if ((from_slot_type != SI_INVENTORY && from_slot_type != SI_WAREHOUSE) || 
        (to_slot_type != SI_INVENTORY && to_slot_type != SI_WAREHOUSE))
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    // 각종 예외 처리
    if (from_slot_type == to_slot_type && from_slot_position == to_slot_position)
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
        
    SCSlotContainer* const from_item_slot_container = GetItemSlotContainer(from_slot_type);
    SCSlotContainer* const to_item_slot_container = GetItemSlotContainer(to_slot_type);

    SCItemSlot& from_item_slot = 
        static_cast<SCItemSlot&>(from_item_slot_container->GetSlot(from_slot_position));
    SCItemSlot& to_item_slot = 
        static_cast<SCItemSlot&>(to_item_slot_container->GetSlot(to_slot_position));

    if (from_item_slot.GetItemCode() != to_item_slot.GetItemCode())
    {
        return RC::RC_ITEM_ITEMCODENOTEQUAL;
    }

    if (from_item_slot.GetMaxNum() <= 1) // || (from_item_slot.GetItemInfo() && from_item_slot.GetItemInfo()->m_wType != eITEMTYPE_WASTE ) )
    {
        return RC::RC_ITEM_ITEMCODENOTEQUAL;
    }
    // NOTE: f100909.1L, it is a different item \
    // even if the item code of a tagged item is equal to it of a not tagged item.
    if (from_item_slot.GetOptionTag().value != to_item_slot.GetOptionTag().value) {
        return RC::RC_ITEM_ITEMCODENOTEQUAL;
    };        
    const WORD number_of_max_combine = from_item_slot.GetMaxNum();
    if (number_of_max_combine > 0xFF)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }

    // 요청한 개수가 실제로 존재하는지 검사한다.
    if (combine_quantity > from_item_slot.GetNum())
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|이동하려는 개수가 실재 존재하는 양보다 많습니다.|출발슬롯 = %d, 출발슬롯개수 = %d, 이동개수 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               from_slot_position, 
               from_item_slot.GetNum(), 
               combine_quantity, 
               player_->GetCharName());
        return RC::RC_ITEM_INVALIDSTATE;
    }
        
    const bool is_divide_combine = (combine_quantity > 0 && combine_quantity != from_item_slot.GetNum());
    const WORD number_of_combine = 
        (is_divide_combine ? combine_quantity : from_item_slot.GetNum()) + to_item_slot.GetNum();

    // 아이템을 분할하여 합친다면
    if (is_divide_combine)
    {
        DURATYPE new_combine_quantity = combine_quantity;
        
        // 합칠 개수가 최대 겹치기 개수를 초과한다면
        if (number_of_combine > number_of_max_combine)
        {
            // 최대 겹치기 개수까지만 합친다.
            new_combine_quantity = number_of_max_combine - to_item_slot.GetNum();
        }
        
        // 로그를 기록한다.
        GAMELOG->LogItemEx(ITEM_COMBINE, 
                           player_, 
                           to_item_slot, 
                           from_item_slot, 
                           0, 
                           new_combine_quantity);

        // 개수를 변경한다.
        from_item_slot_container->UpdateSlot(from_slot_position, -new_combine_quantity);
        to_item_slot_container->UpdateSlot(to_slot_position, new_combine_quantity);

        // 변경된 개수를 설정한다.
        combine_quantity = to_item_slot.GetNum();
    }
    // 분할이 아니라면 이전과 동일한 방식으로 합친다.
    else
    {
        if (number_of_combine > number_of_max_combine)
        {
            GAMELOG->LogItemEx(ITEM_COMBINE, 
                               player_, 
                               to_item_slot, 
                               from_item_slot, 
                               0, 
                               static_cast<BYTE>(number_of_max_combine));

            // to from 개수 변경한다.
            from_item_slot_container->UpdateSlot(from_slot_position, number_of_combine - number_of_max_combine - from_item_slot.GetNum());

            // to slot 개수 변경한다.
            to_item_slot_container->UpdateSlot(to_slot_position, number_of_max_combine - to_item_slot.GetNum());
        }
        else
        {
            GAMELOG->LogItemEx(ITEM_COMBINE, 
                               player_, 
                               to_item_slot, 
                               from_item_slot, 
                               0, 
                               static_cast<BYTE>(number_of_combine));

            // from slot 지운다.
            from_item_slot_container->DeleteSlot(from_slot_position, NULL);

            // to slot 개수 변경한다.
            to_item_slot_container->UpdateSlot(to_slot_position, number_of_combine - to_item_slot.GetNum());
        }
    }
    
    return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT
ItemManager::Divide( POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum )
{
	if( IsRestrictFunction( eFUNCTIONALRESTRICT_DIVIDE_ITEM ) )
		return RC::RC_ITEM_UNUSABLE_FUNCTION;

	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;

	if( !ValidPos( SI_INVENTORY, fromPos ) )
		return RC::RC_ITEM_INVALIDPOS;
	if( !ValidPos( SI_INVENTORY, toPos, FALSE ) )
		return RC::RC_ITEM_INVALIDPOS;
	if( fromPos == toPos )
		return RC::RC_ITEM_INVALIDPOS;

	SCSlotContainer * pAtContainer	= GetItemSlotContainer( SI_INVENTORY );
	if( !pAtContainer->IsEmpty(toPos) )
		return RC::RC_ITEM_INVALIDPOS;

	SCItemSlot & rFromSlot	= static_cast<SCItemSlot &>(pAtContainer->GetSlot( fromPos ));
	if( fromNum == 0 || toNum == 0 || 
		rFromSlot.GetNum() != (fromNum+toNum) )
		return RC::RC_ITEM_INVALID_VALUE;

	pAtContainer->UpdateSlot( rFromSlot.GetPos(), -toNum );

	// toPos에는 시리얼 발급 처리 -----------------------
    SCItemSlot slot; 
    slot.Copy(rFromSlot);
    slot.SetNum(toNum);
    slot.SetSerial(TEMP_DBSERIAL_VALUE);
	pAtContainer->InsertSlot( toPos, slot );
	//pAtContainer->SetLock( toPos, TRUE );
	//MSG_DG_ITEM_SERIAL_SYN msg;
	//msg.m_dwKey		= player_->GetUserKey();
	//msg.m_SlotIdx	= SI_INVENTORY;
	//msg.m_ItemPos	= toPos;
	//player_->SendToGameDBPServer( &msg, sizeof(msg) );
	//// --------------------------------------------------

	GAMELOG->LogItemEx( ITEM_DIVIDE, player_, rFromSlot, slot, 0, fromNum );

	return RC::RC_ITEM_SUCCESS;
}



RC::eITEM_RESULT
ItemManager::Delete( POSTYPE atPos )
{
	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;

	if( !ValidPos( SI_INVENTORY, atPos ) )
		return RC::RC_ITEM_INVALIDPOS;

	SCSlotContainer * pAtContainer	= GetItemSlotContainer( SI_INVENTORY );
	SCItemSlot & rItemSlot	= (SCItemSlot &)pAtContainer->GetSlot( atPos );
//	BASE_ITEMINFO * pInfo = rItemSlot.GetItemInfo();

    // (CHANGES) change to more generic code 'IsPetLocked' & 'IsRiderLocked' ...
    if(rItemSlot.IsUserBlocked())
        return RC::RC_ITEM_CANNOT_DESTROY_ITEM;
	if( rItemSlot.GetLaxLock()->IsLock() )
		return RC::RC_ITEM_CANNOT_DESTROY_ITEM;

	// 삭제가능인 아이템만 삭제할 수 있다.
    if (rItemSlot.CanTradeSell(eITEM_TRADESELL_DELETE) == false) {
        return RC::RC_ITEM_CANNOT_DESTROY_ITEM;
    }
	// 아이템 삭제 로그
	GAMELOG->LogItem( ITEM_DELETE, player_, &rItemSlot, 0, rItemSlot.GetNum() );

	pAtContainer->DeleteSlot( atPos, NULL );

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    PlayerSpaInfo* spa_info = player_->GetPlayerSpaInfo();
    spa_info->CheckAndResetSpaTicket(atPos);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT                                                // ref) eLOG_ITEM
ItemManager::Delete( SLOTIDX fromIndex, POSTYPE atPos, POSTYPE num, int iLogType )
{
	// 이준석 : 땜빵코드 수정 바람!!!
	if( !player_ ) 
		return RC::RC_ITEM_INVALIDSTATE;

    // (BUGFIX) (091202) (WOPS:5334) 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    TransactionClosure transaction(this, TransactionClosure::eTransType_PKDrop);

    // (CHANGES) (091209) (WAVERIX) 삭제 가능 슬롯이 무결성이 보장되지 않는다, all version supported
#if WAVERIX_CODE_RENEWAL

    if(ValidPos(fromIndex, atPos, true) == false)
        return RC::RC_ITEM_INVALIDPOS;

    SCSlotContainer* pAtContainer = GetItemSlotContainer(fromIndex);
    const SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(pAtContainer->GetSlot(atPos));

    if(rItemSlot.IsContainValidItem() == false)
        return RC::RC_ITEM_INVALIDPOS;

    const BASE_ITEMINFO* pItemInfo = rItemSlot.GetItemInfo(); // available instance in this function

    // 삭제가능인 아이템만 삭제할 수 있다.
    if (rItemSlot.CanTradeSell(eITEM_TRADESELL_DELETE) == false) {
        return RC::RC_ITEM_CANNOT_DESTROY_ITEM;
    }
    // 개수만큼 있는지 체크
    DURATYPE duplicated_num = rItemSlot.GetNum();
    if(duplicated_num < num) {
        return RC::RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT;
    }
    else if(duplicated_num == num) {
        // (BUGFIX) (WAVERIX) 아이템 삭제 처리시 해당 슬롯은 초기화 된다.
        // 잘못된 정보를 기록하는 문제 수정하기 위해 순서 보정
        GAMELOG->LogItem(iLogType, player_, &rItemSlot, 0, num);
        pAtContainer->DeleteSlot(atPos, NULL);
    }
    else {//duplicated_num > num
        pAtContainer->UpdateSlot(atPos, -num);
        GAMELOG->LogItem(iLogType, player_, &rItemSlot, 0, num);
    }

    return RC::RC_ITEM_SUCCESS;

#else //---------------------------------------------------------------------------
	if( !ValidPos( fromIndex, atPos ) )
		return RC::RC_ITEM_INVALIDPOS;

	SCSlotContainer * pAtContainer	= GetItemSlotContainer( fromIndex );
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot( atPos );

	// 삭제가능인 아이템만 삭제할 수 있다.
	if( rItemSlot.GetItemInfo()->m_wTradeSellType & eITEM_TRADESELL_DELETE )
		return RC::RC_ITEM_CANNOT_DESTROY_ITEM;

	// 개수만큼 있는지 체크
	if( rItemSlot.GetNum() < num )
		return RC::RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT;
	else if( rItemSlot.GetNum() == num )
		pAtContainer->DeleteSlot(atPos, NULL);
	else
	{
		pAtContainer->UpdateSlot( rItemSlot.GetPos(), -num );
	}

	// 아이템 삭제 로그
	GAMELOG->LogItem( iLogType, player_, &rItemSlot, 0, num );

	return RC::RC_ITEM_SUCCESS;
#endif //!WAVERIX_CODE_RENEWAL
}

// 이것은 현재 사용하지 않는 함수 (앞으로는?)
RC::eITEM_RESULT
ItemManager::Lose( SLOTCODE ItemCode, BOOL bAllOfItem )
{
	if( !ValidState() ) 
		return RC::RC_ITEM_INVALIDSTATE;

	SCSlotContainer * pInventory = GetItemSlotContainer( SI_INVENTORY );
	for(POSTYPE i = 0 ; i < pInventory->GetMaxSlotNum() ; ++i )
	{
		if( pInventory->IsEmpty(i) ) continue;
		const SCItemSlot & rSlot = static_cast<SCItemSlot&>(pInventory->GetSlot(i));
		if( rSlot.GetItemCode() != ItemCode ) continue;
		pInventory->DeleteSlot(i, NULL);

		// 패킷 보냄 < 변경되었으므로 일단 주석처리
		//MSG_CG_ITEM_LOSE_ACK msg;
		//msg.m_ItemPos = i;
		//player_->SendPacket( &msg, sizeof(msg) );

		if( bAllOfItem ) return RC::RC_ITEM_SUCCESS;
	}

	return RC::RC_ITEM_SUCCESS;
}


// 사용상 주의점!!!
// 반드시 성공하는 인자값만을 넣어야 한다.
// 즉, 유효성 체크를 한 인자만 넣어야 한다.
VOID
ItemManager::Lose( SCSlotContainer * pContainer, POSTYPE pos, int num )
{
	SCItemSlot & rSlotItem = (SCItemSlot &)pContainer->GetSlot(pos);
	if( rSlotItem.GetNum() > num )
	{
		pContainer->UpdateSlot( rSlotItem.GetPos(), -num );
	}
	else
	{
		BOOL rt = pContainer->DeleteSlot( rSlotItem.GetPos(), NULL );
		ASSERT( rt && "현재 슬롯 컨테이너에서 삭제할 수 없는 아이템입니다.\n" );
	}
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

VOID
ItemManager::SendSetItemBrd( BOOL bFullOption )
{
	if( !player_ )		return;

	MSG_CG_ITEM_SETITEM_CHANGE_BRD	Brd;
	Brd.m_PlayerKey = (PLAYERKEY)player_->GetObjectKey();
	Brd.m_bFull = bFullOption;
	player_->SendPacketAround( &Brd, sizeof(Brd), FALSE );
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

