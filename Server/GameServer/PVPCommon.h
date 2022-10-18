#pragma once
#ifndef GAMESERVER_PVP_COMMON_H
#define GAMESERVER_PVP_COMMON_H

//==================================================================================================
struct MSG_CG_PVP_INFO_CMD;

enum ePVP_USER_STATE//moved from PVPInfo.h
{
    ePVP_USER_NONE,//아무것도 아닌상태
    ePVP_USER_ENTERED,//들어온 상태
    ePVP_USER_KEYPRESSED,//키를 누르고 기다리는 상태
    ePVP_USER_MATCHLESS,//무적 상태
    ePVP_USER_MATCH,//PVP 상태
    ePVP_USER_DIE,//죽은 상태
};

struct PvPInfoContainer
{
    struct Node;
    static const int kNotFoundIndex = MAX_PARTYMEMBER_NUM;

    void Clear();

    int FindPvPControlIndex(
        DWORD player_key
        ) const;

    Player* FindPvPPlayer(
        DWORD object_key
        ) const;

    bool AddPvPControl(
        DWORD player_key, 
        BYTE team_no
        );

    bool MakeMessage(
        MSG_CG_PVP_INFO_CMD* msg_cmd, 
        const Node* enter_player_node
        );

    int number_of_members_;

    struct Node {
        void ChangeToEntered(Player* player);
        void ChangeToClosed();
        DWORD player_key_;
        bool entered_;//set after enter
        bool closed_;//set before leave or no more enter, to support shallow state control
        BYTE team_no_;
        DWORD entered_tick_;
        Player* player_;//if a real player entered, the value is not null.
    } nodes_[MAX_PARTYMEMBER_NUM];
};

inline void PvPInfoContainer::Clear()
{
    ZeroMemory(this, sizeof(*this));
}

inline int PvPInfoContainer::FindPvPControlIndex(DWORD player_key) const
{
    if (player_key == 0) 
    {
        return _countof(nodes_);
    }

    for (int find_index = 0; find_index < number_of_members_; ++find_index)
    {
        const Node* node = &nodes_[find_index];
        if (node->player_key_ == player_key) 
        {
            return find_index;
        }
    }
    return _countof(nodes_);
}

inline Player* PvPInfoContainer::FindPvPPlayer(DWORD object_key) const
{
    if (object_key == 0) 
    {
        return NULL;
    }

    for (int find_index = 0; find_index < number_of_members_; ++find_index)
    {
        const Node* node = &nodes_[find_index];
        if (node->player_key_ == object_key)
        {
            if (node->entered_ && node->closed_ == false) 
            {
                return node->player_;
            }
            return NULL;
        }
    }
    return NULL;
}

inline void PvPInfoContainer::Node::ChangeToEntered(Player* player)
{
    assert(entered_ == false);
    entered_ = true;
    entered_tick_ = GetTickCount();
    player_ = player;
}

inline void PvPInfoContainer::Node::ChangeToClosed()
{
    closed_ = true;
    player_ = NULL;
}

//==================================================================================================
/*
#if SUN_CODE_DELETE_RESERVED
//--------------------------------------------------------------------------------
//s PVP의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------
#include <MemoryPoolFactory.h>

template<class _Ct>
class PVPPOOL
{
public:
    enum { _MAX_ZONE_POOL_SIZE=1000, };
    PVPPOOL(){ m_Pool.Initialize(_MAX_ZONE_POOL_SIZE, 0, "PVPPOOL", AssertionLogger4Pool_Server); }
    ~PVPPOOL(){ m_Pool.Release();   }
    _Ct* Alloc() { return (_Ct*)m_Pool.Alloc(); }
    BOOL Free(_Ct* p) { return m_Pool.Free(p); }
private:
    util::CMemoryPoolFactory<_Ct> m_Pool;
};


#define __DECL_PVPPOOL(_ClassType)                                  \
    private:                                                        \
    static PVPPOOL<_ClassType> s_Pool;                              \
    public:                                                         \
    static _ClassType* Create() { return s_Pool.Alloc(); }         \
    static void Destroy(_ClassType* pClass) { s_Pool.Free(pClass); }

#define __IMPL_PVPPOOL(_ClassType)                                  \
    PVPPOOL<_ClassType> _ClassType::s_Pool;
//--------------------------------------------------------------------------------
//e 존의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------
#endif //SUN_CODE_DELETE_RESERVED
*/
#endif //GAMESERVER_PVP_COMMON_H