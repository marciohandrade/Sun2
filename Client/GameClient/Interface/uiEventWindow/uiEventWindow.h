#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiEventWindow class
//------------------------------------------------------------------------------

#include "uiEventWindow_def.h"
#include "uibase.h"

class uiEventMan;

class uiEventWindow: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,			//종료버튼
		DLGPOS_BTN_GAME_ITEM,		//게임 아이템
		DLGPOS_BTN_OFFLINE_EVENT,	//오프라인 이벤트
		DLGPOS_BTN_BLANK_EVENT,		

		DLGPOS_TEXT_TITLE,			// 제목 텍스트
		DLGPOS_TEXT_LIST,			// 설명 리스트

		DLGPOS_SCR_VERTICAL,		//스크롤바추가				
        DIALOG_MAX,
    };


public:
    uiEventWindow(InterfaceManager *pUIMan);
    virtual ~uiEventWindow();

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
    uiEventMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
	//virtual void			UpdatePosition();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

	void					refreshEventMessage(bool is_enable_event = false);
	void 					UpScroll(int nRange);
	void 					UpdateMouseWheel();
	void 					DownScroll(int nRange);
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
    uiEventMan*				m_pEventMan;
	bool                    m_bControlsInitialized;
	CCtrlListWZ*			m_pList;
	CCtrlVScrollWZ*         m_pScroll;
    
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiEventWindow::getCtrlIDToPos( WzID wzId )
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
uiEventWindow::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiEventMan*
uiEventWindow::GetManager()
{
    return this->m_pEventMan;
}

//------------------------------------------------------------------------------

