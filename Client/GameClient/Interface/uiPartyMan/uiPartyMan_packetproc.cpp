//------------------------------------------------------------------------------
//  uiPartyMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyMan.h"
#include "globalfunc.h"
#include "uitargetman/uitargetman.h"
#include "InterfaceManager.h"
#include "uiMissionRankMan/uiMissionRankMan.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "MissionScene.h"
#include "uiAssistMan/uiAssistMan.h"

#include "ResultCode.h"
using namespace RC;

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
  #include "ItemShareSystem.h"
  #include "GameConst_Color.h"
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

#include "uisystemman/uiSystemMan.h"

#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "Hero.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"

void _callBack_ReallyCancleReturnPartyMember(bool val) 
{
	uiPartyMan * pPartyMan = ( uiPartyMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY);
	if(!pPartyMan)
		return;
	if( val )
	{
		// ������ ��Ƽ���� ��ȯ���� �ʰڴ�. ������ ����� ��Ƽ�� DB���� ������.
		pPartyMan->NET_SEND_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK(MSG_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK::REJECT_JOIN);
	}
	else
	{
		// ����� �ٽ� �����ϰڴ�.
		//Mod 08.01.xx
		//MSG_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK::THINKING_HUMMMM ���� ����� ������ ������.
		pPartyMan->NET_SEND_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK(MSG_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK::REJECT_JOIN);
	}
}

