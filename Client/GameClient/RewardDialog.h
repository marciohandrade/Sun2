#pragma once

#include "solardialog.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

typedef struct _MISSION_REWARD_ITEM_PARAM
{
	DWORD dwCode;
	DWORD dwCount;
	int   iSet;
	int   iEnchant;
	int   iRank;

}  MISSION_REWARD_ITEM_PARAM;


typedef struct _MISSION_REWARD_ITEM
{
	MISSION_REWARD_ITEM_PARAM param;

	
	BOOL  bSelected;
	BOOL  bRandom;

	
	IconImage *pImage;
} MISSION_REWARD_ITEM;

enum eRewardViewType
{
	REWARD_VIEW_TYPE_NORMAL = 0,
	REWARD_VIEW_TYPE_AC_PREVIEW,	// 미리 보기
	REWARD_VIEW_TYPE_AC_REWARD,		// 실질적으로 보상 받기
};

	#define MAX_REWARD_ITEM       5

class RewardDialog : public SolarDialog
{
public:
	enum eREWARDDIALOG_POS
	{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        REWARDDIALOG_ITEM1_CHECK,
        REWARDDIALOG_ITEM2_CHECK,
        REWARDDIALOG_ITEM3_CHECK,
        REWARDDIALOG_ITEM4_CHECK,
        REWARDDIALOG_ITEM5_CHECK,
        REWARDDIALOG_OK_BUTTON,
        REWARDDIALOG_MONEY_TEXT,
        REWARDDIALOG_MONEY_VALUE_TEXT,
        REWARDDIALOG_EXP_STRING_TEXT,
        REWARDDIALOG_EXP_VALUE_TEXT,
        REWARDDIALOG_TITLE_TEXT,
        REWARDDIALOG_TITLE_VALUE_TEXT,
        REWARDDIALOG_TITLE_PICTURE,
        REWARDDIALOG_MISSION_TEXT,
        REWARDDIALOG_DIALOG_TITLE_TEXT,

        Dialog_Max
#else
		REWARDDIALOG_CLOSE_BUTTON = 0,
		REWARDDIALOG_ITEM1_CHECK,
		REWARDDIALOG_ITEM2_CHECK,
		REWARDDIALOG_ITEM3_CHECK,
		REWARDDIALOG_ITEM4_CHECK,
		REWARDDIALOG_ITEM5_CHECK,
		REWARDDIALOG_REWARD_TEXT,
		REWARDDIALOG_OK_BUTTON,
		REWARDDIALOG_CANCEL_BUTTON,
		REWARDDIALOG_MONEY_VALUE_TEXT,
		REWARDDIALOG_EXP_STRING_TEXT,
		REWARDDIALOG_EXP_VALUE_TEXT,
		REWARDDIALOG_MONEY_ICON_BUTTON,
		REWARDDIALOG_MISSION_TEXT,
		REWARDDIALOG_DIALOG_TITLE_TEXT,

		Dialog_Max
#endif //_NA_003027_20111013_HONOR_SYSTEM
	};


public:
	RewardDialog(void);
	virtual ~RewardDialog(void);

	VOID			Init( CDrawBase * pDrawBase );

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	void    SetRewardMoney(LONGLONG money);
	void    SetRewardExp(EXPTYPE exp);
	void    InitializeRewardItem();

	void	UpdateRewardInfos(DWORD dwIndex, int nArea = 0, int nACHIVEMENT = 0);

    void    SetACReward(DWORD dwRewardIndex, int nArea = 0, int nACHIVEMENT = 0);	
	
	void    SetMissionReward(DWORD dwRewardIndex);
	
	void    SetRewardItem(int index,MISSION_REWARD_ITEM_PARAM param);
	void    SetRandomRewardItem(int index);
	void    DeleteItem(int index);
	void    EnableSendPacket();

	int     GetSelectedItemCount();
	void    SelectItem(int index);
	void    DeselectItem(int index);
	void    OnClickItem(int index);
	void    OnClickOK();
	void    OnClickCancel();
	

	void    OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();

protected:
	void	UpdateTitle();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void    SetTitleReward(WORD reward_title_code);
#endif //_NA_003027_20111013_HONOR_SYSTEM

	void	RewardItemActiveForClass();

public:
	void	SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }
	DWORD	GetDialogKey() {return m_DialogKey; }

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    DWORD GetRewardIndex() { return m_dwRewardIndex; }
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

protected:

	DWORD			m_DialogKey;

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

    LONGLONG        m_RewardMoney;
	EXPTYPE         m_RewardExp;
	BYTE            m_bRewardType;
	DWORD           m_CharType;
	DWORD           m_dwRewardIndex;

	BOOL            m_bSendPacket;

	BOOL            m_bCanSelectItem;
	int             m_iMaxSelectItem;

	MISSION_REWARD_ITEM       m_RewardItem[MAX_REWARD_ITEM];

	FTextParser				m_FTextParser;
	FTextSentence			m_FTextSentence;
	FTextRender				m_FTextRender;

	eRewardViewType			m_eRewardViewType;

private:
	virtual VOID	OnShowWindow( BOOL val );

private:		
	static WzID	    m_wzId[Dialog_Max];

};
