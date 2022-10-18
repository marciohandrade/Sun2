#include "SunClientPrecompiledHeader.h"
#include "uicharacterstatusman.h"
#include "interfacemanager.h"
#include "Char_SkillDialog.h"
#include "Hero.h"
#include "uiCharaterAccumulatePointDialog/uiCharaterAccumulatePointDialog.h"
#include "GlobalFunc.h"
#include "SceneBase.h"
#include "ItemManager.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatus/uiCharacterStatusMain.h"
#include "uiCharacterStatus/uiCharacterStatusInfo.h"
#include "uiCharacterStatus/uiCharacterStatusSockecInfo.h"
#include "uiCharacterStatus/uiCharacterStatusTitleList.h"
#include "uiCharacterStatus/uiCharacterStatusHonorInfo.h"
#include "uiCharacterStatus/uiCharacterStatusHonorDescription.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
#include "uiCharacterStatus/uiCharacterStatusInfo2.h"
#endif // _NA_008472_20150903_IMPROVED_BALANCE

const DWORD IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG = StrToWzID("0400");
const DWORD IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG = StrToWzID("9913");

uiCharacterStatusMan::uiCharacterStatusMan(InterfaceManager *pUIMan)
:uiBaseMan(pUIMan)
{
    accumulate_point_dialog_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void	uiCharacterStatusMan::OnInitialize()
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    CreateUIDialog<uiCharacterStatusMain>(uiCharacterStatusMain::kDialog_ID, 
        "Data\\Interface\\40_1_Status_Window.iwz", this, TRUE);

    CreateUIDialog<uiCharacterStatusInfo>(uiCharacterStatusInfo::kDialog_ID, 
        "Data\\Interface\\40_2_Status_Window.iwz", this, TRUE);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	CreateUIDialog<uiCharacterStatusInfo2>(uiCharacterStatusInfo2::kDialog_ID, 
		"Data\\Interface\\40_2_Status_Window_01.iwz", this, TRUE);
#endif // _NA_008472_20150903_IMPROVED_BALANCE

    CreateUIDialog<uiCharacterStatusSockecInfo>(uiCharacterStatusSockecInfo::kDialog_ID, 
        "Data\\Interface\\40_3_Status_Window.iwz", this, TRUE);

    CreateUIDialog<uiCharacterStatusHonorInfo>(uiCharacterStatusHonorInfo::kDialog_ID, 
        "Data\\Interface\\40_4_Status_Window.iwz", this, TRUE, TRUE);

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CreateUIDialog<uiCharacterStatusCurrencyInfo>(uiCharacterStatusCurrencyInfo::kDialog_ID, 
        "Data\\Interface\\40_4_Status_Window.iwz", this, TRUE, TRUE);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

    status_dialog_tab_manager().Initialize(kTab_Max, 0, NULL);
    status_dialog_tab_manager().PushTab(kStatus_Info, GET_DIALOG(uiCharacterStatusInfo::kDialog_ID));
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	status_dialog_tab_manager().PushTab(kStatus_Info2, GET_DIALOG(uiCharacterStatusInfo2::kDialog_ID));
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    status_dialog_tab_manager().PushTab(kSokect_Info, GET_DIALOG(uiCharacterStatusSockecInfo::kDialog_ID));
    status_dialog_tab_manager().PushTab(kHonor_Info, GET_DIALOG(uiCharacterStatusHonorInfo::kDialog_ID));
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    status_dialog_tab_manager().PushTab(kCurrency_Info, GET_DIALOG(uiCharacterStatusCurrencyInfo::kDialog_ID));
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

    CreateUIDialog<uiCharacterStatusTitleList>(uiCharacterStatusTitleList::kDialog_ID, 
        "Data\\Interface\\40_5_Status_popup.iwz", this, TRUE);

    CreateUIDialog<uiCharacterStatusHonorDescription>(uiCharacterStatusHonorDescription::kDialog_ID, 
        "Data\\Interface\\40_6_List_popup.iwz", this, TRUE);
#else
    CreateUIDialog<Char_SkillDialog>(IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG, 
        "Data\\Interface\\40_Status_Window.iwz",
        this, TRUE);
#endif //_NA_003027_20111013_HONOR_SYSTEM


    accumulate_point_dialog_ = CreateUIDialog<uiCharaterAccumulatePointDialog>(IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG, 
        "Data\\Interface\\99_13_Mileage.iwz", this);
    SUN_ASSERT(accumulate_point_dialog_);

    require_point_ = 1;
}

