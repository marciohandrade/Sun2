#pragma once

#include "uibase.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include <SCItemSlotContainer.h>


class SCItemSlot;

class LotteryInventoryDialog: public uiBase ,public SCItemSlotContainer
{
public:
	LotteryInventoryDialog(InterfaceManager * pUIMan );
	~LotteryInventoryDialog();

	enum eLOTTERY_POS
	{
		LOTTERY_SLOT_00=0, 
		LOTTERY_SLOT_01,
		LOTTERY_SLOT_02,
		LOTTERY_SLOT_03,
		LOTTERY_SLOT_04,
		LOTTERY_SLOT_05,
		LOTTERY_SLOT_06,
		LOTTERY_SLOT_07,
		LOTTERY_TEXT,		
		LOTTERY_BTN_OK,		
		LOTTERY_BTN_CLOSE,
		LOTTERY_MAX
	};

public:
	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual void	OnUpdateSolarDialog();
	virtual void	OnRenderSolarDialog();

	virtual	ItemUnitRenderer * GetItemUnitRender() const { return m_pItemUnitRenderer; }


	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );	// ½½·Ô¿¡ °´Ã¼ Ãß°¡
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	
	void			 SetInvenPos(POSTYPE pos){m_InvenPos = pos;}	
	POSTYPE			 GetInvenPos(){return m_InvenPos; }	
	void			 SendItemUseSync(BOOL bOpen);	
	void			 SendItemCloseSync();	
	void			 SendItemMoveAllSync();
	void			 SetLotteryOpenStatus(BOOL bOpen);
	void			 CreatePandoraSlot(ITEMSTREAMEX &rItemInfo);
	SCSlotContainer* GetSlotContainer();
	void			 SetPandoraBoxName();
	

private:
    void			RefreshLotteryItem(SCItemSlot &rcLottoSlot); 
	void			renderToolTip();	
	void			InitRenderer();
	void			InitControls();
	POSTYPE			getCtrlIDToPos( WzID wzId );
	CControlWZ *	getControl( POSTYPE AtPos );
	void			FlushUnitDraw();
	void			RefreshPandoraSlot();
	
	

private:
	
	POSTYPE					m_InvenPos;
	
	static WzID				m_wzId[LOTTERY_MAX];
	CDrawBase *				m_pDraw;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	SCItemSlot *			m_pItemToolTip;
		
	CCtrlButtonWZ *			m_pCtrlOkBtn;
	CCtrlButtonWZ *			m_pCtrlCloseBtn;

	CCtrlStaticWZ*			m_TxtCtrlTitle;
};

//------------------------------------------------------------------------------
/**
*/
inline	POSTYPE	LotteryInventoryDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < LOTTERY_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return LOTTERY_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ *	LotteryInventoryDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < LOTTERY_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
