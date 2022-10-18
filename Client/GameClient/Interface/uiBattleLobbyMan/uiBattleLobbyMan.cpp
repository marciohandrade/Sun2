#include "SunClientPrecompiledHeader.h"
#include "uibattlelobbyman.h"
#include "interfacemanager.h"

#include "BattleLobby.h"
#include "uiChangeRoomName/uiChangeRoomName.h"

uiBattleLobbyMan::uiBattleLobbyMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pBattleLobby = NULL;
    m_pChangeRoomName = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void	uiBattleLobbyMan::OnInitialize()
{

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    m_pBattleLobby = new BattleLobby;
    if ( !g_InterfaceManager.LoadInterface(
        m_pBattleLobby,
        "Data\\Interface\\63_BattleZone_ready_new.iwz",
        StrToWzID("0630"),
        FALSE) )
    {

    }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

    m_pChangeRoomName = new uiChangeRoomName;
    if ( !g_InterfaceManager.LoadInterface(
        m_pChangeRoomName,
        "Data\\Interface\\63_4_BattleZone_Input_Message.iwz",
        StrToWzID("0635"),
        FALSE) )
    {

    }   
}

