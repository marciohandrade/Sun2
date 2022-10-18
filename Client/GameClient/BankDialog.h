//=======================================================================================================================
/** 뱅크
	@author
			유재영
	@since
			
	@remarks
		- 창을 열 때 MSG_CG_WAREHOUSE_START_SYN 정보를 보내고 
		  MSG_CG_WAREHOUSE_START_ACK 를 받아야 창을 열어준다.	
		- 창을 닫을 때 MSG_CG_WAREHOUSE_END_SYN 정보를 보내고
		  MSG_CG_WAREHOUSE_END_ACK 를 받아야 창을 닫아준다.
*/

#pragma once

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "uiBase.h"
#include <SCItemSlotContainer.h>


#include "uiBankMan/uiBankMan.h"


struct MSG_CG_ITEM_WAREHOUSEITEMALL_ACK;

class InterfaceManager;
#ifndef __001347_LTJ_REMOVE_TEMP_INVENTORY	//if'N'def
class TempInventoryContainer;
#endif//__001347_LTJ_REMOVE_TEMP_INVENTORY	
class BitStream;

class BankDialog : public uiBase, public SCItemSlotContainer
{
	enum BANK_POS
	{
		BANK_BT_SLOT_1 = 0,
		BANK_BT_SLOT_2,
		BANK_BT_SLOT_3,
		BANK_BT_SLOT_4,
		BANK_BT_SLOT_5,
		BANK_BT_SLOT_6,
		BANK_BT_SLOT_7,
		BANK_BT_SLOT_8,
		BANK_BT_SLOT_9,
		BANK_BT_SLOT_10,
		BANK_BT_SLOT_11,
		BANK_BT_SLOT_12,
		BANK_BT_SLOT_13,
		BANK_BT_SLOT_14,
		BANK_BT_SLOT_15,
		BANK_BT_SLOT_16,
		BANK_BT_SLOT_17,
		BANK_BT_SLOT_18,
		BANK_BT_SLOT_19,
		BANK_BT_SLOT_20,
		BANK_CHECK_1,
		BANK_CHECK_2,
		BANK_CHECK_3,
		BANK_CHECK_4,
		BANK_CHECK_5,
		BANK_TEXT_MONEY,		// 얼마있냐를 나타냄.
		BANK_BT_MONEY,			// 돈을 빼낼때 얼마 가져올지에 대한 버튼
		BANK_CHECK_WITHDRAW,	// 빼오다
		BANK_CHECK_DEPOSIT,		// 넣다.
		BANK_BT_EXIT,
		BANK_TI_EXTRA_BANK,		// 보조 창고
		BANK_BTN_UNLOCK,		// 창고 잠금 해제
		BANK_BTN_LOCK,			// 창고 잠금

		BANK_TXT_TITLE,
		
		DIALOG_LOCK_PICTURE_P033,
		DIALOG_LOCK_PICTURE_P032,
		DIALOG_LOCK_PICTURE_P034,
		
		Dialog_MAX
	};

	enum 
	{ 
		BANK_PAGE_PER_SLOT = 20,
		BANK_MAX_SLOT = BANK_BT_SLOT_20 + 1,	
		BANK_CHECK_TAB_MAX = 5,
	};

	enum eBANK_MSG 
	{ 
		eBANK_MSG_WITHDRAW = 0,	
		eBANK_MSG_DEPOSIT,		
		eBANK_MSG_LEAK_RATE,		// 수수료가 부족하다.
		eBANK_MSG_FULL_SLOT,		// 꽉찼다.
		eBANK_MSG_EMPTY,

		eBANK_MSG_MAX
	};

	enum eBANK_CHECK_TYPE
	{
		eBANK_CHECK_TYPE_WITHDRAW = 0,
		eBANK_CHECK_TYPE_DEPOSIT,

		eBANK_CHECK_TYPE_MAX,
	};

	enum eBANK_LOCK_STATE
	{
		eBANK_LOCK_STATE_UNLOCK = 0,
		eBANK_LOCK_STATE_LOCK,

		eBANK_LOCK_STATE_MAX, 
	};


