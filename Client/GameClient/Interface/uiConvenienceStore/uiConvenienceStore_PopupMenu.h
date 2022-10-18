#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"

class uiConvenienceStoreMan;
class uiConvenienceStore_Dialog;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_PopupMenu : public uiBase
{
    typedef std::map< DWORD /*CCtrlButtonWZ::GetCtrlID()*/, std::pair<uiConvenienceStore_Dialog::PopupCommandType, DWORD/*user_data*/> > PopupCommandMap;

#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
protected:
    enum 
    {
        kDialogMargin = 26,
        kControlMargin = 10,
    };
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI

public:
    uiConvenienceStore_PopupMenu(InterfaceManager* ui_man) : uiBase(ui_man), owner_(0), owner_control_(0) {}
    virtual ~uiConvenienceStore_PopupMenu(void) {}

    virtual void OnShowWindow(BOOL val);

    uiConvenienceStoreMan* GetManager();

    virtual void Reset();
    virtual void SetMenu(int button_index, DWORD string_code, bool is_enable, uiConvenienceStore_Dialog::PopupCommandType command_type, DWORD user_data){}

    virtual uiConvenienceStore_Dialog* owner() const { return owner_; }
    virtual CControlWZ* owner_control() const { return owner_control_; }
    virtual void set_owner(uiConvenienceStore_Dialog* dialog, CControlWZ* control) { owner_ = dialog; owner_control_ = control; }

    PopupCommandMap& command_map() { return command_map_; }
#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
    int extra_text_width() const { return extra_text_width_; }
    void set_extra_text_width(int value) { extra_text_width_ = value; }
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI

private:
    uiConvenienceStore_Dialog* owner_;
    CControlWZ* owner_control_;

    PopupCommandMap command_map_;
#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
    int extra_text_width_;
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI  
};
