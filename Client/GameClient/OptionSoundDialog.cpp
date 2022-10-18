#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "OptionSoundDialog.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "globalfunc.h"

const int	c_iSoundMinVolume = 0;
const int	c_iSoundMaxVolume = 100;

#pragma warning ( push )
#pragma warning(disable : 4800)

#ifdef _NA_000000_20130114_RENEWER_UI
#else

WzID OptionSoundDialog::m_wzId[OPTION_SOUND_MAX] = 
{ 
	StrToWzID("B999"),	//	��ư - ����
	StrToWzID("TI06"),	//	�ʱ�ȭ ��ư
	StrToWzID("TI07"),	//	OK 

	StrToWzID("H000"),	//	������� ��ũ��
	StrToWzID("H001"),	//	���ȿ���� ��ũ��
	StrToWzID("H002"),	//	ȿ���� ��ũ��

	StrToWzID("H003"),	//	OPTION_SOUND_SYSTEM,				// �ý��� ���� ��ũ��
	StrToWzID("H004"),	//	OPTION_SOUND_SPEAKER,				// ����Ŀ ��ũ��
	StrToWzID("H005"),	// OPTION_SOUND_MIC,					// ����ũ ��ũ��

	StrToWzID("C005"),	//	�ý��� ���� üũ�ڽ�
	StrToWzID("C006"),	//	NPC ���� üũ�ڽ�

	StrToWzID("T102"),  // ����ä�� �ؽ�Ʈ 
	
};




OptionSoundDialog::OptionSoundDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
	int		i;
	for( i = 0 ; i < OCB_MAX ; ++i )
	{
		m_pCheckBox[i] = NULL;
	}
	for( i = 0 ; i < OHS_MAX ; ++i )
	{
		m_pHScroll[i] = NULL;
	}
}

OptionSoundDialog::~OptionSoundDialog( void )
{
}

VOID OptionSoundDialog::Init( CDrawBase * pDrawBase )
{
	//UpdatePosition();

	TCHAR	szMessage[INTERFACE_STRING_LENGTH]	= {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH]		= {0,};

	DWORD	dwTextColor	= RGBA( 235, 192, 38, 255 );
	DWORD	dwWideTextColor	= RGBA( 245, 166, 46, 255 );
	DWORD	dwSelectColor = RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 0 );
	DWORD	dwSetColorSelBarOnText = RGBA( 255, 255, 255, 255 );

	//	üũ �ڽ� ������( ȿ�� ���� �ѱ� )
	m_pCheckBox[OCB_SYSTEM_VOICE]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_SOUND_CHECKBOX_SYSTEM_VOICE));
	m_pCheckBox[OCB_NPC_VOICE]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_SOUND_CHECKBOX_NPC_VOICE));

	//TODO: ����ä�ÿ� ��Ʈ���� UI���� ���ŵǸ� �� �ڵ嵵 �����ؾ� ��
	CCtrlStaticWZ *pStaticCtrl = static_cast<CCtrlStaticWZ *>(getControl(OPTION_SOUND_STATIC_VOICE_CHAT_CATEGORY));
	if(pStaticCtrl)
		pStaticCtrl->ShowWindowWZ(WZ_HIDE);
	


	//	���� ���� ��ũ�� �� ������
	m_pHScroll[OHS_BGM] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_BG_MUSIC));
    if (m_pHScroll[OHS_BGM])
    {
        if (m_pHScroll[OHS_BGM])
            m_pHScroll[OHS_BGM]->SetScrollRangeWZ( c_iSoundMinVolume, c_iSoundMaxVolume );
    }

	m_pHScroll[OHS_AMBIENT_EFFECT] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_AMBIENT_EFFECT_SOUND));
    if (m_pHScroll[OHS_AMBIENT_EFFECT])
    {
        m_pHScroll[OHS_AMBIENT_EFFECT]->SetScrollRangeWZ( c_iSoundMinVolume, c_iSoundMaxVolume );
    }

	m_pHScroll[OHS_EFFECT] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_EFFECT_SOUND));
    if (m_pHScroll[OHS_EFFECT])
    {
        m_pHScroll[OHS_EFFECT]->SetScrollRangeWZ( c_iSoundMinVolume, c_iSoundMaxVolume );
    }

	m_pHScroll[OHS_SYSTEM] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_SYSTEM));
    if (m_pHScroll[OHS_SYSTEM])
    {
        m_pHScroll[OHS_SYSTEM]->SetScrollRangeWZ( c_iSoundMinVolume, c_iSoundMaxVolume );
    }

	m_pHScroll[OHS_SPEAKER] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_SPEAKER));
    if (m_pHScroll[OHS_SPEAKER])
    {
        m_pHScroll[OHS_SPEAKER]->SetScrollRangeWZ( c_iSoundMinVolume, c_iSoundMaxVolume );
    }

	m_pHScroll[OHS_MIC] = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_SOUND_MIC));

	if (m_pHScroll[OHS_MIC])
	{
		//TODO: ����ä�ÿ� ��Ʈ���� UI���� ���ŵǸ� �� �ڵ嵵 �����ؾ� ��
		m_pHScroll[OHS_MIC]->ShowWindowWZ(WZ_HIDE); //����ũ ���� ��ũ�� ���� 
	}


	//-------------------------------------------------------------------------
	//		�ɼ� ������ ����
	//-------------------------------------------------------------------------

	m_OptionParam = GAMEOPTION->GetGameOption();
    m_OptionParam.SetOptionValue( nOptionType::SYSTEM_VOLUME, Sound::Instance()->GetMasterVolume() );
    m_OptionParam.SetOptionValue( nOptionType::SPEAKER_VOLUME, Sound::Instance()->GetWaveVolume() );

	m_InitOptionParam = m_OptionParam;

	//	��Ʈ�ѿ� ����
	ApplyControl();
}


