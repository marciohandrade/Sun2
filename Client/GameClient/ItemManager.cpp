//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "itemmanager.h"
#include <scslotHeader.h>
#include <SCSlot.h>
#include <SCSlotContainer.h>
#include "QuickSlot.h"
#include "StyleSlot.h"
#include "MouseHandler.h"
#include "EquipmentContainer.h"
#include <ItemInfoParser.h>
#include <RandomItemParser.h>
#include "Hero.h"
#include "InventoryDialog.h"
#include "ShopDialog.h"
#include "InterfaceManager.h"
#include "Item.h"
#include "ObjectManager.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiGuildMan/uiGuildMan_def.h"
    #include "GuildSystem/uiGuildWareHouse.h"
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "EquipmentContainer.h"
#include "DummySlotContainer.h"
#include "DummyItemSlot.h"

#include "ItemCompositeParser.h"
#include <resultCode.h>
#include "ChatDialog.h"
#include "StyleContainer.h"
#include "SkillInfoParser.h"
#include "StyleInfoParser.h"
#include "ShopInfoParser.h"
#include "HeroActionInput.h"
#include "uiTrade/uiTrade.h"
#include "SCSkillSlot.h"
#include "QuickContainer.h"
#include "SlotKeyGenerator.h"
#include "ItemFormula.h"
#include "PlayerAttributes.h"
#include "SoundEffect.h"
#include "ItemSoundInfoParser.h"
#include "EventInventoryDialog.h"
#ifdef __NA_001358_ENCHANT_UNIFICATION
#else //__NA_001358_ENCHANT_UNIFICATION
#include "EnchantDialog2.h"
#endif //__NA_001358_ENCHANT_UNIFICATION

#include "ItemAttrCalculator.h"
#include "Application.h"
#include "uiHelpMan/uiHelpMan.h"
//#include "uiItemComposite/uiItemComposite.h"
#include "ItemETCCompositeDlg.h"
#include "uiitemtotalcomposite/uiItemTotalComposite.h"
#include "CompetitionScene.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiQuestMan/uiQuestMan.h"
#include "HeroActionInput.h"
#include "uiCashItemShopExtensionBuy/uiCashItemShopExtensionBuy.h"
#include "LotteryInventoryDialog.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "RiderInventoryDialog.h"
#include "uiTradeMan/uiTradeMan_def.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "ItemTypeList.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiCharaterAccumulatePointDialog/uiCharaterAccumulatePointDialog.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
    #include "Interface/uiAutobot/uiAutobotPlay.h"
    #include "Interface/uiAutobot/uiAutobotConditionBattle.h"
#endif //_YMS_AUTOBOT_GAME

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#include "uiSkillMan/uiNewSkillMan.h"
#include "uiPerkDialog/uiPerkDialog.h"

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoNumInput/uiLottoNumInput.h"
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET

#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"

#include "PacketRequestManager.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#ifdef _NA_002935_20110704_ITEM_SCORE
#include "HeroEquipmentContainer.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
  #include "uiPartyMan/uiPartyMan.h"
  #include "AppearanceManager.h"
  #include "DiceCastManager.h"
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "CurrencyInfoParser.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogReply.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningMan.h"
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningDialog.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionMan.h"
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionDialog.h"
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#define MODFLOAT(ratio) ((ratio)<0 ? (-0.5f):(0.5f)) //(클라&서버간 리밋,페이트 수치동기화(반올림적용))
MSG_CG_ITEM_SELL_SYN g_ItemSellSyn;

using namespace RC;

BOOL IsEnablePickupDistance(WzVector &vItemPos)
{
	WzVector wzVec1 = vItemPos - g_pHero->GetPosition();
	float fLength = VectorLength(&wzVec1);

	if (fLength <= DISTANCE_SPACE_GET_ITEM)
	{
		return TRUE;
	}
	return FALSE;
}

// eATTR_AUTO_ITEM_PICK_UP 의 값이 1~99 이면 하임줍기 [11/10/2010 iforall]
// eATTR_AUTO_ITEM_PICK_UP 의 값이 100~ 이면 아이템줍기
// 기존에는 하임줍기로만 썼으나 100이상의 값일 경우 아이템줍기로 동작한다.
// 아이템주기를 100이상의 값으로 설정한 이유는 
// 하임줍기옵션(값:1) 이 아이템/버프 등으로 여러번 중첩되면 1이상의 값이 될 수 있어,
// 이를 분별하기 위하여 아이템줍기옵션 값은 100으로 정함.
BOOL IsAutoPickupMoney(WzVector& vItemPos)
{
    if (g_pHero == NULL)
    {
        return FALSE;
    }

    if (IsEnablePickupDistance(vItemPos) == FALSE)
    {
        return FALSE;
    }

    if (g_pHero->GetPlayerAttribute()->GetAutoPickup() > 0)
    {
        g_pHero->SetPetEmotion(E_AUTO_PICK_ITEM);
        return TRUE;
    }

    return FALSE;
}

BOOL IsAutoPickupItem(WzVector& vItemPos)
{
    if (g_pHero == NULL)
    {
        return FALSE;
    }
	
    if (IsEnablePickupDistance(vItemPos) == FALSE)
    {
        return FALSE;
    }
			
	if (g_pHero->GetPlayerAttribute()->GetAutoPickup() >= 100)
	{
        g_pHero->SetPetEmotion(E_AUTO_PICK_ITEM);
        return TRUE;
    }

    return FALSE;
}

ItemManager::ItemManager()
{

}

ItemManager::~ItemManager()
{

	assert(0 == m_UpdateItemSlotMap.size());
	m_UpdateItemSlotMap.clear();

    update_owner_list_.clear();
}

BOOL 
ItemManager::Init()
{
	return TRUE;
}


VOID ItemManager::Release()
{

}


VOID 
ItemManager::Update()
{
}


SCItemSlot* ItemManager::GetInventoryItem( POSTYPE slot_index )
{
    if( slot_index == INVALID_SLOT_POS )
    {
        return NULL;
    }

    if( SCSlotContainer* container = GetContainer( SI_INVENTORY ) )
    {
        return (SCItemSlot*)(&container->GetSlot(slot_index));
    }
    return NULL;
}


SCSlotContainer* ItemManager::GetContainer( SLOTIDX ContainerIndex )
{
	SCSlotContainer * pAtContainer = NULL;

	if ( !g_pHero )
	{
		return NULL;
	}

	switch (ContainerIndex)
	{
	case SI_VENDOR_SELL:
		{
			pAtContainer = GET_DIALOG(IM_VENDOR_MANAGER::VENDOR_SELL)->GetSlotContainer();
		}
		break;
	case SI_INVENTORY:
		{
			InventoryDialog * pDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
			pAtContainer = static_cast<SCSlotContainer * >(pDlg);
		}
		break;
	case SI_EQUIPMENT:
		{
			pAtContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		}
		break;
	case SI_SKILL:
		{
			pAtContainer = g_InterfaceManager.GetUISkillSlotContainer();
		}
		break;
	case SI_WAREHOUSE:
		{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
            uiBankDialog *pBankDlg = GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
            BankDialog *pBankDlg = GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
			pAtContainer = static_cast<SCSlotContainer * >(pBankDlg);
		}
		break;
	case SI_NPCSTORE:
		{
			ShopDialog * pDlg = 
				static_cast<ShopDialog * >( g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP ) );					

			pAtContainer = static_cast<SCSlotContainer * >(pDlg);
		}
		break;
	case SI_QUICK:		
		{
			pAtContainer = g_pHero->GetSlotContainer(SI_QUICK);
		}
		break;

	case SI_TRADE:
		{
			pAtContainer = GET_DIALOG( IM_TRADE_MANAGER::TRADE )->GetSlotContainer();
		}
		break;

	case SI_ENCHANT:
		{
#ifdef __NA_001358_ENCHANT_UNIFICATION
			// no operation
#else //__NA_001358_ENCHANT_UNIFICATION
			EnchantDialog * pDlg = (EnchantDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ENCHANT );
			pAtContainer = pDlg->GetEnchantContainer();
#endif //__NA_001358_ENCHANT_UNIFICATION			
		}
		break;

#ifdef _NA_003966_20111227_ADD_ENCHANT
    case SI_ENCHANT_OPTION:
        {
            EnchantDialog * pDlg = (EnchantDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ENCHANT );
            pAtContainer = pDlg->GetEnchantOptionItemContainer();
        }
        break;
#endif // _NA_003966_20111227_ADD_ENCHANT

	case SI_EXTENSION:
		{

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            //! 연장은 아이템으로 별도 판매
#else

			uiCashItemShopExtensionBuy * pDlg = (uiCashItemShopExtensionBuy *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_CASH_ITEM_SHOP_EXTENSION_BUY );
			pAtContainer = static_cast<SCSlotContainer * >(pDlg);


#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


		}
		break;


	case SI_STYLE:
		{
			StyleContainer * pDlg = (StyleContainer *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_STYLE_CONTAINER );
			pAtContainer = static_cast<SCSlotContainer * >(pDlg);
		}
		break;

	case SI_EVENT_INVENTORY:
		{
			EventInventoryDialog * pDlg = GET_CAST_DIALOG( EventInventoryDialog, IM_INVENTORY_MANAGER::EVENTINVEN_DLG );
			pAtContainer = static_cast<SCSlotContainer * >(pDlg);
		}
		break;

	case SI_ITEM_COMPOSE:
		{
		}
		break;

	case SI_ITEM_ETC_COMPOSE:
		{
			ItemETCCompositeDlg * pDlg = 
				(ItemETCCompositeDlg *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ITEM_ETC_COMPOSE );

			pAtContainer = static_cast<SCSlotContainer * >(pDlg);
		}
		break;

	case SI_GUILDWAREHOUSE:
		{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            DWORD dialog_id = uiGuildWareHouse::kDialog_ID;
#else
            DWORD dialog_id = IM_GUILD_MANAGER::GUILD_WAREHOUSE;
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
			uiGuildWareHouse * pDlg = GET_CAST_DIALOG(uiGuildWareHouse, dialog_id);
			pAtContainer = static_cast<SCSlotContainer*>(pDlg);
		}
		break;

	case SI_LOTTERY:
		{
			LotteryInventoryDialog* pDlg = GET_CAST_DIALOG( LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG );
			pAtContainer = pDlg->GetSlotContainer();
		}
		break;

	case SI_RIDER:
		{
			RiderInventoryDialog *pDlg = GET_CAST_DIALOG( RiderInventoryDialog, IM_INVENTORY_MANAGER::RIDERINVEN_DLG );
			pAtContainer = pDlg->GetSlotContainer();
		}
		break;

#ifdef _YMS_AUTOBOT_GAME
    case SI_AUTOMATIC_INFO:
        {
            uiAutobotConditionBattle* dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
            if (dialog)
            {
                pAtContainer = dialog->GetSlotContainer();
            }
        }
        break;
#endif //_YMS_AUTOBOT_GAME
    case SI_PERK:
        {
            uiPerkDialog* ui_prek_dialog = 
                GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
            if (ui_prek_dialog != NULL)
            {
                pAtContainer = static_cast<SCSlotContainer*>(ui_prek_dialog);
            }            
        }
        break;

    case SI_CUBE:
        {
            uiItemCube* dialog = GET_CAST_DIALOG(uiItemCube, IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE);
            if (dialog)
            {
                pAtContainer = dialog->GetSlotContainer();
            }
        }
        break;

    case SI_SOCKET_COMPOSITE:
        {
            uiSocketComposite* dialog = GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);
            if (dialog)
            {
                pAtContainer = dialog->GetSlotContainer();
            }
        }
        break;
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    case SI_SMARTNPC:
        {
            uiSmartNpcDialogReply* dialog = GET_CAST_DIALOG(uiSmartNpcDialogReply, IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
            if (dialog)
            {
                pAtContainer = dialog->GetSlotContainer();
            }
        }
        break;
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	case SI_EQUIPMENT_AWAKENING:
		{
			uiEquipmentAwakeningDialog* dialog = GET_CAST_DIALOG(uiEquipmentAwakeningDialog, IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG);
			if (dialog)
			{
				pAtContainer = dialog->GetEquipmentWakeupContainer();
			}
		}
		break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	case SI_EQUIPMENT_EVOLUTION:
		{
			uiEquipmentEvolutionDialog* dialog = GET_CAST_DIALOG(uiEquipmentEvolutionDialog, IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG);
			if (dialog)
			{
				pAtContainer = dialog->GetEquipmentEvolutionContainer();
			}
		}
		break;
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	default:
		pAtContainer = NULL;
	}

	return pAtContainer;
}

