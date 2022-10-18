//------------------------------------------------------------------------------
//  uiBattleZoneMan_nethandler.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "BattleLobby.h"
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::HandlerPKBooleans()
{
    int timeOutPacketIdx = TimeOutPKBooleans();

    if (!timeOutPacketIdx)
        return;

    switch (timeOutPacketIdx)
    {
    case ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN:
        assert (!"TIMEOUT!! >> ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN");
        break;

    case ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN:
        assert (!"TIMEOUT!! >> ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN");
        break;

    
	case PACKET_STATUS_SEND_CREATE:
		{
			BattleLobby * pBattleLobby = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
			if(pBattleLobby && pBattleLobby->IsVisibleWindow())
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(eST_SERVER_STATUS_NOT_SMOOTH, szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage ); //서버 상태가 원활하지 않습니다. 잠시 후 다시 시도해 주십시오
			}
		}
		break;


    default:
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("packet has been timeout!!"));
        break;
    }


	WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("TIMEOUT!! >> PacketIndex[%d]"), timeOutPacketIdx);
}

