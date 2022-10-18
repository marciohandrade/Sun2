#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_CashType.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Ask.h"

enum UIControlPos
{
    kControl_Button_Close = 0,
    kControl_Text_Notice1,
    kControl_Text_Notice2,
    kControl_Text_CCoin,
    kControl_Text_PCoin,
    kControl_Button_CCoin,
    kControl_Button_PCoin,
    kControl_Size,
};
//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_CashType::kDialog_ID = StrToWzID("4714");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_CashType::uiConvenienceStore_CashType(InterfaceManager* ui_man)
:uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_Close);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_Notice1);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_Notice2);
    RegisterControlPosition(StrToWzID("T101"), kControl_Text_CCoin);
    RegisterControlPosition(StrToWzID("T102"), kControl_Text_PCoin);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_CCoin);
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_PCoin);
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_CashType::~uiConvenienceStore_CashType( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::Init(CDrawBase* drawbase)
{
    ASSERT(true && "uiConvenienceStore_CashType::Init false");

    GlobalFunc::SetWordWarpText( GetControlWZ_byPosition( kControl_Text_Notice1) );
    GlobalFunc::SetWordWarpText( GetControlWZ_byPosition( kControl_Text_Notice2) );
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::Release(void)
{

}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg)); 
        msg.dwType = uiConvenienceStore_CashType::kDialog_ID;
        msg.wParam = uiConvenienceStore_CashType::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 
//
//#if WZENGINEVER >= 410
//        //155360	알림
//        GetControlWZ_byPosition(kControl_Text_S000)->SetTextID(155360);
//#endif //#if WZENGINEVER >= 410

        g_InterfaceManager.LockDlgs(uiConvenienceStore_CashType::kDialog_ID);
    }
    else
    {
        g_InterfaceManager.UnLockDlgs();
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_CashType::kDialog_ID);

        //if ((owner() != NULL) &&
        //    (owner()->IsVisible() == TRUE) && 
        //    (owner()->IsNeedLockDialog() == true)) 
        //{
        //    g_InterfaceManager.LockDlgs(owner()->GetDialogWZ()->GetDlgID());
        //}
        //else
        //{
        //    g_InterfaceManager.UnLockDlgs();
        //}
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::OnUpdateSolarDialog()
{
#if WZENGINEVER >= 410
    DWORD lock_dlg_id = g_InterfaceManager.GetDialogManager()->GetLockDlgID();
    if ((lock_dlg_id != 0) && (lock_dlg_id != uiConvenienceStore_CashType::kDialog_ID))
    {
        return; 
    }
#endif //#if WZENGINEVER >= 410

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::OnRenderSolarDialog()
{ 
#if WZENGINEVER >= 410
    if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_CashType::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);
        GetDialogWZ()->OnDraw(true);
    }
#endif //#if WZENGINEVER >= 410

    //ftext_render().Render();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_CashType::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiConvenienceStore_CashType::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    //if (GetManager() == NULL)
    //{
    //    return;
    //}

    //------------------------------------------------------------------------------ 
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;

    default:
        uiBase::MessageProc(message);
        //VScrollDialog::MessageProc(message);
        break;
    }
}

//------------------------------------------------------------------------------
bool uiConvenienceStore_CashType::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_CCoin:
        {
            uiBase::ShowInterface(FALSE);
            _OnSelectCashType( ConvenienceStore::Define::kCashType_Cash);
        }
        break;
    case kControl_Button_PCoin:
        {
            uiBase::ShowInterface(FALSE);
            _OnSelectCashType( ConvenienceStore::Define::kCashType_PPCard);
        }
        break;
    case kControl_Button_Close:
        {
            uiBase::ShowInterface(FALSE);
        }
        break;
    }
    return result;
}

void uiConvenienceStore_CashType::OpenSelectCashType()
{
    uiBase::ShowInterface(TRUE);

#if defined(_GLOBAL)
    double money_cp = ConvenienceStore::CashShop::instance()->GetMyCash();
    double money_pp = ConvenienceStore::CashShop::instance()->GetMyPPCash();

    if (GetControlWZ_byPosition(kControl_Text_CCoin))
        GetControlWZ_byPosition(kControl_Text_CCoin)->SetMoney(static_cast<ULONGLONG>(money_cp));
    if (GetControlWZ_byPosition(kControl_Text_PCoin))
        GetControlWZ_byPosition(kControl_Text_PCoin)->SetMoney(static_cast<ULONGLONG>(money_pp));
#elif defined(_YJW_SHOP_USING_MILEAGE)
#else //_KOREA
#endif //

}

void uiConvenienceStore_CashType::_OnSelectCashType( ConvenienceStore::Define::CashType cash_type )
{
    if( buy_product_item_ != NULL )
    {
        // 단일 상품 구입
        GetManager()->purchase()->SetCashType( cash_type );
        GetManager()->purchase()->OpenPurchase( uiConvenienceStore_Purchase::kPurchase_Buy );
        GetManager()->purchase()->InsertPurchase( buy_product_item_, buy_product_basket_index );
    }
    else
    {
        // 장바구니 구입

        // 구매 가능한 아이템의 목록을 만든다
        std::vector<ConvenienceStore::ProductItem*> array;
        for( size_t i=0;i!=GetManager()->basket()->basket_list().size();++i)
        {
            ConvenienceStore::ProductItem* item = GetManager()->basket()->basket_list().at(i);
            if( item->is_valid_cashtype( cash_type ) )
            {
                array.push_back( item );
            }
        }

        if( array.empty() )
        {
            // 구입 가능한 아이템이 없다.
            GetManager()->ask()->OpenAsk( NULL, uiConvenienceStore_Dialog::kAskShow_Message );
            GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155997) );
            GetManager()->ask()->EnableCloseButton();
        }
        else
        {
            GetManager()->purchase()->SetCashType( cash_type );
            GetManager()->purchase()->OpenPurchase( uiConvenienceStore_Purchase::kPurchase_Buy );
            for( size_t i=0;i!=array.size();++i)
            {
                GetManager()->purchase()->InsertPurchase( array[i], i );
            }
        }
    }
}
