#include "SunClientPrecompiledHeader.h"
#include <SCSlotContainer.h>
#include <ItemTypeList.h>
#include "InterfaceManager.h"
#include "SolarDialog.h"
#include "shopdialog.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "ShopInfoParser.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include <SCItemSlot.h>
#include "chatdialog.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Hero.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "HeroActionInput.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "GraphicResourceList.h"
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

WzID ShopDialog::m_wzId[ShopDialog::Dialog_MAX] = 
{		
	StrToWzID("B000"),		//SHOP_BT_SLOT1 = 0,
	StrToWzID("B001"),		//SHOP_BT_SLOT2,
	StrToWzID("B003"),		//SHOP_BT_SLOT3,
	StrToWzID("B004"),		//SHOP_BT_SLOT4,
	StrToWzID("B005"),		//SHOP_BT_SLOT5,
	StrToWzID("B006"),		//SHOP_BT_SLOT6,
	StrToWzID("B007"),		//SHOP_BT_SLOT7,
	StrToWzID("B008"),		//SHOP_BT_SLOT8,
	StrToWzID("B009"),		//SHOP_BT_SLOT9,
	StrToWzID("B010"),		//SHOP_BT_SLOT10,
	StrToWzID("B011"),		//SHOP_BT_SLOT11,
	StrToWzID("B012"),		//SHOP_BT_SLOT12,
	StrToWzID("B013"),		//SHOP_BT_SLOT13,
	StrToWzID("B014"),		//SHOP_BT_SLOT14,
	StrToWzID("B015"),		//SHOP_BT_SLOT15,
	StrToWzID("B016"),		//SHOP_BT_SLOT16,
	StrToWzID("B017"),		//SHOP_BT_SLOT17,
	StrToWzID("B018"),		//SHOP_BT_SLOT18,
	StrToWzID("B019"),		//SHOP_BT_SLOT19,
	StrToWzID("B020"),		//SHOP_BT_SLOT20,
	StrToWzID("B021"),		//SHOP_BT_SLOT21,
	StrToWzID("B022"),		//SHOP_BT_SLOT22,
	StrToWzID("B023"),		//SHOP_BT_SLOT23,
	StrToWzID("B024"),		//SHOP_BT_SLOT24,
	StrToWzID("B025"),		//SHOP_BT_SLOT25,
	StrToWzID("BT00"),		//SHOP_BT_SLOT_HISTORY1
	StrToWzID("BT01"),		//SHOP_BT_SLOT_HISTORY2
	StrToWzID("BT02"),		//SHOP_BT_SLOT_HISTORY3
	StrToWzID("BT03"),		//SHOP_BT_SLOT_HISTORY4
	StrToWzID("BT04"),		//SHOP_BT_SLOT_HISTORY5
	StrToWzID("T003"),		//SHOP_TEXT_REPAIR_PAYMENT,
	StrToWzID("C005"),		//SHOP_CHECK_BUY,
	StrToWzID("C006"),		//SHOP_CHECK_SELL,
	StrToWzID("C007"),		//SHOP_CHECK_REPAIR,
	StrToWzID("B991"),		//SHOP_BT_REPAIR_ALL,			
	StrToWzID("C000"),		//SHOP_CHECK_BERSERKER,
	StrToWzID("C001"),		//SHOP_CHECK_DRAGON,
	StrToWzID("C002"),		//SHOP_CHECK_VALKYRIE,
	StrToWzID("C003"),		//SHOP_CHECK_WEAPON,
	StrToWzID("C004"),		//SHOP_CHECK_GENERAL,
    StrToWzID("P000"), //SHOP_PIC_COST_ICON,

	StrToWzID("B999")
	
};


ShopDialog::ShopDialog(void)
{
	m_pItemUnitRenderer = NULL;
	m_TabNum = 0;
	m_DialogKey = 0;
}

ShopDialog::~ShopDialog(void)
{
}


