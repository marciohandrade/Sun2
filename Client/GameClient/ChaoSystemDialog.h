#pragma once
#include "solardialog.h"

enum ChaoSystemDialog_Pos
{
	CHAOSYSTEM_TIME,
	CHAOSYSTEM_FREECHAO_PICTURE,
	CHAOSYSTEM_CHAO_PICTURE,
	CHAOSYSTEM_DIALOG_MAX,
};


class ChaoSystemDialog : public SolarDialog
{
public:
	ChaoSystemDialog(void);
	~ChaoSystemDialog(void);

	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID    		Process(DWORD dwTick);
	virtual VOID			OnRenderSolarDialog();


	void					SetTime(DWORD dwTime) {m_dwTime = dwTime; m_iTickSum=0;}
	DWORD                   GetTime(){ return m_dwTime;}

    DWORD                   GetDialogKey() { return m_DialogKey; }
    void                    SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	void					RenderSystemTime();
	void					UpdateEachCaseTime();
	void					UpdatePictureGaugeBar();

	void					UpdateToolTip();

private:
	DWORD					m_DialogKey;
	static WzID				m_wzId[CHAOSYSTEM_DIALOG_MAX];
	CCtrlStaticWZ*			m_pStatic;
	CCtrlPictureWZ*			m_pChaoPicture;
	CCtrlPictureWZ*			m_pFreeChaoPicture;


	DWORD					m_dwTime;
	float					m_fPictureHeight;
	float   				m_fPictureWidth;
	float					m_fGaugeWidth;
	RECT 					m_rtPicture;
	RECT 					m_rtSizePicture;
	BOOL					m_bMouseOver;
	int                     m_iTickSum;
};

