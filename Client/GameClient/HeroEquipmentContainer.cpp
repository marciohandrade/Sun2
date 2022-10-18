#include "SunClientPrecompiledHeader.h"
#include "heroequipmentcontainer.h"
#include "ItemInfoParser.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "InventoryDialog.h"
#include "ItemUnitRenderer.h"
#include "QuickSlot.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "MainDialog.h"
#include "ItemAttrCalculator.h"
#include "Mouse.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "InventoryDialog.h"
#include "AppearanceManager.h"
#include "StyleInfoParser.h"
#include "SkillInfoParser.h"
#include "SCItemSlot.h"
#include "SCSkillSlot.h"
#include "DlgPosInfoParser.h"
#include "StyleQuickRegistInfoParser.h"
#include "QuickContainer.h"
    #include "uiSkillMan/uiNewSkillMan.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "EnduranceDialog.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ExtendInventoryDialog.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "Char_SkillDialog.h"
#include "ItemAttrCalculatorEP2.h"

#include "KBMoveCheckManagerInClient.h"

#ifdef _NA_002935_20110704_ITEM_SCORE
#include "ItemScoreParser.h"
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef  _NA_000000_20130114_RENEWER_UI
#include "HeroData.h"
#include "uiHelmetShow.h"
#endif
//----------------------------------------------------------------------------
/**
*/
HeroEquipmentContainer::HeroEquipmentContainer()
	:	m_pItemUnitRenderer(NULL),
    	m_pExtendInvenDlg(NULL),
        m_pInvenMan(NULL),
		m_TabNum(0)
{
    m_bCharCreated = FALSE;
	m_DialogKey = 0;
	m_puiHelmet = NULL;
}


//----------------------------------------------------------------------------
/**
*/
HeroEquipmentContainer::~HeroEquipmentContainer()
{
	Release();
}


//----------------------------------------------------------------------------
/**
*/
VOID HeroEquipmentContainer::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID HeroEquipmentContainer::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	SCItemSlot FromSlot;

	for (int i = 0; i < GetMaxSlotNum(); i++)
	{
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID HeroEquipmentContainer::OnUpdateSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();

	if(m_puiHelmet)
	{
		m_puiHelmet->MoveWindowToEquip((int)GetDialogWZ()->GetDlgX() + 62,
			(int)GetDialogWZ()->GetDlgY() + 76);
	}
	
}



//----------------------------------------------------------------------------
/**
*/
VOID HeroEquipmentContainer::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); i++)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if (rSlot.GetSerial())
		{
			if ( i >= m_TabNum && i < m_TabNum + EQUIP_SLOT_END)
			{
				// 보여줘야할 목록
				if(!m_pItemUnitRenderer)
					continue;

				pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

				if (!pRender)
				{
					pCtrl = getControl( GetRelativeSlotPos(i) );
					rcSlot = pCtrl->GetSizeRect();

					m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
				}

				if (pRender)
					pRender->SetShow(TRUE);
			}
		}

			// 무기는 둘 중 하나만 보여야한다.
	}
}



void HeroEquipmentContainer::TransForm()
{
	Object * pTargetObject = g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	Player * pPlayer = static_cast<Player *>(pTargetObject);
	if(pPlayer)
	{
		pPlayer->GetTransformData().SetTransFormResourceCode(g_pHero->GetTransformData().GetTransFormResourceCode());
		pPlayer->GetTransformData().SetTransFormType(g_pHero->GetTransformData().GetTransFormType());
		pPlayer->GetTransformData().SetTransFormProcessType(g_pHero->GetTransformData().GetTransFormProcessType());
		pPlayer->GetTransformData().SetAppearanceKind(g_pHero->GetTransformData().GetAppearanceKind());
		pPlayer->TransForm();
		pPlayer->SetAnimation("N002", TRUE);
	}
}

void HeroEquipmentContainer::UnTransForm()
{
	Object * pTargetObject = g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	Player * pPlayer = static_cast<Player *>(pTargetObject);

	if(pPlayer)
	{
		pPlayer->GetTransformData().SetTransFormResourceCode(g_pHero->GetTransformData().GetTransFormResourceCode());
		pPlayer->GetTransformData().SetTransFormType(g_pHero->GetTransformData().GetTransFormType());
		pPlayer->GetTransformData().SetTransFormProcessType(g_pHero->GetTransformData().GetTransFormProcessType());
		pPlayer->GetTransformData().SetAppearanceKind(g_pHero->GetTransformData().GetAppearanceKind());
		pPlayer->UnTransForm();
		pPlayer->SetAnimation("N002", TRUE);
	}
}


//----------------------------------------------------------------------------
/**
*/
void HeroEquipmentContainer::RefreshVariation()
{
    Player* player = static_cast<Player *>(g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY)); 

    if (player)
    {
        player->SetPartVariation(PLAYER_VARIATION_FACE, g_pHero->GetPartVariation(PLAYER_VARIATION_FACE));
        player->SetPartVariation(PLAYER_VARIATION_HAIR, g_pHero->GetPartVariation(PLAYER_VARIATION_HAIR));
        player->SetGender(g_pHero->GetGender());
        player->TransformGenderType();
    }
}

//----------------------------------------------------------------------------
/**
*/

void Calc_BoundingVolume2(IN const WzBoundingVolume& bounding_volume, OUT WzVector& gab, OUT WzVector& avg)
{
	SetVector(&gab, 0, 0, 0);
	SetVector(&avg, 0, 0, 0); 
	switch(bounding_volume.m_eBvType)
	{
	case BVT_AABB:
		{
			gab = (bounding_volume.m_aabb.m_wvMax - bounding_volume.m_aabb.m_wvMin);
			avg = (bounding_volume.m_aabb.m_wvMax + bounding_volume.m_aabb.m_wvMin) * 0.5f;
		}
		break;
	case BVT_SPHERE:
		{
			gab.z = bounding_volume.m_BvSphere.m_fRadius;
			avg.z = bounding_volume.m_BvSphere.m_fRadius;
		}
		break;                 
	}
}
void Calc_OrthogonalRect2(IN RECT& area, IN float display_length, IN FPOINT org, IN float zoom, OUT FRECT& orthogonal_rect)
{
	float screen_ratio = float (area.bottom - area.top) / float(area.right - area.left);
	float len = max(0.5f, display_length);
	float size_ratio = screen_ratio/*1.0f*/ / len;
	float width = 1.0f / size_ratio * zoom;
	float height = screen_ratio / size_ratio * zoom;

	orthogonal_rect.left   = width  * (org.x - 0.5f); 
	orthogonal_rect.right  = width  * (org.x + 0.5f);
	orthogonal_rect.top    = height * (org.y - 0.5f);
	orthogonal_rect.bottom = height * (org.y + 0.5f);
}

