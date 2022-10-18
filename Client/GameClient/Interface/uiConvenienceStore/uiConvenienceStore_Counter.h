#pragma once

#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "DropdownControl.h"
#include "TabCheckButtonControl.h"
#include "PageControl.h"
#include "CashShopHeader.h"
#include "uiConvenienceStore_Dialog.h"
#include "uiConvenienceStoreMan.h"

class uiConvenienceStoreMan;
class ConvenienceStore::ShopItemObject;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_Counter : public uiConvenienceStore_Dialog
{
    friend class uiConvenienceStoreMan;

    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,
        kControl_CheckBT_C020,  //!< 모두 받기
        kControl_CheckBT_C002,
        kControl_CheckBT_C021,  //!< pp 카드
        kControl_CheckBT_CT01,  //!< 썬 포인트
        kControl_CheckBT_CT02,  //!< 구매 내역 보기
        kControl_CheckBT_C033,  //!< 캐시아이템
        kControl_CheckBT_C034,  //!< 마이페이지

        kControl_Button_BT12,
        kControl_Button_BT13,
        kControl_Button_B004,
        kControl_Button_B000,
        kControl_Button_B006,
        kControl_Button_B042,
        kControl_Button_B043,
        kControl_Button_B044,
        kControl_Button_B045,
        kControl_Button_B046,
        kControl_Button_B047,
        kControl_Button_B048,
        kControl_Button_B049,
        kControl_Button_B050,
        kControl_Text_ST01,

#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Button_B008,
#else
        kControl_Text_S000,
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Text_S007,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Text_S011,
        kControl_Text_S012,
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S019,
        kControl_Text_S020,
        kControl_Text_S021,
        kControl_Text_S022,
        kControl_Text_S023,
        kControl_Text_S024,
        kControl_Text_S025,
        kControl_Text_S026,
        kControl_Text_S027,
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

        //kControl_Text_S001,
        //kControl_Text_S002,
        //kControl_Text_S003,
        //kControl_Text_S004,
        //kControl_Text_S005,
        //kControl_Text_S006,
        //kControl_Text_S057,
        //kControl_Text_S058,
        //kControl_Text_S059,
        //kControl_Text_S060,

#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_Button_B007,
#else
        kControl_Text_S061,
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
        kControl_List_L000,
        kControl_List_L001,
        kControl_List_L002,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_PI00,
        kControl_Picture_P004,
        kControl_Picture_P000,
        kControl_Picture_P005,
        kControl_Picture_P006,
        kControl_Picture_P007,
        kControl_Picture_P008,
        kControl_Picture_P009,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P013,
        kControl_Picture_P014,
        kControl_Picture_P015,
        kControl_Picture_P016,
        kControl_Picture_P017,
        kControl_Picture_P018,
        kControl_Picture_P019,
        kControl_Picture_P020,
        kControl_Picture_P021,
        kControl_Picture_P022,
        kControl_Picture_P023,
        kControl_Picture_P024,
        kControl_TextImage_B001,
        kControl_TextImage_I001,
        kControl_TextImage_I002,
        kControl_TextImage_I003,
        kControl_TextImage_I004,
        kControl_TextImage_I005,
        kControl_TextImage_I006,
        kControl_TextImage_I007,
        kControl_TextImage_I008,
        kControl_TextImage_I009,

        //이용약관 바로 가기
        KControl_button_B002,

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

    static const int kPagePerCount = 10;

    static void Callback_IconRender(CControlWZ* control_ptr, void* parameter);

public:
    uiConvenienceStore_Counter(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Counter(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick);
    virtual void OnUpdateSolarDialog();
    virtual void NetworkProc(MSG_BASE* msgbase);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual bool OnClickPopupMenu(const uiConvenienceStore_Dialog::PopupCommandType command_type, const DWORD user_data);
    virtual bool OnClickAskDialog(uiConvenienceStore_Dialog::AskCommandType command_type, uiConvenienceStore_Dialog::AskShowType show_type, void* user_data);


    ConvenienceStore::Define::MyPageType CurrentMyPageType();

    TreeMenuGroup& tree_menu() { return tree_menu_; }
    DropdownControl& dropdown_class_filter() { return dropdown_class_filter_; }
    DropdownControl& dropdown_sorting() { return dropdown_sorting_; }
    TabCheckButtonControl& menu_tab() { return menu_tab_; }
    PageControl& page_control() { return page_control_; }
    std::vector<ConvenienceStore::ProductItem*>& obj_list() { return obj_list_; }

    bool IsSellPage();
    bool IsMyStoragePage();
    bool IsGiftStoragePage();

    void UpdatePage();


protected:
    virtual bool IsPossibleShopPopup();
    virtual bool UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    
    void ChangeTabMenu(ConvenienceStore::Define::ShopMenuType menu_type, bool is_gift_menu );
    void ChangeTreeObject(ConvenienceStore::Define::ShopMenuType menu_type);

    void FillProductPage();
    void FillMyStoragePage();
    void FillGiftStoragePage();

    //! 현재 탭 메뉴 위치
    ConvenienceStore::Define::ShopMenuType CurrentShopMenuType();
    int GetCurrentSellCategorySequence();
    int GetCurrentSellCategoryImageType();
    eCHAR_TYPE GetFilterType();


    void ClearProductList();
    void UpdateTooltip(bool is_possible);
    bool IsShowTooltip();

    TreeMenuGroup tree_menu_;

    DropdownControl dropdown_class_filter_;
    DropdownControl dropdown_sorting_;
    TabCheckButtonControl menu_tab_;
    PageControl page_control_;

    std::vector<ConvenienceStore::ProductItem*> obj_list_;

public:
    void set_recv_all_stroage( bool val ) { is_recv_all_storage_ = val; }
    bool is_recv_all_stroage() { return is_recv_all_storage_; }

private:
    bool is_recv_all_storage_;

    int last_sell_category_sequence_;
    eCHAR_TYPE last_sell_category_class_filter_;
};

