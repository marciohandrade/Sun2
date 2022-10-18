#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCharPopupMenu class
//------------------------------------------------------------------------------

#include "uiCharPopupMenu_def.h"
#include "uibase.h"
#include "interfacemanager.h"

class uiCharPopupMan;

class uiCharPopupMenu: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_LST_MENU,

        DIALOG_MAX,
    };

	enum ePOPUP_MENU
	{
        ePMENU_NORMAL_WHISPER,          // 귓속말
#ifdef _NA_002935_20110704_ITEM_SCORE
        ePMENU_SHOW_EQUIP,              //장비보기
#endif //_NA_002935_20110704_ITEM_SCORE
        ePMENU_NORMAL_FRIEND,           // 친구추가
        ePMENU_NORMAL_TRADE,            // 거래
        ePMENU_NORMAL_FOLLOW,           // 따라가기
#ifdef _KIKI_ADD_G_LOG_MENU
		ePMENU_NORMAL_G_LOG,			// G-로그
#endif

		ePMENU_GROUP_ADD,				// 그룹추가
		ePMENU_GROUP_DEL,				// 그룹삭제

        ePMENU_PARTY_MASTER_INVITE,     // 파티초대
        ePMENU_PARTY_MASTER_MIGRATION,  // 파장위임
        ePMENU_PARTY_MASTER_KICK,       // 강제추방
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
        ePMENU_GUILD_INVITE,            // 길드초대
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
        ePMENU_BLOCK_ADD,   // 차단추가
        ePMENU_BLOCK_DELETE,// 차단해제

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        ePMENU_BATTLE_GROUND_VOTE, // 전장 추방투표
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

		ePMENU_CANCEL,
		ePMENU_MAX
	};

public:
    uiCharPopupMenu(InterfaceManager *pUIMan);
    virtual ~uiCharPopupMenu();

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
    uiCharPopupMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_checkSkipMenu(int iMenuIndex);
	void					_setListWidth();	// 스트링 갯수에 따라 유동적으로 변하므로 ShowWindow시 계산할것
public:

	
    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void updateMenu();
	void renderMenu();

	void ClearListSelect() { m_pListControl->SetCurSel(-1); }

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();
    virtual void            Process(DWORD dwTick);

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
    uiCharPopupMan*			m_pCharPopupMan;
    CCtrlListWZ*            m_pListControl;

    bool                    m_bControlsInitialized;

	std::vector<int>		m_vMenu;
	static int				m_PopupMenuArray[ePMENU_MAX];

	int						m_iSaveType;
	bool					m_bSaveGroupType;
	bool					block_member_type_;
#ifdef _NA_002935_20110704_ITEM_SCORE
    bool                    show_equip_menu_;
#endif //_NA_002935_20110704_ITEM_SCORE

	CTimerSimple            m_timerShowoffLater;

	int						m_iListWidth;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCharPopupMenu::getCtrlIDToPos( WzID wzId )
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
uiCharPopupMenu::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCharPopupMan*
uiCharPopupMenu::GetManager()
{
    return this->m_pCharPopupMan;
}


