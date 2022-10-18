#include "SunClientPrecompiledHeader.h"
#include "itemmanager.h"
#include <scslotHeader.h>
#include <SCSlot.h>
#include <SCSlotContainer.h>
#include "QuickSlot.h"
#include "MouseHandler.h"
#include "EquipmentContainer.h"
#include <ItemInfoParser.h>
#include "Hero.h"
#include "InventoryDialog.h"
#include "ShopDialog.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "InterfaceManager.h"
#include "Item.h"
#include "ObjectManager.h"
#include "QuickContainer.h"
#include "ChatDialog.h"
#include "ItemCompositeparser.h"
#include "SCSkillSlot.h"
#include "SkillInfoParser.h"
#include "StyleSlot.h"
#include "StyleContainer.h"
#include "SoundEffect.h"
#include "globalfunc.h"
#include "EnduranceDialog.h"
#include "ItemAttrCalculator.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiFateItemSystemMan/uiFateItemSystemMan.h"
#include "uiFateItemSystem/uiFateItemSystem.h"
#include "HeroEquipmentContainer.h"

#ifdef __NA_001358_ENCHANT_UNIFICATION
#include "uiEnchantMan/uiEnchantMan.h"
#else //__NA_001358_ENCHANT_UNIFICATION
	#include "EnchantDialog2.h"
#endif //__NA_001358_ENCHANT_UNIFICATION

#include "LotteryInventoryDialog.h"
#include "uiItemStatChangeSysMan/uiItemStatChangeSysMan.h"
#include "uiItemStatChangeSys/uiItemStatChangeSys.h"
#include "uiPetMan/uiPetMan.h"
#include "uiPetNameSetting/uiPetNameSetting.h"
#include "RiderInventoryDialog.h"
#include "RiderContainer.h"
#include "SkillAttrCalculator.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
#include "InventoryFilterDialog.h"
#include "SceneBase.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#include "uiSocketComposite/uiSocketComposite.h"
#include "uiSocketMan/uiSocketMan.h"

#include "uiAssistMan/uiAssistMan.h"
#include "uiAskSocketIdentifyDialog/uiAskSocketIdentifyDialog.h"
#include "uiAskSocketRecreateDialog/uiAskSocketRecreateDialog.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "Char_SkillDialog.h"
#include "HeroData.h"

using namespace RC;

VOID ItemManager::OnRecvItemSync(MSG_BASE * pMsg )
{
	switch (pMsg->m_byProtocol)
	{
	case CG_SYNC_ALLFIELDITEMS_CMD:
		{
			MSG_CG_SYNC_ALLFIELDITEMS_CMD * pRecvMsg = (MSG_CG_SYNC_ALLFIELDITEMS_CMD *) pMsg;

			//////////////////////////////////////////////////////////////////////////
			// ������ ����Ʈ�� �޾Ƽ� ó���Ѵ�.
			BOOL bResult = FALSE;
			for (int i = 0; i < pRecvMsg->m_byCount; ++i)
			{
				bResult = ItemManager::Instance()->AddFieldItem(&pRecvMsg->m_pFieldItemRenderInfo[i]);
				__asm nop;
			}

			
		}
		break;

	case CG_SYNC_FIELDITEM_ENTER_BRD:
		{
			// �������� ���� AddFieldItem() �Լ��� ó������.
			MSG_CG_SYNC_FIELDITEM_ENTER_BRD * pRecvMsg = (MSG_CG_SYNC_FIELDITEM_ENTER_BRD *)pMsg;

			ItemManager::Instance()->AddFieldItem(&pRecvMsg->m_ItemRenderInfo);

			if (pRecvMsg->m_dwFromMonsterKey)
			{
				Item *pItem =  (Item *)g_ObjectManager.GetObject(pRecvMsg->m_ItemRenderInfo.m_dwObjectKey);

				if(pItem)
				{
					pItem->HideItem();
					pItem->SetDropMonsterID(pRecvMsg->m_dwFromMonsterKey);

				}
			}
            
		}
		break;

	case CG_SYNC_FIELDITEM_LEAVE_BRD:
		{
			MSG_CG_SYNC_FIELDITEM_LEAVE_BRD * pRecvMsg = (MSG_CG_SYNC_FIELDITEM_LEAVE_BRD *)pMsg;

			for( int i = 0; i < pRecvMsg->m_byCount; ++i )
			{
				Item * pItem = static_cast<Item *>(g_ObjectManager.GetObject(pRecvMsg->m_dwObjectKey[i]) );

				if (pItem == NULL)	continue;

				g_ObjectManager.Delete(pRecvMsg->m_dwObjectKey[i]);
			}
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ItemManager::OnrecvItemComposite(MSG_BASE * pMsg)
{
	TCHAR	szMessage[MAX_MESSAGE_LENGTH];

    switch( pMsg->m_byProtocol )
	{
	case CG_ITEM_SOCKET_FILL_ACK:
		{
			MSG_CG_ITEM_SOCKET_FILL_ACK * pRecvMsg = (MSG_CG_ITEM_SOCKET_FILL_ACK *)pMsg;

			SCSlotContainer * pAtContainer = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
			SCItemSlot & rItemSlot		= (SCItemSlot &)pAtContainer->GetSlot( pRecvMsg->m_TargetPos );
			SCItemSlot & rCrystalSlot	= (SCItemSlot &)pAtContainer->GetSlot( pRecvMsg->m_SocketItemPos );


			// ����� ��þƮ ������ ī��Ʈ �ϳ�������.
			SCItemSlot DeleteCrystalItemSlot;
			if (rCrystalSlot.IsOverlap())
			{
				if (rCrystalSlot.GetNum() > 1	)
				{
					rCrystalSlot.SetNum( rCrystalSlot.GetNum()-1 );
					pAtContainer->UpdateSlot(pRecvMsg->m_SocketItemPos, rCrystalSlot );
				}
				else
				{
					pAtContainer->DeleteSlot(pRecvMsg->m_SocketItemPos, &DeleteCrystalItemSlot);
                    assert(eITEMTYPE_SOCKET_ZARD == DeleteCrystalItemSlot.GetItemInfo()->m_wType && "�����̾ƴѵ� �����ٴ�!!!!");
				}	
			}
			else
			{
				pAtContainer->DeleteSlot(pRecvMsg->m_SocketItemPos, &DeleteCrystalItemSlot);
                assert(eITEMTYPE_SOCKET_ZARD == DeleteCrystalItemSlot.GetItemInfo()->m_wType && "�����̾ƴѵ� �����ٴ�!!!!");
			}

			// ��� �� ����
			SCItemSlot InsertItemSlot(pRecvMsg->m_TargetItemStream);

			pAtContainer->DeleteSlot(pRecvMsg->m_TargetPos, NULL);
			pAtContainer->InsertSlot(pRecvMsg->m_TargetPos, InsertItemSlot);

			// ���� ���
			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, (SCSlot *)&InsertItemSlot);

			TCHAR szResult[256];
			TCHAR szItemName[256];

			g_InterfaceManager.GetItemName(rItemSlot.GetCode(), szItemName, 256);

			//	%s �ɼ� �߰��� �����Ͽ����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_ITEM_OPTION_ADD_SUCCESS, szMessage, MAX_MESSAGE_LENGTH );
			_stprintf(szResult, 
				szMessage, 
				szItemName
				);

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);
		}
		break;

	case CG_ITEM_SOCKET_FILL_NAK:
		{
			MSG_CG_ITEM_SOCKET_FILL_NAK * pRecvMsg = (MSG_CG_ITEM_SOCKET_FILL_NAK *)pMsg;
      
            if (pRecvMsg->m_dwErrorCode == RC_ITEM_SOCKET_FAILED_TO_FILL_BY_GRADE)// ���Ͽ� �ִ� ��밡���� ����� ������ �Ѿ���.
            {
                TCHAR error_message[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};

                g_InterfaceManager.GetItemName(pRecvMsg->target_item_code, item_name);
                g_InterfaceManager.GetInterfaceString(65017, temp_message, INTERFACE_STRING_LENGTH);
                sprintf(error_message, temp_message, item_name);
                //65017 [%s] ��񿡴� �� �̻� �ش����� ��� �ɼ��� �߰��� �� �����ϴ�.
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, error_message);
            }
            else
            {
			    NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
            }

		}
		break;

	case CG_ITEM_EXTRASTONE_USE_ACK:
		{
			MSG_CG_ITEM_EXTRASTONE_USE_ACK * pRecvMsg = (MSG_CG_ITEM_EXTRASTONE_USE_ACK *)pMsg;

			ItemManager::Instance()->DeleteItem(SI_INVENTORY, MouseHandler::Instance()->GetCrystalPos(), 1);
			SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );
			if (!pContainer )
				return;
			
			assert( !pContainer->IsEmpty( MouseHandler::Instance()->GetPendingTargetPos() ) );
			SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot( MouseHandler::Instance()->GetPendingTargetPos() );
			rItemSlot.SetNOptionAttr(pRecvMsg->m_NOptionAttrValueIndex);

			// ����Ʈ�� ������ �����߽��ϴ�.
			const int c_iApplyExtraStone = 5302;			
			g_InterfaceManager.GetInterfaceString(c_iApplyExtraStone, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case CG_ITEM_EXTRASTONE_USE_NAK:
		{
			MSG_CG_ITEM_EXTRASTONE_USE_NAK * pRecvMsg = (MSG_CG_ITEM_EXTRASTONE_USE_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);
		}
		break;


	case CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK:
	case CG_ITEM_SOCKET_EXTRACT_FAILED_ACK:
	case CG_ITEM_SOCKET_EXTRACT_NAK:
	case CG_ITEM_ACCESSORY_CREATE_ACK:
	case CG_ITEM_ACCESSORY_CREATE_NAK:
		{
			g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_ITEM_ETC_COMPOSE, pMsg);
		}
		break;
	}
}

VOID								
ItemManager::OnrecvItemEnchant(MSG_BASE * pMsg)
{
	g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_ENCHANT, pMsg);
}


VOID ItemManager::ParsePacket( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	// TODO:
	// �Լ��� ¥����.. -_-;

	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T3( pMsg, 0 );
		}
		break;

	case CG_ITEM_PICK_MONEY_ACK:
		{
			MSG_CG_ITEM_PICK_MONEY_ACK * pRecvMsg = (MSG_CG_ITEM_PICK_MONEY_ACK *)pMsg;

			LONGLONG	CurrentMoney = 0;
			if (g_pHero) 
			{
				CurrentMoney = g_pHero->GetMoney();
				g_pHero->SetMoney(pRecvMsg->m_Money);
			}

			TCHAR szChatResult[INTERFACE_STRING_LENGTH] = {0,};

			LONGLONG acquiredMoney = pRecvMsg->m_Money - CurrentMoney;
			int bonusMoney = 0;
			if( pRecvMsg->m_BonusRatio > 0)
			{
				bonusMoney  =  (int ) ceil( (float) acquiredMoney *  ( (float) pRecvMsg->m_BonusRatio / (100.f + (float) pRecvMsg->m_BonusRatio) ) );
			}
			if( bonusMoney > 0)
			{
				//5095 %I64u ������ ȹ���Ͽ����ϴ�.(�߰� ���� %d ȹ��)
				g_InterfaceManager.GetInterfaceString(5095, szMessage);
				Snprintf(szChatResult, INTERFACE_STRING_LENGTH-1, szMessage, acquiredMoney - bonusMoney, bonusMoney);
			}
			else
			{
				//	%I64u ���Ӹ� ȹ���Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString( eST_CODE_GET_MONEY, szMessage, MAX_MESSAGE_LENGTH );
				_stprintf(szChatResult, szMessage, acquiredMoney);
			}
#ifdef _DEV_VER
			TCHAR szDebugMessage[INTERFACE_STRING_LENGTH] ={0,};
			Snprintf(szDebugMessage,  INTERFACE_STRING_LENGTH -1,"DEBUG!! ȹ�� ���� = %I64u , BonusRatio = %d" , acquiredMoney, pRecvMsg->m_BonusRatio );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szDebugMessage);
#endif //_DEV_VER


			// ���־��ٴ� �޽��� �ٿ��.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szChatResult, CHAT_TYPE_SYSTEM);

				uiBattleScoreMan* pBattleScoreMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

				if (CurrentMoney>0 && pBattleScoreMan)
				{
					pBattleScoreMan->RefreshInfo(0, acquiredMoney);
				}

			if( GAMEOPTION->GetPickUpItemNotify() )
			{
				GameFramework::GetCurrentScene()->InserCenterTextMessage( szChatResult, PICK );
			}
		}
		break;

	case CG_ITEM_DESTROY_DURA_CMD:
		{
			MSG_CG_ITEM_DESTROY_DURA_CMD * pRecvMsg = (MSG_CG_ITEM_DESTROY_DURA_CMD *)pMsg;
			
			SCSlotContainer * pFromContainer = (SCSlotContainer *)GetContainer(pRecvMsg->m_SlotIndex);
	
			if(pFromContainer)
			{
				SCSlot & rFromSlot = pFromContainer->GetSlot( pRecvMsg->m_SlotPos);
				TCHAR szName1[INTERFACE_STRING_LENGTH];

				BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rFromSlot.GetCode());
				if (pItemInfo)
				{
					g_InterfaceManager.GetItemName( rFromSlot.GetCode(), szName1, INTERFACE_STRING_LENGTH );

					g_InterfaceManager.GetInterfaceString( eST_ITEM_DESTROY_DURA, szText, INTERFACE_STRING_LENGTH );

					_stprintf( szMessage, szText,szName1);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

					pFromContainer->DeleteSlot(pRecvMsg->m_SlotPos, NULL);
	
					MouseHandler::Instance()->RemoveItemAtHand();

				}
			}
		}
		break;

	case CG_ITEM_PICK_BRD:
		{
			MSG_CG_ITEM_PICK_BRD * pRecvMsg = (MSG_CG_ITEM_PICK_BRD *)pMsg;

			Object * pObj = g_ObjectManager.GetObject( pRecvMsg->m_dwObjKey );

			TCHAR szName1[INTERFACE_STRING_LENGTH];

			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pRecvMsg->m_ItemCode);
			if (pItemInfo && pObj)
			{
				g_InterfaceManager.GetItemName( pRecvMsg->m_ItemCode, szName1, INTERFACE_STRING_LENGTH );
				g_InterfaceManager.GetInterfaceString( eST_ITEM_PICK, szText, INTERFACE_STRING_LENGTH );

				_stprintf( szMessage, szText,pObj->GetName(),szName1 );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}

		}
		break;

	case CG_ITEM_USE_BRD:
		{
			MSG_CG_ITEM_USE_BRD * pRecvMsg = (MSG_CG_ITEM_USE_BRD *)pMsg;
			if( Player* pObject = g_ObjectManager.GetPlayer(pRecvMsg->m_dwObjKey) )
            {
				Player* pPlayer=(Player*)pObject;
				pPlayer->CreateWasteItemEffect(pPlayer, pRecvMsg->m_ItemCode);
			}
		}
		break;

//#ifdef _NA_006372_20130108_BADGE_ITEM
    case CG_ITEM_USE_TOGGLE_ACK:
        {
            MSG_CG_ITEM_USE_TOGGLE_ACK* pRecvMsg = (MSG_CG_ITEM_USE_TOGGLE_ACK*)pMsg;

            if( pRecvMsg->toggle_item_type_ == kToggleItem_Badge )
            {
                if( xGetHeroData()->GetBadgeItemData().IsActive() == true )
                {
                    POSTYPE current_equiped_item_position = xGetHeroData()->GetBadgeItemData().GetItemPosition();
                    if( SCItemSlot* pItem = ItemManager::Instance()->GetInventoryItem( current_equiped_item_position ) )
                    {
                        pItem->OnUnEquip();
                    }
                }

                // Ȱ��
                if( pRecvMsg->is_equipped_ == true )
                {
                    if( SCItemSlot* pItem = ItemManager::Instance()->GetInventoryItem( pRecvMsg->item_pos_ ) )
                    {
                        pItem->OnEquip();

                        nsSlot::TOGGLE_ITEMS data;
                        data.is_equiped_ = pRecvMsg->is_equipped_;
                        data.item_pos_ = pRecvMsg->item_pos_;
                        data.item_code_ = pItem->GetCode();
                        xGetHeroData()->SetBadgeItemData(data);
                        g_pHero->RefreshBadgeItemEffect();
                        g_pHero->UpdateItemAttrCalculator();
                        g_pHero->UpdateSkillAttr();

                        break;
                    }
                }

                // ��Ȱ��
                nsSlot::TOGGLE_ITEMS data;
                data.clear();
                xGetHeroData()->SetBadgeItemData(data);
                g_pHero->RefreshBadgeItemEffect();
                g_pHero->UpdateItemAttrCalculator();
                g_pHero->UpdateSkillAttr();
            }
            else if( pRecvMsg->toggle_item_type_ == kToggleItem_PCBang )
            {
                if( xGetHeroData()->GetPCBangItemData().IsActive() == true )
                {
                    POSTYPE current_equiped_item_position = xGetHeroData()->GetPCBangItemData().GetItemPosition();
                    if( SCItemSlot* pItem = ItemManager::Instance()->GetInventoryItem( current_equiped_item_position ) )
                    {
                        pItem->OnUnEquip();
                    }
                }

                // Ȱ��
                if( pRecvMsg->is_equipped_ == true )
                {
                    if( SCItemSlot* pItem = ItemManager::Instance()->GetInventoryItem( pRecvMsg->item_pos_ ) )
                    {
                        pItem->OnEquip();

                        nsSlot::TOGGLE_ITEMS data;
                        data.is_equiped_ = pRecvMsg->is_equipped_;
                        data.item_pos_ = pRecvMsg->item_pos_;
                        data.item_code_ = pItem->GetCode();
                        xGetHeroData()->SetPCBangItemData(data);
                        g_pHero->RefreshPCBangItemEffect();
                        g_pHero->UpdateItemAttrCalculator();
                        g_pHero->UpdateSkillAttr();

                        break;
                    }
                }
                // ��Ȱ��
                nsSlot::TOGGLE_ITEMS data;
                data.clear();
                xGetHeroData()->SetPCBangItemData(data);
                g_pHero->RefreshPCBangItemEffect();
                g_pHero->UpdateItemAttrCalculator();
                g_pHero->UpdateSkillAttr();
            }
        }
        break;
