
#include "SunClientPrecompiledHeader.h"

#include "LotteryInventoryDialog.h"
#include "SlotKeyGenerator.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "SlotKeyGenerator.h"
#include "MouseHandler.h"
#include "InterfaceManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "KeyQueueManager.h"
#include "Application.h"
#include "CastLeaveFieldDialog.h"
#include "CursorChangeTransact.h"
#include "InventoryDialog.h"
#include "ItemManager.h"
#include "LotteryInfoParser.h"
#include <SCItemSlot.h>
//----------------------------------------------------------------------------
/** 
*/
WzID LotteryInventoryDialog::m_wzId[LOTTERY_MAX] = 
{ 
	StrToWzID("BT00"),//LOTTERY_SLOT_00
	StrToWzID("BT01"),//LOTTERY_SLOT_01
	StrToWzID("BT02"),//LOTTERY_SLOT_02
	StrToWzID("BT03"),//LOTTERY_SLOT_03
	StrToWzID("BT04"),//LOTTERY_SLOT_04
	StrToWzID("BT05"),//LOTTERY_SLOT_05
	StrToWzID("BT06"),//LOTTERY_SLOT_06
	StrToWzID("BT07"),//LOTTERY_SLOT_07

	StrToWzID("ST00"),//LOTTERY_TEXT
	StrToWzID("BT08"),//LOTTERY_BTN_OK
	StrToWzID("BT09"),//LOTTERY_BTN_CLOSE
	
};
//----------------------------------------------------------------------------
/**
*/
LotteryInventoryDialog::LotteryInventoryDialog(InterfaceManager * pUIMan)  
: uiBase(pUIMan),
  m_pItemUnitRenderer(NULL),
  m_pCtrlOkBtn(NULL),
  m_pCtrlCloseBtn(NULL)
{
	SCItemSlotContainer::Init( MAX_PANDORA_SLOT_NUM, SI_LOTTERY );
}	
//----------------------------------------------------------------------------
/**
*/
LotteryInventoryDialog::~LotteryInventoryDialog()
{
	this->Release();
}
//----------------------------------------------------------------------------
/**
*/
VOID	
LotteryInventoryDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	
	m_pDraw = pDrawBase;

	
	InitRenderer();
	InitControls();


}
//----------------------------------------------------------------------------
/** 컨트롤러 초기화
*/
void	
LotteryInventoryDialog::InitControls()
{
	m_pCtrlOkBtn	=	static_cast<CCtrlButtonWZ*>(getControl(LOTTERY_BTN_OK));
	m_pCtrlCloseBtn	=	static_cast<CCtrlButtonWZ*>(getControl(LOTTERY_BTN_CLOSE));
	m_TxtCtrlTitle =	static_cast<CCtrlStaticWZ*>(getControl(LOTTERY_TEXT));

}
//----------------------------------------------------------------------------
/**
*/
void
LotteryInventoryDialog::InitRenderer()
{
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( m_pDraw, GetMaxSlotNum() );
	m_pItemUnitRenderer->SetOverLap(FALSE);
}
//----------------------------------------------------------------------------
/**  판도라 아이템 일괄이동
*/
void			 
LotteryInventoryDialog::SendItemMoveAllSync()
{
	MSG_CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN SendPacket;
	SendPacket.m_InventoryPos       = m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));	
}
//----------------------------------------------------------------------------
/** 판도라 상자 닫기
*/
void LotteryInventoryDialog::SendItemCloseSync()
{
	MSG_CG_ITEM_PANDORABOX_CLOSE_SYN SendPacket;
	SendPacket.m_InventoryPos = this->m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
}
//----------------------------------------------------------------------------
/** 판도라 상자 오픈
*/
void
LotteryInventoryDialog::SendItemUseSync(BOOL bOpen)
{
	MSG_CG_ITEM_USE_PANDORA_ITEM_SYN SendPacket;
	SendPacket.m_InventoryPos = this->m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
}
//----------------------------------------------------------------------------
/** 판도라 상자 오픈
*/
void			 
LotteryInventoryDialog::SetLotteryOpenStatus(BOOL bOpen)
{
	SCSlotContainer * pInvenContainer	= g_pHero->GetSlotContainer( SI_INVENTORY);

	if(!pInvenContainer) return;
	
	SCItemSlot & rItemSlot = static_cast<SCItemSlot &>(pInvenContainer->GetSlot(m_InvenPos));

	SCLottoInfo* pSlotLottoInfo= rItemSlot.GetLottoInfo();

	if (pSlotLottoInfo)
    {
        pSlotLottoInfo->SetOpenStatus(bOpen);
    }
}

//----------------------------------------------------------------------------
/**  로또아이템 슬롯정보갱신(슬롯버전바뀜)
*/
void LotteryInventoryDialog::RefreshLotteryItem(SCItemSlot &rcLottoSlot)
{
    SCSlotContainer * pInvenContainer	= g_pHero->GetSlotContainer( SI_INVENTORY);

    if(!pInvenContainer) return;

    SCItemSlot & rItemSlot = static_cast<SCItemSlot &>(pInvenContainer->GetSlot(m_InvenPos));

    //서버로부터 받은 원본이다 요골 아래있는놈한테 복사시켜줘야된다 -_- 복잡;
    SCLottoInfo* pSrcSlot= rcLottoSlot.GetLottoInfo();

    SCLottoInfo* pDestSlot= rItemSlot.GetLottoInfo();

    if(!pDestSlot) return;

    pDestSlot->SetCount(pSrcSlot->GetCount());
    pDestSlot->SetOpenStatus(TRUE);
    pDestSlot->SetLottoIndex(pSrcSlot->GetLottoIndex());
    pDestSlot->SetIdentityStatus(pSrcSlot->GetIdentityStatus());

    for(int i=0;i<MAX_PANDORA_SLOT_NUM;++i)
    {
        MiniSlot srcMiniSlot = pSrcSlot->GetMiniSlot(i);
        MiniSlot TempMiniSlot;
        TempMiniSlot.ExistItem = srcMiniSlot.ExistItem;
        TempMiniSlot.InfoPos   = srcMiniSlot.InfoPos;
        TempMiniSlot.Random    = srcMiniSlot.Random;
        pDestSlot->SetMiniSlot(i,TempMiniSlot);
    }

}

//----------------------------------------------------------------------------
/**  슬롯을돌면서 아템랜더정보 삽입
*/
void
LotteryInventoryDialog::RefreshPandoraSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for(int i=0;i<GetMaxSlotNum();++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if(!IsEmpty(i))
		{
			if(!m_pItemUnitRenderer) continue;

			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

			if (pRender == NULL)
			{
				pCtrl = getControl( i );
				rcSlot = pCtrl->GetSizeRect();
				m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}
            
            if (pRender)
            {
			    pRender->SetShow(TRUE);
            }
            
		}

	}
}
//----------------------------------------------------------------------------
/** 판도라 상자생성
*/
void
LotteryInventoryDialog::CreatePandoraSlot(ITEMSTREAMEX	&rItemInfo)
{
	if(!g_pHero) return;

    SCItemSlot slot;
    slot.Copy(rItemInfo);

    //로또아이템일경우만한다.
    if(!slot.IsLottoItem())
    {
        return;
    }
    
    SCLottoInfo* scLottoInfo = slot.GetLottoInfo();

    if(!scLottoInfo)
    {
        return;
    }

    const LotteryIndexGroup*  pLotteryGroup =  LotteryInfoParser::Instance()->GetLotteryIndexGroup(scLottoInfo->GetLottoIndex());

    if(!pLotteryGroup) return; 

    for(int i=0;i<MAX_PANDORA_SLOT_NUM;++i)
    {
        this->DeleteSlot(i,NULL);

        MiniSlot minislot = scLottoInfo->GetMiniSlot(i);

        if(!minislot.ExistItem) continue;		

        const MiniSlotInfo* Minislotinfo =  pLotteryGroup->GetMiniSlotInfo(minislot.InfoPos);
        if (Minislotinfo == NULL)
        {
            continue;
        }

        SCItemSlot ItemSlot;

        ItemSlot.SetSlotType(ST_ITEM);

        SLOTCODE ItemCode = (SLOTCODE)(Minislotinfo->Random ? (7270):(Minislotinfo->Index));

        ItemSlot.SetCode(ItemCode);

        this->InsertSlot( i, ItemSlot);

    }

    RefreshLotteryItem(slot);//인벤슬롯갱신
    RefreshPandoraSlot();
} 
//----------------------------------------------------------------------------
/** 판도라 툴팁랜더링
*/
void			 
LotteryInventoryDialog::renderToolTip()
{	
	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			if(pUIMan && m_MouseOverObject.pSlot)
			{
				//GS_BUG_FIX물음표아이템은 표기안함
				if(m_MouseOverObject.pSlot->GetCode()!=7270)
				{
					pUIMan->InitTooltip();
					pUIMan->SetTooltipDialogID(GetDialogKey());
					pUIMan->RegisterPandoraItemName(static_cast<SCItemSlot*>( m_MouseOverObject.pSlot ));
				}
			}

		}
	}
	
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < MAX_PANDORA_SLOT_NUM; a++)
	{
		CControlWZ * pControl = getControl(a);
		
		// 슬롯에 선택된 아이템 표시하기
		if (!IsEmpty( a ))
		{
			int islotcode = SCSlotContainer::GetSlot(a).GetCode();

			//물음표아이템만 반짝인다.	
			if(SCSlotContainer::GetSlot(a).GetCode()==7270)
			{
				CControlWZ* pControl = this->getControl(a);
				RECT rt = pControl->GetSizeRect();
				g_InterfaceManager.RenderActivateMark( rt.left - 13, rt.top - 13);
			}
		}

		if(pControl && bMouseOver==FALSE)
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
		return;
	}


}
//----------------------------------------------------------------------------
/** 메시지처리
*/
VOID			
LotteryInventoryDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
						
			if (pos < this->GetMaxSlotNum() && 
				pos >= 0 && pos < MAX_PANDORA_SLOT_NUM)
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
/** 슬롯삽입
*/
RC::eSLOT_INSERT_RESULT 
LotteryInventoryDialog::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC_INSERT_SLOT_FAILED;
    }

	// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	// 클라에서 따로 시리얼을 발급한다.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// 인벤토리	
	CControlWZ * pCtrl = getControl(AtPos);
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	return eInsertSlotResult;
}
//----------------------------------------------------------------------------
/** 슬롯제거
*/
BOOL 
LotteryInventoryDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code: 안전한 슬롯체크 구문(삭제)
    if ((ValidPos(AtPos) == false) ||
        IsEmpty(AtPos))
    {
        return FALSE;
    }

	SCItemSlot & rItemSlot = (SCItemSlot & )GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rItemSlot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	g_SlotKeyGenerator.Restore((DWORD)rItemSlot.GetSerial());

	SCSlotContainer::DeleteSlot(AtPos, pSlotOut );
	
	return TRUE;

}
//----------------------------------------------------------------------------
/**
*/
void
LotteryInventoryDialog::OnUpdateSolarDialog()
{
	if(m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();

	//safecode:다른락이 복권창을 덮어버렸을경우 강제로 닫아버린다.
	if(!g_InterfaceManager.IsLockDlg())
	{
		this->ShowInterface(FALSE);
	}
	
}
//----------------------------------------------------------------------------
/**
*/
void
LotteryInventoryDialog::SetPandoraBoxName()
{
	if(!m_TxtCtrlTitle) return;

	SCSlotContainer * pInvenContainer	= g_pHero->GetSlotContainer( SI_INVENTORY);

	if(!pInvenContainer) return;

	SCItemSlot & rItemSlot = static_cast<SCItemSlot &>(pInvenContainer->GetSlot(m_InvenPos));
		
	const BASE_ITEMINFO *	 pItemInfo =  rItemSlot.GetItemInfo();

	if(!pItemInfo) return;
	
	TCHAR   szName[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString( pItemInfo->m_NCode, szName, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption(m_TxtCtrlTitle, szName);
}

//----------------------------------------------------------------------------
/**
*/
void
LotteryInventoryDialog::OnRenderSolarDialog()
{
	//락일경우만 암전효과 넣는다
	if(g_InterfaceManager.IsLockDlg())
	{
		POINT pt = g_pApplication->Get2DModeMapping();
		RECT rc;
		SetRect(&rc,0,0,pt.x,pt.y);
		g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128),0,0,(float)rc.right,(float)rc.bottom);
		GetDialogWZ()->OnDraw(true);
		
	}
	if(m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render(); 
	
	renderToolTip();
}
//----------------------------------------------------------------------------
/**
*/
void 
LotteryInventoryDialog::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
		{
			DeleteSlot(i, NULL);
		}
	}
}	
//----------------------------------------------------------------------------
/**
*/
VOID	 
LotteryInventoryDialog::Release()
{
	FlushUnitDraw();
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	SCItemSlotContainer::Release();
}
//---------------------------------------------------------------------------- 
/** 
*/
SCSlotContainer* 
LotteryInventoryDialog::GetSlotContainer()
{
	return static_cast<SCSlotContainer *>(this);
}

