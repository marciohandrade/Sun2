#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiHelpAnnounceWindow class
//------------------------------------------------------------------------------

#include "HelpInfoParser.h"
#include "FTextRender.h"
#include "uiBase.h"
#include "timer.h"
class uiHelpMan;

class uiHelpAnnounceWindow: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_TXT_TITLE,
        DLGPOS_TXT_DESC1,
		DLGPOS_TXT_DESC2,

		DLGPOS_IMAGE,
	
		DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE = DLGPOS_TXT_DESC2 - DLGPOS_TXT_TITLE + 1,
		POS_DESC_SIZE = DLGPOS_TXT_DESC2 - DLGPOS_TXT_DESC1 + 1,
    };

public:
    uiHelpAnnounceWindow(InterfaceManager *pUIMan);
    virtual ~uiHelpAnnounceWindow();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    virtual VOID            Process(DWORD dwTick);

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiHelpMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					renderHelpInfo();
	void					SetHelpInfo(BASE_HelpInfo& helpInfo);
	void					SetGMHelpInfo(BASE_HelpInfo& helpInfo, int iParam);


#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
    //! 언어를 바꿀때 사용.
    void                    RestoreHelpInfo();
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

private:
	BOOL					_isEnable();
	void					_renderHelpImage();
	void					_dlgSettingforTrans();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();
	virtual void            UpdatePosition();

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
    uiHelpMan*				m_pHelpMan;

    bool                    m_bControlsInitialized;

	CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];
	CCtrlPictureWZ*			m_pPicture;

	FTextRender				m_TextRender[POS_DESC_SIZE];
	BASE_HelpInfo			m_HelpInfo;

	util::Timer 			m_ShowTimer;
	util::Timer 			m_Counter;	//카운터 메시지를 위한 타이머

	HANDLE                  m_hHelpImage;

    bool                    m_bTrans;
    float                   m_fTrans;
    DWORD                   m_dwTransAcc;
    DWORD                   m_dwTransEnd;

	int						m_iSec;
	int						m_iOldSec;

	int						m_iShowTime;

    std::vector<CControlWZ *>   m_vPicControls;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiHelpAnnounceWindow::getCtrlIDToPos( WzID wzId )
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
uiHelpAnnounceWindow::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiHelpMan*
uiHelpAnnounceWindow::GetManager()
{
    return this->m_pHelpMan;
}

//------------------------------------------------------------------------------

