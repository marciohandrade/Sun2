//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "bankdialog.h"
#include "InterfaceManager.h"
#include "ItemUnitRenderer.h"
#include "ItemManager.h"
#include "MouseHandler.h"
#include <SCItemSlot.h>
#include "ObjectManager.h"
#include "InputNumBankDialog.h"
#include "chatdialog.h"
#include "SlotKeyGenerator.h"
#include "Mouse.h"
#include "CursorChangeTransact.h"
#include "hero.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "HeroActionInput.h"

#include "uiSystemMan/uiSystemMan.h"


#include "uiBankMan/uiBankMan.h"
#include "uiPasswordMan/uiPassWordMan.h"


#include "BitStream.hpp"

WzID BankDialog::m_wzId[Dialog_MAX] = {
	StrToWzID("B001"),	//BANK_BT_SLOT_1 = 0,
	StrToWzID("B002"),	//BANK_BT_SLOT_2,
	StrToWzID("B003"),	//BANK_BT_SLOT_3,
	StrToWzID("B004"),	//BANK_BT_SLOT_4,
	StrToWzID("B005"),	//BANK_BT_SLOT_5,
	StrToWzID("B006"),	//BANK_BT_SLOT_6,
	StrToWzID("B007"),	//BANK_BT_SLOT_7,
	StrToWzID("B008"),	//BANK_BT_SLOT_8,
	StrToWzID("B009"),	//BANK_BT_SLOT_9,
	StrToWzID("B010"),	//BANK_BT_SLOT_10,
	StrToWzID("B011"),	//BANK_BT_SLOT_11,
	StrToWzID("B012"),	//BANK_BT_SLOT_12,
	StrToWzID("B013"),	//BANK_BT_SLOT_13,
	StrToWzID("B014"),	//BANK_BT_SLOT_14,
	StrToWzID("B015"),	//BANK_BT_SLOT_15,
	StrToWzID("B016"),	//BANK_BT_SLOT_16,
	StrToWzID("B017"),	//BANK_BT_SLOT_17,
	StrToWzID("B018"),	//BANK_BT_SLOT_18,
	StrToWzID("B019"),	//BANK_BT_SLOT_19,
	StrToWzID("B020"),	//BANK_BT_SLOT_20,
	StrToWzID("C000"),	//BANK_CHECK_1,
	StrToWzID("C001"),	//BANK_CHECK_2,
	StrToWzID("C002"),	//BANK_CHECK_3,
	StrToWzID("C003"),	//BANK_CHECK_4,
	StrToWzID("C004"),	//BANK_CHECK_5,
	StrToWzID("T003"),	//BANK_TEXT_MONEY,	// ���ֳĸ� ��Ÿ��.	
	StrToWzID("B021"),	//BANK_BT_MONEY,		// ���� ������ �� ���������� ���� ��ư
	StrToWzID("C005"),	//BANK_CHECK_WITHDRAW,		// ������
	StrToWzID("C006"),	//BANK_CHECK_DEPOSIT,		// �ִ�.
	StrToWzID("B999"),	//BANK_BT_EXIT,
	StrToWzID("TI00"),	//BANK_TI_EXTRA_BANK,

	StrToWzID("BT02"),	//BANK_BTN_UNLOCK,
	StrToWzID("BT00"),	//BANK_BTN_LOCK,

	StrToWzID("T999"),	//BANK_TXT_TITLE,
	StrToWzID("P033"), //DIALOG_LOCK_PICTURE_P033
	StrToWzID("P032"), //DIALOG_LOCK_PICTURE_P032
	StrToWzID("P034"), //DIALOG_LOCK_PICTURE_P034

};


//----------------------------------------------------------------------------
/**
*/
BankDialog::BankDialog(InterfaceManager* pUIMan)
	:uiBase(pUIMan)
{
	SCItemSlotContainer::Init( MAX_WAREHOUSE_SLOT_NUM, SI_WAREHOUSE );


	this->m_pBankMan =
		static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
	assert (m_pBankMan);


	m_iTooltipStringCode	= 0;
	m_UseTooltipCtrls.clear();

	m_UseTooltipCtrls[BANK_BTN_UNLOCK]			= 5570;	
	m_UseTooltipCtrls[BANK_BTN_LOCK]			= 5569;

	m_TabNum				= 0;
	this->m_eCheckType		= eBANK_CHECK_TYPE_MAX;
	m_iClickTab				= 0;
	m_pItemUnitRenderer		= NULL;
	m_Money					= 0;
	m_bStarted				= FALSE;

	SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY);

}


