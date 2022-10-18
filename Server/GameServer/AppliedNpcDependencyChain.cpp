#include "stdafx.h"
#include "AppliedNpcDependencyChain.h"
#include <SunEventInfoParser.h>
#include <PublicMath.h>
#include <QuestScriptLoader.h>
#include "ItemManager.h"
#include "Monster.h"
#include "MapNpc.h"
#include "TriggerManagerEx.h"
#include "QuestManager.h"
#include "QuestManager_Concrete.h"
#include "GameZone.h"
#include <AreaConquestCommonHeader.h>
#include "GameInstanceDungeon.h"
#include <World/GameWarControlManager.h>

#include "GameParty/NewGamePartyManager.h"
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
#include "NPCReplaceInfoParser.h"
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#include "MonsterBookParser.h" //_NA_007086_20140318_MONSTERBOOK

//==================================================================================================
//	<AppliedNpcDependencyChain>
WORK_KEY						AppliedNpcDependencyChain::ms_WorkKey = 0;
AppliedNpcDependencyChain*		AppliedNpcDependencyChain::ms_pInstance = NULL;


AppliedNpcDependencyChain::AppliedNpcDependencyChain()
{
	ms_pInstance = this;
}

AppliedNpcDependencyChain::~AppliedNpcDependencyChain()
{
	ms_pInstance = NULL;
}

BOOL
AppliedNpcDependencyChain::DoInit()
{
	m_UpdateTimer.SetTimer(INTERVALforUPDATE);

	//AddPolicyNpcLinkItemDelete( LINK_ITEM_DELETE00_NPCCODE, LINK_ITEM_DELETE00_ITEMCODE );
	//AddPolicyNpcLinkItemDelete( LINK_ITEM_DELETE01_NPCCODE, LINK_ITEM_DELETE01_ITEMCODE );
	//AddPolicyNpcLinkItemDelete( LINK_ITEM_DELETE02_NPCCODE, LINK_ITEM_DELETE02_ITEMCODE );
	//AddPolicyNpcLinkItemDelete( LINK_ITEM_DELETE03_NPCCODE, LINK_ITEM_DELETE03_ITEMCODE );

	return TRUE;
}

BOOL
AppliedNpcDependencyChain::DoProcess()
{
	if( m_UpdateTimer.IsExpired() )
	{
	}
	return TRUE;
}

BOOL
AppliedNpcDependencyChain::IsComplete()
{
	return FALSE;
}

BOOL
AppliedNpcDependencyChain::DoRelease()
{
	return TRUE;
}

VOID
AppliedNpcDependencyChain::OnMsg( sSIG_MSG* pSIG_MSG )
{
    __UNUSED(pSIG_MSG);
	return;
}


//==================================================================================================
//	NpcAction
class IMonsterLinkActions : public INpcLinkAction
{
public:
	virtual		~IMonsterLinkActions() {}
	virtual		BOOL		OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
	{
		return AppliedNpcDependencyChain::Instance()->OnMonsterDeadProcess( pField, pNPC, pOwner, killerObjKey );
	}
};

class IMapNpcLinkActions : public INpcLinkAction
{
public:
	virtual		~IMapNpcLinkActions() {}
	virtual		BOOL		OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
	{
		return AppliedNpcDependencyChain::Instance()->OnMapNpcDeadProcess( pField, pNPC, pOwner, killerObjKey );
	}
};

class ISSQMonsterLinkActions : public INpcLinkAction
{
public:
	virtual		~ISSQMonsterLinkActions() {}
	virtual		BOOL		OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
	{
		return AppliedNpcDependencyChain::Instance()->OnSSQMonsterDeadProcess( pField, pNPC, pOwner, killerObjKey );
	}
};

class IEventRegisterForNpcKillByQuestImpl : public IEventRegisterForNpcKillByQuest
{
public:		IEventRegisterForNpcKillByQuestImpl() {}
			~IEventRegisterForNpcKillByQuestImpl() {}

