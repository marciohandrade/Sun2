#pragma once
#ifndef MASTERSERVER_SERVER_TREE_H
#define MASTERSERVER_SERVER_TREE_H
//==================================================================================================

// TypeDefinedServerSession�� �̰��� �� ������ include�ϱ� ����
// typedef (_TYPE_) TypeDefinedServerSession �� ���·� ������ ��.
struct sSERVER_BASE_INFO
{
    eSERVER_TYPE m_ServerType;
    SERVER_KEY   m_ServerKey;
};

struct sSERVER_NODE : sSERVER_BASE_INFO
{
    TypeDefinedServerSession* m_pServerSession;
};

struct sSERVER_INFO : sSERVER_BASE_INFO
{
    SERVER_INFO m_ServerInfo;
};

//==================================================================================================

// ���ڷ� ������ �ش� ����ü �������� Overflow�� �߻����� �ʴ� ����� ũ��� �Է½��Ѿ� �Ѵ�.
// �ش������ ������ �ʴ� Send�� �Ǵ� Recv�� ��Ŷ ���۸� ����ϴ� ���� ���� �� �ѵ�...
template<typename _TYPENAME_>
struct sPTR_LIST
{
    DWORD m_dwCount;
    // ���� ���� �����ϴ� ������ ���ǵ�... 100���� �����Ǿ� �����Ƿ� ����� ���̴�.
    _TYPENAME_* m_pPTRs[256];
};

typedef sPTR_LIST<sSERVER_NODE> sPTR_SERVER_NODE_ARRAY;
typedef sPTR_LIST<sSERVER_INFO> sPTR_SERVER_INFO_ARRAY;

//==================================================================================================

// Index�� �����ϱ� �̹��ϴ�. �ϴ� ȣ���ϴ� �ʿ��� �����ϵ��� �Ѵ�.
// ��, Session�����ʿ��� �� ������ ����Ѵ�. �ű⼭ �����ϸ� �� ��ƾ�� ȣ������ �ʵ���...

// Algorithm  B-tree ���
template<typename _TYPENAME_>
class ServerTree
{
    static const DWORD kMaxNumberOfWorlds = 15;
    static const DWORD kMaxNumberOfChannels = 10;
    static const DWORD kMaxNumberOfChannelServers = 8;

    static const size_t kMaxNumberOfNodes = kMaxNumberOfWorlds *
                                            kMaxNumberOfChannels *
                                            kMaxNumberOfChannelServers;
public:
    // <TYPE DEFINITION>
    typedef _TYPENAME_ ServerNodeType;
    typedef std::vector<ServerNodeType*> SERVER_NODE_LIST;
    typedef std::vector<SERVER_NODE_LIST*> CHANNEL_LIST;
    typedef std::vector<CHANNEL_LIST*> WORLD_LIST;
    //
    ServerTree();
    ~ServerTree();

    WORLD_LIST* GetWorldList();
    CHANNEL_LIST* GetChannelList(BYTE world_id);
    SERVER_NODE_LIST* GetServerNodeList(BYTE world_id, BYTE channel_id);

    // �迭 �߰� �߰��� NULL���� ���� �� �ִ�. ��� ����� �޾Ƶ��̴� UpperBound������ Loop�� ���� ����̴�.
    ServerNodeType* GetServerArrayList(DWORD& result_number_of_nodes);

    // <METHODs>
    ServerNodeType* AddServer(SERVER_KEY server_key, ServerNodeType* adding_node);
    void RemoveServer(SERVER_KEY server_key);
    ServerNodeType* FindServer(SERVER_KEY server_key);
    void GetPtrList(DWORD session_bitmask, sPTR_LIST<ServerNodeType>* OUT result_of_server_list);

private:
    void Initialize();
    void Expand_WorldListSize(DWORD increase_value = 1);
    void Expand_ChannelListSize(CHANNEL_LIST*& channel_list, DWORD increase_value = 1);
    void _Init_WorldList(WORLD_LIST* world_list);
    void _Init_ChannelList(CHANNEL_LIST* channel_list);
    void _Del_WorldList(WORLD_LIST*& world_list);
    void _Del_ChannelList(CHANNEL_LIST*& channel_list);
    void Release();
    ServerNodeType* _FindEmptyNode();
    //
private:
    DWORD number_of_total_;
    DWORD last_offset_;
    WORLD_LIST* server_tree_;
    // �ϴ� �ִ� �ν� ������ ���� ������ ���Ѵ�.
    ServerNodeType* server_node_array_; // [kMaxBufferSize];

