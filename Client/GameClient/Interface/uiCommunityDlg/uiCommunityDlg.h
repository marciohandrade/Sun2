#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCommunityDlg class
//------------------------------------------------------------------------------

#include "uibase.h"

#define MAX_ICON_NUM					10
#define SHOW_DEFAULT_FRIEND_LINE		10
struct FRIEND_DATA;
struct MAIL_DATA;

enum eCOMMUNITY_MODE;
class uiCommunityMan;

class uiCommunityDlg: public uiBase
{
    enum eDIALOG_POS
    {
        COMMUNITY_DIALOG_EXIT	= 0,

		// 각종 탭
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		COMMUNITY_DIALOG_TAB_FRIEND,
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
		COMMUNITY_DIALOG_TAB_MAIL,
		COMMUNITY_DIALOG_TAB_GROUP,
		COMMUNITY_DIALOG_TAB_SUB_FRIEND,
		COMMUNITY_DIALOG_TAB_SUB_BLOCK,
		COMMUNITY_DIALOG_TAB_SUB_REVENGE,

		// 텍스트
		COMMUNITY_DIALOG_TXT_1,
		COMMUNITY_DIALOG_TXT_2,
		COMMUNITY_DIALOG_TXT_3,
		COMMUNITY_DIALOG_TXT_REVENGE_POINT,
		COMMUNITY_DIALOG_TXT_REJECTION_CHAT,
		COMMUNITY_DIALOG_TXT_HIDE_OFFLINE,
		COMMUNITY_DIALOG_TXT_SHOW_NEW_MAIL,
		COMMUNITY_DIALOG_TXT_REJECTION_GROUPS,

		// 체크버튼
		COMMUNITY_DIALOG_CHECK_REJECTION_CHAT,
		COMMUNITY_DIALOG_CHECK_HIDE_OFFLINE,
		COMMUNITY_DIALOG_CHECK_SHOW_NEW_MAIL,
		COMMUNITY_DIALOG_CHECK_REJECTION_GROUP,

		// 스크롤
		COMMUNITY_DIALOG_VSCROLL,

		// 리스트
		COMMUNITY_DIALOG_LST_FRIEND,
		COMMUNITY_DIALOG_LST_MAIL,

		// 버튼
		// ON/OFF 아이콘
		COMMUNITY_DIALOG_BTN_ON1,
		COMMUNITY_DIALOG_BTN_ON2,
		COMMUNITY_DIALOG_BTN_ON3,
		COMMUNITY_DIALOG_BTN_ON4,
		COMMUNITY_DIALOG_BTN_ON5,
		COMMUNITY_DIALOG_BTN_ON6,
		COMMUNITY_DIALOG_BTN_ON7,
		COMMUNITY_DIALOG_BTN_ON8,
		COMMUNITY_DIALOG_BTN_ON9,
		COMMUNITY_DIALOG_BTN_ON10,
		
		COMMUNITY_DIALOG_BTN_OFF1,
		COMMUNITY_DIALOG_BTN_OFF2,
		COMMUNITY_DIALOG_BTN_OFF3,
		COMMUNITY_DIALOG_BTN_OFF4,
		COMMUNITY_DIALOG_BTN_OFF5,
		COMMUNITY_DIALOG_BTN_OFF6,
		COMMUNITY_DIALOG_BTN_OFF7,
		COMMUNITY_DIALOG_BTN_OFF8,
		COMMUNITY_DIALOG_BTN_OFF9,
		COMMUNITY_DIALOG_BTN_OFF10,
		
		//새쪽지/읽은 쪽지 아이콘
		COMMUNITY_DIALOG_BTN_NEW1,
		COMMUNITY_DIALOG_BTN_NEW2,
		COMMUNITY_DIALOG_BTN_NEW3,
		COMMUNITY_DIALOG_BTN_NEW4,
		COMMUNITY_DIALOG_BTN_NEW5,
		COMMUNITY_DIALOG_BTN_NEW6,
		COMMUNITY_DIALOG_BTN_NEW7,
		COMMUNITY_DIALOG_BTN_NEW8,
		COMMUNITY_DIALOG_BTN_NEW9,
		COMMUNITY_DIALOG_BTN_NEW10,
		
		COMMUNITY_DIALOG_BTN_READ1,
		COMMUNITY_DIALOG_BTN_READ2,
		COMMUNITY_DIALOG_BTN_READ3,
		COMMUNITY_DIALOG_BTN_READ4,
		COMMUNITY_DIALOG_BTN_READ5,
		COMMUNITY_DIALOG_BTN_READ6,
		COMMUNITY_DIALOG_BTN_READ7,
		COMMUNITY_DIALOG_BTN_READ8,
		COMMUNITY_DIALOG_BTN_READ9,
		COMMUNITY_DIALOG_BTN_READ10,