	virtual	BOOL	RegisterNpcKillInfo(WORD/*QCODE*/ questCode, MONSTERCODE npcCode)
	{
		AppliedNpcDependencyChain* pChain = AppliedNpcDependencyChain::Instance();
		pChain->AddPolicyNpcLinkQuest(npcCode);
        pChain->RegisterQuestNpcForNavigation(questCode, npcCode);
		return TRUE;
	};
};

class IEventRegisterForNpcKillByTriggerImpl : public IEventRegisterForNpcKillByTrigger
{
    typedef STLX_VECTOR<REGENID>    UNIT_ID_SET;
    UNIT_ID_SET m_UnitID_Set;
public:
	virtual			~IEventRegisterForNpcKillByTriggerImpl() {}
	virtual	BOOL	RegisterNpcKillInfo( const MONSTERCODE npcCode )
	{
		AppliedNpcDependencyChain* pChain = AppliedNpcDependencyChain::Instance();
		pChain->AddPolicyNpcLinkTrigger( npcCode );
		return TRUE;
	}
	virtual	BOOL	RegisterUnitKillInfo( const REGENID unitID )
	{
		m_UnitID_Set.push_back( unitID );
		return TRUE;
	}

	//	Unit ID 등록은 WUT, WMO 분리 상의 문제로 임시공간이 요구되므로 이에 대한 초기화 작업이 요구된다.
	virtual VOID	PrepareRegistering()
	{
		m_UnitID_Set.clear();
		m_UnitID_Set.reserve( 200 );
	}

    virtual VOID	BuildRegisterInfo(MAPCODE mapCode, FIELDCODE fieldCode,
                                      UnitRegenInfoManager* const pUnitRegenInfoManager)
    {
        AppliedNpcDependencyChain* pChain = AppliedNpcDependencyChain::Instance();
        //const AppliedNpcDependencyChain::QUEST_NPC_NAVI_TABLE& rTable = pChain->GetQuestNpcNaviTable();
        DWORD rCount = 0;
        MONSTERCODE pArray[NUM_REGENUNITTYPE] = {0,};//_NA_0_20100706_RANDOM_REGEN
        const UNITREGENINFO_HASH& rUnitRegenList = pUnitRegenInfoManager->GetUnitRegenInfoHash();
        FOREACH_CONTAINER(const UNITREGENINFO_HASH::value_type& rUnitRegen, rUnitRegenList,
                          UNITREGENINFO_HASH)
        {
            UnitRegenInfo* pUnitRegenInfo = rUnitRegen.second;
            // 주의 : npc코드 마다 시작 위치가 다를수는 있다. [3/22/2010 lst1024]
            // 툴에 의해서 랜덤한 시작 위치를 찍을수 있다.
            const WzVector& rPos = pUnitRegenInfo->GetStartPos(0);
            rCount = pUnitRegenInfo->GetNpcCodeArray(pArray, NUM_REGENUNITTYPE);
            for(DWORD i = 0; i < rCount; ++i)
            {
                MONSTERCODE npcCode = (MONSTERCODE)pArray[i];
                // 상수 테이블로만 남겨두자. 모든 npc의 navi정보 설정.
                pChain->RegisterQuestNpcRegenPosForNavigation(mapCode, fieldCode, npcCode, &rPos);
            }
        }

        FOREACH_CONTAINER(const REGENID unitID, m_UnitID_Set, UNIT_ID_SET)
        {
            UnitRegenInfo* const pUnitRegenInfo = pUnitRegenInfoManager->FindUnit( unitID );
            if(pUnitRegenInfo == 0)
                continue;

            //const WzVector& rPos = pUnitRegenInfo->GetRandomStartPos();         // 의도된 호출? [3/22/2010 lst1024]
            rCount = pUnitRegenInfo->GetNpcCodeArray(pArray, NUM_REGENUNITTYPE);
            for(DWORD i = 0; i < rCount; ++i)
            {
                MONSTERCODE npcCode = (MONSTERCODE)pArray[i];
                RegisterNpcKillInfo(npcCode);
            }
        }
        m_UnitID_Set.clear();
    }
};

//==================================================================================================
//

