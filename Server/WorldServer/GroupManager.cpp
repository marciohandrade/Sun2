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
        //-- �ش� ������ �ٸ� ������ �׷� ��û�� �õ��� ����
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Transaction error", recv_msg->ptszMemeberName);
    }
    else if (dest_user->GetAddBlockInfo() == TRUE)
    {
        //-- �׷� �ɹ��� �׷� ��û �ź� ����� Ȱ��ȭ ���״�.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ON_TOGGLE_ADDBLOCK;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Ad request block user", recv_msg->ptszMemeberName);
    }
    else if (dest_user->IsGroupTransaction() == TRUE)
    {
        //-- ���濡�� ������ �̹� ��û���̴�.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already group request", recv_msg->ptszMemeberName);
    }
    else if (!_tcsnicmp(request_user->GetCharName().c_str(), 
        recv_msg->ptszMemeberName, MAX_CHARNAME_LENGTH))
    {
        //-- �߰��Ϸ��� �׷� �ɹ��� �̸��� �ڽ��� �̸��� ��
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_ADD_ME;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not request to myself", recv_msg->ptszMemeberName);
    }
    else if (dest_user->GetCharLevel() >= request_user->GetCharLevel())
    {
        //-- �߰� �Ϸ��� �׷� �ɹ��� ������ ���ų� ���� ��� ����
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
    }
#if defined(_CHINA)
    else if (request_user->GetCharLevel() < HIGH_MEMBER_LEVEL || 
        dest_user->GetCharLevel() < LOW_MEMBER_LEVEL)
    {
        //-- �߱� ��û���� ������ 80�̻� ���� �ɹ�, 10�̻� ���� �ɹ� ����
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
    }
#endif
    else if (GetGroupHighMember(dest_user) == GROUP_HAVE_ONLINE_HIGH ||
        GetGroupHighMember(dest_user) == GROUP_HAVE_OFFLINE_HIGH)
    {
        //-- �߰� �Ϸ��� �ɸ��Ͱ� ���� ����� ������ �ִ°�?
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already had high member", recv_msg->ptszMemeberName);
    }
    else if (FindGroupMember(request_user, recv_msg->ptszMemeberName))
    {
        //-- �ɹ��� �׷쿡 ���� �Ҷ�
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already Member", recv_msg->ptszMemeberName);
    }
    else if (GetLowMemberCount(request_user) >= MAX_GROUP_MEMBER_LOW_MAX)
    {
        //-- �� �׷�� 6�� ���� �ɹ��� 5����� �����ϴ�.
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::GROUP_OVERFLOW;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s OverFlow member count", recv_msg->ptszMemeberName);
    }
    else
    {
        //-- �׷� �ɹ� ��� ����
        return true;
    }

    //-- �׷� �ɹ� ��� ���� �޼��� Send
    request_user->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
    return false;
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO ���� ����
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

//�׷� �ɹ� �ڷ� �������� Guid�� ������� ������ �����´�.
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

//�׷� �ɹ� �ڷ� �������� �̸��� ������� �ش� ������ ã�´�.
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

//������ ������ �ִ� ��� �� ����
int GroupManager::GetLowMemberCount( User* user )
{ 
    //MAX���� ���� ��������� ��û�� �����ϰ� �ȴ�.
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

    //�ִ� �ɹ������� �������� ������ ���ٰ� �ؼ� ���񽺸� ������� ����.
    //�ش� �κ��ľ��� ���� �α׸� ����� ���� �׷���� ������ ���� ���
    //�׷� ������ ���� ���ϰ� �ϰ� �޼����� ����� ������� �ذ��Ѵ�.
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

    //�ɹ����� 0���̶�� �ص� ���� ��Ŷ�� Ŭ���̾�Ʈ���� ������.

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

//�׷� �ɹ��鿡�� �α����� ����� �˸���. �ɹ����� ��/�������� �����͵� �ٲ��ش�.
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

//�׷� �ɹ��鿡�� �α׾ƿ� �� ����� �˸���. �ɹ����� ��/�������� �����͵� �ٲ��ش�.
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

//���� �ɹ����� ������ ����ġ�� ���ʽ��� �ش�. 
//���� �ɹ��� �α׾ƿ� ����(�ɸ��� ����â ����)��� ����ġ�� �����ϰ� �ȴ�.
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
        //-- �ɹ��� ����.
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
            //-- ���� ����̰ų� ������ ������
            continue;
        }
#else
        if ((high_group_member_Info.groupSTAT != GROUP_LEVEL_HIGH) || 
            (user->GetCharLevel() >= high_group_member_Info.GroupMemberInfo.Level))
        {
            //-- ���� ����̰ų� ������ ������
            continue;
        }
