#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCommunityWriteMail class
//------------------------------------------------------------------------------

#include "uibase.h"

class uiCommunityMan;

class uiCommunityWriteMail: public uiBase
{
    enum COMMUNITY_WRITE_MAIL_DIALOG_POS
    {
        COMMUNITY_WRITE_MAIL_BTN_EXIT = 0,	// BT01
		COMMUNITY_WRITE_MAIL_BTN_SEND,		// BT00

		COMMUNITY_WRITE_MAIL_EDT_TO_USER,	// ED01
		COMMUNITY_WRITE_MAIL_EDT_MSG,		// ED00

        COMMUNITY_WRITE_MAIL_DIALOG_MAX,
    };

	enum
	{
		WRITE_MAIL_EDT_TO_USER = 0,	
		WRITE_MAIL_EDT_MSG,	

		WRITE_MAIL_EDT_MAX,
	};

public:
    uiCommunityWriteMail(InterfaceManager *pUIMan);
    virtual ~uiCommunityWriteMail();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiCommunityMan*         GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
private:
	void					_clickSend();
#ifdef _SCJ_MAKE_DOUBLE_QUOTATION_MARKS
    void                    MakeDoubleQuotationMarks(IN OUT TCHAR* message_pointer);
#endif //_SCJ_MAKE_DOUBLE_QUOTATION_MARKS
public:
	


protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[COMMUNITY_WRITE_MAIL_DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiCommunityMan*			m_pCommunityMan;

    bool                    m_bControlsInitialized;

	CCtrlDxIMEEditBox*		m_pEditCtrls[WRITE_MAIL_EDT_MAX];

	bool					m_bShow;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCommunityWriteMail::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < COMMUNITY_WRITE_MAIL_DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return COMMUNITY_WRITE_MAIL_DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCommunityWriteMail::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < COMMUNITY_WRITE_MAIL_DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCommunityMan*
uiCommunityWriteMail::GetManager()
{
    return this->m_pCommunityMan;
}

//------------------------------------------------------------------------------

