#pragma once

class SSQMonster;
class INpcLinkAction;
class IEventRegisterForNpcKillByQuest;
class IEventRegisterForNpcKillByTrigger;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct sNPC_POLICY
{
	enum ePOLICY {
		NPC_POLICY_NOT_DEFINED						= 0,
		NPC_POLICY_NO_CONTROL						= NPC_POLICY_NOT_DEFINED,	// 컨트롤할 정보 없음
		NPC_POLICY_NPC_ONDEAD_TRIGGER				= 1<<0,		// 몬스터가 죽었을 때 트리거 액션 여부
		NPC_POLICY_NPC_ONDEAD_QUEST					= 1<<1,		// 몬스터가 죽었을 때 퀘스트 액션 여부
		NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST		= 1<<2,		// 몬스터가 죽었을 때 퀘스트 몬스터 죽을때 바로 삭제되는 아이템 처리
		NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_NO_COND	= 1<<3,		// 몬스터가 죽었을 때 아이템 삭제 액션 여부 (미구현)
	};
	//static const DWORD				_MAX_ITEM_DELETE_COUNT	= 5;

	ePOLICY							POLICY;
	union uSHARED_BLK
	{
		struct sTRIGGER_ACTION
		{
			MONSTERCODE				NpcCode;
		}							TRIGGER_ACTION;	// Trigger는 특정 상태만 지칭할 수도 있지만, 그렇게 되면 정보량이 많아지므로 기존대로 간다.

		struct sQUEST_ACTION
		{
			MONSTERCODE				NpcCode;
		}							QUEST_ACTION;	// Quest는 특정 상태만 지칭할 수도 있지만, 그렇게 되면 정보량이 많아지므로 기존대로 간다.

		struct sITEM_DELETE
		{
			MONSTERCODE				NpcCode;
			CODETYPE				ItemCode;
		}							ITEM_DELETE_ACTION;
	}								SHARED_BLK;

	typedef uSHARED_BLK::sTRIGGER_ACTION		sTRIGGER_ACTION;
	typedef uSHARED_BLK::sQUEST_ACTION			sQUEST_ACTION;
	typedef uSHARED_BLK::sITEM_DELETE			sITEM_DELETE;
	inline	VOID					Clear() { POLICY = NPC_POLICY_NOT_DEFINED; ZeroMemory( &SHARED_BLK, sizeof(SHARED_BLK) ); }
	inline	sTRIGGER_ACTION&		GetTriggerInfo()	{ return SHARED_BLK.TRIGGER_ACTION; }
	inline	sQUEST_ACTION&			GetQuestInfo()		{ return SHARED_BLK.QUEST_ACTION; }
	inline	sITEM_DELETE&			GetItemDeleteInfo() { return SHARED_BLK.ITEM_DELETE_ACTION; }
};

//{__NA001277_090205_NAVI_SYSTEM_SUPPORT__
typedef MSG_CG_QUEST_OBJECT_NAVI_POS_CMD::Node  NPC_NAVI_NODE;
struct NPC_NAVI_INFO
{
    NPC_NAVI_INFO() : numberOfNaviInfos(0)
    {
        ZeroMemory(naviInfos, sizeof(naviInfos));
    }

    BYTE numberOfNaviInfos;
    MONSTERCODE naviInfos[5]; // assert check in code-file
};

union uQUEST_NPC_POS_KEY
{
    struct {
        MONSTERCODE npcCode;
        FIELDCODE fieldCode;
        MAPCODE mapCode;
        WORD __reserved__;
    };
    DWORD64 key;
};
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<AppliedNpcDependencyChain>
class AppliedNpcDependencyChain : public IWorkNode
{
private:
    static const DWORD  INTERVALforUPDATE		= 10*1000;
private:
    typedef sNPC_POLICY::ePOLICY			ePOLICY;
	typedef sNPC_POLICY::sTRIGGER_ACTION	sTRIGGER_ACTION;
	typedef sNPC_POLICY::sQUEST_ACTION		sQUEST_ACTION;
	typedef sNPC_POLICY::sITEM_DELETE		sITEM_DELETE;

private:
    typedef std::pair<ePOLICY, sNPC_POLICY>		POLICY_NODE;
	typedef STLX_LIST<POLICY_NODE>			POLICY_LIST;
	typedef POLICY_LIST::iterator			POLICY_LIST_IT;

