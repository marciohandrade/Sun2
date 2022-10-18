//=======================================================================================================================
/// RadarDialog
/**
	@author
		이도형 < elflee@webzen.co.kr >
	@since

	@remarks
- 		
*/
#ifndef __SUNCREDIT_DIALOG_H__
#define __SUNCREDIT_DIALOG_H__

#pragma once

#include "solardialog.h"
#include "bink.h"

#define MOVIE_FRAME_UPDATE_TIME 55

class CreditDialog : public uiBase
{
	enum eCREDOT_POS
	{	
		CREDIT_MAP_IMG,
		CREDIT_MAP_NAME,
		CREDIT_PLUS,
		CREDIT_EXIT,
		CREDIT_PIC_LAYER_BACKSIZE,
		CREDIT_PIC_LAYER_SIZE,
		CREDIT_PIC_LAYER_COUNT,
		CREDIT_TXT_LAYER_COUNT,
		CREDIT_MAX,
	};

public:
	CreditDialog(InterfaceManager *pUIMan);
	virtual ~CreditDialog();

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual BOOL	IsVisibleWindow();

protected:
	virtual VOID	OnShowWindow( BOOL val );
	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	virtual BOOL	InterSectRect( POINT point );
	void			InitControls( void );
	
	// 무비관련
	void			UpdateMovie();
	void			InitBink(bool bConsciousness);
	void			ReleaseBink();
	void			RenderMovie();

private:
	void			renderframe();

protected:
	static WzID		m_wzId[CREDIT_MAX];

	CCtrlPictureWZ*		m_pPicCtrlLayerSize;
	CCtrlPictureWZ*		m_pPicCtrlLayerBackSize;
	CCtrlStaticWZ*		m_pPicCtrlLayer;
	CCtrlStaticWZ*		m_pTxtCtrlLayer;

private:
	eFIS				m_iState;
	HBINK				m_hBink;
	HANDLE				m_hRenderTargetTexture; // Bink용 
};


//========================================================================================

#endif // __SUNCREDIT_DIALOG_H__

