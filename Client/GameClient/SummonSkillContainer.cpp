//---------------------------------------------------------------------------
// SummonSkillContainer
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
#include "SummonSkillContainer.h"
#include "SkillSlotContainer.h"
#include "SkillSlot.h"
#include "QuickContainer.h"
#include "DlgPosInfoParser.h"
#include "ObjectManager.h"
#include "SkillMain.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiSkillTree/uiSkillTree.h"
#include <SkillInfoParser.h>
#include "ItemUnitRenderer.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSummonSkillMan/uiSummonSkillMan.h"
//----------------------------------------------------------------------------
/**
*/
WzID SummonSkillContainer::m_wzId[SUMMONSKILL_QUICK_MAX] = 
{ 
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("B001"),
	StrToWzID("B000"),
	StrToWzID("B002"),
	StrToWzID("B003"),
	StrToWzID("B004"),
#else
	StrToWzID("B001"),
	StrToWzID("B002"),
	StrToWzID("B003"),
	StrToWzID("B004"),
	StrToWzID("B005"),
	StrToWzID("B006"),
	StrToWzID("B007"),
#endif
};


//----------------------------------------------------------------------------
/**
*/
SummonSkillContainer::SummonSkillContainer() 
: m_TabNum(0)
, m_pItemUnitRenderer(NULL)
, m_DialogKey(0)
{
}


//----------------------------------------------------------------------------
/**
*/
SummonSkillContainer::~SummonSkillContainer()
{
	assert( NULL == m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::OnUpdateSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();

	uiSkillMan* skillMan =
		static_cast<uiSkillMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SKILL));

	//Todo:최적화 할것 
	if (skillMan)
	{
		for (int i = 0; i < GetMaxSlotNum(); ++i)
		{	
			if (IsEmpty(i)==FALSE)//빈슬롯이 아니면 
			{
				SkillSlot & rSlot = (SkillSlot &) GetSlot(i);

				SLOTCODE skillCode=rSlot.GetCode();
				
				SkillScriptInfo *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(skillCode);
				assert(pSkillInfo && "적용할수없는 스킬의 결과입니다");
				int state=-1;

				if(!m_pItemUnitRenderer)
					continue;

				ItemUnitRenderer::RenderUnit *pUnit=m_pItemUnitRenderer->GetItemUnit(skillCode);
				if(pUnit)
				{
					state=pUnit->GetState();
				}

				if(pSkillInfo)
				{
					// 소환수 스킬의 슬롯의 자체적으로 업데이트를 하지 않고,
					// 스킬 UI에 있는 슬롯의 Enable/Disable 또는 쿨타임등의 상태를 가져와 그대로 사용한다. 

					if( SkillData* skill_data = xGetSkillData()->GetSkillDataBySkillClassCode(pSkillInfo->m_SkillClassCode) )
                    {
                        if( SkillScriptInfo* skill_info = skill_data->GetCurSkillInfo() )
                        {
						    // 스킬코드가 다르다는건 레벨이 다른것이고, 상태가 다르다면 쿨타임, Enable, Disable등의 정보가 다르다
						    // 두 경우 다 업데이트 한다.
						    if( (pSkillInfo->m_SkillCode != skill_info->m_SkillCode ) ||
							    (state != skill_data->GetState() ) )
						    {
							    m_bFirst = TRUE;

							    // 새롭게 덮어 쓴다.
							    SkillSlot tSlot;
							    tSlot.SetSlotType(ST_SKILL);
							    tSlot.SetCode( skill_info->m_SkillCode );

							    InsertSlot( i, tSlot );	

							    if( ItemUnitRenderer::RenderUnit* pUnit = m_pItemUnitRenderer->GetItemUnit( skill_info->m_SkillCode ) )
							    {
								    // 상태를 설정
								    pUnit->SetState( skill_data->GetState() );
							    }
						    }
                        }
					}
				}
			}
		}
	}


	Character * pTargetObject = (Character *)g_ObjectManager.GetObject(m_dwTargetKEY);

	if (pTargetObject)
	{
		DWORD curhp = pTargetObject->GetHP();


		if (0 == curhp)
		{
			// 아직 정의된것이없다.
			SolarDialog::ShowWindow(FALSE);
		}
	}
	else
		SolarDialog::ShowWindow(FALSE);
}


