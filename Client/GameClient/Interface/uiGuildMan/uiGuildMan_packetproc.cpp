//------------------------------------------------------------------------------
//  uiGuildMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMan.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildMain/uiGuildMain.h"
#include "ObjectManager.h"
#include "ResultCode.h"
#include "AskReplyGuildDialog.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "uiHelpMan/uiHelpMan.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "uiAssistMan/uiAssistMan.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiGuildAllianceRequest/uiGuildAllianceRequest.h"
#include "GuildRankInfoParser.h"
#include "GuildRightInfoParser.h"
#include "uiGuildRightDelegation/uiGuildRightDelegation.h"
#include "uiGuildMemberRightView/uiGuildMemberRightView.h"
#include "uisystemman/uiSystemMan.h"
#include "GuildRankInfoParser.h"
#include "Application.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "GuildSystem/uiGuildWareHouse.h"
#else
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiGuildWareHouseRight/uiGuildWareHouseRight.h"
#include "uiGuildWareHouseLog/uiGuildWareHouseLog.h"
#include "BitStream.hpp"
#include "InputNumBankDialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "uiChangeGuildName/uiChangeGuildName.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#include "uiConvenienceStore/uiConvenienceStore_FriendList.h"

using namespace RC;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void _callback_Guild_Relation_Request(bool bYes) //동맹 신청 요구에 응할것이냐.
{
	if (bYes)
	{
		MSG_CG_GUILD_RELATION_ANSWER_SYN SendPacket;
		SendPacket.m_eAnswerType = eGUILD_ANSWER_YES;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));		
	}
	else
	{
		MSG_CG_GUILD_RELATION_ANSWER_SYN SendPacket;
		SendPacket.m_eAnswerType = eGUILD_ANSWER_NO;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::NetworkProc( MSG_BASE * pMsg )
{
	if ( (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT)
		|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ) )
		return;

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_GUILD:
        {
            switch (pMsg->m_byProtocol)
            {
                
                //
                //  protocol: CG_GUILD_CREATE_SYN
                //  길드 생성
                //

            case CG_GUILD_CREATE_ACK:
                {
                    MSG_CG_GUILD_CREATE_ACK *recvMsg =
                        (MSG_CG_GUILD_CREATE_ACK *)pMsg;

					if(!g_pHero)
						return;

					g_InterfaceManager.GetInterfaceString( eST_SUCCESS_CREATE_GUILD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

					if (helpMan)
					{
						helpMan->CheckHelpMessage(eHK_GUILD);
					}

                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kCreateGuild);
                    }

                    MONEY orgMoney = g_pHero->GetMoney();
					GuildRankInfo	* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( GRADE_AT_GUILD_CREATE );
					if( pRankInfo != NULL )
						orgMoney -= pRankInfo->m_dwHeim;

                    g_pHero->SetMoney(orgMoney);
					uiGuildCreate* guildCreate = GET_CAST_DIALOG( uiGuildCreate, IM_GUILD_MANAGER::GUILD_CREATE);
					assert (guildCreate);
                    g_pHero->SetGuildName( guildCreate->m_szGuildName, MAX_GUILDNAME_LENGTH);

					this->SEND_CG_GUILD_TOTALINFO_REQ_SYN();
					this->DelRelationCon();
                }
                break;

            case CG_GUILD_CREATE_NAK:
                {
                    MSG_CG_GUILD_CREATE_NAK *recvMsg =
                        (MSG_CG_GUILD_CREATE_NAK *)pMsg;

					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
                }
                break;

            case CG_GUILD_CREATE_BRD:
                {
                    MSG_CG_GUILD_CREATE_BRD *recvMsg =
                        (MSG_CG_GUILD_CREATE_BRD *)pMsg;

                    PLAYERKEY	m_MasterPlayerKey = recvMsg->m_MasterPlayerKey;							//< 길드 창설한 플레이어 키
					
                    DWORD       dwTargetKey = 0;
                    Object *    pTargetObj = NULL;

#ifdef	_K2P_OBJECT_MANAGER_MODIFIDY
					pTargetObj = g_ObjectManager.GetObject( m_MasterPlayerKey );
#else
                    bool bFounded = false;

                    Object_Map_Itr	itr = g_ObjectManager.GetBegin();

                    while( g_ObjectManager.GetEnd() != itr )
                    {
                        Object *pObj = (Object *)itr->second;

                        if (pObj->GetObjectKey() == m_MasterPlayerKey)
                        {
                            dwTargetKey = pObj->GetObjectKey();
                            pTargetObj = pObj;
                            bFounded = true;
                            break;
                        }
                        ++itr;
                    }
#endif

                    if( pTargetObj != NULL )
                    {
                        Player* pPlayerObj = (Player *)pTargetObj;
                        assert (pPlayerObj);

                        pPlayerObj->SetGuildName(recvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH);
                    }
                }
                break;

                //
                //  protocol: CG_GUILD_DESTROY_SYN
                //  길드장만 사용 가능한 길드해체
                //

            case CG_GUILD_DESTROY_ACK:
                {
                    MSG_CG_GUILD_DESTROY_ACK *recvMsg =
                        (MSG_CG_GUILD_DESTROY_ACK *)pMsg;

					//	길드를 해산 하셨습니다.
					g_InterfaceManager.GetInterfaceString( eST_DESTROY_GUILD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    this->Clear();

                    m_pUIGuildMenu->ShowInterface(FALSE);
                    m_pUIGuildMain->ShowInterface(FALSE);
                }
                break;

            case CG_GUILD_DESTROY_NAK:
                {
                    MSG_CG_GUILD_DESTROY_NAK *recvMsg =
                        (MSG_CG_GUILD_DESTROY_NAK *)pMsg;

					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
                    //GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("'%d' 에러코드와 함께 NAK가 왔습니다."), recvMsg->m_byErrorCode);
                }
                break;

            case CG_GUILD_DESTROY_BRD:
                {
                    MSG_CG_GUILD_DESTROY_BRD *recvMsg =
                        (MSG_CG_GUILD_DESTROY_BRD *)pMsg;

                    PLAYERKEY	m_MasterPlayerKey = recvMsg->m_MasterPlayerKey;							//< 길드 해산한 플레이어 키

                    DWORD       dwTargetKey = 0;
                    Object *    pTargetObj = NULL;

                    bool bFounded = false;

                    Object_Map_Itr	itr = g_ObjectManager.GetBegin();

                    while( g_ObjectManager.GetEnd() != itr )
                    {
                        Object *pObj = (Object *)itr->second;

                        if (pObj->GetObjectKey() == m_MasterPlayerKey)
                        {
                            dwTargetKey = pObj->GetObjectKey();
                            pTargetObj = pObj;
                            bFounded = true;
                            break;
                        }
                        ++itr;
                    }

                    if (pTargetObj)
                    {
                        Player* pPlayerObj = (Player *)pTargetObj;
                        assert (pPlayerObj);

                        pPlayerObj->ClearGuildRenderInfo();
                    }
                }
                break;

                //
                //  protocol: CG_GUILD_INVITE_JOIN_SYN
                //  길드 초대
                //

            case CG_GUILD_JOIN_FAILED_CMD:
                {
                    MSG_CG_GUILD_JOIN_FAILED_CMD *recvMsg =
                        (MSG_CG_GUILD_JOIN_FAILED_CMD *)pMsg;

					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
                    //GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("'%d' 에러코드와 함께 NAK가 왔습니다."), recvMsg->m_byErrorCode);
                }
                break;

			//길드 초대 받는 사람에게 CMD(길드 초대 창 Show)
			case CG_GUILD_INVITE_CMD:
				{
					MSG_CG_GUILD_INVITE_CMD * pRecvMsg = (MSG_CG_GUILD_INVITE_CMD *) pMsg;
					DWORD	dwUserKey;

					dwUserKey = pRecvMsg->m_OperUserKey;
					TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetInterfaceString( eST_NAMECODE_GUILD_INVITE_YOU, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage, pRecvMsg->m_OperCharName );

					uiAssistMan * pMan = GET_DIALOG_MANAGER( uiAssistMan, UIMAN_ASSIST );

					if( pMan )
						pMan->ShowAskReplyGuildDlg( dwUserKey );
				}
				break;
			//길드 초대 결과 CMD(메세지 출력)
			case CG_GUILD_INVITE_ANSWER_CMD:
				{
					MSG_CG_GUILD_INVITE_ANSWER_CMD * pRecvMsg = (MSG_CG_GUILD_INVITE_ANSWER_CMD *) pMsg;

					Player * pTargetUser = (Player *) g_ObjectManager.GetObject( pRecvMsg->m_TargetObjKey );

					TCHAR szMessage[INTERFACE_STRING_LENGTH] = { '\0',};

					if( pRecvMsg->m_bInveite )
					{
						if( pTargetUser )
						{
							pTargetUser->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
						}

						MSG_CG_GUILD_JOIN_SYN SendPacket;
						SendPacket.m_TargetObjKey = pRecvMsg->m_TargetObjKey;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(6260), pRecvMsg->m_TargetCharName );
					}
					else
					{
						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(6262), pRecvMsg->m_TargetCharName );
					}

					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				}
				break;

			case CG_GUILD_JOIN_CMD:
				{
					//	길드 가입을 성공했습니다.
					g_InterfaceManager.GetInterfaceString( eST_SUCCESS_JOIN_GUILD, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					SEND_CG_GUILD_TOTALINFO_REQ_SYN();
				}
				break;

			case CG_GUILD_INVITE_NAK:
				{
					MSG_CG_GUILD_INVITE_NAK * pRecvMsg = (MSG_CG_GUILD_INVITE_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_INVITE_ANSWER_NAK:
				{
					MSG_CG_GUILD_INVITE_ANSWER_NAK * pRecvMsg = (MSG_CG_GUILD_INVITE_ANSWER_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;


			case CG_GUILD_LOGIN_CHAR_CMD:
				{
					MSG_CG_GUILD_LOGIN_CHAR_CMD * pRecvMsg = (MSG_CG_GUILD_LOGIN_CHAR_CMD *) pMsg;
					SetGuildInfo(pRecvMsg->m_GuildInfo);
					memcpy(&m_GuildCorpsTotalInfo, &pRecvMsg->m_CorpsTotalInfo, sizeof(GUILD_CORPS_TOTAL_INFO));
					// 길드 멤버 정보
					m_vMembers.clear();
					m_vMembers.push_back(pRecvMsg->m_MemberInfo);

                    // 캐릭터 최초 접속시 변경된 길드공지가 있는지 요청
                    MSG_CW_GUILD_SELECT_NOTICE_SYN SendNoticePacket;
                    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendNoticePacket, sizeof(SendNoticePacket));
				}
				break;


			case CG_GUILD_LOGIN_CHAR_BRD:
				{
					MSG_CG_GUILD_LOGIN_CHAR_BRD * pRecvMsg = (MSG_CG_GUILD_LOGIN_CHAR_BRD *) pMsg;
					TCHAR tszCharName[MAX_CHARNAME_LENGTH+1] = {0,};
					StrnCopy(tszCharName, pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);

					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(tszCharName);
					if( pMemberInfo )
					{
						pMemberInfo->m_bLogin = 1;
					}
					if( g_pHero && StrnCmp(g_pHero->GetName() , tszCharName, MAX_CHARNAME_LENGTH) != 0)
					{
						TCHAR szMessge[INTERFACE_STRING_LENGTH] = {0,};
						//6263	길드원 %s 님이 로그인하였습니다.
						Snprintf(szMessge, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6263), tszCharName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessge);
					}
				}
				break;

			case CG_GUILD_LOGOUT_CHAR_BRD:
				{
					MSG_CG_GUILD_LOGOUT_CHAR_BRD * pRecvMsg = (MSG_CG_GUILD_LOGOUT_CHAR_BRD *) pMsg;
					TCHAR tszCharName[MAX_CHARNAME_LENGTH+1] = {0,};
					StrnCopy(tszCharName, pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);

					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(tszCharName);
					if( pMemberInfo )
					{
						pMemberInfo->m_bLogin = 0;
					}
					if( g_pHero && StrnCmp(g_pHero->GetName() , tszCharName, MAX_CHARNAME_LENGTH) != 0)
					{
						TCHAR szMessge[INTERFACE_STRING_LENGTH+1] = {0,};
						//6264	길드원 %s 님이 로그아웃하였습니다.
						Snprintf(szMessge, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6264), tszCharName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessge);
					}
				}
				break;

			case CG_GUILD_CHAR_LEVELUP_BRD:
				{
					MSG_CG_GUILD_CHAR_LEVELUP_BRD * pRecvMsg =(MSG_CG_GUILD_CHAR_LEVELUP_BRD *) pMsg;

					TCHAR tszCharName[MAX_CHARNAME_LENGTH+1] = {0,};
					StrnCopy(tszCharName, pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);

					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(tszCharName);
					if( pMemberInfo )
					{
						pMemberInfo->m_Level = pRecvMsg->m_Level;
					}
				}
				break;

			case CG_GUILD_JOIN_BRD:
				{
					MSG_CG_GUILD_JOIN_BRD * pRecvMsg = (MSG_CG_GUILD_JOIN_BRD *) pMsg;
					_addGuildMember(pRecvMsg->m_MemberInfo);
					//5208	%s님이 길드에 가입하셨습니다.
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(5208, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pRecvMsg->m_MemberInfo.m_tszCharName);
				}
				break;

			case CG_GUILD_JOIN_NAK:
				{
					MSG_CG_GUILD_JOIN_NAK * pRecvMsg = ( MSG_CG_GUILD_JOIN_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

            //
            //  protocol: CG_GUILD_WITHDRAW_SYN
            //

            case CG_GUILD_WITHDRAW_ACK:
                {
					MSG_CG_GUILD_WITHDRAW_ACK *recvMsg =
                        (MSG_CG_GUILD_WITHDRAW_ACK *)pMsg;

					m_pUIGuildMain->ShowInterface(FALSE);

					if( recvMsg->m_byPenaltyType == 3)
					{
						uiGuildMan::ProcessErrorCode(RC_GUILD_KICK_GUILD_BY_LEVELUP);
					}
					else
					{
						//	길드를 탈퇴 하셨습니다.
						g_InterfaceManager.GetInterfaceString( eST_GO_OUT_GUILD, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					SetRelationNoneByGuildMember();//<- 길드가 탈퇴됐으므로 주변 길드원과 관계를 끊자.
					this->Clear();					
                }
                break;

			case CG_GUILD_WITHDRAW_BRD:
				{
					MSG_CG_GUILD_WITHDRAW_BRD * pRecvMsg = (MSG_CG_GUILD_WITHDRAW_BRD *) pMsg;

					TCHAR tszCharName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL 처리 확실히 하기 위하여
					StrnCopy(tszCharName, pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);
					
					VEC_GUILD_MEMBER_ITR itr = m_vMembers.begin();
					while( itr != m_vMembers.end() )
					{
						if(!Strcmp(tszCharName, (*itr).m_tszCharName ) )
						{
							m_vMembers.erase(itr);
							break;
						}
						++itr;
					}

					if( pRecvMsg->m_byPenaltyType == 3)
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
						//7225 %s 님이 31레벨에 달성하여 길드에서  탈퇴되었습니다.
						Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(7225) , tszCharName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}

					Player * pTargetObj =  (Player *) g_ObjectManager.GetObject(tszCharName);
					if( pTargetObj )
					{
						pTargetObj->ClearGuildRenderInfo();
					}

				}
				break;


            case CG_GUILD_WITHDRAW_NAK:
                {
                    MSG_CG_GUILD_WITHDRAW_NAK *recvMsg =
                        (MSG_CG_GUILD_WITHDRAW_NAK *)pMsg;

					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
					//	탈퇴에 장애가 발생하였습니다.\n\n개발팀이 수정중에 있으며,\n수정 후 공지 하도록 하겠습니다.\n불편을 끼쳐드려 대단히 죄송합니다.
					
                }
                break;

            //    //
            //    //  protocol: CG_GUILD_GRANT_LOW_POSITION_SYN
            //    //

            
			case CG_GUILD_KICKPLAYER_BRD:
				{
					MSG_CG_GUILD_KICKPLAYER_BRD * pRecvMsg = (MSG_CG_GUILD_KICKPLAYER_BRD *) pMsg;

					TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] ={0,};
					StrnCopy(szTargetName,pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);
					
					VEC_GUILD_MEMBER_ITR itr = m_vMembers.begin();
					while( itr != m_vMembers.end())
					{
						if( !StrnCmp(szTargetName, (*itr).m_tszCharName, MAX_CHARNAME_LENGTH) )
						{
							m_vMembers.erase(itr);
							break;
						}
						++itr;
					}

					Player * pTarget = (Player *) g_ObjectManager.GetObject(szTargetName);
					if( pTarget )
					{
						pTarget->ClearGuildRenderInfo();
					}

					////5171	%s'길드원이 제명되었습니다.
					g_InterfaceManager.GetInterfaceString(5171, szTemp, INTERFACE_STRING_LENGTH);
					Sprintf(szMessage, szTemp, szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_KICKPLAYER_CMD:
				{
					//5169	길드에서 제명되었습니다.	
					g_InterfaceManager.GetInterfaceString(5169, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					SetRelationNoneByGuildMember(); //<- 길드가 탈퇴됐으므로 주변 길드원과 관계를 끊자.
					m_pUIGuildMain->ShowInterface(FALSE);
					this->Clear();
				}
				break;



			case CG_GUILD_KICKPLAYER_NAK:	// 강제 탈퇴 실패(길드장에게만 전송)
				{
					MSG_CG_GUILD_KICKPLAYER_NAK *recvMsg =
						(MSG_CG_GUILD_KICKPLAYER_NAK *)pMsg;

					if((recvMsg->m_dwErrorCode == 0) || (recvMsg->m_dwErrorCode == RC_GUILD_CANNOT_KICK_STATE))
					{
						TCHAR tszKickCharName[MAX_CHARNAME_LENGTH+1] = {0,};
						StrnCopy(tszKickCharName, recvMsg->m_ptszKickCharName, MAX_CHARNAME_LENGTH);

						//5170	%s'길드원 제명에 실패했습니다.
						g_InterfaceManager.GetInterfaceString(5170, szTemp, INTERFACE_STRING_LENGTH);
						Sprintf(szMessage, szTemp, tszKickCharName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						uiGuildMan::ProcessErrorCode((BYTE)recvMsg->m_dwErrorCode);
					}
				}
				break;


			case CG_GUILD_RANKUP_ACK:
				{
					MSG_CG_GUILD_RANKUP_ACK * recvMsg = ( MSG_CG_GUILD_RANKUP_ACK *) pMsg;
					m_GuildInfo.m_GuildGrade	= recvMsg->m_byNowLevel;
					m_GuildInfo.m_GuildPoint	= recvMsg->m_RemainGP;
					m_GuildInfo.m_UserPoint		= recvMsg->m_RemainUP;
				
					g_pHero->SetMoney(recvMsg->m_RemainMoney);	// 길드 마스터에서 뺐어 온다.

					// 축하합니다! 길드 랭크업에 성공했습니다. 5181
					g_InterfaceManager.GetInterfaceString(eST_SUCCESS_GUILD_RANKUP, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage) ;
				}
				break;

			case CG_GUILD_RANKUP_BRD:
				{
					MSG_CG_GUILD_RANKUP_BRD * pRecvMsg = (MSG_CG_GUILD_RANKUP_BRD *) pMsg;
					m_GuildInfo.m_GuildGrade = pRecvMsg->m_byNowLevel;
					m_GuildInfo.m_GuildPoint = pRecvMsg->m_RemainGP;
					m_GuildInfo.m_UserPoint  = pRecvMsg->m_RemainUP;

					// 축하합니다! 길드 랭크업에 성공했습니다. 5181
					g_InterfaceManager.GetInterfaceString(eST_SUCCESS_GUILD_RANKUP, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage) ;

				}
				break;

			case CG_GUILD_DONATE_UP_BRD:
				{
					MSG_CG_GUILD_DONATE_UP_BRD * pRecvMsg = (MSG_CG_GUILD_DONATE_UP_BRD *) pMsg;
					m_GuildInfo.m_UserPoint = pRecvMsg->m_GuildUP;
				}
				break;


			case CG_GUILD_RANKUP_NAK:
				{
					MSG_CG_GUILD_RANKUP_NAK * recvMsg = ( MSG_CG_GUILD_RANKUP_NAK *) pMsg;
					// 길드 랭크업에 실패했습니다. 5182
					g_InterfaceManager.GetInterfaceString(eST_FAILED_GUILD_RANKUP , szMessage ,INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
				}
				break;


			case CG_GUILD_CONVERT_UP2GP_ACK:
				{
					MSG_CG_GUILD_CONVERT_UP2GP_ACK * recvMsg = ( MSG_CG_GUILD_CONVERT_UP2GP_ACK * ) pMsg;
					
					m_GuildInfo.m_GuildPoint = recvMsg->m_NowGP;
					m_GuildInfo.m_UserPoint  = recvMsg->m_RemainUP;
					// GP(길드포인트)로 변환되었습니다. 5183
					g_InterfaceManager.GetInterfaceString(eST_GP_CONVERT_SUCCESS , szMessage ,INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_CONVERT_UP2GP_BRD:
				{
					MSG_CG_GUILD_CONVERT_UP2GP_BRD * pRecvMsg = ( MSG_CG_GUILD_CONVERT_UP2GP_BRD * ) pMsg;
					m_GuildInfo.m_GuildPoint = pRecvMsg->m_NowGP;
					m_GuildInfo.m_UserPoint = pRecvMsg->m_RemainUP;
				}
				break;


			case CG_GUILD_CONVERT_UP2GP_NAK:
				{
					MSG_CG_GUILD_CONVERT_UP2GP_NAK * recvMsg = ( MSG_CG_GUILD_CONVERT_UP2GP_NAK * ) pMsg;
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);

				}
				break;

			case CG_GUILD_DONATE_UP_ACK:
				{
					MSG_CG_GUILD_DONATE_UP_ACK * recvMsg = ( MSG_CG_GUILD_DONATE_UP_ACK * ) pMsg;

					g_pHero->GetCharInfo()->m_UserPoint = recvMsg->m_RemainUP;
					m_GuildInfo.m_UserPoint = recvMsg->m_GuildUP;  // 길드UP는 따로 처리
					// UP(유저포인트)를 상납하였습니다. 5184
					g_InterfaceManager.GetInterfaceString(eST_DONATE_UP_SUCCESS , szMessage ,INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
					
				}
				break;
			
			case CG_GUILD_DONATE_UP_NAK:
				{
					MSG_CG_GUILD_DONATE_UP_NAK * recvMsg = ( MSG_CG_GUILD_DONATE_UP_NAK * ) pMsg;
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_ADD_INFO_ACK://길드관계 전체 정보 Send
				{
					MSG_CG_GUILD_ADD_INFO_ACK* recvMsg = (MSG_CG_GUILD_ADD_INFO_ACK*)pMsg;
					BYTE btRelCnt  = recvMsg->m_byRelCnt;
					BYTE btCorpCnt = recvMsg->m_byCorpsCnt;
					BASE_GUILD_ADD_INFO stGuildAddInfo = recvMsg->m_AddInfo;

					//싹지우고 새로 받는다.
					DelRelationCon(); 
					for(int i = 0; i < btRelCnt; ++i)
					{
						GUILD_RELATION* pGuildRelation = new GUILD_RELATION;
						memcpy(pGuildRelation, &stGuildAddInfo.m_stRelationGuild[i], sizeof(GUILD_RELATION));

						//////////////////////////////////////////////////////////////////////////
						//주변 캐릭터들의 m_RelationType을 세팅한다.
						Object_Map_Itr	itr = g_ObjectManager.GetBegin();
						while( g_ObjectManager.GetEnd() != itr )
						{
							Player* pPlayer = (Player*)itr->second;

							if(NULL == pPlayer)
							{
								++itr;
								continue;
							}

							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer->GetPlayerGuildID() == pGuildRelation->m_GuildGuid)
								pPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;

							++itr;
						}
						//////////////////////////////////////////////////////////////////////////
						eGUILD_RELATION eTemp = pGuildRelation->m_eRelationType;
						switch(eTemp)
						{
						case eGUILD_RELATION_ALLIENCE:
							{
								m_listGuildRelation_Alliance.push_back(pGuildRelation);
							}
							break;

						case eGUILD_RELATION_BOTH_HOSTILITY:
						case eGUILD_RELATION_PROCLAIM_HOSTILITY:
							{
								m_listGuildRelation_Hostility.push_back(pGuildRelation);
							}
							break;

						case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
							{
								m_listGuildRelation_OneSideHostility.push_back(pGuildRelation);
							}
							break;

						case eGUILD_RELATION_NONE:
						default:
							{
								SAFE_DELETE(pGuildRelation);
								assert(!"GUILD_RELATION_TYPE 잘못날라왔다");
							}
							break;

						} //switch
					} //for
				} //case
				break;

			case CG_GUILD_ADD_INFO_NAK:
				{
					MSG_CG_GUILD_ADD_INFO_NAK* recvMsg = (MSG_CG_GUILD_ADD_INFO_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_RELATION_NAK :
				{
					MSG_CG_GUILD_RELATION_NAK* recvMsg = (MSG_CG_GUILD_RELATION_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_RELATION_REQUEST_CMD:  //길마(또는 대행)가 동맹 관계 수락 여부 묻기
				{
					MSG_CG_GUILD_RELATION_REQUEST_CMD* recvMsg = (MSG_CG_GUILD_RELATION_REQUEST_CMD*)pMsg;

					TCHAR szGuildName[MAX_GUILDNAME_LENGTH + 1] = {0,};
					//YJ
					StrnCopy(szGuildName, recvMsg->m_RequestGuildName, MAX_GUILDNAME_LENGTH);

					uiGuildAllianceRequest* pGuildAllianceRequest = GET_CAST_DIALOG( uiGuildAllianceRequest, IM_GUILD_MANAGER::GUILD_ALLIANCE_REQUEST );
						
					//관계수락 여부를 묻는 경우는 동맹관계 뿐!
					if(pGuildAllianceRequest)
					{
						pGuildAllianceRequest->SetGuildName(szGuildName);
						pGuildAllianceRequest->SetGuildRelationType(recvMsg->m_eType);
						pGuildAllianceRequest->SetCharGUID(recvMsg->m_RequestUserKey);
						pGuildAllianceRequest->ShowInterface(TRUE);
					}

				}
				break;

			case CG_GUILD_RELATION_REJECT_ACK: //동맹 신청 거부당하다.
				{
					MSG_CG_GUILD_RELATION_REJECT_ACK* recvMsg = (MSG_CG_GUILD_RELATION_REJECT_ACK*)pMsg;
					TCHAR szGuildName[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
					StrnCopy(szGuildName, recvMsg->m_szRelGuildName, INTERFACE_STRING_LENGTH);
					//6177	%s 길드의 길드 마스터가 동맹 신청을 거절하였습니다.
					g_InterfaceManager.GetInterfaceString(6177, szMessage);
					Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szGuildName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);
				}
				break;

			case CG_GUILD_RELATION_DELETE_NAK:
				{
					MSG_CG_GUILD_RELATION_DELETE_NAK* recvMsg = (MSG_CG_GUILD_RELATION_DELETE_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
				}
				break;


			case CG_GUILD_RELATION_BRD: //관계 신청 수락했다.
				{
					MSG_CG_GUILD_RELATION_BRD* recvMsg = (MSG_CG_GUILD_RELATION_BRD*)pMsg;
					GUILD_RELATION* pGuildRelation = new GUILD_RELATION;
					memcpy(pGuildRelation, &recvMsg->m_stRELGuild, sizeof(GUILD_RELATION));

					//서버에서 본인의 길드를 보내는 일이있다. 거르자.(거르지않으면 자신의 길드와 동맹,적대관계를 맺는 상황이 발생)
					if(pGuildRelation->m_GuildGuid == GetGuildInfo().m_GuildGuid)
						return;

					TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

					switch(pGuildRelation->m_eRelationType)
					{
					case eGUILD_RELATION_ALLIENCE:
						{
							//6183 %s 길드와 동맹관계가 성립되었습니다.
							g_InterfaceManager.GetInterfaceString(6183, szTemp);

							m_listGuildRelation_Alliance.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_BOTH_HOSTILITY:
						{
							//6186 %s 길드와 쌍방 적대관계가 되었습니다.
							g_InterfaceManager.GetInterfaceString(6186, szTemp);

							m_listGuildRelation_Hostility.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_PROCLAIM_HOSTILITY:
						{
							//6184 %s 길드에게 적대 선포를 하였습니다.
							g_InterfaceManager.GetInterfaceString(6184, szTemp);

							m_listGuildRelation_Hostility.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
						{
							//6185 %s 길드와 일방적인 적대관계가 선포되었습니다.
							g_InterfaceManager.GetInterfaceString(6185, szTemp);

							m_listGuildRelation_OneSideHostility.push_back(pGuildRelation);
						}
						break;
					}

					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer->GetPlayerGuildID() == pGuildRelation->m_GuildGuid)
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
							}
						}
						++itr;
					}

					Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pGuildRelation->m_GuildName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);
				}
				break;

			case CG_GUILD_RELATION_BOTH_AGAINST_BRD: //쌍방적대관계
				{
					MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD* recvMsg = (MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD*)pMsg;

					GUILDGUID gGuid = recvMsg->m_BothGuildGuid;

					//서버에서 본인의 길드를 보내는 일이있다. 거르자.
					if(gGuid == GetGuildInfo().m_GuildGuid)
						return;

					if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == recvMsg->m_eDELType)//일방적대
					{
						std::list<GUILD_RELATION*>::iterator iter;
						for(iter = m_listGuildRelation_OneSideHostility.begin(); iter != m_listGuildRelation_OneSideHostility.end(); ++iter)
						{
							if(0 == m_listGuildRelation_OneSideHostility.size())
								break;

							GUILD_RELATION* pDelRelation = (*iter);
							GUILD_RELATION* pAddRelation = new GUILD_RELATION;
							memcpy(pAddRelation, pDelRelation, sizeof(GUILD_RELATION));

							if(pDelRelation->m_GuildGuid == gGuid)
							{
								//삭제
								SAFE_DELETE(pDelRelation);
								m_listGuildRelation_OneSideHostility.erase(iter);		
								//추가
								pAddRelation->m_eRelationType = eGUILD_RELATION_BOTH_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
								TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
								//6186 %s 길드와 쌍방 적대관계가 되었습니다.
								g_InterfaceManager.GetInterfaceString(6186, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);

								break;
							}
						}
					}
					else if(eGUILD_RELATION_PROCLAIM_HOSTILITY == recvMsg->m_eDELType)//적대 선포
					{
						std::list<GUILD_RELATION*>::iterator iter;
						for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
						{
							if(0 == m_listGuildRelation_Hostility.size())
								break;

							GUILD_RELATION* pDelRelation = (*iter);

							if(pDelRelation->m_GuildGuid == gGuid)
							{
								GUILD_RELATION* pAddRelation = new GUILD_RELATION;
								memcpy(pAddRelation, pDelRelation, sizeof(GUILD_RELATION));

								//삭제
								SAFE_DELETE(pDelRelation);
								m_listGuildRelation_Hostility.erase(iter);		
								//추가
								pAddRelation->m_eRelationType = eGUILD_RELATION_BOTH_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
								TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
								//6186 %s 길드와 쌍방 적대관계가 되었습니다.
								g_InterfaceManager.GetInterfaceString(6186, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);

								break;
							}
						}
					}
					else
					{
						assert(!"MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD 잘못 날라왔다");
					}

					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer && (pPlayer->GetPlayerGuildID() == gGuid))
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
							}
						}
						++itr;
					}
				}
				break;

			case CG_GUILD_RELATION_DELETE_BRD:
				{
					MSG_CG_GUILD_RELATION_DELETE_BRD* recvMsg = (MSG_CG_GUILD_RELATION_DELETE_BRD*)pMsg;

					GUILDGUID gGuid = recvMsg->m_RelGuildGuid;

					//서버에서 본인의 길드를 보내는 일이있다. 거르자.(거르지않으면 자신의 길드와 동맹,적대관계를 해제하는 상황이 발생)
					if(gGuid == GetGuildInfo().m_GuildGuid)
						return;

					TCHAR szGuildName[MAX_GUILDNAME_LENGTH + 1] = {0,};
					TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

					switch(recvMsg->m_eRelType)
					{
					case eGUILD_RELATION_ALLIENCE:
						{
							std::list<GUILD_RELATION*>::iterator iter;
							for(iter = m_listGuildRelation_Alliance.begin(); iter != m_listGuildRelation_Alliance.end(); ++iter)
							{
								if(0 == m_listGuildRelation_Alliance.size())
									break;

								GUILD_RELATION* pRelation = (*iter);

								if(pRelation->m_GuildGuid == gGuid)
								{
									StrnCopy(szGuildName, pRelation->m_GuildName, MAX_GUILDNAME_LENGTH);

									SAFE_DELETE(pRelation);
									m_listGuildRelation_Alliance.erase(iter);

									break;
								}
							}

							//5199	%s 길드와 동맹관계가 해제되었습니다.	
							g_InterfaceManager.GetInterfaceString(5199, szTemp, INTERFACE_STRING_LENGTH);
						}
						break;

					case eGUILD_RELATION_BOTH_HOSTILITY:
					case eGUILD_RELATION_PROCLAIM_HOSTILITY:
						{
							std::list<GUILD_RELATION*>::iterator iter;
							for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
							{
								if(0 == m_listGuildRelation_Hostility.size())
									break;

								GUILD_RELATION* pRelation = (*iter);

								if(pRelation->m_GuildGuid == gGuid)
								{
									StrnCopy(szGuildName, pRelation->m_GuildName, MAX_GUILDNAME_LENGTH);

									SAFE_DELETE(pRelation);
									m_listGuildRelation_Hostility.erase(iter);

									break;
								}
							}

							//6187 %s 길드와 적대관계가 해제 되었습니다.
							g_InterfaceManager.GetInterfaceString(6187, szTemp, INTERFACE_STRING_LENGTH);
						}
						break;

					case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
						{
							std::list<GUILD_RELATION*>::iterator iter;
							for(iter = m_listGuildRelation_OneSideHostility.begin(); iter != m_listGuildRelation_OneSideHostility.end(); ++iter)
							{
								if(0 == m_listGuildRelation_OneSideHostility.size())
									break;

								GUILD_RELATION* pRelation = (*iter);

								if(pRelation->m_GuildGuid == gGuid)
								{
									StrnCopy(szGuildName, pRelation->m_GuildName, MAX_GUILDNAME_LENGTH);

									SAFE_DELETE(pRelation);
									m_listGuildRelation_OneSideHostility.erase(iter);

									break;
								}
							}

							//6187 %s 길드와 일방적인 적대관계 해제
							g_InterfaceManager.GetInterfaceString(6187, szTemp, INTERFACE_STRING_LENGTH);
						}
						break;
					}

					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer && (pPlayer->GetPlayerGuildID() == gGuid))
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
							}
						}
						++itr;
					}

					Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, szGuildName );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);
				}
				break;

			case  CG_GUILD_RELATION_MOVE_BRD:
				{
					MSG_CG_GUILD_RELATION_MOVE_BRD* recvMsg = (MSG_CG_GUILD_RELATION_MOVE_BRD*) pMsg;

					GUILDGUID gGuid = recvMsg->m_GuildGuid;

					//서버에서 본인의 길드를 보내는 일이있다. 거르자.
					if(gGuid == GetGuildInfo().m_GuildGuid)
						return;

					std::list<GUILD_RELATION*>::iterator iter;
					for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
					{
						if(0 == m_listGuildRelation_Hostility.size())
							break;

						GUILD_RELATION* pDelRelation = (*iter);

						if(pDelRelation->m_GuildGuid == gGuid)
						{
							TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
							TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

							GUILD_RELATION* pAddRelation = new GUILD_RELATION;
							memcpy(pAddRelation, pDelRelation, sizeof(GUILD_RELATION));
							//삭제
							SAFE_DELETE(pDelRelation);
							m_listGuildRelation_Hostility.erase(iter);

							//추가
							if(eGUILD_RELATION_PROCLAIM_HOSTILITY == recvMsg->m_eMOVType)//적대 선포
							{
								pAddRelation->m_eRelationType = eGUILD_RELATION_PROCLAIM_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								//6188 %s 길드가 적대관계를 해제하여 %s길드의 일방적인 적대관계가 되었습니다.
								g_InterfaceManager.GetInterfaceString(6188, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName, g_pHero->GetGuildName());
							}
							else if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == recvMsg->m_eMOVType)//일방적 적대
							{
								pAddRelation->m_eRelationType = eGUILD_RELATION_ONE_SIDE_HOSTILITY;

								m_listGuildRelation_OneSideHostility.push_back(pAddRelation);

								//6188 %s 길드가 적대관계를 해제하여 %s길드의 일방적인 적대관계가 되었습니다.
								g_InterfaceManager.GetInterfaceString(6188, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, g_pHero->GetGuildName(), pAddRelation->m_GuildName);
							}

							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);

							break;
						}
					}

					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer && (pPlayer->GetPlayerGuildID() == gGuid))
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = recvMsg->m_eMOVType;
							}
						}
						++itr;
					}
				}
				break;

			//길드 복구
			case CG_GUILD_RECOVERY_GUILD_ACK:
				{
					m_GuildInfo.m_GuildDestroyWaitFlag = eGUILD_DELCHK_NORMAL;

					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					//6234 길드 해산 해제 신청이 완료 되었습니다.
					g_InterfaceManager.GetInterfaceString(6234, szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					m_pUIGuildMenu->ShowInterface(FALSE);
					m_pUIGuildMain->ShowInterface(FALSE);
				}
				break;

			case CG_GUILD_RECOVERY_GUILD_NAK:
				{
					MSG_CG_GUILD_RECOVERY_GUILD_NAK* pRecvMsg = (MSG_CG_GUILD_RECOVERY_GUILD_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			//길드 해산 대기
			case CG_GUILD_DESTROY_WAIT_ACK:
				{
					MSG_CG_GUILD_DESTROY_WAIT_ACK* pRecvMsg = (MSG_CG_GUILD_DESTROY_WAIT_ACK*)pMsg;

					m_GuildInfo.m_GuildDestroyWaitFlag = eGUILD_DELCHK_DESTROY_WAIT;

					//6228	길드 해산 대기 상태가 되었습니다.
					g_InterfaceManager.GetInterfaceString( 6228, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					m_pUIGuildMenu->ShowInterface(FALSE);
					m_pUIGuildMain->ShowInterface(FALSE);

					//길드해산 신청이 수락되면 모든 관계는 지워진다
					this->DelRelationCon();
					//주변 플레이어 정보 갱신
					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer)
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
							}
						}
						++itr;
					}

				}
				break;

			case CG_GUILD_DESTROY_WAIT_BRD:
				{
					MSG_CG_GUILD_DESTROY_WAIT_BRD* pRecvMsg = (MSG_CG_GUILD_DESTROY_WAIT_BRD*)pMsg;

					GUILDGUID GuildID = pRecvMsg->m_RelGuildGuid;
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szGuildName[MAX_GUILDNAME_LENGTH + 1] = {0,};
					TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};

					//주변 플레이어 정보 갱신
					Object_Map_Itr	itr = g_ObjectManager.GetBegin();
					while( g_ObjectManager.GetEnd() != itr )
					{
						Player* pPlayer = (Player*)itr->second;

						if(pPlayer)
						{
							if(PLAYER_OBJECT != pPlayer->GetSCObject().GetObjectType())
							{
								++itr;
								continue;
							}

							if(pPlayer && (pPlayer->GetPlayerGuildID() == GuildID))
							{
								pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
							}
						}
						++itr;
					}

					std::list<GUILD_RELATION*>::iterator iter;
					for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
					{
						if(0 == m_listGuildRelation_Hostility.size())
							break;

						GUILD_RELATION* pRelation = (*iter);

						if(pRelation->m_GuildGuid == GuildID)
						{
							StrnCopy(szGuildName, pRelation->m_GuildName, MAX_GUILDNAME_LENGTH);
							//7236	%s길드가 해산 신청을 하여 해당 길드와의 길드 관계는 모두 해제 되었습니다.
							g_InterfaceManager.GetInterfaceString(7236, szTemp, INTERFACE_STRING_LENGTH);
							Sprintf(szMessage, szTemp, szGuildName);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

							SAFE_DELETE(pRelation);
							m_listGuildRelation_Hostility.erase(iter);

							break;
						}
					}
				}
				break;


			case CG_GUILD_CHANGE_DUTY_NAK:
				{
					MSG_CG_GUILD_CHANGE_DUTY_NAK * pRecvMsg = (MSG_CG_GUILD_CHANGE_DUTY_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_CHANGE_DUTY_BRD:
				{
					MSG_CG_GUILD_CHANGE_DUTY_BRD * pRecvMsg = (MSG_CG_GUILD_CHANGE_DUTY_BRD *) pMsg;
					
					TCHAR szTargetName[MAX_CHARNAME_LENGTH + 1] ={0,};
					Snprintf(szTargetName, MAX_CHARNAME_LENGTH, pRecvMsg->m_szTargetCharName);
					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(szTargetName);
					if(pMemberInfo)
					{
						pMemberInfo->m_eGuildDuty = (eGUILD_DUTY) pRecvMsg->m_byDuty;
						GUILD_DUTY_USE_INFO * pGuildUseInfo = GuildRankInfoParser::Instance()->GetGuildDutyUseInfo(
							GetGuildInfo().m_GuildGrade , pMemberInfo->m_eGuildDuty);
						if( pGuildUseInfo )
						{
							//6276	%s님의 직책이 %s로 변경되었습니다.
							TCHAR szDutyName[INTERFACE_STRING_LENGTH] ={0,};
							g_InterfaceManager.GetInterfaceString(pGuildUseInfo->m_dwStringCode, szDutyName);
							Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6276) ,  szTargetName ,
									szDutyName );
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						}
						if( m_pUIGuildMain )
						{
							m_pUIGuildMain->SetListCaption();
						}
					}
					
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_LIST_ACK:
				{
					MSG_CG_GUILD_RIGHT_DELEGATION_LIST_ACK * pRecvMsg = ( MSG_CG_GUILD_RIGHT_DELEGATION_LIST_ACK *) pMsg;
					
					bool bFromRightDelegationDialog = true;
					if( m_pUIGuildMain && m_pUIGuildMemberRightView )
					{
						if( m_pUIGuildMain->GetMemberInfoButtonState() )
						{
							m_pUIGuildMemberRightView->ShowInterface(TRUE);
							m_pUIGuildMemberRightView->RegisteAllItem(m_pUIGuildMain->GetSelectedPacketForGuildMember() ,pRecvMsg->m_DelegateRight);
							m_pUIGuildMain->SetMemberInfoButtonState(false);
							bFromRightDelegationDialog = false;
						}
					}

					if( bFromRightDelegationDialog )
					{
						uiGuildRightDelegation * puiRightDelegation = GET_CAST_DIALOG( uiGuildRightDelegation, IM_GUILD_MANAGER::GUILD_RIGHT_DELEGATION );

						if( puiRightDelegation )
						{
							puiRightDelegation->SetTargetName(pRecvMsg->m_szTargetCharName);
							puiRightDelegation->AssignCheckState(pRecvMsg->m_DelegateRight);
						}
					}
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_LIST_NAK:
				{
					MSG_CG_GUILD_RIGHT_DELEGATION_LIST_NAK * pRecvMsg = ( MSG_CG_GUILD_RIGHT_DELEGATION_LIST_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_ACK:
				{
					MSG_CG_GUILD_RIGHT_DELEGATION_ACK * pRecvMsg = (MSG_CG_GUILD_RIGHT_DELEGATION_ACK *) pMsg;

					TCHAR szTargetName[MAX_CHARNAME_LENGTH+1] ={0,};
					Snprintf(szTargetName, MAX_CHARNAME_LENGTH, pRecvMsg->m_szTargetCharName);
					//6277	%s 님에게 권한을 위임하였습니다
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1,g_InterfaceManager.GetInterfaceString(6277), szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_CMD:
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
					//6278	새로운 권한을 위임 받았습니다.
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6278));
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_NAK:
				{
					MSG_CG_GUILD_RIGHT_DELEGATION_NAK * pRecvMsg = (MSG_CG_GUILD_RIGHT_DELEGATION_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_CHANGE_MASTER_BRD:
				{
					MSG_CG_GUILD_CHANGE_MASTER_BRD * pRecvMsg = (MSG_CG_GUILD_CHANGE_MASTER_BRD *) pMsg;

					TCHAR tszOldMasterName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL 처리 확실히 하기 위하여
					StrnCopy(tszOldMasterName, pRecvMsg->m_szOldMasterName, MAX_CHARNAME_LENGTH);

					TCHAR tszNewMasterName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL 처리 확실히 하기 위하여
					StrnCopy(tszNewMasterName, pRecvMsg->m_szNewMasterName, MAX_CHARNAME_LENGTH);

					stGUILD_MEMBER_INFO * pOldMasterInfo = GetGuildMemberInfo(tszOldMasterName);
					if( pOldMasterInfo )
					{
						pOldMasterInfo->m_eGuildDuty = eGUILD_DUTY_REGULAR_SOLDIER;
					}

					stGUILD_MEMBER_INFO * pNewMasterInfo = GetGuildMemberInfo(tszNewMasterName);
					if(pNewMasterInfo)
					{
						pNewMasterInfo->m_eGuildDuty = eGUILD_DUTY_MASTER;
					}

					if( !pOldMasterInfo || !pNewMasterInfo)
					{
						assert(!"길드마스터변경됐는데 타겟플레이어가 없다");
					}

					//6279	길드마스터가 %s님으로 변경되었습니다.
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6279) , tszNewMasterName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage);
					SetGuildMasterName(tszNewMasterName);
				}
				break;

			case CG_GUILD_CHANGE_MASTER_NAK:
				{
					MSG_CG_GUILD_CHANGE_MASTER_NAK * pRecvMsg = ( MSG_CG_GUILD_CHANGE_MASTER_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_CREATE_CORPS_BRD:
				{
					MSG_CG_GUILD_CREATE_CORPS_BRD * pRecvMsg = (MSG_CG_GUILD_CREATE_CORPS_BRD *) pMsg;

					if( pRecvMsg->m_byCorpsNum < MAX_GUILD_CORPS_NUM)
					{
						//GUILD_CORPS_TOTAL_INFO 갱신
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bCreate = true;
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_byCorpsNum = pRecvMsg->m_byCorpsNum;
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bRearCamp = false;

						//GUILD_MEMBER_INFO 갱신
						TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] ={0,};
						Snprintf(szTargetName, MAX_CHARNAME_LENGTH,pRecvMsg->m_szTargetCharName );

						stGUILD_MEMBER_INFO * pMemberInfo =	GetGuildMemberInfo(szTargetName);
						if( pMemberInfo )
						{
							pMemberInfo->m_byCorpsNum = pRecvMsg->m_byCorpsNum;
							pMemberInfo->m_eGuildDuty = eGUILD_DUTY_COMMANDER;
						}

						

						TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
						//6280	%s 군단이 창설되었습니다. %s님이 군단장으로 임명되었습니다.
						Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6280),
							GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum), szTargetName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						assert(!"군단창설갯수를 넘었다");
					}

				}
				break;

			case CG_GUILD_CREATE_CORPS_NAK:
				{
					MSG_CG_GUILD_CREATE_CORPS_NAK * pRecvMsg = (MSG_CG_GUILD_CREATE_CORPS_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_DESTROY_CORPS_BRD:
				{
					MSG_CG_GUILD_DESTROY_CORPS_BRD * pRecvMsg = (MSG_CG_GUILD_DESTROY_CORPS_BRD *) pMsg;

					//6207 제 %s 군단이 해산되었습니다.
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(6207,szMessage);
					Snprintf(szResult, INTERFACE_STRING_LENGTH-1,szMessage, GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);

					//GUILD_CORPS_TOTAL_INFO 갱신
					m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bCreate = false;
					m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bRearCamp = false;
					m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_byCorpsNum = pRecvMsg->m_byCorpsNum;
				
					if(m_pUIGuildMain )
					{
						m_pUIGuildMain->SetCurSelCorps(0);
						m_pUIGuildMain->SetListCaption();
					}
				}
				break;

			case CG_GUILD_DESTROY_CORPS_NAK:
				{
					MSG_CG_GUILD_DESTROY_CORPS_NAK * pRecvMsg = (MSG_CG_GUILD_DESTROY_CORPS_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_EMPTY_COMMANDER_INFO_ACK:
				{
					MSG_CG_GUILD_EMPTY_COMMANDER_INFO_ACK * pRecvMsg = (MSG_CG_GUILD_EMPTY_COMMANDER_INFO_ACK *) pMsg;
				}
				break;
			case CG_GUILD_EMPTY_COMMANDER_INFO_NAK:
				{
					MSG_CG_GUILD_EMPTY_COMMANDER_INFO_NAK * pRecvMsg = (MSG_CG_GUILD_EMPTY_COMMANDER_INFO_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;
			case CG_GUILD_CHANGE_COMMANDER_BRD:
				{
					MSG_CG_GUILD_CHANGE_COMMANDER_BRD * pRecvMsg = (MSG_CG_GUILD_CHANGE_COMMANDER_BRD *) pMsg;
					
					TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] = {0,};
					Snprintf(szTargetName, MAX_CHARNAME_LENGTH, pRecvMsg->m_szTargetCharName);

					TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
					//6282	%s님이 %s 군단장으로 변경되었습니다.
					g_InterfaceManager.GetInterfaceString(6282,szTemp);
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1,szTemp, szTargetName,
								GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(szTargetName);
					if( pMemberInfo )
					{
						pMemberInfo->m_eGuildDuty = eGUILD_DUTY_COMMANDER;
						pMemberInfo->m_byCorpsNum = pRecvMsg->m_byCorpsNum;
					}

					if( m_pUIGuildMain )
					{
						m_pUIGuildMain->SetListCaption();
					}
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_CHANGE_COMMANDER_NAK:
				{
					MSG_CG_GUILD_CHANGE_COMMANDER_NAK * pRecvMsg = (MSG_CG_GUILD_CHANGE_COMMANDER_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;
			case CG_GUILD_CHANGE_CORPS_BRD:
				{
					MSG_CG_GUILD_CHANGE_CORPS_BRD * pRecvMsg = (MSG_CG_GUILD_CHANGE_CORPS_BRD *) pMsg;

					TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] ={0,};
					Snprintf(szTargetName, MAX_CHARNAME_LENGTH, pRecvMsg->m_szTargetCharName);

					//길드원 군단 갱신
					stGUILD_MEMBER_INFO * pMember = GetGuildMemberInfo(szTargetName);
					if( pMember)
					{
						pMember->m_byCorpsNum = pRecvMsg->m_byCorpsNum;
					}				

					//6268	%s 님이 %s군단으로 변경되었습니다.
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6268) , 
						szTargetName ,GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
				}
				break;
			case CG_GUILD_CHANGE_CORPS_NAK:
				{
					MSG_CG_GUILD_CHANGE_CORPS_NAK * pRecvMsg = (MSG_CG_GUILD_CHANGE_CORPS_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;


			case CG_GUILD_CREATE_REAR_CAMP_BRD:
				{
					MSG_CG_GUILD_CREATE_REAR_CAMP_BRD * pRecvMsg = (MSG_CG_GUILD_CREATE_REAR_CAMP_BRD *) pMsg;
				
					TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] ={0,};
					Snprintf(szTargetName, MAX_CHARNAME_LENGTH, pRecvMsg->m_szTargetCharName);
					//6283	%s 군단 육성캠프가 생성되었습니다. %s님이 육성캠프장으로 임명되었습니다.
					TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetInterfaceString(6283, szTemp);
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szTemp , 
							GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum), szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					//직책 갱신
					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(szTargetName);
					if( pMemberInfo)
					{
						pMemberInfo->m_eGuildDuty = eGUILD_DUTY_CAMP_LEADER;
					}


					// GUILD_TOTAL_CORPS_INFO 갱신
					if( pRecvMsg->m_byCorpsNum < MAX_GUILD_CORPS_NUM)
					{
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bRearCamp = true;
					}
					else
					{
						assert(!"Invalid Data: CorpsNum");
					}
				}
				break;

			case CG_GUILD_CREATE_REAR_CAMP_NAK:
				{
					MSG_CG_GUILD_CREATE_REAR_CAMP_NAK * pRecvMsg = (MSG_CG_GUILD_CREATE_REAR_CAMP_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_DESTROY_REAR_CAMP_BRD:
				{
					MSG_CG_GUILD_DESTROY_REAR_CAMP_BRD * pRecvMsg = (MSG_CG_GUILD_DESTROY_REAR_CAMP_BRD *) pMsg;
					//6208 제 %s 군단에 육성캠프가 해산되었습니다.
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(6208,szMessage);
					Snprintf(szResult, INTERFACE_STRING_LENGTH-1,szMessage, GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);

					// GUILD_TOTAL_CORPS_INFO 갱신
					if( pRecvMsg->m_byCorpsNum < MAX_GUILD_CORPS_NUM)
					{
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bRearCamp = false;
					}
					else
					{
						assert(!"Invalid Data: CorpsNum");
					}
				}
				break;

			case CG_GUILD_DESTROY_REAR_CAMP_NAK:
				{
					MSG_CG_GUILD_DESTROY_REAR_CAMP_NAK * pRecvMsg = (MSG_CG_GUILD_DESTROY_REAR_CAMP_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;
			case CG_GUILD_CHANGE_CHAOSTATE_BRD:
				{
					MSG_CG_GUILD_CHANGE_CHAOSTATE_BRD * pRecvMsg = (MSG_CG_GUILD_CHANGE_CHAOSTATE_BRD *) pMsg;
					TCHAR tszCharName[MAX_CHARNAME_LENGTH+1] = {0,};
					StrnCopy(tszCharName, pRecvMsg->m_szCharName, MAX_CHARNAME_LENGTH);

					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(tszCharName);
					if( pMemberInfo )
					{
						pMemberInfo->m_byChaoState = pRecvMsg->m_byChaoState;
					}
				}
				break;


			case CG_GUILD_TOTALINFO_REQ_ACK:
				{
					bool bShowGuildWindow = IsGuildInfo();
					MSG_CG_GUILD_TOTALINFO_REQ_ACK * pRecvMsg = (MSG_CG_GUILD_TOTALINFO_REQ_ACK *) pMsg;
					SetGuildInfo(pRecvMsg->m_GuildInfo);
					memcpy(&m_GuildCorpsTotalInfo, &pRecvMsg->m_CorpsTotalInfo, sizeof(GUILD_CORPS_TOTAL_INFO));
					m_bGuildMemberInfo =true;
					// 길드 멤버 정보
					m_vMembers.clear();
					for( int i = 0; i < pRecvMsg->m_MemberTotalInfo.m_byMemberCount; ++i)
					{
						m_vMembers.push_back(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i]);
						if(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_eGuildDuty == eGUILD_DUTY_MASTER)
						{
							SetGuildMasterName(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_tszCharName);
						}

						//같은 길드의 일원은 동맹관계로 처리한다.
						Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_tszCharName);
						if(pPlayer)
						{
							pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
                            if (pPlayer == g_pHero)
                            {
                                pPlayer->GetGuildRenderInfo().m_GuildDuty = 
                                    pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_eGuildDuty;
                            }
						}
					}
					
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
                    //! 2011.7.22 / i4u4me / 캐시샵 작업 
                    uiConvenienceStore_FriendList* friend_list_dlg = GET_CAST_DIALOG(uiConvenienceStore_FriendList, uiConvenienceStore_FriendList::kDialog_ID);
                    if ((friend_list_dlg != NULL) && 
                        (request_uibase_guild_totalinfo_ == friend_list_dlg) &&
                        (friend_list_dlg->IsVisible() == TRUE))
                    {
                        friend_list_dlg->RefreshFriendList();
                    }
                    else if( bShowGuildWindow )
                    {
                        ShowGuildWindow();
                    }
#else
					if( bShowGuildWindow )
					{
						ShowGuildWindow();
					}
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
				}	
				break;

			case CG_GUILD_TOTALINFO_REQ_NAK:
				{
					MSG_CG_GUILD_TOTALINFO_REQ_NAK * pRecvMsg = (MSG_CG_GUILD_TOTALINFO_REQ_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;

			case CG_GUILD_LEAVE_BRD:
				{
					MSG_CG_GUILD_LEAVE_BRD * pRecvMsg = (MSG_CG_GUILD_LEAVE_BRD *) pMsg;

					Object * pObject = g_ObjectManager.GetObject(pRecvMsg->m_LeaverObjectKey);
					if (pObject && (pObject->IsKindOfObject(PLAYER_OBJECT)))
					{
						Player *pTargetPlayer = (Player *) pObject;
						if (pTargetPlayer)
						{
							pTargetPlayer->ClearGuildRenderInfo();
						}
					}
				}
				break;
			case  CG_GUILD_GM_GUILD_POINT_BRD:
				{
					//GM 명령어
					MSG_CG_GUILD_GM_GUILD_POINT_BRD * pRecvMsg = (MSG_CG_GUILD_GM_GUILD_POINT_BRD *) pMsg;
					m_GuildInfo.m_UserPoint = pRecvMsg->m_UP;  
					m_GuildInfo.m_GuildPoint = pRecvMsg->m_GP;
				}
				break;
			case CG_GUILD_GM_GUILD_POINT_NAK:
				{
					MSG_CG_GUILD_GM_GUILD_POINT_NAK * pRecvMsg = (MSG_CG_GUILD_GM_GUILD_POINT_NAK *) pMsg;
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode);
				}
				break;



			case CG_GUILD_GUILDMARK_DELETE_ACK:
				{
					MSG_CG_GUILD_GUILDMARK_DELETE_ACK * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_DELETE_ACK *) pMsg;
					if( IsGuildInfo() )
					{
						SetGuildMarkBackGround(pRecvMsg->m_BackGroundIndex);
						SetGuildMarkPattern(pRecvMsg->m_PatternIndex);
						//7241	길드 마크가 정상적으로 삭제되었습니다.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7241));
					}
				}
				break;

			case CG_GUILD_GUILDMARK_DELETE_BRD:
				{
					MSG_CG_GUILD_GUILDMARK_DELETE_BRD * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_DELETE_BRD *) pMsg;
					if(g_pHero && g_pHero->GetObjectKey() == pRecvMsg->m_dwPlayerKey)
					{
						SetGuildMarkBackGround(pRecvMsg->m_BackGroundIndex);
						SetGuildMarkPattern(pRecvMsg->m_PatternIndex);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7241));
					}

					Player * pTargetPlayer = (Player *) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if( pTargetPlayer)
					{
						pTargetPlayer->GetGuildRenderInfo().m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
						pTargetPlayer->GetGuildRenderInfo().m_PatternIndex = pRecvMsg->m_PatternIndex;
					}


				}
				break;

			case CG_GUILD_GUILDMARK_DELETE_NAK:
				{	
					MSG_CG_GUILD_GUILDMARK_DELETE_NAK * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_DELETE_NAK *) pMsg;
					// 7218	길드 마크 삭제에 실패했습니다.
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7218));
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_bErrorCode);

				}
				break;

			case CG_GUILD_GUILDMARK_REGISTER_ACK:
				{
					MSG_CG_GUILD_GUILDMARK_REGISTER_ACK * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_REGISTER_ACK *) pMsg;
					if( IsGuildInfo() )
					{
						SetGuildMarkBackGround(pRecvMsg->m_BackGroundIndex);
						SetGuildMarkPattern(pRecvMsg->m_PatternIndex);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6164));
					}
				}
				break;

			case CG_GUILD_GUILDMARK_REGISTER_BRD:
				{
					MSG_CG_GUILD_GUILDMARK_REGISTER_BRD * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_REGISTER_BRD *) pMsg;

					if( g_pHero && g_pHero->GetObjectKey() == pRecvMsg->m_dwPlayerKey)
					{
						SetGuildMarkBackGround(pRecvMsg->m_BackGroundIndex);
						SetGuildMarkPattern(pRecvMsg->m_PatternIndex);
						//6127	길드마크가 등록 되었습니다.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6127));
					}

					Player * pTargetPlayer = (Player *) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if( pTargetPlayer )
					{
						pTargetPlayer->GetGuildRenderInfo().m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
						pTargetPlayer->GetGuildRenderInfo().m_PatternIndex = pRecvMsg->m_PatternIndex;
						//////////////////////////////////////////////////////////////////////////
						//마크 등록한 플레이어와의 길드 관계를 셋팅한다.

						if(g_pHero->GetGuildRenderInfo().m_GuildGuid == pTargetPlayer->GetGuildRenderInfo().m_GuildGuid)
						{
							pTargetPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
						}
						else
						{
							std::list<GUILD_RELATION*>::iterator iter;
							for(iter = m_listGuildRelation_Alliance.begin(); iter != m_listGuildRelation_Alliance.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}

							for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}

							for(iter = m_listGuildRelation_OneSideHostility.begin(); iter != m_listGuildRelation_OneSideHostility.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}
						}
						//////////////////////////////////////////////////////////////////////////
	
					}
				}
				break;

			case CG_GUILD_GUILDMARK_REGISTER_NAK:
				{
					MSG_CG_GUILD_GUILDMARK_REGISTER_NAK * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_REGISTER_NAK *) pMsg;
					//6165	길드 마크 등록에 실패하였습니다. 다시 등록해 주십시오.	
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6165));
					uiGuildMan::ProcessErrorCode(pRecvMsg->m_bErrorCode);
					
				}
				break;


			case CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD:
				{
					MSG_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD *) pMsg;
					Player * pTargetPlayer = (Player *) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if( pTargetPlayer )
					{
						pTargetPlayer->GetGuildRenderInfo().m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
						pTargetPlayer->GetGuildRenderInfo().m_PatternIndex = pRecvMsg->m_PatternIndex;
					}
				}
				break;

			case CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD:
				{
					MSG_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD * pRecvMsg = (MSG_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD *) pMsg;
					Player * pTargetPlayer = (Player *) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if( pTargetPlayer )
					{
						pTargetPlayer->GetGuildRenderInfo().m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
						pTargetPlayer->GetGuildRenderInfo().m_PatternIndex = pRecvMsg->m_PatternIndex;

						//////////////////////////////////////////////////////////////////////////
						//마크 등록한 플레이어와의 길드 관계를 셋팅한다.

						if(g_pHero->GetGuildRenderInfo().m_GuildGuid == pTargetPlayer->GetGuildRenderInfo().m_GuildGuid)
						{
							pTargetPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
						}
						else
						{
							std::list<GUILD_RELATION*>::iterator iter;
							for(iter = m_listGuildRelation_Alliance.begin(); iter != m_listGuildRelation_Alliance.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}

							for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}

							for(iter = m_listGuildRelation_OneSideHostility.begin(); iter != m_listGuildRelation_OneSideHostility.end(); ++iter)
							{
								GUILD_RELATION* pGuildRelation = (*iter);
								if(pRecvMsg->m_GuildIndex == pGuildRelation->m_GuildGuid)
								{
									pTargetPlayer->GetGuildRenderInfo().m_RelationType = pGuildRelation->m_eRelationType;
								}
							}
						}
						//////////////////////////////////////////////////////////////////////////

					}

				}
				break;

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
            case CG_GUILD_WAREHOUSE_OPEN_ACK:
            case CG_GUILD_WAREHOUSE_OPEN_NAK:
            case CG_GUILD_WAREHOUSE_CLOSE_ACK:
            case CG_GUILD_WAREHOUSE_CLOSE_NAK:
            case CG_GUILD_WAREHOUSE_MONEY_ACK:
            case CG_GUILD_WAREHOUSE_MONEY_NAK:
                {
                    uiGuildWareHouse* guild_warehouse = GET_CAST_DIALOG(uiGuildWareHouse, IM_GUILD_MANAGER::GUILD_WAREHOUSE);
                    if (guild_warehouse != NULL)
                    {
                        guild_warehouse->NetworkProc(pMsg);
                    }
                }
                break;
