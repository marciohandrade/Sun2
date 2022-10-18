#pragma once

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "uibase.h"
#include "SCItemSlotContainer.h"
#include "uiGuildMan/uiGuildMan.h"

class BitStream;

#define MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM 3

class uiGuildWareHouse : public uiBase, public SCItemSlotContainer
{
public:
	enum eDIALOG_POS
	{
		DIALOG_SLOT00,
		DIALOG_SLOT01,
		DIALOG_SLOT02,
		DIALOG_SLOT03,
		DIALOG_SLOT04,
		DIALOG_SLOT05,

		DIALOG_SLOT10,
		DIALOG_SLOT11,
		DIALOG_SLOT12,
		DIALOG_SLOT13,
		DIALOG_SLOT14,
		DIALOG_SLOT15,

		DIALOG_SLOT20,
		DIALOG_SLOT21,
		DIALOG_SLOT22,
		DIALOG_SLOT23,
		DIALOG_SLOT24,
		DIALOG_SLOT25,

		DIALOG_SLOT30,
		DIALOG_SLOT31,
		DIALOG_SLOT32,
		DIALOG_SLOT33,
		DIALOG_SLOT34,
		DIALOG_SLOT35,

		DIALOG_TXT_GUILD_NAME,
		DIALOG_TXT_MONEY,

		DIALOG_EXIT_BTN,
		DIALOG_MONEY_BTN,
		DIALOG_LOG_BTN,

		DIALOG_SLOT_CHECK_BTN1,
		DIALOG_SLOT_CHECK_BTN2,
		DIALOG_SLOT_CHECK_BTN3,

		DIALOG_OUT_CHECK_BTN,
		DIALOG_IN_CHECK_BTN,

		DIALOG_LOCK_PICTURE_P033,
		DIALOG_LOCK_PICTURE_P032,
		DIALOG_LOCK_PICTURE_P034,

		DIALOG_MAX,
	};

	enum 
	{ 
		POS_MAX_SLOT    =   DIALOG_SLOT35+1,

		POS_SLOT_BEGIN  =   DIALOG_SLOT00,
		POS_SLOT_END    =   POS_MAX_SLOT,
	};

public:
	uiGuildWareHouse(InterfaceManager * pUIMan);
	virtual ~uiGuildWareHouse();

public:
	virtual void            Init(CDrawBase* pDrawBase);
	virtual void            Release();

	void					InitControl();
    void                    InitButton();

	void					InitContainer();
	void					ReleaseContainer();
	void					FlushUnitDraw();

	void					InitRenderer();
	void					ReleaseRenderer();

public:
	virtual	ItemUnitRenderer * GetItemUnitRender() const { return m_pItemUnitRenderer; }

public:
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ½½·Ô¿¡ °´Ã¼ Ãß°¡
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnRenderSolarDialog();
	virtual void            OnUpdateSolarDialog();
	virtual void			Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType );

	uiGuildMan*				GetManager();
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	void					CheckTab(int iCurCheckTab);
	void					CloseSlotInfo(int maxTab);
	void					CheckInOutBtn(eDIALOG_POS eDlgPos, bool bCheck);

	void					popupInputMoney();
	void					UpdateTooltip();

public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	void					SetMoney(MONEY Money)					{m_Money = Money;}
	MONEY					GetMoney()								{return m_Money;}	
	void					SetMaxTab(BYTE btMaxTab)				{m_btMaxTab = btMaxTab;}
	void					ReFreshRenderSlot();
	SCSlotContainer*		GetSlotContainer();

private:
	uiGuildMan*				m_pGuildMan;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	CDrawBase *				m_pDraw;

	MONEY					m_Money;
	BOOL					m_bShow;
	static WzID				m_wzId[DIALOG_MAX];

	CCtrlStaticWZ*			m_pCtrlGuildNameText;
	CCtrlStaticWZ*			m_pCtrlMoneyText;
	CCtrlButtonWZ*			m_pCtrlExitBtn;
	CCtrlButtonWZ*			m_pCtrlRigthBtn;
	CCtrlButtonWZ*			m_pCtrlMoneyBtn;
	CCtrlButtonCheckWZ*		m_pCtrlOutCheckBtn;
	CCtrlButtonCheckWZ*		m_pCtrlInCheckBtn;
	CCtrlButtonWZ*			m_pCtrlLogBtn;
	CCtrlButtonCheckWZ*		m_pCtrlSlotCheckBtn[MAX_GUILD_WAREHOUSE_SLOT_TAB_NUM];

	BYTE					m_btCurTab; //1,2,3
	BYTE					m_btMaxTab; //»ç¿ëÇÒ¼ö ÀÖ´Â ½½·ÔÅÇ °¹¼ö



};

//------------------------------------------------------------------------------
/**
*/
inline	POSTYPE	uiGuildWareHouse::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ *	uiGuildWareHouse::getControl( POSTYPE AtPos )
{
	assert( AtPos < DIALOG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
