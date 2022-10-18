#pragma once
#include "uibase.h"
#include "uibattlescoreman\uibattlescoreman.h"

class uiBattleScoreMan;

class uiBattleScoreSingle : public uiBase
{	
	enum eBATTLE_SCORESINGLE_POS
	{
		BTSCORE_BTN_EXIT=0,
		BTSCORE_LIST,
		BTSCORE_TXT_TITLE,
		BTSCORE_TXT_DISCRIPT,
		BTSCORE_BTN_ID,
		BTSCORE_BTN_CLASS,
		BTSCORE_BTN_RANK,
		BTSCORE_BTN_LV,
		BTSCORE_BTN_MAXDAMAGE,
		BTSCORE_BTN_GIVEDAMAGE,
		BTSCORE_BTN_GETDAMAGE,
		BTSCORE_BTN_KILL,
		BTSCORE_BTN_HEAL,
		BTSCORE_BTN_DIE,
		
		BTSCORE_TXT_EXP,
		BTSCORE_STXT_EXP,
		BTSCORE_TXT_HAIM,		
		BTSCORE_STXT_HAIM,
		
		BTSCORE_PIC_EXP,
		BTSCORE_PIC_HAIM,
		BTSCORE_MAX, 
	};

	enum
	{
		eTXT_EXP,
		eSTXT_EXP,
		eTXT_HAIM,
		eSTXT_HAIM,
		eMAX_TXT,
	};

	enum
	{
		ePIC_EXP,
		ePIC_HAIM,
		eMAX_PIC,
	};
public:
	
	uiBattleScoreSingle(InterfaceManager *pUIMan);
	virtual ~uiBattleScoreSingle(void);
	
	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();
	void				InitInfo();
	void				RefreshInfo(EXPTYPE add_experience, MONEY add_money);

	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );

	uiBattleScoreMan*	GetManager();
	void				_RefreshListCtrlData(nArray<BATTLESCORE_DATA> &ArrayType);
    virtual void		OnRenderSolarDialog();

private:
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );
	void				InitControls();
	void				_clearSelection();

	void				ShowInfo(BOOL eStat);

	void				_SetListCtrlData(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline);

private:
	CCtrlListWZ*		m_pListCtrls;
	CCtrlButtonWZ*		m_pListTitleBtn[uiBattleScoreMan::BTSCROE_LIST_MAX];
	CCtrlPictureWZ*		m_pPicCtrls[eMAX_PIC];
	CCtrlStaticWZ*		m_pTxtCtrls[eMAX_TXT];

	static WzID			m_wzId[BTSCORE_MAX];
	bool				m_bShow;
	BOOL				m_bInitDlg;							//dlg는 최초 OnShowWindow(0)한번호출된다

	uiBattleScoreMan*	m_pBattleScoreMan;

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

	EXPTYPE				m_i64Exp;
	MONEY				m_i64Money;
	bool				m_bIsInited;
};
//------------------------------------------------------------------------------
/**
*/

inline
POSTYPE
uiBattleScoreSingle::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < BTSCORE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return BTSCORE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiBattleScoreSingle::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < BTSCORE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
