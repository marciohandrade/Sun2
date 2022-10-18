#pragma once

#include <SSQ/SSQInfoParser.h>
#include <Status_Operator.h>

class GameField;
class GameInstanceDungeon;
class ClosedSpaceRangeTiles;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQDistributionChart>
//
class SSQDistributionChart : public SectorGridProxy< SSQDistributionChart >
{
	typedef SectorGridProxy< SSQDistributionChart >	base_type;
public:
	typedef ViewBoxCubic			ViewBoxForClosedSpace;
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    // 타락한사원 경계지역 중점 변경..
    #define SSQClosedSpaceCenter_X		(199.70f)
    #define SSQClosedSpaceCenter_Y		(200.0f)
#else
    #define SSQClosedSpaceCenter_X		(200.58f)
	#define SSQClosedSpaceCenter_Y		(200.13f)
#endif  // _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
	#define SSQClosedSpaceCenter_Z		(  9.03f)
	#define SSQClosedSpace_DamageRadius	( 24.50f)
	#define SSQThunderbolt_DamageRadius	(  6.00f)
public:

	enum eDISTRIBUTION				{ D_NONE, D_POSITION_WEIGHT, D_THUNDERBOLT_RANDOM };
	enum ePOSITION_WEIGHT			{ WEIGHT_PLAYER=5, WEIGHT_MONSTER=1, WEIGHT_CLOSEDSPACE=1000, WEIGHT_DISABLE=-1, };
	enum eTHUNDERBOLT_RANDOM		{ TR_, };
	static const DWORD				DISABLE_TILE_ATTR	= /*PTA_ONLY_JUMP|*/PTA_NO_WALK;


private:	GameInstanceDungeon*	m_pSSQField;
			GameField*				m_pGameField;
			DWORD					m_TotalTileNumber;
public:
	const	ViewBoxForClosedSpace&	ClosedSpaceViewBox;
	const	WzVector&				CenterPosition;
	const	FLOAT					ClosedSpaceRangeQ2;
private:	WzVector				m_CenterPosition;
			ViewBoxForClosedSpace	m_ViewBoxForClosedSpace;
	static	ClosedSpaceRangeTiles*	ms_pClosedSpaceRangeTiles;

public:		SSQDistributionChart();
			~SSQDistributionChart();

	inline	VOID			SetProjectionGrid( const SectorGrid* pSectorGrid )
			{
				m_pProjectionGrid = const_cast<SectorGrid*>( pSectorGrid );
				Build( m_pProjectionGrid->SectorXYDistance );
			}
	inline	SectorGrid*		GetProjectionGrid() const { return m_pProjectionGrid; }
			GameField*		SetAndGetSSQField( GameInstanceDungeon* const pSSQField );

			VOID			Init() {}
			VOID			Release() {}

			//	<returns>
			//	-	generated mob number
			INT				RandomDistribution( const SSQInfo* const pSSQInfo, const MatchCheckCodeFilters* const pCheckFilter );
			BOOLEAN			Earthquake_Indication(
									const SSQPattern* const pSSQPattern, const BYTE step_array_index
									);
			BOOLEAN			Earthquake(
									const SSQPattern* const pSSQPattern, const BYTE step_array_index
									, nsSSQ::MSGSUB_SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION& INOUT rMsgFPC
									);
			BOOLEAN			Thunderbolt(
									const SSQInfo* const pSSQInfo
									, const BOOLEAN bShowmanship
									, const WzVector* const pRange
									, const BOOLEAN bProcOutRangeClosedSpace, const DWORD max_value
									, nsSSQ::MSGSUB_SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE& INOUT rMsgTTE
									, nsSSQ::MSGSUB_SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS& INOUT rMsgCDP
									);
			BOOLEAN			ClosedSpace_RemoveDanger(
									const SSQInfo* const pSSQInfo
									);
			BOOLEAN			ClosedSpace(
									const SSQInfo* const pSSQInfo
									, const BYTE earthquake_step
									, nsSSQ::MSGSUB_SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE& INOUT rMsgTCE
									);

	//////////////////////////////////////////////////////////////////////////
private:
	friend class ClosedSpaceRangeTiles;
    __DISABLE_COPY(SSQDistributionChart);
};


class GameWarSSQItemDistribution
{
public:		GameWarSSQItemDistribution();
			GameWarSSQItemDistribution( const SSQInfo* const pSSQInfo, const DWORD init_players );
public:
	//	전투 상황에 따라 아이템 확률을 높여간다. 개수 자체는 제한되어 있지만...
	//	eRatio_MaxDegree=10000 --> 100.00%
	enum eRatio {
		//eRatio_Begin		=  0,	// constant table row index
		//eRatio_WarmUp		=  1,	// constant table row index
		//eRatio_Climax		=  2,	// constant table row index
		eRatio_MaxDegree	= 10000,	/*100%*/

		eRatio_MaxStep		= 4,	// 지진 단계 최고 4단계와 동일
	};

public:		SSQInfo::REWARD_NODE		Drop( const BYTE earthquake_step );

private:
	const	SSQInfo* const				m_pSSQInfo;
	const	DWORD						m_InitPlayers;
			BYTE						m_SelectedCol;
			SSQInfo::REWARD_DROP_INFO	m_DropInfoSpace;
    __DISABLE_COPY(GameWarSSQItemDistribution);
};


class GameWarObserverMode
{
	typedef nsSSQ::Observer::Info	Info;
public:		GameWarObserverMode()
				: m_pField(0)
			{}
			GameWarObserverMode( GameInstanceDungeon* const pIndun );

public:		BOOLEAN			Enter( const nsSSQ::Rules::eC2G_ReqRule rules, Player* const pPlayer );
			BOOLEAN			ChangeCoordinates(
									const nsSSQ::Rules::eC2G_ReqRule rules
									, Player* const pPlayer
									, const Info::eKeyCmd key_cmd );

private:	GameField*		m_pField;
};