INpcLinkAction*
AppliedNpcDependencyChain::GetNpcLinkAction( const MONSTERCODE npcCode, const eOBJECT_TYPE objType )
{
	static INpcLinkNoAction				sINpcLinkNoAction;
	static IMonsterLinkActions			sIMonsterLinkActions;
	static IMapNpcLinkActions			sIMapNpcLinkActions;
	static ISSQMonsterLinkActions		sISSQMonsterLinkActions;

	typedef AppliedNpcDependencyChain::NPCCODE_LIST_IT	NPCCODE_LIST_IT;
	AppliedNpcDependencyChain* const pChain = AppliedNpcDependencyChain::Instance();

	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = pChain->GetNpcDependencyInfo(npcCode, itFIND);
	if( sNPC_POLICY::NPC_POLICY_NO_CONTROL!=policy )
	{
		switch( objType )
		{
        case FRIEND_MONSTER_OBJECT:
		case MONSTER_OBJECT:
		case LUCKYMON_OBJECT:
								return &sIMonsterLinkActions;
		case MAPNPC_OBJECT:		return &sIMapNpcLinkActions;
		case SSQMONSTER_OBJECT:	return &sISSQMonsterLinkActions;
		};
	}
	else
	{
		switch( objType )
		{
        case FRIEND_MONSTER_OBJECT:
		case MONSTER_OBJECT:
		case LUCKYMON_OBJECT:
								return &sIMonsterLinkActions;
		case MAPNPC_OBJECT:		return NULL;
		case SSQMONSTER_OBJECT:	return &sISSQMonsterLinkActions;
		};
	}

	return NULL;
}

IEventRegisterForNpcKillByQuest*
AppliedNpcDependencyChain::GetIEventRegisterForNpcKillByQuest()
{
	static IEventRegisterForNpcKillByQuestImpl		sIEventRegisterForNpcKillByQuestImpl;
	return &sIEventRegisterForNpcKillByQuestImpl;
};


IEventRegisterForNpcKillByTrigger*
AppliedNpcDependencyChain::GetIEventRegisterForNpcKillByTrigger()
{
	static IEventRegisterForNpcKillByTriggerImpl			sIEventRegisterForNpcKillByTriggerImpl;
	return &sIEventRegisterForNpcKillByTriggerImpl;
}

//─────────────────────────────────────────
//
struct KillMobAcceptableStatus : public std::unary_function<Character *, void>
{
	enum { MAX_CHECK_DISTANCE = 30, MAX_CHECK_DISTANCE2 = MAX_CHECK_DISTANCE*MAX_CHECK_DISTANCE };

	KillMobAcceptableStatus( Player* IN pKiller, const BASE_NPCINFO* pInfo )
		: m_pKiller(pKiller), m_ObjectKey(pKiller->GetObjectKey()), m_pNPCInfo(pInfo)
	{
		m_pZone = m_pKiller ? m_pKiller->GetGameZonePtr() : NULL;
		m_ZoneKey = m_pZone ? m_pZone->GetKey() : 0;
		m_pPos = m_pKiller ? m_pKiller->GetPosPtr() : NULL;
	}

	inline BOOL					IsValidState()	{ return m_pZone && m_ObjectKey ; }

	BOOL						Process( Player* IN pPlayer ) const
	{
        // (WAVERIX) (090324) (BUG-FIX)
        // 누군가가 몬스터를 잡을 때, 다른 파티원이 필드에서 나가버린 상황에
        // 문제가 발생할 수 있는 부분 수정
        GameField* pField = pPlayer->GetField();
        GameZone* pZone = pField ? pPlayer->GetGameZonePtr() : 0;
		if((pZone && pPlayer->IsAlive()) == FALSE)
			return FALSE;

		if( m_ObjectKey != pPlayer->GetObjectKey() )
		{
			if( (m_ZoneKey != pZone->GetKey()) || pPlayer->IsDead() )
				return FALSE;

			if( Math_VectorLength2( *m_pPos, *m_pKiller->GetPosPtr() ) > (MAX_CHECK_DISTANCE2) )
				return FALSE;
		}
		return TRUE;
	}

