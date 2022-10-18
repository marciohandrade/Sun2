#pragma once
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

//------------------------------------------------------------------------------
//
//    �۾���: ������
//    uiMissionRankMan class
//------------------------------------------------------------------------------

#include "uiBaseMan.h"

#include "MapInfoParser.h"
#include "RankingManager.h"

#define MAX_WEEK_RANK		10
#define MAX_PREWEEK_RANK	3
#define MAX_FAME_RANK		1
#define MAX_RANK_MEMBER		10
#define MAX_PAGE			4

#define TEMP_STRING_LENGTH	64

#define GENERAL_MON			1
#define LEADER_MON			2

#include <time.h>

TYPEDEF_MAP(MISSION_GROUP, CODETYPE, MapGroup * );


typedef std::multimap<DWORD, sMAPINFO*>		EVENTMAP_MULTIMAP;
typedef std::pair<DWORD, sMAPINFO*>			EVENTMAP_PAIR;
typedef EVENTMAP_MULTIMAP::iterator			EVENTMAP_ITER;



struct stMissionRankData
{
	int		iRank;
	int		iPartyNum;
	int		iLevel;
	DWORD	dwTime;
	//util::_tstring	strName;
    TCHAR strName[MAX_CHARNAME_LENGTH + 1];
};

struct stMissionRankMemberData
{
	int				iLevel;
	//util::_tstring	strGuild;
	//util::_tstring	strName;
    TCHAR strGuild[MAX_GUILDNAME_LENGTH + 1];
    TCHAR strName[MAX_CHARNAME_LENGTH + 1];
};

struct stRqstMissionRankInfo
{
	RANKINGNO	no;
	BYTE		page;
};

struct stEventMapInfo
{
	DWORD	dwCategory;
	int		iGroup;
};

struct IM_MISSION_RANK_MANAGER
{
	static const DWORD RankMenu;
	static const DWORD MissionRankList;
	static const DWORD PointRankList;
	static const DWORD RankTimer;
	static const DWORD RankEnding;
	static const DWORD RankGamble;
	
};

class uiMissionRankTimer;
class uiMissionRankEnding;
class uiMissionRankList;
class uiMissionRankGamble;
class uiRankMenu;

class uiMissionRankMan : public uiBaseMan
{
public:
    uiMissionRankMan(InterfaceManager *pUIMan);

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
	void					InitMissionGroup();

	void					InitMission();
	void					ClearRankData();
	void					SetReceivedStartMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime);
	void					SetReceivedUpdateMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime);
	void					SetClearTime(DWORD dwClearTime);
	void					DisplayTime();
	void					DisplayEnding(int iRank, int iPartyNum, TCHAR* pMasterName);
	void					CloseEndingDlg();


	void					ShowRankList(int iType);
	TCHAR*					GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);
	int						GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);

	void					ShowRankGamble();

	void					CloseMissionRunningDlg();
	void					CloseMissionRankingDlg();

	void					GetTimeString(DWORD dwTime, util::_tstring &strTime);

	void					SetRewardRank(int iRank) { m_dwMyRankForReward = iRank; }
	void					SetReceiveData(BYTE type, RANKINGNO no, BYTE page);	// �����κ��� �޾ƿ� �����͸� �����Ѵ�
	
	bool					IsRankData(RANKINGNO no, BYTE page);
	void					UpdateReceiveData(RANKINGNO no, BYTE page);	

	void					SetMemberData(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type);
	bool					IsEnableReward();					

	void					CheckEnableMissionRankingRecord();
	bool					IsEnableMissionRanking();

	void					Send_CG_MISSION_RANKING_INFO_SYN(BYTE byPendingRankingType, RANKINGNO PendingMissionNo, BYTE byPendingPage);
	void					Send_CG_MISSION_RANKING_REWARD_SYN();
	void					Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(RANKINGNO no, int iIndex, eRANKING_TYPE type);

	RANKINGKEY				GetRewardRankKey()		{ return m_RewardRankKey;	}
	RANKINGNO				GetRewardMissionNo()	{ return m_RewardMissionNo;	}

	void					SetFinishDay(DWORD dwSettledDate);

	bool					DecPage();
	bool					IncPage();
	bool					IsMissionRanking(RANKINGNO no, int iRankSystemType = 1);

	std::vector<stEventMapInfo>		GetEventMapCategory()	{ return m_vecEventMapCategory; }
	EVENTMAP_MULTIMAP		GetEventMap()	{ return m_mapEventMap; }


	int						GetRankingSystemType() { return	m_iRankingSystemType; }
	void					SetRankingSystemType(int iType) { m_iRankingSystemType = iType; }

	void					ShowRankMenu( BOOL bShow );

private:					
	bool					m_bWaitRcvPacket;

	void					_addTestData();

	void					_processErrorCode(DWORD dwErrorCode, eOUTPUT_MSG_TYPE type = eOUTPUT_MSG_CHAT);
	

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
    
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiMissionRankTimer*				m_pUIRankTimer;
	uiMissionRankEnding*			m_pUIRankEnding;
	uiMissionRankList*				m_pUIRankList;
	uiMissionRankList*				m_pUIPointList;
	uiRankMenu *					m_pUIRankMenu;

	uiMissionRankGamble*			m_pUIRankGamble;
	
	DWORD							m_SettledDate;

	RANKINGNO						m_CurMissionNo;				// ���� ����Ʈ�� �����ְ� �ִ� �̼ǹ�ȣ
	

	bool							m_bEnableMissionRanking;	// �̼Ƿ�ŷ ���밡���Ѱ�
	
	MISSION_RANK_MEMBER_INFO		m_MissionPartyInfo[MAX_PARTYMEMBER_NUM];

	EVENTMAP_MULTIMAP				m_mapEventMap;
	std::vector<stEventMapInfo>		m_vecEventMapCategory;


public:
	DWORD							m_dwMissionTime;
	DWORD							m_dwStartMissionTime; // �̼ǽ����� �����ð� --;
	DWORD							m_dwUpdateMissionTime;
	DWORD							m_dwMissionClearTime;	//������ ���� ���� �̼� ����ð�(���� �̼� ����ð�)

	DWORD							m_dwResultMissionClear;	//���� kill �� ���� �� �ð�

	DWORD							m_dwTotalMonsterDecTimeSecond;	//�� ���� ���� �ð�(��)
	DWORD							m_dwGeneralMonsterDecSecond;	//�Ϲ� ���� ���� �ð�(��)
	DWORD							m_dwLeaderMonsterDecSecond;	//���� ���� �ð�(��)

	DWORD							m_dwTotalMonsterKillNum;
	DWORD							m_dwMaxMonsterKillNum;

	DWORD							m_dwMonsterKillNum_General;
	DWORD							m_dwMonsterKillNum_Leader;




	bool							m_bMissionCleared;
	DWORD							m_dwMissionBestTime;	// �ְ���
	DWORD							m_dwElapsedTime;

	int								m_iMyRank;
	int								m_iMyPartyNum;
	util::_tstring					m_strMyPartyMasterName;

	DWORD							m_dwMyRankForReward;		// ���� ���� �� �ִ� ����ΰ�

	RANKINGKEY						m_RewardRankKey;			// ���� ������� ��ŷ key
	RANKINGNO						m_RewardMissionNo;		// ���� ������� �̼� ��ȣ

	stMissionRankData				m_FameData;						// ���� ���� ���
	stMissionRankData				m_PreWeekRank[MAX_PREWEEK_RANK];	// �����ְ� ���� ���
	stMissionRankData				m_CurWeekRank[MAX_WEEK_RANK];		// �̹��ְ� ���� ���
	stMissionRankMemberData			m_MemberData[MAX_RANK_MEMBER];		// ������ ����

	util::_tstring					m_strFinishDay;		

	
	MISSION_GROUP_MAP				m_MapMissionGroup;


	std::vector<MAPCODE>			m_vecMissionCode;

	BYTE							m_PendingRankingType;			//< eRANKING_TYPE ����
	RANKINGNO						m_PendingMissionNo;
	RANKINGKEY						m_PendingRankingKey;
	BYTE							m_PendingPage;

	eRANKING_TYPE					m_RqstRankingType;			//< eRANKING_TYPE ����
	RANKINGNO						m_RqstMissionNo;

	BYTE							m_CurRankPage;				// ���� ����Ʈ�� �����ְ� �ִ� �̼��� ������(�ְ�����)

	std::vector<stRqstMissionRankInfo>			vecRqstMission;

	int								m_iRankingSystemType;
	std::vector<MAPCODE>			m_vecEventMapCode;
	BOOL							m_bRecvStartTimer;

};
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

