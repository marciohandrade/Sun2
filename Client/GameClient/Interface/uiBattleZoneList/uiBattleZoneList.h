#ifndef     __USER_INTERFACE_BATTLEZONELIST_H__
#define     __USER_INTERFACE_BATTLEZONELIST_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneList

    battle zone list user interface

    (C) 2005 ckbang
*/
#include "uiBattleZoneList_def.h"
#include "uiBase.h"
#include "IconMgr.h"
#include "uibattlezoneman/uiBattleZoneMan_def.h"

class uiBattleZoneMan;
class uiBattleZoneList :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT=0,
        DLGPOS_BTN_CREATE,
        DLGPOS_BTN_JOIN,
        DLGPOS_BTN_REFRESH_ROOMLIST,
        DLGPOS_BTN_SEARCH,
        DLGPOS_BTN_SHORTCUT_MISSION,
#ifndef	_EXCECPT_HUNTING_001011
        DLGPOS_BTN_SHORTCUT_HUNTING,
        DLGPOS_BTN_SHORTCUT_CHUNTING,
#endif
        DLGPOS_BTN_SHORTCUT_PVP,

        DLGPOS_CHKBTN_VIEW_ALL,
        DLGPOS_CHKBTN_VIEW_POSSIBLE,
        DLGPOS_CHKBTN_SEARCH_TYPE_ID,
        DLGPOS_CHKBTN_SEARCH_TYPE_ROOM_NO,
        
        DLGPOS_EDT_SEARCH,
        DLGPOS_SCR_VERTICAL,
        DLGPOS_LST_ROOMLIST,
        DLGPOS_TAB_ROOMLIST,

		DLGPOS_LIST_COLUM1,
		DLGPOS_LIST_COLUM2,
		DLGPOS_LIST_COLUM3,
		DLGPOS_LIST_COLUM4,
		DLGPOS_LIST_COLUM5,
		DLGPOS_LIST_COLUM6,
		DLGPOS_LIST_COLUM7,
		DLGPOS_LIST_COLUM8,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_BEGIN       =   DLGPOS_BTN_EXIT,
        POS_BTN_END         =   DLGPOS_BTN_SHORTCUT_PVP + 1,
        NUM_POS_BTN         =   POS_BTN_END - POS_BTN_BEGIN,

        POS_CHKBTN_BEGIN    =   DLGPOS_CHKBTN_VIEW_ALL,
        POS_CHKBTN_END      =   DLGPOS_CHKBTN_VIEW_POSSIBLE + 1,
        NUM_POS_CHKBTN      =   POS_CHKBTN_END - POS_CHKBTN_BEGIN,

        POS_CHKBTN_SEARCH_BEGIN    =   DLGPOS_CHKBTN_SEARCH_TYPE_ID,
        POS_CHKBTN_SEARCH_END      =   DLGPOS_CHKBTN_SEARCH_TYPE_ROOM_NO + 1,
        NUM_POS_CHKBTN_SEARCH      =   POS_CHKBTN_SEARCH_END - POS_CHKBTN_SEARCH_BEGIN,
    };

	enum LIST_INDEX
	{
		LIST_INDEX_NUM = 0,			//	번호
		LIST_INDEX_TYPE,			//	종류
		LIST_INDEX_STATE,			//  진행
		LIST_INDEX_SUPERROOM_TYPE,
		LIST_INDEX_CHUNTING_TYPE,	//	방 타입(경쟁헌팅의 경우에만 표시)
		LIST_INDEX_ROOM_NAME,		//	방 제목
		LIST_INDEX_USER_NUM,		//	인원수
		LIST_INDEX_MAP,				//	맵

		LIST_INDEX_MAX
	};

	struct ZONELISTINFO
	{
		int						m_iZoneListStart;
		int						m_iZoneListEnd;
		int						m_iType;
	};

public:
    uiBattleZoneList(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneList();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();
    void                    Clear();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiBattleZoneMan*        GetManager();
    const uicBattleZoneList_Configs& GetCfgs();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	void					_joinRoom(bool bShowMessage = true);

public:
    void                    ExitZoneList();
	void					AttachChatDialog();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    updateCheckBtns();
    void                    updateLists();
    void                    renderTabs();

    void                    Process(DWORD dwTick);

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
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
    //virtual void            UpdatePosition();

private:
	void					RenderSuperMasterIcon();
	DWORD					GetSuperMasterFontColor(BYTE byRoomType);
	ZONELISTINFO			m_ZoneListInfo;
	RECT					m_rtList;

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlButtonWZ*          m_pBtnControls[NUM_POS_BTN];
    CCtrlButtonCheckWZ*     m_pChkBtnViewControls[NUM_POS_CHKBTN];
    CCtrlButtonCheckWZ*     m_pChkBtnSearchControls[NUM_POS_CHKBTN_SEARCH];
    CCtrlDxIMEEditBox*      m_pEdtControl;
    CCtrlVScrollWZ*         m_pVScrollControl;
    CCtrlListWZ*            m_pLstControl;
    CCtrlStaticWZ*          m_pTabControl;

    bool                    m_bInitControls;

private:
    uicBattleZoneList_Configs   m_cfgs;
    std::vector<KEYTYPE>    m_keys;
    CTimerSimple            m_timerRefresh;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneList::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneList::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneList::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const uicBattleZoneList_Configs&
uiBattleZoneList::GetCfgs()
{
    return m_cfgs;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONELIST_H__
