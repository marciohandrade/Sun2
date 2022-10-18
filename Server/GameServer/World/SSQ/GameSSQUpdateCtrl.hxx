#pragma once

#include <Status_Operator.h>
#include <World/GameWarDistributionChart.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameSSQUpdateCtrl_xxx><Implementation>
//


//────────────────────────────────────────────────────────────────
//
class GameSSQUpdateCtrl
{
public:
	friend struct GameSSQControlManagerImpl;
	friend struct OpSSQInfoController;
	typedef sTIME_INFO						curtime_info;
public:
	static const DWORD		second = 1000;
	static const DWORD		minute = 60*second;

public:		GameSSQUpdateCtrl()
				: ConstVars(m_ConstVars)
				, SharedVars(m_SharedVars)
			{
				m_Timer.Stop();
			}
public:
	virtual	~GameSSQUpdateCtrl() {}
protected:
	VOID						MigrationDefault( 
		const SSQCODE						CodeOfSSQ
		, GameSSQControlManagerImpl* const	pMgrImpl
		, SSQMsgRouterInGame* const			pMsgParser
		, SSQInfo* const					pSSQInfo
		);
	static	VOID				Migration( GameSSQUpdateCtrl* const pDest, const GameSSQUpdateCtrl* pSrc )
	{
		pDest->MigrationDefault(
			pSrc->m_CodeOfSSQ
			, pSrc->m_pManager
			, pSrc->m_pMsgParser
			, pSrc->m_pSSQInfo
			);
		pDest->m_pSSQField		= pSrc->m_pSSQField;
		pDest->m_ConstVars		= pSrc->m_ConstVars;
		pDest->m_SharedVars		= pSrc->m_SharedVars;
	}

	static	VOID						RegisterUpdateCtrl( GameSSQUpdateCtrl* const pTarget );
public:
	static	GameSSQUpdateCtrl*			ChangeToNextUpdateCtrl( GameSSQUpdateCtrl* const pCurUpdater, const GameSSQControlInfo::eTIME_TYPE nextType );

public:
	virtual VOID						Init() = 0;
	virtual	BOOL						Update( const curtime_info& tmInfo ) = 0;
public:		BOOL						UpdateMsg( const curtime_info& tmInfo );
			VOID						RecvedMsgProcess( const WAR_MSG_HDR* IN pStream );
	static	VOID						ConstVarsInit( GameSSQUpdateCtrl* const pUpdateCtrl );
	static	VOID						ConstVarsRelease( GameSSQUpdateCtrl* const pUpdateCtrl );
public:
	inline	const SSQCODE 				GetCodeOfSSQ() const { return m_CodeOfSSQ; }
	inline	const ITimerBase&			GetTimer() const { return m_Timer; }
	inline	const SSQInfo*				GetSSQInfo() const { return m_pSSQInfo; }
	inline	SSQMsgRouterInGame*			GetMsgRouter() const { return m_pMsgParser; }
	inline	eTIME_TYPE					GetTimeType() const { return m_CurType; }
	inline	VOID						SetTimeType( const eTIME_TYPE timeType ) { m_CurType = timeType; }
	inline	GameInstanceDungeon*		GetSSQField() const { return m_pSSQField; }
	inline	VOID						SetDungeon( GameInstanceDungeon* const pDungeon ) { m_pSSQField= pDungeon; }
	inline	GameSSQControlManagerImpl*	GetManager() const { return m_pManager; }

public:
	const	ConstVariables&				ConstVars;
	const	SharedVariables&			SharedVars;

protected:	SSQCODE						m_CodeOfSSQ;
			eTIME_TYPE					m_CurType;
			ITimerBase					m_Timer;
			GameSSQControlManagerImpl*	m_pManager;
			SSQMsgRouterInGame*			m_pMsgParser;
			SSQInfo*					m_pSSQInfo;
			GameInstanceDungeon*		m_pSSQField;
			ConstVariables				m_ConstVars;
			SharedVariables				m_SharedVars;
    __DISABLE_COPY(GameSSQUpdateCtrl);
};

//────────────────────────────────────────────────────────────────
//
class GameSSQUpdateCtrl_None : public GameSSQUpdateCtrl
{
public:
	static const DWORD					interval = 5*second;
	static const eTIME_TYPE				value = GameSSQControlInfo::TT_NONE;
	typedef GameSSQUpdateCtrl_None		type;
	typedef SSQUpdateHandler_info< value, type, interval > updater_info;
public:
    GameSSQUpdateCtrl_None() {}
	virtual VOID						Init();
	virtual	BOOL						Update( const curtime_info& tmInfo );
    __DISABLE_COPY(GameSSQUpdateCtrl_None);
};


//────────────────────────────────────────────────────────────────


class GameSSQReport
{
public:
	static const DWORD			second = GameSSQUpdateCtrl::second;
	static const DWORD			minute = GameSSQUpdateCtrl::minute;
	typedef GameSSQUpdateCtrl::curtime_info			curtime_info;
	static const DWORD			UPDATE_INTERVAL							= 5*second;	//milli-second

public:		GameSSQReport();
			VOID				Init( GameSSQUpdateCtrl* const pUpdater );
			VOID				Update( const curtime_info& tmInfo, const BOOLEAN bForced=FALSE );

private:	GameSSQUpdateCtrl*	m_pUpdater;
			//SSQInfo::COUNT_T	m_LastUpdatedChrNum;
			ITimerBase			m_ReportTimer;
};


//────────────────────────────────────────────────────────────────
//
class SSQTraps
{
public:
	static const DWORD			second = GameSSQUpdateCtrl::second;
	static const DWORD			minute = GameSSQUpdateCtrl::minute;
	typedef GameSSQUpdateCtrl::curtime_info			curtime_info;

public:		enum eTrapFlag {
				TRAP_NONE						= 0,
				TRAP_DEBUG_CONTROL				= 1<< 0,
				TRAP_ENABLED					= 1<< 1,
				TRAP_ENABLED_THUNDERBOLT_SHOW	= 1<< 3,	// 연출 처리
				TRAP_ENABLED_EARTHQUAKE			= 1<< 5,
				TRAP_ENABLED_THUNDERBOLT		= 1<< 6,
				TRAP_ENABLED_CLOSEDSPACE		= 1<< 7,
				TRAP_MAX						= 1<<31,
			};

public:		SSQTraps();
			VOID				Init( GameSSQUpdateCtrl* const pUpdater, const eTrapFlag control_flags );
			VOID				Update( const curtime_info& tmInfo );

public:
	inline	BYTE						GetCurEarthquakeStep() { return EarthquakeInfo.LastUpdateCounter; }

private:
	const	GameSSQUpdateCtrl*					pUpdateCtrl;
			GameField*							pGameField;
	static const DWORD							UPDATE_INTERVAL							= 50;	//milli-second
	static const DWORD							UPDATE_EARTHQUAKE_CHECK_INTERVAL		= 10*second;
	static const DWORD							UPDATE_EARTHQUAKE_CHANGE_TO_INDICATION	=  1*second;
	static const DWORD							UPDATE_EARTHQUAKE_INDICATION_INTERVAL	=  5*second;
			ITimerBase							TrapUpdateTimer;
			GameSSQControlInfo::AcmTmFmt		StartAcmTime;

			enum eEarthquake {
				EARTHQUAKE_NONE, EARTHQUAKE_INDICATION, EARTHQUAKE_EXECUTE,
			};

			DWORD								TrapFlag;		// Trap 제어 필드
			struct Earthquake{
				ITimerBase					UpdateTimer;
				BYTE						Mode;				//	eEarthquake
				SSQPattern::INDEX_T			PatternIndex;
				BYTE						PatternArrayIndex;
				BYTE						LastUpdateCounter;
				SSQPattern*					pPattern;
			}									EarthquakeInfo;	//	TRAP #1
			struct Thunderbolt {
				ITimerBase					UpdateTimer;		//	by ScriptData
				WzVector					SearchRangeWeight;	//	각각의 ViewBox에 적용할 가중치값 설정 (1.5f, 1.5f, 0.f) <- 원래값 지름 3m
				SSQInfo::THUNDERBOLT_NODE	ThunderboltData;
			}									ThunderboltInfo;//	TRAP #2
			struct ClosedSpace {
				ITimerBase					UpdateTimer;		//	by ScriptData
				SSQInfo::CLOSED_SPACE_NODE	ClosedSpaceData;
			}									ClosedSpaceInfo;//	TRAP #3

			SSQDistributionChart				DamageExecuter;
private:
	friend class GameSSQUpdateCtrl_Battle;
    __DISABLE_COPY(SSQTraps);
};


//────────────────────────────────────────────────────────────────
//

class GameSSQUpdateCtrl_Created : public GameSSQUpdateCtrl
{
public:
	static const DWORD				interval = 5*second;
	static const eTIME_TYPE			value = GameSSQControlInfo::TT_CREATED;
	typedef GameSSQUpdateCtrl_Created	type;
	typedef SSQUpdateHandler_info< value, type, interval > updater_info;
public:
    GameSSQUpdateCtrl_Created() {}
	virtual	~GameSSQUpdateCtrl_Created() {}
	virtual VOID				Init();
	virtual	BOOL				Update( const curtime_info& tmInfo );

private:	GameSSQReport		m_Reporter;
			SSQTraps			m_Traps;
    __DISABLE_COPY(GameSSQUpdateCtrl_Created);
};


//────────────────────────────────────────────────────────────────
//

class GameSSQUpdateCtrl_Battle : public GameSSQUpdateCtrl
{
public:
	static const DWORD					interval = 15;
	static const eTIME_TYPE				value = GameSSQControlInfo::TT_BATTLE;
	typedef GameSSQUpdateCtrl_Battle	type;
	typedef SSQUpdateHandler_info< value, type, interval > updater_info;
public:
    GameSSQUpdateCtrl_Battle() {}
	virtual	~GameSSQUpdateCtrl_Battle();
	virtual VOID						Init();
	virtual	BOOL						Update( const curtime_info& tmInfo );

public:
	inline	BYTE						GetCurEarthquakeStep() { return m_Traps.GetCurEarthquakeStep(); }
			BOOLEAN						RequestObserverModeCtrl( const MSGSUB_SSQ_S2S_OBSERVER_MODE_CTRL& rMsg );

private:	typedef SharedVariables		Ret;
			Ret::eBattleState			_CheckBattleEnd( const curtime_info& tmInfo );
			VOID						_Process_Init( const curtime_info& tmInfo );
			Ret::eBattleState			_Process_Battle( const curtime_info& tmInfo );
			BOOLEAN						_Process_EndReserve( const curtime_info& tmInfo, const Ret::eBattleState battle_state );
	static const DWORD					player_event_update_interval = 1*second;
			VOID						_Process_Players( const curtime_info& tmInfo );

#if WAVERIX_CODE_TEST
public:
#else
private:
#endif
			ITimerBase					m_PlayersUpdateTimer;
			GameSSQReport				m_Reporter;
			SSQTraps					m_Traps;

public:		GameWarSSQItemDistribution	ItemDistributer;
			GameWarObserverMode			ObserverControl;
private:
	friend class Traps;
    __DISABLE_COPY(GameSSQUpdateCtrl_Battle);
};


//────────────────────────────────────────────────────────────────
//


class GameSSQUpdateCtrl_Clear : public GameSSQUpdateCtrl
{
public:
	static const DWORD					interval				=  5*second;
	static const DWORD					interval_for_leave		=  5*second;	// 승자 처리 이후 leave까지 대기시간
	static const DWORD					interval_for_leave_wait	= 30*second;	// Leave명령 이후 정상 플로우로 나갈 수 있도록 대기하는 시간.
	static const eTIME_TYPE				value = GameSSQControlInfo::TT_CLEAR;
	typedef GameSSQUpdateCtrl_Clear		type;
	typedef SSQUpdateHandler_info< value, type, interval > updater_info;
public:
    GameSSQUpdateCtrl_Clear() {}
	virtual	~GameSSQUpdateCtrl_Clear() {}
	virtual VOID				Init();
	virtual	BOOL				Update( const curtime_info& tmInfo );

public:		VOID				Winner_Process( const curtime_info& tmInfo );
			VOID				Leave_Process( const curtime_info& tmInfo );

	enum eSTEP { STEP_INIT, STEP_WINNER_PROC, STEP_LEAVE_PROCESS, STEP_WAIT, STEP_MAX };
private:	DWORD				m_ClearStep;
    __DISABLE_COPY(GameSSQUpdateCtrl_Clear);
};


//────────────────────────────────────────────────────────────────
//


typedef GameSSQUpdateCtrl_None::updater_info	GameSSQUpdateCtrl_None_Info;
typedef GameSSQUpdateCtrl_Created::updater_info	GameSSQUpdateCtrl_Created_Info;
typedef GameSSQUpdateCtrl_Battle::updater_info	GameSSQUpdateCtrl_Battle_Info;
typedef GameSSQUpdateCtrl_Clear::updater_info	GameSSQUpdateCtrl_Clear_Info;


#undef SSQ_UPDATE_CTRL_NODE
//#define SSQ_UPDATE_CTRL_NODE( _type )	blah~
#define SSQ_UPDATE_CTRL_LIST()								\
	SSQ_UPDATE_CTRL_NODE( GameSSQUpdateCtrl_None_Info	)	\
	SSQ_UPDATE_CTRL_NODE( GameSSQUpdateCtrl_Created_Info )	\
	SSQ_UPDATE_CTRL_NODE( GameSSQUpdateCtrl_Battle_Info )	\
	SSQ_UPDATE_CTRL_NODE( GameSSQUpdateCtrl_Clear_Info )	

