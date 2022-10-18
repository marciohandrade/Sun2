#include "stdafx.h"

#include "ItemExpDistributor.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
#include <PublicMath.h>
#include "ServerOptionParser.h"
#include <ServerOptionParserEx.h>

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
ItemExpDistributor g_ItemExpDistributor;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	模嶸掏 唸薑( 唳я纂, 嬴檜蠱 )
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

				// pAttacker陛 模�紐撬� 唳辦 斜 輿檣擊 瓊嬴憮 だじ酈蒂 撢たп撿и棻.
				if(pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
				{
					Character *pSummoner = m_pField->FindCharacter( pAttacker->GetSummonerKey() );
					if( pSummoner )
					{
						wPartyKey = pSummoner->GetPartyState().GetPartyKey();
						dwObjKey = pSummoner->GetObjectKey();
					}
				}

				// 模嶸掏 ん檣お蒂 掘и棻.
				if( wPartyKey )
				{
					// だじ錳檣 唳辦
					// и 跨蝶攪縑 渠п憮 2偃 檜鼻曖 だじ陛 唳爵й 熱 氈棻. 斜楚憮 だじん檣お蒂 裘戲煎 婦葬и棻.
					PARTYPOINT_MAP_ITER it = m_mapPartyPoint.find( wPartyKey );
					if( it == m_mapPartyPoint.end() )	
					{
						//⑷營 だじ酈縑 п渡ж朝 だじん檣お陛 嬴霜 橈棻賊...
						stOwnerPoint.m_TotDamage = pRecord->GetTotDamage();
						stOwnerPoint.m_byOwnerPoint = (BYTE)( 70 * pRecord->GetTotDamage() / m_dwMaxHP );
						if( pRecord->IsFirstAttacker() )  stOwnerPoint.m_byOwnerPoint += 30;

						m_mapPartyPoint.insert( PARTYPOINT_MAP_PAIR(wPartyKey, stOwnerPoint) );

						byOwnerPoint = stOwnerPoint.m_byOwnerPoint;
					}
					else
					{
						//だじ酈縑 п渡ж朝 だじん檣お陛 襄營и棻賊...
						PartyOwnerPoint &PartyPoint = it->second;
						PartyPoint.m_TotDamage += pRecord->GetTotDamage();
						PartyPoint.m_byOwnerPoint = (BYTE)( 70 * PartyPoint.m_TotDamage / m_dwMaxHP );
						if( pRecord->IsFirstAttacker() )  PartyPoint.m_byOwnerPoint += 30;

						byOwnerPoint = PartyPoint.m_byOwnerPoint;
					}
				}
				else
				{
					// だじ錳檜 嬴棋 唳辦
					byOwnerPoint = (BYTE)( 70 * pRecord->GetTotDamage() / m_dwMaxHP );
					if( pRecord->IsFirstAttacker() )  byOwnerPoint += 30;
				}

				// 譆渠 模嶸ん檣お蒂 偵褐и棻.
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

			// Local 滲熱
			BYTE				byOwnerPoint;
			PartyOwnerPoint		stOwnerPoint;
			DWORD				dwObjKey;
};


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


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


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

//	NPC朝 避擎 跨蝶攪曖 偌羹檜棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	唳я纂 碟寡
//

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//	唳я纂 碟寡 瑞ず 熱薑 - pNPC Pointer蒂 餌辨ж雖 彊堅紫 籀葬ж朝 寞徹戲煎 滲唳
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
    EXPTYPE groupEXP = resultExp; //爾傘蝶 唳я纂蒂 薯諼и 牖熱 唳я纂 歜衛 盪濰( 斜瑜 爾傘蝶蒂 嬪п )

	double benefit	= 1.0f;

	double var_for_debug = m_pPeakTimeEvent->GetEventExp( pPlayer );	// Peak Time 檜漸お 唳я纂蒂 渦 遽棻.

    //_KR_0_20091021_FIRENDINVITATION_EXTEND // 耀掘 蹺繭 檜漸お 唳я纂蒂 陛骯и棻.
    var_for_debug += pPlayer->GetInvitateFriend().GetAddExpRate(rArgs.radius, rArgs.pwvNpcPos, rArgs.fRadius_Q2);

	benefit += var_for_debug;
	var_for_debug = pZone->GetBonusExp( pPlayer );	// 蓬ぷ寞濰 寞檣雖 羹觼
	benefit += var_for_debug;
	var_for_debug = pPlayer->GetBonusExpRatio();	// 唳я纂 隸陛 樓撩檜 氈朝雖 羹觼
	benefit += var_for_debug;

	//	PC寞 嶸盪檜賊 唳я纂 10%蒂 渦 遽棻.(PC寞 蹂旎嘐陶濠陛 嬴棋雖 羹觼)
	if( pPlayer->CanApplyPCBangBenefit( FALSE ) )
	{	//	(WAVERIX)(080912)(THINKING)
		//	- Singleton縑 櫛熨ж堅 陝謙 褫暮 掘褻蒂 霜蕾 霤褻ж朝 匙檜 渡翱ж啪 儅陝ж朝 雖旎 m_fPCBANG_BONUS_EXP_RATIO偽擎 匙擊
		//	滌紫煎 蹺轎ж賊 渠っ 踹詳擊 熱 氈棻.
		var_for_debug = ( 1.0f + rServerOption.m_fPCBANG_BONUS_EXP_RATIO );
		benefit *= var_for_debug;
	}

    // changed rule by _NA_002471_20110408_EXP_BENEFIT_UP
    // 1101睡攪 唳я纂 爾傘蝶 譆渠高 薯и 鼻щ
    if (benefit > 10.0f) {
        benefit = 10.0f;
    }
    // (WAVERIX)(080912)(REPLACEMENT PROPOSAL)
    // - 啗骯 鼻曖 嘐蝶陛 儅晦朝 雖朝 薑塵ж啪 纔蝶お朝 腎雖 彊懊雖虜, (凳縑憮 僥薯陛 儅晦朝雖 賅葷)
    // п渡 Rounded朝 奩螢葡擊 ж朝等 氈橫憮 瞳橫紫 H/W �秣�, compile option 傳朝 debug|release縑憮 о鼻 翕橾и 唸婁蒂 爾橾 匙檜棻.

    // (JSCHO)(Comment) - Я煎紫 衛蝶蠱(Ы溯檜陛 12衛除檜 剩戲賊 唳я纂 橈擠)
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

	//	(WAVERIX)(080912)(BUG-FIX) double, EXPTYPE, 蛔蛔 顫殮 薑爾陛 詭顫僥濠翮 ん裝婁 棻腦棻.
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
	//	- 跨蝶攪陛 薯剪脹 衛薄曖 撩棟 難薑 唸婁朝 鼻渡�� 氅蜂檜棻.
	//	- 翱婦脹 檜漸お檣 蝶鑒, 等嘐雖 籀葬蛔擎 斛學檜棻. (檜 睡碟擎 樹晝ж晦褻離 褓棻)

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//
BOOLEAN
ItemExpDistributor::GetCalcedExpValue(Player* const pPlayer,
                                      const DistributeExpOprArgs* const pArgs,
                                      DistributeExpOprResult* OUT pResult)
{
    if(!(pArgs && pResult))
        return FALSE;

    ZeroMemory(pResult, sizeof(*pResult));

    //  唳я纂碟寡 偌羹 儅撩
    DistributeExpOpr actor(DistributeExpOpr::DEO_DISABLE_ALL, *pArgs, pResult);
    return actor(pPlayer);
}



//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//
VOID
ItemExpDistributor::DistributeExp( NPC* pNPC, Character* pExpOwner )
{
	// 滲熱 撢た
	GameZone* pZone = pNPC->GetGameZonePtr();
	if( !pZone ) return;
    if(pExpOwner->IsEqualObjectKind(PLAYER_OBJECT) == 0)
		return;

	const eZONETYPE zone_type = pZone->GetZoneType();
	BYTE byRoomBonusType = 0;	 BYTE byRadius = 0;

	// ьた裘曖 唳辦
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

	//	唳я纂碟寡 偌羹 儅撩
	DistributeExpOpr::Option_Default Opr( args );

	if( !NewGamePartyManager::Instance()->For_eachPlayers( PartyKey, Opr ) )
	{
        if(pExpOwner->IsEqualObjectKind(PLAYER_OBJECT))
			Opr( (Player*)pExpOwner );
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	嬴檜蠱 碟寡
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

		// Я煎紫 衛蝶蠱
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
	// Local 滲熱
	//WzVector					vOwnerPos;
	//WzVector					vMemberPos;
	PACKET_TYPE	&				m_Packet;
    //
    __DISABLE_COPY(DistributeItemOpr<PACKET_TYPE>);
};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	ж歜 碟寡
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

		// Я煎紫 衛蝶蠱
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
		// ж歜隸陛 嬴檜蠱檜 氈朝雖 羹觼
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
        if( pMember->GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW ) //и貲曖 斜瑜錳檜塭紫 螞塭檣檜塭賊
        {
            //⑷營 斜瑜 該棲朝 繭碟徽 晦遽 30戲煎 撲薑 腎橫氈戲釭 蝶觼董お だ憮 瞳辨 檜�譟� 蹺�� 滲唳
            g_pGameServer->NotifyHeimAcquisition(pMember->GetCharGuid(), eHEIM_ACQUISITION_PICKUP, MONEY(DropMoney * 0.03f));
        }
    }

private:
	Player *					m_pOwner;
	MONEY						m_Money;
	KEYTYPE						m_ZoneKey;
	BYTE						m_byRadius;

	// Local 滲熱
	//WzVector					vOwnerPos;
	//WzVector					vMemberPos;
	PACKET_TYPE	&				m_Packet;
};

//
//VOID ItemExpDistributor::DistributeItem( Item *pItem, MONEY Money, Player *pItemOwner )
//{
//	// 滲熱 撢た
//	GameZone *pZone = pItemOwner->GetGameZonePtr();
//	if( !pZone )	return;
//
//	// 嘐暮裘曖 唳辦
//	BYTE byRadius = 0;
//	if( pZone->GetZoneType() == eZONETYPE_MISSION )
//	{
//		byRadius = 0;
//	}
//	else
//	{
//		// ьた, в萄
//		byRadius = 60;
//	}
//
//	GameParty *pParty = GamePartyManager::Instance()->FindParty( pItemOwner->GetPartyState().GetPartyKey() );
//
//	// 嬴檜蠱碟寡 偌羹 儅撩
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	嬴檜蠱/ж歜 碟寡 嶸�� 薯и 剪葬 
//
__inline BYTE _GetRadius( GameZone * pZone )
{
	if( !pZone )	return 0xFF;
	if( pZone->GetZoneType() == eZONETYPE_MISSION )
	{// 嘐暮裘曖 唳辦
		return 0;
	}
	else
	{// ьた, в萄
		return 60;
	}
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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
    // to copy container of the member list with in sorting time complexity n﹞log(n) and,
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

    // 褻勒縑 蜃朝 詹幗蒂 瓊朝棻
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
        // Я煎紫 衛蝶蠱
        BYTE byPickRatio = pMember->GetFatigueManager()->GetShareItemPickupRatio();
        if( random(1, 100) > byPickRatio )	continue;
#endif

        pMember->PickMoney(distributeMoney, monster_key, item_owner);
    }
#endif //SUN_CODE_BACKUP
}
