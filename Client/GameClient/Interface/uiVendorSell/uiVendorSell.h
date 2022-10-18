#ifndef     __USER_INTERFACE_VENDORSELL_H__
#define     __USER_INTERFACE_VENDORSELL_H__
//------------------------------------------------------------------------------
/**
    @class uiVendorSell

    personal vendor sell interface

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiVendorSell_def.h"
#include "SCItemSlotContainer.h"

class uiVendorMan;

class uiVendorSell :
    public uiBase,
    public SCItemSlotContainer  // server client shared container
{
    enum eDIALOG_POS
    {
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
        DIALOG_SLOT00=0,
        DIALOG_SLOT01,
        DIALOG_SLOT02,
        DIALOG_SLOT03,
        DIALOG_SLOT04,
        DIALOG_SLOT05,
		DIALOG_SLOT06,
		DIALOG_SLOT07,

        DIALOG_SLOT10,
        DIALOG_SLOT11,
        DIALOG_SLOT12,
        DIALOG_SLOT13,
        DIALOG_SLOT14,
        DIALOG_SLOT15,
		DIALOG_SLOT16,
		DIALOG_SLOT17,

        DIALOG_SLOT20,
        DIALOG_SLOT21,
        DIALOG_SLOT22,
        DIALOG_SLOT23,
        DIALOG_SLOT24,
        DIALOG_SLOT25,
		DIALOG_SLOT26,
		DIALOG_SLOT27,

        DIALOG_SLOT30,
        DIALOG_SLOT31,
        DIALOG_SLOT32,
        DIALOG_SLOT33,
        DIALOG_SLOT34,
        DIALOG_SLOT35,
		DIALOG_SLOT36,
		DIALOG_SLOT37,

		DIALOG_SLOT40,
		DIALOG_SLOT41,
		DIALOG_SLOT42,
		DIALOG_SLOT43,
		DIALOG_SLOT44,
		DIALOG_SLOT45,
		DIALOG_SLOT46,
		DIALOG_SLOT47,
#else
		DIALOG_SLOT00=0,
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
#endif //_NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE

        DIALOG_TXT_SELLITEM,
        DIALOG_TXT_SELLMSG,
        DIALOG_EDIT_SELLMSG,

        DIALOG_TXT_ITEMCOUNTS,
        DIALOG_TXT_ITEMVALUE,
        DIALOG_TXTVALUE_ITEMCOUNTS,
        DIALOG_TXTVALUE_ITEMVALUTE,
#ifdef _JBH_ADD_VENDOR_COMMISION
		DIALOG_TXT_COMMISION_OWNER,
		DIALOG_TXT_COMMISION_VALUE,
#endif

        DIALOG_BTN_STARTEND,
        DIALOG_BTN_MODIFY,
        DIALOG_BTN_SUBITEM,
        DIALOG_BTN_INPUTMODIFY,
		DIALOG_BTN_PAUSE,

        DIALOG_EXIT,
#ifdef _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
        DIALOG_TXT_ITEM_UNIT_VALUE,
        DIALOG_TXTVALUE_ITEM_UNIT_VALUE,
#endif //_DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
        DIALOG_MAX,
    };

    enum 
    { 
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
        POS_MAX_SLOT    =   DIALOG_SLOT47+1,
#else
		POS_MAX_SLOT    =   DIALOG_SLOT35+1,
#endif //_NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
        POS_SLOT_BEGIN  =   DIALOG_SLOT00,
        POS_SLOT_END    =   POS_MAX_SLOT,
    };

public:
    uiVendorSell(InterfaceManager *pUIMan);
    virtual ~uiVendorSell();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual SCSlot *		CreateSlot();
    virtual VOID			Init(POSTYPE MaxSlotSize);    // SCItemSlotContainer 함수 재정의
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    ItemUnitRenderer *      GetItemUnitRender() const;
    virtual SCSlotContainer * GetSlotContainer();
    void                    SetVendorMessage(TCHAR* pmsg);
	uiVendorMan *			GetManager() {return m_pVendorMan; }

    //--------------------------------------------------------------------------
    // functions
    //
public:
    bool                    IsVending();
    bool                    IsVendedItem(POSTYPE FromPos);
    void                    SetCurrentItemSlotInfo(DURATYPE curAmounts,
                                                   MONEY curMoney,
                                                   POSTYPE curPos);

    void                    VendingStartEnd();
    void                    ItemUp(SLOTIDX fromSlotIdx,
                                   SLOTIDX toSlotIdx,
                                   POSTYPE fromPos,
                                   POSTYPE toPos,
                                   MONEY m);
    void                    ItemUpCencel(SLOTIDX fromSlotIdx,
                                         SLOTIDX toSlotIdx,
                                         POSTYPE fromPos,
                                         POSTYPE toPos);

    void                    OpenItemModifyPopup(POSTYPE pos);
    void                    OpenItemModifyPopupAfterVendingStarted(POSTYPE pos);
    void                    ItemModify(POSTYPE pos, MONEY money, bool isNetSend=false);
    void                    ItemModifyCancel();

    void                    NetSendModifySyncByInsertItem();

    // 아이템 수정요청
    void                    QueryItemModifyRequest(POSTYPE pos);

    // 아이템 빼기
    void                    QueryItemSub(POSTYPE pos);
    void                    RecvItemSub(POSTYPE pos);

    void                    ItemSub(POSTYPE pos);

    // item unit renderer
    void                    InitItemRender();
    void                    ResetItemRenderer();
    void                    FlushUnitDraw();
    void                    ReFreshRenderSlot();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
    virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	
	void                    Exit();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();
    void                    updateVendingMsgs();
    void                    updateVendingBtns();
    void                    updateVendingCurrentItemInfos();
#ifdef _JBH_MOD_MONEY_OUTPUT
	void					updateTooltip(POSTYPE atPos , MONEY money);
	DWORD					m_dwPendingTick;
	BOOL					m_bClickMoney;
	POSTYPE					m_MouseOverPostype;
#endif

	BOOL					EnableOpenVendorState();
	void					CloseSubWindow();
	void					InitDataWhenShowWindow();

	void					OpenVendorSyn();
	void					EndVendorSyn();

	void					PauseVendorSyn();
	void					PauseButtonShowNHide();

	void					setFormatMoney(CCtrlStaticWZ* pControl , MONEY money);

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	
	bool					IsStart() { return m_bStarted;}

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    ItemUnitRenderer *		m_pItemUnitRenderer;
    bool                    m_bStarted;

    bool                    m_bFocus;
    TCHAR                    m_szVendorMsg[MAX_VENDOR_TITLE_LENGTH];

    DURATYPE                m_curAmouts;    // 현재 수량
    MONEY                   m_curMoney;     // 현재 값
    POSTYPE                 m_curPos;       // 현재 슬롯 포지션

    POSTYPE                 m_PendingModifyPos;
    VENDOR_ITEMSLOT         m_PendingModifyItemSlot;  //

	uiVendorMan *			m_pVendorMan;

#ifdef _JBH_ADD_VENDOR_COMMISION
	MONEY					m_curCommision;	// 현재 수수료
#endif


	CCtrlButtonWZ *			m_pButtonPause;

	BOOL					m_bShow;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiVendorSell::getCtrlIDToPos( WzID wzId )
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
inline
CControlWZ *
uiVendorSell::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiVendorSell::IsVending()
{
    return this->m_bStarted;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_VENDORSELL_H__
