#ifndef     __USER_INTERFACE_BATTLEZONECREATE_H__
#define     __USER_INTERFACE_BATTLEZONECREATE_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneCreate

    battle zone create user interface

    (C) 2005 ckbang
*/
#include "uiBattleZoneCreate_def.h"
#include "BattleRoomEntry.h"
#include "uibase.h"

class uiBattleZoneMan;
class uiBattleZoneCreate :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT=0,
        DLGPOS_BTN_CREATE,
        DLGPOS_BTN_CANCEL,
        DLGPOS_BTN_PEOPLE_LIMIT,
        DLGPOS_BTN_ROOM_TYPE,

        DLGPOS_CHKBTN_CLASS_B,  // 버서커
        DLGPOS_CHKBTN_CLASS_D,  // 드래곤 나이트
        DLGPOS_CHKBTN_CLASS_V,  // 발키리
        DLGPOS_CHKBTN_CLASS_E,  // 엘리멘탈 리스트
		DLGPOS_CHKBTN_CLASS_S,  //쉐도우
        DLGPOS_CHKBTN_MAP_FOREST,			// 야수의 숲
        DLGPOS_CHKBTN_MAP_UNDERWAY,			// 지하 수로
        DLGPOS_CHKBTN_MAP_FROZENPALACE,		// 설원
        DLGPOS_CHKBTN_MAP_VALLYOFDRAGON,	// 화산
		DLGPOS_CHKBTN_MAP_PUBLIC_CEMETERY,	// 공동묘지
		DLGPOS_CHKBTN_MAP_HELLON_TOWER,		// 헬론의 성
		DLGPOS_CHKBTN_MAP_TUTORIAL,			// 튜토리얼 미션
        DLGPOS_TXT_PEOPLE_LIMIT,
        DLGPOS_TXT_ROOM_TYPE,
        DLGPOS_TXT_ROOM_TYPE_DESC,
        DLGPOS_TXT_MIN_LIMIT_LEVEL,
        DLGPOS_TXT_MAX_LIMIT_LEVEL,

        DLGPOS_EDT_MIN_LEVEL,
        DLGPOS_EDT_MAX_LEVEL,
        DLGPOS_EDT_TITLE,
        DLGPOS_EDT_PASSWORD,

        DLGPOS_LST_ROOM_TYPE,
        DLGPOS_LST_PEOPLE_LIMIT,

		DLGPOS_TI_SUPER_ROOM_NAME,
		DLGPOS_BTN_SUPER_ROOM_SELECT,
		DLGPOS_LIST_SUPER_ROOM,
		DLGPOS_CHKBTN_MAPALL,

		DLGPOS_CHKBTN_MAP_0,
		DLGPOS_CHKBTN_MAP_1,
		DLGPOS_CHKBTN_MAP_2,
		DLGPOS_CHKBTN_MAP_3,
		DLGPOS_CHKBTN_MAP_4,
		DLGPOS_CHKBTN_MAP_5,
		DLGPOS_CHKBTN_MAP_6,
		DLGPOS_CHKBTN_MAP_7,
		DLGPOS_CHKBTN_MAP_8,
		DLGPOS_CHKBTN_MAP_9,
		DLGPOS_CHKBTN_MAP_10,
		DLGPOS_CHKBTN_MAP_11,
		DLGPOS_CHKBTN_MAP_12,
		DLGPOS_CHKBTN_MAP_13,
		DLGPOS_CHKBTN_MAP_14,
		DLGPOS_CHKBTN_MAP_15,
		DLGPOS_CHKBTN_MAP_16,
		DLGPOS_CHKBTN_MAP_17,
		DLGPOS_CHKBTN_MAP_18,
		DLGPOS_CHKBTN_MAP_19,

		DLGPOS_CHKBTN_MAP_20,
		DLGPOS_CHKBTN_MAP_21,
		DLGPOS_CHKBTN_MAP_22,
		DLGPOS_CHKBTN_MAP_23,
		DLGPOS_CHKBTN_MAP_24,
		DLGPOS_CHKBTN_MAP_25,

		DLGPOS_CHKBTN_MAP_26,

		DLGPOS_CHKBTN_MAP_27,
		DLGPOS_CHKBTN_MAP_28,
		DLGPOS_CHKBTN_MAP_29,
		DLGPOS_CHKBTN_MAP_30,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_BEGIN       =   DLGPOS_BTN_EXIT,
        POS_BTN_END         =   DLGPOS_BTN_ROOM_TYPE + 1,
        NUM_POS_BTN         =   POS_BTN_END - POS_BTN_BEGIN,

        POS_CHKBTN_CLASS_BEGIN    =   DLGPOS_CHKBTN_CLASS_B,
		POS_CHKBTN_CLASS_END      =   DLGPOS_CHKBTN_CLASS_S + 1,
        NUM_POS_CLASS_CHKBTN      =   POS_CHKBTN_CLASS_END - POS_CHKBTN_CLASS_BEGIN,

        POS_CHKBTN_MAP_BEGIN    =   DLGPOS_CHKBTN_MAP_FOREST,
		POS_CHKBTN_MAP_END		=	DLGPOS_CHKBTN_MAP_TUTORIAL + 1,
        NUM_POS_MAP_CHKBTN      =   POS_CHKBTN_MAP_END - POS_CHKBTN_MAP_BEGIN,

		POS_CHKBTN_MAP_BEGIN_NEW = DLGPOS_CHKBTN_MAP_0,
		POS_CHKBTN_MAP_END_NEW	 = DLGPOS_CHKBTN_MAP_25,
		NUM_POS_MAP_CHKBTN_NEW	 = POS_CHKBTN_MAP_END_NEW - POS_CHKBTN_MAP_BEGIN_NEW,

        POS_EDT_BEGIN       =   DLGPOS_EDT_MIN_LEVEL,
        POS_EDT_END         =   DLGPOS_EDT_PASSWORD + 1,
        NUM_POS_EDT         =   POS_EDT_END - POS_EDT_BEGIN,

        POS_TXT_BEGIN       =   DLGPOS_TXT_PEOPLE_LIMIT,
        POS_TXT_END         =   DLGPOS_TXT_MAX_LIMIT_LEVEL + 1,
        NUM_POS_TXT         =   POS_TXT_END - POS_TXT_BEGIN,

        POS_LST_BEGIN       =   DLGPOS_LST_ROOM_TYPE,
        POS_LST_END         =   DLGPOS_LST_PEOPLE_LIMIT + 1,
        NUM_POS_LST         =   POS_LST_END - POS_LST_BEGIN,
    };

	enum eEDT_POS
	{
	    EDT_MIN_LEVEL = 0,
        EDT_MAX_LEVEL,
        EDT_TITLE,
        EDT_PASSWORD,
	};

public:
    uiBattleZoneCreate(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneCreate();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();
    void                    Clear();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiBattleZoneMan*        GetManager();
    const uicBattleZoneCreate_Configs& GetCfgs();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    ExitZoneCreate();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    clearModeList();
    void                    clearPeopleList();

    void                    updateCheckBtns();
    void                    updateRoomType();
    void                    updatePeopleLimitType();
    void                    updatePeopleLimitTypeTemp();
    void                    updateLevelLimit();
    void                    renderRoomTypeDescription();

    bool                    checkValidMap(MAPCODE fromCode, MAPCODE toCode);
    void                    checkEditMinMaxLevel();

    bool                    setMapType(WORD wType);
    bool                    setMapTypeMission(WORD wType);
    bool                    setMapTypeHunting(WORD wType);
    bool                    setMapTypePVP(WORD wType);
    bool                    setMapTypeCHunting(WORD wType);
    void                    setMapTypeInfo(const sMAPINFO* mapInfo);
    void                    setClassType(POSTYPE pos);
	
	void					setDefaultRoomName(int iSelect = -1);

	POSTYPE					getMapAreaInfo(int nPos);
	void					clickMapArea(POSTYPE pos);

	void					refreshToolTip(POSTYPE nPos);
	void					checkMapMouseOver();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();
	void					renderTooltip();

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
    CCtrlButtonWZ*          m_pBtnControls[NUM_POS_BTN];
    CCtrlButtonCheckWZ*     m_pChkBtnClassControls[NUM_POS_CLASS_CHKBTN];
    CCtrlButtonCheckWZ*     m_pChkBtnMapControls[NUM_POS_MAP_CHKBTN];

	CCtrlButtonCheckWZ*     m_pChkBtnMapAll;

    CCtrlDxIMEEditBox*      m_pEdtControls[NUM_POS_EDT];
    CCtrlStaticWZ*          m_pTxtControls[NUM_POS_TXT];
    CCtrlListWZ*            m_pLstControls[NUM_POS_LST];

	CCtrlListWZ *			m_pListSuperMaster;
	CCtrlButtonWZ *			m_pButtonSuperMaster;
	CCtrlImageStaticWZ *	m_pTextSuperMaster;
	void					SetDefaultSuperMasterList();
	void					SetTextCurrentSuperMasterType(int curSel);

    bool                    m_bInitControls;

private:
    uicBattleZoneCreate_Configs m_cfgs;
	std::vector<util::_tstring>	m_vMapDesc;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneCreate::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneCreate::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneCreate::CheckControls()
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
const uicBattleZoneCreate_Configs&
uiBattleZoneCreate::GetCfgs()
{
    return m_cfgs;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONECREATE_H__
