#pragma once
#include "gameroom.h"

class GameField;
class Player;
class PVPManager;

class GamePVPRoom : public GameRoom
{
	enum { MAX_PVP_TEAM_NUM = 10, };

	__DECL_CUSTOMPOOL_PTR(GamePVPRoom)
public:
	GamePVPRoom(void);
	virtual ~GamePVPRoom(void);

	virtual void OnCreate();
	virtual void OnDestroy();

	PVPManager*	GetPVPManager();

    void SetAdditionalPVPInfo(
        const ADDITIONAL_PVPINFO* pInfo
        );

	virtual eZONETYPE GetZoneType();

	virtual void Ready( 
        Player* pPlayer, 
        MAPCODE FieldCode, 
        WzID AreaID, 
        WzVector* pwvStartPos 
        );

	virtual BOOL Join( 
        Player* pPlayer, 
        MAPCODE FieldCode, 
        WzID AreaID, 
        WzVector* pwvStartPos 
        );

	virtual BOOL MoveField( 
        Player* pPlayer, 
        MAPCODE FieldCode, 
        WzID AreaID, 
        WzVector* pwvStartPos 
        );

	virtual void Leave( 
        Player* pPlayer 
        );

	virtual void Update( 
        DWORD dwDeltaTick 
        );

	virtual void ChangeMaster(
        DWORD dwNextMasterUserKey
        );

	virtual	void SetZone( 
        Object* pObject, 
        MAPCODE FieldCode, 
        WzID AreaID, 
        WzVector* pwvStartPos 
        );

	virtual eUSER_RELATION_TYPE	IsFriendPVP(
        Player* pAttacker,
        Player* pTarget 
        );

    virtual const ADDITIONAL_PVPINFO* GetAdditionalPVPInfo();

    virtual BOOL IsApplyPenalty();

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual GameBattleResult* GetBattleResult();

private:
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;
	PVPManager*		m_pPVPManager;
};

inline PVPManager* 
GamePVPRoom::GetPVPManager() 
{ 
    return m_pPVPManager;	
}

inline void 
GamePVPRoom::SetAdditionalPVPInfo(const ADDITIONAL_PVPINFO * pInfo) 
{ 
    memcpy(&m_AdditionalPVPInfo, pInfo, sizeof(ADDITIONAL_PVPINFO)); 
}