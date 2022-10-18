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
#endif//_GS_ADD_QUEST_SCROLL_1143(�߱���ũ�ѹ��߰�)

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
                        // 5062 ��� �Ŀ� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.  
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
#endif//_GS_ADD_QUEST_SCROLL_1143(�߱���ũ�ѹ�ó��)

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}
//----------------------------------------------------------------------------
/** ����ó��
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

				//0���� �������¾���.
				if(nGap>0)
				{
					nIndex = nGap/nHeight; //����Ŀ���� ����Ű�� ����Ʈ�ε���
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
						//����Ʈ������
						TCHAR szQName[INTERFACE_STRING_LENGTH + 1] = {0,};
						g_InterfaceManager.GetInterfaceString(pQuest->GetQuestInfo()->GetQNCode(),szQName);

						//���Ϸ�� �ڿ� ����(�Ϸ�) ( 9051 )
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
/** ���콺 �̺�Ʈ
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
/** ���콺 ��ó��
*/
void
uiQuestCommonForward::UpdateMouseWheel()
{
	//1.����üũ�� ���̾�α� ��ü������� �ϰھ�
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
/** ��ũ�� �ٿ�
*/
void
uiQuestCommonForward::DownScroll(int nRange)
{
	if(!m_pScroll) return; 

	int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
	{
		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));

		m_pScroll->SetScrollPosWZ(pos);

		GetCtrlQuestList()->SetBeginDrawIndex(m_pScroll->GetScrollPosWZ());

	}
}
//----------------------------------------------------------------------------
/** ��ũ�� ��
*/
void
uiQuestCommonForward::UpScroll(int nRange)
{
	if(!m_pScroll) return; 

	int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
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
#endif//_GS_ADD_QUEST_SCROLL_1143(��ũ�ѹٸ��콺��ó��)	

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
