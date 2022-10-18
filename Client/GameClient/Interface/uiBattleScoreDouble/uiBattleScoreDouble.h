#pragma once
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
#include "uibase.h"
#include "uibattlescoreman\uibattlescoreman.h"

class uiBattleScoreMan;

class uiBattleScoreDouble : public uiBase
{	
	enum eBATTLE_SCORESINGLE_POS
	{
		BTSCORE_BTN_EXIT=0,
		BTSCORE_BTN_ID01,
		BTSCORE_BTN_CLASS01,
		BTSCORE_BTN_RANK01,
		BTSCORE_BTN_LV01,
		BTSCORE_BTN_MAXDAMAGE01,
		BTSCORE_BTN_GIVEDAMAGE01,
		BTSCORE_BTN_GETDAMAGE01,
		BTSCORE_BTN_KILL01,
		BTSCORE_BTN_HEAL01,
		BTSCORE_BTN_DIE01,
		BTSCORE_TXT_WIN01,
		
		BTSCORE_BTN_DIE02,
		BTSCORE_BTN_HEAL02,
		BTSCORE_BTN_KILL02,
		BTSCORE_BTN_GETDAMAGE02,
		BTSCORE_BTN_GIVEDAMAGE02,
		BTSCORE_BTN_MAXDAMAGE02,
		BTSCORE_BTN_LV02,
		BTSCORE_BTN_RANK02,
		BTSCORE_BTN_CLASS02,
		BTSCORE_BTN_ID02,
		BTSCORE_TXT_WIN02,

		BTSCORE_PIC_01,
		BTSCORE_PIC_02,
		BTSCORE_WINTXT_01,
		BTSCORE_WINTXT_02,

		BTSCORE_LIST01,
		BTSCORE_LIST02,

		BTSCORE_MAX, 
	};


	enum BTSCORE_LSTS
	{
		BTSCORE_LST_LIST01,
		BTSCORE_LST_LIST02,
		BTSCORE_LST_MAX,
	};

	enum
	{
		eWIN01,
		eWIN02,
		eMAX_WINCON,
	};

public:
	
	uiBattleScoreDouble(InterfaceManager *pUIMan);
	virtual ~uiBattleScoreDouble(void);
	
	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();
	
	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );

	uiBattleScoreMan*	GetManager();
	void				_RefreshListCtrlData(nArray<BATTLESCORE_DATA> &ArrayType, E_TEAM eTeamNum);

	void				InitInfo();
	void				RefreshInfo();
    virtual void		OnRenderSolarDialog();

private:
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );
	void				InitControls();
	void				_clearSelection();
	void				_SetListCtrlData(BTSCORE_LSTS eListNum, int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline);
	void				ShowInfo(BOOL eStat);

private:
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

	CCtrlListWZ*		m_pListCtrls[BTSCORE_LST_MAX];
	CCtrlButtonWZ*		m_pListTitleBtn01[uiBattleScoreMan::BTSCROE_LIST_MAX];
	CCtrlButtonWZ*		m_pListTitleBtn02[uiBattleScoreMan::BTSCROE_LIST_MAX];
	CCtrlPictureWZ*		m_pPicCtrls[eMAX_WINCON];
	CCtrlStaticWZ*		m_pTxtCtrls[eMAX_WINCON];

	static WzID			m_wzId[BTSCORE_MAX];
	bool				m_bShow;
	BOOL				m_bInitDlg;							//dlg는 최초 OnShowWindow(0)한번호출된다 

	uiBattleScoreMan*	m_pBattleScoreMan;
	bool				m_bIsInited;
};
//------------------------------------------------------------------------------
/**
*/

inline
POSTYPE
uiBattleScoreDouble::getCtrlIDToPos( WzID wzId )
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
uiBattleScoreDouble::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < BTSCORE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
