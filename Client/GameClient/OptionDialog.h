

#ifndef		_OPTIONDIALOG_H
#define		_OPTIONDIALOG_H

#include "uiBase.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
class OptionDialog : public uiBase
{
	enum eOPTION_POS
	{
		OPTION_BT_EXIT = 0,				//	��ư - ����
		OPTION_BT_INIT,					//	�ʱ�ȭ ��ư
		OPTION_BT_AUTO_INIT,			//	�ڵ����� ��ư - ��� ����
		OPTION_BT_OK,					//	Ű���� ��ư - OK��ư����

		OPTION_BT_SCREEN,				//	�ػ� ��ư
		OPTION_BT_TEXTURE_LEFT,			//	�ý��� ǰ�� ��ư (��)
		OPTION_BT_TEXTURE_RIGHT,		//	�ý��� ǰ�� ��ư (��)
		OPTION_BT_LOD_LEFT,				//	LOD (��)
		OPTION_BT_LOD_RIGHT,			//	LOD (��)
		OPTION_BT_EFFECT_LEFT,			//  ����Ʈ ǰ�� (��)
		OPTION_BT_EFFECT_RIGHT,			//  ����Ʈ ǰ�� (��)

		OPTION_CHECKBOX_SHADOW,			//  �׸��� üũ�ڽ�
		OPTION_CHECKBOX_GLOW,			//	�ǻ��� üũ�ڽ�
		OPTION_CHECKBOX_MAP_SPECULAR,	//	��� ����ŧ�� ȿ�� üũ�ڽ�
		OPTION_CHECKBOX_FOG,			//	�Ȱ�ȿ�� üũ�ڽ�

		OPTION_STATIC_SCREEN,			//	�ػ� 
		OPTION_STATIC_TEXTURE,			//	�ý��� ǰ��
		OPTION_STATIC_LOD,				//	LOD
		OPTION_STATIC_EFFECT,			//	����Ʈ ǰ��
		
		OPTION_LIST_SCREEN,				//	�ػ� ����Ʈ
		
		OPTION_HSCROLL_GAMMA,			// �������� ��ũ��
		OPTION_HSCROLL_CULL,

		OPTION_LIST_SCREEN_VSCR,		// �ػ� ��ũ�� 
		OPTION_LIST_SCREEN_PICTURE,     // ��ũ�� ���� 

#ifdef _YMS_WINDOW_MAXSCREEN
#else
		OPTION_LIST_SCREEN_BG,			// ����Ʈ ���� 
#endif //_YMS_WINDOW_MAXSCREEN

		OPTION_CHECKBOX_TILEBLENDING,	// Ÿ�Ϻ���
        OPTION_CHECKBOX_VOLUME_LIGHT,	// �¾�ȿ��

#ifdef _YMS_WINDOW_MAXSCREEN
        OPTION_SCREEN_MODE_LISTBOX, //!< ��üȭ�� â���
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


	//	�� ��Ʈ��
	CCtrlButtonCheckWZ		*m_pCheckBox[OCB_MAX];
	CCtrlStaticWZ			*m_pStatic[OST_MAX];

	CCtrlHScrollWZ			*m_pHScrollGamma;
	CCtrlHScrollWZ			*m_pHScrollCull;

	CCtrlListWZ				*m_pListScreen;
	BOOL					m_bListScreenShow;	

	//	���� ������ �ӽ� ����
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

	//�ػ� ��ũ���߰�
	void			UpScroll();
	void			DownScroll();
	void			UpdateMouseWheel();

public:
	OptionParam		*GetOptionParam( void ) { return &m_OptionParam; }

	BOOL			Apply( BOOL bCheckApply = TRUE ,BOOL bProgramInit = FALSE);
};



#endif
#endif	//	_OPTIONDIALOG_H



