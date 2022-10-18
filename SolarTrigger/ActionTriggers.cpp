#include "StdAfx.h"

#include ".\actiontriggers.h"

#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
	#include "..\\..\\SolarTrigger\\actionInfo.h"
	#include "TriggerManagerEx.h"
#else
	#include ".\actionInfo.h"
	#include ".\TriggerManager.h"
#endif

#include ".\Trigger.h"
#include ".\Player.h"
#include ".\PacketStruct_CG.h"
#include ".\PacketStruct_AG.h"
#include ".\Map.h"
#include ".\ItemInfoParser.h"
#include ".\ItemManager.h"
#include <Global.h>
#include ".\MissionManager.h"
#include ".\MissionRewardManager.h"
#include ".\GameZone.h"

#include "SCSlotContainer.h"
#include <SCItemSlotContainer.h>

// 방향 지정 몬스터 리젠 위해
#include ".\handler_cg_sync.h"
#include <PublicMath.h>
#include <3DEngine/Common/MathAssistant.h>

// 범위 데미지 위해
#include <MapObject.h>

#include <DropManager.h>
#include "party\GameParty.h"
#include "party\GamePartyManager.h"

#include <QuestManager.h>
#include <QuestManager_Concrete.h>

#include <NPCGroupManager.h>

#define TYPE_CHECKER(outtype, chktype, variable)	\
	( ( ((outtype)((chktype)0xFFFFFFFFFFFFFFFFLL))&variable ) == variable )

__IMPL_POOL(ACTION_CHANGE_OBJECTANI)
VOID ACTION_CHANGE_OBJECTANI::OnAction()
{
#ifdef __NA000405_070406_LAST_OBJSTATE_APPLICATION__
	getMGR()->MAP_STATE.Change( getParent()->TriggerID() );
#endif //

	SUNLOG( eDEV_LOG, _T("[TriggerID:%u]:ACTION_CHANGE_OBJECTANI"), getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_CHANGE_PATHTILE)
VOID ACTION_CHANGE_PATHTILE::OnInit()
{
//#ifdef _DEV_TAIYO_PATHENGINE_CODE
//	// 움직일수없는 타일을 갈수있는 타일로 바꾸는 경우
//	// 먼저, 타일의 속성에 ECT_DYNAMIC를 부여한다.
//	if( CANMOVE == Get()->GetAttribute() )
//	{
//		getMGR()->SetAttribute( Get()->GetTileID(), PTA_NO_WALK);
//	}
//#endif
}
VOID ACTION_CHANGE_PATHTILE::OnAction()
{
	ACTION_CHANGE_PATHTILE_INFO* pINFO = Get();
	if( CANMOVE == pINFO->GetAttribute() )
	{
		getMGR()->UnsetAttribute( pINFO->GetTileID(), PTA_NO_WALK);
	}
	else if( CANNOTMOVE == pINFO->GetAttribute() )
	{
		getMGR()->SetAttribute( pINFO->GetTileID(), PTA_NO_WALK);
	}

#ifdef __NA000405_070406_LAST_OBJSTATE_APPLICATION__
	getMGR()->MAP_STATE.Change( getParent()->TriggerID() );
#endif //
	
	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CHANGE_PATHTILE", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_CHANGE_OBJECTSTATE)
VOID ACTION_CHANGE_OBJECTSTATE::OnAction()
{
	INT objID = Get()->GetObjectID();
	MapObject * pMapObject = getMGR()->FindMapObject( objID );
//	ASSERT(NULL != pMapObject);
	DEBUG_CODE( if( !pMapObject ) return ; );

	switch( Get()->GetState() )
	{
	case ACTION_CHANGE_OBJECTSTATE_INFO::APPEAR:
		{
//			SUNLOG( eFULL_LOG,  "구현안됨!" );
		}
		break;
	case ACTION_CHANGE_OBJECTSTATE_INFO::DISAPPEAR:
		{
//			SUNLOG( eFULL_LOG,  "구현안됨!" );
		}
		break;
	case ACTION_CHANGE_OBJECTSTATE_INFO::REMOVE:
		{
			//DWORD dwNpcKey = pMapObject->GetLinkedNpcObjectKey();
			//if( getMGR()->RemoveMapObject( Get()->GetObjectID() ) )
			//{
			//	//if( 0 != dwNpcKey )
			//	//{
			//	//	//getMGR()->GetField()->RemoveMapNpc( dwNpcKey );
			//	//}
			//	return;
			//}

			SUNLOG( eCRITICAL_LOG,  "[ACTION_CHANGE_OBJECTSTATE::OnAction] [ID:%u]:ACTION_CHANGE_OBJECTSTATE is Failed!!!!!!!!!!!", getParent()->TriggerID() );
		}
		break;
	}

#ifdef __NA000405_070406_LAST_OBJSTATE_APPLICATION__
	getMGR()->MAP_STATE.Change( getParent()->TriggerID() );
#endif //
	
	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CHANGE_OBJECTSTATE", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_PLAY_EVENT)
VOID ACTION_PLAY_EVENT::OnAction()
{
	// 이벤트를 플레이하라고 알려라!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
	if( NULL == pPlayer )
	{
		SUNLOG( eFULL_LOG, "[ACTION_PLAY_EVENT::OnAction] [U:%d]Player NULL!!", getParent()->GetClickedUserKey());
		return;
	}

	MSG_CG_TRIGGER_PLAY_EVENT_CMD	msg;

	msg.m_nEventIndex = Get()->GetEventID();
	pPlayer->SendPacket( &msg, sizeof(msg) );

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_PLAY_EVENT", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_SET_ISATTACK)
VOID ACTION_SET_ISATTACK::OnAction()
{
//	SUNLOG( eFULL_LOG,  "구현안됨!" );
//	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_SET_ISATTACK", getParent()->TriggerID() );
}


class WARP_PLAYER
{
public:
	static VOID Warp( Player * pPlayer, MAPCODE EnterFieldCode, WzID AreaID )
	{
		GameZone * pZone = pPlayer->GetGameZonePtr();
		if( !pZone )	return;

		pZone->Warp( pPlayer, EnterFieldCode, AreaID );

		MSG_CG_TRIGGER_PORTAL_ACK msg;
		msg.m_MapCode		= pZone->GetMapCode();
		msg.m_FieldCode		= EnterFieldCode;
#ifdef __NA070315_RANDOM_PROTOCOL_CHANGE__
		msg.Encode();
#endif //
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
};

class BackToTheVillage
{
	
public:
	BackToTheVillage(){}
	~BackToTheVillage(){}
	VOID operator() ( Player * pPlayer ) 
	{ 
		MSG_AG_ZONE_MISSION_LEAVE_CMD msg;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
	VOID operator () ( std::pair<DWORD,Player *> pair_type )
	{
		operator() ( pair_type.second );
	}
};

class WarpPlayerForField
{
	MAPCODE m_FieldCode;
	WzID m_AreaID;
public: 
	WarpPlayerForField(MAPCODE FieldCode, WzID AreaID ):m_FieldCode(FieldCode),m_AreaID(AreaID){}
	~WarpPlayerForField(){}
	VOID operator() ( Player * pPlayer ) 
	{ 
		WARP_PLAYER::Warp( pPlayer, m_FieldCode, m_AreaID ); 
	}
	VOID operator() ( std::pair<DWORD,Player *> pair_type ) 
	{ 
		operator() ( pair_type.second );
	}
};

__IMPL_POOL(ACTION_PORTAL_PLAYER)
VOID ACTION_PORTAL_PLAYER::OnAction()
{
	MAPCODE EnterFieldCode = Get()->GetMapCode();
	if( Get()->GetType() == ACTION_PORTAL_PLAYER_INFO::FIELDCODE )
	{
		if( Get()->GetIsParty() == PARTY )
		{
			// 현재 필드에 있는 사람을 모두 워프
			// 조건에 해당하는 사람을 알 필요 없음

			WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID());
			getMGR()->ForeachPlayer( op );
		}
		else
		{
			ASSERT( Get()->GetIsParty() == PERSONAL );

			if( getParent()->GetClickedUserKey() != 0 )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
				if( NULL == pPlayer )
				{
					SUNLOG( eFULL_LOG, "[ACTION_PORTAL_PLAYER::OnAction] [U:%d]ACTION_REWARD_PLAYER Player NULL!!", getParent()->GetClickedUserKey());
					return;
				}
				WARP_PLAYER::Warp( pPlayer,EnterFieldCode,Get()->GetAreaID() );
			}
			else
			{
				/*
				WarpPlayerForCondition op(EnterFieldCode,Get()->GetAreaID());
				getParent()->FindPlayer( op);
				*/
				WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID());
				getParent()->For_EachSatisfiedPlayer(op);
				getParent()->RemoveAllSatisfiedPlayer();
			}
		}
	}
	else //if( Get()->GetType() == ACTION_PORTAL_PLAYER_INFO::RELATEDVILLAGE )
	{
		if( Get()->GetIsParty() == PARTY )
		{
			// 현재 필드에 있는 사람을 모두 워프
			// 조건에 해당하는 사람을 알 필요 없음

			BackToTheVillage op;
			getMGR()->ForeachPlayer( op );
			//WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID());
			
		}
		else
		{
			ASSERT( Get()->GetIsParty() == PERSONAL );

			if( getParent()->GetClickedUserKey() != 0 )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
				if( NULL == pPlayer )
				{
					SUNLOG( eFULL_LOG, "[ACTION_PORTAL_PLAYER::OnAction] [U:%d]ACTION_REWARD_PLAYER Player NULL!!", getParent()->GetClickedUserKey());
					return;
				}
				BackToTheVillage op;
				op(pPlayer);
//				WARP_PLAYER::Warp( pPlayer,EnterFieldCode,Get()->GetAreaID() );
			}
			else
			{
				/*
				WarpPlayerForCondition op(EnterFieldCode,Get()->GetAreaID());
				getParent()->FindPlayer( op);
				*/
				BackToTheVillage op;
				getParent()->For_EachSatisfiedPlayer(op);
				getParent()->RemoveAllSatisfiedPlayer();
			}
		}
	}
	
	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_PORTAL_PLAYER", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_SET_OBJECTTHRUST)
VOID ACTION_SET_OBJECTTHRUST::OnAction()
{
//	SUNLOG( eFULL_LOG,  "구현안됨!" );
//	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_SET_OBJECTTHRUST", getParent()->TriggerID() );
}


__IMPL_POOL(ACTION_APPLY_DAMAGE)
VOID ACTION_APPLY_DAMAGE::OnAction()
{
//	SUNLOG( eFULL_LOG,  "구현안됨!" );
//	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_APPLY_DAMAGE", getParent()->TriggerID() );
}


/*
// 주변부의 플레이어만을 체크하는 것으로 바꿀 필요 있음
class ReflectDamageHandler
{
	int		m_dist;
	WzVector m_vPos;
public :

	ReflectDamageHandler( WzVector * pvPos, int dist ){ m_vPos = *pvPos; m_dist = dist; }
	~ReflectDamageHandler(){}

	VOID		operator () ( Player * pPlayer )
	{
		WzVector curPos;
		pPlayer->GetPos( &curPos );

		WzVector wvDist = m_vPos - curPos;
		float fDist2 = VectorLength2( &wvDist );

		if( fDist2 <= m_dist )
		{
			pPlayer->DecreaseHP( 15 );
			MSG_CG_TRIGGER_REFLECTDAMAGE_BRD msg;
			msg.m_byCategory	= CG_BATTLE;
			msg.m_byProtocol	= CG_BATTLE_P2O_ATTACK_BRD;
			msg.m_dwObjectKey	= pPlayer->GetObjectKey();
			msg.m_dwDamage		= 15;
			msg.m_dwHP			= pPlayer->GetHP();
			pPlayer->SendPacketAround( (MSG_BASE_FORWARD*)&msg, sizeof(msg) );
		}
	}
};
*/
__IMPL_POOL(ACTION_REFLECT_DAMAGE)
VOID ACTION_REFLECT_DAMAGE::OnAction()
{
	/*
	MapObject * pMapObject = getMGR()->FindMapObject( Get()->m_iObjectID );
	ASSERT( NULL != pMapObject );
	DEBUG_CODE( if( !pMapObject ) return; );

	INT ReflectDamage = (INT)(( pMapObject->GetMaxHP() - pMapObject->GetHP() ) * Get()->m_fDamage);

	// 일정 범위안의 Object에게 데미지 반사
	// TODO: ReflectDamage??

	WzVector vPos;
	pMapObject->GetPos( &vPos );

	GameField * pField = getMGR()->GetField();
	ReflectDamageHandler handler( &vPos, 10 );
	pField->ForAllPlayer( handler );
	*/
//	SUNLOG( eFULL_LOG,  "구현안됨!" );
//	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_REFLECT_DAMAGE", getParent()->TriggerID() );
}



__IMPL_POOL(ACTION_CREATE_MONSTER)
VOID ACTION_CREATE_MONSTER::OnAction()
{
	m_iRegenCount = 0;
	m_RegenTimer.SetTimer(0);	//< 처음에는 반드시 한번 실행한다.
	if( Get()->GetIsParty() == PERSONAL )
	{
		// 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
		DWORD num = getMGR()->GetPlayerNum();
		m_iMaxCreateCount = Get()->GetCreateCount()*(num==0?1:num);
	}
	else
		m_iMaxCreateCount = Get()->GetCreateCount();

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CREATE_MONSTER,개수(%d)", getParent()->TriggerID(), m_iMaxCreateCount );
}
BOOL ACTION_CREATE_MONSTER::OnActionUpdate()
{
	if( m_iRegenCount == Get()->GetLoopCount() ) 
	{
		return FALSE;
	}
	else
	{ 
		if( m_RegenTimer.IsExpired() )
		{
			m_RegenTimer.SetTimer(Get()->GetDelayTime());

			WzSpecialArea * pAreaInfo = getMGR()->GetAreaInfo( Get()->GetAreaID() );
			ASSERT( pAreaInfo );
			if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
			{
//			ASSERT( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
//			DEBUG_CODE( if( pAreaInfo->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX ) return TRUE; );

				WzVector vPos = {0,0,0};
				for( int i = 0 ; i < m_iMaxCreateCount ; ++ i)
				{
					DWORD dwSelectedTile = random( 0, pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount - 1 );
					vPos = getMGR()->GetRandomPosInTile( pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[dwSelectedTile] );
					getMGR()->CreateNPC( Get()->GetMonsterCode(), 1, &vPos, Get()->GetAreaID(), Get()->GetType() );
				}
				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CREATE_MONSTER COUNT(%d), Tick(%u)", getParent()->TriggerID(), m_iRegenCount, GetTickCount() );
				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:Monster(CODE:%d)(Area:%d)(GroupID:%d)", getParent()->TriggerID(), Get()->GetMonsterCode(), Get()->GetAreaID(), Get()->GetType() );
				++m_iRegenCount;
			}
			else if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_SPHERE )
			{
				WzVector vPos = {0,0,0};
				vPos = pAreaInfo->m_wbvRegion.m_BvSphere.m_wvCenter;
				getMGR()->CreateNPC( Get()->GetMonsterCode(), 1, &vPos, Get()->GetAreaID(), Get()->GetType() );
				++m_iRegenCount;
			}
			else
			{
				SUNLOG( eCRITICAL_LOG,  "[TriggerIDID:%u]:ACTION_CREATE_MONSTER:BvType(%d) not implemented", 
					getParent()->TriggerID(), pAreaInfo->m_wbvRegion.m_eBvType );
			}
//			DEBUG_CODE( if( pAreaInfo->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX ) return TRUE; );
		}
		
	}
	return TRUE;
}

__IMPL_POOL(ACTION_DISPLAY_MSGBOX)
VOID ACTION_DISPLAY_MSGBOX::OnAction()
{
	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_DISPLAY_MSGBOX", getParent()->TriggerID() );
}

class RemoveItemFromInventoryForCondition
{
	SLOTCODE	m_ItemCode;
	INT			m_iCount;
public:
	RemoveItemFromInventoryForCondition( SLOTCODE itemCode, INT count ):m_ItemCode(itemCode), m_iCount(count){}
	~RemoveItemFromInventoryForCondition(){}

	
	BOOL operator()( Player* pPlayer )
	{
		INT iItemCode = m_ItemCode;
		ASSERT( SAFE_NUMERIC_TYPECAST(INT, iItemCode, SLOTCODE) );

		ItemManager* pItemMan = pPlayer->GetItemManager();
		SLOTCODE slotCode = (SLOTCODE)iItemCode;
		POSTYPE pos = m_iCount;

		MSG_CG_ITEM_LOSE_ACK msg;
		if( RC::RC_ITEM_SUCCESS == pItemMan->DeleteAllKindOfItem( 1, &slotCode, &pos, &(msg.m_ItemInfo) ) )
		{
			if( msg.m_ItemInfo.m_Count != 0 )
				pPlayer->SendPacket( &msg, msg.GetSize() );
			return TRUE;
		}

		return FALSE;
	}

	//BOOL operator()( Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo )
	//{
	//	//if( pPlayerInfo->m_State )
	//	{
	//		Player * pPlayer = pPlayerInfo->m_pPlayer;
	//		pPlayer->GetItemManager()->Lose( m_ItemCode, m_byAllOfItem );
	//	}
	//	return FALSE;
	//}
};

__IMPL_POOL(ACTION_REMOVE_ITEM)
VOID ACTION_REMOVE_ITEM::OnAction()
{
	DWORD dwUserKey = getParent()->GetClickedUserKey();
	if( 0 != dwUserKey )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( dwUserKey );
		if( NULL != pPlayer )
		{
			INT iItemCode = Get()->GetItemCode();
			ASSERT( SAFE_NUMERIC_TYPECAST(INT, iItemCode, SLOTCODE) );

			ItemManager* pItemMan = pPlayer->GetItemManager();
			SLOTCODE slotCode = (SLOTCODE)iItemCode;
			POSTYPE pos = Get()->GetItemNum();

			MSG_CG_ITEM_LOSE_ACK msg;
			if( RC::RC_ITEM_SUCCESS == pItemMan->DeleteAllKindOfItem( 1, &slotCode, &pos, &(msg.m_ItemInfo) ) )
			{
				if( msg.m_ItemInfo.m_Count != 0 )
					pPlayer->SendPacket( &msg, msg.GetSize() );
			}
		}
		else
		{
			SUNLOG( eFULL_LOG, "[U:%d]ACTION_REMOVE_ITEM Player NULL!!", dwUserKey );
			return;
		}
	}
	else
	{
		RemoveItemFromInventoryForCondition op( Get()->GetItemCode(), Get()->GetItemNum() );
		getParent()->For_EachSatisfiedPlayer( op );
		getParent()->RemoveAllSatisfiedPlayer();
	}

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_REMOVE_ITEM", getParent()->TriggerID() );
}

// 필드상에 있는 플레이어에게 인벤토리에 보상아이템을 준다.
class DistributeItemToInventoryOnField : public unary_function<Character *, void>
{
	ACTION_REWARD_PLAYER* m_pReward;
public :
	DistributeItemToInventoryOnField(ACTION_REWARD_PLAYER* pReward):m_pReward(pReward) {}
	~DistributeItemToInventoryOnField(){}

	VOID	operator () ( Player * pPlayer ) const
	{
		if( !pPlayer->GetField() )	return;

		WzVector vec = pPlayer->GetRandomPosInAround( 2 );
		SCItemSlot slot;
		slot.SetCode( m_pReward->Get()->GetItemCode() );
		g_DropManager.DropItemToField( pPlayer->GetField(), &vec, pPlayer->GetObjectKey(), 0, slot );
	}
	VOID	operator () ( std::pair<DWORD,Player *> pair_type )
	{
		operator()(pair_type.second);
	}
};

// 조건에 합당한 플레이어에게 인벤토리에 보상을 준다.
class DistributeItemToInventoryForCondition
{
	CODETYPE	m_ItemCode;
	INT			m_Count;
public:
	DistributeItemToInventoryForCondition(CODETYPE ItemCode, INT count):m_ItemCode(ItemCode), m_Count(count){}
	~DistributeItemToInventoryForCondition(){}

	BOOL operator()( Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo )
	{
		Player * pPlayer = pPlayerInfo->m_pPlayer;
		///g_DropManager.DropItemToField( pPlayer->GetUserKey(), m_ItemCode );
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE
		///!!!!! temp TEMPTEMPTEMTPE

		return FALSE;
	}
};


__IMPL_POOL(ACTION_REWARD_PLAYER)
VOID ACTION_REWARD_PLAYER::OnAction()
{
	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo( Get()->GetItemCode() );
	if( NULL == pItemInfo )
	{
		// 에러 처리
		return;
	}

	ASSERT( Get()->GetCount() > 0 );

	if( 0 != getParent()->GetClickedUserKey() )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( getParent()->GetClickedUserKey() );
		if( pPlayer )
		{
			WORD wPartyKey = pPlayer->GetPartyState().GetPartyKey();
			GameParty* pParty = GamePartyManager::Instance()->FindParty( wPartyKey );

			if( pParty )
			{
				DistributeItemToInventoryOnField op( (ACTION_REWARD_PLAYER*)this );
				pParty->ForEachMember( op );

				return;
			}

			SUNLOG( eCRITICAL_LOG, _T("[ACTION_REWARD_PLAYER] GameParty is NULL! (%s)'s PartyKey (%u)"), pPlayer->GetCharName(), wPartyKey );
		}
		SUNLOG( eCRITICAL_LOG, "[ACTION_REWARD_PLAYER] Player is NULL! ClickedUserKey %u", getParent()->GetClickedUserKey() );
	}
	else
	{
		DistributeItemToInventoryOnField op( (ACTION_REWARD_PLAYER*)this );
		getMGR()->ForeachPlayer( op );
	}

}

