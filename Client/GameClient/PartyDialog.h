#ifndef		_PARTYDIALOG_H
#define		_PARTYDIALOG_H
//------------------------------------------------------------------------------
/**
    @class PartyDialog

    파티 클래스
    파티 초대, 탈퇴, 강제 탈퇴 등을 처리하는 클래스

    (C) 2005 ckbang
*/
#include "solardialog.h"

class PartyDialog : public SolarDialog
{
    enum eDIALOG_POS
    {	
        DIALOG_GAUGE = 0,
        DIALOG_B000,
        DIALOG_B999,
        DIALOG_T000,
        DIALOG_T001,
        DIALOG_MAX,
    };	
public:
    PartyDialog();
    virtual ~PartyDialog();

    VOID			Init( CDrawBase * pDrawBase );
    virtual VOID	MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	NetworkProc( MSG_BASE * pMsg ){}

    void            Clear ();
    void            SetObjKey (DWORD objKey);
    DWORD           GetObjKey ();
    void            SetIdx (int idx);
    INT             GetIdx ();
    void            SubMenuTransaction();
    VOID            SetHP( float fHPPercent );

protected:
    virtual VOID	showWindow( BOOL val );

    VOID			SetObjectInfo();

protected:
    virtual VOID    update();
    CControlWZ *	getControl( POSTYPE AtPos );
    POSTYPE			getCtrlIDToPos( WzID wzId );
    static WzID		m_wzId[DIALOG_MAX];

    /// 소유자 오브젝트 키
    DWORD           m_dwObjKey;
    /// 파티 인터페이스의 오름차순 위치
    INT             m_iIdx;

    float			m_fGaugeSize;
    RECT            m_initGaugeRC;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyDialog::Clear ()
{
    this->m_dwObjKey = 0;
    this->m_iIdx = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyDialog::SetObjKey (DWORD objKey)
{
    this->m_dwObjKey = objKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
PartyDialog::GetObjKey ()
{
    return this->m_dwObjKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyDialog::SetIdx (int idx)
{
    m_iIdx = idx;
}

//------------------------------------------------------------------------------
/**
*/
inline
INT
PartyDialog::GetIdx ()
{
    return this->m_iIdx;
}

//------------------------------------------------------------------------------
#endif	//	_PARTYDIALOG_H
