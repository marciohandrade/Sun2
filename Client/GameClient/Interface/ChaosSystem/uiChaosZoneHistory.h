#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "ChaosSystem/ChaosZoneTreeParser.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

class uiChaosZoneHistory : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B999 = 0,
        kControl_Text_T999,
        kControl_Button_B005,
        kControl_VScroll_V001,
        kControl_Text_S000,
        kControl_List_L000,
        //kControl_List_L001,
        kControl_Text_S001,
        kControl_Text_S005,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B007,
        kControl_List_L002,
        kControl_VScroll_V000,
        kControl_List_L003,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneHistory(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneHistory();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL is_show);

    void RefreshHistryInfo();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void RenderTooltip();

    TreeMenuGroup& tree_menu() { return tree_menu_; }
    void UpdateMouseWheel();

    void MakeChaoszoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth,
        DWORD string_code, TCHAR* string, const DWORD* font_colors);
    bool SelectIndex(TreeMenuGroup::TreeObject* tree_object);
    void InitHistroyInfo(const ChaosZoneTreeInfo* history_info);

private:
    TreeMenuGroup tree_menu_;

    CCtrlVScrollWZ* scroll_control_current_;
    CCtrlListWZ* list_control_current_;
    FTextRender	ftext_render_current_;
    CCtrlVScrollWZ* scroll_control_total_;
    CCtrlListWZ* list_control_total_;
    FTextRender	ftext_render_total_;

    bool once_make_list_;
    CHAOSZONE_TREE_INDEX select_index_;
    DWORD last_select_index_;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
