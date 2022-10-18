#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
// <SHARED>
#include <SCStream.h>

//////////////////////////////////////////////////////////////////////////
// <AGENT SERVER>
#include <AgentServer.h>
#include <PacketHandler/PacketHandler.h>
#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/ServerSessionManager.h>
#include <TimeCheck/HeartbeatChecker.h>
#include <Zone/ZoneInterface.h>
#include <Zone/ZoneManager.h>
#include <Zone/PortalManager.h>
#include "User.h"
#include "TempUser.h"
#include "UserManager.h"

#include "Zone/CharSelect.h"

#include <AgentParty/AgentPartyManager.h>

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    #include "../Function/GameOptionManager.h"
#endif

#ifdef _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
    #include <AgentParty/PartyRoomFinder.h>
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <ChaosZoneSystem/OneDaysGuildMissionManager.h>
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

User::User()
	: m_State(POOL_STATE)
	, m_eBehaveState(PLAYER_BEHAVE_IDLE_STATE)
	, m_eTransactionState(TR_END)
	, closing_user_trans_controller_(closing_user_trans_info_)
{
};

User::~User()
{
}

VOID
User::CopyInfo(TempUser* pTempUser)
{
	SetAuthID(pTempUser->GetAuthID());
	SetUserGUID(pTempUser->GetUserGUID());
	SetUserKey(pTempUser->GetUserGUID());			//< USER GUID를 키로 셋팅!
	SetUserID(pTempUser->GetUserID());
	SetClientIP(pTempUser->GetClientIP());
	SetLastSSNKey(pTempUser->GetLastSSNKey());
    SetBillingType(pTempUser->GetBillingType()); //_PCBANG_POLICY_CHANGE_POST_EXPIRED
	SetReservedValue(pTempUser->GetReservedValue(true));
    SetAuthUserType(pTempUser->GetAuthUserType());
	SetSts(pTempUser->GetSts());
	SetPCRoomSts(pTempUser->GetPCRoomSts());
	SetLoginTime(pTempUser->GetLoginTime());
	SetLoginTime_t(pTempUser->GetLoginTime_t());
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	SetAge(pTempUser->GetAge());
	SetRealNameAuth(pTempUser-> GetRealNameAuth());
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	SetGameChuUSN(pTempUser->GetGameChuUSN());
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM    
    SetBetaKey(pTempUser->GetBetaKeyInfo()->GetBetaKey());
#endif

#ifdef _NA_20100802_SPEED_HACK_DETECTION
    GetTickSpeedChecker().SetUserID();
#endif
}

VOID
User::Init()
{
	UserSession::Init();

	m_eTransactionState = TR_END;

	UnlinkServer();

	SECUREINFO.Init();
	SECUREINFO.HEARTBEAT().Init();	// HEARTBEAT의 체크 시작은 캐릭터 선택창에서 이뤄진다.
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    SECUREINFO.TICKSPEED().Init(this);
#endif

	SSQHandler = ISSQHandlerD(this, NULL);

	// 캐릭터 슬롯 초기화 (초기값 SelectedIndex = MAX_CHARACTER_LIST_NUM)
    character_list_.Init();

	SetZoneType(eZONETYPE_MAX);
	SetMoveZoneType(eZONETYPE_MAX, eZONETYPE_MAX, 0);
	SetZoneState(ePRS_NOT_AT_ZONE);

	closing_user_trans_controller_.Init();

	SetReady(FALSE);
	SetMaster(FALSE);
	SetTeam(0);

	GetMissionManager()->Clear();

	SetSelectedCharCHuntingVCount(0);
	SetSelectedCharCHuntingTotCount(0);

	m_bLogOutWait = false;
	m_bExecuteDisconnectUser = false;
    world_service_enabled_state_ = false;
	m_ItemOwnFlag.Init();

	SetState(INIT_STATE);

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	m_byAge = 0;
	m_byRealNameAuth = 0;
#endif
	m_LockItemPosForInstanceDungeon = INVALID_POSTYPE_VALUE;

	//비정상 접속종료 정보 초기화
	m_PartyReturnInfo.Init();
    m_PortalUserInfo.SetUser(this);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    m_ReAuthInfo.Clear();
#endif
    m_ReservedHP = 0;
    m_PlayerStateEvent = PlayerStatEvent::eFlag_None;
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    m_isInvaitPartyRoom = TRUE;
}

