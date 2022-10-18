#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "ChaosSystem/ChaosZoneTreeParser.h"
#include "ChaosSystem/ChaosSystemMan.h"

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>

class uiChaosZoneBattle : public uiBase, public SCItemSlotContainer
{
    enum UIControlPos
    {
        kControl_Picture_P024 = 0, // 보상슬롯 (인덱스 0부터..)
        kControl_Picture_P025,
        kControl_Picture_P026,
        kControl_Picture_P027,
        kControl_Picture_P028,
        kControl_Picture_P029,
        kControl_Picture_P030,
        kControl_Picture_P031,
        kControl_Picture_P032,
        kControl_Picture_P033,  // 보상슬롯 끝

        kControl_Button_B999,
        kControl_Text_T999,
        kControl_Button_B005,
        kControl_VScroll_V001,
        kControl_Text_S000,
        kControl_Picture_P001,
        kControl_Picture_P005,
        kControl_Picture_P009,
        kControl_List_L000,
        kControl_List_L001,
        kControl_Text_S006,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B007,

        kControl_Size,
    };

    enum
    {
        kSlotTotalCount = kControl_Picture_P033 - kControl_Picture_P024 + 1,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneBattle(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneBattle();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnReset();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL is_show);

    void SetReservationButton(bool is_reservation); // 입장 버튼 텍스트 설정
private:
    uiChaosSystemMan* GetManager();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void RenderTooltip();

    TreeMenuGroup& tree_menu() { return tree_menu_; }

    void MakeChaoszoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth,
        DWORD string_code, TCHAR* string, const DWORD* font_colors, DWORD map_code);
    bool SelectIndex(TreeMenuGroup::TreeObject* tree_object);
    void InitBattleInfo(const ChaosZoneTreeInfo* choaszone_info);

    void UpdateMouseWheel();
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);
    void DeleteAllSlot();
    void ReFreshRenderSlot();

private:
    TreeMenuGroup tree_menu_;
    CCtrlVScrollWZ* scroll_control_;
    CCtrlListWZ* list_control_;

    uiChaosSystemMan* ui_chaossystem_manager_;
    bool once_make_list_;
    CHAOSZONE_TREE_INDEX select_index_;     //선택된 트리 인덱스
    DWORD last_select_index_; 

    FTextRender	ftext_render_;

    ItemUnitRenderer* item_unit_renderer_;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
