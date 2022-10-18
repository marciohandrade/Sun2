#pragma once
//------------------------------------------------------------------------------
//		작업자	:	이태준
//		uiPrivateWayPointWindow Class
//		일본 개인 웨이포인트 다이얼로그
//------------------------------------------------------------------------------
#include "uibase.h"
#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"

class uiPersonalWayPointWindow: public uiBase
{
	typedef std::vector<WAYPOINTSLOT>				WAYPOINT_VEC;
	typedef std::vector<WAYPOINTSLOT>::iterator		WAYPOINT_VEC_ITR;
    enum eDIALOG_POS
    {
        DLGPOS_WP_BTN_EXIT,
		DLGPOS_WP_BTN_SAVE,
		DLGPOS_WP_BTN_DELETE,
		DLGPOS_WP_BTN_MOVE,

		DLGPOS_WP_LST,
			
		DLGPOS_WP_VSCROLL,

        DIALOG_WP_MAX,
    };

    enum
    {
        POS_WP_BTN_SIZE = DLGPOS_WP_BTN_MOVE - DLGPOS_WP_BTN_EXIT + 1,
    };

	enum
	{
		WP_BTN_EXIT = 0,
		WP_BTN_SAVE,
		WP_BTN_DELETE,
		WP_BTN_MOVE,
	};

public:

    uiPersonalWayPointWindow(InterfaceManager *pUIMan);
    virtual ~uiPersonalWayPointWindow();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

protected:
    void                    InitControls();
	void					CheckWaypointEnable();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiPersonalWayPointMan*			GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					UpdateMouseWheel();
	void					DownScroll(int nRange);
	void					UpScroll(int nRange);

public:
	void					InitWayPoint();
	bool					IsRecvWaypointInfo();
	bool					IsWaypointInfoPending();
	void					UnSetWaypointInfoPending();

	bool					IsWayPointItemPending();
	void					SetWayPointItemPending(bool bFlag);
	
	bool					IsDialogBtnPending();
	void					SetDialogBtnPending(bool bFlag);

	int						GetUsedWayPointSlotCount();
    //--------------------------------------------------------------------------
    // dialog functions

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();


protected:
	void					RefreshList();
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    void MoveWayPoint();


public:
	const WAYPOINT_VEC& GetWaypointdata() const { return m_vecListData;}  
	int GetSeleteData() { return m_iSelectedIndex; }  

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    void SetSeleteIndex(int index) { m_iSelectedIndex = index; }
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	void					SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();
	void					RECV_CG_CHAR_WAYPOINT_INFO_LOAD_ACK(MSG_BASE * pMsg);

	void					SEND_CG_CHAR_WAYPOINT_INFO_SAVE_SYN();
	void					RECV_CG_CHAR_WAYPOINT_INFO_SAVE_ACK(MSG_BASE * pMsg);

	void					SEND_CG_CHAR_WAYPOINT_INFO_DEL_SYN();
	void					RECV_CG_CHAR_WAYPOINT_INFO_DEL_ACK(MSG_BASE * pMsg);

	void					SEND_CG_CHAR_WAYPOINT_INFO_EXE_CMD();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiPersonalWayPointMan*			m_pWayPointMan;

	static WzID				        m_wzId[DIALOG_WP_MAX];

	CCtrlButtonWZ*					m_pBtnControls[POS_WP_BTN_SIZE];
    CCtrlListWZ*					m_pListControl;
	CCtrlVScrollWZ*					m_pScroll;

	WAYPOINT_VEC					m_vecListData;

    bool							m_bRecvWayPointInfo;
	bool							m_bWayPointInfoPending;// 웨이포인트 정보 요청대기
	bool							m_bWayPointItemPending;// 웨이포인트 아이템 활성화 요청대기
	bool							m_bDialgBtnPending;//다이얼로그 버튼 실행 대기

	int								m_iSelectedIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPersonalWayPointWindow::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_WP_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_WP_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiPersonalWayPointWindow::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_WP_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiPersonalWayPointMan*
uiPersonalWayPointWindow::GetManager()
{
    return m_pWayPointMan;
}

inline
bool uiPersonalWayPointWindow::IsRecvWaypointInfo()
{
	return m_bRecvWayPointInfo;
}
//------------------------------------------------------------------------------
inline
bool uiPersonalWayPointWindow::IsWaypointInfoPending()
{
	return m_bWayPointInfoPending;
}

inline
void uiPersonalWayPointWindow::UnSetWaypointInfoPending()
{
	m_bWayPointInfoPending = false;
}

inline
int uiPersonalWayPointWindow::GetUsedWayPointSlotCount()
{
	return m_vecListData.size();
}


//단일 네임스페이스
namespace PERSIONERWARP  
{
	//패킷을 보내기 위해서.  
	void SEND_CG_FREE_CHAR_WAYPOINT();  
};  
