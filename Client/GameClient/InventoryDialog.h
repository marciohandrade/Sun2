//=======================================================================================================================
/** 단축창 클래스
	@author
			유재영
	@since
			
	@remarks
*/


#ifndef __INVENTORY_DIALOG_H__
#define __INVENTORY_DIALOG_H__

#pragma once

#include "uiBase.h"
#include <SCItemSlotContainer.h>

#include "uiInventoryMan/uiInventoryMan.h"


class SCSlot;
class SCSlotContainer;
class CControlWZ;
class ItemUnitRenderer;
class SCItemSlot;
class InterfaceManager;
class ExtendInventoryDialog;
enum eCHARGE_CACHE_TYPE
{
	eCHARGE_CACHE_NONE,
	eCHARGE_CACHE_TRY_REQUEST_INFO,		// 정보 요청후.
	eCHARGE_CACHE_RECEIVED_INFO,		// 정상적으로 정보를 받음.
};

TYPEDEF_MAP(CHARGE_CACHE_INFO, POSTYPE, eCHARGE_CACHE_TYPE);



class InventoryDialog : public uiBase, public SCItemSlotContainer
{
public:
	enum eINVEN_POS
	{
		INVEN_TB00 = 0,
		INVEN_TB01,
		INVEN_TB02,
		INVEN_TB03,
		INVEN_TB04,
		INVEN_TB05,
		INVEN_TB06,
		INVEN_TB07,
		INVEN_TB08,
		INVEN_TB09,
		INVEN_TB10,
		INVEN_TB11,
		INVEN_TB12,
		INVEN_TB13,
		INVEN_TB14,
		INVEN_TEXT_MONEY,
		INVEN_BT_MONEY,
		INVEN_CHECK_1_PAGE,
		INVEN_CHECK_2_PAGE,
		INVEN_CHECK_3_PAGE,
        INVEN_CHECK_4_PAGE,
		INVEN_CHECK_5_PAGE,

		INVEN_CHECK_CHARGE_1_PAGE,
		INVEN_CHECK_CHARGE_2_PAGE,
		INVEN_CHECK_CHARGE_3_PAGE,
		INVEN_CHECK_CHARGE_4_PAGE,
		INVEN_CHECK_CHARGE_5_PAGE,

        INVEN_BT_OPEN_EVENT,		
		INVEN_BT_WASTE_BASKET,

		INVEN_BT_UNLOCK,
		INVEN_BT_LOCK,

		INVEN_TXT_TITLE,
		INVEN_FLASH_BT_OPEN_EVENT,
		INVEN_PERSONAL_WAYPOINT_BTN,

        INVEN_BT_SETTING,
        INVEN_BT_EXTEND,
        INVEN_BT_EXIT,
		INVEN_MAX,
	};

	enum 
	{ 
		INVEN_MAX_SLOT	= INVEN_TB14+1,
					
		INVEN_TAB_CHARGE_MAX = 5,
		INVEN_TAB_MAX		 = 10,		

	};

private:
	enum eINVENTORY_LOCK_STATE
	{
		eINVENTORY_LOCK_STATE_UNLOCK = 0,
		eINVENTORY_LOCK_STATE_LOCK,

		eINVENTORY_LOCK_STATE_MAX,
	};



	enum CurrentInvenContainer
	{
		INVENTORY = 0,
		TEMP_INVENTORY,
	};

public:
	InventoryDialog(InterfaceManager* pUIMan);
	virtual ~InventoryDialog();

	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	virtual VOID							UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot );

	virtual VOID			OnInsert( SCSlot & IN rSlotIn );
	virtual VOID			OnDelete( SCSlot & IN rSlotIn );

	//virtual void            UpdatePosition();

	// 실제 슬롯정보관련한 위치
	POSTYPE					GetRealSlotPos( POSTYPE pos );
	// 인터페이스 컨트롤 위치.
	POSTYPE					GetRelativeSlotPos( POSTYPE pos );

	VOID					InitRenderer();
	VOID					FlushUnitDraw();
 
	inline int				GetMaxInvenSlotNum(){return INVEN_MAX_SLOT;}
	inline int				GetMaxInvenChargeNum(){return INVEN_TAB_CHARGE_MAX;}

	uiInventoryMan*			GetManager();
	int						getTooltipString(POSTYPE pos);


	virtual ItemUnitRenderer *
							GetItemUnitRender() const;
	inline CDrawBase *		GetDrawBase() const;

	BOOL                    m_bDrawSlotInfo;
	int                     m_iDrawSlotPos;

