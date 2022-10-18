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

    if(level == WFriendInvitationController::Instance()->GetEventMaxLevel()) //이벤트 종료레벨이 되었다.
    {
        //버그수정:5864 친구초대 목표레벨에 도달했는데도 이벤트가 종료되지 않는버그 수정
        EventEnd(pUser);
    }
}

//버그수정:5854 (모든국가공통)초대한 캐릭터가 접속하지 않은 상태에서는 이번트 선물 못받는 버그수정
VOID FriendInvitationLevelChangedProc::SendReWardInfo(CHARGUID Charguid, const char* Charname, User* pSendUser, LEVELTYPE level, eSYSTEM_MEMO_TYPE Memotype)
{
    MSG_DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD msg;
    msg.m_dwKey = 0;    // 시스템으로부터의 쪽지
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
    //도달 했으면 친구 추천 관계 코드를 일반친구로 초기화 시켜 버린당.
    if(ListPorc.IsSetFriendsNormalSts(pUser))	//pUser 이 친구추천 받은 캐릭이야 ?
    {
        //원칙 적으로는 디비 작업이 성공을 해야 서버 데이타를 변경해야 하는데 
        //이 경우는 서버가 실패하던 성공하던 일단 무조건 바꿔야 하는 상태이다~
        ServerSession *pDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
        if(pDBProxy)
        {
            MSG_DW_FRIEND_INVITATIONRELATION_FREE_CMD FriendInvitationMsg;	//DBProxy 에 전송
            FriendInvitationMsg.m_CharGuid = pUser->GetCharGuid();

            pDBProxy->Send((BYTE*)&FriendInvitationMsg, sizeof(FriendInvitationMsg));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "EventEnd Error - No GameDBProxy");
        }				
    }
}
