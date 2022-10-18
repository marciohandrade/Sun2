//------------------------------------------------------------------------------
//  uiGuildWindow_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildWindow.h"
#include "uiGuildWindow_def.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "Hero.h"
#include "ChatDialog.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "map.h"
#include "uiGuildMan/uiGuildMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiGuildDonateUP.h"
#include "uiGuildConvertGP.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"

//------------------------------------------------------------------------------
WzID uiGuildWindow::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("C001"),    // DLGPOS_CHKBTN_MEMBER_INFOS
        StrToWzID("C002"),    // DLGPOS_CHKBTN_GUILD_INFO
        StrToWzID("C003"),    // DLGPOS_CHKBTN_GUILD_STYLE
        StrToWzID("C004"),    // DLGPOS_CHKBTN_GUILD_MAN

        StrToWzID("B999"),    // DLGPOS_BTN_EXIT,

        StrToWzID("T000"),    // DLGPOS_TXT_GUILD_NAME,
        StrToWzID("T003"),    // DLGPOS_TXT_MASTER_NAME,
        StrToWzID("T004"),    // DLGPOS_TXT_GUILD_LEVEL,
        StrToWzID("T006"),    // DLGPOS_TXT_NUM_MEMBERS,
        StrToWzID("T010"),    // DLGPOS_TXT_GUILD_UP,
        StrToWzID("T011"),    // DLGPOS_TXT_PERSON_UP,
        StrToWzID("T012"),    // DLGPOS_TXT_GUILD_OWN_GP,
        StrToWzID("S000"),//DLGPOS_TXT_DOMINATION_MAP_COUNT,
        StrToWzID("S001"),//DLGPOS_TXT_DOMINATION_JOIN_COUNT,
        StrToWzID("S002"),//DLGPOS_TXT_DOMINATION_DEFENSE_COUNT,
        StrToWzID("S003"),//DLGPOS_TXT_DOMINATION_BOSS_HUNTTING_COUNT,

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		StrToWzID("T019"),
		StrToWzID("T020"),
#endif
		StrToWzID("L002"),

		StrToWzID("VS00"),    // DLGPOS_VSCROLL_NOTICE,

		StrToWzID("TI00"),	  // DLGPOS_TI_GUILD_MARK_REGISTER,	  
		StrToWzID("TI01"),	  // DLGPOS_TI_GUILD_GP_EXCHANGE,
		StrToWzID("TI02"),	  // DLGPOS_TI_GUILD_UP_DONATE,
        StrToWzID("I000"),    // DLGPOS_TI_GUILD_DOMINATION_MANAGEMENT,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiGuildWindow::OnShowWindow( BOOL val )
{
    if (val)
    {
        assert(this->m_bControlsInitialized);
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

 
		//m_bShow = true;
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_guild_window;
        msg.wParam = InterfaceManager::DIALOG_GUILD_WINDOW;
        msg.DoSomething = GlobalFunc::DoGuildWindow;
        g_KeyQueueManager.PushBack(msg);

		setGuildNotice();

    }
    else
    {
		//m_bShow = false;

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_guild_window);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_CHKBTN_MEMBER_INFOS:
                {
     //               this->ShowInterface(FALSE);
					//GET_DIALOG( IM_GUILD_MANAGER::GUILD_MAIN )->ShowInterface(TRUE);
                    GetManager()->ChangeTab(GET_DIALOG(IM_GUILD_MANAGER::GUILD_MAIN));
                }
                break;

            case DLGPOS_BTN_EXIT:
                {
                    if (GetManager() != NULL)
                    {
                        GetManager()->set_active_tab(this);
                    }
                    ShowInterfaceWithSound(FALSE);
					//m_bShow = false;
                }
                break;

			case DLGPOS_TI_GUILD_GP_EXCHANGE:
				{
					// gp_exchange 다이얼로그 show 하기
					uiBase * pConvertGP = GET_DIALOG( IM_GUILD_MANAGER::GUILD_CONVERT_GP );
					if( pConvertGP)
					{
						pConvertGP->ShowInterface(TRUE);
					}					
				}
				break;

			case DLGPOS_TI_GUILD_UP_DONATE:
				{
					// gp_donate 다이얼로그 show 하
					uiBase * pDonateUP = GET_DIALOG( IM_GUILD_MANAGER::GUILD_DONATE_UP );

					if( pDonateUP )
					{
						pDonateUP->ShowInterface(TRUE);
					}
				}
				break;
            case DLGPOS_TI_GUILD_DOMINATION_MANAGEMENT:
                {
                    ClickDominationManagementButton();
                }
                break;
            }
        }
        break;

	
		case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
		case RT_MSG_SB_LINETOP:			//.윗쪽 화살표를 눌렀다.
			{
				if(m_pScroll)
				{
					int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
					int mmin = 0 , mmax = 0;
					m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

					if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
					{
						int nSkipLevel=SHOW_DEFAULT_NOTICE_LINE/2;//반페이지이동
						( (pos-nSkipLevel)<mmin ? (pos=mmin):(pos-=nSkipLevel));

						m_pScroll->SetScrollPosWZ(pos);

						this->refreshGuildNotice();
					}
				}
			}
			break;

		case RT_MSG_SB_PAGEBOTTOM:				//. 아래쪽 몸통을 눌렀다.
		case RT_MSG_SB_LINEBOTTOM:				//.아랫쪽 화살표를 눌렀다
			{
				if(m_pScroll)
				{
					int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
					int mmin = 0 , mmax = 0;
					m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

					if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
					{
						int nSkipLevel=SHOW_DEFAULT_NOTICE_LINE/2;//반페이지이동
						( (pos+nSkipLevel)>mmax ? (pos=mmax):(pos+=nSkipLevel));

						m_pScroll->SetScrollPosWZ(pos);

						this->refreshGuildNotice();
					}
				}
			}
			break;


		case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
		case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
		case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
			{
				if(m_pScroll)
				{
					this->refreshGuildNotice();
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
uiGuildWindow::OnUpdateSolarDialog()
{
	if (!IsVisible())
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

    this->m_pChkBtnControls[1]->SetCheckState(true);

    this->updateGuildInfo();
}

void uiGuildWindow::ClickDominationManagementButton()
{
    ContinentMapDlg* continent_map_dialog = 
        GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
    if (continent_map_dialog != NULL)
    {
        BOOL is_visible = continent_map_dialog->IsVisible();
        if (is_visible == FALSE)
        {
            continent_map_dialog->set_continent_map_mode(kContinentDominationManagerMode);
            continent_map_dialog->ShowInterface(TRUE);
        }
        else
        {
            if (continent_map_dialog->continent_map_mode() == kContinentNormalMode)
            {
                continent_map_dialog->set_continent_map_mode(kContinentDominationManagerMode);
            }
        }
    }
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
