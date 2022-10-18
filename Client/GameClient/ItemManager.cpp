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
#define MODFLOAT(ratio) ((ratio)<0 ? (-0.5f):(0.5f)) //(Ŭ��&������ ����,����Ʈ ��ġ����ȭ(�ݿø�����))
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

// eATTR_AUTO_ITEM_PICK_UP �� ���� 1~99 �̸� �����ݱ� [11/10/2010 iforall]
// eATTR_AUTO_ITEM_PICK_UP �� ���� 100~ �̸� �������ݱ�
// �������� �����ݱ�θ� ������ 100�̻��� ���� ��� �������ݱ�� �����Ѵ�.
// �������ֱ⸦ 100�̻��� ������ ������ ������ 
// �����ݱ�ɼ�(��:1) �� ������/���� ������ ������ ��ø�Ǹ� 1�̻��� ���� �� �� �־�,
// �̸� �к��ϱ� ���Ͽ� �������ݱ�ɼ� ���� 100���� ����.
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
            //! ������ ���������� ���� �Ǹ�
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
����ִ� ������ġ�� �˾Ƴ��� �Լ�.

@param	[in] AtSlotIdx
�ش� �����̳� �ε���
[out] POSTYPE & rPosOut
����ִ� ������ġ
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
�ش� �������� ��ġ�Ҽ� �ִ� ���� ��ġ�� �Ѱ��ִ� �Լ�

