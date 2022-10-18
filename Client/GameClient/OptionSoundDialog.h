#ifndef		_OPTIONSOUNDDIALOG_H
#define		_OPTIONSOUNDDIALOG_H

#include "uiBase.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
class OptionSoundDialog : public uiBase
{
	enum eOPTION_POS
	{
		OPTION_SOUND_BT_EXIT = 0,			//	��ư - ����
		OPTION_SOUND_BT_INIT,				//	�ʱ�ȭ ��ư
		OPTION_SOUND_BT_OK,					//	Ű���� ��ư - OK��ư����

		OPTION_SOUND_BG_MUSIC,				//	������� ��ũ��
		OPTION_SOUND_AMBIENT_EFFECT_SOUND,	//	���ȿ���� ��ũ��
		OPTION_SOUND_EFFECT_SOUND,			//	ȿ���� ��ũ��

		OPTION_SOUND_SYSTEM,				// ������ ���� ��ũ��
		OPTION_SOUND_SPEAKER,				// ���̺� ��ũ��
		OPTION_SOUND_MIC,					// ����ũ ��ũ��

		OPTION_SOUND_CHECKBOX_SYSTEM_VOICE,	//	�ý��� ���̽�
		OPTION_SOUND_CHECKBOX_NPC_VOICE,	//	NPC ���̽�
		
		OPTION_SOUND_STATIC_VOICE_CHAT_CATEGORY,	//  ����ä��  �ؽ�Ʈ

		OPTION_SOUND_MAX,
	};	

	enum eOptionCheckBox
	{
		OCB_SYSTEM_VOICE		= 0,
		OCB_NPC_VOICE,
		
		OCB_MAX,
	};


	enum eOptionVolume
	{
		OHS_BGM				= 0,
		OHS_AMBIENT_EFFECT,
		OHS_EFFECT,
		OHS_SYSTEM,
		OHS_SPEAKER,
		OHS_MIC,

		OHS_MAX,
	};


protected:
	static WzID				m_wzId[OPTION_SOUND_MAX];


	//	�� ��Ʈ��
	CCtrlButtonCheckWZ		*m_pCheckBox[OCB_MAX];
	CCtrlHScrollWZ			*m_pHScroll[OHS_MAX];

	//	���� ������ �ӽ� ����
	OptionParam				m_InitOptionParam;
	OptionParam				m_OptionParam;	

public:
	OptionSoundDialog( InterfaceManager * pUIMan );
	virtual ~OptionSoundDialog( void );

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
	void			SoundSetting();

public:
	OptionParam		*GetOptionParam( void ) { return &m_OptionParam; }

	BOOL			Apply( BOOL bCheckApply = TRUE ,BOOL bProgramInit = FALSE);
};
#endif
#endif	//	_OPTIONSOUNDDIALOG_H



