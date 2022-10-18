#include "StdAfx.h"
#include "FieldPvP.h"
#include "ChaoState.h"
#include "Player.h"

#include "ItemManager.h"
#include "DropManager.h"
#include "SCItemSlotContainer.h"
#include "PKScriptParser.h"

#include "GameGuildExtension.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#include "GameGuild.h"
#include "GameGuildManager.h"
#include "DominationInfoParser.h"
#include "GameDominationManager.h"
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

//==================================================================================================
FieldPvPRuleManager::FieldPvPRuleManager( WORD wConflictParts ) 
    : m_wConflictParts(wConflictParts) 
{
    CreateTroubledPart();
}

FieldPvPRuleManager::~FieldPvPRuleManager()
{
    m_vTroubledPart.clear();
}

void
FieldPvPRuleManager::CreateTroubledPart()
{
    m_vTroubledPart.clear();
    if( m_wConflictParts & PTA_PK_TILE )
        m_vTroubledPart.push_back(FieldPvpPtr(new BoundaryPart()));
    if( m_wConflictParts & PTA_CONFLICT_TILE )
        m_vTroubledPart.push_back(FieldPvpPtr(new NomalPart()));
    if( m_wConflictParts & PTA_FREE_CONFLICT_TILE )
        m_vTroubledPart.push_back(FieldPvpPtr(new FreePart()));
    if( m_wConflictParts & PTA_DOMINATION_WAR_TILE )
        m_vTroubledPart.push_back(FieldPvpPtr(new DominationPart()));  
    if( m_wConflictParts & PTA_SAFETY_ZONE )
        m_vTroubledPart.push_back(FieldPvpPtr(new SafetyPart()));
}

FieldPvPRuleManager::FieldPvpPtr FieldPvPRuleManager::GetPart(Player* const player) const
{
    if (obj_util::IsValid(player))
    {
        WORD tile = 0;
        if (player->GetTileAtt(&tile))
        {
            for (FieldPvP_List::const_iterator it = m_vTroubledPart.begin(); 
                it != m_vTroubledPart.end(); ++it)
            {
                FieldPvpPtr field_pvp = *it;
                if (field_pvp->IsPart(tile))
                {
                    return field_pvp;
                }
            }
        }
    }
    return FieldPvpPtr();
}

//==================================================================================================
//필드 서버 공통 PvP룰


BOOL FieldPvP::ChkServer( OUT RC::eBATTLE_RESULT* pErrCode )
{
	// 월드서버와 끊어져 있으면 PK를 막는다.
	if( !g_pGameServer->IsConnectToWorldServer() )
	{
		*pErrCode = RC::RC_BATTLE_SERVER_STATE_IMPOSSIBLE_PK; 
		return FALSE; 
		// PK가 가능한 서버인지 체크
	}

	if( !g_pGameServer->IsPVPMode() )
	{
		*pErrCode = RC::RC_BATTLE_ISNOT_PK_SERVER; 
		return FALSE;
	}

	return TRUE;
}

BOOL FieldPvP::ChkTarget( Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode  )
{ 
	if( !pTarget )						
	{
		*pErrCode = RC::RC_BATTLE_INVALID_MAINTARGET;
		return FALSE;
	}	

	// 타겟이 존재 할시 피격자 체크 (동일 지역이 아니라면 공격 불가)
	WORD wTileAtt = 0;
	if( FALSE == pTarget->GetTileAtt(&wTileAtt) )
	{
		*pErrCode = RC::RC_BATTLE_PLAYER_TILE_INVLIDPOS;
		return FALSE;
	}

	if( !IsPart(wTileAtt) )
	{
		*pErrCode = RC::RC_BATTLE_ISNOT_TROUBLED_PART;		
		return FALSE;
	}

	return TRUE;
}