VOID
User::Release()
{
	//	__NA000630_070817_MOLA_5th_UPDATE
	SECUREINFO.MOLAController().SendQueryRelease(this);

	//	풀에 반환할 때 강제로 클라이언트를 접속해제 시킨다.
	//	case : 게임서버가 다운 되었을 경우
	SetDisconnectCode(RC::RC_DISCONNECT_USER_RELEASE);
	DisconnectUser();			//< tempuser or usersession에 넣지 않는다.

	UnlinkServer();
	SECUREINFO.Release();
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    m_ReAuthInfo.Clear();
#endif
    m_PortalUserInfo.SetUser(0);
    character_list_.Release(); // CHANGES: f110517.2L
    UserSession::Release();

	SetState(POOL_STATE);
}

VOID
User::OnAccept(DWORD dwNetworkIndex)
{
    __UNUSED(dwNetworkIndex);
	ASSERT(!"들어 올 수 없는 부분");
}

void
User::OnDisconnect()
{
    _DisconnectUser();

    // 네트워크 연결이 완전히 종료되었을 때 처리할 부분들을 처리한다. 
    UserManager* const user_manager = UserManager::Instance();
    // CHANGES: f110516.6L, it's possible that the char_guid and char_name of the user are null
    // when user is disconned on the char_select zone.
    GAMELOG->DisconnectChar(this);
    const CHARGUID char_guid = this->GetSelectedCharGuid();
    if (char_guid) {
        user_manager->RemoveUserForSelectedCharInfoHashes(this);
    };

    EndCharLoginTime();

    UserSession::OnDisconnectInfoToAuth();
    UserSession::OnDisconnect();
}

VOID
User::DisconnectLinkedServerUser()
{
	UserSession::DisconnectUser(TRUE);

	if(m_bExecuteDisconnectUser)	return;

	// 파티에 가입되어 있으면 탈퇴!	<- (080212)(WAVERIX) 이것은 서버가 끊겼을 때 문제를 야기한다.
	eREASON_LEAVE_PARTY eReason = m_bLogOutWait ? eREASON_LEAVE_PARTY_INTEND : eREASON_LEAVE_PARTY_ABNORMAL;
	AgentPartyManager::Instance()->LeaveParty(this, eReason);

	// 룸에 있으면 안에서 삭제하고 UnLink
	LeaveRoom();

	// World 서버로 끊어졌음을 알린다.
	MSG_AW_USER_LOGOUT msgCMD;
	msgCMD.m_byCategory = AW_USER_STATUS;
	msgCMD.m_byProtocol = AW_USER_LOGOUT;
	msgCMD.m_dwKey		= GetUserKey();
	SendToWorldServer(&msgCMD, sizeof(msgCMD));

	m_bExecuteDisconnectUser = true;
    world_service_enabled_state_ = false;
}


VOID
User::DisconnectUser(BOOL bGracefulDisconnect)
{
    UserSession::DisconnectUser(bGracefulDisconnect);

	// 유저를 종료시킬 때 굳이 OnDisconnect(실제 네트워크 상의 완전 종료)를 기다리지 않고 미리 처리할 부분들을 처리한다.
	_DisconnectUser();
}

