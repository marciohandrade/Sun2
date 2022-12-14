#pragma once

#include <SSQ/SSQInfoParser.h>		// ProgramCommon
#include <SSQ/SSQPacketProc.h>		// ProgramCommon
#include <SSQ/SSQPacketProcS2S.h>	// Common


class GameSSQUpdateCtrl;
class SSQMsgRouterInGame;
struct MatchCheckCodeFilters;

namespace nsSSQ {};
namespace nsSSQ_Util {};
namespace GameSSQControlInfo {};

using namespace nsSSQ;
using namespace nsSSQ_Util;
using namespace GameSSQControlInfo;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
namespace GameSSQControlInfo
{
	typedef SSQInfo::WzTmFmt		WzTmFmt;	//	h*100 + m
	typedef WORD					AcmTmFmt;	//	h*60 + m
	typedef SSQInfo::SSQCODE		SSQCODE;
	enum eTIME_TYPE
	{
		TT_NONE, TT_CREATED, TT_WAITING, TT_COUNTDOWN, TT_BATTLE, TT_CLEAR, TT_MAX,	//(TT_CLEAR+1)%TT_MAX
	};

	struct sTIME_INFO
	{
		DWORD						CurTick;
		WzTmFmt						WzTime;		//	Webzen Time Format
		AcmTmFmt					AcmTime;	//	Accumulated Time Format
		SYSTEMTIME					SysTime;

		inline static	VOID		SetTimeValue( sTIME_INFO& OUT timeInfo );
	};

	struct SSQ_UPDATER_NODE
	{
		SSQCODE						CodeOfSSQ;
		KEYTYPE						KEY;
		GameSSQUpdateCtrl*			pUpdater;

		SSQ_UPDATER_NODE() : CodeOfSSQ(0), KEY(0), pUpdater(0) {}
		SSQ_UPDATER_NODE( const SSQCODE codeOfSSQ, GameSSQUpdateCtrl* const ptr ) : CodeOfSSQ(codeOfSSQ), KEY(0), pUpdater(ptr) {}

		const KEYTYPE				GetKey() const				{ return KEY; }
		const SSQCODE				GetCodeOfSSQ() const		{ return CodeOfSSQ; }
		GameSSQUpdateCtrl* const	GetUpdater() const			{ return pUpdater; }
		VOID						SetKey( const KEYTYPE key )	{ KEY = key; }
	};

	struct OpSSQInfoController;
	typedef STLX_VECTOR<SSQ_UPDATER_NODE>			SSQ_UPDATERS;
	typedef SSQ_UPDATERS::iterator					SSQ_UPDATERS_IT;


	inline VOID		sTIME_INFO::SetTimeValue( sTIME_INFO& OUT timeInfo )
	{
		timeInfo.CurTick	= GetTickCount();
		util::TimeSync::GetLocalTime( &timeInfo.SysTime );
		timeInfo.WzTime		= timeInfo.SysTime.wHour*100 + timeInfo.SysTime.wMinute;
		timeInfo.AcmTime	= timeInfo.SysTime.wHour* 60 + timeInfo.SysTime.wMinute;
	}


	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


	struct ConstVariables
	{
		MatchCheckCodeFilters*		pCheckFilters;
	};


	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	陝 SSQ襄葆棻 й渡腎朝 奢嶸腎朝 滲熱菟 <- 褒ч 醞縑 啗骯腎朝 匙菟 偵褐й 熱紫 氈擊 匙檜棻.
	struct SharedVariables
	{
		BOOLEAN						bTransaction;	// 謙猿 籀葬蒂 嬪и AG transaction 鼻鷓,

		enum eBattleState {
			eBattle_NotStarted
			, eBattle_Init
			, eBattle_Progress
			, eBattle_Timeout
			, eBattle_1_Remained
			, eBattle_PlayerExtermination
			, eBattle_AllDead
			, eBattle_EndReserved
			,
		};
		struct {	//	Battle States
			eBattleState			BattleState;
			eBattleState			UpdaterState;	//eBattle_NotStarted|eBattle_Init
			SSQInfo::MINUTE_T		RemainedTime;	// unit : minute
			SSQInfo::COUNT_T		RemainedTotal;
			SSQInfo::COUNT_T		RemainedPlayer;
		};

		inline	VOID				UpdateIncreaseCount( const BOOLEAN isPlayer ) const;
		inline	VOID				UpdateDecreaseCount( const BOOLEAN isPlayer ) const;

		//	special interface
		//inline	VOID			UpdateIncreaseCountMobNum( const INT generated_mob_number ) const;
		inline	SSQInfo::MINUTE_T	UpdateCountdown( const SSQInfo::MINUTE_T countdown_value ) const;
	};
};


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


template< eTIME_TYPE _TM_ID, typename _TM_TYPE, DWORD _INTERVAL >
struct SSQUpdateHandler_info : sun_type_info<_TM_TYPE>
{
	static const eTIME_TYPE					value		= _TM_ID;
	static const DWORD						interval	= _INTERVAL;
};

typedef AutoTransactionVTLS<SSQ_UPDATER_NODE>		AutoTransactionCtrlNode;


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


inline VOID
SharedVariables::UpdateIncreaseCount( const BOOLEAN isPlayer ) const
{
	if( isPlayer )
	{
		SSQInfo::COUNT_T& rRemainedPlayer = const_cast<SSQInfo::COUNT_T&>(RemainedPlayer);
		++rRemainedPlayer;
	}

	SSQInfo::COUNT_T& rRemainedTotal = const_cast<SSQInfo::COUNT_T&>(RemainedTotal);
	++rRemainedTotal;
}


inline VOID
SharedVariables::UpdateDecreaseCount( const BOOLEAN isPlayer ) const
{
	if( isPlayer )
	{
		SSQInfo::COUNT_T& rRemainedPlayer = const_cast<SSQInfo::COUNT_T&>(RemainedPlayer);
		rRemainedPlayer = (SSQInfo::COUNT_T) min( DWORD(RemainedPlayer), DWORD(RemainedPlayer-1) );
	}

	SSQInfo::COUNT_T& rRemainedTotal = const_cast<SSQInfo::COUNT_T&>(RemainedTotal);
	rRemainedTotal = (SSQInfo::COUNT_T) min( DWORD(RemainedTotal), DWORD(RemainedTotal-1) );
}

//inline VOID
//SharedVariables::UpdateIncreaseCountMobNum( const INT generated_mob_number ) const
//{
//	SSQInfo::COUNT_T& rRemainedTotal = const_cast<SSQInfo::COUNT_T&>(RemainedTotal);
//	rRemainedTotal += (SSQInfo::COUNT_T)generated_mob_number;
//}


inline SSQInfo::MINUTE_T
SharedVariables::UpdateCountdown( const SSQInfo::MINUTE_T countdown_value ) const
{
	SSQInfo::MINUTE_T& rRemainedTime = const_cast<SSQInfo::MINUTE_T&>(RemainedTime);
	rRemainedTime = countdown_value;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

