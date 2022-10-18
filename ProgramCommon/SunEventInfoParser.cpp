#include "StdAfx.h"
#include "SunEventInfoParser.h"
#include "Define.h"
#include <iostream>

//==================================================================================================

BOOL EVENT_INFO::CheckEvent(time_t curtime)
{
    if (curtime < m_start_time || curtime > m_end_time) {
        return false;
    }
    struct tm* tmCurTime = localtime(&curtime);
    if (!tmCurTime) {
        return true;
    }
    // ���� üũ
    if (m_wEventDayFlag)
    {
        eEVENT_DAY_FLAG eDayFlag = GetDayFlag(tmCurTime->tm_wday);
        if (!(eDayFlag & m_wEventDayFlag)) {
            return false;
        }
    }

    return true;
}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
BOOL EVENT_INFO::SetGMEventWithoutID(eEVENT_MODE event_mode, int start_date, WORD start_time, 
                                     int end_date, WORD end_time, int reward_ratio)
{
    tm tm_date = { };

    //StartDate ó��-------------------------------------------
    tm_date.tm_year = start_date / 10000 - 1900;
    tm_date.tm_mon = ( start_date % 10000 ) / 100 - 1;
    tm_date.tm_mday = start_date % 100;
    tm_date.tm_hour = start_time / 100;
    tm_date.tm_min = start_time % 100;
    if( tm_date.tm_hour == 24 )	
    {
        tm_date.tm_hour = 23;
        tm_date.tm_min = 59;
        tm_date.tm_sec = 59;
    }
    // struct tm�� time_t�� ��ȯ�Ѵ�.
    m_start_time = _mktime64( &tm_date );

    //EndDate ó��---------------------------------------------
    tm_date.tm_year = end_date / 10000 - 1900;
    tm_date.tm_mon = ( end_date % 10000 ) / 100 - 1;
    tm_date.tm_mday = end_date % 100;
    tm_date.tm_hour = end_time / 100;
    tm_date.tm_min = end_time % 100;
    if( tm_date.tm_hour == 24 )	
    {
        tm_date.tm_hour = 23;
        tm_date.tm_min = 59;
        tm_date.tm_sec = 59;
    }
    // struct tm�� time_t�� ��ȯ�Ѵ�.
    m_end_time = _mktime64( &tm_date );

    //---------------------------------------------------------------------------------------------
    m_wEventDayFlag = 0;
    m_byEventMode = event_mode;
    m_wPeakTimeStart = 0;
    m_wPeakTimeEnd = 2400;
    m_byApplyToLvRng[EVENT_INFO::LEVEL_MIN] = 1;
    m_byApplyToLvRng[EVENT_INFO::LEVEL_MAX] = GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st();
    m_fRewardExpRate = (float)(reward_ratio / 100.0f);

    //ID�� ������ �����ϵ��� �Ѵ�
    m_dwEventID = 0;
    //Gm�� ���� �̺�Ʈ���� ǥ��
    is_gm_event_ = true;
    //�̸��˸� Ȯ���÷���
    is_pre_notify_ = false;

    return true;
}
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

//==================================================================================================

SunEventInfoParser::SunEventInfoParser()
{
}

SunEventInfoParser::~SunEventInfoParser()
{
	Unload();
}

VOID SunEventInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );

	m_mapEventSubInfo.clear();
}

VOID SunEventInfoParser::Release()
{
	Unload();
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL	SunEventInfoParser::_Load( BOOL bReload )
{
    // TODO(WAVERIX) �ϱ� GetDataNumber�� ����� Ÿ�Կ� �°� ��ȯ�ϰ� disabled warning�� ��������.
    __UNUSED(bReload);
	int nRowSize = GetRowSize();
	EVENT_INFO	EventInfo;
	TCHAR szTemp[MAX_FIELD_NAME_SIZE] = {0,};

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		struct tm tmDate = { };
		DWORD dwDate, dwTime;

		ZeroMemory( &EventInfo, sizeof(EventInfo) );

		EventInfo.m_dwEventID   = GetDataNumber( "Index", nRow );
        // _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
        // ��ũŸ�� �̺�Ʈ�� ��� NpcCode�� ���� ��� ���� �ĺ��ڷ� ����
        // 0�� ��� ���� ����, 1~255�� ��� ���� ��ȣ
		EventInfo.m_NpcCode	    = GetDataNumber( "NPC"  , nRow );
		EventInfo.m_byEventMode = GetDataNumber( "EventMode", nRow );

		// �̺�Ʈ ���۳�¥�� �����Ѵ�.
		dwDate = GetDataNumber( "StartDate", nRow );
		tmDate.tm_year = dwDate / 10000 - 1900;
		tmDate.tm_mon = ( dwDate % 10000 ) / 100 - 1;
		tmDate.tm_mday = dwDate % 100;
		dwTime = GetDataNumber( "StartTime", nRow );
		tmDate.tm_hour = dwTime / 100;
		tmDate.tm_min = dwTime % 100;
		if( tmDate.tm_hour == 24 )	
		{
			tmDate.tm_hour = 23;
			tmDate.tm_min = 59;
			tmDate.tm_sec = 59;
		}
		
		// struct tm�� time_t�� ��ȯ�Ѵ�.
		EventInfo.m_start_time = _mktime64( &tmDate ); 

		// �̺�Ʈ ���ᳯ¥�� �����Ѵ�.
		dwDate = GetDataNumber( "EndDate", nRow );
		tmDate.tm_year = dwDate / 10000 - 1900;
		tmDate.tm_mon = ( dwDate % 10000 ) / 100 - 1;
		tmDate.tm_mday = dwDate % 100;
		dwTime = GetDataNumber( "EndTime", nRow );
		tmDate.tm_hour = dwTime / 100;
		tmDate.tm_min = dwTime % 100;
		if( tmDate.tm_hour == 24 )	
		{
			tmDate.tm_hour = 23;
			tmDate.tm_min = 59;
			tmDate.tm_sec = 59;
		}

		// struct tm�� time_t�� ��ȯ�Ѵ�.
		EventInfo.m_end_time = _mktime64( &tmDate );
		EventInfo.m_wEventDayFlag	= GetDataNumber( "EventDayFlag", nRow );
		EventInfo.m_byCountMax = GetDataNumber( "CountMax", nRow );
		EventInfo.m_byApplyToLvRng[EVENT_INFO::LEVEL_MIN] = GetDataNumber( "ApplyToMinLv", nRow );
		EventInfo.m_byApplyToLvRng[EVENT_INFO::LEVEL_MAX] = GetDataNumber( "ApplyToMaxLv", nRow );

		for( int i=0; i<_MAX_EVENT_REGITEM; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "RegisterItem%d\0", i+1 );
			EventInfo.m_dwRegisterItemIndex[i]	= GetDataNumber( szTemp, nRow );
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "RegisterItemNum%d\0", i+1 );
			EventInfo.m_wRegisterItemNum[i]		= GetDataNumber( szTemp, nRow );
		}

		EventInfo.m_wPeakTimeStart		= GetDataNumber( "PeakTimeSet", nRow );
		EventInfo.m_wPeakTimeEnd		= GetDataNumber( "PeakTimeEnd", nRow );

		EventInfo.m_dwItemCraftIndex	= GetDataNumber( "ItemCraftIndex", nRow );
		EventInfo.m_dwItemEnchantIndex	= GetDataNumber( "ItemEnchantIndex", nRow );
		EventInfo.m_dwItemDecompoIndex	= GetDataNumber( "ItemDecompoIndex", nRow );

		EventInfo.m_fRewardExpRate		= GetDataNumber( "RewardExpRate", nRow );
		if( EventInfo.m_fRewardExpRate >= 4 )
			SUNLOG( eCRITICAL_LOG, "[SunEventInfoParser] PeakTime ExpRatio[%f] is big!\n", EventInfo.m_fRewardExpRate );

		EventInfo.m_fRewardExpRate2	= GetDataNumber( "CharCreateExpRate", nRow );
		if( EventInfo.m_fRewardExpRate2 >= 4 )
			SUNLOG( eCRITICAL_LOG, "[SunEventInfoParser] PeakTime ExpRatio2[%f] is big!\n", EventInfo.m_fRewardExpRate2 );

		EventInfo.m_fRewardHimeRate		= GetDataNumber( "RewardHimeRate", nRow );
		EventInfo.m_fRewardItemDropRate	= GetDataNumber( "RewardItemDropRate", nRow );

		for(int i=0; i<_MAX_EVENT_REWARD; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "NormalReward1Index%d\0", i+1 );
			EventInfo.m_dwRewardInfo[i]	= GetDataNumber( szTemp, nRow );
		}

		for(int i = 0 ; i < _MAX_EVENT_REWARD; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "NormalReward2Index%d\0", i+1 );
			EventInfo.m_dwPCRoomRewardInfo[i]	= GetDataNumber( szTemp, nRow );
		}

		EventInfo.m_EventRewardCode	= GetDataNumber( "EventReward", nRow );
		EventInfo.m_IsDeleteFlagForRewardedItem	= GetDataNumber( "Delete", nRow );
		for( DWORD i=0 ; i<EVENT_INFO::_MAX_DELETE_ITEM_CODES ; ++i )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "ItemCode%d\0", i+1 );
			EventInfo.m_DeleteRewaredItemCode[i]	= GetDataNumber( szTemp, nRow );
		}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
        EventInfo.is_gm_event_ = false;
        EventInfo.is_pre_notify_ = false;
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
#ifdef __NA_0_20061109_EXP_EVENT_
		for(int i=0; i<EVENT_INFO::_MAX_EXPRATE_NUM; i++ )
		{
			EventInfo.m_fPlayTimeExpRate[i] = GetDataNumber( "EXPEvent", nRow );
		}
#endif

    	// ��¥ ���� �̺�Ʈ�� ��쿡�� ���ʿ� �������� �ʴ´�
		time_t cur_time;	util::TimeSync::time( &cur_time );
		if( cur_time > EventInfo.m_end_time )
		{
//			SUNLOG( eCRITICAL_LOG, "SunEvent Script Time Over[%u]\n", EventInfo.m_dwEventID );
			continue;
		}

		m_mapEventInfo.insert( EVENTINFO_PAIR( EventInfo.m_dwEventID, EventInfo ) );

		EVENT_SUB_INFO	eventSubInfo;
		eventSubInfo.m_dwEventID	= EventInfo.m_dwEventID;
		eventSubInfo.m_byEventMode	= EventInfo.m_byEventMode;
		eventSubInfo.m_byCountMax	= EventInfo.m_byCountMax;

		for( int i=0; i<_MAX_EVENT_REGITEM; i++ )
		{
			eventSubInfo.m_dwRegisterItemIndex[i]	= EventInfo.m_dwRegisterItemIndex[i];
			eventSubInfo.m_wRegisterItemNum[i]		= EventInfo.m_wRegisterItemNum[i];
		}

		m_mapEventSubInfo.insert(EVENT_SUB_INFO_PAIR(eventSubInfo.m_byEventMode, eventSubInfo));
	}

	return TRUE;
}

