#include "SunClientPrecompiledHeader.h"
#include "AutobotAreaVillage.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Action/AutobotActionMove.h"
#include "Autobot/Action/AutobotActionShop.h"
#include "Autobot/Action/AutobotActionReadyVillage.h"
#include "Autobot/Action/AutobotActionReturnField.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Autobot/Action/Global/AutobotActionShouting.h"

#include "GlobalData.h"
#include "Hero.h"
#include "NPC.h"
#include "HeroActionInput.h"
#include "Mouse.h"
#include "RadarFullDialog.h"
#include "Interface/uiRadarMan/uiRadarMan.h"
#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "PacketRequestManager.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "ShopInfoParser.h"
#include "ObjectManager.h"
#include "InventoryDialog.h"

//------------------------------------------------------------------------------
AutobotAreaVillage* AutobotAreaVillage::Instance()
{
  static AutobotAreaVillage instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotAreaVillage::AutobotAreaVillage() : 
    //shop_npc_(NULL), 
    //shop_finish_(false),
    return_field_(false)
{
    state_machine()->PushGlobalState(AutobotActionShouting::Instance());

    state_machine()->RegisterState("ready", AutobotActionReadyVillage::Instance());
    state_machine()->RegisterState("move", AutobotActionMove::Instance());
    state_machine()->RegisterState("shop", AutobotActionShop::Instance());
    state_machine()->RegisterState("returnfield", AutobotActionReturnField::Instance());
}
//------------------------------------------------------------------------------
AutobotAreaVillage::~AutobotAreaVillage()
{
}
//------------------------------------------------------------------------------
void AutobotAreaVillage::Enter(Autobot* owner)
{
    AutobotPasspointManager::Instance()->Clear();

    //set_shop_finish(false);

    //! 따라 가기 옵션 꺼고
    g_HeroInput.SetFollowState(FALSE);

    state_machine()->ChangeState("ready");
}
//------------------------------------------------------------------------------
void AutobotAreaVillage::Exit(Autobot* owner)
{
    //shop_npc_ = NULL;
    //waypoint_ = NULL;
    //shop_finish_ = false;
    return_field_ = false;
}
//------------------------------------------------------------------------------
bool AutobotAreaVillage::Execute(Autobot* owner)
{
    return state_machine()->Update();
    ////if (shop_npc() == NULL)
    //{
    //    shop_npc_ = GameFunc::GetNearestShopNpc(eITEMTYPE_WASTE);
    //}
    ////if (waypoint() == NULL)
    ////{
    ////    waypoint_ = GameFunc::GetWaypoint();
    ////}

    //if ((shop_npc() != NULL)/* && (waypoint() != NULL)*/)
    //{
    //    return state_machine()->Update();
    //}

    //return true;
}
//------------------------------------------------------------------------------ 
bool AutobotAreaVillage::NetworkProc(Autobot* owner, MSG_BASE* packet)
{
    return state_machine()->NetworkProc(packet);
}