VOID 
HeroEquipmentContainer::OnRenderSolarDialog()
{
	

	if (m_pItemUnitRenderer && 
		g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_EQUIP))
	{
		m_pItemUnitRenderer->Render();

		WzVector m_CameraFrom, m_CameraTo, m_wvZ;
		SetVector(&m_CameraFrom, 0.0, 0.0, 0.0);
		SetVector(&m_CameraTo, 0.0, 100.0, 0.0);
		SetVector(&m_wvZ, 0.0, 0.0, 1.0);

        if (!m_bCharCreated)
        {
            DWORD class_code = g_pHero->GetClass();
            ClassCodeEx class_code_ex;
            class_code_ex.class_type = g_pHero->GetClass();
            class_code_ex.gender = g_pHero->GetGender();
            class_code = class_code_ex.class_code;

            Player * pPlayer = (Player *)g_ObjectManager.Add(
									EQUIPMENT_CHARACTER_KEY, 
									PLAYER_OBJECT, 
									class_code, 
									FALSE);
            
            pPlayer->SetDirection( m_CameraFrom, m_CameraTo,TRUE);	
            WzVector pos;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = -100.0f;
		    pPlayer->SetPosition(pos);
                       
            CLIENT_CHARACTER_PART part;
            ZeroMemory(&part,sizeof(part));
            part.m_byClass = g_pHero->GetClass();
            part.m_bySlot =0;

            pPlayer->SetClientChar(part);
            pPlayer->SetAnimation("N002", TRUE);
            pPlayer->SetColor(WzColor_RGBA(255,255,255,255));
            pPlayer->SetShow(TRUE); //직접 그릴것임 
            pPlayer->SetFreeMove(TRUE);
			pPlayer->ProcessAnimation(0);

			pPlayer->SetPartVariation(PLAYER_VARIATION_FACE,g_pHero->GetPartVariation(PLAYER_VARIATION_FACE));
			pPlayer->SetPartVariation(PLAYER_VARIATION_HAIR,g_pHero->GetPartVariation(PLAYER_VARIATION_HAIR));

			pPlayer->HideHelmet(g_pHero->IsHideHelmet(),FALSE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
			pPlayer->HideWing(g_pHero->IsHideWing(),FALSE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_004035_20120227_COSTUME_ITEM
            pPlayer->SetCostumeItemCode( g_pHero->GetCostumeItemCode() );
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
			pPlayer->SetDecorateCostumeItemCode( g_pHero->GetDecorateCostumeItemCode() );
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

           	EQUIP_ITEM_INFO out;
			g_pHero->GetEquipItemTotalInfo(out);
			pPlayer->SetEquipItemTotalInfo(out);

            
			
			
			pPlayer->RefreshAppearRender();


            m_bCharCreated = TRUE;

			pPlayer->SetShow(FALSE);
			pPlayer->SetShadowDraw(FALSE);

			if(g_pHero->IsTransForm())//변신 중일때
			{
				pPlayer->GetTransformData().SetTransFormResourceCode(g_pHero->GetTransformData().GetTransFormResourceCode());
				pPlayer->GetTransformData().SetTransFormType(g_pHero->GetTransformData().GetTransFormType());
				pPlayer->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_TRANSFORM);
				pPlayer->GetTransformData().SetAppearanceKind(g_pHero->GetTransformData().GetAppearanceKind());
				pPlayer->TransForm();
			}
        }

        Object * pTargetObject = g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
        Player * pPlayer = static_cast<Player *>(pTargetObject);

        // 선택된 캐릭터 없음 
       if(pPlayer!=NULL)
        {			
			RECT rcSize;
			CControlWZ *pControl = getControl(EQUIP_CHARACTER_VIEW);
			if (pControl)
			{
				rcSize = pControl->GetSizeRect();
			}
			else 
			{
				GetDialogWZ()->GetClientRect(&rcSize);
			}	       

			int iWidth = rcSize.right - rcSize.left;
			int iHeight = rcSize.bottom - rcSize.top; 

			float fMeshHeight = 1.0f;
			float fMeshWidth = 1.0f;			

			// 캐릭터의 키를 구해보려 하나 무슨짓을해도 소용이없었다.
			if (pPlayer->GetBasePart())
			{
				WzBoundingVolume bv = pPlayer->GetBasePart()->GetBoundingVolume();

				switch(bv.m_eBvType)
				{
				case BVT_AABB:
					fMeshHeight = bv.m_aabb.m_wvMax.y - bv.m_aabb.m_wvMin.y;
					fMeshWidth = bv.m_aabb.m_wvMax.x - bv.m_aabb.m_wvMin.x;
					break;
				case BVT_SPHERE:
					fMeshHeight = (bv.m_BvSphere.m_wvCenter.z + bv.m_BvSphere.m_fRadius) * 1.2f;
					fMeshWidth = bv.m_BvSphere.m_fRadius;
					break;
				}
			}

			// 현재 BoundingBox를 구할수없어 캐릭터의 키를 구할수없더.
			// 캐릭터의 키를 하드코딩한다.
			if (g_pHero)
			{
				switch(g_pHero->GetClass())
				{
                case eCHAR_BERSERKER:
					fMeshHeight = 2.0f;
					fMeshWidth = 1.2f;
					break;
				case eCHAR_DRAGON:
					fMeshHeight = 1.8f;
					fMeshWidth = 1.2f;
					break;
				case eCHAR_VALKYRIE:
					fMeshHeight = 1.7f;
					fMeshWidth = 1.0f;
					break;
				case eCHAR_SHADOW:
					fMeshHeight = 1.7f;
					fMeshWidth = 1.0f;
					break;
				case eCHAR_MAGICIAN:
					fMeshHeight = 1.7f;
					fMeshWidth = 1.0f;
					break;
                case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                    fMeshHeight = 1.7f;
                    fMeshWidth = 1.0f;
                    break;
                case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                    fMeshHeight = 1.7f;
                    fMeshWidth = 1.0f;
                    break;
                case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    fMeshHeight = 1.7f;
                    fMeshWidth = 1.0f;
                    break;
				}
			}

			if(g_pHero->IsCompleteTransForm())//변신 중일때
			{
				fMeshHeight = 2.2f;
				fMeshWidth = 1.2f;
			}

			//변신이 풀렸지만 아직 캐릭터로 복원되지 않았을때
			if(!g_pHero->IsTransForm()&&g_pHero->GetTransformData().GetTransFormProcessType()!=TRANSFORM_PROCESS_NONE&&g_pHero->GetTransformData().GetOldTransFormType())
			{
				fMeshHeight = 2.2f;
				fMeshWidth = 1.2f;
			}


            float ScaleToClass = 1.0f;
            //미스틱에 한해서 예외처리
            if(g_pHero->GetClass() == eCHAR_MYSTIC)
            {
                ScaleToClass = 0.75f;
            }

            {
                FPOINT org;
                pPlayer->SetShow(TRUE);
                g_InterfaceManager.RenderPlayerOnRect( pPlayer, rcSize, PI, org, ScaleToClass );
                pPlayer->SetShow(FALSE);
            }
        }

        ns_formula::EquipSlotsManageInfo::Node* it = this->equip_slot_manage_info_.slots,
            * const end = &equip_slot_manage_info_.slots[_countof(equip_slot_manage_info_.slots)];

        POSTYPE item_position = 0;
        for (; it != end; ++it)
        {
            if (!it->activated)
            {
                SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(item_position));
                ItemUnitRenderer::RenderUnit* render_unit = m_pItemUnitRenderer->GetItemUnit(item_slot.GetSerial());
                if (render_unit)
                {
                    CControlWZ* control = getControl(item_position);
                    if (control)
                    {
                        RECT control_rect = control->GetSizeRect();
                        render_unit->m_pIconImage->RenderFreezing(&control_rect);
                    }                    
                }                
            }
            ++item_position;
        }

#ifdef _NA_002935_20110704_ITEM_SCORE
        DrawItemScore();    //아이템 평점을 출력한다.
#endif //_NA_002935_20110704_ITEM_SCORE  

		if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
		{
			if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
			{
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
					pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}

				MouseHandler::MOUSE_STATE eState = MouseHandler::Instance()->GetMouseState();

				if(MouseHandler::GUILD_WITHDRAW != eState &&
					MouseHandler::GUILD_DEPOSIT != eState &&
					MouseHandler::WITHDRAW != eState &&
					MouseHandler::DEPOSIT != eState )
				{
					CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
				}
			}
		}

		int iMouseX = Mouse::Instance()->GetMouseX();
		int iMouseY = Mouse::Instance()->GetMouseY();

		// 스킵처리 
		// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
		BOOL bMouseOver = FALSE;
		for (POSTYPE a = EQUIP_SLOT_START; a < EQUIP_SLOT_END; ++a)
		{
			CControlWZ * pControl = getControl(a);
			if ( !pControl )
			{
				continue;
			}

			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}

