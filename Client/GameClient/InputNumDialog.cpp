//------------------------------------------------------------------------------
//  InputNumDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "inputnumdialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
// RC Info
const DWORD c_dwRCID = StrToWzID("0840");
const char c_strRCNAME[] = "Data\\Interface\\84_Input_Num.iwz";

const BYTE c_btMaxEditNum = 19;

//------------------------------------------------------------------------------
WzID InputNumDialog::m_wzId[Dialog_MAX] =
{
        StrToWzID("B999"),  // INPUTNUM_BTN_EXIT,      // 닫기
        StrToWzID("E000"),  // INPUTNUM_EDIT_MONEY,    // 돈입력
        StrToWzID("TI00"),  // INPUTNUM_BTN_OK,        // 확인
        StrToWzID("TI01"),  // INPUTNUM_BTN_INIT,      // 초기화
        StrToWzID("TI02"),  // INPUTNUM_BTN_CANCEL,    // 취소
};

//------------------------------------------------------------------------------
/**
*/
InputNumDialog::InputNumDialog(void)
{
    this->m_dwRCID = c_dwRCID;
    this->m_strRCNAME = c_strRCNAME;
}

//------------------------------------------------------------------------------
/**
*/
InputNumDialog::~InputNumDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::Init( CDrawBase * pDrawBase )
{

	// 초기화 해야할 변수들
	this->m_status = INPUTNUM_STATUS_NORMAL;	
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            this->m_status = INPUTNUM_STATUS_OK;
            SolarDialog::ShowWindow(FALSE);
        }
        break;

	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
	        case INPUTNUM_BTN_OK:
                {
                    this->m_status = INPUTNUM_STATUS_OK;
                    SolarDialog::ShowWindow(FALSE);
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
					Exit();
				}
				break;

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
InputNumDialog::Exit()
{
	this->m_status = INPUTNUM_STATUS_CANCEL;
	ShowWindow( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::NetworkProc( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ *
InputNumDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < Dialog_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE
InputNumDialog::getCtrlIDToPos( WzID wzId )
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

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::OnShowWindow( BOOL val )
{
	if (val)
    {
		// 이용할 컨트롤 들 받아놓는다.
		m_pCtrlEditInput = (CCtrlDxIMEEditBox *)getControl(INPUTNUM_EDIT_MONEY);
		assert(m_pCtrlEditInput);
		GlobalFunc::SetFocus( (SolarDialog *)this,  m_pCtrlEditInput );

        this->m_status = INPUTNUM_STATUS_NORMAL;
	
    }
    else
    {
        this->m_status = INPUTNUM_STATUS_CANCEL;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InputNumDialog::OnUpdateSolarDialog()
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};

	m_pCtrlEditInput->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
	int iLen = strlen(m_szMoneyOld);
	int j = 0;
	for(int i = 0; i < iLen; ++i, ++j)
	{
		if(',' != m_szMoneyOld[j])
		{
			szTemp[i] = m_szMoneyOld[j];
		}
		else 
		{
			++j;
			szTemp[i] = m_szMoneyOld[j];
		}
	}
	if(isdigit(szTemp[0]))
	{
		if(c_btMaxEditNum > strlen(szTemp)) //에디트 컨트롤 범위 넘지 않게
		{
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szTemp, &nFmt, m_szMoneyNew, sizeof(m_szMoneyNew));
			m_pCtrlEditInput->SetTextWZ(m_szMoneyNew);
		}
		else
		{
			m_pCtrlEditInput->SetTextWZ("");
		}
	}
	else
	{
		m_pCtrlEditInput->SetTextWZ("");
	}

    if (this->GetStatus() == INPUTNUM_STATUS_CANCEL)
    {
        SolarDialog::ShowWindow(FALSE);
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
