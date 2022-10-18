#include "StdAfx.h"
#include ".\GameHuntingField.h"
#include "Player.h"
#include "Map.h"
#include <MapInfoParser.h>
#include "UnitRegenInfoManager.h"
#include "ResrcManager.h"

__IMPL_CUSTOMPOOL_PTR(GameHuntingField)

GameHuntingField::GameHuntingField(void) 
    : FieldPvPRule(PTA_PK_TILE | PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE |
                   PTA_SAFETY_ZONE) //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
{
}

GameHuntingField::~GameHuntingField(void)
{
}

void GameHuntingField::OnCreate()
{
	ChangeHandler( eZONETYPE_FIELD );
	GameZone::OnCreate();
}
void GameHuntingField::OnDestroy()
{
}

VOID GameHuntingField::Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	DEBUG_CODE(
		GameZone * pEnterZone	= NULL;
	MAPCODE EnterFieldCode	= 0;
	eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
	ASSERT( !(eRoomState & ePRS_AT_ZONE) );
	);

	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

void GameHuntingField::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameHuntingField::Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_FIELD ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameHuntingField::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameZone::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
		return FALSE;
	}

	pPlayer->SaveStartLocation();

	return TRUE;
}

BOOL GameHuntingField::MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	DEBUG_CODE(
		eZONE_STATE eRoomState	= pPlayer->GetGameZoneState();
	ASSERT( (eRoomState & ePRS_BEFORE_ENTER_FIELD) );
	);

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if( !GameZone::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}

	pPlayer->SaveStartLocation();

	return TRUE;
}

VOID GameHuntingField::Leave( Player * pPlayer )
{
	pPlayer->SaveStartLocation();

	GameZone::Leave( pPlayer );
}


VOID GameHuntingField::GetArea( GameField* pField, Player* pPlayer, MAPCODE FieldCode, WzID& AreaID, WzVector& rwvStartPos )
{
	const sMAPINFO* pMAPINFO = MapInfoParser::Instance()->FindMapInfo( FieldCode );
	eZONETYPE ztZONE = (eZONETYPE)pMAPINFO->byMKind;

	if( eZONETYPE_TUTORIAL_FIELD == ztZONE )
	{
		// AreaID : WzID --> 이 값을 고정설정하자. 해당 맵 정보를 읽어들여서
		// GetFixedStartArea...
		eCHAR_TYPE eCharType = pPlayer->GetCharType();
		switch( eCharType )
		{
		case eCHAR_BERSERKER:
			AreaID = CharToWzID( 'S', 'T', 'R', '0' );
			break;
		case eCHAR_DRAGON:
			AreaID = CharToWzID( 'S', 'T', 'R', '1' );
			break;
		case eCHAR_SHADOW:
			AreaID = CharToWzID( 'S', 'T', 'R', '2' );
			break;
		case eCHAR_VALKYRIE:
			AreaID = CharToWzID( 'S', 'T', 'R', '3' );
			break;
		case eCHAR_ELEMENTALIST:	//eCHAR_MAGICIAN
			AreaID = CharToWzID( 'S', 'T', 'R', '4' );
			break;
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            AreaID = CharToWzID('S', 'T', 'R', '5');
            break;
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            AreaID = CharToWzID('S', 'T', 'R', '6');
            break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            AreaID = CharToWzID('S', 'T', 'R', '7');
            break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

			//case eCHAR_BERSERKER:
			//	AreaID = CharToWzID( 'E', 'N', 'T', '1' );
			//	break;
			//case eCHAR_DRAGON:
			//	AreaID = CharToWzID( 'E', 'N', 'T', '1' );
			//	break;
			//case eCHAR_SHADOW:
			//	AreaID = CharToWzID( 'E', 'N', 'T', '2' );
			//	break;
			//case eCHAR_VALKYRIE:
			//	AreaID = CharToWzID( 'E', 'N', 'T', '3' );
			//	break;
			//case eCHAR_ELEMENTALIST:	//eCHAR_MAGICIAN
			//	AreaID = CharToWzID( 'E', 'N', 'T', '4' );
			//	break;
		}
		//pZone->GetFixedStartingVector( (BYTE)eCharType );
	}

	pField->GetAreaRandomStartingVector(AreaID, rwvStartPos);
}

struct AREA_DESTPOS
{
	float		m_fDistance;
	WzVector	m_vDestPos;
};

bool AreaSortComp( const AREA_DESTPOS & AreaPos1, const AREA_DESTPOS & AreaPos2 )
{
	// 거리가 가까운 쪽이 순위가 높다.
	if( AreaPos1.m_fDistance < AreaPos2.m_fDistance )
		return true;
	else
		return false;
}

RC::eSTATUS_RESULT GameHuntingField::ResurrectionAtSafezone( Player *pPlayer )
{
	//죽은상태가 아니면 리턴한다.
	if( pPlayer->IsAlive() )
		return RC::RC_STATUS_CHAR_IS_ALIVE;

	GameField *pField = pPlayer->GetField();
	if( !pField )
		return RC::RC_STATUS_FIELD_IS_NULL;

	DWORD dwAreaID = 0;	WzVector vCurPos, vDestPos; 
	pPlayer->GetPos( &vCurPos );

	static STLX_LIST<AREA_DESTPOS> m_listSortArea;	m_listSortArea.clear();
	TCHAR tszWzID[MAX_AREA_ID_LENGTH+1];
	for( int i = 0; i < m_pMAPINFO->byENTCount; i++ )
	{
		_sntprintf( tszWzID, MAX_AREA_ID_LENGTH, "ENT%d", i+1 );
		tszWzID[MAX_AREA_ID_LENGTH] = '\0';
		if( pField->GetAreaRandomStartingVector( StrToWzID( tszWzID ), vDestPos ) )
		{
			AREA_DESTPOS AreaDestPos;
			AreaDestPos.m_vDestPos = vDestPos;
			AreaDestPos.m_fDistance = VectorLength( &(vDestPos - vCurPos) );
			m_listSortArea.push_back( AreaDestPos );
		}
	}
	m_listSortArea.sort( AreaSortComp );
	vDestPos = m_listSortArea.begin()->m_vDestPos;

	// 시작위치로 텔레포트 시킨후
	if( !pField->TeleportObject( pPlayer, &vDestPos ) )
		return RC::RC_STATUS_TELEPORT_FAIL;

	if( !pPlayer->OnResurrection( 0, 0.15f, 0.15f ) )
	{
		return RC::RC_STATUS_RESURRECTION_FAIL;
	}

	return RC::RC_STATUS_SUCCESS;
}

BOOL GameHuntingField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode )
{
    return FieldPvPRule.CanPVP(pAttacker,pTarget,pErrCode);
}

BOOL GameHuntingField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode ) 
{
    return FieldPvPRule.CanPVP(pAttacker,pTarget,pErrCode); 
}

eUSER_RELATION_TYPE	GameHuntingField::IsFriendPVP( Player* pAttacker,Player* pTarget )
{
    return FieldPvPRule.IsFriendPVP(pAttacker,pTarget); 
}

//_NA_008706_20160229_TEXTLOG_PK
VOID GameHuntingField::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ ) 
{
    FieldPvPRule.KillPlayer(pAttacker,pDeadPlayer,bSummoned, damage_value); 
}

BOOL GameHuntingField::IsPvPAtt( Player* pPlayer, WORD PTATroubld) 
{
    return FieldPvPRule.IsPvPAtt(pPlayer, PTATroubld); 
}
