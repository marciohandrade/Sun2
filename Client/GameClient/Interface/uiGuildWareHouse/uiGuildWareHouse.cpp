#include "SunClientPrecompiledHeader.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "uiGuildWareHouse/uiGuildWareHouse.h"
#include "InterfaceManager.h"
#include "ItemUnitRenderer.h"
#include "InputNumBankDialog.h"
#include "GlobalFunc.h"
#include "SlotKeyGenerator.h"
#include "MouseHandler.h"
#include "uiGuildWareHouseLog/uiGuildWareHouseLog.h"
#include "uiGuildWareHouseRight/uiGuildWareHouseRight.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "KeyQueueManager.h"
#include "GlobalFunc.h"
#include <SCItemSlot.h>

WzID uiGuildWareHouse::m_wzId[DIALOG_MAX] = {
	StrToWzID("B106"), //DIALOG_SLOT00
	StrToWzID("B107"), //DIALOG_SLOT01
	StrToWzID("B108"), //DIALOG_SLOT02
	StrToWzID("B109"), //DIALOG_SLOT03
	StrToWzID("B110"), //DIALOG_SLOT04
	StrToWzID("B111"), //DIALOG_SLOT05

	StrToWzID("B112"), //DIALOG_SLOT10
	StrToWzID("B113"), //DIALOG_SLOT11
	StrToWzID("B114"), //DIALOG_SLOT12
	StrToWzID("B115"), //DIALOG_SLOT13
	StrToWzID("B116"), //DIALOG_SLOT14
	StrToWzID("B117"), //DIALOG_SLOT15

	StrToWzID("B118"), //DIALOG_SLOT20
	StrToWzID("B119"), //DIALOG_SLOT21
	StrToWzID("B120"), //DIALOG_SLOT22
	StrToWzID("B121"), //DIALOG_SLOT23
	StrToWzID("B122"), //DIALOG_SLOT24
	StrToWzID("B123"), //DIALOG_SLOT25

	StrToWzID("B124"), //DIALOG_SLOT30
	StrToWzID("B125"), //DIALOG_SLOT31
	StrToWzID("B126"), //DIALOG_SLOT32
	StrToWzID("B127"), //DIALOG_SLOT33
	StrToWzID("B128"), //DIALOG_SLOT34
	StrToWzID("B129"), //DIALOG_SLOT35

	StrToWzID("T101"), //DIALOG_TXT_GUILD_NAME,
	StrToWzID("T102"), //DIALOG_TXT_MONEY,

	StrToWzID("B100"), //DIALOG_EXIT_BTN,
	//StrToWzID("B101"), //DIALOG_RIGHT_BTN,
	StrToWzID("B105"), //DIALOG_MONEY_BTN,
	StrToWzID("B104"), //DIALOG_LOG_BTN,

	StrToWzID("CT00"), //DIALOG_SLOT_CHECK_BTN1,
	StrToWzID("CT05"), //DIALOG_SLOT_CHECK_BTN2,
	StrToWzID("CT06"), //DIALOG_SLOT_CHECK_BTN3,

	StrToWzID("CT01"), //DIALOG_OUT_CHECK_BTN,
	StrToWzID("CT02"), //DIALOG_IN_CHECK_BTN,

	StrToWzID("P033"), //DIALOG_LOCK_PICTURE_P033,
	StrToWzID("P032"), //DIALOG_LOCK_PICTURE_P032,
	StrToWzID("P034"), //DIALOG_LOCK_PICTURE_P034,
};

//////////////////////////////////////////////////////////////////////////

uiGuildWareHouse::uiGuildWareHouse(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow					= FALSE;
	m_pItemUnitRenderer		= NULL;
	m_Money					= 0;
	m_btCurTab				= 0;
	m_btMaxTab				= 0;

	m_pCtrlGuildNameText = NULL;
	m_pCtrlMoneyText	 = NULL;
	m_pCtrlExitBtn		 = NULL;
	m_pCtrlMoneyBtn		 = NULL;
	m_pCtrlLogBtn		 = NULL;
	m_pCtrlOutCheckBtn	 = NULL;
	m_pCtrlInCheckBtn	 = NULL;
	for(int i = 0; i < MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM; ++i)
	{
		m_pCtrlSlotCheckBtn[i] = NULL;
	}

	this->m_pGuildMan =
		static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
	assert (m_pGuildMan);
	InitContainer();
}

//////////////////////////////////////////////////////////////////////////

