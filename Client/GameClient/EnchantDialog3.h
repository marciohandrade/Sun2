#pragma once

#ifdef _NA_003966_20111227_ADD_ENCHANT

#include "solardialog.h"
#include "ItemOptionHeader.h"
#include "SCItemSlotContainer.h"
#include "EnchantParser.h"


#include "ItemSlotContainer.h"
#include "uiSystemMan/uiSystemMessage.h"

#define MAX_VISIBLE_MATERIAL_COUNT 3



class SCItemSlot;
class EnchantDialog : public SolarDialog
{
private:
    ItemSlotContainer m_SlotContainer[3];
	DWORD m_DialogKey;

public:

	DWORD GetDialogKey() { return m_DialogKey; }
	void SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

    void SetTargetItem( int inventory_slot_index );
    void SetExtraItem( int inventory_slot_index );

    SCItemSlotContainer* GetEnchantContainer() { return m_SlotContainer[0].GetSlotContainer(); }
    SCItemSlotContainer* GetEnchantOptionItemContainer() { return m_SlotContainer[2].GetSlotContainer(); }

    //----------------------------------------------------------------------------
    enum eENCHANT_POS
    {
        ENCHANT_BTN_SLOT_TARGET = 0, // 바꾸지 말것

        ENCHANT_BTN_SLOT_2, // 재료슬롯 1
        ENCHANT_BTN_SLOT_3, // 재료슬롯 2
        ENCHANT_BTN_SLOT_4, // 재료슬롯 3
        ENCHANT_BTN_SLOT_5, // 추가재료슬롯(옵션)

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

        ENCHANT_TXT_MAIN_SUB_OPTION,      // 추가재료 

        ENCHANT_TXT_MAIN_SUB_MAT1_NUM,
        ENCHANT_TXT_MAIN_SUB_MAT2_NUM,
        ENCHANT_TXT_MAIN_SUB_MAT3_NUM,

        ENCHANT_TXT_REQUIRE_MONEY,
        ENCHANT_VSCR_SUB,

        ENCHANT_TXT_TITLE2,
        ENCHANT_TXT_EXTRA_TITLE,

        Dialog_Max
    };

    enum eENCHANT_TAB
    {
        ENCHANT_TAB_25,
        ENCHANT_TAB_50,
        ENCHANT_TAB_75,
        ENCHANT_TAB_100,
        ENCHANT_TAB_MAX,
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

    virtual void    OnUpdateSolarDialog();
    virtual void    OnRenderSolarDialog();

    virtual void    NetworkProc( MSG_BASE* pMsg );
    virtual void    MessageProc( SI_MESSAGE* pMsg );

    void            Enchant( void );
    void            ReTry( void );
    void            Exit( void );

    SCItemSlot* GetTargetItem();
    SCItemSlot* GetExteraItem();

    void            SetCashEnchant( BOOL val );
    BOOL			IsPendingEnchant(POSTYPE pSlotPos);

    const MATERIALS_FOR_LEVEL* GetCurrentEnchantInfo() { return m_matsForLv; }

    void OnEnchantItem_InventoryRightClick(const SLOTIDX from_container_index, 
                                           const POSTYPE from_container_position);

    static void Callback_SendPacketItemEnchantSyn(uiSystemMessage::ButtonIndex button_index);
    DWORD& enchant_popup_dialog_key() { return enchant_popup_dialog_key_; }

protected:
    CControlWZ*     getControl( POSTYPE atPos );
    POSTYPE         getCtrlIDToPos( WzID wzId );

    virtual void    OnShowWindow( BOOL show );

    void            InitDialog( void );
    void            EnableDialog( BOOL enable );

    void            UpdateMouseOverObj( void );
    void            UpdateTooltip( void );

    //virtual RC::eSLOT_INSERT_RESULT InsertSlot( POSTYPE atPos, SCSlot & slot );

    //BOOL            DeleteSlot( POSTYPE atPos, SCSlot * slotOut );
    //void            DeleteAllSlots( void );


