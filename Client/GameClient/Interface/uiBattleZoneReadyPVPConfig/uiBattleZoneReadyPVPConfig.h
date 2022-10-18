#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiBattleZoneReadyPVPConfig class
//------------------------------------------------------------------------------

#include "uibase.h"


class uiBattleZoneMan;

class uiBattleZoneReadyPVPConfig: public uiBase
{
    enum eDIALOG_PVP_CONFIG_POS
    {
        DIALOG_PVP_CONFIG_TXT_RULE,
		DIALOG_PVP_CONFIG_TXT_MODE,

		DIALOG_PVP_CONFIG_CHK_FIELD_1,
		DIALOG_PVP_CONFIG_CHK_FIELD_2,
		DIALOG_PVP_CONFIG_CHK_FIELD_3,	

		DIALOG_PVP_CONFIG_CHK_RULE_1,	// 10킬 
		DIALOG_PVP_CONFIG_CHK_RULE_2,	// 30킬
		DIALOG_PVP_CONFIG_CHK_RULE_3,	// 무한	

		DIALOG_PVP_CONFIG_CHK_MODE_1,	// 개인전
		DIALOG_PVP_CONFIG_CHK_MODE_2,	// 팀전

        DIALOG_PVP_CONFIG_MAX,
    };

	enum
	{
		CHK_FIELD_1 = 0,
		CHK_FIELD_2,
		CHK_FIELD_3,
	};

	enum
	{
		CHK_RULE_1 = 0,
		CHK_RULE_2,
		CHK_RULE_3,
	};

	enum
	{
		CHK_MODE_1 = 0,
		CHK_MODE_2,
	};

    enum
    {
        CHK_FIELD_SIZE = DIALOG_PVP_CONFIG_CHK_FIELD_3 - DIALOG_PVP_CONFIG_CHK_FIELD_1 + 1,
		CHK_RULE_SIZE = DIALOG_PVP_CONFIG_CHK_RULE_3 - DIALOG_PVP_CONFIG_CHK_RULE_1 + 1,
		CHK_MODE_SIZE = DIALOG_PVP_CONFIG_CHK_MODE_2 - DIALOG_PVP_CONFIG_CHK_MODE_1 + 1,
    };


public:
    uiBattleZoneReadyPVPConfig(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneReadyPVPConfig();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
	//virtual void			UpdatePosition();

	void                    Clear();
protected:
    void                    InitControls();

	
    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiBattleZoneMan*		GetManager();
	
    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					initConfigUIText();
	void					updateConfigPVPUI();

public:

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
    static WzID		        m_wzId[DIALOG_PVP_CONFIG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiBattleZoneMan*		m_pBattleZoneMan;

	CCtrlStaticWZ *			m_pCtrlStaticRule;
	CCtrlStaticWZ *			m_pCtrlStaticMode;
	CCtrlButtonCheckWZ *	m_pCtrlCheckField[CHK_FIELD_SIZE];
	CCtrlButtonCheckWZ *	m_pCtrlCheckRule[CHK_RULE_SIZE];
	CCtrlButtonCheckWZ *	m_pCtrlCheckMode[CHK_MODE_SIZE];

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneReadyPVPConfig::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_PVP_CONFIG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_PVP_CONFIG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiBattleZoneReadyPVPConfig::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_PVP_CONFIG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiBattleZoneMan*
uiBattleZoneReadyPVPConfig::GetManager()
{
    return this->m_pBattleZoneMan;
}

//------------------------------------------------------------------------------