__IMPL_POOL(ACTION_CLEAR_MISSION)
VOID ACTION_CLEAR_MISSION::OnAction()
{
	getMGR()->ClearMission();

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CLEAR_MISSION", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_PORTAL_RANDOM)
VOID ACTION_PORTAL_RANDOM::OnAction()
{
	// 해당 영역을 얻어와서
	MAPCODE EnterFieldCode = Get()->GetMapID();
	if( Get()->GetRandom() == 0 )
	{
		int nRandArea = rand() % 8;
		// 랜덤하게 워프
		if( Get()->GetTarget() == PARTY )
		{
			// 현재 필드에 있는 사람을 모두 워프
			// 조건에 해당하는 사람을 알 필요 없음

			WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID( nRandArea ));
			getMGR()->ForeachPlayer( op );
		}
		else
		{
			ASSERT( Get()->GetTarget() == PERSONAL );

			if( getParent()->GetClickedUserKey() != 0 )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
				if( NULL == pPlayer )
				{
					SUNLOG( eFULL_LOG, "[ACTION_PORTAL_RANDOM::OnAction] [U:%d] Player NULL!!", getParent()->GetClickedUserKey());
					return;
				}
				WARP_PLAYER::Warp( pPlayer,EnterFieldCode,Get()->GetAreaID( nRandArea ) );
			}
			else
			{
				WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID( nRandArea ));
				getParent()->For_EachSatisfiedPlayer(op);
				getParent()->RemoveAllSatisfiedPlayer();
			}
		}
	}
	else //if( Get()->GetRandom() == ACTION_PORTAL_RANDOM::_ORDER )
	{
		// 파티원들을 순서대로 이동시킨다
		if( Get()->GetTarget() == PARTY )
		{
			// 현재 필드에 있는 사람을 모두 워프
			// 조건에 해당하는 사람을 알 필요 없음

			BackToTheVillage op;
			getMGR()->ForeachPlayer( op );
			//WarpPlayerForField op(EnterFieldCode,Get()->GetAreaID());
			
		}
		else
		{
			ASSERT( Get()->GetTarget() == PERSONAL );

			if( getParent()->GetClickedUserKey() != 0 )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
				if( NULL == pPlayer )
				{
					SUNLOG( eFULL_LOG, "[ACTION_PORTAL_PLAYER::OnAction] [U:%d]ACTION_REWARD_PLAYER Player NULL!!", getParent()->GetClickedUserKey());
					return;
				}
				BackToTheVillage op;
				op(pPlayer);
//				WARP_PLAYER::Warp( pPlayer,EnterFieldCode,Get()->GetAreaID() );
			}
			else
			{
				/*
				WarpPlayerForCondition op(EnterFieldCode,Get()->GetAreaID());
				getParent()->FindPlayer( op);
				*/
				BackToTheVillage op;
				getParent()->For_EachSatisfiedPlayer(op);
				getParent()->RemoveAllSatisfiedPlayer();
			}
		}
	}
	
//	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_PORTAL_PLAYER", getParent()->TriggerID() );

	// 이동했다고 알려준다
//	SUNLOG( eFULL_LOG,  "구현안됨!" );
//	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_PORTAL_RANDOM", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_ACTIVATE_TRIGGER)
VOID ACTION_ACTIVATE_TRIGGER::OnAction()
{
#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
	if( Get()->GetFlag()==0?TRUE:FALSE )
		getMGR()->InsertActiveTrigger( Get()->GetWzTriggerID() );
	else
		getMGR()->RemoveActiveTrigger( Get()->GetWzTriggerID() );
#else
	getMGR()->ActiveTrigger( Get()->GetWzTriggerID(), (Get()->GetFlag()==0?TRUE:FALSE) );
#endif

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_ACTIVATE_TRIGGER", getParent()->TriggerID() );
}

__IMPL_POOL(ACTION_AREA_DAMAGE)

#ifndef __WAVERIX_APPLY_AREA_DAMAGE_BY_DATA__
class AreaDamageHandler
{
	DWORD	m_dwObjectKey;
	int		m_dist;
	WzVector m_vPos;
	DAMAGETYPE	m_wDamage;
public :

	AreaDamageHandler( DWORD dwObjectKey, WzVector * pvPos, int dist, DAMAGETYPE wDamage )
	{
		m_dwObjectKey = dwObjectKey;
		m_vPos = *pvPos;
		m_dist = dist*dist;
		m_wDamage = wDamage;
	}
	~AreaDamageHandler(){}

	VOID		operator () ( std::pair<DWORD, Player *> pair_type )
	{
		Player * pPlayer = pair_type.second;
		WzVector curPos;
		pPlayer->GetPos( &curPos );

		WzVector wvDist = m_vPos - curPos;
		float fDist2 = VectorLength2( &wvDist );

		if( fDist2 <= m_dist )
		{
			pPlayer->DecreaseHP( m_wDamage );
			MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
			msg.m_dwObjectKey	= pPlayer->GetObjectKey();
			msg.m_wDamage		= m_wDamage;
			msg.m_dwHP			= pPlayer->GetHP();
			pPlayer->SendPacketAround( (MSG_BASE_FORWARD*)&msg, sizeof(msg) );
		}
	}
};

VOID ACTION_AREA_DAMAGE::OnAction()
{
	// 오브젝트 받아서
	MapObject * pMapObject = getMGR()->FindMapObject( Get()->GetObjectIndex() );
	ASSERT(NULL != pMapObject);
	DEBUG_CODE( if( !pMapObject ) return ; );

	// 범위 안에 타겟(개인/파티) 있는지 체크하여 데미지 주자
	WzVector vPos;
	pMapObject->GetPos( &vPos );

	AreaDamageHandler handler( Get()->GetObjectIndex(), &vPos, 20, (DAMAGETYPE)Get()->GetDamage() );
	getMGR()->ForeachPlayer( handler );	// 범위 안에 개인이든 파티든 있EN으면 다 주는 거 아닌가?

	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_AREA_DAMAGE", getParent()->TriggerID() );
}

#else //__WAVERIX_APPLY_AREA_DAMAGE_BY_DATA__

class AreaDamageHandler
{
	DWORD		m_dwObjectKey;
	float		m_dist;
	WzVector	m_vPos;
	WORD		m_wDamage;
public :

	AreaDamageHandler( DWORD dwObjectKey, WzVector* pvPos, float dist, WORD wDamage )
	{
		m_dwObjectKey = dwObjectKey;
		m_vPos = *pvPos;
		m_dist = dist*dist;
		m_wDamage = wDamage;
	}
	~AreaDamageHandler(){}

	VOID		operator () ( std::pair<DWORD, Player *> pair_type )
	{
		Player * pPlayer = pair_type.second;
		WzVector curPos;
		pPlayer->GetPos( &curPos );

		WzVector wvDist = m_vPos - curPos;
		float fDist2 = VectorLength2( &wvDist );

		if( fDist2 <= m_dist )
		{
			pPlayer->DecreaseHP( m_wDamage );
			// <Waverix, 061018> 패킷 처리 최적화할 여지가 보인다
			MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
			msg.m_dwObjectKey	= pPlayer->GetObjectKey();
			msg.m_wDamage		= m_wDamage;
			msg.m_dwHP			= pPlayer->GetHP();
			pPlayer->SendPacketAround( (MSG_BASE_FORWARD*)&msg, sizeof(msg) );
		}
	}
};

VOID ACTION_AREA_DAMAGE::OnAction()
{
	ACTION_AREA_DAMAGE_INFO* pINFO = Get();

	// 오브젝트 받아서
	MapObject * pMapObject = getMGR()->FindMapObject( pINFO->GetObjectIndex() );
	ASSERT(NULL != pMapObject);
	DEBUG_CODE( if( !pMapObject ) return ; );

	// 범위 안에 타겟(개인/파티) 있는지 체크하여 데미지 주자
	WzVector vPos;
	pMapObject->GetPos( &vPos );

	float fRange = pINFO->GetRange();
	AreaDamageHandler handler( pINFO->GetObjectIndex(), &vPos, fRange, (WORD)pINFO->GetDamage() );
	getMGR()->ForeachPlayer( handler );	// 범위 안에 개인이든 파티든 있EN으면 다 주는 거 아닌가?

	SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_AREA_DAMAGE", getParent()->TriggerID() );
}

#endif //__WAVERIX_APPLY_AREA_DAMAGE_BY_DATA__

__IMPL_POOL(ACTION_OPERATE_SWITCH)
VOID ACTION_OPERATE_SWITCH::OnAction()
{
	BYTE Value = getMGR()->GetSwitchValue( Get()->GetSwitchID() );
	BYTE Result = 0;
	sSWITCH ( Get()->GetOperationType() )
	CASE(eOPERATION_EQUAL)
		getMGR()->SetSwitchValue( Get()->GetSwitchID(), Get()->GetArgument() );
	CASE(eOPERATION_PLUS)
		Result = Value + Get()->GetArgument();
		getMGR()->SetSwitchValue( Get()->GetSwitchID(), Result );
	CASE(eOPERATION_MINUS)
		Result = Value - Get()->GetArgument();
		getMGR()->SetSwitchValue( Get()->GetSwitchID(), Result );
	CASE(eOPERATION_MULTIPLE)
		Result = Value * Get()->GetArgument();
		getMGR()->SetSwitchValue( Get()->GetSwitchID(), Result );
	CASE(eOPERATION_DIVIDE)
		Result = Value / Get()->GetArgument();
		getMGR()->SetSwitchValue( Get()->GetSwitchID(), Result );
	eSWITCH
	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_OPERATE_SWITCH", getParent()->TriggerID() );
}

__IMPL_POOL( ACTION_ROAD_CONNECT )		// 길 연결을 설정/해제 한다
VOID ACTION_ROAD_CONNECT::OnAction()
{
}

