#pragma once

#include "gamezone.h"
#include "FieldPvP.h"

class GameHuntingField : public GameZone
{
	__DECL_CUSTOMPOOL_PTR(GameHuntingField)
public:
	GameHuntingField(void);
	virtual ~GameHuntingField(void);

	virtual void		OnCreate();
	virtual void		OnDestroy();

	virtual eZONETYPE	GetZoneType() { return eZONETYPE_FIELD;	}

	virtual void		Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual RC::eSTATUS_RESULT		ResurrectionAtSafezone( Player *pPlayer );

	virtual	void		SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	void 		Leave( Player * pPlayer );

	virtual void		GetArea( GameField* pField, Player* pPlayer, MAPCODE FieldCode, WzID& AreaID, WzVector& rwvStartPos );

public:
	virtual BOOL CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode );
	virtual BOOL CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode );

	virtual eUSER_RELATION_TYPE	IsFriendPVP( Player* pAttacker,Player* pTarget );

    //_NA_008706_20160229_TEXTLOG_PK
    virtual void KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );
	virtual BOOL IsPvPAtt( Player* pPlayer, WORD PTATroubld);

private:
	FieldPvPRuleManager FieldPvPRule;

};
