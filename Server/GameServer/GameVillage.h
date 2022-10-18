#pragma once

#include "gamezone.h"
#include "FieldPvP.h"


class GameVillage : public GameZone
{
public:		GameVillage(void);
	virtual	~GameVillage(void);

	virtual	eZONETYPE	GetZoneType() { return eZONETYPE_VILLAGE;	}
	virtual	void		OnCreate();

	virtual	void		Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	BOOL		Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	void		SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	void 		Leave( Player * pPlayer );

public:
	virtual	BOOL        CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT*  pErrCode ); 
	virtual	BOOL        CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT*   pErrCode ); 

	virtual	eUSER_RELATION_TYPE	IsFriendPVP( Player* pAttacker,Player* pTarget ); 
	virtual	void        KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0 );

private:
    FieldPvPRuleManager FieldPvPRule;

	__DECL_CUSTOMPOOL_PTR(GameVillage)
};
