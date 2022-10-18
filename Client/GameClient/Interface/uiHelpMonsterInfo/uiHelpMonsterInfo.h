#pragma once

//------------------------------------------------------------------------------
//
//    작업자: gs
//    uiHelpMonsterInfo
//------------------------------------------------------------------------------

#include "uibase.h"
#include "FTextRender.h"



class uiHelpMan;
class uiHelpMonsterInfo: public uiBase 
{

	enum eDIALOG_POS
	{
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_OK,

		DLGPOS_TXT_TITLE,
		
		DLGPOS_LST_DESC,

		DLGPOS_SCR_VERTICAL,

		DIALOG_MAX,
	};

	enum
	{
		POS_BTN_SIZE = DLGPOS_BTN_OK - DLGPOS_BTN_EXIT + 1,
	};

	enum eINFO_STATE
	{
		ST_INIT_NONE,		 //초기값
		ST_TARGET_NONE,		 //타켓팅없음
		ST_NOT_MONSTER,		 //몬스터아님
		ST_NOT_CHANGED,		 //상태바뀌지않음
		ST_TARGET_CHANGE,	 //상태바뀌어야함
    };

public:
	uiHelpMonsterInfo(InterfaceManager *pUIMan);
	virtual ~uiHelpMonsterInfo(void);

public:
	virtual void            Init(CDrawBase* pDrawBase);

protected:
	virtual void            OnUpdateSolarDialog();
	
protected:
	void					InsertSkillList(WORD wSkillCode,BOOL bSpecial);
	void					SetMoveCaption(BOOL bMove);
	void					ActionSkill();										//몬스터스킬사용
	void					ActionMove();										//몬스터이동
	void					SetMonsterInfo();									//몬스터정보리스트에 셋팅
	void                    InitControls();
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[DIALOG_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	CCtrlStaticWZ*          m_pTitleControl;
	CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
	CCtrlVScrollWZ*         m_pScrollControl;
	CCtrlListWZ*			m_pListInfo;
	
	std::vector<WORD>		m_vSkillCode;
	DWORD					m_dwOldSelectId;
	int						m_iCurScrollPos;
	eINFO_STATE				m_eState;
	BOOL					m_bTargeting;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiHelpMonsterInfo::getCtrlIDToPos( WzID wzId )
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
uiHelpMonsterInfo::getControl( POSTYPE AtPos )
{
	assert( AtPos < DIALOG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
