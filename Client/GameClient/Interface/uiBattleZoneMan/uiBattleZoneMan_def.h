#ifndef     __UIDEF_MANAGER_BATTLEZONE_H__
#define     __UIDEF_MANAGER_BATTLEZONE_H__

struct IM_BATTLEZONE_MANAGER
{
	static const DWORD BATTLEZONE_READY_PVP_CFG;
	static const DWORD BATTLEZONE_LIST;
	static const DWORD BATTLEZONE_CREATE;
	static const DWORD BATTLEZONE_PASSWORD;
	static const DWORD BATTLEZONE_READY_MISSION_CFG;
	static const DWORD BATTLEZONE_READY_CHUNTING_CFG;
	static const DWORD BATTLEZONE_CHUNTING_WIN;
	static const DWORD BATTLEZONE_CHUNTING_SCORE_BOARD;
	static const DWORD BATTLEZONE_CHUNTING_DOING;
    static const DWORD BATTLEZONE_CREATE_EP2;
};

//  Packet I/O Boolean
//
enum
{
    ePKBOOL_BATTLEZONE_BASE = 0,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_FIND_ROOM_FROM_CHARID_SYN,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN,
    ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTING_SYN,
    ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN,
    ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN,
    ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN,
	ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN,

	PACKET_STATUS_NON,
	PACKET_STATUS_SEND_BAN,				// 강퇴 패킷
	PACKET_STATUS_SEND_LEAVE,			// 나가기 패킷
	PACKET_STATUS_SEND_READY,			// 준비 패킷
	PACKET_STATUS_SEND_CREATE,			// 미션/헌팅 만들기
	PACKET_STATUS_SEND_PLAYER_NUM,		// 유저 인원수 체크
	PACKET_STATUS_SEND_READY_CANCLE,	// 준비 취소 패킷
	PACKET_STATUS_SEND_CHANGE_TEAM,		// 팀 교체

    ePKBOOL_BATTLEZONE_MAX,
};

enum
{
    eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY	=	0,	//	미션 조인

    eBATTLE_ZONE_LIST_ITEM_PVP				=	3,	//	pvp 조인
    eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY	=	4,	//	경쟁헌팅 조인

    eBATTLE_ZONE_LIST_ITEM_MAX,
};


#pragma pack(push,1)

class cBattleZoneListItem
{
public:
    cBattleZoneListItem() {iType = -1;}

    int iType;

    MISSION_ROOMINFO    m_MissionLobbyRoomInfo;
    HUNTING_ROOMINFO    m_HuntingLobbyRoomInfo;
    HUNTING_ROOMINFO    m_HuntingRoomInfo;
    PVP_ROOMINFO		m_PVPRoomInfo;
    CHUNTING_ROOMINFO   m_CHuntingLobbyRoomInfo;
    CHUNTING_ROOMINFO   m_CHuntingRoomInfo;

    cBattleZoneListItem& operator=(const cBattleZoneListItem& rhs);
    cBattleZoneListItem& operator=(cBattleZoneListItem* prhs);
    bool operator==(const cBattleZoneListItem& rhs);
    bool operator==(KEYTYPE key);

    void setM (const MISSION_ROOMINFO& rhs);
    void setP (const PVP_ROOMINFO& rhs);
    void setCH (const CHUNTING_ROOMINFO& rhs, int type);
};

//------------------------------------------------------------------------------
/**
*/
inline
cBattleZoneListItem&
cBattleZoneListItem::operator=(const cBattleZoneListItem& rhs)
{
    this->iType = rhs.iType;

    switch (this->iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            memcpy ((MISSION_ROOMINFO *)&this->m_MissionLobbyRoomInfo,
                    (MISSION_ROOMINFO *)&rhs.m_MissionLobbyRoomInfo,
                    sizeof (MISSION_ROOMINFO));
        }
        break;

  
    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            memcpy ((PVP_ROOMINFO *)&this->m_PVPRoomInfo,
                    (PVP_ROOMINFO *)&rhs.m_PVPRoomInfo,
                    sizeof (PVP_ROOMINFO));
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            memcpy ((CHUNTING_ROOMINFO *)&this->m_CHuntingLobbyRoomInfo,
                    (CHUNTING_ROOMINFO *)&rhs.m_CHuntingLobbyRoomInfo,
                    sizeof (CHUNTING_ROOMINFO));
        }
        break;

	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
cBattleZoneListItem&
cBattleZoneListItem::operator=(cBattleZoneListItem* prhs)
{
    assert (prhs);

    this->iType = prhs->iType;

    switch (this->iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            memcpy ((MISSION_ROOMINFO *)&this->m_MissionLobbyRoomInfo,
                    (MISSION_ROOMINFO *)&prhs->m_MissionLobbyRoomInfo,
                    sizeof (MISSION_ROOMINFO));
        }
        break;

    
    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            memcpy ((PVP_ROOMINFO *)&this->m_PVPRoomInfo,
                    (PVP_ROOMINFO *)&prhs->m_PVPRoomInfo,
                    sizeof (PVP_ROOMINFO));
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            memcpy ((CHUNTING_ROOMINFO *)&this->m_CHuntingLobbyRoomInfo,
                    (CHUNTING_ROOMINFO *)&prhs->m_CHuntingLobbyRoomInfo,
                    sizeof (CHUNTING_ROOMINFO));
        }
        break;

    	
	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
cBattleZoneListItem::setM(const MISSION_ROOMINFO& rhs)
{
    this->iType = eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY;

    memcpy ((MISSION_ROOMINFO *)&this->m_MissionLobbyRoomInfo,
        (MISSION_ROOMINFO *)&rhs,
        sizeof (MISSION_ROOMINFO));
}


//------------------------------------------------------------------------------
/**
*/
inline
void
cBattleZoneListItem::setP(const PVP_ROOMINFO& rhs)
{
    this->iType = eBATTLE_ZONE_LIST_ITEM_PVP;

    memcpy ((PVP_ROOMINFO *)&this->m_PVPRoomInfo,
            (PVP_ROOMINFO *)&rhs,
            sizeof (PVP_ROOMINFO));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
cBattleZoneListItem::setCH(const CHUNTING_ROOMINFO& rhs, int type)
{
    this->iType = eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY;

    memcpy ((CHUNTING_ROOMINFO *)&this->m_CHuntingLobbyRoomInfo,
            (CHUNTING_ROOMINFO *)&rhs,
            sizeof (CHUNTING_ROOMINFO));

}

//------------------------------------------------------------------------------
/**
*/
inline
bool
cBattleZoneListItem::operator==(const cBattleZoneListItem& rhs)
{
    if (this->iType != rhs.iType)
        return false;

    switch (this->iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            if (this->m_MissionLobbyRoomInfo.m_Key != rhs.m_MissionLobbyRoomInfo.m_Key)
                return false;
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            if (this->m_PVPRoomInfo.m_Key != rhs.m_PVPRoomInfo.m_Key)
                return false;
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            if (this->m_CHuntingLobbyRoomInfo.m_Key != rhs.m_CHuntingLobbyRoomInfo.m_Key)
                return false;
        }
        break;

    
	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;

    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
cBattleZoneListItem::operator==(KEYTYPE key)
{
    switch (this->iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            if (this->m_MissionLobbyRoomInfo.m_Key != key)
                return false;
        }
        break;

    
    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            if (this->m_PVPRoomInfo.m_Key != key)
                return false;
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            if (this->m_CHuntingLobbyRoomInfo.m_Key != key)
                return false;
        }
        break;

    
	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;

    }

    return true;
}

#pragma pack(pop)

#endif
