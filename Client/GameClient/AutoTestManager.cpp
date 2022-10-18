/*!
*	\file		AutoTestManager.cpp
*	\brief		AutoTestManager class
*	\author		이지연 (kiki@webzen.co.kr)
*	\version	2009/07/02 first release
*/

#include "SunClientPrecompiledHeader.h"
#include "AutoTestManager.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 


#include <io.h>
#include "uiLoginMan/uiLoginMan.h"
#include "CharacterScene.h"
#include <time.h>
#include "ClientVer.h"
#include "GlobalFunc.h"



bool AutoTestManagerEx::LoadTestCase()
{
    SetActive(false);

    TCHAR file_path[MAX_PATH];
    GetCurrentDirectoryA( MAX_PATH, file_path);
    strcat( file_path, "\\autotestex.ini");


    if( _access(file_path, 0) != 0 )
    {
        ////파일이 존재하지 않으면  autotest 모드로 돌릴 수 없음. 그냥 종료시킨다.
        //SaveLog(true, _T("autotest.ini가 존재하지 않습니다. AutoTest 모드를 실행할 수 없습니다."));
        return false;
    }



    TCHAR category_name[1024];
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_AutoLogin.m_Enable = false;

    for( size_t i=0;i!=10;++i)
    {
        wsprintf(category_name, "autologin%d", i );

        UINT IsEnable = GetPrivateProfileInt(category_name, "enable", 0, file_path );
        if( IsEnable == 0 )
            continue;

        m_AutoLogin.m_Enable = IsEnable == 1;

        CHAR temp[1024];
        GetPrivateProfileString(category_name, "testid", 0, temp, 1024, file_path );
        m_AutoLogin.m_Account = temp;
        GetPrivateProfileString(category_name, "testpw", 0, temp, 1024, file_path );
        m_AutoLogin.m_Passward = temp;
        GetPrivateProfileString(category_name, "server_name", 0, temp, 1024, file_path );
        m_AutoLogin.m_ServerName = temp;
        GetPrivateProfileString(category_name, "character_name", 0, temp, 1024, file_path );
        m_AutoLogin.m_CharacterName = temp;


        break;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////


    for( size_t i=0;i!=10;++i)
    {
        wsprintf(category_name, "command%d", i );

        UINT IsEnable = GetPrivateProfileInt(category_name, "enable", 0, file_path );
        if( IsEnable == 0 )
            continue;

        m_Command.m_Enable = IsEnable == 1;

        CHAR temp[1024];
        CHAR command_name[1024];
        for( size_t j=0;j!=10;++j)
        {
            wsprintf(command_name, "command%d", j);

            if( GetPrivateProfileString(category_name, command_name, NULL, temp, 1024, file_path ) != 0 )
            {
                m_Command.m_CommandArray.push_back(temp);
            }
        }

        break;
    }


    if( m_AutoLogin.m_Enable == true )
    {
        SetActive(true);
    }

    return true;
}


void AutoTestManagerEx::RunStartGame()
{
    // 캐릭터의 이름이 설정되어있는 경우 이름으로 캐릭터를 찾아서 접속
    if( g_CharSelectScene.SetSelectCharacterAutoTestByName( m_AutoLogin.m_CharacterName.c_str() ) == FALSE )
    {
        g_CharSelectScene.SetSelectCharacterAutoTest(0);
    }
    g_CharSelectScene.SendCharSelectPacket();
}

void AutoTestManagerEx::RunLogin()
{
    uiLoginMan* pUILoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);		
    if(pUILoginMan)
    {
        pUILoginMan->SetIDPW4AutoTest(m_AutoLogin.m_Account.c_str(), m_AutoLogin.m_Passward.c_str() );
        pUILoginMan->SEND_AUTH_ID_PASSWORD_CHECK_SYN();
    }
}

void AutoTestManagerEx::RunSelectServer()
{
    uiLoginMan* pUILoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);		
    bool bSelectServer = false;
    if(pUILoginMan)
    {
        // 서버명이 설정되어있는경우 서버이름으로 찾아서 접속
        if( AuthSetverData* pAuthSetverData = pUILoginMan->GetServerDataByServerName( m_AutoLogin.m_ServerName.c_str() ) )
        {
#ifdef _DEV_VER//_DH_LOGIN_LOG_ID_AND_CHANNEL
            TCHAR szTitle[MAX_PATH];
            TCHAR temp_string[MAX_PATH];
            GetWindowText(g_hWndMain, temp_string, sizeof(szTitle));
            sprintf(szTitle, "%s@%s", strtok(temp_string, "@"), pAuthSetverData->m_ServerName.c_str());
            ::SetWindowText(g_hWndMain, szTitle);
#endif//_DH_LOGIN_LOG_ID_AND_CHANNEL

            pUILoginMan->SEND_AUTH_SERVER_SELECT_SYN(pAuthSetverData);
            bSelectServer = true;
        }
    }
}

void AutoTestManagerEx::RunAutoCommand()
{
    for( size_t i=0;i!=m_Command.m_CommandArray.size();++i)
    {
        CHAR temp[1024];
        strcpy(temp, m_Command.m_CommandArray[i].c_str());
        GlobalFunc::ProcessChatMessage(temp);
    }
}

#endif