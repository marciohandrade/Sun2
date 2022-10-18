#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiMissionRankList2.h"
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "RankingDefine.h"
#include "RankingManager.h"
#include "Application.h"

#define MAX_SUB_MISSION_COUNT	5

const int c_iDefaultRankLine = 3;
const int c_iDefaultMemberLine = 4;

#ifdef _INTERNATIONAL_UI
const DWORD c_iListFont = StrToWzID("st10"); 
#else
const DWORD c_iListFont = StrToWzID("m212"); 
#endif//_INTERNATIONAL_UI
#include "Mouse.h"

const DWORD c_ListSelBarColor	       = RGBA(0,51,102,100);
const DWORD c_ListBKColor		       = RGBA(255,255,255,0);
const DWORD c_ListTextColor		       = RGBA(255,255,255,255);
const DWORD c_ListSelTextColor	       = RGBA(255,255,255,255);
const DWORD c_ListNonExistentTextColor = RGBA(255, 0, 0, 255);

const int   c_max_scroll_range = 5;
// 내림차순
inline BOOL MissionRankingListGroupSort_Funter::operator()(RankingManager::LEVEL_AREA_BASE* lhs, RankingManager::LEVEL_AREA_BASE* rhs) const
{
    return ((lhs->m_GroupOrder)<(rhs->m_GroupOrder));
}

// 내림차순
inline BOOL MissionRankingListMissionSort_Funter::operator()(RankingManager::LEVEL_AREA_BASE* lhs, RankingManager::LEVEL_AREA_BASE* rhs) const
{
    return ((lhs->m_MissionOrder)<(rhs->m_MissionOrder));
}

//------------------------------------------------------------------------------
WzID uiMissionRankList2::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	// 종료						//DLGPOS_MRL_BTN_EXIT,

        StrToWzID("T002"),	// 결산일					//DLGPOS_MRL_TXT_DATE,
        StrToWzID("ST00"),  // 내정보                   //DLGPOS_MRL_TXT_MY_RANKING
        StrToWzID("T200"),	// 페이지 표시				//DLGPOS_MRL_TXT_PAGE,

        StrToWzID("LS00"),	// 명예의 전당 리스트		//DLGPOS_MRL_LST_FAME,
        StrToWzID("L001"),	// 주간순위 리스트			//DLGPOS_MRL_LST_WEEK,
        StrToWzID("L005"),	// 전 주간순위 리스트		//DLGPOS_MRL_LST_PREWEEK,
        StrToWzID("L000"),  // 트리 컨트롤러 리스트     //DLGPOS_MRL_LST_TREE_CONTROL
        StrToWzID("L002"),	// 구성원 정보 리스트		//DLGPOS_MRL_LST_MEMBER,

        StrToWzID("S001"),	// 주간순위 스크롤			//DLGPOS_MRL_VSCR_WEEK,
        StrToWzID("S002"),	// 전 주간순위 스크롤		//DLGPOS_MRL_VSCR_PREWEEK,
        StrToWzID("V000"),  // 트리 스크롤              //DLGPOS_MRL_VSCR_TREE,
        StrToWzID("S003"),	// 구성원 스크롤			//DLGPOS_MRL_VSCR_MEMBER,
        
        StrToWzID("B104"),	// 보상받기 버튼			//DLGPOS_MRL_BTN_REWARD,
        StrToWzID("B000"),	// 닫기 버튼				//DLGPOS_MRL_BTN_CLOSE,
        StrToWzID("B105"),	// 페이지 인덱스 감소 버튼(<-)		//DLGPOS_MRL_BTN_PAGE_LEFT,
        StrToWzID("B106"),	// 페이지 인덱스 증가 버튼(->)		//DLGPOS_MRL_BTN_PAGE_RIGHT,

        StrToWzID("T001"),	// DLGPOS_MRL_TXT_TITLE,                                         

        StrToWzID("B002"),	// DLGPOS_MRL_BTN_NAME1,
        StrToWzID("BT01"),	// DLGPOS_MRL_BTN_NAME2,
        StrToWzID("B101"),	// DLGPOS_MRL_BTN_NAME3,

        StrToWzID("B005"),	// DLGPOS_MRL_BTN_POINT1,
        StrToWzID("B003"),	// DLGPOS_MRL_BTN_POINT2,
        StrToWzID("B103"),	// DLGPOS_MRL_BTN_POINT3,

        StrToWzID("CT00"),
        StrToWzID("CT01"),

        StrToWzID("BT00"),  // DLGPOS_MRL_STATIC_RATE
        StrToWzID("BT01"),  // DLGPOS_MRL_STATIC_PARTY_NUMBER
        StrToWzID("BT02"),  // DLGPOS_MRL_STATIC_LEVEL
        StrToWzID("B003"),  // DLGPOS_MRL_STATIC_TIME

        StrToWzID("BT07"), // DLGPOS_MRL_STATIC_GUILD_LEVEL
        StrToWzID("BT06"), // DLGPOS_MRL_STATIC_GUILD
        StrToWzID("BT04"), // DLGPOS_MRL_STATIC_NAME

        StrToWzID("B002"), // DLGPOS_MRL_STATIC_MY_INFO_PARTY_NUMBER
        StrToWzID("B004"), // DLGPOS_MRL_STATIC_MY_INFO_LEVEL
        
        StrToWzID("P007"),  //DLGPOS_MRL_PICTURE_SEARCH_SCROLL_PICTURE,

        StrToWzID("P005"),  //DLGPOS_MRL_PICTURE_GROUP_INFOMATION_PICTURE,
        StrToWzID("T004"),  //DLGPOS_MRL_TXT_GROUP_INFOMATION_TITLE,
        StrToWzID("P014"),  //DLGPOS_MRL_PICTURE_GROUP_INFOMATION_BACKGROUND,
        StrToWzID("P016"),  //DLGPOS_MRL_PICTURE_VSCR_MEMBER_FRAME,
        StrToWzID("P010"),  //DLGPOS_MRL_PICTURE_REWARD_YELLOW_LINE,
        StrToWzID("T003"),  //DLGPOS_MRL_TXT_SETTLEDATE_TITLE,
        StrToWzID("T007"),  //DLGPOS_MRL_TXT_MISSION_TITLE,
};

//------------------------------------------------------------------------------
/**
*/
uiMissionRankList2::uiMissionRankList2(InterfaceManager* ui_manager_ptr) :
    uiBase(ui_manager_ptr),
    Is_controls_initialized_(false),
    tree_controller_ptr_(NULL),    
    tree_list_control_ptr_(NULL),
    current_select_ranking_type_(eRANKING_TYPE_NONE),
    current_sel_mapinfo_(NULL),
    Is_setting_finish_day_(false),    
    wait_texture_(INVALID_HANDLE_VALUE),
    max_group_(0)
{
    this->uimission_rank_manager_ptr_ = GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
    assert (uimission_rank_manager_ptr_);

    if (!this->uimission_rank_manager_ptr_)
    {
#ifdef _DEBUG
        uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if(pSystemMan)
            pSystemMan->Sys_VerifyLock(_T("uimission_rank_manager_ptr_이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

    Is_show_        = false;
    mission_number_ = 0;
    reward_texture_alpha_color_ = 255;
    color_state_ = eCOLOR_STATE_DEC;
}

//------------------------------------------------------------------------------
/**
*/
uiMissionRankList2::~uiMissionRankList2()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);    

    m_iRanking_Max_Type = eRANKING_TYPE_MAX;

    // GroupOrder 최대값으로 셋팅 해주어야 한다.
    const RankingManager::MISSION_LEVEL_AREA_HASH& area_hash = GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();
    RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator iter = area_hash.begin();

    for ( ; iter != area_hash.end(); ++iter )
    {
        RankingManager::LEVEL_AREA_BASE* area_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(iter->second);

        if ( area_ptr )
        {
            if (area_ptr->m_GroupOrder > max_group_)
            {
                max_group_ = area_ptr->m_GroupOrder;
            }
        }
    }    
    group_tree_item_.reserve(max_group_+1);
    for (int i = 0; i < max_group_+1; ++i)
    {
        group_tree_item_.push_back(NULL);
    }

    this->InitControls();

    // 기본적인 텍스쳐를 로딩해 놓는다.    
    if (wait_texture_ == INVALID_HANDLE_VALUE)
    {
        wait_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Loading.tga",
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }

    // 애니메이션 텍스쳐 위치 (12프레임) 프레임당 (40 * 40) 4 x 3 개
    int index = 0;
    float width = 0.0f;
    float height = 0.0f;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            width = j * ANIMATION_FRAME_WIDTH;
            height = i * ANIMATION_FRAME_HEIGHT;

            texture_rectangles_[index].left = width;
            texture_rectangles_[index].top = height;
            texture_rectangles_[index].right = ANIMATION_FRAME_WIDTH;
            texture_rectangles_[index].bottom = ANIMATION_FRAME_HEIGHT;

            ++index;
            if (index >= ANIMATION_FRAME)
            {
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::Release()
{
    if (tree_controller_ptr_)
    {
        tree_controller_ptr_->Release();

        delete tree_controller_ptr_;
        tree_controller_ptr_ = NULL;
    }
    tree_list_control_ptr_ = NULL;

    if (wait_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(wait_texture_);
        wait_texture_ = INVALID_HANDLE_VALUE;
    }

    group_tree_item_.clear();

    rank_data_list_clear();    
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::InitControls()
{
    int i = 0;
    for ( i = 0; i < POS_TXT_SIZE; ++i )
    {
        text_control_ptr_array_[i] = NULL;
        text_control_ptr_array_[i] = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_DATE + i));
    }

    for ( i = 0; i < POS_LST_SIZE; ++i )
    {
        list_control_ptr_array_[i] = NULL;
        list_control_ptr_array_[i] = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_MRL_LST_FAME + i));        
    }

    for ( i = 0; i < POS_BTN_SIZE; ++i )
    {
        button_control_ptr_array_[i] = NULL;
        button_control_ptr_array_[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_BTN_REWARD + i));
    }

    for ( i = 0; i<POS_VSCR_SIZE; ++i )
    {
        scroll_control_ptr_array_[i] = NULL;
        scroll_control_ptr_array_[i] = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_MRL_VSCR_WEEK + i));
    }

    if ( scroll_control_ptr_array_[MRL_VSCR_PREWEEK] )
        scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->ShowWindowWZ( WZ_SHOW );


    title_text_ptr_ = NULL;
    title_text_ptr_ = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_TITLE));
    if ( title_text_ptr_ )
    {
        // 2754 %s 월드 랭킹
        TCHAR title_name[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(2754, 
            title_name, 
            INTERFACE_STRING_LENGTH);
        title_text_ptr_->SetTextWZA(title_name);
    }

    title_text_ptr_ = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_SETTLEDATE_TITLE));
    if ( title_text_ptr_ )
    {
        // 1639 %s 결산일
        TCHAR title_name[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(1639, 
            title_name, 
            INTERFACE_STRING_LENGTH);
        title_text_ptr_->SetTextWZA(title_name);
    }

    title_text_ptr_ = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_MISSION_TITLE));
    if ( title_text_ptr_ )
    {
        // 9003 %s 임무
        TCHAR title_name[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(9003, 
            title_name, 
            INTERFACE_STRING_LENGTH);
        title_text_ptr_->SetTextWZA(title_name);
    }

    for( i = 0; i < POS_LST_BTN_SIZE; ++i )
    {
        list_button_ptr_array_[i] = NULL;
        list_button_ptr_array_[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_BTN_NAME1 + i));        
    }

    week_button_control_ptr_	= static_cast<CCtrlButtonCheckWZ *>( getControl( DLGPOS_MRL_BTN_WEEK ) );
    preweek_button_control_ptr_	= static_cast<CCtrlButtonCheckWZ *>( getControl( DLGPOS_MRL_BTN_PREWEEK ) );    
    
    CCtrlStaticWZ* static_control_ptr = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_PICTURE_SEARCH_SCROLL_PICTURE));
    if ( static_control_ptr )
    {
        static_control_ptr->ShowWindowWZ(WZ_SHOW);        
    }
    
    static_control_ptr = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_MY_RANKING));
    if ( static_control_ptr )
    {
        TCHAR tex[INTERFACE_STRING_LENGTH] = {0, };
        // 2783 %s 내 캐릭터 순위
        g_InterfaceManager.GetInterfaceString( 
            2783,
            tex, 
            INTERFACE_STRING_LENGTH );
        static_control_ptr->SetTextWZA(tex);
    }
    
    CCtrlButtonWZ* button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_BTN_CLOSE));
    if ( button_control_ptr )
    {
        TCHAR tex[INTERFACE_STRING_LENGTH] = {0, };
        // 5 %s 닫기
        g_InterfaceManager.GetInterfaceString( 5, tex, INTERFACE_STRING_LENGTH );
        button_control_ptr->SetTextWZA(tex);
    }

    button_control_ptr = static_cast<CCtrlButtonWZ*>(this->getControl(DLGPOS_MRL_BTN_REWARD));
    if ( button_control_ptr )
    {
        TCHAR tex[INTERFACE_STRING_LENGTH] = {0, };
        // 1793 %s 보상받기
        g_InterfaceManager.GetInterfaceString( 1793, tex, INTERFACE_STRING_LENGTH );
        button_control_ptr->SetTextWZA(tex);
    }

    tree_list_control_ptr_ = 
        static_cast<CCtrlListWZ*> (this->getControl(DLGPOS_MRL_LST_TREE_CONTROL));

    if ( tree_list_control_ptr_ )
    {
        tree_list_control_ptr_->ShowWindowWZ(WZ_SHOW);
    }

    tree_controller_ptr_ = TreeController::CreateTree(20, 20, 
        scroll_control_ptr_array_[MRL_VSCR_TREE], tree_list_control_ptr_);

    CCtrlPictureWZ* picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_REWARD_YELLOW_LINE));
    if ( picture_control_ptr )
    {
        picture_control_ptr->ShowWindowWZ(WZ_HIDE);
        picture_control_ptr->SetDiscardMessage(true);
        picture_control_ptr->SetTransprency(1.0f);
    }

    this->Is_controls_initialized_ = true;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_initRankListSelect()
{
    if ( list_control_ptr_array_[MRL_LST_FAME] )
    {
        list_control_ptr_array_[MRL_LST_FAME]->SetCurSel(-1);
    }

    if ( list_control_ptr_array_[MRL_LST_WEEK] )
    {
        list_control_ptr_array_[MRL_LST_WEEK]->SetCurSel(-1);
    }

    if ( list_control_ptr_array_[MRL_LST_TREE_CONTROL] )
    {
        list_control_ptr_array_[MRL_LST_TREE_CONTROL]->SetCurSel(-1);
    }

    if ( list_control_ptr_array_[MRL_LST_PREWEEK] )
    {
        list_control_ptr_array_[MRL_LST_PREWEEK]->SetCurSel(-1);
    }

    _initMemberList();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_initRankList()
{
    CControlWZ* rate_static_control_ptr          = getControl( DLGPOS_MRL_STATIC_RATE );
    CControlWZ* party_number_static_control_ptr  = getControl( DLGPOS_MRL_STATIC_PARTY_NUMBER );
    CControlWZ* level_static_control_ptr         = getControl( DLGPOS_MRL_STATIC_LEVEL );
    CControlWZ* time_static_control_ptr          = getControl( DLGPOS_MRL_STATIC_TIME );

    CControlWZ* name1_button_control_ptr         = getControl( DLGPOS_MRL_BTN_NAME1 );
    CControlWZ* my_info_level_static_control_ptr = getControl( DLGPOS_MRL_STATIC_MY_INFO_LEVEL );
    CControlWZ* time_button_control_ptr          = getControl( DLGPOS_MRL_BTN_POINT1 );

    if ( list_control_ptr_array_[MRL_LST_FAME] )
    {
        list_control_ptr_array_[MRL_LST_FAME]->DeleteItemAll();
        list_control_ptr_array_[MRL_LST_FAME]->SetColorSelBar(c_ListSelBarColor);
        list_control_ptr_array_[MRL_LST_FAME]->SetUseReport(true);
        list_control_ptr_array_[MRL_LST_FAME]->SetReportUseCnt(4);        
        list_control_ptr_array_[MRL_LST_FAME]->SetHideReportRender(true);

        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_FAME], 0, rate_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_FAME], 1, name1_button_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_FAME], 2, my_info_level_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_FAME], 3, time_button_control_ptr->GetSize().right, NULL );
    }

    if ( list_control_ptr_array_[MRL_LST_WEEK] )
    {
        list_control_ptr_array_[MRL_LST_WEEK]->DeleteItemAll();
        list_control_ptr_array_[MRL_LST_WEEK]->SetColorSelBar(c_ListSelBarColor);
        list_control_ptr_array_[MRL_LST_WEEK]->SetUseReport(true);
        list_control_ptr_array_[MRL_LST_WEEK]->SetReportUseCnt(4);
        list_control_ptr_array_[MRL_LST_WEEK]->SetHideReportRender(true);

        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_WEEK], 0, rate_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_WEEK], 1, party_number_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_WEEK], 2, level_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_WEEK], 3, time_static_control_ptr->GetSize().right, NULL );
    }

    if ( list_control_ptr_array_[MRL_LST_PREWEEK] )
    {
        list_control_ptr_array_[MRL_LST_PREWEEK]->DeleteItemAll();
        list_control_ptr_array_[MRL_LST_PREWEEK]->SetColorSelBar(c_ListSelBarColor);
        list_control_ptr_array_[MRL_LST_PREWEEK]->SetUseReport(true);
        list_control_ptr_array_[MRL_LST_PREWEEK]->SetReportUseCnt(4);
        list_control_ptr_array_[MRL_LST_PREWEEK]->SetHideReportRender(true);

        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_PREWEEK], 0, rate_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_PREWEEK], 1, party_number_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_PREWEEK], 2, level_static_control_ptr->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_PREWEEK], 3, time_static_control_ptr->GetSize().right, NULL );
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_initMemberList()
{
    if ( list_control_ptr_array_[MRL_LST_MEMBER] )
    {
        list_control_ptr_array_[MRL_LST_MEMBER]->DeleteItemAll();
        list_control_ptr_array_[MRL_LST_MEMBER]->SetColorSelBar(c_ListSelBarColor);
        list_control_ptr_array_[MRL_LST_MEMBER]->SetUseReport(true);
        list_control_ptr_array_[MRL_LST_MEMBER]->SetReportUseCnt(3);
        list_control_ptr_array_[MRL_LST_MEMBER]->SetHideReportRender(true);
        list_control_ptr_array_[MRL_LST_MEMBER]->SetSellHeight(21);

        list_control_ptr_array_[MRL_LST_MEMBER]->SetCurSel(-1);

        if ( scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
        {
            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollRangeWZ(0,0);	//스크롤 범위
            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_initPageSet()
{
    if ( list_control_ptr_array_[MRL_LST_WEEK] )
        list_control_ptr_array_[MRL_LST_WEEK]->SetCurSel(-1);

    _initMemberList();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_prepareDisplay()
{
    current_sel_mapinfo_         = NULL;
    current_select_ranking_type_ = eRANKING_TYPE_NONE;
    mission_number_              = 0;
    Is_show_                     = true;   

    _setTitle();
    SetAreaList();
    _initRankListSelect();

    // 이번주 주간 순위
    if ( scroll_control_ptr_array_[MRL_VSCR_WEEK] )
    {
        scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollPosWZ(0);
    }

    // 지난주 주간 순위
    if ( scroll_control_ptr_array_[MRL_VSCR_PREWEEK] )
    {
        scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->SetScrollPosWZ(0);
    }

    // 구성원 정보
    if ( scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
    {
        scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
    }

    get_ui_rank_manager_ptr_()->ClearRankData();
    UpdateMissionText();
    _initRankList();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::OnShowWindow( BOOL val )
{
    if ( val )
    {
        if ( !this->Is_controls_initialized_ )
        {
            this->ShowInterface(FALSE);
            return;
        }

        // 결산일 요청을 위해서 false로 돌려 놓는다.
        set_Is_setting_finish_day(false);
        
        texture_timer_.Off();
        texture_timer_.On(1);
        wait_animation_timer_.Off();
        wait_animation_timer_.On(1);

        _prepareDisplay();
        // 구성원 정보 비활성화
        group_infomation_enable(false);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_mission_rank_list;
        msg.wParam = IM_MISSION_RANK_MANAGER::MissionRankList;        
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        
        if ( week_button_control_ptr_ )
            week_button_control_ptr_->SetCheckState( TRUE );

        if ( preweek_button_control_ptr_ )
            preweek_button_control_ptr_->SetCheckState( FALSE );

        list_control_ptr_array_[MRL_LST_WEEK]->ShowWindowWZ( WZ_SHOW );
        list_control_ptr_array_[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_HIDE );
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_rank_list);
    }
 }

//------------------------------------------------------------------------------
/**
*/
BOOL uiMissionRankList2::_isEnable()
{
    if ( GENERALPARAM->IsNet() )
    {
        if ( this->IsLock() )
            return FALSE;

        if(!g_pHero)
            return FALSE;

        if ( g_pHero && g_pHero->IsShowFullMap() )
            return FALSE;

        if ( !get_ui_rank_manager_ptr_() )
            return FALSE;

        return TRUE;
    }
    else
    {
        return TRUE;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;    

    CCtrlListWZ* item = static_cast<CCtrlListWZ*>(this->getControl(DLGPOS_MRL_LST_TREE_CONTROL));
    if ( tree_controller_ptr_ && item )
    {
        RECT pos   = item->GetSizeRect();
        RECT dumi  = scroll_control_ptr_array_[MRL_VSCR_TREE]->GetSizeRect();
        int mouse_point_x = Mouse::Instance()->GetMouseX();
        int mouse_point_y = Mouse::Instance()->GetMouseY();

        if ( pos.left < mouse_point_x && /*pos.right*/ dumi.right > mouse_point_x &&
            pos.top < mouse_point_y && /*pos.bottom*/ dumi.bottom > mouse_point_y )
        {
            tree_controller_ptr_->MessageProc(pMessage);
        }
    }

    tree_controller_message_proc_check();

    switch( pMessage->eResultMsg )
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch ( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_MRL_BTN_EXIT:
            case DLGPOS_MRL_BTN_CLOSE:
                {
                    Is_show_ = false;
                }
                break;

            case DLGPOS_MRL_BTN_PAGE_LEFT:
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
#else
                    if ( preweek_button_control_ptr_->GetCheckState() == TRUE )
                    {
                        break;
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                    if ( get_ui_rank_manager_ptr_()->DecPage() == true )
                    {
                        _initRankListSelect();
                        UpdatePageText();
                    }
                }
                break;

            case DLGPOS_MRL_BTN_PAGE_RIGHT:
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
#else
                    if ( preweek_button_control_ptr_->GetCheckState() == TRUE )
                    {
                        break;
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                    if ( get_ui_rank_manager_ptr_()->IncPage() == true )
                    {
                        _initRankListSelect();
                        UpdatePageText();
                    }
                }
                break;

            case DLGPOS_MRL_BTN_REWARD:
                {
                    if ( get_ui_rank_manager_ptr_()->get_Is_wait_receive_packet_() == true )
                        break;

                    if ( get_ui_rank_manager_ptr_()->get_Is_can_reward_() == true )
                    {
                        get_ui_rank_manager_ptr_()->Send_CG_MISSION_RANKING_REWARD_SYN();
                    }
                    else
                    {
                        // 보상받을 수 없는 경우의 메시지 처리
                        // 이 부분은 어떻게 할지 고민 좀 해보자.
                        // 5906	보상 대상이 아닙니다.
                        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
                        g_InterfaceManager.GetInterfaceString(5906, szMessage, INTERFACE_STRING_LENGTH);                        
                        GlobalFunc::OutputMsg_SysMsg(szMessage);
                    }
                }
                break;

            case DLGPOS_MRL_LST_FAME:
                {
                    list_control_ptr_array_[MRL_LST_WEEK]->SetCurSel(-1);
                    list_control_ptr_array_[MRL_LST_PREWEEK]->SetCurSel(-1);
                    
                    if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
                    {
                        list_control_ptr_array_[MRL_LST_MEMBER]->SetCurSel(-1);
                    }
                    
                    if ( current_select_ranking_type_ != eRANKING_TYPE_TIME_ATTACK ) //미션랭킹
                    {
                        break;
                    }

                    int index_number = list_control_ptr_array_[MRL_LST_FAME]->GetCurSel();

                    if(index_number >= 0)
                    {                        
                        get_ui_rank_manager_ptr_()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(mission_number_, index_number, eRANKING_MYRANKING);
                    }
                }
                break;

            case DLGPOS_MRL_LST_WEEK:
                {
                    list_control_ptr_array_[MRL_LST_FAME]->SetCurSel(-1);
                    list_control_ptr_array_[MRL_LST_PREWEEK]->SetCurSel(-1);

                    if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
                    {
                        list_control_ptr_array_[MRL_LST_MEMBER]->SetCurSel(-1);
                    }

                    if ( current_select_ranking_type_ != eRANKING_TYPE_TIME_ATTACK ) //미션랭킹
                    {
                        break;
                    }

                    int index_number = list_control_ptr_array_[MRL_LST_WEEK]->GetCurSel();

                    if (index_number >= 0 )
                    {
                        get_ui_rank_manager_ptr_()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(mission_number_, index_number, eRANKING_TODAYWEEK);
                    }
                }
                break;

            case DLGPOS_MRL_LST_PREWEEK:
                {
                    list_control_ptr_array_[MRL_LST_FAME]->SetCurSel(-1);
                    list_control_ptr_array_[MRL_LST_WEEK]->SetCurSel(-1);

                    if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
                    {
                        list_control_ptr_array_[MRL_LST_MEMBER]->SetCurSel(-1);
                    }

                    if ( current_select_ranking_type_ != eRANKING_TYPE_TIME_ATTACK ) //미션랭킹
                    {
                        break;
                    }

                    int index_number = list_control_ptr_array_[MRL_LST_PREWEEK]->GetCurSel();

                    if ( index_number >= 0 )
                    {
                        get_ui_rank_manager_ptr_()->Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(mission_number_, index_number, eRANKING_LASTWEEK);
                    }
                }
                break;

            case DLGPOS_MRL_LST_MEMBER:
                {
                }
                break;

            case DLGPOS_MRL_BTN_WEEK:
                {                    
                    week_button_control_ptr_->SetCheckState( TRUE );
                    preweek_button_control_ptr_->SetCheckState( FALSE );

                    list_control_ptr_array_[MRL_LST_WEEK]->ShowWindowWZ( WZ_SHOW );
                    list_control_ptr_array_[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_HIDE );                    
                    
                    _initMemberList();
                    UpdateRankList();
                }
                break;

            case DLGPOS_MRL_BTN_PREWEEK:
                {
                    week_button_control_ptr_->SetCheckState( FALSE );
                    preweek_button_control_ptr_->SetCheckState( TRUE );

                    list_control_ptr_array_[MRL_LST_WEEK]->ShowWindowWZ( WZ_HIDE );
                    list_control_ptr_array_[MRL_LST_PREWEEK]->ShowWindowWZ( WZ_SHOW );

                    get_ui_rank_manager_ptr_()->UpdateReceiveData(current_select_ranking_type_, mission_number_, 0);
                    UpdatePageText();
                    
                    _initMemberList();
                    UpdateRankList();
                }
                break;
            }
        }
        break;

    case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_MRL_VSCR_WEEK:
                {
                    if ( scroll_control_ptr_array_[MRL_VSCR_WEEK] )
                    {
                        int pos = scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollPosWZ();      //현재 스크롤 값을 받아 온다.
                        int mmin = 0 , mmax = 0;
                        scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

                        if ( pos > mmin )// 현재 입력된 라인수 보다 작으면
                        {
                            pos--;//한줄 감소 시키고 
                            scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollPosWZ(pos);
                        }
                    }
                }
                break;

            case DLGPOS_MRL_VSCR_MEMBER:
                {
                    if ( scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
                    {
                        int pos = scroll_control_ptr_array_[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
                        int mmin = 0 , mmax = 0;
                        scroll_control_ptr_array_[MRL_VSCR_MEMBER]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

                        if ( pos > mmin )// 현재 입력된 라인수 보다 작으면
                        {
                            pos--;//한줄 감소 시키고
                            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollPosWZ(pos);
                        }
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
        {
            switch ( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_MRL_VSCR_WEEK:
                {
                    if ( scroll_control_ptr_array_[MRL_VSCR_WEEK] )
                    {
                        int pos = scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
                        int mmin = 0 , mmax = 0;
                        scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

                        if ( pos < mmax ) // 현재 입력된 라인수 보다 작으면
                        {
                            pos++;//한줄 증가 시키고 
                            scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollPosWZ(pos);
                        }
                    }
                }
                break;

            case DLGPOS_MRL_VSCR_PREWEEK:
                {
                    if ( scroll_control_ptr_array_[MRL_VSCR_PREWEEK] )
                    {
                        int pos = scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
                        int mmin = 0 , mmax = 0;
                        scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

                        if ( pos < mmax )// 현재 입력된 라인수 보다 작으면
                        {
                            pos++;//한줄 증가 시키고 
                            scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->SetScrollPosWZ(pos);
                        }
                    }
                }
                break;

            case DLGPOS_MRL_VSCR_MEMBER:
                {
                    if ( scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
                    {
                        int pos = scroll_control_ptr_array_[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
                        int mmin = 0 , mmax = 0;
                        scroll_control_ptr_array_[MRL_VSCR_MEMBER]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

                        if ( pos < mmax )// 현재 입력된 라인수 보다 작으면
                        {
                            pos++;//한줄 증가 시키고
                            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollPosWZ(pos);
                        }
                    }
                }
                break;
            }
        }
        break;

    default:
        {
            SolarDialog::MessageProc( pMessage );
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::OnUpdateSolarDialog()
{
    if ( !Is_show_ )
    {
        ShowInterface(FALSE);
        return;
    }

    if ( !_isEnable() )
        return;

    //infomation_received_check();

    int current_scroll_pos = 0;
    int start_draw_index   = 0;

    if (get_ui_rank_manager_ptr_()->get_Is_wait_receive_packet_() == true)
    {
        return;
    }

    if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
    {
        if ( week_button_control_ptr_->GetCheckState() )
        {
            if ( list_control_ptr_array_[MRL_LST_WEEK] && scroll_control_ptr_array_[MRL_VSCR_WEEK] )
            {
                current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
                start_draw_index = current_scroll_pos;
                _UpdateMouseWheel(MRL_LST_WEEK);
                list_control_ptr_array_[MRL_LST_WEEK]->SetBeginDrawIndex( start_draw_index );
            }
        }
        else
        {
            if ( list_control_ptr_array_[MRL_LST_PREWEEK] && scroll_control_ptr_array_[MRL_VSCR_WEEK] )
            {
                current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
                start_draw_index   = current_scroll_pos;
                _UpdateMouseWheel(MRL_LST_PREWEEK);
                list_control_ptr_array_[MRL_LST_PREWEEK]->SetBeginDrawIndex( start_draw_index );
            }
        }
        
        if ( list_control_ptr_array_[MRL_LST_TREE_CONTROL] )
        {
            current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_TREE]->GetScrollPosWZ(); // 현재 스크롤 값을 받아 온다
            start_draw_index = current_scroll_pos;
            _UpdateMouseWheel(MRL_LST_TREE_CONTROL);
            list_control_ptr_array_[MRL_LST_TREE_CONTROL]->SetBeginDrawIndex( start_draw_index );
        }
    }
    else
    {
        if ( list_control_ptr_array_[MRL_LST_WEEK] && scroll_control_ptr_array_[MRL_VSCR_WEEK] )
        {
            current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_WEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
            start_draw_index = current_scroll_pos;
            _UpdateMouseWheel(MRL_LST_WEEK);
            list_control_ptr_array_[MRL_LST_WEEK]->SetBeginDrawIndex( start_draw_index );
        }

        // 지난주 주간 순위
        if ( list_control_ptr_array_[MRL_LST_PREWEEK] && scroll_control_ptr_array_[MRL_VSCR_PREWEEK] )
        {
            current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_PREWEEK]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
            start_draw_index = current_scroll_pos;
            _UpdateMouseWheel(MRL_LST_PREWEEK);
            list_control_ptr_array_[MRL_LST_PREWEEK]->SetBeginDrawIndex( start_draw_index );
        }
    }

    // 구성원 정보
    if ( list_control_ptr_array_[MRL_LST_MEMBER] && scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
    {
        current_scroll_pos = scroll_control_ptr_array_[MRL_VSCR_MEMBER]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
        start_draw_index = current_scroll_pos;
        _UpdateMouseWheel(MRL_LST_MEMBER);
        list_control_ptr_array_[MRL_LST_MEMBER]->SetBeginDrawIndex( start_draw_index );
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::SetAreaList()
{
    if ( !get_ui_rank_manager_ptr_() )
        return;

    rank_data_list_clear();

    // TODO : 
    // Area 그룹을 하기전에 소팅을 해야 한다.
    if ( tree_controller_ptr_ )
    {
        std::vector<RankingManager::LEVEL_AREA_BASE*> ranking_group_vec;

        std::map<int, bool> group_check;
        group_check.clear();
        
        std::list<WORD> map_code;
        int count = (GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable()).size();

        tree_controller_ptr_->DeleteAllItem();

        const RankingManager::MISSION_LEVEL_AREA_HASH& area_hash = GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();
        RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator iter = area_hash.begin();


        for ( ; iter != area_hash.end(); ++iter )
        {
            RankingManager::LEVEL_AREA_BASE* area_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(iter->second);

            if ( area_ptr )
            {
                ranking_group_vec.push_back(area_ptr);

                std::map<int, bool>::iterator iter;
                iter = group_check.find(area_ptr->m_GroupOrder);
                if (iter == group_check.end())
                    group_check.insert(make_pair<int, bool>(area_ptr->m_GroupOrder, false));
            }
        }        

        // GroupSort
        static MissionRankingListGroupSort_Funter mission_ranking_list_group_sort_funter;
        if ( !ranking_group_vec.empty() )
            sort(ranking_group_vec.begin(), ranking_group_vec.end(), mission_ranking_list_group_sort_funter);
        

        for ( unsigned int i = 0; i < ranking_group_vec.size(); ++i )
        {
            TCHAR area_name[INTERFACE_STRING_LENGTH] = {0, };
            RankingManager::LEVEL_AREA_BASE* area_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(ranking_group_vec[i]);            

            if ( area_ptr )
            {
                std::map<int, bool>::iterator iter;
                iter = group_check.find(area_ptr->m_GroupOrder);
                if (iter != group_check.end() && group_check[area_ptr->m_GroupOrder] == true)
                {
                    continue;
                }

                ZeroMemory(area_name, sizeof(area_name));
                g_InterfaceManager.GetInterfaceString( 
                    area_ptr->m_GroupString, 
                    area_name, 
                    INTERFACE_STRING_LENGTH );

                group_check[area_ptr->m_GroupOrder] = true;
                map_code.push_back(area_ptr->m_MissionNo);

                group_tree_item_[area_ptr->m_GroupOrder] = 
                    tree_controller_ptr_->AddTreeItem(area_name,
                    0,
                    0,
                    true,
                    StrToWzID("st10"),
                    NULL,
                    NULL);
            }
        }
        group_check.clear();
    }
    SetMissionList();
}

//------------------------------------------------------------------------------
/** 마우스 휠처리
*/
void uiMissionRankList2::_UpdateMouseWheel(eMRL_LST eListType)
{
    RECT rc = list_control_ptr_array_[eListType]->GetSizeRect();

    eMRL_VSCR scroll_type = MRL_VSCR_WEEK;

    switch ( eListType )
    {
    case MRL_LST_PREWEEK:scroll_type=MRL_VSCR_PREWEEK;	break;
    case MRL_LST_TREE_CONTROL:scroll_type=MRL_VSCR_TREE; break;
    case MRL_LST_MEMBER: scroll_type=MRL_VSCR_MEMBER;	break;
    }

    switch ( GetMouseWheelStatus(&rc) )
    {
    case MOUSE_WHEEL_UP:  _DecreaseScroll(scroll_type);	break;

    case MOUSE_WHEEL_DOWN:_IncreaseScroll(scroll_type);	break;
    }
}
//------------------------------------------------------------------------------
/** 
*/
void uiMissionRankList2::_IncreaseScroll(eMRL_VSCR eScrollType)
{
    if(scroll_control_ptr_array_[eScrollType])
    {
        int pos = scroll_control_ptr_array_[eScrollType]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
        int mmin = 0 , mmax = 0;
        scroll_control_ptr_array_[eScrollType]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

        if ( pos < mmax )// 현재 입력된 라인수 보다 작으면 
        {
            pos++;//한줄 증가 시키고 
            scroll_control_ptr_array_[eScrollType]->SetScrollPosWZ(pos);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::_DecreaseScroll(eMRL_VSCR eScrollType)
{
    if ( scroll_control_ptr_array_[eScrollType] )
    {
        int pos = scroll_control_ptr_array_[eScrollType]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
        int mmin = 0 , mmax = 0;
        scroll_control_ptr_array_[eScrollType]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

        if ( pos > mmin )// 현재 입력된 라인수 보다 작으면 
        {
            pos--;//한줄 감소 시키고 
            scroll_control_ptr_array_[eScrollType]->SetScrollPosWZ(pos);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::SetMissionList()
{
    if ( !get_ui_rank_manager_ptr_() )
        return;

    // 먼저 각각의 Ranking 미션을 받아와야 한다.
    std::vector<RankingManager::LEVEL_AREA_BASE*> ranking_mission_vec;
    std::list<WORD> map_code;

    const RankingManager::MISSION_LEVEL_AREA_HASH& area_hash = GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();
    RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator iter = area_hash.begin();

    for ( ; iter != area_hash.end(); ++iter )
    {
        RankingManager::LEVEL_AREA_BASE* area_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(iter->second);

        if ( area_ptr )
        {
            ranking_mission_vec.push_back(area_ptr);
            map_code.push_back(area_ptr->m_MissionNo);
        }        
    }

    // MissionSort
    static MissionRankingListMissionSort_Funter mission_ranking_list_mission_sort_funter;
    if ( !ranking_mission_vec.empty() )
        sort(ranking_mission_vec.begin(), ranking_mission_vec.end(), mission_ranking_list_mission_sort_funter);

    // 이제 정렬까지 다 되었으면 MapInfo를 받아서 셋팅하자.
    for ( unsigned int i = 0; i < ranking_mission_vec.size(); ++i )
    {
        TCHAR mission_name[INTERFACE_STRING_LENGTH] = {0, };
        RankingManager::LEVEL_AREA_BASE* mission_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(ranking_mission_vec[i]);
        bool already = false;

        if ( mission_ptr )
        {
            _stRankData* rank_data_struct_ptr = NULL;

            const sMAPINFO* map_info_ptr = MapInfoParser::Instance()->FindMapInfo(mission_ptr->m_MissionNo);

            if ( map_info_ptr )
            {                
                rank_data_struct_ptr = new _stRankData;
                rank_data_struct_ptr->ranking_system_type = static_cast<eRANKING_SYSTEM_TYPE>(mission_ptr->m_RankingType);
                rank_data_struct_ptr->map_group_ptr = NULL;
                rank_data_struct_ptr->map_info_ptr  = map_info_ptr;
                rank_data_struct_ptr->area_data_ptr = mission_ptr;
                rank_data_list_.push_back(rank_data_struct_ptr);
            }

            ZeroMemory(mission_name, sizeof(mission_name));
            g_InterfaceManager.GetInterfaceString( 
                mission_ptr->m_MissionString, 
                mission_name, 
                INTERFACE_STRING_LENGTH );

            // 여기에 맵데이터를 물려서 사용해야 한다.
            if ( group_tree_item_[mission_ptr->m_GroupOrder] )
            {
                if ( rank_data_struct_ptr )
                {
                    HTREEITEM pItem = 
                        tree_controller_ptr_->AddTreeItemCheckBox(mission_name,
                        0,
                        0,
                        false,
                        StrToWzID("st10"),
                        rank_data_struct_ptr,
                        group_tree_item_[mission_ptr->m_GroupOrder]);
                }
            }
            else
            {
                // 맵코드가 같은 걸 찾아내서 그 랭킹 타입에 추가
                for ( unsigned int i = 0; i < ranking_mission_vec.size(); ++i )
                {
                    RankingManager::LEVEL_AREA_BASE* pre_mission_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(ranking_mission_vec[i]);

                    if ( pre_mission_ptr && (pre_mission_ptr->m_MissionNo == mission_ptr->m_MissionNo)
                        && group_tree_item_[pre_mission_ptr->m_GroupOrder] && rank_data_struct_ptr )
                    {                                                        
                        HTREEITEM pItem = 
                            tree_controller_ptr_->AddTreeItemCheckBox(mission_name,
                            0,
                            0,
                            false,
                            StrToWzID("st10"),
                            rank_data_struct_ptr,
                            group_tree_item_[pre_mission_ptr->m_GroupOrder]);                       
                    }
                }
            }
        }
    }

    // 처음에는 Tree를 닫아 놓는다.
    all_tree_controller_reflresh();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::UpdateMissionText()
{
    if ( !_isEnable() )
        return;
    
    const sMAPINFO * map_info_ptr = current_sel_mapinfo_;

    if ( map_info_ptr )
    {            
        mission_number_ = map_info_ptr->MapCode;

        get_ui_rank_manager_ptr_()->UpdateReceiveData(current_select_ranking_type_, mission_number_, 0);
    }
    else // 초기에 결산일을 요청하기 위해서
    {
        if ( Is_setting_finish_day_ == false )
        {
            // 여기에서 패킷을 보내는 구문이 필요함.
            mission_number_ = 0;
            // 결산일 요청
            get_ui_rank_manager_ptr_()->Send_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN();
        }
    }
    UpdatePageText();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::UpdateRankList()
{
    if ( !_isEnable() )
        return;

    bool Is_point_ranking = false;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    if ( current_select_ranking_type_ >= eRANKING_TYPE_POINT 
        && current_select_ranking_type_ < eRANKING_TYPE_MAX )
    {
        // 포인트 랭킹 타입 이상이면 같은 로직을 사용하기 때문에 bPointRanking으로 제어 한다.
        Is_point_ranking = true;
    }
    else if ( current_select_ranking_type_ <= eRANKING_TYPE_TIME_ATTACK 
        && current_select_ranking_type_ > eRANKING_TYPE_NONE )
    {

    }
    else
    {
        // 여기에 들어오지 말아야 한다.
        return;
    }
#else
    if ( current_select_ranking_type_ == eRANKING_TYPE_POINT )
    {
        Is_point_ranking = true;
    }
    else
    {
        Is_point_ranking = false;
    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

    _initRankList();

    // 명예의 전당
    if ( list_control_ptr_array_[MRL_LST_FAME] )
    {
        int index_number = 0;
        int count_number = 0;

        if(get_ui_rank_manager_ptr_()->my_ranking_view_data_.iRank > 0)	// 데이터가 있으면 리스트에 추가
        {
            index_number = list_control_ptr_array_[MRL_LST_FAME]->InsertItemBack();
            list_control_ptr_array_[MRL_LST_FAME]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->my_ranking_view_data_));

            TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

            DWORD dwStyle = 0;
            for ( count_number = 0; count_number < eFAME_LST_MAX; count_number++ )
            {
                ZeroMemory(szTemp, sizeof(szTemp));
                if ( eFAME_LST_MY_RANKING == count_number ) // 내 순위
                {
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), get_ui_rank_manager_ptr_()->my_ranking_view_data_.iRank);
                    dwStyle = DT_VCENTER | DT_CENTER;
                }
                else if ( eFAME_LST_PARTY_NUM == count_number )	// 파티번호 or 캐릭명 
                {
                    
                    if(Is_point_ranking)
                    {                        
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), get_ui_rank_manager_ptr_()->my_ranking_view_data_.strName);
                    }
                    else
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->my_ranking_view_data_.iPartyNum);
                    }                    

                    dwStyle = DT_VCENTER | DT_CENTER;
                }
                else if ( eFAME_LST_LEVEL == count_number )	// 레벨
                {
                    Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), get_ui_rank_manager_ptr_()->my_ranking_view_data_.iLevel);
                    dwStyle = DT_VCENTER | DT_CENTER;
                }
                else if ( eFAME_LST_TIME == count_number )	// 진행시간 or 포인트
                {
                    if ( Is_point_ranking )
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->my_ranking_view_data_.dwTime);
                    }
                    else
                    {
                        util::_tstring strTime;
                        get_ui_rank_manager_ptr_()->GetTimeString(get_ui_rank_manager_ptr_()->my_ranking_view_data_.dwTime, strTime);
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
                    }

                    dwStyle = DT_VCENTER | DT_CENTER;
                }

                GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_FAME], index_number, count_number,
                    szTemp,
                    StrToWzID("m212"),
                    dwStyle,
                    c_ListBKColor, 
                    c_ListTextColor, 
                    c_ListSelTextColor);
            }
        }
        else
        {
            non_existent_my_ranking_list_control_item();
        }
    }

    // 이번주 주간 순위
    if ( list_control_ptr_array_[MRL_LST_WEEK] )
    {
        int index_number = 0;
        int count_number = 0;

        DWORD dwStyle = 0;

        for ( int i = 0; i < MAX_WEEK_RANK; ++i )
        {
            if(get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].iRank > 0)	// 데이터가 있으면 리스트에 추가
            {
                index_number = list_control_ptr_array_[MRL_LST_WEEK]->InsertItemBack();
                list_control_ptr_array_[MRL_LST_WEEK]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i]));
                TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

                for ( count_number = 0; count_number < eWEEK_LST_MAX; count_number++ )
                {
                    ZeroMemory(szTemp, sizeof(szTemp));
                    if ( eWEEK_LST_RANK == count_number )				// 순위
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-4d"), get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].iRank);
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_PARTY_NUM == count_number )	// 파티번호 or 캐릭명 
                    {
                        if ( Is_point_ranking )
                        {                            
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].strName);
                        }
                        else
                        {
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].iPartyNum);
                        }
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_LEVEL == count_number )		// 레벨
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].iLevel);
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_TIME == count_number )		// 진행시간 or 포인트
                    {
                        if ( Is_point_ranking )
                        {
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].dwTime);
                        }
                        else
                        {
                            util::_tstring strTime;
                            get_ui_rank_manager_ptr_()->GetTimeString(get_ui_rank_manager_ptr_()->current_week_rank_view_data_[i].dwTime, strTime);
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
                        }
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }

                    GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_WEEK], index_number, count_number,
                        szTemp,
                        StrToWzID("m212"),
                        dwStyle,
                        c_ListBKColor, 
                        c_ListTextColor, 
                        c_ListSelTextColor);
                }
            }
        }


        int iPage = 0;

        if ( index_number )
        {
            if (index_number >= c_iDefaultRankLine )
                iPage = (index_number+1)-c_iDefaultRankLine;
        }

        if ( scroll_control_ptr_array_[MRL_VSCR_WEEK] )
        {
            int scroll_range = iPage;

            if ( scroll_range > c_max_scroll_range )
            {
                scroll_range = c_max_scroll_range;
            }
                
            scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollRangeWZ(0,scroll_range);	//스크롤 범위
        }
    }

    // 지난주 주간 순위
    if ( list_control_ptr_array_[MRL_LST_PREWEEK] )
    {
        int index_number = 0;
        int count_number = 0;

        DWORD dwStyle = 0;

        for ( int i = 0; i < MAX_PREWEEK_RANK; ++i )
        {
            if(get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].iRank > 0)	// 데이터가 있으면 리스트에 추가
            {
                index_number = list_control_ptr_array_[MRL_LST_PREWEEK]->InsertItemBack();
                list_control_ptr_array_[MRL_LST_PREWEEK]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i]));
                TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

                for ( count_number = 0; count_number < eWEEK_LST_MAX; count_number++ )
                {
                    ZeroMemory(szTemp, sizeof(szTemp));
                    if ( eWEEK_LST_RANK == count_number )				// 순위
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-4d"), get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].iRank);
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_PARTY_NUM == count_number )	// 파티번호 or 캐릭명 
                    {
                        if ( Is_point_ranking )
                        {                            
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].strName);
                        }
                        else
                        {
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].iPartyNum);
                        }
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_LEVEL == count_number )		// 레벨
                    {
                        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-6d"), get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].iLevel);
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }
                    else if ( eWEEK_LST_TIME == count_number )		// 진행시간 or 포인트
                    {
                        if ( Is_point_ranking )
                        {
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10d"), get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].dwTime);
                        }
                        else
                        {
                            util::_tstring strTime;
                            get_ui_rank_manager_ptr_()->GetTimeString(get_ui_rank_manager_ptr_()->pre_week_rank_view_data_[i].dwTime, strTime);
                            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%-10s"), strTime.c_str());
                        }
                        dwStyle = DT_VCENTER | DT_CENTER;
                    }

                    GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_PREWEEK], index_number, count_number,
                        szTemp,
                        StrToWzID("m212"),
                        dwStyle,
                        c_ListBKColor, 
                        c_ListTextColor, 
                        c_ListSelTextColor);
                }
            }
        }

        int iPage = 0;

        if ( index_number )
        {
            if ( index_number >= c_iDefaultRankLine )
            {
                iPage = (index_number+1)-c_iDefaultRankLine;
            }
        }

        // 기존의 dialog 통합으로 인해서 이전주 스크롤은 공통으로 사용한다.
        // 이전주는 특수한 상황이므로 Range를 0으로 설정하고 위치를 -1로 설정한다.
        if ( preweek_button_control_ptr_->GetCheckState() == TRUE )
        {            
            scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollRangeWZ(0,0);	//스크롤 범위
            scroll_control_ptr_array_[MRL_VSCR_WEEK]->SetScrollPosWZ(-1);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::UpdatePageText()
{
    if ( !_isEnable() )
        return;

    if ( text_control_ptr_array_[MRL_TXT_PAGE] )
    {
        TCHAR message_text[INTERFACE_STRING_LENGTH] ={0,};
        int page_1_number = (get_ui_rank_manager_ptr_()->m_CurRankPage * MAX_WEEK_RANK) + 1;
        int page_2_number = (get_ui_rank_manager_ptr_()->m_CurRankPage * MAX_WEEK_RANK) + MAX_WEEK_RANK;
#ifdef _JAPAN
        Snprintf(message_text, INTERFACE_STRING_LENGTH-1, _T("%d - %d"), page_1_number, page_2_number);
#else
        Snprintf(message_text, INTERFACE_STRING_LENGTH-1, _T("%d ~ %d"), page_1_number, page_2_number);
#endif

        GlobalFunc::SetCaption( text_control_ptr_array_[MRL_TXT_PAGE], message_text);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::UpdateMemberList()
{
    if ( !_isEnable() )
        return;

    // 구성원 정보
    if ( list_control_ptr_array_[MRL_LST_MEMBER] )
    {
        int index_number = 0;
        int count_number = 0;

        _initMemberList();

        CControlWZ* guild_level_static_control = getControl( DLGPOS_MRL_STATIC_GUILD_LEVEL );
        CControlWZ* guild_static_control       = getControl( DLGPOS_MRL_STATIC_GUILD );
        CControlWZ* name_static_control        = getControl( DLGPOS_MRL_STATIC_NAME );

        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_MEMBER], 0, guild_level_static_control->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_MEMBER], 1, guild_static_control->GetSize().right, NULL );
        GlobalFunc::SetReportButtonInfo( list_control_ptr_array_[MRL_LST_MEMBER], 2, name_static_control->GetSize().right, NULL );

        for ( int i = 0; i < MAX_RANK_MEMBER; ++i )
        {
            if ( get_ui_rank_manager_ptr_()->member_view_data_[i].iLevel > 0 )
            {
                index_number = list_control_ptr_array_[MRL_LST_MEMBER]->InsertItemBack();
                list_control_ptr_array_[MRL_LST_MEMBER]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->member_view_data_[i]));
                TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};

                for ( count_number = 0; count_number < eMEMBER_LST_MAX; count_number++ )
                {
                    ZeroMemory(temp_message, sizeof(temp_message));

                    if ( eMEMBER_LST_LEVEL == count_number )			// 레벨
                    {
                        Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%d"), get_ui_rank_manager_ptr_()->member_view_data_[i].iLevel);
                    }
                    else if ( eMEMBER_LST_GUILD == count_number )		// 길드
                    {
                        //Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strGuild.c_str());
                        Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%s"), get_ui_rank_manager_ptr_()->member_view_data_[i].strGuild);
                    }
                    else if ( eMEMBER_LST_NAME == count_number )		// 이름
                    {
                        //Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_MemberData[i].strName.c_str());
                        Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%s"), get_ui_rank_manager_ptr_()->member_view_data_[i].strName);
                    }

                    GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_MEMBER], index_number, count_number,
                        temp_message,
                        StrToWzID("m212"),
                        DT_VCENTER | DT_CENTER,
                        c_ListBKColor, 
                        c_ListTextColor, 
                        c_ListSelTextColor);
                }
            }
        }

        if ( get_ui_rank_manager_ptr_()->member_view_data_[0].iLevel == 0 )
        {        
            index_number = list_control_ptr_array_[MRL_LST_MEMBER]->InsertItemBack();
            list_control_ptr_array_[MRL_LST_MEMBER]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->member_view_data_[0]));
            TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};

            // 레벨
            ZeroMemory(temp_message, sizeof(temp_message));
            Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%d"), get_ui_rank_manager_ptr_()->member_view_data_[0].iLevel);
            GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_MEMBER], index_number, eMEMBER_LST_LEVEL,
                temp_message,
                StrToWzID("m212"),
                DT_VCENTER | DT_CENTER,
                c_ListBKColor, 
                c_ListTextColor, 
                c_ListSelTextColor);

            // 길드
            ZeroMemory(temp_message, sizeof(temp_message));
            Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%s"), get_ui_rank_manager_ptr_()->member_view_data_[0].strGuild);
            GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_MEMBER], index_number, eMEMBER_LST_GUILD,
                temp_message,
                StrToWzID("m212"),
                DT_VCENTER | DT_CENTER,
                c_ListBKColor, 
                c_ListTextColor, 
                c_ListSelTextColor);

            // 이름
            ZeroMemory(temp_message, sizeof(temp_message));
            Snprintf(temp_message, INTERFACE_STRING_LENGTH-1, _T("%s"), get_ui_rank_manager_ptr_()->member_view_data_[0].strName);
            GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_MEMBER], index_number, eMEMBER_LST_NAME,
                temp_message,
                StrToWzID("m212"),
                DT_VCENTER | DT_CENTER,
                c_ListBKColor, 
                c_ListTextColor, 
                c_ListSelTextColor);
        }

        list_control_ptr_array_[MRL_LST_MEMBER]->SetCurSel(-1);

        int page_number = 0;

        if ( index_number )
        {
            if ( index_number >= c_iDefaultMemberLine )
            {
                page_number = (index_number+1)-c_iDefaultMemberLine;
            }
        }


        if ( scroll_control_ptr_array_[MRL_VSCR_MEMBER] )
        {
            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollRangeWZ(0,page_number);	//스크롤 범위
            scroll_control_ptr_array_[MRL_VSCR_MEMBER]->SetScrollPosWZ(0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::UpdateFinishDay()
{
    if ( !_isEnable() )
        return;

    TCHAR temp_text[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR message_text[INTERFACE_STRING_LENGTH] = {0,};

    //9952	결산일: %s
    g_InterfaceManager.GetInterfaceString( 9952, temp_text, INTERFACE_STRING_LENGTH );
    Snprintf(message_text, INTERFACE_STRING_LENGTH-1, temp_text, get_ui_rank_manager_ptr_()->finish_day_string_.c_str());
    GlobalFunc::SetCaption( text_control_ptr_array_[MRL_TXT_DATE], message_text );
}


//------------------------------------------------------------------------------
/**
*/

void uiMissionRankList2::_setTitle()
{
    if ( get_ui_rank_manager_ptr_() )
    {
        TCHAR message_text[INTERFACE_STRING_LENGTH] = {0,};
        
        if ( current_select_ranking_type_ == eRANKING_TYPE_NONE )
        {            
            strcpy(message_text, "None");
            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if(list_button_ptr_array_[i])
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }

        }
        else if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
        {            
            // 9956 %s 파티번호
            g_InterfaceManager.GetInterfaceString(9956, message_text, INTERFACE_STRING_LENGTH);
            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            // 2740 %s 진행시간
            g_InterfaceManager.GetInterfaceString( 2740, message_text, INTERFACE_STRING_LENGTH );
            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
        }
        else if ( current_select_ranking_type_ == eRANKING_TYPE_POINT )	// 포인트 랭킹
        {            
            // 2739 %s 캐릭터명
            g_InterfaceManager.GetInterfaceString( 2739, message_text, INTERFACE_STRING_LENGTH );
            
            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if(list_button_ptr_array_[i])
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            // 2741 %s 점수
            g_InterfaceManager.GetInterfaceString( 2741, message_text, INTERFACE_STRING_LENGTH );
            
            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
        }
        else if ( current_select_ranking_type_ == eRANKING_TYPE_BATTLEGROUND_WIN )    // 이스라필 대지 승률
        {
            // 2739 %s 캐릭터명
            g_InterfaceManager.GetInterfaceString( 2739, message_text, INTERFACE_STRING_LENGTH );

            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            // 2742 %s 전적
            g_InterfaceManager.GetInterfaceString( 2742, message_text, INTERFACE_STRING_LENGTH );

            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
        }
        else if ( current_select_ranking_type_ == eRANKING_TYPE_BATTLEGROUND_KILL )
        {
            // 2739 %s 캐릭터명
            g_InterfaceManager.GetInterfaceString( 2739, message_text, INTERFACE_STRING_LENGTH );

            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            // 2743 %s 킬
            g_InterfaceManager.GetInterfaceString( 2743, message_text, INTERFACE_STRING_LENGTH );

            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
        }
        else if ( current_select_ranking_type_ == eRANKING_TYPE_BATTLEGROUND_ASSIST )
        {
            // 2739 %s 캐릭터명
            g_InterfaceManager.GetInterfaceString( 2739, message_text, INTERFACE_STRING_LENGTH );

            int i;
            for ( i = 0; i <= MRL_LST_BTN_NAME3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
            
            // 2744 %s 어시스트
            g_InterfaceManager.GetInterfaceString( 2744, message_text, INTERFACE_STRING_LENGTH );

            for ( i = MRL_LST_BTN_POINT1; i <= MRL_LST_BTN_POINT3; ++i )
            {
                if ( list_button_ptr_array_[i] )
                {
                    GlobalFunc::SetCaption( list_button_ptr_array_[i], message_text );
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::all_tree_controller_reflresh()
{
    for ( int i = 0; i < max_group_+1; ++i )
    {
        if ( group_tree_item_[i] )
        {
            group_tree_item_[i]->set_open(false);
            group_tree_item_[i]->set_select(false);
        }
    }
    if (tree_controller_ptr_)
    {
        tree_controller_ptr_->Reflush();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::rank_data_list_clear()
{
    std::list<_stRankData*>::iterator iter = rank_data_list_.begin();

    for ( ; iter != rank_data_list_.end(); ++iter )
    {
        delete (*iter);
        (*iter) = NULL;
    }

    rank_data_list_.clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::group_infomation_enable( const bool& isShow )
{
    // 멤버 창 활성화 비활성화 관련
    if ( isShow )
    {
        CCtrlButtonWZ* button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_GUILD_LEVEL));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_GUILD));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_NAME));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_SHOW);
        }

        CCtrlStaticWZ* static_control_ptr = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_GROUP_INFOMATION_TITLE));
        if ( static_control_ptr )
        {
            static_control_ptr->ShowWindowWZ(WZ_SHOW);
        }

        CCtrlPictureWZ* picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_GROUP_INFOMATION_PICTURE));
        if ( picture_control_ptr )
        {
            picture_control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_VSCR_MEMBER_FRAME));
        if ( picture_control_ptr )
        {
            picture_control_ptr->ShowWindowWZ(WZ_SHOW);
        }

        picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_GROUP_INFOMATION_BACKGROUND));
        if ( picture_control_ptr )
        {
            picture_control_ptr->ShowWindowWZ(WZ_SHOW);
        }

        CCtrlVScrollWZ* vscrol_control_ptr = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_MRL_VSCR_MEMBER));
        if ( vscrol_control_ptr )
        {
            vscrol_control_ptr->ShowWindowWZ(WZ_SHOW);
        }
    }
    else
    {
        CCtrlButtonWZ* button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_GUILD_LEVEL));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_GUILD));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        button_control_ptr = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRL_STATIC_NAME));
        if ( button_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_HIDE);
        }

        CCtrlStaticWZ* static_control_ptr = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRL_TXT_GROUP_INFOMATION_TITLE));
        if ( static_control_ptr )
        {
            static_control_ptr->ShowWindowWZ(WZ_HIDE);
        }

        CCtrlPictureWZ* picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_GROUP_INFOMATION_PICTURE));
        if ( picture_control_ptr )
        {
            button_control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_VSCR_MEMBER_FRAME));
        if ( picture_control_ptr )
        {
            picture_control_ptr->ShowWindowWZ(WZ_HIDE);
        }

        picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_GROUP_INFOMATION_BACKGROUND));
        if ( picture_control_ptr )
        {
            picture_control_ptr->ShowWindowWZ(WZ_HIDE);
        }

        CCtrlVScrollWZ* vscrol_control_ptr = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_MRL_VSCR_MEMBER));
        if ( vscrol_control_ptr )
        {
            vscrol_control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::tree_controller_message_proc_check()
{    
    if ( get_ui_rank_manager_ptr_()->get_Is_wait_receive_packet_() == true )
    {
        return;
    }

    if ( tree_controller_ptr_->GetSelItem() && 
        tree_controller_ptr_->GetSelItem()->is_leaf() )
    {
        _stRankData* user_ptr = 
            (_stRankData*)tree_controller_ptr_->GetSelItem()->tree_data.user_data_Ptr_;

        if ( user_ptr && ((current_sel_mapinfo_ != user_ptr->map_info_ptr) || 
            current_select_ranking_type_ != user_ptr->ranking_system_type ))
        {
            current_sel_mapinfo_ = user_ptr->map_info_ptr;
            current_select_ranking_type_ = user_ptr->ranking_system_type;
            _setTitle();
            get_ui_rank_manager_ptr_()->fame_data_and_weeks_and_member_data_clar();
            _initRankList();
            _initRankListSelect();
            UpdateMissionText();

            if ( current_select_ranking_type_ == eRANKING_TYPE_TIME_ATTACK )
            {
                group_infomation_enable(true);
            }
            else
            {
                group_infomation_enable(false);
            }            
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::list_control_ptr_array_clear()
{
    if ( list_control_ptr_array_[MRL_LST_FAME] )
    {
        list_control_ptr_array_[MRL_LST_FAME]->DeleteItemAll();
    }

    if ( list_control_ptr_array_[MRL_LST_WEEK] )
    {
        list_control_ptr_array_[MRL_LST_WEEK]->DeleteItemAll();
    }

    if ( list_control_ptr_array_[MRL_LST_PREWEEK] )
    {
        list_control_ptr_array_[MRL_LST_PREWEEK]->DeleteItemAll();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::wait_play_animation()
{
    static FRECT img_rect = {0,};
    // dialog size
    RECT rcDialog;
    GetDialogWZ()->GetDlgSize(&rcDialog);

    CCtrlListWZ* ui_list_control = static_cast<CCtrlListWZ*>(this->getControl(DLGPOS_MRL_LST_WEEK));

    img_rect.left = static_cast<float>(ui_list_control->GetSizeRect().left);
    img_rect.top = static_cast<float>(ui_list_control->GetSizeRect().top);
    img_rect.right = static_cast<float>(ui_list_control->GetSizeRect().right);
    img_rect.bottom = static_cast<float>(ui_list_control->GetSizeRect().bottom);    

    img_rect.left = img_rect.left + ((img_rect.right - img_rect.left - ANIMATION_FRAME_WIDTH) / 2) + 0.5f;
    img_rect.top = img_rect.top + ((img_rect.bottom - img_rect.top - ANIMATION_FRAME_HEIGHT) / 2);

    int frame = 30;//초당 애니메이션 프래임
    int count = ((wait_animation_timer_.SpentTime() / frame) % ANIMATION_FRAME);

    if (get_ui_rank_manager_ptr_()->get_Is_wait_receive_packet_() == true)
    {
        g_pSunRenderer->RenderTexture(wait_texture_, img_rect.left, img_rect.top, 
            ANIMATION_FRAME_WIDTH, ANIMATION_FRAME_HEIGHT, 
            texture_rectangles_[count].left, texture_rectangles_[count].top, 
            texture_rectangles_[count].right, texture_rectangles_[count].bottom);
        ++count;
    }
    else
    {
        //nop
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::reward_notice_play_animation()
{
    CCtrlPictureWZ* picture_control_ptr = 
        static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_REWARD_YELLOW_LINE));

    if ( picture_control_ptr )
    {
        float rate = (static_cast<float>(reward_texture_alpha_color_) / 255.0f);
        picture_control_ptr->SetTransprency(rate);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankList2::OnRenderSolarDialog()
{
    if (wait_animation_timer_.Check() != TIMER_STAT_NONE)
    {
        if (wait_animation_timer_.Check() == TIMER_STAT_FINISH)
        {
            wait_animation_timer_.Off();
            wait_animation_timer_.On(1);
        }
    }
    wait_play_animation();

    DWORD time = texture_timer_.SpentTime();
    if (time > 20)
    {
        if (color_state_ == eCOLOR_STATE_INC)
        {
            reward_texture_alpha_color_ += 30;
        }
        if (color_state_ == eCOLOR_STATE_DEC)
        {
            reward_texture_alpha_color_ -= 30;
        }

        if (reward_texture_alpha_color_ >= 255)
        {
            reward_texture_alpha_color_ = 255;

            color_state_ = eCOLOR_STATE_DEC;
        }
        else if (reward_texture_alpha_color_ <= 0)
        {
            reward_texture_alpha_color_ = 0;

            color_state_ = eCOLOR_STATE_INC;
        }

        texture_timer_.Off();
        texture_timer_.On(1);
    }

    if ( get_ui_rank_manager_ptr_()->get_Is_can_reward_() )
    {
        reward_notice_play_animation();
    }
}

void uiMissionRankList2::set_reward_yellow_texture_show( BOOL Is_show )
{
    CCtrlPictureWZ* picture_control_ptr = static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MRL_PICTURE_REWARD_YELLOW_LINE));
    if ( picture_control_ptr )
    {
        picture_control_ptr->ShowWindowWZ(Is_show ? WZ_SHOW : WZ_HIDE);        
    }
}

void uiMissionRankList2::non_existent_my_ranking_list_control_item()
{
    // 내 랭킹 순위 없음.
    int index_number = 0;
    index_number = list_control_ptr_array_[MRL_LST_FAME]->InsertItemBack();
    list_control_ptr_array_[MRL_LST_FAME]->SetItemData(index_number, &(get_ui_rank_manager_ptr_()->my_ranking_view_data_));

    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
    ZeroMemory(temp_message, sizeof(temp_message));
    // 5480 %s : 순위에 없음
    g_InterfaceManager.GetInterfaceString(5480, temp_message, INTERFACE_STRING_LENGTH);            
    DWORD dwStyle = 0;
    dwStyle = DT_VCENTER | DT_RIGHT;

    GlobalFunc::SetItem(list_control_ptr_array_[MRL_LST_FAME], index_number, eFAME_LST_TIME,
        temp_message,
        StrToWzID("m212"),
        dwStyle,
        c_ListBKColor, 
        c_ListNonExistentTextColor, 
        c_ListSelTextColor);
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM