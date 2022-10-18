#include "StdAfx.h"
#include "AgentPartyManager.h"
#include "UserSessions/User.h"
#include "AgentPartyOperator.h"
#include "AgentParty.h"
#include "Zone/PVPRoom.h"
#include "Zone/LobbyRoom.h"
#include "Zone/CHuntingRoom.h"
#include "Zone/InstanceDungeon.h"

VOID AgentPartyManager::CreateZone( User *pMaster, ZoneInterface *pCurZone, ZoneInterface *pCreateZone )
{
    if(pMaster == NULL || pCurZone == NULL || pCreateZone == NULL)
        return;

	BaseParty *pBaseParty = FindEditableParty( pMaster->GetPartyState().GetPartyKey() );

	// 존에따라 파티 처리를 다르게 한다.
	switch( pCreateZone->GetType() )
	{
	case eZONETYPE_LOBBY:
		{
			if( pBaseParty )
			{
				// 파티장이 만든 방으로 파티원들을 초대한다.
				if( pBaseParty->GetMasterKey() == pMaster->GetPlayerKey() )
				{
					if( pCurZone->GetType() == eZONETYPE_VILLAGE )
					{
						static MemberOperator Opr;
						Opr.Init( ePARTY_OPERATOR_INVITE, pMaster->GetPlayerKey(), 0 );
						Opr.SetZone( pCreateZone );
						pBaseParty->For_eachMembers( Opr );
					}
				}

				// 로비에서는 파티를 유지하지 않는다.
				LeaveParty( pMaster, eREASON_LEAVE_PARTY_NOREASON );
			}
		}break;
	case eZONETYPE_MISSION:		case eZONETYPE_HUNTING:
		{
			// 이전에 맺고 들어온 파티는 파괴한다.
			if( pBaseParty )
			{
				// 해킹 가능성 경고!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// 새로 파티를 생성한다.
			pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
			pCreateZone->LinkParty( pBaseParty->GetPartyKey() );

			// 파티를 생성한 이상 동기화를 위해 모든 유저들을 파티에 가입시킨다.
			static JoinPartyOperator Opr;
			Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
			pCurZone->Foreach( Opr );
		}break;
	case eZONETYPE_CHUNTING:
		{
			CHuntingRoom *pCHuntingRoom = (CHuntingRoom*)pCreateZone;

			// 이전에 맺고 들어온 파티는 파괴한다.
			if( pBaseParty )
			{
				// 해킹 가능성 경고!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// 새로 파티를 생성한다.
			pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
			pCHuntingRoom->LinkParty( pBaseParty->GetPartyKey() );

			static JoinPartyOperator Opr;
			Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
			pCurZone->Foreach( Opr );
		}break;
	case eZONETYPE_PVP:
		{
			LobbyRoom* pLobbyRoom = (LobbyRoom *)pCurZone;
			PVPRoom *pPVPRoom = (PVPRoom*)pCreateZone;

			// 이전에 맺고 들어온 파티는 파괴한다.
			if( pBaseParty )
			{
				// 해킹 가능성 경고!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// 팀모드에서만 파티를 설정한다.
			if( pPVPRoom->IsPVPTeamMode() )
			{
				// 2번째 팀의 파티장을 임의로 결정한다.
				User *pMaster2 = pLobbyRoom->Get2ndTeamMaster();
				if( !pMaster2 )
				{
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] 2ndTeamMaster is NULL!", pCurZone->GetType() );
					return;
				}

				// 2개의 파티를 생성한다.
				BaseParty *pBaseParty1 = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
				pPVPRoom->LinkParty( pBaseParty1->GetPartyKey() );

				BaseParty *pBaseParty2 = CreateParty( pMaster2->GetPlayerKey(), ePARTY_FIELD );
				pPVPRoom->Link2ndParty( pBaseParty2->GetPartyKey() );

				// 팀별로 파티를 배정한다. pBaseParty1, pBaseParty2
				static JoinPartyOperator Opr;
				Opr.Init( eUSER_OPERATOR_JOINPARTY_PVPROOM, pBaseParty1->GetPartyKey(), pBaseParty2->GetPartyKey() );
				Opr.SetPVPRoomTeam( pMaster->GetTeam(), pMaster2->GetTeam() );
				pLobbyRoom->Foreach( Opr );
			}

		}break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
			InstanceDungeon* const pInstanceDungeon = static_cast<InstanceDungeon*>(pCreateZone);
			switch( pInstanceDungeon->GetDungeonType() )
			{
			case INDUN_KIND_TOP_OF_TRIAL:
			case INDUN_KIND_100LEVEL_QUEST:
			case INDUN_KIND_EXT_03:
			case INDUN_KIND_EXT_04:
			case INDUN_KIND_MAYA_SCROLL:	
				{
					// 인스턴스 던전은 무조건 파티를 맺고 시작하는 것으로 한다. 현재 파티가 없다는 것은 1인 파티를 의미하게 된다.
					// 파티를 맺지 않아도 되지만 게임로직 처리시 파티위주의 처리가 많으므로 무조건 파티를 맺게하는 것이 버그가 없다.
					if( !pBaseParty )
					{
						pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
						
						static JoinPartyOperator Opr;
						Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
						Opr( pMaster );
					}

					pCreateZone->LinkParty( pBaseParty->GetPartyKey() );
                    // (WAVERIX) (090624) (CHANGES) 기존 로직의 문제점 해결 목적:
                    // insert AgentPartyManager::AddRelayUser
                    AgentPartyManager::Instance()->AddRelayUser(pInstanceDungeon);
                    g_ZoneManager.ChangeBattlePlayersCount(pInstanceDungeon,
                                                pInstanceDungeon->GetNumberOfExpectedPlayers());
				}
                break;
			case INDUN_KIND_SS_QUEST:
				{
					// 타락한사원은 유저가 생성하는 것이 아니다. 해킹 or 버그 가능성 경고!
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] INDUN_KIND_SS_QUEST Can't be Created!", pCurZone->GetType() );
				}break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE:
                {
                    SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] INDUN_KIND_CHAOS_ZONE Can't be Created!", pCurZone->GetType() );
                }break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

			default:
				{
					// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] Invalid CurZone[%u] DungeonType[%u]!", pCurZone->GetType(), pInstanceDungeon->GetDungeonType() );
				}
			}
		}break;
	case eZONETYPE_VILLAGE:	case eZONETYPE_CHARSELECT:	case eZONETYPE_QUEST:
	case eZONETYPE_EVENT:	case eZONETYPE_GUILD:		case eZONETYPE_SIEGE:
	case eZONETYPE_FIELD:	case eZONETYPE_TUTORIAL_FIELD:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
		break;
	default:
		{
			// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
			SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] Invalid CurZone[%u] CreateZone[%u]!", pCurZone->GetType(), pCreateZone->GetType() );
		}
	}
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
VOID
AgentPartyManager::EnterZoneByPortal(PORTALKEY portalKey, User* pUser,
                                     ZoneInterface* pCurZone, ZoneInterface* pToCreateZone)
{
    // (WAVERIX) (WARNING)
    //  방이 생성될 때, 파티를 생성하지는 않는다. 유저가 최초 입장할 때 생성한다.
    //  기존 파티 구성 (예: 파티장)을 유지하려면 별도 처리도 고민해야 한다.
    eZONETYPE toZoneType = pToCreateZone->GetType();
    switch(toZoneType)
    {
    case eZONETYPE_MISSION:
    case eZONETYPE_INSTANCE:
        {
            DWORD playerKey = pUser->GetPlayerKey();

            WORD partyKey = pUser->GetPartyState().GetPartyKey();
            if(partyKey)
                LeaveParty(pUser, eREASON_LEAVE_PARTY_NOREASON);

            WORD linkedPartyKey = pToCreateZone->GetLinkParty();
            BaseParty* pLinkedParty = 0;
            if(linkedPartyKey == 0)
            {
                pLinkedParty = CreateParty(playerKey, ePARTY_FIELD);
                linkedPartyKey = pLinkedParty->GetPartyKey();
                pToCreateZone->LinkParty(linkedPartyKey);
            }
            else
            {
                pLinkedParty = FindEditableParty(linkedPartyKey);
            }
            JoinParty(linkedPartyKey, pUser);

            //// 세션별 파티원 리스트를 갱신한다.
            //ServerSession* pSession = pToCreateZone->GetLinkedServer();
            //if(pSession)
            //{
            //    AgentParty* pLinkedPartyInAgent = static_cast<AgentParty*>(pLinkedParty);
            //    AddMemberToSession(pLinkedPartyInAgent,
            //        pSession->GetSessionIndex(),
            //        pUser);
            //}

            
        }
        break;
    default:
        ASSERT(!"Unexpected Status, Check Constraints in caller");
        break;
    }
}
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__


// VOID AgentPartyManager::DestroyZone( ZoneInterface *pZone )
// 방이 파괴될 때 링크된 파티도 정리를 해준다는 개념인데
// 어차피 파티를 생성하면 유저를 통해 파티가 제어된다. 방을 나가면 탈퇴, 들어오면 조인, 모두 나가면 파티는 자동으로 소멸될 것이다.

VOID AgentPartyManager::MigrateCHuntingRoom( User *pMaster, ZoneInterface *pCurZone, ZoneInterface *pEnterZone )
{
	BaseParty *pBaseParty = FindEditableParty( pMaster->GetPartyState().GetPartyKey() );

	CHuntingRoom *pCHuntingRoom = (CHuntingRoom*)pEnterZone;

	// 이전에 맺고 들어온 파티는 파괴한다.
	if( pBaseParty )
	{
		// 해킹 가능성 경고!
		SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::MigrateCHuntingRoom] CurZone[%u] EnterZone[%u] Party[%u] Already exist!", 
			pCurZone->GetType(), pEnterZone->GetType(), pBaseParty->GetMemberNum() );
		DestroyParty( pBaseParty->GetPartyKey() );
	}

	// 새로 파티를 생성한다.
	pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
	pCHuntingRoom->Link2ndParty( pBaseParty->GetPartyKey() );

	static JoinPartyOperator Opr;
	Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
	pCurZone->Foreach( Opr );
}

VOID AgentPartyManager::EnterZone( User *pUser, ZoneInterface *pEnterZone )
{
	AgentParty *pAgentParty = (AgentParty*)FindEditableParty( pUser->GetPartyState().GetPartyKey() );

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if (pAgentParty)
    {
        NotifyMemberMoveArea(pUser, pEnterZone->GetMapCode());
    }

	// 존에따라 파티 처리를 다르게 한다.
	switch( pEnterZone->GetType() )
	{
	case eZONETYPE_LOBBY:
		{
			if( pAgentParty )
			{
				// 로비에서는 파티를 유지하지 않는다.
				LeaveParty( pUser, eREASON_LEAVE_PARTY_NOREASON );
			}
		}break;
	case eZONETYPE_MISSION:		case eZONETYPE_HUNTING:		
	case eZONETYPE_CHUNTING:
		{
			if( pAgentParty )
			{
				if( pEnterZone->GetLinkedServer() )
				{
					// 세션별 파티원 리스트를 갱신한다.
					AddMemberToSession( pAgentParty, pEnterZone->GetLinkedServer()->GetSessionIndex(), pUser );
				}
			}
			else
			{
				// 파티가 없는 것에 대해 해킹 or 버그 가능성 경고!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] ZoneType[%u] Party doesn't exist!", pEnterZone->GetType() );
			}
		}break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
			InstanceDungeon* const pInstanceDungeon = static_cast<InstanceDungeon*>(pEnterZone);
			switch( pInstanceDungeon->GetDungeonType() )
			{
			case INDUN_KIND_TOP_OF_TRIAL:
			case INDUN_KIND_100LEVEL_QUEST:
			case INDUN_KIND_EXT_03:
			case INDUN_KIND_EXT_04:
			case INDUN_KIND_MAYA_SCROLL:
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE:
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
				{
					if( pAgentParty )
					{
						if( pEnterZone->GetLinkedServer() )
						{
							// 세션별 파티원 리스트를 갱신한다.
							AddMemberToSession( pAgentParty, pEnterZone->GetLinkedServer()->GetSessionIndex(), pUser );
						}
					}
					else
					{
						// 파티가 없는 것에 대해 해킹 or 버그 가능성 경고!
                        // 자유전투실에선 해당 로그를 띄울 필요가 없다(파티상태로 입장이 의무가 아님.)
                        // 일일 길드미션에선 해당 로그를 띄울 필요가 없다(파티상태로 입장이 의무가 아님.)
                        if (pEnterZone->GetMapCode() != 40051 /*자유전투실*/) {
							if(pEnterZone->GetMapCode() != 76 /*일일길드미션*/) {
								SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] ZoneType[%u] MapCode[%d] Party doesn't exist!", 
									pEnterZone->GetType(), pEnterZone->GetMapCode());
							}
                        }
					}
				}break;
			case INDUN_KIND_SS_QUEST:
				{
					// 타락한사원은 파티없이 들어온다.
					if( pAgentParty )
					{
						// 해킹 가능성 경고!
						SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] DungeonType[%u] Party[%u] Already exist!", pInstanceDungeon->GetDungeonType(), pAgentParty->GetMemberNum() );
						DestroyParty( pAgentParty->GetPartyKey() );
					}
				}break;
			default:
				{
					// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] Invalid DungeonType[%u]!", pInstanceDungeon->GetDungeonType() );
				}
			}
		}break;
	case eZONETYPE_VILLAGE:	case eZONETYPE_QUEST:
	case eZONETYPE_EVENT:	case eZONETYPE_GUILD:		case eZONETYPE_FIELD:
	case eZONETYPE_SIEGE:	case eZONETYPE_PVP:			case eZONETYPE_TUTORIAL_FIELD:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
		{
			if( pAgentParty )
			{
				if( pEnterZone->GetLinkedServer() )
				{
					// 세션별 파티원 리스트를 갱신한다.
					AddMemberToSession( pAgentParty, pEnterZone->GetLinkedServer()->GetSessionIndex(), pUser );
				}
			}
	}break;
    case eZONETYPE_CHARSELECT:
        if( pAgentParty )
        {
            LeaveParty( pUser, eREASON_LEAVE_PARTY_NOREASON );
        }
        break;
	default:
		{
			// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
			SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] Invalid ZoneType[%u]!", pEnterZone->GetType() );
		}
	}
}

VOID AgentPartyManager::LeaveZone(User* user, ZoneInterface* leave_zone)
{
    WORD party_key = user->GetPartyState().GetPartyKey();
	AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
	if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

	eZONETYPE from_zone_type;	
    eZONETYPE to_zone_type;	
    DWORD to_link_server_index;
    user->GetMoveZoneType(from_zone_type, to_zone_type, to_link_server_index);

	switch (leave_zone->GetType())
	{
	case eZONETYPE_LOBBY://Lobby -> To Zone
		{
			switch( to_zone_type )
			{
			case eZONETYPE_MISSION:
            case eZONETYPE_HUNTING:
			case eZONETYPE_CHUNTING:
            case eZONETYPE_PVP:
				{
					if (leave_zone->GetLinkedServer())
					{
						// 세션별 파티원 리스트를 갱신한다.
						RemoveMemberToSession(party, leave_zone->GetLinkedServerIndex(), 
                                              to_link_server_index, user);
					}
				}
                break;
			case eZONETYPE_VILLAGE:
            case eZONETYPE_CHARSELECT:
            case eZONETYPE_QUEST:
            case eZONETYPE_EVENT:
            case eZONETYPE_GUILD:
            case eZONETYPE_FIELD:
			case eZONETYPE_SIEGE:
            case eZONETYPE_TUTORIAL_FIELD:
            case eZONETYPE_DOMINATION_FIELD:
            case eZONETYPE_SPA_FIELD:
				{
					LeaveParty(user, eREASON_LEAVE_PARTY_INTEND);
				}
                break;
			case eZONETYPE_INSTANCE:
				{
                    SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Can't move From Lobby To instance zone" );
					DestroyParty(party->GetPartyKey());
				}
                break;
			default:
				{
                    SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Invalid Zone Type from lobby", 
                           to_zone_type );
				}
			}
		}break;

	case eZONETYPE_MISSION:		
    case eZONETYPE_HUNTING:
	case eZONETYPE_CHUNTING:	
    case eZONETYPE_PVP:
		{
            if (party->GetType() == ePARTY_FIELD)
            {
                LeaveParty( user, eREASON_LEAVE_PARTY_INTEND );
            }

            if (leave_zone->GetLinkedServer())
            {
                RemoveMemberToSession(party, leave_zone->GetLinkedServerIndex(), to_link_server_index, user);
            }
		}
        break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
			InstanceDungeon* const pInstanceDungeon = static_cast<InstanceDungeon*>(leave_zone);
			switch( pInstanceDungeon->GetDungeonType() )
			{
			case INDUN_KIND_TOP_OF_TRIAL:
			case INDUN_KIND_100LEVEL_QUEST:
			case INDUN_KIND_EXT_03:
			case INDUN_KIND_EXT_04:
			case INDUN_KIND_MAYA_SCROLL:
				{
                    if (party->GetType() == ePARTY_FIELD)
                    {
                        LeaveParty( user, eREASON_LEAVE_PARTY_INTEND );
                    }
				}break;
			case INDUN_KIND_SS_QUEST:
				{
					// 타락한사원은 파티없이 들어온다. 경고 메세지를 남긴다.
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::LeaveZone] DungeonType[%u] Party[%u] Already exist!", pInstanceDungeon->GetDungeonType(), party->GetMemberNum() );
					DestroyParty(party->GetPartyKey());
				}break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE:
                {
                    LeaveParty(user, eREASON_LEAVE_PARTY_INTEND);
                }break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

			default:
				{
					// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::LeaveZone] Invalid DungeonType[%u]!", pInstanceDungeon->GetDungeonType() );
				}
			}

            if (leave_zone->GetLinkedServer())
            {
                RemoveMemberToSession(party, leave_zone->GetLinkedServerIndex(), to_link_server_index, user);
            }
		}break;
	case eZONETYPE_VILLAGE:	case eZONETYPE_CHARSELECT:	case eZONETYPE_QUEST:
	case eZONETYPE_EVENT:	case eZONETYPE_GUILD:		case eZONETYPE_FIELD:
	case eZONETYPE_SIEGE:	case eZONETYPE_TUTORIAL_FIELD:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
		{
			// 캐릭터 선택화면으로 갈 때를 제외하고 기존 파티 유지!
			if( to_zone_type == eZONETYPE_CHARSELECT )
			{
				LeaveParty( user, eREASON_LEAVE_PARTY_INTEND );
			}
			else if(leave_zone->GetLinkedServer() )
			{
				// 세션별 파티원 리스트를 갱신한다.
				RemoveMemberToSession(party, leave_zone->GetLinkedServerIndex(), to_link_server_index, user);
			}
		}break;
	default:
		{
			// 예상하지 못한 존타입이라고 경고 메세지를 남긴다.
			SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::LeaveZone] Invalid ZoneType[%u]!", leave_zone->GetType() );
		}
	}
}


