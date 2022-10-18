#pragma once

#include "NPC.h"
#include "AppliedNpcDependencyChain.h"

class MapNpc : public NPC
{
public:
	MapNpc();
	virtual ~MapNpc();

	void InitMapNPC(WORD NPCCode, MAPOBJECT_INFO *pMapInfo);
    DWORD GetMapObjectID() { return m_pMapObjectInfo->dwMapObjectID; }

	virtual BOOL Update(DWORD dwDeltaTick);
	virtual VOID Reuse() { Release(); }

	virtual VOID OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
	virtual VOID OnLeaveField();
    virtual VOID OnEnterObject( const Sector & IN rSector );
    virtual VOID OnDead();
    virtual eUSER_RELATION_TYPE	IsFriend( Character *pTarget );

protected:
    virtual void _Damaged(DamageArgs* damage_arg);

protected:
    virtual BOOL Init();
	virtual VOID Release();

    __inline VOID SetMapObjectInfo(MAPOBJECT_INFO *pMapObjectInfo) { m_pMapObjectInfo = pMapObjectInfo; }


private:
    eUSER_RELATION_TYPE	IsFriend_FriendMapNpc(Character* pTarget);
    eUSER_RELATION_TYPE	IsFriend_DominationMapNpc(Character* pTarget);
    //
    // fields...
	INpcLinkActionD			m_LinkedAction;
	MAPOBJECT_INFO*			m_pMapObjectInfo;
};
