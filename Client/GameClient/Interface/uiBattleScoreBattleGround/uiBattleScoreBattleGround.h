#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uibase.h"
#include "uibattlescoreman\uibattlescoreman.h"
#include "VScrollDialog.h"

class uiBattleScoreMan;
class uiBattleScoreBattleGround : public uiBase, public VScrollDialog
{	
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,
        kControl_CheckBT_C001,
        kControl_CheckBT_C003,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B007,
        kControl_Button_B006,
        kControl_Button_B010,
        kControl_Button_B011,
        kControl_Button_B012,
        kControl_Text_S000,
        kControl_List_L000,
        kControl_VScroll_V000,
        kControl_Picture_P006,
        kControl_Picture_P008,
        kControl_Picture_P001,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Text_S001,
        kControl_Size,
    };

public:
    enum eBATTLE_GROUND_TAB_BUTTONS
    {
        TAB_BUTTON_ALL_TEAM = 0,
        TAB_BUTTON_HOME_TEAM,
        TAB_BUTTON_AWAY_TEAM,
        TAB_BUTTON_MAX
    };

    enum BATTLE_GROUND_REPORT_LIST
    {
        REPORT_LIST_RANK = 0,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        REPORT_LIST_CLASS,
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        REPORT_LIST_ID,
        REPORT_LIST_LV,
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        REPORT_LIST_KILL_DIE_ASSIST,
#else
        REPORT_LIST_KILL_DIE,
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        REPORT_LIST_SERIES_KILL,
        REPORT_LIST_GIVEDAMAGE,
        REPORT_LIST_GIVEHEAL,
        REPORT_LIST_POINT_ATTACK,   // 거점 공격
        REPORT_LIST_POINT_DEFFENCE, // 거점 방어
        REPORT_LIST_MAX,
    };

public:	
    uiBattleScoreBattleGround(InterfaceManager* manager_ptr);
    virtual ~uiBattleScoreBattleGround(void);
    virtual void		Init(CDrawBase* pDrawBase);
    virtual void		Release();

    virtual void		MessageProc(SI_MESSAGE* pMessage);
    virtual void	    OnShowWindow(BOOL val);
    uiBattleScoreMan*	battle_score_manager();

    void refresh_list_control_data(nArray<BATTLESCORE_DATA> &ArrayType);
    void InitInfo();
    void enable_exit_button(bool is_enable);
    void exit_battle_ground(); // 전장 떠나기

    virtual void		OnRenderSolarDialog();
    virtual void        OnUpdateSolarDialog();

    void set_select_tap(eBATTLE_GROUND_TAB_BUTTONS select){ select_tap_ = select; }
    eBATTLE_GROUND_TAB_BUTTONS select_tap(){ return select_tap_; };
    
private:
    void				set_list_controls_data(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline);
    CCtrlVScrollWZ* vscroll_control();
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);
    bool GetScrollArea(RECT& area);
private:
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    CCtrlListWZ*		battle_players_list_control_;
    CCtrlVScrollWZ*     scroll_control_;
    uiBattleScoreMan*	battle_score_manager_;
    eBATTLE_GROUND_TAB_BUTTONS select_tap_;
    bool				is_init_;

    nArray<BATTLESCORE_DATA> score_member_array; // 맴버 점수 리스트

};
//------------------------------------------------------------------------------
/**
*/


#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND