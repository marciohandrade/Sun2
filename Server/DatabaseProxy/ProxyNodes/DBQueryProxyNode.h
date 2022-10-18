#pragma once
#ifndef DBPROXY_DBQUERY_PROXY_NODE_H
#define DBPROXY_DBQUERY_PROXY_NODE_H

#include <PacketStruct_AD.h>
#include <PacketStruct_DG.h>

struct MSG_AD_CHARINFO_CHARLISTREQ_SYN;
struct MSG_DG_CHARINFO_QUERY_SYN;
class DBUser;
class DBCharacter;
class ChildServerSession;

namespace nsQuery {

class DBQueryProxyManager;

}; //end of namespace

//==================================================================================================
// @history
//  |10.04.15|waverix|created, 
//                   |to support reliable user data db synchronization with life-time management

namespace nsQuery {
;
class DBQueryProxyManager;


class DBQueryProxyNode
{
public:
    class UserNode;
    class CharacterNode;

    enum eType {
        eType_None = 0,
        eType_User,
        eType_Character
    };

    DBQueryProxyNode();

    eType type() const;
    //bool loaded() const;
    int state() const;
    int transaction() const;

    void Init();
    
protected:
    struct Shared {
        int8_t node_type; // eType
        // NOTE: if you want to use this field, do declare the method under a derived class
        bool loaded;
        int state;
        int transaction;
        int request;
    } shared_;

private:
    friend class DBQueryProxyManager;
};

//--------------------------------------------------------------------------------------------------

typedef DBQueryProxyNode::UserNode      DBQueryProxyUserNode;
typedef DBQueryProxyNode::CharacterNode DBQueryProxyCharNode;

//--------------------------------------------------------------------------------------------------

}; //end of namespace 'nsQuery'

//==================================================================================================
// implementation

namespace nsQuery {
;

inline DBQueryProxyNode::DBQueryProxyNode()
{
    ZeroMemory(&shared_, sizeof(shared_));
}

inline void DBQueryProxyNode::Init()
{
    ZeroMemory(&shared_, sizeof(shared_));
}

inline DBQueryProxyNode::eType DBQueryProxyNode::type() const
{
    return static_cast<eType>(shared_.node_type);
}

//inline bool DBQueryProxyNode::loaded() const
//{
//    return shared_.loaded;
//}

inline int DBQueryProxyNode::state() const
{
    return shared_.state;
}

inline int DBQueryProxyNode::transaction() const
{
    return shared_.transaction;
}

}; //end of namespace 'nsQuery'

//==================================================================================================

#define DBPROXY_DBQUERY_PROXY_NODE_INTER_INL

#include "./ProxyNodes/DBQueryProxyUserNode.h"
#include "./ProxyNodes/DBQueryProxyCharNode.h"

#undef DBPROXY_DBQUERY_PROXY_NODE_INTER_INL

//==================================================================================================

#endif //DBPROXY_DBQUERY_PROXY_NODE_H