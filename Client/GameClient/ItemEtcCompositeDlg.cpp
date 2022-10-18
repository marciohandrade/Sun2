#include "SunClientPrecompiledHeader.h"	

#include "ItemETCCompositeDlg.h"
#include <SCSlotContainer.h>
#include "InterfaceManager.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "dummyitemSlot.h"
#include "ObjectManager.h"
#include "ChatDialog.h"
#include "ItemCompositeParser.h"
#include "SlotKeyGenerator.h"
#include "hero.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#define ACTIVE_COLOR	RGBA(0,255,0,255)
#define DEACTIVE_COLOR	RGBA(255,0,0,255) 


WzID ItemETCCompositeDlg::m_wzId[] = 
{
	StrToWzID("B000"), // eITEMENCHANT_BT_SLOT_1 = 0,
	StrToWzID("B001"), // eITEMENCHANT_BT_SLOT_2,
	StrToWzID("B002"), // eITEMENCHANT_BT_SLOT_3,
	StrToWzID("B003"), // eITEMENCHANT_BT_SLOT_4,
	StrToWzID("B004"), // eITEMENCHANT_BT_SLOT_5,
	StrToWzID("B005"), // eITEMENCHANT_BT_SLOT_6,
	StrToWzID("TI01"), // eITEMENCHANT_TI_ENCHANT,
	StrToWzID("TI02"), // eITEMENCHANT_TI_RETRY,
	StrToWzID("TI03"), // eITEMENCHANT_TI_EXIT,
	StrToWzID("T009"), // eITEMENCHANT_VALUE_MONEY
	StrToWzID("T002"), // eITEMENCHANT_VALUE_KINDOFENCHANT,
	StrToWzID("T005"), // eITEMENCHANT_VALUE_SUB_1_MATERIAL,
	StrToWzID("T006"), // eITEMENCHANT_VALUE_SUB_2_MATERIAL,
	StrToWzID("T007"), // eITEMENCHANT_VALUE_SUB_3_MATERIAL,
	StrToWzID("T008"), // eITEMENCHANT_VALUE_SUB_4_MATERIAL,
	StrToWzID("T010"), // eITEMENCHANT_VALUE_SUB_5_MATERIAL,	

	StrToWzID("B999"), // ITEMENCHANT_BUTTON_EXIT
};

//------------------------------------------------------------------------------
/**
*/
ItemETCCompositeDlg::ItemETCCompositeDlg(void) 
: m_pItemUnitRenderer(0)
, m_DialogKey(0)
{
	SCItemSlotContainer::Init(eITEMENCHANT_MAX_SLOT, SI_ITEM_ETC_COMPOSE );
}

//------------------------------------------------------------------------------
/**
*/
ItemETCCompositeDlg::~ItemETCCompositeDlg(void)
{
}


//------------------------------------------------------------------------------
/**
*/
VOID	
ItemETCCompositeDlg::Init( CDrawBase * pDrawBase )
{
	m_eCurCompositeType = eITEMCOMPOSITE_NONE;

	RECT rcDlg;
	GetDialogWZ()->GetDlgSize(&rcDlg);

	int iDeltaX = (rcDlg.right) / 2;
	int iDeltaY = (rcDlg.bottom) / 2;
	int x, y;
	x = g_InterfaceManager.GetWindowRC()->right / 2 - iDeltaX;
	y = g_InterfaceManager.GetWindowRC()->bottom / 2 - iDeltaY;

	MoveWindowWZ(x, y);	

	InitContainer();

	// control 받음
	m_CtrlKindOfCompositeStatic = (CCtrlStaticWZ *)getControl(eITEMENCHANT_VALUE_KINDOFENCHANT);
	assert(m_CtrlKindOfCompositeStatic);

	int i = 0;
	for (; i < eITEMENCHANT_MAX_SUB_SLOT; ++i)
	{
		m_CtrlSubMaterialStatic[i] = (CCtrlStaticWZ *)getControl(eITEMENCHANT_VALUE_SUB_1_MATERIAL+i);
		assert(m_CtrlSubMaterialStatic[i]);
	}

	m_CtrlMoneyStatic = (CCtrlStaticWZ *)getControl(eITEMENCHANT_VALUE_MONEY);
	assert(m_CtrlMoneyStatic);
}

