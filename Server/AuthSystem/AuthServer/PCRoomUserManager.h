#ifndef _SERVER_AUTHSYSTEM_AUTHSERVER_PCROOMUSERMANAGER_H
#define _SERVER_AUTHSYSTEM_AUTHSERVER_PCROOMUSERMANAGER_H
class PCRoomUserManager
{
    struct PCROOMINFO
    {
        int multi_user_enable_type;

        PCROOMINFO()
            : multi_user_enable_type(0)
        {
        }
    };

    typedef DWORD       PCROOMGUID;
    typedef STLX_STRING IPADDRESS;
    typedef DWORD       USERGUID;

    typedef STLX_MAP<PCROOMGUID, PCROOMINFO> PCRoomInfoMap;
    typedef STLX_LIST<USERGUID> UserGUIDList;
    typedef STLX_MAP<IPADDRESS, UserGUIDList> ClientUserGUIDMap;
    PCRoomInfoMap pcroom_info_;
    ClientUserGUIDMap clients_info_;

public:
    PCRoomUserManager()
    {

    }

    void AddAccount(AccountInfo& info)
    {
        DWORD user_guid = info.m_userGuid;
        DWORD pcroom_guid = info.m_pcBangId;
        STLX_STRING client_ip(info.m_clientIp);

        ClientUserGUIDMap::iterator it = clients_info_.find(client_ip);
        if (it == clients_info_.end()) 
        {
            UserGUIDList userguid_list;
            userguid_list.push_back(user_guid);
            
            clients_info_.insert( \
                ClientUserGUIDMap::value_type(client_ip, userguid_list));
        }
        else
        {
            UserGUIDList& userguid_list = (it->second);
            userguid_list.push_back(user_guid);
        }
    }

    void RemoveAccount(AccountInfo& info)
    {
        DWORD user_guid = info.m_userGuid;
        STLX_STRING client_ip(info.m_clientIp);

        ClientUserGUIDMap::iterator it = clients_info_.find(client_ip);
        if (it != clients_info_.end()) 
        {
            UserGUIDList& userguid_list = (it->second);
            userguid_list.remove(user_guid);
        }
    }

    DWORD GetFirstUser(STLX_STRING client_ip)
    {
        ClientUserGUIDMap::const_iterator it = clients_info_.find(client_ip);
        if (it != clients_info_.end()) {
            const UserGUIDList& userguid_list = (it->second);
            return (userguid_list.empty() ? 0 : userguid_list.front());
        }

        return 0;
    }

    void SetPCRoomMultiUserType(DWORD pcroom_guid, int pcroom_type)
    {
        PCRoomInfoMap::iterator it = pcroom_info_.find(pcroom_guid);
        if (it == pcroom_info_.end())
        {
            PCROOMINFO pcroom_info;
            pcroom_info.multi_user_enable_type = pcroom_type;
            pcroom_info_.insert(PCRoomInfoMap::value_type(pcroom_guid, pcroom_info));
        }
        else
        {
            (it->second).multi_user_enable_type = pcroom_type;
        }
    }

    int GetPCRoomMultiUserType(DWORD pcroom_guid)
    {
        PCRoomInfoMap::iterator it = pcroom_info_.find(pcroom_guid);
        if (it != pcroom_info_.end())
        {
            return (it->second).multi_user_enable_type;
        }

        return 0;
    }
};
#endif //_SERVER_AUTHSYSTEM_AUTHSERVER_PCROOMUSERMANAGER_H