#include "SunClientPrecompiledHeader.h"
#include "UIActionFunction.h"
#include "InterfaceManager.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiInventoryMan/uiInventoryDef.h"
#include "ExtendInventoryDialog.h"
#include "uiQuestMan/uiQuestMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "RadarFullDialog.h"
#include "uiConvenienceStore/uiConvenienceStoreMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "uiMissionObject/uiMissionObject.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "SummonSkillContainer.h"
#include "Hero.h"
#include "GameFramework.h"
#include "KeyQueueManager.h"
#include "HeroActionInput.h"
#include "GlobalFunc.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiOptionMan.h"
#endif

void UIActionFunction::ToggleCharacterStatusUI()
{
    uiCharacterStatusMan::ToggleCharacterStatusDialog();
}

void UIActionFunction::ToggleInventoryUI()
{
    SolarDialog* equip_dialog = g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_EQUIP);
    if (equip_dialog != NULL)
    {
        equip_dialog->ShowDialog(!(equip_dialog->IsVisible()));
    }
}

void UIActionFunction::ToggleExtendInventoryUI()
{
    ExtendInventoryDialog* extend_inventory_dialog = 
        GET_CAST_DIALOG(ExtendInventoryDialog, IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG);
    if (extend_inventory_dialog != NULL)
    {
        extend_inventory_dialog->ShowExtendInventory();
    }
}

void UIActionFunction::ToggleSkillUI()
{
    uiBase* skill_dialog = g_InterfaceManager.GetUISkillCurrentDialog();
    if (skill_dialog != NULL)
    {
        skill_dialog->ShowInterfaceWithSound(!(skill_dialog->IsVisible()));
    }
}

void UIActionFunction::ToggleQuestUI()
{
    uiQuestMan* quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (quest_manager != NULL)
    {
        quest_manager->OpenQuestList();
    }
}

void UIActionFunction::ToggleGuildUI()
{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    GuildSystem::Function::ToggleGuildInfoDialog();
#else
    uiGuildMan* guild_manager = GET_DIALOG_MANAGER(uiGuildMan, UIMAN_GUILD);
    if (guild_manager != NULL)
    {
        if (guild_manager->IsShowWindow() == true)
        {
            guild_manager->CloseWin();
        }
        else
        {
            guild_manager->SetShowGuildMain();
            guild_manager->SEND_CG_GUILD_TOTALINFO_REQ_SYN();
        }
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
}

void UIActionFunction::ToggleCommunityUI()
{
    if (GlobalFunc::IsSSQScene() == TRUE)
    {
        return;
    }

    uiCommunityMan* community_manager = GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);
    if (community_manager != NULL)
    {
        community_manager->ShowCommunityDlg();
    }
}

void UIActionFunction::ToggleACUI()
{
    uiBase* ac_dialog = GET_DIALOG(IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);
    if (ac_dialog != NULL) 
    {
        ac_dialog->ShowDialog(!(ac_dialog->IsVisible()));
    }
}

void UIActionFunction::TogglePetUI()
{
    if ((g_pHero != NULL) && 
        (g_pHero->IsPetReturn() == TRUE))
    {
        g_HeroInput.ToggleShowPetInfo();
    }
}

void UIActionFunction::ShowSystemMenuNCancel()
{
    g_KeyQueueManager.KeyQueueAction();
}

void UIActionFunction::ToggleSystemMenuUI()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	uiOptionMan::ToggleShowActionTap();
#else
	GlobalFunc::DoSystemDialog(0,0);
#endif
}

void UIActionFunction::ToggleContinentMap()
{
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ui_radar_manager->ToggleShowContinentMap();
    }
}

void UIActionFunction::ToggleBattleScoreUI()
{
    if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
    {
        return;
    }

    uiBattleScoreMan* battle_score_manager = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
    if ((battle_score_manager != NULL) && 
        (battle_score_manager->GetMode() != uiBattleScoreMan::eBTS_NONE_MODE) &&
        (battle_score_manager->GetMode() != uiBattleScoreMan::eBTS_MODE_MAX))
    {
        battle_score_manager->set_toggle_ui_open(false);
        battle_score_manager->ToggleBattleScoreWindow();
    }
}

void UIActionFunction::ToggleFullRadarMode()
{
    RadarFullDialog* radar_full_dialog = 
        GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
    if (radar_full_dialog != NULL)
    {
        radar_full_dialog->IncreaseRadarState();
        if (eFULL_RADAR_STATE_MAX != radar_full_dialog->GetRadarState())
        {
            radar_full_dialog->ShowDialog(TRUE);
        }
        else
        {
            radar_full_dialog->ShowDialog(FALSE);
        }
    }
}

void UIActionFunction::ToggleCashShopUI()
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#if defined(_SUNGAME_VER)
    if (PROGRAMPARAM->is_block_cash_shop() == false)
#endif //defined(_SUNGAME_VER)
    {
        ConvenienceStore::CashShop::instance()->ToggleOpenKey();
    }
#else
    uiCashItemShopMan *ui_manager = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
    if (ui_manager!= NULL)
    {
        ui_manager->ToggleCashItemShopDlg();
    }
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
}

void UIActionFunction::ToggleGuideInfomationUI()
{
    uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
    if (guide_manager != NULL)
    {
        guide_manager->OpenGuideInfoDialog();
    }
}

void UIActionFunction::ToggleMissionObjectUI()
{
    if (GameFramework::GetCurScene() != SCENE_TYPE_MISSION)
    {
        return;
    }

    uiMissionObject* mission_object_dialog = 
        GET_CAST_DIALOG(uiMissionObject, IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT);
    if (mission_object_dialog != NULL)
    {
        if (mission_object_dialog->IsVisible())
        {
            mission_object_dialog->ShowInterface(FALSE);
        }
        else
        {
            mission_object_dialog->ShowDialog(TRUE);
        }
    }
}

void UIActionFunction::ToggleQuickSlotExtend()
{
    g_HeroInput.ToggleChangeSlotSize();
}

void UIActionFunction::ExchangeQuickSlot()
{
    g_HeroInput.PressQuickSlotKey();
}

void UIActionFunction::CommandSummon(int command_index)
{
    SummonTargetDialog* summon_dialog = 
        GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);

    if (summon_dialog != NULL)
    {
        command_index += SummonTargetDialog::SUMMON_TARGET_CHECK_ATTACK;
        summon_dialog->CommandSummonMoster((SummonTargetDialog::eSUMMON_TARGET_POS)command_index);
    }
}

void UIActionFunction::UseSummonSkill(int skill_index)
{
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
    SummonSkillContainer* summon_skill_container = 
        static_cast<SummonSkillContainer*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SUMMON_SKILL_CONTAINER));

    if (summon_skill_container != NULL)
    {
        skill_index += SummonSkillContainer::SUMMONSKILL_QUICK_BT00;
        summon_skill_container->UseSummonSkill((SummonSkillContainer::eSUMMONSKILL_QUICK_POS)skill_index);
    }
#endif
}