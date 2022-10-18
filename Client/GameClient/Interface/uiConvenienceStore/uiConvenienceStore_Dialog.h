#pragma once

#include "uiBase.h"

class uiConvenienceStoreMan;



//------------------------------------------------------------------------------ 
class uiConvenienceStore_Dialog : public uiBase //VScrollDialog
{
public:
    enum AskShowType
    {
        kAskShow_None = 0,
        kAskShow_Buy,
        kAskShow_Gift,
        kAskShow_StorageBuy_OneRecv,
        kAskShow_StorageBuy_AllRecv,
        kAskShow_StorageBuy_AllRecv_Confirm, //!< 받고 나서 확인창
        kAskShow_StorageGift_OneRecv,
        kAskShow_StorageGift_AllRecv,
        kAskShow_StorageGift_AllRecv_Confirm, //!< 받고 나서 확인창
        kAskShow_Message,
        kAskShow_Select_CashType,   // 지불수단을 선택

        KAskShow_ItemCashNewType,
        KAskShow_ItemCashGifeNewType,
        kAskShow_StorageNewTypeBuy_AllRecv,
        kAskShow_StorageNewTypeGift_AllRecv,
        kAskShow_Size,
    };
    enum AskCommandType
    {
        kAskCommand_OK = 1,
        kAskCommand_Cancel,
        kAskCommand_Size,
    };

    //------------------------------------------------------------------------------ 
    enum PopupCommandType
    {
        kPopupCommand_None = 0,

        //! Counter
        kPopupCommand_Counter_Pickup = 10,
        kPopupCommand_Counter_Preview,
        kPopupCommand_Counter_Buy, 
        kPopupCommand_Counter_Gift,

        //! 
        kPopupCommand_Pickup_Storage,
        kPopupCommand_Pickup_Gift,

        //! MyPage
        kPopupCommand_MyPage_Msg = 20, //!< 메세지보기
        kPopupCommand_MyPage_Recv,      //!< 받기

        //! 장바구니
        kPopupCommand_Basket_Buy = 30,
        kPopupCommand_Basket_Gift,
        kPopupCommand_Basket_Equip, 
        kPopupCommand_Basket_Preview, 
        kPopupCommand_Basket_Delete,
        kPopupCommand_Basket_Size,

        //! 장착
        kPopupCommand_Equip_Buy = 40,
        kPopupCommand_Equip_Gift,
        kPopupCommand_Equip_Unequip, 
    };


public:
    uiConvenienceStore_Dialog(InterfaceManager* ui_man) : uiBase(ui_man) {}
    virtual ~uiConvenienceStore_Dialog(void) {}

    uiConvenienceStoreMan* GetManager();
    virtual bool IsNeedLockDialog() { return false; }
    virtual bool OnClickPopupMenu(const uiConvenienceStore_Dialog::PopupCommandType command_type, const DWORD user_data){ return false; } 
    virtual bool OnClickAskDialog(uiConvenienceStore_Dialog::AskCommandType command_type, uiConvenienceStore_Dialog::AskShowType show_type, void* user_data){ return false; } 

protected:
    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: 툴팁 및 팝업 메뉴를 보열줄수 있는 상태인지?
    //------------------------------------------------------------------------------
    virtual bool IsPossibleShopPopup() { return true; }

    //------------------------------------------------------------------------------
    //! Returns:   bool : true 면 계속 진행 false : 리턴
    //! Qualifier: 팝업 메뉴를 띄울 수 있는지 여부
    //! Parameter: bool is_possible : 현재 팝업 메뉴를 띄울수 있는지 여부
    //! Parameter: bool is_now_show_tooltip : 현재 툴팁이 떠 있는지 여부 (툴팁이 떠 이면면 팝업 안나오게 해 달란다.)
    //------------------------------------------------------------------------------
    virtual bool UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip);

};
