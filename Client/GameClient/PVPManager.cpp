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


// ���� ����
struct PVPInfoKillCountOps
{ 
	// ų���� ��������
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->KillCount > b->KillCount;
	}

};

struct PVPInfoStraightKillOps 
{ 
	// ų���� ��������
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->StraightKill > b->StraightKill;
	}
};

struct PVPInfoDeadKillOps
{
	// ���� ���� ��������
	bool operator()( const PVPPlayerInfo * a, const PVPPlayerInfo * b )
	{
		return a->DeadCount > b->DeadCount;
	}
};

struct PVPInfoBestStraightKillOps
{
	// ���� ���� ��������
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
    //�¸����� ��´�.
    if(!GetTeamPlay())
    {
        return;
    }
    
    DWORD dwResultInfo[MAX_TEAM][2]={0,0,0,0};
    
    for(int i=0;i<MAX_TEAM;++i)
    {
        //0�� û�� 1�� ȫ��
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

    //9055: %dų %d������ %s���� �¸��Ͽ����ϴ�.
    g_InterfaceManager.GetInterfaceString( 9055, szTemp, INTERFACE_STRING_LENGTH );
    
    //�޽��� ��±���(�����ų�̸�����üũ)
    if(dwResultInfo[0][1]>dwResultInfo[1][1] )
    {
        //û��(left)
        g_InterfaceManager.GetInterfaceString( eST_BLUE_TEAM, szTeamName);
        Sprintf( szMessage, szTemp,dwResultInfo[0][1],dwResultInfo[0][0],szTeamName);
    }
    else
    {
        //ȫ��(Right)
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
		// ų���� �÷ȴ�.
		if ( pOrgInfo->bPrevKill )
		{
			// ���� ų���� �ø���
			pOrgInfo->KillCount += pPVPPlayerInfo->KillCount;
			pOrgInfo->StraightKill++;

			if ( pOrgInfo->StraightKill > pOrgInfo->BestStraightKill )
			{
				// �ְ� ���� update
				pOrgInfo->BestStraightKill = pOrgInfo->StraightKill;
			}
		}
		else
		{
			// ���� ų�� �ʱ�ȭ
			pOrgInfo->KillCount += pPVPPlayerInfo->KillCount;
			pOrgInfo->StraightKill = 1;
			pOrgInfo->bPrevKill = TRUE;

			if ( pOrgInfo->StraightKill > pOrgInfo->BestStraightKill )
			{
				// �ְ� ���� update
				pOrgInfo->BestStraightKill = pOrgInfo->StraightKill;
			}
		}
	}
	
	if (pPVPPlayerInfo->DeadCount > 0)
	{
		// ���� Ƚ�� Count

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
			// ������

			PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.begin();

			// �ϴ� ���
			while ( itr != m_PVP_PlayerInfoMap.end() )
			{
				this->m_PVP_PlayerIfoVec[0].push_back( (PVPPlayerInfo *)itr->second );

				itr++;
			};
		}
		break;

	case ePVP_MATCH_TEAM:
		{
			// ����
			PVP_PLAYER_INFO_ITR itr = this->m_PVP_PlayerInfoMap.begin();

			// register
			while ( itr != m_PVP_PlayerInfoMap.end() )
			{
				PVPPlayerInfo * pPVPInfo = (PVPPlayerInfo *)itr->second;
				assert(pPVPInfo);
				assert( pPVPInfo->iTeam > 0 );
				
				int iTeamIdx = (pPVPInfo->iTeam - 1) % MAX_TEAM;		// ���� 2�� ��� �Ƚ��� �����̴�.

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
/**	1.GS_BUG_FIX: �����׾����� ���� �������ش�.
				  1. Ű�����縸���δ� ���۽������ߴ����� �Ǵ��а�
				  2. second�δ� �׾ ����Ȱ�ߴ������¸� üũ�Ѵ�.			
*/
BOOL
PVPManager::CheckLeaveUser( DWORD dwPlayerKey )
{
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);

	if( itr != m_PVP_User_Map.end())
	{
		BYTE *pUser = itr->second;

		//���ܻ�Ȳ:A���ָ��������°� B���׻��¿��� ��Ȱ�� �ϰ� A�� �ٽ� ���´ٸ�
		//		   �׶� ����ó���� ���־���Ѵ�. ��� ���� Ȯ���ؾ���				  	
		return (*pUser ? (TRUE):(FALSE));
	}
	else
	{
		return TRUE;//ó�����³༮�̴�.
	}
}

//----------------------------------------------------------------------------
/**	2. GS_BUG_FIX: ������� �������� Ǯ����
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
/**	2. GS_BUG_FIX: �����׾����� ���� �������ش�.
*/
void
PVPManager::SetUserState( DWORD dwPlayerKey,BOOL bDead)
{
	//ó���������� ����
	InsertLeaveUser( dwPlayerKey );
	
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);
	
	if( itr != m_PVP_User_Map.end())
	{
		BYTE *pUser = itr->second;
		*pUser = bDead;
	}
}
//----------------------------------------------------------------------------
/**	3.GS_BUG_FIX: ������ ������ ������ش�.
*/
void
PVPManager::InsertLeaveUser( DWORD dwPlayerKey )
{
	PVP_USER_MAP_ITR	itr =m_PVP_User_Map.find(dwPlayerKey);
	
	//�ѹ��� �����Ѵ� �����ϸ� ������ ������ ����
	if( itr != m_PVP_User_Map.end()) return;
	
	//���� ó�����³༮�̴� ��������
	BYTE *pUser = new BYTE;
	*pUser = FALSE;
	this->m_PVP_User_Map.insert(PVP_USER_MAP_PAIR(dwPlayerKey, pUser));
	
}
//----------------------------------------------------------------------------
/**	3.GS_BUG_FIX: ������ ������ ������ش�.
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