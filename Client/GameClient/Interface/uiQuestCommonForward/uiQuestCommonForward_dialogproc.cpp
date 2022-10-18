//------------------------------------------------------------------------------
//  uiQuestCommonForward_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"



#include "uiQuestCommonForward.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

//------------------------------------------------------------------------------
WzID uiQuestCommonForward::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("B001"), //eDLG_BTN_JOB,
	StrToWzID("BT00"), //eDLG_BTN_TAB,
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    StrToWzID("S001"), //eDLG_TXT_INIT_DATE,
#endif //_NA_003027_20111013_HONOR_SYSTEM
	StrToWzID("B999"), //eDLG_BTN_EXIT,
	StrToWzID("L001"), //eDLG_LST_QUEST, 
#ifdef _GS_ADD_QUEST_SCROLL_1143
  #ifdef _SCJ_111202_UI_RENEWAL
    StrToWzID("V001"), //eDLG_SCR_QUEST,
  #else
	StrToWzID("VS01"), //eDLG_SCR_QUEST, 
  #endif //_SCJ_111202_UI_RENEWAL
#endif//_GS_ADD_QUEST_SCROLL_1143(중국스크롤바추가)

};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestCommonForward::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        if (!this->GetManager())
			return;

		if (GetManager()->IsNPCWindowPosActivated())
		{
			POINT pt = GetManager()->GetNPCWindowPos();
			GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
			GetManager()->DeactivatedNPCWindowPos();
		}
		else
		{
			//MoveWindowWZ( -1.0f, 120.0f );
		}

		initInfo();
    }
    else
    {
		this->ClearInfo();

		if (!this->CheckControls())
			return;

		if (!this->GetManager())
			return;

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
        if (pt.x != 0 && pt.y != 0)
        {
            GetManager()->SetNPCWindowPos(pt);
        }

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Quest_NPC);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    if (!this->GetManager())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( pos )
			{
			case eDLG_BTN_JOB:
				{
					clickBtnJob(pos);
				}
				break;

			case eDLG_BTN_EXIT:
				{
					this->ShowInterfaceWithSound(FALSE);
				}
				break;

			case eDLG_LST_QUEST:
				{
                    if (GetManager() && GetManager()->SafePKBool(ePKBOOL_QUEST_REWARD))  
                    {  
                        clickList();  
                    }  
                    else  
                    {  
                        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};  
                        // 5062 잠시 후에 다시 시도해 주시기 바랍니다.  
                        g_InterfaceManager.GetInterfaceString(5062, szMessage, MAX_MESSAGE_LENGTH );  
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);  
                    }
				}
				break;

			default:
				break;
            }
        }
        break;
#ifdef _GS_ADD_QUEST_SCROLL_1143
	case RT_MSG_SB_PAGETOP:	
	case RT_MSG_SB_LINETOP:
		{
			UpScroll();
		}
		break;
	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			DownScroll();
		}
		break;
	case RT_MSG_SB_THUMBTRACK:			
	case RT_MSG_SB_THUMBPOSITION:		
	case RT_MSG_SB_ENDSCROLL:			
		{
			if(GetCtrlQuestList() && m_pScroll)
				GetCtrlQuestList()->SetBeginDrawIndex(m_pScroll->GetScrollPosWZ());
		}
		break;
#endif//_GS_ADD_QUEST_SCROLL_1143(중국스크롤바처리)

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}
//----------------------------------------------------------------------------
/** 툴팁처리
*/
void uiQuestCommonForward::renderTooltip()
{
	if ( eMOUSE_OVER_QUEST == m_MouseOverObject.eMouseOverType)
	{
		CCtrlListWZ *pList = GetCtrlQuestList();

		if(!pList) return;
	
		int nIndex = -1;
		RECT rc = pList->GetSizeRect();
		int nHeight = pList->GetSellheight();
		int iMouseX = Mouse::Instance()->GetMouseX();
		int iMouseY = Mouse::Instance()->GetMouseY();
				
		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				int nGap = iMouseY-rc.top;

				//0보다 작을수는없다.
				if(nGap>0)
				{
					nIndex = nGap/nHeight; //현재커서가 가리키는 리스트인덱스
				}
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

				if( pUIMan && nIndex!=-1)
				{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                    QCODE quset_code = (QCODE)GetCtrlQuestList()->GetItemData(nIndex);
                    Quest* pQuest = g_pQMGR_Real->FindQuest(quset_code);
                    if (NULL == pQuest)
                    {
                        pQuest = g_pQMGR_Temp->FindQuest(quset_code);
                    }
#else
					Quest * pQuest = (Quest *)GetCtrlQuestList()->GetItemData(nIndex);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

					if(pQuest)
					{
						//퀘스트명랜더링
						TCHAR szQName[INTERFACE_STRING_LENGTH + 1] = {0,};
						g_InterfaceManager.GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(),szQName);

						//퀘완료시 뒤에 붙음(완료) ( 9051 )
						if(QS_COMPLETED == pQuest->GetQState())
						{
							TCHAR szSuc[INTERFACE_STRING_LENGTH] = {0,};
							GetUIMan()->GetInterfaceString( 9051, szSuc);                     
							_tcscat(szQName, szSuc );
						}

						pUIMan->InitTooltip();
						pUIMan->RegisterTooltipSimpleText(szQName);
						pUIMan->SetTooltipDialogID(GetDialogKey());
					}
				}


			}
		}

	}
}
//----------------------------------------------------------------------------
/** 마우스 이벤트
*/
void uiQuestCommonForward::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if( pMessage->eResultMsg  == RT_MSG_MOUSEOVER) 
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

		if(getCtrlIDToPos(pMessage->dwCtrlID)==eDLG_LST_QUEST)
		{	
			m_MouseOverObject.eMouseOverType = eMOUSE_OVER_QUEST;
		}
	}
}
//----------------------------------------------------------------------------
/** 마우스 휠처리
*/
void
uiQuestCommonForward::UpdateMouseWheel()
{
	//1.영역체크는 다이얼로그 전체사이즈로 하겠씀
	if(!GetDialogWZ() || !m_pScroll) return;
	
	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);

	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP: UpScroll();break;

	case MOUSE_WHEEL_DOWN:DownScroll();break;
	}
}
//----------------------------------------------------------------------------
/** 스크롤 다운
*/
void
uiQuestCommonForward::DownScroll(int nRange)
{
	if(!m_pScroll) return; 

	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));

		m_pScroll->SetScrollPosWZ(pos);

		GetCtrlQuestList()->SetBeginDrawIndex(m_pScroll->GetScrollPosWZ());

	}
}
//----------------------------------------------------------------------------
/** 스크롤 업
*/
void
uiQuestCommonForward::UpScroll(int nRange)
{
	if(!m_pScroll) return; 

	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));

		m_pScroll->SetScrollPosWZ(pos);

		this->GetCtrlQuestList()->SetBeginDrawIndex(m_pScroll->GetScrollPosWZ());
	}

}

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiQuestCommonForward::UpdatePosition()
//{
//    //SolarDialog::UpdatePosition();
//}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	updateInfo();

#ifdef _GS_ADD_QUEST_SCROLL_1143
	UpdateMouseWheel();
#endif//_GS_ADD_QUEST_SCROLL_1143(스크롤바마우스휠처리)	

}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::OnRenderSolarDialog()
{
	m_FTextRender.Render();
	renderTooltip();
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