VOID ShopDialog::Init( CDrawBase * pDrawBase )
{
	this->GetControlsFromDialog();

	// 초기화 해야할 변수들
	m_bDrawSlotInfo = FALSE;
	m_iDrawSlotPos = 0;

	m_Money		= 0;
	m_TabNum	= 0;

	m_eShopMethod = SHOP_METHOD_NONE;

	m_Money = 0;

	Sprintf( m_szText, _T("%I64"), m_Money );
	GlobalFunc::SetCaption( m_pCtrlStaticRepairMoney, m_szText );

	RECT rcDlg;
	GetDialogWZ()->GetDlgSize(&rcDlg);

	int iDeltaX = (rcDlg.right) / 2;
	int iDeltaY = (rcDlg.bottom) / 2;
	int x, y;
	x = g_InterfaceManager.GetWindowRC()->right / 2 - iDeltaX;
	y = g_InterfaceManager.GetWindowRC()->bottom / 2 - iDeltaY;

	MoveWindowWZ(x, y);	

	SCItemSlotContainer::Init(MAX_VENDOR_SLOT_NUM, SI_NPCSTORE);
	InitContainer();
#ifdef _JBH_MOD_MONEY_OUTPUT
	m_bCkickMoney = FALSE;
#endif

	m_PendingHistorySlotPos = -1;

    cost_icon_contol_	= static_cast<CCtrlPictureWZ*>(this->getControl(SHOP_PIC_COST_ICON));
    currency_item_code_ = 0;
    shop_type_ = 0;
    mouse_over_position_ = INVALID_POSTYPE_VALUE;
}

VOID ShopDialog::InitContainer()
{
	InitRenderer();	
}

VOID ShopDialog::ReleaseContainer()
{
    // 2012.02.09 / 송찬종 / 슬롯을 지우기 전에 마우스로 들고 있는 아이템을 놓지 않아 크래쉬 난 현상 수정
    if (MouseHandler::Instance()->IsExistItemAtHand())
    {
        MouseHandler::Instance()->RollbackItemAtHand();
    }

	FlushUnitDraw();

	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

VOID ShopDialog::InitRenderer()
{
	assert( !m_pItemUnitRenderer );

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );
}


VOID ShopDialog::Release()
{
	ReleaseContainer();
}

VOID ShopDialog::FlushUnitDraw()
{
	SCItemSlot FromSlot;

	if (!GetMaxSlotNum())
	{
		return;
	}

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if (!IsEmpty(i))
		{
			DeleteSlot(i, NULL);
		}
	}
}

ItemUnitRenderer * ShopDialog::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}

VOID ShopDialog::MessageProc( SI_MESSAGE * pMessage )
{
	assert(pMessage);

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szResult[INTERFACE_STRING_LENGTH];

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos >= SHOP_BT_SLOT_HISTORY1 && pos <= SHOP_BT_SLOT_HISTORY5)
			{
				if( ! IsEmpty(GetRealSlotPos(pos)) &&  m_eShopMethod == SHOP_METHOD_BUY)
				{
					NET_SEND_CG_ITEM_REPURCHASE_SYN(pos);
				}
				return;
			}

			if( pos < SHOP_MAX_SLOT ) 
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				RECT rcSlot = pControl->GetSizeRect();		

				MouseHandler::Instance()->ItemTransaction(SI_NPCSTORE, GetRealSlotPos(pos), &rcSlot);
			}
			else
			{
				if (MouseHandler::Instance()->IsExistItemAtHand())
				{
					MouseHandler::Instance()->RollbackItemAtHand();
				}

				switch (pos)
				{
				case SHOP_BT_EXIT:
					{
						g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_SHOP, FALSE);
					}
					break;

				case SHOP_CHECK_BUY:
					{
						MouseHandler::Instance()->SetMouseState(MouseHandler::BUY );
						SetCheckTrade(SHOP_METHOD_BUY );
						//	구입할 아이템을 선택하세요.
						g_InterfaceManager.GetInterfaceString( eST_SELECT_BUY_ITEM, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
					break;

				case SHOP_CHECK_SELL:
					{
						MouseHandler::Instance()->SetMouseState(MouseHandler::SELL );
						SetCheckTrade(SHOP_METHOD_SELL );
						//	판매할 아이템을 선택하세요.
						g_InterfaceManager.GetInterfaceString( eST_SELECT_SELL_ITEM, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
					break;

				case SHOP_CHECK_REPAIR:
					{
						MouseHandler::Instance()->SetMouseState(MouseHandler::REPAIR );
						SetCheckTrade(SHOP_METHOD_REPAIR);
						//	수리할 아이템을 선택하세요(아직 구현되지 않음)
						g_InterfaceManager.GetInterfaceString( eST_SELECT_REPAIR_ITEM, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
					break;
					
				case SHOP_BT_REPAIR_ALL:
					{
						//	모든 아이템을 수리한다.(아직 구현되지 않음)
						g_InterfaceManager.GetInterfaceString( eST_REPAIR_ALL_ITEM, szMessage, INTERFACE_STRING_LENGTH );
						MouseHandler::Instance()->SetPendingRepairSlotIdx(SI_EQUIPMENT);
						MouseHandler::Instance()->SetPendingRepairPos(0);						
						MouseHandler::Instance()->SetPendingRepairAll(TRUE);
						TCHAR szTemp[INTERFACE_STRING_LENGTH];
						// %s#%I64u 하임이 소요 됩니다. 5129
						g_InterfaceManager.GetInterfaceString(5129, szTemp, INTERFACE_STRING_LENGTH);
                        TCHAR repair_money_string[INTERFACE_STRING_LENGTH] = "";
                        GlobalFunc::SetPointText(repair_money_string, 
                            ItemManager::Instance()->GetPriceRepairItemAll(SI_EQUIPMENT));
                        // [%s] 를 수리 하시겠습니까?%s 하임이 소요됩니다. 5131
                        Sprintf(szResult, szTemp, szMessage, repair_money_string);
						if(ItemManager::Instance()->IsExistLimitedItem( SI_EQUIPMENT ))
						{
							ZeroMemory(szTemp, INTERFACE_STRING_LENGTH);
							//	6056	# 리미티드아이템은 전체 수리 시 일괄적으로 최대내구도가 감소하게 됩니다.
							g_InterfaceManager.GetInterfaceString(6056, szTemp, INTERFACE_STRING_LENGTH);
							strcat(szResult, szTemp);
						}

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szResult, GlobalFunc::RepairConfirm);

					}
					break;

				case SHOP_CHECK_BERSERKER:
					m_iClickedIndexTab = 0;
					ClickTabCheckButton(m_iClickedIndexTab );
					ReFreshRenderSlot();
					if (MouseHandler::Instance()->IsExistItemAtHand())
						MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->SetShow(TRUE);


					break;
				case SHOP_CHECK_DRAGON:
					m_iClickedIndexTab = 1;
					ClickTabCheckButton(m_iClickedIndexTab );
					ReFreshRenderSlot();
					if (MouseHandler::Instance()->IsExistItemAtHand())
						MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->SetShow(TRUE);
					break;
				case SHOP_CHECK_VALKYRIE:
					m_iClickedIndexTab = 2;
					ClickTabCheckButton(m_iClickedIndexTab );
					ReFreshRenderSlot();
					if (MouseHandler::Instance()->IsExistItemAtHand())
						MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->SetShow(TRUE);
					break;
				case SHOP_CHECK_WEAPON:
					m_iClickedIndexTab = 3;
					ClickTabCheckButton(m_iClickedIndexTab );
					ReFreshRenderSlot();
					if (MouseHandler::Instance()->IsExistItemAtHand())
						MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->SetShow(TRUE);
					break;
				case SHOP_CHECK_GENERAL:
					m_iClickedIndexTab = 4;
					ClickTabCheckButton(m_iClickedIndexTab );
					ReFreshRenderSlot();
					if (MouseHandler::Instance()->IsExistItemAtHand())
						MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->SetShow(TRUE);
					break;
#ifdef _JBH_MOD_MONEY_OUTPUT
				case SHOP_TEXT_REPAIR_PAYMENT:
					{
						m_bCkickMoney = TRUE;
					}
					break;
#endif

				default:
					break;
				}
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID	
ShopDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			POSTYPE RelativePos = GetRealSlotPos(pos);

			if (pos >= 0 && pos < SHOP_MAX_SLOT + MAX_HISTORY_SLOT)
			{
				if (!IsEmpty( RelativePos ))
				{
					SCSlot & rSlot = GetSlot( RelativePos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
                    mouse_over_position_ = pos;
				}
				else
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
			else
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}			
		}
		break;
	}	
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ShopDialog::NetworkProc( MSG_BASE * pMsg )
{

}

CControlWZ * ShopDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < Dialog_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE	ShopDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_MAX;
}

RC::eSLOT_INSERT_RESULT		ShopDialog::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	// HERO이기 때문에 Render

	// 시리얼은 설정되서 온다.

	/// 인벤토리
	CControlWZ * pCtrl = getControl( GetRelativeSlotPos(AtPos) );
	RECT rect = pCtrl->GetSizeRect();

	SCSlot & rRealSlot = GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	ReFreshRenderSlot();

	return eInsertSlotResult;
}

BOOL ShopDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->RemoveItemUnit( GetSlot(AtPos).GetSerial() );

	SCSlotContainer::DeleteSlot( AtPos, pSlotOut );

	ReFreshRenderSlot();

	return TRUE;
}


VOID ShopDialog::OnRenderSolarDialog()
{
	assert(m_pItemUnitRenderer);

    if (m_pItemUnitRenderer)
    {
	    m_pItemUnitRenderer->Render();
    }


	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			if( pUIMan )
			{
				if( m_MouseOverObject.pSlot->GetPos() >= HISTORY_SLOT1 )
				{
                    pUIMan->RegisterItemslotInfoTooltipForShop(m_MouseOverObject.pSlot, kLimitStatCompositeItemPurchased);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
				else
				{
                    BYTE shop_type_value = shop_type();
                    pUIMan->RegisterItemslotInfoTooltipForShop(m_MouseOverObject.pSlot, shop_type_value);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
			}


			if( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
			{
				uiToolTipMan* pUIMan2 = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP2 );
				if( pUIMan2 )
				{
					pUIMan2->RegisterHeroItemInfoTooltipFromSlot( m_MouseOverObject.pSlot );
				}
			}

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = SHOP_BT_SLOT1; a < SHOP_BT_SLOT25+1 +MAX_HISTORY_SLOT; a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
    updateToolTip();
}

VOID ShopDialog::OnUpdateSolarDialog()
{
	assert(m_pItemUnitRenderer);

	m_pItemUnitRenderer->Update();

	updateMoney();

#ifdef _JBH_MOD_MONEY_OUTPUT
#if defined(__KR) || defined(_KOREA)
		updateToolTip();
#	endif // #if defined(__KR) || defined(_KOREA)
#endif // _JBH_MOD_MONEY_OUTPUT
}

BOOL ShopDialog::GetControlsFromDialog()
{
		m_pCtrlStaticRepairMoney				= (CCtrlStaticWZ *)getControl(SHOP_TEXT_REPAIR_PAYMENT);
		m_pCtrlCheckShopButtons[SHOP_METHOD_BUY]	= (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_BUY);
		m_pCtrlCheckShopButtons[SHOP_METHOD_SELL]	= (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_SELL);
		m_pCtrlCheckShopButtons[SHOP_METHOD_REPAIR] = (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_REPAIR);
		m_pCtrlCheckTabShopButtons[Shop_Tab_1]= (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_BERSERKER);
		m_pCtrlCheckTabShopButtons[Shop_Tab_2]	 = (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_DRAGON);
		m_pCtrlCheckTabShopButtons[Shop_Tab_3] = (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_VALKYRIE);
		m_pCtrlCheckTabShopButtons[Shop_Tab_4]		 = (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_WEAPON);
		m_pCtrlCheckTabShopButtons[Shop_Tab_5]		 = (CCtrlButtonCheckWZ *)getControl(SHOP_CHECK_GENERAL);

		return TRUE;
}

VOID	ShopDialog::SetCheckTrade(SHOP_METHOD eShopMethod )
{
	m_eShopMethod = eShopMethod;

	for (int i = 0; i < SHOP_METHOD_MAX; ++i)
	{
		if ( i == m_eShopMethod )
		{
			m_pCtrlCheckShopButtons[eShopMethod]->SetCheckState( TRUE );
		}
		else
		{
			m_pCtrlCheckShopButtons[i]->SetCheckState( FALSE );	
		}
	}
}


VOID ShopDialog::ClickTabCheckButton(int iIndex )
{
	if (iIndex >= Shop_Type_MAX)
	{
		return;
	}

	m_iClickedIndexTab = iIndex;
	m_TabNum = m_iClickedIndexTab * SHOP_MAX_SLOT; 

	for (int i = 0; i < Shop_Type_MAX; ++i)
	{
		m_pCtrlCheckTabShopButtons[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckTabShopButtons[iIndex]->SetCheckState(TRUE);
}


VOID ShopDialog::OnShowWindow(BOOL val)
{
	SolarDialog::OnShowWindow(val);

	if (val)
	{
		if ( !m_pItemUnitRenderer )
		{
			InitContainer();
		}
		RegisteAllHistoryItemSlot();

		// 창고창이 열린상태라면 
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
        uiBankDialog *pBankDlg = GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
        BankDialog *pBankDlg = GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
		if (pBankDlg && pBankDlg->IsVisibleWindow())
		{
			pBankDlg->SendWareHouseEnd();
		}

		SettingShop();

		UpdateData(TRUE);

		this->m_eShopMethod = SHOP_METHOD_NONE;
		this->SetCheckTrade(SHOP_METHOD_NONE);

		ReFreshRenderSlot();

		ClickTabCheckButton(m_iClickedIndexTab );

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

        g_InterfaceManager.AttachFirstDialog(InterfaceManager::DIALOG_SHOP);
        g_InterfaceManager.DeAttachFirstDialog(InterfaceManager::DIALOG_SHOP);
		UpdateData(FALSE);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_shop;
		msg.wParam=InterfaceManager::DIALOG_SHOP;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		
	}
	else
	{
		ReleaseContainer();

		m_iClickedIndexTab = 0;
		m_TabNum = m_iClickedIndexTab * SHOP_MAX_SLOT; 

		this->m_eShopMethod = SHOP_METHOD_NONE;

		MouseHandler::Instance()->RemoveMouseState(MouseHandler::BUY);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::SELL);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::REPAIR);
		
		if (g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_EQUIP))
			g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_shop);

		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}	
}

BOOL ShopDialog::UpdateData(BOOL bTake)
{
	BOOL bRet = GetControlsFromDialog();

	if ( !bRet )
	{
		return FALSE;
	}

	return TRUE;
}

VOID	ShopDialog::updateMoney()
{
	if (g_pHero)
	{
		if(m_pCtrlStaticRepairMoney)
		{
			TCHAR szMoney[INTERFACE_STRING_LENGTH + 1] = {0,};
			TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
            TCHAR money_type[INTERFACE_STRING_LENGTH + 1] = {0,};
            TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0,};
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

			// 돈을 셋팅한다.
            if (shop_type_ == SHOP_TYPE_OF_ITEM)
            {
                int item_count = ItemManager::Instance()->GetOverlapItemCount(currency_item_code_);
                Sprintf(szMoney, _T("%d"), item_count);
                g_InterfaceManager.GetInterfaceString(47642, money_type); //47642 %s 개
            }
            else if (shop_type_ == SHOP_TYPE_OF_ACCUMULATE)
            {
                int gain_point = 0;
                uiCharacterStatusMan* character_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
                if (character_status_manager != NULL)
                {
                    gain_point = character_status_manager->accumulate_point();
                }
                Sprintf(szMoney, _T("%d"), gain_point);
                g_InterfaceManager.GetInterfaceString(47643, money_type); //47643 %s 포인트
            }
            else if (shop_type_ == SHOP_TYPE_OF_CURRENCY)
            {
    #ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
                int gain_point = 0;
                
                uiCharacterStatusCurrencyInfo* ui_currency_info = 
                    GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
                if (ui_currency_info != NULL)
                {
                    // 화폐 코드에 해당되는 포인트정보를 가져옴
                    DWORD currency_code = currency_item_code();
                    uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point_info = 
                        ui_currency_info->GetCurrencyPointInfo(currency_code);
                    if (currency_point_info != NULL)
                    {
                        // 화폐포인트 세팅
                        gain_point = currency_point_info->currency_point;
                    }
                }
                Sprintf(szMoney, _T("%d"), gain_point);
                g_InterfaceManager.GetInterfaceString(47642, money_type); //47642 %s 개
    #endif //_NA004034_20120102_POINT_WALLET_SYSTEM
            }
            else
            {
			    Sprintf(szMoney, _T("%I64u"), g_pHero->GetMoney());
                g_InterfaceManager.GetInterfaceString(47641, money_type); //47641 %s 하임
            }

			::GetNumberFormat(
				LOCALE_SYSTEM_DEFAULT, 
				NULL, 
				szMoney, 
				&nFmt, 
				szFinal, 
				sizeof(szFinal));

            Sprintf(temp_string, money_type, szFinal);
            
#ifdef _JBH_MOD_MONEY_OUTPUT
			DWORD dwColor = GlobalFunc::GetMoneyColor( g_pHero->GetMoney() );
			m_pCtrlStaticRepairMoney->SetTextColor( dwColor );
#else
			m_pCtrlStaticRepairMoney->SetTextColor(RGBA(255,255,255,255));
#endif
			GlobalFunc::SetCaption( m_pCtrlStaticRepairMoney, temp_string );
		}
	}
}

// 실제 슬롯정보관련한 위치
POSTYPE	ShopDialog::GetRealSlotPos( POSTYPE pos )
{
	if( pos >= SHOP_BT_SLOT_HISTORY1 )
	{
		return pos + 100;
	}
	else
	{
		return pos + m_TabNum;
	}
}

// 인터페이스 컨트롤 위치.
POSTYPE ShopDialog::GetRelativeSlotPos( POSTYPE pos )
{
	if( pos >= HISTORY_SLOT1 )
	{
		return pos - 100;
	}
		
	else
	{
		return pos % SHOP_MAX_SLOT;
	}
}

BOOL ShopDialog::AddItem(eSHOP_TAB eShopTabIndex, POSTYPE AtPos, SHOPINFO::SHOP_ITEM * pShopItem)
{
	if ( 0 == pShopItem->m_SellItemCode )
		return FALSE;

	int TabNum = eShopTabIndex * SHOP_MAX_SLOT;

	BASE_ITEMINFO * pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo( pShopItem->m_SellItemCode );

	if (!pBaseItemInfo)
	{
		assert(!"잘못된 아이템이다.!");
		return FALSE;
	}

	static DWORD SERIAL = 1000000;
    SCItemSlot ItemSlot;
    ItemSlot.SetSlotType(ST_ITEM);
    nsSlot::ItemTypeChanger::MakeItemSlotByType(&ItemSlot, 
                                                 pShopItem->m_SellItemCode,
                                                 pShopItem->m_SellItemNum, 
                                                 pShopItem->item_type_);
    ItemSlot.SetNum(pShopItem->m_SellItemNum);
	ItemSlot.SetSerial(SERIAL++);
#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
    ItemSlot.SetDateTime(pShopItem->timelimit_ * 60);
#endif //_NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
	InsertSlot( TabNum + AtPos, ItemSlot);
	return TRUE;
}

