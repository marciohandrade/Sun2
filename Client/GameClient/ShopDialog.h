//=======================================================================================================================
/** 상점
	@author
			유재영
	@since
			
	@remarks
*/

#pragma once

#include "solardialog.h"
#include <SCItemSlotContainer.h>
#include "MouseHandler.h"
#include <ShopInfoParser.h>
#include <SCItemSlot.h>

#define SHOP_ID		1

class SCSlot;
class SCSlotContainer;
class CControlWZ;
class ItemUnitRenderer;
class MouseHandler;


class ShopDialog : public SolarDialog , public SCItemSlotContainer
{

public:

	enum SHOPDIALOG_POS
	{
		SHOP_BT_SLOT1 = 0,
		SHOP_BT_SLOT2,
		SHOP_BT_SLOT3,
		SHOP_BT_SLOT4,
		SHOP_BT_SLOT5,
		SHOP_BT_SLOT6,
		SHOP_BT_SLOT7,
		SHOP_BT_SLOT8,
		SHOP_BT_SLOT9,
		SHOP_BT_SLOT10,
		SHOP_BT_SLOT11,
		SHOP_BT_SLOT12,
		SHOP_BT_SLOT13,
		SHOP_BT_SLOT14,
		SHOP_BT_SLOT15,
		SHOP_BT_SLOT16,
		SHOP_BT_SLOT17,
		SHOP_BT_SLOT18,
		SHOP_BT_SLOT19,
		SHOP_BT_SLOT20,
		SHOP_BT_SLOT21,
		SHOP_BT_SLOT22,
		SHOP_BT_SLOT23,
		SHOP_BT_SLOT24,
		SHOP_BT_SLOT25,
		SHOP_BT_SLOT_HISTORY1,
		SHOP_BT_SLOT_HISTORY2,
		SHOP_BT_SLOT_HISTORY3,
		SHOP_BT_SLOT_HISTORY4,
		SHOP_BT_SLOT_HISTORY5,
		SHOP_TEXT_REPAIR_PAYMENT,
		SHOP_CHECK_BUY,
		SHOP_CHECK_SELL,
		SHOP_CHECK_REPAIR,
		SHOP_BT_REPAIR_ALL,		
		SHOP_CHECK_BERSERKER,
		SHOP_CHECK_DRAGON,
		SHOP_CHECK_VALKYRIE,
		SHOP_CHECK_WEAPON,
		SHOP_CHECK_GENERAL,
        SHOP_PIC_COST_ICON,
		SHOP_BT_EXIT,
		Dialog_MAX
	};

	enum { SHOP_MAX_SLOT = SHOP_BT_SLOT25+1 };

	enum eSHOP_TAB
	{
		Shop_Tab_1 = 0,
		Shop_Tab_2,
		Shop_Tab_3,
		Shop_Tab_4,
		Shop_Tab_5,

		Shop_Type_MAX
	};

	enum SHOP_METHOD
	{ 
		SHOP_METHOD_NONE = -1,
		SHOP_METHOD_BUY = 0, 
		SHOP_METHOD_SELL,
		SHOP_METHOD_REPAIR,

		SHOP_METHOD_MAX
	};

	enum HISTORY_SLOT
	{
		HISTORY_SLOT1	= 125,
		HISTORY_SLOT2	= 126,
		HISTORY_SLOT3	= 127,
		HISTORY_SLOT4	= 128,
		HISTORY_SLOT5	= 129,
		MAX_HISTORY_SLOT = 5,
	};

public:
	ShopDialog(void);
	virtual ~ShopDialog(void);

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();


	VOID			InitContainer();
	VOID			InitRenderer();
	VOID			ReleaseContainer();
	VOID			FlushUnitDraw();

	ItemUnitRenderer *		GetItemUnitRender() const;

	// 실제 슬롯정보관련한 위치
	POSTYPE					GetRealSlotPos( POSTYPE pos );
	// 인터페이스 컨트롤 위치.
	POSTYPE					GetRelativeSlotPos( POSTYPE pos );

