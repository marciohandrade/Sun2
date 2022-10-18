#include "SunClientPrecompiledHeader.h"
#include "Hero.h"
#include "MouseHandler.h"
#include "ItemUnitRenderer.h"
#include "Mouse.h"
#include <ItemInfoParser.h>
#include <SCSlotContainer.h>
#include "SoundEffect.h"
#include "ItemManager.h"
#include "QuickSlot.h"
#include "StyleSlot.h"
#include "SCSkillSlot.h"
#include "InterfaceManager.h"
#include "ShopDialog.h"
#include "InventoryDialog.h"
#include "QuickContainer.h"
#include "StyleContainer.h"
#include "EventInventoryDialog.h"
#include "HeroEquipmentContainer.h"

#include "IconImage.h"
#include "asknormalenchantdialog.h"
#include "AskExtraStoneUse.h"
#include "heroactioninput.h"
#include "SkillInfoParser.h"
#include "StyleInfoParser.h"
#include "chatdialog.h"
#include "dummyItemSlot.h"


#include "SocketOptionDescParser.h"
#include "ItemUnitRenderer.h"
#include "ItemCompositeParser.h"

#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiTradeMan/uiTradeMan.h"
#include "Application.h"
#include "MouseCursor.h"
#ifdef __NA_001358_ENCHANT_UNIFICATION
#else //__NA_001358_ENCHANT_UNIFICATION
#include "EnchantDialog2.h"
#endif //__NA_001358_ENCHANT_UNIFICATION
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "ItemETCCompositeDlg.h"
#include "CastLeaveFieldDialog.h"
#include "uiItemShopDiv/uiItemShopDiv.h"
#include "uiItemDiv/uiItemDiv.h"
#include "uiItemDivMan/uiItemDivMan.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiAssistMan/uiAssistMan.h"
#include "MapInfoParser.h"
#include "uiCashItemShopExtensionBuy/uiCashItemShopExtensionBuy.h"
#include "Map.h"
#include "uiFateItemSystemMan/uiFateItemSystemMan.h"
#include "uiFateItemSystem/uiFateItemSystem.h"
#include "uiPartyMan/uiPartyMan.h"
#include "LotteryInventoryDialog.h"
#include "uiPetNameSetting/uiPetNameSetting.h"
#include "uiPetMan/uiPetMan.h"
#include "ProjectileManagerEx.h"
#include "uiGuildMan/uiGuildMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/uiGuildRelationAsk.h"
#else
    #include "uiChangeGuildName/uiChangeGuildName.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiQuestMan/uiQuestMan.h"
#include "RiderInventoryDialog.h"
#include "RidingInfoParser.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"

#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"

#include "SCPerkSlot.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiPerkDialog/uiPerkDialog.h"

#include "Interface/uiAutobot/uiAutobotConditionBattle.h"
#include "ObjectManager.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#include "SocketSystem/ZardComposeInfoParser.h"
#include "uiSocketComposite/uiSocketComposite.h"
#include "SocketSystem/SocketItemRatioParser.h"
#include "uiSocketMan/uiSocketMan.h"
#include "ItemOptionHeader.h"

#include "SkillMain.h"
#include "CastDialog.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#include "uiSmartNpcDialogMan/uiSmartNpcDialogReply.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#include "ItemCrystalizeListParser.h"
#include "CooltimeManager.h"
#include "ItemCooltimeManager.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningDialog.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
//------------------------------------------------------------------------------
/**
*/
void _CallBack_Item_Use(bool bYes)
{
}

MouseHandler::MouseHandler()
:m_bCtlKeyDown(FALSE)
{
	memset(&m_MouseItem, 0, sizeof(MouseItem));
	m_eTradeState = MouseHandler::NONE;
	m_pItemUnitRender = NULL;
	m_bPendingLockDeleteItem = FALSE;
	InitConfirmState();
	m_bIsProssessionWin = false;
}

MouseHandler::~MouseHandler()
{
	ReleaseUnitRenderer();
}


VOID	MouseHandler::InitConfirmState( void )
{

	SetSavingPosition(FALSE);
	SetTryPortal(FALSE);



	SetTryUseChageItem(FALSE);

	SetTryRemoveSocket(FALSE);

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	SetTryAddRandomOption(FALSE);
#endif	
}


VOID 
MouseHandler::InitUnitRenderer( CDrawBase * pDraw )
{
	assert(pDraw);

	m_pItemUnitRender = new ItemUnitRenderer;
	m_pItemUnitRender->Init(pDraw, MOUSE_ITEM_MAX);
}

VOID				
MouseHandler::ReleaseUnitRenderer()
{
	if (m_pItemUnitRender)
	{
		m_pItemUnitRender->Release();
		delete m_pItemUnitRender;
		m_pItemUnitRender = NULL;
	}
}

VOID MouseHandler::Update()
{
	if( m_MouseItem.m_pRenderUnit	!= NULL )
	{
		ItemUnitRenderer::RenderUnit *	pRenderUnit = m_pItemUnitRender->GetItemUnit(DEFAULT_MOUSE_ITEM_SERIAL);

		int iMouseX, iMouseY;
		iMouseX = Mouse::Instance()->GetMouseX();
		iMouseY = Mouse::Instance()->GetMouseY();

		if(pRenderUnit)
		{
			RECT * prcDest = pRenderUnit->GetViewRect();
			
			if(prcDest)
			{
				int iWidth	= prcDest->right - prcDest->left;
				int iHeight = prcDest->bottom - prcDest->top;

				prcDest->left	= iMouseX - (iWidth/2);
				prcDest->top	= iMouseY - (iHeight/2);
				prcDest->right	= prcDest->left + pRenderUnit->m_iWidth;
				prcDest->bottom	= prcDest->top + pRenderUnit->m_iHeight;
			}
		}

	}
}


VOID MouseHandler::Render()
{
	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	BOOL bAlphaTest = g_pSunRenderer->IsEnableAlphaTest();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->EnableAlphaTest(FALSE);


	if (m_pItemUnitRender)
		m_pItemUnitRender->Render();

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->EnableAlphaTest(bAlphaTest);
}

VOID MouseHandler::RollbackItemAtHand()
{
	if( this->IsExistItemAtHand() )
	{
        if (m_MouseItem.m_pRenderUnit)
        {
		    m_MouseItem.m_pRenderUnit->SetViewRect( &m_MouseItem.m_OldRect );
		    m_MouseItem.m_pRenderUnit->SetShow(TRUE);
            m_MouseItem.m_pRenderUnit->m_altNum = 0;
            m_MouseItem.m_pRenderUnit->m_isDiv = 0;
        }

		// 슬롯을 갱신해준다.
		// 현재 위치에 보이지 말아야 할 상황이 발생하기 때문에 호출
		refreshContainer(m_MouseItem.m_fromSlotIdx);	

		if ( m_MouseItem.m_pFromSlot->GetSlotType() == ST_QUICK ) 
		{
			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_LINK, m_MouseItem.m_pFromSlot);
		}
		else if (m_MouseItem.m_fromSlotIdx == SI_EQUIPMENT)
		{
			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_LINK, m_MouseItem.m_pFromSlot);
		}
		else
		{
			ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, m_MouseItem.m_pFromSlot);
		}
		
		initMouseItem();

        if (m_pItemUnitRender)
        {
		    m_pItemUnitRender->RemoveItemUnit(DEFAULT_MOUSE_ITEM_SERIAL);
        }
	}
}

VOID MouseHandler::RemoveItemAtHand()
{
	if (m_MouseItem.m_pFromSlot)
	{
		m_MouseItem.m_pFromBaseItemInfo = 
			ItemInfoParser::Instance()->GetItemInfo( m_MouseItem.m_pFromSlot->GetCode() );
		// 나중에 타입이 정의된다면 제대로 아이템에 따른 사운드 출력을 해야한다.

	}
	initMouseItem();

	SUN_ASSERT(m_pItemUnitRender);
	if (m_pItemUnitRender)
	{
		this->m_pItemUnitRender->RemoveItemUnit(DEFAULT_MOUSE_ITEM_SERIAL);
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID 
MouseHandler::AddItemAtHandWithItemDiv(DURATYPE divNum,
                                       SLOTIDX fromSlotIdx, 
                                       POSTYPE fromPos, 
                                       eSlotType slotType, 
                                       SLOTCODE code, 
                                       ItemUnitRenderer::RenderUnit *	pRenderUnit, 
                                       RECT * pRect)
{
    m_MouseItem.m_fromSlotIdx		= fromSlotIdx;
    m_MouseItem.m_fromPos			= fromPos;
    m_MouseItem.m_slotType			= slotType;
    m_MouseItem.m_OldRect			= *pRect;
    m_MouseItem.m_pRenderUnit		= pRenderUnit;
    m_MouseItem.m_pFromBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(code);

    if (m_MouseItem.m_pRenderUnit)
    {
        m_MouseItem.m_pRenderUnit->SetSize(pRect->right - pRect->left,  pRect->bottom - pRect->top);
        m_MouseItem.m_pRenderUnit->SetShow(TRUE);		// 아이템 나눌때는 보여주고
    }

    SCSlotContainer * pAtContainer	= ItemManager::Instance()->GetContainer( fromSlotIdx );
    if(pAtContainer && (!pAtContainer->IsEmpty( fromPos ))) 
    {
        m_MouseItem.m_pFromSlot = &pAtContainer->GetSlot( fromPos );
        // 나중에 타입이 정의된다면 제대로 아이템에 따른 사운드 출력을 해야한다.
        // RemoveItemAtHand 참조
        ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PICK, m_MouseItem.m_pFromSlot);
    }

    if ( m_MouseItem.m_pFromSlot && 
        ST_ITEM == m_MouseItem.m_pFromSlot->GetSlotType())
    {
        SCItemSlot * pFromItemSlot = (SCItemSlot *)m_MouseItem.m_pFromSlot;
        
		if( CheckBlockedItem( pFromItemSlot ))
		{
			return;
		}

		if ( pFromItemSlot->IsLocked() )
        {
            RollbackItemAtHand();
            // 특정 메시지 없음 말고.
            return;
        }
        
        m_MouseItem.m_pRenderUnit->m_altNum = pFromItemSlot->GetNum() - divNum;
        m_MouseItem.m_pRenderUnit->m_isDiv = true;
    }

    ItemUnitRenderer::RenderUnit *	pOrgRenderUnit = this->m_pItemUnitRender->AddItemUnit(
        DEFAULT_MOUSE_ITEM_SERIAL, 
        code, 
        pRect, 
        pRenderUnit->m_pRefSlot);

    pOrgRenderUnit->SetSize(pRect->right - pRect->left,  pRect->bottom - pRect->top);
    pOrgRenderUnit->m_altNum = divNum;
    pOrgRenderUnit->m_isDiv = true;

	uiBase * itemDiv = GET_DIALOG(IM_ITEMDIV_MANAGER::ITEMDIV);
	
    if (itemDiv)
    {
        itemDiv->ShowInterface(FALSE);
    }
}

//----------------------------------------------------------------------------
/**
*/
VOID 
MouseHandler::AddItemAtHand( 
								 SLOTIDX fromSlotIdx, 
								 POSTYPE fromPos, 
								 eSlotType slotType, 
								 SLOTCODE code, 
								 ItemUnitRenderer::RenderUnit *	pRenderUnit, 
								 RECT * pRect )
{
	m_MouseItem.m_fromSlotIdx		= fromSlotIdx;
	m_MouseItem.m_fromPos			= fromPos;
	m_MouseItem.m_slotType			= slotType;
	m_MouseItem.m_OldRect			= *pRect;
	m_MouseItem.m_pRenderUnit		= pRenderUnit;
	m_MouseItem.m_pFromBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(code);

    if (m_MouseItem.m_pRenderUnit)
    {
	    m_MouseItem.m_pRenderUnit->SetSize(pRect->right - pRect->left,  pRect->bottom - pRect->top);
	    m_MouseItem.m_pRenderUnit->SetShow(FALSE);		// 실제 유닛은 안보여주고. 
    }

	SCSlotContainer * pAtContainer	= ItemManager::Instance()->GetContainer( fromSlotIdx );
	if (pAtContainer && !pAtContainer->IsEmpty(fromPos)) 
	{
		m_MouseItem.m_pFromSlot = &pAtContainer->GetSlot( fromPos );
		// 나중에 타입이 정의된다면 제대로 아이템에 따른 사운드 출력을 해야한다.
		// RemoveItemAtHand 참조
       	ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PICK, m_MouseItem.m_pFromSlot);
	}

	if ( m_MouseItem.m_pFromSlot && 
		 ST_ITEM == m_MouseItem.m_pFromSlot->GetSlotType())
	{
		SCItemSlot * pFromItemSlot = (SCItemSlot *)m_MouseItem.m_pFromSlot;
		
		if( CheckBlockedItem( pFromItemSlot ))
		{
			return;
		}
		
		if ( pFromItemSlot->IsLocked() )
		{
			RollbackItemAtHand();
			// 특정 메시지 없음 말고.
			return;
		}
	}

	ItemUnitRenderer::RenderUnit *	pOrgRenderUnit = this->m_pItemUnitRender->AddItemUnit(
		DEFAULT_MOUSE_ITEM_SERIAL, 
		code, 
		pRect, 
		pRenderUnit->m_pRefSlot);

	pOrgRenderUnit->SetSize(pRect->right - pRect->left,  pRect->bottom - pRect->top);
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
MouseHandler::ItemTransaction(SLOTIDX SlotIdx, 
								   POSTYPE ToPos, RECT * pRect)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];


	if( IsExistItemAtHand() )
	{ 
		// 여기에 들어왔다는것은 특정 컨테이너에서 마우스에 아이템을 가져온 것이다.

		SCSlotContainer * pFromContainer = 
			ItemManager::Instance()->GetContainer(m_MouseItem.m_fromSlotIdx); 
		SCSlotContainer * pToContainer =
			ItemManager::Instance()->GetContainer(SlotIdx); 
        
        if ( ToPos >= pToContainer->GetMaxSlotNum() )
		{
			return FALSE;
		}
		
		switch( m_MouseItem.m_slotType )
		{
		case ST_ITEM:
			{
                if (m_MouseItem.m_pRenderUnit->m_isDiv)
                {
                    switch(SlotIdx)
                    {
                    case SI_INVENTORY: // no operation
                        {
                            if (m_MouseItem.m_fromSlotIdx == SI_WAREHOUSE ||
                                m_MouseItem.m_fromSlotIdx == SI_GUILDWAREHOUSE)
                            {
                                if (pToContainer && pToContainer->IsEmpty(ToPos))
                                {
                                    ItemManager::Instance()->SendItemMoveMsg(m_MouseItem.m_fromSlotIdx,
                                        SlotIdx, m_MouseItem.m_fromPos, ToPos,
                                        (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum));
                                    return TRUE;
                                }
                                else
                                {
                                    SCSlot & rFromSlot	= pFromContainer->GetSlot(m_MouseItem.m_fromPos);
					                SCSlot & rToSlot	= pToContainer->GetSlot(ToPos);
                    				
					                if ( rFromSlot.GetCode() && rToSlot.GetCode() 
						                && (rFromSlot.GetCode() == rToSlot.GetCode())
						                && SCItemSlot::IsOverlap(rFromSlot.GetCode())
						                )
                                    {
							            ItemManager::Instance()->SendItemCombineMsg(
								            m_MouseItem.m_fromSlotIdx, 
								            SlotIdx, 
								            m_MouseItem.m_fromPos,
								            ToPos,
                                            (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum)
								            );
                                        return TRUE;
                                    }
                                    else
                                    {
                                        return FALSE;
                                    }
                                }
                            }
                        }
                        break;
                    case SI_WAREHOUSE:
                    case SI_GUILDWAREHOUSE:
                        {   // 창고에 나누어서 넣기
                            if (m_MouseItem.m_fromSlotIdx != SlotIdx)
                            {
                                if(pToContainer && pToContainer->IsEmpty(ToPos))
                                {
                                    ItemManager::Instance()->SendItemMoveMsg(m_MouseItem.m_fromSlotIdx,
                                        SlotIdx, m_MouseItem.m_fromPos, ToPos,
                                        (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum));
                                    return TRUE;
                                }
                                else
                                {
                                    SCSlot & rFromSlot	= pFromContainer->GetSlot(m_MouseItem.m_fromPos);
					                SCSlot & rToSlot	= pToContainer->GetSlot(ToPos);
                					
					                if ( rFromSlot.GetCode() && rToSlot.GetCode() 
						                && (rFromSlot.GetCode() == rToSlot.GetCode())
						                && SCItemSlot::IsOverlap(rFromSlot.GetCode())
						                )
                                    {
							            ItemManager::Instance()->SendItemCombineMsg(
								            m_MouseItem.m_fromSlotIdx, 
								            SlotIdx, 
								            m_MouseItem.m_fromPos,
								            ToPos,
                                            (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum)
								            );
                                        return TRUE;
                                    }
                                    else
                                    {
                                        return FALSE;
                                    }
                                }
                            }
                            else
                            {
                                if (ToPos == m_MouseItem.m_fromPos)
                                {
                                    RollbackItemAtHand();
                                    return FALSE;
                                }
                                else
                                {
                                    // DB 문제로 창고에서는 아이템 분할 불가능
                                    // 988	창고에서는 아이템 분할이 불가능 합니다. 인벤토리에서 시도해 주세요
						            g_InterfaceManager.GetInterfaceString(988 , szMessage ,INTERFACE_STRING_LENGTH);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
                                    return FALSE;
                                }
                            }
                        }
                        break;
                    case SI_NPCSTORE:
                        {   // 상점에 나누어서 팔기
                            if (SI_INVENTORY == m_MouseItem.m_fromSlotIdx)// && SI_NPCSTORE == SlotIdx)
				            {
					            // 클라이언트에서는 아이템 거래 타입에 따른 체크 안함
					            // 인벤에서 상점으로 왔을때는 판매
					            ItemManager::Instance()->SendSellMsg(m_MouseItem.m_fromSlotIdx, m_MouseItem.m_fromPos, 
                                    (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum));
					            return TRUE;
				            }
                        }
                        break;
                    default:
                        {
                            // 5133	아이템 분할은 인벤토리에서만 가능합니다.
						    g_InterfaceManager.GetInterfaceString(eST_ITEM_DIV_ONLY_INVENTORY , szMessage ,INTERFACE_STRING_LENGTH);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
                            return FALSE;
                        }
                    }
                }

				// TODO :
				// 이부분 이쁘게 함수로 나눠서 관리하자..

				//////////////////////////////////////////////////////////////////////////
				// Shop container
                if (m_MouseItem.m_fromSlotIdx == SlotIdx && m_MouseItem.m_fromPos == ToPos)
				{
					RollbackItemAtHand();
				}
				else if (SI_NPCSTORE == m_MouseItem.m_fromSlotIdx && SI_NPCSTORE == SlotIdx)
				{
					RollbackItemAtHand();
					return FALSE;
				}
				else if(SI_EQUIPMENT == m_MouseItem.m_fromSlotIdx && SI_GUILDWAREHOUSE == SlotIdx)
				{
					RollbackItemAtHand();
					return FALSE;
				}
				else if (SI_INVENTORY == m_MouseItem.m_fromSlotIdx && SI_NPCSTORE == SlotIdx)
				{
					// 클라이언트에서는 아이템 거래 타입에 따른 체크 안함
					// 인벤에서 상점으로 왔을때는 판매
					ItemManager::Instance()->SendSellMsg(m_MouseItem.m_fromSlotIdx, m_MouseItem.m_fromPos);

					return TRUE;

				}
				else if (SI_NPCSTORE == m_MouseItem.m_fromSlotIdx && SI_INVENTORY == SlotIdx)
				{
					// 상점에서 인벤으로 왔을때는 구매

					SCSlot & rSlot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);

					ShopDialog * pShopDlg = (ShopDialog *)
						ItemManager::Instance()->GetItemContainerDialog(SI_NPCSTORE);

					DWORD dwShopID = (DWORD)pShopDlg->GetShopID();
					SHOPINFO * pShopInfo = (SHOPINFO *)ShopInfoParser::Instance()->GetShopList( dwShopID );

					SHOPINFO::SHOP_ITEM * pShopItem = pShopInfo->GetItem(rSlot.GetCode());
					if (!pShopItem)
					{
						assert(!"ShopInfo.txt 확인!!");
						return FALSE;
					}
						                    
					ItemManager::Instance()->SendBuyMsg(
						pShopDlg ->GetShopID(), 
						rSlot.GetCode(), 
						pShopDlg->GetCurrentTab(), 
						pShopDlg->GetRelativeSlotPos( m_MouseItem.m_fromPos) );

					RollbackItemAtHand();

					return TRUE;
				}
				else if (SI_EVENT_INVENTORY == SlotIdx)
				{
					// 이벤트 인벤토리에다가는 유저가 임의로 올려놓을수 없다.
					RollbackItemAtHand();
					return FALSE;
				}
				else if (SI_EVENT_INVENTORY == m_MouseItem.m_fromSlotIdx && SI_INVENTORY != SlotIdx)
				{
					// 이벤트 인벤토리에다가는 유저가 임의로 올려놓을수 없다.
					RollbackItemAtHand();
					return FALSE;
				}
				else if (SI_EVENT_INVENTORY == m_MouseItem.m_fromSlotIdx && SI_INVENTORY == SlotIdx)
				{
					EventInventoryDialog * pEventInventory = GET_CAST_DIALOG( EventInventoryDialog, IM_INVENTORY_MANAGER::EVENTINVEN_DLG );
					assert( pEventInventory );
					if ( pEventInventory )
					{
						pEventInventory->SendItemMoveMsg(m_MouseItem.m_fromPos);
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// EquipContainer 
				else if (SI_EQUIPMENT == m_MouseItem.m_fromSlotIdx && SI_INVENTORY == SlotIdx)
				{					 

					BOOL bUnEquip = ItemManager::Instance()->CanMoveUnEquipItem(
						m_MouseItem.m_fromSlotIdx, 
						m_MouseItem.m_fromPos, 
						TRUE);

					if (!bUnEquip)
					{
						MouseHandler::Instance()->RollbackItemAtHand();
						return FALSE;
					}
					else
					{
						ItemManager::Instance()->SendItemMoveMsg( 
							m_MouseItem.m_fromSlotIdx, 
							SlotIdx, 
							m_MouseItem.m_fromPos, 
							ToPos 
							);
					}
				}
				else if (SI_EQUIPMENT == SlotIdx && SI_INVENTORY == m_MouseItem.m_fromSlotIdx )
				{
					// 이큅가능한가?
					BOOL bEquipRet = 
						ItemManager::Instance()->CanEquip( m_MouseItem.m_fromSlotIdx, m_MouseItem.m_fromPos, ToPos );

					if ( TRUE == bEquipRet )
					{
						SCItemSlot & rItemSlot = (SCItemSlot &)pFromContainer->GetSlot(m_MouseItem.m_fromPos);
						const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
						if( !pItemInfo )
						{
							return FALSE;
						}
						
						if ( rItemSlot.IsPossessionItem() )
						{// 귀속 아이템 사용 
							if ( rItemSlot.GetIdentifyPossession() == 0 )
							{//귀속아이템 처음 사용
								//사용할건지 팝업(귀속 인증)
								if(	GetPossessionWin() != true )
								{
									//5090	장착시 귀속됩니다. 장착하시겠습니까?
									TCHAR szBuf[INTERFACE_STRING_LENGTH];
									g_InterfaceManager.GetInterfaceString( 5090 , szBuf, INTERFACE_STRING_LENGTH-1);
									
									uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
									if(pSystemMan)
									{
										pSystemMan->Sys_Confirm(NULL, GlobalFunc::UsePossessionItem, NULL/*szTitle*/, szBuf);
										SetPossessionItemPOS(m_MouseItem.m_fromPos, ToPos);
										SetPossessionWin(true);
									}
									return FALSE;
								}
								else
								{
									// 6130: 한번에 하나씩 귀속 할 수 있습니다.
									TCHAR szBuf[INTERFACE_STRING_LENGTH];
									g_InterfaceManager.GetInterfaceString( 6130 , szBuf, INTERFACE_STRING_LENGTH-1);
									GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szBuf );
								}
							}
						}					


						ItemManager::Instance()->SendItemMoveMsg( 
							m_MouseItem.m_fromSlotIdx, 
							SlotIdx, 
							m_MouseItem.m_fromPos, 
							ToPos 
							);
					}
					else
					{

						g_HeroInput.PlayLimitUseSound();
						RollbackItemAtHand();
						return FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				
				//////////////////////////////////////////////////////////////////////////
				// Guild WareHouse
				else if(SI_GUILDWAREHOUSE == SlotIdx && SI_INVENTORY == m_MouseItem.m_fromSlotIdx)
				{
						SCItemSlot & rItemSlot = (SCItemSlot &)pFromContainer->GetSlot(m_MouseItem.m_fromPos);
						if( rItemSlot.IsPossessionItem() && rItemSlot.GetIdentifyPossession() )
						{
							RollbackItemAtHand();
							ItemManager::Instance()->NoticeReasonItemErrorCode(RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM);
							return FALSE;
						}
					ItemManager::Instance()->SendItemMoveMsg( 
						SI_INVENTORY, 
						SI_GUILDWAREHOUSE, 
						m_MouseItem.m_fromPos, 
						ToPos 
						);
				}
				else if(SI_GUILDWAREHOUSE == m_MouseItem.m_fromSlotIdx && SI_INVENTORY == SlotIdx)
				{
					ItemManager::Instance()->SendItemMoveMsg( 
						SI_GUILDWAREHOUSE, 
						SI_INVENTORY, 
						m_MouseItem.m_fromPos, 
						ToPos 
						);
				}

				//라이더-->인벤(UNEQUIP)
				else if(SI_RIDER == m_MouseItem.m_fromSlotIdx && SI_INVENTORY == SlotIdx)
				{
                    //빈슬롯이야된다
                    if(pToContainer && pToContainer->IsEmpty(ToPos))
                    {
                        RiderInventoryDialog *pInven=(RiderInventoryDialog *)pFromContainer;
                        MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN SendMsg;
                        SendMsg.header.request = SendMsg.header.eReq_UnEquipParts;
                        SendMsg.header.args.unequip.from.value = m_MouseItem.m_fromPos+eRIDER_PARTS_POS::StartIndex;
                        SendMsg.header.args.unequip.to         = ToPos;
                        if(pInven)
                        {
                            SendMsg.header.args.unequip.where      = pInven->GetRiderSummonPos();
                        }
                        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
                    }
                    else
                    {
                        //	해당 슬롯이 비어있지않습니다.
                        g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_NOT_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                    }
                    
                    RollbackItemAtHand();

				}//인벤(INVEN)-->라이더(EQUIP)
				else if(SI_INVENTORY == m_MouseItem.m_fromSlotIdx && SI_RIDER == SlotIdx)
				{
                    RiderInventoryDialog *pInven=(RiderInventoryDialog *)pToContainer;

                    //라이더아이템
                    SCItemSlot & rRiderSlot = (SCItemSlot &)pFromContainer->GetSlot(pInven->GetRiderSummonPos());

                    if(!g_pHero->IsRiding() || !rRiderSlot.IsContainValidItem())
                    {
                        assert(!"Not Summon Rider");
                        RollbackItemAtHand();
                        break;
                    }

                    //라이더 파츠아이템
                    SCItemSlot & rItemSlot = (SCItemSlot &)pFromContainer->GetSlot(m_MouseItem.m_fromPos);
                
                    RC::eITEM_RESULT result = rRiderSlot.CanEquipRiderParts(rItemSlot);

                    switch(result)
                    {
                    case RC::RC_ITEM_SUCCESS:
                        {
                            //장착한다.
                            MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN SendMsg;
                            SendMsg.header.request = SendMsg.header.eReq_EquipParts;
                            SendMsg.header.args.equip.from = m_MouseItem.m_fromPos;
                            SendMsg.header.args.equip.to.value   =ToPos+eRIDER_PARTS_POS::StartIndex;
                            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
                        }
                        break;
                    case RC::RC_ITEM_INVALID_TYPE:
                        {
                            //6440:소환한 탈것과 맞지않는아이템입니다.
                            g_InterfaceManager.GetInterfaceString(6440,szMessage);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        }
                        break;
                    case RC::RC_ITEM_CAN_NOT_EQUIP:
                        {
                            //6440 해당 탈것에 장착할수 없는 장비입니다.
                            g_InterfaceManager.GetInterfaceString(6440,szMessage);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        }
                        break;
                    case RC::RC_ITEM_INVALIDPOS:
                        {
                            //6453 탈것 장비위치가 올바르지않습니다.
                            g_InterfaceManager.GetInterfaceString(6453,szMessage);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        }
                        break;
                    }
                       
                    RollbackItemAtHand();
    			}

				//////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// quickcontainer
				else if (SI_QUICK == SlotIdx)
				{
					// 퀵으로 링크

					if (SI_INVENTORY != m_MouseItem.m_fromSlotIdx)
					{
						// 장착 아이템은 인벤토리에서만 퀵으로 등록 할 수 있다.

						MouseHandler::Instance()->RollbackItemAtHand();
						return FALSE;
					}
					
					BOOL bQuick = ItemManager::Instance()->CanLinkQuick(SI_QUICK, ToPos, m_MouseItem.m_pFromSlot);
					if (bQuick)
					{
						ItemManager::Instance()->SendQuickItemLinkMsg( 
							m_MouseItem.m_fromPos, 
							ToPos);
					}
					else
					{
						RollbackItemAtHand();
						return FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				else if ( SI_STYLE == SlotIdx )
				{
					//	스타일 퀵 창에는 스타일만 올려놓을 수 있습니다.
					g_InterfaceManager.GetInterfaceString( eST_STYLE_ONLY_STYLE_QUICK_SLOT, szText, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szText );
					RollbackItemAtHand();

					return FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
                // trade
                else if (SI_TRADE == SlotIdx && SI_INVENTORY == m_MouseItem.m_fromSlotIdx)
                {
					//// 클라이언트에서는 아이템 거래 타입에 따른 체크 안함

					uiTradeMan* tradeMan =
						static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
					assert (tradeMan);

					if (tradeMan)
					{
						tradeMan->TEST_Put(m_MouseItem.m_fromPos);
					}
                }
				else if (SI_TRADE == m_MouseItem.m_fromSlotIdx) 
				{
					switch (SlotIdx) 
					{
					case SI_TRADE:
						RollbackItemAtHand();
						break;

					default:
						{
							uiTradeMan* tradeMan =
							static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
							assert (tradeMan);

							if (tradeMan)
							{
								tradeMan->TEST_Get(m_MouseItem.m_fromPos);
							}			
						}
						break;
					}
				}	
                //------------------------------------------------------------------------------
                //! 큐브
                else if (SlotIdx == SI_CUBE && SI_INVENTORY == m_MouseItem.m_fromSlotIdx)
                {
                    uiItemCube* dialog = GET_CAST_DIALOG(uiItemCube, IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE);
                    if (dialog != NULL)
                    {
                        SCSlot& toSlot = dialog->GetSlot(ToPos);
                        SCSlot& slot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);
                        //if (dialog->CanLinkSlot(&slot, ToPos) == true)
                        {
                            if (this->m_MouseItem.m_fromSlotIdx == SI_INVENTORY)
                            {
                                // 인벤에서만 올릴 수 있도록 설정
                                bool bAlreadyUpdated = false;
                                // check already uploaded
                                for (int i=0; i<dialog->GetMaxSlotNum(); ++i)
                                {
                                    if (!dialog->IsEmpty(i))
                                    {
                                        SCSlot& _slot = pToContainer->GetSlot(i);
                                        if( _slot.GetSerial() == slot.GetSerial() )
                                        {
                                            // 이미 올라갔다.
                                            bAlreadyUpdated = true;
                                        }
                                    }
                                }
                                if( g_pHero && g_pHero->GetCharInfo()->m_iInventoryLock == PLAYERINFO_FOR_PACKET::DISABLEDnEXIST)
                                {
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5571));
                                }
                                else
                                {
                                    ItemManager::Instance()->CopyItem(m_MouseItem.m_fromSlotIdx,
                                        SI_CUBE, m_MouseItem.m_fromPos,ToPos);
                                }
                            }
                        }
                    }
                    RollbackItemAtHand();
                    return TRUE;
                }
                //////////////////////////////////////////////////////////////////////////
                // vendor sell
                else if (SI_VENDOR_SELL == SlotIdx && SI_INVENTORY == m_MouseItem.m_fromSlotIdx)
                {
				
					// 클라이언트에서는 아이템 거래 타입에 따른 체크 안함

                    // 인벤토리에서 가져왔을 경우
                    
                    // pToContainer 이상의 기능을 원하므로 확장된 포인터인 원래클래스 포인터로 처리한다.

					uiVendorSell* vendorSell = GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

                    SCSlot& toSlot      = vendorSell->GetSlot(ToPos);
                    SCSlot& fromSlot	= pFromContainer->GetSlot(m_MouseItem.m_fromPos);

                    if (toSlot.GetCode())
                    {
                        // 슬롯에 채워져 있는 아이템을 클릭했을 경우

                        if (this->IsExistItemAtHand())
                            this->RollbackItemAtHand();

                        uiSCItemSlot& toItemSlot = (uiSCItemSlot&)toSlot;

                        if (toItemSlot.CanEquip())
                        {
                            vendorSell->SetCurrentItemSlotInfo(1,
                                                                toItemSlot.GetMoney(),
                                                                ToPos);
                        }
                        else
                        {
                            vendorSell->SetCurrentItemSlotInfo(toItemSlot.GetNum(),
                                                                toItemSlot.GetMoney(),
                                                                ToPos);
                        }
                    }
                    else
                    {
                        if (this->IsExistItemAtHand())
                        {
                            if (vendorSell->IsVending())
                            {
                                // 개인 상점 시작 후 새로 올리는 아이템

                                this->RollbackItemAtHand();
                            }
                            else
                            {
                                // 개인 상점 시작 전 새로 올리는 아이템

                                if (this->m_MouseItem.m_fromSlotIdx == SI_INVENTORY)
                                {
                                    // 인벤에서만 올릴 수 있도록 설정

                                    bool bAlreadyUpdated = false;

                                    // check already uploaded
                                    for (int i=0; i<vendorSell->GetMaxSlotNum(); ++i)
                                    {
                                        if (!vendorSell->IsEmpty(i))
                                        {
                                            SCSlot& _slot = pToContainer->GetSlot(i);
                                            if( _slot.GetSerial() == fromSlot.GetSerial() )
                                            {
                                                // 이미 올라갔다.
                                                bAlreadyUpdated = true;
                                            }
                                        }
                                    }

									if (!bAlreadyUpdated)
                                    {
                                        // 아이템 가격 올리기 창 띄우기

										if( g_pHero && g_pHero->GetCharInfo()->m_iInventoryLock == PLAYERINFO_FOR_PACKET::DISABLEDnEXIST)
										{
											GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5571));
										}
										else
										{
											uiVendorPopup *vendorPopup = GET_CAST_DIALOG ( uiVendorPopup, IM_VENDOR_MANAGER::VENDOR_POPUP );

											vendorPopup->ShowInterface(TRUE);
											vendorPopup->SetInfos(m_MouseItem.m_fromSlotIdx,
												SI_VENDOR_SELL,
												m_MouseItem.m_fromPos,
												ToPos);
											vendorPopup->SetItemSlot((SCItemSlot*)&toSlot);

											// 일단 개인상점 슬롯에 꽂아놓고 가격책정

											ItemManager::Instance()->CopyItem(m_MouseItem.m_fromSlotIdx,
												SI_VENDOR_SELL,
												m_MouseItem.m_fromPos,
												ToPos);

											g_InterfaceManager.AttachFirstDialog(InterfaceManager::DIALOG_VENDOR_POPUP);
										}

                                        // 마우스에 들고 있던 아이템은 슬롯에 꽂아 놓았으므로 내려놓기
                                        this->RollbackItemAtHand();
                                    }
                                    else
                                    {
                                        this->RollbackItemAtHand();
                                    }
                                }
                            }
                        }
                    }
                }
                //////////////////////////////////////////////////////////////////////////
                // vendor sell
                else if (SI_VENDOR_SELL == SlotIdx && SI_EQUIPMENT == m_MouseItem.m_fromSlotIdx)
                {
                    this->RollbackItemAtHand();

					//	장착중인 아이템은 판매할 수 없습니다.
					const int	c_iNotSellEqItem = 70418;
                    g_InterfaceManager.GetInterfaceString( c_iNotSellEqItem, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );
                }

                //------------------------------------------------------------------------------
                //! 자동 사냥
#ifdef _YMS_AUTOBOT_GAME
                else if (SlotIdx == SI_AUTOMATIC_INFO)
                {
                    uiAutobotConditionBattle* dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, 
                                                            uiAutobotConditionBattle::kDialog_ID);
                    if (dialog != NULL)
                    {
                        SCSlot& slot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);

                        if (dialog->CanLinkSlot(&slot, ToPos) == true)
                        {
                            ItemManager::Instance()->AutobotLinkItem(m_MouseItem.m_fromSlotIdx, 
                                                                        m_MouseItem.m_fromPos, 
                                                                        ToPos, 
                                                                        slot.GetCode());
                        }
                    }

                    RollbackItemAtHand();
                    return TRUE;
                }
#endif //_YMS_AUTOBOT_GAME
                else if (SlotIdx == SI_PERK && m_MouseItem.m_fromSlotIdx == SI_INVENTORY)
                {
                    SCItemSlot* item_slot = static_cast<SCItemSlot*>(m_MouseItem.m_pFromSlot);
                    if (item_slot == NULL)
                    {
                        return FALSE;
                    }

                    const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
                    if (item_info == NULL)
                    {
                        return FALSE;
                    }

                    if (item_info->m_wType == eITEMTYPE_PERK_CASH_ITEM)
                    {
                        ItemUseTransactionFromInventoryByItemTpye(SI_INVENTORY, m_MouseItem.m_fromPos);
                    }
                    
                    RollbackItemAtHand();

                    return TRUE;
                }
                //////////////////////////////////////////////////////////////////////////
                // 인벤 -> 지능형NPC창
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
                else if (SI_INVENTORY == m_MouseItem.m_fromSlotIdx && SI_SMARTNPC == SlotIdx)
                {
                    uiSmartNpcDialogReply* dialog = GET_CAST_DIALOG(uiSmartNpcDialogReply, IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
                    if (dialog && dialog->IsVisibleWindow() &&
                        dialog->GetSubType() == SmartDialogSubType_SlotInput)
                    {
                        SCSlot& toSlot = dialog->GetSlot(ToPos);
                        SCSlot& slot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);
                        if (this->m_MouseItem.m_fromSlotIdx == SI_INVENTORY)
                        {
                            ItemManager::Instance()->CopyItem(m_MouseItem.m_fromSlotIdx,
                                SI_SMARTNPC, m_MouseItem.m_fromPos,ToPos);

                        }
                    }
                    RollbackItemAtHand();

                    return TRUE;
                }
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
				//////////////////////////////////////////////////////////////////////////
				// default transaction
				else
				{
					SCSlot & rFromSlot	= pFromContainer->GetSlot(m_MouseItem.m_fromPos);
					SCSlot & rToSlot	= pToContainer->GetSlot(ToPos);
					
					if ( rFromSlot.GetCode() && rToSlot.GetCode() 
						&& (rFromSlot.GetCode() == rToSlot.GetCode())
						&& SCItemSlot::IsOverlap(rFromSlot.GetCode())
						)
					{
						// 비워있지않고 코드가 같다

						SCItemSlot & rFromItemSlot = (SCItemSlot & )rFromSlot;
						SCItemSlot & rToItemSlot   = (SCItemSlot & )rToSlot;

						BOOL bIsCombineState = (!rFromItemSlot.IsBlocked()
												&&	!rFromItemSlot.IsLocked()
												&&	!rToItemSlot.IsBlocked()
												&&	!rToItemSlot.IsLocked());

						if ( bIsCombineState && rFromItemSlot.GetMaxNum() != rToItemSlot.GetNum())
						{
                            DURATYPE item_count = 0;
                            if (m_MouseItem.m_pRenderUnit->m_isDiv)
                            {
                                item_count = (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum);
                            }
							ItemManager::Instance()->SendItemCombineMsg(
								m_MouseItem.m_fromSlotIdx, 
								SlotIdx, 
								m_MouseItem.m_fromPos,
                                ToPos,
                                item_count );
						}
						else
						{
							RollbackItemAtHand();
						}
					}
					else if (m_MouseItem.m_pRenderUnit->m_isDiv)
                    {
                        
                        ItemManager::Instance()->SendItemMoveMsgWithDiv( 
                            m_MouseItem.m_fromSlotIdx, 
                            SlotIdx, 
                            m_MouseItem.m_fromPos, 
                            ToPos,
                            m_MouseItem.m_pFromSlot->GetNum() - (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum),
                            (m_MouseItem.m_pFromSlot->GetNum() - m_MouseItem.m_pRenderUnit->m_altNum));
                    }
                    else
					{
						ItemManager::Instance()->SendItemMoveMsg( 
							m_MouseItem.m_fromSlotIdx, 
							SlotIdx, 
							m_MouseItem.m_fromPos, 
							ToPos 
							);
					}
				}
				//////////////////////////////////////////////////////////////////////////
			}
			break;

		case ST_QUICK:
			{
#ifdef _YMS_AUTOBOT_GAME
                if (m_MouseItem.m_fromSlotIdx == SI_QUICK && SlotIdx == SI_QUICK)
                {
                    if (m_MouseItem.m_fromPos == ToPos)
                    {
                        // 같은위치면 롤백
                        MouseHandler::Instance()->RollbackItemAtHand();
                        return FALSE;
                    }
                    else
                    {
                        ItemManager::Instance()->SendSimplexMoveLinkMsg(m_MouseItem.m_fromPos, ToPos);
                        return TRUE;
                    }
                }
                else if (m_MouseItem.m_fromSlotIdx == SI_AUTOMATIC_INFO && SlotIdx == SI_AUTOMATIC_INFO)
                {
                    if (m_MouseItem.m_fromPos == ToPos)
                    {
                        // 같은위치면 롤백
                        MouseHandler::Instance()->RollbackItemAtHand();
                        return FALSE;
                    }
                    else
                    {
                        uiAutobotConditionBattle* dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
                        if (dialog != NULL)
                        {
                            SCSlot& slot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);

                            if (dialog->CanLinkSlot(&slot, ToPos) == true)
                            {
                                ItemManager::Instance()->SimplexMoveLinkItem(SI_AUTOMATIC_INFO, m_MouseItem.m_fromPos, ToPos);
                            }
                        }
                        MouseHandler::Instance()->RollbackItemAtHand();
                        return TRUE;
                    }
                }
                else
                {
                    if (m_MouseItem.m_fromSlotIdx == SI_QUICK)
                    {
                        // 퀵 아이템 Unlink
                        ItemManager::Instance()->SendQuickItemRemoveMsg(m_MouseItem.m_fromPos);
                        return TRUE;
                    }
                    else if (m_MouseItem.m_fromSlotIdx == SI_AUTOMATIC_INFO)
                    {
                        //! 언링크
                        ItemManager::Instance()->GetContainer(m_MouseItem.m_fromSlotIdx)->DeleteSlot(m_MouseItem.m_fromPos, NULL);	
                        MouseHandler::Instance()->RemoveItemAtHand();
                        return TRUE;
                    }
                }

#else
				if (SI_QUICK == SlotIdx)
				{
					if (m_MouseItem.m_fromPos == ToPos)
					{
						// 같은위치면 롤백
						MouseHandler::Instance()->RollbackItemAtHand();
						return FALSE;
					}
					else
					{
						ItemManager::Instance()->SendSimplexMoveLinkMsg( 
							m_MouseItem.m_fromPos, 
							ToPos);
						return TRUE;
					}
				}
				else
				{
					// 퀵 아이템 Unlink
					ItemManager::Instance()->SendQuickItemRemoveMsg( m_MouseItem.m_fromPos );
				}
