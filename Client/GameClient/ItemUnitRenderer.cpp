#include "SunClientPrecompiledHeader.h"
#include "itemunitrenderer.h"
#include "ItemUnitCacher.h"
#include <SCSlotHeader.h>
#include "ResourceManager.h"
#include "Camera.h"
#include "SCSlot.h"
#include "quickSlot.h"
#include "ItemInfoParser.h"
#include "IconMgr.h"
#include "IconImage.h"
#include "skillinfoparser.h"
#include "SCItemSlot.h"
#include "Hero.h"
#include "CooltimeManager.h"
#include <SCSkillSlot.h>
#include <SCItemSlot.h>
#include "QuickSlot.h"
#include "DummyItemSlot.h"
#include "Application.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "SkillMain.h"

#include "uiQuestMan/uiQuestMan.h"
#include "ItemCooltimeManager.h"
#include "GameFunc.h"

#include "SCPerkSlot.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "HeroData.h"

//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer::ItemUnitRenderer()
:m_bOverLap(TRUE),
m_pItemUnitHashTable(NULL),
m_pRenderUnitPool(NULL),
m_bShowCooldown(FALSE),
m_bWide(FALSE),
bRenderNumber_(FALSE)
{
	
}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer::~ItemUnitRenderer()
{
	assert( NULL == m_pItemUnitHashTable );
	assert( NULL == m_pRenderUnitPool );

	Release();
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ItemUnitRenderer::Release()
{
	if(m_pItemUnitHashTable)
	{
		m_pItemUnitHashTable->RemoveAll();
		delete m_pItemUnitHashTable;
		m_pItemUnitHashTable	= NULL;
	}
	
	if(m_pRenderUnitPool)
	{
		m_pRenderUnitPool->Release();
		delete m_pRenderUnitPool;
		m_pRenderUnitPool		= NULL;
	}
	
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ItemUnitRenderer::FlushUnitDraw()
{
    if (m_pItemUnitHashTable)
    {
	    RenderUnit * pRenderUnit = NULL;

	    m_pItemUnitHashTable->SetFirst();

	    while( pRenderUnit = m_pItemUnitHashTable->GetNext() )
	    {		
		    if (pRenderUnit->m_pIconImage)
		    {
			    assert(pRenderUnit->m_pRefSlot);
			    if (!pRenderUnit->m_pRefSlot)
			    {
				    ICONMGR()->RemoveIcon( pRenderUnit->m_pIconImage );
                    pRenderUnit->m_pIconImage = NULL;
			    }
		    }
	    }
    }
}


//----------------------------------------------------------------------------
/**
*/
VOID ItemUnitRenderer::Init( CDrawBase * pDraw, DWORD dwPoolSize )
{
	assert( NULL == m_pItemUnitHashTable );
	assert( NULL == m_pRenderUnitPool );



	m_pItemUnitHashTable	= new util::SolarHashTable<RenderUnit *>;
	m_pRenderUnitPool		= new util::CMemoryPoolFactory<RenderUnit>;
	m_pItemUnitHashTable->Initialize( dwPoolSize );
	m_pRenderUnitPool->Initialize( dwPoolSize, dwPoolSize/2+1 );

	m_pDraw = g_pSunRenderer;

    m_pQuestManager 
        = (uiQuestMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
}


//----------------------------------------------------------------------------
/**
*/
VOID ItemUnitRenderer::Update()
{

}


//----------------------------------------------------------------------------
/** 화면의 3D Item Unit Rendering 하는 함수
*/
VOID ItemUnitRenderer::Render()
{	
    if (g_pSunRenderer)
    {
        if(m_pItemUnitHashTable)
        {
            if( m_pItemUnitHashTable->SetFirst() )
            {

	            BOOL bOld2dMode,bOldRealSize;

	            g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);

	            g_pApplication->Set2DMode();

	            g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );
        	
                RenderUnit* pRenderUnit = NULL;
		        while( pRenderUnit = m_pItemUnitHashTable->GetNext() )
		        {
			        this->renderUnit(pRenderUnit);
		        }

                g_pSunRenderer->Set2DMode(bOld2dMode,bOldRealSize);
                g_pSunRenderer->x_pManagerTextOut->EndUseFont();
            }
	    }
    }
}

void ItemUnitRenderer::ChangePositionItemUnit(DBSERIAL serial, RECT* pRect, CControlWZ *pControl)
{
    ItemUnitRenderer::RenderUnit* pData = m_pItemUnitHashTable->GetData((DWORD)serial);
	assert(pData);
    if (pData == NULL)
	{
		return;
	}
    pData->SetViewRect(pRect);
    pData->SetControl(pControl);
}

// 인터페이스에서만 사용하는 함수임 ( 컨테이너 용도임! )
ItemUnitRenderer::RenderUnit * 
ItemUnitRenderer::AddItemUnit( DBSERIAL serial, CODETYPE code, RECT * pRect, const SCSlot * pSlot ,CControlWZ *pControl)
{
	RESOURCECODE resourceCode;

	assert(pSlot);
	if (!pSlot)
	{
		return NULL;
	}

	ItemUnitRenderer::RenderUnit * pData = m_pItemUnitHashTable->GetData((DWORD)serial );
    assert(pData== NULL);
	if (pData)
	{
		return NULL;
	}

	switch (pSlot->GetSlotType())
	{
	case ST_ITEM:
		{
			SCItemSlot *pItemSlot = (SCItemSlot *)pSlot;
            CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
            BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
            resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
		}
		break;

	case ST_SKILL:
		{
			resourceCode = ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(code);
		}
		break;

	case ST_QUICK:
		{
			QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
			switch ( pQuickSlot->GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					SCItemSlot	*pItemSlot = (SCItemSlot *)pQuickSlot->GetOrgSlot();
					assert(pQuickSlot->GetOrgSlot());
					if(!pQuickSlot->GetOrgSlot())
					{
						return NULL;
					}
                    CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
                    if(pQuickSlot->GetOrgPos() == INVALID_POSTYPE_VALUE)
                    {
                        image_code = code;
                    }
                    BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
                    resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
				}
				break;

			case ST_SKILL:
				{
					resourceCode = ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(code);
				}
				break;
			}
		}
		break;

	case ST_DUMMY_ITEM:
		{
			DummyItemSlot * pDummyItemSlot = (DummyItemSlot *)pSlot;
			assert( pDummyItemSlot->GetOrgSlot() );
			if (!pDummyItemSlot->GetOrgSlot()) 
			{
				return NULL;
			}

			switch (pDummyItemSlot->GetOrgSlot()->GetSlotType()) 
			{
			case ST_ITEM:
				{
					SCItemSlot	*pItemSlot = (SCItemSlot *)pDummyItemSlot->GetOrgSlot();
                    CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
                    if (pSlot->GetPos() == INVALID_POSTYPE_VALUE)
                    {
                        image_code = code;
                    }
                    BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
                    resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
				}
				break;

				// TODO : 나중에 스킬이나 다른 슬롯이 포함되면 여기에 추가.

			default:
				assert(0);
				break;
			}
			
		}
		break;
    case ST_STYLE_QUICK:
        {
            // PERKSLOT 의 슬롯타입
            static WzID image_code = 14001;
            const SCPerkSlot* perk_slot = static_cast<const SCPerkSlot*>(pSlot);
            resourceCode = perk_slot->perk_info()->icon_code_;
        }
        break;
	}
	
	IconImage * pIcon = ICONMGR()->Add( resourceCode );
	if(!pIcon)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("아이콘 리소스 코드 오류[%d]"), resourceCode);
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("아이콘 리소스 코드 오류[%d]"), resourceCode);
		assert(pIcon);
	}

	RenderUnit * pRenderUnit	= (RenderUnit *) m_pRenderUnitPool->Alloc();
    assert(pRenderUnit);
    if (!pRenderUnit)
    {
        return NULL;
    }

	pRenderUnit->Init();
	pRenderUnit->SetShow( TRUE );
	pRenderUnit->SetViewRect( pRect );
	pRenderUnit->SetControl(pControl);
	pRenderUnit->m_resourcecode = resourceCode;
	pRenderUnit->m_pIconImage	= pIcon;

	// 정보 표시에 필요해서 묻혀둔다.
	pRenderUnit->m_pRefSlot = pSlot;	// 갯수.
	switch (pSlot->GetSlotType() )
	{
	case ST_ITEM:
		{
			if (pSlot->GetCode())
			{
				// 아이템이 있다.
				pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode());
                SCItemSlot* Item_slot = (SCItemSlot*)pSlot;
                pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));
                pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));
			}
		}
		break;

	case ST_SKILL:
		{
			if (pSlot->GetCode())
			{
				// 아이템이 있다.
				pRenderUnit->m_pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pSlot->GetCode());
			}	
		}
		break;

	case ST_QUICK:
		{
			QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
			switch ( pQuickSlot->GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					// 아이템이 있다.
                    SCItemSlot* Item_slot = (SCItemSlot *)pQuickSlot->GetOrgSlot();
                    pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(Item_slot->GetCode());

                    pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));    
                    pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));
				}
				break;

			case ST_SKILL:
				{
					pRenderUnit->m_pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pSlot->GetCode());
				}
				break;
			}
		}
        break;
    case ST_DUMMY_ITEM:
        {
            DummyItemSlot* dummy_item_slot = (DummyItemSlot*)pSlot;
            assert( dummy_item_slot->GetOrgSlot() );
            if (dummy_item_slot->GetOrgSlot() == NULL)
            {
                return NULL;
            }
            if (dummy_item_slot->GetOrgSlot()->GetSlotType() == ST_ITEM)
            {
                SCItemSlot* Item_slot = (SCItemSlot *)(dummy_item_slot->GetOrgSlot());
                pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(Item_slot->GetCode());

                pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));
                pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));

            }
        }
				break;
	}
	
	assert( NULL == m_pItemUnitHashTable->GetData( (DWORD)serial ) );
	m_pItemUnitHashTable->Add( pRenderUnit, (DWORD)serial );

	return pRenderUnit;
}

