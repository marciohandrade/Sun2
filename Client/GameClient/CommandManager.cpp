

#include "SunClientPrecompiledHeader.h"
#include "Chatdialog.h"
#include "CommandManager.h"
#include "Hero.h"
#include "HeroData.h"
#include "CooltimeManager.h"
#include "InterfaceManager.h"
#include "ResourceManager.h"
#include "LoadScene.h"
#include "MapInfoParser.h"
#include "../../Utility/Nstring.h"
#include "Camera.h"
#include "HeroActionInput.h"
#include "EquipmentContainer.h"
#include <SCItemSlot.h>
#include "StyleInfoParser.h"
#include "SkillInfoParser.h"
#include "ItemInfoParser.h"
#include "ObjectManager.h"
#include "gamefunc.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"

#include "uiGuildMan/uiGuildMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "map.h"
#include "globalfunc.h"
#include "BattleScene.h"
#include "HeroActionInput.h"

#include "RadarDialog.h"
#include "RadarPlusDialog.h"
#include "RadarFullDialog.h"
#include "ObjectManager.h"
#include "TargetDialog.h"
#include "uiTargetMan/uiTargetMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"

#include "uiVendorMan/uiVendorMan_def.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiChatMan/uiChatMan_def.h"
#include "uiInventoryMan/uiInventoryDef.h"
#include "InventoryDialog.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
    #include "GuildSystem/GuildSystemData.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "message.h"
#include "Sound.h"

#include "Application.h"
#include "cLua.h"

#include "GraphicResourceList.h"
#include "NPCInfoParser.h"
#include "Const.h"

#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "uiMissionObject/uiMissionObject.h"

#ifdef _DEV_VER
#include "ContinentMapDlg.h"//_DH_FULL_MAP_PORTAL
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"

#include "uiTargetAggro/uiTargetAggro.h"
#include "uiGMWindowMan/uiGMWindowMan.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

#if WZENGINEVER >= 370
#include "LogManager.h"
#endif // WZENGINEVER >= 370

#include "SkillMain.h"
#include "uiOptionMan.h"

#include "uiOptionMenuMan/uiOptionMenuMan.h"
#endif//_DEV_VER

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiTextureViewerMan/uiTextureViewerMan.h"
#endif
  #ifdef _NA_007086_20140318_MONSTERBOOK
    #include "uiMonsterBookMan/uiMonsterBookMan.h"
  #endif //_NA_007086_20140318_MONSTERBOOK
  #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    #include "uiAttendanceCheck/uiAttendanceCheckDialog.h"
  #endif  _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif

//---------------------------------------------------------------------------------------
//
//		일반 명령어
//
//---------------------------------------------------------------------------------------

CommandManager			g_CommandManager;


CommandManager::CommandManager( void )
{
	
}

CommandManager::~CommandManager( void )
{
}


//---------------------------------------------------------------------------------------
//
//		실제 명령어 처리하는 부분으로
//		리턴값이 TRUE 이면 성공적으로 명령어의 기능을 수행한 것이고,
//		리턴값이 FALSE 이면 명령어가 아닌 것으로 간주하여 이 함수를 호출한 곳에서
//		채팅 명령어를 처리하는 곳으로 넘어간다.
//		따라서 입력받은 스트링이 명령어라면 기능 수행이 실패던 성공이던
//		리턴값은 TRUE 가 되어야 한다.
//
//---------------------------------------------------------------------------------------

BOOL CommandManager::isChatCommand(TCHAR firstString)
{
	if(firstString == _T('/') || firstString  == _T('@') || firstString == _T('!') ||
		firstString == _T('#') || firstString == _T('$') || firstString == _T('&') ||
		firstString == _T('"'))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD CommandManager::ParseCommand(TCHAR * pszString)
{
	if (g_pHero == NULL)
	{
		return TRUE;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szCommand[INTERFACE_STRING_LENGTH] = {0,};

	ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	int iEmoCommandStart = eGAME_COMMAND_EMOTICON_START_INDEX;//이모션 커맨드 시작.
	int iEmoCommandcnt = NUM_GAME_COMMAND_COUNT;//이모션 커맨드 엔드.
	int iCommandcnt = eGAME_COMMAND_ACTION_END_CODE;//일반 커맨드 엔드.
	int iCommandLen = 0;
	int ChatStringLen = lstrlen(pszString);
	TCHAR szTemp[MAX_MESSAGE_LENGTH];
	TCHAR szTemp2[MAX_MESSAGE_LENGTH];
	TCHAR szTemp3[MAX_MESSAGE_LENGTH];


	if (isChatCommand(pszString[0]))
	{
        for( int k = eCHAT_COMMAND_START_INDEX; k < eCHAT_COMMAND_MAX; ++k)
        {
            g_InterfaceManager.GetInterfaceString( k, szCommand, INTERFACE_STRING_LENGTH );
            iCommandLen = _tcslen( szCommand );

            if (iCommandLen > ChatStringLen )
                continue;

            int iCmdLen = _tcslen(szCommand);
            int iTotalLen = _tcslen(pszString);

            TCHAR	szString[MAX_MESSAGE_LENGTH] = {0,};
            TCHAR	szString2[MAX_MESSAGE_LENGTH] = {0,};

            int iMsgLen = 0;

            if(iCmdLen == iTotalLen)
            {
                iMsgLen = 0;
            }
            else if(iTotalLen > iCmdLen)
            {
                StrnCopy(szString, pszString+iCmdLen+1, iTotalLen-iCmdLen-1);
                iMsgLen = _tcslen(szString);
            }
            else
            {
                continue;
            }

            _stscanf(pszString,_T("%s %s %s"),szTemp, szTemp2, szTemp3);

            _strlwr(szTemp);
            _strlwr(szCommand);

            if ( CSTR_EQUAL == CompareString(
                LOCALE_USER_DEFAULT,
                NULL, 
                szTemp,
                _tcslen(szTemp), 
                szCommand,
                iCommandLen) )
            {
                switch( k ) 
                {
                case eCHAT_COMMAND_NORMAL_HOTKEY2:
                case eCHAT_COMMAND_NORMAL_HOTKEY3:	
                case eCHAT_COMMAND_NORMAL:
                    {
                        if(pDlg)
                        {
                            pDlg->OnNoramlChat();

                            if(iMsgLen)
                            {
                                pDlg->SendChatMsg( szString );
                            }
                        }

                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_SHOUT_HOTKEY2:
                case eCHAT_COMMAND_SHOUT_HOTKEY3:
                case eCHAT_COMMAND_SHOUT:
                    {
                        if(pDlg)
                        {
                            if(iMsgLen)
                            {
                                pDlg->SendShoutMsg( szString );
                            }
                        }

                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_PARTY_HOTKEY2:
                case eCHAT_COMMAND_PARTY_HOTKEY3:
                case eCHAT_COMMAND_PARTY:
                    {
                        if(pDlg)
                        {
                            pDlg->OnPartyChat();


                            if(iMsgLen)
                            {
                                uiPartyMan* partyMan =
                                    static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));


                                if (partyMan && partyMan->IsParty())
                                {
                                    pDlg->SendChatPartyMsg(szString);
                                }
                                else
                                {
                                    // 파티 존재하지 않음
                                    g_InterfaceManager.GetInterfaceString( eST_PARTY_NOT_EXIST, szString, MAX_MESSAGE_LENGTH );
                                    pDlg->AddString( szString );
                                }

                            }
                        }

                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_GUILD_HOTKEY2:
                case eCHAT_COMMAND_GUILD:
                    {
                        if(pDlg)
                        {
                            pDlg->OnGuildChat();
                            if(iMsgLen)
                            {
                                bool join_guild = false;
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                                join_guild = GuildSystem::GetGuildSystemData().is_recv_guild_info();
#else
                                uiGuildMan* guildMan = static_cast<uiGuildMan*>(
                                    g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
                                if (guildMan != NULL)
                                {
                                    join_guild = guildMan->IsGuildInfo();
                                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

                                if (join_guild == true)
                                {
                                    // 길드가 존재한다.
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                                    GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
#else
                                    stGUILD_INFO& guild_info = static_cast<stGUILD_INFO>(guildMan->GetGuildInfo());
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                                    pDlg->SendGuildMsg( szString, guild_info.m_GuildGuid );
                                }
                                else
                                {
                                    // 길드가 존재하지 않다면
                                    g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_GUILD, szString, MAX_MESSAGE_LENGTH );
                                    pDlg->AddString( szString );
                                }
                            }
                        }

                        return TRUE;
                    }	
                    break;

                case eCHAT_COMMAND_CHANNEL_HOTKEY2:
                case eCHAT_COMMAND_CHANNEL_HOTKEY3:	
                case eCHAT_COMMAND_CHANNEL_CHAT:
                    {
                        pDlg->OnChannelChat();
                        if(iMsgLen)
                        {
                            pDlg->SendChatChannelMsg(szString);
                        }
                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_WHISPER:
                    {
                        if(iMsgLen)
                        {
                            if(_stscanf(szString,_T("%s"),szString2))
                            {
                                int iCharNameLen = _tcslen(szString2);
                                TCHAR szSendMsg[MAX_MESSAGE_LENGTH] ={0,};
                                StrnCopy(szSendMsg, szString+iCharNameLen+1, iMsgLen-iCharNameLen+1);

                                if(pDlg)
                                    pDlg->SendWhisperMsg(szSendMsg, szString2);
                            }
                        }
                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_GUILD_NOTICE:	// 길드 공지
                    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        // [GUILD_RENEWAL]
#else
                        if(iMsgLen)
                        {
                            uiGuildMan * guildMan =
                                static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
                            assert(guildMan);

                            if (guildMan)
                            {
                                guildMan->Notice(szString);
                            }
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_CHANNEL_JOIN_ENG:
                case eCHAT_COMMAND_CHANNEL_JOIN:	
                    {
                        //방이 개설 되어 있다.
                        //방이 개설 되어 있지않다. 개설 해야한다.

                        ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//( ChatDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
                        if ( pDlg )
                        {
                            pDlg->SendChatChannelCreate( szString );
                        }
                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_CHANNEL_EXIT_ENG:
                case eCHAT_COMMAND_CHANNEL_EXIT:
                    {
                        MSG_CW_CHAT_CHANNEL_OUT_SYN  Packet;
                        GlobalFunc::SendPacket(CI_CHATSERVERIDX,&Packet,sizeof(Packet));

                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_CHANNEL_INVITE_ENG:
                case eCHAT_COMMAND_CHANNEL_INVITE:
                    {

                        if(iMsgLen)
                        {
                            ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//( ChatDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
                            if ( pDlg )
                            {
                                pDlg->SendChatChannelInvite( szString );
                            }
                        }
                        return TRUE;
                    }
                    break;
                case eCHAT_COMMAND_CHANNEL_INFO_REQ:
                    {
                        pDlg->SendChatChannelInfoREQ();
                        return TRUE;
                    }
                    break;

                case eCHAT_COMMAND_TARGET:
                    {
                        TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
                        if (target_dialog != NULL)
                        {
                            target_dialog->SetTargetName(szString);
                        }
                        return TRUE;
                    }
                    break;

                default:
                    break;
                }
            }
        }

        int count = iCommandcnt;
        for( int i = eGAME_COMMAND_ACTION_START_INDEX ; i < count ; ++i )
        {
            g_InterfaceManager.GetInterfaceString( i, szCommand, INTERFACE_STRING_LENGTH );
            iCommandLen = _tcslen( szCommand );

            if (iCommandLen > ChatStringLen )
                continue;


            TCHAR szParameter[512];
            TCHAR * pszPos = NULL;	// 파라미터를 이어서 찾을 경우 사용
            if ( CSTR_EQUAL == CompareString(
                LOCALE_USER_DEFAULT,
                NULL, 
                pszString,
                iCommandLen, 
                szCommand,
                iCommandLen) )
            {
                switch( i ) 
                {

                    //---------------------------------------------------------------------------------------------
                    //		게임 명령어
                    //---------------------------------------------------------------------------------------------

                    //ck: 거래
                case eGAME_COMMAND_TRADE:
                    {
                        if (GlobalFunc::IsSSQScene())
                        {
                            return TRUE;
                        }

                        if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
                        {
                            //5375	카오 상태에서는 이용이 가능하지 않습니다.
                            g_InterfaceManager.GetInterfaceString(5375 , szMessage ,INTERFACE_STRING_LENGTH);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                            return TRUE;
                        }					

                        if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2)==2)
                        {
                            uiTradeMan* tradeMan =
                                static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
                            assert (tradeMan);

                            if (tradeMan)
                            {
                                TCHAR buf[64];
                                for (i=6; i<64; ++i)
                                {
                                    if (pszString[i] == _T('\r'))
                                        break;

                                    buf[(i-6)] = pszString[i];
                                }

                                buf[(i-6)] = 0;

                                tradeMan->Ask(buf);
                            }
                        }
                        else if (_stscanf(pszString,_T("%s"),szTemp)==1)
                        {
                            uiTradeMan* tradeMan =
                                static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
                            assert (tradeMan);

                            if (tradeMan)
                            {
                                tradeMan->Ask(NULL);
                            }
                        }
                        else
                        {
                            //	/거래
                            g_InterfaceManager.GetInterfaceString( eST_GM_TRADE_HELP, szMessage, INTERFACE_STRING_LENGTH );
                            pDlg->AddString( szMessage );
                        }
                    }
                    return TRUE;

                    //ck: 파티: 초대			
                case eGAME_COMMAND_PARTY_INVITE:
                    {
                        uiPartyMan * partyMan =	GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );

                        if( partyMan == NULL )
                            return FALSE;

                        if( _stscanf(pszString,_T("%s %s"),szTemp,szTemp2) == 2 )
                        {
                            if (partyMan->IsParty())
                            {
                                if (partyMan->IsLeader())
                                {
                                    partyMan->AskJoinParty( szTemp2 );
                                }
                                else
                                {
                                    //	초대는 파티장만 유일하게 사용할 수 있습니다.
                                    g_InterfaceManager.GetInterfaceString( eST_GM_PARTY_INVITE_MSG, szMessage, INTERFACE_STRING_LENGTH );
                                    pDlg->AddString( szMessage );
                                }
                            }
                            else
                            {
                                partyMan->AskJoinParty( szTemp2 );
                            }
                        }
                        else
                        {
                            TargetDialog * pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);//GET_CAST_DIALOG( TargetDialog, InterfaceManager::DIALOG_TARGET );
                            if( pTargetDlg != NULL )
                            {
                                Object * pObj = g_ObjectManager.GetObject( pTargetDlg->GetTargetKey() );

                                if( g_pHero && g_pHero->GetObjectKey() == pTargetDlg->GetTargetKey() )
                                    return FALSE;

                                if( pObj != NULL )
                                {
                                    partyMan->AskJoinParty( pObj->GetName() );
                                    return TRUE;
                                }
                            }
                            //	/초대 캐릭명
                            g_InterfaceManager.GetInterfaceString( eST_GM_PARTY_INVITE_HELP, szMessage, INTERFACE_STRING_LENGTH );
                            pDlg->AddString( szMessage );
                        }
                    }
                    return TRUE;

                    // 개인상점
                case eGAME_COMMAND_PRIVATE_TRADE:
                    {
                        if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
                        {
                            //5375	카오 상태에서는 이용이 가능하지 않습니다.
                            g_InterfaceManager.GetInterfaceString(5375, szMessage ,INTERFACE_STRING_LENGTH);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                            return TRUE;
                        }


                        if (_stscanf(pszString,_T("%s"),szTemp)==1)
                        {
                            if ((GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE ) 
                                && (!GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->IsVisible())
                                && (!GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2 )->IsVisible())
                                && (!GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->IsVisible())
                                && (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_BATTLE_LOBBY) == FALSE))
                            {
                                if ( GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL )->IsVisible()			== FALSE
                                    && (g_InterfaceManager.IsShowDialog(IM_BANK_MANAGER::BANK_DIALOG)	== FALSE) )
                                    GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL )->ShowWindow(TRUE);
                            }
                            else
                            {
                                //	개인 상점 개설은 마을에서만 가능합니다.
                                const int	c_iPRIVATE_TRADE = 70399;
                                g_InterfaceManager.GetInterfaceString( c_iPRIVATE_TRADE, szMessage, INTERFACE_STRING_LENGTH );
                                pDlg->AddString( szMessage );
                            }
                        }
                        else
                        {
                            //	'/개인상점' 을 입력하세요.
                            const int	c_iPRIVATE_TRADE = 70384;
                            g_InterfaceManager.GetInterfaceString( c_iPRIVATE_TRADE, szMessage, INTERFACE_STRING_LENGTH );
                            pDlg->AddString( szMessage );
                        }
                    }
                    return TRUE;

                case eGAME_COMMAND_SCREEN_SHOT:
                    {
                        if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
                        {
                            //	켬, 끔
                            TCHAR	szON[INTERFACE_STRING_LENGTH], szOFF[INTERFACE_STRING_LENGTH];
                            g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_ON, szON, INTERFACE_STRING_LENGTH );
                            g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_OFF, szOFF, INTERFACE_STRING_LENGTH );

                            if( _tcsnccmp( szTemp2, szON, 4 ) == 0 )
                            {
                                BattleScene::SetScreenShotState( eSCREEN_SHOT_NOT_UI_AND_HERO );
                                g_InterfaceManager.ShowInterface( FALSE );
                            }
                            else if( _tcsnccmp( szTemp2, szOFF, 4 ) == 0 )
                            {
                                BattleScene::SetScreenShotState( eSCREEN_SHOT_FULL );
                                g_InterfaceManager.ShowInterface( TRUE );
                            }
                            else
                            {
                                //	파라미터가 잘못되었습니다.
                                pDlg->AddString(pszString);
                                g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                                pDlg->AddString( szMessage );
                            }
                        }
                        else
                        {
                            //	파라미터가 잘못되었습니다.
                            pDlg->AddString(pszString);
                            g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                            pDlg->AddString( szMessage );
                        }
                        return TRUE;
                    }
                    break;

                case eGAME_COMMAND_SET_WHISPER:
                    {
                        {
                            ChatDialog * pDlg = NULL;
                            pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
                            assert(pDlg);

                            if (_stscanf(pszString,_T("%s %s %s"),szTemp,szTemp2,szTemp3) == 3)
                            {
                                if (pDlg)
                                {
                                    //	귓말
                                    pszPos = util::GetToken( pszString + iCommandLen, szParameter, _T(" ") );

                                    pDlg->GetCurChatMessage();
                                    pDlg->SendWhisperMsg( pszPos, szTemp2);
                                }
                            }
                            else if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2)==2)
                            {
                                BOOL	bFlag;

                                //	켬, 끔
                                TCHAR	szON[INTERFACE_STRING_LENGTH], szOFF[INTERFACE_STRING_LENGTH];
                                g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_ON, szON, INTERFACE_STRING_LENGTH );
                                g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_OFF, szOFF, INTERFACE_STRING_LENGTH );

                                if( _tcsnccmp( szTemp2, szON, 4 ) == 0 )
                                {
                                    //	 귓말차단( 켬 )
                                    bFlag = FALSE;
                                }
                                else if( _tcsnccmp( szTemp2, szOFF, 4 ) == 0 )
                                {
                                    //	 귓말허용( 끔 )
                                    bFlag = TRUE;
                                }
                                else
                                {
                                    //	파라미터가 잘못되었습니다.
                                    pDlg->AddString(pszString);
                                    g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                                    pDlg->AddString( szMessage );

                                    return TRUE;
                                }

                                MSG_CW_WHISPER_SET_SYN	SendPacket;

                                SendPacket.bWhisper = bFlag;

                                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendPacket, sizeof( SendPacket ));

                                return TRUE;
                            }
                            else
                            {
                                //	[/귓말] [Char Name] [Message]
                                g_InterfaceManager.GetInterfaceString( eST_SHOW_CHAT_MESSAGE_INFO, szMessage, INTERFACE_STRING_LENGTH );
                                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                            }
                        }
                        return TRUE;
                    }
                    break;

                case eGAME_COMMAND_SIT:
                    {
                        //Todo:sit로 히어로 스테이트 바꿀것 

                        if( g_HeroInput.CanSitState() &&!g_pHero->IsTransForm())
                        {
                            MSG_CG_STATUS_CONDITION_CHANGE_SYN	Packet;
                            Packet.m_byCategory = CG_STATUS;
                            Packet.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;		
                            Packet.m_byConditionType = eCHAR_CONDITION_SITDOWN;

                            if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_STATUS_CONDITION_CHANGE_SYN) ) )
                            {
                                assert(!"앉기 패킷 전송 실패");
                                return TRUE;
                            }

