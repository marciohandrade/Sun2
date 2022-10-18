#pragma once

#ifdef _NA_002935_20110704_ITEM_SCORE

#include "uiBase.h"
#include "SolarDialog.h"
#include "equipmentcontainer.h"

class InventoryDialog;
class ItemUnitRenderer;

class OtherPlayerEquipmentContainer : public uiBase, public EquipmentContainer
{
public:
    OtherPlayerEquipmentContainer(InterfaceManager* ui_manager);
    virtual ~OtherPlayerEquipmentContainer();

    VOID                    Release();
    VOID                    Init(CDrawBase* draw_base);

    virtual VOID			MessageProc(SI_MESSAGE* message);
    virtual VOID            MouseOver_MsgProc(SI_MESSAGE* message);
    
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE position, SCSlot& slot);
    BOOL                    DeleteSlot(POSTYPE position, SCSlot* slot_out);

    VOID                    ReFreshRenderSlot();
    VOID                    FlushUnitDraw();
    VOID                    InitItemRender();

    VOID                    SetEquipmentContainer(Player* player);

	DWORD GetDialogKey() { return uiBase::GetDialogKey(); }	// override EquipmentContainer

protected:

    virtual VOID			OnShowWindow(BOOL val);
    virtual VOID            OnRenderSolarDialog();

    void                    DrawItemScore(); //아이템 평점 출력

    static void             ESCKeyProcess(DWORD wparam,DWORD lparam);
    void                    TransForm(Player* player);
    void                    UnTransForm(Player* player);

    virtual CControlWZ*	    getControl(POSTYPE at_pos);
    VOID					getControlPos(POSTYPE at_p, RECT& rect_out);
    VOID                    ChargeItemExtendSlot(int extend_count);

    ItemUnitRenderer*   item_unit_renderer_;

    int                 item_score_;
    bool                is_pc_room_to_target_;
};
#endif //_NA_002935_20110704_ITEM_SCORE