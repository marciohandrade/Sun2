#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "ui_XMas_SoloCoupleReward/ui_XMas_SoloCoupleReward.h"

#include "Mouse.h"
#include "GlobalFunc.h"
#include "GraphicResourceList.h"
#include "ItemInfoParser.h"
#include "SunEventInfoParser.h"
#include "RewardInfoList.h"
#include "FTextRender.h"
#include "FTextSentence.h"
#include "GameFunc.h"

const int c_Str_Interval = 13;
const DWORD c_dw_Item_code = 60000;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_SoloCoupleReward::m_wzID[eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAX] = 
{
	StrToWzID("S001"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_TITLE_TEXT,
	StrToWzID("LS00"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAIN_TEXT_LIST,
	StrToWzID("BT01"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_SLOT_BTN,
	StrToWzID("BT00"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_OK_BTN,
	StrToWzID("B002"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_CANCEL_BTN,
	StrToWzID("LS01"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_ITEM_TEXT_LIST,
	StrToWzID("B001"),	//eDIALOG_XMAS_SOLO_COUPLE_REWARD_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SoloCoupleReward::ui_XMas_SoloCoupleReward(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;

	m_eSoloCoupleState = eSOLO_COUPLE_STATE_NONE;
	m_hSlotTexture     = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SoloCoupleReward::~ui_XMas_SoloCoupleReward()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();

	RECT rtTitle = m_ctrlTitleStatic->GetSizeRect();
	RECT rtMain  = m_ctrlMainList->GetSizeRect();
	RECT rtItemList   = m_ctrlItemList->GetSizeRect();
	m_FtextRender[0].Init(g_pSunRenderer , rtTitle);
	m_FtextRender[1].Init(g_pSunRenderer , rtMain);
	m_FtextRender[2].Init(g_pSunRenderer , rtItemList);
	m_FtextRender[3].Init(g_pSunRenderer , rtItemList);

	m_bTextLoad = TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::OnRenderSolarDialog()
{
	if(g_pHero)
	{
		RenderSlotTexture();
		if(m_bTextLoad)
		{
			AddFormatText();
			m_bTextLoad = FALSE;
		}

		RECT rt[4];
		rt[0] = m_ctrlTitleStatic->GetSizeRect();
		rt[0].left += 90;
		rt[0].top  -= 4;

		rt[1] = m_ctrlMainList->GetSizeRect();
		rt[1].right -= 15;

		rt[2] = m_ctrlItemList->GetSizeRect();
		rt[2].top   -= 5;
		rt[2].right -= 10;

		rt[3] = rt[2];
		rt[3].top += 16;

		for(int i = 0; i < 4; ++i)
		{
			m_FtextRender[i].SetRect(rt[i]);
			m_FtextRender[i].Render();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::RenderSlotTexture()
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
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		LoadItemTextureHandle();
		m_bShow = TRUE;
		m_bTextLoad = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDotype_Event_X_Mas_SoloCoupleReward;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_SOLOCOUPLE_REWARD;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDotype_Event_X_Mas_SoloCoupleReward);
		m_bShow   = FALSE;
		m_bTextLoad = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::LoadItemTextureHandle()
{
	DWORD dwRewardCode = 0;
	DWORD dwItemCode = 0;
	EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_SoloCouple_NPC_Type);
	//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
	if(NULL != pEventInfoVec)
	{
		EVENT_INFO* pEventInfo = pEventInfoVec->back();
		if(pEventInfo)
		{
			dwRewardCode = pEventInfo->m_dwRewardInfo[0];
		}
	}

	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardCode);
	if(eSOLO_COUPLE_STATE_SOLO == m_eSoloCoupleState)
	{
		dwItemCode = pRewardInfo->GetTypeCode(0);
	}
	else if(eSOLO_COUPLE_STATE_COUPLE == m_eSoloCoupleState)
	{
		dwItemCode = pRewardInfo->GetTypeCode(1);
	}
	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);//dwItemCode로 교체
	if(pItemInfo)
	{
		DWORD pIconCode = pItemInfo->m_wVICode;
		RESOURCE_INFO* pResInfo = GraphicResourceList::Instance()->GetItemInfo(pIconCode);
		m_hSlotTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( pResInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_SOLO_COUPLE_REWARD_OK_BTN:
				{
					MSG_CG_EVENT_REQUEST_ITEM_SYN SendMsg;

					EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_SoloCouple_NPC_Type);
					//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
					DWORD dwEventID = 0;
					if(NULL != pEventInfoVec)
					{
						EVENT_INFO* pEventInfo = pEventInfoVec->back();
						if(pEventInfo)
						{
							dwEventID = pEventInfo->m_dwEventID;
						}
						else
						{
							assert(!"ui_XMas_SoloCoupleReward::MessageProc");
						}
					}

					if(eSOLO_COUPLE_STATE_NONE == m_eSoloCoupleState)
					{
						assert(!"ui_XMas_SoloCoupleReward::MessageProc");
					}
					else if(eSOLO_COUPLE_STATE_SOLO == m_eSoloCoupleState)
					{
						SendMsg.SetInfo(dwEventID, MSG_CG_EVENT_REQUEST_ITEM_BADGEMAN_SYN::SOLO_BADGE);
					}
					else if(eSOLO_COUPLE_STATE_COUPLE == m_eSoloCoupleState)
					{
						SendMsg.SetInfo(dwEventID, MSG_CG_EVENT_REQUEST_ITEM_BADGEMAN_SYN::COUPLE_BADGE);
					}

					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_XMAS_SOLO_COUPLE_REWARD_CANCEL_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_XMAS_SOLO_COUPLE_REWARD_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			}
		}
		break;

	default:
		{
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::InitControl()
{
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_SOLO_COUPLE_REWARD_MAIN_TEXT_LIST));
	assert(m_ctrlMainList);

	m_ctrlItemList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_SOLO_COUPLE_REWARD_ITEM_TEXT_LIST));
	assert(m_ctrlItemList);

	m_ctrlTitleStatic = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_XMAS_SOLO_COUPLE_REWARD_TITLE_TEXT));
	assert(m_ctrlTitleStatic);
	if(m_ctrlTitleStatic)
	{
		m_ctrlTitleStatic->SetTextWZ("");
	}

	m_ctrlSlotBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_SOLO_COUPLE_REWARD_SLOT_BTN));
	assert(m_ctrlSlotBtn);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SoloCoupleReward::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rtTitle       = m_ctrlTitleStatic->GetSizeRect();
	rtTitle.left += 90;
	rtTitle.top  -= 4;
	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	rtMain.right -= 15;
	RECT rtItem		   = m_ctrlItemList->GetSizeRect();
	rtItem.top   -= 5;
	rtItem.right -= 10;

	TCHAR szTitle[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};
	TCHAR szItemName[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szItemDesc[INTERFACE_STRING_LENGTH + 1] = {0,};
	
	switch(m_eSoloCoupleState)
	{
	case eSOLO_COUPLE_STATE_NONE:
		{
			assert(!"ui_XMas_SoloCoupleReward::AddFormatText");
		}
		break;

	case eSOLO_COUPLE_STATE_SOLO:
		{
			g_InterfaceManager.GetInterfaceString(1870, szTitle);
			g_InterfaceManager.GetInterfaceString(203106, szMainFinal);
		}
		break;

	case eSOLO_COUPLE_STATE_COUPLE:
		{
			g_InterfaceManager.GetInterfaceString(1872, szTitle);
			g_InterfaceManager.GetInterfaceString(203107, szMainFinal);
		}
		break;
	}

//////////////////////////////////////////////////////////////////////////
//ui Title

	FParser.SetDefaultFont(StrToWzID("mn12") , WzColor_RGBA(240, 202, 121, 255));
	ret = FParser.Parser(szTitle , m_FSentence );

	if( ret )
	{
		m_FtextRender[0].ClearSentence();
		m_FtextRender[0].Append(m_FSentence);
		m_FtextRender[0].SetRect(rtTitle);
	}

//////////////////////////////////////////////////////////////////////////
//Main String

	FParser.SetDefaultFont(StrToWzID("mn12") , WzColor_RGBA(235,224,200,255 ));
	ret = FParser.Parser(szMainFinal , m_FSentence );

	if( ret )
	{
		m_FtextRender[1].ClearSentence();
		m_FtextRender[1].Append(m_FSentence);
		m_FtextRender[1].SetRect(rtMain);
	}

//////////////////////////////////////////////////////////////////////////
// Item Title, Item Desc

	FParser.SetDefaultFont(StrToWzID("st10") , WzColor_RGBA(235,224,200,255 ));
	DWORD dwRewardCode = 0;
	DWORD dwItemCode = 0;
	EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_SoloCouple_NPC_Type);
	//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
	if(NULL != pEventInfoVec)
	{
		EVENT_INFO* pEventInfo = pEventInfoVec->back();
		if(pEventInfo)
		{
			dwRewardCode = pEventInfo->m_dwRewardInfo[0];
		}
	}

	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardCode);
	if(pRewardInfo)
	{
		if(eSOLO_COUPLE_STATE_SOLO == m_eSoloCoupleState)
		{
			dwItemCode = pRewardInfo->GetTypeCode(0);
		}
		else if(eSOLO_COUPLE_STATE_COUPLE == m_eSoloCoupleState)
		{
			dwItemCode = pRewardInfo->GetTypeCode(1);
		}
	}

	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);
	if(pItemInfo)
	{
		g_InterfaceManager.GetInterfaceString(pItemInfo->m_NCode, szItemName);
        GameFunc::GetItemDescription(szItemDesc, NULL, pItemInfo);

		ret = FParser.Parser(szItemName , m_FSentence );
		if( ret )
		{
			m_FtextRender[2].ClearSentence();
			m_FtextRender[2].Append(m_FSentence);
			m_FtextRender[2].SetRect(rtItem);
		}

		rtItem.top += 16;
		ret = FParser.Parser(szItemDesc , m_FSentence );

		if( ret )
		{
			m_FtextRender[3].ClearSentence();
			m_FtextRender[3].Append(m_FSentence);
			m_FtextRender[3].SetRect(rtItem);
		}
	}

//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
