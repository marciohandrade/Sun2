#include "stdafx.h"
#include "User.h"
#include "UserManager.h"
#include "ChannelManager.h"
#include "BaseZone.h"
#include "Channel.h"
#include "WaitingUserList.h"

// 친구 정보 위해
#include <Struct.h>
#include <PacketStruct_CW.h>

// 파티 정보 위해
//#include "Party.h"

// 채금 처리 위해
#include <PacketStruct_DW.h>
#include "WorldServer.h"
#include <ResultCode.h>

#include <PacketStruct_AW.h>
// 길드 처리 위해
#include "WorldGuildManager.h"
#include "WorldGuild.h"

// 채널채팅 처리
#include "ChatRoomManager.h"

//__NA_000313_20070223_REALTIME_CHAT_BLOCK

//world shout consum, interception
#include "ServerSession.h"
#include "ServerSessionManager.h"

User::User()
{
	m_GuildGuid = 0;
}

User::~User()
{

}

VOID User::Init()
{
	// 길드 설정
	if( m_GuildGuid )
	{
		// 이 유저가 길드에 가입되었던 유저일 경우 필히 OnlineMember에서 빼 줘야 한다. - 여기 걸리면 버그-_-;
		MessageOut( eCRITICAL_LOG, "User::Init() Error - Not Initialized GuildGuid!![CharGuid:%u][GuildGuid:%u]", m_CharGuid, m_GuildGuid );
		m_GuildGuid = 0;
	}

	m_bLogoutProcessed	= FALSE;
	m_userType			= INVALID_USER;
	m_dwNetworkIndex	= 0;
	m_dwGUID			= 0;
	m_strCharName.clear();
	m_CharGuid			= 0;
	m_dwTimeoutTick		= 0;

    CHECK_ID_LENGTH_BUFFER(m_ptszAccountID);
    ZeroMemory(m_ptszAccountID, sizeof(m_ptszAccountID));
	ZeroMemory(m_szAuthIP, sizeof(m_szAuthIP));
	m_status			= eZONETYPE_CHARSELECT;

	SetWhisper(FALSE);

	m_dwZoneKey			= 0;
	m_dwFieldCode		= 0;
	m_nSectorIndex		= ULONG_MAX;

	// 친구 설정
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	GetFriendMap()->init();
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    GetFriendMap(FRIEND_INVIATE)->init();
    GetFriendMap(FRIEND_BEINVIATED)->init();
#else
	m_mapFriend.clear();
	m_iterNowFriend = NULL;
#endif	

	m_bFriendChatBlock = FALSE;

#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
	m_bFriendList = FALSE;
#else
	m_dwLastFriendTick = 0;
#endif
	m_bFriendTrans = FALSE;
	m_mapOnlineFriend.clear();
	m_OnlineLevel = 0;

	// 리벤지 설정
	m_mapRevenge.clear();
    // (CHANGES) (100115) (WAVERIX) changes invalid code rule
    m_iterNowRevenge = m_mapRevenge.end();
	m_mapOnlineRevenge.clear();
	m_byRevengePoint = 0;

	// 차단 설정
	m_mapBlock.clear();
    m_iterNowBlock = m_mapBlock.end();

	// 길드 설정
	m_bGuildList = FALSE;
	m_dwLastGuildTick = 0;

	// 채팅존 설정
	m_pNormalChatZone = NULL;

	// 어뷰즈 설정
	m_byChatCount = 0;
	m_dwLastChatTime = 0;

	// GM 설정
	m_byGMGrade = 0;

	// 파티 설정
	RemoveParty();

	// 암호화 설정
	m_bCrypting = FALSE;
	m_dwCryptTime = 0;

	// 음성 채팅 설정
	m_dwRequestGUID = 0;
	m_bVoiceOn = FALSE;		// 기본이 FALSE임. 즉, 장애 후 복구되어도 무조건 FALSE임

	FreeChatBlock();

	m_dwLastShoutTime = 0;

	// 채널채팅 설정
	m_pChannelChatRoom = NULL;

	m_dwLastMemoTick = 0;
	m_bMemoTrans = FALSE;

	m_CharLevel = 0;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	memset(m_tszTmpDecreeMsg, NULL, sizeof(TCHAR) * MAX_SHOUT_LEN);

	m_bChatBlock = FALSE;
	m_dwBlockTime = 0;
	m_dwBlockLastTime =0;

    // __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    //그룹 맺기 시스템
    m_eHighMember      = GROUP_HAVE_NONE;
    m_eLowMember        = GROUP_HAVE_NONE;
    m_bGroupTrans       = FALSE;
    m_CharClass         = 0;
    m_bAddBlock         = FALSE;
    m_curBonusMoney     = 0;
    m_curBonusEXP       = 0;
    m_curAccumEXP       = 0;
    m_GroupMemberMap.clear();
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    gender_ = 0;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
}

VOID User::Release()
{
	// 만일 게임 내에서 강제로 User를 Release하는 코드가 필요하다면 반드시 그 전에 이 체크를 해 주어야 한다
	// 현재는 게임 내에서 별도로 Release하는 일이 없어서 OnDisConnect에만 이 코드가 들어가 있다 - 성현창
//	if( !m_bLogoutProcessed )
//	{
//		MessageOut( eFULL_LOG, "User Release Error - m_bLogoutProcessed Not Setted!!![GUID:%u][CharGuid:%u]", GetGUID(), GetCharGuid() );
//		ProcessLogout();	// 혹시나 Logout()이나 OnDisconnect() 에 걸리지 않는 경우 여기서라도 걸려야 한다
//	}

	m_strCharName.clear();

	m_bWhisper			= FALSE;

	m_dwFieldCode		= 0;
	m_nSectorIndex		= ULONG_MAX;

	m_OnlineLevel		= 0;

    CHECK_ID_LENGTH_BUFFER(m_ptszAccountID);
    ZeroMemory(m_ptszAccountID, sizeof(m_ptszAccountID));

	// GM 설정
	m_byGMGrade = 0;

	// 파티 설정
	RemoveParty();

	// 음성 채팅 설정
	m_dwRequestGUID = 0;
	m_bVoiceOn = FALSE;		// 기본이 FALSE임. 즉, 장애 후 복구되어도 무조건 FALSE임

	FreeChatBlock();

	m_dwLastShoutTime = 0;

	// 채널채팅 설정
	m_pChannelChatRoom = NULL;

	m_CharLevel = 0;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	memset(m_tszTmpDecreeMsg, 0, sizeof(TCHAR) * MAX_SHOUT_LEN);

	m_bChatBlock = FALSE;
	m_dwBlockTime = 0;
	m_dwBlockLastTime =0;

	MessageOut( eFULL_LOG, "User Released" );
}

