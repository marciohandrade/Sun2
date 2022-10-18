#pragma once

#include <Struct.h>
#include <SSQ/SSQHandlerBase.h>
#include <SSQMonster.h>

class Player;
class ISSQHandlerInGame;	//	GameServer ��ü ���� ������ �̴� Player_NestedInfo.hxx �� ��������.
struct SSQInfo;


//��������������������������������������������������������������������������������������������������������������������������������
//	<SSQHandlerInGame>
//
class SSQHandlerInGame : public SSQHandlerBase< SSQHandlerInGame >
{
public:		SSQHandlerInGame();
			~SSQHandlerInGame();

public:		BOOLEAN					IsExistRewardInfo() const;	// Ư�� Ÿ���� �����ϸ�, ���� �˷����� ���� (Status=0)������ ��´�.
			BOOLEAN					Send_RewardProcess( Player* const pPlayer, const BOOLEAN bShowUI = TRUE );
			BOOLEAN					Send_RewardUpdate( Player* const pPlayer );

			BOOLEAN					SetInfo_Empty();
			BOOLEAN					SetInfo_WinnerReward( const DWORD point, const DWORD exp, const BYTE mobkills, const BYTE playerkills, const SLOTCODE reward_itemcode );
			BOOLEAN					SetInfo_QuestFailed( const DWORD point, const DWORD exp, const BYTE mobkills, const BYTE playerkills );
			BOOLEAN					SetInfo_Refund( const WORD itemCode, const DWORD heim );
			//	�÷��� ���߿� �����Ǵ� ������
			BOOLEAN					UpdateInfo_PlayPoint( const DWORD point=0, const DWORD exp=0, const BYTE mobkills=0, const BYTE playerkills=0 );
			BOOLEAN					UpdateInfo_ChangeToWinner();
			BOOLEAN					GetInfo_QuestFail( SSQ_RESERVED_REWARD_INFO::QuestFail& rInfo );
			BOOLEAN					GetInfo_QuestWinner( SSQ_RESERVED_REWARD_INFO::WinnerReward& rInfo );

public:		enum eOrder { eOrder_1st, eOrder_2nd, eOrder_3rd, eOrder_None };
	inline	const eOrder			GetOrder() const { return (eOrder)m_Order; }
	inline	VOID					SetOrder( const eOrder order ) { m_Order = (BYTE)order; }

	static const DWORD				LeaveTimeout_OnDead	= 5*1000;
public:		BOOLEAN					ConfigEvent_OnDead( Player* const pPlayer, const DWORD leave_delay );
			BOOLEAN					QueryEvent_NeedLeaveProcess( Player* const pPlayer );

public:
	inline	BYTE					GetObserverCoordinateIndex() const { return m_ObserverCoordinateIndex; }
	inline	VOID					SetObserverCoordinateIndex( const BYTE coordinate_index ) { m_ObserverCoordinateIndex = coordinate_index; }
public: //__NA001230_SSQ_UPDATE_ADD_EXP_FIELD_SUPPORT__
	inline	INT						GetBonusExpRatio() const { return m_BonusExpRatio; }
	inline	VOID					SetBonusExpRatio( const INT bonus_exp_ratio ) { m_BonusExpRatio = bonus_exp_ratio; }

public:		SSQCharacterStatusInGame	ControlStatus;
private:	BYTE						m_Order;
			BYTE						m_ObserverCoordinateIndex;
			INT							m_BonusExpRatio;	//	�����ֱ�� ���ʽ� ����ġ �ӽ� �����
			ITimerBase					m_DeadTimer;
    __DISABLE_COPY(SSQHandlerInGame);
};

