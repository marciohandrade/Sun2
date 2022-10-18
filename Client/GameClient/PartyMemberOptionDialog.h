#ifndef		_PARTYMEMBEROPTIONDIALOG_H
#define		_PARTYMEMBEROPTIONDIALOG_H
//------------------------------------------------------------------------------
/**
    @class PartyMemberOptionDialog

    파티 클래스
    파티 초대, 탈퇴, 강제 탈퇴 등을 처리하는 클래스

    (C) 2005 ckbang
*/
#include "solardialog.h"

class PartyMemberOptionDialog : public SolarDialog
{
    enum eDIALOG_POS
    {	
        DIALOG_EXIT = 0,
        DIALOG_INVITE,
        DIALOG_LEAVE,
        DIALOG_FORCE_LEAVE,
        DIALOG_PARTY_DESTROY,
        DIALOG_MIGRATION_LEADER,
        DIALOG_ITEM_SHARE_SETTING,
        DIALOG_BUFF_INFO,
        DIALOG_MAX,
    };	
public:
    PartyMemberOptionDialog();
    virtual ~PartyMemberOptionDialog();

    VOID			Init( CDrawBase * pDrawBase );
    virtual VOID	MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	NetworkProc( MSG_BASE * pMsg ){}

    void            Clear ();
    void            SetObjKey (DWORD objKey);
    DWORD           GetObjKey ();
    void            SetIdx (int idx);
    INT             GetIdx ();

protected:
    void            showWindow( BOOL val );

protected:
    CControlWZ *	getControl( POSTYPE AtPos );
    POSTYPE			getCtrlIDToPos( WzID wzId );
    static WzID		m_wzId[DIALOG_MAX];

    /// 소유자 오브젝트 키
    DWORD           m_dwObjKey;
    /// 파티 인터페이스의 오름차순 위치
    INT             m_iIdx;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyMemberOptionDialog::Clear ()
{
    this->m_dwObjKey = 0;
    this->m_iIdx = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyMemberOptionDialog::SetObjKey (DWORD objKey)
{
    this->m_dwObjKey = objKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
PartyMemberOptionDialog::GetObjKey ()
{
    return this->m_dwObjKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PartyMemberOptionDialog::SetIdx (int idx)
{
    m_iIdx = idx;
}

//------------------------------------------------------------------------------
/**
*/
inline
INT
PartyMemberOptionDialog::GetIdx ()
{
    return this->m_iIdx;
}

//------------------------------------------------------------------------------
#endif	//	_PARTYMEMBEROPTIONDIALOG_H
