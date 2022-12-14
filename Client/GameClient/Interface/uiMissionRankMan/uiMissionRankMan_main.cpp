#include "SunClientPrecompiledHeader.h"
#if !defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiMissionRankMan.h"
#include "uiMissionRankTimer/uiMissionRankTimer.h"
#include "uiMissionRankEnding/uiMissionRankEnding.h"
#include "uiMissionRankList/uiMissionRankList.h"
#include "uiMissionRankGamble/uiMissionRankGamble.h"
#include "uiRankMenu/uiRankMenu.h"

#include "uiPartyMan/uiPartyMan.h"
#include "ItemManager.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "Map.h"
#include "uiPartyMan/uiPartyMan.h"

#include "ArchiveOfStream.h"

#include "ResultCode.h"
#include "GameFunc.h"

#include "MissionManager_Concrete.h"
#include "QuestManager_Concrete.h"

#include "Application.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"

#ifdef _INTERNATIONAL_DATE_TOOLTIP
#include"uiToolTipMan/uiToolTipMan.h"
#endif//_INTERNATIONAL_DATE_TOOLTIP

const int c_iCheckPoint = 1000;

using namespace RC;

//==================================================================================================
typedef MissionRanking::RANKING_INFO_LIST RANKING_INFO_LIST;
typedef MissionRankingRecord::MEMBER_HASH MEMBER_HASH;
//==================================================================================================
/**
*/
uiMissionRankMan::uiMissionRankMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIRankTimer	= NULL;
	m_pUIRankEnding = NULL;
	m_pUIRankList	= NULL;
	m_pUIRankGamble = NULL;

	InitMission();
	ClearRankData();

 	SetRankingSystemType(eRANKING_TYPE_NONE);

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::OnRelease()
{
	m_mapEventMap.clear();
	m_vecEventMapCategory.clear();
	m_MapMissionGroup.clear();
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::InitMission()
{
	m_dwTotalMonsterKillNum = 0;
	m_dwMaxMonsterKillNum = 0; 

	m_dwResultMissionClear			= 0;	//????!!????????????
	m_dwTotalMonsterDecTimeSecond	= 0;	//?? ?????? ???? ????
	m_dwGeneralMonsterDecSecond		= 0;	//???? ?????? ???? ????
	m_dwLeaderMonsterDecSecond		= 0;	//???? ???? ????
	
	m_dwMissionTime	= 0;
	m_dwStartMissionTime = 0; 
	m_dwUpdateMissionTime = 0;
	m_dwMissionClearTime = 0;



	m_bMissionCleared = false;
	m_dwMissionBestTime = 0;
	m_bEnableMissionRanking = false;

	m_iMyRank = 0;
	m_iMyPartyNum = 0;
	m_strMyPartyMasterName.clear();

	m_bWaitRcvPacket = false;
	m_bRecvStartTimer = FALSE;

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::ClearRankData()
{
	m_dwMyRankForReward	= 0;
	m_RewardRankKey		= 0;			
	m_RewardMissionNo	= 0;

	m_CurMissionNo		= 0;
	m_CurRankPage		= 0;
	

	GET_MISSION_RANK_MANAGER()->Release();
	GET_MISSION_RANK_MANAGER()->Initialize();
	GET_MISSION_RANK_MANAGER()->SetStatus( eRANKING_STATUS_NORMAL );


	ZeroMemory(&m_MissionPartyInfo, sizeof(m_MissionPartyInfo));

	ZeroMemory(&m_FameData, sizeof(m_FameData));
	ZeroMemory(&m_PreWeekRank, sizeof(m_PreWeekRank));
	ZeroMemory(&m_CurWeekRank, sizeof(m_CurWeekRank));
	ZeroMemory(&m_MemberData, sizeof(m_MemberData));


	m_PendingRankingType	= eRANKING_MAX;
	m_PendingMissionNo		= MAX_MISSION_NUM;
	m_PendingRankingKey		= -1;
	m_PendingPage			= 0;

	// ???? ?????? ????
	m_RqstRankingType = eRANKING_MAX;
	m_RqstMissionNo = MAX_MISSION_NUM;

	vecRqstMission.clear();

	m_SettledDate = 0;
	m_strFinishDay.clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::_addTestData()
{
	m_FameData.iRank		= 1;
	m_FameData.iPartyNum	= 1111;
	m_FameData.iLevel		= 11;
	m_FameData.dwTime = 0;

	m_PreWeekRank[0].iRank = 1;
	m_PreWeekRank[0].iPartyNum = 1212;
	m_PreWeekRank[0].iLevel = 12;
	m_PreWeekRank[0].dwTime = 0;

	m_PreWeekRank[1].iRank = 2;
	m_PreWeekRank[1].iPartyNum = 12;
	m_PreWeekRank[1].iLevel = 1;
	m_PreWeekRank[1].dwTime = 0;

	m_PreWeekRank[2].iRank = 3;
	m_PreWeekRank[2].iPartyNum = 121;
	m_PreWeekRank[2].iLevel = 12;
	m_PreWeekRank[2].dwTime = 0;
	
	m_CurWeekRank[0].iRank = 1;
	m_CurWeekRank[0].iPartyNum = 133;
	m_CurWeekRank[0].iLevel = 14;
	m_CurWeekRank[0].dwTime = 0;

	m_CurWeekRank[1].iRank = 2;
	m_CurWeekRank[1].iPartyNum = 13;
	m_CurWeekRank[1].iLevel = 55;
	m_CurWeekRank[1].dwTime = 0;

	m_CurWeekRank[2].iRank = 3;
	m_CurWeekRank[2].iPartyNum = 1332;
	m_CurWeekRank[2].iLevel = 5;
	m_CurWeekRank[2].dwTime = 0;

	TCHAR szString[256];

	for(int i=0; i<6; ++i)
	{
		m_MemberData[i].iLevel = i+10;
		ZeroMemory(szString, sizeof(szString));
		Snprintf(szString,255,_T("guildguildguild%d"), i);
		//m_MemberData[i].strGuild = szString;
        StrnCopy(m_MemberData[i].strGuild, szString, MAX_GUILDNAME_LENGTH);

		ZeroMemory(szString, sizeof(szString));
		Snprintf(szString,255,_T("namenamename000%d"), i);
		//m_MemberData[i].strName	= szString;
        StrnCopy(m_MemberData[i].strName, szString, MAX_CHARNAME_LENGTH);

	}
	
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetReceivedStartMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime)
{
	m_dwStartMissionTime = dwRecievedTime;
	m_dwMissionTime = dwMissionTime;
	m_bRecvStartTimer = TRUE;

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetReceivedUpdateMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime)
{
	m_dwUpdateMissionTime = dwRecievedTime;
	m_dwMissionTime = dwMissionTime;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetClearTime(DWORD dwClearTime)
{
	m_dwMissionClearTime = dwClearTime;
	m_bMissionCleared = true;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::DisplayTime()
{
	if(g_pMap && (IsMissionRanking((RANKINGNO)g_pMap->GetMapID(), eRANKING_TYPE_TIME_ATTACK) == false))	// ????????(????????????)?? ???????? ?????? ???????? ???????? ????
		return;


	if(m_pUIRankTimer)
	{
		//?????????? BRD?? ???????????? ???? ????????????.
		if(!m_bRecvStartTimer)
		{
			m_pUIRankTimer->ShowInterface(FALSE);
			return;
		}


		if(m_pUIRankTimer->IsVisibleWindow())
		{
			m_pUIRankTimer->UpdateTime();
		}
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		else
		{

			//???? ???????? ???????? ???????????? ??????.
			if( GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG )->IsVisible() )
			{
				m_pUIRankTimer->ShowInterface(FALSE);
			}
			else
			{
				m_pUIRankTimer->ShowInterface(TRUE);
			}

		}
#endif // !defined (_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::DisplayEnding(int iRank, int iPartyNum, TCHAR* pMasterName)
{
	m_iMyRank = iRank;
	m_iMyPartyNum = iPartyNum;
	m_strMyPartyMasterName = pMasterName;

	// ?????????? ???? ????????
	m_dwGeneralMonsterDecSecond = m_dwMonsterKillNum_General * GENERAL_MON;	//???? ?????? ???? ????
	m_dwLeaderMonsterDecSecond = m_dwMonsterKillNum_Leader * LEADER_MON;		//???? ???? ????
	m_dwTotalMonsterDecTimeSecond = m_dwGeneralMonsterDecSecond + m_dwLeaderMonsterDecSecond;	//?? ?????? ???? ????
	
	m_dwResultMissionClear = m_dwMissionClearTime + (m_dwTotalMonsterDecTimeSecond * 1000);		//?????? kill ?? ???? ?? ????

	if(m_pUIRankEnding)
	{
		m_pUIRankEnding->ShowInterface(TRUE);
	}
}



//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::CloseEndingDlg()
{
	if(m_pUIRankEnding)
	{
		m_pUIRankEnding->ShowInterface(FALSE);
	}
}


//------------------------------------------------------------------------------
/**
*/
TCHAR* uiMissionRankMan::GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    if (MissionRanking* const mission_ranking =
            GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                                        end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {
                    return const_cast<TCHAR*>(member->GetCharName());
                }
            }
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
int uiMissionRankMan::GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    if (MissionRanking* const mission_ranking =
            GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                                        end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {
                    return member->GetLV();
                }
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::ShowRankMenu( BOOL bShow )
{
	if( m_pUIRankMenu == NULL )
		return;

	m_pUIRankMenu->ShowInterface( bShow );
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::ShowRankList(int iType)
{
	SetRankingSystemType(iType);

	if( iType == eRANKING_TYPE_TIME_ATTACK )
	{
		m_pUIRankList->ShowInterface( TRUE );
	}
	else
	{
		m_pUIPointList->ShowInterface( TRUE );
	}
	
	if(m_pUIRankGamble)
	{
		m_pUIRankGamble->ShowInterface(FALSE);
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::ShowRankGamble()
{
	if(m_pUIRankGamble)
	{
		m_pUIRankGamble->ShowInterface(TRUE);
	}

	m_pUIRankList->ShowInterface( FALSE );
	m_pUIPointList->ShowInterface( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::CloseMissionRunningDlg()
{
	if(m_pUIRankTimer)
	{
		m_pUIRankTimer->ShowInterface(FALSE);
	}

	if(m_pUIRankEnding)
	{
		m_pUIRankEnding->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::CloseMissionRankingDlg()
{
	m_pUIRankList->ShowInterface( FALSE );
	m_pUIPointList->ShowInterface( FALSE );

	if(m_pUIRankGamble)
	{
		m_pUIRankGamble->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::GetTimeString(DWORD dwTime, util::_tstring &strTime)
{
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};

	DWORD minTick   = 0;
	DWORD secTick   = 0;
	DWORD msecTick  = 0;

	if(dwTime)
	{
		DWORD dwTimeTemp1 = dwTime / SEC_PER_TICK;
		DWORD dwTimeTemp2 = dwTime % SEC_PER_TICK;

		if(dwTimeTemp1)
		{
			minTick   = dwTimeTemp1 / 60;
			secTick   = dwTimeTemp1 % 60;
		}

		if(dwTimeTemp2)
			msecTick  = dwTimeTemp2 / 10;
	}

	strTime.clear();
	Snprintf (szTime, INTERFACE_STRING_LENGTH-1, _T("%03d'%02d''%02d"), minTick, secTick, msecTick);
	strTime = szTime;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::InitMissionGroup()
{
	m_mapEventMap.clear();
	m_vecEventMapCategory.clear();
	m_vecEventMapCode.clear();

	m_vecMissionCode.clear();
	m_MapMissionGroup.clear();


    const RankingManager::MISSION_LEVEL_AREA_HASH& MissionLVAreaHash =
        GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();

	RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator it;
	RankingManager::LEVEL_AREA* pArea = NULL;
	MapGroup * pGroup	= NULL;
	
	MISSION_GROUP_ITR itr;

	int iEventMapGroup = 0;
	std::vector<MAPCODE>  vecEventMapCategory;
	//GS_BUG_FIX:????????????(?????? ???????????????? ???????????????????? ????????)
	std::vector<MAPCODE>  vecMissionMapCategory;

	for(it = MissionLVAreaHash.begin(); it != MissionLVAreaHash.end(); ++it)
	{
		pArea = (it->second);
		
		if(!pArea)
			continue;

		if(pArea->m_RankingType == eRANKING_TYPE_TIME_ATTACK)	//????????
		{
			//GS_BUG_FIX: first ???? ?????? ?????? ????????
			vecMissionMapCategory.push_back(pArea->m_MissionNo);
		}
		else if(pArea->m_RankingType == eRANKING_TYPE_POINT)	//??????????
		{
			vecEventMapCategory.push_back(pArea->m_MissionNo);
		}
	}
	
	//GS_BUG_FIX: second ???????? ?????? ????????
	if(!vecMissionMapCategory.empty())
		sort(vecMissionMapCategory.begin(),vecMissionMapCategory.end());
	
	for(unsigned int i=0;i<vecMissionMapCategory.size();++i)
	{
		m_vecMissionCode.push_back(vecMissionMapCategory[i]);
		
		const sMAPINFO *pMapInfo = MapInfoParser::Instance()->FindMapInfo(vecMissionMapCategory[i]);

		if(pMapInfo)
		{
			itr = m_MapMissionGroup.find(pMapInfo->MapKind);
			if ( itr == m_MapMissionGroup.end() )
			{
				pGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );
				if(pGroup)
				{
					m_MapMissionGroup.insert( MISSION_GROUP_PAIR(pMapInfo->MapKind, pGroup) );
				}
			}
		}
	
	}


	//GS_BUG_FIX:???????? ?????????? ?????????? push_back?? ???? ???????? ??????????.
	if(!vecEventMapCategory.empty())
		sort(vecEventMapCategory.begin(),vecEventMapCategory.end());
	
	for(unsigned int i=0;i<vecEventMapCategory.size();++i)
	{
		const sMAPINFO *pMapInfo = MapInfoParser::Instance()->FindMapInfo(vecEventMapCategory[i]);
		stEventMapInfo stEventMap;

		if(pMapInfo && (pMapInfo->dwANCode))
		{
			if(m_vecEventMapCategory.size() == 0)
			{
				stEventMap.iGroup		= iEventMapGroup;
				stEventMap.dwCategory	= pMapInfo->dwANCode;
				m_vecEventMapCategory.push_back(stEventMap);
			}
			else
			{
				int iInsertIndex = -1;

				for(unsigned int k=0; k < m_vecEventMapCategory.size(); ++k)
				{
					if(m_vecEventMapCategory[k].dwCategory == pMapInfo->dwANCode)
					{
						iInsertIndex = m_vecEventMapCategory[k].iGroup;
						break;
					}
				}

				if(iInsertIndex < 0) // ?? ???? ????
				{
					++iEventMapGroup;
					stEventMap.iGroup = iEventMapGroup;
					stEventMap.dwCategory	= pMapInfo->dwANCode;
					m_vecEventMapCategory.push_back(stEventMap);
				}
			}
			
			m_mapEventMap.insert( EVENTMAP_PAIR(pMapInfo->dwANCode, const_cast< sMAPINFO * >(pMapInfo) ));
			m_vecEventMapCode.push_back(pMapInfo->MapCode);
		}
	}

}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetFinishDay(DWORD dwSettledDate)
{
	m_strFinishDay.clear();

	m_SettledDate = dwSettledDate;
	
	if(m_SettledDate > 0)
	{
		
		int iMonth = m_SettledDate % 10000;
		int iYear = m_SettledDate / 10000;

		if(iMonth)
		{
			int iDay = iMonth % 100;
			iMonth = iMonth / 100;
			
			TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _INTERNATIONAL_DATE_TOOLTIP
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			int nIndex = pUIMan->GetStringMonth(iMonth);
			TCHAR szMonth[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(nIndex, szMonth);//??????
			Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T(" %s %02d, %02d"), 
				szMonth, 
				iDay, 
				iYear);
		}
#else
			Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T("%02d-%02d-%02d"), 
				iYear, 
				iMonth, 
				iDay);
#endif//_INTERNATIONAL_UI
			
			m_strFinishDay = szResult;

		}
	}

	if( m_iRankingSystemType == 1 )
	{
		m_pUIRankList->UpdateFinishDay();
	}
	else
	{
		m_pUIPointList->UpdateFinishDay();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetReceiveData(BYTE type, RANKINGNO no, BYTE page)	// ?????????? ?????? ???????? ????????
{
	bool bPointRanking = false;

	if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_POINT) //??????????
	{
		bPointRanking = true;
	}
	else if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_TIME_ATTACK) //????????
	{
	}
	else
	{
		return;
	}

	m_CurMissionNo = no;
	m_CurRankPage = page;

	ZeroMemory(&m_MemberData, sizeof(m_MemberData));

	MissionRanking *pMR = NULL;
	int iRank=1;
	int iIndex = 0;

	if((type == eRANKING_FAME) && (page == 0))	// ?????? ?????? ???????? 0?????? ???? ????
	{
		ZeroMemory(&m_FameData, sizeof(m_FameData));
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_FAME, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iRank=1;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
			{
				if(iRank > 1)	// ?????? ?????? 1???? ????
					break;

				MissionRankingRecord* const record = (*it);
				m_FameData.iPartyNum = record->GetKey();				// ????????
				m_FameData.dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_FameData.iRank = pMR->GetRanking(m_FameData.iPartyNum); // ????

				if(bPointRanking)
				{
					m_FameData.iLevel = GetMemberLV(no, m_FameData.iPartyNum, (eRANKING_TYPE)type);	// ????
					//m_FameData.strName = GetMemberName(no, m_FameData.iPartyNum, (eRANKING_TYPE)type);	//????
                    StrnCopy(m_FameData.strName, GetMemberName(no, m_FameData.iPartyNum, (eRANKING_TYPE)type), MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_FameData.iLevel = record->GetAvrgLVClient();				// ????????
				}
				
				++iRank;

			}
		}
	}
	else if((type == eRANKING_LASTWEEK) && (page == 0))	// ?????? ?????? ???????? 0?????? ???? ????
	{
		ZeroMemory(&m_PreWeekRank, sizeof(m_PreWeekRank));
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
            {
				if(iIndex >= MAX_PREWEEK_RANK)	//  ???????? 3????????
					break;
				MissionRankingRecord* const record = (*it);
				m_PreWeekRank[iIndex].iPartyNum = record->GetKey();				// ????????
				m_PreWeekRank[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_PreWeekRank[iIndex].iRank = pMR->GetRanking(m_PreWeekRank[iIndex].iPartyNum);	// ????

				if(bPointRanking)
				{
					m_PreWeekRank[iIndex].iLevel = GetMemberLV(no, m_PreWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type);		// ????
					//m_PreWeekRank[iIndex].strName = GetMemberName(no, m_PreWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type);	// ????
                    StrnCopy(m_PreWeekRank[iIndex].strName, 
                                GetMemberName(no, m_PreWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type),
                                MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_PreWeekRank[iIndex].iLevel = record->GetAvrgLVClient();				// ????????
				}

				++iIndex;
			}
		}
	}
	else if(type == eRANKING_TODAYWEEK)
	{
		ZeroMemory(&m_CurWeekRank, sizeof(m_CurWeekRank));

		int iStartWeekRank = (page*MAX_WEEK_RANK) + 1;
		
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
			{
                MissionRankingRecord* const record = (*it);
				int iKey = record->GetKey();				// ????????
				int iWeekRank = pMR->GetRanking(iKey);	// ????

				if(iWeekRank < iStartWeekRank)
					continue;

				if(iIndex >= MAX_WEEK_RANK)	// ?????????? 10???? ?? 50??
					break;

				m_CurWeekRank[iIndex].iPartyNum = iKey;							// ????????
				m_CurWeekRank[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_CurWeekRank[iIndex].iRank = iWeekRank;	// ????

				if(bPointRanking)
				{
					m_CurWeekRank[iIndex].iLevel = GetMemberLV(no, m_CurWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type);		//????
					//m_CurWeekRank[iIndex].strName = GetMemberName(no, m_CurWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type);	//????
                    StrnCopy(m_CurWeekRank[iIndex].strName, 
                        GetMemberName(no, m_CurWeekRank[iIndex].iPartyNum, (eRANKING_TYPE)type),
                        MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_CurWeekRank[iIndex].iLevel = record->GetAvrgLVClient();				// ????????
				}


				++iIndex;
			}
		}
	}

	if( m_iRankingSystemType == 1 )
	{
		m_pUIRankList->UpdateRankList();
	}
	else
	{
		m_pUIPointList->UpdateRankList();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::UpdateReceiveData(RANKINGNO no, BYTE page)
{	
	bool bPointRanking = false;

	if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_POINT) //??????????
	{
		bPointRanking = true;
	}
	else if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_TIME_ATTACK) //????????
	{
	}
	else
	{
		return;
	}

	m_CurMissionNo = no;
	m_CurRankPage = page;

	// ???? ???? ???? ???????? ?????? ?????? ???????? ????
	if(IsRankData(no, page))
	{
		ZeroMemory(&m_FameData, sizeof(m_FameData));
		ZeroMemory(&m_PreWeekRank, sizeof(m_PreWeekRank));
		ZeroMemory(&m_CurWeekRank, sizeof(m_CurWeekRank));
		ZeroMemory(&m_MemberData, sizeof(m_MemberData));

		MissionRanking *pMR = NULL;
		int iRank=1;
		int iIndex = 0;
		
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_FAME, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iRank=1;	
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
			{
				if(iRank > MAX_FAME_RANK)	// ?????? ?????? 1???? ????
					break;
                MissionRankingRecord* const record = (*it);
				m_FameData.iPartyNum = record->GetKey();				// ????????
				m_FameData.dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_FameData.iRank = pMR->GetRanking(m_FameData.iPartyNum); // ????

				if(bPointRanking)
				{
					m_FameData.iLevel = GetMemberLV(no, m_FameData.iPartyNum, eRANKING_FAME);	//????
					//m_FameData.strName = GetMemberName(no, m_FameData.iPartyNum, eRANKING_FAME);//????
                    StrnCopy(m_FameData.strName, 
                        GetMemberName(no, m_FameData.iPartyNum, eRANKING_FAME),
                        MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_FameData.iLevel = record->GetAvrgLVClient();				// ????????
				}


				++iRank;

			}
		}
		
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
			{
				if(iIndex >= MAX_PREWEEK_RANK)	//  ???????? 3????????
					break;
                MissionRankingRecord* const record = (*it);
				m_PreWeekRank[iIndex].iPartyNum = record->GetKey();				// ????????
				m_PreWeekRank[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_PreWeekRank[iIndex].iRank = pMR->GetRanking(m_PreWeekRank[iIndex].iPartyNum);	// ????

				if(bPointRanking)
				{
					m_PreWeekRank[iIndex].iLevel = GetMemberLV(no, m_PreWeekRank[iIndex].iPartyNum, eRANKING_LASTWEEK);	//????
					//m_PreWeekRank[iIndex].strName = GetMemberName(no, m_PreWeekRank[iIndex].iPartyNum, eRANKING_LASTWEEK);	//????
                    StrnCopy(m_PreWeekRank[iIndex].strName, 
                        GetMemberName(no, m_PreWeekRank[iIndex].iPartyNum, eRANKING_LASTWEEK),
                        MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_PreWeekRank[iIndex].iLevel = record->GetAvrgLVClient();				// ????????
				}


				++iIndex;
			}
		}
		
		int iStartWeekRank = (page*MAX_WEEK_RANK) + 1;
		// ?????? ????

		pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, m_CurMissionNo);

		if(pMR)
		{
			const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
			
			iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                              end = mission_list.end();
            for (; it != end; ++it)
			{
                MissionRankingRecord* const record = (*it);
				int iKey = record->GetKey();				// ????????
				int iWeekRank = pMR->GetRanking(iKey);	// ????

				if(iWeekRank < iStartWeekRank)
					continue;

				if(iIndex >= MAX_WEEK_RANK)	// ?????????? 10???? ?? 50??
					break;

				m_CurWeekRank[iIndex].iPartyNum = iKey;							// ????????
				m_CurWeekRank[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ????????
				m_CurWeekRank[iIndex].iRank = iWeekRank;	// ????

				if(bPointRanking)
				{
					m_CurWeekRank[iIndex].iLevel = GetMemberLV(no, m_CurWeekRank[iIndex].iPartyNum, eRANKING_TODAYWEEK);	//????
					//m_CurWeekRank[iIndex].strName = GetMemberName(no, m_CurWeekRank[iIndex].iPartyNum, eRANKING_TODAYWEEK);	//????
                    StrnCopy(m_CurWeekRank[iIndex].strName, 
                        GetMemberName(no, m_CurWeekRank[iIndex].iPartyNum, eRANKING_TODAYWEEK),
                        MAX_CHARNAME_LENGTH);
				}
				else
				{
					m_CurWeekRank[iIndex].iLevel = record->GetAvrgLVClient();				// ????????
				}

				++iIndex;
			}
		}

		
		if( m_iRankingSystemType == 1 )
		{
			m_pUIRankList->UpdateRankList();
		}
		else
		{
			m_pUIPointList->UpdateRankList();
		}
	}
	else
	{
		// ???????? ?????? ?????? sync ????
		m_PendingRankingType	= eRANKING_FAME;
		m_PendingMissionNo		= m_CurMissionNo;
		m_PendingPage			= m_CurRankPage;	

		this->Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::IsRankData(RANKINGNO no, BYTE page)
{
	for(unsigned int i=0; i<vecRqstMission.size(); ++i)
	{
		if((vecRqstMission[i].no == no) && (vecRqstMission[i].page == page))
			return true;
	}

	return false;
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(RANKINGNO no, int iIndex, eRANKING_TYPE type)
{

	if(!m_bWaitRcvPacket)
	{
		ZeroMemory(&m_MemberData, sizeof(m_MemberData));

		RANKINGKEY key;

		if(type == eRANKING_FAME)
		{
			// ?????? ????
			key = m_FameData.iPartyNum;
			
		}
		else if(type == eRANKING_LASTWEEK)
		{
			// ?????? ????
			key = m_PreWeekRank[iIndex].iPartyNum;

		}
		else if(type == eRANKING_TODAYWEEK)
		{
			// ?????? ????
			key = m_CurWeekRank[iIndex].iPartyNum;
		}

		// ???? ???????? ?????? ????

		MissionRanking *pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no);

		if(pMR)
		{
			MissionRankingRecord * pRecord = pMR->FindRanking(key);
			if(pRecord)
			{
                const MEMBER_HASH& members = pRecord->GetMemberHash();
				if(members.empty())		// ???????? ???? ???? ?????? ???????? ????
				{
					m_RqstRankingType		= type;
					m_RqstMissionNo			= no;
					m_PendingRankingKey		= key;

					MSG_CG_MISSION_RANKING_MEMBER_INFO_SYN sync;                    
					sync.m_Type			= m_RqstRankingType;
					sync.m_MissionNo	= m_RqstMissionNo;
					sync.m_Key			= m_PendingRankingKey;
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
					m_bWaitRcvPacket = true;
				}
				else	// ?????? ???? ???????? ????
				{
					SetMemberData(no, key, type);

					if( m_iRankingSystemType == 1 )
					{
						m_pUIRankList->UpdateMemberList();
					}
					else
					{
						m_pUIPointList->UpdateMemberList();
					}
				}
			}
		}

		
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::SetMemberData(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    ZeroMemory(&m_MemberData, sizeof(m_MemberData));

    if (MissionRanking* const mission_ranking =
            GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                                        end = members.end();
            for (int i = 0; i < _countof(m_MemberData) && it != end; ++i, ++it)
            {
                RankingMember* member = it->second;
                if (member == NULL)
                {
                    assert(!"unexpected status");
                    continue;
                };
                if (stMissionRankMemberData* dest = &m_MemberData[i])
                {
                    dest->iLevel = member->GetLV();
                    //dest->strGuild = member->GetGuildName();
                    //dest->strName = member->GetCharName();
                    StrnCopy(dest->strGuild, member->GetGuildName(), MAX_GUILDNAME_LENGTH);
                    StrnCopy(dest->strName, member->GetCharName(), MAX_CHARNAME_LENGTH);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::IsEnableReward()
{
	m_RewardRankKey		= 0;			
	m_RewardMissionNo	= 0;
	m_dwMyRankForReward = 0;

	// ???? ???????? ?? ?????? ????
	if(!g_pHero)
		return false;

	MissionRanking *pMR = NULL;
	RANKINGNO no = 0;
	RANKINGKEY key = 0;
	eRANKING_RESULT result = RC_RANKING_INVALID_STATE;

	if(this->GetRankingSystemType() == eRANKING_TYPE_TIME_ATTACK) // ????????
	{
		for(unsigned int i=0; i < m_vecMissionCode.size(); ++i)
		{
			no = m_vecMissionCode[i];
			// ?????? ?????? ?????? ??????

			pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, no);


			if(pMR)
			{
                const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
                RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                                  end = mission_list.end();
                for (; it != end; ++it)
				{
                    MissionRankingRecord* const record = (*it);
					key = record->GetKey();
					RankingMember* pRankingMember = NULL;
					result = GET_MISSION_RANK_MANAGER()->CanReward( no, key, g_pHero->GetName(), &pRankingMember);

					if(RC_RANKING_SUCCESS == result)
					{
						m_RewardRankKey		= key;			
						m_RewardMissionNo	= no;
						m_dwMyRankForReward = pMR->GetRanking(key);
						return true;
					}
				}
			}

		}
	}
	else if(this->GetRankingSystemType() == eRANKING_TYPE_POINT) //??????????
	{
		for(unsigned int i=0; i < m_vecEventMapCode.size(); ++i)
		{
			no = m_vecEventMapCode[i];
			// ?????? ?????? ?????? ??????
	
			pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, no);


			if(pMR)
			{
                const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();
                RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                                                  end = mission_list.end();
                for (; it != end; ++it)
				{
                    MissionRankingRecord* const record = (*it);
					key = record->GetKey();
					RankingMember* pRankingMember = NULL;
					result = GET_MISSION_RANK_MANAGER()->CanReward( no, key, g_pHero->GetName(), &pRankingMember);

					if(RC_RANKING_SUCCESS == result)
					{
						m_RewardRankKey		= key;			
						m_RewardMissionNo	= no;
						m_dwMyRankForReward = pMR->GetRanking(key);
						return true;
					}
				}
			}

		}
	}
	else
	{
		return false;

	}
	
	// ???????? ?? ???? ?????? ?????? ????
	if(RC_RANKING_INVALID_STATE == result)
	{
		// 5906	???? ?????? ????????.
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(5906, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_SysMsg(szMessage);

	}
	else
	{
		_processErrorCode((DWORD)result);
	}
	
	
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::CheckEnableMissionRankingRecord()	// ???? ?????? ???????? ????
{
	if(!g_pMap)
	{
		return;
	}

	RANKINGNO no = (RANKINGNO)g_pMap->GetMapID();
	if(IsMissionRanking(no, eRANKING_TYPE_TIME_ATTACK) == false)
	{
		m_bEnableMissionRanking = false;
		return;
	}

	ZeroMemory(&m_MissionPartyInfo, sizeof(m_MissionPartyInfo));
	uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
	int iMemberCount = 0;

	if(partyMan)
	{
		STLX_VECTOR<uicPartyMemberInfoPlus>& vPartyMem = partyMan->GetPartyListRef();
		for (int i=0; i<partyMan->GetPartyNum(); ++i)
		{
			if(!vPartyMem[i].IsDead())
			{
				++iMemberCount;
				m_MissionPartyInfo[i].m_Class	= vPartyMem[i].Get().m_byClassType;
				m_MissionPartyInfo[i].m_LV		= vPartyMem[i].Get().m_wLevel;
			}
		}			
	}

	
	RC::eRANKING_RESULT result = GET_MISSION_RANK_MANAGER()->CanRegisterMissionRecord(no, iMemberCount, m_MissionPartyInfo);

	if(result == RC_RANKING_SUCCESS)
		m_bEnableMissionRanking = true;
	else
	{
		m_bEnableMissionRanking = false;

		_processErrorCode(result, eOUTPUT_MSG_TOP_SHOW);
		_processErrorCode(result);
	}

	
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::IsEnableMissionRanking()
{
	return m_bEnableMissionRanking;
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::DecPage()
{
	if(m_CurRankPage <= 0)
		return false;

	--m_CurRankPage;

	UpdateReceiveData(m_CurMissionNo, m_CurRankPage);
	return true;

}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::IncPage()
{
	if(m_CurRankPage >= MAX_PAGE)
		return false;

	++m_CurRankPage;

	UpdateReceiveData(m_CurMissionNo, m_CurRankPage);
	return true;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::Send_CG_MISSION_RANKING_INFO_SYN(BYTE byPendingRankingType, RANKINGNO PendingMissionNo, BYTE byPendingPage)
{

	if(!m_bWaitRcvPacket)
	{
		MSG_CG_MISSION_RANKING_INFO_SYN sync;
		sync.m_Type			= byPendingRankingType;
		sync.m_MissionNo	= PendingMissionNo;
		sync.m_Page			= byPendingPage;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		m_bWaitRcvPacket = true;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::Send_CG_MISSION_RANKING_REWARD_SYN()
{

	if(!m_bWaitRcvPacket)
	{
		MSG_CG_MISSION_RANKING_REWARD_SYN sync;
		sync.m_Key = m_RewardRankKey;
		sync.m_MissionNo = m_RewardMissionNo;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		m_bWaitRcvPacket = true;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::NetworkProc( MSG_BASE * pMsg )
{

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_MISSION:
        {
            switch (pMsg->m_byProtocol)
            {
			case CG_MISSION_RECORD_ELAPSEDTIME_CMD:
			{
				MSG_CG_MISSION_RECORD_ELAPSEDTIME_CMD *pPacket = (MSG_CG_MISSION_RECORD_ELAPSEDTIME_CMD *)pMsg;

				SetReceivedStartMissionTime(clock_function::GetTickCount(), pPacket->m_MissionElapsedTimePerSecond * 1000);
				this->m_dwMissionBestTime = pPacket->m_MissionBestClearTimePerSecond * 1000;

				if(g_pMap)
				{
					//???????????? ??????????
					RANKINGNO no = (RANKINGNO)g_pMap->GetMapID();


					RankingManager::LEVEL_AREA* pArea = GET_MISSION_RANK_MANAGER()->FindLVArea(no);

					if(pArea)
						m_dwMaxMonsterKillNum = pArea->m_MaxMonsterKill;
				}

				CheckEnableMissionRankingRecord();
			}
			break;


			case CG_MISSION_TIMER_CHANGED_BRD:	
			{
				MSG_CG_MISSION_TIMER_CHANGED_BRD *pPacket = (MSG_CG_MISSION_TIMER_CHANGED_BRD *)pMsg;
				
				m_dwTotalMonsterKillNum = pPacket->m_TotalMonsterKillNum;
#ifdef _DEBUG
				TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
				Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("\nCG_MISSION_TIMER_CHANGED_BRD: %ld\n"),pPacket->m_TotalMonsterKillNum);
				OutputDebugString(szDebug);
#endif
			}
			break;

			case CG_MISSION_CLEAR_RECORD_CMD:
			{
				MSG_CG_MISSION_CLEAR_RECORD_CMD *pPacket = (MSG_CG_MISSION_CLEAR_RECORD_CMD *)pMsg;
				m_dwMonsterKillNum_General = pPacket->m_MonsterKillNum_General;
				m_dwMonsterKillNum_Leader = pPacket->m_MonsterKillNum_Leader;

				//////////////////////////////////////////////////////////////////////////				
				// ?????????? ?????? ?????? ?????????? ?????????? ???? ???????? ???? ???????? ?????? ???? ????????.
				if (g_pMission_Concrete && g_pMap)
				{
					// ?????? ?????? SetMissionPoint?? ???? ???? ?????? ???????? ?????? ?????? ?????? ????.

					if ( g_pQMGR_Real )
					{
						g_pQMGR_Real->Event( QUEST_EVENT_MISSION, NULL);
						g_InterfaceManager.UpdateQuestInfo();
					}					
				}
				//////////////////////////////////////////////////////////////////////////				
			}
			break;

			case CG_MISSION_RANKING_SUCCESS_BRD:
			{

				MSG_CG_MISSION_RANKING_SUCCESS_BRD *pPacket = (MSG_CG_MISSION_RANKING_SUCCESS_BRD *)pMsg;
			
				SetClearTime(pPacket->m_ClearTime);

				if(g_pHero)
				{
					uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
					if(partyMan)
					{
						uicPartyMemberInfoPlus *pInfo =	const_cast<uicPartyMemberInfoPlus *>(partyMan->GetPartyLeader());
						if(pInfo)
						{
							TCHAR szMaster[MAX_CHARNAME_LENGTH+1] ={0,};
							StrnCopy(szMaster, pInfo->Get().m_szCharName, MAX_CHARNAME_LENGTH);
							DisplayEnding(pPacket->m_Ranking, pPacket->m_Key, szMaster);
						}
					}

					//  1~50?? ?????? ???????????? ????~
					g_InterfaceManager.GetInterfaceString( 9908, szTemp, INTERFACE_STRING_LENGTH );	// ???????? %d??
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_Ranking);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				
				

			}
			break;

			case CG_MISSION_RANKING_FAILED_BRD:
			{
				MSG_CG_MISSION_RANKING_FAILED_BRD *pPacket = (MSG_CG_MISSION_RANKING_FAILED_BRD *)pMsg;

				SetClearTime(pPacket->m_ClearTime);

				_processErrorCode(pPacket->m_dwErrorCode, eOUTPUT_MSG_CENTER_SHOW);

				_processErrorCode(pPacket->m_dwErrorCode, eOUTPUT_MSG_CHAT);
			}
			break;
		
            case CG_MISSION_RANKING_INFO_ACK:
				{
					m_bWaitRcvPacket = false;
					// ???? ????
					MSG_CG_MISSION_RANKING_INFO_ACK *pPacket = (MSG_CG_MISSION_RANKING_INFO_ACK *)pMsg;
				
					if(m_PendingRankingType == eRANKING_FAME)
					{
						SetFinishDay(pPacket->m_SettledDate);

						stRqstMissionRankInfo missionInfo;
						missionInfo.no		= m_PendingMissionNo;
						missionInfo.page	= m_PendingPage;
						
						vecRqstMission.push_back(missionInfo);
					}

					if((m_PendingRankingType != eRANKING_MAX) && (m_PendingMissionNo != MAX_MISSION_NUM))
					{
						if(GET_MISSION_RANK_MANAGER()->GetRankingType(m_PendingMissionNo) == eRANKING_TYPE_TIME_ATTACK) //????????
						{
							GET_MISSION_RANK_MANAGER()->SerializeForClient((eRANKING_TYPE)m_PendingRankingType, m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_CLIENT_SAVE);
						}
						else if(GET_MISSION_RANK_MANAGER()->GetRankingType(m_PendingMissionNo) == eRANKING_TYPE_POINT) //??????????
						{
							GET_MISSION_RANK_MANAGER()->Serialize((eRANKING_TYPE)m_PendingRankingType, m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_SAVE);
						}
						else
						{
							break;
						}


						SetReceiveData(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);
					}

					++m_PendingRankingType;
					if(m_PendingRankingType < eRANKING_MAX)
					{
						m_PendingMissionNo		= m_CurMissionNo;
						Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);	// ???? ???? ????
					}



				}
				break;

			case CG_MISSION_RANKING_INFO_NAK:
				{
					m_bWaitRcvPacket = false;

					MSG_CG_MISSION_RANKING_INFO_NAK *pPacket = (MSG_CG_MISSION_RANKING_INFO_NAK *)pMsg;
					_processErrorCode(pPacket->m_dwErrorCode);

					if(m_PendingRankingType < eRANKING_TODAYWEEK)
					{
						++m_PendingRankingType;
						m_PendingMissionNo		= m_CurMissionNo;
						Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);	// ???? ???? ????
					}
				}
				break;

			// ???? ???? ????
			case CG_MISSION_RANKING_MEMBER_INFO_ACK:
				{
					m_bWaitRcvPacket = false;

					MSG_CG_MISSION_RANKING_MEMBER_INFO_ACK *pPacket = (MSG_CG_MISSION_RANKING_MEMBER_INFO_ACK *)pMsg;


					MissionRanking *pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(m_RqstRankingType, m_RqstMissionNo);

		  
					if(pMR)
					{
						MissionRankingRecord * pRecord = pMR->FindRanking(m_PendingRankingKey);
						if(pRecord)
						{
							ArchiveOfStream lArchive( pPacket->m_pBUFFER, pPacket->m_Size );
							pRecord->SerializeMember(lArchive, RANKING_SERIALIZE_CLIENT_SAVE);

							SetMemberData(m_RqstMissionNo, m_PendingRankingKey, m_RqstRankingType);

							if( m_iRankingSystemType == 1 )
							{
								m_pUIRankList->UpdateMemberList();
							}
							else
							{
								m_pUIPointList->UpdateMemberList();
							}

						}
					}

				}
				break;

			case CG_MISSION_RANKING_MEMBER_INFO_NAK:
				{
					m_bWaitRcvPacket = false;

					MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK *pPacket = (MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK *)pMsg;
					_processErrorCode(pPacket->m_dwErrorCode);
				}
				break;

			case CG_MISSION_RANKING_REWARD_ACK:
				{
					m_bWaitRcvPacket = false;

					MSG_CG_MISSION_RANKING_REWARD_ACK *pPacket = (MSG_CG_MISSION_RANKING_REWARD_ACK *)pMsg;
					ItemManager::Instance()->InsertInventoryTotalInfo(REWARD, pPacket->m_ItemInfo);
				}
				break;

			case CG_MISSION_RANKING_REWARD_NAK:
				{
					m_bWaitRcvPacket = false;

					MSG_CG_MISSION_RANKING_REWARD_NAK *pPacket = (MSG_CG_MISSION_RANKING_REWARD_NAK *)pMsg;
					_processErrorCode(pPacket->m_dwErrorCode);
				}
				break;


			case CG_MISSION_GET_MISSION_POINT_BRD:
				{
					MSG_CG_MISSION_GET_MISSION_POINT_BRD *pPacket = (MSG_CG_MISSION_GET_MISSION_POINT_BRD *)pMsg;
					g_InterfaceManager.GetInterfaceString(1462 , szTemp ,INTERFACE_STRING_LENGTH);	//1462	???????? ?????? ??????????????(?????????? %d?? ????)
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_dwPoint);
					
					GlobalFunc::OutputMsg_SysMsg(szMessage);

					if(pPacket->m_dwPoint && ((pPacket->m_dwPoint / c_iCheckPoint) >= 1))	// 1000 ?? ?????? ?????????? ????
					{
						//1463	???????? ???????????? ???????????? %d???? ??????????. \n?????? ?????? ???? ?????????? ???????????? ?????? ??????????.
						g_InterfaceManager.GetInterfaceString(1463 , szTemp ,INTERFACE_STRING_LENGTH);
						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_dwPoint);
						GlobalFunc::OutputMsg_SysMsg(szMessage);
					}
				}
				break;


			case CG_MISSION_POINT_CANNOT_REGISTER_BRD:
				{
					//5471 ??????????:?????????? ?????????? ???????? ???? ?????? ??????????.
					g_InterfaceManager.GetInterfaceString(5471 , szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
				}
				break;

            }
        }
        break;
    }


}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan::_processErrorCode(DWORD dwErrorCode, eOUTPUT_MSG_TYPE type/* = eOUTPUT_MSG_CHAT*/)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	int iMessage = 0;

	switch(dwErrorCode)
	{
	case RC_RANKING_FAILED:						// ?? ?? ???? ????
		// 5481	????????: ?? ?? ???? ??????????????.
		iMessage = eST_MISSION_RANK_UNKNOWN_ERROR;
		break;

	case RC_RANKING_INVALID_STATE:				// ?????? ????
		// 5482	????????: ?????? ??????????. 
		iMessage = eST_MISSION_RANK_INVALID_STATUS;
		break;

	case RC_RANKING_ALREADY_REWARDED:			// ???? ???? ???? ????
		// 5483	????????: ???? ?????? ??????????.
		iMessage = eST_MISSION_RANK_ALREADY_REWARDED;
		break;

	case RC_RANKING_DOING_TRANSACTION:			// ???????? ??????, ???? ???? ???? ???? ?? ?????? ???????? ( ???? 2???? ??????! )
		// 5484	????????: ????????????.
		iMessage = eST_MISSION_RANK_DOING_TRANSACTION;
		break;

	case RC_RANKING_NETWORK_ERROR_FOR_PORTAL:	// ???? ?????? ?????? ????. ???????? ?????? ???? ???? ????
		// 5485	????????: ???????? ?????? ???? ????????
		iMessage = eST_MISSION_RANK_NETWORK_PROBLEM;
		break;

	case RC_RANKING_NETWORK_ERROR_FOR_DBPROXY:	// DBP ?????? ?????? ????. ???????? ?????? ???? ???? ????
		// 5485	????????: ???????? ?????? ???? ????????
		iMessage = eST_MISSION_RANK_NETWORK_PROBLEM;
		break;

	case RC_RANKING_NOT_SPACE_FOR_REWARD:		// ?????? ???? ?????? ?????? ????????.
		// 5486	????????: ?????? ???? ???????? ?????? ??????????.
		iMessage = eST_MISSION_RANK_NEDD_INVENTORY_SPACE;
		break;

		// ???? ???? ???? ????
	case RC_RANKING_INSUFFICEINT_PARTY_MEMBER:	// ?????? ???????? ???? ????????(????5??)?? ????????.
		// 5487	????????: ?????? ???????? ???? ???????? ??????????.
		iMessage = eST_MISSION_RANK_REQUIRE_PARTY_MEMBER;
		break;

	case RC_RANKING_A_CLASS_OBTAIN_MAJORITY:	// ?? ???????? ???????? ???? ???? ??????.
		// 5488	????????: ?? ???????? ?????? ?????? ?????????? ????????.
		iMessage = eST_MISSION_RANK_ONE_CLASS_OVER_HALF;
		break;

	case RC_RANKING_DONT_INCLUDE_MISSION_LEVEL_AREA:	// ?????????????? ???????? ???? ?????????? ????????.
		// 5489	????????: ?????????????? ???????? ???? ???????? ??????????.
		iMessage = eST_MISSION_RANK_INVALID_LEVEL_AREA;
		break;

	case RC_RANKING_INVALID_PARAM:					// ?????? ??????????.
		// 5490	????????: ?????? ??????????.
		iMessage = eST_MISSION_RANK_INVALID_PARAMETER;
		break;

	case RC_RANKING_DONT_INCLUDE_RANKING:			// ?????? ???????? ??????????.
		// 5491	????????: ?????? ???????? ??????????.
		iMessage = eST_MISSION_RANK_NOT_INClUDE;
		break;

	case RC_RANKING_INVALID_REWARD_INDEX:			// ???? ???????? ???????? ????????.
		// ????????: ???? ???????? ???????? ???????? 5513
		iMessage =  5513;			// ?????? ????
		break;

	case RC_RANKING_SETTLING_INFO:					// ???? ??????, ???????? ?????? ??????
		// ????????: ???? ??????, ???????? ?????? ????????????. 5514
		iMessage =  5514;			// ?????? ????
		break;

	case RC_RANKING_UNSATISFY_KILL_MONSTER_NUM:
		//5510 ????????:???? ?????? ?? ???? ??????????.
		iMessage = 5510;
		break;

		// ?????????? ???????? ?????? ???? ????????????.
	case RC_RANKING_NOT_FIND_REWARD_USER:			
		iMessage = 5511;
		break;

		// ?????????????? ???? ????????????.
	case RC_RANKING_NOT_FIND_RANKING_INFO:
		iMessage = 5512;
		break;


	case RC_RANKING_REWARD_FAIL_FATIGUE:
		iMessage = 6045;
		break;


	case RC_RANKING_NOT_REGIST_INVALID_PERIOD:
#if defined (_JAPAN)
		// ???? ????????		
		return;
#else
		iMessage = 5512;
#endif		
		break;
	default:
		iMessage = 5481;					// ????????: ?? ?? ???? ??????????????.
		break;
	}
	
	g_InterfaceManager.GetInterfaceString( iMessage , szMessage ,INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szMessage );

	GlobalFunc::OutputMsg_Notice(type, szMessage);
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan::IsMissionRanking(RANKINGNO no, int iRankSystemType)
{
	RankingManager::LEVEL_AREA* pArea = GET_MISSION_RANK_MANAGER()->FindLVArea(no);

	if(pArea)
	{
		if((int)pArea->m_RankingType == iRankSystemType)
			return true;

	}

	return false;
	
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
