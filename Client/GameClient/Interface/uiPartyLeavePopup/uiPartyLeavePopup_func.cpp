//------------------------------------------------------------------------------
//  uiPartyLeavePopup_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyLeavePopup.h"
#include "uiPartyMan/uiPartyMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "BattleScene.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::response_Yes()
{
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (GlobalFunc::IsBattleGroundScene()
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || GlobalFunc::IsGoldRushScene()
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        )
    {
        BattleScene::BackToTheVillageSyn();
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


	this->LeaveParty();
    this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::response_No()
{
    this->ShowInterface(FALSE);
}


void
uiPartyLeavePopup::LeaveParty( void )
{
	uiPartyMan* partyMan = static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
    if (partyMan)
        partyMan->LeaveParty();
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
