#pragma once
#include "uiBaseMan.h"
#include "Nstring.h"


#ifdef _INTERNATIONAL_UI
	const WzID c_wiListFont = StrToWzID("st10");
#else
	const WzID c_wiListFont = StrToWzID("m212");
#endif//_INTERNATIONAL_UI

class uiBattleScoreSingle;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
class uiBattleScoreDouble;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
class uiBattleScoreBlood;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
class uiBattleScoreBattleGround;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
class uiBattleScoreGoldRush;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

struct IM_BATTLESCORE_MANAGER
{
	static const DWORD DLG_BT_SINGLE;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
	static const DWORD DLG_BT_DOUBLE;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static const DWORD DLG_BT_BLOOD;
    static const DWORD DLG_BT_GROUND;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static const DWORD DLG_BT_GOLDRUSH;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
};

enum { TEAM_PLAYER = 20, MAX_PLAYER = 40 };

typedef enum eTeam
{
	eHomeTeam,
	eAwayTeam,
	eTeamCNT,	// MAX�� or NONE TEAM
}E_TEAM;

struct BATTLESCORE_DATA
{
	nString     name;
    BYTE gender;             // ����
	BYTE		byClassCode;
	bool		bConnected;				// ���ӿ���
	LEVELTYPE	wLevel;					// ����
	DWORD   	dwCharGuid;			    // �÷��̾�GUID
    DWORD   	dwTotalDamageToEnemy;   // ������ ���� ������ �ѷ�
    DWORD   	dwTotalDamageByEnemy;   // ������ ���� ������ �ѷ�
    DWORD   	dwTotalHealToParty;     // �ڽŹ� ��Ƽ���� ����� �� ����
    DWORD   	dwMaxDamageToEnemy;     // ������ ���� �ִ� ������
    WORD    	wKillCount;             // ���� Ƚ��
    WORD    	wDeadCount;             // ���� Ƚ��
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BYTE strongpoint_attack;        // ���� ���� ����
    BYTE strongpoint_defense;       // ���� ���� ���
    WORD series_kill;               // ���� ���� ų��
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    WORD        wAssistCount;           // ��ý�Ʈ Ƚ��
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    DWORD        gold_point_;        // ��� ������
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
};

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
struct GOLDRUSH_DATA
{
    nString    name_;
    BYTE       gender_;
    BYTE       byClassCode_;
    DWORD      character_guid_;           // �÷��̾�GUID
    bool       is_connected_;             // ���ӿ���
    LEVELTYPE  level_byte_;               // ����
    WORD       kill_count_;               // ���� Ƚ��
    WORD       dead_count_;               // ���� Ƚ��
    DWORD      my_gold_;                  // �ڽ��� ��� ������
    DWORD      total_demage_to_enemy_;    // ������ ���� ������ �ѷ�
    DWORD      total_heal_to_party_;      // �ڽ� �� ��Ƽ���� ����� �� ����
};
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

enum eBTS_SORT
{
	eBTSSORT_RANK = 0,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    eBTSSORT_CLASS,
    eBTSSORT_NAME,
    eBTSSORT_LV,
#else
	eBTSSORT_NAME,
	eBTSSORT_LV,
	eBTSSORT_CLASS,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	eBTSSORT_MAXDMG,
	eBTSSORT_GIVEDMG,
	eBTSSORT_GETDMG,
	eBTSSORT_HEAL,
	eBTSSORT_KILL,
	eBTSSORT_DIE,
	eBTSSORT_MAX,
};

class uiBattleScoreMan : public uiBaseMan
{
public:
	enum eBTSCROE_LIST
	{
		BTSCROE_LIST_RANK = 0,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        BTSCROE_LIST_CLASS,
        BTSCROE_LIST_ID,
        BTSCROE_LIST_LV,
#else
        BTSCROE_LIST_ID,
        BTSCROE_LIST_LV,
        BTSCROE_LIST_CLASS,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		BTSCROE_LIST_MAXDAMAGE,
		BTSCROE_LIST_GIVEDAMAGE,
		BTSCROE_LIST_GETDAMAGE,
		BTSCROE_LIST_HEAL,
		BTSCROE_LIST_KILL,
		BTSCROE_LIST_DIE,
		BTSCROE_LIST_MAX,
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK, // sort_by �ε��� ��
        BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE, // sort_by �ε��� ��
        BTSCROE_LIST_BATTLE_GROUND_SERIES_KILL, // sort_by �ε��� ��
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        BTSCROE_LIST_GOLDRUSH_GOLDPOINT,        // sort_by �ε��� ��
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	};

	enum eBTSCROE_MODE
	{
		eBTS_NONE_MODE,
		eBTS_MISSION,
		eBTS_SSQ,
		eBTS_HUNTING,
		eBTS_PVP_PRIVATE,
		eBTS_PVP_TEAM,
        eBTS_DOMINATION,
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        eBTS_BATTLE_GROUND,
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        eBTS_GOLD_RUSH,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
		eBTS_MODE_MAX
	};
	