	BOOL						operator() ( Character* pChar ) const
	{
		Player* pPlayer = (Player*)pChar;
		Process( pPlayer );
	}

	Player*						m_pKiller;
	const DWORD					m_ObjectKey;
	const BASE_NPCINFO*			m_pNPCInfo;
	GameZone*					m_pZone;
	ZONEKEY						m_ZoneKey;
	const WzVector*				m_pPos;
    //
    __DISABLE_COPY(KillMobAcceptableStatus);
};

////─────────────────────────────────────────
////
//class _ItemDeleter
//{
//public:		typedef pair<DWORD, Player*>			_PLAYER_HASH_ITR;
//public:		_ItemDeleter( const CODETYPE itemCode ) : m_ItemCode(itemCode) {}
//
//			inline BOOL		IsDeleteAction() const { return m_ItemCode ? TRUE : FALSE; }
//			VOID			operator()( _PLAYER_HASH_ITR rPair ) const
//			{
//				Player* pPlayer = rPair.second;
//				ItemManager* pItemMGR = pPlayer->GetItemManager();
//
//				SLOTCODE slotCode = (SLOTCODE)m_ItemCode;
//				POSTYPE pos = 1;
//
//				static MSG_CG_ITEM_LOSE_ACK msgLOSE;
//				INVEN_DELETE_TOTAL_INFO& rItemInfos = msgLOSE.m_ItemInfo;
//				if( RC::RC_ITEM_SUCCESS == pItemMGR->DeleteAllKindOfItem( 1, &slotCode, &pos, &rItemInfos ) )
//				{
//					if( rItemInfos.m_Count != 0 )
//						pPlayer->SendPacket( &msgLOSE, msgLOSE.GetSize() );
//
//					//GAMELOG->LogItem( ITEM_DELETE, m_pPlayer, &rItemSlot, 0, rItemSlot.GetNum() );
//					return;
//				}
//			}
//
//			inline	VOID	Process( Player* IN pPlayer ) const
//			{ operator()( make_pair( pPlayer->GetObjectKey(), pPlayer ) ); }
//
//private:	const CODETYPE	m_ItemCode;
//};

//─────────────────────────────────────────
//
class LoopKillMonsterEvent : public std::unary_function<Character *, void>
{
public:
	LoopKillMonsterEvent( Player* pKiller, QUEST_MONSTER_KILL_MSG& questMsg, const BASE_NPCINFO* pInfo, const CODETYPE deleteItem=0 )
		: AcceptCond( pKiller, pInfo ), m_QuestMsg(questMsg)//, m_ItemDeleter(deleteItem)
	{
	}
	~LoopKillMonsterEvent() {}

	inline BOOL		IsValidState()	{ return AcceptCond.IsValidState(); }

	inline VOID		Process( Player* IN pPlayer ) const
	{
		// IsValidState는 이미 호출되어 있다고 전제한다.
		if( FALSE == AcceptCond.Process( pPlayer ) )
			return;

		// 퀘스트 처리
		pPlayer->GetQuestManager()->Event( QUEST_EVENT_MONSTERKILL, &m_QuestMsg );

		// AC 처리
		const BASE_NPCINFO* pNPCInfo = AcceptCond.m_pNPCInfo;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		if( 0 != pNPCInfo->m_wACCode )
			pPlayer->GetACManager()->IncConditionValue( pNPCInfo->m_wACCode, 
                                                        pNPCInfo->m_byACReferenceID,
                                                        pNPCInfo->m_MonsterCode);
#else
        if( 0 != pNPCInfo->m_wACCode && 0 != pNPCInfo->m_byACReferenceID )
            pPlayer->GetACManager()->IncConditionValue( pNPCInfo->m_wACCode, 
            pNPCInfo->m_byACReferenceID);
#endif

		//if( m_ItemDeleter.IsDeleteAction() )
		//	m_ItemDeleter.Process( pPlayer );
	}

	// 파티원+개인 모두 적용하자.
	VOID			operator() ( Character* pChar ) const
	{
		Player* pPlayer = (Player*)pChar;
		Process( pPlayer );
	}
private:
	KillMobAcceptableStatus		AcceptCond;
	QUEST_MONSTER_KILL_MSG&		m_QuestMsg;
	//_ItemDeleter				m_ItemDeleter;
    //
    __DISABLE_COPY(LoopKillMonsterEvent);
};

//─────────────────────────────────────────
//
class LoopKillMonsterGradeQuest : public std::unary_function<Character *, void>
{
public:
    LoopKillMonsterGradeQuest( Player* pKiller, QUEST_MONSTER_KILL_GRADE_MSG& questMsg, const BASE_NPCINFO* pInfo )
        : AcceptCond( pKiller, pInfo ), m_QuestMsg(questMsg)
    {
    }
    ~LoopKillMonsterGradeQuest() {}

    inline BOOL		IsValidState()	{ return AcceptCond.IsValidState(); }

    inline VOID		Process( Player* IN pPlayer ) const
    {
        // IsValidState는 이미 호출되어 있다고 전제한다.
        if( FALSE == AcceptCond.Process( pPlayer ) )
            return;

        // 퀘스트 처리
        pPlayer->GetQuestManager()->Event( QUEST_EVENT_MONSTERKILL_GRADE, &m_QuestMsg );
    }

    // 파티원+개인 모두 적용하자.
    VOID			operator() ( Character* pChar ) const
    {
        Player* pPlayer = (Player*)pChar;
        Process( pPlayer );
    }
private:
    KillMobAcceptableStatus		    AcceptCond;
    QUEST_MONSTER_KILL_GRADE_MSG&   m_QuestMsg;
    //
    __DISABLE_COPY(LoopKillMonsterGradeQuest);
};

class MonsterDeadOperator
{
public:
	MonsterDeadOperator( 
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		MONSTERCODE MonsterCode, 
#endif
		KillMobAcceptableStatus *pAcceptCond, BOOLEAN existQuestEvent, BOOLEAN existACEvent, 
		uint8_t type_of_AC, BYTE id_of_AC_ref, QUEST_MONSTER_KILL_MSG * pmsgQuestKill ) 
	{
		m_pAcceptCond = pAcceptCond;
		m_existQuestEvent = existQuestEvent;
		m_existACEvent = existACEvent;
		m_type_of_AC = type_of_AC;
		m_id_of_AC_ref = id_of_AC_ref;
		m_pmsgQuestKill = pmsgQuestKill;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		m_MonsterCode = MonsterCode;
#endif
        do_monster_book_ = FALSE; //_NA_007086_20140318_MONSTERBOOK
	}
	~MonsterDeadOperator() {}