@param 
[in]	rSlot : �ش� ������ ����
[out]	rEqPosOut : ��Ţ ������ ��ġ�� ���´�. 
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

    //������ȿ�˻�
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
			assert(!"���� �������� ��ġ!!");
			return FALSE;
		}
		break;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** ���� �����ؼ� Name String ID�� �޾ƿ´�.
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
/**	������ ����������
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
/** ���� ������ġ�� ������ ����ִ��� üũ.
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

	// ������ �������� ������ ������ �ϰ� Insert ��Ŵ. 
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
					//	[%s] �������� ���� �Ͽ����ϴ�.					
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf( szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}
			}
			break;

		case PICK:
			{

				if ( pBaseItemInfo )
				{
					//	[%s] �������� ȹ�� �߽��ϴ�.					
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

                        //����� or ���׸��� or ��Ų Ȯ��
                        if (ItemBuf.IsDivine())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemStatus, TotalGuideNotice::kDivine); //�����(1)/���׸���(2)/��Ų(3)
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

                        //���� or ����Ʈ Ȯ��
                        if ((pBaseItemInfo->m_bUse == true) &&                      // ������ ���ֻ̹��� ��뿩��
                             (Strcmp(pBaseItemInfo->m_szColor,_T("7875DF")) == 0))  // ���� ���� : 7875DF
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemRareAndElite, TotalGuideNotice::kRare);  // ����(1),����Ʈ(2)
                        }
                        else if (pBaseItemInfo->IsElite())
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kGetItem,
                                TotalGuideNotice::kItemRareAndElite, TotalGuideNotice::kElite);  // ����(1),����Ʈ(2)
                        }
                    }
				}
				else
				{
					assert(!"������ ������ �����!!");
				}				
			}
			break;

		case CREATE:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �Ͽ����ϴ�.
					const int	c_iCreateItem = 70270;
					g_InterfaceManager.GetInterfaceString( c_iCreateItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}				
			}
			break;

		case REWARD:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �޾ҽ��ϴ�.
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
					assert(!"������ ������ �����!!");
				}								
			}
			break;

		case EVENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� �̺�Ʈ �κ��丮���� ���� �Խ��ϴ�.
					g_InterfaceManager.GetInterfaceString( eST_REWARDITEM, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}	
			}
			break;

		case QUEST_OBTAIN:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �޾ҽ��ϴ�.
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
					assert(!"������ ������ �����!!");
				}								
			}
			break;
        case POINT_REWARD:
            {
                if ( pBaseItemInfo )
                {
                    //	[%s] �������� ���� �޾ҽ��ϴ�.
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
                {   // 3408: %s �������� �����Ǿ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(3408, szText, INTERFACE_STRING_LENGTH);
                    _stprintf(szMessage, szText, szItemName);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);
                }
            }
            break;
        case SOCKET_COMPOSTE:
            {
                if (pBaseItemInfo)
                {   // 6493: �����մϴ�. %s�� �����Ǿ����ϴ�.
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

	// ������ �������� ������ ������ �ϰ� Insert ��Ŵ. 
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
					//	[%s] �������� ���� �Ͽ����ϴ�.					
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
					_stprintf( szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}
			}
			break;

		case PICK:
			{

				if ( pBaseItemInfo )
				{
					//	[%s] �������� ȹ�� �߽��ϴ�.					
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
					assert(!"������ ������ �����!!");
				}				
			}
			break;

		case CREATE:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �Ͽ����ϴ�.
					const int	c_iCreateItem = 70270;
					g_InterfaceManager.GetInterfaceString( c_iCreateItem, szText, INTERFACE_STRING_LENGTH );
					_stprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}				
			}
			break;

		case REWARD:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �޾ҽ��ϴ�.
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
					assert(!"������ ������ �����!!");
				}								
			}
			break;

		case EVENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� �̺�Ʈ �κ��丮���� ���� �Խ��ϴ�.
					g_InterfaceManager.GetInterfaceString( eST_REWARDITEM, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);				
				}
				else
				{
					assert(!"������ ������ �����!!");
				}	
			}
			break;

		case QUEST_OBTAIN:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] �������� ���� �޾ҽ��ϴ�.
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
					assert(!"������ ������ �����!!");
				}								
			}
			break;

		case PRESENT:
			{
				if ( pBaseItemInfo )
				{
					//	[%s] ������ �����Ͽ����ϴ�.
					g_InterfaceManager.GetInterfaceString( 5978, szText, INTERFACE_STRING_LENGTH );
					Sprintf(szMessage, szText, szItemName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage, CHAT_TYPE_SYSTEM);		

				}
				else
				{
					assert(!"������ ������ �����!!");
				}	
			}
        case SHOP_STORAGE:
            {
                if ( pBaseItemInfo )
                {
                    //	[%s] �������� ���� �Ͽ����ϴ�.					
                    g_InterfaceManager.GetInterfaceString( eST_NAMECODE_BUY_ITEM, szText, INTERFACE_STRING_LENGTH );
                    _stprintf( szMessage, szText, szItemName );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );				
                }
                else
                {
                    assert(!"������ ������ �����!!");
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
/** �κ��丮���� Ư�� �������� �����Ѵ�.
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
desc : EventInventory�� �������� ä�� �ִ´�.
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

			// �ӽ������� ���� / Ŭ�� ��� �̷��� ó���ϱ�� �ߴ�.
			TempEventItem.SetCode(pEventSlot[i].m_ItemCode);
			TempEventItem.SetNum(pEventSlot[i].m_ItemCount);	

			pContainer->InsertSlot( i, TempEventItem );
		}
	}

	return bRet;
}

//----------------------------------------------------------------------
/** IsExistEmptySlotFromInventoryTotal
desc : Inventory / TempInventory �� ����ִ� ������ �ִ��� üũ�ϴ� �Լ�
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
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("������ �ڵ� %d �� �߸� �� �ڵ� �Դϴ�!!"), ItemCode);
		return FALSE;
	}

	SCItemSlot TempItem;
	TempItem.SetCode((SLOTCODE)ItemCode);
	if ( TempItem.IsOverlap() )
	{
		if ( ItemNum > pInfo->m_byDupNum )
		{
			// ������������ �������� �Ѱ� ���
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
        // ��� �ü�
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

		// �� ���� �̵�
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
        if (itemCount != 0)// ���� �̵�
        {
            FromSlot.SetNum(itemCount);
        }
		// �� ���� �̵�
		pToContainer->InsertSlot( toPos, FromSlot );
        if (itemCount != 0)// ���� �̵�
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

        // ��ȯ
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
                //! ����
                play->ShowInterface(TRUE);
            }
            else if ((fromIdx == SI_EQUIPMENT) && (toIdx != SI_EQUIPMENT))
            {
                //Ż��
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

	// ������ ���� 
	if ( SI_EQUIPMENT == fromIdx)
	{
		// Ż��
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_UNEQUIP, &FromSlot);
	}
	else if (SI_EQUIPMENT == toIdx)
	{
		// ����
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_EQUIP, &FromSlot);
	}
	else
	{
		ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, &FromSlot);
	}
}


/**
SCSlotContainer���� DummySlotContainer �� ��ũ������ ���
*/
VOID LinkDummyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos )
{
	SCSlotContainer *		pFromContainer	= ItemManager::Instance()->GetContainer( fromIdx );
	DummySlotContainer *	pToContainer	= (DummySlotContainer * )ItemManager::Instance()->GetContainer( toIdx );

	SCItemSlot &	FromItemSlot = (SCItemSlot &)pFromContainer->GetSlot(fromPos);
	DummyItemSlot	ToDummyItemSlot;

	// ���� �������� Set�Ѵ�.
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
		// ������ ������ �̵���Ų��.
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
		// �Ǹ��� ������ ����.
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
			assert(!"���ǵ��� ���� ���� - �翵 Ȯ��");
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
		// ��ȯ..
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
		// ��ȯ..
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
        assert(!"�̻��� �ڵ�� Ȯ�� �ٶ�.");
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
            // ������ ������ (����..)
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
        // ��Ȱ��ȭ ����
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
        // Ȱ��ȭ ����
        quick_slot.SetOrgCode(from_slot.GetCode());
        quick_slot.SetOrgSlotIndex(from_container_index);
        quick_slot.SetOrgPos(from_pos);					// �������� ������ �κ����� �Դٰ� ���� ��ġ�� �߿�.
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

    // ��ų
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
        assert(false && "��ų�ڵ尡 �����ʴ´�!!" );	
        return;
    }

    QuickSlot quick_slot;
    quick_slot.Clear();
    quick_slot.SetOrgSlotIndex(SI_SKILL);	
    quick_slot.SetOrgCode(skill_slot->GetCode());	// ��ų������ Code�� ���	
    quick_slot.SetOrgSlotType(ST_SKILL);
    quick_slot.SetOrgSlot(skill_slot);					// �����͵� ����� �����ش�.

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
OrgCode�� ��Ȱ��ȭ�϶��� �ǹ̰� �ִ�.
@remark
- ���� �����۰� �� ���� �������� �����ؼ� ����Ͽ���.
���� �������� �ش� ��ũ�� ���� �������� ���ų� �ٸ��� ������ ��Ȱ��ȭ��.
������ �������� �κ��丮���� �����ϴ��� ã�� ��ƾ�� �̿��ؼ� Ȱ��ȭ ��Ű�� ��ƾ�� �ְ�

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
		assert(!"�̻��� �ڵ�� Ȯ�� �ٶ�.");
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
			// ���� �������̴�.
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
			// ������ ������ (����..)
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
		// ��Ȱ��ȭ ����
		TempQuickSlot.SetOrgCode( OrgCode );
		TempQuickSlot.SetOrgSlotIndex( atIndex );
		TempQuickSlot.SetOrgPos(INVALID_POSTYPE_VALUE/*OrgPos*/); //���׼��� / ������ / SUNKR-262 ������ �̹����� �ٸ��� ��µǴ� ���� ����
		TempQuickSlot.SetOrgSlotType( ST_ITEM );
		/*	##	Scene �̵��ó� ó�� �α�ÿ� �̹� �����ϰ� �ִ� ���� ü���� �ȵǴ� ���� ����
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
		// Ȱ��ȭ ����
		TempQuickSlot.SetOrgCode(rFromSlot.GetCode());
		TempQuickSlot.SetOrgSlotIndex(atIndex);
		TempQuickSlot.SetOrgPos(OrgPos);					// �������� ������ �κ����� �Դٰ� ���� ��ġ�� �߿�.
		TempQuickSlot.SetOrgSlotType(ST_ITEM);
		TempQuickSlot.SetOrgSlot(&rFromSlot);
		TempQuickSlot.SetStatus(SLOT_RENDER_STATE_ACTIVATED);

		SCItemSlot& rFromItemSlot = (SCItemSlot&)rFromSlot;
		if (rFromItemSlot.IsOverlap())
		{
			//GS_BUG_FIX:���׸� źȯ������ó��,���������� �Ѱ����� ��������ʰڴ�
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
�������̳ʿ� ��ų ��ũ
*/
VOID
ItemManager::QuickLinkSkill( SLOTIDX atIndex, SLOTCODE OrgCode, POSTYPE toQuickPos )
{
	if (!g_pHero) return;

	BASE_SKILLINFO * pSkillInfo = 
		(BASE_SKILLINFO *)SkillInfoParser::Instance()->GetSkillInfo(OrgCode);



	if (pSkillInfo)
	{
		// ��ų
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

		SUN_ASSERT( OrgCode == pOrgSkillSlot->GetCode() && "��ų�ڵ尡 �����ʴ´�!!" );	
		TempQuickSlot.SetOrgCode( pOrgSkillSlot->GetCode() );	// ��ų������ Code�� ���	
		TempQuickSlot.SetOrgSlotType( ST_SKILL );
		TempQuickSlot.SetOrgSlot( pOrgSkillSlot );					// �����͵� ����� �����ش�.

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
��Ÿ�� �����̳ʿ� ��Ÿ�� ��ũ
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
		// ��ų
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
		TempStyleSlot.SetOrgCode( OrgCode );						// ��ų������ Code�� ���
		TempStyleSlot.SetOrgSlotType( ST_SKILL );
		TempStyleSlot.SetOrgSlot( pOrgSkillSlot );					// �����͵� ����� �����ش�.

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
// ������ composite ���� ó��
BOOL ItemManager::SpendCompositeMaterials(
	eITEMCOMPOSITECODE eItemCompositeType, 
	SLOTIDX	fromSlotIndex)
{
	SCSlotContainer *pAtContainer = GetContainer(fromSlotIndex);
    if (!pAtContainer)
    {
        return FALSE;
    }

	// ����� ���͸������ �����. 
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
				// �ߺ� ���� �ϸ� count �� �ϳ��� ��������.

				int iCurCount = rGetItemSlot.GetNum();
				int iCurRemainCnt = iCurCount - iRemainDeleteMaterialCount;

				if (0 == iCurRemainCnt)
				{
					pAtContainer->DeleteSlot(iSlotIndex, &DeleteItemSlot);
					break;
				}
				else if (iCurRemainCnt > 0)
				{
					// ����� ���ҳ�
					rGetItemSlot.SetNum( iCurRemainCnt );
					break;
				}
				else if (iCurRemainCnt < 0)
				{
					pAtContainer->DeleteSlot(iSlotIndex, &DeleteItemSlot);

					// �����µ� ���Ҵ�.
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
// Send Packet ����
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

	// ������ �̵� ��Ŷ�� ���������� ��ȿ�� üũ.
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
				//70130	�ش� �������� �̵��Ҽ������ϴ�.
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
	// ������ �̵� ��Ŷ�� ���������� ��ȿ�� üũ.
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
        g_ItemSellSyn.sell_quantity_ = ItemCount; // �Ǹ� ����(�⺻ 0)
		g_ItemSellSyn.Encode();


		//if( rItemSlot.GetEnchantLV() > ENCHANT_LOW )
        if( rItemSlot.GetEnchantLV() > popup_enchant_level )
		{			
			// 5096 ������ �Ű��Ͻðڽ��ϱ�?
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
			//5347 �������� ���� �� ���� �����Դϴ�
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
				// 1601	���� �������� ���� �� �����ϴ�.
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

	// �������� �ٴڿ� �����ðڽ��ϱ�? 5100
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
// �� �����̳� ���� ��Ŷ 
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
// ��Ÿ�� ����
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
	// class  Ȯ��

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
			// �� ���ѿ� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ��ø ���ѿ� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ���� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ���� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ���õ� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ���õ� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ü�� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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
			// ���� ����Ǿ� [%s] �������� ���� �� �� �����ϴ�
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

	// �������Ѽ�ġ ���� �ɼ� ���� ���� �ڵ� - ���� ����Ǵ��� �׽�Ʈ �ʿ�(������:091102)
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
	// �Ǽ����� ���� ���Ѽ�ġ
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
		// Divine ������ ������ ��ġ ���� : ���� ���� LimitStr���� �ȴ�.
		if( ItemSlot.IsDivine() )
		{
			LimitStr = pInfo->m_wDivLimitStr;
		}
		
		LimitStr= GlobalFunc::CalcLimitStat( LimitStr, &ItemSlot);

	}
	else
	{
		DEBUG_CODE( if(LimitStr!=0) DISPMSG("��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�\n", pInfo->m_pszName ); );
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
		// Divine ������ ������ ��ġ ���� : ���� ���� LimitStr���� �ȴ�.S
		if( ItemSlot.IsDivine() )
		{
			LimitDex = pInfo->m_wDivLimitDex;
		}
		LimitDex= GlobalFunc::CalcLimitStat( LimitDex, &ItemSlot);


	}
	else
	{
		DEBUG_CODE( if(LimitDex!=0) DISPMSG("��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�\n", pInfo->m_pszName ); );
	}

	// ���� ����

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
		// Divine ������ ������ ��ġ ���� : ���� ���� LimitStr���� �ȴ�.
		if( ItemSlot.IsDivine() == 1 )
		{
			LimitInt = pInfo->m_wDivLimitInt;
		}
	
		LimitInt= GlobalFunc::CalcLimitStat( LimitInt, &ItemSlot);

	}
	else
	{
		DEBUG_CODE( if(LimitInt!=0) DISPMSG("��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�\n", pInfo->m_pszName ); );
	}

	// ���� ����
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


	// ���� ����
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
		DEBUG_CODE( if(LimitSkill!=0) DISPMSG("��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�\n", pInfo->m_pszName ); );
	}

	// ���� ����
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
		DEBUG_CODE( if(LimitSkill!=0) DISPMSG("��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�\n", pInfo->m_pszName ); );
	}

	// ���� ����
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
		// Divine ������ ������ ��ġ ���� : ���� ���� LimitVit���� �ȴ�.
		if( ItemSlot.IsDivine() )
		{
			LimitVit = pInfo->m_wDivLimitVit;
		}
	
		LimitVit = GlobalFunc::CalcLimitStat( LimitVit, &ItemSlot);
	}
	else
	{
		//DEBUG_CODE( if(LimitVit!=0) SUNLOG( eFULL_LOG, "[CEquipmentSlotContainer::ValidLimitVit] ��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�", pInfo->m_pszName ); );
		assert(!"��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�");
	}


	// ���� ����
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
		// Divine ������ ������ ��ġ ���� : ���� ���� LimitStr���� �ȴ�.
		if( ItemSlot.IsDivine() )
		{
			LimitSpr = pInfo->m_wDivLimitSpr;
		}

		LimitSpr = GlobalFunc::CalcLimitStat( LimitSpr, &ItemSlot);
	}
	else
	{
		assert(!"��ũ��Ʈ����!���(%s)���ƴѵ�0�̾ƴϴ�");
	}


	// ���� ����
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
		//	��â���� �����۰� ��ų�� �÷� ���� �� �ֽ��ϴ�
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

		// ���� / ��� ������ ������
		if (pInfo->m_byMaterialType < eITEM_MATERIAL_TYPE_CAN_EQUIP 
            || (pItemSlot->IsEtheriaItem() || pItemSlot->IsShellItem())
            )
		{
			//	���� �� ��� ������ �����۸� �� â�� �ø� �� �ִ�.
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
				//	��Ÿ���� ��Ÿ�� â�� �÷����� �� �ֽ��ϴ�.
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
		//	��Ÿ�� â ���� �����۰� ��ų�� �÷� ���� �� �ֽ��ϴ�.
		g_InterfaceManager.GetInterfaceString( eST_STYLE_SLOT_ONLY_ITEM_AND_SKILL, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	if (pSlot->GetSlotType() == ST_SKILL)
	{
		ROOT_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetInfo(pSlot->GetCode());

		if (pInfo->IsSkill())
		{
			//	��ų�� �� â�� �÷����� �� �ֽ��ϴ�.
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

	// �̴� �������� �ϴ� ���ƴ޶� �߰�
	// ����Ǹ� ������.
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
			//	�̹� �ش� ������ ��ϵǾ� �ֽ��ϴ�.
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
			//	�̹� �ش� ������ ��ϵǾ� �ֽ��ϴ�.
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

	assert( !"�߸��� ��ũ�Դϴ�." );
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

	assert( !"�߸��� ��þƮ�Դϴ�." );
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

	// ��� �ִ³�?
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
		// �Ѿ��
		return TRUE;
	}
	else
	{
		// ������� �ʴٸ� �ΰ� �۾�
		// 1. Ż�� �� �� �ֳ�? ( ���� �����ۿ� �ִ� �������� )
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

	// ��� �ִ³�?
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
		// �Ѿ��
		return TRUE;
	}
	else
	{
		// ������� �ʴٸ� �ΰ� �۾�
		// 1. Ż�� �� �� �ֳ�? ( ���� �����ۿ� �ִ� �������� )
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
    return TRUE; //������ Ż���� ����������.
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
	// ��� �ִ³�?
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

	// ���� �����Ѱ�?
	if( 0 == pInfo->m_byMaterialType ) 
	{
		return FALSE;		
	}

    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	// ���� ���� Ŭ�����ΰ� ?
	if ( !CanEquipClass(g_pHero, pInfo ) )
	{
		if ( bOutputMsg )
		{
			//	������ �� ���� Ŭ���� �Դϴ�.
			g_InterfaceManager.GetInterfaceString( eST_CAN_NOT_EQUIP_CLASS, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		return FALSE;
	}

    if (CanEquipByGender(g_pHero, pInfo) == FALSE)
    {
        if ( bOutputMsg )
        {
            // 70294 ������ �� ���� Ŭ���� �Դϴ�.(eST_CAN_NOT_EQUIP_CLASS)
            g_InterfaceManager.GetInterfaceString(eST_CAN_NOT_EQUIP_CLASS, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        return FALSE;
    }

	SCSlotContainer * pContainer= g_pHero->GetSlotContainer( SI_EQUIPMENT );

	//Į���� Ư�� �������� ������ �Ҷ�
	if(pInfo->m_wType==eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY&&pContainer)
	{

		//�̹� ���� �ִ��� �˻� 
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
					//	�ش� �������� �ߺ� ���� �Ҽ� ���� 
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
					//	�ش� �������� �ߺ� ���� �Ҽ� ���� 
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
					//	�ش� �������� �ߺ� ���� �Ҽ� ���� 
					g_InterfaceManager.GetInterfaceString( eST_NOT_USE_DUPLICATION, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}

				return FALSE;			
			}
		}

	}


	if (GENERALPARAM->IsNet())
	{
		// ���� ����.
		if ( !CanEquipLimit((SCItemSlot &)rFromSlot, bOutputMsg)) 
		{
			return FALSE;
		}
	}


	// ����ε� ���� ��ġ�ΰ�?
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

	// �ʿ� ������ 
	int i = 0;
	for (; i < pInfo->m_byMatCount; ++i)
	{
		CODETYPE ItemCode		= pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode;
		int		 iMaterialCount = pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum; 
		int		 iAccMaterialCount = 0;	// ���� Count
		BASE_ITEMINFO * pBaseInfo = (BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(ItemCode);
		if (!pBaseInfo)
		{
			// ��ũ��Ʈ ������ �߸��Ǿ���.
			return FALSE;
		}

		int iSlotIndex = 0;
		for (; iSlotIndex < pContainer->GetMaxSlotNum(); ++iSlotIndex )
		{
			if (pContainer->IsEmpty(iSlotIndex))
				continue;

			SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(iSlotIndex);

			// ������ �ڵ尡 ������ 
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

	// �ʿ� ��
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

    //������ �ٵ��鼭 �����Ѿ������ڵ�� ������ üũ�ؼ� ���翩��Ȯ��
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
            //�����߰�����(��þ,��ũ,���) ������ �⺻��üũ
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
            //�����߰�����(��þ,��ũ,���) ������ �⺻��üũ
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
			// 5952	[%s]������ %d���� �Ҹ�Ǿ����ϴ�.
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
			// 6040	�齺���� ���� ���ð��� ����Ǿ����ϴ�.
			TCHAR szWayPointDeleteMsg[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(6040, szWayPointDeleteMsg, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szWayPointDeleteMsg);
		}			



#ifdef _YMS_AUTOBOT_GAME
        const BASE_ITEMINFO* item_info = rSlot.GetItemInfo();
        if ((item_info != NULL) && (item_info->m_wType == eITEMTYPE_AUTO_HUNTING))
        {
            //! �������� ���� �Ǿ �Ա⶧���� ������ ������.
            Autobot::Instance()->Stop(true);
            g_InterfaceManager.ShowDialog(uiAutobotPlay::kDialog_ID, FALSE);
        }
#endif //_YMS_AUTOBOT_GAME

		pContainer->DeleteSlot(FromPos, NULL);
	}

	switch (eDeleteType)
	{
	case RC_ITEM_DELETE_EXPIRED_CHARGE_ITEM: // ����ȭ ������ ��� �Ⱓ ����.
		{
			// 5870	[%s] �������� ��� �Ⱓ�� ����Ǿ� �����Ǿ����ϴ�.
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
			// ITEM_USE_ACK ������ �ʰ� DELETE�� ���⶧���� ���⼭ �Ѵ�.
			if (g_pHero)
			{
				g_pHero->OnUseItemWithCoolTime(rSlot.GetCode());
			}			
		}
		break;

	case RC_ITEM_DELETE_PROTECT_DROP:
		{
			//5952	[%s]������ %d���� �Ҹ�Ǿ����ϴ�.
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

	// �ʿ� ���͸��� ���

	BOOL	bRet = FALSE;

	// �ʿ� ������ 
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
				//1.���� ���ֺ�:�����õ���
				if (charge_subtype == eCHARGE_SUB_FIXED_AMOUNT)
				{
					rOutPos = i;
					return TRUE;
				}//2.�߰��� ���ֺ�:Ŭ���� ����
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

desc :	������ ������ �������� ����Ҷ� �ش� �����̳ʿ��� �����ϴ�
�������� ù��° ��ġ�� �޾Ƽ� �������ش�. 

param : POSTYPE & OUT rOutPos - ù��° ��ġ
���� �������� �ʴٸ� 
FALSE �� �����Ѵ�. 	
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
desc :	���������� ������ �����Ѵ�.
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

	// skill�� ���� �����;���.

	// quick slot �� Ȯ���ذ��鼭 ������ ������ Ȯ���Ѵ�. 

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

					//GS_BUG_FIX:���׸� źȯ������ó��,ù��° �������� ���������ʴ´�
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

						//GS_BUG_FIX:���׸� źȯ������ó��,���������� �Ѱ����� ��������ʰڴ�
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

					//GS_BUGFIX_QUICK:��Ţ�� �ִٸ� ���͵� ���� ��Ȱ��
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
/** �� â�� ��ũ�� â�� ���� ������ ���� ������Ʈ 
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
			OutputDebugStringA("UpdateQuickForEquipItem:: �߸��� ����!!>> ��â ����� Ȯ��!!");
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
			// ���� ����Ű�� �ִ� �ڵ�� �ٸ���


			// ����â�� �ش� �������� ��ϵǾ� �ֳ�?
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

			// �κ��丮���� �ڵ尡 ���� �������� ���������� ��ũ�����ش�.
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
/** �����Կ� ��ϵ� �����۵鿡 ���ؼ� ��ȿ���� ����. 
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
		// �κ��丮�� �������� ������ �����մϴ�.
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

	SCSlot & rFromSlot = pAtContainer->GetSlot( AtPos );			//< 1. �������� �����´�.

	// m_byMaterialType 0 : ���, ��� �Ұ�
	//					1 : ���� ���� 
	//					2 : ��� ����
	//					3 : ����, ��� ����

	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( rFromSlot.GetCode() );
	if (!pInfo)
		return;



	DWORD dwItemCode = rFromSlot.GetCode();
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG(uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG);
    if(dwItemCode == 7293 && pLottoNumInput)// ĳ�� ���� ������(7293) �۷ι�
    {
        pLottoNumInput->SendLottoEntrySyn();
        pLottoNumInput->ShowInterface(FALSE);
    }
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
	switch( pInfo->m_byMaterialType )
	{
    case eITEM_MATERIAL_TYPE_NOT_USE_CAN_DELETE:
	case eITEM_MATERIAL_TYPE_CAN_USE://< ���
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
    //��������Ʈ �������� ���Ǿ������� ���� üũ ó��
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
			dwID ));	// 1�� ������ 

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
		assert(!"�� ������ Ű�� ��ġ¡?!!");
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
			pItemInfo->m_ItemStream.Part.wCode));	// 1�� ������ 

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

            // ������ �ֻ��� ������ ���ڷ� ��ȯ
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
		// ������ ���� ack ��Ŷ �� �����ش�.
		MSG_CG_ITEM_USE_ACK SendPacketAck;
		SendPacketAck.m_atIndex = SI_INVENTORY;
		SendPacketAck.m_atPos = PortalPos;
		// ��Ż ��Ŷ�� �ӿ���?
	}
}




//////////////////////////////////////////////////////////////////////////
// ������ ���� �ʱ�ȭ 
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
			//�ش� �ൿ�� �� �� �ִ� ���°� �ƴմϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSTATE, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_CREATE_TOTEMNPC:
		{
			//	������ �����Ҽ� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5379, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDPOS:
		{ 
			// �ش� ��ġ�� ���Կ� ���� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDPOS, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_EMPTYSLOT:				// Ư����ġ�� ������ ����ִ�.
		{
			//	Ư�� ��ġ�� ������ ����ִ�.
			// Ȯ��
			g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOTEXISTITEMATFIELD:	// �ʵ忡 �������� �������� �ʴ´�.
		{
			//	�ʵ忡 �������� �������� �ʴ´�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOTEXISTITEMATFIELD, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_NOINFO:					// Code�� �ش��ϴ� ������ ������ �������� �ʴ´�.
		{
			//	Code�� �ش��ϴ� ������ ������ �������� �ʴ´�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOINFO, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOSPACEININVENTORY:		// �κ��丮�� �� ������ ����.
		{
			//	�κ��丮�� �� ������ ����
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_NOSPACEININVENTORY, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDSTATEOFPLAYER:		// �÷��̾� ���°� �̻��ϴ�
		{
			//	�÷��̾� ���°� �̻��ϴ�
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSTATEOFPLAYER, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INVALIDPOSFORDROPITEM:		// ����� �������� ��ġ�� �߸��Ǿ���.
		{
			//	����� �������� ��ġ�� �߸��Ǿ���.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDPOSFORDROPITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_DBP_ASYNC:				// DBP�κ��� �񵿱������� �ø��� �߱� ��û ����
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_DBP_ASYNC, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_COOLTIME_ERROR:			// ��Ÿ���� ������ �ʾҴ�.
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_COOLTIME_ERROR, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// Combine
	case RC_ITEM_ITEMCODENOTEQUAL:			// ������ �ڵ尡 �ٸ���.
		{
			//	������ �ڵ尡 �ٸ���.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ITEMCODENOTEQUAL, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// use
	case RC_ITEM_ISNOTWASTEITEM:			// �Ҹ� �������� �ƴϴ�.
		{
			//	�Ҹ� �������� �ƴϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ISNOTWASTEITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_PORTION_USE_LIMIT:
		{
			//5361	���� �̿� �������� ���� �� �̻� ��� �� �� �����ϴ�.	
			g_InterfaceManager.GetInterfaceString( 5361, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CAN_USE_FIELD:
		{
			//5360	�ʵ忡���� ����� �� �ֽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 5360, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;		  

	case RC_ITEM_INVALID_TYPE:
		{
			//5937	�ش� ����� �̿��� �� �ִ� ������ ������ �ƴմϴ�.
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
			// 5938	���� ���������� ����� �������� ���� ����Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5938, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_MOVE_TO_THE_AREA:
		{
			// 5939	�ش� ��ġ�� �̵��� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5939, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_LEARN_SKILL:
		{
			// 5940	��ų�� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5940, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE:
		{
			// 5941	�̵���Ű���� ����� ã�� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5941, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_NOT_STORE_COORD:
		{
			// 5942	��ǥ�� ����Ǿ� ���� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 5942, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_ALREADY_STORE_COORD:
		{
			// 5943	�̹� ��ǥ�� �����մϴ�.
			g_InterfaceManager.GetInterfaceString( 5943, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_STORE_COORD_AT_THIS_AREA:
		{
			// 5944	�ش� ��ġ������ ��ǥ�� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5944, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNOT_DELETE_FOR_SHORT_COUNT:
		{
			// 5945	���� �������� ������ �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 5945, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// sell
	case RC_ITEM_INVALIDSHOPLISTID:		// dwShopListID�� �߸��Ǿ���.
		{
			//	�߸��� �𸮽�Ʈ ID�� �����Ͽ���
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDSHOPLISTID, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_OUTOFSHOPITEMINDEX:		// shop���� �Ǹ��ϴ� ������ �ε����� �ƴϴ�.
		{
			//	�������� �Ǹ��ϴ� �������� �ƴմϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_OUTOFSHOPITEMINDEX, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_INSUFFICIENT_MONEY:
	case RC_ITEM_HAVENOTMONEY:			// ���� ����.
		{
			//	������ �����մϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_HAVENOTMONEY, szMessage, INTERFACE_STRING_LENGTH );
			g_HeroInput.PlayNotEnoughMoneySound();
		}
		break;

		// buy
	case RC_ITEM_ISNOTEMPTYSLOT:			// �� ������ ���� �� á��.
		{
			//	�� ������ ���� �� á��.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ISNOTEMPTYSLOT, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_HAVENOTSPACE:			// ������ �����ϴ�.
		{
			//	������ �����ϴ�
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

	case RC_ITEM_INVALIDVALUE:			// �߸��� ���� �Դ�.
		{
			//	�߸��� ������ �Դ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALIDVALUE, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// pick
#ifdef _JBH_ADD_ITEM_OVER_PLAYING_TIME_MESSAGE
	case RC_ITEM_OVER_PLAYING_TIME:
		{
			// 6047 : �Ƿε� ������ ������ �������� �ֿ� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(6047, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
#endif
		///.....
		// drop
	case RC_ITEM_ERRORDROPMONEY:
		{
			//	���� ������ ���ߴ�.
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
			//	��þƮ�� �ʿ��� ��ᰡ �����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_ENCHANT_HAVENOTMETERIALS, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;


		// rankup
	case RC_ITEM_INVALID_CONDITION:		// üũ ���ǿ� ���� �ʴ´�.
		{
			//	Rankup üũ ���ǿ� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_INVALID_CONDITION, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_CANNT_RANKUP_ITEM:
		{
			//	Rankup �Ҽ� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_CANNT_RANKUP_ITEM, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

		// socket
	case RC_ITEM_FULLSOCKET:				// ������ �� á��.
		{
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_FULLSOCKET, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_ITEM_EXTRACTSOCKET_FAILED:
    case RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO:
		{
			//	�������� ����
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_EXTRACTSOCKET_FAILED, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_ITEM_UNUSABLE_FUNCTION:
		{
			//5345	��� �� �� ���� ����Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5345, szMessage, INTERFACE_STRING_LENGTH );				
		}
		break;

	case RC_ITEM_INVALID_VALUE:
		{
			//5346	�߸��� ���� �Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5346, szMessage, INTERFACE_STRING_LENGTH );				
		}
		break;
	case RC_ITEM_CANNNOT_DROPSTATE:
		{
			//5347	�������� ���� �� ���� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5347, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_NOTEXIST_ITEM:
		{
			//5348	�������� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 5348, szMessage, INTERFACE_STRING_LENGTH );							

		}
		break;
	case RC_ITEM_CANNOT_SELL_ITEM:
		{
			//5349	�ǸŰ� �������� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5349, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_CANNOT_TRADE_ITEM:
		{
			//5350	�ŷ��� �������� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5350, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;
	case RC_ITEM_CANNOT_DROP_ITEM:
		{
			//5351	���� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5351, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;

	case RC_ITEM_CANNOT_EXCHANGE_ITEM:
		{
			//5352	��ȯ �� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5352, szMessage, INTERFACE_STRING_LENGTH );							
		}
		break;

	case RC_ITEM_CANNOT_MOVE_OTHER_SLOT_ITEM:
		{
			//5353	�ٸ� �������� �̵� �� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5353, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_CANNOT_DESTROY_ITEM:
		{
			//5354	�ı� �� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5354, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_CANNOT_PICKUP_OBSERVER:
		{
			//5355	���� ���¿����� �������� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5355, szMessage, INTERFACE_STRING_LENGTH );										
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL:
		{
			// ���� �������� �̿��� �� ���� ����Դϴ�.
			g_InterfaceManager.GetInterfaceString(eST_NOT_USE_CURRENT_LEVEL , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

		// ������, ����
	case RC_ITEM_DONT_NEED_TO_REPAIR:				//< ������ �ʿ� ����
		{
			//5134	������ �Ϸ�� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString(5134 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_REPAIR_ITEM:				//< ������ �Ұ����� �������Դϴ�.
		{
			//5135	������ �� �ִ� �������� �ƴմϴ�.
			g_InterfaceManager.GetInterfaceString(5135 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_COMPOSE_INVALID_LOCATION:			//< ������ �� ���� ���Դϴ�.
		{
			//5136	NPC�� �Ÿ��� �־� ������ ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5136 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_COMPOSE_LIMIT_CLASS:				//< ������ �� ���� Ŭ�����Դϴ�.
		{
			//5137	������ �� ���� Ŭ���� �Դϴ�.
			g_InterfaceManager.GetInterfaceString(5137, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE:		//< ī�����¿����� �̿��� �� ���� ����̴�.
		{
			//5275	ī�� ���¿����� �̿��� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5275, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_USE_QUEST_ACCEPT_ITEM:		//< ����Ʈ ���� �������� ����� �� �����ϴ�.
		{
			// 5138	������ ���� �ʾ� ����Ʈ ���� �������� ����� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5138, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_ELITE:				//< ����Ʈ ������ ��ũ�� �Ұ�
		{
			//5377	����Ʈ �������� ��ũ���� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5377, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_CRYSTALIZE_ELITE:			//< ����Ʈ ������ ����ȭ �Ұ�
		{
			//5376	����Ʈ �������� ����ȭ�� �������� �ʽ��ϴ�.								
			g_InterfaceManager.GetInterfaceString(5376, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_UNIQUE:				//< ����ũ ������ ��ũ�� �Ұ�
		{
			//5374	����ũ �������� ��ũ���� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5374, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_CRYSTALIZE_UNIQUE:			//< ����ũ ������ ����ȭ �Ұ�
		{
			//5373	����ũ �������� ����ȭ�� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5373, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION://< ���Ż��¿����� �̿��� �� ���� ����̴�.
		{
			// 5139	���� ���¿����� �̿��� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5139, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_LENGTH_CANNOT_USE_NPC:				//< NPC�� �̿��� �� ���� �Ÿ�.
		{
			//5141	�Ÿ��� �־� �̿��� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5141, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_DBG_WRONG_PACKET:					//< �߸��� ��Ŷ ����	
		{
			//5142	������ �߸��Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5142, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_ADD_EXTRAINVENTORY:			//< ���̻� ����ȭ �κ��丮�� �߰��� �� ����.
		{
			//5143	����ȭ �κ��丮�� �� �̻� �߰��� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5143, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_DONT_EXIST_TO_REPAIR:
		{
			//85225	������ �������� �����ϴ�
			g_InterfaceManager.GetInterfaceString(85225, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;


	case RC_ITEM_NOT_MIXTURE:
		{
			//5146	����� ����� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString( 5146 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_NOT_INVENTORY:
		{
			//5876	�κ��丮������ ������ �� �ֽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5876, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	case RC_ITEM_INVALID_SOCKET_NUM:	//< ���Ͽɼ� ������ �߸��� ���ϳѹ� �Դϴ�.
		{
			// 5956	���� �ɼ� ���� �� �߸��� ���� ��ȣ�Դϴ�.
			g_InterfaceManager.GetInterfaceString(5956, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOCKET_HAS_NOT_OPTION:	//< ���Ͽɼ��� ���� �������� ������ ���� �Ϸ��� �մϴ�.
		{
			//5875	���� �ɼ��� ���� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5875, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOCKET_DIFFERENT_LEVEL:	//< ��,�ϱ޷��� �ٸ� ���� �ɼ��� ������ �� �����ϴ�.
		{
			//5881	�Ҹ��� �� ����� �޶� ���� �ɼ��� ������ �� �����ϴ�. 
			g_InterfaceManager.GetInterfaceString(5881, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_SOKET_NOT_RUNE_ITEM:	//< ��� �������� '�Ҹ��� ��'�� �ƴϴ�.
		{
			//5882	�Ҹ��� �� �������� �ƴմϴ�.
			g_InterfaceManager.GetInterfaceString(5882, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_ISNOT_PARTY_MEMBER:	//< ��Ƽ���� ��� �������� ��� �Ͻ� �� �����ϴ�.
		{
			//5955	��Ƽ ���¿����� ����� �����մϴ�.
			g_InterfaceManager.GetInterfaceString(5955, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_NOT_UNEQUIP_ALL:					//< ��� ��� �������� ���� �����̴�.
		{
			//5884	�������� �����Ǿ� �ִ� ���¿��� ��� �Ұ��� �մϴ�.
			g_InterfaceManager.GetInterfaceString(5884, szMessage ,INTERFACE_STRING_LENGTH);
			DisplayOption |= eOUTPUT_MSG_TOP_SHOW;
		}
		break;
	case RC_ITEM_NOT_MORE_DECREASE:					//< �� �̻� ������ ������ �� ����.
		{
			//5883	ĳ���� �⺻ �ɷ�ġ�� �����Ͽ� �� �̻� ����� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5883, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_LESS_FOR_USE:						//< �ش� �������� ���ڶ��.
		{
			// 5702	��ᰡ �����մϴ�. 
			g_InterfaceManager.GetInterfaceString(5702, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_USE_MULTI_ALL_ITEM:			//< �ý��� �ʱ�ȭ �������� ������ ����� �� ����.
		{
			//5888	�� ���� �ʱ�ȭ �������� �ѹ��� ����� �����մϴ�. �� �̻� ����� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5888, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_MONSTER_SUMMON_FAILED:				//< ���� ��ȯ �������� ����� ���������� Ȯ���� ���� ����
		{
			//5726	���� ��ȯ �������� ���������� �۵��Ͽ����� ���͸� ��ȯ�ϴ� ������ �����߽��ϴ�.
			g_InterfaceManager.GetInterfaceString(5726, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_WAREHOUSE_NEED_PASSWORD:
		{
			//	â�� ��� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(5571, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_INVENTORY_NEED_PASSWORD:			//< ��� �ʿ��մϴ�.
		{
			//  �κ��丮�� ���� â�� ��� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(5572, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;
	case RC_ITEM_INVENTORY_INVALID_PASSWORD:			//< �߸��� ����Դϴ�.
	case RC_ITEM_INVENTORY_ALREADY_TRANSACTION:				//< �ٸ� ������� ó�����Դϴ�.
	case RC_ITEM_INVENTORY_PWD_SETTING_FLOW_VIOLATION:		//< ��� �������� �÷ο츦 �����߽��ϴ�.


	case RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM:
		{
			// â��� �̵��� �� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString(5891, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;



	case RC_ITEM_CANNOT_RANKDOWN_ELITE:
		{
			// ����Ʈ �������� ��ũ�ٿ� �Ҽ� �����ϴ� 6862
			g_InterfaceManager.GetInterfaceString( 6862, szMessage );
		}
		break;

	case RC_ITEM_CANNOT_RANKDOWN_UNIQUE:
		{
			// ����ũ �������� ��ũ�ٿ� �Ҽ� �����ϴ� 6863
			g_InterfaceManager.GetInterfaceString( 6863, szMessage );
		}
		break;

	case RC_ITEM_NOT_RANK_DOWN_TO_E_RANK_ITEM:
		{
			// E��ũ �������� ��ũ�ٿ� �Ҽ� �����ϴ� 5892
			g_InterfaceManager.GetInterfaceString( 5892, szMessage );
		}
		break;


    case RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND:
	case RC_ITEM_CANNOT_EXTEND_DATE:
		{
			// 5899	�Ⱓ ������ ������ �������� �ƴմϴ�.
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
			//< �ִ� ������ 2���Ͽ����� ��� ����
			g_InterfaceManager.GetInterfaceString( 6053, szMessage );
		}
		break;

	case RC_ITEM_NOT_LIMITED_ITEM:
		{
			//< ����Ƽ�� �������� �ƴ�
			g_InterfaceManager.GetInterfaceString( 6054, szMessage );
		}
		break;

	case RC_ITEM_OVER_REPAIR_COUNT:
		{
			//	6865	������ ���� Ƚ���� �Ѿ� �� �̻� ������ �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 6865, szMessage );
		}
		break;

	case RC_ITEM_CANNOT_RANKUP_LIMITED:
		{
			//	6051	����Ƽ�� �������� ��ũ���� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString( 6051, szMessage );
		}
		break;


	case RC_ITEM_CANNOT_EXTEND_CASH_SLOT:
		{
			//5312	�̹� ����Ͽ� �� �̻� Ȯ���� �� �����ϴ�
			g_InterfaceManager.GetInterfaceString(5312, szMessage);
		}
		break;
	case RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM:
		{
			//5313	����ȭ �����ۿ� ���� �� ����� �̿� �� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5313, szMessage);
		}
		break;

	case RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION:		//< [�̺�Ʈ] �̹� �ٸ� ����� �������Դϴ�. ����� �ٽ� �õ��ϼ���.
		{
			//�̹� �ٸ� ����� �������Դϴ�. ����� �ٽ� �õ��ϼ���.
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_UNKNOWNERROR, szMessage);
		}
		break;
	case RC_ITEM_EVENT_CANNOT_MOVE_TO_INVENTORY:			//< [�̺�Ʈ] Can't Move EventInven->Inven
		{
			//EventInven->Inven
			g_InterfaceManager.GetInterfaceString( eST_RC_ITEM_UNKNOWNERROR, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_REGISTER_AC_ITEM:
		{
			//5124	�� AC �������� �� �̻� ����� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5124 , szMessage);
		}
		break;


	case RC_ITEM_CANT_PICKABLE_BY_LENGTH:
		{
			//5315	�������� ������ �� ���� �Ÿ��Դϴ�.
			g_InterfaceManager.GetInterfaceString(5315, szMessage);
		}
		break;

	case RC_ITEM_CAN_NOT_EQUIP:
		{
			//5091  �������� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5091, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE:  // ��� ����� ���� ����� �� ���� ����
		{
			//7189	��� ����� ���� ����� �� ���� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(7189, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_CASHITEM: //ĳ������ ��� ����� �� ���� ����
		{
			//7190	ĳ�� �������� �־�� ����Ҽ� �ִ� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(7190, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_NOEN_OUT_RIGHT: //��� ��� �� ������ ����.
		{
			//7191	��� â�� ��� ������ �����ϴ�.
			g_InterfaceManager.GetInterfaceString(7191, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_OPEN_USENOT_CASHITEM:
		{
			//7223	��� â�� ���� ���� �� ��� â�� Ȯ�����縦 �̿��� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(7223, szMessage);
		}
		break;

	case RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM:
		{
			//7229	�Ҽӵ� ��尡 ��� ��� â�� Ȯ�� �������� ����� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(7229, szMessage);
		}
		break;
	case RC_ITEM_GUILDWAREHOUSE_NONE_USE_RIGHT:
		{
			//7230	��� â�� Ȯ������ ĳ������ ����� ������ �����ϴ�.
			g_InterfaceManager.GetInterfaceString(7230, szMessage);
		}
		break;

#ifdef	__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
	case RC_ITEM_RANKUP_FAILED_AND_DESTROY:
		{
			//4554 ��ũ���� �����߽��ϴ�!
			g_InterfaceManager.GetInterfaceString( 4554, szMessage );
		}
		break;
#endif


	case RC_ITEM_ETHER_DEVICE_USE_ONLY_TO_WEAPON:
		{	
			//6122	���׸� ������ġ�� ���⿡�� ���������մϴ�.
			g_InterfaceManager.GetInterfaceString(6122, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_EQUIP_ETHER_DEVICE:
		{
			//6123	���׸� ������ġ�� �̹� �����Ǿ� �ֽ��ϴ�.	
			g_InterfaceManager.GetInterfaceString(6123, szMessage);
		}
		break;

	case RC_ITEM_NOT_EQUIP_ETHER_DEVICE:
		{
			//6124	���׸� ������ġ�� ���� �Ǿ� �����ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(6124, szMessage);
		}
		break;

	case RC_ITEM_ALREADY_CHARGE_ETHER_BULLET:
		{
			//6125	���׸� źȯ�� �̹� �����Ǿ� �ֽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(6125, szMessage);
		}
		break;


	case RC_ITEM_NOT_SUMMONED_PET:
		{
			//6397	��ȯ�� ���� ����
			g_InterfaceManager.GetInterfaceString(6397, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_WINDOW_ALREADY_OENDEDSTATE:
		{
			//5385 â�� �̹� �����ֽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5385, szMessage);
		}
		break;

	case RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET:
		{
			//6119 ���׸� źȯ�� Ȱ��ȭ �� �� ����
			g_InterfaceManager.GetInterfaceString(6119, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET:
		{
			//6120 ���׸� źȯ�� ��Ȱ��ȭ �� �� ����
			g_InterfaceManager.GetInterfaceString(6120, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANT_EQUIP_WEAPON:
		{
			//5754: ���⸦������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5754, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CANNOT_SKILL_ACTION:
		{
			//5253 : ��ų����� ���������ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5253, szMessage, INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_CAN_USE_ONLY_CHAO:
		{
			//5280: ī�� ������ ����� �� �ִ�.(����ī�� ����)
			g_InterfaceManager.GetInterfaceString(5280, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;


	case RC_ITEM_CANNOT_USE_PLAYER_LEVEL:
		{
			//< ��� ������ ������ �ƴ�
			g_InterfaceManager.GetInterfaceString( 5297, szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_ITEM_ALREADY_FIREUP_ITEM_SAME_TYPE:			
		{
			//< �̹� ���� Ÿ���� �������� Ȱ��ȭ �Ǿ��ִ�.
			g_InterfaceManager.GetInterfaceString( 6158, szMessage,INTERFACE_STRING_LENGTH);	
		}
		break;

    
        
    case RC_ITEM_ALREADY_EXTRACTED_TO_ETHERIA:           
        {
            //6090: �̹� ���׸��ư� ����� ����Դϴ�.
            g_InterfaceManager.GetInterfaceString( 6090, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
      
    case RC_ITEM_CANT_EXTRACT_TO_ETHERIA:                
        {
             //6091: �� �������� ���׸��� ������ �Ұ����մϴ�.
            g_InterfaceManager.GetInterfaceString( 6091, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
       
    case RC_ITEM_CANT_COMBINED_WITH_ITEM:               
        {
             //< �ش� �������� ������ �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(6094, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
        
    case RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM:      
        {
            //6095: ���� ������ ������ �� �ֽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(6095, szMessage, INTERFACE_STRING_LENGTH);
         }
        break;

    case RC_ITEM_UNAVAILABLE_TARGET:      
        {
            // 3433 ������ ��� �������� ����Ҽ� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(3433, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_NOT_FRIEND:      
        {
            // 3434	�ش� �������� ��� ���������� ����� �� �ֽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(3434, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_ITEM_HEIM_LOTTO_TICKET_MAX_REACHED:
        {
            //5889: ��� ������ ������ ������ �ʰ��Ͽ����ϴ�.
            g_InterfaceManager.GetInterfaceString(5889,szMessage);
        }
        break;

    case RC_ITEM_NOT_UNIDENTIFIED_SOCKET:// ��Ȯ�� ������ �ƴϴ�
        {
            //6486: �ش� �������� ���� ������ �̹� Ȯ�εǾ��ų�, ���� �������� �ƴմϴ�
            g_InterfaceManager.GetInterfaceString(5889,szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_ITEM_IS_SHORT_HAVE_ITEM: // ������ Ÿ�� �������� �����ϱ� ���� ���� �������� �����ϴ�.
        {
            TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

            //47638	%s(��)�� �����Ͽ� �ش� �������� ������ �� �����ϴ�.
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
    case RC_ITEM_IS_SHORT_HAVE_ACCUMULATEPOINT: // ���� ����Ʈ �������� �����ϱ� ���� ���� ����Ʈ�� �����ϴ�.
        {   // 47639 ��������Ʈ�� �����Ͽ� �ش� �������� ������ �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(47639, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;

#ifdef _NA_20100307_BETAKEY_SYSTEM
    case RC_ITEM_CANNOT_EQUIPMENT_BETAKEY:       //�ش� ��ŸŰ�� ������ ���� ������ �����Ҽ� ���� �������̴�.
        {
            // 70542 ������ ���� �ʾ� �ش� ��� ������ �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(70542, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_CANNOT_USE_BETAKEY:             //�ش� ��ŸŰ�� ������ ���� ������ ����Ҽ� ���� �������̴�.
        {
            // 70543 ������ ���� �ʾ� �ش� ��� ����� �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(70543, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#endif//_NA_20100307_BETAKEY_SYSTEM
        
        // _NA_003027_20111013_HONOR_SYSTEM
    case RC_ITEM_BUY_ERROR_OF_FAME:
        {
            // 6516 �ش� �������� �����ϱ�  ���� �� ����� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(6516, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_OF_REPUTE:
        {
            // 6517 �ش� �������� �����ϱ� ���� ���� ����� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(6517, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    case RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT:
        {
            // 70701 �� �̻� �ش� �������� ������ ���� �� �����ϴ�.
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
                    // 47638 %s(��)�� �����Ͽ� �ش� �������� ������ �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceStringFormat(
                        szMessage, INTERFACE_STRING_LENGTH, 47638, currency_name);
                }
            }
        }
        break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
    case RC_ITEM_DEAD_OWNER:
        {
            // 5691 �ش� �������� �������� ���� ĳ���ʹ� ���� �������� ȹ���� �� ���� �����Դϴ�.
            g_InterfaceManager.GetInterfaceString(5691, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_GUILD_LEVEL:
        {
            // 6871 ��巹�� ������ ���� ����
            g_InterfaceManager.GetInterfaceString(6871, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
    case RC_ITEM_BUY_ERROR_GUILD_EXP:
        {
            // 6872 �����嵵 ������ ���� ����
            g_InterfaceManager.GetInterfaceString(6872, szMessage, INTERFACE_STRING_LENGTH);
        }
        break;
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
    case RC_ITEM_CANNOT_USE_ALREADY_APPLY_HIGHRANK_BUFF:
        {
            // 6880 ���� ������ �� ���� ȿ���� ���� ������ ��� ���Դϴ�. �ش� ������ ȿ���� ���� �� ����Ͻñ� �ٶ��ϴ�.
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
	case RC_ITEM_UNKNOWNERROR:			// �˼� ���� ����, Ŭ���̾�Ʈ������ �߸��� ������ Ȥ�� ��ŷ�� ���� ������ ũ��
	default:
		{
			//	�˼� ���� ����!
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
		// �̰͸� ���. 
		break;

		// �ٸ��͵��� �߰��� �ʿ䰡����.
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
�κ��丮, �����κ��丮, �̺�Ʈ �κ��丮, â�� �ִ� ������ ������ ������Ʈ �Ѵ�.
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
    //������Ʈ�� ��ȯ�ϸ鼭 dicecast �Ӽ��� �ִ� �������� �ִ��� �����Ѵ�.
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

        // �������ΰ�?
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
    //����Ʈ ����
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

        bool Is_changed = item_temp->UpdateOwner(); //�ð� ���Ȯ�� �� �ð� ����� ����
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
                is_can_get = GlobalFunc::IsMyItem(item_temp);  //���� �� �ִ� ������ ��Ȯ��
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
    //������Ʈ�� ��ȯ�ϸ鼭 �ش� AC�������� *ǥ�ø� �����Ѵ�
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
      
        // �������ΰ�?
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
    //������Ʈ�� ��ȯ�ϸ鼭 �������� *ǥ�ø� �����Ѵ�
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

        // �������ΰ�?
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
			//5311	��ϵ� �������̿��� ������ �� �����ϴ�.
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
		assert(!"�κ��丮�� �ִ� �����۸� Lock�� �����մϴ�.");
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
void ItemManager::MessageforItem(POSTYPE ItemPos) //����Ʈ�� �� ��� â�� ���� �߰� ����
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
		//6172	���������� ��� â���� ������ �߰� �Ǿ����ϴ�.
		g_InterfaceManager.GetInterfaceString(6172, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW , szMessage );
	}
    else if (eITEMTYPE_EXTEND_CHARACTER_SLOT == pItemInfo->m_wType)
    {
        //5992 ĳ���� ������ ���������� Ȯ�� �Ǿ����ϴ�. ĳ���� ����ȭ�鿡�� Ȯ���� �����մϴ�.
        g_InterfaceManager.GetInterfaceString(5992, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW , szMessage );
    }
}
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
/**  ���׸��� ���Ⱑ������ üũ(����Ʈ,�μ���������,�ͼ�,����ũ,��Ű)�Ұ�
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
/**  ���׸��� ĳ������Ŷ(C2S)
*/
void ItemManager::SendEtheriaCastingStart(POSTYPE slot_pos, POSTYPE etherealizer_pos)
{
    if (g_pHero == NULL)
    {
        return;
    }
    //���̵����¿����� �׼ǺҰ�
    if (g_pHero->IsRiding())
    {
        //6093:�޽��� ��� ���̵����¿����� ������ �Ұ����մϴ�.
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW ,
                                     g_InterfaceManager.GetInterfaceString(6093));
        return;
    }

    if (g_pHero->GetCurState() == STATE::SUMMON_RIDER)
    {
        return; 
    }

    
    //���� �Ǵ�
    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
    if (inventory_manager != NULL)
    {
        inventory_manager->LockInventorySlot(slot_pos);
        inventory_manager->LockInventorySlot(etherealizer_pos);
    }

    // ���׸��� ĳ���ý�����Ŷ
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_CastingStart; 
    send_packet.header.args.extract.slot_pos = slot_pos;
    send_packet.header.args.extract.etherealizer_pos = etherealizer_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}
//--------------------------------------------------------------------------------------------------
/**  ���׸��� ������Ŷ(C2S)
*/
void ItemManager::SendEtheriaExtractPacket(POSTYPE slot_pos, POSTYPE etherealizer_pos)
{
    // ���׸��� ���� ��Ŷ ����ä��(��ûŸ��,�����������ġ)
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_Extract; 
    send_packet.header.args.extract.slot_pos = slot_pos;
    send_packet.header.args.extract.etherealizer_pos = etherealizer_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    
}
//--------------------------------------------------------------------------------------------------
/**  ���׸��� ������Ŷ(C2S)
*/
void ItemManager::SendEtheriaCombinationPacket(POSTYPE to_pos, POSTYPE from_pos)
{
    // ���׸��� ���� ��Ŷ ����ä��(��ûŸ��,�����������ġ)
    MSG_CG_ITEM_ETHERIA_CONTROL_SYN send_packet;
    send_packet.header.request = send_packet.header.eReq_Combine; 
    send_packet.header.args.combine.from_etheria = from_pos;
    send_packet.header.args.combine.to_shell = to_pos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    
}

//--------------------------------------------------------------------------------------------------
/**  ���� Ȯ�� �������� üũ ���� �������� �ƴϸ� �Ұ�
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
        case eSOCKET_UNIDENTIFIED_ITEM://��Ȯ��
        case eSOCKET_IDENTIFIED_ITEM://���ϰ���Ȯ�ε�
        {
            socket_number = 0;
        }
        break;
        case eSOCKET_IDENTIFY_NOT_PROCESS: // ���� �ƴ�
        case eSOCKET_FAIL_IDENTIFY_PROCESS:// ���� 0���� ������
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
// ������ �Ⱓ����


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
        g_InterfaceManager.GetInterfaceString( 6322, message, INTERFACE_STRING_LENGTH); //[%s]�����ۿ� �ð��� �ǵ��� �ɼ��� ȿ���� �ٽ� �ο��Ͻðڽ��ϱ�? \n(���� �ɼ��� ��� ������� ���ο� �ɼ��� �ٽ� �ο� �˴ϴ�.)

        TCHAR buffer[INTERFACE_STRING_LENGTH];
        wsprintf(buffer, message, target_name );

        local_randomize_item::local_source_position = source_pos;
        local_randomize_item::local_target_position = target_pos;

        uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        pSystemMan->Sys_Confirm(buffer, local_randomize_item::_OnConfirm );
    }
    else
    {
        // 5146 ����� ��� �� �� ���� �������Դϴ�.
        TCHAR buffer[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(5146, buffer, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_SysMsg(buffer);
    }

}
#endif

