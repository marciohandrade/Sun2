#include "StdAfx.h"
#include "ServerDataManager.h"
#include "csvLib.h"
#include "AuthAgentServerSession.h"


ServerDataManager::ServerDataManager(void)
{
}

ServerDataManager::~ServerDataManager(void)
{
}

void
ServerDataManager::Destroy()
{
    {
        GROUPDATA_MAP::iterator it = m_mapGroupData.begin(),
                                end = m_mapGroupData.end();
        for( ; it != end; ++it)
        {
            GROUP_DATA* pGroupData = it->second;
            SAFE_DELETE(pGroupData);
        }
        m_mapGroupData.clear();
    };
    {
        SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                                 end = m_mapServerData.end();
        for( ; it != end; ++it)
        {
            ServerData* pServerData = it->second;
            SAFE_DELETE(pServerData);
        }
        m_mapServerData.clear();
    };
}

VOID
ServerDataManager::Init()
{
#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    m_SpecialModeWorld = 0;
    ZeroMemory(m_SpecialModeEnterableIP, sizeof(m_SpecialModeEnterableIP));
#endif

    LoadGroupData();
    LoadServerData();
    Display();
}

void
ServerDataManager::LoadGroupData()
{
    Csvlib csv;
    csv.LoadFromFile("GroupList.csv");

    if(csv.GetCount() >= MAX_GROUP)
    {
        SUNLOG(eCRITICAL_LOG, "[LoadGroupData] Group Info Overflow %d > %d", csv.GetCount(), MAX_GROUP);
        return;
    }

    for (int i = 0; i < csv.GetCount(); ++i)
    {
        GROUP_DATA* pGroupData = new GROUP_DATA;

        pGroupData->m_GroupID = atoi(csv.Select(i, "GroupID").c_str());
        pGroupData->m_LimitedAge = atoi(csv.Select(i, "ReceptAge").c_str());
        strncpy(pGroupData->m_szGroupName, csv.Select(i, "Name").c_str(), MAX_GROUP_NAME_LEN); pGroupData->m_szGroupName[MAX_GROUP_NAME_LEN]='\0';
        pGroupData->m_ConnectStatus = ServerData::SERVER_STATE_DISCONNECTED;

        if(FindGroupData(pGroupData->m_GroupID))
        {
            SUNLOG(eCRITICAL_LOG, "[LoadGroupData] GroupID[%d] Already exist!",
                   pGroupData->m_GroupID);
            SAFE_DELETE(pGroupData);
        }
        else
        {
            m_mapGroupData.insert(GROUPDATA_MAP::value_type(pGroupData->m_GroupID, pGroupData));
        }
    }

    csv.Clear();
}

void
ServerDataManager::LoadServerData()
{
    Csvlib csv;
    csv.LoadFromFile("ServerList.csv");

    if(csv.GetCount() >= MAX_SVR)
    {
        SUNLOG(eCRITICAL_LOG, "[LoadServerData] Server Info Overflow %d > %d", csv.GetCount(), MAX_SVR);
        return;
    }

    for (int i = 0; i < csv.GetCount(); ++i)
    {
        ServerData* pServerData = new ServerData;

        BYTE groupID = atoi(csv.Select(i, "GroupID").c_str());
        BYTE channelID = atoi(csv.Select(i, "ChannelID").c_str());
        GROUP_DATA* pGroupData = FindGroupData(groupID);
        if(!pGroupData) 
        {
            SUNLOG(eCRITICAL_LOG, "[LoadServerData] GroupID[%u] ChannelID[%u] is not found!",
                   groupID, channelID);
            SAFE_DELETE(pServerData);
            continue;
        }

        SERVER_DATA BaseServerData;
        BaseServerData.m_GroupID = groupID;
        BaseServerData.m_ChannelID = channelID;
        BaseServerData.m_CrowdGrade = 0;
        BaseServerData.m_ConnectStatus = 0;
        strncpy(BaseServerData.m_szChannelName, csv.Select(i, "Name").c_str(), MAX_SERVER_NAME_LEN);
        BaseServerData.m_szChannelName[MAX_SERVER_NAME_LEN]='\0';

        pServerData->SetBaseServerData(BaseServerData);
        pServerData->SetGameAgentID(MakeGameAgentID(groupID, BaseServerData.m_ChannelID));
        pServerData->SetCrowdBase(max(1, atoi(csv.Select(i, "ReceptCnt").c_str()) / 9));
        pServerData->SetGameAgentPort(atoi(csv.Select(i, "GameAgentPort").c_str()));
        pServerData->SetGameAgentPublicIP(csv.Select(i, "PublicIP").c_str());
        pServerData->SetGameAgentPrivateIP(csv.Select(i, "PrivateIP").c_str());
        pServerData->SetAuthAgentSession(NULL);
        pServerData->CheckServerCrowd();

        if(FindServerData(pServerData->GetGameAgentID()))
        {
            SUNLOG(eCRITICAL_LOG, "[LoadServerData] GroupID[%u] ChannelID[%u] Already exist!",
                   groupID, channelID);
            SAFE_DELETE(pServerData);
        }
        else
        {
            m_mapServerData.insert(
                SERVERDATA_MAP::value_type(pServerData->GetGameAgentID(), pServerData));
        }
    }

    csv.Clear();
}