//------------------------------------------------------------------------------
/**
*/
VOID	
ItemETCCompositeDlg::Release()
{
	ReleaseContainer();
}


//------------------------------------------------------------------------------
/**
*/
VOID	
ItemETCCompositeDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		initEnchantInfo();
		initEnchantInfoText();
		showEnchantTitleText();

		fillEnchantExtraInfo();
		ItemManager::Instance()->AutoRegisterCompositeMaterials( m_eCurCompositeType );
		activateMaterials(FALSE);

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SELECT_COMPOSITE_MENU, FALSE);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_enchant;
		msg.wParam=InterfaceManager::DIALOG_ENCHANT;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

	}
	else
	{
		FlushUnitDraw();

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_enchant);
		this->m_packetStatus = PACKET_STATUS_NONE;
	}
}


//------------------------------------------------------------------------------
/**
*/
void				
ItemETCCompositeDlg::initEnchantInfo()
{
	m_bValidEnchant	= FALSE;
	this->m_FromTargetPos		= 0;
	this->m_FromTargetSlotIdx	= SI_MAX; 
}


//------------------------------------------------------------------------------
/**
*/
void
ItemETCCompositeDlg::initEnchantInfoText()
{
	int i = 0;
	for (; i< 4; ++i)
	{
		if (m_CtrlSubMaterialStatic[i])
			m_CtrlSubMaterialStatic[i]->SetTextWZ(_T(""));
	}

	if (m_CtrlMoneyStatic)
		m_CtrlMoneyStatic->SetTextWZ(_T("0"));
}


//------------------------------------------------------------------------------
/**
*/
void
ItemETCCompositeDlg::showEnchantTitleText()
{
	int iCodeTitleString = 0;
	TCHAR szTemp[512];
	TCHAR szTemp2[512];

	switch (this->m_eCurCompositeType)
	{
	case eITEMCOMPOSITE_LOW_ENCHANT:
	case eITEMCOMPOSITE_MEDIUM_ENCHANT:
	case eITEMCOMPOSITE_NONE:
		{
			assert(0);
		}
		break;
	case eITEMCOMPOSITE_HIGH_ENCHANT:	
		{
			iCodeTitleString = 862;
			g_InterfaceManager.GetInterfaceString(iCodeTitleString, m_szBuf, 512);
		}
		break;

	case eITEMCOMPOSITE_RANK_DOWN:
		{
			iCodeTitleString = 864;
			g_InterfaceManager.GetInterfaceString(iCodeTitleString, m_szBuf, 512);
		}
		break;

	case eITEMCOMPOSITE_ADD_SOCKET:		
		{
			iCodeTitleString = 865;
			g_InterfaceManager.GetInterfaceString(iCodeTitleString, m_szBuf, 512);
		}
		break;

	case eITEMCOMPOSITE_EXTRACT_SOCKET:
		{
			iCodeTitleString = 866;
			g_InterfaceManager.GetInterfaceString(iCodeTitleString, m_szBuf, 512);
		}
		break;

	case eITEMCOMPOSITE_LOW_RING:
		{
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_LOW, szTemp, 512);
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_RING, szTemp2, 512);
			Sprintf(m_szBuf, _T("%s %s"), szTemp, szTemp2);
		}
		break;
	case eITEMCOMPOSITE_HIGH_RING:
		{
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_HIGH, szTemp, 512);
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_RING, szTemp2, 512);
			Sprintf(m_szBuf, _T("%s %s"), szTemp, szTemp2);
		}
		break;
	case eITEMCOMPOSITE_LOW_NECKLACE:
		{
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_LOW, szTemp, 512);
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_NECKLACE, szTemp2, 512);
			Sprintf(m_szBuf, _T("%s %s"), szTemp, szTemp2);
		}
		break;
	case eITEMCOMPOSITE_HIGH_NECKLACE:
		{	
			// 악세서리 생성 
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_HIGH, szTemp, 512);
			g_InterfaceManager.GetInterfaceString(eST_RANKMENU_INFO_NECKLACE, szTemp2, 512);
			Sprintf(m_szBuf, _T("%s %s"), szTemp, szTemp2);
		}
		break;
	}

	if (m_CtrlKindOfCompositeStatic)
	{
		GlobalFunc::SetCaption( m_CtrlKindOfCompositeStatic, m_szBuf );
	}
}


//------------------------------------------------------------------------------
/**
*/
void				
ItemETCCompositeDlg::fillEnchantExtraInfo()
{
	// TODO: 이곳에 보여줘야할 정보를 채워라.

	// 주 재료 정보
	int iMainMaterialStringIndex	= 0;
	int iMainMaterialStringIndex2	= 0;
	switch (this->m_eCurCompositeType)
	{
	case eITEMCOMPOSITE_LOW_ENCHANT:
	case eITEMCOMPOSITE_MEDIUM_ENCHANT:
	case eITEMCOMPOSITE_HIGH_ENCHANT:
		{
			iMainMaterialStringIndex = 749;	// 무기/ 방어구
		}
		break;

	case eITEMCOMPOSITE_RANK_DOWN:
		{
			iMainMaterialStringIndex	= 749;		// 무기/ 방어구
			iMainMaterialStringIndex2	= 750;		// 악세서리 
		}
		break;

	case eITEMCOMPOSITE_EXTRACT_SOCKET:
		{
			// 옵션제거
			iMainMaterialStringIndex	= 749;		// 무기 / 방어구
			iMainMaterialStringIndex2	= 750;		// 악세서리
		}
		break;

	case eITEMCOMPOSITE_ADD_SOCKET:
		{
			iMainMaterialStringIndex	= 749;
		}
		break;
	
	case eITEMCOMPOSITE_LOW_RING:
	case eITEMCOMPOSITE_LOW_NECKLACE:
		{
			iMainMaterialStringIndex	= 40001;	// 하급쟈드

		}
		break;
	case eITEMCOMPOSITE_HIGH_RING:
	case eITEMCOMPOSITE_HIGH_NECKLACE:
		{
			iMainMaterialStringIndex	= 40002;	// 상급쟈드
		}
		break;

	default:
		break;
	}
	
	if (iMainMaterialStringIndex)
	{
		g_InterfaceManager.GetInterfaceString( iMainMaterialStringIndex, m_szBuf, 512);

		Sprintf(m_szResult, _T("%s"), m_szBuf);
	}

	if (iMainMaterialStringIndex2)
	{
		g_InterfaceManager.GetInterfaceString( iMainMaterialStringIndex, m_szBuf2, 512);

		Sprintf(m_szResult, _T("%s | %s"), m_szBuf, m_szBuf2);
	}

	sITEMCOMPOSITE * pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(this->m_eCurCompositeType);
	CODETYPE code = 0;
	
	if (pInfo)
	{
		for (int i = 0; i < pInfo->m_byMatCount; ++i)
		{
			if (m_CtrlSubMaterialStatic[i])
			{
				code = pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode;
				BASE_ITEMINFO * pBaseItemInfo = (BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(code);

				if (pBaseItemInfo)
				{
					g_InterfaceManager.GetItemName(code, this->m_szBuf, 512);

					GlobalFunc::SetCaption( m_CtrlSubMaterialStatic[i], this->m_szBuf );
				}				
			}
		}
	}

	SetRequiredMoney(pInfo->m_Money);
}

//-------------------------------------------------------------------------------------------
/**
*/
void				
ItemETCCompositeDlg::updateMaterial()
{
	sITEMCOMPOSITE * pInfo = 
		ItemCompositeParser::Instance()->GetCompositeInfo(this->GetCurrentCompositeType());
	if ( !pInfo )
	{
		return;
	}

	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
	{
	
		BOOL bRet = ItemManager::Instance()->IsExistItem(
						SI_INVENTORY,
						pInfo->m_sCompositeMaterials[i].m_dwCompositeMaterialCode,
						pInfo->m_sCompositeMaterials[i].m_bySpendMaterialNum);		

		eSLOT_RENDER_STATE eState		= (bRet)? SLOT_RENDER_STATE_ACTIVATED : SLOT_RENDER_STATE_WARNING;
		DWORD dwColor	= (bRet)? ACTIVE_COLOR : DEACTIVE_COLOR;
		POSTYPE SubMatPos = i+1;

		if ( !IsEmpty(SubMatPos) )
		{
			SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(SubMatPos);

			if(!m_pItemUnitRenderer)
				continue;

			ItemUnitRenderer::RenderUnit * pRedner = 
				this->m_pItemUnitRenderer->GetItemUnit( rItemSlot.GetSerial() );

			if (pRedner)
			{
				pRedner->SetState( eState );
			}
				
			if ( m_CtrlSubMaterialStatic[i] )
			{
				this->m_CtrlSubMaterialStatic[i]->SetTextColor(dwColor);
			}
		}
	}

	if (g_pHero)
	{
		DWORD dwColor = ( g_pHero->GetMoney() >= this->m_RequireMoney )? ACTIVE_COLOR : DEACTIVE_COLOR;
		this->m_CtrlMoneyStatic->SetTextColor(dwColor);
	}

}


//------------------------------------------------------------------------------
/**
*/
void ItemETCCompositeDlg::activateMaterials(BOOL bActivate)
{
}


//------------------------------------------------------------------------------
/**
*/
VOID				
ItemETCCompositeDlg::Enchant()
{
	switch (this->m_eCurCompositeType)
	{
	case eITEMCOMPOSITE_RANK_DOWN:
		{
			
		}
		break;

	case eITEMCOMPOSITE_ADD_SOCKET:		
		{
	
		}
		break;

	case eITEMCOMPOSITE_EXTRACT_SOCKET:
		{
			if (PACKET_STATUS_NONE != this->m_packetStatus )
			{
				return;
			}

			BIT_ADD(this->m_packetStatus, PACKET_STATUS_EXTRACT);

			MSG_CG_ITEM_SOCKET_EXTRACT_SYN SendPacket;
			SendPacket.m_TargetPos = m_FromTargetPos;

			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

		}
		break;
	}
}


//------------------------------------------------------------------------------
/**
*/
VOID				
ItemETCCompositeDlg::ReTry()
{
	if ( this->m_packetStatus != PACKET_STATUS_NONE )
	{
		return;
	}

	initEnchantInfo();
	FlushUnitDraw();

	ItemManager::Instance()->AutoRegisterCompositeMaterials( m_eCurCompositeType );	
}


//------------------------------------------------------------------------------
/**
*/
VOID			
ItemETCCompositeDlg::Exit()
{
	if ( this->m_packetStatus != PACKET_STATUS_NONE )
	{
		return;
	}

	initEnchantInfo();
	initEnchantInfoText();
	g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_ITEM_ETC_COMPOSE, FALSE);
}


