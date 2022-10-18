#include "StdAfx.h"
#include ".\conditiontriggers.h"

#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
	#include ".\TriggerManagerEx.h"
	#include "..\\..\\SolarTrigger\\TriggerMsg.h"
#else
	#include ".\TriggerManager.h"
	#include ".\TriggerMsg.h"
	#include "TriggerAreaHash.h"
#endif

#include ".\Trigger.h"
#include ".\Player.h"
#include ".\ItemManager.h"
#include ".\MissionManager.h"

#include "GameZone.h"

#include "Map.h"
#include <MapObject.h>

#define TYPE_CHECKER(outtype, chktype, variable)	\
	( ( ((outtype)((chktype)0xFFFFFFFFFFFFFFFFLL))&variable ) == variable )

__IMPL_POOL(CONDITION_ALWAYS)
VOID CONDITION_ALWAYS::OnInit()
{
	setSatisfied( TRUE );
}

__IMPL_POOL(CONDITION_COMPARE_SWITCH)
BOOL CONDITION_COMPARE_SWITCH::IsSatisfied()
{
	if( m_bSatisfied ) return TRUE;

	// OPT //@@@@@@ �����ͷ� �̸�(OnInit()) �����ص����ν� ����ȭ ����
	BYTE Value = getMGR()->GetSwitchValue( Get()->GetSwitchID() );

	sSWITCH ( Get()->GetOperationType() )
	CASE(eCOMPARE_EQUAL)
		if( Value == Get()->GetValue() ) setSatisfied( TRUE );
	CASE(eCOMPARE_MORE)
		if( Value > Get()->GetValue() ) setSatisfied( TRUE );
	CASE(eCOMPARE_LESS)
		if( Value < Get()->GetValue() ) setSatisfied( TRUE );
	CASE(eCOMPARE_LESSTHAN)
		if( Value <= Get()->GetValue() ) setSatisfied( TRUE );
	CASE(eCOMPARE_MORETHAN)
		if( Value >= Get()->GetValue() ) setSatisfied( TRUE );
	CASE(eCOMPARE_NOTEQUAL)
		if( Value != Get()->GetValue() ) setSatisfied( TRUE );
	eSWITCH

	return m_bSatisfied;
}


__IMPL_POOL(CONDITION_CLICK_OBJECT)
VOID CONDITION_CLICK_OBJECT::onMsg( TRIGGER_MSG * pmsg )
{
	switch( pmsg->m_dwMsgID )
	{
	case eTRIGGER_CLICK_OBJECT:
		{
			TRIGGER_CLICK_OBJECT * pMsg = (TRIGGER_CLICK_OBJECT *)pmsg;
			
			ASSERT( Get()->GetObjectID() >= 0 );
			if( pMsg->m_dwObjectKey == (DWORD)Get()->GetObjectID() )
			{
				getParent()->SetClickedUserKey( pMsg->m_dwUserKey );
				SUNLOG( eFULL_LOG,  "[CONDITION_CLICK_OBJECT::onMsg] [TriggerID:%u]:CONDITION_CLICK_OBJECT", getParent()->TriggerID() );
				setSatisfied( TRUE );

				MSG_CG_TRIGGER_CLICK_OBJECT_BRD brdMsg;
				Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwUserKey );
				if( pPlayer != NULL )
				{
					brdMsg.m_dwClickedUserObjectKey = pPlayer->GetObjectKey();
					brdMsg.m_dwClickedObjectKey = pMsg->m_dwObjectKey;
					pPlayer->SendPacketAround( &brdMsg, sizeof(MSG_CG_TRIGGER_CLICK_OBJECT_BRD) );
					return;
				}
			}
		}
		break;
//	default:
//		SUNLOG( eFULL_LOG,  "[CONDITION_CLICK_OBJECT::onMsg] [TriggerID:%u]:CONDITION_CLICK_OBJECT �� �ƴѵ� ����� �Դ�. MsgID = %u", getParent()->TriggerID(), pmsg->m_dwMsgID );
//		break;
	}
}

VOID CONDITION_CLICK_OBJECT::OnReset()
{
	getParent()->SetClickedUserKey( 0 );
	setSatisfied( FALSE );
}

