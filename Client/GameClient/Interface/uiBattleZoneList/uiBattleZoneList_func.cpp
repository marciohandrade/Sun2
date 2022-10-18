//------------------------------------------------------------------------------
//  uiBattleZoneList_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneList.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "MapInfoParser.h"
#include "Map.h"
#include "GameConst_Color.h"
#include "uiHelpMan/uiHelpMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZoneList::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
/**
*/
void                    
uiBattleZoneList::ExitZoneList()
{
	if(g_pMap->GetOriginWeather())
	{
		g_pMap->SetRenderWeather(TRUE);
	}

	this->ShowInterface(FALSE);
    this->GetUIMan()->ShowGameInterface(TRUE);

	uiHelpMan* helpMan =
					static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowHelpMenu(TRUE);
	}

	GetManager()->ShowPasswordDlg(false);

}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::Clear()
{
	uiHelpMan* helpMan =
					static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowHelpMenu(FALSE);
	}

    m_cfgs.clear();
#ifdef _INTERNATIONAL_UI
    m_pEdtControl->SetFontID(StrToWzID("st10"));
#else
	m_pEdtControl->SetFontID(StrToWzID("mn12"));	
#endif//_INTERNATIONAL_UI
	m_pEdtControl->SetLimitText(MAX_CHARNAME_LENGTH);
	m_pEdtControl->SetColorCaret(c_InterfaceCaretColor);
    GlobalFunc::SetText(m_pEdtControl, _T(""));

    m_pLstControl->SetColorSelBar(RGBA(0,0,0,125));
    m_pLstControl->SetSellHeight(20);
    m_pLstControl->SetUseReport(true);
	m_pLstControl->SetReportUseCnt(LIST_INDEX_MAX);
    m_pLstControl->SetHideReportRender(true);

    int iCnt = 0;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

	CControlWZ	* pListColum1 = getControl( DLGPOS_LIST_COLUM1 );
	CControlWZ	* pListColum2 = getControl( DLGPOS_LIST_COLUM2 );
	CControlWZ	* pListColum3 = getControl( DLGPOS_LIST_COLUM3 );
	CControlWZ	* pListColum4 = getControl( DLGPOS_LIST_COLUM4 );
	CControlWZ	* pListColum5 = getControl( DLGPOS_LIST_COLUM5 );
	CControlWZ	* pListColum6 = getControl( DLGPOS_LIST_COLUM6 );

	CControlWZ	* pListColum7 = getControl( DLGPOS_LIST_COLUM7 );
	CControlWZ	* pListColum8 = getControl( DLGPOS_LIST_COLUM8 );

#ifdef _INTERNATIONAL_UI
	//=========================================================================================
	//						북미 하드코딩내역
	//
	//	북미버전에서 과도하게 텍스트가 넘어가서 기획과 협의 Waiting(DLGPOS_LIST_COLUM3)
	//  은 뿌리지않고 그곳에 슈방(pc방)마크찍어줄공간확보하고 그옆에 헌팅방 그다음에는 룸네임식으
	//	밀어서 찍기로결정
	//	PS: 기존에 보이지않는 내부적으로 2개의 컬럼이 더 들어간다 DLGPOS_LIST_COLUM4 슈퍼방공간확보
	//		DLGPOS_LIST_COLUM5(헌팅타입:헌팅시에만찍힘)
	//========================================================================================
	//Progress:Waiting은 안보여야된다.
	pListColum3->ShowWindowWZ(WZ_HIDE);
	GlobalFunc::SetReportButtonInfo( m_pLstControl, iCnt,	34, NULL );			//0:No.:1,2
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt, 52, NULL );			//1:Type:missin or hunting
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	20 , NULL );		//2:Progress:wating---> RoomMark 찍을공간확보20
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt, 80, NULL );			//3:Time Attack(헌팅방일때 헌티방타입이 찍히는공간확보)
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	3, NULL );			//4:Time Attack과 오른쪽에 올 방제목사이의 약간의 Gap공간
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	180, NULL );		//5:RoomName
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	25 , NULL );		//6:Players
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	240 , NULL );		//7:MapName		
#else
	GlobalFunc::SetReportButtonInfo( m_pLstControl, iCnt, pListColum1->GetSize().right + 4, NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt, pListColum2->GetSize().right +2, NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	pListColum3->GetSize().right - 8 , NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt, pListColum7->GetSize().right, NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	pListColum8->GetSize().right - 9, NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	pListColum4->GetSize().right - 85, NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	pListColum5->GetSize().right , NULL );
	GlobalFunc::SetReportButtonInfo( m_pLstControl, ++iCnt,	pListColum6->GetSize().right , NULL );