#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
                            g_pHero->set_sitting_packet_delay(TRUE);
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG

                        }
                        return TRUE;
                    }
                    break;

                case eGAME_COMMAND_STAND:
                    {
                        //Todo:패킷 보낼것  
                        if(g_pHero->GetCurState()==STATE::SIT)
                        {

                            MSG_CG_STATUS_CONDITION_CHANGE_SYN	Packet;
                            Packet.m_byCategory = CG_STATUS;
                            Packet.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;		
                            Packet.m_byConditionType = eCHAR_CONDITION_STANDUP;

                            if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_STATUS_CONDITION_CHANGE_SYN) ) )
                            {
                                assert(!"서기 패킷 전송 실패");
                                return TRUE;
                            }
                        }
                        return TRUE;
                    }
                    break;


                case eGAME_COMMAND_FOLLOW_PLAYER:
                    {
                        DWORD	dwTarget = 0;
                        szTemp2[0] = NULL;
                        if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
                        {
                            //	이름으로 찾아서 적용
                            Object	*pObj = g_ObjectManager.GetObject( szTemp2 );
                            if( pObj )
                            {
                                dwTarget = pObj->GetObjectKey();
                                g_HeroInput.SetFollowState( TRUE, dwTarget );
                                return TRUE;
                            }
                            else
                            {
                                //	사용자를 찾을 수 없습니다.
                                g_InterfaceManager.GetInterfaceString( eST_USER_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );
                                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                                return TRUE;
                            }
                        }
                        else
                        {
                            //	이름을 입력하지 않았다. 타겟창에서 얻어오자
                            dwTarget = g_HeroInput.GetCurrentTarget();
                            if( dwTarget )
                            {
                                g_HeroInput.SetFollowState( TRUE, dwTarget );
                                return TRUE;
                            }
                            else
                            {
                                //	선택된 플레이어가 없습니다.
                                g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                                return TRUE;
                            }
                        }
                    }
                    break;

                case eGAME_COMMAND_PARTY_LEAVE:
                    {
                        if ( g_pHero->IsParty() )
                        {
                            uiPartyLeavePopup *partyLeavePopup = GET_CAST_DIALOG( uiPartyLeavePopup, IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

                            if( partyLeavePopup )
                            {
                                if( ( GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE ) ||
                                    ( GameFramework::GetCurScene() == SCENE_TYPE_FIELD ) )
                                    partyLeavePopup->LeaveParty();
                                else
                                    partyLeavePopup->ShowInterface(TRUE);
                            }
                        }
                        else
                        {
                            //	파티에 속해 있지 않습니다.
                            g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_PARTY, szMessage, INTERFACE_STRING_LENGTH );
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                        }

                        return TRUE;
                    }
                    break;

                case eGAME_COMMAND_GUILD_INVITE:
                    {
                        if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2)==2)
                        {
                            TCHAR buf[64];
                            for (i=10; i<64; ++i)
                            {
                                if (pszString[i] == _T('\r'))
                                    break;

                                buf[(i-10)] = pszString[i];
                            }

                            buf[(i-10)] = 0;

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                            GuildSystem::Function::InvitePlayer(buf);
#else
                            uiGuildMan* guildMan =
                                static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
                            assert (guildMan);

                            guildMan->Invite(buf);
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER                        
                        }

                        return TRUE;
                    }
                    break;

                    // 액션 명령어 빠진 내용 추가
#ifdef _NA_004612_120329_ADD_ACTION_COMMAND
                case eGAME_COMMAND_AUTO_ATTACK:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_AUTO_ATTACK + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_PICK_ITEM:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_PICK_ITEM + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_SIT_AND_STAND:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_SIT_AND_STAND + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_ASSIST:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_ASSIST + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_ASSIST_AND_ATTACK:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_ASSIST_AND_ATTACK + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_PARTY_MEMBER_SUMMON:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_PARTY_MEMBER_SUMMON + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_PARTY_MEMBER_WARP:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_PARTY_MEMBER_WARP + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_NORMAL_ATTACK:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_NORMAL_ATTACK + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_FRIEND_ADD:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_FRIEND_ADD + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_CRYSTAL_WARP_CREATE:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_CRYSTAL_WARP_CREATE + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
                case eGAME_COMMAND_IDENTIFY_SOCKET:
                    {
                        g_HeroInput.ProcessUserAction(
                            eACTION_IDENTIFY_SOCKET + eACTION_START_SKILL_CODE);
                        return TRUE;
                    }
                    break;
#endif //_NA_004612_120329_ADD_ACTION_COMMAND

                }	//	switch( i ) 

            }	//	if

        }	//	for (int i = 0; i < iCommandcnt; ++i)

        //#if _GLOBAL
#ifdef _YMS_LANGUAGE_KIND
        {
            TCHAR string_command[64] = {NULL, };
            LanguageType new_type = kLanguageGlobal;

            bool result = (_stscanf(pszString, _T("%s %d"), string_command, &new_type) == 2);

            if (result && (Stricmp(string_command, _T("//changelang")) == 0))
            {
                ChangeLanguage(new_type);
                return TRUE;
            }
        }
#endif

#ifdef _NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        {
            // 타락한사원에서 플레이어 이름 보이기(GM전용)
            TCHAR string_command[64] = {0,};
            TCHAR flag_command[64] = {0,};

            bool result = (_stscanf(pszString, _T("%s %s"), string_command, flag_command) == 2);

            if (result && (Stricmp(string_command, _T("//ssq_player_name")) == 0))
            {
                if (g_pHero->IsGM())
                {
                    _strlwr(flag_command);
                    if (Stricmp(flag_command, _T("on")) == 0)
                    {
                        GENERALPARAM->SetShowSSQPlayerName(true);
                    }
                    else if (Stricmp(flag_command, _T("off")) == 0)
                    {
                        GENERALPARAM->SetShowSSQPlayerName(false);
                    }
                }
                return TRUE;
            }
        }
#endif //_NA_004756_120504_SHOW_SSQ_PLAYER_NAME

#ifdef _DEV_VER
        //find keyword : eGM_CMD_GET_MONSTER_ATTACK_SPEED,   //몬스터공속 출력
        {
            TCHAR string_command[64] = {NULL, };
            BYTE attackspeed_type = 0;
            bool result = (_stscanf(pszString, _T("%s"), string_command) == 1);
            if (result && (Stricmp(string_command, _T("//attackspeed")) == 0))
            {
                attackspeed_type = 1;   // changed
            }
            else if (result && (Stricmp(string_command, _T("//attackspeedall")) == 0))
            {
                attackspeed_type = 2;   // all
            }

            if (attackspeed_type == 1 || attackspeed_type == 2)
            {
                FILE *fp = fopen("attackspeed.txt", "w+");
                if (fp == NULL)
                {
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "error: Can not open file");
                    return FALSE;
                }

                fprintf(fp, "MonsterCode\tAttSpeed1\tAttSpeed2\tName\tAniFile01\tAniFile02");
                for (int j = 0; j < BASE_NPCINFO::MAX_SKILL_NUM; ++j)
                {
                    fprintf(fp, "\tSkillCode%d\tSkillDelay%d", j, j);
                }
                fprintf(fp, "\n");
                SkillScriptInfo* skill_info = NULL;
                NPCInfoParser::Instance()->SetFirst();
                //int npc_count = 0;
                //int ani_count = 0;

                DWORD monster_code = 0;
                bool is_change = false;
                float max_frame1 = 0;
                float max_frame2 = 0;
                DWORD animation_code[10] = {0,};
                float skill_max_frame[10] = {0.0f,};
                TCHAR monster_name[MAX_MONSTERNAME_LENGTH] = {0,};
                TCHAR animation_file_name1[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR animation_file_name2[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR temp_string[5] = {0,};

                const BASE_NPCINFO* npc_info = NPCInfoParser::Instance()->GetNext();
                while (npc_info != NULL)
                {
                    if (attackspeed_type == 2)
                    {
                        is_change = true;
                    }
                    else
                    {
                        is_change = false;
                    }
                    max_frame1 = 0;
                    max_frame2 = 0;

                    RESOURCE_INFO* res_info = GraphicResourceList::Instance()->GetItemInfo(npc_info->m_dwICode);
                    monster_code = npc_info->m_MonsterCode;//monster code
                    if (res_info)
                    {
                        const int temp_resource_id = SCRIPT_RESOURCE_ID_OFFSET + 120003;
                        WzID temp_id = g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit(temp_resource_id, res_info->szFilename);
                        CWzUnit* unit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(temp_id);
                        if (unit)
                        {
                            //++npc_count;
                            WzAnimationInfo* ani_info = NULL;

                            ani_info = unit->GetAnimationInfo(StrToWzID("A001"));
                            if (ani_info)
                            {
                                float max_frame = (float)g_pSunRenderer->GetNumFrameOfAnimation(ani_info->m_hWzdAni) - 1;
                                max_frame1 = (max_frame*1000/30+1000);//attspeed1
                                max_frame1 = (max_frame1)?floor(max_frame1+0.5f):ceil(max_frame1-0.5f);
                                Sprintf(animation_file_name1, _T("%s"), ani_info->m_lpszFileName);
                                if (max_frame1 != npc_info->m_wAttSpeed)
                                {
                                    is_change |= true; 
                                }                            
                                //++ani_count;
                            }
                            else
                            {
                                strncpy(animation_file_name1, "empty_file", sizeof(animation_file_name1));
                            }

                            ani_info = unit->GetAnimationInfo(StrToWzID("A002"));
                            if (ani_info)
                            {
                                float max_frame = (float)g_pSunRenderer->GetNumFrameOfAnimation(ani_info->m_hWzdAni) - 1;
                                max_frame2 = (max_frame*1000/30+1000);//attspeed2
                                max_frame2 = (max_frame2)?floor(max_frame2+0.5f):ceil(max_frame2-0.5f);
                                Sprintf(animation_file_name2, _T("%s"), ani_info->m_lpszFileName);
                                if (max_frame2 != npc_info->m_wAttSpeed2)
                                {
                                    is_change |= true;
                                }
                                //++ani_count;
                            }
                            else
                            {
                                strncpy(animation_file_name2, "empty_file", sizeof(animation_file_name1));
                            }

                            ////////////////////////////////////////////////////////////////////////////
                            for (int skill_index = 0; skill_index < BASE_NPCINFO::MAX_SKILL_NUM; ++skill_index)
                            {
                                if (npc_info->m_wSkillCode[skill_index] != 0)
                                {
                                    skill_info = SkillInfoParser::Instance()->GetSkillInfo(npc_info->m_wSkillCode[skill_index]);
                                    animation_code[skill_index] = skill_info->m_wzidAnimationCode[0].id;
                                    ani_info = unit->GetAnimationInfo(animation_code[skill_index]);

                                    if(ani_info != NULL)
                                    {
                                        float max_frame = (float)g_pSunRenderer->GetNumFrameOfAnimation(ani_info->m_hWzdAni) - 1;
                                        skill_max_frame[skill_index] = (max_frame*1000/30);//skill_frame
                                        if (skill_max_frame[skill_index] > 0)
                                        {
                                            skill_max_frame[skill_index] = floor(skill_max_frame[skill_index]+0.5f);
                                        }
                                        else
                                        {
                                            skill_max_frame[skill_index] = ceil(skill_max_frame[skill_index]-0.5f);
                                        }
                                    }
                                    else
                                    {
                                        skill_max_frame[skill_index] = -1.0f;//ani_info가 없을 경우 -1 값
                                    }
                                }
                                else
                                {
                                    animation_code[skill_index] = 0;
                                    skill_max_frame[skill_index] = 0.0f;
                                }
                            }
                            ////////////////////////////////////////////////////////////////////////////
                            if (is_change == true)
                            {
                                ZeroMemory(monster_name, sizeof(monster_name));
                                strncpy(monster_name, npc_info->m_pszName, sizeof(monster_name));        //npc name
                            }
                            g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(temp_id);
                        }
                        else
                        {
                            fprintf(fp, "%d\t%d\t리소스가 없습니다.\n", monster_code, npc_info->m_dwICode);
                        }
                    }

                    if (is_change == true && res_info != NULL)
                    {
                        monster_name[MAX_MONSTERNAME_LENGTH-1] = '\0';
                        fprintf(fp, "%d\t%d\t%d\t%s\t%s\t%s", monster_code, (int)max_frame1, (int)max_frame2,
                            monster_name, animation_file_name1, animation_file_name2);
                        for (int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i)
                        {
                            if (animation_code[i] == 0)
                            {
                                fprintf(fp, "\t%d\t%d", animation_code[i], (int)skill_max_frame[i]);
                            }
                            else
                            {
                                WzIDToString(temp_string, animation_code[i]);
                                temp_string[4] = '\0';
                                fprintf(fp, "\t%s\t%d", temp_string, (int)skill_max_frame[i]);
                            }
                        }
                        fprintf(fp, "\n");
                    }
                    else if(res_info == NULL)
                    {
                        fprintf(fp, "%d\t%d\t리소스 코드가 없습니다.\n", monster_code, npc_info->m_dwICode);
                    }
                    npc_info = NPCInfoParser::Instance()->GetNext();
                }
                fflush(fp);
                fclose(fp);
            }
        }
#endif//_DEV_VER

#ifdef _USE_LUA_SCRIPT
  #ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
        {
            // 매크로 시작
            if ((Stricmp(pszString, _T("//RunMacro")) == 0))
            {
                GET_LUA()->RunScript("Run");
                return TRUE;
            }
            if ((Stricmp(pszString, _T("//ResetMacro")) == 0))
            {
                xGetHeroData()->macro_system()->ResetMacro();
                return TRUE;
            }
            if ((Stricmp(pszString, _T("//lua")) == 0))
            {
                cLuaMain::instance()->ToggleLuaConsole();
                return TRUE;
            }
        }
  #endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
#endif //_USE_LUA_SCRIPT
    }

    g_InterfaceManager.GetInterfaceString( eCHAT_COMMAND_WHISPER, szCommand, INTERFACE_STRING_LENGTH );
    if(NULL == strstr(pszString, szCommand))
    {
        for(int idx = iEmoCommandStart; idx < iEmoCommandcnt; idx++)
        {
            g_InterfaceManager.GetInterfaceString( idx, szCommand, INTERFACE_STRING_LENGTH );
            iCommandLen = _tcslen( szCommand );

            //타이핑된 문자열에 해당 커맨드 있는지 검사.
            if(NULL != strstr(pszString, szCommand))
            {
                switch( idx ) 
                {
                case eGAME_COMMAND_BOW:					//	인사
                case eGAME_COMMAND_LEAVE:				// 작별
                case eGAME_COMMAND_QUESTION:			// 의문
                case eGAME_COMMAND_SIGH:				// 한숨
                case eGAME_COMMAND_CHEER:				// 환호
                case eGAME_COMMAND_DENIAL:				// 거부
                case eGAME_COMMAND_CONSENT:				// 승낙
                case eGAME_COMMAND_ATTACK:				// 공격
                case eGAME_COMMAND_THANKS:				// 감사
                case eGAME_COMMAND_APOLOGY:				// 사과
                case eGAME_COMMAND_KISS:				// 키스
                case eGAME_COMMAND_LAUGH:				// 웃음
                case eGAME_COMMAND_CRYING:				// 울음
                case eGAME_COMMAND_WELCOME:				// 환영
                case eGAME_COMMAND_HANDCLAP:			// 박수
                case eGAME_COMMAND_AWE:					// 경외
                case eGAME_COMMAND_BEST:				// 최고
                case eGAME_COMMAND_SULKINESS:			// 삐침
                case eGAME_COMMAND_PROVOCATION:			// 도발
                case eGAME_COMMAND_SLEEP:				// 수면
                case eGAME_COMMAND_POSE:				// 포즈
                case eGAME_COMMAND_DANCE:				// 댄스
                case eGAME_COMMAND_GAG:					// 개그
                case eGAME_COMMAND_MIMICRY:				// 동물
                case eGAME_COMMAND_SHOCK:				// 쇼크
                    {
                        SLOTCODE	wSkillID = (SLOTCODE)(idx - eGAME_COMMAND_BOW);

                        TCHAR	szTemp[MAX_MESSAGE_LENGTH]   = {0,};
                        TCHAR	szResult[MAX_MESSAGE_LENGTH] = {0,};
                        g_InterfaceManager.GetInterfaceString(idx, szTemp, MAX_MESSAGE_LENGTH);
                        Sprintf(szResult, "/%s", szTemp);
                        int temp = Strcmp(szResult,pszString);

                        if(0 == temp) // '/'가 붙었을 경우 바로 이모션 처리
                        {
                            //------------------------------------------------------------------------------
                            // 수정 : define _YMS_CANNOT_EMOTION
                            // 내용 : if 문을 ProcessUserEmoticon() 함수 내로 이동
                            // 사유 : 중복된 코드 삭제
                            //       : ProcessUserEmoticon() 함수 안에서 false 가 될수 있는데 상관없이 
                            //         ProcessUserEmoticon() 내용과 상관없이 stop 패킷을 보내게됨
                            //if(g_pHero->IsMove())
                            //	g_pHero->SendStopMovePacket();
                            //------------------------------------------------------------------------------
                            g_HeroInput.ProcessUserEmoticon( (SLOTCODE)(wSkillID + eEMOTION_START_CODE) );

                            return TRUE;
                        }
                        else // '/'가 안붙었을 경우 채팅 메세지 출력후 이모션 처리
                        {
                            return wSkillID + eEMOTION_START_CODE;
                        }
                    }
                    break;
                }
            }
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
void CommandManager::ChangeLanguage(LanguageType new_type)
{
    LanguageType old_type = (LanguageType)CGameOption::Instance()->GetLanguageType();
    
    if ((new_type < kLanguageGlobal) || (new_type > kLanguageGermany))
    {
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "Not support the language");
        return;
    }

    if (old_type == new_type)
    {
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "The same as the language");
        return;
    }

    //------------------------------------------------------------------------------
    //! 파일 체크 
    bool result = true;
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\interface\\StringTable.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\interface\\StringTableForProgram.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\interface\\StringTableForName.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\interface\\StringTable_InstantHelp.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\messagetext.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\HelpLoadingMessage.txt", new_type);
    }
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\QuestText.txt", new_type);
    }
	if (result) 
	{
		result = GlobalFunc::HasFile_Nation("data\\MissionText.txt", new_type);
	}
    if (result) 
    {
        result = GlobalFunc::HasFile_Nation("data\\LibraryText.txt", new_type);
    }
    if (result == false)
    {
        TCHAR error_message[INTERFACE_STRING_LENGTH] = {NULL,};
        _tcscpy(error_message, _T("Has not language file"));

        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, error_message);
        return;
    }


    //------------------------------------------------------------------------------
    //! 언어 바꾸기