	VOID operator () ( Player *pPlayer )
	{
		if( !m_pAcceptCond->Process( pPlayer ) )
			return;

		// 퀘스트 처리
		if( m_existQuestEvent )
			pPlayer->GetQuestManager()->Event( QUEST_EVENT_MONSTERKILL, m_pmsgQuestKill );

		// AC 처리
		if( m_existACEvent )
			pPlayer->GetACManager()->IncConditionValue( m_type_of_AC, m_id_of_AC_ref
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
			, m_MonsterCode
#endif
			);

        if (do_monster_book_) {
#ifdef _NA_007086_20140318_MONSTERBOOK
            MonsterBook player_monster_book = pPlayer->GetMonsterBook();
            if (player_monster_book.find(m_MonsterCode) == 0) {
                MSG_DG_CHARACTER_MONSTERBOOK_ADD_CMD cmd;
                cmd.monster_code = m_MonsterCode;
                pPlayer->SendToGameDBPServer(&cmd, sizeof(cmd));
            }
#endif //_NA_007086_20140318_MONSTERBOOK
        }
	}

private:
	KillMobAcceptableStatus	*	m_pAcceptCond;
	BOOLEAN						m_existQuestEvent;
	BOOLEAN						m_existACEvent;
	uint8_t						m_type_of_AC;
	BYTE						m_id_of_AC_ref;
	QUEST_MONSTER_KILL_MSG *	m_pmsgQuestKill;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	MONSTERCODE					m_MonsterCode;
#endif

public:
    BOOL do_monster_book_; //_NA_007086_20140318_MONSTERBOOK
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

BOOL
AppliedNpcDependencyChain::OnMonsterDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
{
	const BASE_NPCINFO* pInfo = pNPC->GetBaseInfo();
    if(pNPC->IsEqualObjectKind(MONSTER_OBJECT) == 0)
        return FALSE;

	const MONSTERCODE npcCode = pInfo->m_MonsterCode;
	Monster* pMonster = (Monster*)pNPC;

	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = GetNpcDependencyInfo(npcCode, itFIND);

	FlowControl flow;
	FLOWCONTROL_START()
    {
		IfFailThenBreak(pOwner, 0);

		const BOOLEAN existQuestEvent = !!(sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_QUEST&policy);
		const uint8_t type_of_AC = pInfo->m_wACCode;
		const BYTE id_of_AC_ref = pInfo->m_byACReferenceID;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		const BOOLEAN existACEvent = (type_of_AC != 0);
#else
		const BOOLEAN existACEvent = type_of_AC && id_of_AC_ref;
#endif

		QUEST_MONSTER_KILL_MSG msgQuestKill;
		msgQuestKill.m_MonsterCode = npcCode;

		KillMobAcceptableStatus acceptCond( pOwner, pInfo );

		IfFailThenBreak(acceptCond.IsValidState(), 0);

		MonsterDeadOperator Opr( 
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
			pInfo->m_MonsterCode,
#endif
			&acceptCond, existQuestEvent, existACEvent, type_of_AC, id_of_AC_ref, &msgQuestKill );

#ifdef _NA_007086_20140318_MONSTERBOOK
        if (MonsterBookParser::Instance()->FindMonsterBookInfo(npcCode) != NULL) {
            Opr.do_monster_book_ = TRUE;
        }
#endif //_NA_007086_20140318_MONSTERBOOK

		if( !NewGamePartyManager::Instance()->For_eachPlayers( pOwner->GetPartyState().GetPartyKey(), Opr ) )
		{
			Opr( pOwner );
		}
    }
    FLOWCONTROL_END;

	if( sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER&policy )	// no-check pOwner
	{
		TriggerManager* pMGR = pField->GetTriggerManager();

		// Send EventMsg To TriggerManager
		TRIGGER_KILL_MONSTER msgKill;
		msgKill.m_dwMonsterCode = npcCode;
		msgKill.m_dwObjectKey	= killerObjKey; 

		UnitRegenInfo* pRegenInfo = pMonster->GetUnitRegenInfo();
		msgKill.m_MonsterUnitID = ( pRegenInfo ) ? pRegenInfo->GetID() : 0;

		pMGR->OnMsg( &msgKill );
	}

    return OnMonsterDeadProcessGradeQuest( pField, pNPC, pOwner, killerObjKey );
}

BOOL
AppliedNpcDependencyChain::OnMapNpcDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
{
	const BASE_NPCINFO* pInfo = pNPC->GetBaseInfo();
    if(pNPC->IsEqualObjectKind(MAPNPC_OBJECT) == 0)
        return FALSE;

	const MONSTERCODE npcCode = pInfo->m_MonsterCode;
	MapNpc* pMapNPC = (MapNpc*)pNPC;


	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = GetNpcDependencyInfo(npcCode, itFIND);
	if( sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER&policy )
	{
		TriggerManager* pMGR = pField->GetTriggerManager();

		TRIGGER_DAMAGE_OBJECT msgDamage;

		// 트리거는 (1~) 대역을 사용
		msgDamage.m_dwObjectKey = Conv_TriggerObjectKey(pMapNPC->GetMapObjectID());
		pMGR->OnMsg( &msgDamage );

		// Send EventMsg To TriggerManager
		TRIGGER_KILL_MONSTER msgKill;
		msgKill.m_dwMonsterCode = npcCode;
		msgKill.m_dwObjectKey	= killerObjKey; 
		msgKill.m_MonsterUnitID = 0;
		pMGR->OnMsg( &msgKill );
	}

	return TRUE;
}


BOOL
AppliedNpcDependencyChain::OnSSQMonsterDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
{
	const BASE_NPCINFO* const pInfo	= pNPC->GetBaseInfo();
    if(pNPC->IsEqualObjectKind(SSQMONSTER_OBJECT) == 0)
        return FALSE;

	GameZone* const pGameZone = pField->GetGameZone();
	if( pGameZone->GetZoneType() == eZONETYPE_INSTANCE )
	{
		GameInstanceDungeon* const pDungeon = static_cast<GameInstanceDungeon*>( pGameZone );
		if( pDungeon->GetTypeOfIndun() == INDUN_KIND_SS_QUEST )
			GameWarControlManager::OnMsg( GameSSQCtrlCmd::CharacterEvent::OnDead( pNPC, pOwner ), pDungeon );
	}

	return OnMonsterDeadProcess( pField, pNPC, pOwner, killerObjKey );
}

BOOL
AppliedNpcDependencyChain::OnMonsterDeadProcessGradeQuest( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
{
    const BASE_NPCINFO* pInfo = pNPC->GetBaseInfo();
    if(pNPC->IsEqualObjectKind(MONSTER_OBJECT) == 0)
        return FALSE;

    const MONSTERCODE npcCode = pInfo->m_MonsterCode;
    Monster* pMonster = (Monster*)pNPC;

    if( pOwner )
    {   
        QUEST_MONSTER_KILL_GRADE_MSG questMsg;
        questMsg.m_MonsterGrade = pInfo->m_byGrade;
        questMsg.m_MonsterLevel = pInfo->m_DisplayLevel;

        LoopKillMonsterGradeQuest Opr( pOwner, questMsg, pInfo );
        if( Opr.IsValidState() )
        {
            if( !NewGamePartyManager::Instance()->For_eachPlayers( pOwner->GetPartyState().GetPartyKey(), Opr ) )
            {
                Opr( pOwner );
            }
        }
    }
    return TRUE;
}


VOID
AppliedNpcDependencyChain::AddPolicyNpcLinkTrigger( const MONSTERCODE npcCode )
{
	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = GetNpcDependencyInfo(npcCode, itFIND);
	if( !(sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER&policy)  )
	{
		sNPC_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER;
		sNPC_POLICY::sTRIGGER_ACTION& rTriggerAction = rPolicy.SHARED_BLK.TRIGGER_ACTION;
		rTriggerAction.NpcCode = npcCode;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		NPCCODE_LIST_IT itFIND = m_NpcCodeChain.find(npcCode);
		if( itFIND == m_NpcCodeChain.end() )
		{
			NPCCODE_LINK rLink = NPCCODE_LINK( sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER,
				NPCPOLICY_NODE( &(it->second), NULL, NULL )
				);

			m_NpcCodeChain.insert( NPCCODE_NODE( npcCode, rLink ) );
		}
		else
		{
			// 다른 정책은 존재하는 경우.
			NPCCODE_LINK& rLink = itFIND->second;
			ePOLICY_BITSET& rBit = rLink.first;
			NPCPOLICY_NODE& rNODE = rLink.second;
			rNODE.m_pTRIGGER = &(it->second);

			rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER );
		}
	}
}

VOID
AppliedNpcDependencyChain::AddPolicyNpcLinkQuest( const MONSTERCODE npcCode )
{
	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = GetNpcDependencyInfo(npcCode, itFIND);
	if( !(sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_QUEST&policy)  )
	{
		sNPC_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_QUEST;
		sNPC_POLICY::sQUEST_ACTION& rQuestAction = rPolicy.SHARED_BLK.QUEST_ACTION;
		rQuestAction.NpcCode = npcCode;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		NPCCODE_LIST_IT itFIND = m_NpcCodeChain.find(npcCode);
		if( itFIND == m_NpcCodeChain.end() )
		{
			NPCCODE_LINK rLink = NPCCODE_LINK( sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_QUEST,
				NPCPOLICY_NODE( NULL, &(it->second), NULL )
				);

			m_NpcCodeChain.insert( NPCCODE_NODE( npcCode, rLink ) );
		}
		else
		{
			// 다른 정책은 존재하는 경우.
			NPCCODE_LINK& rLink = itFIND->second;
			ePOLICY_BITSET& rBit = rLink.first;
			NPCPOLICY_NODE& rNODE = rLink.second;
			rNODE.m_pQUEST = &(it->second);

			rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_QUEST );
		}
	}
}

