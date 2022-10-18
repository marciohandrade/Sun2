//=======================================================================================================================
/// TradeManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@note 
			- PutMoney : �ŷ�â�� �÷��̾ ������ ������ �׼��� ����� ( +���� �ʴ´� )
			- GetMoney : �ŷ�â�� �÷��̾ ������ ������ �׼��� ����
	@condition check
			- PROPOSAL ���¿��� READY ���·� ���ƿ� ��� : ������ ACCEPT �����̸� PROPOSAL���·� ����
			- ACCEPT���·� ������ �� : ������ PROPOSAL�̿��߸� �ϰ�, ������ ACCEPT�̸� Exchange()�� ���� �ŷ� ����
	@history
			- 2006. 3. 4 : trade���� �����丵, �ŷ���û ���� ����� ����(�ڵ�����)�� ���� ���� ����
			
*/
//=======================================================================================================================

#pragma once

#include <MemoryPoolFactory.h>
#include <SolarHashTable.h>
#include <hash_map>
#include "TradeSlotContainer.h"

//test�� Ǯ

class SCSlotContainer;
class TradeSlot;
class TradeManager
{
public:		TradeManager(void);
			~TradeManager(void);

			VOID Init( DWORD maxPoolSize );
			VOID Release();

			//BOOL CreateTradeContainer( Player * pPlayer1, Player * pPlayer2 );
			//BOOL DestroyTradeContainer( Player * pPlayer );

			RC::eTRADE_RESULT		StartTrade( Player * pRequester, Player * pAcceptor );
			RC::eTRADE_RESULT		EndTrade( Player * pPlayer );

			RC::eTRADE_RESULT		PutItem( Player * pPlayer,								//< �� ��ġ�� ã�Ƽ� �������� �ִ� �Լ�
										POSTYPE org_pos, 
										POSTYPE & OUT trade_pos_out,
										RENDER_ITEMSTREAMEX & OUT trade_stream_out );
			RC::eTRADE_RESULT		GetItem( Player * pPlayer, POSTYPE trade_pos );
			RC::eTRADE_RESULT		PutMoney( Player * pPlayer, MONEY & IN money );
			RC::eTRADE_RESULT		GetMoney( Player * pPlayer, MONEY & IN money );

			// �ŷ� ����
			RC::eTRADE_RESULT		Proposal( Player * pPlayer );

			// ����
			RC::eTRADE_RESULT		Modify( Player * pPlayer );

			// �ŷ� �Ϸ�
			RC::eTRADE_RESULT		Exchange(
										Player* pPlayer1,
										TRADE_TOTAL_INFO& OUT rTradeInfo1,
										MONEY& OUT money1,
										TRADE_TOTAL_INFO& OUT rTradeInfo2,
										MONEY& OUT money2
										);

			BOOL					ValidTradeState( Player * pPlayer );

private:	VOID					_GiveItem(
										SCSlotContainer* pContainer,
										TradeSlot** ppSlot, 
										POSTYPE& INOUT num,
										TRADE_TOTAL_INFO::SLOT_TYPE* OUT rItemSlot,
										BYTE & OUT Count
										);

			__inline TradeSlotContainer* _ALLOC()							
			{
				TradeSlotContainer* pContainer = (TradeSlotContainer *)m_pTradePool.Alloc(); 
				m_TestHash.insert( pContainer );
				return pContainer;
			}
			__inline BOOL			_FREE( TradeSlotContainer* pUnit );


//////////////////////////////////////////////////////////////////////////
// <FIELDs>
private:
			DWORD					m_TradeKey;

			util::CMemoryPoolFactory<TradeSlotContainer>	m_pTradePool;

			typedef STLX_SET<TradeSlotContainer*>	TEST_HASH;
			TEST_HASH	m_TestHash;
};

extern TradeManager g_TradeManager;


