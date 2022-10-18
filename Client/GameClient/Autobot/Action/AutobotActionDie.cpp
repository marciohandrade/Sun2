#include "SunClientPrecompiledHeader.h"
#include "AutobotActionDie.h"
#include "Autobot/Autobot.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionPickup.h"
#include "Autobot/Action/Global/AutobotActionPotion.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"

#include "ItemInfoParser.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"
#include "MouseHandler.h"
#include "GameFramework.h"
#include "ItemManager.h"
#include "SceneBase.h"
#include "GlobalFunc.h"
#include "SCItemSlot.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
AutobotActionDie* AutobotActionDie::Instance()
{
  static AutobotActionDie instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionDie::AutobotActionDie()
{
}
//------------------------------------------------------------------------------
AutobotActionDie::~AutobotActionDie()
{
}
//------------------------------------------------------------------------------
void AutobotActionDie::Enter(AutobotArea* owner)
{
    owner->state_machine()->PopGlobalState(AutobotActionPickup::Instance());
    owner->state_machine()->PopGlobalState(AutobotActionPotion::Instance());
    owner->state_machine()->PopGlobalState(AutobotActionInterval::Instance());
    owner->state_machine()->PopGlobalState(AutobotActionTargeting::Instance());

    AutobotPasspointManager::Instance()->Clear();

    if (AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Resurrection)
    {
        SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, eITEMTYPE_RECALL_ALLIANCE);
        if ((item_slot != NULL) && item_slot->GetNum() > 0)
        {
            if (Autobot::Instance()->UseItem(item_slot) == false)
            {
                //! 아이템 사용 실패
                //! 그냥 대기 한다.
            }
        }
    }
    else if (AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Village)
    {
        //! 마을로 이동 오토 종료
        Autobot::Instance()->Stop(false);
        //GlobalFunc::DeadConfirmInField(true);
        GlobalFunc::DeadConfirmInField(kConfirmSelect1);
    }
}
//------------------------------------------------------------------------------
void AutobotActionDie::Exit(AutobotArea* owner)
{
    owner->state_machine()->PushGlobalState(AutobotActionPickup::Instance());
    owner->state_machine()->PushGlobalState(AutobotActionPotion::Instance());
    owner->state_machine()->PushGlobalState(AutobotActionInterval::Instance());
    owner->state_machine()->PushGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
bool AutobotActionDie::Execute(AutobotArea* owner)
{
    if (g_pHero->IsDead() != TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionDie::NetworkProc(AutobotArea* owner, MSG_BASE* packet)
{
    if (packet->m_byCategory == CG_STATUS)
    {
        switch (packet->m_byProtocol)
        {
        case CG_STATUS_RESURRECTION_NAK:
            {
                owner->state_machine()->ChangeState("ready");
            }
            break;
        }
    }

    return true;
}