#ifdef _DH_OS_LANGUAGETYPE
    CGameOption::Instance()->SetLanguageType(kLanguageGlobal);
    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTable.txt"), ELUIT_NEW);
    SUN_ASSERT_MSG(result, "StringTable.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTableForProgram.txt"));
    SUN_ASSERT_MSG(result, "StringTableForProgram.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTableForName.txt"));
    SUN_ASSERT_MSG(result, "StringTableForName.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTable_InstantHelp.txt"));
    SUN_ASSERT_MSG(result, "StringTable_InstantHelp.txt load failed");
#endif//_DH_OS_LANGUAGETYPE
    CGameOption::Instance()->SetLanguageType(new_type);
#ifdef _DH_OS_LANGUAGETYPE
    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTable.txt"));
#else
    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTable.txt"), ELUIT_NEW);
#endif//_DH_OS_LANGUAGETYPE
    SUN_ASSERT_MSG(result, "StringTable.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTableForProgram.txt"));
    SUN_ASSERT_MSG(result, "StringTableForProgram.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTableForName.txt"));
    SUN_ASSERT_MSG(result, "StringTableForName.txt load failed");

    result = g_InterfaceManager.GetDialogManager()->LoadStringTable(GlobalFunc::AddNationName("StringTable_InstantHelp.txt"));
    SUN_ASSERT_MSG(result, "StringTable_InstantHelp.txt load failed");

    util::_tstring old_file_name;
    util::_tstring new_file_name;

    GlobalFunc::AddNationName(old_type, "data\\messagetext.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "data\\messagetext.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MESSAGE_TOTAL_MAX);
    SUN_ASSERT_MSG(result, "messagetext.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\HelpLoadingMessage.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\HelpLoadingMessage.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), 100);
    SUN_ASSERT_MSG(result, "HelpLoadingMessage.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\QuestText.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\QuestText.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_QUEST_TEXT_INFO);
    SUN_ASSERT_MSG(result, "QuestText.txt load failed");

	GlobalFunc::AddNationName(old_type, "Data\\MissionText.txt", old_file_name);
	GlobalFunc::AddNationName(new_type, "Data\\MissionText.txt", new_file_name);
	result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_QUEST_TEXT_INFO);
	SUN_ASSERT_MSG(result, "MissionText.txt load failed");

    GlobalFunc::AddNationName(old_type, "Data\\LibraryText.txt", old_file_name);
    GlobalFunc::AddNationName(new_type, "Data\\LibraryText.txt", new_file_name);
    result = g_pClientScriptParserManager->ReloadScript(old_file_name.c_str(), new_file_name.c_str(), MAX_HELP_INFO);
    SUN_ASSERT_MSG(result, "LibraryText.txt load failed");

    ////------------------------------------------------------------------------------
    ////! 필터링
    //g_slangFilter.Destroy();
    //g_slangFilter.Create();

    //g_slangNameFilter.Destroy();
    //g_slangNameFilter.Create();
    //if( CGeneralGameParam::Instance()->IsUsePackFile() )
    //{
    //    HRESULT hres = 1;
    //    hres = g_slangFilter.LoadFromPack(GlobalFunc::AddNationName("Data\\Filter.txt"),false);
    //    assert(hres && "Filter.txt 로딩 실패");

    //    hres = g_slangNameFilter.LoadFromPack(GlobalFunc::AddNationName("Data\\FilterName.txt"));
    //    assert(hres && "Data\\FilterName.txt 로딩 실패");
    //}
    //else
    //{
    //    HRESULT hres = 1;
    //    hres = g_slangFilter.Load(GlobalFunc::AddNationName("Data\\Filter.txt"),false);
    //    assert(hres && "Filter.txt 로딩 실패");

    //    hres = g_slangNameFilter.Load(GlobalFunc::AddNationName("Data\\FilterName.txt"));
    //    assert(hres && "Data\\FilterName.txt 로딩 실패");
    //}

    //! UI
    g_InterfaceManager.RestoreCaptionFromStringTable();

    //! 미니맵 
    uiRadarMan* radar_man = static_cast<uiRadarMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_RADAR));
    if (radar_man)
    {
        radar_man->SetMapName(g_pMap->GetFieldName());
    }

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
    //! 도움말
    uiHelpMan* help_man = static_cast<uiHelpMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
    if (help_man)
    {
        help_man->RestoreHelpInfo();
    }
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

    //! 퀘스트 진행 상태
    uiQuestMan* quest_man = static_cast<uiQuestMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST));
    if (quest_man)
    {
        quest_man->updateViewQuestForProgress();
    }


    //! NPC, PET 이름 바꿈
    g_ObjectManager.RestoreObjectName();

}



//---------------------------------------------------------------------------------------
//
//		디버깅용 명령어
//
//---------------------------------------------------------------------------------------

DebugCommandManager		g_DebugCommandManager;


DebugCommandManager::DebugCommandManager( void )
{
}

DebugCommandManager::~DebugCommandManager( void )
{
}

BOOL DebugCommandManager::ParseStyle( TCHAR * pszString )
{
	DWORD dwStyleCode = 0;

	TCHAR szTemp[256];

	_stscanf(pszString, _T("%s"),szTemp);

	dwStyleCode = GetStyleCode(szTemp);
	if(dwStyleCode==0xFFFFFFFF)
	{
		return FALSE;
	}

	//적용 
	g_pHero->SetCurrentAttackStyle(dwStyleCode);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD DebugCommandManager::GetStyleCode(TCHAR * pszString)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	SkillInfoParser::Instance()->SetFirst();
	BASE_STYLEINFO	*pBASE_STYLEINFO = NULL;

	while( true )
	{
		pBASE_STYLEINFO = SkillInfoParser::Instance()->GetNextStyle();
		if( pBASE_STYLEINFO == NULL )
		{
			return 0xFFFFFFFF;
		}

		//	일단 두번째 네임코드만 이용( 창, 지팡이, 오브 등등 )
		g_InterfaceManager.GetInterfaceString( pBASE_STYLEINFO->m_dwStyleNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	찾았다.
			return pBASE_STYLEINFO->m_SkillCode;
		}
	}

	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
스크립트에서 읽어서 체크할것 
*/
BOOL DebugCommandManager::ParseWeapon(TCHAR * pszString)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


	SLOTCODE dwWeaponCode = 0;

	TCHAR szTemp[256];

	_stscanf(pszString,_T("%s"),szTemp);

	dwWeaponCode = ( SLOTCODE )GetWeaponCode(szTemp);
	if(dwWeaponCode==0xFFFFFFFF)
	{
		return FALSE;
	}

	enum
	{
		eWT_TWOHANDAXE		= 5,
		eWT_TWOHANDSWORD	= 16,
		eWT_ONEHANDSWORD	= 31,
		eWT_SPEAR			= 46,
		eWT_STAFF			= 137,
		eWT_ORB				= 152,
	};

	if( GENERALPARAM->IsNet() )//네트웍 모드일경우 
	{
		//해당 무기를 바닥에 떨군다.
		MSG_CG_GM_CREATE_ITEM_SYN SendPacket;
		SendPacket.m_byCategory = CG_GM;
		SendPacket.m_byProtocol	= CG_GM_CREATE_ITEM_SYN;

		DWORD dwCode = eWT_TWOHANDAXE;

		if(dwWeaponCode==eWEAPONTYPE_TWOHANDAXE)
		{
			dwCode = eWT_TWOHANDAXE;
		}
		else if(dwWeaponCode==eWEAPONTYPE_TWOHANDSWORD)
		{
			dwCode = eWT_TWOHANDSWORD;
		}
		else if(dwWeaponCode==eWEAPONTYPE_ONEHANDSWORD)
		{
			dwCode = eWT_ONEHANDSWORD;
		}
		else if(dwWeaponCode==eWT_SPEAR)
		{
			dwCode = eWT_SPEAR;
		}
		else if(dwWeaponCode==eWEAPONTYPE_STAFF)
		{
			dwCode = eWT_STAFF;
		}
		else if(dwWeaponCode==eWEAPONTYPE_ORB)
		{
			dwCode = eWT_ORB;
		}
		SendPacket.m_ItemCode = ( SLOTCODE )dwCode;
		SendPacket.m_dwItemCount = 1;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );    

	}
	else//싱글일경우 강제 장착 시킨다.
	{

		BASE_PLAYERINFO* bp = g_pHero->GetCharInfo();
		//스크립트에서 읽어 체크 할것 

		if(bp&&bp->m_byClassCode==eCHAR_BERSERKER)
		{
			if(dwWeaponCode==eWEAPONTYPE_TWOHANDAXE)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_TWOHANDAXE );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_TWOHANDSWORD)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_TWOHANDSWORD );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_PUNCH)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				//g_InterfaceManager.ShowMessageBox( szMessage, 2000 );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				return FALSE;
			}
		}
		else if(bp&&bp->m_byClassCode==eCHAR_DRAGON)
		{
			if(dwWeaponCode==eWEAPONTYPE_ONEHANDSWORD)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_ONEHANDSWORD );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_SPEAR)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_SPEAR );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_INVALID)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				return FALSE;
			}
		}
		else if(bp&&bp->m_byClassCode==eCHAR_MAGICIAN)
		{
			if(dwWeaponCode==eWEAPONTYPE_PUNCH)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else if(dwWeaponCode==eWEAPONTYPE_STAFF)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_STAFF );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_ORB)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_ORB );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		//	무기 장착 성공
		
		//스타일 체크 무기와 안맞을때 스타일 없음으로 설정 
		g_pHero->CheckStyleForWeapon(dwWeaponCode);

	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD  DebugCommandManager::GetWeaponCode(TCHAR * pszString)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	ItemInfoParser::Instance()->SetFirst();
	BASE_ITEMINFO *pBASE_ITEMINFO = NULL;

	while( true )
	{
		pBASE_ITEMINFO = ItemInfoParser::Instance()->GetNext();
		if( pBASE_ITEMINFO == NULL )
		{
			return 0xFFFFFFFF;
		}

		//	일단 두번째 네임코드만 이용( 창, 지팡이, 오브 등등 )
		g_InterfaceManager.GetInterfaceString( pBASE_ITEMINFO->m_SCNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	찾았다.
			return pBASE_ITEMINFO->m_Code;
		}
	}

	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/

DWORD  DebugCommandManager::GetSkillCode(TCHAR * pszSkillName)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	SkillInfoParser::Instance()->SetFirst();
	SkillScriptInfo *pSkill = NULL;

	while( true )
	{	
		pSkill = SkillInfoParser::Instance()->GetNextSkill();
		if( pSkill == NULL )
		{
			return 0xFFFFFFFF;
		}

		g_InterfaceManager.GetInterfaceString( pSkill->m_dwSkillNcode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszSkillName ) == 0 )
		{
			//	찾았다.
			return pSkill->m_SkillCode;
		}
	}

	return 0xFFFFFFFF;
}

//
////------------------------------------------------------------------------------
///**
//*/
//BOOL DebugCommandManager::ParseSkill(TCHAR * pszString)
//{
//	DWORD	dwSkillCode = 0;
//	int		iLevel = 1;
//	TCHAR	szTemp[256];
//
//	_stscanf(pszString,_T("%s %d"),szTemp,&iLevel);
//
//	dwSkillCode = GetSkillCode(szTemp);
//	if( dwSkillCode == 0xFFFFFFFF )
//	{
//		return FALSE;
//	}
//
//	if (iLevel <= 0)
//	{
//		iLevel = 1;
//	}
//	else if (iLevel > 10)
//	{
//		iLevel = 10;
//	}
//
//	DWORD dwFinalSkillCode;
//
//	if (dwSkillCode == 0)
//	{
//		dwFinalSkillCode = 0;
//
//	}
//	else
//	{
//		dwFinalSkillCode = dwSkillCode + iLevel - 1;
//	}
//
//	g_HeroInput.SetCurrentSkillID((SLOTCODE)dwFinalSkillCode);
//	return TRUE;
//}

void DebugCommandManager::SaveItemList(eDEBUG_COMMAND eCOMD)
{
#ifdef _DEV_VER
	char szFilename[512] = {0, };
	SYSTEMTIME t;
	util::TimeSync::GetLocalTime( &t );

	// 컨테이너 아이템 정보 리스트업
	SCItemSlotContainer* pContainer = NULL;
	if (eCOMD == eDEBUG_COMMAND_SAVE_INVENTORY)
	{
		pContainer = (SCItemSlotContainer*)GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
		_snprintf(szFilename, 511, "./log/InventoryItemList-%d-%d-%d.txt", t.wYear, t.wMonth, t.wDay);
		SaveItemContainer(szFilename, pContainer);
		GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("인벤 아이템리스트 저장"));
	}
	else if (eDEBUG_COMMAND_SAVE_WAREHOUSE)
	{
		//창고
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
        pContainer = (SCItemSlotContainer*)GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
        pContainer = (SCItemSlotContainer*)GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
		_snprintf(szFilename, 511, _T("./log/WareHouseItemList-%d-%d-%d.txt"), t.wYear, t.wMonth, t.wDay);
		SaveItemContainer(szFilename, pContainer);
		GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("창고 아이템리스트 저장(창고는 최소 한번은 열어본 이후여야함)"));
	}
#endif//_DEV_VER
}
void DebugCommandManager::SaveItemContainer(TCHAR* szFilename, SCItemSlotContainer* pCont)
{
#ifdef _DEV_VER

	HANDLE hFile = CreateFile( szFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer( hFile, 0, NULL, FILE_END);

	{
		DWORD written;
		DWORD ItemCount = 0;;
		char szData[512] = {0, };
		// 시간표기		
		SYSTEMTIME t;
		util::TimeSync::GetLocalTime( &t );
		_snprintf(szData, 511, _T("\n//%2d시 %2d분 %2d초 - %s\r\n"), t.wHour, t.wMinute, t.wSecond, g_pHero->GetName());
		WriteFile( hFile, szData, strlen( szData), &written, NULL);

		//각 아이템정보 표기
		BYTE ContMaxNum = pCont->GetMaxSlotNum();
		SCItemSlot*	pSlot = NULL;
		// 필드명 표기
		_snprintf(szData, 511, _T("\t아이템명\t아이템분류\t클래스\t수량\t아이템코드\t인챈트\t랭크\t현재내구도\t최대내구도\t소켓\t남은시간\r\n"));
		WriteFile( hFile, szData, strlen( szData), &written, NULL);

		for (BYTE Index = 0; Index < ContMaxNum; ++Index)
		{
			pSlot = (SCItemSlot*)(&(pCont->GetSlot(Index)));
			if (pSlot->GetItemInfo()->m_Code)
			{
				TCHAR szItemName[64]	= {0, };
				TCHAR szItemType[64]	= {0, };
				TCHAR szEquipClass[64]	= {0, };
				DWORD  dwQuantity		= 0;
				DWORD  dwItemCode		= 0;
				DWORD  dwEnchant		= 0;
				DWORD  dwRank			= 0;
				DWORD  dwDura			= 0;
				DWORD  dwMaxDura		= 0;
				DWORD  dwSokect			= 0;
				DWORD64 dwExpiredTime	= 0;
				SYSTEMTIME ExpiredTime;
				ZeroMemory(&ExpiredTime,sizeof(SYSTEMTIME));
				TCHAR szExpiredTime[64]	= {0, };

				//아이템명
				if (pSlot->GetItemInfo()->IsWeapon() || pSlot->GetItemInfo()->IsArmor())
				{
					TCHAR szTemp1[64] = {0, };
					TCHAR szTemp2[64] = {0, };
					g_InterfaceManager.GetInterfaceString(pSlot->GetItemInfo()->m_NCode, szTemp1, 63);
					g_InterfaceManager.GetInterfaceString(pSlot->GetItemInfo()->m_SCNCode, szTemp2, 63);
					Snprintf(szItemName, 63, _T("%s %s"),szTemp1, szTemp2);
				}
				else
				{
					g_InterfaceManager.GetInterfaceString(pSlot->GetItemInfo()->m_NCode, szItemName, 63);
				}

				//아이템분류
				switch (pSlot->GetItemInfo()->m_wSubType)
				{
				case eITEMSUBTYPE_GENERAL:
					{
						if (pSlot->IsDivine())
						{
							StrnCopy(szItemType, _T("디바인"), 63);
						}
						else
						{
							StrnCopy(szItemType, _T("일반"), 63);
						}
						break;
					}
                case eITEMSUBTYPE_ELITE_SSQ:
				case eITEMSUBTYPE_ELITE:StrnCopy(szItemType, _T("엘리트"), 63);		break;
				case eITEMSUBTYPE_UNIQUE:	StrnCopy(szItemType, _T("유니크"), 63);		break;
				case eITEMSUBTYPE_FATE:		StrnCopy(szItemType, _T("럭키"), 63);		break;
				}
				//사용클래스
				if ( pSlot->GetItemInfo()->m_wEqClass1 && pSlot->GetItemInfo()->m_wEqClass2 && pSlot->GetItemInfo()->m_wEqClass4 && pSlot->GetItemInfo()->m_wEqClass5
					|| !(pSlot->GetItemInfo()->m_wEqClass1 || pSlot->GetItemInfo()->m_wEqClass2 || pSlot->GetItemInfo()->m_wEqClass4 || pSlot->GetItemInfo()->m_wEqClass5))
				{
					Snprintf(szEquipClass, 63, _T("모두"));
				}
				else if( pSlot->GetItemInfo()->m_wEqClass1 )
				{
					g_InterfaceManager.GetInterfaceString( 10001, szEquipClass, INTERFACE_STRING_LENGTH );
				}
				else if ( pSlot->GetItemInfo()->m_wEqClass2)
				{
					g_InterfaceManager.GetInterfaceString( 10003, szEquipClass, INTERFACE_STRING_LENGTH );
				}
				else if ( pSlot->GetItemInfo()->m_wEqClass3)
				{
					g_InterfaceManager.GetInterfaceString( 10005, szEquipClass, INTERFACE_STRING_LENGTH );
				}
				else if ( pSlot->GetItemInfo()->m_wEqClass4)
				{
					g_InterfaceManager.GetInterfaceString( 10007, szEquipClass, INTERFACE_STRING_LENGTH );
				}
				else if ( pSlot->GetItemInfo()->m_wEqClass5)
				{
					g_InterfaceManager.GetInterfaceString( 10009, szEquipClass, INTERFACE_STRING_LENGTH );
				}


				// 수량 및 내구도
				if (pSlot->IsContainValidItem() && pSlot->WithDura())
				{
					dwQuantity = 1;
					dwDura = pSlot->GetDura();
					dwMaxDura = pSlot->GetMaxDura();
				}
				else
				{
					dwQuantity = pSlot->GetNum();
					dwDura = 0;
					dwMaxDura = 0;
				}
				//아이템코드
				dwItemCode = pSlot->GetItemInfo()->m_Code;
				//인챈트
				dwEnchant = pSlot->GetEnchant();
				//랭크
				dwRank = pSlot->GetRank();
				//소켓
				dwSokect = pSlot->GetSocketNum();
				//남은시간
				if (pSlot->GetDateTimeToSYSTEMTIME(ExpiredTime))
				{
					_snprintf(szData, 511, _T("\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d년%d월%d일%d시%d분%d초까지\r\n"),
						szItemName, szItemType, szEquipClass, dwQuantity, dwItemCode, dwEnchant, dwRank, dwDura, dwMaxDura, dwSokect, ExpiredTime.wYear, ExpiredTime.wMonth, ExpiredTime.wDay, ExpiredTime.wHour, ExpiredTime.wMinute, ExpiredTime.wSecond);
				}
				else
				{
					_snprintf(szData, 511, _T("\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"),
						szItemName, szItemType, szEquipClass, dwQuantity, dwItemCode, dwEnchant, dwRank, dwDura, dwMaxDura, dwSokect, dwExpiredTime);
				}
				WriteFile( hFile, szData, strlen( szData), &written, NULL);
				++ItemCount;
			}			
		}
		if (ItemCount < 1)
		{
			_snprintf(szData, 511, _T("\t<아이템없음>\r\n"));
			WriteFile( hFile, szData, strlen( szData), &written, NULL);
		}
	}

	// 파일 닫기
	CloseHandle( hFile);
#endif//_DEV_VER	
}



BOOL DebugCommandManager::ParseCommand( TCHAR * pszString )
{
#ifdef _DEV_VER
    // no operation
#else
    return FALSE; // dev 버전이 아니면 무조건 FALSE
#endif

	if( g_pHero == NULL )
	{
		return FALSE;
	}


	if( GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE )
	{
		return FALSE;
	}

	if(pszString[0] != '/')
	{
		return FALSE;
	}

	
	TCHAR szDevCommand[64] = {0};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0};

	TCHAR szTemp[64] = {0};
	TCHAR szTemp2[64] = {0};
	TCHAR szTemp3[64] = {0};
	TCHAR szParam[64] = {0};


	_stscanf(pszString,_T("%s %s"),szDevCommand, szTemp);

	//작업요청서[001039]: 서버에서 처리하는 GM명령어는 클라에서 관리하지 않지만 클라에서 처리가 필요한 예외적인 특수 명령어는 하드코딩 -_-;

	ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	if(GENERALPARAM->IsNet()==FALSE)	// 싱글모드에서만	가능한 GM 명령어
	{
		if(0 == Stricmp(szDevCommand, _T("//몬스터소환")))
		{
			int monsterCode;
			DWORD  num;

			if (_stscanf(pszString,_T("%s %d %d"),szTemp,&monsterCode,&num)==3)
			{
				Monster *pMon = GameFunc::SpawnMonster(monsterCode);
				if(!pMon)
					pDlg->AddString(_T("몬스터생성불가"));
				
			}
			else 
			{
				//	파라미터 갯수가 잘못 되었습니다.
				pDlg->AddString(pszString);
				g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
				pDlg->AddString( szMessage );
			}
			return TRUE;
		
		}
		else if(0 == Stricmp(szDevCommand, _T("//생성")))
		{
			SCSlotContainer* pContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
			if(NULL == pContainer)
				return TRUE;

			DWORD iMax = pContainer->GetMaxSlotNum();
			TCHAR szTemp[16 + 1] = {0,};
			DWORD dwItemCode = 0;
			DWORD dwItemCnt  = 0;
			BOOL bVal = FALSE;
			if (3 == _stscanf(pszString,_T("%s %d %d"),szTemp, &dwItemCode, &dwItemCnt))
			{
				for(DWORD k = 0; k < dwItemCnt; k++)
				{
					for(DWORD i =0; i < iMax; i++)
					{
						if(pContainer->IsEmpty((BYTE)i))
						{
							SCItemSlot ItemSlot;
							ItemSlot.SetCode((WORD)dwItemCode);
							pContainer->InsertSlot((BYTE)i, ItemSlot);	
							bVal = TRUE;
							break;
						}
						else
						{
							bVal = FALSE;
						}
					}
				}

				if(FALSE == bVal)
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("인벤이 꽉 찼어요"));
				}
			}
			return TRUE;
		}
        else if(0 == Stricmp(szDevCommand, _T("//속도")) || 0 == Stricmp(szDevCommand, _T("//speed")))
        {
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
            int speed_value;
            if (_stscanf(pszString,_T("%s %d"), temp_string, &speed_value) == 2)
            {
                speed_value = max(speed_value, 1);
                speed_value = min(speed_value, 5);
                g_pHero->SetCustomMoveSpeed(TRUE, static_cast<float>(speed_value));
                
                ChatDialog * chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                if (chat_dialog != NULL)
                {
                    // 70383 GM 캐릭터 이동 속도 레벨 : %d
                    TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
                    g_InterfaceManager.GetInterfaceString(70383, temp_string);
                    Sprintf(message_string, temp_string, speed_value);
                    chat_dialog->AddString(message_string);
                }                
            }            
        }
#ifdef _USE_LUA_SCRIPT
        else if (0 == Stricmp(szDevCommand, _T("//lua")))
        {
#ifdef _DEV_VER
            cLuaMain::instance()->ToggleLuaConsole();
#endif //_DEV_VER
        }
#endif //_USE_LUA_SCRIPT
	}
	else
	{
		if(0 == Stricmp(szDevCommand, _T("/renderoption")))
		{
			if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2)==2)
			{
				if (!Strcmp(szTemp2,_T("light")))
				{
					if (GameFramework::GetCurrentScene())
					{
						BattleScene *pScene = (BattleScene *)GameFramework::GetCurrentScene();
						pScene->ShowLightDirection();

						if (g_pMap)
						{
							if (g_pMap->GetWorldBase()->IsEnabledIndoorLight())
							{
								//	실내광원을 사용중입니다
								g_InterfaceManager.GetInterfaceString( eST_GM_USING_INDOOR_LIGHT_HELP, szMessage, INTERFACE_STRING_LENGTH );
								pDlg->AddString( szMessage );
							}	
							else
							{
								//	실외광원을 사용중입니다
								g_InterfaceManager.GetInterfaceString( eST_GM_USING_OUTDOOR_LIGHT_HELP, szMessage, INTERFACE_STRING_LENGTH );
								pDlg->AddString( szMessage );
							}

							WzVector vDir = g_pMap->GetWorldBase()->GetSunLightDir();
							Sprintf(szMessage,_T("Light Direction : %f %f %f"),vDir.x,vDir.y,vDir.z);
							pDlg->AddString( szMessage );									

						}
					}
				}
				else if (!Strcmp(szTemp2,_T("lightcolor")))
				{
					int iR,iG,iB;
					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						if (g_pMap)
						{
							WzVector vDir = g_pMap->GetWorldBase()->GetSunLightDir();								   
							WzColor wcLightColor = WzColor_RGBA(iR,iG,iB,255);
							g_pMap->GetWorldBase()->SetSunLightColor(wcLightColor);
						}
					}

					WzColor color;
					color = g_pMap->GetWorldBase()->GetSunLightColor();
					Sprintf(szTemp3,_T("Current Light Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("shadowbias")))
				{
					float fDepthBias;
					if (_stscanf(pszString,_T("%*s %*s %f"),&fDepthBias)== 1)
					{
						g_pSunRenderer->x_pManagerShadowMap->SetDepthBias(fDepthBias);
					}

					fDepthBias = g_pSunRenderer->x_pManagerShadowMap->GetDepthBias();
					Sprintf(szTemp3,_T("Current Shadow Depth Bias : %e"),fDepthBias);
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("mapambient")))
				{
					int iR,iG,iB;

					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						if (g_pMap)
						{								   
							WzColor wcLightColor = WzColor_RGBA(iR,iG,iB,255);
							g_pMap->GetWorldBase()->SetMapAmbientColor(wcLightColor);

						}

					}

					WzColor color;
					color = g_pMap->GetWorldBase()->GetMapAmbientColor();
					Sprintf(szTemp3,_T("Current Mapambient Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("ambient")))
				{
					int iR,iG,iB;

					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						WzColor wcLightColor = WzColor_RGBA(iR,iG,iB,255);
						g_pSunRenderer->SetColor(wcLightColor,WRIC_AMBIENT);
					}

					WzColor color;
					color = g_pSunRenderer->GetAmbientColor();
					Sprintf(szTemp3,_T("Current Ambient Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("shadowcolor")))
				{
					int iR,iG,iB;

					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						WzColor wcShadowColor = WzColor_RGBA(iR,iG,iB,255);
						g_pSunRenderer->SetShadowColor(wcShadowColor);

					}

					WzColor color;
					color = g_pSunRenderer->GetShadowColor();
					Sprintf(szTemp3,_T("Current Shadow Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("projection")))
				{
					float fNear,fFar;

					if (_stscanf(pszString,_T("%*s %*s %f %f"),&fNear,&fFar)== 2)
					{							   
						g_Camera.SetProjection(fNear,fFar,g_Camera.GetFOV());
					}

					pDlg->AddString(_T("Current Projection Setting"));
					Sprintf(szTemp3,_T("NearClip : %f FarClip : %f FOV : %f"),g_Camera.GetNearClip(),g_Camera.GetFarClip(),g_Camera.GetFOV() * 180.0f / WZ_PI);
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("fov")))
				{
					float fFOV;

					if (_stscanf(pszString,_T("%*s %*s %f"),&fFOV)== 1)
					{						
						fFOV *= WZ_PI / 180.0f;
						g_Camera.SetProjection(g_Camera.GetNearClip(),g_Camera.GetFarClip(),fFOV);
					}

					pDlg->AddString(_T("Current Projection Setting"));
					Sprintf(szTemp3,_T("NearClip : %f FarClip : %f FOV : %f"),g_Camera.GetNearClip(),g_Camera.GetFarClip(),g_Camera.GetFOV() * 180.0f / WZ_PI);
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("warp")))
				{
					int iID;

					if (_stscanf(pszString,_T("%*s %*s %d"),&iID)== 1)
					{
						Object *pObject = g_ObjectManager.GetObject(iID);

						if (pObject) 
						{
							WzVector vPos = pObject->GetPosition();
							if (g_pHero)
							{
								MSG_CG_MAP_TELEPORT_SYN SendPacket;
								SendPacket.m_byCategory = CG_MAP;
								SendPacket.m_byProtocol = CG_MAP_TELEPORT_SYN;
								SendPacket.vPos = vPos;
								g_pHero->SendPacket( &SendPacket, sizeof(SendPacket) );
							}
						}
					}
				}
				else if (!Strcmp(szTemp2,_T("cambient")))
				{
					int iR,iG,iB;

					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						if (g_pMap)
						{								   
							WzColor wcLightColor = WzColor_RGBA(iR,iG,iB,255);									
							g_pMap->SetCharacterAmbientColor(wcLightColor);
							g_pMap->SetApplyAnotherLight(TRUE);
						}

					}

					WzColor color;
					color = g_pMap->GetCharacterAmbientColor();
					Sprintf(szTemp3,_T("Current Character Ambient Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("trate")))
				{
					float fRate;

					if (_stscanf(pszString,_T("%*s %*s %f"),&fRate)== 1)
					{
						if (g_pMap)
						{								   
							GRAPHICPARAM->SetTerrainColorRate(fRate);
						}

					}

					fRate = GRAPHICPARAM->GetTerrainColorRate();
					Sprintf(szTemp3,_T("Current Character Terrain Color Rate: %f"),fRate);
					pDlg->AddString(szTemp3);

				}
				else if (!Strcmp(szTemp2,_T("clight")))
				{
					int iR,iG,iB;

					if (_stscanf(pszString,_T("%*s %*s %d %d %d"),&iR,&iG,&iB)== 3)
					{
						if (g_pMap)
						{								   
							WzColor wcLightColor = WzColor_RGBA(iR,iG,iB,255);
							WzVector vDir = g_pMap->GetWorldBase()->GetSunLightDir();
							g_pSunRenderer->x_pManagerLight->SetCustomDirectionalLight(wcLightColor,vDir);
							g_pSunRenderer->x_pManagerLight->SetCustomLightStyle(CLS_DEFAULT);
						}

					}

					const St_LightInformation *pInfo = g_pSunRenderer->x_pManagerLight->GetCustomLightInfo(0);

					if (pInfo)
					{
						WzColor color;
						color = pInfo->m_wcColor;
						Sprintf(szTemp3,_T("Current Character Directional Light Color : %d %d %d"),Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color));
						pDlg->AddString(szTemp3);
					}

				}
				else if (!Strcmp(szTemp2,_T("maxcameradistance")))
				{
					float fDistance;

					if (_stscanf(pszString,_T("%*s %*s %f"),&fDistance)== 1)
					{
						if (g_pMap)
						{								   
							GENERALPARAM->SetMaxCameraDistance(fDistance);
						}

					}

					fDistance = GENERALPARAM->GetMaxCameraDistance();
					Sprintf(szTemp3,_T("Current Max Camera Distance: %f"),fDistance);
					pDlg->AddString(szTemp3);
				}

			}

			return TRUE;
		}
		else if((0 == Stricmp(szDevCommand, _T("//킬"))) || (0 == Stricmp(szDevCommand, _T("//kill"))))	
		{
			//현재 주인공의 타켓팅된캐릭터를 얻어온다					
			Object *pObj = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

			if(pObj)
			{
				//클라에서 몬스터오브젝트인지체크한다.
				if(pObj->IsKindOfObject(MONSTER_OBJECT))
				{
					//" "+몬스터오브젝트키값 더해서 서버로 보내준다.
					_tcscat( pszString , _T(" ") );
					TCHAR szObjectKey[30];
					_itoa(g_HeroInput.GetCurrentTarget(),szObjectKey,10);
					_tcscat( pszString , szObjectKey);
					return FALSE;	// 서버에 GM 명령어와 함께 오브젝트키를 보내줘야 한다. 패킷을 보내기 위해선 return 값을 FALSE로 처리
				}
			}

			//Target몬스터가 없을때		
			g_InterfaceManager.GetInterfaceString( 7006, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			return TRUE;
		}
		else if((0 == Stricmp(szDevCommand, _T("//쿨타임"))) || (0 == Stricmp(szDevCommand, _T("//ctime"))))
		{
            xGetCooltimeManager()->ResetSkillCooltime();
            xGetCooltimeManager()->ResetItemCooltime();
   //         //스킬쿨타임초기화
   //         xGetHeroData()->ResetCooltime();
			////g_pHero->GetSCCharacter().InitializeCooltime();

			////아이템쿨타임 초기화
			//g_pHero->GetSCPlayer().InitializeItemCooltime();	
		}

#ifdef _DEV_VER
#if (WZENGINEVER >= 480)
        else if(0 == Strcmp(pszString, _T("//콘솔") ) || 0 == Strcmp(pszString, _T("//console") ) )
        {
            LogManager::ToggleConsoleView();
        }
        else if( StrStr( pszString, _T("//콘솔 ") ) )
        {
            LogManager::ProcessCommand( pszString + _tcslen(_T("//콘솔 ")) );
        }
        else if( StrStr( pszString, _T("//console ") ) )
        {
            LogManager::ProcessCommand( pszString + _tcslen(_T("//console ")) );
        }

#ifdef _DEBUG
        else if( StrStr( pszString, _T("//polymorph") ) )
        {
            static DWORD s_ObjectKey = 112211;
            if( Object* pObject = g_ObjectManager.GetObject( s_ObjectKey ))
            {
                if( pObject->IsKindOfObject( CHARACTER_OBJECT ) )
                {
                    Character* pCharacter = (Character*)pObject;
                    pCharacter->SetPolymorph( pCharacter->IsPolymorph() == FALSE );
                }
            }
        }
#endif

#ifdef _DEV_VER

        else if( Strcmp( pszString, _T("//option") ) == 0 )
        {
            uiOptionMan::ShowOptionDialog(0);
        }
        else if( Strcmp( pszString, _T("//option2") ) == 0 )
        {
            uiOptionMan::ShowKeyBindingDialog();
        }

		else if( Strcmp( pszString, _T("//skill_max") ) == 0 )
		{
			std::vector<int> new_code_array;
			if( xGetSkillData()->MakeMaxSkillCodeArray( new_code_array ) == true )
			{
				{
					MSG_CG_GM_STRING_CMD_SYN SendPacket;
					ZeroMemory(SendPacket.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
					strcpy(SendPacket.m_szStringCmd, "스킬포인트업 333" );
					GlobalFunc::SendPacket(CI_GAMESERVERIDX,&SendPacket,sizeof(SendPacket));
				}

				{
					//스킬 배우는 패킷을 날린다
					MSG_CG_SKILL_SELECT_SKILLPOINT_SYN send_packet;
					send_packet.skill_count = new_code_array.size();
					for( size_t i=0;i!=new_code_array.size();++i)
					{
						send_packet.skill_code_array[i] = new_code_array[i];
					}

					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &send_packet, sizeof(send_packet) );
				}
			}

		}
#endif //_DEV_VER


#endif // (WZENGINEVER >= 480)
        else if( StrStr( pszString, _T("//skill_log") ) ) 
        {
            g_InterfaceManager.ToggleSkillResultMessage();
        }

#endif // _DEV_VER

#ifdef _USE_LUA_SCRIPT
        else if (0 == Stricmp(szDevCommand, _T("//lua")))
        {
#ifdef _DEV_VER
            cLuaMain::instance()->ToggleLuaConsole();
#endif //_DEV_VER
        }
#endif

		else if((0 == Stricmp(szDevCommand, _T("//인벤저장")))	|| (0 == Stricmp(szDevCommand, _T("//savei"))))
		{
			SaveItemList(eDEBUG_COMMAND_SAVE_INVENTORY);

			return TRUE;
		}
		else if((0 == Stricmp(szDevCommand, _T("//창고저장"))) || (0 == Stricmp(szDevCommand, _T("//savew"))))
		{
			SaveItemList(eDEBUG_COMMAND_SAVE_WAREHOUSE);

			return TRUE;
		}
		else if(0 == Stricmp(szDevCommand, _T("//autoefflv")))
		{
			TCHAR szOption[8] = {0};
			if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
			{
				if (!Stricmp(szTemp2,_T("on")))
				{
                    g_pEffectManager->SetOption( EOPT_AUTOEFFLVLCTL );
					GlobalFunc::OutputMsg_SysMsg(_T("이펙트 레벨 자동 조정 On"));
				}
				else if (!Stricmp(szTemp2,_T("off")))
				{
                    g_pEffectManager->UnsetOption( EOPT_AUTOEFFLVLCTL );
					GlobalFunc::OutputMsg_SysMsg(_T("이펙트 레벨 자동 조정 Off"));
				}
				else
				{
					g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_SysMsg(szMessage);
				}
			}
			else
			{
				g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_SysMsg(szMessage);
			}

			
			return TRUE;
		}
        else if(0 == Stricmp(szDevCommand, _T("//autoeffrate")))
        {
            TCHAR szOption[8] = {0};
            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                if (!Stricmp(szTemp2,_T("on")))
                {
                    g_pEffectManager->SetOption( EOPT_AUTOEFFRATECTL );
                    GlobalFunc::OutputMsg_SysMsg(_T("자동 이펙트 생성 비율 조절 On"));
                }
                else if (!Stricmp(szTemp2,_T("off")))
                {
                    g_pEffectManager->UnsetOption( EOPT_AUTOEFFRATECTL );
                    GlobalFunc::OutputMsg_SysMsg(_T("자동 이펙트 생성 비율 조절 Off"));
                }
                else
                {
                    g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_SysMsg(szMessage);
                }
            }
            else
            {
                g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_SysMsg(szMessage);
            }


            return TRUE;
        }


#ifdef _DEV_VER
        else if (0 == Stricmp(szDevCommand, _T("//어그로보기")) || (0 == Stricmp(szDevCommand, _T("//show_aggro"))))
        {
            uiTargetAggro* target_aggro_dialog = GET_CAST_DIALOG(uiTargetAggro, IM_TARGET_MANAGER::TARGET_AGGRO);
            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                target_aggro_dialog->set_display_type(atoi(szTemp2));
            }
            else
            {
                target_aggro_dialog->set_display_type(0);//uiTargetAggro::kDisplay_Normal
            }
        }
        else if (0 == Stricmp(szDevCommand, _T("//지엠아이템")) || (0 == Stricmp(szDevCommand, _T("//gmitem"))))
        {
            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                if (!Stricmp(szTemp2,_T("on")) || !Stricmp(szTemp2,_T("켬")))
                {
                    ShowGMWin(UI_GM_ITEM_WINDOW, true);
                }
                else if (!Stricmp(szTemp2,_T("off")) || !Stricmp(szTemp2,_T("끔")))
                {
                    ShowGMWin(UI_GM_ITEM_WINDOW, false);
                }
                else
                {
                    ItemSearch(szTemp2);
                }
            }
            else
            {
                OpenGMWin(UI_GM_ITEM_WINDOW);
            }
        }
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
		else if(0 == Stricmp(szDevCommand, _T("//뷰어")) || (0 == Stricmp(szDevCommand, _T("//viewer"))))
		{
			OpenTextureViewerWin(UI_TEXTURE_VIEWER_SEARCH_WINDOW);
            OpenTextureViewerWin(UI_TEXTURE_VIEWER_TEXTURE_WINDOW);            
		}
#endif
#endif

#ifdef _DH_GM_NPC_WINDOW
        else if (0 == Stricmp(szDevCommand, _T("//지엠엔피시")) || (0 == Stricmp(szDevCommand, _T("//gmnpc"))))
        {
            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                if (!Stricmp(szTemp2,_T("on")) || !Stricmp(szTemp2,_T("켬")))
                {
                    ShowGMWin(UI_GM_ITEM_NPC_WINDOW, true);
                }
                else if (!Stricmp(szTemp2,_T("off")) || !Stricmp(szTemp2,_T("끔")))
                {
                    ShowGMWin(UI_GM_ITEM_NPC_WINDOW, false);
                }
                else
                {
                    ItemSearch(szTemp2);
                }
            }
            else
            {
                OpenGMWin(UI_GM_ITEM_NPC_WINDOW);
            }
        }
#endif//_DH_GM_NPC_WINDOW
#ifdef _DEV_VER
        else if (0 == Stricmp(szDevCommand, _T("//autokill")))
        {
            if (CGeneralGameParam::Instance()->IsEnableAutoKill())
            {
                CGeneralGameParam::Instance()->SetEnableAutoKill(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetEnableAutoKill(true);
            }
        }
        else if (0 == Stricmp(szDevCommand, _T("//autocooltime")))
        {
            if (CGeneralGameParam::Instance()->IsEnableAutoCooltimeReset())
            {
                CGeneralGameParam::Instance()->SetEnableAutoCooltimeReset(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetEnableAutoCooltimeReset(true);
            }
        }
#endif
        else if (0 == Stricmp(szDevCommand, _T("//show_trigger")))
        {
            if (CGeneralGameParam::Instance()->IsShowTrigger())
            {
                CGeneralGameParam::Instance()->SetShowTrigger(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetShowTrigger(true);
            }
        }
        //#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
        else if (0 == Stricmp(szDevCommand, _T("//set_map_portal")))
        {
            enum
            {
                kToggle = 0,
                kOpen = 1,
                kClose = 2,
            };
            BYTE open_type = kToggle;
            ContinentMapDlg* continent_map_dialog = 
                GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);

            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                if (!Stricmp(szTemp2,_T("on")) || !Stricmp(szTemp2,_T("켬")))
                {
                    open_type = kOpen;   
                }
                else if (!Stricmp(szTemp2,_T("off")) || !Stricmp(szTemp2,_T("끔")))
                {
                    open_type = kClose;
                }
                else
                {
                    g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_SysMsg(szMessage);
                }
            }
            else
            {
                open_type = kToggle;
            }

            if (continent_map_dialog)
            {
                bool is_usable = continent_map_dialog->get_is_useable_map_portal();
                if (open_type == kToggle)
                {
                    is_usable = !(is_usable);
                }
                else
                {
                    is_usable = (open_type == kOpen) ? true:false;
                }
                continent_map_dialog->set_is_useable_map_portal(is_usable);
            }
            ContinentEp2MapDlg* continent_map_dialog2 = 
                GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
            if (continent_map_dialog2)
            {
                bool is_usable = continent_map_dialog2->get_is_useable_map_portal();
                if (open_type == kToggle)
                {
                    is_usable = !(is_usable);
                }
                else
                {
                    is_usable = (open_type == kOpen) ? true:false;
                }
                continent_map_dialog2->set_is_useable_map_portal(is_usable);
            }
        }
        //#endif//_DEV_VER //_DH_FULL_MAP_PORTAL
        //#ifdef _DH_SHOW_UI_NAME
        else if (0 == Stricmp(szDevCommand, _T("//show_ui_name")))
        {
            if (CGeneralGameParam::Instance()->IsShowUIName())
            {
                CGeneralGameParam::Instance()->SetShowUIName(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetShowUIName(true);
            }
        }
        //#endif//_DH_SHOW_UI_NAME
        //#ifdef _DEBUG_STRINGIDEX_SHOW
        else if (0 == Stricmp(szDevCommand, _T("//show_string_id")))
        {
            if (CGeneralGameParam::Instance()->IsShowStringId())
            {
                CGeneralGameParam::Instance()->SetShowStringId(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetShowStringId(true);
            }
        }
        //#endif//_DEBUG_STRINGIDEX_SHOW
        //#ifdef _SCJ_SHOW_ITEM_ID
        else if (0 == Stricmp(szDevCommand, _T("//show_item_id")))
        {
            if (CGeneralGameParam::Instance()->IsShowItemId())
            {
                CGeneralGameParam::Instance()->SetShowItemId(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetShowItemId(true);
            }
        }
        //#endif //#ifdef _SCJ_SHOW_ITEM_ID        
        //#ifdef _SCJ_SHOW_NPC_ID
        else if (0 == Stricmp(szDevCommand, _T("//show_npc_id")))
        {
            if (CGeneralGameParam::Instance()->IsShowNpcId())
            {
                CGeneralGameParam::Instance()->SetShowNpcId(false);
            }
            else
            {
                CGeneralGameParam::Instance()->SetShowNpcId(true);
            }
        }
        //#endif _SCJ_SHOW_NPC_ID        
		else if(0 == Stricmp(szDevCommand, _T("//get_skill_tick")))
		{
			if (_stscanf(pszString,_T("%s"),szTemp)==1)
			{
                FILE *fp = fopen("skillspeed.txt", "w+");
                if (fp == NULL)
                {
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "error: Can not open file");
                    return FALSE;
                }

                int pre_resource_code = 0;
                int pre_skill_class_code = 0;
                int resource_code = 0;
                int animation_code = 0;
                WzID temp_id = 0;
                CWzUnit* unit = NULL;
                const int temp_resource_id = SCRIPT_RESOURCE_ID_OFFSET + 120003;
                char code[6] = {0,};

                SkillInfoParser::Instance()->SetFirst();
                SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetNextSkill();

                fprintf(fp, "SkillCode\tSkillClassCode\tMaxTick\tAttacTick\tAniPlayTime\tAniCode\tSkillName\tClassDefine\n");
                while(skill_info != NULL)
                {
                    if (skill_info->m_bySkillUserType == 0)// 0 = 플레이어
                    {
                        //if (pre_skill_class_code != skill_info->m_SkillClassCode)(같은 스킬류 중복 방지 코드)-다뽑아달라해서 주석
                        {
                            pre_skill_class_code = skill_info->m_SkillClassCode;
                            resource_code = GetCharacterResourceCode(skill_info->m_dwClassDefine);
                            animation_code = (skill_info->m_wzidAnimationCode[0].id);
                            WzIDToString(code, animation_code);
                            code[5] = '\0';

                            if (Stricmp("NULL", code) == 0)
                            {
                                // m_SkillClassCode, 애니코드가 없습니다., resource_code, AniCode, SkillName, ClassDefine
                                fprintf(fp, "%d\t%d\t애니코드가 없습니다.\tResource_code(%d)\t%s\t%s\t%d\n", 
                                    skill_info->m_SkillCode, skill_info->m_SkillClassCode, 0, code, 
                                    skill_info->m_szSkillName, skill_info->m_dwClassDefine);
                                skill_info = SkillInfoParser::Instance()->GetNextSkill();
                                continue;
                            }

                            RESOURCE_INFO* res_info = GraphicResourceList::Instance()->GetItemInfo(resource_code);
                            if (res_info != NULL)
                            {
                                if (resource_code != pre_resource_code)// 이전과 현재가 다른 경우 새로 로드
                                {
                                    if (pre_resource_code != 0)// 이전 WzUnit가 0이 아니면 해제
                                    {
                                        g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(temp_id); 
                                    }
                                    temp_id = g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit(temp_resource_id, res_info->szFilename);
                                    unit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(temp_id);
                                    pre_resource_code = resource_code;
                                }

                                if (unit != NULL)
                                {
                                    float custom_frame = -1.0f;
                                    WzAnimationInfo* animation_info = unit->GetAnimationInfo(animation_code);
                                    float max_frame = (float)g_pSunRenderer->GetNumFrameOfAnimation(animation_info->m_hWzdAni) - 1;
                                    // 2014.09.03 스킬 플레이타임 추가
                                    float play_time = (max_frame*1000/30) / animation_info->m_fAnimationSpeed; // (프레임출력시간/재생속도) => 애니메이션 재생시간
                                    play_time = (play_time)?floor(play_time+0.5f):ceil(play_time-0.5f);
                                    max_frame = (max_frame*1000/30) * animation_info->m_fAnimationSpeed;//스킬총 프레임 => tick
                                    max_frame = (max_frame)?floor(max_frame+0.5f):ceil(max_frame-0.5f);
                                    int max_custom_count = animation_info->GetNumCustomInfo();
                                    if (max_custom_count > 0)
                                    {
                                        WzCustomInfo* custom_info_ptr = animation_info->GetCustomInfo(0);
                                        if (custom_info_ptr != NULL)
                                        {
                                            custom_frame = custom_info_ptr->m_fCustomFrame;
                                            custom_frame = (custom_frame*1000/30) * animation_info->m_fAnimationSpeed;//첫 event의 첫번째 임펙트 프레임 => tick
                                            custom_frame = (custom_frame)?floor(custom_frame+0.5f):ceil(custom_frame-0.5f);
                                        }
                                    }
                                    // m_SkillClassCode, MaxTick, AttacTick, AniPlayTime, AniCode, SkillName, ClassDefine
                                    fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%s\t%s\t%d\n", skill_info->m_SkillCode,
                                        skill_info->m_SkillClassCode, (int)(max_frame), (int)(custom_frame),(int)(play_time), code,
                                        skill_info->m_szSkillName, skill_info->m_dwClassDefine);
                                }
                                else
                                {
                                    // m_SkillClassCode, 리소스가 없습니다., resource_code, AniCode, SkillName, ClassDefine
                                    fprintf(fp, "%d\t%d\t리소스가 없습니다.\tResource_code(%d)\t%s\t%s\t%d\n", 
                                        skill_info->m_SkillCode, skill_info->m_SkillClassCode, resource_code, code,
                                        skill_info->m_szSkillName, skill_info->m_dwClassDefine);
                                }
                            }
                            else
                            {
                                // m_SkillClassCode, 리소스코드가 없습니다., resource_code, AniCode, SkillName, ClassDefine
                                fprintf(fp, "%d\t%d\t리소스코드가 없습니다.\tResource_code(%d)\t%s\t%s\t%d\n", 
                                    skill_info->m_SkillCode, skill_info->m_SkillClassCode, resource_code, code, 
                                    skill_info->m_szSkillName, skill_info->m_dwClassDefine);
                            }
                        }
                        /*else
                        {   // 이전 스킬코드와 현재 스킬코드가 같으면
                        }*/
                    }
                    skill_info = SkillInfoParser::Instance()->GetNextSkill();
                }
                g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(temp_id); //마지막 남은 unit을 지운다
                fflush(fp);
                fclose(fp);								
			}
			else 
			{
				//	파라미터 갯수가 잘못 되었습니다.
				pDlg->AddString(pszString);
				g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
				pDlg->AddString( szMessage );
			}
			return TRUE;
		
		}
//_SCJ_GM_CMD_CREATE_ITEM_TO
        else if((0 == Stricmp(szDevCommand, _T("//연속생성"))) || (0 == Stricmp(szDevCommand, _T("//maketo"))))
        {
            SendMaketoPacket(pszString);
            
            return TRUE;
        }
        else if ((0 == Stricmp(szDevCommand, _T("//지역점령전채널"))) || 
                 (0 == Stricmp(szDevCommand, _T("//dominationchannel"))))
        {
            int channel_id = atoi(szTemp);
            if (channel_id > 0)
            {
                PROGRAMPARAM->SetDominationChannel(channel_id);
            }            
        }
    #ifdef _NA_003027_20111013_HONOR_SYSTEM
        else if (0 == Stricmp(szDevCommand, _T("//title_change")))
        {
            int title_code = atoi(szTemp);
            HonorSystem::HonorSystemFunction::UseTitleNameForHero(title_code);
        }
        else if (0 == Stricmp(szDevCommand, _T("//fame_point")))
        {
            int fame_code;
            int fame_point;
            _stscanf(pszString,_T("%s %d %d"), szTemp, &fame_code, &fame_point);
            HonorSystem::HonorSystemFunction::OnChangeFamePoint(fame_code, (DWORD)fame_point);
            HonorSystem::GetHonorSystemData().ChangeFamePoint(fame_code, (DWORD)fame_point);
        }
        else if (0 == Stricmp(szDevCommand, _T("//repute_point")))
        {
            int repute_code;
            int repute_point;
            _stscanf(pszString,_T("%s %d %d"), szTemp, &repute_code, &repute_point);
            HonorSystem::HonorSystemFunction::OnChangeReputePoint(repute_code, (DWORD)repute_point);
            HonorSystem::GetHonorSystemData().ChangeReputePoint(repute_code, (DWORD)repute_point);
        }
    #endif //_NA_003027_20111013_HONOR_SYSTEM
    #ifdef _NA_007086_20140318_MONSTERBOOK
        else if (0 == Stricmp(szDevCommand, _T("//몬스터도감")) || (0 == Stricmp(szDevCommand, _T("//monsterbook"))))
        {
            uiMonsterBookMan* monsterbook_man = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
            if (monsterbook_man == NULL)
            {
                return FALSE;
            }

            if (_stscanf(pszString,_T("%s %s"),szTemp, szTemp2)==2)
            {
                if (!Stricmp(szTemp2,_T("on")) || !Stricmp(szTemp2,_T("켬")))
                {
                    monsterbook_man->ActiveMonsterIconAll(TRUE);
                }
                else if (!Stricmp(szTemp2,_T("off")) || !Stricmp(szTemp2,_T("끔")))
                {
                    monsterbook_man->ResetMonsterBookData();
                }
            }
            else
            {
                monsterbook_man->ActiveMonsterIconAll(TRUE);
            }
        }
    #endif //_NA_007086_20140318_MONSTERBOOK
    #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        else if (0 == Stricmp(szDevCommand, _T("//출석체크")) || (0 == Stricmp(szDevCommand, _T("//attendance"))))
        {
            int year=0, month=0;
            if (_stscanf(pszString,_T("%s %d %d"),szTemp, &year, &month)==3)
            {
                uiAttendanceCheckDialog* ui_attendance_check_dialog = 
                    GET_CAST_DIALOG(uiAttendanceCheckDialog, IM_ATTENDANCE_CHECK_MANAGER::ATTENDANCE_CHECK_DIALOG);
         
                if (ui_attendance_check_dialog)
                {
                    ui_attendance_check_dialog->set_default_month(month);
                    ui_attendance_check_dialog->UpdateCalendar(year, month);
                }
            }
        }
    #endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif//_DEV_VER
	}

	return FALSE;
}
#ifdef _DEV_VER //#ifdef _SCJ_GM_CMD_CREATE_ITEM_TO
bool DebugCommandManager::SendMaketoPacket(TCHAR* command_string)
{  
    TCHAR error_string[MAX_MESSAGE_LENGTH] = {0,};
    TCHAR command[MAX_MESSAGE_LENGTH];
    TCHAR temp_buffer[MAX_MESSAGE_LENGTH];
    int from_item_code;
    int to_item_code;
    _stscanf(command_string,"%s %d %d %s",command,&from_item_code,&to_item_code,temp_buffer);

    const int maketo_max_count = 15; //한번에 15개까지 생성가능
    int count = 0;
    if((0 == Stricmp(command, _T("//연속생성"))) || (0 == Stricmp(command, _T("//maketo"))))
    {
        TCHAR message_string[MAX_MESSAGE_LENGTH];
        TCHAR* first_item_str = NULL;
        TCHAR* second_item_str = NULL;
        TCHAR* message = NULL;

        first_item_str = util::GetToken( command_string, temp_buffer, _T(" ") );
        if (first_item_str && _ttoi(first_item_str) != 0)
        {
            second_item_str = util::GetToken( first_item_str, temp_buffer, _T(" ") );
        }
        if (second_item_str && _ttoi(second_item_str) != 0)
        {
            message = util::GetToken( second_item_str, temp_buffer, _T(" ") );
        }
        if (message == NULL || _ttoi(message) <= 0 || from_item_code <= 0 || to_item_code <= 0)
        {
            Sprintf(error_string, "연속생성 매개변수가 잘못되었습니다.");
        }

        else
        {
            int start = min(from_item_code,to_item_code);
            int end = max(from_item_code,to_item_code);
            for (int i=start; i<=end; ++i, ++count)
            {
                if (count>=maketo_max_count)
                {
                    Sprintf(error_string, "한번에 15개 까지 생성할 수 있습니다.%d부터 %d까지 생성하였습니다.",
                        start,start+maketo_max_count-1);
                    break;
                }
                _stprintf(message_string,"make %d%s",i,message);
                MSG_CG_GM_STRING_CMD_SYN msg;
                ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                StrnCopy(msg.m_szStringCmd,message_string,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
            }
        }
    }
    
    if (error_string != "\0")
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, error_string);
    }

    return (count>0)?true:false;      
}
#endif//_DEV_VER
int DebugCommandManager::GetCharacterResourceCode(DWORD class_define_code)
{
    int resource_code = 0;
    switch (class_define_code)
    {
    case eCHAR_BERSERKER:
    case (eCHAR_BERSERKER+10):  // 체인지업 캐릭터 +10
        resource_code = 40001;
        break;
    case eCHAR_DRAGON:
    case (eCHAR_DRAGON+10):  // 체인지업 캐릭터 +10
        resource_code = 40002;
        break;
    case eCHAR_SHADOW:
    case (eCHAR_SHADOW+10):  // 체인지업 캐릭터 +10
        resource_code = 40003;
        break;
    case eCHAR_VALKYRIE:
    case (eCHAR_VALKYRIE+10):  // 체인지업 캐릭터 +10
        resource_code = 40004;
        break;
    case eCHAR_MAGICIAN:
    case (eCHAR_MAGICIAN+10):  // 체인지업 캐릭터 +10
        resource_code = 40005;
        break;
    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case (eCHAR_MYSTIC + 10):
        resource_code = 40012;
        break;
    case eTRANSFORM_TYPE_DRAGON1:
    case (eTRANSFORM_TYPE_DRAGON1+10):  // 체인지업 캐릭터 +10
        resource_code = 59051;
        break;
    case eTRANSFORM_TYPE_DRAGON2:
    case (eTRANSFORM_TYPE_DRAGON2+10):  // 체인지업 캐릭터 +10
        resource_code = 59052;
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case (eCHAR_HELLROID+10):  // 체인지업 캐릭터 +10
        resource_code = 40013;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case (eCHAR_WITCHBLADE+10):  // 체인지업 캐릭터 +10
        resource_code = 40015;
        break;
    default:
        resource_code = 0;
        break;
    }
    return resource_code;
}

#ifdef _DEV_VER
void DebugCommandManager::OpenGMWin(BYTE gm_window_type)
{
    uiGMWindowMan* ui_gm_window_manager_ptr =
        GET_DIALOG_MANAGER(uiGMWindowMan, UIMAN_GM_WINDOW);

    if (ui_gm_window_manager_ptr)
    {
        ui_gm_window_manager_ptr->ToggleGMWindow(gm_window_type);
    }
}

void DebugCommandManager::ShowGMWin(BYTE gm_window_type, bool value)
{
    uiGMWindowMan* ui_gm_window_manager_ptr =
        GET_DIALOG_MANAGER(uiGMWindowMan, UIMAN_GM_WINDOW);

    if (ui_gm_window_manager_ptr)
    {
        ui_gm_window_manager_ptr->ShowGMWindow(gm_window_type, value);
    }
}

void DebugCommandManager::ItemSearch(TCHAR* search_word)
{
    uiGMWindowMan* ui_gm_window_manager_ptr =
        GET_DIALOG_MANAGER(uiGMWindowMan, UIMAN_GM_WINDOW);

    if (ui_gm_window_manager_ptr)
    {
        ui_gm_window_manager_ptr->ItemSearch(search_word);
    }
}
#endif _DEV_VER

//------------------------------------------------------------------------------ 
bool DebugCommandManager::Check_MashGroup()
{
    TCHAR szFName[_MAX_FNAME] = { 0, };
    TCHAR szExt[_MAX_EXT] = { 0, };

    FILE* wirte_fp = fopen("check_wzm_meshgroup.txt", "wt");
    if (wirte_fp == NULL)
    {
        return false;
    }
    fprintf(wirte_fp, "================================================\n");
    fprintf(wirte_fp, "== wzm 파일 mesh group check ==\n");
    fprintf(wirte_fp, "== 1. 리소스가 없습니다. => GraphicResourceList.txt의 목록에는 있지만 실제로 리소스가 없을경우. ==\n");
    fprintf(wirte_fp, "== 2. 메쉬그룹이 없습니다. => 해당 wzm 파일에 메쉬 그룹이 설정되지 않은 상태 ==\n");
    fprintf(wirte_fp, "== 위 2가지 내용은 모두 수정되어야 할 사항입니다. ==\n");
    fprintf(wirte_fp, "================================================\n");


    util::SolarHashTable<RESOURCE_INFO *>::iterator begin = GraphicResourceList::Instance()->Begin();
    util::SolarHashTable<RESOURCE_INFO *>::iterator end = GraphicResourceList::Instance()->End();
    for (; begin != end; ++begin)
    {
        RESOURCE_INFO* res_info = *begin;
        if (res_info == NULL)
        {
            continue;;
        }

        //! 1. 파일 유무 체크
        FILE *fp = fopen(res_info->szFilename,"rb");
        if (fp == NULL)
        {
            //fprintf(wirte_fp, "리소스가 없습니다.\t%d\t%s\n", res_info->dwCode, res_info->szFilename);
            continue;
        }
        fclose(fp);


        _splitpath(res_info->szFilename, NULL, NULL, szFName, szExt);
        if (_tcsicmp(szExt, _T(".wzu")) != 0)
        {
            continue;
        }

        //! 2. wzu 파일 메쉬 그룹 체크
        const int temp_resource_id = SCRIPT_RESOURCE_ID_OFFSET + 120004;
        WzID temp_id = g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit(temp_resource_id, res_info->szFilename, TRUE, 
                                                                            ULS_CHECK_VALID_ANIMATION | ULS_NO_MULTITHREAD);
        CWzUnit* unit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(temp_id);
        if (unit == NULL)
        {
            continue;
        }

        for( int i = 0; i <= WZ_MAX_LOD; ++i )
        {
            CWzdBase* wzd_base = unit->GetWzdPtr(i);
            if (wzd_base == NULL)
            {
                continue;;
            }

            //! 2.1 wzm 파일만 메쉬 그룹이 있는지 체크 한다.
            _splitpath(wzd_base->GetFileName(), NULL, NULL, szFName, szExt);
            if (_tcsicmp(szExt, _T(".wzm")) != 0)
            {
                continue;
            }
            if ((wzd_base->GetNumberOfMeshGroup() <= 0) 
#if WZENGINEVER >= 400
                || (wzd_base->GetMaxMeshGroup() <= 0)
#endif //#if WZENGINEVER >= 400
                )
            {
                fprintf(wirte_fp, "메쉬그룹이 없습니다.\tLOD:%d\t%s\n", i, wzd_base->GetFileName());
            }
            else
            {
                // 유닛 서브 타입 얻기
                for (int sub_index = 0; sub_index < unit->GetNumSubType(); ++sub_index)
                {
                    CWzUnitSubType* unit_sub_type = unit->GetSubType(sub_index);

                    for (int render_index = 0; render_index < unit_sub_type->GetNumRenderInfo(); ++render_index)
                    {
                        const WzRenderInfo* render_info = unit_sub_type->GetRenderInfo(render_index);

#if WZENGINEVER >= 400
                        if (render_info->m_iMesh >= wzd_base->GetMaxMeshGroup())
                        {
                            fprintf(wirte_fp, "메쉬그룹[%d]이 없습니다.\tLOD:%d\t%s\n", render_info->m_iMesh, i, wzd_base->GetFileName());
                        }
#endif //#if WZENGINEVER >= 400
                    }
                }
            }
        }

        g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(temp_id);
    }

    fclose(wirte_fp);

    return true;
}

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER     // 130311 Texture_Viewer 작업 -홍성현-
void DebugCommandManager::OpenTextureViewerWin(BYTE gm_window_type)
{
	uiTextureViewerMan* ui_texture_viewer_manager_ptr =
		GET_DIALOG_MANAGER(uiTextureViewerMan, UIMAN_TEXTURE_VIEWER_MANAGER);    

    if(gm_window_type == UI_TEXTURE_VIEWER_SEARCH_WINDOW)
    {
        ui_texture_viewer_manager_ptr->textureviewer_search_start();
    }

	if (ui_texture_viewer_manager_ptr)
	{        
		ui_texture_viewer_manager_ptr->ToggleTextureViewerWindow(gm_window_type);
	}
}
#endif
#endif