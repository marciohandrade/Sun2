#pragma once

#include "./SSQHandlerInGame.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQControlFlag>
struct sPOLICY_SSQ : public util::BitSetOperation<32>
{
	typedef util::BIT_OFFSET<  0 >	FLAG_BATTLE_SERVER;
	typedef util::BIT_OFFSET<  1 >	FLAG_FIELD_SERVER;
	static const DWORD			FLAG_MAX_SIZE			= 32;
	static const DWORD			FLAG_DISABLE_ALL		=  0;

	typedef util::BIT_MASK<
		FLAG_BATTLE_SERVER
	>							FGROUP_BATTLE_SERVER_FUNCTION;

	typedef util::BIT_MASK<
		FLAG_FIELD_SERVER
	>							FGROUP_FIELD_SERVER_FUNCTION;


	//enum eFLAG {
	//	PSC_DISABLE_ALL					= 0,
	//	PSC_BATTLE_SERVER				,
	//	PSC_FIELD_SERVER				,
	//	PSC_MAX_SIZE					= 32,
	//};

	//enum eGROUP {
	//	PSG_BATTLE_SERVER_FUNCTION		=
	//	( (1<<PSC_BATTLE_SERVER)
	//	),
	//	PSG_FIELD_SERVER_FUNCTION		=
	//	( (1<<PSC_FIELD_SERVER)
	//	),
	//};


	//─────────────────────────────────────────
	//	<METHODs>
	sPOLICY_SSQ() {}
	inline	VOID			RegisterBattleServerFunction()
	{
		DWORD flags = FGROUP_BATTLE_SERVER_FUNCTION::value;
		Serialize( this->SAVE, flags, 0 );
	}

	inline	VOID			RegisterFieldServerFunction()
	{
		DWORD flags = FGROUP_FIELD_SERVER_FUNCTION::value;
		Serialize( this->SAVE, flags, 0 );
	}

	inline	BOOL			IsCheckedBattleServer() { return (*this)[FLAG_BATTLE_SERVER::value]; }
	inline	BOOL			IsCheckedFieldServer() { return (*this)[FLAG_FIELD_SERVER::value]; }
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameSSQControlManagerImpl>
//
struct GameSSQControlManagerImpl
{
	friend class GameWarControlManager;
	friend class GameSSQUpdateCtrl;
	friend struct OpSSQInfoController;

private:	ITimerBase						ScheduleUpdateTimer;
			SSQ_UPDATERS					UpdateList;
private:	sPOLICY_SSQ						Control;

public:		GameSSQControlManagerImpl();
			~GameSSQControlManagerImpl();

private:	SSQ_UPDATERS_IT	_FindCodeOfSSQ( const SSQCODE CodeOfSSQ );
			SSQ_UPDATERS_IT	_FindZoneKeyofSSQ( const KEYTYPE KeyOfSSQ );
			VOID			_RegisterUpdateCtrl( const SSQCODE CodeOfSSQ, GameSSQUpdateCtrl* const pCtrl );
			VOID			_ReleaseUpdateCtrl();
			VOID			_InitializeUpdateCtrl();
			VOID			_ChangeUpdater( const SSQ_UPDATER_NODE& rNode );

			VOID			_OnDead( GameSSQUpdateCtrl* const pUpdateCtrl, Character* const pTarget, Character* const pKiller );
			BOOLEAN			_OnEnterEvent( GameSSQUpdateCtrl* const pUpdateCtrl, Character* const pCharacter );
			BOOLEAN			_OnLeaveEvent( GameSSQUpdateCtrl* const pUpdateCtrl, Character* const pCharacter );

			BOOLEAN			GM_Command_Process( const GameSSQCtrlCmd::GMCmd& rInfo, Player* const pGmPlayer );
public:		BOOLEAN			ProcessGamble(
									GameSSQUpdateCtrl* const pUpdateCtrl
									, GameField* const pGameField
									, const DWORD prize_winner
									, MSGSUB_SSQ_S2S_GAMBLE_RESULT& msg
									);
			//	<description>
			//	-	정산 처리
			//	<returns>
			//	-	total point
public:		DWORD			SettleAccounts(
									GameSSQUpdateCtrl* const pUpdateCtrl
									, Player* const pPlayer
									, const SSQHandlerInGame::eOrder order = SSQHandlerInGame::eOrder_None	// 순위가 결정되었을 때 사용할 것.
									);
			VOID			UpdateToRankingAndExtra(
									GameSSQUpdateCtrl* const pUpdateCtrl
									, Player* const pPlayer
									);
			BOOLEAN			RewardForWinner(
									GameSSQUpdateCtrl* const pUpdateCtrl
									, Player* const pPlayer
									, const SSQHandlerInGame::eOrder order		// 1,2,3등 
									);
			BOOLEAN			RewardInBattle(
									GameSSQUpdateCtrl* const pUpdateCtrl
									, Player* const pPlayer
									);
#ifdef _NA000000_100427_SSQ_DEFAULT_REWARD_
            // (NOTE) default reward process for all joined players
            BOOLEAN         RewardDefault(GameSSQUpdateCtrl* const update_ctrl,
                                          Player* const player);
#endif
			//─────────────────────────────────────────
			//	<>
private:
    enum eRewardSelect {
        eReward_Default = 0x01, // NOTE: f110602.3L, f100427.x, added
        eReward_InBattle = 0x02,
        eReward_ForWinner = 0x04,
    };
private:	BOOLEAN			_RewardProcess(
									const eRewardSelect reward_selector
									, GameSSQUpdateCtrl* const pUpdateCtrl
									, Player* const pPlayer
									, const SSQInfo::REWARD_NODE& rRewardNode
									, const SSQHandlerInGame::eOrder order
									);
private:	//─────────────────────────────────────────
			//	<PACKET HANDLERs>

#define SSQ_PACKET_HANDLER_AG_DECL( c, p )	\
	VOID	GameSSQControlManagerImpl::OnRecv_##p ( const MSG_##p* pRecvMsg, const WORD wSize, ServerSession* const pSession )

private:	SSQ_PACKET_HANDLER_AG_DECL( AG_ZONE,		AG_ZONE_SSQ_JOIN_INFO_SYN );
			SSQ_PACKET_HANDLER_AG_DECL( AG_ZONE,		AG_ZONE_SSQ_JOIN_SYN );
			SSQ_PACKET_HANDLER_AG_DECL( AG_ZONE,		AG_ZONE_SSQ_ROOM_CTRL_CMD );


			WAVERIX_TEST_BLOCK_IN_CLASS(GameSSQControlManagerImpl);
};
