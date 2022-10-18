#include "SunClientPrecompiledHeader.h"
#include "uiOptionView.h"

#include "TextureListInfoHandleManager.h"
#include "TextureListInfoParser.h"
#include "Mouse.h"

#include "InterfaceManager.h"
#include "GameOptionListParser.h"
#include "GlobalFunc.h"

#include "uiOptionMan.h"
#include "uiToolTipMan/uiToolTipMan.h"

const int kLineCount = 18;
const int c_AnimationFullTime = 1;

////////////////////////////////////////////////////////////////////////
WzID uiOptionView::m_wzId[eOPTION_MAX] =
{
	StrToWzID("C004"),  //eKEYMAP_CHECK_C004 = 0,
};


////////////////////////////////////////////////////////////////////////
cOptionListControl::cOptionListControl()

{
}

cOptionListControl::~cOptionListControl()
{
    m_ItemArray.clear();

    // Release instance
    for( size_t i=0;i!=m_Categorys.size();++i)
    {
        cOptionCategory* pCategory = m_Categorys[i];
        delete pCategory;
		pCategory = NULL;
    }
    m_Categorys.clear();
}


cOptionCategory* cOptionListControl::FindCategory( const std::string& CategoryName )
{
    for( size_t i=0;i!=m_Categorys.size();++i)
    {
        cOptionCategory* pCategory = m_Categorys[i];
        if( pCategory->GetTitle() == CategoryName )
        {
            return pCategory;
        }
    }

    return NULL;
}

void cOptionListControl::LoadPage( DWORD PageIndex )
{
    std::vector<sOptionItemInfo*> Array;
    OptionItemInfoParser::Instance()->GetOptionItemInfoPage( PageIndex, Array );

    for( size_t i=0;i!=Array.size();++i)
    {
        sOptionItemInfo* pOptionInfo = Array[i];
        cOptionCategory* pCategory = FindCategory( pOptionInfo->m_CategoryName);

        if( pCategory == NULL )
        {
            pCategory = new cOptionCategory( pOptionInfo->m_CategoryName );
            m_Categorys.push_back( pCategory );
        }

        nOptionType::eOptionTypeIndex TypeIndex = nOptionFacade::GetOptionTypeIndexFromOptionID( pOptionInfo->m_OptionID );

        if( pOptionInfo->m_ItemType == 0 )
        {
            cOptionComboItem* pItem = new cOptionComboItem();
            pItem->SetTitleText( pOptionInfo->m_DisplayName );
            pItem->SetOptionID( pOptionInfo->m_OptionID );
            pItem->SetOptionTypeIndex( TypeIndex );
            pItem->SetDescriptionText( pOptionInfo->m_Comment );

            // 해상도 설정
            if( pOptionInfo->m_OptionID == 101 )
            {
                for( int j=0;j!=CGameOption::Instance()->GetAvailableScreenSettingCount();++j)
                {
                    const SCREEN_SETTING& ScreenSetting = CGameOption::Instance()->GetAvailableScreenSetting(j);
                    pItem->AddItemText( ScreenSetting.SettingText );
                }
            }
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
			// 하임 거래량 알림
			else if( pOptionInfo->m_OptionID == 236 )
			{
				for( size_t j=0;j!=pOptionInfo->m_ItemDataArray.size();++j)
				{
					if (j > 0)
					{
						TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
						TCHAR szMoneyFinal[INTERFACE_STRING_LENGTH] = {0,};

						NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

						INT64 money_number = _atoi64(pOptionInfo->m_ItemDataArray[j].c_str());
						Sprintf(szMoney, _T("%I64u"), money_number);
						::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));
						DWORD money_color;
						money_color = GlobalFunc::GetMoneyWzColor(money_number);

						pItem->AddItemText( szMoneyFinal );
						pItem->AddItemTextColor(money_color);
					}
					else
					{
						pItem->AddItemText( pOptionInfo->m_ItemDataArray[j] );
						pItem->AddItemTextColor(0xAAFFFFFF);
					}
				}
			}
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
            else
            {
                for( size_t j=0;j!=pOptionInfo->m_ItemDataArray.size();++j)
                {
                    pItem->AddItemText( pOptionInfo->m_ItemDataArray[j] );
                }
            }

            pCategory->AddItem( pItem );

            m_ItemArray.push_back( pItem );
        }
        else if( pOptionInfo->m_ItemType == 1 )
        {
            cOptionGaugeItem* pItem = new cOptionGaugeItem();

            pItem->SetTitleText( pOptionInfo->m_DisplayName );
            pItem->SetOptionID( pOptionInfo->m_OptionID );
            pItem->SetOptionTypeIndex( TypeIndex );
            pItem->SetDescriptionText( pOptionInfo->m_Comment );

            pItem->SetGaugeRange( pOptionInfo->m_GaugeRange );
            pItem->SetGaugePosition( pOptionInfo->m_GaugeRange );
            pCategory->AddItem( pItem );

            m_ItemArray.push_back( pItem );

        }
    }
}


void cOptionListControl::UpdateVisibleItemArray( std::vector<cOptionItemBase*>& Array )
{
    Array.clear();
    for( size_t i=0;i!=m_Categorys.size();++i)
    {
        cOptionCategory* pCategory = m_Categorys[i];
        Array.push_back(pCategory);

        if( pCategory->IsExpended() )
        {
            for( size_t j=0;j!=pCategory->GetItemCount();++j)
            {
                Array.push_back( pCategory->GetItem(j) );
            }
        }
    }
}

void cOptionListControl::RestoreOptionValue()
{
    for( size_t i=0;i!=m_ItemArray.size();++i)
    {
        cOptionItem* pItem = m_ItemArray[i];
        nOptionFacade::RestoreOptionValue( pItem );
    }
}

void cOptionListControl::ApplyOptionValue()
{
    for( size_t i=0;i!=m_ItemArray.size();++i)
    {
        cOptionItem* pItem = m_ItemArray[i];
        nOptionFacade::SetOptionValue( pItem );
    }
}



////////////////////////////////////////////////////////////////////////
uiOptionView::uiOptionView(InterfaceManager* ui_man)
: uiBase(ui_man)
,m_iArrayIndex(-1)
{
    m_Initialized = FALSE;

    m_PopupItem = NULL;
}

uiOptionView::~uiOptionView()
{


}

void uiOptionView::SetActivePageIndex( int ActiveTabIndex )
{
    SetPageIndex(ActiveTabIndex);
    UpdateControlState();
}

void uiOptionView::OnShowWindow( BOOL val )
{
    if( val == TRUE && m_Initialized == FALSE )
    {
        // 아 진짜 잡히면 죽여버림
        TextureListInfoHandleManager::Instance()->LoadIcon(10002);
        TextureListInfoHandleManager::Instance()->LoadIcon(10003);
        TextureListInfoHandleManager::Instance()->LoadIcon(10009);
        TextureListInfoHandleManager::Instance()->LoadIcon(10010);
        TextureListInfoHandleManager::Instance()->LoadIcon(10009);


        m_Initialized = TRUE;
        InitializeOptionView();
        for( size_t i=0;i!=m_PageList.size();++i)
        {
            m_PageList[i]->RestoreOptionValue();
        }
        //SetPageIndex(0);
        //UpdateControlState();
    }
    else if( val == TRUE )
    {
        for( size_t i=0;i!=m_PageList.size();++i)
        {
            m_PageList[i]->RestoreOptionValue();
        }
        //SetPageIndex(0);
        //UpdateControlState();
    }


    if( val == TRUE )
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_Option_System2;
        msg.wParam=InterfaceManager::DIALOG_OPTION;
        msg.DoSomething=GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_System2);
    }



    uiBase::OnShowWindow( val );
}


void uiOptionView::OnRenderSolarDialog()
{

    // DestRect
    if( CControlWZ* pListControl = GetControlWZ(StrToWzID("L000")) )
    {
        RECT list_rect = pListControl->GetSizeRect();
        m_ViewRect = list_rect;
    }

    // Mapping item to control
    int ScrollOffset = m_pScroll->GetScrollPosWZ();


    BOOL bOld2dMode,bOldRealSize;
    g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
    g_pSunRenderer->Set2DMode( TRUE );

    RECT LineRect = m_ViewRect;
    LineRect.bottom = LineRect.top + 24;

    for( size_t i=0;i!=m_VisibleItemArray.size();++i)
    {
        int index = i - ScrollOffset;

        if( index < 0 )
            continue;
        if( index >= kLineCount )
            continue;

        cOptionItemBase* pItem = m_VisibleItemArray[i];
        pItem->Render( LineRect );

#ifdef _NA_000000_20130218_OPTION_TOOLTIP
        if( pItem->GetType() == OPTION_ITEM_TYPE_ITEM ||
            pItem->GetType() == OPTION_ITEM_TYPE_GAUGE )
        {
            cOptionItem* pOptionItem = (cOptionItem*)pItem;
            RECT CaptionRect = LineRect;

            static int s_CaptionWidth = 220;
            CaptionRect.left += 10;
            CaptionRect.right = CaptionRect.left + s_CaptionWidth;

            POINT MousePoint = Mouse::Instance()->GetMousePoint();
            if( PtInRect( &CaptionRect, MousePoint ) )
            {
                if( uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP) )
                {
                    DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
                    pUIMan->RegisterTooltip( pOptionItem->GetDescriptionText().c_str() );
                    pUIMan->SetTooltipDialogID( GetDialogKey() );
                }
            }
        }
#endif //_NA_000000_20130218_OPTION_TOOLTIP



        LineRect.top += 24;
        LineRect.bottom += 24;
    }


    if( m_PopupItem != NULL )
    {
		DWORD font_ID = StrToWzID("m216");
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

        if( m_PopupItemRect.size() >= 10 )
        {
            m_pComboBackground->OnDraw(true);
            m_pComboScroll->OnDraw(true);

            int ComboScrollOffset = m_pComboScroll->GetScrollPosWZ();
            for( size_t i=0;i!=m_PopupItemRect.size();++i)
            {
                int VisibleIndex = i - ComboScrollOffset;

                if( VisibleIndex < 0 )
                    continue;

                if( VisibleIndex >= 5 )
                    continue;


                RECT DestRect = m_PopupItemRect[i].m_Rect;
                const std::string& Text = m_PopupItemRect[i].m_Text;

                OffsetRect(&DestRect, 0, ComboScrollOffset * -18 );

                RECT TitleRect = DestRect;
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
				g_pSunRenderer->x_pManagerTextOut->DrawText( Text.c_str(), &TitleRect, m_PopupItem->GetItemIndex() == i ? 0xFFFFFF77 : m_PopupItem->GetItemTextColor(i), WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);
#else
                g_pSunRenderer->x_pManagerTextOut->DrawText( Text.c_str(), &TitleRect, m_PopupItem->GetItemIndex() == i ? 0xFFFFFF77 : 0xAAFFFFFF, WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
            }
        }
        else
        {
            m_pComboBackground->OnDraw(true);

            for( size_t i=0;i!=m_PopupItemRect.size();++i)
            {
                RECT DestRect = m_PopupItemRect[i].m_Rect;
                const std::string& Text = m_PopupItemRect[i].m_Text;

                RECT TitleRect = DestRect;


#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
				g_pSunRenderer->x_pManagerTextOut->DrawText( Text.c_str(), &TitleRect, m_PopupItem->GetItemIndex() == i ? 0xFFFFFF77 : m_PopupItem->GetItemTextColor(i), WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);
#else
                g_pSunRenderer->x_pManagerTextOut->DrawText( Text.c_str(), &TitleRect, m_PopupItem->GetItemIndex() == i ? 0xFFFFFF77 : 0xAAFFFFFF, WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
            }
        }

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }

    g_pSunRenderer->Set2DMode(bOld2dMode,bOldRealSize);

    

}

void uiOptionView::MoveComboScroll( int Move )
{
    if( Move == 0 )
        return;

    int Position = m_pComboScroll->GetScrollPosWZ();
    int ResultCode = m_pComboScroll->SetScrollPosWZ( (float)(Position + Move) );

    if( ResultCode != 0)
    {
        int Min,Max;
        m_pComboScroll->GetScrollRangeWZ( &Min, &Max );

        if( ResultCode == -1 )
            m_pComboScroll->SetScrollPosWZ( Min );
        else if( ResultCode == 1 )
            m_pComboScroll->SetScrollPosWZ( Max );
    }
}

void uiOptionView::MoveScroll( int Move )
{
    SetPopupItem(NULL);

    if( Move == 0 )
        return;

    int Position = m_pScroll->GetScrollPosWZ();
    int ResultCode = m_pScroll->SetScrollPosWZ( (float)(Position + Move) );

    if( ResultCode != 0)
    {
        int Min,Max;
        m_pScroll->GetScrollRangeWZ( &Min, &Max );

        if( ResultCode == -1 )
            m_pScroll->SetScrollPosWZ( Min );
        else if( ResultCode == 1 )
            m_pScroll->SetScrollPosWZ( Max );
    }

    UpdateControlState();

}

void uiOptionView::UpdateMouseWheel()
{
    //1.영역체크는 다이얼로그 전체사이즈로 하겠씀
    if(!GetDialogWZ() || !m_pScroll) return;

    RECT rc;
    GetDialogWZ()->GetClientRect(&rc);

    if( m_PopupItem != NULL )
    {
        switch(GetMouseWheelStatus(&rc))
        {
        case MOUSE_WHEEL_UP:
            {
                MoveComboScroll( -3 );
            }
            break;
        case MOUSE_WHEEL_DOWN:
            {
                MoveComboScroll( 3 );
            }
            break;
        }
    }
    else
    {
        switch(GetMouseWheelStatus(&rc))
        {
        case MOUSE_WHEEL_UP:
            {
                MoveScroll( -3 );
            }
            break;
        case MOUSE_WHEEL_DOWN:
            {
                MoveScroll( 3 );
            }
            break;
        }
    }
}

void uiOptionView::OnUpdateSolarDialog()
{
	UpdateMouseWheel();

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	POINT pt;
	pt.x = iMouseX;
	pt.y = iMouseY;

	if (GetDialogWZ()->InterSectRect(pt) == false)
	{
		Update_Counter_timer_.Off();
	}

	if(m_iArrayIndex == -1)
	{
		return;
	}

	int ScrollOffset = m_pScroll->GetScrollPosWZ();
	cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+m_iArrayIndex];
	cOptionGaugeItem* pItemGauge = NULL;
	
	if(pItem)
	{
		if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
		{
			pItemGauge = (cOptionGaugeItem*)pItem;
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	int MoverPos = cOptionGaugeItem::MOVE_MAX;
	if (pItemGauge && pItemGauge->GetClickMover(&MoverPos)  && Update_Counter_timer_.SpentTime() >1000 )
	{
		DWORD Value_Is_Item =  pItemGauge->GetGaugePosition();

		if(Value_Is_Item == 0)
		{
			return;
		}

		if(MoverPos == cOptionGaugeItem::MOVE_RIGHT)
		{
			pItemGauge->SetGaugePosition(Value_Is_Item+1);
		}
		else if(MoverPos == cOptionGaugeItem::MOVE_LEFT)
		{	
			pItemGauge->SetGaugePosition(Value_Is_Item-1);
		}

	}

    
}

cOptionItemBase* uiOptionView::PickItem()
{
    CControlWZ* pListControl = GetControlWZ(StrToWzID("L000"));
    if( pListControl == NULL )
        return NULL;

    RECT ListRect = pListControl->GetSizeRect();

    int ScrollOffset = m_pScroll->GetScrollPosWZ();

    RECT LineRect = ListRect;
    LineRect.bottom = LineRect.top + 24;

    for( size_t i=0;i!=m_VisibleItemArray.size();++i)
    {
        int index = i - ScrollOffset;

        if( index < 0 )
            continue;
        if( index >= kLineCount )
            continue;

        POINT pt;
        pt.x = Mouse::Instance()->GetMouseX();
        pt.y = Mouse::Instance()->GetMouseY();

        if( PtInRect( &LineRect, pt ) )
        {
            return m_VisibleItemArray[i];
        }

        LineRect.top += 24;
        LineRect.bottom += 24;
    }

    return NULL;

}

void uiOptionView::SetPopupItem( cOptionComboItem* pItem )
{
    m_PopupItem = pItem;

    m_PopupItemRect.clear();

    if( m_PopupItem == NULL )
    {
        m_pComboBackground->ShowWindowWZ(WZ_HIDE);
        m_pComboScroll->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        m_pComboBackground->ShowWindowWZ(WZ_SHOW);
    }

    if( m_PopupItem != NULL )
    {
        int ScrollOffset = m_pScroll->GetScrollPosWZ();
        int ItemIndex = -1;
        for( size_t i=0;i!=m_VisibleItemArray.size();++i)
        {
            int index = i - ScrollOffset;

            if( index < 0 )
                continue;
            if( index >= kLineCount )
                continue;

            if( m_PopupItem == m_VisibleItemArray[i] )
            {
                ItemIndex = index;
                break;
            }
        }

        if( ItemIndex != -1 )
        {
            int ItemOffsetX = 261;
            int ItemOffsetY = 3;
            int ItemWidth = 118;
            int ItemHeight = 18;

            RECT ItemRect;
            ItemRect.left = m_ViewRect.left + ItemOffsetX;
            ItemRect.top = m_ViewRect.top + ItemOffsetY + ItemIndex*24;
            ItemRect.right = ItemRect.left + ItemWidth;
            ItemRect.bottom = ItemRect.top + ItemHeight;
            ItemRect.top += 24;
            ItemRect.bottom += 24;

            if( m_PopupItem->GetItemCount() > 10 )
            {
                m_pComboScroll->SetScrollRangeWZ(0.0f, m_PopupItem->GetItemCount() - 5.0f );
                m_pComboScroll->SetScrollPosWZ( (float)(m_PopupItem->GetItemIndex() >= m_PopupItem->GetItemCount() - 5 ? m_PopupItem->GetItemCount() - 5 : m_PopupItem->GetItemIndex()));
                m_pComboScroll->ShowWindowWZ(WZ_SHOW);


                for( size_t i=0;i!=m_PopupItem->GetItemCount();++i)
                {
                    sPopupItem item;
                    item.m_Text = m_PopupItem->GetItemText(i);
                    item.m_Rect = ItemRect;
                    m_PopupItemRect.push_back(item);

                    ItemRect.top += 18;
                    ItemRect.bottom += 18;
                }


                RECT BackgroundRect;
                BackgroundRect.left = m_ViewRect.left + ItemOffsetX;
                BackgroundRect.top = m_ViewRect.top + ItemOffsetY + ItemIndex*24 + 24;
                BackgroundRect.right = BackgroundRect.left + ItemWidth;
                BackgroundRect.bottom = BackgroundRect.top + 5*18;

                RECT ScrollRect;
                ScrollRect.left = BackgroundRect.right;
                ScrollRect.right = ScrollRect.left + 10;
                ScrollRect.top = BackgroundRect.top;
                ScrollRect.bottom = BackgroundRect.bottom;

                m_pComboScroll->MoveWindowWZ( (float)ScrollRect.left, (float)ScrollRect.top, (float)ScrollRect.right - ScrollRect.left, (float)ScrollRect.bottom - ScrollRect.top );

                BackgroundRect.right += 10;

                static int xx = 5;
                static int yy = 5;
                InflateRect( &BackgroundRect, xx, yy );

                m_pComboBackground->MoveWindowWZ( (float)BackgroundRect.left, (float)BackgroundRect.top, (float)BackgroundRect.right - BackgroundRect.left, (float)BackgroundRect.bottom - BackgroundRect.top );
            }
            else
            {
                for( size_t i=0;i!=m_PopupItem->GetItemCount();++i)
                {
                    sPopupItem item;
                    item.m_Text = m_PopupItem->GetItemText(i);
                    item.m_Rect = ItemRect;
                    m_PopupItemRect.push_back(item);

                    ItemRect.top += 18;
                    ItemRect.bottom += 18;
                }


                RECT BackgroundRect;
                BackgroundRect.left = m_ViewRect.left + ItemOffsetX;
                BackgroundRect.top = m_ViewRect.top + ItemOffsetY + ItemIndex*24 + 24;
                BackgroundRect.right = BackgroundRect.left + ItemWidth;
                BackgroundRect.bottom = BackgroundRect.top + m_PopupItem->GetItemCount()*18;

                static int xx = 5;
                static int yy = 5;
                InflateRect( &BackgroundRect, xx, yy );

                m_pComboBackground->MoveWindowWZ( (float)BackgroundRect.left, (float)BackgroundRect.top, (float)BackgroundRect.right - BackgroundRect.left, (float)BackgroundRect.bottom - BackgroundRect.top );
            }
        }
    }
}

void uiOptionView::MessageProc( SI_MESSAGE * pMessage )
{
    uiBase::MessageProc( pMessage );

    if( m_PopupItem != NULL )
    {
        if( m_PopupItem->GetItemCount() > 10 )
        {
            if( pMessage->dwCtrlID == m_pComboScroll->GetCtrlID() )
            {
                if( pMessage->eResultMsg == RT_MSG_SB_LINETOP )
                    MoveComboScroll(-1);
                else if( pMessage->eResultMsg == RT_MSG_SB_LINEBOTTOM )
                    MoveComboScroll(1);
                else if( pMessage->eResultMsg == RT_MSG_SB_PAGETOP )
                    MoveComboScroll(-3);
                else if( pMessage->eResultMsg == RT_MSG_SB_PAGEBOTTOM )
                    MoveComboScroll(3);

                return;
            }

            switch( pMessage->eResultMsg )
            {
            case RT_MSG_LBUTTONCLICK:
                //case RT_MSG_LBUTTONDOWN:
                {
                    int ComboScrollOffset = m_pComboScroll->GetScrollPosWZ();

                    for( size_t i=0;i!=m_PopupItemRect.size();++i)
                    {
                        int VisibleIndex = i - ComboScrollOffset;
                        if( VisibleIndex < 0 )
                            continue;
                        if( VisibleIndex >= 5 )
                            continue;

                        POINT pt = Mouse::Instance()->GetMousePoint();
                        pt.y += 18*ComboScrollOffset;

                        if( PtInRect(&m_PopupItemRect[i].m_Rect, pt ) )
                        {
                            m_PopupItem->SetItemIndex(i);
                            SetPopupItem(NULL);
                            return;
                        }
                    }
                }

                SetPopupItem(NULL);
                return;

                //case RT_MSG_LBUTTONCLICK:
            case RT_MSG_RBUTTONCLICK:
            case RT_MSG_RBUTTONDOWN:
            case RT_MSG_EDIT_KEYDOWN:
                SetPopupItem(NULL);
                return;
            }
        }
        else
        {
            switch( pMessage->eResultMsg )
            {
            case RT_MSG_LBUTTONCLICK:
            //case RT_MSG_LBUTTONDOWN:
                {
                    POINT pt = Mouse::Instance()->GetMousePoint();
                    for( size_t i=0;i!=m_PopupItemRect.size();++i)
                    {
                        if( PtInRect(&m_PopupItemRect[i].m_Rect, pt ) )
                        {
                            m_PopupItem->SetItemIndex(i);
                            SetPopupItem(NULL);
                            return;
                        }
                    }
                }

                SetPopupItem(NULL);
                return;

            //case RT_MSG_LBUTTONCLICK:
            case RT_MSG_RBUTTONCLICK:
            case RT_MSG_RBUTTONDOWN:
            case RT_MSG_EDIT_KEYDOWN:
                SetPopupItem(NULL);
                return;
            }
        }

        return;
    }

    for( size_t i=0;i!=m_CheckButtonArray.size();++i)
    {
        if( m_CheckButtonArray[i]->GetCtrlID() == pMessage->dwCtrlID )
        {
            if( pMessage->eResultMsg == RT_MSG_LBUTTONCLICK )
            {
                if( i == 3 )
                {
                    uiOptionMan::ShowKeyBindingDialog();
                    return;
                }

                m_pScroll->SetScrollPosWZ(0.0f);
                SetPageIndex(i);
                UpdateControlState();
                return;
            }
        }
    }

    // 복구
    if( pMessage->dwCtrlID == StrToWzID("B043") && pMessage->eResultMsg == RT_MSG_LBUTTONCLICK )
    {
        if( cOptionListControl* pList = GetPageList() )
        {
            pList->RestoreOptionValue();
        }
        return;
    }

    // 바로 적용
    if( pMessage->dwCtrlID == StrToWzID("B042") && pMessage->eResultMsg == RT_MSG_LBUTTONCLICK )
    {
        if( cOptionListControl* pList = GetPageList() )
        {
            pList->ApplyOptionValue();
        }
        GAMEOPTION->ApplyOption();
        return;
    }

    // 확인
    if( pMessage->dwCtrlID == StrToWzID("B005") && pMessage->eResultMsg == RT_MSG_LBUTTONCLICK )
    {
        // 적용
        for( size_t i=0;i!=m_PageList.size();++i)
        {
            m_PageList[i]->ApplyOptionValue();
        }
        GAMEOPTION->ApplyOption();

        ShowInterfaceWithSound(FALSE);
        return;
    }

    // X닫기, 취소
    if( (pMessage->dwCtrlID == StrToWzID("B999") && pMessage->eResultMsg == RT_MSG_LBUTTONCLICK) ||
        (pMessage->dwCtrlID == StrToWzID("B006") && pMessage->eResultMsg == RT_MSG_LBUTTONCLICK)
        )
    {
        ShowInterfaceWithSound(FALSE);
        return;
    }

    if( pMessage->dwCtrlID == StrToWzID("VS02") )
    {
        if( pMessage->eResultMsg == RT_MSG_SB_LINETOP )
            MoveScroll(-1);
        else if( pMessage->eResultMsg == RT_MSG_SB_LINEBOTTOM )
            MoveScroll(1);
        else if( pMessage->eResultMsg == RT_MSG_SB_PAGETOP )
            MoveScroll(-3);
        else if( pMessage->eResultMsg == RT_MSG_SB_PAGEBOTTOM )
            MoveScroll(3);
        else if( pMessage->eResultMsg == RT_MSG_SB_ENDSCROLL )
            UpdateControlState();
        else if( pMessage->eResultMsg == RT_MSG_SB_THUMBPOSITION )
            UpdateControlState();
        else if( pMessage->eResultMsg == RT_MSG_SB_THUMBTRACK )
            UpdateControlState();
    }
    else if( pMessage->dwCtrlID == StrToWzID("L000") )
    {
        if( pMessage->eResultMsg == RT_MSG_LBUTTONDOWN )
        {
            if( cOptionItemBase* pItem = PickItem() )
            {
                if( pItem->GetType() == OPTION_ITEM_TYPE_CATEGORY )
                {
                    cOptionCategory* pCategory = (cOptionCategory*)pItem;
                    pCategory->SetExpend( pCategory->IsExpended() == false );
                    UpdateControlState();
                }
            }
        }
    }
	else if(pMessage->eResultMsg == RT_MSG_LBUTTONDOWN )
	{
		int ScrollOffset = m_pScroll->GetScrollPosWZ();
		for( size_t i=0;i!=m_ControlArray.size();++i)
		{
			if( ScrollOffset + i >= m_VisibleItemArray.size() )
				break;

			if( m_ControlArray[i]->m_Left == pMessage->dwCtrlID )
			{
				cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+i];

				if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
				{
					cOptionGaugeItem* pItemGauge = (cOptionGaugeItem*)pItem;
					pItemGauge->SetClickMover(true,cOptionGaugeItem::MOVE_LEFT);
					m_iArrayIndex = i;

					Update_Counter_timer_.Off();
					Update_Counter_timer_.On(c_AnimationFullTime);
					pItem->SetPrev();
				}

				
			}
			else if( m_ControlArray[i]->m_Right == pMessage->dwCtrlID )
			{
				cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+i];

				if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
				{
					cOptionGaugeItem* pItemGauge = (cOptionGaugeItem*)pItem;
					pItemGauge->SetClickMover(true,cOptionGaugeItem::MOVE_RIGHT);
					m_iArrayIndex = i;

					Update_Counter_timer_.Off();
					Update_Counter_timer_.On(c_AnimationFullTime);
					pItem->SetNext();
				}
			}
		}
	}
    else if( pMessage->eResultMsg == RT_MSG_LBUTTONCLICK )
    {
        int ScrollOffset = m_pScroll->GetScrollPosWZ();
        for( size_t i=0;i!=m_ControlArray.size();++i)
        {
            if( ScrollOffset + i >= m_VisibleItemArray.size() )
                break;

            if( m_ControlArray[i]->m_Left == pMessage->dwCtrlID )
            {
                cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+i];

				if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
				{
					Update_Counter_timer_.Off();
					m_iArrayIndex = -1;
					return;
				}

                pItem->SetPrev();
            }
            else if( m_ControlArray[i]->m_Right == pMessage->dwCtrlID )
            {
                cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+i];
				
				if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
				{
					Update_Counter_timer_.Off();
					m_iArrayIndex = -1;
					return;
				}

                pItem->SetNext();
            }
            else if( m_ControlArray[i]->m_Center == pMessage->dwCtrlID )
            {
                if( m_VisibleItemArray[ScrollOffset+i]->GetType() == OPTION_ITEM_TYPE_ITEM )
                {
                    cOptionComboItem* pItem = (cOptionComboItem*)m_VisibleItemArray[ScrollOffset+i];
                    SetPopupItem( pItem );
                }
                else if( m_VisibleItemArray[ScrollOffset+i]->GetType() == OPTION_ITEM_TYPE_GAUGE )
                {
                    cOptionGaugeItem* pItem = (cOptionGaugeItem*)m_VisibleItemArray[ScrollOffset+i];
                    if( CControlWZ* pControl = GetControlWZ(pMessage->dwCtrlID) )
                    {
                        RECT ControlRect = pControl->GetSizeRect();
                        int MouseX = Mouse::Instance()->GetMouseX() - ControlRect.left;
                        if( MouseX >= 0 && MouseX < 118 )
                        {
                            float Range = (float)pItem->GetGaugeRange();
                            float tumb = 118.0f / Range;
                            float index = ((float)MouseX + tumb * 0.5f) / tumb;
                            pItem->SetGaugePosition((DWORD)index);
                        }
                    }
                }
            }
        }
    }
	else if( pMessage->eResultMsg == RT_MSG_LBUTTONUP )
	{
		if(m_iArrayIndex == -1)
		{
			return;
		}

		int ScrollOffset = m_pScroll->GetScrollPosWZ();
		cOptionItem* pItem = (cOptionItem*)m_VisibleItemArray[ScrollOffset+m_iArrayIndex];

		if(pItem)
		{
			if(pItem->GetType() == OPTION_ITEM_TYPE_GAUGE)
			{
				cOptionGaugeItem* pItemGauge = (cOptionGaugeItem*)pItem;
				pItemGauge->SetClickMover(false,cOptionGaugeItem::MOVE_NONE);
				Update_Counter_timer_.Off();
				m_iArrayIndex = -1;
			}
		}
	}
}


void uiOptionView::InitializeOptionView()
{
    // 
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B000"), StrToWzID("P003"), StrToWzID("B001") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B002"), StrToWzID("P064"), StrToWzID("B003") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B004"), StrToWzID("P065"), StrToWzID("B007") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B008"), StrToWzID("P066"), StrToWzID("B009") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B010"), StrToWzID("P067"), StrToWzID("B011") ) );

    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B012"), StrToWzID("P068"), StrToWzID("B013") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B014"), StrToWzID("P069"), StrToWzID("B015") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B016"), StrToWzID("P070"), StrToWzID("B017") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B018"), StrToWzID("P071"), StrToWzID("B019") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B020"), StrToWzID("P072"), StrToWzID("B021") ) );

    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B022"), StrToWzID("P073"), StrToWzID("B023") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B024"), StrToWzID("P074"), StrToWzID("B025") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B026"), StrToWzID("P075"), StrToWzID("B027") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B028"), StrToWzID("P076"), StrToWzID("B029") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B030"), StrToWzID("P077"), StrToWzID("B031") ) );

    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B032"), StrToWzID("P078"), StrToWzID("B033") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B034"), StrToWzID("P079"), StrToWzID("B035") ) );
    m_ControlArray.push_back( new sOptionControlItem( StrToWzID("B036"), StrToWzID("P080"), StrToWzID("B037") ) );

    for( size_t i=0;i!=m_ControlArray.size();++i)
    {
        CCtrlPictureWZ* pPicture = (CCtrlPictureWZ*)GetControlWZ(m_ControlArray[i]->m_Center);
        pPicture->EnableLButtonEvent(true);
    }

    //
    m_pScroll = (CCtrlVScrollWZ*)GetControlWZ(StrToWzID("VS02"));
    m_pScroll->SetScrollPosWZ(0.0f);

    //
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C001") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C002") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C003") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C004") ) );


    //
    for( size_t i=0;i!=4;++i)
    {
        cOptionListControl* pOptionListControl = new cOptionListControl();
        pOptionListControl->LoadPage(i);
        m_PageList.push_back( pOptionListControl );
    }
    SetPageIndex(0);


    m_pComboScroll = (CCtrlVScrollWZ*)GetControlWZ(StrToWzID("V003"));
    m_pComboScroll->SetScrollPosWZ(0.0f);

    m_pComboBackground = GetControlWZ(StrToWzID("P006"));

    //m_pComboBackground->SetDrawState(WZ_CUSTOM_DRAW);
    //m_pComboScroll->SetDrawState(WZ_CUSTOM_DRAW);

    SetPopupItem(NULL);
}


