#include "StdAfx.h"
#include ".\handler_MW.h"
#include "WorldServer.h"
#include "MasterServerSession.h"
#include "UserManager.h"
#include "ChannelManager.h"
#include "Channel.h"
#include <PacketStruct_MW.h>
#include <Protocol_MW.h>
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_CW.h>
//#include <GMList.h>
#include <ProhibitionWordParser.h>
#include "ServerSessionManager.h"
#include <PacketStruct_DW.h>

HandlerFromMasterServer_IMPL(MW_NOTICE_CMD)
{
    MSG_MW_NOTICE_CMD* pRecvMsg = (MSG_MW_NOTICE_CMD*)pMsg;

    if(pRecvMsg->dwNoticeLength > MAX_NOTICE_LEN)
    {
        MessageOut(eCRITICAL_LOG,  "Notice Length Over(%d)", pRecvMsg->dwNoticeLength);
        return;
    }

    SERVER_KEY serverKey    = pRecvMsg->dwServerUID;
    BYTE byChannelID        = serverKey.GetChannelID();
    BYTE byServerType        = serverKey.GetServerType();
    //BYTE byServerID            = serverKey.GetServerID();
    
    MSG_CW_NOTICE_BRD noticeMsg;
    noticeMsg.m_byCategory    = CW_CHAT;
    noticeMsg.m_byProtocol    = CW_NOTICE_BRD;
    // CHANGES: f101210.2L, check boundary and consider length with null-terminated position
    if (size_t str_len = strlen(pRecvMsg->szNotice) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        if (str_len >= _countof(pRecvMsg->szNotice)) {
            str_len = _countof(pRecvMsg->szNotice);
        }
        noticeMsg.wLen = static_cast<uint16_t>(str_len);
        strncpy(noticeMsg.szMsg, pRecvMsg->szNotice, _countof(noticeMsg.szMsg));
        noticeMsg.szMsg[_countof(noticeMsg.szMsg) - 1] = '\0';
    };
    //
    noticeMsg.m_byRed = pRecvMsg->m_byRed;
    noticeMsg.m_byGreen = pRecvMsg->m_byGreen;
    noticeMsg.m_byBlue = pRecvMsg->m_byBlue;
        
    if(byChannelID == 0)
    {
        // ä�ι�ȣ�� 0�� ��� ���峻 ��� ä�� ���
        if(byServerType == 0)
        {
            UserManager::Instance()->SendToAll((BYTE*)&noticeMsg, noticeMsg.GetSize());

            char szNotice[MAX_NOTICE_LEN + 1];
            ZeroMemory(szNotice, sizeof(szNotice));
            strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
            MessageOut(eCRITICAL_LOG,  "-----------------------------<< World Notice >>-----------------------------");
            MessageOut(eCRITICAL_LOG,  "%s", szNotice);
            MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
        }
        else
        {
            // ���� Ÿ���� 0�� �ƴ� ��� ��� ä���� Ư�� ������ ����
            switch(byServerType)
            {
            case FIELD_SERVER:
                {
                    ChannelManager::Instance()->SendToVillages((BYTE*)&noticeMsg, noticeMsg.GetSize());

                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
                    MessageOut(eCRITICAL_LOG,  "------------------------<< World/FieldServer notice >>-------------------------");
                    MessageOut(eCRITICAL_LOG,  "%s", szNotice);
                    MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
    
                break;
            case BATTLE_SERVER:
                {
                    ChannelManager::Instance()->SendToBattleZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
/*
                    ChannelManager::Instance()->SendToLobby((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToMissionZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToHuntingZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToQuestZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToPVPZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToEventZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToGuildZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToSiegeZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
*/
                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
                    MessageOut(eCRITICAL_LOG,  "------------------------<< World/BattleServer Notice >>-----------------------");
                    MessageOut(eCRITICAL_LOG,  "%s", szNotice);
                    MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            }
        }
    }
    else
    {
        // ä�ι�ȣ�� 0�� �ƴϹǷ� Ư�� ä�� ���
        Channel* pChannel = ChannelManager::Instance()->GetChannel(byChannelID);
        if(!pChannel)
        {
            MessageOut(eCRITICAL_LOG, "Channel Notice Error - No Channel %u!!", byChannelID);
            return;
        }

        if(byServerType == 0)
        {
            // ����, ��Ʋ�� ��� ����
            pChannel->SendToAll((BYTE*)&noticeMsg, noticeMsg.GetSize());

            char szNotice[MAX_NOTICE_LEN + 1];
            ZeroMemory(szNotice, sizeof(szNotice));
            strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
            MessageOut(eCRITICAL_LOG,  "----------------------------<< %d Channel Notice >>----------------------------", byChannelID);
            MessageOut(eCRITICAL_LOG,  "%s", szNotice);
            MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
        }
        else
        {
            // ���� Ÿ���� 0�� �ƴ� ��� Ư�� ä���� Ư�� ������ ����
            switch(byServerType)
            {
            case FIELD_SERVER:
                {
                    // ���� ���̵� 0�̸� �ش� ���� ��ü�� ����
                    pChannel->SendToVillages((BYTE*)&noticeMsg, noticeMsg.GetSize());

                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
                    MessageOut(eCRITICAL_LOG,  "---------------------------<< %d Channel/FieldServer Notice >>-------------------------", byChannelID);
                    MessageOut(eCRITICAL_LOG,  "%s", szNotice);
                    MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            case BATTLE_SERVER:
                {
                    pChannel->SendToBattleZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
/*
                    ChannelManager::Instance()->SendToLobby((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToMissionZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToHuntingZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToQuestZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToPVPZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToEventZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToGuildZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
                    ChannelManager::Instance()->SendToSiegeZones((BYTE*)&noticeMsg, noticeMsg.GetSize());
*/

                    char szNotice[MAX_NOTICE_LEN + 1];
                    ZeroMemory(szNotice, sizeof(szNotice));
                    strncpy(szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength);
                    MessageOut(eCRITICAL_LOG,  "--------------------------<< %d Channel/BattleServer Notice >>------------------------", byChannelID);
                    MessageOut(eCRITICAL_LOG,  "%s", szNotice);
                    MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------");
                }
                break;
            }
        }
    }
}

// �����Ϳ� ���� �����ϸ� ����Ŷ�� �����޴´�.
HandlerFromMasterServer_IMPL(MW_SERVER_INFO_CMD)
{
    MSG_MW_SERVER_INFO_CMD* pRecvMsg = (MSG_MW_SERVER_INFO_CMD*)pMsg;

    // ���� ����Ű ����
    WorldServer::Instance()->SetServerKey(pRecvMsg->ServerKey);
    MessageOut(eCRITICAL_LOG,  "ServerKey Recved from MasterServer(%d) (%d, %d, %d, %d)", pRecvMsg->ServerKey,
        pRecvMsg->ServerKey.GetWorldID(), pRecvMsg->ServerKey.GetChannelID(), pRecvMsg->ServerKey.GetServerType(), pRecvMsg->ServerKey.GetServerID());

    WorldServer::Instance()->UpdateServerTitle();

    WorldServer::Instance()->SetGameDBProxyServerAddr(pRecvMsg->m_szGameDBProxyServerIP, pRecvMsg->m_wGameDBProxyServerPort);
    // DBProxy���� ����
    WorldServer::Instance()->ConnectToGameDBProxyServer();

    // ���� ����
    WorldServer::Instance()->StartListen(pRecvMsg->m_szWorldServerIP, pRecvMsg->m_wWorldServerPort, 
                                            pRecvMsg->m_szWorldServerInnerIP, pRecvMsg->m_wWorldServerInnerPort);

}

// ������ ������ ��Ʋ�� ���� ��û
HandlerFromMasterServer_IMPL(MW_BATTLEZONEINFO_REQ)
{
    MSG_MW_BATTLEZONEINFO_ANS    sendMsg;

    CHANNEL_MAP& mapChannel = ChannelManager::Instance()->GetChannelMap();

    if(mapChannel.empty())
    {
        // ä���� �ϳ��� ������ �۽� �Ұ�
        sendMsg.byCount = 0;
        pServerSession->Send((BYTE*)&sendMsg, sendMsg.GetSize());
    }

    sendMsg.byCount = (BYTE)mapChannel.size();

    CHANNEL_MAP::iterator    iter;
    int nChannelNum = 0;
    Channel* pChannel;

    for(iter = mapChannel.begin(); iter != mapChannel.end(); iter++)
    {
        pChannel = (*iter).second;

        sendMsg.ChannelInfo[nChannelNum].byWorldID = (BYTE)WorldServer::Instance()->GetWorldID();
        sendMsg.ChannelInfo[nChannelNum].byChannelID = BYTE(pChannel->GetChannelID());

        for(USHORT i = eZONETYPE_LOBBY; i < eZONETYPE_MAX; i++)
        {
            sendMsg.ChannelInfo[nChannelNum].dwZoneCount[i] = pChannel->GetZoneCount((eZONETYPE)i);
//          sendMsg.ChannelInfo[nChannelNum].dwZoneUserCount[i] = pChannel->GetZoneUserNum((eZONETYPE)i);
//          sendMsg.ChannelInfo[nChannelNum].dwZonePCRoomUserCount[i] = pChannel->GetZonePCRoomUserNum((eZONETYPE)i);
        }

/*
        sendMsg.ChannelInfo[nChannelNum].byChannelID = (BYTE)pChannel->GetChannelID();
        sendMsg.ChannelInfo[nChannelNum].dwVillageUserCount = pChannel->GetZoneUserNum(eZONETYPE_VILLAGE) + pChannel->GetZoneUserNum(eZONETYPE_FIELD);
        sendMsg.ChannelInfo[nChannelNum].dwLobbyZoneCount = pChannel->GetZoneCount(eZONETYPE_LOBBY);
        sendMsg.ChannelInfo[nChannelNum].dwLobbyUserCount = pChannel->GetZoneUserNum(eZONETYPE_LOBBY);
        sendMsg.ChannelInfo[nChannelNum].dwHuntingZoneCount = pChannel->GetZoneCount(eZONETYPE_HUNTING);
        sendMsg.ChannelInfo[nChannelNum].dwHuntingUserCount = pChannel->GetZoneUserNum(eZONETYPE_HUNTING);
        sendMsg.ChannelInfo[nChannelNum].dwMissionZoneCount = pChannel->GetZoneCount(eZONETYPE_MISSION);
        sendMsg.ChannelInfo[nChannelNum].dwMissionUserCount = pChannel->GetZoneUserNum(eZONETYPE_MISSION);
        sendMsg.ChannelInfo[nChannelNum].dwQuestZoneCount = pChannel->GetZoneCount(eZONETYPE_QUEST);
        sendMsg.ChannelInfo[nChannelNum].dwQuestUserCount = pChannel->GetZoneUserNum(eZONETYPE_QUEST);
        sendMsg.ChannelInfo[nChannelNum].dwPVPZoneCount = pChannel->GetZoneCount(eZONETYPE_PVP);
        sendMsg.ChannelInfo[nChannelNum].dwPVPUserCount = pChannel->GetZoneUserNum(eZONETYPE_PVP);
        sendMsg.ChannelInfo[nChannelNum].dwEventZoneCount = pChannel->GetZoneCount(eZONETYPE_EVENT);
        sendMsg.ChannelInfo[nChannelNum].dwEventUserCount = pChannel->GetZoneUserNum(eZONETYPE_EVENT);
        sendMsg.ChannelInfo[nChannelNum].dwGuildZoneCount = pChannel->GetZoneCount(eZONETYPE_GUILD);
        sendMsg.ChannelInfo[nChannelNum].dwGuildUserCount = pChannel->GetZoneUserNum(eZONETYPE_GUILD);
        sendMsg.ChannelInfo[nChannelNum].dwSiegeZoneCount = pChannel->GetZoneCount(eZONETYPE_SIEGE);
        sendMsg.ChannelInfo[nChannelNum].dwSiegeUserCount = pChannel->GetZoneUserNum(eZONETYPE_SIEGE);

        sendMsg.ChannelInfo[nChannelNum].dwVillageUserCount = pChannel->GetZonePCRoomUserNum(eZONETYPE_VILLAGE) + pChannel->GetZonePCRoomUserNum(eZONETYPE_FIELD);
        sendMsg.ChannelInfo[nChannelNum].dwLobbyUserCount = pChannel->GetZoneUserNum(eZONETYPE_LOBBY);
        sendMsg.ChannelInfo[nChannelNum].dwMissionUserCount = pChannel->GetZoneUserNum(eZONETYPE_MISSION);
        sendMsg.ChannelInfo[nChannelNum].dwQuestUserCount = pChannel->GetZoneUserNum(eZONETYPE_QUEST);
        sendMsg.ChannelInfo[nChannelNum].dwPVPUserCount = pChannel->GetZoneUserNum(eZONETYPE_PVP);
        sendMsg.ChannelInfo[nChannelNum].dwEventUserCount = pChannel->GetZoneUserNum(eZONETYPE_EVENT);
*/
        nChannelNum++;
    }

    pServerSession->Send((BYTE*)&sendMsg, sendMsg.GetSize());
}

HandlerFromMasterServer_IMPL(SERVERCOMMON_SERVERSHUTDOWN_REQ)
{
    // �������� ����!
    WorldServer::Instance()->Shutdown();

    // �����Ѵٰ� �˷���~
    MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS    sendMsg;

    sendMsg.m_byCategory = SERVERCOMMON;
    sendMsg.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;

    pServerSession->Send((BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS));
}


//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#include "WorldServer.h"
#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_MW.h>
#include "PacketHandler.h"
#include "MasterServerSession.h"

HandlerFromMasterServer_IMPL(MW_CONNECTION_DISCONNECT_CMD)
{
    printf("RECEIVED [MW_CONNECTION_DISCONNECT_CMD]\n");
}

// MASTER SERVER "SM_CONNECTION_PHASE_SHIFT_CMD" ����� �����ؾ� �Ѵ�.
// ����, SM_CONNECTION_PHASE_SHIFT_CMD�� ������ �� �ϰ������� �����ؾ� �Ѵ�.
HandlerFromMasterServer_IMPL(MW_CONNECTION_PHASE_SHIFT_CMD)
{
    MSG_MW_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_MW_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
    ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

    ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);    // BLOCK CMD�� �� �ִ�.
    WorldServer* pWorldServer = WorldServer::Instance();
    ePHASE_SHIFT prevPhase = pWorldServer->GetPhaseStatus();

    switch(ePhase)
    {
    case PHASE_UNKNOWN:
        // ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
        break;
    case PHASE_EXCHANGE:
        {

            if(PHASE_EXCHANGE == prevPhase)
                return;

            pWorldServer->SetPhaseStatus(ePhase);

            if(PHASE_SERVICE == prevPhase)
            {
                // �ٽ� EXCHANGE���·� ��ȯ�Ǿ��ٴ� ���� ���� ���� ���� ���� ������ ������...
                // PHASE_SERVICE -> PHASE_EXCHANGE
                // �̰��� PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE�� �����Ѵٴ� �ǹ�

                // �����͸� �ٽ� �޾ƾ� �ϴ��� ����� NOTIFY_SIGNAL�� �ذ��� ����.
                eSCRIPT_DATA_CMD cmd =
                    SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA;

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
                //if(WorkLoadManager::c_NotExistKey ==
                //  pServerSession->GetWorkLoadContainer().FindWorkKey(scriptCode))

                PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE* pWorkNode =
                    new PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE();

                pWorkNode->SetInformation(pServerSession, pServerSession->GetSyncKey(), (DWORD)PHASE_EXCHANGE);
                WORK_KEY workKey = WorkLoadManager::RegisterTask(pWorkNode);
                if(WorkLoadManager::c_NotExistKey != workKey)
                {
                    pServerSession->GetWorkLoadContainer().RegisterWork(workKey, (DWORD)(__int64)pWorkNode);
                    return;
                }
            }
        }
        break;
    case PHASE_SERVICE:
        {
            if(PHASE_UNKNOWN == prevPhase)
            {
                ASSERT(!"�߸��� ���� ���´� Ȯ���� ��");
                return;
            }
            else if(PHASE_SERVICE == prevPhase)
            {
                ASSERT(!"�߸��� ���� ���´� Ȯ���� �� - ���� ������ �� �ƴѰ�?");
                return;
            }
            pWorldServer->SetPhaseStatus(ePhase);
        }
        break;
    };

    //printf("RECEIVED {MW_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_MW_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n");
}

HandlerFromMasterServer_IMPL(MW_CONNECTION_BLOCK_CMD)
{
}


//
//HandlerFromMasterServer_IMPL(MW_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN)

HandlerFromMasterServer_IMPL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK)
{

}

HandlerFromMasterServer_IMPL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK)
{
}

