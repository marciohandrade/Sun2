#pragma once
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiBaseMan.h"

#include "MapInfoParser.h"
#include "RankingManager.h"

#define MAX_WEEK_RANK		10
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
#define MAX_PREWEEK_RANK    10
#else
#define MAX_PREWEEK_RANK	3
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
#define MAX_FAME_RANK		1
#define MAX_RANK_MEMBER		10
#define MAX_PAGE			4

#define TEMP_STRING_LENGTH	64

#define GENERAL_MON			20
#define LEADER_MON			40

#include <time.h>

struct stMissionRankData
{
    int		iRank;
    int		iPartyNum;
    int		iLevel;
    DWORD	dwTime;    
    TCHAR strName[MAX_CHARNAME_LENGTH + 1];
};

struct stMissionRankMemberData
{
    int				iLevel;    
    TCHAR strGuild[MAX_GUILDNAME_LENGTH + 1];
    TCHAR strName[MAX_CHARNAME_LENGTH + 1];
};

struct stRqstMissionRankInfo
{
    RANKINGNO	no;
    BYTE		page;
    WORD        myranking;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    eRANKING_SYSTEM_TYPE ranking_system_type;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
};

struct stRewardInfo
{
    RANKINGID  ranking_id;
    RANKINGKEY reward_ranking_key;   
};

struct IM_MISSION_RANK_MANAGER
{    
    static const DWORD MissionRankList;
    static const DWORD RankTimer;
    static const DWORD RankEnding;
    static const DWORD RankGamble;

};

// 보상 받을 때에 필요한 정보를 모아 놓는 구조체
struct stRewardSearchInfo
{
    RANKINGID            ranking_id;
    RANKINGNO            mission_number;
    WORD                 mission_string;
    eRANKING_SYSTEM_TYPE ranking_system_type;
};

struct stMemberConfirmedInfo
{
    RANKINGKEY           ranking_key;
    eRANKING_TYPE        pending_ranking_type;
    RANKINGNO            ranking_no;
};

class RewardInfo_Funter
{
public:
    inline BOOL operator()(stRewardInfo lhs, stRewardInfo rhs) const;
};

class uiMissionRankTimer;
class uiMissionRankEnding;
class uiMissionRankList2;
class uiMissionRankGamble;

class uiMissionRankMan2 : public uiBaseMan
{
public:
    uiMissionRankMan2(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();
    virtual void            OnRelease();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void					InitMission();
    void					ClearRankData();
    void					SetReceivedStartMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime);
    void					SetReceivedUpdateMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime);
    void					SetClearTime(DWORD dwClearTime);
    void					DisplayTime();
    void					DisplayEnding(int iRank, int iPartyNum, TCHAR* pMasterName);
    void					CloseEndingDlg();


    void					ShowRankList(int iType);
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    TCHAR*                  GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type, eRANKING_SYSTEM_TYPE ranking_system_type);
    int						GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type, eRANKING_SYSTEM_TYPE ranking_system_type);
#else
    TCHAR*					GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);
    int						GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

    void					ShowRankGamble();

    void					CloseMissionRunningDlg();
    void					CloseMissionRankingDlg();

    void					GetTimeString(DWORD dwTime, util::_tstring &strTime);

    void					SetRewardRank(int iRank) { m_dwMyRankForReward = iRank; }
    void					SetReceiveData(BYTE type, RANKINGNO no, BYTE page);	// 서버로부터 받아온 데이터를 셋팅한다

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    bool					IsRankData(RANKINGNO no, BYTE page, eRANKING_SYSTEM_TYPE ranking_system_type);
#else
    bool					IsRankData(RANKINGNO no, BYTE page);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
    void					UpdateReceiveData(eRANKING_SYSTEM_TYPE type, RANKINGNO no, BYTE page);	

    void					SetMemberData(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);

    void					CheckEnableMissionRankingRecord();
    bool					IsEnableMissionRanking();

    void					Send_CG_MISSION_RANKING_INFO_SYN(BYTE byPendingRankingType, RANKINGNO PendingMissionNo, eRANKING_SYSTEM_TYPE ranking_system_type, BYTE byPendingPage);
    void					Send_CG_MISSION_RANKING_REWARD_SYN();
    void					Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(RANKINGNO no, int iIndex, eRANKING_TYPE type);
    void                    Send_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN();    

    void					SetFinishDay(DWORD dwSettledDate);

    bool					DecPage();
    bool					IncPage();
    bool					IsMissionRanking(RANKINGNO no, int iRankSystemType = 1);

public:
    bool                    get_Is_wait_receive_packet_() { return Is_wait_receive_packet_; }
    void                    set_Is_wait_receive_packet_(const bool& Is_wait_receive_packet) { Is_wait_receive_packet_ = Is_wait_receive_packet; }
    void                    fame_data_and_weeks_and_member_data_clar();

private:					
    bool					Is_wait_receive_packet_;
    void					_processErrorCode(DWORD dwErrorCode, eOUTPUT_MSG_TYPE type = eOUTPUT_MSG_CHAT);

    void                    init_ranking_list_vector_(); // 보상 받을 때 검사 할 vector init

    bool                    get_confirmed_member_party_key(const stMemberConfirmedInfo& confirmed_info);

    // 보상 관련
public:
    bool                    get_Is_reward_list_packet_received_() { return Is_reward_list_packet_received_; }
    void                    set_Is_reward_list_packet_received_(const bool& Is_reward_list_packet_received) { Is_reward_list_packet_received_ = Is_reward_list_packet_received; }
    void                    reward_check();
    void                    reward_info_vector_delete_check(RANKINGID id); // 보상을 성공적으로 이루어 졌을 경우 현재의 보상 리스트에서 제거
    MISSIONNO               get_reward_id_to_mission_number(RANKINGID id);


    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );

public:
    RANKINGID               GetSearchRankingId( RANKINGNO no, eRANKING_SYSTEM_TYPE type );
    bool                    get_Is_can_reward_() { return Is_can_reward_; }

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiMissionRankTimer*				ui_rank_timer_ptr_;
    uiMissionRankEnding*			ui_rank_ending_ptr_;
    uiMissionRankList2*				ui_rank_list_ptr_;    

    uiMissionRankGamble*			ui_rank_gamble_ptr_;    

    DWORD							settle_date_dword_;    

    RANKINGNO						current_mission_number_;				// 현재 리스트를 보여주고 있는 미션번호
    eRANKING_SYSTEM_TYPE            current_mission_system_rank_type_;       // 현재 리스트를 보여주고 있는 랭킹타입    


    bool							Is_enable_mission_ranking_;	// 미션랭킹 적용가능한가    

    MISSION_RANK_MEMBER_INFO		mission_party_info_array_[MAX_PARTYMEMBER_NUM];

public:
    DWORD							m_dwMissionTime;    
    DWORD							m_dwStartMissionTime; // 미션시작을 받은시간 --;
    
    DWORD							m_dwUpdateMissionTime;
    
    DWORD							m_dwMissionClearTime;	//서버로 부터 받은 미션 종료시간(최종 미션 종료시간)
    

    DWORD							m_dwResultMissionClear;	//몬스터 kill 수 빼기 전 시간

    DWORD							m_dwTotalMonsterDecTimeSecond;	//총 몬스터 단축 시간(초)
    DWORD							m_dwGeneralMonsterDecSecond;	//일반 몬스터 단축 시간(초)
    DWORD							m_dwLeaderMonsterDecSecond;	//리더 단축 시간(초)

    DWORD							m_dwTotalMonsterKillNum;
    DWORD							m_dwMaxMonsterKillNum;

    DWORD							m_dwMonsterKillNum_General;
    DWORD							m_dwMonsterKillNum_Leader;

    bool							m_bMissionCleared;
    DWORD							m_dwMissionBestTime;	// 최고기록
    DWORD							m_dwElapsedTime;

    int								m_iMyRank;
    int								m_iMyPartyNum;
    util::_tstring					m_strMyPartyMasterName;

    DWORD							m_dwMyRankForReward;		// 보상 받을 수 있는 등수인가    

    stMissionRankData				my_ranking_view_data_;						    // 나의 랭킹 순위
    stMissionRankData				pre_week_rank_view_data_[MAX_PREWEEK_RANK];	// 지난주간 순위 목록
    stMissionRankData				current_week_rank_view_data_[MAX_WEEK_RANK];		// 이번주간 순위 목록
    stMissionRankMemberData			member_view_data_[MAX_RANK_MEMBER];		// 구성원 정보

    util::_tstring					finish_day_string_;    

    BYTE							m_PendingRankingType;			//< eRANKING_TYPE 참조
    eRANKING_SYSTEM_TYPE            pending_ranking_system_type_;
    RANKINGNO						m_PendingMissionNo;
    RANKINGKEY						m_PendingRankingKey;
    BYTE							m_PendingPage;

    eRANKING_TYPE					m_RqstRankingType;			//< eRANKING_TYPE 참조
    RANKINGNO						m_RqstMissionNo;

    BYTE							m_CurRankPage;				// 현재 리스트를 보여주고 있는 미션의 페이지(주간순위)

    std::vector<stRqstMissionRankInfo>			vecRqstMission;
    BOOL							m_bRecvStartTimer;
    std::list<stMemberConfirmedInfo> confirmed_member_party_confirmed_list_; // 한번이라도 파티멤버를 확인 했을 경우에 등록 해 준다.
private:
    std::vector<stRewardSearchInfo> ranking_reward_vector_; // 랭킹에 포함 될 모든 것들을 저장시켜 놓는다. 나중에 보상을 받을 때 필요
    // 보상이 있을 경우 true
    bool                            Is_can_reward_;
    bool                            Is_reward_list_packet_received_;
    std::vector<stRewardInfo>       reward_info_vector_; // 보상 받을 수 있는 구조체를 모아 놓은 vector
};
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
