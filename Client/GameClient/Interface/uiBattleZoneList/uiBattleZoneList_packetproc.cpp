//------------------------------------------------------------------------------
//  uiBattleZoneList_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneList.h"

#ifdef _CK_MOD_UI_ZONE_RENEWAL
#else

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch (pMsg->m_byProtocol)
    {
        // 미션 로비방 정보 응답
    case CG_ZONE_LIST_MISSIONLOBBY_ACK:
        {
			OutputDebugString(_T("CG_ZONE_LIST_MISSIONLOBBY_ACK\n"));

            this->FindErase(_PKID_CG_ZONE_LIST_MISSIONLOBBY_ACK);

            MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK *msg =
                (MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK *)pMsg;

            if (msg->m_Count>MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK::_MAX_ROOM_NUM)
            {
                ASSERT (!"m_Count가 MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK::_MAX_ROOM_NUM를 넘어섰다.");
                break;
            }

            std::vector<cBattleZoneListItem>        zoneListBak;
            std::vector<cBattleZoneListItem>        zoneListNew;

            for (int i=0; i<this->vBattleZoneListItems.size(); ++i)
            {
                zoneListBak.push_back(this->vBattleZoneListItems[i]);
            }

            for (i=0; i<msg->m_Count; ++i)
            {
                bool bNewList = true;

                for (int j=0; j<this->vBattleZoneListItems.size(); ++j)
                {
                    if (this->vBattleZoneListItems[j].iType
                        != eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY)
                        continue;

                    cBattleZoneListItem srcBattleZoneListItem;
                    srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY;

                    memcpy ((MISSION_ROOMINFO *)&srcBattleZoneListItem.m_MissionLobbyRoomInfo,
                            (MISSION_ROOMINFO *)&msg->m_RoomInfo[i],
                            sizeof (MISSION_ROOMINFO));

                    if (srcBattleZoneListItem == this->vBattleZoneListItems[j])
                    {
                        // 갱신
                        this->vBattleZoneListItems[j] = srcBattleZoneListItem;

                        bNewList = false;
                        break;
                    }
                }

                if (bNewList)
                {
                    // 추가
                    //if (zoneListNew.size() < c_iMAX_ROOMLIST)
                    {
                        cBattleZoneListItem srcBattleZoneListItem;
                        srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY;

                        memcpy ((MISSION_ROOMINFO *)&srcBattleZoneListItem.m_MissionLobbyRoomInfo,
                                (MISSION_ROOMINFO *)&msg->m_RoomInfo[i],
                                sizeof (MISSION_ROOMINFO));

                        zoneListNew.push_back(srcBattleZoneListItem);
                    }
                }
            }

            int numNewZoneList = zoneListNew.size();

            if (numNewZoneList)
            {
                this->vBattleZoneListItems.clear();

                for (i=0; i<numNewZoneList; ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListNew[((numNewZoneList-1) - i)]);
                }

                for (i=0; i<zoneListBak.size(); ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListBak[i]);
                }

                if (c_iMAX_ROOMLIST < this->vBattleZoneListItems.size())
                {
                    int diff = this->vBattleZoneListItems.size() - c_iMAX_ROOMLIST;

                    for (i=0; i<diff; ++i)
                        this->vBattleZoneListItems.pop_back();
                }
            }

            int cursel = this->m_pCtrlList->GetCurSel();
            this->m_pCtrlList->SetCurSel(cursel + numNewZoneList);
        }
        break;

    case CG_ZONE_LIST_MISSIONLOBBY_NAK:
        {
			OutputDebugString(_T("CG_ZONE_LIST_MISSIONLOBBY_NAK\n"));
            this->FindErase(_PKID_CG_ZONE_LIST_MISSIONLOBBY_NAK);
        }
        break;

        // 헌팅 로비방 정보 응답
    case CG_ZONE_LIST_HUNTINGLOBBY_ACK:
        {
            this->FindErase(_PKID_CG_ZONE_LIST_HUNTINGLOBBY_ACK);

			OutputDebugString(_T("CG_ZONE_LIST_HUNTINGLOBBY_ACK\n"));

            MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK *msg =
                (MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK *)pMsg;

            if (msg->m_Count>MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK::_MAX_ROOM_NUM)
            {
                ASSERT (!"m_Count가 MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK::_MAX_ROOM_NUM를 넘어섰다.");
                break;
            }

            std::vector<cBattleZoneListItem>        zoneListBak;
            std::vector<cBattleZoneListItem>        zoneListNew;

            for (int i=0; i<this->vBattleZoneListItems.size(); ++i)
            {
                zoneListBak.push_back(this->vBattleZoneListItems[i]);
            }

            for (i=0; i<msg->m_Count; ++i)
            {
                bool bNewList = true;

                for (int j=0; j<this->vBattleZoneListItems.size(); ++j)
                {
                    if (this->vBattleZoneListItems[j].iType
                        != eBATTLE_ZONE_LIST_ITEM_HUNTING_LOBBY)
                        continue;

                    cBattleZoneListItem srcBattleZoneListItem;
                    srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING_LOBBY;

                    memcpy ((HUNTING_ROOMINFO *)&srcBattleZoneListItem.m_HuntingLobbyRoomInfo,
                            (HUNTING_ROOMINFO *)&msg->m_RoomInfo[i],
                            sizeof (HUNTING_ROOMINFO));

                    if (srcBattleZoneListItem == this->vBattleZoneListItems[j])
                    {
                        // 갱신
                        this->vBattleZoneListItems[j] = srcBattleZoneListItem;

                        bNewList = false;
                        break;
                    }
                }

                if (bNewList)
                {
                    // 추가
                    //if (zoneListNew.size() < c_iMAX_ROOMLIST)
                    {
                        cBattleZoneListItem srcBattleZoneListItem;
                        srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING_LOBBY;

                        memcpy ((HUNTING_ROOMINFO *)&srcBattleZoneListItem.m_HuntingLobbyRoomInfo,
                                (HUNTING_ROOMINFO *)&msg->m_RoomInfo[i],
                                sizeof (HUNTING_ROOMINFO));

                        zoneListNew.push_back(srcBattleZoneListItem);
                    }
                }
            }

            int numNewZoneList = zoneListNew.size();

            if (numNewZoneList)
            {
                this->vBattleZoneListItems.clear();

                for (i=0; i<numNewZoneList; ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListNew[((numNewZoneList-1) - i)]);
                }

                for (i=0; i<zoneListBak.size(); ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListBak[i]);
                }

                if (c_iMAX_ROOMLIST < this->vBattleZoneListItems.size())
                {
                    int diff = this->vBattleZoneListItems.size() - c_iMAX_ROOMLIST;

                    for (i=0; i<diff; ++i)
                        this->vBattleZoneListItems.pop_back();
                }
            }

            int cursel = this->m_pCtrlList->GetCurSel();
            this->m_pCtrlList->SetCurSel(cursel + numNewZoneList);
        }
        break;

    case CG_ZONE_LIST_HUNTINGLOBBY_NAK:
        {
            this->FindErase(_PKID_CG_ZONE_LIST_HUNTINGLOBBY_NAK);
        }
        break;

        // 진행된 헌팅방 정보 응답
    case CG_ZONE_LIST_HUNTING_ACK:
        {
			OutputDebugString(_T("CG_ZONE_LIST_HUNTING_ACK\n"));

            this->FindErase(_PKID_CG_ZONE_LIST_HUNTING_ACK);

            MSG_CG_ZONE_LIST_HUNTING_ACK *msg =
                (MSG_CG_ZONE_LIST_HUNTING_ACK *)pMsg;

            if (msg->m_Count>MSG_CG_ZONE_LIST_HUNTING_ACK::_MAX_ROOM_NUM)
            {
                ASSERT (!"m_Count가 MSG_CG_ZONE_LIST_HUNTING_ACK::_MAX_ROOM_NUM를 넘어섰다.");
                break;
            }

            std::vector<cBattleZoneListItem>        zoneListBak;
            std::vector<cBattleZoneListItem>        zoneListNew;

            for (int i=0; i<this->vBattleZoneListItems.size(); ++i)
            {
                zoneListBak.push_back(this->vBattleZoneListItems[i]);
            }

            for (i=0; i<msg->m_Count; ++i)
            {
                bool bNewList = true;

                for (int j=0; j<this->vBattleZoneListItems.size(); ++j)
                {
                    if (this->vBattleZoneListItems[j].iType
                        != eBATTLE_ZONE_LIST_ITEM_HUNTING)
                        continue;

                    cBattleZoneListItem srcBattleZoneListItem;
                    srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING;

                    memcpy ((HUNTING_ROOMINFO *)&srcBattleZoneListItem.m_HuntingRoomInfo,
                            (HUNTING_ROOMINFO *)&msg->m_RoomInfo[i],
                            sizeof (HUNTING_ROOMINFO));

                    if (srcBattleZoneListItem == this->vBattleZoneListItems[j])
                    {
                        // 갱신
                        this->vBattleZoneListItems[j] = srcBattleZoneListItem;

                        bNewList = false;
                        break;
                    }
                }

                if (bNewList)
                {
                    // 추가
                    //if (zoneListNew.size() < c_iMAX_ROOMLIST)
                    {
                        cBattleZoneListItem srcBattleZoneListItem;
                        srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING;

                        memcpy ((HUNTING_ROOMINFO *)&srcBattleZoneListItem.m_HuntingRoomInfo,
                                (HUNTING_ROOMINFO *)&msg->m_RoomInfo[i],
                                sizeof (HUNTING_ROOMINFO));

                        zoneListNew.push_back(srcBattleZoneListItem);
                    }
                }
            }

            int numNewZoneList = zoneListNew.size();

            if (numNewZoneList)
            {
                this->vBattleZoneListItems.clear();

                for (i=0; i<numNewZoneList; ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListNew[((numNewZoneList-1) - i)]);
                }

                for (i=0; i<zoneListBak.size(); ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListBak[i]);
                }

                if (c_iMAX_ROOMLIST < this->vBattleZoneListItems.size())
                {
                    int diff = this->vBattleZoneListItems.size() - c_iMAX_ROOMLIST;

                    for (i=0; i<diff; ++i)
                        this->vBattleZoneListItems.pop_back();
                }
            }

            int cursel = this->m_pCtrlList->GetCurSel();
            this->m_pCtrlList->SetCurSel(cursel + numNewZoneList);
        }
        break;

    case CG_ZONE_LIST_HUNTING_NAK:
        {
            this->FindErase(_PKID_CG_ZONE_LIST_HUNTING_NAK);
        }
        break;

		// TODO : 여기서 pvp list를 받으면 처리하자.
	case CG_ZONE_LIST_PVPLOBBY_ACK:
		{
#if defined ( _DEBUG )
			OutputDebugString(_T("CG_ZONE_LIST_PVPLOBBY_ACK\n"));
#endif

            this->FindErase(_PKID_CG_ZONE_LIST_PVP_ACK);

			MSG_CG_ZONE_LIST_PVPLOBBY_ACK * msg = (MSG_CG_ZONE_LIST_PVPLOBBY_ACK *) pMsg;

            if (msg->m_Count > MSG_CG_ZONE_LIST_PVPLOBBY_ACK::_MAX_ROOM_NUM)
            {
                ASSERT (!"m_Count가 MSG_CG_ZONE_LIST_PVPLOBBY_ACK::_MAX_ROOM_NUM를 넘어섰다.");
                break;
            }

            std::vector<cBattleZoneListItem>        zoneListBak;
            std::vector<cBattleZoneListItem>        zoneListNew;

            for (int i=0; i<this->vBattleZoneListItems.size(); ++i)
            {
                zoneListBak.push_back(this->vBattleZoneListItems[i]);
            }

            for (i=0; i<msg->m_Count; ++i)
            {
                bool bNewList = true;

                for (int j=0; j<this->vBattleZoneListItems.size(); ++j)
                {
                    if (this->vBattleZoneListItems[j].iType != eBATTLE_ZONE_LIST_ITEM_PVP)
                        continue;

                    cBattleZoneListItem srcBattleZoneListItem;
                    srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_PVP;

                    memcpy ((PVP_ROOMINFO *)&srcBattleZoneListItem.m_PVPRoomInfo,
                            (PVP_ROOMINFO *)&msg->m_RoomInfo[i],
                            sizeof (PVP_ROOMINFO));

                    if (srcBattleZoneListItem == this->vBattleZoneListItems[j])
                    {
                        // 갱신
                        this->vBattleZoneListItems[j] = srcBattleZoneListItem;

                        bNewList = false;
                        break;
                    }
                }

                if (bNewList)
                {
                    // 추가
                    // if (zoneListNew.size() < c_iMAX_ROOMLIST)
                    {
                        cBattleZoneListItem srcBattleZoneListItem;
                        srcBattleZoneListItem.iType = eBATTLE_ZONE_LIST_ITEM_PVP;

                        memcpy ((PVP_ROOMINFO *)&srcBattleZoneListItem.m_PVPRoomInfo,
                                (PVP_ROOMINFO *)&msg->m_RoomInfo[i],
                                sizeof (PVP_ROOMINFO));

                        zoneListNew.push_back(srcBattleZoneListItem);
                    }
                }
            }

            int numNewZoneList = zoneListNew.size();

            if (numNewZoneList)
            {
                this->vBattleZoneListItems.clear();

                for (i=0; i<numNewZoneList; ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListNew[((numNewZoneList-1) - i)]);
                }

                for (i=0; i<zoneListBak.size(); ++i)
                {
                    this->vBattleZoneListItems.push_back(zoneListBak[i]);
                }
            
                if (c_iMAX_ROOMLIST < this->vBattleZoneListItems.size())
                {
                    int diff = this->vBattleZoneListItems.size() - c_iMAX_ROOMLIST;

                    for (i=0; i<diff; ++i)
                        this->vBattleZoneListItems.pop_back();
                }
            }

            int cursel = this->m_pCtrlList->GetCurSel();
            this->m_pCtrlList->SetCurSel(cursel + numNewZoneList);

		}
		break;

	case CG_ZONE_LIST_PVPLOBBY_NAK:
		{
#if defined ( _DEBUG )
			OutputDebugString(_T("CG_ZONE_LIST_PVPLOBBY_NAK\n"));
#endif
            this->FindErase(_PKID_CG_ZONE_LIST_PVP_NAK);
		}
		break;

    case CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK:
        {
            this->FindErase(_PKID_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK);

            MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK* msg =
                (MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK *)pMsg;
            
            this->m_bSearchedRoom = true;

            this->vCurrentBattleZoneListItems.clear();
            this->m_queriedBattleZone.iType = -1;

            this->m_searchedBattleZone.iType = eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY;
            memcpy ((MISSION_ROOMINFO *)&this->m_searchedBattleZone.m_MissionLobbyRoomInfo,
                    (MISSION_ROOMINFO *)&msg->m_RoomInfo,
                    sizeof (MISSION_ROOMINFO));

            TCHAR	szMessage[INTERFACE_STRING_LENGTH];

            int total = 0;
            int count = 0;

            int start = 0;
            int end = 1;

            this->vCurrentBattleZoneListItems.clear();

            for (int i=start; i<end; ++i)
            {
                this->vCurrentBattleZoneListItems.push_back(m_searchedBattleZone);
                this->m_pCtrlList->InsertItemBack();
                total ++;
            }

            //int start = (this->m_pCtrlVScroll->GetScrollPosWZ()*c_iMAX_ROOM);
            int start2 = this->m_pCtrlVScroll->GetScrollPosWZ();
            this->InitScrollBar(1);
            this->m_pCtrlVScroll->SetScrollPosWZ(start2);
            return;
        }
        break;
    case CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK:
        {
            this->FindErase(_PKID_CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK);

            MSG_CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK* msg =
                (MSG_CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK *)pMsg;

            this->m_bSearchedRoom = true;

            this->vCurrentBattleZoneListItems.clear();
            this->m_queriedBattleZone.iType = -1;

            this->m_searchedBattleZone.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING_LOBBY;
            memcpy ((HUNTING_ROOMINFO *)&this->m_searchedBattleZone.m_HuntingLobbyRoomInfo,
                    (HUNTING_ROOMINFO *)&msg->m_RoomInfo,
                    sizeof (HUNTING_ROOMINFO));

            TCHAR	szMessage[INTERFACE_STRING_LENGTH];

            int total = 0;
            int count = 0;

            int start = 0;
            int end = 1;

            this->vCurrentBattleZoneListItems.clear();

            for (int i=start; i<end; ++i)
            {
                this->vCurrentBattleZoneListItems.push_back(m_searchedBattleZone);
                this->m_pCtrlList->InsertItemBack();
                total ++;
            }

            //int start = (this->m_pCtrlVScroll->GetScrollPosWZ()*c_iMAX_ROOM);
            int start2 = this->m_pCtrlVScroll->GetScrollPosWZ();
            this->InitScrollBar(1);
            this->m_pCtrlVScroll->SetScrollPosWZ(start2);

            return;
        }
        break;
    case CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK:
        {
            this->FindErase(_PKID_CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK);

            MSG_CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK* msg =
                (MSG_CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK *)pMsg;

            this->m_bSearchedRoom = true;

            this->vCurrentBattleZoneListItems.clear();
            this->m_queriedBattleZone.iType = -1;

            this->m_searchedBattleZone.iType = eBATTLE_ZONE_LIST_ITEM_HUNTING;
            memcpy ((HUNTING_ROOMINFO *)&this->m_searchedBattleZone.m_HuntingRoomInfo,
                    (HUNTING_ROOMINFO *)&msg->m_RoomInfo,
                    sizeof (HUNTING_ROOMINFO));

            TCHAR	szMessage[INTERFACE_STRING_LENGTH];

            int total = 0;
            int count = 0;

            int start = 0;
            int end = 1;

            this->vCurrentBattleZoneListItems.clear();

            for (int i=start; i<end; ++i)
            {
                this->vCurrentBattleZoneListItems.push_back(m_searchedBattleZone);
                this->m_pCtrlList->InsertItemBack();
                total ++;
            }

            //int start = (this->m_pCtrlVScroll->GetScrollPosWZ()*c_iMAX_ROOM);
            int start2 = this->m_pCtrlVScroll->GetScrollPosWZ();
            this->InitScrollBar(1);
            this->m_pCtrlVScroll->SetScrollPosWZ(start2);

            return;
        }
        break;

	case CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK:
		{
			MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK * msg = 
				(MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK *)pMsg;

            this->FindErase( _PKID_CG_ZONE_PVP_ROOM_FROM_CHARID_ACK );
            this->m_bSearchedRoom = true;

            this->vCurrentBattleZoneListItems.clear();
            this->m_queriedBattleZone.iType = -1;

            this->m_searchedBattleZone.iType = eBATTLE_ZONE_LIST_ITEM_PVP;
            memcpy ((PVP_ROOMINFO *)&this->m_searchedBattleZone.m_PVPRoomInfo,
                    (PVP_ROOMINFO *)&msg->m_RoomInfo,
                    sizeof (PVP_ROOMINFO));

            TCHAR	szMessage[INTERFACE_STRING_LENGTH];

            int total = 0;
            int count = 0;

            int start = 0;
            int end = 1;

            this->vCurrentBattleZoneListItems.clear();

            for (int i=start; i<end; ++i)
            {
                this->vCurrentBattleZoneListItems.push_back(m_searchedBattleZone);
                this->m_pCtrlList->InsertItemBack();
                total ++;
            }

            //int start = (this->m_pCtrlVScroll->GetScrollPosWZ()*c_iMAX_ROOM);
            int start2 = this->m_pCtrlVScroll->GetScrollPosWZ();
            this->InitScrollBar(1);
            this->m_pCtrlVScroll->SetScrollPosWZ(start2);

			return;
		}
		break;

    case CG_ZONE_FIND_ROOM_FROM_CHARID_NAK:
        {
            this->FindErase(_PKID_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK);

            MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK* msg =
                (MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK *)pMsg;

            //msg->m_dwErrorCode
			//	해당 플레이어의 방이 존재하지 않습니다.
			this->GetUIMan()->GetInterfaceString( eST_ZONE_FIND_ROOM_FROM_CHARID_NAK, szMessage, INTERFACE_STRING_LENGTH );
            this->GetUIMan()->Sys_VerifyLock(szMessage, FALSE);
        }
        break;

    default:
        break;
    }

    this->updateRoomLists();

    //if (!this->IsPending())
    //{
    //    this->Notice("배틀존 리스트를 갱신 합니다.");
    //}
}

#endif

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