#endif //_YMS_AUTOBOT_GAME

			}
			break;

		case ST_STYLE_QUICK:
			{
                if (SlotIdx != SI_INVENTORY)
                {
                    return FALSE;
                }
               
                SCPerkSlot* perk_slot = static_cast<SCPerkSlot*>(m_MouseItem.m_pFromSlot);
                const PerkInfo* perk_info = perk_slot->perk_info();
                if ((perk_info != NULL) &&
                    (perk_info->item_code_ == 0))
                {
                    return FALSE;
                }

                BOOL is_empty_slot = pToContainer->IsEmpty(ToPos);
                if (is_empty_slot == FALSE)
                {
                    return FALSE;
                }

                uiPerkDialog* perk_dialog = static_cast<uiPerkDialog*>(pFromContainer);
                if (perk_dialog != NULL)
                {
                    perk_dialog->RequestPerkOperaion(PerkControl::eReq_Uproot, 
                                                     m_MouseItem.m_fromPos, 
                                                     ToPos);
                    RollbackItemAtHand();
                }                

                return TRUE;
                
				
			}
			break;


		case ST_SKILL:
            {
				// 스킬아이템이다.

                    // TODO :
                // 이부분 이쁘게 함수로 나눠서 관리하자..

                //////////////////////////////////////////////////////////////////////////
                // Shop container
                if (m_MouseItem.m_fromSlotIdx == SlotIdx && m_MouseItem.m_fromPos == ToPos)
                {
                    MouseHandler::Instance()->RollbackItemAtHand();
                }
                //////////////////////////////////////////////////////////////////////////

                //////////////////////////////////////////////////////////////////////////
                // quickcontainer
                else if (SI_QUICK == SlotIdx)
                {
                    // 퀵으로 링크

					BOOL bQuick = ItemManager::Instance()->CanLinkQuick(SI_QUICK, ToPos, m_MouseItem.m_pFromSlot);
                    if (bQuick)
                    {
                        ItemManager::Instance()->SendQuickSkillLinkMsg( 
                            m_MouseItem.m_pFromSlot->GetCode(), 
                            ToPos);
                    }
                    else
                    {
                        RollbackItemAtHand();
						return FALSE;
                    }
                }
#ifdef _YMS_AUTOBOT_GAME
                else if (SlotIdx == SI_AUTOMATIC_INFO)
                {
                    uiAutobotConditionBattle* dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
                    if (dialog != NULL)
                    {
                        SCSlot& slot = pFromContainer->GetSlot(m_MouseItem.m_fromPos);

                        if (dialog->CanLinkSlot(&slot, ToPos) == true)
                        {
                            ItemManager::Instance()->AutobotLinkSkill(ToPos, slot.GetCode());
                        }
                    }

                    RollbackItemAtHand();
                    return TRUE;
                }
#endif //_YMS_AUTOBOT_GAME
				else if (SI_STYLE == SlotIdx)
				{
					return FALSE;			
				}
                //////////////////////////////////////////////////////////////////////////

                //////////////////////////////////////////////////////////////////////////
                // default transaction
                else
                {
                    ItemManager::Instance()->SendItemMoveMsg( 
                        m_MouseItem.m_fromSlotIdx, 
                        SlotIdx, 
                        m_MouseItem.m_fromPos, 
                        ToPos 
                        );
                }
                //////////////////////////////////////////////////////////////////////////
            }
			break;


		default:
			{
				assert( !"잘못된 타입의 슬롯을 단축창에 이동했습니다." );
			}
		}

	}
	else
	{
		// 처음 클릭

		SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
		assert(pAtContainer);

		SolarDialog * pContainerDlg = ItemManager::Instance()->GetItemContainerDialog(SlotIdx);
		assert(pContainerDlg);

		if( pAtContainer == NULL || pContainerDlg == NULL )
			return FALSE;

		if ( ToPos >= pAtContainer->GetMaxSlotNum() )
		{
			return FALSE;
		}

		if ( pAtContainer->IsEmpty(ToPos))
		{
			return FALSE;
		}

		SCSlot & rSlot = pAtContainer->GetSlot(ToPos);

		eSlotType eTempSlotType = rSlot.GetSlotType();

		switch( eTempSlotType  )
		{
		case ST_ITEM:
			{
				if ( SI_NPCSTORE == SlotIdx )
				{
					if (GetMouseState() == BUY)
					{
						// 아이디이고 몇번째 
						// 아이템이 몇번째 인덱스인가?

						SCItemSlot & rSlot1 = (SCItemSlot &)pAtContainer->GetSlot(ToPos);

						ShopDialog * pShopDlg = static_cast<ShopDialog * >(pContainerDlg);

						DWORD dwShopID = (DWORD)pShopDlg->GetShopID();
						SHOPINFO * pShopInfo = (SHOPINFO *)ShopInfoParser::Instance()->GetShopList( dwShopID );

						SHOPINFO::SHOP_ITEM * pShopItem = pShopInfo->GetItem(rSlot1.GetCode());
						if (!pShopItem)
						{
							assert(!"ShopInfo.txt 확인!!");
							return FALSE;
						}
						
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
						if (GlobalFunc::HeimTradingvolumeCheck(rSlot1.GetPriceForBuy()))
						{
							uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
							if (ui_system_manager_ptr)
							{
								ui_system_manager_ptr->Sys_SystemHeimTradingvolumeNpcStore(
									pShopDlg->GetDialogKey(), 
									1, 
									SlotIdx, 
									ToPos, 
									eTempSlotType, 
									rSlot1.GetCode(), rSlot1.GetPriceForBuy());
							}
						}
						else
						{
							ItemManager::Instance()->SendBuyMsg(
								pShopDlg ->GetShopID(), 
								rSlot1.GetCode(), pShopDlg->GetCurrentTab(), pShopDlg->GetRelativeSlotPos(ToPos) );
						}
#else
						ItemManager::Instance()->SendBuyMsg(
							pShopDlg ->GetShopID(), 
							rSlot1.GetCode(), pShopDlg->GetCurrentTab(), pShopDlg->GetRelativeSlotPos(ToPos) );
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

						return TRUE;
					}

				}
				else if ( SI_EQUIPMENT == SlotIdx || SI_INVENTORY == SlotIdx)
				{  
					// 수리
					if (GetMouseState() == REPAIR)
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH];
						TCHAR szItemName[INTERFACE_STRING_LENGTH];

						SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;

						g_InterfaceManager.GetItemName( 
							rItemSlot.GetCode(), 
							szItemName, 
							INTERFACE_STRING_LENGTH);                        					

						TCHAR szTemp[INTERFACE_STRING_LENGTH];
						if ((rItemSlot.IsContainValidItem() == false) ||
                            (rItemSlot.WithDura() == false) ||								// 내구도가 없는 아이템
                            (rItemSlot.IsLimited() && (rItemSlot.GetLimitedDuraMax() <= 2)) // 수리할 수 없는 리미티드 아이템
                           )
						{
							// 수리 할 수 없는 아이템 입니다. 5130
							g_InterfaceManager.GetInterfaceString( eST_NOT_REPAIR_STATUS , szTemp ,INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_Notice(
								eOUTPUT_MSG_CHAT, szTemp );
							return FALSE;
						}

						SetPendingRepairSlotIdx(SlotIdx);
						SetPendingRepairPos( ToPos );
						SetPendingRepairAll( FALSE );
						// [%s] 를 수리 하시겠습니까?#%I64u 하임이 소요됩니다. 5131
						g_InterfaceManager.GetInterfaceString( eST_IS_REPAIR_NEED_HAIM , szTemp ,INTERFACE_STRING_LENGTH) ;
                        TCHAR repair_money_string[INTERFACE_STRING_LENGTH] = "";
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                        // 길드 시설 효과 여부
                        if (g_pHero && 
                            g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_REPAIR_HEIM_RATIO) != 0 && 
                            rItemSlot.GetPriceForRepair() > 0)
                        {
                            MONEY repair_need_money = rItemSlot.GetPriceForRepair();
                            int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_REPAIR_HEIM_RATIO);
                            bool is_plus = false;
                            if (ratio > 0)
                                is_plus = true;
                            ratio = abs(ratio);
                            MONEY percent_value = repair_need_money / 100 * ratio;
                            if (is_plus)
                            {
                                repair_need_money += percent_value;
                            }
                            else
                            {
                                repair_need_money -= percent_value;
                            }
                            GlobalFunc::SetPointText(repair_money_string, repair_need_money);
                        }
                        else
                        {
                            GlobalFunc::SetPointText(repair_money_string, rItemSlot.GetPriceForRepair());
                        }
#else
                        GlobalFunc::SetPointText(repair_money_string, rItemSlot.GetPriceForRepair());
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                        // [%s] 를 수리 하시겠습니까?%s 하임이 소요됩니다. 5131
                        Sprintf(
							szMessage, 
							szTemp, 
							szItemName, 
							repair_money_string);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage, GlobalFunc::RepairConfirm);

						return TRUE;
					}
					else if (GetMouseState() == SELL && SI_INVENTORY == SlotIdx)
					{
				
						// 클라이언트에서는 아이템 거래 타입에 따른 체크 안함
						ItemManager::Instance()->SendSellMsg(SlotIdx, ToPos);

						return TRUE;
					}
					else if (GetMouseState() == DEPOSIT && SI_INVENTORY == SlotIdx)
					{	// 입고
						POSTYPE EmptyPos = 0xFF;
						BOOL bRet = ItemManager::Instance()->GetEmptySlotPos(SI_WAREHOUSE, EmptyPos);
						if (bRet)
						{
							ItemManager::Instance()->SendItemMoveMsg(
								SI_INVENTORY, 
								SI_WAREHOUSE, 
								ToPos, 
								EmptyPos
							);
						}						
						
						return TRUE;
					}
					else if(GetMouseState() == GUILD_DEPOSIT && SI_INVENTORY == SlotIdx)
					{	//길드창고 입고
						POSTYPE EmptyPos = 0xFF;
						BOOL bRet = ItemManager::Instance()->GetEmptySlotPos(SI_GUILDWAREHOUSE, EmptyPos);
						if (bRet)
						{
							SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer(SlotIdx); 
							SCItemSlot & rItemSlot = (SCItemSlot &)pFromContainer->GetSlot(ToPos);

							if( rItemSlot.IsPossessionItem() && rItemSlot.GetIdentifyPossession() )
							{								
								ItemManager::Instance()->NoticeReasonItemErrorCode(RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM);
								return FALSE;
							}
							ItemManager::Instance()->SendItemMoveMsg(
								SI_INVENTORY, 
								SI_GUILDWAREHOUSE, 
								ToPos, 
								EmptyPos
								);
						}						

						return TRUE;
					}

					else if ( GetMouseState() == THROW_WASTE_BASKET && SI_INVENTORY == SlotIdx )
					{
						if ( IsLockWasteDeleteItem() )
						{
							return FALSE;							
						}

						LockWasteDeleteItem();

						ItemManager::Instance()->SetPendingDeleteItemPos(ToPos);

						TCHAR	szItemName[INTERFACE_STRING_LENGTH];
						TCHAR	szResult[INTERFACE_STRING_LENGTH];						
						TCHAR	szResultMsg[INTERFACE_STRING_LENGTH];						

						BOOL bExgist = g_InterfaceManager.GetItemName(
							rSlot.GetCode(), 
							szItemName, 
							INTERFACE_STRING_LENGTH );

						//5125	[%s] 아이템을 삭제하시겠습니까?
						g_InterfaceManager.GetInterfaceString(5125, szResult, INTERFACE_STRING_LENGTH);
						
						//예외처리:정상적으로 아이템이름을 세팅못했을경우
						if(!bExgist)
						{
						
						#ifdef _DEBUG
							Snprintf (szItemName, INTERFACE_STRING_LENGTH-1, _T("DebugMsg:Could Not Find ItemCode: %d"),rSlot.GetCode());
							GlobalFunc::default_OutputMsg(eOUTPUT_MSG_CHAT,szItemName);
						#endif//_DEBUG(디버그에서만체크)

							Snprintf (szItemName, INTERFACE_STRING_LENGTH-1, _T("NULL"));
						}

						Snprintf( szResultMsg, INTERFACE_STRING_LENGTH, szResult, szItemName );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
						{
							m_WasteBasketDialogID = pSystemMan->Sys_Confirm(szResultMsg, GlobalFunc::DeleteItemConfirm);
						}

						return TRUE;
					}

				}
                else if ( SI_TRADE == SlotIdx )
                {
                    if (m_MouseItem.m_slotType == ST_NONE)
                    {
                        return TRUE;
                    }

                    if (m_MouseItem.m_fromPos >= 10)
                    {
                        return TRUE;
                    }
                }
				else if ( SI_WAREHOUSE == SlotIdx )
				{
					// 출고
					if (GetMouseState() == WITHDRAW)
					{
						POSTYPE EmptyPos = 0xFF;
						BOOL bRet = ItemManager::Instance()->GetEmptySlotPos(SI_INVENTORY, EmptyPos);
						if (bRet)
						{
							ItemManager::Instance()->SendItemMoveMsg(
								SI_WAREHOUSE, 
								SI_INVENTORY, 
								ToPos, 
								EmptyPos
							);
						}

						return TRUE;
					}
					else if(DEPOSIT == GetMouseState())
					{
						return TRUE;
					}
				}
				else if(SI_GUILDWAREHOUSE == SlotIdx)
				{ //길드창고 출고
					if (GetMouseState() == GUILD_WITHDRAW)
					{
						POSTYPE EmptyPos = 0xFF;
						BOOL bRet = ItemManager::Instance()->GetEmptySlotPos(SI_INVENTORY, EmptyPos);
						if (bRet)
						{
							ItemManager::Instance()->SendItemMoveMsg(
								SI_GUILDWAREHOUSE, 
								SI_INVENTORY, 
								ToPos, 
								EmptyPos
								);
						}

						return TRUE;
					}
					else if(GUILD_DEPOSIT == GetMouseState())
					{
						return TRUE;
					}
				}
                else if (SI_VENDOR_SELL == SlotIdx)
                {
                    // pToContainer 이상의 기능을 원하므로 확장된 포인터인 원래클래스 포인터로 처리한다.

					uiVendorSell *vendorSell = GET_CAST_DIALOG ( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

					if(vendorSell)
					{
						SCSlot& toSlot      = vendorSell->GetSlot(ToPos);

						if (toSlot.GetCode())
						{
							// 슬롯에 채워져 있는 아이템을 클릭했을 경우

							if (this->IsExistItemAtHand())
								this->RollbackItemAtHand();

							uiSCItemSlot& toItemSlot = (uiSCItemSlot&)toSlot;

                            if (toItemSlot.CanEquip())
							{
								vendorSell->SetCurrentItemSlotInfo(1,
									toItemSlot.GetMoney(),
									ToPos);
							}
							else
							{
								vendorSell->SetCurrentItemSlotInfo(toItemSlot.GetNum(),
									toItemSlot.GetMoney(),
									ToPos);
							}

							return TRUE;
						}
					}
					else
					{
						return FALSE;
					}
                }
                else if (SlotIdx == SI_SMARTNPC)
                {
                    return TRUE;
                }
                else if (SlotIdx == SI_DOMINATION_TOKEN)
                {
                    return FALSE;
                }
			}
			break;

		case ST_QUICK:
			{
				QuickSlot & rQuickSlot = (QuickSlot & )rSlot;

				if( !pAtContainer->IsEmpty( ToPos ) )
				{
					ItemUnitRenderer::RenderUnit * pRenderUnit = 
						pContainerDlg->GetItemUnitRender()->GetItemUnit( 
						rQuickSlot.GetOrgSerial() );

					assert(pRenderUnit);
					if (pRenderUnit)
					{
						AddItemAtHand( 
							SlotIdx, 
							ToPos, 
							eTempSlotType, 
							rQuickSlot.GetOrgCode(),
							pRenderUnit, 
							pRect 
							);
					}

					return TRUE;
				}
			}
			break;

		case ST_STYLE_QUICK:
			{
                // Perk 다이얼로그에서 Perk을 들어올린다.
                SCPerkSlot& perk_slot_ref = static_cast<SCPerkSlot&>(rSlot);
                if (perk_slot_ref.is_active_slot() == true)
                {
                    return FALSE;
                }

                const PerkInfo* perk_info = perk_slot_ref.perk_info();
                if (perk_info == NULL)
                {
                    return FALSE;
                }

                ItemUnitRenderer* unit_renderer = pContainerDlg->GetItemUnitRender();
                if (unit_renderer == NULL)
                {
                    return FALSE;
                }
                DWORD perk_group = perk_slot_ref.GetPerkID().group_;
                ItemUnitRenderer::RenderUnit* render_unit = 
                    unit_renderer->GetItemUnit(perk_group);
                if (render_unit == NULL)
                {
                    return FALSE;
                }
                AddItemAtHand(SlotIdx, ToPos, eTempSlotType, 0, render_unit, pRect);
                return TRUE;
			}
			break;

		case ST_SKILL:
			{
				
				SLOTCODE code=rSlot.GetCode();
				BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(code);
				if(pInfo == NULL)// /*&& pInfo->m_bySkillUserType==eSKILL_USER_SUMMONED*/)
				{
					return FALSE;
				}

			}
			break;

		default:
			assert(!"정의되지 않은 아이템 타입이 왔다!!");
			return FALSE;
			break;
		}

		if (!pAtContainer->IsEmpty(ToPos))
		{
			SCSlot & rSlot = pAtContainer->GetSlot(ToPos);

			ItemUnitRenderer::RenderUnit * pRenderUnit = 
				pContainerDlg->GetItemUnitRender()->GetItemUnit( rSlot.GetSerial() );


            if (pRenderUnit)
            {
				BOOL bEnableDiv = TRUE;
                if (bEnableDiv && g_Input.GetState(DIK_LSHIFT, KS_KEY))
                {
                    if (eTempSlotType == ST_ITEM)
                    {
						SCItemSlot& rItemSlot = (SCItemSlot&)pAtContainer->GetSlot(ToPos);

						if (rItemSlot.GetLaxLock()->IsLock()) 
						{
							//GS_ADD:아이템 블럭시 분할할 수 없다.
							return FALSE;
						}

                      
                        if (SlotIdx == SI_NPCSTORE)
                        {
                            if (rItemSlot.GetMaxNum() > 1 && !rItemSlot.IsBlocked() && !rItemSlot.IsLocked())
                            {
                                uiItemShopDiv* itemShopDiv = 
                                    GET_CAST_DIALOG(uiItemShopDiv, IM_ITEMDIV_MANAGER::ITEMSHOPDIV);

                                if (itemShopDiv)
                                {
                                    itemShopDiv->SetData(rItemSlot.GetMaxNum(),
                                        SlotIdx, 
                                        ToPos, 
                                        eTempSlotType, 
                                        pAtContainer->GetSlot(ToPos).GetCode(),
                                        pRenderUnit, 
                                        pRect,
                                        rItemSlot.GetPriceForBuy()
                                        );
                                    itemShopDiv->ShowInterface(TRUE);
                                }
                            }
                            else
                            {
                                AddItemAtHand( 
                                    SlotIdx, 
                                    ToPos, 
                                    eTempSlotType, 
                                    pAtContainer->GetSlot(ToPos).GetCode(),
                                    pRenderUnit, 
                                    pRect 
                                    );
                            }
                        }
                        else
                        {
						    if (rItemSlot.GetNum() > 1 && !rItemSlot.IsBlocked() && !rItemSlot.IsLocked() )
                            {
                                uiItemDiv* itemDiv = GET_CAST_DIALOG(uiItemDiv, IM_ITEMDIV_MANAGER::ITEMDIV);

                                if (itemDiv)
                                {
                                    itemDiv->SetData(rItemSlot.GetNum(),
                                        SlotIdx, 
                                        ToPos, 
                                        eTempSlotType, 
                                        pAtContainer->GetSlot(ToPos).GetCode(),
                                        pRenderUnit, 
                                        pRect 
                                        );
                                    itemDiv->ShowInterface(TRUE);
                                }
                            }
                            else
                            {
                                uiBase* itemDiv = GET_DIALOG(IM_ITEMDIV_MANAGER::ITEMDIV);
                                if ((itemDiv != NULL) && (itemDiv->IsVisible() == TRUE))
                                {
                                    itemDiv->ShowInterface(FALSE);
                                }
                                AddItemAtHand(SlotIdx, 
                                                ToPos, 
                                                eTempSlotType, 
                                                pAtContainer->GetSlot(ToPos).GetCode(),
                                                pRenderUnit, 
                                                pRect);
                            }
                        }
                    } // if( eTempSlotType == ST_ITEM )
                    else
                    {
                        AddItemAtHand( 
                            SlotIdx, 
                            ToPos, 
                            eTempSlotType, 
                            pAtContainer->GetSlot(ToPos).GetCode(),
                            pRenderUnit, 
                            pRect 
                            );
                    }
                } // if (g_Input.GetState(DIK_LSHIFT, KS_KEY))
                else
                {
                    uiBase* itemDiv = GET_DIALOG(IM_ITEMDIV_MANAGER::ITEMDIV);
                    if (itemDiv)
                    {
                        itemDiv->ShowInterface(FALSE);
                    }
                    AddItemAtHand( 
                        SlotIdx, 
                        ToPos, 
                        eTempSlotType, 
                        pAtContainer->GetSlot(ToPos).GetCode(),
                        pRenderUnit, 
                        pRect 
                        );
                }
            }
		}
	}

	return TRUE;
}


//----------------------------------------------------------------
/** ItemUseTransaction
	아이템 사용관련해서 조건 처리와 사용 패킷을 보내는 것을 담당. 
	comment by j0
*/
BOOL MouseHandler::ItemUseTransaction( SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	assert(pAtContainer);

	if (!pAtContainer)
	{
		return FALSE;
	}

	if (!g_pHero)
	{
		return FALSE;
	}
	
	if ( !_canUseTransaction(SlotIdx, AtPos) )
	{
		return FALSE;
	}

	switch (SlotIdx)
	{
	case SI_INVENTORY:
		{	
			ItemUseTransactionFromInventory(SlotIdx, AtPos);
		}
		break;

	case SI_SKILL:
		{
			SCSlot & rSlot = pAtContainer->GetSlot(AtPos);
			ItemUseTransactionFromSkill(rSlot.GetCode());
		}
		break;

	case SI_QUICK:
		{						
			ItemUseTransactionFromQuick(SlotIdx, AtPos);
		}
		break;
	}

	return TRUE;
}

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::_canUseTransaction( SLOTIDX SlotIdx, POSTYPE AtPos)
{
	if( g_pHero == NULL )
		return FALSE;

	if( g_pHero->IsPolymorph() == TRUE )
		return FALSE;

	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	if( pAtContainer == NULL )
	{
		return FALSE;
	}

	switch(SlotIdx)
	{
	case SI_INVENTORY:
		{	
			if (pAtContainer->IsEmpty(AtPos))
			{
				return FALSE;
			}	

			SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

			const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
			if ( pItemInfo )
			{
				if (eITEMTYPE_RECALL_ALLIANCE != pItemInfo->m_wType)
				{
					if (g_pHero && g_pHero->IsDead())
					{
						return FALSE;
					}	
				}
			}

			if ( GlobalFunc::IsInventoryLocknMessage() )
			{
				return FALSE;
			}
		}
		break;

	case SI_QUICK:
		{						
			QuickContainer * pQuickContainer = (QuickContainer * )pAtContainer;
			if ( !pQuickContainer )
			{
				return FALSE;
			}	

            POSTYPE RealQuickPos = pQuickContainer->GetCurTabNum();	// 현재 텝위치.
            RealQuickPos = 0;// 두줄일때는 탭위치를 안더한다.
            AtPos = pQuickContainer->GetRealSlotPos(AtPos);
            RealQuickPos += AtPos;

			// 공통 선행 조건

			if (RealQuickPos > pQuickContainer->GetMaxSlotNum() )
			{
				return FALSE;
			}

			if (pQuickContainer->IsEmpty(RealQuickPos)) 
			{
				return FALSE;
			}

			QuickSlot & rFromSlot = (QuickSlot &)pQuickContainer->GetSlot( RealQuickPos );
			POSTYPE OrgPos		= 0; 

			if ( ST_ITEM == rFromSlot.GetOrgSlotType() )
			{
				if ( !rFromSlot.GetOrgSlot() )
					return FALSE;

				SCItemSlot & rItemSlot = (SCItemSlot &)*rFromSlot.GetOrgSlot();

				const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
				if ( pItemInfo )
				{
					if (eITEMTYPE_RECALL_ALLIANCE != pItemInfo->m_wType)
					{
						if (g_pHero && g_pHero->IsDead())
						{
							return FALSE;
						}	
					}
				}

				if ( GlobalFunc::IsInventoryLocknMessage() )
				{
					return FALSE;
				}
			}
		}
		break;

	default:
	case SI_STYLE:
	case SI_SKILL:
		{
			if (g_pHero && g_pHero->IsDead())
			{
				return FALSE;
			}	
		}
		break;
	}


	return TRUE;
}

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::ItemUseTransactionFromInventory( SLOTIDX SlotIdx, POSTYPE AtPos )
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	if (!pAtContainer)
	{
		return FALSE;
	}

	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if( !pItemInfo )
	{
		return FALSE;
	}


#ifdef _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	if( pItemInfo->m_wType == eITEMTYPE_COSTUME || 
		pItemInfo->m_wType == eITEMTYPE_COSTUME_DECORATE )
