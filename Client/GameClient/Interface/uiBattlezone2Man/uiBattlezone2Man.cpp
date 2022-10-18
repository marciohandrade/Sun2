#include "SunClientPrecompiledHeader.h"

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#include "uiBattlezone2Reservation/uiBattlezone2Reservation.h"
#include "uiBattlezone2Password/uiBattlezone2Password.h"
#include "uiBattlezone2Icon/uiBattlezone2Icon.h"
#include "map.h"
#include "BattlezoneMissionTreeParser.h"
#include "hero.h"
#include "BattleScene.h"
#include "GlobalFunc.h"
#include "uiPartyMan/uiPartyMan.h"
#include "ObjectManager.h"
#include "uiRadarMan/uiRadarMan.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiMenuMan/uiMenuMan.h"
#include "MainDialog2.h"
#endif

const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST = StrToWzID("0612");
const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE = StrToWzID("0613");
const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY = StrToWzID("0614");
const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_ICON = StrToWzID("0615");
const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION = StrToWzID("0616");
const DWORD IM_BATTLEZONE2_MANAGER::BATTLEZONE2_PASSWORD = g_IDGenerator.GenerateID();

uiBattlezone2Man::uiBattlezone2Man(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    ui_battlezone2_list_ = NULL;
    ui_battlezone2_tree_ = NULL;
    ui_battlezone2_lobby_ = NULL;
    ui_battlezone2_reservation_ = NULL;

    battlezone_room_state_ = ROOM_STATE_LIST;
    master_player_key_ = 0;
    is_wait_ = false;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattlezone2Man::StateClear()
{
    //생성방, 예약 방등 체크 끄기 및 초기화
    if (ui_battlezone2_tree_)
    {
        ui_battlezone2_tree_->Clear();
    }

    if (ui_battlezone2_lobby_)
    {
        ui_battlezone2_lobby_->Clear();
    }

    if (ui_battlezone2_reservation_)
    {
        ui_battlezone2_reservation_->Clear();
    }

    if (ui_battlezone2_list_)
    {
        ui_battlezone2_list_->Clear();
    }
}

void uiBattlezone2Man::Clear()
{
    battlezone_room_state_ = ROOM_STATE_LIST;
    master_player_key_ = 0;
    is_wait_ = false;

    StateClear();
    FlushUserSlotSet();
    InitPKBooleans();
}

void uiBattlezone2Man::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_BATTLEZONE_BASE; i<ePKBOOL_BATTLEZONE_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattlezone2Man::OnInitialize()
{
    ui_battlezone2_list_ = CreateUIDialog<uiBattlezone2List>(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST,
        "Data\\Interface\\61_2_BattleZone_List_new.iwz", this);
    assert(ui_battlezone2_list_);

    ui_battlezone2_tree_ = CreateUIDialog<uiBattlezone2Tree>(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE,
        "Data\\Interface\\61_3_BattleZone_Create_room.iwz", this, TRUE);
    assert(ui_battlezone2_tree_);

    ui_battlezone2_lobby_ = CreateUIDialog<uiBattlezone2Lobby>(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY,
        "Data\\Interface\\61_4_BattleZone_ready_new.iwz", this);
    assert(ui_battlezone2_lobby_);

    ui_battlezone2_reservation_ = CreateUIDialog<uiBattlezone2Reservation>(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION,
        "Data\\Interface\\61_6_BattleZone_Reservation.iwz", this);
    assert(ui_battlezone2_reservation_);

#ifdef _NA_000000_20130114_RENEWER_UI
#else
    ui_battlezone2_icon_ = CreateUIDialog<uiBattlezone2Icon>(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_ICON,
        "Data\\Interface\\61_5_BattleZone_Button.iwz", this, TRUE);
    assert(ui_battlezone2_icon_);
#endif
    ui_battlezone2_password_ = CreateUIDialog<uiBattlezone2Password>(
        IM_BATTLEZONE2_MANAGER::BATTLEZONE2_PASSWORD,
        "Data\\Interface\\62_2_Input_Password.iwz", this, FALSE, TRUE);
    assert(ui_battlezone2_password_);



    //
    battlezone_room_state_ = ROOM_STATE_LIST;
    master_player_key_ = 0;

    InitPKBooleans();
    InitializeUserSlotSet();
}

void uiBattlezone2Man::OnRelease()
{
    //Clear();
    ReleaseUserSlotSet();
}

void uiBattlezone2Man::ToggleBattlezoneDialog()
{
#ifdef _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
    if ((g_pMap->IsVill() == FALSE) && 
        (g_pMap->IsField() == FALSE &&
        g_pMap->IsTutorialField() == FALSE))
#else
    if ((g_pMap->IsVill() == FALSE) && 
        (g_pMap->IsField() == FALSE))
#endif // _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
    {
        return;
    }

    uiBattlezone2Man* ui_battlezone2_manager = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (ui_battlezone2_manager != NULL)
    {
        ui_battlezone2_manager->ToggleDialog();
    }
}

void uiBattlezone2Man::DeleteDisConnectedUser(char* character_name)
{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    for (int i = 0; i < (int)user_slot_set_.GetNumUser(); ++i)
    {
        UserSlot* user_slot = user_slot_set_.GetUserByIndex(i);
        if (user_slot == NULL)
        {
            return;
        }

        if(StrnCmp(user_slot->GetPlayerName(), character_name, MAX_CHARNAME_LENGTH) == 0)
        {
            g_ObjectManager.Delete(user_slot->GetBattleZonePlayerKey());
            user_slot->SetBattleZonePlayerKey(0);

            user_slot_set_.LeaveUser(character_name);

            return;
        }
    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

    return;
}
void uiBattlezone2Man::ToggleDialog()
{
    switch (battlezone_room_state_)
    {
    case ROOM_STATE_LIST:
        {
            if (ui_battlezone2_list_)
            {
                ui_battlezone2_list_->ShowInterfaceWithSound((!ui_battlezone2_list_->IsVisible())?TRUE:FALSE);
            }
        }
        break;           
    case ROOM_STATE_LOBBY:
        {
            if (ui_battlezone2_lobby_)
            {
                ui_battlezone2_lobby_->ShowInterfaceWithSound((!ui_battlezone2_lobby_->IsVisible())?TRUE:FALSE);
            }
        }
        break;
    case ROOM_STATE_MISSION:
        {
            //미션일때는 창을 열지 않음
            if (ui_battlezone2_list_)
            {
                ui_battlezone2_list_->ShowInterfaceWithSound(FALSE);
            }
            if (ui_battlezone2_lobby_)
            {
                ui_battlezone2_lobby_->ShowInterfaceWithSound(FALSE);
            }
            //829 미션 지역에서는 배틀존에 연결 할 수 없습니다.
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(829, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
        }
        break;
    }
}

void uiBattlezone2Man::ShowBattlezoneDialog(DWORD type, bool is_show)
{
    switch (type)
    {
    case BATTLEZONE2_DIALOG_LIST:
        {
            if (ui_battlezone2_list_)
            {
                ui_battlezone2_list_->ShowWindow(is_show?TRUE:FALSE);
            }
        }
        break;           
    case BATTLEZONE2_DIALOG_TREE:
        {
            if (ui_battlezone2_tree_)
            {
                ui_battlezone2_tree_->ShowWindow(is_show?TRUE:FALSE);
            }
        }
        break;
    case BATTLEZONE2_DIALOG_LOBBY:
        {
            if (ui_battlezone2_lobby_)
            {
                ui_battlezone2_lobby_->ShowWindow(is_show?TRUE:FALSE);
            }
        }
        break;
    case BATTLEZONE2_DIALOG_RESERVATION:
        {
            if (ui_battlezone2_reservation_)
            {
                ui_battlezone2_reservation_->ShowWindow(is_show?TRUE:FALSE);
            }
        }
        break;
#ifdef _NA_000000_20130114_RENEWER_UI
	case BATTLEZONE2_DIALOG_ICON:
		{
			MainDialog2* pMainDlg = GET_CAST_DIALOG(MainDialog2, IM_MENU_MANAGER::MAIN_DLG_2);
			if (pMainDlg)
			{
				if (battlezone_room_state_ == ROOM_STATE_MISSION)
				{
					pMainDlg->BattleZoneToDisAbleButton(FALSE);
				}
				else
				{
					pMainDlg->BattleZoneToDisAbleButton(is_show?TRUE:FALSE);
				}
			}
		}	
		break;
#else
    case BATTLEZONE2_DIALOG_ICON:
        {
            if (ui_battlezone2_icon_)
            {
                if (battlezone_room_state_ == ROOM_STATE_MISSION)
                {
                    ui_battlezone2_icon_->ShowInterface(FALSE);
                }
                else
                {
                    ui_battlezone2_icon_->ShowInterface(is_show?TRUE:FALSE);
                }
            }
        }
        break;
#endif
    }
}

void uiBattlezone2Man::Update()
{
}

bool uiBattlezone2Man::AddPlayer(UserSlot& enter_user_slot)
{
    //팀정보 세팅
    uiPartyMan* party_manager =
        static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    if (party_manager)
    {
        uicPartyMemberInfoPlus* party_info = party_manager->GetPartyMemberInfo(enter_user_slot.GetPlayerKey());
        if (party_info)
        {
            enter_user_slot.SetTeam(party_info->GetTeam());
        }
        else
        {
            assert(!"party info is null");//파티가 null일때 방이 만들어지면 안되지만 안전코드
            enter_user_slot.SetTeam(0);
        }
    }

    UserSlotSet::eRT_USERSLOT_SET result_code = this->user_slot_set_.AddUser(enter_user_slot);
    if (UserSlotSet::SUCCESS != result_code)
    {
        return false;
    }
    return true;
}

void uiBattlezone2Man::SwapUserSlot(DWORD dest_key, DWORD src_key)
{
    if (FindPlayer(dest_key) == NULL)//방장이 나간 경우
    {
        UserSlot user_slot2 = *FindPlayer(src_key);
        AddPlayer(user_slot2);

        user_slot2.Clear();
        return;
    }

    if (FindPlayer(src_key) == NULL)
    {
        return;
    }
    else
    {
        UserSlot user_slot1 = *FindPlayer(dest_key);
        UserSlot user_slot2 = *FindPlayer(src_key);

        UserSlot temp_user_slot = user_slot1;

        user_slot1.SetSlotState(UserSlot::OPENED);
        user_slot2.SetSlotState(UserSlot::OPENED);
        AddPlayer(user_slot2);
        AddPlayer(temp_user_slot);

        user_slot1.SetSlotState(UserSlot::FILL);
        temp_user_slot.SetSlotState(UserSlot::FILL);
    }  
}

UserSlot* uiBattlezone2Man::FindPlayer(DWORD object_key)
{
    return user_slot_set_.GetUserByKey(object_key);
}

bool uiBattlezone2Man::LeavePlayer(DWORD player_key)
{
    UserSlot* user_slot = FindPlayer(player_key);
    if (user_slot != NULL)
    {
        g_ObjectManager.Delete(user_slot->GetBattleZonePlayerKey());
        user_slot->SetBattleZonePlayerKey(0);
    }

    return (!!user_slot_set_.LeaveUser(player_key));	
}

void uiBattlezone2Man::NetworkProc(MSG_BASE* message_ptr)
{
    if (ui_battlezone2_list_)
    {
        ui_battlezone2_list_->NetworkProc(message_ptr);
    }
    if (ui_battlezone2_tree_)
    {
        ui_battlezone2_tree_->NetworkProc(message_ptr);
    }
    if (ui_battlezone2_lobby_)
    {
        ui_battlezone2_lobby_->NetworkProc(message_ptr);
    }
    if (ui_battlezone2_reservation_)
    {
        ui_battlezone2_reservation_->NetworkProc(message_ptr);
    }
}

int uiBattlezone2Man::GetBlockUserSlotNum()
{
    return (int)user_slot_set_.GetBlockNum();
}

int uiBattlezone2Man::GetNonBlockUserSlotNum()
{
    return (int)user_slot_set_.GetNonBlockNum();
}

UserSlot* uiBattlezone2Man::GetUserSlotByIndex(int index)
{
    UserSlot* user_slot = user_slot_set_.GetUserByIndex(index);
    return user_slot;
}

int	uiBattlezone2Man::GetMaxUserSlot()
{
    return (int)user_slot_set_.GetMaxUser();
}

int	uiBattlezone2Man::GetUserSlotNum()
{
    return (int)user_slot_set_.GetNumUser();
}

void uiBattlezone2Man::ModifyPlayerNumber(int player_numbers)
{
    assert(player_numbers <= 10);

    if (player_numbers <= 0)
    {
        assert(player_numbers > 0);
        return;
    }

    if (SafePKBool(PACKET_STATUS_SEND_PLAYER_NUM))
    {
        SetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);
        MSG_CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN send_packet;
        send_packet.max_count_ = player_numbers;
        pending_player_number_ = player_numbers;
        //m_iPendingPlayerNumBeforeRecvAck

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

#if defined	(_ZONE_TEST_LOBBY_IN_SINGLE)
        MSG_CG_ZONE_LOBBY_NUMBEROFPLAYER_ACK send_packet;
        g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, static_cast<BYTE*>(&send_packet), sizeof(send_packet));
#endif		
    }	
}
//----------------------------------------------------------------------------
/** 
*/
void
uiBattlezone2Man::InitializeUserSlotSet()
{
    user_slot_set_.Initialize(MAX_VIEW_USER_SLOT);
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattlezone2Man::ReleaseUserSlotSet()
{
    user_slot_set_.Release();
}

//----------------------------------------------------------------------------
/** 
*/
void uiBattlezone2Man::FlushUserSlotSet()
{
    //user_slot_set_.Initialize(MAX_VIEW_USER_SLOT);
    user_slot_set_.FlushUsers();
}

/*
void uiBattlezone2Man::SaveSlotObject()
{
    for (int i = 1; i < GetMaxUserSlot() ; ++i)
    {
        UserSlot* slot = GetUserSlotByIndex(i);
        if (slot)
        {
            DWORD player_key = slot->GetBattleZonePlayerKey();
            Object* object = g_ObjectManager.GetObject(player_key);
            if (object)
            {
                player_objects_[i] = *(object);
            }
        }
    }
}*/

void uiBattlezone2Man::SetReadyUserSlot(DWORD object_key, bool state)
{
    int index = user_slot_set_.GetUserIndexFromKey(object_key);
    if (INVALID_USER_SLOT_INDEX == index)
    {
        return;
    }

    UserSlot* slot = user_slot_set_.GetUserByIndex(index);
    if (state)
    {
        if (slot && slot->GetSlotState() == UserSlot::FILL)
        {
            user_slot_set_.SetReadyIndex(index);
        }
    }
    else
    {
        if (slot && slot->GetSlotState() == UserSlot::READY)
        {
            user_slot_set_.SetReadyCancleIndex(index);
        }
    }

    uiPartyMan* PartyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (PartyMan)
    {
        PartyMan->SetBattleZoneReady(object_key, state);
    }
}

void uiBattlezone2Man::SetCancleAllReady()
{
    for (int i = 1; i < GetMaxUserSlot() ; ++i)
    {
        UserSlot* slot = GetUserSlotByIndex(i);
        if (slot->GetSlotState() == UserSlot::READY)
        {
            slot->SetSlotState(UserSlot::FILL);
        }
    }

    bool is_master_ready = false;
    uiPartyMan* PartyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (PartyMan)
    {
        uicPartyMemberInfoPlus* member_info = PartyMan->GetPartyMemberInfo(PartyMan->GetMasterKey());
        if (member_info)
        {
            is_master_ready = member_info->is_ready();
        }
        PartyMan->AllCancleBattleZoneReady();
    }

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

    if (!is_master_ready)
    {
        // 2485 레디가 모두 풀렸습니다.
        g_InterfaceManager.GetInterfaceString(2485, message);

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
        GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
    }
}

void uiBattlezone2Man::SetShareType(BYTE share_type)
{
    if (ui_battlezone2_lobby_)
    {
        ui_battlezone2_lobby_->SetShareType(share_type);
    }
}

void uiBattlezone2Man::SetNonBlockSlot(int nonblock_slot)
{
    UnsetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);
    // 실제 서버에 남아 있는 NonBlock 개수 - 클라에 있는 NonBlock 개수
    int remain_player_number = nonblock_slot - GetNonBlockUserSlotNum();	

    if (remain_player_number == 0) 
    {
        return;
    }

    if (remain_player_number < 0)
    {
        remain_player_number = abs(remain_player_number);

        // 줄였다.
        // 유저 인원 제한이 줄게되면
        // 뒤에서 부터 Open 되어있는 슬롯에 대해서 Block을 걸어준다.

        if (IsMaster() && ui_battlezone2_lobby_)
        {
            UserSlot* user_slot = GetUserSlotByIndex(ui_battlezone2_lobby_->clicked_slot_index());
            if (user_slot && (user_slot->GetSlotState() == UserSlot::OPENED))
            {
                user_slot_set_.SetBlockIndex(ui_battlezone2_lobby_->clicked_slot_index());
                --remain_player_number;
            }
            return;
        }

        for (int i = GetMaxUserSlot()-1; i >= 1; --i)	
        {
            if (remain_player_number <= 0)
            {
                break;
            }

            UserSlot* user_slot = GetUserSlotByIndex(i);
            if (user_slot && (user_slot->GetSlotState() == UserSlot::OPENED))
            {
                user_slot_set_.SetBlockIndex(i);
                --remain_player_number;
            }
        }				
    }
    else 
    {
        // 늘였다. 
        // 유저 인원 제한이 늘게되면
        // 앞에서 부터 Open 되어있는 슬롯에 대해서 Open 한다.

        if (IsMaster() && ui_battlezone2_lobby_)
        {
            UserSlot* user_slot = GetUserSlotByIndex(ui_battlezone2_lobby_->clicked_slot_index());
            if (user_slot && (user_slot->GetSlotState() == UserSlot::BLOCK))
            {
                user_slot_set_.SetOpenIndex(ui_battlezone2_lobby_->clicked_slot_index());
                --remain_player_number;
            }
            return;
        }

        for (int i = 1; i < GetMaxUserSlot(); ++i)	
        {
            if (remain_player_number <= 0)
            {
                break;
            }

            UserSlot* user_slot = GetUserSlotByIndex(i);
            if (user_slot && (user_slot->GetSlotState() == UserSlot::BLOCK))
            {
                --remain_player_number;
                user_slot_set_.SetOpenIndex(i);
            }
        }
    }
}

bool uiBattlezone2Man::IsMaster() const
{
    if (g_pHero)
    {
        return bool(g_pHero->GetObjectKey() == master_player_key_);
    }
    return false;
}

void uiBattlezone2Man::set_current_room_info(PartyRoomInfo* room_info)
{
    if (room_info == NULL)
    {
        return;
    }

    current_room_info_.room_key_ = room_info->room_key_;
    current_room_info_.link_party_key_ = room_info->link_party_key_;
    current_room_info_.is_mission_start_ = room_info->is_mission_start_;
    current_room_info_.current_user_num_ = room_info->current_user_num_;
    current_room_info_.max_user_num_ = room_info->max_user_num_;
    current_room_info_.base_party_room_info = room_info->base_party_room_info;
}

void uiBattlezone2Man::set_current_room_max_user_num(BYTE number)
{
    // 2013.05.24 / 송찬종 / 현재 룸 정보에 최대 유저 수를 조절 안하고 있어서 슬롯 싱크가 안맞던 현상 수정
    current_room_info_.max_user_num_ = number;
}

void uiBattlezone2Man::SetBlockUserSlot()
{
    UnsetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);
    if (user_slot_set_.GetNonBlockNum() > (DWORD)pending_player_number_)
    {
        // 블럭
        if (IsMaster() && ui_battlezone2_lobby_)
        {
            user_slot_set_.SetBlockIndex(ui_battlezone2_lobby_->clicked_slot_index());
        }
        else
        {
            user_slot_set_.SetBlockIndex(pending_player_number_);
        }

        set_current_room_max_user_num(current_room_info()->max_user_num_ -1);
    }
    else 
    {
        // open
        if (IsMaster() && ui_battlezone2_lobby_)
        {
            user_slot_set_.SetOpenIndex(ui_battlezone2_lobby_->clicked_slot_index());
        }
        else
        {
            user_slot_set_.SetOpenIndex(user_slot_set_.GetNonBlockNum());
        }
        set_current_room_max_user_num(current_room_info()->max_user_num_ +1);
    }
}

bool uiBattlezone2Man::IsReadyForStart()
{
    // 방장을 제외한 타유저들이 레디를 걸고있는지 check
    for (unsigned int i = 1; i < user_slot_set_.GetMaxUser(); ++i)
    {
        UserSlot* slot = GetUserSlotByIndex(i);

        if(!slot)
        {
            continue;
        }

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        // 튕긴 사람 제외
        uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
        if (ui_party_manager)
        {
            uicPartyMemberInfoPlus* member_info =
                ui_party_manager->GetPartyMemberInfo(slot->GetPlayerKey());
            if (member_info && (member_info->IsConnected() == false))
            {
                continue;
            }
        }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

        if (slot->GetSlotState() == UserSlot::OPENED)
        {
            // 유저가 들어와있지 않을때는 방장의 권한으로 방생성은 할수있다.
            continue;
        }
        else
        {
            if (slot->GetSlotState() == UserSlot::BLOCK)
            {
                // 블럭된 슬롯은 논외
                continue;
            }
            else
            {
                if (slot->GetSlotState() != UserSlot::READY)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

/*bool uiBattlezone2Man::CloseAllUI()
{
}*/

bool uiBattlezone2Man::StartBattle()
{
    assert(IsMaster());
    
    const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(current_room_info_.base_party_room_info.map_code_);
    if (mapInfo == NULL)
    {
        if (current_room_info_.base_party_room_info.map_code_ != 0)//맵 없음인 경우 0일수 있음
        {
            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("Error: pMapInfo is NULL"));
            assert(!"멥정보가 잘못되었다!!");
        }
        return false;
    }

    eZONETYPE zone_type = BattlezoneMissionTreeParser::Instance()->GetZoneType(current_room_info_.base_party_room_info.map_code_);
    switch (zone_type)
    {
    case eZONETYPE_MISSION:
    {
        set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);

        //BattleScene::SetLobbyFlag(TRUE);
        /*if (SafePKBool(PACKET_STATUS_SEND_CREATE))
        {
            SetPKBool(PACKET_STATUS_SEND_CREATE, 60);
            //BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );
            MSG_CG_ZONE_MISSION_CREATE_SYN SendPacket; //이 패킷의 답변으로 CG_ZONE_MISSION_JOIN_ACK 가 옴.
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }*/

    }
    break;
    case eZONETYPE_INSTANCE:
        {
            eINSTANCE_DUNGEON_KIND dungeon_type = eINSTANCE_DUNGEON_KIND(mapInfo->byMType);
            if (dungeon_type == INDUN_KIND_EXT_03) //ex : 2008 크리스마스 눈사람 이벤트
            {
#ifndef _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
                if (g_pHero->GetWeaponKind() != eWEAPONTYPE_PUNCH)
                {
                    // 5755	무기를 장착하고 있어 이벤트 맵으로 입장할 수 없습니다.
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                    g_InterfaceManager.GetInterfaceString(5755, message, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
                    return false;
                }
#endif // _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
                if (GetUserSlotNum() > 1)
                {
                    // 5756	이벤트 맵 입장인원은 1명으로 제한되어 있어 이벤트 맵으로 입장할 수 없습니다.
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                    g_InterfaceManager.GetInterfaceString(5756, message, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
                    return false;
                }
            }
            set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);

            //BattleScene::SetLobbyFlag(TRUE);
            /*if (SafePKBool(PACKET_STATUS_SEND_CREATE))
            {
                SetPKBool(PACKET_STATUS_SEND_CREATE, 60);
                //BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );
                MSG_CG_ZONE_MISSION_CREATE_SYN SendPacket; //이 패킷의 답변으로 CG_ZONE_MISSION_JOIN_ACK 가 옴.
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
            }*/
        }
        break;

    case eZONETYPE_PVP:
        {
            if (SafePKBool(PACKET_STATUS_SEND_CREATE))
            {
                SetPKBool(PACKET_STATUS_SEND_CREATE, 60);
                set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);

                //BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );

                MSG_CG_ZONE_PVP_CREATE_SYN SendPacket;			//이 패킷의 답변으로 CG_ZONE_PVP_JOIN_ACK 가 옴.
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));			
            }
        }
        break;

    case eZONETYPE_LOBBY:	// 여기가 로비당
        {

        }
        break;
    default:
        {
            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("GetBattleRoomEntry().GetType() == %d"), current_room_info_.base_party_room_info.party_room_type_);
            assert(0);
            return false;
        }
        break;
    }
    return true;
}

