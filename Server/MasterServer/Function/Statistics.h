#pragma once

// ================================================================================================
//   각종 통계관련 정보 모음
// ================================================================================================

#include <StatisticsManager.h>


class GameStatisticsData
{
public:
	VOID						Initialize()
	{
		m_StatisticsManager.Init();
		m_ui64TotalGetMoney			= 0;
		m_ui64TotalLoseMoney		= 0;
		m_ui64TotalUserMoney		= 0;

		m_ui64WarehouseGetMoney		= 0;
		m_ui64WarehouseLoseMoney	= 0;
		m_ui64WarehouseUserMoney	= 0;

		m_usJackpotCnt				= 0;
		m_ui64JackpotMoney			= 0;
	}
public:
	//====================================================================================================
	// <INTERFACEs>
	//


	//----------------------------------------------------------------------------------------------------
	// Common/StatisticsManager에 관련한 처리 루틴들
	//
	//

	// 변경된 통계(_WHATTHEHELL_STATISTICS_CHANGE_20060621)에 맞춘 통화량 통계
	inline VOID					AddStatistics( eSTATISTICS_TYPE etype, ULONGLONG& ui64Statistics )
	{ m_StatisticsManager.PlusStatisticType( etype, ui64Statistics ); }

	inline VOID					ClearStatistics( eSTATISTICS_TYPE etype )
	{ m_StatisticsManager.ClearStatisticType( etype ); }

	inline VOID					ClearAllStatistics()
	{
		m_ui64TotalUserMoney = 0;
		m_ui64WarehouseUserMoney = 0;

		for( DWORD dwtype = eSTATISTIC_MONEY_CIRCULATION ; dwtype < eSTATISTIC_MAX ; ++dwtype )
		{
			m_StatisticsManager.ClearStatisticType( (eSTATISTICS_TYPE)dwtype );
		}
	}

	inline ULONGLONG			GetStatisticsType( eSTATISTICS_TYPE etype )
	{ return m_StatisticsManager.GetStatisticType( etype ); }

	//
	//
	//----------------------------------------------------------------------------------------------------
	//
	// 통화량
	//

	inline VOID					SetAveMoney( ULONGLONG& ui64UserMoney, ULONGLONG& ui64WarehouseMoney )
	{
		m_ui64TotalUserMoney		= ui64UserMoney;
		m_ui64WarehouseUserMoney	= ui64WarehouseMoney;
	}

	inline VOID					SetMoney( ULONGLONG& ui64Get, ULONGLONG& ui64Lose, ULONGLONG& ui64User )
	{
		// 얻은돈과 잃은 돈은 WOPS에서 달라고 할 때까지 쌓아 뒀다가 달라고 하면 주면서 초기화한다

		m_ui64TotalGetMoney = ( m_ui64TotalGetMoney < (ULLONG_MAX - ui64Get) ) ?
			m_ui64TotalGetMoney + ui64Get : ULLONG_MAX ;

		m_ui64TotalLoseMoney = ( m_ui64TotalLoseMoney < (ULLONG_MAX - ui64Lose) ) ?
			m_ui64TotalLoseMoney + ui64Lose : ULLONG_MAX ;

		m_ui64TotalUserMoney = ui64User;
	}

	inline ULONGLONG			GetTotalGetMoney() { return m_ui64TotalGetMoney; }
	inline ULONGLONG			GetTotalLoseMoney() { return m_ui64TotalLoseMoney; }
	inline ULONGLONG			GetAveUserMoney() { return m_ui64TotalUserMoney; }

	//
	//
	//----------------------------------------------------------------------------------------------------
	//
	// 창고 통화량
	//

	inline VOID					SetWarehouseMoney( ULONGLONG& ui64Get, ULONGLONG& ui64Lose, ULONGLONG& ui64User )
	{
		m_ui64WarehouseGetMoney = ( m_ui64WarehouseGetMoney < (ULLONG_MAX - ui64Get) ) ?
			m_ui64WarehouseGetMoney + ui64Get : ULLONG_MAX ;

		m_ui64WarehouseLoseMoney = ( m_ui64WarehouseLoseMoney < (ULLONG_MAX - ui64Lose) ) ?
			m_ui64WarehouseLoseMoney + ui64Lose : ULLONG_MAX ;

		m_ui64WarehouseUserMoney = ui64User;
	}

	inline ULONGLONG			GetWarehouseGetMoney() { return m_ui64WarehouseGetMoney; }
	inline ULONGLONG			GetWarehouseLoseMoney() { return m_ui64WarehouseLoseMoney; }
	inline ULONGLONG			GetAveWarehouseMoney() { return m_ui64WarehouseUserMoney; }

	inline VOID					ClearMoney()
	{
		m_ui64TotalGetMoney		= 0;
		m_ui64TotalLoseMoney	= 0;
		m_ui64TotalUserMoney	= 0;
	}

	inline VOID					ClearWarehouseMoney()
	{
		m_ui64WarehouseGetMoney		= 0;
		m_ui64WarehouseLoseMoney	= 0;
		m_ui64WarehouseUserMoney	= 0;
	}

	//
	//
	//----------------------------------------------------------------------------------------------------
	//
	// 잭팟관련
	//

	inline VOID					SetJackpotMoney( USHORT& usJackpotCnt, ULONGLONG& ui64JackpotMoney )
	{
		m_usJackpotCnt += usJackpotCnt;
		m_ui64JackpotMoney += ui64JackpotMoney;
	}

	inline VOID					GetJackpotInfo( USHORT& usJackpotCnt, ULONGLONG& ui64JackpotMoney )
	{
		usJackpotCnt += m_usJackpotCnt;
		ui64JackpotMoney += m_ui64JackpotMoney;
	}

	inline VOID					ClearJackpotMoney()
	{
		m_usJackpotCnt = 0;
		m_ui64JackpotMoney = 0;
	}
private:
	StatisticsManager	m_StatisticsManager;		// 유저/창고 보유 금액을 제외한 모든 통계 관리용

	struct
	{
		ULONGLONG		m_ui64TotalGetMoney;		// 일정 시간 동안 생겨난 돈 총계
		ULONGLONG		m_ui64TotalLoseMoney;		// 일정 시간 동안 없어진 돈 총계
		ULONGLONG		m_ui64TotalUserMoney;		// 현재 동접 유저들이 갖고 있는 평균 금액

		ULONGLONG		m_ui64WarehouseGetMoney;	// 일정 시간 동안 생겨난 돈 총계
		ULONGLONG		m_ui64WarehouseLoseMoney;	// 일정 시간 동안 없어진 돈 총계
		ULONGLONG		m_ui64WarehouseUserMoney;	// 현재 동접 유저들이 갖고 있는 평균 금액

		ULONGLONG		m_ui64JackpotMoney;			// 일정 시간 동안 잭팟 터진 금액 합계
		USHORT			m_usJackpotCnt;				// 일정 시간 동안 잭팟 터진 횟수
	};
};

class MSG_MO_RTTG_USERMONEY_ANS;
class MSG_MO_RTTG_GAMESTATISTICS_ANS;

class GameStatistics
{
public:
	// 게임서버들의 통화량 처리 > OpServer로 전송하기 위해 사용되므로 인자를 패킷으로...
	// 구 Interface -> CheckServerMoney
	static VOID		SendPacket_SumMoneyCirculation( MSG_MO_RTTG_USERMONEY_ANS* pMsgANS );
	// 기타 통계 집계 -> 통화량이 아닌 다른 정보들
	// 구 Interface -> CheckStatistics
	static VOID		SendPacket_StatisticsEtcInfo( MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS );

	// 창고 통화량 <- NOT USED
	VOID	CheckServerWarehouseMoney();
	// <REPLACED>
	VOID	SendMoneyReq();

private:
	static VOID		MakePacket_SumMoneyCirculation_GatheringInfo(
													GameStatisticsData* pGameData,
													MSG_MO_RTTG_USERMONEY_ANS* pMsgANS );
	static VOID		MakePacket_StatisticsEtcInfo_GatheringInfo(
													GameStatisticsData* pGameData,
													MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS );
};