ItemUnitRenderer::RenderUnit * ItemUnitRenderer::AddItemUnit( DBSERIAL serial, CODETYPE code, RECT * pRect, const SCSlot * pSlot ,bool RenderNumber,CControlWZ *pControl /*= NULL*/ )
{
	bRenderNumber_ = RenderNumber;

	RESOURCECODE resourceCode;

	assert(pSlot);
	if (!pSlot)
	{
		return NULL;
	}

	ItemUnitRenderer::RenderUnit * pData = m_pItemUnitHashTable->GetData((DWORD)serial );
	assert(pData== NULL);
	if (pData)
	{
		return NULL;
	}

	switch (pSlot->GetSlotType())
	{
	case ST_VENDORSEARCH:
	case ST_ITEM:
		{
			SCItemSlot *pItemSlot = (SCItemSlot *)pSlot;
			CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
			BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
			resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
		}
		break;

	case ST_SKILL:
		{
			resourceCode = ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(code);
		}
		break;

	case ST_QUICK:
		{
			QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
			switch ( pQuickSlot->GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					SCItemSlot	*pItemSlot = (SCItemSlot *)pQuickSlot->GetOrgSlot();
					assert(pQuickSlot->GetOrgSlot());
					if(!pQuickSlot->GetOrgSlot())
					{
						return NULL;
					}
					CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
					if(pQuickSlot->GetOrgPos() == INVALID_POSTYPE_VALUE)
					{
						image_code = code;
					}
					BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
					resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
				}
				break;

			case ST_SKILL:
				{
					resourceCode = ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(code);
				}
				break;
			}
		}
		break;

	case ST_DUMMY_ITEM:
		{
			DummyItemSlot * pDummyItemSlot = (DummyItemSlot *)pSlot;
			assert( pDummyItemSlot->GetOrgSlot() );
			if (!pDummyItemSlot->GetOrgSlot()) 
			{
				return NULL;
			}

			switch (pDummyItemSlot->GetOrgSlot()->GetSlotType()) 
			{
			case ST_ITEM:
				{
					SCItemSlot	*pItemSlot = (SCItemSlot *)pDummyItemSlot->GetOrgSlot();
					CODETYPE image_code = GameFunc::GetImagecode(pItemSlot, code);
					if (pSlot->GetPos() == INVALID_POSTYPE_VALUE)
					{
						image_code = code;
					}
					BOOL is_limited = GameFunc::IsLimitedItem(pItemSlot, code);
					resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
				}
				break;

				// TODO : 나중에 스킬이나 다른 슬롯이 포함되면 여기에 추가.

			default:
				assert(0);
				break;
			}

		}
		break;
	case ST_STYLE_QUICK:
		{
			// PERKSLOT 의 슬롯타입
			static WzID image_code = 14001;
			const SCPerkSlot* perk_slot = static_cast<const SCPerkSlot*>(pSlot);
			resourceCode = perk_slot->perk_info()->icon_code_;
		}
		break;
	}

	IconImage * pIcon = ICONMGR()->Add( resourceCode );
	if(!pIcon)
	{
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CENTER_SHOW, _T("아이콘 리소스 코드 오류[%d]"), resourceCode);
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("아이콘 리소스 코드 오류[%d]"), resourceCode);
		assert(pIcon);
	}

	RenderUnit * pRenderUnit	= (RenderUnit *) m_pRenderUnitPool->Alloc();
	assert(pRenderUnit);
	if (!pRenderUnit)
	{
		return NULL;
	}

	pRenderUnit->Init();
	pRenderUnit->SetShow( TRUE );
	pRenderUnit->SetViewRect( pRect );
	pRenderUnit->SetControl(pControl);
	pRenderUnit->m_resourcecode = resourceCode;
	pRenderUnit->m_pIconImage	= pIcon;

	// 정보 표시에 필요해서 묻혀둔다.
	pRenderUnit->m_pRefSlot = pSlot;	// 갯수.
	switch (pSlot->GetSlotType() )
	{
	case ST_ITEM:
		{
			if (pSlot->GetCode())
			{
				// 아이템이 있다.
				pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode());
				SCItemSlot* Item_slot = (SCItemSlot*)pSlot;
				pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));
				pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));
			}
		}
		break;

	case ST_SKILL:
		{
			if (pSlot->GetCode())
			{
				// 아이템이 있다.
				pRenderUnit->m_pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pSlot->GetCode());
			}	
		}
		break;

	case ST_QUICK:
		{
			QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
			switch ( pQuickSlot->GetOrgSlotType() )
			{
			case ST_ITEM:
				{
					// 아이템이 있다.
					SCItemSlot* Item_slot = (SCItemSlot *)pQuickSlot->GetOrgSlot();
					pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(Item_slot->GetCode());

					pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));    
					pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));
				}
				break;

			case ST_SKILL:
				{
					pRenderUnit->m_pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pSlot->GetCode());
				}
				break;
			}
		}
		break;
	case ST_DUMMY_ITEM:
		{
			DummyItemSlot* dummy_item_slot = (DummyItemSlot*)pSlot;
			assert( dummy_item_slot->GetOrgSlot() );
			if (dummy_item_slot->GetOrgSlot() == NULL)
			{
				return NULL;
			}
			if (dummy_item_slot->GetOrgSlot()->GetSlotType() == ST_ITEM)
			{
				SCItemSlot* Item_slot = (SCItemSlot *)(dummy_item_slot->GetOrgSlot());
				pRenderUnit->m_pBaseItemInfo = ItemInfoParser::Instance()->GetItemInfo(Item_slot->GetCode());

				pRenderUnit->SetEdgeImage(SelectItemEdge(pRenderUnit->m_pBaseItemInfo, Item_slot));
				pRenderUnit->SetMarkImage(SelectItemMark(pRenderUnit->m_pBaseItemInfo));

			}
		}
		break;
	}

	assert( NULL == m_pItemUnitHashTable->GetData( (DWORD)serial ) );
	m_pItemUnitHashTable->Add( pRenderUnit, (DWORD)serial );

	return pRenderUnit;
}
VOID ItemUnitRenderer::RemoveItemUnit( DBSERIAL serial )
{
	if(!m_pItemUnitHashTable)
		return;

	RenderUnit * pRenderUnit = m_pItemUnitHashTable->GetData( (DWORD)serial );

	if (!pRenderUnit )
		return;

	ICONMGR()->RemoveIcon(pRenderUnit->m_pIconImage);
    pRenderUnit->m_pIconImage = NULL;
	pRenderUnit->m_pRefSlot = NULL;

	pRenderUnit->SetShow(FALSE);

	m_pItemUnitHashTable->Remove( (DWORD)serial );



	m_pRenderUnitPool->Free( pRenderUnit );
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
ItemUnitRenderer::ShowCooldown(BOOL bFlag)
{
	m_bShowCooldown = bFlag;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::isValidUnit( RenderUnit * pRenderUnit )
{
	if (GENERALPARAM->IsNet())
	{
		return BOOL(pRenderUnit && g_pHero && pRenderUnit->m_pRefSlot);
	}
	else
	{
		return BOOL(pRenderUnit && pRenderUnit->m_pRefSlot);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL
ItemUnitRenderer::renderUnit(RenderUnit * pRenderUnit )
{
	if ( !isValidUnit(pRenderUnit) )
		return FALSE;

	if ( !pRenderUnit->GetShow())
		return FALSE;

	RECT rc;
			
	rc = pRenderUnit->GetControlRect();

	switch (pRenderUnit->m_pRefSlot->GetSlotType() )
	{
	case ST_VENDORSEARCH:
		{
			renderUnitItemNotNumber( &rc, pRenderUnit, (SCItemSlot *)pRenderUnit->m_pRefSlot);
		}
		break;
	case ST_ITEM:
		{
			renderUnitItem( &rc, pRenderUnit, (SCItemSlot *)pRenderUnit->m_pRefSlot);
		}
		break;

	case ST_SKILL:
		{
			renderUnitSkill( &rc, pRenderUnit, (SCSkillSlot *)pRenderUnit->m_pRefSlot, FALSE );
		}
		break;

	case ST_QUICK:
		{
			renderUnitQuick( &rc, pRenderUnit );	
		}
		break;

	case ST_DUMMY_ITEM:
		{
			renderUnitDummy(&rc, pRenderUnit, (DummyItemSlot *)pRenderUnit->m_pRefSlot );
		}
		break;
    case ST_STYLE_QUICK:
        {
            RenderPerkSlot(&rc, pRenderUnit);
        }
        break;

	default:
		break;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**  오버랩이미지 그려주는함수
*/
void				
ItemUnitRenderer::renderItemOverlapImg( RECT * pRect,SCItemSlot * pItemSlot,RenderUnit * pRenderUnit )
{
	renderAddMarkForItem(pRect, pItemSlot, pRenderUnit);

	renderPetLockImg( pRect,pItemSlot);
#ifdef _NA_004035_20120227_COSTUME_ITEM
    renderCostumeLockImg( pRect,pItemSlot);
#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	renderDecorateCostumeLockImg( pRect,pItemSlot);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    renderBadgeitemActiveImage( pRect, pItemSlot );
    renderPCBangItemActiveImage( pRect, pItemSlot );

    renderItemCustomizingImg(pRect, pItemSlot ,pRenderUnit);

    renderCharCustomizeImg(pRect, pItemSlot);

    renderRiderLockImg(pRect,pItemSlot);
    renderWingsLockImg(pRect, pItemSlot);
	renderEtherChargeImg(pRect, pItemSlot, pRenderUnit);
	renderPersnalWaypointImg(pRect, pItemSlot);

	renderPossessionImg( pRect, pItemSlot );
#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	renderDamageImg( pRect, pItemSlot );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

    renderEnchantNum(pRect,pItemSlot,pRenderUnit);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	renderAwakeningNum(pRect, pItemSlot, pRenderUnit);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

}

//-------------------------------------------------------------------------------------------
/**  에테르장전 오버랩이미지
*/
void				
ItemUnitRenderer::renderEtherChargeImg(RECT* pRect, SCItemSlot* pItemSlot, RenderUnit* pRenderUnit)
{
	if (!g_pHero || (SLOT_RENDER_STATE_DEACTIVATED == pRenderUnit->GetState()))
    {
        return;
    }

    if (pItemSlot->GetSlotIdx() != SI_INVENTORY &&
        pItemSlot->GetSlotIdx() != SI_QUICK )
    {
        return;
    }

	if (g_pHero->GetEthreActive() &&
		pItemSlot->GetItemInfo() &&
		(pItemSlot->GetItemInfo()->m_wType == eITEMTYPE_ETHER_BULLET) &&
		(g_pHero->GetEthreInfo().byEtherChargePos == pItemSlot->GetPos()))
	{
		g_InterfaceManager.RenderActivateMark(pRect->left - 13, pRect->top - 13);
	}
}

void ItemUnitRenderer::renderAddMarkForItem(RECT* pRect,SCItemSlot* pItemSlot, RenderUnit* pRenderUnit)
{
    if (pItemSlot)
    {
        const BASE_ITEMINFO* pItemInfo = pItemSlot->GetItemInfo();

        if (pItemInfo != NULL)
        {
            if (pItemSlot->IsEtheriaItem())
            {
                // 에테리아는 그리지 않는다.
                return;
            }

            IconImage* icon_image_ptr = NULL;

            if (pRenderUnit->GetEdgeImage() != NULL)   //테두리가 있으면 출력
            {
                renderIconImageForItem(pRect, pRenderUnit, pRenderUnit->GetEdgeImage());
            }

            if (pRenderUnit->GetMarkImage() != NULL)   //마크가 있으면 출력
            {
                renderIconImageForItem(pRect, pRenderUnit, pRenderUnit->GetMarkImage());
            }
        }
    }
}

IconImage* ItemUnitRenderer::SelectItemEdge(BASE_ITEMINFO* base_item_info, SCItemSlot* item_slot)
{
    if (base_item_info == NULL)
    {
        return NULL;
    }

    IconImage* icon_image_ptr = NULL;

    if (base_item_info->IsChargeItem() == TRUE)  // 캐시아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_CASH_ITEM];
    }
    else if (base_item_info->IsElite() == TRUE)    // 엘리트 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_ELITE];
    }
    else if (item_slot->IsDivine() != FALSE)    // 디바인 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_DEVINE];
    }
    else if (base_item_info->IsRare() == TRUE)    // 레어 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_RARE];
    }
    else if (base_item_info->IsUnique() == TRUE)    // 유니크 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_UNIQUE];
    }

    return icon_image_ptr;
}

IconImage* ItemUnitRenderer::SelectItemMark(BASE_ITEMINFO* base_item_info)
{
    if (base_item_info == NULL)
    {
        return NULL;
    }

    IconImage* icon_image_ptr = NULL;

    if (base_item_info->m_wQuestCode != 0)   // 퀘스트 시작 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_QUEST_START];
    }
    else if (base_item_info->m_wType == eITEMTYPE_AC)  // AC 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_AC];
    }
    else if (m_pQuestManager != NULL && m_pQuestManager->FindQuestItem(base_item_info->m_Code))  // 퀘스트 아이템
    {
        icon_image_ptr = 
            g_InterfaceManager.item_mark_ptr[InterfaceManager::MARK_TYPE_QUEST];
    }

    return icon_image_ptr;
}