//------------------------------------------------------------------------------
/**
*/
VOID				
ItemETCCompositeDlg::SetRequiredMoney(MONEY Money)
{
	if (m_CtrlMoneyStatic)
	{
		TCHAR szMoney[0xff];
		_ui64tot(Money, szMoney, 10);
		GlobalFunc::SetCaption( m_CtrlMoneyStatic, szMoney );
		m_RequireMoney = Money;
	}
}


//------------------------------------------------------------------------------
/**
*/
VOID				
ItemETCCompositeDlg::SetTargetInfo(SLOTIDX fromSlotIdx, POSTYPE fromPos)
{
	m_FromTargetSlotIdx = SI_INVENTORY;
	m_FromTargetPos		= fromPos;
}


//------------------------------------------------------------------------------
/**
*/
VOID	
ItemETCCompositeDlg::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
		
			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			if ( eITEMENCHANT_MAIN_MATERIAL <= pos && eITEMENCHANT_MAX_SLOT > pos)
			{
				if (eITEMENCHANT_MAIN_MATERIAL != pos )
				{
					if (MouseHandler::Instance()->IsExistItemAtHand())
					{
						MouseHandler::Instance()->RollbackItemAtHand();
						return;
					}

					return;
				}

				RECT rcSlot = pControl->GetSizeRect();
				BOOL bRet = MouseHandler::Instance()->ItemCompositeEnchants(SI_ITEM_ETC_COMPOSE, pos, &rcSlot);
				
				MouseHandler::Instance()->RollbackItemAtHand();

				if (bRet)
				{
					m_bValidEnchant = TRUE;
					activateMaterials(TRUE);
				}
				else
				{
					m_bValidEnchant = FALSE;
				}
			}
			else
			{
				switch (pos)
				{					
				case eITEMENCHANT_TI_ENCHANT:
					Enchant();
					break;
				case eITEMENCHANT_TI_RETRY:
					ReTry();
					break;
				case eITEMENCHANT_TI_EXIT:
					Exit();
					break;
				case eITEMENCHANT_BT_EXIT:
					Exit();
					break;

				default:
					break;
				}
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
VOID	
ItemETCCompositeDlg::OnUpdateSolarDialog()
{
    if( m_pItemUnitRenderer )
    {
	    m_pItemUnitRenderer->Update();

	    this->updateMaterial();
    }
}


//------------------------------------------------------------------------------
/**
*/
VOID	
ItemETCCompositeDlg::OnRenderSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	for (POSTYPE a = 0; a < eITEMENCHANT_MAX_SLOT; a++)
	{
		CControlWZ *pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				POSTYPE slotpos = a;

				SCSlot & rSlot = (SCSlot &)GetSlot( slotpos );
				if (rSlot.GetCode() != 0)
				{
					uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
					if(pUIMan)
					{
						pUIMan->RegisterSlotInfoTooltip(&rSlot);
						pUIMan->SetTooltipDialogID(GetDialogKey());
					}
				}

			}
		}

	}
}


