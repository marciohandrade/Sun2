#pragma once


class AuthAgentServerSession;


class ServerData
{
public:
    enum { SERVER_STATE_DISCONNECTED = 0, SERVER_STATE_CONNECTED = 1, };
    ServerData();
    ~ServerData() {}

    inline void SetBaseServerData(SERVER_DATA& BaseServerData) { m_BaseServerData = BaseServerData; }
    inline const SERVER_DATA& GetBaseServerData() const { return m_BaseServerData; }

    inline BYTE GetConnectStatus() const { return m_BaseServerData.m_ConnectStatus; }
    inline VOID SetConnectStatus(BYTE Status) { m_BaseServerData.m_ConnectStatus = Status; }

    inline VOID SetCrowdGrade(BYTE CrowdGrade) { m_BaseServerData.m_CrowdGrade = CrowdGrade; }

    inline const char* GetGameAgentPublicIP() const { return m_szGameAgentPublicIP; }
    inline void SetGameAgentPublicIP(const char* ip);

    inline const char* GetGameAgentPrivateIP() const { return m_szGameAgentPrivateIP; }
    inline void SetGameAgentPrivateIP(const char* ip);

    void IncLoginUserCount();
    void DecLoginUserCount();

    BOOL CheckServerCrowd();

private:
    SERVER_DATA m_BaseServerData;

    char m_szGameAgentPublicIP[MAX_IP_NAME_LENGTH + 1];
    char m_szGameAgentPrivateIP[MAX_IP_NAME_LENGTH + 1];

    __PROPERTY(AGENTID, GameAgentID); // Game Agent GUID (10000 +m_GroupID * 100 + m_ChannelID)
    __PROPERTY(WORD, CrowdBase); // Game Agent Crow Base (reception cnt / 9)
    __PROPERTY(WORD, GameAgentPort);
    __PROPERTY(SHORT, LoginUserCnt);
    __PTR_PROPERTY(AuthAgentServerSession, AuthAgentSession);
};


inline void
ServerData::SetGameAgentPublicIP(const char* ip)
{
    strncpy(m_szGameAgentPublicIP, ip, _countof(m_szGameAgentPublicIP));
    m_szGameAgentPublicIP[_countof(m_szGameAgentPublicIP) - 1] = '\0';
}

inline void
ServerData::SetGameAgentPrivateIP(const char* ip)
{
    strncpy(m_szGameAgentPrivateIP, ip, _countof(m_szGameAgentPrivateIP));
    m_szGameAgentPrivateIP[_countof(m_szGameAgentPrivateIP) - 1] = '\0';
}