#else
    if( pItemInfo->m_wType == eITEMTYPE_COSTUME )
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    {
        //코스튬 아이템
        if( ItemManager::Instance()->CanEquipClass( g_pHero, pItemInfo ) == TRUE &&
            ItemManager::Instance()->CanEquipByGender( g_pHero, pItemInfo ) == TRUE )
        {
            ItemManager::Instance()->SendItemUseMsg(SlotIdx, AtPos);
        }
        else
        {
            // 70667 아이템 사용 조건이 맞지 않습니다.
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
            g_InterfaceManager.GetInterfaceString(70667, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }

        return TRUE;
    }
#endif//_NA_004035_20120227_COSTUME_ITEM

    
    if (SCItemSlot::CanEquip(pItemInfo))
	{
        if ((rItemSlot.GetItemRuleInfo().is_valid_item == false) ||
            (rItemSlot.GetItemRuleInfo().is_etheria || rItemSlot.GetItemRuleInfo().is_shell))
        {
            ItemManager::Instance()->NoticeReasonItemErrorCode(RC::RC_ITEM_CAN_NOT_EQUIP);
            return FALSE;
        }

        //------------------------------------------------------------------------------ 
        if ( rItemSlot.IsPossessionItem() )
        {// 귀속 아이템 사용 
            if ( rItemSlot.GetIdentifyPossession() == 0 )
            {//귀속아이템 처음 사용
                // 클래스 체크
                if ( !ItemManager::Instance()->CanEquipClass(g_pHero, pItemInfo) )
                {
                    //	장착할 수 없는 클레스 입니다.
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
                    g_InterfaceManager.GetInterfaceString( eST_CAN_NOT_EQUIP_CLASS, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

                    return FALSE;
                }

                // 능력치 제한 체크
                if ( !ItemManager::Instance()->CanEquipLimit((SCItemSlot &)rItemSlot, TRUE))
                {
                    return FALSE;
                }
                //사용할건지 팝업(귀속 인증)
                if(	GetPossessionWin() != true )
                {
                    //5090	장착시 귀속됩니다. 장착하시겠습니까?
                    TCHAR szBuf[INTERFACE_STRING_LENGTH];
                    g_InterfaceManager.GetInterfaceString( 5090 , szBuf, INTERFACE_STRING_LENGTH-1);

                    uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                    if(pSystemMan)
                    {
                        //! 여기수정
                        //pSystemMan->Sys_Confirm(NULL, GlobalFunc::UsePossessionItem, NULL/*szTitle*/, szBuf);
                        pSystemMan->Sys_ConfirmLock(szBuf, GlobalFunc::UsePossessionItem);
                    }
                    SetPossessionItemPOS(AtPos, 255);
                    SetPossessionWin(true);
                    return FALSE;
                }
                else
                {
                    // 6130: 한번에 하나씩 귀속 할 수 있습니다.
                    TCHAR szBuf[INTERFACE_STRING_LENGTH];
                    g_InterfaceManager.GetInterfaceString( 6130 , szBuf, INTERFACE_STRING_LENGTH-1);
                    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szBuf );
                    return FALSE;
                }
            }
        }		
        //------------------------------------------------------------------------------ 

		if (g_pMap->IsSnowManEventMap())
		{
			if (pItemInfo->IsWeapon())
			{
				//눈사람이벤트맵에서는 무기 착용금지!
				ItemManager::Instance()->NoticeReasonItemErrorCode(RC_ITEM_CANT_EQUIP_WEAPON);
				return FALSE;
			}
		}
		
		if (_useTransactionFateItemIdentify(rItemSlot, pItemInfo, AtPos) == TRUE)
        {
			return TRUE;
        }


		POSTYPE EQPos = 255;
		BOOL bCorrectPos = ItemManager::Instance()->GetEquipPosition(rItemSlot, EQPos);
		if (!bCorrectPos)
		{
			return FALSE;
		}

		

		BOOL bCanEquip = ItemManager::Instance()->CanEquip(SlotIdx, AtPos, (POSTYPE)EQPos);
		if (bCanEquip)
		{
			ItemManager::Instance()->SendItemMoveMsg(
				SI_INVENTORY, 
				SI_EQUIPMENT,
				AtPos, 
				EQPos);
			
			if (IsLockWasteDeleteItem() && (AtPos == ItemManager::Instance()->GetPendingDeleteItemPos()))
			{
				uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if (pSystemMan)
				{
					pSystemMan->Destroy_Sys_Confirm(m_WasteBasketDialogID);
					GlobalFunc::DeleteItemConfirm(false);
				}				
			}
		}
		else
		{
			g_HeroInput.PlayLimitUseSound();
		}

		return TRUE;
	}
    else if (eITEMTYPE_STAT_CHANGE == pItemInfo->m_wType)
    {
        // 기능 변경으로 인해 다른 곳에서 확인 함
        return FALSE;
    }
    else if(pItemInfo->m_wType == eITEMTYPE_WAX)
    {
         MouseHandler::Instance()->RollbackItemAtHand();
         MouseHandler::Instance()->SetMouseState(MouseHandler::RIDER_WAX);
         CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair);
         keyMsg msg;
         ZeroMemory(&msg,sizeof(msg));
         msg.dwType=eDoType_Cursor_Rider_Wax;				
         msg.DoSomething=GlobalFunc::DoWaxGlossItemCancle;
         g_KeyQueueManager.PushBack(msg);
         SetPendingUseChargeItemPos(AtPos);
    }
	 else if (pItemInfo->IsRandomItem()) 
	 {
		if(	GetRandomWin() != true )
		{
			//6881	[%s] 아이템을 사용하시겠습니까?
			TCHAR szBuf[INTERFACE_STRING_LENGTH];
			TCHAR szText[INTERFACE_STRING_LENGTH];
			TCHAR szItemName[MAX_PATH];
			g_InterfaceManager.GetInterfaceString( 6881 , szBuf, INTERFACE_STRING_LENGTH-1);
			
			g_InterfaceManager.GetItemName( pItemInfo->m_Code, szItemName, MAX_PATH );
			Snprintf( szText, INTERFACE_STRING_LENGTH - 1, szBuf, szItemName );
			
			SetRandomItemPOS(AtPos);
			SetRandomWin(true);

			//인벤토리는 락걸자
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (inventory_manager != NULL)
            {
                inventory_manager->LockInventoryUI();
            }

			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_ConfirmLock(szText, GlobalFunc::UseRandomItem);
			return FALSE;
		}
		else
		{
			// 6882: 해당 아이템은 중복으로 사용할 수 없습니다. 이전 아이템을 사용한 후 시도해 주십시오.
			TCHAR szBuf[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString( 6882 , szBuf, INTERFACE_STRING_LENGTH-1);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szBuf );
		}

	 }
	else if (pItemInfo->IsCanUseWaste() || pItemInfo->IsCanOnlyUseWaste()) 
	{
		BOOL bSpecialItem = FALSE;

		BASE_ITEMINFO * pItemInfo = 
			(BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(rItemSlot.GetCode());
		if(!pItemInfo)
		{
			return FALSE;
		}
		if(g_pHero->CanUseItem(pItemInfo,TRUE) && FALSE == rItemSlot.IsLocked())
		{

			if( ItemUseTransactionFromInventoryByItemTpye(SlotIdx, AtPos ) )
				return TRUE;

		}
	}
    else if(pItemInfo->IsRiderPartsItem())
    {
        if(ItemUseTransactionFromInventoryByRiderItemParts(SlotIdx,AtPos ))
        {
            return TRUE;
        }
    }
	else
	{
		//이큅이아니고 소모성아이템도 아니다.
		if(ItemUseTransactionFromInventoryByChargeStyle(SlotIdx,AtPos ))
			return TRUE;
	}

	return TRUE;
}

//----------------------------------------------------------------
/** 라이더 아이템 파츠에대한처리
*/
BOOL MouseHandler::ItemUseTransactionFromInventoryByRiderItemParts( SLOTIDX SlotIdx, POSTYPE AtPos )
{
    if (!g_pMap) return FALSE;

    if (!g_pHero) return FALSE;

    SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
    SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);
    const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();

    if(g_pHero->IsRiding() && pItemInfo)
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH]; 

        //라이더슬롯이 비어있다면 패킷보낸다.
        POSTYPE RiderEquipPos = pItemInfo->m_wEqPos - eRIDER_PARTS_POS::StartIndex;
        if(g_pHero->GetSlotContainer(SI_RIDER) && g_pHero->GetSlotContainer(SI_RIDER)->IsEmpty(RiderEquipPos))
        {
            //보내기전 이큅가능한지한번더체크

            SCItemSlot & rRiderSlot = (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->m_RiderInfo.m_RiderItemPos);

            if(rRiderSlot.IsRiderItem())
            {
                RC::eITEM_RESULT result = rRiderSlot.CanEquipRiderParts(rItemSlot);

                if(result==RC::RC_ITEM_SUCCESS)
                {
                    MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN SendMsg;
                    SendMsg.header.request = SendMsg.header.eReq_EquipParts;
                    SendMsg.header.args.equip.from = AtPos;
                    SendMsg.header.args.equip.to.value =(BYTE)pItemInfo->m_wEqPos;
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
                    return TRUE;
                }
                else
                {
                    //6440:소환한 탈것과 맞지않는아이템입니다.
                    g_InterfaceManager.GetInterfaceString(6440,szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
            }
        }
        else
        {
            //6454 탈것장비가 이미 착용중입니다.
            g_InterfaceManager.GetInterfaceString(6454,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
    
    return FALSE;
}

//----------------------------------------------------------------
/** 캐쉬아이템 스타일에에 따른처리
*/
BOOL MouseHandler::ItemUseTransactionFromInventoryByChargeStyle( SLOTIDX SlotIdx, POSTYPE AtPos )
{
	if (!g_pMap) return FALSE;
	
	if (!g_pHero) return FALSE;
	
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);
	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();

	//3가지조건을 만족시 서버에게 패킷을보낸다.(ex:추가된 명지휘봉계열)
	// 1. 이큅아이템이 아니여아하고
	// 2. ChargeSubType이  eCHARGE_SUB_FIXED_AMOUNT_EX여야하면	  
	// 3. IsProgressTime이 false일때
    BYTE charge_subtype = rItemSlot.GetItemRuleInfo().charge_sub_type;
	if (charge_subtype == eCHARGE_SUB_FIXED_AMOUNT_EX &&
		!rItemSlot.IsProgressTime())
	{
#ifdef _DEV_VER
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN"));
#endif 
		//지휘봉일때는 우클릭시 켜고 끄고서버로 패킷을 보낸다.
		MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN SendPacket;
		SendPacket.m_FireUpItemPos = AtPos;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
	}
	return TRUE;
}


//----------------------------------------------------------------
/** 처리 했으면 TRUE 처리 하지 못했으면 FALSE 를 리턴한다.
*/
BOOL MouseHandler::ItemUseTransactionFromInventoryByItemTpye(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	// 저장 되지 않은 상태 ( 저장해야 한다. )
	if (!g_pMap)
	{
		return FALSE;
	}

	if (!g_pHero)
	{
		return FALSE;
	}



	// 상단의 함수에서 조건 체크를 했으니 믿고 쓰자.

	SCSlotContainer* pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot& rItemSlot = (SCItemSlot&)pAtContainer->GetSlot(AtPos);
	const BASE_ITEMINFO* pItemInfo = rItemSlot.GetItemInfo();

	//이큅아이템중 m_byEventCode와 동일한 아이템이 있는지체크
	if (pItemInfo->m_byEventCode != 0)
	{
		if (!g_pHero->GetHeroEquipContainer()->FindEventCode(pItemInfo->m_byEventCode))
		{
			//5092 아이템을 적용시킬수 있는 장비를 착용해야됩니다.
			GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5092));
			return FALSE;
		}
	}

	// 유료화 아이템 처리
	switch (pItemInfo->m_wType)
	{
	case eITEMTYPE_SSQ:
		{
			if (g_ProjectileManagerEx.GetProjectile(g_dwSSQPortalKey) == NULL )
            {
				return UseTransationItemSSQ(SlotIdx, AtPos);
            }
			else
            {
				return FALSE;
            }
		}
		break;


	case eITEMTYPE_TOP_OF_TRIAL:
	case eITEMTYPE_TOP_OF_TRIAL_CHARGE:
		{
			return _useTransactionItemTopOfTrial(SlotIdx, AtPos);
		}
		break;

	case eITEMTYPE_PORTAL_QUEST_ITEM:
		{
			return _useTransactionItemPortalQuest(SlotIdx, AtPos);
		}
		break;

	case eITEMTYPE_TOTEM:
		{
			return _useTransactionChargeItemTransact(SlotIdx, AtPos);
		}
		break;


	case eITEMTYPE_PORTAL:				
	case eITEMTYPE_SAVE_COORD:
		return _useTransactionChargeItemPortalScroll(SlotIdx, AtPos);
		break;

	case eITEMTYPE_WAYPOINT:
		return _useTransactionChargeItemPersonalWaypoint(SlotIdx, AtPos);
		break;


	case eITEMTYPE_RECALL:
		{
			if (GameFramework::GetCurrentScene() && 
				(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE))
			{
				// 5067	마을에서는 해당 아이템을 사용할 수 없습니다.
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5067));
				return TRUE;
			}

			return _useTransactionChargeItemTransact(SlotIdx, AtPos);	
		}
		break;


	case eITEMTYPE_RECALL_ALLIANCE:
		{
			if (GameFramework::GetCurrentScene() && 
				GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE )
			{
				// 5067	마을에서는 해당 아이템을 사용할 수 없습니다.
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5067) );
				return TRUE;
			}
			return _useTransactionChargeItemTransact( SlotIdx, AtPos );
		}
		break;

	case eITEMTYPE_REPAIR:
		{
			BOOL bRepairItem = ItemManager::Instance()->IsExistRepairItem(SI_EQUIPMENT);
			if (!bRepairItem)
			{
				//85225	수리할 아이템이 없습니다
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(85225));
				return TRUE;
			}			

			return _useTransactionItemDuraRepair( SlotIdx, AtPos );

		}
		break;



	case eITEMTYPE_STAT_RESET:
		{
			return _useTransactionChargeItemStatInit( SlotIdx, AtPos );							
		}
		break;


	case eITEMTYPE_SOCKETEXTRACT:
		{
			return TRUE;
		}
		break;


	case eITEMTYPE_RANK_DOWN:
		{
			return TRUE;
		}
		break;



	case eITEMTYPE_HAMMER_OF_ALCHEMIST:
		{
			return TRUE;
		}
		break;

	case eITEMTYPE_PET:		
		{
			if(!g_pHero) break;
			
			POSTYPE PetPos = AtPos; 
			//펫소환상태에서 다른펫을 클릭했다(자동펫교체)
			if(g_pHero->IsPetReturn() && g_pHero->GetPetItemPos() != AtPos)
			{
				SCPetInfo  * pPetInfo  = rItemSlot.GetPetInfo();
					
				//소환불가능상태인 펫을 클릭했을때
				if(pPetInfo->GetFullness()<=0 && GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE)
				{
					//6391
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6391));
					break;
				}
				else
				{					
					g_pHero->PetChangeON(AtPos);			
					PetPos = g_pHero->GetPetItemPos();
				}
			}
			
			//펫소환
			MSG_CG_ITEM_USE_PET_ITEM_SYN SendPacket; 
			SendPacket.m_PetItemPos = PetPos;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
		}
		break;

	case eITEMTYPE_PET_FEED:		
		{
			//펫사료 아이템
			ItemManager::Instance()->SendItemUseMsg(SlotIdx, AtPos);

			if(g_pHero)
				g_pHero->UsePetFeed();
		}
		break;

	case eITEMTYPE_PET_NAME_CARD:
		{
			//펫소환상태에서만 가능
			if (!g_pHero || !g_pHero->IsPetReturn()) 
            {
                break;
            }
						
			//펫 이름 설정 아이템
			uiPetNameSetting* pPetNameDlg = GET_CAST_DIALOG(uiPetNameSetting, IM_PET_MANAGER::PET_NAME_DLG);

			if (!pPetNameDlg || pPetNameDlg->IsVisible()) 
            {
                break;
            }

			pPetNameDlg->SetLockNameCard(AtPos);
			pPetNameDlg->ShowInterface(TRUE);
		}
		break;
 

	case eITEMTYPE_ETHER_BULLET:
		{
			if (!g_pHero) 
            {
                break;
            }

			//패킷기다리는상태에서는 일단정지
			if (g_pHero->GetEthreInfo().bWait) 
            {
                break;
            }

			MSG_CG_ITEM_ACTIVE_ETHER_BULLET_SYN SendPacket;
				
			//장전되있는데 다른 탄환을 눌렀을경우
			if (g_pHero->GetEthreActive())
			{
				SendPacket.m_bActive = FALSE;
				if (AtPos == g_pHero->GetEthreInfo().byEtherChargePos) 
				{
					//해제
					SendPacket.m_ActiveBulletItemPos = AtPos;
				}
				else
				{
					//교체
					SendPacket.m_ActiveBulletItemPos = g_pHero->GetEthreInfo().byEtherChargePos;
                    g_pHero->EtherChangeON(AtPos);
				}
			}
			else
			{
				//장전:딜레이상태일때 리턴
				SendPacket.m_bActive = TRUE;
				g_pHero->SetEtherChargePos(AtPos);
				SendPacket.m_ActiveBulletItemPos = AtPos;
				
			}

			g_pHero->SetEtherWaitPacket(true);
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
		break;



		case eITEMTYPE_LOTTO:
			{
				//타락한사원에서는 복권을 열수없다.
				if(GlobalFunc::IsSSQScene())
				{
					break;
				}

				if(!g_pHero) break;
				 
				LotteryInventoryDialog *pPandoraDlg =  GET_CAST_DIALOG( LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG);
				
				if(!pPandoraDlg) break;

				//복권창이 열려있다면 블럭시킨다.
				if(pPandoraDlg->IsVisible()) break;
												
				pPandoraDlg->SetInvenPos(AtPos);
				
				int nEmptySlot =  g_pHero->GetEmptyUseSlot();

				SCLottoInfo* pLottoSlot = rItemSlot.GetLottoInfo();
				if(!pLottoSlot) break;

                if (pLottoSlot->GetIdentityStatus())
                {
                    pPandoraDlg->SendItemUseSync(TRUE);		//판도라 아이템 열기패킷보내기
                }
                else
                {
                    keyMsg msg;
                    ZeroMemory(&msg,sizeof(msg));
                    msg.dwType = eDoType_Lottery_OpenDlg;
                    msg.DoSomething = GlobalFunc::DoLotteryOpenCancel;
                    msg.lParam = InterfaceManager::DIALOG_DELAY_CASTING;
                    g_KeyQueueManager.PushBack(msg);
                    StartLeaveFieldCastTimer(eDoType_Lottery_OpenDlg,3000); 
                    RollbackItemAtHand();
                    //인벤토리는 락걸자
                    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
                    if (inventory_manager != NULL)
                    {
                        inventory_manager->LockInventoryUI();
                    }
                }
			}
			break;  


		case eITEMTYPE_RIDER:// 탈 것 아이템 우클릭시
			{
                //예외처리:1.용변신상태에서 아이템클릭방지(6472)
                //         2.라이더 교체상태(패킷대기시에는 이벤트막는다)
                //         3.캐스팅시작을 보냈을때
                //         4.점프상태에서 클릭방지
                //         5.캐스팅애니중에는 중복클릭을 막는다
                //         6.스킬상태에서는 소환할수없다.
                //         7.스킬큐에 값이 있을때
                //         8.쉐도우 하이드 상태에서는 불가
                if(g_pHero->IsTransForm() || 
                   g_pHero->m_RiderInfo.m_bChangeRider|| 
                   g_pHero->m_RiderInfo.m_bSendSummon || 
                   g_pHero->GetCurState()==STATE::KEYBOARDJUMP ||
                   g_pHero->GetCurState()==STATE::SUMMON_RIDER || 
                   g_pHero->GetCurState()==STATE::SKILL        ||
                   (g_pHero->GetHideSkill()&&g_pHero->GetClass()==eCHAR_SHADOW) || 
                   g_SkillQueue.Size()>0        )
                {
                        //6472:지금은 상태에서는 소환할수없다
                        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
                        g_InterfaceManager.GetInterfaceString(6472,szMessage,INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        break;
                }       

                //기간만료라이더에대한처리
                if(rItemSlot.IsExpiredWaitTime())
                {
                    //라이딩상태에서는  반응없어야함
                    if(g_pHero->IsRiding())
                    {
                        break;
                    }

                    RiderInventoryDialog *pInven = (RiderInventoryDialog *)g_pHero->GetSlotContainer(SI_RIDER);
                    
                    if(pInven)
                    {
                        //열려있고 디스플레이용객체가 보여지는상태일때
                        if(pInven->IsSlotRider())
                        {   
                            //위치가 틀리다면 요놈해제하고 이걸루 교체해준다.
                            if(pInven->GetRiderSummonPos()!=AtPos)
                            {
                                pInven->UnlockSlotRider(TRUE);
                                pInven->CopySlotRider(AtPos);
                            }
                            else
                            {
                                //활성화상태에서 또한번누르면 창을 닫아준다.
                                pInven->ShowInterface(FALSE);
                            }
                        }
                        else
                        {   
                            //안열렸다면 바로 open
                            pInven->CopySlotRider(AtPos);
                            pInven->ShowInterface(TRUE);
                        }
                        break;
                    }
                }
                
         	    MSG_CG_ITEM_RIDER_CONTROL_SYN SendMsg;
        		
                //현재소환상태일때(해제한다:request==eReq_SummonRelease)
			    if(g_pHero->IsRiding())
			    {
                    if(g_pHero->GetCurState()==STATE::KEYBOARDJUMP)
                    {
                        //점프시에는 액션을 막는다.
                        break;
                    }

    			    SendMsg.header.request = SendMsg.header.eReq_SummonRelease;
                    
				    //클릭위치가 현재소환된위치가 아니라면(다른탈것을 클릭했다)
				    if(g_pHero->m_RiderInfo.m_RiderItemPos!=AtPos)
				    {
					    //클라에서 자동의로소환해제후 소환할수있도록 m_bChangeRider변수true
					    g_pHero->m_RiderInfo.m_bChangeRider = TRUE;
					    g_pHero->m_RiderInfo.m_RiderChangePos = AtPos;
                        SendMsg.header.args.summon.slot_pos = g_pHero->m_RiderInfo.m_RiderItemPos;
				    }
                    else
                    {
                        SendMsg.header.args.summon.slot_pos = AtPos;
                    }
                    
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
			    }
			    else
			    {       
                    //보내기전에 소환할수있는 지역인지체크한다
                    const  sRiderInfo* pRiderInfo = RidingInfoParser::Instance()->GetInfo(rItemSlot.GetItemInfo()->m_wSkillCode);

                    const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( (MAPCODE)g_pMap->GetMapID() );

                    if(pRiderInfo && pMapInfo)
                    {
                        if(pRiderInfo->IsAcceptField(eZONETYPE(pMapInfo->byMKind)))
                        {
                            //동기화를 멈춘다
                            g_HeroInput.SetAutoRun(FALSE);
                            g_HeroInput.SetFollowState(FALSE);
                            g_HeroInput.SetAutoAttack(FALSE);

                            //탈것비소환상태에서는 소환캐스팅을 보낸다.
                            SendMsg.header.request = SendMsg.header.eReq_SummonStart;
                            SendMsg.header.args.casting_start.slot_pos = AtPos;
                            g_pHero->m_RiderInfo.m_RiderItemPos = AtPos;
                            g_pHero->m_RiderInfo.m_bSendSummon  = TRUE;
                            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
                        }
                        else
                        {
                            //6436:소환할수없는 지역입니다.
                            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
                            g_InterfaceManager.GetInterfaceString(6436,szMessage,INTERFACE_STRING_LENGTH);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                            break;
                        }
                    }
                    else
                    {
                        assert(!"맵 or 라이더정보없음");
                        break;
                    }
			    }
			}
			break;

        case eITEMTYPE_ETHEREALIZER:
            {
                SetEtheriaExtractState();
                SetPendingUseChargeItemPos(AtPos);
            }
            break;

        case eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL:
        case eITEMTYPE_CHANGE_CHARACTER_FACE:
        case eITEMTYPE_CHANGE_CHARACTER_HAIR:
            {
				if (ItemManager::Instance()->CanEquipClass(g_pHero, pItemInfo))
				{
					SetCharCustomizeDlg(pItemInfo->m_wType, AtPos);
				}
                else
                {
                    int class_code = 0;
                    switch (pItemInfo->GetFirstAvailableClass())
                    {
                    case eCHAR_BERSERKER:
                        class_code = eST_CHARACTER_CLASS_NAME_BERSERKER;
                        break;
                    case eCHAR_DRAGON:		
                        class_code = eST_CHARACTER_CLASS_NAME_DRAGON;
                        break;
                    case eCHAR_SHADOW:		
                        class_code = eST_CHARACTER_CLASS_NAME_SHADOW;
                        break;
                    case eCHAR_VALKYRIE:		
                        class_code = eST_CHARACTER_CLASS_NAME_VALKYRIE;
                        break;
                    case eCHAR_MAGICIAN:
                        class_code = eST_CHARACTER_CLASS_NAME_MAGICIAN;
                        break;
                    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                        class_code = eST_CHARACTER_CLASS_NAME_MYSTIC;
                        break;
                    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                        class_code = eST_CHARACTER_CLASS_NAME_HELLROID;
                        break;
                    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        class_code = eST_CHARACTER_CLASS_NAME_WITCHBLADE;
                        break;
                    }

                    TCHAR class_name[512]; 
                    g_InterfaceManager.GetInterfaceString(class_code, class_name);
                    // 2228: 해당 아이템은 %s 클래스만 사용할 수 있습니다.
                    TCHAR string_msg[INTERFACE_STRING_LENGTH]; 
                    g_InterfaceManager.GetInterfaceString(2228, string_msg);
                    TCHAR output_message[INTERFACE_STRING_LENGTH]; 
                    Snprintf(output_message,INTERFACE_STRING_LENGTH-1, string_msg, class_name);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, output_message);
                }
            }
            break;

		case eITEMTYPE_POLLEN:
		case eITEMTYPE_TORCH:
		case eITEMTYPE_MINE:
		case eITEMTYPE_FIRECRACKER:
			{	
				if(g_pHero->IsRiding())
				{
                    //6455 탈것 소환상태로 해당아이템을 사용할수없습니다.
                    TCHAR szMessage[INTERFACE_STRING_LENGTH]; 
                    g_InterfaceManager.GetInterfaceString(6455,szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					break;
				}
				if( g_pHero && g_pHero->GetCurState() == STATE::IDLE )
				{
					WzVector wvTargetPos;
					if( pItemInfo->m_wType == eITEMTYPE_TORCH )
					{
						wvTargetPos = g_pHero->GetPosition() +  g_pHero->GetDirection() * 10.0f;

						float fT=0.0f;
						int iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( wvTargetPos, &fT, 100.0f,TMGO_NO_COLLISION);

						if( iMesh>=0 && fT > 0.0f && fT < 5.0f)
						{
							wvTargetPos.z -= fT;
						}

					}
					else if( pItemInfo->m_wType == eITEMTYPE_MINE)
					{
						wvTargetPos = g_pHero->GetPosition() + g_pHero->GetDirection() * 0.5f;
					}
					else
					{
						wvTargetPos = g_pHero->GetPosition();
					}	
					if( GameFunc::IsMove(g_pHero->GetPosition(), wvTargetPos) )
					{
						SetPendingSkillItemPos(AtPos);

#ifdef _YJ_PK_OPTION_001120
						BOOL bPKO = GAMEOPTION->GetPKWithoutCtrlKey();
						m_bCtlKeyDown = ( bPKO ||  ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) );
#else
						m_bCtlKeyDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
#endif //_YJ_PK_OPTION_001120
						
						

						MSG_CG_ITEM_USE_MOTION_SYN SendMsg;
						SendMsg.m_wvMainTargetPos = wvTargetPos;
						SendMsg.m_atPos =AtPos;
						SendMsg.m_atIndex =SlotIdx;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
					}
					else
					{
						//6137 해당 지역으로는 사용할 수 없습니다.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6137));
					}
				}
			}	
			break;

		case eITEMTYPE_CHANGE_GUILD_NAME:
			{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                uiGuildRelationAsk* ui_guild_ask = 
                    GET_CAST_DIALOG(uiGuildRelationAsk, uiGuildRelationAsk::kDialog_ID);
                if (ui_guild_ask != NULL)
                {
                    ui_guild_ask->set_guild_ask_type(uiGuildRelationAsk::kGuild_Rename);
                    ui_guild_ask->set_item_slot_position(AtPos);
                    ui_guild_ask->ShowInterface(TRUE);
                }
#else
                if(g_pHero && g_pHero->IsExistGuild())
                {
                    uiChangeGuildName* pDlg = GET_CAST_DIALOG(uiChangeGuildName, IM_GUILD_MANAGER::GUILD_CHANGE_GUILD_NAME);
                    if(pDlg && FALSE == pDlg->IsVisibleWindow())
                    {
                        pDlg->SetSlotInfo(SlotIdx, AtPos);
                        pDlg->ShowInterface(TRUE);
                    }
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
			}
			break;

        case eITEMTYPE_PERK_CASH_ITEM:
            {
                uiPerkDialog* perk_dialog = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
                if (perk_dialog != NULL)
                {
                    perk_dialog->RequestPerkOperaion(PerkControl::eReq_Plants, 
                                                     AtPos,
                                                     0);
                }
            }
            break;

        case eITEMTYPE_DOMINATION_BUFF:
            {
                Object* object_ptr = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
                if (object_ptr != NULL)
                {
                    GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(static_cast<eSlotContainerIndex>(SlotIdx), 
                                                                        AtPos, 
                                                                        object_ptr->GetObjectKey(),
                                                                        object_ptr->GetPosition());
                }
            }
            break;

        case eITEMTYPE_CHANGE_CHARACTER_GENDER:
            {
                // 장비를 전부 해제하였는지 체크
                if (g_pHero == NULL)
                {
                    return FALSE;
                }

                if (g_pHero->CanUseChangeGenderItem() == FALSE )
                {
					TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
					// 5884 아이템이 장착되어 있는 상태에서는 사용이 가능하지 않습니다.
					g_InterfaceManager.GetInterfaceString(5884, message);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
				}
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                else if( g_pHero->GetClass() == eCHAR_HELLROID )
                {
                }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
				else if( g_pHero->GetClass() == eCHAR_MYSTIC )
				{
					//// 미스틱은 사용가능하지 않다고 안내메세지를 표시할것
					//TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
					//// 5884 아이템이 장착되어 있는 상태에서는 사용이 가능하지 않습니다.
					//g_InterfaceManager.GetInterfaceString(5884, message);
					//GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

#ifdef _DEV_VER
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "ㅋㅋㅋㅋ");
#endif //_DEV_VER
				}
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                else if (g_pHero->GetClass() == eCHAR_WITCHBLADE)
                {
                }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
				else
				{
                    const BASE_ITEMINFO* item_info = rItemSlot.GetItemInfo();
                    BYTE gender = static_cast<BYTE>(g_pHero->GetGender());
                    gender = (gender == GenderType::kMale) ? GenderType::kFemale : GenderType::kMale;
                    if (item_info->m_byWasteType != 0 && item_info->m_byWasteType != gender)
                    {
                        return FALSE;
                    }
                    
                    uiAssistMan* assist_man = static_cast<uiAssistMan*>(
                        g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

                    if (assist_man)
                    {
                        assist_man->ShowAskCharChangeGender(gender, AtPos);
                    }
                }
            }
            break;
        case eITEMTYPE_EXTEND_CHARACTER_SLOT:
            {
                uiAssistMan* assist_man = static_cast<uiAssistMan*>(
                    g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

                if (assist_man)
                {
                    assist_man->ShowAskAddCharSlot(AtPos);
                }
            }
            break;
        case eITEMTYPE_SOCKET_COMPOSE_ADDITIVE:
            {
                //쟈드 합성 첨가제의 경우 인벤토리에서 사용 불가능하다.
                // 2011.7.13 / 송찬종 / 스크립트의 아이템 사용 가능(MaterialType)을 끄는 것으로 막지 않고 이곳에서 아무 처리 안하도록 함
                //                      스크립트 상에서 아이템 사용 불가능으로 하면 쟈드 합성창에서 서버에서 제대로 동작 하지 않을 수 있다고 함
            }
            break;

#ifdef _NA_003740_20111122_RANDOM_ITEM
        case eITEMTYPE_RANDOMIZE_ITEM:
            {
                SetRandomizeItemMode( AtPos );
                return TRUE;
            }
            break;
#endif //_NA_003740_20111122_RANDOM_ITEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        case eITEMTYPE_BILL:
            {
                uiAssistMan::ShowUseCurrencyItem(AtPos);
            }
            break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

        case eITEMTYPE_CHARACTER_NAME_CHANGE:
            {
                // 캐릭명 변경 아이템
                if (GameFramework::GetCurrentScene() &&
                    (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE))
                {
                    TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                    // 158555 해당 아이템은 마을에서만 사용 가능합니다.
                    g_InterfaceManager.GetInterfaceString(158555, message);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

                }
                else
                {
                    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                    if (ui_system_manager)
                    {
                        m_SendSlotIdx = SlotIdx;
                        m_SendAtPos	  = AtPos;

                        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                        // 158550 %s 캐릭터에게 캐릭터명 변경권을 사용하시게습니까?
                        g_InterfaceManager.GetInterfaceStringFormat(message,
                            INTERFACE_STRING_LENGTH,
                            158550,
                            g_pHero->GetName());

                        ui_system_manager->Sys_SystemMessage(message, 
                            NULL,
                            GlobalFunc::_fn_UseCharacterNameChangeItem, 
                            uiSystemMessage::kConfirm_Type);
                    }
                }
            }
            break;
            //_NA_006372_20130108_BADGE_ITEM
        case eITEMTYPE_RENDER_EFFECT_ITEM:
            {
                // 뱃지 아이템
                if( ItemManager::Instance()->CanEquipClass( g_pHero, pItemInfo ) == TRUE &&
                    ItemManager::Instance()->CanEquipByGender( g_pHero, pItemInfo ) == TRUE )
                {
                    ItemManager::Instance()->SendItemUseMsg(SlotIdx, AtPos);
                }
                else
                {
                    // 70667 아이템 사용 조건이 맞지 않습니다.
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};	
                    g_InterfaceManager.GetInterfaceString(70667, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
            }
            break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        case eITEMTYPE_SPA_WARP_SCROLL:
            {
                if ((GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE) ||
                    (GameFramework::GetCurScene() == SCENE_TYPE_FIELD))
                {
                    // 온천 입장권
                    keyMsg key_message;
                    ZeroMemory(&key_message, sizeof(key_message));
                    key_message.dwType		= eDoType_Delay_SpaWarp_Dialog;
                    key_message.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
                    key_message.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
                    key_message.wParam		= eDoType_Delay_SpaWarp_Dialog;
                    g_KeyQueueManager.PushBack(key_message);


                    //아이템 락이 성공 하면 케스팅 창을 띄운다
                    if( ItemManager::Instance()->SetItemLock(SlotIdx, AtPos, true) )
                    {
                        ItemManager::Instance()->SetPendingUseItemSlotIdx(SlotIdx);
                        ItemManager::Instance()->SetPendingUseItemPos(AtPos);

                        StartLeaveFieldCastTimer(eDoType_Delay_SpaWarp_Dialog, DELAY_LEAVE_FIELD_TICK);
                    }
                }
                else
                {
                    // 5360 필드에서만 사용할 수 있습니다.
                    GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5360));
                }

                
            }
            break;
        case eITEMTYPE_SPA_TICKET:
            {
                if (GlobalFunc::IsSpaScene())
                {
                    MSG_CG_ITEM_SPA_COSTUME_CHANGE_SYN packet;
                    packet.slotcontainer_type = SlotIdx;
                    packet.item_pos = AtPos;
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
                }
            }
            break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
	default:
		{

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
            if( pItemInfo->m_byWasteType == eITEMWASTE_EXTEND_DATE_WEAPON ||        //< 기간 연장 아이템(무기).
                pItemInfo->m_byWasteType == eITEMWASTE_EXTEND_DATE_ARMOR ||         //< 기간 연장 아이템(방어구).
                pItemInfo->m_byWasteType == eITEMWASTE_EXTEND_DATE_RIDER ||         //< 기간 연장 아이템(탈것).
                pItemInfo->m_byWasteType == eITEMWASTE_EXTEND_DATE_RIDER_PARTS      //< 기간 연장 아이템(탈것의 파츠).
                )
            {
                SetUseExtendItemDateMode( AtPos );
                return TRUE;
            }
#endif// _NA_0_20111012_ITEM_DATE_EXTEND

            


			if(pItemInfo->m_byWasteType == eITEMWASTE_RETURNSCROLL)
			{
				if (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE )
				{
					// 현재 마을에 있다.
					return FALSE;
				}

				keyMsg sendMsg;
				ZeroMemory( &sendMsg, sizeof( sendMsg ) );
				sendMsg.dwType		= eDoType_Delay_Use_Return_Stone;
				sendMsg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
				sendMsg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
				sendMsg.wParam			= eDoType_Delay_Use_Return_Stone;
				g_KeyQueueManager.PushBack( sendMsg );



				//아이템 락이 성공 하면 케스팅 창을 띄운다
				if( ItemManager::Instance()->SetItemLock(SlotIdx, AtPos, true) )
				{
                    ItemManager::Instance()->SetPendingUseItemSlotIdx(SlotIdx);
                    ItemManager::Instance()->SetPendingUseItemPos(AtPos);

					StartLeaveFieldCastTimer( eDoType_Delay_Use_Return_Stone, DELAY_LEAVE_FIELD_TICK );
				}
			}
			else if ( pItemInfo->m_byWasteType == eITEMWASTE_QUESTACCEPT )
			{
				uiQuestMan* pQuestMan = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
				if (pQuestMan)
				{
					pQuestMan->NET_SEND_CG_QUEST_ACCEPT_SYN(pItemInfo->m_wQuestCode, 1, AtPos);
				}				
			}
			else
			{
				if( pItemInfo->m_byPopup != 0 )
				{
					TCHAR szText[INTERFACE_STRING_LENGTH];
					TCHAR szBuf[INTERFACE_STRING_LENGTH];
					TCHAR szItemName[MAX_PATH + 1];

					g_InterfaceManager.GetInterfaceString( 5953, szBuf, INTERFACE_STRING_LENGTH - 1 );					
					g_InterfaceManager.GetItemName( pItemInfo->m_Code, szItemName, MAX_PATH );

					Snprintf( szText, INTERFACE_STRING_LENGTH - 1, szBuf, szItemName );

					m_SendSlotIdx = SlotIdx;
					m_SendAtPos	  = AtPos;	 	

					GlobalFunc::SysConfirmBox( szText, GlobalFunc::SendCheckUseItemByPopup );

				}
				else
				{
					ItemManager::Instance()->SendItemUseMsg( SlotIdx, AtPos );
				}
			}
		}
		break;
	}

	return TRUE;
}

BOOL MouseHandler::UseTransationItemSSQ(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer* pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot& rItemSlot = (SCItemSlot&)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO* pItemInfo = rItemSlot.GetItemInfo();
	if (!pItemInfo )
    {
		return FALSE;
    }


	if (GameFramework::GetCurrentScene() && 
		(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
		SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType()))
	{					
		//5745	배틀존에서는 사용할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5745));
		return TRUE;
	}

	if (FALSE == GetTryUseChageItem())
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);
		TCHAR szText[INTERFACE_STRING_LENGTH];
		TCHAR szItemName[INTERFACE_STRING_LENGTH];
		TCHAR szBuf[INTERFACE_STRING_LENGTH];

		// 5953	[%s]아이템을 사용하시겠습니까?
		g_InterfaceManager.GetInterfaceString(5953, szBuf, INTERFACE_STRING_LENGTH);					
		g_InterfaceManager.GetItemName(rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH);
		Snprintf(
			szText, 
			INTERFACE_STRING_LENGTH-1, 
			szBuf, 
			szItemName);

		GlobalFunc::SysConfirmBox(szText, GlobalFunc::_fn_UseSSQItem);

	}

	return TRUE;
}



BOOL MouseHandler::_useTransactionItemTopOfTrial(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if ( !pItemInfo )
		return FALSE;


	if (GameFramework::GetCurrentScene() && 
		(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
		SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
	{					
		//5745	배틀존에서는 사용할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5745));
		return TRUE;
	}

	if ( FALSE == GetTryUseChageItem() )
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);
		TCHAR szText[INTERFACE_STRING_LENGTH];
		TCHAR szItemName[INTERFACE_STRING_LENGTH];
		TCHAR szBuf[INTERFACE_STRING_LENGTH];

		// 5953	[%s]아이템을 사용하시겠습니까?
		g_InterfaceManager.GetInterfaceString(5953, szBuf, INTERFACE_STRING_LENGTH);					
		g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH );
		Snprintf(
			szText, 
			INTERFACE_STRING_LENGTH-1, 
			szBuf, 
			szItemName);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		pSystemMan->Sys_Confirm(szText, GlobalFunc::_fn_UseTopOfTrialItem);
	}

	return TRUE;
}	



BOOL MouseHandler::_useTransactionItemDuraRepair(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if ( !pItemInfo )
		return FALSE;

	if ( FALSE == GetTryUseChageItem() )
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);
		TCHAR szText[INTERFACE_STRING_LENGTH];
		TCHAR szItemName[INTERFACE_STRING_LENGTH];
		TCHAR szBuf[INTERFACE_STRING_LENGTH];

		// 5953	[%s]아이템을 사용하시겠습니까?
		g_InterfaceManager.GetInterfaceString(5953, szBuf, INTERFACE_STRING_LENGTH);					
		g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH );
		Snprintf(
			szText, 
			INTERFACE_STRING_LENGTH-1, 
			szBuf, 
			szItemName);

		if(ItemManager::Instance()->IsExistLimitedItem( SI_EQUIPMENT ))
		{
			ZeroMemory(szBuf, INTERFACE_STRING_LENGTH);
			//	6056	# 리미티드아이템은 전체 수리 시 일괄적으로 최대내구도가 감소하게 됩니다.
			g_InterfaceManager.GetInterfaceString(6056, szBuf, INTERFACE_STRING_LENGTH);
			strcat(szText, szBuf);
		}

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		pSystemMan->Sys_Confirm(szText, GlobalFunc::UseChargeItem);
	}

	return TRUE;
}	

//----------------------------------------------------------------
BOOL MouseHandler::_useTransactionChargeItemPersonalWaypoint(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	// todo : 우클릭한 아이템을 활성화 한다.
	// 활성화된 아이템일 경우 리턴
	// 활성화되지 않았지만 이미 활성화된 아이템이 있을 경우 리턴
	if (GlobalFunc::IsWayPointItemPending())
    {
		return TRUE;
    }

	SCSlotContainer* pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
    if (!pAtContainer)
    {
        return FALSE;
    }

	SCItemSlot& rItemSlot = (SCItemSlot&)pAtContainer->GetSlot(AtPos);
	
	if (rItemSlot.IsProgressTime())
    {
		return TRUE;
    }

	for (int i = 0; i < pAtContainer->GetMaxSlotNum(); ++i)
	{
		if (pAtContainer->IsEmpty(i))
        {
			continue;
        }

		SCSlot& rSlot = pAtContainer->GetSlot(i);
		if (rSlot.GetSlotType() == ST_ITEM)
		{
			SCItemSlot& ItemSlot = (SCItemSlot&)rSlot;
			if (ItemSlot.GetItemInfo()->m_wType == eITEMTYPE_WAYPOINT)
			{
				if (ItemSlot.IsProgressTime())
				{
					return TRUE;
				}
			}
		}			
	}

	ItemManager::Instance()->SendItemUseMsg(SlotIdx, AtPos);
	GlobalFunc::SetWayPointItemPending(true);

	return TRUE;
}


//----------------------------------------------------------------
/** RETURN : 처리했으면 TRUE, 처리 하지 않았으면 FALSE
*/
BOOL MouseHandler::_useTransactionChargeItemPortalScroll(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();	
	if ( !pItemInfo )
		return FALSE;

	if ( pItemInfo->m_wType != eITEMTYPE_SAVE_COORD)
	{
		return TRUE;	// 이건 따로 처리할것이 없고 사용시에도 아무런 짓도 하지말자. 
	}

	if (GameFramework::GetCurrentScene() && 
		(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
		 SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
	{					
		//5745	배틀존에서는 사용할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5745));
		return TRUE;
	}

	if (rItemSlot.GetMapCode())
	{ // 저장된 상태 ( 포탈 시도를 한다 ).
		// 아이템이 있냐?
		POSTYPE OutPos = INVALID_SLOT_POS;
		BOOL bRet = ItemManager::Instance()->FindFirstItem( eITEMTYPE_PORTAL, OutPos );
		if ( !bRet )
		{
			// 포탈아이템이 없습니다.( 5931 )
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5931));
			return TRUE;
		}

		if ( FALSE == GetTryPortal() )
		{
			SetTryPortal(TRUE);
			SetPendingUseCoordPortalPos(AtPos);

			TCHAR szText[INTERFACE_STRING_LENGTH];
			TCHAR szMapName[INTERFACE_STRING_LENGTH];
			TCHAR szTitle[INTERFACE_STRING_LENGTH];

			const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( rItemSlot.GetMapCode() );
			if ( pMapInfo )
			{
				g_InterfaceManager.GetInterfaceString( pMapInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH );				
				//5946	%s (%d, %d)지점으로 이동하시겠습니까?
				TCHAR szBuf[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString(5946, szBuf, INTERFACE_STRING_LENGTH);					
				Snprintf( szText, INTERFACE_STRING_LENGTH, 
					szBuf, szMapName, rItemSlot.GetX(), rItemSlot.GetY()) ;

				// 저장된 위치로 이동 5932
				g_InterfaceManager.GetInterfaceString(5932, szTitle, INTERFACE_STRING_LENGTH);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Confirm(NULL, GlobalFunc::UsePoralCoordItem, szTitle, szText);
			}
		}
	}
	else
	{
		// 저장되지 않은 상태 ( 포탈아이템에 위치 저장 )
		if ( FALSE == GetSavingPosition() )
		{	
			SetPendingSavePositionPos( AtPos );	
			SetSavingPosition(TRUE);

			TCHAR szText[INTERFACE_STRING_LENGTH];
			TCHAR szBuf[INTERFACE_STRING_LENGTH];
			TCHAR szMapName[INTERFACE_STRING_LENGTH];

			const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( g_pMap->GetMapID() );
			if ( pMapInfo )
			{
				WzVector wzPos = g_pHero->GetPosition();
				g_InterfaceManager.GetInterfaceString( pMapInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH );				

				// %s (%d, %d) 좌표로 저장 하시겠습니까? - 5934
				g_InterfaceManager.GetInterfaceString( 5934, szBuf, INTERFACE_STRING_LENGTH );				
				Snprintf( szText, INTERFACE_STRING_LENGTH, szBuf, szMapName, (SHORT)wzPos.x, (SHORT)wzPos.y) ;

				// 현재위치 저장 ( 5933 )
				g_InterfaceManager.GetInterfaceString( 5933, szBuf, INTERFACE_STRING_LENGTH );				

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
                    pSystemMan->Sys_Confirm(NULL, GlobalFunc::SaveCurrentPosAtItem, szBuf, szText);

			}
		}
	}

	return TRUE;
}





BOOL MouseHandler::_useTransactionChargeItemStatInit(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if ( !pItemInfo )
    {
		return FALSE;
    }

	if ( pItemInfo->m_wType != eITEMTYPE_STAT_RESET)
	{
		return FALSE;	// 이건 따로 처리할것이 없고 사용시에도 아무런 짓도 하지말자. 
	}

    if ((g_pHero != NULL) && (g_pHero->IsTransForm() == TRUE))
    {
        uiSystemMan* system_main = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if (system_main != NULL)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };						
            // 5139	변신 상태에서는 이용이 가능하지 않습니다.
            g_InterfaceManager.GetInterfaceString(5139, message ,INTERFACE_STRING_LENGTH);
            system_main->Sys_VerifyLock(message);
        }
        return FALSE;
    }

	if ( FALSE == GetTryUseChageItem() )
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);

		TCHAR szText[INTERFACE_STRING_LENGTH];
		TCHAR szItemName[INTERFACE_STRING_LENGTH];
		TCHAR szBuf[INTERFACE_STRING_LENGTH];	

		// 아이템 WASTE TYPE 에 따라서 모든 스텟 초기화는 개수 입력하지않고 특정 패킷을 바로 보낸다.
		eWASTETYPE_FOR_STAT_INIT eStatInitType = (eWASTETYPE_FOR_STAT_INIT)pItemInfo->m_byWasteType;

		switch (eStatInitType)
		{
		case eItemWASTE_INIT_ALL:
		case eItemWASTE_INIT_BASIC_STAT_ALL:
		case eItemWASTE_INIT_EXPERTY_ALL:
		
			{
				// 5953	[%s]아이템을 사용하시겠습니까?
				g_InterfaceManager.GetInterfaceString(5953, szBuf, INTERFACE_STRING_LENGTH);					
				g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH );
				Snprintf(
						szText, 
						INTERFACE_STRING_LENGTH-1, 
						szBuf, 
						szItemName);
				

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szText, GlobalFunc::UseChargeItemStatInitAll);

			}
			break;

		default:
			{
				//5725	[%s] 아이템을 몇 개 사용하시겠습니까?				
				g_InterfaceManager.GetInterfaceString(5725, szBuf, INTERFACE_STRING_LENGTH);					
				g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH );
				Snprintf(
					szText, 
					INTERFACE_STRING_LENGTH-1, 
					szBuf, 
					szItemName );

				TCHAR szNum[INTERFACE_STRING_LENGTH];
				_itot( rItemSlot.GetNum(), szNum, 10);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_ConfirmMessage(szText, GlobalFunc::GetStateInitItemNumConfirm, szNum, true);

			}
			break;
		}			
		
	}

	return TRUE;
}	