VOID User::Update()
{
}

BOOL User::CheckFriendList()
{
#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
	if( m_bFriendList )
	{
		MessageOut( eCRITICAL_LOG, "[%s] Friend List Hack Check", m_strCharName.c_str() );
		return FALSE;
	}

	m_bFriendList = TRUE;
	return TRUE;
#else
	DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();;
	if( dwTempTick < m_dwLastFriendTick + 5000 )	// 5초에 한번씩만 길드 확인
	{
		MessageOut( eFULL_LOG, "CheckFriendList Failed - Time Checked[CharGuid:%u]", GetCharGuid() );
		return FALSE;
	}

	m_dwLastFriendTick = dwTempTick;

	return TRUE;
#endif
}

VOID User::InitFriendListInit()
{
	// 내 친구 정보는 더이상 필요없다
	UserManager::Instance()->RemoveFriend( this );

	// 나를 친구 삼은 놈들에게 고한다~나 나간다~
	if( !m_mapOnlineFriend.empty() )
	{
		FRIEND_ONLINE_MAP_ITER	iter;

		for( iter = m_mapOnlineFriend.begin(); iter != m_mapOnlineFriend.end(); iter++ )
		{
			(*iter).second->AlarmFriendStatChange( FRIEND_STAT_LOGOUT, this );	// 나간다고 알려준다!!!
			MessageOut( eFULL_LOG, "Alarm Friend Logout[CharGuid:%u][FriendGuid:%u]", m_CharGuid, (*iter).second->GetCharGuid() );
		}
		m_mapOnlineFriend.clear();
	}

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
    GetFriendMap()->Clear();
    GetFriendMap(FRIEND_INVIATE)->Clear();
    GetFriendMap(FRIEND_BEINVIATED)->Clear();
#else
    // 친구 설정 초기화
    STRUCT_FRIEND_INFO *pFriend;

    if( !m_mapFriend.empty() )
    {
        FRIEND_INFO_MAP_ITER	iterFriend;
        for( iterFriend = m_mapFriend.begin(); iterFriend != m_mapFriend.end(); iterFriend++ )
        {
            pFriend = (*iterFriend).second;

            if( !pFriend )
                continue;

            UserManager::Instance()->FreeFriendInfo( pFriend );
        }
        m_mapFriend.clear();
    }
#endif

	m_bFriendChatBlock = FALSE;
#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
	m_bFriendList = FALSE;
#endif
	m_bFriendTrans = FALSE;
	m_mapOnlineFriend.clear();
	m_OnlineLevel = 0;

	// 차단 설정 초기화
	STRUCT_BLOCK_INFO *pBlock;
	if( !m_mapBlock.empty() )
	{
		BLOCK_INFO_MAP_ITER	iterBlock;
		for( iterBlock = m_mapBlock.begin(); iterBlock != m_mapBlock.end(); iterBlock++ )
		{
			pBlock = (*iterBlock).second;

			if( !pBlock )
				continue;

			UserManager::Instance()->FreeBlockInfo( pBlock );
		}
		m_mapBlock.clear();
	}

	UserManager::Instance()->RemoveRevenge( this );

	// 나를 원수 삼은 놈들에게 고한다~나 나간다~
	if( !m_mapOnlineRevenge.empty() )
	{
		FRIEND_ONLINE_MAP_ITER	iter;

		for( iter = m_mapOnlineRevenge.begin(); iter != m_mapOnlineRevenge.end(); iter++ )
		{
			(*iter).second->AlarmRevengeStatChange( REVENGE_STAT_LOGOUT, this );	// 나간다고 알려준다!!!
			MessageOut( eFULL_LOG, "Alarm Revenge Logout[CharGuid:%u][FriendGuid:%u]", m_CharGuid, (*iter).second->GetCharGuid() );
		}
		m_mapOnlineRevenge.clear();
	}

	// 리벤지 설정 초기화
	STRUCT_REVENGE_INFO *pRevenge;
	if( !m_mapRevenge.empty() )
	{
		REVENGE_INFO_MAP_ITER	iterRevenge;
		for( iterRevenge = m_mapRevenge.begin(); iterRevenge != m_mapRevenge.end(); iterRevenge++ )
		{
			pRevenge = (*iterRevenge).second;

			if( !pRevenge )
				continue;

			UserManager::Instance()->FreeRevengeInfo( pRevenge );
		}
		m_mapRevenge.clear();
	}
}
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

FriendMap* User::GetFriendMap(FRIEND_TYPE type)
{	
	switch(type)
	{
	case FRIEND_NORMAL: return &m_mapFriend;
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
	case FRIEND_INVIATE: return &m_mapInvitateFriend;
	case FRIEND_BEINVIATED: return &m_mapBeInvitatedFriend;
    //}
	}
	return NULL;
}

#else		

STRUCT_FRIEND_INFO* User::GetFirstFriend()
{
//	if( !CheckFriendList() )
//	{
//		return NULL;
//	}

	m_iterNowFriend = m_mapFriend.begin();

	if( m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowFriend).second );
}

STRUCT_FRIEND_INFO* User::GetNextFriend()
{
	if(/* !CheckFriendList() || */m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	m_iterNowFriend++;

	if( m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowFriend).second );
}

BOOL User::AddFriendInfo( STRUCT_FRIEND_INFO* pFriend )
{
	if( GetFriendNum() >= MAX_FRIEND_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[User::AddFriendInfo] OverFlow, User = %s, FriendGuid = %u" , GetCharName().c_str(), pFriend->dwFriendGuid ); 
		return FALSE;
	}

	STRUCT_FRIEND_INFO *pNowFriend = UserManager::Instance()->AllocFriendInfo();	//new STRUCT_FRIEND_INFO;

	memcpy( pNowFriend, pFriend, sizeof(STRUCT_FRIEND_INFO) );

	// 이미 같은 정보가 있으면 에러
	if( FindFriend( pFriend->dwFriendGuid ) != NULL )
	{
		return FALSE;
	}

	m_mapFriend.insert( FRIEND_INFO_MAP_PAIR( pFriend->dwFriendGuid, pNowFriend ) );

	return TRUE;
}

STRUCT_FRIEND_INFO* User::FindFriend( DWORD dwCharGuid )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.find( dwCharGuid );
	if( iter == m_mapFriend.end() )
	{
		return NULL;
	}

	STRUCT_FRIEND_INFO *pFriendInfo = (*iter).second;

	return pFriendInfo;
}

STRUCT_FRIEND_INFO* User::FindFriend( TCHAR* ptszFriendName )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.begin();
	if( iter == m_mapFriend.end() )
	{
		return NULL;
	}

	for( iter = m_mapFriend.begin(); iter != m_mapFriend.end(); iter++ )
	{
		STRUCT_FRIEND_INFO *pFriendInfo = (*iter).second;

		if( !_tcsnicmp( pFriendInfo->sPacketInfo.ptszFriendName, ptszFriendName, MAX_CHARNAME_LENGTH ) )
		{
			return pFriendInfo;
		}
	}

	return NULL;
}

VOID User::RemoveFriend( DWORD dwGuid )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.find( dwGuid );
	if( iter == m_mapFriend.end() )
	{
		return;
	}

	STRUCT_FRIEND_INFO *pFriend = (*iter).second;

//	delete pFriend;
	UserManager::Instance()->FreeFriendInfo( pFriend );
	m_mapFriend.erase(iter);
}
#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING

VOID User::AddOnlineFriend( User *pUser, BOOL bLoggedIn )
{
	// 목록에 추가
	m_mapOnlineFriend.insert( FRIEND_ONLINE_MAP_PAIR( pUser->GetCharGuid(), pUser ) ); 

	if( bLoggedIn )
	{
		// 내가 들어왔다! 알아모셔라!!!
		pUser->AlarmFriendStatChange( FRIEND_STAT_LOGIN, this );
	}
}

VOID User::RemoveOnlineFriend( User *pUser )
{
	FRIEND_ONLINE_MAP_ITER	iter = m_mapOnlineFriend.find( pUser->GetCharGuid() );

	if( iter != m_mapOnlineFriend.end() )
	{
		m_mapOnlineFriend.erase( iter );

		// 내가 들어왔다! 알아모셔라!!!
		pUser->AlarmFriendStatChange( FRIEND_STAT_LOGOUT, this );
	}
}

User* User::FindOnlineFriend( DWORD CharGuid )
{
	FRIEND_ONLINE_MAP_ITER iter = m_mapOnlineFriend.find( CharGuid );

	if( iter == m_mapOnlineFriend.end() )
	{
		return NULL;
	}

	return (*iter).second;
}

VOID User::AlarmFriendStatChange( int nFlag, User *pUser )
{
	switch( nFlag )
	{
		case FRIEND_STAT_LOGIN:		// Login
		{
			// 친구 정보에서 온라인으로 변경!
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
			STRUCT_FRIEND_INFO *pFriendInfo = GetFriendMap()->FindFriend( pUser->GetCharGuid() );
#else
			STRUCT_FRIEND_INFO *pFriendInfo = FindFriend( pUser->GetCharGuid() );
#endif
			

			if( !pFriendInfo )
			{
				MessageOut( eCRITICAL_LOG, "[%s] AlarmFriend Error - No Friend %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
				break;
			}

			pFriendInfo->sPacketInfo.bOnline = TRUE;	// 온라인이다!!!
			pFriendInfo->sPacketInfo.byChannel = static_cast<BYTE>(pUser->GetChannelID());	// 채널 정보도 같이 알려준다

			// 대상 유저가 Gm 일경우 자신이 Gm이 아니면 정보를 주지 않는다./
			if( pUser && 
				TRUE  == pUser->IsGM() &&
				FALSE == IsGM() )
			{
				MessageOut(eCRITICAL_LOG, "[%s] AlarmFriend Error - GM Friend %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
				break;
			}

			MSG_CW_FRIEND_LOGIN_BRD	sendMsg;

			memset( sendMsg.ptszFriendName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
			_tcsncpy( sendMsg.ptszFriendName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
			sendMsg.byChannelID = pFriendInfo->sPacketInfo.byChannel;
			SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_LOGIN_BRD) );
			break;
		}
		case FRIEND_STAT_LOGOUT:	// Logout
		{
			// 친구 정보에서 오프라인으로 변경!
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
			STRUCT_FRIEND_INFO *pFriendInfo = GetFriendMap()->FindFriend( pUser->GetCharGuid() );
#else
			STRUCT_FRIEND_INFO *pFriendInfo = FindFriend( pUser->GetCharGuid() );
#endif
            

            if( !pFriendInfo )
            {
                MessageOut( eFULL_LOG, "[%s] AlarmFriend_Logout Error - No Friend %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
                break;
            }

            pFriendInfo->sPacketInfo.bOnline = FALSE;	// 오프라인이다!!!
            pFriendInfo->sPacketInfo.byChannel = 0;

			MSG_CW_FRIEND_LOGOUT_BRD	sendMsg;

			memset( sendMsg.ptszFriendName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
			_tcsncpy( sendMsg.ptszFriendName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
			SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_LOGOUT_BRD) );
			break;
        }
	}
}

VOID User::SendOnlineFriendAll( BYTE* pMsg, WORD wSize )
{
	if( m_mapOnlineFriend.empty() )
	{
		MessageOut( eFULL_LOG, "SendOnlineFriendAll - No Friend[CharGuid:%u]", m_CharGuid );
		return;
	}

	FRIEND_ONLINE_MAP_ITER	iter;
	User					*pFriend = NULL;

	for( iter = m_mapOnlineFriend.begin(); iter != m_mapOnlineFriend.end(); iter++ )
	{
		pFriend = (*iter).second;

		pFriend->SendPacket( pMsg, wSize );
	}
}

// 리벤지 정보 관리용
STRUCT_REVENGE_INFO* User::GetFirstRevenge()
{
//	if( !CheckRevengeList() )
//	{
//		return NULL;
//	}

	m_iterNowRevenge = m_mapRevenge.begin();

	if( m_iterNowRevenge == m_mapRevenge.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowRevenge).second );
}

STRUCT_REVENGE_INFO* User::GetNextRevenge()
{
	if(/* !CheckRevengeList() || */m_iterNowRevenge == m_mapRevenge.end() )
	{
		return NULL;
	}

	m_iterNowRevenge++;

	if( m_iterNowRevenge == m_mapRevenge.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowRevenge).second );
}

BOOL User::AddRevengeInfo( STRUCT_REVENGE_INFO* pRevenge )
{
	if( GetRevengeNum() >= MAX_REVENGE_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[User::AddRevengeInfo] Overflow, User = %s, RevengeCharGuid = %u" , GetCharName().c_str(), pRevenge->dwRevengeCharGuid ); 
		return FALSE;
	}

	STRUCT_REVENGE_INFO *pNowRevenge = UserManager::Instance()->AllocRevengeInfo();	//new STRUCT_REVENGE_INFO;

	memcpy( pNowRevenge, pRevenge, sizeof(STRUCT_REVENGE_INFO) );

	// 이미 같은 정보가 있으면 에러
	if( FindRevenge( pRevenge->dwRevengeCharGuid ) != NULL )
	{
		return FALSE;
	}

	m_mapRevenge.insert( REVENGE_INFO_MAP_PAIR( pRevenge->dwRevengeCharGuid, pNowRevenge ) );

	return TRUE;
}

STRUCT_REVENGE_INFO* User::FindRevenge( DWORD dwCharGuid )
{
	REVENGE_INFO_MAP_ITER iter = m_mapRevenge.find( dwCharGuid );
	if( iter == m_mapRevenge.end() )
	{
		return NULL;
	}

	STRUCT_REVENGE_INFO *pRevengeInfo = (*iter).second;

	return pRevengeInfo;
}

STRUCT_REVENGE_INFO* User::FindRevenge( TCHAR* ptszRevengeName )
{
	REVENGE_INFO_MAP_ITER iter = m_mapRevenge.begin();
	if( iter == m_mapRevenge.end() )
	{
		return NULL;
	}

	for( iter = m_mapRevenge.begin(); iter != m_mapRevenge.end(); iter++ )
	{
		STRUCT_REVENGE_INFO *pRevengeInfo = (*iter).second;

		if( !_tcsnicmp( pRevengeInfo->sPacketInfo.ptszRevengeName, ptszRevengeName, MAX_CHARNAME_LENGTH ) )
		{
			return pRevengeInfo;
		}
	}

	return NULL;
}

VOID User::RemoveRevenge( DWORD dwGuid )
{
	REVENGE_INFO_MAP_ITER iter = m_mapRevenge.find( dwGuid );
	if( iter == m_mapRevenge.end() )
	{
		return;
	}

	STRUCT_REVENGE_INFO *pRevenge = (*iter).second;

//	delete pRevenge;
	UserManager::Instance()->FreeRevengeInfo( pRevenge );
	m_mapRevenge.erase(iter);
}

VOID User::AddOnlineRevenge( User *pUser, BOOL bLoggedIn )
{
	// 목록에 추가
	m_mapOnlineRevenge.insert( FRIEND_ONLINE_MAP_PAIR( pUser->GetCharGuid(), pUser ) ); 

	if( bLoggedIn )
	{
		// 내가 들어왔다! 알아모셔라!!!
		pUser->AlarmRevengeStatChange( REVENGE_STAT_LOGIN, this );
	}
}

VOID User::RemoveOnlineRevenge( User *pUser )
{
	FRIEND_ONLINE_MAP_ITER	iter = m_mapOnlineRevenge.find( pUser->GetCharGuid() );

	if( iter != m_mapOnlineRevenge.end() )
	{
		m_mapOnlineRevenge.erase( iter );

		// 내가 들어왔다! 알아모셔라!!!
		pUser->AlarmRevengeStatChange( REVENGE_STAT_LOGOUT, this );
	}
}

User* User::FindOnlineRevenge( DWORD CharGuid )
{
	FRIEND_ONLINE_MAP_ITER iter = m_mapOnlineRevenge.find( CharGuid );

	if( iter == m_mapOnlineRevenge.end() )
	{
		return NULL;
	}

	return (*iter).second;
}

VOID User::AlarmRevengeStatChange( int nFlag, User *pUser )
{
	switch( nFlag )
	{
		case REVENGE_STAT_LOGIN:		// Login
		{
			// 리벤지 정보에서 온라인으로 변경!
			STRUCT_REVENGE_INFO *pRevengeInfo = FindRevenge( pUser->GetCharGuid() );

			if( !pRevengeInfo )
			{
				MessageOut( eCRITICAL_LOG, "[%s] AlarmRevenge Error - No Revenge %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
				break;
			}

			pRevengeInfo->sPacketInfo.bOnline = TRUE;	// 온라인이다!!!
			pRevengeInfo->sPacketInfo.byChannelID = static_cast<BYTE>(pUser->GetChannelID());	// 채널 정보도 같이 알려준다

			// 대상 유저가 Gm 일경우 자신이 Gm이 아니면 정보를 주지 않는다./
			if( pUser && 
				TRUE  == pUser->IsGM() &&
				FALSE == IsGM() )
			{
				MessageOut( eCRITICAL_LOG, "[%s] AlarmFriend Error - GM Friend %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
				break;
			}

			MSG_CW_REVENGE_LOGIN_BRD	sendMsg;
			memset( sendMsg.m_ptszCharName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
			_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
			sendMsg.byChannelID = static_cast<BYTE>(pUser->GetChannelID());	// 채널 정보도 같이 알려준다
			SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_REVENGE_LOGIN_BRD) );
			break;
		}
		case REVENGE_STAT_LOGOUT:	// Logout
		{
			// 친구 정보에서 오프라인으로 변경!
			STRUCT_REVENGE_INFO *pRevengeInfo = FindRevenge( pUser->GetCharGuid() );

			if( !pRevengeInfo )
			{
				MessageOut( eFULL_LOG, "[%s] AlarmRevenge Error - No Revenge %s", GetCharName().c_str(), pUser->GetCharName().c_str() );
				break;
			}

			pRevengeInfo->sPacketInfo.bOnline = FALSE;	// 오프라인이다!!!
			pRevengeInfo->sPacketInfo.byChannelID = 0;

			MSG_CW_REVENGE_LOGOUT_BRD	sendMsg;
			memset( sendMsg.ptszCharName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
			_tcsncpy( sendMsg.ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
			SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_REVENGE_LOGOUT_BRD) );
			break;
		}
	}
}

VOID User::SendOnlineRevengeAll( BYTE* pMsg, WORD wSize )
{
	if( m_mapOnlineRevenge.empty() )
	{
		MessageOut( eFULL_LOG, "SendOnlineRevengeAll - No Revenge[CharGuid:%u]", m_CharGuid );
		return;
	}

	FRIEND_ONLINE_MAP_ITER	iter;
	User					*pRevenge = NULL;

	for( iter = m_mapOnlineRevenge.begin(); iter != m_mapOnlineRevenge.end(); iter++ )
	{
		pRevenge = (*iter).second;

		pRevenge->SendPacket( pMsg, wSize );
	}
}


// 차단 정보 관리용

STRUCT_BLOCK_INFO* User::GetFirstBlock()
{
	m_iterNowBlock = m_mapBlock.begin();

	if( m_iterNowBlock == m_mapBlock.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowBlock).second );
}

STRUCT_BLOCK_INFO* User::GetNextBlock()
{
	if( m_iterNowBlock == m_mapBlock.end() )
	{
		return NULL;
	}

	m_iterNowBlock++;

	if( m_iterNowBlock == m_mapBlock.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowBlock).second );
}

BOOL User::AddBlockInfo( STRUCT_BLOCK_INFO* pBlock )
{
	if( GetBlockNum() >= MAX_BLOCK_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[User::AddBlockInfo] Overflow, User = %s, BlockGuid = %u" , GetCharName().c_str(), pBlock->dwBlockGuid ); 
		return FALSE;
	}

	STRUCT_BLOCK_INFO *pNowBlock = UserManager::Instance()->AllocBlockInfo();	//new STRUCT_BLOCK_INFO;

	if( !pNowBlock )
	{
		return FALSE;
	}

	memcpy( pNowBlock, pBlock, sizeof(STRUCT_BLOCK_INFO) );

	// 이미 같은 정보가 있으면 에러
	if( FindBlock( pBlock->dwBlockGuid ) != NULL )
	{
		UserManager::Instance()->FreeBlockInfo( pNowBlock );
		return FALSE;
	}

	m_mapBlock.insert( BLOCK_INFO_MAP_PAIR( pBlock->dwBlockGuid, pNowBlock ) );

	return TRUE;
}

STRUCT_BLOCK_INFO* User::FindBlock( DWORD dwGuid )
{
	BLOCK_INFO_MAP_ITER iter = m_mapBlock.find( dwGuid );
	if( iter == m_mapBlock.end() )
	{
		return NULL;
	}

	STRUCT_BLOCK_INFO *pBlockInfo = (*iter).second;

	return pBlockInfo;
}

STRUCT_BLOCK_INFO* User::FindBlock( TCHAR* ptszBlockName )
{
	BLOCK_INFO_MAP_ITER iter = m_mapBlock.begin();
	if( iter == m_mapBlock.end() )
	{
		return NULL;
	}

	for( iter = m_mapBlock.begin(); iter != m_mapBlock.end(); iter++ )
	{
		STRUCT_BLOCK_INFO *pBlockInfo = (*iter).second;

		if( !_tcsncmp( pBlockInfo->ptszBlockName, ptszBlockName, MAX_CHARNAME_LENGTH ) )
		{
			return pBlockInfo;
		}
	}

	return NULL;
}

VOID User::RemoveBlock( DWORD dwGuid )
{
	BLOCK_INFO_MAP_ITER iter = m_mapBlock.find( dwGuid );
	if( iter == m_mapBlock.end() )
	{
		return;
	}

	STRUCT_BLOCK_INFO *pBlock = (*iter).second;

//	delete pBlock;
	UserManager::Instance()->FreeBlockInfo( pBlock );
	m_mapBlock.erase(iter);
}

VOID User::RemoveBlock( TCHAR* ptszBlockedName )
{
	BLOCK_INFO_MAP_ITER iter = m_mapBlock.begin();
	if( iter == m_mapBlock.end() )
	{
		return;
	}

	STRUCT_BLOCK_INFO* pBlock;

	for( iter = m_mapBlock.begin(); iter != m_mapBlock.end(); iter++ )
	{
		pBlock = (*iter).second;

		if( !_tcsncmp( pBlock->ptszBlockName, ptszBlockedName, MAX_CHARNAME_LENGTH ) )
		{
//			delete pBlock;
			UserManager::Instance()->FreeBlockInfo( pBlock );
			m_mapBlock.erase(iter);
//			MessageOut( eFULL_LOG, "[%s] Remove Block %s", GetCharName().c_str(), ptszBlockedName );
			break;
		}
	}
}

// 길드 리스트를 정해진 시간마다만 보내주도록 제한
BOOL User::CheckGuildList()
{
	DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();

	if( dwTempTick < m_dwLastGuildTick + 5000 )	// 5초에 한번씩만 길드 확인
	{
		MessageOut( eFULL_LOG, "CheckGuildList Failed - Time Checked[CharGuid:%u]", GetCharGuid() );
		return FALSE;
	}

	m_dwLastGuildTick = dwTempTick;

	return TRUE;
}

// 채팅 어뷰즈 체크용
BOOL User::CheckChatTime()
{
	DWORD dwChatTime = GetTickCount();

	// 10분의 1초 이내 시간으로 채팅이 계속 날아오면 체크했다가 10회 이상이면 잘라버린다.
	if( dwChatTime <= m_dwLastChatTime + 100 )
	{
		m_byChatCount++;
		m_dwLastChatTime = dwChatTime;

		if( m_byChatCount > 10 )
		{
			MessageOut( eCRITICAL_LOG, "User[%s][CharGuid:%u] Chat Abuse!!", m_strCharName.c_str(), m_CharGuid );
			Logout();
		}
		return FALSE;
	}

	// 이상 없으면 초기화
	m_byChatCount = 0;
	m_dwLastChatTime = dwChatTime;

	return TRUE;
}

VOID User::SendParty( BYTE* pMsg, WORD wSize )
{
	if( NULL == m_pParty )
	{
		MessageOut( eFULL_LOG, "SendParty Error - No Party[CharGuid:%u]", m_CharGuid );
		return;
	}

	m_pParty->Send( pMsg, wSize );
}

// 채팅금지 세팅
VOID User::SetChatBlock( DWORD dwTime )
{
	m_bChatBlock = TRUE;
	m_dwBlockTime = dwTime * 60000;	// 소스가 분 단위이므로 ms 단위로 고쳐준다
	m_dwBlockLastTime = GetTickCount();

	MessageOut( eFULL_LOG, "SetChatBlockTime[%u][Time:%u]", m_CharGuid, m_dwBlockTime );
}

// 채팅허용 세팅
VOID User::FreeChatBlock()
{
	m_bChatBlock = FALSE;
	m_dwBlockTime = 0;
	m_dwBlockLastTime = 0;
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	MessageOut( eFULL_LOG, "FreeChatBlockTime[%u]", m_CharGuid );
}

//__NA_000313_20070223_REALTIME_CHAT_BLOCK
BOOL User::UpdateBlockTime()
{
	if( !IsChatBlocked() )
	{
		MessageOut( eFULL_LOG, "Not Blocked User[%u]", m_CharGuid );
		return FALSE;
	}

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	DWORD dwThisTime = GetTickCount();

	if( dwThisTime <= m_dwBlockLastTime )
	{
		MessageOut( eFULL_LOG, "?!!!! Invalid Block Time![%u]", m_CharGuid );
		FreeChatBlock();
		return FALSE;
	}

	// 시간 다 되었으면 채팅금지 해제
	if( m_dwBlockTime <= dwThisTime - m_dwBlockLastTime )
	{
//		UserManager::Instance()->RemoveChatBlockUser(this);
		MSG_DW_GM_CHATBLOCK_UPDATE_SYN	sendMsg;

		sendMsg.m_dwKey = m_CharGuid;
		sendMsg.m_dwRemainTime = 0;

		WorldServer::Instance()->SendGameDBProxy( (BYTE*)&sendMsg, sizeof(sendMsg) );

		MessageOut( eFULL_LOG, "End Block Time[%u]", m_CharGuid );
		FreeChatBlock();
		return FALSE;
	}

	DWORD dwTempTime = m_dwBlockTime - ( dwThisTime - m_dwBlockLastTime );

	m_dwBlockLastTime = dwThisTime;

	MSG_DW_GM_CHATBLOCK_UPDATE_SYN	sendMsg;

	sendMsg.m_dwKey = m_CharGuid;
	sendMsg.m_dwRemainTime = dwTempTime / 60000;	// 저장할 때는 다시 분 단위 - 중간에 잘리는 부분은 그냥 버린다

	WorldServer::Instance()->SendGameDBProxy( (BYTE*)&sendMsg, sizeof(sendMsg) );

	m_dwBlockTime = dwTempTime;

	MessageOut( eFULL_LOG, "Block Time Updated[%u][%u]", m_CharGuid, m_dwBlockTime );
	return TRUE;
}

// 채널 채팅 메시지 보내기
BOOL User::SendChannelChat( BYTE* pMsg, WORD wSize )
{
	if( !m_pChannelChatRoom )
	{
		return FALSE;
	}

	m_pChannelChatRoom->Send( pMsg, wSize );

	return TRUE;
}

BOOL User::CheckMemoList()
{
	DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();

	if( dwTempTick < m_dwLastMemoTick + 5000 )	// 5초에 한번씩만 메모 확인
	{
		MessageOut( eFULL_LOG, "CheckMemoList Failed - Time Checked[CharGuid:%u]", GetCharGuid() );
		return FALSE;
	}

	m_dwLastMemoTick = dwTempTick;

	return TRUE;
}

VOID User::OnAccept( DWORD dwNetworkIndex )
{
	SetNetworkIndex( dwNetworkIndex );
}

VOID User::OnDisconnect()
{
	MessageOut( eFULL_LOG, "User::Disconnect()" );
	if( !m_bLogoutProcessed )
	{
		ProcessLogout();
//		Release();
	}
//	else
//	{
//		MessageOut( eFULL_LOG, "유저가 접속 종료[C:%u][Guid:%u][IP:%s]", GetChannelID(), GetGUID(), GetAuthIP() );
//	}
}

VOID User::OnRecv( BYTE *pMsg, WORD wSize )
{
    __UNUSED((pMsg, wSize));
}

VOID User::OnEnterZone( BaseZone *pZone )
{
	SetZoneKey( pZone->GetKey() );
	SetStatus( pZone->GetZoneType() );
	SetMapCode( pZone->GetMapCode() );
}

VOID User::OnLeaveZone( BaseZone *pZone )
{
    __UNUSED(pZone);
	SetZoneKey( 0 );
	SetStatus( eZONETYPE_MAX );	// 나간 상태다 이놈은!!!
}

VOID User::OnEnterSector( BaseZone *pZone )
{
	SetSectorIndex( pZone->GetKey() );
}

VOID User::OnLeaveSector( BaseZone *pZone )
{
    __UNUSED(pZone);
	SetSectorIndex( -1 );
}

VOID User::Logout()
{
	MessageOut( eFULL_LOG, "Logout User - Channel(%d) Guid(%d) IP(%s)", GetChannelID(), GetGUID(), GetAuthIP() );

	// 로그아웃시의 처리
	ProcessLogout();	

	// 네트웍 끊기
	Disconnect();
}

VOID User::ProcessLogout()
{
	m_bLogoutProcessed = TRUE;

	// 채널에서 제거
	Channel *pChannel = ChannelManager::Instance()->GetChannel( GetChannelID() );
	if( pChannel )
	{
		pChannel->RemoveUser( this );
	}

	ProcessCharLogout();
/*
	// 나를 친구 삼은 놈들에게 고한다~나 나간다~
	if( !m_mapOnlineFriend.empty() )
	{
		FRIEND_ONLINE_MAP_ITER	iter;

		for( iter = m_mapOnlineFriend.begin(); iter != m_mapOnlineFriend.end(); iter++ )
		{
			(*iter).second->AlarmFriendStatChange( FRIEND_STAT_LOGOUT, this );	// 나간다고 알려준다!!!
		}
	}

	// 내 친구 정보는 더이상 필요없다
	UserManager::Instance()->RemoveFriend( this );

	// 채팅 금지중이었을 경우 채팅 금지 시간 업데이트 처리한다
	UpdateBlockTime();
*/
	EndCrypt();
}

// 캐릭터에 관련된 정보 정리
VOID User::ProcessCharLogout()
{
	// 친구 리소스 초기화
	InitFriendListInit();
    InitGroupInfo();
	// 채팅 금지중이었을 경우 채팅 금지 시간 업데이트 처리한다
	if( IsChatBlocked() )
	{
        //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		UpdateBlockTime();
	}

	// 파티원이었을 경우 파티에서 탈퇴 처리
	if( m_pParty )
	{
		Channel *pChannel = ChannelManager::Instance()->GetChannel( m_dwChannelID );
		if( pChannel )	// 서버 종료할 때는 Channel이 없을 수도 있다
		{
			pChannel->WithdrawParty( this );
		}
		m_pParty = NULL;
	}

	// 길드 있으면 접속 끊었다고 처리
	if( GetGuildGuid() )
	{
		SetGuildGuid( 0 );
	}

	GetItemOwnFlag().Init();	


	// 채널채팅 로그아웃
	if( GetChannelChatRoom() )
	{
		ChatRoomManager::Instance()->RemoveUser( this );
	}
}


VOID User::SetAccountID( const TCHAR * ptszAccountID )
{
	memcpy( m_ptszAccountID, ptszAccountID, sizeof(TCHAR) * ID_MAX_LEN );
	m_ptszAccountID[ID_MAX_LEN] = '\0';
}

void User::OnLogString(char *pszLog)
{
	MessageOut(eCRITICAL_LOG, pszLog);
}

void User::SendNormalChat( BYTE *pMsg, WORD wSize )
{
	if( IsChatBlocked() )
	{	
		SendChatBlock();
		return;
	}

	if( m_pNormalChatZone )
	{
		if( !m_pNormalChatZone->Send( pMsg, wSize ) )
		{
			// CHAT 시도 실패 - ViewPort 동기화 실패
			MSG_CW_CHAT_NORMAL_NAK	sendMsg;

			sendMsg.m_wErrorCode = RC::RC_CHAT_SYNC;
			SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
			MessageOut( eMIDDLE_LOG, "SendNormalChat Error - Send Failed[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
																											GetCharGuid(), 
																											GetStatus(), 
																											GetZoneKey() );
		}
	}
	else
	{
		MessageOut( eCRITICAL_LOG, "SendNormalChat Error - No NormalChat Zone[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
																												GetCharGuid(), 
																												GetStatus(), 
																												GetZoneKey() );
	}
}

