#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"

#include "SSQScene.h"
#include <ssq/SSQInfoStruct.h>
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>


class uiChaosZoneSSQ : public uiBase, public SCItemSlotContainer
{
    enum UIControlPos
    {
        kControl_Picture_P021 = 0,  // ∫∏ªÛΩΩ∑‘ (¿Œµ¶Ω∫ 0∫Œ≈Õ..)
        kControl_Picture_P023,
        kControl_Picture_P024,
        kControl_Picture_P025,  // ∫∏ªÛΩΩ∑‘ ≥°

        kControl_Button_B999,
        kControl_Text_T999,
        kControl_Button_B005,
        kControl_Text_S000,
        kControl_Picture_P001,
        kControl_Picture_P005,
        kControl_Picture_P019,
        kControl_List_L000,
        kControl_Text_S004,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S005,
        kControl_Text_S003,
        kControl_Text_S006,
        kControl_Picture_P008,
        kControl_Picture_P009,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P013,
        kControl_Picture_P014,
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
        kSlotTotalCount = kControl_Picture_P025 - kControl_Picture_P021 + 1,
    };
public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneSSQ(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneSSQ();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL is_show);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void UpdateSSQInfo();
    void RenderSSQTime();
    void RenderTooltip();
    void InitRewardItemSlot();

    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);
    void DeleteAllSlot();
    void ReFreshRenderSlot();

private:
    CDrawBase* draw_base_;

    bool loaded_texture_;
    HANDLE number_texture_;

    SYSTEMTIME ssq_system_time;
    typedef nsSSQ::SSQTicketTimeInfo SSQTIMEINFO;
    SSQTIMEINFO::eTM ssq_current_state;

    ItemUnitRenderer* item_unit_renderer_;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