//---------------------------------------------------------------------------- 
/** 
*/
VOID	
 LotteryInventoryDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			//판도라 슬롯아이템을 인벤으로 이동시킨다.
			if( pos < MAX_PANDORA_SLOT_NUM ) 
			{
				if(IsEmpty(pos)) break;
				
				MSG_CG_ITEM_MOVE_PANDORA_ITEM_SYN SendPacket;
				SendPacket.m_InventoryPos		= m_InvenPos;
				SendPacket.m_PandoraItemPosFrom = pos;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
			}
		}
		break;
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{

		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			//락상태에서만 반응한다.
			if(g_InterfaceManager.IsLockDlg())
			{
				POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
				
				switch(pos)
				{
				//아이템을 일괄이동시킨다.
				case LOTTERY_BTN_OK:
					{
						g_InterfaceManager.UnLockDlgs();
						
						//인벤토리는 락걸자
                        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
                        if (inventory_manager != NULL)
                        {
                            inventory_manager->LockInventoryUI();
                        }

						//판도라이이템 일괄이동
						MouseHandler::Instance()->RollbackItemAtHand();
						SendItemMoveAllSync();
						
					}
					break;
				
				//창을 닫는다는 패킷을 서버에게 보내준다.
				case LOTTERY_BTN_CLOSE:
					{
						g_InterfaceManager.UnLockDlgs();
						this->ShowInterface(FALSE);
						SendItemCloseSync();
					}
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
