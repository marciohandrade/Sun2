#include "SunClientPrecompiledHeader.h"
#include "uiradarman.h"
#include "InterfaceManager.h"
#include "RadarDialog.h"
#include "RadarPlusDialog.h"
#include "RadarFullDialog.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"
#include "MapInfoParser.h"
#include "Map.h"
#include "GlobalFunc.h"
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"

const DWORD IM_RADAR_MANAGER::RADAR_DIALOG		= StrToWzID("0260");
const DWORD IM_RADAR_MANAGER::RADAR_DIALOG_PLUS	= StrToWzID("0261");
const DWORD IM_RADAR_MANAGER::RADAR_DIALOG_FULL	= StrToWzID("0460");
const DWORD IM_RADAR_MANAGER::CONTINENT_MAP_DLG = StrToWzID("0462");
const DWORD IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG = StrToWzID("0463");

uiRadarMan::uiRadarMan(InterfaceManager *pUIMan) :
uiBaseMan(pUIMan),
m_pRadarDlg(NULL),
m_pRadarPlusDlg(NULL),
m_pRadarFullDlg(NULL)
{
	m_pContinentMapDlg = NULL;

    m_pRadarDlg		= (NULL);
    m_pRadarPlusDlg = (NULL);
    m_pRadarFullDlg = (NULL);
}

//------------------------------------------------------------------------------
/**
*/
void uiRadarMan::OnInitialize()
{
    radar_point_collecter().Initialize();


	m_pRadarDlg = CreateUIDialog<RadarDialog>(
					IM_RADAR_MANAGER::RADAR_DIALOG, 
					"Data\\Interface\\26_Radar.iwz", 
					this, TRUE);

	m_pRadarPlusDlg = CreateUIDialog<RadarPlusDialog>(
					IM_RADAR_MANAGER::RADAR_DIALOG_PLUS, 
					"Data\\Interface\\26_1_Radar.iwz", 
					this, TRUE);

	m_pRadarFullDlg = CreateUIDialog<RadarFullDialog>(
					IM_RADAR_MANAGER::RADAR_DIALOG_FULL, 
					"Data\\Interface\\46_Full_Map.iwz", 
					this, TRUE);

	m_pContinentMapDlg = CreateUIDialog<ContinentMapDlg>(
							IM_RADAR_MANAGER::CONTINENT_MAP_DLG,
							"Data\\Interface\\46_Full_Map2.iwz",
							this, TRUE);

    radar_full_ep2_map_dialog_ = CreateUIDialog<ContinentEp2MapDlg>(
        IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG, 
        "Data\\Interface\\46_3_Full_Map_EP2.iwz", 
        this, TRUE);
}

void uiRadarMan::Process(DWORD dwTick)
{
    int display_time = radar_click_animation().display_time();
    display_time -= dwTick;
    display_time = max(display_time, 0);
    radar_click_animation().set_display_time(display_time);
}

void uiRadarMan::SetMapName(TCHAR* map_name)
{
    if (!map_name)
    {
        return;
    }

    CCtrlStaticWZ* control_static = NULL;
    if (m_pRadarDlg)
    {
        control_static = static_cast<CCtrlStaticWZ*>(m_pRadarDlg->getControl(RadarDialog::RADAR_MAP_NAME));
        if (control_static)
        {
            m_pRadarDlg->SetMapName(control_static, map_name);
        }
    }
    if (m_pRadarPlusDlg)
    {
        control_static = static_cast<CCtrlStaticWZ*>(m_pRadarPlusDlg->getControl(RadarPlusDialog::RADAR_PLUS_MAP_NAME));
        if (control_static)
        {
            m_pRadarPlusDlg->SetMapName(control_static, map_name);
        }
    }
    if (m_pRadarFullDlg)
    {
        control_static = static_cast<CCtrlStaticWZ*>(m_pRadarFullDlg->getControl(RadarFullDialog::RADAR_FULL_MAP_NAME));
        if (control_static)
        {
            m_pRadarFullDlg->SetMapName(control_static, map_name);
        }
    }
}

void uiRadarMan::ToggleShowContinentMap()
{
    if ((m_pContinentMapDlg == NULL) || 
        (radar_full_ep2_map_dialog_ == NULL))
    {
        return;
    }

    if (m_pContinentMapDlg->IsVisible())  // 브라키온 대륙맵이 열려있을때
    {
        m_pContinentMapDlg->ToggleShowDialog();
    }
    else if (radar_full_ep2_map_dialog_->IsVisible()) // 세리엔트 대륙맵이 열려 있을때
    {
        radar_full_ep2_map_dialog_->ToggleShowDialog();
    }
    else // 두 대륙 맵에 닫혀 있을때
    {   // 위치 기반으로 열기
        int continent_number = 1;
        if(g_pHero && g_pMap)
        {
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
            if(map_info)
            {
                continent_number = map_info->ContienetNumber;
            }
        }

        if (continent_number == 1)  // 브라키온:1
        {
            m_pContinentMapDlg->ToggleShowDialog();
        }
        else// (continent_number == 2) // 세리엔트:2
        {
            radar_full_ep2_map_dialog_->ToggleShowDialog();
        }
    }
}

ContinentMapNumber uiRadarMan::GetCurrentContinentMapNumber()
{
    if ((m_pContinentMapDlg != NULL) && 
        (m_pContinentMapDlg->IsVisible() == TRUE))
    {
        return m_pContinentMapDlg->GetContinentMapNumber();
    }
    else if ((radar_full_ep2_map_dialog_ != NULL) && 
             (radar_full_ep2_map_dialog_->IsVisible() == TRUE))
    {
        return radar_full_ep2_map_dialog_->GetContinentMapNumber();
    }
    return kContinentNone;
}

uiBase* uiRadarMan::GetCurrentContinentMap()
{
    ContinentMapNumber continent_map_number = GetCurrentContinentMapNumber();
    if (continent_map_number == kContinentEP1MapNumber)
    {
        return m_pContinentMapDlg;
    }
    else if (continent_map_number == kContinentEP2MapNumber)
    {
        return radar_full_ep2_map_dialog_;
    }

    return NULL;
}

ContinentMapMode uiRadarMan::GetCurrentContinentMapMode()
{
    ContinentMapNumber continent_map_number = GetCurrentContinentMapNumber();
    if (continent_map_number == kContinentEP1MapNumber)
    {
        return m_pContinentMapDlg->continent_map_mode();
    }
    else if (continent_map_number == kContinentEP2MapNumber)
    {
        return radar_full_ep2_map_dialog_->continent_map_mode();
    }

    return kContinentNormalMode;
}

int uiRadarMan::GetCurrnetContinentMapStartPoint()
{
    if ((m_pContinentMapDlg != NULL) && 
        (m_pContinentMapDlg->IsVisible() == TRUE))
    {
        return m_pContinentMapDlg->GetContinentMapStartPoint();
    }
    else if ((radar_full_ep2_map_dialog_ != NULL) && 
        (radar_full_ep2_map_dialog_->IsVisible() == TRUE))
    {
        return radar_full_ep2_map_dialog_->GetContinentMapStartPoint();
    }
    
    return 0;
}

int uiRadarMan::GetCurrnetContinentMapStartEnd()
{
    if ((m_pContinentMapDlg != NULL) && 
        (m_pContinentMapDlg->IsVisible() == TRUE))
    {
        return m_pContinentMapDlg->GetContinentMapStartEnd();
    }
    else if ((radar_full_ep2_map_dialog_ != NULL) && 
        (radar_full_ep2_map_dialog_->IsVisible() == TRUE))
    {
        return radar_full_ep2_map_dialog_->GetContinentMapStartEnd();
    }

    return 0;
}

CControlWZ* uiRadarMan::GetCurrentContinentMapPointControl(int continent_code)
{
    if ((m_pContinentMapDlg != NULL) && 
        (m_pContinentMapDlg->IsVisible() == TRUE))
    {
        return m_pContinentMapDlg->GetContinentMapPointControl(continent_code);
    }
    else if ((radar_full_ep2_map_dialog_ != NULL) && 
        (radar_full_ep2_map_dialog_->IsVisible() == TRUE))
    {
        return radar_full_ep2_map_dialog_->GetContinentMapPointControl(continent_code);
    }

    return NULL;
}

void uiRadarMan::ChangeRadarMap(TCHAR* file_name /*= NULL*/)
{
    if (g_pMap == NULL)
    {
        return;
    }

    LoadMapScript(file_name);
    
    if (m_pRadarDlg != NULL)
    {
        m_pRadarDlg->ChangeMap(file_name);
    }
    if (m_pRadarPlusDlg != NULL)
    {
        m_pRadarPlusDlg->ChangeMap(file_name);
    }
    if (m_pRadarFullDlg != NULL)
    {
        m_pRadarFullDlg->ChangeMap(file_name);
    }
}

void uiRadarMan::LoadMapScript(TCHAR* file_name)
{
    DWORD filed_code = g_pMap->GetCurMMCode();
    if (filed_code == 0)
    {
        return;
    }

    util::SolarFileReader file_reader;
    BOOL bResult = FALSE;

    char* path_name = "Data\\Interface\\";
    char full_path_name[MAX_PATH] = {'\0', };

#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    if (file_name != NULL)
    {
         _snprintf(full_path_name, MAX_PATH - 1, "%s%s.txt", path_name, file_name);

        if (GENERALPARAM->IsUsePackFile() == TRUE)
        {
            bResult = file_reader.OpenFileFromPack("Data\\Resource.wpk", full_path_name, SEPERATOR_QUOTATIONMARKFORSTR);
        }
        else
        {
            bResult = file_reader.OpenFile(full_path_name, SEPERATOR_QUOTATIONMARKFORSTR );
        }
    }

    if (bResult == FALSE)
    {
        _snprintf(full_path_name, MAX_PATH - 1, "%smap_%05d.txt", path_name, filed_code);

        if (GENERALPARAM->IsUsePackFile() == TRUE)
        {
            bResult = file_reader.OpenFileFromPack("Data\\Resource.wpk", full_path_name, SEPERATOR_QUOTATIONMARKFORSTR);
        }
        else
        {
            bResult = file_reader.OpenFile(full_path_name, SEPERATOR_QUOTATIONMARKFORSTR );
        }
    }
#else
    _snprintf(full_path_name, MAX_PATH - 1, "%smap_%05d.txt", path_name, filed_code);

    if (GENERALPARAM->IsUsePackFile() == TRUE)
    {
        bResult = file_reader.OpenFileFromPack("Data\\Resource.wpk", full_path_name, SEPERATOR_QUOTATIONMARKFORSTR);
    }
    else
    {
        bResult = file_reader.OpenFile(full_path_name, SEPERATOR_QUOTATIONMARKFORSTR );
    }
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    
    radar_map_data().Clear();

    if (bResult == TRUE)
    {
        // 맵 이미지의 크기
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        radar_map_data().set_map_image_width(static_cast<float>(file_reader.GetTokenNumber()));
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        radar_map_data().set_map_image_height(static_cast<float>(file_reader.GetTokenNumber()));

        // Matrix 읽어옴
        WzMatrix& transform_matrix = radar_map_data().transform_matrix();
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._11 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._12 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._13 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._14 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._21	= static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._22	= static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._23 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._24 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._31	= static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._32 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._33 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._34 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._41 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._42 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._43 = static_cast<float>(file_reader.GetTokenNumber());
        file_reader.GetNextTokenType( TOKEN_NUMBER );
        transform_matrix._44 = static_cast<float>(file_reader.GetTokenNumber());
    }
    else
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "Failed open mapscript(%s)", full_path_name);
#endif //_DEV_VER        
    }

    file_reader.CloseFile();
}

void uiRadarMan::SetHeroPosition(WzVector hero_position)
{
    // 좌표 계산 및 포인트 수집
    radar_map_data().CalculateTransformPosition(hero_position);

    radar_point_collecter().CollectRadarPoint();
}

void uiRadarMan::SetClickPosition(float position_x, float position_y, int display_time)
{
    radar_click_animation().SetClickPosition(position_x, position_y, display_time);
}