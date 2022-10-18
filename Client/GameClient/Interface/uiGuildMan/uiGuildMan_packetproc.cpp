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
void _callback_Guild_Relation_Request(bool bYes) //���� ��û �䱸�� ���Ұ��̳�.
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
                //  ��� ����
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

                    PLAYERKEY	m_MasterPlayerKey = recvMsg->m_MasterPlayerKey;							//< ��� â���� �÷��̾� Ű
					
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
                //  ����常 ��� ������ �����ü
                //

            case CG_GUILD_DESTROY_ACK:
                {
                    MSG_CG_GUILD_DESTROY_ACK *recvMsg =
                        (MSG_CG_GUILD_DESTROY_ACK *)pMsg;

					//	��带 �ػ� �ϼ̽��ϴ�.
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
                    //GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("'%d' �����ڵ�� �Բ� NAK�� �Խ��ϴ�."), recvMsg->m_byErrorCode);
                }
                break;

            case CG_GUILD_DESTROY_BRD:
                {
                    MSG_CG_GUILD_DESTROY_BRD *recvMsg =
                        (MSG_CG_GUILD_DESTROY_BRD *)pMsg;

                    PLAYERKEY	m_MasterPlayerKey = recvMsg->m_MasterPlayerKey;							//< ��� �ػ��� �÷��̾� Ű

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
                //  ��� �ʴ�
                //

            case CG_GUILD_JOIN_FAILED_CMD:
                {
                    MSG_CG_GUILD_JOIN_FAILED_CMD *recvMsg =
                        (MSG_CG_GUILD_JOIN_FAILED_CMD *)pMsg;

					uiGuildMan::ProcessErrorCode(recvMsg->m_byErrorCode);
                    //GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("'%d' �����ڵ�� �Բ� NAK�� �Խ��ϴ�."), recvMsg->m_byErrorCode);
                }
                break;

			//��� �ʴ� �޴� ������� CMD(��� �ʴ� â Show)
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
			//��� �ʴ� ��� CMD(�޼��� ���)
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
					//	��� ������ �����߽��ϴ�.
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
					// ��� ��� ����
					m_vMembers.clear();
					m_vMembers.push_back(pRecvMsg->m_MemberInfo);

                    // ĳ���� ���� ���ӽ� ����� �������� �ִ��� ��û
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
						//6263	���� %s ���� �α����Ͽ����ϴ�.
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
						//6264	���� %s ���� �α׾ƿ��Ͽ����ϴ�.
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
					//5208	%s���� ��忡 �����ϼ̽��ϴ�.
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
						//	��带 Ż�� �ϼ̽��ϴ�.
						g_InterfaceManager.GetInterfaceString( eST_GO_OUT_GUILD, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					SetRelationNoneByGuildMember();//<- ��尡 Ż������Ƿ� �ֺ� ������ ���踦 ����.
					this->Clear();					
                }
                break;

			case CG_GUILD_WITHDRAW_BRD:
				{
					MSG_CG_GUILD_WITHDRAW_BRD * pRecvMsg = (MSG_CG_GUILD_WITHDRAW_BRD *) pMsg;

					TCHAR tszCharName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL ó�� Ȯ���� �ϱ� ���Ͽ�
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
						//7225 %s ���� 31������ �޼��Ͽ� ��忡��  Ż��Ǿ����ϴ�.
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
					//	Ż�� ��ְ� �߻��Ͽ����ϴ�.\n\n�������� �����߿� ������,\n���� �� ���� �ϵ��� �ϰڽ��ϴ�.\n������ ���ĵ�� ����� �˼��մϴ�.
					
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

					////5171	%s'������ ����Ǿ����ϴ�.
					g_InterfaceManager.GetInterfaceString(5171, szTemp, INTERFACE_STRING_LENGTH);
					Sprintf(szMessage, szTemp, szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_KICKPLAYER_CMD:
				{
					//5169	��忡�� ����Ǿ����ϴ�.	
					g_InterfaceManager.GetInterfaceString(5169, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					SetRelationNoneByGuildMember(); //<- ��尡 Ż������Ƿ� �ֺ� ������ ���踦 ����.
					m_pUIGuildMain->ShowInterface(FALSE);
					this->Clear();
				}
				break;



			case CG_GUILD_KICKPLAYER_NAK:	// ���� Ż�� ����(����忡�Ը� ����)
				{
					MSG_CG_GUILD_KICKPLAYER_NAK *recvMsg =
						(MSG_CG_GUILD_KICKPLAYER_NAK *)pMsg;

					if((recvMsg->m_dwErrorCode == 0) || (recvMsg->m_dwErrorCode == RC_GUILD_CANNOT_KICK_STATE))
					{
						TCHAR tszKickCharName[MAX_CHARNAME_LENGTH+1] = {0,};
						StrnCopy(tszKickCharName, recvMsg->m_ptszKickCharName, MAX_CHARNAME_LENGTH);

						//5170	%s'���� ���� �����߽��ϴ�.
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
				
					g_pHero->SetMoney(recvMsg->m_RemainMoney);	// ��� �����Ϳ��� ���� �´�.

					// �����մϴ�! ��� ��ũ���� �����߽��ϴ�. 5181
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

					// �����մϴ�! ��� ��ũ���� �����߽��ϴ�. 5181
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
					// ��� ��ũ���� �����߽��ϴ�. 5182
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
					// GP(�������Ʈ)�� ��ȯ�Ǿ����ϴ�. 5183
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
					m_GuildInfo.m_UserPoint = recvMsg->m_GuildUP;  // ���UP�� ���� ó��
					// UP(��������Ʈ)�� ���Ͽ����ϴ�. 5184
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

			case CG_GUILD_ADD_INFO_ACK://������ ��ü ���� Send
				{
					MSG_CG_GUILD_ADD_INFO_ACK* recvMsg = (MSG_CG_GUILD_ADD_INFO_ACK*)pMsg;
					BYTE btRelCnt  = recvMsg->m_byRelCnt;
					BYTE btCorpCnt = recvMsg->m_byCorpsCnt;
					BASE_GUILD_ADD_INFO stGuildAddInfo = recvMsg->m_AddInfo;

					//������� ���� �޴´�.
					DelRelationCon(); 
					for(int i = 0; i < btRelCnt; ++i)
					{
						GUILD_RELATION* pGuildRelation = new GUILD_RELATION;
						memcpy(pGuildRelation, &stGuildAddInfo.m_stRelationGuild[i], sizeof(GUILD_RELATION));

						//////////////////////////////////////////////////////////////////////////
						//�ֺ� ĳ���͵��� m_RelationType�� �����Ѵ�.
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
								assert(!"GUILD_RELATION_TYPE �߸�����Դ�");
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

			case CG_GUILD_RELATION_REQUEST_CMD:  //�渶(�Ǵ� ����)�� ���� ���� ���� ���� ����
				{
					MSG_CG_GUILD_RELATION_REQUEST_CMD* recvMsg = (MSG_CG_GUILD_RELATION_REQUEST_CMD*)pMsg;

					TCHAR szGuildName[MAX_GUILDNAME_LENGTH + 1] = {0,};
					//YJ
					StrnCopy(szGuildName, recvMsg->m_RequestGuildName, MAX_GUILDNAME_LENGTH);

					uiGuildAllianceRequest* pGuildAllianceRequest = GET_CAST_DIALOG( uiGuildAllianceRequest, IM_GUILD_MANAGER::GUILD_ALLIANCE_REQUEST );
						
					//������� ���θ� ���� ���� ���Ͱ��� ��!
					if(pGuildAllianceRequest)
					{
						pGuildAllianceRequest->SetGuildName(szGuildName);
						pGuildAllianceRequest->SetGuildRelationType(recvMsg->m_eType);
						pGuildAllianceRequest->SetCharGUID(recvMsg->m_RequestUserKey);
						pGuildAllianceRequest->ShowInterface(TRUE);
					}

				}
				break;

			case CG_GUILD_RELATION_REJECT_ACK: //���� ��û �źδ��ϴ�.
				{
					MSG_CG_GUILD_RELATION_REJECT_ACK* recvMsg = (MSG_CG_GUILD_RELATION_REJECT_ACK*)pMsg;
					TCHAR szGuildName[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
					StrnCopy(szGuildName, recvMsg->m_szRelGuildName, INTERFACE_STRING_LENGTH);
					//6177	%s ����� ��� �����Ͱ� ���� ��û�� �����Ͽ����ϴ�.
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


			case CG_GUILD_RELATION_BRD: //���� ��û �����ߴ�.
				{
					MSG_CG_GUILD_RELATION_BRD* recvMsg = (MSG_CG_GUILD_RELATION_BRD*)pMsg;
					GUILD_RELATION* pGuildRelation = new GUILD_RELATION;
					memcpy(pGuildRelation, &recvMsg->m_stRELGuild, sizeof(GUILD_RELATION));

					//�������� ������ ��带 ������ �����ִ�. �Ÿ���.(�Ÿ��������� �ڽ��� ���� ����,������踦 �δ� ��Ȳ�� �߻�)
					if(pGuildRelation->m_GuildGuid == GetGuildInfo().m_GuildGuid)
						return;

					TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

					switch(pGuildRelation->m_eRelationType)
					{
					case eGUILD_RELATION_ALLIENCE:
						{
							//6183 %s ���� ���Ͱ��谡 �����Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString(6183, szTemp);

							m_listGuildRelation_Alliance.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_BOTH_HOSTILITY:
						{
							//6186 %s ���� �ֹ� ������谡 �Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString(6186, szTemp);

							m_listGuildRelation_Hostility.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_PROCLAIM_HOSTILITY:
						{
							//6184 %s ��忡�� ���� ������ �Ͽ����ϴ�.
							g_InterfaceManager.GetInterfaceString(6184, szTemp);

							m_listGuildRelation_Hostility.push_back(pGuildRelation);
						}
						break;

					case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
						{
							//6185 %s ���� �Ϲ����� ������谡 �����Ǿ����ϴ�.
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

			case CG_GUILD_RELATION_BOTH_AGAINST_BRD: //�ֹ��������
				{
					MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD* recvMsg = (MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD*)pMsg;

					GUILDGUID gGuid = recvMsg->m_BothGuildGuid;

					//�������� ������ ��带 ������ �����ִ�. �Ÿ���.
					if(gGuid == GetGuildInfo().m_GuildGuid)
						return;

					if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == recvMsg->m_eDELType)//�Ϲ�����
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
								//����
								SAFE_DELETE(pDelRelation);
								m_listGuildRelation_OneSideHostility.erase(iter);		
								//�߰�
								pAddRelation->m_eRelationType = eGUILD_RELATION_BOTH_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
								TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
								//6186 %s ���� �ֹ� ������谡 �Ǿ����ϴ�.
								g_InterfaceManager.GetInterfaceString(6186, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);

								break;
							}
						}
					}
					else if(eGUILD_RELATION_PROCLAIM_HOSTILITY == recvMsg->m_eDELType)//���� ����
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

								//����
								SAFE_DELETE(pDelRelation);
								m_listGuildRelation_Hostility.erase(iter);		
								//�߰�
								pAddRelation->m_eRelationType = eGUILD_RELATION_BOTH_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
								TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
								//6186 %s ���� �ֹ� ������谡 �Ǿ����ϴ�.
								g_InterfaceManager.GetInterfaceString(6186, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);

								break;
							}
						}
					}
					else
					{
						assert(!"MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD �߸� ����Դ�");
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

					//�������� ������ ��带 ������ �����ִ�. �Ÿ���.(�Ÿ��������� �ڽ��� ���� ����,������踦 �����ϴ� ��Ȳ�� �߻�)
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

							//5199	%s ���� ���Ͱ��谡 �����Ǿ����ϴ�.	
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

							//6187 %s ���� ������谡 ���� �Ǿ����ϴ�.
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

							//6187 %s ���� �Ϲ����� ������� ����
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

					//�������� ������ ��带 ������ �����ִ�. �Ÿ���.
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
							//����
							SAFE_DELETE(pDelRelation);
							m_listGuildRelation_Hostility.erase(iter);

							//�߰�
							if(eGUILD_RELATION_PROCLAIM_HOSTILITY == recvMsg->m_eMOVType)//���� ����
							{
								pAddRelation->m_eRelationType = eGUILD_RELATION_PROCLAIM_HOSTILITY;

								m_listGuildRelation_Hostility.push_back(pAddRelation);

								//6188 %s ��尡 ������踦 �����Ͽ� %s����� �Ϲ����� ������谡 �Ǿ����ϴ�.
								g_InterfaceManager.GetInterfaceString(6188, szTemp);
								Snprintf(szFinal, INTERFACE_STRING_LENGTH, szTemp, pAddRelation->m_GuildName, g_pHero->GetGuildName());
							}
							else if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == recvMsg->m_eMOVType)//�Ϲ��� ����
							{
								pAddRelation->m_eRelationType = eGUILD_RELATION_ONE_SIDE_HOSTILITY;

								m_listGuildRelation_OneSideHostility.push_back(pAddRelation);

								//6188 %s ��尡 ������踦 �����Ͽ� %s����� �Ϲ����� ������谡 �Ǿ����ϴ�.
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

			//��� ����
			case CG_GUILD_RECOVERY_GUILD_ACK:
				{
					m_GuildInfo.m_GuildDestroyWaitFlag = eGUILD_DELCHK_NORMAL;

					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					//6234 ��� �ػ� ���� ��û�� �Ϸ� �Ǿ����ϴ�.
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

			//��� �ػ� ���
			case CG_GUILD_DESTROY_WAIT_ACK:
				{
					MSG_CG_GUILD_DESTROY_WAIT_ACK* pRecvMsg = (MSG_CG_GUILD_DESTROY_WAIT_ACK*)pMsg;

					m_GuildInfo.m_GuildDestroyWaitFlag = eGUILD_DELCHK_DESTROY_WAIT;

					//6228	��� �ػ� ��� ���°� �Ǿ����ϴ�.
					g_InterfaceManager.GetInterfaceString( 6228, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					m_pUIGuildMenu->ShowInterface(FALSE);
					m_pUIGuildMain->ShowInterface(FALSE);

					//����ػ� ��û�� �����Ǹ� ��� ����� ��������
					this->DelRelationCon();
					//�ֺ� �÷��̾� ���� ����
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

					//�ֺ� �÷��̾� ���� ����
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
							//7236	%s��尡 �ػ� ��û�� �Ͽ� �ش� ������ ��� ����� ��� ���� �Ǿ����ϴ�.
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
							//6276	%s���� ��å�� %s�� ����Ǿ����ϴ�.
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
					//6277	%s �Կ��� ������ �����Ͽ����ϴ�
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1,g_InterfaceManager.GetInterfaceString(6277), szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_GUILD_RIGHT_DELEGATION_CMD:
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
					//6278	���ο� ������ ���� �޾ҽ��ϴ�.
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

					TCHAR tszOldMasterName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL ó�� Ȯ���� �ϱ� ���Ͽ�
					StrnCopy(tszOldMasterName, pRecvMsg->m_szOldMasterName, MAX_CHARNAME_LENGTH);

					TCHAR tszNewMasterName[MAX_CHARNAME_LENGTH +1] ={0,}; // NULL ó�� Ȯ���� �ϱ� ���Ͽ�
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
						assert(!"��帶���ͺ���ƴµ� Ÿ���÷��̾ ����");
					}

					//6279	��帶���Ͱ� %s������ ����Ǿ����ϴ�.
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
						//GUILD_CORPS_TOTAL_INFO ����
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bCreate = true;
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_byCorpsNum = pRecvMsg->m_byCorpsNum;
						m_GuildCorpsTotalInfo.m_CorpsInfo[pRecvMsg->m_byCorpsNum].m_bRearCamp = false;

						//GUILD_MEMBER_INFO ����
						TCHAR szTargetName[MAX_CHARNAME_LENGTH +1] ={0,};
						Snprintf(szTargetName, MAX_CHARNAME_LENGTH,pRecvMsg->m_szTargetCharName );

						stGUILD_MEMBER_INFO * pMemberInfo =	GetGuildMemberInfo(szTargetName);
						if( pMemberInfo )
						{
							pMemberInfo->m_byCorpsNum = pRecvMsg->m_byCorpsNum;
							pMemberInfo->m_eGuildDuty = eGUILD_DUTY_COMMANDER;
						}

						

						TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
						//6280	%s ������ â���Ǿ����ϴ�. %s���� ���������� �Ӹ�Ǿ����ϴ�.
						Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6280),
							GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum), szTargetName);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						assert(!"����â�������� �Ѿ���");
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

					//6207 �� %s ������ �ػ�Ǿ����ϴ�.
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(6207,szMessage);
					Snprintf(szResult, INTERFACE_STRING_LENGTH-1,szMessage, GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);

					//GUILD_CORPS_TOTAL_INFO ����
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
					//6282	%s���� %s ���������� ����Ǿ����ϴ�.
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

					//���� ���� ����
					stGUILD_MEMBER_INFO * pMember = GetGuildMemberInfo(szTargetName);
					if( pMember)
					{
						pMember->m_byCorpsNum = pRecvMsg->m_byCorpsNum;
					}				

					//6268	%s ���� %s�������� ����Ǿ����ϴ�.
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
					//6283	%s ���� ����ķ���� �����Ǿ����ϴ�. %s���� ����ķ�������� �Ӹ�Ǿ����ϴ�.
					TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetInterfaceString(6283, szTemp);
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szTemp , 
							GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum), szTargetName);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					//��å ����
					stGUILD_MEMBER_INFO * pMemberInfo = GetGuildMemberInfo(szTargetName);
					if( pMemberInfo)
					{
						pMemberInfo->m_eGuildDuty = eGUILD_DUTY_CAMP_LEADER;
					}


					// GUILD_TOTAL_CORPS_INFO ����
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
					//6208 �� %s ���ܿ� ����ķ���� �ػ�Ǿ����ϴ�.
					TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(6208,szMessage);
					Snprintf(szResult, INTERFACE_STRING_LENGTH-1,szMessage, GlobalFunc::GetCorpsName(pRecvMsg->m_byCorpsNum));
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);

					// GUILD_TOTAL_CORPS_INFO ����
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
					// ��� ��� ����
					m_vMembers.clear();
					for( int i = 0; i < pRecvMsg->m_MemberTotalInfo.m_byMemberCount; ++i)
					{
						m_vMembers.push_back(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i]);
						if(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_eGuildDuty == eGUILD_DUTY_MASTER)
						{
							SetGuildMasterName(pRecvMsg->m_MemberTotalInfo.m_MemberInfo[i].m_tszCharName);
						}

						//���� ����� �Ͽ��� ���Ͱ���� ó���Ѵ�.
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
                    //! 2011.7.22 / i4u4me / ĳ�ü� �۾� 
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
					//GM ��ɾ�
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
						//7241	��� ��ũ�� ���������� �����Ǿ����ϴ�.
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
					// 7218	��� ��ũ ������ �����߽��ϴ�.
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
						//6127	��帶ũ�� ��� �Ǿ����ϴ�.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6127));
					}

					Player * pTargetPlayer = (Player *) g_ObjectManager.GetObject(pRecvMsg->m_dwPlayerKey);
					if( pTargetPlayer )
					{
						pTargetPlayer->GetGuildRenderInfo().m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
						pTargetPlayer->GetGuildRenderInfo().m_PatternIndex = pRecvMsg->m_PatternIndex;
						//////////////////////////////////////////////////////////////////////////
						//��ũ ����� �÷��̾���� ��� ���踦 �����Ѵ�.

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
					//6165	��� ��ũ ��Ͽ� �����Ͽ����ϴ�. �ٽ� ����� �ֽʽÿ�.	
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
						//��ũ ����� �÷��̾���� ��� ���踦 �����Ѵ�.

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
					//�ϴ� ����
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

					//7252	������ %s�� ����Ǿ����ϴ�.
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

					//7253	%s��尡 %s���� ������ �����Ͽ����ϴ�.
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
					//7196	��� ������ ���� ���߽��ϴ�.
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
					//safecode:�������� �����Ⱚ����������� ���
					if(recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;
						

					tszNotice[recvMsg->m_wNoticeLen] = NULL;

					if(Strcmp(tszNotice, m_tszNotice))	// ���ο� ���� �����϶��� ���
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	��� ������ ���޵Ǿ����ϴ�.
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
					//safecode:�������� �����Ⱚ����������� ���
					if(recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;
					      
					
					tszNotice[recvMsg->m_wNoticeLen] = NULL;

					if(Strcmp(tszNotice, m_tszNotice))	// ���ο� ���� �����϶��� ���
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	��� ������ ���޵Ǿ����ϴ�.
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

					if(Strcmp(tszNotice, m_tszNotice))	// ���ο� ���� �����϶��� ���
					{
						ZeroMemory(m_tszNotice, sizeof(m_tszNotice));
						StrnCopy(m_tszNotice, tszNotice, MAX_GUILDNOTICE_LENGTH);

						//5174	��� ������ ���޵Ǿ����ϴ�.
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
					//5176	��� ���� ���濡 �����߽��ϴ�.	
					g_InterfaceManager.GetInterfaceString(5176, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;


			//�α�â �������� ����.
			case CW_GUILD_WAREHOUSE_LOGLIST_ANS:
				{
					MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS* pRecvMsg = (MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS*)pMsg;
					uiGuildWareHouseLog * pDlg = GET_CAST_DIALOG( uiGuildWareHouseLog, IM_GUILD_MANAGER::GUILD_WAREHOUSE_LOG );
					
					if(pDlg)
					{
						if(1 == pRecvMsg->m_byCurrentPage) //ó�� �޴°Ÿ� �����̳� ������ �ϰ� (1�������� �޴´�)
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
					
					//COMMENT: �αװ� 0�϶� �̻��� ����޼��� �����ٸ��� �ϴ� �ּ�
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
			//5151	��带 â���� �� �����ϴ�.
			iMessage = 5151;
        break;

    case RC_GUILD_CANNOT_CREAT_LEVEL_STATE:
			//5152	������ ���� ��带 â���� �� �����ϴ�.
			iMessage = 5152;
        break;

    case RC_GUILD_CANNOT_CREAT_MONEY_STATE:
			//5153	������ �����Ͽ� ��带 â���� �� �����ϴ�.
			iMessage = 5153;
        break;

    case RC_GUILD_CANNOT_DESTROY_STATE:
			//5154	��带 ��ü�� �� �����ϴ�.
			iMessage = 5154;
        break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT:
    case RC_GUILD_CANNOT_DESTROY_NOT_MASTER_STATE:
			//5154	��带 ��ü�� �� �����ϴ�.
			iMessage = 5154;
        break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST:
	case RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE:
			//5155	������ ������ ��� ��带 ��ü�� �� �����ϴ�.
			iMessage = 5155;
		break;

    case RC_GUILD_NOT_EXIST_TARGET_STATE:
			//5156	��忡 �ʴ��� �÷��̾ ������ �ּ���
			iMessage = 5156;
        break;

    case RC_GUILD_CANNOT_INVITE_STATE:
			//5157	��忡 �ʴ��� �� ���� �����Դϴ�.
			iMessage = 5157;
        break;

    case RC_GUILD_DONT_HAVE_RIGHTS_STATE:
			//5158	��� �ʴ� ������ �����ϴ�.
			iMessage = 5158;
        break;

    case RC_GUILD_FULL_MEMBER_STATE:
			//5159	�� �̻� ������ �߰��� �� �����ϴ�. ��� ������ Ȯ���� �ֽʽÿ�.
			iMessage = 5159;
        break;

    case RC_GUILD_NOT_EXIST_HOST_STATE:
			//5157	��忡 �ʴ��� �� ���� �����Դϴ�.
			iMessage = 5157;
        break;

    case RC_GUILD_CANNOT_WITHDRAW_STATE:
			//5164	��� ������ �� �� ���� ����� �޾ҽ��ϴ�.
			iMessage = 5164;
        break;

    case RC_GUILD_NOT_EXIST_GUILD:
			//5160	�������� �ʴ� ����Դϴ�.
			iMessage = 5160;
        break;

    case RC_GUILD_CANNOT_JOIN_STATE:
			//5161	��忡 ������ �� �����ϴ�.
			iMessage = 5161;
        break;

    case RC_GUILD_CANNOT_SELECT_GUILD_STATE:
			//5162	��� ������ �ҷ����� ���߽��ϴ�.
			iMessage = 5162;
        break;

    case RC_GUILD_WAIT_A_WHILE_THEN_TRY_AGAIN:
			//5163	��� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
			iMessage = 5163;
        break;

	case RC_GUILD_CANNOT_KICK_STATE:
			//5168	���� ���� �����߽��ϴ�.
			iMessage = 5168;
		break;
	
	case RC_GUILD_CANNOT_KICK_RIGHT:
			//5167	���� ���� ������ �����ϴ�.	
			iMessage = 5167;
		break;

	case RC_GUILD_NOT_ENOUGH_UP:
			// UP(��������Ʈ)�� �����մϴ�. 5185
			iMessage = eST_NOT_ENOUGH_UP;
		break;

	case RC_GUILD_NOT_ENOUGH_GP:
			// GP(�������Ʈ)�� �����մϴ�. 5186
			iMessage = eST_NOT_ENOUGH_GP;
		break;

	case RC_GUILD_CANNOT_RANKUP_MONEY_STATE:
			// ������ ���� �մϴ�. : 551
			iMessage = eST_NOT_ENOUGH_MONEY;
		break;

	case RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE:
			// ���̻� ��ũ �� �Ҽ� �����ϴ�.
			iMessage = 5195;
		break;

	case RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE:
			// 5275	ī�� ���¿����� �̿��� �������� �ʽ��ϴ�.
			iMessage = 5275;
		break;

	case RC_GUILD_MARK_INVALID_TYPE:
			// 1198 ��� ��ũ ���Ŀ� ��߳�
			iMessage = 1198;
		break;

	case RC_GUILD_MARK_CREATE_FAILED:
			// 1200 ��� ��ũ ���� ����
			iMessage = 1200;
		break;

	case RC_GUILD_CURRENT_NONE_MASTER:
			//6175 ��� �����Ͱ� �������� �ʽ��ϴ�.
			iMessage = 6175;
		break;

	case RC_GUILD_RELATION_REMAIN_GUILD:
	case RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN:
			//7195	�����谡 �����־ ��带 ��ü�� �� �����ϴ�.
			iMessage = 7195;
		break;

	case RC_GUILD_RELATION_LIST_ERROR:
			//7176	�������� ������ �߸��Ǿ����ϴ�.
			iMessage = 7176;
		break;

	case RC_GUILD_RELATION_ME_FULL:
			//7177	�ش� �������� ���� ��ġ�� �ʰ��Ͽ� �� �̻� �ش� �����踦 ���� �� �����ϴ�.
			iMessage = 7177;
		break;

	case RC_GUILD_RELATION_OTHER_FULL:
			//7178	���� ��尡 �ش� �������� ���� ��ġ�� �ʰ��Ͽ� ���̻� �ش� �����踦 ���� �� �����ϴ�.
			iMessage = 7178;
		break;

	case RC_GUILD_RELATION_CHANGE_RIGHT_NOT:
			//7179	��� ���踦 ������ ������ �����ϴ�.
			iMessage = 7179;
		break;

	case RC_GUILD_RELATION_ALEADY_SET:
			//7180	��� ���踦 �������� ���� �̹� �ٸ� ���谡 �����Ǿ� �ֽ��ϴ�.
			iMessage = 7180;
		break;

	case RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED:
			//7181	������ �������� ��尡 �ڽ��� ���� ���� ���踦 �ΰ� �ִ� ���� ���Ͱ���� ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7181;
		break;

	case RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED:
			//7182	������ �������� ��尡 �ڽ��� ���� ������ �ΰ� �ִ� ���� ���� ����� ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7182;
		break;

	case RC_GUILD_RELATION_HOSTILITY_FAILED:
			//7194	���븦 �������� ��尡 �ڽ��� ���� ���� ���踦 �ΰ� �ִ� ���� ���� ���迩�� ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7194;
		break;

	case RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED:
			//7183	��� ���� �ڽ��� ��尡 �Ϲ��� ���� ���� �Ǵ� ���� ���� ���谡 �ƴϱ� ������ �ֹ� ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7183;
		break;

	case RC_GUILD_RELATION_NONE_DELETE_NOT:
			//7184	��� ���� ��� ���赵 �ΰ� ���� �ʱ� ������ ���踦 ������ �� �� �����ϴ�.
			iMessage = 7184;
		break;

	case RC_GUILD_RELATION_GUILD_DESTROY_WAIT:
			//7185	��� �ػ� ������̶� ��� ���踦 ���� �� �����ϴ�.
			iMessage = 7185;
		break;

	case RC_GUILD_RELATION_GUILD_GRADE_LOW_ME:
			//7215	��� ������ ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7215;
		break;

	case RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER:
			//7216	���� ����� ������ ���� ���踦 ���� �� �����ϴ�.
			iMessage = 7216;
		break;

	case RC_GUILD_RELATION_DELETE_FAILED: //��� ���� ���� ����
			//7219	��� ���� ������ �����߽��ϴ�.
			iMessage = 7219;
		break;

  	case RC_GUILD_RELATION_DELETE_FAILED_BY_TIME:
			//7239 ������踦 ��û�� �� 1�ð��� ������ ������ �����մϴ�.
			iMessage = 7239;
		break;
  
	case RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE:
			//7186	��� ����� ���� ��� â�� �̿��� �� �����ϴ�.
			iMessage = 7186;
		break;

	case RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER:
			//7187	��� â�� ������� ������ �ֽ��ϴ�.
			iMessage = 7187;
		break;

	case RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD:
			//7188	������ ���â�� ������ ���� ����� ��� â�� ������ ��ġ���� �ʽ��ϴ�.
			iMessage = 7188;
		break;

	case RC_GUILD_WAREHOUSE_INVENMONEY_LOW:
			////551 ������ �����մϴ�.
			iMessage = 551;
		break;

	case RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT:
			//7192	��� â���� ������ ����� ������ �����ϴ�.
			iMessage = 7192;
		break;

	case RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT:
			//7193	���� �Ѱ�ġ�� �ʰ��Ͽ����ϴ�.
			iMessage = 7193;
		break;

	case RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN:
			//7237	���â�� ������ �Ǵ� ������ �����־� ��� �ػ� ��û�� �� �� �����ϴ�.
			iMessage = 7237;
		break;

  	case RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING:	
			//7243	��� â�� ������̿��� Ż��/������ �� ���� ����.
			iMessage = 7243;
		break;
  

	case RC_GUILD_DESTROY_PENALTY_TIME:
			//6213	��尡 �ػ�� �� 3���� ������ �ʾ� ��� ���� �� â���� �� �� �����ϴ�.
			iMessage = 6213;
		break;

	case RC_GUILD_WITHDRAW_PENALTY_TIME:
	case RC_GUILD_KICK_PENALTY_TIME:
			//6210	��带 Ż���ϰų� ���� ���� �� 3���� ������ �ʾ� ��忡 ������ �� �����ϴ�.
			iMessage = 6210;
		break;

	case RC_GUILD_RECOVERY_FAILED:
			//6235	��� �ػ� ���� ��û�� �����Ͽ����ϴ�.
			iMessage = 6235;
		break;

	case RC_GUILD_DELCHK_DESTROY_WAIT:
			//6236	��� �ػ� ��� ���Դϴ�.
			iMessage = 6236;
		break;

	case RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT:
			//7206 ����� ������ ���ų�,���� ���� ������ �����ϴ�
			iMessage = 7206;
		break;

	case RC_GUILD_ALREADY_EXIST_DUTY:
			//6253	�̹� �ش� ��å�� ������ �ִ� ������ �ֽ��ϴ�.
			 iMessage = 6253;
		break;
	case RC_GUILD_HAVE_NOT_RIGHT:
			//6252	��� ������ �����ϴ�.
			iMessage = 6252;
		break;
	case RC_GUILD_CANNOT_COMMAND_MYSELF:
			//5197	�ڽſ��Դ� �̿��� �� ���� ����Դϴ�.
			iMessage = 5197;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY:
			//6251	���� ��å�� �ٲٷ��� ��å�� ���Ƽ� ��å�� �ٲܼ� �����ϴ�.
			iMessage = 6251;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY:
			//6195	���� ��å�� �ִ� �������Դ� ��å�� �ο��� �� �����ϴ�.
			iMessage = 6195;
		break;
	case RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER:
		{
			//6271 �ش� ����� �̿��� ������ �����ϴ�.
		}
		break;
	case RC_GUILD_INVALID_DELEGATE_RIGHT:
			//6286	�������� ���� ������ �ƴմϴ�.
			iMessage = 6286;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER:
	case RC_GUILD_PLAYER_ALEADY_DUTY:
			//6223 �ش� ��å�� ���� ������ ��� �����Ͽ� �� �̻� ������ �� �����ϴ�.
			iMessage = 6223;
		break;
	case RC_GUILD_CHANGE_MEMBER_DUTY_FAILED:
			//6287	��å ���濡 �����Ͽ����ϴ�. �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
			iMessage = 6287;
		break;
	case RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED:
			//6288	���� ���ӿ� �����Ͽ����ϴ�.
			iMessage = 6288;
		break;
	case RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14:
			//6222	������� ������ �� 14���� ������� �ʾ� ����� ������ �������� �ʽ��ϴ�.
			iMessage = 6222;
		break;
	case RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING:
			//7242	��� â�� ��� ���� ������ ��å ������ �������� �ʽ��ϴ�.
			iMessage = 7242;
		break;

	case RC_GUILD_SELECT_CHAR_NO_GUILD_MEMBER:
			//6200	�Է��� ���̵� ��Ȯ���� �ʽ��ϴ�. �ٽ� Ȯ�� �� �Է��Ͻñ� �ٶ��ϴ�.
			iMessage = 6200;
		break;
	case RC_GUILD_CREATE_CORPS_FAILED:
			//6289	��� ���� ������ �����Ͽ����ϴ�.
			iMessage = 6289;
		break;
	case RC_GUILD_CORPS_NUM_OVERFLOW:
			//6199	�� �̻� ������ â���� �� �����ϴ�.
			iMessage = 6199;
		break;
	case RC_GUILD_MEMBER_NOT_HEAD_CORPS:
			//6290	�ش� ������ ���� ���� �Ҽ��� �ƴմϴ�.
			iMessage = 6290;
		break;
	case RC_GUILD_NOT_SOLDIER:
	case RC_GUILD_MEMBER_NOT_SOLDIER:
			//7161	�ش� ������ �������� �ƴմϴ�.
			iMessage = 7161;
		break;
	case RC_GUILD_CORPS_NOT_FOUND:
			//7163	�ش� ��忡 ������ �������� �ʾҽ��ϴ�.	
			iMessage = 7163;
		break;
	case RC_GUILD_ALREADY_GUILD_MEMBER:
			//7164	�̹� ��忡 ���ԵǾ� �ֽ��ϴ�.
			iMessage = 7164;
		break;
	case RC_GUILD_ALREADY_EXIST_COMMANDER:
			//7166	�̹� ��忡 �ش� �������� �����մϴ�.
			iMessage = 7166;
		break;
	case RC_GUILD_ALREADY_EXIST_TRAINING_LEADER:
			//7167	�̹� ��忡 �ش� �Ʒô����� �����մϴ�.
			iMessage = 7167;
		break;
	case RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER:
			//7168	������ ��å���δ� ������ �� �����ϴ�.
			iMessage = 7168;
		break;
	case RC_GUILD_CORPS_FULL_MEMBER:
			//7169	�ش� ���ܿ� �ο��� �� �� �� �̻� �̵� �� ������ �� �����ϴ�.
			iMessage = 7169;
		break;
	case RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER:
			//7170	���� ������ �����Ǿ� �ش� ��� �۾��� �����Ͽ����ϴ�. �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
			iMessage = 7170;
		break;
	case RC_GUILD_INVALID_DUTY_IN_BASE_CORPS:
			//7171	���� ���ܿ��� �������� �ʴ� ��å�Դϴ�.	
			iMessage = 7171;
		break;
	case RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS:
			//6237	������ ������ �̹� ������ �ҼӵǾ� �ִ� �����Դϴ�.
			iMessage = 6237;
		break;
	case RC_GUILD_ALREADY_EXIST_REAR_CAMP:
			//6201	���� �� ����ķ���� �����Ͽ� �� �̻� â���� �� �����ϴ�.
			iMessage = 6201;
		break;
	case RC_GUILD_NOT_EXIST_REAR_CAMP:
			//7172	�ش� ���ܿ� ����ķ���� �������� �ʽ��ϴ�.
			iMessage = 7172;
		break;
	case RC_GUILD_CANNOT_LEAVE_MASTER:
			//7173	������� ��� Ż�� �� �� �����ϴ�.
			iMessage = 7173;
		break;
	case RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS:
			//7174	���ұ��ܿ����� ����� �� ���� ����Դϴ�.
			iMessage = 7174;
		break;
	case RC_GUILD_EXIST_MEMBER:
			//6203	���� �� ������ ������ �ػ��� �� �����ϴ�.
			iMessage = 6203;
		break;
	case RC_GUILD_ALREADY_EXIST_GUILD:
			//7214	�̹� ��尡 �����մϴ�.
			iMessage = 7214;
		break;
	case RC_GUILD_CANNOT_LEVELUP:
			//5182	��� ��ũ���� �ʿ��� ������ Ȯ���� �ֽñ� �ٶ��ϴ�.
			iMessage = 5182;
		break;
	case RC_GUILD_NOT_CORPS_MEMBER:
			//7212	���ܿ� ���� ����� �ƴմϴ�.
			iMessage = 7212;
		break;
	case RC_GUILD_NOT_BASE_CORPS:
			//7213	���ұ����� �ƴϿ��� �̿��� �� �����ϴ�.
			iMessage = 7213;
		break;
  
	case RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY:
			//7240 �ش籺���� ���� ���� �г�Ƽ�� ���� ���ܰ� �̵��� �������� �ʽ��ϴ�.
			iMessage = 7240;
		break;
  

	case RC_GUILD_NOT_GUILD_MEMBER:
			//7205	��� ����� �ƴմϴ�.
			iMessage = 7205;
		break;

	case RC_GUILD_NOT_EXIST_GUILD_SEAL:
			//6161 ��� ������ �κ��丮�� �������� �ʽ��ϴ�.
			iMessage = 6161;
		break;

	case RC_GUILD_CHANGE_MARK_NOT_RIGHT:
			//7202	��� ��ũ�� ������ �� �ִ� ������ �����ϴ�.
			iMessage = 7202;
		break;

	case RC_GUILD_CHANGE_MARK_NOT_DUTY_STATE:
			//7203	�ش� ����� �̿��� �� ���� ��å�Դϴ�.
			iMessage = 7203;
		break;
	case RC_GUILD_MARK_LEVEL_LOW:
			//7201	��� ������ ���� ��� ��ũ�� ����Ҽ� �����ϴ�.
			iMessage = 7201;
		break;
	case RC_GUILD_MARK_ALEADY_EXIST:
			//7200	�̹� ��ϵǾ� �ִ� ��� ��ũ�Դϴ�.
			iMessage = 7200;
		break;

	case RC_GUILD_MARK_DELETE_FAILED:
			//7218	��� ��ũ ������ �����߽��ϴ�.
			iMessage = 7218;
		break;

	case RC_GUILD_CANNOT_CREAT_SAMENAME_STATE:
			// 7175 ������ �̸��� ��尡 �ֽ��ϴ�. �ٽ� �Է��� �ֽʽÿ�.
			iMessage = 7175;
		break;

	case RC_GUILD_KICK_GUILD_BY_LEVELUP:
			//31������ �޼��Ͽ� ���� ķ������ ���� Ż��Ǿ����ϴ�.
			iMessage = 7197;
		break;
	case RC_GUILD_NOT_JOIN_GUILD:
			//5165	��忡 ���ԵǾ� ���� �ʽ��ϴ�.
			iMessage = 5165;
		break;
	case RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL:
			//7232	16���� �̻��� ĳ���ʹ� �Ʒú����� ������ �� �����ϴ�.
			iMessage = 7232;
		break;
	case RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL:
			//7211 15���� ���ϸ� �Ʒú����� ������ �� �ֽ��ϴ�
			iMessage = 7211;
		break;
	case RC_GUILD_CORPS_PENALTY_TIME:
			//7238 ������ �����Ͽ� �ش� ������ ���� ������ 3�ϰ� �� �� �����ϴ�.
			iMessage = 7238;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_GRADE:
			//7250 ���� ���� ������ �����ϴ�.
			iMessage = 7250;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB:
	case RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM:
			//7251 ���� ���濡 ���� �߽��ϴ�.
			iMessage = 7251;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME:
			// 7175 ������ �̸��� ��尡 �ֽ��ϴ�. �ٽ� �Է��� �ֽʽÿ�.
			iMessage = 7175;
		break;

	case RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE:
			//70022 ����� ������ �ܾ��Դϴ�.
			iMessage = eST_CANNOT_USE_WORD;
		break;

	case RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD:
		// 3343	���� ���� ���ο����� ���Ż�� �Ұ����Ͽ� ���Ǽ��� �Ա��� �̵��մϴ�. ��õ� �ٶ��ϴ�.
		iMessage = 3343;
		break;
	case RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD:
		//3378 ���� �ش� ������ ���Ǽ��� ���ο� �־� ������ ���Ͽ� ���Ǽ��� �Ա��� ���� �̵� ��ŵ�ϴ�. ��õ� �ٶ��ϴ�.
		iMessage = 3378;
		break;
	case RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY:
		// 3373	'���� ����' ������ ���� ����� �����Ͽ� �����ü�� �Ұ��� �մϴ�.
		iMessage = 3373;
		break;
	case RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY:
		// 3381	���� ����' �������� ���� ���� ���� �����ü�� �Ұ��� �մϴ�.
		iMessage = 3381;
		break;

	case RC_GUILD_WAREHOUSE_GET_CANNOT_DB:
	default:
		{
			//5164	��� ������ �� �� ���� ����� �޾ҽ��ϴ�.
			iMessage = 5164;

#ifdef _DEV_VER
			TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};
			Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T("�����ڵ� %d") , byErrorCode);
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