public:
	VOID					UpdateQuickPos();
    VOID                    UpdatePage();

	virtual VOID			ReFreshRenderSlot();
	VOID					ReFreshRenderSlotALL();
    CurrentInvenContainer	GetCurInvenContainer(){ return m_eCurInvenContainer; }
    void SetExtendInven(ExtendInventoryDialog* extend_inven){ m_pExtendInventoryDialog = extend_inven; }
    void					SetbEventInventory(BOOL bVal);
	void					SetUpdateQuest(bool bUpdate) { m_bUpdateQuest = bUpdate; }
    VOID					ClickETCBtn(POSTYPE pos);
    VOID					ClickSlotBtn(POSTYPE RealPos, RECT rcSlot);
    VOID					DoubleClickSlotBtn(POSTYPE pos);
    VOID					updateMoney(CCtrlStaticWZ *pControl);
    void                    RightClickProcess(POSTYPE real_pos);
    void                    MouseOverProcess(POSTYPE RelativePos);
    void                    SetTooltipString(int StringCode){ m_iTooltipStringCode = StringCode; }
    void                    ProcessToolTip(bool bMouseOver, WzID dialog_key);
    void                    HideWindowProcess();
    void                    InitTitleByLockState(CCtrlStaticWZ* pTxtCtrlTitle, 
                                CCtrlButtonWZ* pCtrlBtnLock, CCtrlButtonWZ* pCtrlBtnUnlock);
    void                    InventoryUnitRender();
    void                    ImpossibleCrystalizationItemIconDeActiveRender();
    void                    RevertCrystalizationItemIconDeActiveRender();
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	void                    ImpossibleAwakeningItemIonDeActiveRender();
	void                    RevertAwakeningItemIconDeActiveRender();
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
    void                    RefreshItemMark();

    void                    SetInventoryMouseOverType(const eMOUSE_OVER_TYPE mouse_over_type);
protected:
	
	POSTYPE					getCtrlIDToPos( WzID wzId );

	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	virtual VOID            Process(DWORD dwTick);

	VOID					getControlPos( POSTYPE AtPos, RECT & rectOut );
	virtual CControlWZ *	getControl( POSTYPE AtPos );
	virtual VOID			OnShowWindow(BOOL val);
	virtual	BOOL			GetControlsFromDialog();

	// MESSAGE HANDLER
	VOID					ClickCheckButton(int iIndex );	
	
    void                    EnableWasteBasket();
	void					InitDlgShowNHide(CCtrlButtonWZ* pCtrlBtnLock, CCtrlButtonWZ* pCtrlBtnUnlock);
	void					SetInventoryLockState( PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE iInventoryLockState );
	void					SetPasswordDlg(int iStringTableNum, int iPasswordDlgState);

	void					PasswordLockingSyn( DWORD dwStrIndex );
#ifdef _NA_000000_20130114_RENEWER_UI
	void					UpdateExtraInventoryPage();
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	// 캐시아이템 슬롯 클릭시 처리
	VOID					ClickCheckChargeBtn(POSTYPE pos);
	VOID					ReceivedChargeInfo(POSTYPE pos);


	// NOTICE BUG 
	VOID					CheckValidInsertWithNotice(POSTYPE pos);

	virtual BOOL			UpdateData(BOOL bTake);

	void					updateToolTip( void );
	int						getTooltipString( void );

	int						m_iTooltipStringCode;
	map<int,int>			m_UseTooltipCtrls;

	VOID					refreshInventoryRenderUnit();
	
	POSTYPE					m_TabNum;
	static WzID				m_wzId[INVEN_MAX];

	CDrawBase *				m_pDraw;

	ItemUnitRenderer *		m_pItemUnitRenderer;
	CurrentInvenContainer	m_eCurInvenContainer;

	SCItemSlot *			m_pItemToolTip;

private:
    ExtendInventoryDialog*  m_pExtendInventoryDialog;
	uiInventoryMan*			m_pInventoryMan;

	CCtrlButtonCheckWZ *	m_pCtrlCheckTab[INVEN_TAB_MAX];


	CCtrlButtonWZ *			m_pCtrlBtnLock[eINVENTORY_LOCK_STATE_MAX];

	CCtrlStaticWZ*			m_TxtCtrlTitle;
    CCtrlStaticWZ*          m_MoneyText;
	bool                    m_bControlsInitialized;
	int						m_iPasswordDlgState;
	int						m_iInventoryLockState;


	int						m_iClickedIndexTab;
	int						m_iPrevClickedIndexTab;

	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];
	TCHAR					m_szNumber[INTERFACE_STRING_LENGTH];

	CHARGE_CACHE_INFO_MAP	m_CharCacheInfoMap;

	DWORD					m_dwFlashTickForEventInven;
	BOOL					m_bFlashEventInven;
	BOOL					m_bEventInventory;
	CCtrlButtonWZ*			m_pCtrlEventBtn;

	bool					m_bUpdateQuest;
#ifdef _NA_000000_20130114_RENEWER_UI
	int						save_inventory_Extra_;
	int						save_inventory_Extra_Next_;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    TCHAR					m_hotkey[2][INTERFACE_STRING_LENGTH];
#endif

public:
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    void CheckModifyOption();
#endif
};

// inline methods
inline CDrawBase * InventoryDialog::GetDrawBase() const
{ 
	return m_pDraw; 
}

#endif // __INVENTORY_DIALOG_H__