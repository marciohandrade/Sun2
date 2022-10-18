#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCommunityReadMail class
//------------------------------------------------------------------------------

#include "uibase.h"

#define SHOW_DEFAULT_READ_MAIL_LINE		3

struct MAIL_DATA;

class uiCommunityMan;

class uiCommunityReadMail: public uiBase
{
    enum COMMUNITY_READ_MAIL_DIALOG_POS
    {
        COMMUNITY_READ_MAIL_BTN_EXIT = 0,	

        COMMUNITY_READ_MAIL_BTN_REPLY,	
		COMMUNITY_READ_MAIL_BTN_DEL,	
		COMMUNITY_READ_MAIL_BTN_CLOSE,	

		COMMUNITY_READ_MAIL_TXT_FROM,	
		COMMUNITY_READ_MAIL_TXT_DATE,	

		COMMUNITY_READ_MAIL_LST,
		COMMUNITY_READ_MAIL_VSCROLL,

        COMMUNITY_READ_MAIL_DIALOG__MAX,
    };

	enum
	{
		READ_MAIL_TXT_FROM = 0,	
		READ_MAIL_TXT_DATE,	

		READ_MAIL_TXT_MAX,
	};


public:
    uiCommunityReadMail(InterfaceManager *pUIMan);
    virtual ~uiCommunityReadMail();

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
	void					_refreshList();
	void					_displaySetting();

public:
	BOOL					SetMailData(MAIL_DATA* pData);
	void					CloseAnswerBtn(BOOL bClose);
    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void					CloseDlg();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[COMMUNITY_READ_MAIL_DIALOG__MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //

private:
	void					UpdateMouseWheel();
	void					DownScroll(int nRange);
	void					UpScroll(int nRange);

private:
    uiCommunityMan*			m_pCommunityMan;

	bool                    m_bControlsInitialized;

	CCtrlStaticWZ*			m_pTxtCtrls[READ_MAIL_TXT_MAX];
	CCtrlListWZ*			m_pListCtrlRead;
	CCtrlVScrollWZ*			m_pScroll;

	bool					m_bShow;
	int						m_LinePerPage;
public:
	std::vector<util::_tstring> m_vstr;
	STRUCT_SUBJECT_PACKETINFO	m_subject;
	TCHAR						m_ptszMemo[MAX_MEMO_STRLENGTH];
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCommunityReadMail::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < COMMUNITY_READ_MAIL_DIALOG__MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return COMMUNITY_READ_MAIL_DIALOG__MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCommunityReadMail::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < COMMUNITY_READ_MAIL_DIALOG__MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCommunityMan*
uiCommunityReadMail::GetManager()
{
    return this->m_pCommunityMan;
}

//------------------------------------------------------------------------------

