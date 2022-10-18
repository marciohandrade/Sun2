#pragma once

#include "uiBase.h"
#include "CashShopHeader.h"
#include "uiConvenienceStore_PopupMenu.h"

class uiConvenienceStoreMan;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_PopupPackage : public uiConvenienceStore_PopupMenu
{
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Text_S000,
        kControl_Text_S028,
        kControl_Text_S029,
        kControl_Text_S030,
        kControl_Text_S031,
        kControl_Text_S032,
        kControl_Text_S033,
        kControl_Text_S034,
        kControl_Text_S035,
        kControl_Text_S036,
        kControl_Text_S037,
        kControl_Text_S038,
        kControl_Text_S039,
        kControl_Text_S040,
        kControl_Text_S041,
        kControl_Text_S042,
        kControl_Text_S043,
        kControl_Text_S044,
        kControl_Text_S045,
        kControl_Text_S046,
        kControl_Text_S047,
        kControl_Text_S048,
        kControl_Text_S049,
        kControl_Text_S050,
        kControl_Text_S051,
        kControl_Text_S052,
        kControl_Text_S053,
        kControl_Text_S054,
        kControl_Text_S055,
        kControl_Text_S056,

        //Lev1 slotImage
        KControl_Image_P059,
        KControl_Image_P087,
        KControl_Image_P088,
        KControl_Image_P089,
        KControl_Image_P090,
        KControl_Image_P091,
        KControl_Image_P092,
        KControl_Image_P093,
        KControl_Image_P094,
        KControl_Image_P095,

        //Lev2 str
        KControl_Image_P002,
        KControl_Image_P060,
        KControl_Image_P061,
        KControl_Image_P062,
        KControl_Image_P063,
        KControl_Image_P064,
        KControl_Image_P065,
        KControl_Image_P066,
        KControl_Image_P067,
        KControl_Image_P068,

        //Lev3 str
        KControl_Image_P021,
        KControl_Image_P069,
        KControl_Image_P070,
        KControl_Image_P071,
        KControl_Image_P072,
        KControl_Image_P073,
        KControl_Image_P074,
        KControl_Image_P075,
        KControl_Image_P076,
        KControl_Image_P077,

        //Lev4
        KControl_Image_P022,
        KControl_Image_P078,
        KControl_Image_P079,
        KControl_Image_P080,
        KControl_Image_P081,
        KControl_Image_P082,
        KControl_Image_P083,
        KControl_Image_P084,
        KControl_Image_P085,
        KControl_Image_P086,

        //Lev5
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

    static const int kListPerCount = 10;

    static void Callback_IconRender(CControlWZ* control_ptr, void* parameter);

public:
    uiConvenienceStore_PopupPackage(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_PopupPackage(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    OnShowWindow(BOOL val);
    virtual void    OnUpdateSolarDialog();

    virtual void    ShowInterface(bool is_show, ConvenienceStore::ProductItem* product);

private:
    void UpdateTooltip();
};
