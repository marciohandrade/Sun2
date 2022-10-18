#ifndef     __USER_INTERFACE_MANAGER_BATTLEZONE_H__
#define     __USER_INTERFACE_MANAGER_BATTLEZONE_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneMan

    interface manager for a battlezone

    (C) 2005 ckbang
*/
#include "uiBattleZoneMan_def.h"
#include "uiBaseMan.h"

#include "BattleRoomEntry.h"
#include "UserSlotSet.h"
#include "UserSlot.h"

struct stSubMission 
{
     BYTE		byIndex;
     int		iRequireMinLV;
};

#define MAX_GUILD_PVP_JOIN_MEMBER	5

enum BattleRoomViewType;
class BattleLobby;
class BattleRoomEntry;

class uiBattleZoneReadyPVPConfig;
class uiBattleZoneCreate;
class uiBattleZoneCreateEp2;
class uiBattleZoneList;
class uiBattleZonePassword;
class uiBattleZoneReadyCHuntingConfig;
class uiBattleZoneReadyMissionConfig;
class uiBattleZoneWinDialog;
class uiBattleZoneCHuntingScoreBoard;
class uiBattleZoneDoing;



class uiBattleZoneMan :
    public uiBaseMan
{
public:
    uiBattleZoneMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void			OnInitialize();
    virtual void            OnRelease();

    void                    InitBattleEntries();
    virtual void            InitPKBooleans();
    virtual void            HandlerPKBooleans();
    void                    HandlerPKB_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:

    void                    ShortCut(BYTE type);
    void                    ShortCut_Mission();
    void                    ShortCut_CHunting();
    void                    ShortCut_PVP();

    void                    SearchID(TCHAR* pSearchID);
    void                    SearchNo(SHORT no);

    void                    ClearZoneListItems();

    const std::vector<cBattleZoneListItem>& GetZoneList(BattleRoomViewType view_type);

    void                    RequestZoneList();
    bool                    RequestJoinZone(KEYTYPE roomKey);

	void					RequestJoinZoneSearchMode(KEYTYPE roomKey);
	void                    RequestJoinZone(cBattleZoneListItem* pZone, util::_tstring pwd = _T(""));
	void					SetWait(bool bWait)		{ m_bWaitInputPassWord = bWait; }
	bool					IsWait()		{ return m_bWaitInputPassWord; }
	void					SetSearchMode(bool bSearch)		{ m_bSearchMode = bSearch; }
	bool					IsSearchMode()					{ return m_bSearchMode; }
	void					ShowPasswordDlg(bool bShow);
	void					RequestJoinPrivateZone(util::_tstring pwd);
	bool                    RequestJoinPrivateZone(KEYTYPE roomKey);

    void                    GameStart();

    // net
	void					NET_SEND_MSG_CG_ZONE_LOBBY_CREATE_SYN();
	byte					GetSuperMasterType() { return m_BattleRoomEntry.m_RoomInfo.m_SpecialType; }
	BASE_ROOMINFO *			GetBaseRoomInfo() { return &m_BattleRoomEntry.m_RoomInfo;}
	BOOL					IsSuperRoomMaster();

	void                    NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_NAK( MSG_BASE * pMsg );

    void                    NET_RECV_MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_SHORTCUT_CHUNTING_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_SHORTCUT_NAK( MSG_BASE * pMsg );

    void                    NET_RECV_MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK( MSG_BASE * pMsg );

    void                    NET_SEND_MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN( BYTE count );
    void                    NET_SEND_MSG_CG_ZONE_LIST_PVPLOBBY_SYN( BYTE count );
    void                    NET_SEND_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN( BYTE count );
    void                    NET_SEND_MSG_CG_ZONE_LIST_CHUNTING_SYN( BYTE count );

    void                    NET_RECV_MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_LIST_PVPLOBBY_ACK( MSG_BASE * pMsg );
    void                    NET_RECV_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK( MSG_BASE * pMsg );

    void                    NET_RECV_MSG_CG_ZONE_LIST_NAK( MSG_BASE * pMsg );

   
    BattleLobby*            GetBattleLobby();
    void                    SetBattleLobby(BattleLobby* pLobby);
    

    void                    ClearCHuntingJoinedFlag();
    bool                    GetCHuntingJoinedFlag();
    void                    ClearHasJoinedFlag();
    bool                    GetHasJoinedFlag();

    // all cfgs
    void                    ClearPendings();

    void                    ModifyMap(int index);

    // mission cfgs
    bool                    IsReadyToModifyMissionCfg();
    BYTE	                GetMinSubMission();
    BYTE                    GetMaxSubMission();

	BYTE					GetSubMissionUp(BYTE byCur);
	BYTE					GetSubMissionDown(BYTE byCur);

    
    // combat hunting cfgs
    void                    ModifyCHuntingMap(int index);
    bool                    IsReadyToModifyCHuntingCfg();
    void                    ModifyCHuntingCfg(WORD                       eField,
                                              eHUNTING_MONSTER_DIFFICULT eDifficult,
                                              eHUNTING_BONUS_TYPE        eBonusType,
                                              eCOMPETITION_RULE_TYPE     eCompetitionRule,
                                              eCOMPETITION_MODE_TYPE     eCompetitionMode);
	
	// pvp cfgs
	void					ModifyPVPCfg(ePVP_RULE_TYPE	eRule,
										ePVP_MODE_TYPE	ePVPMode);

	void					SetRoomSpecialType(int val) { m_RoomSpecialType = val;}
	int						GetRoomSpecialType() {return m_RoomSpecialType;}
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	BOOL					IsSpecialType(eROOM_SPECIAL_TYPE eType) { return (m_RoomSpecialType & eType);}
#endif

	void					SetLobbyMasterPlayerKey( DWORD dwMasterPlayerKey )	{ m_dwMasterPlayerKey = dwMasterPlayerKey;	}
	DWORD					GetLobbyMasterPlayerKey() const						{ return this->m_dwMasterPlayerKey;			}
	bool					IsMaster() const;
	BattleRoomEntry&        GetBattleRoomEntry();
	void					SetBattleRoomEntry(const BattleRoomEntry * IN pBattleRoomEntry);

	void					SetReadyUserSlot(DWORD dwObjectKey, bool bReady);
	void					SetTeamIndexUserSlot(DWORD dwObjectKey, int iTeam);	
	void					SetBlockUserSlot();
	void					SetTeamUserSlot(DWORD dwPlayerKey, int iTeam);
	void					SetTeamUserSlot(PVP_LOBBY_INFO pvpInfo);
	UserSlot*				GetUserSlotByKey(DWORD dwPlayerKey);
	UserSlot*				GetUserSlotByIndex(int iIndex);
	int						GetMaxUserSlot();
	int						GetNumUser();
	int						GetNonBlockUserSlotNum();
	int						GetBlockUserSlotNum();
	void					InitializeUserSlotSet();
	void					ReleaseUserSlotSet();
	void					FlushUserSlotSet();
	void					SetOpenUserSlot(int index);

	void					SetNonBlockSlot(int iNonBlockSlot);
	void					SetDefaultUserSlot();

	BOOL					AddPlayer(UserSlot & EnterUserSlot);
	BOOL					LeavePlayer(DWORD dwPlayerKey);

	VOID					ModifyPlayerNums(int iPlayerNums);

	void					Send_LOBBY_KICKPLAYER_SYN(DWORD dwPlayerKey);


	VOID					ReadyBattle();
	VOID					StartBattle();
	VOID					LeaveLobby();
	VOID					ChangeTeam();

	BOOL					IsReadyForStart();

	BOOL					BanPlayerByIndex(int index);
    BOOL                    BattleZoneListIsVisible();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );

    //--------------------------------------------------------------------------
    // member vars
    //
