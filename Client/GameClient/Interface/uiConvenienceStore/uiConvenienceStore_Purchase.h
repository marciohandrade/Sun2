#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "DropdownControl.h"
#include "uiConvenienceStore_Basket.h"
#include "VScrollDialog.h"
#include "uiConvenienceStore_CashType.h"

class uiConvenienceStoreMan;


//------------------------------------------------------------------------------ 
class uiConvenienceStore_Purchase : public uiConvenienceStore_Dialog, public VScrollDialog
{
public:
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B001,

        kControl_Button_B003, //!< 단일상품
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,

        //kControl_Button_B000, //!< 패키지 상품
        //kControl_Button_B002,
        //kControl_Button_B005,
        //kControl_Button_B010,
        //kControl_Button_B014,

        kControl_Button_B011, //!< 이미지 영역
        kControl_Button_B012,
        kControl_Button_B013,
        kControl_Button_B015,
        kControl_Button_B016,

        kControl_Button_B017, //콤보 버튼
        kControl_Button_B024,
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,

        kControl_Button_B018, //!< 삭제 버튼
        kControl_Button_B020,
        kControl_Button_B021,
        kControl_Button_B022,
        kControl_Button_B023,

        kControl_Button_B028,
        kControl_Button_B029,
        kControl_Text_ST05,
        kControl_Text_ST20,
        kControl_Text_ST12,
        kControl_Text_S000,
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S007,
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Button_B000,
        kControl_Button_B005,
        kControl_Button_B010,
        kControl_Button_B014,
        kControl_Button_B019,
#else
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Text_S011,
        kControl_Text_S012,
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S019,
        kControl_Text_S020,
        kControl_List_L000,
        kControl_List_L005,
        kControl_List_L006,
        kControl_List_L007,
        kControl_List_L008,
        kControl_VScroll_V000,
        kControl_Picture_P002,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_PI11,
        kControl_Picture_PI13,
        kControl_Picture_P001,
        kControl_Picture_P006,

#ifdef _NA_0_20120321_CASHSHOP_POPUP_EFFECT
        kControl_Picture_P008,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P013,
#endif

        kControl_Size,
    };

    enum PurchaseType
    {
        kPurchase_Buy = 0,
        kPurchase_Gift,
        kPurchase_Size,
    };

public:
    static const DWORD kDialog_ID;

    static const int kListPerCount = 5;

public:
    uiConvenienceStore_Purchase(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Purchase(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick){}
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual bool IsNeedLockDialog() { return true; }
    virtual bool OnClickAskDialog(uiConvenienceStore_Dialog::AskCommandType command_type, 
                                uiConvenienceStore_Dialog::AskShowType show_type, 
                                void* user_data); 

    void OpenPurchase( PurchaseType _purchase_type );
    void SetCashType( ConvenienceStore::Define::CashType cash_type ) { cash_type_ = cash_type; }
    void SetGiftMessage( LPCTSTR target_name, LPCTSTR send_message );

    bool InsertPurchase(ConvenienceStore::ProductItem* product, int basket_product_index);
    bool RemovePurchase(int purchase_index);
    void RemovePurchase();

    // 결제할 캐쉬 방식
    ConvenienceStore::Define::CashType GetCashType() { return cash_type_; }


    std::vector<ConvenienceStore::PurchaseItem*>& purchase_list() { return purchase_list_; }
    uiConvenienceStore_Purchase::PurchaseType purchase_type() const { return purchase_type_; }

    DropdownControl* dropdown_price(int index);
    CCtrlButtonWZ* backslot_button(int slot_index);

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

    void UpdateUI(TCHAR* recv_character_name);
    void UpdateTooltip();
    void FillPurchase(int begin_purchase_index);
    CASH GetPurchasePrice();

    ////! 호출한 다이얼로그(장바구니 클리어 할건지 여부때문)
    //uiConvenienceStore_Dialog* owner_;

    //! 메모리를 가지고 있는다.
    //! 종료시 delete 필요
    std::vector<ConvenienceStore::PurchaseItem*> purchase_list_;

    ConvenienceStore::Define::CashType cash_type_;

    PurchaseType purchase_type_;

    DropdownControl dropdown_price_[kListPerCount];

    util::_tstring gift_msg_;
    util::_tstring gift_recv_name_;

    void _UpdateControlState();
};