    BOOL            IsCashEnchant( void );
    BOOL            IsValidEnchant( void );

    void            ActivateProbabilityTab( int index );
    int             GetValidProbabilityIdx( int defIdx );

    eENCHANT_OPTION ProbabilityIdxToOpt( int idx );

    void            OnLButtonClick( SI_MESSAGE* pMsg );
    void            OnRButtonClick( SI_MESSAGE* pMsg );

    void            OnEnchantItemSlotClick( void );
    void            OnChangedEnchantItem(void);
    void            OnChangedExtraItem(void);
    void            OnProbabilityTabClick( POSTYPE pos );
    void            OnBtnClick( POSTYPE pos );

    void            OnEnchantSuccessAck( MSG_BASE* pMsg );
    void            OnEnchantFailedAck( MSG_BASE* pMsg );
    void            OnItemEnchantNak( MSG_BASE* pMsg );

private:
    void            InitUiControls( void );
    void            InitAllInfos( void );

    //
    void            ResetEnchantLevel( void );
    void            ResetEnchantMatsForLv( void );
    void            ResetEnchantMatList( void );
    void            ResetEnchantExtraMaterialSlot();

    void            RecalcEnchantCost( void );

    void            RefreshDefEnchantInfo( void );
    void            RefreshEnchantMatInfo( void );
    void            RefreshEnchantMatList( void );

    //void            ClearEnchantMatList( void );

    BOOL            HaveEnchantMaterials( void );
    BOOL            HaveEnchantCost( void );

    void            RemoveEnchantMatsFromInven( void );

    void TargetItemRock(bool is_rock);
    void ExtraItemRock(bool is_rock);
    
    void SendPacketItemEnchantSyn(bool is_send_packet);

private:
    typedef std::map<POSTYPE, POSTYPE> LockInfoMap;
    typedef std::vector<sMATERIALS> MaterialList;

    static WzID     m_wzId[Dialog_Max];

    ItemUnitRenderer* m_itemRenderer;

    CCtrlStaticWZ*  m_ctlEnchantLvl;
    CCtrlStaticWZ*  m_ctlEnchantCost;
    CCtrlStaticWZ*  m_ctlItemName;

    CCtrlStaticWZ*  m_ctlMatName[MAX_VISIBLE_MATERIAL_COUNT];
    CCtrlStaticWZ*  m_ctlMatNum[MAX_VISIBLE_MATERIAL_COUNT];

    CCtrlStaticWZ*  m_ctlTargetTitle;
    CCtrlStaticWZ*  m_ctlExtraMatTitle;
    CCtrlStaticWZ*  m_ctlExtraMatName;

    COLORREF m_DefaultTextColor;
    COLORREF m_InvalidTextColor;
    COLORREF m_TitleDefaultColor;
    COLORREF m_TitleTextColor1;
    COLORREF m_TitleTextColor2;

    //CCtrlStaticWZ*  m_ctlExtraMatNum;


    CCtrlButtonCheckWZ* m_ctlProbability[ENCHANT_TAB_MAX];

    CCtrlStaticWZ*  m_ctlOptionMatName;
    CCtrlButtonCheckWZ* m_ctlOptionProbability;

    CCtrlVScrollWZ* m_ctlMatListScroll;

    LockInfoMap     m_lockInfoMap;

    POSTYPE         m_targetPos;
    POSTYPE         m_extraItemPos; // 추가첨가물
    POSTYPE         m_pendingTargetPos;

    const MATERIALS_FOR_LEVEL* m_matsForLv;
    MaterialList    m_matList;

    eENCHANTLEVEL   m_enchantLevel;
    MONEY           m_enchantCost;

    eENCHANT_OPTION m_enchantOpt;
    int             m_probabilityIdx;

    DWORD enchant_popup_dialog_key_;
};

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
    return m_SlotContainer[0].GetSlotItem(0) != NULL;
}


#endif //_NA_003966_20111227_ADD_ENCHANT