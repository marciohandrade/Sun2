#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_PopupMenu.h"
#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"

//------------------------------------------------------------------------------ 
uiConvenienceStoreMan* uiConvenienceStore_PopupMenu::GetManager()
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    return static_cast<uiConvenienceStoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CONVENIENCESTORE));
#else
    return NULL;
#endif
    
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenu::Reset()
{
    command_map().clear();
#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
    set_extra_text_width(0);
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI   
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenu::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
    }
    else
    {
        //if (owner() != NULL)
        //{
        //    g_InterfaceManager.GetDialogManager()->RequestFocus(owner()->GetDialogWZ());
        //}
        set_owner(NULL, NULL);
    }
}