#ifndef     __USER_INTERFACE_LOGINCHANNELLIST_H__
#define     __USER_INTERFACE_LOGINCHANNELLIST_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginChannelList

    login channel list user intreface

    (C) 2005 ckbang
*/
#include "uiLoginChannelList_def.h"
#include "PacketStruct_CL.h"
#include "uibase.h"

class uiLoginMan;
class uiLoginChannelList :
    public uiBase
{

#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
    enum eDIALOG_POS
    {
        DLGPOS_BTN_CHANNEL_1=0,
        DLGPOS_BTN_CHANNEL_2,
        DLGPOS_BTN_CHANNEL_3,
        DLGPOS_BTN_CHANNEL_4,
        DLGPOS_BTN_CHANNEL_5,
        DLGPOS_BTN_CHANNEL_6,
        DLGPOS_BTN_CHANNEL_7,

        DLGPOS_TXT_CHANNEL_1,
        DLGPOS_TXT_CHANNEL_2,
        DLGPOS_TXT_CHANNEL_3,
        DLGPOS_TXT_CHANNEL_4,
        DLGPOS_TXT_CHANNEL_5,
        DLGPOS_TXT_CHANNEL_6,
        DLGPOS_TXT_CHANNEL_7,

		DLGPOS_TXT_STATE_1,
		DLGPOS_TXT_STATE_2,
		DLGPOS_TXT_STATE_3,
		DLGPOS_TXT_STATE_4,
		DLGPOS_TXT_STATE_5,
		DLGPOS_TXT_STATE_6,
		DLGPOS_TXT_STATE_7,

		DIALOG_MAX
	};
#else
    enum eDIALOG_POS
    {
        DLGPOS_BTN_CHANNEL_1=0,
        DLGPOS_BTN_CHANNEL_2,
        DLGPOS_BTN_CHANNEL_3,
        DLGPOS_BTN_CHANNEL_4,
        DLGPOS_BTN_CHANNEL_5,
        DLGPOS_BTN_CHANNEL_6,
        DLGPOS_BTN_CHANNEL_7,

        DLGPOS_TXT_CHANNEL_1,
        DLGPOS_TXT_CHANNEL_2,
        DLGPOS_TXT_CHANNEL_3,
        DLGPOS_TXT_CHANNEL_4,
        DLGPOS_TXT_CHANNEL_5,
        DLGPOS_TXT_CHANNEL_6,
        DLGPOS_TXT_CHANNEL_7,

        DLGPOS_IMG_CH1_00,
        DLGPOS_IMG_CH1_01,
        DLGPOS_IMG_CH1_02,
        DLGPOS_IMG_CH1_03,
        DLGPOS_IMG_CH1_04,
        DLGPOS_IMG_CH1_05,
        DLGPOS_IMG_CH1_06,
        DLGPOS_IMG_CH1_07,
        DLGPOS_IMG_CH1_08,

        DLGPOS_IMG_CH2_00,
        DLGPOS_IMG_CH2_01,
        DLGPOS_IMG_CH2_02,
        DLGPOS_IMG_CH2_03,
        DLGPOS_IMG_CH2_04,
        DLGPOS_IMG_CH2_05,
        DLGPOS_IMG_CH2_06,
        DLGPOS_IMG_CH2_07,
        DLGPOS_IMG_CH2_08,

        DLGPOS_IMG_CH3_00,
        DLGPOS_IMG_CH3_01,
        DLGPOS_IMG_CH3_02,
        DLGPOS_IMG_CH3_03,
        DLGPOS_IMG_CH3_04,
        DLGPOS_IMG_CH3_05,
        DLGPOS_IMG_CH3_06,
        DLGPOS_IMG_CH3_07,
        DLGPOS_IMG_CH3_08,

        DLGPOS_IMG_CH4_00,
        DLGPOS_IMG_CH4_01,
        DLGPOS_IMG_CH4_02,
        DLGPOS_IMG_CH4_03,
        DLGPOS_IMG_CH4_04,
        DLGPOS_IMG_CH4_05,
        DLGPOS_IMG_CH4_06,
        DLGPOS_IMG_CH4_07,
        DLGPOS_IMG_CH4_08,

        DLGPOS_IMG_CH5_00,
        DLGPOS_IMG_CH5_01,
        DLGPOS_IMG_CH5_02,
        DLGPOS_IMG_CH5_03,
        DLGPOS_IMG_CH5_04,
        DLGPOS_IMG_CH5_05,
        DLGPOS_IMG_CH5_06,
        DLGPOS_IMG_CH5_07,
        DLGPOS_IMG_CH5_08,

        DLGPOS_IMG_CH6_00,
        DLGPOS_IMG_CH6_01,
        DLGPOS_IMG_CH6_02,
        DLGPOS_IMG_CH6_03,
        DLGPOS_IMG_CH6_04,
        DLGPOS_IMG_CH6_05,
        DLGPOS_IMG_CH6_06,
        DLGPOS_IMG_CH6_07,
        DLGPOS_IMG_CH6_08,

        DLGPOS_IMG_CH7_00,
        DLGPOS_IMG_CH7_01,
        DLGPOS_IMG_CH7_02,
        DLGPOS_IMG_CH7_03,
        DLGPOS_IMG_CH7_04,
        DLGPOS_IMG_CH7_05,
        DLGPOS_IMG_CH7_06,
        DLGPOS_IMG_CH7_07,
        DLGPOS_IMG_CH7_08,

        DIALOG_MAX,
    };
#endif

    enum
    {
        POS_BTN_BEGIN = DLGPOS_BTN_CHANNEL_1,
        POS_BTN_END = DLGPOS_BTN_CHANNEL_7 + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,

        POS_TXT_BEGIN = DLGPOS_TXT_CHANNEL_1,
        POS_TXT_END = DLGPOS_TXT_CHANNEL_7 + 1,
        POS_TXT_SIZE = POS_TXT_END - POS_TXT_BEGIN,

#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
		POS_TXT_STATE_BEGIN	= DLGPOS_TXT_STATE_1,
		POS_TXT_STATE_END	= DLGPOS_TXT_STATE_7 + 1,
		POS_TXT_STATE_SIZE	= POS_TXT_STATE_END - POS_TXT_STATE_BEGIN,
#endif
    };

public:
    uiLoginChannelList(InterfaceManager *pUIMan);
    virtual ~uiLoginChannelList();

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
    uiLoginMan*             GetManager();
    
    //--------------------------------------------------------------------------
    // functions
    //
public:
    INT                    TestSelectedAuthServerGroup();

protected:

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
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	void					SelectChannel(POSTYPE pos, int svIdx);
private:
	void					DrawGauge( int count );
	

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    int                     m_addCounts;
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
	CCtrlStaticWZ*			m_pTxtState[POS_TXT_STATE_SIZE];
#endif

    std::vector<int>        m_vChannelIdx;
    bool                    m_bInitControls;

};

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginChannelList::CheckControls()
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
POSTYPE
uiLoginChannelList::getCtrlIDToPos( WzID wzId )
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
uiLoginChannelList::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGINCHANNELLIST_H__
