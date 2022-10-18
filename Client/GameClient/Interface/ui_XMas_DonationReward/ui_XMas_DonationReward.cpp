#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "ui_XMas_DonationReward/ui_XMas_DonationReward.h"

#include "ItemInfoParser.h"
#include "GraphicResourceList.h"
#include "NPCInfoParser.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "HeroActionInput.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "FTextRender.h"
#include "FTextSentence.h"

const int c_Str_Interval = 13;
const DWORD c_dw_Item_code = 60000;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_DonationReward::m_wzID[eDIALOG_XMAS_DONATION_REWARD_MAX] = 
{
		StrToWzID("LS00"), //eDIALOG_XMAS_DONATION_REWARD_MAIN_TEXT_LIST,
		StrToWzID("BT01"), //eDIALOG_XMAS_DONATION_REWARD_SLOT_BTN,
		StrToWzID("BT00"), //eDIALOG_XMAS_DONATION_REWARD_OK_BTN,
		StrToWzID("LS01"), //eDIALOG_XMAS_DONATION_REWARD_ITEM_TEXT_LIST,
		StrToWzID("B001"), //eDIALOG_XMAS_DONATION_REWARD_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_DonationReward::ui_XMas_DonationReward(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_hSlotTexture    = INVALID_HANDLE_VALUE;
	m_ItemCode		  = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_DonationReward::~ui_XMas_DonationReward()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();

	RECT rtMain  = m_ctrlMainList->GetSizeRect();
	RECT rtItemList   = m_ctrlItemList->GetSizeRect();
	m_FtextRender[0].Init(g_pSunRenderer , rtMain);
	m_FtextRender[1].Init(g_pSunRenderer , rtItemList);
	m_FtextRender[2].Init(g_pSunRenderer , rtItemList);

	m_bTextLoad = TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::OnRenderSolarDialog()
{
	if(g_pHero)
	{
		RenderSlotTexture();
		if(m_bTextLoad)
		{
			AddFormatText();
			m_bTextLoad = FALSE;
		}
		RECT rt[3];
		rt[0] = m_ctrlMainList->GetSizeRect();
		rt[0].right -= 15;

		rt[1] = m_ctrlItemList->GetSizeRect();
		rt[1].top   -= 5;
		rt[1].right -= 10;

		rt[2] = rt[1];
		rt[2].top += 20;

		for(int i = 0; i < 3; ++i)
		{
			m_FtextRender[i].SetRect(rt[i]);
			m_FtextRender[i].Render();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::RenderSlotTexture()
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

void ui_XMas_DonationReward::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		ShowNpcEffect();
		LoadItemTextureHandle();
		m_bShow = TRUE;
		m_bTextLoad = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDotype_Event_X_Mas_Donation_Reward;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_DONATION_REWARD;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDotype_Event_X_Mas_Donation_Reward);
		m_bShow    = FALSE;
		m_bTextLoad = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::ShowNpcEffect()
{
	EXTRA_NPCINFO* pExNpcInfo = NPCInfoParser::Instance()->GetExtraInfo(g_HeroInput.GetClickedNpcCode());
	if(pExNpcInfo)
	{
		if(eNPC_TYPE_EVENT_DONATION == pExNpcInfo->m_eNPCTYPE)
		{
			
			Object* pObj = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
			if(pObj)
			{
				if(NPC_OBJECT == pObj->GetSCObject().GetObjectType())
				{
					pObj->SetNextState(STATE::NPC_SPECIAL_ACT_STATE, g_CurTime);
				}
			}
		}
	}
	else
	{
		assert(!"ui_XMas_DonationReward::ShowNpcEffect()");
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::LoadItemTextureHandle()
{
	assert(m_ItemCode);

	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_ItemCode);//dwItemCode·Î ±³Ã¼
	if(pItemInfo)
	{
		DWORD pIconCode = pItemInfo->m_wVICode;
		RESOURCE_INFO* pResInfo = GraphicResourceList::Instance()->GetItemInfo(pIconCode);
		m_hSlotTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( pResInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_DONATION_REWARD_OK_BTN:
			case eDIALOG_XMAS_DONATION_REWARD_EXIT_BTN:
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

void ui_XMas_DonationReward::InitControl()
{
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_DONATION_REWARD_MAIN_TEXT_LIST));
	assert(m_ctrlMainList);

	m_ctrlItemList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_DONATION_REWARD_ITEM_TEXT_LIST));
	assert(m_ctrlItemList);

	m_ctrlSlotBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_DONATION_REWARD_SLOT_BTN));
	assert(m_ctrlSlotBtn);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationReward::AddFormatText()
{
	if(0 == m_ItemCode)
		return;

	FTextParser		FParser;
	BOOL			ret;

	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	rtMain.right -= 15;
	RECT rtItem		   = m_ctrlItemList->GetSizeRect();
	rtItem.top   -= 5;
	rtItem.right -= 10;

	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};
	TCHAR szItemName[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szItemDesc[INTERFACE_STRING_LENGTH + 1] = {0,};

	//////////////////////////////////////////////////////////////////////////
	//Main String

	g_InterfaceManager.GetInterfaceString(203105, szMainFinal);

	FParser.SetDefaultFont(StrToWzID("mn12") , WzColor_RGBA(235,224,200,255 ));
	ret = FParser.Parser(szMainFinal , m_FSentence );

	if( ret )
	{
		m_FtextRender[0].ClearSentence();
		m_FtextRender[0].Append(m_FSentence);
		m_FtextRender[0].SetRect(rtMain);
	}

	//////////////////////////////////////////////////////////////////////////
	// Item Title, Item Desc

	FParser.SetDefaultFont(StrToWzID("st10") , WzColor_RGBA(235,224,200,255 ));

	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_ItemCode);
	if(pItemInfo)
	{
		g_InterfaceManager.GetInterfaceString(pItemInfo->m_NCode, szItemName);
        GameFunc::GetItemDescription(szItemDesc, NULL, pItemInfo);

		ret = FParser.Parser(szItemName , m_FSentence );
		if( ret )
		{
			m_FtextRender[1].ClearSentence();
			m_FtextRender[1].Append(m_FSentence);
			m_FtextRender[1].SetRect(rtItem);
		}

		rtItem.top += 20;
		ret = FParser.Parser(szItemDesc , m_FSentence );

		if( ret )
		{
			m_FtextRender[2].ClearSentence();
			m_FtextRender[2].Append(m_FSentence);
			m_FtextRender[2].SetRect(rtItem);
		}
	}

	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
