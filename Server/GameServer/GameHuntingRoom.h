#pragma once
#include "gameroom.h"

class GameHuntingRoom : public GameRoom
{
	__DECL_CUSTOMPOOL_PTR(GameHuntingRoom)
public:
	GameHuntingRoom(void);
	virtual ~GameHuntingRoom(void);

	virtual void		OnCreate();
	virtual void		OnDestroy();

	virtual eZONETYPE	GetZoneType() { return eZONETYPE_HUNTING;	}
	virtual VOID		Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		MoveField( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	VOID 		Leave( Player * pPlayer );

	//virtual MAPCODE		GetEntryFieldCode();
	virtual	VOID		SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );

	virtual const ADDITIONAL_ROOMINFO *	GetAdditionalInfo()	{ return &m_AdditionalInfo; }

	virtual RC::ePARTY_RESULT	WarpAtPartyMaster( Player *pPlayer );
	virtual RC::ePARTY_RESULT	SummonReqAtPartyMaster( Player *pPlayer, DWORD dwObjKey );

	__inline VOID SetAdditionalRoomInfo(const ADDITIONAL_ROOMINFO * pInfo) { memcpy(&m_AdditionalInfo, pInfo, sizeof(ADDITIONAL_ROOMINFO)); }

	virtual eUSER_RELATION_TYPE	IsFriendPVP( Player *pPlayer,Character *pTarget ) {  __UNUSED((pPlayer, pTarget)); return USER_RELATION_NEUTRALIST; }

private:
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
};
