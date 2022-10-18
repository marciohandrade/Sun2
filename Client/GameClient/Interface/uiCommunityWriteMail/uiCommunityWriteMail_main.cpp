#include "SunClientPrecompiledHeader.h"

#include "uiCommunityWriteMail.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "GameConst_Color.h"
#include "interfacemanager.h"

//------------------------------------------------------------------------------
WzID uiCommunityWriteMail::m_wzId[COMMUNITY_WRITE_MAIL_DIALOG_MAX] = 
{
        StrToWzID("BT01"),		// COMMUNITY_WRITE_MAIL_BTN_EXIT = 0,	 
		StrToWzID("BT00"),		// COMMUNITY_WRITE_MAIL_BTN_SEND,		 

		StrToWzID("ED01"),		// COMMUNITY_WRITE_MAIL_EDT_TO_USER,	 
		StrToWzID("ED00"),		// COMMUNITY_WRITE_MAIL_EDT_MSG,		 
};

//------------------------------------------------------------------------------
/**
*/
uiCommunityWriteMail::uiCommunityWriteMail(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pCommunityMan =
        static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
    assert (m_pCommunityMan);

    if (!this->m_pCommunityMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCommunityMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_bShow = false;

}

//------------------------------------------------------------------------------
/**
*/
uiCommunityWriteMail::~uiCommunityWriteMail()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityWriteMail::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityWriteMail::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityWriteMail::InitControls()
{
	for (int i=0; i<WRITE_MAIL_EDT_MAX; ++i)
    {
        m_pEditCtrls[i] = static_cast<CCtrlDxIMEEditBox *>(this->getControl(COMMUNITY_WRITE_MAIL_EDT_TO_USER + i));
        assert (this->m_pEditCtrls[i]);

		if(m_pEditCtrls[i])
		{
			m_pEditCtrls[i]->SetColorText( RGBA( 255, 255, 255, 255));
			m_pEditCtrls[i]->SetColorCaret(c_InterfaceCaretColor);
			m_pEditCtrls[i]->SetColorSelText( RGBA( 255, 255, 255, 255) );
#ifdef _INTERNATIONAL_UI
			m_pEditCtrls[i]->SetFontID(StrToWzID("st10"));
#else
			m_pEditCtrls[i]->SetFontID(c_wiCommunityFont);
#endif//_INTERNATIONAL_UI
		}
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCommunityWriteMail::OnShowWindow( BOOL val )
{
    if (val)
    {
        this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		if(!_isEnable())
			return;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_community_write_mail;
        msg.wParam = InterfaceManager::DIALOG_COMMUNITY_WRITE_MAIL;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		if(m_pEditCtrls[WRITE_MAIL_EDT_TO_USER])
		{
			m_pEditCtrls[WRITE_MAIL_EDT_TO_USER]->SetLimitText(MAX_CHARNAME_LENGTH);

			if(_tcslen(GetManager()->m_szRecvNameForMail) > 0)
			{
				m_pEditCtrls[WRITE_MAIL_EDT_TO_USER]->SetTextWZ(GetManager()->m_szRecvNameForMail);
				GlobalFunc::SetFocus( (SolarDialog *)this,  m_pEditCtrls[WRITE_MAIL_EDT_MSG] );
			}
			else
			{
				m_pEditCtrls[WRITE_MAIL_EDT_TO_USER]->ClearText();
				GlobalFunc::SetFocus( (SolarDialog *)this,  m_pEditCtrls[WRITE_MAIL_EDT_TO_USER] );
			}
		}

		if(m_pEditCtrls[WRITE_MAIL_EDT_MSG])
		{	
			m_pEditCtrls[WRITE_MAIL_EDT_MSG]->SetLimitText(MAX_MEMO_LENGTH);
			m_pEditCtrls[WRITE_MAIL_EDT_MSG]->ClearText();
			m_pEditCtrls[WRITE_MAIL_EDT_MSG]->SetHeight(22);
			m_pEditCtrls[WRITE_MAIL_EDT_MSG]->SetLimitLine(10);
		}

		m_bShow = true;
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_community_write_mail);

		m_bShow = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityWriteMail::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
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
void uiCommunityWriteMail::_clickSend()
{
	if(!m_pEditCtrls[WRITE_MAIL_EDT_TO_USER] || !m_pEditCtrls[WRITE_MAIL_EDT_MSG])
		return;

	if(!g_pHero)
		return;

	TCHAR szName[MAX_CHARNAME_LENGTH+1] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	WCHAR wszMemo[INTERFACE_STRING_LENGTH] = {0,};

	m_pEditCtrls[WRITE_MAIL_EDT_TO_USER]->GetTextWZ(szName, MAX_CHARNAME_LENGTH);
	m_pEditCtrls[WRITE_MAIL_EDT_MSG]->GetTextWZW(wszMemo, INTERFACE_STRING_LENGTH);

	TCHAR szMemo[INTERFACE_STRING_LENGTH] = {0,};
	GlobalFunc::ChangeMultiEditInput(wszMemo, szMemo);

	if(_tcslen(szName) == 0)
	{
		//5787	쪽지를 받을 캐릭터 명을 입력하세요
		g_InterfaceManager.GetInterfaceString( 5787	 , szMessage ,INTERFACE_STRING_LENGTH);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage);

		return;
	}

	if(Stricmp(szName, g_pHero->GetName()) == 0)
	{
		// 5795	자기 자신에게는 보낼 수 없습니다.
		g_InterfaceManager.GetInterfaceString( 5795 , szMessage ,INTERFACE_STRING_LENGTH);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage);

		return;
	}

	if(_tcslen(szMemo) == 0)
	{
		// 555	메시지를 입력 하십시오.
		g_InterfaceManager.GetInterfaceString( 555, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage);

		return;
	}

	// 쪽지내용이 최대 사이즈를 넘었다
	if(_tcslen(szMemo) > MAX_MEMO_LENGTH)
	{
		//5788	입력할 수 있는 쪽지 내용의 최대 글자 수를 초과하였습니다.(한글 100자, 숫자-영문 100자)
		g_InterfaceManager.GetInterfaceString( 5788 , szMessage ,INTERFACE_STRING_LENGTH);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage);

		return;
	}

	// 금지단어 check
	if(GlobalFunc::IsSlang(szMemo, false) == TRUE)
	{
		//	사용 금지 단어입니다
		g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage);

		GlobalFunc::SetFocus( (SolarDialog *)this,  m_pEditCtrls[WRITE_MAIL_EDT_MSG] );
		return;
	}

#ifdef _SCJ_MAKE_DOUBLE_QUOTATION_MARKS
    MakeDoubleQuotationMarks(szMemo);
#endif //_SCJ_MAKE_DOUBLE_QUOTATION_MARKS

	if(GetManager())
		GetManager()->SEND_CW_MEMO_SEND_SYN(szName, szMemo);
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _SCJ_MAKE_DOUBLE_QUOTATION_MARKS
void uiCommunityWriteMail::MakeDoubleQuotationMarks(IN OUT TCHAR* message_pointer)
{
    if (message_pointer == NULL)
    {
        return;
    }

    // 따옴표에 따옴표 하나 더 붙이기  (' -> '')
    TCHAR temp_message[MAX_MEMO_LENGTH] = {0,};
    int replace_position = 0;
    int desc_position = 0;

    while(message_pointer[replace_position] != '\0')
    {
        if (message_pointer[replace_position] == '\'')
        {
            temp_message[desc_position] = message_pointer[replace_position];
            temp_message[++desc_position] = message_pointer[replace_position];

        }
        else
        {
           temp_message[desc_position] = message_pointer[replace_position];
        }
        ++replace_position;
        ++desc_position;

        if (desc_position > MAX_MEMO_LENGTH - 3)
        {
            temp_message[MAX_MEMO_LENGTH-1] = '\0';
            break;
        }
    }

    StrnCopy(message_pointer, temp_message, MAX_MEMO_LENGTH);

}
#endif //_SCJ_MAKE_DOUBLE_QUOTATION_MARKS

//------------------------------------------------------------------------------
/**
*/
void uiCommunityWriteMail::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case COMMUNITY_WRITE_MAIL_BTN_EXIT:
                {
                    m_bShow = false;
                }
                break;

			case COMMUNITY_WRITE_MAIL_BTN_SEND:
				{
					_clickSend();
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
void uiCommunityWriteMail::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		this->ShowInterface(FALSE);
		return;
	}

    if (!_isEnable())
        return;
}

