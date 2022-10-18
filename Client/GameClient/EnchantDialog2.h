#pragma once

#ifdef _NA_003966_20111227_ADD_ENCHANT
#include "EnchantDialog3.h"
#else


#include "solardialog.h"
#include "ItemOptionHeader.h"
#include "SCItemSlotContainer.h"
#include "EnchantParser.h"

class SCItemSlot;
class EnchantDialog : public SolarDialog,
                      public SCItemSlotContainer
{
private:
	DWORD m_DialogKey;

public:
	DWORD GetDialogKey() { return m_DialogKey; }
	void SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

public:
    enum eENCHANT_POS
    {
        ENCHANT_BTN_SLOT_1 = 0, // 바꾸지 말것
        ENCHANT_BTN_SLOT_2,
        ENCHANT_BTN_SLOT_3, 
        ENCHANT_BTN_SLOT_4, 
        ENCHANT_BTN_SLOT_5, 
        ENCHANT_BTN_EXIT,
        ENCHANT_BTN_ENCHANT,
        ENCHANT_BTN_RETRY,
        ENCHANT_BTN_CANCEL,
        ENCHANT_CHK_25_PER,
        ENCHANT_CHK_50_PER,
        ENCHANT_CHK_75_PER,
        ENCHANT_CHK_100_PER,
        ENCHANT_TXT_TITLE,
        ENCHANT_TXT_KIND,
        ENCHANT_TXT_ITEM_NAME,
        ENCHANT_TXT_MAIN_SUB_MAT1,
        ENCHANT_TXT_MAIN_SUB_MAT2,
        ENCHANT_TXT_MAIN_SUB_MAT3,
        ENCHANT_TXT_MAIN_SUB_MAT4,
        ENCHANT_TXT_MAIN_SUB_MAT1_NUM,
        ENCHANT_TXT_MAIN_SUB_MAT2_NUM,
        ENCHANT_TXT_MAIN_SUB_MAT3_NUM,
        ENCHANT_TXT_MAIN_SUB_MAT4_NUM,
        ENCHANT_TXT_REQUIRE_MONEY,
        ENCHANT_VSCR_SUB,
        ENCHANT_TXT_TITLE2,
        Dialog_Max
    };

    enum ePACKET_STATUS
    {
        PACKET_STATUS_NONE = 0,
        PACKET_STATUS_ENCHANT = 1,
    };

    EnchantDialog( void );
    virtual ~EnchantDialog( void );

    void            Init( CDrawBase* pDrawBase );
    virtual void    Release( void );

    virtual void    update( void );
    virtual void    render( void );

    virtual void    NetworkProc( MSG_BASE* pMsg );
    virtual void    MessageProc( SI_MESSAGE* pMsg );

    void            Enchant( void );
    void            ReTry( void );
    void            Exit( void );

    void            SetTargetInfo( SLOTIDX fromSlotIdx, POSTYPE fromPos );
    void            SetCashEnchant( BOOL val );
	BOOL			IsPendingEnchant(POSTYPE pSlotPos);

    SCSlotContainer* GetEnchantContainer() { return this; }

    void OnEnchantItem_InventoryRightClick(const SLOTIDX from_container_index, 
                                           const POSTYPE from_container_position);

protected:
    CControlWZ*     getControl( POSTYPE atPos );
    POSTYPE         getCtrlIDToPos( WzID wzId );

    virtual void    OnShowWindow( BOOL show );

    void            InitDialog( void );
    void            EnableDialog( BOOL enable );

    void            UpdateMouseOverObj( void );
    void            UpdateTooltip( void );

    virtual RC::eSLOT_INSERT_RESULT InsertSlot( POSTYPE atPos, SCSlot & slot );

    BOOL            DeleteSlot( POSTYPE atPos, SCSlot * slotOut );
    void            DeleteAllSlots( void );

    BOOL            IsCashEnchant( void );
    BOOL            IsValidEnchant( void );

    void            ActivateProbabilityTab( int index );
    int             GetValidProbabilityIdx( int defIdx );

    eENCHANT_OPTION ProbabilityIdxToOpt( int idx );

    void            OnLButtonClick( SI_MESSAGE* pMsg );

    void            OnEnchantItemSlotClick( void );
    void            OnEnchantItem(void);
    void            OnProbabilityTabClick( POSTYPE pos );
    void            OnBtnClick( POSTYPE pos );

    void            OnEnchantSuccessAck( MSG_BASE* pMsg );
    void            OnEnchantFailedAck( MSG_BASE* pMsg );
    void            OnItemEnchantNak( MSG_BASE* pMsg );

private:
    void            CreateItemRenderer( void );
    void            ReleaseItemRenderer( void );

    void            InitUiControls( void );
    void            InitAllInfos( void );

    void            ResetEnchantLevel( void );
    void            ResetEnchantMatsForLv( void );

    void            ResetEnchantMatList( void );
    void            RecalcEnchantCost( void );

    void            RefreshDefEnchantInfo( void );
    void            RefreshEnchantMatInfo( void );
    void            RefreshEnchantMatList( void );

    void            ClearEnchantMatList( void );

    BOOL            HaveEnchantMaterials( void );
    BOOL            HaveEnchantCost( void );

    void            RemoveEnchantMatsFromInven( void );

	

private:
    typedef std::map<POSTYPE, POSTYPE> LockInfoMap;
    typedef std::vector<sMATERIALS> MaterialList;

    static WzID     m_wzId[Dialog_Max];

    ItemUnitRenderer* m_itemRenderer;

    CCtrlStaticWZ*  m_ctlEnchantLvl;
    CCtrlStaticWZ*  m_ctlEnchantCost;
    CCtrlStaticWZ*  m_ctlItemName;
    CCtrlStaticWZ*  m_ctlMatName[4];
    CCtrlStaticWZ*  m_ctlMatNum[4];
    CCtrlButtonCheckWZ* m_ctlProbability[4];
    CCtrlVScrollWZ* m_ctlMatListScroll;

    LockInfoMap     m_lockInfoMap;

    POSTYPE         m_targetPos;
    POSTYPE         m_pendingTargetPos;

    const MATERIALS_FOR_LEVEL* m_matsForLv;
    MaterialList    m_matList;

    eENCHANTLEVEL   m_enchantLevel;
    MONEY           m_enchantCost;

    eENCHANT_OPTION m_enchantOpt;
    int             m_probabilityIdx;
};

//----------------------------------------------------------------------------
/**
*/
inline void EnchantDialog::SetTargetInfo( SLOTIDX /*fromSlotIdx*/, POSTYPE fromPos )
{
    m_targetPos = fromPos;
}

//----------------------------------------------------------------------------
/**
*/
inline void EnchantDialog::SetCashEnchant( BOOL val )
{
    m_enchantOpt = ( val ? eENCHANT_CASH : eENCHANT_NOT_OPT );
}

//----------------------------------------------------------------------------
/**
*/
inline BOOL EnchantDialog::IsCashEnchant( void )
{
    return ( m_enchantOpt & eENCHANT_CASH );
}

//----------------------------------------------------------------------------
/**
*/
inline BOOL EnchantDialog::IsValidEnchant( void )
{
    assert( GetMaxSlotNum() > 0 );

    return ( GetSlot( 0 ).GetCode() != 0 );
}

#endif