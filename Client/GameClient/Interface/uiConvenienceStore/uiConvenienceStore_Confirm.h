#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "uiConvenienceStore_Purchase.h"
#include "VScrollDialog.h"
#include "FTextRender.h"
#include "uiConvenienceStoreMan.h"

class uiConvenienceStoreMan;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_Confirm: public uiConvenienceStore_Dialog, public VScrollDialog
{
    //  155516	���� Ȯ��
    //  155517	���� Ȯ��

    //155511	�� %s���� �������� ���� �ϼ̽��ϴ�.
    //    155512	�� %s���� �������� ���� �ϼ̽��ϴ�.
    //    155513	�� %s���� ��Ű�� �������� ���� �ϼ̽��ϴ�.
    //    155514	�� %s���� ��Ű�� �������� ���� �ϼ̽��ϴ�.

    friend class uiConvenienceStoreMan;

    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B028,
        kControl_Text_S000,
        kControl_Text_S003,
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S002,
        kControl_Text_S004,
        kControl_Text_S001,
        kControl_VScroll_V000,
        kControl_Picture_P002,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_PI13,
        kControl_Picture_P001,
        kControl_Picture_P006,
        kControl_Picture_P000,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiConvenienceStore_Confirm (InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Confirm (void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick){}
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);

    //! ���� �Ǵ� ���� �ϱ�
    virtual void ShowInterface(bool is_show,
                                uiConvenienceStore_Purchase::PurchaseType purchase_type,
                                ShopItemInfoEx* shop_item_info_ex, 
                                int item_info_ex_count);
    //------------------------------------------------------------------------------ 
    void UpdateCashInfo();
    void SetRecvName(const TCHAR* char_name);

    FTextRender& ftext_render() { return ftext_render_; }

protected:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool GetScrollArea(RECT& area);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void InitFText(FTextParser& parser, FTextSentence& sentence);


    FTextRender	ftext_render_;
};