//------------------------------------------------------------------------------
void uiBattlezone2Man::ClearObjectKey()
{
    // ObjectManager에서 Object가 모두 제거된 상태
    for (int i = 0; i < GetMaxUserSlot(); ++i)
    {
        UserSlot* user_slot = GetUserSlotByIndex(i);
        if (user_slot != NULL)
        {
            user_slot->SetBattleZonePlayerKey(0);
        }
    }
}

//------------------------------------------------------------------------------
void uiBattlezone2Man::MoveBattleZoneIcon()
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    // 배틀존 아이콘을 미니맵 옆에 위치
    RECT radar_dialog_rect = {0,0,0,0};
    RECT icon_rect = {0,0,0,0};
    
    if (ui_battlezone2_icon_ == NULL)
    {
        return;
    }

    if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
    {
        uiBase* radar_dialog = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
        if (radar_dialog != NULL)
        {
			CDialogWZ* const pDialogWz = radar_dialog->GetDialogWZ();

			if	(pDialogWz && ui_battlezone2_icon_)
			{
				pDialogWz->GetClientRect(&radar_dialog_rect);
				ui_battlezone2_icon_->GetDialogWZ()->GetDlgSize(&icon_rect);
				ui_battlezone2_icon_->MoveWindowWZ(radar_dialog_rect.left - icon_rect.right,
					radar_dialog_rect.bottom - icon_rect.bottom);
			}
        }
    }
    else
    {
        uiBase* radar_dialog = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
        if (radar_dialog != NULL)
        {
			CDialogWZ* const pDialogWz = radar_dialog->GetDialogWZ();

			if	(pDialogWz && ui_battlezone2_icon_)
			{
				pDialogWz->GetClientRect(&radar_dialog_rect);
				ui_battlezone2_icon_->GetDialogWZ()->GetDlgSize(&icon_rect);
				ui_battlezone2_icon_->MoveWindowWZ(radar_dialog_rect.left - icon_rect.right,
					radar_dialog_rect.bottom - icon_rect.bottom);
			}
        }
    }
#endif
}

#endif//_DH_BATTLEZONE2_