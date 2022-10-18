#pragma once
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "ChaosSystem/ChaosZoneTreeParser.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include <SCItemSlotContainer.h>

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>

class uiChaosZoneGoldRush : public uiBase, public SCItemSlotContainer
{
    // �� �κ��� ui�� ������ ���� �� �� ����
    enum UIControlPos
    {
        kControl_Picture_P024 = 0, // ���󽽷� (�ε��� 0����..)
        kControl_Picture_P025,
        kControl_Picture_P026,
        kControl_Picture_P027,
        kControl_Picture_P028,
        kControl_Picture_P029,
        kControl_Picture_P030,
        kControl_Picture_P031,
        kControl_Picture_P032,
        kControl_Picture_P033,  // ���󽽷� ��

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
    uiChaosZoneGoldRush(InterfaceManager* ui_manager_ptr);
    virtual ~uiChaosZoneGoldRush();

    virtual void Init(CDrawBase* draw_base_ptr);
    virtual void Release();
    virtual void OnReset();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL is_show);

    void SetReservationButton(const bool& is_resvervation); // ���� ��ư �ؽ�Ʈ ����
private:
    uiChaosSystemMan* GetManager();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void RenderTooltip();    

    TreeMenuGroup& tree_menu() { return tree_menu_; };

    void MakeChaoszoneGoldRushTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth,
        DWORD string_code, TCHAR* string, const DWORD* font_colors, DWORD map_code);
    bool SelectIndex(TreeMenuGroup::TreeObject* tree_object);
    void InitGoldRushInfo(const ChaosZoneTreeInfo* chaoszone_info);

    void UpdateMouseWheel();
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);
    void DeleteAllSlot();
    void ReFreshRendereSlot();

private:
    TreeMenuGroup tree_menu_;
    CCtrlVScrollWZ* scroll_control_ptr_;
    CCtrlListWZ* list_control_ptr_;

    uiChaosSystemMan* ui_chaossystem_manager_;
    bool once_make_list_;
    CHAOSZONE_TREE_INDEX select_index_;     // ���õ� Ʈ�� �ε���
    DWORD last_select_index_;

    FTextRender ftext_render_;

    ItemUnitRenderer* item_unit_renderer_;

public:
    static const DWORD kDialog_ID;
};
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH