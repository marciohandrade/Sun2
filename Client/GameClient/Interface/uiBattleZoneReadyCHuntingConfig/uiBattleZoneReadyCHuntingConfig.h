#ifndef     __USER_INTERFACE_BATTLEZONEREADYCHUNTINGCONFIG_H__
#define     __USER_INTERFACE_BATTLEZONEREADYCHUNTINGCONFIG_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneReadyCHuntingConfig

    interface for a competition hunting config in battlezone ready scene

    (C) 2005 ckbang
*/
#include "uiBattleZoneReadyCHuntingConfig_def.h"
#include "uiBase.h"
#include "interfacemanager.h"
struct stCHunting
{
     WORD		wIndex;
	 CODETYPE	MapCode;
     int		iRequireMinLV;
	 WORD       wfieldType;
};

class uiBattleZoneMan;
class uiBattleZoneReadyCHuntingConfig :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_FIELDLIST,
        DLGPOS_BTN_RULES,
        DLGPOS_BTN_RULES_LEFT,
        DLGPOS_BTN_RULES_RIGHT,
        DLGPOS_BTN_PVPMODES,
        DLGPOS_BTN_PVPMODES_LEFT,
        DLGPOS_BTN_PVPMODES_RIGHT,
        DLGPOS_TXT_FIELD,
        DLGPOS_LST_FIELDLIST,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_BEGIN = DLGPOS_BTN_FIELDLIST,
        POS_BTN_END = DLGPOS_BTN_PVPMODES_RIGHT + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
    };

public:
    uiBattleZoneReadyCHuntingConfig(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneReadyCHuntingConfig();

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
    uiBattleZoneMan*        GetManager();
    const uicBattleZoneReadyCHunting_Config& GetCfgs();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    bool                    Clear();
    void                    SetCfg_Field(WORD fieldId);
    void                    SetCfg_FieldByMapCode(MAPCODE mapCode);
    void                    SetCfg_Diff(eHUNTING_MONSTER_DIFFICULT diff);
    void                    SetCfg_Bonus(eHUNTING_BONUS_TYPE bonus);
    void                    SetCfg_CompMode(eCOMPETITION_MODE_TYPE mode);
    void                    SetCfg_CompRule(eCOMPETITION_RULE_TYPE rule);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    updateUIField();
    void                    updateFieldName();
    void                    updateCompModeType();
    void                    updateCompRuleType();
    
    void                    doRuleMoveLeft();
    void                    doRuleMoveRight();
    void                    doPVPMoveLeft();
    void                    doPVPMoveRight();

private:
    void                    uif_ListGen();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    
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
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    CCtrlListWZ*            m_pLstControl;
    CCtrlStaticWZ*          m_pTxtControl;

    bool                    m_bInitControls;

    bool                    m_bShowFieldList;

private:
    uicBattleZoneReadyCHunting_Config   m_cfgs;

	std::vector<stCHunting>				m_vecCHuntingMapIndex;

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneReadyCHuntingConfig::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneReadyCHuntingConfig::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneReadyCHuntingConfig::CheckControls()
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
const uicBattleZoneReadyCHunting_Config&
uiBattleZoneReadyCHuntingConfig::GetCfgs()
{
    return m_cfgs;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneReadyCHuntingConfig::SetCfg_Field(WORD fieldId)
{
    m_cfgs.fieldType = fieldId;

	for(size_t i=0; i<m_vecCHuntingMapIndex.size(); ++i)
	{
		if(m_vecCHuntingMapIndex[i].wfieldType == m_cfgs.fieldType)
		{
			m_cfgs.fieldIndex = m_vecCHuntingMapIndex[i].wIndex;
			break;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneReadyCHuntingConfig::SetCfg_Diff(eHUNTING_MONSTER_DIFFICULT diff)
{
    // FIXME: 헌팅방 생성시 무조건 난이도는 EASY
    m_cfgs.diffcultType = eHUNTING_MONSTER_DIFFICULT_EASY;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneReadyCHuntingConfig::SetCfg_Bonus(eHUNTING_BONUS_TYPE bonus)
{
    m_cfgs.bonusType = bonus;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneReadyCHuntingConfig::SetCfg_CompMode(eCOMPETITION_MODE_TYPE mode)
{
    m_cfgs.compModeType = mode;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneReadyCHuntingConfig::SetCfg_CompRule(eCOMPETITION_RULE_TYPE rule)
{
    m_cfgs.compRuleType = rule;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONEREADYCHUNTINGCONFIG_H__
