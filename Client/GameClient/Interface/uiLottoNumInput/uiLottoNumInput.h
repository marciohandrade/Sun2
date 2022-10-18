#pragma once
#include "uibase.h"


class uiLottoNumInput : public uiBase
{
	enum eLotto_POS
	{
		LOTTO_BTN_INPUT=0,	//�Է��ϱ�(SEND)
		LOTTO_BTN_AUTO,	 	//�ڵ�����
		LOTTO_BTN_EXIT,	 	//�ݱ��ư

		LOTTO_BTN_UP1,	 	//1��°	UP ��ư
		LOTTO_BTN_UP2,	 	//2��°	UP ��ư
		LOTTO_BTN_UP3,	 	//3��°	UP ��ư
		LOTTO_BTN_UP4,	 	//4��°	UP ��ư

		LOTTO_BTN_DOWN1,	//1��°	DOWN ��ư
		LOTTO_BTN_DOWN2,	//2��°	DOWN ��ư
		LOTTO_BTN_DOWN3,	//3��°	DOWN ��ư
		LOTTO_BTN_DOWN4,	//4��°	DOWN ��ư

		LOTTO_TEXT1,	 	//1��°	����	
		LOTTO_TEXT2,	 	//2��°	����
		LOTTO_TEXT3,	 	//3��°	����
		LOTTO_TEXT4,	 	//4��°	����

		LOTTO_MAX,		
	};

	enum
	{
		LOTTO_BTN_MAX  = LOTTO_BTN_DOWN4 - LOTTO_BTN_INPUT + 1,
		LOTTO_TEXT_MAX = LOTTO_TEXT4 - LOTTO_TEXT1 + 1,
	};


public:

	uiLottoNumInput(InterfaceManager *pUIMan);
	virtual ~uiLottoNumInput(void);

	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();
	virtual void		OnUpdateSolarDialog();

	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );
	void				SEND_LOTTO_ENTRY_SYN(BOOL bAuto);
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    void SendLottoEntrySyn();
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET	
private:
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );
	void				InitControls();

private:
	void				initlottonumber();
	void				refreshLottoNumber();
    bool CheckInputNumber(BOOL bAuto);
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    void FindAndUseLotto(BOOL auto_select);
    BOOL auto_select_;
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
	int					m_arLottoNum[LOTTO_TEXT_MAX];
	static WzID			m_wzId[LOTTO_MAX];
	CCtrlButtonWZ*		m_pLottoBtn[LOTTO_BTN_MAX];
	CCtrlStaticWZ*		m_pTxtCtrl[LOTTO_TEXT_MAX];

};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiLottoNumInput::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < LOTTO_MAX ; ++i )
	{
		if( m_wzId[i] == wzId ) return i;
	}
	return LOTTO_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ *uiLottoNumInput::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < LOTTO_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
