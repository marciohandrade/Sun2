//------------------------------------------------------------------------------
//  uiQuestAutoAcceptAlram_func.cpp
//  (C) 2005 y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestAutoAcceptAlram.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "QuestManager_Concrete.h"
#include "ItemInfoParser.h"
#include "Quest.h"
#include <SolarHashTable.h>
#include "NPCInfoParser.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "QuestTextInfoParser.h"
#include "interfacemanager.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestMan*
uiQuestAutoAcceptAlram::GetManager()
{
    uiQuestMan* puiQuestMan  =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestAutoAcceptAlram::ClearInfo()
{
	if ( !this->CheckControls() )
		return;

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestAutoAcceptAlram::initInfo()
{
	m_BlinkTimer.SetTimer(1000);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestAutoAcceptAlram::updateInfo()
{
	if ( m_BlinkTimer.IsExpired() )
	{
		m_BlinkTimer.Reset();
	}

	float fRatio = 1.0f - m_BlinkTimer.GetProgressRatio();
	if(fRatio >= 0.8f)
	{
		fRatio = 1.0f;
	}

	if(fRatio <= 0.1f)
	{
		fRatio = 0.1f;
	}

	GetDialogWZ()->SetTransparency(fRatio);
}




//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