__IMPL_POOL(CONDITION_DAMAGE_OBJECT)
VOID CONDITION_DAMAGE_OBJECT::onMsg( TRIGGER_MSG * pmsg )
{
	if( eTRIGGER_DAMAGE_OBJECT == pmsg->m_dwMsgID )
	{
		TRIGGER_DAMAGE_OBJECT * pMsg = (TRIGGER_DAMAGE_OBJECT *)pmsg;

		ASSERT( Get()->GetObjectID() >= 0 );
		if( pMsg->m_dwObjectKey == (DWORD)Get()->GetObjectID() )
		{
			//�� ������ �÷��̾ �ִ��� üũ �ʿ�...�Ѱ�?
			MapObject* pMapObject = getMGR()->FindMapObject( pMsg->m_dwObjectKey );
			// �׾�����???
			ASSERT( NULL != pMapObject );
			if( NULL != pMapObject )
			{
				// �������� ���� ���� MapNpc��. MapObject�� 0���� �����Ϸ���,
				// FindMapObject�� Ʈ���� �޽����� ������ ���� �����ؾ� �ϴµ�, ���� �׷� �ʿ䰡 ������? ��...
				// 
				//if( pMapObject->GetHP() == 0 )
				//{
				//	getMGR()->RemoveMapObject( pMsg->m_dwObjectKey );
					// �޽��� ������ �ϳ�?
					setSatisfied(TRUE);
				//}
				return;
			}
			setSatisfied(FALSE);

			//MapObject * pMapObject = getMGR()->FindMapObject( m_pCondition->m_iObjectID );
			//ASSERT(NULL != pMapObject);
			//DEBUG_CODE( if( !pMapObject ) return FALSE; );
			//
			//if( pMapObject->GetHP() <= m_pCondition->m_fDamage )
			//{
			//	SUNLOG( eFULL_LOG,  "[ID:%u]:TriggerDamageObjectCondition", m_pContainer->GetID() );
			//	setSatisfied( TRUE );
			//}
		}
	}
}


__IMPL_POOL(CONDITION_COUNT_NPCKILL)

VOID CONDITION_COUNT_NPCKILL::OnInstanciate()
{
	m_PartyMemberHash.Initialize(MAX_PARTYMEMBER_NUM);
}
VOID CONDITION_COUNT_NPCKILL::OnInit()
{
	m_PartyMemberHash.RemoveAll();
	memset( m_PartyMember, 0, sizeof(PARTY_MEMBER)*MAX_PARTYMEMBER_NUM );
}

