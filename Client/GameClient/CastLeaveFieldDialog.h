#pragma once
#include "solardialog.h"

class CastLeaveFieldDialog : public SolarDialog
{
public:
	enum eCAST_LEAVE_POS
	{
		eCAST_LEAVE_PIC_GAUGE = 0, 
		eCAST_LEAVE_TXT_REMAIN_SEC,
		eCAST_LEAVE_TXT_DESC,

		Dialog_Max
	};

public:
	CastLeaveFieldDialog(void);
	virtual ~CastLeaveFieldDialog(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnUpdateSolarDialog();

	void			UpdatePosition();

	VOID			SetGage( float fHPPercent );
	void			SetDelayTimer( DWORD dwTimetick );

	void			SetDoType( eDoType eType )	{ m_eDoType = eType; }
    bool            CheckToLockBydoType();
    const eDoType   GetDoType(){return m_eDoType;}
    void			CancleCast();
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	virtual VOID	OnShowWindow( BOOL val );

	VOID			updateCheckExpireCast();
	bool IsCancleDealyCast();	
 
private:
	static WzID	    m_wzId[Dialog_Max];
	
	TCHAR			m_szResultInfo[INTERFACE_STRING_LENGTH];
    float			m_fGaugeSize;
	CCtrlStaticWZ * m_pRemainTextControl;
	CCtrlStaticWZ * m_pDescText;
	util::Timer	*	m_pRemainTimer;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	util::Timer	*	m_pServerSelectTimer;
#endif

	eDoType			m_eDoType;
};

void StartLeaveFieldCastTimer( eDoType eType, DWORD dwTime );