		// 친구초대 이벤트용 ON/OFF 아이콘
		COMMUNITY_DIALOG_BTN_EVTOFF1,
		COMMUNITY_DIALOG_BTN_EVTOFF2,
		COMMUNITY_DIALOG_BTN_EVTOFF3,
		COMMUNITY_DIALOG_BTN_EVTOFF4,
		COMMUNITY_DIALOG_BTN_EVTOFF5,
		COMMUNITY_DIALOG_BTN_EVTOFF6,
		COMMUNITY_DIALOG_BTN_EVTOFF7,
		COMMUNITY_DIALOG_BTN_EVTOFF8,
		COMMUNITY_DIALOG_BTN_EVTOFF9,
		COMMUNITY_DIALOG_BTN_EVTOFF10,

		COMMUNITY_DIALOG_BTN_EVTON1,
		COMMUNITY_DIALOG_BTN_EVTON2,
		COMMUNITY_DIALOG_BTN_EVTON3,
		COMMUNITY_DIALOG_BTN_EVTON4,
		COMMUNITY_DIALOG_BTN_EVTON5,
		COMMUNITY_DIALOG_BTN_EVTON6,
		COMMUNITY_DIALOG_BTN_EVTON7,
		COMMUNITY_DIALOG_BTN_EVTON8,
		COMMUNITY_DIALOG_BTN_EVTON9,
		COMMUNITY_DIALOG_BTN_EVTON10,

		COMMUNITY_DIALOG_BTN_ADD_FRIEND,
		COMMUNITY_DIALOG_BTN_DEL_FRIEND,
		COMMUNITY_DIALOG_BTN_ADD_BLOCK,
		COMMUNITY_DIALOG_BTN_DEL_BLOCK,
		COMMUNITY_DIALOG_BTN_DEL_REVENGE,
		COMMUNITY_DIALOG_BTN_WARP,
		COMMUNITY_DIALOG_BTN_DEL_MAIL,
		COMMUNITY_DIALOG_BTN_DEL_ALL_MAIL,
		COMMUNITY_DIALOG_BTN_WRITE_MAIL,
		COMMUNITY_DIALOG_BTN_INVITE,

		COMMUNITY_DIALOG_LIST_BTN_ID,
		COMMUNITY_DIALOG_LIST_BTN_LV,
		COMMUNITY_DIALOG_LIST_BTN_CLASS,
		COMMUNITY_DIALOG_LIST_BTN_CHANNEL,
		COMMUNITY_DIALOG_LIST_BTN_LOGON,

		COMMUNITY_DIALOG_LIST_BTN_SENDER,
		COMMUNITY_DIALOG_LIST_BTN_DATE,
		COMMUNITY_DIALOG_LIST_BTN_MEMO,

		COMMUNITY_DIALOG_BTN_ADD_MEMBER,
		COMMUNITY_DIALOG_BTN_DEL_MEMBER,

        COMMUNITY_DIALOG_MAX,

    };


	enum COMMUNITY_TABS
	{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		COMMUNITY_TAB_FRIEND,
#endif
		COMMUNITY_TAB_MAIL,
		COMMUNITY_TAB_GROUP,
		COMMUNITY_TAB_SUB_FRIEND,
		COMMUNITY_TAB_SUB_BLOCK,
		COMMUNITY_TAB_SUB_REVENGE,

		COMMUNITY_TAB_MAX,
	};

	enum COMMUNITY_TXTS
	{
		COMMUNITY_TXT_1,
		COMMUNITY_TXT_2,
		COMMUNITY_TXT_3,
		COMMUNITY_TXT_REVENGE_POINT,
		COMMUNITY_TXT_REJECTION_GROUPS,
		COMMUNITY_TXT_MAX,
	};

	enum COMMUNITY_CHECKS
	{
		COMMUNITY_CHECK_REJECTION_CHAT,
		COMMUNITY_CHECK_HIDE_OFFLINE,
		COMMUNITY_CHECK_SHOW_NEW_MAIL,
		COMMUNITY_CHECK_REJECTION_GROUP,
		COMMUNITY_CHECK_MAX,
	};

	enum COMMUNITY_LSTS
	{
		COMMUNITY_LST_FRIEND,
		COMMUNITY_LST_MAIL,

		COMMUNITY_LST_MAX,
	};

	enum COMMUNITY_BTNS
	{
		COMMUNITY_BTN_ADD_FRIEND,
		COMMUNITY_BTN_DEL_FRIEND,

		COMMUNITY_BTN_ADD_BLOCK,
		COMMUNITY_BTN_DEL_BLOCK,

		COMMUNITY_BTN_DEL_REVENGE,
		COMMUNITY_BTN_WARP,

