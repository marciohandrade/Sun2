#include "SunClientPrecompiledHeader.h"

#include "QuestManager_Concrete.h"
#include <Quest.h>
#include "QuestCondition_Concrete.h"
#include "QuestAction_Concrete.h"
#include "uiQuestMan/uiQuestMan.h"
#include "InterfaceManager.h"
#ifdef _DEV_VER
#include "GlobalFunc.h"
#endif
#include "ResultCode.h"

extern FILE *			g_pQuestStreamFile = NULL;
QuestManager_Concrete *	g_pQMGR_Temp = NULL;
QuestManager_Concrete *	g_pQMGR_Real = NULL;
QUEST_PROGR_TOTAL_INFO	g_ClientQuestProgressTotalInfo;
QUEST_COMPL_TOTAL_INFO	g_ClientQuestCompleteTotalInfo;

void QuestManager_Concrete::Init( WORD wPoolSize )
{
	m_QUEST_CONDITION_REPEAT.Initialize( wPoolSize );
	m_QUEST_CONDITION_CHARLV.Initialize( wPoolSize );
	m_QUEST_CONDITION_CHARCLASS.Initialize( wPoolSize );
	m_QUEST_CONDITION_CHAO.Initialize( wPoolSize );
	m_QUEST_CONDITION_QUEST.Initialize( wPoolSize );
	m_QUEST_CONDITION_ITEM.Initialize( wPoolSize );
	m_QUEST_CONDITION_CLASSITEM.Initialize( wPoolSize );
	m_QUEST_CONDITION_MONEY.Initialize( wPoolSize );
	m_QUEST_CONDITION_KILLMONSTER.Initialize( wPoolSize );
	m_QUEST_CONDITION_MISSION.Initialize( wPoolSize );
	m_QUEST_CONDITION_AREA.Initialize( wPoolSize );
    m_QUEST_CONDITION_HONOR_RATING.Initialize( wPoolSize );
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    m_QUEST_CONDITION_MISSION_CLEAR.Initialize( wPoolSize );
#endif // _NA_006607_20130402_ADD_QUESTTYPE

	m_QUEST_ACTION_ITEM.Initialize( wPoolSize );

	QUEST_CONDITION_REPEAT::SetPool(&m_QUEST_CONDITION_REPEAT);
	QUEST_CONDITION_CHARLV::SetPool(&m_QUEST_CONDITION_CHARLV);
	QUEST_CONDITION_CHARCLASS::SetPool(&m_QUEST_CONDITION_CHARCLASS);
	QUEST_CONDITION_QUEST::SetPool(&m_QUEST_CONDITION_QUEST);
	QUEST_CONDITION_ITEM::SetPool(&m_QUEST_CONDITION_ITEM);
	QUEST_CONDITION_CLASSITEM::SetPool(&m_QUEST_CONDITION_CLASSITEM);
	QUEST_CONDITION_MONEY::SetPool(&m_QUEST_CONDITION_MONEY);
	QUEST_CONDITION_KILLMONSTER::SetPool(&m_QUEST_CONDITION_KILLMONSTER);
	QUEST_CONDITION_MISSION::SetPool(&m_QUEST_CONDITION_MISSION);
	QUEST_CONDITION_AREA::SetPool(&m_QUEST_CONDITION_AREA);
	QUEST_CONDITION_CHAO::SetPool(&m_QUEST_CONDITION_CHAO);
    QUEST_CONDITION_HONOR_RATING::SetPool(&m_QUEST_CONDITION_HONOR_RATING);
    
	QUEST_ACTION_ITEM::SetPool(&m_QUEST_ACTION_ITEM);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    QUEST_CONDITION_MISSION_CLEAR::SetPool(&m_QUEST_CONDITION_MISSION_CLEAR);
#endif // _NA_006607_20130402_ADD_QUESTTYPE
}

