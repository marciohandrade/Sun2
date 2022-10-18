#ifndef     __USER_INTERFACE_BATTLEZONEREADYMISSIONCONFIG_H__
#define     __USER_INTERFACE_BATTLEZONEREADYMISSIONCONFIG_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneReadyMissionConfig

    interface for a mission config in battlezone ready scene

    (C) 2005 ckbang
*/
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiBase.h"
#include "uiBattleZoneReadyMissionConfig_def.h"

class uiBattleZoneMan;
class uiBattleZoneReadyMissionConfig :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_CHK_MAIN_MISSION,
        DLGPOS_CHK_SUB_MISSION,
        DLGPOS_BTN_SUB_MISSION_UP,
        DLGPOS_BTN_SUB_MISSION_DOWN,

        DIALOG_MAX,
    };

    enum
    {
        POS_CHK_BEGIN = DLGPOS_CHK_MAIN_MISSION,
        POS_CHK_END = DLGPOS_CHK_SUB_MISSION + 1,
        POS_CHK_SIZE = POS_CHK_END - POS_CHK_BEGIN,

        POS_BTN_BEGIN = DLGPOS_BTN_SUB_MISSION_UP,
        POS_BTN_END = DLGPOS_BTN_SUB_MISSION_DOWN + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
    };

public:
    uiBattleZoneReadyMissionConfig(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneReadyMissionConfig();

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

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    Clear();
    void                    SelectMainMission();
    void                    SelectSubMission();
    void                    DoSubMissionUP();
    void                    DoSubMissionDOWN();
    void                    SetCfg_Field(WORD fieldId);
    void                    SetCfg_FieldByMapCode(MAPCODE mapCode);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

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
    CCtrlButtonCheckWZ*     m_pChkControls[POS_CHK_SIZE];
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    bool                    m_bInitControls;

    DWORD			        m_dwTextColorMainMission;
    DWORD			        m_dwTextColorSubMission;
    BYTE                    m_byCurSubMission;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneReadyMissionConfig::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneReadyMissionConfig::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneReadyMissionConfig::CheckControls()
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
uiBattleZoneReadyMissionConfig::SetCfg_Field(WORD fieldId)
{
    m_byCurSubMission = (BYTE)fieldId;
}

//------------------------------------------------------------------------------
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#endif  //  __USER_INTERFACE_BATTLEZONEREADYMISSIONCONFIG_H__
