#include "stdafx.h"
#include "./FriendInvitationRecallproc.h"

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
#include "SolarDateTime.h"
#include "DominationInfoParser.h"

FriendInvitationRecallProc::FriendInvitationRecallProc(void)
{
}

FriendInvitationRecallProc::~FriendInvitationRecallProc(void)
{
}

VOID FriendInvitationRecallProc::SendRecallTimeSelect(CHARGUID charGUID, Player *pPlayer)
{
    MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN msg;

    msg.CharGUID = charGUID;    

    if(!pPlayer->SendToGameDBPServer(&msg, sizeof(msg)))
    {
        SUNLOG( eCRITICAL_LOG, "Send RecallTimeSelect fail (%u)", msg.CharGUID);
    }

}

VOID FriendInvitationRecallProc::SendRecallFailMsg(Player* pPlayer, BYTE ErrCode)
{
    MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK Msg;
    Msg.m_byErrorCode = ErrCode;
    pPlayer->SendPacket(&Msg, sizeof(Msg));
}

BOOL FriendInvitationRecallProc::IsRecalltimeExpired(Player* pPlayer, DWORD FriendGUID)
{    
    DWORD64 RecallTime = pPlayer->GetInvitateFriend().GetRecallTime(FriendGUID);
    DWORD64 CurrentTiem;
    util::GetDateTime_YYYYMMDDHHMMSS(CurrentTiem);

    if(RecallTime + 10000 > CurrentTiem)
    {
        MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK Nmsg;
        Nmsg.m_byErrorCode = MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::RECALLTIME_SHORTAGE;
        Nmsg.m_LeftTime = CurrentTiem - RecallTime;
        pPlayer->SendPacket(&Nmsg, sizeof(Nmsg));
        return FALSE;
    }
    return TRUE;
}

VOID FriendInvitationRecallProc::SendRecallQuestionMsg(Player* pPlayer, Player* pRecallPlayer)
{
    MSG_CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_SYN msg;

    msg.FriendGUID = pPlayer->GetCharGuid();
    _tcsncpy( msg.FriendName, pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    msg.FriendName[MAX_CHARNAME_LENGTH] = '\0';    

    if(!pRecallPlayer->SendPacket(&msg, sizeof(msg)))
    {
        SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::SENDFAIL);
    }
}

BOOL FriendInvitationRecallProc::IsPossibileReCallLocation(Player* pPlayer, Player* pRecallPlayer)
{
    GameField *pRecallField = pRecallPlayer->GetField();

    if(!pRecallField)
    {
        SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERZONE_FRIEND);
        return FALSE;
    }

    GameField *pField = pPlayer->GetField();

    if(!pField)
    {
        SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERZONE_ME); 
        return FALSE;
    }
    else
    {    
        //버그수정:5810 문상현 2010-02-04 모든국가공통 피의성전으로 소환되는 버그 수정
        //지역점령전 맵에서는 소환을 사용할수 없다.
        if(0 != DominationInfoParser::Instance()->GetDominationLinkMapCode(pField->GetFieldCode()))
        {
            SendRecallFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERZONE_ME); 
            return FALSE;
        }   
    }
    return TRUE;
}

BOOL FriendInvitationRecallProc::SendRecallPosition(Player* pPlayer, Player* pRecallPlayer)
{
    if(pRecallPlayer->ReCallPlayer(pRecallPlayer->GetCharName(), 0, pPlayer->GetField()->GetFieldCode(),
                                   pPlayer->GetRandomPosInAround(3), ePORTAL_TYPE_FIRENDINVITATED_EVENT,
                                   INVALID_POSTYPE_VALUE, 0))
    {
        return TRUE;
    }
    return FALSE;
    /*
    MSG_AG_GM_GO_CMD AGMsg; //리콜유저에게 리콜할 위치 정보 전송    
    _tcsncpy( AGMsg.m_szCharName, pRecallPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
    AGMsg.m_RoomKey = 0;    //무조건 필드
    AGMsg.m_FieldCode = pPlayer->GetField()->GetFieldCode();
    pRecallPlayer->GetPos( &AGMsg.m_vCurPos );
    AGMsg.m_vDestPos = pPlayer->GetRandomPosInAround( 3 );
    AGMsg.m_PortalType = ePORTAL_TYPE_FIRENDINVITATED_EVENT;
    AGMsg.m_atItemPos = INVALID_POSTYPE_VALUE;//사용안됨
    AGMsg.m_atItemCode = 0;//사용안됨
    pRecallPlayer->SendPacket( &AGMsg, sizeof(AGMsg) );
    */
}

VOID FriendInvitationRecallProc::SendRecallTimeUpdate(Player* pPlayer, Player* pRecallPlayer)
{
    MSG_DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD Msg; //리콜한 시간 DB 에 작성
    Msg.CharGUID = pPlayer->GetCharGuid();
    Msg.FriendGUID = pRecallPlayer->GetCharGuid();
    util::GetDateTime_YYYYMMDDHHMMSS(Msg.RecallTime);
    pPlayer->GetInvitateFriend().UpdateReCallTime(pRecallPlayer->GetCharGuid(), Msg.RecallTime);
    pPlayer->SendToGameDBPServer(&Msg, sizeof(Msg));
}

VOID FriendInvitationRecallProc::ReCallUser(Player* pPlayer, Player* pRecallPlayer)
{   
    if(!IsPossibileReCallLocation(pPlayer, pRecallPlayer)) return;

    if(SendRecallPosition(pPlayer, pRecallPlayer))
    {
        SendRecallTimeUpdate(pPlayer, pRecallPlayer);
        SendRecallSucceedMsg(pPlayer);
    }    
}

VOID FriendInvitationRecallProc::SendRecallSucceedMsg(Player* pPlayer)
{
    MSG_CG_EVENT_INVITATEFRIEND_RECALL_ACK ACMsg; //리콜한 유저에게 성공 메시지
    pPlayer->SendPacket(&ACMsg, sizeof(ACMsg));
}

BOOL FriendInvitationRecallProc::IsRequestRecall(Player* pPlayer, DWORD FriendGUID)
{
    DWORD64 CurTime;
    util::GetDateTime_YYYYMMDDHHMMSS(CurTime);

    DWORD64 PreRecall_RequestTime = pPlayer->GetInvitatedFriendRecallRequestTime(FriendGUID);


    if(PreRecall_RequestTime + (FRIENDINVITATION_RECALL_MSGBOX_MAXTIME/1000) > CurTime)
    {
        return TRUE;
    }

    pPlayer->SetInvitatedFriendRecallRequestTime(FriendGUID, CurTime);

    return FALSE;
}

VOID FriendInvitationRecallProc::ReCallACK(const Player* Caller, const CHARGUID CalleeGUID)
{
    const Player * Callee = PlayerManager::Instance()->FindPlayerByCharKey( CalleeGUID );
    ASSERT( Callee );
    if(!Callee)
    {
        MessageOut( eCRITICAL_LOG, "AG_EVENT_RECALL_ACK Not Found User GUID [%u]", CalleeGUID );
    }
    else
    {
        //로그를 남긴다.
        GAMELOG->LogAction(ACT_FRIENDINVITATION_RECALL, const_cast<Player*>(Caller), 
                                                       const_cast<Player*>(Callee));
    }
    
    
}

VOID FriendInvitationRecallProc::ReCallNAK(Player* Caller, const BYTE ErrorCode)
{
    SendRecallFailMsg(Caller, MSG_CG_EVENT_INVITATEFRIEND_RECALL_NAK::OTHERZONE_FRIEND);
}