__IMPL_POOL(ACTION_CREATE_DIR_MONSTER)
VOID ACTION_CREATE_DIR_MONSTER::OnAction()
{
	m_iRegenCount = 0;
	m_RegenTimer.SetTimer(0);	//< 처음에는 반드시 한번 실행한다.
	if( Get()->GetIsParty() == PERSONAL )
	{
		// 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
		DWORD num = getMGR()->GetPlayerNum();
		m_iMaxCreateCount = Get()->GetCreateCount()*(num==0?1:num);
	}
	else
		m_iMaxCreateCount = Get()->GetCreateCount();

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_CREATE_MONSTER,개수(%d)", getParent()->TriggerID(), m_iMaxCreateCount );
}
BOOL ACTION_CREATE_DIR_MONSTER::OnActionUpdate()
{
	if( m_iRegenCount == Get()->GetLoopCount() ) 
	{
		return FALSE;
	}
	else
	{ 
		if( m_RegenTimer.IsExpired() )
		{
			m_RegenTimer.SetTimer(Get()->GetDelayTime());

			WzSpecialArea * pAreaInfo = getMGR()->GetAreaInfo( Get()->GetAreaID() );
			ASSERT( pAreaInfo );

			float fAngle;
			int iAngle;
			if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
			{
//			ASSERT( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
//			DEBUG_CODE( if( pAreaInfo->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX ) return TRUE; );

				WzVector vPos = {0,0,0}, vDir = {Get()->GetDirX(), Get()->GetDirY(), Get()->GetDirZ()}, vNormal;
				VectorNormalize( &vNormal, &vDir );
				fAngle = Math_GetAngleFromVector( &vNormal );
				iAngle = static_cast< int > ( fAngle * 180.0f / WZ_PI );	// 프로토콜에 넣어줄 최종 앵글값
				for( int i = 0 ; i < m_iMaxCreateCount ; ++ i)
				{
					DWORD dwSelectedTile = random( 0, pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount - 1 );
					vPos = getMGR()->GetRandomPosInTile( pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[dwSelectedTile] );
					getMGR()->CreateNPC( Get()->GetMonsterCode(), 1, &vPos, Get()->GetAreaID(), 0, iAngle );
				}
	//			SUNLOG( eFULL_LOG,  "[ID:%u]:ACTION_CREATE_MONSTER COUNT(%d), Tick(%u)", getParent()->TriggerID(), m_iRegenCount, GetTickCount() );
	//			SUNLOG( eFULL_LOG,  "[ID:%u]:Monster(CODE:%d)(Area:%d)(GroupID:%d)", getParent()->TriggerID(), Get()->GetMonsterCode(), Get()->GetAreaID(), Get()->GetType() );
				++m_iRegenCount;
			}
			else if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_SPHERE )
			{
				WzVector vPos = {0,0,0}, vDir = {Get()->GetDirX(), Get()->GetDirY(), Get()->GetDirZ()}, vNormal;
				VectorNormalize( &vNormal, &vDir );
				fAngle = Math_GetAngleFromVector( &vNormal );
				iAngle = static_cast< int > ( fAngle * 180.0f / WZ_PI );	// 프로토콜에 넣어줄 최종 앵글값

				vPos = pAreaInfo->m_wbvRegion.m_BvSphere.m_wvCenter;
				getMGR()->CreateNPC( Get()->GetMonsterCode(), 1, &vPos, Get()->GetAreaID(), 0, iAngle );
				++m_iRegenCount;
			}
//			DEBUG_CODE( if( pAreaInfo->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX ) return TRUE; );
		}
		
	}
	return TRUE;
}

__IMPL_POOL(ACTION_CHANGE_LIGHT)
VOID ACTION_CHANGE_LIGHT::OnAction()
{
#ifdef __NA000405_070406_LAST_OBJSTATE_APPLICATION__
	getMGR()->MAP_STATE.Change( getParent()->TriggerID() );
#endif //
}

__IMPL_POOL(ACTION_CHANGE_OBJECT_TYPE)
VOID ACTION_CHANGE_OBJECT_TYPE::OnAction()
{
#ifdef __NA000405_070406_LAST_OBJSTATE_APPLICATION__
	getMGR()->MAP_STATE.Change( getParent()->TriggerID() );
#endif //
}

struct ActionOperator
{
	class QUEST
	{
	public:
		// ActionTrigger <퀘스트 만족여부>
		class Satisfaction
		{
			QCODE	m_QuestCode;
		public:
			Satisfaction( QCODE wQuestCode ) : m_QuestCode(wQuestCode) {}
			VOID operator() ( Player* pPlayer )
			{
				// <060606, waverix>
				//   not implementation
				SUNLOG( eFULL_LOG, "ActionOperator::QUEST::Satisfaction is not implemented." );
				//Quest* pQuest = pPlayer->GetQuestManager()->FindQuest( m_QuestCode );
				//if( !pQuest )
				//{
				//	
				//}
			}
		};	//ActionOperator::QUEST::Satisfaction

		// ActionTrigger <퀘스트 부여>
		class Grant
		{
			QCODE	m_QuestCode;
		public:
			Grant( QCODE wQuestCode ) : m_QuestCode(wQuestCode) {}

			// 아래의 코드는
			VOID operator() ( Player* pPlayer )
			{
				QuestManager* pQuestMan = pPlayer->GetQuestManager();
				Quest * pQuest = pQuestMan->CreateQuest( m_QuestCode );

				if( pQuest && (!pQuest->GetQuestInfo()->IsChildQuest()) )
				{
					if( pQuestMan->CanDoAccept( m_QuestCode, pPlayer ) )
					{
						// <Waverix, 060607> 아래의 패킷은 트리거용으로 하나 새로 만들어서 사용할 것.
						MSG_CG_QUEST_ACCEPT_ACK msg;
						pPlayer->SendPacket( &msg, sizeof(msg) );
						return;
					}
				}

				pQuestMan->DestroyQuest( m_QuestCode );
			}
		};	// ActionOperator::QUEST::Grant
	};	// ActionOperator::QUEST
};	// ActionOperator

// <Waverix>
// Quest 관련 미구현 또는 분석 미비로 인한 트리거 항목 완료 미설정
__IMPL_POOL( ACTION_QUEST_SATISFACTION )
VOID ACTION_QUEST_SATISFACTION::OnAction()
{
	const ACTION_QUEST_SATISFACTION_INFO* pINFO = (const ACTION_QUEST_SATISFACTION_INFO*)Get();
	ASSERT( SAFE_NUMERIC_TYPECAST(ULONG, pINFO->GetQuestID(), QCODE) );

	ActionOperator::QUEST::Satisfaction op( (QCODE)pINFO->GetQuestID() );

	if( getParent()->GetClickedUserKey() != 0 )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
		if( pPlayer )
		{
			op( pPlayer );
			return;
		}

		SUNLOG( eFULL_LOG, "[ACTION_QUEST_SATISFACTION::OnAction] [U:%d] Player NULL!!", getParent()->GetClickedUserKey());
		return;
	}
	else
	{
		// Satisfied Player
		getParent()->For_EachSatisfiedPlayer( op );
		getParent()->RemoveAllSatisfiedPlayer();
	}
}



__IMPL_POOL( ACTION_QUEST_GRANT )
VOID ACTION_QUEST_GRANT::OnAction()
{
	const ACTION_QUEST_GRANT_INFO* pINFO = (const ACTION_QUEST_GRANT_INFO*)Get();
	ASSERT( SAFE_NUMERIC_TYPECAST(ULONG, pINFO->GetQuestID(), QCODE) );

	ActionOperator::QUEST::Grant op( (QCODE)pINFO->GetQuestID() );

	if( getParent()->GetClickedUserKey() != 0 )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(getParent()->GetClickedUserKey());
		if( pPlayer )
		{
			op( pPlayer );
			return;
		}

		SUNLOG( eFULL_LOG, "[ACTION_QUEST_SATISFACTION::OnAction] [U:%d] Player NULL!!", getParent()->GetClickedUserKey());
	}
	else
	{
		// Satisfied Player
		getParent()->For_EachSatisfiedPlayer( op );
		getParent()->RemoveAllSatisfiedPlayer();
	}
}