void User::SendNomalChatToAll( BYTE *pMsg, WORD wSize )
{
	if( IsChatBlocked() )
	{
		SendChatBlock();
		return;
	}

	if( !WorldServer::Instance()->CheckShoutInterval( m_dwLastShoutTime ) )
	{
		MSG_CW_CHAT_SHOUT_NAK	sendMsg;

		sendMsg.m_wErrorCode = RC::RC_CHAT_ALREADYSHOUT;;
		SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		MessageOut( eFULL_LOG, "Shout Interval Checked[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
																							GetCharGuid(), 
																							GetStatus(), 
																							GetZoneKey() );
		return;
	}

	if( m_pNormalChatZone )
	{
		m_pNormalChatZone->SendToAll( pMsg, wSize );
	}
}

#include "WorldServerInfoParser.h"

#include "PacketHandler.h"

BOOL User::SendPacket( BYTE *pMsg, WORD wSize )
{
	union
	{
		MSG_BASE*			pBASE;
		MSG_BASE_INTERNAL*	pINTERNAL;
		BYTE*				pBYTE;
		MSG_CW_CRYPTOGRAPHY_PACKET_ENCODING* pENCODING;
	};

	pBASE = PACKET_CONTROL::GetPacketPtr_BASE_A2C();
	if( pBYTE != pMsg )
		CopyMemory( pBYTE, pMsg, (SIZE_T)wSize );
	wSize = ((WORD)wSize-(WORD)sizeof(DWORD));
	pBYTE = PACKET_CONTROL::GetPacketPtr_A2C();

	if( WorldServerInfoParser::Instance()->IsCryptable() && this->GetCryptography() )
	{
		pENCODING = PACKET_CONTROL::GetPacketPtr_A2C_ENCODING();
		static BOOL bDummy = CryptManager::SetGarbageBlock( (BYTE*)pENCODING, (DWORD)(__int64)pENCODING );

		ASSERT( wSize && wSize<16000 );

		pENCODING->SetSNKey( m_sCrypt.GetSendSNKey() );
		Encrypt( pENCODING->GetPacketStream(), wSize, pENCODING->GetCRC() );

		// remove info
		//if( wSize < sizeof(DWORD) )		memset(pMsg, 0, wSize);
		//else *(DWORD*)pMsg = 0;	// protection overflow

		pMsg = pBYTE;
		wSize += (WORD)pENCODING->GetHeaderSize();

		m_sCrypt.NextRoundSNKey4Send();	// 새로운 SN키를 발급받는다. 
		// <END>
	}

	return Send( pMsg, wSize );
}

//현재 이 함수는 월드 외치기 용으로 사용된다.
VOID User::SendWorldChatToAll(BYTE *pMsg, WORD wSize, BOOL bNow)
{
	//1. 채팅블록된 유저인지 확인한다.
	if(IsChatBlocked())
	{
		SendChatBlock();
		return;
	}

	//2. 외치기 간격을 확인한다.
	if(!WorldServer::Instance()->CheckWorldShoutInterval( m_dwLastShoutTime ))
	{
		MSG_CW_CHAT_SHOUT_NAK	sendMsg;

		sendMsg.m_wErrorCode = RC::RC_CHAT_ALREADYSHOUT;
		SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		MessageOut( eFULL_LOG, "Shout Interval Checked[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
			GetCharGuid(), 
			GetStatus(), 
			GetZoneKey() );
		return;
	}	

	// 기간제이거나 pc방 일경우 즉시 메시지를 클라에 보냄. 아닐경우 아이템 삭제확인 
	if( bNow )
		UserManager::Instance()->SendToAll(pMsg, wSize);
	else
		SendUseDecreeItem();	
}

VOID User::SendChatBlock(RC::eCHAT_RESULT result)
{
	MSG_CW_CHAT_NORMAL_NAK	sendMsg;
	sendMsg.m_wErrorCode = result;
	SendPacket((BYTE*)&sendMsg, sizeof(sendMsg));
	return;
}

VOID User::SendChatBlock()
{
	MSG_CW_CHAT_NORMAL_NAK	sendMsg;

	sendMsg.m_wErrorCode = RC::RC_CHAT_BLOCKED;
	SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

	MessageOut( eFULL_LOG, "Blocked User Want Chat[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
		GetCharGuid(), GetStatus(), GetZoneKey() );

	return;
}

//__NA_000313_20070223_REALTIME_CHAT_BLOCK

VOID User::SendUseDecreeItem()
{
	ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer(GetChannelID());
	if(!pAgentServer)
	{
		MessageOut(eCRITICAL_LOG, "[User::SendUseDecreeItem] Decree Item Use Send Error - No AgentServer!!![%u],[%s]", GetChannelID(), GetCharName().c_str() );
	}	
	else
	{
		MSG_AW_ITEM_USE_DECREE_ITEM_SYN sendMsg;
		sendMsg.m_dwKey = m_dwGUID;
		sendMsg.m_dwCharGuid = m_CharGuid;
		pAgentServer->Send((BYTE*)&sendMsg, sizeof(sendMsg));
	}
}

VOID	User::SetLastChatMsg( const char* szMsg )
{
	strncpy( m_szLastChatMsg, szMsg, MAX_CHATMSG_SIZE );
	m_szLastChatMsg[MAX_CHATMSG_SIZE] = '\0';
}

BOOL User::SetInterceptionOption(eINTERCEPTION_TYPE eCode, BOOL bFlag)
{
	MSG_AW_USER_STATUS_INTERCEPTION_ACK msgACK;
	msgACK.m_dwKey = this->GetGUID();

	switch(eCode)
	{
	case eINTERCEPTION_WHISPER:
		{
			this->SetWhisper(bFlag);

			ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer(GetChannelID());
			if(!pAgentServer)
			{
				MessageOut(eCRITICAL_LOG, "Set Interception Option Error -  No AgentServer!!![%u]", GetChannelID());
				return FALSE;				
			}			

			msgACK.m_byInterceptionCode = eCode;
			msgACK.m_bFlag = bFlag;

			pAgentServer->Send((BYTE*)&msgACK, sizeof(msgACK));
			return TRUE;
		}
		break;
	}

	return FALSE;
}

//{__CN_000001_20090513_ADD_FRIEND_CHECK_GM
INT User::CopyBlockInfo( STRUCT_BLOCK_INFO* pDstBlockInfo, const int size )
{
    INT nCnt = 0;
    BLOCK_INFO_MAP_ITER iter = m_mapBlock.begin();

    for( ; iter != m_mapBlock.end(); ++iter, ++pDstBlockInfo, ++nCnt )
    {
        if( size < nCnt) 
            break;

        memcpy( pDstBlockInfo, (*iter).second, sizeof(STRUCT_BLOCK_INFO) );
    }

    return nCnt;
}