// RemoveUser를 제외한 OnDisconnect의 대부분의 처리를 여기서 처리한다.
VOID
User::_DisconnectUser()
{
	if(m_bExecuteDisconnectUser)	return;

    // CHANGES: f110705.2L, moved from 'DisconnectUser' to prevent duplicated calling.
    // DONE: (sun:r54618, namka199), it's meanless retry instance and release when an user can't found.
#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    GameOptionManager::Instance()->Disconnect_User(this);
#endif

	if(GetLinkedServer())
	{
		// Send To Game Server
		MSG_AG_CONNECTION_DISCONNECT_CMD msgCMD;
        //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
        msgCMD.m_AbNormalExit = m_bLogOutWait ? 0 : 1;

		SendToLinkedServer(&msgCMD, sizeof(msgCMD));
	}
	else
	{
		MSG_AD_CONNECTION_DISCONNECT_CMD msgCMD;
		msgCMD.m_dwKey = GetUserKey();
		msgCMD.m_GuildGuid = GetSelectedCharGuildGuid();
		msgCMD.m_CharGuid = GetSelectedCharGuid();
		msgCMD.m_ChannelID = AgentServer::GetInstance()->GetKey().GetChannelID();
		AgentServer::GetInstance()->SendToDBPServer(&msgCMD, sizeof(msgCMD));
	}

	// 파티에 가입되어 있으면 탈퇴!	<- (080212)(WAVERIX) 이것은 서버가 끊겼을 때 문제를 야기한다.
	eREASON_LEAVE_PARTY eReason = m_bLogOutWait ? eREASON_LEAVE_PARTY_INTEND : eREASON_LEAVE_PARTY_ABNORMAL;
	AgentPartyManager::Instance()->LeaveParty(this, eReason);

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    OnServerSelectQueryResult(m_ReAuthInfo.eReAuthSlot_AllSide, m_ReAuthInfo.eReAuthStep_None, 0);
#endif

    PortalManager::ClearUserInfo(this);
	// 룸에 있으면 안에서 삭제하고 UnLink
	LeaveRoom();
    
#ifdef _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomFinder* room_finder = party_manager->GetPartyRoomFinder();
    if (room_finder != NULL)
    {
        room_finder->DeleteAllCharGuidFromMissionList(this->GetSelectedCharGuid());
    }
#endif  // _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager->IsValidReservationMember(GetUserKey()))
    {
        BattleGroundMemberInfo* const member = \
            battle_ground_manager->FindBattleGroundMember(GetUserKey());
        if (member)
        {
            if (member->group_key == 0)
            {
                battle_ground_manager->RemoveMemberStandbyGroup(member);
                battle_ground_manager->RemoveReservationMember(GetUserKey());
            }
            else
            {
                battle_ground_manager->LeaveRoom(this, GetTRRoomKey());
            }
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	OneDaysGuildMissionManager* const onedaysGuildMissionmanager = OneDaysGuildMissionManager::Instance();
	if(onedaysGuildMissionmanager)
	{
		onedaysGuildMissionmanager->LeaveRoom(GetTRRoomKey(), this);
	}
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    
	// World 서버로 끊어졌음을 알린다.
	MSG_AW_USER_LOGOUT msgCMD;
	msgCMD.m_byCategory = AW_USER_STATUS;
	msgCMD.m_byProtocol = AW_USER_LOGOUT;
	msgCMD.m_dwKey		= GetUserKey();
	SendToWorldServer(&msgCMD, sizeof(msgCMD));

	m_bExecuteDisconnectUser = true;
    world_service_enabled_state_ = false;
}

BOOL
User::IsMovingBetweenServer()
{
    // 서버간 이동
    // FieldServer와 BattleServer간 이동인지의 여부를 확인하기 위함이다
	switch(m_eToZoneType)
	{
	case eZONETYPE_VILLAGE:
		{
			switch(m_eFromZoneType)
			{
			case eZONETYPE_LOBBY:
			case eZONETYPE_FIELD:
            case eZONETYPE_DOMINATION_FIELD:
			case eZONETYPE_CHARSELECT:
            case eZONETYPE_SPA_FIELD:
				break;
			default:
				return TRUE;
			};
		}
		break;
	case eZONETYPE_MISSION:
	case eZONETYPE_HUNTING:
	case eZONETYPE_CHUNTING:
	case eZONETYPE_PVP:
		return TRUE;
	}

	return FALSE;
}

VOID
User::LeaveRoom()
{
	const eZONE_STATE zsSTATE = GetZoneState();

	const BOOL bRet
		= (ePRS_AT_ZONE&zsSTATE) ?				TRUE
		: (ePRS_BEFORE_ENTER_ZONE&zsSTATE) ?	TRUE
		:										FALSE;

	if(bRet)
	{
		ZoneInterface* pZone = g_ZoneManager.FindZone(GetZoneKey());
		pZone->LeaveUser(this, eSEND_NONE, eZONETYPE_MAX, 0);
	}
}

// NOTE: f110622.2L
bool User::NotifyHeIsHereIfSatisfied()
{
    if (m_bExecuteDisconnectUser) {
        return false;
    };
    if (LinkedServer.GetLinkedServer() != NULL) {
        return false;
    };
    if (he_is_in_char_select_timer_.IsExpired() == false) {
        return false;
    };
    AgentServer* const agent_server = AgentServer::GetInstance();
    MSG_AD_CONNECTION_HE_IS_HERE_CMD msg_ntf;
    msg_ntf.m_dwKey = GetUserKey();
    msg_ntf.channel_id_ = agent_server->GetKey().GetChannelID();
    agent_server->SendToDBPServer(&msg_ntf, sizeof(msg_ntf));

    return true;
}

// NOTE: f110622.2L, the below method be used in case he located in the character selection zone
void User::ChangeHeIsHereNotificationTimer(bool started)
{
    if (started) {
        he_is_in_char_select_timer_.SetTimer(CharSelect::kHeIsHereNotificationInterval);
    }
    else {
        he_is_in_char_select_timer_.Clear();
    }
}

VOID
User::SerializePartyInfo(_PARTY_MEMBER_INFO& memberInfo)
{
    ZeroMemory(&memberInfo, sizeof(memberInfo));
	memberInfo.m_dwMemberKey = GetPlayerKey();

    const char* charName = GetSelectedCharName();
	if(charName)
	{   // (WAVERIX) (090408) (BUG-FIX) Buffer Overrun 수정
        strncpy(memberInfo.m_szCharName, charName, _countof(memberInfo.m_szCharName));
		memberInfo.m_szCharName[_countof(memberInfo.m_szCharName) - 1] = '\0';
	}

    memberInfo.m_wLevel = GetCharLV();
    memberInfo.m_byClassType = GetSelectedCharClass();
    memberInfo.m_byChaoState = GetSelectedCharChaoState();

    const DWORD maxHP = GetSelectedCharMaxHP();
    const DWORD curHP = GetSelectedCharCurHP();
    const DWORD maxMP = GetSelectedCharMaxMP();
    const DWORD curMP = GetSelectedCharCurMP();

    memberInfo.m_byHPRatio = maxHP ? (BYTE)(100 * curHP / maxHP) : 100;
    memberInfo.m_byMPRatio = maxMP ? (BYTE)(100 * curMP / maxMP) : 100;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    const DWORD maxSD = GetSelectedCharMaxSD();
    const DWORD curSD = GetSelectedCharCurSD();
    memberInfo.m_bySDRatio = maxSD ? (BYTE)(100 * curSD / maxSD) : 100;
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

#ifdef _NA_002050_20110216_ADD_GENDER
    memberInfo.m_byGender = GetSelectedCharGender();
#endif

    // _NA_20110630_BATTLE_ZONE_RENEWAL
    memberInfo.m_selecteTeam = GetTeam();
    memberInfo.m_isReady = AgentPartyManager::Instance()->GetMemberReadyState(this);
#ifdef _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY
    ZoneInterface* const current_zone = g_ZoneManager.FindZone(GetZoneKey());
    if (current_zone)
    {
        memberInfo.map_code_ = current_zone->GetMapCode();
    }
#else
    memberInfo.map_code_ = GetSelectedCharMapCode();
#endif // _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY
    
  #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    memberInfo.m_isConnected = true;
  #endif

    // reporting
    if((maxHP && maxMP) == false)
    {
        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"][ErrorReport] "
               "UID(%u) Name(%s) CurHP/MaxHP = (%u/%u), CurMP/MaxMP = (%u/%u)",
               GetUserKey(), charName, curHP, maxHP, curMP, maxMP);
    }
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
BOOLEAN
User::OnServerSelectQueryResult(nsPacket::ReAuthInfo::eReAuthSlot reAuthSlot,
                                nsPacket::ReAuthInfoBase::eReAuthStep reAuthStep,
                                const nsPacket::ReAuthTokenStream* arg)
{
    using namespace nsPacket;
    ReAuthInfo::ReAuthStepSlot& slots = m_ReAuthInfo.m_ReAuthSteps;

    BOOLEAN bStepCanceled = 0;
    if(reAuthSlot == m_ReAuthInfo.eReAuthSlot_AllSide)
    {
        if(reAuthStep != m_ReAuthInfo.eReAuthStep_Canceled)
        {
            if(!FlowControl::FCAssert(reAuthStep == m_ReAuthInfo.eReAuthStep_None ||
                                      reAuthStep == m_ReAuthInfo.eReAuthStep_Requested))
                return false;
            m_ReAuthInfo.Clear();

            if(reAuthStep == m_ReAuthInfo.eReAuthStep_Requested)
            {
                m_ReAuthInfo.m_Timeout.SetNextTimeoutByCurTickBased(10 * 1000);
                slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] = m_ReAuthInfo.eReAuthStep_Requested;
                slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] = m_ReAuthInfo.eReAuthStep_Requested;
                m_ReAuthInfo.m_bUseReAuth = TRUE;
            }
            return TRUE;
        }
        else
            bStepCanceled = TRUE;
    }

    // 명령 주는대로 설정하기로 한다.
    if(bStepCanceled == 0 && reAuthSlot == m_ReAuthInfo.eReAuthSlot_AuthServerSide)
    {
        if(reAuthStep == m_ReAuthInfo.eReAuthStep_Accepted)
        {   // assert(arg)
            if(slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] == m_ReAuthInfo.eReAuthStep_Requested)
                slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] = m_ReAuthInfo.eReAuthStep_Accepted;
            else
                bStepCanceled = TRUE;

            if(bStepCanceled == 0 && arg)
            {
                m_ReAuthInfo.SetReAuthStream(arg);

                SUNLOG(eCRITICAL_LOG, "[Game->Client] key : %s", arg->stream);
            }
        }
        else
        {
            slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] = reAuthStep;
        }
    }
    else if(bStepCanceled == 0 && reAuthSlot == m_ReAuthInfo.eReAuthSlot_GameServerSide)
    {
        if(reAuthStep == m_ReAuthInfo.eReAuthStep_Accepted)
        {
            if(slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] == m_ReAuthInfo.eReAuthStep_Requested)
                slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] = m_ReAuthInfo.eReAuthStep_Accepted;
            else
                bStepCanceled = TRUE;
        }
        else
        {
            slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] = reAuthStep;
        }
    }

    if(bStepCanceled)
    {
        if(slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] != m_ReAuthInfo.eReAuthStep_None)
        {
            MSG_AU_AUTH_SERVER_SELECT_SYN msgSYN;
            msgSYN.m_AuthUserID = GetAuthID();
            msgSYN.m_Request = msgSYN.eRequest_Cancel;
            AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&msgSYN, sizeof(msgSYN));

            slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] = m_ReAuthInfo.eReAuthStep_None;
        }
        if(slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] != m_ReAuthInfo.eReAuthStep_None)
        {
            MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_SYN msgSYN;
            msgSYN.m_Control = msgSYN.eControl_StopTransaction;
            SendToLinkedServer(&msgSYN, sizeof(msgSYN));
            slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] = m_ReAuthInfo.eReAuthStep_None;
        }
        m_ReAuthInfo.Clear();
    }

    return TRUE;
}

