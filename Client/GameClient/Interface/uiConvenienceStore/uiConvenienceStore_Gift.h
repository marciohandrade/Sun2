#pragma once

//#ifdef _YMS_TREECONTROL

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "uiConvenienceStore_Basket.h"

class uiConvenienceStoreMan;
class uiConvenienceStore_FriendList;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_Gift : public uiConvenienceStore_Dialog
{
    friend class uiConvenienceStore_FriendList;
    friend class uiConvenienceStoreMan;

public:
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B028,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Text_S000,
        kControl_Text_S002,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S001,
        kControl_Edit_E000,
        kControl_Edit_E001,
        kControl_Picture_P002,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P003,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiConvenienceStore_Gift(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Gift(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick){}
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);

    virtual void ShowInterface(bool is_show, 
                            std::vector<ConvenienceStore::ProductItem*>& basket_vector);
    virtual void ShowInterface(bool is_show, 
                            ConvenienceStore::ProductItem* product);

    //! ���� �Էµ� ĳ���͸� ģ������Ʈ���� �̸��� Ŭ���� ���ؼ� ���� �̸��̸� ���ֱ� ����
    void GetRecvCharacterName(TCHAR* name, int buffer_len);

    std::vector<ConvenienceStore::ProductItem*>& product_list() { return product_list_; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnCharDown(SI_MESSAGE* message);
    bool OnKeyDown(SI_MESSAGE* message);
    bool OnEnterkeyDown(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void SetRecvCharInfo(const TCHAR* char_name, const LEVELTYPE char_level, const TCHAR* guild_name);
    void CheckOk(const TCHAR* char_name, const LEVELTYPE char_level, const TCHAR* guild_name);
    void CheckFail();

    //! �������� ���� ������.
    //! new ���� �ʱ� ������ ������ delete ������ �ʴ´�.
    std::vector<ConvenienceStore::ProductItem*> product_list_;
};

//#endif //#ifdef _YMS_TREECONTROL