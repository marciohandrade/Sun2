//------------------------------------------------------------------------------
//  ServerSelectionDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "BattleScene.h"
#include "InterfaceManager.h"
#include ".\ServerSelectionDialog.h"
#include "LoginScene.h"

WzID ServerSelectionDialog::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("B000"),
        StrToWzID("L000"),
};

//------------------------------------------------------------------------------
/**
*/
ServerSelectionDialog::ServerSelectionDialog()
{
    ParseServerListFile();
    m_curServerCounts = 0;
}

//------------------------------------------------------------------------------
/**
*/
ServerSelectionDialog::~ServerSelectionDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
void
ServerSelectionDialog::Init( CDrawBase * pDrawBase )
{
    MoveWindowWZ( -1.0f, -1.0f );
}

//------------------------------------------------------------------------------
/**
*/
void
ServerSelectionDialog::showWindow( BOOL val )
{
    if (val)
    {
        MoveWindowWZ( -1.0f, -1.0f );
    }
    else
    {
        _asm nop;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ServerSelectionDialog::ServerSelected()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( DIALOG_SERVER_LIST );
    int CurSelection = pLCtrl->GetCurSel();

    if (CurSelection < 0)
    {
        // 선택해서 반전시킨 목록이 없다.
		//	목록에서 하나를 고르십시오
        WzColor color=0xFFFFFFFF;
		TCHAR	szMessage[MAX_MESSAGE_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_SELECT_SERVER_LIST, szMessage, MAX_MESSAGE_LENGTH );
        g_InterfaceManager.ShowMessageBox( szMessage, 10000, &color );
        return;
    }

    _ServerInfo *psvinfo = NULL;

    for (UINT i=0; i<g_LoginScene.GetServerCounts(); ++i)
        if (this->vSvLists[i].curPos == CurSelection)
        {
            psvinfo = &this->vSvLists[i];
            break;
        }

    if (psvinfo && (CurSelection < m_curServerCounts))
    {
        //
        // --> 서버 선택
        //
        MSG_C2S_ASK_SVRSELECT           SendPacket;
        SendPacket.m_byCategory         = CL_AUTH;
        SendPacket.m_byProtocol         = CL_AUTH_C2S_ASK_SVRSELECT;
        SendPacket.bySelectIndex        = psvinfo->altIndex;		//선택한 서버월드의 인덱스

        g_LoginScene.checkSendPacket(sizeof(MSG_C2S_ASK_SVRSELECT), (char *)&SendPacket);

        g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_SERVER_SELECTION, FALSE );
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ServerSelectionDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DIALOG_SERVER_SELECTED:
                {
                    this->ServerSelected();
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONDCLICK:
        {
            this->ServerSelected();
        }
        break;

    //case RT_MSG_EDIT_ENTERKEYDOWN:
    //    {
    //        CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( DIALOG_SERVER_LIST );
    //        int CurSelection = pLCtrl->GetCurSel();

    //        unsigned int i;

    //        for (i=0; i<g_LoginScene.GetServerCounts(); ++i)
    //            if (this->vSvLists[i].curPos == CurSelection)
    //                break;

    //        if (i<g_LoginScene.GetServerCounts())
    //        {
    //            //
    //            // --> 서버 선택
    //            //
    //            MSG_C2S_ASK_SVRSELECT           SendPacket;
    //            SendPacket.m_byCategory         = CL_AUTH;
    //            SendPacket.m_byProtocol         = CL_AUTH_C2S_ASK_SVRSELECT;
    //            SendPacket.bySelectIndex        = this->vSvLists[i].altIndex;		//선택한 서버월드의 인덱스

    //            g_LoginScene.checkSendPacket(sizeof(MSG_C2S_ASK_SVRSELECT), (char *)&SendPacket);

    //            g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_SERVER_SELECTION, FALSE );
    //        }
    //    }
    //    break;
    }
}

//------------------------------------------------------------------------------
/**
*/
const char serverListFileName[] = "GameServerList_Dev.ini";
const char serverListFileName_public[] = "GameServerList_Public.ini";

void
ServerSelectionDialog::ParseServerListFile()
{
    DWORD dwError = GetCurrentDirectory( 512, m_szFullPath );
    if(0 == dwError)
    {
        ASSERT(0);
    }
    strcat(m_szFullPath, "\\");

    if (GENERALPARAM->GetPublic())
    {
        strcat(m_szFullPath, serverListFileName_public); // 주어지는 파일 이름에 대한 풀 패스를 설정함
    }
    else
    {
        strcat(m_szFullPath, serverListFileName); // 주어지는 파일 이름에 대한 풀 패스를 설정함
    }

    FILE *fp;

    fp = fopen (m_szFullPath, "r");
    if (fp == NULL)
    {
		ASSERT( !"서버 목록파일 읽어오기 실패입니다" );
        return;
    }

    unsigned int i;
    char buf[1024];

    while (!feof(fp))
    {
        fgets(buf, 100, fp);

        if (!strncmp (buf, "[GROUP]", 5))
        {
            while (1)
            {
                fgets(buf, 100, fp);

                if (buf[0] == ' ' || buf[0] == '\n')
                    break;

                _ServerGroup svGroup;
                std::string strToken;
                std::string strConv;
                strToken = strtok( buf, "=");
                for (i=0; i<strToken.size(); ++i)
                    if (strToken[i] != ' ')
                        strConv += strToken[i];
                svGroup.Index = atoi (strConv.c_str());

                strToken.clear();
                strConv.clear();
                strToken = strtok( NULL, "\n");
                for (i=0; i<strToken.size(); ++i)
                    if (strToken[i] != ' ')
                        strConv += strToken[i];
                svGroup.strName = strConv;
                vSvGroups.push_back(svGroup);
            }
        }

        if (vSvGroups.size())
        {
            std::string compName = "[";
            compName += vSvGroups[0].strName.c_str();
            compName += "]";
            int compLen = strlen(compName.c_str());
            if (!strncmp (buf, compName.c_str(), compLen))
            {
                while (1)
                {
                    fgets(buf, 100, fp);

                    if (buf[0] == ' ' || buf[0] == '\n')
                        break;

                    _ServerInfo svInfo;
                    std::string strToken;
                    std::string strConv;
                    strToken = strtok( buf, "=");
                    for (i=0; i<strToken.size(); ++i)
                        if (strToken[i] != ' ')
                            strConv += strToken[i];
                    svInfo.groupNo = 0;
                    svInfo.Index = atoi (strConv.c_str());

                    strToken.clear();
                    strConv.clear();
                    strToken = strtok( NULL, "\n");
                    for (i=0; i<strToken.size(); ++i)
                        if (strToken[i] != ' ')
                            strConv += strToken[i];
                    svInfo.strName = strConv;
                    this->vSvLists.push_back(svInfo);
                }
            }
        }

        if (vSvLists.size())
        {
            std::string compName = "[GAME_INFO]";
            int compLen = strlen(compName.c_str());
            if (!strncmp (buf, compName.c_str(), compLen))
            {
                while (1)
                {
                    fgets(buf, 100, fp);

                    if (buf[0] == ' ' || buf[0] == '\n')
                        break;

                    if (buf[0] == ';')
                        continue;

                    std::string strToken;
                    std::string strConv;
                    strToken = strtok( buf, "=");
                    for (i=0; i<strToken.size(); ++i)
                        if (strToken[i] != ' ')
                            strConv += strToken[i];
                    int index = atoi (strConv.c_str());

                    strToken.clear();
                    strConv.clear();
                    strToken = strtok( NULL, "\n");
                    for (i=0; i<strToken.size(); ++i)
                        if (strToken[i] != ' ')
                            strConv += strToken[i];
                    int altIndex = atoi (strConv.c_str());

                    for (i=0; i<this->vSvLists.size(); ++i)
                        if (vSvLists[i].Index == index)
                            vSvLists[i].altIndex = altIndex;
                }
            }
        }
    }

    fclose (fp);
}

//------------------------------------------------------------------------------
/**
*/
void
ServerSelectionDialog::update()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( DIALOG_SERVER_LIST );
    pLCtrl->DeleteItemAll();
    pLCtrl->SetBeginDrawIndex(0);
    char buf[4096];

    //sprintf (buf, "서버 수: %d", g_LoginScene.GetServerCounts());
    //pLCtrl->SetItemText(pLCtrl->InsertItemBack(),buf);

    WORD* pwServerStatusLists = g_LoginScene.GetServerStatusLists();
    int sell = 0;
    m_curServerCounts = 0;
    for (unsigned int i=0; i<g_LoginScene.GetServerCounts(); ++i)
    {
        int altIndex = this->vSvLists[i].altIndex;
        if (this->vSvLists[i].altIndex == -1)
            continue;

        int svStat = pwServerStatusLists[i];

        pLCtrl->SetColorText(WzColor_RGBA( 0, 0, 0, 255 ));

        if (svStat != 65535)
        {
            this->vSvLists[i].curPos = sell++;
            sprintf (buf, "(%d) No. %02d | %s", this->vSvLists[i].curPos,
                                                this->vSvLists[i].altIndex,
                                                (char *)this->vSvLists[i].strName.c_str());
            pLCtrl->SetItemText(pLCtrl->InsertItemBack(),buf);
            m_curServerCounts++;
        }
    }
    pLCtrl->SetUnuseSelBar(TRUE);    
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE
ServerSelectionDialog::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ *
ServerSelectionDialog::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}
