#pragma once
#include "uiBaseMan.h"
#include "uiContinentDefine.h"
#include "uiContinentNavigation.h"
#include "uiRadarMapData.h"
#include "uiRadarPointCollecter.h"
#include "uiRadarClickAnimation.h"

class RadarDialog;
class RadarPlusDialog;
class RadarFullDialog;
class ContinentMapDlg;
class ContinentEp2MapDlg;

struct IM_RADAR_MANAGER
{
	static const DWORD RADAR_DIALOG;
	static const DWORD RADAR_DIALOG_PLUS;
	static const DWORD RADAR_DIALOG_FULL;
	static const DWORD CONTINENT_MAP_DLG;
    static const DWORD CONTINENT_MAP_EP2_DLG;
};

class uiRadarMan :
	public uiBaseMan
{
public:
	uiRadarMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();
    virtual void            Process(DWORD dwTick);

    void ToggleShowContinentMap();

    void SetNavigationPoint() { continent_navigation_.SetNavigationPoint(); }
    void RenderNavigationPoint() { continent_navigation_.RenderNavigationPoint(); }

    ContinentMapNumber GetCurrentContinentMapNumber();
    uiBase* GetCurrentContinentMap();
    ContinentMapMode GetCurrentContinentMapMode();

    int GetCurrnetContinentMapStartPoint();
    int GetCurrnetContinentMapStartEnd();
    CControlWZ* GetCurrentContinentMapPointControl(int continent_code);

    virtual void            SetMapName(TCHAR* map_name);

    uiRadarMapData& radar_map_data() { return radar_map_data_; }
    uiRadarPointCollecter& radar_point_collecter() { return radar_point_collecter_; }
    uiRadarClickAnimation& radar_click_animation() { return radar_click_animation_; }

    void ChangeRadarMap(TCHAR* file_name = NULL);
    void LoadMapScript(TCHAR* file_name);
    void SetHeroPosition(WzVector hero_position);
    void SetClickPosition(float position_x, float position_y,int display_time);

private:
    uiContinentNavigation* continent_navigation() { return &continent_navigation_; }

	RadarDialog *			m_pRadarDlg;
	RadarPlusDialog *		m_pRadarPlusDlg;
	RadarFullDialog *		m_pRadarFullDlg;
	ContinentMapDlg *		m_pContinentMapDlg;
    ContinentEp2MapDlg* radar_full_ep2_map_dialog_;

    uiContinentNavigation continent_navigation_;
    uiRadarMapData radar_map_data_;
    uiRadarPointCollecter radar_point_collecter_;
    uiRadarClickAnimation radar_click_animation_;
};
