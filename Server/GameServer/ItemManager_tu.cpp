#include "stdafx.h"
#include "ItemManager.h"
#include "SlotManager.h"
#include "Player.h"
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>
#include <struct.h>
#include <SCSlotStruct.h>
#include "GameZoneManager.h"

// 횟수 호출하는 함수 필요
// 함수의 시작/끝을 알 수 있는 방법 필요
// 여러가지 출력 함수 필요
/*
class MyItemManagerSetup : public TestSetup
{
public:
	void setup()
	{
		int index = 0 ;
		for( ; index < 1000 ; ++index )
		{
			//m_pSession = ServerSessionManager::Instance()->FindServer(index );
			//if(m_pSession) break; 
		}
		ASSERT( m_pSession );
		if( !m_pSession ) return;

		m_pPlayer = new Player;
		m_pPlayer->Create( m_pSession, 2047, "taiyo", "10.1.1.27", 0 );
		m_pPlayer->Init();

		//m_pPlayer->OnEnterField(&m_Field, &vect);
		m_pPlayer->SetGameZone( ePRS_BEFORE_ENTER_FIELD, NULL, 0, 0 );
		g_GameZoneManager.JoinPlayer( 20, m_pPlayer, 20205, 0, NULL );

		m_pMyItemManager = m_pPlayer->GetItemManager();
	}

	void teardown()
	{
		m_pPlayer->Release();
		m_pMyItemManager = NULL;
		delete m_pPlayer;
	}

protected:
	GameField		m_Field;
	ServerSession * m_pSession;
	Player *		m_pPlayer;
	ItemManager *	m_pMyItemManager;
};
*/


//TESTWITHSETUP( MyItemManager, EnchantTest )
//{  
////	CHECK_DOUBLES_EQUAL (someValue, 2.0f);
//
//	DISPMSG( "-------------------------------------start\n" );
//
//	m_pPlayer->SetLevel( LIMIT_LEVEL_ENCHANT +1 );
//	m_pPlayer->PlusMoney( 1000000 );
//	m_pPlayer->SetHP(100);
//
//	m_pMyItemManager->GetItemSlotContainer( SI_INVENTORY )->ClearAll();
// 
//	SCItemSlot ItemSlot;
//
//	for( int i = 0 ; i < 10000 ; ++i )
//	{
//		
//		SCSlotContainer * pInventory = (SCSlotContainer*)m_pMyItemManager->GetItemSlotContainer( SI_INVENTORY );
//		POSTYPE pos1 = 0, pos2 = 0;
//		if( !pInventory->GetEmptyPos(pos1) ) break;
//		ItemSlot.SetCode( 1 );
//		ItemSlot.SetSerial( i+1 );
//		pInventory->InsertSlot( pos1, ItemSlot );
//		((SCItemSlot&)pInventory->GetSlot(pos1)).SetBlocked(FALSE);
//		if( !pInventory->GetEmptyPos(pos2) ) break;
//		ItemSlot.SetCode( 4201 );
//		ItemSlot.SetSerial( i*10+2 );
//		pInventory->InsertSlot( pos2, ItemSlot );
//		((SCItemSlot&)pInventory->GetSlot(pos2)).SetBlocked(FALSE);
//
//		RC::eITEM_RESULT rt = m_pMyItemManager->Enchant( pos1, 3 );
//		CHECK_LONGS_EQUAL( rt, RC::RC_ITEM_ENCHANT_SUCCESS );
//	}
//
//	DISPMSG( "-------------------------------------ok\n" );
//}

//
//TESTWITHSETUP( MyItemManager, DeleteFromTotalInfo )
//{
//	DISPMSG( "-------------------------------------start\n" );
//
//	m_pPlayer->SetLevel( LIMIT_LEVEL_ENCHANT +1 );
//	m_pPlayer->PlusMoney( 1000000 );
//	m_pPlayer->SetHP(100);
//
//	m_pMyItemManager->GetItemSlotContainer( SI_INVENTORY )->ClearAll();
//	
//	SCItemSlot ItemSlot;
//
//	for( int i = 0 ; i < 10000 ; ++i )
//	{
//		
//		SCSlotContainer * pInventory = (SCSlotContainer*)m_pMyItemManager->GetItemSlotContainer( SI_INVENTORY );
//		POSTYPE pos1 = 0, pos2 = 0;
//		if( !pInventory->GetEmptyPos(pos1) ) break;
//		ItemSlot.SetCode( 1 );
//		ItemSlot.SetSerial( i+1 );
//		pInventory->InsertSlot( pos1, ItemSlot );
//		((SCItemSlot&)pInventory->GetSlot(pos1)).SetBlocked(FALSE);
//		if( !pInventory->GetEmptyPos(pos2) ) break;
//		ItemSlot.SetCode( 4201 );
//		ItemSlot.SetSerial( i*10+2 );
//		pInventory->InsertSlot( pos2, ItemSlot );
//		((SCItemSlot&)pInventory->GetSlot(pos2)).SetBlocked(FALSE);
//
//		RC::eITEM_RESULT rt = m_pMyItemManager->Enchant( pos1, 3 );
//		CHECK_LONGS_EQUAL( rt, RC::RC_ITEM_ENCHANT_SUCCESS );
//	}
//
//	DISPMSG( "-------------------------------------ok\n" );
//}