void QuestManager_Concrete::RecvQuestStream( MSG_CG_QUEST_STREAM_CMD * pcmd )
{
	QUEST_COMPL_TOTAL_INFO * p_compInfo = (QUEST_COMPL_TOTAL_INFO *)(((BYTE*)pcmd) + pcmd->GetSize());
	SerializeStream( *p_compInfo, QSERIALIZE_SAVE );
	QUEST_PROGR_TOTAL_INFO * p_prgInfo = (QUEST_PROGR_TOTAL_INFO *)(((BYTE*)p_compInfo) + p_compInfo->GetSize());
	SerializeStream( *p_prgInfo, QSERIALIZE_SAVE );

#ifdef _DEV_VER
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("QuestManager_Concrete::RecvQuestStream"));
#endif 
}

QuestCondition * QuestManager_Concrete::CreateQuestCondition( QuestConditionInfo * pInfo )
{
	QuestCondition * pCondition = NULL;
	switch( pInfo->GetType() )
	{
	case eQUEST_CONDITION_REPEAT:
		{
			// 풀을 사용해야 함!!!
			QUEST_CONDITION_REPEAT * pConcreteCondition = QUEST_CONDITION_REPEAT::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_REPEAT_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_CHARLV:
		{
			QUEST_CONDITION_CHARLV * pConcreteCondition = QUEST_CONDITION_CHARLV::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_CHARLV_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_CHARCLASS:	//< 진행 퀘스트, 완료 퀘스트, 제한 퀘스트, 필요 퀘스트, 완료 횟수
		{
			QUEST_CONDITION_CHARCLASS * pConcreteCondition = QUEST_CONDITION_CHARCLASS::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_CHARCLASS_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_CHAO:
		{
			QUEST_CONDITION_CHAO * pConcreteCondition = QUEST_CONDITION_CHAO::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_CHAO_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

    case eQUEST_CONDITION_HONOR_RATING:
        {
            QUEST_CONDITION_HONOR_RATING * pConcreteCondition = QUEST_CONDITION_HONOR_RATING::ALLOC();
            pConcreteCondition->SetInfo( (QUEST_CONDITION_HONOR_RATING_INFO *)pInfo );
            pCondition = pConcreteCondition;
        }
        break;

	case eQUEST_CONDITION_QUEST:		//< 해당클래스, 제한 아이템, 필요아이템, 개수
		{
			QUEST_CONDITION_QUEST * pConcreteCondition = QUEST_CONDITION_QUEST::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_QUEST_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_ITEM:
		{
			QUEST_CONDITION_ITEM * pConcreteCondition = QUEST_CONDITION_ITEM::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_ITEM_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_MONEY:
		{
			QUEST_CONDITION_MONEY * pConcreteCondition = QUEST_CONDITION_MONEY::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_MONEY_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_KILLMONSTER:
		{
			QUEST_CONDITION_KILLMONSTER * pConcreteCondition = QUEST_CONDITION_KILLMONSTER::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_KILLMONSTER_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_CLASSITEM:
		{
			QUEST_CONDITION_CLASSITEM * pConcreteCondition = QUEST_CONDITION_CLASSITEM::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_CLASSITEM_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_MISSION:
		{
			QUEST_CONDITION_MISSION * pConcreteCondition = QUEST_CONDITION_MISSION::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_MISSION_INFO *)pInfo );
			pCondition = pConcreteCondition;
		}
		break;

	case eQUEST_CONDITION_AREA:
		{
			QUEST_CONDITION_AREA * pConcreteCondition = QUEST_CONDITION_AREA::ALLOC();
			pConcreteCondition->SetInfo( (QUEST_CONDITION_AREA_INFO *)pInfo );
			pConcreteCondition->Clear();
			pCondition = pConcreteCondition;
		}
		break;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        // 클라에서도 만들어 준다.
    case eQUEST_CONDITION_MISSION_CLEAR:
        {
            QUEST_CONDITION_MISSION_CLEAR * pConcreteCondition = QUEST_CONDITION_MISSION_CLEAR::ALLOC();
            pConcreteCondition->SetInfo( (QUEST_CONDITION_MISSION_CLEAR_INFO *)pInfo);
            pCondition = pConcreteCondition;
        }
        break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE
	}

	return pCondition;
} 

QuestAction * QuestManager_Concrete::CreateQuestAction( QuestActionInfo * pInfo )
{
	QuestAction * pAction = NULL;
	switch( pInfo->GetType() )
	{
	case eQUEST_ACTION_ITEM:
		{
			QUEST_ACTION_ITEM * pConcreteAction = QUEST_ACTION_ITEM::ALLOC();
			pConcreteAction->SetInfo( (QUEST_ACTION_ITEM_INFO *)pInfo );
			pAction = pConcreteAction;
		}
		break;
	}
	return pAction;
}

void QuestManager_Concrete::DestroyQuestCondition( QuestCondition * pCondition )
{
	switch( pCondition->GetType() )
	{
	case eQUEST_CONDITION_REPEAT:		QUEST_CONDITION_REPEAT::FREE( (QUEST_CONDITION_REPEAT *)pCondition ); break;
	case eQUEST_CONDITION_CHARLV:		QUEST_CONDITION_CHARLV::FREE( (QUEST_CONDITION_CHARLV *)pCondition ); break;
	case eQUEST_CONDITION_CHARCLASS:	QUEST_CONDITION_CHARCLASS::FREE( (QUEST_CONDITION_CHARCLASS *)pCondition ); break;	//< 진행 퀘스트, 완료 퀘스트, 제한 퀘스트, 필요 퀘스트, 완료 횟수
    case eQUEST_CONDITION_CHAO:         QUEST_CONDITION_CHAO::FREE( (QUEST_CONDITION_CHAO *)pCondition ); break;
	case eQUEST_CONDITION_QUEST:		QUEST_CONDITION_QUEST::FREE( (QUEST_CONDITION_QUEST *)pCondition ); break;			//< 해당클래스, 제한 아이템, 필요아이템, 개수
	case eQUEST_CONDITION_ITEM:			QUEST_CONDITION_ITEM::FREE( (QUEST_CONDITION_ITEM*)pCondition ); break;
	case eQUEST_CONDITION_MONEY:		QUEST_CONDITION_MONEY::FREE( (QUEST_CONDITION_MONEY *)pCondition ); break;
	case eQUEST_CONDITION_KILLMONSTER:	QUEST_CONDITION_KILLMONSTER::FREE( (QUEST_CONDITION_KILLMONSTER *)pCondition ); break;
    case eQUEST_CONDITION_CLASSITEM:    QUEST_CONDITION_CLASSITEM::FREE( (QUEST_CONDITION_CLASSITEM *)pCondition ); break;
	case eQUEST_CONDITION_MISSION:		QUEST_CONDITION_MISSION::FREE( (QUEST_CONDITION_MISSION *)pCondition ); break;
	case eQUEST_CONDITION_AREA:			QUEST_CONDITION_AREA::FREE( (QUEST_CONDITION_AREA *)pCondition ); break;
    case eQUEST_CONDITION_HONOR_RATING: QUEST_CONDITION_HONOR_RATING::FREE( (QUEST_CONDITION_HONOR_RATING *)pCondition ); break;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    case eQUEST_CONDITION_MISSION_CLEAR:QUEST_CONDITION_MISSION_CLEAR::FREE( (QUEST_CONDITION_MISSION_CLEAR *)pCondition); break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE
	}

}
void QuestManager_Concrete::DestroyQuestAction( QuestAction * pAction )
{
	switch( pAction->GetType() )
	{
	case eQUEST_ACTION_ITEM: QUEST_ACTION_ITEM::FREE((QUEST_ACTION_ITEM*)pAction); break;
	}
}

void QuestManager_Concrete::OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest )
{
	// TODO : 퀘스트의 상태에 따라 적절한 처리의 코드가 필요하다.

	switch( evt )
	{
	case QUEST_EVENT_ITEM:		// 인벤토리에 아이템 획득 / 삭제시
		break;

	case QUEST_EVENT_CHARLV:	// 레벨업 시
		break;

	case QUEST_EVENT_QUEST:		// 수락 / 보상 완료시 체크
		break;

	case QUEST_EVENT_MONEY:		// 돈 획득시
		break;

	case QUEST_EVENT_CHAO:
		break;

	case QUEST_EVENT_TIME:		// Update시 계속
		return;

	case QUEST_EVENT_MISSION:
		break;

	case QUEST_EVENT_MONSTERKILL: // 서버에서 패킷준다.
		break;

	case QUEST_EVENT_AREA:
		{
			QUEST_AREA_MSG * pRecvMsg = (QUEST_AREA_MSG *)pMsg;

			if( pQuest->GetQState() == QS_NORMAL )
			{
				uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
				if (pQuestMan && pQuest)
				{					
					pQuestMan->NET_SEND_CG_QUEST_EVENT_AREA_SYN(
						pQuest->GetQuestCode(), 
						pRecvMsg->m_AreaID );
				}
			}

			return;
		}
		break;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    case QUEST_EVENT_MISSION_CLEAR:
        break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE

	default:
		break;
	}

	// 퀘스트 정보 Update
	uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( pQuestMan )
	{

		if( pQuest )
		{
			if(QS_NORMAL == pQuest->GetQState() && RC::RC_QUEST_SUCCESS == pQuest->CanComplete( g_pHero )) 
			{
				//QUEST_COMPLETE_SYN 보냄
				pQuestMan->NET_SEND_CG_QUEST_COMPLETE_SYN(pQuest->GetQuestCode());
			}
			else if(QS_COMPLETED == pQuest->GetQState() && RC::RC_QUEST_SUCCESS != pQuest->CanComplete( g_pHero ))
			{
				pQuest->SetQState(QS_NORMAL);
			}
		}

		pQuestMan->UpdateQuestInfo();
	}
}

void 
QuestManager_Concrete::OnEventQuestTime(Quest* pQuest)
{

	if (pQuest->GetQState() == QS_FAILED)
	{
		uiQuestMan* pQuestMan = (uiQuestMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
		if (pQuestMan && pQuest)
		{
			pQuestMan->NET_SEND_CG_QUEST_FAILED_SYN(pQuest->GetQuestInfo()->GetQuestCode());

			//타임퀘스트의 경우 자동소멸패킷도 보냄
			if (pQuest->IsTimeQuest())
			{
				pQuestMan->NET_SEND_CG_QUEST_ABANDON_SYN(pQuest->GetQuestCode(), TRUE);
			}
		}
	}
}


void 
QuestManager_Concrete::OnEventForClient(eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest)
{
    if (pQuest == NULL)
    {
        return;
    }

    if (pQuest->GetQuestInfo()->IsChildQuest())
	{
		Quest* pParentQuest = 
			g_pQMGR_Real->FindQuest(pQuest->GetQuestInfo()->GetParentQuestCode());

		assert(pParentQuest);        

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
        // no operation
#else
		QUEST_INFO_HASH_ITR itr(pParentQuest->GetQuestInfo()->GetChildQuestInfoHash().begin()),
			itrend(pParentQuest->GetQuestInfo()->GetChildQuestInfoHash().end());

		for (; itr != itrend; itr++)
		{
			QuestInfo * pInfo = (*itr);
			Quest * pQuestChild = g_pQMGR_Real->FindQuest(pInfo->GetQuestCode());
			if ( pQuestChild->GetQState() == QS_NORMAL)
			{
				OnEvent( QUEST_EVENT_ITEM, NULL, pQuestChild );
			}							
		}
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING

        if (pParentQuest != NULL)
        {
            OnEvent(QUEST_EVENT_ITEM, NULL, pParentQuest );
        }
	}
    else
    {
        OnEvent(QUEST_EVENT_ITEM, NULL, pQuest);
    }
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void QuestManager_Concrete::RecvGroupQuestInfo(MSG_CG_QUEST_TODAYQUEST_INFO_CMD* group_quest_info)
{
    group_quest_map_.clear();
    
    if (group_quest_info == NULL)
    {
        return;
    }

    WORD quest_group = 0;
    WORD quest_code = 0;
    for (int info_index = 0; info_index < group_quest_info->count_; ++info_index)
    {
        quest_group = group_quest_info->today_quest_info[info_index].quest_group;
        quest_code = group_quest_info->today_quest_info[info_index].quest_code;

        group_quest_map_.insert(GROUP_QUEST_MAP::value_type(quest_group, quest_code));
    }
}

QCODE QuestManager_Concrete::GetTodayQuestCode(WORD group_code)
{
    GROUP_QUEST_MAP::iterator found_itr = group_quest_map_.find(group_code);
    if (found_itr != group_quest_map_.end())
    {
        return found_itr->second;
    }

    return 0;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM