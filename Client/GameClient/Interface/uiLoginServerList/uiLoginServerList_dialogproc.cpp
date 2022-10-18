//------------------------------------------------------------------------------
//  uiLoginServerList_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginServerList.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "InterfaceManager.h"
#include <Protocol_CL.h>
#include <PacketStruct_CL.h>
#include "LoadScene.h"
#include "Mouse.h"
#include "uiLoginMan/uiLoginMan.h"
#include "globalfunc.h"

//------------------------------------------------------------------------------
WzID uiLoginServerList::m_wzId[DIALOG_MAX] = 
{ 
#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
#else
	StrToWzID("B999"),  // DLGPOS_BTN_CLOSE,
#endif     
    StrToWzID("T001"),  // DLGPOS_TXT_TITLE,
    StrToWzID("T002"),  // DLGPOS_TXT_HELP,
    StrToWzID("L001"),  // DLGPOS_LST_SERVERS,
    StrToWzID("V000"),  // DLGPOS_SCR_VERTICAL,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiLoginServerList::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        //MoveWindowWZ( 330.0f, 330.0f );

        MoveDefaultWindowPos();

        this->GetDialogWZ()->GetClientRect(&m_fixRC);
        m_fixRC.left += 100;

        this->OffPending();
        this->OffLock();
    
        CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( DLGPOS_LST_SERVERS );
        pLCtrl->DeleteItemAll();
        pLCtrl->SetBeginDrawIndex(0);
        pLCtrl->SetUnuseSelBar(TRUE);
        pLCtrl->SetCurSel(-1);

        this->OffLock();
    }
    else
    {
        this->OnPending();
        this->OnLock();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
		// no operation
#else
		case RT_MSG_LBUTTONCLICK:
			{
				if(getCtrlIDToPos( pMessage->dwCtrlID )==DLGPOS_BTN_CLOSE)
				{
					this->GetManager()->resetTryReconnect();
				}
			}
			break;
#endif//_REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
    case RT_MSG_LBUTTONDCLICK:
        {
            
        }
        break;

    case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( DLGPOS_SCR_VERTICAL );
            int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
            {
                pos--;      // 한줄 감소 시키고 
                pScroll->SetScrollPosWZ(pos);
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
        {
            //.아랫쪽 화살표를 눌렀다
            CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( DLGPOS_SCR_VERTICAL );
            int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
            {
                pos++;      // 한줄 증가 시키고 
                pScroll->SetScrollPosWZ(pos);
            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::OnUpdateSolarDialog()
{
    RECT rc;
    GetDialogWZ()->GetClientRect(&rc);
    switch (this->GetMouseWheelStatus(&rc))
    {
    default:
        break;
    case MOUSE_WHEEL_UP:
        {
            //.윗쪽 화살표를 눌렀다.
            CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( DLGPOS_SCR_VERTICAL );
            int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
            {
                pos--;      // 한줄 감소 시키고 
                pScroll->SetScrollPosWZ(pos);
            }
        }
        break;

    case MOUSE_WHEEL_DOWN:
        {
            //.아랫쪽 화살표를 눌렀다
            CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( DLGPOS_SCR_VERTICAL );
            int pos = pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
            {
                pos++;      // 한줄 증가 시키고 
                pScroll->SetScrollPosWZ(pos);
            }
        }
        break;
    }


	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if (this->GetManager())
    {
		if (!GetManager()->GetAuthGroupNum())
			return;

		if (m_pLstControl)
		{
			m_pLstControl->DeleteItemAll();
			m_pLstControl->SetBeginDrawIndex(0);
		}
		
		int curScrollPos = this->m_pVScrollControl->GetScrollPosWZ();
		int start = (c_iMAX_SERVER_LIST * curScrollPos);
		int end = start + c_iMAX_SERVER_LIST;

		end = min (end, GetManager()->GetAuthGroupNum());

		for (int i = start;i < end;++i)
		{
			AuthGroupData* pAuthGroupData = GetManager()->GetAuthGroupByIndex(i);

			if (!pAuthGroupData)
				continue;
			
			if (m_pLstControl)
			{
				int idx = m_pLstControl->InsertItemBack();
				Sprintf (szMessage, _T("%s"), pAuthGroupData->GetAuthGroupName());
				GlobalFunc::SetItemText( m_pLstControl, idx, szMessage );
				m_pLstControl->SetItemTextColor(idx, 0, RGBA(235,192,38, 255));
				m_pLstControl->SetItemFontID( idx,  0, StrToWzID("m216"));
				m_pLstControl->SetItemBKColor(idx, 0, RGBA(0,0,0,0));
				m_pLstControl->SetColorSelBar(RGBA(0,0,150,150));
				m_pLstControl->SetItemData(idx,(VOID *)i);
			}
		}
        if (this->m_pVScrollControl)
		{
			this->m_pVScrollControl->SetScrollRangeWZ(0, ((m_pLstControl->GetListRowCnt()-1)/c_iMAX_SERVER_LIST));
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::OnRenderSolarDialog()
{
	const int c_ListHeight = 20;
	RECT rtSrc = {0, 0, PROGRAMPARAM->GetNewIconWidth(), PROGRAMPARAM->GetNewIconHeight()};

	int curScrollPos = this->m_pVScrollControl->GetScrollPosWZ();
	int start = (c_iMAX_SERVER_LIST * curScrollPos);
	int end = start + c_iMAX_SERVER_LIST;

	end = min (end, GetManager()->GetAuthGroupNum());
	set<int>& newServer = PROGRAMPARAM->GetNewServerSet();

	for (int i = start;i < end;++i)
	{
		AuthGroupData* pAuthGroupData = GetManager()->GetAuthGroupByIndex(i);

		if ( newServer.find(pAuthGroupData->GetAuthGroupID()) != newServer.end() )		
		{
			RECT rt = m_pLstControl->GetSizeRect();
			rt.left += 127;
			rt.right = rt.left + rtSrc.right;
			rt.top += i * c_ListHeight;
			rt.top += 2;
			rt.bottom = rt.top + rtSrc.bottom;	

			if( m_pNewServerIcon )
			{
				m_pNewServerIcon->Render(&rt,&rtSrc);
			}
		}		
	}	
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
