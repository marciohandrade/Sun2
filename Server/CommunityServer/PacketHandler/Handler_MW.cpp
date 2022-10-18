#include "WorldServerPch.hxx"
#include "./Handler_MW.h"

#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_DW.h>
#include <ProhibitionWordParser.h>

#include <ScriptDataManager.h> // solarauth control

#include "WorldServer.h"
#include "Sessions/ServerSessionInWorld.h"
#include "./PacketHandler.h"
#include "Objects/WorldUserManager.h"
#include "Channel/WorldChannelManager.h"

namespace ns_world { namespace handlers {
;
}}; //end of namespace

const ns_world::PacketHandlerInfo ns_world::handlers::Handler_MW::GetPacketHandlerNodes()
{
#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_EOR
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    { MAKEWORD(_category_, _protocol_), _phase_, &ns_world::handlers::Handler_MW::On##_protocol_ }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const ns_world::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(MW_CONNECTION, MW_SERVER_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(MW_OPERATION, MW_NOTICE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(MW_OPERATION, MW_BATTLEZONEINFO_REQ, PHASE_SERVICE),
        //HANDLER_NODE_INFO(SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_SERVICE),
        //-------------------------------------------------------------------------------------------------
    #ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
        HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_DISCONNECT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_PHASE_SHIFT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_BLOCK_CMD, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_SERVICE),
    #endif //
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    ns_world::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

#undef HANDLER_NODE_INFO
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    void ns_world::handlers::Handler_MW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg_base, const uint16_t msg_size)

#define WAVERIX_WORLD_HANDLER_DEBUGGING() \
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace_mw", \
        __FUNCTION__, server_session, msg_base, msg_size)))

#if SUN_CODE_SAMPLE // to support visual assistx intelli-sense
ns_world::ServerSessionInWorld* server_session;
MSG_BASE* msg_base; uint16_t msg_size;
#endif

//==================================================================================================



//#include "StdAfx.h"
//#include ".\handler_MW.h"
//#include "WorldServer.h"
//#include "MasterServerSession.h"
//#include "UserManager.h"
//#include "ns_world::WorldChannelManager.h"
//#include "ns_world::community::WorldChannel.h"
//#include <PacketStruct_MW.h>
//#include <Protocol_MW.h>
//#include <Protocol_ServerCommon.h>
//#include <PacketStruct_ServerCommon.h>
//#include <PacketStruct_CW.h>
////#include <GMList.h>
//#include <ProhibitionWordParser.h>
//#include "ServerSessionManager.h"
//#include <PacketStruct_DW.h>

// �����Ϳ� ���� �����ϸ� ����Ŷ�� �����޴´�.
HANDLER_NODE_INFO(MW_CONNECTION, MW_SERVER_INFO_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_MW_SERVER_INFO_CMD* recv_msg = static_cast<const MSG_MW_SERVER_INFO_CMD*>(msg_base);
    const SERVER_KEY server_key = recv_msg->ServerKey; // copy
    SUNLOG(eCRITICAL_LOG, "ServerKey Recved from MasterServer(%d) (%d, %d, %d, %d)",
               server_key,
               server_key.GetWorldID(), server_key.GetChannelID(),
               server_key.GetServerType(), server_key.GetServerID());

    if (ns_world::WorldServer* const world_server = WorldServer::Instance())
    {
        world_server->UpdateConnectSessionInfo(*recv_msg);
    };
}

