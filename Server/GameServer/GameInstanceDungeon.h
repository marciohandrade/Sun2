//2007. 01. 12 
//작업자 : 김창현
//메일   : shogen@webzen.co.kr
//내용   : 게임서버의 인스턴스 던전

#pragma once

#include "GameRoom.h"

class RankingSystemCtl;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/ChaosZoneManager.h>
#include <World/ChaosZoneSystem/BattleGround.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameInstanceDungeon>
//
class GameInstanceDungeon : public GameRoom
{
	__DECL_CUSTOMPOOL_PTR(GameInstanceDungeon)
public:		GameInstanceDungeon(void);
	virtual	~GameInstanceDungeon(void);	

	virtual	void				OnCreate();
	virtual	void				OnDestroy();
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    virtual void                Update(DWORD dwDeltaTick);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	virtual	eZONETYPE			GetZoneType() { return eZONETYPE_INSTANCE;	}

	virtual	VOID				Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual	BOOL				Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual	VOID 				Leave( Player* pPlayer );
	virtual	BOOL				MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );	
	virtual	VOID				LeaveForWarp( Player* pPlayer );

	virtual	VOID				SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual	void				OnKillMonster( Monster* pMonster );
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eUSER_RELATION_TYPE	IsFriendPVP( Player *pPlayer, Player *pTarget )
    {
        if (chaos_zone_manager_)
        {
            return chaos_zone_manager_->IsFriendPVP(pPlayer, pTarget);
        }
        return USER_RELATION_FRIEND;
    }
#else
    virtual eUSER_RELATION_TYPE	IsFriendPVP( Player *pPlayer, Player *pTarget ) { __UNUSED((pPlayer, pTarget)); return USER_RELATION_FRIEND; }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008334_20150608_SONNENSCHEIN
    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone(Player* pPlayer);
#endif //_NA_008334_20150608_SONNENSCHEIN

public:
	inline	VOID				SetTypeOfIndun( const eINSTANCE_DUNGEON_KIND typeOfIndun ) { m_KindOfIndun = typeOfIndun; }
	inline	eINSTANCE_DUNGEON_KIND		GetTypeOfIndun() { return m_KindOfIndun; }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    inline void SetChaosZoneType(ChaosZoneType chaos_zone_type) {chaos_zone_type_ = chaos_zone_type;}
    inline ChaosZoneType GetChaosZoneType() const { return chaos_zone_type_; };
    inline ChaosZoneManager* GetChaosZoneManager() const { return chaos_zone_manager_; };
    virtual GameBattleResult* GetGameBattleResult() const;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


private:
	eINSTANCE_DUNGEON_KIND		m_KindOfIndun;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneType chaos_zone_type_;
    ChaosZoneManager* chaos_zone_manager_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};


template< eINSTANCE_DUNGEON_KIND _INDUNKIND >
struct GameInstanceDungeon_Info : sun_type_info<GameInstanceDungeon>
{
	static const eINSTANCE_DUNGEON_KIND		value = _INDUNKIND;
};

typedef GameInstanceDungeon_Info<INDUN_KIND_TOP_OF_TRIAL>	IndunTopOfTrial_Info;
typedef GameInstanceDungeon_Info<INDUN_KIND_SS_QUEST>		IndunSSQField_Info;
typedef GameInstanceDungeon_Info<INDUN_KIND_100LEVEL_QUEST>	Indun100LevelQuest_Info;

/*
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
*/