BOOLEAN
User::OnServerSelectQueryUpdate()
{
    using namespace nsPacket;
    if(m_ReAuthInfo.m_bUseReAuth == 0)
        return TRUE;

    BOOLEAN toBeNext = !!m_ReAuthInfo.m_Timeout.IsExpired();
    if(toBeNext == 0)
        return TRUE;

    ReAuthInfo::ReAuthStepSlot& slots = m_ReAuthInfo.m_ReAuthSteps;
    BOOLEAN bAcceptedAuth =
        slots[m_ReAuthInfo.eReAuthSlot_AuthServerSide] == m_ReAuthInfo.eReAuthStep_Accepted;
    BOOLEAN bAcceptedGame =
        slots[m_ReAuthInfo.eReAuthSlot_GameServerSide] == m_ReAuthInfo.eReAuthStep_Accepted;
    toBeNext = bAcceptedAuth;
    if (toBeNext && bAcceptedGame)
    {
        MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK msgACK;
        msgACK.m_Result = RC::RC_ROOM_SUCCESS;
        const ReAuthTokenStream* pToken = m_ReAuthInfo.GetReAuthStream();
        if(!FlowControl::FCAssert(pToken != 0))
            return FALSE;
        msgACK.m_ReAuthToken = *pToken;
        SendPacket(&msgACK, msgACK.GetSize());
        SetLogOutWait();
        SetDisconnectCode(RC::RC_DISCONNECT_FOR_SERVER_SELECT_TRANSACTION);
        DisconnectUser(TRUE);
    }
    else
    {
        OnServerSelectQueryResult(m_ReAuthInfo.eReAuthSlot_AllSide,
                                  m_ReAuthInfo.eReAuthStep_Canceled, 0);
    }

    return toBeNext;
}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

//#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
void
User::NotifyPcBangBillingStatus(BOOLEAN send_to_dbproxy, BOOLEAN send_to_world)
{
    DWORD pc_room_id = GetReservedValue();  // complex operation by GetBillingType()
    //                    status.eStatus_None | status.eStatus_PcRoom | status.eStatus_PcRoomExpired
    //GetReservedValue()          0                      id                           0

    AgentServer* pAgentServer = AgentServer::GetInstance();
    DWORD user_key = GetUserKey();

    // 유저의 상태를 평가할 필요성 타전
    // Status : added in UserManager(user_key)
    //          request character list ( -> dbproxy)
    //          notify new user enter ( -> world)

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    if(send_to_dbproxy) {
        MSG_AD_CHARINFO_PCBANG_BILLING_NTF msgNTF;
        msgNTF.m_dwKey = user_key;
        msgNTF.m_PcRoomId = pc_room_id;
        pAgentServer->SendToDBPServer(&msgNTF, sizeof(msgNTF));
    }

    if(send_to_world) {
        MSG_AW_USER_STATUS_PCBANG_BILLING_NTF msgNTF;
        msgNTF.m_dwKey = user_key;
        msgNTF.m_PcRoomId = pc_room_id;
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
        msgNTF.premiumservice_ = GetPremiumService();
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
        pAgentServer->SendToWorldServer(&msgNTF, sizeof(msgNTF));
    }
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED

    ;{
        MSG_AG_CHARINFO_PCBANG_BILL_CMD cmd;
        cmd.m_PcRoomId = pc_room_id;
        SendToLinkedServer(&cmd, sizeof(cmd));
    };

    // post estimated status
    // gameserver -> gameserver || gameserver -> agentserver (character select)
    // update pc billing status
}
//#endif











