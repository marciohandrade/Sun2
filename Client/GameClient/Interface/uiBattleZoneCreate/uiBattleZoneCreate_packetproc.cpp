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
	// ���� void VillageScene::OnRecvCG_ZONE( MSG_BASE * pMsg ) �̰�����
	// ó���ϰ��ֽ��ϴ�. 
	// �ȿ��� ó���ҷ��� ������ ���ּ���.

	/*
    switch (pMsg->m_byProtocol)
    {
    case CG_ZONE_LOBBY_CREATE_ACK:
        {
            MSG_CG_ZONE_LOBBY_CREATE_ACK* msg =
                (MSG_CG_ZONE_LOBBY_CREATE_ACK *)pMsg;

            this->m_RoomInfo.SetSerial(msg->m_LobbyKey);

            // BattleLobby�� ������
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