VOID CONDITION_COUNT_NPCKILL::onMsg( TRIGGER_MSG * pmsg )
{
	switch( pmsg->m_dwMsgID )
	{
	case eTRIGGER_KILL_MONSTER:
		{
			TRIGGER_KILL_MONSTER* pMsg = (TRIGGER_KILL_MONSTER *)pmsg;
			CONDITION_COUNT_NPCKILL_INFO* pINFO = Get();

			if( PARTY == pINFO->GetIsParty() )
			{
				ASSERT( SAFE_NUMERIC_TYPECAST(INT, pINFO->GetMonsterCode(), MONSTERCODE) );

				PARTY_MEMBER* pMEMBER = &(m_PartyMember[0]);
				if( pMsg->m_dwMonsterCode != (DWORD)pINFO->GetMonsterCode() )
				{
					// ��κ��� �� ���ǿ� �ɸ� ���̴�.
					return;
				}
				else
				{
					// else�� �����̴�. ��, ������ ���...
					++(pMEMBER->m_iKillCount);
				}

				if( pINFO->GetKillCount() > pMEMBER->m_iKillCount )
				{
					return;
				}

				//------------------------
				// ���� ����
				// == or <
				setSatisfied( TRUE );
				SUNLOG( eFULL_LOG,	\
					"[CONDITION_COUNT_NPCKILL::onMsg] [ID:%u]:MONSTER:[%u], KILL CONDITION %u : KILL %u ",	\
					getParent()->TriggerID(), pINFO->GetMonsterCode(),	\
					pINFO->GetKillCount(), m_PartyMember[0].m_iKillCount );
				//------------------------

				if( pINFO->GetKillCount() < pMEMBER->m_iKillCount )
				{
					// < �� ���ǿ� �ɸ��� ��찡 ������? Ȥ�� �𸥴�. �ִٸ� ã���� 060601, WAVERIX
					SUNLOG( eCRITICAL_LOG,	\
						"[CONDITION_COUNT_NPCKILL::onMsg] Over Count Killed! TriggerID [%u]: MonsterID [%u], MAX [%u]<KillCount [%u]",	\
						getParent()->TriggerID(), pINFO->GetMonsterCode(),	\
						pINFO->GetKillCount(), m_PartyMember[0].m_iKillCount );
				}
			//TRIGGER_KILL_MONSTER * pMsg = (TRIGGER_KILL_MONSTER *)pmsg;
			//if( PARTY == Get()->GetIsParty() )
			//{
			//	// ��Ƽ
			//	ASSERT( Get()->GetMonsterCode() >= 0 );
			//	if( pMsg->m_dwMonsterCode == (DWORD)Get()->GetMonsterCode() )
			//	{
			//		++m_PartyMember[0].m_iKillCount;
			//	}

			//	if( m_PartyMember[0].m_iKillCount == Get()->GetKillCount() )
			//	{
			//		SUNLOG( eFULL_LOG,  "[CONDITION_COUNT_NPCKILL::onMsg] [ID:%u]:CONDITION_COUNT_NPCKILL(MONSTER:%d)", getParent()->TriggerID(), Get()->GetMonsterCode() );
			//		setSatisfied( TRUE );
			//	}
			//}
			}
			else
			{
				ASSERT( !"���� ���δ��� NPC KILL�� ������� �ʴ´�." );
				////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//// ���� ��Ƽ�ÿ� ���͸� ���� �÷��̾� ������ ���� �� �� ����.!!!
				////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//if( Get()->GetMethod() == METHOD_GENERAL )
				//{
				//	PARTY_MEMBER * pPartyMember = m_PartyMemberHash.GetData( pMsg->m_dwObjectKey );
				//	if( pPartyMember )
				//	{
				//		// ������
				//		if( pMsg->m_dwMonsterCode == (DWORD)Get()->GetMonsterCode() )
				//		{
				//			++pPartyMember->m_iKillCount;
				//			pPartyMember->m_iKillCount = BOUND_CHECK( 0, pPartyMember->m_iKillCount, Get()->GetKillCount() );
				//		}
				//	}
				//	else
				//	{
				//		// ������
				//		int i = 0;
				//		while( i < MAX_PARTYMEMBER_NUM )
				//		{
				//			if( 0 == m_PartyMember[i].m_dwPlayerKey )
				//			{
				//				// �󽽷�
				//				m_PartyMember[i].m_dwPlayerKey = pMsg->m_dwObjectKey;
				//				if( pMsg->m_dwMonsterCode == (DWORD)Get()->GetMonsterCode() )
				//				{
				//					++m_PartyMember[i].m_iKillCount;
				//					m_PartyMember[i].m_iKillCount = BOUND_CHECK( 0, m_PartyMember[i].m_iKillCount, Get()->GetKillCount() );
				//				}
				//				m_PartyMemberHash.Add( &m_PartyMember[i], m_PartyMember[i].m_dwPlayerKey );
				//				break;
				//			}
				//			++i;
				//		}
				//	}
				//	BOOL bSuccess = TRUE;
				//	m_PartyMemberHash.SetFirst();
				//	while( ( pPartyMember = m_PartyMemberHash.GetNext() ) != NULL )
				//	{
				//		if( pPartyMember->m_iKillCount != Get()->GetKillCount() )
				//		{
				//			bSuccess = FALSE;
				//			break;	
				//		}
				//	}

				//	if(  bSuccess ) 
				//	{
				//		SUNLOG( eFULL_LOG,  "[CONDITION_COUNT_NPCKILL::onMsg] [ID:%u]:CONDITION_COUNT_NPCKILL�Ϲ�", getParent()->TriggerID() );
				//		setSatisfied( TRUE );
				//	}
				//}
				//else if( Get()->GetMethod() == METHOD_ACCUMULATE )
				//{
				//	if( pMsg->m_dwMonsterCode == (DWORD)Get()->GetMonsterCode() )
				//	{
				//		++m_PartyMember[0].m_iKillCount;
				//	}

				//	if( m_PartyMember[0].m_iKillCount >= Get()->GetKillCount()*(int)getMGR()->GetPlayerNum() )
				//	{
				//		SUNLOG( eFULL_LOG,  "[CONDITION_COUNT_NPCKILL::onMsg] [ID:%u]:CONDITION_COUNT_NPCKILL(MONSTER:%d)����", getParent()->TriggerID(), Get()->GetMonsterCode() );
				//		setSatisfied( TRUE );
				//	}
				//}
			}
			
		}
		break;
	}
}

__IMPL_POOL(CONDITION_PASS_TIME)
VOID CONDITION_PASS_TIME::OnInit()
{
	m_dwStartTick = GetTickCount();
}

BOOL CONDITION_PASS_TIME::IsSatisfied()
{
	if( m_bSatisfied ) return TRUE;

	if( m_dwStartTick + Get()->GetPassTime() <= GetTickCount() )
	{
		setSatisfied( TRUE );
	}
	
	return m_bSatisfied;
}

class AddSatisfiedPlayer
{
	Trigger * m_pTrigger;
	BOOL m_bChanged;
public:
	AddSatisfiedPlayer(Trigger * po):m_pTrigger(po),m_bChanged(FALSE){}
	~AddSatisfiedPlayer(){}

	BOOL IsChanged() { return m_bChanged;	}
	BOOL operator()( DWORD UserKey )
	{
		Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
		if( !pPlayer || !pPlayer->GetField() )
		{
			m_bChanged = TRUE;
			return FALSE;
		}
		m_pTrigger->AddSatisfiedPlayer( pPlayer );
		return TRUE;
	}
};
__IMPL_POOL(CONDITION_ENTER_AREA)
VOID CONDITION_ENTER_AREA::OnInit()
{
	getParent()->RemoveAllSatisfiedPlayer();
}
VOID CONDITION_ENTER_AREA::OnReset()
{
	getParent()->RemoveAllSatisfiedPlayer();
	setSatisfied(FALSE);
}
BOOL CONDITION_ENTER_AREA::IsSatisfied()
{
	if( m_bSatisfied ) return TRUE;

	if( getMGR()->GetPlayerNumForAreaID(Get()->GetAreaID()) == Get()->GetPlayerCount() )
	{
		// ���ǿ� �����Ѵ�.
		// �÷��̾��� ��ȿ�� üũ
		// ��ȿ���� �ʴ´ٸ�, �÷��� ����

		AddSatisfiedPlayer op(getParent());
#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
		getMGR()->ForEachPlayerInArea(Get()->GetAreaID(), op);
#else
		getMGR()->For_EachPlayerInArea(Get()->GetAreaID(), op);
#endif

		if( !op.IsChanged() )
		{
			setSatisfied( TRUE );
		}
	}

	return m_bSatisfied;
}

VOID CONDITION_ENTER_AREA::onMsg( TRIGGER_MSG * pmsg )
{
	switch( pmsg->m_dwMsgID )
	{
	case eTRIGGER_ENTER_AREA:
		{
			TRIGGER_ENTER_AREA * pMsg = (TRIGGER_ENTER_AREA *)pmsg;
			if( Get()->GetAreaID() == pMsg->m_AreaID )
			{
				IsSatisfied();
			}
		}break;
	case eTRIGGER_LEAVE_AREA:
		{
			TRIGGER_LEAVE_AREA * pMsg = (TRIGGER_LEAVE_AREA *)pmsg;
			if( Get()->GetAreaID() == pMsg->m_AreaID )
			{
				IsSatisfied();
			}
		}break;
	}
}


class HaveItemPlayer
{
	SLOTCODE	m_ItemCode;
	INT			m_Count;
public:
	HaveItemPlayer( SLOTCODE ItemCode, INT count ):m_ItemCode(ItemCode), m_Count(count) {}
	~HaveItemPlayer(){}

	//BOOL operator()( Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo )
	BOOL operator()( Player* pPlayer )
	{
		//Player * pPlayer = pPlayerInfo->m_pPlayer;
		if( m_Count == pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, m_ItemCode, m_Count ) )
		{
			return TRUE;
		}
		return FALSE;
	}
};


__IMPL_POOL(CONDITION_HAVE_ITEM)
VOID CONDITION_HAVE_ITEM::OnInit()
{
	m_dwCheckTimer = GetTickCount();
}

