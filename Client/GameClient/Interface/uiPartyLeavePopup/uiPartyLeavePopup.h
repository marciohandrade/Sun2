#ifndef     __USER_INTERFACE_PARTYLEAVEPOPUP_H__
#define     __USER_INTERFACE_PARTYLEAVEPOPUP_H__
//------------------------------------------------------------------------------
/**
    @class uiPartyLeavePopup

    interface for a party leave popup.

    (C) 2005 ckbang
*/
#include "uiBase.h"

class uiPartyLeavePopup :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_OK = 0,
        DLGPOS_CANCEL,
        DLGPOS_MSG,

        DIALOG_MAX,
    };

public:
    uiPartyLeavePopup(InterfaceManager *pUIMan);
    virtual ~uiPartyLeavePopup();

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
    ASK_STATUS              GetStatus();
    void                    SetInviterName (const char* str);
    void                    SetInviterKey (DWORD dwObjKey);
	virtual bool            ShowInterface( BOOL val, BOOL showOff=FALSE );
    //--------------------------------------------------------------------------
    // functions
    //
public:
    virtual VOID            Process(DWORD dwTick);

	void                    LeaveParty( void );


private:
    void                    response_Yes();
    void                    response_No();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void                    Exit();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * /*pMsg*/ ) {};         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    ASK_STATUS              m_status;

    BOOL                    bDestroy;                               // ≈∏¿Ã∏”
    BOOL                    bDestroyTimer;                          //
    DWORD                   dwDestroyTick;                          //
    DWORD                   dwDestroyTickOver;                      //
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPartyLeavePopup::getCtrlIDToPos( WzID wzId )
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
uiPartyLeavePopup::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiPartyLeavePopup::ASK_STATUS
uiPartyLeavePopup::GetStatus()
{
    return m_status;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_PARTYLEAVEPOPUP_H__
