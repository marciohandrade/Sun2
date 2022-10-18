#pragma once

#include "uibase.h"

class uiLoginMan;
class uiLoginOneTimePassword :  public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_EDT_OTP,
        DLGPOS_BTN_CONNECT,

        DIALOG_MAX,
    };
    enum
    {
        MAX_OTP_LENGTH = 6,
    };

public:
    enum eOneTimePassword_Stamp
    {
        NoneStamp,
        UseNotOneTimePassword,
        UseOneTimePassword,
        UseNPassOneTimePassword,
        UseNRetryOneTimePassword,
        UseNExpiredOneTimePassword,
        UseNFailOneTimePassword,
        Auth_NonOneTimePasswordFlow,
        ReAuth_NonOneTimePasswordFlow,
    };

    uiLoginOneTimePassword(InterfaceManager *pUIMan);
    virtual ~uiLoginOneTimePassword();

public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    virtual void	        MessageProc( SI_MESSAGE * pMessage );

    void                    SetOTPUserStamp(eOneTimePassword_Stamp dwStamp) {   m_dwOTPStamp = dwStamp; }
    DWORD                   GetOTPUserStamp()   {   return m_dwOTPStamp; }

protected:
    virtual void            OnShowWindow(BOOL val);
    virtual void            OnUpdateSolarDialog();

    void                    SendOneTimePassword();

protected:
    static WzID		        m_wzId[DIALOG_MAX];

protected:
    CCtrlDxIMEEditBox*      m_pEdtControl;
    CCtrlButtonWZ*          m_pBtnControl;

    //
    eOneTimePassword_Stamp  m_dwOTPStamp;
    TCHAR                   m_szOTPInput[MAX_OTP_LENGTH+1];
};