//#endif //_NA_006372_20130108_BADGE_ITEM

    case CG_ITEM_PICK_MONEY_NAK:
        {
			MSG_CG_ITEM_PICK_MONEY_NAK * pRecvMsg = (MSG_CG_ITEM_PICK_MONEY_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			Item *pItem = (Item *)g_ObjectManager.GetObject( pRecvMsg->m_dwFieldItemObjectKey );
			if( pItem )
			{
				pItem->SetSendPickPacket( FALSE );
				pItem->InitState(STATE::ITEM_DROP_STATE, g_CurTime);
			}

			//	CG_ITEM_PICK_MONEY_NAK
            WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );

#if defined( _DEBUG )
            g_InterfaceManager.ShowMessageBox( _T("CG_ITEM_PICK_MONEY_NAK"), 10000, &Color, 0 );
#endif
        }
        break;

    case CG_ITEM_DIVIDE_ACK:
        {
            MSG_CG_ITEM_DIVIDE_ACK * pRecvMsg = (MSG_CG_ITEM_DIVIDE_ACK *)pMsg;

            MoveItemWithDiv();

            MouseHandler::Instance()->RemoveItemAtHand();
			
			//GS_BUG_FIX:������ġ�� Ʋ���� �����Ҷ� ���������߸���
			UpdateQuickInfo();

			//�κ������� �������� �и��ϴ� ��쿣 ����Ʈ ������ ������ �ʿ� ����.
			//g_InterfaceManager.UpdateQuestInfo();// ����Ʈ ���� ����
        }
        break;

    case CG_ITEM_DIVIDE_NAK:
        {
            MSG_CG_ITEM_DIVIDE_NAK * pRecvMsg = (MSG_CG_ITEM_DIVIDE_NAK *)pMsg;

            NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

            MouseHandler::Instance()->RollbackItemAtHand();
        }
        break;

	case CG_ITEM_MOVE_ACK:
		{
			MSG_CG_ITEM_MOVE_ACK * pRecvMsg = (MSG_CG_ITEM_MOVE_ACK *)pMsg;
            QuickContainer* quick_container = (QuickContainer*)g_pHero->GetQuickContainer();
            if (quick_container)
            {
                if ((pRecvMsg->m_fromIndex == SI_INVENTORY || pRecvMsg->m_fromIndex == SI_EQUIPMENT) &&
                    (pRecvMsg->m_toIndex == SI_INVENTORY || pRecvMsg->m_toIndex == SI_EQUIPMENT))
                {
                    quick_container->set_is_item_move(true);
                }
            }
            MoveItem( pRecvMsg->m_fromIndex, pRecvMsg->m_toIndex, pRecvMsg->m_fromPos, pRecvMsg->m_toPos, pRecvMsg->move_quantity_);
            if (quick_container)
            {
                quick_container->set_is_item_move(false);
            }
            MouseHandler::Instance()->RemoveItemAtHand();
		}
		break;

	case CG_ITEM_MOVE_NAK:
		{
			MSG_CG_ITEM_MOVE_NAK * pRecvMsg = (MSG_CG_ITEM_MOVE_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

// ----------------------------------------------------------------------------------------
// ���� ���� QUICK ��ũ �̵� ó��
	case CG_ITEM_QUICK_LINKITEM_ACK:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_ACK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKITEM_ACK *)pMsg;

			SCSlotContainer * pFromContainer = (SCSlotContainer *)GetContainer(SI_INVENTORY);
			
			// ������ ��ũ
			QuickContainer * pQuickDlg = (QuickContainer * )GetItemContainerDialog(SI_QUICK);
			
			SCSlot & rFromSlot = pFromContainer->GetSlot( pRecvMsg->m_OrgPos);
			QuickLinkItem( SI_INVENTORY, pRecvMsg->m_OrgPos, pRecvMsg->m_ToPos, rFromSlot.GetCode());

			MouseHandler::Instance()->RollbackItemAtHand();		

			TCHAR szName1[INTERFACE_STRING_LENGTH];

			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rFromSlot.GetCode());
			if (pItemInfo)
			{
				g_InterfaceManager.GetItemName( rFromSlot.GetCode(), szName1, INTERFACE_STRING_LENGTH );
				//	[%s] �������� ��� �Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString( eST_ITEM_QUICK_LINKITEM, szText, INTERFACE_STRING_LENGTH );
				_stprintf( szMessage, szText, szName1 );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
		}
		break;

	case CG_ITEM_QUICK_LINKITEM_NAK:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_NAK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKITEM_NAK *)pMsg;

#if defined( _DEBUG )
			g_InterfaceManager.ShowMessageBox( _T("CG_ITEM_QUICK_LINKITEM_NAK"), 10000, NULL, 0 );