#if defined(_SERVER)
BOOL		SunEventInfoParser::_LoadEventReward( BOOL bReload )
{
    // TODO(WAVERIX) �ϱ� GetDataNumber�� ����� Ÿ�Կ� �°� ��ȯ�ϰ� disabled warning�� ��������.
    __UNUSED(bReload);
	int nRowSize = GetRowSize();
	EVENT_MINMAX_TYPE_INFO	eventReward;
	TCHAR szTemp[MAX_FIELD_NAME_SIZE] = {0,};

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		const EVENT_MINMAX_TYPE_INFO::INDEX_T erID = GetDataNumber( "Index", nRow );
		if( m_MinMaxMap.find( erID ) != m_MinMaxMap.end() )
		{
			SUNLOG( eCRITICAL_LOG, "Script Data Fail, File =  %s, Code = %u", m_pszEventFileName, erID );
			FASSERT( !"Duplicated ID" );
			return FALSE;
		}

		ZeroMemory( &eventReward, sizeof(eventReward) );
		eventReward.m_Index = erID;

#ifdef _NA_20100307_BETAKEY_SYSTEM
        eventReward.m_BeytaKey                = GetDataInt("BetaKeyCode", nRow);
#endif

		eventReward.m_MaxGroupRound = GetDataNumber( "MaxRoot", nRow );
		if( EVENT_MINMAX_TYPE_INFO::_TOTAL_ROUND_UNLIMITED == eventReward.m_MaxGroupRound )
			eventReward.m_MaxGroupRound = ULONG_MAX;
		eventReward.m_LimitHeimPerDonation = GetDataNumber( "MinGiveHime", nRow );
		EVENT_MINMAX_TYPE_INFO::NODE* pOffset = eventReward.m_Nodes;
		for( INT index=0 ; index<EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT ; ++index, ++pOffset )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MinNum%d\0", index+1 );
			pOffset->Min = GetDataNumber( szTemp, nRow );
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MaxNum%d\0", index+1 );
			pOffset->Max = GetDataNumber( szTemp, nRow );
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "CountNum%d\0", index+1 );
			eventReward.m_Limits[index] = GetDataNumber( szTemp, nRow );
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "DropCode%d\0", index+1 );
			eventReward.m_DropCodes[index] = GetDataNumber( szTemp, nRow );	// DropCode�������� lazy check�� ����.

			// ����
			if( index && !pOffset->Min ) // �� ���ǿ� �ɸ���, ������� �ʴ� �׷��̴�.
			{
				pOffset->Min = ULONG_MAX;
				pOffset->Max = ULONG_MAX;
			}
		}

		// make gap nodes
		pOffset = eventReward.m_Nodes;
		EVENT_MINMAX_TYPE_INFO::GAP* pGap = (EVENT_MINMAX_TYPE_INFO::GAP*)&eventReward.m_Gaps[0].Max;
		for( DWORD i=0 ; i<EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT ; ++i, ++pGap, ++pOffset )
		{
			*pGap = *(EVENT_MINMAX_TYPE_INFO::GAP*)pOffset;
			pGap->Min = ULONG_MAX == pGap->Min ? ULONG_MAX : --pGap->Min;
			pGap->Max = pGap->Max < pGap->Min ? pGap->Min : pGap->Max;
			//++pGap->Max;
		}
		eventReward.m_Gaps[0].Min = 0;
		eventReward.m_Gaps[EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT].Max = ULONG_MAX;

		m_MinMaxMap.insert( std::make_pair(erID, eventReward) );
	};

	return TRUE;
}
#endif

#pragma warning ( pop )


//��������������������������������������������������������������������������������������������������������������������������������������������������
//
//
#if defined(_SERVER)

inline static VOID		_FindNodeMinMaxBoundary( EVENT_MINMAX_TYPE_INFO* IN pInfo, const DWORD money, BOOL& OUT bIsGap, DWORD& nodeIdx )
{
	bIsGap = pInfo->m_Gaps[EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT].Min <= money;
	nodeIdx = bIsGap ? EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT : 0;

	if( !bIsGap )
	{
		EVENT_MINMAX_TYPE_INFO::NODE* pNode = pInfo->m_Nodes;
		EVENT_MINMAX_TYPE_INFO::GAP* pGap = pInfo->m_Gaps;
		for( ; nodeIdx<EVENT_MINMAX_TYPE_INFO::_MAX_GROUPNODE_COUNT ; ++nodeIdx, ++pNode, ++pGap )
		{
			if( pNode->Max < money )
				continue;

			bIsGap = ( (pGap->Min <= money) && (money <= pGap->Max) );
			BOOL bIsCurNode = !bIsGap && ( (pNode->Min <= money) && (money <= pNode->Max) );

			if( bIsGap || bIsCurNode )
				break;
		}
	}
}