		COMMUNITY_BTN_DEL_MAIL,
		COMMUNITY_BTN_DEL_ALL_MAIL,
		
		COMMUNITY_BTN_WRITE_MAIL,
		COMMUNITY_BTN_INVITE,

		COMMUNITY_LIST_BTN_ID,
		COMMUNITY_LIST_BTN_LV,
		COMMUNITY_LIST_BTN_CLASS,
		COMMUNITY_LIST_BTN_CHANNEL,
		COMMUNITY_LIST_BTN_LOGON,

		COMMUNITY_LIST_BTN_SENDER,
		COMMUNITY_LIST_BTN_DATE,
		COMMUNITY_LIST_BTN_MEMO,

		COMMUNITY_BTN_ADD_MEMBER,
		COMMUNITY_BTN_DEL_MEMBER,

		COMMUNITY_BTN_MAX,
	};

	enum COMMUNITY_ON_BTNS
	{
		COMMUNITY_BTN_ON1,
		COMMUNITY_BTN_ON2,
		COMMUNITY_BTN_ON3,
		COMMUNITY_BTN_ON4,
		COMMUNITY_BTN_ON5,
		COMMUNITY_BTN_ON6,
		COMMUNITY_BTN_ON7,
		COMMUNITY_BTN_ON8,
		COMMUNITY_BTN_ON9,
		COMMUNITY_BTN_ON10,
		
		COMMUNITY_BTN_ON_MAX,
	};

	enum COMMUNITY_ON_OFF_BTNS
	{
		COMMUNITY_BTN_OFF1,
		COMMUNITY_BTN_OFF2,
		COMMUNITY_BTN_OFF3,
		COMMUNITY_BTN_OFF4,
		COMMUNITY_BTN_OFF5,
		COMMUNITY_BTN_OFF6,
		COMMUNITY_BTN_OFF7,
		COMMUNITY_BTN_OFF8,
		COMMUNITY_BTN_OFF9,
		COMMUNITY_BTN_OFF10,
		
		COMMUNITY_BTN_OFF_MAX,
	};

	enum COMMUNITY_MAIL_NEW_BTNS
	{
		COMMUNITY_BTN_NEW1,
		COMMUNITY_BTN_NEW2,
		COMMUNITY_BTN_NEW3,
		COMMUNITY_BTN_NEW4,
		COMMUNITY_BTN_NEW5,
		COMMUNITY_BTN_NEW6,
		COMMUNITY_BTN_NEW7,
		COMMUNITY_BTN_NEW8,
		COMMUNITY_BTN_NEW9,
		COMMUNITY_BTN_NEW10,
		
		COMMUNITY_BTN_NEW_MAX,
	};

	enum COMMUNITY_MAIL_READ_BTNS
	{
		COMMUNITY_BTN_READ1,
		COMMUNITY_BTN_READ2,
		COMMUNITY_BTN_READ3,
		COMMUNITY_BTN_READ4,
		COMMUNITY_BTN_READ5,
		COMMUNITY_BTN_READ6,
		COMMUNITY_BTN_READ7,
		COMMUNITY_BTN_READ8,
		COMMUNITY_BTN_READ9,
		COMMUNITY_BTN_READ10,
		
		COMMUNITY_BTN_READ_MAX,
	};
	enum COMMUNITY_EVTON_BTNS
	{
		COMMUNITY_BTN_EVTON1,
		COMMUNITY_BTN_EVTON2,
		COMMUNITY_BTN_EVTON3,
		COMMUNITY_BTN_EVTON4,
		COMMUNITY_BTN_EVTON5,
		COMMUNITY_BTN_EVTON6,
		COMMUNITY_BTN_EVTON7,
		COMMUNITY_BTN_EVTON8,
		COMMUNITY_BTN_EVTON9,
		COMMUNITY_BTN_EVTON10,
		
		COMMUNITY_BTN_EVTON_MAX,
	};

	enum COMMUNITY_EVTOFF_BTNS
	{
		COMMUNITY_BTN_EVTOFF1,
		COMMUNITY_BTN_EVTOFF2,
		COMMUNITY_BTN_EVTOFF3,
		COMMUNITY_BTN_EVTOFF4,
		COMMUNITY_BTN_EVTOFF5,
		COMMUNITY_BTN_EVTOFF6,
		COMMUNITY_BTN_EVTOFF7,
		COMMUNITY_BTN_EVTOFF8,
		COMMUNITY_BTN_EVTOFF9,
		COMMUNITY_BTN_EVTOFF10,
		
		COMMUNITY_BTN_EVTOFF_MAX,
	};