#ifdef _NA_000000_20130114_RENEWER_UI
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
        for (POSTYPE pos = EQUIP_SCORE_START; pos < EQUIP_SCORE_END; ++pos)
        {
            CControlWZ* control = getControl(pos);
            if (control == NULL)
            {
                 continue;
			}

            RECT rect = control->GetSizeRect();
            if (rect.left <= iMouseX && rect.right >= iMouseX)
            {
                if (rect.top <= iMouseY && rect.bottom >= iMouseY)
                {
                    bMouseOver = TRUE;
                }
            }
        }
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

		if ( !bMouseOver )
		{
			m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		}
		else
		{
			RenderTooltip( GetDialogKey() );
		}
	}

#ifdef _NA_000000_20130114_RENEWER_UI
	

	if(m_puiHelmet)
	{
		m_puiHelmet->Render();
	}

#endif

}


//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::Init( CDrawBase * pDrawBase )
{
	m_bDrawSlotInfo = FALSE;
	m_iDrawSlotPos = 0;

	//// 위치 조정
 //   RECT rc;
 //   GetDialogWZ()->GetDlgSize(&rc);

 //   float width = (float)rc.right;
 //   float height = (float)rc.bottom;

 //   RECT *rcWindow = g_InterfaceManager.GetWindowRC();

 //   long sw;
 //   long sh;
 //   sw = 1024; sh = 768;

 //   float pw = float( (sw - (width - 15.0f)) );
 //   float ph = float( 210 );

 //   MoveWindowWZ( (float)pw, (float)ph );    
}


//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::InitItemRender()
{
	if (NULL == m_pItemUnitRenderer )
	{
		m_pItemUnitRenderer = new ItemUnitRenderer;
		m_pItemUnitRenderer->Init( m_pInvenDlg->GetDrawBase(), GetMaxSlotNum() );
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::getControlPos( POSTYPE AtPos, RECT & rectOut )
{
	CControlWZ * pCtrl = getControl( AtPos );
	rectOut = pCtrl->GetSizeRect();
}

//----------------------------------------------------------------------------
/**
*/
#ifdef _NA_002935_20110704_ITEM_SCORE
void HeroEquipmentContainer::DrawItemScore()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	CControlWZ* title_control = getControl(EQUIP_TXT_TITLE);

	if ( title_control== NULL )
	{
		return;
	}

	//현재 스트링ID를 받아온다.
	DWORD TextID= title_control->GetTextID();

	TCHAR notice_message[INTERFACE_STRING_LENGTH];
	memset(notice_message,0,sizeof(TCHAR)*INTERFACE_STRING_LENGTH);

    int item_score = g_pHero->GetItemScore();

	//장비평점은 최대 99999임.
	TCHAR notice_Score[6];
	sprintf(notice_Score,"%d",item_score);

	g_InterfaceManager.GetInterfaceStringFormat(notice_message, INTERFACE_STRING_LENGTH, TextID, notice_Score);

	title_control->SetTextWZA(notice_message);
#else

	CControlWZ* title_control = getControl(EQUIP_SCORE_NUMBER1);
	if (title_control)
	{
		RECT first_position = title_control->GetSizeRect();
		int item_score = g_pHero->GetItemScore();
		g_InterfaceManager.RenderDrawItemScore(first_position,item_score);
	}

#endif//#ifdef _NA_000000_20130114_RENEWER_UI

}
#endif //_NA_002935_20110704_ITEM_SCORE

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		HeroEquipmentContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
    g_pHero->GetQuickContainer()->UpdateEquip(rSlot.GetSlotIdx(), rSlot.GetPos(), rSlot.GetCode(), 
        SI_EQUIPMENT, AtPos, rSlot.GetCode());
	g_pHero->GetQuickContainer()->UpdateQuickSlot( rSlot.GetSlotIdx(), rSlot.GetPos(), rSlot.GetCode(), 
		SI_EQUIPMENT, AtPos, rSlot.GetCode());//GS_FIX:퀵슬롯을 업데이트한다.

	RC::eSLOT_INSERT_RESULT eInsertSlotResult = EquipmentContainer::InsertSlot( AtPos, rSlot );

	// 3D 모델링 Render
	CControlWZ * pCtrl = getControl( AtPos );
	RECT rect = pCtrl->GetSizeRect();

	SCSlot & rRealSlot = GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	if (pPlayer) 
	{
		SCSlotContainer *pCon = pPlayer->GetSlotContainer(SI_EQUIPMENT);
		if (pCon) 
		{
			pCon->InsertSlot(AtPos,rSlot);

		}
	}

	
	ItemManager::Instance()->UpdateQuickInfo();
	EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>
		(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
	if(pEnduranceDlg)
		pEnduranceDlg->UpdateEquipEnduranceState();

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if (SLOTCODE item_code = static_cast<SCItemSlot&>(rRealSlot).GetItemCode())
    {
        if (ItemInfoParser::Instance()->CheckChecksum(item_code) == false)
        {
            nsSync::MemoryHackMonitor::Instance()->ForceIncreaseClientTestSerial();
        }
    }
#endif

	return eInsertSlotResult;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
HeroEquipmentContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	// render unit 를 먼저 지운다.
	SCSlot & rSlot = GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	// 시리얼 Restore는 EquipmentContainer 에서 한다. 
	EquipmentContainer::DeleteSlot( AtPos, pSlotOut );	

	ReFreshRenderSlot();

	Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	if (pPlayer) 
	{
		SCSlotContainer *pCon = pPlayer->GetSlotContainer(SI_EQUIPMENT);
		if (pCon) 
		{
			pCon->DeleteSlot(AtPos,NULL);

		}
	}

	ItemManager::Instance()->UpdateQuickInfo();

	EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>
		(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
	if(pEnduranceDlg)
		pEnduranceDlg->UpdateEquipEnduranceState();

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

	return TRUE;
}


VOID HeroEquipmentContainer::OnInsert( SCSlot & IN rSlotIn )
{

	SCItemSlot & rItemSlot = (SCItemSlot &)rSlotIn;
	rItemSlot.OnEquip();

	ItemManager::Instance()->AddUpdateItemSlot(&rSlotIn);

}

VOID HeroEquipmentContainer::OnDelete( SCSlot & IN rSlotIn )
{

	SCItemSlot & rItemSlot = (SCItemSlot &)rSlotIn;
	rItemSlot.OnUnEquip();

	ItemManager::Instance()->RemoveUpdateItemSlot(&rSlotIn);

}


//-------------------------------------------------------------------------------------------
/**
*/
void            
HeroEquipmentContainer::UpdatePosition()
{
	RECT rcSize;
	RECT rcClient;
	this->GetDialogWZ()->GetDlgSize(&rcSize);
	this->GetDialogWZ()->GetClientRect(&rcClient);
	
	POINT pt;
	BOOL bRet = DlgPosInfoParser::Instance()->GetDlgPoint( this->GetDialogWZ()->GetDlgID(), &pt);
	if (bRet)
	{
		m_pInvenDlg->MoveWindowWZ( (float)pt.x, (float)(rcSize.bottom + pt.y) );
        RECT rcExtendInvenSize;
        m_pExtendInvenDlg->GetDialogWZ()->GetDlgSize(&rcExtendInvenSize);
        m_pExtendInvenDlg->MoveWindowWZ((float)(rcClient.right - rcExtendInvenSize.right), 
                                        (float)(rcSize.bottom + pt.y));
	}

	MoveWindowWZ(pt.x,pt.y);
	
	if(m_puiHelmet)
	{
		m_puiHelmet->MoveWindowToEquip((int)GetDialogWZ()->GetDlgX() + 62,
			(int)GetDialogWZ()->GetDlgY() + 72);
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::UpdateSlot( POSTYPE AtPos,SCSlot & rSlot )
{
	EquipmentContainer::UpdateSlot( AtPos, rSlot);

	Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	if (pPlayer) 
	{
		SCSlotContainer *pCon = pPlayer->GetSlotContainer(SI_EQUIPMENT);
		if (pCon) 
		{
			pCon->UpdateSlot(AtPos,rSlot);
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
HeroEquipmentContainer::getControl( POSTYPE AtPos )
{
	assert( AtPos < EQUIP_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


//------------------------------------------------------------------------------
VOID HeroEquipmentContainer::ShowDialog(BOOL val)
{
    if (IsVisible())
    {
        if ((MouseHandler::Instance()->GetMouseState() == MouseHandler::EXTRASTONE_USE) ||
            (MouseHandler::Instance()->GetMouseState() == MouseHandler::SOCKET_REMOVE) || 
            (MouseHandler::Instance()->GetMouseState() == MouseHandler::SOCKET_FILL) || 
            (MouseHandler::Instance()->GetMouseState() == MouseHandler::ETHERIA_COMBINATION) || 
            (MouseHandler::Instance()->GetMouseState() == MouseHandler::ETHER_DEVICE_FILL))
        {
            MouseHandler::Instance()->RemoveMouseState(MouseHandler::EXTRASTONE_USE);
            MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_REMOVE);
            MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHER_DEVICE_FILL);
            MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_FILL);
            MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_COMBINATION);

            return;
        }
    }
    SolarDialog::ShowDialog(val);
}

//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::OnShowWindow( BOOL val )
{
	if (val)
	{
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_inventory;
		msg.wParam=InterfaceManager::DIALOG_EQUIP;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		ReFreshRenderSlot();

#if NATION_BUILD_VERSION < 1004
		this->UpdatePosition();
#endif

		if( g_pHero )
		{
			ChargeItemExtendSlot(g_pHero->GetCharInfo()->m_ExtraEquipCashSlotCount);
		}

#ifdef _NA_000000_20130114_RENEWER_UI
		if(g_pHero != NULL)
		{
			SetChangeBackgroundIsClass(g_pHero->GetClass());
		}

		m_puiHelmet = GET_CAST_DIALOG( uiHelmetShow, IM_INVENTORY_MANAGER::HELMET_UI );
		if(m_puiHelmet)
		{
			m_puiHelmet->ShowWindow(true);
			CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
			pDialogManager->MoveHead(m_puiHelmet->GetDialogKey());
		}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_inventory);

		m_puiHelmet = GET_CAST_DIALOG( uiHelmetShow, IM_INVENTORY_MANAGER::HELMET_UI );
		if(m_puiHelmet)
		{
			m_puiHelmet->ShowWindow(false);
		}
	}

    if (m_pInvenMan && m_pInvenMan->is_normal_inventory_type())
    {
        if (m_pInvenDlg)
        {
            m_pInvenDlg->ShowWindow(val);
        }
    }
    else
    {
        if (m_pExtendInvenDlg)
        {
            m_pExtendInvenDlg->ShowWindow(val);
        }
    }

	if(m_pInvenMan != NULL)
    {
        m_pInvenMan->SetRiderInvenDlg(val);
    }
    else
    {
        m_pInvenMan = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
        if(m_pInvenMan != NULL)
        {
            m_pInvenMan->SetRiderInvenDlg(val);
        }
    }
}

//------------------------------------------------------------------------------
ItemUnitRenderer * HeroEquipmentContainer::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}


//----------------------------------------------------------------------------
/**
*/
VOID HeroEquipmentContainer::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			if(m_puiHelmet)
			{
				m_puiHelmet->CheckMessageMouse(true);
			}

			POSTYPE  pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( EQUIP_MAX == pos ) 
			{
				return;
			}

			/// 마우스에 아이템이 있으면
			/// 들어 갈 수 있는건지 제한 체크하고 
			/// 아이템을 슬롯에 장착한다.

			switch( pos )
			{
			case EQUIP_EXIT:
				g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_EQUIP, FALSE );
				break;
			}

			if (EQUIP_SLOT_START <= pos && EQUIP_SLOT_END > pos )
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				RECT rcSlot = pControl->GetSizeRect();

				if(g_pHero&&g_pHero->IsTransForm())
				{
					SCSlot & rSlot = GetSlot(pos);


					if(rSlot.GetCode())
					{
						//변신중사용불가
						TCHAR	szMessage[MAX_MESSAGE_LENGTH];
						g_InterfaceManager.GetInterfaceString(5139, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					}
				}

				MouseHandler::Instance()->ItemTransaction( SI_EQUIPMENT, GetRealSlotPos(pos), &rcSlot );
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

//----------------------------------------------------------------------------
/**
*/
VOID			
HeroEquipmentContainer::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			if (EQUIP_SLOT_START <= pos && EQUIP_SLOT_END > pos )
			{
				POSTYPE RelativePos = GetRealSlotPos(pos);

				if ( RelativePos >= GetMaxSlotNum() )
				{
					// 잘못된 슬롯 위치를 참조할려고한다 리턴.
					return;
				}

				if (!IsEmpty( RelativePos ))
				{
					m_iTooltipStringCode = 0;

					SCSlot & rSlot = GetSlot( RelativePos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
				}
				else
				{
					m_iTooltipStringCode = getTooltipString(pos);
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifdef _NA_000000_20130114_RENEWER_UI
#else
            else if ((EQUIP_SCORE_START <= pos) && (EQUIP_SCORE_END > pos) )
            {
                // 11478 : 장비 평점
                m_iTooltipStringCode = 11478;
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
            }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
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
/**
*/
VOID 
HeroEquipmentContainer::NetworkProc( MSG_BASE * pMsg )
{

}


//----------------------------------------------------------------------------
/**
*/
VOID 
HeroEquipmentContainer::UseItem( POSTYPE AtPos )
{
}

VOID HeroEquipmentContainer::ReloadItemRender(BOOL bFlag)
{
	// 몇개 있냐?
	POSTYPE MaxPos = EquipmentContainer::GetMaxSlotNum();

	for (int i = 0; i < MaxPos; ++i)
	{
		SCSlot & rSlot = EquipmentContainer::GetSlot(i);
		// 3D 모델링 Render

		if ( 0 != rSlot.GetSerial() )
		{
			if(!m_pItemUnitRenderer)
				continue;


			if ( !m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() ) )
			{
				// 비워있다면

				POSTYPE RelativePos = GetRelativeSlotPos(i);
				CControlWZ * pCtrl = getControl( RelativePos );
				RECT rect = pCtrl->GetSizeRect();
				
				ItemUnitRenderer::RenderUnit * pItemRender = 
					m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rect, &rSlot ,pCtrl);

                if (pItemRender)
                {
				    pItemRender->SetShow(bFlag);
                }
			}
		}

	}
	

	// 돌면서 아이템 리소스 연결안된거 만들어준다.
	// show flag는 false로 해두고
	// refresh 로 보여줄부분 선택되게 한다.


}

void HeroEquipmentContainer::ChargeItemExtendSlot(int iExtendCount)
{

	// Hero에 확장된 슬롯수를 저장해 놓는다.
	if( g_pHero )
	{
		g_pHero->GetCharInfo()->m_ExtraEquipCashSlotCount = iExtendCount;
	}

	CCtrlButtonWZ * pChargeSlot4 =	(CCtrlButtonWZ *) getControl( EQUIP_CHARGE4 );
	CCtrlButtonWZ * pChargeSlot5 =  (CCtrlButtonWZ *) getControl( EQUIP_CHARGE5 );
	if( !pChargeSlot4 || !pChargeSlot5 )
		return;

	pChargeSlot4->EnableWindowWZ(FALSE);
	pChargeSlot5->EnableWindowWZ(FALSE);

	pChargeSlot4->ShowWindowWZ(WZ_HIDE);
	pChargeSlot5->ShowWindowWZ(WZ_HIDE);

	CControlWZ * pImageCharge4 = getControl(EQUIP_PIC_CHARGE4);
	assert(pImageCharge4);
	CControlWZ * pImageCharge5 = getControl(EQUIP_PIC_CHARGE5);
	assert(pImageCharge5);

	if( !pImageCharge4 || !pImageCharge5)
		return;

	pImageCharge4->ShowWindowWZ(WZ_HIDE);
	pImageCharge5->ShowWindowWZ(WZ_HIDE);

	if( iExtendCount >= 1)
	{
		pChargeSlot4->EnableWindowWZ(TRUE);
		pChargeSlot4->ShowWindowWZ(WZ_SHOW);
		pImageCharge4->ShowWindowWZ(WZ_SHOW);
	}

	if( iExtendCount >= 2)
	{
		pChargeSlot5->EnableWindowWZ(TRUE);
		pChargeSlot5->ShowWindowWZ(WZ_SHOW);
		pImageCharge5->ShowWindowWZ(WZ_SHOW);
	}
}

//해당이벤트 코드를 가진아이템존재여부
BOOL HeroEquipmentContainer::FindEventCode(BYTE byEventCode)	
{
	POSTYPE MaxSlot = EquipmentContainer::GetMaxSlotNum();
	const BASE_ITEMINFO* pItemInfo = NULL;

	for (int i = 0; i < MaxSlot; ++i)
	{
		if (!IsEmpty(i))
		{
			SCItemSlot& rItemSlot = (SCItemSlot&)GetSlot(i);
			pItemInfo = rItemSlot.GetItemInfo();
			if (pItemInfo)
			{
				if (pItemInfo->m_byEventCode == byEventCode)
                {
					return TRUE;
                }
			}
		}
	}

	return FALSE;
}
