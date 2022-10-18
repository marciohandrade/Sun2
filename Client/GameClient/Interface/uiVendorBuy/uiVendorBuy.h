#ifndef     __USER_INTERFACE_VENDORBUY_H__
#define     __USER_INTERFACE_VENDORBUY_H__
//------------------------------------------------------------------------------
/**
    @class uiVendorBuy

    personal vendor buy interface

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiVendorBuy_def.h"
#include "SCItemSlotContainer.h"

class uiVendorMan;

class uiVendorBuy :
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
        DIALOG_EDIT_SELLMSG,
        DIALOG_TXT_SELLMSG,

        DIALOG_TXT_ITEMCOUNTS,
        DIALOG_TXT_ITEMVALUE,
        DIALOG_TXTVALUE_ITEMCOUNTS,
        DIALOG_TXTVALUE_ITEMVALUTE,
        DIALOG_TXT_ITEM_UNIT_PRICE,

        DIALOG_BTN_ITEMBUY,

        DIALOG_EXIT,

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
    uiVendorBuy(InterfaceManager *pUIMan);
    virtual ~uiVendorBuy();

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
	uiVendorMan *			GetManager() { return m_pVendorMan; }

    //--------------------------------------------------------------------------
    // functions
    //
public:
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	void					SendItemBuySync(POSTYPE pos  , int num = NULL);
#else
    void                    SendItemBuySync(POSTYPE pos );
#endif
    void                    ItemSub(POSTYPE pos);
    void                    QueryView(DWORD key);
    void                    QueryViewEnd();

    // item unit renderer
    void                    InitItemRender();
    void                    ResetItemRenderer();
    void                    FlushUnitDraw();
    void                    ReFreshRenderSlot();

	void					SetBuyItemsSearchFoucs(POSTYPE pos,byte count ,LONGLONG price);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
    virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

	//bitstream
	virtual void			Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType );
    void                    Exit();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();
#ifdef _JBH_MOD_MONEY_OUTPUT
	void					updateToolTip();
	DWORD					m_dwPendingTick;
	BOOL					m_bClickMoney;
#endif
    void                    updateVendingCurrentItemInfos();

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

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    inline void ClearCheckItem();
    inline void SetCheckItem(POSTYPE pos, bool check);
    inline bool GetCheckItem(POSTYPE pos);
    void RenderCheckItem();
    ItemUnitRenderer *		m_pItemUnitRenderer;
	uiVendorMan *			m_pVendorMan;
    TCHAR                   m_szVendorMsg[MAX_VENDOR_TITLE_LENGTH + 1];
    DURATYPE                m_curAmouts;    // 현재 수량
    MONEY                   m_curMoney;     // 현재 값
    POSTYPE                 m_curPos;       // 현재 슬롯 포지션
    DWORD                   m_dwSalerKey;   // 판매자의 오브젝트키
    bool check_item_[POS_MAX_SLOT]; //check item !
};
//------------------------------------------------------------------------------
/**
*/
inline void uiVendorBuy::ClearCheckItem()
{
    ZeroMemory (&check_item_, sizeof (check_item_));
}
//------------------------------------------------------------------------------
/**
*/
inline bool uiVendorBuy::GetCheckItem(POSTYPE pos)
{
    if ((pos >= 0) &&
        (pos < POS_MAX_SLOT))
    {
        return check_item_[pos];    

    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
inline void uiVendorBuy::SetCheckItem(POSTYPE pos, bool check)
{
    if ((pos >= 0) &&
        (pos < POS_MAX_SLOT))
    {
       check_item_[pos] = check;

    }
}
//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiVendorBuy::getCtrlIDToPos( WzID wzId )
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
uiVendorBuy::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}



//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_VENDORBUY_H__