BOOL CONDITION_HAVE_ITEM::IsSatisfied()
{
	if( m_bSatisfied ) return TRUE;

	DWORD dwTickV = GetTickCount();
	if( m_dwCheckTimer + CONDITION_HAVE_ITEM::c_dwDelayCheckTime <= dwTickV )
	{
		m_dwCheckTimer = dwTickV;

		// ��������� ��찡 ������ ������ ���� ����� Ŭ���� ����� ����
		// �� �� �ϳ�
		if( getParent()->GetClickedUserKey() != 0 )
		{
			Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( getParent()->GetClickedUserKey() );
			if( NULL != pPlayer )
			{
				if( Get()->GetItemNum() == pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, Get()->GetItemCode(), Get()->GetItemNum() ) )
				{
					setSatisfied( TRUE );
					return TRUE;
				}
			}
			//HaveItemPlayer op( Get()->GetItemCode() );
			//getParent()->For_EachSatisfiedPlayer(
			//getMGR()->For_EachPlayerInArea(
			//if( getParent()->FindPlayer( op ) )
			//{
			//	setSatisfied( TRUE );
			//}
		}
		else
		{
			HaveItemPlayer op( Get()->GetItemCode(), Get()->GetItemNum() );
			getParent()->For_EachSatisfiedPlayer(op);
			setSatisfied( TRUE );
			//getParent()->RemoveAllSatisfiedPlayer();
		}
	}

	return m_bSatisfied;
}


__IMPL_POOL(CONDITION_MULTI_SWITCH)
BOOL CONDITION_MULTI_SWITCH::IsSatisfied()
{
	if( m_bSatisfied ) return TRUE;

	int satisfiedNum = 0;
	for( BYTE idx = 0 ; idx < CONDITION_MULTI_SWITCH_INFO::_MAX_SWITCH_ID_NUM ; ++idx )
	{
		if( INVALID_SWITCH_ID_VALUE == Get()->GetSwitchID( idx ) ) continue;

		BYTE value = getMGR()->GetSwitchValue( Get()->GetSwitchID( idx ) );
		sSWITCH( Get()->GetOperationType1() )
		CASE(eCOMPARE_EQUAL)
			if( value == Get()->GetValue1() ) ++satisfiedNum;
		CASE(eCOMPARE_MORE)
			if( value > Get()->GetValue1() ) ++satisfiedNum;
		CASE(eCOMPARE_LESS)
			if( value < Get()->GetValue1() ) ++satisfiedNum;
		CASE(eCOMPARE_LESSTHAN)
			if( value <= Get()->GetValue1() ) ++satisfiedNum;
		CASE(eCOMPARE_MORETHAN)
			if( value >= Get()->GetValue1() ) ++satisfiedNum;
		CASE(eCOMPARE_NOTEQUAL)
			if( value != Get()->GetValue1() ) ++satisfiedNum;
		DEFAULT
		eSWITCH
	}
	
	sSWITCH( Get()->GetOperationType2() )
	CASE(eCOMPARE_EQUAL)
		if( satisfiedNum == Get()->GetValue2() ) setSatisfied( TRUE );
	CASE(eCOMPARE_MORE)
		if( satisfiedNum > Get()->GetValue2() ) setSatisfied( TRUE );
	CASE(eCOMPARE_LESS)
		if( satisfiedNum < Get()->GetValue2() ) setSatisfied( TRUE );
	CASE(eCOMPARE_LESSTHAN)
		if( satisfiedNum <= Get()->GetValue2() ) setSatisfied( TRUE );
	CASE(eCOMPARE_MORETHAN)
		if( satisfiedNum >= Get()->GetValue2() ) setSatisfied( TRUE );
	CASE(eCOMPARE_NOTEQUAL)
		if( satisfiedNum != Get()->GetValue2() ) setSatisfied( TRUE );
	DEFAULT
	eSWITCH
	
	DEBUG_CODE( if(m_bSatisfied) SUNLOG( eFULL_LOG,  "[CONDITION_MULTI_SWITCH::IsSatisfied] [ID:%u]:TriggerMultiSwitchCondition", getParent()->TriggerID() ); );
	
	return m_bSatisfied;
}




__IMPL_POOL(CONDITION_MISSION_POINT)
VOID CONDITION_MISSION_POINT::OnInit()
{
	//m_iNumberOfPlayersInArea = 0;
}

