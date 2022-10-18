#pragma once

// ================================================================================================
//   ���� ������ ���� ����
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
	// Common/StatisticsManager�� ������ ó�� ��ƾ��
	//
	//

	// ����� ���(_WHATTHEHELL_STATISTICS_CHANGE_20060621)�� ���� ��ȭ�� ���
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
	// ��ȭ��
	//

	inline VOID					SetAveMoney( ULONGLONG& ui64UserMoney, ULONGLONG& ui64WarehouseMoney )
	{
		m_ui64TotalUserMoney		= ui64UserMoney;
		m_ui64WarehouseUserMoney	= ui64WarehouseMoney;
	}

	inline VOID					SetMoney( ULONGLONG& ui64Get, ULONGLONG& ui64Lose, ULONGLONG& ui64User )
	{
		// �������� ���� ���� WOPS���� �޶�� �� ������ �׾� �״ٰ� �޶�� �ϸ� �ָ鼭 �ʱ�ȭ�Ѵ�

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
	// â�� ��ȭ��
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
	// ���̰���
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
	StatisticsManager	m_StatisticsManager;		// ����/â�� ���� �ݾ��� ������ ��� ��� ������

	struct
	{
		ULONGLONG		m_ui64TotalGetMoney;		// ���� �ð� ���� ���ܳ� �� �Ѱ�
		ULONGLONG		m_ui64TotalLoseMoney;		// ���� �ð� ���� ������ �� �Ѱ�
		ULONGLONG		m_ui64TotalUserMoney;		// ���� ���� �������� ���� �ִ� ��� �ݾ�

		ULONGLONG		m_ui64WarehouseGetMoney;	// ���� �ð� ���� ���ܳ� �� �Ѱ�
		ULONGLONG		m_ui64WarehouseLoseMoney;	// ���� �ð� ���� ������ �� �Ѱ�
		ULONGLONG		m_ui64WarehouseUserMoney;	// ���� ���� �������� ���� �ִ� ��� �ݾ�

		ULONGLONG		m_ui64JackpotMoney;			// ���� �ð� ���� ���� ���� �ݾ� �հ�
		USHORT			m_usJackpotCnt;				// ���� �ð� ���� ���� ���� Ƚ��
	};
};

class MSG_MO_RTTG_USERMONEY_ANS;
class MSG_MO_RTTG_GAMESTATISTICS_ANS;

class GameStatistics
{
public:
	// ���Ӽ������� ��ȭ�� ó�� > OpServer�� �����ϱ� ���� ���ǹǷ� ���ڸ� ��Ŷ����...
	// �� Interface -> CheckServerMoney
	static VOID		SendPacket_SumMoneyCirculation( MSG_MO_RTTG_USERMONEY_ANS* pMsgANS );
	// ��Ÿ ��� ���� -> ��ȭ���� �ƴ� �ٸ� ������
	// �� Interface -> CheckStatistics
	static VOID		SendPacket_StatisticsEtcInfo( MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS );

	// â�� ��ȭ�� <- NOT USED
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


