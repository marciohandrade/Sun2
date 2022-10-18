#pragma once
//------------------------------------------------------------------------------
//
//    �۾���: ������
//    uiEventWindow class
//------------------------------------------------------------------------------

#include "uiEventWindow_def.h"
#include "uibase.h"

class uiEventMan;

class uiEventWindow: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,			//�����ư
		DLGPOS_BTN_GAME_ITEM,		//���� ������
		DLGPOS_BTN_OFFLINE_EVENT,	//�������� �̺�Ʈ
		DLGPOS_BTN_BLANK_EVENT,		

		DLGPOS_TEXT_TITLE,			// ���� �ؽ�Ʈ
		DLGPOS_TEXT_LIST,			// ���� ����Ʈ

		DLGPOS_SCR_VERTICAL,		//��ũ�ѹ��߰�				
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

