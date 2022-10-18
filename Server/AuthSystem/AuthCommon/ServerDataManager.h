#pragma once

#include <map>
#include "ServerData.h"
#include <Singleton.h>

using namespace util;

class AuthAgentServerSession;


class ServerDataManager : public util::Singleton<ServerDataManager>
{
public:
    ServerDataManager();
    ~ServerDataManager();

    VOID Init();
    VOID Destroy();
    VOID Display();

    GROUP_DATA* FindGroupData(BYTE GroupID);
    ServerData* FindServerData(AGENTID GameAgentID);
    ServerData* FindServerData(BYTE GroupID, BYTE ChannelID);
    AuthAgentServerSession* FindAuthAgentServerSession(AGENTID GameAgentID);

    // (CHANGES) (ADDIN) add interface to support special mode control
    BYTE GetLiveGroupListInfo(GROUP_DATA* OUT pGroupListInfo,
                              const char requester_ip[MAX_IP_NAME_LENGTH + 1]);
    BYTE GetLiveServerListInfo(SERVER_DATA* OUT pServerListInfo,
                               const char requester_ip[MAX_IP_NAME_LENGTH + 1]);
    BYTE GetServerIPListInfo(sGAME_AGENT_INFO* OUT pGameAgentIPInfo);

    BYTE GetDynamicServerListInfo(DYNAMIC_SERVER_DATA* OUT pDynamicServerListInfo);
    VOID SetDynamicServerListInfo(DYNAMIC_SERVER_DATA* IN pDynamicServerListInfo, BYTE Count);

    template<class Operator>
    void Foreach_ServerData(Operator& op);

#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    // (NOTE) auth common�� ����������, login front�� ó���ϱ�� �Ѵ�.
    void ConfigureSpecialModeWorld(BYTE special_world_id,
                                   const char special_world_enterable_ip[MAX_IP_NAME_LENGTH + 1]);
    BOOLEAN IsEnterableWorld(BYTE enter_world_id, const char requester_ip[MAX_IP_NAME_LENGTH + 1]);
#endif

private:
    VOID LoadGroupData();
    VOID LoadServerData();
    AGENTID MakeGameAgentID(BYTE GroupID, BYTE ChannelID);

private:
    typedef STLX_MAP<DWORD, GROUP_DATA*> GROUPDATA_MAP;
    typedef STLX_MAP<DWORD, ServerData*> SERVERDATA_MAP;

    GROUPDATA_MAP  m_mapGroupData;
    SERVERDATA_MAP m_mapServerData;
#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    // MODE  WOPS  ����  ����
    //  1    �系  �系  �系
    //  2    ��ü  ��ü  ��ü
    //  3    ��ü  ��ü  �系 (�߰��Ǵ� ���)
    BYTE m_SpecialModeWorld; //0:default mode, !0:��忡 ������� SpeicalMode����Ǵ� world ��ȣ
    char m_SpecialModeEnterableIP[MAX_IP_NAME_LENGTH + 1]; // !0�� ��, ������ world�� ���� ��� IP
#endif
};


template<class Operator>
void ServerDataManager::Foreach_ServerData(Operator& op)
{
    SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                             end = m_mapServerData.end();
    for(; it != end; ++it)
    {
        ServerData* serverData = it->second;
        op(serverData);
    }
}









