#include "SunClientPrecompiledHeader.h"


#include "uiPasswordDlg.h"
#include "uiPasswordMan/uiPassWordMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "GameFramework.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiinventoryman/uiInventoryMan.h"
#include "uibankman/uiBankMan.h"


//------------------------------------------------------------------------------
WzID uiPasswordDlg::m_wzId[eDIALOG_PASSWORD_DLG_MAX] = 
{
	StrToWzID("B001"),	//	eDIALOG_PASSWORD_DLG_BTN_EXIT = 0,			//	X ��ư

	StrToWzID("T001"),	//	eDIALOG_PASSWORD_DLG_TXT_MESSAGE,			//	�н����� �޽���

	StrToWzID("ST00"),	//	eDIALOG_PASSWORD_DLG_TXT,					//	�н����� �ؽ�Ʈ

	StrToWzID("B010"),	//	eDIALOG_PASSWORD_DLG_BTN_1,					//	1�� ��ư
	StrToWzID("B011"),	//	eDIALOG_PASSWORD_DLG_BTN_2,					//	2�� ��ư
	StrToWzID("B012"),	//	eDIALOG_PASSWORD_DLG_BTN_3,					//	3�� ��ư
	StrToWzID("B006"),	//	eDIALOG_PASSWORD_DLG_BTN_4,					//	4�� ��ư
	StrToWzID("B007"),	//	eDIALOG_PASSWORD_DLG_BTN_5,					//	5�� ��ư
	StrToWzID("B008"),	//	eDIALOG_PASSWORD_DLG_BTN_6,					//	6�� ��ư
	StrToWzID("B002"),	//	eDIALOG_PASSWORD_DLG_BTN_7,					//	7�� ��ư
	StrToWzID("B003"),	//	eDIALOG_PASSWORD_DLG_BTN_8,					//	8�� ��ư
	StrToWzID("B004"),	//	eDIALOG_PASSWORD_DLG_BTN_9,					//	9�� ��ư
	StrToWzID("B014"),	//	eDIALOG_PASSWORD_DLG_BTN_0,					//	0�� ��ư
#ifdef _GS_ADD_PWD_X_BTN_1143
	StrToWzID("BT01"),	//	eDIALOG_PASSWORD_DLG_BTN_X,					//	X�� ��ư(�빮��)
#endif//_GS_ADD_PWD_X_BTN_1143(�빮��)

#ifdef _GS_ADD_PWD_X_BTN_1245
	StrToWzID("BT02"),	//	eDIALOG_PASSWORD_DLG_BTN_X2,				//	X�� ��ư(�ҹ���)
#endif//_GS_ADD_PWD_X_BTN_1245(�ҹ���)

	StrToWzID("BT00"),	//	eDIALOG_PASSWORD_DLG_BTN_CHGPASSWORD,		//	�н����� ���� ��ư

	StrToWzID("B005"),	//	eDIALOG_PASSWORD_DLG_BTN_BACKSPACE,			//	�齺���̽� ��ư
	StrToWzID("B009"),	//	eDIALOG_PASSWORD_DLG_BTN_CLEAR,				//	Ŭ���� ��ư

	StrToWzID("B015"),	//	eDIALOG_PASSWORD_DLG_BTN_OK,				//	Ȯ�� ��ư
	StrToWzID("B013"),	//	eDIALOG_PASSWORD_DLG_BTN_CANCLE,			//	��� ��ư

};

//------------------------------------------------------------------------------
/**
*/
uiPasswordDlg::uiPasswordDlg(InterfaceManager *pUIMan) :
	uiBase(pUIMan), m_bControlsInitialized(false)
{
	this->m_pPasswordMan =
		static_cast<uiPasswordMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (m_pPasswordMan);

	if (!this->m_pPasswordMan)
	{
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pPasswordMan�� �����ϴ�."));
#endif
		this->ShowInterface(FALSE);
		return;
	}

	m_iTooltipStringCode = 0;
	m_UseTooltipCtrls.clear();

	m_UseTooltipCtrls[eDIALOG_PASSWORD_DLG_BTN_CHGPASSWORD]	= 11193;
	m_UseTooltipCtrls[eDIALOG_PASSWORD_DLG_BTN_BACKSPACE]	= 11191;
	m_UseTooltipCtrls[eDIALOG_PASSWORD_DLG_BTN_CLEAR]		= 11192;

	ZeroMemory(m_strPassword, sizeof(m_strPassword));
	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiPasswordDlg::~uiPasswordDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/

int	uiPasswordDlg::getTooltipString(POSTYPE pos)
{
	map<int,int>::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);
	if (iter == m_UseTooltipCtrls.end())
		return 0;

	return iter->second;
}


//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_iCalledState = ePassWordCalledState_None;

	this->InitControls();
	
	m_randomizer.Create(ePASSWORD_NUM_DLG_NUM_MAX);
}


//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::InitControls()
{
	m_pTxtCtrlMessage = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_PASSWORD_DLG_TXT_MESSAGE));
	assert(m_pTxtCtrlMessage);

	m_pTxtCtrlPassword = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_PASSWORD_DLG_TXT));
	assert(m_pTxtCtrlPassword);

	for(int i = 0; i < ePASSWORD_NUM_DLG_NUM_MAX; ++i)
	{
		m_pBtnPasswordNum[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_PASSWORD_DLG_BTN_1 + i));
		assert(m_pBtnPasswordNum[i]);
	}

	m_pBtnPasswordChg = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_PASSWORD_DLG_BTN_CHGPASSWORD));
	assert(m_pBtnPasswordChg);

	this->m_bControlsInitialized = true;
}

void uiPasswordDlg::OnShowWindow( BOOL val )
{
	if (val)
	{
		this->MoveDefaultWindowPos();

		if (!this->m_bControlsInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_Password;
		msg.wParam = InterfaceManager::DIALOG_PASSWORD_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		_clearAllItem();
		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Password);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::updateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < eDIALOG_PASSWORD_DLG_MAX; a++)
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

	if(m_iTooltipStringCode > 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->RegisterTooltipForWareNInventoryLock(m_iTooltipStringCode);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::OnUpdateSolarDialog()
{
	updateToolTip();

	if(!m_bShow)
		this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0, };
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			int	iCtrlMenu = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( iCtrlMenu )
			{
			case eDIALOG_PASSWORD_DLG_BTN_EXIT:
			case eDIALOG_PASSWORD_DLG_BTN_CANCLE:
				{
					m_bShow = false;
					if (m_fnPassword)
						m_fnPassword(false);
				}
				break;
			case eDIALOG_PASSWORD_DLG_BTN_1:
			case eDIALOG_PASSWORD_DLG_BTN_2:
			case eDIALOG_PASSWORD_DLG_BTN_3:
			case eDIALOG_PASSWORD_DLG_BTN_4:
			case eDIALOG_PASSWORD_DLG_BTN_5:
			case eDIALOG_PASSWORD_DLG_BTN_6:
			case eDIALOG_PASSWORD_DLG_BTN_7:
			case eDIALOG_PASSWORD_DLG_BTN_8:
			case eDIALOG_PASSWORD_DLG_BTN_9:
			case eDIALOG_PASSWORD_DLG_BTN_0:
#ifdef _GS_ADD_PWD_X_BTN_1143
			case eDIALOG_PASSWORD_DLG_BTN_X:
#endif//_GS_ADD_PWD_X_BTN_1143(�빮��)
#ifdef _GS_ADD_PWD_X_BTN_1245
			case eDIALOG_PASSWORD_DLG_BTN_X2:
#endif//_GS_ADD_PWD_X_BTN_1245(�ҹ���)

				{
					TCHAR szPassword[3] = {0,};
					int iCurIndex = iCtrlMenu - eDIALOG_PASSWORD_DLG_BTN_1;

					m_pBtnPasswordNum[iCurIndex]->GetTextWZ(szPassword , 3);

					int strCount = _tcslen(m_strPassword);
					if(strCount < m_iPasswordMaxCount)
					{
						_tcscat(m_strPassword, szPassword);
						GlobalFunc::SetCaptionByPasswordType(m_pTxtCtrlPassword, m_strPassword);

					}
					else
					{
						uiSystemMan * pSystemMan = 
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							switch(m_iCalledState)
							{
							case ePassWordCalledState_Inven:
								{
									uiInventoryMan* pInvenMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );

									assert (pInvenMan);
									if(pInvenMan->GetPasswordState() == eInventoryDlgState_Setting_SSN 
										|| pInvenMan->GetPasswordState() ==  eInventoryDlgState_Check_Clear )
									{
										//	5586	�Է¹����� �ʰ��Ͽ����ϴ�. �ٽ� �Է����ֽñ� �ٶ��ϴ�.
										g_InterfaceManager.GetInterfaceString( 5586, szMessage, INTERFACE_STRING_LENGTH );
										pSystemMan->Sys_Verify(szMessage);
										return;
									}
								}
								break;

							case ePassWordCalledState_WareHouse:
								{
									uiBankMan* pBankMan =
										static_cast<uiBankMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BANK));
									assert (pBankMan);
									if(pBankMan->GetPasswordState() == eBankDlgState_Setting_SSN
										|| pBankMan->GetPasswordState() ==  eBankDlgState_Check_Clear )
									{
										//	5586	�Է¹����� �ʰ��Ͽ����ϴ�. �ٽ� �Է����ֽñ� �ٶ��ϴ�.
										g_InterfaceManager.GetInterfaceString( 5586, szMessage, INTERFACE_STRING_LENGTH );
										pSystemMan->Sys_Verify(szMessage);
										return;
									}
								}
								break;

							default:
								break;
							}
							// ��й�ȣ�� �ּ� 4�ڿ��� �ִ� 8�ڱ��� �����մϴ�.
							// ��й�ȣ�� �ּ� %d�ڿ��� �ִ� %d�ڱ��� �����մϴ�.
							g_InterfaceManager.GetInterfaceString( 5566, szMessage, INTERFACE_STRING_LENGTH );
							Sprintf(szTemp, szMessage, m_iPasswordMinCount, m_iPasswordMaxCount);
							pSystemMan->Sys_Verify(szTemp);
						}
					}

					_SetPasswordKey();

				}
				break;

			case eDIALOG_PASSWORD_DLG_BTN_CHGPASSWORD:
				{
					// 5552	���� ��ȣ ����� [��� �� ��]���·� �����Ͻðڽ��ϱ�?
					g_InterfaceManager.GetInterfaceString( 5552,  szMessage, INTERFACE_STRING_LENGTH );
					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
					{
						switch(m_iCalledState)
						{
						case ePassWordCalledState_Inven:
								pSystemMan->Sys_Confirm(szMessage ,GlobalFunc::_fn_uiInvenPassword_Clear);
							break;

						case ePassWordCalledState_WareHouse:
								pSystemMan->Sys_Confirm(szMessage , GlobalFunc::_fn_uiBankPassword_Clear);
							break;

						default:
							break;
						}
					}
				}
				break;


			case eDIALOG_PASSWORD_DLG_BTN_BACKSPACE:
				{
				
					int strCount = _tcslen(m_strPassword);
					if(strCount > 0)
					{
						m_strPassword[strCount-1] = '\0';
						GlobalFunc::SetCaptionByPasswordType(m_pTxtCtrlPassword, m_strPassword);
					}
				}
				break;

			case eDIALOG_PASSWORD_DLG_BTN_CLEAR:
				{
					ZeroMemory(m_strPassword, sizeof(m_strPassword));
					GlobalFunc::SetCaption(m_pTxtCtrlPassword, m_strPassword);
				}
				break;

			case eDIALOG_PASSWORD_DLG_BTN_OK:
				{
					int strCount = _tcslen(m_strPassword);
					if(strCount >= m_iPasswordMinCount && strCount <= m_iPasswordMaxCount)
					{
						m_bShow = false;
						if (m_fnPassword)
							m_fnPassword(true);
					}
					else
					{
						uiSystemMan * pSystemMan = 
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							// ��й�ȣ�� �ּ� %d�ڿ��� �ִ� %d�ڱ��� �����մϴ�.
							g_InterfaceManager.GetInterfaceString( 5566,  szMessage, INTERFACE_STRING_LENGTH );
							Sprintf(szTemp, szMessage, m_iPasswordMinCount, m_iPasswordMaxCount);
							pSystemMan->Sys_Verify(szTemp, NULL);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			m_iTooltipStringCode = getTooltipString(pos);
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiPasswordDlg::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::_clearAllItem()
{
	GlobalFunc::SetCaption(m_pTxtCtrlMessage, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlPassword, _T(""));

	ZeroMemory(m_strPassword, sizeof(m_strPassword));
	m_iPasswordMinCount = 0;
	m_iPasswordMaxCount = 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::SetPassword( TCHAR* szMessage, int iMinCount, int iMaxCount, BOOL bShowPassChgBtn, int iCalled )
{
	_SetPsaawordMinMaxCount( iMinCount, iMaxCount );
	_SetPasswordMessage( szMessage );
	_SetPasswordKey();
	_SetChgPassBtn( bShowPassChgBtn );
	m_iCalledState = iCalled;
}
//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::_SetPasswordMessage( TCHAR* szMessage )
{
	if(m_pTxtCtrlMessage)
	{
		GlobalFunc::SetCaption(m_pTxtCtrlMessage, szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::_SetChgPassBtn( BOOL bShowPassChgBtn )
{
	if(m_pBtnPasswordChg)
	{
		if(bShowPassChgBtn)
			m_pBtnPasswordChg->ShowWindowWZ(WZ_SHOW);
		else
			m_pBtnPasswordChg->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordDlg::_SetPasswordKey( )
{
	m_randomizer.RandomShuffule();

	for( int i = 0; i < ePASSWORD_NUM_DLG_NUM_MAX; ++i)
	{
		m_pBtnPasswordNum[i]->SetTextID(m_randomizer.m_vecRandom[i] + 1105);

#ifdef _GS_ADD_PWD_X_BTN_1143
		if(ePASSWORD_NUM_DLG_NUM_X==m_randomizer.m_vecRandom[i])
		{
			m_pBtnPasswordNum[i]->SetTextID(1135);//1135:X(�빮��)
		}
#endif//_GS_ADD_PWD_X_BTN_1143(�빮��)

#ifdef _GS_ADD_PWD_X_BTN_1245
		if(ePASSWORD_NUM_DLG_NUM_X2==m_randomizer.m_vecRandom[i])
		{
			m_pBtnPasswordNum[i]->SetTextID(1624);//1624:x(�ҹ���)
		}
#endif//_GS_ADD_PWD_X_BTN_1245(�ҹ���)
		
	}


}
//------------------------------------------------------------------------------