VOID OptionSoundDialog::Release()
{
}


VOID OptionSoundDialog::MessageProc( SI_MESSAGE * pMessage )
{
	int		iIndex = -1;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	*pszTRUE = _T("TRUE");
	TCHAR	*pszFALSE = _T("FALSE");

	int		iScrollPos;
	float	fSoundVolume = 0.0f;
	float	fVolumeStep = 0.15f;

	switch( pMessage->eResultMsg ) 
	{
		//-----------------------------------------------------------------------------------------
		//		���� ��ũ�� �� ���콺�� ������
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_THUMBPOSITION:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_SOUND_BG_MUSIC:
				{
					iScrollPos = m_pHScroll[OHS_BGM]->GetScrollPosWZ();
					if( iScrollPos <= 0 )
					{
						g_SoundBGM.SetVolume( 0.0f );
					}
					else
					{
						fSoundVolume = (float)iScrollPos / (float)c_iSoundMaxVolume;
						g_SoundBGM.SetVolume( fSoundVolume );
					}

					//	���� ����
                    m_OptionParam.SetOptionValue( nOptionType::BGM_VOLUME, iScrollPos );
				}
				break;

			case OPTION_SOUND_AMBIENT_EFFECT_SOUND:
				{
					iScrollPos = m_pHScroll[OHS_AMBIENT_EFFECT]->GetScrollPosWZ();
					if( iScrollPos <= 0 )
					{
					}
					else
					{
						fSoundVolume = (float)iScrollPos / (float)c_iSoundMaxVolume;
					}

					//	���� ����
                    m_OptionParam.SetOptionValue( nOptionType::AMBIENT_VOLUME, iScrollPos );

                    if (g_pMap)
					    g_pMap->SetAmbientVolume(fSoundVolume);
				}
				break;

			case OPTION_SOUND_EFFECT_SOUND:
				{
                    iScrollPos = m_pHScroll[OHS_EFFECT]->GetScrollPosWZ();
                    
					if( iScrollPos <= 0 )
					{
					}
					else
					{
						fSoundVolume = (float)iScrollPos / (float)c_iSoundMaxVolume;
					}

					//	���� ����
                    m_OptionParam.SetOptionValue( nOptionType::EFFECT_VOLUME, iScrollPos );
				}
				break;

			case OPTION_SOUND_SYSTEM:				// ������ ���� ��ũ��
				{
					iScrollPos = m_pHScroll[OHS_SYSTEM]->GetScrollPosWZ();
					
					if( iScrollPos > 0 )
					{
						fSoundVolume = (float)iScrollPos / (float)c_iSoundMaxVolume;
					}

					Sound::Instance()->SetMasterVolume( fSoundVolume, FALSE );
					
					//	���� ����
                    m_OptionParam.SetOptionValue( nOptionType::SYSTEM_VOLUME, iScrollPos );
				}
				break;

			case OPTION_SOUND_SPEAKER:				// ���̺� ��ũ��
				{
					iScrollPos = m_pHScroll[OHS_SPEAKER]->GetScrollPosWZ();
					
					if( iScrollPos > 0 )
					{
						fSoundVolume = (float)iScrollPos / (float)c_iSoundMaxVolume;
					}

					Sound::Instance()->SetWaveVolume( fSoundVolume, FALSE );
					
					//	���� ����
                    m_OptionParam.SetOptionValue( nOptionType::SPEAKER_VOLUME, iScrollPos );
				}
				break;

			}
			
		}
		break;

		//-----------------------------------------------------------------------------------------
		//		���� ��ũ�� �� ���� ��ư Ŭ��
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINELEFT:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_SOUND_BG_MUSIC:
				{
                    int OldBGMVolume = m_OptionParam.GetOptionValue(nOptionType::BGM_VOLUME);
                    int SoundVolume = max(0, OldBGMVolume - 15 );

                    g_SoundBGM.SetVolume( (float)SoundVolume*0.01f );

                    if (m_pHScroll[OHS_BGM])
					    m_pHScroll[OHS_BGM]->SetScrollPosWZ( SoundVolume );

					//	���� ����
                    m_OptionParam.SetOptionValue(nOptionType::BGM_VOLUME, SoundVolume );
				}
				break;

			case OPTION_SOUND_AMBIENT_EFFECT_SOUND:
				{
                    int OldAmbientVolume = m_OptionParam.GetOptionValue(nOptionType::AMBIENT_VOLUME);
                    int SoundVolume = max(0, OldAmbientVolume - 15 );

                    if (m_pHScroll[OHS_AMBIENT_EFFECT])
    					m_pHScroll[OHS_AMBIENT_EFFECT]->SetScrollPosWZ( SoundVolume );

					//	���� ����
                    m_OptionParam.SetOptionValue(nOptionType::AMBIENT_VOLUME, SoundVolume );

                    if (g_pMap)
					    g_pMap->SetAmbientVolume((float)SoundVolume*0.01f);
				}
				break;

			case OPTION_SOUND_EFFECT_SOUND:
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::EFFECT_VOLUME);
                    int Volume = max(0, OldVolume - 15 );

                    if (m_pHScroll[OHS_EFFECT])
					    m_pHScroll[OHS_EFFECT]->SetScrollPosWZ( Volume );

					//	���� ����
                    m_OptionParam.SetOptionValue(nOptionType::EFFECT_VOLUME, Volume );
				}
				break;

			case OPTION_SOUND_SYSTEM:				// ������ ���� ��ũ��
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::SYSTEM_VOLUME);
                    int Volume = max(0, OldVolume - 15 );

                    if (m_pHScroll[OHS_SYSTEM])
                        m_pHScroll[OHS_SYSTEM]->SetScrollPosWZ( Volume );

                    //	���� ����
                    m_OptionParam.SetOptionValue(nOptionType::SYSTEM_VOLUME, Volume );

					Sound::Instance()->SetMasterVolume( (float)Volume, FALSE );
				}
				break;

			case OPTION_SOUND_SPEAKER:				// ���̺� ��ũ��
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::SPEAKER_VOLUME);
                    int Volume = max(0, OldVolume - 15 );

                    if (m_pHScroll[OHS_SPEAKER])
                        m_pHScroll[OHS_SPEAKER]->SetScrollPosWZ( Volume );

                    //	���� ����
                    m_OptionParam.SetOptionValue(nOptionType::SPEAKER_VOLUME, Volume );

					Sound::Instance()->SetWaveVolume( (float)Volume * 0.01f, FALSE );
				}
				break;

			}
		}
		break;


		//-----------------------------------------------------------------------------------------
		//		���� ��ũ�� �� ������ ��ư Ŭ��
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINERIGHT:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_SOUND_BG_MUSIC:
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::BGM_VOLUME);
                    int Volume = min(100, OldVolume + 15 );

                    if (m_pHScroll[OHS_BGM])
                        m_pHScroll[OHS_BGM]->SetScrollPosWZ( Volume );

                    m_OptionParam.SetOptionValue( nOptionType::BGM_VOLUME, Volume );

					g_SoundBGM.SetVolume( (float)Volume * 0.01f );

				}
				break;

			case OPTION_SOUND_AMBIENT_EFFECT_SOUND:
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::AMBIENT_VOLUME);
                    int Volume = min(100, OldVolume + 15 );

                    if (m_pHScroll[OHS_AMBIENT_EFFECT])
                        m_pHScroll[OHS_AMBIENT_EFFECT]->SetScrollPosWZ( Volume );

                    m_OptionParam.SetOptionValue( nOptionType::AMBIENT_VOLUME, Volume );

                    if (g_pMap)
					    g_pMap->SetAmbientVolume((float)Volume * 0.01f);
				}
				break;

			case OPTION_SOUND_EFFECT_SOUND:
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::EFFECT_VOLUME);
                    int Volume = min(100, OldVolume + 15 );

                    if (m_pHScroll[OHS_EFFECT])
                        m_pHScroll[OHS_EFFECT]->SetScrollPosWZ( Volume );

                    m_OptionParam.SetOptionValue( nOptionType::EFFECT_VOLUME, Volume );
				}
				break;

			case OPTION_SOUND_SYSTEM:				// ������ ���� ��ũ��
				{
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::SYSTEM_VOLUME);
                    int Volume = min(100, OldVolume + 15 );

                    if (m_pHScroll[OHS_SYSTEM])
                        m_pHScroll[OHS_SYSTEM]->SetScrollPosWZ( Volume );

                    m_OptionParam.SetOptionValue( nOptionType::SYSTEM_VOLUME, Volume );

					Sound::Instance()->SetMasterVolume( (float)Volume * 0.01f, FALSE );
				}
				break;

			case OPTION_SOUND_SPEAKER:				// ���̺� ��ũ��
				{	
                    int OldVolume = m_OptionParam.GetOptionValue(nOptionType::SPEAKER_VOLUME);
                    int Volume = min(100, OldVolume + 15 );

                    if (m_pHScroll[OHS_SPEAKER])
                        m_pHScroll[OHS_SPEAKER]->SetScrollPosWZ( Volume );

                    m_OptionParam.SetOptionValue( nOptionType::SPEAKER_VOLUME, Volume );

                    Sound::Instance()->SetWaveVolume( (float)Volume * 0.01f, FALSE );
					
				}
				break;
			}
		}
		break;


		//-----------------------------------------------------------------------------------------
		//		���콺 ���� Ŭ�� �̺�Ʈ�� �Ͼ�� ��, ����?
		//-----------------------------------------------------------------------------------------
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				//---------------------------------------------------------------------------------
				//		üũ �ڽ���~
				//---------------------------------------------------------------------------------
			case OPTION_SOUND_CHECKBOX_SYSTEM_VOICE:
				{
					if (m_pCheckBox[OCB_SYSTEM_VOICE])
                        m_OptionParam.SetOptionValue( nOptionType::USE_SYSTEM_VOICE, m_pCheckBox[OCB_SYSTEM_VOICE]->GetCheckState() );
				}
				break;

			case OPTION_SOUND_CHECKBOX_NPC_VOICE:
				{
					if (m_pCheckBox[OCB_NPC_VOICE])
                        m_OptionParam.SetOptionValue( nOptionType::USE_NPC_VOICE, m_pCheckBox[OCB_NPC_VOICE]->GetCheckState() );
				}
				break;


				//---------------------------------------------------------------------------------
				//		�Ʒ� ���� ��ư�� X ���� ��ư
				//---------------------------------------------------------------------------------
			case OPTION_SOUND_BT_INIT:	//	�ʱ�ȭ - ó�� â ������ ���� ����
				{
					m_OptionParam = m_InitOptionParam;
					ApplyControl();
					SoundSetting();
				}
				break;

			case OPTION_SOUND_BT_OK:		//	OK ��ư - ������ �ǹ�
				{
					ApplyScroll();
					ApplyControl();
					Apply();
					ShowDialog(FALSE);
				}
				break;

			case OPTION_SOUND_BT_EXIT:	//	���� ��ư - ����� �ǹ�
				{
					m_OptionParam = m_InitOptionParam;
					ApplyControl();
					SoundSetting();
					ShowDialog(FALSE);
				}
				break;
			}

		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}

}

