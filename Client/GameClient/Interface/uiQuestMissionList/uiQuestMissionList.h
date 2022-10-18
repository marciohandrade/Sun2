#ifndef     __UIQUESTMISSIONLIST_H__
#define     __UIQUESTMISSIONLIST_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestMissionList

    미션 정보 리스트

    (C) 2005 y2jinc 
*/
#include "uiBase.h"
#include "uiQuestMissionList_def.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"
#include "globalfunc.h"
class uiQuestMan;

class uiQuestMissionList :
    public uiBase
{
    enum eDIALOG_POS
    {
		eDLGPOS_CHK_MISSION,
		eDLGPOS_CHK_QUEST,	
		eDLGPOS_LST_MISSION_LIST,
		eDLGPOS_BTN_SELECT_MISSION,
		eDLGPOS_BTN_EXIT,
		eDLGPOS_BTN_MAIN,
		eDLGPOS_BTN_SUB1,
		eDLGPOS_BTN_SUB2,
		eDLGPOS_BTN_SUB3,
		eDLGPOS_BTN_SUB4,
		eDLGPOS_TXT_REWARD_1,
		eDLGPOS_TXT_REWARD_2,
		eDLGPOS_TXT_REWARD_3,
		        
        DIALOG_MAX,
    };

    enum
    {
		QUEST_CATEGORY_BEGIN= eDLGPOS_CHK_MISSION,
		QUEST_CATEGORY_SIZE	= eDLGPOS_CHK_QUEST-QUEST_CATEGORY_BEGIN+1,    

		BEGIN_MISSION_TYPE  = eDLGPOS_BTN_MAIN,
		MAX_MISSION_TYPE    = eDLGPOS_BTN_SUB4-eDLGPOS_BTN_MAIN+1,
		BEGIN_REWARD        = eDLGPOS_TXT_REWARD_1,
		MAX_REWARD_COUNT    = eDLGPOS_TXT_REWARD_3-BEGIN_REWARD+1,
    };

public:
    uiQuestMissionList(InterfaceManager *pUIMan);
    virtual ~uiQuestMissionList();

	TYPEDEF_MAP(MISSION_GROUP, CODETYPE, MapGroup * );

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiQuestMan*				GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ClearInfo();

	// 메니저에서 공동으로 호출하는 함수.
	void					UpdateMissionInfo();

protected:
	void					initInfo();
	void					updateInfo();

	void					initMissionGroupMap();
	void					refreshMissionInfo();
    
	void					clickChkCategory(POSTYPE pos);
	void					clickMissionList();
	void					clickMissionBtn();
	void					clickMissionSubBtn(POSTYPE pos);

private:
	void					refreshMissionReward();

	//--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
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

    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL                    m_bInitControls;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];

	MISSION_GROUP_MAP		m_MissionMapGroupMap;

	//control
	UI_CONTROL_LIST_PROPERTY(MissionList, StrToWzID("LS00"));
	UI_CONTROL_STATIC_PROPERTY(CurSelMission, StrToWzID("T201"));
    	
	CCtrlButtonCheckWZ *	m_pCtrlCheckCategory[QUEST_CATEGORY_SIZE];
	CCtrlButtonCheckWZ *	m_pCtrlStaticMissionInfo[MAX_MISSION_TYPE];
	CCtrlStaticWZ *			m_pCtrlStaticRewardInfo[MAX_REWARD_COUNT];
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestMissionList::getCtrlIDToPos( WzID wzId )
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
uiQuestMissionList::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestMissionList::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __UIQUESTMISSIONLIST_H__
