#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "ShopItemObject.h"

//------------------------------------------------------------------------------ 
class uiConvenienceStore_ItemDetail : public uiConvenienceStore_Dialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B028,
        kControl_Text_S000,
        kControl_Text_S002,
        kControl_Text_S005,
        kControl_Text_S001,
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Edit_E000,
        kControl_Picture_P002,
        kControl_Picture_P005,
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
#else
        kControl_Picture_P007,
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Picture_P001,
        kControl_Picture_P000,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P006,
        kControl_Picture_P008,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

    static void Callback_IconRender(CControlWZ* control_ptr, void* parameter);

public:
    uiConvenienceStore_ItemDetail(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_ItemDetail(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    OnShowWindow(BOOL val);
    virtual void    Process(DWORD tick);
    virtual void    OnUpdateSolarDialog();
    virtual void    OnRenderSolarDialog();
    virtual void    NetworkProc(MSG_BASE* msgbase);
    virtual void    MessageProc(SI_MESSAGE* message);
    virtual void    ShowInterface(ConvenienceStore::ShopItemObject* pObject );

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
};
