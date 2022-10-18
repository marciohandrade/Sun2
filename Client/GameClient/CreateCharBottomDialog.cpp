#include "SunClientPrecompiledHeader.h"
#include "createcharbottomdialog.h"
#include "createcharleftdialog.h"
#include "InterfaceManager.h"
#include "CharacterScene.h"
#include "Nstring.h"
#include "GlobalFunc.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "CreateCharCustomDialog.h"
#include "PlayerDef.h"

CreateCharBottomDialog::CreateCharBottomDialog(InterfaceManager* pUIman)
	:uiBase(pUIman)
{
}

CreateCharBottomDialog::~CreateCharBottomDialog(void)
{
}


WzID CreateCharBottomDialog::m_wzId[CCB_MAX] = 
{
    StrToWzID("I005"),
    StrToWzID("I006"),
};


VOID CreateCharBottomDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
}


VOID CreateCharBottomDialog::Release()
{

}

//------------------------------------------------------------------------------ 
void CreateCharBottomDialog::OnShowWindow(BOOL val)
{
    uiBase::OnShowWindow(val);
}
//------------------------------------------------------------------------------ 
VOID CreateCharBottomDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case CCB_CREATE:
				OnClickCreate();				
				break;
			case CCB_CANCEL:
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                g_CharSelectScene.SetCreateCharacterMode(FALSE);
#else
				g_CharSelectScene.OnCancelCreateCharacter();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
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

CControlWZ * CreateCharBottomDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < CCB_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE	CreateCharBottomDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CCB_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CCB_MAX;

}

BOOL    CreateCharBottomDialog::OnClickCreate()
{
    CreateCharCustomDialog* pCustomDlg = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);

	TCHAR	szMessage[MAX_MESSAGE_LENGTH];
    DWORD dwClassCode = 0;
    bool is_selected = false;

    ClassCodeEx class_code_ex;
    class_code_ex.class_type = pCustomDlg->GetSelectedCharacterClass();
    class_code_ex.gender = pCustomDlg->GetSelectedCharacterGender();
    dwClassCode = class_code_ex.class_code;
    is_selected = (class_code_ex.class_type != eCHAR_NONE) ? true : false;
    if (is_selected == false)
    {
		//	선택된 캐릭터 없음
		g_InterfaceManager.GetInterfaceString(eST_NOT_SELECTED_CHARACTER, szMessage, MAX_MESSAGE_LENGTH);
        g_InterfaceManager.ShowMessageBox(szMessage, 3000, NULL, 0);
        return FALSE;
    }

	TCHAR name[256] = {0,};

	ZeroMemory(name,sizeof(TCHAR));

    if (pCustomDlg->IsVisible())
    {
        pCustomDlg->GetCharName(name);
    }

	int iLength =_tcslen(name);

    if (iLength == 0)
    {
		//	캐릭터 이름을 입력하세요
		g_InterfaceManager.GetInterfaceString( eST_INPUT_CHARACTER_NAME, szMessage, MAX_MESSAGE_LENGTH );
        g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
        return FALSE;
    }
	else if (iLength < 4)
	{
		// 이름은 영문 4자 한글 2자 이상 입력해주세요
		g_InterfaceManager.GetInterfaceString(eST_NAME_OVER2_ENG_OVER_4 , szMessage ,INTERFACE_STRING_LENGTH );
		g_InterfaceManager.ShowMessageBox(szMessage, 3000, NULL, 0 );
		return FALSE;
	}
	else if (iLength > MAX_CHARNAME_LENGTH)
	{
		//	이름은 영문 16자 한글 8자 이내로 입력해 주십시오.
		g_InterfaceManager.GetInterfaceString( eST_CHECK_INPUT_NAME_LENGTH, szMessage, MAX_MESSAGE_LENGTH );
		g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
        return FALSE;
	}

	if(g_slangNameFilter.IsHaveReplace(name))
	{
		//	금지 단어 사용
		g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
		g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
		return FALSE;
	}

#if defined(__KR) || defined(_KOREA)
		if(g_slangNameFilter.CheckIncludeString(name))
		{
			//	금지 단어 사용
			g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
			g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
			return FALSE;
		}
#	else	//#if defined(__KR) || defined(_KOREA)
		if( GlobalFunc::CheckIncludeStringOnAbroad(name ) )
		{
			//	금지 단어 사용
			g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
			g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
			return FALSE;
		}
#	endif	// #if defined(__KR) || defined(_KOREA)


    //패킷쏘자.
    if( GENERALPARAM->IsNet() )
    {
        g_CharSelectScene.SendMakeCharPacket(dwClassCode, name);
    }
	else
	{
		g_CharSelectScene.SetCreateCharacterMode(FALSE);
	}

	return TRUE;

}