__IMPL_POOL( ACTION_QUEST_SETUP )
VOID ACTION_QUEST_SETUP::OnAction()
{
}

__IMPL_POOL( ACTION_USE_CATEGORY )
VOID ACTION_USE_CATEGORY::OnAction()
{
}

// 기존에 존재하는 액션 트리거
//__IMPL_POOL( ACTION_CREATE_MONSTER )

__IMPL_POOL( ACTION_CREATE_MONSTER_GROUP )
VOID ACTION_CREATE_MONSTER_GROUP::OnAction()
{
	// 다음의 루틴은 ACTION_CREATE_MONSTER 의 로직을 따라한 것이다.
	m_iRegenCount = 0;
	m_dwTimeout = 0;

	const ACTION_CREATE_MONSTER_GROUP_INFO* pINFO = (const ACTION_CREATE_MONSTER_GROUP_INFO*)Get();
	if( PARTY == pINFO->GetPartyState() )
	{
		m_iMaxCreateCount = pINFO->GetGroupCount();
	}
	else //PERSONAL
	{
		// 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
		DWORD count = getMGR()->GetPlayerNum();
		m_iMaxCreateCount = pINFO->GetGroupCount() * (count==0 ? 1 : count);
	}
}

#include "NPCGroupManager.h"

BOOL ACTION_CREATE_MONSTER_GROUP::OnActionUpdate()
{
	const ACTION_CREATE_MONSTER_GROUP_INFO* pINFO = (const ACTION_CREATE_MONSTER_GROUP_INFO*)Get();

	if( pINFO->GetGroupCount() == m_iRegenCount )
	{
		return FALSE;
	}
	else
	{
		// 몬스터 그룹 생성은 주기가 없다. 하지만 한번에 생성함으로 인한 부하(꼭 서버만의 부하는 아닐터)
		// 를 감안해 적당한 주기를 주고 생성하기로 하자. 단 이를 위한 테스트는 해봐야 할 것이다.
		const DWORD dwInterval = 100000;	//10초?
		DWORD dwCurTick = GetTickCount();
		if( m_dwTimeout < dwCurTick )
		{
			m_dwTimeout = dwCurTick + dwInterval;

			const ACTION_CREATE_MONSTER_GROUP_INFO* pINFO = (const ACTION_CREATE_MONSTER_GROUP_INFO*)Get();

			ASSERT( SAFE_NUMERIC_TYPECAST(INT, pINFO->GetAreaID(), DWORD) );
			ASSERT( SAFE_NUMERIC_TYPECAST(INT, pINFO->GetAreaID(), WORD) );

			GameZone* pZone =
#ifdef __WAVERIX_PREV_CODE_CHANGE_BY_TAIYO_CODE_
					getMGR()->GetField()->GetMap()->GetGameZone();

			INT count = pINFO->GetGroupCount();

			for( INT i=0 ; i<count ; ++i )
			{
				NPCGroupManager::Instance()->CreateGroup(
					pZone->GetKey(), (DWORD)pINFO->GetAreaID(), (WORD)pINFO->GetGroupID() );
			}
#else
					NULL;
#endif
			
		}
	}
	return TRUE;
}

__IMPL_POOL( ACTION_SHOW_OR_HIDE_UNIT )
VOID ACTION_SHOW_OR_HIDE_UNIT::OnAction()
{

	// 맵툴의 관점에서는 몬스터 나타남/사라짐이지만, 실제 서버에서는
	// 몬스터 유닛 생성 / 파괴

	const ACTION_SHOW_OR_HIDE_UNIT_INFO* pINFO = (const ACTION_SHOW_OR_HIDE_UNIT_INFO*)Get();
#ifdef __NA_276_20070213_UNIT_TRIGGER
	if( pINFO->GetIsHide() == ACTION_SHOW_OR_HIDE_UNIT_INFO::SHOW )
	{
		getMGR()->CreateNPCUnit( pINFO->GetMonsterUnitID() );
	}
	else if( pINFO->GetIsHide() == ACTION_SHOW_OR_HIDE_UNIT_INFO::HIDE )
	{
		getMGR()->DestroyNPCUnit( pINFO->GetMonsterUnitID() );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "ACTION_SHOW_OR_HIDE_UNIT:Invalid Value(%u)", pINFO->GetIsHide() );
	}
#endif
}


__IMPL_POOL( ACTION_SHOW_GUIDE_MSGBOX )
VOID ACTION_SHOW_GUIDE_MSGBOX::OnAction()
{
#ifndef __WAVERIX_NEW_TRIGGER_WORKS_TEST__
	SUNLOG( eCRITICAL_LOG, "ACTION_SHOW_GUIDE_MSGBOX::OnAction" );
#endif
}

__IMPL_POOL( ACTION_SHOW_OR_HIDE_MSGBOX )
VOID ACTION_SHOW_OR_HIDE_MSGBOX::OnAction()
{
}

// 몬스터 랜덤 스폰
#ifdef __NA_000277_20070306_MONSTER_RANDOM_SPAWN_TRIGGER
__IMPL_POOL( ACTION_RANDOM_MONSTER_SPAWN )
VOID ACTION_RANDOM_MONSTER_SPAWN::OnAction()
{
	m_nCurRegenCount = 0;
	m_RegenTimer.SetTimer(0);	//< 처음에는 반드시 한번 실행한다.

	m_MonsterRandomizer.Clear();

	BOOL rt = FALSE;
	// 몬스터 1부터 5까지 대입한다.
	rt = m_MonsterRandomizer.AddRatio( Get()->GetMonsterID_1(), Get()->GetMonsterRate_1(), MAX_MONSTER_RATIO );
	ASSERT( rt );
	rt = m_MonsterRandomizer.AddRatio( Get()->GetMonsterID_2(), Get()->GetMonsterRate_2(), MAX_MONSTER_RATIO );
	ASSERT( rt );
	rt = m_MonsterRandomizer.AddRatio( Get()->GetMonsterID_3(), Get()->GetMonsterRate_3(), MAX_MONSTER_RATIO );
	ASSERT( rt );
	rt = m_MonsterRandomizer.AddRatio( Get()->GetMonsterID_4(), Get()->GetMonsterRate_4(), MAX_MONSTER_RATIO );
	ASSERT( rt );
	rt = m_MonsterRandomizer.AddRatio( Get()->GetMonsterID_5(), Get()->GetMonsterRate_5(), MAX_MONSTER_RATIO );
	ASSERT( rt );
}

