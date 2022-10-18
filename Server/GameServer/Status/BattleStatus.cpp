#include "StdAfx.h"
#include ".\battlestatus.h"
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#include "GameInstanceDungeon.h"
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

BattleStatus::BattleStatus(void)
{
}

BattleStatus::~BattleStatus(void)
{
}
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
//------------------------------------------------------------------------------------------------
void BattleStatus::End()
{
    Character* owner = GetOwner();
    if(owner->IsEqualObjectKind(PLAYER_OBJECT))
    {
        Player* player = static_cast<Player*>(owner);
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        ChaosZoneType chaoszone_type = player->GetChaosZoneType();
        if ( chaoszone_type == kBattleGround || chaoszone_type == eCHAOSZONE_TYPE_GOLDRUSH ) 
        {
            PlayerAssistMeter* player_assist_meter = player->GetPlayerAssistMeter();
            if (player_assist_meter != NULL) {
                player_assist_meter->Clear();
            }
            else // if (player_assist_meter == NULL)
            {
                // Critical Error!
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : Guid[%d] Player's AssistMeter is NULL!", 
                                        player->GetCharGuid());
            }
        }
#else
         if ( player->IsThereBattleGroundZone() ) {
             player->GetPlayerAssistMeter()->Clear();
         }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    }
}

#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST