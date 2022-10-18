#pragma once
#include "uibase.h"
#include <SCItemSlotContainer.h>
class uiPetMan;
#define MAX_LOTTO_SLOT		1			//���𽽷԰���

struct LOTTO_BASE_INFO
{
	DWORD wDrawNum;					//�ζ�ȸ��
	BYTE  arLuckyNum[LOTTO_NUM_PICKS];	//��÷��ȣ
	MONEY LottoHeim;					//���Ǳ��
	DWORD dwWinnerNum;					//1���÷�ڼ�
	MONEY WinnerHeim;					//1���÷��
};

struct LOTTO_APPLY_DATA
{
	LOTTO_APPLY_DATA( void )
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD wDrawNum;
	BYTE  byEntryCnt;
	BYTE  arNumInfo[LOTTO_MAX_TICKETS][LOTTO_NUM_PICKS];
};

class uiLottoWindow : public uiBase,public SCItemSlotContainer
{
	enum eLotto_POS
	{
		LOTTO_BTN_SLOT			=0,		//���Թ�ư(���𳻿��Ʒ�)
		LOTTO_BTN_EXIT,					//�ݱ��ư
		LOTTO_BTN_APPLY,				//�����ϱ�

		LOTTO_TXT_INFO,					//���ӷζǼ���	  m_pTxtCtrl[0]
		LOTTO_TXT_DRAWNUM,				//ȸ��÷��ȣ	  m_pTxtCtrl[1]
		LOTTO_TXT_WINNUM,				//��÷��ȣ		  m_pTxtCtrl[2]
		LOTTO_TXT_HEIM_TURN,			//��ȸ���Ǳ��	  m_pTxtCtrl[3]
		LOTTO_TXT_WINNER_TURN,			//��ȸ��÷�ڼ�	  m_pTxtCtrl[4]
		LOTTO_TXT_WINHEIM_TURN,			//��ȸ��1���÷�� m_pTxtCtrl[5]
		LOTTO_TXT_HEIM,					//���Ǳ�ݾ�	  m_pTxtCtrl[6]
		LOTTO_TXT_WINNER,				//��÷�ڼ�		  m_pTxtCtrl[7]
		LOTTO_TXT_WINHEIM,				//1���÷��		  m_pTxtCtrl[8]
	
		LOTTO_LIST_TEXT,				//����Ʈ����
				
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        LOTTO_SCROLL,				//��ũ�ѹ�
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
		LOTTO_MAX

	};

	enum
	{
		LOTTO_BTN_MAX  = LOTTO_BTN_APPLY - LOTTO_BTN_SLOT + 1,
		LOTTO_TEXT_MAX = LOTTO_TXT_WINHEIM - LOTTO_TXT_INFO + 1,
	};
	
	//�ؽ�Ʈ ��Ʈ�ѷ� �ε���
	enum
	{
		IDX_LOTTO_INFO=0,				//���ӷζǼ���	  m_pTxtCtrl[0]
		IDX_LOTTO_DRAWNUM,				//ȸ��÷��ȣ	  m_pTxtCtrl[1]
		IDX_LOTTO_WINNUM,				//��÷��ȣ		  m_pTxtCtrl[2]
		IDX_LOTTO_HEIM_TURN,			//��ȸ���Ǳ��	  m_pTxtCtrl[3]
		IDX_LOTTO_WINNER_TURN,			//��ȸ��÷�ڼ�	  m_pTxtCtrl[4]
		IDX_LOTTO_WINHEIM_TURN,			//��ȸ��1���÷�� m_pTxtCtrl[5]
		IDX_LOTTO_HEIM,					//���Ǳ�ݾ�	  m_pTxtCtrl[6]
		IDX_LOTTO_WINNER,				//��÷�ڼ�		  m_pTxtCtrl[7]
		IDX_LOTTO_WINHEIM,				//1���÷��		  m_pTxtCtrl[8]
	};


public:

	uiLottoWindow(InterfaceManager *pUIMan);
	virtual ~uiLottoWindow(void);

	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();

	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual VOID		MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );
	void 				SetItemNum(int iNum=0);
	void 				RefreshLottoSlot();
	
	//�ζ�������ºҰ�(��ư�׸޽����޸�����)
	void				SetEnableWindow(bool bFlag);

	//�ζ��������� 
	void				SetApplyInfo();
	void				InsertLastApply(WORD wDrawNum,BYTE *arApplyNum);	
	void				InsertApplyInfo(LOTTO_APPLY_DATA& Data,BOOL bTail=FALSE);

	// �ζǱ⺻���� 
	void				SetLottoBaseInfo(LOTTO_BASE_INFO &rInfo);

private:
	void				InsertNewApply(WORD wDrawNum,BYTE *arApplyNum);
	void				ClearAllInLottoInfo();	
	void				renderToolTip();
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );

	void				LottoErrorMsg(int iStringCode);
	void				InitControls();
	void				InitUnitRenderer();
	
	virtual RC::eSLOT_INSERT_RESULT	InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );
	virtual BOOL					DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    void DownScroll(int nRange);
    void UpScroll(int nRange);
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
private:
	static WzID			m_wzId[LOTTO_MAX];

	CCtrlButtonWZ*		m_pLottoBtn[LOTTO_BTN_MAX];
	CCtrlStaticWZ*		m_pTxtCtrl[LOTTO_TEXT_MAX];
	
	CCtrlListWZ*		m_pList;			//���𳻿� ����Ʈ	
	CCtrlVScrollWZ*     m_pScroll;			//���𳻿� ��ũ��

	ItemUnitRenderer *	m_pItemUnitRenderer;
	SCItemSlot *		m_pItemToolTip;

	std::list<LOTTO_APPLY_DATA>	m_listApplyContainer;
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    BYTE offset_list_;
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiLottoWindow::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < LOTTO_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return LOTTO_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ *uiLottoWindow::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < LOTTO_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