HandlerFromMasterServer_IMPL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD)
{
    //printf("RECEIVED {_DATA_TRANSFER_SENDDATA_CMD} ");

    MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD = (MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD*)pMsg;
    switch((DWORD)pMsgCMD->m_wControlCmd)
    {
    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER:
        {
            DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_INFO.m_wDataType;
            DWORD dwBufferSize = (DWORD)pMsgCMD->m_FILE_INFO.m_dwBufferSize;

            // <Work><Sequence>
            //   �ش� ó���� ���� ������ �ε��� ��û�� ���Ŀ� ���� ù��° ���信 ���� �����̴�.
            // <1> �ش� ��ũ��Ʈ �ڵ尡 �ùٸ� �� �˻� -> F : NAK
            // <2> T : �ش� ��ũ��Ʈ �ڵ忡 ���۸� �Ҵ��Ѵ�.
            //     DataStreamCommand (SET blah~
            // <3> ��Ÿ Ÿ�Ӿƿ������ ������å�� ������ ��

            //printf("\n\tHEADER {Data Type=%u, Buffer Size = %u\n", dwDataType, dwBufferSize);

            BYTE* pSTREAM;

            eSCRIPT_DATA_CMD cmdSTREAM =
                SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;

            ScriptDataManager::DataStreamCommand(cmdSTREAM, (eSCRIPT_CODE)dwDataType, pSTREAM, dwBufferSize);

            eSCRIPT_DATA_CMD cmdSTATUS =
                SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING;
            ScriptDataManager::DataStatusCommand(cmdSTATUS, (eSCRIPT_CODE)dwDataType);
        }
        break;

    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA:
        {
            DWORD dwDataType = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wDataType;
            DWORD dwTokenNo = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wTokenNo;
            DWORD dwBufferSize = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wBufferSize;

            // <Work><Sequence>
            //   �ش� ó���� ���� ������ �ε��� ��û�� ���� ������ ó���ϸ�, ���� �����Ͱ� �ԷµǾ� �´�.
            //   �����ڰ� �ö����� �ش� ��ū�� �ش��ϴ� ��ġ��
            //   (�ش� ��ũ���� �Ҵ�� ���۸� �� �� ��ġ��) �����͸� ������ ����.
            //   DataStreamCommand (SET blah~

            ScriptDataManager::SaveDataStreamToken(
                (eSCRIPT_CODE)dwDataType,
                dwTokenNo,
                dwBufferSize,
                pMsgCMD->m_TOKEN_INFO.m_pSTREAM
                );

            //if(SCRIPT_CODE_KEY(FIELD) == dwDataType)
            //{
            //  //printf("\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize);
            //  printf((CHAR*)pMsgCMD->m_TOKEN_INFO.m_pSTREAM);
            //  printf("\n\nDELIM %u\n\n", pMsgCMD->m_TOKEN_INFO.m_pSTREAM[dwBufferSize-1]);
            //}
            //printf("\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize);

        }
        break;

    case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE:
        // �̰����� �����ϵ��� ��������.
        // �Ϸ�Ǹ�, �ش� ��ũ��Ʈ ������ �Ϸ� �ݹ�(���� ���� ����)�� �����Ѵ�.
        //AsyncReloadData
        {
            DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_wDataType;
            DWORD dwCRC = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_dwCRC;

            printf("SCRIPT %u\t RECEIVED\n", dwDataType);

            eSCRIPT_DATA_CMD cmdSTATUS =
                SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADED;

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
HandlerFromMasterServer_IMPL(MW_MULTIMEMO_SEND_SYN)
{
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        MSG_MW_MULTIMEMO_SEND_SYN* pRecvMsg = (MSG_MW_MULTIMEMO_SEND_SYN*)pMsg;

        util::StrArrayForMemo memo_string;
        _tcsncpy(memo_string, pRecvMsg->m_MMemoInfo.m_szMessage, _countof(memo_string));
        memo_string[_countof(memo_string) - 1] = _T('\0');

        IfFailThenBreak(ProhibitionWordParser::Instance()->CheckFilter(memo_string), 0);

        //20070131_�޸� Ư������ �Ľ��ϰ� �������� �ٿ��ֱ�
    #ifdef _KOREA
        ProhibitionWordParser::Instance()->CheckIncludeSymbol(memo_string);
    #endif

        ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
        IfFailThenBreak(pGameDBProxy, RC::RC_MEMO_NOSERVER);

        MSG_DW_MULTIMEMO_SEND_SYN sendMsg;
        sendMsg.m_dwKey        = pRecvMsg->m_dwKey;
        sendMsg.m_MMemoInfo = pRecvMsg->m_MMemoInfo;
        BOOST_STATIC_ASSERT(
            (_countof(memo_string) == _countof(pRecvMsg->m_MMemoInfo.m_szMessage)) &&
            (_countof(memo_string) == _countof(sendMsg.m_MMemoInfo.m_szMessage)));
        CopyMemory(sendMsg.m_MMemoInfo.m_szMessage, memo_string,
            sizeof(sendMsg.m_MMemoInfo.m_szMessage));

        IfFailThenBreak(pGameDBProxy->Send((BYTE*)&sendMsg, sizeof(MSG_DW_MULTIMEMO_SEND_SYN)),
                        RC::RC_MEMO_NOSERVER);

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    const DWORD errCode = flow.GetErrorCode();

    MSG_MW_MULTIMEMO_SEND_SYN* pMsgREQ = (MSG_MW_MULTIMEMO_SEND_SYN*)pMsg;
    MSG_MW_MULTIMEMO_SEND_ACK  AckMsg;
    AckMsg.m_dwKey        = pMsgREQ->m_dwKey;
    AckMsg.m_dwCount    = pMsgREQ->m_MMemoInfo.m_nCount;

    for(DWORD i = 0; i < AckMsg.m_dwCount; ++i)
    {
        AckMsg.m_arrResultInfoList->SetCharName(pMsgREQ->m_MMemoInfo.m_arrUserInfoList[i].m_szRecvCharName);
        AckMsg.m_arrResultInfoList->SetResult(errCode);
    }

    pServerSession->Send((BYTE*)&AckMsg,sizeof(MSG_MW_MULTIMEMO_SEND_ACK));

    assert(SAFE_NUMERIC_TYPECAST(DWORD, errCode, BYTE));
}
//}__NA001254_20090109_RTTG_FUNC_ADD