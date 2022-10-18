#include "StdAfx.h"
#include "GroupManager.h"
#include "User.h"
#include "UserManager.h"
#include "Struct.h"

#include "WorldServer.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include "PacketStruct_DW.h"
#include "PacketStruct_AW.h"

#include <SolarDateTime.h>

#include <time.h>

GroupManager::GroupManager(void)
{
    m_pGroupPool = new util::CMemoryPoolFactory<STRUCT_GROUP_MEMBER_INFO>;
    m_pGroupPool->Initialize(50, 50);
}

GroupManager::~GroupManager(void)
{
    delete m_pGroupPool;
}

bool 
GroupManager::CanAddGroupMember(User* request_user, User* dest_user, MSG_BASE* msg)
{
    MSG_CW_GROUP_ADD_SYN *recv_msg = static_cast<MSG_CW_GROUP_ADD_SYN*>(msg);

    MSG_CW_GROUP_ADD_NAK nak_msg;

    if (request_user == NULL || dest_user == NULL)
    {
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ERR_DEFAULT;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found user");
    }
    else if (request_user->IsGroupTransaction() == TRUE)
    {
        //-- 해당 유저가 다른 유저와 그룹 신청을 시도한 상태
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Transaction error", recv_msg->ptszMemeberName);
    }
    else if (dest_user->GetAddBlockInfo() == TRUE)
    {
        //-- 그룹 맴버가 그룹 요청 거부 토글을 활성화 시켰다.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ON_TOGGLE_ADDBLOCK;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Ad request block user", recv_msg->ptszMemeberName);
    }
    else if (dest_user->IsGroupTransaction() == TRUE)
    {
        //-- 상대방에게 누군가 이미 요청중이다.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already group request", recv_msg->ptszMemeberName);
    }
    else if (!_tcsnicmp(request_user->GetCharName().c_str(), 
        recv_msg->ptszMemeberName, MAX_CHARNAME_LENGTH))
    {
        //-- 추가하려는 그룹 맴버의 이름과 자신의 이름을 비교
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_ADD_ME;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not request to myself", recv_msg->ptszMemeberName);
    }
    else if (dest_user->GetCharLevel() >= request_user->GetCharLevel())
    {
        //-- 추가 하려는 그룹 맴버의 레벨이 같거나 높은 경우 실패
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
    }
#if defined(_CHINA)
    else if (request_user->GetCharLevel() < HIGH_MEMBER_LEVEL || 
        dest_user->GetCharLevel() < LOW_MEMBER_LEVEL)
    {
        //-- 중국 요청으로 레벨이 80이상 상위 맴버, 10이상 하위 맴버 가능
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
    }
#endif
    else if (GetGroupHighMember(dest_user) == GROUP_HAVE_ONLINE_HIGH ||
        GetGroupHighMember(dest_user) == GROUP_HAVE_OFFLINE_HIGH)
    {
        //-- 추가 하려는 케릭터가 상위 멤버를 가지고 있는가?
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already had high member", recv_msg->ptszMemeberName);
    }
    else if (FindGroupMember(request_user, recv_msg->ptszMemeberName))
    {
        //-- 맴버가 그룹에 존재 할때
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already Member", recv_msg->ptszMemeberName);
    }
    else if (GetLowMemberCount(request_user) >= MAX_GROUP_MEMBER_LOW_MAX)
    {
        //-- 총 그룹원 6명 하위 맴버는 5명까지 가능하다.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::GROUP_OVERFLOW;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s OverFlow member count", recv_msg->ptszMemeberName);
    }
    else
    {
        //-- 그룹 맴버 등록 가능
        return true;
    }

    //-- 그룹 맴버 등록 실패 메세지 Send
    request_user->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
    return false;
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO 인자 변경
void GroupManager::SendGroupMemberInfo(User* pUser, 
                                       const STRUCT_GROUP_MEMBER_INFO& stGroupMemberInfo, 
                                       eGROUP_STAT stat)
{
    MSG_CW_GROUP_ADD_ACK sendMsg;
    STRUCT_GROUP_MEMBER_PACKET_INFO& GroupMemberInfo = sendMsg.m_MemberInfo;
    const STRUCT_GROUP_MEMBER_PACKET_INFO& MemberPacketInfo = stGroupMemberInfo.GroupMemberInfo;

    GroupMemberInfo.bOnline = MemberPacketInfo.bOnline;
    GroupMemberInfo.byChannel = MemberPacketInfo.byChannel;
    GroupMemberInfo.byClass = MemberPacketInfo.byClass;
    GroupMemberInfo.Level = MemberPacketInfo.Level;
    _tcsncpy(GroupMemberInfo.ptszMemberName, MemberPacketInfo.ptszMemberName, 
        _countof(GroupMemberInfo.ptszMemberName));
    GroupMemberInfo.ptszMemberName[_countof(GroupMemberInfo.ptszMemberName) - 1] = '\0';
    GroupMemberInfo.lastDate = MemberPacketInfo.lastDate;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    sendMsg.charGUID = stGroupMemberInfo.charGUID;
#endif
    sendMsg.m_GroupInfo = stat;

    pUser->SendPacket((BYTE*)&sendMsg, sizeof(sendMsg));
}

//그룹 맴버 자료 구조에서 Guid를 기반으로 정보를 가져온다.
STRUCT_GROUP_MEMBER_INFO* GroupManager::FindGroupMember(User* user, DWORD char_guid)
{
    if (user == NULL) 
    {
        return NULL;
    }

    GROUP_MEMBER_MAP_ITER find_iterator = user->m_GroupMemberMap.find(char_guid);

    if (find_iterator == user->m_GroupMemberMap.end())
    {
        return NULL;
    }

    return find_iterator->second;
}

//그룹 맴버 자료 구조에서 이름을 기반으로 해당 정보를 찾는다.
STRUCT_GROUP_MEMBER_INFO* GroupManager::FindGroupMember(User* user, TCHAR* ptszMemberName)
{
    if (user == NULL)
    {
        return NULL;
    }

    GROUP_MEMBER_MAP_ITER begin_iterator = user->m_GroupMemberMap.begin(),
                          end_iterator = user->m_GroupMemberMap.end();

    if (begin_iterator == end_iterator)
    {
        return NULL;
    }

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const STRUCT_GROUP_MEMBER_INFO& member_info = *(begin_iterator->second);

        if (!_tcsnicmp(member_info.GroupMemberInfo.ptszMemberName,
                       ptszMemberName, MAX_CHARNAME_LENGTH))
        {
            return begin_iterator->second;
        }
    }

    return NULL;
}

