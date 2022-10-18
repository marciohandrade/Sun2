#ifndef _POINTWALLET_POINTSYTEM_H
#define _POINTWALLET_POINTSYTEM_H

namespace PointSystem {

template<typename T>
class PointElement
{
    T value;

public:
    PointElement()
        : value(0)
    {
    };

    ~PointElement()
    {
    };

    void SetValue(T _value)
    {
        value = _value;
    };

    T GetValue() const
    {
        return value;
    };
};

template<typename T1, typename T2>
class PointNodeMap
{
    typedef STLX_HASH_MAP<T1, T2> NodeMap;
    NodeMap node_map_;

public:
    PointNodeMap()
    {
    }

    ~PointNodeMap()
    {
    }

    T2* find(T1 _key) const
    {
        const NodeMap::const_iterator it = node_map_.find(_key);
        if(it != node_map_.end())
            return const_cast<T2*>(&it->second);

        return NULL;
    }

    T2* insert(T1 _key)
    {
        T2* it = find(_key);
        if(it != NULL) 
            return it;
        
        node_map_.insert(NodeMap::value_type(_key, T2()));
        
        return find(_key);
    }

    void erase(T1 _key)
    {
        NodeMap::iterator it = node_map_.find(_key);
        if(it != node_map_.end())
            node_map_.erase(it);
    }

    void clear()
    {
        node_map_.clear();
    }
};

typedef PointElement<INT /*point*/> CodeNode;
typedef PointNodeMap<DWORD /*code*/, CodeNode> UserNode;
typedef PointNodeMap<DWORD /*uuid*/, UserNode> GuildNode;
typedef PointNodeMap<DWORD /*guid*/, GuildNode> PointSystem;

} //namespace PointSystem

#endif //_POINTWALLET_POINTSYTEM_H