VOID
AppliedNpcDependencyChain::AddPolicyNpcLinkItemDelete( const MONSTERCODE npcCode, const CODETYPE itemCode )
{
	NPCCODE_LIST_IT itFIND;
	const ePOLICY policy = GetNpcDependencyInfo(npcCode, itFIND);
	if( !(sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST&policy)  )
	{
		sNPC_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST;
		sNPC_POLICY::sITEM_DELETE& rItemDelAction = rPolicy.SHARED_BLK.ITEM_DELETE_ACTION;
		rItemDelAction.NpcCode = npcCode;
		rItemDelAction.ItemCode = itemCode;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		NPCCODE_LIST_IT itFIND = m_NpcCodeChain.find(npcCode);
		if( itFIND == m_NpcCodeChain.end() )
		{
			NPCCODE_LINK rLink = NPCCODE_LINK( sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST,
				NPCPOLICY_NODE( NULL, NULL, &(it->second) )
				);

			m_NpcCodeChain.insert( NPCCODE_NODE( npcCode, rLink ) );
		}
		else
		{
			// 다른 정책은 존재하는 경우.
			NPCCODE_LINK& rLink = itFIND->second;
			ePOLICY_BITSET& rBit = rLink.first;
			NPCPOLICY_NODE& rNODE = rLink.second;
			rNODE.m_pITEM_DEL = &(it->second);

			rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST );
		}
	}
}

