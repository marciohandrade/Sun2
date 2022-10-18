#pragma once
#include "uibase.h"
#include <SCItemSlotContainer.h>
class uiPetMan;
#define MAX_LOTTO_SLOT		1			//응모슬롯갯수

struct LOTTO_BASE_INFO
{
	DWORD wDrawNum;					//로또회차
	BYTE  arLuckyNum[LOTTO_NUM_PICKS];	//당첨번호
	MONEY LottoHeim;					//복권기금
	DWORD dwWinnerNum;					//1등당첨자수
	MONEY WinnerHeim;					//1등당첨금
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
		LOTTO_BTN_SLOT			=0,		//슬롯버튼(응모내역아래)
		LOTTO_BTN_EXIT,					//닫기버튼
		LOTTO_BTN_APPLY,				//응모하기

		LOTTO_TXT_INFO,					//하임로또설명	  m_pTxtCtrl[0]
		LOTTO_TXT_DRAWNUM,				//회당첨번호	  m_pTxtCtrl[1]
		LOTTO_TXT_WINNUM,				//당첨번호		  m_pTxtCtrl[2]
		LOTTO_TXT_HEIM_TURN,			//몇회복권기금	  m_pTxtCtrl[3]
		LOTTO_TXT_WINNER_TURN,			//몇회당첨자수	  m_pTxtCtrl[4]
		LOTTO_TXT_WINHEIM_TURN,			//몇회당1등당첨금 m_pTxtCtrl[5]
		LOTTO_TXT_HEIM,					//복권기금액	  m_pTxtCtrl[6]
		LOTTO_TXT_WINNER,				//당첨자수		  m_pTxtCtrl[7]
		LOTTO_TXT_WINHEIM,				//1등당첨금		  m_pTxtCtrl[8]
	
		LOTTO_LIST_TEXT,				//리스트내용
				
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        LOTTO_SCROLL,				//스크롤바
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
		LOTTO_MAX

	};

	enum
	{
		LOTTO_BTN_MAX  = LOTTO_BTN_APPLY - LOTTO_BTN_SLOT + 1,
		LOTTO_TEXT_MAX = LOTTO_TXT_WINHEIM - LOTTO_TXT_INFO + 1,
	};
	
	//텍스트 컨트롤러 인덱스
	enum
	{
		IDX_LOTTO_INFO=0,				//하임로또설명	  m_pTxtCtrl[0]
		IDX_LOTTO_DRAWNUM,				//회당첨번호	  m_pTxtCtrl[1]
		IDX_LOTTO_WINNUM,				//당첨번호		  m_pTxtCtrl[2]
		IDX_LOTTO_HEIM_TURN,			//몇회복권기금	  m_pTxtCtrl[3]
		IDX_LOTTO_WINNER_TURN,			//몇회당첨자수	  m_pTxtCtrl[4]
		IDX_LOTTO_WINHEIM_TURN,			//몇회당1등당첨금 m_pTxtCtrl[5]
		IDX_LOTTO_HEIM,					//복권기금액	  m_pTxtCtrl[6]
		IDX_LOTTO_WINNER,				//당첨자수		  m_pTxtCtrl[7]
		IDX_LOTTO_WINHEIM,				//1등당첨금		  m_pTxtCtrl[8]
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
	
	//로또응모상태불가(버튼및메시지달리찍음)
	void				SetEnableWindow(bool bFlag);

	//로또응모정보 
	void				SetApplyInfo();
	void				InsertLastApply(WORD wDrawNum,BYTE *arApplyNum);	
	void				InsertApplyInfo(LOTTO_APPLY_DATA& Data,BOOL bTail=FALSE);

	// 로또기본정보 
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
	
	CCtrlListWZ*		m_pList;			//응모내역 리스트	
	CCtrlVScrollWZ*     m_pScroll;			//응모내용 스크롤

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
