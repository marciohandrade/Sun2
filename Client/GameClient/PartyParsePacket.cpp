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
        // ��Ƽ�� ����
    case CG_PARTY_MEMBER_INFO_CMD:
        {
            // ���ο� ���� Ȥ�� ���� ��
            // ����� ��Ƽ ���� �����Ѵ�.
            this->SetInfos((MSG_CG_PARTY_MEMBER_INFO_CMD *)pMsg);

            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // ��Ƽ�� ���� ��ε�
    case CG_PARTY_MEMBER_JOIN_BRD:
        {
            this->AddInfo((MSG_CG_PARTY_MEMBER_JOIN_BRD *)pMsg);

            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->ShowPartyInfoDlgs(true);
        }
        break;



        // ��Ƽ Ż�� ����
    case CG_PARTY_LEAVE_REQUEST_NAK:
        {
            MSG_CG_PARTY_LEAVE_REQUEST_NAK *msg = (MSG_CG_PARTY_LEAVE_REQUEST_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��ƼŻ�� ���� ��ε�(ī��Ʈ ����)
    case CG_PARTY_LEAVE_START_BRD:
        {
            MSG_CG_PARTY_LEAVE_START_BRD *msg = (MSG_CG_PARTY_LEAVE_START_BRD *)pMsg;

            // ��Ƽ�� ����
            this->DelInfo(msg->m_dwObjKey);

            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // ��ƼŻ�� �Ϸ� ��ε�(������ ��Ƽ���� ����)
    case CG_PARTY_LEAVE_COMPLETE_BRD:
        {
            MSG_CG_PARTY_LEAVE_COMPLETE_BRD *msg = (MSG_CG_PARTY_LEAVE_COMPLETE_BRD *)pMsg;

            // ��Ƽ�� ����
            this->DelInfo(msg->m_dwObjKey);

            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->ShowPartyInfoDlgs(true);
        }
        break;



        // ���� �߹� ����
    case CG_PARTY_FORCED_EXPULSION_NAK:
        {
            MSG_CG_PARTY_FORCED_EXPULSION_NAK *msg = (MSG_CG_PARTY_FORCED_EXPULSION_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;




        // ��Ƽ�� ���� ��ε�
    case CG_PARTY_CHANGE_MASTER_BRD:
        {
            MSG_CG_PARTY_CHANGE_MASTER_BRD *msg = (MSG_CG_PARTY_CHANGE_MASTER_BRD *)pMsg;

            this->SetMaster(msg->m_dwObjKey);
        
            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->ShowPartyInfoDlgs(true);
        }
        break;

        // ��Ƽ�� ���� ����
    case CG_PARTY_CHANGE_MASTER_NAK:
        {
            MSG_CG_PARTY_CHANGE_MASTER_NAK *msg = (MSG_CG_PARTY_CHANGE_MASTER_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;


   
        // ��Ƽ �ʴ� ����
    case CG_PARTY_INVITE_REQUEST_SERVER_NAK:
        {
            MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *msg = (MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ �ʴ� ����
    case CG_PARTY_INVITE_RESPONSE_SERVER_SYN:
        {
            MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *msg = (MSG_CG_PARTY_INVITE_RESPONSE_SERVER_SYN *)pMsg;

            this->actAskInviting(msg->m_szMasterName, msg->m_dwMasterUserKey);
        }
        break;

        // ��Ƽ ���� ����
    case CG_PARTY_DESTROY_PARTY_NAK:
        {
            MSG_CG_PARTY_DESTROY_PARTY_NAK *msg = (MSG_CG_PARTY_DESTROY_PARTY_NAK *)pMsg;
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ ����
    case CG_PARTY_DESTROY_PARTY_BRD:
        {
            this->LeaveParty();
        }
        break;
    }
}
