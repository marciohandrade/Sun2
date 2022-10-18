#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "RadarInfoParser.h"
#include "Quest.h"
#include "QuestManager_Concrete.h"
#include "Map.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "QuestTextInfoParser.h"
#include "NPCInfoParser.h"
#include "questinfomanagerex.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "Hero.h"
#include "MissionManager_Concrete.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "uiAssistMan/uiAssistMan.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "DominationInfoParser.h"
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"
#include "uiDominationBettingList/uiDominationBettingList.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "uiDominationZoneMarkDlg/ZoneMarkRewardInfo.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef _NA_000000_20130114_RENEWER_UI
const DWORD kPerFrameTickCount = 30;
#endif //_NA_000000_20130114_RENEWER_UI

WzID ContinentMapDlg::m_wzID[eCONTINENT_MAP_POS_MAX] = 
{
		StrToWzID("BT00"), //eCONTINENT_MAP_POS_POINT_BTN_START = 0,
		StrToWzID("BT01"),
		StrToWzID("BT02"),
		StrToWzID("BT03"),
		StrToWzID("BT04"),
		StrToWzID("BT05"),
		StrToWzID("BT06"),
		StrToWzID("BT07"),
		StrToWzID("BT08"),
		StrToWzID("BT09"),
		StrToWzID("BT10"),
		StrToWzID("BT11"),
		StrToWzID("BT12"),
		StrToWzID("BT13"),
		StrToWzID("BT14"),
		StrToWzID("BT15"),
		StrToWzID("BT16"),
		StrToWzID("BT17"),
		StrToWzID("BT18"),
		StrToWzID("BT19"),
		StrToWzID("BT20"),
		StrToWzID("BT21"),
		StrToWzID("BT22"),
		StrToWzID("BT23"),
		StrToWzID("BT24"),
		StrToWzID("BT25"),
		StrToWzID("BT26"),
		StrToWzID("BT27"),
		StrToWzID("BT28"),
		StrToWzID("BT29"),
		StrToWzID("BT30"), //eCONTINENT_MAP_POS_POINT_BTN_END = 30,
		StrToWzID("BT32"),
		StrToWzID("BT33"),
		StrToWzID("BT34"),
		StrToWzID("BT35"),
		StrToWzID("BT36"),
		StrToWzID("BT37"),
		StrToWzID("BT38"), //eCONTINENT_MAP_POS_POINT_BTN_END = 37,

        StrToWzID("B999"), //eCONTINENT_MAP_POS_EXIT_BTN = 31,
        StrToWzID("S005"),   // 지도 팝업 선택된 것
        StrToWzID("L001"),  // 지도 팝업 리스트
        StrToWzID("B000"),  // 지도 팝업 화살표
        StrToWzID("C000"),  // 신규 대륙 전체 영역
        StrToWzID("C001"),  // 신규 대륙 영역1
        StrToWzID("C002"),  // 신규 대륙 영역2
        StrToWzID("C003"),  // 신규 대륙 영역3

        StrToWzID("P005"),  // 점령전 진행 아이콘 도움말

		//eCONTINENT_MAP_POS_MAX = 32,
};