//----------------------------------------------------------------------------
/**
*/
VOID SummonSkillContainer::NetworkProc( MSG_BASE * pMsg )
{

}

//----------------------------------------------------------------------------
/**
*/
VOID			
SummonSkillContainer::OnShowWindow(BOOL val)
{
	if (val)
	{
        MoveDefaultWindowPos();
		m_bFirst=FALSE;
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
SummonSkillContainer::OnRenderSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();

//임시로 막았음
/*
#if APPLY_MOUSE_OVER

	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			g_InterfaceManager.RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (DWORD a = SUMMONSKILL_QUICK_BT00; a < SUMMONSKILL_QUICK_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}

#else
*/
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	DWORD a = SUMMONSKILL_QUICK_BT00;
	for (a = SUMMONSKILL_QUICK_BT00; a < SUMMONSKILL_QUICK_MAX; a++)
	{
		CControlWZ *pControl = getControl((POSTYPE)a);
		RECT rc = pControl->GetSizeRect();
		
		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				if ( !IsEmpty((POSTYPE)a) )
				{
					SkillSlot & rSlot = (SkillSlot  &)GetSlot( (POSTYPE)a );

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

//#endif

}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::Init( CDrawBase * pDrawBase )
{
	// 창 위치 조정
	MoveWindowWZ(633,600);				
	
	SummonSkillSlotContainer::Init( SUMMONSKILL_QUICK_MAX );

	//m_pSkillHashTable->Initialize( SUMMONSKILL_QUICK_MAX );
	
	m_dwTargetKEY = 0;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::InitItemRender()
{

	if (m_pItemUnitRenderer) 
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, GetMaxSlotNum() );
	m_pItemUnitRenderer->ShowCooldown(TRUE);
}


/**	!! StyleSlot 만 Insert한다.
*/
RC::eSLOT_INSERT_RESULT		SummonSkillContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	if ( !IsEmpty(AtPos) )
	{
		DeleteSlot(AtPos,NULL);
	}

	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SummonSkillSlotContainer::InsertSlot( AtPos,rSlot );
	
	SkillSlot & rSkillSlot = (SkillSlot &)(GetSlot(AtPos));

	CControlWZ * pCtrl = getControl( AtPos );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( 
			rSkillSlot.GetCode(),			
			rSkillSlot.GetCode(), 
			&rect, 
			&rSkillSlot ,pCtrl);
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
SummonSkillContainer::getControlPos( POSTYPE AtPos, RECT & rectOut )
{
	CControlWZ * pCtrl = getControl( AtPos );
	rectOut = pCtrl->GetSizeRect();
}

//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::Clear()
{
	for (int i = SUMMONSKILL_START_SLOT; i < SUMMONSKILL_MAX_SLOT; ++i)
	{
		if (!IsEmpty(i))
			DeleteSlot(i,NULL);
	}
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
SummonSkillContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	// 소스 퀵 포지션 초기화
	SkillSlot & rSlot = (SkillSlot &)GetSlot(AtPos);

	if (!IsEmpty(AtPos))
	{	
		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetCode() );
		}
		else
		{
			assert(m_pItemUnitRenderer);
		}
	}

	return SummonSkillSlotContainer::DeleteSlot( AtPos, pSlotOut );
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
SummonSkillContainer::getControl( POSTYPE AtPos )
{
	assert( AtPos < SUMMONSKILL_QUICK_MAX );
	return GetControlWZ( m_wzId[ AtPos] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
SummonSkillContainer::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < SUMMONSKILL_QUICK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return SUMMONSKILL_QUICK_MAX;
}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer * 
SummonSkillContainer::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}

//----------------------------------------------------------------------------
/**
*/
BOOL SummonSkillContainer::UseSummonSkill(eSUMMONSKILL_QUICK_POS pos)
{
	if( pos >= SUMMONSKILL_QUICK_MAX ) return FALSE;

	CControlWZ * pControl = getControl(pos);
	assert(pControl);

	SCSlot& slot=GetSlot(pos);

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if(!m_pItemUnitRenderer)
		return FALSE;

	ItemUnitRenderer::RenderUnit *pUnit=m_pItemUnitRenderer->GetItemUnit(slot.GetCode());
	if(pUnit)
	{
		if(pUnit->GetState()==SLOT_RENDER_STATE_ACTIVATED)
		{
			g_HeroInput.OnSkill(slot.GetCode());
			return TRUE;
		}
		else
		{
			//	해당 스킬이 비활성화 상태입니다.
			g_InterfaceManager.GetInterfaceString( eST_DEACTIVATE_SKILL, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			return FALSE;
		}

	}
	else
	{	
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("스킬 창이 비어 있습니다."));
		return FALSE;
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos >= SUMMONSKILL_QUICK_MAX ) return;

			CControlWZ * pControl = getControl(pos);
			assert(pControl);

			SCSlot& slot=GetSlot(pos);

			if(!m_pItemUnitRenderer)
			{
				assert(m_pItemUnitRenderer);
				break;
			}

			ItemUnitRenderer::RenderUnit *pUnit=m_pItemUnitRenderer->GetItemUnit(slot.GetCode());
			if(pUnit)
			{
				if(pUnit->GetState()==SLOT_RENDER_STATE_ACTIVATED)
				{
					g_HeroInput.OnSkill(slot.GetCode());
				}
				else
				{
					//	해당 스킬이 비활성화 상태입니다.
					g_InterfaceManager.GetInterfaceString( eST_DEACTIVATE_SKILL, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}

			}
		}
		break;

	default:
		SolarDialog::MessageProc(pMessage);
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID			
SummonSkillContainer::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			if (pos >= SUMMONSKILL_QUICK_BT00 && pos < SUMMONSKILL_QUICK_MAX)
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
SummonSkillContainer::FlushUnitDraw()
{
	if (m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->FlushUnitDraw();
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SummonSkillContainer::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		SkillSlot & rSkillSlot = (SkillSlot &)rSlot;

		if (!IsEmpty(i))
		{
			if ( i >= SUMMONSKILL_START_SLOT && i < SUMMONSKILL_MAX_SLOT )
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여줘야할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rSkillSlot.GetCode() );

				if (!pRender )
				{
					pCtrl = getControl( i );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( 
						rSkillSlot.GetCode(), 
						rSkillSlot.GetCode(), 
						&rcSlot, 
						&rSkillSlot,pCtrl);
				}

				if (pRender)
					pRender->SetShow(TRUE);
			}
			else
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여지지 말아야 할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetCode() );

				if (!pRender )
				{
					pCtrl = getControl( i );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( 
						rSkillSlot.GetCode(), 
						rSkillSlot.GetCode(), 
						&rcSlot, 
						&rSkillSlot ,pCtrl);
				}

				if (pRender)
					pRender->SetShow(FALSE);				
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
POSTYPE 
SummonSkillContainer::GetPosByCode( CODETYPE Code )
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{	
		if (IsEmpty(i))
		{
			continue;
		}

		SkillSlot & rSlot = (SkillSlot &) GetSlot(i);

		if ( rSlot.GetCode() == Code )
		{
			return i;
		}
	}

	return INVALID_SLOT_POS;
}

void SummonSkillContainer::MoveDefaultWindowPos()
{
    if (g_pHero != NULL)
    {
        QuickContainer* quick_container = g_pHero->GetQuickContainer();

        int quick_size = 1;
        if (quick_container != NULL)
        {
            if (quick_container->double_size() == true)
            {
                quick_size = 2;
            }
            //else // int quick_size = 1
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
            uiSummonSkillMan* ui_summon_skill_man = 
                GET_DIALOG_MANAGER(uiSummonSkillMan, UIMAN_SUMMONSKILL);
            if (ui_summon_skill_man != NULL)
            {
                ui_summon_skill_man->MoveSummonSkillDialog(quick_size);
            } 
#endif
        }
    }
}
