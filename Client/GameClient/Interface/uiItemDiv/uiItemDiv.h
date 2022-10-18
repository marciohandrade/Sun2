#ifndef     __USER_INTERFACE_ITEM_DIV_H__
#define     __USER_INTERFACE_ITEM_DIV_H__
//------------------------------------------------------------------------------
/**
    @class uiItemDiv

    interface for a item division dialog

    (C) 2006 ckbang
*/
#include "uiBase.h"
#include "uiItemDiv_def.h"

#include "ItemUnitRenderer.h"

class uiItemDiv :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_OK,
        DLGPOS_BTN_CANCEL,
        DLGPOS_BTN_L_DEC,
        DLGPOS_BTN_R_INC,
        DLGPOS_EDT_NUM,
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        DLGPOS_BTN_CLOSE, 
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
        DIALOG_MAX,
    };

public:
    uiItemDiv(InterfaceManager *pUIMan);
    virtual ~uiItemDiv();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    SetData(DURATYPE divNum,
                                    SLOTIDX fromSlotIdx, 
                                    POSTYPE fromPos, 
                                    eSlotType slotType, 
                                    SLOTCODE code, 
                                    ItemUnitRenderer::RenderUnit *	pRenderUnit, 
                                    RECT * pRect);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );

    void                    OnOkBtnClick();
    void                    OnLDecBtnClick();
    void                    OnRIncBtnClick();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
    //virtual void            UpdatePosition();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlDxIMEEditBox*      m_pEdtControl;
    CCtrlButtonWZ*          m_pBtnControls[2];
    CCtrlButtonWZ*          m_pBtnOk;
    CCtrlButtonWZ*          m_pBtnCancel;

    bool                    m_bInitControls;

    DURATYPE                m_divNum;
    SLOTIDX                 m_fromSlotIdx;
    POSTYPE                 m_fromPos;
    eSlotType               m_slotType;
    SLOTCODE                m_code;
    ItemUnitRenderer::RenderUnit *	m_pRenderUnit; 
    RECT                    m_Rect;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiItemDiv::getCtrlIDToPos( WzID wzId )
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
uiItemDiv::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiItemDiv::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiItemDiv::SetData(DURATYPE divNum,
                   SLOTIDX fromSlotIdx, 
                   POSTYPE fromPos, 
                   eSlotType slotType, 
                   SLOTCODE code, 
                   ItemUnitRenderer::RenderUnit *	pRenderUnit, 
                   RECT * pRect)
{
    m_divNum = divNum - 1;
    m_fromSlotIdx = fromSlotIdx;
    m_fromPos = fromPos;
    m_slotType = slotType;
    m_code = code;
    m_pRenderUnit = pRenderUnit; 
    SetRect (&m_Rect, pRect->left, pRect->top, pRect->right, pRect->bottom);
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_ITEM_DIV_H__
