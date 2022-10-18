#include "SunClientPrecompiledHeader.h"

#include "uiCommunityMan.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "uiCommunityInfo/uiCommunityInfo.h"
#include "uiCommunityReadMail/uiCommunityReadMail.h"
#include "uiCommunityWriteMail/uiCommunityWriteMail.h"
#include "uiCommunityChat/uiCommunityChat.h"
#include "uiGuildMan/uiGuildMan.h"
#include "interfaceManager.h"
#include "GameFramework.h"
#include "GlobalFunc.h"
#include "uiLoginMan/uiLoginMan.h"

#include "uiToolTipMan/uiToolTipMan.h"
#include "uiHeroGaugeMan/uiHeroGaugeMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "SoundEffect.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ResultCode.h"
#include "itemmanager.h"

#include "Hero.h"
#include "BattleScene.h"
#include "SolarDateTime.h"
#include "time.h"

#include "uiEventMan/uiEventMan.h"
#include "uiEventFriendWindow.h"
#include "uiConvenienceStore/uiConvenienceStore_FriendList.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/GuildSystemData.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

using namespace RC;

//------------------------------------------------------------------------------
/**
*/
uiCommunityMan::uiCommunityMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
    ,m_iCommunityMode(0)
{

#ifdef _YMS_UI_STATE_REMEMBER
    ZeroMemory(last_scrollbar_position_, sizeof(last_scrollbar_position_));
    last_community_mode_ = eCM_FRIEND;
    last_community_friend_subject_mode_ = eCM_FRIEND;
#endif

    m_pUICommunityDlg = NULL;
	m_pUICommunityInfo = NULL;
	m_pUICommunityReadMail = NULL;
	m_pUICommunityWriteMail = NULL;
	
	m_bNotDisplayOffLine	= false;
	m_bDisplayNewMail		= false;


	for(int i = eCM_FRIEND; i < eCM_MODEMAX; ++i)
	{
		m_iSortBy[i] = eCOMSORT_NAME;
		m_bOrderBy[i] = false;
	}
	m_iSortReady = eCOMSORT_BNONE;
	m_bIsOpenGroupJoinWinodw = FALSE;
	m_bRecvGroupMemberInfo = false;
	ZeroMemory(m_GroupName, sizeof(m_GroupName));

    onlined_friend_count_ = 0;
    onlined_group_count_ = 0;
	ZeroMemory(m_szRecvNameForMail, sizeof(m_szRecvNameForMail));

	SetRqstJoinGame(false);
	Clear();
}

void uiCommunityMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRelease()
{
	ClearFriendData();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::InitPKBooleans()
{
	this->m_vPKBool.clear();

	for (int i= ePKBOOL_COMMUNITY_BASE; i < ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ; ++i)
	{
		uicPacketControl packetControl;
		this->m_vPKBool.push_back(packetControl);
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Process(DWORD dwTick)
{
	if(IsWaitJoinGame() && m_PacketTimer.IsExpired())
	{
		SetWaitJoinGame(false);
	}

	CheckGroupLevel();		// 그룹 레벨 차이가 난다면 메시지를 출력
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Clear()
{
	m_bRejectGroup		= false;
	ZeroMemory(m_GroupName, sizeof(m_GroupName));

    m_bRejectChat		= false;
	ZeroMemory(&m_FriendInfo, sizeof(m_FriendInfo));

	SetMode(eCM_FRIEND);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Reset()
{
	if(m_pUICommunityDlg)
	{
		m_pUICommunityDlg->ShowInterface(TRUE, TRUE);
		m_pUICommunityDlg->Reset();
	}

    ZeroMemory(last_scrollbar_position_, sizeof(last_scrollbar_position_));
    set_last_community_mode(eCM_FRIEND);
    set_last_community_friend_subject_mode(eCM_FRIEND);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetMode(int iMode)
{
    if ((iMode >= eCM_FRIEND) && (iMode < eCM_MODEMAX))
    {
        m_iCommunityMode = iMode;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowCommunityDlg()
{
	if(m_pUICommunityDlg)
	{
		if(m_pUICommunityDlg->IsVisibleWindow())
			m_pUICommunityDlg->ShowInterface(FALSE);
		else
			m_pUICommunityDlg->ShowInterface(TRUE);
	}

}

void uiCommunityMan::OnCallBackMailDeleteInReadDialog( bool bYes )
{
    if (bYes)
    {	
        if( GetPendingDelMemoGuid())
        {
            SEND_CW_MEMO_DELETE_SYN( GetPendingDelMemoGuid() );
        }

        m_pUICommunityReadMail->CloseDlg();
    }

    SetPendingDelMemoGuid(0);
    SetSystemDialog(false);
}

//------------------------------------------------------------------------------
/**
*/
LEVELTYPE uiCommunityMan::GetLevel()
{
    FRIEND_DATA* friend_data= GetFriendData(m_FriendInfo.m_ptszCharName);
    if (friend_data != NULL)
    {
        return friend_data->lv;
    }

    return 0;
//
//	if(GetMode() == eCM_FRIEND)
//	{
//		for(int i=0;i<m_FriendArray.Size();++i)
//		{
//			if(!Strcmp(m_FriendArray[i].name.Get(),m_FriendInfo.m_ptszCharName))
//			{
//				return m_FriendArray[i].lv;
//			}
//		}
//	}
//
//	else if(GetMode() == eCM_REVENGE)
//	{
//		for(int i=0;i<m_RevengeArray.Size();++i)
//		{
//			if(!Strcmp(m_RevengeArray[i].name.Get(),m_FriendInfo.m_ptszCharName))
//			{
//				return m_RevengeArray[i].lv;
//			}
//		}
//	}
//	else if(GetMode() == eCM_GROUP)
//	{
//		for(int i=0;i<m_GroupArray.Size();++i)
//		{
//			if(!Strcmp(m_GroupArray[i].name.Get(),m_FriendInfo.m_ptszCharName))
//			{
//				return m_GroupArray[i].lv;
//			}
//		}
//	}
//
//	return 0;
}

#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
GenderType::Value uiCommunityMan::GetGender()
{
    FRIEND_DATA* friend_data= GetFriendData(m_FriendInfo.m_ptszCharName);
    if (friend_data != NULL)
    {
        return (GenderType::Value)friend_data->gender;
    }

    return GenderType::kFemale;
}
#endif  // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

//------------------------------------------------------------------------------
/**
*/
BYTE uiCommunityMan::GetClass()
{
    FRIEND_DATA* friend_data= GetFriendData(m_FriendInfo.m_ptszCharName);
    if (friend_data != NULL)
    {
        return friend_data->bClassCode;
    }

    return 0;
}


//------------------------------------------------------------------------------
/**
*/
CHARGUID uiCommunityMan::GetGUID(TCHAR* character_name)
{
    if ((m_pUICommunityInfo != NULL) && (m_pUICommunityInfo->request_uimanager_id() == UIMAN_GUILD))
    {
    #ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GUILD_MEMBER_INFO* guild_member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_name);
        if (guild_member_info != NULL)
        {
            return guild_member_info->m_CharGuid;
        }
    #else
        uiGuildMan* guild_manager = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_manager != NULL)
        {
            stGUILD_MEMBER_INFO* member_info = guild_manager->GetGuildMemberInfo(character_name);
            if (member_info != NULL)
            {
                return member_info->m_CharGuid;
            }
        }
    #endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        return 0;
    }

    FRIEND_DATA* friend_data= GetFriendData(character_name);
    if (friend_data != NULL)
    {
        return friend_data->dwFriendGuid;
    }

    return 0;
}

//------------------------------------------------------------------------------
FRIEND_DATA*  uiCommunityMan::GetFriendData(TCHAR* character_name)
{
    if (character_name == NULL)
    {
        return NULL;
    }
    if(GetMode() == eCM_FRIEND)
    {
        for(int i=0;i<m_FriendArray.Size();++i)
        {
            if(!Strcmp(m_FriendArray[i].name.Get(), character_name))
            {
                return &(m_FriendArray[i]);
            }
        }
    }
    else if(GetMode() == eCM_REVENGE)
    {
        for(int i=0;i<m_RevengeArray.Size();++i)
        {
            if(!Strcmp(m_RevengeArray[i].name.Get(), character_name))
            {
                return &(m_RevengeArray[i]);
            }
        }
    }

    else if(GetMode() == eCM_GROUP)
    {
        for(int i=0;i<m_GroupArray.Size();++i)
        {
            if(!Strcmp(m_GroupArray[i].name.Get(), character_name))
            {
                return &(m_GroupArray[i]);
            }
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
FRIEND_DATA*  uiCommunityMan::GetFriendData(CHARGUID character_guid)
{
    if (character_guid <= 0)
    {
        return NULL;
    }
    if (GetMode() == eCM_FRIEND)
    {
        for (int i = 0; i < m_FriendArray.Size(); ++i)
        {
            if (m_FriendArray[i].dwFriendGuid == character_guid)
            {
                return &(m_FriendArray[i]);
            }
        }
    }
    else if (GetMode() == eCM_REVENGE)
    {
        for (int i = 0; i < m_RevengeArray.Size(); ++i)
        {
            if (m_RevengeArray[i].dwFriendGuid == character_guid)
            {
                return &(m_RevengeArray[i]);
            }
        }
    }
    else if(GetMode() == eCM_GROUP)
    {
        for (int i = 0; i < m_GroupArray.Size(); ++i)
        {
            if (m_GroupArray[i].dwFriendGuid == character_guid)
            {
                return &(m_GroupArray[i]);
            }
        }
    }
    return NULL;
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Send_CW_GROUP_STATUS_REQ(CHAR *ptszCharName)
{
	this->SetPendingInfoMode(GetMode());

	MSG_CW_GROUP_STATUS_REQ sync;

	ZeroMemory(&sync.m_ptszCharName, sizeof(sync.m_ptszCharName));
	StrnCopy(sync.m_ptszCharName, ptszCharName, MAX_CHARNAME_LENGTH);

	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupInfo(FRIEND_INFO_DATA info)
{
	CopyMemory(&m_GroupInfo, &info, sizeof(FRIEND_INFO_DATA));
}

//------------------------------------------------------------------------------
/** 쪽지내용을 파싱해서 어떤메모타입인지 체크한다.
*/
INT64 uiCommunityMan::ParseMemoInfo(char *pStr,int nIndex,int nBufSize, unsigned int uBufSize, char *pDest)
{
	if(!pStr) return -1; 

	char szTemp[INTERFACE_STRING_LENGTH+1]={0,};
	
	int nNeedIndex= 0;
	int nStep=0;

	for(int i=0;i<nBufSize;++i)
	{
		//마지막문자열이면 무조건 끝낸다
		if(pStr[i]=='\0')
		{
			szTemp[nStep] = pStr[i];
			if(pDest && uBufSize > 0)
			{
				StrnCopy(pDest, szTemp, uBufSize);
			}
			INT64 nNum = _atoi64(szTemp);
			return nNum;
		}

		//구분자 체크
		if(pStr[i]=='|')
		{	
			//원하던인덱스를찾았다면 변환후 리턴해준다.
			if(nNeedIndex==nIndex)
			{
				szTemp[i] = '\0';
				if(pDest && uBufSize > 0)
				{
					StrnCopy(pDest, szTemp, uBufSize);
				}
				INT64 nNum = _atoi64(szTemp);
				_asm nop;
				return nNum;
			}
			else
			{
				++nNeedIndex;
				nStep = 0;
				memset(szTemp,0,sizeof(szTemp));
				continue;
			}
		}
		szTemp[nStep] = pStr[i];
		++nStep;
	}
	return -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Send_CW_FRIEND_STATUS_REQ(CHAR *ptszCharName, CHARGUID char_guid)
{
	this->SetPendingInfoMode(GetMode());

	MSG_CW_FRIEND_STATUS_REQ sync;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (char_guid <= 0)
    {
        return;
    }
    sync.m_dwToCharGuid = char_guid;
#else
    ZeroMemory(&sync.m_ptszCharName, sizeof(sync.m_ptszCharName));
    StrnCopy(sync.m_ptszCharName, ptszCharName, MAX_CHARNAME_LENGTH);
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO


	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::Send_CW_FRIEND_STATUS_JOIN_SYN()
{
	if(m_FriendInfo.m_byZoneType == eZONETYPE_LOBBY)
	{
		MSG_CW_FRIEND_STATUS_JOIN_SYN sync;

		ZeroMemory(&sync.m_ptszCharName, sizeof(sync.m_ptszCharName));
		StrnCopy(sync.m_ptszCharName, m_FriendInfo.m_ptszCharName, MAX_CHARNAME_LENGTH);

		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
	}
	else
	{
		// 5450	로비에 있는 친구에게만 사용할 수 있습니다.
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(5450, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetFriendInfo(FRIEND_INFO_DATA info)
{
	CopyMemory(&m_FriendInfo, &info, sizeof(FRIEND_INFO_DATA));
}

//------------------------------------------------------------------------------
/**
*/
bool uiCommunityMan::IsMyChannel()
{
	uiLoginMan *pLoginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if (pLoginMan)
	{
		AuthSetverData* pServerData = pLoginMan->GetCurrentServerData();
		if (pServerData->m_ChannelID == m_FriendInfo.m_byChannel)
			return true;
	}

	return false;
}

//------------------------------------------------------------------------------
/**
*/
BYTE uiCommunityMan::GetMyChannel()
{
	uiLoginMan *pLoginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if (pLoginMan)
	{
		AuthSetverData* pServerData = pLoginMan->GetCurrentServerData();
		if (pServerData)
			return pServerData->m_ChannelID;
	}

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
LPCTSTR	uiCommunityMan::GetMyServer()
{
	uiLoginMan *pLoginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
	if (pLoginMan)
	{
		AuthSetverData* pServerData = pLoginMan->GetCurrentServerData();
		if (pServerData)
			return pServerData->m_GroupName.c_str();
	}

	return NULL;
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetWaitJoinGame(bool bWait)
{ 
	m_bWaitJoinGame = bWait;	

	if(bWait)
	{
		m_PacketTimer.SetTimer(60000);	// 60초 대기
	}
	else
	{
		m_PacketTimer.Clear();
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ClearFriendData()
{
	m_bRecvMemberInfo	= false;
	m_iNewFriendChat	= 0;

	m_bRecvMailInfo		= false;

	m_vecMailData.clear();

	m_chatDlgArray.Clear();
	m_FriendArray.Clear();
	m_BlockFriendArray.Clear();
	m_RevengeArray.Clear();

	m_bRecvGroupMemberInfo = false;
	m_GroupArray.Clear();

	m_byRevengePoint	= 0;

	SetWaitJoinGame(false);

	ClearPendingData();
    onlined_friend_count_ = 0;
    onlined_group_count_ = 0;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ClearSelection()
{
	m_iCurFriendSelect		= -1;
	m_iCurBlockSelect		= -1;
    m_iCurRevengeSelect		= -1;
	m_iCurMailSelect		= -1;
	m_iCurGroupSelect		= -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ClearPendingData()
{
	ZeroMemory(m_szPendingDelFriend, sizeof(m_szPendingDelFriend));
	ZeroMemory(m_szPendingDelBlock, sizeof(m_szPendingDelBlock));
	ZeroMemory(m_szPendingDelRevenge, sizeof(m_szPendingDelRevenge));
	
	m_iPendingInfoMode	= eCM_FRIEND;

	m_PendingDelMemoGuid = 0;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetPendingDelFriend(TCHAR* pName)	
{ 
	if(pName == NULL)
		ZeroMemory(m_szPendingDelFriend, sizeof(m_szPendingDelFriend));
	else
		StrnCopy(m_szPendingDelFriend, pName, MAX_CHARNAME_LENGTH);	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetPendingDelBlock(TCHAR* pName)	
{ 
	if(pName == NULL)
		ZeroMemory(m_szPendingDelBlock, sizeof(m_szPendingDelBlock));
	else
		StrnCopy(m_szPendingDelBlock, pName, MAX_CHARNAME_LENGTH);	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetPendingDelRevenge(TCHAR* pName)	
{ 
	if(pName == NULL)
		ZeroMemory(m_szPendingDelRevenge, sizeof(m_szPendingDelRevenge));
	else
		StrnCopy(m_szPendingDelRevenge, pName, MAX_CHARNAME_LENGTH);	
}
	
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetPendingDelGroup(TCHAR* pName)	
{ 
	if(pName == NULL)
		ZeroMemory(m_szPendingDelGroup, sizeof(m_szPendingDelGroup));
	else
		StrnCopy(m_szPendingDelGroup, pName, MAX_CHARNAME_LENGTH);	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddFriend( const STRUCT_FRIEND_PACKET_INFO& friend_info, DWORD guild_id )
{
    FRIEND_DATA data;
    data.name		= friend_info.ptszFriendName;
    data.bOnline	= friend_info.bOnline;
    data.lv			= friend_info.Level;
    data.bClassCode	= friend_info.byClass;
    data.byChannel  = friend_info.byChannel;
    data.dwFriendGuid = guild_id;
    data.byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_FRIEND;	// INFO_FRIEND: 일반 친구

#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    data.gender     = friend_info.gender;
#endif //_NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

    m_FriendArray.PushBack(data);
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::DeleteFriend(TCHAR *pName)
{
	for(int i=0;i<m_FriendArray.Size();++i)
	{
		if(!Strcmp(m_FriendArray[i].name.Get(),pName))
		{
			int index = FindChatDialog(pName);
			if(index != -1)
			{
				DeleteChatDlgId(m_chatDlgArray[index].dwDlgId);
			}

			m_FriendArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindFriend(TCHAR *pName)
{
	for(int i=0;i<m_FriendArray.Size();++i)
	{
		if(!Strcmp(m_FriendArray[i].name.Get(),pName))
		{
			return i;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindBlockFriend(TCHAR *pName)
{
	for(int i=0;i<m_BlockFriendArray.Size();++i)
	{
		if(!Strcmp(m_BlockFriendArray[i].name.Get(),pName))
		{
			return i;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetFriendOnline(TCHAR *pName, BOOL bOn, BYTE byChannel)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindFriend(szWideString);

	if(index != -1)
	{
		m_FriendArray[index].bOnline = bOn;
		m_FriendArray[index].byChannel = byChannel;
        if (bOn != 0)
        {
            ++onlined_friend_count_;
        }
        else
        {
            --onlined_friend_count_;
        }
        
        if (GetMode() == eCM_FRIEND)
        {
            if (m_pUICommunityDlg)
            {
                m_pUICommunityDlg->RefreshOnlineMember(eCM_FRIEND);
            }
        }
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetFriendLevel(TCHAR *pName, LEVELTYPE lv)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindFriend(szWideString);

	if(index != -1)
	{
		m_FriendArray[index].lv = lv;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddBlockFriend(TCHAR *pName)
{
	FRIEND_DATA data;

	data.name = pName;

	int index = FindChatDialog(pName);
	if(index != -1)
	{
		DeleteChatDlgId(m_chatDlgArray[index].dwDlgId);
	}

	m_BlockFriendArray.PushBack(data);

    //차단된 캐릭터가 친구라면 친구관계 설정 변경
    for (int i=0;i<m_FriendArray.Size();++i)
	{
		if (!Strcmp(m_FriendArray[i].name.Get(), pName))
		{
			if (m_FriendArray[i].byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_INVIAT)                 // 친구관계 친구라면
            {
                m_FriendArray[i].byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_BLOCKED_INVIAT;         // 차단된 친구관계
            }
            else if (m_FriendArray[i].byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED)        // 친구관계 친구라면
            {
                m_FriendArray[i].byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_BLOCKED_BEINVIATED;     // 차단된 친구관계
            }
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::DeleteBlockFriend(TCHAR *pName)
{
	for(int i=0;i<m_BlockFriendArray.Size();++i)
	{
		if(!Strcmp(m_BlockFriendArray[i].name.Get(),pName))
		{
			m_BlockFriendArray.Erase(i);
			return TRUE;
		}
	}

    //차단된 캐릭터가 친구라면 친구관계 설정 변경
    for(int i=0;i<m_FriendArray.Size();++i)
	{
		if(!Strcmp(m_FriendArray[i].name.Get(),pName))
		{
			if(m_FriendArray[i].byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_BLOCKED_INVIAT)             // 친구관계 친구라면
            {
                m_FriendArray[i].byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_INVIAT;                     // 차단된 친구관계
            }
            else if(m_FriendArray[i].byFriendSts == STRUCT_FRIEND_DB_INFO::INFO_BLOCKED_BEINVIATED)    // 친구관계 친구라면
            {
                m_FriendArray[i].byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED;                 // 차단된 친구관계
            }
		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::IsOnline(TCHAR *pName)
{
    if (m_pUICommunityInfo->request_uimanager_id() == UIMAN_GUILD)
    {
    #ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GUILD_MEMBER_INFO* guild_member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(pName);
        if (guild_member_info != NULL)
        {
            return guild_member_info->m_bLogin;
        }
    #else
        uiGuildMan* guild_manager = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_manager)
        {
            stGUILD_MEMBER_INFO* member_info = guild_manager->GetGuildMemberInfo(pName);
            if (member_info)
            {
                return member_info->m_bLogin;
            }
        }
    #endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        return false;
    }

	if(GetMode() == eCM_FRIEND)
	{
		for(int i=0;i<m_FriendArray.Size();++i)
		{
			if(!Strcmp(m_FriendArray[i].name.Get(),pName))
			{
				return m_FriendArray[i].bOnline;
			}
		}
	}
	else if(GetMode() == eCM_REVENGE)
	{
		for(int i=0;i<m_RevengeArray.Size();++i)
		{
			if(!Strcmp(m_RevengeArray[i].name.Get(),pName))
			{
				return m_RevengeArray[i].bOnline;
			}
		}
	}
	else if(GetMode() == eCM_GROUP)
	{
		for(int i=0;i<m_GroupArray.Size();++i)
		{
			if(!Strcmp(m_GroupArray[i].name.Get(),pName))
			{
				return m_GroupArray[i].bOnline;
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BYTE uiCommunityMan::IsFriendSts(TCHAR *pName)
{
	if(GetMode() == eCM_FRIEND)
	{
		for(int i=0;i<m_FriendArray.Size();++i)
		{
			if(!Strcmp(m_FriendArray[i].name.Get(),pName))
			{
				return m_FriendArray[i].byFriendSts;
			}
		}
	}
	return FALSE;
}
//------------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityMan::GetSelectString()
{
	if(GetMode() == eCM_FRIEND)			// 친구
	{
		return GetFriendSelectString();
	}
	else if(GetMode() == eCM_BLOCK)	// 차단
	{
		return GetBlockFriendSelectString();
	}
	else if(GetMode() == eCM_GROUP)	// 그룹
	{
		return GetGroupSelectString();
	}
	else							// 리벤지
	{
		return GetRevengeSelectString();
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityMan::GetFriendSelectString()
{
	if(m_pUICommunityDlg)
	{
		m_iCurFriendSelect = m_pUICommunityDlg->GetCurListSel();

		if((m_iCurFriendSelect >= 0) && (m_iCurFriendSelect < m_FriendArray.Size()))
		{
			FRIEND_DATA *pData = m_pUICommunityDlg->GetCurData(m_iCurFriendSelect);
			if(pData)
			{
				return (TCHAR *)pData->name.Get();
			}
			else
			{
				return NULL;
			}
		}
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityMan::GetBlockFriendSelectString()
{
	if(m_pUICommunityDlg)
	{
		m_iCurBlockSelect = m_pUICommunityDlg->GetCurListSel() + m_pUICommunityDlg->GetCurScrollCount();

		if((m_iCurBlockSelect >= 0) && (m_iCurBlockSelect < m_BlockFriendArray.Size()))
		{
			if (m_BlockFriendArray.Size())
			{
				assert(m_iCurBlockSelect<m_BlockFriendArray.Size());
				return (TCHAR *)m_BlockFriendArray[m_iCurBlockSelect].name.Get();
			}
			else
			{
				return NULL;
			}
		}
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::DeleteChatDlgId(DWORD dwId)
{
	for(int i=0;i<m_chatDlgArray.Size();++i)
	{
		if(m_chatDlgArray[i].dwDlgId==dwId)
		{
			m_chatDlgArray.Erase(i);

			SolarDialog *pDlg=g_InterfaceManager.GetDialog((WzID)dwId);
			if(pDlg)
			{
				pDlg->ShowDialog(FALSE);
				pDlg->SetDestroyWindow(TRUE);
			}
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddFriendChatData(FRIEND_CHAT_DATA & data)
{
	m_chatDlgArray.Append(data);
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindChatDialog(TCHAR *pName)
{
	for(int i=0;i<m_chatDlgArray.Size();++i)
	{
		if(!Strcmp(m_chatDlgArray[i].name.Get(),pName))
		{
			if(m_chatDlgArray[i].dwDlgId == -1)
			{
				return -1;
			}
			else
			{
				return i;
			}
		}
	}

	return -1;
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::RemoveAllChatDialog()
{
	for(int i=0;i<m_chatDlgArray.Size();++i)
	{
		SolarDialog *pDlg = g_InterfaceManager.GetDialog((WzID)m_chatDlgArray[i].dwDlgId);
		if(pDlg)
		{
			pDlg->ShowDialog(FALSE);
			pDlg->SetDestroyWindow(TRUE);
		}
	}
	
	m_iNewFriendChat = 0;
	m_chatDlgArray.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetMessageRejection(BOOL bRejection)
{
	m_bRejectChat = (bRejection != FALSE);
	
	if(bRejection)
	{
		RemoveAllChatDialog();
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_FRIEND_NEWLIST_SYN()
{
	if(!m_bRecvMemberInfo)
	{
		//친구 리스트요청 
		MSG_CW_FRIEND_NEWLIST_SYN sync;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));

		//메시지 거부상태 세팅
		MSG_CW_FRIEND_CHATBLOCK_SYN sync2;

		sync2.m_bBlock = m_bRejectChat;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync2, sizeof (sync2));	
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowRevengeResultCode(BYTE bError)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if(bError==RC_REVENGE_FAILED)
	{
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("알수 없는 에러") );

	}
	else if(bError==RC_REVENGE_NO_POINT)
	{
		// 리벤지 포인트가 부족합니다 5276
		g_InterfaceManager.GetInterfaceString(eST_INSUFFICIENT_REVENGE_POINT ,szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
	}
    else if(bError==RC_REVENGE_INVALID_CHAR)
	{
		// 리벤지 대상이 아닙니다. 5277
		g_InterfaceManager.GetInterfaceString(eST_INVALID_REVENGE_PERSON ,szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
	else if(bError==RC_REVENGE_INVALID_SITUATION)
	{
		//리벤지 가능한 상황이 아닙니다. 5278
		g_InterfaceManager.GetInterfaceString(eST_INVALID_REVENGE_STATUS ,szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

	}
	else if(bError==RC_REVENGE_NO_CHAR)
	{
		// 대상이 존재하지 않습니다. 5279
		g_InterfaceManager.GetInterfaceString(eST_NO_EXIST_REVENGE_PERSON ,szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddRevenge(TCHAR *pName, LEVELTYPE lv, BYTE bClassCode, BOOL bOnline, 
                                BYTE byChannel, CHARGUID character_guid)
{
	FRIEND_DATA data;

	data.name		= pName;
	data.bOnline	= bOnline;
	data.lv			= lv;
	data.bClassCode	= bClassCode;
	data.byChannel	= byChannel;
	data.byFriendSts = 1;//일반친구
    data.dwFriendGuid = character_guid;

	m_RevengeArray.PushBack(data);
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::DeleteRevenge(TCHAR *pName)
{
	for(int i=0;i<m_RevengeArray.Size();++i)
	{
		if(!Strcmp(m_RevengeArray[i].name.Get(),pName))
		{
			int index = FindChatDialog(pName);
			if(index != -1)
			{
				DeleteChatDlgId(m_chatDlgArray[index].dwDlgId);
			}

			m_RevengeArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindRevenge(TCHAR *pName)
{
	for(int i=0;i<m_RevengeArray.Size();++i)
	{
		if(!Strcmp(m_RevengeArray[i].name.Get(),pName))
		{
			return i;
		}
	}

	return -1;
}

void uiCommunityMan::SetRevengeOnline(TCHAR *pName, BOOL bOn, BYTE byChannel)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindRevenge(szWideString);
	
	if(index != -1)
	{
		m_RevengeArray[index].bOnline = bOn;
		m_RevengeArray[index].byChannel = byChannel;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetRevengeLevel(TCHAR *pName, LEVELTYPE lv)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindRevenge(szWideString);

	if(index != -1)
	{
		m_RevengeArray[index].lv = lv;
	}

}

//------------------------------------------------------------------------------
/**
*/
TCHAR *	uiCommunityMan::GetRevengeSelectString()
{
	if(m_pUICommunityDlg)
	{
		m_iCurRevengeSelect = m_pUICommunityDlg->GetCurListSel();

		if((m_iCurRevengeSelect >= 0) && (m_iCurRevengeSelect < m_RevengeArray.Size()))
		{
			FRIEND_DATA *pData = m_pUICommunityDlg->GetCurData(m_iCurRevengeSelect);
			if(pData)
			{
				return (TCHAR *)pData->name.Get();
			}
			else
			{
				return NULL;
			}
			
		}
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddGroup(TCHAR *pName, LEVELTYPE lv, BYTE bClassCode, BOOL bOnline, 
                              BYTE byChannel, BYTE byMaster, INT64 i64LastDate,
                              CHARGUID character_guid)
{
	FRIEND_DATA data;

	data.name		= pName;
	data.bOnline	= bOnline;
	data.lv			= lv;
	data.bClassCode	= bClassCode;
	data.byChannel	= byChannel;
	data.byMaster	= byMaster;
	data.lastDate   = i64LastDate;
	data.dwFriendGuid = character_guid;	// 그룹은 guid를 쓰지 않는다
	data.byFriendSts = STRUCT_FRIEND_DB_INFO::INFO_FRIEND;	// INFO_FRIEND: 일반 친구
	m_GroupArray.PushBack(data);
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityMan::DeleteGroup(TCHAR *pName)
{
	for(int i=0;i<m_GroupArray.Size();++i)
	{
		if(!Strcmp(m_GroupArray[i].name.Get(),pName))
		{
			int index = FindChatDialog(pName);
			if(index != -1)
			{
				DeleteChatDlgId(m_chatDlgArray[index].dwDlgId);
			}
			m_GroupArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupOnline(TCHAR *pName, BOOL bOn, BYTE byChannel)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindGroup(szWideString);

	if(index != -1)
	{
		m_GroupArray[index].bOnline = bOn;
		m_GroupArray[index].byChannel = byChannel;
		m_GroupArray[index].lastDate = GetNowTimeINT64();// 로그아웃시간 설정(현재시간으로)
        if (bOn != 0)
        {
            ++onlined_group_count_;
        }
        else
        {
            --onlined_group_count_;
        }
        
        if (GetMode() == eCM_GROUP)
        {
            if (m_pUICommunityDlg)
            {
                m_pUICommunityDlg->RefreshOnlineMember(eCM_GROUP);
            }
        }
	}
}

//------------------------------------------------------------------------------
/**
*/
INT64 uiCommunityMan::GetNowTimeINT64()
{
	DWORD64 time = 0;
	return (INT64)GetDateTime_YYYYMMDDHHMM(time);
}

//------------------------------------------------------------------------------
/**
*/
bool uiCommunityMan::CheckGroupLevel()
{
	bool bRes = false;
	if(m_GroupMessageTimer.GetRemainedTime() <= 0)	// 시간이 끝났다면
	{
		TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szVal[INTERFACE_STRING_LENGTH + 1] = {0,};

		for(int i = 0; i < m_GroupArray.Size(); ++i)
		{
			if(g_pHero && (m_GroupArray[i].lv >= g_pHero->GetLevel()) && m_GroupArray[i].byMaster == GROUP_LEVEL_LOW)
			{
				// 메시지를 출력하고
				//  %s님의 레벨이 동일하거나 더 높아 그룹 효과를 받을 수 없습니다.
				StrnCopy(szWideString, m_GroupArray[i].name.Get(), MAX_CHARNAME_LENGTH);
				g_InterfaceManager.GetInterfaceString(7044, szMessage, INTERFACE_STRING_LENGTH);
				//
				Sprintf(szVal, szMessage, szWideString);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);

				bRes = true;
			}
		}
		m_GroupMessageTimer.SetTimer(GROUP_MESSAGE_TIMER);	// 설정 10분
	}
	return bRes;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::CheckGroupLevelfromHero()
{
	bool bRes = true;
	for(int i = 0; i < m_GroupArray.Size(); ++i)
	{
		if(g_pHero && m_GroupArray[i].lv >= g_pHero->GetLevel() && m_GroupArray[i].byMaster == GROUP_LEVEL_LOW)
		{
			bRes = false;	// 한명이라도 나보다 레벨이 높거나 같으면 Timer를 지우지 않는다.
			break;
		}
	}
	if(bRes)
	{
		ClearGroupTimer();
	}
	else
	{
		if(m_GroupMessageTimer.GetRemainedTime() <= 0)	// 타이머가 없는 경우
		{
			SetGroupTimer(1000);	// 타이머를 켠다. 1초
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupTimer(DWORD dwTime)
{
	m_GroupMessageTimer.SetTimer(dwTime);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ClearGroupTimer()
{
	m_GroupMessageTimer.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupLevel(TCHAR *pName, LEVELTYPE lv)
{
	TCHAR szWideString[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy(szWideString, pName, MAX_CHARNAME_LENGTH);

	int index = FindGroup(szWideString);

	if(index != -1)
	{
		m_GroupArray[index].lv = lv;
	}

}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindGroup(TCHAR *pName)
{
	for(int i=0;i<m_GroupArray.Size();++i)
	{
		if(!Strcmp(m_GroupArray[i].name.Get(),pName))
		{
			return i;
		}
	}

	return -1;
}


//------------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityMan::GetGroupSelectString()
{
	if(m_pUICommunityDlg)
	{
		m_iCurGroupSelect = m_pUICommunityDlg->GetCurListSel();

		if((m_iCurGroupSelect >= 0) && (m_iCurFriendSelect < m_GroupArray.Size()))
		{
			FRIEND_DATA *pData = m_pUICommunityDlg->GetCurData(m_iCurGroupSelect);
			if(pData)
			{
				return (TCHAR *)pData->name.Get();
			}
			else
			{
				return NULL;
			}
		}
	}

	return NULL;
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_GROUP_RESTORELIST_SYN()
{
	//그룹 정보 복구요청 
	if(m_bRecvGroupMemberInfo)	// true일땐 존 이동시만
	{
		MSG_CW_GROUP_RESTOR_CMD sync;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_GROUP_NEWLIST_SYN()
{
	if(!m_bRecvGroupMemberInfo) // false일땐 최초시만
	{
		//그룹 리스트요청 
		MSG_CW_GROUP_GROUPLIST_REQ sync;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));

		//그룹 신청 거부상태 세팅
		MSG_CW_GROUP_ADDBLOCK_SYN sync2;
		sync2.m_bBlock = m_bRejectGroup;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync2, sizeof (sync2));	
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupName(char *pName)
{
	StrnCopy(m_GroupName, pName, MAX_CHARNAME_LENGTH);
}

//------------------------------------------------------------------------------
/**
*/
const char *uiCommunityMan::GetGroupName()
{
	return (const char*) m_GroupName;
}
//------------------------------------------------------------------------------
/**
*/
void _Confirm_ReponseJoinGroup(bool bYes)
{
	uiCommunityMan * pMan = (uiCommunityMan * ) g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY);
	assert(pMan);

	if (bYes)
    {
		pMan->SetOpenGroupJoinWindow(FALSE);
        MSG_CW_GROUP_ADD_RESPONSE_ACK sync;
        ZeroMemory(sync.ptszMemeberName, sizeof(sync.ptszMemeberName));
		StrnCopy(sync.ptszMemeberName, pMan->GetGroupName(), MAX_CHARNAME_LENGTH);

        GlobalFunc::SendPacketToWorldServer(&sync, sizeof (sync));
    }
    else
    {
		pMan->SetOpenGroupJoinWindow(FALSE);
        MSG_CW_GROUP_ADD_RESPONSE_NAK sync;
        ZeroMemory(sync.ptszMemeberName, sizeof(sync.ptszMemeberName));
		StrnCopy(sync.ptszMemeberName, pMan->GetGroupName(), MAX_CHARNAME_LENGTH);
		sync.m_byResult = MSG_CW_GROUP_ADD_RESPONSE_NAK::IGNOR_ADD_MEMBER;

        GlobalFunc::SendPacketToWorldServer(&sync, sizeof (sync));
    }
	
    if( g_pHero )
    {
        g_pHero->SetWaitingInviateGroup(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ResponseJoinGroup(char *pszMasterName)
{
	// 이미 그룹창이 열려 있으면 리턴
	if( m_bIsOpenGroupJoinWinodw )
		return;

	m_bIsOpenGroupJoinWinodw = TRUE;
	DWORD dwUIiD = 0;

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szResult[INTERFACE_STRING_LENGTH] = {0,};

	// 7032 : %s님으로부터 그룹 멤버 등록 권유 메시지가 도착했습니다.
	g_InterfaceManager.GetInterfaceString(7032,szMessage,INTERFACE_STRING_LENGTH);
    Snprintf (szResult,INTERFACE_STRING_LENGTH-1, szMessage, pszMasterName);

	SetGroupName(pszMasterName);

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
	{
		pSystemMan->Sys_Confirm(szResult, _Confirm_ReponseJoinGroup, NULL, NULL, 100, 101, false, 15000);
	}

    if( g_pHero )
    {
        g_pHero->SetWaitingInviateGroup(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetGroupRejection(BOOL bRejection)
{
	m_bRejectGroup = (bRejection != FALSE);
}

//------------------------------------------------------------------------------
/** 기본 소트
*/
bool _BaseSort_Name(const FRIEND_DATA &a, const FRIEND_DATA &b, bool bOrderBy)
{
	if(bOrderBy)
	{
		return (a.name < b.name);
	}
	else
	{
		return (a.name >= b.name);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_Lv(const FRIEND_DATA &a, const FRIEND_DATA &b, bool bOrderBy)
{
	if(a.lv == b.lv)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.lv < b.lv);
	}
	else
	{
		return  (a.lv >= b.lv);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_Class(const FRIEND_DATA &a, const FRIEND_DATA &b, bool bOrderBy)
{
	int j;
	TCHAR szTemp[2][INTERFACE_STRING_LENGTH] = {0,};

	for(int i = 0; i < 2; ++i)
	{
		if(i == 0)
			j = a.bClassCode;
		else if(i == 1)
			j = b.bClassCode;
		else
			j = -1;

        Snprintf(szTemp[i], 
            INTERFACE_STRING_LENGTH, 
            _T("%s"), 
            GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(j)));
	}

	if(strcmp(szTemp[0], szTemp[1])==0)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return (strcmp(szTemp[0], szTemp[1])<0);
	}
	else
	{
		return (strcmp(szTemp[0], szTemp[1])>=0);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_byChannel(const FRIEND_DATA &a, const FRIEND_DATA &b, bool bOrderBy)
{
	if(a.byChannel == b.byChannel)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return (a.byChannel < b.byChannel);
	}
	else
	{
		return (a.byChannel >= b.byChannel);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_bOnline(const FRIEND_DATA &a, const FRIEND_DATA &b, bool bOrderBy)
{
	if(a.byChannel == b.byChannel)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return (a.bOnline < b.bOnline);
	}
	else
	{
		return (a.bOnline >= b.bOnline);
	}
}
//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_NameMail(const MAIL_DATA &a, const MAIL_DATA &b, bool bOrderBy)
{
	if(!strcmp(a.m_subject.m_SendName, b.m_subject.m_SendName))	// 같은 경우 다시 날짜순으로 정렬
	{
		return (0 < strcmp(a.m_subject.m_tszCreateDate, b.m_subject.m_tszCreateDate));
	}

	if(bOrderBy)
	{
		return (0 < strcmp(a.m_subject.m_SendName, b.m_subject.m_SendName));
	}
	else
	{
		return (0 > strcmp(a.m_subject.m_SendName, b.m_subject.m_SendName));
	}
}
//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_DateMail(const MAIL_DATA &a, const MAIL_DATA &b, bool bOrderBy)
{
	if(!strcmp(a.m_subject.m_tszCreateDate, b.m_subject.m_tszCreateDate))	// 같은 경우 다시 이름순으로 정렬
	{
		return (0 < strcmp(a.m_subject.m_SendName, b.m_subject.m_SendName));
	}

	if(bOrderBy)
	{
		return (0 < strcmp(a.m_subject.m_tszCreateDate, b.m_subject.m_tszCreateDate));
	}
	else
	{
		return (0 > strcmp(a.m_subject.m_tszCreateDate, b.m_subject.m_tszCreateDate));
	}
}
//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_MemoMail(const MAIL_DATA &a, const MAIL_DATA &b, bool bOrderBy)
{
	if(!strcmp(a.m_subject.m_ptszSubject, b.m_subject.m_ptszSubject))	// 같은 경우 다시 날짜순으로 정렬
	{
		return (0 < strcmp(a.m_subject.m_tszCreateDate, b.m_subject.m_tszCreateDate));
	}

	if(bOrderBy)
	{
		return (0 < strcmp(a.m_subject.m_ptszSubject, b.m_subject.m_ptszSubject));
	}
	else
	{
		return (0 > strcmp(a.m_subject.m_ptszSubject, b.m_subject.m_ptszSubject));
	}
}
//------------------------------------------------------------------------------
/** 오름차순 GetOrderBy()==true / 내림차순  GetOrderBy()==false
*/
uiCommunityMan* GetCommunityMan()
{
	static uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
	return pCommMan;
}

bool _Sort_Name(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
	return _BaseSort_Name(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _Sort_Lv(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
    return _BaseSort_Lv(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _Sort_Class(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
	return _BaseSort_Class(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _Sort_byChannel(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
	return _BaseSort_byChannel(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _Sort_bOnline(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
	return _BaseSort_bOnline(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SortbyList(nArray<FRIEND_DATA> &Array, int iSortBy, bool OrderBy)
{
	bool (*fpSortBy)(const FRIEND_DATA &a, const FRIEND_DATA &b);

	// (OrderBy) true 오름차순, false 내림차순

	SetOrderBy(GetMode(), OrderBy);

	switch(iSortBy)
	{
	case eCOMSORT_NAME:
		fpSortBy = _Sort_Name;
		break;
	case eCOMSORT_LV:
		fpSortBy = _Sort_Lv;
		break;
	case eCOMSORT_CLASS:
		fpSortBy = _Sort_Class;
		break;
	case eCOMSORT_CHANNEL:
		fpSortBy = _Sort_byChannel;
		break;
	case eCOMSORT_ONLINE:
		fpSortBy = _Sort_bOnline;
		break;
	default:
		fpSortBy = NULL;
		break;
	}

	if(Array.Size()>=2 && fpSortBy != NULL)// 리스트가 2개 이상일때만 정렬한다.
	{
		std::sort(Array.Begin(), Array.End(), fpSortBy);
	}
}
//------------------------------------------------------------------------------
/** 오름차순 GetOrderBy()==true / 내림차순  GetOrderBy()==false
*/
bool _Sort_NameMail(const MAIL_DATA &a, const MAIL_DATA &b)
{
	return _BaseSort_NameMail(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _Sort_DateMail(const MAIL_DATA &a, const MAIL_DATA &b)
{
	return _BaseSort_DateMail(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
bool _BaseSort_MemoMail(const MAIL_DATA &a, const MAIL_DATA &b)
{
	return _BaseSort_MemoMail(a, b, GetCommunityMan()->GetOrderBy(GetCommunityMan()->GetMode()));
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SortbyListMail(std::vector<MAIL_DATA> &Array, int iSortBy, bool OrderBy)
{
	bool (*fpSortBy)(const MAIL_DATA &a, const MAIL_DATA &b);

	// (OrderBy) true 오름차순, false 내림차순

	SetOrderBy(GetMode(), OrderBy);

	switch(iSortBy)
	{
	case eCOMSORT_SENDER:
		fpSortBy = _Sort_NameMail;
		break;
	case eCOMSORT_DATE:
		fpSortBy = _Sort_DateMail;
		break;
	case eCOMSORT_MEMO:
		fpSortBy = _BaseSort_MemoMail;
		break;
	default:
		fpSortBy = NULL;
		break;
	}

	if(Array.size()>=2 && fpSortBy != NULL)// 리스트가 2개 이상일때만 정렬한다.
	{
		std::sort(Array.begin(), Array.end(), fpSortBy);
	}
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityMan::FindChatData(TCHAR *pName)
{
	for(int i=0;i<m_chatDlgArray.Size();++i)
	{
		if(!Strcmp(m_chatDlgArray[i].name.Get(),pName))
		{
			return i;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowFriendChatDlg(TCHAR *pName)
{
	if(m_iNewFriendChat > 0)
	{
		for(int i=0;i<m_chatDlgArray.Size();++i)
		{
			if(pName)
			{
				if(!Strcmp(m_chatDlgArray[i].name.Get(),pName))
				{
					NewFriendChatDlg(i);
					return;
				}
			}
			else
			{
				if(NewFriendChatDlg(i) == true)
					return;
			}
		}	
	}
}

//----------------------------------------------------------------------------
/**
*/
bool uiCommunityMan::NewFriendChatDlg(int iIndex)
{
	if(m_chatDlgArray[iIndex].dwDlgId == -1)
	{
		--m_iNewFriendChat;		

		// 1:1 대화창 연결
		uiCommunityChat *pFriendChatDialog = new uiCommunityChat(this->GetUIMan());
		pFriendChatDialog->SetFriendName((TCHAR*)m_chatDlgArray[iIndex].name.Get());
		m_chatDlgArray[iIndex].dwDlgId = g_InterfaceManager.NewInterface( pFriendChatDialog, "Data\\Interface\\66_1_Message.iwz", TRUE);

		if(pFriendChatDialog)
		{
			for(size_t s = 0; s < m_chatDlgArray[iIndex].vstrChatMsg.size(); ++s)
			{
				pFriendChatDialog->AddString((TCHAR*)m_chatDlgArray[iIndex].vstrChatMsg[s].c_str(), RGBA(131,190,126,255));
			}
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddChatMsg(TCHAR * ptszFriendNameFrom ,TCHAR *ptszChatMsg ,COLORREF TextColor) 
{	
	TCHAR	szResult[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T("%s:%s"), ptszFriendNameFrom, ptszChatMsg);

	for(int i=0;i<m_chatDlgArray.Size();++i)
	{
		if(!Strcmp(m_chatDlgArray[i].name.Get(),ptszFriendNameFrom))
		{
			m_chatDlgArray[i].vstrChatMsg.push_back(szResult);
			return;
		}
	}
	
}
//
////----------------------------------------------------------------------------
///**
//*/
//void uiCommunityMan::ShowTooltipForNewChatMsg()
//{
//	if(m_iNewFriendChat > 0)
//	{
//		for(int i=0;i<m_chatDlgArray.Size();++i)
//		{
//			if(m_chatDlgArray[i].dwDlgId == -1)
//			{
//				//5921	%s님께서 대화신청을 하셨습니다.
//				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
//				TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//				
//				g_InterfaceManager.GetInterfaceString(5921, szTemp, INTERFACE_STRING_LENGTH);
//                Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_chatDlgArray[i].name.Get());
//
//				uiToolTipMan* pTooltipMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
//				if(pTooltipMan)
//				{
//					pTooltipMan->RegisterTooltipSimpleText(szMessage);
//					pTooltipMan->SetTooltipDialogID(GetDialogKey());
//				}
//
//			}
//		}	
//	}
//}

//----------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityMan::GetFriendNameForTooltip()
{
	if(m_iNewFriendChat > 0)
	{
		for(int i=0;i<m_chatDlgArray.Size();++i)
		{
			if(m_chatDlgArray[i].dwDlgId == -1)
			{
				return (TCHAR*) m_chatDlgArray[i].name.Get();
			}
		}	
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::RequestFriendChat(bool bInfoDlg)	// 1:1 대화
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szName[MAX_CHARNAME_LENGTH+1] = {0,};

	if(bInfoDlg)
	{
		StrnCopy(szName, m_FriendInfo.m_ptszCharName, MAX_CHARNAME_LENGTH);
	}
	else
	{
		if(GetSelectString() != NULL)
		{
			StrnCopy(szName, GetSelectString(), MAX_CHARNAME_LENGTH);
		}
	}

	if(GetRejectChat())
	{
		//	대화 거부 상태입니다.
		g_InterfaceManager.GetInterfaceString( eST_REJECT_CHAT_STATE, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
	}
	else
	{
		g_InterfaceManager.GetInterfaceString( 1306, szMessage, INTERFACE_STRING_LENGTH );
		
		if(IsOnline(szName))
		{
			if(FindChatData(szName) >= 0)
			{
				// 이미 해당친구로부터 받아놓은 메시지가 있을 경우엔 그걸 같이 보여준다.
				ShowFriendChatDlg(szName);

			}
			else if(FindChatDialog(szName) < 0)	// 아니면 새로 창을 만든다
			{
				//1:1 대화창 연결
				uiCommunityChat *pFriendChatDialog = new uiCommunityChat(this->GetUIMan());
				if(pFriendChatDialog)
				{
					pFriendChatDialog->SetFriendName((TCHAR*)szName);
					FRIEND_CHAT_DATA temp;
					ZeroMemory(&temp, sizeof(temp));
					temp.dwDlgId = g_InterfaceManager.NewInterface( pFriendChatDialog, "Data\\Interface\\66_1_Message.iwz", TRUE);
					temp.name = szName;

					AddFriendChatData(temp);

				}
			}

		}
		else
		{
			//	현재 오프라인 상태 입니다.
			g_InterfaceManager.GetInterfaceString( eST_OFFLINE_STATE, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::_processResultCode(BYTE byErrorCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	switch(byErrorCode)
	{
	case RC_FRIEND_NOTCONNECTED:			// 접속 안 돼 있음
		g_InterfaceManager.GetInterfaceString( eST_OFFLINE_STATE, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		break;

	case RC_FRIEND_NOTFRIEND:				// 친구 아님
		// 5451	친구로 등록되어 있지 않습니다.
		g_InterfaceManager.GetInterfaceString( 5451, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		break;

	case RC_FRIEND_DIFFCHANNEL:				// 채널이 다름
		g_InterfaceManager.GetInterfaceString( 5251, szMessage, INTERFACE_STRING_LENGTH );	//다른 채널에 있는 친구에게는 사용할 수 없습니다.	
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		break;

	case RC_FRIEND_INVALIDZONE:				// 조인 가능한 방 타입(로비, 헌팅방)이 아님
		{
			// 5452	해당 캐릭터가 참여 가능한 방에 있지 않습니다.
			g_InterfaceManager.GetInterfaceString( 5452, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	default:
		{
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("Friend ErrMsg [%d]"), byErrorCode);
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowFriendInfoDlg(BOOL bShow)
{
    if(m_pUICommunityInfo)
    {
        if (bShow == TRUE)
        {
            m_pUICommunityInfo->ShowInterface(bShow);
        }
        else
        {
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            if ((m_pUICommunityInfo->request_uimanager_id() != UIMAN_GUILD) && 
                (m_pUICommunityInfo->request_uimanager_id() != UIMAN_CONVENIENCESTORE))
            {
                m_pUICommunityInfo->ShowInterface(bShow);
            }
#else
            if (m_pUICommunityInfo->request_uimanager_id() != UIMAN_GUILD)
            {
                m_pUICommunityInfo->ShowInterface(bShow);
            }
#endif
        }
    }
}



//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowWriteMailDlg(TCHAR* pszName /*= NULL*/)
{
	if(m_pUICommunityWriteMail && !m_pUICommunityWriteMail->IsVisibleWindow())
	{
		ZeroMemory(m_szRecvNameForMail, sizeof(m_szRecvNameForMail));

		if(pszName == NULL)
		{
			if(GetSelectString() != NULL)
			{
				StrnCopy(m_szRecvNameForMail, GetSelectString(), MAX_CHARNAME_LENGTH);
			}
		}
		else
		{
			StrnCopy(m_szRecvNameForMail, pszName, MAX_CHARNAME_LENGTH);
		}
		
		m_pUICommunityWriteMail->ShowInterface(TRUE);
		
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SetWindowFocus()
{
	// 친구창
	if(m_pUICommunityDlg && m_pUICommunityDlg->IsVisibleWindow())
	{
		m_pUICommunityDlg->GetDialogWZ()->ShowWindowWZ(WZ_SHOW);
	}
	
	// 친구정보창
	if(m_pUICommunityInfo && m_pUICommunityInfo->IsVisibleWindow())
	{
		m_pUICommunityInfo->GetDialogWZ()->ShowWindowWZ(WZ_SHOW);
	}

	// 친구채팅창
	for(int i=0; i<m_chatDlgArray.Size(); ++i)
	{
		//1:1 대화창 연결
		uiCommunityChat * pDlg = 
			(uiCommunityChat *)g_InterfaceManager.GetDialog( (WzID)m_chatDlgArray[i].dwDlgId );

		if(pDlg && pDlg->IsVisibleWindow())
		{
			pDlg->GetDialogWZ()->ShowWindowWZ(WZ_SHOW);
		}
	}

	if(m_pUICommunityReadMail && m_pUICommunityReadMail->IsVisibleWindow())
	{
		m_pUICommunityReadMail->GetDialogWZ()->ShowWindowWZ(WZ_SHOW);
	}

	if(m_pUICommunityWriteMail && m_pUICommunityWriteMail->IsVisibleWindow())
	{
		m_pUICommunityWriteMail->GetDialogWZ()->ShowWindowWZ(WZ_SHOW);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowReadMailDlg(MAIL_DATA *pData)
{
	if(m_pUICommunityReadMail)
	{
		BOOL bSystem =	m_pUICommunityReadMail->SetMailData(pData);
		m_pUICommunityReadMail->ShowInterface(TRUE);
		m_pUICommunityReadMail->CloseAnswerBtn(bSystem);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ShowReadMailDlg(MEMOGUID guid)
{
	if(m_pUICommunityReadMail)
	{
		for(unsigned int i=0; i<m_vecMailData.size(); ++i)
		{
			if(guid == m_vecMailData[i].m_subject.m_MemoGuid)
			{
				BOOL bSystem = m_pUICommunityReadMail->SetMailData(&m_vecMailData[i]);
				m_pUICommunityReadMail->ShowInterface(TRUE);
				m_pUICommunityReadMail->CloseAnswerBtn(bSystem);
				break;
			}
		}
	}

}

//------------------------------------------------------------------------------
void   uiCommunityMan::SEND_CW_FRIEND_MEMO_UPDATE_SYN(CHARGUID game_user_id, TCHAR* memo)
{
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO

    MSG_CW_FRIEND_MEMO_UPDATE_SYN sync;
    ZeroMemory(sync.m_ptszMemo, sizeof(sync.m_ptszMemo));

    sync.m_dwToCharGuid = game_user_id;
    StrnCopy(sync.m_ptszMemo, memo, MAX_FRIEND_MEMO_LENGHT);

    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));

#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_MEMO_SEND_SYN(TCHAR *pszName, TCHAR *pszMemo)	// 쪽지 전송
{
	MSG_CW_MEMO_SEND_SYN sync;
	ZeroMemory(sync.m_ptszRecvName, sizeof(sync.m_ptszRecvName));
	ZeroMemory(sync.m_ptszMemo, sizeof(sync.m_ptszMemo));
	StrnCopy(sync.m_ptszRecvName, pszName, MAX_CHARNAME_LENGTH);
	StrnCopy(sync.m_ptszMemo, pszMemo, MAX_MEMO_STRLENGTH-1);

	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_MEMO_LIST_REQ()	// 쪽지 목록 요청
{
	if (g_pNetwork && (g_pNetwork->IsCommunicatable(CI_CHATSERVERIDX) == TRUE))
	{
		if(!m_bRecvMailInfo)
		{
			MSG_CW_MEMO_LIST_REQ sync;
			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
		}
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		// 5026	서버 상태가 원활하지 않습니다. 잠시 후에 다시 시도해 주시기 바랍니다.
		g_InterfaceManager.GetInterfaceString(eST_SERVER_STATE_BAD, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage );  
	}
}

//------------------------------------------------------------------------------
/**
*/
bool uiCommunityMan::SEND_CW_MEMO_VIEW_REQ(MEMOGUID guid)	// 쪽지 내용 확인
{
	if (SafePKBool(ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ))
	{
		if (g_pNetwork && (g_pNetwork->IsCommunicatable(CI_CHATSERVERIDX) == TRUE))
		{
			MSG_CW_MEMO_VIEW_REQ sync;
			sync.m_MemoGuid = guid;

			GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
		}
		else
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			// 5026	서버 상태가 원활하지 않습니다. 잠시 후에 다시 시도해 주시기 바랍니다.
			g_InterfaceManager.GetInterfaceString(eST_SERVER_STATE_BAD, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage );  
		}

		// 패킷 팬딩 셋	
		SetPKBool(ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ, TIMEOUT_PACKET_PENDING); // 서버로직의 문제로 해당 패킷 송수신에 대해서는 펜딩하지 않는다.
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::SEND_CW_MEMO_DELETE_SYN(MEMOGUID guid)		// 쪽지 삭제 요청
{
	if (g_pNetwork && (g_pNetwork->IsCommunicatable(CI_CHATSERVERIDX) == TRUE))
	{
		MSG_CW_MEMO_DELETE_SYN sync;
		sync.m_MemoGuid = guid;
        sync.m_MemoType = GetMemoType(guid);

		// 쪽지 전체 삭제일때는 sync.m_MemoGuid 에 0을 넣어 보내자

		GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		// 5026	서버 상태가 원활하지 않습니다. 잠시 후에 다시 시도해 주시기 바랍니다.
		g_InterfaceManager.GetInterfaceString(eST_SERVER_STATE_BAD, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage );  

	}

}

void uiCommunityMan::OnRecvCG_Group(MSG_BASE *pMsg)
{	
    TCHAR	message_text[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	notice_text[INTERFACE_STRING_LENGTH+1] = {0,};
	
	if (m_pUICommunityDlg == NULL)
    {
		return;
    }

	switch(pMsg->m_byProtocol)
	{
	case CG_STATUS_GROUPMONEY_BRD:	// 하임 획득
		{
			MSG_CG_STATUS_GROUPMONEY_BRD * pRecvPacket = (MSG_CG_STATUS_GROUPMONEY_BRD *)pMsg;

			if (m_pUICommunityDlg->IsVisibleWindow())
            {
				m_pUICommunityDlg->RefreshFriendList();
            }

			if (g_pHero == NULL)
            {
                // 히어로가 없습니다.
                return;
            }

			MONEY result_money = g_pHero->GetMoney() + pRecvPacket->bonusMoney;
			g_pHero->SetMoney(result_money);
			//  7052: 그룹 멤버로부터 %d 하임을 보상 받았습니다.
			g_InterfaceManager.GetInterfaceString(7052, message_text, INTERFACE_STRING_LENGTH);
			Sprintf(notice_text, message_text, pRecvPacket->bonusMoney);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_text);

			if (GameFramework::GetCurrentScene() && 
                pRecvPacket->bonusMoney)
			{
				if (GAMEOPTION->GetPickUpItemNotify())
				{
					GameFramework::GetCurrentScene()->InserCenterTextMessage(notice_text, PICK );
				}
			}
		}
		break;
	default:
		{
		//assert(!"'uiCommunityMan::OnRecvCG_Friend()' receive unknown protocol from Game Server"); //게임 서버에서 정의되지 않은 프로토콜이 왔다.!!
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRecvCG_Event(MSG_BASE *pMsg)
{
	
    TCHAR	szMessage[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	szWideString[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	szVal[INTERFACE_STRING_LENGTH+1] = {0,};
	
	if(!m_pUICommunityDlg)
		return;

	switch(pMsg->m_byProtocol)
	{
	case CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK:	//	친구 초대 이벤트 보상 목록 받기 성공
		{
			MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK *)pMsg;
			if(pRecvPacket)
			{
				uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
				uiEventFriend * eventDlg = GET_CAST_DIALOG( uiEventFriend, IM_EVENT_MANAGER::EVENT_FRIEND );

				if(eventMan && eventDlg)
				{
                    eventMan->InitInvitateFriendList();
					for(int i = 0; i < pRecvPacket->m_Count; ++i)
					{
						if(pRecvPacket->RewardInfo[i].RewardSts == 0)	// 보상을 안받은 경우만
						{
							eventMan->InsertList(&pRecvPacket->RewardInfo[i]);
						}
					}
					eventMan->ShowEventFriendWin(TRUE);// 보상목록 창 열기
				}
			}
		}
		break;
	case CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK:	// 친구 초대 이벤트 보상 목록 받기 실패
		{
			MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK *)pMsg;
			if(pRecvPacket)
			{
				int iStrNum = 0;	
				switch(pRecvPacket->byErrorCode)
				{
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK::DB_FAIL:	// DB 시스템 오류
					iStrNum = 6081; // DB 시스템 오류
					break;
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK::DBPROXY_FAIL:	// DB PROXY 서버 연결 불량
					iStrNum = 6081;	// DB PROXY 서버 연결 불량
					break;
				default:
					iStrNum = 6081; // 알 수 없는 ErrorCode
					break;
				}
				if(iStrNum != 0)
				{
					g_InterfaceManager.GetInterfaceString(iStrNum, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
			}
		}
		break;	
	case CG_EVENT_INVITATEFRIEND_REWARD_ACK:	// 친구 초대 이벤트에 대한 보상 받기 성공
		{
			MSG_CG_EVENT_INVITATEFRIEND_REWARD_ACK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_REWARD_ACK *)pMsg;
			if(pRecvPacket)
			{
				ItemManager::Instance()->InsertInventoryTotalInfo( REWARD, pRecvPacket->m_TotalInfo );
				
				uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
				uiEventFriend * eventDlg = GET_CAST_DIALOG( uiEventFriend, IM_EVENT_MANAGER::EVENT_FRIEND );

				if(eventMan && eventDlg)	// 목록 갱신
				{
					eventMan->SendEventFriend();
				}
			}
		}
		break;
	case CG_EVENT_INVITATEFRIEND_REWARD_NAK:	// 친구 초대 이벤트에 대한 보상 받기 실패
		{
			MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK *)pMsg;
			if(pRecvPacket)
			{
				int iStrNum = 0;	
				switch(pRecvPacket->m_byErrorCode)
				{
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::DB_FAIL:
					iStrNum = 6081; // DB 시스템 오류
					break;
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::DBPROXY_FAIL:
					iStrNum = 6081;	// DB PROXY 서버 연결 불량
					break;
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::RECEV_REWARDED:
					iStrNum = 6081;	// 이미 보상 받았음
					break;
				case MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::NOTOBTAIN_INVEN:
					iStrNum = 6081;	// 인벤에 넣을수 없음
					break;
				default:
					iStrNum = 6081; // 알 수 없는 ErrorCode
					break;
				}
				if(iStrNum != 0)
				{
					g_InterfaceManager.GetInterfaceString(iStrNum, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
			}
		}
		break;
	/*case CG_EVENT_INVITATEFRIEND_RECALLTIME_ACK:
		{
			MSG_CG_EVENT_INVITATEFRIEND_RECALLTIME_ACK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_RECALLTIME_ACK *)pMsg;
			if(pRecvPacket)
			{

			}
		}
		break;
	case CG_EVENT_INVITATEFRIEND_RECALLTIME_NAK:
        {
			MSG_CG_EVENT_INVITATEFRIEND_RECALLTIME_NAK * pRecvPacket = (MSG_CG_EVENT_INVITATEFRIEND_RECALLTIME_NAK *)pMsg;
			if(pRecvPacket)
			{
				int iStrNum = 0;	
				switch(pRecvPacket->m_byErrorCode)
				{
				case 1:	// DB 시스템 오류
					iStrNum = 6081; // DB 시스템 오류
					break;
				case 2:	// DB PROXY 서버 연결 불량
					iStrNum = 6081;	// DB PROXY 서버 연결 불량
					break;
				default:
					iStrNum = 6081; // 알 수 없는 ErrorCode
					break;
				}
				if(iStrNum != 0)
				{
					g_InterfaceManager.GetInterfaceString(iStrNum, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
			}
		}
		break;*/

	case CG_STATUS_GROUPMONEY_BRD:	// 하임 획득
		{
			MSG_CG_STATUS_GROUPMONEY_BRD * pRecvPacket = (MSG_CG_STATUS_GROUPMONEY_BRD *)pMsg;

			if (m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			if (g_pHero)
			{
				MONEY orgMoney = g_pHero->GetMoney() + pRecvPacket->bonusMoney;
				g_pHero->SetMoney(orgMoney);
				//  7052: 그룹 멤버로부터 %d 하임을 보상 받았습니다.
				g_InterfaceManager.GetInterfaceString(7052, szMessage, INTERFACE_STRING_LENGTH);
				Sprintf(szVal, szMessage, pRecvPacket->bonusMoney);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);

				if (GameFramework::GetCurrentScene() && pRecvPacket->bonusMoney)
				{
					if (GAMEOPTION->GetPickUpItemNotify())
					{
						GameFramework::GetCurrentScene()->InserCenterTextMessage( szVal, PICK );
					}
				}
			}
			else	// 히어로가 없습니다.
			{
				return;
			}
		}
		break;

    case CG_EVENT_ITEM_REWARD_RESULT_CMD:
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        {
            MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD* reward_cmd_packet = static_cast<MSG_CG_EVENT_ITEM_REWARD_RESULT_CMD*>(pMsg);
            if (reward_cmd_packet->result_value_ == TRUE)
            {
                // 쪽지를 통한 아이템/하임 습득이 이루어진 것으로 MemoGUID 를 통해 쪽지 내용 요청을 한다.
                SEND_CW_MEMO_VIEW_REQ(reward_cmd_packet->memo_guid_);
            }
        }
#endif //_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        break;
    case CG_EVENT_SYSTEMMEMO_ITEM_REWARD_ANS:
        {
            MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_ANS* reward_ans_packet = static_cast<MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_ANS*>(pMsg);
            MAIL_DATA* mail_data = NULL;
            size_t mail_data_index = m_vecMailData.size();
            for(size_t index = 0; index < mail_data_index; ++index)
            {
                if(reward_ans_packet->memo_guid_ == m_vecMailData[index].m_subject.m_MemoGuid)
                {
                    mail_data = &(m_vecMailData[index]);
                    break;
                }
            }
            // 성공이면 패스 
            if (mail_data != NULL)
            {
                if (reward_ans_packet->result_value_ == RC::RC_ITEM_SUCCESS)
                {
                    mail_data->m_recvItemCode = 0;
                    mail_data->m_amountCost = 0;
                }
                else
                {
                    // 실패면 읽음상태 변경 
                    mail_data->m_subject.m_byRead = 0;
                    mail_data->m_bRecvMemo = false;
                    // 에러코드처리 추가
                    ItemManager::Instance()->NoticeReasonItemErrorCode(reward_ans_packet->result_value_);
                }
            }            
        }
        break;
	default:
		{
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::NetworkProcWorld( MSG_BASE * pMsg )
{
	if(!g_pHero)
		return;

	switch (pMsg->m_byCategory)
	{
	case CW_FRIEND:
		{
			OnRecvCW_Friend(pMsg);
		}
		break;

	case CW_REVENGE:
		{
			OnRecvCW_Revenge(pMsg);
		}
		break;

	case CW_MEMO:
		{
			OnRecvCW_Memo(pMsg);
		}
		break;
	case CW_GROUP:
		{
			OnRecvCW_Group(pMsg);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRecvCW_Group(MSG_BASE *pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	szWideString[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	szVal[INTERFACE_STRING_LENGTH+1] = {0,};
	
	if(!m_pUICommunityDlg)
		return;

	switch(pMsg->m_byProtocol)
	{
	case CW_GROUP_GROUPLIST_ANS:// 멤버 목록
		{
			m_bRecvGroupMemberInfo = true;

			MSG_CW_GROUP_GROUPLIST_ANS * pRecvPacket = (MSG_CW_GROUP_GROUPLIST_ANS *)pMsg;

			for(int i=0;i<pRecvPacket->byGroupMemberNum;++i)
			{
				ZeroMemory(szWideString, sizeof(szWideString));
				StrnCopy(szWideString , pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.ptszMemberName, MAX_CHARNAME_LENGTH);
				AddGroup(szWideString, pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.Level, 
										pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.byClass,
										pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.bOnline, 
										pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.byChannel,
										pRecvPacket->sGroupMemberInfo[i].groupSTAT,
										pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.lastDate,
                                        pRecvPacket->sGroupMemberInfo[i].charGUID);
                if (pRecvPacket->sGroupMemberInfo[i].GroupMemberInfo.bOnline != 0)
                {
                    ++onlined_group_count_;
                }
			}

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			SetGroupTimer(0);
			CheckGroupLevelfromHero(); // 목록 요청후 레벨 비교후 타이머 동작 결정.
		}
		break;

	case CW_GROUP_GROUPLIST_NAK:// 멤버 목록 요청 실패
		{
			MSG_CW_GROUP_GROUPLIST_NAK * pRecvPacket = (MSG_CW_GROUP_GROUPLIST_NAK *)pMsg;

			//	7049: 그룹 멤버 요청에 실패하였습니다.
			g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		break;

	case CW_GROUP_ADD_RESPONSE_SYN:
		{
			MSG_CW_GROUP_ADD_RESPONSE_SYN *pRecvPacket = (MSG_CW_GROUP_ADD_RESPONSE_SYN *)pMsg;
			ResponseJoinGroup(pRecvPacket->ptszMemeberName);
		}
		break;

	case CW_GROUP_ADD_ACK:
		{
			MSG_CW_GROUP_ADD_ACK * pRecvPacket = (MSG_CW_GROUP_ADD_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString ,pRecvPacket->m_MemberInfo.ptszMemberName, MAX_CHARNAME_LENGTH);

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
			AddGroup(szWideString, 
                    pRecvPacket->m_MemberInfo.Level,
                    pRecvPacket->m_MemberInfo.byClass, 
                    pRecvPacket->m_MemberInfo.bOnline, 
                    pRecvPacket->m_MemberInfo.byChannel, 
                    pRecvPacket->m_GroupInfo, 
                    pRecvPacket->m_MemberInfo.lastDate, 
                    pRecvPacket->charGUID);
#else
            AddGroup(szWideString, 
                    pRecvPacket->m_MemberInfo.Level,
                    pRecvPacket->m_MemberInfo.byClass, 
                    pRecvPacket->m_MemberInfo.bOnline, 
                    pRecvPacket->m_MemberInfo.byChannel, 
                    pRecvPacket->m_GroupInfo, 
                    pRecvPacket->m_MemberInfo.lastDate, 
                    0);
#endif

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

            if (GetMode() == eCM_GROUP)
            {
                if (m_pUICommunityDlg)
                {
                    m_pUICommunityDlg->RefreshOnlineMember(eCM_GROUP);
                }
            }
			if(pRecvPacket->m_GroupInfo == GROUP_LEVEL_HIGH)
			{
			}
			else	//GROUP_LEVEL_LOW, 상위인 경우 하위 멤버를 받는다.
			{
				//  7037: %s님이 멤버 등록에 수락하였습니다.
				g_InterfaceManager.GetInterfaceString(7037, szMessage, INTERFACE_STRING_LENGTH);
				Sprintf(szVal, szMessage, szWideString);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
			}
		}
		break;
	case CW_GROUP_ADD_NAK:
		{
			MSG_CW_GROUP_ADD_NAK * pRecvPacket = (MSG_CW_GROUP_ADD_NAK *)pMsg;

			switch(pRecvPacket->m_byResult)
			{
			case MSG_CW_GROUP_ADD_NAK::NO_USER:
				{
					//	70029: 사용자를 찾을 수가 없습니다.
					g_InterfaceManager.GetInterfaceString( 70029, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP:
				{
					//	7050: 이미 등록된 멤버 입니다.
					g_InterfaceManager.GetInterfaceString( 7050, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::NOT_ADD_ME:
				{
					//	7051: 자신은 등록 할수 없습니다.
					g_InterfaceManager.GetInterfaceString( 7051, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::GROUP_OVERFLOW: 
				{
					//	7039: 그룹 멤버 허용 인원을 초과하였습니다. (하위 멤버 최대 5명 가능)
					g_InterfaceManager.GetInterfaceString( 7039, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::NOT_MATCHING_LEVEL:
				{
					//	7040: 등록할 멤버의 레벨은 자신보다 낮아야 합니다.
					g_InterfaceManager.GetInterfaceString( 7040, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::IGNOR_ADD_MEMBER:
				{
					ZeroMemory(szWideString, sizeof(szWideString));
					StrnCopy(szWideString , pRecvPacket->ptszMemeberName, MAX_CHARNAME_LENGTH);

					//	7038: %s님이 멤버 등록을 거절하였습니다.
					g_InterfaceManager.GetInterfaceString( 7038, szMessage, INTERFACE_STRING_LENGTH );
					Snprintf(szVal, INTERFACE_STRING_LENGTH-1 , szMessage, szWideString);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::RES_TIME_OUT:
				{
					ZeroMemory(szWideString, sizeof(szWideString));
					StrnCopy(szWideString , pRecvPacket->ptszMemeberName, MAX_CHARNAME_LENGTH);

					//	%s님이 일정 시간 동안 권유 요청에 대한 응답이 없어 알림 창이 종료되었습니다.
					g_InterfaceManager.GetInterfaceString( 7045, szMessage, INTERFACE_STRING_LENGTH );
					Snprintf(szVal, INTERFACE_STRING_LENGTH-1 , szWideString);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_ADD_NAK::ON_TOGGLE_ADDBLOCK:
				{
					ZeroMemory(szWideString, sizeof(szWideString));
					//	해당 유저는 그룹 멤버 권유 거부 상태입니다.
					g_InterfaceManager.GetInterfaceString( 7056, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
            case MSG_CW_GROUP_ADD_NAK::BEHAVE_VENDOR_ESTABLISHER:
                {
                    // 5815	상대방이 개인상점을 개설 중입니다.
                    g_InterfaceManager.GetInterfaceString(5815 , szMessage ,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                }
                break;
			default: //ERR_DEFAULT
				{
					//	7049: 그룹 멤버 요청에 실패하였습니다.
					g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
			}

		}
		break;
	case CW_GROUP_DEL_ACK:// 멤버 삭제 성공(등록 해제한 캐릭터)
		{
			MSG_CW_GROUP_DEL_ACK * pRecvPacket = (MSG_CW_GROUP_DEL_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszMemeberName,  MAX_CHARNAME_LENGTH);

			DeleteGroup(szWideString);
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

            if (GetMode() == eCM_GROUP)
            {
                if (m_pUICommunityDlg)
                {
                    m_pUICommunityDlg->RefreshOnlineMember(eCM_GROUP);
                }
            }
			//  7034: %s님이 그룹의 멤버에서 해제되었습니다.
			g_InterfaceManager.GetInterfaceString(7034, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(szVal, szMessage, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}
		break;

	case CW_GROUP_DEL_BRD:// 멤버 삭제 통보(등록 해제 당한 캐릭터)

		{
			MSG_CW_GROUP_DEL_BRD * pRecvPacket = (MSG_CW_GROUP_DEL_BRD *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszMemeberName,  MAX_CHARNAME_LENGTH);

			DeleteGroup(szWideString);
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			//  %s님이 그룹의 멤버에서 해제하였습니다.
			g_InterfaceManager.GetInterfaceString(7035, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(szVal, szMessage, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}
		break;

	case CW_GROUP_DEL_NAK:// 멤버 삭제 실패
		{
			MSG_CW_GROUP_DEL_NAK * pRecvPacket = (MSG_CW_GROUP_DEL_NAK *)pMsg;

			g_InterfaceManager.GetInterfaceString(7049, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(szVal, szMessage, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}
		break;

	case CW_GROUP_LOGIN_BRD:
		{
			MSG_CW_GROUP_LOGIN_BRD * pRecvPacket = (MSG_CW_GROUP_LOGIN_BRD *)pMsg;

			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1 ]= {0,};
			StrnCopy( szConvertCharName , pRecvPacket->ptszMemeberName, MAX_CHARNAME_LENGTH );

			SetGroupOnline(szConvertCharName, TRUE, pRecvPacket->byChannel);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			//  그룹멤버 %s님이 로그인 하였습니다.
			g_InterfaceManager.GetInterfaceString(7047, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(szVal, szMessage, szConvertCharName);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}	
		break;

	case CW_GROUP_LOGOUT_BRD:
		{
			MSG_CW_GROUP_LOGOUT_BRD * pRecvPacket = (MSG_CW_GROUP_LOGOUT_BRD *)pMsg;
			
			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1] = {0,};
			StrnCopy( szConvertCharName , pRecvPacket->ptszMemeberName, MAX_CHARNAME_LENGTH );

			SetGroupOnline(szConvertCharName, FALSE, 0);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();


			//  그룹멤버 %s님이 로그아웃 하였습니다.
			g_InterfaceManager.GetInterfaceString(7048, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(szVal, szMessage, szConvertCharName);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}
		break;

	case CW_GROUP_LEVELUP_BRD:	//멤버 레벨업 알림, 레벨 오버되면 10분마다 출력
		{
			MSG_CW_GROUP_LEVELUP_BRD *pRecvPacket = (MSG_CW_GROUP_LEVELUP_BRD *)pMsg;

			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1];
			StrnCopy( szConvertCharName , pRecvPacket->ptszMemeberName, MAX_CHARNAME_LENGTH );
			SetGroupLevel(szConvertCharName, pRecvPacket->charLevel);
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			if(pRecvPacket->charLevel >= g_pHero->GetLevel())
			{
				//레벨업 처리, timer설정 및 갱신
				SetGroupTimer(1000);	// 1초후 바로 메시지 출력
			}
		}
		break;

	case CW_GROUP_ACCUMEXP_BRD:	// 누적 경험치
		{
			MSG_CW_GROUP_ACCUMEXP_BRD * pRecvPacket = (MSG_CW_GROUP_ACCUMEXP_BRD *)pMsg;

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();


			// 경험치 추가 설정
			if (g_pHero) 
			{
				//g_pHero->SetMoney(pRecvPacket->m_Money);
				EXPTYPE	addexp = pRecvPacket->accumEXP;			// 추가될				
				EXPTYPE exp = g_pHero->GetCharInfo()->m_i64Exp + addexp;

				g_pHero->SetExp(exp);
			}
			else // 히어로가 없습니다.
			{
				return;
			}

			/* // TODO: 서버측 요청으로 메시지 띄우지 않음, 추후 다른 곳에서 출력
			if(pRecvPacket->checkDATE == GROUP_LEVEL_HIGH_OVERDATE)
			{
				//  그룹 멤버로부터 %d 경험치를 보상 받았으며, 오프라인 상태 7일 이후의 경험치는 보상 받을 수 없습니다.
				g_InterfaceManager.GetInterfaceString(7043, szMessage, INTERFACE_STRING_LENGTH);	
				Sprintf(szVal, szMessage, pRecvPacket->accumEXP);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
			}
			else if(pRecvPacket->accumEXP >= MAX_GROUP_EXP)
			{
				//  그룹 멤버로부터 최대 보너스 경험치 5,000,000을 보상 받았습니다.
				g_InterfaceManager.GetInterfaceString(7042, szMessage, INTERFACE_STRING_LENGTH);	
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
			else
			{
				//  그룹 멤버로부터 %d 경험치를 보상 받았습니다.
				g_InterfaceManager.GetInterfaceString(7041, szMessage, INTERFACE_STRING_LENGTH);	
				Sprintf(szVal, szMessage, pRecvPacket->accumEXP);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
			}
			if( GAMEOPTION->GetPickUpItemNotify() )	// 가운데 메시지
			{
				GameFramework::GetCurrentScene()->InserCenterTextMessage( szVal, PICK );
			}
*/
		}
		break;

		case CW_GROUP_STATUS_ANS:
		{
			MSG_CW_GROUP_STATUS_ANS *pRecvPacket = (MSG_CW_GROUP_STATUS_ANS *)pMsg;
			FRIEND_INFO_DATA info;
			ZeroMemory(&info, sizeof(info));
			StrnCopy(info.m_ptszCharName, pRecvPacket->m_ptszCharName, MAX_CHARNAME_LENGTH);
			StrnCopy(info.m_tszGuildName, pRecvPacket->m_tszGuildName, MAX_GUILDNAME_LENGTH);
			info.m_byChannel	= pRecvPacket->m_byChannel;
			info.m_byZoneType	= pRecvPacket->m_byZoneType;
			info.m_dwZoneKey	= pRecvPacket->m_dwZoneKey;
			info.m_MapCode		= pRecvPacket->m_MapCode;
			info.m_byDuty		= pRecvPacket->m_byDuty;
			SetFriendInfo(info);
			ShowFriendInfoDlg();							
		}
		break;

	case CW_GROUP_STATUS_NAK:
		{
			MSG_CW_GROUP_STATUS_NAK *pRecvPacket = (MSG_CW_GROUP_STATUS_NAK *)pMsg;
			switch(pRecvPacket->dwResult)
			{
			case MSG_CW_GROUP_STATUS_NAK::NO_MEMBER_DATA://데이터 없다
				{
					//	데이터가 없다. : 7049: 그룹 멤버 요청에 실패하였습니다.
					g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			case MSG_CW_GROUP_STATUS_NAK::NOT_MEMBER:
				{
					//	그룹멤버가 아닙니다. : 7049: 그룹 멤버 요청에 실패하였습니다.
					g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			//case MSG_CW_GROUP_STATUS_NAK::ERR_DEFAULT:   //알수없는 에러
			default:
				{
					//	그룹 정보 요청 실패 에러 : 7049: 그룹 멤버 요청에 실패하였습니다.
					g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
				}
				break;
			}
		}
		break;

	case CW_GROUP_BONUSEXP_BRD:	// 경험치
		{
			MSG_CW_GROUP_BONUSEXP_BRD * pRecvPacket = (MSG_CW_GROUP_BONUSEXP_BRD *)pMsg;

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			// 경험치 추가 설정
			if (g_pHero) 
			{
				//g_pHero->SetMoney(pRecvPacket->m_Money);
				EXPTYPE	addexp = pRecvPacket->bonusEXP;			// 추가될				
				EXPTYPE exp = g_pHero->GetCharInfo()->m_i64Exp + addexp;

				g_pHero->SetExp(exp);
			
				/* // TODO: 서버측 요청으로 메시지 띄우지 않음, 추후 다른 곳에서 출력
				//  그룹 멤버로부터 %d 경험치를 보상 받았습니다.
				g_InterfaceManager.GetInterfaceString(7036, szMessage, INTERFACE_STRING_LENGTH);
				TCHAR szVal[INTERFACE_STRING_LENGTH + 1] = {0,};
				Sprintf(szVal, szMessage, pRecvPacket->bonusEXP);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);

				if( GAMEOPTION->GetPickUpItemNotify() )
				{
					GameFramework::GetCurrentScene()->InserCenterTextMessage( szVal, PICK );
				}
				*/
			}
			else	// 히어로가 없습니다.
			{
				return;
			}
		}
		break;

	case CW_GROUP_ADDBLOCK_ACK:
		{
			MSG_CW_GROUP_ADDBLOCK_ACK * pRecvPacket = (MSG_CW_GROUP_ADDBLOCK_ACK *)pMsg;
			SetGroupRejection(pRecvPacket->m_bNowBlock);
		}
		break;

	case CW_GROUP_ADDBLOCK_NAK:
		{
			BOOL rt = IsRecvGroup();
			SetGroupRejection(!rt);
		}
		break;
#ifdef _DH_GROUPSYSTEM_ALL3
   case CW_GROUP_REWARD_FAIL_CMD:
        {
            // 그룹 정보 요청 실패 에러 : 7049: 그룹 멤버 요청에 실패하였습니다.
			g_InterfaceManager.GetInterfaceString( 7049, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
        }
        break;
#endif//_DH_GROUPSYSTEM_ALL3

	default:
		{
			// 들어올 수 없는 곳 로그 남기기 TODO
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::NetworkProc( MSG_BASE * pMsg )
{
    switch (pMsg->m_byCategory)
	{
	case CG_EVENT:
		{
			OnRecvCG_Event(pMsg);
		}
		break;
    case CG_STATUS:
        {
            OnRecvCG_Group(pMsg);
        }
        break;
    default:
        {
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
bool _Find_GUID(const FRIEND_DATA &a, const FRIEND_DATA &b)
{
	return (a.dwFriendGuid == b.dwFriendGuid);
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRecvCW_Friend(MSG_BASE *pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH+1] = {0,};
	TCHAR	szWideString[INTERFACE_STRING_LENGTH+1] = {0,};
	
	if(!m_pUICommunityDlg)
		return;

	switch(pMsg->m_byProtocol)
	{
	case CW_FRIEND_FRIENDLIST_ACK:// 친구 목록
		{
			m_bRecvMemberInfo = true;

			MSG_CW_FRIEND_FRIENDLIST_ACK * pRecvPacket = (MSG_CW_FRIEND_FRIENDLIST_ACK *)pMsg;

			for(int i=0;i<pRecvPacket->byFriendStatNum;++i)
			{
                AddFriend( pRecvPacket->sFriendStat[i].sPacketInfo, pRecvPacket->sFriendStat[i].dwFriendGuid );

                if (pRecvPacket->sFriendStat[i].sPacketInfo.bOnline != 0)
                {
                    ++onlined_friend_count_;
                }
			}

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            //! 2011.7.22 / i4u4me / 캐시샵 작업 
            uiConvenienceStore_FriendList* friend_list_dlg = GET_CAST_DIALOG(uiConvenienceStore_FriendList, uiConvenienceStore_FriendList::kDialog_ID);
            if ((friend_list_dlg != NULL) && (friend_list_dlg->IsVisible() == TRUE))
            {
                friend_list_dlg->RefreshFriendList();
            }
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
		}
		break;

	case CW_FRIEND_BLOCKLIST_ACK://차단 목록 
		{
			MSG_CW_FRIEND_BLOCKLIST_ACK * pRecvPacket = (MSG_CW_FRIEND_BLOCKLIST_ACK *)pMsg;

			for(int i=0;i<pRecvPacket->byBlockStatNum;++i)
			{
				ZeroMemory(szWideString, sizeof(szWideString));
				StrnCopy(szWideString , pRecvPacket->sBlockStat[i].ptszBlockName,  MAX_CHARNAME_LENGTH);

				AddBlockFriend(szWideString);
			}

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

		}
		break;

	case CW_FRIEND_NEWLIST_NAK:// 친구 목록 요청 실패
		{
			MSG_CW_FRIEND_NEWLIST_NAK * pRecvPacket = (MSG_CW_FRIEND_NEWLIST_NAK *)pMsg;

			//	친구/차단 목록 요청 실패
			g_InterfaceManager.GetInterfaceString( eST_CW_FRIEND_NEWLIST_NAK, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		break;

	case CW_FRIEND_LOGIN_BRD:// 접속 중에 친구 로그온
		{
			MSG_CW_FRIEND_LOGIN_BRD * pRecvPacket = (MSG_CW_FRIEND_LOGIN_BRD *)pMsg;

			TCHAR szFriendName[MAX_CHARNAME_LENGTH +1] ={0,};
			StrnCopy( szFriendName , pRecvPacket->ptszFriendName , MAX_CHARNAME_LENGTH );

			SetFriendOnline(szFriendName, TRUE, pRecvPacket->byChannelID);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
		}
		break;

	case CW_FRIEND_LOGOUT_BRD:// 접속 중에 친구 로그아웃
		{
			MSG_CW_FRIEND_LOGOUT_BRD * pRecvPacket = (MSG_CW_FRIEND_LOGOUT_BRD *)pMsg;

			TCHAR szFriendName[MAX_CHARNAME_LENGTH +1] ={0,};
			StrnCopy( szFriendName , pRecvPacket->ptszFriendName , MAX_CHARNAME_LENGTH );
			SetFriendOnline(szFriendName, FALSE, 0);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
		}
		break;

	case CW_FRIEND_ADD_ACK:// 친구 추가
		{
			
			MSG_CW_FRIEND_ADD_ACK * pRecvPacket = (MSG_CW_FRIEND_ADD_ACK *)pMsg;
            AddFriend( pRecvPacket->sFriendStat.sPacketInfo, pRecvPacket->sFriendStat.dwFriendGuid );
            if (GetMode() == eCM_FRIEND)
            {
                if (m_pUICommunityDlg)
                {
                    m_pUICommunityDlg->RefreshOnlineMember(eCM_FRIEND);
                }
            }
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

            ZeroMemory(szWideString, sizeof(szWideString));
            StrnCopy(szWideString ,pRecvPacket->sFriendStat.sPacketInfo.ptszFriendName, MAX_CHARNAME_LENGTH);

			//  %s님이 친구목록에 추가 되었습니다.
			g_InterfaceManager.GetInterfaceString(5454, szMessage, INTERFACE_STRING_LENGTH);
			TCHAR szVal[INTERFACE_STRING_LENGTH + 1] = {0,};
			Sprintf(szVal, szMessage, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szVal);
		}
		break;

	case CW_FRIEND_ADD_NAK:
		{
			MSG_CW_FRIEND_ADD_NAK * pRecvPacket = (MSG_CW_FRIEND_ADD_NAK *)pMsg;

			if(pRecvPacket->m_byResult==MSG_CW_FRIEND_ADD_NAK::NO_USER)
			{
				//	해당 유저가 없습니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_USER, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else if(pRecvPacket->m_byResult==MSG_CW_FRIEND_ADD_NAK::ALREADY_FRIEND)
			{
				//	등록된 유저 입니다.
				g_InterfaceManager.GetInterfaceString( eST_ALREADY_ADDED_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else if(pRecvPacket->m_byResult==MSG_CW_FRIEND_ADD_NAK::FRIEND_IS_ME)
			{
				//	자신은 등록 할수 없습니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_ADD_YOURSELF, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else
			{
				//	친구 등록 실패
				g_InterfaceManager.GetInterfaceString( eST_FAILED_ADD_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}

		}
		break;


	case CW_FRIEND_DEL_ACK:// 친구 삭제 성공
		{
			MSG_CW_FRIEND_DEL_ACK * pRecvPacket = (MSG_CW_FRIEND_DEL_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszDeletedName,  MAX_CHARNAME_LENGTH);

			DeleteFriend(szWideString);
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
            if (GetMode() == eCM_FRIEND)
            {
                if (m_pUICommunityDlg)
                {
                    m_pUICommunityDlg->RefreshOnlineMember(eCM_FRIEND);
                }
            }
		}
		break;

	case CW_FRIEND_DEL_NAK:// 친구 삭제 실패
		{
			MSG_CW_FRIEND_DEL_NAK * pRecvPacket = (MSG_CW_FRIEND_DEL_NAK *)pMsg;

			if(pRecvPacket->m_byResult == MSG_CW_FRIEND_DEL_NAK::NOT_FRIEND_USER)
			{
				//	해당 유저가 없습니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_USER, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else
			{
				//	친구 삭제 실패
				g_InterfaceManager.GetInterfaceString( eST_FAILED_DELETE_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
		}
		break;

	case CW_FRIEND_BLOCK_ACK:// 차단 허용
		{
			MSG_CW_FRIEND_BLOCK_ACK * pRecvPacket = (MSG_CW_FRIEND_BLOCK_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString ,pRecvPacket->ptszBlockedName, MAX_CHARNAME_LENGTH);

			AddBlockFriend(szWideString);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
		}
		break;

	case CW_FRIEND_BLOCK_NAK:// 차단 실패
		{
			MSG_CW_FRIEND_BLOCK_NAK * pRecvPacket = (MSG_CW_FRIEND_BLOCK_NAK *)pMsg;
			if(pRecvPacket->m_byResult == MSG_CW_FRIEND_BLOCK_NAK::NO_USER)
			{
				//	해당 유저가 없습니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_USER, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else
			{
				//	차단 실패
				g_InterfaceManager.GetInterfaceString( eST_FAILED_REJECTION, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}

		}
		break;

	case CW_FRIEND_BLOCK_FREE_ACK:
		{
			MSG_CW_FRIEND_BLOCK_FREE_ACK * pRecvPacket = (MSG_CW_FRIEND_BLOCK_FREE_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszBlockedName, MAX_CHARNAME_LENGTH); 

			DeleteBlockFriend(szWideString);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

		}
		break;

	case CW_FRIEND_BLOCK_FREE_NAK:
		{
			MSG_CW_FRIEND_BLOCK_FREE_NAK * pRecvPacket = (MSG_CW_FRIEND_BLOCK_FREE_NAK *)pMsg;
			if(pRecvPacket->m_byResult==MSG_CW_FRIEND_BLOCK_FREE_NAK::NO_USER)
			{
				//	해당 유저가 없습니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_USER, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else if(pRecvPacket->m_byResult==MSG_CW_FRIEND_BLOCK_FREE_NAK::NOT_BLOCKED)
			{
				//	차단된 유저가 아닙니다.
				g_InterfaceManager.GetInterfaceString( eST_NOT_BLOCKED_USER, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
			else
			{
				//	차단 해제 실패
				g_InterfaceManager.GetInterfaceString( eST_FAILED_BLOCK_FREE, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
			}
		}
		break;


	case CW_FRIEND_CHAT_BRD:// 친구가 나에게 채팅을 시도해옴
		{
			MSG_CW_FRIEND_CHAT_BRD * pRecvPacket = (MSG_CW_FRIEND_CHAT_BRD *)pMsg;

			if(IsRecvMessage())
			{
				break;
			}

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszFriendNameFrom, MAX_CHARNAME_LENGTH);

			int index = FindChatDialog(szWideString);

			TCHAR buffer[MSG_CW_FRIEND_CHAT_BRD::_MAX_CHATMSG_SIZE + 1] = {0,};
			StrnCopy(buffer, pRecvPacket->ptszChatMsg ,pRecvPacket->byMsgLength);

			if(index == -1)//없으면 창 생성 
			{
				int i = FindChatData(szWideString);
				if(i>=0)
				{
					AddChatMsg(szWideString,buffer,RGBA(131,190,126,255));
				}
				else
				{
					FRIEND_CHAT_DATA temp;
					temp.dwDlgId = -1;
					temp.vstrChatMsg.clear();
					++m_iNewFriendChat;
					temp.name = szWideString;	

					AddFriendChatData(temp);
					AddChatMsg(szWideString,buffer,RGBA(131,190,126,255));

					g_SoundEffect.PlayUI( UI_SOUND_QUEST_ACCEPT );
				}
			}
			else
			{
				if(m_chatDlgArray[index].dwDlgId >= 0)
				{
					//1:1 대화창 연결
					uiCommunityChat *pFriendChatDialog = (uiCommunityChat *)g_InterfaceManager.GetDialog((WzID)m_chatDlgArray[index].dwDlgId);

					if(pFriendChatDialog)
					{
						pFriendChatDialog->AddString(szWideString,buffer,RGBA(131,190,126,255));
					}
				}
			}
		}
		break;


	case CW_FRIEND_CHAT_NAK:// 친구 채팅 실패
		{
			MSG_CW_FRIEND_CHAT_NAK * pRecvPacket = (MSG_CW_FRIEND_CHAT_NAK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->ptszFriendNameTo , MAX_CHARNAME_LENGTH);

			int index = FindChatDialog(szWideString);

			if(index != -1)
			{
				//1:1 대화창 연결
				uiCommunityChat *pFriendChatDialog = (uiCommunityChat *)g_InterfaceManager.GetDialog((WzID)m_chatDlgArray[index].dwDlgId);

				if(pFriendChatDialog)
				{
					if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::NOT_FRIEND)
					{
						//	친구가 아닙니다.
						g_InterfaceManager.GetInterfaceString( eST_NOT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						pFriendChatDialog->AddString(szMessage,WzColor_RGBA(240,201,121, 255));
					}
					else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::OFFLINE_FRIEND)
					{
						//	대화 상대가 오프라인 입니다.
						g_InterfaceManager.GetInterfaceString( eST_OFFLINE_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						pFriendChatDialog->AddString(szMessage,WzColor_RGBA(240,201,121, 255));
					}
					else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::BLOCKED_FRIEND)
					{
						//	차단 상태 입니다.
						g_InterfaceManager.GetInterfaceString( eST_REJECTION_STATE, szMessage, INTERFACE_STRING_LENGTH );
						pFriendChatDialog->AddString(szMessage,WzColor_RGBA(240,201,121, 255));
					}
					else
					{
						//	친구 채팅 실패
						g_InterfaceManager.GetInterfaceString( eST_FAILED_CHAT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						pFriendChatDialog->AddString(szMessage,WzColor_RGBA(240,201,121, 255));
					}

				}
				else
				{
					if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::NOT_FRIEND)
					{
						//	친구가 아닙니다.
						g_InterfaceManager.GetInterfaceString( eST_NOT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
					else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::OFFLINE_FRIEND)
					{
						//	대화 상대가 오프라인 입니다.
						g_InterfaceManager.GetInterfaceString( eST_OFFLINE_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
					else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::BLOCKED_FRIEND)
					{
						//	차단 상태 입니다.
						g_InterfaceManager.GetInterfaceString( eST_REJECTION_STATE, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
					else
					{
						//	친구 채팅 실패
						g_InterfaceManager.GetInterfaceString( eST_FAILED_CHAT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
					}
				}
			}
			else
			{
				if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::NOT_FRIEND)
				{
					//	친구가 아닙니다.
					g_InterfaceManager.GetInterfaceString( eST_NOT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::OFFLINE_FRIEND)
				{
					//	대화 상대가 오프라인 입니다.
					g_InterfaceManager.GetInterfaceString( eST_OFFLINE_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				else if(pRecvPacket->m_byResult == MSG_CW_FRIEND_CHAT_NAK::BLOCKED_FRIEND)
				{
					//	차단 상태 입니다.
					g_InterfaceManager.GetInterfaceString( eST_REJECTION_STATE, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				else
				{
					//	친구 채팅 실패
					g_InterfaceManager.GetInterfaceString( eST_FAILED_CHAT_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}

			}
		}
		break;

	case CW_FRIEND_CHATBLOCK_ACK:
		{
			MSG_CW_FRIEND_CHATBLOCK_ACK * pRecvPacket = (MSG_CW_FRIEND_CHATBLOCK_ACK *)pMsg;

			SetMessageRejection(pRecvPacket->m_bNowBlock);
		}
		break;

	case CW_FRIEND_CHATBLOCK_NAK:
		{
			BOOL rt = IsRecvMessage();
			SetMessageRejection(!rt);
		}
		break;

	case CW_FRIEND_LEVELUP_BRD:	// 친구 레벨업 알림
		{
			MSG_CW_FRIEND_LEVELUP_BRD *pRecvPacket = (MSG_CW_FRIEND_LEVELUP_BRD *)pMsg;

			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1];
			StrnCopy( szConvertCharName , pRecvPacket->m_ptszCharName , MAX_CHARNAME_LENGTH );
			SetFriendLevel(szConvertCharName, pRecvPacket->m_Level);
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

		}
		break;

	case CW_FRIEND_STATUS_ANS:
		{
            OnRecvCW_Friend_STATUS_ANS(pMsg);
		}
		break;

	case CW_FRIEND_STATUS_NAK:
		{
			MSG_CW_FRIEND_STATUS_NAK *pRecvPacket = (MSG_CW_FRIEND_STATUS_NAK *)pMsg;

			this->_processResultCode(pRecvPacket->m_byErrorCode);
		}
		break;

	case CW_FRIEND_STATUS_JOIN_ACK:
		{
			MSG_CW_FRIEND_STATUS_JOIN_ACK *pRecvPacket = (MSG_CW_FRIEND_STATUS_JOIN_ACK *)pMsg;
			
			if(!IsWaitJoinGame())
			{
				SetWaitJoinGame(true);
				
				SetRqstJoinGame(true);
				MSG_CG_ZONE_LIST_REFRESH_SYN sync;
				ZeroMemory(&sync, sizeof(sync));
				sync.m_byCategory = CG_ZONE;
				sync.m_byProtocol = CG_ZONE_LIST_REFRESH_SYN;
				sync.m_RoomKey = (KEYTYPE)pRecvPacket->m_dwZoneKey;
				sync.m_byRoomType = eZONETYPE_LOBBY;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

			}
		}
		break;

	case CW_FRIEND_STATUS_JOIN_NAK:
		{
			SetWaitJoinGame(false);
			
			MSG_CW_FRIEND_STATUS_JOIN_NAK *pRecvPacket = (MSG_CW_FRIEND_STATUS_JOIN_NAK *)pMsg;
			
			this->_processResultCode(pRecvPacket->m_byErrorCode);
		}
		break;


	case CW_FRIENDINVITATE_INVITATELIST_CMD:	//친구 목록이 전송된 다음 전송되는 패킷, m_GuidList에 친구의 캐릭터 Guid가 들어갑니다.
		{										//만일 m_GuidList에 있는 캐릭터 GUID가 나의 친구목록에 없다면 무시됩니다.
			MSG_CW_FRIENDINVITATE_INVITATELIST_CMD * pRecvPacket = (MSG_CW_FRIENDINVITATE_INVITATELIST_CMD *)pMsg;
			if(pRecvPacket)
			{
				int iCnt = pRecvPacket->m_InvitateFriendInfo.m_byListCount;
				iCnt = min(iCnt, MAX_FRIEND_STAT_NUM);

				BYTE byFriendSts = pRecvPacket->m_InvitateFriendInfo.m_FriendSts;

				for(int i = 0; i < iCnt ; ++i)
				{					
					for(int j = 0; j < m_FriendArray.Size(); ++j)
					{
						if(m_FriendArray[j].dwFriendGuid == pRecvPacket->m_InvitateFriendInfo.m_GuidList[i])
						{
							m_FriendArray[j].byFriendSts = byFriendSts;
						}
					}
				}
			}
		}
		break;


#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    case CW_FRIEND_MEMO_UPDATE_ACK:
        {
            MSG_CW_FRIEND_MEMO_UPDATE_ACK* pk = static_cast<MSG_CW_FRIEND_MEMO_UPDATE_ACK*>(pMsg);
            if (m_pUICommunityInfo != NULL)
            {
                m_pUICommunityInfo->FriendMemo_InputButtonState(uiCommunityInfo::kFriendMemoButtonStatus_Disable);
            }
        }
        break;

    case CW_FRIEND_MEMO_UPDATE_NAK:
        {		
            _processResultCode((static_cast<MSG_CW_FRIEND_MEMO_UPDATE_NAK*>(pMsg))->m_byErrorCode);

            if (m_pUICommunityInfo != NULL)
            {
                m_pUICommunityInfo->FriendMemo_InputButtonState(uiCommunityInfo::kFriendMemoButtonStatus_Disable);
            }
        }
        break;
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

	default:
		assert(!"월드 서버에서 정의되지 않은 프로토콜이 왔다.!!");
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRecvCW_Revenge(MSG_BASE *pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szWideString[INTERFACE_STRING_LENGTH] = {0,};

	if(!m_pUICommunityDlg)
		return;

	switch(pMsg->m_byProtocol)
	{
	case CW_REVENGE_ADD_CMD:
		{
			MSG_CW_REVENGE_ADD_CMD * pRecvPacket = (MSG_CW_REVENGE_ADD_CMD *)pMsg;
		
			m_byRevengePoint=pRecvPacket->m_byRevengePoint;
			ZeroMemory(szWideString, sizeof(szWideString));

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
            StrnCopy(szWideString , pRecvPacket->m_sRevengeStat.sPacketInfo.ptszRevengeName, 
                    MAX_CHARNAME_LENGTH);
			AddRevenge(szWideString,
                        pRecvPacket->m_sRevengeStat.sPacketInfo.Level,
                        pRecvPacket->m_sRevengeStat.sPacketInfo.byClass,
                        pRecvPacket->m_sRevengeStat.sPacketInfo.bOnline, 
                        pRecvPacket->m_sRevengeStat.sPacketInfo.byChannelID,
                        pRecvPacket->m_sRevengeStat.dwRevengeCharGuid);
#else
            StrnCopy(szWideString , pRecvPacket->m_sRevengeStat.ptszRevengeName, 
                    MAX_CHARNAME_LENGTH);
            AddRevenge(szWideString,
                        pRecvPacket->m_sRevengeStat.Level,
                        pRecvPacket->m_sRevengeStat.byClass,
                        pRecvPacket->m_sRevengeStat.bOnline, 
                        pRecvPacket->m_sRevengeStat.byChannelID,
                        0);
#endif

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			//5271	%s님을 리벤지 리스트에 추가하였습니다.
			TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

			g_InterfaceManager.GetInterfaceString(5271, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf (szString, INTERFACE_STRING_LENGTH-1, szTemp, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szString );


		}
		break;


	case CW_REVENGE_DEL_ACK:
		{
			MSG_CW_REVENGE_DEL_ACK * pRecvPacket = (MSG_CW_REVENGE_DEL_ACK *)pMsg;
			
			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->m_ptszCharName, MAX_CHARNAME_LENGTH);

			DeleteRevenge(szWideString);
			
			m_pUICommunityDlg->RefreshFriendList();

			//5272	%s님을 리벤지 리스트에서 삭제하였습니다.
			TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

			g_InterfaceManager.GetInterfaceString(5272, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf (szString, INTERFACE_STRING_LENGTH-1, szTemp, szWideString);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szString );


		}	
		break;

	case CW_REVENGE_DEL_NAK:
		{
			MSG_CW_REVENGE_DEL_NAK * pRecvPacket = (MSG_CW_REVENGE_DEL_NAK *)pMsg;
			
			ShowRevengeResultCode(pRecvPacket->m_byErrorCode);
		}
		break;


	case CW_REVENGE_LIST_CMD:
		{
			MSG_CW_REVENGE_LIST_CMD * pRecvPacket = (MSG_CW_REVENGE_LIST_CMD *)pMsg;
			
			m_byRevengePoint=pRecvPacket->byRevengePoint;

			for(int i=0;i<pRecvPacket->byRevengeStatNum;++i)
			{
				ZeroMemory(szWideString, sizeof(szWideString));

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
                StrnCopy(szWideString , pRecvPacket->sRevengeStat[i].sPacketInfo.ptszRevengeName, 
                        MAX_CHARNAME_LENGTH);
				AddRevenge(szWideString,
                    pRecvPacket->sRevengeStat[i].sPacketInfo.Level,
                    pRecvPacket->sRevengeStat[i].sPacketInfo.byClass,
                    pRecvPacket->sRevengeStat[i].sPacketInfo.bOnline, 
                    pRecvPacket->sRevengeStat[i].sPacketInfo.byChannelID,
                    pRecvPacket->sRevengeStat[i].dwRevengeCharGuid);	
#else
                StrnCopy(szWideString , pRecvPacket->sRevengeStat[i].ptszRevengeName, 
                        MAX_CHARNAME_LENGTH);
                AddRevenge(szWideString,
                    pRecvPacket->sRevengeStat[i].Level,
                    pRecvPacket->sRevengeStat[i].byClass,
                    pRecvPacket->sRevengeStat[i].bOnline, 
                    pRecvPacket->sRevengeStat[i].byChannelID,
                    0);	
#endif
			}

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

			this->SEND_CW_MEMO_LIST_REQ();
		}
		break;

	case CW_REVENGE_LOGIN_BRD:
		{
			MSG_CW_REVENGE_LOGIN_BRD * pRecvPacket = (MSG_CW_REVENGE_LOGIN_BRD *)pMsg;

			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1 ]= {0,};
			StrnCopy( szConvertCharName , pRecvPacket->m_ptszCharName, MAX_CHARNAME_LENGTH );

			SetRevengeOnline(szConvertCharName, TRUE, pRecvPacket->byChannelID);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
		}	
		break;

	case CW_REVENGE_LOGOUT_BRD:
		{
			MSG_CW_REVENGE_LOGOUT_BRD * pRecvPacket = (MSG_CW_REVENGE_LOGOUT_BRD *)pMsg;
			
			TCHAR szConvertCharName[MAX_CHARNAME_LENGTH +1] = {0,};
			StrnCopy( szConvertCharName , pRecvPacket->ptszCharName, MAX_CHARNAME_LENGTH );

			SetRevengeOnline(szConvertCharName, FALSE, 0);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();

		}
		break;

	case CW_REVENGE_LEVELUP_BRD:
		{
			MSG_CW_REVENGE_LEVELUP_BRD* pRecvPacket = (MSG_CW_REVENGE_LEVELUP_BRD*)pMsg;
			TCHAR szCharName[MAX_CHARNAME_LENGTH +1] = {0,};
			StrnCopy(szCharName , pRecvPacket->m_ptszCharName ,MAX_CHARNAME_LENGTH );
			SetRevengeLevel(szCharName, pRecvPacket->m_Level);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshFriendList();
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddMailData(STRUCT_SUBJECT_PACKETINFO *pSubject)
{
	for(unsigned int i=0; i<m_vecMailData.size(); ++i)
	{
		if(pSubject->m_MemoGuid == m_vecMailData[i].m_subject.m_MemoGuid)
			return;
	}

	if(m_vecMailData.size() >= MAX_MEMO_NUM)
	{
		// 쪽지 최대갯수를 넘겼을 경우
		// 오래된 쪽지부터 지운다 guid가 작은것 부터
		DelOldMail();
	}

	MAIL_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.m_bRecvMemo = false;
	CopyMemory(&(data.m_subject), pSubject, sizeof(STRUCT_SUBJECT_PACKETINFO));

	m_vecMailData.push_back(data);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddMailDataBegin(STRUCT_SUBJECT_PACKETINFO *pSubject)
{
	for(unsigned int i=0; i<m_vecMailData.size(); ++i)
	{
		if(pSubject->m_MemoGuid == m_vecMailData[i].m_subject.m_MemoGuid)
			return;
	}

	if(m_vecMailData.size() >= MAX_MEMO_NUM)
	{
		// 쪽지 최대갯수를 넘겼을 경우
		// 오래된 쪽지부터 지운다 guid가 작은것 부터
		DelOldMail();
	}

	MAIL_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.m_bRecvMemo = false;
	CopyMemory(&(data.m_subject), pSubject, sizeof(STRUCT_SUBJECT_PACKETINFO));

	std::vector<MAIL_DATA>::iterator it = m_vecMailData.begin();
	m_vecMailData.insert(it, data);
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::AddMailView(const MSG_CW_MEMO_VIEW_ANS* memo_view)
{
    MAIL_DATA* mail_data = NULL;
    size_t mail_data_index = m_vecMailData.size();
	for(size_t index = 0; index < mail_data_index; ++index)
	{
        MAIL_DATA* mail_data = &(m_vecMailData[index]);
		if(memo_view->m_MemoGuid == mail_data->m_subject.m_MemoGuid)
		{
			mail_data->m_bRecvMemo = true;
			mail_data->m_subject.m_byRead = 1;

			StrnCopy(m_vecMailData[index].m_ptszMemo, 
                     memo_view->m_ptszMemo, 
                     _tcslen(memo_view->m_ptszMemo));
            mail_data->m_recvItemCode = memo_view->m_recvItemCode;
            mail_data->m_amountCost = memo_view->m_amountCost;
            // 받아야할 아이템/하임이 있을 경우 요청한다. :관련하여 UI가 추가되면 위치를 옮겨야함
            if (mail_data->m_amountCost != 0)
            {
                SEND_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ(mail_data);
            }            
			break;
		}
	}
}

void uiCommunityMan::SEND_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ(const MAIL_DATA* mail_data)
{
    MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ send_packet;
    send_packet.memo_guid_ = mail_data->m_subject.m_MemoGuid;
    StrnCopy(send_packet.memo_contents_, mail_data->m_ptszMemo, MAX_MEMO_STRLENGTH);
    send_packet.memo_contents_[MAX_MEMO_STRLENGTH - 1] = '\0';
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::DelOldMail()
{
	MEMOGUID delGuid = 0;

	std::vector<MAIL_DATA>::iterator it;
	for(it = m_vecMailData.begin(); it != m_vecMailData.end(); ++it)
	{
		if(delGuid == 0)
		{
			// 처음
			delGuid = (*it).m_subject.m_MemoGuid;
		}
		else
		{
			// 제일 작은 guid를 찾는다
			if(delGuid > (*it).m_subject.m_MemoGuid)
				delGuid = (*it).m_subject.m_MemoGuid;
		}
	}

	if(delGuid > 0)
	{
		DelMail(delGuid, false);
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		//5771 쪽지 최대 개수 초과로 인해 오래된 쪽지를 삭제했습니다.
		g_InterfaceManager.GetInterfaceString(5771 , szMessage ,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_SysMsg(szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::DelMail(MEMOGUID guid, bool bMsgShow/* = true*/)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	std::vector<MAIL_DATA>::iterator it;
	for(it = m_vecMailData.begin(); it != m_vecMailData.end(); ++it)
	{
		if((*it).m_subject.m_MemoGuid == guid)
		{
			m_vecMailData.erase(it);
			if(bMsgShow)
			{
				// 5772	쪽지를 삭제했습니다.
				g_InterfaceManager.GetInterfaceString( 5772 , szMessage ,INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_SysMsg(szMessage);
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::DelAllMailData(bool bMsgShow/* = true*/)
{
	m_vecMailData.clear();

	if(bMsgShow)
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		//5773	전체 쪽지를 삭제했습니다.
		g_InterfaceManager.GetInterfaceString( 5773 , szMessage ,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_SysMsg(szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
MEMOGUID uiCommunityMan::GetCurMailGuid()
{
	if(m_pUICommunityDlg)
	{
		m_iCurMailSelect = m_pUICommunityDlg->GetCurListSel();

		if((m_iCurMailSelect >= 0) && ((unsigned int)m_iCurMailSelect < m_vecMailData.size()))
		{
			MAIL_DATA *pData = m_pUICommunityDlg->GetCurMailData(m_iCurMailSelect);
			if(pData)
			{
				return pData->m_subject.m_MemoGuid;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::OnRecvCW_Memo(MSG_BASE *pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szWideString[INTERFACE_STRING_LENGTH] = {0,};

	if(!m_pUICommunityDlg)
		return;

	switch(pMsg->m_byProtocol)
	{
	case CW_MEMO_SEND_ACK:	// 쪽지 전송 성공
		{
			MSG_CW_MEMO_SEND_ACK *pRecvPacket = (MSG_CW_MEMO_SEND_ACK *)pMsg;

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->m_ptszRecvName, MAX_CHARNAME_LENGTH);
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
			//5774	%s님에게 쪽지를 전송했습니다.
			g_InterfaceManager.GetInterfaceString( 5774 , szTemp ,INTERFACE_STRING_LENGTH);

			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , szWideString);
			GlobalFunc::OutputMsg_SysMsg(szMessage);
			m_pUICommunityWriteMail->ShowInterface(FALSE);
		}
		break;

	case CW_MEMO_SEND_NAK:	// 쪽지 전송 실패
		{
			MSG_CW_MEMO_SEND_NAK *pRecvPacket = (MSG_CW_MEMO_SEND_NAK *)pMsg;
			ProcessMemoErrCode(pRecvPacket->m_byErrorCode);
		}
		break;

	case CW_MEMO_LIST_ANS:	// 쪽지 목록
		{
			m_bRecvMailInfo = true;

			MSG_CW_MEMO_LIST_ANS *pRecvPacket = (MSG_CW_MEMO_LIST_ANS *)pMsg;
			if(pRecvPacket->m_byMemoPageCnt == 1)
			{
				m_vecMailData.clear();
			}

			for(int i=0; i<pRecvPacket->m_byMemoCnt; ++i)
			{
				AddMailDataBegin(&(pRecvPacket->m_Subject[i]));
			}

			if(pRecvPacket->m_byMemoPageCnt == pRecvPacket->m_byMemoPageCnt)
			{
				// 전송 완료
				if(m_pUICommunityDlg->IsVisibleWindow())
					m_pUICommunityDlg->RefreshMailList();
			}

			if( HaveNewNote() )
			{
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 5449 ) );
			}
			
		}
		break;

	case CW_MEMO_LIST_NAK:	// 쪽지 목록 실패
		{
		}
		break;

	case CW_MEMO_VIEW_ANS:	// 쪽지 내용 확인
		{
			UnsetPKBool(ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ);
			MSG_CW_MEMO_VIEW_ANS *pRecvPacket = (MSG_CW_MEMO_VIEW_ANS *)pMsg;

			AddMailView(pRecvPacket);

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshMailList();

			ShowReadMailDlg(pRecvPacket->m_MemoGuid);
		}
		break;

	case CW_MEMO_VIEW_NAK:	// 쪽지 내용 실패
		{
			UnsetPKBool(ePKBOOL_COMMUNITY_CW_MEMO_VIEW_REQ);
			MSG_CW_MEMO_VIEW_NAK *pRecvPacket = (MSG_CW_MEMO_VIEW_NAK *)pMsg;
			ProcessMemoErrCode(pRecvPacket->m_byErrorCode);
		}
		break;

	case CW_MEMO_DELETE_ACK:	// 쪽지 삭제
		{
			MSG_CW_MEMO_DELETE_ACK *pRecvPacket = (MSG_CW_MEMO_DELETE_ACK *)pMsg;
			
			if(pRecvPacket->m_MemoGuid == 0) 
			{
				DelAllMailData();
			}
			else
			{
				DelMail(pRecvPacket->m_MemoGuid);
			}
			
			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshMailList();
		}
		break;

	case CW_MEMO_DELETE_NAK:	// 쪽지 삭제 실패
		{
			MSG_CW_MEMO_DELETE_NAK *pRecvPacket = (MSG_CW_MEMO_DELETE_NAK *)pMsg;
			ProcessMemoErrCode(pRecvPacket->m_byErrorCode);
		}
		break;

	case CW_MEMO_NEWMEMO_BRD:	// 새 쪽지 알림
		{
			MSG_CW_MEMO_NEWMEMO_BRD *pRecvPacket = (MSG_CW_MEMO_NEWMEMO_BRD *)pMsg;

			AddMailDataBegin(&(pRecvPacket->m_Subject));

			if(m_pUICommunityDlg->IsVisibleWindow())
				m_pUICommunityDlg->RefreshMailList();

			ZeroMemory(szWideString, sizeof(szWideString));
			StrnCopy(szWideString , pRecvPacket->m_Subject.m_SendName, MAX_CHARNAME_LENGTH);

			if(pRecvPacket->m_Subject.m_MemoType==STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
			{
				char *pStr = pRecvPacket->m_Subject.m_ptszSubject;

				//0번째인덱스는 어떤종류의 시스템메시지인지 나타내는값이다(서버와의 규약)
				int nSystemType = (int)ParseMemoInfo(pStr,0,MAX_SUBJECT_STRLENGTH-1);

				switch(nSystemType)
				{
				case STRUCT_SUBJECT_PACKETINFO::NORMAL_TYPE:
					{
					}
					break;
				case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE://하임행운일경우:발신자명을 맵핑시켜준다.
                case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE_NEW:
					{
						//1885:하임행운
						g_InterfaceManager.GetInterfaceString( 1885	 , szWideString);
					}
					break;

				case STRUCT_SUBJECT_PACKETINFO::GROUP_TYPE ://그룹일경우:발신자명을 맵핑시켜준다.
					{
						//314: 그룹관리자
						g_InterfaceManager.GetInterfaceString(314,szWideString);
					}
					break;

				case STRUCT_SUBJECT_PACKETINFO::FRINVATE_START_TYPE ://친구이벤트:발신자명을 맵핑시켜준다.
				case STRUCT_SUBJECT_PACKETINFO::FRINVATE_END_TYPE:
				case STRUCT_SUBJECT_PACKETINFO::FRINVATE_PRESENT_TYPE:
				case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_START_TYPE:
				case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_END_TYPE:
				case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_PRESENT_TYPE:
					{
						//4007: 친구초대 이벤트 요원
						g_InterfaceManager.GetInterfaceString(4007,szWideString);
					}
					break;
				case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN:
				case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD:
                case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN_NEW:
                case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD_NEW:
					{
						// 발신자맵핑 3377 피의성전 관리자
						g_InterfaceManager.GetInterfaceString(3377,szWideString);
					}
					break;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                case STRUCT_SUBJECT_PACKETINFO::WORLDRANKING_TYPE_REWARD_ALARAM:
                    {
                        // 5478 월드랭킹                        
                        g_InterfaceManager.GetInterfaceString(5478, szWideString);
                    }
                    break;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                case STRUCT_SUBJECT_PACKETINFO::GUILD_JOIN_REQUEST:
                    {
                        // 1444 : 길드 관리인
                        g_InterfaceManager.GetInterfaceString(1444, szWideString);
                    }
                    break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
				case STRUCT_SUBJECT_PACKETINFO::SUN_RANKING_SETTLE_RESULT_TYPE:
					{
						// 3599 : SUN 랭킹
						g_InterfaceManager.GetInterfaceString(3599, szWideString);
					}
					break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM

				default:
					break;
				}
			}

			TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
			//5775	%s님에게 쪽지를 받았습니다.
			g_InterfaceManager.GetInterfaceString( 5775	 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , szWideString);
			GlobalFunc::OutputMsg_SysMsg(szMessage);

		}
		break;

	}
}

//------------------------------------------------------------------------------
/**
*/

BOOL uiCommunityMan::HaveNewNote()
{
	MAIL_DATA_VECTOR_ITRTYPE itr = m_vecMailData.begin();
	MAIL_DATA_VECTOR_ITRTYPE end = m_vecMailData.end();

	for( ; itr != end; ++itr )
	{
		if( itr->m_subject.m_byRead == 0 )	// 읽지 않은게 한게 라도 있으면 TRUE 리턴
			return TRUE;
	}

	return FALSE;
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::ProcessMemoErrCode(BYTE byErrCode)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	bool bCloseMemo = true;

	switch(byErrCode)
	{
	case RC_MEMO_SUCCESS:	// 성공
		break;

	case RC_MEMO_FAILED:	// 실패
		{
			//5776	쪽지: 실패했습니다.
			g_InterfaceManager.GetInterfaceString( 5776 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_MEMO_NOUSER:	// 해당 사용자 없음
		{
			// 5777	쪽지: 해당 사용자가 없습니다.
			g_InterfaceManager.GetInterfaceString( 5777 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_MEMO_NOMEMO:	// 해당하는 쪽지가 없음
		{
			//5778	쪽지: 해당 쪽지가 없습니다.
			g_InterfaceManager.GetInterfaceString( 5778 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_MEMO_WAIT_INFO:		// 일정 시간 안에 계속 요청할 수 없음
		{
			__asm nop;
		}
		break;

	case RC_MEMO_DOING_TRANSACTION:
		{
			__asm nop;
		}
		break;

	default:
		{
			//5779	쪽지: 오류입니다.
			g_InterfaceManager.GetInterfaceString( 5779 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	}
	if(bCloseMemo)
	{
		m_pUICommunityWriteMail->ShowInterface(FALSE);
	}

	if(szMessage[0] != NULL)
	{
		GlobalFunc::OutputMsg_SysMsg(szMessage);
	}
	
}



//------------------------------------------------------------------------------
/**
*/
void uiCommunityMan::RqstRefreshFriendInfo()
{
	if(_tcslen(m_FriendInfo.m_ptszCharName) > 0)
	{
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO    //! Send_CW_GROUP_STATUS_REQ 사용하지 않는다.
        Send_CW_FRIEND_STATUS_REQ(m_FriendInfo.m_ptszCharName, m_FriendInfo.m_dwCharGuid);
#else
		if(IsOnline(m_FriendInfo.m_ptszCharName))
		{
    #ifdef __KR_001355_20090624_GROUPINGSYSTEM_ALL
			if(GetPendingInfoMode() == eCM_GROUP )
			{
                Send_CW_GROUP_STATUS_REQ(m_FriendInfo.m_ptszCharName);
			}
			else
    #endif//__KR_001355_20090624_GROUPINGSYSTEM_ALL :elflee
			{
				Send_CW_FRIEND_STATUS_REQ(m_FriendInfo.m_ptszCharName, m_FriendInfo.m_dwCharGuid);
			}
		}
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO 
	}
}
//------------------------------------------------------------------------------
/**
*/
//------------------------------------------------------------------------------
//! 길드창에서 double 클릭시도 사용할수 있게 함수로 뺌
void uiCommunityMan::OnRecvCW_Friend_STATUS_ANS(MSG_BASE *pMsg)
{
    MSG_CW_FRIEND_STATUS_ANS *pRecvPacket = (MSG_CW_FRIEND_STATUS_ANS *)pMsg;
    FRIEND_INFO_DATA info;
    ZeroMemory(&info, sizeof(info));
    StrnCopy(info.m_ptszCharName, pRecvPacket->m_ptszCharName, MAX_CHARNAME_LENGTH);
    StrnCopy(info.m_tszGuildName, pRecvPacket->m_tszGuildName, MAX_GUILDNAME_LENGTH);
    info.m_byChannel	= pRecvPacket->m_byChannel;
    info.m_byZoneType	= pRecvPacket->m_byZoneType;
    info.m_dwZoneKey	= pRecvPacket->m_dwZoneKey;
    info.m_MapCode		= pRecvPacket->m_MapCode;
    info.m_byDuty		= pRecvPacket->m_byDuty;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    StrnCopy(info.m_ptszMemo, pRecvPacket->m_ptszMemo, MAX_FRIEND_MEMO_LENGHT);
    info.m_dwCharGuid = GetGUID(info.m_ptszCharName);
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

    SetFriendInfo(info);
    ShowFriendInfoDlg();							
}

STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE uiCommunityMan::GetMemoType(MEMOGUID memo_guid)
{
    STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE memo_type = STRUCT_SUBJECT_PACKETINFO::NORMAL_MEMO;

    if (memo_guid != 0)
    {
        std::vector<MAIL_DATA>::iterator memo_data_itr = m_vecMailData.begin();
        std::vector<MAIL_DATA>::iterator memo_data_itr_end = m_vecMailData.end();
        for ( ; memo_data_itr != memo_data_itr_end; ++memo_data_itr)
        {
            if (memo_guid == (*memo_data_itr).m_subject.m_MemoGuid)
            {
                memo_type = (*memo_data_itr) .m_subject.m_MemoType;
                break;
            }
        }
    }

    return memo_type;
}
