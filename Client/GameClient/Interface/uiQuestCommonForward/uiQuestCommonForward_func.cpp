//------------------------------------------------------------------------------
//  uiQuestCommonForward_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "Quest.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestCommonForward.h"
#include "ItemComposeListParser.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "NPCInfoParser.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "GameFunc.h"
#include "NPCVoiceInfoParser.h"
#include "QuestInfoManagerEx.h"
#include "Hero.h"

#include <atltime.h>

//------------------------------------------------------------------------------
/**
*/
uiQuestMan*
uiQuestCommonForward::GetManager()
{
    uiQuestMan* puiQuestMan =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::ClearInfo()
{
	if ( !this->CheckControls() )
		return;

	m_VecForward.clear();
}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::initInfo()
{
	assert(CheckControls());
	if ( !CheckControls() )
	{	
		return;
	}

	hideAllBtn();

	this->ClearInfo();
	
	EXTRA_NPCINFO * pInfo = 
		NPCInfoParser::Instance()->GetExtraInfo(this->GetManager()->GetNPCCode());
	if ( pInfo )
	{
		if ( pInfo->m_dwJobStrID )
		{
			GetUIMan()->GetInterfaceString( pInfo->m_dwJobStrID, m_szResult, INTERFACE_STRING_LENGTH);
			m_pBTN_Job->ShowWindowWZ(WZ_SHOW);
			m_pBTN_Tab->ShowWindowWZ(WZ_SHOW);
			GlobalFunc::SetCaption(m_pBTN_Job, m_szResult);
		}
		else
		{
		   m_pBTN_Job->ShowWindowWZ(WZ_HIDE);
		   m_pBTN_Tab->ShowWindowWZ(WZ_HIDE);
		}

		sFORWARD_INFO ForwardInfo;
		ForwardInfo.type = pInfo->m_eNPCTYPE;
		m_VecForward.push_back(ForwardInfo);

		BASE_NPCINFO * pInfo = 
			NPCInfoParser::Instance()->GetNPCInfo( GetManager()->GetNPCCode() );

		if ( pInfo )
		{
//			GetCtrlTitle()->SetTextColor(TITLE_TEXT_COLOR);
			GetUIMan()->GetInterfaceString( pInfo->m_NCode, m_szResult, INTERFACE_STRING_LENGTH);
#ifdef _DEV_VER
            //#ifdef _SCJ_SHOW_NPC_ID
            if (CGeneralGameParam::Instance()->IsShowNpcId())
            {
                TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                StrnCopy(buffer_string, m_szResult, INTERFACE_STRING_LENGTH-1); 
                Snprintf(m_szResult, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                    buffer_string, pInfo->m_MonsterCode);
            }
            //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
			this->SetCaptionTitle(m_szResult);		
			this->ShowTitle(TRUE);
			
		}
		else
		{
			this->ShowTitle(FALSE);
		}
	}
	else
	{
		this->ShowTitle(FALSE);
	}

	initQuestInfo();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::initQuestInfo()
{	
	GetCtrlQuestList()->DeleteItemAll();

	GetCtrlQuestList()->SetColorText(TEXT_COLOR_QUEST_LIST_GENERAL);
	GetCtrlQuestList()->SetColorSelBar(BAR_COLOR_SELECT_LIST);
	GetCtrlQuestList()->SetColorSelBarOnText(TEXT_COLOR_ON_SELECT_LIST);
	GetCtrlQuestList()->SetIsOutline(FALSE);
	GetCtrlQuestList()->SetUseReport(FALSE);
	GetCtrlQuestList()->SetFontID(FONT_QUEST_LIST_GENERAL);


	registerConversation();
	registerMissionList();
	registerRewardList();
#ifdef _GS_ADD_QUEST_SCROLL_1143
	registerScrollRange();
#endif//_GS_ADD_QUEST_SCROLL_1143
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    registerDailyQuestInitDate();
#endif //_NA_003027_20111013_HONOR_SYSTEM
}

void					
uiQuestCommonForward::registerConversation()
{
	BASE_NPCVoiceInfo * pInfo = 
		NPCVoiceInfoParser::Instance()->GetNPCVoiceInfo( GetManager()->GetNPCCode() );

	m_FTextRender.ClearSentence();

	if (pInfo)
	{
		//int iVoiceIndex = rand() % MAX_NPC_VOICE_SAMPLE;
		int iVoiceIndex = NPC::GetNpcVoiceIndex();

		TCHAR szText[512];
		ZeroMemory(szText,512);

		DWORD dwTextID = pInfo->m_Meet[iVoiceIndex].dwTextID;

		if (dwTextID) 
		{
			GetUIMan()->GetInterfaceString(dwTextID,szText,512);
			if ( m_FTextParser.Parser(szText, m_FTextSentence) )
			{
				m_FTextRender.Append(m_FTextSentence);
			}
		}
		else
		{
			if ( pInfo->m_Meet[0].dwTextID )
			{
				GetUIMan()->GetInterfaceString(pInfo->m_Meet[0].dwTextID,szText,512);
				if ( m_FTextParser.Parser(szText, m_FTextSentence) )
				{
					m_FTextRender.Append(m_FTextSentence);
				}
			}
		}
	};
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::registerMissionList()
{
	int i = 0;
	int index = 0;
	QUEST_LIST::const_iterator itr;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if ( GetManager()->GetCanAcceptList().size() )
	{
		index = GetCtrlQuestList()->InsertItemBack();

		// 9003 : 임무
		g_InterfaceManager.GetInterfaceString(9003,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::SetItem(
			GetCtrlQuestList(), 
			index, 0, szMessage, 
			FONT_QUEST_LIST_CATEGORY, 
			DT_VCENTER | DT_LEFT, 
			TEXT_BG_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_ON_SELECT_LIST );

		GetCtrlQuestList()->SetItemData(index, 0);

		// 수락받을 수 있는 목록
		itr = GetManager()->GetCanAcceptList().begin();
		for ( ; itr != GetManager()->GetCanAcceptList().end(); ++itr)
		{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            QCODE quest_code = *itr;

            Quest* pQuest = g_pQMGR_Temp->FindQuest(quest_code);
            if (NULL == pQuest)
            {
                pQuest = g_pQMGR_Real->FindQuest(quest_code);
            }
#else

			Quest * pQuest = *itr;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
			assert(pQuest);

			if(QUEST_CATEGORY_MISSION == pQuest->GetQType())
			{
				//9023	메인
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9023));
			}
			else if(QUEST_CATEGORY_QUEST == pQuest->GetQType())
			{
				//9024	서브
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9024));
			}
			
			WzColor wcTextColor = TEXT_COLOR_QUEST_LIST_GENERAL;
			WzColor wcSelectColor = TEXT_COLOR_ON_SELECT_LIST;

			QUEST_COND_HASH & AcceptConHash = pQuest->GetAcceptContionHash();
			QUEST_COND_HASH_IT itr = AcceptConHash.begin(); 
			while(itr != AcceptConHash.end())
			{
				QuestCondition * pAcceptCondition = (*itr);
				if(eQUEST_CONDITION_CHARLV == pAcceptCondition->GetType())
				{
					QUEST_CONDITION_CHARLV* pQCC = (QUEST_CONDITION_CHARLV*)pAcceptCondition;
					if(g_pHero->GetLevel() < pQCC->GetInfo()->GetMinCharLV())
					{
						if( MAX_SHOW_QUEST_LEVEL_GAP > (pQCC->GetInfo()->GetMinCharLV() - g_pHero->GetLevel()) )
						{
							wcTextColor   = TEXT_COLOR_QUEST_LIST_DEACTIVE;
							wcSelectColor = TEXT_COLOR_QUEST_LIST_DEACTIVE;
						}
					}
					break;
				}

				++itr;
			}

			GetUIMan()->GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);
			_tcscat(m_szResult, m_szText);
			index = GetCtrlQuestList()->InsertItemBack();

			GlobalFunc::CovertSimpleText(m_szResult,sizeof(m_szResult),FONT_QUEST_LIST_GENERAL,GetCtrlQuestList()->GetSizeRect());

			GlobalFunc::SetItem(
				GetCtrlQuestList(),
				index,0,m_szResult,
				FONT_QUEST_LIST_GENERAL,
				DT_VCENTER | DT_LEFT, 
				TEXT_BG_COLOR_QUEST_LIST_GENERAL,
				wcTextColor,
				wcSelectColor);

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            GetCtrlQuestList()->SetItemData(index, (VOID*)pQuest->GetQuestCode());
#else
            GetCtrlQuestList()->SetItemData(index, pQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
		}
	}
}
//-------------------------------------------------------------------------------------------
/**	스크롤바 영역을 설정한다.
*/
void uiQuestCommonForward::registerScrollRange()
{
	if(!m_pScroll) return;

	//리스트갯수를구한후 최대뿌릴수있는 갯수(제목빼고 11개)
	int iPage = GetCtrlQuestList()->GetListRowCnt()-MAX_QUEST_LIST_LINE;
	
	//1보다 작다면 스크롤 필요없다.
	if(iPage<1) iPage=0; 

	m_pScroll->SetScrollRangeWZ(0,iPage);

	GetCtrlQuestList()->GetListRowCnt();

}
//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::registerRewardList()
{
	int i = 0;
	int index = 0;
	QUEST_LIST::const_iterator itr;

	if ( GetManager()->GetCompleteList().size() + 
		 GetManager()->GetInperfectCompleteList().size() + 
		 GetManager()->GetProgressList().size())
	{
		//////////////////////////////////////////////////////////////////////////	
		// 보상	
        
        // 9006 보상
        GetUIMan()->GetInterfaceString( 9006, m_szText, INTERFACE_STRING_LENGTH);
		index = GetCtrlQuestList()->InsertItemBack();
		GlobalFunc::SetItem(
			GetCtrlQuestList(), 
			index, 0, m_szText, 
			FONT_QUEST_LIST_CATEGORY, 
			DT_VCENTER | DT_LEFT, 
			TEXT_BG_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_ON_SELECT_LIST );

		GetCtrlQuestList()->SetItemData(index, 0);

		TCHAR szTemp[INTERFACE_STRING_LENGTH];

		// (완료) : 9051
		g_InterfaceManager.GetInterfaceString(9051 , szTemp , INTERFACE_STRING_LENGTH);
		// 보상 받을 수 있는 목록
		itr = GetManager()->GetCompleteList().begin();
		for ( ; itr != GetManager()->GetCompleteList().end(); ++itr)
		{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            QCODE quest_code = *itr;

            Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
            if (NULL == pQuest)
            {
                pQuest = g_pQMGR_Temp->FindQuest(quest_code);
            }
#else
			Quest * pQuest = *itr;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

			assert(pQuest);

			if(QUEST_CATEGORY_MISSION == pQuest->GetQType())
			{
				//9023	메인
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9023));
			}
			else if(QUEST_CATEGORY_QUEST == pQuest->GetQType())
			{
				//9024	서브
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9024));
			}

			GetUIMan()->GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);
			_tcscat(m_szResult, m_szText);
			_tcscat(m_szResult, szTemp);
			index = GetCtrlQuestList()->InsertItemBack();
			GlobalFunc::CovertSimpleText(m_szResult,sizeof(m_szResult),FONT_QUEST_LIST_GENERAL,GetCtrlQuestList()->GetSizeRect());
			GlobalFunc::SetItem(
				GetCtrlQuestList(),
				index,0,m_szResult,
				FONT_QUEST_LIST_GENERAL,
				DT_VCENTER | DT_LEFT, 
				TEXT_BG_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_ON_SELECT_LIST);

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            GetCtrlQuestList()->SetItemData(index, (VOID*)pQuest->GetQuestCode());
#else
            GetCtrlQuestList()->SetItemData(index, pQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
		}

		// 진행중인 퀘스트 목록
		itr = GetManager()->GetProgressList().begin();
		for ( ; itr != GetManager()->GetProgressList().end(); ++itr)
		{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            QCODE quest_code = *itr;

            Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
            if (NULL == pQuest)
            {
                pQuest = g_pQMGR_Temp->FindQuest(quest_code);
            }
#else
			Quest * pQuest = *itr;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

			assert(pQuest);

			if(QUEST_CATEGORY_MISSION == pQuest->GetQType())
			{
				//9023	메인
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9023));
			}
			else if(QUEST_CATEGORY_QUEST == pQuest->GetQType())
			{
				//9024	서브
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9024));
			}

			GetUIMan()->GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);
			_tcscat(m_szResult, m_szText);
			index = GetCtrlQuestList()->InsertItemBack();
		GlobalFunc::CovertSimpleText(m_szResult,sizeof(m_szResult),FONT_QUEST_LIST_GENERAL,GetCtrlQuestList()->GetSizeRect());
			GlobalFunc::SetItem(
				GetCtrlQuestList(),
				index,0,m_szResult,
				FONT_QUEST_LIST_GENERAL,
				DT_VCENTER | DT_LEFT, 
				TEXT_BG_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_ON_SELECT_LIST);	

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            GetCtrlQuestList()->SetItemData(index, (VOID*)pQuest->GetQuestCode());
#else
            GetCtrlQuestList()->SetItemData(index, pQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
		}

		// 미완료 하여 보상 받지 못하는 목록
		itr = GetManager()->GetInperfectCompleteList().begin();
		for ( ; itr != GetManager()->GetInperfectCompleteList().end(); ++itr)
		{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            QCODE quest_code = *itr;

            Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
            if (NULL == pQuest)
            {
                pQuest = g_pQMGR_Temp->FindQuest(quest_code);
            }
#else
			Quest * pQuest = *itr;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

			assert(pQuest);

			if(QUEST_CATEGORY_MISSION == pQuest->GetQType())
			{
				//9023	메인
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9023));
			}
			else if(QUEST_CATEGORY_QUEST == pQuest->GetQType())
			{
				//9024	서브
				Sprintf(m_szResult, "  [%s] ", g_InterfaceManager.GetInterfaceString(9024));
			}

			GetUIMan()->GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);
			_tcscat(m_szResult, m_szText);
			index = GetCtrlQuestList()->InsertItemBack();
			GlobalFunc::CovertSimpleText(m_szResult,sizeof(m_szResult),FONT_QUEST_LIST_GENERAL,GetCtrlQuestList()->GetSizeRect());
			GlobalFunc::SetItem(
				GetCtrlQuestList(),
				index,0,m_szResult,
				FONT_QUEST_LIST_GENERAL,
				DT_VCENTER | DT_LEFT, 
				TEXT_BG_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_QUEST_LIST_GENERAL,
				TEXT_COLOR_ON_SELECT_LIST);

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            GetCtrlQuestList()->SetItemData(index, (VOID*)pQuest->GetQuestCode());
#else
            GetCtrlQuestList()->SetItemData(index, pQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
		}
	}
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void uiQuestCommonForward::registerDailyQuestInitDate()
{
    CControlWZ* init_date_control = GetControlWZ(StrToWzID("S001"));
    if (init_date_control == NULL)
    {
        return;
    }
    init_date_control->ShowWindowWZ(WZ_HIDE);

    if (g_pQMGR_Real == NULL)
    {
        return;
    }

    NPC* npc_ptr = static_cast<NPC*>(g_ObjectManager.GetObject(GetManager()->GetPendingNPCKey()));
    if ((GetManager()->GetCompleteDailyQuestList().size() == 0) && 
        ((npc_ptr != NULL) && (npc_ptr->IsHaveDailyQuest() == false)))
    {
        return;
    }
    
    CTime current_time = util::TimeSync::_time64(NULL);
    CTime daily_init_time = CTime(current_time.GetYear(), 
        current_time.GetMonth(), 
        current_time.GetDay(), 
        4, 
        0, 
        0);
    if (daily_init_time < current_time)
    {
        daily_init_time += CTimeSpan(1, 0, 0, 0);
    }

    CTimeSpan remain_init_time = daily_init_time - current_time;
    
    TCHAR notice_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR remain_hours_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR remain_minutes_string[INTERFACE_STRING_LENGTH] = {'\0', };

    _i64toa(remain_init_time.GetTotalHours(), remain_hours_string, 10);
    _i64toa(remain_init_time.GetMinutes(), remain_minutes_string, 10);

    g_InterfaceManager.GetInterfaceStringFormat(notice_string, 
        INTERFACE_STRING_LENGTH, 
        6499, 
        remain_hours_string, 
        remain_minutes_string);
    init_date_control->ShowWindowWZ(WZ_SHOW);
    init_date_control->AddStyle(TP_WORDBREAK);
    GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(init_date_control), notice_string);
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::updateInfo()
{
	this->m_FTextRender.SetRect( GetCtrlConversation()->GetSizeRect() );
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::hideAllBtn()
{
	assert(CheckControls());
	if ( !CheckControls() )
	{	
		return;
	}

    this->m_pBTN_Job->ShowWindowWZ(WZ_HIDE);
    this->m_pBTN_Tab->ShowWindowWZ(WZ_HIDE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
uiQuestCommonForward::clickBtnJob(POSTYPE pos)
{
	pos = pos - eDLG_BTN_JOB;
	if ( m_VecForward.size() > pos && pos >= 0)
	{
		SolarDialog * pDlg = GameFunc::OpenNPCMeetDialog( 
			GetManager()->GetMapID(), 
			GetManager()->GetFieldID(),
			GetManager()->GetNPCCode());

		NPC * pNPC = (NPC *)g_ObjectManager.GetObject( GetManager()->GetPendingNPCKey() );
		if ( pNPC && pDlg)
		{
			pNPC->SetOpenDialog(pDlg);
			pDlg->SetOwnerKey(GetManager()->GetPendingNPCKey());			
		}	

		this->ShowInterface(FALSE);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestCommonForward::clickList()
{
	int iCurSel = this->GetCtrlQuestList()->GetCurSel();

	if (-1 == iCurSel )
	{
		return;
	}

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
    QCODE quest_code = (QCODE)GetCtrlQuestList()->GetItemData(iCurSel);
    Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
    if (NULL == pQuest)
    {
        pQuest = g_pQMGR_Temp->FindQuest(quest_code);
    }
#else
    Quest * pQuest = (Quest *)GetCtrlQuestList()->GetItemData(iCurSel);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
	if ( !pQuest )
	{
		return;
	}	

    if (g_pHero == NULL)
    {
        return;
    }

    eQUEST_GAIN_STATE quest_gain_state = 
        GetManager()->GetStateInQuestListForForward(pQuest->GetQuestCode());
    int quest_accept_value = 0;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if (pQuest->GetQuestInfo()->IsDailyQuest() == TRUE)
    {
        if (g_pQMGR_Real != NULL)
        {
            DWORD today_daily_quest_count;
            DWORD today_complete_count = 0;

            if (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_NORMAL)
            {
                today_daily_quest_count = 
                    g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_NORMAL, &today_complete_count);
                // 하루 완료 제한 체크
                if (quest_gain_state == QUEST_REWARD)
                {
                    if (today_complete_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
                    {
                        quest_accept_value = kCompletedDailyQuestMaxCount;
                    }
                }
                else if (today_daily_quest_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
                {
                    quest_accept_value = kCantAcceptDailyQuestMaxCount;
                }
            }
            else if (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD)
            {
                today_daily_quest_count = 
                    g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_GUILD, &today_complete_count);
                // 하루 완료 제한 체크
                if (quest_gain_state == QUEST_REWARD)
                {
                    if (today_complete_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
                    {
                        quest_accept_value = kCompletedGuildDailyQuestMaxCount;
                    }
                }
                else if (today_daily_quest_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
                {
                    quest_accept_value = kCantAcceptGuildDailyQuestMaxCount;
                }
            }
        }
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	WORD wAcceptLv = 0;
	QUEST_COND_HASH & AcceptConHash = pQuest->GetAcceptContionHash();
	QUEST_COND_HASH_IT itr = AcceptConHash.begin(); 
	while(itr != AcceptConHash.end())
	{
		QuestCondition * pAcceptCondition = (*itr);
		if(eQUEST_CONDITION_CHARLV == pAcceptCondition->GetType())
		{
			QUEST_CONDITION_CHARLV* pQCC = (QUEST_CONDITION_CHARLV*)pAcceptCondition;
			if(pQCC && (g_pHero->GetLevel() < pQCC->GetInfo()->GetMinCharLV()))
			{
				if( MAX_SHOW_QUEST_LEVEL_GAP > (pQCC->GetInfo()->GetMinCharLV() - g_pHero->GetLevel()) )
				{
					wAcceptLv = pQCC->GetInfo()->GetMinCharLV();
					quest_accept_value = kCantAcceptLevel;
				}
			}
			break;
		}

		++itr;
	}

    GetCtrlQuestList()->DeleteItemAll();
    m_FTextRender.ClearSentence();

    switch (quest_accept_value)
    {
    case kCantAcceptLevel:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
            TCHAR szQuestName[INTERFACE_STRING_LENGTH + 1] = {0,};
            TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

            g_InterfaceManager.GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(), szQuestName);
            g_InterfaceManager.GetInterfaceString(1851, szMessage);
            Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szQuestName, wAcceptLv);

            if ( m_FTextParser.Parser(szFinal, m_FTextSentence) )
            {
                m_FTextRender.Append(m_FTextSentence);
            }
        }
        break;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    case kCantAcceptDailyQuestMaxCount:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0, };
            g_InterfaceManager.GetInterfaceString(6496, szMessage);
            if (m_FTextParser.Parser(szMessage, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        break;
    case kCompletedDailyQuestMaxCount:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0, };
            g_InterfaceManager.GetInterfaceString(6495, szMessage);
            if (m_FTextParser.Parser(szMessage, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        break;
    case kCantAcceptGuildDailyQuestMaxCount:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0, };
            g_InterfaceManager.GetInterfaceString(6902, szMessage);
            if (m_FTextParser.Parser(szMessage, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        break;
    case kCompletedGuildDailyQuestMaxCount:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0, };
            g_InterfaceManager.GetInterfaceString(6901, szMessage);
            if (m_FTextParser.Parser(szMessage, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
        break;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    default:
        {
            SolarDialog * pDlg = 
                GetManager()->OpenRewardWindow(pQuest->GetQuestInfo(), quest_gain_state);

            NPC * pNPC = (NPC *)g_ObjectManager.GetObject( GetManager()->GetPendingNPCKey() );
            if ( pNPC && pDlg && GetManager()->GetNPCCode() )
            {
                pNPC->SetOpenDialog(pDlg);
                pDlg->SetOwnerKey(GetManager()->GetPendingNPCKey());			
            }
        }
        break;
    }
}

#ifdef _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD
void uiQuestCommonForward::AutoclickFirstQuest()
{
	if ( GetManager()->GetCanAcceptList().size() == 0 )
	{
		ShowInterface(FALSE);
		return;
	}

	QCODE quest_code = (QCODE)GetCtrlQuestList()->GetItemData(1);
	if (quest_code == 0)
	{
		ShowInterface(FALSE);
		return;
	}
	Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
	if (NULL == pQuest)
	{
		pQuest = g_pQMGR_Temp->FindQuest(quest_code);
	}

	if ( !pQuest )
	{
		return;
	}	

	if (g_pHero == NULL)
	{
		return;
	}

	eQUEST_GAIN_STATE quest_gain_state = 
		GetManager()->GetStateInQuestListForForward(pQuest->GetQuestCode());
	int quest_accept_value = 0;
	if (pQuest->GetQuestInfo()->IsDailyQuest() == TRUE)
	{
		if (g_pQMGR_Real != NULL)
		{
			DWORD today_daily_quest_count;
			DWORD today_complete_count = 0;

			if (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_NORMAL)
			{
				today_daily_quest_count = 
					g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_NORMAL, &today_complete_count);
				// 하루 완료 제한 체크
				if (quest_gain_state == QUEST_REWARD)
				{
					if (today_complete_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
					{
						quest_accept_value = kCompletedDailyQuestMaxCount;
					}
				}
				else if (today_daily_quest_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
				{
					quest_accept_value = kCantAcceptDailyQuestMaxCount;
				}
			}
			else if (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD)
			{
				today_daily_quest_count = 
					g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_GUILD, &today_complete_count);
				// 하루 완료 제한 체크
				if (quest_gain_state == QUEST_REWARD)
				{
					if (today_complete_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
					{
						quest_accept_value = kCompletedGuildDailyQuestMaxCount;
					}
				}
				else if (today_daily_quest_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
				{
					quest_accept_value = kCantAcceptGuildDailyQuestMaxCount;
				}
			}
		}
	}

	WORD wAcceptLv = 0;
	QUEST_COND_HASH & AcceptConHash = pQuest->GetAcceptContionHash();
	QUEST_COND_HASH_IT itr = AcceptConHash.begin(); 
	while(itr != AcceptConHash.end())
	{
		QuestCondition * pAcceptCondition = (*itr);
		if(eQUEST_CONDITION_CHARLV == pAcceptCondition->GetType())
		{
			QUEST_CONDITION_CHARLV* pQCC = (QUEST_CONDITION_CHARLV*)pAcceptCondition;
			if(pQCC && (g_pHero->GetLevel() < pQCC->GetInfo()->GetMinCharLV()))
			{
				if( MAX_SHOW_QUEST_LEVEL_GAP > (pQCC->GetInfo()->GetMinCharLV() - g_pHero->GetLevel()) )
				{
					wAcceptLv = pQCC->GetInfo()->GetMinCharLV();
					quest_accept_value = kCantAcceptLevel;
				}
			}
			break;
		}

		++itr;
	}

	GetCtrlQuestList()->DeleteItemAll();
	m_FTextRender.ClearSentence();

	if (quest_accept_value > kAcceptQuest)
	{
		ShowInterface(FALSE);
		return;
	}

	SolarDialog * pDlg = 
		GetManager()->OpenRewardWindow(pQuest->GetQuestInfo(), quest_gain_state);

	NPC * pNPC = (NPC *)g_ObjectManager.GetObject( GetManager()->GetPendingNPCKey() );
	if ( pNPC && pDlg && GetManager()->GetNPCCode() )
	{
		pNPC->SetOpenDialog(pDlg);
		pDlg->SetOwnerKey(GetManager()->GetPendingNPCKey());
	}
}

bool uiQuestCommonForward::AutoFirstRewardQuest()
{
	if (GetManager()->GetCompleteList().size() == 0)
	{
		return false;
	}

	QUEST_LIST::const_iterator itr;

	// 보상 받을 수 있는 목록
	itr = GetManager()->GetCompleteList().begin();

	QCODE quest_code = *itr;

	Quest* pQuest = g_pQMGR_Real->FindQuest(quest_code);
	if (NULL == pQuest)
	{
		pQuest = g_pQMGR_Temp->FindQuest(quest_code);
	}

	assert(pQuest);

	eQUEST_GAIN_STATE quest_gain_state = 
		GetManager()->GetStateInQuestListForForward(pQuest->GetQuestCode());

	SolarDialog * pDlg = 
		GetManager()->OpenRewardWindow(pQuest->GetQuestInfo(), quest_gain_state);

	NPC * pNPC = (NPC *)g_ObjectManager.GetObject( GetManager()->GetPendingNPCKey() );
	if ( pNPC && pDlg && GetManager()->GetNPCCode() )
	{
		pNPC->SetOpenDialog(pDlg);
		pDlg->SetOwnerKey(GetManager()->GetPendingNPCKey());
	}

	return true;
}
#endif // _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
