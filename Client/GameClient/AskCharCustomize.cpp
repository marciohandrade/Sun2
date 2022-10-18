#include "SunClientPrecompiledHeader.h"
#include "AskCharCustomize.h"
#include "GameConst_Color.h"
#include "InterfaceManager.h"
#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "Hero.h"



AskCharCustomize::AskCharCustomize(InterfaceManager *pUIMan)
:AskDialog(pUIMan),
customize_type_(kBase)
{

}

AskCharCustomize::~AskCharCustomize()
{

}

void AskCharCustomize::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
        this->DoIt();
        SetStatus(ASK_STATUS_NORMAL);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        this->Cancel();
        SetStatus(ASK_STATUS_NORMAL);
    }
}
void AskCharCustomize::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
                }
                break;
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc( pMessage );
        }
        break;
    }
}

VOID AskCharCustomize::OnShowWindow( BOOL val )
{
    if( val )
    {
        MoveCenter();
        SetTexts();
    }
    else
    {
        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
        if ((inventory_manager != NULL) &&
            (customize_type() == KGender))
        {
            inventory_manager->UnlockInventoryUI();
        }
    }
}

void AskCharCustomize::SetTexts()
{
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};

    int string_code = 2139;

    switch(customize_type())
    {
    case kHair:
        {
            string_code = 2137;
        }
        break;
    case kFace:
        {
            string_code = 2138;
        }
        break;
    case KGender:
        {
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (inventory_manager != NULL)
            {
                inventory_manager->LockInventoryUI();
            }

            TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR class_name[INTERFACE_STRING_LENGTH] = {'\0', };
            int class_name_code = GlobalFunc::GetClassNameCode(static_cast<BYTE>(g_pHero->GetClass()));
            // 5990 %s 클래스의 성별을 변경 하시겠습니까?
            g_InterfaceManager.GetInterfaceString(5990, temp_message);
            g_InterfaceManager.GetInterfaceString(class_name_code, class_name);
            Snprintf(message, INTERFACE_STRING_LENGTH - 1, temp_message, class_name);
            InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);

            return;
        }
        break;
    }

    //선택하신 string_code 로 변경하시겠습니까?
    g_InterfaceManager.GetInterfaceString(string_code, message);
    InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}

void AskCharCustomize::DoIt()
{
    ShowInterface(FALSE);

    if (customize_type() == KGender)
    {
        // 성별변경 아이템 사용 패킷 전송
        MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN packet;
        packet.item_position_ = slot_position();
        packet.character_appearance_.appearance_type_ = ChangeCharacterAppearanceTypeConst::kGender;
        packet.character_appearance_.gender = change_gender();
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));

        return;
    }
    
    uiCharCustomizeDlg* char_customize_dlg = GET_CAST_DIALOG( uiCharCustomizeDlg,
        IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG );

    if (char_customize_dlg &&
        char_customize_dlg->IsVisibleWindow())
    {
        char_customize_dlg->SendCharCustomizePacket();
        char_customize_dlg->ShowInterface(FALSE);
    }
}
void AskCharCustomize::Cancel()
{
    ShowInterface(FALSE);
}
