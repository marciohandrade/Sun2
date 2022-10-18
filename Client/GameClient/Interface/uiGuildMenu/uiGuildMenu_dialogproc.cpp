//------------------------------------------------------------------------------
//  uiGuildMenu_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMenu.h"
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
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiGuildRankup.h"
#include "uiGuildMarkCombination/uiGuildMarkCombination.h"
#include "uiGuildWareHouse/uiGuildWareHouse.h"
#include "GuildRightInfoParser.h"

void
guilddestoryConfirm( bool bYes )
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    if (bYes)
    {
        uiGuildMan* guildMan =
            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        assert (guildMan);

        guildMan->SEND_CG_GUILD_DESTROY_SYN();
    }
    else
    {
		//	��� �ػ��� ��� �ϼ̽��ϴ�.
		g_InterfaceManager.GetInterfaceString( eST_CANCEL_DESTROY_GUILD, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_Verify(szMessage, FALSE);

    }
}

void GuildDestoryCancelConfirm(bool bYes)
{
	if(bYes)
	{
		MSG_CG_GUILD_RECOVERY_GUILD_SYN SendMsg;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
}


void _CallBack_GuildMarkDestory(bool bYes)
{
	if( bYes)
	{
		uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
		if( pGuildMan )
		{
			pGuildMan->SEND_CG_GUILD_GUILDMARK_DELETE_SYN();
		}
	}
}

void _CallBack_GuildMarkRegiste(int confirm_value)
{
    switch (confirm_value)
    {
    case kConfirmSelect1:
        {
            uiBase* pUIGuildMark = GET_DIALOG(IM_GUILD_MANAGER::GUILD_MARK_COMBINATION);
            if (pUIGuildMark)
            {
                pUIGuildMark->ShowInterface(TRUE);
            }
        }
        break;
    case kConfirmSelect2:
        {
            uiSystemMan* pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (pSystemMan)
            {
                //6168 ��帶ũ�� ������ �Ŀ��� ������ ���� ������, �� ��� �� ��� ������ �ʿ��մϴ�. ���� �����Ͻðڽ��ϱ�?
                pSystemMan->Sys_Confirm(g_InterfaceManager.GetInterfaceString(6168), 
                                        _CallBack_GuildMarkDestory, NULL,NULL,1197);
            }
        }
        break;
    case kConfirmClose:
        {
            // no operation
        }
        break;
    }
}


//------------------------------------------------------------------------------
WzID uiGuildMenu::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("B999"),    // DLGPOS_BTN_EXIT,
	StrToWzID("B001"),	  //DLGPOS_BTN_GUILD_CREATE_AND_DESTORY
	StrToWzID("B003"),	  //DLGPOS_BTN_GUILD_RANK_UP,
	StrToWzID("B005"),	  //DLGPOS_BTN_CORP_CREATE_DESTORY,
	StrToWzID("B002"),	  //DLGPOS_BTN_CAMP_CREATE_DESTORY,
	StrToWzID("B004"),	  //DLGPOS_BTN_GUILD_MARK,
	StrToWzID("B006"),	  //DLGPOS_BTN_GUILD_WAREHOUSE,
	StrToWzID("L001"),    // DLGPOS_LST_GUILD_NOTICE,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiGuildMenu::OnShowWindow( BOOL val )
{
    if (val)
    {
        //SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		m_bShow = true;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_guild_menu;
        msg.wParam = InterfaceManager::DIALOG_GUILD_MENU;
        msg.DoSomething = GlobalFunc::DoGuildMenu;
        g_KeyQueueManager.PushBack(msg);

		UpdateButtonShowNHide();

        TCHAR   szMessage[INTERFACE_STRING_LENGTH];
	    m_pListControl->DeleteItemAll();
        m_pListControl->SetBeginDrawIndex(0);
		int idx = this->m_pListControl->InsertItemBack();
		g_InterfaceManager.GetInterfaceString(1036,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::SetItemText( this->m_pListControl, idx, szMessage );
		this->m_pListControl->SetItemTextColor(idx, 0, RGBA(235,192,38, 255));
#ifdef _INTERNATIONAL_UI
		m_pListControl->SetSellHeight(16);
		this->m_pListControl->SetItemFontID(idx,  0, StrToWzID("st10"));
#else
		this->m_pListControl->SetItemFontID(idx,  0, StrToWzID("mn12"));	
#endif//_INTERNATIONAL_UI
		this->m_pListControl->SetItemBKColor(idx, 0, RGBA(0,0,0,0));
		this->m_pListControl->SetColorSelBar(RGBA(0,0,150,0));
		this->m_pListControl->SetItemSelTextColor(idx, 0, RGBA(235,192,38, 255));

		//��������������(��Ʈ��:1036) �� �����Ѵ�.
#ifdef _INTERNATIONAL_UI 
		RECT rc = m_pListControl->GetSizeRect();
		std::vector<util::_tstring> vstrMsg;
		int iListWidth = rc.right - rc.left - 20;
		vstrMsg.clear();
		g_InterfaceManager.GetInterfaceString(1037,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg, szMessage, StrToWzID("st10"), _tcslen(szMessage), iListWidth);
		for (size_t i=0; i<vstrMsg.size(); ++i)
		{
			int idx = this->m_pListControl->InsertItemBack();
			TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());
			GlobalFunc::SetItemText( this->m_pListControl, idx, pStr );
			this->m_pListControl->SetItemTextColor(idx, 0, RGBA(235,192,38, 255));
			this->m_pListControl->SetItemFontID(idx,  0, StrToWzID("st10"));	
			this->m_pListControl->SetItemBKColor(idx, 0, RGBA(0,0,0,0));
			this->m_pListControl->SetColorSelBar(RGBA(0,0,150,0));
			this->m_pListControl->SetItemSelTextColor(idx, 0, RGBA(235,192,38, 255));
		} 
#else
		idx = this->m_pListControl->InsertItemBack();
		g_InterfaceManager.GetInterfaceString(1037,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::SetItemText( this->m_pListControl, idx, szMessage );
		this->m_pListControl->SetItemTextColor(idx, 0, RGBA(235,192,38, 255));
		this->m_pListControl->SetItemFontID(idx,  0, StrToWzID("mn12"));
		this->m_pListControl->SetItemBKColor(idx, 0, RGBA(0,0,0,0));
		this->m_pListControl->SetColorSelBar(RGBA(0,0,150,0));
		this->m_pListControl->SetItemSelTextColor(idx, 0, RGBA(235,192,38, 255));
#endif//_INTERNATIONAL_UI


       
    }
    else
    {
		m_bShow = false;

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_guild_menu);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMenu::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH + 1];

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_EXIT:
                {
					m_bShow = false;
                    
                }
                break;

			case DLGPOS_BTN_GUILD_CREATE_AND_DESTORY:
				{
					OnClickGuildCreateNDestory();
				}
				break;

			case DLGPOS_BTN_CORP_AND_CAMP_CREATE:
				{
                    BOOL right_by_grade_corps = GuildRightInfoParser::Instance()->CheckGradeForRight(
                        eGUILD_RIGHTS_CREATE_CORPS, GetManager()->GetGuildInfo().m_GuildGrade);
                    BOOL right_by_grade_camp = GuildRightInfoParser::Instance()->CheckGradeForRight(
                        eGUILD_RIGHTS_CREATE_CAMP, GetManager()->GetGuildInfo().m_GuildGrade);
					if( right_by_grade_corps == TRUE || right_by_grade_camp == TRUE )
					{
						ShowInterface(FALSE);
						OnClickCorpNCampCreate();
					}
					else
					{
						//7207	��� ������ ���� �ش� ����� �̿��� �� �����ϴ�.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7207));
					}
				}
				break;

			case DLGPOS_BTN_CORP_AND_CAMP_DESTORY:
				{
                    BOOL right_by_grade_corps = GuildRightInfoParser::Instance()->CheckGradeForRight(
                        eGUILD_RIGHTS_CREATE_CORPS, GetManager()->GetGuildInfo().m_GuildGrade);
                    BOOL right_by_grade_camp = GuildRightInfoParser::Instance()->CheckGradeForRight(
                        eGUILD_RIGHTS_CREATE_CAMP, GetManager()->GetGuildInfo().m_GuildGrade);
                    if( right_by_grade_corps == TRUE || right_by_grade_camp == TRUE)
					{
						ShowInterface(FALSE);
						OnClickCorpNCampDestory();
					}
					else
					{
						//7207	��� ������ ���� �ش� ����� �̿��� �� �����ϴ�.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7207));
					}
				}
				break;


            case DLGPOS_BTN_GUILD_RANK_UP:
                {					
					// ���� ��帶���� �̸�,
					this->ShowInterface(FALSE);
					uiGuildRankup * pRankup = GET_CAST_DIALOG( uiGuildRankup, IM_GUILD_MANAGER::GUILD_RANKUP );

					if( pRankup)
					{
						pRankup->SetDlgState(uiGuildRankup::eRANKUP_DLG_STATE_RANKUP);
						pRankup->ShowInterface(TRUE);
					}					
                }
                break;

			case DLGPOS_BTN_GUILD_MARK:
				{
					//��� ��޸� üũ �Ѵ�.(���� ������ üũ ����)
					if( GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_GUILD_MARK, GetManager()->GetGuildInfo().m_GuildGrade) ) 
					{
						uiSystemMan * pSystemMan = ( uiSystemMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if( pSystemMan )
						{
							this->ShowInterface(FALSE);
							//6266	��帶ũ
							TCHAR szGuildMark[INTERFACE_STRING_LENGTH] ={0,};
							g_InterfaceManager.GetInterfaceString(6266,szGuildMark);
							//6163 ��� ��ũ�� ��� �� ����, ������ �� �� �ֽ��ϴ�.
							pSystemMan->Sys_ConfirmCloseButton(g_InterfaceManager.GetInterfaceString(6163), 
                                                               _CallBack_GuildMarkRegiste, 
                                                               szGuildMark, 
                                                               NULL, 
                                                               1194, 
                                                               1197);
						}
					}
					else
					{
						//7207	��� ������ ���� �ش� ����� �̿��� �� �����ϴ�.
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7207));
					}
				}
				break;

			case DLGPOS_BTN_GUILD_WAREHOUSE:
                {
				    if(GRADE_AT_GUILD_CREATE >= this->GetManager()->GetGuildInfo().m_GuildGrade) //��� ������ �ɷ��̸�
				    {
						//7186	��� ����� ���� ��� â�� �̿��� �� �����ϴ�.
						g_InterfaceManager.GetInterfaceString(7186, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				    }
					else
					{
						this->ShowInterface(FALSE);
						
						uiGuildRankup* pDlg = GET_CAST_DIALOG( uiGuildRankup, IM_GUILD_MANAGER::GUILD_RANKUP );
						if(pDlg)
						{
							pDlg->SetDlgState(uiGuildRankup::eRANKUP_DLG_STATE_GUILD_WAREHOUSE_CONFIRM);
							pDlg->ShowInterface(TRUE);
						}
					}
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

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMenu::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		this->ShowInterface(FALSE);
		return;
	}

    if (this->IsLock())
        return;

	UpdateButtonShowNHide();
	UpdateButtonCaption();
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMenu::OnClickGuildCreateNDestory()
{
	if( GetManager()->IsGuildInfo() )
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		
		if(eGUILD_DELCHK_DESTROY_WAIT == GetManager()->GetGuildDelChk())
		{
			//6231	��� �ػ� ��û�� �����ϸ� �����踦 ������ ��� ��� ������ ������ �����մϴ�.\n��� �ػ� ��û�� �����Ͻðڽ��ϱ�?
			g_InterfaceManager.GetInterfaceString(6231, szMessage);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_ConfirmLock(szMessage, GuildDestoryCancelConfirm);
		}
		else
		{
			//6212	��� �ػ� ��û�� �ϸ� 7�� �Ŀ� ��尡 �����Ǹ� ��� â��, ��� UP, GP �� ������� ��� �ʱ�ȭ�˴ϴ�. ���� �ػ��Ͻðڽ��ϱ�?
			g_InterfaceManager.GetInterfaceString(6212, szMessage);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_ConfirmLock(szMessage, guilddestoryConfirm);
		}

	}
	else
	{
		if (this->GetManager()->CheckCreatCondition())
		{
			this->ShowInterface(FALSE);
			GET_DIALOG( IM_GUILD_MANAGER::GUILD_CREATE )->ShowInterface( TRUE );
			
		}
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