BOOL ACTION_RANDOM_MONSTER_SPAWN::OnActionUpdate()
{
	// 현재 리젠된 횟수와 Max리젠 횟수가 같다면... 그만 한다.
	if( m_nCurRegenCount == Get()->GetMaxRegenCount() ) 
	{
		return FALSE;
	}
	else
	{ 
		//1. 리젠시간이 지났다면..
		if( m_RegenTimer.IsExpired() )
		{
			//2. 리젠시간을 다시 적용하고..
			m_RegenTimer.SetTimer( Get()->GetRegenTime() );

			//3. 현재 리젠영역를 구한다.
			WzSpecialArea * pAreaInfo = getMGR()->GetAreaInfo( Get()->GetAreaID() );
			ASSERT( pAreaInfo );

			float fAngle;
			int iAngle;

			if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
			{
				WzVector vPos = {0,0,0}, vDir = {Get()->GetDirX(), Get()->GetDirY(), Get()->GetDirZ()}, vNormal;
				VectorNormalize( &vNormal, &vDir );
				fAngle = Math_GetAngleFromVector( &vNormal );
				iAngle = static_cast< int > ( fAngle * 180.0f / WZ_PI );	// 프로토콜에 넣어줄 최종 앵글값
				int nMonsterCnt = GetMonsterCount();

				for( int i = 0 ; i < nMonsterCnt ; ++ i)
				{
					DWORD dwSelectedTile = random( 0, pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount - 1 );
					vPos = getMGR()->GetRandomPosInTile( pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[dwSelectedTile] );
					getMGR()->CreateNPC( GetRandomMonster(), 1, &vPos, Get()->GetAreaID(), 0, iAngle );
				}
				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_RANDOM_MONSTER_SPAWN COUNT(%d), Tick(%u)", getParent()->TriggerID(), m_nCurRegenCount, GetTickCount() );
				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:Monster(CODE:%d)(Area:%d)(GroupID:%d)", getParent()->TriggerID(), GetRandomMonster(), Get()->GetAreaID(), 0 );
				++m_nCurRegenCount;
			}
			else if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_SPHERE )
			{
				WzVector vPos = {0,0,0}, vDir = {Get()->GetDirX(), Get()->GetDirY(), Get()->GetDirZ()}, vNormal;
				VectorNormalize( &vNormal, &vDir );
				fAngle = Math_GetAngleFromVector( &vNormal );
				iAngle = static_cast< int > ( fAngle * 180.0f / WZ_PI );	// 프로토콜에 넣어줄 최종 앵글값

				vPos = pAreaInfo->m_wbvRegion.m_BvSphere.m_wvCenter;
				int nMonsterCnt = GetMonsterCount();
				getMGR()->CreateNPC( GetRandomMonster(), 1, &vPos, Get()->GetAreaID(), 0, iAngle );
				if( nMonsterCnt > 1 )
					SUNLOG( eCRITICAL_LOG,  "[TriggerID:%u]:ACTION_RANDOM_MONSTER_SPAWN[BVT_SPHERE] Invalid Monster Count(%d), Tick(%u)", getParent()->TriggerID(), nMonsterCnt, GetTickCount() );

				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_RANDOM_MONSTER_SPAWN COUNT(%d), Tick(%u)", getParent()->TriggerID(), m_nCurRegenCount, GetTickCount() );
				SUNLOG( eDEV_LOG,  "[TriggerID:%u]:Monster(CODE:%d)(Area:%d)(GroupID:%d)", getParent()->TriggerID(), GetRandomMonster(), Get()->GetAreaID(), 0 );
				++m_nCurRegenCount;
			}
			else
			{
				SUNLOG( eCRITICAL_LOG,  "[TriggerIDID:%u]:ACTION_RANDOM_MONSTER_SPAWN:BvType(%d) not implemented", 
					getParent()->TriggerID(), pAreaInfo->m_wbvRegion.m_eBvType );
			}
		}

	}
	return TRUE;
}

//리젠시킬 몬스터의 개수를 구한다.
INT	 ACTION_RANDOM_MONSTER_SPAWN::GetMonsterCount()
{
	int nMonsterCount = 0;
	int nRandomCnt = random( Get()->GetMinCreateMonsterCnt(), Get()->GetMaxCreateMonsterCnt() );

	if( Get()->GetIsParty() == PERSONAL )
	{
		// 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
		DWORD dwPlayerNum = getMGR()->GetPlayerNum();
		nMonsterCount = nRandomCnt*(dwPlayerNum==0?1:dwPlayerNum);
	}
	else
		nMonsterCount = nRandomCnt;

	SUNLOG( eDEV_LOG,  "[TriggerID:%u]:ACTION_RANDOM_MONSTER_SPAWN, 개수(%d)", getParent()->TriggerID(), nMonsterCount );

	return nMonsterCount;
}

INT	 ACTION_RANDOM_MONSTER_SPAWN::GetRandomMonster()
{
	return m_MonsterRandomizer.RandomizedKey();
}

#endif  //__NA_000277_20070306_MONSTER_RANDOM_SPAWN_TRIGGER


//#ifdef __NA_000104_20070123_TOP_OF_TRIAL

class ShowTheFloor
{
private:
	MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD&	m_rMsgCMD;
	WORD								m_wPartyKey;
public:
	ShowTheFloor( MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD& rMsgCMD ) : m_rMsgCMD(rMsgCMD), m_wPartyKey(0) {}
	VOID operator() ( Player* pPlayer ) 
	{
		WORD wPartyKey = pPlayer->GetPartyState().GetPartyKey();
		if( m_wPartyKey != wPartyKey )
		{
			m_rMsgCMD.m_dwPlayerKey = pPlayer->GetObjectKey();
			pPlayer->SendPacketToParty( &m_rMsgCMD, sizeof(m_rMsgCMD) );
			m_wPartyKey = wPartyKey;
		}
	}
};

__IMPL_POOL( ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR )
VOID ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR::OnAction()
{
#ifdef __NA000357_070322_TRIGGER_SHOW_THE_FLOOR__
	ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR_INFO* pINFO = Get();

	MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD msgCMD;
	msgCMD.m_byFloor = pINFO->GetFloor();

	DWORD dwUserKey = getParent()->GetClickedUserKey();
	if( 0 != dwUserKey )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( dwUserKey );
		if( pPlayer )
		{
			msgCMD.m_dwPlayerKey = pPlayer->GetObjectKey();
			pPlayer->SendPacketToParty( &msgCMD, sizeof(msgCMD) );
		}
		
	}
	else
	{
		ShowTheFloor fnShowTheFloor( msgCMD );
		getParent()->For_EachSatisfiedPlayer( fnShowTheFloor );
	}
#endif //
}

//#endif //