AGENTID
ServerDataManager::MakeGameAgentID(BYTE groupID, BYTE channelID)
{
	AGENTID gameAgentID = 10000 + (groupID - 1)* 100 + (channelID - 1);
	return gameAgentID;
}

GROUP_DATA*
ServerDataManager::FindGroupData(BYTE groupID)
{
	GROUPDATA_MAP::iterator it = m_mapGroupData.find(groupID);
	if(it != m_mapGroupData.end())
		return it->second;
	return NULL;
}	

ServerData*
ServerDataManager::FindServerData(AGENTID gameAgentID)
{
    SERVERDATA_MAP::iterator it = m_mapServerData.find(gameAgentID);
    if(it != m_mapServerData.end())
        return it->second;
    return NULL;
}

ServerData*
ServerDataManager::FindServerData(BYTE groupID, BYTE channelID)
{
    AGENTID gameAgentID = MakeGameAgentID(groupID, channelID);
    return FindServerData(gameAgentID);
}

AuthAgentServerSession*
ServerDataManager::FindAuthAgentServerSession(AGENTID gameAgentID)
{
    ServerData* pServerData = FindServerData(gameAgentID);
    if(!pServerData)
        return NULL;

    return pServerData->GetAuthAgentSession();
}

VOID
ServerDataManager::Display()
{
    SUNLOG(eCRITICAL_LOG,
           "[GAMEAGENTID] [GROUP] [CHANNEL] [PublicIP] [PrivateIP] [ConnectStatus] [MAX] : %d\n",
           m_mapServerData.size());

    SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                             end = m_mapServerData.end();
    for( ; it != end; ++it)
    {
        ServerData* pServerData = it->second;
        GROUP_DATA* pGroupData = FindGroupData(pServerData->GetBaseServerData().m_GroupID);
        if(pGroupData) 
        {
            SUNLOG(eCRITICAL_LOG, "%d   %s   %s   %s	%s   %d \n", 
                   pServerData->GetGameAgentID(),
                   pGroupData->m_szGroupName,
                   pServerData->GetBaseServerData().m_szChannelName, 
                   pServerData->GetGameAgentPublicIP(), pServerData->GetGameAgentPrivateIP(),
                   pServerData->GetConnectStatus());
        }
    }
}

#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
// (NOTE) auth common에 설정하지만, login front만 처리하기로 한다.
void
ServerDataManager::ConfigureSpecialModeWorld(BYTE special_world_id,
    const char special_world_enterable_ip[MAX_IP_NAME_LENGTH + 1])
{
    if(special_world_id == 0) {
        m_SpecialModeWorld = 0;
        ZeroMemory(m_SpecialModeEnterableIP, sizeof(m_SpecialModeEnterableIP));
        return;
    }

    // setup configure
    m_SpecialModeWorld = special_world_id;
    strncpy(m_SpecialModeEnterableIP, special_world_enterable_ip, _countof(m_SpecialModeEnterableIP));

    for(char* it = m_SpecialModeEnterableIP;
        it != &m_SpecialModeEnterableIP[_countof(m_SpecialModeEnterableIP)];
        ++it)
    {
        BOOLEAN is_acceptable = ('0' <= *it && *it <= '9') || ('.' == *it);
        if(is_acceptable == false)
            *it = '\0';
    }
}

