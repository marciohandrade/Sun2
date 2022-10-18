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
        // �� ���� ��û �� ��Ƽ�� ������ ���ʷ� �޴´�.
    case CG_PARTY_MEMBER_INFO_CMD:
        {
            this->InitMemberInfos((MSG_CG_PARTY_MEMBER_INFO_CMD *)pMsg);
            this->GetUIMan()->ResetUIPartys();
        }
        break;

        // ��Ƽ�� ���� ��ε�
    case CG_PARTY_MEMBER_JOIN_BRD:
        {
            this->AddMemberInfo((MSG_CG_PARTY_MEMBER_JOIN_BRD *)pMsg);
            this->GetUIMan()->ResetUIPartys();
        }
        break;
    
        // ��Ƽ �ʴ� ����
    case CG_PARTY_INVITE_REQUEST_SERVER_NAK:
        {
            MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *msg =
                (MSG_CG_PARTY_INVITE_REQUEST_SERVER_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ �ʴ뿡 ���� ����
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

        // ���� �߹� ����
    case CG_PARTY_FORCED_EXPULSION_NAK:
        {
            MSG_CG_PARTY_FORCED_EXPULSION_NAK *msg =
                (MSG_CG_PARTY_FORCED_EXPULSION_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ�� ���� ��ε�
    case CG_PARTY_CHANGE_MASTER_BRD:
        {
            MSG_CG_PARTY_CHANGE_MASTER_BRD *msg =
                (MSG_CG_PARTY_CHANGE_MASTER_BRD *)pMsg;

            this->Migration(msg->m_dwObjKey);

            // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
            this->GetUIMan()->ResetUIPartys();

            this->PrintInterfaceMessage(eST_PARTY_MIGRATION);

            // ����Ǹ� �����ִ� ������â�� ������.
            this->GetUIMan()->GetUserInterface(UI_BATTLEZONE_RELAY)->ShowInterface(FALSE);
        }
        break;

        // ��Ƽ�� ���� ����
    case CG_PARTY_CHANGE_MASTER_NAK:
        {
            MSG_CG_PARTY_CHANGE_MASTER_NAK *msg =
                (MSG_CG_PARTY_CHANGE_MASTER_NAK *)pMsg;
            
            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ ���� ����
    case CG_PARTY_DESTROY_PARTY_NAK:
        {
            MSG_CG_PARTY_DESTROY_PARTY_NAK *msg =
                (MSG_CG_PARTY_DESTROY_PARTY_NAK *)pMsg;

            this->Reason ((ePARTY_RESULT)msg->m_byErrorCode);
        }
        break;

        // ��Ƽ ����
    case CG_PARTY_DESTROY_PARTY_BRD:
        {
            this->PrintInterfaceMessage(eST_PARTY_DESTROYED);

            //�������� �������� Ư���������� �̵��϶�~~ ��� ��Ŷ�� ������� ���
            this->ComebackToVillage();
        }
        break;

        // ��Ƽ�����κ��� �켱 ���� ��� ���� �Խ�
    case CG_PARTY_SELECT_TARGET_BRD:
        {
            MSG_CG_PARTY_SELECT_TARGET_BRD *msg =
                (MSG_CG_PARTY_SELECT_TARGET_BRD *)pMsg;

            this->NET_RECV_CG_PARTY_SELECT_TARGET_BRD(msg);
        }
        break;

        // �켱 ���� ��� ���� ����
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
