#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "ui_XMas_SantaReward/ui_XMas_SantaReward.h"

#include "uiEventMan/uiEventMan.h"

#include "Application.h"
#include "Mouse.h"
#include "GlobalFunc.h"
#include "MouseHandler.h"
#include "SunEventInfoParser.h"
#include "ItemInfoParser.h"
#include "GraphicResourceList.h"
#include "RewardInfoList.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include <SCItemSlot.h>

const int c_Str_Interval = 13;
const DWORD c_dw_Item_code = 60000;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_SantaReward::m_wzID[eDIALOG_XMAS_SANTA_REWARD_MAX] = 
{
	StrToWzID("LS00"),	//eDIALOG_XMAS_SANTA_REWARD_MAIN_TEXT_LIST,
	StrToWzID("BT01"),	//eDIALOG_XMAS_SANTA_REWARD_SLOT_BTN,
	StrToWzID("BT02"),	//eDIALOG_XMAS_SANTA_REWARD_OK_BTN,
	StrToWzID("BT00"),	//eDIALOG_XMAS_SANTA_REWARD_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SantaReward::ui_XMas_SantaReward(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	memset(&m_SlotInfo,0,sizeof(m_SlotInfo));
	m_bShow			  = FALSE;
	m_bRenderToolTip  = FALSE;
	m_bOnSlot		  = FALSE;
	m_hSlotTexture	  = INVALID_HANDLE_VALUE;
	m_ctrlMainList    = NULL;
	m_ctrlSlotBtn     = NULL;
	m_eSlotState	  = eSLOT_STATE_NONE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SantaReward::~ui_XMas_SantaReward()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;
	m_bRenderToolTip  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	UpdateToolTip();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	if(m_ctrlSlotBtn)
	{
		RECT rc = m_ctrlSlotBtn->GetSizeRect();

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

void ui_XMas_SantaReward::RenderTooltip()
{
	switch(m_eSlotState)
	{
	case eSLOT_STATE_NONE:
		{
		}
		break;

	case eSLOT_STATE_SOUCE:
		{
			uiToolTipMan* pToolTipMan = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
			if(pToolTipMan)
			{
				DWORD dwItemCode = 0;
				EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_Santa_NPC_Type);
				//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
				if(NULL != pEventInfoVec)
				{
					EVENT_INFO* pEventInfo = pEventInfoVec->back();
					if(pEventInfo)
					{
						dwItemCode = pEventInfo->m_dwRegisterItemIndex[0];
					}

					BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);//dwItemCode로 수정

					if(pItemInfo)
					{
						pToolTipMan->RegisterTooltip_Reward(pItemInfo);
						pToolTipMan->SetTooltipDialogID(GetDialogKey());
					}
				}
			}
		}
		break;

	case eSLOT_STATE_DEST:
		{
			uiToolTipMan* pToolTipMan = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
			if(pToolTipMan)
			{
				if(m_SlotInfo.dwRewardItemCode!=0)
				{
					BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_SlotInfo.dwRewardItemCode);
					if(pItemInfo)
					{
						pToolTipMan->RegisterTooltip_Reward(pItemInfo);
						pToolTipMan->SetTooltipDialogID(GetDialogKey());
					}
				}
			}
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::OnRenderSolarDialog()
{
	if(g_pHero)
	{
		AddFormatText();
		RenderSlotTexture();

		if(m_bRenderToolTip)
		{
			RenderTooltip();
		}
	}
}

void ui_XMas_SantaReward::RenderSlotTexture()
{	
	if(m_hSlotTexture!=INVALID_HANDLE_VALUE)
	{
		RECT rt = m_ctrlSlotBtn->GetSizeRect();
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

		RenderRewardOverlap();
	}
}

//갯수찍기
void ui_XMas_SantaReward::RenderRewardOverlap()
{
	if(m_eSlotState!=eSLOT_STATE_DEST || m_SlotInfo.dwRewardItemNum<=0 || !m_ctrlSlotBtn)
	{
		return;
	}

	RECT rt = m_ctrlSlotBtn->GetSizeRect();

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );

	SIZE FontSize;
	TCHAR	szNum[256];
	Sprintf(szNum, _T("%d"), m_SlotInfo.dwRewardItemNum);

	size_t len = _tcslen(szNum);
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szNum, len, &FontSize);

	int x = rt.right  - ( FontSize.cx + len );
	int y = rt.bottom - ( FontSize.cy );
	SIZE size = { x + FontSize.cx + len, y + FontSize.cy };
	RECT temp = { x, y, size.cx, size.cy };

	g_pSunRenderer->x_pManagerTextOut->DrawText( szNum, &temp,
		DEFAULT_FONT_WZCOLOR, WzColor_RGBA( 0,0,0,0 ),TP_HLEFT,
		ETS_OUTLINE, WzColor_RGBA(  0,0,0,255 ) );

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

//////////////////////////////////////////////////////////////////////////
//	보상받은 아이템을 그려주는걸로 변경
//////////////////////////////////////////////////////////////////////////
void ui_XMas_SantaReward::SetRewardInfo(INVENTORY_TOTAL_INFO & rTotalInfo)
{
	if(!g_pHero)
	{
		return;
	}
	
	SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);

	if(!pContainer)
	{
		return;
	}
	
	//기획팀과협의:산타UI 슬롯은하나기때문에 다른종류아이템이 왔을경우에 무조건
	//			   하나만처리함
	POSTYPE total = rTotalInfo.m_InvenCount;
	ITEMSLOTEX * pSlot = rTotalInfo.m_Slot;

	// 무조건 서버에서 보내온 정보를 믿고 Insert 시킴. 
	for(POSTYPE i=0;i<total;++i)
	{
		SCItemSlot ItemBuf;
		ItemBuf.Clear();
		ItemBuf.Copy( pSlot[i].m_Stream);
		m_SlotInfo.dwRewardItemNum = 0;

		if (!pContainer->IsEmpty(pSlot[i].m_Pos))
		{
			SCItemSlot &rInvenSlot = (SCItemSlot &)pContainer->GetSlot(pSlot[i].m_Pos);
			m_SlotInfo.dwRewardItemNum = ItemBuf.GetNum()-rInvenSlot.GetNum();
		}
		else
		{
			m_SlotInfo.dwRewardItemNum = ItemBuf.GetNum();
		}

		if(m_SlotInfo.dwRewardItemNum<0)
		{
			m_SlotInfo.dwRewardItemNum = 0;
		}

		const BASE_ITEMINFO * pRewardItem  = ItemBuf.GetItemInfo();
		

		if(pRewardItem)
		{
			m_SlotInfo.dwRewardItemCode = pRewardItem->m_Code;

			if(INVALID_HANDLE_VALUE == m_hSlotTexture)
			{
				DWORD pIconCode = pRewardItem->m_wVICode;
				RESOURCE_INFO* pResInfo = GraphicResourceList::Instance()->GetItemInfo(pIconCode);
				if(pResInfo)
				{
					m_hSlotTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( pResInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
				}

			}
			//아이템이 존재한다면  break
			break;
		}
		
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::Process(DWORD dwTick)
{
	//슬롯버튼 영역에 아이템을 든 커서가 위치해 있는지 체크
	if(MouseHandler::Instance()->IsExistItemAtHand())
	{
		int iX, iY;
		iX = Mouse::Instance()->GetMouseX();
		iY = Mouse::Instance()->GetMouseY();
		if(m_ctrlSlotBtn)
		{
			RECT rt = m_ctrlSlotBtn->GetSizeRect();
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

void ui_XMas_SantaReward::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_hSlotTexture		  = INVALID_HANDLE_VALUE;
		m_eSlotState	      = eSLOT_STATE_NONE;

		MoveCenter();
		m_bShow				  = TRUE;
		m_SlotInfo.dwRewardItemCode    = 0;
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDotype_Event_X_Mas_SantaReward;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_SANTA_REWARD;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDotype_Event_X_Mas_SantaReward);
		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_SANTA_REWARD_OK_BTN:
				{
					switch(m_eSlotState)
					{
					case eSLOT_STATE_NONE: //무시한다.
						{
						}
						break;

					case eSLOT_STATE_DEST:
						{
							OnShowWindow(FALSE);
						}
						break;

					case eSLOT_STATE_SOUCE:
						{
							BOOL bRet = FALSE;
							MSG_CG_EVENT_ITEM_REG_SYN	sync;
							EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_Santa_NPC_Type);
							//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
							if(NULL != pEventInfoVec)
							{
								EVENT_INFO* pEventInfo = pEventInfoVec->back();							
								if(pEventInfo)
								{
									sync.m_dwEventID = pEventInfo->m_dwEventID;
									bRet = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

									if(bRet)
									{
										memset(&m_SlotInfo,0,sizeof(m_SlotInfo));
										m_hSlotTexture = INVALID_HANDLE_VALUE;	
									}
								}
								
							}
						}
						break;

					default:
						{
							assert(!"ui_XMas_SantaReward::MessageProc");
						}
					}
				}
				break;

			case eDIALOG_XMAS_SANTA_REWARD_SLOT_BTN:
				{
					if(m_bOnSlot)
					{
						if(MouseHandler::Instance()->IsExistItemAtHand())
						{
							DWORD dwItemCode = 0;
							EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_Santa_NPC_Type);
							//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
							if(NULL != pEventInfoVec)
							{
								EVENT_INFO* pEventInfo = pEventInfoVec->back();
								if(pEventInfo)
								{
									dwItemCode = pEventInfo->m_dwRegisterItemIndex[0];
								}
							}

							MouseHandler::MouseItem* pMouseItem = MouseHandler::Instance()->GetMouseItem();
							if(dwItemCode == pMouseItem->m_pFromBaseItemInfo->m_Code)
							{
								BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);
								if(pItemInfo)
								{
									DWORD pIconCode = pItemInfo->m_wVICode;
									RESOURCE_INFO* pResInfo = GraphicResourceList::Instance()->GetItemInfo(pIconCode);
									if(pResInfo)
									{
										m_hSlotTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( pResInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
										m_eSlotState   = eSLOT_STATE_SOUCE;
									}
								}
								else
								{
									assert(!"ui_XMas_SantaReward::MessageProc");
								}
							}
						}

						MouseHandler::Instance()->RollbackItemAtHand();		
					}
				}
				break;

			case eDIALOG_XMAS_SANTA_REWARD_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			}
		}
		break;

	default:
		{
			m_bRenderToolTip = FALSE;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::InitControl()
{
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_SANTA_REWARD_MAIN_TEXT_LIST));
	assert(m_ctrlMainList);
	m_ctrlSlotBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_SANTA_REWARD_SLOT_BTN));
	assert(m_ctrlSlotBtn);
	m_ctrlOKBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_SANTA_REWARD_OK_BTN));
	assert(m_ctrlOKBtn);
	if(m_ctrlOKBtn)
	{
		m_ctrlOKBtn->SetTextWZ("");
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaReward::AddFormatText()
{
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(_T("mn12"));

	RECT rtOKBtn		   = m_ctrlOKBtn->GetSizeRect();
	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	TCHAR szOKBtn[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};

	switch(m_eSlotState)
	{
	case eSLOT_STATE_NONE:
	case eSLOT_STATE_SOUCE:
		{
			//1874		전달 완료
			g_InterfaceManager.GetInterfaceString(1874, szOKBtn);
			//203108	내 선물상자를 찾아온 건가?
			g_InterfaceManager.GetInterfaceString(203108, szMainFinal);
		}
		break;

	case eSLOT_STATE_DEST:
		{
			//100		확인
			g_InterfaceManager.GetInterfaceString(100, szOKBtn);
			//4101		고맙네. 이건 내 답례라네.
			g_InterfaceManager.GetInterfaceString(4101, szMainFinal);
		}
		break;
	}

	rtOKBtn.top += 10;
	this->GetDrawBase()->x_pManagerTextOut->DrawText(
		szOKBtn,
		&rtOKBtn,
		WzColor_RGBA(240, 202, 121, 255),
		WzColor_RGBA(0,0,0,0),
		TP_HCENTER);

	this->GetDrawBase()->x_pManagerTextOut->DrawText(
		szMainFinal,
		&rtMain,
		WzColor_RGBA(235,224,200,255 ),
		WzColor_RGBA(0,0,0,0));

	this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
