//---------------------------------------------------------------------------
// PVPManager
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "pvpmanager.h"
#include "GlobalFunc.h"
#include "Player.h"
#ifdef _GS_GSP_TEAM_PVP_MSG
#include "InterfaceManager.h"
#endif//_GS_GSP_TEAM_PVP_MSG

//----------------------------------------------------------------------------
/**
*/
PVPPlayerInfo::PVPPlayerInfo() 
: 
dwPlayerKey(0),
iTeam(INVALID_TEAM),
KillCount(0),
DeadCount(0),	
Score(0),
bPrevKill(FALSE),
StraightKill(0),
BestStraightKill(0)
{
}

//----------------------------------------------------------------------------
/**
*/
PVPPlayerInfo::~PVPPlayerInfo()
{
}

//----------------------------------------------------------------------------
/**
*/
void PVPPlayerInfo::Init()
{
	dwPlayerKey		= 0;
	iTeam			= INVALID_TEAM;
	KillCount		= 0;
	DeadCount		= 0;	
	Score			= 0;
	bPrevKill		= FALSE;
	StraightKill	= 0;
	BestStraightKill= 0;
}


// 정렬 관련
struct PVPInfoKillCountOps
{ 
	// 킬수로 내림차순
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->KillCount > b->KillCount;
	}

};

struct PVPInfoStraightKillOps 
{ 
	// 킬수로 내림차순
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->StraightKill > b->StraightKill;
	}
};

struct PVPInfoDeadKillOps
{
	// 죽은 수로 내림차순
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->DeadCount > b->DeadCount;
	}
};

struct PVPInfoBestStraightKillOps
{
	// 죽은 수로 내림차순
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->BestStraightKill > b->BestStraightKill;
	}
};

PVPManager::PVPManager(void)
{
}

PVPManager::~PVPManager(void)
{
	Release();
}


//----------------------------------------------------------------------------
/**
*/
void			
PVPManager::Init()
{
	RemoveAll();
	this->m_bTeamPlay = FALSE;
}

//----------------------------------------------------------------------------
/**
*/
void			
PVPManager::Release()
{
	DeleteAllUser();
	RemoveAll();
	this->m_bTeamPlay = FALSE;
}
//----------------------------------------------------------------------------
/**
*/
void			
PVPManager::Add( DWORD dwPlayerKey )
{
	PVPPlayerInfo * pPVPInfo = new PVPPlayerInfo;
	assert(pPVPInfo);
	if ( !pPVPInfo )
		return;

	PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.find( dwPlayerKey );
	if ( itr != this->m_PVP_PlayerInfoMap.end() )
	{
		return;
	}

	pPVPInfo->dwPlayerKey = dwPlayerKey;
	
	this->m_PVP_PlayerInfoMap.insert(PVP_PLAYER_INFO_PAIR(dwPlayerKey, pPVPInfo));
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
PVPManager::Remove( DWORD dwPlayerKey )
{
	PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.find( dwPlayerKey );
	if ( this->m_PVP_PlayerInfoMap.end() == itr )
	{
		return FALSE;
	}

	SAFE_DELETE( itr->second );

	m_PVP_PlayerInfoMap.erase(itr);

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
void			
PVPManager::RemoveAll()
{

	PVP_PLAYER_INFO_ITR itr = m_PVP_PlayerInfoMap.begin();
	while ( itr != m_PVP_PlayerInfoMap.end() )
	{
		SAFE_DELETE( itr->second );
		itr++;
	}
		
	this->m_PVP_PlayerInfoMap.clear();

	for ( int i = 0; i < MAX_TEAM; ++i)
	{
		this->m_PVP_PlayerIfoVec[i].clear();
	}
}

//----------------------------------------------------------------------------
/**
*/
void					
PVPManager::SetTeamPlay(BOOL bFlag)
{
	this->m_bTeamPlay = bFlag;
}

//----------------------------------------------------------------------------
/**
*/
BOOL					
PVPManager::GetTeamPlay()
{
	return this->m_bTeamPlay;
}


#ifdef _GS_GSP_TEAM_PVP_MSG
//----------------------------------------------------------------------------
/**
*/
VOID PVPManager::ShowWinTeamMessage()
{
    //승리팀만 찍는다.
    if(!GetTeamPlay())
    {
        return;
    }
    
    DWORD dwResultInfo[MAX_TEAM][2]={0,0,0,0};
    
    for(int i=0;i<MAX_TEAM;++i)
    {
        //0은 청팀 1은 홍팀
        PVP_PLAYER_INFO_VEC_ITR itr = m_PVP_PlayerIfoVec[i].begin();
        while(itr!=m_PVP_PlayerIfoVec[i].end() )
        {
            PVPPlayerInfo *pPVPInfo = *itr;
            if(pPVPInfo)
            {
                dwResultInfo[i][0] +=pPVPInfo->DeadCount;
                dwResultInfo[i][1] +=pPVPInfo->KillCount;
            }
            ++itr;
        }
    }
    
    TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0};
    TCHAR	szTemp[INTERFACE_STRING_LENGTH],szTeamName[INTERFACE_STRING_LENGTH];

    //9055: %d킬 %d데쓰로 %s팀이 승리하였습니다.
    g_InterfaceManager.GetInterfaceString( 9055, szTemp, INTERFACE_STRING_LENGTH );
    
    //메시지 출력구문(어느쪽킬이많은지체크)
    if(dwResultInfo[0][1]>dwResultInfo[1][1] )
    {
        //청팀(left)
        g_InterfaceManager.GetInterfaceString( eST_BLUE_TEAM, szTeamName);
        Sprintf( szMessage, szTemp,dwResultInfo[0][1],dwResultInfo[0][0],szTeamName);
    }
    else
    {
        //홍팀(Right)
        g_InterfaceManager.GetInterfaceString( eST_RED_TEAM, szTeamName);
        Sprintf( szMessage, szTemp,dwResultInfo[1][1],dwResultInfo[1][0],szTeamName);
    }

    GlobalFunc::default_OutputMsg( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage);
    
}
#endif//_GS_GSP_TEAM_PVP_MSG
//----------------------------------------------------------------------------
/**
*/
BOOL  			
PVPManager::GetPVPInfo( DWORD dwPlayerKey, PVPPlayerInfo * OUT pPVPPlayerInfo)
{
	assert(pPVPPlayerInfo);
	if (NULL == pPVPPlayerInfo)
	{
		return FALSE;
	}

	PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.find( dwPlayerKey );
	if ( this->m_PVP_PlayerInfoMap.end() == itr )
	{
		return FALSE;
	}

	PVPPlayerInfo * pOrgInfo = itr->second;
	memcpy( pPVPPlayerInfo, pOrgInfo, sizeof( PVPPlayerInfo ) );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
PVPManager::ModifyPVPInfo( DWORD dwPlayerKey, PVPPlayerInfo * IN pPVPPlayerInfo )
{
	assert(pPVPPlayerInfo);
	if (NULL == pPVPPlayerInfo)
	{
		return FALSE;
	}

	PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.find( dwPlayerKey );
	if ( this->m_PVP_PlayerInfoMap.end() == itr )
	{
		return FALSE;
	}

	PVPPlayerInfo * pOrgInfo = itr->second;
    
	if (pPVPPlayerInfo->KillCount > 0 )
	{
		// 킬수를 올렸다.
		if ( pOrgInfo->bPrevKill )
		{
			// 연승 킬수를 올리자
			pOrgInfo->KillCount += pPVPPlayerInfo->KillCount;
			pOrgInfo->StraightKill++;

			if ( pOrgInfo->StraightKill > pOrgInfo->BestStraightKill )
			{
				// 최고 연승 update
				pOrgInfo->BestStraightKill = pOrgInfo->StraightKill;
			}
		}
		else
		{
			// 연승 킬수 초기화
			pOrgInfo->KillCount += pPVPPlayerInfo->KillCount;
			pOrgInfo->StraightKill = 1;
			pOrgInfo->bPrevKill = TRUE;

			if ( pOrgInfo->StraightKill > pOrgInfo->BestStraightKill )
			{
				// 최고 연승 update
				pOrgInfo->BestStraightKill = pOrgInfo->StraightKill;
			}
		}
	}
	
	if (pPVPPlayerInfo->DeadCount > 0)
	{
		// 죽은 횟수 Count

		pOrgInfo->bPrevKill = FALSE;
		pOrgInfo->DeadCount += pPVPPlayerInfo->DeadCount;
	}

	if (pPVPPlayerInfo->iTeam != INVALID_TEAM)
	{
		pOrgInfo->iTeam = pPVPPlayerInfo->iTeam;
	}

	if (pPVPPlayerInfo->Score > 0)
	{
		pOrgInfo->Score = pPVPPlayerInfo->Score;
	}

	return TRUE;
}


VOID			
PVPManager::SortPVPInfo(ePVP_MATCH_TYPE eMatchType, ePVPINFO_SORT_TYPE eSortType)
{
	// clear
	int i = 0;
	for ( i = 0; i < MAX_TEAM; ++i )
	{
		this->m_PVP_PlayerIfoVec[i].clear();	// i team
	}

	switch (eMatchType) 
	{
	default:
	case ePVP_MATCH_PRIVATE:
		{
			// 개인전

			PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.begin();

			// 일단 등록
			while ( itr != m_PVP_PlayerInfoMap.end() )
			{
				this->m_PVP_PlayerIfoVec[0].push_back( (PVPPlayerInfo *)itr->second );

				itr++;
			};
		}
		break;

	case ePVP_MATCH_TEAM:
		{
			// 팀전
			PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.begin();

			// register
			while ( itr != m_PVP_PlayerInfoMap.end() )
			{
				PVPPlayerInfo * pPVPInfo = (PVPPlayerInfo *)itr->second;
				assert(pPVPInfo);
				assert( pPVPInfo->iTeam > 0 );
				
				int iTeamIdx = (pPVPInfo->iTeam - 1) % MAX_TEAM;		// 팀이 2개 라고 픽스된 상태이다.

				this->m_PVP_PlayerIfoVec[iTeamIdx].push_back( (PVPPlayerInfo *)itr->second );

				itr++;
			}

			// process

			for ( i = 0; i < MAX_TEAM; ++i )
			{
				sortPVPInfo(eSortType, this->m_PVP_PlayerIfoVec[i]);
			}
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
PVP_PLAYER_INFO_VEC & 
PVPManager::GetTeamPVPInfoVec(int iTeam)
{
	return this->m_PVP_PlayerIfoVec[iTeam];
}

//----------------------------------------------------------------------------
/**
*/
PVP_PLAYER_INFO_ITR 
PVPManager::GetBegin()
{
	return this->m_PVP_PlayerInfoMap.begin();
}

//----------------------------------------------------------------------------
/**
*/
PVP_PLAYER_INFO_ITR 
PVPManager::GetEnd()
{
	return this->m_PVP_PlayerInfoMap.end();
}

//----------------------------------------------------------------------------
/**
*/
VOID					
PVPManager::sortPVPInfo(ePVPINFO_SORT_TYPE eSortType, PVP_PLAYER_INFO_VEC & rSortVec)
{
	// 
	switch (eSortType) 
	{
	case ePVPINFO_SORT_STRAIGHT_KILL:
		{
			sort( 
				rSortVec.begin(), 
				rSortVec.end(), 
				PVPInfoStraightKillOps()
				);
		}
		break;

	case ePVPINFO_SORT_BEST_STRAIGHT_KILL:
		{
			sort( 
				rSortVec.begin(), 
				rSortVec.end(), 
				PVPInfoBestStraightKillOps()
				);
		}
		break;

	case ePVPINFO_SORT_DEAD_COUNT:
		{
			sort( 
				rSortVec.begin(), 
				rSortVec.end(), 
				PVPInfoDeadKillOps()
				);
		}
		break;

	default:
	case ePVPINFO_SORT_KILL_COUNT:
		{
		sort( 
			rSortVec.begin(), 
			rSortVec.end(), 
			PVPInfoKillCountOps()
			);
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**	1.GS_BUG_FIX: 유저죽었을때 값을 삽입해준다.
				  1. 키값존재만으로는 시작시입장했는지를 판단학고
				  2. second로는 죽어서 막부활했는지상태를 체크한다.			
*/
BOOL
PVPManager::CheckLeaveUser( DWORD dwPlayerKey )
{
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);

	if( itr != m_PVP_User_Map.end())
	{
		BYTE *pUser = itr->second;

		//예외상황:A가멀리떨어졌는고 B가그상태에서 부활을 하고 A가 다시 들어온다면
		//		   그땐 무적처리를 해주어야한다. 고로 값을 확인해야함				  	
		return (*pUser ? (TRUE):(FALSE));
	}
	else
	{
		return TRUE;//처음들어온녀석이다.
	}
}

//----------------------------------------------------------------------------
/**	2. GS_BUG_FIX: 모든유저 무적상태 풀어줌
*/
void					
PVPManager::SetAllUserState( BOOL bStatus)
{

	PVP_USER_MAP_ITR itr  = m_PVP_User_Map.begin();

	while(itr!=m_PVP_User_Map.end())
	{
		BYTE *pUser = itr->second;
		 *pUser = bStatus;
		itr++;
	}
}
//----------------------------------------------------------------------------
/**	2. GS_BUG_FIX: 유저죽었을때 값을 삽입해준다.
*/
void
PVPManager::SetUserState( DWORD dwPlayerKey,BOOL bDead)
{
	//처음들어왔으면 삽입
	InsertLeaveUser( dwPlayerKey );
	
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);
	
	if( itr != m_PVP_User_Map.end())
	{
		BYTE *pUser = itr->second;
		*pUser = bDead;
	}
}
//----------------------------------------------------------------------------
/**	3.GS_BUG_FIX: 유저가 떠날때 등록해준다.
*/
void
PVPManager::InsertLeaveUser( DWORD dwPlayerKey )
{
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);
	
	//한번만 삽입한다 삽입하면 게임이 끝나야 제거
	if( itr != m_PVP_User_Map.end()) return;
	
	//여긴 처음들어온녀석이다 삽입하자
	BYTE *pUser = new BYTE;
	*pUser = FALSE;
	this->m_PVP_User_Map.insert(PVP_USER_MAP_PAIR(dwPlayerKey, pUser));
	
}
//----------------------------------------------------------------------------
/**	3.GS_BUG_FIX: 유저가 떠날때 등록해준다.
*/
void
PVPManager::DeleteAllUser()
{
	
	PVP_USER_MAP_ITR itr  = m_PVP_User_Map.begin();

	while(itr!=m_PVP_User_Map.end())
	{
		SAFE_DELETE(itr->second);
		itr++;
	}

	m_PVP_User_Map.clear();
}