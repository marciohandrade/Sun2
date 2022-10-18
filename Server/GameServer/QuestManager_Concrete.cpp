#include "stdafx.h"
#include <Quest.h>
#include "QuestManager_Concrete.h"
#include "QuestCondition_Concrete.h"
#include "QuestAction_Concrete.h"
#include "Player.h"
#include "Monster.h"
#include "GameZone.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
VarMsg2	QuestManager_Concrete::m_VarMsg;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
util::PoolRouter<eQUEST_ACTION_MAX>						QuestManager_Concrete::ms_PoolRouter;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IMPLEMENTATION><QuestManager_Concrete>
//
VOID		QuestManager_Concrete::Init( WORD wPoolSize )
{
	// Condition
	ms_PoolRouter.Register<QUEST_CONDITION_REPEAT>		(eQUEST_CONDITION_REPEAT);
	ms_PoolRouter.Register<QUEST_CONDITION_CHARLV>		(eQUEST_CONDITION_CHARLV);
	ms_PoolRouter.Register<QUEST_CONDITION_CHARCLASS>	(eQUEST_CONDITION_CHARCLASS);
	ms_PoolRouter.Register<QUEST_CONDITION_QUEST>		(eQUEST_CONDITION_QUEST);		
	ms_PoolRouter.Register<QUEST_CONDITION_ITEM>		(eQUEST_CONDITION_ITEM);		
	ms_PoolRouter.Register<QUEST_CONDITION_CLASSITEM>	(eQUEST_CONDITION_CLASSITEM);
	ms_PoolRouter.Register<QUEST_CONDITION_MONEY>		(eQUEST_CONDITION_MONEY);
	ms_PoolRouter.Register<QUEST_CONDITION_KILLMONSTER>	(eQUEST_CONDITION_KILLMONSTER);
	ms_PoolRouter.Register<QUEST_CONDITION_MISSION>		(eQUEST_CONDITION_MISSION);
	ms_PoolRouter.Register<QUEST_CONDITION_AREA>		(eQUEST_CONDITION_AREA);
	ms_PoolRouter.Register<QUEST_CONDITION_ITEM_SPACE>	(eQUEST_CONDITION_ITEM_SPACE);
	ms_PoolRouter.Register<QUEST_CONDITION_CHAO>		(eQUEST_CONDITION_CHAO);
    ms_PoolRouter.Register<QUEST_CONDITION_CREATE_PORTAL>(eQUEST_CONDITION_CREATE_PORTAL);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    ms_PoolRouter.Register<QUEST_CONDITION_DAILY_QUEST> (eQUEST_CONDITION_DAILY_QUEST);
#endif //_NA_003027_20111013_HONOR_SYSTEM
	// Action
	ms_PoolRouter.Register<QUEST_ACTION_ITEM>			(eQUEST_ACTION_ITEM);
	ms_PoolRouter.Register<QUEST_ACTION_CHANGEofCLASS>	(eQUEST_ACTION_CHANGEofCLASS);
	ms_PoolRouter.Register<QUEST_ACTION_SPAWN_MONSTER>	(eQUEST_ACTION_SPAWN_MONSTER);
    //__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
    ms_PoolRouter.Register<QUEST_ACTION_SPAWN_MONSTER_ABANDON> (eQUEST_ACTION_SPAWN_MONSTER_ABANDON);
    ms_PoolRouter.Register<QUEST_ACTION_CREATE_PORTAL>  (eQUEST_ACTION_CREATE_PORTAL);
    ms_PoolRouter.Register<QUEST_ACTION_CLOSE_PORTAL>   (eQUEST_ACTION_CLOSE_PORTAL);
	ms_PoolRouter.Register<QUEST_ACTION_DELETE_COLLECTIONID> (eQUEST_ACTION_DELETE_COLLECTIONID);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    ms_PoolRouter.Register<QUEST_ACTION_DAILY_QUEST>    (eQUEST_ACTION_DAILY_QUEST);
    ms_PoolRouter.Register<QUEST_ACTION_DAILY_QUEST_DELETE> (eQUEST_ACTION_DAILY_QUEST_DELETE);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    ms_PoolRouter.Register<QUEST_CONDITION_MISSION_CLEAR>   (eQUEST_CONDITION_MISSION_CLEAR);
#endif //_NA_006607_20130402_ADD_QUESTTYPE

    #undef DECL_POOL_INITIALIZE
    #define DECL_POOL_INITIALIZE(IdxOfType, init_size, expand_size) \
        ms_PoolRouter.Initialize(e##IdxOfType, init_size, expand_size, #IdxOfType)

    // Condition
    DECL_POOL_INITIALIZE(QUEST_CONDITION_REPEAT,        0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_CHARLV,        0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_CHARCLASS,     0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_QUEST,         0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_ITEM,          0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_CLASSITEM,     0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_MONEY,         0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_KILLMONSTER,   0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_MISSION,       0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_AREA,          0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_ITEM_SPACE,    0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_CHAO,          0, 20);
    DECL_POOL_INITIALIZE(QUEST_CONDITION_CREATE_PORTAL, 0, 20);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    DECL_POOL_INITIALIZE(QUEST_CONDITION_DAILY_QUEST,   0, 20);
#endif //_NA_003027_20111013_HONOR_SYSTEM
    // Action
    DECL_POOL_INITIALIZE(QUEST_ACTION_ITEM,             0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_CHANGEofCLASS,    0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_SPAWN_MONSTER,    0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_SPAWN_MONSTER_ABANDON, 0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_CREATE_PORTAL,    0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_CLOSE_PORTAL,     0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_DELETE_COLLECTIONID, 0, 20);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    DECL_POOL_INITIALIZE(QUEST_ACTION_DAILY_QUEST,      0, 20);
    DECL_POOL_INITIALIZE(QUEST_ACTION_DAILY_QUEST_DELETE, 0, 20);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    DECL_POOL_INITIALIZE(QUEST_CONDITION_MISSION_CLEAR, 0, 20);
#endif //_NA_006607_20130402_ADD_QUESTTYPE

    #undef DECL_POOL_INITIALIZE
}

VOID		QuestManager_Concrete::Init( Player* pPlayer )
{
	SetPlayer(pPlayer);
    RegisterNpcSummonEvent(0, 0);
}
VOID		QuestManager_Concrete::Release()
{
	DestroyAll();
}


QuestCondition*		QuestManager_Concrete::CreateQuestCondition( QuestConditionInfo* pInfo )
{
	const DWORD poolIdx = (DWORD)pInfo->GetType();
	if( FlowControl::FCAssert( poolIdx<eQUEST_CONDITION_MAX ) )
	{
		QuestCondition* pCondition = (QuestCondition*)ms_PoolRouter.Alloc( poolIdx );

		// 더 좋은 방법은 있지만 관두자...-.-;;;
		switch( pInfo->GetType() )
		{
#define CONVERT_TYPE_AND_SETINFO( _TYPE )	\
	e##_TYPE: ((_TYPE*)pCondition)->SetInfo( (_TYPE##_INFO*)pInfo ); break
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_REPEAT		);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_CHARLV		);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_CHARCLASS		);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_QUEST			);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_ITEM			);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_CLASSITEM		);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_MONEY			);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_KILLMONSTER	);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_MISSION		);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_AREA			);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_ITEM_SPACE	);
			case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_CHAO			);
            case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_CREATE_PORTAL );
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_DAILY_QUEST   );
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
            case CONVERT_TYPE_AND_SETINFO(QUEST_CONDITION_MISSION_CLEAR);
#endif // _NA_006607_20130402_ADD_QUESTTYPE
#undef CONVERT_TYPE_AND_SETINFO
		}

		return pCondition;
	}

	return NULL;
} 

QuestAction*		QuestManager_Concrete::CreateQuestAction( QuestActionInfo* pInfo )
{
	const DWORD poolIdx = (DWORD)pInfo->GetType();
	//	(071112) poolIdx == eQUEST_ACTION_ITEM
	if( FlowControl::FCAssert( !( (poolIdx<eQUEST_CONDITION_MAX) || (poolIdx>eQUEST_ACTION_MAX) ) ) )
	{
		QuestAction* pAction = (QuestAction*)ms_PoolRouter.Alloc( poolIdx );
		switch( pInfo->GetType() )
		{
#define CONVERT_TYPE_AND_SETINFO( _TYPE )	\
	e##_TYPE: ((_TYPE*)pAction)->SetInfo( (_TYPE##_INFO*)pInfo ); break
			case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_ITEM				);
			case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_CHANGEofCLASS	);
			case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_SPAWN_MONSTER	);
            case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_SPAWN_MONSTER_ABANDON);
            case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_CREATE_PORTAL	);
            case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_CLOSE_PORTAL     );	
			case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_DELETE_COLLECTIONID	);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_DAILY_QUEST      );
            case CONVERT_TYPE_AND_SETINFO(QUEST_ACTION_DAILY_QUEST_DELETE);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#undef CONVERT_TYPE_AND_SETINFO
		}

		return pAction;
	}

	return NULL;
}

VOID		QuestManager_Concrete::DestroyQuestCondition( QuestCondition* pCondition )
{
	const DWORD poolIdx = (DWORD)pCondition->GetType();
	if( FlowControl::FCAssert( poolIdx<eQUEST_CONDITION_MAX ) )
		ms_PoolRouter.Free( poolIdx, pCondition );
}
VOID		QuestManager_Concrete::DestroyQuestAction( QuestAction* pAction )
{
	const DWORD poolIdx = (DWORD)pAction->GetType();
	//	(071112) poolIdx == eQUEST_ACTION_ITEM
	if( FlowControl::FCAssert( !( (poolIdx<eQUEST_CONDITION_MAX) || (poolIdx>eQUEST_ACTION_MAX) ) ) )
		ms_PoolRouter.Free( poolIdx, pAction );
}

//VOID		QuestManager_Concrete::RecvQuestStream( MSG_CG_QUEST_STREAM_CMD* pcmd )
//{
//	QUEST_COMPL_TOTAL_INFO* p_compInfo = (QUEST_COMPL_TOTAL_INFO *)(((BYTE*)pcmd) + pcmd->GetSize());
//	SerializeStream( *p_compInfo, QSERIALIZE_SAVE );
//	QUEST_PROGR_TOTAL_INFO* p_prgInfo = (QUEST_PROGR_TOTAL_INFO *)(((BYTE*)p_compInfo) + p_compInfo->GetSize());
//	SerializeStream( *p_prgInfo, QSERIALIZE_SAVE );
//}

VOID		QuestManager_Concrete::SendQuestStream()
{
	MSG_CG_QUEST_STREAM_CMD* pcmd = m_VarMsg.StartTypePtr( Param2Class<MSG_CG_QUEST_STREAM_CMD>() );
	m_VarMsg.SetSize( pcmd->GetSize() );

	QUEST_COMPL_TOTAL_INFO* p_compInfo = (QUEST_COMPL_TOTAL_INFO*)m_VarMsg.NextPtr();
	SerializeStream( *p_compInfo, QSERIALIZE_LOAD );
	m_VarMsg.SetSize( p_compInfo->GetSize() );
	QUEST_PROGR_TOTAL_INFO* p_prgInfo = (QUEST_PROGR_TOTAL_INFO*)m_VarMsg.NextPtr();
	SerializeStream( *p_prgInfo, QSERIALIZE_LOAD );
	m_VarMsg.SetSize( p_prgInfo->GetSize() );

    if( (p_prgInfo->m_Count + p_compInfo->m_Count) == 0 &&
        (FALSE == m_pPlayer->IsGMUser()) )
    {
        return;
    }

    m_pPlayer->SendExPacket( m_VarMsg.GetMsgCount(), m_VarMsg.GetMsgPPtr(), m_VarMsg.GetMsgSizePtr() );		
}

VOID		QuestManager_Concrete::OnEvent( eQUEST_EVENT evt, QUEST_MSG* pMsg, Quest* pQuest )
{
	// TODO : 퀘스트의 상태에 따라 적절한 처리의 코드가 필요하다.

	switch( evt )
	{
	//case QUEST_EVENT_ITEM:
	//case QUEST_EVENT_CHARLV:
	//case QUEST_EVENT_QUEST:
	//case QUEST_EVENT_MONEY:
	case QUEST_EVENT_MONSTERKILL:
		{
			QUEST_MONSTER_KILL_MSG* pmsg = (QUEST_MONSTER_KILL_MSG *)pMsg;
			MSG_CG_QUEST_MONSTER_KILL_CMD msg;
			msg.m_QuestCode = pQuest->GetQuestCode();
			msg.m_MonsterCode = pmsg->m_MonsterCode; 
			//msg.m_MonsterNum = pmsg->m_MonsterNum;
			m_pPlayer->SendPacket( &msg, sizeof(msg) );
		}
		break;
    case QUEST_EVENT_MONSTERKILL_GRADE:
        {
            QUEST_MONSTER_KILL_GRADE_MSG* pmsg = (QUEST_MONSTER_KILL_GRADE_MSG *)pMsg;
            MSG_CG_QUEST_MONSTER_KILL_GRADE_CMD msg;
            msg.m_QuestCode = pQuest->GetQuestCode();
            msg.m_MonsterGrade = pmsg->m_MonsterGrade; 
            msg.m_MonsterLevel = pmsg->m_MonsterLevel;
            m_pPlayer->SendPacket( &msg, sizeof(msg) );
        }
        break;

    case QUEST_EVENT_AREA:
        {
            QUEST_AREA_MSG* pmsg = (QUEST_AREA_MSG *)pMsg;
            MSG_CG_QUEST_EVENT_AREA_ACK msg;
            msg.m_AreaID = pmsg->m_AreaID;
            m_pPlayer->SendPacket( &msg, sizeof(msg) );
        }
        break;
	//case QUEST_EVENT_TIME:
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    case QUEST_EVENT_MISSION_CLEAR:
        {
            QUEST_MISSION_CLEAR_MSG* pmsg = (QUEST_MISSION_CLEAR_MSG *)pMsg;
            MSG_CG_QUEST_MISSION_CLEAR_CMD msg;
            msg.quest_code = pQuest->GetQuestCode();
            msg.map_code = pmsg->mission_code;
            m_pPlayer->SendPacket(&msg, sizeof(msg));
        }
        break;
#endif //_NA_006607_20130402_ADD_QUESTTYPE
	}
}

//{__NA001277_090205_NAVI_SYSTEM_SUPPORT__
struct MakeNaviStream
{
    typedef AppliedNpcDependencyChain::QUEST_NPC_NAVI_TABLE         QUEST_NPC_NAVI_TABLE;
    typedef AppliedNpcDependencyChain::QUEST_NPC_NAVI_SEARCH_TABLE  QUEST_NPC_NAVI_SEARCH_TABLE;

    MakeNaviStream(MAPCODE mapCode, FIELDCODE fieldCode,
                   MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* INOUT pMsg)
        : m_rNaviTable(AppliedNpcDependencyChain::Instance()->GetQuestNpcNaviTable())
        , m_rSearchTable(AppliedNpcDependencyChain::Instance()->GetQuestNpcNaviSearchTable())
        , navi_end(m_rNaviTable.end())
        , search_end(m_rSearchTable.end())
        , m_pMsg(pMsg)
    {
        ZeroMemory(&m_Key, sizeof(m_Key));
        m_Key.mapCode = mapCode;
        m_Key.fieldCode = fieldCode;
        pMsg->m_CodeType = pMsg->OPTION_NULL;
        pMsg->m_NumberOfNodes = 0;
        ZeroMemory(pMsg->m_Nodes, sizeof(pMsg->m_Nodes));
    }

    void operator()(Quest* pQuest)
    {
        if(m_pMsg->m_NumberOfNodes >= _countof(m_pMsg->m_Nodes))
            return;

        QCODE questCode = pQuest->GetQuestCode();
        QUEST_NPC_NAVI_TABLE::const_iterator it_quest = m_rNaviTable.find(questCode);
        if(it_quest == navi_end)
            return;

        const NPC_NAVI_INFO& rInfo = it_quest->second;
        const MONSTERCODE* pIt = rInfo.naviInfos;
        const MONSTERCODE* pItEnd = &rInfo.naviInfos[rInfo.numberOfNaviInfos];
        for( ; pIt != pItEnd; ++pIt)
        {
            m_Key.npcCode = *pIt;
            QUEST_NPC_NAVI_SEARCH_TABLE::const_iterator it = m_rSearchTable.find(m_Key.key);
            if(it == search_end)
                continue;

            NPC_NAVI_NODE& node = m_pMsg->m_Nodes[m_pMsg->m_NumberOfNodes++];
            node = it->second;
            node.questCode = questCode;
        }
    }

    const QUEST_NPC_NAVI_TABLE& m_rNaviTable;
    const QUEST_NPC_NAVI_SEARCH_TABLE& m_rSearchTable;
    QUEST_NPC_NAVI_TABLE::const_iterator navi_end;
    QUEST_NPC_NAVI_SEARCH_TABLE::const_iterator search_end;
    uQUEST_NPC_POS_KEY m_Key;
    MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* m_pMsg;
};

BOOLEAN
QuestManager_Concrete::MakeInfoQuestNpcNaviStream(MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* INOUT pMsg,
                                                  QCODE questCode)
{
    GameField* pGameField = m_pPlayer->GetField();
    MAPCODE mapCode = pGameField ? pGameField->GetGameZone()->GetMapCode() : 0;
    if(FALSE == (pGameField && mapCode))
        return false;

    FIELDCODE fieldCode = pGameField->GetFieldCode();
    MakeNaviStream opr(mapCode, fieldCode, pMsg);

    if(questCode == 0)
    {
        ForeachProgressQuest(opr);
        pMsg->m_CodeType = pMsg->OPERATION_ALL_INFO | pMsg->CODETYPE_NPC;
    }
    else if(Quest* pQuest = FindQuest(questCode))
    {
        opr(pQuest);
        pMsg->m_CodeType = pMsg->OPERATION_ADD_INFO | pMsg->CODETYPE_NPC;
    }
    return pMsg->m_NumberOfNodes != 0;;
}
//}__NA001277_090205_NAVI_SYSTEM_SUPPORT__
//{__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
BOOLEAN
QuestManager_Concrete::OnEventNpcSummonQuestAbandon()
{
    const DWORD FOR_RESOURCE_RELEASE = 0;
    GameField* pGameField = m_pPlayer->GetField();
    S_TRY   // (WAVERIX) (090325) 한번 이 패턴을 시험삼아 사용해 보기로 한다.
    {
        if(pGameField == 0 || m_pPlayer == 0)
            S_THROW0();
        if(m_NpcSummonInfo[eNpcSummonInfo_Code] == 0 || m_NpcSummonInfo[eNpcSummonInfo_Key] == 0)
            S_THROW0();

        Object* pObject = pGameField->FindObject(MONSTER_OBJECT, m_NpcSummonInfo[eNpcSummonInfo_Key]);
        if(pObject == 0 || pObject->IsEqualObjectKind(MONSTER_OBJECT) == 0)
            S_THROW0();

        Monster* pMonster = static_cast<Monster*>(pObject);
        if(pMonster->GetBaseInfo()->m_MonsterCode != m_NpcSummonInfo[eNpcSummonInfo_Code])
            S_THROW0();

        pGameField->KillNPC(m_pPlayer, pMonster, FALSE, FALSE);
    }
    S_CATCH0()
    {
        m_NpcSummonInfo[eNpcSummonInfo_Code] = m_NpcSummonInfo[eNpcSummonInfo_Key] = 0;
        return FALSE;
    }

    m_NpcSummonInfo[eNpcSummonInfo_Code] = m_NpcSummonInfo[eNpcSummonInfo_Key] = 0;
    return TRUE;
}
