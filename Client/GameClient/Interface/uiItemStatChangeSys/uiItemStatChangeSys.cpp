#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiItemStatChangeSys/uiItemStatChangeSys.h"

#include <SCSlot.h>
#include <SCSlotContainer.h>
#include <SCItemSlot.h>

#include "ItemInfoParser.h"
#include "GraphicResourceList.h"
#include "ResourceManager.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "GameFunc.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiItemStatChangeSys::m_wzID[eDIALOG_ITEM_STAT_CHANGE_MAX] = 
{
		StrToWzID("TI00"),//eDIALOG_ITEM_STAT_CHANGE_SYS_OK_BTN,
		StrToWzID("TI01"),//eDIALOG_ITEM_STAT_CHANGE_SYS_CANCEL_BTN,
		StrToWzID("BT00"),//eDIALOG_ITEM_STAT_CHANGE_SYS_EXIT_BTN,
		StrToWzID("C000"),//eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_1,
		StrToWzID("C001"),//eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_2,
		StrToWzID("C002"),//eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_3,
		StrToWzID("C003"),//eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_4,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiItemStatChangeSys::uiItemStatChangeSys(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_bSendPacket	  = FALSE;

	DWORD dwTemp[4] = {0, 0, 0, 0};
	SetSlotInfo(MAX_CHANGE_ITEMSTAT_SLOT_NUM, &dwTemp[0], INVALID_POSTYPE_VALUE, INVALID_POSTYPE_VALUE);
	m_dwSelItemCode = 0;
	m_hTexHandle = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiItemStatChangeSys::~uiItemStatChangeSys()
{
	if (m_hTexHandle != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTexHandle);
		m_hTexHandle = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::Release()
{
	if (m_hTexHandle != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTexHandle);
		m_hTexHandle = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::OnRenderSolarDialog()
{
	RenderSlotTexture();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::SetSlotInfo(int iCnt, DWORD* SlotInfo, POSTYPE atItemPos, POSTYPE ChangeItemPos)
{
	ZeroMemory(&m_dwItemCode, sizeof(m_dwItemCode));
	for(int i = 0; i < iCnt; ++i)
	{
		 m_dwItemCode[i] = SlotInfo[i];
	}

	m_ItemPosInInven = atItemPos;
	m_ChangedPosItem = ChangeItemPos;

	if((INVALID_POSTYPE_VALUE != m_ItemPosInInven) && (INVALID_POSTYPE_VALUE != m_ChangedPosItem))
	{
		//해당 아이템 잠금상태(반투명 처리)
		SCSlotContainer* pAtContainer  = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
		SCItemSlot& rItemSlot1		   = (SCItemSlot&)pAtContainer->GetSlot( m_ItemPosInInven );
		SCItemSlot& rItemSlot2		   = (SCItemSlot&)pAtContainer->GetSlot( m_ChangedPosItem );
		rItemSlot1.SetLock(TRUE);
		rItemSlot2.SetLock(TRUE);
	}

	SCSlotContainer* pAtContainer  = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (!pAtContainer)
    {
        return;
    }

	if(INVALID_POSTYPE_VALUE == m_ChangedPosItem)
    {
		return;
    }

	if(pAtContainer->IsEmpty(m_ChangedPosItem))
    {
		return;
    }

	SCItemSlot& rItemSlot		   = (SCItemSlot&)pAtContainer->GetSlot( m_ChangedPosItem );
	CODETYPE resourceCode = 0xffffffff;

    CODETYPE image_code = GameFunc::GetImagecode(&rItemSlot, m_dwItemCode[0]);
    BOOL is_limited = GameFunc::IsLimitedItem(&rItemSlot, m_dwItemCode[0]);
    
    if (image_code != 0)
    {
        resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
    }

	RESOURCE_INFO * pInfo = GraphicResourceList::Instance()->GetItemInfo(resourceCode);

	if ( pInfo )
	{
		m_hTexHandle = g_pSunRenderer->x_pManagerTexture->LoadTexture(pInfo->szFilename ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiItemStatChangeSys::RenderSlotTexture()
{	
	HANDLE hTexture = NULL;
	WzColor color[4];

	int i = 0;
	BOOL bNoSel = TRUE;
	for(i = 0; i < MAX_CHANGE_ITEMSTAT_SLOT_NUM; ++i)
	{
		if(0 == m_dwSelItemCode)
			break;

		if(m_dwItemCode[i] == m_dwSelItemCode)
			bNoSel = FALSE;
	}

	for(i = 0; i < MAX_CHANGE_ITEMSTAT_SLOT_NUM; ++i)
	{
		if((m_hTexHandle != INVALID_HANDLE_VALUE) && (0 != m_dwItemCode[i]))
		{
			int iMouseX = Mouse::Instance()->GetMouseX();
			int iMouseY = Mouse::Instance()->GetMouseY();

			RECT rt = m_pSlotChkBtn[i]->GetSizeRect();
			float fWidth  = (float)(rt.right - rt.left);
			float fHeigth = (float)(rt.bottom - rt.top);
			if(TRUE == bNoSel)
			{
				for (int a = 0; a < 4; a++)
				{
					color[a] = WzColor_RGBA(255,255,255,255);
				}
			}
			else
			{
				if(m_dwItemCode[i] == m_dwSelItemCode)
				{
					rt.top -= 2;
					for (int a = 0; a < 4; a++)
					{
						color[a] = WzColor_RGBA(255,255,255,255);
					}
				}
				else
				{
					for (int a = 0; a < 4; a++)
					{
						color[a] = WzColor_RGBA(255,255,255,100);
					}
				}
			}
			g_pSunRenderer->RenderTexture( m_hTexHandle, 
				(float)rt.left,
				(float)rt.top,
				fWidth,
				fHeigth,
				0,
				0,
				(float)ICON_IMAGE_SIZE,
				(float)ICON_IMAGE_SIZE,
				color);

			if (rt.left <= iMouseX && rt.right >= iMouseX)
			{
				if (rt.top <= iMouseY && rt.bottom >= iMouseY)
				{
					uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
					if(NULL == pUIMan)
						return;

					BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_dwItemCode[i]);
					if(NULL == pItemInfo)
						continue;

					SCSlotContainer* pAtContainer = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
                    if (!pAtContainer)
                    {
                        continue;
                    }

					if (INVALID_POSTYPE_VALUE == m_ChangedPosItem)
                    {
						continue;
                    }

					if (pAtContainer->IsEmpty(m_ChangedPosItem))
                    {
						continue;
                    }

					SCItemSlot& rItemSlot		   = (SCItemSlot&)pAtContainer->GetSlot( m_ChangedPosItem );

					pUIMan->RegisterTooltip_ItemStatChange(pItemInfo, &rItemSlot);	
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_dwSelItemCode = 0;
		m_bShow = TRUE;
		m_bSendPacket = FALSE;
	}
	else
	{
		if((TRUE == m_bShow) &&
		   (INVALID_POSTYPE_VALUE != m_ItemPosInInven) &&
		   (INVALID_POSTYPE_VALUE != m_ChangedPosItem) && 
		   (FALSE == m_bSendPacket))
		{
			//해당 아이템 잠금상태(반투명 처리)
			SCSlotContainer* pAtContainer  = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
			SCItemSlot& rItemSlot1		   = (SCItemSlot&)pAtContainer->GetSlot( m_ItemPosInInven );
			SCItemSlot& rItemSlot2		   = (SCItemSlot&)pAtContainer->GetSlot( m_ChangedPosItem );
			rItemSlot1.SetLock(FALSE);
			rItemSlot2.SetLock(FALSE);

			m_ItemPosInInven = INVALID_POSTYPE_VALUE;
			m_ChangedPosItem = INVALID_POSTYPE_VALUE;

			MSG_CG_ITEM_STAT_CHANGE_SELECT_SYN SendMsg;
			SendMsg.m_bSelect = FALSE;
			SendMsg.m_SelectItemCode = 0;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
		}

		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiItemStatChangeSys::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_ITEM_STAT_CHANGE_SYS_OK_BTN:
				{
					if(0 == m_dwSelItemCode)
					{
						//155233	선택된 아이템이 없습니다.
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						g_InterfaceManager.GetInterfaceString(155233, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						if((INVALID_POSTYPE_VALUE != m_ItemPosInInven) && (INVALID_POSTYPE_VALUE != m_ChangedPosItem))
						{
							//해당 아이템 잠금상태(반투명 처리)
							SCSlotContainer* pAtContainer  = (SCSlotContainer *)ItemManager::Instance()->GetContainer(SI_INVENTORY);
							SCItemSlot& rItemSlot1		   = (SCItemSlot&)pAtContainer->GetSlot( m_ItemPosInInven );
							SCItemSlot& rItemSlot2		   = (SCItemSlot&)pAtContainer->GetSlot( m_ChangedPosItem );
							rItemSlot1.SetLock(FALSE);
							rItemSlot2.SetLock(FALSE);
						}

						MSG_CG_ITEM_STAT_CHANGE_SELECT_SYN SendMsg;
						SendMsg.m_bSelect = TRUE;
						SendMsg.m_SelectItemCode = m_dwSelItemCode;
						if(GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg)))
							m_bSendPacket = TRUE;
						OnShowWindow(FALSE);
					}
				}
				break;

			case eDIALOG_ITEM_STAT_CHANGE_SYS_CANCEL_BTN:
			case eDIALOG_ITEM_STAT_CHANGE_SYS_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_1:
				{
					m_dwSelItemCode = m_dwItemCode[0];
				}
				break;

			case eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_2:
				{
					m_dwSelItemCode = m_dwItemCode[1];
				}
				break;

			case eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_3:
				{
					m_dwSelItemCode = m_dwItemCode[2];
				}
				break;

			case eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_4:
				{
					m_dwSelItemCode = m_dwItemCode[3];
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

void	uiItemStatChangeSys::InitControl()
{
	for(int i = 0; i < MAX_CHANGE_ITEMSTAT_SLOT_NUM; ++i)
	{
		m_pSlotChkBtn[i] = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_ITEM_STAT_CHANGE_SYS_SLOT_1 + i));
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
