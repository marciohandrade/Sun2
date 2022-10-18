#ifndef __ITEM_UNIT_RENDERER_H__
#define __ITEM_UNIT_RENDERER_H__

//=============================================================================================================================
///  CWzUnitDraw class를 통합적으로 그리는 ItemUnitRenderer class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 12
	@remarks
		- 컨텐츠와 엔진부분을 분리하기 위해 CWzUnitDraw를 일괄적으로 Render()하는 클래스

*/
//=============================================================================================================================
#pragma once


#include <Singleton.h>
#include <SolarHashTable.h>

#include "SCSlot.h"

// 아이템 슬롯 관련한 RENDER_STATE
enum eSLOT_RENDER_STATE
{
	SLOT_RENDER_STATE_NONE	   = 0,
	SLOT_RENDER_STATE_QUESTION = 1,
    SLOT_RENDER_STATE_DEACTIVATED,
    SLOT_RENDER_STATE_ACTIVATED,
	SLOT_RENDER_STATE_FORCE_ACTIVATED,
	SLOT_RENDER_STATE_WARNING,
	SLOT_RENDER_STATE_BLOCK,
	SLOT_RENDER_STATE_REWARDLIST,
};


class CWzUnitDraw;
class CDrawBase;

class IconImage;

class SCItemSlot;
class QuickSlot;
class SCSkillSlot;
class DummyItemSlot;
class uiQuestMan;

class ItemUnitRenderer
{
public:
	class RenderUnit
	{
	public:
		RenderUnit()
            : m_fRotateRad(0.f)
            , m_bRotate(FALSE)
            , m_pRefSlot(0)
            , m_pBaseItemInfo(0)
            , m_bForceOverLap(FALSE)
            , m_State(SLOT_RENDER_STATE_NONE) 
        {
        }

		VOID Init()
		{
			m_fRotateRad		= 0.f;
			m_bRotate			= FALSE;
			m_pRefSlot			= 0;
			m_pBaseItemInfo		= 0;
			m_State			    = SLOT_RENDER_STATE_NONE;
            m_altNum            = 0;
            m_isDiv             = false;
            m_pMarkImage        = 0;
            m_pEdgeImage        = 0;
		}

		VOID SetSize( int wid, int hei )
		{
			m_ViewRect.right	= m_ViewRect.left + wid;
			m_ViewRect.bottom	= m_ViewRect.top + hei;

			m_iWidth		= wid;	 
			m_iHeight		= hei;	 
		}

		VOID SetPosition( int x, int y )
		{
			int width			= m_ViewRect.right - m_ViewRect.left;
			int height			= m_ViewRect.bottom - m_ViewRect.top;

			m_ViewRect.left		= x;
			m_ViewRect.top		= y;
			m_ViewRect.right	= x + width;
			m_ViewRect.bottom	= y + height;
		}
		
		void SetShow(BOOL val)
		{
			if (m_pRefSlot && m_pRefSlot->GetCode() == 4001)
			{
				_asm nop;
			}

			m_bShow = val;
		}

		RECT* GetViewRect() { return &m_ViewRect; }
		VOID SetViewRect( RECT * pRect ) { m_ViewRect = *pRect; }
		void SetControl(CControlWZ *pControl) { m_pControl = pControl; }

		RECT GetControlRect()
		{
			if (m_pControl)
			{
				return m_pControl->GetSizeRect();
			}
			else
			{
				return m_ViewRect;
				
			}
		}

        BOOL GetShow() { return m_bShow; }


        //-ck-
        void SetState(eSLOT_RENDER_STATE state) { m_State = state; }
        eSLOT_RENDER_STATE GetState() { return m_State; }
        void SetForceOverLap(BOOL bFlag) { m_bForceOverLap = bFlag; }
        BOOL GetForceOverLap() { return m_bForceOverLap; }

        void SetMarkImage(IconImage* pImage) { m_pMarkImage = pImage; }
        void SetEdgeImage(IconImage* pImage) { m_pEdgeImage = pImage; }
        IconImage* GetMarkImage() { return m_pMarkImage; }
        IconImage* GetEdgeImage() { return m_pEdgeImage; }

	public:
		IconImage*		    m_pIconImage; 
		RESOURCECODE        m_resourcecode;
		const SCSlot*	    m_pRefSlot;
		union 
		{
			BASE_ITEMINFO*	m_pBaseItemInfo;
			BASE_SKILLINFO* m_pBaseSkillInfo;
		};

		int				    m_iWidth;
		int				    m_iHeight;
        int                 m_altNum;
        bool                m_isDiv;
		
	private:
		RECT			    m_ViewRect;
		float			    m_fRotateRad;
		BOOL			    m_bRotate;
		BOOL			    m_bShow;
        eSLOT_RENDER_STATE  m_State;
        BOOL                m_bForceOverLap;
		CControlWZ*         m_pControl;
        IconImage*          m_pMarkImage;
        IconImage*          m_pEdgeImage;
	};


private:
    util::SolarHashTable<RenderUnit *> *	m_pItemUnitHashTable;
    util::CMemoryPoolFactory<RenderUnit> *	m_pRenderUnitPool;
    CDrawBase *								m_pDraw;

    BOOL                                    m_bShowCooldown;
    BOOL                                    m_bWide;
    BOOL                                    m_bOverLap;
    bool									bRenderNumber_;

    uiQuestMan*                             m_pQuestManager;

public:
	ItemUnitRenderer();
	~ItemUnitRenderer();

	VOID									Init( CDrawBase * pDraw, DWORD dwPoolSize );
	VOID									SetOverLap(BOOL bOverLap){ m_bOverLap = bOverLap;}
	VOID									Release();

	VOID									FlushUnitDraw();

	VOID									Update();
	VOID									Render();

	ItemUnitRenderer::RenderUnit *			AddItemUnit( DBSERIAL serial, CODETYPE code, RECT * pRect, const SCSlot * pSlot ,CControlWZ *pControl = NULL);
	ItemUnitRenderer::RenderUnit *			AddItemUnit( DBSERIAL serial, CODETYPE code, RECT * pRect, const SCSlot * pSlot ,bool RenderNumber ,CControlWZ *pControl = NULL);
	VOID									RemoveItemUnit( DBSERIAL serial );
    void                                    ChangePositionItemUnit(DBSERIAL serial, RECT* pRect, CControlWZ *pControl);
	ItemUnitRenderer::RenderUnit *			GetItemUnit( DBSERIAL serial ) { return m_pItemUnitHashTable->GetData((DWORD)serial ); }

	void                                    ShowCooldown(BOOL bFlag);
	void                                    RecalculateViewRect();

	void                                    SetWideFlag(BOOL bWide);

    void                                    RenderPerkSlot(RECT* perk_slot_rect, RenderUnit* render_unit);

    void                                    RefreshItemMark();

    util::SolarHashTable<RenderUnit*>*      GetItemUnitHashTable() const { return m_pItemUnitHashTable; }

private:
    IconImage* SelectItemEdge(BASE_ITEMINFO* base_item_info, SCItemSlot* item_slot);
    IconImage* SelectItemMark(BASE_ITEMINFO* base_item_info);
    void renderIconImageForItem(RECT* rect_ptr,RenderUnit* render_unit,IconImage* item_mark_image);
    void renderEnchantNum(RECT* pRect, SCItemSlot* pItemSlot, RenderUnit* pRenderUnit);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	void renderAwakeningNum(RECT* rect, SCItemSlot* item_slot, RenderUnit* render_unit);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

	void									renderItemOverlapImg( RECT * pRect,SCItemSlot * pItemSlot,RenderUnit * pRenderUnit);
	BOOL									isValidUnit( RenderUnit * pRenderUnit );

	BOOL									renderUnit( RenderUnit * pRenderUnit );
    BOOL									renderUnitItem( RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot);
	
	//넘버링을 처리 하지 않는 드로우
	BOOL									renderUnitItemNotNumber( RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot );

	BOOL									renderUnitQuickItem( RECT * pRect, RenderUnit * pRenderUnit, QuickSlot * pQuickSlot);
    BOOL									renderUnitDummy( RECT * pRect, RenderUnit * pRenderUnit, DummyItemSlot * pDummySlot );


	BOOL									renderItemInvalid(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot);
	BOOL									renderItemCooltime(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot);
    void									renderItemOverlapNum(RECT * pRect, RenderUnit * pRenderUnit, SCItemSlot * pItemSlot);

	void									renderQuickItemOverlapNum(RECT * pRect, QuickSlot * pQuickSlot);

	BOOL									renderUnitQuick( RECT * pRect, RenderUnit * pRenderUnit );
	void									renderUnitQuickDebugInfo( RECT * pRect, QuickSlot * pQuickSlot);

	BOOL									renderUnitSkill( RECT * pRect, RenderUnit * pRenderUnit, SCSkillSlot * pSkillSlot, BOOL bFromQuickSlot);
	BOOL									renderSkillCooltime(RECT * pRect, RenderUnit * pRenderUnit, SCSkillSlot * pSkillSlot, BOOL bFromQuickSlot);
		
	void									renderSlotState(RECT * pRect, RenderUnit * pRenderUnit);

	BOOL									renderItemInvalidDate(RECT * pRect,RenderUnit * pRenderUnit, SCItemSlot * pItemSlot);
	

    void									renderPetLockImg( RECT * pRect,SCItemSlot * pItemSlot );
#ifdef _NA_004035_20120227_COSTUME_ITEM
    void									renderCostumeLockImg( RECT * pRect,SCItemSlot * pItemSlot );
#endif// _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	void                                    renderDecorateCostumeLockImg( RECT * pRect,SCItemSlot * pItemSlot );
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    void                                    renderBadgeitemActiveImage( RECT* pRect, SCItemSlot* pItemSlot );
    void                                    renderPCBangItemActiveImage( RECT* pRect, SCItemSlot* pItemSlot );
    void                                    renderAuraActiveImage( RECT* pRect, SCSkillSlot* pSkillSlot );

    void                                    renderItemCustomizingImg(RECT* rect, SCItemSlot* slot_item, RenderUnit* render_unit);
    void                                    RenderSpinCooltime(RECT* rect, RenderUnit* render_unit, SCItemSlot* item_slot);

    void                                    renderCharCustomizeImg(RECT* rect, SCItemSlot* slot_item);

    void                                    renderWingsLockImg(RECT* area, SCItemSlot* item_slot);
    void									renderRiderLockImg( RECT * pRect,SCItemSlot * pItemSlot );
    void									renderEtherChargeImg(RECT* pRect, SCItemSlot* pItemSlot, RenderUnit* pRenderUnit);
    void									renderPersnalWaypointImg(RECT* pRect, SCItemSlot* pItemSlot);

    void									renderPossessionImg( RECT * pRect,SCItemSlot * pItemSlot );
#ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
    void									renderDamageImg( RECT * pRect,SCItemSlot * pItemSlot );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

    void                                    renderAddMarkForItem(RECT* pRect, SCItemSlot* pItemSlot, RenderUnit* pRenderUnit);


};

#endif // __ITEM_UNIT_RENDERER_H__