BOOL MouseHandler::_useTransactionChargeItemTransact(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if ( !pItemInfo )
		return FALSE;

	if ( FALSE == GetTryUseChageItem() )
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);

		TCHAR szText[INTERFACE_STRING_LENGTH];
		TCHAR szItemName[INTERFACE_STRING_LENGTH];
		TCHAR szBuf[INTERFACE_STRING_LENGTH];

		// 5953	[%s]아이템을 사용하시겠습니까?
		g_InterfaceManager.GetInterfaceString(5953, szBuf, INTERFACE_STRING_LENGTH);					
		g_InterfaceManager.GetItemName( rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH );
		Snprintf(
			szText, 
			INTERFACE_STRING_LENGTH-1, 
			szBuf, 
			szItemName);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Confirm(szText, GlobalFunc::UseChargeItem);
	}

	return TRUE;
}

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::ItemUseTransactionFromQuick( SLOTIDX SlotIdx, POSTYPE AtPos )
{
	// 퀵컨테이너에서 아이템 사용을 할려고 한다.
	QuickContainer * pAtContainer = 
		(QuickContainer * )ItemManager::Instance()->GetItemContainerDialog( SI_QUICK);
	if (!pAtContainer)
	{
		return FALSE;
	}	

	POSTYPE RealQuickPos = pAtContainer->GetCurTabNum();	// 현재 텝위치.
    RealQuickPos = 0;// 두줄일때는 탭위치를 안더한다.
    AtPos = pAtContainer->GetRealSlotPos(AtPos);
    RealQuickPos += AtPos;

	// 공통 선행 조건

	if (RealQuickPos > pAtContainer->GetMaxSlotNum() )
	{
		return FALSE;
	}

	if( pAtContainer->IsEmpty( RealQuickPos ) ) 
	{
		return FALSE;
	}

	QuickSlot & rFromSlot = (QuickSlot &)pAtContainer->GetSlot( RealQuickPos );
	POSTYPE OrgPos		= 0;        

	if (SLOT_RENDER_STATE_ACTIVATED !=  rFromSlot.GetStatus())
	{
		return FALSE;
	}

	///////////////////////////////////////////////

	switch (rFromSlot.GetOrgSlotType())
	{
	case ST_ITEM:
		{
			if (SI_INVENTORY != rFromSlot.GetOrgSlotIndex())
			{
				// 이미 다른 (장착)창에 등록되어있는 상태이다.
				return FALSE;
			}

			if (!rFromSlot.GetOrgSlot())
			{
				return FALSE;
			}
			
			const BASE_ITEMINFO* pOrgItemInfo = 
				((SCItemSlot*)rFromSlot.GetOrgSlot())->GetItemInfo();

			if (!pOrgItemInfo)
			{
				return FALSE;
			}

			POSTYPE OrgPos = rFromSlot.GetOrgPos();
			BOOL bRet = TRUE;

			if (pOrgItemInfo->IsCanUseWaste())
			{
				bRet = ItemManager::Instance()->FindFirstItem(rFromSlot.GetOrgCode(), OrgPos);				
			}

			if (bRet)
			{
				//GS_BUG_FIX:에테르 탄환은예외처리,IsCanUseWaste 아이템특성상 불가능
				if (pOrgItemInfo->m_wType == eITEMTYPE_ETHER_BULLET) 
				{
					ItemUseTransactionFromInventory(SI_INVENTORY, rFromSlot.GetOrgPos());
				}
				else
				{
					ItemUseTransactionFromInventory(SI_INVENTORY, OrgPos);	
				}
				
			}							

		}
		break;

	case ST_SKILL:
		{
            // 스킬 사용은 실제 스킬을 이용
            int skill_code = xGetSkillData()->GetRealSkillCode(rFromSlot.GetOrgCode() );
            if( skill_code != INVALID_SKILL_CODE )
            {
                // 액션은 그대로 사용
                ItemUseTransactionFromSkill( skill_code );
            }
            else
            {
                ItemUseTransactionFromSkill( rFromSlot.GetOrgCode() );   // Action
            }
		}
		break;
	}

	return TRUE;
}

BOOL MouseHandler::ItemUseTransactionFromSkill( SLOTCODE SlotCode )
{
	return g_HeroInput.OnSkill( SlotCode );
}

BOOL MouseHandler::ItemUseTransactionFromStyle( SLOTIDX SlotIdx, POSTYPE AtPos )
{
	StyleContainer *pAtContainer = (StyleContainer * )ItemManager::Instance()->GetItemContainerDialog( SI_STYLE);
    if (!pAtContainer)
    {
        return FALSE;
    }

	POSTYPE RealStylePos = pAtContainer->GetCurTabNum();	// 현재 텝위치.
	RealStylePos += AtPos;

	if (RealStylePos > pAtContainer->GetMaxSlotNum() )
	{
		return FALSE;
	}

	if( pAtContainer->IsEmpty( RealStylePos ) ) 
	{
		return FALSE;
	}

	StyleSlot & rFromSlot = (StyleSlot &) pAtContainer->GetSlot( RealStylePos );
	POSTYPE OrgPos		= 0;        

	assert(rFromSlot.GetOrgSlotType() == ST_SKILL);
	// 스타일 선택. 

	SCSkillSlot * pSkillSlot = (SCSkillSlot *)ItemManager::Instance()->GetSkillSlotByCode( rFromSlot.GetOrgCode() );
	if (!pSkillSlot)
	{
		assert(!"이부분이 오다니 확인해보자.!!");
		return FALSE;
	}

	BASE_STYLEINFO * pStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(pSkillSlot->GetCode());

	if(pStyleInfo)
	{
		//스타일지정 
		if(	g_pHero->GetCurrentAttackStyle() != pSkillSlot->GetCode() )
        {
            if( g_pHero->CheckChangeAttackStyle( pSkillSlot->GetCode() ) == TRUE )
	        {
                g_pHero->SetCurrentAttackStyle(pSkillSlot->GetCode());

		        MSG_CG_STYLE_SELECT_STYLE_SYN packet;
		        packet.m_byCategory		= CG_STYLE;
		        packet.m_byProtocol		= CG_STYLE_SELECT_STYLE_SYN;
		        packet.m_NewStyleCode = pSkillSlot->GetCode();

		        if(GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet) )==FALSE)
		        {
			        assert(!"스타일 변경 패킷 전송 실패");
		        }
		        else 
		        {
#ifdef _DEBUG
			        g_InterfaceManager.ShowMessageBox(_T("스타일패킷전송"),1000,NULL,0);
#endif			
		        }
	        }
        }
	}
	else
	{
		assert(0);
	}

	return TRUE;
}

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::ItemCompositeNormalEnchant(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	assert(pAtContainer);
    if (!pAtContainer)
    {
        return FALSE;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );
		return FALSE;
	}

	// 조합아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);
	SCItemSlot & rCrystalItemSlot = (SCItemSlot & )pAtContainer->GetSlot(m_CrystalPos);

	if (!rItemSlot.GetItemInfo()->IsArmor() &&
		!rItemSlot.GetItemInfo()->IsWeapon() 
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
        && !rItemSlot.IsWingItem()
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
        )
	{
		//	인첸트가 가능하지 않은 아이템 입니다!
		g_InterfaceManager.GetInterfaceString( eST_NOT_ENCHANT_ITEM, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );
		return FALSE;
	}

	BYTE byEnchant = rItemSlot.GetEnchant();  
	if ( byEnchant > 8)
	{
		//	상급 인첸트는 제조 NPC에게 문의 하시기 바랍니다.
		g_InterfaceManager.GetInterfaceString( eST_NOT_HERE_HIGH_ENCHANT, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, CHAT_TYPE_SYSTEM );
		return FALSE;
	}

	eNormalEnchantType EnchantType;
	
	// 하 / 중 인첸트 분기
	if ( byEnchant <= 5)					// 0~5는 하급 인첸트 이다.
	{
		// 하급 인첸트 아이템이 이냐?
		sITEMCOMPOSITE * pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(eITEMCOMPOSITE_LOW_ENCHANT);

		if(!pInfo)
			return FALSE;

		if (rCrystalItemSlot.GetCode() != pInfo->m_sCompositeMaterials[0].m_dwCompositeMaterialCode )
		{
			//	하급 인첸트에 필요한 재료가 아닙니다.
			g_InterfaceManager.GetInterfaceString( eST_NOT_LOW_ENCHANT, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}			

		// 필요물품이 있느냐?
		if (!ItemManager::Instance()->IsExistCompositeMaterials(eITEMCOMPOSITE_LOW_ENCHANT, SI_INVENTORY) )
		{
			g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}

        
		// 인첸트 조건에 따라 처리
		EnchantType = LOW_ENCHANT;
	}
	else if (byEnchant <= 8)		// 6~8 은 중급 인첸트이다.
	{
		// 중급 인첸트
		sITEMCOMPOSITE * pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(eITEMCOMPOSITE_MEDIUM_ENCHANT);

		if(!pInfo)
			return FALSE;


		if (rCrystalItemSlot.GetCode() != pInfo->m_sCompositeMaterials[0].m_dwCompositeMaterialCode )
		{
			//	중급 인첸트에 필요한 재료가 아닙니다.
			g_InterfaceManager.GetInterfaceString( eST_NOT_MID_ENCHANT, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}			

		// 필요물품이 있느냐?
		if (!ItemManager::Instance()->IsExistCompositeMaterials(eITEMCOMPOSITE_MEDIUM_ENCHANT, SI_INVENTORY) )
		{
			g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}

		// 인첸트 조건에 따라 처리
		
		EnchantType = MIDDLE_ENCHANT;
	}

	uiAssistMan* pMan = (uiAssistMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
	if(pMan)
	{
		pMan->ShowAskNormalEnchantDialog(
			EnchantType,
			rItemSlot.GetCode(), 
			MouseHandler::Instance()->GetCrystalPos(),	// 유저에게서 받아놓은 보석위치
			TargetPos
		);
	}

	MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);

    return TRUE;

}

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::ItemExtension(SLOTIDX SlotIdx, POSTYPE AtPos, RECT * pRect)
{


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //! 연장은 아이템으로 별도 판매
#else

	SCSlotContainer *	pToContainer = ItemManager::Instance()->GetContainer(SlotIdx); 
	uiCashItemShopExtensionBuy * pUICashITemShopExtensionBuy 
		= (uiCashItemShopExtensionBuy *)ItemManager::Instance()->GetItemContainerDialog(SlotIdx);

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if( IsExistItemAtHand() )
	{ 
        SCItemSlot* from_item_slot = static_cast<SCItemSlot*>(m_MouseItem.m_pFromSlot);

		if (from_item_slot == NULL)
		{
			return FALSE;
		}

		if (from_item_slot->GetItemInfo() == NULL)
		{
			return FALSE;
		}

		if (ST_ITEM != from_item_slot->GetSlotType())
		{
			return FALSE;
		}

        if (ItemManager::CanPeriodExtend(from_item_slot) == false)
		{
			// 5899	기간 연장이 가능한 아이템이 아닙니다.
			const int c_iCannotInchantLevel = 5899;
			g_InterfaceManager.GetInterfaceString(c_iCannotInchantLevel, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, INTERFACE_STRING_LENGTH);
			return FALSE;
		}

		if(!from_item_slot->IsExpiredWaitTime())
		{
			// 5898	사용기간이 만료되지 않은 아이템입니다.
			const int c_iCannotInchantLevel = 5898;
			g_InterfaceManager.GetInterfaceString(c_iCannotInchantLevel, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, INTERFACE_STRING_LENGTH);
			return FALSE;
		}

        if (from_item_slot->CanUseMixture(MIXTURE_PERIOD_EXTEND) == false)
		{
			g_InterfaceManager.GetInterfaceString(5937, szMessage, INTERFACE_STRING_LENGTH);	//5937	해당 기능을 이용할 수 있는 아이템 종류가 아닙니다.
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage, INTERFACE_STRING_LENGTH);

			return FALSE;
		}

		if ( SI_INVENTORY != m_MouseItem.m_fromSlotIdx && SI_EQUIPMENT != m_MouseItem.m_fromSlotIdx )
		{
			return FALSE;
		}

		for (int i = 0; i < pUICashITemShopExtensionBuy->GetMaxSlotNum(); ++i)
		{
			if (pUICashITemShopExtensionBuy->IsEmpty(i))
			{
				continue;
			}
			pUICashITemShopExtensionBuy->DeleteSlot(i, NULL);
		}

		ItemManager::Instance()->CopyItem(
			m_MouseItem.m_fromSlotIdx, 
			SI_EXTENSION,
			m_MouseItem.m_fromPos,
			AtPos );

		pUICashITemShopExtensionBuy->SetTargetInfo( m_MouseItem.m_fromSlotIdx, this->m_MouseItem.m_fromPos );
	}
	else
	{
		// 다시 들어올필요가 없다.
		return FALSE;
	}


#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


	return TRUE;
}

//----------------------------------------------------------------
/** 
*/
#ifdef __NA_001358_ENCHANT_UNIFICATION
#else //__NA_001358_ENCHANT_UNIFICATION
BOOL				
MouseHandler::ItemCompositeNewEnchant(SLOTIDX SlotIdx, POSTYPE AtPos)
{
    if( IsExistItemAtHand() == FALSE )
    { 
        return FALSE;
    }

    if ( !m_MouseItem.m_pFromSlot )
    {
        return FALSE;
    }

    return ItemCompositeNewEnchant_InventoryRightClick( this->m_MouseItem.m_toSlotIdx, SlotIdx, this->m_MouseItem.m_fromSlotIdx, AtPos );
}
#endif //__NA_001358_ENCHANT_UNIFICATION

//------------------------------------------------------------------------------
bool MouseHandler::ItemCompositeNewEnchant_InventoryRightClick(const SLOTIDX from_container_index, 
                                                               const SLOTIDX to_container_index, 
                                                               const POSTYPE from_position_in_container, 
                                                               const POSTYPE to_position_in_container)
{
    if (from_container_index != SI_INVENTORY)
    {
        //! 인첸트는 인벤토리에 있는 아이템만 가능하다. : 5307
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5307));
        return false;
    }


    if (to_container_index != SI_ENCHANT && to_container_index != SI_ENCHANT_OPTION )
    {
        return false;
    }

    EnchantDialog* enchant_dialog = (EnchantDialog *)ItemManager::Instance()->GetItemContainerDialog(to_container_index);
    if( enchant_dialog == NULL )
    {
        return false;
    }


    // 인챈트 대상 슬롯
    if (to_container_index == SI_ENCHANT )
    {
        SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index);
        SCSlotContainer* enchant_container_ptr = ItemManager::Instance()->GetContainer(to_container_index); 
        if ((inventory_container_ptr == NULL) || 
            (inventory_container_ptr->IsEmpty(from_position_in_container) == TRUE) || 
            (enchant_container_ptr == NULL))
        {
            return false;
        }

        SCItemSlot* item = static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_position_in_container)));

        if ((item == NULL) || 
            (item->GetItemInfo() == NULL) || 
            (item->GetSlotType() != ST_ITEM))
        {
            return false;
        }

        if (item->IsLocked())
        {
            return false;
        }

        if ((item->GetItemInfo()->IsWeapon() == FALSE) &&
            (item->GetItemInfo()->IsArmor() == FALSE)
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
            && (item->IsWingItem() == FALSE)
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
            )
        {
            return false;
        }

        if (enchant_container_ptr->IsEmpty(to_position_in_container) == FALSE)
        {
            enchant_dialog->ReTry();
        }

        TCHAR error_message[INTERFACE_STRING_LENGTH] = {NULL,};

        if ((item->IsFateItem() == TRUE) && 
            (item->GetFateIdentify() == FALSE))
        {
            //6329	인증 의식이 완료된 아이템만 인첸트가 가능합니다.
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6329));
            return false;
        }

        if (item->GetItemInfo()->m_LV == 0)
        {
            // 아이템 레벨이 0이면 인첸트 하지 못한다. : 5306
            g_InterfaceManager.GetInterfaceString(5306, error_message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, error_message);
            return false;
        }

        if (item->GetEnchant() >= MAX_ENCHANT_GRADE )
        {
            // 최고 인첸트입니다!
            g_InterfaceManager.GetInterfaceString(eST_MAX_ENCHANT_LEVEL, 
                                                error_message, 
                                                INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, error_message);
            return false;
        }

        return true;
    }
#ifdef _NA_003966_20111227_ADD_ENCHANT
    else if( to_container_index == SI_ENCHANT_OPTION )
    {
        if( const MATERIALS_FOR_LEVEL* const enchant_material_info = enchant_dialog->GetCurrentEnchantInfo() )
        {
            if (enchant_material_info->IsDownGrade() == false)
            {
                return false;
            }

            if( SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index) )
            {
                SCItemSlot* from_itemslot_ptr = static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_position_in_container)));

                if ((from_itemslot_ptr == NULL) || 
                    (from_itemslot_ptr->GetItemInfo() == NULL) || 
                    (from_itemslot_ptr->GetSlotType() != ST_ITEM))
                {
                    return false;
                }

                const BASE_ITEMINFO* const down_guard_item_info = from_itemslot_ptr->GetItemInfo();
                if (down_guard_item_info->m_wType != eITEMTYPE_ENCHANT)
                {
                    return false;
                }

                if( enchant_material_info->GetDownGradeType() == 0 )
                {
                    return false;
                }

                if (down_guard_item_info->m_wSubType != enchant_material_info->GetDownGradeType())
                {
                    return false;
                }

                return true;
            }
        }
    }
#endif //_NA_003966_20111227_ADD_ENCHANT

    return false;
}

//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
bool MouseHandler::ItemEquipmentAwakening_InventoryRightClick( const SLOTIDX from_container_index, const SLOTIDX to_container_index, const POSTYPE from_position_in_container, const POSTYPE to_position_in_container )
{
	if (from_container_index != SI_INVENTORY)
	{
		// 3967 : 각성은 인벤토리에 있는 아이템만 가능합니다
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(3967));
		return false;
	}

	if (to_container_index != SI_EQUIPMENT_AWAKENING)
	{
		return false;
	}

	uiEquipmentAwakeningDialog* equipment_wakeup_dialog = static_cast<uiEquipmentAwakeningDialog*>(ItemManager::Instance()->GetItemContainerDialog(to_container_index));
	if( equipment_wakeup_dialog == NULL )
	{
		return false;
	}

	// 진화 대상 슬롯
	if (to_container_index == SI_EQUIPMENT_AWAKENING )
	{
		SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index);
		SCSlotContainer* equipment_awakening_container = ItemManager::Instance()->GetContainer(to_container_index); 
		if ((inventory_container_ptr == NULL) || 
			(inventory_container_ptr->IsEmpty(from_position_in_container) == TRUE) || 
			(equipment_awakening_container == NULL))
		{
			return false;
		}

		SCItemSlot* item = static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_position_in_container)));

		if ((item == NULL) || 
			(item->GetItemInfo() == NULL) || 
			(item->GetSlotType() != ST_ITEM))
		{
			return false;
		}

		if (item->IsLocked())
		{
			return false;
		}
	}

	return true;
}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
//-------------------------------------------------------------------------------------------
/**
*/

#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
bool MouseHandler::ItemEquipmentEvolution_InventoryRightClick( const SLOTIDX from_container_index, const SLOTIDX to_container_index, const POSTYPE from_position_in_container, const POSTYPE to_position_in_container )
{
	if (from_container_index != SI_INVENTORY)
	{
		// 3968 : 진하는 인벤토리에 있는 아이템만 가능합니다
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(3968));
		return false;
	}

	if (to_container_index != SI_EQUIPMENT_EVOLUTION)
	{
		return false;
	}

	uiEquipmentAwakeningDialog* equipment_wakeup_dialog = static_cast<uiEquipmentAwakeningDialog*>(ItemManager::Instance()->GetItemContainerDialog(to_container_index));
	if( equipment_wakeup_dialog == NULL )
	{
		return false;
	}

	// 인챈트 대상 슬롯
	if (to_container_index == SI_EQUIPMENT_EVOLUTION )
	{
		SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index);
		SCSlotContainer* equipment_evolution_container = ItemManager::Instance()->GetContainer(to_container_index); 
		if ((inventory_container_ptr == NULL) || 
			(inventory_container_ptr->IsEmpty(from_position_in_container) == TRUE) || 
			(equipment_evolution_container == NULL))
		{
			return false;
		}

		SCItemSlot* item = static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_position_in_container)));

		if ((item == NULL) || 
			(item->GetItemInfo() == NULL) || 
			(item->GetSlotType() != ST_ITEM))
		{
			return false;
		}

		if (item->IsLocked())
		{
			return false;
		}
	}

	return true;
}
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

//----------------------------------------------------------------
/** 
*/
BOOL MouseHandler::ItemCompositeSocketFill(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	assert(pAtContainer);
    if (!pAtContainer)
    {
        return FALSE;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	// 조합아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);

	if (!rItemSlot.GetItemInfo()->IsArmor() &&
		!rItemSlot.GetItemInfo()->IsWeapon() )
	{
		//	인첸트가 가능하지 않은 아이템 입니다!
		g_InterfaceManager.GetInterfaceString( eST_NOT_ENCHANT_ITEM, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	if (rItemSlot.GetSocketNum() <= 0)
	{
		//	채워 넣을 소켓 자리가 없습니다!
		g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_SOCKET_FOR_INSERT, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	uiAssistMan* pMan = (uiAssistMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
	if(pMan)
	{
		pMan->ShowAskSocketFillDialog(			
			rItemSlot.GetCode(), 
			MouseHandler::Instance()->GetCrystalPos(),	// 유저에게서 받아놓은 보석위치
			TargetPos
		);
	}
	MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);

    return TRUE;	
}
//--------------------------------------------------------------------------------------------------
/** 소켓 확인 이벤트
*/
bool MouseHandler::ItemSocketIndentifyTransction(POSTYPE slot_pos)
{
    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    RollbackItemAtHand();

    POSTYPE use_pos = GetPendingUseChargeItemPos();

    if (ItemManager::Instance()->IsInvalidSocketIdentify(slot_pos, use_pos) == false)
    {
        return false;
    }

    SCSlotContainer* container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (container == NULL)
    {
        return false;

    }
    SCItemSlot& use_slot = static_cast<SCItemSlot&>(container->GetSlot(slot_pos));
    if (use_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }

    if (use_slot.GetSocketIdentifyType() != eSOCKET_UNIDENTIFIED_ITEM)
    {
        return false;
    }

    if (use_slot.IsFateItem() && use_slot.GetFateIdentify() == false)
    {
        return false;
    }

    uiAssistMan* assist_man = static_cast<uiAssistMan*>(
        g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  
    if (assist_man == NULL)
    {
        return false;
    }

    assist_man->ShowAskSocketIdentifyDialog(use_slot.GetItemInfo()->m_Code, use_pos, slot_pos);
    return true;
}
//--------------------------------------------------------------------------------------------------
/** 아이템 결정화 이벤트
*/
bool MouseHandler::ItemCrystalizationTransction( POSTYPE TargetPos )
{
    RollbackItemAtHand();

    SCSlotContainer* container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (container == NULL)
    {
        return false;
    }
    SCItemSlot& use_slot = static_cast<SCItemSlot&>(container->GetSlot(TargetPos));
    if (use_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }

    const BASE_ITEMINFO* selected_item_info_ptr = use_slot.GetItemInfo();

    if (selected_item_info_ptr == NULL)
    {
        return false;
    }

    if (use_slot.CanUseMixture(MIXTURE_CRYSTALIZE) == false)
    {
        return false;
    }

    // 이미 진행중이면 취소
    float rate = 100.f;
    if (ItemCooltimeManager::Instance()->FindCooltime(use_slot.GetSerial(), &rate))
    {
        if (ItemCooltimeManager::Instance()->CancelCooltime(use_slot.GetSerial()))
        {
            // 5850 : 선택한 아이템의 결정화가 취소되었습니다.
            TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(5850, format_string);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, format_string);
            return true;
        }
    }

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(use_slot, use_slot.GetEnchant());
#else
    SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(use_slot);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
    if (slot_code == 0)
    {
        return false;
    }

    sITEM_CRYSTALIZE_LIST* item_crystalize_list = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo(slot_code);
    if (item_crystalize_list == NULL)
    {
        return false;
    }

    if ((use_slot.GetEnchant() >= 10) || 
        selected_item_info_ptr->m_wSubType == eITEMSUBTYPE_ELITE || 
        selected_item_info_ptr->m_wSubType == eITEMSUBTYPE_UNIQUE                    
        )
    {
        // Callback 메시지로 연결
        uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };        
        TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };
        TCHAR result_item_name[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetItemName(selected_item_info_ptr->m_Code, item_name);
        Snprintf(result_item_name, INTERFACE_STRING_LENGTH-1, "%s+%d", item_name, use_slot.GetEnchant());
        
        // 5847 : 정말로 [%s] 아이템을 결정화 하시겠습니까? 결정화가 완료되면 아이템을 복구할 수 없습니다.
        g_InterfaceManager.GetInterfaceString(5847, format_string);
        Snprintf(result_message, INTERFACE_STRING_LENGTH-1, format_string, result_item_name);
        ui_system_manager_ptr->Sys_SystemMessage(result_message, 
            NULL, 
            uiItemCompositeMan::_Callback_Crystalization_Confirm,
            uiSystemMessage::kConfirm_Type,
            true,
            0,
            100,
            155536,
            155537,
            TargetPos
            );

        return true;
    }

    InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG(IM_INVENTORY_MANAGER::INVEN_DLG));
    if (inventory_dialog_ptr)
    {
        CooltimeStruct data;
        ZeroMemory(&data, sizeof(data));
        data.type = kCrystalization;
        //data.max_time = 8000;
        SkillScriptInfo* skill_script_info_ptr = 
            SkillInfoParser::Instance()->GetSkillInfo(35024);
        data.max_time = static_cast<float>(skill_script_info_ptr ? skill_script_info_ptr->m_dwCoolTime : 8000);
        data.Crystalization.slot_code = slot_code;
        data.Crystalization.target_pos = TargetPos;
        data.slot_index = SI_INVENTORY;
        data.item_pos = TargetPos;
        data.dealy_time = 1000;
        data.serial_key = use_slot.GetSerial();
        ItemCooltimeManager::Instance()->InsertItem(&data);
    }

    return true;
}
//--------------------------------------------------------------------------------------------------
/** 헤르메시즘 성서 이벤트
*/
bool MouseHandler::ItemSocketRecreateTransction(POSTYPE slot_pos)
{
    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    RollbackItemAtHand();

    POSTYPE use_pos = GetPendingUseChargeItemPos();

    SCSlotContainer* container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (container == NULL)
    {
        return false;
    }
    SCItemSlot& use_slot = static_cast<SCItemSlot&>(container->GetSlot(slot_pos));
    if (use_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }

    const BASE_ITEMINFO* selected_item_info_ptr = use_slot.GetItemInfo();

    if (selected_item_info_ptr == NULL)
    {
        return false;
    }

    if (!selected_item_info_ptr->IsArmor() && !selected_item_info_ptr->IsWeapon())
    {
        return false;
    }

    if (use_slot.CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false)
    {
        return false;
    }

    const BASE_ITEMINFO* request_item_info = NULL;
    SCItemSlot& from_slot = static_cast<SCItemSlot&>(container->GetSlot(use_pos));
    request_item_info = from_slot.GetItemInfo();

    // 소켓 생성 확률 정보를 찾을 수 있는지 검사한다.
    if (request_item_info)
    {
        const SocketItemRatioParser* const ratio_parser = SocketItemRatioParser::Instance();
        SocketItemRatioInfo::RatioType::Value ratio_type = 
            ratio_parser->GetRatioType(selected_item_info_ptr->m_wType, true);
        const SocketItemRatioInfo::RatioType::Value fixed_ratio_type = request_item_info->m_wSubType;
        if (fixed_ratio_type != 0)
        {
            ratio_type = fixed_ratio_type;
            if (selected_item_info_ptr->IsArmor())
            {
                ++ratio_type;
            }
        }
        const SocketItemRatioInfo* const target_ratio = 
            ratio_parser->FindSocketRatio(ratio_type, selected_item_info_ptr->m_LV);
        if (target_ratio == NULL)
        {
            return false;
        }
    }
    

    uiAssistMan* assist_man = static_cast<uiAssistMan*>(
        g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  
    if (assist_man == NULL)
    {
        return false;
    }

    assist_man->ShowAskSocketRecreateDialog(use_slot.GetItemInfo()->m_Code, use_pos, slot_pos);
    return true;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL				
MouseHandler::ItemCompositeExtraStoneUse(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	assert(pAtContainer);
    if (!pAtContainer)
    {
        return FALSE;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	// 조합아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);

	if (!rItemSlot.GetItemInfo()->IsArmor() &&
		!rItemSlot.GetItemInfo()->IsWeapon() && 
		!rItemSlot.GetItemInfo()->IsAccessory() && 
		!rItemSlot.GetItemInfo()->IsSpecialAccessory() )
	{
		//	엑스트라스톤 사용이 불가능 합니다.	: 5308
		const int c_iCannotUseExtraStone = 5308;
		g_InterfaceManager.GetInterfaceString( c_iCannotUseExtraStone, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	int iValueIndex = rItemSlot.GetNOptionValueIndex();
	if( iValueIndex >= sEXTRASTONE_OPT_INFO::MAX_VALUE_NUM-1 )
	{
		//5123	더 이상 옵션을 추가할 수 없습니다.
		g_InterfaceManager.GetInterfaceString( 5123, szMessage, INTERFACE_STRING_LENGTH);
		//Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("더 이상 옵션 추가를 하실 수 없습니다.") );
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
		return FALSE;
	}

	uiAssistMan* pMan = (uiAssistMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
	if(pMan)
	{
		pMan->ShowAskExtraStoneUse(			
			rItemSlot.GetCode(), 
			MouseHandler::Instance()->GetCrystalPos(),	// 유저에게서 받아놓은 보석위치
			TargetPos
		);
	}

	return TRUE;
}

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
BOOL MouseHandler::ItemCompositeAddRandomOption(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	if ( MouseHandler::Instance()->GetTryAddRandomOption() )
	{
		return FALSE;
	}

	MouseHandler::Instance()->SetTryAddRandomOption(TRUE);

	SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	assert(pAtContainer);
	if (!pAtContainer)
    {
		return FALSE;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szItemName[INTERFACE_STRING_LENGTH];
	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	// 조합아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);

	if ( !rItemSlot.GetItemInfo()->IsCanEquip() )
	{
		//	해당 아이템에는 사용이 불가능 합니다.
		return FALSE;
	}

	if ( rItemSlot.GetSocketNum() > SOCKET_MAX)
	{
		// 더 이상 옵션을 추가할 수 없습니다.
		return FALSE;
	}

	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	//5148	[%s]에 랜덤옵션을 추가하시겠습니까?
	g_InterfaceManager.GetInterfaceString( 5148 , szTemp ,INTERFACE_STRING_LENGTH);
	g_InterfaceManager.GetItemName(rItemSlot.GetCode(), szItemName, INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szItemName);


	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Confirm(szMessage, GlobalFunc::AddRandomOption);


	return TRUE;
}
#endif

/** ItemCompositeRankUPTransction 
	랭크업 관련된 처리만!
	y2jinc
*/

bool MouseHandler::ItemCubeTransection(SLOTIDX slot_idx, POSTYPE to_position, RECT* pRect)
{
    SCSlotContainer* to_container = ItemManager::Instance()->GetContainer(slot_idx); 
    if (to_container == NULL)
    {
        return false;
    }

    DummyItemSlot DummyItem;
    TCHAR message[INTERFACE_STRING_LENGTH];

    if(IsExistItemAtHand())
    { 
        if ( SI_INVENTORY != m_MouseItem.m_fromSlotIdx )
        {
            //	아이템 조합은 인벤토리에 있는 아이템만 가능합니다!
            g_InterfaceManager.GetInterfaceString(eST_ITEM_COMPOSITE_RANKUP_IN_INVENTORY, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            return false;
        }

        if (m_MouseItem.m_pRenderUnit->m_isDiv)
        {
            // 988	창고에서는 아이템 분할이 불가능 합니다. 인벤토리에서 시도해 주세요
            g_InterfaceManager.GetInterfaceString(988 , message ,INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, message);
            return false;
        }

        SCSlotContainer* from_container = ItemManager::Instance()->GetContainer(m_MouseItem.m_fromSlotIdx);

        // 비워있어야 채울수 있다.
        if (!to_container->IsEmpty(to_position))
        {
            //	슬롯이 비어있지 않습니다!
            g_InterfaceManager.GetInterfaceString(eST_TARGET_SLOT_IS_NOT_EMPTY, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            return false;
        }

        SCItemSlot* from_itemslot_ptr = static_cast<SCItemSlot*>(m_MouseItem.m_pFromSlot);
        if (from_itemslot_ptr &&
            (from_itemslot_ptr->IsPetItem()) &&
            (from_itemslot_ptr->IsUserBlocked() == true))
        {
            // 5692 소환된 펫은 합성재료로 사용할 수 없습니다. 펫 소환을 해제한후 다시 시도해 주십시오.
            TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(5692, message_buffer,INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);

            return false;
        }
/*
        uiItemCube* cube_dialog = (uiItemCube*)to_container;
        if(cube_dialog == NULL)
        {
            return false;
        }*/

        //SCItemSlot& from_item_slot = (SCItemSlot&)from_container->GetSlot(m_MouseItem.m_fromPos);

        // 여기에 들어왔다는것은 특정 컨테이너에서 랭크업 컨테이너로 아이템을 가져온것이다.
        switch(m_MouseItem.m_slotType)
        {
        case ST_ITEM:
            {
                assert(m_MouseItem.m_pFromSlot);
                DummyItem.SetOrgSlot(m_MouseItem.m_pFromSlot);
                DummyItem.SetFromContainerIdx(m_MouseItem.m_fromSlotIdx);
                DummyItem.SetFromPosition(m_MouseItem.m_fromPos);		
                to_container->InsertSlot(to_position, DummyItem);
                MouseHandler::Instance()->RollbackItemAtHand();
            }
            break;
        default:
            {
                assert( !"잘못된 타입의 슬롯을 이동했습니다." );
                return false;
            }
        }
        return true;
    }
    return true;
}

bool MouseHandler::ItemCubeInventoryRightClick(const SLOTIDX from_container_index, 
    const SLOTIDX to_container_index, const POSTYPE from_postype)
{
    DummyItemSlot DummyItem;

    if (IsExistItemAtHand()) // 마우스에 잡힌것이 없을 경우만
    {
        return false;
    }

    if (from_container_index != SI_INVENTORY)
    {
        //	아이템 조합은 인벤토리에 있는 아이템만 가능합니다!
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
            g_InterfaceManager.GetInterfaceString(eST_ITEM_COMPOSITE_RANKUP_IN_INVENTORY));
        return false;
    }
    if (to_container_index != SI_CUBE)
    {
        return false;
    }

    SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index);
    SCSlotContainer* cube_container_ptr = ItemManager::Instance()->GetContainer(to_container_index); 
    if ((inventory_container_ptr == NULL) || 
        (inventory_container_ptr->IsEmpty(from_postype) == TRUE) || 
        (cube_container_ptr == NULL))
    {
        return false;
    }

    SCItemSlot* from_itemslot_ptr = 
        static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_postype)));
    if ((from_itemslot_ptr == NULL) || 
        (from_itemslot_ptr->GetItemInfo() == NULL) || 
        (from_itemslot_ptr->GetSlotType() != ST_ITEM))
    {
        return false;
    }

    if ((from_itemslot_ptr->IsPetItem()) &&
        (from_itemslot_ptr->IsUserBlocked() == true))
    {
        // 5692 소환된 펫은 합성재료로 사용할 수 없습니다. 펫 소환을 해제한후 다시 시도해 주십시오.
        TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(5692, message_buffer,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);

        return false;
    }

    if (from_itemslot_ptr->IsLocked())
    {
        return false;
    }

    SCItemSlot& from_item_slot = (SCItemSlot&)inventory_container_ptr->GetSlot(from_postype);
    // 여기에 들어왔다는것은 특정 컨테이너에서 랭크업 컨테이너로 아이템을 가져온것이다.
    assert(&from_item_slot);
    DummyItem.SetOrgSlot(&from_item_slot);
    DummyItem.SetFromContainerIdx(from_container_index);
    DummyItem.SetFromPosition(from_postype);

    POSTYPE empty_pos = 0;
    while(!cube_container_ptr->IsEmpty(empty_pos))
    {
        ++empty_pos;
        if (empty_pos >= cube_container_ptr->GetMaxSlotNum())
        {
            return false;
        }
    }
    cube_container_ptr->InsertSlot(empty_pos, DummyItem); 
    return true;
}

bool MouseHandler::ItemSocketCompositeTransection(SLOTIDX slot_idx, POSTYPE to_position, RECT* pRect)
{
    SCSlotContainer* to_container = ItemManager::Instance()->GetContainer(slot_idx); 
    if (to_container == NULL)
    {
        return false;
    }

    DummyItemSlot DummyItem;
    TCHAR message[INTERFACE_STRING_LENGTH];

    if(IsExistItemAtHand())
    { 
        if (SI_INVENTORY != m_MouseItem.m_fromSlotIdx)
        {
            //	아이템 조합은 인벤토리에 있는 아이템만 가능합니다!
            g_InterfaceManager.GetInterfaceString(eST_ITEM_COMPOSITE_RANKUP_IN_INVENTORY, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            return false;
        }

        if (m_MouseItem.m_pRenderUnit->m_isDiv)
        {
            // 988	창고에서는 아이템 분할이 불가능 합니다. 인벤토리에서 시도해 주세요
            g_InterfaceManager.GetInterfaceString(988 , message ,INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, message);
            return false;
        }

        SCSlotContainer* from_container = ItemManager::Instance()->GetContainer(m_MouseItem.m_fromSlotIdx);

        // 비워있어야 채울수 있다.
        if (!to_container->IsEmpty(to_position))
        {
            //	슬롯이 비어있지 않습니다!
            g_InterfaceManager.GetInterfaceString(eST_TARGET_SLOT_IS_NOT_EMPTY, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            return false;
        }

        uiSocketComposite* dialog = 
            GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);
        TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
        // 여기에 들어왔다는것은 특정 컨테이너에서 쟈드 합성 컨테이너로 아이템을 가져온것이다.
        switch(m_MouseItem.m_slotType)
        {
        case ST_ITEM:
            {
                assert(m_MouseItem.m_pFromSlot);
                SCItemSlot& from_item_slot = (SCItemSlot&)from_container->GetSlot(
                    m_MouseItem.m_fromPos);

                if (to_position < uiSocketComposite::SOCKET_COMPOSITE_SLOT_RESOURCE_MAX)
                {
                    if (from_item_slot.IsZardItem() == false)
                    {   // 쟈드만 올릴 수 있습니다.
                        return false;
                    }
                    int pre_zard_grade = 0;
                    for (int i = 0; i < uiSocketComposite::SOCKET_COMPOSITE_SLOT_RESOURCE_MAX; ++i)
                    {
                        if (pre_zard_grade != 0)
                        {
                            break;
                        }
                        if (to_container->IsEmpty(i))
                        {
                            if (from_item_slot.GetItemInfo()->zard_grade > kZardGrade4)
                            {   // 6489 상급쟈드 합성은 현재의 기술로 불가능합니다
                                if (dialog && dialog->IsVisibleWindow())
                                {
                                    g_InterfaceManager.GetInterfaceString(6489, message_buffer,INTERFACE_STRING_LENGTH);
                                    dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING4,
                                        message_buffer);
                                }
                                return false;
                            } 
                        }
                        else
                        {
                            DummyItemSlot& dummy_item_slot = (DummyItemSlot&)to_container->GetSlot(i);
                            SCItemSlot* to_item_slot = (SCItemSlot*)dummy_item_slot.GetOrgSlot();
                            if (to_item_slot && to_item_slot->GetItemInfo() != NULL)
                            {
                                pre_zard_grade = to_item_slot->GetItemInfo()->zard_grade; //최초 쟈드 등급을 저장
                            }
                        }
                    }

                    if (pre_zard_grade != 0)
                    {
                        if (pre_zard_grade != from_item_slot.GetItemInfo()->zard_grade)
                        {   // 6488: 쟈드의 등급이 다릅니다. 같은 등급의 쟈드만 합성할 수 있습니다.
                            g_InterfaceManager.GetInterfaceString(6488, message_buffer,INTERFACE_STRING_LENGTH);
                            if (dialog && dialog->IsVisibleWindow())
                            {
                                dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING4,
                                    message_buffer);
                            }  
                            return false; // 이전에 등록된 쟈드와 등급이 다르면 안됨
                        }            
                    }
                }
                else if(to_position == uiSocketComposite::SOCKET_COMPOSITE_SLOT_ADD_RESOURCE)
                {
                    if (from_item_slot.IsZardAdditiveItem() == false)
                    {   // 쟈드 합성 첨가제만 올릴 수 있습니다.
                        return false;
                    }
                }
                else
                {
                    return false;
                }

                DummyItem.SetOrgSlot(m_MouseItem.m_pFromSlot);
                DummyItem.SetFromContainerIdx(m_MouseItem.m_fromSlotIdx);
                DummyItem.SetFromPosition(m_MouseItem.m_fromPos);		
                to_container->InsertSlot(to_position, DummyItem);
                MouseHandler::Instance()->RollbackItemAtHand();
            }
            break;
        default:
            {
                assert( !"잘못된 타입의 슬롯을 이동했습니다." );
                return false;
            }
        }
        if (dialog && dialog->IsVisibleWindow())
        {
            dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING4, "");
        }        
        return true;
    }
    return true;
}

bool MouseHandler::SocketCompositeInventoryRightClick(const SLOTIDX from_container_index, 
                                               const SLOTIDX to_container_index, const POSTYPE from_postype)
{
    DummyItemSlot DummyItem;

    if (IsExistItemAtHand()) // 마우스에 잡힌것이 없을 경우만
    {
        return false;
    }

    if (from_container_index != SI_INVENTORY)
    {
        //	아이템 조합은 인벤토리에 있는 아이템만 가능합니다!
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
            g_InterfaceManager.GetInterfaceString(eST_ITEM_COMPOSITE_RANKUP_IN_INVENTORY));
        return false;
    }
    if (to_container_index != SI_SOCKET_COMPOSITE)
    {
        return false;
    }

    SCSlotContainer* inventory_container_ptr = ItemManager::Instance()->GetContainer(from_container_index);
    SCSlotContainer* socket_composite_container_ptr = ItemManager::Instance()->GetContainer(to_container_index); 
    if ((inventory_container_ptr == NULL) || 
        (inventory_container_ptr->IsEmpty(m_MouseItem.m_fromPos) == TRUE) || 
        (socket_composite_container_ptr == NULL))
    {
        return false;
    }

    SCItemSlot* from_itemslot_ptr = 
        static_cast<SCItemSlot*>(&(inventory_container_ptr->GetSlot(from_postype)));
    if ((from_itemslot_ptr == NULL) || 
        (from_itemslot_ptr->GetItemInfo() == NULL) || 
        (from_itemslot_ptr->GetSlotType() != ST_ITEM))
    {
        return false;
    }

    if (from_itemslot_ptr->IsLocked())
    {
        return false;
    }

    SCItemSlot& from_item_slot = (SCItemSlot&)inventory_container_ptr->GetSlot(from_postype);
    // 여기에 들어왔다는것은 특정 컨테이너에서 쟈드 합성 컨테이너로 아이템을 가져온것이다.
    assert(&from_item_slot);
    DummyItem.SetOrgSlot(&from_item_slot);
    DummyItem.SetFromContainerIdx(from_container_index);
    DummyItem.SetFromPosition(from_postype);

    POSTYPE empty_pos = 0;

    if (inventory_container_ptr->IsEmpty(from_postype))
    {
        return false;
    }

    if (from_item_slot.IsZardItem() == true)
    {
        while(!socket_composite_container_ptr->IsEmpty(empty_pos))
        {
            ++empty_pos;
            if (empty_pos >= uiSocketComposite::SOCKET_COMPOSITE_SLOT_RESOURCE_MAX)
            {
                return false;
            }
        }

        uiSocketComposite* dialog = 
            GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);
        TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
        int pre_zard_grade = 0;
        for (int i = 0; i < uiSocketComposite::SOCKET_COMPOSITE_SLOT_RESOURCE_MAX; ++i)
        {
            if (pre_zard_grade != 0)
            {
                break;
            }
            if (socket_composite_container_ptr->IsEmpty(i))
            {
                if (from_item_slot.GetItemInfo()->zard_grade >= kZardGrade4)
                {   // 상급은 조합 불가
                    // 6489 상급쟈드 합성은 현재의 기술로 불가능합니다
                    g_InterfaceManager.GetInterfaceString(6489, message_buffer,INTERFACE_STRING_LENGTH);
                    if (dialog && dialog->IsVisibleWindow())
                    {
                        dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING4,
                            message_buffer);
                    } 
                    return false;
                }
            }
            else
            {
                DummyItemSlot& dummy_item_slot = (DummyItemSlot&)socket_composite_container_ptr->GetSlot(i);
                SCItemSlot* to_item_slot = (SCItemSlot*)dummy_item_slot.GetOrgSlot();
                if (to_item_slot && to_item_slot->GetItemInfo() != NULL)
                {
                    pre_zard_grade = to_item_slot->GetItemInfo()->zard_grade; //최초 쟈드 등급을 저장
                }
            }
        }

        if (pre_zard_grade != 0)
        {
            if (pre_zard_grade != from_item_slot.GetItemInfo()->zard_grade)
            {   // 6488: 쟈드의 등급이 다릅니다. 같은 등급의 쟈드만 합성할 수 있습니다.
                g_InterfaceManager.GetInterfaceString(6488, message_buffer,INTERFACE_STRING_LENGTH);
                if (dialog && dialog->IsVisibleWindow())
                {
                    dialog->SetMessage(uiSocketComposite::SOCKET_COMPOSITE_INFO_STRING4,
                        message_buffer);
                } 
                return false; // 이전에 등록된 쟈드와 등급이 다르면 안됨
            }            
        }
    }
    else if (from_item_slot.IsZardAdditiveItem() == true)
    {
        empty_pos = uiSocketComposite::SOCKET_COMPOSITE_SLOT_ADD_RESOURCE;
        if (!socket_composite_container_ptr->IsEmpty(empty_pos))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    socket_composite_container_ptr->InsertSlot(empty_pos, DummyItem);
    return true;
}

BOOL MouseHandler::ItemCompositeEnchants(SLOTIDX SlotIdx, POSTYPE AtPos, RECT * pRect)
{
	SCSlotContainer *pToContainer = ItemManager::Instance()->GetContainer(SlotIdx); 
    if (!pToContainer)
    {
        return FALSE;
    }

	ItemETCCompositeDlg *pEnchantDlg = (ItemETCCompositeDlg *)ItemManager::Instance()->GetItemContainerDialog(SlotIdx);
	 
	SCItemSlot ItemSlot;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if( IsExistItemAtHand() )
	{ 
		// target pos 의 슬롯이 비워있어야한다.

		if (!pToContainer->IsEmpty(AtPos))
		{
			// target 아이템이 차있습니다.
			// 재등록해라.
			return FALSE;
		}

		switch (pEnchantDlg->GetCurrentCompositeType())
		{
		case eITEMCOMPOSITE_EXTRACT_SOCKET:
			{
				if (ST_ITEM != this->m_MouseItem.m_pFromSlot->GetSlotType())
				{
					return FALSE;
				}
			}
			break;
		default:
			return FALSE;
			break;
		}

		// 기본 재료 들이 등록되어 있나?
		if ( !ItemManager::Instance()->IsExistCompositeMaterials( 
				pEnchantDlg->GetCurrentCompositeType(), 
				SI_INVENTORY) 
			)
		{
			//	필요한 재료가 부족합니다.
			g_InterfaceManager.GetInterfaceString( eST_ITEM_NOT_EXIST_COMPOSITE_MATERIALS, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}

		pEnchantDlg->SetTargetInfo(m_MouseItem.m_fromSlotIdx, m_MouseItem.m_fromPos);
		ItemManager::Instance()->CopyItem(
				m_MouseItem.m_fromSlotIdx, 
				SI_ITEM_ETC_COMPOSE, 
				m_MouseItem.m_fromPos,
				AtPos );	
	}
	else
	{
		// 다시 들어올필요가 없다.
		return FALSE;
	}

	return TRUE;
}


MouseHandler::MOUSE_STATE MouseHandler::GetMouseState() const
{
	return m_eTradeState;
}

VOID MouseHandler::SetMouseState(MOUSE_STATE eShop)
{
	m_eTradeState = eShop;

	switch (m_eTradeState)
	{
	case BUY:
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Buy);
		}
		break;

	case SELL:
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Sell);
		}
		break;
    case ETHERIA_EXTRACT:
    case ETHERIA_EXTRACT_CASTING:
	case REPAIR:
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair);
		}
		break;

	case WITHDRAW:
	case GUILD_WITHDRAW:
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_WireHouse_Out);
		}
		break;

	case DEPOSIT:
	case GUILD_DEPOSIT:
		{
			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_WireHouse_In );
		}
		break;


	case ETHER_DEVICE_FILL:
    case RIDER_WAX:
    case ETHERIA_COMBINATION:
	case ENCHANT:
	case SOCKET_FILL:
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	case AWAKENING:
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
		}
		break;

	case EXTRASTONE_USE:
		{
			CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
		}
		break;


	case THROW_WASTE_BASKET:
		{
			// TODO : 지울때 나오는 아이콘 모양 추가 요청
			CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Waste_Basket );
		}
		break;

	case SOCKET_REMOVE:
		CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
		break;	

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	case ADD_RANDOM_OPTION:
		CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
		break;
