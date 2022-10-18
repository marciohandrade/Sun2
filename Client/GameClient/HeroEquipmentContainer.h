//=======================================================================================================================
/** EquipmentContainer
	@author
			유재영
	@since
			
	@remarks
*/


#ifndef __HERO_EQUIPMENT_CONTAINER_H__
#define __HERO_EQUIPMENT_CONTAINER_H__

#pragma once

#include "SolarDialog.h"
#include "equipmentcontainer.h"

class InventoryDialog;
class ItemUnitRenderer;
class ExtendInventoryDialog;
class uiInventoryMan;

#ifdef  _NA_000000_20130114_RENEWER_UI
class uiHelmetShow;
#endif

class HeroEquipmentContainer : public EquipmentContainer, public SolarDialog
{
public:
	HeroEquipmentContainer();
	virtual ~HeroEquipmentContainer();

	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	void					SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }
	DWORD					GetDialogKey() { return m_DialogKey; }

	virtual VOID						UpdateSlot( POSTYPE AtPos, SCSlot & rSlot );
	virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );	//< 슬롯에 객체 추가	
	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );	// 슬롯의 객체 삭제

	virtual VOID			OnInsert( SCSlot & IN rSlotIn );
	virtual VOID			OnDelete( SCSlot & IN rSlotIn );

	virtual void            UpdatePosition();

    virtual VOID            ShowDialog(BOOL val);
	
	VOID					InitItemRender();
	VOID					FlushUnitDraw();
	VOID					ReloadItemRender(BOOL bFlag);
	VOID					ReFreshRenderSlot();
    void RefreshVariation();

	// for solarDialog
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );

    VOID                    SetExtendInventory(ExtendInventoryDialog* pExtendInvenDlg){ m_pExtendInvenDlg = pExtendInvenDlg;}
	VOID					SetInventoryDialog( InventoryDialog * pInvenDlg ) { m_pInvenDlg = pInvenDlg; }
	VOID					UseItem( POSTYPE AtPos );
	
	void                    TransForm();
	void                    UnTransForm();
	virtual ItemUnitRenderer* GetItemUnitRender() const;
	void					ChargeItemExtendSlot(int iExtendCount);
	BOOL					FindEventCode(BYTE byEventCode);	//해당이벤트 코드를 가진아이템존재여부

	void					SetHelmetUI(uiHelmetShow* base) { m_puiHelmet = base; }
protected:
	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	virtual VOID			OnShowWindow(BOOL val);
	virtual CControlWZ*	    getControl(POSTYPE AtPos);
	VOID					getControlPos(POSTYPE AtPos, RECT& rectOut);

#ifdef _NA_002935_20110704_ITEM_SCORE
    void                    DrawItemScore(); //아이템 평점 출력
#endif //_NA_002935_20110704_ITEM_SCORE

	DWORD					m_DialogKey;

	InventoryDialog*		m_pInvenDlg;
	ItemUnitRenderer*		m_pItemUnitRenderer;
    ExtendInventoryDialog*  m_pExtendInvenDlg;
    uiInventoryMan*         m_pInvenMan;

#ifdef  _NA_000000_20130114_RENEWER_UI
	uiHelmetShow*			m_puiHelmet;
#endif

	BOOL					m_bDrawSlotInfo;
	int						m_iDrawSlotPos;


	POSTYPE					m_TabNum;
    BOOL                    m_bCharCreated;
	BOOL                    m_bTransForm;
};

#endif // __HERO_EQUIPMENT_CONTAINER_H__