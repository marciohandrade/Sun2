#include "SunClientPrecompiledHeader.h"
#include "AutobotActionReturnField.h"
#include "Autobot/Autobot.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionPickup.h"
#include "Autobot/Action/Global/AutobotActionPotion.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"

#include "SCItemSlot.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"
#include "MouseHandler.h"
#include "GameFramework.h"
#include "ItemManager.h"
#include "SceneBase.h"
#include "ItemInfoParser.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
AutobotActionReturnField* AutobotActionReturnField::Instance()
{
  static AutobotActionReturnField instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionReturnField::AutobotActionReturnField()
{
}
//------------------------------------------------------------------------------
AutobotActionReturnField::~AutobotActionReturnField()
{
}
//------------------------------------------------------------------------------
void AutobotActionReturnField::Enter(AutobotArea* owner)
{
    MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD send_packet;

    if (GlobalFunc::SendPacketToGameServer(&send_packet, sizeof(send_packet)) == FALSE)
    {
        owner->state_machine()->ChangeState("ready");
        return;
    }
}
//------------------------------------------------------------------------------
bool AutobotActionReturnField::NetworkProc(AutobotArea* owner, MSG_BASE* packet)
{
    if (packet->m_byCategory == CG_MAP)
    {
        switch (packet->m_byProtocol)
        {
        case CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK:
            {
                MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK* nak = static_cast<MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK*>(packet);

                owner->state_machine()->ChangeState("ready");
                Autobot::Instance()->Stop(false);

                //TCHAR error_message[INTERFACE_STRING_LENGTH] = {0, };
                //g_InterfaceManager.GetStringACError(nak->m_byErrorCode, 
                //    error_message, 
                //    INTERFACE_STRING_LENGTH);
                //GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, error_message, INTERFACE_STRING_LENGTH);
            }
            break;
        }
    }
    return true;
}