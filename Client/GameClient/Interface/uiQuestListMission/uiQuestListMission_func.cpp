//------------------------------------------------------------------------------
//  uiQuestListMission_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestListMission.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "QuestManager_Concrete.h"
#include "ItemInfoParser.h"
#include "Quest.h"
#include <SolarHashTable.h>
#include "NPCInfoParser.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "QuestTextInfoParser.h"
#include "MissionManager_Concrete.h"
#include "MapInfoParser.h"
#include "Hero.h"
#include "uiSystemMan/uiSystemMan.h"
#include "questinfomanagerex.h"
//------------------------------------------------------------------------------
/**
*/

const int cMaxQuestList     = 6;

    const int cMaxMissionList   = 15;




#define	MAX_QUEST_NPC_NUM	2

uiQuestMan*
uiQuestListMission::GetManager()
{
    uiQuestMan* puiQuestMan  =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::ClearInfo()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if ( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

	if ( !this->CheckControls() )
		return;

	GetCtrlQuestMissionList()->DeleteItemAll();

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
#else
	// �ʱ�ȭ 
	QUEST_GROUP_ITR itr = m_QuestGroup.begin();
	while (itr != m_QuestGroup.end()) 
	{
		itr->second.clear();
		++itr;
	}

	m_QuestGroup.clear();
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

	m_FTextRender.ClearSentence();
}

//-------------------------------------------------------------------------------------------
/**
*/
ItemUnitRenderer *	
uiQuestListMission::GetItemUnitRender() const
{
	return this->m_pItemUnitRenderer;
}

//-------------------------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		uiQuestListMission::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// ���Կ� ����
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// �κ��丮
	CControlWZ * pCtrl = getControl( AtPos );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		
		ItemUnitRenderer::RenderUnit *  pRenderUnit = m_pItemUnitRenderer->AddItemUnit( dwSerial, rSlot.GetCode(), &rect, &GetSlot(AtPos), pCtrl );	
		if(pRenderUnit)
		{
			pRenderUnit->SetState(SLOT_RENDER_STATE_REWARDLIST);
		}
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	return eInsertSlotResult;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL				
uiQuestListMission::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	SCSlot & rSlot = GetSlot(AtPos);
	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	return SCSlotContainer::DeleteSlot(AtPos, pSlotOut);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::UpdateQuestInfo()
{
	ClearInfo();

	refreshQuestMission();
	refreshQuestInfoReward();
	refreshQuestCancelBtn();
	CCtrlStaticWZ* pTitle =  (CCtrlStaticWZ*)getControl(eDLGPOS_TITLE);
	if(pTitle && m_pCurQuest)
	{
		TCHAR szTitle[INTERFACE_STRING_LENGTH + 1] = {0,};
		g_InterfaceManager.GetInterfaceString(m_pCurQuest->GetQuestInfo()->GetQNCode(), szTitle);
#ifdef _INTERNATIONAL_UI
		GlobalFunc::SetCaptionSimple(pTitle,szTitle);
#else
		pTitle->SetTextWZ(szTitle);
#endif//_INTERNATIONAL_UI
		
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::initInfo()
{
	m_pCtrlCheckQuestType[0]->SetCheckState(TRUE);  // ����Ʈ �̼�
	m_pCtrlCheckQuestType[1]->SetCheckState(FALSE);	// ����Ʈ ����

	GetCtrlQuestMissionList()->SetUnuseSelBar(TRUE);

	GetCtrlQuestMissionList()->SetColorSelBar(BAR_COLOR_SELECT_LIST_UNUSE);

	SetCaptionRewardHeim(_T("0"));
	SetCaptionRewardExp(_T("0"));
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::updateInfo()
{
	m_FTextRender.SetRect( GetCtrlQuestMissionList()->GetSizeRect() );
	updateMouseWheel();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::updateMouseWheel()
{
	RECT rc;
	int iMaxLine = 0;

	size_t iMaxMissionList = m_FTextRender.GetDrawLineInRect();

	if(m_FTextRender.GetMaxLine() > iMaxMissionList)
	{
		iMaxLine = m_FTextRender.GetMaxLine()- iMaxMissionList;
	}

	GetCtrlQuestMissionVScroll()->SetScrollRangeWZ(0, iMaxLine );	

	rc = GetCtrlQuestMissionList()->GetSizeRect();
	switch (GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			//.���� ȭ��ǥ�� ������.
			int pos = GetCtrlQuestMissionVScroll()->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestMissionVScroll()->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
			{
				pos--;      // ���� ���� ��Ű�� 
				GetCtrlQuestMissionVScroll()->SetScrollPosWZ(pos);
				m_FTextRender.SetBeginLineIndex(pos);

			}
		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			int pos = GetCtrlQuestMissionVScroll()->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestMissionVScroll()->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
			{
				pos++;      // ���� ���� ��Ű�� 
				GetCtrlQuestMissionVScroll()->SetScrollPosWZ(pos);
				m_FTextRender.SetBeginLineIndex(pos);
			}
		}
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::ListMessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{ 
	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_LINETOP:
		{
			//.���� ȭ��ǥ�� ������.
			//
			if ( GetWzIDQuestMissionVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestMissionVScroll()->GetScrollPosWZ();

				--index;
				index = max(0, index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestMissionVScroll()->SetScrollPosWZ( index );
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			if ( GetWzIDQuestMissionVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestMissionVScroll()->GetScrollPosWZ();

				++index;
				index = min((int)m_FTextRender.GetMaxLine(), index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestMissionVScroll()->SetScrollPosWZ( index );
			}
		}
		break;

	
	case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			//.���� �巡������ ���콺�� ���Ҵ�.
			if ( GetWzIDQuestMissionVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestMissionVScroll()->GetScrollPosWZ();
				m_FTextRender.SetBeginLineIndex( index );
			}
		}
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**	 �ӹ��� ���� ���÷���
	 �긮�� + �ӹ� 
	 TODO : ���� �ؽ�Ʈ�� ����.
*/
void					
uiQuestListMission::refreshQuestMission()
{
	if ( !g_pHero || !GetManager() || !CheckControls() )
		return;

	GetCtrlQuestMissionList()->SetUseReport(FALSE);

	if ( NULL == m_pCurQuest )
	{
		return;
	}

	Quest* pQuest = m_pCurQuest;

	if(0 == m_btDlgState)
	{
		DWORD dwBridID = pQuest->GetQuestInfo()->GetSCode_Brief();

		QuestTextInfo * pInfo =
			QuestTextInfoParser::Instance()->GetQuestTextInfo(dwBridID);

		m_FTextRender.ClearSentence();

		if (pInfo) 
		{
			SUN_STRING strTemp = pInfo->m_pszText;
			if ( m_FTextParser.Parser(strTemp, m_FTextSentence) )
			{
				m_FTextRender.Append(m_FTextSentence);
				m_FTextSentence.Release();

				m_FTextParam.eType = FORMAT_TEXT_PARAM_RETURN;
				m_FTextParam.dwValue = 1; // 1ĭ �����.
	#ifdef _INTERNATIONAL_UI
				m_FTextParam.wzFont = StrToWzID("st10");
	#else
				m_FTextParam.wzFont = StrToWzID("mn12");
	#endif//_INTERNATIONAL_UI

				m_FTextSentence.Pushback(m_FTextParam);

				m_FTextRender.Append(m_FTextSentence);
			}
			
		}
	}
	else if(1 == m_btDlgState)
	{
		m_FTextRender.ClearSentence();

		DWORD dwBrifID = pQuest->GetQuestInfo()->GetSCode_Start();

		QuestTextInfo * pInfo =
			QuestTextInfoParser::Instance()->GetQuestTextInfo(dwBrifID);
		if ( !pInfo )
		{
			return;
		}

		if ( m_FTextParser.Parser( pInfo->m_pszText, m_FTextSentence) )
		{
			m_FTextRender.Append(m_FTextSentence);
		}	
	}
	else
	{
		assert(!"uiQuestListMission::refreshQuestMission()::TextRender");
	}

	QUEST_COND_HASH & CompleteConHash = (QUEST_COND_HASH & )pQuest->GetCompleteConditionHash();
	QUEST_COND_HASH_IT itr = CompleteConHash.begin(); 

	BOOL	IsEmptyLine = FALSE;

	while ( itr != pQuest->GetCompleteConditionHash().end()	)
	{
		QuestCondition * pCompleteCondition = (*itr);
		switch ( pCompleteCondition->GetType() )
		{
		case eQUEST_CONDITION_ITEM:			
			{
				QUEST_CONDITION_ITEM * pConditionItem = (QUEST_CONDITION_ITEM *)pCompleteCondition;
				insertConditionItem(pConditionItem);
				IsEmptyLine = TRUE;
			}
			break;
		case eQUEST_CONDITION_MONEY:
			{
				QUEST_CONDITION_MONEY * pCondition = (QUEST_CONDITION_MONEY *)pCompleteCondition;                
				insertConditionMoney(pCondition);
				IsEmptyLine = TRUE;
			}
			break;
		case eQUEST_CONDITION_KILLMONSTER:
			{
				QUEST_CONDITION_KILLMONSTER * pCondition = (QUEST_CONDITION_KILLMONSTER *)pCompleteCondition;
				insertConditionKillMon(pCondition);
				IsEmptyLine = TRUE;
			}
			break;
		case eQUEST_CONDITION_MISSION:
			{
				QUEST_CONDITION_MISSION * pCondition = (QUEST_CONDITION_MISSION *)pCompleteCondition;
				insertConditionMission( pCondition );
				IsEmptyLine = TRUE;
			}
			break;
		case eQUEST_CONDITION_QUEST:
			{
				QUEST_CONDITION_QUEST * pCondition = (QUEST_CONDITION_QUEST *)pCompleteCondition;
				insertConditionQuest( pCondition );
				IsEmptyLine = TRUE;
			}
			break;

		case eQUEST_CONDITION_AREA:
			{
				//Ž�� ���ǵ� ����
				QUEST_CONDITION_AREA* pCondition = 
					(QUEST_CONDITION_AREA*)pCompleteCondition;
				insertConditionArea(pCondition, pQuest->GetQuestInfo());

				IsEmptyLine = TRUE;
			}
			break;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        case eQUEST_CONDITION_MISSION_CLEAR:
            {
                QUEST_CONDITION_MISSION_CLEAR* pCondition =
                    (QUEST_CONDITION_MISSION_CLEAR*)pCompleteCondition;
                insertConditionMissionClear(pCondition, pQuest->GetQuestInfo());

                IsEmptyLine = TRUE;
            }
            break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE

		case eQUEST_CONDITION_CLASSITEM:
		case eQUEST_CONDITION_CHARLV:
		case eQUEST_CONDITION_CHARCLASS:
		case eQUEST_CONDITION_REPEAT:
		default:
			break;
		}

		++itr;
	}

	QuestInfo * pQuestInfo = g_QuestInfoManager.FindQuestInfo( pQuest->GetQuestCode() );

	if( pQuestInfo != NULL )
	{
		BASE_NPCINFO * pStart  = NPCInfoParser::Instance()->GetNPCInfo( pQuestInfo->GetNPCCode_Start() );	
		BASE_NPCINFO * pReward = NPCInfoParser::Instance()->GetNPCInfo( pQuestInfo->GetNPCCode_Reward() );

		TCHAR szResult[INTERFACE_STRING_LENGTH + 1];
		

		if( IsEmptyLine )
		{
			m_FTextSentence.Release();

#ifdef _INTERNATIONAL_UI
			m_FTextParam.wzFont = StrToWzID("st10");
#else
			m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
			m_FTextParam.eType = FORMAT_TEXT_PARAM_RETURN;
			m_FTextParam.dwValue = 1;
			m_FTextSentence.Pushback( m_FTextParam );
			m_FTextRender.Append( m_FTextSentence );
		}

        DWORD dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;

        if( pStart != NULL )
        {
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
            TCHAR npc_code[INTERFACE_STRING_LENGTH + 1] = {'\0',};
            _itot(pStart->m_MonsterCode, npc_code, 10);

            TCHAR format_string[INTERFACE_STRING_LENGTH + 1] = {'\0',};
            g_InterfaceManager.GetInterfaceStringFormat(format_string, 
                INTERFACE_STRING_LENGTH,
                2922,
                npc_code);

            _sntprintf(szResult, INTERFACE_STRING_LENGTH, "%s %s",
                g_InterfaceManager.GetInterfaceString(1429), format_string);
            if (m_FTextParser.Parser(szResult, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
#else
            TCHAR szStart[MAX_PATH + 1];
            g_InterfaceManager.GetInterfaceString( pStart->m_NCode, szStart, MAX_PATH );
            _sntprintf( szResult, INTERFACE_STRING_LENGTH, "%s %s", g_InterfaceManager.GetInterfaceString( 1429 ), szStart );

#ifdef _INTERNATIONAL_UI
			AddFText( dwColor,  StrToWzID("st10"), szResult );
#else
			AddFText( dwColor,  StrToWzID("mn12"), szResult );
#endif//_INTERNATIONAL_UI

#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC


        }

        if( pReward != NULL )
        {
#ifdef _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC
            TCHAR npc_code[INTERFACE_STRING_LENGTH + 1] = {'\0',};
            _itot(pReward->m_MonsterCode, npc_code, 10);

            TCHAR format_string[INTERFACE_STRING_LENGTH + 1] = {'\0',};
            g_InterfaceManager.GetInterfaceStringFormat(format_string, 
                INTERFACE_STRING_LENGTH,
                2922,
                npc_code);

            _sntprintf(szResult, INTERFACE_STRING_LENGTH, "%s %s",
                g_InterfaceManager.GetInterfaceString(1430), format_string);
            if (m_FTextParser.Parser(szResult, m_FTextSentence))
            {
                m_FTextRender.Append(m_FTextSentence);
            }
#else
            TCHAR szReward[MAX_PATH + 1];
            g_InterfaceManager.GetInterfaceString( pReward->m_NCode, szReward, MAX_PATH );
            _sntprintf( szResult, INTERFACE_STRING_LENGTH, "%s %s", g_InterfaceManager.GetInterfaceString( 1430 ), szReward );

#ifdef _INTERNATIONAL_UI
			AddFText( dwColor,  StrToWzID("st10"), szResult );
#else
			AddFText( dwColor,  StrToWzID("mn12"), szResult );
#endif//_INTERNATIONAL_UI

#endif //_SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC


		}
	}

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    // no operation
#else
    refreshUnionQuest(pQuest);
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING
}

void uiQuestListMission::AddFText( DWORD dwColor, WzID wzFont, LPCTSTR szText )
{
#ifdef _INTERNATIONAL_UI
	if( m_FTextParser.Parser(szText, m_FTextSentence) )
	{
		m_FTextRender.Append(m_FTextSentence);
		m_FTextSentence.Release();
		m_FTextParam.eType = FORMAT_TEXT_PARAM_RETURN;
		m_FTextParam.dwValue = 1; // 1ĭ �����.
		m_FTextParam.wzFont = wzFont;
        m_FTextParam.align_text_style = TP_HLEFT;
		m_FTextSentence.Pushback(m_FTextParam);
		m_FTextRender.Append(m_FTextSentence);
	}
#else
	m_FTextSentence.Release();
	m_FTextParam.Clear();
	m_FTextParam.dwColor = dwColor;
	m_FTextParam.wzFont = wzFont;
    m_FTextParam.align_text_style = TP_HLEFT;
	m_FTextParam.m_StrContents = szText;		
	m_FTextSentence.Pushback( m_FTextParam );
	m_FTextRender.Append( m_FTextSentence );
#endif//_INTERNATIONAL_UI
}

//-------------------------------------------------------------------------------------------
/** ����Ʈ ���� ��ư SHOW / HIDE
*/
void					
uiQuestListMission::refreshQuestCancelBtn()
{
	if ( !g_pHero || !GetManager() || !CheckControls() )
		return;

	if( NULL == g_pQMGR_Real->FindQuest(m_pCurQuest->GetQuestCode()) )
	{
		ShowQuestCancelBtn(FALSE);
	}
	else
	{
		ShowQuestCancelBtn(TRUE);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::insertConditionItem(QUEST_CONDITION_ITEM * pConditionItem)
{		
	for ( int i = 0; i < pConditionItem->GetInfo()->GetItemArrayCount(); ++i)
	{
		m_FTextSentence.Release();

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

			WORD RealItemNum = ItemManager::Instance()->GetItemCount( SI_INVENTORY, NeedItemCode );

			// [� ������]    min(NeedItemNum, RealItemNum) / RealItemNum; 

			GetUIMan()->GetInterfaceString(pItemInfo->m_NCode, m_szText, INTERFACE_STRING_LENGTH);

			if( m_pCurQuest && g_pQMGR_Real->FindQuest(m_pCurQuest->GetQuestCode()) )
			{
				Sprintf(m_szResult, 
					_T("%s    (%d / %d)"),
					m_szText,
					min(NeedItemNum, RealItemNum), 
					NeedItemNum );
			}
			else
			{
				Sprintf(m_szResult, 
					_T("%s    (%d / %d)"),
					m_szText,
					NeedItemNum, 
					NeedItemNum );
			}
			DWORD dwTextColor = 0;						
			m_FTextParam.Clear();

			if ( RC::RC_QUEST_SUCCESS == pConditionItem->CanComplete(g_pHero) )
			{
				m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
			}
			else
			{
				m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
			}  
#ifdef _INTERNATIONAL_UI
			m_FTextParam.wzFont = StrToWzID("st10");
#else
			m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
			m_FTextParam.m_StrContents = m_szResult;		

			m_FTextSentence.Pushback(m_FTextParam);
		}
		else
		{
			// �������� ������Ѵ�.
			CODETYPE NeedItemCode = pConditionItem->GetInfo()->GetItemCode(i);
			WORD NeedItemNum = pConditionItem->GetInfo()->GetItemNum(i);

			BOOL bIsExist = ItemManager::Instance()->IsExistItem( SI_INVENTORY, NeedItemCode, NeedItemNum);

			bIsExist;
			// [� ������] [%d] ���� ������ ���� ����. 
		}    

		m_FTextRender.Append( m_FTextSentence );
	}
}
//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::insertConditionMoney(QUEST_CONDITION_MONEY * pConditionMoney)
{
	m_FTextSentence.Release();

	if (!pConditionMoney)
		return;

	MONEY NeedMoney = pConditionMoney->GetInfo()->GetNeedMoney();

	m_FTextParam.Clear();

	// 9049 �ʿ� ���� : %I64u
    TCHAR szMessage[INTERFACE_STRING_LENGTH];
    GetUIMan()->GetInterfaceString( 9049, szMessage, INTERFACE_STRING_LENGTH);

	Sprintf(m_szResult, szMessage, NeedMoney );

	if ( g_pHero->GetMoney() >= NeedMoney )
	{
		m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
	}
	else
	{
		m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
	}  
#ifdef _INTERNATIONAL_UI
	m_FTextParam.wzFont = StrToWzID("st10");
#else
	m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
	m_FTextParam.m_StrContents = m_szResult;		

	m_FTextSentence.Pushback(m_FTextParam);

	m_FTextRender.Append( m_FTextSentence );
}


//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestListMission::insertConditionKillMon(QUEST_CONDITION_KILLMONSTER * pConditionKillMon)
{
	TCHAR szName[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szGrade[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szLevel[INTERFACE_STRING_LENGTH + 1] = {0,};
	GetUIMan()->GetInterfaceString(1668, szLevel); //1668 ����

	for ( int i = 0; i < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++i)
	{
		m_FTextSentence.Release();
		m_FTextParam.Clear();

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
				GetUIMan()->GetInterfaceString(pInfo->m_NCode, szName, INTERFACE_STRING_LENGTH);
#ifdef _DEV_VER
                //#ifdef _SCJ_SHOW_NPC_ID
                if (CGeneralGameParam::Instance()->IsShowNpcId())
                {
                    TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                    StrnCopy(buffer_string, szName, INTERFACE_STRING_LENGTH-1); 
                    Snprintf(szName, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
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

		WORD	 wRequireMonsterNum = pConditionKillMon->GetInfo()->GetMonsterNum(i);
		WORD	 wCurMonsterNum = pConditionKillMon->GetCurMonsterNum(i);

		if ( RC::RC_QUEST_SUCCESS == pConditionKillMon->CanComplete(g_pHero) )
		{
			m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
		}
		else
		{
			m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
		}  

		switch(MonsterGrade)
		{
		case eNPC_GENERAL:			//11501	�Ϲ�
		case eNPC_MAPOBJECT_NPC:	// 2009.08.14 - �ű�����Ʈ���� ���ͻ�� �������� �ʿ�����ƮNPC�� �߰��Ǿ� �Ϲݵ������ ó��
			// (Ÿ��â���� �̹� �ʿ�����ƮNPC �� ��� �Ϲ����� ����� �ϰ� ����)
			g_InterfaceManager.GetInterfaceString(11501, szGrade);
			break;
		case eNPC_ELITE: //11503	����
			g_InterfaceManager.GetInterfaceString(11503, szGrade);
			break;
		case eNPC_LEADER: //11502	����
			g_InterfaceManager.GetInterfaceString(11502, szGrade);
			break;
		case eNPC_MIDDLEBOSS: //11504	ĸƾ
			g_InterfaceManager.GetInterfaceString(11504, szGrade);
			break;
		case eNPC_BOSS: //11505	����
			g_InterfaceManager.GetInterfaceString(11505, szGrade);
			break;
		case eNPC_LUCKY_MONSTER: //11500	��Ű
			g_InterfaceManager.GetInterfaceString(11500, szGrade);
			break;

		default:
			break;
		}

		if( m_pCurQuest && g_pQMGR_Real->FindQuest(m_pCurQuest->GetQuestCode()) )
		{
			Sprintf(m_szResult, 
				_T("[%s%d/%s] %s    ( %d / %d )"), 
				szLevel,
				MonsterLevel,
				szGrade,
				szName,
				min(wCurMonsterNum,wRequireMonsterNum), 
				wRequireMonsterNum );
		}
		else
		{
			Sprintf(m_szResult, 
				_T("[%s%d/%s] %s    ( %d / %d )"), 
				szLevel,
				MonsterLevel,
				szGrade,
				szName,
				wRequireMonsterNum, 
				wRequireMonsterNum );
		}
		m_FTextParam.m_StrContents = m_szResult;

#ifdef _INTERNATIONAL_UI
		m_FTextParam.wzFont = StrToWzID("st10");

		if( m_FTextParser.Parser(m_szResult, m_FTextSentence) )
		{
			m_FTextRender.Append(m_FTextSentence);
			m_FTextSentence.Release();
			m_FTextParam.eType = FORMAT_TEXT_PARAM_RETURN;
			m_FTextParam.dwValue = 1; // 1ĭ �����.
			m_FTextParam.wzFont = m_FTextParam.wzFont;
            m_FTextParam.align_text_style = m_FTextParam.align_text_style;
			m_FTextSentence.Pushback(m_FTextParam);
			m_FTextRender.Append(m_FTextSentence);
		}

#else
		m_FTextParam.wzFont = StrToWzID("mn12");
		
		m_FTextSentence.Pushback(m_FTextParam);

		m_FTextRender.Append( m_FTextSentence );
#endif//_INTERNATIONAL_UI
	}    
}

//-------------------------------------------------------------------------------------------
/**
*/
void uiQuestListMission::insertConditionQuest(QUEST_CONDITION_QUEST * pConditionQuest)
{
	for ( int i = 0; i < pConditionQuest->GetInfo()->GetQuestArrayCount(); ++i)
	{
		m_FTextSentence.Release();

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
			m_FTextParam.Clear();

			TCHAR szSuc[INTERFACE_STRING_LENGTH] = {0,};

			QUESTPART_COMPL* pQC = g_pQMGR_Real->FindComplQuest((QCODE)QuestCode);
			if(NULL != pQC && 0 != pQC->m_Num)
			{
				m_FTextParam.dwColor = TEXT_COLOR_CONDITION_QUEST_COMPLETE;
				//9051	(�Ϸ�)
				g_InterfaceManager.GetInterfaceString(9051, szSuc);
			}
			else
			{
				m_FTextParam.dwColor = TEXT_COLOR_CONDITION_QUEST_GENERAL;
			}  

			GetUIMan()->GetInterfaceString(pQ->GetQuestInfo()->GetQNCode(), m_szText, INTERFACE_STRING_LENGTH);

			Sprintf(m_szResult, 
				_T("%s%s"), 
				m_szText,
				szSuc);
#ifdef _INTERNATIONAL_UI
			m_FTextParam.wzFont = StrToWzID("st10");
#else
			m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
			m_FTextParam.m_StrContents = m_szResult;		

			m_FTextSentence.Pushback(m_FTextParam);

			m_FTextRender.Append( m_FTextSentence );	
		}
	}    
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::insertConditionMission(QUEST_CONDITION_MISSION * pConditionMission)
{
	for ( int i = 0; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++i)
	{
		m_FTextSentence.Release();

		CODETYPE MissionCode = pConditionMission->GetInfo()->GetMissionCode(i);
		if (!MissionCode)
			continue;

		WORD	 wRequireMissionPoint = pConditionMission->GetInfo()->GetMissionNum(i);
		WORD	 wCurMissionPoint = g_pMission_Concrete->GetMissionPoint( (BYTE)MissionCode );

		m_FTextParam.Clear();

		if ( RC::RC_QUEST_SUCCESS == pConditionMission->CanComplete(g_pHero) )
		{
			m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
		}
		else
		{
			m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
		}  

		// [� �̼�] (%d/%d)

		const sMAPINFO * pInfo = 
			MapInfoParser::Instance()->FindMapInfo( (MAPCODE)MissionCode );
		if ( pInfo )
		{
			GetUIMan()->GetInterfaceString(pInfo->dwNCode, m_szText, INTERFACE_STRING_LENGTH);

			if( m_pCurQuest && g_pQMGR_Real->FindQuest(m_pCurQuest->GetQuestCode()) )
			{
				Sprintf(m_szResult, 
					_T("%s    ( %d / %d )"), 
					m_szText, 
					min(wCurMissionPoint,wRequireMissionPoint), 
					wRequireMissionPoint );

			}
			else 
			{
				Sprintf(m_szResult, 
					_T("%s    ( %d / %d )"), 
					m_szText, 
					wRequireMissionPoint, 
					wRequireMissionPoint );
			}

#ifdef _INTERNATIONAL_UI
			m_FTextParam.wzFont = StrToWzID("st10");
#else
			m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
			m_FTextParam.m_StrContents = m_szResult;		

			m_FTextSentence.Pushback(m_FTextParam);

			m_FTextRender.Append( m_FTextSentence );		
		}
	}    		
}


void uiQuestListMission::insertConditionArea(QUEST_CONDITION_AREA* pConditionArea, QuestInfo * pQuestInfo)
{
	for ( int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
	{
		DWORD AreaID = pConditionArea->GetInfo()->GetAreaID(Index);
		if (!AreaID)
			break;

		m_FTextSentence.Release();
		m_FTextParam.Clear();

		TCHAR szSucs[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR szAreaText[INTERFACE_STRING_LENGTH] = {0, };

		if ( TRUE == pConditionArea->GetAreaCondition(Index) )
		{
			m_FTextParam.dwColor = TEXT_COLOR_CONDITION_QUEST_GENERAL;
			g_InterfaceManager.GetInterfaceString(9051, szSucs);
		}
		else
		{
			m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;
		}

		g_InterfaceManager.GetInterfaceString(pQuestInfo->GetTISMissionText(Index), szAreaText);

		Sprintf(m_szResult, _T("  %s %s"), szAreaText,	szSucs);

#ifdef _INTERNATIONAL_UI
		m_FTextParam.wzFont = StrToWzID("st10");
#else
		m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
		m_FTextParam.m_StrContents = m_szResult;		

		m_FTextSentence.Pushback(m_FTextParam);

		m_FTextRender.Append( m_FTextSentence );
	}
}

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
void uiQuestListMission::insertConditionMissionClear(QUEST_CONDITION_MISSION_CLEAR* pConditionMissionClear, QuestInfo * pQuestInfo)
{  
    for(int i = 0; i < QUEST_CONDITION_MISSION_CLEAR::MAX_MISSION_CLEAR_CHECK_NUM; ++i)
    {
        MAPCODE map_id = pConditionMissionClear->GetInfo()->GetMissionCode(i);
        DWORD max_clear_number = pConditionMissionClear->GetInfo()->GetMissionNum(i);

        if (!map_id || !max_clear_number)
        {
            continue;
        }

        m_FTextSentence.Release();
        m_FTextParam.Clear();        

        QCODE QuestCode = pQuestInfo->GetQuestCode();

        Quest* pQ1 = g_pQMGR_Real->FindQuest((QCODE)QuestCode);
        Quest* pQ2 = g_pQMGR_Temp->FindQuest((QCODE)QuestCode);
        Quest* pQ = NULL;
        if(pQ1)
            pQ = pQ1;
        else if(pQ2)
            pQ = pQ2;
        DWORD current_clear_number;
        QUESTPART_COMPL* pQC = g_pQMGR_Real->FindComplQuest((QCODE)QuestCode);
        if(NULL != pQC && 0 != pQC->m_Num)
        {
            m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;

            current_clear_number = max_clear_number;
        }
        else
        {
            m_FTextParam.dwColor = TEXT_COLOR_QUEST_LIST_MISSION_GENERAL;

            current_clear_number = pConditionMissionClear->GetCurMissionClearNum(i);
        }
        
        sMAPINFO * area_name = MapInfoParser::Instance()->FindMapInfo(map_id);
        TCHAR map_name[INTERFACE_STRING_LENGTH] = {0, };
        if (area_name)
        {
            g_InterfaceManager.GetInterfaceString(area_name->dwNCode, map_name);
        }        
        
        sprintf(m_szResult, "%s (%d / %d)", map_name, current_clear_number, max_clear_number);
        
        m_FTextParam.m_StrContents = m_szResult;

        m_FTextParam.wzFont = StrToWzID("st10");

        m_FTextSentence.Pushback(m_FTextParam);

        m_FTextRender.Append( m_FTextSentence );
    }
}
#endif // _NA_006607_20130402_ADD_QUESTTYPE


//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
#else
void					
uiQuestListMission::refreshUnionQuest( Quest * pQuest )
{
	// ���� ����Ʈ �ϰ�� �ΰ� ������ ǥ��.
	if ( pQuest->GetQuestInfo()->IsUnionQuest() )
	{
		QUEST_INFO_HASH_ITR itrUnion = pQuest->GetQuestInfo()->GetChildQuestInfoHash().begin();
		for (; itrUnion != pQuest->GetQuestInfo()->GetChildQuestInfoHash().end(); ++itrUnion )
		{
			QuestInfo * pChildQuestInfo = *itrUnion;
			m_FTextSentence.Release();
			m_FTextParam.Clear();

			GetUIMan()->GetInterfaceString(
				pChildQuestInfo->GetQNCode(), 
				m_szResult, 
				INTERFACE_STRING_LENGTH );

			Quest * pChildQuest = g_pQMGR_Real->FindQuest( pChildQuestInfo->GetQuestCode() );
			switch( pChildQuest->GetQState() )
			{
			case QS_COMPLETED:
				{                    
                    // (�Ϸ�) ( 9051 )
                    GetUIMan()->GetInterfaceString( 9051, m_szText, INTERFACE_STRING_LENGTH );                     
					_tcscat(m_szResult, m_szText );
				}
				break;

            case QS_FAILED:
                {
                    // (����) ( 9052 )
                    GetUIMan()->GetInterfaceString( 9052, m_szText, INTERFACE_STRING_LENGTH );                     
                    _tcscat(m_szResult, m_szText );
                }
                break;

			default:
				{
				}
				break;
			}

#ifdef _INTERNATIONAL_UI
			m_FTextParam.wzFont = StrToWzID("st10");
#else
			m_FTextParam.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
			m_FTextParam.m_StrContents = m_szResult;		

			m_FTextSentence.Pushback( m_FTextParam );
			m_FTextRender.Append( m_FTextSentence );
		}		
	}
}
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::clickChkQuestType(POSTYPE pos)
{
	switch (pos)
	{
	case eDLGPOS_CHK_QUEST_MISSION:
		{
			if(1 == m_btDlgState)
			{
				m_btDlgState = 0;
				refreshQuestMission();
			}
			// ���簡 �̼�â�̱� ������ ����.
			//GetManager()->OpenQuestListMission();
		}
		break;
	case eDLGPOS_CHK_QUEST_CONTENT:
		{		
			if(0 == m_btDlgState)
			{
				m_btDlgState = 1;
				refreshQuestMission();
			}
		}
		break;

	default:
		return;
	}	

	pos -= QUEST_CHECK_LIST_TYPE_BEGIN;
	for ( int i = 0; i < QUEST_CHECK_LIST_TYPE_SIZE; ++i )
	{
		this->m_pCtrlCheckQuestType[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckQuestType[pos]->SetCheckState(TRUE);	
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::clickQuestAbandon()
{
	TCHAR szText[INTERFACE_STRING_LENGTH];
	if(NULL == m_pCurQuest)
	{
		return;
	}
	Quest * pQuest = m_pCurQuest;
	GetManager()->SetPendingAbandonQuestCode( pQuest->GetQuestCode() );

	GetUIMan()->GetInterfaceString( 
		pQuest->GetQuestInfo()->GetQNCode(), 
		szText, 
		INTERFACE_STRING_LENGTH );

	if ( pQuest->GetQuestInfo()->IsChildQuest() )
	{
		return;
	}

	UpdateQuestInfo();

    //5904	[%s] ����Ʈ�� �����Ͻðڽ��ϱ�?
    TCHAR szBufMessage[INTERFACE_STRING_LENGTH];
    GetUIMan()->GetInterfaceString( 5904, szBufMessage, INTERFACE_STRING_LENGTH );
    Sprintf(m_szResult, szBufMessage, szText);

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Confirm(m_szResult, GlobalFunc::QuestAbandon);

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListMission::refreshQuestInfoReward()
{
	Quest * pQuest = m_pCurQuest;
	if ( !pQuest )
	{
		return;
	}

	// ����Ʈ ���� ����
	assert(g_pHero);
	if( !g_pHero)
    {
		return;
    }

	DWORD dwRewardCode = pQuest->GetQuestInfo()->GetRewardCode(g_pHero->GetClass());
	RewardInfo * pRewardInfo = RewardInfoList::Instance()->FindRewardInfo( dwRewardCode );
	assert(pRewardInfo);
	if (!pRewardInfo) 
	{
		return;
	}	

	// ���� or ī�� Ÿ��
	if ( pRewardInfo->GetChaoTime() )
	{
		// ī���ð� : 9043
		GetUIMan()->GetInterfaceString(9043, m_szText, INTERFACE_STRING_LENGTH);
		SetCaptionRewardChao( m_szText );
		Sprintf(m_szText, _T("%ld"), pRewardInfo->GetChaoTime());
	}
	else
	{
		// ���� : 999
		GetUIMan()->GetInterfaceString(999, m_szText, INTERFACE_STRING_LENGTH);
		SetCaptionRewardChao( m_szText );

		TCHAR szMoney[INTERFACE_STRING_LENGTH + 1] = {0,};
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		SetCaptionRewardChao( m_szText );
		Sprintf(szMoney, _T("%I64u"), pRewardInfo->GetHime());
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, m_szText, sizeof(m_szText));
	}

	SetCaptionRewardHeim(m_szText);

	// ����ġ
	{
		TCHAR exp_string[INTERFACE_STRING_LENGTH + 1] = {0, };
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		Sprintf(exp_string, _T("%I64d"), pRewardInfo->GetExp() );
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, exp_string, &nFmt, m_szText, sizeof(m_szText));
	}
	SetCaptionRewardExp(m_szText);

    Snprintf(m_szText, INTERFACE_STRING_LENGTH, _T("%d"), pRewardInfo->GetCoin());
    reward_point_button_->SetTextWZ(m_szText);

	ItemType * pItemType = NULL;    
	SCItemSlot TempSlot;
	// �Ƚ� ������
	BASE_ITEMINFO * pItemInfo = 
		ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetItemCode() );
	if (pItemInfo)
	{
		TempSlot.SetCode( pRewardInfo->GetItemCode() );

		if ( pItemInfo->m_byDupNum > 1 )
		{			
			TempSlot.SetNum( (DURATYPE)pRewardInfo->GetItemTypeIndex() );          
		}
		else
		{
			pItemType = 
				ItemTypeList::Instance()->FindItemType( pRewardInfo->GetItemTypeIndex() );
			if (pItemType)
			{
				TempSlot.SetEnchant( pItemType->GetEnchant() );
				TempSlot.SetRank((eRANK)pItemType->GetRank() );
                nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());
			}			
		}	

		InsertSlot( SLOT_FIX_ITEM, TempSlot);
	}	

	switch ( pRewardInfo->GetRefType() )
	{
	case RewardInfo::REF_TYPE_SELECT:
		{
			// ���� ������
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( 0 == pRewardInfo->GetTypeCode(i) )
				{
					continue;
				}

				pItemInfo = 
					ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetTypeCode(i) );
				if (pItemInfo)
				{
					TempSlot.SetCode( (SLOTCODE)pRewardInfo->GetTypeCode(i) );

					if (pItemInfo->m_byDupNum > 1) 
					{					    
						TempSlot.SetNum( (DURATYPE)pRewardInfo->GetTypeIndex(i) );
					}
					else
					{
						pItemType = 
							ItemTypeList::Instance()->FindItemType( pRewardInfo->GetTypeIndex(i) );
						if ( pItemType )
						{
							TempSlot.SetEnchant( pItemType->GetEnchant() );
							TempSlot.SetRank( (eRANK)pItemType->GetRank() );	
                            nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());
						}
					}

					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}
		}
		break;
	case RewardInfo::REF_TYPE_RANDOM:
		{
			// ���� ������
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( pRewardInfo->GetTypeCode(i) )
				{
					TempSlot.SetCode( RANDOM_ITEM_CODE );	// ����ǥ ������
					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}	
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_ITEMCODE:
		{
			// ���� ������
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( 0 == pRewardInfo->GetTypeCode(i) )
				{
					continue;
				}

				pItemInfo = 
					ItemInfoParser::Instance()->GetItemInfo( pRewardInfo->GetTypeCode(i) );
				if (pItemInfo)
				{
					TempSlot.SetCode( (SLOTCODE)pRewardInfo->GetTypeCode(i) );

					if (pItemInfo->m_byDupNum > 1) 
					{					    
						TempSlot.SetNum( (DURATYPE)pRewardInfo->GetTypeIndex(i) );
					}
					else
					{
						pItemType = 
							ItemTypeList::Instance()->FindItemType( pRewardInfo->GetTypeIndex(i) );
						if ( pItemType )
						{
							TempSlot.SetEnchant( pItemType->GetEnchant() );
							TempSlot.SetRank( (eRANK)pItemType->GetRank() );					
                            nsSlot::ItemTypeChanger::ChangeItemByType(&TempSlot, pItemType->GetItemType());
						}
					}

					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_DROPINFO:
		{
			// ���� ������
			for ( int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
			{
				if ( pRewardInfo->GetTypeCode(i) )
				{
					TempSlot.SetCode( RANDOM_ITEM_CODE );	// ����ǥ ������
					InsertSlot( SLOT_SELECT_BEGIN+i, TempSlot );
				}
			}	
		}
		break;
	default:
		assert(0);
		break;
	}
}

//#endif // end of __NA_20061103_REWARD_STRUCT_CHANGE



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
