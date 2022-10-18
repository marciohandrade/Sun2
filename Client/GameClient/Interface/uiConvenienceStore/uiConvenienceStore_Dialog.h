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
        kAskShow_StorageBuy_AllRecv_Confirm, //!< �ް� ���� Ȯ��â
        kAskShow_StorageGift_OneRecv,
        kAskShow_StorageGift_AllRecv,
        kAskShow_StorageGift_AllRecv_Confirm, //!< �ް� ���� Ȯ��â
        kAskShow_Message,
        kAskShow_Select_CashType,   // ���Ҽ����� ����

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
        kPopupCommand_MyPage_Msg = 20, //!< �޼�������
        kPopupCommand_MyPage_Recv,      //!< �ޱ�

        //! ��ٱ���
        kPopupCommand_Basket_Buy = 30,
        kPopupCommand_Basket_Gift,
        kPopupCommand_Basket_Equip, 
        kPopupCommand_Basket_Preview, 
        kPopupCommand_Basket_Delete,
        kPopupCommand_Basket_Size,

        //! ����
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
    //! Qualifier: ���� �� �˾� �޴��� �����ټ� �ִ� ��������?
    //------------------------------------------------------------------------------
    virtual bool IsPossibleShopPopup() { return true; }

    //------------------------------------------------------------------------------
    //! Returns:   bool : true �� ��� ���� false : ����
    //! Qualifier: �˾� �޴��� ��� �� �ִ��� ����
    //! Parameter: bool is_possible : ���� �˾� �޴��� ���� �ִ��� ����
    //! Parameter: bool is_now_show_tooltip : ���� ������ �� �ִ��� ���� (������ �� �̸�� �˾� �ȳ����� �� �޶���.)
    //------------------------------------------------------------------------------
    virtual bool UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip);

};
