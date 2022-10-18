//=======================================================================================================================
/// RadarFullDialog
/**
@author
임대진 < genius200@webzen.co.kr >
@since

@remarks
- 		
*/


#pragma once
#include "solardialog.h"
#include "BaseRadarDialog.h"
#include "RadarFullMissionInfo.h"

class RadarFullDialog : public BaseRadarDialog
{
    friend class uiRadarMan;


public:

	enum eRADAR_PLUS_POS
	{	
		RADAR_FULL_MAP_IMG,
		RADAR_FULL_FRAME,
		RADAR_FULL_EXIT,
		RADAR_FULL_MAP_NAME,
		RADAR_FULL_ALPHA_MAP,
		RADAR_FULL_TEXT,
        RADAR_FULL_TEXT2,
		RADAR_FULL_CHECKSHOWMISSION,
		RADAR_FULL_TEXT3,
        RADAR_FULL_UNEXTEND,
        RADAR_FULL_EXTEND,

        RADAR_BASE_IMAGE_P000,
        RADAR_BASE_IMAGE_P001,

		RADAR_FULL_MAX,
	};	

	RadarFullDialog(InterfaceManager *pUIMan);
	virtual ~RadarFullDialog(void);

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual BOOL	IsVisibleWindow();

public:
	virtual void	ChangeMap(TCHAR* file_name = NULL);
    virtual bool    GetPictureSize(RECT& rect);;

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    void			CheckModifyOption();
#endif

protected:
	virtual VOID	OnShowWindow( BOOL val );
	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

    CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

protected:
	VOID			loadMapImage( WORD fieldID );
	VOID			updateMap();
    VOID            doActionSize();

    void ShowMapImageControl();

	virtual DWORD	getOpenDlgSoundIndex()
	{
		return UI_SOUND_MAP_OPEN;
	}

	virtual DWORD	getCloseDlgSoundIndex()
	{
		return UI_SOUND_MAP_CLOSE;
	}

    virtual bool is_full_map() { return true; }

	//Stinr형 16진수 핵사 코드를 10진수 DWORD값으로 만드는 함수
	__forceinline DWORD  GetHexValue(const TCHAR* Value);
	__forceinline int    GetHexMulti(int Value);
	__forceinline int    GetHexConvertor(char charset);
	

protected:
	static WzID		m_wzId[RADAR_FULL_MAX];
	WORD Offsettops_ ;
	WORD Offsetlefts_;

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
	CControlWZ*				 m_CheckBoxCtrl;
	bool					 showMissionInfo_;
	bool					 showMissionController_;
	std::vector<MissionData*> m_listMissionHelper_;

#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_0_20111013_RADAR_PATHFINDING
private:
    bool OnClickRadar();
#endif//_NA_0_20111013_RADAR_PATHFINDING
    BOOL CheckClickedPosition();

};