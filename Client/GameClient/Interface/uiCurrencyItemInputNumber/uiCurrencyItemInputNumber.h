#pragma once
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include "uiBase.h"

class uiCurrencyItemInputNumber : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_T999 = 0, // 타이틀
        kControl_Text_T001,     // 안내문구
        kControl_Button_B001,   // 증가버튼
        kControl_Button_B005,   // 감소버튼
        kControl_Button_TI00,   // 확인버튼
        kControl_Button_TI01,   // 취소버튼
        kControl_Button_B004,   // 닫기버튼
        kControl_Edit_E000,     // 개수 입력 에디트

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiCurrencyItemInputNumber(InterfaceManager* interface_manager);
    virtual ~uiCurrencyItemInputNumber();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);

    void UseCurrencyItem(POSTYPE item_position);

    POSTYPE currency_item_position() const { return currency_item_position_; }
    DURATYPE use_item_num() const { return use_item_num_; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    
    void SEND_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN();

    void set_currency_item_position(POSTYPE value) { currency_item_position_ = value; }
    void set_use_item_num(DURATYPE value) { use_item_num_ = value; }
    DURATYPE current_item_num() const { return current_item_num_; }
    void set_current_item_num(DURATYPE value) { current_item_num_ = value; }

private:
    POSTYPE currency_item_position_;
    DURATYPE use_item_num_;
    DURATYPE current_item_num_;
    
};

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM