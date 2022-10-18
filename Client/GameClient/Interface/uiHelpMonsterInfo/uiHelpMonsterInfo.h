#pragma once

//------------------------------------------------------------------------------
//
//    �۾���: gs
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
		ST_INIT_NONE,		 //�ʱⰪ
		ST_TARGET_NONE,		 //Ÿ���þ���
		ST_NOT_MONSTER,		 //���;ƴ�
		ST_NOT_CHANGED,		 //���¹ٲ�������
		ST_TARGET_CHANGE,	 //���¹ٲ�����
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
	void					ActionSkill();										//���ͽ�ų���
	void					ActionMove();										//�����̵�
	void					SetMonsterInfo();									//������������Ʈ�� ����
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
