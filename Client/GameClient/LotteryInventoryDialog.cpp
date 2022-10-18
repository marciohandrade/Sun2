
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
/** ��Ʈ�ѷ� �ʱ�ȭ
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
/**  �ǵ��� ������ �ϰ��̵�
*/
void			 
LotteryInventoryDialog::SendItemMoveAllSync()
{
	MSG_CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN SendPacket;
	SendPacket.m_InventoryPos       = m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));	
}
//----------------------------------------------------------------------------
/** �ǵ��� ���� �ݱ�
*/
void LotteryInventoryDialog::SendItemCloseSync()
{
	MSG_CG_ITEM_PANDORABOX_CLOSE_SYN SendPacket;
	SendPacket.m_InventoryPos = this->m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
}
//----------------------------------------------------------------------------
/** �ǵ��� ���� ����
*/
void
LotteryInventoryDialog::SendItemUseSync(BOOL bOpen)
{
	MSG_CG_ITEM_USE_PANDORA_ITEM_SYN SendPacket;
	SendPacket.m_InventoryPos = this->m_InvenPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
}
//----------------------------------------------------------------------------
/** �ǵ��� ���� ����
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
/**  �ζǾ����� ������������(���Թ����ٲ�)
*/
void LotteryInventoryDialog::RefreshLotteryItem(SCItemSlot &rcLottoSlot)
{
    SCSlotContainer * pInvenContainer	= g_pHero->GetSlotContainer( SI_INVENTORY);

    if(!pInvenContainer) return;

    SCItemSlot & rItemSlot = static_cast<SCItemSlot &>(pInvenContainer->GetSlot(m_InvenPos));

    //�����κ��� ���� �����̴� ��� �Ʒ��ִ³����� ���������ߵȴ� -_- ����;
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
/**  ���������鼭 ���۷������� ����
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
/** �ǵ��� ���ڻ���
*/
void
LotteryInventoryDialog::CreatePandoraSlot(ITEMSTREAMEX	&rItemInfo)
{
	if(!g_pHero) return;

    SCItemSlot slot;
    slot.Copy(rItemInfo);

    //�ζǾ������ϰ�츸�Ѵ�.
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

    RefreshLotteryItem(slot);//�κ����԰���
    RefreshPandoraSlot();
} 
//----------------------------------------------------------------------------
/** �ǵ��� ����������
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
				//GS_BUG_FIX����ǥ�������� ǥ�����
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
		
		// ���Կ� ���õ� ������ ǥ���ϱ�
		if (!IsEmpty( a ))
		{
			int islotcode = SCSlotContainer::GetSlot(a).GetCode();

			//����ǥ�����۸� ��¦�δ�.	
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
/** �޽���ó��
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
/** ���Ի���
*/
RC::eSLOT_INSERT_RESULT 
LotteryInventoryDialog::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:������ ����üũ ����(����)
    if (ValidPos(AtPos) == false)
    {
        return RC_INSERT_SLOT_FAILED;
    }

	// ���Կ� ����
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	// Ŭ�󿡼� ���� �ø����� �߱��Ѵ�.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// �κ��丮	
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
/** ��������
*/
BOOL 
LotteryInventoryDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code: ������ ����üũ ����(����)
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

	//safecode:�ٸ����� ����â�� ������������ ������ �ݾƹ�����.
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
	//���ϰ�츸 ����ȿ�� �ִ´�
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

			//�ǵ��� ���Ծ������� �κ����� �̵���Ų��.
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
			//�����¿����� �����Ѵ�.
			if(g_InterfaceManager.IsLockDlg())
			{
				POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
				
				switch(pos)
				{
				//�������� �ϰ��̵���Ų��.
				case LOTTERY_BTN_OK:
					{
						g_InterfaceManager.UnLockDlgs();
						
						//�κ��丮�� ������
                        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
                        if (inventory_manager != NULL)
                        {
                            inventory_manager->LockInventoryUI();
                        }

						//�ǵ��������� �ϰ��̵�
						MouseHandler::Instance()->RollbackItemAtHand();
						SendItemMoveAllSync();
						
					}
					break;
				
				//â�� �ݴ´ٴ� ��Ŷ�� �������� �����ش�.
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