BOOL ShopDialog::SettingShop()
{
	m_pShopListInfo =  ShopInfoParser::Instance()->GetShopList( GetShopID() );

	if (!m_pShopListInfo)
	{
		return FALSE;
	}
    currency_item_code_ = m_pShopListInfo->GetCurrencyItemCode();
    shop_type_ = m_pShopListInfo->GetShopType();
    SetCostIconFromType();
	int iCnt = 0;

	int iTab1Cnt = 0;
	int iTab2Cnt = 0;
	int iTab3Cnt = 0;
	int iTab4Cnt = 0;
	int iTab5Cnt = 0;

	m_pShopListInfo->SetFirstItem();

	SHOPINFO::SHOP_ITEM	* pShopItem = NULL;

	for ( int i = 0; i < SHOPINFO::_MAX_TAB_NUM; ++i)
	{
		for ( int j = 0; j < SHOPINFO::_MAX_SELLITEM_NUM; ++j )
		{
			SHOPINFO::SHOP_ITEM * pShopItem = m_pShopListInfo->GetItemForIndex(i, j);

			if (iTab1Cnt < SHOP_MAX_SLOT)
			{
				BOOL bRet = AddItem((eSHOP_TAB)i, j, pShopItem);
                prices_[i][j] = pShopItem->item_cost_;
			}
            else
            {
                prices_[i][j] = 0;
            }
		}
	}

	ReFreshRenderSlot();

	return TRUE;
}


VOID ShopDialog::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if( (i >= m_TabNum && i < m_TabNum+SHOP_MAX_SLOT) || (i >= HISTORY_SLOT1 && i<= HISTORY_SLOT5) )
		{
			// 보여줘야할 목록

			if ( !IsEmpty(i) )
			{	
				if(!m_pItemUnitRenderer)
					continue;

				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender )
				{
					pCtrl = getControl( GetRelativeSlotPos(i) );
					rcSlot = pCtrl->GetSizeRect();

					pRender = m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
				}

				if (pRender)
                {
				    pRender->SetShow(TRUE);
                }
				else
				{
					assert(!"ShopDialog - RenderUnit is Null Pointer");
				}

			}
		}
		else
		{
			// 보여지지 말아야 할 목록

			if ( !IsEmpty(i) )
			{
				if(!m_pItemUnitRenderer)
					continue;

				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender )
				{
					pCtrl = getControl( GetRelativeSlotPos(i) );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
				}
				if (pRender)
					pRender->SetShow(FALSE);
			}
		}
	}
}


VOID ShopDialog::SetShopID(DWORD dwID)
{
	m_dwShopID = dwID;
}

DWORD ShopDialog::GetShopID() const
{
	return m_dwShopID;
}

#ifdef _JBH_MOD_MONEY_OUTPUT
void ShopDialog::updateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;

	// 효율을 위하여 SHOP_TEXT_REPAIR_PAYMENT 컨트롤만 검사함.
	CControlWZ * pControl = getControl(SHOP_TEXT_REPAIR_PAYMENT);

	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{

				if( m_dwPendingTick == 0)
				{
					m_dwPendingTick = clock_function::GetTickCount();
				}
				bMouseOver = TRUE;
			}
			else
			{
				m_dwPendingTick = 0;
				m_bCkickMoney = FALSE;
			}
		}
		else
		{
			m_dwPendingTick = 0;
			m_bCkickMoney = FALSE;
		}
	}

	DWORD dwTick = 0;
	if( m_dwPendingTick > 0 )
	{
		dwTick = clock_function::GetTickCount() - m_dwPendingTick;
	}

	if ( ( bMouseOver && dwTick > 2000 ) || m_bCkickMoney )
	{
		if (g_pHero && (g_pHero->GetMoney()  > 0))
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			memset(szMessage , 0 , sizeof( szMessage ) );
			GlobalFunc::ChangeMoneyToStringMoney( g_pHero->GetMoney()  , szMessage );
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if( pUIMan )
			{
				pUIMan->InitTooltip();
				pUIMan->RegisterTooltip(szMessage ,_tcslen(szMessage) , 25 );
				pUIMan->SetTooltipDialogID( GetDialogKey() );
			}
		}
	}
}
#endif

