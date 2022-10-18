//------------------------------------------------------------------------------
//  uiQuestListContent_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestListContent.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "QuestTextInfoParser.h"
#include "uiSystemMan/uiSystemMan.h"

const int cMaxQuestList			= 6;
const int cMaxMissionContents   = 15;

//------------------------------------------------------------------------------
/**
*/
uiQuestMan *
uiQuestListContent::GetManager()
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
uiQuestListContent::ClearInfo()
{
	if ( !this->CheckControls() )
		return;

	m_FTextSentence.Release();
	m_FTextRender.ClearSentence();

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
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::UpdateQuestInfo()
{
	ClearInfo();
	refreshQuestList();
	refreshQuestContent();
	refreshQuestCancelBtn();
	refreshQuestPreviewCheck();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::initInfo()
{
	m_pCtrlCheckCategory[0]->SetCheckState(FALSE);	// �̼�
	m_pCtrlCheckCategory[1]->SetCheckState(TRUE);	// ����Ʈ
	m_pCtrlCheckQuestType[0]->SetCheckState(FALSE); // ����Ʈ �̼�
	m_pCtrlCheckQuestType[1]->SetCheckState(TRUE);	// ����Ʈ ����

	GetCtrlProgressQuestList()->SetColorSelBar(BAR_COLOR_SELECT_LIST);
	GetCtrlQuestContentList()->SetColorSelBar(BAR_COLOR_SELECT_LIST_UNUSE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::updateInfo()
{
	m_FTextRender.SetRect( GetCtrlQuestContentList()->GetSizeRect() );
	updateMouseWheel();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::updateMouseWheel()
{
	if(GetCtrlProgressQuestList()->GetListRowCnt()>cMaxQuestList)
    {
		m_QuestListVscrInfo.ScrollRange.iMax = (GetCtrlProgressQuestList()->GetListRowCnt()+1)-cMaxQuestList;
    }
	else
    {
		m_QuestListVscrInfo.ScrollRange.iMax = 0;
    }

    
	GetCtrlProgressQuestVScroll()->SetScrollRangeWZ(
        0, 
        m_QuestListVscrInfo.ScrollRange.iMax );

	RECT rc;
	// QuestList
	rc = GetCtrlProgressQuestList()->GetSizeRect();
	switch (GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			//.���� ȭ��ǥ�� ������.
			int index = GetCtrlProgressQuestVScroll()->GetScrollPosWZ();

			--index;
			index = max(0, index);
			m_QuestListVscrInfo.iStartDrawIndex = m_QuestListVscrInfo.iCurPos = index;
			GetCtrlProgressQuestVScroll()->SetScrollPosWZ( index );
            GetManager()->SetCurPosProgressListVscr(index);

            GetCtrlProgressQuestList()->SetBeginDrawIndex( index );
            GetManager()->SetCurBeginDrawProgressList( index );
		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			int index = GetCtrlProgressQuestVScroll()->GetScrollPosWZ();

			++index;
            int imax = GetCtrlProgressQuestList()->GetListRowCnt();							
			index = min(imax, index);
			m_QuestListVscrInfo.iStartDrawIndex = m_QuestListVscrInfo.iCurPos = index;			
			GetCtrlProgressQuestVScroll()->SetScrollPosWZ( index );
            GetManager()->SetCurPosProgressListVscr(index);

			GetCtrlProgressQuestList()->SetBeginDrawIndex( index );
            GetManager()->SetCurBeginDrawProgressList( index );
		}
		break;
	}

	int iMaxLine = 0;

	if(m_FTextRender.GetMaxLine()>cMaxMissionContents)
	{
		iMaxLine = (m_FTextRender.GetMaxLine()+1) - cMaxMissionContents;
	}

	GetCtrlQuestContentVScroll()->SetScrollRangeWZ(0, iMaxLine );

	rc = GetCtrlQuestContentList()->GetSizeRect();
	switch (GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			//.���� ȭ��ǥ�� ������.
			int pos = GetCtrlQuestContentVScroll()->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestContentVScroll()->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
			{
				pos--;      // ���� ���� ��Ű�� 
				GetCtrlQuestContentVScroll()->SetScrollPosWZ(pos);
				m_FTextRender.SetBeginLineIndex(pos);

			}
		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			int pos = GetCtrlQuestContentVScroll()->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			GetCtrlQuestContentVScroll()->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
			{
				pos++;      // ���� ���� ��Ű�� 
				GetCtrlQuestContentVScroll()->SetScrollPosWZ(pos);
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
uiQuestListContent::ListMessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_LINETOP:
		{
			//.���� ȭ��ǥ�� ������.
			//
			if ( GetWzIDQuestContentVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestContentVScroll()->GetScrollPosWZ();

				--index;
				index = max(0, index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestContentVScroll()->SetScrollPosWZ( index );
			}
			else
			if ( GetWzIDProgressQuestVScroll() == pMessage->dwCtrlID)
			{
				int pos = GetCtrlProgressQuestVScroll()->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
				int mmin = 0 , mmax = 0;
				GetCtrlProgressQuestVScroll()->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

				if(pos>mmin)// ���� �Էµ� ���μ� ���� ������ 
				{
					pos--;													//���� ���� ��Ű�� 
					GetCtrlProgressQuestVScroll()->SetScrollPosWZ(pos);
                    GetManager()->SetCurPosProgressListVscr(pos);
					//visible line ���� 
					GetCtrlProgressQuestList()->SetBeginDrawIndex( pos );
                    GetManager()->SetCurBeginDrawProgressList( pos );
				}
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			if ( GetWzIDQuestContentVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestContentVScroll()->GetScrollPosWZ();

				++index;
				index = min((int)m_FTextRender.GetMaxLine(), index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestContentVScroll()->SetScrollPosWZ( index );
			}
			else
			if ( GetWzIDProgressQuestVScroll() == pMessage->dwCtrlID)
			{
				if(GetCtrlProgressQuestList()->GetListRowCnt()>cMaxQuestList)
                {
					m_QuestListVscrInfo.ScrollRange.iMax = (GetCtrlProgressQuestList()->GetListRowCnt()+1)-cMaxQuestList;
                }
				else
                {
					m_QuestListVscrInfo.ScrollRange.iMax = 0;
                }


                GetCtrlProgressQuestVScroll()->SetScrollRangeWZ(
                    0, 
                    m_QuestListVscrInfo.ScrollRange.iMax);

                int index = GetCtrlProgressQuestVScroll()->GetScrollPosWZ();

                ++index;
                index = min(GetCtrlProgressQuestList()->GetListRowCnt(), index);
                m_QuestListVscrInfo.iStartDrawIndex = m_QuestListVscrInfo.iCurPos = index;			
                GetCtrlProgressQuestVScroll()->SetScrollPosWZ( index );
                GetManager()->SetCurPosProgressListVscr(index);
                GetCtrlProgressQuestList()->SetBeginDrawIndex( index );
                GetManager()->SetCurBeginDrawProgressList( index );
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			//.���� �巡������ ���콺�� ���Ҵ�.
			if ( GetWzIDQuestContentVScroll() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestContentVScroll()->GetScrollPosWZ();
				m_FTextRender.SetBeginLineIndex( index );
			}
			else if (GetWzIDProgressQuestVScroll() == pMessage->dwCtrlID)
			{
				m_QuestListVscrInfo.iCurPos = GetCtrlProgressQuestVScroll()->GetScrollPosWZ();
				m_QuestListVscrInfo.iStartDrawIndex = m_QuestListVscrInfo.iCurPos;
                GetManager()->SetCurPosProgressListVscr(m_QuestListVscrInfo.iStartDrawIndex);
				GetCtrlProgressQuestList()->SetBeginDrawIndex( m_QuestListVscrInfo.iStartDrawIndex );				
                GetManager()->SetCurBeginDrawProgressList( m_QuestListVscrInfo.iStartDrawIndex );
			}
		}
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::refreshQuestList()
{
	if ( !g_pHero || !GetManager() || !CheckControls() )
		return;

	GetCtrlProgressQuestList()->DeleteItemAll();

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

	int index = 0;
	// ������ �ִ� ����Ʈ�� ������ �߷�����.
	for ( int i = 1; i < MAX_QUEST_COMPL_NUM; ++i)
	{
		Quest * pQuest = g_pQMGR_Real->FindQuest(i);
		if (!pQuest)
		{
			continue;
		}

		itr = m_QuestGroup.find( pQuest->GetQuestInfo()->GetGroupCode() );
		if ( itr == m_QuestGroup.end() )
		{
			QUEST_LIST QuestList;
			QuestList.push_back(pQuest);
			m_QuestGroup.insert( QUEST_GROUP_PAIR(pQuest->GetQuestInfo()->GetGroupCode(), QuestList));			
		}
		else
		{
			itr->second.push_back( pQuest );
		}
	}

	// ����Ʈ�� ������ �߰��Ѵ�.

	itr = m_QuestGroup.begin();
	while (itr != m_QuestGroup.end()) 
	{
		DWORD dwGroupID = itr->first;
		GetUIMan()->GetInterfaceString(dwGroupID, m_szText, INTERFACE_STRING_LENGTH);
        
		index = GetCtrlProgressQuestList()->InsertItemBack();

		GlobalFunc::SetItem(
			GetCtrlProgressQuestList(), 
			index, 0, m_szText, 
			FONT_QUEST_LIST_CATEGORY, 
			DT_VCENTER | DT_LEFT, 
			TEXT_BG_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_QUEST_LIST_CATEGORY,
			TEXT_COLOR_ON_SELECT_LIST );

		GetCtrlProgressQuestList()->SetItemData(index, 0);

		QUEST_LIST::iterator ListItr = itr->second.begin();
		while ( ListItr != itr->second.end() )
		{
			index = GetCtrlProgressQuestList()->InsertItemBack();

			Quest * pQuest = (*ListItr);
			DWORD dwQuestStrID = pQuest->GetQuestInfo()->GetQNCode();
			DWORD dwItemTextColor = 0;

			GetUIMan()->GetInterfaceString(dwQuestStrID, m_szText, INTERFACE_STRING_LENGTH);

			TCHAR szMessage[INTERFACE_STRING_LENGTH];

			switch ( pQuest->GetQState() )
			{
			case QS_COMPLETED:
				{
					dwItemTextColor = TEXT_COLOR_QUEST_LIST_COMPLETE;
					// (�Ϸ�) ( 9051 )
					GetUIMan()->GetInterfaceString( 9051, szMessage, INTERFACE_STRING_LENGTH ); 
					_tcscat(m_szText, szMessage);

				}
				break;
			case QS_FAILED:
				{
					dwItemTextColor = TEXT_COLOR_QUEST_LIST_COMPLETE;
					// (����) ( 9052 )
					GetUIMan()->GetInterfaceString( 9052, szMessage, INTERFACE_STRING_LENGTH ); 
					_tcscat(m_szText, szMessage );
				}
				break;
			case QS_NORMAL:
				{
					dwItemTextColor = TEXT_COLOR_QUEST_LIST_GENERAL;
				}				
			default:
				break;
			}

			if ( pQuest->GetQuestInfo()->IsChildQuest() )
			{
				StrCopy(m_szResult, _T("    "));
			}
			else
			{
				StrCopy(m_szResult, _T("  "));
			}
            			
			_tcscat(m_szResult, m_szText);
			GlobalFunc::SetItem(
				GetCtrlProgressQuestList(), 
				index, 0, m_szResult, 
				FONT_QUEST_LIST_GENERAL, 
				DT_VCENTER | DT_LEFT, 
				TEXT_BG_COLOR_QUEST_LIST_CATEGORY,
				dwItemTextColor,
				TEXT_COLOR_ON_SELECT_LIST );

			GetCtrlProgressQuestList()->SetItemData(index, pQuest);			

			++ListItr;
		}

		++itr;
	}
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

	GetCtrlProgressQuestList()->SetUseReport(FALSE);

	if ( -1 != GetManager()->GetCurSelectQuestIndex() )
	{
		GetCtrlProgressQuestList()->SetCurSel(GetManager()->GetCurSelectQuestIndex());
	}       

	Sprintf( m_szResult, _T("%d/%d"), g_pQMGR_Real->QuestNum(), MAX_QUEST_PROGR_NUM);
	SetCaptionQuestCount(m_szResult);
} 

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::refreshQuestContent()
{
	if ( !g_pHero || !GetManager() || !CheckControls() )
		return;

	GetCtrlQuestContentList()->DeleteItemAll();
	GetCtrlQuestContentList()->SetUseReport(FALSE);

	int iQuestindex = GetManager()->GetCurSelectQuestIndex();
	if ( -1 == iQuestindex )
	{
		return;
	}

	Quest * pQuest = (Quest *)GetCtrlProgressQuestList()->GetItemData(iQuestindex);
	if ( !pQuest )
	{
		return;
	}

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

//-------------------------------------------------------------------------------------------
/** ����Ʈ ���� ��ư SHOW / HIDE
*/
void					
uiQuestListContent::refreshQuestCancelBtn()
{
	if ( !g_pHero || !GetManager() || !CheckControls() )
		return;

	ShowQuestCancelBtn(FALSE);

	int iQuestindex = GetManager()->GetCurSelectQuestIndex();
	if ( -1 == iQuestindex )
	{
		return;
	}

	Quest * pQuest = (Quest *)GetCtrlProgressQuestList()->GetItemData(iQuestindex);
	if ( !pQuest )
	{
		return;
	}

	if ( pQuest->GetQuestInfo()->IsChildQuest() )
	{
		return;
	}

	ShowQuestCancelBtn(TRUE);	
}



//-------------------------------------------------------------------------------------------
/** 
*/
void					
uiQuestListContent::refreshQuestPreviewCheck()
{
	if ( !CheckControls() )
		return;

	SetCheckQuestPreviewChk( GetManager()->GetQuestViewProgress() );
}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::clickChkCategory(POSTYPE pos)
{
	switch (pos)
	{
	case eDLGPOS_CHK_MISSION:
		{
			GetManager()->OpenForceMissionList();
		}
		break;
	case eDLGPOS_CHK_QUEST:
		{
			// ���簡 ����Ʈâ�̱� ������ ����.
		}
		break;
	}

	pos -= QUEST_CATEGORY_BEGIN;
	for ( int i = 0; i < QUEST_CATEGORY_SIZE; ++i )
	{
		this->m_pCtrlCheckCategory[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckCategory[pos]->SetCheckState(TRUE);	
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::clickChkQuestType(POSTYPE pos)
{
	switch (pos)
	{
	case eDLGPOS_CHK_QUEST_MISSION:
		{
		}
		break;
	case eDLGPOS_CHK_QUEST_CONTENT:
		{			
			//GetManager()->OpenQuestListContent();
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
uiQuestListContent::clickQuestList()
{
	int iIndex = GetCtrlProgressQuestList()->GetCurSel();
	if ( iIndex == -1 )
	{
		return;
	}

	Quest * pQuest = (Quest *)GetCtrlProgressQuestList()->GetItemData(iIndex);
	if ( !pQuest )
	{
		return;
	}

	if ( g_Input.IsPressShiftKey() )
	{
		GetManager()->AddQuestForProgress( pQuest->GetQuestCode() );
	}


	if ( GetManager()->GetCurSelectQuestIndex() == iIndex )
	{
		return;
	}

	GetManager()->SetCurSelectQuestIndex(iIndex);
	GetManager()->SetCurSelectQuestCode(pQuest->GetQuestCode());

	UpdateQuestInfo();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::clickRbtnQuestList()
{
	int iIndex = GetCtrlProgressQuestList()->GetCurSel();

	if ( iIndex == -1 )
	{
		return;
	}

	Quest * pQuest = (Quest *)GetCtrlProgressQuestList()->GetItemData(iIndex);
	if ( !pQuest )
	{
		return;
	}

	GetManager()->SetCurSelectQuestIndex(iIndex);
	GetManager()->SetPendingAbandonQuestCode( pQuest->GetQuestCode() );

	GetUIMan()->GetInterfaceString( 
		pQuest->GetQuestInfo()->GetQNCode(), 
		m_szText, 
		INTERFACE_STRING_LENGTH );

	if ( pQuest->GetQuestInfo()->IsChildQuest() )
	{
		return;
	}

	UpdateQuestInfo();

    // 5904 [%s] ����Ʈ�� �����Ͻðڽ��ϱ�?	
    TCHAR szBufMessage[INTERFACE_STRING_LENGTH];
    GetUIMan()->GetInterfaceString( 5904, szBufMessage, INTERFACE_STRING_LENGTH );
    Sprintf(m_szResult, szBufMessage, m_szText);

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Confirm(m_szResult, GlobalFunc::QuestAbandon);

}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::clickQuestPreviewCheck()
{
	if ( !CheckControls() )
		return;

	GetManager()->SetQuestViewProgress( !GetManager()->GetQuestViewProgress() );

	refreshQuestPreviewCheck();


	if( GetManager()->GetQuestViewProgress() )
	{
		// ShiftŰ�� ������ ����Ʈ ����Ʈ�� Ŭ���ϸ� ����Ʈ �����Ȳ�� ǥ�õ˴ϴ�.
		TCHAR msg[INTERFACE_STRING_LENGTH];
		GetUIMan()->GetInterfaceString( 5261, msg, INTERFACE_STRING_LENGTH );

		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW , msg );
	}

}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestListContent::clickQuestAbandon()
{
	TCHAR szText[INTERFACE_STRING_LENGTH];
	int iIndex = GetManager()->GetCurSelectQuestIndex();

	if ( iIndex == -1 )
	{
		GetManager()->SetCurSelectQuestCode(0);
		return;
	}

	Quest * pQuest = (Quest *)GetCtrlProgressQuestList()->GetItemData(iIndex);
	if ( !pQuest )
	{
		GetManager()->SetCurSelectQuestCode(0);
		return;
	}

	GetManager()->SetCurSelectQuestIndex(iIndex);
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





//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
