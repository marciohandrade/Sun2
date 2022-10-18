//------------------------------------------------------------------------------
//  uiQuestMissionList_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <assert.h>

#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED

#include "util/util.h"
#include "QuestEx.h"
#include "MissionRewardParser.h"
#include "MapInfoParser.h"
#include <Struct.h>
#include <SolarFileReader.h>
#include "CSunOnline.h"
#include "logic.h"

#include "uiQuestMissionList.h"

extern QuestInfoManager2* g_pQuestInfoManager;


//------------------------------------------------------------------------------
/**
*/
uiQuestMissionList::uiQuestMissionList()
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestMissionList::~uiQuestMissionList()
{
    this->ClearInfo();
    m_MissionMapGroupMap.clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::ClearInfo()
{
//	int iCurMission = GetManager()->GetCurSelectMissionIndex();
//	if ( -1 == iCurMission )
//	{
//		for ( int i = 0; i < MAX_MISSION_TYPE; ++i)
//		{
//			GlobalFunc::SetCaption(m_pCtrlStaticMissionInfo[i], _T(""));
//		}
//
//		for ( i = 0; i < MAX_REWARD_COUNT; ++i)
//		{
//			GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], _T(""));
//		}
//
//		SetCaptionCurSelMission(_T(""));
//	}
//
//	ShowMissionList(FALSE);
}

void
uiQuestMissionList::initMissionGroupMap()
{
	// 미션 맵에대한 그룹 정보를 모은다.
	m_MissionMapGroupMap.clear();

	sMAPINFO_HASH & rMapInfoHash = MapInfoParser::Instance()->GetMapInfoHash();
	sMAPINFO_HASH::iterator MAPINFO_itr = rMapInfoHash.begin();
	sMAPINFO * pMapInfo = NULL;
	while (MAPINFO_itr != rMapInfoHash.end() )
	{
		pMapInfo =   MAPINFO_itr->second;
		MAPINFO_itr++;
		if ( pMapInfo->byMKind != eZONETYPE_MISSION )
		{
			continue;
		}

		MISSION_GROUP_ITR itr = m_MissionMapGroupMap.find(pMapInfo->MapKind);
		if ( itr == m_MissionMapGroupMap.end() )
		{
			MapGroup * pGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );
			assert(pGroup);
			m_MissionMapGroupMap.insert( MISSION_GROUP_PAIR(pMapInfo->MapKind, pGroup) );
		}
	}

	// 미션 맵 
	MISSION_GROUP_ITR itr = m_MissionMapGroupMap.begin();

	for ( ; itr != m_MissionMapGroupMap.end(); ++itr)
	{
		MapGroup * pMapGroup = (MapGroup *)itr->second;		

		if ( !pMapGroup )
		{
			continue;
		}

		const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(0);
		if ( pMapInfo )
		{
			char pszName[256];
			GetStringByCode(pMapInfo->dwNCode, pszName,256);
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::UpdateMissionInfo(MissionManager* pMissionMgr, BSTR* bstrXml)
{
	ClearInfo();

	CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
		CMarkupArchive::ArchiveModeStoring |
		CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeQuestList.xml");

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// refreshMissionInfo
	//////////////////////////////////////////////////////////////////////////

//	for ( int i = 0; i < MAX_MISSION_TYPE; ++i )
//	{
//		if ( i >= 4 ) // 현재는 메인, 서브1, 2, 3 까지 가능.
//			continue;
//	}

	MISSION_GROUP_ITR itr = m_MissionMapGroupMap.begin();
	for ( ; itr != m_MissionMapGroupMap.end(); ++itr)
	{
		MapGroup * pMapGroup = (MapGroup *)itr->second;		
		if (!pMapGroup)
		{
			return;
		}

		// 메인
		// 서브1
		// 서브2
		// 서브3
		for ( int i = 0; i < MAX_MISSION_TYPE; ++i )
		{
			if ( i >= 4 ) // 현재는 메인, 서브1, 2, 3 까지 가능.
				continue;

			const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(i);
			if ( pMapInfo )
			{
				char pszName[256];
				GetStringByCode(pMapInfo->dwNCode, pszName,256);
			}		
			else
			{
			//	GlobalFunc::SetCaption( m_pCtrlStaticMissionInfo[i], _T(""));				
			}
		}

//		int iSubIndex = GetManager()->GetCurSelectMissionSubIndex();
//		if (-1 == iSubIndex) 
//		{
//			GetManager()->SetCurSelectMissionSubIndex(0);
//			iSubIndex = 0;
//		}
//
//		const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(iSubIndex);
//		if (!pMapInfo)
//		{
//			return;
//		}
//
//		char pszName[256];
//		GetStringByCode(pMapInfo->dwNCode, pszName,256);	
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// refreshMissionReward
	//////////////////////////////////////////////////////////////////////////
	MapGroup * pMapGroup = NULL;
	assert(pMapGroup);
	if (!pMapGroup)
	{
		return;
	}

	int iSubIndex = 0; //GetManager()->GetCurSelectMissionSubIndex();
	if (-1 == iSubIndex) 
	{
		//	GetManager()->SetCurSelectMissionSubIndex(0);
		iSubIndex = 0;
	}

	const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(iSubIndex);
	if (!pMapInfo)
	{
		return;
	}

	WORD wMissionPoint = pMissionMgr->GetMissionPoint((BYTE)pMapInfo->MapCode);
	WORD wClearRewardMission = 0;

	// 보상 미완료 : 9037
	for ( int i = 0; i < 3; i++)
	{
		char pszName[256];
		GetStringByCode(9037, pszName,256);
		//	GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], m_szText);		
	}

	MissionRewardInfo * pRewardInfo = 
		MissionRewardParser::Instance()->GetMissionReward((BYTE)pMapInfo->MapCode);
	if ( !pRewardInfo )
	{
		return;
	}

	int iClass = (int)0; //g_pHero->GetClass();

	int iReward1 = pRewardInfo->CLASSs[iClass].REWARDs[0].m_bMissionNum;
	int iReward2 = pRewardInfo->CLASSs[iClass].REWARDs[1].m_bMissionNum;
	int iReward3 = pRewardInfo->CLASSs[iClass].REWARDs[2].m_bMissionNum;


	if (iReward3 > 0 && 
		wMissionPoint >= iReward3 ) 
	{
		wClearRewardMission = 3;
	}
	else 
	{
		if (iReward2 > 0 &&
			wMissionPoint >= iReward2 ) 
		{
			wClearRewardMission = 2;
		}
		else 
		{
			if ( wMissionPoint > 0 && 
				iReward1 >= wMissionPoint ) 
			{
				wClearRewardMission = 1;
			}
			else
			{
				wClearRewardMission = 0;
			}

			// 보상 완료 : 9036
			for (int i = 0; i < wClearRewardMission; i++)
			{
				char pszName[256];
				GetStringByCode(9036, pszName,256);
				//	GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], m_szText);
			}
		}
	}
}

#endif//#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