BOOLEAN
ServerDataManager::IsEnterableWorld(BYTE enter_world_id,
                                    const char requester_ip[MAX_IP_NAME_LENGTH + 1])
{
    if(m_SpecialModeWorld == 0)
        return true;

    if(enter_world_id == m_SpecialModeWorld) {
        const ULONG tag_if_special_then_check_ip = *(ULONG*)requester_ip;
        const ULONG tag_if_special_then_enterable_ip = *(ULONG*)m_SpecialModeEnterableIP;

        if(tag_if_special_then_check_ip != tag_if_special_then_enterable_ip)
            return false;

        if(strncmp(m_SpecialModeEnterableIP, requester_ip,
                   _countof(m_SpecialModeEnterableIP)) != 0)
        {
            return false;
        }
    }
    return true;
}
#endif

BYTE
ServerDataManager::GetLiveGroupListInfo(GROUP_DATA* OUT pGroupListInfo,
                                        const char requester_ip[MAX_IP_NAME_LENGTH + 1])
{
#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    const ULONG tag_if_special_then_check_ip = *(ULONG*)requester_ip;
    ULONG tag_if_special_then_enterable_ip = 0;
    if(m_SpecialModeWorld != 0)
        tag_if_special_then_enterable_ip = *(ULONG*)m_SpecialModeEnterableIP;
#endif

    int count = 0;
    GROUPDATA_MAP::iterator it = m_mapGroupData.begin(),
                            end = m_mapGroupData.end();
    for( ; it != end; ++it)
    {
        GROUP_DATA* pGroupData = it->second;

        if(pGroupData->m_ConnectStatus == ServerData::SERVER_STATE_DISCONNECTED)
            continue;

    #ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
        if(m_SpecialModeWorld != 0) {
            if(pGroupData->m_GroupID == m_SpecialModeWorld) {
                if(tag_if_special_then_check_ip != tag_if_special_then_enterable_ip)
                    continue;
                if(strncmp(m_SpecialModeEnterableIP, requester_ip,
                           _countof(m_SpecialModeEnterableIP)) != 0)
                {
                    continue;
                }
            }
        }
    #endif

        pGroupListInfo[count] = *pGroupData;
        ++count;
    }

    return count;
}

BYTE
ServerDataManager::GetLiveServerListInfo(SERVER_DATA* OUT pServerListInfo,
                                         const char requester_ip[MAX_IP_NAME_LENGTH + 1])
{
#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    const ULONG tag_if_special_then_check_ip = *(ULONG*)requester_ip;
    ULONG tag_if_special_then_enterable_ip = 0;
    if(m_SpecialModeWorld != 0)
        tag_if_special_then_enterable_ip = *(ULONG*)m_SpecialModeEnterableIP;
#endif

    int count = 0;
    SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                             end = m_mapServerData.end();
    for( ; it != end; ++it)
    {
        ServerData* pServerData = it->second;

        if(pServerData->GetConnectStatus() == ServerData::SERVER_STATE_DISCONNECTED)
            continue;

    #ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
        if(m_SpecialModeWorld != 0) {
            const SERVER_DATA& server_data = pServerData->GetBaseServerData();
            if(server_data.m_GroupID == m_SpecialModeWorld) {
                if(tag_if_special_then_check_ip != tag_if_special_then_enterable_ip)
                    continue;
                if(strncmp(m_SpecialModeEnterableIP, requester_ip,
                           _countof(m_SpecialModeEnterableIP)) != 0)
                {
                    continue;
                }
            }               
        }
    #endif

        pServerListInfo[count] = pServerData->GetBaseServerData();
        ++count;
    }

    return count;
}

