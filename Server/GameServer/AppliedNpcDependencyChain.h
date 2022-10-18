#pragma once

class SSQMonster;
class INpcLinkAction;
class IEventRegisterForNpcKillByQuest;
class IEventRegisterForNpcKillByTrigger;
//����������������������������������������������������������������������������������������������
//
struct sNPC_POLICY
{
	enum ePOLICY {
		NPC_POLICY_NOT_DEFINED						= 0,
		NPC_POLICY_NO_CONTROL						= NPC_POLICY_NOT_DEFINED,	// ��Ʈ���� ���� ����
		NPC_POLICY_NPC_ONDEAD_TRIGGER				= 1<<0,		// ���Ͱ� �׾��� �� Ʈ���� �׼� ����
		NPC_POLICY_NPC_ONDEAD_QUEST					= 1<<1,		// ���Ͱ� �׾��� �� ����Ʈ �׼� ����
		NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_QUEST		= 1<<2,		// ���Ͱ� �׾��� �� ����Ʈ ���� ������ �ٷ� �����Ǵ� ������ ó��
		NPC_POLICY_NPC_ONDEAD_ITEM_DELETE_NO_COND	= 1<<3,		// ���Ͱ� �׾��� �� ������ ���� �׼� ���� (�̱���)
	};
	//static const DWORD				_MAX_ITEM_DELETE_COUNT	= 5;

	ePOLICY							POLICY;
	union uSHARED_BLK
	{
		struct sTRIGGER_ACTION
		{
			MONSTERCODE				NpcCode;
		}							TRIGGER_ACTION;	// Trigger�� Ư�� ���¸� ��Ī�� ���� ������, �׷��� �Ǹ� �������� �������Ƿ� ������� ����.

		struct sQUEST_ACTION
		{
			MONSTERCODE				NpcCode;
		}							QUEST_ACTION;	// Quest�� Ư�� ���¸� ��Ī�� ���� ������, �׷��� �Ǹ� �������� �������Ƿ� ������� ����.

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

//����������������������������������������������������������������������������������������������
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

	typedef sNPC_POLICY::ePOLICY			ePOLICY_BITSET;		// �ش� �ڵ忡 ���õ� ��� POLICY bitset
	typedef sNPC_POLICY*					sTRIGGER_PTR;		// ���� Ʈ���� �����
	typedef sNPC_POLICY*					sQUEST_PTR;			// ���� ����Ʈ �����
	typedef sNPC_POLICY*					sITEM_DELETE_PTR;	// ������ ���� ó�� �����
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
			// �ʿ��� �� �����ϱ�� ����.
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

	//����������������������������������������������������������������������������������
	//enum eHARDCODE_VALUE
	//{
	//	LINK_ITEM_DELETE00_NPCCODE		= 38399,		// ����0-������ �̺�Ʈ NPC
	//	LINK_ITEM_DELETE00_ITEMCODE		= 40194,		// ����0
	//	LINK_ITEM_DELETE01_NPCCODE		= 38403,		// ����1-������ �̺�Ʈ NPC
	//	LINK_ITEM_DELETE01_ITEMCODE		= 40195,		// ����1
	//	LINK_ITEM_DELETE02_NPCCODE		= 38408,		// ����2-������ �̺�Ʈ NPC
	//	LINK_ITEM_DELETE02_ITEMCODE		= 40196,		// ����2
	//	LINK_ITEM_DELETE03_NPCCODE		= 38413,		// ����3-������ �̺�Ʈ NPC
	//	LINK_ITEM_DELETE03_ITEMCODE		= 40197,		// ����3
	//};
};

//����������������������������������������������������������������������������������������������
//	NpcAction
class INpcLinkAction
{
public:
	virtual		~INpcLinkAction() {}
	//	pOwner�� ���� ���ǿ� ���� ������ �ʿ��� ��쿡 ���, �� �������(071209)
	//	(Owner && !bTransformation) == TRUE �� ���ǿ� ���ؼ� Quest ó���� �̷����� �ְ�,
	//	Trigger�� ���� ���� �� ������ �����ϰ� All rounding�ȴ�.
	//	���� �ѹ��� ȣ���Ϸ���,
	//		NpcLinkAction.OnDead( pField, pNPC, (pOwner&&!bTransaction) ? pOwner : NULL );
	//	�� ���� �ϸ� �ȴ�. AC������... ��...
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

