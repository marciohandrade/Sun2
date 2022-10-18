#include "StdAfx.h"
#include ".\handler_cg_warehouse.h"
#include ".\Player.h"
#include ".\PlayerManager.h"
#include <PacketStruct_DG.h>
#include "WarehouseSlotContainer.h"
#include "ExtraNPCManager.h"
#include "FunctionalRestrictManager.h"

#include "GameServerEx.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_WAREHOUSE::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_WAREHOUSE::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_START_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_MONEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_END_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_PWD_SETTING_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_PWD_LOCKING_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAREHOUSE, CG_WAREHOUSE_PWD_CHECK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_WAREHOUSE_IMPL(CG_WAREHOUSE_START_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_WAREHOUSE_START_SYN * pRecvMsg = (MSG_CG_WAREHOUSE_START_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	//�Ÿ����� �������� ����
	if( FALSE == g_ExtraNPCManager.IsLengthCanUseNPC(pRecvMsg->m_NPCCode, pPlayer, eNPC_TYPE_BANK) )
	{
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_UNAVAILABLE_LENGTH;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	if( pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
	{
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	// â�� ���� ����
	if( pPlayer->GetLevel() < LIMIT_LEVEL_WARHOUSE )
	{
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_UNABLE_FUNCTION_FOR_CURR_LEVEL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
	if( !pContainer->IsEnableService() )
	{
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_NOT_AVAILABLE_SERVICE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	if( pContainer->IsStarted() )
	{
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	MSG_DG_WAREHOUSE_START_SYN msg;
	msg.m_dwKey			= pRecvMsg->m_dwKey;
	if( FALSE == pPlayer->SendToGameDBPServer( &msg, sizeof(msg) ) )
	{
		SUNLOG( eFULL_LOG,  "[Handler_CG_WAREHOUSE_IMPL(CG_WAREHOUSE_START_SYN)] [ERROR]DBP����Ŷ(DG_WAREHOUSE_START_SYN)�������Ҽ������ϴ�." );
		MSG_CG_WAREHOUSE_START_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_NOT_AVAILABLE_SERVICE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );

		return;
	}
}

Handler_CG_WAREHOUSE_IMPL(CG_WAREHOUSE_MONEY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_WAREHOUSE_MONEY_SYN * pRecvMsg = (MSG_CG_WAREHOUSE_MONEY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	// â�� ���� ���� üũ
	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
	if( !pContainer->ValidState() )
	{
		MSG_CG_WAREHOUSE_MONEY_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_NOT_AVAILABLE_SERVICE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	if( !pContainer->IsStarted() )
	{
		MSG_CG_WAREHOUSE_MONEY_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	// GS�� �켱 ���� �Ա� Ȥ�� ���
	BOOL rt = FALSE;
	if( pRecvMsg->m_byType == MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY )
	{
		// â�� ���� ����
		if( pRecvMsg->m_Money <= pPlayer->GetMoney() )
		{
			if( pContainer->DepositMoney( pRecvMsg->m_Money ) )
			{
//				pPlayer->SetMoney( pPlayer->GetMoney() - pRecvMsg->m_Money );
				pPlayer->MinusMoney( pRecvMsg->m_Money, Player::eMONEY_CIRCULATING );	// SetMoney�� �� ���� �κ� ���� MinusMoney()�� ��ü��(��â)
				rt = TRUE;

				GAMELOG->LogMoney( MONEY_WAREHOUSE_PUT, pPlayer, pRecvMsg->m_Money );
			}
		}
	}
	else if( pRecvMsg->m_byType == MSG_CG_WAREHOUSE_MONEY_SYN::GETMONEY )
	{
		if( pPlayer->CanPlusMoney( pRecvMsg->m_Money ) )
		{
			// â���� ���� ã��
			if( pContainer->WithDrawMoney( pRecvMsg->m_Money ) )
			{
				// ����� ���� �ִ�.
				pPlayer->PlusMoney( pRecvMsg->m_Money, Player::eMONEY_CIRCULATING );	// SetMoney�� �� �Դ� �κ� ���� PlusMoney()�� ��ü��(��â)
				rt = TRUE;

				GAMELOG->LogMoney( MONEY_WAREHOUSE_GET, pPlayer, pRecvMsg->m_Money );
			}
		}
	}

	if( TRUE == rt )
	{
		MSG_CG_WAREHOUSE_MONEY_ACK msg;
		msg.m_InventoryMoney = pPlayer->GetMoney();
		msg.m_WarehouseMoney = pContainer->GetCurMoney();
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
	else
	{
		MSG_CG_WAREHOUSE_MONEY_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_INSUFFICIENT_MONEY;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
}

Handler_CG_WAREHOUSE_IMPL(CG_WAREHOUSE_END_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_WAREHOUSE_END_SYN * pRecvMsg = (MSG_CG_WAREHOUSE_END_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	// â�� ���� ����
	CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
	if( !pContainer->IsStarted() )
	{
		MSG_CG_WAREHOUSE_END_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	//if( !pContainer->IsChanged() )
	//{
	//	//SUNLOG( eFULL_LOG, "��ȭ����â������[%s]", pPlayer->GetCharName() );
	//	//â�� ���� ���� �α� �����
	//	GAMELOG->WriteWareHouseInfo( pPlayer, FALSE );

	//	MSG_CG_WAREHOUSE_END_ACK amsg;
	//	pPlayer->SendPacket( &amsg, sizeof(amsg) );
	//	// ���� ���·� �ٲ�
	//	pContainer->EndWarehouse();
	//	return;
	//}

	// ���� DB�� Item Stream + �� ����
	// DBP������ �ٷ� �����Ѵ�?????
	if( !pPlayer->SerializeInfoToDBProxy() )
	{
		MSG_CG_WAREHOUSE_END_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WAREHOUSE_NOT_AVAILABLE_SERVICE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	MSG_DG_WAREHOUSE_END_SYN msg;
	msg.m_dwKey = pPlayer->GetUserKey();
	BOOL rt = pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
	ASSERT( rt );

	//â�� ���� ���� �α� �����
	GAMELOG->WriteWareHouseInfo( pPlayer, FALSE );
	//GAMELOG->LogInvenInfo( SNAPSHOT_WAREHOUSE_CLOSE_INVEN, pPlayer );
	GAMELOG->LogSnapShot( pPlayer );

	// ���� ���·� �ٲ�
	pContainer->EndWarehouse();
}


Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_SETTING_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_SETTING_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_WAREHOUSE_PWD_SETTING_SYN( (MSG_CG_WAREHOUSE_PWD_SETTING_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_WAREHOUSE_PWD_SETTING_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_CHECK_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_CHECK_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_WAREHOUSE_PWD_CHECK_SYN( (MSG_CG_WAREHOUSE_PWD_CHECK_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_WAREHOUSE_PWD_CHECK_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_LOCKING_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_WAREHOUSE_IMPL( CG_WAREHOUSE_PWD_LOCKING_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_WAREHOUSE_PWD_LOCKING_SYN( (MSG_CG_WAREHOUSE_PWD_LOCKING_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_WAREHOUSE_PWD_LOCKING_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}


