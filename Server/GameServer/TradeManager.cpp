#include "StdAfx.h"
#include ".\trademanager.h"
#include ".\TradeSlot.h"
#include ".\Player.h"
#include ".\ItemManager.h"
#include ".\SCItemSlotContainer.h"
#include <SCItemSlot.h>
#include ".\TradeSlotContainer.h"
#include "GameServerEx.h"
#include "FunctionalRestrictManager.h"
#include "ChaoState.h"

TradeManager g_TradeManager;

TradeManager::TradeManager(void)
{
}

TradeManager::~TradeManager(void)
{
	Release();
}

VOID		TradeManager::Init( DWORD maxPoolSize )
{
	m_TradeKey = 0;
	m_pTradePool.Initialize( maxPoolSize, 0, "TradeSlotContainer", AssertionLogger4Pool_Server );
}

VOID		TradeManager::Release()
{
	m_pTradePool.Release();
}

RC::eTRADE_RESULT
TradeManager::StartTrade( Player * pRequester, Player * pAcceptor )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pRequester->GetChaoState().IsChaoState() || pAcceptor->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( pRequester->GetBehave() != PLAYER_BEHAVE_IDLE_STATE ||
		pAcceptor->GetBehave() != PLAYER_BEHAVE_IDLE_STATE )
		return RC::RC_TRADE_INVALID_STATE;

	// (WAVERIX) �����ڵ� ������ ��.
	BOOL bEnabled = FALSE;

	//1. Requester üũ.
	bEnabled = pRequester->GetItemManager()->ValidContainer( SI_INVENTORY );
	if( !bEnabled )
	{
		ProtectorInventory& rPROTECT = pRequester->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			return RC::RC_TRADE_INVENTORY_NEED_PASSWORD;
		return RC::RC_TRADE_INVALID_STATE;
	}

	//2. Accepter üũ.
	bEnabled = pAcceptor->GetItemManager()->ValidContainer( SI_INVENTORY );
	if( !bEnabled )
	{
		ProtectorInventory& rPROTECT = pAcceptor->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			return RC::RC_TRADE_INVENTORY_NEED_PASSWORD;
		return RC::RC_TRADE_INVALID_STATE;
	}


	TradeSlotContainer * pReq_Container	= _ALLOC();
	if( !pReq_Container )
		return RC::RC_TRADE_INVALID_STATE;
	TradeSlotContainer * pAcpt_Container = _ALLOC();
	if( !pAcpt_Container )
	{
		SUNLOG( eCRITICAL_LOG, "�ŷ������̳ʸ��Ҵ��Ҽ�����." );
		if( !_FREE(pReq_Container) )
		{
			SUNLOG( eCRITICAL_LOG, "�ŷ�Ǯ��ȯ����0[%s][%s]", 
				pRequester->GetCharName(), pAcceptor->GetCharName() );
		}
		return RC::RC_TRADE_INVALID_STATE;
	}

	DWORD gen_key = ++m_TradeKey;

	pReq_Container->Init( pRequester, pAcceptor, gen_key );
	pAcpt_Container->Init( pAcceptor, pRequester, gen_key );

	pRequester->SetTradeContainer( pReq_Container );
	pAcceptor->SetTradeContainer( pAcpt_Container );
	pRequester->SetBehave(PLAYER_BEHAVE_TRADE_STATE);
	pAcceptor->SetBehave(PLAYER_BEHAVE_TRADE_STATE);

	SUNLOG( eFULL_LOG, "�ŷ�����[%s][%s]", 
		pRequester->GetCharName(), pAcceptor->GetCharName() );

	return RC::RC_TRADE_SUCCESS;
}

__inline
BOOL TradeManager::_FREE( TradeSlotContainer* pUnit )	
{
	ASSERT(pUnit);

	FlowControl flow;
	FLOWCONTROL_START()
    {
		IfFailThenBreak(pUnit, 10);

		TEST_HASH::iterator it = m_TestHash.find(pUnit);
		IfFailThenBreak(it != m_TestHash.end(), 20);

		//Player* pPlayer1 = pUnit->GetPlayer();
		//Player* pPlayer2 = pUnit->GetCounterPlayer();

		m_TestHash.erase(it);
		return m_pTradePool.Free(pUnit); 
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case 10: case 20:
		{
			Player* pPlayer1 = pUnit->GetPlayer();
			Player* pPlayer2 = pUnit->GetCounterPlayer();
			const TCHAR* pErrorStr =
				flow.GetErrorCode() == 10 ? _T("Invalid Release Ptr")
				: flow.GetErrorCode() == 20 ? _T("TRADE �̶̹Ȱ����ּ��Ǹ޸𸮰���ȯ�Ǿ���")
				: NULL;
			SUNLOG( eCRITICAL_LOG, _T("[%s:%d][%s:%d] %s  PTR(0x%p)"),
				pPlayer1->GetCharName(), pPlayer1->GetBehave(), pPlayer2->GetCharName(), pPlayer2->GetBehave(),
				pErrorStr, pUnit );
		}
		break;
	}
	return FALSE;
}

RC::eTRADE_RESULT
TradeManager::EndTrade( Player * pRequester )
{
	if( pRequester->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( pRequester->GetBehave() != PLAYER_BEHAVE_TRADE_STATE )
		return RC::RC_TRADE_INVALID_STATE;

	ASSERT( pRequester->GetTradeContainer() );
	Player * pPlayer2 = pRequester->GetTradeContainer()->GetCounterPlayer();
	ASSERT( pPlayer2->GetBehave() == PLAYER_BEHAVE_TRADE_STATE );
	if( pPlayer2->GetBehave() != PLAYER_BEHAVE_TRADE_STATE )
	{
		SUNLOG( eCRITICAL_LOG, "EndTrade()[p2:%s:%d]�����̻�", pPlayer2->GetCharName(), pPlayer2->GetBehave() );
        pRequester->GetTradeContainer()->ClearAll();
        if( !_FREE(pRequester->GetTradeContainer()) )
		{
			SUNLOG( eCRITICAL_LOG, "EndTrade()P1�ŷ�Ǯ��ȯ����[P1:%s][P2:%s:%d]", 
				pRequester->GetCharName(), pPlayer2->GetCharName(), pPlayer2->GetBehave() );
		}
        pRequester->SetTradeContainer( NULL );
        pRequester->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
		return RC::RC_TRADE_INVALID_STATE;
	}

    pRequester->GetTradeContainer()->ClearAll();
    pPlayer2->GetTradeContainer()->ClearAll();
	if( !_FREE(pRequester->GetTradeContainer()) )
	{
		SUNLOG( eCRITICAL_LOG, "�ŷ�Ǯ��ȯ������2[%s][%s]", 
			pRequester->GetCharName(), pPlayer2->GetCharName() );
	}
	if( !_FREE(pPlayer2->GetTradeContainer()) )
	{
		SUNLOG( eCRITICAL_LOG, "�ŷ�Ǯ��ȯ������3[%s][%s]", 
			pRequester->GetCharName(), pPlayer2->GetCharName() );
	}
    pRequester->SetTradeContainer( NULL );
    pPlayer2->SetTradeContainer( NULL );

	pRequester->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
	pPlayer2->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
	
	SUNLOG( eFULL_LOG, "�ŷ�����[%s][%s]", 
		pRequester->GetCharName(), pPlayer2->GetCharName() );

	return RC::RC_TRADE_SUCCESS;
}

BOOL		TradeManager::ValidTradeState( Player * pPlayer )
{
	if( pPlayer->IsExpiredPlayer() || pPlayer->GetBehave() != PLAYER_BEHAVE_TRADE_STATE )
		return FALSE;

	Player * pPlayer2 = pPlayer->GetTradeContainer()->GetCounterPlayer();
	if( pPlayer2->IsExpiredPlayer() || pPlayer2->GetBehave() != PLAYER_BEHAVE_TRADE_STATE )
		return FALSE;

	return TRUE;
}

RC::eTRADE_RESULT
TradeManager::PutItem(
						Player * pPlayer, 
						POSTYPE org_pos, 
						POSTYPE & OUT trade_pos_out,
						RENDER_ITEMSTREAMEX & OUT trade_stream_out )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_INVALID_STATE;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	if( pContainer->GetTradeState() != TRADE_READY )
		return RC::RC_TRADE_FAILED;

	SCSlotContainer * pInventoryContainer = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
	if( !pInventoryContainer->ValidPos( org_pos ) || 
		pInventoryContainer->IsEmpty( org_pos ) ||
		pInventoryContainer->IsLocked( org_pos ) )
		return RC::RC_TRADE_FAILED;

	SCItemSlot & rSlot = (SCItemSlot &)pInventoryContainer->GetSlot( org_pos );

    // (CHANGES) change to more generic code 'IsPetLocked' & 'IsRiderLocked' ...
    if(rSlot.IsUserBlocked())
        return RC::RC_TRADE_CANNOT_TRADE_ITEM;

	if( rSlot.GetLaxLock()->IsLock() )
	{
		//SUNLOG( eCRITICAL_LOG, "Can not Trade - LaxLock");
		return RC::RC_TRADE_CANNOT_TRADE_ITEM;
	}
    if (rSlot.IsPossessionItem() && rSlot.GetIdentifyPossession() && (!rSlot.IsDestroyItem())) {
        return RC::RC_TRADE_CANNOT_TRADE_ITEM;
    }
#ifdef _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE
    if(rSlot.IsProgressTime())
        return RC::RC_TRADE_CANNOT_TRADE_ITEM;
#endif

    if (rSlot.CanTradeSell(eITEM_TRADESELL_TRADE) == false) {
        return RC::RC_TRADE_CANNOT_TRADE_ITEM;
    }
    if (rSlot.CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false &&
        rSlot.IsProgressTime())
    {
        return RC::RC_TRADE_CANNOT_TRADE_ITEM;
    }

	// �����̳� ������� �ְ�, ���� ��ġ�� output
	if( !pContainer->PutItem( rSlot, trade_pos_out ) )
		return RC::RC_TRADE_FAILED;

	((SCItemSlot &)rSlot).CopyOut( trade_stream_out );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::GetItem( Player * pPlayer, POSTYPE trade_pos )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	if( pContainer->GetTradeState() != TRADE_READY )
		return RC::RC_TRADE_FAILED;

	if( !pContainer->ValidPos( trade_pos ) || pContainer->IsEmpty(trade_pos ) )
		return RC::RC_TRADE_FAILED;

	pContainer->DeleteSlot( trade_pos, NULL );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::PutMoney( Player * pPlayer, MONEY & IN money )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	if( pContainer->GetTradeState() != TRADE_READY )
		return RC::RC_TRADE_FAILED;

	if( pPlayer->GetMoney() < money )
		return RC::RC_TRADE_FAILED;

	pContainer->SetMoney( money );
	
	Player * pPartner = pContainer->GetCounterPlayer();
	GAMELOG->LogMoney( MONEY_TRADE_PUT_PRICE, pPlayer, money, pContainer->GetMoney(), pPartner->GetUserID(), pPartner->GetCharName() );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::GetMoney( Player * pPlayer, MONEY & IN money )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	if( pContainer->GetTradeState() != TRADE_READY )
		return RC::RC_TRADE_FAILED;

	if( pContainer->GetMoney() < money )
		return RC::RC_TRADE_FAILED;

	pContainer->SetMoney( (pContainer->GetMoney() - money) );

	Player * pPartner = pContainer->GetCounterPlayer();
	GAMELOG->LogMoney( MONEY_TRADE_GET_PRICE, pPlayer, money, pContainer->GetMoney(), pPartner->GetUserID(), pPartner->GetCharName() );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::Proposal( Player * pPlayer )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	if( pContainer->GetTradeState() != TRADE_READY )
		return RC::RC_TRADE_FAILED;

	pContainer->SetTradeState( TRADE_PROPOSAL );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::Modify( Player * pPlayer )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pPlayer->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pPlayer) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pContainer = pPlayer->GetTradeContainer();
	// �ڽ��� ���� �����̸� �������� ���Ѵ�.
	if( pContainer->GetTradeState() == TRADE_READY )
		return RC::RC_TRADE_FAILED;

	// ������ ACCEPT�����̸� PROPOSAL ���·� ����

	Player * pPlayer2 = PlayerManager::Instance()->FindPlayerByUserKey( pContainer->GetCounterPlayer()->GetUserKey() );
	if( !pPlayer2 )
	{
		SUNLOG( eCRITICAL_LOG, "[TradeManager::Modify] pPartner[%d] doesn't exist!", pContainer->GetCounterPlayer()->GetUserKey() );
		return RC::RC_TRADE_INVALID_STATE;
	}

	TradeSlotContainer * pContainer2 = pPlayer2->GetTradeContainer();
	switch( pContainer2->GetTradeState() )
	{
		case TRADE_ACCEPT:
		{
			pContainer2->SetTradeState(TRADE_PROPOSAL);
		}break;
	}

	// �ڽ��� ���� ���·� ����
	pContainer->SetTradeState( TRADE_READY );

	return RC::RC_TRADE_SUCCESS;
}