    static BYTE empty_node_static[sizeof(ServerNodeType)];
};

//==================================================================================================

template<typename _TYPENAME_>
BYTE ServerTree<_TYPENAME_>::empty_node_static[sizeof(ServerNodeType)] = { 0, };

template<typename _TYPENAME_>
ServerTree<_TYPENAME_>::ServerTree()
{
    Initialize();
}

template<typename _TYPENAME_>
ServerTree<_TYPENAME_>::~ServerTree()
{
    Release();
}

template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::WORLD_LIST*
ServerTree<_TYPENAME_>::GetWorldList()
{
    return server_tree_;
}

template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::CHANNEL_LIST*
ServerTree<_TYPENAME_>::GetChannelList(BYTE world_id)
{
    if (server_tree_->size() > world_id) {
        return (*server_tree_)[world_id];
    }
    return NULL;
}

template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::SERVER_NODE_LIST*
ServerTree<_TYPENAME_>::GetServerNodeList(BYTE world_id, BYTE channel_id)
{
    if (server_tree_->size() <= world_id) {
        return NULL;
    };
    CHANNEL_LIST* channel_list = (*server_tree_)[world_id];
    if (channel_list == NULL || channel_list->size() <= channel_id) {
        return NULL;
    };
    SERVER_NODE_LIST* node_list = (*channel_list)[channel_id];
    return node_list;
}

// �迭 �߰� �߰��� NULL���� ���� �� �ִ�. ��� ����� �޾Ƶ��̴� UpperBound������ Loop�� ���� ����̴�.
template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::ServerNodeType*
ServerTree<_TYPENAME_>::GetServerArrayList(DWORD& result_number_of_nodes)
{
    result_number_of_nodes = last_offset_ + 1;
    return server_node_array_;
}

// <METHODs>
template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::ServerNodeType*
ServerTree<_TYPENAME_>::AddServer(SERVER_KEY server_key, ServerNodeType* adding_node)
{
    const DWORD world_id = server_key.GetWorldID();
    const DWORD channel_id = server_key.GetChannelID();

    ServerNodeType* const find_node = _FindEmptyNode();
    // NULL�� ��� ��� �ذ��ؾ� �ϳ�? ...
    if (FlowControl::FCAssert(find_node) == false) {
        return NULL;
    };

    *find_node = *adding_node;

    // WARNING: check 'index out of range'
    if (server_tree_->size() <= world_id) {
        server_tree_->resize(world_id + 1);
    };
    CHANNEL_LIST*& channel_list = (*server_tree_)[world_id];
    if (channel_list == NULL) {
        channel_list = new CHANNEL_LIST;
    };
    if (channel_list->size() <= channel_id) {
        channel_list->resize(channel_id + 1);
    }
    SERVER_NODE_LIST*& node_list = (*channel_list)[channel_id];
    if (node_list == NULL) {
        node_list = new SERVER_NODE_LIST;
    };
    node_list->push_back(find_node);
    return find_node;
}

template<typename _TYPENAME_>
void ServerTree<_TYPENAME_>::RemoveServer(SERVER_KEY server_key)
{
    const DWORD world_id = server_key.GetWorldID();
    const DWORD channel_id = server_key.GetChannelID();
    const eSERVER_TYPE server_type = static_cast<eSERVER_TYPE>(server_key.GetServerType());
    __TOUCHED(server_type);
    // WARNING: check 'index out of range'
    if (FlowControl::FCAssert(server_tree_->size() > world_id) == false) {
        return;
    };
    CHANNEL_LIST* channel_list = (*server_tree_)[world_id];
    if (FlowControl::FCAssert(channel_list && channel_list->size() > channel_id) == false) {
        return;
    };
    SERVER_NODE_LIST* node_list = (*channel_list)[channel_id];
    if (FlowControl::FCAssert(node_list) == false) {
        return;
    }
    SERVER_NODE_LIST::iterator it = node_list->begin();
    SERVER_NODE_LIST::const_iterator end = node_list->end();
    for ( ; it != end; ++it)
    {
        ServerNodeType* node = *it;
        if (server_key == node->m_ServerKey)
        {
            *node = *reinterpret_cast<ServerNodeType*>(empty_node_static);
            node_list->erase(it);
            return;
        }
    }
}

