#include "SunClientPrecompiledHeader.h"
#include "AskAirShipPortal.h"
#include "GameConst_Color.h"
#include "MapInfoParser.h"
#include "Map.h"
#include "InterfaceManager.h"
#include "uiAssistMan/uiAssistMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ObjectManager.h"
#include "Hero.h"

AskAirShipPortal::AskAirShipPortal(InterfaceManager* ui_man)
:AskDialog(ui_man)
{
    map_code_ = 0;
    air_ship_target_object_key_ = 0;
}

AskAirShipPortal::~AskAirShipPortal()
{

}

void AskAirShipPortal::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
        this->DoIt();
        SetStatus(ASK_STATUS_NORMAL);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        this->Cancel();
        SetStatus(ASK_STATUS_NORMAL);
    }

    if (air_ship_target_object_key_ != 0)
    {
        MapObject* object_ptr = static_cast<MapObject*>(g_ObjectManager.GetObject(air_ship_target_object_key_));
        if(object_ptr && g_pHero)
        {
            WzVector diff = object_ptr->GetPosition() - g_pHero->GetVisiblePos();
            float distance = VectorLength2( &diff );
            if ( distance > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
            {
                map_code_ = 0;
                ShowInterface(FALSE);
            }
        }
    }
}
void AskAirShipPortal::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
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

void AskAirShipPortal::OnShowWindow( BOOL val )
{
    if (val)
    {
        map_code_ = 0;
        MoveCenter();
        SetTexts();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_AirShip_Comfirm;
        msg.wParam=IM_ASSIST_MANAGER::AIRSHIP_PORTAL_DIALOG;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        air_ship_target_object_key_ = 0;
        g_KeyQueueManager.DeleteMsg(eDoType_AirShip_Comfirm);
    }
}

void AskAirShipPortal::SetTexts()
{
    // 11477 확장지역으로 이동하시겠습니까?
    // 11473 상공에 있는 비공정에 탑승 할 수 있습니다.\n이동 하겠습니까?
    DWORD string_code  = airship_zone() ? (11477):(11473);
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
    g_InterfaceManager.GetInterfaceString(string_code, message);
    InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}

void AskAirShipPortal::DoIt()
{
    MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
    sync.m_wIndex = portal_index();
    
    const sMAP_ENTERENCE_PORTAL* const portal_info = MapInfoParser::Instance()->GetMapPortal(sync.m_wIndex);
    if (portal_info)
    {
        map_code_ = portal_info->wFieldTo;
    }    
    
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
    ShowInterface(FALSE);
}
void AskAirShipPortal::Cancel()
{
    map_code_ = 0;
    ShowInterface(FALSE);
}

bool AskAirShipPortal::AirshipErrorMessage(BYTE error_code)
{    
    const sMAPINFO* const map_info = MapInfoParser::Instance()->FindMapInfo(map_code_);
    if (map_info && map_info->byMKind == eZONETYPE_AIRSHIP_FIELD && error_code == RC::RC_ROOM_INVALID_LIMITLEVEL)
    {
        TCHAR   szMessage[INTERFACE_STRING_LENGTH];
        //11472	이동 가능한 레벨이 아닙니다.
        g_InterfaceManager.GetInterfaceString(11472, szMessage, INTERFACE_STRING_LENGTH);
        uiSystemMan* pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
        if (pSystemMan)
        {
            pSystemMan->Sys_Verify(szMessage);
        }
        map_code_ = 0;
        return true;
    }
    return false;
}

void AskAirShipPortal::set_air_ship_target_object_key_( const DWORD& air_ship_target_object_key )
{
    air_ship_target_object_key_ = air_ship_target_object_key;
}