//유저가 가지고 있는 멤버 수 리턴
int GroupManager::GetLowMemberCount( User* user )
{ 
    //MAX값을 리턴 결과적으로 요청이 실패하게 된다.
    if (user == NULL) return -1;

    GROUP_MEMBER_MAP_ITER begin_iterator = user->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER end_iterator = user->m_GroupMemberMap.end();

    if (begin_iterator == end_iterator)
    {
        return 0;
    }

    int member_count = 0;
    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        const STRUCT_GROUP_MEMBER_INFO& member_Info = *(begin_iterator->second);

        if (member_Info.groupSTAT == GROUP_LEVEL_LOW)
            ++member_count;
    }

    return member_count;
}

void GroupManager::AddMemberInfo( User* pUser,  const STRUCT_GROUP_MEMBER_INFO& Member )
{    
    if (pUser == NULL) 
        return;

    if (pUser->GetGroupMemberCount() >= MAX_GROUP_MEMBER)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]OverFlow, User = %s, MemberGuid = %u" , 
            pUser->GetCharName().c_str(), Member.charGUID );
        return;
    }

    if (FindGroupMember(pUser, Member.charGUID) == NULL)
    {
        STRUCT_GROUP_MEMBER_INFO* pCurMember = AllocMemberInfo();

        *pCurMember = Member;
        pUser->m_GroupMemberMap.insert(GROUP_MEMBER_MAP_PAIR(pCurMember->charGUID, pCurMember ));
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Insert SameUser, User = %s, MemberGuid = %u" , 
            pUser->GetCharName().c_str(), Member.charGUID); 
    }
}

void GroupManager::SendMemberList(User* user)
{
    if (user == NULL) return;

    int member_count = user->GetGroupMemberCount();
    int low_member_count = GetLowMemberCount(user);

    //최대 맴버수보다 높을수는 없지만 높다고 해서 서비스를 멈출수는 없다.
    //해당 부분파악을 위해 로그를 남기며 많은 그룹원을 가지고 있을 경우
    //그룹 혜택을 받지 못하게 하고 메세지를 남기는 방법으로 해결한다.
    assert(member_count <= MAX_GROUP_MEMBER && member_count >= 0);
    if (member_count > MAX_GROUP_MEMBER || member_count < 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]User[%s]Groupmember count is incorrect = %d",
            user->GetCharName().c_str(), member_count);
    }

    assert(low_member_count <= MAX_GROUP_MEMBER_LOW_MAX && low_member_count >= 0);
    if (low_member_count > MAX_GROUP_MEMBER_LOW_MAX || low_member_count < 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]User[%s]Group Low Member Num Over = %d",
            user->GetCharName().c_str(), low_member_count);
    }

    GROUP_MEMBER_MAP_ITER begin_iterator = user->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER end_iterator = user->m_GroupMemberMap.end();

    //맴버수가 0명이라고 해도 응답 패킷을 클라이언트에게 보낸다.

    MSG_CW_GROUP_GROUPLIST_ANS send_msg;

    for (int index = 0; begin_iterator != end_iterator; ++index, ++begin_iterator)
    {
        const STRUCT_GROUP_MEMBER_INFO& group_Info = *(begin_iterator->second);
        send_msg.sGroupMemberInfo[index] = group_Info;
    }

    send_msg.byGroupMemberNum = member_count;
    user->SendPacket((BYTE*)&send_msg, send_msg.GetSize());

    SUNLOG(eFULL_LOG, "["__FUNCTION__"][%s][Guid:%u] Group List Send[Count:%d]", 
        user->GetCharName().c_str(), user->GetGUID(), member_count);
}

STRUCT_GROUP_MEMBER_INFO* GroupManager::InsertMemberInfo( User* pUser, STRUCT_GROUP_MEMBER_INFO* pMember, eGROUP_STAT eMemberStat )
{
    if(!pUser) return NULL;

    pMember->accumEXP = 0;
    pMember->charGUID = pUser->GetCharGuid();
    pMember->groupSTAT = eMemberStat;
    pMember->GroupMemberInfo.bOnline = TRUE;
    pMember->GroupMemberInfo.byChannel = (BYTE)pUser->GetChannelID();
    pMember->GroupMemberInfo.byClass = pUser->GetCharClass();
    pMember->GroupMemberInfo.Level = pUser->GetCharLevel();

    memset( pMember->GroupMemberInfo.ptszMemberName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
    _tcsncpy( pMember->GroupMemberInfo.ptszMemberName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
    pMember->GroupMemberInfo.ptszMemberName[MAX_CHARNAME_LENGTH] = '\0';

    return pMember;
}

void GroupManager::RemoveMemberInfo( User* pUser, DWORD charGuid )
{
    if (pUser == NULL) 
        return;

    GROUP_MEMBER_MAP_ITER iter;

    iter = pUser->m_GroupMemberMap.find(charGuid);

    if (iter == pUser->m_GroupMemberMap.end())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%s] RemoveNewFriend Error[CharGuid:%u]", 
            pUser->GetCharName().c_str() );

        return;
    }

    pUser->m_GroupMemberMap.erase(iter);
}

//그룹 맴버들에게 로그인한 사실을 알린다. 맴버들의 온/오프라인 데이터도 바꿔준다.
void GroupManager::NotifyLoginState(User* pUser)
{
    GROUP_MEMBER_MAP_ITER beginIT = pUser->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER endIT = pUser->m_GroupMemberMap.end();
    if (beginIT == endIT) return;

    for (int i = 0; beginIT != endIT ; ++beginIT)
    {
        const STRUCT_GROUP_MEMBER_INFO& Info = *(beginIT->second);
        User* sendUser = UserManager::Instance()->FindUserByCharGuid(Info.charGUID);

        if (sendUser == NULL) 
            continue;

        SetMemeberConnectStat(sendUser, pUser->GetCharGuid(), FALSE);
        UpdateGroupInfo(sendUser);

        MSG_CW_GROUP_LOGOUT_BRD Msg;
        _tcsncpy(Msg.ptszMemeberName, pUser->GetCharName().c_str(), _countof(Msg.ptszMemeberName));
        Msg.ptszMemeberName[_countof(Msg.ptszMemeberName) - 1] = '\0';

        sendUser->SendPacket((BYTE*)&Msg, sizeof(Msg));
    }
}

//그룹 맴버들에게 로그아웃 한 사실을 알린다. 맴버들의 온/오프라인 데이터도 바꿔준다.
void GroupManager::NotifyLogOutState(User* pUser)
{
    GROUP_MEMBER_MAP_ITER beginIT = pUser->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER endIT = pUser->m_GroupMemberMap.end();
    if (beginIT == endIT) return;

    for (int i = 0; beginIT != endIT ; ++beginIT)
    {
        const STRUCT_GROUP_MEMBER_INFO& Info = *(beginIT->second);
        User* sendUser = UserManager::Instance()->FindUserByCharGuid(Info.charGUID);

        if (sendUser == NULL) 
            continue;

        SetMemeberConnectStat(sendUser, pUser->GetCharGuid(), TRUE);
        UpdateGroupInfo(sendUser);

        MSG_CW_GROUP_LOGIN_BRD Msg;
        _tcsncpy(Msg.ptszMemeberName, pUser->GetCharName().c_str(), _countof(Msg.ptszMemeberName));
        Msg.ptszMemeberName[_countof(Msg.ptszMemeberName) - 1] = '\0';
        Msg.byChannel = static_cast<BYTE>(pUser->GetChannelID());

        sendUser->SendPacket((BYTE*)&Msg, sizeof(Msg));
    }

}

//상위 맴버에게 지정된 경험치를 보너스로 준다. 
//상위 맴버가 로그아웃 상태(케릭터 선택창 포함)라면 경험치를 저장하게 된다.
void GroupManager::NotifyEXPState(User* user)
{
    if (user == NULL)
    {
        return;
    }

    GROUP_MEMBER_MAP_ITER begin_iterator = user->m_GroupMemberMap.begin(),
                          end_iterator = user->m_GroupMemberMap.end();

    if (begin_iterator == end_iterator) 
    {
        //-- 맴버가 없다.
        return;
    }

    for (; begin_iterator != end_iterator ; ++begin_iterator)
    {
        const STRUCT_GROUP_MEMBER_INFO& high_group_member_Info = *(begin_iterator->second);

        User* high_memeber_user =
            UserManager::Instance()->FindUserByCharGuid(high_group_member_Info.charGUID);

#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
        if ((high_group_member_Info.groupSTAT != GROUP_LEVEL_LOW) || 
            (user->GetCharLevel() <= high_group_member_Info.GroupMemberInfo.Level))
        {
            //-- 상위 멤버이거나 레벨이 높으면
            continue;
        }
#else
        if ((high_group_member_Info.groupSTAT != GROUP_LEVEL_HIGH) || 
            (user->GetCharLevel() >= high_group_member_Info.GroupMemberInfo.Level))
        {
            //-- 상위 멤버이거나 레벨이 높으면
            continue;
        }
#endif

        if((high_memeber_user == NULL) || (high_memeber_user->GetStatus() == eZONETYPE_CHARSELECT))
        {
            //-- 멤버가 없거나 케릭터 선택창 이면
            SendAccumEXP(user->GetGUID(), high_group_member_Info.charGUID, user->GetCurBonusEXP());  
            continue;
        }

        ServerSession* agentserver = 
            ServerSessionManager::Instance()->FindAgentServer(high_memeber_user->GetChannelID());
        if (agentserver == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found agentserver");
            return;
        }

        MSG_AW_STATUS_GROUP_ADDEXP_CMD send_msg;
        {
            send_msg.m_CharGuid = high_group_member_Info.charGUID;
            send_msg.m_Amount = user->m_curBonusEXP;
            _tcsncpy(send_msg.m_szCharName, user->GetCharName().c_str(), 
                     _countof(send_msg.m_szCharName));
            send_msg.m_szCharName[_countof(send_msg.m_szCharName) - 1] = '\0';
        }
        agentserver->Send((BYTE*)&send_msg, sizeof(send_msg));
    }
}