void ItemUnitRenderer::renderIconImageForItem(RECT* rect_ptr,RenderUnit* render_unit,IconImage* item_mark_image)
{
    if (item_mark_image != NULL)
    {
        if (render_unit->GetState() != SLOT_RENDER_STATE_DEACTIVATED)
        {
            item_mark_image->Render(rect_ptr);
        }
        else
        {
            item_mark_image->RenderDeactivated(rect_ptr);
        }
    }
}

void ItemUnitRenderer::RefreshItemMark()
{
    //퀘스트가 변경 되었을 때 아이콘 마크를 재설정 (퀘스트는 중간에 변경될 수 있기 때문)
    RenderUnit* render_unit = NULL;
    IconImage* icon_image_ptr = NULL;

    if (m_pItemUnitHashTable != NULL && m_pQuestManager != NULL)
    {
        m_pItemUnitHashTable->SetFirst();
        while( render_unit = m_pItemUnitHashTable->GetNext() )
        {
            if (render_unit->m_pRefSlot->GetSlotType() == ST_ITEM)
            {
                icon_image_ptr = SelectItemMark(render_unit->m_pBaseItemInfo);

                render_unit->SetMarkImage(icon_image_ptr);
            }
        }
    }
}
void ItemUnitRenderer::renderEnchantNum(RECT* pRect, SCItemSlot* pItemSlot, RenderUnit* pRenderUnit)
{
    //강화 단계 출력   
    if (pItemSlot->GetEnchant() > 0)
    {
        const BASE_ITEMINFO* pItemInfo = pItemSlot->GetItemInfo();
        if ((pItemInfo != NULL) &&
            (pItemInfo->IsArmor() || pItemInfo->IsWeapon()
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
            || pItemSlot->IsWingItem()
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
            ))
        {
            RECT temp_rect = {pRect->left,
                pRect->top + 2,
                pRect->right - 2,
                pRect->bottom
            };
            g_InterfaceManager.RenderDrawNumber(temp_rect,pItemSlot->GetEnchant(),DT_RIGHT);
        }
    }
}

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
void ItemUnitRenderer::renderAwakeningNum( RECT* rect, SCItemSlot* item_slot, RenderUnit* render_unit )
{
	// 각성 단계 출력
	if ( item_slot->GetAwakening() > 0 )
	{
		const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
		if ( (item_info != NULL) && 
			 (item_info->IsArmor() || item_info->IsWeapon()) )
		{
			RECT temp_rect = {rect->left + 2,
				rect->top + 2,
				rect->right - 2,
				rect->bottom - 2
			};

			POINT pt = { temp_rect.left, temp_rect.bottom - 9 };
			POINT result_pt = g_InterfaceManager.RenderDrawSign(pt, eIMG_STAR, eIMAGE_NUMBER_ORANGE);
			temp_rect.right += (result_pt.x - temp_rect.left);
			temp_rect.left = result_pt.x;

			g_InterfaceManager.RenderDrawNumber(temp_rect, item_slot->GetAwakening(), DT_LEFT | DT_BOTTOM, eIMAGE_NUMBER_ORANGE);
		}
	}
}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

