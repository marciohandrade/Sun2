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
    // (NOTE) auth common에 설정하지만, login front만 처리하기로 한다.
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
    // MODE  WOPS  국내  유럽
    //  1    사내  사내  사내
    //  2    전체  전체  전체
    //  3    전체  전체  사내 (추가되는 모드)
    BYTE m_SpecialModeWorld; //0:default mode, !0:모드에 상관없이 SpeicalMode적용되는 world 번호
    char m_SpecialModeEnterableIP[MAX_IP_NAME_LENGTH + 1]; // !0일 때, 지정된 world에 접속 허용 IP
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