#endif			


	case RANK_DOWN:
		CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Item_Rank_Down);
		break;


	case REPAIR_LIMITED:
		CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair_Limited_Item);
		break;

	case NONE:
	default:
		{
			CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID				
MouseHandler::RemoveMouseState(MOUSE_STATE eShop)
{
	if ( GetMouseState() == eShop )
	{
		SetMouseState( MouseHandler::NONE );

		if ( MouseHandler::THROW_WASTE_BASKET == eShop )
		{
			UnLockWasteDeleteItem();
		}

        if ( MouseHandler::CRYSTALIZATION == eShop )
        {
            InventoryDialog * inventory_dialog_ptr 
                = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
            if (inventory_dialog_ptr)
            {
                inventory_dialog_ptr->RevertCrystalizationItemIconDeActiveRender();
            }
        }
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID MouseHandler::initMouseItem()
{
	m_MouseItem.m_fromSlotIdx		= 0;
	m_MouseItem.m_fromPos			= 0;
	m_MouseItem.m_pRenderUnit		= NULL;
	m_MouseItem.m_pFromSlot			= NULL;
	m_MouseItem.m_pFromBaseItemInfo = NULL;
	SetRectEmpty( &m_MouseItem.m_OldRect);	
}

//----------------------------------------------------------------------------
/**
*/
VOID				
MouseHandler::refreshContainer(SLOTIDX SlotIdx)
{
	SolarDialog * pSolarDialog = 
		(SolarDialog *)ItemManager::Instance()->GetItemContainerDialog(SlotIdx);

	if (!pSolarDialog)
	{
		return;
	}

	switch (SlotIdx)
	{
	case SI_EVENT_INVENTORY:	// temp inventory 는 창고에 리소스 정보를 두고 있다.
	case SI_INVENTORY:
		{
			InventoryDialog * pInvenDialog = (InventoryDialog *)pSolarDialog;
			if ( pInvenDialog )
			{
				pInvenDialog->ReFreshRenderSlot();
			}
		}
		break;

	case SI_EQUIPMENT:
		{
			HeroEquipmentContainer * pEquipContainer = (HeroEquipmentContainer *)pSolarDialog;
			if ( pEquipContainer )
			{
				pEquipContainer->ReFreshRenderSlot();
			}
		}
		break;
	case SI_WAREHOUSE:
		{
			pSolarDialog = 
				(SolarDialog *)ItemManager::Instance()->GetItemContainerDialog(SlotIdx);

			
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
            uiBankDialog* pBankDialog = (uiBankDialog*)pSolarDialog;
            if ( pBankDialog )
            {
                // 2012.09.06 창고에서 특정 아이템을 클릭한 후 그 자리에 다시 클릭 할 때, 아이콘이 사라지는 현상 수정
                // 이유는 아직 아이템을 마우스 핸들러에서 내려놓지 않은 상황에서 ReFreshRenderSlot()가 호출되어 가려짐
                //pBankDialog->ReFreshRenderSlot();
            }
#else
            BankDialog* pBankDialog = (BankDialog*)pSolarDialog;
            if ( pBankDialog )
            {
                pBankDialog->ReFreshRenderSlot();
            }
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
		}
		break;

	default:
		break;
	}

}


BOOL MouseHandler::CheckBlockedItem( SCItemSlot * pSlot)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if(pSlot->IsBlocked())
	{
		RollbackItemAtHand();
		g_InterfaceManager.GetInterfaceString(eST_BLOCKED_ITEM_NOT_MOVE,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		return TRUE;
	}
	return FALSE;
}

BOOL MouseHandler::ItemRepairLimitedTransction(SLOTIDX SlotIdx, POSTYPE TargetPos)
{
	MouseHandler::Instance()->SetTryUseChageItem(TRUE);

	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);

	assert(pAtContainer);
	if (!pAtContainer)
    {
		return FALSE;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	// 비워있느냐?
	if (pAtContainer->IsEmpty(TargetPos))
	{
		//	해당 슬롯이 비어있습니다!
		g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return FALSE;
	}

	// 랭크다운 아이템이 적법한지 검사
	SCItemSlot & rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(TargetPos);

	if ( !rItemSlot.IsLimited() )
	{
		//	해당 아이템에는 사용이 불가능 합니다.
		return FALSE;
	}

	return TRUE;
}


BOOL MouseHandler::_useTransactionFateItemIdentify(SCItemSlot & rItemSlot, const BASE_ITEMINFO * pItemInfo, POSTYPE AtPos)
{
	if( rItemSlot.IsFateItem() && !rItemSlot.GetFateIdentify())
	{
		uiFateItemSystem* pFateItemSystem=  GET_CAST_DIALOG( uiFateItemSystem, IM_FATEITEM_SYSTEM::FATEITEM_DLG );

		if( pFateItemSystem && !pFateItemSystem->IsVisibleWindow() )
		{
			pFateItemSystem->ShowInterface(TRUE);
			pFateItemSystem->SetTargetItem(pItemInfo, AtPos );
			pFateItemSystem->SetTargetItemLock(TRUE);
		}
		return TRUE;
	}
	return FALSE;
}


BOOL MouseHandler::_useTransactionItemPortalQuest(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	if (GameFramework::GetCurrentScene() && 
		(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
		SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
	{					
		//5745	배틀존에서는 사용할 수 없습니다.
		TCHAR szBuf[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(5745, szBuf, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szBuf);
		return TRUE;
	}

	SCSlotContainer * pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
	SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);

	const BASE_ITEMINFO * pItemInfo = rItemSlot.GetItemInfo();
	if ( !pItemInfo )
		return FALSE;


	if ( FALSE == GetTryUseChageItem() )
	{
		SetTryUseChageItem(TRUE);
		SetPendingUseChargeItemPos(AtPos);

		const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(pItemInfo->m_wSkillCode);
		if( pMapInfo )
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
			g_InterfaceManager.GetInterfaceString(6269, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szTemp ,
				g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode));

			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			pSystemMan->Sys_Confirm(szMessage, GlobalFunc::_fn_UsePortalQuestItem);
		}
	}

	return TRUE;
}



BOOL MouseHandler::ItemEtherDeviceFillTransction(SLOTIDX SlotIdx, POSTYPE AtPos)
{
	

	SCSlotContainer* container = ItemManager::Instance()->GetContainer(SlotIdx);
	if (container == NULL)
	{
		return FALSE;
	}

	SCItemSlot& item_slot = static_cast<SCItemSlot &>(container->GetSlot(AtPos));
	if (item_slot.IsContainValidItem() == FALSE)
	{
		return FALSE;
	}

	int string_index = -1;
	
	// 70130:해당 슬롯이 비어있습니다!
	if (container->IsEmpty(AtPos))
	{
		string_index = eST_TARGET_SLOT_IS_EMPTY;
	}
	// 6122:에테르 방출장치는 무기에만 장착가능합니다.
	if (item_slot.GetItemInfo()->IsWeapon() == FALSE)
	{
		string_index = 6122;
	}
	// 6123:에테르 방출장치가 이미 장착되어 있습니다.	
	if (item_slot.IsEquipEtherDevice())
	{
		string_index = 6123;
	}

	// 6122:에테르 방출장치는 무기에만 장착가능합니다.
	if (item_slot.IsEtheriaItem() ||
		item_slot.IsShellItem())
	{
		string_index = 6122;
	}

	if (string_index > 0)
	{
		TCHAR fail_msg[INTERFACE_STRING_LENGTH] ={0,};
		g_InterfaceManager.GetInterfaceString(string_index, fail_msg, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, fail_msg);
		return FALSE;
	}

	uiAssistMan* assist_man = static_cast<uiAssistMan*>(
								g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

	if (assist_man == NULL)
	{
		return FALSE;
	}

	MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
	assist_man->ShowAskEtherDeviceDialog(item_slot.GetCode(), AtPos, GetPendingUseChargeItemPos());

	return TRUE;
}


BOOL MouseHandler::ItemRiderWaxChangeTransction(SLOTIDX SlotIdx, POSTYPE AtPos)
{
    SCSlotContainer *pAtContainer = ItemManager::Instance()->GetContainer(SlotIdx);
    if (!pAtContainer)
    {
        return FALSE;
    }
    SCItemSlot & rItemSlot = (SCItemSlot &)pAtContainer->GetSlot(AtPos);
    SCItemSlot & rItemFromSlot = (SCItemSlot &)pAtContainer->GetSlot(GetPendingUseChargeItemPos());

    BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( rItemSlot.GetCode() );
    if( !pInfo)
        return FALSE;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

    // 비워있느냐?
    if (pAtContainer->IsEmpty(AtPos))
    {
        //	해당 슬롯이 비어있습니다!
        g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
        return FALSE;
    }

    if( !pInfo->IsRiderPartsItem())
    {
        //6449 탈것의 장착 아이템에만 사용 할 수 있습니다.
        g_InterfaceManager.GetInterfaceString(6449,szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return FALSE;
    }

    uiAssistMan * pAssistMan = (uiAssistMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
    if( pAssistMan  && rItemFromSlot.IsContainValidItem())
    {
        pAssistMan->ShowAskRiderWaxGlossDialog(rItemFromSlot.GetCode(), AtPos, GetPendingUseChargeItemPos());
    }

    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    return TRUE;
}


#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
void MouseHandler::ExtendItemDateTransction( const POSTYPE& TargetPosition )
{
    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    RollbackItemAtHand();
    ItemManager::Instance()->SendExtendItemDate( m_UseWasteItemPosition, TargetPosition );
}

#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
void MouseHandler::RandomizeTransction( const POSTYPE& TargetPosition )
{
    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    RollbackItemAtHand();
    ItemManager::Instance()->SendRandomizeItem( m_RandomizeItemPosition, TargetPosition );
}

#endif//_NA_003740_20111122_RANDOM_ITEM



//--------------------------------------------------------------------------------------------------
/** 에테리아 추출이벤트
*/
bool MouseHandler::ItemEtheriaExtractTransction(POSTYPE slot_pos)
{
    CastDialog* cast_dlg = (CastDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);   

  
    if(cast_dlg && cast_dlg->IsVisible() && cast_dlg->GetCastType() == CastDialog::DO_Etheria)
    {
        //이미 하나가 처리 되고 있음으로 다른것을 작업하지 않는다.
        //키가 예약된것 취소시킴
        GlobalFunc::DoEtheriaExtract(0,0);
        return false;
    }

    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);
    RollbackItemAtHand();

    //에테리아 액션후클릭후 마우스커서 추출상태에서 아이템 클릭시
    //클라 -> 서버로 간단한 체크(추출불가아이템식별)후 패킷을 보냄
    POSTYPE use_pos = GetPendingUseChargeItemPos();

    if (ItemManager::Instance()->IsInvalidEtheriaExtract(slot_pos, use_pos) == false)
    {
        return false;
    }

    SCSlotContainer* container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (container == NULL)
    {
        return false;
        
    }
    SCItemSlot& use_slot = static_cast<SCItemSlot&>(container->GetSlot(use_pos));
    if (use_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }
    
    uiAssistMan* assist_man = static_cast<uiAssistMan*>(
                                      g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  
    if (assist_man == NULL)
    {
        return false;
    }

    assist_man->ShowAskItemCustomize(use_slot.GetItemInfo()->m_byWasteType, slot_pos, use_pos);
    return true;
}
//--------------------------------------------------------------------------------------------------
/** 에테리아 조합이벤트
*/
bool MouseHandler::ItemEtheriaCombinationTransction(SLOTIDX slot_index, POSTYPE to_pos)
{
    MouseHandler::Instance()->SetMouseState(MouseHandler::NONE);

    SCSlotContainer* container_ptr = ItemManager::Instance()->GetContainer(slot_index);
   
    if (container_ptr == NULL)
    {
        return false;
    }
    
    SCItemSlot& to_slot = (SCItemSlot &)container_ptr->GetSlot(to_pos);
    SCItemSlot& from_slot = (SCItemSlot &)container_ptr->GetSlot(GetPendingUseChargeItemPos());

    if (GetPendingUseChargeItemPos() == to_pos)
    {
        return false;
    }

    // 빈슬롯인지 체크
    if (container_ptr->IsEmpty(to_pos))
    {
        //	해당 슬롯이 비어있습니다!
        TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
        g_InterfaceManager.GetInterfaceString( eST_TARGET_SLOT_IS_EMPTY, message, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message );
        return false;
    }
    // form슬롯은 반드시 에테리어야한다
    if (from_slot.IsContainValidItem() == FALSE ||
        from_slot.IsEtheriaItem() == false)
    {
        return false;
    }

	SCItemSlot temp_slot(to_slot);
	RC::eITEM_RESULT result = temp_slot.CombineWithEtheria(from_slot);

	if (result != RC_ITEM_SUCCESS)
	{
		ItemManager::Instance()->NoticeReasonItemErrorCode(result);
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------
/** 에테리아 추출 망치뜨기
*/
void MouseHandler::SetEtheriaExtractState()
{
    RollbackItemAtHand();
    SetMouseState(MouseHandler::ETHERIA_EXTRACT);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair);
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_Etheria_Extract;				
    msg.DoSomething = GlobalFunc::DoEtheriaExtract;
    g_KeyQueueManager.PushBack(msg);
}

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
//--------------------------------------------------------------------------------------------------
/** 소비 아이템 사용
다른 아이템을 타겟을 가지는 소비아이템의 사용
*/
void MouseHandler::SetUseExtendItemDateMode( const POSTYPE& ItemPosition )
{
    m_UseWasteItemPosition = ItemPosition;

    RollbackItemAtHand();
    SetMouseState(MouseHandler::EXTEND_ITEMDATE);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair);
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_Extend_ItemDate;				
    msg.DoSomething = GlobalFunc::DoExtendItemDate;
    g_KeyQueueManager.PushBack(msg);
}
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM

void MouseHandler::SetRandomizeItemMode( const POSTYPE& ItemPosition )
{
    m_RandomizeItemPosition = ItemPosition;

    RollbackItemAtHand();
    SetMouseState(MouseHandler::RANDOMIZE_ITEM);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Repair);
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_Randomize_Item;				
    msg.DoSomething = GlobalFunc::DoRandomizeItem;
    g_KeyQueueManager.PushBack(msg);


    SCItemSlotContainer* inventory = (SCItemSlotContainer*)g_pHero->GetSlotContainer( SI_INVENTORY );
    SCItemSlot& source_item = (SCItemSlot&)inventory->GetSlot(ItemPosition);

    TCHAR source_name[MAX_ITEMNAME_LENGTH];
    g_InterfaceManager.GetItemName( source_item.GetItemCode(), source_name, MAX_ITEMNAME_LENGTH );

    // 70581 [%s] 기능을 사용할 아이템을 선택해주세요.
    TCHAR message[INTERFACE_STRING_LENGTH];
    g_InterfaceManager.GetInterfaceString(70581, message, INTERFACE_STRING_LENGTH);



    TCHAR buffer[INTERFACE_STRING_LENGTH];
    wsprintf( buffer, message, source_name );
    GlobalFunc::OutputMsg_SysMsg(buffer);

}
#endif//_NA_003740_20111122_RANDOM_ITEM


//--------------------------------------------------------------------------------------------------
/**  미확인 소켓 확인 망치뜨기
*/
void MouseHandler::SetSocketIdentifyState()
{
    RollbackItemAtHand();
    SetMouseState(MouseHandler::SOCKET_IDENTIFY);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_SocketIdentify;				
    msg.DoSomething = GlobalFunc::DoSocketIdentify;
    g_KeyQueueManager.PushBack(msg);
}
//--------------------------------------------------------------------------------------------------
/**  결정화 망치뜨기
*/
void MouseHandler::SetCrystalizationState()
{
    RollbackItemAtHand();
    SetMouseState(MouseHandler::CRYSTALIZATION);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
    keyMsg msg;
    ZeroMemory(&msg, sizeof(msg));
    msg.dwType = eDoType_Crystalization;
    msg.DoSomething = GlobalFunc::DoCrystalization;
    g_KeyQueueManager.PushBack(msg);
}
//--------------------------------------------------------------------------------------------------
/**  헤르메시즘의 성서
*/
void MouseHandler::SetRecreateSocketState()
{
    RollbackItemAtHand();
    SetMouseState(MouseHandler::SOCKET_RECREATE);
    CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Composite);
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_SocketRecreate;				
    msg.DoSomething = GlobalFunc::DoSocketRecreate;
    g_KeyQueueManager.PushBack(msg);
}

void MouseHandler::SetCharCustomizeDlg(WORD item_type, POSTYPE slot_pos)
{

    uiCharCustomizeDlg* custom_dlg = GET_CAST_DIALOG(uiCharCustomizeDlg,
        IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG);

    if (custom_dlg == NULL)
    {
        return;
    }

    if (custom_dlg->IsVisibleWindow())
    {
        return;
    }

    custom_dlg->GenerateCustomInfo(item_type, slot_pos);
    custom_dlg->ShowInterface(TRUE);
}

void MouseHandler::SetDivideItemOff()
{
	if (m_MouseItem.m_pRenderUnit != NULL)
	{
		m_MouseItem.m_pRenderUnit->m_isDiv = false;
	}
}
