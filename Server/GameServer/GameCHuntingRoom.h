#pragma once
#include "GameRoom.h"

class CHuntingManager;
class Player;

class GameCHuntingRoom : public GameRoom
{
	__DECL_CUSTOMPOOL_PTR(GameCHuntingRoom)
public:
	GameCHuntingRoom();
	~GameCHuntingRoom();


	virtual void		OnCreate();
	virtual void		OnDestroy();

	inline CHuntingManager *	GetCHuntingManager() { return m_pCHuntingManager;	}

	virtual RC::eSTATUS_RESULT	ResurrectionAtSafezone( Player *pPlayer );

	virtual eZONETYPE	GetZoneType() { return eZONETYPE_CHUNTING;	}
	virtual VOID		Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual VOID		Leave( Player * pPlayer );
	virtual VOID		Update( DWORD dwDeltaTick );

	virtual	VOID		SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	//virtual MAPCODE		GetEntryFieldCode();
	virtual const ADDITIONAL_CHUNTINGINFO *	GetAdditionalCHuntingInfo()	{ return &m_AdditionalInfo; }

	virtual RC::ePARTY_RESULT	WarpAtPartyMaster( Player *pPlayer );
	virtual RC::ePARTY_RESULT	SummonReqAtPartyMaster( Player *pPlayer, DWORD dwObjKey );

	virtual BOOL		IsApplyPenalty();

	virtual eUSER_RELATION_TYPE	IsFriendPVP( Player *pAttacker, Player *pTarget );

	__inline VOID SetAdditionalCHuntingInfo(const ADDITIONAL_CHUNTINGINFO * pInfo) { memcpy(&m_AdditionalInfo, pInfo, sizeof(m_AdditionalInfo)); }

private:
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;
	CHuntingManager *		m_pCHuntingManager;
};