uiGuildWareHouse::~uiGuildWareHouse()
{
	this->Release();
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_bShow	 = FALSE;
	m_pItemUnitRenderer = NULL;
	m_pDraw = pDrawBase;

	SetMoney(0);

	InitControl();
	InitRenderer();
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::InitControl()
{
	m_pCtrlGuildNameText = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXT_GUILD_NAME));
	assert(m_pCtrlGuildNameText);
	m_pCtrlMoneyText	 = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXT_MONEY));
	assert(m_pCtrlMoneyText);

	m_pCtrlExitBtn		 = static_cast<CCtrlButtonWZ*>(getControl(DIALOG_EXIT_BTN));
	assert(m_pCtrlExitBtn);
	m_pCtrlMoneyBtn		 = static_cast<CCtrlButtonWZ*>(getControl(DIALOG_MONEY_BTN));
	assert(m_pCtrlMoneyBtn);
	m_pCtrlLogBtn		 = static_cast<CCtrlButtonWZ*>(getControl(DIALOG_LOG_BTN));
	assert(m_pCtrlLogBtn);

	for(int i = 0; i < MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM; ++i)
	{
		m_pCtrlSlotCheckBtn[i] = static_cast<CCtrlButtonCheckWZ*>(getControl(DIALOG_SLOT_CHECK_BTN1 + i));
		assert(m_pCtrlSlotCheckBtn[i]);
	}

	m_pCtrlOutCheckBtn		 = static_cast<CCtrlButtonCheckWZ*>(getControl(DIALOG_OUT_CHECK_BTN));
	assert(m_pCtrlOutCheckBtn);
	m_pCtrlInCheckBtn		 = static_cast<CCtrlButtonCheckWZ*>(getControl(DIALOG_IN_CHECK_BTN));
	assert(m_pCtrlInCheckBtn);


}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::InitContainer()
{
	SCItemSlotContainer::Init( MAX_GUILD_WAREHOUSE_SLOT_NUM, SI_GUILDWAREHOUSE );
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::InitRenderer()
{
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( m_pDraw, GetMaxSlotNum() );
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::Release()
{
	ReleaseContainer();
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::ReleaseContainer()
{
	FlushUnitDraw();
	ReleaseRenderer();
	SCItemSlotContainer::Release();
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}
}	

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::ReleaseRenderer()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

//////////////////////////////////////////////////////////////////////////

RC::eSLOT_INSERT_RESULT uiGuildWareHouse::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	// 클라에서 따로 시리얼을 발급한다.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// 인벤토리	
	CControlWZ * pCtrl = getControl( AtPos%POS_MAX_SLOT );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	//	TODO 인벤토리 확인
	ReFreshRenderSlot();

	return eInsertSlotResult;

}

//////////////////////////////////////////////////////////////////////////

BOOL uiGuildWareHouse::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	if (IsEmpty(AtPos))
	{
		return FALSE;
	}

	SCItemSlot & rItemSlot = (SCItemSlot & )GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rItemSlot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	g_SlotKeyGenerator.Restore(static_cast<DWORD>(rItemSlot.GetSerial()));

	SCSlotContainer::DeleteSlot(AtPos, pSlotOut );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::OnShowWindow( BOOL val )
{
	if (val)
	{
		m_bShow = TRUE;

		CheckTab(DIALOG_SLOT_CHECK_BTN1);
		if(g_pHero)
		{
			if(m_pCtrlGuildNameText)
				m_pCtrlGuildNameText->SetTextWZ(g_pHero->GetGuildName());

		}
		this->MoveCenter();

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SHOP, FALSE);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

		ReFreshRenderSlot();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_WareHouse;
		msg.wParam = IM_GUILD_MANAGER::GUILD_WAREHOUSE;
		msg.DoSomething = GlobalFunc::DoGuildWareHouse;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse);

		m_bShow    = FALSE;
		m_btCurTab = 0;

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, FALSE);

		m_pCtrlOutCheckBtn->SetCheckState(FALSE);
		m_pCtrlInCheckBtn->SetCheckState(FALSE);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_WITHDRAW);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_DEPOSIT);

		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::OnRenderSolarDialog()
{
	if(m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	if(m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();

	UpdateTooltip();

	TCHAR szMoney[INTERFACE_STRING_LENGTH + 1]      = {0,};
	TCHAR szMoneyFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
	Snprintf(szMoney, INTERFACE_STRING_LENGTH, _T("%I64u"), m_Money);
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));
	if(m_pCtrlMoneyText)
		m_pCtrlMoneyText->SetTextWZ(szMoneyFinal);

	if( m_pCtrlOutCheckBtn->GetCheckState() )
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::GUILD_WITHDRAW );
	}
	else if( m_pCtrlInCheckBtn->GetCheckState() )
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::GUILD_DEPOSIT );
	}
	else
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::NONE );
	}
}