	typedef sNPC_POLICY::ePOLICY			ePOLICY_BITSET;		// 해당 코드에 관련된 모든 POLICY bitset
	typedef sNPC_POLICY*					sTRIGGER_PTR;		// 관련 트리거 존재시
	typedef sNPC_POLICY*					sQUEST_PTR;			// 관련 퀘스트 존재시
	typedef sNPC_POLICY*					sITEM_DELETE_PTR;	// 아이템 제거 처리 존재시
	struct NPCPOLICY_NODE
	{
		sTRIGGER_PTR		m_pTRIGGER;
		sQUEST_PTR			m_pQUEST;
		sITEM_DELETE_PTR	m_pITEM_DEL;
		NPCPOLICY_NODE(
            const sTRIGGER_PTR pTRIGGER=NULL,
			const sQUEST_PTR pQUEST=NULL,
			const sITEM_DELETE_PTR pITEM_DEL=NULL
			) : m_pTRIGGER(pTRIGGER), m_pQUEST(pQUEST), m_pITEM_DEL(pITEM_DEL)
		{}
	};
	//typedef pair<sTRIGGER_PTR, sQUEST_PTR>		NPCPOLICY_NODE;
	typedef std::pair<ePOLICY_BITSET, NPCPOLICY_NODE>	NPCCODE_LINK;	//
	typedef STLX_MAP<CODETYPE, NPCCODE_LINK>		NPCCODE_LIST;
	typedef NPCCODE_LIST::iterator					NPCCODE_LIST_IT;
	typedef std::pair<CODETYPE, NPCCODE_LINK>			NPCCODE_NODE;
public:
    typedef STLX_HASH_MAP<WORD/*QCODE*/, NPC_NAVI_INFO> QUEST_NPC_NAVI_TABLE;
    // [MAPCODE|FIELDCODE|MONSTERCODE] - uQUEST_NPC_POS_KEY
    typedef STLX_HASH_MAP<DWORD64, NPC_NAVI_NODE> QUEST_NPC_NAVI_SEARCH_TABLE;

private:	AppliedNpcDependencyChain();
public:
	virtual	~AppliedNpcDependencyChain();
	static VOID CreateInstance() { ms_pInstance ? 0 : new AppliedNpcDependencyChain(); }
	static VOID DestoryInstance() { SAFE_DELETE(ms_pInstance); }
	inline
	static AppliedNpcDependencyChain* Instance() { return ms_pInstance; }
	inline static WORK_KEY GetWorkKey() { return ms_WorkKey; }
	inline static VOID  SetWorkKey( WORK_KEY wKey ) { ms_WorkKey = wKey; }
public:
	virtual BOOL    DoInit();
	virtual BOOL    DoProcess();
	virtual BOOL    IsComplete();
	virtual BOOL    DoRelease();
	// Control Interface
	virtual	VOID    OnMsg( sSIG_MSG* pSIG_MSG );

	inline ePOLICY GetNpcDependencyInfo( const MONSTERCODE npcCode )
	{
		NPCCODE_LIST_IT itFIND = m_NpcCodeChain.find(npcCode);
		return itFIND != m_NpcCodeChain.end() ? itFIND->second.first : sNPC_POLICY::NPC_POLICY_NO_CONTROL;
	}
	inline ePOLICY GetNpcDependencyInfo( const MONSTERCODE npcCode, NPCCODE_LIST_IT& OUT itFIND )
	{
		itFIND = m_NpcCodeChain.find(npcCode);
		return itFIND != m_NpcCodeChain.end() ? itFIND->second.first : sNPC_POLICY::NPC_POLICY_NO_CONTROL;
	}
    //{__NA001277_090205_NAVI_SYSTEM_SUPPORT__
    void RegisterQuestNpcRegenPosForNavigation(MAPCODE mapCode, FIELDCODE fieldCode,
                                               MONSTERCODE npcCode, const WzVector* pos);
    void RegisterQuestNpcForNavigation(WORD /*QCODE*/ questCode, MONSTERCODE npcCode);
    inline const QUEST_NPC_NAVI_TABLE& GetQuestNpcNaviTable() const { return m_QuestNpcNaviTable; }
    inline const QUEST_NPC_NAVI_SEARCH_TABLE& GetQuestNpcNaviSearchTable() const { return m_QuestNpcNaviSearchTable; }
    //}
	//inline	BOOL					IsExpirationItem( const CODETYPE itemCode )
	//{	return !!( sNPC_POLICY::NPC_POLICY_TRIGGER_ACTION&GetItemDependencyInfo(itemCode) );	}
	//inline	BOOL					IsExclusiveItem( const CODETYPE itemCode )
	//{	return !!( sNPC_POLICY::NPC_POLICY_QUEST_ACTION&GetItemDependencyInfo(itemCode) );	}
	//inline	BOOL					IsRenderEffectItem( const CODETYPE itemCode )
	//{	return !!( sNPC_POLICY::NPC_POLICY_ITEM_DELETE_ACTION&GetItemDependencyInfo(itemCode) );	}

public:		CODETYPE				GetDeleteItem( const MONSTERCODE npcCode );
			CODETYPE				GetDeleteItem( const MONSTERCODE npcCode, NPCCODE_LIST_IT& IN rIT );

