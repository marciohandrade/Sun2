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

	// �������� ��Ƽ ó���� �ٸ��� �Ѵ�.
	switch( pCreateZone->GetType() )
	{
	case eZONETYPE_LOBBY:
		{
			if( pBaseParty )
			{
				// ��Ƽ���� ���� ������ ��Ƽ������ �ʴ��Ѵ�.
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

				// �κ񿡼��� ��Ƽ�� �������� �ʴ´�.
				LeaveParty( pMaster, eREASON_LEAVE_PARTY_NOREASON );
			}
		}break;
	case eZONETYPE_MISSION:		case eZONETYPE_HUNTING:
		{
			// ������ �ΰ� ���� ��Ƽ�� �ı��Ѵ�.
			if( pBaseParty )
			{
				// ��ŷ ���ɼ� ���!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// ���� ��Ƽ�� �����Ѵ�.
			pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
			pCreateZone->LinkParty( pBaseParty->GetPartyKey() );

			// ��Ƽ�� ������ �̻� ����ȭ�� ���� ��� �������� ��Ƽ�� ���Խ�Ų��.
			static JoinPartyOperator Opr;
			Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
			pCurZone->Foreach( Opr );
		}break;
	case eZONETYPE_CHUNTING:
		{
			CHuntingRoom *pCHuntingRoom = (CHuntingRoom*)pCreateZone;

			// ������ �ΰ� ���� ��Ƽ�� �ı��Ѵ�.
			if( pBaseParty )
			{
				// ��ŷ ���ɼ� ���!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// ���� ��Ƽ�� �����Ѵ�.
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

			// ������ �ΰ� ���� ��Ƽ�� �ı��Ѵ�.
			if( pBaseParty )
			{
				// ��ŷ ���ɼ� ���!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] CreateZone[%u] Party[%u] Already exist!", 
					pCurZone->GetType(), pCreateZone->GetType(), pBaseParty->GetMemberNum() );
				DestroyParty( pBaseParty->GetPartyKey() );
			}

			// ����忡���� ��Ƽ�� �����Ѵ�.
			if( pPVPRoom->IsPVPTeamMode() )
			{
				// 2��° ���� ��Ƽ���� ���Ƿ� �����Ѵ�.
				User *pMaster2 = pLobbyRoom->Get2ndTeamMaster();
				if( !pMaster2 )
				{
					SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::CreateZone] CurZone[%u] 2ndTeamMaster is NULL!", pCurZone->GetType() );
					return;
				}

				// 2���� ��Ƽ�� �����Ѵ�.
				BaseParty *pBaseParty1 = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
				pPVPRoom->LinkParty( pBaseParty1->GetPartyKey() );

				BaseParty *pBaseParty2 = CreateParty( pMaster2->GetPlayerKey(), ePARTY_FIELD );
				pPVPRoom->Link2ndParty( pBaseParty2->GetPartyKey() );

				// ������ ��Ƽ�� �����Ѵ�. pBaseParty1, pBaseParty2
				static JoinPartyOperator Opr;
				Opr.Init( eUSER_OPERATOR_JOINPARTY_PVPROOM, pBaseParty1->GetPartyKey(), pBaseParty2->GetPartyKey() );
				Opr.SetPVPRoomTeam( pMaster->GetTeam(), pMaster2->GetTeam() );
				pLobbyRoom->Foreach( Opr );
			}

		}break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// �߰� Ÿ�� ����� �ϴ� ���� �߰��� ��
			InstanceDungeon* const pInstanceDungeon = static_cast<InstanceDungeon*>(pCreateZone);
			switch( pInstanceDungeon->GetDungeonType() )
			{
			case INDUN_KIND_TOP_OF_TRIAL:
			case INDUN_KIND_100LEVEL_QUEST:
			case INDUN_KIND_EXT_03:
			case INDUN_KIND_EXT_04:
			case INDUN_KIND_MAYA_SCROLL:	
				{
					// �ν��Ͻ� ������ ������ ��Ƽ�� �ΰ� �����ϴ� ������ �Ѵ�. ���� ��Ƽ�� ���ٴ� ���� 1�� ��Ƽ�� �ǹ��ϰ� �ȴ�.
					// ��Ƽ�� ���� �ʾƵ� ������ ���ӷ��� ó���� ��Ƽ������ ó���� �����Ƿ� ������ ��Ƽ�� �ΰ��ϴ� ���� ���װ� ����.
					if( !pBaseParty )
					{
						pBaseParty = CreateParty( pMaster->GetPlayerKey(), ePARTY_FIELD );
						
						static JoinPartyOperator Opr;
						Opr.Init( eUSER_OPERATOR_JOINPARTY, pBaseParty->GetPartyKey() );
						Opr( pMaster );
					}

					pCreateZone->LinkParty( pBaseParty->GetPartyKey() );
                    // (WAVERIX) (090624) (CHANGES) ���� ������ ������ �ذ� ����:
                    // insert AgentPartyManager::AddRelayUser
                    AgentPartyManager::Instance()->AddRelayUser(pInstanceDungeon);
                    g_ZoneManager.ChangeBattlePlayersCount(pInstanceDungeon,
                                                pInstanceDungeon->GetNumberOfExpectedPlayers());
				}
                break;
			case INDUN_KIND_SS_QUEST:
				{
					// Ÿ���ѻ���� ������ �����ϴ� ���� �ƴϴ�. ��ŷ or ���� ���ɼ� ���!
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
					// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
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
			// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
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
    //  ���� ������ ��, ��Ƽ�� ���������� �ʴ´�. ������ ���� ������ �� �����Ѵ�.
    //  ���� ��Ƽ ���� (��: ��Ƽ��)�� �����Ϸ��� ���� ó���� ����ؾ� �Ѵ�.
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

            //// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
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
// ���� �ı��� �� ��ũ�� ��Ƽ�� ������ ���شٴ� �����ε�
// ������ ��Ƽ�� �����ϸ� ������ ���� ��Ƽ�� ����ȴ�. ���� ������ Ż��, ������ ����, ��� ������ ��Ƽ�� �ڵ����� �Ҹ�� ���̴�.

VOID AgentPartyManager::MigrateCHuntingRoom( User *pMaster, ZoneInterface *pCurZone, ZoneInterface *pEnterZone )
{
	BaseParty *pBaseParty = FindEditableParty( pMaster->GetPartyState().GetPartyKey() );

	CHuntingRoom *pCHuntingRoom = (CHuntingRoom*)pEnterZone;

	// ������ �ΰ� ���� ��Ƽ�� �ı��Ѵ�.
	if( pBaseParty )
	{
		// ��ŷ ���ɼ� ���!
		SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::MigrateCHuntingRoom] CurZone[%u] EnterZone[%u] Party[%u] Already exist!", 
			pCurZone->GetType(), pEnterZone->GetType(), pBaseParty->GetMemberNum() );
		DestroyParty( pBaseParty->GetPartyKey() );
	}

	// ���� ��Ƽ�� �����Ѵ�.
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

	// �������� ��Ƽ ó���� �ٸ��� �Ѵ�.
	switch( pEnterZone->GetType() )
	{
	case eZONETYPE_LOBBY:
		{
			if( pAgentParty )
			{
				// �κ񿡼��� ��Ƽ�� �������� �ʴ´�.
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
					// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
					AddMemberToSession( pAgentParty, pEnterZone->GetLinkedServer()->GetSessionIndex(), pUser );
				}
			}
			else
			{
				// ��Ƽ�� ���� �Ϳ� ���� ��ŷ or ���� ���ɼ� ���!
				SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] ZoneType[%u] Party doesn't exist!", pEnterZone->GetType() );
			}
		}break;
	case eZONETYPE_INSTANCE:
		{
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// �߰� Ÿ�� ����� �ϴ� ���� �߰��� ��
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
							// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
							AddMemberToSession( pAgentParty, pEnterZone->GetLinkedServer()->GetSessionIndex(), pUser );
						}
					}
					else
					{
						// ��Ƽ�� ���� �Ϳ� ���� ��ŷ or ���� ���ɼ� ���!
                        // ���������ǿ��� �ش� �α׸� ��� �ʿ䰡 ����(��Ƽ���·� ������ �ǹ��� �ƴ�.)
                        // ���� ���̼ǿ��� �ش� �α׸� ��� �ʿ䰡 ����(��Ƽ���·� ������ �ǹ��� �ƴ�.)
                        if (pEnterZone->GetMapCode() != 40051 /*����������*/) {
							if(pEnterZone->GetMapCode() != 76 /*���ϱ��̼�*/) {
								SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] ZoneType[%u] MapCode[%d] Party doesn't exist!", 
									pEnterZone->GetType(), pEnterZone->GetMapCode());
							}
                        }
					}
				}break;
			case INDUN_KIND_SS_QUEST:
				{
					// Ÿ���ѻ���� ��Ƽ���� ���´�.
					if( pAgentParty )
					{
						// ��ŷ ���ɼ� ���!
						SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::EnterZone] DungeonType[%u] Party[%u] Already exist!", pInstanceDungeon->GetDungeonType(), pAgentParty->GetMemberNum() );
						DestroyParty( pAgentParty->GetPartyKey() );
					}
				}break;
			default:
				{
					// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
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
					// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
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
			// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
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
						// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
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
			BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// �߰� Ÿ�� ����� �ϴ� ���� �߰��� ��
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
					// Ÿ���ѻ���� ��Ƽ���� ���´�. ��� �޼����� �����.
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
					// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
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
			// ĳ���� ����ȭ������ �� ���� �����ϰ� ���� ��Ƽ ����!
			if( to_zone_type == eZONETYPE_CHARSELECT )
			{
				LeaveParty( user, eREASON_LEAVE_PARTY_INTEND );
			}
			else if(leave_zone->GetLinkedServer() )
			{
				// ���Ǻ� ��Ƽ�� ����Ʈ�� �����Ѵ�.
				RemoveMemberToSession(party, leave_zone->GetLinkedServerIndex(), to_link_server_index, user);
			}
		}break;
	default:
		{
			// �������� ���� ��Ÿ���̶�� ��� �޼����� �����.
			SUNLOG( eCRITICAL_LOG, "[AgentPartyManager::LeaveZone] Invalid ZoneType[%u]!", leave_zone->GetType() );
		}
	}
}


