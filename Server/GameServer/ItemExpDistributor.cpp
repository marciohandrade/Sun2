#include "stdafx.h"

#include "ItemExpDistributor.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#include <PublicMath.h>
#include "ServerOptionParser.h"
#include <ServerOptionParserEx.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#include "CharacterFormula.h"
#include "BattleRecord.h"
#include "PeakTimeEvent.h"

#include "Map.h"
#include "GameZone.h"

#include "GameRoom.h"

#include "GameParty/NewGameParty.h"
#include "GameParty/NewGamePartyManager.h"

#include "TargetManager.h"
#include "FatigueManager.h"

#include "Item.h"
#include "ItemManager.h"
#include "Player.h"
#include "./SkillSystem/PerkManager.h" // _NA002217_100728_EP2_PERK_SYSTEM_

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
ItemExpDistributor g_ItemExpDistributor;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	소유권 결정( 경험치, 아이템 )
//
class GetOwnership
{
	struct PartyOwnerPoint
	{
		BYTE	m_byOwnerPoint;
		DWORD	m_TotDamage;
	};

	typedef STLX_MAP<WORD, PartyOwnerPoint>		PARTYPOINT_MAP;			// < PartyKey, PartyOwnerPoint >
	typedef PARTYPOINT_MAP::iterator			PARTYPOINT_MAP_ITER;
	typedef std::pair<WORD, PartyOwnerPoint>	PARTYPOINT_MAP_PAIR;

public:		GetOwnership( GameField* pField, DWORD dwMaxHP )
				: m_pField(pField), m_dwMaxHP(dwMaxHP), m_dwHighOwnerObjKey(0), m_byHighOwnerPoint(0) {}
			~GetOwnership() {}

			VOID	operator () ( BattleRecord::Node* pRecord )
			{
				dwObjKey = pRecord->GetObjectKey();
				Character *pAttacker = m_pField->FindCharacter( dwObjKey );
				if( !pAttacker )	return;

				WORD wPartyKey = pAttacker->GetPartyState().GetPartyKey();

				// pAttacker가 소환체인 경우 그 주인을 찾아서 파티키를 셋팅해야한다.
				if(pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
				{
					Character *pSummoner = m_pField->FindCharacter( pAttacker->GetSummonerKey() );
					if( pSummoner )
					{
						wPartyKey = pSummoner->GetPartyState().GetPartyKey();
						dwObjKey = pSummoner->GetObjectKey();
					}
				}

				// 소유권 포인트를 구한다.
				if( wPartyKey )
				{
					// 파티원인 경우
					// 한 몬스터에 대해서 2개 이상의 파티가 경쟁할 수 있다. 그래서 파티포인트를 맵으로 관리한다.
					PARTYPOINT_MAP_ITER it = m_mapPartyPoint.find( wPartyKey );
					if( it == m_mapPartyPoint.end() )	
					{
						//현재 파티키에 해당하는 파티포인트가 아직 없다면...
						stOwnerPoint.m_TotDamage = pRecord->GetTotDamage();
						stOwnerPoint.m_byOwnerPoint = (BYTE)( 70 * pRecord->GetTotDamage() / m_dwMaxHP );
						if( pRecord->IsFirstAttacker() )  stOwnerPoint.m_byOwnerPoint += 30;

						m_mapPartyPoint.insert( PARTYPOINT_MAP_PAIR(wPartyKey, stOwnerPoint) );

						byOwnerPoint = stOwnerPoint.m_byOwnerPoint;
					}
					else
					{
						//파티키에 해당하는 파티포인트가 존재한다면...
						PartyOwnerPoint &PartyPoint = it->second;
						PartyPoint.m_TotDamage += pRecord->GetTotDamage();
						PartyPoint.m_byOwnerPoint = (BYTE)( 70 * PartyPoint.m_TotDamage / m_dwMaxHP );
						if( pRecord->IsFirstAttacker() )  PartyPoint.m_byOwnerPoint += 30;

						byOwnerPoint = PartyPoint.m_byOwnerPoint;
					}
				}
				else
				{
					// 파티원이 아닌 경우
					byOwnerPoint = (BYTE)( 70 * pRecord->GetTotDamage() / m_dwMaxHP );
					if( pRecord->IsFirstAttacker() )  byOwnerPoint += 30;
				}

				// 최대 소유포인트를 갱신한다.
				if( byOwnerPoint >= m_byHighOwnerPoint )
				{
					m_byHighOwnerPoint = byOwnerPoint;
					m_dwHighOwnerObjKey = dwObjKey;
				}
			}

			DWORD	GetOwnerObjKey()	{ return m_dwHighOwnerObjKey; }


private:	PARTYPOINT_MAP		m_mapPartyPoint;
			GameField *			m_pField;
			DWORD				m_dwMaxHP;

			DWORD				m_dwHighOwnerObjKey;
			BYTE				m_byHighOwnerPoint;

			// Local 변수
			BYTE				byOwnerPoint;
			PartyOwnerPoint		stOwnerPoint;
			DWORD				dwObjKey;
};


//────────────────────────────────────────────────────────────────


Character*
ItemExpDistributor::DecideExpOwnership( NPC *pNPC )
{
	GameField *pField = pNPC->GetField();
	if( !pField )	return NULL;

	BattleRecord *pBattleRecord = pNPC->GetTargetManager()->GetBattleRecord();
	if( !pBattleRecord->GetSize() )	
		return NULL; 

	GetOwnership Opr( pField, pNPC->GetMaxHP() );
	pBattleRecord->ForEachRecord( Opr );

	return pField->FindCharacter( Opr.GetOwnerObjKey() );
}


//────────────────────────────────────────────────────────────────

//	NPC는 죽은 몬스터의 객체이다.
Player*
ItemExpDistributor::DecideItemOwnership( NPC *pNPC )
{
	Character* pOwner = NULL;

	pOwner = DecideExpOwnership( pNPC );
	if( !pOwner )	return NULL;

	if(pOwner->IsEqualObjectKind(PLAYER_OBJECT))
	{
		return (Player*)pOwner;
	}
	
	return NULL;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	경험치 분배
//

//────────────────────────────────────────────────────────────────
//	경험치 분배 루틴 수정 - pNPC Pointer를 사용하지 않고도 처리하는 방법으로 변경
class DistributeExpOpr : public std::unary_function<Player*, EXPTYPE>
{
			//	Process Options
public:		enum ePolicy {
					DEO_DISABLE_ALL					= 0,
					DEO_CHECK_ZONE_INTEGRITY		= 1<< 0,
					DEO_CHECK_PARTY_RETURN_ABUSE	= 1<< 1,
					DEO_APPLY_ADD_EXP				= 1<< 2,
			};
			//	Command Options
public:		struct Option_Default;
			struct Option_GetExp;

private:
	const	ePolicy						Policy;
	const	DistributeExpOprArgs&		rArgs;
	const	SERVER_COMMON_OPTION&		rServerOption;
			PeakTimeEvent* const		m_pPeakTimeEvent;
			DistributeExpOprResult*		m_pOutputResult;

public:		DistributeExpOpr(
				const ePolicy					policy
				, const DistributeExpOprArgs&	_rArgs
				, DistributeExpOprResult* OUT	_pOutputResult)
				: Policy(policy)
				, rArgs(_rArgs)
				, rServerOption( ServerOptionParserEx::Instance()->GetServerOption() )
				, m_pPeakTimeEvent( PeakTimeEvent::Instance() )
				, m_pOutputResult( _pOutputResult )
			{
			}

			BOOLEAN		operator() ( Player* pPlayer ) const;
};


struct DistributeExpOpr::Option_Default : public DistributeExpOpr
{
	Option_Default( const DistributeExpOprArgs& _rArgs )
		: DistributeExpOpr( (ePolicy)
			( DEO_CHECK_ZONE_INTEGRITY
			| DEO_CHECK_PARTY_RETURN_ABUSE
			| DEO_APPLY_ADD_EXP
			)
			, _rArgs
			, NULL
		)
	{}
};
//struct DistributeExpOpr::Option_GetExp : public DistributeExpOpr
//{
//	Option_GetExp( const DistributeExpOprArgs& _rArgs )
//		: DistributeExpOpr( _rArgs )
//	{}
//};


BOOLEAN
DistributeExpOpr::operator() ( Player* pPlayer ) const
{
	GameZone* const pZone = pPlayer->GetGameZonePtr();
	if( Policy & DEO_CHECK_ZONE_INTEGRITY )
	{
		if( FALSE == ( pZone && (rArgs.pGameZone==pZone) && pPlayer->IsAlive() ) )
			return 0;

		//	Skip Logic
		//if( ( pPlayer->GetObjectType() & PLAYER_OBJECT ) != PLAYER_OBJECT )	return;

		if( rArgs.radius && ( Math_VectorLength2( *rArgs.pwvNpcPos, *pPlayer->GetPosPtr() ) > rArgs.fRadius_Q2 ) )
			return 0;
	}

	if( Policy & DEO_CHECK_PARTY_RETURN_ABUSE )	//__NA_780_PREVENTION_PARTY_RETURN_ABUSE
	{
		GameField* const pFieldOfPlayer = pPlayer->GetField();
		if( !rArgs.pGameField || (rArgs.pGameField != pFieldOfPlayer) )
			return 0;
	}

	const LEVELTYPE player_level = pPlayer->GetLevel();

	EXPTYPE resultExp = GetKillExpOfParty(
								rArgs.roomBonusType
								, rArgs.memberNum
								, rArgs.totalLevel
								, player_level
								, rArgs.npcLevel
								, rArgs.fNpcExpRatio
								, rArgs.npcMaxHP	// attack_damage
								, rArgs.npcMaxHP	// max HP
								, rArgs.zoneType
								, pPlayer
								);
    //
    EXPTYPE groupEXP = resultExp; //보너스 경험치를 제외한 순수 경험치 임시 저장( 그룹 보너스를 위해 )

	double benefit	= 1.0f;

	double var_for_debug = m_pPeakTimeEvent->GetEventExp( pPlayer );	// Peak Time 이벤트 경험치를 더 준다.

    //_KR_0_20091021_FIRENDINVITATION_EXTEND // 친구 추천 이벤트 경험치를 가산한다.
    var_for_debug += pPlayer->GetInvitateFriend().GetAddExpRate(rArgs.radius, rArgs.pwvNpcPos, rArgs.fRadius_Q2);

	benefit += var_for_debug;
	var_for_debug = pZone->GetBonusExp( pPlayer );	// 슈퍼방장 방인지 체크
	benefit += var_for_debug;
	var_for_debug = pPlayer->GetBonusExpRatio();	// 경험치 증가 속성이 있는지 체크
	benefit += var_for_debug;

	//	PC방 유저이면 경험치 10%를 더 준다.(PC방 요금미납자가 아닌지 체크)
	if( pPlayer->CanApplyPCBangBenefit( FALSE ) )
	{	//	(WAVERIX)(080912)(THINKING)
		//	- Singleton에 익숙하고 각종 옵션 구조를 직접 참조하는 것이 당연하게 생각하는 지금 m_fPCBANG_BONUS_EXP_RATIO같은 것을
		//	별도로 추출하면 대판 욕먹을 수 있다.
		var_for_debug = ( 1.0f + rServerOption.m_fPCBANG_BONUS_EXP_RATIO );
		benefit *= var_for_debug;
	}

    // changed rule by _NA_002471_20110408_EXP_BENEFIT_UP
    // 1101부터 경험치 보너스 최대값 제한 상향
    if (benefit > 10.0f) {
        benefit = 10.0f;
    }
    // (WAVERIX)(080912)(REPLACEMENT PROPOSAL)
    // - 계산 상의 미스가 생기는 지는 정밀하게 테스트는 되지 않았지만, (뭐에서 문제가 생기는지 모름)
    // 해당 Rounded는 반올림을 하는데 있어서 적어도 H/W 환경, compile option 또는 debug|release에서 항상 동일한 결과를 보일 것이다.

    // (JSCHO)(Comment) - 피로도 시스템(플레이가 12시간이 넘으면 경험치 없음)
    const int fatigue_exp_ratio = pPlayer->GetFatigueManager()->GetExpRatio();
    const int nTotalBonusRatio  = util::LL_Ceil(benefit * 100.f - 100.f);
    const int nTotalBenefit     = util::LL_Ceil(benefit * fatigue_exp_ratio);
    resultExp = (EXPTYPE)util::LL_Ceil(double(resultExp) * double(nTotalBenefit) / 100.0f);

    if (Policy & DEO_APPLY_ADD_EXP) {
        pPlayer->AddExp(resultExp, rArgs.npc_key, nTotalBonusRatio);
    #ifdef _NA002217_100728_EP2_PERK_SYSTEM_
        // add only battle experience value
        pPlayer->GetPerkManager()->AddExp(resultExp);
    #endif
    };

    // __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
    if (pPlayer->GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW)
    {
        pPlayer->GroupBonusAcceptForEXP( groupEXP, resultExp, eEXP_ACQUISITION_HUNTING);
    }
    #else
    if (pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_OFFLINE_HIGH ||
        pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_ONLINE_HIGH)
    {
        pPlayer->GroupBonusAcceptForEXP( groupEXP, resultExp, eEXP_ACQUISITION_HUNTING );
    }
    #endif

	if( m_pOutputResult )
	{
		m_pOutputResult->TotalExp	= resultExp;
		m_pOutputResult->BonusExp	= nTotalBonusRatio;
	}

	//	(WAVERIX)(080912)(BUG-FIX) double, EXPTYPE, 등등 타입 정보가 메타문자열 포맷과 다르다.
    /*bool show_log = true;
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_distributed_exp", &show_log)));
    if (show_log)
    {
	SUNLOG( eDEV_LOG,
		"[DistributeExpOpr] Name = %s, dbBenefit = %0.3e, ExpRatio = %d, nTotalBenefit = %d,"
		" ResultExp = %I64d, nTotalBonusRatio = %d"
		, pPlayer->GetCharName(), benefit, fatigue_exp_ratio, nTotalBenefit, resultExp, nTotalBonusRatio );
    }*/
	//	(WAVERIX)(080912)(PERFORMANCE MONITORING REPORT)
	//	- 몬스터가 제거된 시점의 성능 측정 결과는 상당히 엉망이다.
	//	- 연관된 이벤트인 스킬, 데미지 처리등은 극악이다. (이 부분은 언급하기조차 싫다)

	return TRUE;
}


//────────────────────────────────────────────────────────────────


//────────────────────────────────────────────────────────────────
//
BOOLEAN
ItemExpDistributor::GetCalcedExpValue(Player* const pPlayer,
                                      const DistributeExpOprArgs* const pArgs,
                                      DistributeExpOprResult* OUT pResult)
{
    if(!(pArgs && pResult))
        return FALSE;

    ZeroMemory(pResult, sizeof(*pResult));

    //  경험치분배 객체 생성
    DistributeExpOpr actor(DistributeExpOpr::DEO_DISABLE_ALL, *pArgs, pResult);
    return actor(pPlayer);
}



//────────────────────────────────────────────────────────────────
//
VOID
ItemExpDistributor::DistributeExp( NPC* pNPC, Character* pExpOwner )
{
	// 변수 셋팅
	GameZone* pZone = pNPC->GetGameZonePtr();
	if( !pZone ) return;
    if(pExpOwner->IsEqualObjectKind(PLAYER_OBJECT) == 0)
		return;

	const eZONETYPE zone_type = pZone->GetZoneType();
	BYTE byRoomBonusType = 0;	 BYTE byRadius = 0;

	// 헌팅맵의 경우
	if( zone_type == eZONETYPE_HUNTING )
	{
		GameRoom * pGameRoom = (GameRoom *)pZone;
		byRoomBonusType = pGameRoom->GetAdditionalInfo()->m_Bonus;
		byRadius = 60;
	}
	else
	{
		byRadius = 60;
	}

	WORD PartyKey = pExpOwner->GetPartyState().GetPartyKey();

	WORD wMemberNum = 1;
	LEVELTYPE TotalLevel = pExpOwner->GetLevel();

	NewGamePartyManager::Instance()->GetAreaLivePartyInfo( PartyKey, pZone->GetKey(), pNPC->GetPosPtr(), byRadius, wMemberNum, TotalLevel );

	DistributeExpOprArgs args(
		pZone
		, pNPC->GetField()
		, zone_type
		, byRoomBonusType
		, byRadius
		, wMemberNum
		, TotalLevel
		, (FLOAT)byRadius*byRadius
		, pNPC->GetObjectKey()
		, pNPC->GetPosPtr()
		, pNPC->GetGradeExpRatio()
		, pNPC->GetMaxHP()
		, pNPC->GetDisplayLevel());

	//	경험치분배 객체 생성
	DistributeExpOpr::Option_Default Opr( args );

	if( !NewGamePartyManager::Instance()->For_eachPlayers( PartyKey, Opr ) )
	{
        if(pExpOwner->IsEqualObjectKind(PLAYER_OBJECT))
			Opr( (Player*)pExpOwner );
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	아이템 분배
//
template<class PACKET_TYPE>
class DistributeItemOpr : public std::unary_function<Player *, void>
{
public:
	DistributeItemOpr( Player *pOwner, Item *pItem, KEYTYPE ZoneKey, BYTE byRadius, PACKET_TYPE & packet ) : m_pOwner(pOwner), 
						m_pItem(pItem), m_ZoneKey(ZoneKey), m_byRadius(byRadius), m_Packet(packet) {}
	~DistributeItemOpr() {}

	VOID operator () ( Player *pMember ) const
	{
		GameZone *pZone = pMember->GetGameZonePtr();
		if( !pZone )						return;
		if( m_ZoneKey != pZone->GetKey() )	return;
		if( pMember->IsDead() )				return;
		if( pMember->GetObjectKey() == m_pOwner->GetObjectKey() )	return;

		// 피로도 시스템
		BYTE byPickRatio = pMember->GetFatigueManager()->GetShareItemPickupRatio();
		if( random(1, 100) > byPickRatio )	return;

		if( m_byRadius != 0 && Math_VectorLength( *m_pOwner->GetPosPtr(), *pMember->GetPosPtr() ) > m_byRadius )	return;

		GameField* pOwnerField = m_pOwner->GetField();
		GameField* pMemberField	= pMember->GetField();
		if( !pOwnerField || !pMemberField )
			return;
		if( pOwnerField->GetFieldCode() != pMemberField->GetFieldCode() )
			return;

		if( m_pItem )
		{
			SCItemSlot ItemSlot = m_pItem->GetItemSlot();
			if( RC::RC_ITEM_SUCCESS == pMember->GetItemManager()->Pick( 1, ItemSlot, &m_Packet.m_ItemInfo ) )
				pMember->SendPacket( &m_Packet, m_Packet.GetSize() );
		}
	}

private:
	Player *					m_pOwner;
	Item *						m_pItem;
	KEYTYPE						m_ZoneKey;
	BYTE						m_byRadius;
	// Local 변수
	//WzVector					vOwnerPos;
	//WzVector					vMemberPos;
	PACKET_TYPE	&				m_Packet;
    //
    __DISABLE_COPY(DistributeItemOpr<PACKET_TYPE>);
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	하임 분배
//
template<class PACKET_TYPE>
class DistributeMoneyOpr : public std::unary_function<Player *, void>
{
public:
	DistributeMoneyOpr( Player *pOwner, MONEY Money, KEYTYPE ZoneKey, BYTE byRadius, PACKET_TYPE & packet ) : 
	  m_pOwner(pOwner), m_Money(Money), m_ZoneKey(ZoneKey), m_byRadius(byRadius),m_Packet(packet) {}
	~DistributeMoneyOpr() {}

	VOID operator () ( Player *pMember ) const
	{
		GameZone *pZone = pMember->GetGameZonePtr();
		if( !pZone )						return;
		if( m_ZoneKey != pZone->GetKey() )	return;
		if( pMember->IsDead() )				return;
		if( pMember->GetObjectKey() == m_pOwner->GetObjectKey() )	return;

		// 피로도 시스템
		BYTE byPickRatio = pMember->GetFatigueManager()->GetShareItemPickupRatio();
#ifndef __NA_000632_20070817_CHINA_FATIGUE_HEIM
		if( random(1, 100) > byPickRatio )	return;
#endif

		if( m_byRadius != 0 && Math_VectorLength( *m_pOwner->GetPosPtr(), *pMember->GetPosPtr() ) > m_byRadius )	return;

		GameField* pOwnerField = m_pOwner->GetField();
		GameField* pMemberField	= pMember->GetField();
		if( !pOwnerField || !pMemberField )
			return;
		if( pOwnerField->GetFieldCode() != pMemberField->GetFieldCode() )
			return;

		MONEY DropMoney = m_Money;
		// 하임증가 아이템이 있는지 체크
		if( pMember->GetBonusMoneyRatio() )
		{
			DropMoney = (MONEY)(ceil)( DropMoney * ( 1.f + pMember->GetBonusMoneyRatio() ) );
		}

#ifdef __NA_000632_20070817_CHINA_FATIGUE_HEIM
		DropMoney = ( DropMoney / (float)(100.0/byPickRatio) );
#endif

		pMember->PlusMoney( DropMoney );
		m_Packet.m_Money = pMember->GetMoney();
		m_Packet.m_BonusRatio = (WORD) ceil( pMember->GetBonusMoneyRatio() * 100 );
		pMember->SendPacket( &m_Packet, sizeof(m_Packet) );

		GAMELOG->LogMoney( MONEY_PICK, pMember, DropMoney );
        if( pMember->GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW ) //한명의 그룹원이라도 온라인이라면
        {
            //현재 그룹 머니는 천분율 기준 30으로 설정 되어있으나 스크립트 파서 적용 이후로 추후 변경
            g_pGameServer->NotifyHeimAcquisition(pMember->GetCharGuid(), eHEIM_ACQUISITION_PICKUP, MONEY(DropMoney * 0.03f));
        }
    }

private:
	Player *					m_pOwner;
	MONEY						m_Money;
	KEYTYPE						m_ZoneKey;
	BYTE						m_byRadius;

	// Local 변수
	//WzVector					vOwnerPos;
	//WzVector					vMemberPos;
	PACKET_TYPE	&				m_Packet;
};

//
//VOID ItemExpDistributor::DistributeItem( Item *pItem, MONEY Money, Player *pItemOwner )
//{
//	// 변수 셋팅
//	GameZone *pZone = pItemOwner->GetGameZonePtr();
//	if( !pZone )	return;
//
//	// 미션맵의 경우
//	BYTE byRadius = 0;
//	if( pZone->GetZoneType() == eZONETYPE_MISSION )
//	{
//		byRadius = 0;
//	}
//	else
//	{
//		// 헌팅, 필드
//		byRadius = 60;
//	}
//
//	GameParty *pParty = GamePartyManager::Instance()->FindParty( pItemOwner->GetPartyState().GetPartyKey() );
//
//	// 아이템분배 객체 생성
//	if( pParty )
//	{
//		DistributeItemOpr<MSG_CG_ITEM_PICK_MONEY_ACK> Opr( pItemOwner, pItem, Money, pZone->GetKey(), byRadius );
//
//		switch(pZone->GetZoneType())
//		{
//		case eZONETYPE_FIELD:
//			{
//				if( pItem && pItem->GetItemSlot().GetItemInfo()->m_wType == eITEMTYPE_QUEST ||
//					pItem && pItem->GetItemSlot().GetItemInfo()->m_wType == eITEMTYPE_AC )
//				{
//					pParty->ForEachMember( Opr );
//				}
//			}break;
//		case eZONETYPE_MISSION:
//		case eZONETYPE_HUNTING:
//		case eZONETYPE_CHUNTING:
//			{
//				pParty->ForEachMember( Opr );
//			}break;
//		default:
//			{
//			
//			}
//		}
//	}
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	아이템/하임 분배 유효 제한 거리 
//
__inline BYTE _GetRadius( GameZone * pZone )
{
	if( !pZone )	return 0xFF;
	if( pZone->GetZoneType() == eZONETYPE_MISSION )
	{// 미션맵의 경우
		return 0;
	}
	else
	{// 헌팅, 필드
		return 60;
	}
}


//────────────────────────────────────────────────────────────────
// changed by _NA_0_20101206_PARTY_ITEM_DISTRIBUTE_CHANGE
VOID ItemExpDistributor::DistributeItem( Item * pItem, Player * pItemOwner )
{
    if( !pItem )	return;

    WORD PartyKey = pItemOwner->GetPartyState().GetPartyKey();

    GameZone * pZone = pItemOwner->GetGameZonePtr();
    if( !pZone ) return;

    MSG_CG_ITEM_PICK_ACK amsg;
    DistributeItemOpr<MSG_CG_ITEM_PICK_ACK> Opr( pItemOwner, pItem, pZone->GetKey(), _GetRadius(pZone), amsg );

    switch(pZone->GetZoneType())
    {
    case eZONETYPE_FIELD:
    case eZONETYPE_MISSION:
    case eZONETYPE_HUNTING:
    case eZONETYPE_CHUNTING:
    case eZONETYPE_INSTANCE:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:       //_NA_008069_20150210_ADD_SPA_SYSTEM
        {
            const SCItemSlot& ItemSlot = pItem->GetItemSlot();
            const BASE_ITEMINFO* ItemInfo = ItemSlot.GetItemInfo();
            const eITEMTYPE type = static_cast<eITEMTYPE>(ItemInfo->m_wType);

            bool all_party_member_obtain_type = false;
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            sPOLICY_ZONE& Policyment = pZone->Policyment;
            if(Policyment.FIELD & Policyment.ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER) {
                all_party_member_obtain_type = true;
            }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

            if (type == eITEMTYPE_QUEST ||
                type == eITEMTYPE_AC || 
                type == eITEMTYPE_EVENT_FIELD_PARTY_ALL_MEMBER_PICK_UP
                || all_party_member_obtain_type
                )
            {
                NewGamePartyManager::Instance()->For_eachPlayers( PartyKey, Opr );
            }
        }break;
    default:
        {
            //SUNLOG(eCRITICAL_LOG, "Do!!! Coding to DistributeItem formula by ZoneType");
        }
    }
}

// implemented by _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE
void ItemExpDistributor::DistributeMoney(MONEY money, DWORD monster_key, Player* item_owner)
{
    // NOTE: f110413.2L, the below code block that is implemented by
    // '_NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE' is logically integrity.
    // however, time and space complexity is too expensive.
    // for example, approximately (MAX_PARTYMEMBER_NUM + n) space allocating numbers based on
    // to copy container of the member list with in sorting time complexity n·log(n) and,
    // that of the linear container for selected members, etc...
#if SUN_CODE_RENEWAL
    GameZone* const game_zone = (item_owner != NULL) ? item_owner->GetGameZonePtr()
                                                     : NULL;
    if (game_zone == NULL) {
        return;
    };
    GameField* const game_field = (item_owner != NULL) ? item_owner->GetField() : NULL;
    if (game_field == NULL) {
        return;
    };
    //
    const WORD party_key = item_owner->GetPartyState().GetPartyKey();
    const NewGameParty* const party = (party_key == 0)
        ? NULL
        : static_cast<const NewGameParty*>(NewGamePartyManager::Instance()->FindParty(party_key));

    if (party == NULL) {
        return;
    };
    ;{  // filter by the party game rule
        DWORD number_of_party_members = party->GetMemberNum();
        assert(number_of_party_members <= MAX_PARTYMEMBER_NUM);
        if (number_of_party_members == 0) {
            return;
        };
    };

    struct SelectedMembers
    {
        SelectedMembers() { ZeroMemory(this, sizeof(*this)); working_it = players; }
        Player** worked_end() const { return working_it; };
        bool continued_work() const { return working_it != &players[_countof(players)]; }
        Player** working_it;
        Player* players[MAX_PARTYMEMBER_NUM];
    } selected_members;
    //
    int pickable_radius_squared = _GetRadius(game_zone);
    ;   pickable_radius_squared *= pickable_radius_squared;
    const WzVector& center_pos = *item_owner->GetPosPtr();

    typedef NewGameParty::MEMBER_MAP PartyMembers;
    const PartyMembers& member_list = party->GetMemberList();
    const PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const PartyMembers::value_type& member_node, member_list, PartyMembers)
    {
        // PartyMembers::value_type::second_type
        const _PARTY_MEMBER_INFO& member_info = member_node.second;
        Player* member = player_manager->FindPlayerByObjKey(member_info.m_dwMemberKey);
        if (member == NULL || member->IsDead()) {
            continue;
        };
        if (game_field != member->GetField()) {
            continue;
        };
        if ((pickable_radius_squared != 0) &&
            (Math_VectorLength2(center_pos, *member->GetPosPtr()) > pickable_radius_squared))
        {
            continue;
        };
        // meet the all conditions
        *selected_members.working_it++ = member;
        if (selected_members.continued_work() == false) {
            assert(!"the number of members has grater than expected specification");
            break;
        };
    }; //end 'FOREACH_CONTAINER'
    //
    // do members exist?
    const size_t number_of_selected_members = static_cast<size_t>(
        selected_members.working_it - selected_members.players);
    if (number_of_selected_members == 0) {
        return;
    };

    const MONEY each_allocated_money = (money / number_of_selected_members);
    ;{  // NOTE: main processing
        Player** it = selected_members.players;
        Player** const end = selected_members.worked_end();
        for ( ; it != end; ++it)
        {
            Player* member = *it;
        #if !defined(__NA_000632_20070817_CHINA_FATIGUE_HEIM)
            // fatigue system content
            if (FatigueManager* fatigue = member->GetFatigueManager())
            {
                //fatigue->GetShareItemPickupRatio
                //fatigue->GetShareHeimPickupRatio
                int pickable_ratio = fatigue->GetShareItemPickupRatio();
                int seed = (random(1, 100));
                if (seed > pickable_ratio) {
                    continue;
                };
            }
        #endif
            member->PickMoney(each_allocated_money, monster_key, item_owner);
        }
    };
#else //SUN_CODE_BACKUP
    WORD party_key = item_owner->GetPartyState().GetPartyKey();
    NewGameParty* pParty = (NewGameParty*)NewGamePartyManager::Instance()->FindParty(party_key);
    if(!pParty)
        return;

    DWORD number_of_party_members = pParty->GetMemberNum();
    if(number_of_party_members < 1)
        return;

	GameZone * game_zone = item_owner->GetGameZonePtr();
	if( !game_zone ) return;

    WORD pickable_radius = _GetRadius(game_zone);

    BaseParty::MEMBER_MAP tempParty = pParty->GetMemberList();
    STLX_VECTOR<DWORD> tempParty2;
    tempParty2.reserve(MAX_PARTYMEMBER_NUM);

    // 조건에 맞는 멤버를 찾는다
    for(BaseParty::MEMBER_MAP::iterator iter = tempParty.begin(); iter != tempParty.end(); ++iter)
    {
        const _PARTY_MEMBER_INFO& memberInfo = iter->second;

        Player *pMember = PlayerManager::Instance()->FindPlayerByObjKey( memberInfo.m_dwMemberKey );
        if( !pMember ) continue;
        if( pMember->IsDead() ) continue;

        GameZone *pZone = pMember->GetGameZonePtr();
        if( !pZone ) continue;

        GameField* pOwnerField = item_owner->GetField();
        GameField* pMemberField	= pMember->GetField();
        if( !pOwnerField || !pMemberField )
            continue;
        if( pOwnerField->GetFieldCode() != pMemberField->GetFieldCode() )
            continue;

        if( pickable_radius != 0 && Math_VectorLength( *item_owner->GetPosPtr(), *pMember->GetPosPtr() ) > pickable_radius )	continue;

        tempParty2.push_back(memberInfo.m_dwMemberKey);
    }

    DWORD nParty2Num = tempParty2.size();
    if(nParty2Num < 1)
        return;

    DWORD distributeMoney = money / nParty2Num;
    for(STLX_VECTOR<DWORD>::iterator iter2 = tempParty2.begin(); iter2 != tempParty2.end(); ++iter2)
    {
        Player *pMember = PlayerManager::Instance()->FindPlayerByObjKey(*iter2);
        if( !pMember )	continue;
        if( pMember->IsDead() ) continue;

#if !defined(__NA_000632_20070817_CHINA_FATIGUE_HEIM)
        // 피로도 시스템
        BYTE byPickRatio = pMember->GetFatigueManager()->GetShareItemPickupRatio();
        if( random(1, 100) > byPickRatio )	continue;
#endif

        pMember->PickMoney(distributeMoney, monster_key, item_owner);
    }
#endif //SUN_CODE_BACKUP
}
