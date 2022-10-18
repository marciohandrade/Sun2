\
#include "SunClientPrecompiledHeader.h"

#include "RiderInventoryDialog.h"
#include "SlotKeyGenerator.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"
#include "GameFunc.h"
#include "Hero.h"
#include "SlotKeyGenerator.h"
#include "MouseHandler.h"
#include "InterfaceManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "InventoryDialog.h"
#include "ItemManager.h"
#include "Rider.h"
#include "RidingInfoParser.h"
#include <SCItemSlot.h>

WzVector l_vNormal ={0.0f,0.0f,1.0f};

//----------------------------------------------------------------------------
/**
*/
RiderInventoryDialog::RiderInventoryDialog(InterfaceManager * pUIMan)  
: uiBase(pUIMan),
  m_pItemUnitRenderer(NULL),
  m_pDisplayRider(NULL),
  m_bSlotRider(FALSE),
  m_fScale(1.f),
  m_fAngleZ(0.3f),
  m_eRotateType(R_ROTATE_NONE),
  m_iTooltipStringCode(0),
  m_pCtrlCloseBtn(NULL)
{
    m_pRotateBtn[0]=NULL;
    m_pRotateBtn[1]=NULL;
	m_UseTooltipCtrls.clear();
    m_UseTooltipCtrls[RIDER_SLOT_01] = 12112;
    m_UseTooltipCtrls[RIDER_SLOT_02] = 12113;
    m_UseTooltipCtrls[RIDER_SLOT_03] = 12114;
}	
//----------------------------------------------------------------------------
/**
*/
RiderInventoryDialog::~RiderInventoryDialog()
{
   DeleteViewRider();
   m_UseTooltipCtrls.clear();
   this->Release();
}
//----------------------------------------------------------------------------
/**
*/
POSTYPE          
RiderInventoryDialog::GetRiderSummonPos()
{
    if(IsSlotRider())
    {
        return m_SummmonPos;
    }
    
    return g_pHero->m_RiderInfo.m_RiderItemPos;
}
//----------------------------------------------------------------------------
/**
*/
VOID	
RiderInventoryDialog::Init( CDrawBase * pDrawBase )
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
RiderInventoryDialog::InitControls()
{
	m_pCtrlCloseBtn=static_cast<CCtrlButtonWZ*>(getControl(RIDER_BTN_EXIT));//닫기
	m_TxtCtrlTitle =static_cast<CCtrlStaticWZ*>(getControl(RIDER_BTN_TEXT));//라이더명
    
    m_pRotateBtn[0]=static_cast<CCtrlButtonWZ*>(getControl(RIDER_BTN_LEFT));//왼쪽
    m_pRotateBtn[1]=static_cast<CCtrlButtonWZ*>(getControl(RIDER_BTN_RIGHT));//오른쪽


}
//------------------------------------------------------------------------------
int	RiderInventoryDialog::getTooltipString(POSTYPE pos)
{
    USE_TOOLTIP_CTRL_EQUIP_WINDOW::iterator iter;


    iter = m_UseTooltipCtrls.find(pos);

    if (iter != m_UseTooltipCtrls.end())
    {
        return iter->second;

    }
    else
    {
        return 0;
    }
}
//----------------------------------------------------------------------------
/**
*/
void 
RiderInventoryDialog::DeleteViewRider()
{
    UnlockSlotRider();

    if(m_pDisplayRider)
    {
        m_pDisplayRider->Destroy();
        SAFE_DELETE(m_pDisplayRider);
    }
}
//----------------------------------------------------------------------------
/** 슬롯라이더 아이템을 락을 풀어준다.
*/
void
RiderInventoryDialog::UnlockSlotRider(BOOL bRemove)
{
    if(m_bSlotRider)
    {   
        m_bSlotRider = FALSE;
        SCItemSlot & RiderSlot = (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(m_SummmonPos);
 
        if(RiderSlot.IsContainValidItem() && RiderSlot.IsRiderItem())
        {
            if(RiderSlot.GetLaxLock()->IsLock())
            {
                RiderSlot.GetLaxLock()->Clear();
            }
        }

        if(bRemove && m_pDisplayRider)
        {
           m_pDisplayRider->Destroy();
           SAFE_DELETE(m_pDisplayRider);
        }
    }
}
//----------------------------------------------------------------------------
/** 주인공의 소환된 라이더로부터 복사본을 만들어낸다.
*/
void
RiderInventoryDialog::CopyHeroRider(Rider* pRIder)
{
    //널값이 들어오면 해제해버린다.
    if(pRIder)
    {
        UnlockSlotRider();

        //최초생성이라면 메모리할당해준다.
        if(!m_pDisplayRider)
        {
            m_pDisplayRider = new Rider;
        }
        //여기서부터 복사한다.
        m_pDisplayRider->CopyRider(pRIder);

        //애니셋팅
        m_pDisplayRider->SetAnimation(GlobalFunc::GetRiderAnimationByPlayerAni(eANI_STAND, false),TRUE);
        m_pDisplayRider->EnableFunction(FALSE);
        UpdateData();
    }
    else
    {
        if(m_pDisplayRider)
        {
            DeleteViewRider();
        }
    }
}
//----------------------------------------------------------------------------
/** 슬롯 정보로부터 라이더 복사본을 만들어낸다.(기한제아이템 만료시)
*/
void             
RiderInventoryDialog::CopySlotRider(POSTYPE pos)
{
    //pos는 인벤포지션이다: 인벤의로부터 View라이더를 만들어낸다.
    SCItemSlot & RiderSlot = (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(pos);
       
    //라이더아이템인지 체크
    if(RiderSlot.IsRiderItem() && RiderSlot.IsContainValidItem())
    {
        //주인공이 탈것상태가 아니여야 한다.
        if(g_pHero->IsRiding())
        {
            //6455 탈것 소환상태로 해당아이템을 사용할수없습니다.
            TCHAR szMessage[INTERFACE_STRING_LENGTH]; 
            g_InterfaceManager.GetInterfaceString(6455,szMessage);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            return;
        }
    
        //▶여기서부터는 슬롯복사본 라이더상태가되는것이다.
        m_bSlotRider = TRUE;
        
        //락상태가아니라면 잠궈준다.
        if(!RiderSlot.GetLaxLock()->IsLock())
        {
            m_SummmonPos = pos;
            RiderSlot.GetLaxLock()->EnterLock();
        }
        
        //아직생성이 안된상태라면
        if(!m_pDisplayRider) 
        {
          m_pDisplayRider = new Rider;
        }

        //라이더 슬롯컨테이너를 재구축한다. 구축후에는 파츠를및 바디를 만들어낸다.
        this->CopyItem(RiderSlot);

        //▶여기서부터 외형복사(유닛)가이루어진다.
        //step1:BODY복사
        m_pDisplayRider->CopyRiderBody(RiderSlot.GetItemInfo()->m_wSkillCode, g_pHero);
        UpdateData();
                
        //step2:PARTS복사
        for(int i=0;i<MAX_RIDER_SLOT;++i)
        {
            SCItemSlot &rRiderPart = (SCItemSlot &)GetSlot(i);

            //슬롯유효성체크
            if(rRiderPart.IsContainValidItem())
            {
                m_pDisplayRider->AttachRiderPart(i,rRiderPart.GetCode());
            }
            else
            {
                m_pDisplayRider->DetachRiderPart(i);
            }
        }
        m_pDisplayRider->SetAnimation(GlobalFunc::GetRiderAnimationByPlayerAni(eANI_STAND, false),TRUE);
        m_pDisplayRider->EnableFunction(FALSE);
    }
}
//----------------------------------------------------------------------------
/**
*/
void
RiderInventoryDialog::InitRenderer()
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
/**
*/
void
RiderInventoryDialog::OnShowWindow(BOOL val)
{
	if(val)
	{
#if NATION_BUILD_VERSION < 1004
		//InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );

		//if(pInven)
		//{
			SolarDialog * pInven = ItemManager::Instance()->GetItemContainerDialog(SI_INVENTORY);

			
			if(pInven)
			{
				
				RECT rcHeroInven,rcRiderInven;
				pInven->GetDialogWZ()->GetClientRect(&rcHeroInven);
				GetDialogWZ()->GetDlgSize(&rcRiderInven);
                //인벤 아래쪽에 붙임
                MoveWindowWZ(rcHeroInven.left, rcHeroInven.bottom);
			}
		//}
#endif //NATION_BUILD_VERSION
//!
//------------------------------------------------------------------------------ 

	}
    else
    {
        UnlockSlotRider(TRUE);
    }
}
//----------------------------------------------------------------------------
/** 필드간 이동시 저장정보를 통해  탈것을 원래되로 되돌린다
*/
void    
RiderInventoryDialog::ReloadRider(SCItemSlot &RiderSlot)
{
   //파츠변경시 기존 컨테이너를 전부 날린다.
    Clear();

    if(RiderSlot.IsRiderItem())
    {
        DWORD dwRiderCode = RiderSlot.GetItemInfo()->m_wSkillCode;

        for(int i=0;i<MAX_RIDER_SLOT;++i)
        {
            SCRiderPartsData parts_data;
            memset(&parts_data,0,sizeof(parts_data));

            eRIDER_PARTS_POS Position; 
            Position.value = Position.StartIndex+i;

            SCItemSlot ItemSlot;
            if(RiderSlot.GetRiderPartsItem(Position,&ItemSlot))
            {
                if(ItemSlot.IsContainValidItem())
                {
                    this->InsertSlot( i, ItemSlot);
                }
            }
            else
            {
                _asm nop;
            }
        }

        if(dwRiderCode!=0)
        {
            g_pHero->CreateRider(dwRiderCode);
            g_pHero->RefreshRider();
        }
    }
}
//----------------------------------------------------------------------------
/**
*/
void			 
RiderInventoryDialog::renderToolTip()
{	
    uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    
    if(!pUIMan)
    {
        return;
    }
    
	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType && m_MouseOverObject.pSlot)
		{
			pUIMan->InitTooltip();
	    	pUIMan->RegisterSlotInfoTooltip(static_cast<SCItemSlot*>( m_MouseOverObject.pSlot ));
			pUIMan->SetTooltipDialogID(GetDialogKey());

    	}
	}
	
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < MAX_RIDER_SLOT; a++)
	{
		CControlWZ * pControl = getControl(a);
		
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
	}
    else
    {
        if(m_iTooltipStringCode)
        {
            pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);    
			pUIMan->SetTooltipDialogID(GetDialogKey());
        }
    }


}
//----------------------------------------------------------------------------
/** 메시지처리
*/
VOID			
RiderInventoryDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if (pos >= 0 &&  pos <MAX_RIDER_SLOT)
            {
                if ( pos >= GetMaxSlotNum() )
                {
                    // 잘못된 슬롯 위치를 참조할려고한다 리턴.
                    return;
                }

                if (!IsEmpty( pos ))
                {
                    m_iTooltipStringCode = 0;

                    SCSlot & rSlot = GetSlot( pos );
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                    m_MouseOverObject.pSlot = &rSlot;
                }
                else
                {
                    m_iTooltipStringCode = getTooltipString(pos);
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
                }
            }
            else
            {
                m_iTooltipStringCode = 0;
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
RiderInventoryDialog::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC_INSERT_SLOT_FAILED;
    }

    // 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = RiderContainer::InsertSlot( AtPos, rSlot );

    
    // 클라에서 따로 시리얼을 발급한다.
    SCSlot &SlotRef = GetSlot(AtPos);
    
    /// 인벤토리	
    CControlWZ * pCtrl = getControl(AtPos);

    if(pCtrl && m_pItemUnitRenderer)
    {
        RECT rect = pCtrl->GetSizeRect();

        m_pItemUnitRenderer->AddItemUnit( SlotRef.GetSerial(), SlotRef.GetCode(), &rect, &SlotRef, pCtrl);
    }

	return eInsertSlotResult;
}
//----------------------------------------------------------------------------
/** 슬롯제거
*/
BOOL 
RiderInventoryDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
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
    
	RiderContainer::DeleteSlot(AtPos, pSlotOut );
	
	return TRUE;

}
//----------------------------------------------------------------------------
/** 디스플레이용 라이더를 애니메이션시켜준다.
*/
VOID
RiderInventoryDialog::Process(DWORD dwTick)
{
	if(g_pHero && m_pDisplayRider)
	{
    	m_pDisplayRider->Process(dwTick, NULL);
		m_pDisplayRider->AnimateUnit(NULL);
	}
}
//----------------------------------------------------------------------------
/**
*/
void RiderInventoryDialog::updateRotate()
{
    static float fRange = 0.07f;
    if(m_eRotateType==R_ROTATE_NONE)
    {
        return;
    }

    switch(m_eRotateType)
    {
    case R_ROTATE_LEFT:
        {
            rotateRider(fRange);
        }
        break;
    case R_ROTATE_RIGHT:
        {
            rotateRider(-fRange);
        }
        break;
    }

    RECT rc;
    GetDialogWZ()->GetClientRect(&rc);
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();
    if (rc.left <= iMouseX && rc.right >= iMouseX)
    {
        if (rc.top <= iMouseY && rc.bottom >= iMouseY)
        {
            return;
        }
    }

    m_eRotateType=R_ROTATE_NONE;
    
}
//----------------------------------------------------------------------------
/**
*/
void
RiderInventoryDialog::OnUpdateSolarDialog()
{
	if(m_pItemUnitRenderer)
    {
		m_pItemUnitRenderer->Update();

        updateRotate();
    }
}
//----------------------------------------------------------------------------
/**
*/
void
RiderInventoryDialog::UpdateData()
{
	if(!m_TxtCtrlTitle) return;

    POSTYPE Pos= g_pHero->IsRiding() ? (g_pHero->m_RiderInfo.m_RiderItemPos):(m_SummmonPos);

    SCItemSlot & rSlot = (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(Pos);

    GlobalFunc::SetCaption(m_TxtCtrlTitle,"");

    if( rSlot.IsContainValidItem() && rSlot.IsRiderItem())
    {
        if(m_pDisplayRider)
        {
            const  sRiderInfo* pRiderInfo = RidingInfoParser::Instance()->GetInfo(rSlot.GetItemInfo()->m_wSkillCode);
            m_fScale = (pRiderInfo) ? (pRiderInfo->size_control_):(1.f);
            WzVector vScale = {m_fScale,m_fScale,m_fScale};
            
            //회전각은 계속유지
            m_pDisplayRider->SetRotation(&l_vNormal,m_fAngleZ);
            m_pDisplayRider->SetScale(vScale);
            static int nLod =0;
            m_pDisplayRider->SetForceLod(nLod);
        }
                
        TCHAR   szName[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString( rSlot.GetItemInfo()->m_NCode, szName, INTERFACE_STRING_LENGTH );
        GlobalFunc::SetCaption(m_TxtCtrlTitle, szName);
    }
}
//----------------------------------------------------------------------------
/**
*/
void
RiderInventoryDialog::OnRenderSolarDialog()
{
	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->Render(); 
	}
	
	RenderRider();

	renderToolTip();
}
//----------------------------------------------------------------------------
/** 라이더 뷰포트에랜더하기(여기서 애니메이션한다)
*/
void
RiderInventoryDialog::RenderRider()
{
	CControlWZ *pControl = getControl(RIDER_SLOT_VIEW);
	if (pControl && m_pDisplayRider)
	{
        RECT rcSize = pControl->GetSizeRect();
        const float fMeshHeight = 2.6f;
		const float fMeshWidth = 4.0f;	
		// 뷰포트는 반드시 g_InterfaceManager.CalcuateViewPortWithRect로 생성해야함
		RECT rcViewport;
		rcViewport = g_InterfaceManager.CalcuateViewPortWithRect(rcSize);

        g_InterfaceManager.RenderRiderOnRect(m_pDisplayRider,rcViewport,fMeshWidth,fMeshHeight);
	}

}
//----------------------------------------------------------------------------
/**
*/
void 
RiderInventoryDialog::FlushUnitDraw()
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
RiderInventoryDialog::Release()
{
	FlushUnitDraw();
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	RiderContainer::Release();
}
//---------------------------------------------------------------------------- 
/** 
*/
RiderContainer* 
RiderInventoryDialog::GetSlotContainer()
{
	return static_cast<RiderContainer*>(this);
}

void RiderInventoryDialog::rotateRider(float fAngle)
{
    if(m_pDisplayRider)
    {
        m_fAngleZ += fAngle;
        // 0~360도 사이로 유지
        ClampAngle(&m_fAngleZ);
        m_pDisplayRider->SetRotation(&l_vNormal,m_fAngleZ);
    }
}
//---------------------------------------------------------------------------- 
/** 
*/
VOID	
 RiderInventoryDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
    case RT_MSG_LBUTTONUP:
        {
            m_eRotateType = R_ROTATE_NONE;
        }
        break;
    case RT_MSG_LBUTTONCLICK:
        {
            m_eRotateType = R_ROTATE_NONE;

            if(getCtrlIDToPos( pMessage->dwCtrlID )==RIDER_BTN_EXIT)
            {
                ShowInterface(FALSE);
            }
        }
        break;

	case RT_MSG_LBUTTONDOWN:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch(pos)
            {
            case RIDER_BTN_LEFT:
                {
                    m_eRotateType = R_ROTATE_LEFT;
                }
                break;
            case RIDER_BTN_RIGHT:
                {
                    m_eRotateType = R_ROTATE_RIGHT;
                }
                break;
            }
			
			//라이더인벤에 아이템 놓다.
			if(pos <= RIDER_SLOT_03) 
			{
				CControlWZ * pControl = getControl(pos);
				if (pControl)
				{
					RECT rcSlot = pControl->GetSizeRect();
					MouseHandler::Instance()->ItemTransaction(SI_RIDER, (pos), &rcSlot);
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