#endif

        if((high_memeber_user == NULL) || (high_memeber_user->GetStatus() == eZONETYPE_CHARSELECT))
        {
            //-- ����� ���ų� �ɸ��� ����â �̸�
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

//���� �ɹ����� ���ʽ� �ݾ��� �ش�.���� �ɹ��� ���������̸� �����Ѵ�.
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

//�ɹ��鿡�� ������ ����� �˸���. ���� �ɹ��� ������ �������� ���̻� �׷� ȿ���� ���ӵ� �� 
//������ Ŭ���̾�Ʈ ������ �˷��ش�.
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
        //�ſ� Ư���� ��Ȳ�̳� �ش� ������ ������� ����. Ȯ���� ��ó��
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

//������ ������ �ִ� GroupMemberMap���� �ش� ������ �¶��� �������� ������ �����Ѵ�.
bool GroupManager::SetMemeberConnectStat( User* pUser, DWORD dwCharGuid, BOOL bConnectStat )
{
    if(!pUser) return FALSE;

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = FindGroupMember(pUser, dwCharGuid);

    if(!pMemberInfo) return FALSE;

    pMemberInfo->GroupMemberInfo.bOnline = bConnectStat;

    return TRUE;
}

//�ʵ� ����� �ɸ����� �׷� ������ �����Ѵ�.
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

//������ �׷� �ɹ��� �ʱ�ȭ �Ҷ�( ���� ������ ���� ��� )
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

//������ ���� �׷� ������ ���� ������ ����ȭ ��Ų��
void GroupManager::UpdateGroupInfo( User* pUser )
{
    if (pUser == NULL) 
        return;

    pUser->m_eHighMember = GROUP_HAVE_NONE;
    pUser->m_eLowMember = GROUP_HAVE_NONE;

    GROUP_MEMBER_MAP_ITER beginIter = pUser->m_GroupMemberMap.begin();
    GROUP_MEMBER_MAP_ITER endIter = pUser->m_GroupMemberMap.end();

    //MAP�� ����� ��� ������ ������� �׷� ������ �����Ѵ�.
    //�ɹ��������� �Ѹ��̶� �¶����� �Ǿ������� �˻縦 ��ģ��.
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
        
        //������ ����� ���� �¶������μ��� �Ǿ������� ���̻� ���캼 �ʿ� ����.
        if (pUser->m_eHighMember == GROUP_HAVE_ONLINE_HIGH && 
            pUser->m_eLowMember == GROUP_HAVE_ONLINE_LOW)
            break;

        beginIter++;
    }

    SendGroupInfoSet( pUser, pUser->m_eHighMember, pUser->m_eLowMember );
}

//���� ���� ��� ������ �����Ѵ�( ���� ���� ����� ����ȭ )
void GroupManager::SetGroupHighMember( User* pUser, eGROUP_STAT eHighMember )
{
    if(!pUser) return;

    pUser->m_eHighMember = eHighMember;
}

//���� ���� ��� ������ �����Ѵ�( ���� ���� ����� ����ȭ )
void GroupManager::SetGroupLowMember( User* pUser, eGROUP_STAT eLowMember )
{
    if(!pUser) return;

    pUser->m_eLowMember = eLowMember;
}

//�׷� �ɹ� ���� ����ȭ�� ���� ������ ���� ������ ������.
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

//lastDate�� ���� �ð����̰� chDate���� ũ�� FALSE ����
bool GroupManager::IsOverBonusDate( DWORD64 last_date, INT check_date  )
{
    tm stLastDate;
    SYSTEMTIME LASTTIME;
    time_t t_LastTime, t_NowTime, t_AccumTime;
    int date;

    last_date = last_date * 100;//�ʽð��� ���߱� ���� 100�� �����ش�.
    util::YYYYMMDDHHMMSSToSYSTEMTIME(last_date, LASTTIME);//DWORD64�� LASTTIME���� ����

    stLastDate.tm_year  = LASTTIME.wYear - 1900;
    stLastDate.tm_mon   = LASTTIME.wMonth - 1;
    stLastDate.tm_mday  = LASTTIME.wDay;
    stLastDate.tm_hour  = LASTTIME.wHour;
    stLastDate.tm_min   = LASTTIME.wMinute;
    stLastDate.tm_sec   = LASTTIME.wSecond;

    //util::GetDateTime_YYMMDDHHMMSS() : time()���� ���� �ð��� DWORD64�� ����
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
    sendMsg.m_dwKey = 0;    // �ý������κ����� ����
    sendMsg.m_SendGuid = 0; // �ý������κ����� ����
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

