#pragma once

#include <ServerSessions/ServerSessionEx.h>
#include "ServerSessions/ServerSessionManager.h"
#include "UserSessions/User.h"
#include "UserSessions/UserManager.h"
#include "Zone/ZoneInterface.h"
#include "Zone/RoomInterface.h"
#include "Zone/ZoneManager.h"
#include <AgentParty/AgentPartyManager.h>

enum ePARTY_OPERATOR_TYPE
{
	ePARTY_OPERATOR_SEND,
	ePARTY_OPERATOR_INIT,
	ePARTY_OPERATOR_INVITE,
	ePARTY_OPERATOR_RELAY_USER,
	ePARTY_OPERATOR_FITNESS_LEVEL_CHECK,
    ePARTY_OPERATOR_SAME_CLASS_CHECK,
	ePARTY_OPERATOR_MIGRATE_CHECK,
	ePARTY_OPERATOR_TOWEROFCURSE_MIGRATE,
    //{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    ePARTY_OPERATOR_PORTAL_CREATE,
    ePARTY_OPERATOR_PORTAL_CHANGE,
    ePARTY_OPERATOR_PORTAL_CLOSE,
    ePARTY_OPERATOR_PORTAL_GETSTATE,
    //}
	ePARTY_OPERATOR_SHADOW_CHECK,
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    ePARTY_OPERATOR_KICK_USER_AFTER_CHECK_LEVEL,

};

class LinkServerOperator
{
public:
	LinkServerOperator(MSG_BASE* pMsg, WORD wSize) : m_pMsg(pMsg), m_wSize(wSize) {}
	~LinkServerOperator() {}

	VOID operator() (DWORD sessionIndex)
	{
		ServerSessionEx* pServerSession = ServerSessionManager::Instance()->FindServer(sessionIndex);
		if(pServerSession == 0)
            return;

		if(pServerSession->GetServerType() == FIELD_SERVER ||
           pServerSession->GetServerType() == BATTLE_SERVER)
		{
			pServerSession->SendPacket(m_pMsg, m_wSize);
		}
	}

private:
	MSG_BASE*		m_pMsg;
	WORD			m_wSize;
};


class MemberOperator
{
public:
	MemberOperator() {}
	~MemberOperator() {}

	VOID Init(ePARTY_OPERATOR_TYPE eType, DWORD exceptKey, KEYTYPE checkZoneKey)
	{
		m_eOprType = eType;
		m_ExceptKey = exceptKey;
		m_CheckZoneKey = checkZoneKey;
		m_SuccessFlag = TRUE;
		m_pMsg = NULL;
		m_pZone = NULL;
		m_Param1 = m_Param2 = 0;
		memset(m_charClasses, 0, eCHAR_TYPE_MAX);
	}

	VOID SetSendPacket(MSG_BASE *pMsg, WORD wSize)	{ m_pMsg = pMsg;	m_Param1 = wSize; }
	VOID SetZone(ZoneInterface *pZone)				{ m_pZone = pZone; }
	VOID SetFitnessLV(LEVELTYPE MinLevel, LEVELTYPE MaxLevel)	{ m_Param1 = MinLevel; m_Param2 = MaxLevel; }
	VOID SetTransaction(DWORD Tran)					{ m_Param1 = Tran; }

	BOOL IsSuccess()	{ return m_SuccessFlag; }

	VOID operator () (_PARTY_MEMBER_INFO *pMemberInfo)
	{
		if(m_ExceptKey && m_ExceptKey == pMemberInfo->m_dwMemberKey)
			return;

		// 이미 실패한 체크루틴이므로 더 이상 돌 필요가 없다.
		if(m_SuccessFlag == FALSE)
			return;

		User *pUser = UserManager::Instance()->GetUserByObjKey(pMemberInfo->m_dwMemberKey);
		if(!pUser)	return;

		switch(m_eOprType)
		{
		case ePARTY_OPERATOR_SEND:			pUser->SendPacket(m_pMsg, (WORD)m_Param1);	break;
		case ePARTY_OPERATOR_INIT:			pUser->GetPartyState().Init();					break;
		case ePARTY_OPERATOR_INVITE:		m_pZone->Invite(pUser);						break;
		case ePARTY_OPERATOR_RELAY_USER:	
			{
				RoomInterface * pRoom = (RoomInterface*)m_pZone;
				pUser->StartTimer(RoomInterface::MAX_RELAY_TIMEOUT_TIME);
				pRoom->AddRelayUser(pUser, pUser->GetPlayerKey());
			}break;
		case ePARTY_OPERATOR_FITNESS_LEVEL_CHECK:
			{
				if(pUser->GetCharLV() < m_Param1 || m_Param2 < pUser->GetCharLV())
					m_SuccessFlag = FALSE;
			}break;
        case ePARTY_OPERATOR_SAME_CLASS_CHECK:
            {
                if(pUser->GetCharClass() >= eCHAR_TYPE_MAX)	return;

                ++m_charClasses[pUser->GetCharClass()];
                if(m_charClasses[pUser->GetCharClass()] >= 5)
                    m_SuccessFlag = false;
            }break;
		case ePARTY_OPERATOR_SHADOW_CHECK:
			{
				if(pUser->GetCharClass() == eCHAR_SHADOW)
					m_SuccessFlag = FALSE;
			}
            break;
		case ePARTY_OPERATOR_MIGRATE_CHECK:
			{
                if(pUser->IsBeginTransaction())
                    m_SuccessFlag = FALSE;
                eZONE_STATE zoneState = pUser->GetZoneState();
                if(zoneState != ePRS_AT_VILLAGE &&
                    zoneState != ePRS_AT_DOMINATION_FIELD &&
                    zoneState != ePRS_AT_FIELD)
                    m_SuccessFlag = FALSE;
			}
            break;
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        case ePARTY_OPERATOR_KICK_USER_AFTER_CHECK_LEVEL:
            {
                if(pUser->GetCharLV() < m_Param1 || m_Param2 < pUser->GetCharLV())
                {
                    AgentPartyManager* party_manager = AgentPartyManager::Instance();
                    party_manager->LeaveParty(pUser, eREASON_LEAVE_PARTY_NOT_MATCH_LEVEL);
                }
            }
            break;
        default:
            __debugbreak(); // 그냥 뻗는게 문제점 해결에 더 유용할 것이다. 개발에서 발견될 터.
            break;
		}
	}

// (WAVERIX) (090210) (Chnages) private->protected
protected:
	ePARTY_OPERATOR_TYPE	m_eOprType;
	DWORD					m_ExceptKey;
	BOOL					m_SuccessFlag;
	KEYTYPE					m_CheckZoneKey;

	MSG_BASE *				m_pMsg;
	ZoneInterface *			m_pZone;
	DWORD					m_Param1;
	DWORD					m_Param2;

	BYTE					m_charClasses[eCHAR_TYPE_MAX];
};

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
class MemberQueryResultOperator : public MemberOperator
{
public:
    struct UserState
    {
        User* pUser;
        WORD state;    // PortalUserInfo::eState | PortalKey
        PORTALKEY prevPortalKey; // state: changed일 경우 기존 정보 제거 목적
    };
    typedef STLX_VECTOR<UserState>  UserStateList;

    VOID Init(ePARTY_OPERATOR_TYPE eType, DWORD ExceptKey, KEYTYPE CheckZoneKey)
    {
        MemberOperator::Init(eType, ExceptKey, CheckZoneKey);
        m_IsTempPortal = 0;
        m_PortalKey = INVALID_PORTAL_KEY;
        m_pQuestPortalInfo = 0;
        m_UserStateList.clear();
    }

    inline VOID SetQuestPortalInfo(BOOLEAN isTempPortal, PORTALKEY portalKey,
                                   const QuestPortalInfo* pQuestPortalInfo)
    {
        m_IsTempPortal = isTempPortal;
        m_PortalKey = portalKey;
        m_pQuestPortalInfo = pQuestPortalInfo;
    }
    inline const UserStateList& GetUserStateList() const { return m_UserStateList; }

    VOID operator()(_PARTY_MEMBER_INFO* pMemberInfo)
    {
        if(m_ExceptKey && m_ExceptKey == pMemberInfo->m_dwMemberKey)
            return;

        // 이미 실패한 체크루틴이므로 더 이상 돌 필요가 없다.
        if(m_SuccessFlag == FALSE)
            return;

        User* pUser = UserManager::Instance()->GetUserByObjKey(pMemberInfo->m_dwMemberKey);
        if(pUser == 0) // < 이래도 되는 걸까?
            return;

        if(m_eOprType == ePARTY_OPERATOR_TOWEROFCURSE_MIGRATE)
            MigrateTowerOfCurse(pUser);
        else if(m_eOprType == ePARTY_OPERATOR_PORTAL_CREATE)
            PortalCreate(pUser);
        else if(m_eOprType == ePARTY_OPERATOR_PORTAL_CHANGE)
            PortalChange(pUser);
        else if(m_eOprType == ePARTY_OPERATOR_PORTAL_CLOSE)
            PortalClose(pUser);
        else if(m_eOprType == ePARTY_OPERATOR_PORTAL_GETSTATE)
            PortalGetState(pUser);
    }

    // UserState = { pUser, migrate (ok/fail) state, 0 }
    VOID MigrateTowerOfCurse(User* pUser);
    VOID PortalCreate(User* pUser);
    VOID PortalChange(User* pUser);
    VOID PortalClose(User* pUser);
    VOID PortalGetState(User* pUser);
private:
    BOOLEAN m_IsTempPortal;
    PORTALKEY m_PortalKey;
    const QuestPortalInfo* m_pQuestPortalInfo;
    UserStateList m_UserStateList;
};
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

enum eUSER_OPERATOR_TYPE
{
	eUSER_OPERATOR_JOINPARTY,
	eUSER_OPERATOR_JOINPARTY_PVPROOM,
};

class JoinPartyOperator
{
public:
	JoinPartyOperator() {}
	~JoinPartyOperator() {}

	VOID Init(eUSER_OPERATOR_TYPE eType, WORD PartyKey1, WORD PartyKey2=0)
	{
		m_eOprType = eType;
		m_PartyKey1 = PartyKey1;
		m_PartyKey2 = PartyKey2;
		m_TeamKey1 = m_TeamKey2 = 0;
	}

	VOID SetPVPRoomTeam(BYTE TeamKey1, BYTE TeamKey2)	{ m_TeamKey1 = TeamKey1; m_TeamKey2 = TeamKey2; }

	VOID operator () (User* pUser)
	{
		switch(m_eOprType)
		{
		case eUSER_OPERATOR_JOINPARTY:			
			{
				AgentPartyManager::Instance()->JoinParty(m_PartyKey1, pUser);
			}break;
		case eUSER_OPERATOR_JOINPARTY_PVPROOM:
			{
				if(pUser->GetTeam() == m_TeamKey1)
					AgentPartyManager::Instance()->JoinParty(m_PartyKey1, pUser);
				else if(pUser->GetTeam() == m_TeamKey2)
					AgentPartyManager::Instance()->JoinParty(m_PartyKey2, pUser);
				else
					SUNLOG(eCRITICAL_LOG, "[JoinPartyOperator] Invalid TeamKey1[%u] TeamKey2[%u] UserTeamKey[%u]!", m_TeamKey1, m_TeamKey2, pUser->GetTeam());
			}break;
		}
		
	}

private:
	eUSER_OPERATOR_TYPE	m_eOprType;
	WORD				m_PartyKey1;
	WORD				m_PartyKey2;
	BYTE				m_TeamKey1;
	BYTE				m_TeamKey2;
};

