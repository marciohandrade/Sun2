#ifndef     _INPUTNUMDIALOG_H
#define     _INPUTNUMDIALOG_H
//------------------------------------------------------------------------------
/**
    @class InputNumDialog

    머니 입력창 클래스

    (C) 2005 ckbang
*/
#include "solardialog.h"

class InputNumDialog : public SolarDialog
{
protected:
    enum INPUTNUM_POS
    {
        INPUTNUM_BTN_EXIT=0,    // 닫기
        INPUTNUM_EDIT_MONEY,    // 돈입력
        INPUTNUM_BTN_OK,        // 확인
        INPUTNUM_BTN_INIT,      // 초기화
        INPUTNUM_BTN_CANCEL,    // 취소

        Dialog_MAX
    };

    enum INPUTNUM_STATUS
    {
        INPUTNUM_STATUS_NORMAL=0,
        INPUTNUM_STATUS_OK,
        INPUTNUM_STATUS_INIT,
        INPUTNUM_STATUS_CANCEL,
    };

public:
    InputNumDialog(void);
    virtual ~InputNumDialog(void);

    virtual void    Init( CDrawBase * pDrawBase );
    virtual void    Release();

public:
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual void    NetworkProc( MSG_BASE * pMsg );

    INPUTNUM_STATUS GetStatus();
    void            SetStatus(INPUTNUM_STATUS eStatus);
	void            Exit();

    // resource infos
    DWORD           rcinfo_GetID();
    const char *    rcinfo_GetPath();

protected:  // dialog common control
    CControlWZ *    getControl( POSTYPE AtPos );
    POSTYPE         getCtrlIDToPos( WzID wzId );

    virtual void    OnUpdateSolarDialog();
    virtual VOID    OnShowWindow( BOOL val );

    // 컨트롤 
    CCtrlDxIMEEditBox *    m_pCtrlEditInput;

    INPUTNUM_STATUS m_status;
    static WzID     m_wzId[Dialog_MAX];

protected:  // resource infos
    DWORD           m_dwRCID;
    const char *    m_strRCNAME;

	TCHAR			m_szMoneyOld[INTERFACE_STRING_LENGTH + 1];
	TCHAR			m_szMoneyNew[INTERFACE_STRING_LENGTH + 1];
};

//------------------------------------------------------------------------------
/**
*/
inline
InputNumDialog::INPUTNUM_STATUS
InputNumDialog::GetStatus()
{
    return m_status;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
InputNumDialog::SetStatus(INPUTNUM_STATUS eStatus)
{
    m_status = eStatus;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
InputNumDialog::rcinfo_GetID()
{
    return this->m_dwRCID;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char *
InputNumDialog::rcinfo_GetPath()
{
    return this->m_strRCNAME;
}

//------------------------------------------------------------------------------
#endif  //  _INPUTNUMDIALOG_H
