#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_PopupPackage.h"
#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Dialog.h"
#include "GlobalFunc.h"
#include "ProductInfo.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_PopupPackage::kDialog_ID = StrToWzID("4710");

//------------------------------------------------------------------------------ 
//! static function
void uiConvenienceStore_PopupPackage::Callback_IconRender(CControlWZ* control_ptr, void* parameter)
{
    uiConvenienceStore_Counter* onwer = static_cast<uiConvenienceStore_Counter*>(parameter);
    if ((control_ptr != NULL) && (onwer != NULL))
    {
        if( ConvenienceStore::ProductInfo* info = reinterpret_cast<ConvenienceStore::ProductInfo*>(control_ptr->GetUserData()) )
        {
            if( IconImage* pIconImage = info->GetIconImage() )
            {
                RECT rc = control_ptr->GetSizeRect();
                pIconImage->Render(&rc);
            }
        }
    }
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupPackage::uiConvenienceStore_PopupPackage(InterfaceManager* ui_man) : uiConvenienceStore_PopupMenu(ui_man)
{
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S028"), kControl_Text_S028);
    RegisterControlPosition(StrToWzID("S029"), kControl_Text_S029);
    RegisterControlPosition(StrToWzID("S030"), kControl_Text_S030);
    RegisterControlPosition(StrToWzID("S031"), kControl_Text_S031);
    RegisterControlPosition(StrToWzID("S032"), kControl_Text_S032);
    RegisterControlPosition(StrToWzID("S033"), kControl_Text_S033);
    RegisterControlPosition(StrToWzID("S034"), kControl_Text_S034);
    RegisterControlPosition(StrToWzID("S035"), kControl_Text_S035);
    RegisterControlPosition(StrToWzID("S036"), kControl_Text_S036);
    RegisterControlPosition(StrToWzID("S037"), kControl_Text_S037);
    RegisterControlPosition(StrToWzID("S038"), kControl_Text_S038);
    RegisterControlPosition(StrToWzID("S039"), kControl_Text_S039);
    RegisterControlPosition(StrToWzID("S040"), kControl_Text_S040);
    RegisterControlPosition(StrToWzID("S041"), kControl_Text_S041);
    RegisterControlPosition(StrToWzID("S042"), kControl_Text_S042);
    RegisterControlPosition(StrToWzID("S043"), kControl_Text_S043);
    RegisterControlPosition(StrToWzID("S044"), kControl_Text_S044);
    RegisterControlPosition(StrToWzID("S045"), kControl_Text_S045);
    RegisterControlPosition(StrToWzID("S046"), kControl_Text_S046);
    RegisterControlPosition(StrToWzID("S047"), kControl_Text_S047);
    RegisterControlPosition(StrToWzID("S048"), kControl_Text_S048);
    RegisterControlPosition(StrToWzID("S049"), kControl_Text_S049);
    RegisterControlPosition(StrToWzID("S050"), kControl_Text_S050);
    RegisterControlPosition(StrToWzID("S051"), kControl_Text_S051);
    RegisterControlPosition(StrToWzID("S052"), kControl_Text_S052);
    RegisterControlPosition(StrToWzID("S053"), kControl_Text_S053);
    RegisterControlPosition(StrToWzID("S054"), kControl_Text_S054);
    RegisterControlPosition(StrToWzID("S055"), kControl_Text_S055);
    RegisterControlPosition(StrToWzID("S056"), kControl_Text_S056);

    RegisterControlPosition(StrToWzID("P059"), KControl_Image_P059);
    RegisterControlPosition(StrToWzID("P087"), KControl_Image_P087);
    RegisterControlPosition(StrToWzID("P088"), KControl_Image_P088);
    RegisterControlPosition(StrToWzID("P089"), KControl_Image_P089);
    RegisterControlPosition(StrToWzID("P090"), KControl_Image_P090);
    RegisterControlPosition(StrToWzID("P091"), KControl_Image_P091);
    RegisterControlPosition(StrToWzID("P092"), KControl_Image_P092);
    RegisterControlPosition(StrToWzID("P093"), KControl_Image_P093);
    RegisterControlPosition(StrToWzID("P094"), KControl_Image_P094);
    RegisterControlPosition(StrToWzID("P095"), KControl_Image_P095);
    RegisterControlPosition(StrToWzID("P002"), KControl_Image_P002);
    RegisterControlPosition(StrToWzID("P060"), KControl_Image_P060);
    RegisterControlPosition(StrToWzID("P061"), KControl_Image_P061);
    RegisterControlPosition(StrToWzID("P062"), KControl_Image_P062);
    RegisterControlPosition(StrToWzID("P063"), KControl_Image_P063);
    RegisterControlPosition(StrToWzID("P064"), KControl_Image_P064);
    RegisterControlPosition(StrToWzID("P065"), KControl_Image_P065);
    RegisterControlPosition(StrToWzID("P066"), KControl_Image_P066);
    RegisterControlPosition(StrToWzID("P067"), KControl_Image_P067);
    RegisterControlPosition(StrToWzID("P068"), KControl_Image_P068);
    RegisterControlPosition(StrToWzID("P021"), KControl_Image_P021);
    RegisterControlPosition(StrToWzID("P069"), KControl_Image_P069);
    RegisterControlPosition(StrToWzID("P070"), KControl_Image_P070);
    RegisterControlPosition(StrToWzID("P071"), KControl_Image_P071);
    RegisterControlPosition(StrToWzID("P072"), KControl_Image_P072);
    RegisterControlPosition(StrToWzID("P073"), KControl_Image_P073);
    RegisterControlPosition(StrToWzID("P074"), KControl_Image_P074);
    RegisterControlPosition(StrToWzID("P075"), KControl_Image_P075);
    RegisterControlPosition(StrToWzID("P076"), KControl_Image_P076);
    RegisterControlPosition(StrToWzID("P077"), KControl_Image_P077);
    RegisterControlPosition(StrToWzID("P022"), KControl_Image_P022);
    RegisterControlPosition(StrToWzID("P078"), KControl_Image_P078);
    RegisterControlPosition(StrToWzID("P079"), KControl_Image_P079);
    RegisterControlPosition(StrToWzID("P080"), KControl_Image_P080);
    RegisterControlPosition(StrToWzID("P081"), KControl_Image_P081);
    RegisterControlPosition(StrToWzID("P082"), KControl_Image_P082);
    RegisterControlPosition(StrToWzID("P083"), KControl_Image_P083);
    RegisterControlPosition(StrToWzID("P084"), KControl_Image_P084);
    RegisterControlPosition(StrToWzID("P085"), KControl_Image_P085);
    RegisterControlPosition(StrToWzID("P086"), KControl_Image_P086);
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupPackage::~uiConvenienceStore_PopupPackage( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupPackage::Init(CDrawBase* drawbase)
{
    ASSERT(true && "uiConvenienceStore_PopupPackage::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupPackage::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        int a = 0;
    }
    else
    {
        for (int i = 0; i < kListPerCount; ++i)
        {
            if (GetControlWZ_byPosition(kControl_Button_B000 + i))
                GetControlWZ_byPosition(kControl_Button_B000 + i)->SetUserData(NULL);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupPackage::OnUpdateSolarDialog()
{
    UpdateTooltip();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupPackage::ShowInterface(bool is_show, ConvenienceStore::ProductItem* product)
{
    if ((product == NULL) || (product->is_package() == false)) 
    {
        return;
    }

    uiConvenienceStore_PopupMenu::ShowInterface(is_show); 

    if (is_show)
    {
        for (int i = 0; i < kListPerCount; ++i)
        {
            CControlWZ* name_control = GetControlWZ_byPosition(kControl_Text_S000 + i);
            CControlWZ* count_control = GetControlWZ_byPosition(kControl_Text_S037 + i);
            CControlWZ* date_control = GetControlWZ_byPosition(kControl_Text_S047 + i);
            CControlWZ* picture_control = GetControlWZ_byPosition(kControl_Button_B000 + i);
            
            CControlWZ* panel_control1 = GetControlWZ_byPosition(KControl_Image_P059 + i);
            CControlWZ* panel_control2 = GetControlWZ_byPosition(KControl_Image_P002 + i);
            CControlWZ* panel_control3 = GetControlWZ_byPosition(KControl_Image_P021 + i);
            CControlWZ* panel_control4 = GetControlWZ_byPosition(KControl_Image_P022 + i);

            if (name_control == NULL || 
                count_control == NULL || 
                date_control == NULL || 
                picture_control == NULL ||
                panel_control1 == NULL ||
                panel_control2 == NULL ||
                panel_control3 == NULL ||
                panel_control4 == NULL)
            {
                continue;
            }

            if (i < (int)product->GetInfoVector().size())
            {
                picture_control->ShowWindowWZ(WZ_SHOW);
                name_control->ShowWindowWZ(WZ_SHOW);
                count_control->ShowWindowWZ(WZ_SHOW);
                date_control->ShowWindowWZ(WZ_SHOW);
                panel_control1->ShowWindowWZ(WZ_SHOW);
                panel_control2->ShowWindowWZ(WZ_SHOW);
                panel_control3->ShowWindowWZ(WZ_SHOW);
                panel_control4->ShowWindowWZ(WZ_SHOW);

                std::vector<ConvenienceStore::ProductInfo>::const_reference product_info = product->GetInfoVector().at(i);
                picture_control->SetUserData(reinterpret_cast<DWORD>(&product_info));
                picture_control->SetCallbackDraw(uiConvenienceStore_PopupPackage::Callback_IconRender, this);

                //155524	%s
                //155525    %s°³
                //155526	%sÀÏ
                GlobalFunc::SetTextFromStringTable(name_control, 155524, (TCHAR*)product_info.GetProductName());
                if (product_info.GetProductType() == ConvenienceStore::Define::kProductType[ConvenienceStore::Define::kShopProduct_Date])
                {
                    GlobalFunc::SetTextFromStringTable(count_control, 155525, 1);

                    long sec_value = (long)product_info.GetPriceProperty().GetValue();
                    int day_value = sec_value / 60 / 60 / 24;
                    GlobalFunc::SetTextFromStringTable(date_control, 155526, day_value);
                }
                else //if ((product_info.shop_product->ProductType == ConvenienceStore::Define::kProductType[ConvenienceStore::Define::kShopProduct_Waste]) || (product_info.shop_product->ProductType == ConvenienceStore::Define::kProductType[ConvenienceStore::Define::kShopProduct_Infinite]))
                {
                    GlobalFunc::SetTextFromStringTable(count_control, 155525, product_info.GetPriceProperty().GetValue());
                    date_control->SetTextWZ("");
                }
            }
            else
            {
                picture_control->SetUserData(NULL);
                name_control->SetTextWZ("");
                count_control->SetTextWZ("");
                date_control->SetTextWZ("");

                picture_control->ShowWindowWZ(WZ_HIDE);
                name_control->ShowWindowWZ(WZ_HIDE);
                count_control->ShowWindowWZ(WZ_HIDE);
                date_control->ShowWindowWZ(WZ_HIDE);
                panel_control1->ShowWindowWZ(WZ_HIDE);
                panel_control2->ShowWindowWZ(WZ_HIDE);
                panel_control3->ShowWindowWZ(WZ_HIDE);
                panel_control4->ShowWindowWZ(WZ_HIDE);
            }
        }

        RECT area;
        GetDialogWZ()->GetDlgSize(&area);  
        float height = 15 + (21.0f * product->GetProductCount());
        MoveWindowWZ((float)area.left, (float)area.top, (float)area.right, height);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupPackage::UpdateTooltip()
{
    for (int i = 0; i < kListPerCount; ++i)
    {
        CControlWZ* picture_control = GetControlWZ_byPosition(kControl_Button_B000 + i);
        if ((IsOverControl(picture_control) == true) && (picture_control->GetUserData() != NULL))
        {
            ConvenienceStore::ProductInfo* info = reinterpret_cast<ConvenienceStore::ProductInfo*>(picture_control->GetUserData());
            GetManager()->RegisterTooltip(info,0);
        }
    }
}