RC::eSKILL_RESULT FieldPvP::ConvErrCode( RC::eBATTLE_RESULT ErrCode )
{
	switch( ErrCode )
	{

	case RC::RC_BATTLE_FAILED:						return RC::RC_SKILL_FAILED;
	case RC::RC_BATTLE_ISNOT_PK_SERVER:				return RC::RC_SKILL_ISNOT_PK_SERVER;
	case RC::RC_BATTLE_INVALID_MAINTARGET:			return RC::RC_SKILL_TARGET_NOTEXIST;			
	case RC::RC_BATTLE_ISNOT_TROUBLED_PART:			return RC::RC_SKILL_ISNOT_TROUBLED_TYPE;		
	case RC::RC_BATTLE_TARGET_LEVEL_IS_LOW:			return RC::RC_SKILL_TARGET_LEVEL_IS_LOW;		
	case RC::RC_BATTLE_SAME_GUILD_CHARACTER:		return RC::RC_SKILL_SAME_GUILD_CHARACTER;
	case RC::RC_BATTLE_SAME_PARTY_CHARACTER:		return RC::RC_SKILL_SAME_PARTY_CHARACTER;
	case RC::RC_BATTLE_PLAYER_TILE_INVLIDPOS:		return RC::RC_SKILL_PLAYER_TILE_INVLIDPOS;		
	case RC::RC_BATTLE_ATTACKER_LEVEL_IS_LOW:		return RC::RC_SKILL_ATTACKER_LEVEL_IS_LOW;		
	case RC::RC_BATTLE_PK_IMPOSSIBLE_TARGET_TYPE:	return RC::RC_SKILL_PK_IMPOSSIBLE_TARGET_TYPE;	
	case RC::RC_BATTLE_SERVER_STATE_IMPOSSIBLE_PK:	return RC::RC_SKILL_SERVER_STATE_IMPOSSIBLE_PK;
	default:		 
		break;
	}	

	return RC::RC_SKILL_FAILED;
}

eUSER_RELATION_TYPE FieldPvP::IsFriendPVP( Player* pAttacker, Player *pTarget )
{
	RC::eBATTLE_RESULT dummy; 
	return	PvPRule(pAttacker,pTarget,&dummy);
}

BOOL FieldPvP::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT*  pErrCode )
{
	if( !ChkServer(pErrCode) ) 
		return FALSE;			//  서버가 pvp할수 없다면 공격 불가.

	PvPRule(pAttacker,pTarget,pErrCode);
	if( *pErrCode != RC::RC_BATTLE_SUCCESS )	 
		return FALSE;

	return TRUE;
}

BOOL FieldPvP::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode )
{
	RC::eBATTLE_RESULT BattleErrCode = RC::RC_BATTLE_SUCCESS;
	if( !CanPVP(pAttacker,pTarget,&BattleErrCode) )
	{
		*pErrCode = ConvErrCode(BattleErrCode);
		return FALSE;
	}

	return TRUE;
}

VOID FieldPvP::Resurrection( Player* pDeadPlayer )
{
	GameField* pField = pDeadPlayer->GetField();
	WzVector wvPosOfPlayer;
	if( pField )
		wvPosOfPlayer = pField->GetRandomStartingVector();
	else
		pDeadPlayer->GetPos(&wvPosOfPlayer);

	pDeadPlayer->SetFallDead(wvPosOfPlayer);
}

BOOL FieldPvP::ChkDropItem( Player* pAttacker, Player* pDeadPlayer )
{
	// GM 맵에서는 아이템을 떨구지 않는다.
	GameField *pField = pDeadPlayer->GetField();
	if( pField )
	{
		if( HardCode::MAPCODE_for_GM == pField->GetFieldCode() )
			return FALSE;
	}

	// 드랍 방지 아이템이 존재하는지 체크
    if (pDeadPlayer->GetItemManager()->GetItemFunctionDropProtector().ProtectDrop(pAttacker)) {
        return FALSE;
    };
	return TRUE;
}

VOID FieldPvP::DropItem( Player* pAttacker, Player* pDeadPlayer, SLOTIDX DropSlotIndex )
{
	POSTYPE atSlotPos = 0;
	SCSlotContainer* pContainer = NULL;

	switch(DropSlotIndex)
	{
	case SI_EQUIPMENT:
		pContainer = pDeadPlayer->GetItemManager()->GetItemSlotContainer( SI_EQUIPMENT );
		if( !SelectPKDropItem( pContainer, atSlotPos ) )
		{
			pContainer = pDeadPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
			if( !pDeadPlayer->GetCharInfo()->m_iInventoryLock && SelectPKDropItem(pContainer,atSlotPos) )
				DropSlotIndex = SI_INVENTORY;
			else
				return;
		}
		break;

	case SI_INVENTORY:
		pContainer = pDeadPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
		if( !pDeadPlayer->GetCharInfo()->m_iInventoryLock && SelectPKDropItem(pContainer,atSlotPos) )
		{}
		else
			return;

		break;
	}

	// 선택한 아이템을 필드에 떨군다.
	SCItemSlot DropItemSlotOut;
	SCSlotContainer * pAtContainer = pDeadPlayer->GetItemManager()->GetItemSlotContainer( DropSlotIndex );
	if( !pAtContainer->DeleteSlot( atSlotPos, &DropItemSlotOut ) )
		return;

	if( DropItemSlotOut.IsPossessionItem() && DropItemSlotOut.GetIdentifyPossession() && !DropItemSlotOut.IsDestroyItem() )
	{
		switch( PKScriptParser::Instance()->GetPKOptionInfo().m_ePossessionDropOption )
		{
		case PKOptionInfo::ENUM_POSSESSION_DROP_NO:
			SUNLOG( eCRITICAL_LOG, "Can't come to this logic...");
			break;
		case PKOptionInfo::ENUM_POSSESSION_DROP_ORIGINAL:
				DropItemSlotOut.SetIdentifyPossession( FALSE );
			break;
		case PKOptionInfo::ENUM_POSSESSION_DROP_DESTROY:
				DropItemSlotOut.SetDestroyItem( TRUE );
			break;
		}
	}

	WzVector wvCurPos = pDeadPlayer->GetRandomPosInAround( 2, 30 );
	if( !g_DropManager.DropItemToField( pDeadPlayer->GetField(), &wvCurPos, pAttacker->GetObjectKey(), 0, DropItemSlotOut ) )
		return;

	MSG_CG_ITEM_PK_DROP_ITEM_CMD msg;
	msg.m_atIndex = DropSlotIndex;
	msg.m_atPos = atSlotPos;
	pDeadPlayer->SendPacket( &msg, sizeof( msg ) );

	// 게임 로그
	GAMELOG->LogItem( ITEM_PK_DROP, pDeadPlayer, &DropItemSlotOut, 0, DropItemSlotOut.GetNum() ); 
}

BOOL FieldPvP::SelectPKDropItem( SCSlotContainer* pContainer, POSTYPE & OUT atSlotPos )
{
	// 현재 컨테이너에 들어있는 아이템 개수
	if( pContainer->GetSlotNum() == 0 )	
		return FALSE;

	STLX_VECTOR<POSTYPE> vecValidSlot;

	PKOptionInfo::ePoessessionDropOption DropOption = PKScriptParser::Instance()->GetPKOptionInfo().m_ePossessionDropOption;
	//(shogen) for문을 두번 돌지 말고 벡터를 
	//두개써서 하는게 시간복잡도를 줄일건데...
	//오늘 퇴사라 그냥 갑니다. 다음에 보시는 분들 수정 부탁해요 '')/

	//장비만 검색해 본다.
	for( POSTYPE SlotPos = 0; SlotPos < pContainer->GetMaxSlotNum(); ++SlotPos )
	{
		if( pContainer->IsEmpty(SlotPos) )	continue;

		// 락여부 체크
		if( pContainer->IsLocked(SlotPos) )	
			continue;

#ifdef _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE
		// 드랍 불가능한 아이템인가? ( 퀘스트 아이템 )
		SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(SlotPos);

		//드랍불가 아이템 이외의 것은 모두 드랍이 가능하도록 변경
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
			continue;
		}
#else
		// 드랍 불가능한 아이템인가? ( 퀘스트 아이템 )
		SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(SlotPos);
		// 모두 불능의 아이템이면 바로 패스한다
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DONTALL) == false) {
			continue;
		}

		//드랍불가 아이템이면서 PK드랍 가능아이템인가 검사
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
			//_NA_006052_20120904_CANNOT_DROP_CAN_PKDROP
			if (rSlot.CanTradeSell(eITEM_TRADESEEL_DROP_CAN_PKDROP) == true) {
				continue;
			}
		}
#endif //(_CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE)


		//장비 아이템인가?
		if( (!rSlot.GetItemInfo()->IsWeapon()) && !(rSlot.GetItemInfo()->IsArmor()) )
			continue;

		if( rSlot.IsPossessionItem() && rSlot.GetIdentifyPossession() )
		{
			if( DropOption == PKOptionInfo::ENUM_POSSESSION_DROP_NO )
			{
				if( !rSlot.IsDestroyItem() )
					continue;
			}
		}

		// 드랍 가능한 아이템을 일단 setValidSlot에 넣는다.
		vecValidSlot.push_back( SlotPos );
	}

	if( vecValidSlot.size() > 0 )
	{
		// 랜덤하게 섞은 후 첫번째 항목을 선택한다.
		random_shuffle( vecValidSlot.begin(), vecValidSlot.end() );
		atSlotPos = vecValidSlot[0];
		return TRUE;
	}	

	return FALSE;
}


BYTE FieldPvP::PKDropRatio( int iLevelDiff )
{
	BYTE byDropRatio = 0;
	if( iLevelDiff < 0 )
	{
		byDropRatio = max(0, iLevelDiff + 20 );
	}		
	else if ( iLevelDiff == 0 )
	{
#ifdef _CH_006797_20130703_PK_DROP_RULE_MODIFY
        byDropRatio = 10;
#else
		byDropRatio = 25;
#endif // _CH_006797_20130703_PK_DROP_RULE_MODIFY
	}
	else if( iLevelDiff > 0 )
	{
		byDropRatio = min(100, iLevelDiff + 30 );
#ifdef _CH_006797_20130703_PK_DROP_RULE_MODIFY
        if (byDropRatio > 20)
        {
            byDropRatio = 20;
        }
#else
#ifdef _NA_006057_20120904_PK_DROP_RATIO_MODIFY
        if (byDropRatio > 50)
        {
            byDropRatio = 50;
        }
#endif // _NA_006057_20120904_PK_DROP_RATIO_MODIFY
#endif // _CH_006797_20130703_PK_DROP_RULE_MODIFY
	}
	return byDropRatio;
}

// 길드 무관이거나 길원 일때 false, 쌍방적대 적대 선포일때 true
bool FieldPvP::HostilityRelationCheck(Player* pAttacker, Player* pTarget)
{
	if( !pAttacker || !pTarget ) return false;

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld == 0 || DefGld == 0 || AttkGld == DefGld ) return false;

	return GuildSmallContentEntry::RelationSystem::IsHostilityRelation(AttkGld, DefGld);
}

bool FieldPvP::IsOneSideHostility(Player* pAttacker, Player* pTarget)
{
	if( !pAttacker || !pTarget ) return false;

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld == 0 || DefGld == 0 || AttkGld == DefGld ) return false;

	return eGUILD_RELATION_ONE_SIDE_HOSTILITY ==
           GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld,DefGld);
}

//////////////////////////////////////////////////////////////////////////
//자유 분쟁지역룰

BOOL FreePart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_FreePart & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

BOOL FreePart::RandomizePercent( Player* pAttacker, Player* pDeadPlayer )
{ 
	// 드랍 확률을 체크한다.
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	int iLevelDiff = pDeadPlayer->GetLevel() - pAttacker->GetLevel();

#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
    if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL )
    {
        if( !pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
        {
            // 일반 상태에서는 PK당해도 리벤지 대상이 아닐경우 아이템을 드랍하지 않는다
            return FALSE;
        }			
    }
#endif // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM

	BYTE DropRatio = PKDropRatio( iLevelDiff );

	return random_percent(DropRatio);
}

VOID FreePart::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ )
{
	// 소환체를 죽인 것이 아니라면 자유 분쟁 지역에서 아이템을 떨군다.
	if( !bSummoned )
	{
		WORD wTileAtt = 0;
		if( FALSE == pDeadPlayer->GetTileAtt(&wTileAtt) )
		{
			// 대상의 타일이 존재하지 않는다.
			SUNLOG( eCRITICAL_LOG, "[TroubledPart::%s] Invalid Tile! \n", __FUNCTION__ );
			return;
		}

		// PK 로그를 남긴다.
        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_TROUBLED_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_TROUBLED_DIE,  pDeadPlayer, pAttacker, damage_value );

		//피격자가 프리 분쟁 지역일시 아이템을 떨군다.
#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
        if( RandomizePercent(pAttacker,pDeadPlayer) && 
            ChkDropItem(pAttacker,pDeadPlayer) )
#else  // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
        if( ChkDropItem(pAttacker,pDeadPlayer) && 
            RandomizePercent(pAttacker,pDeadPlayer) )
#endif // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
		{
			SLOTIDX		  DropSlotIndex  = SI_INVENTORY;
			PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

			if( random_percent(stPKOptionInfo.m_EquipDropRatio) )
				DropSlotIndex = SI_EQUIPMENT;

			DropItem( pAttacker, pDeadPlayer, DropSlotIndex );
		}

		//마을에서 죽을시는 부활 시켜준다. 
		//MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN 패킷이 들어올 경우 로직 변경 필요.
		if( pDeadPlayer->GetGameZoneState() == ePRS_AT_VILLAGE ) Resurrection(pDeadPlayer);
	}
}

eUSER_RELATION_TYPE FreePart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
	*pErrCode = RC::RC_BATTLE_SUCCESS;

	if( pAttacker == pTarget )			
		return USER_RELATION_FRIEND;									// 0. 자기자신인지이면 공격(버프) 가능

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;								// 1. 범위 공격시 CanPVP를 통하지 않으므로 체크 필요(타일속성).

	if( !pAttacker->IsForceAttack() )									//.// 일반 공격일 경우
	{
		if( pAttacker->GetPartyState() == pTarget->GetPartyState() )	// 2. 파티원 아군 
			return USER_RELATION_FRIEND;

		GUILDGUID AttkGld   = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
		GUILDGUID TargetGld = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

		if( AttkGld != 0 && AttkGld == TargetGld )	
			return USER_RELATION_FRIEND;								// 3. 길드원이면 아군이다.
	
        if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, TargetGld) ==
            eGUILD_RELATION_ALLIENCE)
			return USER_RELATION_FRIEND;								// 3_1. 길드 관계 동맹이면 아군.
	}

	return USER_RELATION_ENEMY;											// 4. 예외에 비해당 유저 적군.
}

//////////////////////////////////////////////////////////////////////////
//일반 분쟁지역룰

BOOL NomalPart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_NomalPart & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

VOID NomalPart::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ )
{
	if( !bSummoned )
	{
		// PK 로그를 남긴다.

        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_TROUBLED_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_TROUBLED_DIE,  pDeadPlayer, pAttacker, damage_value );

		//마을에서 죽을시는 부활 시켜준다.
		//현재 마을에서 죽는 경우 없음.
		//MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN 패킷이 들어올 경우 로직 변경 필요.
		if( pDeadPlayer->GetGameZoneState() == ePRS_AT_VILLAGE ) Resurrection(pDeadPlayer);
	}
}

eUSER_RELATION_TYPE NomalPart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
	*pErrCode = RC::RC_BATTLE_SUCCESS;

	if( pAttacker == pTarget )			
		return USER_RELATION_FRIEND;									// 0. 자기자신인지이면 공격(버프) 가능

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;								// 1. 범위 공격시 CanPVP를 통하지 않으므로 체크 필요(타일속성).

	if( pAttacker->GetPartyState() == pTarget->GetPartyState() )		// 2. 파티원 공격 불가 
		return USER_RELATION_FRIEND;

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld != 0 && AttkGld == DefGld )	
		return USER_RELATION_FRIEND;									// 3. 길드원이면 아군이다.

    if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, DefGld) ==
        eGUILD_RELATION_ALLIENCE)
		return USER_RELATION_FRIEND;									// 3_1. 길드 관계 동맹이면 아군.

	return USER_RELATION_ENEMY;											// 4. 강제공격이 존재 하지 않으며, 예외에 비해당 유저 적군.
}

//////////////////////////////////////////////////////////////////////////
//경계지역룰


//길드 관계에 따른 pk룰 정리
// 기본정의 : "길드간 선전포고(적대선포)를 한 경우의 공격은 정당하며,
//			정당한 공격은 상대를 죽여도 패널티를 받지 않으며, 아이템 또한 드랍되지 않는다."

//
//경계지역의 경우 B에게 선전포고를 한 A의 공격은 정당공격으로 패널티를 받지 않으며
//B가 죽임을 당해도 아이템을 드랍하지 않는다.

//반대의 경우 B가 선전포고를 받아 들이지 않고(쌍방적대), A를 공격할 경우 패널티를 받으며,
//A가 아이템을 드랍하게 된다.

BOOL BoundaryPart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_BoundaryPart & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

BOOL BoundaryPart::RandomizePercent( Player* pAttacker, Player* pDeadPlayer )
{ 
	// 드랍 확률을 체크한다.
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	int iLevelDiff = pDeadPlayer->GetLevel() - pAttacker->GetLevel();	BYTE DropRatio = 0;
	if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL )
	{
		// 죽은넘이 킬러의 리벤지 대상이면 50% 확률을 적용시킨다.
		if( pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
		{
			DropRatio = stPKOptionInfo.m_BadFriendDropRatio;
		}
		else
		{		
#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
            // 일반 상태에서는 PK당해도 리벤지 대상이 아닐경우 아이템을 드랍하지 않는다
            return FALSE;
#else  // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
            DropRatio = PKDropRatio( iLevelDiff );
#endif // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
		}			
	}
	else if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_PREV_CHAO )
	{
		DropRatio = stPKOptionInfo.m_PreChaoDropRatio;
	}
	else if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_CHAO )
	{
		DropRatio = stPKOptionInfo.m_ChaoDropRatio;
	}

	return random_percent(DropRatio);
}

// 
VOID BoundaryPart::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ )
{
	//(lst1024)(080905)(2387)소환수를 죽일 경우 패널티 없음.
	if( bSummoned ) return;

	if( HostilityRelationCheck(pAttacker,pDeadPlayer) )	
	{
        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_HOSTILITY_GUILD_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_HOSTILITY_GUILD_DIE,  pDeadPlayer, pAttacker, damage_value );
		
#ifdef _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION
        if( ChkDropItem(pAttacker,pDeadPlayer) && 
            RandomizePercent(pAttacker,pDeadPlayer) )
        {
            SLOTIDX		  DropSlotIndex  = SI_INVENTORY;
            PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

            if( random_percent(stPKOptionInfo.m_EquipDropRatio) )
                DropSlotIndex = SI_EQUIPMENT;

            DropItem( pAttacker, pDeadPlayer, DropSlotIndex );
        }
#endif //_CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION
	}
	else
	{

        PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

		// 보라돌이 상태 해제
		pAttacker->GetChaoState().EndBoradoriState();
		// pTarget이 리벤지 리스트에 등록된 유저라면 월드서버에 삭제를 요청한다.
		if( pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
		{
			pAttacker->GetChaoState().RequestRemoveBadFriend( pDeadPlayer->GetCharGuid() );
		}
    
    #if !defined(_CHINA) //CHINA_PK_RULE - 중국 버전은 사용하지 않는다.
        // (lst1024)(090617) 공격자가 일반이고, 상대가 프리카오일 경우는 카오성향을 올리지않는다.
		else if( pAttacker->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL &&
                 pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_PREV_CHAO ) 
        {}
    #endif
        // 상대가 카오, 보라돌이 상태라면 나의 카오성향은 올리지 않는다.
        else if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_CHAO || pDeadPlayer->GetChaoState().IsBoradori() ) 
		{}
        // (lst1024)(090617)카오룰 변경 자유 분쟁에 소환수로 공격후 경계지역으로 도망, 소환수가 타 케릭을 죽일시 여기로 들어올수 있음.
        // 20랩 이하는 보호필요.
		else if( pAttacker->GetLevel() < stPKOptionInfo.m_PKProtectLV )	
        {}
		else
		{
			// 카오성향을 올린다.
			pAttacker->GetChaoState().UpChaoState();

			// 상대방의 리벤지 리스트에 이미 등록된 유저인지 체크
			if( !pDeadPlayer->GetChaoState().FindBadFriend( pAttacker->GetCharGuid() ) )
			{
				// 리벤지 리스트에 등록할 것인지 묻는다.
				MSG_CG_CHAO_ASK_REVENGE_ADDITION_CMD cmdMsg;
				_tcsncpy( cmdMsg.m_ptszRevengeName, pAttacker->GetCharName(), MAX_CHARNAME_LENGTH );
				cmdMsg.m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
				pDeadPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

				// 리벤지 대상의 이름을 기억해 놓는다.
				pDeadPlayer->GetChaoState().SetRevengeName( cmdMsg.m_ptszRevengeName );
			}
		}
		
		// PK 로그를 남긴다.
        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_PK_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_PK_DIE, pDeadPlayer, pAttacker, damage_value );

#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
        if( RandomizePercent(pAttacker,pDeadPlayer) && 
            ChkDropItem(pAttacker,pDeadPlayer) )
#else  // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
        if( ChkDropItem(pAttacker,pDeadPlayer) && 
            RandomizePercent(pAttacker,pDeadPlayer) )
#endif // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
		{
			SLOTIDX		  DropSlotIndex  = SI_INVENTORY;
			PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

			if( random_percent(stPKOptionInfo.m_EquipDropRatio) )
				DropSlotIndex = SI_EQUIPMENT;

			DropItem( pAttacker, pDeadPlayer, DropSlotIndex );
		}
	}
}


