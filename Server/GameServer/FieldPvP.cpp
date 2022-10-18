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
//�ʵ� ���� ���� PvP��


BOOL FieldPvP::ChkServer( OUT RC::eBATTLE_RESULT* pErrCode )
{
	// ���弭���� ������ ������ PK�� ���´�.
	if( !g_pGameServer->IsConnectToWorldServer() )
	{
		*pErrCode = RC::RC_BATTLE_SERVER_STATE_IMPOSSIBLE_PK; 
		return FALSE; 
		// PK�� ������ �������� üũ
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

	// Ÿ���� ���� �ҽ� �ǰ��� üũ (���� ������ �ƴ϶�� ���� �Ұ�)
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
		return FALSE;			//  ������ pvp�Ҽ� ���ٸ� ���� �Ұ�.

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
	// GM �ʿ����� �������� ������ �ʴ´�.
	GameField *pField = pDeadPlayer->GetField();
	if( pField )
	{
		if( HardCode::MAPCODE_for_GM == pField->GetFieldCode() )
			return FALSE;
	}

	// ��� ���� �������� �����ϴ��� üũ
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

	// ������ �������� �ʵ忡 ������.
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

	// ���� �α�
	GAMELOG->LogItem( ITEM_PK_DROP, pDeadPlayer, &DropItemSlotOut, 0, DropItemSlotOut.GetNum() ); 
}

BOOL FieldPvP::SelectPKDropItem( SCSlotContainer* pContainer, POSTYPE & OUT atSlotPos )
{
	// ���� �����̳ʿ� ����ִ� ������ ����
	if( pContainer->GetSlotNum() == 0 )	
		return FALSE;

	STLX_VECTOR<POSTYPE> vecValidSlot;

	PKOptionInfo::ePoessessionDropOption DropOption = PKScriptParser::Instance()->GetPKOptionInfo().m_ePossessionDropOption;
	//(shogen) for���� �ι� ���� ���� ���͸� 
	//�ΰ��Ἥ �ϴ°� �ð����⵵�� ���ϰǵ�...
	//���� ���� �׳� ���ϴ�. ������ ���ô� �е� ���� ��Ź�ؿ� '')/

	//��� �˻��� ����.
	for( POSTYPE SlotPos = 0; SlotPos < pContainer->GetMaxSlotNum(); ++SlotPos )
	{
		if( pContainer->IsEmpty(SlotPos) )	continue;

		// ������ üũ
		if( pContainer->IsLocked(SlotPos) )	
			continue;

#ifdef _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE
		// ��� �Ұ����� �������ΰ�? ( ����Ʈ ������ )
		SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(SlotPos);

		//����Ұ� ������ �̿��� ���� ��� ����� �����ϵ��� ����
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
			continue;
		}
#else
		// ��� �Ұ����� �������ΰ�? ( ����Ʈ ������ )
		SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(SlotPos);
		// ��� �Ҵ��� �������̸� �ٷ� �н��Ѵ�
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DONTALL) == false) {
			continue;
		}

		//����Ұ� �������̸鼭 PK��� ���ɾ������ΰ� �˻�
		if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
			//_NA_006052_20120904_CANNOT_DROP_CAN_PKDROP
			if (rSlot.CanTradeSell(eITEM_TRADESEEL_DROP_CAN_PKDROP) == true) {
				continue;
			}
		}
#endif //(_CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE)


		//��� �������ΰ�?
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

		// ��� ������ �������� �ϴ� setValidSlot�� �ִ´�.
		vecValidSlot.push_back( SlotPos );
	}

	if( vecValidSlot.size() > 0 )
	{
		// �����ϰ� ���� �� ù��° �׸��� �����Ѵ�.
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

// ��� �����̰ų� ��� �϶� false, �ֹ����� ���� �����϶� true
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
//���� ����������

BOOL FreePart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_FreePart & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

BOOL FreePart::RandomizePercent( Player* pAttacker, Player* pDeadPlayer )
{ 
	// ��� Ȯ���� üũ�Ѵ�.
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	int iLevelDiff = pDeadPlayer->GetLevel() - pAttacker->GetLevel();

#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
    if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL )
    {
        if( !pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
        {
            // �Ϲ� ���¿����� PK���ص� ������ ����� �ƴҰ�� �������� ������� �ʴ´�
            return FALSE;
        }			
    }
#endif // _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM

	BYTE DropRatio = PKDropRatio( iLevelDiff );

	return random_percent(DropRatio);
}

VOID FreePart::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ )
{
	// ��ȯü�� ���� ���� �ƴ϶�� ���� ���� �������� �������� ������.
	if( !bSummoned )
	{
		WORD wTileAtt = 0;
		if( FALSE == pDeadPlayer->GetTileAtt(&wTileAtt) )
		{
			// ����� Ÿ���� �������� �ʴ´�.
			SUNLOG( eCRITICAL_LOG, "[TroubledPart::%s] Invalid Tile! \n", __FUNCTION__ );
			return;
		}

		// PK �α׸� �����.
        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_TROUBLED_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_TROUBLED_DIE,  pDeadPlayer, pAttacker, damage_value );

		//�ǰ��ڰ� ���� ���� �����Ͻ� �������� ������.
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

		//�������� �����ô� ��Ȱ �����ش�. 
		//MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN ��Ŷ�� ���� ��� ���� ���� �ʿ�.
		if( pDeadPlayer->GetGameZoneState() == ePRS_AT_VILLAGE ) Resurrection(pDeadPlayer);
	}
}

eUSER_RELATION_TYPE FreePart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
	*pErrCode = RC::RC_BATTLE_SUCCESS;

	if( pAttacker == pTarget )			
		return USER_RELATION_FRIEND;									// 0. �ڱ��ڽ������̸� ����(����) ����

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;								// 1. ���� ���ݽ� CanPVP�� ������ �����Ƿ� üũ �ʿ�(Ÿ�ϼӼ�).

	if( !pAttacker->IsForceAttack() )									//.// �Ϲ� ������ ���
	{
		if( pAttacker->GetPartyState() == pTarget->GetPartyState() )	// 2. ��Ƽ�� �Ʊ� 
			return USER_RELATION_FRIEND;

		GUILDGUID AttkGld   = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
		GUILDGUID TargetGld = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

		if( AttkGld != 0 && AttkGld == TargetGld )	
			return USER_RELATION_FRIEND;								// 3. �����̸� �Ʊ��̴�.
	
        if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, TargetGld) ==
            eGUILD_RELATION_ALLIENCE)
			return USER_RELATION_FRIEND;								// 3_1. ��� ���� �����̸� �Ʊ�.
	}

	return USER_RELATION_ENEMY;											// 4. ���ܿ� ���ش� ���� ����.
}

//////////////////////////////////////////////////////////////////////////
//�Ϲ� ����������

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
		// PK �α׸� �����.

        //_NA_008706_20160229_TEXTLOG_PK
        GAMELOG->LogAction_PK( ACT_TROUBLED_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_TROUBLED_DIE,  pDeadPlayer, pAttacker, damage_value );

		//�������� �����ô� ��Ȱ �����ش�.
		//���� �������� �״� ��� ����.
		//MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN ��Ŷ�� ���� ��� ���� ���� �ʿ�.
		if( pDeadPlayer->GetGameZoneState() == ePRS_AT_VILLAGE ) Resurrection(pDeadPlayer);
	}
}

eUSER_RELATION_TYPE NomalPart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
	*pErrCode = RC::RC_BATTLE_SUCCESS;

	if( pAttacker == pTarget )			
		return USER_RELATION_FRIEND;									// 0. �ڱ��ڽ������̸� ����(����) ����

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;								// 1. ���� ���ݽ� CanPVP�� ������ �����Ƿ� üũ �ʿ�(Ÿ�ϼӼ�).

	if( pAttacker->GetPartyState() == pTarget->GetPartyState() )		// 2. ��Ƽ�� ���� �Ұ� 
		return USER_RELATION_FRIEND;

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld != 0 && AttkGld == DefGld )	
		return USER_RELATION_FRIEND;									// 3. �����̸� �Ʊ��̴�.

    if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, DefGld) ==
        eGUILD_RELATION_ALLIENCE)
		return USER_RELATION_FRIEND;									// 3_1. ��� ���� �����̸� �Ʊ�.

	return USER_RELATION_ENEMY;											// 4. ���������� ���� ���� ������, ���ܿ� ���ش� ���� ����.
}

//////////////////////////////////////////////////////////////////////////
//���������


//��� ���迡 ���� pk�� ����
// �⺻���� : "��尣 ��������(���뼱��)�� �� ����� ������ �����ϸ�,
//			������ ������ ��븦 �׿��� �г�Ƽ�� ���� ������, ������ ���� ������� �ʴ´�."

//
//��������� ��� B���� �������� �� A�� ������ ����������� �г�Ƽ�� ���� ������
//B�� ������ ���ص� �������� ������� �ʴ´�.

//�ݴ��� ��� B�� �������� �޾� ������ �ʰ�(�ֹ�����), A�� ������ ��� �г�Ƽ�� ������,
//A�� �������� ����ϰ� �ȴ�.

BOOL BoundaryPart::IsPart( const WORD wTileAtt ) 
{ 
    static const BYTE opt = PKScriptParser::Instance()->GetPKOptionInfo().m_IgnorePKTile;

    if( PKOptionInfo::ignore_BoundaryPart & opt )
        return FALSE;

    return FieldPvP::IsPart(wTileAtt);
}

