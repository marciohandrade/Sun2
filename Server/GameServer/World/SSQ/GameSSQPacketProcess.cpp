#include "stdafx.h"
#include <PacketStruct_AG.h>

#include "GameInstanceDungeon.h"
#include "GameField.h"
#include "ObjectFactory.h"

#include <World/GameWarControlManager.h>
#include <World/GameWarDistributionChart.h>
#include "SSQMonster.h"
#include <Status/StatusManager.h>


// for debugging control
#undef WAVERIX_DEBUGGING_OUTPUTn
#undef WAVERIX_DEBUGGING_OUTPUTnV

#define WAVERIX_DEBUGGING_OUTPUTn( msg )	\
SUNLOG( eCRITICAL_LOG, "[%s][%u][%u] %s\n", __FUNCTION__, __LINE__,	\
	   (DWORD)((GetTickCount()-tickvalue)/(1000)), msg );					\
	   tickvalue = GetTickCount()

#define WAVERIX_DEBUGGING_OUTPUTnV( value )	\
{ CHAR _message[10];					\
WAVERIX_DEBUGGING_OUTPUTn( itoa( value, _message, sizeof(_message) ) ); }

static DWORD tickvalue = GetTickCount();

#include "GameSSQControlInfos.hxx"
#include "GameSSQPacketProcess.hxx"
#include "GameSSQUpdateCtrl.hxx"
#include "GameSSQControlManagerImpl.hxx"
#include "SSQHandlerInGame.h"

VOID
SSQMsgRouterInGame::SlotUpdate()
{
}

VOID
SSQMsgRouterInGame::SlotClear( User* const pUser )
{
}

#if WAVERIX_CODE_TEST
static BYTE selected_pattern_step_index = 0;
static SSQPattern* selected_pattern_ptr = 0;
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// (WAVERIX) (090716) (CHANGES) move function block from hxx to cpp
VOID
SSQMsgRouterInGame::Update(const SSQInfo::INDEX_T IDofSSQ)
{
    if(!(m_pUpdateCtrl))
        return;

    //SSQMsgRouterInGame& rThis = *this;
    //Foreach(&SSQMsgRouterInGame::NodeUpdate, rThis);
    SlotUpdate();

    // (WAVERIX) (090716) (CHANGES) use high-load balanced network
    // assert(ENABLE_FIELD_HIGH_LOAD_BALANCING);
    //GameInstanceDungeon* pDungeon = m_pUpdateCtrl->GetSSQField();
    //GameField* pGameField = pDungeon ? pDungeon->GetEntryField() : 0;

    {	// AG
        MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD* const pMsg = m_msgCMD_AG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD) != size)
        {
            m_msgCMD_AG.SetKey(IDofSSQ);
            g_pGameServer->SendToServer(AGENT_SERVER, pMsg, size);
            m_msgCMD_AG.Reset();
        }
    };
//--------------------------------------------------------------------------------------------------
//-------------------------------------- UNUSED SECTION --------------------------------------------
    {	// CG
        MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD* const pMsg = m_msgCMD_CG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD) != size)
        {
            g_pGameServer->SendToServer(AGENT_SERVER, pMsg, size);
            m_msgCMD_CG.Reset();
        }
    };
    {	// CG
        MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD* const pMsg = m_msgBRD_CG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD) != size)
        {
            g_pGameServer->SendToServer(AGENT_SERVER, pMsg, size);
            m_msgBRD_CG.Reset();
        }
    };
//-------------------------------------- UNUSED SECTION --------------------------------------------
//--------------------------------------------------------------------------------------------------
}

