#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiEventItemSelect class
//------------------------------------------------------------------------------
#include "uibase.h"
#include "uiEventItemSelect_def.h"

struct EVENT_INFO_LIST
{
	CODETYPE		m_dwEventID;
	BYTE			m_byEventMode;
	BYTE			m_byCountMax;
	CODETYPE		m_dwRegisterItemIndex[5];
	WORD			m_wRegisterItemNum[5];
	int				m_iCurRegItemNum[5];
	int				m_iNameString[5];
};

class uiEventMan;

class uiEventItemSelect: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,		//종료버튼
		DLGPOS_TXT_ITEM,		//재료 (경품응모용)	
		DLGPOS_TXT_COUNT,		//참가횟수 (경품응모용)
		DLGPOS_TXT_ITEM2,		//재료 (랜던보상용)
		DLGPOS_BTN_OK,			//확인
		DLGPOS_BTN_CANCEL,		//취소

		DLGPOS_TEXT_TITLE,		//제목

		DLGPOS_LIST,			//리스트(아이템)
        DLGPOS_VSCROLL,         //스크롤
        DIALOG_MAX,
    };

	enum
    {
		TXT_ITEM,
		TXT_COUNT,
		TXT_ITEM2,
		POS_TXT_SIZE = DLGPOS_TXT_ITEM2 - DLGPOS_TXT_ITEM + 1,
    };

public:
    uiEventItemSelect(InterfaceManager *pUIMan);
    virtual ~uiEventItemSelect();

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
	void					_initList();
	void					_refreshList();
	int						_getEventListIndex();


public:
	void					SetEventItemStatusList(MSG_BASE * pMsg);
	void					UpdateList();
	void					SuccessItemReg(CODETYPE dwEventID);


    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );

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
    uiEventMan*				m_pEventMan;
    CCtrlListWZ*            m_pListControl;
	CCtrlStaticWZ*          m_pTxtControl[POS_TXT_SIZE];
	bool                    m_bControlsInitialized;

	std::vector<EVENT_INFO_LIST>		m_vecEventList[2];

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiEventItemSelect::getCtrlIDToPos( WzID wzId )
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
uiEventItemSelect::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiEventMan*
uiEventItemSelect::GetManager()
{
    return this->m_pEventMan;
}

//------------------------------------------------------------------------------
