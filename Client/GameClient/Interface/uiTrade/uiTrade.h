#ifndef     __USER_INTERFACE_TRADE_H__
#define     __USER_INTERFACE_TRADE_H__
//------------------------------------------------------------------------------
/**
    @class uiTrade

    personal trading interface

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiTrade_def.h"
#include "SCItemSlotContainer.h"
#include "ResultCode.h"
using namespace RC;

class uiTradeMan;
class uiTrade :
    public uiBase,
    public SCItemSlotContainer  // server client shared container
{
    enum eDIALOG_POS
    {
        DLGPOS_SLOT00 = 0,
        DLGPOS_SLOT01,
        DLGPOS_SLOT02,
        DLGPOS_SLOT03,
        DLGPOS_SLOT04,
        DLGPOS_SLOT05,
        DLGPOS_SLOT06,
        DLGPOS_SLOT07,
        DLGPOS_SLOT08,
        DLGPOS_SLOT09,

        DLGPOS_SLOT10,
        DLGPOS_SLOT11,
        DLGPOS_SLOT12,
        DLGPOS_SLOT13,
        DLGPOS_SLOT14,
        DLGPOS_SLOT15,
        DLGPOS_SLOT16,
        DLGPOS_SLOT17,
        DLGPOS_SLOT18,
        DLGPOS_SLOT19,

        DLGPOS_BTN_1_MONEY,
        DLGPOS_BTN_2_MONEY,

        DLGPOS_BTN_ACCEPT,      // 거래 수락
        DLGPOS_BTN_PROPOSAL,    // 거래 제시
        DLGPOS_BTN_CANCEL,      // 거래 취소

        DLGPOS_TXT_1_ITEM_NAME,
        DLGPOS_TXT_2_ITEM_NAME,

        DLGPOS_TXT_1_STATUS,
        DLGPOS_TXT_2_STATUS,

        DLGPOS_TXT_1_MONEY,
        DLGPOS_TXT_2_MONEY,

#ifdef _NA_000000_20130114_RENEWER_UI
#else
		DLGPOS_TXT_COMPLETE,    // 거래 수락
        DLGPOS_TXT_ACCEPT,      // 거래 제시
        DLGPOS_TXT_CANCEL,      // 거래 취소
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
        DLGPOS_BTN_EXIT,

        DIALOG_MAX,
    };

    enum
    {
        POS_1_SLOT_BEGIN              = DLGPOS_SLOT00,
        POS_1_SLOT_END                = DLGPOS_SLOT09+1,
        POS_1_SLOT_SIZE               = POS_1_SLOT_END - POS_1_SLOT_BEGIN,

        POS_2_SLOT_BEGIN              = DLGPOS_SLOT10,
        POS_2_SLOT_END                = DLGPOS_SLOT19+1,
        POS_2_SLOT_SIZE               = POS_2_SLOT_END - POS_2_SLOT_BEGIN,
    };

public:
    uiTrade(InterfaceManager *pUIMan);
    virtual ~uiTrade();

    
    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

private:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiTradeMan*             GetManager();

    ItemUnitRenderer *      GetItemUnitRender() const;
    virtual SCSlotContainer * GetSlotContainer();

    void                    PutOneStart();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    // item unit renderer
    void                    InitItemRender();
    void                    ResetItemRenderer();
    void                    FlushUnitDraw();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
    virtual BOOL			DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
    bool                    IsInInventoryItem(POSTYPE pos);

    //------------------------------------------------------------------------------
    inline
    int
    GetSlotSize1()
    {
        return POS_1_SLOT_SIZE;
    };

    //------------------------------------------------------------------------------
    inline
    int
    GetSlotSize2()
    {
        return POS_2_SLOT_SIZE;
    };

    void                        TradeEnd(TRADE_TOTAL_INFO tradeInfo);

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();

    void                    updateStatus();

    void                    renderToolTip();

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

	TYPEDEF_MAP(EXTRA_INFO, POSTYPE, POSTYPE);

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    inline void ClearCheckItem();
    inline void SetCheckItem(POSTYPE pos, bool check);
    inline bool GetCheckItem(POSTYPE pos);
    void RenderCheckItem();
    uiTradeMan*             m_pTradeMan;

    CCtrlButtonWZ*          m_pSlotBtnControls1[POS_1_SLOT_SIZE];
    CCtrlButtonWZ*          m_pSlotBtnControls2[POS_2_SLOT_SIZE];
    CCtrlButtonWZ*          m_pBtnMoneys[2];
    CCtrlButtonWZ*          m_pBtnComplete;
    CCtrlButtonWZ*          m_pBtnAccept;
    CCtrlButtonWZ*          m_pBtnCancel;
    CCtrlStaticWZ*          m_pTxtItemNames[2];
    CCtrlStaticWZ*          m_pTxtStatus[2];
    CCtrlStaticWZ*          m_pTxtMoneys[2];
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	CCtrlStaticWZ*          m_pTxtComplete;
    CCtrlStaticWZ*          m_pTxtAccept;
    CCtrlStaticWZ*          m_pTxtCancel;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    ItemUnitRenderer *		m_pItemUnitRenderer;

	EXTRA_INFO_MAP			m_LockedItemPosMap;
    bool check_item_[POS_2_SLOT_SIZE]; //check item !
	
};
//------------------------------------------------------------------------------
/**
*/
inline void uiTrade::ClearCheckItem()
{
     ZeroMemory (&check_item_, sizeof (check_item_));
}
//------------------------------------------------------------------------------
/**
*/
inline bool uiTrade::GetCheckItem(POSTYPE pos)
{
    if ((pos >= 0) &&
        (pos < GetSlotSize2()))
    {
        return check_item_[pos];    
        
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
inline void uiTrade::SetCheckItem(POSTYPE pos, bool check)
{
    //유효범위체크(상대방슬롯 10~19까지만)
    if ((pos >= GetSlotSize2()) &&
        (pos <  GetSlotSize1() + GetSlotSize2()))
    {   
        pos -= GetSlotSize2();
        check_item_[pos] = check;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiTrade::getCtrlIDToPos( WzID wzId )
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
inline CControlWZ * uiTrade::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline uiTradeMan* uiTrade::GetManager()
{
    return this->m_pTradeMan;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_TRADE_H__
