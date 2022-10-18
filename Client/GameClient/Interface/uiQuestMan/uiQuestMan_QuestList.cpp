#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "npcInfoParser.h"
#include "uiQuestCommonForward/uiQuestCommonForward.h"
#include "QuestManager_Concrete.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "uiQuestListMission/uiQuestListMission.h"
#include "uiQuestReward/uiQuestReward.h"
#include "GameFunc.h"
#include "uiQuestAutoAcceptAlram/uiQuestAutoAcceptAlram.h"
#include "Application.h"
#include "questinfomanagerex.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "DlgPosInfoParser.h"
#include "Map.h"
#include "globalfunc.h"
#include "ObjectManager.h"
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#include "uiQuestList/uiQuestList.h"
#endif//_NA_004644_20110506_QUEST_NAVIGATION
//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
uiQuestMan::IsOpenQuestList()
{
	return m_pUIQuestListDlg->IsVisible();
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
uiQuestMan::IsOpenQuestNPC()
{
	return BOOL( m_pUIQuestCommonForward->IsVisible() || 
				 m_pUIQuestReward->IsVisible() );

}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::CloseQuestList()
{
	if(m_pUIQuestListDlg->IsVisible())
		m_pUIQuestListDlg->ShowInterface(FALSE);
	g_KeyQueueManager.DeleteMsg( eDoType_dialog_Quest_Player );
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::CloseQuestNPC()
{
	if ( m_pUIQuestReward->IsVisible() )
	{
		m_pUIQuestReward->ShowInterface(FALSE);		
	}

	if ( m_pUIQuestCommonForward->IsVisible() )
	{
		m_pUIQuestCommonForward->ShowInterfaceWithSound(FALSE);
	}

	g_KeyQueueManager.DeleteMsg( eDoType_dialog_Quest_NPC );
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::OpenQuestList()
{
	BOOL bRet = FALSE;
	switch (this->GetQuestCategory())
	{
	case QUEST_CATEGORY_QUEST:
		{
			if ( m_pUIQuestListDlg->IsVisible() )
				m_pUIQuestListDlg->ShowInterface(FALSE);
			else
				m_pUIQuestListDlg->ShowInterfaceWithSound(TRUE);

		}
		break;

	case QUEST_CATEGORY_MISSION:
		{
			m_pUIQuestListDlg->ShowInterfaceWithSound(TRUE);
		}
		break;

	default:
		assert(!"여기 올수 없는데 어떻게 왔다냐?!!!");
		return;
		break;
	}

	if ( bRet )
	{
		g_KeyQueueManager.DeleteMsg( eDoType_dialog_Quest_Player );

		// key queueing
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_Quest_Player;
		msg.wParam = QUEST_WINDOW_PLAYER;
		msg.DoSomething = GlobalFunc::DoShowQuestWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Quest_Player);
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
void 
uiQuestMan::OpenForceQuestList()
{
	this->SetQuestCategory(QUEST_CATEGORY_QUEST);

	this->OpenQuestList();	
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::OpenForceMissionList()
{
	this->SetQuestCategory(QUEST_CATEGORY_MISSION);

	this->OpenQuestList();
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::UpdateQuestAlram(BOOL bFlag )
{
	if (bFlag)
	{
		if (this->GetAcceptAutoQuestCount())
		{
			m_pUIAutoAcceptAlram->ShowInterface(TRUE);
		}		
		else
		{
			m_pUIAutoAcceptAlram->ShowInterface(FALSE);
		}
	}
	else
	{
		m_pUIAutoAcceptAlram->ShowInterface(FALSE);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
uiQuestMan::AddQuestForProgress( QCODE QuestCode )
{
	BOOL bRet = TRUE;

	if ( !g_pQMGR_Real )
	{
		return bRet;
	}

	Quest * pQuest = g_pQMGR_Real->FindQuest( QuestCode );
	if ( !pQuest )
	{
		return bRet;
	}

	if ( isExistQuestForProgress( QuestCode ) )
	{
		// 존재하는 걸 다시 넣을려고 하면 지워라 ( 기획요청 )
		DelQuestForProgress( QuestCode );
	}
	else
	{
		QuestInfo * pQuestInfo = pQuest->GetQuestInfo();
		if (!pQuestInfo)
		{
			return FALSE;
		}

		if ( pQuestInfo->IsChildQuest() )
		{
			// 자식 퀘스트는 따로 추가되지 않는다.
			// 메시지는 따로 출력해준다.
			return FALSE;
		}
		
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
        // no operation
#else
		if ( pQuestInfo->IsUnionQuest() )
		{
			int iAddCountQuest = 1;
			QCODE Q1 = QuestCode;
			while(1) 
			{
				++Q1;
				QuestInfo * pQuestChildInfo = g_QuestInfoManager.FindQuestInfo( Q1 );
				if ( !pQuestChildInfo ) break;
				if ( !pQuestChildInfo->IsChildQuest() ) break;

				++iAddCountQuest;
			};
			
			if ( MAX_QUEST_PREVIEW_COUNT < m_QuestViewProgressMap.size() + iAddCountQuest )
			{
				// 최고 퀘스트 카운트가 넘어 섰다.
				//5378	더 이상 퀘스트 진행상황 리스트에 추가할 수 없습니다.
				g_InterfaceManager.GetInterfaceString( 5378, m_szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szMessage );
				return FALSE;
			}

			m_QuestViewProgressMap.insert( QUEST_VIEW_PROGRESS_PAIR(QuestCode, pQuest) );			

			// 연합 퀘스트들의 자식 퀘를 추가한다.
			Q1 = QuestCode;				
			while(1) 
			{
				++Q1;
				Quest * pQuest = g_pQMGR_Real->FindQuest( Q1 );
				QuestInfo * pQuestChildInfo = g_QuestInfoManager.FindQuestInfo( Q1 );
				if ( !pQuest || !pQuestChildInfo ) break;
				if ( pQuestChildInfo->IsChildQuest() )	
					m_QuestViewProgressMap.insert( QUEST_VIEW_PROGRESS_PAIR(Q1, pQuest) );
			};
		}
		else
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING
		{
			if ( MAX_QUEST_PREVIEW_COUNT < m_QuestViewProgressMap.size() + 1 )
			{
				// 최고 퀘스트 카운트가 넘어 섰다.
				//5378	더 이상 퀘스트 진행상황 리스트에 추가할 수 없습니다.
				g_InterfaceManager.GetInterfaceString( 5378, m_szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szMessage );
				return FALSE;			
			}

			m_QuestViewProgressMap.insert( QUEST_VIEW_PROGRESS_PAIR(QuestCode, pQuest) );
		}		
	}
	
	updateViewQuestForProgress();

	return TRUE; 
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::DelQuestForProgress( QCODE QuestCode )
{
	QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo( QuestCode );
	if ( !pQuestInfo )
	{
		return;
	}

	if ( pQuestInfo->IsChildQuest() )
	{
		// 자식 퀘스트는 따로 삭제할수 없다.
		// 메시지는 따로 출력해준다.
		return;
	}

	if ( 0 == m_QuestViewProgressMap.size() )
	{
		return;
	}

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    // no operation
#else
	if ( pQuestInfo->IsUnionQuest() )
	{
		QUEST_VIEW_PROGRESS_ITR itr = m_QuestViewProgressMap.find( QuestCode );
		if ( itr != m_QuestViewProgressMap.end() )
		{
			m_QuestViewProgressMap.erase( itr++ );

			while (1)
			{						
				QCODE Q1 = itr->first;
				QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo( QuestCode );
				if ( !pQuestInfo )
					break;

				if ( !pQuestInfo->IsChildQuest() )
					break;

				if ( itr == m_QuestViewProgressMap.end() )
					break;

				m_QuestViewProgressMap.erase( itr++ );
			}			
		}
	}
	else
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING
	{
		QUEST_VIEW_PROGRESS_ITR itr = m_QuestViewProgressMap.find( QuestCode );
		if ( itr != m_QuestViewProgressMap.end() )
		{
			m_QuestViewProgressMap.erase( itr );
		}
	}

	updateViewQuestForProgress();
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::ClearQuestInfos()
{		
	SetQuestViewProgressRect();
	m_QuestViewProgressSize.cx = m_QuestViewProgressSize.cy = 0;

	clearViewQuestForProgressContents();
	clearViewQuestForProgress();
}

void uiQuestMan::SetQuestViewProgressRect()
{
	DlgPosInfoParser::Instance()->GetDlgPoint(StrToWzID("0838"), &m_PtQuestViewProgress);
}

//-------------------------------------------------------------------------------------------
/**
*/
sQuestPreview * 
uiQuestMan::allocQuestPreview()
{
	if (m_pQuestPreviewPool)
	{
		return (sQuestPreview *) m_pQuestPreviewPool->Alloc();
	}
	else
	{
		return NULL;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::freeQuestPreview( sQuestPreview * pInfo )
{
	if (m_pQuestPreviewPool)
	{
		m_pQuestPreviewPool->Free(pInfo);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
uiQuestMan::isExistQuestForProgress( QCODE QuestCode )
{
	QUEST_VIEW_PROGRESS_ITR itr = m_QuestViewProgressMap.find( QuestCode );

	return BOOL( itr != m_QuestViewProgressMap.end() ); 
}

//-------------------------------------------------------------------------------------------
/**
*/
int GetOrderOfPriority(QuestCondition* quest_condition)
{
    //우선순위
    if (quest_condition)
    {
        switch (quest_condition->GetType())
        {
        case eQUEST_CONDITION_QUEST:
            return ORDER_CONDITION_QUEST;
        case eQUEST_CONDITION_KILLMONSTER:
            return ORDER_CONDITION_MONSTER;
        case eQUEST_CONDITION_ITEM:
            return ORDER_CONDITION_ITEM;
        case eQUEST_CONDITION_AREA:
            return ORDER_CONDITION_AREA;
        default:
            return ORDER_CONDITION_DEFAULT;
        }
    }
    return ORDER_CONDITION_DEFAULT;
}

//-------------------------------------------------------------------------------------------
/**
*/
bool SortViewQuestForProgress(QuestCondition* a_condition,
                              QuestCondition* b_condition)
{
    int order_of_priority_a = GetOrderOfPriority(a_condition);
    int order_of_priority_b = GetOrderOfPriority(b_condition);
    
    if (order_of_priority_a > order_of_priority_b)
    {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::updateViewQuestForProgress()
{
	if ( !g_pHero )
	{
		return;
	}

	clearViewQuestForProgressContents();

	g_pApplication->Set2DMode();

	QuestCompletCheck();

	// 등록된 퀘스트들을 돌면서 정보를 만든다.
	// 최대 Width, Height를 구한다.
	SIZE FontSize = {0,};	
	QUEST_VIEW_PROGRESS_ITR itr = m_QuestViewProgressMap.begin();

	while ( itr != m_QuestViewProgressMap.end() )
    {
		QCODE QuestCode = (QCODE)itr->first;

		Quest * pQuest = g_pQMGR_Real->FindQuest( QuestCode );	// 리얼에서 찾아서 보여주자 혹시 몰라서리 이러케함.

		if (pQuest)
		{
			QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo( QuestCode );
			if ( pQuestInfo )
			{
				// 퀘스트 이름.
				insertQuestNameForProgContents( pQuest );

#ifdef _SCJ_120104_SORT_QUEST_VIEW_PROGRESS
                // #3889 퀘스트 수락 후 퀘스트 추적 항목 순서를 우선순위에 따라 정렬
                quest_condition_vector.clear();
                // 정렬을 위해 벡터로 옴김

                QUEST_COND_HASH& complete_condition_hash =
                    static_cast<QUEST_COND_HASH&>(pQuest->GetCompleteConditionHash());
                QUEST_COND_HASH_IT itr = complete_condition_hash.begin(); 
                while ( itr != pQuest->GetCompleteConditionHash().end()	)
                {
                    QuestCondition* complete_condition = (*itr);
                    quest_condition_vector.push_back(complete_condition);
                    itr++;
                }

                // 정렬
                std::sort(quest_condition_vector.begin(),
                    quest_condition_vector.end(),
                    SortViewQuestForProgress);

                // 정렬된 임무내용(x/max)
                STLX_VECTOR<QuestCondition*>::iterator vector_itr =
                    quest_condition_vector.begin();
                while (vector_itr != quest_condition_vector.end())
                {
                    QuestCondition* pCompleteCondition = (*vector_itr);
#else
                // 임무내용(x/max)			
                QUEST_COND_HASH & CompleteConHash = (QUEST_COND_HASH & )pQuest->GetCompleteConditionHash();
                QUEST_COND_HASH_IT itr = CompleteConHash.begin(); 
                while ( itr != pQuest->GetCompleteConditionHash().end()	)
                {
                    QuestCondition * pCompleteCondition = (*itr);
#endif //_SCJ_120104_SORT_QUEST_VIEW_PROGRESS

					switch ( pCompleteCondition->GetType() )
					{
					case eQUEST_CONDITION_ITEM:			
						{
							QUEST_CONDITION_ITEM * pConditionItem = 
								(QUEST_CONDITION_ITEM *)pCompleteCondition;
							insertConditionItemForProgContents(pConditionItem);
						}
						break;
					
					case eQUEST_CONDITION_KILLMONSTER:
						{
							QUEST_CONDITION_KILLMONSTER * pCondition = 
								(QUEST_CONDITION_KILLMONSTER *)pCompleteCondition;
							insertConditionMonForProgContents(pCondition);
						}
						break;
					case eQUEST_CONDITION_QUEST:
						{
							QUEST_CONDITION_QUEST* pCondition = 
								(QUEST_CONDITION_QUEST*)pCompleteCondition;
							insertConditionQuestForProgContents(pCondition);
						}
						break;

					case eQUEST_CONDITION_AREA:
						{
							//탐사 조건들 삽입
							QUEST_CONDITION_AREA* pCondition = 
								(QUEST_CONDITION_AREA*)pCompleteCondition;
							insertConditionAreaQuestForProgContents(pCondition, pQuestInfo);
						}
						break;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
                    case eQUEST_CONDITION_MISSION_CLEAR:
                        {
                            QUEST_CONDITION_MISSION_CLEAR* pCondition =
                                (QUEST_CONDITION_MISSION_CLEAR*)pCompleteCondition;
                            insertConditionMissionClearForProgContents(pCondition, pQuestInfo);
                        }
                        break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE

					default:
						break;
					}
#ifdef _SCJ_120104_SORT_QUEST_VIEW_PROGRESS
                    ++vector_itr;
#else
					++itr;
#endif //_SCJ_120104_SORT_QUEST_VIEW_PROGRESS
				}

				m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;
			}				
		}

		itr++;
	}
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    uiQuestList* quest_list = GET_CAST_DIALOG(uiQuestList, IM_QUEST_MANAGER::QUEST_LIST);
    if (quest_list)
    {
        quest_list->SetQuestList();
    }
#endif//_NA_004644_20110506_QUEST_NAVIGATION
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::renderViewQuestForProgress( DWORD dwTick )
{
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#else
	if ( !g_InterfaceManager.IsShowGameInterface() || !g_InterfaceManager.GetShowFlag() )
		return;

	if ( g_pMap && !g_pMap->IsRender()	)
	{
		return;
	}

	if ( !GetQuestViewProgress() )
		return;

	RECT rcView;

	g_pApplication->Set2DMode();

    SIZE FontSize = {0,0};

	rcView.left		= m_PtQuestViewProgress.x - ( m_QuestViewProgressSize.cx + TEXT_MARGIN_FOR_PREVIEW );
	rcView.top		= m_PtQuestViewProgress.y + TEXT_MARGIN_FOR_PREVIEW;
	rcView.right	= m_PtQuestViewProgress.x;	
	rcView.bottom	= m_PtQuestViewProgress.y + ( m_QuestViewProgressSize.cy + TEXT_MARGIN_FOR_PREVIEW );

	//rcView.left		= m_PtQuestViewProgress.x - ( m_QuestViewProgressSize.cx);
	//rcView.top		= m_PtQuestViewProgress.y + TEXT_MARGIN_FOR_PREVIEW;
	//rcView.right	= m_PtQuestViewProgress.x;	
	//rcView.bottom	= m_PtQuestViewProgress.y + ( m_QuestViewProgressSize.cy + TEXT_MARGIN_FOR_PREVIEW);
	
	for ( unsigned int i = 0; i < m_QuestViewProgContentMap.size(); ++i )
	{
		sQuestPreview * pInfo = (sQuestPreview *)m_QuestViewProgContentMap[i];

		if ( !pInfo ) continue;

		switch (pInfo->m_eType)
		{
		case eQUEST_PREVIEW_TITLE:
			{
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_CATEGORY );
			}
			break;

		default:
		case eQUEST_PREVIEW_MISSION:
			{
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL);
			}
			break;
		}
		TCHAR * pszContents = (TCHAR *)(const TCHAR *)pInfo->m_strContent.c_str();

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( pszContents, _tcslen(pszContents), &FontSize );		
        g_pSunRenderer->x_pManagerTextOut->DrawText( pszContents, &rcView, pInfo->m_wcColor, WzColor_RGBA(0,0,0,0), TP_HLEFT, ETS_SHADOW );
		rcView.top += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	}
#endif//_NA_004644_20110506_QUEST_NAVIGATION
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::clearViewQuestForProgressContents()
{	
	m_QuestViewProgressSize.cx = m_QuestViewProgressSize.cy = 0;

	std::vector<sQuestPreview *>::iterator	it;
	for( it = m_QuestViewProgContentMap.begin() ; it != m_QuestViewProgContentMap.end() ; it++ )
	{
		sQuestPreview * pInfo = *it;
		pInfo->m_strContent.clear();
		freeQuestPreview(pInfo);
	}

	m_QuestViewProgContentMap.clear();	
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::clearViewQuestForProgress()
{
	m_QuestViewProgressMap.clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::insertQuestNameForProgContents(Quest * pQuest )
{
	if (!pQuest)
		return;

	QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo( pQuest->GetQuestCode() );
	if ( !pQuestInfo )
	{
		return;
	}

	if ( !pQuestInfo->IsChildQuest() )
	{
		sQuestPreview * pPreviewInfo = allocQuestPreview();
		if (NULL == pPreviewInfo)
			return;
		SIZE FontSize;

		// 퀘스트 이름					
		DWORD dwQuestStrID = pQuestInfo->GetQNCode();					

		GetUIMan()->GetInterfaceString(dwQuestStrID, m_szText, INTERFACE_STRING_LENGTH);

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
        pPreviewInfo->quest_code = pQuest->GetQuestCode();
#endif//_NA_004644_20110506_QUEST_NAVIGATION

		switch ( pQuest->GetQState() )
		{
		case QS_COMPLETED:
			{
                pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
                pPreviewInfo->state = QS_COMPLETED;
#else
				// (완료) ( 9051 )
				GetUIMan()->GetInterfaceString( 9051, m_szMessage, INTERFACE_STRING_LENGTH ); 
				_tcscat(m_szText, m_szMessage);
#endif//_NA_004644_20110506_QUEST_NAVIGATION
			}
			break;
		case QS_FAILED:
			{
                pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_LIST_FAILED;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
                pPreviewInfo->state = QS_FAILED;
#else
				// (실패) ( 9052 )
				GetUIMan()->GetInterfaceString( 9052, m_szMessage, INTERFACE_STRING_LENGTH ); 
				_tcscat(m_szText, m_szMessage );
#endif//_NA_004644_20110506_QUEST_NAVIGATION
			}
			break;

		default:
		case QS_NORMAL:
			{
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
                pPreviewInfo->state = QS_NORMAL;
#endif//_NA_004644_20110506_QUEST_NAVIGATION
				pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_NAME;
			}									
			break;
		}

		pPreviewInfo->m_strContent = m_szText;					
		pPreviewInfo->m_eType = eQUEST_PREVIEW_TITLE;

		m_QuestViewProgContentMap.push_back(pPreviewInfo);

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_CATEGORY );
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szText, _tcslen(m_szText), &FontSize );
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		m_QuestViewProgressSize.cx = max(m_QuestViewProgressSize.cx, FontSize.cx);
		m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;
	}
}
//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::RefreshConditionItemForProgContents(QCODE quest_code)
{
    Quest* quest_pointer = g_pQMGR_Real->FindQuest(quest_code);
    if (RC::RC_QUEST_SUCCESS == quest_pointer->CanComplete(g_pHero))
    {
        if(QS_NORMAL == quest_pointer->GetQState())
        {
            NET_SEND_CG_QUEST_COMPLETE_SYN(quest_code);
        }
    }
    else
    {
        if (QS_COMPLETED == quest_pointer->GetQState())
        {
            quest_pointer->SetQState(QS_NORMAL);

            uiQuestCommonForward* quest_common_forward =
                GET_CAST_DIALOG(uiQuestCommonForward, IM_QUEST_MANAGER::QUEST_COMMON_FORWARD);
            if (quest_common_forward && quest_common_forward->IsVisible())
            {
                quest_common_forward->ShowInterface(FALSE);
            }
        }
    }
}
//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
bool uiQuestMan::ConditionItemCompleteCheck(QCODE quest_code, SLOTCODE item_code)
{
    Quest* quest = g_pQMGR_Real->FindQuest(quest_code);
    if (quest == NULL)
    {
        return false;
    }

    QuestInfo* quest_info = quest->GetQuestInfo();
    if (quest_info == NULL)
    {
        return false;
    }

    // 임무내용(x/max)
    QUEST_COND_HASH& complete_condition_hash =
        static_cast<QUEST_COND_HASH&>(quest->GetCompleteConditionHash());
    QUEST_COND_HASH_IT itr = complete_condition_hash .begin();
    QUEST_COND_HASH_IT itr_end = complete_condition_hash.end();
    while (itr != itr_end)
    {
        QuestCondition* complete_condition = (*itr);
        if (complete_condition->GetType() == eQUEST_CONDITION_ITEM)
        {
            QUEST_CONDITION_ITEM * condition_item = 
                static_cast<QUEST_CONDITION_ITEM*>(complete_condition);
            if (condition_item->GetInfo()->IsNecessaryItem())
            {
                // 지정해준 아이템을 모으는 수집퀘스트인지
                for (BYTE i = 0; i < condition_item->GetInfo()->GetItemArrayCount(); ++i)
                {
                    if (item_code == condition_item->GetInfo()->GetItemCode(i))
                    {
                        if (ItemManager::Instance()->IsExistItem(
                            SI_INVENTORY,
                            condition_item->GetInfo()->GetItemCode(i),
                            condition_item->GetInfo()->GetItemNum(i)))
                        {
                            return true;
                        }
                        // 해당 아이템을 수집하는 퀘스트가 하나라고 단정한다면 리턴함
                        return false;
                    }
                }
            }
        }
        ++itr;
    }

    return false;

}
#endif //_NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::QuestCompletCheck()
{
	std::vector<DWORD> pQuestCodes = m_pUIQuestListDlg->GetMainQuestCodes();

	for(unsigned int i = 0; i < pQuestCodes.size(); ++i)
	{			
		QCODE quest_code = (QCODE)pQuestCodes[i];

		Quest* quest_pointer = g_pQMGR_Real->FindQuest(quest_code);	// 리얼에서 찾아서 보여주자 혹시 몰라서리 이러케함.

		if (quest_pointer)
		{
			QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(quest_code);
			if (quest_info)
			{
                // 임무내용(x/max)
                QUEST_COND_HASH& complete_condition_hash  = 
                    static_cast<QUEST_COND_HASH&>(quest_pointer->GetCompleteConditionHash());
                QUEST_COND_HASH_IT itr = complete_condition_hash .begin();
                QUEST_COND_HASH_IT itr_end = complete_condition_hash.end();
                while (itr != itr_end)
                {
                    QuestCondition* complete_condition = (*itr);
                    if (complete_condition->GetType() == eQUEST_CONDITION_ITEM)
                    {
                        QUEST_CONDITION_ITEM * condition_item = 
                            static_cast<QUEST_CONDITION_ITEM*>(complete_condition);
                        if (condition_item->GetInfo()->IsNecessaryItem())
                        {
                            RefreshConditionItemForProgContents(quest_code);
                            break;
                        }
                    }
                    ++itr;
                }
			}
		}
	}

	pQuestCodes = m_pUIQuestListDlg->GetSubQuestCodes();

	for(unsigned int i = 0; i < pQuestCodes.size(); ++i)
	{
		QCODE quest_code = (QCODE)pQuestCodes[i];

		Quest* quest_pointer = g_pQMGR_Real->FindQuest(quest_code);	// 리얼에서 찾아서 보여주자 혹시 몰라서리 이러케함.

		if (quest_pointer)
		{
			QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(quest_code);
			if (quest_info)
			{
                // 임무내용(x/max)
                QUEST_COND_HASH& complete_condition_hash  = 
                    static_cast<QUEST_COND_HASH&>(quest_pointer->GetCompleteConditionHash());
                QUEST_COND_HASH_IT itr = complete_condition_hash .begin();
                QUEST_COND_HASH_IT itr_end = complete_condition_hash.end();
                while (itr != itr_end)
                {
                    QuestCondition* complete_condition = (*itr);
                    if (complete_condition->GetType() == eQUEST_CONDITION_ITEM)
                    {
                        QUEST_CONDITION_ITEM * condition_item = 
                            static_cast<QUEST_CONDITION_ITEM*>(complete_condition);
                        if (condition_item->GetInfo()->IsNecessaryItem())
                        {
                            RefreshConditionItemForProgContents(quest_code);
                            break;
                        }
                    }
                    ++itr;
                }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------
/**
*/
void
uiQuestMan::insertConditionItemForProgContents(QUEST_CONDITION_ITEM * pConditionItem)
{
	if (!pConditionItem)
		return;

	for ( int i = 0; i < pConditionItem->GetInfo()->GetItemArrayCount(); ++i)
	{
		if ( pConditionItem->GetInfo()->IsNecessaryItem() )
		{			
			CODETYPE NeedItemCode = pConditionItem->GetInfo()->GetItemCode(i);			
			WORD NeedItemNum = pConditionItem->GetInfo()->GetItemNum(i);      

			if ( 0 == NeedItemCode ) 
				continue;

			BASE_ITEMINFO * pItemInfo = 
				(BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(NeedItemCode);
			if (!pItemInfo )
			{
				assert(pItemInfo);
				continue;
			}

			sQuestPreview * pPreviewInfo = allocQuestPreview();
			if (NULL == pPreviewInfo)
				break;
			SIZE FontSize;

			pPreviewInfo->m_eType = eQUEST_PREVIEW_MISSION;

			WORD RealItemNum = ItemManager::Instance()->GetItemCount( SI_INVENTORY, NeedItemCode );

			// [어떤 아이템]    min(NeedItemNum, RealItemNum) / RealItemNum; 

			GetUIMan()->GetInterfaceString(pItemInfo->m_NCode, m_szText, INTERFACE_STRING_LENGTH);
			Sprintf(m_szResult, 
				_T("  %s    (%d / %d)"),
				m_szText,
				min(NeedItemNum, RealItemNum), 
				NeedItemNum );

			if (RC::RC_QUEST_SUCCESS == pConditionItem->CanComplete(g_pHero))
			{
				pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
			}
			else
			{
				// 퀘스트가 불가능할때.... 가능하던 것이 아이템을 버려서 안될경우 서버에 보내야하는가? 처음에 모으지 않았을때는?
				pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT;
			}  			

			g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL );
			pPreviewInfo->m_strContent = m_szResult;		
			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szResult, _tcslen(m_szResult), &FontSize );
			g_pSunRenderer->x_pManagerTextOut->EndUseFont();

			m_QuestViewProgressSize.cx = max( m_QuestViewProgressSize.cx, FontSize.cx );
			m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;

			m_QuestViewProgContentMap.push_back(pPreviewInfo);
		}
		else
		{
			// 아이템이 없어야한다.
			CODETYPE NeedItemCode = pConditionItem->GetInfo()->GetItemCode(i);
			WORD NeedItemNum = pConditionItem->GetInfo()->GetItemNum(i);

			BOOL bIsExist = ItemManager::Instance()->IsExistItem( SI_INVENTORY, NeedItemCode, NeedItemNum);

			bIsExist;
			// [어떤 아이템] [%d] 개를 가지고 있지 말라. 
		}    		
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::insertConditionMonForProgContents(QUEST_CONDITION_KILLMONSTER * pConditionKillMon)
{
	if (!pConditionKillMon)
		return;

	TCHAR szGrade[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szLevel[INTERFACE_STRING_LENGTH + 1] = {0,};
	GetUIMan()->GetInterfaceString(1668, szLevel); //1668 레벨

	for ( int i = 0; i < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++i)
	{
		CODETYPE MonsterCode = pConditionKillMon->GetInfo()->GetMonsterCode(i);
		DWORD MonsterGrade = pConditionKillMon->GetInfo()->GetMonsterGrade(i);
		DWORD MonsterLevel = pConditionKillMon->GetInfo()->GetMonsterLevel(i);


		if (!MonsterCode && !MonsterGrade && !MonsterLevel)
			break;
		
		if (MonsterCode)
		{
			BASE_NPCINFO * pInfo = NPCInfoParser::Instance()->GetNPCInfo(MonsterCode);
			if (pInfo)
			{
#ifdef _DH_NPC_GROUP_NAME
                if (pInfo->group_name != 0)
                {
                    TCHAR group_name[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR monster_name[INTERFACE_STRING_LENGTH] = {0,};
                    GetUIMan()->GetInterfaceString(pInfo->group_name, group_name, INTERFACE_STRING_LENGTH);
                    GetUIMan()->GetInterfaceString(pInfo->m_NCode, monster_name, INTERFACE_STRING_LENGTH);
                    Snprintf(m_szText, INTERFACE_STRING_LENGTH, "<%s> %s", group_name, monster_name);
                }
                else
#endif//_DH_NPC_GROUP_NAME
                {
				    GetUIMan()->GetInterfaceString(pInfo->m_NCode, m_szText, INTERFACE_STRING_LENGTH);			
                }
#ifdef _DEV_VER
                //#ifdef _SCJ_SHOW_NPC_ID
                if (CGeneralGameParam::Instance()->IsShowNpcId())
                {
                    TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                    StrnCopy(buffer_string, m_szText, INTERFACE_STRING_LENGTH-1); 
                    Snprintf(m_szText, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                        buffer_string, pInfo->m_MonsterCode);
                }
                //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

				MonsterGrade = pInfo->m_byGrade;
				MonsterLevel = pInfo->m_DisplayLevel;
			}
			else
				break;
		}
		else
		{
			Sprintf(m_szText, "");
		}

		sQuestPreview * pPreviewInfo = allocQuestPreview();
		if (NULL == pPreviewInfo)
			break;
		SIZE FontSize;

		WORD	 wRequireMonsterNum = pConditionKillMon->GetInfo()->GetMonsterNum(i);
		WORD	 wCurMonsterNum = pConditionKillMon->GetCurMonsterNum(i);

		pPreviewInfo->m_eType = eQUEST_PREVIEW_MISSION;

		if ( RC::RC_QUEST_SUCCESS == pConditionKillMon->CanComplete(g_pHero) )
		{
			pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
		}
		else
		{
			pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT;
		}

		switch(MonsterGrade)
		{
		case eNPC_GENERAL:			//11501	일반
		case eNPC_MAPOBJECT_NPC:	// 2009.08.14 - 신규퀘스트에서 몬스터사냥 조건으로 맵오브젝트NPC가 추가되어 일반등급으로 처리
									// (타겟창에서 이미 맵오브젝트NPC 의 경우 일반으로 출력을 하고 있음)
			g_InterfaceManager.GetInterfaceString(11501, szGrade);
			break;
		case eNPC_ELITE: //11503	정예
			g_InterfaceManager.GetInterfaceString(11503, szGrade);
			break;
		case eNPC_LEADER: //11502	리더
			g_InterfaceManager.GetInterfaceString(11502, szGrade);
			break;
		case eNPC_MIDDLEBOSS: //11504	캡틴
			g_InterfaceManager.GetInterfaceString(11504, szGrade);
			break;
		case eNPC_BOSS: //11505	보스
			g_InterfaceManager.GetInterfaceString(11505, szGrade);
			break;
		case eNPC_LUCKY_MONSTER: //11500	럭키
			g_InterfaceManager.GetInterfaceString(11500, szGrade);
			break;

		default:
			break;
		}

		Sprintf(m_szResult, 
			_T("  [%s%d/%s] %s    (%d / %d)"), 
			szLevel,
			MonsterLevel,
			szGrade,
			m_szText, 
			min(wCurMonsterNum,wRequireMonsterNum), 
			wRequireMonsterNum );

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL );
		pPreviewInfo->m_strContent = m_szResult;		
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szResult, _tcslen(m_szResult), &FontSize );
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		m_QuestViewProgressSize.cx = max( m_QuestViewProgressSize.cx, FontSize.cx );
		m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;

		m_QuestViewProgContentMap.push_back(pPreviewInfo);
	}    
}

void uiQuestMan::insertConditionQuestForProgContents(QUEST_CONDITION_QUEST * pConditionQuest)
{
	if(!pConditionQuest)
		return;

	for ( int i = 0; i < pConditionQuest->GetInfo()->GetQuestArrayCount(); ++i)
	{
		CODETYPE QuestCode = pConditionQuest->GetInfo()->GetQuestCode(i);
		if (!QuestCode)
			break;

		Quest* pQ1 = g_pQMGR_Real->FindQuest((QCODE)QuestCode);
		Quest* pQ2 = g_pQMGR_Temp->FindQuest((QCODE)QuestCode);
		Quest* pQ = NULL;
		if(pQ1)
			pQ = pQ1;
		else if(pQ2)
			pQ = pQ2;

		if(pQ)
		{
			sQuestPreview * pPreviewInfo = allocQuestPreview();
			if (NULL == pPreviewInfo)
				break;
			SIZE FontSize;

			pPreviewInfo->m_eType = eQUEST_PREVIEW_MISSION;

			TCHAR szSuc[INTERFACE_STRING_LENGTH] = {0,};
			QUESTPART_COMPL* pQC = g_pQMGR_Real->FindComplQuest((QCODE)QuestCode);
			if(NULL != pQC && 0 != pQC->m_Num) //완료
			{
				pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#else
				//9051	(완료)
				g_InterfaceManager.GetInterfaceString(9051, szSuc);
#endif//_NA_004644_20110506_QUEST_NAVIGATION
			}
			else
			{
				pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT;
			} 

			GetUIMan()->GetInterfaceString(pQ->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);

			Sprintf(m_szResult, 
				_T("  %s%s"), 
				m_szText,
				szSuc);

			g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL );
			pPreviewInfo->m_strContent = m_szResult;		
			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szResult, _tcslen(m_szResult), &FontSize );
			g_pSunRenderer->x_pManagerTextOut->EndUseFont();

			m_QuestViewProgressSize.cx = max( m_QuestViewProgressSize.cx, FontSize.cx );
			m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;		

			m_QuestViewProgContentMap.push_back(pPreviewInfo);
		}
	}    
}


void uiQuestMan::insertConditionAreaQuestForProgContents(QUEST_CONDITION_AREA* pConditionArea, QuestInfo * pQuestInfo)
{
	if(!pConditionArea || !pQuestInfo)
		return;

	for ( int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
	{
		DWORD AreaID = pConditionArea->GetInfo()->GetAreaID(Index);
		if (!AreaID)
			break;
		
		sQuestPreview * pPreviewInfo = allocQuestPreview();
		if (NULL == pPreviewInfo)
			return;
		SIZE FontSize;

		TCHAR szSucs[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR szAreaText[INTERFACE_STRING_LENGTH] = {0, };
		pPreviewInfo->m_eType = eQUEST_PREVIEW_MISSION;
		

		if ( TRUE == pConditionArea->GetAreaCondition(Index) )
		{
			pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#else
			g_InterfaceManager.GetInterfaceString(9051, szSucs);
#endif//_NA_004644_20110506_QUEST_NAVIGATION
		}
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#else
		else
		{
			pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT;
		}
#endif//_NA_004644_20110506_QUEST_NAVIGATION

		g_InterfaceManager.GetInterfaceString(pQuestInfo->GetTISMissionText(Index), szAreaText);

		Sprintf(m_szResult, _T("  %s %s"), szAreaText,	szSucs);

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL );
		pPreviewInfo->m_strContent = m_szResult;		
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szResult, _tcslen(m_szResult), &FontSize );
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		m_QuestViewProgressSize.cx = max( m_QuestViewProgressSize.cx, FontSize.cx );
		m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;		

		m_QuestViewProgContentMap.push_back(pPreviewInfo);
	}
}


void uiQuestMan::SetNaviQuest(Quest* pQ)
{
	if(m_pUIQuestListDlg)
	{
		m_pUIQuestListDlg->SetNaviQuest(pQ);
	}
}

void uiQuestMan::ClearKillMonQuest4Navi()
{
	int idx = m_vecKillMonQuest4Navi.size();
	for(int i = 0; i < idx; ++i)
	{
		SAFE_DELETE(m_vecKillMonQuest4Navi[i]);
	}
	m_vecKillMonQuest4Navi.clear();
}

void uiQuestMan::SetKillMonQuest4Navi(DWORD QCode, DWORD MonCode, POINT MonPos)
{
	int iSize = m_vecKillMonQuest4Navi.size();
	if(0 == iSize)
	{
		KILL_MON_QUEST_INFO_4_NAVI* pTemp = new KILL_MON_QUEST_INFO_4_NAVI;
		pTemp->dwQuestCode = QCode;
		pTemp->dwMonCode[0] = MonCode;
		pTemp->ptMonPos[0] = MonPos;

		m_vecKillMonQuest4Navi.push_back(pTemp);
	}
	else
	{
		BOOL bFind = FALSE;
		for(int i = 0; i < iSize; ++i)
		{
			KILL_MON_QUEST_INFO_4_NAVI* pTemp = m_vecKillMonQuest4Navi[i];
			if(pTemp->dwQuestCode == QCode)
			{
				for(int k = 0; k < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM; k++)
				{
					if(0 == pTemp->dwMonCode[k])
					{
						bFind = TRUE;
						pTemp->dwMonCode[k] = MonCode;
						pTemp->ptMonPos[k] = MonPos;
						break;
					}
				}
			}
		}

		if(FALSE == bFind)
		{
			KILL_MON_QUEST_INFO_4_NAVI* pTemp = new KILL_MON_QUEST_INFO_4_NAVI;
			pTemp->dwQuestCode = QCode;
			pTemp->dwMonCode[0] = MonCode;
			pTemp->ptMonPos[0] = MonPos;

			m_vecKillMonQuest4Navi.push_back(pTemp);
		}
	}
}

void uiQuestMan::RemoveKillMonQuest4Navi(DWORD quest_code)
{
    std::vector<KILL_MON_QUEST_INFO_4_NAVI*>::iterator found_itr = m_vecKillMonQuest4Navi.begin();
    std::vector<KILL_MON_QUEST_INFO_4_NAVI*>::iterator end_itr = m_vecKillMonQuest4Navi.end();
    for( ; found_itr != end_itr; ++found_itr)
    {
        KILL_MON_QUEST_INFO_4_NAVI* found_navi_ptr = *found_itr;
        if(found_navi_ptr->dwQuestCode == quest_code)
        {
            m_vecKillMonQuest4Navi.erase(found_itr);
            break;
        }
    }
}

void uiQuestMan::SEND_CG_COLLECT_ALLCOLLECTIVE_SYN()
{
	MSG_CG_COLLECT_ALLCOLLECTIVE_SYN SendMsg;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

	Object_Map_Itr itr = g_ObjectManager.GetBegin();
	MapObject* pMapObj = NULL;
	while(itr != g_ObjectManager.GetEnd() )
	{
		Object * pObject = static_cast<Object *>(itr->second);

		if(NULL == pObject)
		{
			++itr;
			continue;
		}

		if(FALSE == pObject->IsKindOfObject(MAP_OBJECT))
		{
			++itr;
			continue;
		}

		pMapObj = (MapObject*)pObject;
		pMapObj->SetCollectionID(0);

		++itr;
	}
}

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
void uiQuestMan::insertConditionMissionClearForProgContents(QUEST_CONDITION_MISSION_CLEAR* pConditionMission, QuestInfo * pQuestInfo)
{
    for (int i = 0; i < QUEST_CONDITION_MISSION_CLEAR::MAX_MISSION_CLEAR_CHECK_NUM; ++i)
    {
        MAPCODE map_id = pConditionMission->GetInfo()->GetMissionCode(i);
        DWORD max_clear_number = pConditionMission->GetInfo()->GetMissionNum(i);

        if (!map_id || !max_clear_number)
        {
            continue;
        }

        sQuestPreview * pPreviewInfo = allocQuestPreview();
        if (NULL == pPreviewInfo)
            return;

        SIZE FontSize;
        pPreviewInfo->m_eType = eQUEST_PREVIEW_MISSION;

        DWORD current_mission_clear_number;
        if ( RC::RC_QUEST_SUCCESS == pConditionMission->CanComplete(g_pHero) )
        {
            pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
            current_mission_clear_number = max_clear_number;
        }
        else
        {
            pPreviewInfo->m_wcColor = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT;
            current_mission_clear_number = pConditionMission->GetCurMissionClearNum(i);
        }

        //지역명
        TCHAR map_name[INTERFACE_STRING_LENGTH] = {0, };
        sMAPINFO * area_name = MapInfoParser::Instance()->FindMapInfo(map_id);
        if (area_name)
        {
            g_InterfaceManager.GetInterfaceString(area_name->dwNCode, map_name);
        }

        sprintf(m_szResult, "%s (%d / %d)", map_name, current_mission_clear_number, max_clear_number);

        g_pSunRenderer->x_pManagerTextOut->StartToUseFont( FONT_QUEST_LIST_GENERAL );
        pPreviewInfo->m_strContent = m_szResult;
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( m_szResult, _tcslen(m_szResult), &FontSize );
        g_pSunRenderer->x_pManagerTextOut->EndUseFont();

        m_QuestViewProgressSize.cx = max( m_QuestViewProgressSize.cx, FontSize.cx );
        m_QuestViewProgressSize.cy += FontSize.cy + TEXT_HEIGHT_GAP_FOR_PREVIEW;

        m_QuestViewProgContentMap.push_back(pPreviewInfo);
    }
}
#endif // _NA_006607_20130402_ADD_QUESTTYPE