//하위 맴버에게 보너스 금액을 준다.하위 맴버가 오프라인이면 무시한다.
void GroupManager::NotifyMoneyState(User* user)
{
    GROUP_MEMBER_MAP_ITER begin_iterator = user->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER end_iterator = user->m_GroupMemberMap.end();

    if (begin_iterator == end_iterator) return;

    for (; begin_iterator != end_iterator ; ++begin_iterator)
    {
        const STRUCT_GROUP_MEMBER_INFO& low_group_member_Info = *(begin_iterator->second);

        User* low_member_user = \
            UserManager::Instance()->FindUserByCharGuid(low_group_member_Info.charGUID);

        if (low_member_user == NULL) 
        {
            continue;
        }
#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
        if ((low_group_member_Info.groupSTAT != GROUP_LEVEL_HIGH) || 
            (user->GetCharLevel() >= low_member_user->GetCharLevel()))    
        {
            continue;
        }
#else
        if ((low_group_member_Info.groupSTAT != GROUP_LEVEL_LOW) || 
            (user->GetCharLevel() <= low_member_user->GetCharLevel()))    
        {
            continue;
        }
#endif
        ServerSession* agentserver = 
            ServerSessionManager::Instance()->FindAgentServer(low_member_user->GetChannelID());
        if (agentserver != NULL)
        {
            MSG_AW_STATUS_GROUP_ADDMONEY_CMD send_msg;
            send_msg.m_CharGuid = low_group_member_Info.charGUID;
            send_msg.m_Amount   = user->m_curBonusMoney;
            _tcsncpy(send_msg.m_szCharName, user->GetCharName().c_str(), 
                _countof(send_msg.m_szCharName));
            send_msg.m_szCharName[_countof(send_msg.m_szCharName) - 1] = '\0';

            agentserver->Send((BYTE*)&send_msg, sizeof(send_msg));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found AgentServer");
        }
    }

}

//맴버들에게 레벨업 사실을 알린다. 상위 맴버와 레벨이 같아지면 더이상 그룹 효과가 지속될 수 
//없음을 클라이언트 측에서 알려준다.
void GroupManager::NotifyLevelUpState(User* pUser)
{
    GROUP_MEMBER_MAP_ITER beginIT = pUser->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER endIT = pUser->m_GroupMemberMap.end();
    if(beginIT == endIT) return;

    for (int i = 0; beginIT != endIT ; ++beginIT)
    {
        const STRUCT_GROUP_MEMBER_INFO& Info = *(beginIT->second);
        User* sendUser = UserManager::Instance()->FindUserByCharGuid(Info.charGUID);

        if(sendUser == NULL) 
            continue;

        MSG_CW_GROUP_LEVELUP_BRD Msg;
        _tcsncpy(Msg.ptszMemeberName,  pUser->GetCharName().c_str(), _countof(Msg.ptszMemeberName));
        Msg.ptszMemeberName[_countof(Msg.ptszMemeberName) - 1] = '\0';
        Msg.charLevel = (BYTE)pUser->GetCharLevel();

        sendUser->SendPacket((BYTE*)&Msg, sizeof(Msg));
    }

}

void GroupManager::SendAroundMember( User* pUser, eSTAT_ALAM member_stat )
{
    if(pUser == NULL) 
        return;

    int group_num = pUser->GetGroupMemberCount();
    if( group_num <= 0) return;
    
    assert(group_num <= MAX_GROUP_MEMBER);
    if(group_num > MAX_GROUP_MEMBER)
    {
        //매우 특수한 상황이나 해당 로직을 멈출수는 없다. 확인후 후처리
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]%s Member count is Over[count:%d]",
            pUser->GetCharName().c_str(), group_num);
    }


    switch (member_stat)
    {
    case LOG_OUT:
        {
            NotifyLoginState(pUser);
            break;
        }
    case LOG_IN:
        {
            NotifyLogOutState(pUser);
            break;
        }
    case GET_MONEY:
        {
            NotifyMoneyState(pUser);
            break;
        }
    case GET_EXP:
        {
            NotifyEXPState(pUser);
            break;
        }
    case MEMBER_LEVELUP:
        {
            NotifyLevelUpState(pUser);
            break;
        }
    }
}

//유저가 가지고 있는 GroupMemberMap에서 해당 유저의 온라인 오프라인 정보를 세팅한다.
bool GroupManager::SetMemeberConnectStat( User* pUser, DWORD dwCharGuid, BOOL bConnectStat )
{
    if(!pUser) return FALSE;

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = FindGroupMember(pUser, dwCharGuid);

    if(!pMemberInfo) return FALSE;

    pMemberInfo->GroupMemberInfo.bOnline = bConnectStat;

    return TRUE;
}

//필드 입장시 케릭터의 그룹 정보를 세팅한다.
bool GroupManager::SetMemberInfo(User* pUser, const STRUCT_GROUP_MEMBER_INFO& Member)
{
    if (pUser == NULL) 
        return FALSE;

    if (pUser->GetGroupMemberCount() > MAX_GROUP_MEMBER)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]OverFlow, User = %s, MemberGuid = %u" , 
            pUser->GetCharName().c_str(), Member.charGUID); 
    }

    if (FindGroupMember(pUser, Member.charGUID) == NULL)
    {

        STRUCT_GROUP_MEMBER_INFO* pCurMember = AllocMemberInfo();
        *pCurMember = Member;

        pUser->m_GroupMemberMap.insert(GROUP_MEMBER_MAP_PAIR(Member.charGUID, pCurMember));

        return TRUE;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Already Group, User[%s], MemberGuid = %u" , 
            pUser->GetCharName().c_str(), Member.charGUID); 

        return FALSE;
    }
}

STRUCT_GROUP_MEMBER_INFO* GroupManager::AllocMemberInfo()
{
    return (STRUCT_GROUP_MEMBER_INFO*) m_pGroupPool->Alloc();
}

//유저의 그룹 맴버를 초기화 할때( 유저 재접속 같은 경우 )
void GroupManager::RealeseMember( User *pUser )
{
    if(!pUser) return;

    GROUP_MEMBER_MAP_ITER iter;
    
    while( !pUser->m_GroupMemberMap.empty() )
    {
        iter = pUser->m_GroupMemberMap.begin();
        pUser->m_GroupMemberMap.erase(iter);
    }
}

//유저의 현재 그룹 정보를 게임 서버와 동기화 시킨다
void GroupManager::UpdateGroupInfo( User* pUser )
{
    if (pUser == NULL) 
        return;

    pUser->m_eHighMember = GROUP_HAVE_NONE;
    pUser->m_eLowMember = GROUP_HAVE_NONE;

    GROUP_MEMBER_MAP_ITER beginIter = pUser->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER endIter = pUser->m_GroupMemberMap.end();

    //MAP에 저장된 멤버 정보를 기반으로 그룹 정보를 세팅한다.
    //맴버상하위중 한명이라도 온라인이 되어있으면 검사를 마친다.
    while (beginIter != endIter)
    {
        const STRUCT_GROUP_MEMBER_INFO& CurMemberInfo = *(beginIter->second);
        const STRUCT_GROUP_MEMBER_PACKET_INFO& GroupMemberInfo = CurMemberInfo.GroupMemberInfo;

        if (CurMemberInfo.groupSTAT == GROUP_LEVEL_HIGH)
        {
            if (GroupMemberInfo.bOnline == TRUE)
            {
                pUser->m_eHighMember = GROUP_HAVE_ONLINE_HIGH;
            }
            else
            {
                pUser->m_eHighMember = GROUP_HAVE_OFFLINE_HIGH;
            }
        }
        else if (pUser->m_eLowMember != GROUP_HAVE_ONLINE_LOW &&
                 CurMemberInfo.groupSTAT == GROUP_LEVEL_LOW)
        {
            if (GroupMemberInfo.bOnline == TRUE)
            {
                pUser->m_eLowMember = GROUP_HAVE_ONLINE_LOW;
            }
            else
            {
                pUser->m_eLowMember = GROUP_HAVE_OFFLINE_LOW;
            }
        }
        
        //상하위 멤버가 전부 온라인으로세팅 되어있으면 더이상 살펴볼 필요 없다.
        if (pUser->m_eHighMember == GROUP_HAVE_ONLINE_HIGH && 
            pUser->m_eLowMember == GROUP_HAVE_ONLINE_LOW)
            break;

        beginIter++;
    }

    SendGroupInfoSet( pUser, pUser->m_eHighMember, pUser->m_eLowMember );
}

//유저 상위 멤버 정보를 세팅한다( 게임 서버 내용과 동기화 )
void GroupManager::SetGroupHighMember( User* pUser, eGROUP_STAT eHighMember )
{
    if(!pUser) return;

    pUser->m_eHighMember = eHighMember;
}

//유저 하위 멤버 정보를 세팅한다( 게임 서버 내용과 동기화 )
void GroupManager::SetGroupLowMember( User* pUser, eGROUP_STAT eLowMember )
{
    if(!pUser) return;

    pUser->m_eLowMember = eLowMember;
}

//그룹 맴버 정보 동기화를 위한 정보를 게임 서버로 보낸다.
void GroupManager::SendGroupInfoSet(User* pUser, eGROUP_STAT eHighMember, eGROUP_STAT eLowMember)
{
    if(pUser == NULL) 
        return;

    ServerSession* pAgentServer = 
        ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );

    if(pAgentServer == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found AgentServer");
        return;
    }

    ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if(pGameDBProxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found DBProxy");
        return;
    }

    MSG_AW_STATUS_GROUP_SET_MEMBERINFO_CMD agentMsg;

    agentMsg.m_CharGuid = pUser->GetCharGuid();
    agentMsg.m_HighGroupinfo = pUser->m_eHighMember;
    agentMsg.m_LowGroupInfo = pUser->m_eLowMember;

    pAgentServer->Send((BYTE*)&agentMsg, sizeof(agentMsg));

    MSG_DW_GROUP_SET_MEMBERINFO_CMD DBPMsg;

    DBPMsg.m_UserKey = pUser->GetGUID();
    DBPMsg.m_HighGroupinfo = pUser->m_eHighMember;
    DBPMsg.m_LowGroupInfo = pUser->m_eLowMember;

    pGameDBProxy->Send((BYTE*)&DBPMsg, sizeof(DBPMsg));

    return;
}