SolarDialog * 
ItemManager::GetItemContainerDialog(SLOTIDX ContainerIndex)
{
	InterfaceManager::eDIALOG_ID eDialogID;

	switch (ContainerIndex)
	{
	case SI_VENDOR_SELL:
		eDialogID = InterfaceManager::DIALOG_VENDOR_SELL;
		break;
	case SI_EQUIPMENT:
		eDialogID = InterfaceManager::DIALOG_EQUIP;
		break;
	case SI_INVENTORY:
		eDialogID = InterfaceManager::DIALOG_INVENTORY;
		break;
	case SI_QUICK:
		eDialogID = InterfaceManager::DIALOG_QUICK;
		break;
	case SI_STYLE:
		eDialogID = InterfaceManager::DIALOG_STYLE_CONTAINER;
		break;
	case SI_SKILL:
		return g_InterfaceManager.GetUISkillCurrentDialog();
	case SI_WAREHOUSE:
        return GET_DIALOG(IM_BANK_MANAGER::BANK_DIALOG);
		break;
	case SI_NPCSTORE:
		eDialogID = InterfaceManager::DIALOG_SHOP;
		break;	
	case SI_ENCHANT:
    case SI_ENCHANT_OPTION:
		eDialogID = InterfaceManager::DIALOG_ENCHANT;
		break;

	case SI_EXTENSION:
		eDialogID = InterfaceManager::DIALOG_CASH_ITEM_SHOP_EXTENSION_BUY;
		break;

	case SI_TRADE:
		eDialogID = InterfaceManager::DIALOG_TRADE;
		break;

	case SI_EVENT_INVENTORY:
		eDialogID = InterfaceManager::DIALOG_EVENT_INVENTORY;
		break;

	case SI_ITEM_COMPOSE:
		eDialogID = InterfaceManager::DIALOG_ITEM_COMPOSITE;
		break;

	case SI_ITEM_ETC_COMPOSE:
		eDialogID = InterfaceManager::DIALOG_ITEM_ETC_COMPOSE;		
		break;

	case SI_GUILDWAREHOUSE:
        {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            DWORD dialog_id = uiGuildWareHouse::kDialog_ID;
#else
            DWORD dialog_id = IM_GUILD_MANAGER::GUILD_WAREHOUSE;
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            return GET_DIALOG(dialog_id);
        }
        break;

	case SI_LOTTERY:
		{
			return GET_CAST_DIALOG( LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG );
		}
		break;


	case SI_RIDER:	
		{
			return GET_CAST_DIALOG( RiderInventoryDialog, IM_INVENTORY_MANAGER::RIDERINVEN_DLG );
		}
		break;


    case SI_AUTOMATIC_INFO:
        {
#ifdef _YMS_AUTOBOT_GAME
            return GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
#endif //_YMS_AUTOBOT_GAME
        }
        break;

    case SI_CUBE:	
        {
            return GET_CAST_DIALOG(uiItemCube, IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE);
        }
        break;

    case SI_PERK:
        {
            return GET_DIALOG(IM_SKILL_MANAGER::SKILL_PERK_DLG);
        }
        break;

    case SI_SOCKET_COMPOSITE:
        {
            return GET_DIALOG(IM_SOCKET_MANAGER::SOCKET_COMPOSITE);
        }
        break;
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    case SI_SMARTNPC:
        {
            return GET_DIALOG(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
        }
        break;
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	case SI_EQUIPMENT_AWAKENING:
		{
			return GET_DIALOG(IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG);
		}
		break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	case SI_EQUIPMENT_EVOLUTION:
		{
			return GET_DIALOG(IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG);
		}
		break;
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	default:
		eDialogID = InterfaceManager::DIALOG_MAX;
		break;
	}
	return g_InterfaceManager.GetDialog( eDialogID );
}
//=============================================================================================================================
/**
@remarks
비워있는 슬롯위치를 알아내는 함수.

@param	[in] AtSlotIdx
해당 컨테이너 인덱스
[out] POSTYPE & rPosOut
비워있는 슬롯위치
*/
BOOL ItemManager::GetEmptySlotPos(SLOTIDX AtSlotIdx, POSTYPE & OUT rPosOut)
{
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	assert(pContainer);
	if (!pContainer)
	{
		return FALSE;
	}

	return pContainer->GetEmptyPos( rPosOut );
}


//----------------------------------------------------------------------------
/**
@desc
해당 아이템이 위치할수 있는 장착 위치를 넘겨주는 함수

@param 
[in]	rSlot : 해당 아이템 슬롯
[out]	rEqPosOut : 이큅 가능한 위치를 얻어온다. 
*/
BOOL								
ItemManager::GetEquipPosition(SCSlot & rSlot, POSTYPE & OUT rEqPosOut)
{
	assert(rSlot.GetSlotType() == ST_ITEM);		
	if (rSlot.GetSlotType() != ST_ITEM)
	{
		//
		return FALSE;
	}

	SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;

    //슬롯유효검사
    if(!rItemSlot.IsContainValidItem())
    {
        return FALSE;
    }

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if (!pItemInfo)
	{
		return FALSE;
	}

	switch ( pItemInfo->m_wEqPos )
	{
	case eEQUIPCONTAINER_WEAPON:
	case eEQUIPCONTAINER_ARMOR:
	case eEQUIPCONTAINER_PROTECTOR:
	case eEQUIPCONTAINER_HELMET:
	case eEQUIPCONTAINER_PANTS:
	case eEQUIPCONTAINER_BOOTS:
	case eEQUIPCONTAINER_GLOVE:
	case eEQUIPCONTAINER_BELT:
	case eEQUIPCONTAINER_SHIRTS:
	case eEQUIPCONTAINER_NECKLACE:
		rEqPosOut = (POSTYPE)pItemInfo->m_wEqPos;
		break;

	case eEQUIPCONTAINER_SACCESSORY1:
	case eEQUIPCONTAINER_SACCESSORY2:
	case eEQUIPCONTAINER_SACCESSORY3:
		{		
			SCSlotContainer *pContainer = this->GetContainer(SI_EQUIPMENT);
            if (!pContainer)
            {
                return FALSE;
            }

            if (pContainer->IsEmpty(eEQUIPCONTAINER_SACCESSORY1) == TRUE)
            {
                rEqPosOut = eEQUIPCONTAINER_SACCESSORY1;
            }
            else if (pContainer->IsEmpty(eEQUIPCONTAINER_SACCESSORY2) == TRUE)
            {
                rEqPosOut = eEQUIPCONTAINER_SACCESSORY2;
            }
            else    // if (pContainer->IsEmpty(eEQUIPCONTAINER_SACCESSORY3))
            {
                rEqPosOut = eEQUIPCONTAINER_SACCESSORY3;
            }
		}
		break;

	case eEQUIPCONTAINER_RING1:
	case eEQUIPCONTAINER_RING2:
		{
			SCSlotContainer * pContainer = this->GetContainer(SI_EQUIPMENT);
            if (!pContainer)
            {
                return FALSE;
            }

			if ( pContainer->IsEmpty( eEQUIPCONTAINER_RING1 ) )
			{
				rEqPosOut = eEQUIPCONTAINER_RING1;
			}
			else
				if ( pContainer->IsEmpty( eEQUIPCONTAINER_RING2 ) )
				{
					rEqPosOut = eEQUIPCONTAINER_RING2;
				}
				else
				{
					rEqPosOut = eEQUIPCONTAINER_RING1;
				}
		}
		break;
	case eEQUIPCONTAINER_CHARGE1:
	case eEQUIPCONTAINER_CHARGE2:
	case eEQUIPCONTAINER_CHARGE3:
	case eEQUIPCONTAINER_CHARGE4:
	case eEQUIPCONTAINER_CHARGE5:

		{	

			int iExtraChargeItemSlot = 0;
			if( g_pHero && g_pHero->GetCharInfo() )
			{
				iExtraChargeItemSlot = g_pHero->GetCharInfo()->m_ExtraEquipCashSlotCount;
			}

			SCSlotContainer * pContainer = this->GetContainer(SI_EQUIPMENT);

            if (!pContainer)
            {
                return FALSE;
            }

			if ( pContainer->IsEmpty( eEQUIPCONTAINER_CHARGE1 ) )
			{
				rEqPosOut = eEQUIPCONTAINER_CHARGE1;
			}
			else if ( pContainer->IsEmpty( eEQUIPCONTAINER_CHARGE2 ) )
			{
				rEqPosOut = eEQUIPCONTAINER_CHARGE2;
			}
			else if ( pContainer->IsEmpty( eEQUIPCONTAINER_CHARGE3 ) )
			{
				rEqPosOut = eEQUIPCONTAINER_CHARGE3;
			}
			else if( iExtraChargeItemSlot == 2)
			{
				if( pContainer->IsEmpty( eEQUIPCONTAINER_CHARGE4) )
				{
					rEqPosOut = eEQUIPCONTAINER_CHARGE4;
				}
				else if ( pContainer->IsEmpty( eEQUIPCONTAINER_CHARGE5) )
				{
					rEqPosOut = eEQUIPCONTAINER_CHARGE5;
				}
				else
				{
					rEqPosOut = eEQUIPCONTAINER_CHARGE1;
				}

			}
			else
			{
				rEqPosOut = eEQUIPCONTAINER_CHARGE1;
			}
		}
		break;

	case eEQUIPCONTAINER_PC_ROOM1:
	case eEQUIPCONTAINER_PC_ROOM2:
	case eEQUIPCONTAINER_PC_ROOM3:
		{
			SCSlotContainer * pContainer = this->GetContainer(SI_EQUIPMENT);
            if (!pContainer)
            {
                return FALSE;
            }

			bool bAvaliblePcRoomItem = pContainer && g_pHero && g_pHero->GetPCBangMode();
			if( bAvaliblePcRoomItem )
			{
				if( pContainer->IsEmpty(eEQUIPCONTAINER_PC_ROOM1))
				{
					rEqPosOut = eEQUIPCONTAINER_PC_ROOM1;
				}
				else if( pContainer->IsEmpty(eEQUIPCONTAINER_PC_ROOM2))
				{
					rEqPosOut = eEQUIPCONTAINER_PC_ROOM2;
				}
				else if( pContainer->IsEmpty(eEQUIPCONTAINER_PC_ROOM3))
				{
					rEqPosOut = eEQUIPCONTAINER_PC_ROOM3;
				}
				else
				{	
					rEqPosOut = eEQUIPCONTAINER_PC_ROOM1;
				}
			}
			else
			{
				return FALSE;
			}
		}
		break;

	default:
		{
			assert(!"정의 되지않은 위치!!");
			return FALSE;
		}
		break;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 조합 관련해서 Name String ID를 받아온다.
*/
DWORD								
ItemManager::GetCompositeNameID(eITEMCOMPOSITECODE eCode)
{
	DWORD dwStrID = 0;
	switch ( eCode )
	{
	case eITEMCOMPOSITE_LOW_ENCHANT:		
	case eITEMCOMPOSITE_MEDIUM_ENCHANT:
	case eITEMCOMPOSITE_HIGH_ENCHANT:		dwStrID = 744;	break;
	case eITEMCOMPOSITE_RANK_UP:			dwStrID = 763;	break;
	case eITEMCOMPOSITE_RANK_DOWN:			dwStrID = 864;	break;
	case eITEMCOMPOSITE_EXTRACT_SOCKET:		dwStrID = 9528; break;
	case eITEMCOMPOSITE_ADD_SOCKET:			dwStrID = 9523; break;
	case eITEMCOMPOSITE_LOW_RING:			dwStrID = 9504;	break;
	case eITEMCOMPOSITE_HIGH_RING:			dwStrID = 9506; break;
	case eITEMCOMPOSITE_LOW_NECKLACE:		dwStrID = 9505; break;
	case eITEMCOMPOSITE_HIGH_NECKLACE:		dwStrID = 9507; break;
	case eITEMCOPOSITE_PROTECTION_CRYSTAL:	dwStrID = 9502; break;
	case eITEMCOPOSITE_SHANING_CRYSTAL:		dwStrID = 9503; break;
	case eITEMCOPOSITE_SOUL_SHEILD_LOW:		dwStrID = 9517; break;
	case eITEMCOPOSITE_SOUL_SHEILD_HIGH:	dwStrID = 9508; break;
	case eITEMCOPOSITE_SOUL_ARMLET_LOW:		dwStrID = 9520; break;
	case eITEMCOPOSITE_SOUL_ARMLET_HIGH:	dwStrID = 9516; break;
	case eITEMCOPOSITE_SOUL_WING_LOW:		dwStrID = 9518; break;
	case eITEMCOPOSITE_SOUL_WING_HIGH:		dwStrID = 9509; break;
	case eITEMCOPOSITE_SOUL_EYE_LOW:		dwStrID = 9519; break;
	case eITEMCOPOSITE_SOUL_EYE_HIGH:		dwStrID = 9510; break;
	case eITEMCOPOSITE_EXTRASTONE_CREATE:	dwStrID = 9511; break;
	case eITEMCOPOSITE_CRYSTALIZE:			dwStrID = 9500; break;
    case eITEMCOPOSITE_ZARD:                dwStrID = 3412; break;
	case eITEMCOMPOSITE_HEIM_ENCHANT:       dwStrID = 719; break;
	case eITEMCOMPOSITE_CASH_ENCHANT:		dwStrID = 720; break;
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	case eITEMCOMPOSITE_EQUIPMENT_WAKEUP:   dwStrID = 4000; break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	case eITEMCOMPOSITE_EQUIPMENT_EVOLUTION: dwStrID = 3999; break;
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	case eITEMCOMPOSITE_NONE:	
		break;
	}

	return dwStrID;
}

//-------------------------------------------------------------------------------------------
/**	아이템 갯수얻어오기
*/
WORD								
ItemManager::GetItemCount(SLOTIDX AtSlotIdx, CODETYPE ItemCode)
{
    WORD item_count = 0;
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);

	if (slot_container == NULL)
    {
        return item_count;
    }
    
    POSTYPE max_slot = slot_container->GetMaxSlotNum();
	for (POSTYPE slot_index = 0; slot_index < max_slot; ++slot_index)
	{
        if (slot_container->ValidPos(slot_index) == false)
        {
            continue;
        }

        SCSlot& slot_ref = slot_container->GetSlot(slot_index);
		if (slot_ref.GetCode() == ItemCode)
		{
			item_count += slot_ref.GetNum();
		}
	}

	return item_count;
}

//------------------------------------------------------------------------------
WORD ItemManager::GetItemCount(SLOTIDX AtSlotIdx, eITEMTYPE item_type, eWASTETYPE waste_type)
{
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);		
    if (slot_container == NULL)
    {
        return 0;
    }

    WORD item_count = 0;
    
    for (int i = 0; i < slot_container->GetMaxSlotNum(); ++i)
    {
        if ((slot_container->IsEmpty(i) == TRUE) || 
            (slot_container->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot& item_slot = (SCItemSlot&)(slot_container->GetSlot(i));
        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if (item_info != NULL)
        {
            if ((item_info->m_wType == item_type) && 
                (item_info->m_byWasteType == waste_type))
            {
                item_count += item_slot.GetNum();
            }
        }
    }

    return item_count;
}

//----------------------------------------------------------------------------
/** 현재 슬롯위치에 슬롯이 비워있는지 체크.
*/
BOOL								
ItemManager::IsEmptySlotPos(SLOTIDX AtSlotIdx, POSTYPE Pos)
{
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	assert(pContainer);
	if (!pContainer)
	{
		return FALSE;
	}

	return BOOL( pContainer->IsEmpty(Pos) );
}

//----------------------------------------------------------------------------
/**
*/
DWORD								
ItemManager::GetItemSound(eSLOT_SOUND_TYPE eSoundType, SCItemSlot * pItemSlot)
{
	if ( !pItemSlot )
	{
		return DWORD(-1);
	}

	const BASE_ITEMINFO * pItemInfo = pItemSlot->GetItemInfo();
	if ( !pItemInfo )
	{
		return DWORD(-1);
	}

	BASE_ItemSoundInfo * pSoundInfo = ItemSoundInfoParser::Instance()->GetItemSoundInfo( pItemInfo->m_nSoundIndex );

	if ( !pSoundInfo )
	{
		return DWORD(-1);
	}

	switch (eSoundType)
	{
	case eSLOT_SOUND_TYPE_PICK:		return pSoundInfo->m_dw_PickDefault;	break;
	case eSLOT_SOUND_TYPE_PUT:		return pSoundInfo->m_dw_PutDefault;		break;
	case eSLOT_SOUND_TYPE_EQUIP:	return pSoundInfo->m_dw_PutWear;		break;
	case eSLOT_SOUND_TYPE_UNEQUIP:	return pSoundInfo->m_dw_RemoveWear;		break;
	case eSLOT_SOUND_TYPE_LINK:		return pSoundInfo->m_dw_LinkQslot;		break;
	case eSLOT_SOUND_TYPE_UNLINK:	return pSoundInfo->m_dw_UnlinkQslot;	break;
	case eSLOT_SOUND_TYPE_DUMP:		return pSoundInfo->m_dw_Dump;			break;

	default:
		return DWORD(-1);
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
DWORD								
ItemManager::GetSkillSound(eSLOT_SOUND_TYPE eSoundType, SCSkillSlot * pSkillSlot)
{
	if ( !pSkillSlot )
	{
		return DWORD(-1);
	}

	switch (eSoundType)
	{
	case eSLOT_SOUND_TYPE_PICK:		return UI_SOUND_SKILL_ICON_PICK;	break;
	case eSLOT_SOUND_TYPE_PUT:		return UI_SOUND_SKILL_ICON_DROP;	break;
	case eSLOT_SOUND_TYPE_EQUIP:	
	case eSLOT_SOUND_TYPE_UNEQUIP:	
	case eSLOT_SOUND_TYPE_LINK:		return UI_SOUND_SKILL_ICON_DROP;	break;
	case eSLOT_SOUND_TYPE_UNLINK:	
	case eSLOT_SOUND_TYPE_DUMP:
	default:
		return DWORD(-1);
		break;
	}

}

//----------------------------------------------------------------------------
/**
*/
VOID								
ItemManager::PlaySlotSound( eSLOT_SOUND_TYPE eSoundType, SCSlot * pSlot )
{
	if ( !pSlot ) 
	{
		return;
	}

	DWORD dwSoundID = 0;

	eSlotType SlotType = pSlot->GetSlotType();

	switch (SlotType) 
	{
	case ST_ITEM:
		{
			dwSoundID = GetItemSound(eSoundType, (SCItemSlot *)pSlot);
		}
		break;

	case ST_SKILL:
		{
			dwSoundID = GetSkillSound(eSoundType, (SCSkillSlot *)pSlot);
		}
		break;

	case ST_STYLE_QUICK:
		{
            dwSoundID = GetSkillSound(eSoundType, (SCSkillSlot*)(0xFFFFFFFF));
		}
		break;

	case ST_QUICK:
		{
			QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
			switch ( pQuickSlot->GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					dwSoundID = GetItemSound(eSoundType, (SCItemSlot *)pQuickSlot->GetOrgSlot());
				}
				break;

			case ST_SKILL:
				{
					dwSoundID = GetSkillSound(eSoundType, (SCSkillSlot *)pQuickSlot->GetOrgSlot());
				}
				break;
			}
		}
		break;

	case ST_DUMMY_ITEM:
		{
			DummyItemSlot * pDummySlot	= (DummyItemSlot *)pSlot;			
			SCItemSlot *	pItemSlot	= (SCItemSlot *)pDummySlot->GetOrgSlot();
			if ( !pItemSlot )
			{
				dwSoundID = DWORD(-1);
			}
			else
			{
				dwSoundID = GetItemSound(eSoundType, pItemSlot);
			}
		}
		break;

	default:
		break;
	}

	g_SoundEffect.PlayUI( dwSoundID );	
}


//----------------------------------------------------------------------------
/**
*/
CODETYPE 
ItemManager::InsertInventoryTotalInfo( eINSERTTOTALTYPE eType, INVENTORY_TOTAL_INFO & rTotalInfo)
{

	CODETYPE itemcode = 0;

	SCSlotContainer *pContainer = GetContainer(SI_INVENTORY);
	SUN_ASSERT(pContainer); 
    if (!pContainer)
    {
        return itemcode;
    }

	//
	// total info set

	ITEMSLOTEX * pSlot = rTotalInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = rTotalInfo.m_InvenCount;

	SCItemSlot ItemBuf;
	TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];
	TCHAR szItemName[MAX_PATH];
	const BASE_ITEMINFO * pBaseItemInfo = NULL;

    InventoryDialog * pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
    SUN_ASSERT(pInvenDlg);

	// 무조건 서버에서 보내온 정보를 믿고 Insert 시킴. 
	for(POSTYPE i=start;i<total;++i)
	{
		ItemBuf.Clear();
		ItemBuf.Copy( pSlot[i].m_Stream );
		if (pContainer->IsEmpty(pSlot[i].m_Pos))
		{
			pContainer->InsertSlot(pSlot[i].m_Pos, ItemBuf);
		}
		else
		{
			if(pInvenDlg)
				pInvenDlg->SetUpdateQuest(false);

			pContainer->DeleteSlot(pSlot[i].m_Pos, NULL);

			if(pInvenDlg)
				pInvenDlg->SetUpdateQuest(true);

			pContainer->InsertSlot(pSlot[i].m_Pos, ItemBuf);
		}

		pBaseItemInfo = ItemBuf.GetItemInfo();

		if (pBaseItemInfo)
		{
			ZeroMemory(szItemName,sizeof(szItemName));
			GameFunc::GetItemName(&ItemBuf, pBaseItemInfo, szItemName, MAX_PATH);

			itemcode = pBaseItemInfo->m_Code;
		}

		switch ( eType )
		{
		case BUY:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 구매 하였습니다.					
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf( szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}
			}
			break;

		case PICK:
			{

				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 획득 했습니다.					
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_GET_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);		

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType );
					}

					uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

					if (helpMan)
					{
						helpMan->CheckHelpMessage(eHK_ITEM, (SLOTCODE)pBaseItemInfo->m_Code, (BOOL)ItemBuf.IsLimited());

					}
                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem, TotalGuideNotice::kItemCode, pBaseItemInfo->m_Code);
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem, TotalGuideNotice::kItemType, pBaseItemInfo->m_wType);

                        //디바인 or 에테리아 or 스킨 확인
                        if (ItemBuf.IsDivine())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemStatus, TotalGuideNotice::kDivine); //디바인(1)/에테리아(2)/스킨(3)
                        }
                        else if (ItemBuf.IsEtheriaItem())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemStatus, TotalGuideNotice::kEtheriaItem);
                        }
                        else if (ItemBuf.IsShellItem())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemStatus, TotalGuideNotice::kShellItem);
                        }

                        //레어 or 엘리트 확인
                        if ((pBaseItemInfo->m_bUse == true) &&                      // 아이템 네이밍색깔 사용여부
                             (Strcmp(pBaseItemInfo->m_szColor,_T("7875DF")) == 0))  // 레어 색상 : 7875DF
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemRareAndElite, TotalGuideNotice::kRare);  // 레어(1),엘리트(2)
                        }
                        else if (pBaseItemInfo->IsElite())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemRareAndElite, TotalGuideNotice::kElite);  // 레어(1),엘리트(2)
                        }
                    }
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}				
			}
			break;

		case CREATE:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 생성 하였습니다.
					const int	c_iCreateItem = 70270;
					g_InterfaceManager.GetInterfaceString( c_iCreateItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}				
			}
			break;

		case REWARD:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 보상 받았습니다.
					const int	c_iRewardItem = 70271;
					g_InterfaceManager.GetInterfaceString( c_iRewardItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);		

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						g_InterfaceManager.GetInterfaceString( c_iRewardItem, szText, INTERFACE_STRING_LENGTH );
						_stprintf(szMessage, szText, szItemName );
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType);
					}

				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}								
			}
			break;

		case EVENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 이벤트 인벤토리에서 가져 왔습니다.
					g_InterfaceManager.GetInterfaceString( eST_REWARDITEM, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}	
			}
			break;

		case QUEST_OBTAIN:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 보상 받았습니다.
					const int	c_iRewardItem = 70271;
					g_InterfaceManager.GetInterfaceString( c_iRewardItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						g_InterfaceManager.GetInterfaceString( eST_NAMECODE_GET_ITEM, szText, INTERFACE_STRING_LENGTH );
						_stprintf(szMessage, szText, szItemName );
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType );
					}
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}								
			}
			break;
        case POINT_REWARD:
            {
                if ( pBaseItemInfo )
                {
                    //	[%s] 아이템을 보상 받았습니다.
                    g_InterfaceManager.GetInterfaceString( 70271, szText, INTERFACE_STRING_LENGTH );
                    _stprintf(szMessage, szText, szItemName );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);

                    uiCharaterAccumulatePointDialog* accumulatepoint_dialog = GET_CAST_DIALOG(uiCharaterAccumulatePointDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG);
                    if (accumulatepoint_dialog)
                    {
                        accumulatepoint_dialog->InserRewardMessage(szMessage);
                    }
                }
            }
            break;
        case CUBE:
            {
                if (pBaseItemInfo)
                {   // 3408: %s 아이템이 생성되었습니다.
                    g_InterfaceManager.GetInterfaceString(3408, szText, INTERFACE_STRING_LENGTH);
                    _stprintf(szMessage, szText, szItemName);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);
                }
            }
            break;
        case SOCKET_COMPOSTE:
            {
                if (pBaseItemInfo)
                {   // 6493: 축하합니다. %s가 생성되었습니다.
                    g_InterfaceManager.GetInterfaceString(6493, szText, INTERFACE_STRING_LENGTH);
                    _stprintf(szMessage, szText, szItemName);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);

                    uiSocketComposite* dialog = 
                        GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);
                    if (dialog && dialog->IsVisibleWindow())
                    {
                        DummyItemSlot DummyItem;
                        SCItemSlot& from_item_slot = 
                            (SCItemSlot&)pContainer->GetSlot(pSlot[i].m_Pos);

                        if ((from_item_slot.GetItemInfo() != NULL) || 
                            (from_item_slot.GetSlotType() == ST_ITEM))
                        {
                            assert(&from_item_slot);
                            DummyItem.Clear();
                            DummyItem.SetOrgSlot(&from_item_slot);
                            DummyItem.SetFromContainerIdx(SI_INVENTORY);
                            DummyItem.SetFromPosition(pSlot[i].m_Pos);
                            DummyItem.SetSlotType(ST_ITEM);

                            dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING3, szMessage);
                            SCSlotContainer* socket_composite_container_ptr = 
                                ItemManager::Instance()->GetContainer(SI_SOCKET_COMPOSITE);
                            socket_composite_container_ptr->InsertSlot(uiSocketComposite::SOCKET_COMPOSITE_SLOT_RESULT_ITEM,
                                DummyItem);
                        }
                    }
                }
            }
            break;
		}
	}

	// play sound 
    if(eType != POINT_REWARD)
    {
        ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, (SCSlot *)&ItemBuf);
    }

	return itemcode;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::InsertJustInventory(eINSERTTOTALTYPE eType, JUST_INVENTORY_TOTAL_INFO & rTotalInfo)
{
	SCSlotContainer *pContainer = GetContainer(SI_INVENTORY);
	SUN_ASSERT(pContainer); 

	if (!pContainer)
	{
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, _T("Invalid inventory container. Please try reconnect."));		
		return FALSE;
	}

	//
	// total info set

	ITEMSLOTEX * pSlot = rTotalInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = rTotalInfo.m_Count;

	SCItemSlot ItemBuf;
	TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];
	TCHAR szItemName[MAX_PATH];
	const BASE_ITEMINFO * pBaseItemInfo = NULL;

    InventoryDialog * pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
    SUN_ASSERT(pInvenDlg);

	// 무조건 서버에서 보내온 정보를 믿고 Insert 시킴. 
	for(POSTYPE i = start; i < total; ++i)
	{
		ItemBuf.Clear();
		ItemBuf.Copy( pSlot[i].m_Stream );
		if (pContainer->IsEmpty(pSlot[i].m_Pos))
		{
			pContainer->InsertSlot(pSlot[i].m_Pos, ItemBuf);
		}
		else
		{
            if(pInvenDlg)
                pInvenDlg->SetUpdateQuest(false);

            pContainer->DeleteSlot(pSlot[i].m_Pos, NULL);

            if(pInvenDlg)
                pInvenDlg->SetUpdateQuest(true);

            pContainer->InsertSlot(pSlot[i].m_Pos, ItemBuf);
		}

		pBaseItemInfo = ItemBuf.GetItemInfo();

		if (pBaseItemInfo)
		{
			ZeroMemory(szItemName,sizeof(szItemName));
			GameFunc::GetItemName(&ItemBuf, pBaseItemInfo, szItemName, MAX_PATH);
		}

		switch ( eType )
		{
		case BUY:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 구매 하였습니다.					
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf( szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}
			}
			break;

		case PICK:
			{

				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 획득 했습니다.					
					g_InterfaceManager.GetInterfaceString(eST_NAMECODE_GET_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf( szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);		

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType );
					}

					uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

					if (helpMan)
					{

						helpMan->CheckHelpMessage(eHK_ITEM, (SLOTCODE)pBaseItemInfo->m_Code, (BOOL)ItemBuf.IsLimited());
					}
                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem, pBaseItemInfo->GetSubTypeDetail(), pBaseItemInfo->m_Code);
                    }
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}				
			}
			break;

		case CREATE:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 생성 하였습니다.
					const int	c_iCreateItem = 70270;
					g_InterfaceManager.GetInterfaceString( c_iCreateItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}				
			}
			break;

		case REWARD:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 보상 받았습니다.
					const int	c_iRewardItem = 70271;
					g_InterfaceManager.GetInterfaceString( c_iRewardItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);	

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						g_InterfaceManager.GetInterfaceString( eST_NAMECODE_GET_ITEM, szText, INTERFACE_STRING_LENGTH );
						_stprintf(szMessage, szText, szItemName );
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType );
					}
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}								
			}
			break;

		case EVENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 이벤트 인벤토리에서 가져 왔습니다.
					g_InterfaceManager.GetInterfaceString( eST_REWARDITEM, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}	
			}
			break;

		case QUEST_OBTAIN:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 아이템을 보상 받았습니다.
					const int	c_iRewardItem = 70271;
					g_InterfaceManager.GetInterfaceString( c_iRewardItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);			

					if( GAMEOPTION->GetPickUpItemNotify() )
					{
						g_InterfaceManager.GetInterfaceString( eST_NAMECODE_GET_ITEM, szText, INTERFACE_STRING_LENGTH );
						_stprintf(szMessage, szText, szItemName );
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szMessage, eType );
					}
				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}								
			}
			break;

		case PRESENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] 선물을 수령하였습니다.
					g_InterfaceManager.GetInterfaceString( 5978, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);		

				}
				else
				{
					assert(!"아이템 정보가 썩었다!!");
				}	
			}
        case SHOP_STORAGE:
            {
                if ( pBaseItemInfo )
                {
                    //	[%s] 아이템을 구매 하였습니다.					
                    g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
                    _stprintf( szMessage, szText, szItemName );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
                }
                else
                {
                    assert(!"아이템 정보가 썩었다!!");
                }
            }
            break;
		}
	}

	// play sound 
	ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, (SCSlot *)&ItemBuf);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 인벤토리에서 특정 아이템을 제거한다.