template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::ServerNodeType*
ServerTree<_TYPENAME_>::FindServer(SERVER_KEY server_key)
{
    const DWORD world_id = server_key.GetWorldID();
    const DWORD channel_id = server_key.GetChannelID();

    // WARNING: check 'index out of range'
    if (server_tree_->size() <= world_id) {
        return NULL;
    };
    CHANNEL_LIST* channel_list = (*server_tree_)[world_id];
    if (channel_list == NULL || channel_list->size() <= channel_id) {
        return NULL;
    };
    SERVER_NODE_LIST* node_list = (*channel_list)[channel_id];
    if (node_list == NULL) {
        return NULL;
    };
    SERVER_NODE_LIST::const_iterator it = node_list->begin(),
                                     end = node_list->end();
    for ( ; it != end; ++it)
    {
        ServerNodeType* node = *it;
        if (server_key == node->m_ServerKey) {
            return node;
        }
    }
    return NULL;
}

template<typename _TYPENAME_>
void ServerTree<_TYPENAME_>::GetPtrList(DWORD session_bitmask,
                                        sPTR_LIST<ServerNodeType>* OUT result_of_server_list)
{
    ServerNodeType* node_it = server_node_array_;
    ServerNodeType** dest_node_it = result_of_server_list->m_pPTRs;
    DWORD count = 0;
    const DWORD upperbound = last_offset_ + 1;

    for (DWORD i = 0; i < upperbound; ++i, ++node_it)
    {
        const DWORD server_type = static_cast<DWORD>(node_it->m_ServerType);

        if (session_bitmask & (1 << server_type))
        {
            *dest_node_it = node_it;
            ++count;
            ++dest_node_it;
        }
    }

    result_of_server_list->m_dwCount = count;
}

template<typename _TYPENAME_>
void ServerTree<_TYPENAME_>::Initialize()
{
    number_of_total_ = 0;
    last_offset_ = 0;

    server_tree_ = new WORLD_LIST;
    server_tree_->resize(kMaxNumberOfWorlds);  // ���� ����

    server_node_array_ = new ServerNodeType[kMaxNumberOfNodes];
    ZeroMemory(server_node_array_, sizeof(ServerNodeType) * kMaxNumberOfNodes);
}

template<typename _TYPENAME_>
void ServerTree<_TYPENAME_>::Release()
{
    number_of_total_ = 0;
    last_offset_ = 0;

    WORLD_LIST::iterator world_it = server_tree_->begin(),
                         world_end = server_tree_->end();
    for ( ; world_it != world_end; ++world_it)
    {
        CHANNEL_LIST* channel_list = *world_it;
        if (channel_list == NULL) {
            continue;
        }

        CHANNEL_LIST::iterator channel_it = channel_list->begin(),
                               channel_end = channel_list->end();
        for ( ; channel_it != channel_end; ++channel_it)
        {
            SERVER_NODE_LIST* node_list = *channel_it;
            SAFE_DELETE(node_list);
        }
        SAFE_DELETE(channel_list);
    }
    SAFE_DELETE(server_tree_);
    SAFE_DELETE(server_node_array_);
}

template<typename _TYPENAME_>
typename ServerTree<_TYPENAME_>::ServerNodeType*
ServerTree<_TYPENAME_>::_FindEmptyNode()
{
    ServerNodeType* it = server_node_array_;
    const ServerNodeType* const end = server_node_array_ + kMaxNumberOfNodes;
    for ( ; it != end; ++it)
    {
        if (it->m_ServerType != UNKNOWN_SERVER) {
            continue;
        }
        if (memcmp(it, empty_node_static, sizeof(*it)) != 0) {
            continue;
        }
        const DWORD find_index = static_cast<DWORD>(it - server_node_array_);
        if (last_offset_ < find_index) {
            last_offset_ = find_index;
        }
        return it;
    }
    return NULL;
}

#endif //MASTERSERVER_SERVER_TREE_H