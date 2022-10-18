
#include "stdafx.h"
#include "MissionManager.h"
#include "MissionRewardManager.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include <SCItemSlotContainer.h>
#include "Player.h"

SLOTCODE	MissionRewardManager::rewardItemCode[MAX_INVENTORY_SLOT_NUM];
POSTYPE		MissionRewardManager::rewardItemNum[MAX_INVENTORY_SLOT_NUM];

BYTE				MissionRewardManager::GetTypeOfMissionPoint( )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo::sCLASS::sREWARD *pREWARD, *pCurr;

	if( rMRA.GetMissionRewardInfoClassCode() == NULL )
		GetMissionRewardInfoNClassCode();

	pREWARD = pCurr = rMRA.GetMissionRewardInfoClassCode()->REWARDs;

	if( rMRA.GetMissionPoint() > 0 )
	{
		DWORD nextIDX = 1;
		for( DWORD i=0 ; i<MissionRewardInfo::MISSION_REWARD_MAX_SIZE ; ++i )
		{
			if( rMRA.GetMissionPoint() >= (WORD)pCurr[i].m_bMissionNum )
			{
				bool bOK = (MissionRewardInfo::MISSION_REWARD_MAX_SIZE != nextIDX) ?
					(rMRA.GetMissionPoint() < pCurr[nextIDX].m_bMissionNum) : true;

				if( bOK )
					return (BYTE)i;

				pREWARD = &pCurr[nextIDX++];
				continue;
			}
		}
	}

	return MissionRewardInfo::MISSION_REWARD_MAX_SIZE;
}

WORD				MissionRewardManager::GetRewardCode( BOOL bPreUpdate )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo::sCLASS::sREWARD *pREWARD, *pCurr;

	if( rMRA.GetMissionRewardInfoClassCode() == NULL )
	{
		if( GetMissionRewardInfoNClassCode() == NULL )
			return (WORD)0xFFFF;
	}

	pREWARD = pCurr = rMRA.GetMissionRewardInfoClassCode()->REWARDs;

	WORD wMissionPoint = rMRA.GetMissionPoint() + (bPreUpdate ? 1 : 0);

	if( wMissionPoint > 0 )
	{
		DWORD nextIDX = 1;
		for(int i = 0; i < MissionRewardInfo::MISSION_REWARD_MAX_SIZE; ++i)
		{
			WORD wMPoint = (WORD)pCurr[i].m_bMissionNum;
			if( wMPoint && (wMissionPoint >= wMPoint) )
			{
				bool bOK = (MissionRewardInfo::MISSION_REWARD_MAX_SIZE != nextIDX) ?
					(wMissionPoint < pCurr[nextIDX].m_bMissionNum) : true;

				if( bOK )
					return pREWARD->m_wRewardCode;

				pREWARD = &pCurr[nextIDX++];
				continue;
			}
			else
			{	// wMPoint가 0인 경우에는 무한대로 설정하자.
				if( !wMPoint )
				{
                    if(i > 0)
    					return pCurr[i - 1].m_wRewardCode;
                    FlowControl::FCAssert(!"i <= 0");
				}
			}
		}
	}

	return (WORD)0xFFFF;
}

// m_MissionRewardInfo 정보에 의거 계산된다.
MissionRewardInfo*	MissionRewardManager::GetMissionRewardInfoNClassCode()
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	eCHAR_TYPE type = rMRA.GetPlayer()->GetCharType();
	MissionRewardParser* pParser = MissionRewardParser::Instance();

	MissionRewardInfo* pMRI = pParser->GetMissionReward( rMRA.GetNum() );
	if( pMRI )
	{
		rMRA.SetMissionPoint( rMRA.GetMissionManager()->GetMissionPoint(rMRA.GetNum()) );

		MissionRewardInfo::sCLASS* pCLASS = &(pMRI->CLASSs[type]);
		ASSERT( pCLASS->m_bClassCode == type );

		rMRA.SetMissionRewardInfoClassCode( pCLASS );
	}

	return pMRI;
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardItem( INVENTORY_TOTAL_INFO& OUT rInvenInfo )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo* pMRI = GetMissionRewardInfoNClassCode();

	return RewardItem( rMRA.GetMissionRewardInfoClassCode(), rInvenInfo );
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardExp( EXPTYPE* OUT pExp )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo* pMRI = GetMissionRewardInfoNClassCode();

	return RewardExp( rMRA.GetMissionRewardInfoClassCode(), pExp );
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardHime( MONEY* OUT pMoney )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo* pMRI = GetMissionRewardInfoNClassCode();

	return RewardHime( rMRA.GetMissionRewardInfoClassCode(), pMoney );
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardExp( const MissionRewardInfo::sCLASS* IN pMRIclass, EXPTYPE* OUT pExp )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo::sCLASS::sREWARD *pREWARD, *pCurr;

	if( rMRA.GetMissionRewardInfoClassCode() == NULL )
	{
		if( GetMissionRewardInfoNClassCode() == NULL )
			return RC::RC_REWARD_FAILED;
	}

	pREWARD = pCurr = rMRA.GetMissionRewardInfoClassCode()->REWARDs;

	if( ((DWORD)(__int64)pREWARD)<4000 )
	{
		ASSERT( 0 && "뚫렸다!" );
		return RC::RC_REWARD_FAILED;
	}

	if( rMRA.GetMissionPoint() > 0 )
	{
		DWORD nextIDX = 1;
		for(int i = 0; i < MissionRewardInfo::MISSION_REWARD_MAX_SIZE; ++i)
		{
			WORD wMPoint = (WORD)pCurr[i].m_bMissionNum;
			if( wMPoint && (rMRA.GetMissionPoint() >= wMPoint) )
			{
				bool bOK = (MissionRewardInfo::MISSION_REWARD_MAX_SIZE != nextIDX) ?
					(rMRA.GetMissionPoint() < pCurr[nextIDX].m_bMissionNum) : true;

				if( bOK )
				{
					WORD wExpRatio = rMRA.GetPlayer()->GetFatigueManager()->GetExpRewardRatio();
			#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
					// 현재 이벤트 중이면
					SunEventInfoParser* const pSunEventInfoParser = SunEventInfoParser::Instance();
					EVENT_INFO* const pEventInfo = pSunEventInfoParser->FindEventInfoFromMode( eEVENT_GUILD_MISSION_EVENT );
					//현재 이벤트 중이고, 길드 클리어 했으면.
					if( pEventInfo && pEventInfo->CheckEvent() && rMRA.GetPlayer()->GetMissionManager()->IsGuildEvent() )
					{
                        BOOST_STATIC_WARNING(false); // TODO: is valid calculation?
						wExpRatio += pEventInfo->m_fRewardExpRate*100;
					}
			#endif
                    //_KR_0_20091021_FIRENDINVITATION_EXTEND //미션 보상에 경험치 증가시키기
                    GameField* const field = rMRA.GetPlayer()->GetField();
                    if(field)
                    {
                        wExpRatio += rMRA.GetPlayer()->GetInvitateFriend().GetAddMissionExpRate(field->GetFieldCode());
                    }                    
					return g_RewardManager.RewardExp( pREWARD->m_wRewardCode, rMRA.GetPlayer(), *pExp, wExpRatio );
				}

				pREWARD = &pCurr[nextIDX++];
				continue;
			}
			else
			{	// wMPoint가 0인 경우에는 무한대로 설정하자.
				if( !wMPoint )
				{
                    if(i > 0) {
					    WORD wExpRatio = rMRA.GetPlayer()->GetFatigueManager()->GetExpRewardRatio();
					    return g_RewardManager.RewardExp( pCurr[i-1].m_wRewardCode, rMRA.GetPlayer(), *pExp, wExpRatio );
                    }
                    FlowControl::FCAssert(!"i <= 0");
				}
			}
		}
	}

	return RC::RC_REWARD_FAILED;
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardHime( const MissionRewardInfo::sCLASS* IN pMRIclass, MONEY* OUT pMoney )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo::sCLASS::sREWARD *pREWARD, *pCurr;

	if( rMRA.GetMissionRewardInfoClassCode() == NULL )
	{
		if( GetMissionRewardInfoNClassCode() == NULL )
			return RC::RC_REWARD_FAILED;
	}

	pREWARD = pCurr = rMRA.GetMissionRewardInfoClassCode()->REWARDs;

	if( ((DWORD)(__int64)pREWARD)<4000 )
	{
		ASSERT( 0 && "뚫렸다!" );
		return RC::RC_REWARD_FAILED;
	}

	if( rMRA.GetMissionPoint() > 0 )
	{
		DWORD nextIDX = 1;
		for(int i = 0; i < MissionRewardInfo::MISSION_REWARD_MAX_SIZE; ++i)
		{
			WORD wMPoint = (WORD)pCurr[i].m_bMissionNum;
			if( wMPoint && (rMRA.GetMissionPoint() >= wMPoint) )
			{
				bool bOK = (MissionRewardInfo::MISSION_REWARD_MAX_SIZE != nextIDX) ?
					(rMRA.GetMissionPoint() < pCurr[nextIDX].m_bMissionNum) : true;

				if( bOK )
				{
					BYTE byHeimRatio = rMRA.GetPlayer()->GetFatigueManager()->GetHeimRewardRatio();
					return g_RewardManager.RewardHime( pREWARD->m_wRewardCode, rMRA.GetPlayer(), *pMoney, byHeimRatio );
				}

				pREWARD = &pCurr[nextIDX++];
				continue;
			}
			else
			{	// wMPoint가 0인 경우에는 무한대로 설정하자.
				if( !wMPoint )
				{
					if(i > 0) {
					    BYTE byHeimRatio = rMRA.GetPlayer()->GetFatigueManager()->GetHeimRewardRatio();
					    return g_RewardManager.RewardHime( pCurr[i-1].m_wRewardCode, rMRA.GetPlayer(), *pMoney, byHeimRatio );
                    }
                    FlowControl::FCAssert(!"i <= 0");
				}
			}
		}
	}

	return RC::RC_REWARD_FAILED;
}

// m_MissionRewardInfo 정보에 의거 계산된다.
RC::eREWARD_RESULT	MissionRewardManager::RewardItem( const MissionRewardInfo::sCLASS* IN pMRIclass, INVENTORY_TOTAL_INFO& OUT rInvenInfo )
{
	MissionRewardArgs& rMRA = GetMissionRewardArgs();
	MissionRewardInfo::sCLASS::sREWARD *pREWARD, *pCurr;

	if( rMRA.GetMissionRewardInfoClassCode() == NULL )
	{
		if( GetMissionRewardInfoNClassCode() == NULL )
			return RC::RC_REWARD_FAILED;
	}

	if( rMRA.GetPlayer()->GetFatigueManager()->GetItemReward() == FALSE )
	{
		rInvenInfo.m_InvenCount = 0;
		rInvenInfo.m_TmpInvenCount = 0;
		return RC::RC_REWARD_SUCCESS;							// 보상받지 못하는 피로시간이면, 성공으로 리턴해서 하임과 경험치를 얻게 한다.
	}
	pREWARD = pCurr = rMRA.GetMissionRewardInfoClassCode()->REWARDs;

	if( ((DWORD)(__int64)pREWARD)<4000 )
	{
		ASSERT( 0 && "뚫렸다!" );
		return RC::RC_REWARD_FAILED;
	}

	if( rMRA.GetMissionPoint() > 0 )
	{
		DWORD nextIDX = 1;
		for(int i = 0; i < MissionRewardInfo::MISSION_REWARD_MAX_SIZE; ++i)
		{
			WORD wMPoint = (WORD)pCurr[i].m_bMissionNum;
			if( wMPoint && (rMRA.GetMissionPoint() >= wMPoint) )
			{
				bool bOK = (MissionRewardInfo::MISSION_REWARD_MAX_SIZE != nextIDX) ?
					(rMRA.GetMissionPoint() < pCurr[nextIDX].m_bMissionNum) : true;

				if( bOK )
					return g_RewardManager.RewardItem( RewardManager::eREWARD_MISSION, pREWARD->m_wRewardCode, rMRA.GetSelectedItem(), rMRA.GetPlayer(), rInvenInfo );

				pREWARD = &pCurr[nextIDX++];
				continue;
			}
			else
			{	// wMPoint가 0인 경우에는 무한대로 설정하자.
				if( !wMPoint )
				{
                    if(i > 0)
                        return g_RewardManager.RewardItem( RewardManager::eREWARD_MISSION, pCurr[i-1].m_wRewardCode, rMRA.GetSelectedItem(), rMRA.GetPlayer(), rInvenInfo );
                    FlowControl::FCAssert(!"i <= 0");
				}
			}
		}
	}

	return RC::RC_REWARD_FAILED;
}
