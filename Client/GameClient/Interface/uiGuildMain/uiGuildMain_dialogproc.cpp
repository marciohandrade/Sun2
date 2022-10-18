//------------------------------------------------------------------------------
//  uiGuildMain_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMain.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "Hero.h"
#include "ChatDialog.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "map.h"
#include "uiGuildMan/uiGuildMan.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Mouse.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiGuildNotice/uiGuildNotice.h"
#include "uiGuildRightDelegation/uiGuildRightDelegation.h"
#include "uiGuildDutyVest/uiGuildDutyVest.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiCommunityInfo/uiCommunityInfo.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "GuildRightInfoParser.h"

//------------------------------------------------------------------------------
WzID uiGuildMain::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("L001"),    // DLGPOS_LST_MEMBERS,
		StrToWzID("ST00"),	  // DLGPOS_TXT_CURRENT_SELECT
		StrToWzID("ST01"),	  // DLGPOS_TXT_NUMBER_OF_GUILDMEMBER
		StrToWzID("LS00"),	  // DLGPOS_LST_CORP
		StrToWzID("TI00"),	//DLGPOS_BTN_SORT_NAME,
		StrToWzID("TI01"),	//DLGPOS_BTN_SORT_LEVEL,
		StrToWzID("TI02"),	//DLGPOS_BTN_SORT_CLASS,
		StrToWzID("TI11"),	//DLGPOS_BTN_SORT_DUTY,
		StrToWzID("TI03"),	//DLGPOS_BTN_SORT_CONNECT,

        StrToWzID("C001"),    // DLGPOS_CHKBTN_MEMBER_INFOS
        StrToWzID("C002"),    // DLGPOS_CHKBTN_GUILD_INFO
        StrToWzID("C003"),    // DLGPOS_CHKBTN_GUILD_STYLE
        StrToWzID("C004"),    // DLGPOS_CHKBTN_GUILD_MAN

        StrToWzID("B999"),    // DLGPOS_BTN_EXIT,
        StrToWzID("TI05"),    // DLGPOS_BTN_GUILD_INVITE,
        StrToWzID("TI06"),    // DLGPOS_BTN_GUILD_DELETE,
        StrToWzID("TI08"),    // DLGPOS_BTN_GUILD_EXIT,
        StrToWzID("TI10"),    // DLGPOS_BTN_GUILD_NOTICE,
        StrToWzID("TI14"),    // DLGPOS_BTN_DUTY_VEST,
       
		StrToWzID("TI04"),	  // DLGPOS_BTN_GUILD_RELATIONSHIP
		StrToWzID("TI07"),	  // DLGPOS_BTN_GUILD_MASTER_DELEGATION
		StrToWzID("TI12"),	  // DLGPOS_BTN_GUILD_MEMBER_RIGHT_INFO
		StrToWzID("TI13"),	  // DLGPOS_BTN_GUILD_CORP_CHANGE
		StrToWzID("BT00"),	  // DLGPOS_BTN_CORP_LIST

		StrToWzID("TI09"),    // DLGPOS_BTN_RIGHT_DELEGATION,

        StrToWzID("VS00"),    // DLGPOS_SCR_VERTICAL,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiGuildMain::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		//m_bShow = true;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_guild_main;
        msg.wParam = InterfaceManager::DIALOG_GUILD_MAIN;
        msg.DoSomething = GlobalFunc::DoGuildMain;
        g_KeyQueueManager.PushBack(msg);

		if (m_pScrollControl)
        {
			m_pScrollControl->SetScrollPosWZ(last_scrollbar_position());
        }

		SetListCaption();
		m_pListCorps->ShowWindowWZ(WZ_HIDE);

    }
    else
    {
        //m_bShow = false;

        if (m_pScrollControl)
        {
            set_last_scrollbar_position(m_pScrollControl->GetScrollPosWZ());
        }

        uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
        if ((community_man != NULL) &&
			( community_man->GetCommunityinfo_Dialog() != NULL))
        {
            if (community_man->GetCommunityinfo_Dialog()->request_uimanager_id() == UIMAN_GUILD)
            {
                community_man->GetCommunityinfo_Dialog()->ShowInterface(val);
            }
        }

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_guild_main);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {   
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_CHKBTN_GUILD_INFO:
                {
                    //this->ShowInterface(FALSE);
                    //GET_DIALOG(IM_GUILD_MANAGER::GUILD_WINDOW)->ShowInterface(TRUE);
                    GetManager()->ChangeTab(GET_DIALOG(IM_GUILD_MANAGER::GUILD_WINDOW));
                }
                break;

            case DLGPOS_BTN_EXIT:
                {
                    if (GetManager())
                    {
                        GetManager()->set_active_tab(this);
                    }
                    ShowInterfaceWithSound(FALSE);
					//m_bShow = FALSE;
                }
                break;

                // 길드 초대
            case DLGPOS_BTN_GUILD_INVITE:
                {
                    if(GetManager())
					{
						GetManager()->Invite(NULL);
					}
                }
                break;

                // 길드 제명
            case DLGPOS_BTN_GUILD_DELETE:
                {
					int start = this->m_pScrollControl->GetScrollPosWZ();

                    int cursel = this->m_pLstControl->GetCurSel();

                    if (cursel >= 0)
                    {
						int iIndex = start + cursel;
					
						VEC_GUILD_MEMBER & rGuildMembers = const_cast<VEC_GUILD_MEMBER &> (GetManager()->GetGuildMembers());
						if(rGuildMembers.size() < static_cast<size_t>(iIndex + 1))
							return;
						stGUILD_MEMBER_INFO * member = GetSelectedPacketForGuildMember();
                        if (member)
                        {
							if(GetManager())
							{
								TCHAR tszKickCharName[MAX_CHARNAME_LENGTH+1] = {0,};
								StrnCopy(tszKickCharName, member->m_tszCharName, MAX_CHARNAME_LENGTH);

								GetManager()->Kick(tszKickCharName);

							}
							

                        }
                    }

                }
                break;

                // 길드 탈퇴
            case DLGPOS_BTN_GUILD_EXIT:
                {
                    if(GetManager())
					{
						GetManager()->Withdraw();
					}
                }
                break;

			case DLGPOS_BTN_GUILD_NOTICE:
				{
					// 길드 공지 창 띄우기
					OpenGuildNotice();	
				}
				break;


			case DLGPOS_BTN_DUTY_VEST:	//직책 부여
				{
					OnClickDutyVest();
				}
				break;

			case DLGPOS_BTN_RIGHT_DELEGATION:	//권한 위임
				{
					OnClickRightDelegation();
				}
				break;

			case DLGPOS_BTN_GUILD_MASTER_DELEGATION: // 길드 마스터 위임
				{
					OnClickGuildMasterDelegation();
				}
				break;

			case DLGPOS_BTN_GUILD_MEMBER_RIGHT_INFO: // 길드 멤버 권한 정보
				{
					OnClickGuildMemberRightInfo();
				}
				break;

			case DLGPOS_BTN_GUILD_CORP_CHANGE:
				{
					OnClickGuildCropChange();
				}
				break;
			case DLGPOS_BTN_CORP_LIST:
				{
					if( m_pListCorps )
					{
						
						ENUM_STATEWZ eState = m_pListCorps->GetState() == WZ_SHOW ?WZ_HIDE : WZ_SHOW;
						
						if( eState == WZ_SHOW )
							InsertAllCorps();

						m_pListCorps->ShowWindowWZ(eState);
					}
				}
				break;
			case DLGPOS_LST_CORP:
				{
					if( m_pListCorps->GetCurSel() != -1)
					{
						int iCurSel =  m_pListCorps->GetCurSel();
						byte * pData =	(byte *)m_pListCorps->GetItemData(iCurSel);
						m_iCurSelCorps = *pData;
						m_pScrollControl->SetScrollPosWZ(0);
						SetListCaption();
					}
					m_pListCorps->ShowWindowWZ(WZ_HIDE);

				}
				break;

			case DLGPOS_BTN_SORT_NAME:
				{
					GetManager()->SortMember(eSORT_NAME);
				}
				break;

			case DLGPOS_BTN_SORT_LEVEL:
				{
					GetManager()->SortMember(eSORT_LV);
				}
				break;

			case DLGPOS_BTN_SORT_CLASS:
				{
					GetManager()->SortMember(eSORT_CLASS);
				}
				break;

			case DLGPOS_BTN_SORT_DUTY:
				{
					GetManager()->SortMember(eSORT_DUTY);
				}
				break;

			case DLGPOS_BTN_SORT_CONNECT:
				{
					GetManager()->SortMember(eSORT_CONNETION);
				}
				break;
			
			case DLGPOS_BTN_GUILD_RELATIONSHIP:
				{
                    BOOL right_by_grade = GuildRightInfoParser::Instance()->CheckGradeForRight(
                        eGUILD_RIGHTS_RELATION, GetManager()->GetGuildInfo().m_GuildGrade);
                    if (right_by_grade == FALSE)
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						//7207 레벨이 낮아 해당 기능 사용 못한다.
						g_InterfaceManager.GetInterfaceString(7207, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						MSG_CG_GUILD_ADD_INFO_SYN SendMsg;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

						uiBase * pGuildAlliance = GET_DIALOG( IM_GUILD_MANAGER::GUILD_ALLIANCE );
						if(pGuildAlliance)
						{
							pGuildAlliance->ShowInterface(TRUE);
						}
					}
				}
				break;


            }
        }
        break;

    case RT_MSG_LBUTTONDCLICK:
        {
            OnLDoubleButtonClick(pMessage);
        }
        break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;

	case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            int pos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
            {
				int nSkipLevel=m_MaxPage/2;//반페이지이동
				( (pos-nSkipLevel)<mmin ? (pos=mmin):(pos-=nSkipLevel));

                this->m_pScrollControl->SetScrollPosWZ(pos);
            }
        }
        break;

	case RT_MSG_SB_PAGEBOTTOM:		//아랫쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM:
        {
            //.아랫쪽 화살표를 눌렀다
            int pos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
            {
				int nSkipLevel=m_MaxPage/2;//반페이지이동

				( (pos+nSkipLevel)>mmax ? (pos=mmax):(pos+=nSkipLevel));

                this->m_pScrollControl->SetScrollPosWZ(pos);
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::OnUpdateSolarDialog()
{
	if(!IsVisible())
	{
		this->ShowInterface(FALSE);
		return;
	}

    if (this->IsLock())
        return;

    for (int i=0; i<POS_CHKBTN_SIZE; ++i)
    {
        this->m_pChkBtnControls[i]->SetCheckState(false);
    }

    this->m_pChkBtnControls[0]->SetCheckState(true);

    this->updateMemberInfos();

	updateMouseWheel();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::OnRenderSolarDialog()
{
    this->renderMemberInfos();
}

//------------------------------------------------------------------------------
bool uiGuildMain::OnLDoubleButtonClick(SI_MESSAGE* message)
{
    switch (getCtrlIDToPos(message->dwCtrlID))
    {
    case DLGPOS_LST_MEMBERS:
        {
            stGUILD_MEMBER_INFO* member_info = GetMemberInfo_byListboxCurSel();
            if ((member_info != NULL) && 
#ifndef _NA_0_20100527_ADD_CHAR_RELATION_MEMO   //#if'N'def
                (member_info->m_bLogin == TRUE) && 
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
                (g_pHero != NULL) &&
                (_tcscmp(g_pHero->GetName(), member_info->m_tszCharName) != 0))
            {
                GetManager()->Send_CW_FRIEND_STATUS_REQ(member_info->m_tszCharName, 
                                                        member_info->m_CharGuid);

                uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
                if ((community_man != NULL) && 
				   (community_man->GetCommunityinfo_Dialog() != NULL))
                {
                    community_man->GetCommunityinfo_Dialog()->set_request_uimanager_id(UIMAN_GUILD);
                }
            }
        }
        break;
    }

    return true;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
