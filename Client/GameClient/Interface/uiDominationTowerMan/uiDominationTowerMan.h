#pragma once
#include "uiBaseMan.h"
#include "MouseHandler.h"


struct IM_DOMINATION_TOWER_MANAGER
{
	static const DWORD DOMINATIONWAR_TOWER_MAIN_DIALOG;
	static const DWORD DOMINATIONWAR_BETTING_LIST_DIALOG;
	static const DWORD DOMINATIONWAR_AREA_DIALOG;
	static const DWORD DOMINATIONWAR_TIMER_DIALOG;
	static const DWORD DOMINATIONWAR_ZONEMARK_DIALOG;
	static const DWORD DOMINATIONWAR_STAMP_DIALOG;

    static const DWORD DOMINATIONWAR_BETTING_LIST;

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    static const DWORD DOMINATIONWAR_MANAGER_DIALOG;
    static const DWORD DOMINATIONWAR_APPLY_BETTING_DIALOG;
    static const DWORD DOMINATIONWAR_APPLY_BETTING_DIALOG2;
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    static const DWORD DOMINATIONWAR_JOIN_LIST1;
    static const DWORD DOMINATIONWAR_JOIN_LIST2;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    static const DWORD DOMINATIONWAR_RESURRECTION_POPUP;
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    static const DWORD DOMINATIONWAR_AREA_OWNER_TUTORIAL_DIALOG;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
};

class uiDominationTowerMainDlg;
class uiDominationTowerBettingListDlg;
class uiDominationTowerManagerDlg;
class uiDominationTowerAreaDlg;
class uiDominationTimer;
class uiDominationZoneMarkDlg;
class uiDominationStamp;
class SCItemSlotContainer;

class uiDominationBettingList;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
class uiDominationJoinList1;
class uiDominationJoinList2;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
class uiDominationTowerApplyBettingDlg;
class uiDominationTowerApplyBettingDlg2;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
class uiDominationResurrection;
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
class uiDominationAreaOwnerTutorial;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

typedef std::map<MAPCODE, DOMINATION_GUILD_INFO>			DOMINATIONGUILDMAP;
typedef std::pair<MAPCODE, DOMINATION_GUILD_INFO>			PAIR_DOMINATIONGUILD;
typedef std::map<MAPCODE, DOMINATION_GUILD_INFO>::iterator  DOMINATIONMAP_ITR;

class uiDominationTowerMan : public uiBaseMan
{
	enum ePacketPending
	{
		ePKBOOL_DOMINATION_BASE = 0,

        ePKBOOL_DOMINATION_NOTICE_SYN,
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
        ePKBOOL_DOMINATION_APPLY_SYN,
		ePKBOOL_DOMINATION_BATTING_HEIM_SYN,
        ePKBOOL_DOMINATION_ACCEP_HEIM_SYN,
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
		ePKBOOL_DOMINATION_MANAGE_SYN,
		ePKBOOL_DOMINATION_SAVE_GATESTAT_SYN,
		ePKBOOL_DOMINATION_MINIMAP_LIST_SYN,
        ePKBOOL_DOMINATION_ADD_DEFFENSEWALL_SYN,
        // _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        ePKBOOL_DOMINATION_MEMBER_JOIN_SYN,
        ePKBOOL_DOMINATION_MEMBER_LEAVE_SYN,
        ePKBOOL_DOMINATION_REFRESH_MEMBERLIST_REQ_SYN,

		ePKBOOL_DOMINATIONLOGIN_MAX,
	};
	const static DWORD TIMEOUT_PACKET_PENDING = 10;

    enum 
    {
        MAX_TOKEN_ITEM_SLOT = 1,
    };


public:
    enum MemberListOrder
    {
        kListOrderNone = 0,
        kListOrderClassGreater,
        kListOrderClassLess,
        kListOrderLevelGreater,
        kListOrderLevelLess,
        kListOrderNameGreater,
        kListOrderNameLess,
    };

	uiDominationTowerMan(InterfaceManager *pUIMan);

public:
    virtual void OnInitialize();
    virtual void OnRelease();
	virtual void Update();
	virtual void InitPKBooleans();
	

	void Clear();

	// 패킷 수신 함수
	virtual void NetworkProc(MSG_BASE* pMsg);	

	// 패킷 전송 함수
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    void SEND_DOMINATION_APPLY_SYN();
    void SEND_DOMINATION_BATTING_HEIM_SYN();
    void SEND_DOMINATION_ACCEP_HEIM_SYN(MONEY BettingHeim);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

	void SEND_DOMINATION_NOTICE_SYN(DWORD dwObjectKey);
	
	void SEND_DOMINATION_MANAGE_SYN();
	void SEND_DOMINATION_SAVE_GATESTAT_SYN(BYTE* pDefenseWall);
	void SEND_DOMINATION_MINIMAP_LIST_SYN();
    void SEND_DOMINATION_ADD_DEFFENSEWALL_SYN();

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    void SEND_DOMINATION_REFRESH_MEMBERLIST_REQ(MAPCODE map_code);
    void SEND_DOMINATION_MEMBER_JOIN_SYN(MAPCODE map_code);
    void SEND_DOMINATION_MEMBER_LEAVE_SYN(MAPCODE map_code, CHARGUID char_guid);

    void SetChallengerGuildInfo(DOMINATION_GUILD_INFO* pGuildInfo);
    DOMINATION_GUILD_INFO* GetChallengerGuildInfo()	{	return &m_ChallengerGuildInfo; }

    MemberListOrder friend_member_list_order() { return friend_member_list_order_; }
    void set_friend_member_list_order(MemberListOrder list_order) { friend_member_list_order_ = list_order;}

    MemberListOrder enemy_member_list_order() { return enemy_member_list_order_; }
    void set_enemy_member_list_order(MemberListOrder list_order) { enemy_member_list_order_ = list_order; }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

	static void SEND_LEAVE_DOMINATION();
	
	void SetDominationGuildInfo(DOMINATION_GUILD_INFO* pGuildInfo);
	DOMINATION_GUILD_INFO* GetDominationGuildInfo()	{	return &m_DominationGuildInfo; }
	
	bool ShowDominationGuildInfo()		{	return m_bShowDominationGuildInfo; }
	DOMINATIONGUILDMAP& GetDominationGuildMap()		{	return m_mapDominatioGuild; }

	MONEY GetOwnHeim()	{	return m_OwnHeim; }
	MONEY GetMostOfferHeim()	{	return m_MostOfferHeim; }
	void SetMostOfferHeim(MONEY Heim)	{	m_MostOfferHeim = Heim; }

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    DWORD GetDominationObject();
    int find_domination_object_index_(const DWORD& object_key);
    void set_domination_object_show(const BOOL& is_show);
    void domination_object_key_relation_clear();
    void set_mondschien_guild_name(TCHAR* mondschien_guild_name);
    TCHAR* get_mondschien_guild_name();
    void ShowDominationAreaOwnerDialog(const BOOL& is_show);
    BOOL get_is_character_enter_() { return is_character_enter_; }
    void set_is_character_enter_(const BOOL& is_character_enter) { is_character_enter_ = is_character_enter; }
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    void set_domination_area_owner_guild_name_tutorial(TCHAR* area_owner_guild_name, 
                                                       const sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO* allience_guild_names, 
                                                       const BYTE& allience_guild_count);
#else
    void set_domination_area_owner_guild_name_tutorial(TCHAR* area_owner_guild_name);
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
#else
	DWORD GetDominationObject()	{	return m_dwObjectKey; }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	void SetDominationObject(DWORD dwObjectKey);
	bool IsShowDominationObject()	{	return m_bShowDominationObject; }
    MAPCODE GetDominationManagedMapCode();
	
	static void RenderGuildMark(CControlWZ* pControl, DOMINATION_GUILD_INFO* pGuildInfo, BYTE byAlpha);
    static void RenderGuildMark(CControlWZ* pControl, DOMINATION_GUILD_INFO* pGuildInfo, BYTE byAlpha, bool bTooltipGuildName, DWORD TooltipParentDialogKey );
    static void HideDominationManagmentDialog();

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    bool IsDefenceGuildAlliance(GUILDGUID target_guid);
    static void ComfirmDominationEnter(bool is_enter);
#else
    static void ComfirmDominationJoin(bool is_join);
    static void ComfirmDominationLeave(bool is_leave);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL


    static bool SortMemberListByClassLess(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);
    static bool SortMemberListByClassGreater(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);
    static bool SortMemberListByLevelLess(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);
    static bool SortMemberListByLevelGreater(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);
    static bool SortMemberListByNameLess(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);
    static bool SortMemberListByNameGreater(const DominationMemberInfo& member_info_ref_1, DominationMemberInfo& member_info_ref2);

    DominationMemberInfo& leave_member_info() { return leave_member_info_; }
    void set_leave_member_info(DominationMemberInfo& member_info) { leave_member_info_ = member_info; }

    static void RefreshZoneMarkInfo();

    DWORD GetContinentMapMode();

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    void ShowDominationResurrectionPopup();
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

private:
    // 패킷 수신 함수
    void RECV_DOMINATION_NOTICE_ACK(MSG_BASE * pMsg);
#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    void RECV_DOMINATION_APPLY_ACK(MSG_BASE * pMsg);
    void RECV_DOMINATION_BATTING_HEIM_ACK(MSG_BASE * pMsg);
    void RECV_DOMINATION_ACCEP_HEIM_ACK(MSG_BASE * pMsg);
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

    void RECV_DOMINATION_MANAGE_ACK(MSG_BASE * pMsg);
    void RECV_DOMINATION_SAVE_GATESTAT_ACK(MSG_BASE * pMsg);
    void RECV_DOMINATION_MINIMAP_LIST_ACK(MSG_BASE * pMsg);
    void RECV_DOMINATION_START_NOTIFY_BRD(MSG_BASE * pMsg);
    void RECV_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK(MSG_BASE* pMsg);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void RECV_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD(MSG_BASE* pMsg);
#else
    void RECV_DOMINATION_MEMBERLIST_NTF(MSG_BASE* pMsg);
    void RECV_DOMINATION_MEMBER_LEAVE_ACK(MSG_BASE* pMsg);
    void RECV_DOMINATION_MEMBER_JOIN_ACK(MSG_BASE* pMsg);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    

	// 에러코드 처리 함수 작성
	void ProcessErrorCode(DWORD dwErrorcode);

private:
	uiDominationTowerMainDlg* m_pMainDialog;
	uiDominationTowerAreaDlg* m_DominationArea;
	uiDominationTimer* m_pDominationTimer;
	uiDominationZoneMarkDlg* m_pDominationZoneMark;
	uiDominationStamp* m_pDominationStamp;

    uiDominationBettingList* m_pDominationBettingList;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    uiDominationTowerBettingListDlg* m_pBettingList;
    uiDominationJoinList1* m_pDominationJoinList1;
    uiDominationJoinList2* m_pDominationJoinList2;
    uiDominationTowerApplyBettingDlg* m_pApplyBetting;
    uiDominationTowerApplyBettingDlg2* m_pApplyBetting2;

    MemberListOrder friend_member_list_order_;
    MemberListOrder enemy_member_list_order_;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else
    uiDominationTowerManagerDlg* m_DominationManager;
#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
    uiDominationResurrection* ui_domination_resurrection_;
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

	DOMINATION_GUILD_INFO m_DominationGuildInfo;
	DOMINATION_GUILD_INFO m_ChallengerGuildInfo;

	MONEY m_OwnHeim;
	MONEY m_MostOfferHeim;

	bool m_bShowDominationGuildInfo;

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    uiDominationAreaOwnerTutorial* ui_domination_owner_tutorial_dialog_;
    // 점령전 오브젝트가 여러개 있을 수 있다.
    std::vector<DWORD> domination_object_key_vector_;
    int domination_object_key_current_index_;
    TCHAR mondschien_guild_name_[MAX_GUILDNAME_LENGTH+1];
    BOOL is_character_enter_;
#else
	DWORD m_dwObjectKey;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	bool m_bShowDominationObject;

	DOMINATIONGUILDMAP m_mapDominatioGuild;
    DominationMemberInfo leave_member_info_;
    
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
private:
    STLX_VECTOR<GUILDGUID> defence_guild_alliance_container_; // 수성 길드의 동맹 리스트
public:
    STLX_VECTOR<GUILDGUID>& defence_guild_alliance_container() { return defence_guild_alliance_container_; }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
    bool is_domination_manage_dummy_packet_;
#endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
};
