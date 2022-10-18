#pragma once
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uibase.h"
#include "VScrollDialog.h"
#include "TreeController.h"
#include "MapInfoParser.h"
#include "./RankingManager.h"

#define ANIMATION_FRAME 12
#define ANIMATION_FRAME_WIDTH 40.0f
#define ANIMATION_FRAME_HEIGHT 40.0f

class uiMissionRankMan2;
class MissionRankingListGroupSort_Funter
{
public:
    inline BOOL operator()(RankingManager::LEVEL_AREA_BASE* lhs, RankingManager::LEVEL_AREA_BASE* rhs) const;
};

class MissionRankingListMissionSort_Funter
{
public:
    inline BOOL operator()(RankingManager::LEVEL_AREA_BASE* lhs, RankingManager::LEVEL_AREA_BASE* rhs) const;
};

class uiMissionRankList2: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_MRL_BTN_EXIT,

        DLGPOS_MRL_TXT_DATE,
        DLGPOS_MRL_TXT_MY_RANKING,
        DLGPOS_MRL_TXT_PAGE,

        DLGPOS_MRL_LST_FAME,
        DLGPOS_MRL_LST_WEEK,
        DLGPOS_MRL_LST_PREWEEK,
        DLGPOS_MRL_LST_TREE_CONTROL,
        DLGPOS_MRL_LST_MEMBER,        

        DLGPOS_MRL_VSCR_WEEK,
        DLGPOS_MRL_VSCR_PREWEEK,
        DLGPOS_MRL_VSCR_TREE,
        DLGPOS_MRL_VSCR_MEMBER,
        
        DLGPOS_MRL_BTN_REWARD,
        DLGPOS_MRL_BTN_CLOSE,

        DLGPOS_MRL_BTN_PAGE_LEFT,
        DLGPOS_MRL_BTN_PAGE_RIGHT,

        DLGPOS_MRL_TXT_TITLE,

        DLGPOS_MRL_BTN_NAME1,
        DLGPOS_MRL_BTN_NAME2,
        DLGPOS_MRL_BTN_NAME3,

        DLGPOS_MRL_BTN_POINT1,
        DLGPOS_MRL_BTN_POINT2,
        DLGPOS_MRL_BTN_POINT3,

        DLGPOS_MRL_BTN_WEEK,
        DLGPOS_MRL_BTN_PREWEEK,

        DLGPOS_MRL_STATIC_RATE,
        DLGPOS_MRL_STATIC_PARTY_NUMBER,
        DLGPOS_MRL_STATIC_LEVEL,       
        DLGPOS_MRL_STATIC_TIME,

        DLGPOS_MRL_STATIC_GUILD_LEVEL,
        DLGPOS_MRL_STATIC_GUILD,
        DLGPOS_MRL_STATIC_NAME,

        DLGPOS_MRL_STATIC_MY_INFO_PARTY_NUMBER,
        DLGPOS_MRL_STATIC_MY_INFO_LEVEL,
        
        DLGPOS_MRL_PICTURE_SEARCH_SCROLL_PICTURE,        

        DLGPOS_MRL_PICTURE_GROUP_INFOMATION_PICTURE,
        DLGPOS_MRL_TXT_GROUP_INFOMATION_TITLE,
        DLGPOS_MRL_PICTURE_GROUP_INFOMATION_BACKGROUND,
        DLGPOS_MRL_PICTURE_VSCR_MEMBER_FRAME,

        DLGPOS_MRL_PICTURE_REWARD_YELLOW_LINE,

        DLGPOS_MRL_TXT_SETTLEDATE_TITLE,
        DLGPOS_MRL_TXT_MISSION_TITLE,

        DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE	= DLGPOS_MRL_TXT_PAGE - DLGPOS_MRL_TXT_DATE + 1,
        POS_LST_SIZE	= DLGPOS_MRL_LST_MEMBER - DLGPOS_MRL_LST_FAME + 1,
        POS_VSCR_SIZE	= DLGPOS_MRL_VSCR_MEMBER - DLGPOS_MRL_VSCR_WEEK + 1,
        POS_BTN_SIZE	= DLGPOS_MRL_BTN_CLOSE - DLGPOS_MRL_BTN_REWARD + 1,
        POS_LST_BTN_SIZE = DLGPOS_MRL_BTN_POINT3 - DLGPOS_MRL_BTN_NAME1 + 1,
    };

    enum eMRL_LST_BTN
    {
        MRL_LST_BTN_NAME1,
        MRL_LST_BTN_NAME2,
        MRL_LST_BTN_NAME3,

        MRL_LST_BTN_POINT1,
        MRL_LST_BTN_POINT2,
        MRL_LST_BTN_POINT3,
    };

    enum eMRL_TXT
    {
        MRL_TXT_DATE,
        MRL_TXT_MY_RANKING,
        MRL_TXT_PAGE,
    };

    enum eMRL_LST
    {
        MRL_LST_FAME,
        MRL_LST_WEEK,
        MRL_LST_PREWEEK,
        MRL_LST_TREE_CONTROL,
        MRL_LST_MEMBER,        
    };

    enum eMRL_VSCR
    {
        MRL_VSCR_WEEK,
        MRL_VSCR_PREWEEK,
        MRL_VSCR_TREE,
        MRL_VSCR_MEMBER,
    };

    enum eMRL_BTN
    {        
        MRL_BTN_REWARD,
        MRL_BTN_CLOSE,
    };

    enum eFAME_LST
    {
        eFAME_LST_MY_RANKING,
        eFAME_LST_PARTY_NUM,
        eFAME_LST_LEVEL,
        eFAME_LST_TIME,

        eFAME_LST_MAX,
    };

    enum eWEEK_LST
    {
        eWEEK_LST_RANK,
        eWEEK_LST_PARTY_NUM,
        eWEEK_LST_LEVEL,
        eWEEK_LST_TIME,

        eWEEK_LST_MAX,
    };

    enum eMEMBER_LST
    {
        eMEMBER_LST_LEVEL,
        eMEMBER_LST_GUILD,
        eMEMBER_LST_NAME,

        eMEMBER_LST_MAX,
    };

    enum eCOLOR_STATE
    {
        eCOLOR_STATE_INC,
        eCOLOR_STATE_DEC,
    };

    // TreeController에서 판별 할 수 있는 부가 정보들을 넣어 놓는다.
    struct _stRankData
    {
        _stRankData()
        {
            ranking_system_type = eRANKING_TYPE_NONE;
            map_group_ptr       = NULL;
            map_info_ptr        = NULL;
            area_data_ptr       = NULL;
        }

        eRANKING_SYSTEM_TYPE             ranking_system_type;
        MapGroup*                        map_group_ptr;
        sMAPINFO*                        map_info_ptr;
        RankingManager::LEVEL_AREA_BASE* area_data_ptr;
    };
private:
    std::list<_stRankData*>    rank_data_list_;
    sMAPINFO*                  current_sel_mapinfo_;

public:
    void rank_data_list_clear();


public:
    uiMissionRankList2(InterfaceManager* ui_manager_ptr);
    virtual ~uiMissionRankList2();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);    
    virtual void            OnRenderSolarDialog();

    virtual void            Release();

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiMissionRankMan2*             get_ui_rank_manager_ptr_();

    //--------------------------------------------------------------------------
    // functions
    //
private:
    BOOL					_isEnable();
    void					_prepareDisplay();
    void					_initRankListSelect();
    void					_initRankList();
    void					_initMemberList();
    void					_initPageSet();
    void					_setTitle();
    void					_UpdateMouseWheel(eMRL_LST eListType);
    void					_IncreaseScroll(eMRL_VSCR eScrollType);
    void					_DecreaseScroll(eMRL_VSCR eScrollType);

    void                    tree_controller_message_proc_check();

public:
    void					SetAreaList();
    void					SetMissionList();
    void					UpdateMissionText();    
    void					UpdatePageText();

    void					UpdateRankList();
    void					UpdateMemberList();

    void					UpdateFinishDay();

private:
    void                    all_tree_controller_reflresh();
private:
    void                    group_infomation_enable(const bool& isShow);
    void                    list_control_ptr_array_clear(); // tree_control_list만 빼고 모두 deleteAll
public:
    bool                    Is_setting_finish_day() { return Is_setting_finish_day_; }
    void                    set_Is_setting_finish_day(const bool& Is_setting_finish_day) { Is_setting_finish_day_ = Is_setting_finish_day; }
    eRANKING_SYSTEM_TYPE    get_current_select_ranking_type_() { return current_select_ranking_type_; }
    void                    non_existent_my_ranking_list_control_item();
    //--------------------------------------------------------------------------
    // dialog functions
    //
protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // texture_animation functions
    //
private:
    void                    wait_play_animation();
    void                    reward_notice_play_animation();
public:
    void                    set_reward_yellow_texture_show( BOOL Is_show );

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:    
    uiMissionRankMan2*     uimission_rank_manager_ptr_;

    CCtrlStaticWZ*          text_control_ptr_array_[POS_TXT_SIZE];    
    CCtrlListWZ*            list_control_ptr_array_[POS_LST_SIZE];  

    CCtrlButtonWZ*          button_control_ptr_array_[POS_BTN_SIZE];

    CCtrlVScrollWZ*         scroll_control_ptr_array_[POS_VSCR_SIZE];

    CCtrlStaticWZ*          title_text_ptr_;
    CCtrlButtonWZ*          list_button_ptr_array_[POS_LST_BTN_SIZE];
    
    bool                    Is_controls_initialized_;    
    bool                    Is_show_;    
    
    MAPCODE                 mission_number_;        // 미션맵번호

    CCtrlButtonCheckWZ*    	week_button_control_ptr_;
    CCtrlButtonCheckWZ*    	preweek_button_control_ptr_;

    TreeController*         tree_controller_ptr_;
    std::vector<HTREEITEM>  group_tree_item_;
    CCtrlListWZ*            tree_list_control_ptr_;
    eRANKING_SYSTEM_TYPE    current_select_ranking_type_;
    int                     max_group_;

    // 처음에는 결산일만 초기화 해준다.
    bool                    Is_setting_finish_day_;
    int                     m_iRanking_Max_Type;
private:    
    int                     reward_texture_alpha_color_;
    CTimerSimple            texture_timer_;
    HANDLE                  wait_texture_;
    FRECT                   texture_rectangles_[ANIMATION_FRAME];
    CTimerSimple            wait_animation_timer_;    
    eCOLOR_STATE            color_state_;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiMissionRankList2::getCtrlIDToPos( WzID wzId )
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
uiMissionRankList2::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiMissionRankMan2*
uiMissionRankList2::get_ui_rank_manager_ptr_()
{
    return this->uimission_rank_manager_ptr_;
}
//------------------------------------------------------------------------------
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM