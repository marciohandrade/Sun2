//------------------------------------------------------------------------------
//  uiBattleZoneList_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneList.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "Hero.h"
#include "ChatDialog.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "map.h"
#include "HeroActionInput.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiChatMan/uiChatMan_def.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneList::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B999"),  // DLGPOS_BTN_EXIT=0,
    StrToWzID("TI10"),  // DLGPOS_BTN_CREATE,
    StrToWzID("TI09"),  // DLGPOS_BTN_JOIN,
    StrToWzID("TI04"),  // DLGPOS_BTN_REFRESH_ROOMLIST,
    StrToWzID("TI05"),  // DLGPOS_BTN_SEARCH,
    StrToWzID("TI06"),  // DLGPOS_BTN_SHORTCUT_MISSION,
#ifndef	_EXCECPT_HUNTING_001011
    StrToWzID("TI07"),  // DLGPOS_BTN_SHORTCUT_HUNTING,
    StrToWzID("TI08"),  // DLGPOS_BTN_SHORTCUT_CHUNTING,
#endif
    StrToWzID("TI01"),  // DLGPOS_BTN_SHORTCUT_PVP,

    StrToWzID("C001"),  // DLGPOS_CHKBTN_VIEW_ALL,
    StrToWzID("C002"),  // DLGPOS_CHKBTN_VIEW_POSSIBLE,
    StrToWzID("C011"),  // DLGPOS_CHKBTN_SEARCH_TYPE_ID,
    StrToWzID("C012"),  // DLGPOS_CHKBTN_SEARCH_TYPE_ROOM_NO,
    
    StrToWzID("E001"),  // DLGPOS_EDT_SEARCH,
    StrToWzID("VS00"),  // DLGPOS_SCR_VERTICAL,
    StrToWzID("L000"),  // DLGPOS_LST_ROOMLIST,
    StrToWzID("TI00"),  // DLGPOS_TAB_ROOMLIST,

	StrToWzID("ST00"),  // DLGPOS_LIST_COLUM1,
	StrToWzID("ST01"),  // DLGPOS_LIST_COLUM2,
	StrToWzID("ST02"),  // DLGPOS_LIST_COLUM3,
	StrToWzID("ST03"),  // DLGPOS_LIST_COLUM4,
	StrToWzID("ST04"),  // DLGPOS_LIST_COLUM5,
	StrToWzID("ST05"),  // DLGPOS_LIST_COLUM6,
	StrToWzID("ST07"),  // DLGPOS_LIST_COLUM7,
	StrToWzID("ST06"),  // DLGPOS_LIST_COLUM8,

};

//------------------------------------------------------------------------------
/**
called to show request
*/
void
uiBattleZoneList::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
        {
            ShowInterface(FALSE);
            return;
        }

		if(GetManager())
			GetManager()->InitPKBooleans();

        this->Clear();

        //UpdatePosition();

        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
            partyMan->uiClearAll();

        // TODO: 이거 제대로 동작하는가?
        g_HeroInput.ClearPickingTarget();

        // 베틀 존 목록 UI 활성화 할 경우 맵 렌더링을 OFF
        if (g_pMap)
            g_pMap->SetRenderFlag(FALSE);

        ////
        //// support widemode
        ////
        //RECT rcDlgSize;
        //this->GetDialogWZ()->GetDlgSize(&rcDlgSize);

        //POINT pt = g_pApplication->Get2DModeMapping();

        //RECT rcInterface;
        //rcInterface.left = rcInterface.top = 0;
        //rcInterface.right = pt.x;
        //rcInterface.bottom = pt.y;

        //int x = (rcInterface.right / 2) - (rcDlgSize.right / 2);
        //int y = (rcInterface.bottom / 2) - (rcDlgSize.bottom / 2);

        //MoveWindowWZ( (float)x, (float)y );

        //
        // setup almost top to chet dialog
        //
		AttachChatDialog();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_battle_List;
        msg.wParam=InterfaceManager::DIALOG_BATTLEZONE_LIST;
        msg.DoSomething=GlobalFunc::DoZoneList;
        g_KeyQueueManager.PushBack(msg);

		GetManager()->SetWait(false);

        GetManager()->ClearCHuntingJoinedFlag();
        GetManager()->ClearHasJoinedFlag();

		m_rtList = m_pLstControl->GetSizeRect();
#ifdef _INTERNATIONAL_UI
		m_rtList.left += 94;
#else
		m_rtList.left += 174;
#endif//_INTERNATIONAL_UI
		m_rtList.right = m_rtList.left + 20;
    }
    else
    {

        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
            partyMan->uiShowAll();

        // 맵 렌더링 On
        if (g_pMap)
        {
            g_pMap->SetRenderFlag(TRUE);
        }

        ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
        if (pChatDlg)
        {
            // 최상위로 설정된 채팅 다이얼로그 닫기
            g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);
            // 채팅 속성창도 디테치
		    g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_MENU_DIALOG);
            g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG);
            g_InterfaceManager.DeAttachFirstDialog(IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);
            pChatDlg->MoveDefaultPosition();
            pChatDlg->ShowWindow(FALSE);
        }

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_battle_List);

		GetManager()->SetWait(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void     
uiBattleZoneList::AttachChatDialog()
{
	ChatDialog * pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (pChatDlg)
    {
		g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_DIALOG);
		pChatDlg->ShowWindow(TRUE);

        RECT rcClient;
        this->GetDialogWZ()->GetClientRect(&rcClient);
        pChatDlg->MoveBattleZonePosition( rcClient );

        //채팅관련 속성창도 위에 표시
		g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_MENU_DIALOG);
        g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG);
        g_InterfaceManager.AttachFirstDialog(IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);
	}
}

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiBattleZoneList::UpdatePosition()
//{
//    //MoveWindowWZ(748, 180);
//}