private:

	uiBattleZoneReadyPVPConfig*			m_pUIBattleZoneReadyPVPConfig;

    uiBattleZoneCreate*					m_pUIBattleZoneCreate;
    uiBattleZoneCreateEp2* ui_battlezone_create_ep2_;
    uiBattleZoneList*					m_pUIBattleZoneList;
    uiBattleZonePassword*				m_pUIBattleZonePassword;
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    uiBattleZoneReadyCHuntingConfig*	m_pUIBattleZoneReadyCHuntingConfig;
    uiBattleZoneReadyMissionConfig*		m_pUIBattleZoneReadyMissionConfig;
	uiBattleZoneWinDialog*				m_pUIBattleZoneWinDialog;
    uiBattleZoneCHuntingScoreBoard*		m_pUIBattleZoneCHuntingScoreBoard;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	uiBattleZoneDoing*					m_pUIBattleZoneDoing;

    BattleLobby *						m_pBattleLobby;

    BYTE								m_byMinSubMission;
    BYTE								m_byMaxSubMission;

    std::vector<cBattleZoneListItem>    m_zoneListItem;
    std::vector<cBattleZoneListItem>    m_renderedZoneListItem;
    cBattleZoneListItem					m_searchedZoneListItem;

    KEYTYPE								m_keyCHuntingJoined;
    bool								m_bCHuntingJoined;

    bool								m_bHasJoined;

    // pending datas
    MAPCODE								m_pendingMapCode;
    WORD								m_pendingMapChangeIndex;
    eHUNTING_MONSTER_DIFFICULT			m_pendingDifficult;
    eHUNTING_BONUS_TYPE					m_pendingBonus;
    eCOMPETITION_RULE_TYPE				m_pendingCompetitionRule;
    eCOMPETITION_MODE_TYPE				m_pendingCompetitionMode;

	bool								m_bWaitInputPassWord;
	cBattleZoneListItem*				m_pPrivateZone;
	bool								m_bSearchMode;

	cBattleZoneListItem					m_friendJoinLobby;

	DWORD								m_dwMasterPlayerKey;
	BattleRoomEntry						m_BattleRoomEntry;

	UserSlotSet							m_UserSlotSet;
	int									m_RoomSpecialType;

