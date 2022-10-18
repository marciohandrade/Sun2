#pragma once

#include "uiBase.h"
#include "SCPerkSlotContainer.h"
#include "SkillSystem/PerkDataContainer.h"

class InterfaceManager;
class uiSkillMan;
class uiPerkDialog;
class ItemUnitRenderer;

class uiPerkSlot
    : public uiBase
{
    enum UIControlPos
    {
        // ��ϵ� Perk ����
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B000,

        kControl_Size,
    };

public:
    // ������/�Ҹ���
    uiPerkSlot(InterfaceManager* ui_manager);
    virtual ~uiPerkSlot();

    // �ʱ�ȭ/����
    virtual void Init(CDrawBase* pDrawBase);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * pMessage);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();

private:
    void RegisterControl();

    bool OnMouseOver(SI_MESSAGE* message);

private:
    uiPerkDialog* ui_perk_dialog_;

    bool is_mouse_over_in_perk_slot_;
    POSTYPE mouse_over_perk_slot_;
};