	enum eBANK_PACKETSTATUS
	{
		eBANK_START_PACKET	= 1,
		eBANK_END_PACKET	= 2,
	};

public:
	BankDialog(InterfaceManager* pUIMan);
	virtual ~BankDialog(void);

public:
	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual VOID	MouseOver_MsgProc( SI_MESSAGE * pMessage );

	virtual BOOL	UpdateData(BOOL bTake);

	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();


	virtual VOID	OnShowWindow(BOOL val);

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

	virtual VOID	OnInsert( SCSlot & IN rSlotIn );
	virtual VOID	OnDelete( SCSlot & IN rSlotIn );

	VOID			InitContainer();
	VOID			ReleaseContainer();

	VOID			popupInputMoney();

	// 실제 슬롯정보관련한 위치
	POSTYPE			GetRealSlotPos( POSTYPE pos );
	// 인터페이스 컨트롤 위치.
	POSTYPE			GetRelativeSlotPos( POSTYPE pos );

	VOID			InitRenderer();
	VOID			FlushUnitDraw();
	VOID			ReFreshRenderSlot();
	
#ifndef __001347_LTJ_REMOVE_TEMP_INVENTORY	//if'N'def
	VOID			FlushUnitDrawTempInven();
	VOID			RefreshRenderTempInvenSlot();
#endif//__001347_LTJ_REMOVE_TEMP_INVENTORY	

	VOID			SendWareHouseStart();
	VOID			SendWareHouseEnd();

	

	virtual void	Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType );

	virtual	ItemUnitRenderer * GetItemUnitRender() const { return m_pItemUnitRenderer; }


	VOID			SetMoney(MONEY Money)					{m_Money = Money;}
	MONEY			GetMoney()								{return m_Money;}	
	void			SetBankLockState(int iBankLockState)	{GetManager()->SetBankLockState(iBankLockState);}
	int				GetBankLockState()						{return GetManager()->GetBankLockState();}

	VOID			initBankBtn();

	VOID			initBankLockBtn();
	void			InitTitleByLockState( int iLockState );

	VOID			CheckBankBtn(eBANK_CHECK_TYPE eCheckBtn);
	VOID			CheckBankTab(int iTab);
	VOID			ResetCheckBankTab();


	void			SetPasswordDlg(int iStringTableNum, int iPasswordDlgState = eBankDlgState_None);

	VOID			GetControls();

#ifndef __001347_LTJ_REMOVE_TEMP_INVENTORY	//if'N'def
	VOID			ChangeTempInven(BOOL bFlag);
	TempInventoryContainer *	GetTempInventory();
#endif//__001347_LTJ_REMOVE_TEMP_INVENTORY	
							
protected:

	uiBankMan*		GetManager();

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	void			updateToolTip( void );
	int				getTooltipString(POSTYPE pos);

	void			PasswordLockingSyn( DWORD dwStrIndex );


	// controls
	CCtrlStaticWZ *			m_pCtrlMoneyStatic;
	CCtrlButtonCheckWZ *	m_pCtrlCheckBtn[eBANK_CHECK_TYPE_MAX];

	CCtrlButtonWZ *			m_pCtrlBtnLock[eBANK_LOCK_STATE_MAX];
	CCtrlStaticWZ*			m_TxtCtrlTitle;



private:

	uiBankMan*				m_pBankMan;

	int						m_iTooltipStringCode;
	map<int,int>			m_UseTooltipCtrls;


	static WzID				m_wzId[Dialog_MAX];

	POSTYPE					m_TabNum;

	CDrawBase *				m_pDraw;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	eBANK_CHECK_TYPE		m_eCheckType;

	BOOL					m_bSendPacket;

	MONEY                   m_Money;
	TCHAR					m_szResult[0xff];
	TCHAR					m_szNumber[0xff];
	int						m_iClickTab;
	BOOL					m_bStarted;

#ifndef __001347_LTJ_REMOVE_TEMP_INVENTORY	//if'N'def
	BOOL					m_bExtraBank;		// 임시인벤인지 확인
	TempInventoryContainer *m_pTempItemContainer;
#endif//__001347_LTJ_REMOVE_TEMP_INVENTORY		

	CCtrlButtonCheckWZ *	m_pCtrlCheckTab[BANK_CHECK_TAB_MAX];
};

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
