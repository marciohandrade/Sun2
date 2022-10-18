//------------------------------------------------------------------------------
//  uiLoginAccount_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginAccount.h"
#include "uiLoginMan/uiLoginMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
uiLoginAccount::uiLoginAccount(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bInitControls(false),
	hide_check_mouse_(false)
{
    m_pBtnControl = NULL;
    m_pEdtControls[0] = NULL;
    m_pEdtControls[1] = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiLoginAccount::~uiLoginAccount()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::InitControls()
{
    m_pBtnControl =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_CONNECT));
    assert (m_pBtnControl);

    int i;

    for (i=0; i<POS_EDT_SIZE; ++i)
    {
        m_pEdtControls[i] =
            static_cast<CCtrlDxIMEEditBox *>(this->getControl(DLGPOS_EDT_ID + i));
        assert (m_pEdtControls[i]);
    }

    GlobalFunc::SetFocus( (SolarDialog *)this,  m_pEdtControls[0] );

    m_pEdtControls[0]->SetLimitText( MAX_EDIT_IDLEN );
    m_pEdtControls[0]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[0]->SetColorCaret(RGBA(255,100,100,255));
    m_pEdtControls[0]->SetColorText(RGBA(255,255,255,255));
    m_pEdtControls[0]->ClearText();

	//FIXME: 홈페이지에서 10글자를 사용하므로 입력제한만 걸어두고
	//서버와는 PASSWD_MAX_LEN를 사용

	m_pEdtControls[1]->SetLimitText( MAX_EDIT_PASSLEN );
    m_pEdtControls[1]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[1]->SetColorCaret(RGBA(255,100,100,255));
    m_pEdtControls[1]->SetColorText(RGBA(255,255,255,255));
    m_pEdtControls[1]->ClearText();

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::Release()
{
}

void uiLoginAccount::hideCheckonShow( bool hide )
{
	hide_check_mouse_ = hide;
	m_fTrans = 0.0f;
	m_dwTransAcc = 0;
	m_dwTransEnd =(SEC_PER_TICK) * 1;
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
