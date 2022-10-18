//------------------------------------------------------------------------------
//  uiBattleZoneMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneMan.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneReadyMissionConfig/uiBattleZoneReadyMissionConfig.h"
#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"
#include "VillageScene.h"
#include "BattleLobby.h"
#include "UserSlot.h"
#include "ObjectManager.h"
#include "LoadScene.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "EventInfoParser.h"
#include "ResultCode.h"
#include "MissionScene.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "Map.h"
#include "uiPartyMan/uiPartyMan.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif//_DH_BATTLEZONE2_
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_ZONE:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_ZONE_LOBBY_CREATE_ACK:
                {
                    this->NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_ACK(pMsg);
                }
                break;

            case CG_ZONE_LOBBY_CREATE_NAK:
                {
                    this->NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_NAK(pMsg);
                }
                break;

// change map
            case CG_ZONE_LOBBY_CHANGE_MAP_ACK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN);

					if(BattleScene::IsLobby())
					{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
						m_pUIBattleZoneReadyMissionConfig->SetCfg_Field(m_pendingMapChangeIndex);
                        m_pUIBattleZoneReadyCHuntingConfig->SetCfg_Field(m_pendingMapChangeIndex);
#endif
						GetBattleRoomEntry().SetMapCode( m_PendingMapCode );
						BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );
					}
                }
                break;

            case CG_ZONE_LOBBY_CHANGE_MAP_NAK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHANGE_MAP_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_MAP_NAK*)pMsg;

						m_pendingMapChangeIndex = 0;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
                }
                break;

            case CG_ZONE_LOBBY_CHANGE_MAP_BRD:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHANGE_MAP_BRD* recv =
							(MSG_CG_ZONE_LOBBY_CHANGE_MAP_BRD *)pMsg;

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
						m_pUIBattleZoneReadyMissionConfig->SetCfg_FieldByMapCode(recv->m_MapCode);
						m_pUIBattleZoneReadyCHuntingConfig->SetCfg_FieldByMapCode(recv->m_MapCode);
#endif //#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
						GetBattleRoomEntry().SetMapCode( recv->m_MapCode);
						BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );
					}
                }
                break;



			// competition hunting config
            case CG_ZONE_LOBBY_CHUNTING_CONFIG_ACK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN);

                    // very care
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_Diff(m_pendingDifficult);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_Bonus(m_pendingBonus);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompMode(m_pendingCompetitionMode);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompRule(m_pendingCompetitionRule);
#endif //ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES 
                }
                break;
            case CG_ZONE_LOBBY_CHUNTING_CONFIG_NAK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN);

#ifdef _DEBUG
                    GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CHAT, _T("LOBBY_HUNTING_CONFIG_NAK!!"));
#endif
                }
                break;
            case CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN);

                    MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD* recv =
                        (MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD *)pMsg;
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                    // pendings wasn't saved
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_Diff((eHUNTING_MONSTER_DIFFICULT)recv->m_AdditionalInfo.m_Difficulty);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_Bonus((eHUNTING_BONUS_TYPE)recv->m_AdditionalInfo.m_Bonus);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompMode((eCOMPETITION_MODE_TYPE)recv->m_AdditionalInfo.m_Mode);
                    m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompRule((eCOMPETITION_RULE_TYPE)recv->m_AdditionalInfo.m_Rule);
#endif//#ifndef ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
                }
                break;

// shorcut transfer
            case CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK: { NET_RECV_MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK(pMsg); } break;
            case CG_ZONE_SHORTCUT_PVPLOBBY_ACK:  { NET_RECV_MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK(pMsg); } break;
            case CG_ZONE_SHORTCUT_CHUNTINGLOBBY_ACK: { NET_RECV_MSG_CG_ZONE_SHORTCUT_CHUNTING_ACK(pMsg); } break;
            case CG_ZONE_SHORTCUT_NAK: { NET_RECV_MSG_CG_ZONE_SHORTCUT_NAK(pMsg); } break;

// zone listup, lookup
            case CG_ZONE_LIST_MISSIONLOBBY_ACK: { NET_RECV_MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK(pMsg); } break;
            case CG_ZONE_LIST_PVPLOBBY_ACK: { NET_RECV_MSG_CG_ZONE_LIST_PVPLOBBY_ACK(pMsg); } break;
            case CG_ZONE_LIST_CHUNTINGLOBBY_ACK: { NET_RECV_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK(pMsg); } break;

// zone listup, naks
            case CG_ZONE_LIST_MISSIONLOBBY_NAK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN);
                    NET_RECV_MSG_CG_ZONE_LIST_NAK(pMsg);
                }
                break;


            case CG_ZONE_LIST_PVPLOBBY_NAK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN);
                    NET_RECV_MSG_CG_ZONE_LIST_NAK(pMsg);
                }
                break;

			case CG_ZONE_LIST_CHUNTINGLOBBY_NAK:
                {
                    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN);
                    NET_RECV_MSG_CG_ZONE_LIST_NAK(pMsg);
                }
                break;


            case CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK:
                {
					ClearZoneListItems();
					SetSearchMode(true);
					
					MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK* recv =
                        (MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK*)pMsg;

					m_searchedZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY;
					m_searchedZoneListItem.m_MissionLobbyRoomInfo = recv->m_RoomInfo;

                }
                break;


            case CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK:
                {
					ClearZoneListItems();
					SetSearchMode(true);
					//PVP_ROOMINFO
					MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK* recv =
                        (MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK*)pMsg;

					m_searchedZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_PVP;
					m_searchedZoneListItem.m_PVPRoomInfo = recv->m_RoomInfo;
                }
                break;

			case CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK:
				{
					ClearZoneListItems();
					SetSearchMode(true);
					
					MSG_CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK* recv =
                        (MSG_CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK*)pMsg;


					m_searchedZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY;
					m_searchedZoneListItem.m_CHuntingLobbyRoomInfo = recv->m_RoomInfo;
				}
				break;


            case CG_ZONE_FIND_ROOM_FROM_CHARID_NAK:
                {
					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

					g_InterfaceManager.GetInterfaceString(93, szMessage, INTERFACE_STRING_LENGTH);

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_VerifyLock(szMessage);

                }
                break;


			case CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD:
				{
					MSG_CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD* recv = 
						(MSG_CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD*)pMsg;

					SetNonBlockSlot(recv->m_byMaxLimitUser);
			
				}
				break;


            case CG_ZONE_CHUNTING_JOIN_ACK:
                {
					MSG_CG_ZONE_CHUNTING_JOIN_ACK* recv =
                        (MSG_CG_ZONE_CHUNTING_JOIN_ACK*)pMsg;

					if (recv->m_dwMasterObjKey)
                    {
						SetLobbyMasterPlayerKey(recv->m_dwMasterObjKey);
                    }

                    g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHUNTING, recv->m_MapCode, 0);

					BattleScene::SetLobbyFlag(FALSE);
					BattleScene::SetLobbyInPending(FALSE);

					UnsetPKBool(PACKET_STATUS_SEND_CREATE);

                    GameFramework::ChangeScene(SCENE_TYPE_LOAD);

                    m_pUIBattleZoneList->ShowInterface(FALSE);
                }
                break;

			case CG_ZONE_CHUNTING_JOIN_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CREATE);

					MSG_CG_ZONE_CHUNTING_JOIN_NAK* recv =
						(MSG_CG_ZONE_CHUNTING_JOIN_NAK*)pMsg;

					WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("recv CG_ZONE_CHUNTING_JOIN_NAK: mapcode(%d)"), recv->m_byErrorCode);

					g_InterfaceManager.GetStringZoneError(recv->m_byErrorCode, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);

				}
				break;


            case CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK:
                {
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK* recv =
							(MSG_CG_ZONE_LOBBY_CHUNTING_REQUEST_INFO_ACK*)pMsg;

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
						m_pUIBattleZoneReadyCHuntingConfig->SetCfg_FieldByMapCode(recv->m_MapCode);
						m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompRule((eCOMPETITION_RULE_TYPE)recv->m_AdditionalCHuntingInfo.m_Rule);
						m_pUIBattleZoneReadyCHuntingConfig->SetCfg_CompMode((eCOMPETITION_MODE_TYPE)recv->m_AdditionalCHuntingInfo.m_Mode);
#endif//#ifndef ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

						SetNonBlockSlot(recv->m_NonBlockNum);

						for (int i = 0; i < recv->m_Count; ++i)
						{
							SetReadyUserSlot(recv->m_dwReadyPlayerKey[i], true);
						}

						GetBattleRoomEntry().SetMapCode(recv->m_MapCode);
						GetBattleRoomEntry().SetAdditionalCHuntingInfo(&recv->m_AdditionalCHuntingInfo);
						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
                }
                break;

			case CG_ZONE_LOBBY_JOIN_NAK:
				{
					BattleRoomEntry* entry = BattleScene::GetPendingBattleRoomEntry();

					if (entry)
					{
						std::vector<cBattleZoneListItem>::iterator  iter;

						for (iter = m_zoneListItem.begin(); iter != m_zoneListItem.end(); ++iter)
						{
							if ((*iter) == static_cast<KEYTYPE>( entry->GetSerial()) )
							{
								m_zoneListItem.erase (iter);
								break;
							}
						}
					}
				}
				break;


			case CG_ZONE_LIST_REFRESH_MISSIONLOBBY_ACK:
				{
					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan && pCommMan->IsWaitJoinGame())
					{
						//MISSION_ROOMINFO	m_RoomInfo;
						MSG_CG_ZONE_LIST_REFRESH_MISSIONLOBBY_ACK *pRecvPacket = (MSG_CG_ZONE_LIST_REFRESH_MISSIONLOBBY_ACK*)pMsg;
						
						m_friendJoinLobby.iType = -1; // clear
						m_friendJoinLobby.setM(pRecvPacket->m_RoomInfo);

						if(pRecvPacket->m_RoomInfo.m_byRoomPublic == eZONEPUBLIC_PRIVATE)
						{
							// 비번 입력 창을 show
							if(m_pUIBattleZonePassword)
							{
								m_pPrivateZone = NULL;
								m_pPrivateZone = &m_friendJoinLobby;
								ShowPasswordDlg(true);
							}
						}
						else
						{
							RequestJoinZone(&m_friendJoinLobby);
						}

						pCommMan->SetWaitJoinGame(false);
					}
				}
				break;

			case CG_ZONE_LIST_REFRESH_PVPLOBBY_ACK:
				{
					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan && pCommMan->IsWaitJoinGame())
					{
						//PVP_ROOMINFO		m_RoomInfo;
						MSG_CG_ZONE_LIST_REFRESH_PVPLOBBY_ACK *pRecvPacket = (MSG_CG_ZONE_LIST_REFRESH_PVPLOBBY_ACK*)pMsg;

						m_friendJoinLobby.iType = -1; // clear
						m_friendJoinLobby.setP(pRecvPacket->m_RoomInfo);

						if(pRecvPacket->m_RoomInfo.m_byRoomPublic == eZONEPUBLIC_PRIVATE)
						{
							// 비번 입력 창을 show
							if(m_pUIBattleZonePassword)
							{
								m_pPrivateZone = NULL;
								m_pPrivateZone = &m_friendJoinLobby;
								ShowPasswordDlg(true);
							}
						}
						else
						{
							RequestJoinZone(&m_friendJoinLobby);
						}

						pCommMan->SetWaitJoinGame(false);
					}
				}
				break;

			case CG_ZONE_LIST_REFRESH_CHUNTINGLOBBY_ACK:
				{
					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan && pCommMan->IsWaitJoinGame())
					{
						MSG_CG_ZONE_LIST_REFRESH_CHUNTINGLOBBY_ACK *pRecvPacket = (MSG_CG_ZONE_LIST_REFRESH_CHUNTINGLOBBY_ACK*)pMsg;
						
						m_friendJoinLobby.iType = -1; // clear
						m_friendJoinLobby.setCH (pRecvPacket->m_RoomInfo, 0);

						if(pRecvPacket->m_RoomInfo.m_byRoomPublic == eZONEPUBLIC_PRIVATE)
						{
							// 비번 입력 창을 show
							if(m_pUIBattleZonePassword)
							{
								m_pPrivateZone = NULL;
								m_pPrivateZone = &m_friendJoinLobby;
								ShowPasswordDlg(true);
							}
						}
						else
						{
							RequestJoinZone(&m_friendJoinLobby);
						}

						pCommMan->SetWaitJoinGame(false);
					}
				}
				break;

			case CG_ZONE_LIST_REFRESH_NONE_ACK:
				{
					MSG_CG_ZONE_LIST_REFRESH_NONE_ACK *pRecvPacket = (MSG_CG_ZONE_LIST_REFRESH_NONE_ACK*)pMsg;

					m_friendJoinLobby.iType = -1; // clear

					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan)
					{
						pCommMan->SetWaitJoinGame(false);
						pCommMan->SetRqstJoinGame(false);
					}

					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString( 93, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case CG_ZONE_LIST_REFRESH_NAK:
				{
					//DWORD				m_dwErrorCode;
					MSG_CG_ZONE_LIST_REFRESH_NAK *pRecvPacket = (MSG_CG_ZONE_LIST_REFRESH_NAK*)pMsg;
					
					m_friendJoinLobby.iType = -1; // clear

					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan)
					{
						pCommMan->SetWaitJoinGame(false);
						pCommMan->SetRqstJoinGame(false);
					}

					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString( 93, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;


			case CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_MISSION_REQUEST_INFO_ACK *)pMsg;

						SetNonBlockSlot(pRecvMsg->m_NonBlockNum);

						for (int i = 0; i < pRecvMsg->m_Count; ++i)
						{
							SetReadyUserSlot(pRecvMsg->m_dwReadyPlayerKey[i], true);
						}

						GetBattleRoomEntry().SetMapCode( pRecvMsg->m_MapCode );
						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
				}
				break;

			case CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_PVP_REQUEST_INFO_ACK *)pMsg;

						SetNonBlockSlot(pRecvMsg->m_NonBlockNum);

						for (int i = 0; i < pRecvMsg->m_Count; ++i)
						{

							if ( pRecvMsg->m_PlayerInfo[i].m_Ready )
							{
								SetReadyUserSlot(pRecvMsg->m_PlayerInfo[i].m_dwPlayerKey, true);
							}

							SetTeamIndexUserSlot(pRecvMsg->m_PlayerInfo[i].m_dwPlayerKey, pRecvMsg->m_PlayerInfo[i].m_Team);
							if (g_pHero && pRecvMsg->m_PlayerInfo[i].m_dwPlayerKey == g_pHero->GetObjectKey())
							{
								uiBattleScoreMan* pBattleScoreMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

								if(pBattleScoreMan)
								{
									pBattleScoreMan->SetHeroTeam(pRecvMsg->m_PlayerInfo[i].m_Team);
								}
							}
						}

						// 확인
						GetBattleRoomEntry().SetAdditionalPVPInfo(&pRecvMsg->m_AdditionalPVPInfo);
						const sFIELDINFO * pFieldInfo = GetBattleRoomEntry().GetCurFieldInfo();
						assert(pFieldInfo);
						GetBattleRoomEntry().SetMapCode( pRecvMsg->m_MapCode );
						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
				}
				break;

			case CG_ZONE_LOBBY_NONBLOCKSLOTNUM_BRD:		//< 구성원들에게 전송
				{
					UnsetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);

					if(BattleScene::IsLobby())
					{
						// 마스터가 아닐때만 의미가 있다.
						// 최고 인원수에 따라 뒤에서부터 블럭해나간다.
						if(IsMaster())
							return;

						MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_BRD *)pMsg;

						SetNonBlockSlot(pRecvMsg->m_NonBlockSlotNum);

						GetBattleRoomEntry().SetMaxPlayer(pRecvMsg->m_NonBlockSlotNum);
						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
				}
				break;

			case CG_ZONE_MASTER_CHANGED_BRD:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_MASTER_CHANGED_BRD * pRecvMsg = (MSG_CG_ZONE_MASTER_CHANGED_BRD *)pMsg;

						int iTeam = 0;
						UserSlot * pUserSlot = this->m_UserSlotSet.GetUserByKey( pRecvMsg->m_dwMasterPlayerKey );
						if ( pUserSlot )
						{
							iTeam = pUserSlot->GetTeam();
						}
						
						LeavePlayer(GetLobbyMasterPlayerKey());	
						LeavePlayer(pRecvMsg->m_dwMasterPlayerKey);

						// 방장 등록 
						SetLobbyMasterPlayerKey(pRecvMsg->m_dwMasterPlayerKey);
					
						UserSlot EnterUser;
						EnterUser.SetPlayerKey(pRecvMsg->m_dwMasterPlayerKey);
						AddPlayer(EnterUser);

						SetTeamIndexUserSlot(pRecvMsg->m_dwMasterPlayerKey, iTeam);
					}
				}
				break;

			// 플레이어 강퇴
			case CG_ZONE_LOBBY_KICKPLAYER_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_BAN);
					// 여기 들어왔다면 강퇴 당한것이다.

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_KICKPLAYER_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_KICKPLAYER_ACK *)pMsg;
					}
					
				}
				break;

			case CG_ZONE_LOBBY_KICKPLAYER_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_BAN);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_KICKPLAYER_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_KICKPLAYER_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
					
				}
				break;

			case CG_ZONE_LOBBY_LEAVE_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_LEAVE);

					assert(BattleScene::IsLobby());
					if(BattleScene::IsLobby())
					{					
						assert(g_pHero);
						if(g_pHero)
						{
							uiBattleScoreMan* pBattleScoreMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

							if(pBattleScoreMan)
							{
								pBattleScoreMan->SetHeroTeam(0);
							}
						}

						MSG_CG_ZONE_LOBBY_LEAVE_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_LEAVE_ACK *)pMsg;
						
						BattleScene::SetLobbyFlag(FALSE);
						BattleScene::SetLobbyOutPending(TRUE);

						if(m_pBattleLobby)
							m_pBattleLobby->ShowWindow(FALSE);

						if(g_pMap->IsMission())//미션 재시도 관련일 경우 마을 로딩.
						{		
							BattleScene::m_bPendingBackToSomeWhere = false;
							g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_VILLAGE, g_pHero->GetCharInfo()->m_iRegion, 0);
							GameFramework::ChangeScene(SCENE_TYPE_LOAD);							
							g_InterfaceManager.ShowGameInterface(TRUE);

						}
						else
						{
							// 멥오브젝트와 NPC 만 제외하고 delete 후 enter syn를 날려줌.
							g_ObjectManager.DeleteObjectsByType(PLAYER_OBJECT);
							g_ObjectManager.DeleteObjectsByType(MONSTER_OBJECT);
							g_ObjectManager.DeleteObjectsByType(NPC_OBJECT);

							
							BattleScene::SendPlayerEnterSyn(TRUE);
						}

						g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
		           
					}
				}
				break;

			case CG_ZONE_LOBBY_LEAVE_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_LEAVE);

					if(BattleScene::IsLobby())
					{	
						MSG_CG_ZONE_LOBBY_LEAVE_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_LEAVE_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
						g_InterfaceManager.NetworkProc(InterfaceManager::DIALOG_SYSTEM, pMsg);
					}
				}
				break;

			// [구성원의 권한]

			// 구성원이 레디 버튼을 누른다.
			case CG_ZONE_LOBBY_READY_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_ACK *)pMsg;

						assert(g_pHero);
						assert(!IsMaster());

						SetReadyUserSlot(g_pHero->GetObjectKey(), true);

						TCHAR szStaticText[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString(302, szStaticText, INTERFACE_STRING_LENGTH);
						GlobalFunc::SetCaption( m_pBattleLobby->GetCtrlTIStartNReady(), szStaticText );
					}
				}
				break;

			case CG_ZONE_LOBBY_READY_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}

				}
				break;

			case CG_ZONE_LOBBY_READY_BRD:				//< 구성원들에게 전송
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_BRD *)pMsg;
						assert(g_pHero);
						SetReadyUserSlot(pRecvMsg->m_PlayerKey, true);
					}
				}
				break;

			case CG_ZONE_LOBBY_READY_CANCEL_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY_CANCLE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_CANCEL_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_CANCEL_ACK *)pMsg;
					
						if(g_pHero)
							SetReadyUserSlot(g_pHero->GetObjectKey(), false);

						TCHAR szStaticText[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString(110, szStaticText, INTERFACE_STRING_LENGTH);
						GlobalFunc::SetCaption( m_pBattleLobby->GetCtrlTIStartNReady(), szStaticText );
					}
				}
				break;

			case CG_ZONE_LOBBY_READY_CANCEL_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY_CANCLE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_CANCEL_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_CANCEL_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}

				}
				break;

			case CG_ZONE_LOBBY_READY_CANCEL_BRD:
				{
					UnsetPKBool(PACKET_STATUS_SEND_READY_CANCLE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_READY_CANCEL_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_CANCEL_BRD *)pMsg;
						SetReadyUserSlot(pRecvMsg->m_PlayerKey, false);
					}
					
				}
				break;

			// [방장의 권한]
			// 인원수 변경

			// 인원수가 변경될때 방장한테만 현재 slot 위치의 block 정보를 제대로 보여주고
			// 타 유저에게는 블럭된 슬롯은 뒤에 차곡차곡 쌓인다(기획과 협의했음!!)
			case CG_ZONE_LOBBY_NONBLOCKSLOTNUM_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_ACK *)pMsg;
						
						// 인원수 셋팅
						if(!IsMaster())
							return;
					
						SetBlockUserSlot();

						GetBattleRoomEntry().SetMaxPlayer(m_iPendingPlayerNumBeforeRecvAck);
						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
				}
				break;

			case CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;



			// 팀 변경
			case CG_ZONE_LOBBY_CHANGE_TEAM_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CHANGE_TEAM);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHANGE_TEAM_ACK * pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_TEAM_ACK *)pMsg;

						if(g_pHero)
						{
							SetTeamUserSlot(g_pHero->GetObjectKey(), m_PendingiTeam);
						}
					}
				}
				break;

			case CG_ZONE_LOBBY_CHANGE_TEAM_BRD:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHANGE_TEAM_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_TEAM_BRD *)pMsg;
						
						SetTeamUserSlot(pRecvMsg->m_dwPlayerKey,  pRecvMsg->m_Team);
					}
				}
				break;

			case CG_ZONE_LOBBY_CHANGE_TEAM_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CHANGE_TEAM);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_CHANGE_TEAM_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_TEAM_NAK *)pMsg;
						
						//	팀교체를 할 수 없는 상태입니다.
						g_InterfaceManager.GetInterfaceString( 70421, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
					}
				}
				break;

			// pvp config
            case CG_ZONE_LOBBY_PVP_CONFIG_NAK:
				{
					this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_PVP_CONFIG_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_PVP_CONFIG_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

			case CG_ZONE_CHUNTING_CREATE_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CREATE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_CHUNTING_CREATE_NAK * pRecvMsg = (MSG_CG_ZONE_CHUNTING_CREATE_NAK*)pMsg;

						g_InterfaceManager.GetStringZoneError( pRecvMsg->m_byErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

            case CG_ZONE_LOBBY_PVP_CONFIG_BRD:
                {
					this->UnsetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_PVP_CONFIG_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_PVP_CONFIG_BRD *)pMsg;

						const sMAPINFO * pMapInfo = GetBattleRoomEntry().GetCurMapInfo();
						assert(pMapInfo);
						GetBattleRoomEntry().SetAdditionalPVPInfo(&pRecvMsg->m_AdditionalPVPInfo);

						for (int i = 0; i < pRecvMsg->m_Count; ++i)
						{
							SetTeamUserSlot(pRecvMsg->m_PlayerInfo[i]);

							if (g_pHero && pRecvMsg->m_PlayerInfo[i].m_dwPlayerKey == g_pHero->GetObjectKey())
							{
								uiBattleScoreMan* pBattleScoreMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

								if(pBattleScoreMan)
								{
									pBattleScoreMan->SetHeroTeam(pRecvMsg->m_PlayerInfo[i].m_Team);
								}
							}
						}
						
						// 확인
						const sFIELDINFO * pFieldInfo = GetBattleRoomEntry().GetCurFieldInfo();
						assert(pFieldInfo);

						BattleScene::SetPendingBattleRoomEntry(&GetBattleRoomEntry());
					}
                }
                break;



			case CG_ZONE_MISSION_CREATE_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CREATE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_MISSION_CREATE_NAK * pRecvMsg = (MSG_CG_ZONE_MISSION_CREATE_NAK *)pMsg;
						if (pRecvMsg->m_dwErrorCode == RC::RC_ROOM_INSUFFICIENT_USER_NUM)
						{
			
							if (GetBattleRoomEntry().GetCurMapInfo())
							{
								TCHAR szNotice[512] = {0,};
								//	최소 %d 명의 인원이 필요합니다.
								g_InterfaceManager.GetInterfaceString( eST_MISSION_CREATE_NAK_MSG, szNotice, INTERFACE_STRING_LENGTH );
								Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szNotice, GetBattleRoomEntry().GetCurMapInfo()->byMinUserNum);
							}
							else
							{
								g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
							}
						}
						else
						{
							g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
						}


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

			case CG_ZONE_PVP_CREATE_NAK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CREATE);

					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_PVP_CREATE_NAK * pRecvMsg = (MSG_CG_ZONE_PVP_CREATE_NAK *)pMsg;
						if (pRecvMsg->m_dwErrorCode == RC::RC_ROOM_INSUFFICIENT_USER_NUM)
						{
							if (GetBattleRoomEntry().GetCurMapInfo())
							{
								TCHAR szNotice[512] = {0,};
								//	최소 %d 명의 인원이 필요합니다.
								g_InterfaceManager.GetInterfaceString( eST_MISSION_CREATE_NAK_MSG, szNotice, INTERFACE_STRING_LENGTH );
								Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szNotice, GetBattleRoomEntry().GetCurMapInfo()->byMinUserNum);
							}
							else
							{
								g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
							}
						}
						else
						{
							g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
						}


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

			case CG_ZONE_MISSION_JOIN_ACK: // 방장/멤버 들이 이패킷을 받고 해당 미션으로 들어간다.
				{
#ifdef _DH_BATTLEZONE2_
                    //배틀존 로비 캐릭터 오브젝트 저장
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_)
                    {
                        ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);
                    }
#else
					if (BattleScene::IsLobby())
#endif//_DH_BATTLEZONE2_
					{
						MSG_CG_ZONE_MISSION_JOIN_ACK* pRecvMsg = (MSG_CG_ZONE_MISSION_JOIN_ACK*)pMsg;

#if defined( _JSH_ADD__PLAY_MOVIE_BEFORE_MISSION_ )
						g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_MOVIEPLAY, pRecvMsg->m_MapCode, 0);
#else
						g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_MISSION, pRecvMsg->m_MapCode, 0);
#endif

						BattleScene::SetLobbyFlag(FALSE);
						BattleScene::SetLobbyInPending(FALSE);

						EventInfoParser::Instance()->InitPlayedEvent();

						g_MissionScene.InitMissionTime();

						UnsetPKBool(PACKET_STATUS_SEND_CREATE);

           				GameFramework::ChangeScene(SCENE_TYPE_LOAD);
					}
				}
				break;

			case CG_ZONE_MISSION_JOIN_NAK:
				{
					WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("recv CG_ZONE_MISSION_JOIN_NAK"));

#ifdef _DH_BATTLEZONE2_
#else
                        if (BattleScene::IsLobby())
#endif//_DH_BATTLEZONE2_
					{
						MSG_CG_ZONE_MISSION_JOIN_NAK * pRecvMsg = (MSG_CG_ZONE_MISSION_JOIN_NAK *)pMsg;

						WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("process CG_ZONE_MISSION_JOIN_NAK: errcode(%d)"), pRecvMsg->m_dwErrorCode);

						if ( pRecvMsg->m_dwErrorCode == RC::RC_ROOM_INSUFFICIENT_USER_NUM)
						{
							if (GetBattleRoomEntry().GetCurMapInfo())
							{
								TCHAR szNotice[512] = {0,};
								//	최소 %d 명의 인원이 필요합니다.
								g_InterfaceManager.GetInterfaceString( eST_MISSION_CREATE_NAK_MSG, szNotice, INTERFACE_STRING_LENGTH );
								Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szNotice, GetBattleRoomEntry().GetCurMapInfo()->byMinUserNum);				
							}
						}
						else
						{
							g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
						}


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;


			case CG_ZONE_PVP_JOIN_ACK:
				{
					UnsetPKBool(PACKET_STATUS_SEND_CREATE);
#ifdef _DH_BATTLEZONE2_
                    //배틀존 로비 캐릭터 오브젝트 저장
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_)
                    {
                        ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_MISSION);
                    }
#else
                    if (BattleScene::IsLobby())
#endif//_DH_BATTLEZONE2_
					{
						assert(g_pHero);

						MSG_CG_ZONE_PVP_JOIN_ACK* pRecvMsg = (MSG_CG_ZONE_PVP_JOIN_ACK*)pMsg;
						g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_PVP, pRecvMsg->m_MapCode, 0);

						BattleScene::SetLobbyFlag(FALSE);
						BattleScene::SetLobbyInPending(FALSE);

           				GameFramework::ChangeScene(SCENE_TYPE_LOAD);			
					}
				}
				break;

			case CG_ZONE_PVP_JOIN_NAK:
				{
#ifdef _DH_BATTLEZONE2_
#else
                    if (BattleScene::IsLobby())
#endif//_DH_BATTLEZONE2_
					{
						MSG_CG_ZONE_PVP_JOIN_NAK * pRecvMsg = (MSG_CG_ZONE_PVP_JOIN_NAK *)pMsg;

						if ( pRecvMsg->m_dwErrorCode == RC::RC_ROOM_INSUFFICIENT_USER_NUM)
						{
							if (GetBattleRoomEntry().GetCurMapInfo())
							{
								TCHAR szNotice[512] = {0,};
								//	최소 %d 명의 인원이 필요합니다.
								g_InterfaceManager.GetInterfaceString( eST_MISSION_CREATE_NAK_MSG, szNotice, INTERFACE_STRING_LENGTH );
								Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szNotice, GetBattleRoomEntry().GetCurMapInfo()->byMinUserNum);			
							}
							else
							{
								g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
							}
						}
						else
						{
							g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
						}

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

			case CG_ZONE_LOBBY_REQUEST_INFO_NAK:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_REQUEST_INFO_NAK * pRecvMsg = (MSG_CG_ZONE_LOBBY_REQUEST_INFO_NAK *)pMsg;

						g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

					}
				}
				break;

			case CG_ZONE_LOBBY_PVP_INFO_BRD:
				{
					if(BattleScene::IsLobby())
					{
						MSG_CG_ZONE_LOBBY_PVP_INFO_BRD * pRecvMsg = (MSG_CG_ZONE_LOBBY_PVP_INFO_BRD *)pMsg;

						SetTeamIndexUserSlot(pRecvMsg->m_dwPlayerKey, pRecvMsg->m_Team);
					}
				}
				break;



			case CG_ZONE_CHANGE_SPECIALTYPE_CMD:
				{
					MSG_CG_ZONE_CHANGE_SPECIALTYPE_CMD * pRecvMsg = (MSG_CG_ZONE_CHANGE_SPECIALTYPE_CMD * )pMsg;
					if( pRecvMsg->m_SpecialType == eROOM_SPECIAL_NORMAL )
					{
						uiPartyMan * partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
						if(partyMan)
						{
							if(partyMan->GetPartyNum() > 1)	// KEY_PARTY_ALONE : 파티원이 혼자이면 메시지를 띄우지 않는다.
							{
								g_InterfaceManager.GetInterfaceString(5861 , szMessage ,INTERFACE_STRING_LENGTH);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );
							}
						}
					}
					this->GetBattleRoomEntry().m_RoomInfo.m_SpecialType =  pRecvMsg->m_SpecialType;
				}
				break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