	uiBattleScoreMan(InterfaceManager *pUIMan);

public:
	virtual void            OnInitialize();
	virtual void	        NetworkProc( MSG_BASE * pMsg );
	void					InitInfo();
	void					Reset();
	inline void				SetOrderBy(bool bOrderBy){ m_bOrderBy = bOrderBy; }
	inline bool				GetOrderBy(void){ return m_bOrderBy; }
	void					SortbyList();
	inline void				SetSortBy(int bSortBy){ m_iSortBy = bSortBy; }
	inline int				GetSortBy(){ return m_iSortBy; }
	void					SetHeroTeam(int iTeam){ m_iHeroTeam = iTeam; }
	int						GetHeroTeam(){ return m_iHeroTeam; }
	void					SetWinPoint(int iWin, E_TEAM eTeam){ m_iWinPoint[eTeam] = iWin; }
	int						GetWinPoint(E_TEAM eTeam){ return m_iWinPoint[eTeam]; }
	bool					GetIsInit(){ return m_bIsInited; }
	void					SetIsInit(bool bIs){ m_bIsInited = bIs; }
    eBTSCROE_MODE			GetMode(){ return m_eBTS_Mode; }
	void					SetMode(eBTSCROE_MODE eMode){ m_eBTS_Mode = eMode; }
    bool                    toggle_ui_open(){ return toggle_ui_open_; }
    void                    set_toggle_ui_open(bool value){ toggle_ui_open_ = value; }
    bool                    first_open(){ return first_open_; }
    void                    set_first_open(bool value){ first_open_ = value; }
    void					ShowBattleScoreWin();
    bool                    IsBattleScoreWinOpen();
	void					ClearSelection();
	void					ClearMemberData();
	void					ResetHuntingData();
	int						GetPartyMember();
	void					RefreshInfo(EXPTYPE add_experience, MONEY add_money);
	void					DisconnectMember(int index, E_TEAM eTeam);
    void					ConnectMember(int index, E_TEAM eTeam);
    void                    SendPacketBattleResultSYN();
    void                    PartyRefresh();
    int                     FindMemberToName(LPCTSTR name, E_TEAM eTeam);
    int						FindMember(DWORD dwPlayerKey, E_TEAM eTeam);
	void					RemoveMember(int index, E_TEAM eTeam);
	void					RefreshListCtrlData();
    void                    ToggleBattleScoreWindow();

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    void                    RefreshBattleGroundList(); // ���� ����Ʈ ����
    void                    AllMemberScoreArray(nArray<BATTLESCORE_DATA>* all_member_array); //��� �� ��� ����
    void                    AllMemberSortbyList(nArray<BATTLESCORE_DATA>& all_member_array); //��� �� ��� ����
    
    void                    EnableBattleGroundExitButton(); // ���� ������ ��ư Ȱ��ȭ
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    void                    RefreshGoldRushList(); // ��巯�� ����Ʈ ����
    void                    EnableGoldRushExitButton(); // ���� ������ ��ư Ȱ��ȭ
    int                     get_team_gold_point(E_TEAM eTeam);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

public:
    nArray<BATTLESCORE_DATA> m_BTSMemberArray[eTeamCNT];
private:
    uiBattleScoreSingle*	m_pBattleScoreSingle;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
	uiBattleScoreDouble*	m_pBattleScoreDouble;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    uiBattleScoreBlood*		m_pBattleScoreBlood;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    uiBattleScoreBattleGround* battle_score_battle_ground;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    uiBattleScoreGoldRush*  battle_score_gold_rush_ptr_;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	eBTSCROE_MODE			m_eBTS_Mode;
	int						m_iCurTeam01Select;
	int						m_iCurTeam02Select;
	int						m_iSortBy;
	bool					m_bOrderBy;
	int						m_iHeroTeam;
	int						m_iWinPoint[eTeamCNT];
	bool					m_bIsInited;
    bool                    toggle_ui_open_;
    bool                    first_open_;
};

const int c_iListHeight = 20;
const int c_iListWidth[uiBattleScoreMan::BTSCROE_LIST_MAX] =
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    50,	//BTSCROE_LIST_RANK = 0,
    32,	//BTSCROE_LIST_CLASS,
    209,//BTSCROE_LIST_ID,
    61,	//BTSCROE_LIST_LV,
    96,	//BTSCROE_LIST_MAXDAMAGE,
    121,//BTSCROE_LIST_GIVEDAMAGE,
    121,//BTSCROE_LIST_GETDAMAGE,~
    69,	//BTSCROE_LIST_HEAL,
    59,	//BTSCROE_LIST_KILL,
    58	//BTSCROE_LIST_DIE,
#else
	43,	//BTSCROE_LIST_RANK = 0,
	199,//BTSCROE_LIST_ID,
	43,	//BTSCROE_LIST_LV,
	72,	//BTSCROE_LIST_CLASS,
	93,	//BTSCROE_LIST_MAXDAMAGE,
	143,//BTSCROE_LIST_GIVEDAMAGE,
	143,//BTSCROE_LIST_GETDAMAGE,
	68,	//BTSCROE_LIST_HEAL,
	55,	//BTSCROE_LIST_KILL,
	58	//BTSCROE_LIST_DIE,
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};
