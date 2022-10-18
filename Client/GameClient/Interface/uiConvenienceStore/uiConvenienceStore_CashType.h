#pragma once

#include "uiBase.h"
#include "uiConvenienceStoreMan.h"

//------------------------------------------------------------------------------ 
class uiConvenienceStore_CashType : public uiConvenienceStore_Dialog
{
    ConvenienceStore::ProductItem* buy_product_item_; // NULL 인경우, 장바구니에서의 구입을 의미
    int buy_product_basket_index; // 장바구니에서 구입할 경우의 해당 인덱스, 카운터에서 바로 구입할 경우 -1

public:
    void SetBuyProductInfo( ConvenienceStore::ProductItem* buy_product_item, int basket_index )
    {
        buy_product_item_ = buy_product_item;
        buy_product_basket_index = basket_index;
    }

public:
    static const DWORD kDialog_ID;

public:
    void OpenSelectCashType();

    uiConvenienceStore_CashType(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_CashType(void);

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
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnOK();
    void OnCancel();

    void _OnSelectCashType( ConvenienceStore::Define::CashType cash_type );
};
