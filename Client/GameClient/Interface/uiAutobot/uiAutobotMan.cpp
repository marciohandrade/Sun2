#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotMan.h"
#include "uiAutobotConditionBattle.h"
#include "uiAutobotConditionSupport.h"
#include "uiAutobotResult.h"
#include "uiAutobotPlay.h"
#include "interfacemanager.h"

#include "Autobot/Condition/AutobotConditionManager.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------ 
uiAutobotMan::uiAutobotMan(InterfaceManager* manager) : 
    uiBaseMan(manager),
    active_tab_(NULL)
{
}

//------------------------------------------------------------------------------ 
void uiAutobotMan::OnInitialize()
{
    CreateUIDialog<uiAutobotConditionBattle>(uiAutobotConditionBattle::kDialog_ID, 
        "Data\\Interface\\95_1_Battle.iwz", this);

    CreateUIDialog<uiAutobotConditionSupport>(uiAutobotConditionSupport::kDialog_ID, 
        "Data\\Interface\\95_2_Support.iwz", this);

    CreateUIDialog<uiAutobotResult>(uiAutobotResult::kDialog_ID, 
        "Data\\Interface\\95_4_Result.iwz", this);

    CreateUIDialog<uiAutobotPlay>(uiAutobotPlay::kDialog_ID, 
        "Data\\Interface\\95_3_edit.iwz", this);
}

//------------------------------------------------------------------------------ 
void uiAutobotMan::ChangeTab(DWORD dialog_id)
{
    float cx = -1.0f;
    float cy = -1.0f;


    if (active_tab() != NULL)
    {
        cx = active_tab()->GetDialogWZ()->GetDlgX();
        cy = active_tab()->GetDialogWZ()->GetDlgY();

        active_tab()->ShowInterface(FALSE);
    }

    set_active_tab(static_cast<uiBase*>(g_InterfaceManager.GetDialog(dialog_id)));

    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);

        if (cx >= 0.0f && cy >= 0.0f)
        {
            active_tab()->MoveWindowWZ(cx, cy);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotMan::ShowTab()
{
    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);
    }
    else
    {
        ChangeTab(uiAutobotConditionBattle::kDialog_ID);
    }

    //------------------------------------------------------------------------------ 
    uiAutobotConditionBattle* battle_dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
    uiAutobotConditionSupport* support_dialog = GET_CAST_DIALOG(uiAutobotConditionSupport, uiAutobotConditionSupport::kDialog_ID);
    if (battle_dialog != NULL)
    {
        if (battle_dialog->IsLoaded() == FALSE)
        {
            battle_dialog->LoadInterface();
        }
        battle_dialog->DefaultUI();
        battle_dialog->UpdateUI();
    }
    if (support_dialog != NULL)
    {
        if (support_dialog->IsLoaded() == FALSE)
        {
            support_dialog->LoadInterface();
        }
        support_dialog->DefaultUI();
        support_dialog->UpdateUI();
    }
}
//------------------------------------------------------------------------------ 
bool uiAutobotMan::UpdateCondition()
{
    uiAutobotConditionBattle* battle_dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
    uiAutobotConditionSupport* support_dialog = GET_CAST_DIALOG(uiAutobotConditionSupport, uiAutobotConditionSupport::kDialog_ID);
    if (battle_dialog == NULL || support_dialog == NULL)
    {
        return false;
    }

    AutobotConditionManager::Instance()->Clear();
    {
        battle_dialog->UpdateCondition();
        support_dialog->UpdateCondition();
    }

    SLOTCODE item_code = 0;
    if (g_pHero->GetHeroEquipContainer()->IsEquipItem(eITEMTYPE_AUTO_HUNTING, &item_code) == true)
    {
        //! 저장하기 전에 유효성 검사
        AutobotConditionManager::Instance()->CheckValid(item_code);
    }
#ifdef _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET
    if (AutobotConditionManager::Instance()->SendCheck() == false)
    {
        AutobotConditionManager::Instance()->set_is_loaded(true);
    }
    else
#endif //_SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET
    {
        MSG_CG_ETC_GAMEOPTION_SAVE_SYN syn;
        ZeroMemory(&(syn.game_option_), sizeof(syn.game_option_));

        ArchiveOfStream ar(syn.game_option_.binary_data_, MAX_GAMEOPTIONSTREAM_SIZE);
        AutobotConditionManager::Instance()->SaveToBuffer(ar);

        syn.game_option_.option_type_ = GameOption::kAutoHunting;
        syn.game_option_.binary_data_size_ = ar.GetSize();

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &syn, syn.GetSize());
    }

    return true;

}

#endif //_YMS_AUTOBOT_GAME