//------------------------------------------------------------------------------
//  uiBattleZoneCreate_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneCreate.h"


//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreate::NetworkProc( MSG_BASE * pMsg )
{
	// 현재 void VillageScene::OnRecvCG_ZONE( MSG_BASE * pMsg ) 이곳에서
	// 처리하고있습니다. 
	// 안에서 처리할려면 포워딩 해주세요.

	/*
    switch (pMsg->m_byProtocol)
    {
    case CG_ZONE_LOBBY_CREATE_ACK:
        {
            MSG_CG_ZONE_LOBBY_CREATE_ACK* msg =
                (MSG_CG_ZONE_LOBBY_CREATE_ACK *)pMsg;

            this->m_RoomInfo.SetSerial(msg->m_LobbyKey);

            // BattleLobby로 포워딩
        }
        break;

    case CG_ZONE_LOBBY_CREATE_NAK:
        {
        }
        break;

    default:
        break;
    }
	*/
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
