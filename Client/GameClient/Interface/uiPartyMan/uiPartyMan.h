#ifndef     __USER_INTERFACE_MANAGER_PARTY_H__
#define     __USER_INTERFACE_MANAGER_PARTY_H__
//------------------------------------------------------------------------------
/**
    @class uiPartyMan

    interface manager for a party

    (C) 2005 ckbang
*/
#include "uiPartyMan_def.h"
#include "uiBaseMan.h"

class uiPartyOption;
class uiPartyLeavePopup;
class uiPartyMember;
class ItemShareSystem;

struct stPendingUnexpectedStatus
{
	//비정상 종료시 파티복귀 에서 필요한 값들은 Pending 해놓을 구조체
	DWORD					m_dwReturnPlayerKey;
	TCHAR					m_szReturnPlayerName[MAX_CHARNAME_LENGTH+1];
};


class uiPartyMan : public uiBaseMan 
{
public:
    uiPartyMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
    virtual void            Process(DWORD dwTick);
	virtual void			OnInitialize();
    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    Reason(BYTE byErrorCode);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiPartyMember*          m_pUIPartyMembers[c_sMAX_PARTY_MEMBER_NUM];
    uiPartyOption*          m_pUIPartyOption;
    uiPartyLeavePopup*      m_pUIPartyLeavePopup;
	ItemShareSystem*		m_pUIItemShareSystem;


    BOOL                    _LoadUI();

public:
    bool                    IsLeader();
    bool                    IsShowBuffInfos();

    void                    uiClear(short idx);
    void                    uiClearAll();
    void                    uiShowAll();

    bool                    Comp_HeroKey(DWORD dwKey);
    void                    Insert_MemberInfo(uicPartyMemberInfoPlus memInfo);
	void                    Remove_MemberInfo(DWORD dwObjKey, BYTE byReason);
    
    void                    RefreshPartyHideInfo(BOOL bClear=FALSE);
    void                    ChangePartyHideInfo(DWORD dwObjKey,BOOL bClear);

    void                    uiPartySort(int sortType=eUIPARTY_SORT_NORMAL);
	void                    PutMsg_PlayerEntered(uicPartyMemberInfoPlus memInfo);
	void                    Migration(DWORD dwObjKey);
	void                    DestroyParty();
    void                    LeaveParty();
	void                    AskJoinParty( const char * szName );
    void                    ResponseJoinParty(char *pszMasterName);

    // uiPartyMan_net.cpp
	void                    InitInfo(MSG_CG_PARTY_CREATE_CMD *pMsg);
    void                    AddInfo(MSG_CG_PARTY_JOIN_BRD* pMsg);


	void					NET_SEND_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK(byte byAnswer);
	void					NET_SEND_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK(bool val);
	void					_callBack_ReturnParty(bool /*val*/) { return;}

	void					DeleteUnExpectedPartyMember(TCHAR * pszCharName);

	// uiPartyMan_info.cpp
    void                    ClearParty();
    bool                    IsParty();
    bool                    IsPartyMember(DWORD dwObjKey);
    DWORD IsPartyMemberForName(const TCHAR* character_name);
    short                   GetPartyNum();
    int GetPartyIndex(TCHAR* character_name);
    int GetPartyIndexByObjectKey(DWORD object_key);
    const uicPartyMemberInfoPlus&   GetParty(short idx);
    const uicPartyMemberInfoPlus*   GetPartyLeader();
    STLX_VECTOR<uicPartyMemberInfoPlus> GetPartyList();
    STLX_VECTOR<uicPartyMemberInfoPlus>& GetPartyListRef() { return m_vParty; }
	void					SetPartyList(const STLX_VECTOR<uicPartyMemberInfoPlus>& partyList);
#ifdef _DH_BATTLEZONE2_
    int                     GetPartyIndexFromObjectkey(DWORD object_key);
    void                    SetBattleZoneReady(int object_key,bool is_ready);
    void                    AllCancleBattleZoneReady();
    void                    CheckOutputZoneType();
#endif //_DH_BATTLEZONE2_

	void					ForcedExpulsion();
	void					ChangeMaster();
	void					ChangeLevel(DWORD dwObjKey, WORD wLevel);
	void					ChangeHP(DWORD dwObjKey, BYTE byHpRatio);
	void					ChangeMP(DWORD dwObjKey, BYTE byMpRatio);
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    void					ChangeSD(DWORD object_key, BYTE sd_ratio);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	void                    ChangeChaoState(DWORD dwObjKey, BYTE byChaoState);
	void					NET_RECV_MSG_CG_PARTY_NAVI_INFO_BRD(MSG_CG_PARTY_NAVI_INFO_BRD* pMsg_NaviInfo);
	PartyNaviInfo*			GetPartyNaviInfo()				{ return m_pPartyNaviInfo; }
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    void                    NET_RECV_MSG_CG_PARTY_DICECAST_RESULT_CMD(MSG_CG_PARTY_DICECAST_RESULT_CMD* dicecast_result);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

	void					DeadPartyMembaer(DWORD dwObjKey);
	void					ResurrectionPartyMember(DWORD dwObjKey);
	
	void					SetOpenPartyJoinWindow(BOOL val) { m_bIsOpenPartyJoinWinodw = val; }

	void					UpdatePartyItemShareSystem();

    void                    SetTargetingMember(int party_member_index);
	uicPartyMemberInfoPlus*	GetPartyMemberInfo(DWORD dwObjKey);
	uicPartyMemberInfoPlus*	GetPartyMemberInfoFromIndex(DWORD dwIndex);

	DWORD					GetMasterKey();
    WORD GetPartyKey(){ return m_PartyKey; }

	PartyNaviInfo*			m_pPartyNaviInfo;


	void					uiSetPartView();
	void 					SetShowFace(bool val);
	void 					SetShowGague(bool val);
	void 					SetShowBuf(bool val){ m_bIsShowBuf = val; }
	bool 					GetShowFace(){ return m_bIsShowFace; }
	bool 					GetShowGague(){ return m_bIsShowGague; }
	bool 					GetShowBuf(){ return m_bIsShowBuf; }

    void                    UpdateStatusIcon();

    void                    Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN();

protected:
    void                    NoticeReasonLeaveParty(DWORD dwObjKey, BYTE byReason);
private:
    bool                    m_bImTheLeader;
    bool                    m_bShowBuffInfos;
	BOOL					m_bIsOpenPartyJoinWinodw;

	bool 					m_bIsShowFace;
	bool 					m_bIsShowGague;
	bool 					m_bIsShowBuf;

    STLX_VECTOR<uicPartyMemberInfoPlus>  m_vParty;

	WORD						m_PartyKey;
	DWORD						m_MasterKey;

	std::deque<stPendingUnexpectedStatus>	m_dequePartyReturn;
	DWORD					m_dwPendingPartyKey;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    bool                    is_opened_return_party_window_;
    bool                    is_disconnected_uesr_kick_;
    char                    kick_player_name_[MAX_CHARNAME_LENGTH+1];
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

};

void _callBack_ReallyCancleReturnPartyMember(bool val);
void _callBack_Recall_PartyMember(bool val);

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiPartyMan::IsLeader()
{
    return this->m_bImTheLeader;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiPartyMan::IsShowBuffInfos()
{
    return this->m_bShowBuffInfos;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
uiPartyMan::GetMasterKey()
{
	return m_MasterKey;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_PARTY_H__