VOID CONDITION_MISSION_POINT::onMsg( TRIGGER_MSG * pmsg )
{
	if( eTRIGGER_ENTER_AREA == pmsg->m_dwMsgID )
	{
		TRIGGER_ENTER_AREA* pMsg = (TRIGGER_ENTER_AREA*)pmsg;

		// ���� �÷��̾ �� ������ �ִ°�?
		Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwUserKey );
		if( !pPlayer )				return;
		if( !pPlayer->GetField() )	return;

		INT iMapCode;

		do
		{
			GameZone* pGameZone = pPlayer->GetGameZonePtr();
			if( pGameZone )
				iMapCode = (INT)pGameZone->GetMapCode();
		} while(0);

		if( SAFE_NUMERIC_TYPECAST(INT, iMapCode, BYTE) )
		{
			INT iMissionPoint = pPlayer->GetMissionManager()->GetMissionPoint( (BYTE)iMapCode );

			sSWITCH( Get()->GetFlag() )
			CASE(eCOMPARE_EQUAL)
				if( iMissionPoint == Get()->GetMissionPoint() ) setSatisfied( TRUE );
			CASE(eCOMPARE_MORE)
				if( iMissionPoint > Get()->GetMissionPoint() ) setSatisfied( TRUE );
			CASE(eCOMPARE_LESS)
				if( iMissionPoint < Get()->GetMissionPoint() ) setSatisfied( TRUE );
			DEFAULT
			eSWITCH
		}			
	}

	/*
	switch( pmsg->m_dwMsgID )
	{
	case eTRIGGER_ENTER_AREA:
		{
			TRIGGER_ENTER_AREA * pMsg = (TRIGGER_ENTER_AREA *)pmsg;

			//�̿����� �÷��̾ �ִ��� üũ
			if( Get()->GetAreaID() == pMsg->m_AreaID )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayer( pMsg->m_dwUserKey );
				WzVector vPlayerPos;
				pPlayer->GetPos( &vPlayerPos );

				WzSpecialArea * pAreaInfo = getMGR()->GetAreaInfo( Get()->GetAreaID() );
				ASSERT( pAreaInfo );
				if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_AABB )
				{
					BvAABB & aabb = pAreaInfo->m_wbvRegion.m_aabb;
					//if( aabb.m_wvMin.x <= vPlayerPos.x &&  vPlayerPos.x <= aabb.m_wvMax.x )
					{
						//if( aabb.m_wvMin.y <= vPlayerPos.y &&  vPlayerPos.y <= aabb.m_wvMax.y )
						{
							if( pPlayer->GetMissionManager()->GetMissionPoint(0) >= Get()->GetMissionPoint() )
							{
								Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo = getParent()->GetConditionPlayer( pPlayer->GetUserKey() );
								if( pPlayerInfo ) 
									pPlayerInfo->m_State |= Trigger::ENTER_AREA_POINT_PLAYER;
								else
									getParent()->AddConditionPlayer( pPlayer, Trigger::ENTER_AREA_POINT_PLAYER );

								SUNLOG( eFULL_LOG,  "[ID:%u]:CONDITION_MISSION_POINT", getParent()->TriggerID() );
								setSatisfied( TRUE );
							}
						}
					}
				}
				else if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
				{
					INT iPlayerTileIdx = getMGR()->GetTileToStand( &vPlayerPos );
					if( -1 == iPlayerTileIdx )
					{
						SUNLOG( eFULL_LOG,  "��ġ������ �߸��Ǿ���.-1" );
						return ;
					}

					BvPathTileIndex * pTile = &pAreaInfo->m_wbvRegion.m_BvPathTileIndex;
					// TEMP // !!!!! ����ȭ �ȸ¾Ƽ� �켱 üũ ����
					//for(WORD idx = 0 ; idx < pTile->m_nCount ; ++idx )
					{
						//if( pTile->m_pdwData[idx] == iPlayerTileIdx )
						{
							// �̼�����Ʈ�� �� �̻��϶�
							if( pPlayer->GetMissionManager()->GetMissionPoint(0) >= Get()->GetMissionPoint() )
							{
								Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo = getParent()->GetConditionPlayer( pPlayer->GetUserKey() );
								if( pPlayerInfo ) 
									pPlayerInfo->m_State |= Trigger::ENTER_AREA_POINT_PLAYER;
								else
									getParent()->AddConditionPlayer( pPlayer, Trigger::ENTER_AREA_POINT_PLAYER );
								
								SUNLOG( eFULL_LOG,  "[ID:%u]:CONDITION_MISSION_POINT", getParent()->TriggerID() );
								setSatisfied( TRUE );
							}
							
							break;
						}
					}
				}
			}
		}
		break;
	case eTRIGGER_LEAVE_AREA:
		{
			TRIGGER_LEAVE_AREA * pMsg = (TRIGGER_LEAVE_AREA *)pmsg;

			if( Get()->GetAreaID() == pMsg->m_AreaID )
			{
				Player * pPlayer = PlayerManager::Instance()->FindPlayer( pMsg->m_dwUserKey );
				WzVector vPlayerPos;
				pPlayer->GetPos( &vPlayerPos );

				WzSpecialArea * pAreaInfo = getMGR()->GetAreaInfo( Get()->GetAreaID() );
				ASSERT( pAreaInfo );
				if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_AABB )
				{
					BvAABB & aabb = pAreaInfo->m_wbvRegion.m_aabb;
					//if( aabb.m_wvMin.x <= vPlayerPos.x &&  vPlayerPos.x <= aabb.m_wvMax.x )
					{
						//if( aabb.m_wvMin.y <= vPlayerPos.y &&  vPlayerPos.y <= aabb.m_wvMax.y )
						{
							Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo = getParent()->GetConditionPlayer( pMsg->m_dwUserKey );
							if( pPlayerInfo->m_State == Trigger::ENTER_AREA_POINT_PLAYER )
							{
								getParent()->RemoveConditionPlayer( pMsg->m_dwUserKey );
							}
							else
							{
								pPlayerInfo->m_State &= ~Trigger::ENTER_AREA_POINT_PLAYER;
							}

							if( IsSatisfied() )
								setSatisfied( FALSE );							
						}
					}
				}
				else if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
				{
					INT iPlayerTileIdx = getMGR()->GetTileToStand( &vPlayerPos );
					if( -1 == iPlayerTileIdx )
					{
						SUNLOG( eFULL_LOG,  "��ġ������ �߸��Ǿ���.-1" );
						return ;
					}

					BvPathTileIndex * pTile = &pAreaInfo->m_wbvRegion.m_BvPathTileIndex;
					// TEMP // !!!!! ����ȭ �ȸ¾Ƽ� �켱 üũ ����
					//for(WORD idx = 0 ; idx < pTile->m_nCount ; ++idx )
					{
						//if( pTile->m_pdwData[idx] == iPlayerTileIdx )
						{
							Trigger::PLAYER_TRIGGER_INFO * pPlayerInfo = getParent()->GetConditionPlayer( pMsg->m_dwUserKey );
							if( pPlayerInfo->m_State == Trigger::ENTER_AREA_POINT_PLAYER )
							{
								getParent()->RemoveConditionPlayer( pMsg->m_dwUserKey );
							}
							else
							{
								pPlayerInfo->m_State &= ~Trigger::ENTER_AREA_POINT_PLAYER;
							}

							if( IsSatisfied() )
								setSatisfied( FALSE );
							break;
						}
					}
				}

				
			}
		}
		break;
	}
	*/
}

__IMPL_POOL(CONDITION_COMPARE_VARIABLE)

BOOL	CONDITION_COMPARE_VARIABLE::IsSatisfied()
{
	return FALSE;
}

#include <QuestInfo.h>
#include <QuestManager.h>

class QuestProgress
{
	BOOL&	m_bOrRet;
	QCODE	m_wQuestCode;
	CONDITION_QUEST_PROGRESS_INFO::ePROGRESS m_eCond;
public:
	QuestProgress( BOOL& bOrRet, QCODE wQuestCode, CONDITION_QUEST_PROGRESS_INFO::ePROGRESS iCond )
		: m_bOrRet(bOrRet), m_wQuestCode(wQuestCode), m_eCond(iCond) {}

	VOID operator() ( Player* pPlayer )
	{
		QuestManager* pQuestMan = (QuestManager*)pPlayer->GetQuestManager();
		Quest* pQuest = NULL;

		switch( m_eCond )
		{
		case CONDITION_QUEST_PROGRESS_INFO::ePROGRESS::IN_PROGRESS: /*��������*/ 
			if( pQuestMan->FindQuest( m_wQuestCode ) )
				m_bOrRet = TRUE;
			break;
		case CONDITION_QUEST_PROGRESS_INFO::ePROGRESS::COMPLETED: /*�Ϸ���*/ 
			if( pQuestMan->FindComplQuest( m_wQuestCode ) )
				m_bOrRet = TRUE;
			break;
		};
	}
};