void _callBack_Recall_PartyMember(bool val) // ��Ƽ���� ��Ƽ�� ��ȯ
{
	
	if( val )
	{
		uiPartyMan * pPartyMan = ( uiPartyMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY);
		if( pPartyMan )
		{
			pPartyMan->NET_SEND_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK(MSG_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK::ACCEPT_JOIN);
		}
	}
	else
	{
		uiSystemMan * pSystemMan = (uiSystemMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if( pSystemMan )
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
			// ������ ��Ƽ���� ���� ��Ű�� �����ðڽ��ϱ�? 5234
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString( 5234 ) );
			pSystemMan->Sys_Confirm(szMessage, _callBack_ReallyCancleReturnPartyMember);
		}
	}
}
void _callBack_Return_Party(bool val) // ��Ƽ���� ��Ƽ�忡�� ����
{	
	uiPartyMan * pPartyMan = ( uiPartyMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY);
	if( pPartyMan )
	{
		pPartyMan->NET_SEND_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK(val);
	} 
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::NetworkProc(MSG_BASE* pMsg)
{
    switch (pMsg->m_byCategory)
    {
    case CG_PARTY:
        {
            switch (pMsg->m_byProtocol)
            {
                // �� ���� ��û �� ��Ƽ�� ������ ���ʷ� �޴´�.
			case CG_PARTY_CREATE_CMD:
				{
					this->InitInfo((MSG_CG_PARTY_CREATE_CMD*)pMsg);
                    RefreshPartyHideInfo();
                    SetShowFace(GetShowFace());
				}
				break;

                // ��Ƽ�� ���� ��ε�
            case CG_PARTY_JOIN_BRD:
                {
                    this->AddInfo((MSG_CG_PARTY_JOIN_BRD*)pMsg);
                    /////////////////////elfleeTest
                    uiBattleScoreMan* battle_score_manager = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			        if (battle_score_manager)
			        {
                        if (battle_score_manager->GetMode() == uiBattleScoreMan::eBTS_MISSION)
                        {   // �̼��� ��Ƽ�� �߰��� ��Ʋ ���ھ� ����
                            battle_score_manager->PartyRefresh();
                        }
                    }  
                    SetShowFace(GetShowFace());
                    SetShowGague(GetShowGague());
                    SetShowBuf(GetShowBuf());
                }
                break;

            case CG_PARTY_JOIN_NAK:
                {
                    MSG_CG_PARTY_JOIN_NAK *msg =
                        (MSG_CG_PARTY_JOIN_NAK *)pMsg;

						if( msg->m_byErrorCode == RC_PARTY_INVALID_PLAYER_STATE)
						{
							GlobalFunc::ErrorCode_Player_State(msg->m_ePlayerState );
						}
						else
						{
							this->Reason(msg->m_byErrorCode);
						}
                }
                break;

            case CG_PARTY_JOIN_RESPONSE_SYN:
                {
                    MSG_CG_PARTY_JOIN_RESPONSE_SYN *msg =
                        (MSG_CG_PARTY_JOIN_RESPONSE_SYN *)pMsg;
                    this->ResponseJoinParty(msg->m_szMasterName);
                }
                break;

                // ��Ƽ�� ���� ��ε�
            case CG_PARTY_CHANGE_MASTER_BRD:
                {
                    MSG_CG_PARTY_CHANGE_MASTER_BRD *msg =
                        (MSG_CG_PARTY_CHANGE_MASTER_BRD *)pMsg;
                    this->Migration(msg->m_dwObjKey);
                }
                break;

                // ��Ƽ�� ���� ����
            case CG_PARTY_CHANGE_MASTER_NAK:
                {
                    MSG_CG_PARTY_CHANGE_MASTER_NAK *msg =
                        (MSG_CG_PARTY_CHANGE_MASTER_NAK *)pMsg;

                    this->Reason(msg->m_byErrorCode);
                }
                break;

                // ��Ƽ ���� ����
            case CG_PARTY_DESTROY_NAK:
                {
                    MSG_CG_PARTY_DESTROY_NAK *msg =
                        (MSG_CG_PARTY_DESTROY_NAK *)pMsg;

                    this->Reason(msg->m_byErrorCode);
                }
                break;

            case CG_PARTY_LEAVE_BRD:
                {
                    MSG_CG_PARTY_LEAVE_BRD *msg =
                        (MSG_CG_PARTY_LEAVE_BRD *)pMsg;

					if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION) && Comp_HeroKey(msg->m_dwObjKey))
					{
						if(msg->m_byReason == eREASON_LEAVE_PARTY_BAN)	// ���� �����̸� ����
							g_MissionScene.SetLeavePartyBan();
					}
                    this->Remove_MemberInfo(msg->m_dwObjKey, msg->m_byReason);
                    ChangePartyHideInfo(msg->m_dwObjKey,TRUE);
                    uiBattleScoreMan* battle_score_manager = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			        if (battle_score_manager)
			        {
                        if (battle_score_manager->GetMode() == uiBattleScoreMan::eBTS_MISSION)
                        {   // �̼��� ��Ƽ�� Ż��� ��Ʋ ���ھ� ����
                            battle_score_manager->PartyRefresh();
                        }
                    }
                    SetShowFace(GetShowFace());
#ifdef _DH_BATTLEZONE2_
                    if (msg->m_byReason == eREASON_LEAVE_PARTY_NOT_MATCH_LEVEL && Comp_HeroKey(msg->m_dwObjKey))
                    {
                        uiPartyMan* party_manager =
                            static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                        if (party_manager)
                        {
                            party_manager->Reason(RC::RC_PARTY_KICKOUT_PARTY_ROOM);
                        }
                    }
#endif//_DH_BATTLEZONE2_
				}
                break;

            case CG_PARTY_LEAVE_NAK:
                {
                    MSG_CG_PARTY_LEAVE_NAK *msg =
                        (MSG_CG_PARTY_LEAVE_NAK *)pMsg;

                    this->Reason(msg->m_byErrorCode);
                }
                break;

                // ��Ƽ ����
            case CG_PARTY_DESTROY_BRD:
                {
                    if (g_pHero != NULL && g_pHero->FixedPortalState.has_portal_info() == true)
                    {
                        //���� ��Ƽ�� ���� �ֱ� ������ �޼����� ������� �ʴ´�.
                    }
                    else
                    {
                        TCHAR	szMessage[INTERFACE_STRING_LENGTH];
                        g_InterfaceManager.GetInterfaceString( eST_PARTY_DESTROYED, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    }
                    this->DestroyParty();
                }
                break;

			case CG_PARTY_MEMBER_LEVEL_UP_BRD:	// ��Ƽ�� ���� ��ȭ
				{
					MSG_CG_PARTY_MEMBER_LEVEL_UP_BRD *msg = (MSG_CG_PARTY_MEMBER_LEVEL_UP_BRD*)pMsg;
					this->ChangeLevel(msg->m_dwObjKey, msg->m_wLevel);
					this->ChangeHP(msg->m_dwObjKey, msg->m_byHPRatio);
					this->ChangeMP(msg->m_dwObjKey, msg->m_byMPRatio);
				}
				break;

			case CG_PARTY_MEMBER_HP_CHANGE_BRD:	// ��Ƽ�� ü�� ��ȭ
				{
					MSG_CG_PARTY_MEMBER_HP_CHANGE_BRD* msg = (MSG_CG_PARTY_MEMBER_HP_CHANGE_BRD*)pMsg;

					for (int Index = 0; Index < msg->m_NumberOfNodes; ++Index)
					{
						if (Comp_HeroKey(msg->m_Nodes[Index].objectKey))
                        {
							continue;
                        }

						this->ChangeHP(msg->m_Nodes[Index].objectKey, msg->m_Nodes[Index].ratio);
					}					
				}
				break;

			case CG_PARTY_MEMBER_MP_CHANGE_BRD:	// ��Ƽ�� MP ��ȭ
				{
					MSG_CG_PARTY_MEMBER_MP_CHANGE_BRD* msg = (MSG_CG_PARTY_MEMBER_MP_CHANGE_BRD*)pMsg;

					for (int Index = 0; Index < msg->m_NumberOfNodes; ++Index)
					{
						if (Comp_HeroKey(msg->m_Nodes[Index].objectKey))
                        {
							continue;
                        }

						this->ChangeMP(msg->m_Nodes[Index].objectKey, msg->m_Nodes[Index].ratio);
					}					
					
				}
				break;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            case CG_PARTY_MEMBER_SD_CHANGE_BRD:	// ��Ƽ�� ���� ��ȭ
                {
                    MSG_CG_PARTY_MEMBER_SD_CHANGE_BRD* msg = (MSG_CG_PARTY_MEMBER_SD_CHANGE_BRD*)pMsg;

                    for (int Index = 0; Index < msg->number_of_nodes; ++Index)
                    {
                        if (Comp_HeroKey(msg->nodes[Index].object_key))
                        {
                            continue;
                        }

                        this->ChangeSD(msg->nodes[Index].object_key, msg->nodes[Index].ratio);
                    }					
                }
                break;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
			case CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD:
				{
					MSG_CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD *msg = (MSG_CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD *)pMsg;
					
					this->ChangeChaoState(msg->m_dwObjKey,msg->m_byChaoState);
				}
				break;

			case CG_PARTY_MEMBER_DEAD_BRD:		// ��Ƽ�� ����
				{
					MSG_CG_PARTY_MEMBER_DEAD_BRD *msg = (MSG_CG_PARTY_MEMBER_DEAD_BRD*)pMsg;
					this->DeadPartyMembaer(msg->m_dwObjKey);
				}
				break;

			case CG_PARTY_MEMBER_RESURRECTION_BRD:	// ��Ƽ�� ��Ȱ
				{
					MSG_CG_PARTY_MEMBER_RESURRECTION_BRD	*msg = (MSG_CG_PARTY_MEMBER_RESURRECTION_BRD*)pMsg;

					this->ResurrectionPartyMember(msg->m_dwObjKey);		
					this->ChangeHP(msg->m_dwObjKey, msg->m_byHPRatio);
					this->ChangeMP(msg->m_dwObjKey, msg->m_byMPRatio);
				}
				break;



			case CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD:
				{
					MSG_CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD* pRecvMsg = (MSG_CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD *) pMsg;
					
					TCHAR szNotice[INTERFACE_STRING_LENGTH] ={0,};
					//5235: <%s>���� ��Ƽ ���͸� �����Ͽ����ϴ�.
					Snprintf(szNotice , INTERFACE_STRING_LENGTH -1 , g_InterfaceManager.GetInterfaceString( 5235 ), pRecvMsg->CHAR_NAME());
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szNotice);
					DeleteUnExpectedPartyMember(pRecvMsg->CHAR_NAME());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                    uiBattlezone2Man* battlezone2_manager =
                        GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (battlezone2_manager)
                    {
                        battlezone2_manager->DeleteDisConnectedUser(pRecvMsg->CHAR_NAME());
                    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
				}
				break;


		
			case CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD:
				{
					MSG_CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD * pRecvMsg = (MSG_CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD *) pMsg;
					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
					{	
					 	m_dwPendingPartyKey  = pRecvMsg->m_wPartyKey;// ������ �ٽ� ������� �Ѵ�. -_-;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        if (is_opened_return_party_window_ == false)
                        {
                            if ((g_InterfaceManager.IsLockDlg() == false) &&
                                ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE) ||
                                (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD)))
                            {
                                uiAssistMan* assist_man = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
                                if (assist_man)
                                {
                                    is_opened_return_party_window_ = true;
                                    assist_man->ShowAskPartyReturnDialog();
                                }
                            }
                        }
#else
                        TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
                        Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString( 5238 ) ); //������ ���ᰡ �Ǳ� ������ ��Ƽ�� ���� �Ͻðڽ��ϱ�?

                        if(TRUE == g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_REWARD))
                        {	
                            //����â�� �����ִٸ� Ȯ��â���� ó��
                            pSystemMan->Sys_VerifyLock( g_InterfaceManager.GetInterfaceString( 5085 ) );
                            _callBack_Return_Party(false);															//���;���
                        }
                        else
                        {
                            pSystemMan->Sys_ConfirmLock(szMessage, _callBack_Return_Party);
                        }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
					}
				}
				break;

			case CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD:
				{
					MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD * pRecvMsg = ( MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD *) pMsg;
					DeleteUnExpectedPartyMember(pRecvMsg->CHAR_NAME());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                    uiBattlezone2Man* battlezone2_manager =
                        GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (battlezone2_manager)
                    {
                        battlezone2_manager->DeleteDisConnectedUser(pRecvMsg->CHAR_NAME());
                        battlezone2_manager->UnsetPKBool(
                            uiBattlezone2Man::PACKET_STATUS_SEND_DISCONNECTED_USER_BAN);
                    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                    SetShowFace(GetShowFace());
				}
				break;

			case CG_PARTY_NAVI_INFO_BRD:
				{
					MSG_CG_PARTY_NAVI_INFO_BRD* pRecvMsg = (MSG_CG_PARTY_NAVI_INFO_BRD*)pMsg;
					if (IsParty())
					{
						NET_RECV_MSG_CG_PARTY_NAVI_INFO_BRD(pRecvMsg);
					}					
				}
				break;

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            case CG_PARTY_DICECAST_RESULT_CMD:
                {
                    MSG_CG_PARTY_DICECAST_RESULT_CMD* recv_messege =
                        static_cast<MSG_CG_PARTY_DICECAST_RESULT_CMD*>(pMsg);
                    this->NET_RECV_MSG_CG_PARTY_DICECAST_RESULT_CMD(recv_messege);
                }
                break;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            case CG_PARTY_ON_ENTER_GAME_ROOM_BRD:
                {
                    MSG_CG_PARTY_ON_ENTER_GAME_ROOM_BRD* recv_message = 
                        static_cast<MSG_CG_PARTY_ON_ENTER_GAME_ROOM_BRD*>(pMsg);

                    if (recv_message->zone_policy_item_obtain_all_party_member_ == true)
                    {
                        m_pUIItemShareSystem->SetPartyObtainRoom(true);

                        // 5250 ��� ��Ƽ���� �������� �����ϰ� ȹ���� �� �ִ� ��Ʋ�� �Դϴ�.
                        WzColor message_color = c_Chat_NoticeColor;
                        TCHAR message[INTERFACE_STRING_LENGTH + 1] = {'\0',};
                        g_InterfaceManager.GetInterfaceString(5250, message);
                        g_InterfaceManager.ShowCenterMessageBox(message, 10000, &message_color, FALSE );
                    }
                }
                break;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