WzID ContinentMapDlg::m_wzID_DominationCtrl[eCMDOMINATION_POS_MAX] = 
{
		StrToWzID("PI02"),//eCMDOMINATION_DIALOG_PIC
		StrToWzID("BT99"),//eCMDOMINATION_GUILD_MARK_BTN
		StrToWzID("ST00"),//eCMDOMINATION_GUILD_NAME_TEXT
		StrToWzID("ST01"),//eCMDOMINATION_GUILD_MASTER_TEXT
};
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
WzID ContinentMapDlg::wz_id_continent_owner_guild_control[eCONTINENT_MAP_OWNER_GUILD_POS_MAX] = 
{
    StrToWzID("B002"),//eCONTINENT_MAP_OWNER_GUILD_MARK_BTN
    StrToWzID("S000"),//eCONTINENT_MAP_OWNER_GUILD_NAME_TEXT
    StrToWzID("S001"),//eCONTINENT_MAP_OWNER_GUILD_MASTER_TEXT
};
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ContinentMapDlg::ContinentMapDlg(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;

    continent_map_mode_ = kContinentNormalMode;
    area_select_image_ = INVALID_HANDLE_VALUE;
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    is_actived_map_portal_ = false;
    is_useable_map_portal_ = true;
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

#ifdef _NA_000000_20130114_RENEWER_UI
	all_hide_alpha = -1;
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ContinentMapDlg::~ContinentMapDlg()
{
	Release();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();

    LoadImage();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::Release()
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if(area_select_image_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(area_select_image_);
        area_select_image_ = INVALID_HANDLE_VALUE;
    }
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

void ContinentMapDlg::SetAirShipMode()
{
    set_continent_map_mode(kContinentAirShipMode);

    ZeroMemory(airship_mapcode, sizeof(airship_mapcode));

    sMAPPORTAL_HASH* portal_hash =  MapInfoParser::Instance()->GetMapPortalHashTable();
    sMAPPORTAL_HASH::const_iterator itr = portal_hash->begin();

    while (itr != portal_hash->end())
    {
        sMAP_ENTERENCE_PORTAL* portal_info = itr->second;// (*itr);

        // npc_id값이 유효하고 현재 필드와동일하다면
        if (portal_info &&
           (portal_info->npc_id_ != 0) &&
           (portal_info->wWorldFrom == g_pMap->GetMapID()))
        {
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(portal_info->wWorldTo);

            //대륙맵에서 이동가능지역 활성화
            if (map_info &&
               (map_info->wContinentCode > 0) &&
               (map_info->wContinentCode <= eCONTINENT_MAP_POS_POINT_BTN_CNT)
               && (map_info->ContienetNumber == kContinentEP1MapNumber)
               )
            {
                airship_mapcode[map_info->wContinentCode - 1] = portal_info->wIndex; 
            }
        }
        ++itr;
    }
}
void ContinentMapDlg::RenderAirShipPoint()
{
    RECT* over_rect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_AIRSHIP_OVER);
    RECT* on_rect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_AIRSHIP_ON);

    if ((over_rect == NULL) ||
       (over_rect == NULL))
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    for (int index = 0; index < eCONTINENT_MAP_POS_POINT_BTN_CNT; ++index)
    {
        if (airship_mapcode[index] == 0)
        {
            continue;
        }
        
        if (m_pPointBtn[index] == NULL)
        {
            continue;
        }
        
        RECT rect_button = m_pPointBtn[index]->GetSizeRect();
        
        // 버튼 중심점
        int center_x = rect_button.left + (rect_button.right - rect_button.left)/2;
        int center_y = rect_button.top + (rect_button.bottom - rect_button.top)/2;
        
        // 버튼중심점으로부터 비행선크기에 맞게 재조정
        rect_button.left = center_x - (over_rect->right/2);
        rect_button.top = center_y - (over_rect->bottom/2);
        rect_button.right = rect_button.left + over_rect->right;
        rect_button.bottom = rect_button.top + over_rect->bottom;
        
        bool is_mouse_over = false;
        if ((mouse_x > rect_button.left) && 
            (mouse_x < rect_button.right) && 
            (mouse_y > rect_button.top) && 
            (mouse_y < rect_button.bottom))
        {
            is_mouse_over = true;
            RenderAirShipTooltip(airship_mapcode[index]);
        }

        RECT* rect = is_mouse_over ? (over_rect):(on_rect);
        g_pSunRenderer->RenderTexture(RadarInfoParser::Instance()->GetTexture(),
                                      (float)rect_button.left,
                                      (float)rect_button.top,
                                      (float)rect->right,
                                      (float)rect->bottom,
                                      (float)rect->left,
                                      (float)rect->top,
                                      (float)rect->right,
                                      (float)rect->bottom);
    }

}

void ContinentMapDlg::RenderAirShipTooltip(WORD portal_index)
{
    MapInfoParser* parser = MapInfoParser::Instance();

    uiToolTipMan* tooltip = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    if (tooltip == NULL)
    {
        return;
    }

    sMAP_ENTERENCE_PORTAL* portal_info = parser->GetMapPortal(portal_index);
    if (portal_info == NULL)
    {
        return;
    }

    const sMAPINFO* world_to = parser->FindMapInfo(portal_info->wWorldTo);
    if (world_to == NULL)
    {
        return;
    }

    TCHAR message[INTERFACE_STRING_LENGTH + 1] = {0,};
    TCHAR map_name[INTERFACE_STRING_LENGTH + 1] = {0,};
    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0,};
    
    //11470	%s \n%d LV %d 하임(클릭 시 이동)
    g_InterfaceManager.GetInterfaceString(11470, message);

    // 이동할 맵지역
    g_InterfaceManager.GetInterfaceString(world_to->dwNCode, map_name);
    
    Snprintf(tooltip_message,
             INTERFACE_STRING_LENGTH,
             message,                  
             map_name,                  // 이동할 목적지
             portal_info->byMin_Level,  // 이동 가능레벨
             portal_info->dwHeim);      // 이동요금    

    m_pGuildNameText->SetTextWZ(map_name);

    tooltip->RegisterTooltip(tooltip_message,
                            -1, 
                            22, 
                            StrToWzID("s210"),
                            WzColor_RGBA(255, 255, 255, 255),
                            TOOLTIP_BG_WZCOLOR,
                            c_dwTOOLTIP_EXTRA_INFO_DT_LEFT,
                            eSPECIAL_TOOLTIP_NOMAL_ITEM);

    tooltip->AddEmptyLineForTooltip();
	tooltip->SetTooltipDialogID( GetDialogKey() );

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ContinentMapDlg::OnRenderSolarDialog()
{
	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();

    DOMINATION_GUILD_INFO* area_over_guild_info = NULL;
    POSTYPE over_button_pos = 0xFF;
    bool is_area_over = false;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    DOMINATION_GUILD_INFO* continent_owner_guild_info = NULL;

    continent_owner_guild_name_->SetTextWZ(_T(""));
    continent_owner_guild_master_->SetTextWZ(_T(""));
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	m_pGuildNameText->SetTextWZ(_T(" "));
	m_pGuildMasterText->SetTextWZ(_T(" "));

    ContinentMapMode map_mode = continent_map_mode();

    switch (map_mode)
    {
    case kContinentDominationManagerSelectedAreaMode:
        {
            RenderSelectAreaImage(area_info_.area_button, true);
        }
        break;
    case kContinentAirShipMode:
        {
            RenderAirShipPoint();
        }
        return;
    }

	uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
	if (pUIMan)
	{
		DOMINATIONGUILDMAP& mapDominatioGuild = pUIMan->GetDominationGuildMap();
		if ( pUIMan->ShowDominationGuildInfo() && mapDominatioGuild.size() > 0)
		{
			RECT rc;
            MAPCODE tempMapCode = 0;
            DOMINATION_GUILD_INFO* pGuildInfo = NULL;
			DOMINATIONMAP_ITR itr = mapDominatioGuild.begin();
			DOMINATIONMAP_ITR itrEnd = mapDominatioGuild.end();
			for ( ;itr != itrEnd; ++itr )
			{
				tempMapCode = itr->first;
				pGuildInfo = &(itr->second);

				const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(tempMapCode);				

				if (NULL == pMapInfo)
					continue;
				if( -1 >= pMapInfo->wContinentCode - 1)
					continue;
                if (pMapInfo->ContienetNumber != kContinentEP1MapNumber)// 기존대륙
                {
                    continue;
                }
				if(NULL == m_pPointBtn[pMapInfo->wContinentCode -1])
					continue;

				uiDominationTowerMan::RenderGuildMark(m_pPointBtn[pMapInfo->wContinentCode -1], 
                                                      pGuildInfo, 255);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
                // 점령전 진행 상태 마크 출력
                RenderDominationProgressMark(m_pPointBtn[pMapInfo->wContinentCode -1], pGuildInfo);

                // 점령전 진행단계 도움말 컨트롤 표시 여부
                CControlWZ* control_pointer = getControl(eCONTINENT_MAP_POS_DOMINATION_HELP);
                
                if (control_pointer &&
                    itr == mapDominatioGuild.begin())
                {
                    if (pGuildInfo->progress_state_ == DOMINATION_PROGRESS_STATE_NONE)
                    {
                        control_pointer->ShowWindowWZ(WZ_HIDE);
                    }
                    else
                    {
                        control_pointer->ShowWindowWZ(WZ_SHOW);
                    }
                }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

                // mouseover 길드명 및 마스터 출력
				rc = m_pPointBtn[pMapInfo->wContinentCode -1]->GetSizeRect();
				if(iMouseX > rc.left && 
					iMouseX < rc.right && 
					iMouseY > rc.top && 
					iMouseY < rc.bottom)
				{
                    is_area_over = true;
					area_over_guild_info = pGuildInfo;
                    over_button_pos = pMapInfo->wContinentCode -1;
				}
                if (map_mode == kContinentDominationManagerSelectedAreaMode)
                {
                    MAPCODE temp_domination_mapcode = 
                        DominationInfoParser::Instance()->GetDominationFieldCode(tempMapCode);
                    if (area_info_.area_mapcode == temp_domination_mapcode)
                    {
                        uiDominationTowerMan::RenderGuildMark(m_pGuildMarkBtn, pGuildInfo, 255);
                        m_pGuildNameText->SetTextWZ(pGuildInfo->m_tszGuildName);
                        m_pGuildMasterText->SetTextWZ(pGuildInfo->m_tszGuildMaster);
                    }
                }
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL                
                const sDOMINATION_INFO* domination_info =
                    DominationInfoParser::Instance()->GetDominationInfo(tempMapCode);
                if (domination_info && domination_info->m_DominationType == DOMINATION_TYPE_MONDSCHIEN
                    && pMapInfo->ContienetNumber == 1)
                {
                    // 1 대륙의 몬트샤인을 점령한 길드일 경우 표시
                    continent_owner_guild_info = pGuildInfo;
                }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
			}
		}

        if (is_area_over == true)
        {
            uiDominationTowerMan::RenderGuildMark(m_pGuildMarkBtn, area_over_guild_info, 255);
            m_pGuildNameText->SetTextWZ(area_over_guild_info->m_tszGuildName);
            m_pGuildMasterText->SetTextWZ(area_over_guild_info->m_tszGuildMaster);
            if (over_button_pos != 0xFF)
            {
                RenderSelectAreaImage(m_pPointBtn[over_button_pos], false);
            }
        }
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        if (continent_owner_guild_info)
        {
            uiDominationTowerMan::RenderGuildMark(continent_owner_guild_mark_button_, continent_owner_guild_info, 255);
            continent_owner_guild_name_->SetTextWZ(continent_owner_guild_info->m_tszGuildName);
            continent_owner_guild_master_->SetTextWZ(continent_owner_guild_info->m_tszGuildMaster);
        }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    if ((map_mode == kContinentDominationManagerMode) ||
        (map_mode == kContinentDominationManagerMode))
    {
        // 각 지역의 옵션 출력
        for (int area_index = 0; area_index < eCONTINENT_MAP_POS_POINT_BTN_CNT; ++area_index)
        {
            CCtrlButtonWZ* area_button = m_pPointBtn[area_index];
            if (area_button == NULL)
            {
                continue;
            }

            RECT rc = area_button->GetSizeRect();
            if(iMouseX > rc.left && 
                iMouseX < rc.right && 
                iMouseY > rc.top && 
                iMouseY < rc.bottom)
            {
                ZoneMarkRewardInfo zone_mark_info;
                MAPCODE map_portal_infomations[kContinentMapPortalMax];
                int count = MapInfoParser::Instance()->FindMapPortalInfo(map_portal_infomations, 
                    kContinentMapPortalMax, 
                    area_index + 1, 
                    kContinentEP1MapNumber);
                if (count > 0)
                {
                    zone_mark_info.SetZoneMarkInfo(map_portal_infomations[0]);
                    zone_mark_info.RenderTooltip(0, true, GetDialogKey());
                }
            }
        }
    }
    else
#endif //_NA_003923_20120130_GUILD_RENEWAL    
    {
        uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
        if (ui_radar_manager != NULL)
        {
            ui_radar_manager->RenderNavigationPoint();
        } 
    }

	RECT* pRect = NULL;

	if(g_pHero && g_pMap)
	{
		const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
		if(pMapInfo)
		{
            if (pMapInfo->ContienetNumber != kContinentEP1MapNumber) // 브라키온 대륙이 아니면
            {
                return;
            }

			if( -1 >= pMapInfo->wContinentCode - 1)
				return;

			float fWidth = (float)m_pPointBtn[pMapInfo->wContinentCode - 1]->GetSizeRect().left;
			float fHeight = (float)m_pPointBtn[pMapInfo->wContinentCode - 1]->GetSizeRect().top;

			pRect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_CONTINENT_MAP_HERO_POS);
			g_pSunRenderer->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
				fWidth - 20.f,
				fHeight - 48.f,
				(float)pRect->right,
				(float)pRect->bottom,
				(float)pRect->left,
				(float)pRect->top,
				(float)pRect->right,
				(float)pRect->bottom,
				NULL,
				0.0f, 
				RTO_DEFAULT);

#ifdef _NA_000000_20130114_RENEWER_UI
			static DWORD m_LastTickCount = 0;
			DWORD tick_count = clock_function::GetTickCount();

			if (m_LastTickCount == 0)
			{
				m_LastTickCount = tick_count;
			}

			DWORD current_tick_count = (tick_count - m_LastTickCount);
			if (current_tick_count > kPerFrameTickCount)
			{
				m_LastTickCount = tick_count;
			}

			RenderSplineImageAlphaWarp(current_tick_count,fWidth - 20.f,fHeight - 48.f);
#endif
			
		}
	}


	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		m_bShow = TRUE;

        uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
        if (ui_radar_manager != NULL)
        {
            ui_radar_manager->SetNavigationPoint();
        }

		uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
		if (pUIMan)
		{
			pUIMan->SEND_DOMINATION_MINIMAP_LIST_SYN();
		}

        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_Dialog_Continent_Map_Dlg;
		msg.DoSomething=GlobalFunc::DoContinentMapDlg;
		g_KeyQueueManager.PushBack(msg);

        if (popup_select_text_)
        {
            popup_select_text_->SetTextID(51010);// 51010: 브라키온 대륙
            popup_list_->SetCurSel(0);// 브라키온 대륙
        }
	}
	else
	{
		m_bShow = FALSE;

		g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Continent_Map_Dlg);

        continent_map_mode_ = kContinentNormalMode;

        uiDominationTowerMan::HideDominationManagmentDialog();
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
        is_actived_map_portal_ = false;
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL
	}
}