*/
BOOL							
ItemManager::DeleteInventoryTotalInfo( eDELETETOTALTYPE eType, SLOTIDX AtSlotIdx, INVEN_DELETE_TOTAL_INFO & rDeleteTotalInfo)
{
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	assert(pContainer);

	if ( !pContainer )
	{
		return FALSE;
	}

	for (int i = 0; i < rDeleteTotalInfo.m_Count; ++i)
	{
		BOOL bDelete = DeleteItem(AtSlotIdx, 
			rDeleteTotalInfo.m_Slot[i].m_Pos, 
			rDeleteTotalInfo.m_Slot[i].m_Num);

		assert(bDelete);	

		if ( !bDelete )
		{
			return FALSE;
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------
/** InsertEventTotalInfo
desc : EventInventory에 아이템을 채워 넣는다.
author : y2jinc
date : 2006. 1. 20
*/
BOOL								
ItemManager::InsertEventTotalInfo( EVENT_ITEM_TOTAL_INFO & rTotalInfo)
{
	SCSlotContainer * pContainer = GetContainer(SI_EVENT_INVENTORY);
	assert(pContainer); 

	BOOL bRet = TRUE;

	//
	// total info set

	EVENTSLOT * pEventSlot = rTotalInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = rTotalInfo.m_Count;

	assert( pContainer->GetMaxSlotNum() >= EVENT_ITEM_TOTAL_INFO::MAX_SLOT_NUM );

	SCItemSlot TempEventItem;

	if ( pContainer )
	{
		for ( int i = 0; i < total; ++i)
		{
			TempEventItem.Clear();

			// 임시적으로 서버 / 클라 모두 이렇게 처리하기로 했다.
			TempEventItem.SetCode(pEventSlot[i].m_ItemCode);
			TempEventItem.SetNum(pEventSlot[i].m_ItemCount);	

			pContainer->InsertSlot( i, TempEventItem );
		}
	}

	return bRet;
}

//----------------------------------------------------------------------
/** IsExistEmptySlotFromInventoryTotal
desc : Inventory / TempInventory 에 비워있는 슬롯이 있는지 체크하는 함수
author : y2jinc
date : 2005. 9. 5
*/
BOOL ItemManager::IsExistEmptySlotFromInventoryTotal()
{
	POSTYPE EmptyPos;

	if ( this->GetEmptySlotPos(SI_INVENTORY, EmptyPos) )
	{
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::Test_InsertItemAtInventory( CODETYPE ItemCode, int ItemNum )
{
	SCSlotContainer * pContainer = GetContainer(SI_INVENTORY);

	if (ItemNum < 0 )
		return FALSE;

	POSTYPE EmptyPos;

	if ( !this->GetEmptySlotPos(SI_INVENTORY, EmptyPos) )
	{
		return FALSE;
	}	

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemCode );
	if ( !pInfo )
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("아이템 코드 %d 가 잘못 된 코드 입니다!!"), ItemCode);
		return FALSE;
	}

	SCItemSlot TempItem;
	TempItem.SetCode((SLOTCODE)ItemCode);
	if ( TempItem.IsOverlap() )
	{
		if ( ItemNum > pInfo->m_byDupNum )
		{
			// 오버랩가능한 개수보다 넘게 줬다
			TempItem.SetNum(pInfo->m_byDupNum);
			pContainer->InsertSlot(EmptyPos, TempItem);

			int iRemainNum = ItemNum-pInfo->m_byDupNum;

			return Test_InsertItemAtInventory(ItemCode, iRemainNum);            
		}
		else
		{
			TempItem.SetNum(ItemNum);
			pContainer->InsertSlot(EmptyPos, TempItem);
		}
	}
	else
	{
		pContainer->InsertSlot(EmptyPos, TempItem);            
		return Test_InsertItemAtInventory(ItemCode, --ItemNum);           
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID								
ItemManager::RepairItem( SLOTIDX AtSlotIdx, POSTYPE pos )
{
	SCSlotContainer *pContainer = GetContainer(AtSlotIdx);
	if (pContainer)
	{
		if (pContainer->IsEmpty(pos))
		{
			SUN_ASSERT_MSG(0, "pContainer->IsEmpty return TRUE");
			return;
		}

		SCSlot & rSlot = pContainer->GetSlot( pos );
		if (ST_ITEM != rSlot.GetSlotType())
		{
			SUN_ASSERT_MSG(0, "ST_ITEM != rSlot.GetSlotType()");
			return;							
		}

		SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;

        if (rItemSlot.IsContainValidItem() == false)
        {
            return;
        }

		if(rItemSlot.IsLimited() && rItemSlot.GetDura() < rItemSlot.GetMaxDura())
		{
			if (rItemSlot.DecreasingMaxDura() == FALSE)
            {
				return;		
            }
		}
        if (rItemSlot.WithDura() == false)
        {
            return;
        }

		rItemSlot.SetDura( rItemSlot.GetMaxDura() );
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID								
ItemManager::RepairItemAll( SLOTIDX AtSlotIdx )
{
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	if ( pContainer )
	{
		for ( POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
		{
			if ( pContainer->IsEmpty( pos ) )
			{
				continue;
			}

			SCSlot & rSlot = pContainer->GetSlot( pos );
			if ( ST_ITEM != rSlot.GetSlotType() )
			{
				continue;						
			}

			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
            if (rItemSlot.IsContainValidItem() == false)
            {
                continue;
            }
			if(rItemSlot.IsBlocked())
			{
				continue;
			}

			if(rItemSlot.IsLimited() && rItemSlot.GetDura() < rItemSlot.GetMaxDura())
			{
				if (rItemSlot.DecreasingMaxDura() == FALSE)
                {
					continue;		
                }
			}

            if (rItemSlot.WithDura() == false)
            {
                continue;
            }

			rItemSlot.SetDura(rItemSlot.GetMaxDura());
		}
	}	
}

//-------------------------------------------------------------------------------------------
/**
*/
MONEY                               
ItemManager::GetPriceRepairItemAll( SLOTIDX AtSlotIdx )
{
	MONEY RepairMoney = 0;
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	if ( pContainer )
	{
		for ( POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
		{
			if ( pContainer->IsEmpty( pos ) )
			{
				continue;
			}

			SCSlot & rSlot = pContainer->GetSlot( pos );
			if ( ST_ITEM != rSlot.GetSlotType() )
			{
				continue;						
			}

			SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(rSlot);

            if (rItemSlot.IsContainValidItem() == false)
            {
                continue;
            }
			if (rItemSlot.IsLimited() && (rItemSlot.GetLimitedDuraMax() <= 2))
            {
				continue;
            }

            if (rItemSlot.WithDura() == false)
            {
                continue;
            }

			RepairMoney += rItemSlot.GetPriceForRepair();
		}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 길드 시설
        int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_REPAIR_HEIM_RATIO);
        if (ratio != 0 && RepairMoney > 0)
        {
            bool is_plus = false;
            if (ratio > 0)
                is_plus = true;
            MONEY percent_value = 0;
            ratio = abs(ratio);
            percent_value = RepairMoney / 100 * ratio;
            if (is_plus)
            {
                RepairMoney += percent_value;
            }
            else
            {
                RepairMoney -= percent_value;
            }
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	}	

	return RepairMoney;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
ItemManager::IsExistRepairItem( SLOTIDX AtSlotIdx )
{
	BOOL bExistRepairItem = FALSE;

	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	if ( pContainer )
	{
		for ( POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
		{
			if ( pContainer->IsEmpty( pos ) )
			{
				continue;
			}

			SCSlot & rSlot = pContainer->GetSlot( pos );
			if ( ST_ITEM != rSlot.GetSlotType() )
			{
				continue;						
			}

			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;

            if (rItemSlot.IsContainValidItem() == false)
            {
                continue;
            }
			if ( !rItemSlot.WithDura() )
			{
				continue;
			}

			const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
			if( !pItemInfo )
			{
				continue;
			}

			if ( rItemSlot.GetDura() >= rItemSlot.GetMaxDura() )
			{
				continue;
			}

			bExistRepairItem = TRUE;
			break;
		}
	}	

	return bExistRepairItem;
}
//------------------------------------------------------------------------------
bool ItemManager::IsExistRepairItem(SLOTIDX AtSlotIdx, DURATYPE less_dura)
{
    SCSlotContainer* pContainer = GetContainer(AtSlotIdx);
    if (pContainer)
    {
        for (POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
        {
            if (pContainer->IsEmpty(pos))
            {
                continue;
            }

            SCSlot & rSlot = pContainer->GetSlot(pos);
            if (ST_ITEM != rSlot.GetSlotType())
            {
                continue;						
            }

            SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;

            if (rItemSlot.IsContainValidItem() == false)
            {
                continue;
            }
            if (!rItemSlot.WithDura())
            {
                continue;
            }

            const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
            if (pItemInfo == NULL)
            {
                continue;
            }

            if (rItemSlot.GetDura() >= less_dura)
            {
                continue;
            }

            return true;
        }
    }	

    return false;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
ItemManager::IsExistLimitedItem( SLOTIDX AtSlotIdx )
{
	SCSlotContainer * pContainer = GetContainer(AtSlotIdx);
	if ( pContainer )
	{
		for ( POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
		{
			if ( pContainer->IsEmpty( pos ) )
			{
				continue;
			}

			SCSlot & rSlot = pContainer->GetSlot( pos );
			if ( ST_ITEM != rSlot.GetSlotType() )
			{
				continue;						
			}

			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
			if(rItemSlot.IsLimited())
				return TRUE;
		}
	}
	return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemManager::MoveItemWithDiv()
{
	SCSlotContainer * pFromContainer	= g_pHero->GetSlotContainer( div_fromIdx );
	if( !pFromContainer)
		return;

	SCSlotContainer * pToContainer		= g_pHero->GetSlotContainer( div_toIdx );
	if( !pToContainer )
		return;

	SolarDialog * pFromContainerDlg = ItemManager::Instance()->GetItemContainerDialog(div_fromIdx);
	assert(pFromContainerDlg);
	SolarDialog * pToContainerDlg = ItemManager::Instance()->GetItemContainerDialog(div_toIdx);
	assert(pToContainerDlg);

	SCItemSlot& FromSlot = (SCItemSlot &)pFromContainer->GetSlot(div_fromPos);
	FromSlot.SetNum(div_fromNum);

	ItemUnitRenderer::RenderUnit * pRenderUnit1 = 
		pFromContainerDlg->GetItemUnitRender()->GetItemUnit( FromSlot.GetSerial() );
	if (pRenderUnit1)
	{
		pRenderUnit1->m_altNum = 0;
		pRenderUnit1->m_isDiv = false;
	}

	if( TRUE == pToContainer->IsEmpty( div_toPos ) )
	{
		InventoryDialog * pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
		if(pInvenDlg)
		{
			pInvenDlg->SetUpdateQuest(false);
		}

		// 한 방향 이동
		pToContainer->InsertSlot( div_toPos, FromSlot );

		if(pInvenDlg)
		{
			pInvenDlg->SetUpdateQuest(true);
		}

		SCItemSlot& ToSlot = (SCItemSlot &)pToContainer->GetSlot(div_toPos);
		ToSlot.SetNum(div_toNum);

		uiItemTotalcomposite * pTotalComposite = GET_CAST_DIALOG( uiItemTotalcomposite, IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE );

		if ( pTotalComposite->IsVisible() )	
			pTotalComposite->updateInfo();


		ItemUnitRenderer::RenderUnit * pRenderUnit2 = 
			pToContainerDlg->GetItemUnitRender()->GetItemUnit( ToSlot.GetSerial() );
		if (pRenderUnit2)
		{
			pRenderUnit2->m_altNum = 0;
			pRenderUnit2->m_isDiv = false;
		}
	}

}

VOID 
ItemManager::MoveItem(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount)
{
	SCSlotContainer * pFromContainer	= g_pHero->GetSlotContainer( fromIdx );
	if( !pFromContainer )
		return;
	SCSlotContainer * pToContainer		= g_pHero->GetSlotContainer( toIdx );
	if( !pToContainer )
		return;

	bool bUpdateOnOff = false;
	InventoryDialog * pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );

	if ((SI_INVENTORY == fromIdx) && (SI_INVENTORY == toIdx) && pInvenDlg)
	{
		bUpdateOnOff = true;
		pInvenDlg->SetUpdateQuest(false);
	}
	
	SCItemSlot FromSlot;
	pFromContainer->DeleteSlot( fromPos, &FromSlot );

	if(bUpdateOnOff)
		pInvenDlg->SetUpdateQuest(true);

	if( TRUE == pToContainer->IsEmpty( toPos ) )
	{
		if(bUpdateOnOff)
			pInvenDlg->SetUpdateQuest(false);

        DURATYPE remain_item_count = (FromSlot.GetNum()-itemCount);
        if (itemCount != 0)// 분할 이동
        {
            FromSlot.SetNum(itemCount);
        }
		// 한 방향 이동
		pToContainer->InsertSlot( toPos, FromSlot );
        if (itemCount != 0)// 분할 이동
        {
            FromSlot.SetNum(remain_item_count);
            pFromContainer->InsertSlot(fromPos, FromSlot);
        }

		if(bUpdateOnOff)
			pInvenDlg->SetUpdateQuest(true);
	}
	else
	{

        if(bUpdateOnOff == true)
        {
            pInvenDlg->SetUpdateQuest(false);
        }

        // 교환
		SCItemSlot ToSlot;
		pToContainer->DeleteSlot( toPos, &ToSlot );
		pFromContainer->InsertSlot( fromPos, ToSlot );
		pToContainer->InsertSlot( toPos, FromSlot );

        if(bUpdateOnOff == true)
        {
            pInvenDlg->SetUpdateQuest(true);
        }
	}

    //------------------------------------------------------------------------------ 
    const BASE_ITEMINFO* item_info = FromSlot.GetItemInfo();
#ifdef _YMS_AUTOBOT_GAME
    if (item_info->m_wType == eITEMTYPE_AUTO_HUNTING)
    {
        uiAutobotPlay* play = GET_CAST_DIALOG(uiAutobotPlay, uiAutobotPlay::kDialog_ID);
        if (play != NULL)
        {
            if ((fromIdx != SI_EQUIPMENT) && (toIdx == SI_EQUIPMENT))
            {
                //! 장착
                play->ShowInterface(TRUE);
            }
            else if ((fromIdx == SI_EQUIPMENT) && (toIdx != SI_EQUIPMENT))
            {
                //탈착
                if (Autobot::Instance()->IsStarted())
                {
                    Autobot::Instance()->Stop(false);
                }
                play->ShowInterface(FALSE);
            }
        }
    }
#endif //_YMS_AUTOBOT_GAME
    //------------------------------------------------------------------------------ 

    if (item_info->m_wType == eITEMTYPE_WINGS)
    {
        if ((fromIdx == SI_EQUIPMENT) && (toIdx != SI_EQUIPMENT))
        {
            if (g_pHero->is_spread_wings())
            {
                g_pHero->set_spread_wings(false);
            }
        }
    }

	// 아이템 사운드 
	if ( SI_EQUIPMENT == fromIdx)
	{
		// 탈착
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_UNEQUIP, &FromSlot);
	}
	else if (SI_EQUIPMENT == toIdx)
	{
		// 장착
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_EQUIP, &FromSlot);
	}
	else
	{
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, &FromSlot);
	}
}


/**
SCSlotContainer에서 DummySlotContainer 로 링크설때만 사용
*/
VOID LinkDummyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos )
{
	SCSlotContainer *		pFromContainer	= ItemManager::Instance()->GetContainer( fromIdx );
	DummySlotContainer *	pToContainer	= (DummySlotContainer * )ItemManager::Instance()->GetContainer( toIdx );

	SCItemSlot &	FromItemSlot = (SCItemSlot &)pFromContainer->GetSlot(fromPos);
	DummyItemSlot	ToDummyItemSlot;

	// 더미 아이템을 Set한다.
	ToDummyItemSlot.SetOrgSlot( &FromItemSlot );
	ToDummyItemSlot.SetFromContainerIdx(fromIdx);
	ToDummyItemSlot.SetFromPosition(fromPos);

	pToContainer->DeleteSlot( toPos, NULL );
	pFromContainer->InsertSlot( fromPos, ToDummyItemSlot );

	ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, &FromItemSlot);
}


VOID ItemManager::TradeBuyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos )
{
	SCSlotContainer * pFromContainer = g_pHero->GetSlotContainer( fromIdx );
	SCSlotContainer * pToContainer	 = g_pHero->GetSlotContainer( toIdx );
	if( !pToContainer)
		return;
	SCItemSlot FromSlot;


	if( FALSE == pToContainer->IsEmpty( toPos ) )
	{
		// 구매한 슬롯을 이동시킨다.
		SCItemSlot ToSlot;
		pToContainer->InsertSlot( toPos, ToSlot );
	}
}


VOID ItemManager::TradeSellItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos )
{
	SCSlotContainer * pFromContainer	= g_pHero->GetSlotContainer( fromIdx );
	if( !pFromContainer)
		return;
	SCSlotContainer * pToContainer		= g_pHero->GetSlotContainer( toIdx );

	SCItemSlot FromSlot;
	if( FALSE == pFromContainer->IsEmpty( fromPos ) )
	{
		// 판매한 슬롯을 삭제.
		pFromContainer->DeleteSlot( fromPos, NULL );
	}
}


VOID ItemManager::SimplexMoveLinkItem( SLOTIDX AtIndex, POSTYPE fromPos, POSTYPE toQuickPos )
{
	switch (AtIndex)
	{
    case SI_AUTOMATIC_INFO:
	case SI_QUICK:
		{
			moveLinkItem( AtIndex, fromPos, toQuickPos );
		}
		break;

	case SI_STYLE:
		{
			moveLinkStyle( AtIndex, fromPos, toQuickPos );
		}
		break;

	default:
		{
			assert(!"정의되지 않은 상태 - 재영 확인");
		}
		break;
	}	
}


VOID ItemManager::moveLinkItem( SLOTIDX AtIndex, POSTYPE fromQuickPos, POSTYPE toQuickPos )
{
	if (!g_pHero) return;

	SCSlotContainer * pFromContainer	= g_pHero->GetSlotContainer( AtIndex );
	if( !pFromContainer)
		return;
	SCSlotContainer * pToContainer		= g_pHero->GetSlotContainer( AtIndex );
	if( !pToContainer)
		return;
	QuickSlot FromSlot;
	pFromContainer->DeleteSlot( fromQuickPos, &FromSlot );
	if( TRUE == pToContainer->IsEmpty( toQuickPos ) )
	{
		pToContainer->InsertSlot( toQuickPos, FromSlot );	
	}
	else
	{
		// 교환..
		QuickSlot ToSlot;

		pToContainer->DeleteSlot( toQuickPos, &ToSlot );

		pFromContainer->InsertSlot( fromQuickPos, ToSlot );
		pToContainer->InsertSlot( toQuickPos, FromSlot );
	}

	ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_LINK, (SCSlot *)&FromSlot);
}

//---------------------------------------------------------------------
/**
@pre 
- g_pHero != NULL
- AtIndex == SI_STYLE
- 
@post
*/
VOID ItemManager::moveLinkStyle( SLOTIDX AtIndex, POSTYPE fromQuickPos, POSTYPE toQuickPos )
{
	if (!g_pHero) return;

	SCSlotContainer * pFromContainer	= g_pHero->GetSlotContainer( AtIndex );
	if( !pFromContainer)
		return;
	SCSlotContainer * pToContainer		= g_pHero->GetSlotContainer( AtIndex );
	if( !pToContainer)
		return;

	StyleSlot FromSlot;
	pFromContainer->DeleteSlot( fromQuickPos, &FromSlot );
	if( TRUE == pToContainer->IsEmpty( toQuickPos ) )
	{
		pToContainer->InsertSlot( toQuickPos, FromSlot );	
	}
	else
	{
		// 교환..
		StyleSlot ToSlot;

		pToContainer->DeleteSlot( toQuickPos, &ToSlot );

		pFromContainer->InsertSlot( fromQuickPos, ToSlot );
		pToContainer->InsertSlot( toQuickPos, FromSlot );
	}

	ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_LINK, (SCSlot *)&FromSlot);
}

//------------------------------------------------------------------------------ 
void ItemManager::AutobotLinkItem(SLOTIDX from_container_index, POSTYPE from_pos, POSTYPE to_pos, SLOTCODE org_code)
{
    if (g_pHero == NULL) 
    {
        return;
    }

    SCSlotContainer* from_container = g_pHero->GetSlotContainer(from_container_index);
    if (from_container == NULL)
    {
        return;
    }

    SCSlotContainer* to_container = g_pHero->GetSlotContainer(SI_AUTOMATIC_INFO);
    if (to_container == NULL)
    {
        return;
    }

    SCSlot& slot = from_container->GetSlot(from_pos);

    QuickSlot quick_slot;
    quick_slot.Clear();

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(org_code);
    if (item_info == NULL)
    {
        assert(!"이상한 코드네 확인 바람.");
        return;
    }

    BOOL bRet = TRUE;

    if (from_pos == QUICK_SLOT_NULL_VALUE) 
    {
        bRet = FALSE;
    }
    else
    {

        if (SCItemSlot::CanEquip(item_info))
        {
            //no operation
        }
        else
        {
            // 비장착 아이템 (물약..)
            if (from_container->IsEmpty(from_pos))
            {
                bRet = ItemManager::Instance()->FindFirstItem(org_code, from_pos);	
            }
            else
            {
                if (slot.GetCode() != org_code)
                {
                    bRet = ItemManager::Instance()->FindFirstItem(org_code , from_pos);	
                }
            }
        }
    }

    SCSlot& from_slot = from_container->GetSlot(from_pos);

    if (bRet == FALSE)
    {
        // 비활성화 상태
        quick_slot.SetOrgCode(org_code);
        quick_slot.SetOrgSlotIndex(from_container_index);
        quick_slot.SetOrgPos(from_pos);
        quick_slot.SetOrgSlotType(ST_ITEM);
        quick_slot.SetOrgSlot(&from_slot);
        quick_slot.SetOverlapCount(0);
        quick_slot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
    }
    else
    {
        // 활성화 상태
        quick_slot.SetOrgCode(from_slot.GetCode());
        quick_slot.SetOrgSlotIndex(from_container_index);
        quick_slot.SetOrgPos(from_pos);					// 아이템은 무조건 인벤에서 왔다고 보고 위치가 중요.
        quick_slot.SetOrgSlotType(ST_ITEM);
        quick_slot.SetOrgSlot(&from_slot);
        quick_slot.SetStatus(SLOT_RENDER_STATE_ACTIVATED);

        SCItemSlot& from_item_slot = static_cast<SCItemSlot&>(from_slot);
        if (from_item_slot.IsOverlap())
        {
            if(item_info->m_wType == eITEMTYPE_ETHER_BULLET) 
            {
                quick_slot.SetOverlapCount(from_slot.GetNum());	
            }
            else
            {
                quick_slot.SetOverlapCount(GetOverlapItemCount(from_slot.GetCode()));
            }
        }
        else
        {
            quick_slot.SetOverlapCount(0);
        }
    }

    to_container->DeleteSlot(to_pos, NULL );
    to_container->InsertSlot(to_pos, quick_slot);
}



//------------------------------------------------------------------------------ 
void ItemManager::AutobotLinkSkill(POSTYPE to_pos, SLOTCODE org_code)
{
    if (g_pHero == NULL) 
    {
        return;
    }

    BASE_SKILLINFO* skill_info = (BASE_SKILLINFO*)SkillInfoParser::Instance()->GetSkillInfo(org_code);
    if (skill_info == NULL)
    {
        return;
    }

    // 스킬
    SCSlotContainer* to_container = g_pHero->GetSlotContainer(SI_AUTOMATIC_INFO);
    if (to_container == NULL)
    {
        return;
    }

    SCSkillSlot* skill_slot = const_cast<SCSkillSlot *>(GetSkillSlotByCode(org_code));
    if (skill_slot == NULL)
    {
        return;
    }

    const SkillScriptInfo* skill_script_info = SkillInfoParser::Instance()->GetSkillInfo(org_code);
    if (skill_script_info == NULL)
    {
        return;
    }

    if (skill_script_info->m_SkillClassCode != skill_slot->GetSkillInfo()->m_SkillClassCode)
    {
        assert(false && "스킬코드가 맞지않는다!!" );	
        return;
    }

    QuickSlot quick_slot;
    quick_slot.Clear();
    quick_slot.SetOrgSlotIndex(SI_SKILL);	
    quick_slot.SetOrgCode(skill_slot->GetCode());	// 스킬에서는 Code로 사용	
    quick_slot.SetOrgSlotType(ST_SKILL);
    quick_slot.SetOrgSlot(skill_slot);					// 포인터도 제대로 쎗해준다.

    if (to_container->IsEmpty(to_pos))
    {
        to_container->InsertSlot( to_pos, quick_slot );
    }
    else
    {
        to_container->DeleteSlot(to_pos, NULL);
        to_container->InsertSlot(to_pos, quick_slot);
    }
}




//------------------------------------------------------------------------
/**
OrgCode는 비활성화일때만 의미가 있다.
@remark
- 장착 아이템과 비 장착 아이템을 구분해서 등록하였다.
장착 아이템은 해당 링크된 곳에 아이템이 없거나 다르면 무조건 비활성화다.
비장착 아이템은 인벤토리내에 존재하는지 찾는 루틴을 이용해서 활성화 시키는 루틴이 있고

*/
VOID ItemManager::QuickLinkItem( SLOTIDX atIndex, POSTYPE OrgPos, POSTYPE toQuickPos, SLOTCODE OrgCode /*= 0*/)
{
	if (!g_pHero) 
    {
        return;
    }

	SCSlotContainer *pFromContainer = g_pHero->GetSlotContainer( atIndex );
	SUN_ASSERT(pFromContainer);
	if (!pFromContainer)
    {
		return;
    }

	SCSlotContainer *pToContainer = g_pHero->GetSlotContainer( SI_QUICK );
	SUN_ASSERT(pToContainer);
    if (!pToContainer)
    {
        return;
    }
    

	QuickSlot TempQuickSlot;
	TempQuickSlot.Clear();

	POSTYPE FromPos = OrgPos;

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( OrgCode );
	if ( !pItemInfo )
	{
		assert(!"이상한 코드네 확인 바람.");
		OrgCode;
		return;
	}

	BOOL bRet = TRUE;

	if ( FromPos == QUICK_SLOT_NULL_VALUE) 
	{
		bRet = FALSE;
	}
	else
	{
        if (SCItemSlot::CanEquip(pItemInfo))
		{
			// 장착 아이템이다.
			if (pFromContainer->IsEmpty(FromPos))
			{
				bRet = FALSE;	
			}
			else
			{
				SCSlot & rSlot = pFromContainer->GetSlot(OrgPos);
				if (rSlot.GetCode() != OrgCode)
				{
					bRet = FALSE;
				}
			}
		}
		else
		{
			// 비장착 아이템 (물약..)
			if (pFromContainer->IsEmpty(FromPos))
			{
				bRet = ItemManager::Instance()->FindFirstItem(OrgCode, FromPos);	
			}
			else
			{
				SCSlot & rSlot = pFromContainer->GetSlot(OrgPos);
				if (rSlot.GetCode() != OrgCode)
				{
					bRet = ItemManager::Instance()->FindFirstItem(OrgCode , FromPos);	
				}
			}
		}
	}

	SCSlot & rFromSlot  = pFromContainer->GetSlot( FromPos );

	if (!bRet)
	{
		// 비활성화 상태
		TempQuickSlot.SetOrgCode( OrgCode );
		TempQuickSlot.SetOrgSlotIndex( atIndex );
		TempQuickSlot.SetOrgPos(INVALID_POSTYPE_VALUE/*OrgPos*/); //버그수정 / 송찬종 / SUNKR-262 퀵슬롯 이미지가 다르게 출력되는 현상 수정
		TempQuickSlot.SetOrgSlotType( ST_ITEM );
		/*	##	Scene 이동시나 처음 로긴시에 이미 장착하고 있던 무기 체인지 안되던 버그 수정
		##	2006/06/22 lammy 
		*/
		TempQuickSlot.SetOrgSlot( &rFromSlot );
		/*	##	*/
		TempQuickSlot.SetOverlapCount(0);
		assert(OrgCode != 0);
		TempQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
	}
	else
	{
		// 활성화 상태
		TempQuickSlot.SetOrgCode(rFromSlot.GetCode());
		TempQuickSlot.SetOrgSlotIndex(atIndex);
		TempQuickSlot.SetOrgPos(OrgPos);					// 아이템은 무조건 인벤에서 왔다고 보고 위치가 중요.
		TempQuickSlot.SetOrgSlotType(ST_ITEM);
		TempQuickSlot.SetOrgSlot(&rFromSlot);
		TempQuickSlot.SetStatus(SLOT_RENDER_STATE_ACTIVATED);

		SCItemSlot& rFromItemSlot = (SCItemSlot&)rFromSlot;
		if (rFromItemSlot.IsOverlap())
		{
			//GS_BUG_FIX:에테르 탄환은예외처리,겹쳐지지만 총갯수를 계산하지않겠다
			if (pItemInfo->m_wType == eITEMTYPE_ETHER_BULLET) 
			{
				TempQuickSlot.SetOverlapCount(rFromSlot.GetNum());	
			}
			else
			{
				TempQuickSlot.SetOverlapCount(GetOverlapItemCount(rFromSlot.GetCode()));
			}
			
		}
		else
		{
			TempQuickSlot.SetOverlapCount(0);
		}
	}


	pToContainer->DeleteSlot(toQuickPos, NULL);
	pToContainer->InsertSlot(toQuickPos, TempQuickSlot);

}

//---------------------------------------
/*
퀵컨테이너에 스킬 링크
*/
VOID
ItemManager::QuickLinkSkill( SLOTIDX atIndex, SLOTCODE OrgCode, POSTYPE toQuickPos )
{
	if (!g_pHero) return;

	BASE_SKILLINFO * pSkillInfo = 
		(BASE_SKILLINFO *)SkillInfoParser::Instance()->GetSkillInfo(OrgCode);



	if (pSkillInfo)
	{
		// 스킬
		SCSlotContainer *pToContainer = g_pHero->GetSlotContainer( SI_QUICK );
		SUN_ASSERT(pToContainer);
        if (!pToContainer)
        {
            return;
        }

		SCSkillSlot *pOrgSkillSlot = const_cast<SCSkillSlot *>(GetSkillSlotByCode(OrgCode));
		SUN_ASSERT(pOrgSkillSlot);
		if (!pOrgSkillSlot)
		{
			return;
		}

		QuickSlot TempQuickSlot;
		TempQuickSlot.Clear();
		TempQuickSlot.SetOrgSlotIndex( SI_SKILL );	

		SUN_ASSERT( OrgCode == pOrgSkillSlot->GetCode() && "스킬코드가 맞지않는다!!" );	
		TempQuickSlot.SetOrgCode( pOrgSkillSlot->GetCode() );	// 스킬에서는 Code로 사용	
		TempQuickSlot.SetOrgSlotType( ST_SKILL );
		TempQuickSlot.SetOrgSlot( pOrgSkillSlot );					// 포인터도 제대로 쎗해준다.

		if (pToContainer->IsEmpty(toQuickPos))
		{
			pToContainer->InsertSlot( toQuickPos, TempQuickSlot );
		}
		else
		{
			pToContainer->DeleteSlot( toQuickPos, NULL );
			pToContainer->InsertSlot(toQuickPos, TempQuickSlot);
		}
	}
	else
	{
		assert(0);
	}
}


//---------------------------------------
/*
스타일 컨테이너에 스타일 링크
*/
VOID 
ItemManager::StyleLink(SLOTCODE OrgCode, POSTYPE toQuickPos )
{
	if (!g_pHero) return;

	ROOT_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetInfo(OrgCode);
	assert( pInfo );
	if( !pInfo ) return;

	BASE_SKILLINFO * pSkillInfo = NULL;
	BASE_STYLEINFO * pStyleInfo = NULL;


	if (pInfo->IsSkill())
	{
		pSkillInfo = (BASE_SKILLINFO *)pInfo;

	}
	else
	{
		pStyleInfo = (BASE_STYLEINFO *)pInfo;

	}

	if (pStyleInfo)
	{
		// 스킬
		SCSlotContainer *pToContainer = g_pHero->GetSlotContainer( SI_STYLE );
		SUN_ASSERT(pToContainer);
        if (!pToContainer)
        {
            return;
        }

		SCSkillSlot *pOrgSkillSlot = const_cast<SCSkillSlot *>(GetSkillSlotByCode(OrgCode));
		SUN_ASSERT(pOrgSkillSlot);
		if (!pOrgSkillSlot)
		{
			return;
		}

		StyleContainer * pStyleDlg = 
			(StyleContainer *)ItemManager::Instance()->GetItemContainerDialog(SI_STYLE);

        if (!pStyleDlg)
        {
            return;
        }

		if (pStyleDlg->IsExistSameCodeItem(pOrgSkillSlot->GetCode()))
		{
			SUN_ASSERT_MSG(0, "pStyleDlg->IsExistSameCodeItem(pOrgSkillSlot->GetCode()) == TRUE");
			return;
		}

		StyleSlot TempStyleSlot;
		TempStyleSlot.Clear();
		TempStyleSlot.SetOrgCode( OrgCode );						// 스킬에서는 Code로 사용
		TempStyleSlot.SetOrgSlotType( ST_SKILL );
		TempStyleSlot.SetOrgSlot( pOrgSkillSlot );					// 포인터도 제대로 쎗해준다.

		if (pToContainer->IsEmpty(toQuickPos))
		{
			pToContainer->InsertSlot( toQuickPos, TempStyleSlot );
		}
		else
		{
			pToContainer->DeleteSlot( toQuickPos, NULL );
			pToContainer->InsertSlot(toQuickPos, TempStyleSlot);
		}
	}
	else
	{
		assert(0);
	}
}


VOID ItemManager::CopyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos )
{
	SCSlotContainer * pFromContainer = this->GetContainer( fromIdx );
	assert (pFromContainer);
	SCSlotContainer * pToContainer	 = this->GetContainer( toIdx );
	assert (pToContainer);

	SCSlot &  rSlot	= pFromContainer->GetSlot( fromPos);

	pToContainer->InsertSlot( toPos, rSlot );
}

//--------------------------------------------------------------------
// 아이템 composite 관련 처리
BOOL ItemManager::SpendCompositeMaterials(
	eITEMCOMPOSITECODE eItemCompositeType, 
	SLOTIDX	fromSlotIndex)
{
	SCSlotContainer *pAtContainer = GetContainer(fromSlotIndex);
    if (!pAtContainer)
    {
        return FALSE;
    }

	// 사용한 메터리얼들을 지운다. 
	int iMaterialsIndex = 0;
	int iSlotIndex = 0;
	int iDeleteMaterialCount = 0;
	int	iRemainDeleteMaterialCount = 0;
	CODETYPE DeleteMaterialCode = 0;
	SCItemSlot DeleteItemSlot;

	sITEMCOMPOSITE *pItemComposite = NULL;
	pItemComposite = ItemCompositeParser::Instance()->GetCompositeInfo(eItemCompositeType );

	if (!pItemComposite)
	{
		assert(0);
		return FALSE;
	}

	for (; iMaterialsIndex < pItemComposite->m_byMatCount; ++iMaterialsIndex)
	{
		iRemainDeleteMaterialCount = iDeleteMaterialCount = 
			pItemComposite->m_sCompositeMaterials[iMaterialsIndex].m_bySpendMaterialNum;

		DeleteMaterialCode = pItemComposite->m_sCompositeMaterials[iMaterialsIndex].m_dwCompositeMaterialCode;

		for (iSlotIndex = 0; iSlotIndex < pAtContainer->GetMaxSlotNum(); ++iSlotIndex)
		{
			if (iRemainDeleteMaterialCount < 1)
				break;

			if (pAtContainer->IsEmpty(iSlotIndex))
				continue;

			SCItemSlot & rGetItemSlot = (SCItemSlot &)pAtContainer->GetSlot(iSlotIndex);
			if (rGetItemSlot.GetCode() != DeleteMaterialCode)
				continue;

			if (rGetItemSlot.IsOverlap())
			{
				// 중복 가능 하면 count 를 하나씩 빼나간다.

				int iCurCount = rGetItemSlot.GetNum();
				int iCurRemainCnt = iCurCount - iRemainDeleteMaterialCount;

				if (0 == iCurRemainCnt)
				{
					pAtContainer->DeleteSlot(iSlotIndex, &DeleteItemSlot);
					break;
				}
				else if (iCurRemainCnt > 0)
				{
					// 지우고 남았넹
					rGetItemSlot.SetNum( iCurRemainCnt );
					break;
				}
				else if (iCurRemainCnt < 0)
				{
					pAtContainer->DeleteSlot(iSlotIndex, &DeleteItemSlot);

					// 지웠는데 남았다.
					iRemainDeleteMaterialCount -= iCurCount;
				}
			}
			else
			{
				pAtContainer->DeleteSlot(iSlotIndex, &DeleteItemSlot);
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Send Packet 관련
VOID ItemManager::SendItemMoveMsg(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE ItemCount)
{
	MSG_CG_ITEM_MOVE_SYN msg;
	msg.m_byCategory	= CG_ITEM;
	msg.m_byProtocol	= CG_ITEM_MOVE_SYN;
	msg.m_fromIndex		= fromIdx;
	msg.m_toIndex		= toIdx;
	msg.m_fromPos		= fromPos;
	msg.m_toPos			= toPos;
    msg.move_quantity_  = ItemCount;

	// 아이템 이동 패킷을 보내기전에 유효성 체크.
	SCSlotContainer* pFromContainer = GetContainer(fromIdx);
	SCSlotContainer* pToContainer = GetContainer(toIdx);
	if (pFromContainer && pToContainer) 
	{
		if ((fromPos < pFromContainer->GetMaxSlotNum()) &&
			(toPos < pToContainer->GetMaxSlotNum())) 
		{
			assert(pFromContainer->GetSlot(fromPos).GetSlotType() == ST_ITEM);
			assert(pToContainer->GetSlot(toPos).GetSlotType() == ST_ITEM);
			SCItemSlot& rFromItemSlot = (SCItemSlot&)pFromContainer->GetSlot(fromPos);
			SCItemSlot& rToItemFromSlot = (SCItemSlot&)pToContainer->GetSlot(toPos);

			if (rFromItemSlot.GetLaxLock()->IsLock() || rToItemFromSlot.GetLaxLock()->IsLock())
			{
				//70130	해당 슬롯으로 이동할수없습니다.
				NoticeReasonItemErrorCode(RC_ITEM_INVALIDPOS);
				MouseHandler::Instance()->RollbackItemAtHand();
				return;
			}

			if (rFromItemSlot.IsLocked() || rToItemFromSlot.IsLocked())
			{
				MouseHandler::Instance()->RollbackItemAtHand();
				return;
			}

			if ((toIdx == SI_EQUIPMENT) && (pToContainer->IsEmpty(toPos) == FALSE))
			{
				if (CanUnEquipItem(toPos, TRUE) == FALSE)
                {
					return;
                }
			}
		}
	}

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));

	if (!GENERALPARAM->IsNet())
	{
		MSG_CG_ITEM_MOVE_ACK SendPacketAck;
		SendPacketAck.m_fromIndex		= fromIdx;
		SendPacketAck.m_toIndex			= toIdx;
		SendPacketAck.m_fromPos			= fromPos;
		SendPacketAck.m_toPos			= toPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck));
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID ItemManager::SendItemMoveMsgWithDiv(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum )
{
	div_fromIdx = fromIdx;
	div_toIdx = toIdx;
	div_fromPos = fromPos;
	div_toPos = toPos;
	div_fromNum = fromNum;
	div_toNum = toNum;

	MSG_CG_ITEM_DIVIDE_SYN msg;

	msg.m_fromPos = fromPos;
	msg.m_toPos = toPos;
	msg.m_fromNum = fromNum;
	msg.m_toNum = toNum;

    //------------------------------------------------------------------------------ 
    if (PacketRequestManager::Instance()->is_sent(msg.m_byCategory, msg.m_byProtocol) == true)
    {
        MouseHandler::Instance()->RollbackItemAtHand();
        return;
    }

    //------------------------------------------------------------------------------ 
	// 아이템 이동 패킷을 보내기전에 유효성 체크.
	SCSlotContainer * pFromContainer = GetContainer(fromIdx);
	SCSlotContainer * pToContainer = GetContainer(toIdx);
	if (pFromContainer && pToContainer) 
	{
		if (fromPos < pFromContainer->GetMaxSlotNum() &&
			toPos < pToContainer->GetMaxSlotNum() ) 
		{
			assert(pFromContainer->GetSlot(fromPos).GetSlotType() == ST_ITEM );
			assert(pToContainer->GetSlot(toPos).GetSlotType() == ST_ITEM );
			SCItemSlot & rFromItemSlot = (SCItemSlot &)pFromContainer->GetSlot( fromPos );
			SCItemSlot & rToItemFromSlot = (SCItemSlot &)pToContainer->GetSlot( toPos );

			if ( rFromItemSlot.IsLocked() || rToItemFromSlot.IsLocked() )
			{
				MouseHandler::Instance()->RollbackItemAtHand();
				return;
			}	
		}
	}

    //------------------------------------------------------------------------------ 
    MSG_CG_ITEM_DIVIDE_SYN* syn = new MSG_CG_ITEM_DIVIDE_SYN;
    MSG_CG_ITEM_DIVIDE_ACK* ack = new MSG_CG_ITEM_DIVIDE_ACK;
    MSG_CG_ITEM_DIVIDE_NAK* nak = new MSG_CG_ITEM_DIVIDE_NAK;

    PacketRequestManager::Instance()->PushPacket(syn, 
                                                ack, 
                                                nak,
                                                sizeof(MSG_CG_ITEM_DIVIDE_SYN), 
                                                PACKET_WAITING_MILLISECOND,
                                                false);
    //------------------------------------------------------------------------------  

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &msg, sizeof(msg) );

	if ( !GENERALPARAM->IsNet() )
	{
		MSG_CG_ITEM_DIVIDE_ACK msg;
		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&msg, sizeof(msg) );
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID								
ItemManager::SendItemRepairMsg( SLOTIDX AtIdx, POSTYPE Pos, bool bAllRepair /* = FALSE */ )
{
	MSG_CG_ITEM_REPAIR_DURA_SYN SendPackt;
	SendPackt.m_SlotIndex = AtIdx;
	SendPackt.m_bAllRepair = (bool)bAllRepair;
	SendPackt.m_SlotPos = Pos;

	SendPackt.m_NPCCode = g_HeroInput.GetClickedNpcCode();
	GlobalFunc::SendPacketToGameServer(&SendPackt, sizeof(SendPackt));

	if ( !GENERALPARAM->IsNet() )
	{
		MSG_CG_ITEM_REPAIR_DURA_ACK SendPacktAck;
		SendPacktAck.m_bAllRepair = (bool)bAllRepair;
		SendPacktAck.m_SlotIndex = AtIdx;
		SendPacktAck.m_Money = 11;
		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacktAck, sizeof(SendPacktAck));
	}
}

void SendItemSell( bool b )
{
	if( b )
	{
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &g_ItemSellSyn, sizeof( MSG_CG_ITEM_SELL_SYN ) );
	}
}

VOID ItemManager::SendSellMsg(SLOTIDX atIndex, POSTYPE atPos, DURATYPE ItemCount, eENCHANTLEVEL popup_enchant_level)
{
	SCSlotContainer *pContainer = GetContainer(atIndex);

    if (!pContainer)
    {
        return;
    }

	if (pContainer->IsEmpty(atPos))
	{
		return;
	}

	SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(atPos);

	if ( !rItemSlot.GetItemInfo() )
	{
		return;
	}

	if ( 0 == rItemSlot.GetItemInfo()->m_ExchangedItem )
	{
		g_ItemSellSyn.m_byCategory = CG_ITEM;
		g_ItemSellSyn.m_byProtocol = CG_ITEM_SELL_SYN;

		g_ItemSellSyn.m_atIndex	= atIndex;
		g_ItemSellSyn.m_atPos		= atPos;
		g_ItemSellSyn.m_NPCCode = g_HeroInput.GetClickedNpcCode();
        g_ItemSellSyn.sell_quantity_ = ItemCount; // 판매 수량(기본 0)
		g_ItemSellSyn.Encode();


		//if( rItemSlot.GetEnchantLV() > ENCHANT_LOW )
        if( rItemSlot.GetEnchantLV() > popup_enchant_level )
		{			
			// 5096 정말로 매각하시겠습니까?
			GlobalFunc::SysConfirmBox( g_InterfaceManager.GetInterfaceString( 5096 ), SendItemSell );
		}
		else
		{
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &g_ItemSellSyn, sizeof(MSG_CG_ITEM_SELL_SYN) );
		}
	}
	else
	{
		m_PendingExchangeContainIdx	= atIndex;

		MSG_CG_ITEM_EXCHANGE_SYN ItemExchangeSyn;
		m_PendingExchangeItemPos = ItemExchangeSyn.m_atPos = atPos;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &ItemExchangeSyn, sizeof(MSG_CG_ITEM_EXCHANGE_SYN) );

	}
}

VOID ItemManager::SendBuyMsg( DWORD dwShopListID, SLOTCODE ItemCode, BYTE byTabIndex, POSTYPE ItemPos, DURATYPE ItemCount)
{
	SHOPINFO * pShopInfo = ShopInfoParser::Instance()->GetShopList(dwShopListID);

	assert(byTabIndex < SHOPINFO::_MAX_TAB_NUM);
	assert(ItemPos < SHOPINFO::_MAX_SELLITEM_NUM);

	MSG_CG_ITEM_BUY_SYN ItemBuySyn;
	ItemBuySyn.m_byCategory = CG_ITEM;
	ItemBuySyn.m_byProtocol = CG_ITEM_BUY_SYN;

	ItemBuySyn.m_dwShopListID		= dwShopListID;
	ItemBuySyn.m_ShopTabIndex		= byTabIndex;
	ItemBuySyn.m_ShopItemIndex		= ItemPos;
    ItemBuySyn.buy_quantity_        = ItemCount;
	ItemBuySyn.m_NPCCode  = g_HeroInput.GetClickedNpcCode();

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &ItemBuySyn, sizeof(MSG_CG_ITEM_BUY_SYN) );
}

