#include "SunClientPrecompiledHeader.h"
#include "Interface/uiItemTotalComposite/uiItemTotalComposite_def.h"
#include "Interface/uiItemTotalComposite/uiItemTotalComposite.h"
#include "ItemComposeListParser.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "CursorChangeTransact.h"
#include "Hero.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemTypeList.h"


const int c_AnimationFullTime = 1;

WzID uiItemTotalcomposite::m_wzId[DIALOG_MAX] =
{
	StrToWzID("P101"),	  //DIALOG_PIC_SLOT1,
	StrToWzID("P102"),	  //DIALOG_PIC_SLOT2,
	StrToWzID("P103"),	  //DIALOG_PIC_SLOT3,
	StrToWzID("P104"),	  //DIALOG_PIC_SLOT4,
	StrToWzID("P105"),	  //DIALOG_PIC_SLOT5,
	StrToWzID("P106"),	  //DIALOG_PIC_SLOT6,
	StrToWzID("P100"),	  //DIALOG_PIC_RESULT,
	StrToWzID("T100"),	  //DIALOG_TXT_CAPTION = 0,
	StrToWzID("T101"),    //DIALOG_TXT_CUR_CATEGORY,	
	StrToWzID("T110"),	  //DIALOG_TXT_TOTAL_NUM,
	StrToWzID("T112"),	  //DIALOG_TXT_MANUAL_NUM,
	StrToWzID("T114"),    //DIALOG_TXT_CHARGE,
	StrToWzID("T103"),	  //DIALOG_TXT_RESULT_ITEM,
	StrToWzID("T104"),	  //DIALOG_TXT_SLOT1,
	StrToWzID("T105"),	  //DIALOG_TXT_SLOT2,
	StrToWzID("T106"),	  //DIALOG_TXT_SLOT3,
	StrToWzID("T107"),	  //DIALOG_TXT_SLOT4,
	StrToWzID("T108"),	  //DIALOG_TXT_SLOT5,w
	StrToWzID("T109"),	  //DIALOG_TXT_SLOT6,
    StrToWzID("S000"),	  //DIALOG_TXT_SLOT7,
	StrToWzID("B100"),	  //DIALOG_BTN_EXIT,
	StrToWzID("B101"),	  //DIALOG_BTN_LIST_ON,
	StrToWzID("B102"),	  //DIALOG_BTN_ALL_CREATE,
	StrToWzID("B105"),	  //DIALOG_BTN_MANUAL_CREATE,
	StrToWzID("B103"),	  //DIALOG_BTN_LEFT,
	StrToWzID("B104"),	  //DIALOG_BTN_RIGHT,
	StrToWzID("B106"),	  //DIALOG_BTN_CANCLE	
	StrToWzID("VS00"),	  //DIALOG_SCROLBAR,
	StrToWzID("L100"),	  //DIALOG_LIST_EACH_CATEGORY,
	StrToWzID("L102"),	  //DIALOG_LIST_CATEGORY,
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("P008"),	  //,
	StrToWzID("V001"),	  //,
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
						  //DIALOG_MAX,
};


//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::OnUpdateSolarDialog()
{
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    if(m_pList_Category->GetState() == WZ_HIDE)
    {
         Tree_Controller_Ptr_->update();
    }
#endif

	if ( this->m_pTotalItemUnitRenderer )
	{
		this->m_pTotalItemUnitRenderer->Update();	
	}

	bool isCounterPick = false;

	if ( is_click_Btn_ && Update_Counter_timer_.SpentTime() >1000 )
	{
		isCounterPick = true;
	}

	if (isCounterPick && is_Set_Plus_Min_ != eValue_None)
	{
		int Value_Is_Item =  GetManager()->GetManualCompositeNum();

		if(Value_Is_Item <  GetManager()->GetMaxCompositeNum() && is_Set_Plus_Min_ == eValue_Plus)
		{
			GetManager()->SetManualCompositeNum(GetManager()->GetManualCompositeNum() + 1);
			// FALSE 이면 조합가능한 갯수값을 변경하지 않고 유지.
			updateInfo(FALSE);
		}

        // 시간을 주어서 클릭하는 것을 1보다 클 때로 해야 0으로 내려가지 않는다.		
        if(Value_Is_Item> 1  && is_Set_Plus_Min_ == eValue_Minus)
        {
            GetManager()->SetManualCompositeNum(GetManager()->GetManualCompositeNum() -1);
            // FALSE 이면 조합가능한 갯수값을 변경하지 않고 유지.
            updateInfo(FALSE);
        }

	}
	updateMouseWheel();
}


void _callback_CompositeCreate(bool bYes)
{
	if(bYes)
	{
		uiItemCompositeMan * pCompositeMan = ( uiItemCompositeMan * )
								g_InterfaceManager.GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE);

		if( pCompositeMan)
		{
			pCompositeMan->NET_SEND_CG_TOTAL_ITEM_COMPOSITE_SYN();
		}

		if(g_pHero)
			g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		// 5718	아이템 조합을 취소했습니다.
		g_InterfaceManager.GetInterfaceString(eST_CANCLE_ITEM_COMPOSITE , szMessage , INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
		{
			pSystemMan->Sys_Verify(szMessage);
		}
		if(g_pHero)
		{
			g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::MessageProc( SI_MESSAGE * pMessage )
{
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    if(m_pList_Category->GetState() == WZ_HIDE)
    {
        Tree_Controller_Ptr_->MessageProc(pMessage);
    }
#endif
    
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	switch(pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONUP:
		{
			Update_Counter_timer_.Off();
			is_click_Btn_ = false;
			is_Set_Plus_Min_ = eValue_None;
		}
		break;
	case RT_MSG_LBUTTONDOWN:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case DIALOG_BTN_LEFT:
				{
					Update_Counter_timer_.Off();
					Update_Counter_timer_.On(c_AnimationFullTime);

					if ( GetManager()->IsPKPending() )
					{
						return;
					}

					if(GetManager()->GetCompositeResultType() != sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE)
					{
						is_click_Btn_ = true;
						is_Set_Plus_Min_ = eValue_Minus;
					}

				}
				break;
			case DIALOG_BTN_RIGHT:
				{
					Update_Counter_timer_.Off();
					Update_Counter_timer_.On(c_AnimationFullTime);

					if ( GetManager()->IsPKPending() )
					{
						return;
					}
					
					if(GetManager()->GetCompositeResultType() != sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE)
					{
						is_click_Btn_ = true;
						is_Set_Plus_Min_ = eValue_Plus;
					}
					
				}
				break;
			}
		}
		break;
	case RT_MSG_LBUTTONCLICK:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case DIALOG_BTN_LIST_ON:
				{
					ENUM_STATEWZ eState = m_pList_Category->GetState() == WZ_SHOW ? WZ_HIDE : WZ_SHOW;
					m_pList_Category->ShowWindowWZ(eState);
#ifdef _NA_000000_20130114_RENEWER_UI
					UpdateTree2();

					m_pImageDrapDown->ShowWindowWZ(eState);
					m_pVScrollDrapDown->ShowWindowWZ(eState);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

				}
				break;
			case DIALOG_LIST_CATEGORY:
				{


#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
                    ClearInfo();
                    UpdateTreeReViewer();
#else
                    ClearInfo();
                    UpdateTree();
#endif
					m_pList_Category->ShowWindowWZ(WZ_HIDE);
#ifdef _NA_000000_20130114_RENEWER_UI
					m_pImageDrapDown->ShowWindowWZ(WZ_HIDE);
					m_pVScrollDrapDown->ShowWindowWZ(WZ_HIDE);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
					//셋팅된 아이템 코드가 올바르면모두 업데이트.
					if(SetCurrentSelectedItemCode()) 
					{
						updateAllInfomation();
					}
				}	
				break;
			case DIALOG_LIST_EACH_CATEGORY:
				{
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#endif
					ClearInfo();
					//셋팅된 아이템 코드가 올바르면 모두 업데이트.
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
                    HTREEITEM SelItem = Tree_Controller_Ptr_->GetSelItem();
                    TreeCategory* item = NULL;
                    if(SelItem)
                    {
                       item = (TreeCategory*)SelItem->tree_data.user_data_Ptr_;
                    }
            
                    if(item && item->isMaxDepth())
                    {
                         m_iCurSelectedItemCode = item->CompositionCode; 
                    }
                   
					if(m_iCurSelectedItemCode > 0)
					{
						updateAllInfomation();
					}
#else
					if(SetCurrentSelectedItemCode())
					{
						updateAllInfomation();
					}
#endif
				}
				break;
			case DIALOG_BTN_LEFT:
				{
					
					if ( GetManager()->IsPKPending() )
					{
						return;
					}

					if(Update_Counter_timer_.SpentTime() < 1000)
					{

						if(GetManager()->GetCompositeResultType() == sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE)
						{
							// 보상 아이템은 1개만 생성가능합니다. 5371
							g_InterfaceManager.GetInterfaceString(eST_BOSANG_ITEM_CREATE_ONLY_ONE , szMessage ,INTERFACE_STRING_LENGTH );
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage );
						}
                        // Max 개수가 255개인 상태에서 ManualCompsiteNum이 0일 때 255개 보다 작게 되므로 이 안으로 들어와서 -를 양산한다.						
                        if(GetManager()->GetManualCompositeNum() > 1)
                        {
                            //기본 상태 +1;
                            GetManager()->SetManualCompositeNum(GetManager()->GetManualCompositeNum() -1);
                            // FALSE 이면 조합가능한 갯수값을 변경하지 않고 유지.
                            updateInfo(FALSE);
                        }
					}

					Update_Counter_timer_.Off();
					is_click_Btn_ = false;
					is_Set_Plus_Min_ = eValue_None;

				}
				break;
			case DIALOG_BTN_RIGHT:
				{
					

					if ( GetManager()->IsPKPending() )
					{
						return;
					}

					if(Update_Counter_timer_.SpentTime() < 1000)
					{
						if(GetManager()->GetCompositeResultType() == sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE)
						{
							// 보상 아이템은 1개만 생성가능합니다. 5371
							g_InterfaceManager.GetInterfaceString(eST_BOSANG_ITEM_CREATE_ONLY_ONE , szMessage ,INTERFACE_STRING_LENGTH );
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage );
						}
						if(GetManager()->GetManualCompositeNum() <  GetManager()->GetMaxCompositeNum())
						{
							//기본 상태 +1;
							GetManager()->SetManualCompositeNum(GetManager()->GetManualCompositeNum() +1);
							// FALSE 이면 조합가능한 갯수값을 변경하지 않고 유지.
							updateInfo(FALSE);
						}
					}

					Update_Counter_timer_.Off();
					is_click_Btn_ = false;
					is_Set_Plus_Min_ = eValue_None;
				}
				break;
			case DIALOG_BTN_MANUAL_CREATE:
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					if ( GetManager()->IsPKPending() )
					{
						return;
					}

					if(GetManager()->IsReadyForTotalCompositeMaterials())
					{
						sITEM_COMPOSE_LIST * pResultInfo = ItemComposeListParser::Instance()->GetComposeListInfo(m_iCurSelectedItemCode);
						if(! pResultInfo)
							return;

						BOOL bSuccessOnly = TRUE;
						_COMPOSELIST_HASH * phashTable = ItemComposeListParser::Instance()->GetHashTable();
						if( ! phashTable )
							return;
						sITEM_COMPOSE_LIST * pCompositeList = phashTable->GetData( pResultInfo->m_Code);
						if( !pCompositeList )
							return;
						if( pCompositeList->m_byCreateRatio != 100)
						{
							bSuccessOnly = FALSE;
						}

						if( bSuccessOnly )
						{
							TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,} , szItemName[128] = {0,};
							g_InterfaceManager.GetInterfaceString(pResultInfo->m_NCode,szItemName,128);
							// 5721	[%s] %d개를 생성하시겠습니까?
							g_InterfaceManager.GetInterfaceString(eST_IS_CREATE_NUM_ITEM , szTemp ,INTERFACE_STRING_LENGTH);
					#ifdef _INTERNATIONAL_UI
							Sprintf( szMessage,szTemp,GetManager()->GetManualCompositeNum(),szItemName);
					#else
							Sprintf( szMessage,szTemp,szItemName,GetManager()->GetManualCompositeNum());
					#endif//_INTERNATIONAL_UI(어순문제로 순서바꿈)
							
						}
						else
						{
							g_InterfaceManager.GetInterfaceString(5723 , szMessage ,INTERFACE_STRING_LENGTH );
						}


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_ConfirmLock(szMessage, _callback_CompositeCreate);

						if(g_pHero)
							g_pHero->SetBeHaveState(PLAYER_BEHAVE_SYSTEM_LOCK);

					}				
				}
				break;
			case DIALOG_BTN_CANCLE:
			case DIALOG_BTN_EXIT:
				{
					ClearInfo();
					this->ShowInterfaceWithSound(FALSE);
				}
				break;
			}
		}
		break; 

	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_LINETOP:
		{
#ifdef _NA_000000_20130114_RENEWER_UI
			bool isVScrollClick = false;

			if (m_pVScrollDrapDown && m_pVScrollDrapDown->GetState() == WZ_SHOW)
			{
				int pos = m_pVScrollDrapDown->GetScrollPosWZ();
				int mmin = 0 , mmax = 0;
				m_pVScrollDrapDown->GetScrollRangeWZ( &mmin, &mmax );

				if (pos > mmin)// 현재 입력된 라인수 보다 작으면 
				{
					--pos;//한줄 감소 시키고 
					m_pVScrollDrapDown->SetScrollPosWZ(pos);

					if( m_pList_Category->GetListRowCnt() > MAX_LIST_DATA)
					{
						// 위로 올릴수 있는 상태인가?
						if ( m_iDropdownCurScrollValue > 0 )
						{
							m_iDropdownCurScrollValue -= 1;
							m_pVScrollDrapDown->SetScrollPosWZ( m_iDropdownCurScrollValue );
							m_pList_Category->SetBeginDrawIndex(m_iDropdownCurScrollValue);
						}
					}
				}

				isVScrollClick = true;
			}

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#else
			if (m_pVScroll && isVScrollClick == false)
			{
				int pos = m_pVScroll->GetScrollPosWZ();
				int mmin = 0 , mmax = 0;
				m_pVScroll->GetScrollRangeWZ( &mmin, &mmax );

				if (pos > mmin)// 현재 입력된 라인수 보다 작으면 
				{
					--pos;//한줄 감소 시키고 
					m_pVScroll->SetScrollPosWZ(pos);
					DecreaseListCategoryScroll();
				}
			}
#endif
#else
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{

			case DIALOG_SCROLBAR:
				{
					DecreaseListCategoryScroll();
				}
				break;
			 }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

			

		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
#ifdef _NA_000000_20130114_RENEWER_UI
			bool isVScrollClick = false;

			if (m_pVScrollDrapDown && m_pVScrollDrapDown->GetState() == WZ_SHOW)
			{
				int pos = m_pVScrollDrapDown->GetScrollPosWZ();
				int mmin = 0 , mmax = 0;
				m_pVScrollDrapDown->GetScrollRangeWZ( &mmin, &mmax );

				if (pos < mmax)// 현재 입력된 라인수 보다 작으면 
				{
					++pos;//한줄 감소 시키고 
					m_pVScrollDrapDown->SetScrollPosWZ(pos);

					if( m_pList_Category->GetListRowCnt() > MAX_LIST_DATA)
					{
						// 아래로 내릴수 있는 상태인가?
						if ( m_iDropdownCurScrollValue < m_iDropdownMaxScrollValue )
						{
							m_iDropdownCurScrollValue += 1;
							m_pVScrollDrapDown->SetScrollPosWZ( m_iDropdownCurScrollValue );
							m_pList_Category->SetBeginDrawIndex(m_iDropdownCurScrollValue);
						}
					}
				}

				isVScrollClick = true;
			}

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#else
			if (m_pVScroll && isVScrollClick == false)
			{
				int pos = m_pVScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pVScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if (pos < mmax)// 맥스값 보다 작으면 
				{
					++pos;//한줄 증가 시키고 
					m_pVScroll->SetScrollPosWZ(pos);
					IncreaseListCategoryScroll();
				}
				
			}
#endif
#else
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case DIALOG_SCROLBAR:
				{
					IncreaseListCategoryScroll();
				}
				break;
			}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

			

		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
	case RT_MSG_SB_THUMBPOSITION:
		{
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#endif
			switch( getCtrlIDToPos( pMessage->dwCtrlID ))
			{
#ifndef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER // if'N'def
			case DIALOG_SCROLBAR:
				{
					if( m_pList_Category && m_pList_Category->GetState() != WZ_SHOW)
					{
						m_iCurScrollValue = m_pVScroll->GetScrollPosWZ();
						m_pList_Each_Category->SetBeginDrawIndex( m_iCurScrollValue );
					}		
				}
				break;
#endif
#ifdef _NA_000000_20130114_RENEWER_UI
			case DIALOG_SCROLBAR_DRAPDOWN:
				{
					if( m_pList_Category && m_pList_Category->GetState() == WZ_SHOW)
					{
						m_iDropdownCurScrollValue = m_pVScrollDrapDown->GetScrollPosWZ();
						m_pList_Category->SetBeginDrawIndex( m_iDropdownCurScrollValue );
					}
				}
				break;
#endif
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

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
uiItemTotalcomposite::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= 0 && pos < SLOT_SIZE)
			{
				if (!IsEmpty( pos ))
				{
					SCSlot & rSlot = GetSlot( pos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
				}
				else
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
			else
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}			
		}
		break;
	}		
}

//------------------------------------------------------------------------------
/** 중국 엘리트 1.5차_2nd작업
*/
void uiItemTotalcomposite::SetItemSubInfo()
{
	 POSTYPE pos  = m_MouseOverObject.pSlot->GetPos();
	
	//슬롯범위를 벗어나지못하게끔
	if(pos>sITEM_COMPOSE_LIST::MAX_COMPOSE_ITEM_NUM) return;
	
	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( m_iCurSelectedItemCode );
	
	//조합정보 없으면 리턴!
	if(!pInfo) return; 

	DWORD dwItemTypeCode = (pos==sITEM_COMPOSE_LIST::MAX_COMPOSE_ITEM_NUM ? (pInfo->m_Result.m_ItemTypeIndex):(pInfo->m_pMaterial[pos].m_ItemTypeIndex));

	ItemType * pItemType = ItemTypeList::Instance()->FindItemType( dwItemTypeCode );
	
	//아이템타입리스트 없으면 리턴!
	if(!pItemType) return; 

	SCItemSlot* pMouseOverItem = (SCItemSlot*)m_MouseOverObject.pSlot;
	
	//인챈정보
	BYTE byEnchant = pItemType->GetEnchant();
	pMouseOverItem->SetEnchant(byEnchant);

    nsSlot::ItemTypeChanger::ChangeItemByType(pMouseOverItem, pItemType->GetItemType());
	

	//랭크정보
	BYTE byRankInfo = pItemType->GetRank();
	pMouseOverItem->SetRank((eRANK)byRankInfo);
}

//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::OnRenderSolarDialog()
{
	if ( this->m_pTotalItemUnitRenderer )
	{
		this->m_pTotalItemUnitRenderer->Render();		
	}

    uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			if(pUIMan)
			{
				SetItemSubInfo();
							
				pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	POINT pt;
	pt.x = iMouseX;
	pt.y = iMouseY;

	if (GetDialogWZ()->InterSectRect(pt) == false)
	{
		Update_Counter_timer_.Off();
		is_click_Btn_ = false;
		is_Set_Plus_Min_ = eValue_None;
	}

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = SLOT_BEGIN; a < SLOT_SIZE + SLOT_BEGIN; a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}

    #ifdef _SCJ_LONG_TEXT_REDUCING
    if (pUIMan != NULL)
    {
        RECT control_rect;
        MaterialNameList::iterator found_itr = material_name_tooltip_list_.begin();
        MaterialNameList::iterator end_itr = material_name_tooltip_list_.end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            CControlWZ* control_ptr = GetControlWZ(found_itr->first);
            if (control_ptr == NULL)
            {
                continue;
            }

            control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX)
            {
                if (control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
                {
                    pUIMan->RegisterTooltipSimpleText(const_cast<TCHAR*>(found_itr->second.c_str()));
					pUIMan->SetTooltipDialogID(GetDialogKey());
                    break;
                }
            }
        }
    }    
    #endif //_SCJ_LONG_TEXT_REDUCING

}

//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::OnShowWindow( BOOL val )
{

	if(val)
	{
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_TotalComposite;
		msg.wParam=InterfaceManager::DIALOG_ITEM_TOTAL_COMPOSITE;
		msg.DoSomething=GlobalFunc::DoTotalComposite;
		g_KeyQueueManager.PushBack(msg);

		m_pList_Category->ShowWindowWZ(WZ_HIDE);

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        UpdateTreeReViewer();
#else
        UpdateTree();	
#endif
		
		UpdateTree2();
	}
	else
	{
		is_click_Btn_ = false;
		is_Set_Plus_Min_ = eValue_None;

		this->ClearInfo();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_TotalComposite);
	}
}
