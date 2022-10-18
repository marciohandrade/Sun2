//------------------------------------------------------------------------------
//  CharSelectDialog.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "charselectdialog.h"
#include "InterfaceManager.h"
#include "GameFramework.h"
#include "Loadscene.h"
#include "CharacterScene.h"
#include "player.h"
#include "Application.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "SolarDateTime.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiCharacterSelectList/uiCharacterSelectList.h"
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "CreateCharCustomDialog.h"
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

//------------------------------------------------------------------------------
/**
*/
void _CallBack_Character_Delelte(bool bYes, const char * pString )
{
	g_Input.Reset();

	if (bYes)
	{	
		//Todo:캐릭터 삭제 패킷 보내자.
		if(g_CharSelectScene.SendDeleteCharPacket(pString)==FALSE)
		{
			TCHAR	szMessage[MAX_MESSAGE_LENGTH];
			//	삭제할 캐릭터가 없습니다
			g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_DELETE_CHARACTER, szMessage, MAX_MESSAGE_LENGTH );
			g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
		}
	}

	g_CharSelectScene.SetChangeSelectedCharacter(TRUE);
}

//------------------------------------------------------------------------------
/**
*/
void _CallBack_Character_Delelte_Confrim(bool bYes )
{
	g_Input.Reset();

	if (bYes)
	{	
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		const int	c_iDeleteChar = 70265;
		g_InterfaceManager.GetInterfaceString( c_iDeleteChar, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
        {
			pSystemMan->Sys_ConfirmString(szMessage, _CallBack_Character_Delelte);
        }
	}
	else
	{
		g_CharSelectScene.SetChangeSelectedCharacter(TRUE);
	}
	g_CharSelectScene.m_bPickCharLock=FALSE;
}

void _CallBack_Cancle_RemainDelChar(bool bYes)
{
	g_Input.Reset();
	if( bYes)
	{
		// 캐릭터 삭제취소 패킷 날리기.
		g_CharSelectScene.SEND_CHARINFO_RECOVER_SYN();
	}

	g_CharSelectScene.SetChangeSelectedCharacter(TRUE);

}

void _Callback_RemailDelChar_Delete(bool bYes)
{
	g_Input.Reset();
	if( bYes)
	{
		g_CharSelectScene.SetChangeSelectedCharacter(TRUE);
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

		//86004 캐릭터 삭제를 취소하시겠습니까?
		g_InterfaceManager.GetInterfaceString(86004 , szMessage ,INTERFACE_STRING_LENGTH);
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Confirm( szMessage, _CallBack_Cancle_RemainDelChar);
	}
	g_CharSelectScene.m_bPickCharLock=FALSE;
}

WzID CharSelectDialog::m_wzId[CharSelect_Max] = 
{
    StrToWzID("I001"),
    StrToWzID("I002"),
    StrToWzID("I003"),
    StrToWzID("I004"),
#ifdef	_K2P_ADD_MOVE_TO_SERVER_BUTTON_001020
    StrToWzID("I005"),
#endif
    StrToWzID("I000"),
};

//------------------------------------------------------------------------------
/**
*/
CharSelectDialog::CharSelectDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
CharSelectDialog::~CharSelectDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectDialog::Init( CDrawBase * pDrawBase )
{

	RECT rcDlgSize;
	this->GetDialogWZ()->GetDlgSize(&rcDlgSize);

	POINT pt = g_pApplication->Get2DModeMapping();

	RECT rcInterface;
	rcInterface.left = rcInterface.top = 0;
	rcInterface.right = pt.x;
	rcInterface.bottom = pt.y;
	
	int x = (rcInterface.right / 2) - (rcDlgSize.right / 2);
	int y = (rcInterface.bottom / 2) - (rcDlgSize.bottom / 2);	

	MoveWindowWZ( (float)x, (float)y );
	
    CControlWZ* control = getControl(CharSelect_UnDelete);
    if (control)
    {
        control->ShowWindowWZ(WZ_HIDE);
    }

	m_bSendPacket = 0;
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch (pMessage->eResultMsg)
	{
	case RT_MSG_RBUTTONCLICK:
		_asm nop;
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			DWORD dwPacketStatus = GameFramework::GetCurrentScene()->GetpacketStatus();

			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case CharSelectDialog::CharSelect_Start:
				{
					OnSelectStart();
    			}
				break;

            case CharSelectDialog::CharSelect_Create:
                {	
                    OnCreateCharacter();
                }
                break;
                
            case CharSelectDialog::CharSelect_Delete:
            case CharSelectDialog::CharSelect_UnDelete:
                {
                    OnDeleteCharacter();
                }
                break;

            case CharSelectDialog::CharSelect_Exit:
                {
                    GlobalFunc::LogOutGame();
                }
                break;
#ifdef	_K2P_ADD_MOVE_TO_SERVER_BUTTON_001020
			case CharSelectDialog::CharSelect_ServerList:
				{
					GlobalFunc::GoToServerList();
				}
				break;
#endif
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


//------------------------------------------------------------------------------
/**
*/
VOID CharSelectDialog::OnUpdateSolarDialog()
{
    if ((g_Input.GetState(DIK_RETURN, KS_UP) != FALSE) || 
        (g_Input.GetState(DIK_NUMPADENTER, KS_UP) != FALSE))
    {
        OnSelectStart();
    }
}



//----------------------------------------------------------------------------
/**
*/
VOID	
CharSelectDialog::OnShowWindow(BOOL val)
{
    SolarDialog::OnShowWindow(val);
	if (val)
	{
		// 게임데이터를 초기화해준다.
		GlobalFunc::InitGameInfo();
	}

    uiBase* character_list = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
    if (character_list != NULL)
    {
        character_list->ShowInterface(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectDialog::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/**
*/
void CharSelectDialog::OnSelectStart()
{
    g_Input.Reset();
    WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );

    if( g_CharSelectScene.SendCharSelectPacket() == FALSE )
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

        //	선택된 캐릭터가 없습니다
        g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_CHARACTER, szMessage, INTERFACE_STRING_LENGTH );
        g_InterfaceManager.ShowMessageBox( szMessage, 3000, &Color, 0 );
    }
}

//------------------------------------------------------------------------------
/**
*/
void CharSelectDialog::OnDeleteCharacter()
{
    g_Input.Reset();
    //선택된 캐릭터 가 없거나,캐릭터 선택 패킷을 보냈을땐 처리 안함 
    if ((g_CharSelectScene.GetSelectCharOId() == INVALID_OBJECT_KEY) || 
        BIT_CHECK(g_CharSelectScene.GetpacketStatus(),CharacterScene::PACKET_STATUS_CHARACTERSCENE_SELCHAR))
    {
        return;
    }
    Player * pPlayer =(Player *)g_ObjectManager.GetObject(g_CharSelectScene.GetCurrentSelectedPlayerKey());
    if( pPlayer == NULL )
        return;

    uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
    if( pSystemMan == NULL)
        return;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    // 캐릭 삭제 대기중이다.
    if( pPlayer->m_DelChk > 0) 
    {
        // 19888 캐릭터삭제까지 00일 00시간 남았습니다.
        int RemainTotalTime = pPlayer->GetCharDelRemainTime();
        int day = 0 , hour = 0 , minute = 0;

        int remainder = RemainTotalTime;
        // 1일 보다 큰가
        if( RemainTotalTime >= 86400)
        {
            day = RemainTotalTime / 86400;
            remainder = RemainTotalTime % 86400;
        }
        // 1시간 보다 큰가
        if( RemainTotalTime >= 3600 )
        {
            hour = remainder / 3600;
            remainder = RemainTotalTime % 3600;
        }
        // 분 단위 인가
        minute = remainder / 60;


        TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(86002 , szMessage , INTERFACE_STRING_LENGTH);
        Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , szMessage , day , hour );

        pSystemMan->Sys_Confirm(szResult , _Callback_RemailDelChar_Delete, NULL, NULL, 100, 86003);
    }
    else
    {
        // 일반적인 캐릭터 삭제 처리
        DWORD dwLv = g_CharSelectScene.GetSelectCharLevel();
        DWORD level_for_wait = PROGRAMPARAM->delete_level_for_wait();
        int	c_iDeleteChar = 5032;
        if (dwLv >= level_for_wait)
        {
            c_iDeleteChar = 5033;
        }
        g_InterfaceManager.GetInterfaceString(c_iDeleteChar, szMessage, INTERFACE_STRING_LENGTH);

        pSystemMan->Sys_Confirm(szMessage, _CallBack_Character_Delelte_Confrim);
    }

    g_CharSelectScene.SetChangeSelectedCharacter(FALSE);
    g_CharSelectScene.m_bPickCharLock=TRUE;
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ * CharSelectDialog::getControl( POSTYPE AtPos )
{
    assert( AtPos < CharSelect_Max );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE	CharSelectDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CharSelect_Max ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return CharSelect_Max;
}

void CharSelectDialog::OnCreateCharacter()
{
    g_Input.Reset();

    if( g_CharSelectScene.GetNumCharacters() >= MAX_CHARACTER_LIST_NUM )
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH];

        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_NUMBER_FULL, szMessage, INTERFACE_STRING_LENGTH );
        g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
        return;
    }

#ifdef _NHN_USA_CHANNELING
    GlobalFunc::SendHanReportPacket(301);//캐릭생성(301)
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)

    g_CharSelectScene.LoadCreateCharacterRoom();
    ShowDialog(FALSE);

    uiBase* ui_dialog = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
    if (ui_dialog != NULL)
    {
        ui_dialog->ShowInterface(FALSE);
    }
    g_InterfaceManager.LockDlgs(0);



    g_CharSelectScene.MakeCinematicCharacter();
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CreateCharCustomDialog* createcharcustomdialog_ptr = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);
    if (createcharcustomdialog_ptr != NULL)
    {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        createcharcustomdialog_ptr->OnClickRace(eCHAR_WITCHBLADE);
#else
        createcharcustomdialog_ptr->OnClickRace(eCHAR_HELLROID);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void CharSelectDialog::EnableButton(bool val, CHARSELECT_POS type)
{
    CControlWZ* control_ptr = getControl(type);

    if (control_ptr == NULL)
    {
        return;
    }

    if (val)
    {
        control_ptr->EnableWindowWZ(true);
    }
    else
    {
        control_ptr->EnableWindowWZ(false);
    }
}

void CharSelectDialog::ShowDeleteButton(bool val)
{
    CControlWZ* delete_control_ptr = getControl(CharSelect_Delete);
    CControlWZ* undelete_control_ptr = getControl(CharSelect_UnDelete);

    if (delete_control_ptr == NULL || undelete_control_ptr == NULL)
    {
        return;
    }

    if (val)
    {
        delete_control_ptr->ShowWindowWZ(WZ_SHOW);
        undelete_control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        delete_control_ptr->ShowWindowWZ(WZ_HIDE);
        undelete_control_ptr->ShowWindowWZ(WZ_SHOW);
    }
}
