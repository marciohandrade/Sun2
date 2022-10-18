
#pragma once

#include "Player.h"
#include "GameZone.h"
#include <PacketStruct_DG.h>
#include "ObjectFactory.h"
#include "GameServer.h"

class PlayerDisconnectHandler
{
public:
	PlayerDisconnectHandler(){}
	~PlayerDisconnectHandler(){}

	VOID operator()( Player * pPlayer )
	{

		GameZone * pEnterZone	= NULL;
		CODETYPE EnterFieldCode = 0;
		eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
		if( eRoomState & ePRS_AT_ZONE )
		{
			pEnterZone->Leave( pPlayer );
		}
	
		// send PlayerTotalInfo to DBProxy
		pPlayer->SerializeInfoToDBProxy();

		MSG_DG_CHARINFO_UDPATE_CMD msg5;
		msg5.m_dwKey = pPlayer->GetUserKey();
		pPlayer->SendToGameDBPServer( &msg5, sizeof(msg5) );
		
		PlayerManager::Instance()->RemovePlayer( pPlayer );
		ObjectFactory::Instance()->FreeObject( pPlayer );
	}
};

class PlayerUnregisterHandler
{
public:
	PlayerUnregisterHandler(){}
	~PlayerUnregisterHandler(){}

	VOID operator()( Player * pPlayer )
	{
		GameZone * pEnterZone	= NULL;
		CODETYPE EnterFieldCode = 0;
		eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
		if( eRoomState & ePRS_AT_ZONE )
		{
			pEnterZone->Leave( pPlayer );
		}

		PlayerManager::Instance()->RemovePlayer( pPlayer );
		ObjectFactory::Instance()->FreeObject( pPlayer );
	}
};
