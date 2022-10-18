#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "ItemComposeListParser.h"
#include "ItemCrystalizeListParser.h"
//#include "uiItemComposite/uiItemComposite.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "uiQuestListMission/uiQuestListMission.h"
#include "uiQuestMissionList/uiQuestMissionList.h"
#include "uiQuestListContent/uiQuestListContent.h"
#include "uiQuestTimeInfo/uiQuestTimeInfo.h"
#include "QuestManager_Concrete.h"
#include "uiTimeQuest/uiTimeQuest.h"
#include "inventorydialog.h"
//-------------------------------------------------------------------------------------------
/**
*/
uiQuestMan::uiQuestMan(InterfaceManager* pUIMan) : uiBaseMan(pUIMan)
{
	m_pPendingQuest = NULL;
	SetCurSelectQuestCode(0);
	SetCurSelectMissionIndex(-1);
	SetCurSelectMissionSubIndex(-1);
    SetCurBeginDrawProgressList(0);
    SetCurPosProgressListVscr(0);    
	SetQuestViewProgress(TRUE);	

	m_pQuestPreviewPool = NULL;
	m_szMessage[0]	= NULL;
	m_szResult[0]	= NULL;
	m_szText[0]		= NULL;	
	m_pUITimeQuest	= NULL;

}

void 
uiQuestMan::OnInitialize()
{
    _LoadUI();

    SetQuestCategory( QUEST_CATEGORY_QUEST );
    SetQuestListType( QUEST_LIST_MISSION );
    SetCurSelectMissionIndex(-1);
    SetCurSelectQuestIndex(-1);
    SetCurSelectQuestCode(0);

    SetForwardAutoAlram(FALSE);
    InitPendingNPCKey();	

    if ( !m_pQuestPreviewPool )
    {
        m_pQuestPreviewPool = new util::CMemoryPoolFactory<sQuestPreview>;
        assert(m_pQuestPreviewPool);
        m_pQuestPreviewPool->Initialize( 20, 10 );
    }

#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
    refresh_quest_check_timer_ = new util::Timer;
    refresh_quest_check_pending_ = false;
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
}


//-------------------------------------------------------------------------------------------
/**
*/
void            
uiQuestMan::OnRelease()
{
	//for ( UINT i = UID_BEGIN; i <= UID_END; ++i)
	//{
	//	UnloadUI(i);
	//}

	m_PendingAutoQuestList.clear();

	clearQuestList();

	ClearQuestInfos();

	if (m_pQuestPreviewPool	)
	{		
		m_pQuestPreviewPool->Release();
		SAFE_DELETE(m_pQuestPreviewPool);
	}	

	ClearKillMonQuest4Navi();

    m_Activated_Quest_Item.clear();


#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
    SAFE_DELETE(refresh_quest_check_timer_);
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
}

//-------------------------------------------------------------------------------------------
/**
*/
void    
uiQuestMan::Process(DWORD dwTick)
{
	g_pQMGR_Real->Update();
}

//-------------------------------------------------------------------------------------------
/**
*/
void                    
uiQuestMan::SetPlayerWindowPos(const POINT& pt)
{
	m_CurPlayerWindowPoint.SetPoint(pt);	
}


//-------------------------------------------------------------------------------------------
/**
*/
const POINT&            
uiQuestMan::GetPlayerWindowPos() const
{
	return m_CurPlayerWindowPoint.GetPoint();
}

//-------------------------------------------------------------------------------------------
/**
*/
void                    
uiQuestMan::SetNPCWindowPos(const POINT& pt)
{
	m_CurNPCWindowPoint.SetPoint(pt);
}