HANDLER_NODE_INFO(MW_OPERATION, MW_NOTICE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    // WARNING: server_session is the master session or agent sessions
    const MSG_MW_NOTICE_CMD* recv_msg = static_cast<const MSG_MW_NOTICE_CMD*>(msg_base);
    if (recv_msg->dwNoticeLength > MAX_NOTICE_LEN)
    {
        SUNLOG(eCRITICAL_LOG,  "Notice Length Over(%d)", recv_msg->dwNoticeLength);
        return;
    }

    SERVER_KEY serverKey    = recv_msg->dwServerUID;
    BYTE byChannelID        = serverKey.GetChannelID();
    BYTE byServerType        = serverKey.GetServerType();
    //BYTE byServerID            = serverKey.GetServerID();
    
    MSG_CW_NOTICE_BRD noticeMsg;
    noticeMsg.m_byCategory    = CW_CHAT;
    noticeMsg.m_byProtocol    = CW_NOTICE_BRD;
    // CHANGES: f101210.2L, check boundary and consider length with null-terminated position
    if (size_t str_len = strlen(recv_msg->szNotice) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        if (str_len >= _countof(recv_msg->szNotice)) {
            str_len = _countof(recv_msg->szNotice);
        }
        noticeMsg.wLen = static_cast<uint16_t>(str_len);
        strncpy(noticeMsg.szMsg, recv_msg->szNotice, _countof(noticeMsg.szMsg));
        noticeMsg.szMsg[_countof(noticeMsg.szMsg) - 1] = '\0';
    };
    //
    noticeMsg.m_byRed = recv_msg->m_byRed;
    noticeMsg.m_byGreen = recv_msg->m_byGreen;
    noticeMsg.m_byBlue = recv_msg->m_byBlue;

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldChannelManager* const channel_manager = ns_world::WorldChannelManager::Instance();
    if (byChannelID == 0)
    {
        // ä�ι�ȣ�� 0�� ��� ���峻 ��� ä�� ���
        if (byServerType == 0)
        {
            user_manager->SendToAll(&noticeMsg, noticeMsg.GetSize());

            char szNotice[MAX_NOTICE_LEN + 1];
            ZeroMemory(szNotice, sizeof(szNotice));
            strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
            SUNLOG(eCRITICAL_LOG, "-----------------------------<< World Notice >>-----------------------------");
            SUNLOG(eCRITICAL_LOG, "%s", szNotice);
            SUNLOG(eCRITICAL_LOG, "------------------------------------------------------------------------");
        }
        else
        {
            // ���� Ÿ���� 0�� �ƴ� ��� ��� ä���� Ư�� ������ ����
            switch (byServerType)
            {
            case FIELD_SERVER:
                {
                    channel_manager->SendToStaticFields(&noticeMsg, noticeMsg.GetSize());

                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
                    SUNLOG(eCRITICAL_LOG, "------------------------<< World/FieldServer notice >>-------------------------");
                    SUNLOG(eCRITICAL_LOG, "%s", szNotice);
                    SUNLOG(eCRITICAL_LOG, "------------------------------------------------------------------------");
                }
    
                break;
            case BATTLE_SERVER:
                {
                    channel_manager->SendToBattleZones(&noticeMsg, noticeMsg.GetSize());
                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
                    SUNLOG(eCRITICAL_LOG,  "------------------------<< World/BattleServer Notice >>-----------------------");
                    SUNLOG(eCRITICAL_LOG,  "%s", szNotice);
                    SUNLOG(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            }
        }
    }
    else
    {
        // ä�ι�ȣ�� 0�� �ƴϹǷ� Ư�� ä�� ���
        ns_world::community::WorldChannel* pChannel = \
            ns_world::WorldChannelManager::Instance()->FindChannel(byChannelID);
        if (!pChannel)
        {
            SUNLOG(eCRITICAL_LOG, "WorldChannel Notice Error - No WorldChannel %u!!", byChannelID);
            return;
        }

        if (byServerType == 0)
        {
            // ����, ��Ʋ�� ��� ����
            pChannel->SendToAll(&noticeMsg, noticeMsg.GetSize());

            char szNotice[MAX_NOTICE_LEN + 1];
            ZeroMemory(szNotice, sizeof(szNotice));
            strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
            SUNLOG(eCRITICAL_LOG, "----------------------------<< %d WorldChannel Notice >>----------------------------", byChannelID);
            SUNLOG(eCRITICAL_LOG, "%s", szNotice);
            SUNLOG(eCRITICAL_LOG, "------------------------------------------------------------------------");
        }
        else
        {
            // ���� Ÿ���� 0�� �ƴ� ��� Ư�� ä���� Ư�� ������ ����
            switch (byServerType)
            {
            case FIELD_SERVER:
                {
                    // ���� ���̵� 0�̸� �ش� ���� ��ü�� ����
                    pChannel->SendToStaticFields(&noticeMsg, noticeMsg.GetSize());

                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
                    SUNLOG(eCRITICAL_LOG,  "---------------------------<< %d FieldServer Notice >>-------------------------", byChannelID);
                    SUNLOG(eCRITICAL_LOG,  "%s", szNotice);
                    SUNLOG(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            case BATTLE_SERVER:
                {
                    pChannel->SendToBattleZones(&noticeMsg, noticeMsg.GetSize());
                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, recv_msg->dwNoticeLength);
                    SUNLOG(eCRITICAL_LOG,  "--------------------------<< %d BattleServer Notice >>------------------------", byChannelID);
                    SUNLOG(eCRITICAL_LOG,  "%s", szNotice);
                    SUNLOG(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            }
        }
    }
}

// ������ ������ ��Ʋ�� ���� ��û
HANDLER_NODE_INFO(MW_OPERATION, MW_BATTLEZONEINFO_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    MSG_MW_BATTLEZONEINFO_ANS result_msg;

    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();
    //
    typedef ns_world::WorldChannelManager::CHANNEL_MAP ChannelTable;
    const ChannelTable& channels = channel_manager->GetChannelMap();

    if (channels.empty())
    {
        result_msg.byCount = 0;
        server_session->SendPacket(&result_msg, result_msg.GetSize());
        return; // CHANGES: f110812.1L, added 'return' for valid control flow
    };

    size_t upperbound = channels.size();
    if (FlowControl::FCAssert(upperbound <= _countof(result_msg.ChannelInfo)) == false) {
        upperbound = _countof(result_msg.ChannelInfo);
    };
    result_msg.byCount = static_cast<uint8_t>(upperbound);
    //
    const uint8_t world_id = ns_world::WorldServer::Instance()->GetServerKey().GetWorldID();
    ChannelTable::const_iterator channel_it = channels.begin();
    const ChannelTable::const_iterator channel_end = channels.end();
    for (int index = 0; channel_it != channel_end; ++index, ++channel_it)
    {
        const ChannelTable::value_type& channel_pair = *channel_it;
        STRUCT_BATTLEZONE_INFO* const dest_it = &result_msg.ChannelInfo[index];
        ZeroMemory(dest_it, sizeof(*dest_it));
        //
        dest_it->byWorldID = world_id;
        dest_it->byChannelID = static_cast<uint8_t>(channel_pair.channel->GetChannelID());
        //
        for (int zone_type_index = eZONETYPE_LOBBY;
             zone_type_index < eZONETYPE_MAX;
             ++zone_type_index)
        {
#if WORLD_CONTENTS_BLOCK
            // TODO: add statistics storage
            //dest_it->dwZoneCount[zone_type_index] = channel->
#endif
        };
    };
    server_session->SendPacket(&result_msg, result_msg.GetSize());
    return;
}

#if SUN_CODE_BACKUP // removes the unused section
HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_SERVICE)
{
    // �������� ����!
    WorldServer::Instance()->Shutdown();

    // �����Ѵٰ� �˷���~
    MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS    sendMsg;

    sendMsg.m_byCategory = SERVERCOMMON;
    sendMsg.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;

    server_session->Send((BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS));
}
#endif //SUN_CODE_BACKUP

//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_DISCONNECT_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    CONSOLELOG(eCRITICAL_LOG, _T("RECEIVED [MW_CONNECTION_DISCONNECT_CMD]"));
};

// MASTER SERVER "SM_CONNECTION_PHASE_SHIFT_CMD" ����� �����ؾ� �Ѵ�.
// ����, SM_CONNECTION_PHASE_SHIFT_CMD�� ������ �� �ϰ������� �����ؾ� �Ѵ�.
HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_PHASE_SHIFT_CMD, PHASE_SERVICE)
{
    const MSG_MW_CONNECTION_PHASE_SHIFT_CMD* recv_msg = \
        static_cast<const MSG_MW_CONNECTION_PHASE_SHIFT_CMD*>(msg_base);

    // BLOCK CMD�� �� �ִ�.
    ePHASE_SHIFT shifted_phase = ePHASE_SHIFT(recv_msg->m_dwPHASE_SHIFT);
    ;            shifted_phase = ePHASE_SHIFT(shifted_phase & PHASE_UNLIMITED);
    //
    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    const ePHASE_SHIFT privious_phase = world_server->GetPhaseStatus();

    switch (shifted_phase)
    {
    case PHASE_UNKNOWN:
        // ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
        break;
    case PHASE_EXCHANGE:
        {
            if (privious_phase == PHASE_EXCHANGE) {
                return;
            };
            world_server->SetPhaseStatus(shifted_phase);

            if (privious_phase == PHASE_SERVICE)
            {
                // �ٽ� EXCHANGE���·� ��ȯ�Ǿ��ٴ� ���� ���� ���� ���� ���� ������ ������...
                // PHASE_SERVICE -> PHASE_EXCHANGE
                // �̰��� PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE�� �����Ѵٴ� �ǹ�

                // �����͸� �ٽ� �޾ƾ� �ϴ��� ����� NOTIFY_SIGNAL�� �ذ��� ����.
                eSCRIPT_DATA_CMD cmd = SCRIPT_DATA_CMD::SET
                                     | SCRIPT_DATA_CMD::RANGE::ONENODE
                                     | SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA;

                ScriptDataManager::DataStatusCommand(cmd, SCRIPT_NOTIFY_SIGNAL);
            }
            else
            {
                //
                // PHASE_UNKNOWN -> PHASE_EXCHANGE
                //
                // ���񽺷��� ��ȯ�̹Ƿ� ���� ���۵��, ���������� SolarAuth���� ���񽺸�
                // ���� �� �ִ�. �Ǵ� �־��� CMD�� ���� ó���� ������ �� �ִ�.
                // �̰����� WorkLoad�����ϸ� �ȴ�.
                // �̰� �̿��� ������ ���� ���� WorkLoad ��������. �ƴϸ� ���ŵǵ��� �����Ѵ�.

                // <THINKING2>
                //   ������ ������� �ʴ� �۾��� ���� �� �ִ�. �̸� ��� �ذ��� ���ΰ�?
                //if (WorkLoadManager::c_NotExistKey ==
                //  server_session->GetWorkLoadContainer().FindWorkKey(scriptCode))

                PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE* work_node =
                    new PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE();

                work_node->SetInformation(server_session,
                    server_session->GetSyncKey(), DWORD(PHASE_EXCHANGE));
                WORK_KEY work_key = WorkLoadManager::RegisterTask(work_node);
                if (WorkLoadManager::c_NotExistKey != work_key)
                {
                    server_session->GetWorkLoadContainer().RegisterWork(
                        work_key, (DWORD)(__int64)work_node);
                    return;
                }
            }
        }
        break;
    case PHASE_SERVICE:
        {
            if (privious_phase == PHASE_UNKNOWN)
            {
                ASSERT(!"�߸��� ���� ���´� Ȯ���� ��");
                return;
            }
            else if (privious_phase == PHASE_SERVICE)
            {
                ASSERT(!"�߸��� ���� ���´� Ȯ���� �� - ���� ������ �� �ƴѰ�?");
                return;
            }
            world_server->SetPhaseStatus(shifted_phase);
        }
        break;
    };

    //printf("RECEIVED {MW_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_MW_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n");
}

HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_BLOCK_CMD, PHASE_SERVICE)
{
    __UNUSED((server_session, msg_base, msg_size));
}


//
//HandlerFromMasterServer_IMPL(MW_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN)

HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_SERVICE)
{
    __UNUSED((server_session, msg_base, msg_size));
}

HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_SERVICE)
{
    __UNUSED((server_session, msg_base, msg_size));
}

HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();

    const MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD* recv_msg = \
        static_cast<const MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD*>(msg_base);

    switch (recv_msg->m_wControlCmd)
    {
    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER:
        {
            DWORD dwDataType = recv_msg->m_FILE_INFO.m_wDataType;
            DWORD dwBufferSize = recv_msg->m_FILE_INFO.m_dwBufferSize;

            // <Work><Sequence>
            //   �ش� ó���� ���� ������ �ε��� ��û�� ���Ŀ� ���� ù��° ���信 ���� �����̴�.
            // <1> �ش� ��ũ��Ʈ �ڵ尡 �ùٸ� �� �˻� -> F : NAK
            // <2> T : �ش� ��ũ��Ʈ �ڵ忡 ���۸� �Ҵ��Ѵ�.
            //     DataStreamCommand (SET blah~
            // <3> ��Ÿ Ÿ�Ӿƿ������ ������å�� ������ ��

            //printf("\n\tHEADER {Data Type=%u, Buffer Size = %u\n", dwDataType, dwBufferSize);

            BYTE* pSTREAM;

            eSCRIPT_DATA_CMD cmdSTREAM = SCRIPT_DATA_CMD::SET
                                       | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;

            ScriptDataManager::DataStreamCommand(cmdSTREAM,
                (eSCRIPT_CODE)dwDataType, pSTREAM, dwBufferSize);

            eSCRIPT_DATA_CMD cmdSTATUS = SCRIPT_DATA_CMD::SET
                                       | SCRIPT_DATA_CMD::RANGE::ONENODE
                                       | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING;
            ScriptDataManager::DataStatusCommand(cmdSTATUS, (eSCRIPT_CODE)dwDataType);
        }
        break;
    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA:
        {
            DWORD dwDataType = recv_msg->m_TOKEN_INFO.m_Header.m_wDataType;
            DWORD dwTokenNo = recv_msg->m_TOKEN_INFO.m_Header.m_wTokenNo;
            DWORD dwBufferSize = recv_msg->m_TOKEN_INFO.m_Header.m_wBufferSize;

            // <Work><Sequence>
            //   �ش� ó���� ���� ������ �ε��� ��û�� ���� ������ ó���ϸ�, ���� �����Ͱ� �ԷµǾ� �´�.
            //   �����ڰ� �ö����� �ش� ��ū�� �ش��ϴ� ��ġ��
            //   (�ش� ��ũ���� �Ҵ�� ���۸� �� �� ��ġ��) �����͸� ������ ����.
            //   DataStreamCommand (SET blah~

            ScriptDataManager::SaveDataStreamToken(
                (eSCRIPT_CODE)dwDataType,
                dwTokenNo,
                dwBufferSize,
                recv_msg->m_TOKEN_INFO.m_pSTREAM);

            //if (SCRIPT_CODE_KEY(FIELD) == dwDataType)
            //{
            //  //printf("\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize);
            //  printf((CHAR*)recv_msg->m_TOKEN_INFO.m_pSTREAM);
            //  printf("\n\nDELIM %u\n\n", recv_msg->m_TOKEN_INFO.m_pSTREAM[dwBufferSize-1]);
            //}
            //printf("\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize);

        }
        break;

    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE:
        // �̰����� �����ϵ��� ��������.
        // �Ϸ�Ǹ�, �ش� ��ũ��Ʈ ������ �Ϸ� �ݹ�(���� ���� ����)�� �����Ѵ�.
        //AsyncReloadData
        {
            DWORD dwDataType = recv_msg->m_FILE_CLOSURE.m_wDataType;
            DWORD dwCRC = recv_msg->m_FILE_CLOSURE.m_dwCRC;

            CONSOLELOG(eCRITICAL_LOG, _T("SCRIPT %u\t RECEIVED"), dwDataType);

            eSCRIPT_DATA_CMD cmdSTATUS = SCRIPT_DATA_CMD::SET
                                       | SCRIPT_DATA_CMD::RANGE::ONENODE
                                       | SCRIPT_DATA_CMD::STATUS::MOVE2LOADED;

            ScriptDataManager::DataStatusCommand(cmdSTATUS, (eSCRIPT_CODE)dwDataType/*, dwCRC*/);
            ScriptDataManager::ProcessLoadedData((eSCRIPT_CODE)dwDataType);
        }
        break;
    };
}

//HandlerFromMasterServer_IMPL(MW_DATA_TRANSFER_SENDDATA_ACK)
//HandlerFromMasterServer_IMPL(MW_DATA_TRANSFER_SENDDATA_NAK)

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------

//{__NA001254_20090109_RTTG_FUNC_ADD
HANDLER_NODE_INFO(MW_OPERATION, MW_MULTIMEMO_SEND_SYN, PHASE_SERVICE)
{
    MSG_MW_MULTIMEMO_SEND_SYN recv_msg[1];
    if (FlowControl::FCAssert(sizeof(recv_msg) >= msg_size) == false) {
        return;
    };
    ZeroMemory(recv_msg, sizeof(recv_msg));
    CopyMemory(recv_msg, msg_base, msg_size);

    recv_msg->m_MMemoInfo.m_szMessage[_countof(recv_msg->m_MMemoInfo.m_szMessage) - 1] = _T('\0');
    //
    struct FailHandler
    {
        FailHandler(const MSG_MW_MULTIMEMO_SEND_SYN& req_msg,
                    ns_world::ServerSessionInWorld* session)
                    : req_msg_(req_msg), session_(session), memo_result_(RC::RC_MEMO_SUCCESS)
        {}
        ~FailHandler()
        {
            MSG_MW_MULTIMEMO_SEND_ACK fail_msg;
            fail_msg.m_dwKey = req_msg_.m_dwKey;
            BOOST_STATIC_ASSERT(_countof(fail_msg.m_arrResultInfoList) ==
                                _countof(req_msg_.m_MMemoInfo.m_arrUserInfoList));
            const int upperbound = min(req_msg_.m_MMemoInfo.m_nCount,
                                       _countof(req_msg_.m_MMemoInfo.m_arrUserInfoList));
            ASSERT(upperbound == req_msg_.m_MMemoInfo.m_nCount);
            for (int index = 0; index < upperbound; ++index)
            {
                const MMEMO_INFOS::USER_INFO& src_it = req_msg_.m_MMemoInfo.m_arrUserInfoList[index];
                MMEMO_ERR_RESULT* dest_it = &fail_msg.m_arrResultInfoList[index];
                dest_it->SetCharName(src_it.m_szRecvCharName);
                dest_it->SetResult(memo_result_);
            };
            fail_msg.m_dwCount = upperbound;
            session_->SendPacket(&fail_msg, sizeof(fail_msg));
        };
        //
        const MSG_MW_MULTIMEMO_SEND_SYN& req_msg_;
        ns_world::ServerSessionInWorld* const session_;
        RC::eMEMO_RESULT memo_result_;
        //
    private:
        FailHandler(const FailHandler& rhs)
            : req_msg_(rhs.req_msg_), session_(rhs.session_), memo_result_(rhs.memo_result_) {}
        void operator=(const FailHandler&) {}
        //
    } fail_handler(*recv_msg, server_session);
    //
    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    ns_world::ServerSessionInWorld* const dbproxy = world_server->GetGameDBProxy();
    if (dbproxy == NULL) {
        fail_handler.memo_result_ = RC::RC_MEMO_NOSERVER;
        return;
    };
    //
    ProhibitionWordParser* const prohibition_word_parser = ProhibitionWordParser::Instance();
    if (prohibition_word_parser->CheckFilter(recv_msg->m_MMemoInfo.m_szMessage) == false) {
        fail_handler.memo_result_ = RC::RC_MEMO_FAILED;
        return;
    };
    //20070131_�޸� Ư������ �Ľ��ϰ� �������� �ٿ��ֱ�
#ifdef _KOREA
    prohibition_word_parser->CheckIncludeSymbol(recv_msg->m_MMemoInfo.m_szMessage);
#endif

    MSG_DW_MULTIMEMO_SEND_SYN msg_syn;
    msg_syn.m_dwKey = recv_msg->m_dwKey;
    msg_syn.m_MMemoInfo = recv_msg->m_MMemoInfo;
    dbproxy->SendPacket(&msg_syn, sizeof(dbproxy));

    return;
    //
}
//}__NA001254_20090109_RTTG_FUNC_ADD