#endif//_INTERNATIONAL_UI

    this->GetManager()->ClearZoneListItems();
    this->GetManager()->RequestZoneList();
    m_pVScrollControl->ShowWindowWZ(WZ_HIDE);

    m_timerRefresh.On(10);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::updateCheckBtns()
{
    for (int i=0; i<eCHKVIEW_MAX; ++i)
    {
        m_pChkBtnViewControls[i]->SetCheckState(true);
    }
 
    m_pChkBtnViewControls[m_cfgs.viewType]->SetCheckState(false);

    for (int i=0; i<eCHKSEARCH_MAX; ++i)
    {
        m_pChkBtnSearchControls[i]->SetCheckState(false);
    }

    m_pChkBtnSearchControls[m_cfgs.searchType]->SetCheckState(true);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::renderTabs()
{
    uicBattleZoneList_Tab* pcfgTab = &m_cfgs.mTab;

    DWORD dwListTabColor = pcfgTab->dwListTabColor;
    DWORD dwListTabColorShadowed = pcfgTab->dwListTabColorShadowed;

    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    RECT rc_listctrl;
    rc_listctrl = this->m_pTabControl->GetSize();
    rc_listctrl.top += 4;

    RECT rc;
    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));

    int left_margin = 0;
    int right_margin = 0;
    int top_margin = 0;
    int strCode = 0;

    //
    //  방 번호
    //
    left_margin = right_margin;
    right_margin = right_margin + 30;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[0];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
    g_InterfaceManager.GetInterfaceString( strCode, szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);

    //
    //  헌팅 / 미션
    //
    left_margin = right_margin;
    right_margin = right_margin + 62;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[1];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
    g_InterfaceManager.GetInterfaceString( strCode, szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);

    //
    //  진행
    //
    left_margin = right_margin;
    right_margin = right_margin + 60;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[2];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
	g_InterfaceManager.GetInterfaceString(eST_PROGRESSING,szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);

    //
    //  방 제목
    //
    left_margin = right_margin;
    right_margin = right_margin + 276;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[3];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
    g_InterfaceManager.GetInterfaceString( strCode, szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);

    //
    //  인원수 %d/%d
    //
    left_margin = right_margin;
    right_margin = right_margin + 55;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[4];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
    g_InterfaceManager.GetInterfaceString( strCode, szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);

    //
    //  맵 이름
    //
    left_margin = right_margin;
    right_margin = right_margin + 143;
    top_margin = 21;
    strCode = pcfgTab->vTabCodes[5];
    SetRect(&rc, rc_listctrl.left + left_margin, rc_listctrl.top, rc_listctrl.left + right_margin, rc_listctrl.top + top_margin);
    g_InterfaceManager.GetInterfaceString( strCode, szMessage, INTERFACE_STRING_LENGTH );
    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage, &rc, dwListTabColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER, ETS_SHADOW);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::updateLists()
{
    m_pLstControl->ShowWindowWZ(WZ_SHOW);
    
    int backup_select_index = m_pLstControl->GetCurSel();
    m_pLstControl->DeleteItemAll();
    m_pLstControl->SetBeginDrawIndex(0);

    BYTE byView = eBATTLE_ZONE_LIST_ITEM_MAX;
    int totalListSize = 0;

    const std::vector<cBattleZoneListItem>& zoneListItem = GetManager()->GetZoneList(m_cfgs.viewType);

	if(zoneListItem.size() > 2)
	{
		totalListSize = ((zoneListItem.size() - 1) / c_iMAX_ROOM);
	}

	if (totalListSize)
	{
		m_pVScrollControl->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
		m_pVScrollControl->ShowWindowWZ(WZ_HIDE);
	}

    // setting scroll
    m_pVScrollControl->SetScrollRangeWZ(0, totalListSize);
    int curPos = m_pVScrollControl->GetScrollPosWZ();
    if (curPos > totalListSize)
    {
        curPos--;
        if (curPos<0) curPos = 0;
        m_pVScrollControl->SetScrollPosWZ(0);
    }

	m_keys.clear();

	if(zoneListItem.size() == 0) 
	{
		m_ZoneListInfo.m_iType = NULL;
		m_ZoneListInfo.m_iZoneListEnd = 0;
		m_ZoneListInfo.m_iZoneListEnd =0;

		return;
	}
    // make list table
	int iStartIndex = curPos * c_iMAX_ROOM;
	int iEndIndex = 0;
	if((int)zoneListItem.size() < (iStartIndex + c_iMAX_ROOM))
	{
		iEndIndex = zoneListItem.size();
	}
	else
	{
		iEndIndex = iStartIndex + c_iMAX_ROOM;
	}

	m_ZoneListInfo.m_iZoneListEnd	= iEndIndex;
	m_ZoneListInfo.m_iZoneListStart = iStartIndex;
	m_ZoneListInfo.m_iType			= byView;

#ifdef _INTERNATIONAL_UI
	DWORD dwListFont = StrToWzID("st10");
	DWORD dwListNFont = StrToWzID("st10");
#else
	DWORD dwListFont = StrToWzID("mn12");
	DWORD dwListNFont = StrToWzID("tt12");
#endif//_INTERNATIONAL_UI
	
	for (int i=iStartIndex; i<iEndIndex; ++i)
    {
		cBattleZoneListItem* pZoneListItem2 =
            (cBattleZoneListItem *)&zoneListItem[i];

		TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

		int idx = m_pLstControl->InsertItemBack();

        int roomKey;
        switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: {roomKey = pZoneListItem2->m_MissionLobbyRoomInfo.m_Key;} break;
        case eBATTLE_ZONE_LIST_ITEM_PVP: {roomKey = pZoneListItem2->m_PVPRoomInfo.m_Key;} break;
        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: {roomKey = pZoneListItem2->m_CHuntingLobbyRoomInfo.m_Key;} break;
        default: {roomKey = 0;} break;
        }

		//	번호
        int iCnt = LIST_INDEX_NUM;
        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), roomKey);
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListFont,
                            DT_VCENTER | DT_CENTER,
                            RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA (255, 255, 255, 255));


        m_pLstControl->SetItemData(idx, (int *)roomKey);

        m_keys.push_back(roomKey);

        int roomType;
        switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: {roomType = eST_MISSION;} break;
        case eBATTLE_ZONE_LIST_ITEM_PVP: {roomType = eST_BATTLE_PVP;} break;
        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: {roomType = eST_HUNTING;} break;
        default: {roomType = 0;} break;
        }
        
        g_InterfaceManager.GetInterfaceString( roomType, szMessage, INTERFACE_STRING_LENGTH );

		//	종류
        iCnt = LIST_INDEX_TYPE;
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListFont,
                            DT_VCENTER | DT_CENTER,
                            RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA (255, 255, 255, 255));

        int roomStatus;
        switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: {roomStatus = 1;} break;
        case eBATTLE_ZONE_LIST_ITEM_PVP: {roomStatus = 2;} break;
        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: {roomStatus = 1;} break;
        default: {roomStatus = 0;} break;
        }

        g_InterfaceManager.GetInterfaceString(eST_WAITING,szMessage,INTERFACE_STRING_LENGTH);

        if (roomStatus == 1)
			g_InterfaceManager.GetInterfaceString(eST_WAITING,szMessage,INTERFACE_STRING_LENGTH);
        else if (roomStatus == 2)
			g_InterfaceManager.GetInterfaceString(eST_GOING,szMessage,INTERFACE_STRING_LENGTH);

