#pragma once

struct PVPPlayerInfo
{	
	PVPPlayerInfo();
	~PVPPlayerInfo();

	void Init();

	DWORD	dwPlayerKey;
	int		iTeam;
	WORD	KillCount;	// ���μ�
	WORD	DeadCount;	// ������ 
	WORD	StraightKill;
	WORD	BestStraightKill;
	DWORD	Score;

	BOOL	bPrevKill;	// ���� �÷���
};


typedef std::map<DWORD, PVPPlayerInfo *>	PVP_PLAYER_INFO_MAP;
typedef PVP_PLAYER_INFO_MAP::iterator		PVP_PLAYER_INFO_ITR;
typedef std::pair<DWORD, PVPPlayerInfo *>		PVP_PLAYER_INFO_PAIR;

typedef std::vector<PVPPlayerInfo *>		PVP_PLAYER_INFO_VEC;
typedef PVP_PLAYER_INFO_VEC::iterator		PVP_PLAYER_INFO_VEC_ITR;

//GS_BUG_FIX:��������Ʈ�����۾��������� PVP_SCENE������ �÷��̾�� �Ÿ��� ����
//			 PLAYER� �����߿� ����� �������Ѵ� �̸� Ŭ�󿡼� ����ó���ϱ����
typedef		stdext::hash_map<DWORD, BYTE*>	PVP_USER_MAP;
typedef		PVP_USER_MAP::iterator			PVP_USER_MAP_ITR;
typedef		std::pair<DWORD, BYTE*>				PVP_USER_MAP_PAIR;



class PVPManager
{
public:
	PVPManager(void);
	~PVPManager(void);

	void					Init();
	void					Release();

	void					Add( DWORD dwPlayerKey );
	BOOL					Remove( DWORD dwPlayerKey );
	void					RemoveAll();

	void					SetTeamPlay(BOOL bFlag);
	BOOL					GetTeamPlay();
	

	BOOL  					GetPVPInfo( DWORD dwPlayerKey, PVPPlayerInfo * OUT pPVPPlayerInfo);
	BOOL					ModifyPVPInfo( DWORD dwPlayerKey, PVPPlayerInfo * IN pPVPPlayerInfo );

	VOID					SortPVPInfo(ePVP_MATCH_TYPE eMatchType, ePVPINFO_SORT_TYPE eSortType);

	PVP_PLAYER_INFO_VEC &	GetTeamPVPInfoVec(int iTeam); 

	PVP_PLAYER_INFO_ITR		GetBegin();
	PVP_PLAYER_INFO_ITR		GetEnd();
    
#ifdef _GS_GSP_TEAM_PVP_MSG
    VOID                    ShowWinTeamMessage();
#endif//_GS_GSP_TEAM_PVP_MSG
    
	// inline funcs
	inline WORD				GetPlayerCount();

	//GS_BUG_FIX: ��������Ʈ�۾���������  PVP����Ű����
	BOOL					CheckLeaveUser( DWORD dwPlayerKey );
	void					SetUserState( DWORD dwPlayerKey,BOOL bDead );
	void					SetAllUserState( BOOL bStatus);
	void					InsertLeaveUser( DWORD dwPlayerKey);
	void					DeleteAllUser();


protected:
	VOID					sortPVPInfo(ePVPINFO_SORT_TYPE eSortType, PVP_PLAYER_INFO_VEC & rSortVec);

private:
	PVP_PLAYER_INFO_MAP			m_PVP_PlayerInfoMap;
	PVP_PLAYER_INFO_VEC			m_PVP_PlayerIfoVec[MAX_TEAM];
	BOOL						m_bTeamPlay;	// ����

	PVP_USER_MAP				m_PVP_User_Map;

};


inline WORD		
PVPManager::GetPlayerCount()
{
	return m_PVP_PlayerInfoMap.size();
}