//---------------------------------------------------------------------------
// StyleContainer
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "stylecontainer.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "QuickSlot.h"
#include "ItemManager.h"
#include "InventoryDialog.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"
#include "HeroActionInput.h"
#include "mouse.h"
#include "interfaceManager.h"
#include "StyleSlotContainer.h"
#include "StyleContainer.h"
#include "StyleSlot.h"
#include "QuickContainer.h"
#include "CursorChangeTransact.h"
#include "uiToolTipMan/uiToolTipMan.h"
//----------------------------------------------------------------------------
/**
*/
WzID StyleContainer::m_wzId[STYLE_QUICK_MAX] = 
{ 
	StrToWzID("B001"),
	StrToWzID("B002"),
	StrToWzID("B003"),
	StrToWzID("B004"),
};


//----------------------------------------------------------------------------
/**
*/
StyleContainer::StyleContainer() : m_TabNum(0), m_pItemUnitRenderer(NULL)
{
}


//----------------------------------------------------------------------------
/**
*/
StyleContainer::~StyleContainer()
{
	assert( NULL == m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::OnUpdateSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();
}


//----------------------------------------------------------------------------
/**
*/
VOID StyleContainer::NetworkProc( MSG_BASE * pMsg )
{

}

//----------------------------------------------------------------------------
/**
*/
VOID			
StyleContainer::OnShowWindow(BOOL val)
{
	if (val)
	{
		
	}
	else
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::OnRenderSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();

	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = STYLE_QUICK_BT00; a < STYLE_QUICK_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}

	// render select style
	CControlWZ * pCurSelectStyle = NULL;
	for (int a = STYLE_QUICK_BT00; a < STYLE_QUICK_MAX; a++)
	{
		if ( IsEmpty((POSTYPE)a) )
		{
			continue;
		}

		if ( MouseHandler::Instance()->GetMouseItem()->m_pFromSlot && 
			 g_pHero->GetCurrentAttackStyle() == MouseHandler::Instance()->GetMouseItem()->m_pFromSlot->GetCode() )
		{
			continue;
		}

		StyleSlot & rSlot	= (StyleSlot  &)GetSlot( (POSTYPE)a );
		if (g_pHero && 
			g_pHero->GetCurrentAttackStyle() == rSlot.GetOrgCode())
		{		
			pCurSelectStyle		= getControl((POSTYPE)a);
			if(pCurSelectStyle)
			{
				m_rcSelectCurStyle	= pCurSelectStyle->GetSizeRect();
				g_InterfaceManager.RenderActivateMark( m_rcSelectCurStyle.left - 13, m_rcSelectCurStyle.top - 13);
			}
		}
	}

}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::Init( CDrawBase * pDrawBase )
{
	// 창 위치 조정
	RECT rcDialog;

	QuickContainer * pQuickDlg = (QuickContainer *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_QUICK);
	if (pQuickDlg)
	{
		pQuickDlg->GetDialogWZ()->GetDlgSize(&rcDialog);

		MoveWindowWZ( (float)rcDialog.left + rcDialog.right+2, (float)717.f);
	}

	StyleSlotContainer::Init(MAX_STYLE_SLOT_NUM);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::InitItemRender()
{

	if (m_pItemUnitRenderer) 
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, GetMaxSlotNum() );
	m_pItemUnitRenderer->ShowCooldown(TRUE);
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
StyleContainer::IsExistSameSerialItem(DBSERIAL Serial)
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		QuickSlot & rQuickSlot = (QuickSlot &)GetSlot(i);

		if ( rQuickSlot.GetSerial() == Serial )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
StyleContainer::IsExistSameCodeItem( CODETYPE ItemCode )
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		StyleSlot & rStyleSlot = (StyleSlot &) GetSlot(i);
		
		if (rStyleSlot.GetOrgCode())
		{ 
			// 링크가 되어있다면

			if ( rStyleSlot.GetOrgCode() == ItemCode )
			{
				return TRUE;
			}
		}
	}

	return FALSE;

}