#ifdef _INTERNATIONAL_UI
		//영문(북미)버전에서는 안들어감(Waiting 진행도)
		// 리스트인덱스는 4번째임 5번째는 RoomName
#else
		//  진행
        iCnt = LIST_INDEX_STATE;
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListFont,
                            DT_VCENTER | DT_CENTER,
                            RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(255, 255, 255, 255));
#endif//_INTERNATIONAL_UI

		//	방 타입(경쟁헌팅의 경우에만 표시)
#ifdef _INTERNATIONAL_UI
		//북미버전에는 Waithing이 빠지고 이곳에 [Time Attack] or [Survival]이 들어간다
		iCnt = LIST_INDEX_SUPERROOM_TYPE;
#else
		iCnt = LIST_INDEX_CHUNTING_TYPE;
#endif//_INTERNATIONAL_UI
		const WzColor c_colorSurvival = RGBA(71,255,1,255);
		const WzColor c_colorTimeAttack = RGBA(222,255,57,255);
		const WzColor c_colorDeathMatch = RGBA(255,32,33,255);
		
		DWORD dwCHuntingtTextColor = RGBA(180, 180, 180, 255);
		DWORD dwCHuntingtSelTextColor = RGBA(255, 255, 255, 255);

		if(pZoneListItem2->iType == eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY)
		{
			TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};
			switch(pZoneListItem2->m_CHuntingLobbyRoomInfo.m_AdditionalInfo.m_Mode)
			{
			case eCOMPETITION_MODE_SURVIVAL:
				{
					dwCHuntingtTextColor = c_colorSurvival;
					dwCHuntingtSelTextColor = c_colorSurvival;
					g_InterfaceManager.GetInterfaceString(714,szTemp,INTERFACE_STRING_LENGTH);
				}
				break;

			case eCOMPETITION_MODE_TIME_ATTACK:
				{
					dwCHuntingtTextColor = c_colorTimeAttack;
					dwCHuntingtSelTextColor = c_colorTimeAttack;
					g_InterfaceManager.GetInterfaceString(715,szTemp,INTERFACE_STRING_LENGTH);
				}
				break;

			case eCOMPETITION_MODE_DEATH_MATCH:
				{
					dwCHuntingtTextColor = c_colorDeathMatch;
					dwCHuntingtSelTextColor = c_colorDeathMatch;
					g_InterfaceManager.GetInterfaceString(9311,szTemp,INTERFACE_STRING_LENGTH);
				}
				break;
			}

			Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("[%s]"), szTemp);
		}
		else
		{
			Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T(""));
		}


		GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListNFont,
                            DT_VCENTER | DT_RIGHT,
                            RGBA(0,0,0,0), dwCHuntingtTextColor, dwCHuntingtSelTextColor);

		BYTE	SuperRoomType = eROOM_SPECIAL_NORMAL;
		//	방 제목
		switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: 
			{
				Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), pZoneListItem2->m_MissionLobbyRoomInfo.m_pszRoomTitle);
				SuperRoomType =  pZoneListItem2->m_MissionLobbyRoomInfo.m_RoomInfo.m_SpecialType;
			} 
			break;


        case eBATTLE_ZONE_LIST_ITEM_PVP: 
			{
				Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), pZoneListItem2->m_PVPRoomInfo.m_pszRoomTitle);
				SuperRoomType = pZoneListItem2->m_PVPRoomInfo.m_RoomInfo.m_SpecialType;
			} 
			break;

        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: 
			{
				Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), pZoneListItem2->m_CHuntingLobbyRoomInfo.m_pszRoomTitle);
				SuperRoomType = pZoneListItem2->m_CHuntingLobbyRoomInfo.m_RoomInfo.m_SpecialType;
			} 
			break;


        default: 
			{
				Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("NULL"));
				SuperRoomType = eROOM_SPECIAL_NORMAL;
			} 
			break;
        }

		DWORD RoomNameColor = GetSuperMasterFontColor(SuperRoomType);

        iCnt = LIST_INDEX_ROOM_NAME;
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListNFont,
                            DT_VCENTER | DT_LEFT,
							RGBA(0,0,0,0), RoomNameColor, RGBA(255, 255, 255, 255));
                            

        int roomMin, roomMax;
        switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: {roomMin = pZoneListItem2->m_MissionLobbyRoomInfo.m_CurUserNum; roomMax = pZoneListItem2->m_MissionLobbyRoomInfo.m_MaxLimitUserNum;} break;
        case eBATTLE_ZONE_LIST_ITEM_PVP: {roomMin = pZoneListItem2->m_PVPRoomInfo.m_CurUserNum; roomMax = pZoneListItem2->m_PVPRoomInfo.m_MaxLimitUserNum;} break;
        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: {roomMin = pZoneListItem2->m_CHuntingLobbyRoomInfo.m_CurUserNum; roomMax = pZoneListItem2->m_CHuntingLobbyRoomInfo.m_MaxLimitUserNum;} break;
        default: {roomMin = 1; roomMax = 5;} break;
        }

        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d/%d"), roomMin, roomMax);

		//	인원수
        iCnt = LIST_INDEX_USER_NUM;
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListFont,
                            DT_VCENTER | DT_CENTER,
                            RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(255, 255, 255, 255));

        int roomMapCode;
        switch (pZoneListItem2->iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: {roomMapCode = pZoneListItem2->m_MissionLobbyRoomInfo.m_MapCode;} break;
        case eBATTLE_ZONE_LIST_ITEM_PVP: {roomMapCode = pZoneListItem2->m_PVPRoomInfo.m_MapCode;} break;
        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: {roomMapCode = pZoneListItem2->m_CHuntingLobbyRoomInfo.m_MapCode;} break;
        default: {roomMapCode = 50001;} break;
        }

        const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(roomMapCode);
        if (mapInfo)
        {
            g_InterfaceManager.GetInterfaceString( mapInfo->dwNCode, szMessage, INTERFACE_STRING_LENGTH );
        }
        else
        {
            Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("Unknown"));
        }

		//	맵
        iCnt = LIST_INDEX_MAP;
        GlobalFunc::SetItem(m_pLstControl, idx, iCnt,
                            szMessage,
                            dwListFont,
                            DT_VCENTER | DT_CENTER,
                            RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(255, 255, 255, 255));

        if (c_iMAX_ROOM <= idx)
            break;
    }

    if (backup_select_index < m_pLstControl->GetListRowCnt())
    {
        m_pLstControl->SetCurSel(backup_select_index);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::_joinRoom(bool bShowMessage/* = true*/)
{
	int curSel = m_pLstControl->GetCurSel();
    if (curSel != -1)
    {
        //! 배틀존 입장시 날개 접음
        g_pHero->SendWingsControlByRider(false, false);
        g_pHero->SendWingsControlByAccessory(false, false);

		int roomKey = (int)m_pLstControl->GetItemData(curSel);

		if(GetManager()->IsSearchMode())
		{
			this->GetManager()->RequestJoinPrivateZone(roomKey);
		}
		else
		{
			this->GetManager()->RequestJoinZone(roomKey);
		}
    }
    else
    {
		if(bShowMessage)
		{
			//선택하신 방이 없습니다.
			TCHAR	szMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(eST_NO_SELECTED_ROOM ,szMessage,INTERFACE_STRING_LENGTH);

			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(szMessage);

		}
    }
}

void uiBattleZoneList::RenderSuperMasterIcon()
{
    const std::vector<cBattleZoneListItem>& zoneListItem = GetManager()->GetZoneList(m_cfgs.viewType);

	if( zoneListItem.size() == 0 )
		return;

	g_pSunRenderer->Set2DMode(TRUE);

	BYTE	SuperRoomType = eROOM_SPECIAL_NORMAL;
	int		zoneListCount = 0;

	
	for ( int i = m_ZoneListInfo.m_iZoneListStart ; i < m_ZoneListInfo.m_iZoneListEnd ; i++ )
	{
		BASE_ROOMINFO * pBaseRoomInfo = NULL;
		cBattleZoneListItem* pZoneListItem = (cBattleZoneListItem *)&zoneListItem[i];

		if( pZoneListItem )
		{
			switch (pZoneListItem->iType)
			{
			case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY: 
				{
					pBaseRoomInfo =  &pZoneListItem->m_MissionLobbyRoomInfo.m_RoomInfo;
				} 
				break;


			case eBATTLE_ZONE_LIST_ITEM_PVP: 
				{
					pBaseRoomInfo = &pZoneListItem->m_PVPRoomInfo.m_RoomInfo;
				} 
				break;

			case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY: 
				{
					pBaseRoomInfo = &pZoneListItem->m_CHuntingLobbyRoomInfo.m_RoomInfo;
				} 
				break;

			}

			
			if( !pBaseRoomInfo )
				continue;

			const int c_zoneListHeight = 20;
			RECT rt = m_rtList;
			rt.top +=  zoneListCount *c_zoneListHeight;
			rt.bottom = rt.top + c_zoneListHeight;
			zoneListCount += 1;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
            RECT rect_guild_pvp = rt;
#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
			if (pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_PCBANG))
			{
                g_InterfaceManager.RenderBattleRoomIcon(
                    InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
                    &rt);
                rect_guild_pvp.left  += 20;
                rect_guild_pvp.right += 20;
			}
			else
#endif
			if (pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_ITEM))
			{
                g_InterfaceManager.RenderBattleRoomIcon(
                    InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
                    &rt);
                rect_guild_pvp.left  += 20;
                rect_guild_pvp.right += 20;
			}

			if (pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
			{
				g_InterfaceManager.RenderBattleRoomIcon(
                    InterfaceManager::eBATTLE_ICON_GUILD_PVP_ROOM,
                    &rect_guild_pvp);
			}
#else
			switch (pBaseRoomInfo->m_SpecialType)
			{
#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
			case eROOM_SPECIAL_PCBANG_SUPER_ROOM:
				{
					g_InterfaceManager.RenderBattleRoomIcon(
                        InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
                        &rt);
				}
				break;
#endif
			case eROOM_SPECIAL_ITEM1_SUPER_ROOM:
				{
					g_InterfaceManager.RenderBattleRoomIcon(
                        InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
                        &rt);
				}
				break;
			}
#endif

		}
	}
}

DWORD uiBattleZoneList::GetSuperMasterFontColor(BYTE byRoomType)
{
	DWORD fontColor;
	switch( byRoomType )
	{
#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
	case eROOM_SPECIAL_PCBANG_SUPER_ROOM:
		{
			fontColor = RGBA(0 , 176, 240,255);
		}
		break;
#endif
	case eROOM_SPECIAL_ITEM1_SUPER_ROOM:
		{
			fontColor = RGBA(183 , 27, 23, 255);
		}
		break;

	case eROOM_SPECIAL_NORMAL:
	default:
		{
			fontColor  = RGBA(180, 180, 180, 255);
		}
		break;
	}

	return fontColor;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