void uiGuildWareHouse::UpdateTooltip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < DIALOG_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
	}
	else
	{
		if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
		{
			if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
			{

				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
					pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID ) ;

			if( pos < POS_MAX_SLOT ) 
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				POSTYPE RelativePos = pos + (m_btCurTab * POS_MAX_SLOT - POS_MAX_SLOT);//24Slot * 3Tab
				RECT rcSlot = pControl->GetSizeRect();

				MouseHandler::Instance()->ItemTransaction(SI_GUILDWAREHOUSE, RelativePos, &rcSlot);
			}
			else
			{
				switch ( pos )
				{
				case DIALOG_SLOT_CHECK_BTN1:
				case DIALOG_SLOT_CHECK_BTN2:
				case DIALOG_SLOT_CHECK_BTN3:
					{
						CheckTab(pos);
						ReFreshRenderSlot();
					}
					break;

				case DIALOG_EXIT_BTN:
					{
						GetManager()->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
					}
					break;
				
				case DIALOG_MONEY_BTN:
					{
						popupInputMoney();
					}
					break;
				case DIALOG_OUT_CHECK_BTN:
					{
						MouseHandler::Instance()->SetMouseState(MouseHandler::GUILD_WITHDRAW );
						CheckInOutBtn(DIALOG_OUT_CHECK_BTN, m_pCtrlOutCheckBtn->GetCheckState());
					}
					break;
				case DIALOG_IN_CHECK_BTN:
					{
						MouseHandler::Instance()->SetMouseState(MouseHandler::GUILD_DEPOSIT );
						CheckInOutBtn(DIALOG_IN_CHECK_BTN, m_pCtrlInCheckBtn->GetCheckState());
					}
					break;
				case DIALOG_LOG_BTN:
					{
						uiGuildWareHouseLog * pDlg = GET_CAST_DIALOG( uiGuildWareHouseLog, IM_GUILD_MANAGER::GUILD_WAREHOUSE_LOG );
						if(pDlg)
						{
							if(pDlg->IsVisible())
							{
							}
							else
							{
								MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ SendMsg;
								GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendMsg, sizeof(SendMsg));
							}
						}
					}
					break;
				}
			}
		}
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if(RT_MSG_MOUSEOVER == pMessage->eResultMsg)
	{
		POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
		POSTYPE RelativePos = pos + (m_btCurTab * POS_MAX_SLOT - POS_MAX_SLOT);//24Slot * 3Tab
		if(pos < POS_MAX_SLOT)
		{
			if (!IsEmpty( RelativePos ))
			{
				SCSlot & rSlot = GetSlot( RelativePos );
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
				m_MouseOverObject.pSlot = &rSlot;
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
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::CheckTab(int iCurCheckTab)
{
	int iCurTabNum = (iCurCheckTab - DIALOG_SLOT_CHECK_BTN1); //0,1,2
	if(m_btMaxTab < iCurTabNum)
	{
		for(int i = 0; i < MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM; ++i)
		{
			if(m_pCtrlSlotCheckBtn[i])
				m_pCtrlSlotCheckBtn[i]->SetCheckState(FALSE);
		}

		//무조건 1번 탭으로 Back
		if(m_pCtrlSlotCheckBtn[0])
		{
			m_pCtrlSlotCheckBtn[0]->SetCheckState(TRUE);
			m_btCurTab = 1;
		}
		
        /*
		TCHAR szTemp[INTERFACE_STRING_LENGTH +1] = {0,};

		if(1 == iCurTabNum) //Guild Grade
		{
			//7189	길드 등급이 낮아 사용할 수 없는 슬롯입니다.
			g_InterfaceManager.GetInterfaceString(7189, szTemp);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szTemp);
		}
		else if(2 == iCurTabNum) //cash
		{
			//7190	캐쉬 아이템이 있어야 사용할수 있는 슬롯입니다.
			g_InterfaceManager.GetInterfaceString(7190, szTemp);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szTemp);
		}
        */
	}
	else
	{
		for(int i = 0; i < MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM; ++i)
		{
			if(m_pCtrlSlotCheckBtn[i])
				m_pCtrlSlotCheckBtn[i]->SetCheckState(FALSE);
		}

		if(m_pCtrlSlotCheckBtn[iCurTabNum])
		{
			m_pCtrlSlotCheckBtn[iCurTabNum]->SetCheckState(TRUE);
			m_btCurTab = iCurCheckTab - DIALOG_SLOT_CHECK_BTN1 + 1; //1,2,3중에 하나.
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::CheckInOutBtn(eDIALOG_POS eDlgPos, bool bCheck)
{
	if(DIALOG_IN_CHECK_BTN != eDlgPos && DIALOG_OUT_CHECK_BTN != eDlgPos)
	{
		assert(!"uiGuildWareHouse::CheckInOutBtn");
		return;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

	if(DIALOG_OUT_CHECK_BTN == eDlgPos)
	{
		m_pCtrlOutCheckBtn->SetCheckState(bCheck);
		m_pCtrlInCheckBtn->SetCheckState(FALSE);

		if(TRUE == bCheck)
		{
			//	출고할 아이템을 선택해 주세요.
			g_InterfaceManager.GetInterfaceString( eST_WAREHOUSING_OF_GOOD_ASK, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
	}
	else if(DIALOG_IN_CHECK_BTN == eDlgPos)
	{
		m_pCtrlInCheckBtn->SetCheckState(bCheck);
		m_pCtrlOutCheckBtn->SetCheckState(FALSE);

		if(TRUE == bCheck)
		{
			//	입고할 아이템을 선택해 주세요.
			g_InterfaceManager.GetInterfaceString( eST_TAKE_OUT_WAREHOUSE_ASK, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
	}
}

//////////////////////////////////////////////////////////////////////////

uiGuildMan* uiGuildWareHouse::GetManager()
{
	return m_pGuildMan;
}

void uiGuildWareHouse::popupInputMoney()
{
	InputNumBankDialog *pInputNumBankDlg;

	pInputNumBankDlg =
		(InputNumBankDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);

	if (pInputNumBankDlg)
		g_InterfaceManager.DeleteInterface(pInputNumBankDlg->rcinfo_GetID());

	pInputNumBankDlg = new InputNumBankDialog;
	assert(pInputNumBankDlg);
	if ( !g_InterfaceManager.LoadInterface(
		pInputNumBankDlg,
		(char *)pInputNumBankDlg->rcinfo_GetPath(),
		pInputNumBankDlg->rcinfo_GetID(),
		FALSE) 
		)
	{
		return;
	}

    pInputNumBankDlg->SetDialogKey( pInputNumBankDlg->rcinfo_GetID() );
    pInputNumBankDlg->SetDialogResourceName( (char*)pInputNumBankDlg->rcinfo_GetPath() );

	pInputNumBankDlg->SetWareHouseType(eWAREHOUSE_TYPE_GUILD);
	g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, TRUE);
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::ReFreshRenderSlot()
{
	if ( !this->IsVisibleWindow() )
	{
		return;
	}

	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if (rSlot.GetSerial())
		{
			if ((i >= m_btCurTab * POS_MAX_SLOT - POS_MAX_SLOT) && (i < m_btCurTab * POS_MAX_SLOT))
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여줘야할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender )
				{
					pCtrl = getControl( i%POS_MAX_SLOT );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
				}

				if (pRender)
					pRender->SetShow(TRUE);
			}
			else
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여지지 말아야 할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender )
				{
					pCtrl = getControl( i%POS_MAX_SLOT );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
				}

				if (pRender)
					pRender->SetShow(FALSE);				
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

SCSlotContainer* uiGuildWareHouse::GetSlotContainer()
{
	return static_cast<SCSlotContainer *>(this);
}

//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouse::Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType )
{
	SCItemSlotContainer::Serialize( pos, bitstream, eType );

	if( eType == eSLOT_SERIALIZE_CLIENT_SAVE )
	{
		// 클라에서 따로 시리얼을 발급한다.
		DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
		SCSlot & rSetSlot = GetSlot(pos);
		rSetSlot.SetSerial(dwSerial);

		/// 인벤토리	
		CControlWZ * pCtrl = getControl( pos%POS_MAX_SLOT );
		RECT rect = pCtrl->GetSizeRect();

		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);	
		}
		else
		{
			assert(!"m_pItemUnitRenderer == NULL");
		}
	}
}
//------------------------------------------------------------------------------
void uiGuildWareHouse::InitButton()
{
    if (m_pCtrlInCheckBtn != NULL)
    {
        m_pCtrlInCheckBtn->SetCheckState(false);
    }
    if (m_pCtrlOutCheckBtn != NULL)
    {
        m_pCtrlOutCheckBtn->SetCheckState(false);
    }
}

void uiGuildWareHouse::CloseSlotInfo( int maxTab )
{

}
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