//----------------------------------------------------------------------------
/**
*/
BankDialog::~BankDialog(void) 
{
	m_UseTooltipCtrls.clear();
	SCItemSlotContainer::Release();
}


//----------------------------------------------------------------------------
/**
*/
VOID
BankDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	// �ʱ�ȭ �ؾ��� ������
	m_TabNum = 0;
	m_bSendPacket = FALSE;
	m_pItemUnitRenderer = NULL;
	m_bStarted = FALSE;
	

	SetMoney(0);

	GetControls();

	CheckBankTab(0);
	initBankBtn();

	this->m_packetStatus = 0;

	RECT rc;
	GetDialogWZ()->GetDlgSize(&rc);

	long width = rc.right;
	long height = rc.bottom;

	float mw = (long)width * 0.5f;
	float mh = (long)height * 0.5f;

	RECT *rcWindow = g_InterfaceManager.GetWindowRC();

	long sw;
	long sh;
	if (rcWindow)
	{
		sw = (rcWindow->right - rcWindow->left);
		sh = (rcWindow->bottom - rcWindow->top);
	}
	else
	{
		sw = 1024; sh = 768;
	}

	float pw = (sw * 0.5f) - mw;
	float ph = (sh * 0.5f) - mh;

	MoveWindowWZ(pw, ph);

	CCtrlPictureWZ* LockImage1= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P033);
	if(LockImage1)
	{
		LockImage1->ShowWindowWZ(WZ_HIDE);
	}
	CCtrlPictureWZ* LockImage2= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P032);
	if(LockImage2)
	{
		LockImage2->ShowWindowWZ(WZ_HIDE);
	}
	CCtrlPictureWZ* LockImage3= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P034);
	if(LockImage3)
	{
		LockImage3->ShowWindowWZ(WZ_HIDE);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	SCItemSlot FromSlot;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}
}	

VOID			
BankDialog::SendWareHouseStart()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (this->m_bStarted)
	{
		//	â�� ���� �����Դϴ�.
		g_InterfaceManager.GetInterfaceString( eST_WAREHOUSE_ALREADY_OPEN, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return;
	}

#if defined ( _DEBUG )
	//	â�� ������ ������ �Դϴ�...
	g_InterfaceManager.GetInterfaceString( eST_SEND_WAREHOUSE_START, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
#endif

	if ( BIT_CHECK( this->m_packetStatus, eBANK_START_PACKET ) )
	{
		return;
	}

	BIT_ADD( this->m_packetStatus, eBANK_START_PACKET );


	GetManager()->Send_CG_WAREHOUSE_START_SYN();

}

VOID			
BankDialog::SendWareHouseEnd()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (!m_bStarted)
	{
		return;
	}

	if ( BIT_CHECK( this->m_packetStatus, eBANK_END_PACKET ) )
	{
		//	â�� ������ ���� �� �Դϴ�.
		g_InterfaceManager.GetInterfaceString( eST_SEND_WAREHOUSE_END, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return;
	}

	BIT_ADD( this->m_packetStatus, eBANK_END_PACKET );

	// â�� ���� ������ �޾ƿ� ���¶��
	MSG_CG_WAREHOUSE_END_SYN SendEndPacket;
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendEndPacket, sizeof(SendEndPacket) );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::InitRenderer()
{
	// �ӽ�
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( m_pDraw, GetMaxSlotNum() );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			GetControls();

			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			if( pos < BANK_MAX_SLOT ) 
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				POSTYPE RelativePos = GetRealSlotPos(pos);
				RECT rcSlot = pControl->GetSizeRect();		

				MouseHandler::Instance()->ItemTransaction(SI_WAREHOUSE, 
					                                      RelativePos, 
					                                      &rcSlot);
			}
			else
			{
				switch(pos)
				{
				case BANK_CHECK_1:
					{
						m_iClickTab = 0;
						CheckBankTab(m_iClickTab);
						this->ReFreshRenderSlot();
					}
					break;
				case BANK_CHECK_2:
					{
						m_iClickTab = 1;
						CheckBankTab(m_iClickTab);
						this->ReFreshRenderSlot();
					}
					break;
				case BANK_CHECK_3:
					{
						m_iClickTab = 2;
						CheckBankTab(m_iClickTab);
						this->ReFreshRenderSlot();
					}
					break;
				case BANK_CHECK_4:
					{
						m_iClickTab = 3;
						CheckBankTab(m_iClickTab);
						this->ReFreshRenderSlot();
					}
					break;
				case BANK_CHECK_5:
					{
						m_iClickTab = 4;
						CheckBankTab(m_iClickTab);
						this->ReFreshRenderSlot();
					}
					break;

				case BANK_BT_MONEY:
					{
                        popupInputMoney();
					}
					break;

				case BANK_BT_EXIT:
					{
						this->SendWareHouseEnd();
					}
					break;

				case BANK_CHECK_WITHDRAW:	// ���
					{
						MouseHandler::Instance()->SetMouseState( MouseHandler::WITHDRAW );
						CheckBankBtn(eBANK_CHECK_TYPE_WITHDRAW);

						//	����� �������� ������ �ּ���.
						g_InterfaceManager.GetInterfaceString( eST_WAREHOUSING_OF_GOOD_ASK, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
					break;
				
				case BANK_CHECK_DEPOSIT:	// �԰�
					{
						MouseHandler::Instance()->SetMouseState( MouseHandler::DEPOSIT );
						CheckBankBtn(eBANK_CHECK_TYPE_DEPOSIT);

						//	�԰��� �������� ������ �ּ���.
						g_InterfaceManager.GetInterfaceString( eST_TAKE_OUT_WAREHOUSE_ASK, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
					break;

				case BANK_BTN_UNLOCK:
					{
						int iStrIndex;
						if(GetBankLockState() == MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY)
						{
							//	5584	â�� ��ȣ ����� ����Ͻðڽ��ϱ�? ��й�ȣ�� ������ ���, ���� ���� �� �ڵ� ��� ���·� ����˴ϴ�.
							iStrIndex = 5584;
						}
						else
						{
							//	5580	â�� ��� ���·� �����Ͻðڽ��ϱ�?							
							iStrIndex = 5580;
						}
						PasswordLockingSyn(iStrIndex);

					}
					break;

				case BANK_BTN_LOCK:
					{
						//	5581	â�� ���� ���·� �����Ͻðڽ��ϱ�?
						PasswordLockingSyn(5581);						
					}
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

void BankDialog::PasswordLockingSyn( DWORD dwStrIndex )
{
	uiSystemMan* pSystemMan =
		static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
	if(pSystemMan)
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0, };
		g_InterfaceManager.GetInterfaceString( dwStrIndex, szMessage, INTERFACE_STRING_LENGTH );
		pSystemMan->Sys_Confirm(szMessage, GlobalFunc::_fn_uiBankPasswordLockingSyn);
	}
}

//----------------------------------------------------------------------------
/**
*/
void BankDialog::SetPasswordDlg(int iStringTableNum, int iPasswordDlgState)
{
	int iMinCount = 0;
	int iMaxCount = MAX_SSN_COUNT;

	GetManager()->SetPasswordState(iPasswordDlgState);
	if(iPasswordDlgState == eBankDlgState_Setting_SSN || iPasswordDlgState == eBankDlgState_Check_Clear)
	{ 
		iMinCount = 0; 
		iMaxCount = MAX_SSN_COUNT;
	}
	else
	{ 
		iMinCount = 4; 
		iMaxCount = MAX_WAREHOUSE_INVENTORY_PWD_LENGTH;
	}

    BOOL showPassChgBtn = TRUE;

	showPassChgBtn = ( GetManager()->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);
	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(iStringTableNum, GlobalFunc::_fn_uiBankPassword, iMinCount, iMaxCount, showPassChgBtn, ePassWordCalledState_WareHouse);
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID	
BankDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			POSTYPE RelativePos = GetRealSlotPos(pos);

			if (pos >= 0 && pos < BANK_MAX_SLOT)
			{
				// Bank
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
				POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

				m_iTooltipStringCode = getTooltipString(pos);

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
BankDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
		// ���̾�α׸� ��������.
		RECT rc;
		GetDialogWZ()->GetDlgSize(&rc);

		long width = rc.right;
		long height = rc.bottom;

		float mw = (long)width * 0.5f;
		float mh = (long)height * 0.5f;

		RECT *rcWindow = g_InterfaceManager.GetWindowRC();

		long sw;
		long sh;
		if (rcWindow)
		{
			sw = (rcWindow->right - rcWindow->left);
			sh = (rcWindow->bottom - rcWindow->top);
		}
		else
		{
			sw = 1024; sh = 768;
		}

		float pw = (float)(sw * 0.5f) - mw;
		float ph = (float)(sh * 0.5f) - mh;

		MoveWindowWZ(pw, ph);


		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SHOP, FALSE);

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

		this->m_iClickTab = 0;
		initBankBtn();
		CheckBankTab(m_iClickTab);
		this->ReFreshRenderSlot();

		initBankLockBtn();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_bank;
		msg.wParam=IM_BANK_MANAGER::BANK_DIALOG;
		msg.DoSomething=GlobalFunc::DoBank;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		if (g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_EQUIP)
			&& FALSE == g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_SHOP))
			g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);

		if ( m_bStarted && false == BIT_CHECK(this->m_packetStatus, eBANK_END_PACKET) )
		{
			SendWareHouseEnd();
		}		

		this->SetMoney(0);
		m_bStarted = FALSE;

		this->m_packetStatus = 0;

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_bank);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, FALSE); 
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::WITHDRAW);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::DEPOSIT);
		
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::NetworkProc( MSG_BASE * pMsg )
{
	if(pMsg->m_byCategory == CG_WAREHOUSE)
	{
	
		GetManager()->ClearWaitPacket();
	
		switch ( pMsg->m_byProtocol )
		{
		case CG_WAREHOUSE_START_ACK:
			{
				MSG_CG_WAREHOUSE_START_ACK * pRecvMsg = (MSG_CG_WAREHOUSE_START_ACK *)pMsg;

				FlushUnitDraw();

				BitStream bitstream(pRecvMsg->m_BitStreamBuffer, pRecvMsg->m_Size, FALSE);
				SCItemSlotContainer::Serialize(	bitstream, eSLOT_SERIALIZE_CLIENT_SAVE );

	
				SetBankLockState(pRecvMsg->m_byWarehouseLock);
	
				// â�� ����
				SetMoney(pRecvMsg->m_WarehouseMoney);
				// ���� ������
				ReFreshRenderSlot();
				m_bStarted = TRUE;
				BIT_REMOVE( this->m_packetStatus, eBANK_START_PACKET );

				ShowDialog(TRUE);
			}
			break;

		case CG_WAREHOUSE_START_NAK:
			{
				MSG_CG_WAREHOUSE_START_NAK * pRecvMsg = (MSG_CG_WAREHOUSE_START_NAK *)pMsg;

				m_bStarted = FALSE;
				BIT_REMOVE( this->m_packetStatus, eBANK_START_PACKET );

				TCHAR szText[INTERFACE_STRING_LENGTH];
				
				g_InterfaceManager.GetStringWarehouseError( pRecvMsg->m_dwErrorCode, szText, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, szText );

				g_InterfaceManager.ShowWindow(IM_BANK_MANAGER::BANK_DIALOG, FALSE);
			}
			break;

	
		case CG_WAREHOUSE_PWD_LOCKING_ACK:
			{
				MSG_CG_WAREHOUSE_PWD_LOCKING_ACK* pRecvMsg = (MSG_CG_WAREHOUSE_PWD_LOCKING_ACK *)pMsg;

				TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0, };

				switch( pRecvMsg->m_byRetCode )
				{
				case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_SUCCESS_LOCKED:
					{
						SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST);
						initBankLockBtn();
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	â�� ��� ���·� ����Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString( 5573, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
						GetManager()->ClearPassword();
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_LOCKING:
					{
						GetManager()->Send_CG_WAREHOUSE_PWD_SETTING_SYN(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_FIRST);
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_UNLOCKING:
					{
						GetManager()->Send_CG_WAREHOUSE_PWD_CHECK_SYN(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_FIRST);
					}
					break;
				}
			}
			break;

		case CG_WAREHOUSE_PWD_LOCKING_NAK:
			{
				MSG_CG_WAREHOUSE_PWD_LOCKING_NAK* pRecvMsg = (MSG_CG_WAREHOUSE_PWD_LOCKING_NAK *)pMsg;

				TCHAR szMessage[INTERFACE_STRING_LENGTH];

				GetManager()->ClearPassword();
				g_InterfaceManager.GetStringWarehouseError( pRecvMsg->m_byErrCode, szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, szMessage );
			}
			break;

		case CG_WAREHOUSE_PWD_CHECK_ACK:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				MSG_CG_WAREHOUSE_PWD_CHECK_ACK* pRevMsg = (MSG_CG_WAREHOUSE_PWD_CHECK_ACK *)pMsg;

				GetManager()->ClearPassword();

				switch(pRevMsg->m_byRetCode)
				{
				case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS:
					{
						SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEXIST);
						initBankLockBtn();
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	â�� ���� ���·� ����Ǿ����ϴ�
							g_InterfaceManager.GetInterfaceString( 5575, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_PWD:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eBankDlgState_Check_Unlock);
							//	��й�ȣ�� Ʋ�Ƚ��ϴ�.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiBankPassword_invalidPassword);
						}
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_PWD:
					{
						//	��й�ȣ�� �Է����ּ���
						SetPasswordDlg(5551, eBankDlgState_Check_Unlock);
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_SSN:
					{
						//	�ֹε�Ϲ�ȣ ���ڸ� �Է�
						SetPasswordDlg(5579, eBankDlgState_Check_Clear);
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_SSN:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eBankDlgState_Check_Clear);
							//	�ֹε�Ϲ�ȣ�� ��ġ���� �ʽ��ϴ�.
							g_InterfaceManager.GetInterfaceString( 5845, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiBankPassword_invalidSSN);
						}
					}
					break;

				default:
					break;
				}
			}
			break;

		case CG_WAREHOUSE_PWD_SETTING_ACK:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				MSG_CG_WAREHOUSE_PWD_SETTING_ACK * pRecvMsg = (MSG_CG_WAREHOUSE_PWD_SETTING_ACK *)pMsg;

				//	�������� ���� ���ο� �н���������.. �н����带 �����ϴ°������� ���� ������ �ټ� ���ٰ� �ؼ�.. �̷� �����ιڿ�.. -_-
				//	�����ϰ� Ŭ��� ���� ���Ѵ�.. ��.. -_-;; �ȴ�..

				switch(pRecvMsg->m_byRetCode)
				{
				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL:
				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_CANCELED:
					GetManager()->ClearPassword();
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS:
					{
						GetManager()->ClearPassword();
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	��й�ȣ�� �����Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString( 5556, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
						SetBankLockState( MSG_CG_WAREHOUSE_START_ACK::ENABLEnEXIST );
						initBankLockBtn();
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_NEED_SSN:
					{
						//	�ֹε�Ϲ�ȣ ���ڸ� �Է�
						SetPasswordDlg(5579, eBankDlgState_Setting_SSN);
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_INVALID_SSN:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eBankDlgState_Setting_SSN);
							//	�ֹε�Ϲ�ȣ�� ��ġ���� �ʽ��ϴ�.
							g_InterfaceManager.GetInterfaceString( 5845, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiBankPassword_invalidSSN);
						}
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_NEED_PWD:
					{
						//	��й�ȣ�� �������ּ���
						SetPasswordDlg(5578, eBankDlgState_Setting_PasswordCmp);
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_INVALID_PWD:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eBankDlgState_Lock);
							//	��й�ȣ�� Ʋ�Ƚ��ϴ�.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiBankPassword_invalidPassword);
						}
					}
					break;

				case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS_CLEAR:
					{
						SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY);
						initBankLockBtn();
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	��� ����� [��� �� ��]���� �����Ͽ����ϴ�.
							g_InterfaceManager.GetInterfaceString( 5577, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
					}
					break;
				
							
				default:
					break;
				}
			}
			break;

		case CG_WAREHOUSE_PWD_SETTING_NAK:
			{
				GetManager()->ClearPassword();
			}
			break;

	

		case CG_WAREHOUSE_MONEY_ACK:
			{
				MSG_CG_WAREHOUSE_MONEY_ACK * pRecvMsg = (MSG_CG_WAREHOUSE_MONEY_ACK *)pMsg;

				if(g_pHero)
					g_pHero->SetMoney(pRecvMsg->m_InventoryMoney);

				SetMoney( pRecvMsg->m_WarehouseMoney );

				InputNumBankDialog * pInputNumBankDlg =
					(InputNumBankDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);
				if (pInputNumBankDlg)
				{
					pInputNumBankDlg->ClearPacketFlag();
				}		
			}
			break;

		case CG_WAREHOUSE_MONEY_NAK:
			{
				MSG_CG_WAREHOUSE_MONEY_NAK * pRecvMsg = (MSG_CG_WAREHOUSE_MONEY_NAK *)pMsg;

				InputNumBankDialog * pInputNumBankDlg =
					(InputNumBankDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);
				if (pInputNumBankDlg)
				{
					pInputNumBankDlg->ClearPacketFlag();
				}		

                TCHAR szText[INTERFACE_STRING_LENGTH];

                BOOL ret = g_InterfaceManager.GetStringWarehouseError( pRecvMsg->m_dwErrorCode, szText, INTERFACE_STRING_LENGTH);
                if( ret )
                {
                    GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, szText );
                }
                else
                {
                    ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_dwErrorCode);
                }
			}
			break;

		case CG_WAREHOUSE_END_ACK:
			{
				// ���������� â�� ���´�.
				ShowDialog(FALSE);
				m_bStarted = FALSE;

				BIT_REMOVE( this->m_packetStatus, eBANK_END_PACKET );
	#if defined ( _DEBUG )
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("���������� â�� �����Ͽ����ϴ�."));
	#endif
			}
			break;

		case CG_WAREHOUSE_END_NAK:
			{
				MSG_CG_WAREHOUSE_END_NAK * pRecvMsg = (MSG_CG_WAREHOUSE_END_NAK *)pMsg;

				BIT_REMOVE( this->m_packetStatus, eBANK_END_PACKET );
				m_bStarted = FALSE;

	#if defined ( _DEBUG )
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Ư�������� â�� ������ �������� �� �Ͽ����ϴ�."));
	#endif

                TCHAR szText[INTERFACE_STRING_LENGTH];

                BOOL ret = g_InterfaceManager.GetStringWarehouseError( pRecvMsg->m_dwErrorCode, szText, INTERFACE_STRING_LENGTH);
                if( ret )
                {
                    GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, szText );
                }
                else
                {
                    ItemManager::Instance()->NoticeReasonItemErrorCode( pRecvMsg->m_dwErrorCode );
                }
			}
			break;

		default:
			{
				assert(!"���ǵ��� ���� â�� ��Ŷ");
			}
			break;
		}
	}
}