eUSER_RELATION_TYPE BoundaryPart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
	*pErrCode = RC::RC_BATTLE_SUCCESS;

	if( pAttacker == pTarget )			
		return USER_RELATION_FRIEND;								// 0. 자기자신인지이면 공격(버프) 가능

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;							// 1. 범위 공격시 CanPVP를 통하지 않으므로 체크 필요(타일속성).

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld != 0 && AttkGld == DefGld )	
	{
		return USER_RELATION_FRIEND;								// 2. 길드원이면 아군이다.
	}

	//(lst1024)(080901) 길드 관계 동맹일 경우 강제공격 불가.
    if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, DefGld) ==
        eGUILD_RELATION_ALLIENCE)
		return USER_RELATION_FRIEND;								// 3. 길드 관계 동맹이면 아군.

    if (GuildSmallContentEntry::RelationSystem::IsHostilityRelation(AttkGld, DefGld))
		return USER_RELATION_ENEMY;									// 4. 길드 관계 적대면 적군.

	if( pAttacker->GetPartyState() == pTarget->GetPartyState() )	
	{
		return USER_RELATION_FRIEND;								// 5. 파티원 이면 아군.
	}

 #if defined(_CHINA) //CHINA_PK_RULE - 중국 보라돌이 자동 강제공격 삭제.
	if( pTarget->GetChaoState().IsChaoState() )
 #else
	if( pTarget->GetChaoState().IsBoradori() ||
		pTarget->GetChaoState().IsChaoState() )					
 #endif	
		return USER_RELATION_ENEMY;									// 6. 보라돌이, 카오상태이면 적군.

	if( pAttacker->IsForceAttack() )							//.// 강제 공격룰
	{
		if( IsOneSideHostility(pAttacker,pTarget) )				
			return USER_RELATION_ENEMY;								// 7. PK룰 보다 길드룰이 우선 하므로, 일방적대시에도 뉴비 공격이 가능하다.

		PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
		if( pTarget->GetLevel() < stPKOptionInfo.m_PKProtectLV )	
		{
			*pErrCode = RC::RC_BATTLE_TARGET_LEVEL_IS_LOW;
			return USER_RELATION_NEUTRALIST;						// 8. 피격자 레벨 체크
		}

		if( pAttacker->GetLevel() < stPKOptionInfo.m_PKAllowableLV )	 
		{
			*pErrCode = RC::RC_BATTLE_ATTACKER_LEVEL_IS_LOW;
			return USER_RELATION_NEUTRALIST;						// 9. 공격자 레벨 체크
		}

		return USER_RELATION_ENEMY;									// 10. 일반 카오
	}

	return USER_RELATION_FRIEND;									// 11. 예외에 비해당 하는 유저 아군
}


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//지역점령전룰

BOOL DominationPart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_Domination & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

VOID DominationPart::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ )
{
    if( !bSummoned )
    {
        //_NA_008706_20160229_TEXTLOG_PK
        // PK 로그를 남긴다.
        GAMELOG->LogAction_PK( ACT_DOMINATION_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_DOMINATION_DIE,  pDeadPlayer, pAttacker, damage_value );
    }
}

eUSER_RELATION_TYPE DominationPart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
    *pErrCode = RC::RC_BATTLE_SUCCESS;

    if( pAttacker == pTarget )			
        return USER_RELATION_FRIEND;									// 0. 자기자신인지이면 공격(버프) 가능

    if( !ChkTarget(pTarget,pErrCode) )	
        return USER_RELATION_NEUTRALIST;								// 1. 범위 공격시 CanPVP를 통하지 않으므로 체크 필요(타일속성).

    GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
    GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
    
    if (AttkGld == 0 || DefGld == 0)
        return USER_RELATION_ENEMY;
    else if (AttkGld == DefGld )	
        return USER_RELATION_FRIEND;									// 3. 길드원이면 아군이다.

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    // 4. 공격자와 피격자의 관계가 적대라도 각자의 길드가 수성측과 동맹일 경우.. 아군이다.
    GameField* field = pAttacker->GetField();
    if (field == NULL) 
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found field");
        return USER_RELATION_NEUTRALIST;
    }

    MAPCODE map_code = \
        DominationInfoParser::Instance()->GetDominationFieldCode(field->GetFieldCode());
    if (map_code == 0)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found mapcode");
        return USER_RELATION_NEUTRALIST;
    }

    const DOMINATION_INFO* domination_info = GameDominationManager::Instance()->GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found domination info");    
        return USER_RELATION_NEUTRALIST;
    }

    const GUILDGUID defense_guild_guid = domination_info->m_inGuildInfo.m_GuildGuid;
    GameGuild* const defense_game_guild = g_GameGuildManager.FindGuild(defense_guild_guid);

    // 수성 길드가 있을 경우..
    if (defense_game_guild)
    {
        if (AttkGld == defense_guild_guid ||
            DefGld == defense_guild_guid)
        {
            GameGuild* const attacker_game_guild = g_GameGuildManager.FindGuild(AttkGld);
            GuildRelationSystem::GuildRelationEntry* attacker_guild_entry = \
                attacker_game_guild->GetRelationEntry();
            eGUILD_RELATION relation = attacker_guild_entry->GetRelationType(DefGld);
            if (relation == eGUILD_RELATION_ALLIENCE)
            {
                return USER_RELATION_FRIEND;
            }
        }

        GuildRelationSystem::GuildRelationEntry* defense_guild_entry = \
            defense_game_guild->GetRelationEntry();
        eGUILD_RELATION attacker_relation = defense_guild_entry->GetRelationType(AttkGld);
        eGUILD_RELATION target_relation = defense_guild_entry->GetRelationType(DefGld);
        if (attacker_relation == eGUILD_RELATION_ALLIENCE && 
            target_relation == eGUILD_RELATION_ALLIENCE)
        {
            return USER_RELATION_FRIEND;
        }

        else if (attacker_relation != eGUILD_RELATION_ALLIENCE &&
                 target_relation != eGUILD_RELATION_ALLIENCE)
        {
            GameGuild* const attacker_game_guild = g_GameGuildManager.FindGuild(AttkGld);
            GuildRelationSystem::GuildRelationEntry* attacker_guild_entry = \
                attacker_game_guild->GetRelationEntry();
            eGUILD_RELATION relation = attacker_guild_entry->GetRelationType(DefGld);
            if (relation == eGUILD_RELATION_ALLIENCE)
            {
                return USER_RELATION_FRIEND;
            }
        }

    }

    // 수성 길드가 없을 경우.. (빈성)
    else 
    {
        GameGuild* const attacker_game_guild = g_GameGuildManager.FindGuild(AttkGld);
        GuildRelationSystem::GuildRelationEntry* attacker_guild_entry = \
            attacker_game_guild->GetRelationEntry();
        eGUILD_RELATION relation = attacker_guild_entry->GetRelationType(DefGld);
        if (relation == eGUILD_RELATION_ALLIENCE)
        {
            return USER_RELATION_FRIEND;
        }
    }
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    return USER_RELATION_ENEMY;											
}
//////////////////////////////////////////////////////////////////////////

//==================================================================================================
BOOL
SafetyPart::IsPart(const WORD wTileAtt)
{
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    // (NOTE) 어차피 옵션 자체가 PvP방지이므로 로직은 동일하게 false반환으로...
    if(PKOptionInfo::ignore_SafetyPart & opt)
        return false;

    // 하기 내용은 좀 더 검증 후 'false'로 수정할 필요 있다.
    return FieldPvP::IsPart(wTileAtt);
}

VOID
SafetyPart::KillPlayer(Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, BOOL is_oneshot)
{
    SUNLOG(eCRITICAL_LOG,
           "["__FUNCTION__"]|Msg=violation PvP rule|Attacker=%s|Dead player=%s|Option=%d|",
           pAttacker->GetCharName(), pDeadPlayer->GetCharName(), !!bSummoned);
}

eUSER_RELATION_TYPE
SafetyPart::PvPRule(Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode)
{
    // (NOTE) NormalPart와 유사하다.
    *pErrCode = RC::RC_BATTLE_SUCCESS;

    // 0. 자기자신인지이면 공격(버프) 가능
    if(pAttacker == pTarget)
        return USER_RELATION_FRIEND;

    // 1. 파티원 공격 불가 
    if(pAttacker->GetPartyState() == pTarget->GetPartyState())
        return USER_RELATION_FRIEND;

    GUILDGUID guild_of_attacker = pAttacker->GetCharInfo()->m_GuildGuid;
    if(guild_of_attacker != 0) {
        GUILDGUID guild_of_target = pTarget->GetCharInfo()->m_GuildGuid;
        if(guild_of_target != 0) {
            if(guild_of_attacker == guild_of_target)
                return USER_RELATION_FRIEND;
            // (NOTE) 이건 무리하게 검사하지 않도록 한다.
            //if(GetRelationType(guild_of_attacker, guild_of_target) == eGUILD_RELATION_ALLIENCE)
            //    return USER_RELATION_FRIEND;
        }
    }

    // (WARNING) 좀 더 룰에 대한 고려를 할 필요가 있다.
    return USER_RELATION_NEUTRALIST;
}