#else

			case CG_GUILD_WAREHOUSE_OPEN_ACK:
				{
					MSG_CG_GUILD_WAREHOUSE_OPEN_ACK* pRecvMsg = (MSG_CG_GUILD_WAREHOUSE_OPEN_ACK*)pMsg;

					uiGuildWareHouse* pDlg = GET_CAST_DIALOG( uiGuildWareHouse, IM_GUILD_MANAGER::GUILD_WAREHOUSE );

					if(pDlg)
					{
						pDlg->FlushUnitDraw();
						pDlg->SetMoney(pRecvMsg->m_WarehouseMoney);
						pDlg->SetMaxTab(pRecvMsg->m_SlotTabNum);

						BitStream bitstream(pRecvMsg->m_BitStreamBuffer, pRecvMsg->m_Size, FALSE);
						pDlg->SCItemSlotContainer::Serialize(	bitstream, eSLOT_SERIALIZE_CLIENT_SAVE );

						pDlg->ShowInterface(TRUE);
					}
				}
				break;

			case CG_GUILD_WAREHOUSE_OPEN_NAK:
				{
					MSG_CG_GUILD_WAREHOUSE_OPEN_NAK* pRecvMsg = (MSG_CG_GUILD_WAREHOUSE_OPEN_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(static_cast<BYTE>(pRecvMsg->m_bErrorCode));
				}
				break;

			case CG_GUILD_WAREHOUSE_CLOSE_ACK:
				{
					uiBase * pDlg = GET_DIALOG( IM_GUILD_MANAGER::GUILD_WAREHOUSE );
					if(pDlg)
					{
						pDlg->ShowInterface(FALSE);
					}
				}
				break;

			case CG_GUILD_WAREHOUSE_CLOSE_NAK:
				{
					//일단 끄자
					uiBase * pDlg = GET_DIALOG( IM_GUILD_MANAGER::GUILD_WAREHOUSE );

					if(pDlg)
					{
						pDlg->ShowInterface(FALSE);
					}

					MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK* pRecvMsg = (MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(static_cast<BYTE>(pRecvMsg->m_bErrorCode));
				}
				break;

			case CG_GUILD_WAREHOUSE_MONEY_ACK:
				{
					MSG_CG_GUILD_WAREHOUSE_MONEY_ACK * pRecvMsg = (MSG_CG_GUILD_WAREHOUSE_MONEY_ACK *)pMsg;

					if(g_pHero)
						g_pHero->SetMoney(pRecvMsg->m_InventoryMoney);

					uiGuildWareHouse* pGuildWareHouse = GET_CAST_DIALOG( uiGuildWareHouse, IM_GUILD_MANAGER::GUILD_WAREHOUSE );

					if(pGuildWareHouse)
					{
						pGuildWareHouse->SetMoney(pRecvMsg->m_WarehouseMoney);
					}

					InputNumBankDialog * pInputNumBankDlg =
						(InputNumBankDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_INPUT_NUM);
					if (pInputNumBankDlg)
					{
						pInputNumBankDlg->ClearPacketFlag();
					}	
				}
				break;

			case CG_GUILD_WAREHOUSE_MONEY_NAK:
				{
					MSG_CG_GUILD_WAREHOUSE_MONEY_NAK* pRecvMsg = (MSG_CG_GUILD_WAREHOUSE_MONEY_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(static_cast<BYTE>(pRecvMsg->m_dwErrorCode));
				}
				break;
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

			case CG_GUILD_NAME_CHANGE_BRD:
				{
					MSG_CG_GUILD_NAME_CHANGE_BRD* pRecvMsg = (MSG_CG_GUILD_NAME_CHANGE_BRD*)pMsg;
					g_pHero->SetGuildName(pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH);
					SetGuildName(pRecvMsg->m_tszGuildName);

					//7252	길드명이 %s로 변경되었습니다.
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					g_InterfaceManager.GetInterfaceString(7252, szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pRecvMsg->m_tszGuildName);
				}
				break;

			case CG_GUILD_NAME_CHANGE_NAK:
				{
					MSG_CG_GUILD_NAME_CHANGE_NAK* pRecvMsg = (MSG_CG_GUILD_NAME_CHANGE_NAK*)pMsg;
					uiGuildMan::ProcessErrorCode(static_cast<BYTE>( pRecvMsg->m_bErrorCode));
				}
				break;

			case CG_GUILD_NAME_CHANGE_SECTOR_BRD:
				{
					MSG_CG_GUILD_NAME_CHANGE_SECTOR_BRD* pRecvMsg = (MSG_CG_GUILD_NAME_CHANGE_SECTOR_BRD*)pMsg;
					Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if(pPlayer)
					{
						pPlayer->SetGuildName(pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH);
					}
				}
				break;

			case CG_GUILD_NAME_CHANGE_RELATION_BRD:
				{
					MSG_CG_GUILD_NAME_CHANGE_RELATION_BRD* pRecvMsg = (MSG_CG_GUILD_NAME_CHANGE_RELATION_BRD*)pMsg;

					//7253	%s길드가 %s길드로 길드명을 변경하였습니다.
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					g_InterfaceManager.GetInterfaceString(7253, szMessage);

					switch(pRecvMsg->m_RelationType)
					{
					case eGUILD_RELATION_ALLIENCE:
						{
							std::list<GUILD_RELATION*>::iterator iter = m_listGuildRelation_Alliance.begin();
							std::list<GUILD_RELATION*>::iterator iterEnd = m_listGuildRelation_Alliance.end();
							while(iter != iterEnd)
							{
								GUILD_RELATION* pGR = (*iter);
								if(pRecvMsg->m_GuildGuid == pGR->m_GuildGuid)
								{
									GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pGR->m_GuildName, pRecvMsg->m_tszGuildName);
									StrnCopy(pGR->m_GuildName, pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH + 1);
									break;
								}

								++iter;
							}
						}
						break;

					case eGUILD_RELATION_BOTH_HOSTILITY:
					case eGUILD_RELATION_PROCLAIM_HOSTILITY:
						{
							std::list<GUILD_RELATION*>::iterator iter = m_listGuildRelation_Hostility.begin();
							std::list<GUILD_RELATION*>::iterator iterEnd = m_listGuildRelation_Hostility.end();
							while(iter != iterEnd)
							{
								GUILD_RELATION* pGR = (*iter);
								if(pRecvMsg->m_GuildGuid == pGR->m_GuildGuid)
								{
									GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pGR->m_GuildName, pRecvMsg->m_tszGuildName);
									StrnCopy(pGR->m_GuildName, pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH + 1);
									break;
								}

								++iter;
							}
						}
						break;

					case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
						{
							std::list<GUILD_RELATION*>::iterator iter = m_listGuildRelation_OneSideHostility.begin();
							std::list<GUILD_RELATION*>::iterator iterEnd = m_listGuildRelation_OneSideHostility.end();
							while(iter != iterEnd)
							{
								GUILD_RELATION* pGR = (*iter);
								if(pRecvMsg->m_GuildGuid == pGR->m_GuildGuid)
								{
									GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pGR->m_GuildName, pRecvMsg->m_tszGuildName);
									StrnCopy(pGR->m_GuildName, pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH + 1);
									break;
								}

								++iter;
							}
						}
						break;

					default:
						assert(!"CG_GUILD_NAME_CHANGE_RELATION_BRD");
					}
				}
				break;

            default:
                break;
            }
        }
        break;
    case CG_BATTLE:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_DOMINATION_UPDATE_GUILDINFO_BRD:
                {
                    MSG_CG_DOMINATION_UPDATE_GUILDINFO_BRD* recv_packet = 
                        static_cast<MSG_CG_DOMINATION_UPDATE_GUILDINFO_BRD*>(pMsg);
                    m_GuildInfo.m_DominationMapCount = recv_packet->domination_map_count_;
                    m_GuildInfo.m_DominationJoinCount = recv_packet->domination_join_count_;
                    m_GuildInfo.m_DominationDefenseCount = recv_packet->domination_defense_count_;
                    m_GuildInfo.m_DominationHuntingCount = recv_packet->domination_hunting_count_;
                    if (m_pUIGuildWindow != NULL)
                    {
                        m_pUIGuildWindow->updateGuildInfo();
                    }
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::NetworkProcWorld(MSG_BASE * pMsg )
{
	if ( (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT)
		|| ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ) )
		return;

    switch (pMsg->m_byCategory)
    {
    case CW_GUILD:
        {
            switch (pMsg->m_byProtocol)
            {
			case CW_GUILD_CHAT_BRD:
			case CW_GUILD_CHAT_NAK:
				{
					g_InterfaceManager.NetworkProc( IM_CHAT_MANAGER::CHAT_DIALOG, pMsg );//g_InterfaceManager.NetworkProc( InterfaceManager::DIALOG_CHAT, pMsg );
				}
				break;
			

			case CW_GUILD_SELECT_NOTICE_NAK:
				{
					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
					//7196	길드 공지를 받지 못했습니다.
					g_InterfaceManager.GetInterfaceString(7196, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;
			case CW_GUILD_SELECT_NOTICE_ACK:
				{
					MSG_CW_GUILD_SELECT_NOTICE_ACK *recvMsg = (MSG_CW_GUILD_SELECT_NOTICE_ACK *)pMsg;

					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

					TCHAR	tszNotice[MAX_GUILDNOTICE_LENGTH+1] = {0,};

					StrnCopy(tszNotice , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
					//safecode:서버에서 쓰레기값날라왔을때를 대비
					if(recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;
						

					tszNotice[recvMsg->m_wNoticeLen] = NULL;

					if(Strcmp(tszNotice, m_tszNotice))	// 새로운 공지 사항일때만 출력
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	길드 공지가 전달되었습니다.
						g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						m_pUIGuildWindow->setGuildNotice();
					}


				}
				break;
			case CW_GUILD_NOTICE_CMD:
				{
					MSG_CW_GUILD_NOTICE_CMD *recvMsg = (MSG_CW_GUILD_NOTICE_CMD *)pMsg;

					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
								
					TCHAR	tszNotice[MAX_GUILDNOTICE_LENGTH+1] = {0,};

					StrnCopy(tszNotice , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
					//safecode:서버에서 쓰레기값날라왔을때를 대비
					if(recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;
					      
					
					tszNotice[recvMsg->m_wNoticeLen] = NULL;

					if(Strcmp(tszNotice, m_tszNotice))	// 새로운 공지 사항일때만 출력
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	길드 공지가 전달되었습니다.
						g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						m_pUIGuildWindow->setGuildNotice();
					}

				}
				break;
			case CW_GUILD_CHANGE_NOTICE_BRD:
				{
					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

					MSG_CW_GUILD_CHANGE_NOTICE_BRD *recvMsg = (MSG_CW_GUILD_CHANGE_NOTICE_BRD *)pMsg;

					TCHAR	tszNotice[MAX_GUILDNOTICE_LENGTH+1] = {0,};

					StrnCopy(tszNotice , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
					tszNotice[recvMsg->m_wNoticeLen] = NULL;

					if(Strcmp(tszNotice, m_tszNotice))	// 새로운 공지 사항일때만 출력
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	길드 공지가 전달되었습니다.
						g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					 	m_pUIGuildWindow->setGuildNotice();
					}

				}
				break;

			case CW_GUILD_CHANGE_NOTICE_NAK:
				{
					MSG_CW_GUILD_CHANGE_NOTICE_NAK* pRecvMsg  =(MSG_CW_GUILD_CHANGE_NOTICE_NAK*)pMsg;
					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
					//5176	길드 공지 변경에 실패했습니다.	
					g_InterfaceManager.GetInterfaceString(5176, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;


			//로그창 열때마다 갱신.
			case CW_GUILD_WAREHOUSE_LOGLIST_ANS:
				{
					MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS* pRecvMsg = (MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS*)pMsg;
					uiGuildWareHouseLog * pDlg = GET_CAST_DIALOG( uiGuildWareHouseLog, IM_GUILD_MANAGER::GUILD_WAREHOUSE_LOG );
					
					if(pDlg)
					{
						if(1 == pRecvMsg->m_byCurrentPage) //처음 받는거면 컨테이너 깨끗이 하고 (1페이지씩 받는다)
						{
							pDlg->ClearLogList();
						}
						for(int i = 0; i < pRecvMsg->m_byLogCount; ++i)
						{
							pDlg->SetLogList(pRecvMsg->m_LogList[i]);
						}
						pDlg->ShowInterface(TRUE);
					}
				}
				break;

			case CW_GUILD_WAREHOUSE_LOGLIST_NAK:
				{
					MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK* pRecvMsg = (MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK*)pMsg;
					
					//COMMENT: 로그가 0일때 이상한 결과메세지 나간다릴래 일단 주석
					//uiGuildMan::ProcessErrorCode(pRecvMsg->m_byErrorCode); 
				}
				break;

			default:
                break;
            }
        }//CW_GUILD
        break;
    }//MAIN Case
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::ProcessErrorCode(BYTE byErrorCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] ={0,};
	int iMessage = 0;

	switch (byErrorCode)
    {
    case RC_GUILD_CANNOT_CREAT_GUILD_STATE:
			//5151	길드를 창설할 수 없습니다.
			iMessage = 5151;
        break;

    case RC_GUILD_CANNOT_CREAT_LEVEL_STATE:
			//5152	레벨이 낮아 길드를 창설할 수 없습니다.
			iMessage = 5152;
        break;

    case RC_GUILD_CANNOT_CREAT_MONEY_STATE:
			//5153	하임이 부족하여 길드를 창설할 수 없습니다.
			iMessage = 5153;
        break;

    case RC_GUILD_CANNOT_DESTROY_STATE:
			//5154	길드를 해체할 수 없습니다.
			iMessage = 5154;
        break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT:
    case RC_GUILD_CANNOT_DESTROY_NOT_MASTER_STATE:
			//5154	길드를 해체할 수 없습니다.
			iMessage = 5154;
        break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST:
	case RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE:
			//5155	길드원이 존재할 경우 길드를 해체할 수 없습니다.
			iMessage = 5155;
		break;

    case RC_GUILD_NOT_EXIST_TARGET_STATE:
			//5156	길드에 초대할 플레이어를 선택해 주세요
			iMessage = 5156;
        break;

    case RC_GUILD_CANNOT_INVITE_STATE:
			//5157	길드에 초대할 수 없는 상태입니다.
			iMessage = 5157;
        break;

    case RC_GUILD_DONT_HAVE_RIGHTS_STATE:
			//5158	길드 초대 권한이 없습니다.
			iMessage = 5158;
        break;

    case RC_GUILD_FULL_MEMBER_STATE:
			//5159	더 이상 길드원을 추가할 수 없습니다. 길드 레벨을 확인해 주십시오.
			iMessage = 5159;
        break;

    case RC_GUILD_NOT_EXIST_HOST_STATE:
			//5157	길드에 초대할 수 없는 상태입니다.
			iMessage = 5157;
        break;

    case RC_GUILD_CANNOT_WITHDRAW_STATE:
			//5164	길드 관련한 알 수 없는 결과를 받았습니다.
			iMessage = 5164;
        break;

    case RC_GUILD_NOT_EXIST_GUILD:
			//5160	존재하지 않는 길드입니다.
			iMessage = 5160;
        break;

    case RC_GUILD_CANNOT_JOIN_STATE:
			//5161	길드에 가입할 수 없습니다.
			iMessage = 5161;
        break;

    case RC_GUILD_CANNOT_SELECT_GUILD_STATE:
			//5162	길드 정보를 불러오지 못했습니다.
			iMessage = 5162;
        break;

    case RC_GUILD_WAIT_A_WHILE_THEN_TRY_AGAIN:
			//5163	잠시 후 다시 시도해 주시기 바랍니다.
			iMessage = 5163;
        break;

	case RC_GUILD_CANNOT_KICK_STATE:
			//5168	길드원 제명에 실패했습니다.
			iMessage = 5168;
		break;
	
	case RC_GUILD_CANNOT_KICK_RIGHT:
			//5167	길드원 제명 권한이 없습니다.	
			iMessage = 5167;
		break;

	case RC_GUILD_NOT_ENOUGH_UP:
			// UP(유저포인트)가 부족합니다. 5185
			iMessage = eST_NOT_ENOUGH_UP;
		break;

	case RC_GUILD_NOT_ENOUGH_GP:
			// GP(길드포인트)가 부족합니다. 5186
			iMessage = eST_NOT_ENOUGH_GP;
		break;

	case RC_GUILD_CANNOT_RANKUP_MONEY_STATE:
			// 하임이 부족 합니다. : 551
			iMessage = eST_NOT_ENOUGH_MONEY;
		break;

	case RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE:
			// 더이상 랭크 업 할수 없습니다.
			iMessage = 5195;
		break;

	case RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE:
			// 5275	카오 상태에서는 이용이 가능하지 않습니다.
			iMessage = 5275;
		break;

	case RC_GUILD_MARK_INVALID_TYPE:
			// 1198 길드 마크 형식에 어긋남
			iMessage = 1198;
		break;

	case RC_GUILD_MARK_CREATE_FAILED:
			// 1200 길드 마크 생성 실패
			iMessage = 1200;
		break;

	case RC_GUILD_CURRENT_NONE_MASTER:
			//6175 길드 마스터가 존재하지 않습니다.
			iMessage = 6175;
		break;

	case RC_GUILD_RELATION_REMAIN_GUILD:
	case RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN:
			//7195	길드관계가 남아있어서 길드를 해체할 수 없습니다.
			iMessage = 7195;
		break;

	case RC_GUILD_RELATION_LIST_ERROR:
			//7176	길드관계의 정보가 잘못되었습니다.
			iMessage = 7176;
		break;

	case RC_GUILD_RELATION_ME_FULL:
			//7177	해당 길드관계의 제한 수치를 초과하여 더 이상 해당 길드관계를 맺을 수 없습니다.
			iMessage = 7177;
		break;

	case RC_GUILD_RELATION_OTHER_FULL:
			//7178	상대방 길드가 해당 길드관계의 제한 수치를 초과하여 더이상 해당 길드관계를 맺을 수 없습니다.
			iMessage = 7178;
		break;

	case RC_GUILD_RELATION_CHANGE_RIGHT_NOT:
			//7179	길드 관계를 설정할 권한이 없습니다.
			iMessage = 7179;
		break;

	case RC_GUILD_RELATION_ALEADY_SET:
			//7180	길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있습니다.
			iMessage = 7180;
		break;

	case RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED:
			//7181	동맹을 맺으려는 길드가 자신의 길드와 적대 관계를 맺고 있는 길드와 동맹관계라 동맹 관계를 맺을 수 없습니다.
			iMessage = 7181;
		break;

	case RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED:
			//7182	동맹을 맺으려는 길드가 자신의 길드와 동맹을 맺고 있는 길드와 적대 관계라 동맹 관계를 맺을 수 없습니다.
			iMessage = 7182;
		break;

	case RC_GUILD_RELATION_HOSTILITY_FAILED:
			//7194	적대를 맺으려는 길드가 자신의 길드와 동맹 관계를 맺고 있는 길드와 동맹 관계여서 적대 관계를 맺을 수 없습니다.
			iMessage = 7194;
		break;

	case RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED:
			//7183	상대 길드와 자신의 길드가 일방적 적대 상태 또는 적대 선포 관계가 아니기 때문에 쌍방 적대 관계를 맺을 수 없습니다.
			iMessage = 7183;
		break;

	case RC_GUILD_RELATION_NONE_DELETE_NOT:
			//7184	상대 길드와 어떠한 관계도 맺고 있지 않기 때문에 관계를 삭제를 할 수 없습니다.
			iMessage = 7184;
		break;

	case RC_GUILD_RELATION_GUILD_DESTROY_WAIT:
			//7185	길드 해산 대기중이라 길드 관계를 맺을 수 없습니다.
			iMessage = 7185;
		break;

	case RC_GUILD_RELATION_GUILD_GRADE_LOW_ME:
			//7215	길드 레벨이 낮아 관계를 맺을 수 없습니다.
			iMessage = 7215;
		break;

	case RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER:
			//7216	상대방 길드의 레벨이 낮아 관계를 맺을 수 없습니다.
			iMessage = 7216;
		break;

	case RC_GUILD_RELATION_DELETE_FAILED: //길드 관계 삭제 실패
			//7219	길드 관계 삭제에 실패했습니다.
			iMessage = 7219;
		break;

  	case RC_GUILD_RELATION_DELETE_FAILED_BY_TIME:
			//7239 적대관계를 신청한 후 1시간이 지나야 해제가 가능합니다.
			iMessage = 7239;
		break;
  
	case RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE:
			//7186	길드 등급이 낮아 길드 창고를 이용할 수 없습니다.
			iMessage = 7186;
		break;

	case RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER:
			//7187	길드 창고를 사용중인 유저가 있습니다.
			iMessage = 7187;
		break;

	case RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD:
			//7188	유저의 길드창고 정보와 실제 길드의 길드 창고 정보가 일치하지 않습니다.
			iMessage = 7188;
		break;

	case RC_GUILD_WAREHOUSE_INVENMONEY_LOW:
			////551 하임이 부족합니다.
			iMessage = 551;
		break;

	case RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT:
			//7192	길드 창고의 하임을 출금할 권한이 없습니다.
			iMessage = 7192;
		break;

	case RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT:
			//7193	하임 한계치를 초과하였습니다.
			iMessage = 7193;
		break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN:
			//7237	길드창고에 아이템 또는 하임이 남아있어 길드 해산 신청을 할 수 없습니다.
			iMessage = 7237;
		break;

  	case RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING:	
			//7243	길드 창고 사용중이여서 탈퇴/제명을 할 수가 없다.
			iMessage = 7243;
		break;
  

	case RC_GUILD_DESTROY_PENALTY_TIME:
			//6213	길드가 해산된 후 3일이 지나지 않아 길드 가입 및 창설을 할 수 없습니다.
			iMessage = 6213;
		break;

	case RC_GUILD_WITHDRAW_PENALTY_TIME:
	case RC_GUILD_KICK_PENALTY_TIME:
			//6210	길드를 탈퇴하거나 제명 당한 지 3일이 지나지 않아 길드에 가입할 수 없습니다.
			iMessage = 6210;
		break;

	case RC_GUILD_RECOVERY_FAILED:
			//6235	길드 해산 해제 신청에 실패하였습니다.
			iMessage = 6235;
		break;

	case RC_GUILD_DELCHK_DESTROY_WAIT:
			//6236	길드 해산 대기 중입니다.
			iMessage = 6236;
		break;

	case RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT:
			//7206 길드등급 레벨이 낮거나,공지 변경 권한이 없습니다
			iMessage = 7206;
		break;

	case RC_GUILD_ALREADY_EXIST_DUTY:
			//6253	이미 해당 직책을 가지고 있는 길드원이 있습니다.
			 iMessage = 6253;
		break;
	case RC_GUILD_HAVE_NOT_RIGHT:
			//6252	길드 권한이 없습니다.
			iMessage = 6252;
		break;
	case RC_GUILD_CANNOT_COMMAND_MYSELF:
			//5197	자신에게는 이용할 수 없는 기능입니다.
			iMessage = 5197;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY:
			//6251	현재 직책과 바꾸려는 직책이 같아서 직책을 바꿀수 없습니다.
			iMessage = 6251;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY:
			//6195	상위 직책에 있는 길드원에게는 직책을 부여할 수 없습니다.
			iMessage = 6195;
		break;
	case RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER:
		{
			//6271 해당 기능을 이용할 권한이 없습니다.
		}
		break;
	case RC_GUILD_INVALID_DELEGATE_RIGHT:
			//6286	정상적인 권한 위임이 아닙니다.
			iMessage = 6286;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER:
	case RC_GUILD_PLAYER_ALEADY_DUTY:
			//6223 해당 직책을 가진 길드원이 모두 존재하여 더 이상 변경할 수 없습니다.
			iMessage = 6223;
		break;
	case RC_GUILD_CHANGE_MEMBER_DUTY_FAILED:
			//6287	직책 변경에 실패하였습니다. 다시 시도해 주시기 바랍니다.
			iMessage = 6287;
		break;
	case RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED:
			//6288	권한 위임에 실패하였습니다.
			iMessage = 6288;
		break;
	case RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14:
			//6222	길드장을 변경한 지 14일이 경과하지 않아 길드장 위임이 가능하지 않습니다.
			iMessage = 6222;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING:
			//7242	길드 창고를 사용 중인 길드원은 직책 변경이 가능하지 않습니다.
			iMessage = 7242;
		break;

	case RC_GUILD_SELECT_CHAR_NO_GUILD_MEMBER:
			//6200	입력한 아이디가 정확하지 않습니다. 다시 확인 후 입력하시기 바랍니다.
			iMessage = 6200;
		break;
	case RC_GUILD_CREATE_CORPS_FAILED:
			//6289	길드 군단 생성에 실패하였습니다.
			iMessage = 6289;
		break;
	case RC_GUILD_CORPS_NUM_OVERFLOW:
			//6199	더 이상 군단을 창설할 수 없습니다.
			iMessage = 6199;
		break;
	case RC_GUILD_MEMBER_NOT_HEAD_CORPS:
			//6290	해당 길드원은 직할 군단 소속이 아닙니다.
			iMessage = 6290;
		break;
	case RC_GUILD_NOT_SOLDIER:
	case RC_GUILD_MEMBER_NOT_SOLDIER:
			//7161	해당 길드원은 정예군이 아닙니다.
			iMessage = 7161;
		break;
	case RC_GUILD_CORPS_NOT_FOUND:
			//7163	해당 길드에 군단이 생성되지 않았습니다.	
			iMessage = 7163;
		break;
	case RC_GUILD_ALREADY_GUILD_MEMBER:
			//7164	이미 길드에 가입되어 있습니다.
			iMessage = 7164;
		break;
	case RC_GUILD_ALREADY_EXIST_COMMANDER:
			//7166	이미 길드에 해당 군단장이 존재합니다.
			iMessage = 7166;
		break;
	case RC_GUILD_ALREADY_EXIST_TRAINING_LEADER:
			//7167	이미 길드에 해당 훈련대장이 존재합니다.
			iMessage = 7167;
		break;
	case RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER:
			//7168	동일한 직책으로는 변경할 수 없습니다.
			iMessage = 7168;
		break;
	case RC_GUILD_CORPS_FULL_MEMBER:
			//7169	해당 군단에 인원이 꽉 차 더 이상 이동 및 가입할 수 없습니다.
			iMessage = 7169;
		break;
	case RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER:
			//7170	서버 연결이 해제되어 해당 길드 작업에 실패하였습니다. 다시 시도해 주시기 바랍니다.
			iMessage = 7170;
		break;
	case RC_GUILD_INVALID_DUTY_IN_BASE_CORPS:
			//7171	직할 군단에는 존재하지 않는 직책입니다.	
			iMessage = 7171;
		break;
	case RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS:
			//6237	선택한 군단은 이미 길드원이 소속되어 있는 군단입니다.
			iMessage = 6237;
		break;
	case RC_GUILD_ALREADY_EXIST_REAR_CAMP:
			//6201	군단 애 육성캠프가 존재하여 더 이상 창설할 수 없습니다.
			iMessage = 6201;
		break;
	case RC_GUILD_NOT_EXIST_REAR_CAMP:
			//7172	해당 군단에 육성캠프가 존재하지 않습니다.
			iMessage = 7172;
		break;
	case RC_GUILD_CANNOT_LEAVE_MASTER:
			//7173	길드장은 길드 탈퇴를 할 수 없습니다.
			iMessage = 7173;
		break;
	case RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS:
			//7174	직할군단에서는 사용할 수 없는 기능입니다.
			iMessage = 7174;
		break;
	case RC_GUILD_EXIST_MEMBER:
			//6203	군단 내 길드원이 있으면 해산할 수 없습니다.
			iMessage = 6203;
		break;
	case RC_GUILD_ALREADY_EXIST_GUILD:
			//7214	이미 길드가 존재합니다.
			iMessage = 7214;
		break;
	case RC_GUILD_CANNOT_LEVELUP:
			//5182	길드 랭크업에 필요한 조건을 확인해 주시기 바랍니다.
			iMessage = 5182;
		break;
	case RC_GUILD_NOT_CORPS_MEMBER:
			//7212	군단에 속한 멤버가 아닙니다.
			iMessage = 7212;
		break;
	case RC_GUILD_NOT_BASE_CORPS:
			//7213	직할군단이 아니여서 이용할 수 없습니다.
			iMessage = 7213;
		break;
  
	case RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY:
			//7240 해당군단은 길드원 제명 패널티로 인해 군단간 이동이 가능하지 않습니다.
			iMessage = 7240;
		break;
  

	case RC_GUILD_NOT_GUILD_MEMBER:
			//7205	길드 멤버가 아닙니다.
			iMessage = 7205;
		break;

	case RC_GUILD_NOT_EXIST_GUILD_SEAL:
			//6161 길드 인장이 인벤토리에 존재하지 않습니다.
			iMessage = 6161;
		break;

	case RC_GUILD_CHANGE_MARK_NOT_RIGHT:
			//7202	길드 마크를 설정할 수 있는 권한이 없습니다.
			iMessage = 7202;
		break;

	case RC_GUILD_CHANGE_MARK_NOT_DUTY_STATE:
			//7203	해당 기능을 이용할 수 없는 직책입니다.
			iMessage = 7203;
		break;
	case RC_GUILD_MARK_LEVEL_LOW:
			//7201	길드 레벨이 낮아 길드 마크를 등록할수 없습니다.
			iMessage = 7201;
		break;
	case RC_GUILD_MARK_ALEADY_EXIST:
			//7200	이미 등록되어 있는 길드 마크입니다.
			iMessage = 7200;
		break;

	case RC_GUILD_MARK_DELETE_FAILED:
			//7218	길드 마크 삭제에 실패했습니다.
			iMessage = 7218;
		break;

	case RC_GUILD_CANNOT_CREAT_SAMENAME_STATE:
			// 7175 동일한 이름의 길드가 있습니다. 다시 입력해 주십시오.
			iMessage = 7175;
		break;

	case RC_GUILD_KICK_GUILD_BY_LEVELUP:
			//31레벨에 달성하여 육성 캠프에서 강제 탈퇴되었습니다.
			iMessage = 7197;
		break;
	case RC_GUILD_NOT_JOIN_GUILD:
			//5165	길드에 가입되어 있지 않습니다.
			iMessage = 5165;
		break;
	case RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL:
			//7232	16레벨 이상의 캐릭터는 훈련병으로 가입할 수 없습니다.
			iMessage = 7232;
		break;
	case RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL:
			//7211 15레벨 이하만 훈련병으로 변경할 수 있습니다
			iMessage = 7211;
		break;
	case RC_GUILD_CORPS_PENALTY_TIME:
			//7238 길드원을 제명하여 해당 군단의 가입 권유를 3일간 할 수 없습니다.
			iMessage = 7238;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_GRADE:
			//7250 길드명 변경 권한이 없습니다.
			iMessage = 7250;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB:
	case RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM:
			//7251 길드명 변경에 실패 했습니다.
			iMessage = 7251;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME:
			// 7175 동일한 이름의 길드가 있습니다. 다시 입력해 주십시오.
			iMessage = 7175;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE:
			//70022 사용이 금지된 단어입니다.
			iMessage = eST_CANNOT_USE_WORD;
		break;

	case RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD:
		// 3343	피의 성전 내부에서는 길드탈퇴가 불가능하여 피의성전 입구로 이동합니다. 재시도 바랍니다.
		iMessage = 3343;
		break;
	case RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD:
		//3378 현재 해당 길드원이 피의성전 내부에 있어 제명을 위하여 피의성전 입구로 강제 이동 시킵니다. 재시도 바랍니다.
		iMessage = 3378;
		break;
	case RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY:
		// 3373	'피의 성전' 점령전 입찰 기록이 존재하여 길드해체가 불가능 합니다.
		iMessage = 3373;
		break;
	case RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY:
		// 3381	피의 성전' 점령전에 참여 중인 길드는 길드해체가 불가능 합니다.
		iMessage = 3381;
		break;

	case RC_GUILD_WAREHOUSE_GET_CANNOT_DB:
	default:
		{
			//5164	길드 관련한 알 수 없는 결과를 받았습니다.
			iMessage = 5164;

#ifdef _DEV_VER
			TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};
			Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T("에러코드 %d") , byErrorCode);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);
#endif
		}
        break;
    }
	
	g_InterfaceManager.GetInterfaceString( iMessage, szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("[GUILDERR]'%d'"), byErrorCode);
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
