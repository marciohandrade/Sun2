//------------------------------------------------------------------------------
//  InputNumTradeDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "inputnumtradedialog.h"
#include "uiTradeMan/uiTradeMan.h"
#include "hero.h"
#include "globalfunc.h"
#include "interfacemanager.h"
//------------------------------------------------------------------------------
/**
*/
InputNumTradeDialog::InputNumTradeDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
InputNumTradeDialog::~InputNumTradeDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumTradeDialog::Init( CDrawBase * pDrawBase )
{
    InputNumDialog::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumTradeDialog::Release()
{
    InputNumDialog::Release();
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumTradeDialog::OnShowWindow( BOOL val )
{
    InputNumDialog::OnShowWindow(val);

    if (val)
    {
        RECT rcDlgSize;
        this->GetDialogWZ()->GetDlgSize(&rcDlgSize);
        int x = 512 - (rcDlgSize.right / 2);
        MoveWindowWZ( (float)x, 15.0f );

        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_num_trade;
		msg.wParam=InterfaceManager::DIALOG_INPUT_NUM;
		msg.DoSomething=GlobalFunc::DoInputNumTradeBank;
		g_KeyQueueManager.PushBack(msg);
	
		// ��Ʈ UI ���� ���� �ڵ����
		//if(m_pCtrlEditInput)
		//	m_pCtrlEditInput->SetFontID(StrToWzID("mn12"));
    }
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_num_trade);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumTradeDialog::MoneyProcess()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    uiTradeMan* tradeMan =
        static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));

    if (tradeMan)
    {
        TCHAR	pszInputMoney[128] = {0,};

		// �ݾ��� �ִ� ������(128 byte)�� �Ѿ����ϴ�.
		if(!GlobalFunc::CompareLimitText(m_pCtrlEditInput, pszInputMoney, 128, 5794))
			return;

        if (lstrlen(pszInputMoney)==0)
        {
			//	�ݾ��� �Է����ּ���!!
			g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
        }
        else
        {
			TCHAR szTemp[128] = {0,};
			int iLen = strlen(pszInputMoney);
			int j = 0;
			for(int i = 0; i < iLen; ++i, ++j)
			{
				if(',' != pszInputMoney[j])
				{
					szTemp[i] = pszInputMoney[j];
				}
				else 
				{
					++j;
					szTemp[i] = pszInputMoney[j];
				}
			}
			if (this->checkDigit(szTemp))
			{
				MONEY cur_Money = (MONEY)_ttoi64(szTemp);
				MONEY bak_Money = tradeMan->GetTradeInfo1().m_DealMoney;

				if (g_pHero && (g_pHero->GetMoney() < cur_Money))
				{
					//	�Է� �Ͻ� �� �ִ� �ݾ��� ���� �����ݿ� ���� �˴ϴ�.
					const int	c_iInputMoney = 70385;
					g_InterfaceManager.GetInterfaceString( c_iInputMoney, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
					m_pCtrlEditInput->ClearText();
				}
				else
				{
					tradeMan->TEST_PutMoney(cur_Money);
					SolarDialog::ShowWindow(FALSE);
				}
			}
			else
			{
				//	�ݾ��� ���ڷθ� �Է��Ͻʽÿ�
				g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY_IN_FIGURES, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
				m_pCtrlEditInput->ClearText();
			}
        }
    }
    else
    {
        SolarDialog::ShowWindow(FALSE);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
InputNumTradeDialog::checkDigit(const TCHAR *str)
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
InputNumTradeDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            this->m_status = INPUTNUM_STATUS_OK;

            this->MoneyProcess();
        }
        break;

	case RT_MSG_LBUTTONCLICK:
        {
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case INPUTNUM_BTN_OK:
                {
                    this->m_status = INPUTNUM_STATUS_OK;

                    this->MoneyProcess();
                }
                break;

            case INPUTNUM_BTN_INIT:
                {
                    this->m_status = INPUTNUM_STATUS_INIT;
                    m_pCtrlEditInput->ClearText();
                }
                break;

            case INPUTNUM_BTN_CANCEL:
                {
					this->m_status = INPUTNUM_STATUS_CANCEL;
                    ShowWindow( FALSE );
                    
                }

            case INPUTNUM_BTN_EXIT:
                {
					Exit();
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

//------------------------------------------------------------------------------
/**
*/
void 
InputNumTradeDialog::Exit()
{
	this->m_status = INPUTNUM_STATUS_NORMAL;
	ShowWindow( FALSE );

}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
