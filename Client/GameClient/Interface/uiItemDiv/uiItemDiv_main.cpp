//------------------------------------------------------------------------------
//  uiItemDiv.cpp
//  (C) 2006 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "MouseHandler.h"
#include "uiItemDiv.h"
#include "uiItemDivMan/uiItemDivMan.h"
#include "InventoryDialog.h"
#include "globalfunc.h"
#include "interfacemanager.h"
//------------------------------------------------------------------------------
WzID uiItemDiv::m_wzId[DIALOG_MAX] = 
{ 
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    StrToWzID("TI00"),  // DLGPOS_BTN_OK
    StrToWzID("TI01"),  // DLGPOS_BTN_CANCEL
    StrToWzID("B005"),  // DLGPOS_BTN_L_DEC
    StrToWzID("B001"),  // DLGPOS_BTN_R_INC
    StrToWzID("E000"),  // DLGPOS_EDT_NUM
    StrToWzID("B004"),  // DLGPOS_BTN_CLOSE
#else
    StrToWzID("TI00"),  // DLGPOS_BTN_OK
    StrToWzID("TI01"),  // DLGPOS_BTN_CANCEL
    StrToWzID("B001"),  // DLGPOS_BTN_L_DEC
    StrToWzID("B002"),  // DLGPOS_BTN_R_INC
    StrToWzID("E000"),  // DLGPOS_EDT_NUM
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
};

//------------------------------------------------------------------------------
/**
*/
uiItemDiv::uiItemDiv(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiItemDiv::~uiItemDiv()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::InitControls()
{
    m_pEdtControl = static_cast<CCtrlDxIMEEditBox *>(this->getControl(DLGPOS_EDT_NUM));
    assert (m_pEdtControl);

    m_pBtnControls[0] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_L_DEC));
    assert (m_pBtnControls[0]);
    m_pBtnControls[1] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_R_INC));
    assert (m_pBtnControls[1]);

    m_pBtnOk = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_OK));
    assert (m_pBtnOk);

    m_pBtnCancel = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_CANCEL));
    assert (m_pBtnCancel);

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::Release()
{
}


//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiItemDiv::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        //UpdatePosition();

        //SolarDialog::MoveCenter();

        m_pEdtControl->ClearText();
        m_pEdtControl->SetFontID(StrToWzID("mn12"));
        m_pEdtControl->SetFocusWZ();

        TCHAR   szMessage[INTERFACE_STRING_LENGTH];
        Sprintf (szMessage, _T("%d"), m_divNum);

        GlobalFunc::SetText(m_pEdtControl, szMessage);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_item_div;
        msg.wParam = InterfaceManager::DIALOG_ITEM_DIV;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		this->GetUIMan()->AttachFirstDialog(InterfaceManager::DIALOG_ITEM_DIV);
    }
    else
    {
		this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_ITEM_DIV);
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_item_div);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
_checkDigit(const TCHAR *str)
{
    int len = lstrlen(str);
    int i;

    for (i=0; i<len; ++i)
    {
        if (str[i]<_T('0') || str[i]>_T('9'))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_EDT_NUM:
                {
                    OnOkBtnClick();
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case DLGPOS_BTN_OK:
				{
                    OnOkBtnClick();
				}
				break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
            case DLGPOS_BTN_CLOSE:
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
            case DLGPOS_BTN_CANCEL:
                {
                    ShowInterface(FALSE);
                }
                break;

            case DLGPOS_BTN_L_DEC:
                {
                    OnLDecBtnClick();
                }
                break;

            case DLGPOS_BTN_R_INC:
                {
                    OnRIncBtnClick();
                }
                break;
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

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiItemDiv::UpdatePosition()
//{
//    MoveWindowWZ(733, 289);
//}


//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::OnOkBtnClick()
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH];
    TCHAR   szTemp[INTERFACE_STRING_LENGTH];

    GlobalFunc::GetText(m_pEdtControl, szTemp, INTERFACE_STRING_LENGTH);

    if (lstrlen(szTemp)==0)
    {
		// [알림] 분할하실 개수를 입력하세요 5127
		g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_NUM, szMessage ,INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
    else
    {
        if (_checkDigit(szTemp))
        {
            int aNum = (int)_ttoi64(szTemp);

            if( aNum < 1 || aNum > m_divNum )
            {
		        // 잘못된 정보입니다. 5346 (현재 있는 메시지 중에 그나마)
                // 개수를 다시 입력하세요 등의 메시지를 뿌리는게 더 적당
		        g_InterfaceManager.GetInterfaceString(5346, szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                Sprintf (szTemp, _T("%d"), m_divNum);
                GlobalFunc::SetText(m_pEdtControl, szTemp);

                return;
            }

            MouseHandler::Instance()->AddItemAtHandWithItemDiv(aNum,
                m_fromSlotIdx,
                m_fromPos,
                m_slotType,
                m_code,
                m_pRenderUnit,
                &m_Rect);
        }
        else
        {
			// [알림] 분할하실 개수를 숫자로 입력해 주세요 5128
			g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_ONLY_NUMBER , szMessage ,INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::OnLDecBtnClick()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];
    TCHAR   szTemp[INTERFACE_STRING_LENGTH];

    GlobalFunc::GetText(m_pEdtControl, szTemp, INTERFACE_STRING_LENGTH);

    if (lstrlen(szTemp)==0)
    {
		// [알림] 분할하실 개수를 입력하세요 5127
		g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_NUM, szMessage ,INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
    else
    {
        if (_checkDigit(szTemp))
        {
            int aNum = (int)_ttoi64(szTemp);

            --aNum;

            if( aNum > m_divNum )
            {
                aNum = m_divNum;
            }
            else if( aNum < 1 )
            {
                aNum = 1;
            }

            Sprintf (szTemp, _T("%d"), aNum);
            GlobalFunc::SetText(m_pEdtControl, szTemp);
        }
        else
        {
			// [알림] 분할하실 개수를 숫자로 입력해 주세요 5128
			g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_ONLY_NUMBER , szMessage ,INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDiv::OnRIncBtnClick()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];
    TCHAR   szTemp[INTERFACE_STRING_LENGTH];

    GlobalFunc::GetText(m_pEdtControl, szTemp, INTERFACE_STRING_LENGTH);

    if (lstrlen(szTemp)==0)
    {
		// [알림] 분할하실 개수를 입력하세요 5127
		g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_NUM, szMessage ,INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
    else
    {
        if (_checkDigit(szTemp))
        {
            int aNum = (int)_ttoi64(szTemp);

            ++aNum;

            if( aNum > m_divNum )
            {
                aNum = m_divNum;
            }
            else if( aNum < 1 )
            {
                aNum = 1;
            }

            Sprintf (szTemp, _T("%d"), aNum);
            GlobalFunc::SetText(m_pEdtControl, szTemp);
        }
        else
        {
			// [알림] 분할하실 개수를 숫자로 입력해 주세요 5128
			g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_ONLY_NUMBER , szMessage ,INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