VOID OptionSoundDialog::NetworkProc( MSG_BASE * pMsg )
{
}


CControlWZ*  OptionSoundDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < OPTION_SOUND_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE OptionSoundDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < OPTION_SOUND_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return OPTION_SOUND_MAX;
}


VOID OptionSoundDialog::OnShowWindow( BOOL val )
{

	if( val == TRUE )
	{
		ApplyControl();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Option_Sound;
		msg.wParam=InterfaceManager::DIALOG_OPTION_SOUND;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

	}
	else
	{
		ApplyControl();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_Sound);
	}

}

void OptionSoundDialog::ApplyControl( void )
{
	//-------------------------------------------------------------------------
	//		üũ ��ư�� �� ����
	//-------------------------------------------------------------------------
    if (m_pCheckBox[OCB_SYSTEM_VOICE])
        m_pCheckBox[OCB_SYSTEM_VOICE]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_SYSTEM_VOICE) );
    if (m_pCheckBox[OCB_NPC_VOICE])
        m_pCheckBox[OCB_NPC_VOICE]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_NPC_VOICE) );


	//-------------------------------------------------------------------------
	//		���� ���� ���� �� �ʱ�ȭ
	//-------------------------------------------------------------------------

	//	��� ���� ���� ���� �� �ʱ�ȭ
    if (m_pHScroll[OHS_BGM])
	{
        m_pHScroll[OHS_BGM]->SetScrollPosWZ( m_OptionParam.GetOptionValue(nOptionType::BGM_VOLUME) );
	}

	//	��� ȿ���� ���� ���� �� �ʱ�ȭ
    if (m_pHScroll[OHS_AMBIENT_EFFECT])
	{
        m_pHScroll[OHS_AMBIENT_EFFECT]->SetScrollPosWZ( m_OptionParam.GetOptionValue(nOptionType::AMBIENT_VOLUME) );
	}

	//	�Ϲ� ȿ���� ���� ���� �� �ʱ�ȭ
    if (m_pHScroll[OHS_EFFECT])
	{
        m_pHScroll[OHS_EFFECT]->SetScrollPosWZ( m_OptionParam.GetOptionValue(nOptionType::EFFECT_VOLUME) );
	}

	// ������
	if (m_pHScroll[OHS_SYSTEM])
	{
        m_pHScroll[OHS_SYSTEM]->SetScrollPosWZ( m_OptionParam.GetOptionValue(nOptionType::SYSTEM_VOLUME) );
	}

	// ���̺�
	if (m_pHScroll[OHS_SPEAKER])
	{
        m_pHScroll[OHS_SPEAKER]->SetScrollPosWZ( m_OptionParam.GetOptionValue(nOptionType::SPEAKER_VOLUME) );
	}

}

void OptionSoundDialog::ApplyScroll()
{
	//	��� ���� ���� ���� �� �ʱ�ȭ
	if (m_pHScroll[OHS_BGM])
	{
        m_OptionParam.SetOptionValue( nOptionType::BGM_VOLUME, m_pHScroll[OHS_BGM]->GetScrollPosWZ() );
	}

	//	��� ȿ���� ���� ���� �� �ʱ�ȭ
	if (m_pHScroll[OHS_AMBIENT_EFFECT])
	{
        m_OptionParam.SetOptionValue( nOptionType::AMBIENT_VOLUME, m_pHScroll[OHS_AMBIENT_EFFECT]->GetScrollPosWZ() );
	}

	//	�Ϲ� ȿ���� ���� ���� �� �ʱ�ȭ
	if (m_pHScroll[OHS_EFFECT])
	{
        m_OptionParam.SetOptionValue( nOptionType::EFFECT_VOLUME, m_pHScroll[OHS_EFFECT]->GetScrollPosWZ() );
	}

	// ������
	if (m_pHScroll[OHS_SYSTEM])
	{
        m_OptionParam.SetOptionValue( nOptionType::SYSTEM_VOLUME, m_pHScroll[OHS_SYSTEM]->GetScrollPosWZ() );
	}

	// ���̺�
	if (m_pHScroll[OHS_SPEAKER])
	{
        m_OptionParam.SetOptionValue( nOptionType::SPEAKER_VOLUME, m_pHScroll[OHS_SPEAKER]->GetScrollPosWZ() );
	}
}