//-------------------------------------------------------------------------------------------
/**
*/
const POINT&            
uiQuestMan::GetNPCWindowPos() const 
{
	return m_CurNPCWindowPoint.GetPoint();
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
uiQuestMan::IsPlayerWindowPosActivated() const
{
	return m_CurPlayerWindowPoint.IsActivated();
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
uiQuestMan::IsNPCWindowPosActivated() const
{
	return m_CurNPCWindowPoint.IsActivated();
}

//-------------------------------------------------------------------------------------------
/**
*/
void                    
uiQuestMan::DeactivatedPlayerWindowPos()
{
	m_CurPlayerWindowPoint.DeActivated();
}

//-------------------------------------------------------------------------------------------
/**
*/
void                    
uiQuestMan::DeactivatedNPCWindowPos()
{
	m_CurNPCWindowPoint.DeActivated();
}

//-------------------------------------------------------------------------------------------
/** 타임 퀘스트메시지찍기
*/
void	
uiQuestMan::QuestMessage(int iStringCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];	
	g_InterfaceManager.GetInterfaceString(iStringCode , szMessage ,INTERFACE_STRING_LENGTH );
	g_InterfaceManager.ShowMessageBox(szMessage, 5000, NULL, 0);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);	
}
//-------------------------------------------------------------------------------------------
/** 
*/
void		
uiQuestMan::ShowTimeQuest(BOOL bFlag)
{
	if ((g_pQMGR_Real == NULL) ||
		(m_pUITimeQuest == NULL))
	{
		return;	
	}

	if (bFlag)
	{
		if (g_pQMGR_Real->GetProgressTimeQuest())
		{
			
			if (g_pQMGR_Real->FindQuest(g_pQMGR_Real->GetProgressTimeQuest()) &&
			   (m_pUITimeQuest->IsVisible() == FALSE))
			{
				m_pUITimeQuest->ShowInterface(TRUE);
			}
		}
	}
	else
	{
		if (m_pUITimeQuest->IsVisible())
		{
			m_pUITimeQuest->ShowInterface(FALSE);
		}
	}
}
//-------------------------------------------------------------------------------------------
/** 타임퀘스트가 있는지 업데이트
*/
void
uiQuestMan::updateTimeQuest()
{
	if ((g_pQMGR_Real == NULL) ||
		(m_pUITimeQuest == NULL))
	{
		return;	
	}
	
	if (g_pQMGR_Real->GetProgressTimeQuest())
	{
		Quest* quest_info_ptr = g_pQMGR_Real->FindQuest(g_pQMGR_Real->GetProgressTimeQuest());
		
		if (quest_info_ptr)
		{
			m_pUITimeQuest->SetTimeQuest(quest_info_ptr);
			
			if (m_pUITimeQuest->IsVisible() == FALSE)
			{
				m_pUITimeQuest->ShowInterface(TRUE);
			}
		}
	}
	else
	{
		if (m_pUITimeQuest->IsVisible())
		{
			m_pUITimeQuest->SetTimeQuest(NULL);
			m_pUITimeQuest->ShowInterface(FALSE);
		}
	}
	
}