RC::eTRADE_RESULT
TradeManager::Exchange( Player * pMyself, 
										 TRADE_TOTAL_INFO & OUT rTradeInfo1, 
										 MONEY & OUT money1, 
										 TRADE_TOTAL_INFO & OUT rTradeInfo2, 
										 MONEY & OUT money2 )
{
	if( g_FunctionalRestrictMgr.IsRestrictFunction( eFUNCTIONALRESTRICT_USE_TRADE ) )
		return RC::RC_TRADE_NOT_AVAILABLE_SERVICE;

	if( pMyself->GetChaoState().IsChaoState() )
		return RC::RC_TRADE_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidTradeState(pMyself) )
		return RC::RC_TRADE_CANCEL;

	TradeSlotContainer * pMyContainer = pMyself->GetTradeContainer();

	Player * pPartner = PlayerManager::Instance()->FindPlayerByUserKey( pMyContainer->GetCounterPlayer()->GetUserKey() );
	if( !pPartner )
	{
		SUNLOG( eCRITICAL_LOG, "[TradeManager::Exchange] pPartner[%d] doesn't exist!", pMyContainer->GetCounterPlayer()->GetUserKey() );
		return RC::RC_TRADE_INVALID_STATE;
	}

	TradeSlotContainer * pPartnerContainer = pPartner->GetTradeContainer();

	switch ( pMyContainer->GetTradeState() )
	{
	case TRADE_PROPOSAL: break;
	default:
		return RC::RC_TRADE_INVALID_STATE;
	}
	switch ( pPartnerContainer->GetTradeState() )
	{
	case TRADE_READY: return RC::RC_TRADE_INVALID_STATE;
	case TRADE_PROPOSAL:
		pMyContainer->SetTradeState(TRADE_ACCEPT);					//< �ڽ��� ���¸� ACCEPT�� ����
		return RC::RC_TRADE_COUNTERPARTNER_NOTACCEPT;
	}
	//SUNLOG( eFULL_LOG, "Exchange����1[%s][%s]", pMyself->GetCharName(), pPartner->GetCharName() );

	// �ڽ��� ���¸� ACCEPT�� ���� ��, �Ѵ� Accept
	pMyContainer->SetTradeState(TRADE_ACCEPT);

	//----------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------
	SCSlotContainer * pInventory1 = pMyself->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
	SCSlotContainer * pInventory2 = pPartner->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
#if SUN_CODE_BACKUP
	SCSlotContainer * pTemporary1 = pMyself->GetItemManager()->GetItemSlotContainer(SI_TEMPINVENTORY);
	SCSlotContainer * pTemporary2 = pPartner->GetItemManager()->GetItemSlotContainer(SI_TEMPINVENTORY);
#endif
	// �ڽ��� �������� ���� �� �ִ� ���� ���� ����
	if( pInventory1->GetEmptySlotNum() < pPartnerContainer->GetSlotNum() )
	{
		return RC::RC_TRADE_PLAYER1_HAVENOTSPACE;
	}

	// ��Ʈ�ʰ� �������� ���� �� �ִ� ���� ���� ����
	if( pInventory2->GetEmptySlotNum() < pMyContainer->GetSlotNum() )
	{
		return RC::RC_TRADE_PLAYER1_HAVENOTSPACE;
	}
	
	// ���� ��ġ ����
	if(  pMyself->GetMoney() < pMyContainer->GetMoney() )
		return RC::RC_TRADE_PLAYER1_HAVENOTMONEY;
	if(  pPartner->GetMoney() < pPartnerContainer->GetMoney() )
		return RC::RC_TRADE_PLAYER2_HAVENOTMONEY;

	if( !pMyself->CanPlusMoney( pPartnerContainer->GetMoney() ) )
		return RC::RC_TRADE_FAILED;

	if( !pPartner->CanPlusMoney( pMyContainer->GetMoney() ) )
		return RC::RC_TRADE_FAILED;

	//SUNLOG( eFULL_LOG, "Exchange����2[%s][%s]", pMyself->GetCharName(), pPartner->GetCharName() );

	//���� ���鼭 ä���
	TradeSlot * pMyTradeSlot[MAX_TRADE_SLOT_NUM>>1] = {NULL, };
	POSTYPE MyTradeItemNum = 0;
	for( int i = pMyContainer->GetMaxSlotNum()-1 ; i >= 0  ; --i )
	{
		if( !pMyContainer->IsEmpty(i) )
		{
			TradeSlot & rTradeSlot = (TradeSlot &)pMyContainer->GetSlot(i);
			if( rTradeSlot.GetLinkItem()->GetNum() != rTradeSlot.GetNum() )
			{
				// ó�� �ø� ������ ����� Ʋ����
				return RC::RC_TRADE_DIFFERENT_ITEM_INFO;
			}
			pMyTradeSlot[MyTradeItemNum++] = &rTradeSlot;
		}
	}
	// 2��° �÷��̾��� �������� 1��° �÷��̾�� �ش�
	TradeSlot * pPartnerTradeSlot[MAX_TRADE_SLOT_NUM>>1] = {NULL, };
	POSTYPE PartnerTradeItemNum = 0;
	for( int i = pPartnerContainer->GetMaxSlotNum()-1 ; i >= 0  ; --i )
	{
		if( !pPartnerContainer->IsEmpty(i) )
		{
			TradeSlot & rTradeSlot = (TradeSlot &)pPartnerContainer->GetSlot(i);
			if( rTradeSlot.GetLinkItem()->GetNum() != rTradeSlot.GetNum() )
			{
				// ó�� �ø� ������ ����� Ʋ����
				return RC::RC_TRADE_DIFFERENT_ITEM_INFO;
			}
			pPartnerTradeSlot[PartnerTradeItemNum++] = &rTradeSlot;
		}
	}

	// 1��° �÷��̾��� �������� 2��° �÷��̾�� �ش�
	rTradeInfo2.m_InvenCount = 0;
	rTradeInfo2.m_TmpInvenCount = 0;
	POSTYPE RemainNum = MyTradeItemNum;
	_GiveItem( pInventory2, pMyTradeSlot, RemainNum, &rTradeInfo2.m_Slot[0], rTradeInfo2.m_InvenCount );
#if SUN_CODE_BACKUP
	if( RemainNum != 0 )
		_GiveItem( pTemporary2, pMyTradeSlot, RemainNum, &rTradeInfo2.m_Slot[rTradeInfo2.m_InvenCount], rTradeInfo2.m_TmpInvenCount );
#endif
	//-------------- �α� --------------------------------------------------------------------------------------------------
	for(int i = 0 ; i < MyTradeItemNum ; ++i )
	{
		SCItemSlot* pItemSlot = pMyTradeSlot[i]->GetLinkItem(); ASSERT( pItemSlot );
		// �Ҹ� ������ ���ο� ���� ���ϱ�
		if( !pItemSlot ) continue;
		GAMELOG->LogItem( ITEM_TRADE_GIVE, pMyself, pItemSlot, 0, pItemSlot->GetNum(), pPartner->GetUserID(), pPartner->GetCharName() );
		GAMELOG->LogItem( ITEM_TRADE_RECEIVE, pPartner, pItemSlot, 0, pItemSlot->GetNum(), pMyself->GetUserID(), pMyself->GetCharName() );
	}
	//-------------- �α� --------------------------------------------------------------------------------------------------

	// 1��° �÷��̾�� �ش�
	rTradeInfo1.m_InvenCount = 0;
	rTradeInfo1.m_TmpInvenCount = 0;
	RemainNum = PartnerTradeItemNum;
	_GiveItem( pInventory1, pPartnerTradeSlot, RemainNum, &rTradeInfo1.m_Slot[0], rTradeInfo1.m_InvenCount );
#if SUN_CODE_BACKUP
	if( RemainNum != 0 )
		_GiveItem( pTemporary1, pPartnerTradeSlot, RemainNum, &rTradeInfo1.m_Slot[rTradeInfo1.m_InvenCount], rTradeInfo1.m_TmpInvenCount );