/**	!! StyleSlot 만 Insert한다.
*/
RC::eSLOT_INSERT_RESULT		StyleContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = StyleSlotContainer::InsertSlot( AtPos, rSlot );
	
	StyleSlot & rStyleSlot = (StyleSlot &)(GetSlot(AtPos));

	CControlWZ * pCtrl = getControl( AtPos );
	if(!pCtrl)
		return RC::RC_INSERT_SLOT_FAILED;

	RECT rect = pCtrl->GetSizeRect();

	assert(  ST_STYLE_QUICK == rSlot.GetSlotType() );

	StyleSlot & rFromStyleSlot = (StyleSlot &)rSlot;

	// 퀵슬롯은 껍데기만 있는놈들이라 실제 슬롯을 묻혀준다.
	assert(rStyleSlot.GetOrgSlot());
	rStyleSlot.SetOrgSlot( rFromStyleSlot.GetOrgSlot() );

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( 
			rStyleSlot.GetOrgCode(),			// 스타일은 코드로 시리얼을 대신한다.
			rStyleSlot.GetOrgCode(), 
			&rect, 
			rStyleSlot.GetOrgSlot() ,pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	return eInsertSlotResult;
}

//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::getControlPos( POSTYPE AtPos, RECT & rectOut )
{
	CControlWZ * pCtrl = getControl( AtPos );
	rectOut = pCtrl->GetSizeRect();
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
StyleContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	// 소스 퀵 포지션 초기화
	StyleSlot & rSlot = (StyleSlot &)GetSlot(AtPos);

	if (!IsEmpty(AtPos))
	{	
		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetOrgCode() );
		}
		else
		{
			assert(m_pItemUnitRenderer);
		}
	}

	return StyleSlotContainer::DeleteSlot( AtPos, pSlotOut );
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
StyleContainer::getControl( POSTYPE AtPos )
{
	assert( AtPos < STYLE_QUICK_MAX );
	return GetControlWZ( m_wzId[ AtPos] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
StyleContainer::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < STYLE_QUICK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return STYLE_QUICK_MAX;
}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer * 
StyleContainer::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos >= STYLE_QUICK_BT00  && pos < STYLE_QUICK_MAX ) 
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				if(pControl)
				{
					RECT rcSlot = pControl->GetSizeRect();
					MouseHandler::Instance()->ItemTransaction(SI_STYLE, pos, &rcSlot);
				}
			}
		}
		break;

	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos >= STYLE_QUICK_MAX ) return;

			CControlWZ * pControl = getControl(pos);
			assert(pControl);

			if(pControl)
			{
				MouseHandler::Instance()->ItemUseTransaction(SI_STYLE, pos);
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


//----------------------------------------------------------------------------
/**
*/
VOID			
StyleContainer::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			if (pos >= STYLE_QUICK_BT00 && pos < STYLE_QUICK_MAX)
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


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	m_pItemUnitRenderer->FlushUnitDraw();

	QuickSlot FromSlot;

	for (int i = 0; i < GetMaxSlotNum(); i++)
	{
			
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

}


//----------------------------------------------------------------------------
/**
*/
VOID 
StyleContainer::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); i++)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		StyleSlot & rQuickSlot = (StyleSlot &)rSlot;

		if (!IsEmpty(i))
		{
			if ( i >= STYLE_START_SLOT && i < STYLE_MAX_SLOT )
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여줘야할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rQuickSlot.GetOrgCode() );

				if (!pRender )
				{
					pCtrl = getControl( i );
					if(pCtrl)
					{
						rcSlot = pCtrl->GetSizeRect();

						m_pItemUnitRenderer->AddItemUnit( 
							rQuickSlot.GetSerial(), 
							rQuickSlot.GetOrgCode(), 
							&rcSlot, 
							rQuickSlot.GetOrgSlot() ,pCtrl);
					}
				}

				if (pRender)
					pRender->SetShow(TRUE);
			}
			else
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여지지 말아야 할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender )
				{
					pCtrl = getControl( i );
					if(pCtrl)
					{
						rcSlot = pCtrl->GetSizeRect();

						m_pItemUnitRenderer->AddItemUnit( 
							rQuickSlot.GetSerial(), 
							rQuickSlot.GetOrgCode(), 
							&rcSlot, 
							rQuickSlot.GetOrgSlot() ,pCtrl);
					}
				}

				if (pRender)
					pRender->SetShow(FALSE);				
			}
		}
	}
}