VOID	SunEventInfoParser::FindMinMaxInfoByBoundary(
											sEVENT_MINMAX_QUERY& IN rARG,
											sEVENT_MINMAX_RESULT& OUT rResult
											)
{
	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		MONEY totalSum = (MONEY)rARG.donatedMoney + rARG.addinMoney;
		IfFailThenBreak(SAFE_NUMERIC_TYPECAST(MONEY, totalSum, DWORD), 10);
        IfFailThenBreak(!m_MinMaxMap.empty(), 20);

		EVENT_MINMAX_MAP_IT itFIND = m_MinMaxMap.find(rARG.linkedID);
        IfFailThenBreak(itFIND != m_MinMaxMap.end(), 30);

		EVENT_MINMAX_TYPE_INFO& rINFO = itFIND->second;
		EVENT_MINMAX_TYPE_INFO::NODE* pNode = rINFO.m_Nodes;

		{	// Check Constraint
			if( rARG.bSkipCheckHeims == FALSE )
			{
				const EVENT_MINMAX_TYPE_INFO::HEIM_BOUND_T limitHeim = rINFO.m_LimitHeimPerDonation;
				IfFailThenBreak(limitHeim <= rARG.addinMoney, 50);
			}

			const EVENT_MINMAX_TYPE_INFO::ROUND_T limitRound = rINFO.m_MaxGroupRound;
			IfFailThenBreak(limitRound > rARG.curTotalRounds, 60);
		};

		BOOL bIsGapCur = FALSE;
		DWORD nodeIdxCur = 0;

		_FindNodeMinMaxBoundary( &rINFO, rARG.donatedMoney, bIsGapCur, nodeIdxCur );
		//printf( "result isGap %u : curNode %u\n", bIsGapCur, nodeIdxCur );

		BOOL bIsGapNext = FALSE;
		DWORD nodeIdxNext = 0;
		_FindNodeMinMaxBoundary( &rINFO, (DWORD)totalSum, bIsGapNext, nodeIdxNext );
		//printf( "result isGap %u : curNode %u\n", bIsGapNext, nodeIdxNext );

		// bIsGapNext == TRUE �� ���, ������ �׷쿡 �����ִ��� ���θ� �Ǵ��Ѵ�.
		BOOL bRoundUp = bIsGapNext && (ULONG_MAX == rINFO.m_Nodes[nodeIdxNext].Min);
		if( bIsGapCur && bIsGapNext )
		{
			// ���� ���� �� �ִ� ���°� �ƴϴ�. ������ ��ü ������ �ʿ��� �� �𸥴�.
			if( bRoundUp )
			{
				//rResult.m_Result = sEVENT_MINMAX_RESULT::CNTL_ROUND_UP;
				//ASSERT( !"Not Imementation" );
				if(FALSE == flow.FCAssert(nodeIdxNext != 0))
				{
                    SetFailFc(nodeIdxNext != 0, 150);
					break;
				}
				const DWORD upperboundHeim = rINFO.m_Nodes[nodeIdxNext-1].Max;
				DWORD remains = (DWORD)(totalSum % upperboundHeim);
				sEVENT_MINMAX_QUERY query( rARG.linkedID, 0, remains, rARG.pEventInfo, 0, rARG.curTotalRounds, TRUE );
				FindMinMaxInfoByBoundary( query, rResult );
				// �����̸�, totalround ����
				if( sEVENT_MINMAX_RESULT::CNTL_FAIL != rResult.m_Result )
					++rResult.m_TotalRounds;
				return;
			}
			else
			{
				// ���� ��ġ ������Ų��.
				rResult.SetInfo( sEVENT_MINMAX_RESULT::CNTL_UPDATE, 0, rARG.curRewardedCount, (DWORD)totalSum, rARG.curTotalRounds );
			}
		}

		if( bIsGapCur && !bIsGapNext )
		{
			// ���� �׷��� ������ �޴´�.
			rResult.SetInfo( sEVENT_MINMAX_RESULT::CNTL_SUCCESS, rINFO.m_DropCodes[nodeIdxNext], 1, (DWORD)totalSum, rARG.curTotalRounds );
		}

		if( !bIsGapCur && bIsGapNext )
		{
			if( bRoundUp )
			{
				if(FALSE == flow.FCAssert(nodeIdxNext != 0))
				{
                    SetFailFc(nodeIdxNext != 0, 160);
					break;
				}
				const DWORD upperboundHeim = rINFO.m_Nodes[nodeIdxNext-1].Max;
				DWORD remains = (DWORD)(totalSum % upperboundHeim);
				sEVENT_MINMAX_QUERY query( rARG.linkedID, 0, remains, rARG.pEventInfo, 0, rARG.curTotalRounds, TRUE );
				FindMinMaxInfoByBoundary( query, rResult );
				// �����̸�, total round ����
				if( sEVENT_MINMAX_RESULT::CNTL_FAIL != rResult.m_Result )
					++rResult.m_TotalRounds;
				return;
			}
			else
			{
				rResult.SetInfo( sEVENT_MINMAX_RESULT::CNTL_UPDATE, 0, 0, (DWORD)totalSum, rARG.curTotalRounds );
			}
		}

		if( !bIsGapCur && !bIsGapNext )
		{	// ���� �� ���� ��Ȳ�� �����Ѵ�. (1) ���� ����, (2) ���� ~ ���� ������ �ѹ��� �پ� ���� ���
			DWORD calcedRewardedCnt = (nodeIdxCur == nodeIdxNext) ?
				rARG.curRewardedCount+1 /*���� ����*/ 
				: 1 /*���� �׷� �ǳ� �ٰ� ���� �׷��� ������ �����Ѵ�. ���� �׷��� ������ �޴´�.*/;
			const EVENT_MINMAX_TYPE_INFO::ROUND_T limitCount = rINFO.m_Limits[nodeIdxNext];
			( !limitCount || (calcedRewardedCnt > limitCount) ) ?
				rResult.SetInfo( sEVENT_MINMAX_RESULT::CNTL_UPDATE, 0, limitCount, (DWORD)totalSum, rARG.curTotalRounds )
				: rResult.SetInfo( sEVENT_MINMAX_RESULT::CNTL_SUCCESS, rINFO.m_DropCodes[nodeIdxNext], calcedRewardedCnt, (DWORD)totalSum, rARG.curTotalRounds );
		}

		// ������ ��� ���� <- ���� ������ ���, ���� ȸ�� ���� �� �ּ� ���� ��η� �˻�
		if( sEVENT_MINMAX_RESULT::CNTL_FAIL != rResult.m_Result )
		{	// �� ���� TRUE == ( !bIsGapNext ) ������ ���̴�.
			// nodeIdxNext �� ���Ǵ� ���� ����
			const EVENT_MINMAX_TYPE_INFO::ROUND_T limitCount = rINFO.m_Limits[nodeIdxNext];
			IfFailThenBreak(limitCount >= rResult.m_RewaredCount, 100);
		}

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case 10: case 20: case 30:
	case 50: case 60:	// �ּ� ���� ��η� �̸�, ��ü ���� ���Ѱ� ����
	case 100: case 110:	// ���� ���� ����, �׷� ���� ȸ�� ���Ѱ� ����
	case 150: case 160:	// Assertion Failed
	default:
		ZeroMemory( &rResult, sizeof(rResult) );
		rResult.m_Result = sEVENT_MINMAX_RESULT::CNTL_FAIL;
		switch( flow.GetErrorCode() )
		{
		case 50: rResult.m_ResultCode = RC::RC_EVENT_DONATION_HEIM_LIMIT; break;
		case 60: rResult.m_ResultCode = RC::RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW; break;
		}
		break;
	}

	return /*RETURN FALSE VALUE*/;
}
#endif



EVENT_INFO * SunEventInfoParser::FindEventInfo( CODETYPE dwEventID )
{
	EVENTINFO_MAP_ITER	iter = m_mapEventInfo.find( dwEventID );

	if( iter == m_mapEventInfo.end() )
	{
		return NULL;
	}

	return &( (*iter).second );
}

VOID SunEventInfoParser::Unload()
{
	m_mapEventInfo.clear();
	m_mapEventSubInfo.clear();
}

EVENT_INFO *	SunEventInfoParser::FindEventInfoFromMode( eEVENT_MODE eMode )
{
	EVENT_SUB_INFO_ITER iter = m_mapEventSubInfo.find( eMode );

	if( iter != m_mapEventSubInfo.end() )
	{
		EVENT_SUB_INFO* pSubInfo = &(*iter).second;
		if( pSubInfo )
			return FindEventInfo( pSubInfo->m_dwEventID );
	}

	return NULL;
}

//������ �̺�Ʈ�� ��쿡�� ����Ѵ�.
EVENT_INFO_VEC*	SunEventInfoParser::FindEventInfoFromNpcType( CODETYPE dwNpcType )
{
    m_vecFindedEventInfo.clear();
	EVENTINFO_MAP_ITER iter = m_mapEventInfo.begin();
	while(iter != m_mapEventInfo.end())
	{
		EVENT_INFO* pEventInfo = &(*iter).second;
		if(pEventInfo->m_NpcCode == dwNpcType)
		{
			m_vecFindedEventInfo.push_back(pEventInfo);
		}
		++iter;
	}

	if(0 == m_vecFindedEventInfo.size())
		return NULL;
	else
		return &m_vecFindedEventInfo;
}


BOOL	SunEventInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_SUNEVENTINFO:
		return _Load( bReload );
#if defined(_SERVER)
	case SCRIPT_EVENTREWARD_INFO:
		return _LoadEventReward( bReload );
#endif
	}

	return FALSE; 	
}