#endif
	//-------------- �α� --------------------------------------------------------------------------------------------------
	for(int i = 0 ; i < PartnerTradeItemNum ; ++i )
	{
		SCItemSlot* pItemSlot = pPartnerTradeSlot[i]->GetLinkItem(); ASSERT(pItemSlot);
		if( !pItemSlot ) continue;
		GAMELOG->LogItem( ITEM_TRADE_GIVE, pPartner, pItemSlot, 0, pItemSlot->GetNum(), pMyself->GetUserID(), pMyself->GetCharName() );
		GAMELOG->LogItem( ITEM_TRADE_RECEIVE, pMyself, pItemSlot, 0, pItemSlot->GetNum(), pPartner->GetUserID(), pPartner->GetCharName() );
	}
	//-------------- �α� --------------------------------------------------------------------------------------------------

	
	// �κ��丮�� ���� ������ �����
	for( POSTYPE i = 0 ; i < (MAX_TRADE_SLOT_NUM>>1) ; ++i )
	{
		if( !pMyContainer->IsEmpty(i) )
			pInventory1->DeleteSlot( ((TradeSlot &)pMyContainer->GetSlot(i)).GetLinkItem()->GetPos(), NULL );
		if( !pPartnerContainer->IsEmpty(i) )
			pInventory2->DeleteSlot( ((TradeSlot &)pPartnerContainer->GetSlot(i)).GetLinkItem()->GetPos(), NULL );
	}
	
	// ���ŷ�
	// My���� Money����
	// ������ ���� ����Money�� ������ �ְ�
	pMyself->MinusMoney( pMyContainer->GetMoney(), Player::eMONEY_CIRCULATING );
	pMyself->PlusMoney( pPartnerContainer->GetMoney(), Player::eMONEY_CIRCULATING );
	
	pPartner->MinusMoney( pPartnerContainer->GetMoney(), Player::eMONEY_CIRCULATING );
	pPartner->PlusMoney( pMyContainer->GetMoney(), Player::eMONEY_CIRCULATING );
	
	//-------------- �α� --------------------------------------------------------------------------------------------------
	// ���ŷ� �α� �����(�ֱ�)
	if( pPartnerContainer->GetMoney() + pMyContainer->GetMoney() > 0 )
	{
		GAMELOG->LogMoney( MONEY_TRADE, pMyself, pMyContainer->GetMoney(), pPartnerContainer->GetMoney(), 
							pPartner->GetUserID(), pPartner->GetCharName() );
		GAMELOG->LogMoney( MONEY_TRADE, pPartner, pPartnerContainer->GetMoney(), pMyContainer->GetMoney(), 
							pMyself->GetUserID(), pMyself->GetCharName() );
	}
	//-------------- �α� --------------------------------------------------------------------------------------------------

	money1 = pMyself->GetMoney();
	money2 = pPartner->GetMoney();

	// �ŷ�â ��� �����
	pMyContainer->ClearAll();
	pPartnerContainer->ClearAll();

	return RC::RC_TRADE_SUCCESS;
}


// SCSlotContainer * pContainer : �ְ����ϴ� �����̳�
// TradeSlot **& prSlot			: �� ������ ����
// POSTYPE & INOUT num			: �� ������ (����) ����
// TRADE_TOTAL_INFO & OUT Info  : �� �����̳� ��ġ, ������ ����
VOID		TradeManager::_GiveItem( SCSlotContainer * pContainer, TradeSlot ** ppSlot, POSTYPE & INOUT num, TRADE_TOTAL_INFO::SLOT_TYPE * OUT rItemSlot, BYTE & OUT Count )
{
	for( POSTYPE pos = 0 ; pos < pContainer->GetMaxSlotNum() && (num != 0) ; ++pos )
	{
		if( pContainer->IsEmpty(pos) )
		{
			if( num == 0 )
			{
				SUNLOG(eCRITICAL_LOG, "[Error]Trade Num == 0" );
				return;
			}
			--num;
			rItemSlot[Count].m_Pos = pos;
			ppSlot[num]->GetLinkItem()->SetLock(FALSE);
			ppSlot[num]->GetLinkItem()->CopyOut( rItemSlot[Count].m_Stream );
			pContainer->InsertSlot(pos, *(ppSlot[num]->GetLinkItem()) );
			++Count;
		}
	}
}

