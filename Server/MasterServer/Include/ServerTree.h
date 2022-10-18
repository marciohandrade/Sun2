#pragma once
#ifndef MASTERSERVER_SERVER_TREE_H
#define MASTERSERVER_SERVER_TREE_H
//==================================================================================================

// TypeDefinedServerSession은 이것을 이 파일을 include하기 전에
// typedef (_TYPE_) TypeDefinedServerSession 의 형태로 선언할 것.
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

// 인자로 제공시 해당 구조체 형식으로 Overflow가 발생되지 않는 충분한 크기로 입력시켜야 한다.
// 해당시점에 사용되지 않는 Send용 또는 Recv용 패킷 버퍼를 사용하는 편이 좋을 듯 한뎅...
template<typename _TYPENAME_>
struct sPTR_LIST
{
    DWORD m_dwCount;
    // 서버 정보 제공하는 개수를 보건데... 100개로 설정되어 있으므로 충분할 것이다.
    _TYPENAME_* m_pPTRs[256];
};

typedef sPTR_LIST<sSERVER_NODE> sPTR_SERVER_NODE_ARRAY;
typedef sPTR_LIST<sSERVER_INFO> sPTR_SERVER_INFO_ARRAY;

//==================================================================================================

// Index를 지정하기 미묘하다. 일단 호출하는 쪽에서 감안하도록 한다.
// 즉, Session관리쪽에서 이 역할을 담당한다. 거기서 실패하면 이 루틴을 호출하지 않도록...

// Algorithm  B-tree 방식
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

    // 배열 중간 중간에 NULL값이 있을 수 있다. 계산 방법은 받아들이는 UpperBound까지의 Loop를 도는 방법이다.
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
    // 일단 최대 인식 가능한 서버 개수로 평가한다.
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

// 배열 중간 중간에 NULL값이 있을 수 있다. 계산 방법은 받아들이는 UpperBound까지의 Loop를 도는 방법이다.
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
    // NULL일 경우 어떻게 해결해야 하나? ...
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
    server_tree_->resize(kMaxNumberOfWorlds);  // 월드 개수

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