public:
	std::vector<stSubMission>			m_vecMissionMapIndex;

	int									m_iPendingPlayerIndex;				// 블럭 / 오픈할 할 유저슬롯 Index
	int									m_iPendingPlayerNumBeforeRecvAck;	// 인원수 제한 설정후 ack 패킷을 받기전에 저장해두는 변수
																// Ack 시 본인은 셋, Brd는 상대들
	ADDITIONAL_ROOMINFO					m_PendingAdditionalInfo;
	int									m_PendingiTeam;
	CODETYPE							m_PendingMapCode;

};


//------------------------------------------------------------------------------
/**
*/
inline
BYTE
uiBattleZoneMan::GetSubMissionDown(BYTE byCur)
{
	int iSize = m_vecMissionMapIndex.size();
	
	if(iSize <= 0)
	{
		return 0;
	}

	int iPos = 0;
	for(int i=0; i<iSize; ++i)
	{
		if(m_vecMissionMapIndex[i].byIndex == byCur)
		{
			iPos = i;
		}
	}

	++iPos;
	if(iPos >= (iSize-1))
	{
		iPos = iSize-1;
	}

	return m_vecMissionMapIndex[iPos].byIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
uiBattleZoneMan::GetSubMissionUp(BYTE byCur)
{
	int iSize = m_vecMissionMapIndex.size();
	if(iSize <= 0)
	{
		return 0;
	}

	int iPos = 0;
	for(int i=0; i<iSize; ++i)
	{
		if(m_vecMissionMapIndex[i].byIndex == byCur)
		{
			iPos = i;
		}
	}

	--iPos;
	if(iPos <= 0)
	{
		iPos = 0;
	}

	return m_vecMissionMapIndex[iPos].byIndex;

}


//------------------------------------------------------------------------------
/**
*/
inline
BYTE
uiBattleZoneMan::GetMinSubMission()
{
	if(m_vecMissionMapIndex.size())
		return m_vecMissionMapIndex[0].byIndex;
	else
		return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
uiBattleZoneMan::GetMaxSubMission()
{
	int iPos = m_vecMissionMapIndex.size() - 1;
	return m_vecMissionMapIndex[iPos].byIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
BattleLobby*
uiBattleZoneMan::GetBattleLobby()
{
    assert (m_pBattleLobby);
    return m_pBattleLobby;
}

//------------------------------------------------------------------------------
/**
*/
inline
BattleRoomEntry&
uiBattleZoneMan::GetBattleRoomEntry()
{
    return m_BattleRoomEntry;
}

//------------------------------------------------------------------------------
/**
*/
inline
void			
uiBattleZoneMan::SetBattleRoomEntry(const BattleRoomEntry * IN pBattleRoomEntry)
{
	if (!pBattleRoomEntry)
	{
		assert(pBattleRoomEntry);
		return;
	}

	memcpy(
		&m_BattleRoomEntry, 
		pBattleRoomEntry, 
		sizeof(BattleRoomEntry) );
}



//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneMan::ClearCHuntingJoinedFlag()
{
    m_bCHuntingJoined = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneMan::GetCHuntingJoinedFlag()
{
    return m_bCHuntingJoined;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBattleZoneMan::ClearHasJoinedFlag()
{
    m_bHasJoined = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneMan::GetHasJoinedFlag()
{
    return m_bHasJoined;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_SYSTEM_H__