BOOL OptionSoundDialog::Apply( BOOL bCheckApply ,BOOL bProgramInit)
{
	//---------------------------------------------------------------------------------------------
	//		�� �Ʒ��� ���� ���� ����� �۾��� �ֽÿ�.
	//---------------------------------------------------------------------------------------------
	SoundSetting();

    // Copy
	OptionParam tempOption = GAMEOPTION->GetGameOption();

    tempOption.SetOptionValue( nOptionType::USE_NPC_VOICE,      m_OptionParam.GetOptionValue(nOptionType::USE_NPC_VOICE ) );    
    tempOption.SetOptionValue( nOptionType::USE_SYSTEM_VOICE,   m_OptionParam.GetOptionValue(nOptionType::USE_SYSTEM_VOICE ) ); 
                                                                                                          
    tempOption.SetOptionValue( nOptionType::BGM_VOLUME,         m_OptionParam.GetOptionValue(nOptionType::BGM_VOLUME ) );       
    tempOption.SetOptionValue( nOptionType::AMBIENT_VOLUME,     m_OptionParam.GetOptionValue(nOptionType::AMBIENT_VOLUME ) );   
    tempOption.SetOptionValue( nOptionType::EFFECT_VOLUME,      m_OptionParam.GetOptionValue(nOptionType::EFFECT_VOLUME ) );    
    tempOption.SetOptionValue( nOptionType::SYSTEM_VOLUME,      m_OptionParam.GetOptionValue(nOptionType::SYSTEM_VOLUME ) );    
    tempOption.SetOptionValue( nOptionType::SPEAKER_VOLUME,     m_OptionParam.GetOptionValue(nOptionType::SPEAKER_VOLUME ) );   
	
	m_InitOptionParam = tempOption;
	GAMEOPTION->SetGameOption( tempOption );

	if( bProgramInit == FALSE )
	{
		GAMEOPTION->Save();
	}

	return TRUE;
}

void OptionSoundDialog::SoundSetting()
{
	//	���� ���� ����
    Sound::Instance()->SetBGMVolume( m_OptionParam.GetOptionValueFloat( nOptionType::BGM_VOLUME ) );
    g_SoundBGM.SetVolume( m_OptionParam.GetOptionValueFloat( nOptionType::BGM_VOLUME) );

    Sound::Instance()->SetAmbientEffectVolume( m_OptionParam.GetOptionValueFloat( nOptionType::AMBIENT_VOLUME ) );
	if (g_pMap)
	{
        g_pMap->SetAmbientVolume(m_OptionParam.GetOptionValueFloat( nOptionType::AMBIENT_VOLUME) );
	}

    Sound::Instance()->SetEffectVolume( m_OptionParam.GetOptionValueFloat( nOptionType::EFFECT_VOLUME ) );
    Sound::Instance()->SetMasterVolume( m_OptionParam.GetOptionValueFloat( nOptionType::SYSTEM_VOLUME), TRUE );
    Sound::Instance()->SetWaveVolume( m_OptionParam.GetOptionValueFloat( nOptionType::SPEAKER_VOLUME), TRUE );

}
#endif
#pragma warning ( pop )
