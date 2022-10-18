

#ifndef		_OPTIONDIALOG_H
#define		_OPTIONDIALOG_H

#include "uiBase.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
class OptionDialog : public uiBase
{
	enum eOPTION_POS
	{
		OPTION_BT_EXIT = 0,				//	버튼 - 종료
		OPTION_BT_INIT,					//	초기화 버튼
		OPTION_BT_AUTO_INIT,			//	자동설정 버튼 - 사용 안함
		OPTION_BT_OK,					//	키설정 버튼 - OK버튼으로

		OPTION_BT_SCREEN,				//	해상도 버튼
		OPTION_BT_TEXTURE_LEFT,			//	택스쳐 품질 버튼 (좌)
		OPTION_BT_TEXTURE_RIGHT,		//	택스쳐 품질 버튼 (우)
		OPTION_BT_LOD_LEFT,				//	LOD (좌)
		OPTION_BT_LOD_RIGHT,			//	LOD (좌)
		OPTION_BT_EFFECT_LEFT,			//  이펙트 품질 (좌)
		OPTION_BT_EFFECT_RIGHT,			//  이펙트 품질 (우)

		OPTION_CHECKBOX_SHADOW,			//  그림자 체크박스
		OPTION_CHECKBOX_GLOW,			//	뽀샤시 체크박스
		OPTION_CHECKBOX_MAP_SPECULAR,	//	배경 스펙큘러 효과 체크박스
		OPTION_CHECKBOX_FOG,			//	안개효과 체크박스

		OPTION_STATIC_SCREEN,			//	해상도 
		OPTION_STATIC_TEXTURE,			//	택스쳐 품질
		OPTION_STATIC_LOD,				//	LOD
		OPTION_STATIC_EFFECT,			//	이펙트 품질
		
		OPTION_LIST_SCREEN,				//	해상도 리스트
		
		OPTION_HSCROLL_GAMMA,			// 감마조절 스크롤
		OPTION_HSCROLL_CULL,

		OPTION_LIST_SCREEN_VSCR,		// 해상도 스크롤 
		OPTION_LIST_SCREEN_PICTURE,     // 스크롤 백판 

#ifdef _YMS_WINDOW_MAXSCREEN
#else
		OPTION_LIST_SCREEN_BG,			// 리스트 백판 
#endif //_YMS_WINDOW_MAXSCREEN

		OPTION_CHECKBOX_TILEBLENDING,	// 타일블렌딩
        OPTION_CHECKBOX_VOLUME_LIGHT,	// 태양효과

#ifdef _YMS_WINDOW_MAXSCREEN
        OPTION_SCREEN_MODE_LISTBOX, //!< 전체화면 창모드
        OPTION_SCREEN_MODE_BUTTON, 
        OPTION_SCREEN_MODE_STATIC, 
#endif //_YMS_WINDOW_MAXSCREEN
      
		OPTION_MAX,
	};	

	enum eOptionCheckBox
	{
		OCB_SHADOW		= 0,
		OCB_GLOW,
		OCB_MAP_SPECULAR,
		OCB_FOG,
		OCB_BLEND,
        OCB_VOLUME_LIGHT,
		OCB_MAX,
	};

	enum eOptionStatic
	{
		OST_SCREEN				= 0,
		OST_TEXTURE,
		OST_LOD,
		OST_EFFECT,
	
		OST_MAX,
	};


protected:
	static WzID				m_wzId[OPTION_MAX];


	//	각 컨트롤
	CCtrlButtonCheckWZ		*m_pCheckBox[OCB_MAX];
	CCtrlStaticWZ			*m_pStatic[OST_MAX];

	CCtrlHScrollWZ			*m_pHScrollGamma;
	CCtrlHScrollWZ			*m_pHScrollCull;

	CCtrlListWZ				*m_pListScreen;
	BOOL					m_bListScreenShow;	

	//	각종 설정값 임시 보관
	OptionParam				m_InitOptionParam;
	OptionParam				m_OptionParam;	

	CCtrlVScrollWZ *		m_pScreenVScrollCtrl;
	CCtrlPictureWZ *        m_pScrollBg;
	CCtrlPictureWZ *        m_pScreenBg;


public:
	OptionDialog( InterfaceManager * pUIMan );
	virtual ~OptionDialog( void );

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual VOID	OnUpdateSolarDialog();

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	VOID			OnShowWindow( BOOL val );
	//void            UpdatePosition();

	void			ApplyControl(bool is_check_mode = false);
	void			ApplyScroll();

	//해상도 스크롤추가
	void			UpScroll();
	void			DownScroll();
	void			UpdateMouseWheel();

public:
	OptionParam		*GetOptionParam( void ) { return &m_OptionParam; }

	BOOL			Apply( BOOL bCheckApply = TRUE ,BOOL bProgramInit = FALSE);
};



#endif
#endif	//	_OPTIONDIALOG_H