INT User::CopyFriendInfo( STRUCT_FRIEND_INFO* pDstFriendInfo, const int size ) 
{
	INT nCnt = 0;

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	FRIEND_INFO_MAP_ITER iter = GetFriendMap()->begin();
	for( ; iter != GetFriendMap()->end(); ++iter, ++pDstFriendInfo, ++nCnt )
#else
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.begin();
	for( ; iter != m_mapFriend.end(); ++iter, ++pDstFriendInfo, ++nCnt )
#endif	
	{
		if( size < nCnt) 
			break;

		const STRUCT_FRIEND_INFO* pSrcFriendInfo = (*iter).second;
		memcpy( pDstFriendInfo, pSrcFriendInfo, sizeof(STRUCT_FRIEND_INFO) );

		if( pDstFriendInfo->sPacketInfo.bOnline )
		{
			const User* pOnlineFriend = UserManager::Instance()->FindUser( pDstFriendInfo->sPacketInfo.ptszFriendName );
			if( pOnlineFriend && 
				TRUE  == pOnlineFriend->IsGM() &&
				FALSE == IsGM() )
			{
				pDstFriendInfo->sPacketInfo.bOnline   = FALSE;
				pDstFriendInfo->sPacketInfo.byChannel = 0;
			}
		}
	}

	return nCnt;
}

INT User::CopyRevengeInfo( STRUCT_REVENGE_INFO* pDstRevengeInfo, const int size )
{
    INT nCnt = 0;
    REVENGE_INFO_MAP_ITER iter = m_mapRevenge.begin();

    for( ; iter != m_mapRevenge.end(); ++iter, ++pDstRevengeInfo, ++nCnt )
    {
        if( size < nCnt) 
            break;

        const STRUCT_REVENGE_INFO* pSrcRevengeInfo = (*iter).second;
        memcpy( pDstRevengeInfo, pSrcRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );

        if( pDstRevengeInfo->sPacketInfo.bOnline )
        {
            const User* pOnlineRevenge = UserManager::Instance()->FindUser( pDstRevengeInfo->sPacketInfo.ptszRevengeName );
            if( pOnlineRevenge && 
                TRUE  == pOnlineRevenge->IsGM() &&
                FALSE == IsGM() )
            {
                pDstRevengeInfo->sPacketInfo.bOnline = FALSE;
                pDstRevengeInfo->sPacketInfo.byChannelID = 0;
            }
        }
    }

    return nCnt;
}

// 삭제 _NA_0_20100527_ADD_CHAR_RELATION_MEMO
INT User::CopyRevengeInfo( STRUCT_REVENGE_PACKET_INFO* pDstRevengeInfo, const int size )
{
    INT nCnt = 0;
    REVENGE_INFO_MAP_ITER iter = m_mapRevenge.begin();

    for( ; iter != m_mapRevenge.end(); ++iter, ++pDstRevengeInfo, ++nCnt )
    {
        if( size < nCnt) 
            break;

        const STRUCT_REVENGE_INFO* pSrcRevengeInfo = (*iter).second;
        memcpy( pDstRevengeInfo, &pSrcRevengeInfo->sPacketInfo, sizeof(STRUCT_REVENGE_PACKET_INFO) );

        if( pDstRevengeInfo->bOnline )
        {
            const User* pOnlineRevenge = UserManager::Instance()->FindUser( pDstRevengeInfo->ptszRevengeName );
            if( pOnlineRevenge && 
                TRUE  == pOnlineRevenge->IsGM() &&
                FALSE == IsGM() )
            {
                pDstRevengeInfo->bOnline = FALSE;
                pDstRevengeInfo->byChannelID = 0;
            }
        }
    }

    return nCnt;
}

VOID User::SetStructMemberInfo( User* pUser, eGROUP_STAT eMemberStat )
{
    GroupManager::Instance()->InsertMemberInfo( pUser, &m_tempMemberInfo, eMemberStat );
}

VOID User::InitGroupInfo()
{
    //케릭터 로그 아웃시에 시스템 정보들을 초기화 한다. 
    GroupManager::Instance()->SendAroundMember( this, GroupManager::LOG_OUT);//초기화 하기전 처리 해야 한다.
    m_eHighMember      = GROUP_HAVE_NONE;
    m_eLowMember        = GROUP_HAVE_NONE;
    m_bGroupTrans       = FALSE;
    m_CharClass         = 0;
    m_bAddBlock         = FALSE;
    m_curBonusMoney     = 0;
    m_curBonusEXP       = 0;
    m_curAccumEXP       = 0;
    GroupManager::Instance()->RealeseMember( this );
    m_GroupMemberMap.clear();    
}

BYTE User::GetGroupMemberCount( )
{
    return static_cast<BYTE>(m_GroupMemberMap.size());
}
//}__CN_000001_20090513_ADD_FRIEND_CHECK_GM

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
RC::eFRIEND_RESULT User::CheckConditionAccessFriendInfo(CHARGUID friendguid)
{
    STRUCT_FRIEND_INFO* pFriendInfo = GetFriendMap()->FindFriend(friendguid);
    if (pFriendInfo != NULL)
        return RC::RC_FRIEND_SUCCESS;           // 친구

    STRUCT_REVENGE_INFO* pRevengeInfo = FindRevenge(friendguid);
    if (pRevengeInfo != NULL)
        return RC::RC_FRIEND_SUCCESS;           // 리벤지 대상

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = 
        GroupManager::Instance()->FindGroupMember(this, friendguid);

    if (pMemberInfo != NULL)
        return RC::RC_FRIEND_SUCCESS;           // 그룹원

    if (m_GuildGuid != INVALID_GUILDGUID)
    {
        WorldGuild* guild = WorldGuildManager::Instance()->FindGuild(m_GuildGuid);
        if (guild != NULL)
        {
            const WorldGuildMember* member = guild->FindMember(friendguid);
            if (member != NULL)
                return RC::RC_FRIEND_SUCCESS;   // 길드원 (한번이상 접속한 유저)  
            else
                return RC::RC_FRIEND_NOTGUILDMEMBER;   
        }
    }

    return RC::RC_FRIEND_NOTFRIEND;
}

//!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
void User::SetPremiumService( DWORD premiumservice )
{
    m_dwPremiumService = premiumservice;
}

DWORD User::GetPremiumService()
{
    return m_dwPremiumService;
}

bool User::CanApplyPCBangBenefit()
{
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    if (GetPremiumService() > 0) {
        return true;
    }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

    return (GetPCRoomID() > 0);
}

//!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM