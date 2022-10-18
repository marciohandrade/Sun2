//=======================================================================================================================
/// RadarDialog
/**
	@author
		À¯Àç¿µ < y2jinc@webzen.co.kr >
	@since

	@remarks
- 		
*/

#ifndef __RADAR_DIALOG_H__
#define __RADAR_DIALOG_H__

#pragma once

#include "solardialog.h"
#include "RadarInfoParser.h"
#include "BaseRadarDialog.h"

class RadarDialog : public BaseRadarDialog
{
    friend class uiRadarMan;
// [2010-2-1 by i4u4me]  
public:
	enum eRADAR_POS
	{	
		RADAR_MAP_IMG,
		RADAR_MAP_NAME,
		RADAR_PLUS,
		RADAR_EXIT,
		RADAR_PIC_LAYER_BACKSIZE,
		RADAR_PIC_LAYER_SIZE,
		RADAR_PIC_LAYER_COUNT,
		RADAR_TXT_LAYER_COUNT,

#ifdef _NA_000000_20130114_RENEWER_UI
		RADAR_MAP_B001,
		RADAR_MAP_P002,
		RADAR_MAP_P003,

		RADAR_MAP_BACK,
		RADAR_MAP_BACK2,
#endif

		RADAR_MAX,
	};


public:
	RadarDialog(InterfaceManager *pUIMan);
	virtual ~RadarDialog();

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	
	virtual BOOL	IsVisibleWindow();
	virtual void	ChangeMap(TCHAR* file_name = NULL);
	virtual void	SetCurLayer( int iCurLayer = 1);		
	virtual void	SetMaxLayer( int iMaxLayer );

#ifdef _NA_000000_20130114_RENEWER_UI
	void HideAllControl(bool value);
	void DisablePosInWindow(bool value);

	bool gethidecheck() { return show_hide_check_; }
#endif

protected:
    bool            OnClickRadar();

	void			loadMapImage( WORD fieldID );

	virtual VOID	OnShowWindow( BOOL val );
	VOID			updateMap();
	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	virtual BOOL	InterSectRect( POINT point );
	BOOL            CheckClickedPosition();


	void			InitControls( void );
	void			InitLayerShowNHide( void );

	virtual DWORD	getOpenDlgSoundIndex()
	{
		return UI_SOUND_MAP_ZOOM_OUT;
	}

	virtual DWORD	getCloseDlgSoundIndex()
	{
		return UI_SOUND_MAP_ZOOM_IN;
	}

    bool OnClickPathFindMove( CControlWZ* pPictureControl );

	
protected:
	static WzID		m_wzId[RADAR_MAX];

	int				m_iCurLayer;
	float			m_fLayerSize;

	CCtrlPictureWZ*		m_pPicCtrlLayerSize;
	CCtrlPictureWZ*		m_pPicCtrlLayerBackSize;
	CCtrlPictureWZ*		m_pPicCtrlLayer;
	CCtrlStaticWZ*		m_pTxtCtrlLayer;

#ifdef _NA_000000_20130114_RENEWER_UI
	bool				show_hide_check_;

	CCtrlPictureWZ*		show_on_radar_iamge2_;
	CCtrlPictureWZ*		show_on_radar_iamge3_;
	CCtrlButtonWZ*		show_on_radar_button_;
#endif
};


//========================================================================================

#endif // __RADAR_DIALOG_H__