	enum FRIEND_LIST
	{
		FRIEND_LIST_NAME = 0,
		FRIEND_LIST_LEVEL,
		FRIEND_LIST_CLASS,
		FRIEND_LIST_CHANNEL,
		FRIEND_LIST_ONLINE,
		FRIEND_LIST_MAX,
	};

	enum MAIL_LIST
	{
		MAIL_LIST_NAME = 0,
		MAIL_LIST_DATE,
		MAIL_LIST_MEMO,
		
		MAIL_LIST_MAX,
	};

public:
    uiCommunityDlg(InterfaceManager *pUIMan);
    virtual ~uiCommunityDlg();

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
    uiCommunityMan*			GetManager();
    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
#ifdef _NA_000000_20130114_RENEWER_UI
	void					_setDefaultFriend(ENUM_STATEWZ eState,bool ShowingTap = true);
#else
	void					_setDefaultFriend(ENUM_STATEWZ eState);
#endif
	void					_setModeFriend(ENUM_STATEWZ eState);
	void					_setModeBlock(ENUM_STATEWZ eState);
	void					_setModeRevenge(ENUM_STATEWZ eState);
	void					_setModeMail(ENUM_STATEWZ eState);
	void					_refreshMailNum();
	void					_setModeGroup(ENUM_STATEWZ eState);
	void					_SortByType(int eModeType, int eSortType);
	void					_ShowListIcons();
	void					_HideListIcons();
	time_t					GetLastTime(INT64 lastDate);

	void					_hideMailIcons();
	void					_hideOnOffIcons();
	void					_setNewMail(int index);
	void					_setReadMail(int index);
	void					_setOnline(int index, BYTE byFriendSts = 1);
	void					_setOffline(int index, BYTE byFriendSts = 1);
	void					_hideMailIcon(int index);
	void					_hideOnOffIcon(int index);
	void					_refreshOnOffIcon();
	void					_refreshMailIcon();

	void					_updateCheckState();
	void					_clearSelection();

	void					_RefreshControl(nArray<FRIEND_DATA> &ArrayType);
	void					_SettListCtrlData(COMMUNITY_LSTS eType,int nIndex,int nSubIndex,TCHAR* pszText,BOOL bOnLine, BYTE byMaster = false);
	void					_RefreshMailControl();

    void ViewMemoOnList();

public:
	void					Clear();
	void					Reset();

	void					ChangeMode(int iMode, bool bForce = false);

	void					RefreshControl();
	void					RefreshFriendList();
	void					RefreshMailList();

	int						GetCurListSel();
	FRIEND_DATA *			GetCurData(int iSelect);
	MAIL_DATA *				GetCurMailData(int iSelect);
	int						GetCurScrollCount() { return m_pScroll->GetScrollPosWZ(); }
	
    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void RefreshOnlineMember(eCOMMUNITY_MODE community_mode);
protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

private:
	void					UpdateMouseWheel();
	void					DownScroll(int nRange);
	void					UpScroll(int nRange);

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[COMMUNITY_DIALOG_MAX];

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

	CCtrlButtonCheckWZ*     m_pBtnCheckTabs[COMMUNITY_TAB_MAX];
	CCtrlStaticWZ*			m_pTxtCtrls[COMMUNITY_TXT_MAX];
	CCtrlButtonCheckWZ*     m_pBtnChecks[COMMUNITY_CHECK_MAX];
    CCtrlListWZ*			m_pListCtrls[COMMUNITY_LST_MAX];
    CCtrlButtonWZ*			m_pBtnCtrls[COMMUNITY_BTN_MAX];
	CCtrlButtonWZ*			m_pOnBtn[COMMUNITY_BTN_ON_MAX];
	CCtrlButtonWZ*			m_pOffBtn[COMMUNITY_BTN_OFF_MAX];
	CCtrlButtonWZ*			m_pNewBtn[COMMUNITY_BTN_NEW_MAX];
	CCtrlButtonWZ*			m_pReadBtn[COMMUNITY_BTN_READ_MAX];
	CCtrlButtonWZ*			m_pEvtOnBtn[COMMUNITY_BTN_EVTON_MAX];
	CCtrlButtonWZ*			m_pEvtOffBtn[COMMUNITY_BTN_EVTOFF_MAX];
	CCtrlVScrollWZ*			m_pScroll;

    bool					m_bControlsInitialized;

    int friend_list_column_[FRIEND_LIST_MAX];
    int mail_list_column_[MAIL_LIST_MAX];
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCommunityDlg::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < COMMUNITY_DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return COMMUNITY_DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCommunityDlg::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < COMMUNITY_DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}


//------------------------------------------------------------------------------
/**
*/
void _CallBack_Friend_Add(bool bYes, const char *pString );	//외부에서 친구 추가 할때



//------------------------------------------------------------------------------
