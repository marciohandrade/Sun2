#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"
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
#include "uiDominationBettingList/uiDominationBettingList.h"
#include "DominationInfoParser.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"

const DWORD kPerFrameTickCount = 30;

//////////////////////////////////////////////////////////////////////////

WzID ContinentEp2MapDlg::m_wzID[eCONTINENT_MAP_POS_MAX] = 
{                                                       // �� �̸�                      // wolrd.txt �� ContinentCode
        StrToWzID("B005"),//eCMDOMINATION_DIALOG_PIC    // ���׸� �Ȱ��� ��           1   
        StrToWzID("BT01"),//eCMDOMINATION_DIALOG_PIC    // hidden 01                    2
        StrToWzID("B011"),//eCMDOMINATION_DIALOG_PIC    // �õ� ��������                3
        StrToWzID("B001"),//eCMDOMINATION_DIALOG_PIC    // �ϴ� ���� ���               4
        StrToWzID("B008"),//eCMDOMINATION_DIALOG_PIC    // �뽺ų���� �߿���            5

        StrToWzID("B007"),//eCMDOMINATION_DIALOG_PIC    // hidden 02                    6
        StrToWzID("B014"),//eCMDOMINATION_DIALOG_PIC    // ������ ��� ���ƽý�         7
        StrToWzID("B009"),//eCMDOMINATION_DIALOG_PIC    // �ڸ���ũ �ؾȼ�              8
        StrToWzID("B010"),//eCMDOMINATION_DIALOG_PIC    // ������� ����                9
        StrToWzID("B020"),//eCMDOMINATION_DIALOG_PIC    // ��������                     10

        StrToWzID("BT00"),//eCMDOMINATION_DIALOG_PIC    // õ���� ��                    11
        StrToWzID("B012"),//eCMDOMINATION_DIALOG_PIC    // �����ڸ� ���ѱ�              12
        StrToWzID("B013"),//eCMDOMINATION_DIALOG_PIC    // ��ȯ�� �縷                  13
        StrToWzID("B015"),//eCMDOMINATION_DIALOG_PIC    // ������� �Ƚ�ó              14
        StrToWzID("B016"),//eCMDOMINATION_DIALOG_PIC    // �������� ����                15

        StrToWzID("B017"),//eCMDOMINATION_DIALOG_PIC    // ���Į�� ����              16
        StrToWzID("B018"),//eCMDOMINATION_DIALOG_PIC    // �ڸ���ũ                     17
        StrToWzID("B019"),//eCMDOMINATION_DIALOG_PIC    // ��䷹�� ����              18
        StrToWzID("B006"),//eCMDOMINATION_DIALOG_PIC    // �Ҿ���� ��ȥ                19
        
        
        


        StrToWzID("PI02"),//eCMDOMINATION_DIALOG_PIC
        StrToWzID("BT99"),//eCMDOMINATION_GUILD_MARK_BTN
        StrToWzID("ST00"),//eCMDOMINATION_GUILD_NAME_TEXT
        StrToWzID("ST01"),//eCMDOMINATION_GUILD_MASTER_TEXT
        
        StrToWzID("B999"), //eCONTINENT_MAP_POS_EXIT_BTN = 31,
        StrToWzID("S005"),   // ���� �˾� ���õ� ��
        StrToWzID("L001"),  // ���� �˾� ����Ʈ
        StrToWzID("B000"),  // ���� �˾� ȭ��ǥ
        StrToWzID("C000"),  // �ű� ��� ��ü ����
        StrToWzID("C001"),  // �ű� ��� ����1
        StrToWzID("C002"),  // �ű� ��� ����2
        StrToWzID("C003"),  // �ű� ��� ����3

		//eCONTINENT_MAP_POS_MAX = 32,
};

ContinentEp2MapDlg::ContinentEp2MapDlg(InterfaceManager * pUIMan) : uiBase(pUIMan)
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

ContinentEp2MapDlg::~ContinentEp2MapDlg()
{
	Release();
}

void ContinentEp2MapDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();

    LoadSelectAreaImage();
}

//////////////////////////////////////////////////////////////////////////

void ContinentEp2MapDlg::Release()
{
}

//////////////////////////////////////////////////////////////////////////

void ContinentEp2MapDlg::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

