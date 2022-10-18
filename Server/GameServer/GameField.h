#pragma once

//=======================================================================================================================
/// GameField class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 2. 20~
	@history
			- 2006/8/5 : ObjectManger통합 작업
	@todo 
			- MapObject의 ObjectKey 정리, 다른 object와 겹치지 않게
							
*/
//=======================================================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#include "Player.h"
#include "Field.h"
#include "Sector.h"
#include "SectorInfo.h"
#include <World/HighLoadBalancer.h>	//__NA001144_080828_HIGH_RESOLUTION_SECTOR_LOAD_BALANCE_POS_SYNC__
#include "UnitRegenManager.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class EffectManager;
class UnitRegenManager;
class UnitRegenInfoManager;
class UnitGroupManager;
class UnitGroupInfoManager;
class UnitRegenInfo;
class RegenLocation;
class Monster;
class TotemNpc;
class TriggerManagerEx;
class Collection;
class CollectionManager;
struct IsSendPlayer;
class CrystalWarp;
//
//_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER 
struct TargetFinderArg;
struct IObjectFinder;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameField>
//
class GameField : public Field
{
	__DECL_CUSTOMPOOL_PTR(GameField)

	enum {
		DEFAULT_PLAYER_POOL_SIZE	= 10,
		DEFAULT_NPC_POOL_SIZE		= 70,
		DEFAULT_MAPOBJECT_POOL_SIZE = 100,
		DEFAULT_FIELDITEM_POOL_SIZE = 20,
		DEFAULT_DEADOBJECT_POOL_SIZE = 20,
	};

#ifdef _NA_0_20100416_RELOAD_MAPOBJECT
    friend class ZoneReloader;
#endif

public:		GameField();
public:
	virtual	~GameField();

	virtual	VOID				Release();
	virtual	VOID				Init(
									GameZone*				pZone
									, FieldInfo*			pFieldInfo
									, const TCHAR*			pszGroupName
									, UnitRegenInfoManager*	pUnitRegenInfoMgr
									, UnitGroupInfoManager*	pUnitGroupInfoMgr
									, bool					bPreMapLoading
									, bool					bUsingViewPort
									, bool					bPreRegen
									);
	virtual	VOID				Update( DWORD dwDeltaTick );

	// 오브젝트 입장, 퇴장
	virtual	BOOL				EnterField( Object* pObject, const WzVector* pwzVec, int iAngle=-1 );
	virtual	BOOL				LeaveField( Object* pObject );

//────────────────────────────────────────────────────────────────
//
    const SectorGridVR* GetSectorGridVR(
        const SECTOR_IDX coordinates, const float radius) const;

    template<typename _MSGSUBTYPE>
    void SendPacketAroundForSync(Character* const sender, const _MSGSUBTYPE* const sub_msg, const BOOLEAN bPostAction = false);
    template<typename _MSGSUBTYPE>
    void SendPacketAroundForSync(const SECTOR_IDX coordinates, const _MSGSUBTYPE* const sub_msg);
private:	VOID SendPacketAroundForSync_(const SECTOR_IDX coordinates, const BYTE _TAG, WAR_TAG_V* const IN pMsg, const WORD wSize, const BOOLEAN bPostAction = FALSE);
public:
	inline	VOID SendPacket( Player* const pReceiver, MSG_BASE_FORWARD* pMsg, WORD wSize );
			VOID SendExPacket( Player* const pReceiver, const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize );

	inline	VOID SendPacketAround( const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize, const DWORD except_objkey = 0 );
			VOID SendExPacketAround( const SECTOR_IDX sector_idx, const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize, const DWORD except_objkey = 0 );

	inline	VOID SendPacketToAll( MSG_BASE_FORWARD* pMsg, WORD wSize, const DWORD except_objkey = 0 );
			VOID SendExPacketToAll( const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize, const DWORD except_objkey = 0 );

  //{ __NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__
    // (WAVERIX) (090716) (ADD-IN) for High-load balanced network support
    inline  VOID SendPacketToAgent(MSG_BASE_FORWARD* pMsg, WORD wSize);
            VOID SendExPacketToAgent(const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize);
  //}
			VOID SendAIMessageAroundExceptMe( NPC* const pSender, AI_MSG* const pMsg, const WORD wSize, const DWORD delay = 0 );

    inline void SendPacketToSector(const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize,
                                   const DWORD except_objkey = 0);
    void SendExPacketToSector(const SECTOR_IDX sector_idx,
                              const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize,
                              const DWORD except_objkey = 0);
    // (CHANGES) (f100504.1L) change the interface specification to support a complex condition
    // the replacement facility for SendXXPacketToSector<IsSendPlayer>
    inline void SendPacketToSector(const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize,
                                   IObjectFinder* finder);
    void SendExPacketToSector(const SECTOR_IDX sector_idx,
                              const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize,
                              IObjectFinder* finder);
//
//────────────────────────────────────────────────────────────────
//
			BOOL				TeleportObject( Character* const pCharacter, const WzVector* pPos, const BOOLEAN bForcedCommand=FALSE );
			RC::eSYNC_RESULT	JumpObject( Character* const pCharacter, const WzVector* const pPos );
public:
	inline	EffectManager*		GetEffectManager()	{ return m_pEffectManager; }
	inline	TriggerManagerEx*	GetTriggerManager()	{ return m_pTriggerManager; }
	inline	CollectionManager* GetCollectManeger() { return m_pCollectionManager; }
            Character*          SearchTarget(const TargetFinderArg& find_arg);

public:
    // CHANGES: f110303.1L, added new interface for enhanced target searching.
    // added by _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    bool FindTargetsForNpcAI(
        const ns_object::SearchOption::NpcAI& search_option,
        ns_object::SearchOption::NpcAI::result_type* const search_result);
    bool FindTargetsUsingSingleSearch(
        const ns_object::SearchOption::SingleSearch& search_option,
        ns_object::SearchOption::SingleSearch::result_type* const search_result);
public:
    // 주변 섹터 루프
    // (CHANGES) (f100503.3L) change the interface specification so that finders is very rare
    NPC* SearchNeighborNpc(SECTOR_IDX sector_index, DWORD object_key) const;
    NPC* SearchNeighborNpc(SECTOR_IDX sector_index, const IObjectFinder* filter) const;
    Item* SearchNeighborItem(SECTOR_IDX sector_index, DWORD object_key) const;
    Item* SearchNeighborItem(SECTOR_IDX sector_index, const IObjectFinder* filter) const;
public:
	inline	UnitGroupManager*	GetUnitGroupMgr() { return m_pUnitGroupMgr; }
	inline	UnitRegenManager*	GetUnitRegenMgr() { return m_pUnitRegenManager; }
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    ns_trigger::TriggerNpcLinkMatchTable* GetNpcTriggerEvents() const;

public:		void				DestroyObject( Object* pObj );
private:	VOID				AddMapObjectAll();
			void				_AddObject( Object* pObj );
			bool				_DeleteObject( Object* pObj );
            void                _LinkedMapNpc(Object* pObj);
            void                _UnLinkedMapNpc(Object* pObj);
			VOID				SpawnSector( DWORD dwSectorIndex, Object* pObject );
public:		VOID				BuildInstanceInfo();	// Player 최초 입장시 초기화 하는 것들.
private:

// 몬스터 관련
public:
	typedef STLX_HASH_MAP<DWORD, Character*>	_CHARACTER_HASH;
	typedef STLX_HASH_MAP<DWORD, Player*>		_PLAYER_HASH;
	typedef STLX_HASH_MAP<DWORD, NPC*>			_NPC_HASH;
	typedef STLX_HASH_MAP<DWORD, Item*>			_ITEM_HASH;
	typedef STLX_HASH_MAP<DWORD, Collection*>	_COLLECTION_HASH;
	typedef STLX_HASH_MAP<DWORD, MapObject*>	_MAPOBJECT_HASH;

public:
	inline const _CHARACTER_HASH&		GetRefCharacterList() const	{ return m_CharacterHash; }
	inline const _PLAYER_HASH&			GetRefPlayerList() const	{ return m_PlayerHash; }
	inline const _NPC_HASH&				GetRefNPCList() const		{ return m_NPCHash; }
			VOID				AddExpAllMember( DWORD64 Exp );

private:	__PTR_PROPERTY( GameZone, GameZone );
private:	EffectManager*				m_pEffectManager;
			TriggerManagerEx*			m_pTriggerManager;

			UnitRegenManager*			m_pUnitRegenManager;
			UnitGroupManager*			m_pUnitGroupMgr;
			UnitGroupInfoManager*		m_pUnitGroupInfoMgr;
			CollectionManager*			m_pCollectionManager;
            //
            bool m_bPreRegen;
            bool instanced_resourse_;
    static const DWORD  SECTOR_SYNC_UPDATE_INTERVAL;
	struct {
			ITimerBase					m_SyncUpdater;
			HLB_BufferInfo*				m_pHLB;
	};		friend class HighLoadBalancer;

private:	_CHARACTER_HASH				m_CharacterHash;
			_PLAYER_HASH				m_PlayerHash;		// Player의 생성주기는 GameField와 같이하지 않는다.
			_NPC_HASH					m_NPCHash;			// NPC, Item, MapObject들은 GameField가 사라질 때 같이 소멸된다.
			_NPC_HASH					m_NPCHashForUnitID;
			_ITEM_HASH					m_FieldItemHash;
			_COLLECTION_HASH			m_FieldCollectHash;
			_MAPOBJECT_HASH				m_MapObjectHash;
            // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
            ns_trigger::TriggerNpcLinkMatchTable* npc_trigger_events_;
			TCHAR						m_tszTriggerGroupName[MAX_MAPNAME_LENGTH+1];

public:		
    // 필드내 모두 루프
    template<typename OPERATOR_T> 
    void ForAllPlayer(OPERATOR_T& op)
    { 
        for (_PLAYER_HASH::iterator it = m_PlayerHash.begin(); it != m_PlayerHash.end(); )
        {
            Player* const player = it->second;
            if (obj_util::IsValid(player, __FUNCTION__, __LINE__))
            {
                op(player);
                ++it;
            }
            else
            {
                // 맵과 백터 구분 주의
                m_PlayerHash.erase(it++);
            }
        }
    }

    template<typename OPERATOR_T> 
    void ForAllNPC(OPERATOR_T& op)
    { 
        for (_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end(); )
        {
            NPC* const npc = it->second;
            if (obj_util::IsValid(npc, __FUNCTION__, __LINE__))
            {
                op(npc);
                ++it;
            }
            else
            {
                // 맵과 백터 구분 주의
                m_NPCHash.erase(it++);
            }
        }
    }

    MapNpc* FindMapNpc(const Object::Key object_key) const;
	
    MapObject* FindMapObject(const Object::Key object_key) const;
    
    Character* FindCharacter(const Object::Key object_key) const;

	NPC* FindNPCForUnitID(const DWORD unit_id) const;
    
    Object* FindObject(const eOBJECT_TYPE object_type, const Object::Key object_key) const;

public:
	virtual	int					GetPlayerNum()		{ return (int)m_PlayerHash.size(); }
	int GetNPCNum()	const 
    { 
        return (int)m_NPCHash.size(); 
    }
	
    int GetDeadNPCNum() const
    { 
        return (int)m_pUnitRegenManager->GetCount(); 
    }

			// 리젠을 이용한 몬스터 생성

			// 최적화 필요 함수들!!!!

public:		Monster*			CreateMonster( WORD NPCCode, const WzVector* pvPos, 
									WORD wGroupID=0, int iAngle=-1, BYTE byMoveType=0, DWORD dwMoveAreaID=0, WORD MissionAreaID=0 );
			Summoned*			CreateSummonByPlayer(  WORD NPCCode, const WzVector* pvPos, 
									Character* pSummoner, SummonedNPCs* pOwnerSummonNPCs, DWORD dwExpireTime, SLOTCODE SkillCode, const PLAYER_SUMMON_INFO* const summon_info);
			Summoned*			CreateSummonDeadMonster( WORD NPCCode, const WzVector* pvPos, Character *pSummoner, 
									SummonedNPCs* pOwnerSummonNPCs, SLOTCODE SkillCode );
            CrystalWarp*        CreateSummonCrystalWarp( WORD NPCCode, const WzVector* pvPos,
                                                         DWORD dwGuildGuid, DWORD dwExpireTime );
			Monster*			CreateSummonByNPC( WORD NPCCode, const WzVector* pvPos, 
									Character* pSummoner, SummonedNPCs* pOwnerSummonNPCs );
			MapNpc*				CreateMapNPC( WORD NPCCode, const WzVector* pvPos, MAPOBJECT_INFO* pMapInfo );

			TotemNpc*			CreateTotemNPC( WORD NPCCode, const WzVector* pvPos, Player* pSummoner, SCItemSlot & IN slot );
			Collection*			CreateCollection( MAPOBJECT_INFO *pMapInfo );
			Monster*			SpawnMonster( UnitRegenInfo* pUnitRegenInfo );

			Item*				CreateItem( const WzVector* pvPos, DWORD dwPlayerKey, DWORD dwFromMonsterKey, SCItemSlot & slot );
            Item* CreateMoney(const WzVector* creating_pos,
                DWORD owner_key, DWORD from_monster_key, const MONEY& money);

            BOOL	            KillNPC(Player* pPlayer, NPC* pNPC, BOOL bIsDrop, BOOLEAN bBrd = TRUE);
            BOOL                GmDamageNPC(Player* pPlayer, NPC* pNPC, const FLOAT fDamagePercent);
			VOID				KillAllNPC( Player* pPlayer, int iRange, const FLOAT fDamagePercent );
			VOID				KillAllItem( Player* pPlayer, int iRange );
            VOID				KillOneMonster( NPC* pNPC, Player* pPlayer, BOOL IsExpItemDrop );
            VOID				KillMonster( MONSTERCODE MonsterCode, Player* pPlayer, BOOL IsExpItemDrop );
            // add a function to support gm command 'pickup'. the pick up event has many events \
            // even though the all acquisition by gm command is a test.
            bool PickupAllItems(Player* gm_player, bool use_duplicable_slot_mode);
private:	Monster*			AllocMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID );
};



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

inline VOID
GameField::SendPacketAround( const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize, const DWORD except_objkey )
{
	SendExPacketAround( sector_idx, 1, reinterpret_cast<BYTE**>(&pMsg), &wSize, except_objkey );
}

inline VOID
GameField::SendPacketToAll( MSG_BASE_FORWARD* pMsg, WORD wSize, const DWORD except_objkey )
{
	SendExPacketToAll( 1, reinterpret_cast<BYTE**>(&pMsg), &wSize, except_objkey );
}

//__NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__
// (WAVERIX) (090716) (ADD-IN) for High-load balanced network support
inline VOID
GameField::SendPacketToAgent(MSG_BASE_FORWARD* pMsg, WORD wSize)
{
    SendExPacketToAgent(1, reinterpret_cast<BYTE**>(&pMsg), &wSize);
}

template< typename _MSGSUBTYPE >
inline void
GameField::SendPacketAroundForSync(Character* const sender, const _MSGSUBTYPE* const sub_msg,
                                   const BOOLEAN post_action)
{
    if (sender == NULL) {
        return;
    }
    const BYTE _TAG = sub_msg->value;
    SendPacketAroundForSync_(sender->GetSectorIndex(), _TAG,
                             static_cast<WAR_TAG_V*>(const_cast<_MSGSUBTYPE*>(sub_msg)),
                             sub_msg->GetSize(),
                             post_action);
}

// f110106.1L, added by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
template< typename _MSGSUBTYPE >
inline void
GameField::SendPacketAroundForSync(const SECTOR_IDX coordinates, const _MSGSUBTYPE* const sub_msg)
{
    const BYTE _TAG = sub_msg->value;
    SendPacketAroundForSync_(coordinates, _TAG,
                             static_cast<WAR_TAG_V*>(const_cast<_MSGSUBTYPE*>(sub_msg)),
                             sub_msg->GetSize(),
                             false);
}

inline VOID
GameField::SendPacket( Player* const pReceiver, MSG_BASE_FORWARD* pMsg, WORD wSize )
{
	SendExPacket( pReceiver, 1, (BYTE**)&pMsg, &wSize );
}


inline void
GameField::SendPacketToSector(const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize,
                              const DWORD except_objkey)
{
	SendExPacketToSector(sector_idx, 1, reinterpret_cast<BYTE**>(&pMsg), &wSize, except_objkey);
}

// (CHANGES) (f100504.1L) change the interface specification to support a complex condition
// the replacement facility for SendXXPacketToSector<IsSendPlayer>
inline void
GameField::SendPacketToSector(const SECTOR_IDX sector_idx, MSG_BASE_FORWARD* pMsg, WORD wSize,
                              IObjectFinder* finder)
{
    SendExPacketToSector(sector_idx, 1, reinterpret_cast<BYTE**>(&pMsg), &wSize, finder);
}

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
inline ns_trigger::TriggerNpcLinkMatchTable* GameField::GetNpcTriggerEvents() const {
    return npc_trigger_events_;
};