VOID ItemManager::SendPickupItem(DWORD ItemObjectKey)
{
	MSG_CG_ITEM_PICK_SYN SendPacket;
	SendPacket.m_dwFieldItemObjectKey = ItemObjectKey;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_ITEM_PICK_SYN) );
}

VOID ItemManager::SendPickupMoney(DWORD ItemObjectKey)
{
	MSG_CG_ITEM_PICK_MONEY_SYN SendPacket;
	SendPacket.m_dwFieldItemObjectKey = ItemObjectKey;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_ITEM_PICK_MONEY_SYN) );
}

void ItemManager::SendPickUpItemObject(Item* item_object)
{
    if (item_object == NULL)
    {
        return;
    }

    switch (item_object->GetItemType())
    {
    case ITEM_RENDER_INFO::eFIELDITEM_ITEM:
        {
            SendPickupItem(item_object->GetObjectKey());
        }
        break;
    case ITEM_RENDER_INFO::eFIELDITEM_MONEY:
        {
            SendPickupMoney(item_object->GetObjectKey());
        }
        break;
    }
}

void _callback_ItemManager_SendDropItemMsg(bool bYes)
{
	if (bYes)
	{
		ItemManager::Instance()->VerifiedSendDropItemMsg();
	}
	else
	{
		if (MouseHandler::Instance()->IsExistItemAtHand())
			MouseHandler::Instance()->RollbackItemAtHand();
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID ItemManager::VerifiedSendDropItemMsg()
{
	MSG_CG_ITEM_DROP_SYN		SendPacket;

	SendPacket.m_byCategory = CG_ITEM;
	SendPacket.m_byProtocol = CG_ITEM_DROP_SYN;
	SendPacket.m_atIndex	= m_verify_dropFromIdx;
	SendPacket.m_atPos		= m_verify_dropFromPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_ITEM_DROP_SYN));

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_DROP_ACK SendPacketAck;
		SendPacketAck.m_atIndex = m_verify_dropFromIdx;
		SendPacketAck.m_atPos = m_verify_dropFromPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}

VOID ItemManager::SendDropItemMsg(SLOTIDX FromIdx, POSTYPE FromPos)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if (!MouseHandler::Instance()->IsExistItemAtHand())
    {
		return;
    }

	SCSlotContainer* pContainer = GetContainer(FromIdx);
	if (pContainer)
	{
		SCItemSlot& rItemSlot = (SCItemSlot&)pContainer->GetSlot(FromPos);

		if (rItemSlot.GetLaxLock()->IsLock())
		{
			//5347 아이템을 버릴 수 없는 상태입니다
			NoticeReasonItemErrorCode(RC_ITEM_CANNNOT_DROPSTATE);
			MouseHandler::Instance()->RollbackItemAtHand();
			return;
		}

	}


#ifdef _JBH_ADD_BLOCK_HIGH_VALUE_ITEM_DRPO
	SCSlotContainer* pContainer = GetContainer(FromIdx);
	if (pContainer)
	{
		SCItemSlot& rItemSlot = (SCItemSlot&)pContainer->GetSlot(FromPos);
		if (rItemSlot.IsOverlap() == FALSE)
		{
			if (rItemSlot.IsDivine() || (rItemSlot.GetEnchant() >= 5))
			{
				// 1601	고가의 아이템은 버릴 수 없습니다.
				g_InterfaceManager.GetInterfaceString(eST_NOT_DROP_HIGH_VALUE_ITEM, szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				MouseHandler::Instance()->RollbackItemAtHand();
				return;
			}
		}
	}
#endif

	m_verify_dropFromIdx = FromIdx;
	m_verify_dropFromPos = FromPos;

	// 아이템을 바닥에 버리시겠습니까? 5100
	g_InterfaceManager.GetInterfaceString(5100, szMessage, INTERFACE_STRING_LENGTH);

	uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
    {
		pSystemMan->Sys_ConfirmLock(szMessage, _callback_ItemManager_SendDropItemMsg);
    }

	MouseHandler::Instance()->RollbackItemAtHand();
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID								
ItemManager::SendDeleteItemMsg( POSTYPE FromPos )
{
	if( ! IsItemDeleteAvalibeState(FromPos) )	
	{
		return;
	}

	MSG_CG_ITEM_DELETE_SYN SendPacket;
	SendPacket.m_AtPos = FromPos;
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_ITEM_DELETE_SYN));	

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_DELETE_ACK SendPacketAck;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}


VOID ItemManager::SendQuickItemLinkMsg( POSTYPE atPos, POSTYPE ToPos)
{
	MSG_CG_ITEM_QUICK_LINKITEM_SYN SendPacket;

	SendPacket.m_OrgPos = atPos;
	SendPacket.m_ToPos = ToPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_QUICK_LINKITEM_ACK SendPacketAck;
		SendPacketAck.m_OrgPos = atPos;
		SendPacketAck.m_ToPos = ToPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}

//---------------------------------------------------------------
// 퀵 컨테이너 관련 패킷 
VOID ItemManager::SendQuickSkillLinkMsg( SLOTCODE OrgCode, POSTYPE ToPos)
{
	MSG_CG_ITEM_QUICK_LINKSKILL_SYN SendPacket;

	SendPacket.m_SkillCode = OrgCode;
	SendPacket.m_ToPos = ToPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_QUICK_LINKSKILL_ACK SendPacketAck;
		SendPacketAck.m_SkillCode = OrgCode;
		SendPacketAck.m_ToPos = ToPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}


VOID ItemManager::SendSimplexMoveLinkMsg(POSTYPE fromPos, POSTYPE ToQuickPos)
{
	MSG_CG_ITEM_QUICK_MOVE_SYN			SendPacket;

	SendPacket.m_fromPos	= fromPos;
	SendPacket.m_toPos		= ToQuickPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_QUICK_MOVE_ACK SendPacketAck;
		SendPacketAck.m_fromPos = fromPos;
		SendPacketAck.m_toPos = ToQuickPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}

VOID ItemManager::SendQuickItemRemoveMsg( POSTYPE AtPos)
{
	MSG_CG_ITEM_QUICK_UNLINK_SYN Sendpacket;

	Sendpacket.m_atPos = AtPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &Sendpacket, sizeof(Sendpacket) );

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_ITEM_QUICK_UNLINK_ACK SendPacketAck;
		SendPacketAck.m_atPos = AtPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}

////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------
// 스타일 관련
VOID 
ItemManager::SendQuickStyleLinkMsg(SLOTCODE OrgCode, POSTYPE ToPos)
{
	MSG_CG_STYLE_LINK_SYN SendPacket;

	SendPacket.m_StyleCode	= OrgCode;
	SendPacket.m_ToPos		= ToPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_STYLE_LINK_ACK SendPacketAck;
		SendPacketAck.m_StyleCode	= OrgCode;
		SendPacketAck.m_ToPos		= ToPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}


VOID 
ItemManager::SendQuickStyleMoveLinkMsg(POSTYPE fromPos, POSTYPE ToPos)
{
	MSG_CG_STYLE_LINKMOVE_SYN SendPacket;

	SendPacket.m_fromPos = fromPos;
	SendPacket.m_toPos = ToPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_STYLE_LINKMOVE_ACK SendPacketAck;
		SendPacketAck.m_fromPos = fromPos;
		SendPacketAck.m_toPos = ToPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}


VOID 
ItemManager::SendQuickStyleRemoveMsg(POSTYPE AtPos )
{
	MSG_CG_STYLE_UNLINK_SYN		SendPacket;

	SendPacket.m_atPos = AtPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	if ( !GENERALPARAM->IsNet() )
	{   
		MSG_CG_STYLE_UNLINK_ACK SendPacketAck;
		SendPacketAck.m_atPos = AtPos;

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck) );
	}
}


//////////////////////////////////////////////////////////////////////////

VOID 
ItemManager::SendWareHouseItemAllSyn()
{
	MSG_CG_WAREHOUSE_START_SYN			SendPacket;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
}


BOOL 
ItemManager::CanEquipClass(Player * pPlayer, const BASE_ITEMINFO * pInfo ) const
{
	// class  확인

	if( !pPlayer || !pInfo)
		return FALSE;

	switch ( (eCHAR_TYPE)pPlayer->GetClass() )
	{
	case eCHAR_BERSERKER:
		{
			if (!pInfo->m_wEqClass1)
			{
				return FALSE;
			}
		}
		break;
	case eCHAR_DRAGON:
		{
			if (!pInfo->m_wEqClass2)
			{
				return FALSE;
			}
		}
		break;
	case eCHAR_SHADOW:
		{
			if (!pInfo->m_wEqClass3)
			{
				return FALSE;
			}
		}
		break;
	case eCHAR_VALKYRIE:
		{
			if (!pInfo->m_wEqClass4)
			{
				return FALSE;
			}
		}
		break;
	case eCHAR_MAGICIAN:
		{
			if (!pInfo->m_wEqClass5)
			{
				return FALSE;
			}
		}
		break;
    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            if (!pInfo->m_wEqClass6)
            {
                return FALSE;
            }
        }
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            if (!pInfo->m_wEqClass7)
            {
                return FALSE;
            }
        }
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            if (!pInfo->m_wEqClass8)
            {
                return FALSE;
            }
        }
        break;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL
ItemManager::CanEquipLimit( SCItemSlot & rItemSlot, BOOL bOuputMsg)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	TCHAR szResult[INTERFACE_STRING_LENGTH];
	TCHAR szItem[INTERFACE_STRING_LENGTH];

	if (!g_pHero)
	{
		return FALSE;
	}

	if ( !ValidLimitStr( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 힘 제한에 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_STR, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}

		return FALSE;
	}

	if ( !ValidLimitDex( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 민첩 제한에 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_DEX, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}

		return FALSE;
	}

	if ( !ValidLimitInt( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 지력 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_INT, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}

		return FALSE;
	}

	if ( !ValidLimitLevel( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 레벨 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_LEVEL, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}

		return FALSE;
	}

	if ( !ValidLimitSkillExp1( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 숙련도 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_EXP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}
		return FALSE;
	}

	if ( !ValidLimitSkillExp2( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 숙련도 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_EXP, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}
		return FALSE;
	}

	if ( !ValidLimitVit( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 체력 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_VIT, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}
		return FALSE;
	}

	if ( !ValidLimitSpr( rItemSlot ) )
	{
		if ( bOuputMsg )
		{
			// 지력 위배되어 [%s] 아이템을 착용 할 수 없습니다
			g_InterfaceManager.GetInterfaceString( eST_ITEM_EQUIP_LIMIT_INT, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItem, INTERFACE_STRING_LENGTH );
			_stprintf( szResult, szMessage, szItem);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult );
		}
		return FALSE;
	}

	return TRUE;
}

BOOL ItemManager::CanEquipByGender(Player* pPlayer, const BASE_ITEMINFO* pInfo)
{
    if ((pPlayer == NULL) || 
        (pInfo == NULL))
    {
        return FALSE;
    }

    BYTE gender = static_cast<BYTE>(pPlayer->GetGender());
    bool can_equip = pInfo->IsCanUseGender(gender);
    return (can_equip == true) ? TRUE : FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
ItemManager::ValidLimitAll( SHORT Player_Limit, SHORT Item_Limit ) const
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	// 착용제한수치 감소 옵션 실제 적용 코드 - 실제 적용되는지 테스트 필요(이태준:091102)
	short decrease_limit_stat = pCharInfo->GetDecreaseLimitStat();

	if( Player_Limit + decrease_limit_stat >= Item_Limit ) return TRUE;

	return FALSE;

}
//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::ValidLimitStr( SCItemSlot & ItemSlot ) const 
{
	// 악세서리 레벨 제한수치
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitStr = pInfo->m_wLimitStr;
	if( 0 == LimitStr ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitStr = pInfo->m_wDivLimitStr;
		}
		
		LimitStr= GlobalFunc::CalcLimitStat( LimitStr, &ItemSlot);

	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
        
	{
		// Divine 아이템 힘제한 수치 증가 : 계산된 값이 LimitStr값이 된다.
		if( ItemSlot.IsDivine() )
		{
			LimitStr = pInfo->m_wDivLimitStr;
		}
		
		LimitStr= GlobalFunc::CalcLimitStat( LimitStr, &ItemSlot);

	}
	else
	{
		DEBUG_CODE( if(LimitStr!=0) DISPMSG("스크립트오류!장비(%s)가아닌데0이아니다\n", pInfo->m_pszName ); );
	}

	return ValidLimitAll( GetSTR(pCharInfo), LimitStr);
}


//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::ValidLimitDex( SCItemSlot & ItemSlot ) const 
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitDex = pInfo->m_wLimitDex;
	if( 0 == LimitDex ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitDex = pInfo->m_wDivLimitDex;
		}
		LimitDex = GlobalFunc::CalcLimitStat( LimitDex, &ItemSlot);

	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		// Divine 아이템 힘제한 수치 증가 : 계산된 값이 LimitStr값이 된다.S
		if( ItemSlot.IsDivine() )
		{
			LimitDex = pInfo->m_wDivLimitDex;
		}
		LimitDex= GlobalFunc::CalcLimitStat( LimitDex, &ItemSlot);


	}
	else
	{
		DEBUG_CODE( if(LimitDex!=0) DISPMSG("스크립트오류!장비(%s)가아닌데0이아니다\n", pInfo->m_pszName ); );
	}

	// 스탯 제한

	return ValidLimitAll( GetDEX(pCharInfo), LimitDex);
}


//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::ValidLimitInt( SCItemSlot & ItemSlot ) const 
{	
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitInt = pInfo->m_wLimitInt;
	if( 0 == LimitInt ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitInt = pInfo->m_wDivLimitInt;
		}
		LimitInt = GlobalFunc::CalcLimitStat( LimitInt, &ItemSlot);
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		// Divine 아이템 힘제한 수치 증가 : 계산된 값이 LimitStr값이 된다.
		if( ItemSlot.IsDivine() == 1 )
		{
			LimitInt = pInfo->m_wDivLimitInt;
		}
	
		LimitInt= GlobalFunc::CalcLimitStat( LimitInt, &ItemSlot);

	}
	else
	{
		DEBUG_CODE( if(LimitInt!=0) DISPMSG("스크립트오류!장비(%s)가아닌데0이아니다\n", pInfo->m_pszName ); );
	}

	// 스탯 제한
	return ValidLimitAll( GetINT(pCharInfo), LimitInt);
}


//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::ValidLimitLevel( SCItemSlot & ItemSlot ) const
{	
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitLV = pInfo->m_LimitEqLevel;
	if( 0 == LimitLV ) return TRUE;


	// 레벨 제한
	if( g_pHero->GetLevel() < LimitLV ) 
	{
		return FALSE;
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL
ItemManager::ValidLimitSkillExp1( SCItemSlot & ItemSlot ) const
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitSkill = pInfo->m_wLimitSkil1;
	if( 0 == LimitSkill ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitSkill = pInfo->m_wDivLimitSkill1;
		}

		LimitSkill= GlobalFunc::CalcLimitStat( LimitSkill, &ItemSlot);
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		if( ItemSlot.IsDivine() )
		{
			LimitSkill = pInfo->m_wDivLimitSkill1;
		}
		LimitSkill= GlobalFunc::CalcLimitStat( LimitSkill, &ItemSlot);

	}
	else
	{
		DEBUG_CODE( if(LimitSkill!=0) DISPMSG("스크립트오류!장비(%s)가아닌데0이아니다\n", pInfo->m_pszName ); );
	}

	// 스탯 제한
	return ValidLimitAll( GetExperty1(pCharInfo), LimitSkill);
}


//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::ValidLimitSkillExp2( SCItemSlot & ItemSlot ) const
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( ItemSlot.GetCode() );
	if( !pInfo ) return FALSE;

	WORD LimitSkill = pInfo->m_wLimitSkil2;
	if( 0 == LimitSkill ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitSkill = pInfo->m_wDivLimitSkill2;
		}

		LimitSkill = GlobalFunc::CalcLimitStat( LimitSkill, &ItemSlot);
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		if( ItemSlot.IsDivine() )
		{
			LimitSkill = pInfo->m_wDivLimitSkill2;
		}

		LimitSkill = GlobalFunc::CalcLimitStat( LimitSkill, &ItemSlot);
	}
	else
	{
		DEBUG_CODE( if(LimitSkill!=0) DISPMSG("스크립트오류!장비(%s)가아닌데0이아니다\n", pInfo->m_pszName ); );
	}

	// 스탯 제한
	return ValidLimitAll( GetExperty2(pCharInfo), LimitSkill);
}