void ContinentMapDlg::ClickAIrshipPortal(POSTYPE pos)
{
    if ((pos < eCONTINENT_MAP_POS_POINT_BTN_CNT) &&
       (airship_mapcode[pos] != 0))
    {
        sMAP_ENTERENCE_PORTAL* portal_info = MapInfoParser::Instance()->GetMapPortal(airship_mapcode[pos]);
        
        if (portal_info == NULL)
        {
            return;
        }

        uiAssistMan* assist_man = static_cast<uiAssistMan*>(
            g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

        if (assist_man)
        {
            assist_man->ShowAskAirShipPortal(portal_info->wIndex, true);
        }
    }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
            POSTYPE control_pos = getControlIDToPos(pMessage->dwCtrlID);
            if (control_pos == eCONTINENT_MAP_POS_EXIT_BTN)
            {
                OnShowWindow(FALSE);
            }
            else if (control_pos >= eCONTINENT_MAP_POS_AREA1 && control_pos <= eCONTINENT_MAP_POS_AREA3)
            {
                // 대륙맵 변경
                ContinentEp2MapDlg* continent_map_ep2_dialog = 
                    GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
                if (continent_map_ep2_dialog != NULL)
                {
                    continent_map_ep2_dialog->ChangeContinentCode(continent_map_mode_);
                    this->ShowInterface(FALSE);
                    continent_map_ep2_dialog->ShowWindow(WZ_SHOW);
                }
            }
            else if (control_pos == eCONTINENT_MAP_POS_POPUP_BUTTON)
            {
                if (popup_list_)
                {
                    if (is_popup_list_show_ == false)
                    {
                        is_popup_list_show_ = true;
                        popup_list_->ShowWindowWZ(WZ_SHOW);
                    }
                    else
                    {
                        is_popup_list_show_ = false;
                        popup_list_->ShowWindowWZ(WZ_HIDE);
                    }                    
                }
            }
            else if (control_pos == eCONTINENT_MAP_POS_POPUP_LIST)
            {
                if (popup_list_)
                {
                    int iSelect = popup_list_->GetCurSel();
                
                    if (iSelect == 0)
                    {
                        popup_list_->ShowWindowWZ(WZ_HIDE);
                    }
                    else if (iSelect == 1)
                    {
                        popup_list_->ShowWindowWZ(WZ_HIDE);
                        // 대륙맵 변경
                        ContinentEp2MapDlg* continent_map_ep2_dialog = 
                            GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
                        if (continent_map_ep2_dialog != NULL)
                        {
                            continent_map_ep2_dialog->ChangeContinentCode(continent_map_mode_);
                            this->ShowInterface(FALSE);
                            continent_map_ep2_dialog->ShowWindow(WZ_SHOW);
                        }
                    }
                }
            }
            else
            {
                if (continent_map_mode() == kContinentNormalMode)
                {
                    break;
                }

                // 점령전관리 컨트롤 처리
                if (continent_map_mode() == kContinentAirShipMode)
                {
                    ClickAIrshipPortal(control_pos);
                }
                else
                {
                    if ((control_pos >= eCONTINENT_MAP_POS_POINT_BTN_START) &&
                        (control_pos <= eCONTINENT_MAP_POS_POINT_BTN_END))
                    {
                        ClickAreaButton(control_pos);
                    }
                }
            }
		}
		break;
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    case RT_MSG_RBUTTONCLICK:
        {
            if (continent_map_mode() == kContinentNormalMode)
            {
                if (is_useable_map_portal_ == true)
                {
                    POSTYPE control_pos = getControlIDToPos(pMessage->dwCtrlID);
                    if (control_pos >= eCONTINENT_MAP_POS_POINT_BTN_START && control_pos <= eCONTINENT_MAP_POS_POINT_BTN_END)
                    {
                        GetMapPortalFromControl(control_pos);
                    }
                    else
                    {
                        GetMapPortalFromControl(-1);
                    }
                }
            }
        }
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL
	default:
		{
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentMapDlg::InitControl()
{
	m_pExitBtn			   = static_cast<CCtrlButtonWZ*>(getControl(eCONTINENT_MAP_POS_EXIT_BTN));
	assert(m_pExitBtn);

	for(int i = 0; i < eCONTINENT_MAP_POS_POINT_BTN_CNT; ++i)
	{
		m_pPointBtn[i] = static_cast<CCtrlButtonWZ*>(getControl(eCONTINENT_MAP_POS_POINT_BTN_START + i));
		assert(m_pPointBtn[i]);
	}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    continent_owner_guild_mark_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(wz_id_continent_owner_guild_control[eCONTINENT_MAP_OWNER_GUILD_MARK_BTN]));
    assert(continent_owner_guild_mark_button_);
    continent_owner_guild_name_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(wz_id_continent_owner_guild_control[eCONTINENT_MAP_OWNER_GUILD_NAME_TEXT]));
    assert(continent_owner_guild_name_);
    continent_owner_guild_master_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(wz_id_continent_owner_guild_control[eCONTINENT_MAP_OWNER_GUILD_MASTER_TEXT]));
    assert(continent_owner_guild_master_);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	m_pGuildMarkBtn = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzID_DominationCtrl[eCMDOMINATION_GUILD_MARK_BTN]));
	assert(m_pGuildMarkBtn);
	m_pGuildNameText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzID_DominationCtrl[eCMDOMINATION_GUILD_NAME_TEXT]));
	assert(m_pGuildNameText);
	m_pGuildMasterText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzID_DominationCtrl[eCMDOMINATION_GUILD_MASTER_TEXT]));
	assert(m_pGuildMasterText);

    popup_select_text_ = static_cast<CCtrlStaticWZ*>(getControl(eCONTINENT_MAP_POS_POPUP_SELECT_TEXT));
    assert(popup_select_text_);
    popup_list_ = static_cast<CCtrlListWZ*>(getControl(eCONTINENT_MAP_POS_POPUP_LIST));
    assert(popup_list_);
    if (popup_list_ != NULL)
    {
        popup_list_->ShowWindowWZ(WZ_HIDE);
        is_popup_list_show_ = false;

        popup_list_->DeleteItemAll();
        popup_list_->SetBeginDrawIndex(0);

        int	iSellHeight = popup_list_->GetSellheight();
        RECT rc = popup_list_->GetSizeRect();
        int index;

        DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
        DWORD c_dwSelTextColor = RGBA(0,220,0,255);
        DWORD c_dwBackColor = RGBA(0,0,0,0);

        TCHAR string[INTERFACE_STRING_LENGTH] = {0};
        index = popup_list_->InsertItemBack();
        g_InterfaceManager.GetInterfaceString(51010, string);// 51010: 브라키온 대륙
        popup_list_->SetItemFontID(0, 0, StrToWzID("mn12") );
        GlobalFunc::SetItemText(popup_list_, index, string);
        GlobalFunc::SetItem(popup_list_, index, 0, string, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
            c_dwBackColor, c_dwTextColor, c_dwSelTextColor);

        index = popup_list_->InsertItemBack();
        g_InterfaceManager.GetInterfaceString(51011, string);// 51011 세리엔트 대륙
        popup_list_->SetItemFontID(1, 0, StrToWzID("mn12") );
        GlobalFunc::SetItemText(popup_list_, index, string);
        GlobalFunc::SetItem(popup_list_, index, 0, string, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
            c_dwBackColor, c_dwTextColor, c_dwSelTextColor);
    }
    popup_down_button_ = static_cast<CCtrlButtonWZ*>(getControl(eCONTINENT_MAP_POS_POPUP_BUTTON));
    assert(popup_down_button_);;
    for (int i = 0; i < eNEW_MAP_AREA_MAX; ++i)
    {
        new_world_areas_[i] =static_cast<CCtrlButtonCheckWZ*>(getControl(eCONTINENT_MAP_POS_AREA0 + i));
        assert(new_world_areas_[i]);
    } 

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    CControlWZ* control_pointer = getControl(eCONTINENT_MAP_POS_DOMINATION_HELP);
    if (control_pointer)
    {
        control_pointer->ShowWindowWZ(WZ_HIDE);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

}
void ContinentMapDlg::ChangeContinentCode(int continent_map_mode)
{
    switch(continent_map_mode)
    {
    case kContinentNormalMode:
        continent_map_mode_ = kContinentNormalMode;
        break;
    case kContinentDominationManagerMode:
    case kContinentDominationManagerSelectedAreaMode:
        set_continent_map_mode(kContinentDominationManagerMode);
        break;
    case kContinentAirShipMode:
        SetAirShipMode();
        break;
    default:
        continent_map_mode_ = kContinentNormalMode;
    }
}

void ContinentMapDlg::ClickAreaButton(POSTYPE area_button_pos)
{
    CControlWZ* area_button = getControl(area_button_pos);
    if (area_button == NULL)
    {
        return;
    }

    // 어느 지역인지 조사
    const DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP& domination_info_map = 
        DominationInfoParser::Instance()->GetDominationInfoMap();
    if (domination_info_map.size() > 0)
    {   
        DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP::const_iterator itr = domination_info_map.begin();
        DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP::const_iterator end_itr = domination_info_map.end();
        for ( ;itr != end_itr; ++itr)
        {
            MAPCODE temp_mapcode = itr->first;
            const sDOMINATION_INFO* domination_info = &(itr->second);

            const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(temp_mapcode);				

            if (pMapInfo == NULL)
                continue;
            if (pMapInfo->ContienetNumber != kContinentEP1MapNumber)
            {
                continue;
            }
            if (((pMapInfo->wContinentCode - 1) < eCONTINENT_MAP_POS_POINT_BTN_START) ||
                ((pMapInfo->wContinentCode - 1) > eCONTINENT_MAP_POS_POINT_BTN_END))
                continue;
            if (m_pPointBtn[pMapInfo->wContinentCode - 1] == NULL)
                continue;

            if (area_button == m_pPointBtn[pMapInfo->wContinentCode - 1])
            {
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                // 몬트 샤인일 경우에는 성문관리를 하지 못한다.
                if (domination_info->m_DominationType == DOMINATION_TYPE_MONDSCHIEN)
                    continue;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

                // 해당 지역에 대해서 요청 패킷 전송
                uiDominationTowerMan* domination_manager = 
                    GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
                if (domination_manager)
                {
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
                    // naklu_check // 여기서 점령지역 창을 닫음 (아래서 NOTICE_SYN을 날림)
#else
                    uiBase* betting_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
                    if (betting_list != NULL)
                    {
                        betting_list->ShowInterface(FALSE);
                    }

                    set_continent_map_mode(kContinentDominationManagerSelectedAreaMode);
                    // 선택된 컨트롤 포지션 저장
                    area_info_.area_pos = area_button_pos;
                    // 선택된 버튼컨트롤 저장
                    area_info_.area_button = static_cast<CCtrlButtonWZ*>(area_button);
                    // 점령전 맵코드 저장
                    area_info_.area_mapcode = domination_info->m_DominationMapCode;

                    domination_manager->SEND_DOMINATION_NOTICE_SYN(0);

#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
                    uiBase* join_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST2);
                    if (join_list != NULL)
                    {
                        join_list->ShowInterface(FALSE);
                    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
                }
            }
        }
    }
}

void ContinentMapDlg::LoadImage()
{
    CManagerTexture* texture_manager = g_pSunRenderer->x_pManagerTexture;

    if (texture_manager == NULL)
    {
        return;
    }

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    area_select_image_ = 
        texture_manager->LoadTexture("Data\\Interface\\all_butten_new2.tga", 
                                     TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif

}

void ContinentMapDlg::RenderSelectAreaImage(CCtrlButtonWZ* area_button_control, bool is_select)
{
    if (area_button_control == NULL)
    {
        return;
    }

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if (area_button_control == NULL || area_select_image_ == INVALID_HANDLE_VALUE)
    {
        return;
    }
#endif

    const float render_size = 24.0f;
    RECT control_rect = area_button_control->GetSizeRect();
    float control_width = static_cast<float>(control_rect.right - control_rect.left);
    float control_height = static_cast<float>(control_rect.bottom - control_rect.top);
    float dest_offset = (render_size - control_width) / 2.0f;
    float source_pos_x, source_pos_y;

    if (is_select == true)
    {
        source_pos_x = 807.0f;
        source_pos_y = 682.0f;
    }
    else
    {
        source_pos_x = 807.0f;
        source_pos_y = 658.0f;
    }

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    ENUM_ALPHABLEND old_alpha_mode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

    WzColor color[4] = {WzColor_RGBA(255,255,255,255), 
                        WzColor_RGBA(255,255,255,255),
                        WzColor_RGBA(255,255,255,255),
                        WzColor_RGBA(255,255,255,255)};

    g_pSunRenderer->RenderTexture(area_select_image_, 
                                  static_cast<float>(control_rect.left) - dest_offset, 
                                  static_cast<float>(control_rect.top) - dest_offset, 
                                  render_size , render_size, 
                                  source_pos_x, source_pos_y, 
                                  render_size, render_size, color);

    g_pSunRenderer->SetAlphaBlendMode(old_alpha_mode);
#endif

}

#ifdef _NA_000000_20130114_RENEWER_UI
void ContinentMapDlg::RenderSplineImageAlphaWarp( DWORD rendertick,float x, float y )
{
	//현재 색상에서 알파 색상으로 
	WzColor color[4];

	//선형보간 공식 g(t) = y0 * (1.0f - t) + y1*t;

	//1초의 단위로 나눈다.
	static float GetNowtime = 0;
	static bool turnChange = false;

	GetNowtime += (float)(rendertick/1000.0f);

	int postAlpha = all_hide_alpha;
	all_hide_alpha = GetNowtime >= 1.0f ? (int)((GetNowtime=0.0f) +\
		( turnChange==true) ? -1 : 0):\
		( turnChange==false ? -1 : 0 );

	turnChange = (postAlpha != all_hide_alpha) ? !turnChange : turnChange;


	int ResultLerpColor = (all_hide_alpha == -1) ? (int)(0 *  (1.0f - GetNowtime) + 255 * GetNowtime) : \
		(int)(255 *  (1.0f - GetNowtime) + 0 * GetNowtime);



	color[0] = WzColor_RGBA(255,255,255,ResultLerpColor);
	color[1] = WzColor_RGBA(255,255,255,ResultLerpColor);
	color[2] = WzColor_RGBA(255,255,255,ResultLerpColor);
	color[3] = WzColor_RGBA(255,255,255,ResultLerpColor);

	RECT* pRect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_MAP_ANIMATION);

	if (pRect == NULL)
	{
		return;
	}

	g_pSunRenderer->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
		x,
		y,
		(float)pRect->right,
		(float)pRect->bottom,
		(float)pRect->left,
		(float)pRect->top,
		(float)pRect->right,
		(float)pRect->bottom,
		color);
}
#endif //#ifdef _NA_000000_20130114_RENEWER_UI

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
void ContinentMapDlg::GetMapPortalFromControl(int control_position)
{
    uiDominationBettingList* betting_list = GET_CAST_DIALOG(uiDominationBettingList, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
    if (betting_list != NULL)
    {
        betting_list->ShowInterface(TRUE);
        betting_list->GetMapPortalFromControl(kContinentEP1MapNumber, control_position);
    }
}
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void ContinentMapDlg::RenderDominationProgressMark(CControlWZ* control, DOMINATION_GUILD_INFO* guild_info)
{
    if (control == NULL || guild_info == NULL)
    {
        return;
    }

    eRADAR_POINT_COLOR radar_point_color = eRADAR_POINT_COUNT;

    switch (guild_info->progress_state_)
    {
    case DOMINATION_PROGRESS_STATE_NONE:
    case DOMINATION_PROGRESS_STATE_MAX:
        {
            return;
        }
        break;
    case DOMINATION_PROGRESS_STATE_READY:
        {
            radar_point_color = eRADAR_NPC_DOMINATION_YELLOW;

        }
        break;
    case DOMINATION_PROGRESS_STATE_START:
        {
            radar_point_color = eRADAR_NPC_DOMINATION_GREEN;
        }
        break;
    case DOMINATION_PROGRESS_STATE_BREAK_GATE:
    case DOMINATION_PROGRESS_STATE_BREAK_TEMPLEOFBLOOD:
        {
            radar_point_color = eRADAR_NPC_DOMINATION_ORANGE;
        }
        break;
    case DOMINATION_PROGRESS_STATE_BREAK_CRYSTALIZEDBLOOD:
        {
            radar_point_color = eRADAR_NPC_DOMINATION_RED;
        }
        break;
    case DOMINATION_PROGRESS_STATE_END:
        {
            radar_point_color = eRADAR_NPC_DOMINATION_END;
        }
        break;
    default:
        {
            return;
        }
        break;
    }

    RECT* image_rect = RadarInfoParser::Instance()->GetRadarImgInfo(radar_point_color);

    //점령 길드 마크 출력
    if (image_rect != NULL)
    {
        ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();
        RECT rtSize = control->GetSizeRect();
        float fWidth = (float) rtSize.right - rtSize.left;
        float fHeight = (float) rtSize.bottom - rtSize.top;
        g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

        const int size_delta = 11;
        BYTE image_alpha = 255;

        //완료는 깜빡이지 않고, 활성화 이미지를 보여줌
        if (guild_info->progress_state_ != DOMINATION_PROGRESS_STATE_END)
        {
            DWORD current_tick = clock_function::GetTickCount();
            const DWORD half_freq = 1000;
            const DWORD stop_time = 800;


            current_tick %= (half_freq*2 + stop_time);

            if (current_tick < half_freq)
            {
                image_alpha = static_cast<BYTE>(255 * (current_tick / static_cast<float>(half_freq)));
            }
            else if (current_tick < half_freq + stop_time)
            {
                image_alpha = 255;
            }
            else
            {
                image_alpha = static_cast<BYTE>(255 * (1 - ((current_tick - half_freq - stop_time) / static_cast<float>(half_freq))));
            }
        }

        WzColor Color[4] = {
            WzColor_RGBA(255,255,255,image_alpha),
            WzColor_RGBA(255,255,255,image_alpha),
            WzColor_RGBA(255,255,255,image_alpha),
            WzColor_RGBA(255,255,255,image_alpha)
        };

        g_pSunRenderer->RenderTexture(
            RadarInfoParser::Instance()->GetTexture(),
            (float)rtSize.left - size_delta,
            (float)rtSize.top - size_delta,
            fWidth + size_delta + size_delta,
            fHeight + size_delta + size_delta,
            (float)image_rect->left,
            (float)image_rect->top,
            (float)image_rect->right,
            (float)image_rect->bottom,
            Color);

        // 마크 위에 활성화 이미지가 뜨도록 이 자리에 배치
        if (guild_info->progress_state_ == DOMINATION_PROGRESS_STATE_END)
        {
            //완료는 깜빡이지 않고, 활성화 이미지를 보여줌
            g_InterfaceManager.RenderActivateMark(rtSize.left - 20, rtSize.top - 20); // 20은 수동으로 사이즈를 맞춤
        }

        g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
//////////////////////////////////////////////////////////////////////////
