//=======================================================================================================================
/// RadarPlusDialog
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/


#pragma once
#include "solardialog.h"
#include "RadarInfoParser.h"
#include "timer.h"

#include "BaseRadarDialog.h"

class RadarPlusDialog : public BaseRadarDialog
{
    friend class uiRadarMan;

public:

	enum eRADAR_PLUS_POS
	{	
		RADAR_PLUS_MAP_IMG,
		RADAR_PLUS_MAP_NAME,
		RADAR_PLUS_MAP_POS,
		RADAR_PLUS_MINUS,		// minus 인지 확실히 알자..
		RADAR_PLUS_EXIT,
		RADAR_PLUS_PIC_LAYER_BACKSIZE,
		RADAR_PLUS_PIC_LAYER_SIZE,
		RADAR_PLUS_PIC_LAYER_COUNT,
		RADAR_PLUS_TXT_LAYER_COUNT,

#ifdef _NA_000000_20130114_RENEWER_UI
		RADAR_MAP_B001,
		RADAR_MAP_P002,
		RADAR_MAP_P003,

		RADAR_MAP_BACK,
		RADAR_MAP_BACK2,
#endif


		RADAR_PLUS_MAX,
	};	

	RadarPlusDialog(InterfaceManager *pUIMan);
	virtual ~RadarPlusDialog(void);

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual BOOL	IsVisibleWindow();
	void            UpdatePosition();

public:
	virtual void	ChangeMap(TCHAR* file_name = NULL);


	virtual void	SetCurLayer( int iCurLayer = 1);
	virtual void	SetMaxLayer( int iMaxLayer );

	
	BOOL            CheckClickedPosition();

#ifdef _NA_000000_20130114_RENEWER_UI
	void HideAllControl(bool value);
	void DisablePosInWindow(bool value);
	bool gethidecheck() { return show_hide_check_; }
#endif

protected:
	static WzID		m_wzId[RADAR_PLUS_MAX];
	virtual VOID	OnShowWindow( BOOL val );
	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

    CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

protected:
	VOID			loadMapImage( WORD fieldID );
	VOID			updateMap();

	void			InitControls( void );
	void			InitLayerShowNHide( void );
	virtual BOOL	InterSectRect( POINT point );

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