__IMPL_POOL(CONDITION_QUEST_PROGRESS)
BOOL	CONDITION_QUEST_PROGRESS::IsSatisfied()
{
	if( m_bSatisfied )
		return TRUE;

	const CONDITION_QUEST_PROGRESS_INFO* pINFO = (const CONDITION_QUEST_PROGRESS_INFO*)Get();

	ASSERT( SAFE_NUMERIC_TYPECAST(ULONG, pINFO->GetQuestID(), QCODE) );

	BOOL bOrRet = FALSE;
	QuestProgress op( bOrRet, (QCODE)pINFO->GetQuestID(), (CONDITION_QUEST_PROGRESS_INFO::ePROGRESS)pINFO->GetProgress() );

	if( getParent()->GetClickedUserKey() != 0 )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( getParent()->GetClickedUserKey() );
		if( NULL != pPlayer )
		{
			op( pPlayer );
			if( bOrRet )
				setSatisfied( TRUE );
		}
	}
	else
	{
		//if( getParent()->ForEach_SatisfiedPlayer_Need_OR_Result( op ) )
		//	setSatisfied( TRUE );
		//SUNLOG( eCRITICAL_LOG, "CONDITION_QUEST_PROGRESS not Implemented" );

		getParent()->For_EachSatisfiedPlayer( op );
		if( bOrRet )
			setSatisfied( TRUE );
	}

	return m_bSatisfied;
}

class CheckClassID
{
	BOOL&		m_bOrRet;
	eCHAR_TYPE	m_CharType;
public:
	CheckClassID( BOOL& bOrRet, eCHAR_TYPE eCharType ) : m_bOrRet(bOrRet), m_CharType(eCharType) {}

	VOID operator() ( Player* pPlayer )
	{
		if( pPlayer->GetCharType() == m_CharType )
			m_bOrRet = TRUE;
	}
};

__IMPL_POOL(CONDITION_CHECK_CLASS_ID)
BOOL	CONDITION_CHECK_CLASS_ID::IsSatisfied()
{
	if( m_bSatisfied )
		return TRUE;

	const CONDITION_CHECK_CLASS_ID_INFO* pINFO = (const CONDITION_CHECK_CLASS_ID_INFO*)Get();

	if( getParent()->GetClickedUserKey() != 0 )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( getParent()->GetClickedUserKey() );
		if( NULL != pPlayer )
		{
			// <Waverix, 060606>
			// �ϴ��� �������� �־��� Class ID�� eCHAR_TYPE�� ��ġ�Ѵ�. ���ĸ� ���� ��ȹ�ʿ� �̰��� ������
			// ������ ��û�� ��.
			eCHAR_TYPE eClass = (eCHAR_TYPE)pINFO->GetClassID();
			if( pPlayer->GetCharType() == eClass )
				setSatisfied( TRUE );
		}
	}
	else
	{	// ��Ƽ�� ���Ե��� �ʴ´�.
		BOOL bOrRet = FALSE;
		CheckClassID op( bOrRet, (eCHAR_TYPE)pINFO->GetClassID() );
		getParent()->For_EachSatisfiedPlayer( op );
		if( bOrRet )
			setSatisfied( TRUE );
	}

	return m_bSatisfied;
}

#ifdef __NA_000328_UNIT_DESTROY_TRIGGER
__IMPL_POOL(CONDITION_NPCKILL_FOR_UNITID)
VOID CONDITION_NPCKILL_FOR_UNITID::onMsg( TRIGGER_MSG * pmsg )
{
	switch( pmsg->m_dwMsgID )
	{
	case eTRIGGER_KILL_MONSTER:
		{
			TRIGGER_KILL_MONSTER* pMsg = (TRIGGER_KILL_MONSTER *)pmsg;
			CONDITION_NPCKILL_FOR_UNITID_INFO* pINFO = Get();

			if( pMsg->m_MonsterUnitID != pINFO->GetMonsterUnitID() )
			{
				return;
			}
			//------------------------
			// ���� ����
			setSatisfied( TRUE );
			SUNLOG( eFULL_LOG, "[CONDITION_NPCKILL_FOR_UNITID::onMsg] [ID:%u]:MONSTER:[%u]", getParent()->TriggerID(), pINFO->GetMonsterUnitID() );
			//------------------------
		}
		break;
	}
}
#endif // __NA_000328_UNIT_DESTROY_TRIGGER