#endif

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

	case CG_ITEM_QUICK_LINKSKILL_ACK:
		{
			MSG_CG_ITEM_QUICK_LINKSKILL_ACK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKSKILL_ACK *)pMsg;


			SCSlotContainer * pFromContainer = (SCSlotContainer *)GetContainer(SI_SKILL);
			
			// ������ ��ũ
			QuickContainer * pQuickDlg = (QuickContainer * )GetItemContainerDialog(SI_QUICK);			

			SCSkillSlot * pSkillSlot = NULL;

			//��ũ�� �ɾ��ش�.	
			QuickLinkSkill( SI_SKILL, pRecvMsg->m_SkillCode, pRecvMsg->m_ToPos);
			MouseHandler::Instance()->RemoveItemAtHand();

			pSkillSlot = const_cast<SCSkillSlot *>(this->GetSkillSlotByCode( pRecvMsg->m_SkillCode ) );
			if (pSkillSlot)
			{
				TCHAR szSkillName[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
				//	[%s] ��ų�� ��� �߽��ϴ�.
				g_InterfaceManager.GetInterfaceString( eST_ITEM_QUICK_LINKSKILL, szText, INTERFACE_STRING_LENGTH);
				_stprintf( szMessage, szText, szSkillName );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

				// ��ų ��� �ϴ� �Ҹ�
				ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_LINK, pSkillSlot);
			}
		}
		break;

	case CG_ITEM_QUICK_LINKSKILL_NAK:
		{
			MSG_CG_ITEM_QUICK_LINKSKILL_NAK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKSKILL_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
		}
		break;

	case CG_ITEM_QUICK_UNLINK_ACK:
		{
			MSG_CG_ITEM_QUICK_UNLINK_ACK * pRecvMsg = (MSG_CG_ITEM_QUICK_UNLINK_ACK *)pMsg;

			SCSlotContainer * pContainer = GetContainer(SI_QUICK);
			
			assert(pContainer);
			if(!pContainer)
				return;

			QuickSlot & rQuickSlot = (QuickSlot &)pContainer->GetSlot(pRecvMsg->m_atPos);

			switch ( rQuickSlot.GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					TCHAR szName1[INTERFACE_STRING_LENGTH];

					BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rQuickSlot.GetOrgCode());
					if (pItemInfo)
					{
						g_InterfaceManager.GetItemName(rQuickSlot.GetOrgCode(), szName1, INTERFACE_STRING_LENGTH);
						//	[%s] �������� ���� �Ͽ����ϴ�.
						g_InterfaceManager.GetInterfaceString(eST_ITEM_QUICK_UNLINK_ITEM, szText, INTERFACE_STRING_LENGTH);
						_stprintf(szMessage,szText, szName1);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
				break;

			case ST_SKILL:
				{
					SCSkillSlot * pSkillSlot = const_cast<SCSkillSlot *>(this->GetSkillSlotByCode( rQuickSlot.GetOrgCode()) );
					if (pSkillSlot)
					{
						TCHAR szSkillName[INTERFACE_STRING_LENGTH];

						g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
						//	[%s] ��ų�� ���� �Ͽ����ϴ�.
						g_InterfaceManager.GetInterfaceString( eST_ITEM_QUICK_UNLINK_SKILL, szText, INTERFACE_STRING_LENGTH);
						_stprintf(szMessage, szText, szSkillName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}					
				}
				break;
			}

			pContainer->DeleteSlot(pRecvMsg->m_atPos, NULL);	
			MouseHandler::Instance()->RemoveItemAtHand();

			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_UNLINK, (SCSlot *)&rQuickSlot);
		}
		break;

	case CG_ITEM_QUICK_UNLINK_NAK:
		{
			MSG_CG_ITEM_QUICK_UNLINK_NAK * pRecvMsg = (MSG_CG_ITEM_QUICK_UNLINK_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;


	case CG_ITEM_QUICK_MOVE_ACK:
		{
			MSG_CG_ITEM_QUICK_MOVE_ACK * pRecvMsg = (MSG_CG_ITEM_QUICK_MOVE_ACK *)pMsg;

			SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer( SI_QUICK );
			QuickSlot & rQuickSlot = (QuickSlot &)pFromContainer->GetSlot(pRecvMsg->m_fromPos);

			ItemManager::Instance()->SimplexMoveLinkItem( 
				SI_QUICK, 
				pRecvMsg->m_fromPos, 
				pRecvMsg->m_toPos );

			MouseHandler::Instance()->RemoveItemAtHand();
		}
		break;

	case CG_ITEM_QUICK_MOVE_NAK:
		{
			MSG_CG_ITEM_QUICK_MOVE_NAK * pRecvMsg = (MSG_CG_ITEM_QUICK_MOVE_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

	///////////////////////////////////////////////////////////////////////////////////////////

	case CG_ITEM_PICK_ACK:
		{
			MSG_CG_ITEM_PICK_ACK * pRecvMsg = (MSG_CG_ITEM_PICK_ACK *)pMsg;

			ItemManager::Instance()->InsertInventoryTotalInfo( PICK, pRecvMsg->m_ItemInfo);
		}
		break;
	case CG_ITEM_PICK_NAK:
		{
			MSG_CG_ITEM_PICK_NAK * pRecvMsg = (MSG_CG_ITEM_PICK_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			Item *pItem = (Item *)g_ObjectManager.GetObject( pRecvMsg->m_dwFieldItemObjectKey );
			if( pItem )
			{
				pItem->SetSendPickPacket( FALSE );
				pItem->InitState(STATE::ITEM_DROP_STATE, g_CurTime);
			}

		}
		break;

	case CG_ITEM_DROP_ACK:
		{
			MSG_CG_ITEM_DROP_ACK * pRecvMsg = (MSG_CG_ITEM_DROP_ACK *)pMsg;

			SCItemSlot DropItem;

			SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_atIndex);
			assert(pContainer);
			if(!pContainer)
				return;

			assert(!pContainer->IsEmpty( pRecvMsg->m_atPos ));

			SCSlot & rDropSlot = pContainer->GetSlot( pRecvMsg->m_atPos );
			pContainer->DeleteSlot(pRecvMsg->m_atPos, &DropItem);


			MouseHandler::Instance()->RemoveItemAtHand();	
		}
		break;

	case CG_ITEM_PK_DROP_ITEM_CMD:
		{
			MSG_CG_ITEM_PK_DROP_ITEM_CMD * pRecvMsg = (MSG_CG_ITEM_PK_DROP_ITEM_CMD *)pMsg;

			SCItemSlot DropItem;

			SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_atIndex);
			assert(pContainer);

			if(!pContainer)
				return;

			assert(!pContainer->IsEmpty( pRecvMsg->m_atPos ));

			SCSlot & rDropSlot = pContainer->GetSlot( pRecvMsg->m_atPos );
			SLOTCODE itemCode=rDropSlot.GetCode();
			pContainer->DeleteSlot(pRecvMsg->m_atPos, &DropItem);

			MouseHandler::Instance()->RemoveItemAtHand();	

			TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
			g_InterfaceManager.GetItemName(itemCode, szName, INTERFACE_STRING_LENGTH);

			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH];

			// PK�� ���� [%s] �������� ����߷Ƚ��ϴ�. 5132
			g_InterfaceManager.GetInterfaceString(eST_PKED_AND_DROP_ITEM , szTemp ,INTERFACE_STRING_LENGTH);
			Sprintf(szMessage, szTemp, szName);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case CG_ITEM_DROP_NAK:
		{
			MSG_CG_ITEM_DROP_NAK * pRecvMsg = (MSG_CG_ITEM_DROP_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			//	CG_ITEM_DROP_NAK
			WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );

#if defined( _DEBUG )
			g_InterfaceManager.ShowMessageBox( _T("CG_ITEM_DROP_NAK"), 10000, &Color, 0 );
#endif
		}
		break;

	case CG_ITEM_COMBINE_ACK:
		{
			MSG_CG_ITEM_COMBINE_ACK * pRecvMsg = (MSG_CG_ITEM_COMBINE_ACK *)pMsg;

			SCSlotContainer * pFromContainer = GetContainer( pRecvMsg->m_fromIndex );
			SCSlotContainer * pToContainer = GetContainer( pRecvMsg->m_toIndex );

			SCSlot & rToSlot = (pToContainer->GetSlot(pRecvMsg->m_toPos));
			SCSlot & rFromSlot = (pFromContainer->GetSlot(pRecvMsg->m_fromPos));

			CODETYPE ItemCode = rToSlot.GetCode();
			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( ItemCode );
			
			WORD wToCount = rToSlot.GetNum();
			WORD wFromCount = rFromSlot.GetNum();
			WORD wCombineCount = wToCount + wFromCount;

			SCItemSlot & rItemToSlot = (SCItemSlot &)rToSlot;

			MouseHandler::Instance()->SetDivideItemOff();
            if (pRecvMsg->combine_quantity_ == 0 && wCombineCount > rItemToSlot.GetMaxNum())
			{
				WORD wRemainCount = wCombineCount - rItemToSlot.GetMaxNum();
				rToSlot.SetNum( rItemToSlot.GetMaxNum() );
				rFromSlot.SetNum( (DURATYPE)wRemainCount );
				
				MouseHandler::Instance()->RollbackItemAtHand();
			}
			else
			{
				SCItemSlot ItemSlot;
                if (pRecvMsg->combine_quantity_ != 0)//�������
                {
                    rToSlot.SetNum(pRecvMsg->combine_quantity_);
                    WORD wRemainCount = wCombineCount - pRecvMsg->combine_quantity_;
                    rFromSlot.SetNum((DURATYPE)wRemainCount);
                }
                else
                {
				    rToSlot.SetNum( (DURATYPE)wCombineCount );
				    pFromContainer->DeleteSlot( pRecvMsg->m_fromPos, &ItemSlot);
                }
				MouseHandler::Instance()->RemoveItemAtHand();
			}

			g_InterfaceManager.UpdateQuestInfo();// ����Ʈ ���� ����

			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, (SCSlot *)&rToSlot);
		}
		break;

	case CG_ITEM_COMBINE_NAK:
		{
			MSG_CG_ITEM_COMBINE_NAK * pRecvMsg = (MSG_CG_ITEM_COMBINE_NAK *)pMsg;

			MouseHandler::Instance()->RollbackItemAtHand();

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

#if defined( _DEBUG )
			//	CG_ITEM_COMBINE_NAK
			WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
			g_InterfaceManager.ShowMessageBox( _T("CG_ITEM_COMBINE_NAK"), 10000, &Color, 0 );
#endif
		}
		break;


	case CG_ITEM_DELETE_CMD:
		{
			MSG_CG_ITEM_DELETE_CMD * pRecvMsg = (MSG_CG_ITEM_DELETE_CMD *)pMsg;

            // �����Ϸ��� �������� PC��ȿ���������ΰ��,
            if( xGetHeroData()->GetPCBangItemData().IsActive() == true )
            {
                if( pRecvMsg->m_atIndex == SI_INVENTORY &&
                    pRecvMsg->m_atPos == xGetHeroData()->GetPCBangItemData().GetItemPosition() )
                {
                    POSTYPE current_equiped_item_position = xGetHeroData()->GetPCBangItemData().GetItemPosition();
                    if( SCItemSlot* pItem = ItemManager::Instance()->GetInventoryItem( current_equiped_item_position ) )
                    {
                        pItem->OnUnEquip();
                    }

                    // ����
                    nsSlot::TOGGLE_ITEMS data;
                    data.clear();
                    xGetHeroData()->SetPCBangItemData(data);
                    g_pHero->RefreshPCBangItemEffect();
                    g_pHero->UpdateItemAttrCalculator();
                    g_pHero->UpdateSkillAttr();
                }
            }

			DeleteItemCmd(pRecvMsg->m_atIndex, pRecvMsg->m_atPos, 1, (eITEM_DELETE_RESULT)pRecvMsg->m_byReason);


		}
		break;

	case CG_ITEM_EXTRAINVENTORY_ADD_CMD:
		{
			MSG_CG_ITEM_EXTRAINVENTORY_ADD_CMD * pRecvMsg = (MSG_CG_ITEM_EXTRAINVENTORY_ADD_CMD *)pMsg;
			
			if (g_pHero)
			{
				g_pHero->ResizeInventoryTotalInfo(pRecvMsg->m_ExtraInventoryTabCount);
			}			

			// 5871 ���������� ���� Ȯ�� �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5871, szMessage, INTERFACE_STRING_LENGTH);

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case CG_ITEM_USE_ACK:
		{
		MSG_CG_ITEM_USE_ACK * pRecvMsg = (MSG_CG_ITEM_USE_ACK *)pMsg;

			// �������� ����ض�.
			switch (pRecvMsg->m_atIndex)
			{
			case SI_INVENTORY:
				{
					MessageforItem(pRecvMsg->m_atPos);
					UseInventoryItem(pRecvMsg->m_atIndex, pRecvMsg->m_atPos);
				}
				break;
			case SI_EQUIPMENT:
				{
				}
				break;
			}					
		}
		break;

	case CG_ITEM_USE_NAK:
		{
			MSG_CG_ITEM_USE_NAK * pRecvMsg = (MSG_CG_ITEM_USE_NAK *)pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode);

#if defined( _DEBUG )
            //	CG_ITEM_USE_NAK
            WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
			g_InterfaceManager.ShowMessageBox( _T("CG_ITEM_USE_NAK"), 10000, &Color, 0 );
#endif
		}
		break;

	case CG_ITEM_USE_MOTION_BRD:
		{
			MSG_CG_ITEM_USE_MOTION_BRD* pRecvMsg = (MSG_CG_ITEM_USE_MOTION_BRD*)pMsg;

			Object* pObj = g_ObjectManager.GetObject(pRecvMsg->m_dwAttackerKey);

			if (pObj && pObj->IsKindOfObject(PLAYER_OBJECT))
			{
				Player* pPlayer = (Player*)pObj;

				BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pRecvMsg->m_ItemCode);
				if (pItemInfo)
				{
					switch (pItemInfo->m_wType)
					{
					case eITEMTYPE_TORCH:
					case eITEMTYPE_POLLEN:
					case eITEMTYPE_FIRECRACKER:
					case eITEMTYPE_MINE:
						{
							PLAYER_ACTION action;
							ZeroMemory(&action, sizeof(action));
							action.ActionID = ACTION_FIRECRACKER;
							action.FIRECRACKER.dwItemCode = pRecvMsg->m_ItemCode;
							action.FIRECRACKER.vTargetPos = pRecvMsg->m_wvMainTargetPos;
							pPlayer->PutAction(&action);
						}
						break;
					}
				}
			}
		}
		break;

	case CG_ITEM_USE_MOTION_NAK:
		{	
			MSG_CG_ITEM_USE_MOTION_NAK * pRecvMsg = (MSG_CG_ITEM_USE_MOTION_NAK *) pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);
		}
		break;


	case CG_ITEM_SELL_ACK:
		{
			MSG_CG_ITEM_SELL_ACK * pRecvMsg = (MSG_CG_ITEM_SELL_ACK *)pMsg;

			SCItemSlot	DeleteSlot;

			SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_atIndex);
			assert(pContainer); 
			
			if(!pContainer)
				return;
            if (pRecvMsg->sell_quantity_ != 0)
            {
                pContainer->DeleteSlot(pRecvMsg->m_atPos, &DeleteSlot);
                DeleteSlot.SetNum((DeleteSlot.GetNum() - pRecvMsg->sell_quantity_));
                pContainer->InsertSlot(pRecvMsg->m_atPos, DeleteSlot);
                DeleteSlot.SetNum(pRecvMsg->sell_quantity_);
            }
            else
            {
			    pContainer->DeleteSlot(pRecvMsg->m_atPos, &DeleteSlot);
            }

			ShopDialog * pShopDlg = 
				(ShopDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP );
			if( pShopDlg )
			{
				// �Ǹ��� ������ �����丮 ��Ͽ� �߰�.
				pShopDlg->PushHistorySlot(DeleteSlot);
			}


			if (0 == DeleteSlot.GetCode())
			{
				// ����־��µ� �����ȾҴٳ�?
				// ��·�ų� �������� �϶�� �ϴ� �׳� �ؾ߰�����
				// �׽�Ʈ������ ���д�.
				assert("CG_ITEM_SELL_ACK");
			}

			if (MouseHandler::Instance()->IsExistItemAtHand())
			{
				MouseHandler::Instance()->RemoveItemAtHand();
			}

			assert(g_pHero);
			if(!g_pHero)
				return;

            g_pHero->SetMoney(pRecvMsg->m_Money);
		}
		break;
	case CG_ITEM_SELL_NAK:
		{
			MSG_CG_ITEM_SELL_NAK * pRecvMsg = (MSG_CG_ITEM_SELL_NAK *)pMsg;

			if (MouseHandler::Instance()->IsExistItemAtHand())
			{
				MouseHandler::Instance()->RollbackItemAtHand();
			}

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
		}
		break;

	case CG_ITEM_DELETE_ACK:
		{
			MSG_CG_ITEM_DELETE_ACK * pRecvMsg = (MSG_CG_ITEM_DELETE_ACK *)pMsg;

			SCSlotContainer * pContainer = GetContainer(SI_INVENTORY);
			if ( pContainer )
			{
				pContainer->DeleteSlot( GetPendingDeleteItemPos(),NULL);
			}			

			MouseHandler::Instance()->UnLockWasteDeleteItem();
		}
		break;

	case CG_ITEM_DELETE_NAK:
		{
			MSG_CG_ITEM_DELETE_NAK * pRecvMsg = (MSG_CG_ITEM_DELETE_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

            MouseHandler::Instance()->UnLockWasteDeleteItem();
		}
		break;

	case CG_ITEM_CHANGE_DURA_CMD:
		{
			MSG_CG_ITEM_CHANGE_DURA_CMD * pRecvMsg = (MSG_CG_ITEM_CHANGE_DURA_CMD *)pMsg;

			SCSlotContainer * pContainer = 
				ItemManager::Instance()->GetContainer( SI_EQUIPMENT );

			if ( pContainer )
			{
				assert( !pContainer->IsEmpty( pRecvMsg->m_EquipPos ) );
				if ( !pContainer->IsEmpty( pRecvMsg->m_EquipPos ) )
				{
					SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_EquipPos );
					assert( rItemSlot.GetCode() == pRecvMsg->m_ItemCode );
					rItemSlot.SetDura( pRecvMsg->m_Dura );
                    g_pHero->GetHeroEquipContainer()->OnChangedEquipItemDurability(rItemSlot);
#ifdef _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD
                    ItemAttrCalculator itemCalc(*(g_pHero->GetPlayerAttribute()), g_pHero->GetEquipmentContainer());
                    itemCalc.EquipAll(!!g_pHero->GetPCBangMode(), false, g_pHero->GetLevel());
                    g_pHero->GetPlayerAttribute()->Update();
#endif //_NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD
				}			

				if (g_pHero) 
				{
					if (0 == pRecvMsg->m_Dura)  
					{
                        // �������� 0�� �Ǿ��� �� ���� ������ ����� �Ѵ�.
                        g_pHero->RefreshAppearRender();
                        GlobalFunc::EquipmentPlayerRefreshAppearRender();

						TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
						g_InterfaceManager.GetItemName(pRecvMsg->m_ItemCode, szName, INTERFACE_STRING_LENGTH);

						TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
						TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

						//5147	%s�� �������� 0�̵Ǿ����ϴ�.
						g_InterfaceManager.GetInterfaceString( 5147 , szTemp ,INTERFACE_STRING_LENGTH);
						Sprintf(szMessage, szTemp , szName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
				}

				EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>
					(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
				if(pEnduranceDlg)
				{
					pEnduranceDlg->UpdateEquipEnduranceState();
				}

			}
		}
		break;

	case CG_ITEM_REPAIR_DURA_ACK:
		{
			MSG_CG_ITEM_REPAIR_DURA_ACK * pRecvMsg = (MSG_CG_ITEM_REPAIR_DURA_ACK *)pMsg;

			if ( pRecvMsg->m_bAllRepair )
			{
				// Equip�� �ִ� ��� �������� �������� Ǯ�� ä�����
				RepairItemAll( SI_EQUIPMENT );

				// 5950	������ �������� ��� �����Ǿ����ϴ�.
				TCHAR szMessage[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString(5950, szMessage, INTERFACE_STRING_LENGTH);
				
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
			}
			else
			{
				// Ư�� �����ۿ� �������� ä�����.
				RepairItem( pRecvMsg->m_SlotIndex, pRecvMsg->m_SlotPos );		

				SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_SlotIndex);
				if ( pContainer )
				{
					SCSlot & rSlot = pContainer->GetSlot( pRecvMsg->m_SlotPos );

					// �޽��� ���					
					TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};	
					
					// 5951	[%s]�������� �����Ǿ����ϴ�.
					g_InterfaceManager.GetInterfaceString(5951, szMessage, INTERFACE_STRING_LENGTH);
					g_InterfaceManager.GetItemName(rSlot.GetCode(), szName, INTERFACE_STRING_LENGTH);					

					Snprintf( szResult, INTERFACE_STRING_LENGTH-1, szMessage, szName);

					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szResult);

					SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
					if(rItemSlot.IsLimited())
					{
						ZeroMemory(szMessage, sizeof(szMessage));
						//	6055	��� ������ ���� �ִ� �������� 2�����Ͽ����ϴ�.
						g_InterfaceManager.GetInterfaceString(6055, szMessage, INTERFACE_STRING_LENGTH);
						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Verify(szMessage);
					}

				}
			}

			if ( g_pHero )
			{
				g_pHero->SetMoney( pRecvMsg->m_Money );
				g_pHero->UpdateItemAttrCalculator();
				g_pHero->UpdateSkillAttr();
                g_pHero->RefreshAppearRender();
			}

            GlobalFunc::EquipmentPlayerRefreshAppearRender();

			EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>
				(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));

			if(pEnduranceDlg)
			{
				pEnduranceDlg->UpdateEquipEnduranceState();
			}
		}
		break;

	case CG_ITEM_REPAIR_DURA_NAK:
		{
			MSG_CG_ITEM_REPAIR_DURA_NAK * pRecvMsg = (MSG_CG_ITEM_REPAIR_DURA_NAK *)pMsg;

			MouseHandler::Instance()->RollbackItemAtHand();				

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);
		}
		break;	


	case CG_ITEM_EXCHANGE_ACK:
		{
			SCItemSlot	DeleteSlot;
			MSG_CG_ITEM_EXCHANGE_ACK * pRecvMsg = (MSG_CG_ITEM_EXCHANGE_ACK *)pMsg;

			if (MouseHandler::Instance()->IsExistItemAtHand())
			{
				MouseHandler::Instance()->RemoveItemAtHand();
			}

			SCSlotContainer * pContainer = GetContainer(m_PendingExchangeContainIdx);
			assert(pContainer); 
			
			if(!pContainer)
				return;

			pContainer->DeleteSlot(m_PendingExchangeItemPos, NULL);

			this->InsertInventoryTotalInfo(PICK, pRecvMsg->m_tagInventory );
		}
		break;

	case CG_ITEM_EXCHANGE_NAK:
		{
			MSG_CG_ITEM_EXCHANGE_NAK * pRecvMsg = (MSG_CG_ITEM_EXCHANGE_NAK *)pMsg;

			if (MouseHandler::Instance()->IsExistItemAtHand())
			{
				MouseHandler::Instance()->RollbackItemAtHand();
			}

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);           
		}
		break;


	case CG_ITEM_BUY_ACK:
		{
			MSG_CG_ITEM_BUY_ACK * pRecvMsg = (MSG_CG_ITEM_BUY_ACK *)pMsg;
			
			InsertInventoryTotalInfo( BUY, pRecvMsg->m_TotalInfo );

			assert(g_pHero);

			if(!g_pHero)
				return;
            if (pRecvMsg->m_ShopType == SHOP_TYPE_OF_ACCUMULATE)
            {
                uiCharacterStatusMan* character_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
                if (character_status_manager != NULL)
                {
                    character_status_manager->set_accumulate_point((DWORD)pRecvMsg->m_Money);
                }
            }
            else // �Ϲ� ����, // Ȥ�� ������(�������)
            {
			    g_pHero->SetMoney(pRecvMsg->m_Money);
            }
		}
		break;

	case CG_ITEM_BUY_NAK:
		{
			MSG_CG_ITEM_BUY_NAK * pRecvMsg = (MSG_CG_ITEM_BUY_NAK *)pMsg;
			
			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();

			//	MSG_CG_ITEM_BUY_NAK
			WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );

#if defined( _DEBUG )
			g_InterfaceManager.ShowMessageBox( _T("MSG_CG_ITEM_BUY_NAK"), 10000, &Color, 0 );
#endif
		}
		break;	


	case CG_ITEM_INVENTORY_PWD_LOCKING_ACK:
	case CG_ITEM_INVENTORY_PWD_LOCKING_NAK:
	case CG_ITEM_INVENTORY_PWD_SETTING_ACK:
	case CG_ITEM_INVENTORY_PWD_SETTING_NAK:
	case CG_ITEM_INVENTORY_PWD_CHECK_ACK:
		g_InterfaceManager.NetworkProc( InterfaceManager::DIALOG_INVENTORY, pMsg);
		break;

	case CG_ITEM_EXTRASTONE_USE_ACK:		
	case CG_ITEM_EXTRASTONE_USE_NAK:
	case CG_ITEM_SOCKET_FILL_SYN:
	case CG_ITEM_SOCKET_FILL_ACK:
	case CG_ITEM_SOCKET_FILL_NAK:
	case CG_ITEM_ACCESSORY_CREATE_ACK:
	case CG_ITEM_ACCESSORY_CREATE_NAK:
		{
			this->OnrecvItemComposite(pMsg);
		}
		break;

	case CG_ITEM_ENCHANT_SUCCESS_ACK:
	case CG_ITEM_ENCHANT_FAILED_ACK:
	case CG_ITEM_ENCHANT_NAK:
		{
			this->OnrecvItemEnchant(pMsg);
		}
		break;

	case CG_ITEM_COMPOSE_SUCCESS_ACK:
	case CG_ITEM_COMPOSE_FAILED_ACK:
	case CG_ITEM_COMPOSE_NAK:
		{

		}
		break;

	case CG_ITEM_CRYSTALIZE_SUCCESS_ACK:	
	case CG_ITEM_CRYSTALIZE_FAILED_ACK:
	case CG_ITEM_CRYSTALIZE_NAK:
		{

		}
		break;

	case CG_ITEM_USE_RANDOM_ITEM_ACK:
		{
			//this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);
			MSG_CG_ITEM_USE_RANDOM_ITEM_ACK * pRecvMsg = (MSG_CG_ITEM_USE_RANDOM_ITEM_ACK *)pMsg;

			// Ư�� ��ġ�� ���� �������� �Һ��Ѵ�.
			POSTYPE pTargetPos;
			MouseHandler::Instance()->GetRandomItemPOS(pTargetPos);
			ItemManager::Instance()->DeleteItem( SI_INVENTORY, pTargetPos, 1, COMPOSE_USE );

			ItemManager::Instance()->InsertInventoryTotalInfo( CREATE, pRecvMsg->m_ItemInfo );

			//�κ��丮�� ��Ǯ��
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (inventory_manager != NULL)
            {
                inventory_manager->UnlockInventoryUI();
            }

		}
		break;
	case CG_ITEM_USE_RANDOM_ITEM_NAK:
		{
			//this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);
			MSG_CG_ITEM_USE_RANDOM_ITEM_NAK * pRecvMsg = (MSG_CG_ITEM_USE_RANDOM_ITEM_NAK *)pMsg;
			ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
			//�κ��丮�� ��Ǯ��
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (inventory_manager != NULL)
            {
                inventory_manager->UnlockInventoryUI();
            }
		}
		break;
    case CG_ITEM_LOSE_ACK:
		{
			MSG_CG_ITEM_LOSE_ACK * pRecvMsg = (MSG_CG_ITEM_LOSE_ACK *)pMsg;

			DeleteInventoryTotalInfo(QUEST_DELETE, SI_INVENTORY, pRecvMsg->m_ItemInfo);

		}
		break;

	case CG_ITEM_OBTAIN_ITEM_CMD:
		{
			MSG_CG_ITEM_OBTAIN_ITEM_CMD * pRecvMsg = (MSG_CG_ITEM_OBTAIN_ITEM_CMD *)pMsg;
			InsertInventoryTotalInfo(REWARD, pRecvMsg->m_tagInventory);

		}
		break;
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    case CG_ITEM_OBTAIN_ITEM_NAK:
        {
            MSG_CG_ITEM_OBTAIN_ITEM_NAK * pRecvMsg = (MSG_CG_ITEM_OBTAIN_ITEM_NAK *)pMsg;
            TCHAR error_string[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetStringRewardError( pRecvMsg->m_byErrorCode, error_string, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, error_string);			
        }
        break;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
	// ������ �Ҹ�
	case CG_ITEM_FREE_OWNERSHIP_BRD:
		{
			MSG_CG_ITEM_FREE_OWNERSHIP_BRD * pRecvMsg = (MSG_CG_ITEM_FREE_OWNERSHIP_BRD *)pMsg;

			Item *pItem = (Item *)g_ObjectManager.GetObject(pRecvMsg->m_dwObjectKey);
			if (pItem)
			{
				pItem->SetOwner(0);
			}

		}
		break;

	case CG_ITEM_BLOCKC_CMD:
		{
			MSG_CG_ITEM_BLOCKC_CMD * pRecvMsg = (MSG_CG_ITEM_BLOCKC_CMD *)pMsg;

			SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_SlotIdx);
			assert(pContainer);
			if ( pContainer )
			{
				// �������� ��ġ �̸鼭 
				// �ش� ��ġ�� �������� �־����.
				if ( pRecvMsg->m_SlotPos >= 0 && 
					 pRecvMsg->m_SlotPos < pContainer->GetMaxSlotNum() && 
					 !pContainer->IsEmpty( pRecvMsg->m_SlotPos ) )
				{
					SCSlot & rSlot = pContainer->GetSlot( pRecvMsg->m_SlotPos );

					assert( ST_ITEM == rSlot.GetSlotType() );
					if ( ST_ITEM == rSlot.GetSlotType() )
					{
						SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;	
						rItemSlot.SetBlocked(TRUE);
						
						// �ø��� �ߺ����� ���� �������� ����Ͻ� �� �����ϴ�. ����� �Ű��� �ֽñ� �ٶ��ϴ�. 5024
						g_InterfaceManager.GetInterfaceString(eST_ITEM_SERIAL_DUPLICATE , szMessage ,INTERFACE_STRING_LENGTH);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Verify(szMessage);

					}
				}
			}
		}
		break;


	case CG_ITEM_CHANGE_ITEM_INFO_CMD:
		{
			MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD * pRecvMsg = (MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD *)pMsg;

			SCSlotContainer * pAtContainer = GetContainer(pRecvMsg->m_SlotIndex);
			if ( pAtContainer )
			{
				assert( !pAtContainer->IsEmpty( pRecvMsg->m_SlotPos ) );
				assert( pRecvMsg->m_SlotPos < pAtContainer->GetMaxSlotNum() );

				if (pRecvMsg->m_SlotPos >= pAtContainer->GetMaxSlotNum())
				{
					GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, 
						_T("�ش罽�� ��ġ�� �߸��� ��ġ�� ���ŵǷ��Ѵ�. �ش� �޽����� ���� ���α׷����� �˷��ֽñ� �ٶ��ϴ�.") );
					return;
				}
				
				SCItemSlot ItemSlot(pRecvMsg->m_ItemStream);	

				if (g_pHero)
				{
					g_pHero->OnUseItemWithCoolTime(ItemSlot.GetCode());
				}			

				pAtContainer->DeleteSlot(pRecvMsg->m_SlotPos, NULL); // ���������� �������ش�. ( ������ ���ҽ������� ���� �����ش� )
				pAtContainer->InsertSlot(pRecvMsg->m_SlotPos, ItemSlot);


				if( ItemSlot.GetItemInfo() )
				{
                    BYTE charge_subtype = ItemSlot.GetItemRuleInfo().charge_sub_type;
					TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
					if (charge_subtype == eCHARGE_SUB_COORDINATE)
					{
						// 5936	���� ��ǥ�� ���������� ���� �Ͽ����ϴ�
						g_InterfaceManager.GetInterfaceString(5936, szText, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szText);
					}
                    else if(ItemManager::CanPeriodExtend(&ItemSlot) == true) //<- �Ⱓ���� ���� ������ �ϰ��
					{
#ifdef __NA_001358_ENCHANT_UNIFICATION
						// no operation
#else //__NA_001358_ENCHANT_UNIFICATION						
						EnchantDialog * pDlg = (EnchantDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ENCHANT );

						if(pDlg)
						{
							//��Ű�������� ��� ��þ�����ø޽��� ����
							if(pDlg->IsVisibleWindow() && pDlg->IsPendingEnchant(pRecvMsg->m_SlotPos))
							{
								break;
							}
						}
#endif //__NA_001358_ENCHANT_UNIFICATION

						TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

                        AskSocketIdentifyDialog* socket_identify_dialog =
                            static_cast<AskSocketIdentifyDialog*>(GET_CAST_DIALOG(AskSocketIdentifyDialog, IM_ASSIST_MANAGER::SOCKET_IDENTIFY_DIALOG));
                        bool is_socket_identify = false;
                        if (socket_identify_dialog)
                        {
                            is_socket_identify = socket_identify_dialog->is_socket_identify();
                            socket_identify_dialog->set_is_socket_identify(false);
                        }
                        if (socket_identify_dialog == false)
                        {
						    if(ItemSlot.IsExpiredWaitTime())
						    {
							    //6867	%s �������� �Ⱓ�� ����Ǿ����ϴ�.
							    g_InterfaceManager.GetInterfaceString( 6867,szTemp);
						    }
						    else
						    {
							    if( ItemSlot.IsExpiredTime() )
							    {
								    //6867	%s �������� �Ⱓ�� ����Ǿ����ϴ�.
								    g_InterfaceManager.GetInterfaceString( 6867,szTemp);

							    }
							    else
							    {
								    //6868	%s �������� �Ⱓ������ �Ϸ�Ǿ����ϴ�.
								    g_InterfaceManager.GetInterfaceString( 6868,szTemp);
							    }
						    }
                        }

						TCHAR szItemName[INTERFACE_STRING_LENGTH] ={0,};
						g_InterfaceManager.GetItemName(ItemSlot.GetCode(), szItemName);
						Snprintf(szText, INTERFACE_STRING_LENGTH-1, szTemp , szItemName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szText);
					}
					else if (charge_subtype == eCHARGE_SUB_COORDINATE_EX)
					{
						GlobalFunc::SetWayPointItemPending(false);
					}
				}
				
			}
		}
		break;


	case CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK:
		{
			MSG_CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK * pRecvMsg = (MSG_CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK *) pMsg;

			SCSlotContainer * pAtContainer = GetContainer(SI_INVENTORY);
			if( !pAtContainer )
				return;

			SCItemSlot & rChargeItemSlot	= (SCItemSlot &)pAtContainer->GetSlot( pRecvMsg->m_ChargeIntemPos );


			// ����� ��þƮ ������ ī��Ʈ �ϳ�������.
			SCItemSlot DeleteChargeItemSlot;
			if (rChargeItemSlot.IsOverlap())
			{
				if (rChargeItemSlot.GetNum() > 1	)
				{
					rChargeItemSlot.SetNum( rChargeItemSlot.GetNum()-1 );
					pAtContainer->UpdateSlot(pRecvMsg->m_ChargeIntemPos, rChargeItemSlot );
				}
				else
				{
					pAtContainer->DeleteSlot(pRecvMsg->m_ChargeIntemPos, &DeleteChargeItemSlot);
					assert(eITEMTYPE_SOCKETEXTRACT == DeleteChargeItemSlot.GetItemInfo()->m_wType && "����(����)�������̾ƴѵ� �����ٴ�!!!!");
				}	
			}
			else
			{
				pAtContainer->DeleteSlot(pRecvMsg->m_ChargeIntemPos, &DeleteChargeItemSlot);
				assert(eITEMTYPE_SOCKETEXTRACT == DeleteChargeItemSlot.GetItemInfo()->m_wType && "����(����)�������̾ƴѵ� �����ٴ�!!!!");
			}
			
			// ������ ä�� �ֱ�
			SCItemSlot InsertItemSlot(pRecvMsg->m_TargetItemStream);
			pAtContainer->DeleteSlot(pRecvMsg->m_TargetPos, NULL);
			pAtContainer->InsertSlot(pRecvMsg->m_TargetPos, InsertItemSlot);
		
            DWORD string_code = 0;
            TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR error_message[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

            SCItemSlot &item_slot = (SCItemSlot &)pAtContainer->GetSlot(pRecvMsg->m_TargetPos);
            g_InterfaceManager.GetItemName(item_slot.GetCode(), item_name);

            switch(MouseHandler::Instance()->GetSocketRemoveType())
            {
            case kZardGrade1:
                string_code = 5879; //[%s]�� �ϱ� ���� �ɼ��� �����Ͽ����ϴ�.
                break;
            case kZardGrade2:
                string_code = 5913; //[%s]�� �߱� ���� �ɼ��� �����Ͽ����ϴ�.
                break;
            case kZardGrade3:
                string_code = 5880; //[%s]�� ��� ���� �ɼ��� �����Ͽ����ϴ�.
                break;
            case kZardGrade4:
                string_code = 5914; //[%s]�� �ֻ�� ���� �ɼ��� �����Ͽ����ϴ�.
                break;
            }

            if (string_code != 0)
            {
                g_InterfaceManager.GetInterfaceString(string_code, temp_string, INTERFACE_STRING_LENGTH);
                Sprintf(error_message, temp_string, item_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, error_message);
            }
            else
            {
                assert(!"ItemManager_PacketProc.cpp: not found zard_grade (��� ����� �߸� �Ǿ����ϴ�)");
            }

			MouseHandler::Instance()->SetTryRemoveSocket(FALSE);
			MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_REMOVE );
		}
		break;

	case CG_ITEM_SOCKET_EXTRACT_NAK:
		{
			MSG_CG_ITEM_SOCKET_EXTRACT_NAK * pRecvMsg = (MSG_CG_ITEM_SOCKET_EXTRACT_NAK *) pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);  
			MouseHandler::Instance()->SetTryRemoveSocket(FALSE);
			MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_REMOVE );
		}
		break;

	case CG_ITEM_INIT_STAT_ACK:
		{
			MSG_CG_ITEM_INIT_STAT_ACK * pRecvMsg = (MSG_CG_ITEM_INIT_STAT_ACK *)pMsg;

			DeleteItem( 
				SI_INVENTORY, 
				pRecvMsg->m_ItemSlotPos, 
				pRecvMsg->m_wDecreaseValue, 
				DELETE_STAT_INIT_ITEM );
		}
		break;

	case CG_ITEM_INIT_STAT_NAK:
		{
			MSG_CG_ITEM_INIT_STAT_NAK * pRecvMsg = (MSG_CG_ITEM_INIT_STAT_NAK *)pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode); 					
		}
		break;

	case CG_ITEM_STAT_ONE_CHANGE_CMD:
		{
			MSG_CG_ITEM_STAT_ONE_CHANGE_CMD * pRecvMsg = (MSG_CG_ITEM_STAT_ONE_CHANGE_CMD *)pMsg;

			if (!g_pHero)
				return;

			eATTR_TYPE attrType = (eATTR_TYPE)pRecvMsg->m_AttrType;	

			assert( attrType < eATTR_MAX && attrType >= 0 );

			if(eATTR_TYPE_INVALID != attrType)
			{
				(*g_pHero->GetPlayerAttribute())[attrType][eATTR_KIND_BASE] = pRecvMsg->m_dwValue;
				g_pHero->GetPlayerAttribute()->Update();
			}



			BASE_PLAYERINFO*  pPlayInfo = g_pHero->GetCharInfo();
			pPlayInfo->m_iRemainStat = pRecvMsg->m_iRemainStat;

			// update�� ������ ���´�. 
			g_pHero->GetPlayerAttribute()->GetCharInfo( pPlayInfo );

			g_pHero->UpdateItemAttrCalculator();
			g_pHero->UpdateSkillAttr();
		}
		break;

	case CG_ITEM_STAT_ALL_CHANGE_CMD:
		{
			MSG_CG_ITEM_STAT_ALL_CHANGE_CMD * pRecvMsg = (MSG_CG_ITEM_STAT_ALL_CHANGE_CMD *)pMsg;
			if (!g_pHero)
				return;

			(*g_pHero->GetPlayerAttribute())[eATTR_STR][eATTR_KIND_BASE] = pRecvMsg->m_sStrength;					
			(*g_pHero->GetPlayerAttribute())[eATTR_DEX][eATTR_KIND_BASE] = pRecvMsg->m_sDexterity;
			(*g_pHero->GetPlayerAttribute())[eATTR_VIT][eATTR_KIND_BASE] = pRecvMsg->m_sVitality;					
			(*g_pHero->GetPlayerAttribute())[eATTR_INT][eATTR_KIND_BASE] = pRecvMsg->m_sInteligence;
			(*g_pHero->GetPlayerAttribute())[eATTR_SPR][eATTR_KIND_BASE] = pRecvMsg->m_sSpirit;					
			(*g_pHero->GetPlayerAttribute())[eATTR_EXPERTY1][eATTR_KIND_BASE] = pRecvMsg->m_sSkillStat1;
			(*g_pHero->GetPlayerAttribute())[eATTR_EXPERTY2][eATTR_KIND_BASE] = pRecvMsg->m_sSkillStat2;					
			BASE_PLAYERINFO*  pPlayInfo = g_pHero->GetCharInfo();
			pPlayInfo->m_iRemainStat = pRecvMsg->m_iRemainStat;
				
			g_pHero->GetPlayerAttribute()->Update();

			// update�� ������ ���´�. 
			g_pHero->GetPlayerAttribute()->GetCharInfo( pPlayInfo );

			g_pHero->UpdateItemAttrCalculator();
			g_pHero->UpdateSkillAttr();

            uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

            g_pHero->RefreshAppearRender();
            GlobalFunc::EquipmentPlayerRefreshAppearRender();

			// 5886	���������� ��� ������ �ʱ�ȭ �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString( 5886, szMessage, INTERFACE_STRING_LENGTH );

			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage);
		}
		break;


#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	case CG_ITEM_EXCHANGE_REWARD_ITEM_ACK:
		{
			MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_ACK * pRecvMsg = (MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_ACK*) pMsg;
			InsertInventoryTotalInfo(REWARD, pRecvMsg->m_ItemInfo);
		}
		break;

	case CG_ITEM_EXCHANGE_REWARD_ITEM_NAK:
		{
			MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_NAK * pRecvMsg = ( MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_NAK* )pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode);
		}
		break;