// Server<->Server (A<->G) SSQ_MESSAGE_HANDLERs
BOOLEAN
SSQMsgRouterInGame::Routing( const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header )
{
	const SSQCODE CodeOfSSQ					= m_pUpdateCtrl->GetCodeOfSSQ();
	GameInstanceDungeon* const pDungeonSSQ	= m_pUpdateCtrl->GetSSQField();
	const SSQInfo* const pSSQInfo			= m_pUpdateCtrl->GetSSQInfo();
	//	pDungeonSSQ는 null일 수 있다. 사용 필요시 검사해서 사용할 것.
	const KEYTYPE KeyOfZone					= pDungeonSSQ ? pDungeonSSQ->GetKey() : 0;
	WAR_MSG_HDR* const pHeader				= &m_msgCMD_AG.MSG_HEADER;

	WAR_TAG_P pIt	= const_cast<WAR_TAG_P>( pMsg );
	WAR_TAG_P pEnd	= pIt + header.size;
	for( WORD size=0 ; pIt<pEnd ; pIt+=size )
	{
		size	= 10000;	// 미 설정에 대한 스킵 설정

		switch( pIt->_TAG )
		{
		case SSQ_S2S_FIELD_CREATE:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_FIELD_CREATE );

				WAVERIX_DEBUGGING_OUTPUTn( "SSQ_S2S_FIELD_CREATE" );

				// 생성된 인던을 SSQ를 위한 인던으로 전환한다.
				if( pDungeonSSQ )
				{
					MSGSUB_SSQ_S2S_FIELD_CREATED msg;
					msg.CodeOfSSQ	= CodeOfSSQ;	// map code 대신 id
					msg.m_ZoneKey	= KeyOfZone;
					msg.SerializeTo( pHeader );

					m_pUpdateCtrl = GameSSQUpdateCtrl::ChangeToNextUpdateCtrl( m_pUpdateCtrl, GameSSQControlInfo::TT_CREATED );
				}
				else
				{
					MSGSUB_SSQ_S2S_FIELD_DESTORYED msg;
					msg.CodeOfSSQ	= CodeOfSSQ;
					msg.m_ZoneKey	= KeyOfZone;	// 난다? 고레와...
					msg.SerializeTo( pHeader );
					//	이후 상태 처리는 어캐?
				}
			}
			break;
		case SSQ_S2S_FIELD_CREATED:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_FIELD_CREATED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_FIELD_CREATED] \n" );
			}
			break;
		case SSQ_S2S_FIELD_BLOCK:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_FIELD_BLOCK );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_FIELD_BLOCK] \n" );

//#if WAVERIX_CODE_TEST
//				m_pUpdateCtrl = m_pUpdateCtrl->ChangeToNextUpdateCtrl( m_pUpdateCtrl, GameSSQControlInfo::TT_NONE );
//#endif
			}
			break;
		case SSQ_S2S_FIELD_DESTORY:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_FIELD_DESTORY );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_FIELD_DESTORY] \n" );
			}
			break;
		case SSQ_S2S_FIELD_DESTORYED:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_FIELD_DESTORYED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_FIELD_DESTORYED] \n" );
			}
			break;
		case SSQ_S2S_GAMBLE_START				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_GAMBLE_START );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_GAMBLE_START] \n" );

				GameField* const pField			= pDungeonSSQ->GetEntryField();

				typedef GameField::_PLAYER_HASH		_PLAYER_HASH;
				const _PLAYER_HASH& rPlayers	= pField->GetRefPlayerList();
				const DWORD player_num			= (DWORD)rPlayers.size();
				const DWORD prize_winner
					= player_num
					?	RandomNumberGenerator::GetRandomNumberRange( 0, player_num )
					:	player_num
					;

				{	//	당첨 처리.
					MSGSUB_SSQ_S2S_GAMBLE_RESULT msg;
					if( prize_winner == player_num )	// fail 당첨자 없다. 사실 이래서야 안되겠지? 테스트 용으로 생각하자.
					{
						ZeroMemory( &msg, sizeof(msg) );
					}
					else
					{
						GameSSQControlManagerImpl* const pManager = m_pUpdateCtrl->GetManager();
						pManager->ProcessGamble( m_pUpdateCtrl, pField, prize_winner, msg );
					}

					msg.SerializeTo( pHeader );
				};
			}
			break;
		case SSQ_S2S_GAMBLE_RESULT				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_GAMBLE_RESULT );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_GAMBLE_RESULT] \n" );
			}
			break;
		case SSQ_S2S_BATTLE_COUNTDOWN			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_COUNTDOWN );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_COUNTDOWN] \n" );

				GameField* const pGameField = pDungeonSSQ->GetEntryField();
				typedef GameField::_PLAYER_HASH		_PLAYER_HASH;
				const _PLAYER_HASH& rPlayers = pGameField->GetRefPlayerList();
				FOREACH_CONTAINER( const _PLAYER_HASH::value_type& rPlayer, rPlayers, _PLAYER_HASH )
				{
					Player* const pPlayer = rPlayer.second;
                    if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
                    {
                        continue;
                    }
					ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
					assert( rHandler.IsValidState() );
					rHandler->SetInfo_QuestFailed( 0, 0, 0, 0 );
				}
			}
			break;
		case SSQ_S2S_BATTLE_PREPARATION:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_PREPARATION );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_PREPARATION] \n" );

				SSQDistributionChart chart;
				GameField* const pField = chart.SetAndGetSSQField( pDungeonSSQ );
				const SectorGrid& rGrid = pField->SectorGridRef;
				chart.SetProjectionGrid( &rGrid );
				chart.Build( 10/*meter*/ );

				//for( DWORD loop=3 ; loop ; --loop )
				const INT gened_mob_num = chart.RandomDistribution( pSSQInfo, m_pUpdateCtrl->ConstVars.pCheckFilters );
				//m_pUpdateCtrl->SharedVars.UpdateIncreaseCountMobNum( gened_mob_num );
			}
			break;
		case SSQ_S2S_BATTLE_START				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_START );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_START] \n" );

				GameField* const pField			= pDungeonSSQ->GetEntryField();
				typedef GameField::_CHARACTER_HASH		_CHARACTER_HASH;
				const _CHARACTER_HASH& rList	= pField->GetRefCharacterList();
				FOREACH_CONTAINER( const _CHARACTER_HASH::value_type& rNode, rList, _CHARACTER_HASH )
				{
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
                    Object* const object = rNode.second;
                    if (object != NULL && object->IsDeleted())
                    {
                        const Object::Key key = object->GetObjectKey();
                        SUNLOG(
                            eCRITICAL_LOG, 
                            _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                            __FUNCTION__, 
                            __LINE__, 
                            key
                        );
                        continue;
                    }
//#endif
					Character* const pCharacter	= rNode.second;
					StatusManager* const pStatusManager = pCharacter->GetStatusManager();
					pStatusManager->Status.On_FilterControl( FALSE, TRUE, m_pUpdateCtrl->ConstVars.pCheckFilters );
					pStatusManager->ChangeInterState( eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK, FALSE );
				}

				m_pUpdateCtrl = GameSSQUpdateCtrl::ChangeToNextUpdateCtrl( m_pUpdateCtrl, GameSSQControlInfo::TT_BATTLE );
			}
			break;
		case SSQ_S2S_BATTLE_CANCEL				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_CANCEL );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_CANCEL] \n" );
			}
			break;
		case SSQ_S2S_BATTLE_END					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_END );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_END] \n" );

				GameField* const pField				= pDungeonSSQ->GetEntryField();
				typedef GameField::_CHARACTER_HASH	_CHARACTER_HASH;
				const _CHARACTER_HASH& rCharacters	= pField->GetRefCharacterList();

				typedef STLX_VECTOR<Character*>	CHAR_LIST;
				CHAR_LIST tempCharList;
				tempCharList.reserve( rCharacters.size() );

				FOREACH_CONTAINER( const _CHARACTER_HASH::value_type& rCharacter, rCharacters, _CHARACTER_HASH )
				{
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
                    Object* const object = rCharacter.second;
                    if (object != NULL && object->IsDeleted())
                    {
                        const Object::Key key = object->GetObjectKey();
                        SUNLOG(
                            eCRITICAL_LOG, 
                            _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                            __FUNCTION__, 
                            __LINE__, 
                            key
                        );
                        continue;
                    }
//#endif
					Character* const pCharacter	= rCharacter.second;
					const BOOLEAN isPlayer = pCharacter->IsEqualObjectKind(PLAYER_OBJECT);
					if( isPlayer )
					{
						StatusManager* const pStatusManager = pCharacter->GetStatusManager();
						pStatusManager->Status.On_FilterControl( TRUE, TRUE, m_pUpdateCtrl->ConstVars.pCheckFilters );
						pStatusManager->ChangeInterState( eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK, TRUE );
					}
					else
					{
						tempCharList.push_back( pCharacter );
					}
				}

				ObjectFactory* const pObjectFactory = ObjectFactory::Instance();
				FOREACH_CONTAINER( CHAR_LIST::value_type& rChars, tempCharList, CHAR_LIST )
				{
					Character* const pCharacter = rChars;
					pField->LeaveField( pCharacter );
					pObjectFactory->FreeObject( pCharacter );
				}

				if( FlowControl::FCAssert( m_pUpdateCtrl->GetTimeType() == TT_BATTLE ) )
				{

				}
			}
			break;
		case SSQ_S2S_BATTLE_CLEAR				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_CLEAR );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_CLEAR] \n" );

				m_pUpdateCtrl = GameSSQUpdateCtrl::ChangeToNextUpdateCtrl( m_pUpdateCtrl, GameSSQControlInfo::TT_CLEAR );
			}
			break;
		case SSQ_S2S_BATTLE_CLEARED				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_BATTLE_CLEARED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_BATTLE_CLEARED] \n" );
			}
			break;
		case SSQ_S2S_ZONE_SSQ_JOIN_NTF			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_ZONE_SSQ_JOIN_NTF );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_ZONE_SSQ_JOIN_NTF] \n" );
				Player* const pPlayer = PlayerManager::Instance()->FindPlayerByObjKey( rMsg.ObjectKey );
				if( FlowControl::FCAssert( pPlayer ) )
				{
					ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
					rRewarder.SetUsedItemCode( rMsg.ItemCode );
				}
			}
			break;

		case SSQ_S2S_OBSERVER_MODE_CTRL			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_OBSERVER_MODE_CTRL );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_S2S_OBSERVER_MODE_CTRL] \n" );

				const eTIME_TYPE updater_type = m_pUpdateCtrl->GetTimeType();
				if( updater_type == TT_BATTLE )
				{
					GameSSQUpdateCtrl_Battle* const pUpdaterBattle =
						static_cast<GameSSQUpdateCtrl_Battle*>( m_pUpdateCtrl );
					pUpdaterBattle->RequestObserverModeCtrl( rMsg );
				}
			}
			break;

			//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
			// C<->G SSQ_MESSAGE_HANDLERs
		case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_ENABLE_ENTER );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER						:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_CLOSE_ENTER );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_CLOSE_ENTER] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_GAMBLE_START					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_GAMBLE_START );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_GAMBLE_START] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_BATTLE_START					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_START );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_BATTLE_START] \n" );
			}
			break;

		case SSQ_PROTOCOL_NOTICE_BATTLE_END						:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_END );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_NOTICE_BATTLE_END] \n" );
			}
			break;

		case SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION] \n" );

#if WAVERIX_CODE_TEST
				typedef SSQInfoParser::SSQPATTERN_TABLE		SSQPATTERN_TABLE;
				const SSQPATTERN_TABLE& rPatterns = SSQInfoParser::Instance()->GetRefSSQPattern();
				//FOREACH_CONTAINER( const SSQPATTERN_TABLE::value_type& rPattern, rPatterns, SSQPATTERN_TABLE )
				const DWORD upperbound = (DWORD)rPatterns.size();
				const DWORD selected_pattern = RandomNumberGenerator::GetRandomNumberRange( 0, upperbound );

				SSQPATTERN_TABLE::const_iterator where = rPatterns.begin();
				advance( where, selected_pattern );
				SSQPattern* pPattern = where->second;

				const SSQPattern::INDEX_T pattern_index = pPattern->m_Index;
				const SSQPattern::STEP_T step_upperbound = pPattern->m_BanLiftStep;
				const DWORD select_index = RandomNumberGenerator::GetRandomNumberRange( 0, step_upperbound );

				{
					MSGSUB_SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION msg;
					msg.PatternIndex	= pattern_index;
					msg.StepArrayIndex	= (BYTE)select_index;
					msg.SerializeTo( pHeader );
				};


				selected_pattern_step_index = (BYTE)select_index;
				selected_pattern_ptr = pPattern;
#endif
			}
			break;

		case SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE] \n" );

#if WAVERIX_CODE_TEST
				{
					MSGSUB_SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE msg;
					msg.SerializeTo( pHeader );
				};
				{	//	SSQDistributionChar Initialize
					SSQDistributionChart DamageExecuter;
					GameField* const pField = DamageExecuter.SetAndGetSSQField( pDungeonSSQ );
					const SectorGrid& rGrid	= pField->SectorGridRef;
					DamageExecuter.SetProjectionGrid( &rGrid );

					MSGSUB_SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION msg;
					DamageExecuter.Earthquake( selected_pattern_ptr, selected_pattern_step_index, msg );
					msg.SerializeTo( pHeader );
				};
#endif
			}
			break;

		case SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE );
//				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE] \n" );

#if WAVERIX_CODE_TEST
				{
					MSGSUB_SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE msg;
					msg.Count = 1;
					SetVector( &msg.Position[0], 200.f, 200.f, 18.f );
					msg.SerializeTo( pHeader );
				};
#endif
			}
			break;

		case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE] \n" );
#if WAVERIX_CODE_TEST
				{
					rMsg.SerializeTo( pHeader );
				};
#endif
			}
			break;

		case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED] \n" );
			}
			break;

		case SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION					:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION] \n" );
			}
			break;

		case SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS] \n" );
			}
			break;

		case SSQ_PROTOCOL_INFORMATION_REMAINED_TIME				:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_REMAINED_TIME );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_INFORMATION_REMAINED_TIME] \n" );
			}
			break;

		case SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING] \n" );
			}
			break;

		case SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED			:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED] \n" );
			}
			break;

		case SSQ_PROTOCOL_CTRL_TAG_TAIL							:
			{
				const SSQ_MSG_CTRL_TAG_TAIL& rMsg = static_cast<const SSQ_MSG_CTRL_TAG_TAIL&>(*pIt);
				size = rMsg.GetSize();

				//pOffset += sizeof( SSQ_MSG_CTRL_TAG_TAIL );
				//rHdr.size -= sizeof( SSQ_MSG_CTRL_TAG_TAIL );
				WAVERIX_DEBUGGING_OUTPUTn( "[SSQ_PROTOCOL_CTRL_TAG_TAIL] \n" );
			}
			break;
		default:
			{
				SUNLOG( eCRITICAL_LOG, "Can't Readable SSQ Msg Format %2u", pIt->_TAG );
			#ifdef _DEBUG
				ASSERT( !"Can't Readable SSQ Msg Format" );
			#endif
			}
			break;
		}
	}
	return TRUE;
}


