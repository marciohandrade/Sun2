#ifndef __SUMMON_SKILL_CONTAINER_H__
#define __SUMMON_SKILL_CONTAINER_H__

//----------------------------------------------------------------------------
/** 소환 스킬 창 클래스
	@author
			김지완
	@since
			
	@remarks
	-2005.12.20 추가 
*/


#pragma once

#include "SolarDialog.h"
#include <SCItemSlotContainer.h>
#include "SummonSkillSlotContainer.h"

class CControlWZ;
class ItemUnitRenderer;

class SummonSkillContainer : public SummonSkillSlotContainer, public SolarDialog
{
public:

	enum eSUMMONSKILL_QUICK_POS
	{
#ifdef _NA_000000_20130114_RENEWER_UI
		SUMMONSKILL_QUICK_BT00 = 0,
		SUMMONSKILL_QUICK_BT01,
		SUMMONSKILL_QUICK_BT02,
		SUMMONSKILL_QUICK_BT03,
		SUMMONSKILL_QUICK_BT04,
#else
		SUMMONSKILL_QUICK_BT00 = 0,
		SUMMONSKILL_QUICK_BT01,
		SUMMONSKILL_QUICK_BT02,
		SUMMONSKILL_QUICK_BT03,
		SUMMONSKILL_QUICK_BT04,
		SUMMONSKILL_QUICK_BT05,
		SUMMONSKILL_QUICK_BT06,
#endif
		SUMMONSKILL_QUICK_MAX,
	};	

	enum 
	{ 
		SUMMONSKILL_START_SLOT = 0,
 #ifdef _NA_000000_20130114_RENEWER_UI
 		SUMMONSKILL_MAX_SLOT = SUMMONSKILL_QUICK_BT04+1,
 #else
		SUMMONSKILL_MAX_SLOT = SUMMONSKILL_QUICK_BT06+1,
 #endif
	};

	SummonSkillContainer();
	virtual ~SummonSkillContainer();

	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );									// 슬롯의 객체 삭제

	// for solarDialog
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );	

	virtual VOID			OnShowWindow(BOOL val);

	VOID			        Clear();
	VOID					FlushUnitDraw();
	VOID					InitItemRender();
	ItemUnitRenderer *		GetItemUnitRender() const;

	VOID					SetTargetKey(DWORD dwTargetKEY);

	VOID					ReFreshRenderSlot();

	int						GetCurTabNum() const { return m_TabNum; }

	POSTYPE					GetPosByCode(CODETYPE code );

	VOID					ForceUpdate();

	BOOL                    UseSummonSkill(eSUMMONSKILL_QUICK_POS pos);
    virtual void MoveDefaultWindowPos();


	void SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }
	DWORD GetDialogKey() { return m_DialogKey; }

protected:

	DWORD m_DialogKey;

	virtual CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	VOID					getControlPos( POSTYPE AtPos, RECT & rectOut );

	static WzID				m_wzId[SUMMONSKILL_QUICK_MAX];
	ItemUnitRenderer *		m_pItemUnitRenderer;
	int						m_TabNum;
	RECT					m_rcSelectCurStyle;

	MouseOverObject			m_MouseOverObject;
	DWORD					m_dwTargetKEY;
	BOOL                    m_bFirst;
};

inline VOID	SummonSkillContainer::ForceUpdate()
{
	if(this->IsVisibleWindow())
		m_bFirst=FALSE;
}

inline VOID SummonSkillContainer::SetTargetKey(DWORD dwTargetKEY)
{
	m_dwTargetKEY = dwTargetKEY;
}
#endif // __STYLE_CONTAINER_H__