// CHANGES: f101030.1L, change the specification to accept multi-agent on same ip address
BYTE
ServerDataManager::GetServerIPListInfo(sGAME_AGENT_INFO* OUT game_agent_ip_info_array)
{
    int count = 0;
    SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                             end = m_mapServerData.end();
    for ( ; it != end; ++it, ++count)
    {
        const ServerData* server_data = it->second;
        sGAME_AGENT_INFO* const game_agent_ip_info = &game_agent_ip_info_array[count];
        //
        game_agent_ip_info->game_agent_id_ = server_data->GetGameAgentID();
        game_agent_ip_info->game_agent_port_ = server_data->GetGameAgentPort();
        //
        strncpy(game_agent_ip_info->game_agent_public_ip_,
                server_data->GetGameAgentPublicIP(),
                _countof(game_agent_ip_info->game_agent_public_ip_));
        game_agent_ip_info->game_agent_public_ip_[\
            _countof(game_agent_ip_info->game_agent_public_ip_) - 1] = '\0';
        //
        strncpy(game_agent_ip_info->game_agent_private_ip_,
                server_data->GetGameAgentPrivateIP(),
                _countof(game_agent_ip_info->game_agent_private_ip_));
        game_agent_ip_info->game_agent_private_ip_[\
            _countof(game_agent_ip_info->game_agent_private_ip_) - 1] = '\0';
        //
    }

    return count;
}

BYTE
ServerDataManager::GetDynamicServerListInfo(DYNAMIC_SERVER_DATA* OUT pDynamicServerListInfo)
{
    int count = 0;
    SERVERDATA_MAP::iterator it = m_mapServerData.begin(),
                             end = m_mapServerData.end();
    for( ; it != end; ++it)
    {
        ServerData* pServerData = it->second;
        DYNAMIC_SERVER_DATA& rServerListInfo = pDynamicServerListInfo[count];
        rServerListInfo.m_GameAgentID = pServerData->GetGameAgentID();
        rServerListInfo.m_ConnectStatus = pServerData->GetBaseServerData().m_ConnectStatus;
        rServerListInfo.m_LoginUserCnt = pServerData->GetLoginUserCnt();
        rServerListInfo.m_CrowdGrade = pServerData->GetBaseServerData().m_CrowdGrade;
        ++count;
    }

    return count;
}

VOID
ServerDataManager::SetDynamicServerListInfo(DYNAMIC_SERVER_DATA* IN pDynamicServerListInfo, BYTE count)
{
    if(count == 0)
        return;

    GROUPDATA_MAP::iterator it = m_mapGroupData.begin(),
                            end = m_mapGroupData.end();
    for( ; it != end; ++it)
    {
        GROUP_DATA* pGroupData = it->second;
        pGroupData->m_ConnectStatus = ServerData::SERVER_STATE_DISCONNECTED;
    }

    for(BYTE i = 0; i < count; ++i)
    {
        DYNAMIC_SERVER_DATA& rServerListInfo = pDynamicServerListInfo[i];
        ServerData* pServerData = FindServerData(rServerListInfo.m_GameAgentID);
        if(pServerData == 0)
        {
            SUNLOG(eCRITICAL_LOG, "[SetDynamicServerListInfo] pServerData[%d] is NULL!",
                   rServerListInfo.m_GameAgentID);
            continue;
        }

        pServerData->SetGameAgentID(rServerListInfo.m_GameAgentID);
        pServerData->SetConnectStatus(rServerListInfo.m_ConnectStatus);
        pServerData->SetLoginUserCnt(rServerListInfo.m_LoginUserCnt);
        pServerData->SetCrowdGrade(rServerListInfo.m_CrowdGrade);

        if(pServerData->GetConnectStatus() == ServerData::SERVER_STATE_CONNECTED)
        {
            GROUP_DATA* pGroupData = FindGroupData(pServerData->GetBaseServerData().m_GroupID);
            if(pGroupData)
                pGroupData->m_ConnectStatus = ServerData::SERVER_STATE_CONNECTED;
        }
    }
}





