			VOID					AddPolicyNpcLinkTrigger( const MONSTERCODE npcCode );
			VOID					AddPolicyNpcLinkQuest( const MONSTERCODE npcCode );
			VOID					AddPolicyNpcLinkItemDelete( const MONSTERCODE npcCode, const CODETYPE itemCode );
			// 필요할 때 구현하기로 하자.
			//VOID					RemovePolicyNpcLinkTrigger( const MONSTERCODE npcCode );
			//VOID					RemovePolicyNpcLinkQuest( const MONSTERCODE npcCode );
			//VOID					RemovePolicyNpcLinkItemDelete( const MONSTERCODE npcCode );

			INpcLinkAction*			GetNpcLinkAction( const MONSTERCODE npcCode, const eOBJECT_TYPE objType );
			BOOL					OnMonsterDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey );
			BOOL					OnMapNpcDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey );
			BOOL					OnSSQMonsterDeadProcess( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey );
            BOOL					OnMonsterDeadProcessGradeQuest( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey );

            IEventRegisterForNpcKillByQuest*    GetIEventRegisterForNpcKillByQuest();
			IEventRegisterForNpcKillByTrigger*  GetIEventRegisterForNpcKillByTrigger();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    MONSTERCODE GetRepleaceMonsterCode(GameZone* game_zone, MONSTERCODE monster_code, bool regist_trigger = false);
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

private:	POLICY_LIST					m_PolicyChain;
			NPCCODE_LIST				m_NpcCodeChain;
			ITimerBase					m_UpdateTimer;
            QUEST_NPC_NAVI_TABLE        m_QuestNpcNaviTable;
            QUEST_NPC_NAVI_SEARCH_TABLE m_QuestNpcNaviSearchTable;

	static	WORK_KEY					ms_WorkKey;
	static	AppliedNpcDependencyChain*	ms_pInstance;

	//─────────────────────────────────────────
	//enum eHARDCODE_VALUE
	//{
	//	LINK_ITEM_DELETE00_NPCCODE		= 38399,		// 구슬0-제거할 이벤트 NPC
	//	LINK_ITEM_DELETE00_ITEMCODE		= 40194,		// 구슬0
	//	LINK_ITEM_DELETE01_NPCCODE		= 38403,		// 구슬1-제거할 이벤트 NPC
	//	LINK_ITEM_DELETE01_ITEMCODE		= 40195,		// 구슬1
	//	LINK_ITEM_DELETE02_NPCCODE		= 38408,		// 구슬2-제거할 이벤트 NPC
	//	LINK_ITEM_DELETE02_ITEMCODE		= 40196,		// 구슬2
	//	LINK_ITEM_DELETE03_NPCCODE		= 38413,		// 구슬3-제거할 이벤트 NPC
	//	LINK_ITEM_DELETE03_ITEMCODE		= 40197,		// 구슬3
	//};
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	NpcAction
class INpcLinkAction
{
public:
	virtual		~INpcLinkAction() {}
	//	pOwner는 기존 조건에 따른 설정이 필요한 경우에 사용, 즉 현재기준(071209)
	//	(Owner && !bTransformation) == TRUE 인 조건에 대해서 Quest 처리가 이뤄지고 있고,
	//	Trigger에 의한 것은 위 조건을 무시하고 All rounding된다.
	//	따라서 한번만 호출하려면,
	//		NpcLinkAction.OnDead( pField, pNPC, (pOwner&&!bTransaction) ? pOwner : NULL );
	//	와 같이 하면 된다. AC정보는... 음...
	virtual		BOOL		OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey ) = 0;
};

class INpcLinkNoAction : public INpcLinkAction
{
public:
	virtual		~INpcLinkNoAction() {}
	virtual		BOOL		OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey ) { __UNUSED((pField, pNPC, pOwner, killerObjKey)); return TRUE; }
};

class INpcLinkActionD
{
public:
	INpcLinkActionD() : m_pInterface(NULL) {}
	INpcLinkActionD( const INpcLinkAction* pIAction ) : m_pInterface(const_cast<INpcLinkAction*>(pIAction)) {}
	INpcLinkActionD( const INpcLinkActionD& rSrc ) : m_pInterface(rSrc.m_pInterface) {}
	~INpcLinkActionD() { m_pInterface = NULL; }

	inline	BOOL			OnDead( GameField* IN pField, NPC* IN pNPC, Player* IN pOwner, const DWORD killerObjKey )
	{ return m_pInterface ? m_pInterface->OnDead(pField, pNPC, pOwner, killerObjKey) : TRUE; }

	INpcLinkAction*			m_pInterface;
};