BOOL BoundaryPart::RandomizePercent( Player* pAttacker, Player* pDeadPlayer )
{ 
	// ��� Ȯ���� üũ�Ѵ�.
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	int iLevelDiff = pDeadPlayer->GetLevel() - pAttacker->GetLevel();	BYTE DropRatio = 0;
	if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL )
	{
		// �������� ų���� ������ ����̸� 50% Ȯ���� �����Ų��.
		if( pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
		{
			DropRatio = stPKOptionInfo.m_BadFriendDropRatio;
		}
		else
		{		
#ifdef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM
            // �Ϲ� ���¿����� PK���ص� ������ ����� �ƴҰ�� �������� ������� �ʴ´�
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
	//(lst1024)(080905)(2387)��ȯ���� ���� ��� �г�Ƽ ����.
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

		// ������ ���� ����
		pAttacker->GetChaoState().EndBoradoriState();
		// pTarget�� ������ ����Ʈ�� ��ϵ� ������� ���弭���� ������ ��û�Ѵ�.
		if( pAttacker->GetChaoState().FindBadFriend( pDeadPlayer->GetCharGuid() ) )
		{
			pAttacker->GetChaoState().RequestRemoveBadFriend( pDeadPlayer->GetCharGuid() );
		}
    
    #if !defined(_CHINA) //CHINA_PK_RULE - �߱� ������ ������� �ʴ´�.
        // (lst1024)(090617) �����ڰ� �Ϲ��̰�, ��밡 ����ī���� ���� ī�������� �ø����ʴ´�.
		else if( pAttacker->GetChaoState().GetChaoState() == eCHAO_STATE_NORMAL &&
                 pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_PREV_CHAO ) 
        {}
    #endif
        // ��밡 ī��, ������ ���¶�� ���� ī�������� �ø��� �ʴ´�.
        else if( pDeadPlayer->GetChaoState().GetChaoState() == eCHAO_STATE_CHAO || pDeadPlayer->GetChaoState().IsBoradori() ) 
		{}
        // (lst1024)(090617)ī���� ���� ���� ���￡ ��ȯ���� ������ ����������� ����, ��ȯ���� Ÿ �ɸ��� ���Ͻ� ����� ���ü� ����.
        // 20�� ���ϴ� ��ȣ�ʿ�.
		else if( pAttacker->GetLevel() < stPKOptionInfo.m_PKProtectLV )	
        {}
		else
		{
			// ī�������� �ø���.
			pAttacker->GetChaoState().UpChaoState();

			// ������ ������ ����Ʈ�� �̹� ��ϵ� �������� üũ
			if( !pDeadPlayer->GetChaoState().FindBadFriend( pAttacker->GetCharGuid() ) )
			{
				// ������ ����Ʈ�� ����� ������ ���´�.
				MSG_CG_CHAO_ASK_REVENGE_ADDITION_CMD cmdMsg;
				_tcsncpy( cmdMsg.m_ptszRevengeName, pAttacker->GetCharName(), MAX_CHARNAME_LENGTH );
				cmdMsg.m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
				pDeadPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

				// ������ ����� �̸��� ����� ���´�.
				pDeadPlayer->GetChaoState().SetRevengeName( cmdMsg.m_ptszRevengeName );
			}
		}
		
		// PK �α׸� �����.
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
		return USER_RELATION_FRIEND;								// 0. �ڱ��ڽ������̸� ����(����) ����

	if( !ChkTarget(pTarget,pErrCode) )	
		return USER_RELATION_NEUTRALIST;							// 1. ���� ���ݽ� CanPVP�� ������ �����Ƿ� üũ �ʿ�(Ÿ�ϼӼ�).

	GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld != 0 && AttkGld == DefGld )	
	{
		return USER_RELATION_FRIEND;								// 2. �����̸� �Ʊ��̴�.
	}

	//(lst1024)(080901) ��� ���� ������ ��� �������� �Ұ�.
    if (GuildSmallContentEntry::RelationSystem::GetRelationType(AttkGld, DefGld) ==
        eGUILD_RELATION_ALLIENCE)
		return USER_RELATION_FRIEND;								// 3. ��� ���� �����̸� �Ʊ�.

    if (GuildSmallContentEntry::RelationSystem::IsHostilityRelation(AttkGld, DefGld))
		return USER_RELATION_ENEMY;									// 4. ��� ���� ����� ����.

	if( pAttacker->GetPartyState() == pTarget->GetPartyState() )	
	{
		return USER_RELATION_FRIEND;								// 5. ��Ƽ�� �̸� �Ʊ�.
	}

 #if defined(_CHINA) //CHINA_PK_RULE - �߱� ������ �ڵ� �������� ����.
	if( pTarget->GetChaoState().IsChaoState() )
 #else
	if( pTarget->GetChaoState().IsBoradori() ||
		pTarget->GetChaoState().IsChaoState() )					
 #endif	
		return USER_RELATION_ENEMY;									// 6. ������, ī�������̸� ����.

	if( pAttacker->IsForceAttack() )							//.// ���� ���ݷ�
	{
		if( IsOneSideHostility(pAttacker,pTarget) )				
			return USER_RELATION_ENEMY;								// 7. PK�� ���� ������ �켱 �ϹǷ�, �Ϲ�����ÿ��� ���� ������ �����ϴ�.

		PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
		if( pTarget->GetLevel() < stPKOptionInfo.m_PKProtectLV )	
		{
			*pErrCode = RC::RC_BATTLE_TARGET_LEVEL_IS_LOW;
			return USER_RELATION_NEUTRALIST;						// 8. �ǰ��� ���� üũ
		}

		if( pAttacker->GetLevel() < stPKOptionInfo.m_PKAllowableLV )	 
		{
			*pErrCode = RC::RC_BATTLE_ATTACKER_LEVEL_IS_LOW;
			return USER_RELATION_NEUTRALIST;						// 9. ������ ���� üũ
		}

		return USER_RELATION_ENEMY;									// 10. �Ϲ� ī��
	}

	return USER_RELATION_FRIEND;									// 11. ���ܿ� ���ش� �ϴ� ���� �Ʊ�
}


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//������������

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
        // PK �α׸� �����.
        GAMELOG->LogAction_PK( ACT_DOMINATION_KILL, pAttacker, pDeadPlayer, damage_value );
        GAMELOG->LogAction_PK( ACT_DOMINATION_DIE,  pDeadPlayer, pAttacker, damage_value );
    }
}

eUSER_RELATION_TYPE DominationPart::PvPRule( Player* pAttacker, Player *pTarget, OUT RC::eBATTLE_RESULT* pErrCode )
{
    *pErrCode = RC::RC_BATTLE_SUCCESS;

    if( pAttacker == pTarget )			
        return USER_RELATION_FRIEND;									// 0. �ڱ��ڽ������̸� ����(����) ����

    if( !ChkTarget(pTarget,pErrCode) )	
        return USER_RELATION_NEUTRALIST;								// 1. ���� ���ݽ� CanPVP�� ������ �����Ƿ� üũ �ʿ�(Ÿ�ϼӼ�).

    GUILDGUID AttkGld = pAttacker->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
    GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
    
    if (AttkGld == 0 || DefGld == 0)
        return USER_RELATION_ENEMY;
    else if (AttkGld == DefGld )	
        return USER_RELATION_FRIEND;									// 3. �����̸� �Ʊ��̴�.

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    // 4. �����ڿ� �ǰ����� ���谡 ����� ������ ��尡 �������� ������ ���.. �Ʊ��̴�.
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

    // ���� ��尡 ���� ���..
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

    // ���� ��尡 ���� ���.. (��)
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

    // (NOTE) ������ �ɼ� ��ü�� PvP�����̹Ƿ� ������ �����ϰ� false��ȯ����...
    if(PKOptionInfo::ignore_SafetyPart & opt)
        return false;

    // �ϱ� ������ �� �� ���� �� 'false'�� ������ �ʿ� �ִ�.
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
    // (NOTE) NormalPart�� �����ϴ�.
    *pErrCode = RC::RC_BATTLE_SUCCESS;

    // 0. �ڱ��ڽ������̸� ����(����) ����
    if(pAttacker == pTarget)
        return USER_RELATION_FRIEND;

    // 1. ��Ƽ�� ���� �Ұ� 
    if(pAttacker->GetPartyState() == pTarget->GetPartyState())
        return USER_RELATION_FRIEND;

    GUILDGUID guild_of_attacker = pAttacker->GetCharInfo()->m_GuildGuid;
    if(guild_of_attacker != 0) {
        GUILDGUID guild_of_target = pTarget->GetCharInfo()->m_GuildGuid;
        if(guild_of_target != 0) {
            if(guild_of_attacker == guild_of_target)
                return USER_RELATION_FRIEND;
            // (NOTE) �̰� �����ϰ� �˻����� �ʵ��� �Ѵ�.
            //if(GetRelationType(guild_of_attacker, guild_of_target) == eGUILD_RELATION_ALLIENCE)
            //    return USER_RELATION_FRIEND;
        }
    }

    // (WARNING) �� �� �꿡 ���� ����� �� �ʿ䰡 �ִ�.
    return USER_RELATION_NEUTRALIST;
}
