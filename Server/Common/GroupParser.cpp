#include "stdafx.h"
#include "GroupParser.h"
#include <SolarFileReader.h>
#include "NPCInfoParser.h"

GroupParser::GroupParser() : m_pGroupHashTable ( NULL )
{
}

GroupParser::~GroupParser()
{
	ASSERT( m_pGroupHashTable == NULL );
}

VOID GroupParser::Release()
{
	Unload();
	SAFE_DELETE(m_pGroupHashTable);
}

VOID GroupParser::Init( DWORD dwPoolSize )
{
	ASSERT( m_pGroupHashTable == NULL );
	m_pGroupHashTable = new util::SolarHashTable<BASE_GROUPINFO*>;
	m_pGroupHashTable->Initialize( dwPoolSize );
}

void GroupParser::Unload()
{
    if (m_pGroupHashTable == NULL) {
        return;
    }
    m_pGroupHashTable->SetFirst();
    while (BASE_GROUPINFO* group_info = m_pGroupHashTable->GetNext())
    {
        SAFE_DELETE(group_info);
    }
    m_pGroupHashTable->RemoveAll();
}

VOID GroupParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL GroupParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
	char szTemp[32];

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		WORD wID = GetDataWORD( "GroupID", nRow );

		BASE_GROUPINFO * pInfo = m_pGroupHashTable->GetData( wID );
		if( !pInfo ) 
		{
			// 없으면!
			pInfo = new BASE_GROUPINFO;
			m_pGroupHashTable->Add( pInfo, wID );
		}
		else
		{
			if( FALSE == bReload )
			{
				SUNLOG( eCRITICAL_LOG, "Script File Data Error, File = %s, ID = %d", GetCurFileName(), wID );
				ASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		pInfo->wGroupID			= wID;
		pInfo->dwFormationType	= GetDataDWORD( "FormationType", nRow );
		pInfo->fLinkRange		= GetDataFloat( "LinkRange", nRow );
		pInfo->dwLeaderCode		= GetDataDWORD( "LeaderCode", nRow );

		for( int i = 0; i < MAX_FOLLOWER_NUM; ++i )
		{
			_snprintf( szTemp, 32, "FollowerCode%d", i+1 );
			pInfo->FollowerInfo[i].dwMonCode	= GetDataDWORD( szTemp, nRow );
			_snprintf( szTemp, 32, "FollowerNum%d", i+1 );
			pInfo->FollowerInfo[i].wNum			= GetDataDWORD( szTemp, nRow );
		}

		CheckValidity( pInfo );
	}

	return TRUE;
}


BOOL GroupParser::CheckValidity( BASE_GROUPINFO *pGroupInfo )
{
	BOOL bSuccess = TRUE;

	// 리더 몬스터
	BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( pGroupInfo->dwLeaderCode );
	if( !pBaseNPCInfo )
	{
		SUNLOG( eCRITICAL_LOG, "[GroupParser] GroupID[%d] NPCInfo[%d] doesn't exist! \n", pGroupInfo->wGroupID, pGroupInfo->dwLeaderCode );
		pGroupInfo->dwLeaderCode = 1;
		bSuccess = FALSE;
	}

	// 쫄병
	for( int i = 0; i < MAX_FOLLOWER_NUM; ++i )
	{
		if( !pGroupInfo->FollowerInfo[i].dwMonCode )		continue;

		pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( pGroupInfo->FollowerInfo[i].dwMonCode );
		if( !pBaseNPCInfo )
		{
			SUNLOG( eCRITICAL_LOG, "[GroupParser] GroupID[%d] NPCInfo[%d] doesn't exist! \n", pGroupInfo->wGroupID, pGroupInfo->FollowerInfo[i].dwMonCode );
			pGroupInfo->FollowerInfo[i].dwMonCode = 0;
			pGroupInfo->FollowerInfo[i].wNum = 0;
			bSuccess = FALSE;
		}
	}

	return bSuccess;
}

//#pragma warning ( pop )


BOOL	GroupParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_GROUP:
		return _Load( bReload );
	};

	return FALSE;
}
