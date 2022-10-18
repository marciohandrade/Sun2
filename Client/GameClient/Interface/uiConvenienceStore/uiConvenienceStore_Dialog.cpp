#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Dialog.h"
#include "uiConvenienceStore_PopupMenu.h"
#include "uiConvenienceStore_PopupMenuFive.h"
#include "uiConvenienceStore_PopupMenuFour.h"
#include "uiConvenienceStore_PopupMenuTwo.h"
#include "uiConvenienceStore_PopupMenuThree.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "Mouse.h"

//------------------------------------------------------------------------------ 
uiConvenienceStoreMan* uiConvenienceStore_Dialog::GetManager()
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    return static_cast<uiConvenienceStoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CONVENIENCESTORE));
#else
    return NULL;
#endif
}

//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Dialog::UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip)
{
    if (is_possible == false || is_now_show_tooltip == true)
    {
        if ((GetManager()->popup_menu_five()->owner() == NULL) || (GetManager()->popup_menu_five()->owner() == this))
        {
            GetManager()->popup_menu_five()->ShowInterface(FALSE);
        }
        if ((GetManager()->popup_menu_four()->owner() == NULL) || (GetManager()->popup_menu_four()->owner() == this))
        {
            GetManager()->popup_menu_four()->ShowInterface(FALSE);
        }
        if ((GetManager()->popup_menu_three()->owner() == NULL) || (GetManager()->popup_menu_three()->owner() == this))
        {
            GetManager()->popup_menu_three()->ShowInterface(FALSE);
        }
        if ((GetManager()->popup_menu_two()->owner() == NULL) || (GetManager()->popup_menu_two()->owner() == this))
        {
            GetManager()->popup_menu_two()->ShowInterface(FALSE);
        }
        return false;
    }
    for (int i = 0; i < ConvenienceStore::Define::kPopupMenuKind_Size; ++i)
    {
        uiConvenienceStore_PopupMenu* popup = GetManager()->popup_menu(static_cast<ConvenienceStore::Define::PopupMenuKind>(i));
        //if ((popup->owner() != NULL) && (popup->owner() != this))
        //{ 
        //    return false;
        //}
        if (popup->IsVisible() == TRUE)
        {
            if ((popup->InterSectRect(Mouse::Instance()->CheckMousePos()) == TRUE) ||
                (IsOverControl(popup->owner_control()) == true))
            {
                return false;
            }
        }
    }

    return true;
}