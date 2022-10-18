#ifndef     __USER_INTERFACE_VENDORPOPUP_H__
#define     __USER_INTERFACE_VENDORPOPUP_H__
//------------------------------------------------------------------------------
/**
    @class uiVendorPopup

    personal vendor sell interface

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiVendorPopup_def.h"
#include "SCItemSlot.h"

class uiVendorPopup :
    public uiBase
{
    enum eDIALOG_POS
    {
        DIALOG_TXT_ORG_VALUE=0,
        DIALOG_TXT_AFTER_VALUE,
        DIALOG_INPUT_ORG_VALUE,
        DIALOG_EDIT_AFTER_VALUE,
        DIALOG_OK,
        DIALOG_CANCEL,

        DIALOG_MAX,
    };

public:
    uiVendorPopup(InterfaceManager *pUIMan);
    virtual ~uiVendorPopup();

    enum ASK_STATUS
    {
        ASK_STATUS_NORMAL=0,
        ASK_STATUS_OK,
        ASK_STATUS_CANCEL,
    };

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    ASK_STATUS      GetStatus();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    SetInfos(SLOTIDX fromSlotIdx,
                                     SLOTIDX toSlotIdx,
                                     POSTYPE fromPos,
                                     POSTYPE toPos);
    void                    SetItemSlot(SCItemSlot* pItemSlot);
    void                    SetUseModify();
    void                    SetRequiredQuery();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void                    Exit();
    void                    SetFocus();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    bool                    checkDigit(const TCHAR* str);
    bool                    checkLimit(MONEY money);

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    bool                    m_bModify;
    bool                    m_bRequiredQuery;
    bool                    m_bFocus;
    ASK_STATUS              m_status;

    SCItemSlot*             m_rItemSlot;
    SLOTIDX                 m_FromSlotIdx;
    SLOTIDX                 m_ToSlotIdx;
    POSTYPE                 m_FromPos;
    POSTYPE                 m_ToPos;

	TCHAR			m_szMoneyOld[INTERFACE_STRING_LENGTH + 1];
	TCHAR			m_szMoneyNew[INTERFACE_STRING_LENGTH + 1];
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiVendorPopup::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiVendorPopup::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiVendorPopup::ASK_STATUS
uiVendorPopup::GetStatus()
{
    return m_status;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiVendorPopup::SetInfos(SLOTIDX fromSlotIdx,
                        SLOTIDX toSlotIdx,
                        POSTYPE fromPos,
                        POSTYPE toPos)
{
    this->m_FromSlotIdx = fromSlotIdx;
    this->m_ToSlotIdx = toSlotIdx;
    this->m_FromPos = fromPos;
    this->m_ToPos = toPos;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiVendorPopup::SetItemSlot(SCItemSlot* pItemSlot)
{
    this->m_rItemSlot = pItemSlot;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiVendorPopup::SetUseModify()
{
    this->m_bModify = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiVendorPopup::SetRequiredQuery()
{
    this->m_bRequiredQuery = true;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_VENDORPOPUP_H__