//-------------------------------------------------------------------------------------------
/**  펫관련 오버랩이미지
*/
void ItemUnitRenderer::renderPetLockImg( RECT * pRect,SCItemSlot * pItemSlot )
{
	if(!g_pHero) return;

	if (pItemSlot &&
        pItemSlot->IsPetItem() &&	//펫이아이템이며
		g_pHero->IsPetReturn() &&	//펫이소환상태고,현재슬롯과 일치하면 락아이콘 랜더
		g_pHero->GetPetItemPos() == pItemSlot->GetPos() &&
        (pItemSlot->GetSlotIdx() == SI_INVENTORY || pItemSlot->GetSlotIdx() == SI_QUICK))
	{
		g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
		g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
	}
}

#ifdef _NA_004035_20120227_COSTUME_ITEM
void ItemUnitRenderer::renderCostumeLockImg( RECT * pRect, SCItemSlot * pItemSlot )
{
    if(g_pHero == NULL )
        return;

    if( pItemSlot == NULL )
        return;

    if( pItemSlot->GetSlotIdx() != SI_INVENTORY && pItemSlot->GetSlotIdx() != SI_QUICK )
        return;

    bool is_costume_item = false;
    if( const BASE_ITEMINFO* item_info = pItemSlot->GetItemInfo() )
    {
        is_costume_item = item_info->m_wType == eITEMTYPE_COSTUME;
    }

    if( is_costume_item == false )
        return;

    if( g_pHero->GetActivedCostumeItemSlotIndex() == pItemSlot->GetPos() )
    {
        g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
        g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
    }
}
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
void ItemUnitRenderer::renderDecorateCostumeLockImg( RECT * pRect,SCItemSlot * pItemSlot )
{
	if(g_pHero == NULL )
		return;

	if( pItemSlot == NULL )
		return;

	if( pItemSlot->GetSlotIdx() != SI_INVENTORY && pItemSlot->GetSlotIdx() != SI_QUICK )
		return;

	bool is_decorate_costume_item = false;
	if( const BASE_ITEMINFO* item_info = pItemSlot->GetItemInfo() )
	{
		is_decorate_costume_item = item_info->m_wType == eITEMTYPE_COSTUME_DECORATE;
	}

	if( is_decorate_costume_item == false )
		return;

	if( g_pHero->GetActivedDecorateCostumeItemSlotIndex() == pItemSlot->GetPos() )
	{
		g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
		g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
	}
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

void ItemUnitRenderer::renderAuraActiveImage( RECT * pRect, SCSkillSlot* pSkillSlot )
{
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if(g_pHero == NULL )
        return;

    if( const SkillScriptInfo* pSkillInfo = pSkillSlot->GetSkillInfo() )
    {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if( pSkillInfo->IsToggleSkill() == false && pSkillInfo->IsAuraSkill() == false )
            return;

        eCHAR_STATE_TYPE StateType = g_pHero->FindToggleSkillState( pSkillInfo->m_SkillClassCode );
#else
        if( pSkillInfo->IsAuraSkill() == false )
            return;

        eCHAR_STATE_TYPE StateType = g_pHero->FindAuraSkillState( pSkillInfo->m_SkillClassCode );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if( StateType != eCHAR_STATE_INVALID )
        {
            g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
            g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
        }
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
}

void ItemUnitRenderer::renderBadgeitemActiveImage( RECT * pRect, SCItemSlot * pItemSlot )
{
    if(g_pHero == NULL )
        return;

    if( pItemSlot->GetSlotIdx() != SI_INVENTORY && pItemSlot->GetSlotIdx() != SI_QUICK )
        return;

    if( xGetHeroData()->GetBadgeItemData().IsActive() == true )
    {
        if( xGetHeroData()->GetBadgeItemData().GetItemPosition() == pItemSlot->GetPos() )
        {
            g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
            g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
        }
    }
}

void ItemUnitRenderer::renderPCBangItemActiveImage( RECT * pRect, SCItemSlot * pItemSlot )
{
    if(g_pHero == NULL )
        return;

    if( pItemSlot->GetSlotIdx() != SI_INVENTORY && pItemSlot->GetSlotIdx() != SI_QUICK )
        return;

    if( xGetHeroData()->GetPCBangItemData().IsActive() == true )
    {
        if( xGetHeroData()->GetPCBangItemData().GetItemPosition() == pItemSlot->GetPos() )
        {
            g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
            g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
        }
    }
}

void ItemUnitRenderer::renderItemCustomizingImg(RECT* rect, 
                                            SCItemSlot* slot_item, 
                                            RenderUnit* render_unit)
{
    if (g_pHero == NULL ||
        slot_item == NULL)
    {
        return;
    }

    if (slot_item->IsPetItem() && 
        slot_item->IsEtheriaItem() && 
        render_unit->GetState() == SLOT_RENDER_STATE_DEACTIVATED)
    {
        g_InterfaceManager.pet_etheria_icon()->RenderDeactivated(rect);
        return;
    }
    else if (slot_item->IsPetItem() &&
        slot_item->IsEtheriaItem())
    {
        g_InterfaceManager.pet_etheria_icon()->Render(rect);
        return;
    }

    if (ItemCooltimeManager::Instance()->FindCooltime(slot_item->GetSerial()))
    {
        return;
    }

    if (slot_item->IsShellItem() &&
        render_unit &&
        render_unit->m_pIconImage)
    {
       render_unit->m_pIconImage->RenderCoverImage(rect, WzColor_RGBA(0, 0, 0, 175));
    }
}

void ItemUnitRenderer::renderCharCustomizeImg(RECT* rect, SCItemSlot* slot_item)
{
    if ((slot_item == NULL) || 
        (g_pHero == NULL))
    {
        return;
    }

    if (slot_item->IsContainValidItem()  == FALSE)
    {
        return;
    }

     WORD item_type = slot_item->GetItemInfo()->m_wType;
     if ((item_type == eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL) ||
         (item_type == eITEMTYPE_CHANGE_CHARACTER_FACE) ||
         (item_type == eITEMTYPE_CHANGE_CHARACTER_HAIR))
     {
        if (GameFunc::IsActiveCharCustomize(slot_item->GetPos()))
        {
            g_InterfaceManager.RenderActivateMark(rect->left-13,rect->top-13);
        }
     }
}

//------------------------------------------------------------------------------ 
void ItemUnitRenderer::renderWingsLockImg(RECT* area, SCItemSlot* item_slot)
{
    if (g_pHero == NULL)
    {
        return;
    }
    if ((item_slot->GetItemInfo()->m_wType == eITEMTYPE_WINGS) && item_slot->IsUserBlocked())
    {
        g_InterfaceManager.m_pPetIconLockImg->Render(area);
        g_InterfaceManager.RenderActivateMark(area->left - 13, area->top - 13);
    }
}
//------------------------------------------------------------------------------ 
void ItemUnitRenderer::renderRiderLockImg( RECT * pRect,SCItemSlot * pItemSlot )
{
    if (g_pHero == NULL)
    {
        return;
    }

    if (pItemSlot &&
        pItemSlot->IsRiderItem() &&	//탈것아이템이며
        (pItemSlot->IsRiderSummoned() || pItemSlot->GetLaxLock()->IsLock()))
    {
        g_InterfaceManager.m_pPetIconLockImg->Render(pRect);
        g_InterfaceManager.RenderActivateMark(pRect->left-13,pRect->top-13);
    }
}



void ItemUnitRenderer::renderPersnalWaypointImg(RECT* pRect, SCItemSlot* pItemSlot)
{
	if (pItemSlot->IsWayPointItem() &&
		pItemSlot->IsProgressTime())
	{
		g_InterfaceManager.m_pPetIconLockImg->Render(pRect);		
	}
}

void ItemUnitRenderer::renderPossessionImg( RECT * pRect,SCItemSlot * pItemSlot )
{
	if( pItemSlot->IsPossessionItem() && pItemSlot->GetIdentifyPossession() )
	{
		// 아이콘 출력
		g_InterfaceManager.m_pIdentifyPossessionImg->Render(pRect);
	}
}

#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
void ItemUnitRenderer::renderDamageImg( RECT * pRect,SCItemSlot * pItemSlot )
{
	if(pItemSlot && pItemSlot->IsDestroyItem())
	{
		// 아이콘 출력
		if(pItemSlot->GetItemInfo()->IsWeapon())//무기
		{
			g_InterfaceManager.m_pIdentifyDamegeImg[InterfaceManager::eITEM_WEAPON]->Render(pRect);
		}
		else if(pItemSlot->GetItemInfo()->IsArmor())//아머
		{
			g_InterfaceManager.m_pIdentifyDamegeImg[InterfaceManager::eITEM_ARMOR]->Render(pRect);
		}
		else if(pItemSlot->GetItemInfo()->IsAccessory() || pItemSlot->GetItemInfo()->IsSpecialAccessory())//악세사리
		{
			g_InterfaceManager.m_pIdentifyDamegeImg[InterfaceManager::eITEM_ACCESSORY]->Render(pRect);
		}
		else
		{
			// 나머지
		}
	}
}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderUnitItem( RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot )
{
    //슬롯유효체크
	if ( !pRenderUnit || !pItemSlot->IsContainValidItem())
	{
		return FALSE;
	}

	if ( !renderItemInvalid(pRect, pRenderUnit, pItemSlot) )
	{
		if ( !renderItemCooltime( pRect, pRenderUnit, pItemSlot ) )
		{
			this->renderSlotState( pRect, pRenderUnit );
		}
	}
  
	renderItemOverlapImg( pRect,pItemSlot,pRenderUnit);

    RenderSpinCooltime(pRect, pRenderUnit, pItemSlot);

	if(bRenderNumber_ == false)
	 renderItemOverlapNum(pRect, pRenderUnit, pItemSlot);

    return TRUE;
}

BOOL									
ItemUnitRenderer::renderUnitItemNotNumber( RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot )
{
	//슬롯유효체크
	if ( !pRenderUnit || !pItemSlot->IsContainValidItem())
	{
		return FALSE;
	}

	if ( !renderItemInvalid(pRect, pRenderUnit, pItemSlot) )
	{
		if ( !renderItemCooltime( pRect, pRenderUnit, pItemSlot ) )
		{
			this->renderSlotState( pRect, pRenderUnit );
		}
	}

	renderItemOverlapImg( pRect,pItemSlot,pRenderUnit);

	RenderSpinCooltime(pRect, pRenderUnit, pItemSlot);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
ItemUnitRenderer::RenderSpinCooltime(RECT* rect, RenderUnit* render_unit, SCItemSlot* item_slot)
{
    if ((render_unit == NULL) || 
        (item_slot== NULL))
    {
        return;
    }

    if (item_slot->IsLocked() == false)
    {
        return;
    }

    float rate = 100.f;

    // 락타임이 걸린아이템인지 체크
    if (ItemCooltimeManager::Instance()->FindCooltime(item_slot->GetSerial(), &rate) == false)
    {
        return;
    }

    render_unit->m_pIconImage->RenderSpinCooltime(rect, rate);

}
//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderUnitQuickItem( RECT * pRect, RenderUnit * pRenderUnit, QuickSlot * pQuickSlot)
{
	if ( !pRenderUnit || !pQuickSlot)
	{
		return FALSE;
	}
	
	SCItemSlot * pItemSlot = (SCItemSlot *)pQuickSlot->GetOrgSlot();

	if (!renderItemInvalidDate(pRect, pRenderUnit, pItemSlot))
	{
		if(!renderItemCooltime( pRect, pRenderUnit, pItemSlot))
		{
			this->renderSlotState( pRect, pRenderUnit );
            if (pQuickSlot->GetOrgPos() != INVALID_POSTYPE_VALUE)
            {
		        renderItemOverlapImg( pRect,pItemSlot,pRenderUnit);
            }
		}
	}

	renderQuickItemOverlapNum(pRect, pQuickSlot);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderUnitDummy( RECT * pRect, RenderUnit * pRenderUnit, DummyItemSlot * pDummySlot )
{
	if ( !pRenderUnit || !pDummySlot)
	{
		return FALSE;
	}


	if ( !pDummySlot->GetOrgSlot() )
	{
		return FALSE;		
	}

	assert( pDummySlot->GetOrgSlot()->GetSlotType() == ST_ITEM );

	renderSlotState( pRect, pRenderUnit );
	SCItemSlot* pOriSlot = (SCItemSlot*)(pDummySlot->GetOrgSlot());
	if ( pOriSlot->IsPossessionItem() || pOriSlot->GetIdentifyPossession() )
	{
		renderPossessionImg( pRect, pOriSlot );
	}

    renderItemOverlapImg(pRect, (SCItemSlot*)pDummySlot->GetOrgSlot(), pRenderUnit);

    renderItemOverlapNum(pRect, pRenderUnit, (SCItemSlot *)pDummySlot->GetOrgSlot() );

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderUnitQuick( RECT * pRect, RenderUnit * pRenderUnit )
{
	assert(ST_QUICK == pRenderUnit->m_pRefSlot->GetSlotType() );

	QuickSlot * pQuickSlot = (QuickSlot *)pRenderUnit->m_pRefSlot;
	
	assert( pRect && pQuickSlot );
	if (!pRect || !pQuickSlot) 
	{
		return FALSE;
	}

	switch (pQuickSlot->GetOrgSlotType())
	{
	case ST_ITEM:
		{
			pRenderUnit->SetState(pQuickSlot->GetStatus());
			this->renderUnitQuickItem( pRect, pRenderUnit, pQuickSlot );
		}
		break;

	case ST_SKILL:
		{
			this->renderUnitSkill( pRect, pRenderUnit, (SCSkillSlot *)pQuickSlot->GetOrgSlot(), TRUE );
		}
		break;
	}
	
#if defined( _DEBUG )
	renderUnitQuickDebugInfo( pRect, pQuickSlot);
#endif

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void									
ItemUnitRenderer::renderUnitQuickDebugInfo( RECT * pRect, QuickSlot * pQuickSlot )
{
	if ( !pQuickSlot )
		return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];	

	SIZE		FontSize;
	WzColor		FontColor;

	FontColor = DEFAULT_FONT_WZCOLOR;

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );

	// quick slot index
	Sprintf(szMessage, _T("%d:%d"), pQuickSlot->GetOrgSlotIndex(), pQuickSlot->GetOrgPos() );

	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMessage, _tcslen(szMessage), &FontSize);
	g_pSunRenderer->x_pManagerTextOut->DrawText( 
		szMessage, 
		(int)pRect->right-(FontSize.cx), 
		(int)pRect->bottom-(FontSize.cy*3), 
		DEFAULT_FONT_WZCOLOR);

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();	
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL ItemUnitRenderer::renderUnitSkill( RECT * pRect, RenderUnit * pRenderUnit, SCSkillSlot * pSkillSlot, BOOL bFromQuickSlot )
{
	assert(pRect && pSkillSlot);
	if (!pRect || !pSkillSlot) 
	{
		return FALSE;
	}

    SLOTCODE skill_code;
    if( bFromQuickSlot == TRUE )
    {
        // 퀵슬롯의 슬롯은 실제 가지고있는 스킬코드를 가져옴
        skill_code = xGetSkillData()->GetRealSkillCode( pSkillSlot->GetCode() );

        if( skill_code == INVALID_SKILL_CODE )
        {
            // 이경우에는 액션등이므로 원래의 코드를 사용
            skill_code = pSkillSlot->GetCode();
        }
    }
    else
    {
        // 스킬창의 슬롯
        skill_code = pSkillSlot->GetCode();
    }

    SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( skill_code );
    
    if ( !pSkillInfo )
        return FALSE;

    if( pSkillInfo->IsStyle() == true )
    {
        if ( m_bShowCooldown && 
            (pRenderUnit->GetState() == SLOT_RENDER_STATE_NONE ||
            pRenderUnit->GetState() == SLOT_RENDER_STATE_ACTIVATED||
            pRenderUnit->GetState() == SLOT_RENDER_STATE_FORCE_ACTIVATED) )
        {
            util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(pSkillInfo->m_SkillCode);
            if (pTimer) 
            {
                //스킬 사용 조건 체크 
                if(pRenderUnit->m_pIconImage)
                {
                    if(!g_pHero->CanUseAction(pSkillInfo->m_SkillCode))
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderWarningSpinCooltime(pRect, rate);
                    }
                    else
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);
                    }
                }
            }
        }
        else
        {
            if( pRenderUnit->GetState() == SLOT_RENDER_STATE_DEACTIVATED )
            {
                renderSlotState(pRect, pRenderUnit );
            }
            else
            {
                if( g_pHero->CanUseStyleLimitWeapon(pSkillSlot->GetCode(), FALSE) == TRUE )
                {
                    renderSlotState(pRect, pRenderUnit );
                }
                else
                {
                    if(pRenderUnit->m_pIconImage)
                    {
                        pRenderUnit->m_pIconImage->RenderWarning(pRect);
                    }
                }
            }
        }
    }
    else if( pSkillInfo->IsActionSkill() == true )
    {
        if(pRenderUnit->m_pIconImage)
        {
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
            // 커스텀 패시브 액션 버튼 일때만으로 한정한다.
            if ( pSkillInfo->m_SkillCode == 35023 )
            {
                util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(pSkillInfo->m_SkillCode);
                if ( pTimer )
                {
                    if(!g_pHero->CanUseSkillWithoutCooltime(pSkillInfo->m_SkillCode, FALSE, true))
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderWarningSpinCooltime(pRect, rate);
                    }
                    else
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);
                    }
                }
            }
            else
            {
                if(!g_pHero->CanUseAction(pSkillSlot->GetCode()))
                    pRenderUnit->m_pIconImage->RenderWarning(pRect);
                else
                    pRenderUnit->m_pIconImage->Render(pRect);
            }
#else
            if(!g_pHero->CanUseAction(pSkillSlot->GetCode()))
                pRenderUnit->m_pIconImage->RenderWarning(pRect);
            else
                pRenderUnit->m_pIconImage->Render(pRect);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
        }
    }
    else if( pSkillInfo->IsEmoticonSkill() == true )
    {
        if(pRenderUnit->m_pIconImage)
        {
            if(!g_pHero->CanUseEmotion(pSkillSlot->GetCode()))
                pRenderUnit->m_pIconImage->RenderWarning(pRect);
            else
                pRenderUnit->m_pIconImage->Render(pRect);
        }
    }
    else 
    {
        if ( m_bShowCooldown && 
            (pRenderUnit->GetState() == SLOT_RENDER_STATE_NONE ||
            pRenderUnit->GetState() == SLOT_RENDER_STATE_ACTIVATED||
            pRenderUnit->GetState() == SLOT_RENDER_STATE_FORCE_ACTIVATED) )
        {
            util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(pSkillInfo->m_SkillCode);

            if (pTimer) 
            {
                if(pRenderUnit->m_pIconImage)
                {
                    if(!g_pHero->CanUseSkillWithoutCooltime(pSkillInfo->m_SkillCode, FALSE, true))
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderWarningSpinCooltime(pRect, rate);
                    }
                    else
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
                        if( g_pHero->IsActivedComboSkill( pSkillInfo->m_SkillClassCode ) )
                        {
                            g_InterfaceManager.RenderComboIconEffect( pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top );
                        }
#endif
                    }
                }
            }

        }
        else 
        {
            if( pRenderUnit->GetState() == SLOT_RENDER_STATE_DEACTIVATED )
            {
                renderSlotState(pRect, pRenderUnit );
            }
            else
            {
                if( g_pHero->CanUseSkillWithoutCooltime(pSkillSlot->GetCode(), FALSE) == TRUE )
                {
                    renderSlotState(pRect, pRenderUnit );
                }
                else
                {
                    if(pRenderUnit->m_pIconImage)
                    {
                        pRenderUnit->m_pIconImage->RenderWarning(pRect);
                    }
                }
            }
        }

        renderAuraActiveImage( pRect, pSkillSlot );
    }



	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderSkillCooltime(RECT * pRect, RenderUnit * pRenderUnit, SCSkillSlot * pSkillSlot, BOOL bFromQuickSlot )
{
	BOOL bRendered = FALSE;

    int skill_code;

    
    if( bFromQuickSlot == TRUE )
    {
        // 퀵슬롯의 슬롯은 실제 가지고있는 스킬코드를 가져옴
        skill_code = xGetSkillData()->GetRealSkillCode( pSkillSlot->GetCode() );

        if( skill_code == INVALID_SKILL_CODE )
        {
            // 이경우에는 액션
            skill_code = pSkillSlot->GetCode();
        }
    }
    else
    {
        // 스킬창의 슬롯
         skill_code = pSkillSlot->GetCode();
    }

    BASE_SKILLINFO * pSkillInfo = (BASE_SKILLINFO *)SkillInfoParser::Instance()->GetSkillInfo( skill_code );

	if (!pSkillInfo) 
	{
		return FALSE;
	}


	if ( m_bShowCooldown && 
		(pRenderUnit->GetState() == SLOT_RENDER_STATE_NONE ||
		 pRenderUnit->GetState() == SLOT_RENDER_STATE_ACTIVATED||
		 pRenderUnit->GetState() == SLOT_RENDER_STATE_FORCE_ACTIVATED) )
	{
		util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(pSkillInfo->m_SkillCode);

		if (pTimer) 
		{
			//스킬 사용 조건 체크 
			if( pSkillInfo->IsActionSkill())
			{
				if(pRenderUnit->m_pIconImage)
				{
                    if(!g_pHero->CanUseAction(pSkillInfo->m_SkillCode))
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderWarningSpinCooltime(pRect, rate);
                    }
                    else
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);
                    }
				}

			}
			else //스킬 일때 
			{
				if(pRenderUnit->m_pIconImage)
				{
                    if(!g_pHero->CanUseSkillWithoutCooltime(pSkillInfo->m_SkillCode, FALSE, true))
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderWarningSpinCooltime(pRect, rate);
                    }
                    else
                    {
                        float rate = (pTimer->GetProgressRatio()) * 100.0f;
                        pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
                        if( g_pHero->IsActivedComboSkill( pSkillInfo->m_SkillClassCode ) )
                        {
                            g_InterfaceManager.RenderComboIconEffect( pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top );
                        }
#endif

                    }
				}

			}			
		}
	}
    else
    {
        BOOL bUseSkillWithOutCooltime = TRUE;
        if( pRenderUnit->GetState() != SLOT_RENDER_STATE_DEACTIVATED )
        {
            if ( pSkillInfo->IsSkill() )
            {
                bUseSkillWithOutCooltime = g_pHero->CanUseSkillWithoutCooltime(pSkillSlot->GetCode(), FALSE);
            }
            else if( pSkillInfo->IsStyle() )
            {
                bUseSkillWithOutCooltime = g_pHero->CanUseStyleLimitWeapon(pSkillSlot->GetCode(), FALSE);
            }
        }


        if (pSkillInfo->IsSkill())
        {
            if( pSkillInfo->IsActionSkill())
            {
                if(pRenderUnit->m_pIconImage)
                {
                    if(!g_pHero->CanUseAction(pSkillSlot->GetCode()))
                        pRenderUnit->m_pIconImage->RenderWarning(pRect);
                    else
                        pRenderUnit->m_pIconImage->Render(pRect);
                }
            }
            else 
                if( pSkillInfo->IsEmoticonSkill() )
                {
                    if(pRenderUnit->m_pIconImage)
                    {
                        if(!g_pHero->CanUseEmotion(pSkillSlot->GetCode()))
                            pRenderUnit->m_pIconImage->RenderWarning(pRect);
                        else
                            pRenderUnit->m_pIconImage->Render(pRect);
                    }
                }
                else
                    if ( FALSE == bUseSkillWithOutCooltime )
                    {
                        if(pRenderUnit->m_pIconImage)
                        {
                            pRenderUnit->m_pIconImage->RenderWarning(pRect);
                        }
                    }
                    else
                    {
                        renderSlotState(pRect, pRenderUnit );
                    }
        }
        else
        {
            if ( FALSE == bUseSkillWithOutCooltime )
            {
                if(pRenderUnit->m_pIconImage)
                {
                    pRenderUnit->m_pIconImage->RenderWarning(pRect);
                }
            }
            else
            {
                renderSlotState(pRect, pRenderUnit );
            }
        }
    }


	return bRendered;
}

