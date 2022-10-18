#include "SunClientPrecompiledHeader.h"
#ifdef _YJ_FISHING_SYSTEM_000933
#include "InterfaceManager.h"
#include "uiFishingTrade/uiFishingTrade.h"

#include "Application.h"
#include "Mouse.h"
#include "GlobalFunc.h"
#include "MouseHandler.h"
#include "ItemInfoParser.h"
#include "GraphicResourceList.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "IconImage.h"

//eITEMTYPE_FISH_HYBRID			= 20022,	//잡종 물고기
//eITEMTYPE_FISH_WASTE			= 20023,	//잡종 물고기
//eITEMTYPE_FISH_HIGH_CLASS		= 20024,	//잡종 물고기
//eITEMTYPE_FISH_RARE			= 20025,	//잡종 물고기
//eITEMTYPE_FISH_AC				= 20026,	//잡종 물고기
//eITEMTYPE_FISH_CASH			= 20027,	//잡종 물고기

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiFishingTrade::m_wzID[eDIALOG_FISHING_TRADE_MAX] = 
{
	StrToWzID("B001"),	//eDIALOG_FISHING_TRADE_EXIT_BTN,
	StrToWzID("BT00"),	//eDIALOG_FISHING_TRADE_TRADE_BTN,
	StrToWzID("B002"),	//eDIALOG_FISHING_TRADE_CANCEL_BTN,

	StrToWzID("BT02"),	//eDIALOG_FISHING_TRADE_SLOT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiFishingTrade::uiFishingTrade(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_bOnSlot		  = FALSE;
	m_hSlotTexture	  = INVALID_HANDLE_VALUE;
	m_bRenderToolTip  = FALSE;
	m_dwItemCode	  = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiFishingTrade::~uiFishingTrade()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	UpdateToolTip();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	if(m_pctrlSlotBtn)
	{
		RECT rc = m_pctrlSlotBtn->GetSizeRect();

		if(rc.left <= iMouseX
			&& rc.right >= iMouseX
			&& rc.top <= iMouseY 
			&& rc.bottom >= iMouseY)
		{
			m_bRenderToolTip = TRUE;
		}
		else
		{
			m_bRenderToolTip = FALSE;
		}

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::OnRenderSolarDialog()
{
	if(m_hSlotTexture!=INVALID_HANDLE_VALUE)
	{
		RECT rt = m_pctrlSlotBtn->GetSizeRect();
		float fWidth  = (float)(rt.right - rt.left);
		float fHeigth = (float)(rt.bottom - rt.top);
		g_pSunRenderer->RenderTexture( m_hSlotTexture, 
			(float)rt.left,
			(float)rt.top,
			fWidth,
			fHeigth,
			0,
			0,
			(float)ICON_IMAGE_SIZE,
			(float)ICON_IMAGE_SIZE);

		RenderTooltip();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::RenderTooltip()
{
	if(FALSE == m_bRenderToolTip)
		return;

	uiToolTipMan* pToolTipMan = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
	if(pToolTipMan)
	{
		BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_dwItemCode);//dwItemCode로 수정
		if(pItemInfo)
		{
			pToolTipMan->RegisterTooltip_Reward(pItemInfo);
			pToolTipMan->SetTooltipDialogID(GetDialogKey());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::Process(DWORD dwTick)
{
	//슬롯버튼 영역에 아이템을 든 커서가 위치해 있는지 체크
	if(MouseHandler::Instance()->IsExistItemAtHand())
	{
		int iX, iY;
		iX = Mouse::Instance()->GetMouseX();
		iY = Mouse::Instance()->GetMouseY();
		if(m_pctrlSlotBtn)
		{
			RECT rt = m_pctrlSlotBtn->GetSizeRect();
			if((rt.bottom > iY) &&
				(rt.top < iY)    &&
				(rt.right > iX)  &&
				(rt.left < iX))
			{
				m_bOnSlot = TRUE;
			}
			else
			{
				m_bOnSlot = FALSE;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;
	}
	else
	{
		m_bShow   = FALSE;
		m_dwItemCode = 0;
		m_hSlotTexture = INVALID_HANDLE_VALUE;
		m_bRenderToolTip = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_FISHING_TRADE_EXIT_BTN:
			case eDIALOG_FISHING_TRADE_CANCEL_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			case eDIALOG_FISHING_TRADE_TRADE_BTN:
				{
				}
				break;
			case eDIALOG_FISHING_TRADE_SLOT_BTN:
				{
					if(m_bOnSlot)
					{
						if(MouseHandler::Instance()->IsExistItemAtHand())
						{
							MouseHandler::MouseItem* pMouseItem = MouseHandler::Instance()->GetMouseItem();
							if(pMouseItem)
							{
								BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pMouseItem->m_pFromBaseItemInfo->m_Code);
								if(pItemInfo)
								{
									DWORD pIconCode = pItemInfo->m_wVICode;
									RESOURCE_INFO* pResInfo = GraphicResourceList::Instance()->GetItemInfo(pIconCode);
									m_dwItemCode   = pItemInfo->m_Code;
									m_hSlotTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( pResInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
								}
								else
								{
									assert(!"uiFishingTrade::MessageProc");
								}
							}
						}

						MouseHandler::Instance()->RollbackItemAtHand();		
					}
				}
				break;
			}
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingTrade::InitControl()
{
	m_pctrlSlotBtn = (CCtrlButtonWZ*)getControl(eDIALOG_FISHING_TRADE_SLOT_BTN);
	assert(m_pctrlSlotBtn);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_YJ_FISHING_SYSTEM_000933