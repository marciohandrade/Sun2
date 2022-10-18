#pragma once

#include "gameroom.h"
#include "MissionRecordManager.h"

#include "RankingSystemCtl.h"
#include <TriggerManagerEx.h>

class GameMissionRoom : public GameRoom
{
	__DECL_CUSTOMPOOL_PTR(GameMissionRoom)
public:
	GameMissionRoom(void);
	virtual ~GameMissionRoom(void);

	virtual void		OnKillMonster( Monster* pMonster );

	virtual void		OnCreate();
	virtual void		OnDestroy();

	virtual eZONETYPE	GetZoneType() { return eZONETYPE_MISSION;	}

	virtual VOID		Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual BOOL		Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual	VOID 		Leave( Player* pPlayer );
	virtual	BOOL		MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );	
	virtual	VOID		LeaveForWarp( Player* pPlayer );

	virtual	VOID		SetZone( Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );

	virtual eUSER_RELATION_TYPE	IsFriendPVP( Player *pPlayer,Player *pTarget ) { __UNUSED((pPlayer, pTarget)); return USER_RELATION_FRIEND; }

#ifdef _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
	TriggerManagerEx::SWITCH_CONTROL SWITCH;
	inline BYTE GetSwitchValue(BYTE index)             { return SWITCH.Get(index); }
	inline void SetSwitchValue(BYTE index, BYTE value) { SWITCH.Set(index, value); }
#endif //_NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
};

class Trigger;
template< class PACKET>
class SendActionDoneTriggers
{
	PACKET m_msg;
public:
	SendActionDoneTriggers(){ m_msg.m_byCount = 0; }
	~SendActionDoneTriggers(){}

	BOOL SendInfo( Player* pPlayer )
	{
		if( m_msg.m_byCount != 0 )
			return pPlayer->SendPacket( &m_msg, m_msg.GetSize() );
		return TRUE;
	}
	void operator()( Trigger* pTrigger )
	{
		if( pTrigger->GetTriggerState() == Trigger::TERMINATING_STATE )
		{
			if( pTrigger->IsAlwaysTrigger() ) return;

			ASSERT( m_msg.m_byCount < PACKET::MAX_DONE_TRIGGER_ID_NUM );
			if( m_msg.m_byCount >= PACKET::MAX_DONE_TRIGGER_ID_NUM ) return;
			m_msg.m_dwDoneTriggerID[m_msg.m_byCount++] = pTrigger->TriggerID();
		}
	}
};
