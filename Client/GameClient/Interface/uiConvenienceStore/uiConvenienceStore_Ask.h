#pragma once

#include "uiBase.h"
#include "uiConvenienceStoreMan.h"
//#include "uiConvenienceStore_Purchase.h"
//#include "uiConvenienceStore_Dialog.h"
#include "VScrollDialog.h"
#include "FTextRender.h"

class uiConvenienceStoreMan;
class uiConvenienceStore_Dialog;
class ConvenienceStore::ShopItemObject;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_Ask : public uiBase, public VScrollDialog
{
public:
    static const DWORD kDialog_ID;



private:
    FTextRender	ftext_render_;

    //! 호출한 다이얼로그
    uiConvenienceStore_Dialog* owner_;

    //! OK 버튼 클릭시 넘겨 준다.
    void* user_data_;
    uiConvenienceStore_Dialog::AskShowType ask_show_type_;

#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    ConvenienceStore::ShopItemObject* is_now_ask_item_;
#endif


public:
    void OpenAsk( uiConvenienceStore_Dialog* owner, uiConvenienceStore_Dialog::AskShowType ask_show_type );
    void AppendItemText( ConvenienceStore::ShopItemObject* object );

#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    void AppendItemTextToString(DWORD index,ConvenienceStore::ShopItemObject* object);
#endif

    void AppendFText(const TCHAR* msg);
    void DisableCloseButton();
    void EnableCloseButton();
    bool IsEnableCloseButton();

    // user_data는 아이템을 개별로 살때의 아이템의 포인터임 (ConvenienceStore::ShopItemObject*) 
    void SetItemObject( ConvenienceStore::ShopItemObject* item_object) { user_data_ = item_object; } 

#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    ConvenienceStore::ShopItemObject* GetNowAskItem() { return is_now_ask_item_; }
#endif

public:
    uiConvenienceStore_Ask(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_Ask(void);

private:
    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void NetworkProc(MSG_BASE* msgbase);
    virtual void MessageProc(SI_MESSAGE* message);


private:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool GetScrollArea(RECT& area);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

private:
    uiConvenienceStore_Dialog* owner() const { return owner_; }
    FTextRender& ftext_render() { return ftext_render_; }

    void UpdateTextScroll();

    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void OnOK();
    void OnCancel();

    void InitFText(FTextParser& parser);
    void UpdateButtonState();

    void MakeItemFullName( ConvenienceStore::ShopItemObject* item_object_ptr, OUT LPTSTR full_name );

    static void _OnClickEscape(DWORD wParam,DWORD lParam); // ESC 입력처리를 위해

};