void uiCharacterStatusMan::SEND_MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN()
{
    MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN sync;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
    SetPKBool(PKBOOL_CHARACTER_STATUS_ACCEPT_REWARD_SYN);
}

void uiCharacterStatusMan::Clear()
{
    accumulate_time_ = 0;
    accumulate_point_ = 0;
    require_point_ = 0;
}

void uiCharacterStatusMan::RECV_MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD(MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD* recv_msg)
{
    accumulate_time_ = recv_msg->m_dwAccumulateTime;
    accumulate_point_ = recv_msg->m_wAccumulatePoint;
    require_point_ = recv_msg->m_byRequirePoint;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    uiCharacterStatusMain* status_dialog = GET_CAST_DIALOG(uiCharacterStatusMain, uiCharacterStatusMain::kDialog_ID);
    
#else
    Char_SkillDialog* status_dialog = GET_CAST_DIALOG(Char_SkillDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG);
#endif //_NA_003027_20111013_HONOR_SYSTEM
    if (status_dialog != NULL)
    {
        status_dialog->InitAccumulateTimer();
    }
}

void uiCharacterStatusMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int index = PKBOOL_CHARACTER_STATUS_BASE; index < PKBOOL_CHARACTER_STATUS_MAX; ++index)
    {
        uicPacketControl packet_control;
        this->m_vPKBool.push_back(packet_control);
    }
}

void uiCharacterStatusMan::SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(bool is_first)
{
    if (SafePKBool(PKBOOL_CHARACTER_STATUS_USE_POINT_SYN))
    {
        // 포인트 갯수 체크하여 패킷전송
        if (accumulate_point_ >= require_point_)
        {
            MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN sync;
            if (is_first)
            {
                accumulate_point_dialog_->set_require_point(require_point_);
                sync.m_First = 1;
            }
            else
            {
                sync.m_First = 0;
            }
            
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
            // 패킷 팬딩 셋
            SetPKBool(PKBOOL_CHARACTER_STATUS_USE_POINT_SYN, TIMEOUT_PACKET_PENDING);
        }
        else
        {
            ProcessErrorCode(RC::RC_ACCUMULATE_POINT_HAVENOTPOINT);
        }
    }    
}

void uiCharacterStatusMan::NetworkProc( MSG_BASE * pMsg )
{
    switch (pMsg->m_byCategory)
    {
    case CG_ITEM:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK:
                {
                    UnsetPKBool(PKBOOL_CHARACTER_STATUS_USE_POINT_SYN);
                    MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK* recv_msg = (MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK*)pMsg;
                    accumulate_point_dialog_->ShowInterface(TRUE);
                    accumulate_point_dialog_->PrepareRewardItemGroup(recv_msg->m_ItemCodeArray);
                }
                break;
            case CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK:
                {
                    UnsetPKBool(PKBOOL_CHARACTER_STATUS_USE_POINT_SYN);
                    MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK* recv_msg = (MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_NAK*)pMsg;
                    ProcessErrorCode(recv_msg->m_dwErrorCode);
                }
                break;
            case CG_ITEM_USE_ACCUMULATE_POINT_ACK:
                {
                    UnsetPKBool(PKBOOL_CHARACTER_STATUS_ACCEPT_REWARD_SYN);
                    MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK* recv_msg = (MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK*)pMsg;
                    accumulate_point_dialog_->DisplayRewardItem(recv_msg);
                }
                break;
            case CG_ITEM_USE_ACCUMULATE_POINT_NAK:
                {
                    UnsetPKBool(PKBOOL_CHARACTER_STATUS_ACCEPT_REWARD_SYN);
                    MSG_CG_ITEM_USE_ACCUMULATE_POINT_NAK* recv_msg = (MSG_CG_ITEM_USE_ACCUMULATE_POINT_NAK*)pMsg;
                    ProcessErrorCode(recv_msg->m_dwErrorCode);
                }
                break;
            }
        }
    }
}

void uiCharacterStatusMan::ProcessErrorCode(DWORD error_code)
{
    int string_code = 0;
    switch (error_code)
    {    
    case RC::RC_ACCUMULATE_POINT_FAILED:
        // 70017	잠시 후 다시 시도 하십시오.
        string_code = 70017;
        break;
    case RC::RC_ACCUMULATE_POINT_INITIALIZE:
        // 해당에러코드는 처리하지 않는다.
        break;
    case RC::RC_ACCUMULATE_POINT_NOSPACEININVENTORY:
        // 6146	인벤토리에 공간이 없습니다.
        string_code = 6146;
        break;
    case RC::RC_ACCUMULATE_POINT_HAVENOTPOINT:
        // 6147	코인이 부족합니다.
        string_code = 6147;
        break;
    }

    if (string_code)
    {
        TCHAR error_message[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(string_code, error_message);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, error_message);
        
        accumulate_point_dialog_->ProcessErrorCode(error_code);
    }
}

void uiCharacterStatusMan::ShowGainAccumulatePoint(DWORD new_point)
{
    // 포인트획득 메시지 출력
    SceneBase* scene_base = GameFramework::GetCurrentScene();
    uiCharacterStatusMan* character_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
    if ((scene_base != NULL)
        && (character_status_manager != NULL))
    {
        int gain_point = new_point - character_status_manager->accumulate_point();
        if (gain_point > 0)
        {
            character_status_manager->set_accumulate_point(new_point);
            TCHAR message[INTERFACE_STRING_LENGTH + 1] = {0, };
            Snprintf(message, INTERFACE_STRING_LENGTH, g_InterfaceManager.GetInterfaceString(6148), gain_point);
            scene_base->InserCenterTextMessage(message, REWARD);
#ifdef _DEV_VER
            GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "GainAccumulatePoint : %d", gain_point);
#endif
        }        
    }
}


void uiCharacterStatusMan::ShowCharacterStatusDialog(bool is_show)
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    DWORD status_dialog_id = uiCharacterStatusMain::kDialog_ID;
#else
    DWORD status_dialog_id = IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    uiBase* status_dialog = GET_DIALOG(status_dialog_id);
    if (status_dialog != NULL)
    {
        status_dialog->ShowInterfaceWithSound((is_show == true) ? TRUE : FALSE);
    }
}

void uiCharacterStatusMan::ToggleCharacterStatusDialog()
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    DWORD status_dialog_id = uiCharacterStatusMain::kDialog_ID;
#else
    DWORD status_dialog_id = IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    uiBase* status_dialog = GET_DIALOG(status_dialog_id);
    if (status_dialog != NULL)
    {
        BOOL is_show = status_dialog->IsVisible();
        is_show = (is_show == FALSE) ? TRUE : FALSE;
        status_dialog->ShowInterfaceWithSound(is_show);
    }
}

PlayerAttributes* uiCharacterStatusMan::GetSimulationPlayerAttributes()
{
    PlayerAttributes* player_attributes = NULL;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    uiCharacterStatusInfo* status_dialog = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);
    if (status_dialog != NULL)
    {
        player_attributes = status_dialog->GetSimulationPlayerAttributes();
    }
#else
    Char_SkillDialog* status_dialog = GET_CAST_DIALOG(Char_SkillDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG);
    if (status_dialog != NULL)
    {
        player_attributes = &(status_dialog->simulation_player_attributes());
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

    return player_attributes;
}

void uiCharacterStatusMan::RefreshSimulationPlayerAttributes()
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    uiCharacterStatusInfo* status_dialog = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);
    if (status_dialog != NULL)
    {
        status_dialog->RefreshSimulationPlayerAttributes();
    }
#else
    Char_SkillDialog* status_dialog = GET_CAST_DIALOG(Char_SkillDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG);
    if (status_dialog != NULL)
    {
        status_dialog->RefreshSimulationPlayerAttributes();
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM
}

void uiCharacterStatusMan::ResetSimulationStatCount()
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    uiCharacterStatusInfo* status_dialog = GET_CAST_DIALOG(uiCharacterStatusInfo, uiCharacterStatusInfo::kDialog_ID);
    if (status_dialog != NULL)
    {
        status_dialog->ResetSimulationStatCount();
    }
#else
    Char_SkillDialog* status_dialog = GET_CAST_DIALOG(Char_SkillDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_STATUS_DLG);
    if (status_dialog != NULL)
    {
        status_dialog->reset_simulation_stat_count();
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM
    
}