#include "stdafx.h"
#include "AuthInternal.hxx"

#if defined(_DEBUG_SHOW_THE_PACKET_FLOW_)

#define CATE_PROTO_NODE(category, protocol)   \
    { category, protocol, sizeof(MSG_##protocol), "MSG_"#protocol }

struct CateProtoNode {
    union {
        struct {
            BYTE category, protocol;
        };
        WORD category_and_protocol;
    };
    WORD packet_size;
    const char* name;
};

#include "AuthProtocol_AS.h"
#include "AuthProtocol_BS.h"
#include <SunAuth/AuthProtocol_CF.h>
#include "AuthProtocol_FS.h"
#include <SunAuth/AuthProtocol_GA.h>
#include "AuthProtocol_RS.h"

static CateProtoNode cateProtoNode[] =
{
    // ������ LoginFront���� ���ʷ� Ŭ���̾�Ʈ�� ����
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_READY_CMD),

    // Ŭ���̾�Ʈ�� ���� ��û(����Ȯ��)
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_VERIFY_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_VERIFY_ACK),

    // ID, PW üũ - START!
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_ID_PASSWORD_CHECK_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_ID_PASSWORD_CHECK_SYN),

//#ifdef _SHAPE_AUTH
//    // ID, PW üũ - ���� ����
//    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_SHAPE_SYN),
//    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_SHAPE_SYN),
//    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_SHAPE_ACK),
//    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_SHAPE_ACK),
//#endif
#ifdef _SECURE_AUTH
    // ID, PW üũ - ��Ʈ���� ī�� ����
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_MATRIX_CARD_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_MATRIX_CARD_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_MATRIX_CARD_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_MATRIX_CARD_ACK),
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_OTP_PASS_CMD),
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_OTP_PASS_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_OTP_PASS_ACK),
    //CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_MATRIX_CARD_ACK),
#endif
    // ID, PW üũ - END!
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_ID_PASSWORD_CHECK_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_ID_PASSWORD_CHECK_ACK),

    // ���� ��� ��û
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_SERVER_LIST_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_SERVER_LIST_NTF),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_SERVER_LIST_WORLD_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_SERVER_LIST_CHANNEL_ACK),

    // ���� ���� - START!
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_SERVER_SELECT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_SERVER_SELECT_SYN),

    // ���� ���� - GameAgent ���� �� �̸� �˸�
    CATE_PROTO_NODE(CATEGORY_AUTH, SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK),

    // ���� ���� - END!
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_SERVER_SELECT_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_SERVER_SELECT_ACK),

    // PC LOGIN, LOGOUT CMD From GameAgent
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_PC_LOGIN_CMD),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_PC_LOGIN_CMD),
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_PC_LOGOUT_CMD),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_PC_LOGOUT_CMD),

    // PC LOGOUT ASK To GameAgent
    CATE_PROTO_NODE(CATEGORY_AUTH, SA_AUTH_ASK_PC_LOGOUT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, AG_AUTH_ASK_PC_LOGOUT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_ASK_PC_LOGOUT_ACK),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_ASK_PC_LOGOUT_ACK),

    // GameAgent LOGIN
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_GAMEAGENT_LOGIN_CMD),

    // Billing ������ GameAgnet�� �˸�
    CATE_PROTO_NODE(CATEGORY_AUTH, SA_AUTH_BILLING_ALARM_CMD),
    CATE_PROTO_NODE(CATEGORY_AUTH, AG_AUTH_BILLING_ALARM_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_BILLING_ALARM_ACK),

    // Client Disconnect ���� ���� From LoginFront
    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_CLIENT_DISCONNECT_CMD),

    // Client Disconnect ��� ���� To LoginFront
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_CLIENT_DISCONNECT_CMD),

    // PC Name�� UserCount NTF From GameAgent
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_PC_NAME_NTF),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_PC_NAME_NTF),
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_USER_COUNT_NTF),
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_USER_COUNT_NTF),

    // NTF To LoginFront
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_GAME_VERSION_NTF),
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_CONTROL_IPLIST_NTF),

    // NTF To AuthAgent
    CATE_PROTO_NODE(CATEGORY_AUTH, SA_AUTH_AGENT_IP_LIST_NTF),

    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_REAUTH_PASS_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FS_AUTH_REAUTH_PASS_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, AU_AUTH_SERVER_SELECT_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, AU_AUTH_SERVER_SELECT_ACK),

    // ���İ� ��ġ���� ��û
    CATE_PROTO_NODE(CATEGORY_AUTH, CF_AUTH_REQ_PATCH_VER_SYN),
    CATE_PROTO_NODE(CATEGORY_AUTH, FC_AUTH_REQ_PATCH_VER_ACK),

    { 0, 0, 0, 0 },
};

static CateProtoNode cateProtoFilterNode[] =
{
    CATE_PROTO_NODE(CATEGORY_AUTH, SF_AUTH_SERVER_LIST_NTF),// AuthServer -> LoginFront
    CATE_PROTO_NODE(CATEGORY_AUTH, GA_AUTH_USER_COUNT_NTF), // GameAgent -> AuthAgent
    CATE_PROTO_NODE(CATEGORY_AUTH, AS_AUTH_USER_COUNT_NTF), // AuthAgent -> AuthServer
    { 0, 0, 0, 0 },
};

void
AuthPacketFlowAnalysis::Instance()
{
}

void
AuthPacketFlowAnalysis::ParsePacket(MSG_BASE* msg, WORD size)
{
    const WORD protocol = MAKEWORD(msg->m_byCategory, msg->m_byProtocol);
    CateProtoNode* it = 0;

    it = cateProtoFilterNode;
    while(const WORD src = it->category_and_protocol) {
        if(src == protocol) {
            return;
        }
        ++it;
    }

    it = cateProtoNode;
    while(const WORD src = it->category_and_protocol) {
        if(src != protocol) {
            ++it;
            continue;
        }

        printf("[%-40s][%4u]\n", it->name, it->packet_size);
        return;
    }

    return ParsePacket((MSG_BASE_INTERNAL*)msg, size);
}

void
AuthPacketFlowAnalysis::ParsePacket(MSG_BASE_INTERNAL* msg, WORD size)
{
    const WORD protocol = MAKEWORD(msg->m_byCategory, msg->m_byProtocol);
    CateProtoNode* it = 0;

    it = cateProtoFilterNode;
    while(const WORD src = it->category_and_protocol) {
        if(src == protocol) {
            return;
        }
        ++it;
    }

    it = cateProtoNode;
    while(const WORD src = it->category_and_protocol) {
        if(src != protocol) {
            ++it;
            continue;
        }

        printf("[%-40s][%4u]\n", it->name, it->packet_size);
        return;
    }
    printf("Can't Find Protocol %3u:%3u\n", msg->m_byCategory, msg->m_byProtocol);
}


void
AuthPacketFlowAnalysis::TracingFlow(const char* file, const char* function, const size_t line)
{
    printf("[FlowCapture] [%-40s] - line: %3u (file:%s)\n", function, line, file);
}

#else //!_DEBUG_SHOW_THE_PACKET_FLOW_

#endif //!defined(_DEBUG_SHOW_THE_PACKET_FLOW_)