	VOID					ReFreshRenderSlot();


	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual BOOL	UpdateData(BOOL bTake);	

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	
	RC::eSLOT_INSERT_RESULT					PushHistorySlot( SCSlot & rSlot);
	BOOL									PopHistorySlot();
	void									RegisteAllHistoryItemSlot();
	void									SetPendingHistorySlot(POSTYPE AtPos) {m_PendingHistorySlotPos = AtPos;}
	POSTYPE									GetPendingHistorySlot() {return m_PendingHistorySlotPos;}
	void									NET_SEND_CG_ITEM_REPURCHASE_SYN(POSTYPE pos);
	void									ClearAllHistorySlot();

	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();

	virtual VOID	OnShowWindow(BOOL val);

	BOOL 			GetControlsFromDialog();

	DWORD			GetDialogKey() { return m_DialogKey; }
	void			SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

	// 상점에 특화된 메소드
public:
	BOOL			SettingShop();
	VOID			SetShopID(DWORD dwID);
	DWORD			GetShopID() const;
	BYTE			GetCurrentTab() { return m_iClickedIndexTab; }

protected:
	BOOL			AddItem(eSHOP_TAB eShopTabIndex, POSTYPE AtPos, SHOPINFO::SHOP_ITEM * pShopItem);	
	VOID			updateMoney();
    void            updateToolTip();

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
    void            SetCostIconFromType();
public:
	VOID			SetCheckTrade(SHOP_METHOD eShopMethod );
	VOID			ClickTabCheckButton(int iIndex);
    SLOTCODE currency_item_code(){ return currency_item_code_; };
    BYTE shop_type(){ return shop_type_; }
    DWORD get_price();// 마우스가 올라간 아이템이나 하임의 가격을 얻어온다

#ifdef _JBH_MOD_MONEY_OUTPUT
private:
	void			updateToolTip();
	DWORD			m_dwPendingTick;
	BOOL			m_bCkickMoney;
#endif

private:
	DWORD				m_DialogKey;

	int					m_dwShopID;
	MONEY				m_Money;
	SHOPINFO *			m_pShopListInfo;

	TCHAR				m_szText[512];

	POSTYPE				m_TabNum;
	int					m_iClickedIndexTab;

	BOOL				m_bDrawSlotInfo;
	int					m_iDrawSlotPos;

	SCItemSlot			m_ItemHistorySlot[MAX_HISTORY_SLOT];
	POSTYPE				m_PendingHistorySlotPos;


	// controls
private:
	CCtrlStaticWZ *			m_pCtrlStaticRepairMoney;
	CCtrlButtonCheckWZ *	m_pCtrlCheckShopButtons[SHOP_METHOD_MAX];
	CCtrlButtonCheckWZ *	m_pCtrlCheckTabShopButtons[Shop_Type_MAX];
    CCtrlPictureWZ* cost_icon_contol_;
	SHOP_METHOD				m_eShopMethod;

private:
	ItemUnitRenderer *		m_pItemUnitRenderer;
    SLOTCODE currency_item_code_;
    BYTE shop_type_;
    POSTYPE mouse_over_position_;
    DWORD prices_[SHOPINFO::_MAX_TAB_NUM][SHOPINFO::_MAX_SELLITEM_NUM];
	static WzID		m_wzId[Dialog_MAX];
};

inline DWORD ShopDialog::get_price() // 마우스가 올라간 아이템이나 하임의 가격을 얻어온다
{
    if (m_iClickedIndexTab < SHOPINFO::_MAX_TAB_NUM && m_iClickedIndexTab >= 0 &&
        mouse_over_position_ < SHOPINFO::_MAX_SELLITEM_NUM && mouse_over_position_ >= 0)
    {
        return prices_[m_iClickedIndexTab][mouse_over_position_];
    }
    return 0;
}
