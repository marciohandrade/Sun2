#include "stdafx.h"
/*#ifdef _DEBUG


#include ".\gameshopmanager.h"
#include <PacketStruct_GS.h>
#include <PacketStruct_CG.h>
#include "PlayerManager.h"
#include "GameShopUser.h"
#include "ShopUser.h"
#include "Player.h"


TEST( MyGameShopManager, GameShopManager )
{  
//	CHECK_DOUBLES_EQUAL (someValue, 2.0f);

	DISPMSG( "-------------------------------------start\n" );

	//g_GameShopManager.OnStart();

	DISPMSG( "-------------------------------------ok\n" );
}

#define __NA_0_20070124_SHOP_UNITTEST
#ifdef __NA_0_20070124_SHOP_UNITTEST

TEST( MyGameShopManager, GameShopManager2 )
{  
	//	CHECK_DOUBLES_EQUAL (someValue, 2.0f);
	DISPMSG( "-------------------------------------start\n" );

	DWORD UserKey = 2077;
	int val = 0;
	int m_PriceGuid = 280;
	DWORD m_GiftGuid = 0;
	DWORD m_ActionType = 0;
	TCHAR m_tszGiftMessage[MAX_GIFT_MESSAGE_LENGTH]="¼±¹°ÀÌ´Ù¹Þ¾Æ¶ù!";
	TCHAR m_tszCharName[MAX_CHARNAME_LENGTH] = "ÄíÇØÇØ";

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) return ;


	switch( val )
	{
	case 0:
		{
        #if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)
			RC::eSHOP_RESULT rt = g_GameShopManager.CanSendGiftNDoRequest( pPlayer, 
				0, m_PriceGuid, m_tszCharName, m_tszGiftMessage );
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_SEND_GIFT_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}
        #endif
		}break;
	case 1:
		{
			RC::eSHOP_RESULT rt = g_GameShopManager.CanActionRecvGiftNDoRequest( pPlayer, m_ActionType, m_GiftGuid, m_tszGiftMessage );
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_ACTION_RECV_GIFT_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}
		}break;
	case 2:
		{
			RC::eSHOP_RESULT rt = g_GameShopManager.RequestRecvGiftCount( pPlayer );
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_IS_RECV_GIFT_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}

		}break;
	case 3:
		{
			RC::eSHOP_RESULT rt = g_GameShopManager.RequestRecvGiftList( pPlayer );
			if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
			{
				MSG_CG_SHOP_RECV_GIFT_LIST_NAK nmsg;
				nmsg.m_byErrorCode = rt;
				pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			}

		}break;
	}

	DISPMSG( "-------------------------------------ok\n" );
}

#endif



#endif*/