//-------------------------------------------------------------------------------------------
/**
*/
void
uiQuestMan::UpdateQuestTime()
{
#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
    if (refresh_quest_check_timer_->IsExpiredManual(FALSE) == true)
    {
        QuestCompletCheck();
    }
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::UpdateQuestInfo()
{
	if(m_pUIQuestListDlg && m_pUIQuestListDlg->IsVisible())
	{
		m_pUIQuestListDlg->RefreshQuestList();
	}

	if (m_puiQuestListMission && m_puiQuestListMission->IsVisible() )
	{
		m_puiQuestListMission->UpdateQuestInfo();		
	}

	updateViewQuestForProgress();
	updateTimeQuest();


	RefreshNPCStateForQuest(); // NPC 상태갱신 함수- 맨 마지막에 업데이트하도록 위치를 옮긴다
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::UpdateMissionInfo()
{
}
//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::EventItem()
{
	g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL );
}


void 
uiQuestMan::PreRenderInterface( DWORD dwTick )
{
	if (!g_pHero)
	{
		return;
	}

	renderViewQuestForProgress( dwTick );
}


void RefreshNPCStateForQuest()
{
	uiQuestMan * pQuestMan = 
		(uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( pQuestMan )
	{
		pQuestMan->RefreshNPCState();
	}
}


BOOL UpdateAutoAcceptRequest()
{
    uiQuestMan * pQuestMan = 
        (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
    if ( !pQuestMan )
        return FALSE; 

    if (g_pQMGR_Temp)
    {
        g_pQMGR_Temp->DestroyAll();
    }

    if (g_pQMGR_Real)
    {
        g_pQMGR_Real->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_LOAD);
    }

    if (g_pQMGR_Temp)
    {
        g_pQMGR_Temp->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_SAVE);
    }

    eQUEST_GAIN_STATE eCurState = QUEST_NONE;
    QCODE QuestCode = 1;
    for (QuestCode = 1; QuestCode < MAX_QUEST_COMPL_NUM; ++QuestCode)
    {
        eCurState = pQuestMan->GetQuestGainState(QuestCode, 0);   

        Quest * pAcceptQuest = g_pQMGR_Temp->FindQuest(QuestCode);        
        if ( QUEST_ACCEPT == eCurState &&
             pAcceptQuest && 
             eQUEST_ACCEPT_GENERAL == pAcceptQuest->GetQuestInfo()->GetQuestAcceptType())
        {
            // 자동 수락이 아니고 시작 NPC가 설정이 안되있다            
            pQuestMan->SetNPCCode(0);
			pQuestMan->InitPendingNPCKey();
            pQuestMan->OpenRewardWindow( pAcceptQuest->GetQuestInfo(), QUEST_ACCEPT);

            return TRUE;
        }
    }

    return FALSE;
}




BOOL UpdateAutoRewardRequest()
{
    uiQuestMan * pQuestMan = 
        (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
    if ( !pQuestMan )
        return FALSE; 

    if (g_pQMGR_Temp)
    {
        g_pQMGR_Temp->DestroyAll();
    }

    if (g_pQMGR_Real)
    {
        g_pQMGR_Real->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_LOAD);
    }

    if (g_pQMGR_Temp)
    {
        g_pQMGR_Temp->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_SAVE);
    }

    eQUEST_GAIN_STATE eCurState = QUEST_NONE;
    QCODE QuestCode = 1;
    for (QuestCode = 1; QuestCode < MAX_QUEST_COMPL_NUM; ++QuestCode)
    {
        eCurState = pQuestMan->GetQuestGainState(QuestCode, 0);       
        Quest * pRewardQuest = g_pQMGR_Real->FindQuest(QuestCode);
        if ( pRewardQuest && eCurState == QUEST_REWARD )
        {
            // 보상 창을 열어준다.
            pQuestMan->SetNPCCode(0);
			pQuestMan->InitPendingNPCKey();
            pQuestMan->OpenRewardWindow( pRewardQuest->GetQuestInfo(), QUEST_REWARD );

            return TRUE;
        }
    }

    return FALSE;
}


void uiQuestMan::RefreshFTextRender()
{
	if( m_puiQuestListMission->IsVisible() )
		m_puiQuestListMission->RefreshFTextRender();
}

void uiQuestMan::ClearPendingQuestCompletePacket()
{
	m_vecSendCompletPacket.clear();
}

bool uiQuestMan::SetPendingQuestCompletePacket(QCODE qcode)
{
	if(IsPendingQuestCompletePacket(qcode) == false)
	{
		m_vecSendCompletPacket.push_back(qcode);
		return true;
	}
	else
	{
		return false;
	}
}

bool uiQuestMan::IsPendingQuestCompletePacket(QCODE qcode)
{
	for(size_t i = 0; i < m_vecSendCompletPacket.size(); i++)
	{
		if( qcode == m_vecSendCompletPacket[i] )
		{
			// 이미 보냈던 패킷 이다.
			return true;
		}
	}

	return false;
	
}

void uiQuestMan::DelPendingQuestCompletePacket(QCODE qcode)
{
	std::vector<QCODE>::iterator itBegin = m_vecSendCompletPacket.begin();
	std::vector<QCODE>::iterator itEnd = m_vecSendCompletPacket.end();

	for(; itBegin != itEnd;)
	{
		if( qcode == *itBegin )
		{
			// 있으면 지우자
			itBegin = m_vecSendCompletPacket.erase(itBegin);
            itEnd = m_vecSendCompletPacket.end();
		}
        else
        {
             ++itBegin;
        }
	}
}

