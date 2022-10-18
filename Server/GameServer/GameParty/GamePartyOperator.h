#pragma once

#include <PublicMath.h>
#include "GameZone.h"
#include "GameGuildExtension.h"

enum eGAMEPARTY_OPERATOR_TYPE
{
	eGAMEPARTY_OPERATOR_PLAYER,
	eGAMEPARTY_OPERATOR_SEND,
	eGAMEPARTY_OPERATOR_INIT,
	eGAMEPARTY_OPERATOR_AREA_LIVE_PARTYINFO,
	eGAMEPARTY_OPERATOR_ADD_EXP,
	eGAMEPARTY_OPERATOR_HOSTILITY_CHECK,
    eGAMEPARTY_OPERATOR_VALID_PLAYER_INFO_FOR_PARTY_RETURN,
    eGAMEPARTY_OPERATOR_ENTER_ZONE_CONDITION_MEMBER_INFO,
    eGAMEPARTY_OPERATOR_ENTER_ZONE_CONDITION_PARTYMEMBER_INFO,
    eGAMEPARTY_OPERATOR_CHANGE_OPTION_CONDITION_PARTYMEMBER_INFO,
};

class BasePlayerOperator
{
public:
    void operator()(Player* player) { __UNUSED(player); }
};

template< class PlayerOperator=BasePlayerOperator >
class GameMemberOperator
{
public:
	GameMemberOperator() {}
	~GameMemberOperator() {}

	VOID Init( eGAMEPARTY_OPERATOR_TYPE eType, DWORD ExceptKey )
	{
		m_eOprType = eType;
		m_ExceptKey = ExceptKey;
		m_SuccessFlag = TRUE;
		m_pMsg = NULL;
		m_pwzPos = NULL;
		m_Param1 = m_Param2 = m_Param3 = m_Param4 = 0;
	}

	BOOL IsSuccess()	{ return m_SuccessFlag; }

	VOID SetSendPacket( MSG_BASE *pMsg, WORD wSize )		{ m_pMsg = pMsg;	m_Param1 = wSize; }
	VOID SetPlayerOpr( PlayerOperator *pOpr )				{ m_pPlayerOpr = pOpr; }
	VOID SetAreaInfo( ZONEKEY ZoneKey, const WzVector* pNpcPos, BYTE byRadius )	{ m_Param1 = ZoneKey; m_pwzPos = pNpcPos;	m_Param2 = byRadius; }
	VOID GetAreaInfo( WORD & OUT TotalNum, LEVELTYPE & OUT TotalLevel )			{ TotalNum = (WORD)m_Param3; TotalLevel = (LEVELTYPE)m_Param4; }
	VOID SetExp( EXPTYPE Exp )								{ m_Param1 = (DWORD)Exp; }
	VOID SetHostilityGuildGuid( DWORD HostilityGuildGuid )	{ m_Param1 = HostilityGuildGuid; }
	BOOL IsExistHostilityMember()							{ return m_Param2; }

    inline DWORD GetFirstValidPlayerForPartyReturn() const
    {
        if(m_eOprType == eGAMEPARTY_OPERATOR_VALID_PLAYER_INFO_FOR_PARTY_RETURN)
            return m_Param1;
        return 0;
    }

	VOID operator () ( _PARTY_MEMBER_INFO *pMemberInfo )
	{
		if( m_ExceptKey && m_ExceptKey == pMemberInfo->m_dwMemberKey )
			return;

		// 이미 실패한 체크루틴이므로 더 이상 돌 필요가 없다.
		if( m_SuccessFlag == FALSE )
			return;

		Player *pPlayer = PlayerManager::Instance()->FindPlayerByObjKey( pMemberInfo->m_dwMemberKey );
		if( !pPlayer )	return;

		switch( m_eOprType )
		{
		case eGAMEPARTY_OPERATOR_PLAYER:	(*m_pPlayerOpr)( pPlayer );			break;
		case eGAMEPARTY_OPERATOR_SEND:		pPlayer->SendPacket( (MSG_BASE_FORWARD*)m_pMsg, (WORD)m_Param1 );	break;
		case eGAMEPARTY_OPERATOR_INIT:		
            {
                //파티가 해제 됐다.
                pPlayer->GetInvitateFriend().ClearFriendList(InvitateFriendInfo::FRIEND_INVIATEINPARTY);
                pPlayer->GetPartyState().Init();	
            }
            break;
		case eGAMEPARTY_OPERATOR_AREA_LIVE_PARTYINFO:
			{
				if( !pPlayer->GetPosPtr() )					return;
				if( Math_VectorLength( *m_pwzPos, *pPlayer->GetPosPtr() ) > m_Param2 )
					return;
				GameZone* pGameZone = pPlayer->GetGameZonePtr();
				if( !pGameZone )							return;
				if( pGameZone->GetKey() != m_Param1 )		return;
				if( pPlayer->IsDead() )						return;

				m_Param3++;
				m_Param4 += pPlayer->GetLevel();
			}break;
		case eGAMEPARTY_OPERATOR_ADD_EXP:	pPlayer->AddExp( m_Param1 );		break;
		case eGAMEPARTY_OPERATOR_HOSTILITY_CHECK:
			{
				if (GuildSmallContentEntry::RelationSystem::IsAllHostilityRelation(
                        pPlayer->GetGuildGuid(), m_Param1))
				{
					m_SuccessFlag = FALSE;
					m_Param2 = TRUE;
				}
			}break;
        case eGAMEPARTY_OPERATOR_VALID_PLAYER_INFO_FOR_PARTY_RETURN:
            {
                GameField* pField = pPlayer->GetField();
                if(pField != 0) // 필요하면 더 검증하기로 하자.&& pPlayer->
                {
                    m_Param1 = pPlayer->GetObjectKey();
                    m_SuccessFlag = FALSE;
                    break;
                }
            }
            break;
        default: // (WAVERIX) (090316) Append default
            ASSERT(!"Invalid GamePartyOperator Type");
            break;
    	}
	}

protected:  // (WAVERIX) (090316) Change Private->Protected
	eGAMEPARTY_OPERATOR_TYPE	m_eOprType;
	DWORD						m_ExceptKey;
	BOOL						m_SuccessFlag;

	PlayerOperator *			m_pPlayerOpr;
	MSG_BASE *					m_pMsg;
	const WzVector *			m_pwzPos;
	DWORD						m_Param1;
	DWORD						m_Param2;
	DWORD						m_Param3;
	DWORD						m_Param4;
};


//{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
struct MSG_AG_ZONE_CAN_CREATE_ROOM_ACK;
namespace nsQuestPortal {
    struct EnterSynArg;
};

class GameMemberEnterConditonOperator : public GameMemberOperator<>
{
public:
    typedef GameMemberOperator<>    base_type;
    void Init(eGAMEPARTY_OPERATOR_TYPE eType, DWORD exceptKey,
        MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* pMsg, nsQuestPortal::EnterSynArg* pEnterArg)
    {
        base_type::Init(eType, exceptKey);
        m_pListMsg = pMsg;
        m_ListNumber = 0;
        m_pEnterArg = pEnterArg;
    }

    // Implementation : HANDLE_AG_ZONE.cpp, AG_ZONE_CAN_CREATE_ROOM_SYN
    void operator()(_PARTY_MEMBER_INFO* pMemberInfo);
    void operator()(Player* pPlayer, DWORD playerKey);

private:
    MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* m_pListMsg;
    WORD m_ListNumber;
    nsQuestPortal::EnterSynArg* m_pEnterArg;
};
//}

//_NA_20110630_BATTLE_ZONE_RENEWAL
struct MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK;
namespace nsPartyRoomMission{
    struct EnterSynArg;
};

class CheckMemberConditionToEnterRoomOperator : public GameMemberOperator<>
{
public:
    typedef GameMemberOperator<> base_type;

    void Init(eGAMEPARTY_OPERATOR_TYPE operator_type, 
              DWORD except_key,
              MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK* ack_msg,
              nsPartyRoomMission::EnterSynArg* enter_arg)
    {
        base_type::Init(operator_type, except_key);
        list_of_msg_ = ack_msg;
        number_of_list_ = 0;
        enter_arg_ = enter_arg;
    }

    // Implementation : HANDLE_AG_ZONE.cpp, AG_ZONE_CAN_CREATE_ROOM_SYN
    void operator()(_PARTY_MEMBER_INFO* party_member_info);

private:
    MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK* list_of_msg_;
    WORD number_of_list_;
    nsPartyRoomMission::EnterSynArg* enter_arg_;
};

//파티룸을 변경 하였을 경우 파티원의 상태를 체크 한다.
class CheckMemberConditionToChangeOptionOperator : public GameMemberOperator<>
{
public:
    typedef GameMemberOperator<> base_type;

    void Init(eGAMEPARTY_OPERATOR_TYPE operator_type, 
              DWORD except_key,
              MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* ack_msg,
              nsPartyRoomMission::EnterSynArg* enter_arg)
    {
        base_type::Init(operator_type, except_key);
        list_of_msg_ = ack_msg;
        number_of_list_ = 0;
        enter_arg_ = enter_arg;
    }

    // Implementation : HANDLE_AG_ZONE.cpp, AG_ZONE_CAN_CREATE_ROOM_SYN
    void operator()(_PARTY_MEMBER_INFO* party_member_info);

private:
    MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* list_of_msg_;
    WORD number_of_list_;
    nsPartyRoomMission::EnterSynArg* enter_arg_;
};

// used to support _KR_0_20091021_FIRENDINVITATION_EXTEND
//새로운 멤버가 파티에 들어왔을때 기존 파티원들과 친구 관계가 있는지 파악하고 설정한다.
class CheckJoinInvitateFriendInParty
{
public:
    CheckJoinInvitateFriendInParty() {}
    ~CheckJoinInvitateFriendInParty() {}

    VOID Init(Player* pPlayer)
    {
        m_pPlayer = pPlayer;
    }    

    VOID operator () (_PARTY_MEMBER_INFO *pMemberInfo)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(pMemberInfo->m_dwMemberKey);
        if (pPlayer == NULL)
        {
            return;
        }

        if(pPlayer->GetInvitateFriend().IsInvitateFriend(m_pPlayer->GetCharGuid()))
        {
            pPlayer->GetInvitateFriend().AddFriend(InvitateFriendInfo::FRIEND_INVIATEINPARTY, m_pPlayer->GetCharGuid());
            m_pPlayer->GetInvitateFriend().AddFriend(InvitateFriendInfo::FRIEND_INVIATEINPARTY, pPlayer->GetCharGuid());            
        }
    }

private:
    Player* m_pPlayer; //이 멤버가 지금 파티에 들어왔다.

};

// used to support _KR_0_20091021_FIRENDINVITATION_EXTEND
//파티에서 멤버가 떠났을때 친구관계를 파악 하고 설정한다.
class CheckLeaveInvitateFriendInParty
{
public:
    CheckLeaveInvitateFriendInParty() {}
    ~CheckLeaveInvitateFriendInParty() {}

    VOID Init(Player* pPlayer)
    {
        m_pPlayer = pPlayer;
        m_pPlayer->GetInvitateFriend().ClearFriendList(InvitateFriendInfo::FRIEND_INVIATEINPARTY);
    }    

    VOID operator () (_PARTY_MEMBER_INFO *pMemberInfo)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(pMemberInfo->m_dwMemberKey);
        ASSERT(pPlayer); if( !pPlayer ) return;

        pPlayer->GetInvitateFriend().RemoveFriend(InvitateFriendInfo::FRIEND_INVIATEINPARTY, m_pPlayer->GetCharGuid());        
    }

private:
    Player* m_pPlayer; //이 멤버가 지금 파티에서 나간다.

};