BOOL ItemManager::ValidLimitVit( SCItemSlot & ItemSlot ) const
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	const BASE_ITEMINFO * pInfo = ItemSlot.GetItemInfo();

	WORD LimitVit = pInfo->m_wLimitVit;
	if( 0 == LimitVit ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitVit = pInfo->m_wDivLimitVit;
		}

		LimitVit= GlobalFunc::CalcLimitStat( LimitVit, &ItemSlot);
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		// Divine 아이템 힘제한 수치 증가 : 계산된 값이 LimitVit값이 된다.
		if( ItemSlot.IsDivine() )
		{
			LimitVit = pInfo->m_wDivLimitVit;
		}
	
		LimitVit = GlobalFunc::CalcLimitStat( LimitVit, &ItemSlot);
	}
	else
	{
		//DEBUG_CODE( if(LimitVit!=0) SUNLOG( eFULL_LOG, "[CEquipmentSlotContainer::ValidLimitVit] 스크립트오류!장비(%s)가아닌데0이아니다", pInfo->m_pszName ); );
		assert(!"스크립트오류!장비(%s)가아닌데0이아니다");
	}


	// 스탯 제한
	return ValidLimitAll( GetVIT(pCharInfo), LimitVit );
}

BOOL ItemManager::ValidLimitSpr( SCItemSlot & ItemSlot ) const
{
	PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();			
	BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

	const BASE_ITEMINFO * pInfo = ItemSlot.GetItemInfo();
	if(!pInfo)
		return FALSE;

	WORD LimitSpr = pInfo->m_wLimitSpr;
	if( 0 == LimitSpr ) return TRUE;

	if( ItemSlot.GetItemInfo()->IsWeapon() )
	{
		if( ItemSlot.IsDivine() )
		{
			LimitSpr = pInfo->m_wDivLimitSpr;
		}

		LimitSpr = GlobalFunc::CalcLimitStat( LimitSpr, &ItemSlot);
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if( ItemSlot.GetItemInfo()->IsArmor() || ItemSlot.IsWingItem())
#else
    else if( ItemSlot.GetItemInfo()->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		// Divine 아이템 힘제한 수치 증가 : 계산된 값이 LimitStr값이 된다.
		if( ItemSlot.IsDivine() )
		{
			LimitSpr = pInfo->m_wDivLimitSpr;
		}

		LimitSpr = GlobalFunc::CalcLimitStat( LimitSpr, &ItemSlot);
	}
	else
	{
		assert(!"스크립트오류!장비(%s)가아닌데0이아니다");
	}


	// 스탯 제한
	return ValidLimitAll( GetSPR(pCharInfo), LimitSpr );
}

//----------------------------------------------------------------------------
/**
@remark 
[prev] pSlot == [Item], [Skill], [Quick]

@return
true, false
*/
BOOL
ItemManager::CanLinkQuick( SLOTIDX AtIndex, POSTYPE ToPos, const SCSlot * IN pSlot)
{
	if (!pSlot)
	{
		return FALSE;
	}

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


	if (ST_ITEM  != pSlot->GetSlotType() &&
		ST_SKILL != pSlot->GetSlotType() &&
		ST_QUICK != pSlot->GetSlotType())
	{
		//	퀵창에는 아이템과 스킬만 올려 놓을 수 있습니다
		g_InterfaceManager.GetInterfaceString( eST_QUICK_LINK_ONLY_ITEM_AND_SKILL, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	if (pSlot->GetSlotType() == ST_ITEM)
	{
		SCItemSlot * pItemSlot = (SCItemSlot *)pSlot;
		const BASE_ITEMINFO * pInfo = pItemSlot->GetItemInfo();
		if (!pInfo)
		{
			return FALSE;
		}

		// 착용 / 사용 가능한 아이템
		if (pInfo->m_byMaterialType < eITEM_MATERIAL_TYPE_CAN_EQUIP 
            || (pItemSlot->IsEtheriaItem() || pItemSlot->IsShellItem())
            )
		{
			//	장착 및 사용 가능한 아이템만 퀵 창에 올릴 수 있다.
			g_InterfaceManager.GetInterfaceString( eST_ITEM_MATERIAL_TYPE_CAN_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}
	}
	else
		if (pSlot->GetSlotType() == ST_SKILL)
		{
			ROOT_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetInfo(pSlot->GetCode());

			if (!pInfo->IsSkill())
			{
				//	스타일은 스타일 창에 올려놓을 수 있습니다.
				g_InterfaceManager.GetInterfaceString( eST_STYLE_ONLY_STYLE_SLOT, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

				return FALSE;
			}
		}

		return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::CanLinkStyleQuick( SLOTIDX AtIndex, POSTYPE ToPos, const SCSlot * IN pSlot)
{
	if (!pSlot)
	{
		return FALSE;
	}

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


	if (ST_SKILL		!= pSlot->GetSlotType() &&
		ST_STYLE_QUICK	!= pSlot->GetSlotType())
	{
		//	스타일 창 에는 아이템과 스킬만 올려 놓을 수 있습니다.
		g_InterfaceManager.GetInterfaceString( eST_STYLE_SLOT_ONLY_ITEM_AND_SKILL, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	if (pSlot->GetSlotType() == ST_SKILL)
	{
		ROOT_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetInfo(pSlot->GetCode());

		if (pInfo->IsSkill())
		{
			//	스킬은 퀵 창에 올려놓을 수 있습니다.
			g_InterfaceManager.GetInterfaceString( eST_STYLE_NOT_QUICK_SLOT, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			return FALSE;
		}
	}

	StyleContainer *pContainer = (StyleContainer *)GetContainer(SI_STYLE);
    if (!pContainer)
    {
        return FALSE;
    }

	// 이는 서버에서 일단 막아달라 했고
	// 변경되면 없애자.
	if (!pContainer->IsEmpty(ToPos))
	{
		return FALSE;
	}

	if (ST_STYLE_QUICK == pSlot->GetSlotType())
	{
		StyleSlot * pQuickSlot = (StyleSlot *)pSlot;

		if ( pContainer->IsExistSameCodeItem(pQuickSlot->GetOrgCode())	||
			pContainer->IsExistSameSerialItem(pQuickSlot->GetSerial()) )
		{
			//	이미 해당 슬롯이 등록되어 있습니다.
			g_InterfaceManager.GetInterfaceString( eST_SLOT_NOT_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}
	}
	else
	{
		if ( pContainer->IsExistSameCodeItem(pSlot->GetCode() )	||
			pContainer->IsExistSameSerialItem(pSlot->GetSerial() ) )
		{
			//	이미 해당 슬롯이 등록되어 있습니다.
			g_InterfaceManager.GetInterfaceString( eST_SLOT_NOT_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}
	}	

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
eRANK_LEVEL							
ItemManager::GetCompRankLevel(SCItemSlot & rItemSlot )
{
	switch (rItemSlot.GetRank())
	{
	case RANK_F: case RANK_E: case RANK_D: case RANK_C: 
		return RANK_LOW;
	case RANK_B: case RANK_MA: case RANK_A: 
		return RANK_MIDDLE;
	case RANK_PA: case RANK_MS: case RANK_S: case RANK_PS: 
		return RANK_HIGH; 
	}

	assert( !"잘못된 랭크입니다." );
	return RANK_LOW;
}


//----------------------------------------------------------------------------
/**
*/
eENCHANTLEVEL						
ItemManager::GetCompEnchantLevel(SCItemSlot & rItemSlot )
{
	switch( rItemSlot.GetEnchant() )
	{
	case 0: case 1: case 2: case 3: case 4: case 5: return ENCHANT_LOW;
	case 6: case 7: case 8:  return ENCHANT_MIDDLE;
	case 9: case 10: case 11: case 12: return ENCHANT_HIGH;
#ifdef _NA_003966_20111227_ADD_ENCHANT
    case 13: case 14: case 15: return ENCHANT_HIGHEST;
#endif //_NA_003966_20111227_ADD_ENCHANT
	}

	assert( !"잘못된 인첸트입니다." );
	return ENCHANT_LOW;
}


BOOL								
ItemManager::CanMoveEquipItem( SLOTIDX AtIndex, POSTYPE AtPos, BOOL bOutputMsg /*= FALSE*/ )
{
	if (!g_pHero)
    {
        return FALSE;
    }

	SCSlotContainer *pAtContainer = g_pHero->GetSlotContainer( AtIndex );

	if (!pAtContainer)
    {
		return FALSE;
    }

	// 비워 있는냐?
	if (pAtContainer->IsEmpty(AtPos)) 
    {
		return FALSE;
    }

	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos );

	if ( !rItemSlot.GetItemInfo() )
	{
		return FALSE;
	}

	POSTYPE EQPos = 255;

	BOOL bCorrectPos = GetEquipPosition(rItemSlot, EQPos);
	EQPos = AtPos;
	if (!bCorrectPos )
	{
		return FALSE;
	}

	BOOL bEmpty = IsEmptySlotPos(AtIndex, EQPos);
	if (bEmpty)
	{
		// 넘어간다
		return TRUE;
	}
	else
	{
		// 비워있지 않다면 부가 작업
		// 1. 탈착 할 수 있나? ( 장착 아이템에 있는 아이템을 )
		BOOL bUnEquipItem = CanUnEquipItem(EQPos, TRUE);
		if ( !bUnEquipItem )
		{
			return FALSE;	
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL	
ItemManager::CanMoveUnEquipItem( SLOTIDX AtIndex, POSTYPE AtPos, BOOL bOutputMsg /*= FALSE*/ )
{
	if (!g_pHero) return FALSE;

	SCSlotContainer * pAtContainer	= g_pHero->GetSlotContainer( AtIndex );

	if( !pAtContainer)
		return FALSE;

	// 비워 있는냐?
	if ( pAtContainer->IsEmpty( AtPos ) ) 
		return FALSE;

	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos );

	if ( !rItemSlot.GetItemInfo() )
	{
		return FALSE;
	}

	POSTYPE EQPos = 255;

	BOOL bCorrectPos = GetEquipPosition(rItemSlot, EQPos);
	EQPos = AtPos;
	if (!bCorrectPos )
	{
		return FALSE;
	}

	BOOL bEmpty = IsEmptySlotPos(AtIndex, EQPos);
	if (bEmpty)
	{
		// 넘어간다
		return TRUE;
	}
	else
	{
		// 비워있지 않다면 부가 작업
		// 1. 탈착 할 수 있나? ( 장착 아이템에 있는 아이템을 )
		BOOL bUnEquipItem = CanUnEquipItem(EQPos, TRUE);
		if ( !bUnEquipItem )
		{
			return FALSE;	
		}
	}

	return TRUE;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::CanUnEquipItem( POSTYPE ToPos, BOOL bOutputMsg)
{
    return TRUE; //무조건 탈착은 가능해진다.
}



//----------------------------------------------------------------------------
/**
*/
BOOL 
ItemManager::CanEquip( SLOTIDX AtIndex,POSTYPE AtPos, POSTYPE ToPos, BOOL bOutputMsg /*= TRUE*/, BOOL bTransformCheck  )
{
	if (!g_pHero) return FALSE;
    
    if(bTransformCheck && g_pHero->GetTransformData().GetTransFormProcessType()!=TRANSFORM_PROCESS_NONE)
    {
        return FALSE;
    }

    if(g_pHero->IsTransForm()&&bTransformCheck)
	{
		return FALSE;
	}

	SCSlotContainer * pAtContainer	= g_pHero->GetSlotContainer( AtIndex );
	if( !pAtContainer)
		return FALSE;
	// 비워 있는냐?
	if( pAtContainer->IsEmpty( AtPos ) ) 
		return FALSE;

	SCSlot & rFromSlot = pAtContainer->GetSlot( AtPos );
	assert(rFromSlot.GetSlotType() == ST_ITEM);
	if (rFromSlot.GetSlotType() != ST_ITEM )
	{
		return FALSE;
	}

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( rFromSlot.GetCode() );  
	if (!pInfo)
	{
		return FALSE;
	}

	// 장착 가능한가?
	if( 0 == pInfo->m_byMaterialType ) 
	{
		return FALSE;		
	}

    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	// 장착 가능 클레스인가 ?
	if ( !CanEquipClass(g_pHero, pInfo ) )
	{
		if ( bOutputMsg )
		{
			//	장착할 수 없는 클레스 입니다.
			g_InterfaceManager.GetInterfaceString( eST_CAN_NOT_EQUIP_CLASS, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		return FALSE;
	}

    if (CanEquipByGender(g_pHero, pInfo) == FALSE)
    {
        if ( bOutputMsg )
        {
            // 70294 장착할 수 없는 클레스 입니다.(eST_CAN_NOT_EQUIP_CLASS)
            g_InterfaceManager.GetInterfaceString(eST_CAN_NOT_EQUIP_CLASS, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        return FALSE;
    }

	SCSlotContainer * pContainer= g_pHero->GetSlotContainer( SI_EQUIPMENT );

	//칼버스 특수 아이템을 찰려고 할때
	if(pInfo->m_wType==eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY&&pContainer)
	{

		//이미 차고 있는지 검사 
		SCSlot & rAcc1Slot = pContainer->GetSlot( EquipmentContainer::EQUIP_SACCESSORY1 );
		BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rAcc1Slot.GetCode() );  
		//1
		if(pItemInfo&&
			pItemInfo->m_wType==eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY)
		{
			if(ToPos== EquipmentContainer::EQUIP_SACCESSORY2||
				ToPos== EquipmentContainer::EQUIP_SACCESSORY3)
			{

				if ( bOutputMsg )
				{
					//	해당 아이템은 중복 착용 할수 없음 
					g_InterfaceManager.GetInterfaceString( eST_NOT_USE_DUPLICATION, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}

				return FALSE;			
			}
		}

		//2
		SCSlot & rAcc2Slot = pContainer->GetSlot( EquipmentContainer::EQUIP_SACCESSORY2 );
		pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rAcc2Slot.GetCode() );  

		if(pItemInfo&&
			pItemInfo->m_wType==eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY)
		{
			if(ToPos== EquipmentContainer::EQUIP_SACCESSORY1||
				ToPos== EquipmentContainer::EQUIP_SACCESSORY3)
			{

				if ( bOutputMsg )
				{
					//	해당 아이템은 중복 착용 할수 없음 
					g_InterfaceManager.GetInterfaceString( eST_NOT_USE_DUPLICATION, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}

				return FALSE;			
			}
		}

		//3
		SCSlot & rAcc3Slot = pContainer->GetSlot( EquipmentContainer::EQUIP_SACCESSORY3 );
		pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rAcc3Slot.GetCode() );  

		if(pItemInfo&&
			pItemInfo->m_wType==eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY)
		{
			if(ToPos== EquipmentContainer::EQUIP_SACCESSORY1||
				ToPos== EquipmentContainer::EQUIP_SACCESSORY2)
			{

				if ( bOutputMsg )
				{
					//	해당 아이템은 중복 착용 할수 없음 
					g_InterfaceManager.GetInterfaceString( eST_NOT_USE_DUPLICATION, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}

				return FALSE;			
			}
		}

	}


	if (GENERALPARAM->IsNet())
	{
		// 제한 사항.
		if ( !CanEquipLimit((SCItemSlot &)rFromSlot, bOutputMsg)) 
		{
			return FALSE;
		}
	}


	// 제대로된 장착 위치인가?
	switch( pInfo->m_wEqPos )
	{
	case eEQUIPCONTAINER_PROTECTOR:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_PROTECTOR );
		}
		break;

	case eEQUIPCONTAINER_RING1:
	case eEQUIPCONTAINER_RING2:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_RING1 || 
				ToPos == EquipmentContainer::EQUIP_RING2 );
		}
		break;

	case eEQUIPCONTAINER_WEAPON:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_WEAPON1 );
		}
		break;
	case eEQUIPCONTAINER_SHIRTS:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_SHIRTS );
		}
		break;
	case eEQUIPCONTAINER_BOOTS:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_BOOTS );
		}
		break;
	case eEQUIPCONTAINER_PANTS:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_PANTS );
		}
		break;
	case eEQUIPCONTAINER_BELT:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_BELT );
		}
		break;
	case eEQUIPCONTAINER_HELMET:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_HELMET );
		}
		break;
	case eEQUIPCONTAINER_ARMOR:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_ARMOR );
		}
		break;
	case eEQUIPCONTAINER_GLOVE:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_GLOVE );
		}
		break;
	case eEQUIPCONTAINER_NECKLACE:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_NECKLACE );
		}
		break;

	case eEQUIPCONTAINER_SACCESSORY1:
	case eEQUIPCONTAINER_SACCESSORY2:
	case eEQUIPCONTAINER_SACCESSORY3:
		{
            return BOOL(ToPos == EquipmentContainer::EQUIP_SACCESSORY1 || 
                        ToPos == EquipmentContainer::EQUIP_SACCESSORY2 ||
                        ToPos == EquipmentContainer::EQUIP_SACCESSORY3);

		}
		break;

	case eEQUIPCONTAINER_CHARGE1:
	case eEQUIPCONTAINER_CHARGE2:
	case eEQUIPCONTAINER_CHARGE3:
	case eEQUIPCONTAINER_CHARGE4:
	case eEQUIPCONTAINER_CHARGE5:
		{
			int iExtraChargeItemSlot = 0;
			if( g_pHero && g_pHero->GetCharInfo() )
			{
				iExtraChargeItemSlot = g_pHero->GetCharInfo()->m_ExtraEquipCashSlotCount;
			}

			bool bCanEquip = (ToPos == EquipmentContainer::EQUIP_CHARGE1|| 
				ToPos == EquipmentContainer::EQUIP_CHARGE2 ||
				ToPos == EquipmentContainer::EQUIP_CHARGE3);

			if( iExtraChargeItemSlot == 2)
			{
				bCanEquip = (bCanEquip || ToPos == EquipmentContainer::EQUIP_CHARGE4 ||
					ToPos == EquipmentContainer::EQUIP_CHARGE5);
			}
			return bCanEquip;
		}
#ifdef _GS_GSP_REMOVE_PCROOM_SLOT
	//no operation
#else
	case eEQUIPCONTAINER_PC_ROOM1:
	case eEQUIPCONTAINER_PC_ROOM2:
	case eEQUIPCONTAINER_PC_ROOM3:
		{
			return BOOL(ToPos == EquipmentContainer::EQUIP_PC_ROOM1 ||
				ToPos == EquipmentContainer::EQUIP_PC_ROOM2 ||
				ToPos == EquipmentContainer::EQUIP_PC_ROOM3); 
		}
		break;
#endif//_GS_GSP_REMOVE_PCROOM_SLOT
	default:
		{
			return FALSE;
		}
	}


	return TRUE;
}

BOOL ItemManager::IsExistCompositeMaterials(
	eITEMCOMPOSITECODE eItemCompositeType,
	SLOTIDX fromSlotIndex)
{
	sITEMCOMPOSITE *pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(eItemCompositeType);
    if (!pInfo)
    {
        return FALSE;
    }

	SCSlotContainer *pContainer = GetContainer( fromSlotIndex );
    if (!pContainer)
    {
        return FALSE;
    }

	BOOL bRet = FALSE;

	// 필요 아이템 
	int i = 0;
	for (; i < pInfo->m_byMatCount; ++i)
	{
		CODETYPE ItemCode		= pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode;
		int		 iMaterialCount = pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum; 
		int		 iAccMaterialCount = 0;	// 누적 Count
		BASE_ITEMINFO * pBaseInfo = (BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(ItemCode);
		if (!pBaseInfo)
		{
			// 스크립트 정보가 잘못되었다.
			return FALSE;
		}

		int iSlotIndex = 0;
		for (; iSlotIndex < pContainer->GetMaxSlotNum(); ++iSlotIndex )
		{
			if (pContainer->IsEmpty(iSlotIndex))
				continue;

			SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(iSlotIndex);

			// 아이템 코드가 같으면 
			if (rItemSlot.GetCode() == ItemCode)
			{
				if ( iMaterialCount > 1)
				{
					iMaterialCount -= rItemSlot.GetNum();
					if (iMaterialCount <= 0)
					{
						bRet = TRUE;
						break;
					}
				}
				else
				{
					iMaterialCount = 0;
					bRet = TRUE;
					break;
				}
			}
		}

		if ( iMaterialCount > 0 )
		{
			return FALSE;
		}
	}

	// 필요 돈
	if (g_pHero && g_pHero->GetMoney() >= pInfo->m_Money )
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;	
	}


	return bRet;
}

int                                 
ItemManager::GetSlotIndex(SLOTIDX FromSlotIdx,eWASTETYPE type)
{
	SCSlotContainer * pContainer = GetContainer( FromSlotIdx );
	if ( !pContainer )
	{
		return -1;
	}

	for ( int i = 0; i < pContainer->GetMaxSlotNum(); ++i)
	{
		if ( pContainer->IsEmpty(i) )
			continue;

		SCSlot & rSlot = pContainer->GetSlot(i);

		BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rSlot.GetCode() );
		if ( pItemInfo &&pItemInfo->m_byWasteType==type) 
		{
			return i;
		}
	}

	return -1;
}

BOOL								
ItemManager::IsExistItem(SLOTIDX from_slot_index, 
                         CODETYPE item_code, 
                         int item_num, 
                         ItemType* item_type_ptr/* = NULL*/, 
                         DWORD item_type_index/* = 0*/)
{
    SCSlotContainer* container_ptr = GetContainer(from_slot_index);
    if (container_ptr == NULL)
    {
        return FALSE;
    }

    if (item_num <= 0)
    {
        return FALSE;
    }

    BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info_ptr == NULL) 
    {
        return FALSE;
    }

    BOOL is_exist = FALSE;

    //슬롯을 다돌면서 동일한아이템코드및 갯수를 체크해서 존재여부확인
    int max_slot_num = container_ptr->GetMaxSlotNum();
    bool is_found_item = false;
    for (int slot_pos = 0; slot_pos < max_slot_num; ++slot_pos)
    {
        if (container_ptr->IsEmpty(slot_pos))
        {
            continue;
        }

        is_found_item = false;
        SCItemSlot& item_slot_ref =  static_cast<SCItemSlot&>(container_ptr->GetSlot(slot_pos));
        if (item_slot_ref.GetCode() == item_code)
        {
            //슬롯추가정보(인첸,랭크,디바) 없을땐 기본만체크
            if(item_type_ptr != NULL)
            {
                if((item_slot_ref.GetEnchant() == item_type_ptr->GetEnchant()) &&
                    (item_slot_ref.GetRank() == item_type_ptr->GetRank()) &&
                    (nsSlot::ItemTypeChanger::IsEqualItemType(item_slot_ref, item_type_ptr) == true))
                {
                    is_found_item = true;
                }
            }
            else
            {
                if (item_type_index == 0)
                {
                    if ((item_slot_ref.IsEtheriaItem() == false) && 
                        (item_slot_ref.IsShellItem() == false))
                    {
                        is_found_item = true;
                    }
                }
                else
                {
                    DWORD item_type_value = nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot_ref);
                    if ((item_type_index - 1) == item_type_value)
                    {
                        is_found_item = true;
                    }
                }
            }

            if (is_found_item == true)
            {
                if (item_slot_ref.GetNum() >= item_num)
                {
                    is_exist = TRUE;
                    break;
                }
                else
                {
                    item_num -= item_slot_ref.GetNum();
                }
            }
        }
    }

    return is_exist;
}

BOOL 
ItemManager::SpendItem(SLOTIDX from_slot_index, 
                       CODETYPE item_code, 
                       int item_num, 
                       ItemType* item_type_ptr/* = NULL*/, 
                       DWORD item_type_index/* = 0*/)
{
    SCSlotContainer* container_ptr = GetContainer(from_slot_index);
    if (container_ptr == NULL)
    {
        return FALSE;
    }

    if (item_num <= 0)
    {
        return FALSE;
    }

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL) 
    {
        return FALSE;
    }

    BOOL result = FALSE;
    bool is_found_item = false;
    int max_slot_num = container_ptr->GetMaxSlotNum();
    for (int slot_pos = 0; slot_pos < max_slot_num; ++slot_pos)
    {
        if (container_ptr->IsEmpty(slot_pos))
        {
            continue;
        }

        is_found_item = false;
        SCItemSlot& item_slot_ref =  static_cast<SCItemSlot&>(container_ptr->GetSlot(slot_pos));
        if (item_slot_ref.GetCode() == item_code)
        {
            //슬롯추가정보(인첸,랭크,디바) 없을땐 기본만체크
            if(item_type_ptr)
            {
                if ((item_slot_ref.GetEnchant() == item_type_ptr->GetEnchant()) && 
                    (item_slot_ref.GetRank() == item_type_ptr->GetRank()) &&
                    (nsSlot::ItemTypeChanger::IsEqualItemType(item_slot_ref, item_type_ptr) == true))
                {
                    is_found_item = true;
                }

            }
            else
            {
                if (item_type_index == 0)
                {
                    if ((item_slot_ref.IsEtheriaItem() == false) && 
                        (item_slot_ref.IsShellItem() == false))
                    {
                        is_found_item = true;
                    }
                }
                else
                {
                    DWORD item_type_value = nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot_ref);
                    if ((item_type_index - 1) == item_type_value)
                    {
                        is_found_item = true;
                    }
                }
            }

            if (is_found_item == true)
            {
                if ( item_slot_ref.GetNum() < item_num )
                {
                    item_num -= item_slot_ref.GetNum();
                    container_ptr->DeleteSlot(slot_pos, NULL);
                }
                else if (item_slot_ref.GetNum() == item_num)
                {
                    container_ptr->DeleteSlot(slot_pos, NULL);
                    result = TRUE;
                    break;
                }
                else
                {
                    item_slot_ref.SetNum(item_slot_ref.GetNum() - item_num);
                    result = TRUE;
                    break;
                }
            }
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL								
ItemManager::DeleteItem(SLOTIDX FromSlotIdx, POSTYPE FromPos, int iItemNum, eDELETETOTALTYPE eDeleteType /*= DELETE_NONE*/)
{
	TCHAR szItemName[INTERFACE_STRING_LENGTH];
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
	SCSlotContainer * pContainer = GetContainer( FromSlotIdx );
	if ( !pContainer )
	{
		return FALSE;
	}

	if ( iItemNum <= 0 )
	{
		return FALSE;
	}

	if ( pContainer->IsEmpty(FromPos) )
	{
		return FALSE;
	}

	SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(FromPos);

	g_InterfaceManager.GetItemName(
		rSlot.GetCode(), 
		szItemName, 
		INTERFACE_STRING_LENGTH );

	if ( rSlot.GetNum() > iItemNum )
	{
		rSlot.SetNum( rSlot.GetNum() - iItemNum	);
	}
	else
	{
		pContainer->DeleteSlot(FromPos, NULL);
	}

	switch (eDeleteType)
	{
	case COMPOSE_USE:
		{
			g_InterfaceManager.GetInterfaceString(
				5709, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			Snprintf( 
				szResult, 
				INTERFACE_STRING_LENGTH-1, 
				szMessage, 
				szItemName);
		}
		break;

	case QUEST_DELETE:
		{
			g_InterfaceManager.GetInterfaceString(
				5709, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			Snprintf( 
				szResult, 
				INTERFACE_STRING_LENGTH-1, 
				szMessage, 
				szItemName);
		}
		break;

	case DELETE_STAT_INIT_ITEM:
		{
			// 5952	[%s]아이템 %d개가 소모되었습니다.
			g_InterfaceManager.GetInterfaceString(
				5952, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			Snprintf( 
				szResult, 
				INTERFACE_STRING_LENGTH-1, 
				szMessage, 
				szItemName, iItemNum);

			if (g_pHero)
			{
				g_pHero->OnUseItemWithCoolTime( rSlot.GetCode() );
			}
		}
		break;

	default:
		return TRUE;
		break;
	}

	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);

	return TRUE;
}

BOOL ItemManager::DeleteItemCmd(SLOTIDX FromSlotIdx, POSTYPE FromPos, int iItemNum, eITEM_DELETE_RESULT eDeleteType)
{
	TCHAR szItemName[INTERFACE_STRING_LENGTH];
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};

	SCSlotContainer* pContainer = GetContainer(FromSlotIdx);
	if (!pContainer)
	{
		return FALSE;
	}

	if (iItemNum <= 0)
	{
		return FALSE;
	}

	if (pContainer->IsEmpty(FromPos))
	{
		return FALSE;
	}

	SCItemSlot& rSlot = (SCItemSlot&)pContainer->GetSlot(FromPos);

	g_InterfaceManager.GetItemName(
		rSlot.GetCode(), 
		szItemName, 
		INTERFACE_STRING_LENGTH );

	if (rSlot.GetNum() > iItemNum)
	{
		rSlot.SetNum(rSlot.GetNum() - iItemNum);
	}
	else
	{
		if (rSlot.IsWayPointItem())
		{
			// 6040	룬스톤의 남은 사용시간이 종료되었습니다.
			TCHAR szWayPointDeleteMsg[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(6040, szWayPointDeleteMsg, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szWayPointDeleteMsg);
		}			



#ifdef _YMS_AUTOBOT_GAME
        const BASE_ITEMINFO* item_info = rSlot.GetItemInfo();
        if ((item_info != NULL) && (item_info->m_wType == eITEMTYPE_AUTO_HUNTING))
        {
            //! 아이템이 만료 되어서 왔기때문에 마을로 보낸다.
            Autobot::Instance()->Stop(true);
            g_InterfaceManager.ShowDialog(uiAutobotPlay::kDialog_ID, FALSE);
        }
#endif //_YMS_AUTOBOT_GAME

		pContainer->DeleteSlot(FromPos, NULL);
	}

	switch (eDeleteType)
	{
	case RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM: // 유료화 아이템 사용 기간 만료.
		{
			// 5870	[%s] 아이템의 사용 기간이 만료되어 삭제되었습니다.
			g_InterfaceManager.GetInterfaceString(
				5870, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			Snprintf( 
				szResult, 
				INTERFACE_STRING_LENGTH-1, 
				szMessage, 
				szItemName);

			uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if (pSystemMan)
            {
				pSystemMan->Sys_Verify(szResult);
            }

			return TRUE;
		}
		break;

	case RC_ITEM_DELETE_PORTAL_SUCCESS:
		{
			// ITEM_USE_ACK 가오지 않고 DELETE가 오기때문에 여기서 한다.
			if (g_pHero)
			{
				g_pHero->OnUseItemWithCoolTime(rSlot.GetCode());
			}			
		}
		break;

	case RC_ITEM_DELETE_PROTECT_DROP:
		{
			//5952	[%s]아이템 %d개가 소모되었습니다.
			g_InterfaceManager.GetInterfaceString(
				5952, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			Snprintf( 
				szResult, 
				INTERFACE_STRING_LENGTH-1, 
				szMessage, 
				szItemName, 
				iItemNum);
		}
		break;

	case RC_ITEM_DELETE_REVENGE_STONE:	
	default:
		return TRUE;
		break;
	}

	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);

	return TRUE;
}

BOOL 
ItemManager::AutoRegisterCompositeMaterials(
	eITEMCOMPOSITECODE eItemCompositeType, 
	SLOTIDX	atSlotIndex /*= SI_ENCHANT*/,
	SLOTIDX	fromSlotIndex /*= SI_INVENTORY*/)
{
	sITEMCOMPOSITE * pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(eItemCompositeType);

	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(atSlotIndex);
	SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer(fromSlotIndex);

	// 필요 메터리얼 등록

	BOOL	bRet = FALSE;

	// 필요 아이템 
	int i = 0;
	for (; i < pInfo->m_byMatCount; ++i)
	{
		CODETYPE ItemCode		= pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode;
		int		 iMaterialCount = pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum; 

		SCItemSlot	ItemSlot;
		ItemSlot.SetCode((SLOTCODE)ItemCode);
		ItemSlot.SetNum(iMaterialCount);

		pAtContainer->InsertSlot(i+1, ItemSlot);
	}

	return TRUE;
}


int	
ItemManager::GetOverlapItemCount( CODETYPE code, SLOTIDX AtIndex/* = SI_INVENTORY*/)
{
	int iCount = 0;

	SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(AtIndex);
    if (!pAtContainer)
    {
        return iCount;
    }

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
			continue;

		SCSlot & rSlot = pAtContainer->GetSlot(i);

		if ( code != rSlot.GetCode() )
			continue;

		iCount += rSlot.GetNum();
	}

	return iCount;
}	

BOOL ItemManager::FindActiveChargeItem(eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex)
{
	SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(AtIndex); 
    if (!pAtContainer)
    {
        return FALSE;
    }

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
			continue;

		SCSlot & rSlot = pAtContainer->GetSlot(i);
		if ( rSlot.GetSlotType() == ST_ITEM )
		{
			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
			if ( rItemSlot.GetItemInfo()->m_wType == eItemTypeInfo)
			{
                BYTE charge_subtype = rItemSlot.GetItemRuleInfo().charge_sub_type;
				//1.기존 지휘봉:생성시동작
				if (charge_subtype == eCHARGE_SUB_FIXED_AMOUNT)
				{
					rOutPos = i;
					return TRUE;
				}//2.추가된 지휘봉:클릭시 동작
				else if(charge_subtype == eCHARGE_SUB_FIXED_AMOUNT_EX && rItemSlot.IsProgressTime())
				{
					rOutPos = i;
					return TRUE;
				}
			}
		}			
	}
	return FALSE;
}


//------------------------------------------------------------------------------
/** FindFirstItem
author : y2jinc

desc :	퀵에서 아이템 아이템을 사용할때 해당 컨테이너에서 존재하는
아이템의 첫번째 위치를 받아서 리턴해준다. 

param : POSTYPE & OUT rOutPos - 첫번째 위치
만약 존재하지 않다면 
FALSE 를 리턴한다. 	
*/
BOOL	
ItemManager::FindFirstItem( CODETYPE code, POSTYPE & OUT rOutPos, SLOTIDX AtIndex /*= SI_INVENTORY*/)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(AtIndex); 

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
			continue;

		SCSlot & rSlot = pAtContainer->GetSlot(i);
		if (rSlot.GetCode() == code)
		{
			rOutPos = i;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL 
ItemManager::FindFirstItem( eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex /*= SI_INVENTORY*/)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(AtIndex); 

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
			continue;

		SCSlot & rSlot = pAtContainer->GetSlot(i);
		if ( rSlot.GetSlotType() == ST_ITEM )
		{
			SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
			if ( rItemSlot.GetItemInfo()->m_wType == eItemTypeInfo)
			{
				rOutPos = i;
				return TRUE;
			}
		}			
	}

	return FALSE;
}
//------------------------------------------------------------------------------
SCItemSlot* ItemManager::FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type)
{
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);		
    if (slot_container == NULL)
    {
        return NULL;
    }

    POSTYPE max_slot = slot_container->GetMaxSlotNum();

    for (int i = 0; i < max_slot; ++i)
    {
        if ((slot_container->IsEmpty(i) == TRUE) || 
            (slot_container->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot& item_slot = (SCItemSlot&)(slot_container->GetSlot(i));
        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if (item_info != NULL)
        {
            if (item_info->m_wType == item_type)
            {
                return &item_slot;
            }
        }
    }

    return NULL;
}
//------------------------------------------------------------------------------
SCItemSlot* ItemManager::FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type, eWASTETYPE waste_type)
{
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);		
    if (slot_container == NULL)
    {
        return NULL;
    }

    POSTYPE max_slot = slot_container->GetMaxSlotNum();

    for (int i = 0; i < max_slot; ++i)
    {
        if ((slot_container->IsEmpty(i) == TRUE) || 
            (slot_container->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot& item_slot = (SCItemSlot&)(slot_container->GetSlot(i));
        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if (item_info != NULL)
        {
            if ((item_info->m_wType == item_type) && 
                (item_info->m_byWasteType == waste_type))
            {
                return &item_slot;
            }
        }
    }

    return NULL;
}
//------------------------------------------------------------------------------
SCItemSlot* ItemManager::FindFirstItemSlot(SLOTIDX AtSlotIdx, SLOTCODE item_code)
{
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);		
    if (slot_container == NULL)
    {
        return NULL;
    }

    POSTYPE max_slot = slot_container->GetMaxSlotNum();

    for (POSTYPE i = 0; i < max_slot; ++i)
    {
        if ((slot_container->IsEmpty(i) == TRUE) || 
            (slot_container->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot& item_slot = (SCItemSlot&)(slot_container->GetSlot(i));
        if (item_slot.GetCode() == item_code)
        {
            return &item_slot;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
SCItemSlot* ItemManager::FindFirstItemSlot(SLOTIDX AtSlotIdx, 
                                           SLOTCODE item_code, 
                                           NUMTYPE less_count)
{
    SCSlotContainer* slot_container = GetContainer(AtSlotIdx);		
    if (slot_container == NULL)
    {
        return NULL;
    }

    POSTYPE max_slot = slot_container->GetMaxSlotNum();

    for (int i = 0; i < max_slot; ++i)
    {
        if ((slot_container->IsEmpty(i) == TRUE) || 
            (slot_container->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }

        SCItemSlot& item_slot = (SCItemSlot&)(slot_container->GetSlot(i));
        if ((item_slot.GetCode() == item_code) && (item_slot.GetNum() <= less_count))
        {
            return &item_slot;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/** UpdateQuickInfo
author : y2jinc
desc :	퀵아이템의 정보를 갱신한다.
*/
VOID ItemManager::UpdateQuickInfo(eSlotContainerIndex container_index)
{
	SCSlotContainer* pAtContainer = g_pHero->GetSlotContainer(container_index);
    if (!pAtContainer)
    {
        return;
    }

	SCSlotContainer* pInvenContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
    if (!pInvenContainer)
    {
        return;
    }

	// skill은 따로 가져와야함.

	// quick slot 을 확인해가면서 가각의 정보를 확인한다. 

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
        {
			continue;
        }

		QuickSlot& rQuickSlot = (QuickSlot&)pAtContainer->GetSlot(i);

		switch (rQuickSlot.GetOrgSlotType())
		{
		case ST_ITEM:
			{
				POSTYPE FromPos;
				BASE_ITEMINFO* pItemInfo = NULL;

				pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rQuickSlot.GetOrgCode());
				if (!pItemInfo)
				{
					continue;
				}

				if (pItemInfo->IsCanUseWaste())
				{

					BOOL bRet = ItemManager::Instance()->FindFirstItem(rQuickSlot.GetOrgCode(), FromPos);						
					if (!bRet)
					{
						rQuickSlot.SetOverlapCount(0);
						rQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
                        rQuickSlot.SetOrgPos(INVALID_POSTYPE_VALUE);
						continue;
					}	

					//GS_BUG_FIX:에테르 탄환은예외처리,첫번째 아이템을 참조하지않는다
					if (pItemInfo->m_wType == eITEMTYPE_ETHER_BULLET)
					{
						FromPos = rQuickSlot.GetOrgPos();
					}

					

					rQuickSlot.SetOrgPos(FromPos);

					SCItemSlot& rItemSlot = (SCItemSlot&)pInvenContainer->GetSlot(FromPos);

					rQuickSlot.SetStatus(SLOT_RENDER_STATE_ACTIVATED);
					if (rItemSlot.IsOverlap()) 
					{
						int OverlapCount = this->GetOverlapItemCount(rQuickSlot.GetOrgCode());

						//GS_BUG_FIX:에테르 탄환은예외처리,겹쳐지지만 총갯수를 계산하지않겠다
						if (pItemInfo->m_wType == eITEMTYPE_ETHER_BULLET) 
						{
                            OverlapCount = rItemSlot.GetNum();
						}

						assert(OverlapCount > 0);
						rQuickSlot.SetOverlapCount(OverlapCount);
						rQuickSlot.SetOrgSlot(&rItemSlot);
					}
					else
					{
						if (pItemInfo->m_wType == eITEMTYPE_ETHER_BULLET)
						{
							rQuickSlot.SetOverlapCount(0);
							rQuickSlot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
						}
					}
				}
				else if (pItemInfo->IsCanOnlyUseWaste() || 
                         SCItemSlot::CanEquip(pItemInfo))
				{
					SCSlotContainer* pFromContainer = GetContainer(rQuickSlot.GetOrgSlotIndex());
					if (!pFromContainer)
					{
						rQuickSlot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
						continue;
					}

					//GS_BUGFIX_QUICK:이큅에 있다면 볼것도 없이 비활성
					if (rQuickSlot.GetOrgSlotIndex() == SI_EQUIPMENT)
					{
						rQuickSlot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
						continue;
					}

					if (pFromContainer->IsEmpty(rQuickSlot.GetOrgPos()))
					{
                        // unlink
                        if (container_index == SI_QUICK)
                        {
                            InventoryDialog* inventory_dialog = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
                            if (inventory_dialog != NULL)
                            {   
                                QuickContainer* quick_container = (QuickContainer*)pAtContainer;
                                if (quick_container->is_item_move() == false)
                                {
                                    SendQuickItemRemoveMsg(i);
                                }
                            }
                        }
						rQuickSlot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
						continue;
					}

					SCSlot& rOrgSlot = pFromContainer->GetSlot(rQuickSlot.GetOrgPos());
					if (rOrgSlot.GetCode() != rQuickSlot.GetOrgCode())
					{
						rQuickSlot.SetStatus(SLOT_RENDER_STATE_DEACTIVATED);
						continue;
					}

					rQuickSlot.SetStatus(SLOT_RENDER_STATE_ACTIVATED);
				}
			}
			break;

		case ST_SKILL:
			break;
		}
	}

}


//----------------------------------------------------------------------------
/** 퀵 창에 링크된 창착 가능 아이템 정보 업데이트 
*/
VOID		
ItemManager::UpdateQuickForEquipItemAll()
{
	SCSlotContainer *pAtContainer = g_pHero->GetSlotContainer( SI_QUICK);
    SUN_ASSERT(pAtContainer);
    if (!pAtContainer)
    {
        return;
    }

	SCSlotContainer *pEquipContainer = g_pHero->GetSlotContainer( SI_EQUIPMENT);
    SUN_ASSERT(pEquipContainer);
    if (!pEquipContainer)
    {
        return;
    }
	

	if (!g_pHero)
    {
		return;
    }

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); i++)
	{
		if ( pAtContainer->IsEmpty(i) )
		{
			continue;
		}

		QuickSlot & rQuickSlot = (QuickSlot &)pAtContainer->GetSlot(i);

		if (rQuickSlot.GetOrgSlotType() != ST_ITEM)
		{
			continue;
		}

		BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rQuickSlot.GetOrgCode() );
		assert(pItemInfo);
		if (!pItemInfo)
		{
			OutputDebugStringA("UpdateQuickForEquipItem:: 잘못된 정보!!>> 퀵창 담당자 확인!!");
			continue;
		}

        if (SCItemSlot::CanEquip(pItemInfo) == false)
		{
			continue;
		}

		SLOTIDX OrgSlotIdx = rQuickSlot.GetOrgSlotIndex();
		POSTYPE OrgPos	   = rQuickSlot.GetOrgPos();
		SCSlotContainer * pOrgContainer = this->GetContainer( OrgSlotIdx );
		if ( !pOrgContainer )
		{
			continue;
		}

		SCSlot & rSlot = pOrgContainer->GetSlot( OrgPos );
		if ( rSlot.GetCode() != rQuickSlot.GetOrgCode() )
		{
			// 실제 가리키고 있는 코드와 다르다


			// 장착창에 해당 아이템이 등록되어 있나?
			if ( !pEquipContainer->IsEmpty( (POSTYPE)pItemInfo->m_wEqPos ) )
			{
				SCSlot & rSlot = pEquipContainer->GetSlot( (POSTYPE)pItemInfo->m_wEqPos );
				if ( rSlot.GetCode() == rQuickSlot.GetOrgCode() )
				{
					rQuickSlot.SetOrgSlotIndex(SI_EQUIPMENT );
					rQuickSlot.SetOrgPos( (POSTYPE)pItemInfo->m_wEqPos );
					assert( !pEquipContainer->IsEmpty( (POSTYPE)pItemInfo->m_wEqPos ) );
					rQuickSlot.SetStatus( SLOT_RENDER_STATE_ACTIVATED );
					continue;
				}
			}

			// 인벤토리에서 코드가 같은 아이템의 포지션으로 링크시켜준다.
			BOOL bRet = this->FindFirstItem( rQuickSlot.GetOrgCode(), OrgPos );
			if (bRet)
			{
				rQuickSlot.SetOrgSlotIndex( SI_INVENTORY );
				rQuickSlot.SetOrgPos( OrgPos );
				rQuickSlot.SetStatus( SLOT_RENDER_STATE_ACTIVATED );
			}
			else
			{
				rQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
				rQuickSlot.SetOrgSlot(NULL);
			}
		}
		else
		{
			rQuickSlot.SetStatus( SLOT_RENDER_STATE_ACTIVATED );
		}
	}
}


//----------------------------------------------------------------------------
/** 퀵슬롯에 등록된 아이템들에 대해서 유효성을 갱신. 
*/
VOID								
ItemManager::UpdateValidQuickItem()
{
	SCSlotContainer * pAtContainer		= g_pHero->GetSlotContainer( SI_QUICK);
	assert(pAtContainer);

	if (!g_pHero)
		return;

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); i++)
	{
		if ( pAtContainer->IsEmpty(i) )
		{
			continue;
		}

		QuickSlot & rQuickSlot = (QuickSlot &)pAtContainer->GetSlot(i);

		if ( ST_ITEM != rQuickSlot.GetOrgSlotType() )
		{
			continue;
		}

		SCSlotContainer * pFromContainer = GetContainer(rQuickSlot.GetOrgSlotIndex());
		if (!pFromContainer)
		{
			rQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
			continue;
		}

		if ( pFromContainer->IsEmpty( rQuickSlot.GetOrgPos() ) )
		{
			rQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
			continue;
		}

		SCSlot & rOrgSlot = pFromContainer->GetSlot( rQuickSlot.GetOrgPos() );
		if (rOrgSlot.GetCode() != rQuickSlot.GetOrgCode() )
		{
			rQuickSlot.SetStatus( SLOT_RENDER_STATE_DEACTIVATED );
			continue;
		}

		rQuickSlot.SetStatus( SLOT_RENDER_STATE_ACTIVATED );
	}
}
VOID ItemManager::UseEquipmentItem( SLOTIDX AtIndex, POSTYPE AtPos )
{
	if (!g_pHero) return;
	if( AtIndex != SI_EQUIPMENT ) return;

	SCSlotContainer * pAtContainer	= g_pHero->GetSlotContainer( AtIndex );
	if( !pAtContainer)
		return;
	if( pAtContainer->IsEmpty( AtPos ) ) return;
	
	SCSlotContainer * pToContainer	= g_pHero->GetSlotContainer( SI_INVENTORY );
	POSTYPE emptyPos = 0;
	if( pToContainer->GetEmptyPos( emptyPos ) )
	{
		SendItemMoveMsg( AtIndex, SI_INVENTORY, AtPos, emptyPos );
	}
	else
	{
		// 인벤토리에 내려놓을 공간이 부족합니다.
		return ;
	}
}

void ItemManager::IncItemCount( SLOTIDX AtIndex, POSTYPE AtPos, int Count )
{
    SCSlotContainer * pAtContainer	= g_pHero->GetSlotContainer( AtIndex );
    if ( !pAtContainer )
    {
        return;
    }

    SCSlot& rFromSlot = pAtContainer->GetSlot( AtPos );

    WORD wCount = (WORD)rFromSlot.GetNum();            
    if (wCount+Count > 0 )
    {
        rFromSlot.SetNum((DURATYPE)wCount+Count);
        pAtContainer->UpdateSlot(AtPos, rFromSlot);
    }
    else
    {
        SCItemSlot ItemSlot;
        pAtContainer->DeleteSlot(AtPos, &ItemSlot);
    }
}

VOID ItemManager::UseInventoryItem( SLOTIDX AtIndex, POSTYPE AtPos )
{
	SCSlotContainer * pAtContainer	= g_pHero->GetSlotContainer( AtIndex );
	if ( !pAtContainer )
	{
		return;
	}

	if ( AtPos >= pAtContainer->GetMaxSlotNum() )
	{
		ASSERT(!"Invalid Item Position!!");
		return;
	}

	if ( pAtContainer->IsEmpty( AtPos ) ) 
	{
		return;
	}

	SCSlot & rFromSlot = pAtContainer->GetSlot( AtPos );			//< 1. 아이템을 가져온다.

	// m_byMaterialType 0 : 사용, 장비 불가
	//					1 : 장착 가능 
	//					2 : 사용 가능
	//					3 : 장착, 사용 가능

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( rFromSlot.GetCode() );
	if (!pInfo)
		return;



	DWORD dwItemCode = rFromSlot.GetCode();
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG(uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG);
    if(dwItemCode == 7293 && pLottoNumInput)// 캐쉬 복권 아이템(7293) 글로벌
    {
        pLottoNumInput->SendLottoEntrySyn();
        pLottoNumInput->ShowInterface(FALSE);
    }
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
	switch( pInfo->m_byMaterialType )
	{
    case eITEM_MATERIAL_TYPE_NOT_USE_CAN_DELETE:
	case eITEM_MATERIAL_TYPE_CAN_USE://< 재료
		{
            IncItemCount( AtIndex, AtPos, -1 );
		}
		break;

	default:
		break;
	}

	if (g_pHero)
	{
		g_pHero->OnUseItemWithCoolTime(dwItemCode);
	}

	if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING)
	{
		BASE_ITEMINFO * pItemInfo = (BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(dwItemCode);
		g_CompetitionScene.OnUsePotion(pItemInfo);
	}
#ifdef  _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    //일일퀘스트 아이템이 사용되었는지에 대한 체크 처리
    if(pInfo->IsDailyQeustResetItem())
    {
        if(g_pHero)
        {
            WzVector vPos = g_pHero->GetSkillEffectPos(eSKILLEFFECT_POS_BODY );
            HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A896"), vPos, g_pHero->GetObjectKey());

            CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
            if (pBody && pBody->IsLoop())
            {
                g_pEffectManager->RemoveEffectPtr(hHandle);
            }
        }
    }
#endif
}

void ItemManager::AddItemForSingleMode( DWORD dwObjectKey, DWORD dwID )
{
	Item * pItem = static_cast<Item *>(g_ObjectManager.Add( 
			dwObjectKey, 
			ITEM_OBJECT, 
			dwID ));	// 1은 아이템 

	if( pItem )
	{
		pItem->SetPosition( g_pHero->GetVisiblePos() );
	}
}


BOOL ItemManager::AddFieldItem( const ITEM_RENDER_INFO * pItemInfo )
{
	DWORD		dwObjectKey		= 0;
	CODETYPE	ItemCode		= 0;
	ITEM_RENDER_INFO::eFIELDITEMTYPE eFieldItemType	= 
		ITEM_RENDER_INFO::eFIELDITEM_ITEM;
	WzVector	WzvVec;

	dwObjectKey = pItemInfo->m_dwObjectKey;

	eFieldItemType = 
		(ITEM_RENDER_INFO::eFIELDITEMTYPE)pItemInfo->m_byFieldItemType;


	if ( g_ObjectManager.GetObject(dwObjectKey) )
	{
		assert(!"왜 아이템 키가 겹치징?!!");
		return FALSE;
	}

	SetVector(
		&WzvVec, 
		pItemInfo->m_fPos[0],
		pItemInfo->m_fPos[1],
		pItemInfo->m_fPos[2]);

	Item* item_object = NULL;
    bool is_auto_pick_up = false;

	if (eFieldItemType == ITEM_RENDER_INFO::eFIELDITEM_ITEM)
	{
		item_object = static_cast<Item*>(g_ObjectManager.Add( 
			dwObjectKey, 
			ITEM_OBJECT, 
			pItemInfo->m_ItemStream.Part.wCode));	// 1은 아이템 

		if (item_object != NULL)
        {
			item_object->SetItem((ITEMSTREAMEX&)pItemInfo->m_ItemStream);
        }
		else
        {
			return FALSE;
        }

        if (IsAutoPickupItem(WzvVec) == TRUE)
        {
            if (uiAreaConquestMan::CanPickACItem(item_object, true) == true)
            {
                is_auto_pick_up = true;
            }
        }
	}
	else
	{
		item_object = static_cast<Item*>(g_ObjectManager.Add(dwObjectKey, ITEM_OBJECT, 0 ));
        if (IsAutoPickupMoney(WzvVec))
        {
            is_auto_pick_up = true;
        }
#ifdef _ADD_CHARGE_ITEM_AUTO_GET_HAIM
		else if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING) || 
			     (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
		{
			if ((g_pHero != NULL) &&  
                (g_pHero->GetPCBangMode() != 0))
			{
				if (IsEnablePickupDistance(WzvVec))
				{
					is_auto_pick_up = true;
				}
			}
		}
#endif

		if (item_object)
        {
			item_object->SetMoney((DWORD)pItemInfo->m_Money);
        }
		else
        {
			return FALSE;
        }
	}

    if (is_auto_pick_up == true)
    {
        SendPickUpItemObject(item_object);
    }

	item_object->SetPosition(WzvVec);
	item_object->SetOwner(pItemInfo->m_dwOwnerPlayerKey);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    if (pItemInfo->dicecast_item_type_ != 0)
    {
        uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
        if (partyMan && partyMan->IsPartyMember(pItemInfo->m_dwOwnerPlayerKey))
        {
            item_object->set_is_dice_item(true);

            // 외형을 주사위 아이템 상자로 변환
            APPEARANCEINFO appearInfo;
            ZeroMemory(&appearInfo,sizeof(appearInfo));
            appearInfo.ObjectType = APPEARANCEINFO::ITEM;
            appearInfo.ItemCode = pItemInfo->m_ItemStream.Part.wCode;
            appearInfo.is_dice_item = true;
            item_object->SetDiceAppearance(&appearInfo);
        }
    }
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    item_object->set_expire_time_for_owner(pItemInfo->m_dwRemainedTime);
    bool is_can_get;
    if (item_object->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
    {
        is_can_get = GlobalFunc::CanGetHaim(item_object);
    }
    else
    {
        is_can_get = GlobalFunc::IsMyItem(item_object);
    }

    if ((is_can_get == false)
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        || (pItemInfo->dicecast_item_type_ != 0)
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
        )
    {
        item_object->set_is_can_get_item(false);
        AddOwnerList(dwObjectKey);
    }
	return TRUE;
}
VOID ItemManager::SendTopOfTrialItemMsg( POSTYPE atPos )
{
	MSG_CG_ZONE_CREATE_TOP_OF_TRIAL_SYN	SendPacketAck;

	SendPacketAck.m_ItemPos = atPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
}


VOID ItemManager::SendItemUseMsg(SLOTIDX	atIndex, POSTYPE atPos)
{
	MSG_CG_ITEM_USE_SYN SendPacket;

	SendPacket.m_byCategory = CG_ITEM;
	SendPacket.m_byProtocol = CG_ITEM_USE_SYN;
	SendPacket.m_atIndex	= atIndex;
	SendPacket.m_atPos		= atPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	if ( !GENERALPARAM->IsNet() )
	{
		if(atIndex==SI_INVENTORY)
		{
			MessageforItem(atPos);
			UseInventoryItem(atIndex,atPos);
		}
	}


}
VOID ItemManager::SendItemCombineMsg(SLOTIDX fromIndex, SLOTIDX toIndex, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount)
{
	MSG_CG_ITEM_COMBINE_SYN SendPacket;

	SendPacket.m_byCategory = CG_ITEM;
	SendPacket.m_byProtocol = CG_ITEM_COMBINE_SYN;

	SendPacket.m_fromIndex	= fromIndex;
	SendPacket.m_toIndex	= toIndex;
	SendPacket.m_fromPos	= fromPos;
	SendPacket.m_toPos		= toPos;
    SendPacket.combine_quantity_ = itemCount;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
}



VOID ItemManager::SendItemPortalMsg( POSTYPE CoordPos, POSTYPE PortalPos )
{
	MSG_CG_ITEM_PORTAL_SYN SendPacket;

	SendPacket.m_CoordPos = CoordPos;
	SendPacket.m_PortalPos = PortalPos;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	if ( !GENERALPARAM->IsNet() )
	{
		// 아이템 유즈 ack 패킷 을 날려준다.
		MSG_CG_ITEM_USE_ACK SendPacketAck;
		SendPacketAck.m_atIndex = SI_INVENTORY;
		SendPacketAck.m_atPos = PortalPos;
		// 포탈 패킷이 머였지?
	}
}




//////////////////////////////////////////////////////////////////////////
// 아이템 스텟 초기화 
VOID ItemManager::SendStatInitItem( POSTYPE FromPos, int iNum)
{
	MSG_CG_ITEM_INIT_STAT_SYN SendPacket;
	SendPacket.m_ItemSlotPos = FromPos;
	SendPacket.m_wDecreaseValue = iNum;

	GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

	if ( !GENERALPARAM->IsNet() )
	{		
		MSG_CG_ITEM_INIT_STAT_ACK SendPacketAck;

		SendPacketAck.m_wDecreaseValue = iNum;

		if (g_pApplication)
			g_pApplication->AddVirtualPacket( CI_GAMESERVERIDX, (BYTE*)&SendPacketAck, sizeof(SendPacketAck));
	}
}

VOID ItemManager::SetIndexPosInStatChangePkt(SLOTIDX Idx, POSTYPE Pos)
{
	m_Item_Stat_Change_Use_Syn.m_atIndex = Idx;
	m_Item_Stat_Change_Use_Syn.m_atPos = Pos;
}

VOID ItemManager::SetChangeItemPosInStatChangePkt(POSTYPE ChangeItemPos)
{
	m_Item_Stat_Change_Use_Syn.m_ChangeItemPos = ChangeItemPos;
}

VOID ItemManager::SendStatChangeItem()
{
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_Item_Stat_Change_Use_Syn, sizeof(m_Item_Stat_Change_Use_Syn) );
}

VOID ItemManager::NoticeReasonItemErrorCode(DWORD dwErrCode)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	int		DisplayOption = eOUTPUT_MSG_CHAT;

	switch ((eITEM_RESULT)dwErrCode)
	{
	case RC_ITEM_INVALIDSTATE:
		{
			//해당 행동을 할 수 있는 상태가 아닙니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSTATE, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_CREATE_TOTEMNPC:
		{
			//	토템을 생성할수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5379, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDPOS:
		{ 
			// 해당 위치의 슬롯에 넣을 수 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDPOS, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_EMPTYSLOT:				// 특정위치의 슬롯이 비어있다.
		{
			//	특정 위치의 슬롯이 비어있다.
			// 확인
			g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOTEXISTITEMATFIELD:	// 필드에 아이템이 존재하지 않는다.
		{
			//	필드에 아이템이 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOTEXISTITEMATFIELD, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_NOINFO:					// Code에 해당하는 아이템 정보가 존재하지 않는다.
		{
			//	Code에 해당하는 아이템 정보가 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOINFO, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOSPACEININVENTORY:		// 인벤토리에 빈 공간이 없다.
		{
			//	인벤토리에 빈 공간이 없다
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOSPACEININVENTORY, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDSTATEOFPLAYER:		// 플레이어 상태가 이상하다
		{
			//	플레이어 상태가 이상하다
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSTATEOFPLAYER, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDPOSFORDROPITEM:		// 드랍할 아이템의 위치가 잘못되었다.
		{
			//	드랍할 아이템의 위치가 잘못되었다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDPOSFORDROPITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_DBP_ASYNC:				// DBP로부터 비동기적으로 시리얼 발급 요청 상태
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_DBP_ASYNC, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_COOLTIME_ERROR:			// 쿨타임이 끝나지 않았다.
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_COOLTIME_ERROR, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// Combine
	case RC_ITEM_ITEMCODENOTEQUAL:			// 아이템 코드가 다르다.
		{
			//	아이템 코드가 다르다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ITEMCODENOTEQUAL, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// use
	case RC_ITEM_ISNOTWASTEITEM:			// 소모성 아이템이 아니다.
		{
			//	소모성 아이템이 아니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ISNOTWASTEITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_PORTION_USE_LIMIT:
		{
			//5361	포션 이용 제한으로 인해 더 이상 사용 할 수 없습니다.	
			g_InterfaceManager.GetInterfaceString( 5361, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CAN_USE_FIELD:
		{
			//5360	필드에서만 사용할 수 있습니다.
			g_InterfaceManager.GetInterfaceString( 5360, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;		  

	case RC_ITEM_INVALID_TYPE:
		{
			//5937	해당 기능을 이용할 수 있는 아이템 종류가 아닙니다.
			g_InterfaceManager.GetInterfaceString( 5937, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		//case RC_ITEM_FIELD_IS_NULL:
		//	{			
		//		g_InterfaceManager.GetInterfaceString( 5938, szMessage, INTERFACE_STRING_LENGTH );
		//	}
		//	break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE:
		{
			// 5938	현재 지역에서는 사용이 가능하지 않은 기능입니다.
			g_InterfaceManager.GetInterfaceString( 5938, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_MOVE_TO_THE_AREA:
		{
			// 5939	해당 위치로 이동할 수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5939, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_LEARN_SKILL:
		{
			// 5940	스킬을 습득할 수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5940, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE:
		{
			// 5941	이동시키려는 대상을 찾을 수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5941, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOT_STORE_COORD:
		{
			// 5942	좌표가 저장되어 있지 않습니다.
			g_InterfaceManager.GetInterfaceString( 5942, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_ALREADY_STORE_COORD:
		{
			// 5943	이미 좌표가 존재합니다.
			g_InterfaceManager.GetInterfaceString( 5943, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_STORE_COORD_AT_THIS_AREA:
		{
			// 5944	해당 위치에서는 좌표를 저장할 수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5944, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT:
		{
			// 5945	갯수 부족으로 삭제가 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString( 5945, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// sell
	case RC_ITEM_INVALIDSHOPLISTID:		// dwShopListID가 잘못되었다.
		{
			//	잘못된 삽리스트 ID를 참조하였다
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSHOPLISTID, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_OUTOFSHOPITEMINDEX:		// shop에서 판매하는 아이템 인덱스가 아니다.
		{
			//	상점에서 판매하는 아이템이 아닙니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_OUTOFSHOPITEMINDEX, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INSUFFICIENT_MONEY:
	case RC_ITEM_HAVENOTMONEY:			// 돈이 없다.
		{
			//	하임이 부족합니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_HAVENOTMONEY, szMessage, INTERFACE_STRING_LENGTH );
			g_HeroInput.PlayNotEnoughMoneySound();
		}
		break;

		// buy
	case RC_ITEM_ISNOTEMPTYSLOT:			// 빈 슬롯이 없다 꽉 찼다.
		{
			//	빈 슬롯이 없다 꽉 찼다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ISNOTEMPTYSLOT, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_HAVENOTSPACE:			// 공간이 부족하다.
		{
			//	공간이 부족하다
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_HAVENOTSPACE, szMessage, INTERFACE_STRING_LENGTH );

			InventoryDialog* pDlg = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
			if (pDlg)
			{
				int nCount = pDlg->GetSlotNum();
				int nMax = (g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + pDlg->GetMaxInvenChargeNum()) * pDlg->GetMaxInvenSlotNum();

				if ( nCount == nMax )
				{
					WzColor color = WzColor_RGB(197, 47, 47);
					TCHAR szMessage[INTERFACE_STRING_LENGTH];
					g_InterfaceManager.GetInterfaceString(5661, szMessage, INTERFACE_STRING_LENGTH-1);
					g_InterfaceManager.ShowCenterMessageBox(szMessage, 5000, &color, FALSE );
				}
			}
		}
		break;

	case RC_ITEM_INVALIDVALUE:			// 잘못된 값이 왔다.
		{
			//	잘못된 정보가 왔다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDVALUE, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// pick
#ifdef _JBH_ADD_ITEM_OVER_PLAYING_TIME_MESSAGE
	case RC_ITEM_OVER_PLAYING_TIME:
		{
			// 6047 : 피로도 도달한 계정은 아이템을 주울 수 없습니다.
			g_InterfaceManager.GetInterfaceString(6047, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
#endif
		///.....
		// drop
	case RC_ITEM_ERRORDROPMONEY:
		{
			//	돈을 떨구지 못했다.
#ifdef _DEBUG
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ERRORDROPMONEY, szMessage, INTERFACE_STRING_LENGTH );
#endif
		}
		break;

	case RC_ITEM_ENCHANT_DOWN_FAILED:
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ENCHANT_DOWN_FAILED, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_ENCHANT_HAVENOTMETERIALS:
		{
			//	인첸트시 필요한 재료가 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ENCHANT_HAVENOTMETERIALS, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;


		// rankup
	case RC_ITEM_INVALID_CONDITION:		// 체크 조건에 맞지 않는다.
		{
			//	Rankup 체크 조건에 만족하지 않습니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALID_CONDITION, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNT_RANKUP_ITEM:
		{
			//	Rankup 할수 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_CANNT_RANKUP_ITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// socket
	case RC_ITEM_FULLSOCKET:				// 소켓이 꽉 찼다.
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_FULLSOCKET, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_EXTRACTSOCKET_FAILED:
    case RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO:
		{
			//	소켓제거 실패
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_EXTRACTSOCKET_FAILED, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_UNUSABLE_FUNCTION:
		{
			//5345	사용 할 수 없는 기능입니다.
			g_InterfaceManager.GetInterfaceString( 5345, szMessage, INTERFACE_STRING_LENGTH );				
		}
		break;

	case RC_ITEM_INVALID_VALUE:
		{
			//5346	잘못된 정보 입니다.
			g_InterfaceManager.GetInterfaceString( 5346, szMessage, INTERFACE_STRING_LENGTH );				
		}
		break;
	case RC_ITEM_CANNNOT_DROPSTATE:
		{
			//5347	아이템을 버릴 수 없는 상태입니다.
			g_InterfaceManager.GetInterfaceString( 5347, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_NOTEXIST_ITEM:
		{
			//5348	아이템이 존재하지 않습니다.
			g_InterfaceManager.GetInterfaceString( 5348, szMessage, INTERFACE_STRING_LENGTH );							

		}
		break;
	case RC_ITEM_CANNOT_SELL_ITEM:
		{
			//5349	판매가 가능하지 않은 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5349, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_CANNOT_TRADE_ITEM:
		{
			//5350	거래가 가능하지 않은 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5350, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_CANNOT_DROP_ITEM:
		{
			//5351	버릴 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5351, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;

	case RC_ITEM_CANNOT_EXCHANGE_ITEM:
		{
			//5352	교환 할 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5352, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;

	case RC_ITEM_CANNOT_MOVE_OTHER_SLOT_ITEM:
		{
			//5353	다른 슬롯으로 이동 할 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5353, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_CANNOT_DESTROY_ITEM:
		{
			//5354	파기 할 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5354, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_CANNOT_PICKUP_OBSERVER:
		{
			//5355	투명 상태에서는 아이템을 습득할 수 없습니다.
			g_InterfaceManager.GetInterfaceString( 5355, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL:
		{
			// 현재 레벨에서 이용할 수 없는 기능입니다.
			g_InterfaceManager.GetInterfaceString(eST_NOT_USE_CURRENT_LEVEL , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

		// 내구도, 수리
	case RC_ITEM_DONT_NEED_TO_REPAIR:				//< 수리할 필요 없음
		{
			//5134	수리가 완료된 아이템입니다.
			g_InterfaceManager.GetInterfaceString(5134 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_REPAIR_ITEM:				//< 수리가 불가능한 아이템입니다.
		{
			//5135	수리할 수 있는 아이템이 아닙니다.
			g_InterfaceManager.GetInterfaceString(5135 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_COMPOSE_INVALID_LOCATION:			//< 조합할 수 없는 맵입니다.
		{
			//5136	NPC와 거리가 멀어 조합을 진행할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5136 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_COMPOSE_LIMIT_CLASS:				//< 조합할 수 없는 클래스입니다.
		{
			//5137	조합할 수 없는 클래스 입니다.
			g_InterfaceManager.GetInterfaceString(5137, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE:		//< 카오상태에서는 이용할 수 없는 기능이다.
		{
			//5275	카오 상태에서는 이용이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5275, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_USE_QUEST_ACCEPT_ITEM:		//< 퀘스트 수락 아이템을 사용할 수 없습니다.
		{
			// 5138	조건이 맞지 않아 퀘스트 수락 아이템을 사용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5138, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_ELITE:				//< 엘리트 아이템 랭크업 불가
		{
			//5377	엘리트 아이템은 랭크업이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5377, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_CRYSTALIZE_ELITE:			//< 엘리트 아이템 결정화 불가
		{
			//5376	엘리트 아이템은 결정화가 가능하지 않습니다.								
			g_InterfaceManager.GetInterfaceString(5376, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_UNIQUE:				//< 유니크 아이템 랭크업 불가
		{
			//5374	유니크 아이템은 랭크업이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5374, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_CRYSTALIZE_UNIQUE:			//< 유니크 아이템 결정화 불가
		{
			//5373	유니크 아이템은 결정화가 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5373, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION://< 변신상태에서는 이용할 수 없는 기능이다.
		{
			// 5139	변신 상태에서는 이용이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5139, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_LENGTH_CANNOT_USE_NPC:				//< NPC를 이용할 수 없는 거리.
		{
			//5141	거리가 멀어 이용이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5141, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_DBG_WRONG_PACKET:					//< 잘못된 패킷 수신	
		{
			//5142	정보가 잘못되었습니다.
			g_InterfaceManager.GetInterfaceString(5142, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_ADD_EXTRAINVENTORY:			//< 더이상 유료화 인벤토리를 추가할 수 없다.
		{
			//5143	유료화 인벤토리를 더 이상 추가할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5143, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_DONT_EXIST_TO_REPAIR:
		{
			//85225	수리할 아이템이 없습니다
			g_InterfaceManager.GetInterfaceString(85225, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;


	case RC_ITEM_NOT_MIXTURE:
		{
			//5146	기능을 사용할 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString( 5146 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_NOT_INVENTORY:
		{
			//5876	인벤토리에서만 제거할 수 있습니다.
			g_InterfaceManager.GetInterfaceString(5876, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	case RC_ITEM_INVALID_SOCKET_NUM:	//< 소켓옵션 삭제시 잘못된 소켓넘버 입니다.
		{
			// 5956	소켓 옵션 삭제 시 잘못된 소켓 번호입니다.
			g_InterfaceManager.GetInterfaceString(5956, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOCKET_HAS_NOT_OPTION:	//< 소켓옵션이 없는 아이템의 소켓을 삭제 하려고 합니다.
		{
			//5875	소켓 옵션이 없어 제거할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5875, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOCKET_DIFFERENT_LEVEL:	//< 상,하급룬이 다른 소켓 옵션을 삭제할 수 없습니다.
		{
			//5881	소멸의 룬 등급이 달라 소켓 옵션을 제거할 수 없습니다. 
			g_InterfaceManager.GetInterfaceString(5881, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOKET_NOT_RUNE_ITEM:	//< 사용 아이템이 '소멸의 룬'이 아니다.
		{
			//5882	소멸의 룬 아이템이 아닙니다.
			g_InterfaceManager.GetInterfaceString(5882, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_ISNOT_PARTY_MEMBER:	//< 파티원이 없어서 아이템을 사용 하실 수 없습니다.
		{
			//5955	파티 상태에서만 사용이 가능합니다.
			g_InterfaceManager.GetInterfaceString(5955, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_NOT_UNEQUIP_ALL:					//< 장비를 모두 해제하지 않은 상태이다.
		{
			//5884	아이템이 장착되어 있는 상태에서 사용 불가능 합니다.
			g_InterfaceManager.GetInterfaceString(5884, szMessage ,INTERFACE_STRING_LENGTH);
			DisplayOption |= eOUTPUT_MSG_TOP_SHOW;
		}
		break;
	case RC_ITEM_NOT_MORE_DECREASE:					//< 더 이상 스탯을 감소할 수 없다.
		{
			//5883	캐릭터 기본 능력치에 도달하여 더 이상 사용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5883, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_LESS_FOR_USE:						//< 해당 아이템이 모자라다.
		{
			// 5702	재료가 부족합니다. 
			g_InterfaceManager.GetInterfaceString(5702, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_USE_MULTI_ALL_ITEM:			//< 올스텟 초기화 아이템은 복수로 사용할 수 없다.
		{
			//5888	올 스탯 초기화 아이템은 한번만 사용이 가능합니다. 더 이상 사용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5888, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_MONSTER_SUMMON_FAILED:				//< 몬스터 소환 아이템을 사용은 성공했지만 확률로 인해 실패
		{
			//5726	몬스터 소환 아이템이 정상적으로 작동하였지만 몬스터를 소환하는 데에는 실패했습니다.
			g_InterfaceManager.GetInterfaceString(5726, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_WAREHOUSE_NEED_PASSWORD:
		{
			//	창고가 잠금 상태입니다.
			g_InterfaceManager.GetInterfaceString(5571, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_INVENTORY_NEED_PASSWORD:			//< 비번 필요합니다.
		{
			//  인벤토리와 보조 창고가 잠금 상태입니다.
			g_InterfaceManager.GetInterfaceString(5572, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;
	case RC_ITEM_INVENTORY_INVALID_PASSWORD:			//< 잘못된 비번입니다.
	case RC_ITEM_INVENTORY_ALREADY_TRANSACTION:				//< 다른 비번관련 처리중입니다.
	case RC_ITEM_INVENTORY_PWD_SETTING_FLOW_VIOLATION:		//< 비번 설정관련 플로우를 위반했습니다.


	case RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM:
		{
			// 창고로 이동할 수 없는 아이템입니다.
			g_InterfaceManager.GetInterfaceString(5891, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;



	case RC_ITEM_CANNOT_RANKDOWN_ELITE:
		{
			// 엘리트 아이템은 랭크다운 할수 없습니다 6862
			g_InterfaceManager.GetInterfaceString( 6862, szMessage );
		}
		break;

	case RC_ITEM_CANNOT_RANKDOWN_UNIQUE:
		{
			// 유니크 아이템은 랭크다운 할수 없습니다 6863
			g_InterfaceManager.GetInterfaceString( 6863, szMessage );
		}
		break;

	case RC_ITEM_NOT_RANK_DOWN_TO_E_RANK_ITEM:
		{
			// E랭크 아이템은 랭크다운 할수 없습니다 5892
			g_InterfaceManager.GetInterfaceString( 5892, szMessage );
		}
		break;


    case RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND:
	case RC_ITEM_CANNOT_EXTEND_DATE:
		{
			// 5899	기간 연장이 가능한 아이템이 아닙니다.
			g_InterfaceManager.GetInterfaceString( 5899, szMessage );
		}
		break;


	case RC_ITEM_DONOT_HAVE_OWNERSHIP:
		{
			g_InterfaceManager.GetInterfaceString( 5217, szMessage );
		}
		break;


	case RC_ITEM_CANNOT_REPAIR_MAX_DURA_TWO_OVER:
		{
			//< 최대 내구도 2이하에서만 사용 가능
			g_InterfaceManager.GetInterfaceString( 6053, szMessage );
		}
		break;

	case RC_ITEM_NOT_LIMITED_ITEM:
		{
			//< 리미티드 아이템이 아님
			g_InterfaceManager.GetInterfaceString( 6054, szMessage );
		}
		break;

	case RC_ITEM_OVER_REPAIR_COUNT:
		{
			//	6865	가능한 수리 횟수를 넘어 더 이상 수리가 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString( 6865, szMessage );
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_LIMITED:
		{
			//	6051	리미티드 아이템은 랭크업이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString( 6051, szMessage );
		}
		break;


	case RC_ITEM_CANNOT_EXTEND_CASH_SLOT:
		{
			//5312	이미 사용하여 더 이상 확장할 수 없습니다
			g_InterfaceManager.GetInterfaceString(5312, szMessage);
		}
		break;
	case RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM:
		{
			//5313	유료화 아이템에 대해 이 기능을 이용 할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5313, szMessage);
		}
		break;

	case RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION:		//< [이벤트] 이미 다른 명령이 수행중입니다. 잠시후 다시 시도하세요.
		{
			//이미 다른 명령이 수행중입니다. 잠시후 다시 시도하세요.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_UNKNOWNERROR, szMessage);
		}
		break;
	case RC_ITEM_EVENT_CANNOT_MOVE_TO_INVENTORY:			//< [이벤트] Can't Move EventInven->Inven
		{
			//EventInven->Inven
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_UNKNOWNERROR, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_REGISTER_AC_ITEM:
		{
			//5124	이 AC 아이템은 더 이상 사용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5124 , szMessage);
		}
		break;


	case RC_ITEM_CANT_PICKABLE_BY_LENGTH:
		{
			//5315	아이템을 습득할 수 없는 거리입니다.
			g_InterfaceManager.GetInterfaceString(5315, szMessage);
		}
		break;

	case RC_ITEM_CAN_NOT_EQUIP:
		{
			//5091  아이템을 장착할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5091, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE:  // 길드 등급이 낮아 사용할 수 없는 슬롯
		{
			//7189	길드 등급이 낮아 사용할 수 없는 슬롯입니다.
			g_InterfaceManager.GetInterfaceString(7189, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_CASHITEM: //캐쉬템이 없어서 사용할 수 없는 슬롯
		{
			//7190	캐쉬 아이템이 있어야 사용할수 있는 슬롯입니다.
			g_InterfaceManager.GetInterfaceString(7190, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_NOEN_OUT_RIGHT: //길드 출고를 할 권한이 없다.
		{
			//7191	길드 창고 출고 권한이 없습니다.
			g_InterfaceManager.GetInterfaceString(7191, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_OPEN_USENOT_CASHITEM:
		{
			//7223	길드 창고가 열려 있을 때 길드 창고 확장자재를 이용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(7223, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM:
		{
			//7229	소속된 길드가 없어서 길드 창고 확장 아이템을 사용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(7229, szMessage);
		}
		break;
	case RC_ITEM_GUILDWAREHOUSE_NONE_USE_RIGHT:
		{
			//7230	길드 창고 확장자재 캐쉬템을 사용할 권한이 없습니다.
			g_InterfaceManager.GetInterfaceString(7230, szMessage);
		}
		break;

#ifdef	__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
	case RC_ITEM_RANKUP_FAILED_AND_DESTROY:
		{
			//4554 랭크업에 실패했습니다!
			g_InterfaceManager.GetInterfaceString( 4554, szMessage );
		}
		break;
#endif


	case RC_ITEM_ETHER_DEVICE_USE_ONLY_TO_WEAPON:
		{	
			//6122	에테르 방출장치는 무기에만 장착가능합니다.
			g_InterfaceManager.GetInterfaceString(6122, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_EQUIP_ETHER_DEVICE:
		{
			//6123	에테르 방출장치가 이미 장착되어 있습니다.	
			g_InterfaceManager.GetInterfaceString(6123, szMessage);
		}
		break;

	case RC_ITEM_NOT_EQUIP_ETHER_DEVICE:
		{
			//6124	에테르 방출장치가 장착 되어 있지않습니다.
			g_InterfaceManager.GetInterfaceString(6124, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_CHARGE_ETHER_BULLET:
		{
			//6125	에테르 탄환이 이미 장전되어 있습니다.
			g_InterfaceManager.GetInterfaceString(6125, szMessage);
		}
		break;


	case RC_ITEM_NOT_SUMMONED_PET:
		{
			//6397	소환된 펫이 없음
			g_InterfaceManager.GetInterfaceString(6397, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_WINDOW_ALREADY_OENDEDSTATE:
		{
			//5385 창이 이미 열려있습니다.
			g_InterfaceManager.GetInterfaceString(5385, szMessage);
		}
		break;

	case RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET:
		{
			//6119 에테르 탄환을 활성화 할 수 없다
			g_InterfaceManager.GetInterfaceString(6119, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET:
		{
			//6120 에테르 탄환을 비활성화 할 수 없다
			g_InterfaceManager.GetInterfaceString(6120, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANT_EQUIP_WEAPON:
		{
			//5754: 무기를장착할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5754, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_SKILL_ACTION:
		{
			//5253 : 스킬사용이 가능하지않습니다.
			g_InterfaceManager.GetInterfaceString(5253, szMessage, INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CAN_USE_ONLY_CHAO:
		{
			//5280: 카오 유저만 사용할 수 있다.(프리카오 포함)
			g_InterfaceManager.GetInterfaceString(5280, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;


	case RC_ITEM_CANNOT_USE_PLAYER_LEVEL:
		{
			//< 사용 가능한 레벨이 아님
			g_InterfaceManager.GetInterfaceString( 5297, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_ALREADY_FIREUP_ITEM_SAME_TYPE:			
		{
			//< 이미 같은 타입의 아이템이 활성화 되어있다.
			g_InterfaceManager.GetInterfaceString( 6158, szMessage,INTERFACE_STRING_LENGTH);	
		}
		break;

    
        
    case RC_ITEM_ALREADY_EXTRACTED_TO_ETHERIA:           
        {
            //6090: 이미 에테리아가 추출된 장비입니다.
            g_InterfaceManager.GetInterfaceString( 6090, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
      
    case RC_ITEM_CANT_EXTRACT_TO_ETHERIA:                
        {
             //6091: 이 아이템은 에테리아 추출이 불가능합니다.
            g_InterfaceManager.GetInterfaceString( 6091, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
       
    case RC_ITEM_CANT_COMBINED_WITH_ITEM:               
        {
             //< 해당 아이템은 결합할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(6094, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
        
    case RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM:      
        {
            //6095: 같은 종류만 결합할 수 있습니다.
            g_InterfaceManager.GetInterfaceString(6095, szMessage, INTERFACE_STRING_LENGTH);
         }
        break;

    case RC_ITEM_UNAVAILABLE_TARGET:      
        {
            // 3433 지정된 대상에 아이템을 사용할수 없습니다.
            g_InterfaceManager.GetInterfaceString(3433, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_NOT_FRIEND:      
        {
            // 3434	해당 아이템은 방어 진영에서만 사용할 수 있습니다.
            g_InterfaceManager.GetInterfaceString(3434, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_ITEM_HEIM_LOTTO_TICKET_MAX_REACHED:
        {
            //5889: 사용 가능한 아이템 개수를 초과하였습니다.
            g_InterfaceManager.GetInterfaceString(5889,szMessage);
        }
        break;

    case RC_ITEM_NOT_UNIDENTIFIED_SOCKET:// 미확인 소켓이 아니다
        {
            //6486: 해당 아이템은 소켓 수량이 이미 확인되었거나, 소켓 아이템이 아닙니다
            g_InterfaceManager.GetInterfaceString(5889,szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_ITEM_IS_SHORT_HAVE_ITEM: // 아이템 타입 상점에서 구입하기 위한 소지 아이템이 부족하다.
        {
            TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

            //47638	%s(이)가 부족하여 해당 아이템을 구입할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(47638, temp_string, INTERFACE_STRING_LENGTH);

            ShopDialog* shop_dialog = static_cast<ShopDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP));
            if (shop_dialog)
            {
                CODETYPE item_code = shop_dialog->currency_item_code();
                g_InterfaceManager.GetItemName(item_code, item_name);
            }
            Sprintf(szMessage, temp_string, item_name);
        }
        break;
    case RC_ITEM_IS_SHORT_HAVE_ACCUMULATEPOINT: // 적립 포인트 상점에서 구입하기 위한 소지 포인트가 부족하다.
        {   // 47639 접속포인트가 부족하여 해당 아이템을 구입할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(47639, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

#ifdef _NA_20100307_BETAKEY_SYSTEM
    case RC_ITEM_CANNOT_EQUIPMENT_BETAKEY:       //해당 배타키를 가지지 못한 유저는 장착할수 없는 아이템이다.
        {
            // 70542 조건이 맞지 않아 해당 장비를 착용할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(70542, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_CANNOT_USE_BETAKEY:             //해당 배타키를 가지지 못한 유저는 사용할수 없는 아이템이다.
        {
            // 70543 조건이 맞지 않아 해당 장비를 사용할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(70543, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#endif//_NA_20100307_BETAKEY_SYSTEM
        
        // _NA_003027_20111013_HONOR_SYSTEM
    case RC_ITEM_BUY_ERROR_OF_FAME:
        {
            // 6516 해당 아이템을 구입하기  위한 명성 등급이 낮습니다.
            g_InterfaceManager.GetInterfaceString(6516, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_OF_REPUTE:
        {
            // 6517 해당 아이템을 구입하기 위한 평판 등급이 낮습니다.
            g_InterfaceManager.GetInterfaceString(6517, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    case RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT:
        {
            // 70701 더 이상 해당 아이템을 지갑에 넣을 수 없습니다.
            g_InterfaceManager.GetInterfaceString(70701, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_FAILED_TO_BUY_BY_POINT:
        {
            // ...
            ShopDialog* shop_dialog = static_cast<ShopDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP));
            if (shop_dialog != NULL)
            {
                DWORD currency_code = shop_dialog->currency_item_code();
                CurrencyInfoParser::CurrencyInfo* currency_info = 
                    CurrencyInfoParser::Instance()->GetCurrencyInfo(currency_code);
                if (currency_info != NULL)
                {
                    TCHAR currency_name[INTERFACE_STRING_LENGTH] = {'\0', };
                    g_InterfaceManager.GetInterfaceString(currency_info->currency_name_code, currency_name);
                    // 47638 %s(이)가 부족하여 해당 아이템을 구입할 수 없습니다.
                    g_InterfaceManager.GetInterfaceStringFormat(
                        szMessage, INTERFACE_STRING_LENGTH, 47638, currency_name);
                }
            }
        }
        break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
    case RC_ITEM_DEAD_OWNER:
        {
            // 5691 해당 아이템의 소유권을 가진 캐릭터는 현재 아이템을 획득할 수 없는 상태입니다.
            g_InterfaceManager.GetInterfaceString(5691, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_GUILD_LEVEL:
        {
            // 6871 길드레벨 조건이 맞지 않음
            g_InterfaceManager.GetInterfaceString(6871, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_GUILD_EXP:
        {
            // 6872 길드공헌도 조건이 맞지 않음
            g_InterfaceManager.GetInterfaceString(6872, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
    case RC_ITEM_CANNOT_USE_ALREADY_APPLY_HIGHRANK_BUFF:
        {
            // 6880 같은 종류의 더 강한 효과를 가진 물약을 사용 중입니다. 해당 물약의 효과가 끝난 후 사용하시기 바랍니다.
            g_InterfaceManager.GetInterfaceString(6880, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#endif // _NA_006689_20130503_CANNOTUSE_BUFFITEM
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    case RC_ITEM_CANNOT_USE:
        {
             g_InterfaceManager.GetInterfaceString(5267, szMessage, INTERFACE_STRING_LENGTH);
        } 
        break;
#endif
	case RC_ITEM_FAILED:
	case RC_ITEM_UNKNOWNERROR:			// 알수 없는 에러, 클라이언트에서의 잘못된 데이터 혹은 해킹에 의해 데이터 크랙
	default:
		{
			//	알수 없는 에러!
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_UNKNOWNERROR, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	}

	GlobalFunc::OutputMsg_Notice( DisplayOption, szMessage );
}


VOID ItemManager::AddUpdateItemSlot( SCSlot * pItemSlot )
{
	SCItemSlot * pRefItemSlot = (SCItemSlot *)pItemSlot;

	if ( !pRefItemSlot )
		return;

	if (IsChargeItem(pRefItemSlot) == false)
	{
		return;
	}

    BYTE charge_subtype = pRefItemSlot->GetItemRuleInfo().charge_sub_type;
	switch (charge_subtype)
	{
    case eCHARGE_SUB_RIDER_WITH_PART:
	case eCHARGE_SUB_FIXED_AMOUNT:
	case eCHARGE_SUB_FIXED_AMOUNT_EX:
	case eCHARGE_SUB_FIXED_QUANTITY:
    case eCHARGE_SUB_COORDINATE_EX:
		// 이것만 통과. 
		break;

		// 다른것들은 추가할 필요가없다.
	default:
		return;
		break;
	}

	UPDATE_ITEMSLOT_ITR itr = m_UpdateItemSlotMap.find((VOID *)pItemSlot);

	if ( itr == m_UpdateItemSlotMap.end() )
	{
		m_UpdateItemSlotMap.insert( UPDATE_ITEMSLOT_PAIR((VOID *)pItemSlot, pItemSlot) );
	}
	else
	{
		assert(0);
	}
}

VOID ItemManager::RemoveUpdateItemSlot( SCSlot * pItemSlot )
{
	UPDATE_ITEMSLOT_ITR itr = m_UpdateItemSlotMap.find((VOID *)pItemSlot);

	if ( itr != m_UpdateItemSlotMap.end() )
	{
		m_UpdateItemSlotMap.erase(itr);
	}
}

//----------------------------------------------------------------------------
/**
인벤토리, 템프인벤토리, 이벤트 인벤토리, 창고에 있는 아이템 정보를 업데이트 한다.
*/
VOID ItemManager::UpdateItemSlot()
{
	UPDATE_ITEMSLOT_ITR itr = m_UpdateItemSlotMap.begin();

	for (; itr != m_UpdateItemSlotMap.end(); ++itr)
	{
		SCItemSlot * pItemSlot = (SCItemSlot *)itr->second;

		pItemSlot->Update((BOOL)g_pHero->GetPCBangMode());
	}
}

VOID ItemManager::ClearUpdateItemSlot()
{
	m_UpdateItemSlotMap.clear();
}

bool ItemManager::CanPeriodExtend(SCItemSlot* item_slot)
{
    if (item_slot == NULL)
    {
        return false;
    }

    BYTE charge_subtype = item_slot->GetItemRuleInfo().charge_sub_type; 
    if (charge_subtype != eCHARGE_SUB_FIXED_AMOUNT_WAIT &&
        charge_subtype != eCHARGE_SUB_FATE &&
        charge_subtype != eCHARGE_SUB_RIDER_WITH_PART)
    {
        return false;
    }

    return true;
}

bool ItemManager::IsChargeItem(SCItemSlot* item_slot)
{
    if (item_slot == NULL)
    {
        return false;
    }

    if ((item_slot->GetItemRuleInfo().IsEnableTimer() == false) && 
        (item_slot->GetItemRuleInfo().IsEnableCoord() == false))
    {
        return false;
    }

    return true;
}

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
bool ItemManager::CheckDrawDiceItem()
{
    //오브젝트를 순환하면서 dicecast 속성이 있는 아이템이 있는지 조사한다.
    Item            *item_ptr;
    Object          *object;
    SCObject        sc_object;
    Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr object_itr_end = g_ObjectManager.GetEnd();
    for( ; object_itr != object_itr_end; ++object_itr)
    {
        object = static_cast<Object *>(object_itr->second);
        if (0 == object)
        {
            continue;
        }
        sc_object = (object_itr->second)->GetSCObject();

        // 아이템인가?
        if (sc_object.GetObjectType() == ITEM_OBJECT)
        {
            item_ptr = static_cast<Item *>(object);

            if (item_ptr->is_dice_item() == true)
            {
                return true;
            }
        }
    }
    return false;
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

void ItemManager::UpdateOwnerList()
{
    //리스트 갱신
    std::vector<DWORD>::iterator list_iter = update_owner_list_.begin();
    std::vector<DWORD>::iterator list_iter_end = update_owner_list_.end();
    while (list_iter != list_iter_end)
    {
        DWORD item_object_key = (*list_iter);
        Item* item_temp = static_cast<Item*>(g_ObjectManager.GetObject(item_object_key));

        if (item_temp == NULL)
        {
            list_iter = update_owner_list_.erase(list_iter);
            list_iter_end = update_owner_list_.end();
            continue;
        }

        bool Is_changed = item_temp->UpdateOwner(); //시간 경과확인 후 시간 종료면 삭제
        if (Is_changed == true)
        {
            list_iter = update_owner_list_.erase(list_iter);
            list_iter_end = update_owner_list_.end();
            item_temp->set_is_can_get_item(true);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            item_temp->set_is_dice_item(false);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
            continue;
        }

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        if (item_temp->is_dice_item() == false)
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
        {
            bool is_can_get = false;
            if (item_temp->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
            {
                is_can_get = GlobalFunc::CanGetHaim(item_temp);
            }
            else
            {
                is_can_get = GlobalFunc::IsMyItem(item_temp);  //먹을 수 있는 템인지 재확인
            }

            item_temp->set_is_can_get_item(is_can_get);
            if (is_can_get == true)
            {
                list_iter = update_owner_list_.erase(list_iter);
                list_iter_end = update_owner_list_.end();
                continue;
            }
        }

        ++list_iter;
    }
}

void ItemManager::AddOwnerList(DWORD item_key)
{
    update_owner_list_.push_back(item_key);
}

void ItemManager::ClearOwnerList()
{
    update_owner_list_.clear();
}

void ItemManager::UpdateCanGetACItem(SLOTCODE item_code)
{   
    //오브젝트를 순환하면서 해당 AC아이템의 *표시를 해제한다
    Item            *item_ptr;
    Object          *object;
    SCObject        sc_object;
    Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
    for( object_itr = g_ObjectManager.GetBegin() ; object_itr != g_ObjectManager.GetEnd() ; ++object_itr )
    {
        object = static_cast<Object *>(object_itr->second);
        if (0 == object)
        {
            return;
        }
        sc_object = (object_itr->second)->GetSCObject();
      
        // 아이템인가?
        if( sc_object.GetObjectType() == ITEM_OBJECT )
        {
           item_ptr = static_cast<Item *>(object);
           if (item_ptr->GetItemSlot().GetItemInfo()->m_Code == item_code)
           {
                item_ptr->set_is_can_get_item(false);
           }
        }
    }
}   

void ItemManager::UpdateMyItem()
{   
    //오브젝트를 순환하면서 아이템의 *표시를 갱신한다
    Item            *item_ptr;
    Object          *object;
    SCObject        sc_object;
    Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
    for( object_itr = g_ObjectManager.GetBegin() ; object_itr != g_ObjectManager.GetEnd() ; ++object_itr )
    {
        object = static_cast<Object *>(object_itr->second);
        if (0 == object)
        {
            return;
        }
        sc_object = (object_itr->second)->GetSCObject();

        // 아이템인가?
        if( sc_object.GetObjectType() == ITEM_OBJECT )
        {
            item_ptr = static_cast<Item *>(object);

            bool is_can_get;
            if (item_ptr->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
            {
                is_can_get = GlobalFunc::CanGetHaim(item_ptr);
            }
            else
            {
                is_can_get = GlobalFunc::IsMyItem(item_ptr);
            }

            item_ptr->set_is_can_get_item(is_can_get);

            if ((is_can_get == false)
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
                || (item_ptr->is_dice_item() == true)
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
                ) 
            {
                AddOwnerList(item_ptr->GetObjectKey());
            }
        }
    }
}  

//----------------------------------------------------------------------------
/**
*/
const SCSkillSlot *
ItemManager::GetSkillSlotByCode(CODETYPE code)
{
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    if (ui_skill_man == NULL)
    {
        return NULL;
    }

    return ui_skill_man->GetSkillSlotByCode(code);	
}

BOOL ItemManager::IsItemDeleteAvalibeState(POSTYPE pos)
{
	SCSlotContainer* pInventory	= g_pHero->GetSlotContainer(SI_INVENTORY);
	if (pInventory)
	{
		SCItemSlot& rItemSlot = (SCItemSlot&)pInventory->GetSlot(pos);

		BOOL bAvailbeState = TRUE;

		if (rItemSlot.GetLaxLock()->IsLock()) 
		{
			bAvailbeState = FALSE;
			MouseHandler::Instance()->UnLockWasteDeleteItem();
		}

		if (rItemSlot.IsLocked())
		{
			bAvailbeState = FALSE;
			NoticeReasonItemErrorCode(RC_ITEM_INVALIDPOS);
		}

		if (bAvailbeState == FALSE)
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			//5311	블록된 아이템이여서 삭제할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5311, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			return FALSE;
		}
	}

	return TRUE;
}


bool ItemManager::SetItemLock(SLOTIDX slotIndex, POSTYPE atPos, bool bLock)
{
	if( slotIndex != SI_INVENTORY )
	{
		assert(!"인벤토리에 있는 아이템만 Lock이 가능합니다.");
		return false;
	}

	SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );;
	if( pContainer )
	{
		SCItemSlot & pTargetItem = ( SCItemSlot&) pContainer->GetSlot(atPos);
		pTargetItem.SetLock(bLock);
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
void ItemManager::MessageforItem(POSTYPE ItemPos) //포인트업 및 길드 창고 슬롯 추가 공지
{
	SCSlotContainer * pFromContainer = 
		ItemManager::Instance()->GetContainer( SI_INVENTORY );
	SCItemSlot& SlotTemp = (SCItemSlot&)pFromContainer->GetSlot(ItemPos);
	const BASE_ITEMINFO* pItemInfo = SlotTemp.GetItemInfo();

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if(eITEMWASTE_STAT_POINT_UP == pItemInfo->m_byWasteType)
	{
		g_InterfaceManager.GetInterfaceString(5070, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW , szMessage );
	}
	else if(eITEMWASTE_GUILDWAREHOUSE_SLOTCASH == pItemInfo->m_byWasteType)
	{
		//6172	정상적으로 길드 창고의 슬롯이 추가 되었습니다.
		g_InterfaceManager.GetInterfaceString(6172, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW , szMessage );
	}
    else if (eITEMTYPE_EXTEND_CHARACTER_SLOT == pItemInfo->m_wType)
    {
        //5992 캐릭터 슬롯이 정상적으로 확장 되었습니다. 캐릭터 선택화면에서 확인이 가능합니다.
        g_InterfaceManager.GetInterfaceString(5992, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW , szMessage );
    }
}
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
/**  에테리아 추출가능한지 체크(엘리트,부서진아이템,귀속,유니크,럭키)불가
*/
bool ItemManager::IsInvalidEtheriaExtract(POSTYPE slot_pos, POSTYPE etherealizer_pos)
{
    SCSlotContainer* slot_container = GetContainer(SI_INVENTORY); 

    if (slot_container  == NULL)
    {
        return false;
    }

    SCItemSlot& etherealizer_item = (SCItemSlot&)slot_container->GetSlot(etherealizer_pos);
    if ((etherealizer_item.IsContainValidItem() == FALSE)||
        (etherealizer_item.GetItemInfo()->m_wType !=  eITEMTYPE_ETHEREALIZER))
    {
        return false;
    }

    SCItemSlot& slot_item = (SCItemSlot&)slot_container->GetSlot(slot_pos);
    if (slot_item.IsContainValidItem() == FALSE)
    {
        return false;
    }
    
    RC::eITEM_RESULT result;

    if (slot_item.CanExtractToEtheria(&result) == false)
    {
        NoticeReasonItemErrorCode(result);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
/**  에테리아 캐스팅패킷(C2S)
*/
void ItemManager::SendEtheriaCastingStart(POSTYPE slot_pos, POSTYPE etherealizer_pos)
{
    if (g_pHero == NULL)
    {
        return;
    }
    //라이딩상태에서는 액션불가
    if (g_pHero->IsRiding())
    {
        //6093:메시지 출력 라이딩상태에서는 추출이 불가능합니다.
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW ,
                                     g_InterfaceManager.GetInterfaceString(6093));
        return;
    }

    if (g_pHero->GetCurState() == STATE::SUMMON_RIDER)
    {
        return; 
    }

    
    //락을 건다
    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
    if (inventory_manager != NULL)
    {
        inventory_manager->LockInventorySlot(slot_pos);
        inventory_manager->LockInventorySlot(etherealizer_pos);
    }

    // 에테리아 캐스팅시작패킷
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_CastingStart; 
    send_packet.header.args.extract.slot_pos = slot_pos;
    send_packet.header.args.extract.etherealizer_pos = etherealizer_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}
//--------------------------------------------------------------------------------------------------
/**  에테리아 추출패킷(C2S)
*/
void ItemManager::SendEtheriaExtractPacket(POSTYPE slot_pos, POSTYPE etherealizer_pos)
{
    // 에테리아 추출 패킷 내용채움(요청타입,추출아이템위치)
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_Extract; 
    send_packet.header.args.extract.slot_pos = slot_pos;
    send_packet.header.args.extract.etherealizer_pos = etherealizer_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    
}
//--------------------------------------------------------------------------------------------------
/**  에테리아 조합패킷(C2S)
*/
void ItemManager::SendEtheriaCombinationPacket(POSTYPE to_pos, POSTYPE from_pos)
{
    // 에테리아 조합 패킷 내용채움(요청타입,추출아이템위치)
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_Combine; 
    send_packet.header.args.combine.from_etheria = from_pos;
    send_packet.header.args.combine.to_shell = to_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    
}

//--------------------------------------------------------------------------------------------------
/**  소켓 확인 가능한지 체크 소켓 아이템이 아니면 불가
*/
bool ItemManager::IsInvalidSocketIdentify(POSTYPE slot_pos, POSTYPE socket_item_pos)
{
    SCSlotContainer* slot_container = GetContainer(SI_INVENTORY); 

    if (slot_container  == NULL)
    {
        return false;
    }

    SCItemSlot& slot_item = (SCItemSlot&)slot_container->GetSlot(slot_pos);
    if (slot_item.IsContainValidItem() == FALSE)
    {
        return false;
    }

//    RC::eITEM_RESULT result;

    int socket_number = -1;
    switch (slot_item.GetSocketIdentifyType())
    {
        case eSOCKET_UNIDENTIFIED_ITEM://미확인
        case eSOCKET_IDENTIFIED_ITEM://소켓갯수확인됨
        {
            socket_number = 0;
        }
        break;
        case eSOCKET_IDENTIFY_NOT_PROCESS: // 소켓 아님
        case eSOCKET_FAIL_IDENTIFY_PROCESS:// 소켓 0개인 아이템
        default:
            socket_number = -1;
            break;
    }

    if (socket_number < -1)
    {
        NoticeReasonItemErrorCode(RC::RC_ITEM_NOT_MIXTURE);
    }
    
    return true;
}

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
// 아이템 기간연장


namespace local_extend_item_data
{
    POSTYPE local_source_position, local_target_position;
    void _OnComfirmExtendItemData( bool yes )
    {
        if( yes )
        {
            MSG_CG_ITEM_USE_EXTEND_DATE_SYN msg;
            msg.extend_item_position_ = local_source_position;
            msg.target_item_position_ = local_target_position;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));
        }
    }
};

void ItemManager::SendExtendItemDate(POSTYPE source_pos, POSTYPE target_pos )
{
    SCSlotContainer* slot_container = GetContainer(SI_INVENTORY); 
    if (slot_container  == NULL)
    {
        return;
    }

    SCItemSlot& target_item = (SCItemSlot&)slot_container->GetSlot(target_pos);
    SCItemSlot& source_item = (SCItemSlot&)slot_container->GetSlot(source_pos);


    // target itemname
    TCHAR target_name[MAX_ITEMNAME_LENGTH];
    g_InterfaceManager.GetItemName( target_item.GetItemCode(), target_name, MAX_ITEMNAME_LENGTH );


    // extend date 
    TCHAR extend_data[1024];
    wsprintf( extend_data, "%d", source_item.GetItemInfo()->m_MaterialCode );



    // [%s] [%s] use ok?
    TCHAR message[INTERFACE_STRING_LENGTH];
    g_InterfaceManager.GetInterfaceString( 2567, message, INTERFACE_STRING_LENGTH);

    TCHAR result[INTERFACE_STRING_LENGTH];
    wsprintf( result, message, target_name, extend_data );
    //Snprintf( result, INTERFACE_STRING_LENGTH-1, target_name, message );

    
    local_extend_item_data::local_source_position = source_pos;
    local_extend_item_data::local_target_position = target_pos;

    uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
    pSystemMan->Sys_Confirm(result, local_extend_item_data::_OnComfirmExtendItemData);
}
#endif//_NA_0_20111012_ITEM_DATE_EXTEND


#ifdef _NA_003740_20111122_RANDOM_ITEM

namespace local_randomize_item
{
    POSTYPE local_source_position, local_target_position;
    void _OnConfirm( bool yes )
    {
        if( yes )
        {
            MSG_CG_ITEM_RANDOMIZE_SYN msg;
            msg.request_item_pos = local_source_position;
            msg.target_item_pos = local_target_position;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));
        }
    }

};

void ItemManager::SendRandomizeItem(POSTYPE source_pos, POSTYPE target_pos )
{
    RandomItemParser* parser = RandomItemParser::Instance();

    SCItemSlotContainer* inventory = (SCItemSlotContainer*)g_pHero->GetSlotContainer( SI_INVENTORY );

    SCItemSlot* request_item;
    SCItemSlot* target_item;
    if( parser->EnableItemRandomize( *inventory, source_pos, target_pos, request_item, target_item ) )
    {

        SCItemSlot& target_item = (SCItemSlot&)inventory->GetSlot(target_pos);

        // target itemname
        TCHAR target_name[MAX_ITEMNAME_LENGTH];
        g_InterfaceManager.GetItemName( target_item.GetItemCode(), target_name, MAX_ITEMNAME_LENGTH );

        TCHAR message[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString( 6322, message, INTERFACE_STRING_LENGTH); //[%s]아이템에 시간을 되돌려 옵션의 효과를 다시 부여하시겠습니까? \n(현재 옵션은 모두 사라지며 새로운 옵션이 다시 부여 됩니다.)

        TCHAR buffer[INTERFACE_STRING_LENGTH];
        wsprintf(buffer, message, target_name );

        local_randomize_item::local_source_position = source_pos;
        local_randomize_item::local_target_position = target_pos;

        uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        pSystemMan->Sys_Confirm(buffer, local_randomize_item::_OnConfirm );
    }
    else
    {
        // 5146 기능을 사용 할 수 없는 아이템입니다.
        TCHAR buffer[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(5146, buffer, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_SysMsg(buffer);
    }

}
#endif