void ContinentEp2MapDlg::SetAirShipMode()
{
    set_continent_map_mode(kContinentAirShipMode);

    ZeroMemory(airship_mapcode, sizeof(airship_mapcode));

    sMAPPORTAL_HASH* portal_hash =  MapInfoParser::Instance()->GetMapPortalHashTable();
    sMAPPORTAL_HASH::const_iterator itr = portal_hash->begin();

    while (itr != portal_hash->end())
    {
        sMAP_ENTERENCE_PORTAL* portal_info = itr->second; //(*itr);

        // npc_id���� ��ȿ�ϰ� ���� �ʵ�͵����ϴٸ�
        if (portal_info &&
            (portal_info->npc_id_ != 0) &&
            (portal_info->wWorldFrom == g_pMap->GetMapID()))
        {
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(portal_info->wWorldTo);

            //����ʿ��� �̵��������� Ȱ��ȭ
            if (map_info &&
                (map_info->wContinentCode > 0) &&
                (map_info->wContinentCode <= eCONTINENT_MAP_POS_POINT_BTN_CNT) &&
                (map_info->ContienetNumber == kContinentEP2MapNumber))
            {
                airship_mapcode[map_info->wContinentCode - 1] = portal_info->wIndex; 
            }
        }
        ++itr;
    }
}
void ContinentEp2MapDlg::RenderAirShipPoint()
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

        // ��ư �߽���
        int center_x = rect_button.left + (rect_button.right - rect_button.left)/2;
        int center_y = rect_button.top + (rect_button.bottom - rect_button.top)/2;

        // ��ư�߽������κ��� ���༱ũ�⿡ �°� ������
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

void ContinentEp2MapDlg::RenderAirShipTooltip(WORD portal_index)
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

    //11470	%s \n%d LV %d ����(Ŭ�� �� �̵�)
    g_InterfaceManager.GetInterfaceString(11470, message);

    // �̵��� ������
    g_InterfaceManager.GetInterfaceString(world_to->dwNCode, map_name);

    Snprintf(tooltip_message,
        INTERFACE_STRING_LENGTH,
        message,                  
        map_name,                  // �̵��� ������
        portal_info->byMin_Level,  // �̵� ���ɷ���
        portal_info->dwHeim);      // �̵����    

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
void ContinentEp2MapDlg::OnRenderSolarDialog()
{
	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();
    DOMINATION_GUILD_INFO* area_over_guild_info = NULL;
    POSTYPE over_button_pos = 0xFF;
    bool is_area_over = false;

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

                if (pMapInfo->ContienetNumber != kContinentEP2MapNumber) // �ű� ��� EP2
                {
                    continue;
                }

                if(NULL == m_pPointBtn[pMapInfo->wContinentCode -1])
                    continue;

                uiDominationTowerMan::RenderGuildMark(m_pPointBtn[pMapInfo->wContinentCode -1], pGuildInfo, 255);

                // mouseover ���� �� ������ ���
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

    }

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ui_radar_manager->RenderNavigationPoint();
    }

	RECT* pRect = NULL;
	
	if(g_pHero && g_pMap)
	{
		const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
		if(pMapInfo)
		{
            if (pMapInfo->ContienetNumber != kContinentEP2MapNumber) // �ű� ����� �ƴϸ�
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

void ContinentEp2MapDlg::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ContinentEp2MapDlg::OnShowWindow(BOOL val)
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
            popup_select_text_->SetTextID(51011);   // 51011: ������Ʈ ���
            popup_list_->SetCurSel(1);// ������Ʈ ���
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

void ContinentEp2MapDlg::ClickAIrshipPortal(POSTYPE pos)
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

void ContinentEp2MapDlg::MessageProc(SI_MESSAGE * pMessage)
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
            else if(control_pos >= eCONTINENT_MAP_POS_AREA1 && control_pos <= eCONTINENT_MAP_POS_AREA3)
            {
                // ����� ����
                ContinentMapDlg* continent_map_dialog = 
                    GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
                if (continent_map_dialog != NULL)
                {                    
                    continent_map_dialog->ChangeContinentCode(continent_map_mode_);
                    this->ShowInterface(FALSE);
                    continent_map_dialog->ShowWindow(WZ_SHOW);
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
                        // ����� ����
                        ContinentMapDlg* continent_map_dialog = 
                            GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
                        if (continent_map_dialog != NULL)
                        {
                            continent_map_dialog->ChangeContinentCode(continent_map_mode_);
                            this->ShowInterface(FALSE);
                            continent_map_dialog->ShowWindow(WZ_SHOW);
                        }
                    }
                    else if (iSelect == 1)
                    {
                        popup_list_->ShowWindowWZ(WZ_HIDE);
                    }
                }
            }
            else
            {
                if (continent_map_mode() == kContinentNormalMode)
                {
                    break;
                }

                // ���������� ��Ʈ�� ó��
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

void ContinentEp2MapDlg::InitControl()
{
	exit_button_ = static_cast<CCtrlButtonWZ*>(getControl(eCONTINENT_MAP_POS_EXIT_BTN));
	assert(exit_button_);

    CCtrlStaticWZ* temp_control_static;

    temp_control_static = static_cast<CCtrlStaticWZ*>(getControl(eCMDOMINATION_GUILD_NAME_TEXT));
    assert(temp_control_static);
    if (temp_control_static)
    {
        temp_control_static->ShowWindowWZ(WZ_HIDE);
    }

    temp_control_static = static_cast<CCtrlStaticWZ*>(getControl(eCMDOMINATION_GUILD_MASTER_TEXT));
    assert(temp_control_static);
    if (temp_control_static)
    {
        temp_control_static->ShowWindowWZ(WZ_HIDE);
    }

    for(int i = 0; i < eCONTINENT_MAP_POS_POINT_BTN_CNT; ++i)
    {
        m_pPointBtn[i] = static_cast<CCtrlButtonWZ*>(getControl(eCONTINENT_MAP_POS_POINT_BTN_START + i));
        assert(m_pPointBtn[i]);
    }

    m_pGuildDialogPic = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzID[eCMDOMINATION_DIALOG_PIC]));
    assert(m_pGuildDialogPic);
    m_pGuildMarkBtn = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzID[eCMDOMINATION_GUILD_MARK_BTN]));
    assert(m_pGuildMarkBtn);
    m_pGuildNameText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzID[eCMDOMINATION_GUILD_NAME_TEXT]));
    assert(m_pGuildNameText);
    m_pGuildMasterText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzID[eCMDOMINATION_GUILD_MASTER_TEXT]));
    assert(m_pGuildMasterText);

    popup_select_text_ = static_cast<CCtrlStaticWZ*>(getControl(eCONTINENT_MAP_POS_POPUP_SELECT_TEXT));
    assert(popup_select_text_);
    popup_list_ = static_cast<CCtrlListWZ*>(getControl(eCONTINENT_MAP_POS_POPUP_LIST));
    assert(popup_list_);
    if (popup_list_ != NULL)
    {
        popup_list_->ShowWindowWZ(WZ_HIDE);
        is_popup_list_show_ = false;

        int	iSellHeight = popup_list_->GetSellheight();
        RECT rc = popup_list_->GetSizeRect();
        int index;

        DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
        DWORD c_dwSelTextColor = RGBA(0,220,0,255);
        DWORD c_dwBackColor = RGBA(0,0,0,0);

        TCHAR string[INTERFACE_STRING_LENGTH] = {0};
        index = popup_list_->InsertItemBack();
        g_InterfaceManager.GetInterfaceString(51010, string);// 51010: ���Ű�� ���
        popup_list_->SetItemFontID(0, 0, StrToWzID("mn12") );
        GlobalFunc::SetItemText(popup_list_, index, string);
        GlobalFunc::SetItem(popup_list_, index, 0, string, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
            c_dwBackColor, c_dwTextColor, c_dwSelTextColor);

        index = popup_list_->InsertItemBack();
        g_InterfaceManager.GetInterfaceString(51011, string);// 51011: ������Ʈ ���
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
}

void ContinentEp2MapDlg::ChangeContinentCode(int continent_map_mode)
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
//////////////////////////////////////////////////////////////////////////
void ContinentEp2MapDlg::ClickAreaButton(POSTYPE area_button_pos)
{
    CControlWZ* area_button = getControl(area_button_pos);
    if (area_button == NULL)
    {
        return;
    }

    // ��� �������� ����
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
            
            if (pMapInfo->ContienetNumber != kContinentEP2MapNumber)
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
                // ��Ʈ ������ ��쿡�� ������ ���´�.
                if (domination_info->m_DominationType == DOMINATION_TYPE_MONDSCHIEN)
                    continue;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
                // �ش� ������ ���ؼ� ��û ��Ŷ ����
                uiDominationTowerMan* domination_manager = 
                    GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
                if (domination_manager)
                {
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
                    // naklu_check // �����2 ���� (������ ���������� ��û�ϱ⿡ �ݴ°� ����. �Ʒ��� NOTICE_SYN ����.)
#else
                    uiBase* betting_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
                    if (betting_list != NULL)
                    {
                        betting_list->ShowInterface(FALSE);
                    }
                    set_continent_map_mode(kContinentDominationManagerSelectedAreaMode);
                    // ���õ� ��Ʈ�� ������ ����
                    area_info_.area_pos = area_button_pos;
                    // ���õ� ��ư��Ʈ�� ����
                    area_info_.area_button = static_cast<CCtrlButtonWZ*>(area_button);
                    // ������ ���ڵ� ����
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

void ContinentEp2MapDlg::LoadSelectAreaImage()
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    CManagerTexture* texture_manager = g_pSunRenderer->x_pManagerTexture;

    area_select_image_ = 
        texture_manager->LoadTexture("Data\\Interface\\all_butten_new2.tga", 
        TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif
}

void ContinentEp2MapDlg::RenderSelectAreaImage(CCtrlButtonWZ* area_button_control, bool is_select)
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if (area_button_control == NULL || area_select_image_ == INVALID_HANDLE_VALUE)
    {
        return;
    }
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
void ContinentEp2MapDlg::RenderSplineImageAlphaWarp( DWORD rendertick,float x, float y )
{
    //���� ���󿡼� ���� �������� 
    WzColor color[4];

    //�������� ���� g(t) = y0 * (1.0f - t) + y1*t;

    //1���� ������ ������.
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

void ContinentEp2MapDlg::GetMapPortalFromControl(int control_position)
{
    uiDominationBettingList* betting_list = GET_CAST_DIALOG(uiDominationBettingList, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
    if (betting_list != NULL)
    {
        betting_list->ShowInterface(TRUE);
        betting_list->GetMapPortalFromControl(kContinentEP2MapNumber, control_position);
    }
}
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL
//////////////////////////////////////////////////////////////////////////