//------------------------------------------------------------------------------
/**
*/
CControlWZ *	
ItemETCCompositeDlg::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


//------------------------------------------------------------------------------
/**
*/
POSTYPE		
ItemETCCompositeDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemETCCompositeDlg::InitContainer()
{
	InitRenderer();	
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemETCCompositeDlg::ReleaseContainer()
{
	FlushUnitDraw();

	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemETCCompositeDlg::InitRenderer()
{
	assert( !m_pItemUnitRenderer );

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemETCCompositeDlg::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		DeleteSlot(i, NULL);
	}
}


//------------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		ItemETCCompositeDlg::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	//// 락킹 온.
	if ( eITEMENCHANT_MAIN_MATERIAL == AtPos )
	{
		SCItemSlot & rFromItemSlot = (SCItemSlot &)rSlot;
		rFromItemSlot.SetLock( TRUE );

		m_LockingInfoMap.insert( LOCKING_INFO_PAIR(AtPos, rSlot.GetPos() ) );
	}

	/// 인벤토리
	CControlWZ * pCtrl = getControl( AtPos );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot ,pCtrl);
	}
	else
	{ 
		assert(m_pItemUnitRenderer);
	}

	return eInsertSlotResult;
}


//------------------------------------------------------------------------------
/**
*/
BOOL
ItemETCCompositeDlg::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	SCSlot & rSlot = GetSlot(AtPos);
	
	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	LOCKING_INFO_ITR itr = m_LockingInfoMap.find( AtPos );
	if ( itr != m_LockingInfoMap.end() )
	{
		POSTYPE FromPos = itr->second;
		// 현재는 무조건 인벤토리에서 올수 있다.
		SCSlotContainer * pFromContainer = 
			ItemManager::Instance()->GetContainer( SI_INVENTORY );
		if ( pFromContainer )
		{
			if ( FromPos < pFromContainer->GetMaxSlotNum() && FromPos >= 0)
			{
				SCItemSlot & rItemSlot = (SCItemSlot & )pFromContainer->GetSlot(FromPos);			
				rItemSlot.SetLock(FALSE);
			}			
			else
			{
				assert( !"잘못된 아이템 위치 이다.!!!");
			}
		}

		m_LockingInfoMap.erase(itr);
	}

	return SCSlotContainer::DeleteSlot(AtPos, pSlotOut);
}


//------------------------------------------------------------------------------
/**
*/
ItemUnitRenderer * 
ItemETCCompositeDlg::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}


//------------------------------------------------------------------------------
/**
*/
VOID 
ItemETCCompositeDlg::SetCurrentCompositeType(eITEMCOMPOSITECODE eCompositeType)
{
	this->m_eCurCompositeType = eCompositeType;
}


eITEMCOMPOSITECODE 
ItemETCCompositeDlg::GetCurrentCompositeType(void)
{
	return this->m_eCurCompositeType;
}