//{__NA001277_090205_NAVI_SYSTEM_SUPPORT__
void
AppliedNpcDependencyChain::RegisterQuestNpcRegenPosForNavigation(MAPCODE mapCode,
                                                                 FIELDCODE fieldCode,
                                                                 MONSTERCODE npcCode,
                                                                 const WzVector* pos)
{
    uQUEST_NPC_POS_KEY conv = { 0, };
    conv.npcCode = npcCode;
    conv.fieldCode = fieldCode;
    conv.mapCode = mapCode;

    // 중복되는 경우는 기존 정보 날려버린다.
    NPC_NAVI_NODE& node = m_QuestNpcNaviSearchTable[conv.key];
    node.questCode = 0;
    node.npcCode = npcCode;
    node.posX = (short)pos->x;
    node.posY = (short)pos->y;
}

void
AppliedNpcDependencyChain::RegisterQuestNpcForNavigation(WORD /*QCODE*/ questCode,
                                                         MONSTERCODE npcCode)
{
    NPC_NAVI_INFO& node = m_QuestNpcNaviTable[questCode];
    BOOST_STATIC_ASSERT(_countof(node.naviInfos) ==
                        QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM);
    if(_countof(node.naviInfos) <= node.numberOfNaviInfos)
        return;

    node.naviInfos[node.numberOfNaviInfos++] = npcCode;
}

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
MONSTERCODE AppliedNpcDependencyChain::GetRepleaceMonsterCode( \
    GameZone* game_zone, MONSTERCODE monster_code, bool regist_trigger )
{
    if (!game_zone)
        return 0;

    MONSTERCODE replaace_monster_code = \
        NPCReplaceInfoParser::Instance()->GetReplaceNPCCode(game_zone->GetMapCode(), monster_code);

    if (replaace_monster_code) 
    {
        if (regist_trigger) {
            const sNPC_POLICY::ePOLICY policy = GetNpcDependencyInfo(monster_code);

            // 원래몹에 OnDeadTrigger가 있으면 교체몹에도 트리거를 걸어줌
            if (sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER&policy ) {
                AddPolicyNpcLinkTrigger(replaace_monster_code);
            }
        }

        return replaace_monster_code;
    }

    return 0;
}
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
//}