void _callback_zoneList_partyleave (bool bYes)
{
    if (bYes)
    {
        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
        {
            if (partyMan->IsParty())
            {
                partyMan->LeaveParty();
            }
        }

		GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->ShowInterface( TRUE );
        
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	if(GetManager()->IsWait())
	{
		return;
	}

    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_LBUTTONDCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			switch(pos)
			{
			case DLGPOS_LST_ROOMLIST:
				{
					_joinRoom(false);
				}
				break;
			}
		}
		break;

    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( pos )
            {
            case DLGPOS_BTN_EXIT:
                {
                    ExitZoneList();
                }
                break;

            case DLGPOS_BTN_CREATE:
                {
                    DWORD dialog_id_code = IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE;
                    int continent_number = 1;
                    if(g_pHero && g_pMap)
                    {
                        const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
                        if(map_info)
                        {
                            continent_number = map_info->ContienetNumber;
                        }
                    }

                    if (continent_number == 1)
                    {
                        dialog_id_code = IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE;
                    }
                    else
                    {
                        dialog_id_code = IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2;
                    }

                    uiPartyMan* partyMan =
                        static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
                    if (partyMan)
                    {
                        if (partyMan->IsParty() && !partyMan->IsLeader())
                        {
							TCHAR	szMsg[INTERFACE_STRING_LENGTH];
							//파티에 속해있습니다.\n다른 존으로의 이동을 위해서 파티를 탈퇴하셔야 합니다.\n\n탈퇴하시겠습니까?
							g_InterfaceManager.GetInterfaceString(eST_ANOTHER_ZONE_JOIN_REQUIRE_QUIT_PARTY,szMsg,INTERFACE_STRING_LENGTH);

							uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_ConfirmLock(szMsg, _callback_zoneList_partyleave);

                        }
                        else
						{
							GET_DIALOG(dialog_id_code)->ShowInterface( TRUE );
                            
						}
                    }
                    else
					{
						GET_DIALOG(dialog_id_code)->ShowInterface( TRUE );
						
					}
                }
                break;

            case DLGPOS_BTN_REFRESH_ROOMLIST:
                {
                    this->GetManager()->RequestZoneList();
                }
                break;

            case DLGPOS_BTN_JOIN:
                {
					_joinRoom();
                }
                break;

            case DLGPOS_CHKBTN_VIEW_ALL:
                {
                    this->m_cfgs.viewType = eCHKVIEW_TOTAL;
                }
                break;
            case DLGPOS_CHKBTN_VIEW_POSSIBLE:
                {
                    this->m_cfgs.viewType = eCHKVIEW_POSSIBLE;
                }
                break;


            case DLGPOS_CHKBTN_SEARCH_TYPE_ID:
            case DLGPOS_CHKBTN_SEARCH_TYPE_ROOM_NO:
                {
                    this->m_cfgs.searchType = (pos - POS_CHKBTN_SEARCH_BEGIN);	
                }
                break;

            case DLGPOS_BTN_SHORTCUT_MISSION:
#ifndef _EXCECPT_HUNTING_001011
            case DLGPOS_BTN_SHORTCUT_HUNTING:
            case DLGPOS_BTN_SHORTCUT_CHUNTING:
#endif
            case DLGPOS_BTN_SHORTCUT_PVP:
                {
                    this->GetManager()->ShortCut( pos - DLGPOS_BTN_SHORTCUT_MISSION );
                }
                break;

            case DLGPOS_BTN_SEARCH:
                {
                    ZeroMemory(m_cfgs.strSearchID, sizeof(m_cfgs.strSearchID));
                    // 캐릭터명의 최대 사이즈(16 byte)를 넘었습니다.
                    if(!GlobalFunc::CompareLimitText(m_pEdtControl, m_cfgs.strSearchID, MAX_CHARNAME_LENGTH, 5790))
                        break;

                    if (m_cfgs.searchType == eCHKSEARCH_ID)
                    {
                        if (lstrcmp (m_cfgs.strSearchID, _T("")))
                        {
                            this->GetManager()->SearchID(m_cfgs.strSearchID);
                        }
                    }
                    else
                    {
                        if (lstrcmp (m_cfgs.strSearchID, _T("")))
                        {
                            bool allAlpha = true;
                            for (int i = 0; i < lstrlen(m_cfgs.strSearchID); ++i)
                            {
                                if (!(m_cfgs.strSearchID[i]>=_T('0') && m_cfgs.strSearchID[i]<=_T('9')))
                                {
                                    allAlpha = false;
                                }
                            }

                            int value = _ttoi(m_cfgs.strSearchID);

                            if (allAlpha)
                            {
                                this->GetManager()->SearchNo(value);
                            }
                            else
                            {
                                g_InterfaceManager.GetInterfaceString(5211, szMessage, INTERFACE_STRING_LENGTH);

								uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
								if(pSystemMan)
									pSystemMan->Sys_VerifyLock(szMessage);

                            }
                        }
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            if(m_pVScrollControl)
			{
				int pos = m_pVScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pVScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
				{
					pos--;      // 한줄 감소 시키고 
					m_pVScrollControl->SetScrollPosWZ(pos);
				}
			}
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
        {
            //.아랫쪽 화살표를 눌렀다
            if(m_pVScrollControl)
			{
				int pos = m_pVScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pVScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
				{
					pos++;      // 한줄 증가 시키고 
					m_pVScrollControl->SetScrollPosWZ(pos);
				}
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
uiBattleZoneList::OnUpdateSolarDialog()
{
	if(GetManager()->IsWait())
	{
		return;
	}

    this->updateCheckBtns();
    this->updateLists();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::OnRenderSolarDialog()
{
    this->RenderSuperMasterIcon();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::Process(DWORD dwTick)
{
    if (!this->IsVisible())
        return;

	if(GetManager()->IsSearchMode())
		return;

    if (m_timerRefresh.Check() != TIMER_STAT_NONE)
    {
        if (m_timerRefresh.Check() == TIMER_STAT_FINISH)
        {
			this->GetManager()->RequestZoneList();

			m_timerRefresh.Off();
            m_timerRefresh.On(c_iCYCLE_REFRESH_ROOM_LIST);
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
