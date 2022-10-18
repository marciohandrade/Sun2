#ifndef _PcbangPlayTimeDialog_h_
#define _PcbangPlayTimeDialog_h_

#include "solardialog.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

enum PcbangPlayTime_Pos
{	
	PCBANGPLAYTIME_PCITURE ,
	PCBANGPLAYTIME_CAPTION ,
	PCBANGPLAYTIME_MAX,
};

class PcbangPlayTimeDialog : public SolarDialog
{

public:
	PcbangPlayTimeDialog(void);
	~PcbangPlayTimeDialog(void);

	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID    		Process(DWORD dwTick);
	virtual VOID			OnShowWindow(BOOL val);

public:

	void	SetTime(int iTime);
	void	UpdateGauge();

protected:
	UI_CONTROL_STATIC_PROPERTY(PcbangPlayPercentage , StrToWzID("T001") );
	UI_CONTROL_STATIC_PROPERTY(PcbangPlayTime , StrToWzID("T002") );
	UI_CONTROL_STATIC_PROPERTY(PcbangTextTop , StrToWzID("T003") );
	CControlWZ *			getControl( POSTYPE AtPos );

private:
	static WzID				m_wzId[PCBANGPLAYTIME_MAX];
	float					m_fPictureWidth;
	int						m_iMaxWidth;
	int						m_iCurTime;
	int						m_iPendingTime;
	SYSTEMTIME				m_sysLocalTime;
	CCtrlPictureWZ *		m_pPicture;
	int						m_iUpdateSecond;
	CCtrlStaticWZ *			m_pCaption;
};


#endif // _PcbangPlayTimeDialog_h_
