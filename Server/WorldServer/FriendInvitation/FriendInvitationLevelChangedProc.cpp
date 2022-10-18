#include "StdAfx.h"
#include ".\friendinvitationlevelchangedproc.h"

#include "../UserManager.h"
#include "../User.h"
#include "WFriendInvitationController.h"
#include "PacketStruct_DW.h"

FriendInvitationLevelChangedProc::FriendInvitationLevelChangedProc(void)
{
}

FriendInvitationLevelChangedProc::~FriendInvitationLevelChangedProc(void)
{
}

VOID FriendInvitationLevelChangedProc::ChangedEventLevel(User* pUser, LEVELTYPE level)
{
    STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetFirstFriend();
    if(NULL == pFriendInfo) return;

    do 
    {
            SendReWardInfo(pFriendInfo->dwFriendGuid, pFriendInfo->sPacketInfo.ptszFriendName, pUser, level, 
                WFriendInvitationController::Instance()->GetEventMaxLevel() == level ? SYSMEMO_FRIENDINVITATION_INVATE_END : SYSMEMO_FRIENDINVITATION_INVATE_PRESENT);
    } while( pFriendInfo = pUser->GetFriendMap(User::FRIEND_BEINVIATED)->GetNextFriend() );		

    SendReWardInfo(pUser->GetCharGuid(), pUser->GetCharName().c_str(), pUser, level, 
        WFriendInvitationController::Instance()->GetEventMaxLevel() == level ? SYSMEMO_FRIENDINVITATION_BEINVATED_END : SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT);

    if(level == WFriendInvitationController::Instance()->GetEventMaxLevel()) //�̺�Ʈ ���᷹���� �Ǿ���.
    {
        //���׼���:5864 ģ���ʴ� ��ǥ������ �����ߴµ��� �̺�Ʈ�� ������� �ʴ¹��� ����
        EventEnd(pUser);
    }
}

//���׼���:5854 (��籹������)�ʴ��� ĳ���Ͱ� �������� ���� ���¿����� �̹�Ʈ ���� ���޴� ���׼���
VOID FriendInvitationLevelChangedProc::SendReWardInfo(CHARGUID Charguid, const char* Charname, User* pSendUser, LEVELTYPE level, eSYSTEM_MEMO_TYPE Memotype)
{
    MSG_DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD msg;
    msg.m_dwKey = 0;    // �ý������κ����� ����
    msg.m_SendGuid = Charguid;
    msg.m_RevGuid = pSendUser->GetCharGuid();
    msg.m_Level = level;
    _tcsncpy( msg.m_ptszRecvName, Charname, MAX_CHARNAME_LENGTH );
    msg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';	

    _sntprintf(msg.m_ptszMemo, MAX_MEMO_LENGTH, "%d|%s|%d|", Memotype, pSendUser->GetCharName().c_str(), level);

    msg.m_ptszMemo[MAX_MEMO_LENGTH] = '\0';  

    ServerSession *pDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if(pDBProxy)
    {
        pDBProxy->Send((BYTE*)&msg, sizeof(msg));
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "SendReWardInfo Error - No GameDBProxy");
    }
}

VOID FriendInvitationLevelChangedProc::EventEnd(User* pUser)
{
    //���� ������ ģ�� ��õ ���� �ڵ带 �Ϲ�ģ���� �ʱ�ȭ ���� ������.
    if(ListPorc.IsSetFriendsNormalSts(pUser))	//pUser �� ģ����õ ���� ĳ���̾� ?
    {
        //��Ģ �����δ� ��� �۾��� ������ �ؾ� ���� ����Ÿ�� �����ؾ� �ϴµ� 
        //�� ���� ������ �����ϴ� �����ϴ� �ϴ� ������ �ٲ�� �ϴ� �����̴�~
        ServerSession *pDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
        if(pDBProxy)
        {
            MSG_DW_FRIEND_INVITATIONRELATION_FREE_CMD FriendInvitationMsg;	//DBProxy �� ����
            FriendInvitationMsg.m_CharGuid = pUser->GetCharGuid();

            pDBProxy->Send((BYTE*)&FriendInvitationMsg, sizeof(FriendInvitationMsg));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "EventEnd Error - No GameDBProxy");
        }				
    }
}
