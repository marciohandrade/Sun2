//=======================================================================================================================
/// TradeManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@note 
			- PutMoney : 거래창에 플레이어가 설정한 하임의 액수를 덮어쓴다 ( +하지 않는다 )
			- GetMoney : 거래창에 플레이어가 설정한 하임의 액수를 뺀다
	@condition check
			- PROPOSAL 상태에서 READY 상태로 돌아올 경우 : 상대방이 ACCEPT 상태이면 PROPOSAL상태로 변경
			- ACCEPT상태로 변경할 때 : 상대방이 PROPOSAL이여야만 하고, 상대방이 ACCEPT이면 Exchange()한 다음 거래 종료
	@history
			- 2006. 3. 4 : trade관련 리펙토링, 거래요청 수락 방식의 변경(자동수락)과 제한 조건 변경
			
*/
//=======================================================================================================================

#pragma once

#include <MemoryPoolFactory.h>
#include <SolarHashTable.h>
#include <hash_map>
#include "TradeSlotContainer.h"

//test용 풀

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

			RC::eTRADE_RESULT		PutItem( Player * pPlayer,								//< 빈 위치를 찾아서 아이템을 넣는 함수
										POSTYPE org_pos, 
										POSTYPE & OUT trade_pos_out,
										RENDER_ITEMSTREAMEX & OUT trade_stream_out );
			RC::eTRADE_RESULT		GetItem( Player * pPlayer, POSTYPE trade_pos );
			RC::eTRADE_RESULT		PutMoney( Player * pPlayer, MONEY & IN money );
			RC::eTRADE_RESULT		GetMoney( Player * pPlayer, MONEY & IN money );

			// 거래 제시
			RC::eTRADE_RESULT		Proposal( Player * pPlayer );

			// 수정
			RC::eTRADE_RESULT		Modify( Player * pPlayer );

			// 거래 완료
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