RC::eSLOT_INSERT_RESULT ShopDialog::PushHistorySlot(SCSlot & rSlot)
{
	int iCount = 0;
	//슬롯에 있는건 다 지우고, m_ItemHistorySlot에 정보를 채워 넣는다
	for( int i =0; i < MAX_HISTORY_SLOT; ++i)
	{
		SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(HISTORY_SLOT1 + i);
		if( rItemSlot.GetCode() )
		{
			iCount++;
		}
		DeleteSlot(HISTORY_SLOT1 + i,NULL);
	}

	//채워진 정보를 슬롯에 넣는다.
	int insertCount =0;
	for( int i =0; i < MAX_HISTORY_SLOT; ++i )
	{
		// not insert latest Item
		if( iCount == 5 && i ==0 )
			continue;

		if( m_ItemHistorySlot[i].GetCode() )
		{
			InsertSlot(HISTORY_SLOT1 + insertCount, m_ItemHistorySlot[i]);
            m_ItemHistorySlot[insertCount].Copy(m_ItemHistorySlot[i]);
			//memcpy(&m_ItemHistorySlot[insertCount], &m_ItemHistorySlot[i], sizeof(SCItemSlot));
			insertCount++;
		}
	}

	
	//배열의 범위를 넘어섰을경우 방지
	if (MAX_HISTORY_SLOT <= insertCount)
	{
		return RC_INSERT_SLOT_FAILED;
	}

	// 
	static DWORD dwHistorySlotIndex = 2000000;
	rSlot.SetSerial(dwHistorySlotIndex++);
	InsertSlot(HISTORY_SLOT1 + insertCount, rSlot);
    SCItemSlot& item_slot = (SCItemSlot&)rSlot;
    m_ItemHistorySlot[insertCount].Copy(item_slot);
	//memcpy(&m_ItemHistorySlot[insertCount], &rSlot, sizeof(SCItemSlot));
	return RC_INSERT_SLOT_SUCCESS;
}
BOOL ShopDialog::PopHistorySlot()
{
	// pop할 아이템을 빼고 모은다.
	int insertCount = 0;
	for( int i =0; i < MAX_HISTORY_SLOT; ++i)
	{
		if( m_PendingHistorySlotPos == HISTORY_SLOT1 + i)
		{
			DeleteSlot(HISTORY_SLOT1 +i , NULL);
			continue;
		}

		SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(HISTORY_SLOT1 + i);
		if( rItemSlot.GetCode())
		{
            m_ItemHistorySlot[insertCount].Copy(rItemSlot);
			//memcpy(&m_ItemHistorySlot[insertCount], &rItemSlot, sizeof(SCItemSlot));
			insertCount++;
		}
		DeleteSlot(HISTORY_SLOT1 +i , NULL);
	}

	// insertCount 뒤의 아이템은 clear
	for( int i = insertCount; i < MAX_HISTORY_SLOT; ++i)
	{
		m_ItemHistorySlot[i].Clear();
	}

	//새롭게 만들어진 m_ItemHistorySlot을 슬롯에 넣어준다.
	insertCount = 0;
	for( int i =0; i < MAX_HISTORY_SLOT; ++i)
	{
		if( m_ItemHistorySlot[i].GetCode() )
		{
			InsertSlot(HISTORY_SLOT1 + insertCount, m_ItemHistorySlot[i]);
			insertCount++;
		}
	}
	
	return TRUE;
}	
void ShopDialog::RegisteAllHistoryItemSlot()
{
	for( int i =0; i< MAX_HISTORY_SLOT; ++i )
	{
		DeleteSlot(HISTORY_SLOT1 +i , NULL);
	}

	int insertCount =0;
	for( int i =0; i <MAX_HISTORY_SLOT; ++i)
	{
		if( m_ItemHistorySlot[i].GetCode() )
		{
			InsertSlot(HISTORY_SLOT1 +insertCount, m_ItemHistorySlot[i]);
			insertCount++;
		}
	}
}
void ShopDialog::NET_SEND_CG_ITEM_REPURCHASE_SYN(POSTYPE pos)
{
	if (m_PendingHistorySlotPos == ((BYTE)-1))
	{
		m_PendingHistorySlotPos = GetRealSlotPos(pos);
		MSG_CG_ITEM_REPURCHASE_SYN SendPacket;
		SendPacket.m_NPCCode = g_HeroInput.GetClickedNpcCode();
		SendPacket.m_RepurchasePos = pos - SHOP_BT_SLOT_HISTORY1;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	}	
}
void ShopDialog::ClearAllHistorySlot()
{
	for( int i =0; i< MAX_HISTORY_SLOT; ++i)
	{
		m_ItemHistorySlot[i].Clear();
	}
}
void ShopDialog::updateToolTip()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    bool bMouseOver = false;
    for (POSTYPE a = 0; a < Dialog_MAX; a++)
    {
        CControlWZ * pControl = getControl(a);

        if(pControl)
        {
            RECT rc = pControl->GetSizeRect();

            if (rc.left <= iMouseX && rc.right >= iMouseX)
            {
                if (rc.top <= iMouseY && rc.bottom >= iMouseY)
                {
                    bMouseOver = true;
                    break;
                }
            }
        }
    }

    if (bMouseOver)
    {
        CControlWZ* control = getControl(SHOP_PIC_COST_ICON);
        uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
        if (control)
        {
            RECT control_rect = control->GetSizeRect();
            DWORD tooltip_string_code = 0;

            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX && 
                control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                switch (shop_type())
                {
                case SHOP_TYPE_OF_NARMAL://일반
                    tooltip_string_code = 999; //999 하임
                    break;
                case SHOP_TYPE_OF_ITEM://아이템
                    {
                        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(currency_item_code());
                        if (item_info)
                        {
                            TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
                            g_InterfaceManager.GetItemName(item_info->m_Code, item_name, INTERFACE_STRING_LENGTH);
                            if (tooltip_manager)
                            {
                                tooltip_manager->RegisterTooltipFormat(item_name, 0, 0);
								tooltip_manager->SetTooltipDialogID(GetDialogKey());
							}
                        }
                    }
                    break;
                case SHOP_TYPE_OF_ACCUMULATE://접속 포인트
                    tooltip_string_code = 47644; //47644 접속포인트
                    break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
                case SHOP_TYPE_OF_CURRENCY://화폐 포인트
                    {
                        uiCharacterStatusCurrencyInfo* ui_currency_info = 
                            GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
                        if (ui_currency_info != NULL)
                        {
                            uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point_info = 
                                ui_currency_info->GetCurrencyPointInfo(currency_item_code());
                            if (currency_point_info != NULL)
                            {
                                tooltip_string_code = currency_point_info->currency_info->currency_name_code;
                            }
                        }
                    }
                    break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
                }
            }

            if (tooltip_string_code > 0 && tooltip_manager) 
            {
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
                tooltip_manager->RegisterTooltipWithStringCode(tooltip_string_code);
            }
        }
    }    
}

void ShopDialog::SetCostIconFromType()
{
    if (cost_icon_contol_)
    {
        DWORD texture_number = 0;
        switch (shop_type())
        {
        case SHOP_TYPE_OF_NARMAL://일반
            texture_number = 33501;
            break;
        case SHOP_TYPE_OF_ITEM://아이템
            {
                BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(currency_item_code());
                if (item_info)
                {
                    texture_number = item_info->m_wVICode;
                }
            }
            break;
        case SHOP_TYPE_OF_ACCUMULATE://접속 포인트
            texture_number = 32950;
            break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        case SHOP_TYPE_OF_CURRENCY://화폐 포인트
            {
                uiCharacterStatusCurrencyInfo* ui_currency_info = 
                    GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
                if (ui_currency_info != NULL)
                {
                    uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point_info = 
                        ui_currency_info->GetCurrencyPointInfo(currency_item_code());
                    if (currency_point_info != NULL)
                    {
                        texture_number = currency_point_info->currency_info->currency_icon_code;
                    }
                }
            }
            break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
        }
        SetControlImage(cost_icon_contol_->GetCtrlID(), texture_number, 0, 0, 32, 32);
    }
}
