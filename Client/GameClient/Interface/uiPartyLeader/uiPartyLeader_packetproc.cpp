//------------------------------------------------------------------------------
//  uiPartyLeader_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyLeader.h"
#include "Protocol_CG.h"
#include "PacketStruct_CG.h"
#include "Hero.h"
#include "uiPartyMember/uiPartyMember.h"

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::NetworkProc( MSG_BASE * pMsg )
{
#ifdef _CK_RENEW_PARTY
#else
    switch(pMsg->m_byProtocol)
    {
        // 방 생성 요청 후 파티원 정보를 최초로 받는다.
    case CG_PARTY_MEMBER_INFO_CMD:
        {
            this->InitMemberInfos((MSG_CG_PARTY_MEMBER_INFO_CMD *)pMsg);
            this->GetUIMan()->ResetUIPartys();
        }
        break;

        // 파티원 조인 브로드
    case CG_PARTY_MEMBER_JOIN_BRD:
        {
            this->AddMemberInfo((MSG_CG_PARTY_MEMBER_JOIN_BRD *)pMsg);
            this->GetUIMan()->ResetUIPartys();
        }
        break;
    
        // 파티 초대 응답
    case CG_PARTY_INVITE_REQUEST_SERVER_NAK:
        {
            MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *msg =
                (MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티 초대에 대한 응답
    case CG_PARTY_INVITE_RESPONSE_SERVER_SYN:
        {
            MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *msg =
                (MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *)pMsg;
#ifdef	UNICODE
			TCHAR	szWideString[INTERFACE_STRING_LENGTH];
			MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, msg->m_szMasterName, MAX_CHARNAME_LENGTH, szWideString, MAX_CHARNAME_LENGTH );
			this->AskInviting(szWideString, msg->m_dwMasterUserKey);
#else
			this->AskInviting(msg->m_szMasterName, msg->m_dwMasterUserKey);
#endif
        }
        break;

    case CG_PARTY_INVITE_REQUEST_SERVER_ACK:
        {
        }
        break;

        // 강제 추방 응답
    case CG_PARTY_FORCED_EXPULSION_NAK:
        {
            MSG_CG_PARTY_FORCED_EXPULSION_NAK *msg =
                (MSG_CG_PARTY_FORCED_EXPULSION_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티장 변경 브로드
    case CG_PARTY_CHANGE_MASTER_BRD:
        {
            MSG_CG_PARTY_CHANGE_MASTER_BRD *msg =
                (MSG_CG_PARTY_CHANGE_MASTER_BRD *)pMsg;

            this->Migration(msg->m_dwObjKey);

            // 변경된 파티원들의 UI를 표시한다.
            this->GetUIMan()->ResetUIPartys();

            this->PrintInterfaceMessage(eST_PARTY_MIGRATION);

            // 변경되면 보고있던 릴레이창은 내린다.
            this->GetUIMan()->GetUserInterface(UI_BATTLEZONE_RELAY)->ShowInterface(FALSE);
        }
        break;

        // 파티장 변경 응답
    case CG_PARTY_CHANGE_MASTER_NAK:
        {
            MSG_CG_PARTY_CHANGE_MASTER_NAK *msg =
                (MSG_CG_PARTY_CHANGE_MASTER_NAK *)pMsg;
            
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티 해제 실패
    case CG_PARTY_DESTROY_PARTY_NAK:
        {
            MSG_CG_PARTY_DESTROY_PARTY_NAK *msg =
                (MSG_CG_PARTY_DESTROY_PARTY_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // 파티 해제
    case CG_PARTY_DESTROY_PARTY_BRD:
        {
            this->PrintInterfaceMessage(eST_PARTY_DESTROYED);

            //서버에서 수동으로 특정지역으로 이동하라~~ 라는 패킷을 보내기로 약속
            this->ComebackToVillage();
        }
        break;

        // 파티장으로부터 우선 공격 대상 지정 왔슴
    case CG_PARTY_SELECT_TARGET_BRD:
        {
            MSG_CG_PARTY_SELECT_TARGET_BRD *msg =
                (MSG_CG_PARTY_SELECT_TARGET_BRD *)pMsg;

            this->NET_RECV_CG_PARTY_SELECT_TARGET_BRD(msg);
        }
        break;

        // 우선 공격 대상 지정 실패
    case CG_PARTY_SELECT_TARGET_NAK:
        {
        }
        break;
    }
#endif
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
