//------------------------------------------------------------------------------
//  uiQuestMissionList_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestMissionList.h"
#include "MissionManager_Concrete.h"
#include "MissionRewardParser.h"
#include "Hero.h"
//------------------------------------------------------------------------------
/**
*/
uiQuestMan*
uiQuestMissionList::GetManager()
{
    uiQuestMan* puiQuestMan =
        static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    assert (puiQuestMan);
    return puiQuestMan;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::ClearInfo()
{
	if ( !CheckControls() )
		return;

	int iCurMission = GetManager()->GetCurSelectMissionIndex();
	if ( -1 == iCurMission )
	{
		for ( int i = 0; i < MAX_MISSION_TYPE; ++i)
		{
			GlobalFunc::SetCaption(m_pCtrlStaticMissionInfo[i], _T(""));
		}

		for ( int i = 0; i < MAX_REWARD_COUNT; ++i)
		{
			GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], _T(""));
		}

		SetCaptionCurSelMission(_T(""));
	}

	ShowMissionList(FALSE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::initInfo()
{
	m_pCtrlCheckCategory[0]->SetCheckState(TRUE);	
	m_pCtrlCheckCategory[1]->SetCheckState(FALSE);	

	ShowMissionList(FALSE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::updateInfo()
{

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::initMissionGroupMap()
{
    // 미션 맵에대한 그룹 정보를 모은다.

	m_MissionMapGroupMap.clear();


	if ( !CheckControls() )
	{
		return;
	}

	sMAPINFO_HASH & rMapInfoHash = MapInfoParser::Instance()->GetMapInfoHash();
	sMAPINFO_HASH::const_iterator MAPINFO_itr = rMapInfoHash.begin();
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

	GetCtrlMissionList()->SetUnuseSelBar(TRUE);
	GetCtrlMissionList()->SetColorSelBar(BAR_COLOR_SELECT_LIST);
	GetCtrlMissionList()->SetReportIsOutline(FALSE);
	GetCtrlMissionList()->SetUseReport(FALSE);

	// 미션 맵 

	int index = -1; 

	MISSION_GROUP_ITR itr = m_MissionMapGroupMap.begin();

	for ( ; itr != m_MissionMapGroupMap.end(); ++itr)
	{
		MapGroup * pMapGroup = (MapGroup *)itr->second;		

		if ( !pMapGroup )
		{
			continue;
		}

		sMAPINFO * pMapInfo = (sMAPINFO *)(const sMAPINFO *)pMapGroup->GetMapInfoIndex(0);

		pMapInfo = GlobalFunc::GetMainMapFromGroup( (MAPCODE)itr->first );

		if ( pMapInfo )
		{	
			index = GetCtrlMissionList()->InsertItemBack();

			GetUIMan()->GetInterfaceString( 
				pMapInfo->dwANCode, 
				m_szText, 
				INTERFACE_STRING_LENGTH );

			GlobalFunc::SetItem(
				GetCtrlMissionList(), 
				index, 0, m_szText, 
				FONT_MISSION_LIST_GENERAL, 
				DT_VCENTER | DT_CENTER, 
				TEXT_BG_COLOR_MISSION_LIST_GENERAL,
				TEXT_COLOR_MISSION_LIST_GENERAL,
				TEXT_COLOR_ON_SELECT_LIST );


			GetCtrlMissionList()->SetItemData(index, (VOID*)(pMapGroup));
		}
	}

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::refreshMissionInfo()
{
	if ( !g_pMission_Concrete )
		return;

	int iMissionIndex = GetManager()->GetCurSelectMissionIndex();


	for ( int i = 0; i < MAX_MISSION_TYPE; ++i )
	{
		GlobalFunc::SetCaption( m_pCtrlStaticMissionInfo[i], _T(""));
		m_pCtrlStaticMissionInfo[i]->ShowWindowWZ(WZ_HIDE);
	}
	
	MapGroup * pMapGroup = (MapGroup *)GetCtrlMissionList()->GetItemData(iMissionIndex);
	
	if (!pMapGroup)
	{
		return;
	}

	for ( int i = 0; i < MAX_MISSION_TYPE; ++i )
	{
		const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(i);
		if ( pMapInfo )
		{
			m_pCtrlStaticMissionInfo[i]->ShowWindowWZ(WZ_SHOW);
			GetUIMan()->GetInterfaceString(
				pMapInfo->dwNCode, 
				m_szText, 
				INTERFACE_STRING_LENGTH);

			GlobalFunc::SetCaption( m_pCtrlStaticMissionInfo[i], m_szText);

			if (GetManager()->GetCurSelectMissionSubIndex() == i)
			{
				m_pCtrlStaticMissionInfo[i]->SetCheckState(TRUE);
			}
			else
			{
				m_pCtrlStaticMissionInfo[i]->SetCheckState(FALSE);
			}

		}		
		else
		{
			GlobalFunc::SetCaption( m_pCtrlStaticMissionInfo[i], _T(""));
			m_pCtrlStaticMissionInfo[i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	int iSubIndex = GetManager()->GetCurSelectMissionSubIndex();
	if (-1 == iSubIndex) 
	{
		GetManager()->SetCurSelectMissionSubIndex(0);
		iSubIndex = 0;
	}

	const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(iSubIndex);
	if (!pMapInfo)
	{
		return;
	}

	GetUIMan()->GetInterfaceString(
		pMapInfo->dwANCode, 
		m_szText, 
		INTERFACE_STRING_LENGTH);

	SetCaptionCurSelMission(m_szText);

	refreshMissionReward();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::clickChkCategory(POSTYPE pos)
{
	switch (pos)
	{
	case eDLGPOS_CHK_MISSION:
		{
			// 현재가 미션창이기 때문에 무시.
		}
		break;
	case eDLGPOS_CHK_QUEST:
		{
			GetManager()->OpenForceQuestList();
		}
		break;
	}

	pos -= QUEST_CATEGORY_BEGIN;
	for ( int i = 0; i < QUEST_CATEGORY_SIZE; ++i )
	{
		m_pCtrlCheckCategory[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckCategory[pos]->SetCheckState(TRUE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::clickMissionList()
{
	int index = GetCtrlMissionList()->GetCurSel();	
	if ( -1 == index )
	{
		return;
	}

	MapGroup * pGroup = NULL;

	assert( index < (int)m_MissionMapGroupMap.size() );
	if ( index < (int)m_MissionMapGroupMap.size() )
	{
		pGroup = (MapGroup *)GetCtrlMissionList()->GetItemData(index);
		assert(pGroup);
		if ( pGroup )
		{
			GetManager()->SetCurSelectMissionIndex(index);
			
			sMAPINFO * pMapInfo = GlobalFunc::GetMainMapFromGroup(pGroup);
			if (pMapInfo)
			{
				GetManager()->SetCurSelectMissionSubIndex(pMapInfo->byMType);
				GetUIMan()->GetInterfaceString( 
					pMapInfo->dwANCode, 
					m_szText, 
					INTERFACE_STRING_LENGTH );
			}

			SetCaptionCurSelMission( m_szText );
		}
	}

	ShowMissionList(FALSE);

	UpdateMissionInfo();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::clickMissionBtn()
{
	RECT rcRect = GetCtrlCurSelMission()->GetSizeRect();

	int iWidth  = GetCtrlMissionList()->GetSizeRect().right - GetCtrlMissionList()->GetSizeRect().left;
	int iHeight = GetCtrlMissionList()->GetSizeRect().bottom - GetCtrlMissionList()->GetSizeRect().top;
	int x = rcRect.left;
	int y = rcRect.bottom + 2;
	GetCtrlMissionList()->MoveWindowWZ( (float)x, (float)y, (float)iWidth, (float)iHeight );

	if ( WZ_SHOW == GetCtrlMissionList()->GetState() )
	{
		ShowMissionList(FALSE);		
	}
	else
	{
		ShowMissionList(TRUE);		
	}
	
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::clickMissionSubBtn(POSTYPE pos)
{
    GetManager()->SetCurSelectMissionSubIndex(pos);

	UpdateMissionInfo();
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiQuestMissionList::refreshMissionReward()
{  
	int iMissionIndex = GetManager()->GetCurSelectMissionIndex();

	MapGroup * pMapGroup = (MapGroup *)GetCtrlMissionList()->GetItemData(iMissionIndex);
	assert(pMapGroup);

	if (!pMapGroup)
	{
		return;
	}

	int iSubIndex = GetManager()->GetCurSelectMissionSubIndex();
	if (-1 == iSubIndex) 
	{
		GetManager()->SetCurSelectMissionSubIndex(0);
		iSubIndex = 0;
	}

	const sMAPINFO * pMapInfo = pMapGroup->GetMapInfoIndex(iSubIndex);
	if (!pMapInfo)
	{
		return;
	}

	WORD wMissionPoint = 
		g_pMission_Concrete->GetMissionPoint((BYTE)pMapInfo->MapCode);
	WORD wClearRewardMission = 0;


	// 보상 미완료 : 9037
	for ( int i = 0; i < 3; ++i)
	{
		GetUIMan()->GetInterfaceString(9037, m_szText, INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], m_szText);		
	}

	MissionRewardInfo * pRewardInfo = 
		MissionRewardParser::Instance()->GetMissionReward((BYTE)pMapInfo->MapCode);

	if ( !pRewardInfo )
	{
		return;
	}


	int iClass = (int)g_pHero->GetClass();

	int iReward1 = pRewardInfo->CLASSs[iClass].REWARDs[0].m_bMissionNum;
	int iReward2 = pRewardInfo->CLASSs[iClass].REWARDs[1].m_bMissionNum;
	int iReward3 = pRewardInfo->CLASSs[iClass].REWARDs[2].m_bMissionNum;


	if (iReward3 > 0 && 
		wMissionPoint >= iReward3 ) 
	{
		wClearRewardMission = 3;
	}
	else 
	if (iReward2 > 0 &&
		wMissionPoint >= iReward2 ) 
	{
		wClearRewardMission = 2;
	}
	else 
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
	for ( int i = 0; i < wClearRewardMission; ++i)
	{
		GetUIMan()->GetInterfaceString(9036, m_szText, INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption(m_pCtrlStaticRewardInfo[i], m_szText);
	}	
}

//-------------------------------------------------------------------------------------------
/**
*/
void		
uiQuestMissionList::UpdateMissionInfo()
{
	ClearInfo();
	refreshMissionInfo();
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------