//----------------------------------------------------------------------------
/**
*/

uiBankMan* BankDialog::GetManager()
{
	return m_pBankMan;
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
BankDialog::getControl( POSTYPE AtPos )
{
	if (AtPos >= Dialog_MAX)
	{
		return NULL;
	}

	return GetControlWZ( m_wzId[AtPos] );
}

//----------------------------------------------------------------------------
/**
*/

void BankDialog::updateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
	BOOL bMouseOver = FALSE;
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
					bMouseOver = TRUE;
					break;
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		m_iTooltipStringCode = 0;
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
		else if(m_iTooltipStringCode > 0)
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterTooltipForWareNInventoryLock(m_iTooltipStringCode);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}
//----------------------------------------------------------------------------
/**
*/
int	BankDialog::getTooltipString(POSTYPE pos)
{
	map<int,int>::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);

	if (iter == m_UseTooltipCtrls.end())
		return 0;

	return iter->second;
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	
BankDialog::getCtrlIDToPos( WzID wzId )
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


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::OnUpdateSolarDialog()
{
	g_HeroInput.ClearPickingTarget();

	m_pItemUnitRenderer->Update();


	updateToolTip();

	CCtrlStaticWZ * pStatic = (CCtrlStaticWZ *) getControl(BANK_TEXT_MONEY); 
	if (pStatic)
	{
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		Sprintf(m_szResult, _T("%I64u"), m_Money);
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, m_szResult, &nFmt, m_szNumber, sizeof(m_szNumber));
		GlobalFunc::SetCaption( pStatic, m_szNumber );	
	}

	if( m_pCtrlCheckBtn[eBANK_CHECK_TYPE_WITHDRAW]->GetCheckState() )
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::WITHDRAW );
	}
	else if( m_pCtrlCheckBtn[eBANK_CHECK_TYPE_DEPOSIT]->GetCheckState() )
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::DEPOSIT );
	}
	else
	{
		MouseHandler::Instance()->SetMouseState( MouseHandler::NONE );
	}

}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
	    m_pItemUnitRenderer->Render();
    }
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
BankDialog::UpdateData(BOOL bTake)
{
	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	
BankDialog::GetRealSlotPos( POSTYPE pos )
{
	return pos + m_TabNum;
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	
BankDialog::GetRelativeSlotPos( POSTYPE pos )
{
	return pos % BANK_MAX_SLOT;
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		BankDialog::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// ���Կ� ����
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	// Ŭ�󿡼� ���� �ø����� �߱��Ѵ�.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// �κ��丮	
	CControlWZ * pCtrl = getControl( GetRelativeSlotPos(AtPos) );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

//	TODO �κ��丮 Ȯ��
	ReFreshRenderSlot();

	return eInsertSlotResult;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
BankDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
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
		assert(m_pItemUnitRenderer);
	}

	g_SlotKeyGenerator.Restore((DWORD)rItemSlot.GetSerial());

	SCSlotContainer::DeleteSlot(AtPos, pSlotOut );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
VOID BankDialog::OnInsert( SCSlot & IN rSlotIn )
{ 
	ItemManager::Instance()->AddUpdateItemSlot(&rSlotIn);
}

//----------------------------------------------------------------------------
/**
*/
VOID	
BankDialog::OnDelete( SCSlot & IN rSlotIn )
{
	ItemManager::Instance()->RemoveUpdateItemSlot(&rSlotIn);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::ReFreshRenderSlot()
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
			if ((i >= m_TabNum) && 
                (i < (m_TabNum + BANK_MAX_SLOT)))
 			{
				// ��������� ���
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
					pRender->SetShow(TRUE);
			}
			else
			{
				// �������� ���ƾ� �� ���
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

//----------------------------------------------------------------------------
/**
*/
void BankDialog::Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType )
{
	SCItemSlotContainer::Serialize( pos, bitstream, eType );

	if( eType == eSLOT_SERIALIZE_CLIENT_SAVE )
	{
		// Ŭ�󿡼� ���� �ø����� �߱��Ѵ�.
		DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
		SCSlot & rSetSlot = GetSlot(pos);
		rSetSlot.SetSerial(dwSerial);

		/// �κ��丮	
		CControlWZ * pCtrl = getControl( GetRelativeSlotPos(pos) );
		RECT rect = pCtrl->GetSizeRect();

		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);	
		}
		else
		{
			assert(m_pItemUnitRenderer);
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::InitContainer()
{
	InitRenderer();	
}

//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::ReleaseContainer()
{
	FlushUnitDraw();

	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::initBankBtn()
{
	for (int i = 0; i < eBANK_CHECK_TYPE_MAX; ++i)
	{
		m_pCtrlCheckBtn[i]->SetCheckState(FALSE);
	}
}

//----------------------------------------------------------------------------
/**
*/

void BankDialog::InitTitleByLockState( int iLockState )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if(m_TxtCtrlTitle)
	{
		if(iLockState == MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST)
		{
			// â��(���)
			g_InterfaceManager.GetInterfaceString( 1189, szMessage, INTERFACE_STRING_LENGTH );
		}
		else
		{
			// â��(����)
			g_InterfaceManager.GetInterfaceString( 1190, szMessage, INTERFACE_STRING_LENGTH );
		}

		GlobalFunc::SetCaption(m_TxtCtrlTitle, szMessage);

	}
}
//-------------------------------------------------------------------------------------------
/**
*/
void BankDialog::initBankLockBtn()
{
#ifdef _GS_GSP_REMOVE_LOCKBTN_
	//����ư�� �ƿ� ���� ���繹��
	m_pCtrlBtnLock[eBANK_LOCK_STATE_UNLOCK]->ShowWindowWZ(WZ_HIDE);
	m_pCtrlBtnLock[eBANK_LOCK_STATE_LOCK]->ShowWindowWZ(WZ_HIDE);
	InitTitleByLockState( GetBankLockState() );
#else
	if( GetBankLockState() == MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST )
	{
		m_pCtrlBtnLock[eBANK_LOCK_STATE_LOCK]->ShowWindowWZ(WZ_SHOW);
		m_pCtrlBtnLock[eBANK_LOCK_STATE_UNLOCK]->ShowWindowWZ(WZ_HIDE);
	}
	else
	{
		m_pCtrlBtnLock[eBANK_LOCK_STATE_UNLOCK]->ShowWindowWZ(WZ_SHOW);
		m_pCtrlBtnLock[eBANK_LOCK_STATE_LOCK]->ShowWindowWZ(WZ_HIDE);
	}

	InitTitleByLockState( GetBankLockState() );
#endif//_GS_GSP_REMOVE_LOCKBTN_
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::CheckBankBtn(eBANK_CHECK_TYPE eCheckBtn)
{
	for (int i = 0; i < eBANK_CHECK_TYPE_MAX; ++i)
	{
		m_pCtrlCheckBtn[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckBtn[eCheckBtn]->SetCheckState(TRUE);	
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::CheckBankTab(int iTab)
{
	assert(iTab < BANK_CHECK_TAB_MAX);
	
	for (int i = 0; i < BANK_CHECK_TAB_MAX; ++i)
	{
		m_pCtrlCheckTab[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckTab[iTab]->SetCheckState(TRUE);
	m_iClickTab = iTab;

	m_TabNum = m_iClickTab * BANK_MAX_SLOT;
}


//----------------------------------------------------------------------------
/**
*/
VOID		
BankDialog::ResetCheckBankTab()
{
	for (int i = 0; i < BANK_CHECK_TAB_MAX; ++i)
	{
		m_pCtrlCheckTab[i]->SetCheckState(FALSE);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::GetControls()
{
	int i = 0;
	
	for (i = 0; i < BANK_CHECK_TAB_MAX; ++i)

	{
		m_pCtrlCheckTab[i] = (CCtrlButtonCheckWZ *)getControl(BANK_CHECK_1+i);
		assert(m_pCtrlCheckTab[i]);
	}
	
	for (i = 0; i < eBANK_CHECK_TYPE_MAX; ++i)
	{
		m_pCtrlCheckBtn[i] = (CCtrlButtonCheckWZ *)getControl(BANK_CHECK_WITHDRAW+i);
		assert(m_pCtrlCheckBtn[i]);
	}


	for (i = 0; i < eBANK_LOCK_STATE_MAX; ++i)
	{
		m_pCtrlBtnLock[i] = (CCtrlButtonWZ *)getControl(BANK_BTN_UNLOCK+i);
		assert(m_pCtrlBtnLock[i]);
	}

	m_TxtCtrlTitle = (CCtrlStaticWZ *)getControl(BANK_TXT_TITLE);
	assert(m_TxtCtrlTitle);


	m_pCtrlMoneyStatic = (CCtrlStaticWZ *)getControl(BANK_TEXT_MONEY);
	GlobalFunc::SetCaption( m_pCtrlMoneyStatic, _T("0") );

}

//----------------------------------------------------------------------------
/**
*/
VOID 
BankDialog::popupInputMoney()
{
    InputNumBankDialog *pInputNumBankDlg;

    pInputNumBankDlg =
        (InputNumBankDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);

    if (pInputNumBankDlg)
        g_InterfaceManager.DeleteInterface(pInputNumBankDlg->rcinfo_GetID());

    pInputNumBankDlg = new InputNumBankDialog;
    assert(pInputNumBankDlg);

	if(pInputNumBankDlg)
	{
		if ( !g_InterfaceManager.LoadInterface(
												pInputNumBankDlg,
												(char *)pInputNumBankDlg->rcinfo_GetPath(),
												pInputNumBankDlg->rcinfo_GetID(),
												FALSE) 
			)
		{
			return;
		}

		pInputNumBankDlg->SetWareHouseType(eWAREHOUSE_TYPE_BANK);

	    g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, TRUE);
	}

}

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