//-------------------------------------------------------------------------------------------
/** 퀵슬롯 기간연장체크
*/
BOOL ItemUnitRenderer::renderItemInvalidDate(RECT * pRect,RenderUnit * pRenderUnit, SCItemSlot * pItemSlot)
{
	if(pItemSlot && pItemSlot->IsContainValidItem())
	{
		if ( pItemSlot->GetItemInfo() && 
			pItemSlot->GetItemInfo()->IsChargeItem() ) 
		{
            BYTE charge_subtype = pItemSlot->GetItemRuleInfo().charge_sub_type;
			if ((eCHARGE_SUB_FIXED_AMOUNT_EX == charge_subtype && 
				TRUE == pItemSlot->IsProgressTime()) ||
				eCHARGE_SUB_FIXED_QUANTITY == charge_subtype)
			{
				DWORD dwRemain_hour =(DWORD) pItemSlot->GetDateTime()/(60*60);	
				DWORD dwRemainMin	=(DWORD) pItemSlot->GetDateTime() / 60 % 60;
				if ( 0 == dwRemain_hour && dwRemainMin < 10 )
				{
					// 10분 미만 이다.
					if(pRenderUnit->m_pIconImage)
						pRenderUnit->m_pIconImage->RenderWarning( pRect );

					return TRUE;
				}		
			}
		}


	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderItemInvalid(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot)
{

	SCItemSlot * pRefItemSlot = (SCItemSlot *)pRenderUnit->m_pRefSlot;
	if ( pRefItemSlot->IsBlocked() )
	{
		if(pRenderUnit->m_pIconImage)
			pRenderUnit->m_pIconImage->RenderBlocked( pRect );

		return TRUE;
	}
	else if ( pRefItemSlot->IsLocked() )
	{
		if(pRenderUnit->m_pIconImage)
			pRenderUnit->m_pIconImage->RenderDeactivated( pRect );

		return TRUE;
	}
	else if ( pRefItemSlot->GetItemInfo() && 
		      pRefItemSlot->GetItemInfo()->IsChargeItem() ) 
	{
        BYTE charge_subtype = pRefItemSlot->GetItemRuleInfo().charge_sub_type;
		if ((eCHARGE_SUB_FIXED_AMOUNT_EX == charge_subtype && 
            TRUE == pRefItemSlot->IsProgressTime()) ||
            eCHARGE_SUB_FIXED_QUANTITY == charge_subtype)
		{
			DWORD dwRemain_hour =(DWORD) pItemSlot->GetDateTime()/(60*60);	
			DWORD dwRemainMin	=(DWORD) pRefItemSlot->GetDateTime() / 60 % 60;
			if ( 0 == dwRemain_hour && dwRemainMin < 10 )
			{
				// 10분 미만 이다.
				if(pRenderUnit->m_pIconImage)
					pRenderUnit->m_pIconImage->RenderWarning( pRect );

				return TRUE;
			}		
		}
	}

	
	return FALSE;

}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL									
ItemUnitRenderer::renderItemCooltime(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot)
{
	if( !pItemSlot )
		return FALSE;

	BOOL bRendered = FALSE;

	BASE_ITEMINFO * pItemInfo = 
		ItemInfoParser::Instance()->GetItemInfo( pItemSlot->GetCode() );

	if (!pItemInfo) 
	{
		return FALSE;
	}

	if ( m_bShowCooldown && 
		(pRenderUnit->GetState() == SLOT_RENDER_STATE_NONE ||
		 pRenderUnit->GetState() == SLOT_RENDER_STATE_ACTIVATED) )
	{
		util::Timer* pTimer = xGetCooltimeManager()->GetItemCooltime(pItemInfo->m_wCoolTimeType);

		if (pTimer) 
		{
			if(pRenderUnit->m_pIconImage)
            {
                float rate = (pTimer->GetProgressRatio()) * 100.0f;
                pRenderUnit->m_pIconImage->RenderSpinCooltime(pRect, rate);
            }
			bRendered = TRUE;
		}
	}

	return bRendered;
}

//-------------------------------------------------------------------------------------------
/**
*/
void
ItemUnitRenderer::renderItemOverlapNum(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot)
{
	if(!m_bOverLap || !pItemSlot ) return; 

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );

	SIZE		FontSize;
	
    if(!pItemSlot->GetItemInfo() || pItemSlot->GetItemInfo()->m_byDupNum<1)
    {
        return;
    }

	if (pRenderUnit->GetForceOverLap() || pItemSlot->IsOverlap())
	{
		TCHAR	szNum[256];
        if (pRenderUnit->m_isDiv)
            Sprintf(szNum, _T("%d"), pRenderUnit->m_altNum);
        else
            Sprintf(szNum, _T("%d"), pItemSlot->GetNum());

		size_t len = _tcslen(szNum);
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szNum, len, &FontSize);

		int x = pRect->right  - ( FontSize.cx + len );
		int y = pRect->bottom - ( FontSize.cy );
		SIZE size = { x + FontSize.cx + len, y + FontSize.cy };
		RECT temp = { x, y, size.cx, size.cy };

		g_pSunRenderer->x_pManagerTextOut->DrawText( szNum, &temp,
			DEFAULT_FONT_WZCOLOR, WzColor_RGBA( 0,0,0,0 ), 
			TP_HLEFT,
			ETS_OUTLINE, WzColor_RGBA(  0,0,0,255 ) );

	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}


//-------------------------------------------------------------------------------------------
/**
*/
void									
ItemUnitRenderer::renderQuickItemOverlapNum(RECT * pRect, QuickSlot * pQuickSlot)
{
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );

	SIZE		FontSize;
	WzColor		FontColor;
	FontColor = DEFAULT_FONT_WZCOLOR;

	if ( !pQuickSlot )
		return;
	
	//갯수가 0이라면 안찍겠다
	if(pQuickSlot->GetOverlapCount()==0) return;

	SCItemSlot * pItemSlot = (SCItemSlot *)pQuickSlot->GetOrgSlot();

	if ( pItemSlot && 
		 pItemSlot->IsOverlap() &&
		 pItemSlot->GetItemInfo() &&
		 pItemSlot->GetItemInfo()->m_byDupNum > 1
		)
	{
		TCHAR	szNum[256];
		Sprintf(szNum, _T("%d"), pQuickSlot->GetOverlapCount() );

		size_t len = _tcslen(szNum);
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( szNum, len, &FontSize );

		int x = pRect->right  - ( FontSize.cx + len );
		int y = pRect->bottom - ( FontSize.cy );
		SIZE size = { x + FontSize.cx + len, y + FontSize.cy };
		RECT temp = { x, y, size.cx, size.cy };

		g_pSunRenderer->x_pManagerTextOut->DrawText( szNum, &temp,
			DEFAULT_FONT_WZCOLOR, WzColor_RGBA( 0,0,0,0 ), 
			TP_HLEFT,
			ETS_OUTLINE, WzColor_RGBA(  0,0,0,255 ) );
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

//-------------------------------------------------------------------------------------------
/**
*/
void									
ItemUnitRenderer::renderSlotState(RECT * pRect, RenderUnit * pRenderUnit)
{
	if(!pRenderUnit->m_pIconImage)
		return;

	switch(pRenderUnit->GetState())
	{
	case SLOT_RENDER_STATE_REWARDLIST: 
		{
			pRenderUnit->m_pIconImage->RenderRewardList(pRect);
		}
		break;

	case SLOT_RENDER_STATE_QUESTION: 
		{
			pRenderUnit->m_pIconImage->RenderDeactivated(pRect);
		}
		break;

	case SLOT_RENDER_STATE_BLOCK:
		{
			pRenderUnit->m_pIconImage->RenderBlocked(pRect);
		}
		break;

	case SLOT_RENDER_STATE_DEACTIVATED: 
		{
			pRenderUnit->m_pIconImage->RenderDeactivated(pRect);
		}
		break;
	case SLOT_RENDER_STATE_WARNING:
		{
			pRenderUnit->m_pIconImage->RenderWarning(pRect);
		}
		break;

	case SLOT_RENDER_STATE_ACTIVATED: 
	case SLOT_RENDER_STATE_NONE:
	default:
		{
			pRenderUnit->m_pIconImage->Render(pRect);
		}
		break;	
	}
}

void ItemUnitRenderer::RenderPerkSlot(RECT* perk_slot_rect, RenderUnit* render_unit)
{
    if ((perk_slot_rect == NULL) || 
        (render_unit == NULL)) 
    {
        return;
    }

    // render perk icon
    render_unit->m_pIconImage->Render(perk_slot_rect);
    // render perk level icon
    const SCPerkSlot* perk_slot_ref = static_cast<const SCPerkSlot*>(render_unit->m_pRefSlot);

    if (perk_slot_ref != NULL)
    {
        PerkID perk_id = perk_slot_ref->perk_info()->perk_id_;
        uiPerkDialog::RenderPerkLevelImage(perk_id.level_, perk_slot_rect);
    }    
}