#endif

	case CG_ITEM_HAMMER_OF_ALCHEMIST_ACK:
		{
			MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_ACK * pRecvMsg = (MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_ACK *) pMsg;
			
			UseInventoryItem(SI_INVENTORY, pRecvMsg->m_ChargeItemPos);
			MouseHandler::Instance()->SetTryUseChageItem(FALSE);
			SCItemSlot ResultItme(pRecvMsg->m_TargetItemStream);
			// ������ �������� �κ��丮�� �־��ش�.
			SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			if( ! pFromContainer )
				break;
			pFromContainer->DeleteSlot(pRecvMsg->m_TargetItemPos , NULL);
			pFromContainer->InsertSlot(pRecvMsg->m_TargetItemPos , ResultItme);

		}
		break;

	case CG_ITEM_HAMMER_OF_ALCHEMIST_NAK:
		{
			MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_NAK * pRecvMsg = (MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_NAK *) pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
			MouseHandler::Instance()->SetTryUseChageItem(FALSE);
		}
		break;



	case CG_ITEM_EXTEND_CASH_SLOT_CMD:
		{
			MSG_CG_ITEM_EXTEND_CASH_SLOT_CMD * pRecvMsg = (MSG_CG_ITEM_EXTEND_CASH_SLOT_CMD *) pMsg;

			HeroEquipmentContainer * pEquipContainer = (HeroEquipmentContainer *)ItemManager::Instance()->GetItemContainerDialog(SI_EQUIPMENT);
			if( pEquipContainer )
			{
				pEquipContainer->ChargeItemExtendSlot(pRecvMsg->m_ExtraEquipCashSlotCount);
			}

			// ���� ������ ������ Ȯ��Ǿ����ϴ� 5871
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 5871 ) );
		}
		break;

	case CG_ITEM_SETITEM_CHANGE_BRD:
		{
			//�ϴ°� ����.
		}
		break;



	case CG_ITEM_VALIDATION_OF_REIHALT_ACK:
		{
			uiFateItemSystem* pFateItemSystem=  GET_CAST_DIALOG( uiFateItemSystem, IM_FATEITEM_SYSTEM::FATEITEM_DLG );

			if( pFateItemSystem )
			{
				MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK * pRecvMsg = ( MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK *) pMsg;
				//6327	��Ű ������ �ɷ�ġ�� �������Դϴ�,
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6327));
				pFateItemSystem->SetTargetItemPos(pRecvMsg->m_TargetItemPos);
				
				//����: �ٷ� �κ��丮�� �־��ִ°��� �ƴ϶�, �׺� UI�� �Ϸ�� �Ŀ� �κ��丮�� �������� �־��ٰ���.
				SCItemSlot ResultItme(pRecvMsg->m_TargetItemStream);
				pFateItemSystem->SetResultItem(ResultItme);
				pFateItemSystem->SetFateItemProcessState(eFIS_RESULT_SUCCESS);
			}
		}
		break;

	case CG_ITEM_VALIDATION_OF_REIHALT_NAK:
		{
			MSG_CG_ITEM_VALIDATION_OF_REIHALT_NAK * pRecvMsg = ( MSG_CG_ITEM_VALIDATION_OF_REIHALT_NAK *) pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			//6330	��Ű������ ������ �����Ͽ����ϴ�. �ٽ� �õ��� �ֽʽÿ�.
			GlobalFunc::OutputMsg_SysMsg(g_InterfaceManager.GetInterfaceString(6330));
			//uiFateItemSystem * pFateItemSystem = ( uiFateItemSystem *) g_InterfaceManager.GetUserInterface(UI_FATE_ITEM_SYSTEM);
			uiFateItemSystem* pFateItemSystem=  GET_CAST_DIALOG( uiFateItemSystem, IM_FATEITEM_SYSTEM::FATEITEM_DLG );

			if( pFateItemSystem )
			{
				pFateItemSystem->SetTargetItemLock(FALSE);
				pFateItemSystem->SetFateItemProcessState(eFIS_END);
				pFateItemSystem->InitBaseAtributeCaption();
			}
		}
		break;

	case CG_ITEM_REPURCHASE_ACK:
		{
			MSG_CG_ITEM_REPURCHASE_ACK * pRecvMsg = (MSG_CG_ITEM_REPURCHASE_ACK *) pMsg;
			assert(g_pHero);
			if( !g_pHero)
				return;
			InsertInventoryTotalInfo( BUY, pRecvMsg->m_TotalInfo );
			g_pHero->SetMoney(pRecvMsg->m_Money);

			ShopDialog * pShopDlg = (ShopDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP );
			if( pShopDlg )
			{
				pShopDlg->PopHistorySlot();
				pShopDlg->SetPendingHistorySlot(-1);
			}
		}
		break;

	case CG_ITEM_REPURCHASE_NAK:
		{
			MSG_CG_ITEM_REPURCHASE_NAK * pRecvMsg = (MSG_CG_ITEM_REPURCHASE_NAK *) pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode);
			ShopDialog * pShopDlg = (ShopDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP );
			if( pShopDlg )
			{
				pShopDlg->SetPendingHistorySlot(-1);
			}

		}
		break;



	case CG_ITEM_EQUIP_ETHER_DEVICE_ACK:
		{
			MSG_CG_ITEM_EQUIP_ETHER_DEVICE_ACK * pRecvMsg = (MSG_CG_ITEM_EQUIP_ETHER_DEVICE_ACK *) pMsg;
			if( pRecvMsg->m_Result == RC_ITEM_SUCCESS )
			{
				SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
				if( pContainer )
				{
					//���⿡ ���׸� ��밡���ϵ��� ����
					SCItemSlot & rItemSlot = (SCItemSlot &) pContainer->GetSlot(pRecvMsg->m_WeaponItemPos);
					rItemSlot.SetEquipEtherDevice(TRUE);

					// ���׸� ���� ������ �����ϱ�
					SCItemSlot & rDeviceItemSlot = (SCItemSlot &) pContainer->GetSlot(pRecvMsg->m_DeviceItemPos);
					SCItemSlot DeleteSlot;
					if (rDeviceItemSlot.IsOverlap())
					{
						if (rDeviceItemSlot.GetNum() > 1	)
						{
							rDeviceItemSlot.SetNum( rDeviceItemSlot.GetNum()-1 );
							pContainer->UpdateSlot(pRecvMsg->m_DeviceItemPos, rDeviceItemSlot );
						}
						else
						{
							pContainer->DeleteSlot(pRecvMsg->m_DeviceItemPos, &DeleteSlot);
							assert(eITEMTYPE_ETHER_DEVICE == DeleteSlot.GetItemInfo()->m_wType);
						}	
					}
					else
					{
						pContainer->DeleteSlot(pRecvMsg->m_DeviceItemPos, &DeleteSlot);
						assert(eITEMTYPE_ETHER_DEVICE == DeleteSlot.GetItemInfo()->m_wType);
					}

					MouseHandler::Instance()->SetMouseState( MouseHandler::NONE );
					//���� �޼��� �Ѹ���
					TCHAR szWeaponName[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szWeaponName, INTERFACE_STRING_LENGTH -1 );

					//6129	%s�� ���׸� ������ġ�� �����Ͽ����ϴ�.
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6129) , szWeaponName);
				}
			}
			else
			{
				NoticeReasonItemErrorCode(pRecvMsg->m_Result);
			}
		}
		break;

	case CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK:
		{
			MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK * pRecvMsg = (MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK *) pMsg;

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK:%d"),pRecvMsg->m_ResultCode);
#endif 
			if( pRecvMsg->m_ResultCode == RC_ITEM_SUCCESS )
			{
				SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
				
				if( pContainer )
				{
					//6159 %s �������� Ȱ��ȭ�Ǿ����ϴ�.	
					g_InterfaceManager.GetInterfaceString(6159,szText);
					SCItemSlot & rItemSlot	= (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_FireUpItemPos);
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szText,g_InterfaceManager.GetInterfaceString(rItemSlot.GetItemInfo()->m_NCode) );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
			}
			else
			{
				NoticeReasonItemErrorCode(pRecvMsg->m_ResultCode);	
			}
		}
		break;


#ifdef _NA_004035_20120227_COSTUME_ITEM
    case CG_ITEM_USE_COSTUME_ACK:
        {
            MSG_CG_ITEM_USE_COSTUME_ACK* message = (MSG_CG_ITEM_USE_COSTUME_ACK*)pMsg;
            if( message->is_equipped_costume_ == true )
            {
                if( SCItemSlot* item = GetInventoryItem( message->costume_item_pos_ ) )
                {
                    item->OnEquip();

                    g_pHero->SetActivedCostumeItemSlotIndex( message->costume_item_pos_ );
                    g_pHero->SetCostumeItemCode( item->GetCode() );
                    g_pHero->RefreshAppearRender(TRUE);

                    g_pHero->UpdateSkillAttr();


                    if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY) )
                    {
                        pPlayer->SetCostumeItemCode( item->GetCode() );
                        pPlayer->RefreshAppearRender(TRUE);
                    }

                }
            }
            else
            {
                if( g_pHero->GetCostumeItemCode() != 0 )
                {
                    if( SCItemSlot* item = GetInventoryItem( g_pHero->GetActivedCostumeItemSlotIndex() ) )
                    {
                        item->OnUnEquip();
                    }
                }
                
                g_pHero->SetActivedCostumeItemSlotIndex( INVALID_SLOT_POS );
                g_pHero->SetCostumeItemCode(0);
                g_pHero->RefreshAppearRender(TRUE);

                g_pHero->UpdateSkillAttr();

                if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY) )
                {
                    pPlayer->SetCostumeItemCode(0);
                    pPlayer->RefreshAppearRender(TRUE);
                }
            }
        }
        break;

    case CG_ITEM_USE_COSTUME_BRD:
        {
            MSG_CG_ITEM_USE_COSTUME_BRD* message = (MSG_CG_ITEM_USE_COSTUME_BRD*)pMsg;
            if( Character* character = g_ObjectManager.GetCharacter( message->object_key_) )
            {
                character->SetCostumeItemCode( message->costume_item_code_ );
                character->RefreshAppearRender();
            }
        }
        break;
#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	case CG_ITEM_USE_DECORATE_COSTUME_ACK:
		{
			MSG_CG_ITEM_USE_DECORATE_COSTUME_ACK* message = (MSG_CG_ITEM_USE_DECORATE_COSTUME_ACK*)pMsg;
			if( message->is_equipped_costume_ == true )
			{
				if( SCItemSlot* item = GetInventoryItem( message->deco_costume_item_pos_ ) )
				{
					item->OnEquip();

					g_pHero->SetActivedDecorateCostumeItemSlotIndex( message->deco_costume_item_pos_ );
					g_pHero->SetDecorateCostumeItemCode( item->GetCode() );
					g_pHero->RefreshAppearRender(TRUE);

					g_pHero->UpdateSkillAttr();


					if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY) )
					{
						pPlayer->SetDecorateCostumeItemCode( item->GetCode() );
						pPlayer->RefreshAppearRender(TRUE);
					}

				}
			}
			else
			{
				if( g_pHero->GetDecorateCostumeItemCode() != 0 )
				{
					if( SCItemSlot* item = GetInventoryItem( g_pHero->GetActivedDecorateCostumeItemSlotIndex() ) )
					{
						item->OnUnEquip();
					}
				}

				g_pHero->SetActivedDecorateCostumeItemSlotIndex( INVALID_SLOT_POS );
				g_pHero->SetDecorateCostumeItemCode(0);
				g_pHero->RefreshAppearRender(TRUE);

				g_pHero->UpdateSkillAttr();

				if( Player* pPlayer = (Player*)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY) )
				{
					pPlayer->SetDecorateCostumeItemCode(0);
					pPlayer->RefreshAppearRender(TRUE);
				}
			}
		}
		break;

	case CG_ITEM_USE_DECORATE_COSTUME_BRD:
		{
			MSG_CG_ITEM_USE_DECORATE_COSTUME_BRD* message = (MSG_CG_ITEM_USE_DECORATE_COSTUME_BRD*)pMsg;
			if( Character* character = g_ObjectManager.GetCharacter( message->object_key_) )
			{
				character->SetDecorateCostumeItemCode( message->deco_costume_item_code_ );
				character->RefreshAppearRender();
			}
		}
		break;
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

	case CG_ITEM_USE_PET_ITEM_ACK:
		{
			MSG_CG_ITEM_USE_PET_ITEM_ACK * pRecvMsg = (MSG_CG_ITEM_USE_PET_ITEM_ACK *) pMsg;

			//�ش�޽����ڵ�
			int iResultCode = 0;
		
			if(!g_pHero) break;
			switch(pRecvMsg->m_Result)
			{
				//6393	���� ��ȯ �Ͽ����ϴ�.
			case RC_ITEM_SUMMON_SUMMON_SUCCESS:
				{
					SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
					
					if(!pContainer)break;
					
					SCItemSlot & rItemSlot =  (SCItemSlot &)pContainer->GetSlot(pRecvMsg->m_PetItemPos);
									
					//��Ⱓ��
					rItemSlot.StartQuantity();
					g_pHero->CreatePetSummon(rItemSlot.GetImageItemInfo()->m_wSkillCode); //����� 
					g_pHero->UpdatePetInfo(pRecvMsg->m_PetItemPos);					 //�������Ʈ	
					iResultCode = 6393;
                    rItemSlot.SetUserBlocked(TRUE);
                    
				}break;

				//6394 ���� ����
			case RC_ITEM_SUMMON_RETURN_SUCCESS:
				{
					g_pHero->DeletePetSummon();//���ȯ ����
					g_pHero->UpdateSkillAttr();//��ɼ� ����
					iResultCode = 6394;

					//��Ⱓ��
					SCItemSlot & rItemSlot =  (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->GetPetItemPos());
					rItemSlot.EndQuantity();
                    rItemSlot.SetUserBlocked(FALSE);
				
					//�� �ڵ���ȯ(��ȯ��Ŷ����)
					if(g_pHero->IsPetChange())
					{
						g_pHero->PetChangeOFF();
						MSG_CG_ITEM_USE_PET_ITEM_SYN SendPacket; 
						SendPacket.m_PetItemPos = g_pHero->PetChangePos();
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					}

				}break;

				//6395	���� ��ȯ ���Դϴ�.���� �� ��� �Ͻñ� �ٶ��ϴ�.
			case RC_ITEM_SUMMON_ALREADY_SUMMONED:			iResultCode = 6395;break;				
				//6396	�� �������� �ƴϿ��� ��� �� �� �����ϴ�.
			case RC_ITEM_SUMMON_NOT_PET_ITEM:				iResultCode = 6396;break;						
				//?? ��ȯ�� ���� ���µ� RETURN �Ϸ��� ��
			case RC_ITEM_SUMMON_NOT_SUMMONED_PET:			iResultCode = 6397;break;				
				//?? ��ȯ�� ���̶� �����Ϸ��� �������� ���� �ٸ�.
			case RC_ITEM_SUMMON_MISMATCH_PET_ITEM:			iResultCode = 6395;break;				
				//6391	�������� �����Ͽ� ���� ��ȯ�Ҽ������ϴ�.
			case RC_ITEM_CAN_NOT_SUMMON_FULLNESS_ZERO:		iResultCode = 6391;break;		
				//6397	��ȯ�� ���� ����
			case RC_ITEM_NOT_SUMMONED_PET:					iResultCode = 6397;break;				

			}

			if(iResultCode!=0)
			{
				g_InterfaceManager.GetInterfaceString(iResultCode,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}

		}
		break;
 
	case CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK:
		{
			MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK* pRecvMsg = (MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK*)pMsg;

			uiPetNameSetting* pPetNameDlg = GET_CAST_DIALOG(uiPetNameSetting, IM_PET_MANAGER::PET_NAME_DLG);
			
			if (!pPetNameDlg) 
            {
                break;
            }

			if (pRecvMsg->m_ResultCode == RC_ITEM_SUCCESS)
			{
				pPetNameDlg->SetPetName();
			}
			else
			{
				pPetNameDlg->ShowInterface(FALSE);
				NoticeReasonItemErrorCode(pRecvMsg->m_ResultCode);
			}
		}
		break;

 

	case CG_ITEM_USE_PANDORA_ITEM_ACK:
		{
			if(g_pHero == NULL)
            {
                break;
            }

			MSG_CG_ITEM_USE_PANDORA_ITEM_ACK * pRecvMsg = (MSG_CG_ITEM_USE_PANDORA_ITEM_ACK*) pMsg;
			
			LotteryInventoryDialog* pPandoraDlg=  GET_CAST_DIALOG( LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG );

			if(!pPandoraDlg) return;
			
			g_InterfaceManager.UnLockDlgs();

			if(pRecvMsg->m_Identity)
			{
				//�ǵ�������ۼ���
				pPandoraDlg->ShowInterface(TRUE);
				pPandoraDlg->MoveCenter();
				pPandoraDlg->CreatePandoraSlot(pRecvMsg->m_PandoraItemStream);
				g_InterfaceManager.LockDlgs(IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG,TRUE);
				pPandoraDlg->SetPandoraBoxName();
			}
			else
			{
				pPandoraDlg->SetLotteryOpenStatus(FALSE);
			}
			
		}
		break;  
	case CG_ITEM_MOVE_PANDORA_ITEM_ACK:
		{
			if (!g_pHero) 
            {
                break;
            }

			MSG_CG_ITEM_MOVE_PANDORA_ITEM_ACK* pRecvMsg = (MSG_CG_ITEM_MOVE_PANDORA_ITEM_ACK*) pMsg;
		
			switch (pRecvMsg->m_Result)
			{
			case RC_ITEM_LOTTO_CONCRETIZE_SUCCESS:
				{
					LotteryInventoryDialog* pPandoraDlg = GET_CAST_DIALOG(LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG);
					ItemManager::Instance()->InsertInventoryTotalInfo(PICK, pRecvMsg->m_InventoryInfo);

					if (pPandoraDlg == NULL)
                    {
                        break;
                    }

					//�ǵ���������� �缳���Ѵ�.
					if (pRecvMsg->m_Remove)
					{	
						SCSlotContainer* pContainer = GetContainer(SI_INVENTORY);
						if (pContainer)
						{		
							pPandoraDlg->SendItemCloseSync();
							pContainer->DeleteSlot(pPandoraDlg->GetInvenPos(), NULL);
						}

						g_InterfaceManager.UnLockDlgs();
						pPandoraDlg->ShowInterface(FALSE);
					}
					else
					{
						pPandoraDlg->CreatePandoraSlot(pRecvMsg->m_PandoraItemStream);				
					}

				}
				break;

			default:
				{
					//6133	������ �̵��� �����Ͽ����ϴ�.
					g_InterfaceManager.GetInterfaceString(6133,szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}

			}
		}
		break;
	case CG_ITEM_MOVE_PANDORA_ITEM_NAK:
		{
			//�ǵ��� ������ Move Nak!
			MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK* pRecvMsg = (MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK*) pMsg;
			
            g_InterfaceManager.UnLockDlgs();

            if (pRecvMsg->m_Result == RC_ITEM_HAVENOTSPACE)
            {
                NoticeReasonItemErrorCode(pRecvMsg->m_Result);
            }
            else
            {
                //6133	������ �̵��� �����Ͽ����ϴ�.
                g_InterfaceManager.GetInterfaceString(6133,szMessage);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
		}
		break;

	case CG_ITEM_USE_PANDORA_ITEM_NAK:
		{
			//�ǵ��� ������ Use Nak!
			MSG_CG_ITEM_USE_PANDORA_ITEM_NAK * pRecvMsg = (MSG_CG_ITEM_USE_PANDORA_ITEM_NAK*) pMsg;

			if(pRecvMsg->m_Result!=0)
			{
                uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
                if (inventory_manager != NULL)
                {
                    inventory_manager->UnlockInventoryUI();
                }

				//6132	�ǵ����� ���� ���⸦ �����Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString(6132,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}

		}
		break;
	//============================================================
	// ���� FIX�� ��Ŷ����
	//============================================================
	case CG_ITEM_PANDORABOX_CLOSE_ACK:
		{
			//ó���Ұ� ����
		}
		break;

	case CG_ITEM_PANDORABOX_CLOSE_NAK:
		{
			MSG_CG_ITEM_PANDORABOX_CLOSE_NAK * pRecvMsg = (MSG_CG_ITEM_PANDORABOX_CLOSE_NAK*) pMsg;

			if(pRecvMsg->m_Result)
			{
				//6132	�ǵ����� ���� ���⸦ �����Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString(6132,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
		}
		break;

	case CG_ITEM_ACTIVE_ETHER_BULLET_ACK:
		{
			//DESC:���ΰ��� ���׸�źȯ������ ����.���������� ACK
			if (!g_pHero) 
            {
                return;
            }
			
			//��Ŷ���Ǯ��
			g_pHero->SetEtherWaitPacket(false);

			MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK* pRecvMsg = (MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK*)pMsg;

			if (RC_ITEM_SUCCESS == pRecvMsg->m_ResultCode)
			{
				//SUCCESS ����: ���� or ����
				bool bCharge = !(g_pHero->GetEthreActive());
			
				if (bCharge)
				{
					g_pHero->SetEtherChargePos(g_pHero->GetEthreInfo().byEtherChargePos);
					g_pHero->SetEtherActive(pRecvMsg->m_BulletEffectCode);
					g_pHero->SetEtherItemLock(FALSE, TRUE);	
					
				}
				else
				{
                    g_pHero->SetEtherItemLock(FALSE,FALSE);	
					g_pHero->SetEtherActive(0);
					g_pHero->UpdateSkillAttr();
					//��ü���¶��
					if (g_pHero->GetEthreInfo().bChange)
					{	
						//��ü����
						g_pHero->SetEtherWaitPacket(true);
						MSG_CG_ITEM_ACTIVE_ETHER_BULLET_SYN SendPacket;
						g_pHero->EtherChangeOFF();
						SendPacket.m_bActive = TRUE;
						SendPacket.m_ActiveBulletItemPos = g_pHero->GetEthreInfo().ChangePos;
						g_pHero->SetEtherChargePos(SendPacket.m_ActiveBulletItemPos);
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
					}
					else
					{
						_asm nop;
					}
				}

				g_pHero->CreateEtherWeaponEffect(false, bCharge);
				
			}
			else
			{
				NoticeReasonItemErrorCode(pRecvMsg->m_ResultCode);
			}
		}
		break;

	case CG_ITEM_ETHER_ACTIVE_STATE_BRD:
		{
			//DESC:�ٸ��������� ���׸�źȯ ����.����
			if(!g_pHero) return;

			MSG_CG_ITEM_ETHER_ACTIVE_STATE_BRD * pRecvMsg	= (MSG_CG_ITEM_ETHER_ACTIVE_STATE_BRD*)pMsg;
			
			Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
			
			if(!pPlayer) 
			{
				assert(!"CG_ITEM_ETHER_ACTIVE_STATE_BRD:�÷��̾� ����");
				return;
			}
			
			
			if(pRecvMsg->m_bActive)
			{
				pPlayer->SetEtherActive(pRecvMsg->m_BulletEffectCode);
				pPlayer->CreateEtherWeaponEffect(false,TRUE);
			}
			else
			{
				pPlayer->SetEtherActive(0);
				pPlayer->CreateEtherWeaponEffect(false,FALSE);
			}
		}
		break;
	case CG_ITEM_INACTIVE_ETHER_BULLET_CMD:
		{
			//TODO:���⼭�� ���׸������� ��������
			if(!g_pHero) return;

			//������������ ���´� 
			g_pHero->SetEtherItemLock(FALSE,FALSE);

			g_pHero->SetEtherActive(0);

			g_pHero->SetEtherWaitPacket(false);
			
			g_pHero->CreateEtherWeaponEffect(false,FALSE);
			
			g_pHero->UpdateSkillAttr();
		}
		break;
	case CG_ITEM_USE_CMD:
		{
			if(!g_pHero) return;
			
			MSG_CG_ITEM_USE_CMD * pRecvMsg	= (MSG_CG_ITEM_USE_CMD*)pMsg;

			SCSlotContainer * pContainer = GetContainer(SI_INVENTORY);
			
			if( !pContainer ) return;

			//���׸� źȯ�Ҹ���Ŷ
			SCItemSlot & rBulletItemSlot = (SCItemSlot &) pContainer->GetSlot(pRecvMsg->m_ItemPos);
			SCItemSlot DeleteBulletSlot;
			if (rBulletItemSlot.IsOverlap())
			{
				if (rBulletItemSlot.GetNum() > 1	)
				{
					rBulletItemSlot.SetNum( rBulletItemSlot.GetNum()-1 );
					pContainer->UpdateSlot(pRecvMsg->m_ItemPos, rBulletItemSlot );
					if(g_pHero)
						g_pHero->CreateEtherWeaponEffect(TRUE);


				}
				else
				{
					pContainer->DeleteSlot(pRecvMsg->m_ItemPos, &DeleteBulletSlot);
					assert(eITEMTYPE_ETHER_BULLET == DeleteBulletSlot.GetItemInfo()->m_wType);
				}	
			}
			else
			{
				pContainer->DeleteSlot(pRecvMsg->m_ItemPos,&DeleteBulletSlot);
				assert(eITEMTYPE_ETHER_BULLET == DeleteBulletSlot.GetItemInfo()->m_wType);
			}
		}
        break;


        //------------------------------------------------------------------------------
        /**  Ż�Ͱ��� ��Ŷ ó��(��ȯ.����.ĳ���ý���.ĳ�������)
        */
	case CG_ITEM_RIDER_CONTROL_BRD:
		{
		MSG_CG_ITEM_RIDER_CONTROL_BRD * pRecvMsg	= (MSG_CG_ITEM_RIDER_CONTROL_BRD*)pMsg;

            if(pRecvMsg->header.result==pRecvMsg->header.eRet_Success)
			{
               switch(pRecvMsg->header.request)
               {

               case pRecvMsg->header.eCmd_SyncRider: 
                   {
                        SerialRiderSyncCommand(&pRecvMsg->header,&pRecvMsg->data);
                   }
                   break;

               case pRecvMsg->header.eReq_SummonStart: 
                   {
                        SerialRiderCastingStart(&pRecvMsg->header,&pRecvMsg->data);
                   }
                   break;

               case pRecvMsg->header.eReq_SummonCancel:
                   {
                        SerialRiderCastingCancle(&pRecvMsg->header,&pRecvMsg->data);
                   }
                   break;

               case pRecvMsg->header.eReq_Summon:
                   {
                        SerialRiderSummon(&pRecvMsg->header,&pRecvMsg->data);
                   }
                   break;

               case pRecvMsg->header.eReq_SummonBattleRelease:
               case pRecvMsg->header.eReq_SummonRelease:
                   {
                        SerialRiderSummonRelease(&pRecvMsg->header,&pRecvMsg->data);            
                   }
                   break;
               default:
                   {
                       _asm nop;
                   }
                   break;
               }
            }
            else
            {
                SerialRiderFail(&pRecvMsg->header,&pRecvMsg->data);
            }
		}
		break;
        //------------------------------------------------------------------------------
        /**   Ż�Ͱ��� ������Ŷ ó��(�ν�,��Ţ.����Ţ)
        */
    case CG_ITEM_RIDER_CONTROL_PARTS_BRD:
        {
            MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD* pRecvMsg	= (MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD*)pMsg;

            if(pRecvMsg->header.result==pRecvMsg->header.eRet_Success)
            {
                switch(pRecvMsg->header.request)
                {
                case pRecvMsg->header.eReq_WaxGloss://���̴������ν�
                    {
                        SerialRiderWaxGloss(&pRecvMsg->header,&pRecvMsg->data);
                    }
                    break;
                case pRecvMsg->header.eReq_EquipParts://���̴�������Ţ
                    {
                        SerialRiderPartEquip(&pRecvMsg->header,&pRecvMsg->data);
                    }
                    break;
                case pRecvMsg->header.eReq_UnEquipParts://���̴���������Ţ
                    {
                        SerialRiderPartUnEquip(&pRecvMsg->header,&pRecvMsg->data);    
                    }
                    break;
                }
            }
            else
            {
                SerialRiderFail(&pRecvMsg->header,&pRecvMsg->data);
            }
		}
		break;


    case CG_ITEM_ETHERIA_CONTROL_BRD:
    case CG_ITEM_ETHERIA_CONTROL_ACK:
        {
            // ���׸��� ����.���հ��� ��Ŷ �ΰ�� ��� ����Ŷ�Ƿ� ���հ����ȴ�  
            MSG_CG_ITEM_ETHERIA_CONTROL_ACK* recv_msg = (MSG_CG_ITEM_ETHERIA_CONTROL_ACK*)pMsg;

            EtheriaControl* header = &recv_msg->header;
            EtheriaControlData* data = &recv_msg->data;
            
            switch(header->request)
            {
            case header->eReq_Extract:
                {
                    SerialEtheriaExtract(header, data);
                }
                break;
            case header->eReq_Combine:
                {
                    SerialEtheriaCombination(header, data);
                }
                break;
           case header->eReq_CastingStart:
                {
                    SerialEtheriaCastingStart(header, data);
                }
                break;
            case header->eReq_CastingCancel:
                {
                    SerialEtheriaCastingCancle(header, data);
                }
                break;
            default:
                {
                    _asm nop;
                }
                break;
            }
        }
        break;

	case CG_ITEM_STAT_CHANGE_USE_ACK:
		{
			MSG_CG_ITEM_STAT_CHANGE_USE_ACK* pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_USE_ACK*)pMsg;
			int ItemCnt = pRecvMsg->m_ItemCount;
			uiItemStatChangeSys* pDlg = GET_CAST_DIALOG( uiItemStatChangeSys, IM_ITEM_STAT_CHANGE_MANAGER::ITEM_STAT_CHANGE );
			if(pDlg)
			{
				pDlg->SetSlotInfo(pRecvMsg->m_ItemCount, &pRecvMsg->m_ItemCodes[0], pRecvMsg->m_atPos, pRecvMsg->m_ChangeItemPos);
				pDlg->ShowInterface(TRUE);
			}

			MouseHandler::Instance()->RemoveMouseState( MouseHandler::ITEM_STAT_CHANGE );
		}
		break;

	case CG_ITEM_STAT_CHANGE_USE_NAK:
		{
			MSG_CG_ITEM_STAT_CHANGE_USE_NAK* pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_USE_NAK*)pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode);

			MouseHandler::Instance()->RemoveMouseState( MouseHandler::ITEM_STAT_CHANGE );
		}
		break;

	case CG_ITEM_STAT_CHANGE_SELECT_ACK:
		{
			MSG_CG_ITEM_STAT_CHANGE_SELECT_ACK* pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_SELECT_ACK*)pMsg;
            
            InventoryDialog* inventory_dialog = static_cast<InventoryDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_INVENTORY));
            if (inventory_dialog)
            {
                inventory_dialog->SetUpdateQuest(false);
                g_pHero->SetInventoryTotalInfo(pRecvMsg->m_InventoryInfo);
                inventory_dialog->SetUpdateQuest(true);
            }			

			MouseHandler::Instance()->RemoveMouseState( MouseHandler::ITEM_STAT_CHANGE );
		}
		break;

	case CG_ITEM_STAT_CHANGE_SELECT_NAK:
		{
			MSG_CG_ITEM_STAT_CHANGE_SELECT_NAK* pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_SELECT_NAK*)pMsg;
			NoticeReasonItemErrorCode(pRecvMsg->m_ErrorCode);

			MouseHandler::Instance()->RemoveMouseState( MouseHandler::ITEM_STAT_CHANGE );
		}
		break;

	case CG_ITEM_IDENTIFY_POSSESSION_ACK:
		{
			MSG_CG_ITEM_IDENTIFY_POSSESSION_ACK* pRecvMsg = (MSG_CG_ITEM_IDENTIFY_POSSESSION_ACK*)pMsg;
			if ( pRecvMsg->m_ResultCode == RC::RC_ITEM_SUCCESS )
			{
				POSTYPE AtPos, EQPos;
				MouseHandler::Instance()->GetPossessionItemPOS(AtPos, EQPos);
				SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
				SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);
				rItemSlot.SetIdentifyPossession(TRUE);
				if ( EQPos == 255 )
				{
					
					ItemManager::Instance()->GetEquipPosition(rItemSlot, EQPos);
				}
				
				ItemManager::Instance()->SendItemMoveMsg(
					SI_INVENTORY, 
					SI_EQUIPMENT,
					AtPos, 
					EQPos);
			}
		}
		break;

#ifdef __NA_001358_ENCHANT_UNIFICATION
	case CG_ITEM_INSTANT_ENCHANT_ACK:
		{
			uiEnchantMan* pUIMan = GET_DIALOG_MANAGER(uiEnchantMan, UIMAN_ENCHANT);
			if (pUIMan)
			{
				pUIMan->OnEnchantPacketAck(pMsg);
			}			
		}
		break;
	case CG_ITEM_INSTANT_ENCHANT_NAK:
		{
			uiEnchantMan* pUIMan = GET_DIALOG_MANAGER(uiEnchantMan, UIMAN_ENCHANT);
			if (pUIMan)
			{
				pUIMan->OnEnchantPacketNak(pMsg);
			}			
		}
		break;
#endif //__NA_001358_ENCHANT_UNIFICATION
    case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK:
    case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK:
    case CG_ITEM_USE_ACCUMULATE_POINT_ACK:
    case CG_ITEM_USE_ACCUMULATE_POINT_NAK:
        // uiCharacterStatusMan::NetworkProc() ���� ó��
        break;
    case CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK:
        {
            MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK* recive_packet = 
                    (MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK*)pMsg;
            InventoryFilterDialog* inventory_filter_dialog = 
                GET_CAST_DIALOG(InventoryFilterDialog, IM_INVENTORY_MANAGER::INVEN_FILTER_DLG);
            inventory_filter_dialog->SetItemInventoryConfig(recive_packet->changed);
        }
        break;
    case CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK:
    case CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK:
        g_InterfaceManager.NetworkProc(IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE, pMsg);        
        break;
    /*case CG_ITEM_IDENTIFY_SOCKET_ACK:        
        {            // TODO:���� �۾� �ʿ�!!
            MSG_CG_ITEM_IDENTIFY_SOCKET_ACK* recive_packet =                
                (MSG_CG_ITEM_IDENTIFY_SOCKET_ACK*)pMsg;            
            int i = recive_packet->error_code_;
        }        
        break;*/
    case CG_ITEM_RECREATE_SOCKET_ACK:
        {
            MSG_CG_ITEM_RECREATE_SOCKET_ACK* recive_packet = (MSG_CG_ITEM_RECREATE_SOCKET_ACK*)pMsg;
            
            TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR result_message[INTERFACE_STRING_LENGTH] = {0,};

            AskSocketRecreateDialog* socket_recreate_dialog =
                static_cast<AskSocketRecreateDialog*>(GET_CAST_DIALOG(AskSocketRecreateDialog, IM_ASSIST_MANAGER::SOCKET_RECREATE_DIALOG));
            if (socket_recreate_dialog != NULL)
            {
                DWORD item_code = socket_recreate_dialog->item_code();
                g_InterfaceManager.GetItemName(item_code, item_name, INTERFACE_STRING_LENGTH);
            }

            if (recive_packet->result_code == RC::RC_ITEM_SUCCESS)
            {
                //48276	%s ��� ������ �����Ǿ����ϴ�. �����մϴ�!
                g_InterfaceManager.GetInterfaceString(48276, string_buffer, INTERFACE_STRING_LENGTH);
                Sprintf(result_message, string_buffer, item_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message);

            }
            else if (recive_packet->result_code == RC::RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO)
            {
                //48275	%s ��� ���� Ȯ���� 1�� �� �ο�������, �ƹ� ��ȭ�� �Ͼ�� �ʾҽ��ϴ�.
                g_InterfaceManager.GetInterfaceString(48275, string_buffer, INTERFACE_STRING_LENGTH);
                Sprintf(result_message, string_buffer, item_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message);
            }
            else
            {
                NoticeReasonItemErrorCode(recive_packet->result_code);
            }
        }        
        break;

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    case CG_ITEM_USE_EXTEND_DATE_NAK:
        {
            MSG_CG_ITEM_USE_EXTEND_DATE_NAK* pRecvMsg = (MSG_CG_ITEM_USE_EXTEND_DATE_NAK*)pMsg;
            NoticeReasonItemErrorCode(pRecvMsg->error_code_);
        }
        break;
    case CG_ITEM_USE_EXTEND_DATE_ACK:
        {
            MSG_CG_ITEM_USE_EXTEND_DATE_ACK* pRecvMsg = (MSG_CG_ITEM_USE_EXTEND_DATE_ACK*)pMsg;
            ItemManager::Instance()->IncItemCount( SI_INVENTORY, pRecvMsg->extend_item_position_, -1 );
            SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );

            // ��� �� ����
            SCItemSlot InsertItemSlot(pRecvMsg->target_item_stream_);

            pContainer->DeleteSlot(pRecvMsg->target_item_position_, NULL);
            pContainer->InsertSlot(pRecvMsg->target_item_position_, InsertItemSlot);
        }
        break;
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    case CG_ITEM_RANDOMIZE_ACK:
        {
            MSG_CG_ITEM_RANDOMIZE_ACK* pRecvMsg = (MSG_CG_ITEM_RANDOMIZE_ACK*)pMsg;
            if( pRecvMsg->result_code == RC::RC_ITEM_SUCCESS )
            {
                if( SCSlotContainer* container = ItemManager::Instance()->GetContainer( SI_INVENTORY ) )
                {
                    SCItemSlot& slot = (SCItemSlot&)container->GetSlot( pRecvMsg->target_item_pos );

                    TCHAR item_name[MAX_ITEMNAME_LENGTH];
                    g_InterfaceManager.GetItemName( slot.GetItemCode(), item_name );

                    TCHAR message[INTERFACE_STRING_LENGTH];
                    g_InterfaceManager.GetInterfaceString( 70582, message, INTERFACE_STRING_LENGTH); //[%s] �����ۿ� ���������� �ɼ��� �ο��Ǿ����ϴ�.

                    TCHAR buffer[INTERFACE_STRING_LENGTH];
                    wsprintf( buffer, message, item_name );

                    GlobalFunc::OutputMsg_SysMsg(buffer);
                }
            }
        }
        break;
#endif

    case CG_ITEM_ZARD_COMPOSE_ACK:        
        {            
            g_InterfaceManager.NetworkProc(IM_SOCKET_MANAGER::SOCKET_COMPOSITE, pMsg);
        }        
        break;
		{
			assert( !"�߸��� Item ���������� ��� �Խ��ϴ�." );
		}
	}
}


VOID ItemManager::OnRecvStyleLink(MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	switch( pMsg->m_byProtocol )
	{
	case CG_STYLE_LINK_ACK:
		{
			MSG_CG_STYLE_LINK_ACK * pRecvMsg = (MSG_CG_STYLE_LINK_ACK *)pMsg;
	
			//��ũ�� �ɾ��ش�.	
			StyleLink( pRecvMsg->m_StyleCode, pRecvMsg->m_ToPos);
			MouseHandler::Instance()->RemoveItemAtHand();

			SCSkillSlot * pSkillSlot = const_cast<SCSkillSlot *>( this->GetSkillSlotByCode( pRecvMsg->m_StyleCode) );
			if (pSkillSlot)
			{
				TCHAR szSkillName[INTERFACE_STRING_LENGTH];

				g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
				//	%s ��Ÿ���� ��� �Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString( eST_STYLE_LINK, szText, INTERFACE_STRING_LENGTH);
				_stprintf(szMessage, szText, szSkillName);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
			}

		}
		break;

	case CG_STYLE_LINK_NAK:
		{
			MSG_CG_ITEM_QUICK_LINKSKILL_NAK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKSKILL_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

	case CG_STYLE_UNLINK_ACK:
		{
			MSG_CG_STYLE_UNLINK_ACK * pRecvMsg = (MSG_CG_STYLE_UNLINK_ACK *)pMsg;

			SCSlotContainer * pContainer = GetContainer(SI_STYLE);

			StyleSlot & rStyleSlot = (StyleSlot &)pContainer->GetSlot(pRecvMsg->m_atPos);

			SCSkillSlot * pSkillSlot = const_cast<SCSkillSlot *>( this->GetSkillSlotByCode( rStyleSlot.GetOrgCode()) );
			if (pSkillSlot)
			{
				TCHAR szSkillName[INTERFACE_STRING_LENGTH];

				g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
				//	%s ��Ÿ���� ���� �Ͽ����ϴ�.
				g_InterfaceManager.GetInterfaceString( eST_STYLE_UNLINK, szText, INTERFACE_STRING_LENGTH);
				Sprintf(szMessage, szText, szSkillName);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
			}

			pContainer->DeleteSlot(pRecvMsg->m_atPos, NULL);
			MouseHandler::Instance()->RemoveItemAtHand();
		}
		break;

	case CG_STYLE_UNLINK_NAK:
		{
			MSG_CG_STYLE_UNLINK_NAK * pRecvMsg = (MSG_CG_STYLE_UNLINK_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;


	case CG_STYLE_LINKMOVE_ACK:
		{
			MSG_CG_STYLE_LINKMOVE_ACK * pRecvMsg = (MSG_CG_STYLE_LINKMOVE_ACK *)pMsg;

			ItemManager::Instance()->SimplexMoveLinkItem(
				SI_STYLE, 
				pRecvMsg->m_fromPos, 
				pRecvMsg->m_toPos );

			MouseHandler::Instance()->RemoveItemAtHand();
		}
		break;

	case CG_STYLE_LINKMOVE_NAK:
		{
			MSG_CG_STYLE_LINKMOVE_NAK * pRecvMsg = (MSG_CG_STYLE_LINKMOVE_NAK *)pMsg;

			NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

	}
}

//------------------------------------------------------------------------------
/**  ���̴� ��ȯ
*/
void ItemManager::SerialRiderSummon(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_ItemAction) 
    {
        return;
    }

    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    
    if(!pPlayer)
    {
        assert(!"PlayerKey����:SerialRiderSummon");
        return;
    }

    RiderContainer* pContainer = (RiderContainer*)pPlayer->GetSlotContainer(SI_RIDER);

    if(!pContainer)
    {
        assert(!"�����̳ʾ���:SerialRiderSummon");
        return;
    }

    DWORD dwRiderCode =0;

    //���ΰ��϶��� ����������������
    if(pPlayer==g_pHero)
    {
        //step1:���̴� �������� ���ͼ� ��Ŷ�Ƿ� ���� ��Ʈ����Copy�Ѵ�
        g_pHero->m_RiderInfo.m_RiderItemPos = header->args.summon.slot_pos;
        SCItemSlot & rItemSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.summon.slot_pos);
        
        if(rItemSlot.IsRiderItem() && rItemSlot.GetItemInfo())
        {
            //�ܿɼǰ�� 1:������ �ɼǸ�����
            SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
			BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
			BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
            
            SkillCalc.ApplyRidingRider(false,IsPcRoom, rItemSlot,data->item_action.update_bits);

            //�����ι��� �����۽�Ʈ������ �����ϱ⋚���� �����ø����� �ݵ�ù���ؾߵȴ�
            DBSERIAL backup_serial =  rItemSlot.GetSerial();
            rItemSlot.Copy(data->item_action.item_slot);
            rItemSlot.SetSerial(backup_serial);

            //�ܿɼǰ�� 2:�Ž��� �ɼǸ��߰�
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemSlot,data->item_action.update_bits);

            //step2:ĳ������ ������ ���̴��κ��� ���ŵǾ���Ѵ�
            dwRiderCode = pContainer->CopyItem(rItemSlot);

            if(dwRiderCode!=0)
            {              
                //���̴�����ȯ
                g_pHero->CreateRider(dwRiderCode,TRUE);
                //���̴�����������
                g_pHero->RefreshRider();
                //6441 Ż���� ��ȯ�Ͽ����ϴ�.
                TCHAR	szMessage[MAX_MESSAGE_LENGTH];
                g_InterfaceManager.GetInterfaceString(6441,szMessage);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
        }

    }//case �÷��̾�: �÷��̾��ȯ��Ŷ�� ������� �÷��̾��� ĳ���þִϸ������ش�.
    else
    {
        //step1:������������ �ϴ� �����Ѵ�.
        SCItemSlot ItemSlot;
        ItemSlot.Copy(data->item_action.item_slot);
        //���̴������̳ʰ���
        dwRiderCode = pContainer->CopyItem(ItemSlot);

        if(dwRiderCode!=0)
        {
            pPlayer->CreateRider(dwRiderCode,TRUE);
            //���̴���������
            pPlayer->RefreshRider();
        }

         //��ȯĳ���� �ٷ���ҽ����ش�
        if(pPlayer->GetCurState()==STATE::SUMMON_RIDER)    
        {
            if(pPlayer->GetNextState() == STATE::NONE)
            {
                pPlayer->SetNextState(STATE::IDLE,g_CurTime);
            }
            pPlayer->CommitChangeState(g_CurTime);
        }
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ��ȯ����
*/
void ItemManager::SerialRiderSummonRelease(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_Actor)
    {
        return;
    }
    
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->actor.requester);

    if(!pPlayer)
    {
        assert(!"�÷��̾�Ű����");
        return;
    }

    if(pPlayer==g_pHero)
    {
        SCItemSlot & rItemToSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.summon.slot_pos);

        //step1:������ ���� Ǯ���ش�.
        if(rItemToSlot.IsRiderItem())
        {
          //�ܿɼǰ�� 1:���� �ɼǸ�����(RiderSummonStatus���ؾ�´�)
          nsSlot::RiderUpdateBits uBit =  rItemToSlot.RiderSummonStatus(FALSE);
          SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
		  BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
		  BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
          SkillCalc.ApplyRidingRider(false,IsPcRoom,rItemToSlot,uBit);
        }

        if(header->result_code==RC::RC_RIDER_SUMMON_RELEASED_BY_HIT)
        {
            ResultRiderMessage(header->result_code);
        }
        else
        {
            //6442:Ż���� �����Ͽ����ϴ�.
            TCHAR	szMessage[MAX_MESSAGE_LENGTH];
            g_InterfaceManager.GetInterfaceString(6442,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        
       
        g_pHero->DestroyRider();
        
        //step2:�ڵ�Ż�ͺ���ó��(������ �ٷ� ��ȯĳ������ ������)
        if(g_pHero->m_RiderInfo.m_bChangeRider)
        {   
            g_pHero->m_RiderInfo.m_bChangeRider= FALSE;
            MSG_CG_ITEM_RIDER_CONTROL_SYN SendMsg;
            SendMsg.header.request = SendMsg.header.eReq_SummonStart;
            g_pHero->m_RiderInfo.m_RiderItemPos = g_pHero->m_RiderInfo.m_RiderChangePos;
            SendMsg.header.args.casting_start.slot_pos = g_pHero->m_RiderInfo.m_RiderChangePos;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
        }
    }
    else
    {
            pPlayer->DestroyRider();
    }
}
//------------------------------------------------------------------------------
/**  ������ ����ȭ�����߱�����
*/
void ItemManager::SerialRiderSyncCommand(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_ItemAction)
    {
        return;
    }
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    //���ο��Ը��´�
    if(pPlayer==g_pHero)
    {
        SCItemSlot & rItemSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.sync.where);
        

        if(rItemSlot.IsRiderItem() )
        {
            //�ٷ� ��ü�⶧���� �ø����� ����Ѵ�
            DBSERIAL backup_serial =  rItemSlot.GetSerial();

            if(rItemSlot.IsRiderSummoned())
            {
                //�ܿɼǰ�� 1:������ �ɼǸ�����
                SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
				BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
				BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
                SkillCalc.ApplyRidingRider(false,IsPcRoom,rItemSlot,data->item_action.update_bits);
                rItemSlot.Copy(data->item_action.item_slot);
                //�ܿɼǰ�� 2:�Ž��� �ɼǸ��߰�
                SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemSlot,data->item_action.update_bits);
            }
            else
            {
                //�ܴٸ��������ǰ�쿡�� Copy�� ������
                rItemSlot.Copy(data->item_action.item_slot);
            }

            rItemSlot.SetSerial(backup_serial);
            
        }
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ĳ���ý���
*/
void ItemManager::SerialRiderCastingStart(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_Actor)
    {
        return;
    }
   
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->actor.requester);
    assert(pPlayer && "�÷��̾�Ű����");

    if(pPlayer)
    {
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_SUMMON_RIDER;
        if(pPlayer==g_pHero)
        {
            g_pHero->m_RiderInfo.m_bSendSummon  = FALSE;
            g_pHero->m_RiderInfo.m_bCancelSummon = FALSE;
            action.RIDEROBJECT.ItemPos = g_pHero->m_RiderInfo.m_RiderItemPos;
            RiderInventoryDialog *pRiderContainer = (RiderInventoryDialog *)GetContainer(SI_RIDER);
            if(pRiderContainer)
            {
                pRiderContainer->ShowInterface(FALSE);
            }
            
        }
        else
        {
            action.RIDEROBJECT.ItemPos = 0;
        }
        pPlayer->PutAction(&action);
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ĳ�������
*/
void ItemManager::SerialRiderCastingCancle(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_Actor)
    {
        return;
    }

    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->actor.requester);
    assert(pPlayer && "�÷��̾�Ű����");


    if( pPlayer == g_pHero )
    {
        // ��ȯ ���°� ������ �ʾҴµ� ��� ��Ŷ���� �޴°�찡 �־.. (�׼�ť���� ���� ������ΰ��)
        g_pHero->m_RiderInfo.m_bCancelSummon = TRUE;
        return;
    }

    if(pPlayer)
    {
        if(pPlayer->GetCurState()==STATE::SUMMON_RIDER)    
        {
            if(pPlayer->GetNextState() == STATE::NONE)
                pPlayer->SetNextState(STATE::IDLE,g_CurTime);

            pPlayer->CommitChangeState(g_CurTime);
        }
    }
}
//------------------------------------------------------------------------------
/**  ���̴������� �ν�
*/
void ItemManager::SerialRiderWaxGloss(RiderControl* header,RiderControlData* data)
{
    SCSlotContainer* pContainer= GetContainer(SI_INVENTORY);
    
    if(!pContainer)
    {
        return;
    }

    //Hero�����̳�
    SCItemSlot & rItemToSlot =  (SCItemSlot &)pContainer->GetSlot(header->args.wax_gloss.to_parts);
    SCItemSlot & rItemFromSlot = (SCItemSlot &)pContainer->GetSlot(header->args.wax_gloss.from_wax);
    DBSERIAL backup_serial =  rItemToSlot.GetSerial();

    SCItemSlot DeleteSlot;
    if(rItemFromSlot.IsOverlap())
    {
        if (rItemFromSlot.GetNum() > 1	)
        {
            rItemFromSlot.SetNum( rItemFromSlot.GetNum()-1 );
            pContainer->UpdateSlot(header->args.wax_gloss.from_wax, rItemFromSlot );
        }
        else
        {
            pContainer->DeleteSlot(header->args.wax_gloss.from_wax, &DeleteSlot);
            assert(eITEMTYPE_WAX == DeleteSlot.GetItemInfo()->m_wType);
        }	

    }
    else
    {
        pContainer->DeleteSlot(header->args.wax_gloss.from_wax, &DeleteSlot);
        assert(eITEMTYPE_WAX == DeleteSlot.GetItemInfo()->m_wType);
    }
   
    if(rItemToSlot.IsContainValidItem())
    {
        int nPreEnChant = rItemToSlot.GetEnchant();
        rItemToSlot.Copy(data->item_action.item_slot);
        rItemToSlot.SetSerial(backup_serial);
        int nCurEnChant = rItemToSlot.GetEnchant();
        
        //6447 �ν� �������� ���� �Ǿ����ϴ�.
        int iStringIdx = 6447;

        //������ȿ���������� ���������
        if(nPreEnChant==nCurEnChant)
        {
            //6448 ����������ȿ���� ����Ǿ����ϴ�.
            iStringIdx = 6448;
        }
        else
        {
            //�������϶�
            if(nCurEnChant==0)
            {
                //6450 �ν� ������ ȿ���� ��������ϴ�.
                iStringIdx = 6450;
            }
        }
        
        TCHAR	szMessage[MAX_MESSAGE_LENGTH];
        g_InterfaceManager.GetInterfaceString(iStringIdx,szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}
//------------------------------------------------------------------------------
/**  ���̴������� ��Ţ
*/
void ItemManager::SerialRiderPartEquip(RiderControl* header,RiderControlData* data)
{
    //�÷��̾�
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    if(!pPlayer)
    {
        assert(!"PlayerKey ����:SerialRiderPartEquip");
        return;
    }
    RiderContainer *pRiderContainer = (RiderContainer *)pPlayer->GetSlotContainer(SI_RIDER);
    
    if(!pRiderContainer)
    {
        assert(!"�����̳ʾ���:SerialRiderPartEquip");
        return;
    }

    //���ΰ�
    if(pPlayer==g_pHero)
    {
        //step1:�����۰��� ó���κ�
        POSTYPE RiderPos =  header->args.equip.to.value - eRIDER_PARTS_POS::StartIndex;

        //step2:�κ����� ���̴������� �����ͼ� ��Ŷ�Ƿ� ���� ��Ʈ���� ������ �����ɾ���������
        SCItemSlot & rItemFromSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(g_pHero->m_RiderInfo.m_RiderItemPos);
      
        if(rItemFromSlot.IsRiderItem())
        {
            //�ܿɼǰ�� 1:������ �ɼǸ�����
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
			BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
			BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
            SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
            SkillCalc.ApplyRidingRider(false,IsPcRoom,rItemFromSlot,data->item_action.update_bits);
            
            DBSERIAL backup_serial =  rItemFromSlot.GetSerial();
            rItemFromSlot.Copy(data->item_action.item_slot);
            rItemFromSlot.SetSerial(backup_serial);

            //�ܿɼǰ�� 2:�Ž��� �ɼǸ��߰�
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemFromSlot,data->item_action.update_bits);

            BOOL bRet = GetContainer(SI_INVENTORY)->DeleteSlot(header->args.equip.from,NULL);

            pRiderContainer->CopyItem(rItemFromSlot);
            pPlayer->RefreshRider();

            //6443	��� �����Ͽ����ϴ�.
            TCHAR	szMessage[MAX_MESSAGE_LENGTH];
            g_InterfaceManager.GetInterfaceString(6443,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            
        }
        else
        {
            assert(!"���̴� ������ ������ġ����");
        }
    }
    else
    {
       SCItemSlot  ItemSlot; 
       ItemSlot.Copy(data->item_action.item_slot);
       pRiderContainer->CopyItem(ItemSlot);
       pPlayer->RefreshRider();
    }
}
//------------------------------------------------------------------------------
/**  ���̴������� ����Ţ
*/
void ItemManager::SerialRiderPartUnEquip(RiderControl* header,RiderControlData* data)
{

    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    if(!pPlayer)
    {
        assert(!"PlayerKey ����:SerialRiderPartUnEquip");
        return;
    }

    RiderContainer* pContainer = (RiderContainer*)pPlayer->GetSlotContainer(SI_RIDER);
    if(!pContainer )
    {
        assert(!"�����̳ʾ���:SerialRiderPartUnEquip");
        return;
    }

    //���ΰ��ΰ�?
    if (pPlayer == g_pHero)
    {
        POSTYPE InvenPos =  header->args.unequip.to;
        POSTYPE RiderPos =  header->args.unequip.from.value- eRIDER_PARTS_POS::StartIndex;

        SCItemSlot & rItemRider =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.unequip.where);
        SCItemSlot & rItemToSlot =  (SCItemSlot &)GetContainer(SI_RIDER)->GetSlot(RiderPos);

        DBSERIAL backup_serial =  rItemRider.GetSerial();

        //�ܿɼǰ�� 1:������ �ɼǸ�����
        SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
		BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
		BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
        
        if (g_pHero->IsRiding())
        {
            SkillCalc.ApplyRidingRider(false,IsPcRoom,rItemRider,data->item_action.update_bits);
        }

        //step1:���̴������ۿ��� ������������ �������
        SCItemSlot  NewItemPart;
        rItemRider.UnEquipRiderParts(header->args.unequip.from,&NewItemPart);

        //step2:��Ŷ�Ƿ� ������������ ���̴������ۿ� ������
        rItemRider.Copy(data->item_action.item_slot);
        rItemRider.SetSerial(backup_serial);

        //�ܿɼǰ�� 2:�Ž��� �ɼ� �߰�
        if (g_pHero->IsRiding())
        {
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemRider,data->item_action.update_bits);
        }

        //step3:����� ������������ �κ����Կ� �־��ش�.
        GetContainer(SI_INVENTORY)->InsertSlot(InvenPos,NewItemPart);
        pContainer->CopyItem(rItemRider);
        pPlayer->RefreshRider();

        //6444	��� �����Ͽ����ϴ�.
        //�������̽��� ��Ʈ�� ����� ���缭 ����,
        TCHAR	szMessage[MAX_MESSAGE_LENGTH];
        g_InterfaceManager.GetInterfaceString(6444,szMessage,MAX_MESSAGE_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

    }
    else
    {
       SCItemSlot ItemSlot;
       ItemSlot.Copy(data->item_action.item_slot);
       Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
       pContainer->CopyItem(ItemSlot);
       pPlayer->RefreshRider();
      
    }
}
//------------------------------------------------------------------------------
/**  ���̵��� ȣ�������ν�..Ż���̼�ȯ�Ǿ��ִ���üũ
*/
void ItemManager::CheckRiderSummon()
{
    SCSlotContainer * pAtContainer = GetContainer(SI_INVENTORY); 

    if(!pAtContainer)
    {
        return;
    }

    for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
    {
        if (pAtContainer->IsEmpty(i))
            continue;

        SCItemSlot & rItemSlot =(SCItemSlot&) pAtContainer->GetSlot(i);

        if(rItemSlot.IsRiderItem() && rItemSlot.IsRiderSummoned())
        {
            RiderInventoryDialog *pRiderContainer = (RiderInventoryDialog *)GetContainer(SI_RIDER);

            if(pRiderContainer)
            {
                memset(&(g_pHero->m_RiderInfo),0,sizeof(RIDER_INFO));
                SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
                rItemSlot.RiderSummonStatus(FALSE);
                nsSlot::RiderUpdateBits bits = rItemSlot.RiderSummonStatus(TRUE);
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
				BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
				BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
                SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemSlot,bits);
                
                g_pHero->m_RiderInfo.m_RiderItemPos= i;
                pRiderContainer->ReloadRider(rItemSlot);
                return;
            }
        }
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ����
*/
void ItemManager::SerialRiderFail(RiderControl* header,RiderControlData* data)
{
#ifdef _DEBUG
    switch(header->result)
    {
    case header->eRet_Failed:break;
    case header->eRet_ItemFailed:break;// ref: RC::RC_ITEM_~~
    case header->eRet_RoomFailed:break;// ref: RC::RC_ROOM_~~
    case header->eRet_StatusFailed:break;// ref: RC::RC_STATUS_~~
    }
#endif//_DEBUG
    ResultRiderMessage(header->result_code);

    if ((header->request==header->eReq_SummonRelease) || (header->request==header->eReq_SummonBattleRelease))
    {
        if(g_pHero->m_RiderInfo.m_bChangeRider)
        {
            g_pHero->m_RiderInfo.m_bChangeRider =FALSE;
        }
    }
    //Fail�ε� ������ Ÿ�԰������� ��Ŷó���� �޶�����.
    switch(data->data_type)
    {
    case data->eDataType_Actor:
        {
            Player *pPlayer  = (Player*)g_ObjectManager.GetObject(data->actor.requester);
            assert(pPlayer && "�÷��̾�Ű����");
            if(pPlayer)
            {
                //��ȯĳ���� �ٷ���ҽ����ش�
                if(pPlayer->GetCurState()==STATE::SUMMON_RIDER)    
                {
                    if(pPlayer->GetNextState() == STATE::NONE)
                    {
                        pPlayer->SetNextState(STATE::IDLE,g_CurTime);
                    }
                    pPlayer->CommitChangeState(g_CurTime);
                }
            }
        }
        break;
    default:
        {
            if(g_pHero->m_RiderInfo.m_bSendSummon)
            {
                g_pHero->m_RiderInfo.m_bSendSummon = FALSE;
            }
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ���и޽���
*/
void ItemManager::ResultRiderMessage(WORD wCode)
{
    int iStringCode=0;  
    	
    switch(wCode)
    {
    case RC_RIDER_FAILED:                   iStringCode  = 0;break;// ���� (Normal)
#ifdef _DEV_VER
    case RC_RIDER_UNEXPECTED_FAILED:        iStringCode  = 6457;break;// ���� (Unexpected)
    case RC_RIDER_CANT_FIND_INFO:           iStringCode  = 6458;break;// ���̴��� ���õ� ������ ã�� �� ����. 
#endif//_DEV_VER
    case RC_RIDER_NOT_RIDER_ITEM:           iStringCode  = 6459;break;// ���̴� ������ �ƴ�
    case RC_RIDER_TIME_EXPIRED:             iStringCode  = 6460;break;// �Ⱓ ����� ������
    case RC_RIDER_MISMATCH_SUMMON_RIDER:    iStringCode  = 6463;break;// ��ȯ�� ������ ���̴��� �����Ϸ��� ���̴��� Ʋ����.
    case RC_RIDER_SUMMON_CASTING:           iStringCode  = 6464;break;// ��ȯ ĳ���� ���Դϴ�. 
    case RC_RIDER_ALREADY_SUMMONED:         iStringCode  = 6461;break;// �̹� ��ȯ�Ǿ� �ֽ��ϴ�.
    case RC_RIDER_ALREADY_SUMMON_RELEASED:  iStringCode  = 6462;break;// �̹� ��ȯ �����Ǿ� �ֽ��ϴ�.
    case RC_RIDER_SUMMON_RELEASED_BY_HIT:   iStringCode  = 6465;break;// ���� ��ġ�ų� �װų� �ؼ� ��ȯ �����˴ϴ�.
    }
    
    //�޽������
    if(0 != iStringCode)
    {
        TCHAR	szMessage[MAX_MESSAGE_LENGTH]={0,};
        g_InterfaceManager.GetInterfaceString(iStringCode, szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}


//--------------------------------------------------------------------------------------------------
/**  ���׸��� ������(S2C)

     (1)���þ������̹ٲ�� + (2)���ο�����۵� ����
*/
void ItemManager::SerialEtheriaExtract(EtheriaControl* header,EtheriaControlData* data)
{
    // ACK: �κ� ������
    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
    if (inventory_manager != NULL)
    {
        inventory_manager->UnlockInventorySlot(header->args.extract.slot_pos);
        inventory_manager->UnlockInventorySlot(header->args.extract.etherealizer_pos);
    }

    InventoryDialog* inven = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if (inven == NULL) 
    {
        return;
    }

    // ���� �޽���
    if (header->result != header->eRet_Success)
    {
        //6115:���׸��� ������ �����Ͽ����ϴ�.
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString(6115));
        NoticeReasonItemErrorCode(header->result_code);
        return;
    }

    //��Ŷ����: header��data�� ����ġ    
    if ((data->data_type == data->eDataType_Extract) ||
        (data->data_type == data->eDataType_Changed))
    {
        // no operation
    }
    else
    {
        return;
    }
  
    //(0)���� �������� ���ų� or ����
    POSTYPE use_pos = header->args.extract.etherealizer_pos;
    SCItemSlot& use_item = (SCItemSlot &)inven->GetSlot(use_pos);
    if (use_item.IsContainValidItem())
    {
        DBSERIAL backup_serial =  use_item.GetSerial();
        SCItemSlot delete_slot;

        if (use_item.IsOverlap() &&
            (use_item.GetNum() > 1))
        {
            use_item.SetNum(use_item.GetNum() - 1);
            inven->UpdateSlot(use_pos, use_item );
        }
        else
        {
            inven->DeleteSlot(use_pos, &delete_slot);
        }
    }
    
    //(1)���þ����� �ϴ� ���ŵ�
    inven->DeleteSlot(header->args.extract.slot_pos, NULL);

    if (data->data_type == data->eDataType_Changed)
    {
        // �������ϼ����ְ� or ���׸����ϼ����ִ�
        SCItemSlot change_item(data->changed.changed_item);
        inven->InsertSlot(header->args.extract.slot_pos, change_item);
    }
    else
    {
        // ���õȾ������� �����Ⱑ�ȴ�
        SCItemSlot shell_item(data->extract.shell_item);
        inven->InsertSlot(header->args.extract.slot_pos, shell_item);

        // ���׸��ư� ���� �����ȴ�
        SCItemSlot etheria_item;
        etheria_item.Copy(data->extract.etheria_item.m_Stream);
        inven->InsertSlot(data->extract.etheria_item.m_Pos, etheria_item);
    }
    
    //6114:���׸��� ������ �����Ͽ����ϴ�.
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString(6114));

}
//--------------------------------------------------------------------------------------------------
/**  ���׸��� ���հ��(S2C)

     (1)���յɾ������� �ٲ�� + (2)���׸��ƴ����ŵ�
*/
void ItemManager::SerialEtheriaCombination(EtheriaControl* header,EtheriaControlData* data)
{
    if (header->result == header->eRet_Success)
    {
        //���׸��� ���� SUCCESS!!
        SCSlotContainer* inventory = GetContainer(SI_INVENTORY);
        if (inventory == NULL )
        {
            return;
        }

        //��Ŷ����: header��data�� ����ġ    
        if (data->data_type != data->eDataType_Combine)
        {
            return;
        }

        //���յɾ����۽��Ծ��´�
        SCItemSlot& original_item = (SCItemSlot&)inventory->GetSlot(header->args.combine.to_shell);

        // �����۽��Թ�ȿüũ
        if (original_item.IsContainValidItem() == FALSE)
        {
            return;
        }

        //(1)���վ����۹ٲ�� (�����Ⱑ �Ǿ����)
        inventory->DeleteSlot(header->args.combine.to_shell, NULL);
        SCItemSlot combine_item(data->combine.combined_item);
        inventory->InsertSlot(header->args.combine.to_shell, combine_item);

        //(2)���׸��� ��������
        inventory->DeleteSlot(header->args.combine.from_etheria, NULL);

        //6117:���׸��� ������ �����Ͽ����ϴ�
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6117));
    }
    else
    {
        // ���и޽��� ���
        NoticeReasonItemErrorCode(header->result_code);
    }
}
//--------------------------------------------------------------------------------------------------
/**  ���׸��� ĳ���ý���(S2C)
*/
void ItemManager::SerialEtheriaCastingStart(EtheriaControl* header,EtheriaControlData* data)
{
    Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(data->actor.requester));

    bool is_hero = (player_ptr == g_pHero);

    if (header->result == header->eRet_Success)
    {   
        if (player_ptr->IsKindOfObject(PLAYER_OBJECT) == FALSE)
        {
            return;
        }
        //���� �׼ǽ���
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ETHERIA_EXTRACT;
        action.ETHERIA.slot_pos = header->args.casting.slot_pos;
        action.ETHERIA.etherealizer_pos = header->args.casting.etherealizer_pos;
        player_ptr->PutAction(&action);
    }
    else
    {
        // ���и޽��� ���
        NoticeReasonItemErrorCode(header->result_code);

        if (is_hero == false)
        {
            return;
        }
        //���н� ���
        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
        if (inventory_manager != NULL)
        {
            inventory_manager->UnlockInventorySlot(header->args.casting.slot_pos);
            inventory_manager->UnlockInventorySlot(header->args.casting.etherealizer_pos);
        }
    }
}
//--------------------------------------------------------------------------------------------------
/**  ���׸��� ĳ�������(S2C)
*/
void ItemManager::SerialEtheriaCastingCancle(EtheriaControl* header,EtheriaControlData* data)
{
    Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(data->actor.requester));

    bool is_hero = (player_ptr == g_pHero);
  
    if (player_ptr &&
        player_ptr->IsKindOfObject(PLAYER_OBJECT))
    {
        if (player_ptr->GetCurState() == STATE::ETHERIA_EXTRACT)    
        {
            if (player_ptr->GetNextState() == STATE::NONE)
            {
                player_ptr->SetNextState(STATE::IDLE,g_CurTime);
            }
            player_ptr->CommitChangeState(g_CurTime);
        }
    }

    uiAssistMan* assist_manager = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
    if ((assist_manager == NULL) || 
        (inventory_manager == NULL))
    {
        return;
    }

    POSTYPE slot_pos, use_pos;
    assist_manager->GetItemCustomizePos(slot_pos, use_pos);
    inventory_manager->UnlockInventorySlot(slot_pos);
    inventory_manager->UnlockInventorySlot(use_pos);
}

//--------------------------------------------------------------------------------------------------
/**  ĳ���� Ŀ���͸���¡ �����Ŷ
*/
void ItemManager::SerialCharCustomize(void* recv_packet)
{
    MSG_CG_CHARINFO_CHANGE_APPEARANCE_BRD* data = (MSG_CG_CHARINFO_CHANGE_APPEARANCE_BRD*)recv_packet;

    if (data->result_ != RC_ITEM_SUCCESS)
    {
        NoticeReasonItemErrorCode(data->result_);
        return;
    }

    Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(data->player_key_));

    if (player_ptr == NULL)
    {
        assert(!"CG_CHARINFO_CHANGE_APPEARANCE_BRD �ش� Player����");
        return;
    }

    ChangeCharacterAppearanceTypeConst::ValueType change_type = data->character_appearance_.appearance_type_;
    if (change_type & ChangeCharacterAppearanceTypeConst::kFace)
    {
        player_ptr->SetPartVariation(PLAYER_VARIATION_FACE, data->character_appearance_.face_);
    }
    if (change_type & ChangeCharacterAppearanceTypeConst::kHair)
    {
        player_ptr->SetPartVariation(PLAYER_VARIATION_HAIR, data->character_appearance_.hair_);
    }
    if (change_type & ChangeCharacterAppearanceTypeConst::kHeight)
    {
        player_ptr->SetHeightVariation(data->character_appearance_.height_);
    }
    bool is_changed_gender = false;
    if (change_type & ChangeCharacterAppearanceTypeConst::kGender)
    {
        GenderType::Value gender_type = static_cast<GenderType::Value>(data->character_appearance_.gender);
        if (gender_type != player_ptr->GetGender())
        {
            is_changed_gender = true;
        }
        player_ptr->SetGender(gender_type);
        player_ptr->TransformGenderType();
    }
    else
    {
        player_ptr->RefreshAppearRender();
    }

    if (is_changed_gender)
    {
        player_ptr->CreateEffectAtHead(StrToWzID("A766"), 0.0f);
    }
    else
    {
        player_ptr->CreateEffectAtHead(StrToWzID("A716"), 0.0f);
    }    

    if (player_ptr == g_pHero)
    {
        InventoryDialog* inventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
        if (inventory == NULL)
        {
            return;
        }
        POSTYPE use_pos = data->item_position_;
        SCItemSlot& use_item = (SCItemSlot &)inventory->GetSlot(use_pos);
        if (use_item.IsContainValidItem())
        {
            DBSERIAL backup_serial =  use_item.GetSerial();
            if (use_item.IsOverlap() &&
                (use_item.GetNum() > 1))
            {
                use_item.SetNum(use_item.GetNum() - 1);
                inventory->UpdateSlot(use_pos, use_item);
            }
            else
            {
                inventory->DeleteSlot(use_pos, NULL);
            }
        }

        HeroEquipmentContainer * container_ptr  =  g_pHero->GetHeroEquipContainer();

        if (container_ptr)
        {
            container_ptr->RefreshVariation();
        }
    }
}


