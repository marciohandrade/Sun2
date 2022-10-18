#ifndef     __USER_INTERFACE_ITEM_SHOP_DIV_H__
#define     __USER_INTERFACE_ITEM_SHOP_DIV_H__
#include "uiBase.h"

#include "ItemUnitRenderer.h"

class uiItemShopDiv : public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_OK,
        DLGPOS_BTN_CANCEL,
        DLGPOS_TEXT_MAX_NUMBER,
        DLGPOS_TEXT_HIEM,
        DLGPOS_EDT_NUM,
        DIALOG_MAX,
    };

public:
    uiItemShopDiv(InterfaceManager *ui_manager_ptr);
    virtual ~uiItemShopDiv();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void Init(CDrawBase* draw_base_ptr);
    virtual void Release();
    void InitControls();
    bool CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    SetData(DURATYPE div_num,
                                    SLOTIDX from_slot_idx, 
                                    POSTYPE from_pos, 
                                    eSlotType slot_type, 
                                    SLOTCODE code, 
                                    ItemUnitRenderer::RenderUnit* render_unit_ptr, 
                                    RECT* rect_ptr,
                                    MONEY price);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void OnShowWindow(BOOL value);
    virtual void OnUpdateSolarDialog();
    void         on_ok_button_click();

protected:
    CControlWZ*	 get_control(POSTYPE at_pos);
    POSTYPE	get_control_id_to_position(WzID wz_id);
    static WzID	 wzId_[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void MessageProc(SI_MESSAGE* message_ptr);   // Pure
    //virtual void UpdatePosition();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlDxIMEEditBox* edit_control_;
    CCtrlStaticWZ* static_text_controls_[2];
    CCtrlButtonWZ* ok_button_;
    CCtrlButtonWZ* cancel_button_;
    bool init_controls_;
    DURATYPE divide_num_;
    SLOTIDX from_slot_idx_;
    POSTYPE from_postype_;
    eSlotType slot_type_;
    SLOTCODE slot_code_;
    ItemUnitRenderer::RenderUnit* render_unit_ptr_; 
    RECT rect_;
    MONEY price_;
    DWORD pre_divide_number_;
};

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiItemShopDiv::get_control_id_to_position(WzID wz_id)
{
    for (POSTYPE i = 0; i < DIALOG_MAX; ++i)
    {
        if (wzId_[i] == wz_id)
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiItemShopDiv::get_control(POSTYPE at_pos)
{
    assert(at_pos < DIALOG_MAX);
    return GetControlWZ(wzId_[at_pos]);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiItemShopDiv::CheckControls()
{
    if (!this->init_controls_)
    {
        assert ("Unknown controls!!");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline void uiItemShopDiv::SetData(DURATYPE div_num,
                                    SLOTIDX from_slot_idx, 
                                    POSTYPE from_pos, 
                                    eSlotType slot_type, 
                                    SLOTCODE code, 
                                    ItemUnitRenderer::RenderUnit* render_unit_ptr, 
                                    RECT* rect_ptr,
                                    MONEY price)
{
    divide_num_ = div_num;
    from_slot_idx_ = from_slot_idx;
    from_postype_ = from_pos;
    slot_type_ = slot_type;
    slot_code_ = code;
    render_unit_ptr_ = render_unit_ptr; 
    SetRect (&rect_, rect_ptr->left, rect_ptr->top, rect_ptr->right, rect_ptr->bottom);
    price_ = price;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_ITEM_SHOP_DIV_H__
