#ifndef     __uiQuestReward_H__
#define     __uiQuestReward_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestReward

    interface for Item Composite

    (C) 2005 y2jinc 
*/
#include "uiBase.h"
#include "uiQuestReward_def.h"
#include "ItemComposeListParser.h"
#include "SCItemSlotContainer.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "uiQuestMan/uiQuestMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"

class RewardInfo;
class uiQuestMan;
class Quest;
class QuestInfo;

class uiQuestReward :
    public uiBase, public SCItemSlotContainer
{
    enum eDIALOG_POS
    {
		eDLGPOS_SLOT_SELECT_ITEM_1 = 0,
		eDLGPOS_SLOT_SELECT_ITEM_2,
		eDLGPOS_SLOT_SELECT_ITEM_3,
		eDLGPOS_SLOT_SELECT_ITEM_4,
		eDLGPOS_SLOT_SELECT_ITEM_5,
		eDLGPOS_SLOT_FIX_ITEM,
		eDLGPOS_VSCR_QUEST_INFO,
		eDLGPOS_BTN_EXIT,
		eDLGPOS_BTN_OK,
		eDLGPOS_BTN_CANCEL,
        eDLGPOS_BTN_REWARD_POINT,
        DIALOG_MAX,
    };

    enum
    {
		SLOT_BEGIN			= eDLGPOS_SLOT_SELECT_ITEM_1,
		SLOT_SIZE			= eDLGPOS_SLOT_FIX_ITEM-eDLGPOS_SLOT_SELECT_ITEM_1+1,

		SLOT_FIX_ITEM		= eDLGPOS_SLOT_FIX_ITEM,

		SLOT_SELECT_BEGIN	= eDLGPOS_SLOT_SELECT_ITEM_1,
		SLOT_SELECT_SIZE	= eDLGPOS_SLOT_SELECT_ITEM_5-eDLGPOS_SLOT_SELECT_ITEM_1+1,

		
    };

	struct sSelectRewardSlotIdx
	{
		POSTYPE pos;
	};

public:
    uiQuestReward(InterfaceManager *pUIMan);
    virtual ~uiQuestReward();

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
    uiQuestMan*				GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ClearInfo();

	// Item °ü·Ã
   	ItemUnitRenderer *		GetItemUnitRender() const;

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & rSlot );									// ½½·Ô¿¡ °´Ã¼ Ãß°¡
	BOOL									DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

    void                    SetCurQuestCode(QuestInfo * pQuestInfo, eQUEST_GAIN_STATE QuestState);

protected:
	void					initInfo();
	void					initBtnInfo();
	void					initQuestInfo();
	void					updateInfo();

	void					clickBtn(POSTYPE pos);
	BOOL					clickSelectItemSlot(POSTYPE pos);

private:
	void					initQuestInfoConversation();
	void					initQuestInfoReward();

	int						RewardItemActiveForClass(RewardInfo * pRewardInfo);

	//--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
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
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );


    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL                    m_bInitControls;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];

	FTextParser				m_FTextParser;
	FTextSentence			m_FTextSentence;
	FTextRender				m_FTextRender;
	
	// controls
	UI_CONTROL_STATIC_PROPERTY(Title,		StrToWzID("T001"));
	UI_CONTROL_STATIC_PROPERTY(QuestInfo,	StrToWzID("T003"));
	UI_CONTROL_BUTTON_PROPERTY(RewardHeim,	StrToWzID("T101"));
	UI_CONTROL_BUTTON_PROPERTY(RewardOK,	StrToWzID("B001"));
	UI_CONTROL_BUTTON_PROPERTY(RewardCancel,StrToWzID("B002"));
	UI_CONTROL_VSCROLL_PROPERTY(QuestInfoVscr,StrToWzID("V001"));
	UI_CONTROL_BUTTON_PROPERTY(RewardExp, StrToWzID("T102"));	
	UI_CONTROL_STATIC_PROPERTY(RewardChao, StrToWzID("T201"));
    CCtrlButtonWZ* reward_point_button_;
	
	sScrollInfo				m_VScrollInfo;

    QuestInfo *             m_pCurQuestInfo;
	eQUEST_GAIN_STATE		m_eQuestGainState;

	std::list<sSelectRewardSlotIdx>		
							m_SelectRewardItemList;
	BYTE					m_bSelectItem;    
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestReward::getCtrlIDToPos( WzID wzId )
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
uiQuestReward::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestReward::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __uiQuestReward_H__

