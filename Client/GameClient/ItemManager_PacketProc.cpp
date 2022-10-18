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
			// 아이템 리스트를 받아서 처리한다.
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
			// 위에꺼랑 같이 AddFieldItem() 함수로 처리하자.
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


			// 사용한 인첸트 보석의 카운트 하나내린다.
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
                    assert(eITEMTYPE_SOCKET_ZARD == DeleteCrystalItemSlot.GetItemInfo()->m_wType && "보석이아닌데 지웠다니!!!!");
				}	
			}
			else
			{
				pAtContainer->DeleteSlot(pRecvMsg->m_SocketItemPos, &DeleteCrystalItemSlot);
                assert(eITEMTYPE_SOCKET_ZARD == DeleteCrystalItemSlot.GetItemInfo()->m_wType && "보석이아닌데 지웠다니!!!!");
			}

			// 결과 물 세팅
			SCItemSlot InsertItemSlot(pRecvMsg->m_TargetItemStream);

			pAtContainer->DeleteSlot(pRecvMsg->m_TargetPos, NULL);
			pAtContainer->InsertSlot(pRecvMsg->m_TargetPos, InsertItemSlot);

			// 사운드 출력
			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, (SCSlot *)&InsertItemSlot);

			TCHAR szResult[256];
			TCHAR szItemName[256];

			g_InterfaceManager.GetItemName(rItemSlot.GetCode(), szItemName, 256);

			//	%s 옵션 추가가 성공하였습니다.
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
      
            if (pRecvMsg->m_dwErrorCode == RC_ITEM_SOCKET_FAILED_TO_FILL_BY_GRADE)// 소켓에 최대 사용가능한 등급의 갯수가 넘었다.
            {
                TCHAR error_message[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};

                g_InterfaceManager.GetItemName(pRecvMsg->target_item_code, item_name);
                g_InterfaceManager.GetInterfaceString(65017, temp_message, INTERFACE_STRING_LENGTH);
                sprintf(error_message, temp_message, item_name);
                //65017 [%s] 장비에는 더 이상 해당등급의 쟈드 옵션을 추가할 수 없습니다.
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

			// 엑스트라 스톤을 적용했습니다.
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
	// 함수로 짜르장.. -_-;

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
				//5095 %I64u 하임을 획득하였습니다.(추가 하임 %d 획득)
				g_InterfaceManager.GetInterfaceString(5095, szMessage);
				Snprintf(szChatResult, INTERFACE_STRING_LENGTH-1, szMessage, acquiredMoney - bonusMoney, bonusMoney);
			}
			else
			{
				//	%I64u 하임를 획득하였습니다.
				g_InterfaceManager.GetInterfaceString( eST_CODE_GET_MONEY, szMessage, MAX_MESSAGE_LENGTH );
				_stprintf(szChatResult, szMessage, acquiredMoney);
			}
#ifdef _DEV_VER
			TCHAR szDebugMessage[INTERFACE_STRING_LENGTH] ={0,};
			Snprintf(szDebugMessage,  INTERFACE_STRING_LENGTH -1,"DEBUG!! 획득 하임 = %I64u , BonusRatio = %d" , acquiredMoney, pRecvMsg->m_BonusRatio );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szDebugMessage);
#endif //_DEV_VER


			// 돈주었다는 메시지 뛰운다.
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

                // 활성
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

                // 비활성
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

                // 활성
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
                // 비활성
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
			
			//GS_BUG_FIX:갱신위치가 틀리다 분할할때 갯수셋팅잘못됨
			UpdateQuickInfo();

			//인벤내에서 아이템을 분리하는 경우엔 퀘스트 정보를 갱신할 필요 없다.
			//g_InterfaceManager.UpdateQuestInfo();// 퀘스트 정보 갱신
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
// 새로 생긴 QUICK 링크 이동 처리
	case CG_ITEM_QUICK_LINKITEM_ACK:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_ACK * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKITEM_ACK *)pMsg;

			SCSlotContainer * pFromContainer = (SCSlotContainer *)GetContainer(SI_INVENTORY);
			
			// 퀵으로 링크
			QuickContainer * pQuickDlg = (QuickContainer * )GetItemContainerDialog(SI_QUICK);
			
			SCSlot & rFromSlot = pFromContainer->GetSlot( pRecvMsg->m_OrgPos);
			QuickLinkItem( SI_INVENTORY, pRecvMsg->m_OrgPos, pRecvMsg->m_ToPos, rFromSlot.GetCode());

			MouseHandler::Instance()->RollbackItemAtHand();		

			TCHAR szName1[INTERFACE_STRING_LENGTH];

			BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rFromSlot.GetCode());
			if (pItemInfo)
			{
				g_InterfaceManager.GetItemName( rFromSlot.GetCode(), szName1, INTERFACE_STRING_LENGTH );
				//	[%s] 아이템을 등록 하였습니다.
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
			
			// 퀵으로 링크
			QuickContainer * pQuickDlg = (QuickContainer * )GetItemContainerDialog(SI_QUICK);			

			SCSkillSlot * pSkillSlot = NULL;

			//링크를 걸어준다.	
			QuickLinkSkill( SI_SKILL, pRecvMsg->m_SkillCode, pRecvMsg->m_ToPos);
			MouseHandler::Instance()->RemoveItemAtHand();

			pSkillSlot = const_cast<SCSkillSlot *>(this->GetSkillSlotByCode( pRecvMsg->m_SkillCode ) );
			if (pSkillSlot)
			{
				TCHAR szSkillName[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
				//	[%s] 스킬을 등록 했습니다.
				g_InterfaceManager.GetInterfaceString( eST_ITEM_QUICK_LINKSKILL, szText, INTERFACE_STRING_LENGTH);
				_stprintf( szMessage, szText, szSkillName );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

				// 스킬 등록 하는 소리
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
						//	[%s] 아이템을 해제 하였습니다.
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
						//	[%s] 스킬을 해제 하였습니다.
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

			// PK로 인해 [%s] 아이템을 떨어뜨렸습니다. 5132
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
                if (pRecvMsg->combine_quantity_ != 0)//나눈경우
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

			g_InterfaceManager.UpdateQuestInfo();// 퀘스트 정보 갱신

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

            // 삭제하려는 아이템이 PC방효과아이템인경우,
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

                    // 해제
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

			// 5871 정상적으로 텝이 확장 되었습니다.
			g_InterfaceManager.GetInterfaceString(5871, szMessage, INTERFACE_STRING_LENGTH);

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case CG_ITEM_USE_ACK:
		{
		MSG_CG_ITEM_USE_ACK * pRecvMsg = (MSG_CG_ITEM_USE_ACK *)pMsg;

			// 아이템을 사용해라.
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
				// 판매한 아이템 히스토리 목록에 추가.
				pShopDlg->PushHistorySlot(DeleteSlot);
			}


			if (0 == DeleteSlot.GetCode())
			{
				// 비워있었는데 어케팔았다냐?
				// 어쨌거나 서버에서 하라고 하니 그냥 해야겠지만
				// 테스트용으로 놔둔다.
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
                        // 내구도가 0이 되었을 때 외형 갱신을 해줘야 한다.
                        g_pHero->RefreshAppearRender();
                        GlobalFunc::EquipmentPlayerRefreshAppearRender();

						TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
						g_InterfaceManager.GetItemName(pRecvMsg->m_ItemCode, szName, INTERFACE_STRING_LENGTH);

						TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
						TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

						//5147	%s의 내구도가 0이되었습니다.
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
				// Equip에 있는 모든 아이템의 내구력을 풀로 채워줘라
				RepairItemAll( SI_EQUIPMENT );

				// 5950	장착된 아이템이 모두 수리되었습니다.
				TCHAR szMessage[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString(5950, szMessage, INTERFACE_STRING_LENGTH);
				
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
			}
			else
			{
				// 특정 아이템에 내구력을 채워줘라.
				RepairItem( pRecvMsg->m_SlotIndex, pRecvMsg->m_SlotPos );		

				SCSlotContainer * pContainer = GetContainer(pRecvMsg->m_SlotIndex);
				if ( pContainer )
				{
					SCSlot & rSlot = pContainer->GetSlot( pRecvMsg->m_SlotPos );

					// 메시지 출력					
					TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};	
					
					// 5951	[%s]아이템이 수리되었습니다.
					g_InterfaceManager.GetInterfaceString(5951, szMessage, INTERFACE_STRING_LENGTH);
					g_InterfaceManager.GetItemName(rSlot.GetCode(), szName, INTERFACE_STRING_LENGTH);					

					Snprintf( szResult, INTERFACE_STRING_LENGTH-1, szMessage, szName);

					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szResult);

					SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
					if(rItemSlot.IsLimited())
					{
						ZeroMemory(szMessage, sizeof(szMessage));
						//	6055	장비 수리로 인해 최대 내구도가 2감소하였습니다.
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
            else // 일반 하임, // 혹은 아이템(상관없음)
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

			// 특정 위치의 랜덤 아이템을 소비한다.
			POSTYPE pTargetPos;
			MouseHandler::Instance()->GetRandomItemPOS(pTargetPos);
			ItemManager::Instance()->DeleteItem( SI_INVENTORY, pTargetPos, 1, COMPOSE_USE );

			ItemManager::Instance()->InsertInventoryTotalInfo( CREATE, pRecvMsg->m_ItemInfo );

			//인벤토리는 락풀기
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
			//인벤토리는 락풀기
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
	// 소유권 소멸
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
				// 정상적인 위치 이면서 
				// 해당 위치에 아이템이 있어야함.
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
						
						// 시리얼 중복으로 인해 아이템을 사용하실 수 없습니다. 운영팀에 신고해 주시기 바랍니다. 5024
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
						_T("해당슬롯 위치가 잘못된 위치에 갱신되려한다. 해당 메시지를 보면 프로그램팀에 알려주시기 바랍니다.") );
					return;
				}
				
				SCItemSlot ItemSlot(pRecvMsg->m_ItemStream);	

				if (g_pHero)
				{
					g_pHero->OnUseItemWithCoolTime(ItemSlot.GetCode());
				}			

				pAtContainer->DeleteSlot(pRecvMsg->m_SlotPos, NULL); // 기존정보를 제거해준다. ( 아이콘 리소스때문에 먼저 지워준다 )
				pAtContainer->InsertSlot(pRecvMsg->m_SlotPos, ItemSlot);


				if( ItemSlot.GetItemInfo() )
				{
                    BYTE charge_subtype = ItemSlot.GetItemRuleInfo().charge_sub_type;
					TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
					if (charge_subtype == eCHARGE_SUB_COORDINATE)
					{
						// 5936	현재 좌표를 성공적으로 저장 하였습니다
						g_InterfaceManager.GetInterfaceString(5936, szText, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szText);
					}
                    else if(ItemManager::CanPeriodExtend(&ItemSlot) == true) //<- 기간연장 가능 아이템 일경우
					{
#ifdef __NA_001358_ENCHANT_UNIFICATION
						// no operation
#else //__NA_001358_ENCHANT_UNIFICATION						
						EnchantDialog * pDlg = (EnchantDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_ENCHANT );

						if(pDlg)
						{
							//럭키아이템의 경우 인첸성공시메시지 막기
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
							    //6867	%s 아이템의 기간이 만료되었습니다.
							    g_InterfaceManager.GetInterfaceString( 6867,szTemp);
						    }
						    else
						    {
							    if( ItemSlot.IsExpiredTime() )
							    {
								    //6867	%s 아이템의 기간이 만료되었습니다.
								    g_InterfaceManager.GetInterfaceString( 6867,szTemp);

							    }
							    else
							    {
								    //6868	%s 아이템의 기간연장이 완료되었습니다.
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


			// 사용한 인첸트 보석의 카운트 하나내린다.
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
					assert(eITEMTYPE_SOCKETEXTRACT == DeleteChargeItemSlot.GetItemInfo()->m_wType && "소켓(유료)아이템이아닌데 지웠다니!!!!");
				}	
			}
			else
			{
				pAtContainer->DeleteSlot(pRecvMsg->m_ChargeIntemPos, &DeleteChargeItemSlot);
				assert(eITEMTYPE_SOCKETEXTRACT == DeleteChargeItemSlot.GetItemInfo()->m_wType && "소켓(유료)아이템이아닌데 지웠다니!!!!");
			}
			
			// 아이템 채워 주기
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
                string_code = 5879; //[%s]의 하급 소켓 옵션을 삭제하였습니다.
                break;
            case kZardGrade2:
                string_code = 5913; //[%s]의 중급 소켓 옵션을 삭제하였습니다.
                break;
            case kZardGrade3:
                string_code = 5880; //[%s]의 상급 소켓 옵션을 삭제하였습니다.
                break;
            case kZardGrade4:
                string_code = 5914; //[%s]의 최상급 소켓 옵션을 삭제하였습니다.
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
                assert(!"ItemManager_PacketProc.cpp: not found zard_grade (쟈드 등급이 잘못 되었습니다)");
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

			// update한 스텟을 얻어온다. 
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

			// update한 스텟을 얻어온다. 
			g_pHero->GetPlayerAttribute()->GetCharInfo( pPlayInfo );

			g_pHero->UpdateItemAttrCalculator();
			g_pHero->UpdateSkillAttr();

            uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

            g_pHero->RefreshAppearRender();
            GlobalFunc::EquipmentPlayerRefreshAppearRender();

			// 5886	정상적으로 모든 스탯이 초기화 되었습니다.
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
			// 생성된 아이템을 인벤토리에 넣어준다.
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

			// 유료 아이템 슬롯이 확장되었습니다 5871
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 5871 ) );
		}
		break;

	case CG_ITEM_SETITEM_CHANGE_BRD:
		{
			//하는거 없다.
		}
		break;



	case CG_ITEM_VALIDATION_OF_REIHALT_ACK:
		{
			uiFateItemSystem* pFateItemSystem=  GET_CAST_DIALOG( uiFateItemSystem, IM_FATEITEM_SYSTEM::FATEITEM_DLG );

			if( pFateItemSystem )
			{
				MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK * pRecvMsg = ( MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK *) pMsg;
				//6327	럭키 아이템 능력치를 결정중입니다,
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6327));
				pFateItemSystem->SetTargetItemPos(pRecvMsg->m_TargetItemPos);
				
				//주의: 바로 인벤토리에 넣어주는것이 아니라, 겜블 UI가 완료된 후에 인벤토리에 아이템을 넣어줄것임.
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

			//6330	럭키아이템 인증에 실패하였습니다. 다시 시도해 주십시오.
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
					//무기에 에테르 사용가능하도록 셋팅
					SCItemSlot & rItemSlot = (SCItemSlot &) pContainer->GetSlot(pRecvMsg->m_WeaponItemPos);
					rItemSlot.SetEquipEtherDevice(TRUE);

					// 에테르 장착 아이템 삭제하기
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
					//성공 메세지 뿌리기
					TCHAR szWeaponName[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szWeaponName, INTERFACE_STRING_LENGTH -1 );

					//6129	%s에 에테르 방출장치를 장착하였습니다.
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
					//6159 %s 아이템이 활성화되었습니다.	
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

			//해당메시지코드
			int iResultCode = 0;
		
			if(!g_pHero) break;
			switch(pRecvMsg->m_Result)
			{
				//6393	펫을 소환 하였습니다.
			case RC_ITEM_SUMMON_SUMMON_SUCCESS:
				{
					SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
					
					if(!pContainer)break;
					
					SCItemSlot & rItemSlot =  (SCItemSlot &)pContainer->GetSlot(pRecvMsg->m_PetItemPos);
									
					//펫기간제
					rItemSlot.StartQuantity();
					g_pHero->CreatePetSummon(rItemSlot.GetImageItemInfo()->m_wSkillCode); //펫생성 
					g_pHero->UpdatePetInfo(pRecvMsg->m_PetItemPos);					 //펫업데이트	
					iResultCode = 6393;
                    rItemSlot.SetUserBlocked(TRUE);
                    
				}break;

				//6394 해제 성공
			case RC_ITEM_SUMMON_RETURN_SUCCESS:
				{
					g_pHero->DeletePetSummon();//펫소환 해제
					g_pHero->UpdateSkillAttr();//펫옵션 해제
					iResultCode = 6394;

					//펫기간제
					SCItemSlot & rItemSlot =  (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->GetPetItemPos());
					rItemSlot.EndQuantity();
                    rItemSlot.SetUserBlocked(FALSE);
				
					//펫 자동소환(소환패킷전송)
					if(g_pHero->IsPetChange())
					{
						g_pHero->PetChangeOFF();
						MSG_CG_ITEM_USE_PET_ITEM_SYN SendPacket; 
						SendPacket.m_PetItemPos = g_pHero->PetChangePos();
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					}

				}break;

				//6395	펫이 소환 중입니다.해제 후 사용 하시길 바랍니다.
			case RC_ITEM_SUMMON_ALREADY_SUMMONED:			iResultCode = 6395;break;				
				//6396	팻 아이템이 아니여서 사용 할 수 없습니다.
			case RC_ITEM_SUMMON_NOT_PET_ITEM:				iResultCode = 6396;break;						
				//?? 소환된 펫이 없는데 RETURN 하려고 함
			case RC_ITEM_SUMMON_NOT_SUMMONED_PET:			iResultCode = 6397;break;				
				//?? 소환된 펫이랑 해제하려는 아이템이 서로 다름.
			case RC_ITEM_SUMMON_MISMATCH_PET_ITEM:			iResultCode = 6395;break;				
				//6391	포만감이 부족하여 펫을 소환할수없습니다.
			case RC_ITEM_CAN_NOT_SUMMON_FULLNESS_ZERO:		iResultCode = 6391;break;		
				//6397	소환된 펫이 없음
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
				//판도라아이템셋팅
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

					//판도라아이템은 재설정한다.
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
					//6133	아이템 이동을 실패하였습니다.
					g_InterfaceManager.GetInterfaceString(6133,szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}

			}
		}
		break;
	case CG_ITEM_MOVE_PANDORA_ITEM_NAK:
		{
			//판도라 아이템 Move Nak!
			MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK* pRecvMsg = (MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK*) pMsg;
			
            g_InterfaceManager.UnLockDlgs();

            if (pRecvMsg->m_Result == RC_ITEM_HAVENOTSPACE)
            {
                NoticeReasonItemErrorCode(pRecvMsg->m_Result);
            }
            else
            {
                //6133	아이템 이동을 실패하였습니다.
                g_InterfaceManager.GetInterfaceString(6133,szMessage);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
		}
		break;

	case CG_ITEM_USE_PANDORA_ITEM_NAK:
		{
			//판도라 아이템 Use Nak!
			MSG_CG_ITEM_USE_PANDORA_ITEM_NAK * pRecvMsg = (MSG_CG_ITEM_USE_PANDORA_ITEM_NAK*) pMsg;

			if(pRecvMsg->m_Result!=0)
			{
                uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
                if (inventory_manager != NULL)
                {
                    inventory_manager->UnlockInventoryUI();
                }

				//6132	판도라의 상자 열기를 실패하였습니다.
				g_InterfaceManager.GetInterfaceString(6132,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}

		}
		break;
	//============================================================
	// 추후 FIX시 패킷정리
	//============================================================
	case CG_ITEM_PANDORABOX_CLOSE_ACK:
		{
			//처리할거 없음
		}
		break;

	case CG_ITEM_PANDORABOX_CLOSE_NAK:
		{
			MSG_CG_ITEM_PANDORABOX_CLOSE_NAK * pRecvMsg = (MSG_CG_ITEM_PANDORABOX_CLOSE_NAK*) pMsg;

			if(pRecvMsg->m_Result)
			{
				//6132	판도라의 상자 열기를 실패하였습니다.
				g_InterfaceManager.GetInterfaceString(6132,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
		}
		break;

	case CG_ITEM_ACTIVE_ETHER_BULLET_ACK:
		{
			//DESC:주인공이 에테르탄환에대한 장전.해제에대한 ACK
			if (!g_pHero) 
            {
                return;
            }
			
			//패킷대기풀기
			g_pHero->SetEtherWaitPacket(false);

			MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK* pRecvMsg = (MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK*)pMsg;

			if (RC_ITEM_SUCCESS == pRecvMsg->m_ResultCode)
			{
				//SUCCESS 조건: 장전 or 해제
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
					//교체상태라면
					if (g_pHero->GetEthreInfo().bChange)
					{	
						//교체장전
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
			//DESC:다른유저들이 에테르탄환 장전.해제
			if(!g_pHero) return;

			MSG_CG_ITEM_ETHER_ACTIVE_STATE_BRD * pRecvMsg	= (MSG_CG_ITEM_ETHER_ACTIVE_STATE_BRD*)pMsg;
			
			Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
			
			if(!pPlayer) 
			{
				assert(!"CG_ITEM_ETHER_ACTIVE_STATE_BRD:플레이어 없음");
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
			//TODO:여기서는 에테르장전을 강제해제
			if(!g_pHero) return;

			//장전아이템을 얻어온다 
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

			//에테르 탄환소모패킷
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
        /**  탈것관련 패킷 처리(소환.해제.캐스팅시작.캐스팅취소)
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
        /**   탈것관련 파츠패킷 처리(왁싱,이큅.언이큅)
        */
    case CG_ITEM_RIDER_CONTROL_PARTS_BRD:
        {
            MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD* pRecvMsg	= (MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD*)pMsg;

            if(pRecvMsg->header.result==pRecvMsg->header.eRet_Success)
            {
                switch(pRecvMsg->header.request)
                {
                case pRecvMsg->header.eReq_WaxGloss://라이더파츠왁스
                    {
                        SerialRiderWaxGloss(&pRecvMsg->header,&pRecvMsg->data);
                    }
                    break;
                case pRecvMsg->header.eReq_EquipParts://라이더파츠이큅
                    {
                        SerialRiderPartEquip(&pRecvMsg->header,&pRecvMsg->data);
                    }
                    break;
                case pRecvMsg->header.eReq_UnEquipParts://라이더파츠언이큅
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
            // 에테리아 추출.조합관련 패킷 두결과 모두 요패킷의로 통합관리된다  
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
        // uiCharacterStatusMan::NetworkProc() 에서 처리
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
        {            // TODO:서버 작업 필요!!
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
                //48276	%s 장비에 소켓이 생성되었습니다. 축하합니다!
                g_InterfaceManager.GetInterfaceString(48276, string_buffer, INTERFACE_STRING_LENGTH);
                Sprintf(result_message, string_buffer, item_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message);

            }
            else if (recive_packet->result_code == RC::RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO)
            {
                //48275	%s 장비에 소켓 확률을 1번 더 부여했으나, 아무 변화도 일어나지 않았습니다.
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

            // 결과 물 세팅
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
                    g_InterfaceManager.GetInterfaceString( 70582, message, INTERFACE_STRING_LENGTH); //[%s] 아이템에 성공적으로 옵션이 부여되었습니다.

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
			assert( !"잘못된 Item 프로토콜이 들어 왔습니다." );
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
	
			//링크를 걸어준다.	
			StyleLink( pRecvMsg->m_StyleCode, pRecvMsg->m_ToPos);
			MouseHandler::Instance()->RemoveItemAtHand();

			SCSkillSlot * pSkillSlot = const_cast<SCSkillSlot *>( this->GetSkillSlotByCode( pRecvMsg->m_StyleCode) );
			if (pSkillSlot)
			{
				TCHAR szSkillName[INTERFACE_STRING_LENGTH];

				g_InterfaceManager.GetInterfaceString( pSkillSlot->GetSkillInfo()->m_dwSkillNcode, szSkillName, INTERFACE_STRING_LENGTH);
				//	%s 스타일을 등록 하였습니다.
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
				//	%s 스타일을 해제 하였습니다.
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
/**  라이더 소환
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
        assert(!"PlayerKey없음:SerialRiderSummon");
        return;
    }

    RiderContainer* pContainer = (RiderContainer*)pPlayer->GetSlotContainer(SI_RIDER);

    if(!pContainer)
    {
        assert(!"컨테이너없음:SerialRiderSummon");
        return;
    }

    DWORD dwRiderCode =0;

    //주인공일때는 아이템정보를참조
    if(pPlayer==g_pHero)
    {
        //step1:라이더 아이템을 얻어와서 패킷의로 받은 스트림을Copy한다
        g_pHero->m_RiderInfo.m_RiderItemPos = header->args.summon.slot_pos;
        SCItemSlot & rItemSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.summon.slot_pos);
        
        if(rItemSlot.IsRiderItem() && rItemSlot.GetItemInfo())
        {
            //●옵션계산 1:구슬롯 옵션를제거
            SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
			BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
			BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
            
            SkillCalc.ApplyRidingRider(false,IsPcRoom, rItemSlot,data->item_action.update_bits);

            //서버로받은 아이템스트림으로 갱신하기떄문에 기존시리얼은 반드시백업해야된다
            DBSERIAL backup_serial =  rItemSlot.GetSerial();
            rItemSlot.Copy(data->item_action.item_slot);
            rItemSlot.SetSerial(backup_serial);

            //●옵션계산 2:신슬롯 옵션를추가
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemSlot,data->item_action.update_bits);

            //step2:캐스팅이 끝나고 라이더인벤이 갱신되어야한다
            dwRiderCode = pContainer->CopyItem(rItemSlot);

            if(dwRiderCode!=0)
            {              
                //라이더를소환
                g_pHero->CreateRider(dwRiderCode,TRUE);
                //라이더파츠를갱신
                g_pHero->RefreshRider();
                //6441 탈것을 소환하였습니다.
                TCHAR	szMessage[MAX_MESSAGE_LENGTH];
                g_InterfaceManager.GetInterfaceString(6441,szMessage);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
        }

    }//case 플레이어: 플레이어소환패킷이 왔을경우 플레이어의 캐스팅애니를없애준다.
    else
    {
        //step1:아이템정보를 일단 복사한다.
        SCItemSlot ItemSlot;
        ItemSlot.Copy(data->item_action.item_slot);
        //라이더컨테이너갱신
        dwRiderCode = pContainer->CopyItem(ItemSlot);

        if(dwRiderCode!=0)
        {
            pPlayer->CreateRider(dwRiderCode,TRUE);
            //라이더파츠갱신
            pPlayer->RefreshRider();
        }

         //소환캐스팅 바로취소시켜준다
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
/**  라이더 소환해제
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
        assert(!"플레이어키없음");
        return;
    }

    if(pPlayer==g_pHero)
    {
        SCItemSlot & rItemToSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.summon.slot_pos);

        //step1:슬롯의 락을 풀어준다.
        if(rItemToSlot.IsRiderItem())
        {
          //●옵션계산 1:슬롯 옵션를제거(RiderSummonStatus통해얻는다)
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
            //6442:탈것을 해제하였습니다.
            TCHAR	szMessage[MAX_MESSAGE_LENGTH];
            g_InterfaceManager.GetInterfaceString(6442,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        
       
        g_pHero->DestroyRider();
        
        //step2:자동탈것변경처리(해제후 바로 소환캐스팅을 보낸다)
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
/**  서버와 동기화를맞추기위한
*/
void ItemManager::SerialRiderSyncCommand(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_ItemAction)
    {
        return;
    }
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    //본인에게만온다
    if(pPlayer==g_pHero)
    {
        SCItemSlot & rItemSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.sync.where);
        

        if(rItemSlot.IsRiderItem() )
        {
            //바로 교체기때문에 시리얼은 백업한다
            DBSERIAL backup_serial =  rItemSlot.GetSerial();

            if(rItemSlot.IsRiderSummoned())
            {
                //●옵션계산 1:구슬롯 옵션를제거
                SkillAttrCalculator SkillCalc(*(g_pHero->GetPlayerAttribute()));
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
				BOOL IsPcRoom = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else		
				BOOL IsPcRoom = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
                SkillCalc.ApplyRidingRider(false,IsPcRoom,rItemSlot,data->item_action.update_bits);
                rItemSlot.Copy(data->item_action.item_slot);
                //●옵션계산 2:신슬롯 옵션를추가
                SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemSlot,data->item_action.update_bits);
            }
            else
            {
                //●다른아이템의경우에는 Copy로 끝난다
                rItemSlot.Copy(data->item_action.item_slot);
            }

            rItemSlot.SetSerial(backup_serial);
            
        }
    }
}
//------------------------------------------------------------------------------
/**  라이더 캐스팅시작
*/
void ItemManager::SerialRiderCastingStart(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_Actor)
    {
        return;
    }
   
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->actor.requester);
    assert(pPlayer && "플레이어키없음");

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
/**  라이더 캐스팅취소
*/
void ItemManager::SerialRiderCastingCancle(RiderControl* header,RiderControlData* data)
{
    if(data->data_type!=data->eDataType_Actor)
    {
        return;
    }

    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->actor.requester);
    assert(pPlayer && "플레이어키없음");


    if( pPlayer == g_pHero )
    {
        // 소환 상태가 되지도 않았는데 취소 패킷부터 받는경우가 있어서.. (액션큐에서 아직 대기중인경우)
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
/**  라이더아이템 왁싱
*/
void ItemManager::SerialRiderWaxGloss(RiderControl* header,RiderControlData* data)
{
    SCSlotContainer* pContainer= GetContainer(SI_INVENTORY);
    
    if(!pContainer)
    {
        return;
    }

    //Hero컨테이너
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
        
        //6447 왁스 아이템이 적용 되었습니다.
        int iStringIdx = 6447;

        //동일한효과아이템을 사용했을때
        if(nPreEnChant==nCurEnChant)
        {
            //6448 같은아이템효과가 적용되었습니다.
            iStringIdx = 6448;
        }
        else
        {
            //무광택일때
            if(nCurEnChant==0)
            {
                //6450 왁스 아이템 효과가 사라졌습니다.
                iStringIdx = 6450;
            }
        }
        
        TCHAR	szMessage[MAX_MESSAGE_LENGTH];
        g_InterfaceManager.GetInterfaceString(iStringIdx,szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}
//------------------------------------------------------------------------------
/**  라이더아이템 이큅
*/
void ItemManager::SerialRiderPartEquip(RiderControl* header,RiderControlData* data)
{
    //플레이어
    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    if(!pPlayer)
    {
        assert(!"PlayerKey 없음:SerialRiderPartEquip");
        return;
    }
    RiderContainer *pRiderContainer = (RiderContainer *)pPlayer->GetSlotContainer(SI_RIDER);
    
    if(!pRiderContainer)
    {
        assert(!"컨테이너없음:SerialRiderPartEquip");
        return;
    }

    //주인공
    if(pPlayer==g_pHero)
    {
        //step1:아이템관련 처리부분
        POSTYPE RiderPos =  header->args.equip.to.value - eRIDER_PARTS_POS::StartIndex;

        //step2:인벤에서 라이더슬롯을 가져와서 패킷의로 받은 스트림을 복사후 장착될아이템제거
        SCItemSlot & rItemFromSlot =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(g_pHero->m_RiderInfo.m_RiderItemPos);
      
        if(rItemFromSlot.IsRiderItem())
        {
            //●옵션계산 1:구슬롯 옵션를제거
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

            //●옵션계산 2:신슬롯 옵션를추가
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemFromSlot,data->item_action.update_bits);

            BOOL bRet = GetContainer(SI_INVENTORY)->DeleteSlot(header->args.equip.from,NULL);

            pRiderContainer->CopyItem(rItemFromSlot);
            pPlayer->RefreshRider();

            //6443	장비를 장착하였습니다.
            TCHAR	szMessage[MAX_MESSAGE_LENGTH];
            g_InterfaceManager.GetInterfaceString(6443,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            
        }
        else
        {
            assert(!"라이더 아이템 슬롯위치오류");
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
/**  라이더아이템 언이큅
*/
void ItemManager::SerialRiderPartUnEquip(RiderControl* header,RiderControlData* data)
{

    Player *pPlayer = (Player*)g_ObjectManager.GetObject(data->item_action.requester);
    if(!pPlayer)
    {
        assert(!"PlayerKey 없음:SerialRiderPartUnEquip");
        return;
    }

    RiderContainer* pContainer = (RiderContainer*)pPlayer->GetSlotContainer(SI_RIDER);
    if(!pContainer )
    {
        assert(!"컨테이너없음:SerialRiderPartUnEquip");
        return;
    }

    //주인공인가?
    if (pPlayer == g_pHero)
    {
        POSTYPE InvenPos =  header->args.unequip.to;
        POSTYPE RiderPos =  header->args.unequip.from.value- eRIDER_PARTS_POS::StartIndex;

        SCItemSlot & rItemRider =  (SCItemSlot &)GetContainer(SI_INVENTORY)->GetSlot(header->args.unequip.where);
        SCItemSlot & rItemToSlot =  (SCItemSlot &)GetContainer(SI_RIDER)->GetSlot(RiderPos);

        DBSERIAL backup_serial =  rItemRider.GetSerial();

        //●옵션계산 1:구슬롯 옵션를제거
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

        //step1:라이더아이템에서 파츠아이템을 끄집어낸다
        SCItemSlot  NewItemPart;
        rItemRider.UnEquipRiderParts(header->args.unequip.from,&NewItemPart);

        //step2:패킷의로 받은아이템을 라이더아이템에 셋팅함
        rItemRider.Copy(data->item_action.item_slot);
        rItemRider.SetSerial(backup_serial);

        //●옵션계산 2:신슬롯 옵션 추가
        if (g_pHero->IsRiding())
        {
            SkillCalc.ApplyRidingRider(true,IsPcRoom,rItemRider,data->item_action.update_bits);
        }

        //step3:끄집어낸 파츠아이템을 인벤슬롯에 넣어준다.
        GetContainer(SI_INVENTORY)->InsertSlot(InvenPos,NewItemPart);
        pContainer->CopyItem(rItemRider);
        pPlayer->RefreshRider();

        //6444	장비를 해제하였습니다.
        //인터페이스의 스트링 사이즈에 마춰서 변경,
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
/**  존이동시 호출함으로써..탈것이소환되어있는지체크
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
/**  라이더 실패
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
    //Fail인데 데이터 타입값에따라 패킷처리가 달라진다.
    switch(data->data_type)
    {
    case data->eDataType_Actor:
        {
            Player *pPlayer  = (Player*)g_ObjectManager.GetObject(data->actor.requester);
            assert(pPlayer && "플레이어키없음");
            if(pPlayer)
            {
                //소환캐스팅 바로취소시켜준다
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
/**  라이더 실패메시지
*/
void ItemManager::ResultRiderMessage(WORD wCode)
{
    int iStringCode=0;  
    	
    switch(wCode)
    {
    case RC_RIDER_FAILED:                   iStringCode  = 0;break;// 실패 (Normal)
#ifdef _DEV_VER
    case RC_RIDER_UNEXPECTED_FAILED:        iStringCode  = 6457;break;// 실패 (Unexpected)
    case RC_RIDER_CANT_FIND_INFO:           iStringCode  = 6458;break;// 라이더에 관련된 정보를 찾을 수 없다. 
#endif//_DEV_VER
    case RC_RIDER_NOT_RIDER_ITEM:           iStringCode  = 6459;break;// 라이더 아이템 아님
    case RC_RIDER_TIME_EXPIRED:             iStringCode  = 6460;break;// 기간 만료된 아이템
    case RC_RIDER_MISMATCH_SUMMON_RIDER:    iStringCode  = 6463;break;// 소환된 상태의 라이더와 해제하려는 라이더가 틀리다.
    case RC_RIDER_SUMMON_CASTING:           iStringCode  = 6464;break;// 소환 캐스팅 중입니다. 
    case RC_RIDER_ALREADY_SUMMONED:         iStringCode  = 6461;break;// 이미 소환되어 있습니다.
    case RC_RIDER_ALREADY_SUMMON_RELEASED:  iStringCode  = 6462;break;// 이미 소환 해제되어 있습니다.
    case RC_RIDER_SUMMON_RELEASED_BY_HIT:   iStringCode  = 6465;break;// 성공 다치거나 죽거나 해서 소환 해제됩니다.
    }
    
    //메시지출력
    if(0 != iStringCode)
    {
        TCHAR	szMessage[MAX_MESSAGE_LENGTH]={0,};
        g_InterfaceManager.GetInterfaceString(iStringCode, szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}


//--------------------------------------------------------------------------------------------------
/**  에테리아 추출결과(S2C)

     (1)선택아이템이바뀌고 + (2)새로운아이템도 생김
*/
void ItemManager::SerialEtheriaExtract(EtheriaControl* header,EtheriaControlData* data)
{
    // ACK: 인벤 락제거
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

    // 실패 메시지
    if (header->result != header->eRet_Success)
    {
        //6115:에테리아 추출이 실패하였습니다.
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString(6115));
        NoticeReasonItemErrorCode(header->result_code);
        return;
    }

    //패킷에러: header와data가 불일치    
    if ((data->data_type == data->eDataType_Extract) ||
        (data->data_type == data->eDataType_Changed))
    {
        // no operation
    }
    else
    {
        return;
    }
  
    //(0)사용된 아이템은 제거나 or 갱신
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
    
    //(1)선택아이템 일단 제거됨
    inven->DeleteSlot(header->args.extract.slot_pos, NULL);

    if (data->data_type == data->eDataType_Changed)
    {
        // 껍데기일수도있고 or 에테리아일수도있다
        SCItemSlot change_item(data->changed.changed_item);
        inven->InsertSlot(header->args.extract.slot_pos, change_item);
    }
    else
    {
        // 선택된아이템은 껍데기가된다
        SCItemSlot shell_item(data->extract.shell_item);
        inven->InsertSlot(header->args.extract.slot_pos, shell_item);

        // 에테리아가 새로 생성된다
        SCItemSlot etheria_item;
        etheria_item.Copy(data->extract.etheria_item.m_Stream);
        inven->InsertSlot(data->extract.etheria_item.m_Pos, etheria_item);
    }
    
    //6114:에테리아 추출이 성공하였습니다.
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString(6114));

}
//--------------------------------------------------------------------------------------------------
/**  에테리아 조합결과(S2C)

     (1)조합될아이템이 바뀌고 + (2)에테리아는제거됨
*/
void ItemManager::SerialEtheriaCombination(EtheriaControl* header,EtheriaControlData* data)
{
    if (header->result == header->eRet_Success)
    {
        //에테리아 조합 SUCCESS!!
        SCSlotContainer* inventory = GetContainer(SI_INVENTORY);
        if (inventory == NULL )
        {
            return;
        }

        //패킷에러: header와data가 불일치    
        if (data->data_type != data->eDataType_Combine)
        {
            return;
        }

        //조합될아이템슬롯얻어온다
        SCItemSlot& original_item = (SCItemSlot&)inventory->GetSlot(header->args.combine.to_shell);

        // 아이템슬롯무효체크
        if (original_item.IsContainValidItem() == FALSE)
        {
            return;
        }

        //(1)조합아이템바뀌고 (껍데기가 되어버림)
        inventory->DeleteSlot(header->args.combine.to_shell, NULL);
        SCItemSlot combine_item(data->combine.combined_item);
        inventory->InsertSlot(header->args.combine.to_shell, combine_item);

        //(2)에테리아 슬롯제거
        inventory->DeleteSlot(header->args.combine.from_etheria, NULL);

        //6117:에테리아 조합이 성공하였습니다
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6117));
    }
    else
    {
        // 실패메시지 출력
        NoticeReasonItemErrorCode(header->result_code);
    }
}
//--------------------------------------------------------------------------------------------------
/**  에테리아 캐스팅시작(S2C)
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
        //추출 액션시작
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ETHERIA_EXTRACT;
        action.ETHERIA.slot_pos = header->args.casting.slot_pos;
        action.ETHERIA.etherealizer_pos = header->args.casting.etherealizer_pos;
        player_ptr->PutAction(&action);
    }
    else
    {
        // 실패메시지 출력
        NoticeReasonItemErrorCode(header->result_code);

        if (is_hero == false)
        {
            return;
        }
        //실패시 언락
        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
        if (inventory_manager != NULL)
        {
            inventory_manager->UnlockInventorySlot(header->args.casting.slot_pos);
            inventory_manager->UnlockInventorySlot(header->args.casting.etherealizer_pos);
        }
    }
}
//--------------------------------------------------------------------------------------------------
/**  에테리아 캐스팅취소(S2C)
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
/**  캐릭터 커스터마이징 결과패킷
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
        assert(!"CG_CHARINFO_CHANGE_APPEARANCE_BRD 해당 Player없음");
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