void uiQuestMan::AddQuestItem(QUEST_CONDITION_ITEM* pConditionItem)
{
    for (int i = 0; i < pConditionItem->GetInfo()->GetItemArrayCount(); ++i)
    {
        if (pConditionItem->GetInfo()->IsNecessaryItem())
        {
            CODETYPE NeedItemCode = pConditionItem->GetInfo()->GetItemCode(i);			
	        if (0 == NeedItemCode)
            {
		        continue;
            }
            m_Activated_Quest_Item.insert(NeedItemCode);
        }
    }
}

void uiQuestMan::DeleteQuestItem(QUEST_CONDITION_ITEM* pConditionItem)
{
    for (int i = 0; i < pConditionItem->GetInfo()->GetItemArrayCount(); ++i)
    {
        if (pConditionItem->GetInfo()->IsNecessaryItem())
        {
            CODETYPE NeedItemCode = pConditionItem->GetInfo()->GetItemCode(i);			
	        if (0 == NeedItemCode)
            {
		        continue;
            }
            m_Activated_Quest_Item.erase(NeedItemCode);
        }
    }
}

bool uiQuestMan::FindQuestItem(CODETYPE ItemCode)
{
    std::multiset<CODETYPE>::iterator iter = m_Activated_Quest_Item.find(ItemCode);
    std::multiset<CODETYPE>::iterator iter_end = m_Activated_Quest_Item.end();
    if (iter != iter_end)
    {
        return true;
    }
    return false;
}

void uiQuestMan::UpdateQuestItem(QCODE QuestCode)
{
	Quest* pQuest = g_pQMGR_Real->FindQuest(QuestCode);
    if (pQuest == NULL)
    {
        return;
    }
	
    QUEST_COND_HASH & CompleteConHash = (QUEST_COND_HASH &)pQuest->GetCompleteConditionHash();
	QUEST_COND_HASH_IT itr = CompleteConHash.begin(); 
    QUEST_COND_HASH_IT iter_end = pQuest->GetCompleteConditionHash().end();
	while (itr != iter_end)
	{
		QuestCondition* pCompleteCondition = (*itr);
		switch (pCompleteCondition->GetType())
		{
		case eQUEST_CONDITION_ITEM:			
			{
				QUEST_CONDITION_ITEM* pConditionItem = 
					(QUEST_CONDITION_ITEM *)pCompleteCondition;
                if (pConditionItem != NULL)
                {
                    AddQuestItem(pConditionItem);    
                }
			}
			break;					
		default:
			break;
		}
		++itr;
	}
    InventoryDialog* dialog_ptr = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if (dialog_ptr != NULL)
    {
        dialog_ptr->RefreshItemMark();
    }
}

void uiQuestMan::RemoveQuestItem(QCODE QuestCode)
{
	Quest* pQuest = g_pQMGR_Real->FindQuest(QuestCode);
    if(pQuest == NULL)
    {
        return;
    }
    QUEST_COND_HASH & CompleteConHash = (QUEST_COND_HASH &)pQuest->GetCompleteConditionHash();
	QUEST_COND_HASH_IT itr = CompleteConHash.begin(); 
    QUEST_COND_HASH_IT iter_end = pQuest->GetCompleteConditionHash().end();

	while (itr != iter_end)
	{
		QuestCondition* pCompleteCondition = (*itr);
		switch (pCompleteCondition->GetType())
		{
		case eQUEST_CONDITION_ITEM:			
			{
				QUEST_CONDITION_ITEM* pConditionItem = 
					(QUEST_CONDITION_ITEM *)pCompleteCondition;
                if (pConditionItem != NULL)
                {
                    DeleteQuestItem(pConditionItem);    
                }
			}
			break;					
		default:
			break;
		}
		++itr;
    }

    InventoryDialog* dialog_ptr = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if (dialog_ptr != NULL)
    {
        dialog_ptr->RefreshItemMark();
    }
}