void uiOptionView::UpdateControlState()
{
    for( size_t i=0;i!=m_CheckButtonArray.size();++i)
    {
        m_CheckButtonArray[i]->SetCheckState( m_PageIndex == i );
    }

    if( cOptionListControl* pList = GetPageList() )
    {
        // UpdateVisibleItemArray
        pList->UpdateVisibleItemArray( m_VisibleItemArray );

        if( m_VisibleItemArray.size() > kLineCount )
        {
            m_pScroll->SetScrollRangeWZ( 0.0f, (float)m_VisibleItemArray.size() - kLineCount);
        }
        else
        {
            m_pScroll->SetScrollRangeWZ( 0.0f, 0.0f );
        }

        // Mapping item to control
        int ScrollOffset = m_pScroll->GetScrollPosWZ();

        for( size_t i=0;i!=kLineCount;++i)
        {
            int index = ScrollOffset + i;
            if( (size_t)index >= m_VisibleItemArray.size() )
            {
                GetControlWZ(m_ControlArray[i]->m_Left)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ(m_ControlArray[i]->m_Center)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ(m_ControlArray[i]->m_Right)->ShowWindowWZ(WZ_HIDE);
                continue;;
            }

            if( m_VisibleItemArray[index]->GetType() == OPTION_ITEM_TYPE_CATEGORY )
            {
                GetControlWZ(m_ControlArray[i]->m_Left)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ(m_ControlArray[i]->m_Center)->ShowWindowWZ(WZ_HIDE);
                GetControlWZ(m_ControlArray[i]->m_Right)->ShowWindowWZ(WZ_HIDE);
                continue;;
            }

            GetControlWZ(m_ControlArray[i]->m_Left)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ(m_ControlArray[i]->m_Center)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ(m_ControlArray[i]->m_Right)->ShowWindowWZ(WZ_SHOW);
        }
    }
}

void uiOptionView::Init( CDrawBase * pDrawBase )
{
	//CControlWZ* GetKeyMapTap = getControl( eKEYMAP_CHECK_C004 );

	//if(GetKeyMapTap)
	//{
	//	GetKeyMapTap->EnableWindowWZ(FALSE);
	//}
}

CControlWZ * uiOptionView::getControl( POSTYPE AtPos )
{
	assert( AtPos < eOPTION_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE uiOptionView::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eOPTION_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return eOPTION_MAX;
}

VOID uiOptionView::Release()
{
	for (size_t i = 0 ; i < m_PageList.size() ; ++i)
	{
		delete m_PageList[i];
		m_PageList[i] = NULL;
	}
	m_PageList.clear();

	for( size_t i=0;i < m_ControlArray.size();++i)
	{
		delete m_ControlArray[i];
		m_ControlArray[i] = NULL;
	}
	m_ControlArray.clear();
}