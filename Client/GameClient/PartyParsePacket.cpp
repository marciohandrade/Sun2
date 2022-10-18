//------------------------------------------------------------------------------
//  PartyParsePacket.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Party.h"

//------------------------------------------------------------------------------
/**
*/
void
Party::Parse(MSG_BASE * pMsg)
{
    switch(pMsg->m_byProtocol)
    {
        // 파티원 정보
    case CG_PARTY_MEMBER_INFO_CMD:
        {
            // 새로운 유저 혹은 방장 등
            // 변경된 파티 구성 갱신한다.
            this->SetInfos((MSG_CG_PARTY_MEMBER_INFO_CMD *)pMsg);

            // 변경된 파티원들의 UI를 표시한다.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // 파티원 조인 브로드
    case CG_PARTY_MEMBER_JOIN_BRD:
        {
            this->AddInfo((MSG_CG_PARTY_MEMBER_JOIN_BRD *)pMsg);

            // 변경된 파티원들의 UI를 표시한다.
            this->ShowPartyInfoDlgs(true);
        }
        break;



        // 파티 탈퇴 실패
    case CG_PARTY_LEAVE_REQUEST_NAK:
        {
            MSG_CG_PARTY_LEAVE_REQUEST_NAK *msg = (MSG_CG_PARTY_LEAVE_REQUEST_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티탈퇴 시작 브로드(카운트 시작)
    case CG_PARTY_LEAVE_START_BRD:
        {
            MSG_CG_PARTY_LEAVE_START_BRD *msg = (MSG_CG_PARTY_LEAVE_START_BRD *)pMsg;

            // 파티원 삭제
            this->DelInfo(msg->m_dwObjKey);

            // 변경된 파티원들의 UI를 표시한다.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // 파티탈퇴 완료 브로드(실제로 파티에서 빠짐)
    case CG_PARTY_LEAVE_COMPLETE_BRD:
        {
            MSG_CG_PARTY_LEAVE_COMPLETE_BRD *msg = (MSG_CG_PARTY_LEAVE_COMPLETE_BRD *)pMsg;

            // 파티워 삭제
            this->DelInfo(msg->m_dwObjKey);

            // 변경된 파티원들의 UI를 표시한다.
            this->ShowPartyInfoDlgs(true);
        }
        break;



        // 강제 추방 응답
    case CG_PARTY_FORCED_EXPULSION_NAK:
        {
            MSG_CG_PARTY_FORCED_EXPULSION_NAK *msg = (MSG_CG_PARTY_FORCED_EXPULSION_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;




        // 파티장 변경 브로드
    case CG_PARTY_CHANGE_MASTER_BRD:
        {
            MSG_CG_PARTY_CHANGE_MASTER_BRD *msg = (MSG_CG_PARTY_CHANGE_MASTER_BRD *)pMsg;

            this->SetMaster(msg->m_dwObjKey);
        
            // 변경된 파티원들의 UI를 표시한다.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // 파티장 변경 응답
    case CG_PARTY_CHANGE_MASTER_NAK:
        {
            MSG_CG_PARTY_CHANGE_MASTER_NAK *msg = (MSG_CG_PARTY_CHANGE_MASTER_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;


   
        // 파티 초대 응답
    case CG_PARTY_INVITE_REQUEST_SERVER_NAK:
        {
            MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *msg = (MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티 초대 묻기
    case CG_PARTY_INVITE_RESPONSE_SERVER_SYN:
        {
            MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *msg = (MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *)pMsg;

            this->actAskInviting(msg->m_szMasterName, msg->m_dwMasterUserKey);
        }
        break;

        // 파티 해제 실패
    case CG_PARTY_DESTROY_PARTY_NAK:
        {
            MSG_CG_PARTY_DESTROY_PARTY_NAK *msg = (MSG_CG_PARTY_DESTROY_PARTY_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티 해제
    case CG_PARTY_DESTROY_PARTY_BRD:
        {
            this->LeaveParty();
        }
        break;
    }
}
