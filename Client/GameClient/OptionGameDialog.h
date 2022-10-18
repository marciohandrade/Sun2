

#ifndef		OPTIONGAMEDIALOG_H
#define		OPTIONGAMEDIALOG_H

#include "uiBase.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
class OptionGameDialog : public uiBase
{
	enum eOPTION_GAME_POS
	{
		OPTION_GAME_BT_EXIT,				//	��ư - ����

		OPTION_GAME_BT_INIT,				//	��ư - �ʱ�ȭ
		OPTION_GAME_BT_OK,					//	��ư - Ȯ��

		
		OPTION_GAME_TEXT_DAMAGE,			//	�ý�Ʈ - ���� ��ġ

		OPTION_GAME_CHECK_NAME,				//	üũ��ư - �̸�
		OPTION_GAME_CHECK_GRADE,			//	üũ��ư - ���
		OPTION_GAME_CHECK_GUILD,			//	üũ��ư - ����
		OPTION_GAME_CHECK_PET_NAME,			//	üũ�ڽ� - �����
		
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
		OPTION_GAME_CHECK_HEAD_EFFECT,		//  üũ�ڽ� - �κ�����Ʈ
#endif//__CN_ADD_HEAD_EFFECT_FLAG

		OPTION_GAME_CHECK_NPC,				//	üũ��ư - NPC �̸�
		OPTION_GAME_CHECK_HELM,				//	üũ��ư - ���� ���̱�
		OPTION_GAME_CHECK_AUTOATTACK,		//	üũ��ư - �ڵ� ����
		OPTION_GAME_CHECK_HELP,				//	üũ��ư - ����
		OPTION_GAME_CHECK_FIX_UI,			//  üũ��ư - �������̽� ����
        OPTION_GAME_CHECK_AUTOTARGET,		//	üũ��ư - �ڵ� Ÿ��
        OPTION_GAME_CHECK_FORWARDTARGET,    //  üũ��ư - ������ Ÿ��
        OPTION_GAME_CHECK_FIXTARGET,        //  üũ��ư - Ÿ�� ����
        OPTION_GAME_CHECK_CHANGE_EYE_MOUSE, //  üũ��ư - �þ� ���콺 ����
        OPTION_GAME_CHECK_MOVE_TO_CAMERA,   //  üũ��ư - ī�޶� �������� �̵�
        OPTION_GAME_HSCROLL_MOUSE_SENS,     //  ���� ��ũ�� - ���콺 ���� ����
        OPTION_GAME_HSCROLL_MOUSE_VIEW_SENS,//  ���� ��ũ�� - ���� ��ȯ �ӵ�

		OPTION_GAME_CKECK_SKILL_QUEUEING,		// üũ��ư - ��ųť��
		OPTION_GAME_HSCROLL_MOUSE_VIEW_SPEED,	// ���� ��ũ�� - ���콺 ���� �ӵ�

		OPTION_GAME_BT_DAMAGE_LEFT,				//	��ư - ( ���� ��ġ ǥ��) LEFT
		OPTION_GAME_BT_DAMAGE_RIGHT,			//	��ư - ( ���� ��ġ ǥ��) RIGHT

		OPTION_GAME_CHECK_MARKET_TITLE,
		OPTION_GAME_CHECK_MULTITHREAD,
		OPTION_GAME_CHECK_MOVIE_PLAY,
		OPTION_GAME_CHECK_CAMERA_DISTANCE,
		OPTION_GAME_CHECK_SPRING_CAMERA,
		OPTION_GAME_CHECK_WHISPER_BLOCK,
		OPTION_GAME_CHECK_TRADE_BLOCK,
		OPTION_GAME_CHECK_DISPLAY_MONSTERINFO,
		OPTION_GAME_CHECK_SHOW_MY_RENDERINFO,

#ifdef _YJ_PK_OPTION_001120
		OPTION_GAME_CHECK_PK_WITHOUT_CTRL_KEY,
#endif //_YJ_PK_OPTION_001120

		OPTION_GAME_CHECK_PICK_UP_ITEM_NOTIFY,
		OPTION_GAME_CHECK_NOTIFY_DROP_ITEM,

		OPTION_GAME_CHECK_SHOW_PLAYERS,
		OPTION_GAME_CHECK_SHOW_VENDORS,
#ifdef _DH_OS_LANGUAGETYPE
		OPTION_GAME_TEXT_LANGUAGE,
		OPTION_GAME_LIST_LANGUAGE,
		OPTION_GAME_BACK_LANGUAGE,
		OPTION_GAME_SCROLL_BACK_LANGUAGE,
		OPTION_GAME_SCORLL_LANGUAGE,
		OPTION_GAME_POPUP_BUTTON,
#endif//_DH_OS_LANGUAGETYPE

        OPTION_GAME_CHANNEL_ACCEPT,

        OPTION_GAME_HSCROLL_CAMERA_VIEW_POS,

#ifdef _DH_BATTLEZONE2_
        OPTION_GAME_PICKING_FRIEND,
#endif //_DH_BATTLEZONE2_
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        OPTION_VISIBLE_WING,
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
		OPTION_GAME_MAX,
	};

	enum eOptionGameList
	{
		OGLC_DAMAGE			= 0,

		OGLC_MAX,
	};

	enum eOptionGameCheckBox
	{
		OGCB_NAME		= 0,
		OGCB_GRADE,
		OGCB_GUILD,
		OGCB_PET_NAME,	
		
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
		OGCB_HEAD_EFFECT,
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN�Ǽ��縮����Ʈ��ư)

		OGCB_NPC,
		OGCB_HELM,
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        OGCB_WING,
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
		OGCB_AUTOATTACK,
		OGCB_HELP,
		OGCB_FIX_UI,
        OGCB_AUTOTARGET,
        OGCB_FORWARDTARGET,
        OGCB_FIXTARGET,
        OGCB_CHANGE_EYE_MOUSE,
        OGCB_MOVE_TO_CAMERA,
		OGCB_SKILL_QUEUEING,
		OGCB_MARKET_TITLE,
		OGCB_MULTITHREAD,
		OGCB_MOVIE_PLAY,
		OGCB_CAMERA_DISTANCE,
		OGCB_SPRING_CAMERA,
		OGCB_WHISPER_BLOCK,
		OGCB_TRADE_BLOCK,
		OGCB_DISPLAY_MONSTERINFO,
		OGCB_SHOW_MY_RENDERINFO,

#ifdef _YJ_PK_OPTION_001120
		OGCB_PK_WITHOUT_CTRL_KEY,
#endif //_YJ_PK_OPTION_001120

		OGCB_PICK_UP_ITEM_NOTIFY,
		OGCB_NOTIFY_DROP_ITEM,

		OGCB_SHOW_PLAYERS,
		OGCB_SHOW_VENDORS,
        OGCB_CHANNEL_ACCEPT,

#ifdef _DH_BATTLEZONE2_
        OGCB_PICKING_FRIEND,
#endif //_DH_BATTLEZONE2_
		OGCB_MAX,
	};

    enum eOptionGameHscoll
    {
        OGHS_MOUSE_SENS,
        OGHS_MOUSE_VIEW_SENS,
		OGHS_MOUSE_VIEW_SPEED,
        OGHS_CAMERA_VIEW_POS,
        OGHS_MAX,
    };

protected:
	static WzID				m_wzId[OPTION_GAME_MAX];

	//	�� ��Ʈ��
	CCtrlButtonCheckWZ		*m_pCheckBox[OGCB_MAX];

	CCtrlStaticWZ			*m_pStaticDamage;


    CCtrlHScrollWZ*         m_pHScroll[OGHS_MAX];

	//	���� ������ �ӽ� ����
	OptionParam				m_InitOptionParam;
	OptionParam				m_OptionParam;	
#ifdef _DH_OS_LANGUAGETYPE
	CCtrlStaticWZ* language_text_;
	CCtrlListWZ* language_list_;
	CCtrlPictureWZ* language_back_;
	CCtrlPictureWZ* language_vscroll_back_;
	CCtrlVScrollWZ* language_vscroll_;
	bool is_language_popup_;
#endif//_DH_OS_LANGUAGETYPE

public:
	OptionGameDialog( InterfaceManager * pUIMan );
	virtual ~OptionGameDialog();

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	VOID			OnShowWindow( BOOL val );
	//void            UpdatePosition();

	void			ApplyControl( void );
	void			ApplyScroll();

public:
#ifdef _DH_OS_LANGUAGETYPE
	void ShowLanguageOption(bool is_show);
	void SetLanguage(LanguageType language_type);
	DWORD GetLanguage(LanguageType language_type);
#endif//_DH_OS_LANGUAGETYPE
	OptionParam		*GetOptionParam( void ) { return &m_OptionParam; }
	BOOL			Apply( BOOL bCheckApply = TRUE ,BOOL bProgramInit = FALSE);
	//void			SetAutoTarget();
	//void			SetSkillQueueing();
	//void			SetFixTarget();
	//void			SetForwardTarget();
	void			SetHelmetInfo(BOOL bFlag);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    void			SetWingInfo(BOOL bFlag);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	void			InitBlockCheck( BOOL bTradeBlock, BOOL bWhisperBlock );
	void			SetBlockCheck( eINTERCEPTION_TYPE eType, BOOL bFlag );
	void			Send_CG_STATUS_INTERCEPTION_SYN(eINTERCEPTION_TYPE eType, BOOL bFlag);

private:
    void _OnApplyChangedOption( nOptionType::eOptionTypeIndex TypeIndex, int NewOptionValue );
};

float GetCameraPos(int index);
#endif

#endif	//	OPTIONGAMEDIALOG_H


