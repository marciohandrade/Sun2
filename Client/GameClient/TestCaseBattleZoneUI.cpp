#include "SunClientPrecompiledHeader.h"
#include "TestCaseBattleZoneUI.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "interfacemanager.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"
//-------------------------------------------------------------------------------------------
/**
*/
TestCaseBattleZoneUI::TestCaseBattleZoneUI(void)
{
	this->SetName(_T("BATTLEZONE"));
}

//-------------------------------------------------------------------------------------------
/**
*/
TestCaseBattleZoneUI::~TestCaseBattleZoneUI(void)
{
}


//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseBattleZoneUI::ProcessKeyInput(DWORD dwTick)
{
    if (g_Input.GetState (DIK_B, KS_DOWN))
    {
		uiBase * battleZoneCreateIFC = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE );

        if (battleZoneCreateIFC)
        {
            battleZoneCreateIFC->ShowInterface(!battleZoneCreateIFC->IsVisible());
        }
    }
}

BOOL TestCaseBattleZoneUI::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3/* = 0*/, int iParam4 /*= 0*/)
{
	return TRUE;
}