eGROUP_STAT GroupManager::GetGroupHighMember( User* pUser ) 
{
    if(!pUser) return GROUP_STAT_ERROR;

    return pUser->m_eHighMember; 
}
eGROUP_STAT GroupManager::GetGroupLowMember( User* pUser ) 
{ 
    if(!pUser) return GROUP_STAT_ERROR;

    return pUser->m_eLowMember; 
}

bool GroupManager::SendAccumEXP( DWORD userGuid, CHARGUID highGuid, EXPTYPE accumEXP )
{
    MSG_DW_GROUP_ACCUMEXP_CMD sendMsg;
    {
        sendMsg.m_UserKey = userGuid;
        sendMsg.m_HighCharGUID = highGuid;
        sendMsg.m_AccumEXP = (DWORD)accumEXP;
    }
    ServerSession *game_db_proxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if(game_db_proxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]Not found gamebdproxy");
        return FALSE;
    }
    game_db_proxy->Send((BYTE*)&sendMsg, sizeof(sendMsg));

    return TRUE;
}

//lastDate와 현재 시간차이가 chDate보다 크면 FALSE 리턴
bool GroupManager::IsOverBonusDate( DWORD64 last_date, INT check_date  )
{
    tm stLastDate;
    SYSTEMTIME LASTTIME;
    time_t t_LastTime, t_NowTime, t_AccumTime;
    int date;

    last_date = last_date * 100;//초시간을 맞추기 위해 100을 곱해준다.
    util::YYYYMMDDHHMMSSToSYSTEMTIME(last_date, LASTTIME);//DWORD64를 LASTTIME으로 변경

    stLastDate.tm_year  = LASTTIME.wYear - 1900;
    stLastDate.tm_mon   = LASTTIME.wMonth - 1;
    stLastDate.tm_mday  = LASTTIME.wDay;
    stLastDate.tm_hour  = LASTTIME.wHour;
    stLastDate.tm_min   = LASTTIME.wMinute;
    stLastDate.tm_sec   = LASTTIME.wSecond;

    //util::GetDateTime_YYMMDDHHMMSS() : time()으로 구한 시간을 DWORD64로 변경
    //util::GetDateTime_YYMMDDHHMM()

    t_LastTime = mktime(&stLastDate);

    util::TimeSync::time(&t_NowTime);

    t_AccumTime = t_NowTime - t_LastTime;

    date = (INT)t_AccumTime / (3600*24);

    if (date > check_date)
        return FALSE;

    return TRUE;
}

void GroupManager::SendAccumEXPMemo( User* pUser )
{

    TCHAR toCharName[MAX_CHARNAME_LENGTH + 1];

    _tcsncpy(toCharName, pUser->GetCharName().c_str(), _countof(toCharName));
    toCharName[_countof(toCharName) - 1] = '\0';

    MSG_DW_MEMO_SEND_SYN sendMsg;
    sendMsg.m_dwKey = 0;    // 시스템으로부터의 쪽지
    sendMsg.m_SendGuid = 0; // 시스템으로부터의 쪽지
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    sendMsg.m_typeSystemMemo = NORMAL_MEMO;
    sendMsg.m_recvItemCode = 0;
    sendMsg.m_amountCost = 0;
    sendMsg.m_recvGuid = 0;
    // }
    _tcsncpy(sendMsg.m_ptszRecvName, toCharName, _countof(sendMsg.m_ptszRecvName));
    sendMsg.m_ptszRecvName[_countof(sendMsg.m_ptszRecvName) - 1] = '\0';
    
    _sntprintf(sendMsg.m_ptszMemo, _countof(sendMsg.m_ptszMemo), "%d|%d", 
               SYSMEMO_GROUP_MGR, pUser->GetCurAccumEXP());
    sendMsg.m_ptszMemo[_countof(sendMsg.m_ptszMemo) - 1] = '\0';

    ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (pGameDBProxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]No GameDBProxy");
        return ;
    }

    pGameDBProxy->Send((BYTE*)&sendMsg, sizeof(sendMsg));
}

