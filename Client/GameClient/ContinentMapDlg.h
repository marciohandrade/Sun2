#pragma once
#include "uibase.h"
#include "uiRadarMan/uiContinentDefine.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "QuestManager_Concrete.h"

struct DOMINATION_AREA_INFO
{
    POSTYPE area_pos;
    CCtrlButtonWZ* area_button;
    MAPCODE area_mapcode;
};

class ContinentMapDlg : public uiBase
{
public:
	enum eCONTINENT_MAP_POS
	{

		eCONTINENT_MAP_POS_POINT_BTN_START = 0,
		eCONTINENT_MAP_POS_POINT_BTN_END = 37,

		eCONTINENT_MAP_POS_POINT_BTN_CNT = eCONTINENT_MAP_POS_POINT_BTN_END + 1,

		eCONTINENT_MAP_POS_EXIT_BTN = eCONTINENT_MAP_POS_POINT_BTN_CNT,
        eCONTINENT_MAP_POS_POPUP_SELECT_TEXT,//StrToWzID("S005"),   // 지도 팝업 선택된 것
        eCONTINENT_MAP_POS_POPUP_LIST,//StrToWzID("L001"),  // 지도 팝업 리스트
        eCONTINENT_MAP_POS_POPUP_BUTTON,//StrToWzID("B000"),  // 지도 팝업 화살표
        eCONTINENT_MAP_POS_AREA0,//StrToWzID("C000"),  // 신규 대륙 전체 영역
        eCONTINENT_MAP_POS_AREA1,//StrToWzID("C001"),  // 신규 대륙 영역1
        eCONTINENT_MAP_POS_AREA2,//StrToWzID("C002"),  // 신규 대륙 영역2
        eCONTINENT_MAP_POS_AREA3,//StrToWzID("C003"),  // 신규 대륙 영역3
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        eCONTINENT_MAP_POS_DOMINATION_HELP,//StrToWzID("P005"),  // 점령전 진행 아이콘 도움말
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        eCONTINENT_MAP_POS_MAX,
	};

	enum eCONTINENT_MAP_DOMINATION_POS
	{
		eCMDOMINATION_DIALOG_PIC,
		eCMDOMINATION_GUILD_MARK_BTN,
		eCMDOMINATION_GUILD_NAME_TEXT,
		eCMDOMINATION_GUILD_MASTER_TEXT,

		eCMDOMINATION_POS_MAX
	};

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    enum eCONTINENT_MAP_OWNER_GUILD_POS
    {
        eCONTINENT_MAP_OWNER_GUILD_MARK_BTN,
        eCONTINENT_MAP_OWNER_GUILD_NAME_TEXT,
        eCONTINENT_MAP_OWNER_GUILD_MASTER_TEXT,
        eCONTINENT_MAP_OWNER_GUILD_POS_MAX
    };
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

    enum eCONTINENT_MAP_AREA
    {
        eNEW_MAP_AREA0,// 신규 대륙 전체 영역
        eNEW_MAP_AREA1,// 신규 대륙 영역1
        eNEW_MAP_AREA2,// 신규 대륙 영역2
        eNEW_MAP_AREA3,// 신규 대륙 영역3
        eNEW_MAP_AREA_MAX,
    };

	ContinentMapDlg(InterfaceManager * pUIMan );
	~ContinentMapDlg();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);
    void                ToggleShowDialog();
    void SetAirShipMode();

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();


    // 각지역에 대한 마우스 오버 및 선택 이미지 처리
    void ClickAreaButton(POSTYPE area_button_pos);
    void RenderSelectAreaImage(CCtrlButtonWZ* area_button_control, bool is_select);

    void LoadImage();

    void RenderAirShipPoint();
    void RenderAirShipTooltip(WORD portal_index);
    void ClickAIrshipPortal(POSTYPE pos);

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    void GetMapPortalFromControl(int positon);
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void RenderDominationProgressMark(CControlWZ* control, DOMINATION_GUILD_INFO* guild_info);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

public:
    ContinentMapNumber GetContinentMapNumber() { return kContinentEP1MapNumber; }
    int GetContinentMapStartPoint() { return eCONTINENT_MAP_POS_POINT_BTN_START; }
    int GetContinentMapStartEnd() { return eCONTINENT_MAP_POS_POINT_BTN_END; }
    CControlWZ* GetContinentMapPointControl(int continent_code);

    MAPCODE GetDominationManagedMapcode();
    ContinentMapMode continent_map_mode()  { return continent_map_mode_; }
    void set_continent_map_mode(ContinentMapMode map_mode) {   continent_map_mode_ = map_mode; }    
    void ChangeContinentCode(int continent_map_mode);

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    void set_is_useable_map_portal(bool val) { is_useable_map_portal_ = val; }
    bool get_is_useable_map_portal() { return is_useable_map_portal_; }
    void set_is_actived_map_portal(bool val) { is_actived_map_portal_ = val; }
    bool get_is_actived_map_portal() { return is_actived_map_portal_; }
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL
private:
	static WzID		m_wzID[eCONTINENT_MAP_POS_MAX];
	static WzID		m_wzID_DominationCtrl[eCMDOMINATION_POS_MAX];
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    static WzID    wz_id_continent_owner_guild_control[eCONTINENT_MAP_OWNER_GUILD_POS_MAX];

    CCtrlButtonWZ* continent_owner_guild_mark_button_;
    CCtrlStaticWZ* continent_owner_guild_name_;
    CCtrlStaticWZ* continent_owner_guild_master_;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	CCtrlButtonWZ*  m_pExitBtn;
	CCtrlButtonWZ*  m_pPointBtn[eCONTINENT_MAP_POS_POINT_BTN_CNT];

	CCtrlButtonWZ* m_pGuildMarkBtn;
	CCtrlStaticWZ* m_pGuildNameText;
	CCtrlStaticWZ* m_pGuildMasterText;

   
    ContinentMapMode continent_map_mode_;
    
    DOMINATION_AREA_INFO area_info_;
    HANDLE area_select_image_;

    CCtrlStaticWZ* popup_select_text_;
    CCtrlListWZ* popup_list_;
    CCtrlButtonWZ* popup_down_button_;
    CCtrlButtonCheckWZ* new_world_areas_[eNEW_MAP_AREA_MAX];
    bool is_popup_list_show_;

	BOOL			m_bShow;

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    bool is_actived_map_portal_;
    bool is_useable_map_portal_;
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

    // 비공정 뿌릴 리스트(시작시 담는다)
    WORD airship_mapcode[eCONTINENT_MAP_POS_POINT_BTN_CNT];
#ifdef _NA_000000_20130114_RENEWER_UI
private:
	CTimerSimple animation_refresh_timer_;
	int			all_hide_alpha;

	void RenderSplineImageAlphaWarp(DWORD rendertick,float x, float y);		
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
};

inline CControlWZ * ContinentMapDlg::getControl(POSTYPE AtPos)
{
	assert(AtPos < eCONTINENT_MAP_POS_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ContinentMapDlg::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eCONTINENT_MAP_POS_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eCONTINENT_MAP_POS_MAX;
}

inline void ContinentMapDlg::ToggleShowDialog()
{
    BOOL is_visible = IsVisible();
    // 대륙맵이 열려있지 않으면 show
    if (is_visible == FALSE)
    {
        set_continent_map_mode(kContinentNormalMode);
        ShowInterface(TRUE);
    }
    else
    {
        ShowInterface(FALSE);
    }
}

inline MAPCODE ContinentMapDlg::GetDominationManagedMapcode()
{
    if (continent_map_mode_ == kContinentDominationManagerSelectedAreaMode)
    {
        return area_info_.area_mapcode;
    }
    return 0;
}

inline CControlWZ* ContinentMapDlg::GetContinentMapPointControl(int continent_code)
{
    if ((continent_code < GetContinentMapStartPoint()) || 
        (continent_code > GetContinentMapStartEnd()))
    {
        return NULL;
    }

    return m_pPointBtn[continent_code];
}